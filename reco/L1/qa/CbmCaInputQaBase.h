/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmCaInputQaBase.h
/// \date   13.01.2023
/// \brief  Class providing basic functionality for CA input QA-tasks
/// \author S.Zharko <s.zharko@lx-pool.gsi.de>

#ifndef CbmCaInputQaBase_h
#define CbmCaInputQaBase_h 1

#include "Logger.h"

#include <array>

/// Class CbmCaInputQaBase contains common functionality for all of the CA input QA-tasks
///
class CbmCaInputQaBase {
public:
  /// Default constructor
  CbmCaInputQaBase() = default;

  /// Default destructor
  ~CbmCaInputQaBase() = default;


  /// Sets maximum allowed deviation of pull distribution width from unity
  /// \param devThrsh  Deviation threshold (1 - devThrsh, 1 + devThrsh)
  void SetPullWidthDeviationThrsh(double devThrsh) { fPullWidthThrsh = devThrsh; }

  /// Sets maximum allowed deviation of residual mean from zero (-devThrsh * rms, +devThrsh * rms)
  /// \param devThrsh  Deviation threshold in sigmas of residual distribution
  void SetResidualMeanDeviationThrsh(double devThrsh) { fResMeanThrsh = devThrsh; }

  /// Sets hit efficiency analysis parameters: efficiency fit range
  /// \param thrsh    Threshold for efficiency in a given range
  /// \param loRange  Lower bound of the integration range
  /// \param upRange  Upper limit of the integration range
  void SetEfficiencyThrsh(double thrsh, double loRange, double upRange)
  {
    LOG(info) << "Call: SetEfficiencyThrsh";
    fEffThrsh    = thrsh;
    fEffRange[0] = loRange;
    fEffRange[1] = upRange;
    LOG(info) << fEffRange[0] << ' ' << fEffRange[1];
  }

  /// Sets maximum allowed difference between z-position of hit and station
  /// \param  dz  Difference between station and hit position z components [cm]
  void SetMaxDiffZStationHit(double dz) { fMaxDiffZStHit = dz; }

  /// Sets lower momentum threshold
  /// \param mom Minimum absolute value of momentum
  void SetMinMomentum(double mom) { fMinMomentum = mom; }


protected:
  // *********************
  // ** Data structures **
  // *********************

  /// Structure to store fit residuals result
  struct ResidualFitResult {
    double mean = 0;  ///< mean of the distribution
    double lo   = 0;  ///< lower limit for the mean
    double hi   = 0;  ///< higher limit for the mean
  };

  // ----- QA constants (thresholds, ranges etc.)
  double fResMeanThrsh   = .5;   ///< Maximum allowed deviation of residual mean from zero [sigma]
  double fPullWidthThrsh = .1;   ///< Maximum allowed deviation of pull width from unity
  double fEffThrsh       = .5;   ///< Threshold for hit efficiency in the selected range
  double fMaxDiffZStHit  = 1.;   ///< Maximum allowed difference between z-position of hit and station [cm]
  double fMinMomentum    = .05;  ///< Minimum momentum of particle [GeV/c]

  std::array<double, 2> fEffRange = {0., 100.};  ///< Range for hit efficiency approximation
};

#endif  // CbmCaInputQaBase_h
