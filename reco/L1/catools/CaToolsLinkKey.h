/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CaToolsLinkKey.h
/// \brief  Data structure to represent a MC link in CA tracking MC module
/// \since  25.11.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsLinkKey_h
#define CaToolsLinkKey_h 1

#include <boost/functional/hash.hpp>

namespace ca::tools
{
  struct LinkKey {
    /// Constructor from index, event and file of a link
    /// \param  index  Index of MC track/point in external data structure
    /// \param  event  Index of MC event
    /// \param  file   Index of MC file
    LinkKey(int index, int event, int file) : fIndex(index), fEvent(event), fFile(file) {}

    /// Comparison operator
    friend bool operator==(const LinkKey& lhs, const LinkKey& rhs)
    {
      return lhs.fFile == rhs.fFile && lhs.fEvent == rhs.fEvent && lhs.fIndex == rhs.fIndex;
    }

    int fIndex = -1;  ///< Index of MC point/track in external data structures
    int fEvent = -1;  ///< Index of MC event
    int fFile  = -1;  ///< Index of MC file
  };
}  // namespace ca::tools

namespace std
{
  /// A hash specialization for ca::tools::LinkKey objects
  template<>
  struct hash<ca::tools::LinkKey> {
    std::size_t operator()(const ca::tools::LinkKey& key) const
    {
      std::size_t res = 0;
      boost::hash_combine(res, key.fFile);
      boost::hash_combine(res, key.fEvent);
      boost::hash_combine(res, key.fIndex);
      return res;
    }
  };
}  // namespace std

#endif  // CaToolsLinkKey_h
