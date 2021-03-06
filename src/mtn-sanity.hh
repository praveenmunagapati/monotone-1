// Copyright (C) 2006 Timothy Brownawell <tbrownaw@gmail.com>
//
// This program is made available under the GNU GPL version 2.0 or
// greater. See the accompanying file COPYING for details.
//
// This program is distributed WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.

#ifndef __MTN_SANITY_HH__
#define __MTN_SANITY_HH__

#include "sanity.hh"

struct mtn_sanity : public sanity
{
  mtn_sanity();
  ~mtn_sanity();
  void initialize(int, char **, char const *);

private:
  void inform_log(std::string const &msg);
  void inform_message(std::string const &msg);
  void inform_warning(std::string const &msg);
  void inform_error(std::string const &msg);
};

#endif

// Local Variables:
// mode: C++
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
