/** @file PairAnalysisStyleDefs.h
  * @copyright Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer], Etienne Bechtel **/

#ifndef PAIRANALYSISSTYLEDEFS_H
#define PAIRANALYSISSTYLEDEFS_H
///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
// PairAnalysis stylers                                                                    //
//                                                                                       //
//                                                                                       //
// Authors:                                                                              //
//   Julian Book <Julian.Book@cern.ch>                                                   //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>
#include <TAttMarker.h>
#include <TColor.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TStyle.h>

#include "PairAnalysisStyler.h"

namespace PairAnalysisStyler
{

  /* enum class Eidx  { kRaw=100, kBgrd, kSig, kFit, kCocktail, kNidx }; */
  /* enum class EfillMark { kOpenMarker=1, kFullMarker, kDontCare }; */
  /* enum class Estyle  { kNMaxMarker=13, kNMaxLine=4, kNMaxColor=17 }; */
  /* enum class Epalette  { kDefault=0, kGoodBad }; */

  static Int_t fCol[static_cast<Int_t>(PairAnalysisStyler::Eidx::kNidx)
                    - static_cast<Int_t>(PairAnalysisStyler::Eidx::kRaw)]    = {kBlack, kTeal - 8, kBlack, kTeal - 7,
                                                                             kTeal - 7};
  static Int_t fMrk[static_cast<Int_t>(PairAnalysisStyler::Eidx::kNidx)
                    - static_cast<Int_t>(PairAnalysisStyler::Eidx::kRaw)]    = {kFullCircle, kOpenCircle, kOpenCircle,
                                                                             kDot, kDot};
  static Double_t fSze[static_cast<Int_t>(PairAnalysisStyler::Eidx::kNidx)
                       - static_cast<Int_t>(PairAnalysisStyler::Eidx::kRaw)] = {1., 1., 1., 1., 1.};
  static Int_t fLne[static_cast<Int_t>(PairAnalysisStyler::Eidx::kNidx)
                    - static_cast<Int_t>(PairAnalysisStyler::Eidx::kRaw)]    = {kSolid, kSolid, kSolid, kSolid, kSolid};
  static Double_t fWdt[static_cast<Int_t>(PairAnalysisStyler::Eidx::kNidx)
                       - static_cast<Int_t>(PairAnalysisStyler::Eidx::kRaw)] = {2., 2., 2., 2., 2.};
  static Int_t fFll[static_cast<Int_t>(PairAnalysisStyler::Eidx::kNidx)
                    - static_cast<Int_t>(PairAnalysisStyler::Eidx::kRaw)]    = {0, 0, 0, 0, 0};  //kFEmpty

  static Int_t Marker[] = {kFullCircle, kFullDiamond, kFullSquare,  kFullCross,  kFullStar,  kMultiply, kPlus,
                           kStar,       kOpenCircle,  kOpenDiamond, kOpenSquare, kOpenCross, kOpenStar};  // kNMaxMarker

  static Int_t Line[] = {kSolid, kDashed, kDotted,
                         //			      9,
                         kDashDotted};  // kNMaxLine

  static Int_t Color[] = {kRed - 4,    kBlue - 4,   kBlack,       kGreen + 1,  kAzure + 1, kOrange + 2,
                          kSpring + 4, kViolet + 1, kOrange,      kGray + 1,   kRed + 2,   kCyan + 1,
                          kGreen + 3,  kBlue + 1,   kMagenta + 1, kOrange - 6, kCyan - 2};  // kNMaxColor

  static Int_t Fill[] = {-1};

  static UInt_t fLegAlign = 22;  // legend alignement (11,12,21,22)

  static TStyle* fUserDielStyle = nullptr;  // user defined style
}  // namespace PairAnalysisStyler

#endif
