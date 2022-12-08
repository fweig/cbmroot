/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMPSDDIGIDATA_H
#define CBMPSDDIGIDATA_H 1


#include "CbmPsdDigi.h"

#ifndef NO_ROOT
#include <Rtypes.h>  // for ClassDef
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <vector>

/** @class CbmPsdDigiData
 ** @brief Container class for CbmPsdDigi objects
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 7.12.2022
 ** @version 1.0
 **
 ** Container class for transporting CbmPsdDigi objects.
 ** The current implementation is the simplest one: a std::vector.
 **/
class CbmPsdDigiData {

public:
  std::vector<CbmPsdDigi> fDigis = {};  ///< Data vector

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
  ClassDefNV(CbmPsdDigiData, 1);
#endif
};

#endif
