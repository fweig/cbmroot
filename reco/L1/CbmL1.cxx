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
#include "CbmMCDataObject.h"
#include "CbmStsFindTracks.h"
#include "CbmStsHit.h"
#include "CbmTrackingDetectorInterfaceInit.h"

#include "FairEventHeader.h"
#include "FairRunAna.h"

#include "TGeoArb8.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TMatrixD.h"
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

#include "KFTopoPerformance.h"

ClassImp(CbmL1);

static L1Algo gAlgo _fvecalignment;  // TODO: Change coupling logic between L1Algo and CbmL1

CbmL1* CbmL1::fpInstance = 0;


// ---------------------------------------------------------------------------------------------------------------------
//
CbmL1::CbmL1() : CbmL1("L1") {}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmL1::CbmL1(const char* name, Int_t verbose, Int_t performance, int dataMode, const TString& dataDir,
             int findParticleMode)
  : FairTask(name, verbose)
  , fIODataManager(L1IODataManager(gAlgo.GetParameters()))
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

  if (1 == fSTAPDataMode || 2 == fSTAPDataMode) { this->DefineSTAPNames(dataDir); }

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


  // turn on reconstruction in sub-detectors

  fUseMVD  = true;
  fUseSTS  = true;
  fUseMUCH = false;
  fUseTRD  = false;
  fUseTOF  = false;

  FairRootManager* fairManager = FairRootManager::Instance();
  {
    CbmStsFindTracks* findTask = L1_DYNAMIC_CAST<CbmStsFindTracks*>(FairRunAna::Instance()->GetTask("STSFindTracks"));
    if (findTask) fUseMVD = findTask->MvdUsage();
    if (CbmKF::Instance()->vMvdMaterial.size() == 0) { fUseMVD = false; }
  }


  if (L1Algo::TrackingMode::kMcbm == fTrackingMode) {
    fUseMUCH = 1;
    fUseTRD  = 1;
    fUseTOF  = 1;
    fInitManager.DevSetIgnoreHitSearchAreas(true);
  }

  if (L1Algo::TrackingMode::kGlobal == fTrackingMode) {
    //at the moment trd2d tracking only
    fUseMVD  = 0;
    fUseSTS  = 0;
    fUseMUCH = 0;
    fUseTRD  = 1;
    fUseTOF  = 0;

    fInitManager.DevSetUseOfOriginalField();
    //fInitManager.DevSetIgnoreHitSearchAreas(true);
    //fInitManager.DevSetIsMatchDoubletsViaMc(true);
    //fInitManager.DevSetIsMatchTripletsViaMc(true);
    fInitManager.SetMaxTripletPerDoublets(1000);
  }

  CheckDetectorPresence();

  fpStsPoints  = 0;
  fpMvdPoints  = 0;
  fpMuchPoints = 0;
  fpTrdPoints  = 0;
  fpTofPoints  = 0;
  fpMCTracks   = 0;

  fpMvdHitMatches  = 0;
  fpTrdHitMatches  = 0;
  fpMuchHitMatches = 0;
  fpTofHitMatches  = 0;

  fpStsClusters = 0;

  fvFileEvent.clear();

  if (!fLegacyEventMode) {  //  Time-slice mode selected
    LOG(info) << GetName() << ": running in time-slice mode.";
    fTimeSlice = NULL;
    fTimeSlice = (CbmTimeSlice*) fairManager->GetObject("TimeSlice.");
    if (fTimeSlice == NULL) LOG(fatal) << GetName() << ": No time slice branch in the tree!";

  }  //? time-slice mode

  else  // event mode
    LOG(info) << GetName() << ": running in event mode.";


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

    fpMCTracks = mcManager->InitBranch("MCTrack");

    if (nullptr == fpMCTracks) LOG(fatal) << GetName() << ": No MCTrack data!";
    if (nullptr == fpMcEventHeader) LOG(fatal) << GetName() << ": No MC event header data!";

    if (!fLegacyEventMode) {
      fpEventList = (CbmMCEventList*) fairManager->GetObject("MCEventList.");
      if (nullptr == fpEventList) LOG(fatal) << GetName() << ": No MCEventList data!";
    }

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

    if (!fUseTRD) {
      fpTrdPoints     = 0;
      fpTrdHitMatches = 0;
    }
    else {
      fpTrdHitMatches = (TClonesArray*) fairManager->GetObject("TrdHitMatch");
      fpTrdPoints     = mcManager->InitBranch("TrdPoint");
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

    if (!fUseTOF) {
      fpTofPoints     = 0;
      fpTofHitMatches = 0;
    }
    else {
      fpTofPoints     = mcManager->InitBranch("TofPoint");
      fpTofHitMatches = static_cast<TClonesArray*>(fairManager->GetObject("TofHitMatch"));
    }
  }
  else {
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

    if (CbmKF::Instance()->GetMagneticField()) {
      fInitManager.SetFieldFunction([](const double(&inPos)[3], double(&outB)[3]) {
        CbmKF::Instance()->GetMagneticField()->GetFieldValue(inPos, outB);
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
      auto correctionMvd    = [this](L1Material& material, const L1MaterialInfo& homogenious) {
        this->ApplyCorrectionToMaterialMap<L1DetectorID::kMvd>(material, homogenious);
      };
      for (int iSt = 0; iSt < fNMvdStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kMvd, iSt);
        stationInfo.SetStationType(1);  // MVD
        stationInfo.SetTimeInfo(mvdInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetTimeResolution(mvdInterface->GetTimeResolution(iSt));
        stationInfo.SetFieldStatus(fTrackingMode == L1Algo::TrackingMode::kMcbm ? 0 : 1);
        stationInfo.SetZ(mvdInterface->GetZ(iSt));
        stationInfo.SetXmax(mvdInterface->GetXmax(iSt));
        stationInfo.SetYmax(mvdInterface->GetYmax(iSt));
        stationInfo.SetRmin(mvdInterface->GetRmin(iSt));
        stationInfo.SetRmax(mvdInterface->GetRmax(iSt));
        stationInfo.SetMaterialSimple(mvdInterface->GetThickness(iSt), mvdInterface->GetRadLength(iSt));
        stationInfo.SetMaterialMap(std::move(materialTableMvd[iSt]), correctionMvd);
        // TODO: The CA TF result is dependent from type of geometry settings. Should be understood (S.Zharko)
        stationInfo.SetFrontBackStripsGeometry(
          (fscal) mvdInterface->GetStripsStereoAngleFront(iSt), (fscal) mvdInterface->GetStripsSpatialRmsFront(iSt),
          (fscal) mvdInterface->GetStripsStereoAngleBack(iSt), (fscal) mvdInterface->GetStripsSpatialRmsBack(iSt));
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
        fInitManager.AddStation(stationInfo);
        LOG(info) << "- MVD station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
      }
    }

    // *** STS stations info ***
    if (fUseSTS) {
      auto materialTableSts = ReadMaterialBudget(L1DetectorID::kSts);
      auto correctionSts    = [this](L1Material& material, const L1MaterialInfo& homogenious) {
        this->ApplyCorrectionToMaterialMap<L1DetectorID::kSts>(material, homogenious);
      };
      for (int iSt = 0; iSt < fNStsStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kSts, iSt);
        stationInfo.SetStationType(0);  // STS
        stationInfo.SetTimeInfo(stsInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetTimeInfo(L1Algo::TrackingMode::kMcbm != fTrackingMode ? stsInterface->IsTimeInfoProvided(iSt)
                                                                             : false);
        stationInfo.SetTimeResolution(stsInterface->GetTimeResolution(iSt));
        stationInfo.SetFieldStatus(L1Algo::TrackingMode::kMcbm == fTrackingMode ? 0 : 1);
        stationInfo.SetZ(stsInterface->GetZ(iSt));
        stationInfo.SetXmax(stsInterface->GetXmax(iSt));
        stationInfo.SetYmax(stsInterface->GetYmax(iSt));
        stationInfo.SetRmin(stsInterface->GetRmin(iSt));
        stationInfo.SetRmax(stsInterface->GetRmax(iSt));
        stationInfo.SetMaterialSimple(stsInterface->GetThickness(iSt), stsInterface->GetRadLength(iSt));
        stationInfo.SetMaterialMap(std::move(materialTableSts[iSt]), correctionSts);
        // TODO: The CA TF result is dependent from type of geometry settings. Should be understood (S.Zharko)
        stationInfo.SetFrontBackStripsGeometry(
          (fscal) stsInterface->GetStripsStereoAngleFront(iSt), (fscal) stsInterface->GetStripsSpatialRmsFront(iSt),
          (fscal) stsInterface->GetStripsStereoAngleBack(iSt), (fscal) stsInterface->GetStripsSpatialRmsBack(iSt));
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
        fInitManager.AddStation(stationInfo);
        LOG(info) << "- STS station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
      }
    }

    // *** MuCh stations info ***
    if (fUseMUCH) {
      auto materialTableMuch = ReadMaterialBudget(L1DetectorID::kMuch);
      auto correctionMuch    = [this](L1Material& material, const L1MaterialInfo& homogenious) {
        this->ApplyCorrectionToMaterialMap<L1DetectorID::kMuch>(material, homogenious);
      };
      for (int iSt = 0; iSt < fNMuchStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kMuch, iSt);
        stationInfo.SetStationType(2);  // MuCh
        stationInfo.SetTimeInfo(muchInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetTimeInfo(L1Algo::TrackingMode::kMcbm != fTrackingMode ? stsInterface->IsTimeInfoProvided(iSt)
                                                                             : false);
        stationInfo.SetTimeResolution(muchInterface->GetTimeResolution(iSt));
        stationInfo.SetFieldStatus(0);
        stationInfo.SetZ(muchInterface->GetZ(iSt));
        stationInfo.SetXmax(muchInterface->GetXmax(iSt));
        stationInfo.SetYmax(muchInterface->GetYmax(iSt));
        stationInfo.SetRmin(muchInterface->GetRmin(iSt));
        stationInfo.SetRmax(muchInterface->GetRmax(iSt));
        stationInfo.SetMaterialSimple(muchInterface->GetThickness(iSt), muchInterface->GetRadLength(iSt));
        stationInfo.SetMaterialMap(std::move(materialTableMuch[iSt]), correctionMuch);
        // TODO: The CA TF result is dependent from type of geometry settings. Should be understood (S.Zharko)
        stationInfo.SetFrontBackStripsGeometry(
          (fscal) muchInterface->GetStripsStereoAngleFront(iSt), (fscal) muchInterface->GetStripsSpatialRmsFront(iSt),
          (fscal) muchInterface->GetStripsStereoAngleBack(iSt), (fscal) muchInterface->GetStripsSpatialRmsBack(iSt));
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
        fInitManager.AddStation(stationInfo);
        LOG(info) << "- MuCh station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
      }
    }

    // *** TRD stations info ***
    if (fUseTRD) {
      auto materialTableTrd = ReadMaterialBudget(L1DetectorID::kTrd);
      auto correctionTrd    = [this](L1Material& material, const L1MaterialInfo& homogenious) {
        this->ApplyCorrectionToMaterialMap<L1DetectorID::kTrd>(material, homogenious);
      };
      for (int iSt = 0; iSt < fNTrdStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kTrd, iSt);
        stationInfo.SetStationType((iSt == 1 || iSt == 3) ? 6 : 3);  // MuCh
        stationInfo.SetTimeInfo(trdInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetTimeInfo(L1Algo::TrackingMode::kMcbm != fTrackingMode ? stsInterface->IsTimeInfoProvided(iSt)
                                                                             : false);
        stationInfo.SetTimeResolution(trdInterface->GetTimeResolution(iSt));
        stationInfo.SetFieldStatus(0);
        stationInfo.SetZ(trdInterface->GetZ(iSt));
        stationInfo.SetXmax(trdInterface->GetXmax(iSt));
        stationInfo.SetYmax(trdInterface->GetYmax(iSt));
        stationInfo.SetRmin(trdInterface->GetRmin(iSt));
        stationInfo.SetRmax(trdInterface->GetRmax(iSt));
        stationInfo.SetMaterialSimple(trdInterface->GetThickness(iSt), trdInterface->GetRadLength(iSt));
        stationInfo.SetMaterialMap(std::move(materialTableTrd[iSt]), correctionTrd);
        fscal trdFrontPhi   = trdInterface->GetStripsStereoAngleFront(iSt);
        fscal trdBackPhi    = trdInterface->GetStripsStereoAngleBack(iSt);
        fscal trdFrontSigma = trdInterface->GetStripsSpatialRmsFront(iSt);
        fscal trdBackSigma  = trdInterface->GetStripsSpatialRmsBack(iSt);
        if (L1Algo::TrackingMode::kGlobal == fTrackingMode) {
          trdFrontSigma = 1.1;
          trdBackSigma  = 1.1;
          // stationInfo.SetTimeResolution(1.e10);
          stationInfo.SetTimeInfo(false);
        }
        stationInfo.SetFrontBackStripsGeometry(trdFrontPhi, trdFrontSigma, trdBackPhi, trdBackSigma);
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
        if (iSt == 1 && L1Algo::TrackingMode::kMcbm == fTrackingMode && fMissingHits) {
          stationInfo.SetTrackingStatus(false);
        }
        fInitManager.AddStation(stationInfo);
        LOG(info) << "- TRD station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
      }
    }

    // *** TOF stations info ***
    if (fUseTOF) {
      auto materialTableTof = ReadMaterialBudget(L1DetectorID::kTof);
      auto correctionTof    = [this](L1Material& material, const L1MaterialInfo& homogenious) {
        this->ApplyCorrectionToMaterialMap<L1DetectorID::kTof>(material, homogenious);
      };
      for (int iSt = 0; iSt < fNTofStationsGeom; ++iSt) {
        auto stationInfo = L1BaseStationInfo(L1DetectorID::kTof, iSt);
        stationInfo.SetStationType(4);
        stationInfo.SetTimeInfo(tofInterface->IsTimeInfoProvided(iSt));
        stationInfo.SetTimeInfo(L1Algo::TrackingMode::kMcbm != fTrackingMode ? stsInterface->IsTimeInfoProvided(iSt)
                                                                             : false);
        stationInfo.SetTimeResolution(tofInterface->GetTimeResolution(iSt));
        stationInfo.SetFieldStatus(0);
        stationInfo.SetZ(tofInterface->GetZ(iSt));
        auto thickness = tofInterface->GetThickness(iSt);
        auto radLength = tofInterface->GetRadLength(iSt);
        stationInfo.SetMaterialSimple(thickness, radLength);
        stationInfo.SetMaterialMap(std::move(materialTableTof[iSt]), correctionTof);
        stationInfo.SetXmax(tofInterface->GetXmax(iSt));
        stationInfo.SetYmax(tofInterface->GetYmax(iSt));
        stationInfo.SetRmin(tofInterface->GetRmin(iSt));
        stationInfo.SetRmax(tofInterface->GetRmax(iSt));
        fscal tofFrontPhi   = tofInterface->GetStripsStereoAngleFront(iSt);
        fscal tofBackPhi    = tofInterface->GetStripsStereoAngleBack(iSt);
        fscal tofFrontSigma = tofInterface->GetStripsSpatialRmsFront(iSt);
        fscal tofBackSigma  = tofInterface->GetStripsSpatialRmsBack(iSt);
        stationInfo.SetFrontBackStripsGeometry(tofFrontPhi, tofFrontSigma, tofBackPhi, tofBackSigma);
        stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
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
    auto trackingIterFastPrim = L1CAIteration("FastPrimIter");
    trackingIterFastPrim.SetMinNhits(4);
    trackingIterFastPrim.SetMinNhitsStation0(3);
    trackingIterFastPrim.SetTrackChi2Cut(10.f);
    trackingIterFastPrim.SetTripletChi2Cut(23.4450f);  // = 7.815 * 3;  // prob = 0.05
    trackingIterFastPrim.SetDoubletChi2Cut(7.56327f);  // = 1.3449 * 2.f / 3.f;  // prob = 0.1
    trackingIterFastPrim.SetPickGather(3.0f);
    trackingIterFastPrim.SetTripletLinkChi2(25.);
    trackingIterFastPrim.SetMaxInvMom(1.0 / 0.5);
    trackingIterFastPrim.SetMaxSlopePV(1.1f);
    trackingIterFastPrim.SetMaxSlope(2.748f);
    trackingIterFastPrim.SetMaxDZ(0);
    trackingIterFastPrim.SetTargetPosSigmaXY(1, 1);
    trackingIterFastPrim.SetPrimaryFlag(true);
    trackingIterFastPrim.SetExtendTracksFlag(true);

    auto trackingIterAllPrim = L1CAIteration("AllPrimIter");
    trackingIterAllPrim.SetMinNhits(4);
    trackingIterAllPrim.SetMinNhitsStation0(3);
    trackingIterAllPrim.SetTrackChi2Cut(10.f);
    trackingIterAllPrim.SetTripletChi2Cut(23.4450f);
    trackingIterAllPrim.SetDoubletChi2Cut(7.56327f);
    trackingIterAllPrim.SetPickGather(4.0f);
    trackingIterAllPrim.SetTripletLinkChi2(25.);
    trackingIterAllPrim.SetMaxInvMom(1.0 / 0.05);
    trackingIterAllPrim.SetMaxSlopePV(1.1f);
    trackingIterAllPrim.SetMaxSlope(2.748f);
    trackingIterAllPrim.SetMaxDZ(0.1);
    trackingIterAllPrim.SetTargetPosSigmaXY(1, 1);
    trackingIterAllPrim.SetPrimaryFlag(true);
    trackingIterAllPrim.SetExtendTracksFlag(true);

    auto trackingIterFastPrim2 = L1CAIteration("FastPrim2Iter");
    trackingIterFastPrim2.SetMinNhits(4);
    trackingIterFastPrim2.SetMinNhitsStation0(4);
    trackingIterFastPrim2.SetTrackChi2Cut(10.f);
    trackingIterFastPrim2.SetTripletChi2Cut(21.1075f);
    trackingIterFastPrim2.SetDoubletChi2Cut(7.56327f);
    trackingIterFastPrim2.SetPickGather(3.0f);
    trackingIterFastPrim2.SetTripletLinkChi2(25.);
    trackingIterFastPrim2.SetMaxInvMom(1.0 / 0.5);
    trackingIterFastPrim2.SetMaxSlopePV(1.1f);
    trackingIterFastPrim2.SetMaxSlope(2.748f);
    trackingIterFastPrim2.SetMaxDZ(0);
    trackingIterFastPrim2.SetTargetPosSigmaXY(5, 5);
    trackingIterFastPrim2.SetPrimaryFlag(true);
    trackingIterFastPrim2.SetExtendTracksFlag(true);

    auto trackingIterAllSec = L1CAIteration("AllSecIter");
    trackingIterAllSec.SetMinNhits(4);
    trackingIterAllSec.SetMinNhitsStation0(4);
    trackingIterAllSec.SetTrackChi2Cut(10.f);
    trackingIterAllSec.SetTripletChi2Cut(18.7560f);  // = 6.252 * 3;  // prob = 0.1
    trackingIterAllSec.SetDoubletChi2Cut(7.56327f);
    trackingIterAllSec.SetPickGather(4.0f);
    trackingIterAllSec.SetTripletLinkChi2(25.);
    trackingIterAllSec.SetMaxInvMom(1.0 / 0.1);
    trackingIterAllSec.SetMaxSlopePV(1.5f);
    trackingIterAllSec.SetMaxSlope(2.748f);
    trackingIterAllSec.SetMaxDZ(0.1);
    trackingIterAllSec.SetTargetPosSigmaXY(10, 10);
    trackingIterAllSec.SetPrimaryFlag(false);
    trackingIterAllSec.SetExtendTracksFlag(true);

    auto trackingIterFastPrimJump = L1CAIteration("FastPrimJumpIter");
    trackingIterFastPrimJump.SetMinNhits(4);
    trackingIterFastPrimJump.SetMinNhitsStation0(4);
    trackingIterFastPrimJump.SetTrackChi2Cut(10.f);
    trackingIterFastPrimJump.SetTripletChi2Cut(21.1075f);  // prob = 0.01
    trackingIterFastPrimJump.SetDoubletChi2Cut(7.56327f);
    trackingIterFastPrimJump.SetPickGather(3.0f);
    trackingIterFastPrimJump.SetTripletLinkChi2(25.);
    trackingIterFastPrimJump.SetMaxInvMom(1.0 / 0.5);
    trackingIterFastPrimJump.SetMaxSlopePV(1.1f);
    trackingIterFastPrimJump.SetMaxSlope(2.748f);
    trackingIterFastPrimJump.SetMaxDZ(0);
    trackingIterFastPrimJump.SetTargetPosSigmaXY(5, 5);
    trackingIterFastPrimJump.SetPrimaryFlag(true);
    trackingIterFastPrimJump.SetJumpedFlag(true);
    trackingIterFastPrimJump.SetExtendTracksFlag(true);

    auto trackingIterAllPrimJump = L1CAIteration("AllPrimJumpIter");
    trackingIterAllPrimJump.SetMinNhits(4);
    trackingIterAllPrimJump.SetMinNhitsStation0(4);
    trackingIterAllPrimJump.SetTrackChi2Cut(10.f);
    trackingIterAllPrimJump.SetTripletChi2Cut(18.7560f);
    trackingIterAllPrimJump.SetDoubletChi2Cut(7.56327f);
    trackingIterAllPrimJump.SetPickGather(4.0f);
    trackingIterAllPrimJump.SetTripletLinkChi2(25.);
    trackingIterAllPrimJump.SetMaxInvMom(1.0 / 0.1);
    trackingIterAllPrimJump.SetMaxSlopePV(1.1f);
    trackingIterAllPrimJump.SetMaxSlope(2.748f);
    trackingIterAllPrimJump.SetMaxDZ(0.1);
    trackingIterAllPrimJump.SetTargetPosSigmaXY(5, 5);
    trackingIterAllPrimJump.SetPrimaryFlag(true);
    trackingIterAllPrimJump.SetJumpedFlag(true);
    trackingIterAllPrimJump.SetExtendTracksFlag(true);

    auto trackingIterAllSecJump = L1CAIteration("AllSecJumpIter");
    trackingIterAllSecJump.SetMinNhits(4);
    trackingIterAllSecJump.SetMinNhitsStation0(4);
    trackingIterAllSecJump.SetTrackChi2Cut(10.f);
    trackingIterAllSecJump.SetTripletChi2Cut(21.1075f);
    trackingIterAllSecJump.SetDoubletChi2Cut(7.56327f);
    trackingIterAllSecJump.SetPickGather(4.0f);
    trackingIterAllSecJump.SetTripletLinkChi2(25.);
    trackingIterAllSecJump.SetMaxInvMom(1.0 / 0.1);
    trackingIterAllSecJump.SetMaxSlopePV(1.5f);
    trackingIterAllSecJump.SetMaxSlope(2.748f);
    trackingIterAllSecJump.SetMaxDZ(0.1);
    trackingIterAllSecJump.SetTargetPosSigmaXY(10, 10);
    trackingIterAllSecJump.SetPrimaryFlag(false);
    trackingIterAllSecJump.SetJumpedFlag(true);
    trackingIterAllSecJump.SetExtendTracksFlag(true);

    auto trackingIterAllPrimE = L1CAIteration("AllPrimEIter");
    trackingIterAllPrimE.SetMinNhits(4);
    trackingIterAllPrimE.SetMinNhitsStation0(3);
    trackingIterAllPrimE.SetTrackChi2Cut(10.f);
    trackingIterAllPrimE.SetTripletChi2Cut(23.4450f);
    trackingIterAllPrimE.SetDoubletChi2Cut(7.56327f);
    trackingIterAllPrimE.SetPickGather(4.0f);
    trackingIterAllPrimE.SetTripletLinkChi2(25.);
    trackingIterAllPrimE.SetMaxInvMom(1.0 / 0.05);
    trackingIterAllPrimE.SetMaxSlopePV(1.1f);
    trackingIterAllPrimE.SetMaxSlope(2.748f);
    trackingIterAllPrimE.SetMaxDZ(0.1);
    trackingIterAllPrimE.SetTargetPosSigmaXY(1, 1);
    trackingIterAllPrimE.SetPrimaryFlag(true);
    trackingIterAllPrimE.SetElectronFlag(true);
    trackingIterAllPrimE.SetExtendTracksFlag(true);

    auto trackingIterAllSecE = L1CAIteration("AllSecEIter");
    trackingIterAllSecE.SetMinNhits(4);
    trackingIterAllSecE.SetMinNhitsStation0(4);
    trackingIterAllSecE.SetTrackChi2Cut(10.f);
    trackingIterAllSecE.SetTripletChi2Cut(18.7560f);
    trackingIterAllSecE.SetDoubletChi2Cut(7.56327f);
    trackingIterAllSecE.SetPickGather(4.0f);
    trackingIterAllSecE.SetTripletLinkChi2(25.);
    trackingIterAllSecE.SetMaxInvMom(1.0 / 0.05);
    trackingIterAllSecE.SetMaxSlopePV(1.5f);
    trackingIterAllSecE.SetMaxSlope(2.748f);
    trackingIterAllSecE.SetMaxDZ(0.1);
    trackingIterAllSecE.SetTargetPosSigmaXY(10, 10);
    trackingIterAllSecE.SetPrimaryFlag(false);
    trackingIterAllSecE.SetElectronFlag(true);
    trackingIterAllSecE.SetExtendTracksFlag(true);

    // Select default track finder
    if (L1Algo::TrackingMode::kMcbm == fTrackingMode) {

      trackingIterFastPrim.SetMinNhits(4);
      trackingIterFastPrim.SetMinNhitsStation0(4);

      trackingIterAllSecE.SetMinNhits(4);
      trackingIterAllSecE.SetMinNhitsStation0(4);
      trackingIterAllPrim.SetMaxInvMom(1. / 0.1);
      trackingIterAllPrimE.SetMaxInvMom(1. / 0.1);
      trackingIterAllSecE.SetMaxInvMom(1. / 0.1);

      trackingIterAllSecJump.SetMinNhits(4);
      trackingIterAllSecJump.SetMinNhitsStation0(4);
      trackingIterFastPrim.SetMaxInvMom(1.0 / 0.3);
      trackingIterAllSec.SetMaxInvMom(1.0 / 0.3);
      trackingIterFastPrimJump.SetMaxInvMom(1.0 / 0.3);
      trackingIterAllPrimJump.SetMaxInvMom(1.0 / 0.3);
      trackingIterAllSecJump.SetMaxInvMom(1.0 / 0.3);

      trackingIterAllSec.SetMinNhits(4);
      trackingIterAllSec.SetMinNhitsStation0(4);
      trackingIterFastPrim.SetExtendTracksFlag(false);
      trackingIterAllPrim.SetExtendTracksFlag(false);
      trackingIterAllPrimJump.SetExtendTracksFlag(false);
      trackingIterAllSec.SetExtendTracksFlag(false);

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
        it.SetMaxInvMom(1.0 / 0.1);  //(1.0 / 0.5);
        it.SetMaxSlopePV(.1f);
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
      trd2dIter2.SetMaxInvMom(1.0 / 0.05);  //(1.0 / 0.5);
      trd2dIter2.SetMaxSlopePV(.5f);
      trd2dIter2.SetMaxSlope(.5f);
      trd2dIter2.SetMaxDZ(0.05);
      trd2dIter2.SetTargetPosSigmaXY(8 * 10, 6 * 10);  //(1, 1);
      trd2dIter2.SetPrimaryFlag(false);
      trd2dIter2.SetExtendTracksFlag(true);
      //trd2dIter2.SetFirstStationIndex(12);

      // Initialize CA track finder iterations sequence

      fInitManager.SetCAIterationsNumberCrosscheck(1);
      //fInitManager.PushBackCAIteration(trackingIterFastPrim);
      //fInitManager.PushBackCAIteration(globalIterPrimFast);
      fInitManager.PushBackCAIteration(trd2dIter2);
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

    // **********************
    // ** Set special cuts **
    // **********************

    fInitManager.SetGhostSuppression(fGhostSuppression);
    fInitManager.SetTrackingLevel(fTrackingLevel);
    fInitManager.SetMomentumCutOff(fMomentumCutOff);

    // Form parameters container
    if (!fInitManager.FormParametersContainer()) { return kFATAL; }

    // Write to file if needed
    if (1 == fSTAPDataMode) { this->WriteSTAPParamObject(); }
  }


  // Init L1 algo core

  fpAlgo = &gAlgo;
  fpAlgo->Init(fUseHitErrors, fTrackingMode, fMissingHits);

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
  fvFileEvent.clear();

  // TODO: move these values to CbmL1Parameters namespace (S.Zharko)
  bool areDataLeft = true;   // whole TS processed?
  float TsStart    = 0;      // starting time of sub-TS
  float TsLength   = 10000;  // length of sub-TS
  float TsOverlap  = 15;     // min length of overlap region
  int FstHitinTs   = 0;      // 1st hit index in TS


  // TODO: Refactor this part, check usage ---------------------------------
  int nStsHits = (fUseSTS && fpStsHits ? fpStsHits->GetEntriesFast() : 0);

  /// sort input hits by time
  L1Vector<std::pair<double, int>> SortHits("CbmL1::SortHits");
  SortHits.reserve(nStsHits);
  float start_t = 10000000000;
  for (Int_t j = 0; j < nStsHits; j++) {
    CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(fpStsHits->At(j));
    double t      = sh->GetTime();
    if (t < start_t) start_t = t;
    SortHits.push_back(std::pair<double, int>(t, j));
  }
  TsStart = start_t;  ///reco TS start time is set to smallest hit time

  std::sort(SortHits.begin(), SortHits.end());
  fvSortedStsHitsIndexes.clear();
  fvSortedStsHitsIndexes.reserve(SortHits.size());
  for (unsigned int i = 0; i < SortHits.size(); i++) {
    int j = SortHits[i].second;
    fvSortedStsHitsIndexes.push_back(j);
  };
  // -----------------------------------------------------------------------

  if (!fLegacyEventMode && fPerformance) {

    int nofEvents = fpEventList->GetNofEvents();
    for (int iE = 0; iE < nofEvents; iE++) {
      int fileId  = fpEventList->GetFileIdByIndex(iE);
      int eventId = fpEventList->GetEventIdByIndex(iE);
      fvFileEvent.insert(DFSET::value_type(fileId, eventId));
    }
  }
  else {
    Int_t iFile  = FairRunAna::Instance()->GetEventHeader()->GetInputFileId();
    Int_t iEvent = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
    fvFileEvent.insert(DFSET::value_type(iFile, iEvent));
  }

  if (fVerbose > 1) { cout << "\nCbmL1::Exec event " << ++nevent << " ...\n\n"; }
#ifdef _OPENMP
  omp_set_num_threads(1);
#endif
  // repack data

  L1Vector<CbmL1Track> vRTracksCur("CbmL1::vRTracksCur");
  // FIXME: Update the following code block in order to account for MuCh, TRD and TOF (S.Zharko)
  {
    int nHits = 0;
    int nSta  = 1;
    if (fUseMVD && fpMvdHits) {
      nHits += fpMvdHits->GetEntriesFast();
      nSta += fNMvdStations;
    }
    if (fUseSTS && fpStsHits) {
      nHits += fpStsHits->GetEntriesFast();
      nSta += fNStsStations;
    }
    vRTracksCur.reserve(10 + (2 * nHits) / nSta);
  }

  fTrackingTime = 0;


  while (areDataLeft) {
    if (event) {
      areDataLeft = false;
      TsStart     = 0;
      TsLength    = 2000000000;  // TODO: Why this number was selected? (S.Zharko)
      TsOverlap   = 0;
      FstHitinTs  = 0;
    }


    // ----- Read data from branches and send data from IODataManager to L1Algo ----------------------------------------
    ReadEvent(TsStart, TsLength, TsOverlap, FstHitinTs, areDataLeft, event);


    if (fPerformance) {
      HitMatch();
      // calculate the max number of Hits\mcPoints on continuous(consecutive) stations
      for (auto it = fvMCTracks.begin(); it != fvMCTracks.end(); ++it) {
        it->Init();
      }
    }

    if ((fPerformance) && (fSTAPDataMode < 2)) { InputPerformance(); }

    //  FieldApproxCheck();
    //  FieldIntegralCheck();

    // TODO: Remove code below (S.Zharko)
    //     for (unsigned int iH = 0; iH < (*fpAlgo->vHits).size(); ++iH) {
    // #ifdef USE_EVENT_NUMBER
    //       L1Hit& h = const_cast<L1Hit&>((*fpAlgo->vHits)[iH]);
    //       h.n      = -1;
    // #endif
    //       if (fvExternalHits[iH].mcPointIds.size() == 0) continue;
    // #ifdef USE_EVENT_NUMBER
    //       const CbmL1MCPoint& mcp = fvMCPoints[fvExternalHits[iH].mcPointIds[0]];
    //       h.n                     = mcp.event;
    // #endif
    //     }

    if (fVerbose > 1) { cout << "L1 Track finder..." << endl; }
    fpAlgo->CATrackFinder();
    //      IdealTrackFinder();
    fTrackingTime += fpAlgo->fCATime;

    if (fVerbose > 1) { cout << "L1 Track finder ok" << endl; }
    //  fpAlgo->L1KFTrackFitter( fExtrapolateToTheEndOfSTS );

    {  // track fit
      L1FieldValue b = fpAlgo->GetParameters()->GetVertexFieldValue();

      if ((fabs(b.x[0]) < 0.0000001) && (fabs(b.y[0]) < 0.0000001) && (fabs(b.z[0]) < 0.0000001)) {
        // no field
        fpAlgo->KFTrackFitter_simple();
      }
      else {
        fpAlgo->L1KFTrackFitter();
      }
    }

    if (fVerbose > 1) { cout << "L1 Track fitter  ok" << endl; }

    // save reconstructed tracks
    if (fLegacyEventMode) vRTracksCur.clear();
    int start_hit = 0;

    float TsStart_new = TsStart + TsLength - TsOverlap;

    for (L1Vector<L1Track>::iterator it = fpAlgo->fTracks.begin(); it != fpAlgo->fTracks.end(); it++) {

      CbmL1Track t;
      for (int i = 0; i < 7; i++)
        t.T[i] = it->TFirst[i];
      for (int i = 0; i < 21; i++)
        t.C[i] = it->CFirst[i];
      for (int i = 0; i < 7; i++)
        t.TLast[i] = it->TLast[i];
      for (int i = 0; i < 21; i++)
        t.CLast[i] = it->CLast[i];
      for (int k = 0; k < 7; k++)
        t.Tpv[k] = it->Tpv[k];
      for (int k = 0; k < 21; k++)
        t.Cpv[k] = it->Cpv[k];
      t.chi2 = it->chi2;
      t.NDF  = it->NDF;
      //t.T[4] = it->Momentum;
      t.Hits.clear();
      t.fTrackTime = it->fTrackTime;

      L1Vector<int> HitsLocal("CbmL1::HitsLocal");
      HitsLocal.reserve(it->NHits);

      for (int i = 0; i < it->NHits; i++) {
        int start_hit1 = start_hit;
        if (fpAlgo->fRecoHits[start_hit1] > fvExternalHits.size() - 1) start_hit++;
        else if (!fLegacyEventMode) {
          t.Hits.push_back((fpAlgo->GetInputData()->GetHit(fpAlgo->fRecoHits[start_hit]).ID));
        }
        else {
          t.Hits.push_back(fpAlgo->fRecoHits[start_hit]);
        }
        HitsLocal.push_back(fpAlgo->fRecoHits[start_hit++]);
      }

      t.mass        = fpAlgo->fDefaultMass;  // pion mass
      t.is_electron = 0;

      t.SetId(vRTracksCur.size());
      //  CbmL1Track* prtra = &t;

      int indd         = 0;
      bool isInOverlap = 0;


      for (unsigned int i = 0; i < HitsLocal.size(); i++) {
        //      if ((*ih) > int(fvExternalHits.size() - 1)) {
        //         indd = 1;
        //         break;
        //       }

        if (fvExternalHits[HitsLocal[i]].t >= (TsStart + TsLength - TsOverlap)) {
          isInOverlap = 1;
          if (TsStart_new > fvExternalHits[HitsLocal[i]].t) TsStart_new = fvExternalHits[HitsLocal[i]].t;
        }

        int nMCPoints = fvExternalHits[HitsLocal[i]].mcPointIds.size();
        for (int iP = 0; iP < nMCPoints; iP++) {
          int iMC = fvExternalHits[HitsLocal[i]].mcPointIds[iP];
          if (iMC > int(fvMCPoints.size() - 1)) {
            //           cout << " iMC " << iMC << " fvMCPoints.size() " <<  fvMCPoints.size() << endl;
            indd = 1;
          }
        }
      }

      if (indd == 1) continue;

      if ((!fLegacyEventMode) && (isInOverlap == 1)) {

        continue;  ///Discard tracks from overlap region

        /// set strips as unused
        //for (unsigned int i = 0; i < HitsLocal.size(); i++) {
        //  fpAlgo->SetFUnUsed(const_cast<unsigned char&>((*fpAlgo->fStripFlag)[fvExternalHits[HitsLocal[i]].f]));
        //  fpAlgo->SetFUnUsed(const_cast<unsigned char&>((*fpAlgo->fStripFlag)[fvExternalHits[HitsLocal[i]].b]));
        //}
      }
      vRTracksCur.push_back(t);
    }

    for (int i = 0; i < nStsHits; ++i) {

      CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(fpStsHits->At(fvSortedStsHitsIndexes[i]));
      float time    = sh->GetTime();

      if (TsStart_new <= time) {
        FstHitinTs = i;  // TODO: shouldn't it be in the fLegacyEventMode == true only? (S.Zharko)
        break;
      }
    }

    if (!fLegacyEventMode) TsStart = TsStart_new;  ///Set new TS strat to earliest discarted track

    if (!fLegacyEventMode) cout << "CA Track Finder: " << fpAlgo->fCATime << " s/sub-ts" << endl << endl;
  }


  if (fPerformance) {

    float start = 0;
    float end   = 10000000000.f;
    int fHit    = 0;
    bool stop   = 0;

    ReadEvent(start, end, start, fHit, stop, event);
    HitMatch();
    // calculate the max number of Hits\mcPoints on continuous(consecutive) stations

    for (L1Vector<CbmL1MCTrack>::iterator it = fvMCTracks.begin(); it != fvMCTracks.end(); ++it)
      it->Init();
  }
  //
  //   if (fSTAPDataMode % 2 == 1) {  // 1,3
  //     WriteSTAPAlgoData();
  //     WriteSTAPPerfData();
  //   };
  //   if (fSTAPDataMode >= 2) {  // 2,3
  //     //ReadSTAPAlgoData();
  //
  //     ReadSTAPPerfData();
  //   };

  fvRecoTracks.clear();
  fvRecoTracks.reserve(vRTracksCur.size());
  for (unsigned int iTrack = 0; iTrack < vRTracksCur.size(); iTrack++) {

    for (unsigned int iHit = 0; iHit < vRTracksCur[iTrack].Hits.size(); iHit++)
      if (!fLegacyEventMode) { vRTracksCur[iTrack].Hits[iHit] = fvSortedHitsIndexes[vRTracksCur[iTrack].Hits[iHit]]; }

    fvRecoTracks.push_back(vRTracksCur[iTrack]);
  }


  if ((fPerformance) && (fSTAPDataMode < 2)) { InputPerformance(); }

  // TODO: Remove this code (S.Zharko)
  //  for (unsigned int iH = 0; iH < (*fpAlgo->vHits).size(); ++iH) {
  //#ifdef USE_EVENT_NUMBER
  //    L1Hit& h = const_cast<L1Hit&>((*fpAlgo->vHits)[iH]);
  //    h.n      = -1;
  //#endif
  //    if (fvExternalHits[iH].mcPointIds.size() == 0) continue;
  //#ifdef USE_EVENT_NUMBER
  //    const CbmL1MCPoint& mcp = fvMCPoints[fvExternalHits[iH].mcPointIds[0]];
  //    h.n                     = mcp.event;
  //#endif
  //  }

  // output performance
  if (fPerformance) {
    if (fVerbose > 1) { cout << "Performance..." << endl; }
    //HitMatch();
    TrackMatch();
  }


  if (fPerformance) {
    EfficienciesPerformance();
    HistoPerformance();
    TrackFitPerformance();
    // TimeHist();
    ///    WriteSIMDKFData();
  }
  if (fVerbose > 1) { cout << "End of L1" << endl; }

  static bool ask = 0;
  char symbol;
  if (ask) {
    std::cout << std::endl << "L1 run (any/r/q) > ";
    do {
      std::cin.get(symbol);
      if (symbol == 'r') ask = false;
      if ((symbol == 'e') || (symbol == 'q')) exit(0);
    } while (symbol != '\n');
  }

  // }
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
  std::string histoOutName = dir + "/L1_histo_" + p.filename().string();
  LOG(info) << "\033[31;1mHistograms will be saved to: \033[0m" << histoOutName;

  TFile* outfile = new TFile(histoOutName.c_str(), "RECREATE");
  outfile->cd();
  writedir2current(fHistoDir);
  outfile->Close();
  outfile->Delete();

  gFile      = currentFile;
  gDirectory = curr;
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
  fpAlgo->fTracks.clear();
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
      const int hitI      = MC.Hits[iH];
      const CbmL1Hit& hit = fvExternalHits[hitI];

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


    algoTr.Momentum  = MC.p;
    algoTr.TFirst[0] = MC.x;
    algoTr.TFirst[1] = MC.y;
    algoTr.TFirst[2] = MC.px / MC.pz;
    algoTr.TFirst[3] = MC.py / MC.pz;
    algoTr.TFirst[4] = MC.q / MC.p;
    algoTr.TFirst[5] = MC.z;

    fpAlgo->fTracks.push_back(algoTr);
  }
}  // void CbmL1::IdealTrackFinder()


/// -----   STandAlone Package service-functions  -----------------------------

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmL1::DefineSTAPNames(TString dirName)
{
  namespace bfs = boost::filesystem;

  // Check, if dirName path exists
  if (!bfs::exists(dirName.Data())) {
    LOG(warn) << "CbmL1: directory " << dirName.Data()
              << " (full path: " << bfs::system_complete(dirName.Data()).string()
              << ") for writing L1AlgoData object does not exist";
    fSTAPDataDir = ".";
  }

  // Check, if dirName path is a directory
  if (!bfs::is_directory(fSTAPDataDir.Data())) {
    LOG(warn) << "CbmL1: path " << fSTAPDataDir.Data()
              << " (full path: " << bfs::system_complete(fSTAPDataDir.Data()).string() << ") is not a directory";
    fSTAPDataDir = ".";
  }

  // TODO: Prepare directory, if it is not defined?
  //
  LOG(info) << "CbmL1: STAP data root directory is \033[1;32m" << bfs::system_complete(fSTAPDataDir.Data())
            << "\033[0m";

  // Output prefix definition
  bfs::path pathToOutput = FairRunAna::Instance()->GetUserOutputFileName().Data();
  fSTAPDataPrefix        = pathToOutput.filename().string();
  TString sOutputSuffix  = ".reco.root";
  {
    int suffixIdx = fSTAPDataPrefix.Index(sOutputSuffix);
    if (suffixIdx != -1) { fSTAPDataPrefix.Remove(suffixIdx, sOutputSuffix.Length()); }
  }
  // Leave prefix without points on the edges, if there are any
  fSTAPDataPrefix = fSTAPDataPrefix.Strip(TString::EStripType::kBoth, '.');

  // Directory for handling L1InputData objects
  TString sInputDataDir = fSTAPDataDir + "/" + TString(kSTAPAlgoIDataDir.data());
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
  TString filename = fSTAPDataDir + "/" + TString(kSTAPAlgoIDataDir.data()) + "/" + fSTAPDataPrefix + "."
                     + TString::Format(kSTAPAlgoIDataSuffix.data(), iJob);

  // Write file
  fIODataManager.WriteInputData(filename.Data());
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
  fIODataManager.ReadInputData(filename.Data());
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
#if 0
  /// Write geometry info
  if (first) {
    FairField* dMF = CbmKF::Instance()->GetMagneticField();

    std::fstream FileGeo;
    FileGeo.open("geo.dat", ios::out);

    std::fstream FieldCheck;
    FieldCheck.open("field.dat", ios::out);

    Double_t bfg[3], rfg[3];

    rfg[0] = 0.;
    rfg[1] = 0.;
    rfg[2] = 0.;
    dMF->GetFieldValue(rfg, bfg);
    FileGeo << rfg[2] << " " << bfg[0] << " " << bfg[1] << " " << bfg[2] << " " << endl;

    rfg[0] = 0.;
    rfg[1] = 0.;
    rfg[2] = 2.5;
    dMF->GetFieldValue(rfg, bfg);
    FileGeo << rfg[2] << " " << bfg[0] << " " << bfg[1] << " " << bfg[2] << " " << endl;

    rfg[0] = 0.;
    rfg[1] = 0.;
    rfg[2] = 5.0;
    dMF->GetFieldValue(rfg, bfg);
    FileGeo << rfg[2] << " " << bfg[0] << " " << bfg[1] << " " << bfg[2] << " " << endl << endl;
    FileGeo << fNStations << endl;

    const int M = 5;  // polinom order
    const int N = (M + 1) * (M + 2) / 2;

    for (Int_t ist = 0; ist < fNStations; ist++) {
      fscal f_phi, f_sigma, b_phi, b_sigma;

      double C[3][N];
      double z = 0;
      double Xmax, Ymax;
      if (ist < fNMvdStations) {
        CbmKFTube& t = CbmKF::Instance()->vMvdMaterial[ist];
        f_phi        = 0;
        f_sigma      = 5.e-4;
        b_phi        = 3.14159265358 / 2.;
        b_sigma      = 5.e-4;
        z            = t.z;
        Xmax = Ymax = t.R;
      }
      else {
        CbmStsStation* station = CbmStsSetup::Instance()->GetStation(ist - fNMvdStations);
        f_phi                  = station->GetSensorRotation();
        b_phi                  = f_phi;
        double Pi              = 3.14159265358;
        f_phi += station->GetSensorStereoAngle(0) * Pi / 180.;
        b_phi += station->GetSensorStereoAngle(1) * Pi / 180.;
        f_sigma = station->GetSensorPitch(0) / TMath::Sqrt(12);
        b_sigma = f_sigma;
        //f_sigma *= cos(f_phi);  // TODO: think about this
        //b_sigma *= cos(b_phi);
        z = station->GetZ();

        Xmax = station->GetXmax();
        Ymax = station->GetYmax();
      }

      double dx = 1.;  // step for the field approximation
      double dy = 1.;

      if (dx > Xmax / N / 2) dx = Xmax / N / 4.;
      if (dy > Ymax / N / 2) dy = Ymax / N / 4.;

      for (int i = 0; i < 3; i++)
        for (int k = 0; k < N; k++)
          C[i][k] = 0;
      TMatrixD A(N, N);
      TVectorD b0(N), b1(N), b2(N);
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
          A(i, j) = 0.;
        b0(i) = b1(i) = b2(i) = 0.;
      }
      for (double x = -Xmax; x <= Xmax; x += dx)
        for (double y = -Ymax; y <= Ymax; y += dy) {
          double r = sqrt(fabs(x * x / Xmax / Xmax + y / Ymax * y / Ymax));
          if (r > 1.) continue;
          Double_t w    = 1. / (r * r + 1);
          Double_t p[3] = {x, y, z};
          Double_t B[3] = {0., 0., 0.};
          CbmKF::Instance()->GetMagneticField()->GetFieldValue(p, B);
          TVectorD m(N);
          m(0) = 1;
          for (int i = 1; i <= M; i++) {
            int k = (i - 1) * (i) / 2;
            int l = i * (i + 1) / 2;
            for (int j = 0; j < i; j++)
              m(l + j) = x * m(k + j);
            m(l + i) = y * m(k + i - 1);
          }

          TVectorD mt = m;
          for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
              A(i, j) += w * m(i) * m(j);
            b0(i) += w * B[0] * m(i);
            b1(i) += w * B[1] * m(i);
            b2(i) += w * B[2] * m(i);
          }
        }
      double det;
      A.Invert(&det);
      TVectorD c0 = A * b0, c1 = A * b1, c2 = A * b2;
      for (int i = 0; i < N; i++) {
        C[0][i] = c0(i);
        C[1][i] = c1(i);
        C[2][i] = c2(i);
      }

      double c_f = cos(f_phi);
      double s_f = sin(f_phi);
      double c_b = cos(b_phi);
      double s_b = sin(b_phi);

      double det_m = c_f * s_b - s_f * c_b;
      det_m *= det_m;
      //      double C00 = ( s_b*s_b*f_sigma*f_sigma + s_f*s_f*b_sigma*b_sigma )/det_m;
      //      double C10 =-( s_b*c_b*f_sigma*f_sigma + s_f*c_f*b_sigma*b_sigma )/det_m;
      //      double C11 = ( c_b*c_b*f_sigma*f_sigma + c_f*c_f*b_sigma*b_sigma )/det_m;

      //      float delta_x = sqrt(C00);
      //      float delta_y = sqrt(C11);
      FileGeo << "    " << ist << " ";
      if (ist < fNMvdStations) {
        CbmKFTube& t = CbmKF::Instance()->vMvdMaterial[ist];
        FileGeo << t.z << " ";
        FileGeo << t.dz << " ";
        FileGeo << t.RadLength << " ";
      }
      else if (ist < (fNStsStations + fNMvdStations)) {
        CbmStsStation* station = CbmStsSetup::Instance()->GetStation(ist - fNMvdStations);
        FileGeo << station->GetZ() << " ";
        FileGeo << station->GetSensorD() << " ";
        FileGeo << station->GetRadLength() << " ";
      }
      FileGeo << f_sigma << " ";
      FileGeo << b_sigma << " ";
      FileGeo << f_phi << " ";
      FileGeo << b_phi << endl;
      FileGeo << "    " << N << endl;
      FileGeo << "       ";
      for (int ik = 0; ik < N; ik++)
        FileGeo << C[0][ik] << " ";
      FileGeo << endl;
      FileGeo << "       ";
      for (int ik = 0; ik < N; ik++)
        FileGeo << C[1][ik] << " ";
      FileGeo << endl;
      FileGeo << "       ";
      for (int ik = 0; ik < N; ik++)
        FileGeo << C[2][ik] << " ";
      FileGeo << endl;
    }
    FileGeo.close();
  }
#endif
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
      CbmL1HitStore& h = fvHitStore[RecTrack->Hits[iHit]];
      st[jHit]         = h.iStation;
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

double CbmL1::boundedGaus(double sigma)
{
  assert(sigma > 0. && std::isfinite(sigma));
  double x = 0.;
  do {
    x = gRandom->Gaus(0., sigma);
  } while (fabs(x) > 3.5 * sigma);
  return x;
}
