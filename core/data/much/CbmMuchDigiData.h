/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMMUCHDIGIDATA_H
#define CBMMUCHDIGIDATA_H 1


#include "CbmMuchDigi.h"

#ifndef NO_ROOT
#include <Rtypes.h>  // for ClassDef
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <vector>

/** @class CbmMuchDigiData
 ** @brief Container class for CbmMuchDigi objects
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 7.12.2022
 ** @version 1.0
 **
 ** Container class for transporting CbmMuchDigi objects.
 ** The current implementation is the simplest one: a std::vector.
 **/
class CbmMuchDigiData {

public:
  std::vector<CbmMuchDigi> fDigis = {};  ///< Data vector

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
  ClassDefNV(CbmMuchDigiData, 1);
#endif
};

#endif
