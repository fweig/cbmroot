/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMTOFDIGIDATA_H
#define CBMTOFDIGIDATA_H 1


#include "CbmTofDigi.h"

#ifndef NO_ROOT
#include <Rtypes.h>  // for ClassDef
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

#include <vector>

/** @class CbmTofDigiData
 ** @brief Container class for CbmTofDigi objects
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 7.12.2022
 ** @version 1.0
 **
 ** Container class for transporting CbmTofDigi objects.
 ** The current implementation is the simplest one: a std::vector.
 **/
class CbmTofDigiData {

public:
  std::vector<CbmTofDigi> fDigis = {};  ///< Data vector

  friend class boost::serialization::access;

  /** @brief BOOST serializer**/
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fDigis;
  }

  /** @brief Clear content **/
  void Clear() { fDigis.clear(); }

  // --- ROOT serializer
#ifndef NO_ROOT
  ClassDefNV(CbmTofDigiData, 1);
#endif
};

#endif
