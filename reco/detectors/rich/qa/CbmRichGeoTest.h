/* Copyright (C) 2009-2020 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev, Semen Lebedev [committer] */

/**
 * \file CbmRichGeoTest.h
 *
 * \brief RICH geometry checking and testing.
 *
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 **/

#ifndef CBMRICHGEOTEST
#define CBMRICHGEOTEST

#include "FairTask.h"

class TH1;
class TH2;
class TH3;
class TH1D;
class TH2D;
class TH3D;
class TClonesArray;
class CbmRichRingFitterCOP;
class CbmRichRingFitterEllipseTau;
class CbmRichRing;
class CbmRichRingLight;
class TCanvas;
class CbmHistManager;
class TVector3;
class CbmMCDataArray;
class CbmMCEventList;
class CbmDigiManager;

#include <string>
#include <vector>

using namespace std;

/**
 * \class CbmRichGeoTest
 *
 * \brief RICH geometry checking and testing.
 *
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 **/
class CbmRichGeoTest : public FairTask {

public:
  /**
     * \brief Standard constructor.
     */
  CbmRichGeoTest();

  /**
     * \brief Standard destructor.
     */
  virtual ~CbmRichGeoTest();

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

  void SetDrawPmts(Bool_t draw) { fDrawPmts = draw; }
  void SetDrawEventDisplay(Bool_t draw) { fDrawEventDisplay = draw; }

private:
  /**
     * \brief Initialize histograms.
     */
  void InitHistograms();


  /**
     * \brief Fill MC histogram for detector acceptance calculation.
     */
  void ProcessMc();

  /**
     * \brief Loop over all rings in array and fill ring parameters histograms.
     */
  void RingParameters();

  /**
     * \brief Fit ring using ellipse fitter and fill histograms.
     * \param[in] histIndex Fitting type index, 0 - hit fitting, 1 - MC points fitting.
     * \param[in] ring Pointer to CbmRichRing to be fitted and filled in histograms.
     * \param[in] momentum MC momentum of particle produced ring.
     */
  void FitAndFillHistEllipse(Int_t histIndex, CbmRichRingLight* ring, Double_t momentum);

  /**
     * \brief Fit ring using circle fitter and fill histograms.
     * \param[in] histIndex Fitting type index, 0 - hit fitting, 1 - MC points fitting.
     * \param[in] ring Pointer to CbmRichRingLight to be fitted and filled in histograms.
     * \param[in] momentum MC momentum of particle produced ring.
     */
  void FitAndFillHistCircle(Int_t histIndex, CbmRichRingLight* ring, Double_t momentum);

  /**
     * \brief Calculate difference between ellipse parameters
     *  for two fitting using hits and MC points for fit and fill
     *  corresponding histograms.
     * \param[in] ring Ring fitted using hits.
     * \param[in] ringMc Ring fitted using MC points
     */
  void FillMcVsHitFitEllipse(CbmRichRingLight* ring, CbmRichRingLight* ringMc);

  /**
     * \brief Calculate difference between circle parameters
     *  for two fittings using hits and MC points for fit and fill
     *  corresponding histograms.
     * \param[in] ring Ring fitted using hits.
     * \param[in] ringMc Ring fitted using MC points
     */
  void FillMcVsHitFitCircle(CbmRichRingLight* ring, CbmRichRingLight* ringMc);

  /**
     * \brief Calculate residuals between hits and MC points and fill histograms.
     */
  void ProcessHits();

  /**
     * \brief Create histogram: RICH detector acceptance vs.
     * minimum required number of hits in ring
     */
  TH1D* CreateAccVsMinNofHitsHist();

  /**
     *  \brief Draw histograms.
     */
  void DrawHist();

  /**
     * \brief Draw ring in separate TCanvas.
     * \param[in] ring Ring with RICH hits.
     * \param[in] ringPoint Ring with MC RICH points.
     */
  void DrawRing(CbmRichRingLight* ringHit, CbmRichRingLight* ringPoint);


  void DrawH2MeanRms(TH2* hist, const string& canvasName);

  /**
     * \brief DrawPmts
     */
  void DrawPmts();

  void DrawPmtPoint(const string& coordinates, const vector<Int_t>& ids, Bool_t isDrawPixel);

  /**
     * \brief Calculate efficiency.
     * \param[in] histRec
     * \param[in] histAcc
     */
  string CalcEfficiency(TH1* histRec, TH1* histAcc);

  /**
     * \brief Copy constructor.
     */
  CbmRichGeoTest(const CbmRichGeoTest&);

  /**
     * \brief Assignment operator.
     */
  CbmRichGeoTest& operator=(const CbmRichGeoTest&);

  string fOutputDir;  // output directory for results

  TClonesArray* fRichHits;
  TClonesArray* fRichRings;
  CbmMCDataArray* fRichRefPlanePoints;
  CbmDigiManager* fDigiMan = nullptr;
  CbmMCDataArray* fRichPoints;
  CbmMCDataArray* fMcTracks;
  TClonesArray* fRichRingMatches;
  CbmMCEventList* fEventList;

  // rings will be fitted on a fly
  CbmRichRingFitterCOP* fCopFit;
  CbmRichRingFitterEllipseTau* fTauFit;

  CbmHistManager* fHM;  // Histogram manager

  Int_t fEventNum;
  Int_t fMinNofHits;  // Min number of hits in ring for detector acceptance calculation.

  // fitting efficiency
  Double_t fMinAaxis;
  Double_t fMaxAaxis;
  Double_t fMinBaxis;
  Double_t fMaxBaxis;
  Double_t fMinRadius;
  Double_t fMaxRadius;

  Int_t fNofDrawnRings;  // store number of drawn rings
  Bool_t fDrawPmts;      // draw pixels and PMTs to test rotation procedure
  Bool_t fDrawEventDisplay;

  ClassDef(CbmRichGeoTest, 1)
};

#endif
