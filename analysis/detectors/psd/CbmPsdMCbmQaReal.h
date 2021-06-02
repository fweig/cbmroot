/** @file CbmPsdMCbmQaReal.h
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

#ifndef MCBM_PSD_QA_REAL
#define MCBM_PSD_QA_REAL

#include "CbmEvent.h"
#include "CbmHistManager.h"

#include "FairTask.h"

class TClonesArray;
class CbmPsdMCbmHit;
class CbmTofTracklet;
class TVector3;
class CbmDigiManager;

#include <map>
#include <vector>

using namespace std;


class CbmPsdMCbmQaReal : public FairTask {

public:
  /**
     * \brief Standard constructor.
     */
  CbmPsdMCbmQaReal();

  /**
     * \brief Standard destructor.
     */
  virtual ~CbmPsdMCbmQaReal() {};

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
    * Apply restriction to full mRICH Acceptance (for Simulations)
    */
  void DoDrawCanvas(bool val = true) { fDoDrawCanvas = val; }

  /**
    * Apply restriction to full mRICH Acceptance (for Simulations)
    */
  void DoWriteHistToFile(bool val = true) { fDoWriteHistToFile = val; }


private:
  CbmDigiManager* fDigiMan = nullptr;

  TClonesArray* fT0Digis;
  TClonesArray* fPsdHits;
  TClonesArray* fTofHits;
  TClonesArray* fTofTracks;
  TClonesArray* fCbmEvent;

  CbmHistManager* fHM;

  Int_t fEntryNum;
  string fOutputDir;  // output dir for results

  bool fDoWriteHistToFile = true;
  bool fDoDrawCanvas      = true;

  bool fDigiHitsInitialized = false;


  Double_t fCbmEventStartTime = 0.;

  /**
     * \brief Initialize histograms.
     */
  void InitHistograms();

  /**
     *  \brief Draw histograms.
     */
  void DrawHist();

  /**
     * \brief Copy constructor.
     */
  CbmPsdMCbmQaReal(const CbmPsdMCbmQaReal&);

  /**
     * \brief Assignment operator.
     */
  CbmPsdMCbmQaReal& operator=(const CbmPsdMCbmQaReal&);


  ClassDef(CbmPsdMCbmQaReal, 1)
};

#endif
