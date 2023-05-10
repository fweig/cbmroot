/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCmpDrawer.h
/// @brief  Class for a canvas with a comparison of multiple graphs or histograms (header)
/// @since  21.04.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#ifndef CbmQaCmpDrawer_h
#define CbmQaCmpDrawer_h 1

#include "Logger.h"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TLegend.h"
#include "TPad.h"

#include <algorithm>
#include <limits>
#include <type_traits>
#include <vector>

/// @brief  Class to draw a comparison of objects on the provided canvas or pad
/// @tparam Obj Type of underlying objects (TH1D, TGraph, TProfile, ...)
///
template<class Obj>
class CbmQaCmpDrawer {
public:
  /// @brief Default constructor
  CbmQaCmpDrawer();

  /// @brief Destructor
  ~CbmQaCmpDrawer() = default;

  /// @brief Copy constructor
  CbmQaCmpDrawer(const CbmQaCmpDrawer&) = delete;

  /// @brief Move constructor
  CbmQaCmpDrawer(CbmQaCmpDrawer&&) = delete;

  /// @brief Copy assignment operator
  CbmQaCmpDrawer& operator=(const CbmQaCmpDrawer&) = delete;

  /// @brief Move assignment operator
  CbmQaCmpDrawer& operator=(CbmQaCmpDrawer&&) = delete;

  /// @brief Clears the set of objects
  void Clear();

  /// @brief Registers an object to draw
  /// @param pObj  Pointer to object
  /// @param title Title of object (appears in the legend).
  void RegisterObject(const Obj* pObj, const char* title = nullptr);

  /// @brief Draw objects
  /// @param opt  Drawing option:
  ///             TODO: Specify options
  void Draw(Option_t* opt) const;

  /// @brief Sets minimum of the histogram/graph
  /// @param min  Minimum
  ///
  /// If the minimum is not set with this function, it will be set to 0 or the minimal value of the objects
  void SetMinimum(double min) { fMinimum = min; }

  /// @brief Sets maximum of the histogram/graph
  /// @param max  Maximum
  ///
  /// If the maximum is not set with this function, it will be set to the maximum value of the objects, multiplied
  /// by the value (1. + kIndentFromMax), which is 1.1 by default
  void SetMaximum(double max) { fMaximum = max; }

private:
  // Some constant expressions (TODO: move to a separate header)
  static constexpr double kLegEntryHight = 0.06;  ///< Hight of one legend entry in Y axis
  static constexpr double kLegEntryWidth = 0.35;  ///< Width of the legend
  static constexpr double kLegRightBound = 0.99;  ///< Right bound of the legend
  static constexpr double kLegTopBound   = 0.90;  ///< Top bound of the legend
  static constexpr double kLegTextSize   = 0.04;  ///< Text size of the legend entries
  static constexpr double kIndentFromMax = 0.1;   ///< Indent from the maximum (percentage from (max - min))

  double fMinimum = std::numeric_limits<double>::signaling_NaN();
  double fMaximum = std::numeric_limits<double>::signaling_NaN();

  std::vector<Obj*> fvpObjects;        ///< Set of objects to be drawn
  std::vector<TString> fvsLegEntries;  ///< Entries to legend
};


// ****************************
// **     Implementation     **
// ****************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<class Obj>
CbmQaCmpDrawer<Obj>::CbmQaCmpDrawer()
{
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<class Obj>
void CbmQaCmpDrawer<Obj>::Clear()
{
  fvpObjects.clear();
  fvsLegEntries.clear();
  fMinimum = std::numeric_limits<double>::signaling_NaN();
  fMaximum = std::numeric_limits<double>::signaling_NaN();
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<class Obj>
void CbmQaCmpDrawer<Obj>::RegisterObject(const Obj* pObj, const char* title)
{
  if (!pObj) {
    LOG(info) << "CbmQaCmpDrawer: attempt to register a nullptr (" << title << "), ignored";
    return;
  }

  // Add entry to objects array
  auto* pObjClone = static_cast<Obj*>(pObj->Clone());
  fvpObjects.push_back(pObjClone);

  // Add entry to legend
  TString legEntryName = title ? title : pObj->GetTitle();
  fvsLegEntries.push_back(legEntryName);
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<class Obj>
void CbmQaCmpDrawer<Obj>::Draw(Option_t* /*opt*/) const
{
  // TH1, TProfile
  if constexpr (std::is_base_of_v<TH1, Obj> && !std::is_base_of_v<TH2, Obj> && !std::is_base_of_v<TH3, Obj>) {
    // Maximum and minimum
    double maxY = fMaximum;
    if (std::isnan(maxY)) {
      auto HistMax = [](const Obj* lhs, const Obj* rhs) { return lhs->GetMaximum() < rhs->GetMaximum(); };
      maxY         = (*std::max_element(fvpObjects.cbegin(), fvpObjects.cend(), HistMax))->GetMaximum();
      maxY *= (1. + kIndentFromMax);
    }
    double minY = fMinimum;
    if (std::isnan(minY)) {
      auto HistMin = [](const Obj* lhs, const Obj* rhs) { return lhs->GetMinimum() < rhs->GetMinimum(); };
      minY         = (*std::min_element(fvpObjects.cbegin(), fvpObjects.cend(), HistMin))->GetMaximum();
      minY         = (minY < 0.) ? minY : 0.;
    }

    // Draw a stack of histograms
    for (auto it = fvpObjects.begin(); it != fvpObjects.end(); ++it) {
      (*it)->SetStats(false);
      (*it)->SetMinimum(minY);
      (*it)->SetMaximum(maxY);
      (*it)->Draw(it == fvpObjects.begin() ? "" : "same");
    }
  }
  else if constexpr (std::is_base_of_v<TH2, Obj> || std::is_base_of_v<TH3, Obj>) {
    int nObjects = fvpObjects.size();
    int nRows    = static_cast<int>(std::sqrt(nObjects));
    int nCols    = nObjects / nRows;
    if (nCols * nRows < nObjects) { ++nCols; }
    gPad->Divide(nCols, nRows);

    for (auto it = fvpObjects.begin(); it != fvpObjects.end(); ++it) {
      if constexpr (std::is_base_of_v<TH2, Obj>) { (*it)->Draw("colz"); }
      else {
        (*it)->Draw();
      }
    }
    return;  // do not draw legend
  }

  // Draw legend
  double legX0 = kLegRightBound - kLegEntryWidth;
  double legX1 = kLegRightBound;
  double legY0 = kLegTopBound - kLegEntryHight * fvpObjects.size();
  double legY1 = kLegTopBound;

  auto* pLeg = new TLegend(legX0, legY0, legX1, legY1);
  pLeg->SetTextSize(kLegTextSize);
  for (int iObj = 0; iObj < (int) fvpObjects.size(); ++iObj) {
    pLeg->AddEntry(fvpObjects[iObj], fvsLegEntries[iObj], "lp");
  }
  pLeg->Draw();
}

#endif  // CbmQaCmpDrawer_h
