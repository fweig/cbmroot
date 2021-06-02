/** @file CbmRichMCbmQa.h
  * @copyright Copyright (C) 2017-2020 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev, Gregor Pitsch [committer] **/

#ifndef MCBM_RICH_QA
#define MCBM_RICH_QA

#include "CbmHistManager.h"  // for ROOTCLING

#include "FairTask.h"

class TClonesArray;
class CbmRichRing;

#include <map>
#include <vector>

using namespace std;

class CbmRichMCbmQa : public FairTask {

public:
  /**
     * \brief Standard constructor.
     */
  CbmRichMCbmQa();

  /**
     * \brief Standard destructor.
     */
  virtual ~CbmRichMCbmQa() {};

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

private:
  /**
     * \brief Initialize histograms.
     */
  void InitHistograms();

  /**
     *  \brief Draw histograms.
     */
  void DrawHist();

  /**
     * Return low edges of the histogram bins which matches pixel structure.
     */
  vector<Double_t> GetHistBins(Bool_t isX);

  void DrawCircle(CbmRichRing* ring);

  void DrawEvent();


  /**
     * \brief Copy constructor.
     */
  CbmRichMCbmQa(const CbmRichMCbmQa&);

  /**
     * \brief Assignment operator.
     */
  CbmRichMCbmQa& operator=(const CbmRichMCbmQa&);


  CbmHistManager* fHM;


  Int_t fEventNum;


  string fOutputDir;  // output dir for results


  TClonesArray* fMCTracks;
  TClonesArray* fRichPoints;
  TClonesArray* fRichDigis;
  TClonesArray* fRichHits;
  TClonesArray* fRichRings;
  TClonesArray* fRichRingMatches;
  TClonesArray* fRefPlanePoints;
  TClonesArray* fGlobalTracks;
  TClonesArray* fTrdTracks;
  TClonesArray* fTofHits;
  TClonesArray* fTofPoints;
  TClonesArray* fTofHitMatches;

  ClassDef(CbmRichMCbmQa, 1)
};

#endif
