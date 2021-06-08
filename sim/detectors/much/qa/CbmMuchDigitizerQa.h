/* Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Eugeny Kryshen, Dominik Smith [committer], Sergey Gorbunov, Vikas Singhal, Ekata Nandy */

/*** \brief  Definition of the CbmMuchDigitizerQa class
  * \date   25.09.2020 **/

#ifndef CbmMuchDigitizerQa_H
#define CbmMuchDigitizerQa_H

#include "CbmLink.h"
#include "CbmMuchPointInfo.h"

#include "FairTask.h"

#include "TFolder.h"
#include "TParameter.h"

#include <map>
#include <vector>

class TBuffer;
class TClass;
class TClonesArray;
class TF1;
class TMemberInspector;
class CbmMuchGeoScheme;
class CbmDigiManager;
class CbmQaCanvas;
class TCanvas;
class TH1F;
class TH2F;
class TVector2;
class CbmMuchPad;
class CbmMCDataArray;
class CbmMCDataManager;
class CbmTimeSlice;

/// QA for the MUCH detector after a "digitization" step of the simulation.
/// The class reimplements corresponding QA checks from old CbmMuchHitFinderQa class
/// made by E. Kryshen & V. Singhal & E. Nandy
///
class CbmMuchDigitizerQa : public FairTask {

public:
  CbmMuchDigitizerQa(const char* name = "MuchHitFinderQa", Int_t verbose = 1);
  virtual ~CbmMuchDigitizerQa();
  virtual InitStatus Init();
  virtual void Exec(Option_t* option);
  virtual void Finish();
  virtual void SetParContainers();

  /// Prepare Qa output and return it as a reference to an internal folder.
  /// The reference is non-const, because FairSink can not write const objects
  TFolder& GetQa();

private:
  CbmMuchDigitizerQa(const CbmMuchDigitizerQa&);
  CbmMuchDigitizerQa& operator=(const CbmMuchDigitizerQa&);

  static Double_t LandauMPV(Double_t* x, Double_t* par);
  static Double_t MPV_n_e(Double_t Tkin, Double_t mass);

  /// Occupance analysis - all pads,fired pads,
  /// and fired/all distributions as functions of radius
  ///
  void OccupancyQa();

  /// get pad from the digi address
  const CbmMuchPad* GetPad(UInt_t address) const;

  void InitChargeHistos();
  void InitPadHistos();
  void InitLengthHistos();
  int InitChannelPadInfo();
  void InitFits();
  void InitCanvases();
  void DeInit();

  int CheckConsistency();
  int ProcessMCPoints();
  void FillTotalPadsHistos();
  void FillChargePerPoint();
  void FillDigitizerPerformancePlots();
  void PrintFrontLayerPoints();
  void PrintFrontLayerDigis();

  void DrawChargeCanvases();
  void DrawPadCanvases();
  void DrawLengthCanvases();
  void OutputNvsS();

  TFolder* histFolder;  /// folder wich contains histogramms

  // setup
  FairRootManager* fManager    = nullptr;
  CbmMCDataManager* fMcManager = nullptr;
  CbmTimeSlice* fTimeSlice     = nullptr;

  // geometry
  CbmMuchGeoScheme* fGeoScheme = nullptr;
  Int_t fNstations             = 0;
  CbmDigiManager* fDigiManager = nullptr;

  // containers
  CbmMCDataArray* fPoints    = nullptr;
  CbmMCDataArray* fMCTracks  = nullptr;
  TClonesArray* fDigis       = nullptr;
  TClonesArray* fDigiMatches = nullptr;

  std::map<CbmLink, CbmMuchPointInfo> fMcPointInfoMap = {};  //! map point link -> point info

  CbmMuchPointInfo& getPointInfo(const CbmLink& link)
  {
    assert(fMcPointInfoMap.find(link) != fMcPointInfoMap.end());
    return fMcPointInfoMap[link];
  }

  TFolder fOutFolder;        /// output folder with histos and canvases
  TParameter<int> fNevents;  /// number of processed events

  // internal unscaled histograms, need to be scaled at the output
  std::vector<TH1F*> fvUsPadsFiredR;   // fired pads vs R, per station
  std::vector<TH2F*> fvUsPadsFiredXY;  // fired pads vs XY, per station

  // output histograms
  TH1F* fhTrackCharge            = nullptr;  /// MC point charge
  TH1F* fhTrackChargeLog         = nullptr;  /// MC point charge log scale
  TH1F* fhTrackChargePr_1GeV_3mm = nullptr;  /// MC point charge for selected protons

  TH1F* fhTrackLength   = nullptr;
  TH1F* fhTrackLengthPi = nullptr;
  TH1F* fhTrackLengthPr = nullptr;
  TH1F* fhTrackLengthEl = nullptr;

  TH2F* fhTrackChargeVsTrackEnergyLog   = nullptr;
  TH2F* fhTrackChargeVsTrackEnergyLogPi = nullptr;
  TH2F* fhTrackChargeVsTrackEnergyLogPr = nullptr;
  TH2F* fhTrackChargeVsTrackEnergyLogEl = nullptr;
  TH2F* fhTrackChargeVsTrackLength      = nullptr;
  TH2F* fhTrackChargeVsTrackLengthPi    = nullptr;
  TH2F* fhTrackChargeVsTrackLengthPr    = nullptr;
  TH2F* fhTrackChargeVsTrackLengthEl    = nullptr;
  TH2F* fhNpadsVsS                      = nullptr;

  std::vector<TH1F*> fvTrackCharge;    // MC point charge per station
  std::vector<TH1F*> fvPadsTotalR;     // number of pads vs R, per station
  std::vector<TH1F*> fvPadsFiredR;     // fired pads vs R, per station
  std::vector<TH1F*> fvPadOccupancyR;  // pad occupancy vs R, per station

  // output canvaces with histogramm collections
  CbmQaCanvas* fCanvCharge         = nullptr;
  CbmQaCanvas* fCanvStationCharge  = nullptr;
  CbmQaCanvas* fCanvChargeVsEnergy = nullptr;
  CbmQaCanvas* fCanvChargeVsLength = nullptr;
  CbmQaCanvas* fCanvTrackLength    = nullptr;
  CbmQaCanvas* fCanvNpadsVsArea    = nullptr;
  CbmQaCanvas* fCanvUsPadsFiredXY  = nullptr;
  CbmQaCanvas* fCanvPadOccupancyR  = nullptr;
  CbmQaCanvas* fCanvPadsTotalR     = nullptr;

  TF1* fFitEl = nullptr;
  TF1* fFitPi = nullptr;
  TF1* fFitPr = nullptr;

  Int_t fnPadSizesX = 0;
  Int_t fnPadSizesY = 0;

  Double_t fPadMinLx = 0.;
  Double_t fPadMinLy = 0.;
  Double_t fPadMaxLx = 0.;
  Double_t fPadMaxLy = 0.;

  ClassDef(CbmMuchDigitizerQa, 0)
};

#endif
