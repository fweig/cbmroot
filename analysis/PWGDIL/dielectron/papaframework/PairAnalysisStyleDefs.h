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


#include <TStyle.h>
#include <TAttMarker.h>
#include <TColor.h>
#include <Rtypes.h>
#include <TLegend.h>
#include <TLegendEntry.h>

namespace PairAnalysisStyler
{

//  enum Eidx : Int_t { kRaw=100, kBgrd, kSig, kFit, kCocktail, kNidx };
//  enum EfillMark : Int_t { kOpenMarker=1, kFullMarker, kDontCare };
//  enum Estyle : Int_t { kNMaxMarker=13, kNMaxLine=4, kNMaxColor=17 };
//  enum Epalette : Int_t { kDefault=0, kGoodBad };

  static Int_t    fCol[kNidx-kRaw]={kBlack,kTeal-8,kBlack,kTeal-7,kTeal-7};
  static Int_t    fMrk[kNidx-kRaw]={kFullCircle,kOpenCircle,kOpenCircle,kDot,kDot};
  static Double_t fSze[kNidx-kRaw]={1.,1.,1.,1.,1.};
  static Int_t    fLne[kNidx-kRaw]={kSolid,kSolid,kSolid,kSolid,kSolid};
  static Double_t fWdt[kNidx-kRaw]={2.,2.,2.,2.,2.};
  static Int_t    fFll[kNidx-kRaw]={0,0,0,0,0}; //kFEmpty

  static Int_t Marker[]= {kFullCircle,
			  kFullDiamond,
			  kFullSquare,
			  kFullCross,
			  kFullStar,
			  kMultiply,
			  kPlus,
			  kStar,
			  kOpenCircle,
			  kOpenDiamond,
			  kOpenSquare,
			  kOpenCross,
			  kOpenStar}; // kNMaxMarker

  static Int_t Line[]= {kSolid,
			kDashed,
			kDotted,
			//			      9,
			kDashDotted }; // kNMaxLine

  static Int_t Color[]= {kRed-4
			 ,kBlue-4
			 ,kBlack
			 ,kGreen+1
			 ,kAzure+1
			 ,kOrange+2
			 ,kSpring+4
			 ,kViolet+1
			 ,kOrange
			 ,kGray+1
			 ,kRed+2
			 ,kCyan+1
			 ,kGreen+3
			 ,kBlue+1
			 ,kMagenta+1
			 ,kOrange-6
			 ,kCyan-2
  }; // kNMaxColor

  static Int_t Fill[]= {-1};

  static UInt_t fLegAlign = 22;           // legend alignement (11,12,21,22)

  static TStyle *fUserDielStyle = nullptr;   // user defined style
} 

#endif
