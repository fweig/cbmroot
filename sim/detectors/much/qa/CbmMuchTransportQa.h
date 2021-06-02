/** @file CbmMuchTransportQa.h
  * @copyright Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Sergey Gorbunov, Eugeny Kryshen, Vikas Singhal, Ekata Nandy, Dominik Smith [committer] **/

/** @file   CbmMuchTransportQa.h
  * @brief  Definition of the CbmMuchTransportQa class
  * @date   21.10.2020 **/

#ifndef CbmMuchTransportQa_H
#define CbmMuchTransportQa_H

#include "FairTask.h"

#include "TParameter.h"
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TFolder.h>

#include <vector>

class CbmMCDataArray;
class CbmMCDataManager;
class CbmTimeSlice;
class CbmMuchPoint;
class CbmQaCanvas;
class TBuffer;
class TClass;
class TClonesArray;
class TH1F;
class TH2F;
class TMemberInspector;
class CbmQaPie;

/// QA for the MUCH detector after a "transport" step of the simulation.
/// The class reimplements corresponding QA checks from old CbmMuchHitFinderQa class
/// made by E. Kryshen & V. Singhal & E. Nandy
///
class CbmMuchTransportQa : public FairTask {

public:
  /// Constructor
  CbmMuchTransportQa(const char* name = "MuchHitFinderQa", Int_t verbose = 1);
  /// Deactivated copy constructors
  CbmMuchTransportQa(const CbmMuchTransportQa&) = delete;
  CbmMuchTransportQa& operator=(const CbmMuchTransportQa&) = delete;

  /// Destructor
  virtual ~CbmMuchTransportQa();

  /// FairTask methods
  void SetParContainers();
  InitStatus Init();
  InitStatus ReInit();
  void Exec(Option_t* option);
  void Finish();

  /// Prepare Qa output and return it as a reference to an internal folder.
  /// The reference is non-const, because FairSink can not write const objects
  TFolder& GetQa();

private:
  void InitCountingHistos();
  void InitFractionHistos();
  void Init2dSpatialDistributionHistos();
  void InitRatioPieCharts();
  void InitCanvases();
  void FillCountingHistos(Int_t stId, Int_t motherId, Int_t pdgCode);
  void Fill2dSpatialDistributionHistos(CbmMuchPoint* point, Int_t stId);

  /// Reset varibles & deallocate memory.
  /// When not called by destructor, need to be folloed by Init().
  void DeInit();

  void MakePRatioPieCharts();
  void MakePrimRatioPieCharts();
  void DrawCanvases();

  /// geometry
  Int_t fNstations = 0;

  // setup
  FairRootManager* fManager    = nullptr;
  CbmMCDataManager* fMcManager = nullptr;
  CbmTimeSlice* fTimeSlice     = nullptr;

  /// containers
  CbmMCDataArray* fPoints   = nullptr;
  CbmMCDataArray* fMcTracks = nullptr;

  ///

  TFolder* histFolder;       /// subfolder for histograms
  TFolder fOutFolder;        /// output folder with histos and canvases
  TParameter<int> fNevents;  /// number of processed events

  /// internal unscaled histogramms
  TH1F* fhUsNtraAll  = nullptr;  /// number of all tracks
  TH1F* fhUsNtraPrim = nullptr;  /// number of primary tracks
  TH1F* fhUsNtraSec  = nullptr;  /// number of secondary tracks
  TH1F* fhUsNtraPr   = nullptr;  /// number of protons
  TH1F* fhUsNtraPi   = nullptr;  /// number of pions
  TH1F* fhUsNtraEl   = nullptr;  /// number of electrons
  TH1F* fhUsNtraMu   = nullptr;  /// number of muons
  TH1F* fhUsNtraKa   = nullptr;  /// number of kaons
  std::vector<TH1F*> fvUsNtra;   /// pointers to the above fhUsNtra* histos

  /// output histograms
  std::vector<TH2F*> fvMcPointXY;    /// MC point Y vs X [N stations]
  std::vector<TH2F*> fvMcPointPhiZ;  /// MC point Phi vs Z [N stations]
  std::vector<TH2F*> fvMcPointRZ;    /// MC point R vs Z [N stations]

  TH1F* fhNtracks      = nullptr;  /// number of all tracks / event
  TH1F* fhFractionPrim = nullptr;  /// fraction of primary tracks
  TH1F* fhFractionSec  = nullptr;  /// fraction of secondary tracks
  TH1F* fhFractionPr   = nullptr;  /// fraction of protons
  TH1F* fhFractionPi   = nullptr;  /// fraction of pions
  TH1F* fhFractionEl   = nullptr;  /// fraction of electrons
  TH1F* fhFractionMu   = nullptr;  /// fraction of muons
  TH1F* fhFractionKa   = nullptr;  /// fraction of kaons
  std::vector<TH1F*> fvFraction;   /// pointers to the above histos

  /// output pie charts
  std::vector<CbmQaPie*> fvMcPointPRatio;     /// MC point particle ratio pie charts [N stations]
  std::vector<CbmQaPie*> fvMcPointPrimRatio;  /// MC point particle ratio pie charts [N stations]

  // output canvaces with histogramm collections
  CbmQaCanvas* fCanvStationXY   = nullptr;
  CbmQaCanvas* fCanvStationPhiZ = nullptr;
  CbmQaCanvas* fCanvStationRZ   = nullptr;
  CbmQaCanvas* fCanvNtra        = nullptr;

  // output canvaces with pie chart collections
  CbmQaCanvas* fCanvStationPRatio    = nullptr;
  CbmQaCanvas* fCanvStationPrimRatio = nullptr;

  ClassDef(CbmMuchTransportQa, 0)
};

#endif
