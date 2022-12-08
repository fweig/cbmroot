/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMSTSDIGIDATA_H
#define CBMSTSDIGIDATA_H 1


#include "CbmStsDigi.h"

#ifndef NO_ROOT
#include <Rtypes.h>  // for ClassDef
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <vector>

/** @class CbmStsDigiData
 ** @brief Container class for CbmStsDigi objects
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 7.12.2022
 ** @version 1.0
 **
 ** Container class for transporting CnmStsDigi objects.
 ** The current implementation is the simplest one: a std::vector.
 **/
class CbmStsDigiData {

public:
  std::vector<CbmStsDigi> fDigis = {};  ///< Data vector

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
  ClassDefNV(CbmStsDigiData, 1);
#endif
};

#endif
