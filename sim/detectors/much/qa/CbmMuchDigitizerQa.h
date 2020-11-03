/// \file   CbmMuchDigitizerQa.h
/// \brief  Definition of the CbmMuchDigitizerQa class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \author Eugeny Kryshen
/// \author Vikas Singhal
/// \author Ekata Nandy
/// \date   25.09.2020

#ifndef CbmMuchDigitizerQa_H
#define CbmMuchDigitizerQa_H

#include "FairTask.h"
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TFolder.h>
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
  virtual void FinishTask();
  virtual void SetParContainers();
  static void DivideCanvas2D(TCanvas* c, int nPads);

protected:
  /* DigitizerQa - analysis of digitizer performance - charge distributions
   * for tracks. Track length distrivutions. Statistics on particle types
   */
  void DigitizerQa();

  /* Occupance analysis - all pads,fired pads,
   * and fired/all distributions as functions of radius
   */
  void OccupancyQa();

private:
  Int_t GetNChannels(Int_t iStation);
  Int_t GetNSectors(Int_t iStation);
  TVector2 GetMinPadSize(Int_t iStation);
  TVector2 GetMaxPadSize(Int_t iStation);
  static Double_t LandauMPV(Double_t* x, Double_t* par);
  static Double_t MPV_n_e(Double_t Tkin, Double_t mass);

  CbmMuchDigitizerQa(const CbmMuchDigitizerQa&);
  CbmMuchDigitizerQa& operator=(const CbmMuchDigitizerQa&);

  TFolder* histFolder;
  void InitChargeCanvases();
  void InitPadCanvases();
  void InitChargeHistos();
  void InitLengthHistos();
  void InitPadHistos();
  void InitChannelPadInfo();
  void InitFits();
  void DeInit();

  void FillTotalPadsHistos();
  void DrawCanvases();
  void OutputNvsS();

  void FillChargePerPoint();
  void FillDigitizerPerformancePlots();

  void PrintFrontLayerPoints();
  void PrintFrontLayerDigis();

  // geometry
  CbmMuchGeoScheme* fGeoScheme = nullptr;
  Int_t fNstations             = 0;
  CbmDigiManager* fDigiManager = nullptr;

  // containers
  TClonesArray* fPoints      = nullptr;
  TClonesArray* fDigis       = nullptr;
  TClonesArray* fDigiMatches = nullptr;
  TClonesArray* fMCTracks    = nullptr;
  TClonesArray* fPointInfos  = nullptr;  /// temporary additional information

  TFolder fOutFolder;  /// output folder with histos and canvases
  Int_t fNevents = 0;  /// number of processed events

  // internal unscaled histograms, need to be scaled at the output
  std::vector<TH1F*> fvUsPadsFiredR;   // fired pads vs R, per station
  std::vector<TH2F*> fvUsPadsFiredXY;  // fired pads vs XY, per station

  // output histograms
  TH1F* fhMcPointCharge    = nullptr;  /// MC point charge
  TH1F* fhMcPointChargeLog = nullptr;  /// MC point charge log scale
  TH1F* fhMcPointChargePr_1GeV_3mm =
    nullptr;  /// MC point charge for selected protons

  TH1F* fhTrackLength   = nullptr;
  TH1F* fhTrackLengthPi = nullptr;
  TH1F* fhTrackLengthPr = nullptr;
  TH1F* fhTrackLengthEl = nullptr;

  TH2F* fhMcPointChargeVsTrackEnergyLog   = nullptr;
  TH2F* fhMcPointChargeVsTrackEnergyLogPi = nullptr;
  TH2F* fhMcPointChargeVsTrackEnergyLogPr = nullptr;
  TH2F* fhMcPointChargeVsTrackEnergyLogEl = nullptr;
  TH2F* fhMcPointChargeVsTrackLength      = nullptr;
  TH2F* fhMcPointChargeVsTrackLengthPi    = nullptr;
  TH2F* fhMcPointChargeVsTrackLengthPr    = nullptr;
  TH2F* fhMcPointChargeVsTrackLengthEl    = nullptr;
  TH2F* fhNpadsVsS                        = nullptr;

  std::vector<TH1F*> fvMcPointCharge;  // MC point charge per station
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

  Int_t fSignalPoints = 0;  // Number of signal MC points
  Int_t fnPadSizesX   = 0;
  Int_t fnPadSizesY   = 0;

  Double_t fPadMinLx = 0.;
  Double_t fPadMinLy = 0.;
  Double_t fPadMaxLx = 0.;
  Double_t fPadMaxLy = 0.;

  ClassDef(CbmMuchDigitizerQa, 0)
};

#endif
