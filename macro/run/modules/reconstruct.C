/* Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

/** Macro module for registration of reconstruction tasks to a run
 **
 ** This macro creates the task chain for event-by-event reconstruction.
 ** The run instance must be FairRunAna.
 ** The macro assumes a CbmSetup instance.
 ** Reconstruction tasks are instantiated depending on the presence
 ** of the detector in the setup.
 **
 ** If the option useMC is chosen, L1 will be run with performance
 ** evaluation.
 **
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 10 February 2016
 **/


// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmFindPrimaryVertex.h"
#include "CbmKF.h"
#include "CbmL1.h"
#include "CbmL1StsTrackFinder.h"
#include "CbmLitFindGlobalTracks.h"
#include "CbmMatchRecoToMC.h"
#include "CbmMuchFindHitsGem.h"
#include "CbmMvdClusterFinder.h"
#include "CbmMvdHitFinder.h"
#include "CbmPVFinderKF.h"
#include "CbmPsdHitProducer.h"
#include "CbmRecoSts.h"
#include "CbmRichHitProducer.h"
#include "CbmRichReconstruction.h"
#include "CbmSetup.h"
#include "CbmStsFindTracksEvents.h"
#include "CbmTofSimpClusterizer.h"
#include "CbmTrdClusterFinder.h"
#include "CbmTrdHitProducer.h"

#include <FairRun.h>

#include <TSystem.h>
#endif


Bool_t reconstruct(Bool_t useMC = kFALSE, Bool_t searchPV = kTRUE)
{

  // -----   Get the run instance   ------------------------------------------
  FairRun* run = FairRun::Instance();
  if (!run) {
    std::cerr << "-E- reconstruct: No run instance!" << std::endl;
    return kFALSE;
  }
  std::cout << std::endl;
  std::cout << "-I- Macro reconstruct.C called for run " << run->GetName() << std::endl;
  // -------------------------------------------------------------------------


  // -----   Get the CBM setup instance   ------------------------------------
  CbmSetup* setup = CbmSetup::Instance();
  std::cout << std::endl;
  std::cout << "-I- reconstruct: Found setup " << setup->GetTitle() << std::endl;
  // -------------------------------------------------------------------------


  // -----   Ideal event building   ------------------------------------------
  //run->AddTask(new CbmBuildEventsIdeal());
  // -------------------------------------------------------------------------


  // -----   Local reconstruction in MVD   ----------------------------------
  if (setup->IsActive(ECbmModuleId::kMvd)) {

    CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Cluster Finder", 0, 0);
    run->AddTask(mvdCluster);
    std::cout << "-I- : Added task " << mvdCluster->GetName() << std::endl;

    CbmMvdHitfinder* mvdHit = new CbmMvdHitfinder("MVD Hit Finder", 0, 0);
    mvdHit->UseClusterfinder(kTRUE);
    run->AddTask(mvdHit);
    std::cout << "-I- : Added task " << mvdHit->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in STS   ----------------------------------
  if (setup->IsActive(ECbmModuleId::kSts)) {
    FairTask* stsReco = new CbmRecoSts();
    run->AddTask(stsReco);
    std::cout << "-I- : Added task " << stsReco->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in MUCH   ---------------------------------
  if (setup->IsActive(ECbmModuleId::kMuch)) {

    // --- Parameter file name
    TString geoTag;
    setup->GetGeoTag(ECbmModuleId::kMuch, geoTag);
    Int_t muchFlag = 0;
    if (geoTag.Contains("mcbm")) muchFlag = 1;

    std::cout << geoTag(0, 4) << std::endl;
    TString parFile = gSystem->Getenv("VMCWORKDIR");
    parFile         = parFile + "/parameters/much/much_" + geoTag(0, 4) + "_digi_sector.root";
    std::cout << "Using parameter file " << parFile << std::endl;

    // --- Hit finder for GEMs
    FairTask* muchHitGem = new CbmMuchFindHitsGem(parFile.Data(), muchFlag);
    run->AddTask(muchHitGem);

    //		// --- Hit finder for Straws
    //		CbmMuchFindHitsStraws* strawFindHits =
    //				new CbmMuchFindHitsStraws(parFile.Data());
    //		run->AddTask(strawFindHits);
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TRD   ----------------------------------
  if (setup->IsActive(ECbmModuleId::kTrd)) {

    Double_t triggerThreshold       = 0.5e-6;  // SIS100
    CbmTrdClusterFinder* trdCluster = new CbmTrdClusterFinder();
    trdCluster->SetNeighbourEnable(true, false);
    trdCluster->SetMinimumChargeTH(triggerThreshold);
    trdCluster->SetRowMerger(true);
    run->AddTask(trdCluster);
    std::cout << "-I- : Added task " << trdCluster->GetName() << std::endl;

    CbmTrdHitProducer* trdHit = new CbmTrdHitProducer();
    run->AddTask(trdHit);
    std::cout << "-I- : Added task " << trdHit->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TOF   ----------------------------------
  if (setup->IsActive(ECbmModuleId::kTof)) {
    CbmTofSimpClusterizer* tofCluster = new CbmTofSimpClusterizer("TOF Simple Clusterizer", 0);
    tofCluster->SetOutputBranchPersistent("TofHit", kTRUE);
    tofCluster->SetOutputBranchPersistent("TofDigiMatch", kTRUE);
    run->AddTask(tofCluster);
    std::cout << "-I- : Added task " << tofCluster->GetName() << std::endl;
  }
  // -------------------------------------------------------------------------


  // -----   Local reconstruction in PSD reconstruction   --------------------
  if (setup->IsActive(ECbmModuleId::kPsd)) {
    CbmPsdHitProducer* psdHit = new CbmPsdHitProducer();
    run->AddTask(psdHit);
    std::cout << "-I- : Added task CbmPsdHitProducer" << std::endl;
  }
  // -------------------------------------------------------------------------


  // -----  Hit matching (required for L1 tracking with MC input)  -----------
  if (useMC) {
    CbmMatchRecoToMC* match1 = new CbmMatchRecoToMC();
    run->AddTask(match1);
  }
  // -------------------------------------------------------------------------


  // -----   Track finding in (MVD+) STS    ----------------------------------
  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);
  CbmL1* l1 = nullptr;
  if (useMC) l1 = new CbmL1("L1", 1, 3);
  else
    l1 = new CbmL1("L1", 0);
  // --- Material budget file names
  TString mvdGeoTag;
  if (setup->GetGeoTag(ECbmModuleId::kMvd, mvdGeoTag)) {
    TString parFile = gSystem->Getenv("VMCWORKDIR");
    parFile         = parFile + "/parameters/mvd/mvd_matbudget_" + mvdGeoTag + ".root";
    std::cout << "Using material budget file " << parFile << std::endl;
    l1->SetMvdMaterialBudgetFileName(parFile.Data());
  }
  TString stsGeoTag;
  if (setup->GetGeoTag(ECbmModuleId::kSts, stsGeoTag)) {
    TString parFile = gSystem->Getenv("VMCWORKDIR");
    parFile         = parFile + "/parameters/sts/sts_matbudget_" + stsGeoTag + ".root";
    std::cout << "Using material budget file " << parFile << std::endl;
    l1->SetStsMaterialBudgetFileName(parFile.Data());
  }
  run->AddTask(l1);
  std::cout << "-I- : Added task " << l1->GetName() << std::endl;

  CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
  FairTask* stsFindTracks           = new CbmStsFindTracksEvents(stsTrackFinder, setup->IsActive(ECbmModuleId::kMvd));
  run->AddTask(stsFindTracks);
  std::cout << "-I- : Added task " << stsFindTracks->GetName() << std::endl;
  // -------------------------------------------------------------------------


  // -----   Primary vertex finding   ---------------------------------------
  if (searchPV) {
    CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
    CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
    run->AddTask(findVertex);
    std::cout << "-I- : Added task " << findVertex->GetName() << std::endl;
  }
  // -------------------------------------------------------------------------


  // ---   Global track finding   --------------------------------------------
  CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
  finder->SetTrackingType("branch");
  finder->SetMergerType("nearest_hit");
  run->AddTask(finder);
  std::cout << "-I- : Added task " << finder->GetName() << std::endl;
  // -------------------------------------------------------------------------


  // -----   RICH reconstruction   ------------------------------------------
  if (setup->IsActive(ECbmModuleId::kRich)) {

    CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
    run->AddTask(richHitProd);

    CbmRichReconstruction* richReco = new CbmRichReconstruction();
    run->AddTask(richReco);
    std::cout << "-I- : Added task " << richReco->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  /*
  // -----   ECAL reconstruction   ------------------------------------------
  if ( setup->IsActive(ECbmModuleId::kEcal) ) {

    CbmEcalHitProducerFastMC* ecalHit =
        new CbmEcalHitProducerFastMC("ECAL HitProducer");
    run->AddTask(ecalHit);
    std::cout << "-I- : Added task " << ecalHit->GetName() << std::endl;

  }
  // ------------------------------------------------------------------------
*/

  // -----   Track matching  -----------------------------------------------
  if (useMC) {
    CbmMatchRecoToMC* match2 = new CbmMatchRecoToMC();
    run->AddTask(match2);
  }
  // -------------------------------------------------------------------------

  return kTRUE;
}
