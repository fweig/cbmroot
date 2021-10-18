/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMDIGIEVENT_H
#define CBMDIGIEVENT_H 1

#include "CbmDigiData.h"

#include <boost/serialization/access.hpp>

#ifndef NO_ROOT
#include <Rtypes.h>  // for ClassDef
#endif

/** @struct CbmDigiEvent
 ** @brief Container of digis from all detectors in an event
 **/
struct CbmDigiEvent {
  CbmDigiData fData;  ///< Event data
  uint64_t fNumber;   ///< Event identifier
  double fTime;       ///< Event trigger time [ns]
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fData;
    ar& fNumber;
    ar& fTime;
  }
#ifndef NO_ROOT
  ClassDefNV(CbmDigiEvent, 1);
#endif
};

#endif /* CBMDIGIEVENT_H */
