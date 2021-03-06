// Copyright (C) 2005 Derek Scherger <derek@echologic.com>
//
// This program is made available under the GNU GPL version 2.0 or
// greater. See the accompanying file COPYING for details.
//
// This program is distributed WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.

#include "base.hh"
#include "safe_map.hh"
#include "vector.hh"
#include "restrictions.hh"
#include "file_io.hh"
#include "roster.hh"
#include "database.hh" // for parent_roster

using std::make_pair;
using std::map;
using std::set;
using std::vector;

// TODO: add check for relevant rosters to be used by log
//
// i.e.  as log goes back through older and older rosters it may hit one
// that pre-dates any of the nodes in the restriction. the nodes that the
// restriction includes or excludes may not have been born in a sufficiently
// old roster. at this point log should stop because no earlier roster will
// include these nodes.

typedef map<node_id, restricted_path::status>::const_iterator
        node_status_iterator;

typedef map<file_path, restricted_path::status>::const_iterator
        path_status_iterator;

typedef set<file_path>::const_iterator path_iterator;

static void
add_parents(map<node_id, restricted_path::status> & node_map,
            roster_t const & roster)
{
  set<node_id> parents;

  // accumulate the set of parents of all included nodes
  for (node_status_iterator i = node_map.begin(); i != node_map.end(); ++i)
    {
      if (i->second == restricted_path::included && roster.has_node(i->first))
        {
          node_id parent = roster.get_node(i->first)->parent;
          while (!null_node(parent) && parents.find(parent) == parents.end())
            {
              parents.insert(parent);
              parent = roster.get_node(parent)->parent;
            }
        }
    }

  // add implicit includes for all required parents
  for (set<node_id>::const_iterator i = parents.begin();
       i != parents.end(); ++i)
    {
      node_status_iterator n = node_map.find(*i);
      if (n == node_map.end())
        {
          file_path fp;
          roster.get_name(*i, fp);
          L(FL("including missing parent '%s'") % fp);
          node_map[*i] = restricted_path::required;
        }
      else if (n->second == restricted_path::included)
        {
          node_map[*i] = restricted_path::included_required;
        }
      else if (n->second == restricted_path::excluded)
        {
          file_path fp;
          roster.get_name(*i, fp);
          W(F("including required parent '%s'") % fp);
          node_map[*i] = restricted_path::excluded_required;
        }
    }
}

static void
map_nodes(map<node_id, restricted_path::status> & node_map,
          roster_t const & roster,
          set<file_path> const & paths,
          set<file_path> & known_paths,
          restricted_path::status const status)
{
  for (path_iterator i = paths.begin(); i != paths.end(); ++i)
    {
      if (roster.has_node(*i))
        {
          known_paths.insert(*i);
          node_id nid = roster.get_node(*i)->self;

          node_status_iterator n = node_map.find(nid);
          if (n != node_map.end())
            E(n->second == status, origin::user,
              F("conflicting include/exclude on path '%s'") % *i);
          else
            node_map[nid] = status;

        }
    }
}

static void
map_nodes(map<node_id, restricted_path::status> & node_map,
          roster_t const & roster,
          set<file_path> const & included_paths,
          set<file_path> const & excluded_paths,
          set<file_path> & known_paths)
{
  map_nodes(node_map, roster, included_paths, known_paths,
            restricted_path::included);
  map_nodes(node_map, roster, excluded_paths, known_paths,
            restricted_path::excluded);
}

static void
add_parents(map<file_path, restricted_path::status> & path_map)
{
  set<file_path> parents;

  // accumulate the set of parents of all included paths
  for (path_status_iterator i = path_map.begin(); i != path_map.end(); ++i)
    {
      if (i->second == restricted_path::included)
        {
          file_path parent = i->first.dirname();
          while (!parent.empty() && parents.find(parent) == parents.end())
            {
              parents.insert(parent);
              parent = parent.dirname();
            }
          parents.insert(parent);
        }
    }

  // add implicit includes for all required parents
  for (set<file_path>::const_iterator i = parents.begin();
       i != parents.end(); ++i)
    {
      path_status_iterator p = path_map.find(*i);
      if (p == path_map.end())
        {
          L(FL("including missing parent '%s'") % *i);
          path_map[*i] = restricted_path::required;
        }
      else if (p->second == restricted_path::included)
        {
          path_map[*i] = restricted_path::included_required;
        }
      else if (p->second == restricted_path::excluded)
        {
          W(F("including required parent '%s'") % *i);
          path_map[*i] = restricted_path::excluded_required;
        }
    }

}

static void
map_paths(map<file_path, restricted_path::status> & path_map,
          set<file_path> const & paths,
          restricted_path::status const status)
{
  for (path_iterator i = paths.begin(); i != paths.end(); ++i)
    {
      path_status_iterator p = path_map.find(*i);
      if (p != path_map.end())
        E(p->second == status, origin::user,
          F("conflicting include/exclude on path '%s'") % *i);
      else
        path_map[*i] = status;
    }
}

namespace
{
  // ignored paths are allowed into the restriction but are not considered
  // invalid if they are found in none of the restriction's rosters
  struct unknown_unignored_node : public path_predicate<file_path>
  {
    explicit unknown_unignored_node(set<file_path> const & known_paths,
                                    path_predicate<file_path> const & ignore)
      : known_paths(known_paths), ignore_file(ignore)
    {}
    virtual bool operator()(file_path const & p) const
    {
      return (known_paths.find(p) == known_paths.end() && !ignore_file(p));
    }

  private:
    set<file_path> const & known_paths;
    path_predicate<file_path> const & ignore_file;
  };

  struct unknown_unignored_path : public path_predicate<file_path>
  {
    explicit unknown_unignored_path(path_predicate<file_path> const & ignore)
      : ignore_file(ignore)
    {}
    virtual bool operator()(file_path const & p) const
    {
      return !path_exists(p) && !ignore_file(p);
    }
  private:
    path_predicate<file_path> const & ignore_file;
  };
}

static void
validate_paths(set<file_path> const & included_paths,
               set<file_path> const & excluded_paths,
               path_predicate<file_path> const & is_unknown)
{
  int bad = 0;

  for (path_iterator i = included_paths.begin(); i != included_paths.end(); ++i)
    if (is_unknown(*i))
      {
        bad++;
        W(F("restriction includes unknown path '%s'") % *i);
      }

  for (path_iterator i = excluded_paths.begin(); i != excluded_paths.end(); ++i)
    if (is_unknown(*i))
      {
        bad++;
        W(F("restriction excludes unknown path '%s'") % *i);
      }

  E(bad == 0, origin::user,
    FP("%d unknown path", "%d unknown paths", bad) % bad);
}

restriction::restriction(vector<file_path> const & includes,
                         vector<file_path> const & excludes,
                         long depth)
  : included_paths(includes.begin(), includes.end()),
    excluded_paths(excludes.begin(), excludes.end()),
    depth(depth)
{}

node_restriction::node_restriction(vector<file_path> const & includes,
                                   vector<file_path> const & excludes,
                                   long depth,
                                   roster_t const & roster,
                                   path_predicate<file_path> const & ignore,
                                   include_rules const & rules)
  : restriction(includes, excludes, depth)
{
  map_nodes(node_map, roster, included_paths, excluded_paths, known_paths);

  if (rules == implicit_includes)
    add_parents(node_map, roster);

  validate_paths(included_paths, excluded_paths,
                 unknown_unignored_node(known_paths, ignore));
}

node_restriction::node_restriction(vector<file_path> const & includes,
                                   vector<file_path> const & excludes,
                                   long depth,
                                   roster_t const & roster1,
                                   roster_t const & roster2,
                                   path_predicate<file_path> const & ignore,
                                   include_rules const & rules)
  : restriction(includes, excludes, depth)
{
  map_nodes(node_map, roster1, included_paths, excluded_paths, known_paths);
  map_nodes(node_map, roster2, included_paths, excluded_paths, known_paths);

  if (rules == implicit_includes)
    {
      add_parents(node_map, roster1);
      add_parents(node_map, roster2);
    }

  validate_paths(included_paths, excluded_paths,
                 unknown_unignored_node(known_paths, ignore));
}

node_restriction::node_restriction(vector<file_path> const & includes,
                                   vector<file_path> const & excludes,
                                   long depth,
                                   parent_map const & rosters1,
                                   roster_t const & roster2,
                                   path_predicate<file_path> const & ignore,
                                   include_rules const & rules)
  : restriction(includes, excludes, depth)
{
  for (parent_map::const_iterator i = rosters1.begin();
       i != rosters1.end(); i++)
    map_nodes(node_map, parent_roster(i),
              included_paths, excluded_paths, known_paths);
  map_nodes(node_map, roster2, included_paths, excluded_paths, known_paths);

  if (rules == implicit_includes)
    {
      for (parent_map::const_iterator i = rosters1.begin();
           i != rosters1.end(); i++)
        add_parents(node_map, parent_roster(i));
      add_parents(node_map, roster2);
    }

  validate_paths(included_paths, excluded_paths,
                 unknown_unignored_node(known_paths, ignore));
}

path_restriction::path_restriction(vector<file_path> const & includes,
                                   vector<file_path> const & excludes,
                                   long depth,
                                   path_predicate<file_path> const & ignore)
  : restriction(includes, excludes, depth)
{
  map_paths(path_map, included_paths, restricted_path::included);
  map_paths(path_map, excluded_paths, restricted_path::excluded);
  add_parents(path_map);
  validate_paths(included_paths, excluded_paths,
                 unknown_unignored_path(ignore));
}

path_restriction::path_restriction(vector<file_path> const & includes,
                                   vector<file_path> const & excludes,
                                   long depth,
                                   path_restriction::skip_check_t)
  : restriction(includes, excludes, depth)
{
  map_paths(path_map, included_paths, restricted_path::included);
  map_paths(path_map, excluded_paths, restricted_path::excluded);
  add_parents(path_map);
}

bool
node_restriction::includes(roster_t const & roster, node_id nid) const
{
  MM(roster);
  I(roster.has_node(nid));

  file_path fp;
  roster.get_name(nid, fp);

  if (included_paths.empty() && excluded_paths.empty())
    {
      if (depth != -1)
        {
          int path_depth = fp.depth();
          if (path_depth <= depth)
            {
              L(FL("depth includes nid %d path '%s'") % nid % fp);
              return true;
            }
          else
            {
              L(FL("depth excludes nid %d path '%s'") % nid % fp);
              return false;
            }
        }
      else
        {
          // don't log this, we end up using rather a bit of cpu time just
          // in the logging code, for totally unrestricted operations.
          return true;
        }
    }

  node_id current = nid;
  int path_depth = 0;

  while (!null_node(current) && (depth == -1 || path_depth <= depth))
    {
      node_status_iterator r = node_map.find(current);

      if (r != node_map.end())
        {
          switch (r->second)
            {
            case restricted_path::included:
            case restricted_path::included_required:
              L(FL("explicit include of nid %d path '%s'") % current % fp);
              return true;

            case restricted_path::excluded:
              L(FL("explicit exclude of nid %d path '%s'") % current % fp);
              return false;

            case restricted_path::required:
            case restricted_path::excluded_required:
              if (path_depth == 0)
                {
                  L(FL("implicit include of nid %d path '%s'") % current % fp);
                  return true;
                }
              else
                {
                  return false;
                }
            }
        }

      const_node_t node = roster.get_node(current);
      current = node->parent;
      path_depth++;
    }

  if (included_paths.empty())
    {
      L(FL("default include of nid %d path '%s'")
        % nid % fp);
      return true;
    }
  else
    {
      L(FL("(debug) default exclude of nid %d path '%s'")
        % nid % fp);
      return false;
    }
}

bool
path_restriction::includes(file_path const & pth) const
{
  if (included_paths.empty() && excluded_paths.empty())
    {
      if (depth != -1)
        {
          int path_depth = pth.depth();
          if (path_depth <= depth)
            {
              L(FL("depth includes path '%s'") % pth);
              return true;
            }
          else
            {
              L(FL("depth excludes path '%s'") % pth);
              return false;
            }
        }
      else
        {
          L(FL("empty include of path '%s'") % pth);
          return true;
        }
    }

  int path_depth = 0;
  file_path fp = pth;
  while (depth == -1 || path_depth <= depth)
    {
      path_status_iterator r = path_map.find(fp);

      if (r != path_map.end())
        {
          switch (r->second)
            {
            case restricted_path::included:
            case restricted_path::included_required:
              L(FL("explicit include of path '%s'") % pth);
              return true;

            case restricted_path::excluded:
              L(FL("explicit exclude of path '%s'") % pth);
              return false;

            case restricted_path::required:
            case restricted_path::excluded_required:
              if (path_depth == 0)
                {
                  L(FL("implicit include of path '%s'") % pth);
                  return true;
                }
              else
                {
                  return false;
                }
            }
        }

      if (fp.empty())
        break;
      fp = fp.dirname();
      path_depth++;
    }

  if (included_paths.empty())
    {
      L(FL("default include of path '%s'") % pth);
      return true;
    }
  else
    {
      L(FL("default exclude of path '%s'") % pth);
      return false;
    }
}

// Local Variables:
// mode: C++
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
