/* Copyright (C) 2007-2020 St. Petersburg Polytechnic University, St. Petersburg
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Evgeny Kryshen, Andrey Lebedev, Mikhail Ryzhinskiy [committer] */

/**
 * \file CbmMuchCluster.h
 * \author Evgeny Kryshen <e.kryshen@gsi.de>
 * \date 13.08.12
 * \version 2.0
 * \brief Data container for MUCH clusters.
 **/
#ifndef CBMMUCHCLUSTER_H
#define CBMMUCHCLUSTER_H 1

#include "CbmCluster.h"  // for CbmCluster

#include <Rtypes.h>  // for ClassDef

/**
 * \class CbmMuchCluster
 * \author Evgeny Kryshen <e.kryshen@gsi.de>
 * \brief Data container for MUCH clusters.
 */
class CbmMuchCluster : public CbmCluster {
public:
  /**
   * \brief Default constructor.
   */
  CbmMuchCluster();

  /**
   * \brief Destructor.
   */
  virtual ~CbmMuchCluster();

  ClassDef(CbmMuchCluster, 2)
};

#endif
