/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMDIGIDATA_H
#define CBMDIGIDATA_H 1

#include "CbmStsDigi.h"

#include <boost/serialization/access.hpp>

#include <vector>

#ifndef NO_ROOT
#include <Rtypes.h>  // for ClassDef
#endif


/** @struct CbmDigiVector
 ** @brief Digi collection in a std::vector
 **
 ** This is the simplest form of a collection of detector digis. A detector-specific
 ** implementation may choose a different representation (i.e., several vectors) or
 ** add meta-data.
 **/
template<class Digi>
struct CbmDigiVec {
  std::vector<Digi> fDigis;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fDigis;
  }
};


/** @struct CbmDigiData
 ** @brief Collection of digis from all detector systems
 **
 ** If no detector-specific collection class is provided, the simplest form (DigiVector)
 ** is used.
 **
 ** The current implementation uses STS only - to be expanded for all systems.
 **/
struct CbmDigiData {
  friend class boost::serialization::access;
  typedef CbmDigiVec<CbmStsDigi> StsDigiData;
  StsDigiData fSts;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fSts;
  }
#ifndef NO_ROOT
  ClassDefNV(CbmDigiData, 1);
#endif
};


#endif /* CBMDIGIDATA_H */
