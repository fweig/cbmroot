/* Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig [committer] */

/** Macro module for registration of digitiser tasks to a run
 **
 ** The run instance can either be FairRunSim or FairRunAna.
 ** The macro assumes a CbmSetup instance. The digitisers for the
 ** active detectors within the setup are instantiated.
 **
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 10 February 2016
 **/


Bool_t reconstruct()
{


  // -----   Get the run instance   ------------------------------------------
  FairRun* run = FairRun::Instance();
  if (!run) {
    std::cerr << "-E- digitize: No run instance!" << std::endl;
    return kFALSE;
  }
  std::cout << std::endl;
  std::cout << "-I- Macro reconstruct.C called for run " << run->GetName() << std::endl;
  // -------------------------------------------------------------------------


  // -----   Get the CBM setup instance   ------------------------------------
  CbmSetup* setup = CbmSetup::Instance();
  std::cout << std::endl;
  std::cout << "-I- digitize: Found setup " << setup->GetTitle() << std::endl;
  // -------------------------------------------------------------------------


  // -----   Local reconstruction in MVD   ----------------------------------
  if (setup->IsActive(kMvd)) {

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
  if (setup->IsActive(kSts)) {

    CbmStsFindClusters* stsCluster = new CbmStsFindClusters();
    stsCluster->UseEventMode();
    run->AddTask(stsCluster);
    std::cout << "-I- : Added task " << stsCluster->GetName() << std::endl;

    FairTask* stsHit = new CbmStsFindHitsEvents();
    run->AddTask(stsHit);
    std::cout << "-I- : Added task " << stsHit->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in MUCH   ---------------------------------
  if (setup->IsActive(kMuch)) {

    // --- Parameter file name
    TString geoTag;
    setup->GetGeoTag(kMuch, geoTag);
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


  // -----   Local reconstruction in TRD   ----------------------------------
  if (setup->IsActive(kTrd)) {

    Double_t triggerThreshold           = 0.5e-6;  // SIS100
    Bool_t triangularPads               = false;   // Bucharest triangular pad-plane layout
    CbmTrdClusterFinderFast* trdCluster = new CbmTrdClusterFinderFast();
    trdCluster->SetNeighbourTrigger(true);
    trdCluster->SetTriggerThreshold(triggerThreshold);
    trdCluster->SetNeighbourRowTrigger(false);
    trdCluster->SetPrimaryClusterRowMerger(true);
    trdCluster->SetTriangularPads(triangularPads);
    run->AddTask(trdCluster);
    std::cout << "-I- : Added task " << trdCluster->GetName() << std::endl;

    CbmTrdHitProducerCluster* trdHit = new CbmTrdHitProducerCluster();
    trdHit->SetTriangularPads(triangularPads);
    run->AddTask(trdHit);
    std::cout << "-I- : Added task " << trdHit->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TOF   ----------------------------------
  if (setup->IsActive(kTof)) {
    CbmTofSimpClusterizer* tofCluster = new CbmTofSimpClusterizer("TOF Simple Clusterizer", 0);
    tofCluster->SetOutputBranchPersistent("TofHit", kTRUE);
    tofCluster->SetOutputBranchPersistent("TofDigiMatch", kTRUE);
    run->AddTask(tofCluster);
    std::cout << "-I- : Added task " << tofCluster->GetName() << std::endl;
  }
  // -------------------------------------------------------------------------

  /*
  // -----   Track finding in (MVD+) STS    -----------------------------------------
  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);
  CbmL1* l1 = new CbmL1();
  // --- Material budget file names
  TString mvdGeoTag;
  if ( setup->GetGeoTag(kMvd, mvdGeoTag) ) {
    TString parFile = gSystem->Getenv("VMCWORKDIR");
    parFile = parFile + "/parameters/mvd/mvd_matbudget_" + mvdGeoTag + ".root";
    std::cout << "Using material budget file " << parFile << std::endl;
    l1->SetMvdMaterialBudgetFileName(parFile.Data());
  }
  TString stsGeoTag;
  if ( setup->GetGeoTag(kSts, stsGeoTag) ) {
    TString parFile = gSystem->Getenv("VMCWORKDIR");
    parFile = parFile + "/parameters/sts/sts_matbudget_" + stsGeoTag + ".root";
    std::cout << "Using material budget file " << parFile << std::endl;
    l1->SetStsMaterialBudgetFileName(parFile.Data());
  }
  run->AddTask(l1);
  std::cout << "-I- : Added task " << l1->GetName() << std::endl;

  CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
  FairTask* stsFindTracks = new CbmStsFindTracksEvents(stsTrackFinder, setup->IsActive(kMvd));
  run->AddTask(stsFindTracks);
  std::cout << "-I- : Added task " << stsFindTracks->GetName() << std::endl;
  // -------------------------------------------------------------------------

  // -----   Primary vertex finding   ---------------------------------------
  CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
  CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
  run->AddTask(findVertex);
  std::cout << "-I- : Added task " << findVertex->GetName() << std::endl;
  // -------------------------------------------------------------------------

  // ---   Global track finding   --------------------------------------------
  CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
  finder->SetTrackingType("branch");
  finder->SetMergerType("nearest_hit");
  run->AddTask(finder);
  std::cout << "-I- : Added task " << finder->GetName() << std::endl;
  // -------------------------------------------------------------------------
*/


  // -----   RICH reconstruction   ------------------------------------------
  if (setup->IsActive(kRich)) {

    CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
    run->AddTask(richHitProd);

    CbmRichReconstruction* richReco = new CbmRichReconstruction();
    richReco->UseMCbmSetup();
    run->AddTask(richReco);
    std::cout << "-I- : Added task " << richReco->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  /*
  // -----   ECAL reconstruction   ------------------------------------------
  if ( setup->IsActive(kEcal) ) {

    CbmEcalHitProducerFastMC* ecalHit =
    		new CbmEcalHitProducerFastMC("ECAL HitProducer");
    run->AddTask(ecalHit);
    std::cout << "-I- : Added task " << ecalHit->GetName() << std::endl;

  }
  // ------------------------------------------------------------------------
*/

  return kTRUE;
}
