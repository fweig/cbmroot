/* Copyright (C) 2011-2020 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Volker Friese */

#/**
 * \file CbmLitClusteringQa.h
 * \brief FairTask for clustering performance calculation.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */

#ifndef CBMLITCLUSTERINGQA_H
#define CBMLITCLUSTERINGQA_H

#include "CbmMCEventList.h"
#include "CbmModuleList.h"
#include "CbmMuchGeoScheme.h"
#include "CbmTimeSlice.h"
#include "cbm/base/CbmLitDetectorSetup.h"

#include <FairTask.h>

#include <string>
#include <vector>

class CbmHistManager;
class CbmMCDataArray;
class CbmDigiManager;

class CbmLitClusteringQa : public FairTask {
public:
  /**
    * \brief Constructor.
    */
  CbmLitClusteringQa();

  /**
    * \brief Destructor.
    */
  virtual ~CbmLitClusteringQa();

  /**
     * \brief Derived from FairTask.
     */
  virtual InitStatus Init();

  /**
     * \brief Derived from FairTask.
     */
  virtual void Exec(Option_t* opt);

  /**
     * \brief Derived from FairTask.
     */
  virtual void Finish();

  /** Setters */
  void SetOutputDir(const string& outputDir) { fOutputDir = outputDir; }
  void SetMuchDigiFileName(const string& digiFileName) { fMuchDigiFileName = digiFileName; }

private:
  Int_t GetStationId(Int_t address, ECbmModuleId detId);

  /**
     * \brief Read data branches.
     */
  void ReadDataBranches();

  void InitMuchGeoScheme(const string& digiFileName);

  void ProcessPoints(Int_t iEvent, CbmMCDataArray* points, const string& detName, ECbmModuleId detId);

  template<class Digi>
  void ProcessDigis(const string& detName);

  void ProcessClusters(const TClonesArray* clusters, const TClonesArray* clusterMatches, const string& detName,
                       ECbmModuleId detId);

  void ProcessHits(const TClonesArray* hits, const TClonesArray* hitMatches, const string& detName, ECbmModuleId detId);

  /**
     *
     */
  void FillEventCounterHistograms(Int_t iEvent);

  /**
     *
     */
  void CreateHistograms();

  /**
     *
     */
  void CreateNofObjectsHistograms(ECbmModuleId detId, const string& detName);

  /**
     *
     */
  void CreateNofObjectsHistograms(ECbmModuleId detId, const string& detName, const string& parameter,
                                  const string& xTitle);

  void CreateClusterParametersHistograms(ECbmModuleId detId, const string& detName);

  void FillResidualAndPullHistograms(CbmMCDataArray* points, const TClonesArray* hits, const TClonesArray* hitMatches,
                                     const string& detName, ECbmModuleId detId);

  void FillHitEfficiencyHistograms(Int_t iEvent, CbmMCDataArray* points, const TClonesArray* hits,
                                   const TClonesArray* hitMatches, const string& detName, ECbmModuleId detId);

  void CreateHitEfficiencyHistograms(ECbmModuleId detId, const string& detName, const string& parameter,
                                     const string& xTitle, Int_t nofBins, Double_t minBin, Double_t maxBin);

  CbmHistManager* fHM    = nullptr;    // Histogram manager
  std::string fOutputDir = "";         // Output directory for results
  CbmLitDetectorSetup fDet {};         // For detector setup determination
  CbmDigiManager* fDigiMan = nullptr;  //! Interface to digi data

  // Pointers to data arrays
  CbmMCDataArray* fMCTracks = nullptr;  //! CbmMCTrack

  CbmMCDataArray* fMvdPoints = nullptr;  //! CbmMvdPoint
  TClonesArray* fMvdClusters = nullptr;  //! CbmMvdCluster
  TClonesArray* fMvdHits     = nullptr;  //! CbmMvdHit

  CbmMCDataArray* fStsPoints = nullptr;
  ;  //! CbmStsPoint
  TClonesArray* fStsClusters = nullptr;
  ;                                            //! CbmStsCluster
  TClonesArray* fStsHits           = nullptr;  //! CbmStsHit array
  TClonesArray* fStsClusterMatches = nullptr;  //! CbmMatch (cluster)
  TClonesArray* fStsHitMatches     = nullptr;  //! CbmMatch (hit)

  CbmMCDataArray* fRichPoints = nullptr;  //! CbmRichPoint
  TClonesArray* fRichHits     = nullptr;  //! CbmRichHit

  CbmMCDataArray* fMuchPoints        = nullptr;  //! CbmMuchPoint
  TClonesArray* fMuchClusters        = nullptr;  //! CbmMuchCluster
  TClonesArray* fMuchPixelHits       = nullptr;  //! CbmMuchPixelHit
  TClonesArray* fMuchClusterMatches  = nullptr;  //! CbmMatch array
  TClonesArray* fMuchPixelHitMatches = nullptr;  //! CbmMatch (hit)

  CbmMCDataArray* fTrdPoints       = nullptr;  //! CbmTrdPoint
  TClonesArray* fTrdClusters       = nullptr;  //! CbmTrdCluster
  TClonesArray* fTrdHits           = nullptr;  //! CbmTrdHit
  TClonesArray* fTrdClusterMatches = nullptr;  //! CbmMatch (cluster)
  TClonesArray* fTrdHitMatches     = nullptr;  //! CbmMatch (hit)

  CbmMCDataArray* fTofPoints = nullptr;  //! CbmTofPoint
  TClonesArray* fTofHits     = nullptr;  //! CbmTofHit

  string fMuchDigiFileName = "";

  CbmTimeSlice* fTimeSlice   = nullptr;
  CbmMCEventList* fEventList = nullptr;

  CbmLitClusteringQa(const CbmLitClusteringQa&) = delete;
  CbmLitClusteringQa& operator=(const CbmLitClusteringQa&) = delete;

  ClassDef(CbmLitClusteringQa, 2);
};

#endif /* CBMLITCLUSTERINGQA_H */
