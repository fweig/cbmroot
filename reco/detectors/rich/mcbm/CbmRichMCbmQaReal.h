/* Copyright (C) 2019-2020 UGiessen/JINR-LIT, Giessen/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Adrian Amatus Weber, Florian Uhlig */

#ifndef MCBM_RICH_QA_REAL
#define MCBM_RICH_QA_REAL

#include "CbmEvent.h"
#include "CbmHistManager.h"  // for ROOTCLING
#include "CbmRichRingFinderHoughImpl.h"
#include "CbmTsEventHeader.h"

#include "FairTask.h"

class TClonesArray;
class CbmRichRing;
class CbmRichHit;
#include "CbmTofTracklet.h"
class TVector3;
class CbmDigiManager;
class CbmRichMCbmSEDisplay;
#include "CbmTofDigi.h"

#include <map>
#include <vector>

using namespace std;


class CbmRichMCbmQaReal : public FairTask {

public:
  /**
     * \brief Standard constructor.
     */
  CbmRichMCbmQaReal();

  /**
     * \brief Standard destructor.
     */
  virtual ~CbmRichMCbmQaReal() {};

  /**
     * \brief Inherited from FairTask.
     */
  virtual InitStatus Init();

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Exec(Option_t* option);

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Finish();

  /**
     * \brief Set output directory where you want to write results (figures and json).
     * \param[in] dir Path to the output directory.
     */
  void SetOutputDir(const string& dir) { fOutputDir = dir; }


  /**
     * \brief Draw histogram from file
     */
  void DrawFromFile(const string& fileName, const string& outputDir);

  /**
    * Apply restriction to current mRICH Acceptance (for Simulations)
    */
  void DoRestrictToAcc() { fRestrictToAcc = true; }


  /**
    * Apply restriction to full mRICH Acceptance (for Simulations)
    */
  void DoRestrictToFullAcc(bool val = true) { fRestrictToFullAcc = val; }


  /**
    * Apply restriction to full mRICH Acceptance (for Simulations)
    */
  void DoDrawCanvas(bool val = true) { fDoDrawCanvas = val; }

  /**
    * Apply restriction to full mRICH Acceptance (for Simulations)
    */
  void DoWriteHistToFile(bool val = true) { fDoWriteHistToFile = val; }


  /**
    * Move X-Position of mRICH in Histograms (e.g. for Geometry changes)
    */
  void XOffsetHistos(Double_t val = 0.) { fXOffsetHisto = val; }

  /**
    * Limit of Single Event Displays that should be drawn 
    */
  void SetMaxNofDrawnEvents(Int_t val = 100) { fMaxNofDrawnEvents = val; }

  /**
    * Set an trigger on the tof Hits. 
    */
  void SetTriggerTofHits(Int_t val = 0) { fTriggerTofHits = val; }

  /**
    * Set an trigger on the RICH Hits. 
    */
  void SetTriggerRichHits(Int_t val = 0) { fTriggerRichHits = val; }


  /**
    * Set a flag to draw only Single Event Displays with minimum one ring.
    */
  void SetSEDisplayRingOnly(bool val = true) { bSeDisplayRingOnly = val; }

  /**
    * Activate generation of Time related histograms
    */
  void ActivateTimeHistograms(bool val = true) { fDoTimePlots = val; }

  /**
    * Set an ToT cut of the RICH Hits.
    */
  void SetTotRich(Double_t min, Double_t max)
  {
    fTotRichMin = min;
    fTotRichMax = max;
  }

  bool isOnTarget(CbmTofTracklet* tTrack)
  {

    Double_t val = std::sqrt(tTrack->GetFitX(0.) * tTrack->GetFitX(0.) + tTrack->GetFitY(0.) * tTrack->GetFitY(0.));
    if (val < 10.) return true;

    return false;
  }

private:
  CbmDigiManager* fDigiMan = nullptr;

  //TClonesArray* fT0Digis;
  const std::vector<CbmTofDigi>* fT0Digis = nullptr;

  TClonesArray* fRichHits;

  TClonesArray* fRichRings;

  TClonesArray* fTofHits;

  TClonesArray* fTofTracks;

  const CbmTsEventHeader* fTSHeader;

  TClonesArray* fCbmEvent;


  CbmHistManager* fHM;


  Double_t fXOffsetHisto;

  Double_t fTotRichMin;

  Double_t fTotRichMax;

  Int_t fEventNum;

  Int_t fNofDrawnRings;

  Int_t fNofDrawnRichTofEv;

  Int_t fMaxNofDrawnEvents;

  Int_t fTriggerRichHits;

  Int_t fTriggerTofHits;

  Int_t fTracksinRich = 0;

  Int_t fRingsWithTrack[6] = {0, 0, 0, 0, 0, 0};  //rwt;ring;track;ringCut;trackCut;combinations;

  Int_t fTracksinRichWithRichHits[4] = {0, 0, 0, 0};

  uint64_t fTSMinTime = 0;

  string fOutputDir;  // output dir for results

  bool fRestrictToAcc     = false;
  bool fRestrictToFullAcc = false;

  bool fDoWriteHistToFile = true;
  bool fDoDrawCanvas      = true;

  bool bSeDisplayRingOnly = false;

  bool fDigiHitsInitialized = false;

  bool fDoTimePlots = false;

  bool RestrictToFullAcc(CbmTofTracklet* track);
  bool RestrictToFullAcc(TVector3& pos);
  bool RestrictToFullAcc(Double_t x, Double_t y);

  TVector3 extrapolate(CbmTofHit* tofHit, Double_t Z);

  Double_t fCbmEventStartTime = 0.;
  CbmEvent* fEventPnt         = nullptr;

  std::array<Double_t, 2304> offset_read;
  std::array<Double_t, 2304> offset;
  std::array<uint32_t, 2304> offset_cnt;

  CbmRichMCbmSEDisplay* fSeDisplay = nullptr;

  CbmRichMCbmSEDisplay* fSeDsply_TR = nullptr;

  /**
     * \brief Initialize histograms.
     */
  void InitHistograms();

  /**
     *  \brief Draw histograms.
     */
  void DrawHist();

  void RichRings();

  void DrawRing(CbmRichRing* ring);

  void DrawEvent(CbmEvent* ev, std::vector<int>& ringIndx, bool full);

  void DrawRing(CbmRichRing* ring, std::vector<CbmTofTracklet*> track) { DrawRing(ring, track, false); };

  void DrawRing(CbmRichRing* ring, std::vector<CbmTofTracklet*> track, bool full);

  void DrawRichTofEv(const std::vector<int> richHitIndx, const std::vector<int> tofTrackIndx);

  std::pair<int, double> FindClosestTrack(const CbmRichRing* ring, const std::vector<CbmTofTracklet*> track);

  std::pair<int, double> FindClosestRing(CbmTofTracklet* track, std::vector<int>& ringIndx);

  bool isAccmRICH(CbmTofTracklet* track);

  template<typename T = CbmRichHit>
  bool doToT(T* hit)
  {
    if ((hit->GetToT() > fTotRichMin) && (hit->GetToT() < fTotRichMax)) return true;
    return false;
  }

  Double_t getBeta(CbmTofTracklet* track);

  Double_t getBeta(CbmRichRing* ring);

  void analyseRing(CbmRichRing* ring, CbmEvent* ev, std::pair<int, double>& clTrack);

  Bool_t cutRadius(CbmRichRing* ring);
  Bool_t cutDistance(std::pair<int, double>& clTrack);


  /**
     * \brief Copy constructor.
     */
  CbmRichMCbmQaReal(const CbmRichMCbmQaReal&);

  /**
     * \brief Assignment operator.
     */
  CbmRichMCbmQaReal& operator=(const CbmRichMCbmQaReal&);


  ClassDef(CbmRichMCbmQaReal, 1)
};

#endif
