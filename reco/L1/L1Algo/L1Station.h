/* Copyright (C) 2007-2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Igor Kulakov */

#ifndef L1Station_h
#define L1Station_h 1

#include "L1Field.h"
#include "L1MaterialInfo.h"
#include "L1UMeasurementInfo.h"
#include "L1XYMeasurementInfo.h"


#define L1Station_h_TMP_DEBUG 1

#ifdef L1Station_h_TMP_DEBUG
#include <iomanip>
#endif  // L1Station_h_TMP_DEBUG

class L1Station {

public:
  // TODO: rewrite initialization (default values to fields, remove constructor (check struct))
  //L1Station()
  //  : type(0)
  //  , timeInfo(0)
  //  , z(0)
  //  , Rmin(0)
  //  , Rmax(0)
  //  , Sy(0)
  //  ,  /// z-position, min & max station radius, field integral
  //  materialInfo()
  //  , fieldSlice()
  //  , frontInfo()
  //  , backInfo()
  //  , xInfo()
  //  , yInfo()
  //  , XYInfo()
  //{
  //}

  // TODO: test this constructors
  //L1Station(const L1Station &) = default;
  //L1Station & operator=(const L1Station &) = default;
  //L1Station(L1Station &&) = default;
  //L1Station & operator=(L1Station &&) = default;

  int type {0};
  int timeInfo {0};
  fvec z {0};
  fvec Rmin {0};
  fvec Rmax {0};
  fvec Sy {0};
  L1MaterialInfo materialInfo {};
  L1FieldSlice fieldSlice {};
  L1UMeasurementInfo frontInfo {};
  L1UMeasurementInfo backInfo {};
  L1UMeasurementInfo xInfo {};
  L1UMeasurementInfo yInfo {};
  L1XYMeasurementInfo XYInfo {};


  void Print() const
  {
#ifdef L1Station_h_TMP_DEBUG
    LOG(info) << "==== L1Station object at " << this;
    LOG(info) << "- L1Station fields:";
    LOG(info) << "--- Station type ID:             " << type;
    LOG(info) << "--- z position:                  " << z[0];
    LOG(info) << "--- Rmin:                        " << Rmin[0];
    LOG(info) << "--- Rmax:                        " << Rmax[0];
    LOG(info) << "--- Thickness (X), cm:           " << materialInfo.thick[0];
    LOG(info) << "--- Radiational length (X0), cm: " << materialInfo.RL[0];
    LOG(info) << "--- X / X0:                      " << materialInfo.RadThick[0];
    LOG(info) << "--- log(X / X0):                 " << materialInfo.logRadThick[0];
    LOG(info) << "--- Field approximation coefficients:";
    LOG(info) << "      idx         CX         CY         CZ";
    for (int idx = 0; idx < 21; ++idx) {
      LOG(info) << std::setw(9) << std::setfill(' ') << idx << ' ' << std::setw(10) << std::setfill(' ')
                << fieldSlice.cx[idx][0] << ' ' << std::setw(10) << std::setfill(' ') << fieldSlice.cy[idx][0] << ' '
                << std::setw(10) << std::setfill(' ') << fieldSlice.cz[idx][0];
    }
    LOG(info) << "--- Strips geometry:";
    LOG(info) << "----- Front:";
    LOG(info) << "------- cos(phi):            " << frontInfo.cos_phi[0];
    LOG(info) << "------- sin(phi):            " << frontInfo.sin_phi[0];
    LOG(info) << "------- sigma2:              " << frontInfo.sigma2[0];
    LOG(info) << "----- Back:";
    LOG(info) << "------- cos(phi):            " << backInfo.cos_phi[0];
    LOG(info) << "------- sin(phi):            " << backInfo.sin_phi[0];
    LOG(info) << "------- sigma2:              " << backInfo.sigma2[0];
    LOG(info) << "----- XY cov matrix:";
    LOG(info) << "------- C00:                 " << XYInfo.C00[0];
    LOG(info) << "------- C10:                 " << XYInfo.C10[0];
    LOG(info) << "------- C11:                 " << XYInfo.C11[0];
    LOG(info) << "----- X layer:";
    LOG(info) << "------- cos(phi):            " << xInfo.cos_phi[0];
    LOG(info) << "------- sin(phi):            " << xInfo.sin_phi[0];
    LOG(info) << "------- sigma2:              " << xInfo.sigma2[0];
    LOG(info) << "----- Y layer:";
    LOG(info) << "------- cos(phi):            " << yInfo.cos_phi[0];
    LOG(info) << "------- sin(phi):            " << yInfo.sin_phi[0];
    LOG(info) << "------- sigma2:              " << yInfo.sigma2[0];
    LOG(info) << "";
#endif  // L1Station_h_TMP_DEBUG
  }

} _fvecalignment;

#endif
