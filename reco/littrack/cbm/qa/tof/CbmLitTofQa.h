/** @file CbmLitTofQa.h
  * @copyright Copyright (C) 2013-2018 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [committer] **/

/**
 * \file CbmLitTofQa.h
 * \brief Task for TOF QA.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 */

#ifndef CBMLITTOFQA_H_
#define CBMLITTOFQA_H_

#include "CbmStsKFTrackFitter.h"

#include "FairTask.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class CbmHistManager;
class CbmVertex;
class FairTrackParam;
class CbmMCDataArray;

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

/**
 * \class CbmLitTofQa
 * \brief  Task for TOF QA.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 */
class CbmLitTofQa : public FairTask {
public:
  /**
    * \brief Constructor.
    */
  CbmLitTofQa();

  /**
    * \brief Destructor.
    */
  virtual ~CbmLitTofQa();

  /**
    * \brief Inherited from FairTask.
    */
  virtual InitStatus Init();

  /**
    * \brief Inherited from FairTask.
    */
  virtual void Exec(Option_t* opt);

  /**
    * \brief Inherited from FairTask.
    */
  virtual void Finish();

  /* Setters */
  void SetOutputDir(const string& dir) { fOutputDir = dir; }

  /**
    * \brief Set momentum range and number of bins for histograms.
    */
  void SetPRange(Int_t bins, Int_t min, Int_t max)
  {
    fPRangeBins = bins;
    fPRangeMin  = min;
    fPRangeMax  = max;
  }

private:
  /**
   * \brief Reads data branches.
   */
  void ReadDataBranches();

  /**
    * \brief Assign default track categories and track acceptance functions.
    */
  void FillTrackCategoriesAndAcceptanceFunctions();

  /**
    * \brief Creates histograms.
    */
  void CreateHistograms();

  void ProcessMC(Int_t iEvent);

  void ProcessGlobalTracks();

  void ProcessTofHits();

  void ProcessTofTracks();

  void FitHistograms();

  CbmLitTofQa(const CbmLitTofQa&);
  CbmLitTofQa& operator=(const CbmLitTofQa&);

  Bool_t fIsFixedBounds;  // if true than fixed bounds are used for histograms

  string fOutputDir;  // Output directory for images

  Double_t fPRangeMin;  // Min momentum
  Double_t fPRangeMax;  // Max momentum
  Int_t fPRangeBins;    // Number of bins

  CbmHistManager* fHM;  // Histogram manager

  // Data branches
  TClonesArray* fGlobalTracks;     // CbmGlobalTrack array
  TClonesArray* fStsTracks;        // CbmStsTrack array
  TClonesArray* fStsTrackMatches;  // CbmTrackMatchNew array
  TClonesArray* fTofHits;          // CbmTofHit array
  CbmMCDataArray* fTofPoints;      // CbmTofPoint array
  TClonesArray* fTofHitsMatches;
  TClonesArray* fTofTracks;   // CbmTofTrack array
  CbmMCDataArray* fMCTracks;  // CbmMCTrack array

  CbmVertex* fPrimVertex;         // Pointer to the primary vertex
  CbmStsKFTrackFitter fKFFitter;  // Pointer to the Kalman Filter Fitter algorithm

  vector<string> fTrackCategories;  // Vector of track category names


  //#ifndef  __ROOTCLING__
  typedef Bool_t (*LitTrackAcceptanceFunction)(CbmMCDataArray* mcTracks, Int_t eventNo, Int_t index);
  map<string, LitTrackAcceptanceFunction>
    fTrackAcceptanceFunctions;  // maps track category name to track acceptance function
                                //#endif

  set<pair<Int_t, Int_t>> fMCTrackIdForTofHits;    // Set of MC track IDs for all existing TOF hits
  set<pair<Int_t, Int_t>> fMCTrackIdForTofPoints;  // Set of MC track IDs for all existing TOF points

  ClassDef(CbmLitTofQa, 1)
};

#endif /* CBMLITTOFQA_H_ */
