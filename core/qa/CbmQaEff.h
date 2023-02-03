/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmQaEff.h
/// \date   20.01.2023
/// \author S.Zharko <s.zharko@gsi.de>
/// \brief  Declaration of CbmQaEff class


#ifndef CbmQaEff_h
#define CbmQaEff_h 1

#include "CbmQaCanvas.h"

#include "Logger.h"

#include "TEfficiency.h"
#include "TFitResultPtr.h"
#include "TGraphAsymmErrors.h"
#include "TH2.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TVirtualPad.h"


/// Implementation of ROOT TEfficiency class, which adds handy functionality and improves fitting and drawing
///
class CbmQaEff : public TEfficiency {
public:
  /// Default constructor
  CbmQaEff();

  /// Copy constructor
  CbmQaEff(const CbmQaEff& other);

  /// Other constructors
  /// \tparam  Args  Variadic template parameter, which is passed to the constructor of base class
  template<typename... Args>
  CbmQaEff(Args... args);

  /// Destructor
  ~CbmQaEff() = default;

  /// Fit method reimplementation
  template<typename... Args>
  TFitResultPtr Fit(Args... args)
  {
    return TEfficiency::Fit(args...);
  }

  /// Integrates efficiency over the range
  /// \param  lo  Lower bound of integration range
  /// \param  hi  Higher bound of integration range
  /// \return Pointer to efficiency object, which contains only one point
  CbmQaEff* Integral(double lo, double hi);


  ClassDef(CbmQaEff, 1);
};

// **********************************************************
// **     Template and inline functions implementation     **
// **********************************************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename... Args>
CbmQaEff::CbmQaEff(Args... args) : TEfficiency(args...)
{
}

#endif  // CbmQaEff_h
