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
#include "TPaveText.h"
#include "TStyle.h"
#include "TVirtualPad.h"

#include <tuple>

/// Implementation of ROOT TEfficiency class, which adds handy functionality and improves fitting and drawing
///
class CbmQaEff : public TEfficiency {
public:
  static constexpr int kMarkerStyle = 20;

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

  void Paint(Option_t* opt)
  {
    TEfficiency::Paint(opt);
    SetStats();
    if (fpStats) { fpStats->Draw(); }
  }

  void Draw(Option_t* opt = "")
  {
    if (GetDimension() == 1) { this->SetMarkerStyle(kMarkerStyle); }
    TEfficiency::Draw(opt);
  }

  /// Draws copy of the object
  /// \param opt     Option string
  /// \param postfix Postfix of the name
  CbmQaEff* DrawCopy(Option_t* opt, const char* postfix = "_copy") const;

  /// Gets integrated efficiency in a selected range
  /// \note Works only for 1D efficiency
  /// \param  lo  Lower bound of integration range
  /// \param  hi  Higher bound of integration range
  /// \return Pointer to efficiency object, which contains only one point
  CbmQaEff* Integral(double lo, double hi);

  /// Gets total integrated efficiency
  /// \return A tuple:
  ///         - 0: value
  ///         - 1: lower error
  ///         - 2: upper error
  std::tuple<double, double, double> GetTotalEfficiency() const;

  /// @brief Sets statistics box for efficiency
  void SetStats();

  ClassDef(CbmQaEff, 1);

private:
  TPaveText* fpStats = nullptr;  ///< Statistics box

  // Functions of the base class, using which can produce inconsistent results
  using TEfficiency::SetBetaBinParameters;
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
