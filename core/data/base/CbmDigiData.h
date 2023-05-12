/* Copyright (C) 2021-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMDIGIDATA_H
#define CBMDIGIDATA_H 1

#include "CbmBmonDigiData.h"
#include "CbmMuchDigiData.h"
#include "CbmPsdDigiData.h"
#include "CbmRichDigiData.h"
#include "CbmStsDigiData.h"
#include "CbmTofDigiData.h"
#include "CbmTrdDigiData.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#ifndef NO_ROOT
#include <Rtypes.h>  // for ClassDef
#endif


/** @class CbmDigiData
 ** @brief Collection of digis from all detector systems
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 7.12.2022
 ** @version 2.0
 **/
class CbmDigiData {

public:
  CbmBmonDigiData fT0;    ///< Beam monitor data
  CbmStsDigiData fSts;    ///< STS data
  CbmMuchDigiData fMuch;  ///< MUCH data
  CbmRichDigiData fRich;  ///< RICH data
  CbmTrdDigiData fTrd;    ///< TRD data
  CbmTrdDigiData fTrd2d;  ///< TRD2D data
  CbmTofDigiData fTof;    ///< TOF data
  CbmPsdDigiData fPsd;    ///< PSD data

  friend class boost::serialization::access;
  /** @brief BOOST serializer**/
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    // note, version is always the latest when saving
    ar& fT0;
    ar& fSts;
    ar& fMuch;
    ar& fTrd;
    ar& fTrd2d;
    ar& fTof;
    ar& fPsd;
    ar& fRich;
  }

  // --- ROOT serializer
#ifndef NO_ROOT
  ClassDefNV(CbmDigiData, 3);
#endif

  /** @brief Clear content **/
  void Clear()
  {
    fT0.Clear();
    fSts.Clear();
    fMuch.Clear();
    fTrd.Clear();
    fTrd2d.Clear();
    fTof.Clear();
    fPsd.Clear();
    fRich.Clear();
  }
};

BOOST_CLASS_VERSION(CbmDigiData, 4)
#endif /* CBMDIGIDATA_H */
