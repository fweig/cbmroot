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

#include "vectors/L1vec.h"

namespace boost
{
  namespace serialization
  {
    template<class Archive>
    void serialize(Archive& ar, fvec& vect, const unsigned int)
    {
      for (size_t i = 0; i < fvec::size(); ++i) {
        ar&(reinterpret_cast<fscal*>(&vect))[i];
      }
    }
  }  // namespace serialization
}  // namespace boost

#endif  // L1SimdSerializer_h
