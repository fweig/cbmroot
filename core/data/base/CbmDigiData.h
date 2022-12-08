/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMDIGIDATA_H
#define CBMDIGIDATA_H 1

#include "CbmMuchDigi.h"
#include "CbmMuchDigiData.h"
#include "CbmPsdDigi.h"
#include "CbmPsdDigiData.h"
#include "CbmRichDigi.h"
#include "CbmRichDigiData.h"
#include "CbmStsDigi.h"
#include "CbmStsDigiData.h"
#include "CbmTofDigi.h"
#include "CbmTofDigiData.h"
#include "CbmTrdDigi.h"
#include "CbmTrdDigiData.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include <vector>


/** @struct DigiVec
 ** @brief Digi collection in a std::vector
 **
 ** This is the simplest form of a collection of detector digis. A detector-specific
 ** implementation may choose a different representation (i.e., several vectors) or
 ** add meta-data.
 **/
template<class Digi>
struct DigiVec {
  std::vector<Digi> fDigis;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fDigis;
  }
  void Clear() { fDigis.clear(); }
};


/** @struct CbmDigiData
 ** @brief Collection of digis from all detector systems
 **
 ** If no detector-specific collection class is provided, the simplest form (DigiVector)
 ** is used.
 **/
struct CbmDigiData {
  friend class boost::serialization::access;
  CbmTofDigiData fT0;
  CbmStsDigiData fSts;
  CbmMuchDigiData fMuch;
  CbmRichDigiData fRich;
  CbmTrdDigiData fTrd;
  CbmTofDigiData fTof;
  CbmPsdDigiData fPsd;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fT0;
    ar& fSts;
    ar& fMuch;
    ar& fTrd;
    ar& fTof;
    ar& fPsd;
    ar& fRich;
  }
  void Clear()
  {
    fT0.Clear();
    fSts.Clear();
    fMuch.Clear();
    fTrd.Clear();
    fTof.Clear();
    fPsd.Clear();
    fRich.Clear();
  }
};


#endif /* CBMDIGIDATA_H */
