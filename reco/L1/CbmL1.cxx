/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel, Sergey Gorbunov [committer], Valentina Akishina, Igor Kulakov, Maksym Zyzak */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction
 *
 *  Authors: I.Kisel,  S.Gorbunov
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de
 *
 *====================================================================
 *
 *  L1 main program
 *
 *====================================================================
 */

#include "CbmL1.h"

#include "CbmKF.h"
#include "CbmKFVertex.h"
#include "CbmL1PFFitter.h"
#include "CbmMCDataManager.h"
#include "CbmMuchTrackingInterface.h"
#include "CbmMvdTrackingInterface.h"
#include "CbmSetup.h"
#include "CbmStsTrackingInterface.h"
#include "CbmTofTrackingInterface.h"
#include "CbmTrdTrackingInterface.h"

#include <boost/filesystem.hpp>
// TODO: include of CbmSetup.h creates problems on Mac
// #include "CbmSetup.h"
#include "CbmEvent.h"
#include "CbmMCDataObject.h"
#include "CbmStsFindTracks.h"
#include "CbmStsHit.h"
#include "CbmTrackingDetectorInterfaceInit.h"

#include "FairEventHeader.h"
#include "FairField.h"
#include "FairRunAna.h"

#include "TGeoArb8.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TMatrixD.h"
#include "TNtuple.h"
#include "TProfile2D.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TVectorD.h"
#include <TFile.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>

#include "L1Algo/L1Algo.h"
#include "L1Algo/L1Assert.h"
#include "L1Algo/L1Branch.h"
#include "L1Algo/L1Field.h"
#include "L1Algo/L1Hit.h"
#include "L1Event.h"

using std::cout;
using std::endl;
using std::ios;

ClassImp(CbmL1);

static L1Algo gAlgo _fvecalignment;  // TODO: Change coupling logic between L1Algo and CbmL1

CbmL1* CbmL1::fpInstance = 0;


// ---------------------------------------------------------------------------------------------------------------------
//
CbmL1::CbmL1() : CbmL1("L1") {}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmL1::CbmL1(const char* name, Int_t verbose, Int_t performance, int dataMode, const char* dataDir,
             int findParticleMode)
  : FairTask(name, verbose)
  , fPerformance(performance)
  , fSTAPDataMode(dataMode)
  , fFindParticlesMode(findParticleMode)
{
  if (!fpInstance) fpInstance = this;

  switch (fSTAPDataMode) {
    case 1: LOG(info) << "CbmL1: input data will be written for a standalone usage"; break;
    case 2: LOG(info) << "CbmL1: input data will be read from external files"; break;
    default: LOG(info) << "CbmL1: tracking will be run without external data R/W"; break;
  }

  fpIODataManager = std::make_shared<L1IODataManager>();

  this->DefineSTAPNames(dataDir);

  if (!CbmTrackingDetectorInterfaceInit::Instance()) {
    LOG(fatal) << "CbmL1: CbmTrackingDetectorInterfaceInit instance was not found. Please, add it as a task to your "
                  "reco macro right before the KF and L1 tasks:\n"
               << "\033[1;30mrun->AddTask(new CbmTrackingDetectorInterfaceInit());\033[0m";
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmL1::~CbmL1()
{
  if (fpInstance == this) fpInstance = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::CheckDetectorPresence()
{
  fUseMVD  = fUseMVD && CbmSetup::Instance()->IsActive(ECbmModuleId::kMvd);
  fUseSTS  = fUseSTS && CbmSetup::Instance()->IsActive(ECbmModuleId::kSts);
  fUseMUCH = fUseMUCH && CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch);
  fUseTRD  = fUseTRD && CbmSetup::Instance()->IsActive(ECbmModuleId::kTrd);
  fUseTOF  = fUseTOF && CbmSetup::Instance()->IsActive(ECbmModuleId::kTof);
  {
    // TODO: temporary code!!
    // for a moment, the MVD digitizer doesn't work in TB mode
    // check the presence of MVD hits to make sure the MVD is really active
    if (!FairRootManager::Instance()->GetObject("MvdHit")) { fUseMVD = false; }
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::DisableTrackingStation(L1DetectorID detID, int iSt)
{
  if (L1DetectorID::kEND != detID) { fvmDisabledStationIDs[detID].insert(iSt); }
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmL1::ReInit()
{
  SetParContainers();
  return Init();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::SetInputConfigName(const char* filename)
{
  LOG(info) << "Reading parameters from configuration file: " << filename;
  LOG(warn) << "\033[1;31mParameters configuration for tracking is not finalized! To be used only for tests! \033[0m";
  fInitManager.SetInputConfigName(std::string(filename));
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmL1::Init()
{
  if (fVerbose > 1) {
    char y[20] = " [0;33;44m";         // yellow
    char Y[20] = " [1;33;44m";         // yellow bold
    char W[20] = " [1;37;44m";         // white bold
    char o[20] = " [0m\n";             // color off
    Y[0] = y[0] = W[0] = o[0] = 0x1B;  // escape character

    cout << endl << endl;
    cout << "  " << W << "                                                                 " << o;
    cout << "  " << W << "  ===////======================================================  " << o;
    cout << "  " << W << "  =                                                           =  " << o;
    cout << "  " << W << "  =                   " << Y << "L1 on-line finder" << W << "                       =  " << o;
    cout << "  " << W << "  =                                                           =  " << o;
    cout << "  " << W << "  =  " << W << "Cellular Automaton 3.1 Vector" << y << " with " << W << "KF Quadro" << y
         << " technology" << W << "  =  " << o;
    cout << "  " << W << "  =                                                           =  " << o;
    cout << "  " << W << "  =  " << y << "Designed for CBM collaboration" << W << "                           =  " << o;
    cout << "  " << W << "  =  " << y << "All rights reserved" << W << "                                      =  " << o;
    cout << "  " << W << "  =                                                           =  " << o;
    cout << "  " << W << "  ========================================================////=  " << o;
    cout << "  " << W << "                                                                 " << o;
    cout << endl << endl;
  }

  if (fVerbose > 1) {
#ifdef _OPENMP
    LOG(info) << "L1 is running with OpenMP" << endl;
#else
    LOG(info) << "L1 is running without OpenMP" << endl;
#endif
  }

  fHistoDir = gROOT->mkdir("L1");
  fHistoDir->mkdir("Input");
  fHistoDir->mkdir("Fit");

  fTableDir = gROOT->mkdir("L1Tables");

  // turn on reconstruction in sub-detectors

  fUseMVD  = false;
  fUseSTS  = false;
  fUseMUCH = false;
  fUseTRD  = false;
  fUseTOF  = false;

  FairRootManager* fairManager = FairRootManager::Instance();

  if (L1Algo::TrackingMode::kSts == fTrackingMode) {
    fUseMVD  = 1;
    fUseSTS  = 1;
    fUseMUCH = 0;
    fUseTRD  = 0;
    fUseTOF  = 0;
    // check if MVD is switched off in the Sts task
    CbmStsFindTracks* findTask = L1_DYNAMIC_CAST<CbmStsFindTracks*>(FairRunAna::Instance()->GetTask("STSFindTracks"));
    if (findTask) fUseMVD = findTask->MvdUsage();
  }

  if (L1Algo::TrackingMode::kMcbm == fTrackingMode) {
    fUseMVD  = 1;
    fUseSTS  = 1;
    fUseMUCH = 1;
    fUseTRD  = 1;
    fUseTOF  = 1;
    fInitManager.DevSetIgnoreHitSearchAreas(true);  // uncomment for debug
  }

  if (L1Algo::TrackingMode::kGlobal == fTrackingMode) {
    //at the moment trd2d tracking only
    fUseMVD  = 0;
    fUseSTS  = 0;
    fUseMUCH = 0;
    fUseTRD  = 1;
    fUseTOF  = 0;

    fInitManager.DevSetUseOfOriginalField();
    fInitManager.DevSetIgnoreHitSearchAreas(true);
    fInitManager.SetMaxTripletPerDoublets(1000);
  }

  //fInitManager.DevSetIsMatchDoubletsViaMc(true);
  //fInitManager.DevSetIsMatchTripletsViaMc(true);
  //fInitManager.DevSetIsExtendTracksViaMc(true);
  //fInitManager.DevSetIsSuppressOverlapHitsViaMc(true);

  CheckDetectorPresence();

  fpStsPoints  = 0;
  fpMvdPoints  = 0;
  fpMuchPoints = 0;
  fpTrdPoints  = 0;
  fpTofPoints  = 0;
  fpMcTracks   = 0;

  fpMvdHitMatches  = 0;
  fpTrdHitMatches  = 0;
  fpMuchHitMatches = 0;
  fpTofHitMatches  = 0;

  fpStsClusters = 0;

  fvSelectedMcEvents.clear();

  fTimeSlice = (CbmTimeSlice*) fairManager->GetObject("TimeSlice.");
  if (!fTimeSlice) { LOG(fatal) << GetName() << ": No time slice branch in the tree!"; }

  fpStsClusters       = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("StsCluster"));
  fpStsHitMatches     = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("StsHitMatch"));
  fpStsClusterMatches = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("StsClusterMatch"));

  fpStsHits = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("StsHit"));

  if (!fUseMUCH) {
    fpMuchPixelHits   = nullptr;
    fpMuchDigis       = nullptr;  // NOTE: Used only with fPerformance = true
    fpMuchDigiMatches = nullptr;  // NOTE: Used only with fPerformance = true
    fpMuchClusters    = nullptr;  // NOTE: Used only with fPerformance = true
    fpMuchPoints      = nullptr;  // NOTE: Used only with fPerformance = true
    fpMuchHitMatches  = nullptr;  // NOTE: Used only with fPerformance = true
  }
  else {
    fpMuchPixelHits = (TClonesArray*) fairManager->GetObject("MuchPixelHit");
  }

  if (!fUseTRD) {
    fpTrdPoints     = nullptr;
    fpTrdHitMatches = nullptr;
    fpTrdHits       = nullptr;
  }
  else {
    fpTrdHits = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("TrdHit"));
  }

  if (!fUseTOF) {
    fpTofPoints     = nullptr;
    fpTofHitMatches = nullptr;
    fpTofHits       = nullptr;
  }
  else {
    fpTofHits = (TClonesArray*) fairManager->GetObject("TofHit");
  }

  if (fPerformance) {
    CbmMCDataManager* mcManager = (CbmMCDataManager*) fairManager->GetObject("MCDataManager");
    if (nullptr == mcManager) LOG(fatal) << GetName() << ": No CbmMCDataManager!";

    fpMcEventHeader = mcManager->GetObject("MCEventHeader.");

    fpMcTracks = mcManager->InitBranch("MCTrack");

    if (nullptr == fpMcTracks) LOG(fatal) << GetName() << ": No MCTrack data!";
    if (nullptr == fpMcEventHeader) LOG(fatal) << GetName() << ": No MC event header data!";

    fpMcEventList = (CbmMCEventList*) fairManager->GetObject("MCEventList.");
    if (nullptr == fpMcEventList) LOG(fatal) << GetName() << ": No MCEventList data!";

    if (fUseMVD) {
      fpMvdPoints      = mcManager->InitBranch("MvdPoint");
      fpMvdDigiMatches = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("MvdDigiMatch"));
      fpMvdHitMatches  = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("MvdHitMatch"));
      if (!fpMvdHitMatches) { LOG(error) << "No fpMvdHitMatches provided, performance is not done correctly"; }
    }

    if (fUseSTS) {
      fpStsPoints = mcManager->InitBranch("StsPoint");
      if (nullptr == fpStsPoints) LOG(fatal) << GetName() << ": No StsPoint data!";
    }

    fpTrdPoints = mcManager->InitBranch("TrdPoint");

    if (!fUseTRD) { fpTrdHitMatches = 0; }
    else {
      fpTrdHitMatches = (TClonesArray*) fairManager->GetObject("TrdHitMatch");
    }

    if (!fUseMUCH) {
      fpMuchPoints     = 0;
      fpMuchHitMatches = 0;
    }
    else {
      fpMuchDigis       = (TClonesArray*) fairManager->GetObject("MuchDigi");
      fpMuchDigiMatches = (TClonesArray*) fairManager->GetObject("MuchDigiMatch");
      fpMuchClusters    = (TClonesArray*) fairManager->GetObject("MuchCluster");
      fpMuchPoints      = mcManager->InitBranch("MuchPoint");
      fpMuchHitMatches  = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("MuchPixelHitMatch"));
    }

    fpTofPoints = mcManager->InitBranch("TofPoint");

    if (!fUseTOF) { fpTofHitMatches = 0; }
    else {
      fpTofHitMatches = static_cast<TClonesArray*>(fairManager->GetObject("TofHitMatch"));
    }
  }

  if (!fUseMVD) { fpMvdHits = 0; }
  else {
    fpMvdHits = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("MvdHit"));
  }

  // *****************************
  // ** Geometry initialization **
  // *****************************

  // Read parameters object from a binary file
  if (2 == fSTAPDataMode) {
    this->ReadSTAPParamObject();

    fNMvdStationsGeom  = fInitManager.GetNstationsGeometry(L1DetectorID::kMvd);
    fNStsStationsGeom  = fInitManager.GetNstationsGeometry(L1DetectorID::kSts);
    fNTrdStationsGeom  = fInitManager.GetNstationsGeometry(L1DetectorID::kTrd);
    fNMuchStationsGeom = fInitManager.GetNstationsGeometry(L1DetectorID::kMuch);
    fNTofStationsGeom  = fInitManager.GetNstationsGeometry(L1DetectorID::kTof);
    fNStationsGeom     = fInitManager.GetNstationsGeometry();

    if (fMissingHits) { CbmTofTrackingInterface::Instance()->FixHitsStationsMismatch(); }
  }
  // Parameters initialization, based on the FairRunAna chain
  else {
    auto mvdInterface  = CbmMvdTrackingInterface::Instance();
    auto stsInterface  = CbmStsTrackingInterface::Instance();
    auto muchInterface = CbmMuchTrackingInterface::Instance();
    auto trdInterface  = CbmTrdTrackingInterface::Instance();
    auto tofInterface  = CbmTofTrackingInterface::Instance();

    // NOTE: hack for "mcbm_beam_2021_07_surveyed" to account for a mismactch in the station
    //       indeces of hits in TOF
    if (fMissingHits) { tofInterface->FixHitsStationsMismatch(); }

    fNMvdStationsGeom  = (fUseMVD) ? mvdInterface->GetNtrackingStations() : 0;
    fNStsStationsGeom  = (fUseSTS) ? stsInterface->GetNtrackingStations() : 0;
    fNMuchStationsGeom = (fUseMUCH) ? muchInterface->GetNtrackingStations() : 0;
    fNTrdStationsGeom  = (fUseTRD) ? trdInterface->GetNtrackingStations() : 0;
    fNTofStationsGeom  = (fUseTOF) ? tofInterface->GetNtrackingStations() : 0;
    fNStationsGeom = fNMvdStationsGeom + fNStsStationsGeom + fNMuchStationsGeom + fNTrdStationsGeom + fNTofStationsGeom;

    // **************************
    // ** Field initialization **
    // **************************

    if (FairRunAna::Instance()->GetField()) {
      fInitManager.SetFieldFunction([](const double(&inPos)[3], double(&outB)[3]) {
        FairRunAna::Instance()->GetField()->GetFieldValue(inPos, outB);
      });
    }
    else {
      fInitManager.SetFieldFunction([](const double(&)[3], double(&outB)[3]) {
        outB[0] = 0.;
        outB[1] = 0.;
        outB[2] = 0.;
      });
    }

    // ***************************
    // ** Target initialization **
    // ***************************

    auto& target = CbmKF::Instance()->vTargets[0];
    fInitManager.SetTargetPosition(target.x, target.y, target.z);

    // *********************************
    // ** Target field initialization **
    // *********************************

    fInitManager.InitTargetField(/*zStep = */ 2.5 /*cm*/);  // Replace zStep -> sizeZfieldRegion = 2 * zStep (TODO)

    // **************************************
    // **                                  **
    // ** STATIONS GEOMETRY INITIALIZATION **
    // **                                  **
    // **************************************


    // ***************************************************
    // ** Active tracking detector subsystems selection **
    // ***************************************************

    std::set<L1DetectorID> vActiveTrackingDetectorIDs {};  // Set of detectors active in tracking

    if (fUseMVD) { vActiveTrackingDetectorIDs.insert(L1DetectorID::kMvd); }
    if (fUseSTS) { vActiveTrackingDetectorIDs.insert(L1DetectorID::kSts); }
    if (fUseMUCH) { vActiveTrackingDetectorIDs.insert(L1DetectorID::kMuch); }
    if (fUseTRD) { vActiveTrackingDetectorIDs.insert(L1DetectorID::kTrd); }
    if (fUseTOF) { vActiveTrackingDetectorIDs.insert(L1DetectorID::kTof); }
    fInitManager.SetActiveDetectorIDs(vActiveTrackingDetectorIDs);

    // *********************************************************************
    // ** Counting numbers of stations for different detector subsystems  **
    // *********************************************************************


    // Provide crosscheck number of stations for the fInitManagera
    fInitManager.SetNstations(L1DetectorID::kMvd, fNMvdStationsGeom);
    fInitManager.SetNstations(L1DetectorID::kSts, fNStsStationsGeom);
    fInitManager.SetNstations(L1DetectorID::kMuch, fNMuchStationsGeom);
    fInitManager.SetNstations(L1DetectorID::kTrd, fNTrdStationsGeom);
    fInitManager.SetNstations(L1DetectorID::kTof, fNTofStationsGeom);


    // ***************************************
    // ** Stations geometry initialization  **
    // ***************************************

    // *** MVD stations info ***
    if (fUseMVD) {
      auto materialTableMvd = ReadMaterialBudget(L1DetectorID::kMvd);
      for (int iSt = 0; iSt < fNMvdStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kMvd, iSt);
        stationInfo.SetStationType(1);  // MVD
        stationInfo.SetTimeInfo(mvdInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetFieldStatus(fTrackingMode == L1Algo::TrackingMode::kMcbm ? 0 : 1);
        stationInfo.SetZ(mvdInterface->GetZ(iSt));
        stationInfo.SetXmax(mvdInterface->GetXmax(iSt));
        stationInfo.SetYmax(mvdInterface->GetYmax(iSt));
        stationInfo.SetRmin(mvdInterface->GetRmin(iSt));
        stationInfo.SetRmax(mvdInterface->GetRmax(iSt));
        stationInfo.SetZthickness(mvdInterface->GetThickness(iSt));
        stationInfo.SetMaterialMap(std::move(materialTableMvd[iSt]));
        // TODO: The CA TF result is dependent from type of geometry settings. Should be understood (S.Zharko)
        stationInfo.SetFrontBackStripsGeometry((fscal) mvdInterface->GetStripsStereoAngleFront(iSt),
                                               (fscal) mvdInterface->GetStripsStereoAngleBack(iSt));
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
        if (fvmDisabledStationIDs[L1DetectorID::kMvd].find(iSt) != fvmDisabledStationIDs[L1DetectorID::kMvd].cend()) {
          stationInfo.SetTrackingStatus(false);
        }
        fInitManager.AddStation(stationInfo);
        LOG(info) << "- MVD station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
      }
    }

    // *** STS stations info ***
    if (fUseSTS) {
      auto materialTableSts = ReadMaterialBudget(L1DetectorID::kSts);
      for (int iSt = 0; iSt < fNStsStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kSts, iSt);
        stationInfo.SetStationType(0);  // STS
        stationInfo.SetTimeInfo(stsInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetTimeInfo(L1Algo::TrackingMode::kMcbm != fTrackingMode ? stsInterface->IsTimeInfoProvided(iSt)
                                                                             : false);
        stationInfo.SetFieldStatus(L1Algo::TrackingMode::kMcbm == fTrackingMode ? 0 : 1);
        stationInfo.SetZ(stsInterface->GetZ(iSt));
        stationInfo.SetXmax(stsInterface->GetXmax(iSt));
        stationInfo.SetYmax(stsInterface->GetYmax(iSt));
        stationInfo.SetRmin(stsInterface->GetRmin(iSt));
        stationInfo.SetRmax(stsInterface->GetRmax(iSt));
        stationInfo.SetZthickness(stsInterface->GetThickness(iSt));
        stationInfo.SetMaterialMap(std::move(materialTableSts[iSt]));
        // TODO: The CA TF result is dependent from type of geometry settings. Should be understood (S.Zharko)
        stationInfo.SetFrontBackStripsGeometry((fscal) stsInterface->GetStripsStereoAngleFront(iSt),
                                               (fscal) stsInterface->GetStripsStereoAngleBack(iSt));
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
        if (fvmDisabledStationIDs[L1DetectorID::kSts].find(iSt) != fvmDisabledStationIDs[L1DetectorID::kSts].cend()) {
          stationInfo.SetTrackingStatus(false);
        }
        fInitManager.AddStation(stationInfo);
        LOG(info) << "- STS station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
      }
    }

    // *** MuCh stations info ***
    if (fUseMUCH) {
      auto materialTableMuch = ReadMaterialBudget(L1DetectorID::kMuch);
      for (int iSt = 0; iSt < fNMuchStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kMuch, iSt);
        stationInfo.SetStationType(2);  // MuCh
        stationInfo.SetTimeInfo(muchInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetTimeInfo(L1Algo::TrackingMode::kMcbm != fTrackingMode ? stsInterface->IsTimeInfoProvided(iSt)
                                                                             : false);
        stationInfo.SetFieldStatus(0);
        stationInfo.SetZ(muchInterface->GetZ(iSt));
        stationInfo.SetXmax(muchInterface->GetXmax(iSt));
        stationInfo.SetYmax(muchInterface->GetYmax(iSt));
        stationInfo.SetRmin(muchInterface->GetRmin(iSt));
        stationInfo.SetRmax(muchInterface->GetRmax(iSt));
        stationInfo.SetZthickness(muchInterface->GetThickness(iSt));
        stationInfo.SetMaterialMap(std::move(materialTableMuch[iSt]));
        // TODO: The CA TF result is dependent from type of geometry settings. Should be understood (S.Zharko)
        stationInfo.SetFrontBackStripsGeometry((fscal) muchInterface->GetStripsStereoAngleFront(iSt),
                                               (fscal) muchInterface->GetStripsStereoAngleBack(iSt));
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
        if (fvmDisabledStationIDs[L1DetectorID::kMuch].find(iSt) != fvmDisabledStationIDs[L1DetectorID::kMuch].cend()) {
          stationInfo.SetTrackingStatus(false);
        }
        fInitManager.AddStation(stationInfo);
        LOG(info) << "- MuCh station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
      }
    }

    // *** TRD stations info ***
    if (fUseTRD) {
      auto materialTableTrd = ReadMaterialBudget(L1DetectorID::kTrd);
      for (int iSt = 0; iSt < fNTrdStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kTrd, iSt);
        stationInfo.SetStationType((iSt == 1 || iSt == 3) ? 6 : 3);  // MuCh
        stationInfo.SetTimeInfo(trdInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetTimeInfo(L1Algo::TrackingMode::kMcbm != fTrackingMode ? stsInterface->IsTimeInfoProvided(iSt)
                                                                             : false);
        stationInfo.SetFieldStatus(0);
        stationInfo.SetZ(trdInterface->GetZ(iSt));
        stationInfo.SetXmax(trdInterface->GetXmax(iSt));
        stationInfo.SetYmax(trdInterface->GetYmax(iSt));
        stationInfo.SetRmin(trdInterface->GetRmin(iSt));
        stationInfo.SetRmax(trdInterface->GetRmax(iSt));
        stationInfo.SetZthickness(trdInterface->GetThickness(iSt));
        stationInfo.SetMaterialMap(std::move(materialTableTrd[iSt]));
        fscal trdFrontPhi = trdInterface->GetStripsStereoAngleFront(iSt);
        fscal trdBackPhi  = trdInterface->GetStripsStereoAngleBack(iSt);
        if (L1Algo::TrackingMode::kGlobal == fTrackingMode) { stationInfo.SetTimeInfo(false); }
        stationInfo.SetFrontBackStripsGeometry(trdFrontPhi, trdBackPhi);
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
        if (iSt == 1 && L1Algo::TrackingMode::kMcbm == fTrackingMode && fMissingHits) {
          stationInfo.SetTrackingStatus(false);
        }
        if (fvmDisabledStationIDs[L1DetectorID::kTrd].find(iSt) != fvmDisabledStationIDs[L1DetectorID::kTrd].cend()) {
          stationInfo.SetTrackingStatus(false);
        }
        fInitManager.AddStation(stationInfo);
        LOG(info) << "- TRD station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
      }
    }

    // *** TOF stations info ***
    if (fUseTOF) {
      auto materialTableTof = ReadMaterialBudget(L1DetectorID::kTof);
      for (int iSt = 0; iSt < fNTofStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kTof, iSt);
        stationInfo.SetStationType(4);
        stationInfo.SetTimeInfo(tofInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetTimeInfo(L1Algo::TrackingMode::kMcbm != fTrackingMode ? stsInterface->IsTimeInfoProvided(iSt)
                                                                             : false);
        stationInfo.SetFieldStatus(0);
        stationInfo.SetZ(tofInterface->GetZ(iSt));
        auto thickness = tofInterface->GetThickness(iSt);
        stationInfo.SetZthickness(thickness);
        stationInfo.SetMaterialMap(std::move(materialTableTof[iSt]));
        stationInfo.SetXmax(tofInterface->GetXmax(iSt));
        stationInfo.SetYmax(tofInterface->GetYmax(iSt));
        stationInfo.SetRmin(tofInterface->GetRmin(iSt));
        stationInfo.SetRmax(tofInterface->GetRmax(iSt));
        fscal tofFrontPhi = tofInterface->GetStripsStereoAngleFront(iSt);
        fscal tofBackPhi  = tofInterface->GetStripsStereoAngleBack(iSt);
        stationInfo.SetFrontBackStripsGeometry(tofFrontPhi, tofBackPhi);
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
        if (fvmDisabledStationIDs[L1DetectorID::kTof].find(iSt) != fvmDisabledStationIDs[L1DetectorID::kTof].cend()) {
          stationInfo.SetTrackingStatus(false);
        }
        fInitManager.AddStation(stationInfo);
        LOG(info) << "- TOF station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
      }
    }

    // ****************************************
    // **                                    **
    // ** TRACKING ITERATIONS INITIALIZATION **
    // **                                    **
    // ****************************************

    // TODO: Need to provide a selection: default iterations input (these hard-coded ones), input from file or input
    //       from running macro (S.Zharko)

    auto trIterDefault = L1CAIteration("default");
    trIterDefault.SetMinNhits(4);
    trIterDefault.SetMinNhitsStation0(4);
    trIterDefault.SetDoubletChi2Cut(7.56327f);       // = 1.3449 * 2.f / 3.f;  // prob = 0.1
    trIterDefault.SetTripletChi2Cut(23.4450f);       // = 7.815 * 3;  // prob = 0.05
    trIterDefault.SetTripletFinalChi2Cut(23.4450f);  //new 7.5);
    trIterDefault.SetTripletLinkChi2(25.);           // new:10
    trIterDefault.SetTrackChi2Cut(10.);
    trIterDefault.SetMaxSlope(2.748f);
    trIterDefault.SetMaxSlopePV(1.1f);
    trIterDefault.SetMaxDZ(0.1);
    trIterDefault.SetPickGather(4.0f);
    trIterDefault.SetExtendTracksFlag(true);

    auto trackingIterFastPrim = L1CAIteration(trIterDefault, "FastPrimIter");
    trackingIterFastPrim.SetMinNhitsStation0(3);
    trackingIterFastPrim.SetPickGather(3.0f);
    trackingIterFastPrim.SetMaxQp(1.0 / 0.5);
    trackingIterFastPrim.SetMaxDZ(0);
    trackingIterFastPrim.SetTargetPosSigmaXY(1, 1);
    trackingIterFastPrim.SetPrimaryFlag(true);

    auto trackingIterAllPrim = L1CAIteration(trIterDefault, "AllPrimIter");
    trackingIterAllPrim.SetMinNhitsStation0(3);
    trackingIterAllPrim.SetMaxQp(1.0 / 0.05);
    trackingIterAllPrim.SetTargetPosSigmaXY(1, 1);
    trackingIterAllPrim.SetPrimaryFlag(true);

    auto trackingIterFastPrim2 = L1CAIteration(trIterDefault, "FastPrim2Iter");
    trackingIterFastPrim2.SetTripletChi2Cut(21.1075f);
    trackingIterFastPrim2.SetTripletFinalChi2Cut(21.1075f);
    trackingIterFastPrim2.SetPickGather(3.0f);
    trackingIterFastPrim2.SetMaxQp(1.0 / 0.5);
    trackingIterFastPrim2.SetMaxDZ(0);
    trackingIterFastPrim2.SetTargetPosSigmaXY(5, 5);
    trackingIterFastPrim2.SetPrimaryFlag(true);

    auto trackingIterAllSec = L1CAIteration(trIterDefault, "AllSecIter");
    trackingIterAllSec.SetTripletChi2Cut(18.7560f);  // = 6.252 * 3;  // prob = 0.1
    trackingIterAllSec.SetTripletFinalChi2Cut(18.7560f);
    trackingIterAllSec.SetMaxQp(1.0 / 0.1);
    trackingIterAllSec.SetTargetPosSigmaXY(10, 10);
    trackingIterAllSec.SetMaxSlopePV(1.5);
    trackingIterAllSec.SetPrimaryFlag(false);

    auto trackingIterFastPrimJump = L1CAIteration(trIterDefault, "FastPrimJumpIter");
    trackingIterFastPrimJump.SetTripletChi2Cut(21.1075f);  // prob = 0.01
    trackingIterFastPrimJump.SetTripletFinalChi2Cut(21.1075f);
    trackingIterFastPrimJump.SetPickGather(3.0f);
    trackingIterFastPrimJump.SetMaxQp(1.0 / 0.5);
    trackingIterFastPrimJump.SetMaxDZ(0);
    trackingIterFastPrimJump.SetTargetPosSigmaXY(5, 5);
    trackingIterFastPrimJump.SetPrimaryFlag(true);
    trackingIterFastPrimJump.SetJumpedFlag(true);

    auto trackingIterAllPrimJump = L1CAIteration(trIterDefault, "AllPrimJumpIter");
    trackingIterAllPrimJump.SetTripletChi2Cut(18.7560f);
    trackingIterAllPrimJump.SetTripletFinalChi2Cut(18.7560f);
    trackingIterAllPrimJump.SetMaxQp(1.0 / 0.1);
    trackingIterAllPrimJump.SetTargetPosSigmaXY(5, 5);
    trackingIterAllPrimJump.SetPrimaryFlag(true);
    trackingIterAllPrimJump.SetJumpedFlag(true);

    auto trackingIterAllSecJump = L1CAIteration(trIterDefault, "AllSecJumpIter");
    trackingIterAllSecJump.SetTripletChi2Cut(21.1075f);
    trackingIterAllSecJump.SetTripletFinalChi2Cut(21.1075f);
    trackingIterAllSecJump.SetMaxQp(1.0 / 0.1);
    trackingIterAllSecJump.SetMaxSlopePV(1.5f);
    trackingIterAllSecJump.SetTargetPosSigmaXY(10, 10);
    trackingIterAllSecJump.SetPrimaryFlag(false);
    trackingIterAllSecJump.SetJumpedFlag(true);

    auto trackingIterAllPrimE = L1CAIteration(trIterDefault, "AllPrimEIter");
    trackingIterAllPrimE.SetMinNhitsStation0(3);
    trackingIterAllPrimE.SetMaxQp(1.0 / 0.05);
    trackingIterAllPrimE.SetTargetPosSigmaXY(1, 1);
    trackingIterAllPrimE.SetPrimaryFlag(true);
    trackingIterAllPrimE.SetElectronFlag(true);

    auto trackingIterAllSecE = L1CAIteration(trIterDefault, "AllSecEIter");
    trackingIterAllSecE.SetTripletChi2Cut(18.7560f);
    trackingIterAllSecE.SetTripletFinalChi2Cut(18.7560f);
    trackingIterAllSecE.SetMaxQp(1.0 / 0.05);
    trackingIterAllSecE.SetMaxSlopePV(1.5f);
    trackingIterAllSecE.SetTargetPosSigmaXY(10, 10);
    trackingIterAllSecE.SetPrimaryFlag(false);
    trackingIterAllSecE.SetElectronFlag(true);

    // Select default track finder
    if (L1Algo::TrackingMode::kMcbm == fTrackingMode) {

      trackingIterFastPrim.SetMinNhits(4);
      trackingIterFastPrim.SetMinNhitsStation0(4);

      trackingIterAllSecE.SetMinNhits(4);
      trackingIterAllSecE.SetMinNhitsStation0(4);
      trackingIterAllPrim.SetMaxQp(1. / 0.1);
      trackingIterAllPrimE.SetMaxQp(1. / 0.1);
      trackingIterAllSecE.SetMaxQp(1. / 0.1);

      trackingIterAllSecJump.SetMinNhits(4);
      trackingIterAllSecJump.SetMinNhitsStation0(4);
      trackingIterFastPrim.SetMaxQp(1.0 / 0.3);
      trackingIterAllSec.SetMaxQp(1.0 / 0.3);
      trackingIterFastPrimJump.SetMaxQp(1.0 / 0.3);
      trackingIterAllPrimJump.SetMaxQp(1.0 / 0.3);
      trackingIterAllSecJump.SetMaxQp(1.0 / 0.3);

      trackingIterAllSec.SetMinNhits(4);
      trackingIterAllSec.SetMinNhitsStation0(4);
      trackingIterFastPrim.SetExtendTracksFlag(false);
      trackingIterAllPrim.SetExtendTracksFlag(false);
      trackingIterAllPrimJump.SetExtendTracksFlag(false);
      trackingIterAllSec.SetExtendTracksFlag(false);

      trackingIterFastPrim.SetMaxSlopePV(9.5);
      trackingIterAllPrim.SetMaxSlopePV(9.5);
      trackingIterAllPrimJump.SetMaxSlopePV(9.5);
      trackingIterAllSec.SetMaxSlopePV(9.5);

      fInitManager.SetCAIterationsNumberCrosscheck(4);
      // Initialize CA track finder iterations sequence
      fInitManager.PushBackCAIteration(trackingIterFastPrim);
      fInitManager.PushBackCAIteration(trackingIterAllPrim);
      fInitManager.PushBackCAIteration(trackingIterAllPrimJump);
      fInitManager.PushBackCAIteration(trackingIterAllSec);
    }
    else if (L1Algo::TrackingMode::kGlobal == fTrackingMode) {

      // Initialize CA track finder iterations sequence

      auto globalIterPrimFast = L1CAIteration("globalIterPrimFast");
      {
        auto& it = globalIterPrimFast;
        it.SetMinNhits(3);
        it.SetMinNhitsStation0(3);
        it.SetTrackChi2Cut(7.f);         //10.f
        it.SetTripletChi2Cut(23.4450f);  // = 7.815 * 3;  // prob = 0.05
        it.SetDoubletChi2Cut(7.56327f);  // = 1.3449 * 2.f / 3.f;  // prob = 0.1
        it.SetPickGather(3.0f);
        it.SetTripletLinkChi2(16.0);
        it.SetMaxQp(1.0 / 0.1);  //(1.0 / 0.5);
        it.SetMaxSlopePV(9.5);
        it.SetMaxSlope(.5f);
        it.SetMaxDZ(0.05);
        it.SetTargetPosSigmaXY(1., 1.);  //(1, 1);
        //it.SetPrimaryFlag(true);
        it.SetExtendTracksFlag(true);
        it.SetPrimaryFlag(false);
        //it.SetExtendTracksFlag(false);
        //it.SetFirstStationIndex(11);
      }

      auto trd2dIter2 = L1CAIteration("Trd2dIter2");
      trd2dIter2.SetMinNhits(3);
      trd2dIter2.SetMinNhitsStation0(3);
      trd2dIter2.SetTrackChi2Cut(7.f);              //10.f
      trd2dIter2.SetTripletChi2Cut(2 * 23.4450f);   // = 7.815 * 3;  // prob = 0.05
      trd2dIter2.SetDoubletChi2Cut(4. * 7.56327f);  // = 1.3449 * 2.f / 3.f;  // prob = 0.1
      trd2dIter2.SetPickGather(3.0f);
      trd2dIter2.SetTripletLinkChi2(16.);
      trd2dIter2.SetMaxQp(1.0 / 0.05);  //(1.0 / 0.5);
      trd2dIter2.SetMaxSlopePV(9.5f);
      trd2dIter2.SetMaxSlope(.5f);
      trd2dIter2.SetMaxDZ(0.05);
      trd2dIter2.SetTargetPosSigmaXY(8 * 10, 6 * 10);  //(1, 1);
      trd2dIter2.SetPrimaryFlag(false);
      trd2dIter2.SetExtendTracksFlag(true);
      //trd2dIter2.SetFirstStationIndex(12);

      auto trd2dIter3 = L1CAIteration("trd2dIter3");
      trd2dIter3.SetMinNhits(4);
      trd2dIter3.SetMinNhitsStation0(4);
      trd2dIter3.SetTrackChi2Cut(30);          //10.f
      trd2dIter3.SetTripletChi2Cut(15. * 2.);  // = 7.815 * 3;  // prob = 0.05
      trd2dIter3.SetDoubletChi2Cut(15.);       // = 1.3449 * 2.f / 3.f;  // prob = 0.1
      trd2dIter3.SetPickGather(3.0f);
      trd2dIter3.SetTripletLinkChi2(200.);
      trd2dIter3.SetMaxQp(1.0 / 0.1);  //(1.0 / 0.5);
      trd2dIter3.SetMaxSlopePV(9.5);
      trd2dIter3.SetMaxSlope(.4);  //.5f);
      trd2dIter3.SetMaxDZ(0.05);
      trd2dIter3.SetTargetPosSigmaXY(7 * 10, 6 * 10);  //(1, 1);
      trd2dIter3.SetPrimaryFlag(false);
      trd2dIter3.SetExtendTracksFlag(true);

      // Initialize CA track finder iterations sequence

      fInitManager.SetCAIterationsNumberCrosscheck(1);
      //fInitManager.PushBackCAIteration(trackingIterFastPrim);
      //fInitManager.PushBackCAIteration(globalIterPrimFast);
      fInitManager.PushBackCAIteration(trd2dIter3);
      /*
      fInitManager.SetCAIterationsNumberCrosscheck(5);
      fInitManager.PushBackCAIteration(trackingIterFastPrim);
      fInitManager.PushBackCAIteration(trackingIterAllPrim);
      fInitManager.PushBackCAIteration(trackingIterAllPrimJump);
      fInitManager.PushBackCAIteration(trackingIterAllSec);
       */
    }
    else {
      fInitManager.SetCAIterationsNumberCrosscheck(4);

      // Initialize CA track finder iterations sequence
      fInitManager.PushBackCAIteration(trackingIterFastPrim);
      fInitManager.PushBackCAIteration(trackingIterAllPrim);
      fInitManager.PushBackCAIteration(trackingIterAllPrimJump);
      fInitManager.PushBackCAIteration(trackingIterAllSec);
      //fInitManager.PushBackCAIteration(trackingIterAllPrimE);
      //fInitManager.PushBackCAIteration(trackingIterAllSecE);
      //fInitManager.PushBackCAIteration(trackingIterFastPrimJump);
      //fInitManager.PushBackCAIteration(trackingIterFastPrim2);
      //fInitManager.PushBackCAIteration(trackingIterAllSecJump);
    }

    // *******************************
    // ** Initialize search windows **
    // *******************************

    if (fsInputSearchWindowsFilename.size()) {
      fInitManager.DevSetIsParSearchWUsed();
      fInitManager.ReadSearchWindows(fsInputSearchWindowsFilename);
    }
    else {
      fInitManager.DevSetIsParSearchWUsed(false);
    }


    // **********************
    // ** Set special cuts **
    // **********************

    fInitManager.SetGhostSuppression(fGhostSuppression);
    fInitManager.SetTrackingLevel(fTrackingLevel);
    fInitManager.SetMomentumCutOff(fMomentumCutOff);

    // Form parameters container
    if (!fInitManager.FormParametersContainer()) { return kFATAL; }

    // Write to file if needed
    if (1 == fSTAPDataMode || 4 == fSTAPDataMode) { this->WriteSTAPParamObject(); }
  }


  // Init L1 algo core

  fpAlgo = &gAlgo;
  fpAlgo->Init(fTrackingMode, fMissingHits);

  //
  // ** Send formed parameters object to L1Algo instance **
  //
  fInitManager.SendParameters(fpAlgo);

  /*** Get numbers of active stations ***/

  fNMvdStations  = fpAlgo->GetParameters()->GetNstationsActive(L1DetectorID::kMvd);
  fNStsStations  = fpAlgo->GetParameters()->GetNstationsActive(L1DetectorID::kSts);
  fNTrdStations  = fpAlgo->GetParameters()->GetNstationsActive(L1DetectorID::kTrd);
  fNMuchStations = fpAlgo->GetParameters()->GetNstationsActive(L1DetectorID::kMuch);
  fNTofStations  = fpAlgo->GetParameters()->GetNstationsActive(L1DetectorID::kTof);
  fNStations     = fpAlgo->GetParameters()->GetNstationsActive();

  LOG(info) << fpAlgo->GetParameters()->ToString(0);
  fpIODataManager->SetNofActiveStations(fNStations);

  LOG(info) << "----- Numbers of stations active in tracking -----";
  LOG(info) << "  MVD:    " << fNMvdStations;
  LOG(info) << "  STS:    " << fNStsStations;
  LOG(info) << "  MuCh:   " << fNMuchStations;
  LOG(info) << "  TRD:    " << fNTrdStations;
  LOG(info) << "  ToF:    " << fNTofStations;
  LOG(info) << "  Total:  " << fNStations;

  return kSUCCESS;
}


void CbmL1::Reconstruct(CbmEvent* event)
{
  static int nevent = 0;

  fvSelectedMcEvents.clear();

  int bestMcFile  = -1;
  int bestMcEvent = -1;

  if (fPerformance) {
    if (event) {
      CbmMatch* match = event->GetMatch();
      if (!match) {
        LOG(error) << "CbmL1: match between reco and MC events missing!! Performance can not be evaluated!!";
        fPerformance = 0;
      }
      else {
        cout << "CbmL1: mc events all " << fpMcEventList->GetNofEvents() << " mc events linked " << match->GetNofLinks()
             << std::endl;
        for (int iLink = 0; iLink < match->GetNofLinks(); iLink++) {
          const CbmLink& link = match->GetLink(iLink);
          fvSelectedMcEvents.insert(DFSET::value_type(link.GetFile(), link.GetEntry()));
          cout << "CbmL1: linked mc event file " << link.GetFile() << " event  " << link.GetEntry() << std::endl;
        }
        if (match->GetNofLinks() > 1) {
          const CbmLink& link = match->GetMatchedLink();
          bestMcFile          = link.GetFile();
          bestMcEvent         = link.GetEntry();
        }
      }
    }
    else {
      int nofEvents = fpMcEventList->GetNofEvents();
      for (int iE = 0; iE < nofEvents; iE++) {
        int fileId  = fpMcEventList->GetFileIdByIndex(iE);
        int eventId = fpMcEventList->GetEventIdByIndex(iE);
        fvSelectedMcEvents.insert(DFSET::value_type(fileId, eventId));
      }
    }
  }

  if (fVerbose > 1) { cout << "\nCbmL1::Exec event " << ++nevent << " ...\n\n"; }
#ifdef _OPENMP
  omp_set_num_threads(1);
#endif

  // ----- Read data from branches and send data from IODataManager to L1Algo ----------------------------------------

  ReadEvent(event);

  if (fPerformance) {
    HitMatch();
    // calculate the max number of Hits\mcPoints on continuous(consecutive) stations
    for (auto it = fvMCTracks.begin(); it != fvMCTracks.end(); ++it) {
      it->Init();
    }
    if (bestMcFile >= 0) {  // suppress mc tracks from complementary mc events
      for (auto it = fvMCTracks.begin(); it != fvMCTracks.end(); ++it) {
        if (it->iFile != bestMcFile || it->iEvent != bestMcEvent) { it->SetIsReconstructable(false); }
      }
    }
  }

  //
  // tracker input performance is moved to external QA tasks.
  // InputPerformance() method is not optimised to run with the event builder
  // TODO: verify QA tasks and remove InputPerformance()
  // if ((fPerformance) && (fSTAPDataMode < 2)) { InputPerformance(); }
  //

  //  FieldApproxCheck();
  //  FieldIntegralCheck();

  if (fVerbose > 1) { LOG(info) << "L1 Track finder..."; }
  fpAlgo->CaTrackFinder();
  //       IdealTrackFinder();
  fTrackingTime = fpAlgo->fCaRecoTime;

  if (fVerbose > 1) { LOG(info) << "L1 Track finder ok"; }

  // save reconstructed tracks

  fvRecoTracks.clear();
  fvRecoTracks.reserve(fpAlgo->fRecoTracks.size());

  int trackFirstHit = 0;

  for (L1Vector<L1Track>::iterator it = fpAlgo->fRecoTracks.begin(); it != fpAlgo->fRecoTracks.end();
       trackFirstHit += it->NHits, it++) {

    CbmL1Track t;

    for (int i = 0; i < L1TrackPar::kNparTr; i++) {
      t.T[i]     = it->TFirst[i];
      t.TLast[i] = it->TLast[i];
      t.Tpv[i]   = it->Tpv[i];
    }

    for (int i = 0; i < L1TrackPar::kNparCov; ++i) {
      t.C[i]     = it->CFirst[i];
      t.CLast[i] = it->CLast[i];
      t.Cpv[i]   = it->Cpv[i];
    }

    t.chi2 = it->chi2;
    t.NDF  = it->NDF;
    t.Hits.clear();
    t.mass        = fpAlgo->fDefaultMass;  // pion mass
    t.is_electron = 0;
    t.SetId(fvRecoTracks.size());

    for (int i = 0; i < it->NHits; i++) {
      int caHitId  = fpAlgo->fRecoHits[trackFirstHit + i];
      int cbmHitID = fpAlgo->GetInputData().GetHit(caHitId).ID;
      t.Hits.push_back(cbmHitID);
    }
    fvRecoTracks.push_back(t);
  }

  LOG(debug) << "CA Track Finder: " << fpAlgo->fCaRecoTime << " s/sub-ts";


  if ((fPerformance) && (fSTAPDataMode < 2)) { InputPerformance(); }


  // output performance
  if (fPerformance) {
    if (fVerbose > 1) { LOG(info) << "Performance..."; }


    TrackMatch();
    EfficienciesPerformance();
    HistoPerformance();
    TrackFitPerformance();
    if (fsMcTripletsOutputFilename.size()) { DumpMCTripletsToTree(); }
    // TimeHist();
    ///    WriteSIMDKFData();
  }
  if (fVerbose > 1) { LOG(info) << "Tracking performance... done"; }
  if (fVerbose > 1) { LOG(info) << "End of CA"; }

  static bool ask = 0;
  char symbol;
  if (ask) {
    LOG(info) << "\nCA run (any/r/q) > ";
    do {
      std::cin.get(symbol);
      if (symbol == 'r') ask = false;
      if ((symbol == 'e') || (symbol == 'q')) exit(0);
    } while (symbol != '\n');
  }
}

// -----   Finish CbmStsFitPerformanceTask task   -----------------------------
void CbmL1::Finish()
{
  TDirectory* curr   = gDirectory;
  TFile* currentFile = gFile;


  // Open output file and write histograms
  boost::filesystem::path p = (FairRunAna::Instance()->GetUserOutputFileName()).Data();
  std::string dir           = p.parent_path().string();
  if (dir.empty()) dir = ".";
  {
    std::string histoOutName = dir + "/L1_histo_" + p.filename().string();
    LOG(info) << "\033[31;1mL1 performance histograms will be saved to: \033[0m" << histoOutName;
    TFile* outfile = new TFile(histoOutName.c_str(), "RECREATE");
    outfile->cd();
    writedir2current(fHistoDir);
    outfile->Close();
    outfile->Delete();
  }
  {
    std::string tablesOutName = dir + "/L1_perftable_" + p.filename().string();
    LOG(info) << "\033[31;1mL1 performance tables will be saved to: \033[0m" << tablesOutName;
    TFile* outfile = new TFile(tablesOutName.c_str(), "RECREATE");
    outfile->cd();
    writedir2current(fTableDir);
    outfile->Close();
    outfile->Delete();
  }

  if (fpMcTripletsOutFile) {
    fpMcTripletsOutFile->cd();
    fpMcTripletsTree->Write();
    fpMcTripletsOutFile->Close();
    fpMcTripletsOutFile->Delete();
  }

  gFile      = currentFile;
  gDirectory = curr;
  fpAlgo->Finish();
}


void CbmL1::writedir2current(TObject* obj)
{
  if (!obj->IsFolder()) obj->Write();
  else {
    TDirectory* cur = gDirectory;
    TDirectory* sub = cur->mkdir(obj->GetName());
    sub->cd();
    TList* listSub = (L1_DYNAMIC_CAST<TDirectory*>(obj))->GetList();
    TIter it(listSub);
    while (TObject* obj1 = it())
      writedir2current(obj1);
    cur->cd();
  }
}

/// -----   Ideal Tracking   -----------------------------

void CbmL1::IdealTrackFinder()
{
  fpAlgo->fRecoTracks.clear();
  fpAlgo->fRecoHits.clear();

  for (L1Vector<CbmL1MCTrack>::iterator i = fvMCTracks.begin(); i != fvMCTracks.end(); ++i) {
    CbmL1MCTrack& MC = *i;

    if (!MC.IsReconstructable()) continue;
    if (!(MC.ID >= 0)) continue;

    if (MC.Hits.size() < 4) continue;

    L1Track algoTr;
    algoTr.NHits = 0;

    L1Vector<int> hitIndices("CbmL1::hitIndices", fpAlgo->GetParameters()->GetNstationsActive(), -1);

    for (unsigned int iH = 0; iH < MC.Hits.size(); iH++) {
      const int hitI               = MC.Hits[iH];
      const CbmL1HitDebugInfo& hit = fvHitDebugInfo[hitI];

      const int iStation = fvMCPoints[hit.mcPointIds[0]].iStation;

      if (iStation >= 0) hitIndices[iStation] = hitI;
    }


    for (int iH = 0; iH < fpAlgo->GetParameters()->GetNstationsActive(); iH++) {
      const int hitI = hitIndices[iH];
      if (hitI < 0) continue;

      // fpAlgo->fRecoHits.push_back(hitI);
      algoTr.NHits++;
    }


    if (algoTr.NHits < 3) continue;

    for (int iH = 0; iH < fpAlgo->GetParameters()->GetNstationsActive(); iH++) {
      const int hitI = hitIndices[iH];
      if (hitI < 0) continue;
      fpAlgo->fRecoHits.push_back(hitI);
    }

    algoTr.TFirst[0] = MC.x;
    algoTr.TFirst[1] = MC.y;
    algoTr.TFirst[2] = MC.px / MC.pz;
    algoTr.TFirst[3] = MC.py / MC.pz;
    algoTr.TFirst[4] = MC.q / MC.p;
    algoTr.TFirst[5] = MC.z;

    fpAlgo->fRecoTracks.push_back(algoTr);
  }
}  // void CbmL1::IdealTrackFinder()


/// -----   STandAlone Package service-functions  -----------------------------

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::DefineSTAPNames(const char* dirName)
{
  // FIXME: SZh 01.03.2023: Clean STAP names
  namespace bfs = boost::filesystem;

  if (fSTAPDataMode == 0) { return; }

  // Define file prefix (/path/to/data/setup.reco.root -> "setup.reco")
  bfs::path pathToRecoOutput = FairRunAna::Instance()->GetUserOutputFileName().Data();
  fSTAPDataPrefix            = pathToRecoOutput.filename().string();
  fSTAPDataPrefix.ReplaceAll(".root", "");
  fSTAPDataPrefix = fSTAPDataPrefix.Strip(TString::EStripType::kBoth, '.');

  TString sDirName = TString(dirName);
  if (sDirName.Length() == 0) {
    fSTAPDataDir = pathToRecoOutput.parent_path().string();
    L1_SHOW(fSTAPDataDir);
  }
  else if (bfs::exists(sDirName.Data()) && bfs::is_directory(sDirName.Data())) {
    fSTAPDataDir = sDirName;
  }
  else {
    fSTAPDataDir = ".";
  }

  LOG(info) << "CbmL1: STAP data root directory is \033[1;32m" << bfs::system_complete(fSTAPDataDir.Data())
            << "\033[0m";

  if (fSTAPDataMode == 4) { return; }

  // Directory for handling L1InputData objects
  TString sInputDataDir = fSTAPDataDir + "/" + fSTAPDataPrefix + "_cainputdata";
  if (!bfs::exists(sInputDataDir.Data())) {
    LOG(warn) << "CbmL1: directory for tracking input data does not exist. It will be created";
    bfs::create_directories(sInputDataDir.Data());
  }
  LOG(info) << "CbmL1: STAP tracking input jobs directory is \033[1;32m" << bfs::system_complete(sInputDataDir.Data())
            << "\033[0m";
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::WriteSTAPParamObject()
{
  TString filename = fSTAPDataDir + "/" + fSTAPDataPrefix + "." + TString(kSTAPParamSuffix.data());
  fInitManager.WriteParametersObject(filename.Data());
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::WriteSTAPAlgoInputData(int iJob)  // must be called after ReadEvent
{
  TString filename =
    fSTAPDataDir + "/" + fSTAPDataPrefix + "_cainputdata/" + +TString::Format(kSTAPAlgoIDataSuffix.data(), iJob);

  // Write file
  fpIODataManager->WriteInputData(filename.Data());
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::WriteSTAPPerfInputData()  // must be called after ReadEvent
{
  LOG(fatal) << "CbmL1: Running in standalone mode is not available at the moment. It will be updated soon...";
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::ReadSTAPParamObject()
{
  TString filename = fSTAPDataDir + "/" + fSTAPDataPrefix + "." + TString(kSTAPParamSuffix.data());
  fInitManager.ReadParametersObject(filename.Data());
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::ReadSTAPAlgoInputData(int iJob)
{
  TString filename = fSTAPDataDir + "/" + TString(kSTAPAlgoIDataDir) + "/" + fSTAPDataPrefix + "."
                     + TString::Format(kSTAPAlgoIDataSuffix.data(), iJob);

  // Read file
  fpIODataManager->ReadInputData(filename.Data());
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::ReadSTAPPerfInputData()
{
  LOG(fatal) << "CbmL1: Running in standalone mode is not available at the moment. It will be updated soon...";
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::WriteSIMDKFData()
{
  // TODO: Must be totally reimplemented (S.Zharko)
  static bool first = 1;

  ///Write Tracks and MC Tracks

  static int TrNumber = 0;
  std::fstream Tracks, McTracksCentr, McTracksIn, McTracksOut;
  if (first) {
    Tracks.open("tracks.dat", std::fstream::out);
    McTracksCentr.open("mctrackscentr.dat", std::fstream::out);
    McTracksIn.open("mctracksin.dat", std::fstream::out);
    McTracksOut.open("mctracksout.dat", std::fstream::out);
    first = 0;
  }
  else {
    Tracks.open("tracks.dat", std::fstream::out | std::fstream::app);
    McTracksCentr.open("mctrackscentr.dat", std::fstream::out | std::fstream::app);
    McTracksIn.open("mctracksin.dat", std::fstream::out | std::fstream::app);
    McTracksOut.open("mctracksout.dat", std::fstream::out | std::fstream::app);
  }

  for (L1Vector<CbmL1Track>::iterator RecTrack = fvRecoTracks.begin(); RecTrack != fvRecoTracks.end(); ++RecTrack) {
    if (RecTrack->IsGhost()) continue;

    CbmL1MCTrack* MCTrack = RecTrack->GetMCTrack();
    if (!(MCTrack->IsPrimary())) continue;

    int NHits = (RecTrack->Hits).size();
    float x[20], y[20], z[20];
    int st[20];
    int jHit = 0;
    for (int iHit = 0; iHit < NHits; iHit++) {
      CbmL1HitDebugInfo& h = fvHitDebugInfo[RecTrack->Hits[iHit]];
      st[jHit]             = h.iStation;
      if (h.ExtIndex < 0) {
        CbmMvdHit* MvdH = (CbmMvdHit*) fpMvdHits->At(-h.ExtIndex - 1);
        x[jHit]         = MvdH->GetX();
        y[jHit]         = MvdH->GetY();
        z[jHit]         = MvdH->GetZ();
        jHit++;
      }
      else {
        CbmStsHit* StsH = (CbmStsHit*) fpStsHits->At(h.ExtIndex);
        x[jHit]         = StsH->GetX();
        y[jHit]         = StsH->GetY();
        z[jHit]         = StsH->GetZ();
        jHit++;
      }
    }

    Tracks << TrNumber << " " << MCTrack->x << " " << MCTrack->y << " " << MCTrack->z << " " << MCTrack->px << " "
           << MCTrack->py << " " << MCTrack->pz << " " << MCTrack->q << " " << NHits << endl;

    float AngleXAxis = 0, AngleYAxis = 0;
    for (int i = 0; i < NHits; i++)
      Tracks << "     " << st[i] << " " << x[i] << " " << y[i] << " " << z[i] << " " << AngleXAxis << " " << AngleYAxis
             << endl;
    Tracks << endl;

    int NMCPoints = (MCTrack->Points).size();

    McTracksIn << TrNumber << " " << MCTrack->x << " " << MCTrack->y << " " << MCTrack->z << " " << MCTrack->px << " "
               << MCTrack->py << " " << MCTrack->pz << " " << MCTrack->q << " " << NMCPoints << endl;
    McTracksOut << TrNumber << " " << MCTrack->x << " " << MCTrack->y << " " << MCTrack->z << " " << MCTrack->px << " "
                << MCTrack->py << " " << MCTrack->pz << " " << MCTrack->q << " " << NMCPoints << endl;
    McTracksCentr << TrNumber << " " << MCTrack->x << " " << MCTrack->y << " " << MCTrack->z << " " << MCTrack->px
                  << " " << MCTrack->py << " " << MCTrack->pz << " " << MCTrack->q << " " << NMCPoints << endl;

    for (int iPoint = 0; iPoint < NMCPoints; iPoint++) {
      CbmL1MCPoint& MCPoint = fvMCPoints[MCTrack->Points[iPoint]];
      McTracksIn << "     " << MCPoint.iStation << " " << MCPoint.xIn << " " << MCPoint.yIn << " " << MCPoint.zIn << " "
                 << MCPoint.pxIn << " " << MCPoint.pyIn << " " << MCPoint.pzIn << endl;
      McTracksOut << "     " << MCPoint.iStation << " " << MCPoint.xOut << " " << MCPoint.yOut << " " << MCPoint.zOut
                  << " " << MCPoint.pxOut << " " << MCPoint.pyOut << " " << MCPoint.pzOut << endl;
      McTracksCentr << "     " << MCPoint.iStation << " " << MCPoint.x << " " << MCPoint.y << " " << MCPoint.z << " "
                    << MCPoint.px << " " << MCPoint.py << " " << MCPoint.pz << endl;
    }
    McTracksIn << endl;
    McTracksOut << endl;
    McTracksCentr << endl;

    TrNumber++;
  }
}

// ---------------------------------------------------------------------------------------------------------------------
// NOTE: this function should be called before fInitManager.SendParameters(fpAlgo)
std::vector<L1Material> CbmL1::ReadMaterialBudget(L1DetectorID detectorID)
{
  std::vector<L1Material> result {};
  if (fMatBudgetFileName.find(detectorID) != fMatBudgetFileName.end()) {
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    auto rlFile = TFile(fMatBudgetFileName.at(detectorID));
    if (rlFile.IsZombie()) { LOG(fatal) << "File " << fMatBudgetFileName.at(detectorID) << " is zombie!"; }
    else {
      LOG(info) << "Reading material budget for " << GetDetectorName(detectorID) << " from file "
                << fMatBudgetFileName.at(detectorID);
    }

    result.resize(fInitManager.GetNstationsGeometry(detectorID));
    TString stationNamePrefix = "Radiation Thickness [%], Station";

    // NOTE: Loop over geometry stations. We probably do not know which stations are active/inactive (S.Zharko)
    for (int iSt = 0; iSt < fInitManager.GetNstationsGeometry(detectorID); ++iSt) {
      // TODO: Unify material table names (S.Zharko)
      TString stationName = stationNamePrefix + (detectorID == L1DetectorID::kMvd ? iSt : iSt + 1);
      auto* hStaRadLen    = rlFile.Get<TProfile2D>(stationName);
      LOG_IF(fatal, !hStaRadLen) << "CbmL1: material budget profile " << stationName << " does not exist in file "
                                 << fMatBudgetFileName.at(detectorID);
      int nBins  = hStaRadLen->GetNbinsX();
      float rMax = hStaRadLen->GetXaxis()->GetXmax();
      result[iSt].SetBins(nBins, rMax);
      for (int iBinX = 0; iBinX < nBins; ++iBinX) {
        for (int iBinY = 0; iBinY < nBins; ++iBinY) {
          result[iSt].SetRadThickBin(iBinX, iBinY, 0.01 * hStaRadLen->GetBinContent(iBinX, iBinY));
        }  // iBinX
      }    // iBinY
      LOG(info) << "- station " << iSt;
    }  // iSt
    gFile      = oldFile;
    gDirectory = oldDir;
  }  // if mat budget file found
  else {
    LOG(fatal) << "No material budget file is found for " << GetDetectorName(detectorID);
  }
  return result;
}

// ---------------------------------------------------------------------------------------------------------------------
//
double CbmL1::boundedGaus(double sigma)
{
  assert(sigma > 0. && std::isfinite(sigma));
  double x = 0.;
  do {
    x = gRandom->Gaus(0., sigma);
  } while (fabs(x) > 3.5 * sigma);
  return x;
}
