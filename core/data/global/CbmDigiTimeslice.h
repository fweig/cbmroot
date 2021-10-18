/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMDIGITIMESLICE_H
#define CBMDIGITIMESLICE_H 1

#include "CbmDigiData.h"

#include "TimesliceDescriptor.hpp"

#include <boost/serialization/access.hpp>


/** @struct CbmDigiTimeslice
 ** @brief Container of digis from all detectors in a time slice
 **/
struct CbmDigiTimeslice {
  CbmDigiData fData;
  fles::TimesliceDescriptor fDesc;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fData;
    ar& fDesc;
  }
};

#endif /* CBMDIGITIMESLICE_H */
