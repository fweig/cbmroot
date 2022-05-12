/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef MCBM_RICH_QA_RICH_ONLY
#define MCBM_RICH_QA_RICH_ONLY

#include "CbmEvent.h"
#include "CbmRichRingFinderHoughImpl.h"

#include "FairTask.h"
class TClonesArray;
class CbmRichRing;
class CbmRichHit;
class CbmTofTracklet;
class CbmHistManager;
class TVector3;
class CbmDigiManager;
class CbmRichMCbmSEDisplay;

#include <tuple>

#include <array>
#include <map>
#include <vector>

using namespace std;


class CbmRichMCbmQaRichOnly : public FairTask {

public:
  /**
     * \brief Standard constructor.
     */
  CbmRichMCbmQaRichOnly();

  /**
     * \brief Standard destructor.
     */
  virtual ~CbmRichMCbmQaRichOnly() {};

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
    * Set an trigger on the RICH Hits. 
    */
  void SetTriggerRichHits(Int_t val = 0) { fTriggerRichHits = val; }

  /**
    * activate the generation of new ICD correction iterations
    */
  void SetIcdGeneration(bool val = true) { fGenerateICD = val; }


private:
  CbmDigiManager* fDigiMan = nullptr;

  TClonesArray* fRichHits;

  TClonesArray* fRichRings;

  TClonesArray* fCbmEvent;

  CbmHistManager* fHM;

  Double_t fXOffsetHisto;

  Int_t fEventNum;

  Int_t fNofDrawnRings;

  Int_t fNofDrawnRichTofEv;

  Int_t fNofDrawnEvents;

  Int_t fMaxNofDrawnEvents;

  Int_t fTriggerRichHits;


  string fOutputDir;  // output dir for results

  bool fRestrictToAcc     = false;
  bool fRestrictToFullAcc = false;

  bool fDoWriteHistToFile = true;
  bool fDoDrawCanvas      = true;

  bool fGenerateICD = false;

  std::array<Double_t, 2304> ICD_offset_read;
  std::array<Double_t, 2304> ICD_offset;
  std::array<uint32_t, 2304> ICD_offset_cnt;

  CbmRichMCbmSEDisplay* fSeDisplay = nullptr;


  /**
     * \brief Initialize histograms.
     */
  void InitHistograms();

  /**
     *  \brief Draw histograms.
     */
  void DrawHist();

  void RichRings();

  void DrawEvent(CbmEvent* ev, std::vector<int>& ringIndx, bool full);

  void DrawRing(CbmRichRing* ring) { DrawRing(ring, false); };

  void DrawRing(CbmRichRing* ring, bool full);


  bool doToT(CbmRichHit* hit);


  void analyseRing(CbmRichRing* ring, CbmEvent* ev);

  Bool_t cutRadius(CbmRichRing* ring);

  void save_ICD(std::array<Double_t, 2304>& offsets, unsigned int iteration);

  void read_ICD(std::array<Double_t, 2304>& offsets, unsigned int iteration);

  /**
     * \brief Copy constructor.
     */
  CbmRichMCbmQaRichOnly(const CbmRichMCbmQaRichOnly&);

  /**
     * \brief Assignment operator.
     */
  CbmRichMCbmQaRichOnly& operator=(const CbmRichMCbmQaRichOnly&);


  ClassDef(CbmRichMCbmQaRichOnly, 1)
};

#endif
