#ifndef PAIRANALYSISSTYLER_H
#define PAIRANALYSISSTYLER_H
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
#include <TH1.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TStyle.h>


namespace PairAnalysisStyler
{

  enum class Eidx
  {
    kRaw = 100,
    kBgrd,
    kSig,
    kFit,
    kCocktail,
    kNidx
  };
  enum class Epalette
  {
    kDefault = 0,
    kGoodBad
  };
  enum class EfillMark
  {
    kOpenMarker = 1,
    kFullMarker,
    kDontCare
  };
  enum class Estyle
  {
    kNMaxMarker = 13,
    kNMaxLine   = 4,
    kNMaxColor  = 17
  };

  void SetStyle(TStyle* userStyle);

  void SetStyle(Eidx idx, Int_t col = kBlack, Int_t marker = kOpenCircle, Double_t size = 1.5, Int_t line = kSolid,
                Double_t width = 2., Int_t fill = kFEmpty);

  void LoadStyle();
  void Style(TObject* obj, Int_t idx = 0);
  void SetForceLineStyle(Int_t line = kSolid);
  void SetForceColor(Int_t color = kBlack);
  void SetForceFillStyle(Int_t fill = kFSolid);

  void SetForceMarkerFillStyle(EfillMark fill);

  void SetPalette(Epalette colors = Epalette::kDefault, Bool_t reverse = kFALSE);

  void SetLegendAlign(UInt_t align);
  void SetLegendAttributes(TLegend* leg, Bool_t fill = kFALSE);

  TH1* GetFirstHistogram();
  TLegendEntry* GetLegendEntry(Int_t idx);

}  // namespace PairAnalysisStyler

#endif
