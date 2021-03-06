// Copyright (C) 2010 and later by various people
// see monotone commit logs for details and authors
//
// This program is made available under the GNU GPL version 2.0 or
// greater. See the accompanying file COPYING for details.
//
// This program is distributed WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.

#ifndef __CACHE_LOGGER_HH__
#define __CACHE_LOGGER_HH__

#include <memory>

class cache_logger_impl;

class cache_logger
{
  std::shared_ptr<cache_logger_impl> _impl;
  int max_size;
public:
  // if given the empty filename, do nothing
  explicit cache_logger(std::string const & filename, int max_size);

  bool logging() const { return static_cast<bool>(_impl); }

  void log_exists(bool exists, int position, int item_count, int est_size) const;
  void log_touch(bool exists, int position, int item_count, int est_size) const;
  void log_fetch(bool exists, int position, int item_count, int est_size) const;
  void log_insert(int items_removed, int item_count, int est_size) const;
};

#endif


// Local Variables:
// mode: C++
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
