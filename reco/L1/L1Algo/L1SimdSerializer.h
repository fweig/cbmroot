/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   L1SimdSerializer.h
/// \brief  External serialization for SIMD vector
/// \since  02.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef L1SimdSerializer_h
#define L1SimdSerializer_h 1

#include <boost/serialization/access.hpp>
#include <boost/serialization/split_free.hpp>

#include "CaSimd.h"
/// This header defines functionality for saving and loading SIMDized vectors. At the moment, only the first element of
/// the vector can be saved, and the loaded vector will be horizontally equal.
///
namespace boost
{
  namespace serialization
  {
    template<class Archive>
    void save(Archive& ar, const fvec& vect, unsigned int)
    {
      ar << vect[0];
    }

    template<class Archive>
    void load(Archive& ar, fvec& vect, unsigned int)
    {
      fscal buffer;
      ar >> buffer;
      for (size_t i = 0; i < fvec::size(); ++i) {
        vect[i] = buffer;
      }
    }

    template<class Archive>
    void serialize(Archive& ar, fvec& vect, const unsigned int version)
    {
      split_free(ar, vect, version);
    }
  }  // namespace serialization
}  // namespace boost

#endif  // L1SimdSerializer_h
