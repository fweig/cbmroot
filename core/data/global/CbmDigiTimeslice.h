/* Copyright (C) 2021-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMDIGITIMESLICE_H
#define CBMDIGITIMESLICE_H 1

#include "CbmDigiData.h"

#include "TimesliceDescriptor.hpp"

#include <boost/serialization/access.hpp>


/** @class CbmDigiTimeslice
 ** @brief Collection of digis from all detector systems within one timeslice
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 7.12.2022
 ** @version 1.0
 **/
class CbmDigiTimeslice {

public:
  CbmDigiData fData;                ///< Timeslice data
  fles::TimesliceDescriptor fDesc;  ///< Timeslice descriptor (metadata)

  friend class boost::serialization::access;
  /** @brief BOOST serializer**/
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fData;
    ar& fDesc;
  }

  /** @brief Clear content **/
  void Clear()
  {
    fData.Clear();
    fDesc = fles::TimesliceDescriptor();
  }
};

#endif /* CBMDIGITIMESLICE_H */
