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
#include "CbmTrdTrackingInterface.h"

#include <boost/filesystem.hpp>
// TODO: include of CbmSetup.h creates problems on Mac
// #include "CbmSetup.h"
#include "CbmMCDataObject.h"
#include "CbmStsFindTracks.h"
#include "CbmStsHit.h"
#include "CbmTofCell.h"
#include "CbmTofDigiBdfPar.h"
#include "CbmTofDigiPar.h"  // in tof/TofParam
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
#include "L1AlgoInputData.h"
#include "L1Event.h"

using std::cout;
using std::endl;
using std::ios;

#include "KFTopoPerformance.h"

ClassImp(CbmL1)

  static L1Algo algo_static _fvecalignment;  // TODO: gAlgo

//L1AlgoInputData* fData_static _fvecalignment;

CbmL1* CbmL1::fInstance = 0;


CbmL1::CbmL1() : CbmL1("L1") {}


CbmL1::CbmL1(const char* name, Int_t iVerbose, Int_t _fPerformance, int fSTAPDataMode_, TString fSTAPDataDir_,
             int findParticleMode_)
  : FairTask(name, iVerbose)
  , fPerformance(_fPerformance)
  , fSTAPDataMode(fSTAPDataMode_)
  , fSTAPDataDir(fSTAPDataDir_)
  , fFindParticlesMode(findParticleMode_)
{
  if (!fInstance) fInstance = this;
  if (!fpInitManager) { fpInitManager = algo_static.GetInitManager(); }

  if (!CbmTrackingDetectorInterfaceInit::Instance()) {
    LOG(fatal) << "CbmL1: CbmTrackingDetectorInterfaceInit instance was not found. Please, add it as a task to your "
                  "reco macro before the KF and L1 task:\n"
               << "\033[1;30mrun->AddTask(new CbmTrackingDetectorInterfaceInit());\033[0m";
  }
}

CbmL1::~CbmL1()
{
  if (fInstance == this) fInstance = nullptr;
}

void CbmL1::CheckDetectorPresence()
{
  fUseMVD  = fUseMVD && CbmSetup::Instance()->IsActive(ECbmModuleId::kMvd);
  fUseSTS  = fUseSTS && CbmSetup::Instance()->IsActive(ECbmModuleId::kSts);
  fUseMUCH = fUseMUCH && CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch);
  fUseTRD  = fUseTRD && CbmSetup::Instance()->IsActive(ECbmModuleId::kTrd);
  fUseTOF  = fUseTOF && CbmSetup::Instance()->IsActive(ECbmModuleId::kTof);
}


void CbmL1::SetParContainers()
{
  LOG(warn) << "(!) \033[1;32mCALL CbmL1::SetParContainers\033[0m";
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  fDigiPar            = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
  //  fTrdDigiPar = (CbmTrdDigiPar*)(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdDigiPar"));

  fTofDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
  rtdb->initContainers(ana->GetRunId());  // needed to get tracking stations for ToF hits
}


InitStatus CbmL1::ReInit()
{
  SetParContainers();
  return Init();
}

InitStatus CbmL1::Init()
{

  fData = new L1AlgoInputData();

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

  // turn on reconstruction in sub-detectors

  fUseMVD  = 1;
  fUseSTS  = 1;
  fUseMUCH = 0;
  fUseTRD  = 0;
  fUseTOF  = 0;

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
    fpInitManager->DevSetIgnoreHitSearchAreas(true);
  }


  if (L1Algo::TrackingMode::kGlobal == fTrackingMode) {
    //SGtrd2D!!
    fUseMVD  = 0;
    fUseSTS  = 0;
    fUseMUCH = 0;
    fUseTRD  = 1;
    fUseTOF  = 0;
    fpInitManager->DevSetIgnoreHitSearchAreas(true);
    fpInitManager->DevSetFitSingletsFromTarget(true);
    //fpInitManager->DevSetIsMatchDoubletsViaMc(true);
  }

  CheckDetectorPresence();

  fStsPoints  = 0;
  fMvdPoints  = 0;
  fMuchPoints = 0;
  fTrdPoints  = 0;
  fTofPoints  = 0;
  fMCTracks   = 0;

  listMvdHitMatches  = 0;
  fTrdHitMatches     = 0;
  listMuchHitMatches = 0;
  fTofHitDigiMatches = 0;

  listStsClusters = 0;

  vFileEvent.clear();

  if (!fLegacyEventMode) {  //  Time-slice mode selected
    LOG(info) << GetName() << ": running in time-slice mode.";
    fTimeSlice = NULL;
    fTimeSlice = (CbmTimeSlice*) fairManager->GetObject("TimeSlice.");
    if (fTimeSlice == NULL) LOG(fatal) << GetName() << ": No time slice branch in the tree!";

  }  //? time-slice mode

  else  // event mode
    LOG(info) << GetName() << ": running in event mode.";


  listStsClusters     = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("StsCluster"));
  listStsHitMatch     = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("StsHitMatch"));
  listStsClusterMatch = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("StsClusterMatch"));

  listStsHits = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("StsHit"));

  if (!fUseMUCH) {
    fMuchPixelHits = 0;

    fDigisMuch       = 0;
    fDigiMatchesMuch = 0;
    fClustersMuch    = 0;

    fMuchPoints        = 0;
    listMuchHitMatches = 0;
  }
  else {
    fMuchPixelHits = (TClonesArray*) fairManager->GetObject("MuchPixelHit");
  }

  if (!fUseTRD) {
    fTrdPoints     = 0;
    fTrdHitMatches = 0;
    listTrdHits    = 0;
  }
  else {
    listTrdHits = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("TrdHit"));
  }

  if (!fUseTOF) {
    fTofPoints         = 0;
    fTofHitDigiMatches = 0;
    fTofHits           = 0;
  }
  else {
    fTofHits = (TClonesArray*) fairManager->GetObject("TofHit");
  }

  if (fPerformance) {
    CbmMCDataManager* mcManager = (CbmMCDataManager*) fairManager->GetObject("MCDataManager");
    if (NULL == mcManager) LOG(fatal) << GetName() << ": No CbmMCDataManager!";

    fMcEventHeader = mcManager->GetObject("MCEventHeader.");

    fMCTracks = mcManager->InitBranch("MCTrack");

    if (NULL == fMCTracks) LOG(fatal) << GetName() << ": No MCTrack data!";
    if (NULL == fMcEventHeader) LOG(fatal) << GetName() << ": No MC event header data!";

    if (!fLegacyEventMode) {
      fEventList = (CbmMCEventList*) fairManager->GetObject("MCEventList.");
      if (NULL == fEventList) LOG(fatal) << GetName() << ": No MCEventList data!";
    }

    if (fUseMVD) {
      fMvdPoints         = mcManager->InitBranch("MvdPoint");
      listMvdDigiMatches = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("MvdDigiMatch"));
      listMvdHitMatches  = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("MvdHitMatch"));
      if (!listMvdHitMatches) { LOG(error) << "No listMvdHitMatches provided, performance is not done correctly"; }
    }

    if (fUseSTS) {
      fStsPoints = mcManager->InitBranch("StsPoint");
      if (NULL == fStsPoints) LOG(fatal) << GetName() << ": No StsPoint data!";
    }

    if (!fUseTRD) {
      fTrdPoints     = 0;
      fTrdHitMatches = 0;
    }
    else {
      fTrdHitMatches = (TClonesArray*) fairManager->GetObject("TrdHitMatch");
      fTrdPoints     = mcManager->InitBranch("TrdPoint");
    }

    if (!fUseMUCH) {
      fMuchPoints        = 0;
      listMuchHitMatches = 0;
    }
    else {

      fDigisMuch         = (TClonesArray*) fairManager->GetObject("MuchDigi");
      fDigiMatchesMuch   = (TClonesArray*) fairManager->GetObject("MuchDigiMatch");
      fClustersMuch      = (TClonesArray*) fairManager->GetObject("MuchCluster");
      fMuchPoints        = mcManager->InitBranch("MuchPoint");
      listMuchHitMatches = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("MuchPixelHitMatch"));
    }

    if (!fUseTOF) {
      fTofPoints         = 0;
      fTofHitDigiMatches = 0;
    }
    else {
      fTofPoints         = mcManager->InitBranch("TofPoint");
      fTofHitDigiMatches = static_cast<TClonesArray*>(fairManager->GetObject("TofHitMatch"));
    }
  }
  else {
  }
  if (!fUseMVD) { listMvdHits = 0; }
  else {
    listMvdHits = L1_DYNAMIC_CAST<TClonesArray*>(fairManager->GetObject("MvdHit"));
  }

  NMvdStationsGeom  = 0;
  NStsStationsGeom  = 0;
  NMuchStationsGeom = 0;
  NTrdStationsGeom  = 0;
  NTOFStationGeom   = 0;
  NStationGeom      = 0;

  // TODO: Replace algo initialization in the constructor (S.Zharko)
  algo = &algo_static;

  /**************************
   ** Field initialization **
   **************************/

  fpInitManager->SetFieldFunction([](const double(&inPos)[3], double(&outB)[3]) {
    CbmKF::Instance()->GetMagneticField()->GetFieldValue(inPos, outB);
  });

  /***************************
   ** Target initialization **
   ***************************/

  auto& target = CbmKF::Instance()->vTargets[0];
  fpInitManager->SetTargetPosition(target.x, target.y, target.z);

  /*********************************
   ** Target field initialization **
   *********************************/

  fpInitManager->InitTargetField(/*zStep = */ 2.5 /*cm*/);  // Replace zStep -> sizeZfieldRegion = 2 * zStep (TODO)

  /**************************************
   **                                  **
   ** STATIONS GEOMETRY INITIALIZATION **
   **                                  **
   **************************************/


  /***************************************************
   ** Active tracking detector subsystems selection **
   ***************************************************/

  fActiveTrackingDetectorIDs.clear();

  if (fUseMVD) { fActiveTrackingDetectorIDs.insert(L1DetectorID::kMvd); }
  if (fUseSTS) { fActiveTrackingDetectorIDs.insert(L1DetectorID::kSts); }
  if (fUseMUCH) { fActiveTrackingDetectorIDs.insert(L1DetectorID::kMuch); }
  if (fUseTRD) { fActiveTrackingDetectorIDs.insert(L1DetectorID::kTrd); }
  if (fUseTOF) { fActiveTrackingDetectorIDs.insert(L1DetectorID::kTof); }
  fpInitManager->SetActiveDetectorIDs(fActiveTrackingDetectorIDs);

  /*********************************************************************
   ** Counting numbers of stations for different detector subsystems  **
   *********************************************************************/

  //CbmMuchGeoScheme* fGeoScheme = CbmMuchGeoScheme::Instance();

  /*** MuCh ***/
  //if (fUseMUCH) {
  //  /// Save old global file and folder pointer to avoid messing with FairRoot
  //  fGeoScheme->Init(fMuchDigiFile, 0);
  //  for (int iStation = 0; iStation < fGeoScheme->GetNStations(); iStation++) {
  //    const CbmMuchStation* station = fGeoScheme->GetStation(iStation);
  //    int nLayers                   = station->GetNLayers();
  //    NMuchStationsGeom += nLayers;
  //    std::cout << "\033[1;31mMUCH: station " << iStation << " layer\033[0m\n";
  //  }
  //}

  /*** TRD ***/
  //if (fUseTRD) {
  //  Int_t layerCounter  = 0;
  //  TObjArray* topNodes = gGeoManager->GetTopNode()->GetNodes();
  //  for (Int_t iTopNode = 0; iTopNode < topNodes->GetEntriesFast(); iTopNode++) {
  //    TGeoNode* topNode = static_cast<TGeoNode*>(topNodes->At(iTopNode));
  //    if (TString(topNode->GetName()).Contains("trd")) {
  //      TObjArray* layers = topNode->GetNodes();
  //      for (Int_t iLayer = 0; iLayer < layers->GetEntriesFast(); iLayer++) {
  //        TGeoNode* layer = static_cast<TGeoNode*>(layers->At(iLayer));
  //        if (TString(layer->GetName()).Contains("layer")) { layerCounter++; }
  //      }
  //    }
  //  }
  //  NTrdStationsGeom = layerCounter;
  //  //if ((fTrackingMode == L1Algo::TrackingMode::kMcbm) && (fMissingHits)) { NTrdStationsGeom = NTrdStationsGeom - 1; }
  //  LOG(info) << " NTrdStations " << NTrdStationsGeom;
  //}

  /*** ToF ***/
  vector<float> TofStationZ;
  vector<int> TofStationN;
  if (fUseTOF) {
    NTOFStationGeom = fTofDigiBdfPar->GetNbTrackingStations();
    TofStationZ.resize(NTOFStationGeom, 0);
    TofStationN.resize(NTOFStationGeom, 0);

    for (int iSmType = 0; iSmType < fTofDigiBdfPar->GetNbSmTypes(); iSmType++) {
      for (int iSm = 0; iSm < fTofDigiBdfPar->GetNbSm(iSmType); iSm++) {
        for (int iRpc = 0; iRpc < fTofDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
          int iAddr                = CbmTofAddress::GetUniqueAddress(iSm, iRpc, 0, 0, iSmType);
          int station              = fTofDigiBdfPar->GetTrackingStation(iSmType, iSm, iRpc);
          CbmTofCell* fChannelInfo = fDigiPar->GetCell(iAddr);
          if (NULL == fChannelInfo) break;
          float z = fChannelInfo->GetZ();
          float x = fChannelInfo->GetX();
          //float y = fChannelInfo->GetY();

          if (station < 0) continue;

          if (fMissingHits) {
            if ((x > 20) && (z > 270) && (station == 1)) station = 2;
            if (z > 400) continue;
          }
          TofStationZ[station] += z;
          TofStationN[station] += 1;
        }
      }
    }
    for (Int_t i = 0; i < NTOFStationGeom; i++)
      TofStationZ[i] = TofStationZ[i] / TofStationN[i];
  }


  /*** MVD and STS ***/
  auto mvdInterface  = CbmMvdTrackingInterface::Instance();
  auto stsInterface  = CbmStsTrackingInterface::Instance();
  auto muchInterface = CbmMuchTrackingInterface::Instance();
  auto trdInterface  = CbmTrdTrackingInterface::Instance();

  NMvdStationsGeom  = (fUseMVD) ? mvdInterface->GetNtrackingStations() : 0;
  NStsStationsGeom  = (fUseSTS) ? stsInterface->GetNtrackingStations() : 0;
  NMuchStationsGeom = (fUseMUCH) ? muchInterface->GetNtrackingStations() : 0;
  NTrdStationsGeom  = (fUseTRD) ? trdInterface->GetNtrackingStations() : 0;
  NStationGeom      = NMvdStationsGeom + NStsStationsGeom + NMuchStationsGeom + NTrdStationsGeom + NTOFStationGeom;

  // Provide crosscheck number of stations for the fpInitManagera
  fpInitManager->SetNstations(L1DetectorID::kMvd, NMvdStationsGeom);
  fpInitManager->SetNstations(L1DetectorID::kSts, NStsStationsGeom);
  fpInitManager->SetNstations(L1DetectorID::kMuch, NMuchStationsGeom);
  fpInitManager->SetNstations(L1DetectorID::kTrd, NTrdStationsGeom);
  fpInitManager->SetNstations(L1DetectorID::kTof, NTOFStationGeom);

  {
    if (fSTAPDataMode % 2 == 1) {  // 1,3
      LOG(fatal) << "CbmL1::Init: geo vector was removed, currently data cannot be written to a text-file";
      // TODO: Rewrite parameters i/o into L1InitManager (S.Zharko, 12.05.2022)
      //WriteSTAPGeoData(geo);
    };
    //if(fSTAPDataMode >= 2){ // 2,3
    //  int ind2, ind = geo.size();
    //  ReadSTAPGeoData(geo, ind2);
    //  if (ind2 != ind)  LOG(error) << "-E- CbmL1: Read geometry from file " << fSTAPDataDir + "geo_algo.txt was NOT successful.";
    //};
  }

  if (fSTAPDataMode >= 2) {  // 2,3
    LOG(fatal) << "CbmL1::Init: geo vector was removed, currently data cannot be read from a text-file. "
               << "Please, run CbmL1 task with STAPDataMode option < 2";
    // TODO: Rewrite parameters i/o into L1InitManager (S.Zharko, 12.05.2022)
    //int ind2, ind = geo.size();
    //ReadSTAPGeoData(geo, ind2);
    //if (ind2 != ind)
    //  LOG(error) << "-E- CbmL1: Read geometry from file " << fSTAPDataDir + "geo_algo.txt was NOT successful.";
  }


  /****************************
   ** Material budget input  **
   ****************************/

  // NOTE: std::vector of material tables. Vector sizes correspond to number of stations provided by geometry, i.e. both
  //       active and inactive station are represented in the folloving vectors
  auto materialTableMvd  = ReadMaterialBudget(L1DetectorID::kMvd);
  auto materialTableSts  = ReadMaterialBudget(L1DetectorID::kSts);
  auto materialTableMuch = ReadMaterialBudget(L1DetectorID::kMuch);
  auto materialTableTrd  = ReadMaterialBudget(L1DetectorID::kTrd);
  auto materialTableTof  = ReadMaterialBudget(L1DetectorID::kTof);

  /* User corrections (optional) */
  auto correctionMvd = [this](L1Material& material, const L1MaterialInfo& homogenious) {
    this->ApplyCorrectionToMaterialMap<L1DetectorID::kMvd>(material, homogenious);
  };
  auto correctionSts = [this](L1Material& material, const L1MaterialInfo& homogenious) {
    this->ApplyCorrectionToMaterialMap<L1DetectorID::kSts>(material, homogenious);
  };
  auto correctionMuch = [this](L1Material& material, const L1MaterialInfo& homogenious) {
    this->ApplyCorrectionToMaterialMap<L1DetectorID::kMuch>(material, homogenious);
  };
  auto correctionTrd = [this](L1Material& material, const L1MaterialInfo& homogenious) {
    this->ApplyCorrectionToMaterialMap<L1DetectorID::kTrd>(material, homogenious);
  };
  auto correctionTof = [this](L1Material& material, const L1MaterialInfo& homogenious) {
    this->ApplyCorrectionToMaterialMap<L1DetectorID::kTof>(material, homogenious);
  };

  /***************************************
   ** Stations geometry initialization  **
   ***************************************/

  /*** MVD stations info ***/
  if (fUseMVD) {
    for (int iSt = 0; iSt < NMvdStationsGeom; ++iSt) {
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
      fpInitManager->AddStation(stationInfo);
      LOG(info) << "- MVD station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
    }
  }

  /*** STS stations info ***/
  if (fUseSTS) {
    for (int iSt = 0; iSt < NStsStationsGeom; ++iSt) {
      auto stationInfo = L1BaseStationInfo(L1DetectorID::kSts, iSt);
      stationInfo.SetStationType(0);  // STS
      stationInfo.SetTimeInfo(stsInterface->IsTimeInfoProvided(iSt));
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
      fpInitManager->AddStation(stationInfo);
      LOG(info) << "- STS station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
    }
  }

  /*** MuCh stations info ***/
  if (fUseMUCH) {
    for (int iSt = 0; iSt < NMuchStationsGeom; ++iSt) {
      auto stationInfo = L1BaseStationInfo(L1DetectorID::kMuch, iSt);
      stationInfo.SetStationType(2);  // MuCh
      stationInfo.SetTimeInfo(muchInterface->IsTimeInfoProvided(iSt));
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
      fpInitManager->AddStation(stationInfo);
      LOG(info) << "- MuCh station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
    }
  }

  /*** TRD stations info ***/
  if (fUseTRD) {
    for (int iSt = 0; iSt < NTrdStationsGeom; ++iSt) {
      auto stationInfo = L1BaseStationInfo(L1DetectorID::kTrd, iSt);
      stationInfo.SetStationType((iSt == 1 || iSt == 3) ? 6 : 3);  // MuCh
      stationInfo.SetTimeInfo(trdInterface->IsTimeInfoProvided(iSt));
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
      if (L1Algo::TrackingMode::kGlobal == fTrackingMode) {  //SGtrd2D!!
        trdFrontSigma = 1.1;
        trdBackSigma  = 1.1;
        stationInfo.SetTimeResolution(1.e10);
        stationInfo.SetTimeInfo(false);
      }
      stationInfo.SetFrontBackStripsGeometry(trdFrontPhi, trdFrontSigma, trdBackPhi, trdBackSigma);
      stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
      if (iSt == 1 && L1Algo::TrackingMode::kMcbm == fTrackingMode && fMissingHits) {
        stationInfo.SetTrackingStatus(false);
      }
      fpInitManager->AddStation(stationInfo);
      LOG(info) << "- TRD station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
    }
  }

  /*** TOF stations info ***/
  if (fUseTOF) {
    for (int iSt = 0; iSt < NTOFStationGeom; ++iSt) {
      auto stationInfo = L1BaseStationInfo(L1DetectorID::kTof, iSt);
      stationInfo.SetStationType(4);
      stationInfo.SetTimeInfo(1);
      stationInfo.SetTimeResolution(0.075);
      stationInfo.SetFieldStatus(0);
      stationInfo.SetZ(TofStationZ[iSt]);
      auto thickness = 10.;
      auto radLength = 2.;
      stationInfo.SetMaterialSimple(thickness, radLength);
      stationInfo.SetMaterialMap(std::move(materialTableTof[iSt]), correctionTof);
      stationInfo.SetXmax(20.);
      stationInfo.SetYmax(20.);
      stationInfo.SetRmin(0.);
      stationInfo.SetRmax(150.);
      fscal tofFrontPhi   = 0;
      fscal tofBackPhi    = TMath::Pi() / 2.;
      fscal tofFrontSigma = 0.42;
      fscal tofBackSigma  = 0.23;
      stationInfo.SetFrontBackStripsGeometry(tofFrontPhi, tofFrontSigma, tofBackPhi, tofBackSigma);
      stationInfo.SetTrackingStatus(target.z < stationInfo.GetZdouble() ? true : false);
      fpInitManager->AddStation(stationInfo);
      LOG(info) << "- TOF station " << iSt << " at z = " << stationInfo.GetZdouble() << " cm";
    }
  }

  /****************************************
   **                                    **
   ** TRACKING ITERATIONS INITIALIZATION **
   **                                    **
   ****************************************/

  // TODO: Need to provide a selection: default iterations input (these hardcoded ones), input from file or input
  //       from runing macro (S.Zharko)
  auto trackingIterFastPrim = L1CAIteration("FastPrimIter");
  trackingIterFastPrim.SetTrackChi2Cut(10.f);
  trackingIterFastPrim.SetTripletChi2Cut(23.4450f);  // = 7.815 * 3;  // prob = 0.05
  trackingIterFastPrim.SetDoubletChi2Cut(7.56327f);  // = 1.3449 * 2.f / 3.f;  // prob = 0.1
  trackingIterFastPrim.SetPickGather(3.0f);
  trackingIterFastPrim.SetPickNeighbour(5.0f);
  trackingIterFastPrim.SetMaxInvMom(1.0 / 0.5);
  trackingIterFastPrim.SetMaxSlopePV(1.1f);
  trackingIterFastPrim.SetMaxSlope(2.748f);
  trackingIterFastPrim.SetMaxDZ(0);
  trackingIterFastPrim.SetTargetPosSigmaXY(1, 1);
  trackingIterFastPrim.SetMinLevelTripletStart(0);
  trackingIterFastPrim.SetPrimary(true);

  auto trackingIterAllPrim = L1CAIteration("AllPrimIter");
  trackingIterAllPrim.SetTrackChi2Cut(10.f);
  trackingIterAllPrim.SetTripletChi2Cut(23.4450f);
  trackingIterAllPrim.SetDoubletChi2Cut(7.56327f);
  trackingIterAllPrim.SetPickGather(4.0f);
  trackingIterAllPrim.SetPickNeighbour(5.0f);
  trackingIterAllPrim.SetMaxInvMom(1.0 / 0.05);
  trackingIterAllPrim.SetMaxSlopePV(1.1f);
  trackingIterAllPrim.SetMaxSlope(2.748f);
  trackingIterAllPrim.SetMaxDZ(0.1);
  trackingIterAllPrim.SetTargetPosSigmaXY(1, 1);
  trackingIterAllPrim.SetMinLevelTripletStart(0);
  trackingIterAllPrim.SetPrimary(true);

  auto trackingIterFastPrim2 = L1CAIteration("FastPrim2Iter");
  trackingIterFastPrim2.SetTrackChi2Cut(10.f);
  trackingIterFastPrim2.SetTripletChi2Cut(21.1075f);
  trackingIterFastPrim2.SetDoubletChi2Cut(7.56327f);
  trackingIterFastPrim2.SetPickGather(3.0f);
  trackingIterFastPrim2.SetPickNeighbour(5.0f);
  trackingIterFastPrim2.SetMaxInvMom(1.0 / 0.5);
  trackingIterFastPrim2.SetMaxSlopePV(1.1f);
  trackingIterFastPrim2.SetMaxSlope(2.748f);
  trackingIterFastPrim2.SetMaxDZ(0);
  trackingIterFastPrim2.SetTargetPosSigmaXY(5, 5);
  trackingIterFastPrim2.SetMinLevelTripletStart(0);
  trackingIterFastPrim2.SetPrimary(true);

  auto trackingIterAllSec = L1CAIteration("AllSecIter");
  trackingIterAllSec.SetTrackChi2Cut(10.f);
  trackingIterAllSec.SetTripletChi2Cut(18.7560f);  // = 6.252 * 3;  // prob = 0.1
  trackingIterAllSec.SetDoubletChi2Cut(7.56327f);
  trackingIterAllSec.SetPickGather(4.0f);
  trackingIterAllSec.SetPickNeighbour(5.0f);
  trackingIterAllSec.SetMaxInvMom(1.0 / 0.1);
  trackingIterAllSec.SetMaxSlopePV(1.5f);
  trackingIterAllSec.SetMaxSlope(2.748f);
  trackingIterAllSec.SetMaxDZ(0.1);
  trackingIterAllSec.SetTargetPosSigmaXY(10, 10);
  trackingIterAllSec.SetMinLevelTripletStart(1);
  trackingIterAllSec.SetPrimary(false);

  auto trackingIterFastPrimJump = L1CAIteration("FastPrimJumpIter");
  trackingIterFastPrimJump.SetTrackChi2Cut(10.f);
  trackingIterFastPrimJump.SetTripletChi2Cut(21.1075f);  // prob = 0.01
  trackingIterFastPrimJump.SetDoubletChi2Cut(7.56327f);
  trackingIterFastPrimJump.SetPickGather(3.0f);
  trackingIterFastPrimJump.SetPickNeighbour(5.0f);
  trackingIterFastPrimJump.SetMaxInvMom(1.0 / 0.5);
  trackingIterFastPrimJump.SetMaxSlopePV(1.1f);
  trackingIterFastPrimJump.SetMaxSlope(2.748f);
  trackingIterFastPrimJump.SetMaxDZ(0);
  trackingIterFastPrimJump.SetTargetPosSigmaXY(5, 5);
  trackingIterFastPrimJump.SetMinLevelTripletStart(0);
  trackingIterFastPrimJump.SetPrimary(true);

  auto trackingIterAllPrimJump = L1CAIteration("AllPrimJumpIter");
  trackingIterAllPrimJump.SetTrackChi2Cut(10.f);
  trackingIterAllPrimJump.SetTripletChi2Cut(18.7560f);
  trackingIterAllPrimJump.SetDoubletChi2Cut(7.56327f);
  trackingIterAllPrimJump.SetPickGather(4.0f);
  trackingIterAllPrimJump.SetPickNeighbour(5.0f);
  trackingIterAllPrimJump.SetMaxInvMom(1.0 / 0.1);
  trackingIterAllPrimJump.SetMaxSlopePV(1.1f);
  trackingIterAllPrimJump.SetMaxSlope(2.748f);
  trackingIterAllPrimJump.SetMaxDZ(0.1);
  trackingIterAllPrimJump.SetTargetPosSigmaXY(5, 5);
  trackingIterAllPrimJump.SetMinLevelTripletStart(0);
  trackingIterAllPrimJump.SetPrimary(true);

  auto trackingIterAllSecJump = L1CAIteration("AllSecJumpIter");
  trackingIterAllSecJump.SetTrackChi2Cut(10.f);
  trackingIterAllSecJump.SetTripletChi2Cut(21.1075f);
  trackingIterAllSecJump.SetDoubletChi2Cut(7.56327f);
  trackingIterAllSecJump.SetPickGather(4.0f);
  trackingIterAllSecJump.SetPickNeighbour(5.0f);
  trackingIterAllSecJump.SetMaxInvMom(1.0 / 0.1);
  trackingIterAllSecJump.SetMaxSlopePV(1.5f);
  trackingIterAllSecJump.SetMaxSlope(2.748f);
  trackingIterAllSecJump.SetMaxDZ(0.1);
  trackingIterAllSecJump.SetMinLevelTripletStart(1);
  trackingIterAllSecJump.SetTargetPosSigmaXY(10, 10);

  auto trackingIterAllPrimE = L1CAIteration("AllPrimEIter");
  trackingIterAllPrimE.SetTrackChi2Cut(10.f);
  trackingIterAllPrimE.SetTripletChi2Cut(23.4450f);
  trackingIterAllPrimE.SetDoubletChi2Cut(7.56327f);
  trackingIterAllPrimE.SetPickGather(4.0f);
  trackingIterAllPrimE.SetPickNeighbour(5.0f);
  trackingIterAllPrimE.SetMaxInvMom(1.0 / 0.05);
  trackingIterAllPrimE.SetMaxSlopePV(1.1f);
  trackingIterAllPrimE.SetMaxSlope(2.748f);
  trackingIterAllPrimE.SetMaxDZ(0.1);
  trackingIterAllPrimE.SetTargetPosSigmaXY(1, 1);
  trackingIterAllPrimE.SetMinLevelTripletStart(0);
  trackingIterAllPrimE.SetPrimary(true);

  auto trackingIterAllSecE = L1CAIteration("AllSecEIter");
  trackingIterAllSecE.SetTrackChi2Cut(10.f);
  trackingIterAllSecE.SetTripletChi2Cut(18.7560f);
  trackingIterAllSecE.SetDoubletChi2Cut(7.56327f);
  trackingIterAllSecE.SetPickGather(4.0f);
  trackingIterAllSecE.SetPickNeighbour(5.0f);
  trackingIterAllSecE.SetMaxInvMom(1.0 / 0.05);
  trackingIterAllSecE.SetMaxSlopePV(1.5f);
  trackingIterAllSecE.SetMaxSlope(2.748f);
  trackingIterAllSecE.SetMaxDZ(0.1);
  trackingIterAllSecE.SetMinLevelTripletStart(1);
  trackingIterAllSecE.SetTargetPosSigmaXY(10, 10);

  // Select default track finder
  if (L1Algo::TrackingMode::kMcbm == fTrackingMode) {
    trackingIterAllPrim.SetMaxInvMom(1. / 0.1);
    trackingIterAllPrimE.SetMaxInvMom(1. / 0.1);
    trackingIterAllSecE.SetMaxInvMom(1. / 0.1);

    trackingIterFastPrim.SetMaxInvMom(1.0 / 0.3);
    trackingIterAllSec.SetMaxInvMom(1.0 / 0.3);
    trackingIterFastPrimJump.SetMaxInvMom(1.0 / 0.3);
    trackingIterAllPrimJump.SetMaxInvMom(1.0 / 0.3);
    trackingIterAllSecJump.SetMaxInvMom(1.0 / 0.3);

    fpInitManager->SetCAIterationsNumberCrosscheck(4);
    // Initialize CA track finder iterations sequence
    fpInitManager->PushBackCAIteration(trackingIterFastPrim);
    fpInitManager->PushBackCAIteration(trackingIterAllPrim);
    fpInitManager->PushBackCAIteration(trackingIterAllPrimJump);
    fpInitManager->PushBackCAIteration(trackingIterAllSec);
  }
  else {
    fpInitManager->SetCAIterationsNumberCrosscheck(4);
    // Initialize CA track finder iterations sequence
    fpInitManager->PushBackCAIteration(trackingIterFastPrim);
    fpInitManager->PushBackCAIteration(trackingIterAllPrim);
    fpInitManager->PushBackCAIteration(trackingIterAllPrimJump);
    fpInitManager->PushBackCAIteration(trackingIterAllSec);
    //fpInitManager->PushBackCAIteration(trackingIterAllPrimE);
    //fpInitManager->PushBackCAIteration(trackingIterAllSecE);
    //fpInitManager->PushBackCAIteration(trackingIterFastPrimJump);
    //fpInitManager->PushBackCAIteration(trackingIterFastPrim2);
    //fpInitManager->PushBackCAIteration(trackingIterAllSecJump);
  }

  /**********************
   ** Set special cuts **
   **********************/

  fpInitManager->SetGhostSuppression(fGhostSuppression);
  fpInitManager->SetTrackingLevel(fTrackingLevel);
  fpInitManager->SetMomentumCutOff(fMomentumCutOff);

  /**********************/

  algo->Init(fUseHitErrors, fTrackingMode, fMissingHits);

  /*** Get numbers of active stations ***/

  NMvdStations  = fpInitManager->GetNstationsActive(L1DetectorID::kMvd);
  NStsStations  = fpInitManager->GetNstationsActive(L1DetectorID::kSts);
  NTrdStations  = fpInitManager->GetNstationsActive(L1DetectorID::kTrd);
  NMuchStations = fpInitManager->GetNstationsActive(L1DetectorID::kMuch);
  NTOFStation   = fpInitManager->GetNstationsActive(L1DetectorID::kTof);
  NStation      = fpInitManager->GetNstationsActive();

  LOG(info) << "----- Numbers of stations active in tracking -----";
  LOG(info) << "  MVD:    " << NMvdStations;
  LOG(info) << "  STS:    " << NStsStations;
  LOG(info) << "  MuCh:   " << NMuchStations;
  LOG(info) << "  TRD:    " << NTrdStations;
  LOG(info) << "  ToF:    " << NTOFStation;
  LOG(info) << "  Total:  " << NStation;


  return kSUCCESS;
}


void CbmL1::Exec(Option_t* /*option*/) {}

void CbmL1::Reconstruct(CbmEvent* event)
{
  static int nevent = 0;
  vFileEvent.clear();
  int nStsHits = 0;
  if (fUseSTS && listStsHits) { nStsHits = listStsHits->GetEntriesFast(); }

  L1Vector<std::pair<double, int>> SortHits("CbmL1::SortHits");
  SortHits.reserve(nStsHits);

  float start_t = 10000000000;

  // TODO: move these values to CbmL1Parameters namespace (S.Zharko)
  bool areDataLeft = true;   // whole TS processed?
  float TsStart    = 0;      // starting time of sub-TS
  float TsLength   = 10000;  // length of sub-TS
  float TsOverlap  = 15;     // min length of overlap region
  int FstHitinTs   = 0;      // 1st hit index in TS

  /// sort input hits by time
  for (Int_t j = 0; j < nStsHits; j++) {
    CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(j));
    double t      = sh->GetTime();
    if (t < start_t) start_t = t;
    SortHits.push_back(std::pair<double, int>(t, j));
  }

  TsStart = start_t;  ///reco TS start time is set to smallest hit time

  std::sort(SortHits.begin(), SortHits.end());
  StsIndex.clear();
  StsIndex.reserve(SortHits.size());
  for (unsigned int i = 0; i < SortHits.size(); i++) {
    int j = SortHits[i].second;
    StsIndex.push_back(j);
  };


  if (!fLegacyEventMode && fPerformance) {

    int nofEvents = fEventList->GetNofEvents();
    for (int iE = 0; iE < nofEvents; iE++) {
      int fileId  = fEventList->GetFileIdByIndex(iE);
      int eventId = fEventList->GetEventIdByIndex(iE);
      vFileEvent.insert(DFSET::value_type(fileId, eventId));
    }
  }
  else {
    Int_t iFile  = FairRunAna::Instance()->GetEventHeader()->GetInputFileId();
    Int_t iEvent = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
    vFileEvent.insert(DFSET::value_type(iFile, iEvent));
  }

  if (fVerbose > 1) { cout << "\nCbmL1::Exec event " << ++nevent << " ...\n\n"; }
#ifdef _OPENMP
  omp_set_num_threads(1);
#endif
  // repack data

  L1Vector<CbmL1Track> vRTracksCur("CbmL1::vRTracksCur");  // reconstructed tracks
  {
    int nHits = 0;
    int nSta  = 1;
    if (fUseMVD && listMvdHits) {
      nHits += listMvdHits->GetEntriesFast();
      nSta += NMvdStations;
    }
    if (fUseSTS && listStsHits) {
      nHits += listStsHits->GetEntriesFast();
      nSta += NStsStations;
    }
    vRTracksCur.reserve(10 + (2 * nHits) / nSta);
  }

  fTrackingTime = 0;


  while (areDataLeft) {
    fData->Clear();

    if (event) {

      fData->fStripFlag.clear();

      areDataLeft = false;
      TsStart     = 0;
      TsLength    = 2000000000;
      TsOverlap   = 0;
      FstHitinTs  = 0;
    }

    if (fSTAPDataMode >= 2) {  // 2,3
      fData->ReadHitsFromFile(fSTAPDataDir.Data(), 1, fVerbose);

      algo->SetData(fData->GetHits(), fData->GetNstrips(), fData->GetSFlag(), fData->GetHitsStartIndex(),
                    fData->GetHitsStopIndex());
    }
    else {
      ReadEvent(fData, TsStart, TsLength, TsOverlap, FstHitinTs, areDataLeft, event);
    }

    if constexpr (0) {  // correct hits on MC // dbg
      TRandom3 random;
      L1Vector<int> strips("CbmL1::strips");
      for (unsigned int iH = 0; iH < (*algo->vHits).size(); ++iH) {
        L1Hit& h = const_cast<L1Hit&>((*algo->vHits)[iH]);
#ifdef USE_EVENT_NUMBER
        h.n = -1;
#endif
        if (vHits[iH].mcPointIds.size() == 0) continue;

        const CbmL1MCPoint& mcp = vMCPoints[vHits[iH].mcPointIds[0]];

#ifdef USE_EVENT_NUMBER
        h.n = mcp.event;
#endif
        const int ista       = (*algo->fStripFlag)[h.f] / 4;
        const L1Station& sta = algo->GetParameters()->GetStation(ista);
        if (std::find(strips.begin(), strips.end(), h.f) != strips.end()) {  // separate strips

          (*algo->fStripFlag).push_back((*algo->fStripFlag)[h.f]);

          h.f = algo->fNstrips;
          algo->fNstrips++;
        }
        strips.push_back(h.f);
        if (std::find(strips.begin(), strips.end(), h.b) != strips.end()) {
          (*algo->fStripFlag).push_back((*algo->fStripFlag)[h.b]);
          h.b = algo->fNstrips;
          algo->fNstrips++;
        }
        strips.push_back(h.b);

        double u = mcp.x * sta.frontInfo.cos_phi[0] + mcp.y * sta.frontInfo.sin_phi[0];
        double v = mcp.x * sta.backInfo.cos_phi[0] + mcp.y * sta.backInfo.sin_phi[0];

#if 1  // GAUSS
        u += random.Gaus(0, sqrt(sta.frontInfo.sigma2)[0]);
        v += random.Gaus(0, sqrt(sta.backInfo.sigma2)[0]);
#else  // UNIFORM
        u += 3.5 * sqrt(sta.frontInfo.sigma2)[0] * random.Uniform(-1, 1);
        v += 3.5 * sqrt(sta.backInfo.sigma2)[0] * random.Uniform(-1, 1);
#endif
        h.u = u;
        h.v = v;
        h.z = mcp.z;
      }
    }

    if (fPerformance) {
      HitMatch();
      // calculate the max number of Hits\mcPoints on continuous(consecutive) stations
      for (L1Vector<CbmL1MCTrack>::iterator it = vMCTracks.begin(); it != vMCTracks.end(); ++it)
        it->Init();
    }

    if (fSTAPDataMode % 2 == 1) {  // 1,3
      WriteSTAPAlgoData();
      WriteSTAPPerfData();
    };
    if (fSTAPDataMode >= 2) {  // 2,3
      //ReadSTAPAlgoData();

      ReadSTAPPerfData();
    };

    if ((fPerformance) && (fSTAPDataMode < 2)) { InputPerformance(); }

    //  FieldApproxCheck();
    //  FieldIntegralCheck();

    for (unsigned int iH = 0; iH < (*algo->vHits).size(); ++iH) {
#ifdef USE_EVENT_NUMBER
      L1Hit& h = const_cast<L1Hit&>((*algo->vHits)[iH]);
      h.n      = -1;
#endif
      if (vHits[iH].mcPointIds.size() == 0) continue;
#ifdef USE_EVENT_NUMBER
      const CbmL1MCPoint& mcp = vMCPoints[vHits[iH].mcPointIds[0]];
      h.n                     = mcp.event;
#endif
    }

    if (fVerbose > 1) { cout << "L1 Track finder..." << endl; }
    algo->CATrackFinder();
    //      IdealTrackFinder();
    fTrackingTime += algo->fCATime;

    if (fVerbose > 1) { cout << "L1 Track finder ok" << endl; }
    //  algo->L1KFTrackFitter( fExtrapolateToTheEndOfSTS );

    {  // track fit
      L1FieldValue b = algo->GetParameters()->GetVertexFieldValue();

      if ((fabs(b.x[0]) < 0.0000001) && (fabs(b.y[0]) < 0.0000001) && (fabs(b.z[0]) < 0.0000001)) {
        algo->KFTrackFitter_simple();
      }
      else {
        if (L1Algo::TrackingMode::kGlobal == fTrackingMode || L1Algo::TrackingMode::kMcbm == fTrackingMode) {
          algo->L1KFTrackFitterMuch();
        }
        else {
          algo->L1KFTrackFitter();
        }
      }
    }

    if (fVerbose > 1) { cout << "L1 Track fitter  ok" << endl; }

    // save recontstructed tracks
    if (fLegacyEventMode) vRTracksCur.clear();
    int start_hit = 0;

    float TsStart_new = TsStart + TsLength - TsOverlap;

    for (L1Vector<L1Track>::iterator it = algo->fTracks.begin(); it != algo->fTracks.end(); it++) {

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
        if (algo->fRecoHits[start_hit1] > vHits.size() - 1) start_hit++;
        else if (!fLegacyEventMode) {
          t.Hits.push_back(((*algo->vHits)[algo->fRecoHits[start_hit]]).ID);
        }
        else {
          t.Hits.push_back(algo->fRecoHits[start_hit]);
        }
        HitsLocal.push_back(algo->fRecoHits[start_hit++]);
      }

      t.mass        = algo->fDefaultMass;  // pion mass
      t.is_electron = 0;

      t.SetId(vRTracksCur.size());
      //  CbmL1Track* prtra = &t;

      int indd         = 0;
      bool isInOverlap = 0;


      for (unsigned int i = 0; i < HitsLocal.size(); i++) {
        //      if ((*ih) > int(vHits.size() - 1)) {
        //         indd = 1;
        //         break;
        //       }

        if (vHits[HitsLocal[i]].t >= (TsStart + TsLength - TsOverlap)) {
          isInOverlap = 1;
          if (TsStart_new > vHits[HitsLocal[i]].t) TsStart_new = vHits[HitsLocal[i]].t;
        }

        int nMCPoints = vHits[HitsLocal[i]].mcPointIds.size();
        for (int iP = 0; iP < nMCPoints; iP++) {
          int iMC = vHits[HitsLocal[i]].mcPointIds[iP];
          if (iMC > int(vMCPoints.size() - 1)) {
            //           cout << " iMC " << iMC << " vMCPoints.size() " <<  vMCPoints.size() << endl;
            indd = 1;
          }
        }
      }

      if (indd == 1) continue;

      if ((!fLegacyEventMode) && (isInOverlap == 1)) {

        continue;  ///Discard tracks from overlap region

        /// set strips as unused
        for (unsigned int i = 0; i < HitsLocal.size(); i++) {
          algo->SetFUnUsed(const_cast<unsigned char&>((*algo->fStripFlag)[vHits[HitsLocal[i]].f]));
          algo->SetFUnUsed(const_cast<unsigned char&>((*algo->fStripFlag)[vHits[HitsLocal[i]].b]));
        }
      }
      vRTracksCur.push_back(t);
    }

    for (int i = 0; i < nStsHits; i++) {

      CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(StsIndex[i]));
      float time    = sh->GetTime();

      if (TsStart_new <= time) {
        FstHitinTs = i;
        break;
      }
    }

    if (!fLegacyEventMode) TsStart = TsStart_new;  ///Set new TS strat to earliest discarted track

    if (!fLegacyEventMode) cout << "CA Track Finder: " << algo->fCATime << " s/sub-ts" << endl << endl;
  }


  if (fPerformance) {

    float start = 0;
    float end   = 10000000000.f;
    int fHit    = 0;
    bool stop   = 0;

    ReadEvent(fData, start, end, start, fHit, stop, event);
    HitMatch();
    // calculate the max number of Hits\mcPoints on continuous(consecutive) stations

    for (L1Vector<CbmL1MCTrack>::iterator it = vMCTracks.begin(); it != vMCTracks.end(); ++it)
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

  vRTracks.clear();
  vRTracks.reserve(vRTracksCur.size());
  for (unsigned int iTrack = 0; iTrack < vRTracksCur.size(); iTrack++) {

    for (unsigned int iHit = 0; iHit < vRTracksCur[iTrack].Hits.size(); iHit++)
      if (!fLegacyEventMode) vRTracksCur[iTrack].Hits[iHit] = SortedIndex[vRTracksCur[iTrack].Hits[iHit]];

    vRTracks.push_back(vRTracksCur[iTrack]);
  }


  if ((fPerformance) && (fSTAPDataMode < 2)) { InputPerformance(); }


  for (unsigned int iH = 0; iH < (*algo->vHits).size(); ++iH) {
#ifdef USE_EVENT_NUMBER
    L1Hit& h = const_cast<L1Hit&>((*algo->vHits)[iH]);
    h.n      = -1;
#endif
    if (vHits[iH].mcPointIds.size() == 0) continue;
#ifdef USE_EVENT_NUMBER
    const CbmL1MCPoint& mcp = vMCPoints[vHits[iH].mcPointIds[0]];
    h.n                     = mcp.event;
#endif
  }

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
  algo->fTracks.clear();
  algo->fRecoHits.clear();

  for (L1Vector<CbmL1MCTrack>::iterator i = vMCTracks.begin(); i != vMCTracks.end(); ++i) {
    CbmL1MCTrack& MC = *i;

    if (!MC.IsReconstructable()) continue;
    if (!(MC.ID >= 0)) continue;

    if (MC.Hits.size() < 4) continue;

    L1Track algoTr;
    algoTr.NHits = 0;

    L1Vector<int> hitIndices("CbmL1::hitIndices", algo->GetNstations(), -1);

    for (unsigned int iH = 0; iH < MC.Hits.size(); iH++) {
      const int hitI      = MC.Hits[iH];
      const CbmL1Hit& hit = vHits[hitI];

      const int iStation = vMCPoints[hit.mcPointIds[0]].iStation;

      if (iStation >= 0) hitIndices[iStation] = hitI;
    }


    for (int iH = 0; iH < algo->GetNstations(); iH++) {
      const int hitI = hitIndices[iH];
      if (hitI < 0) continue;

      // algo->fRecoHits.push_back(hitI);
      algoTr.NHits++;
    }


    if (algoTr.NHits < 3) continue;

    for (int iH = 0; iH < algo->GetNstations(); iH++) {
      const int hitI = hitIndices[iH];
      if (hitI < 0) continue;
      algo->fRecoHits.push_back(hitI);
    }


    algoTr.Momentum  = MC.p;
    algoTr.TFirst[0] = MC.x;
    algoTr.TFirst[1] = MC.y;
    algoTr.TFirst[2] = MC.px / MC.pz;
    algoTr.TFirst[3] = MC.py / MC.pz;
    algoTr.TFirst[4] = MC.q / MC.p;
    algoTr.TFirst[5] = MC.z;

    algo->fTracks.push_back(algoTr);
  }
}  // void CbmL1::IdealTrackFinder()


/// -----   STandAlone Package service-functions  -----------------------------

void CbmL1::WriteSTAPGeoData(const L1Vector<float>& geo_)
{
  // write geo in file
  TString fgeo_name = fSTAPDataDir + "geo_algo.txt";
  std::ofstream fgeo(fgeo_name);
  fgeo.setf(ios::scientific, ios::floatfield);
  fgeo.precision(20);
  int size = geo_.size();
  for (int i = 0; i < size; i++) {
    fgeo << geo_[i] << endl;
  };
  fgeo.close();
  cout << "-I- CbmL1: Geometry data has been written in " << fgeo_name << endl;
}  // void CbmL1::WriteSTAPGeoData(void* geo_, int size)


void CbmL1::WriteSTAPAlgoData()  // must be called after ReadEvent
{
  // write algo data in file
  static int vNEvent = 1;
  std::fstream fadata;

  TString fadata_name = fSTAPDataDir + "data_algo.txt";
  //    if ( vNEvent <= maxNEvent ) {
  if (1) {

    if (vNEvent == 1) fadata.open(fadata_name, std::fstream::out);  // begin new file
    else
      fadata.open(fadata_name, std::fstream::out | std::fstream::app);

    fadata << "Event:"
           << " ";
    fadata << vNEvent << endl;
    // write vStrips
    int n = algo->fNstrips;
    fadata << n << endl;
    if (fVerbose >= 4) {
      cout << "vStrips[" << n << "]"
           << " have been written." << endl;
    }
    // write fStripFlag
    n = (*algo->fStripFlag).size();
    fadata << n << endl;
    unsigned char element;
    for (int i = 0; i < n; i++) {
      element = (*algo->fStripFlag)[i];
      fadata << static_cast<int>(element) << endl;
    };
    if (fVerbose >= 4) {
      cout << "fStripFlag[" << n << "]"
           << " have been written." << endl;
    }
    if (fVerbose >= 4) {
      cout << "fStripFlagB[" << n << "]"
           << " have been written." << endl;
    }
    // write vHits
    n = (*algo->vHits).size();
    fadata << n << endl;
    for (int i = 0; i < n; i++) {
      const L1Hit& h = (*algo->vHits)[i];
      fadata << static_cast<int>(h.f) << " ";
      fadata << static_cast<int>(h.b) << " ";
#ifdef USE_EVENT_NUMBER
      fadata << static_cast<unsigned short int>(h.n) << " ";
#endif
      fadata << h.z << " ";
      fadata << h.u << " ";
      fadata << h.v << " ";
      // fadata  << (*algo->vHits)[i].time << endl;
      fadata << h.t << endl;
    };
    if (fVerbose >= 4) {
      cout << "vHits[" << n << "]"
           << " have been written." << endl;
    }
    // write HitsStartIndex and HitsStopIndex
    n = 20;
    for (int i = 0; i < n; i++) {
      if (int(L1Constants::size::kMaxNstations) + 1 > i) { fadata << algo->HitsStartIndex[i] << endl; }
      else {
        fadata << 0 << endl;
      }
    };
    for (int i = 0; i < n; i++) {
      if (int(L1Constants::size::kMaxNstations) + 1 > i) fadata << algo->HitsStopIndex[i] << endl;
      else
        fadata << 0 << endl;
    };


    fadata.close();
  }
  cout << "-I- CbmL1: CATrackFinder data for event number " << vNEvent << " have been written in file " << fadata_name
       << endl;
  vNEvent++;
}  // void CbmL1::WriteSTAPAlgoData()


void CbmL1::WriteSTAPPerfData()  // must be called after ReadEvent
{
  std::fstream fpdata;
  fpdata << std::setprecision(8);

  static int vNEvent = 1;

  TString fpdata_name = fSTAPDataDir + "data_perfo.txt";
  // write data for performance in file
  //   if ( vNEvent <= maxNEvent )  {
  if (1) {

    if (vNEvent == 1) fpdata.open(fpdata_name, std::fstream::out);  // begin new file
    else
      fpdata.open(fpdata_name, std::fstream::out | std::fstream::app);

    fpdata << "Event: ";
    fpdata << vNEvent << endl;
    // write vMCPoints
    Int_t n = vMCPoints.size();  // number of elements
    fpdata << n << endl;
    for (int i = 0; i < n; i++) {
      fpdata << vMCPoints[i].xIn << " ";
      fpdata << vMCPoints[i].yIn << " ";
      fpdata << vMCPoints[i].zIn << "  ";
      fpdata << vMCPoints[i].pxIn << " ";
      fpdata << vMCPoints[i].pyIn << " ";
      fpdata << vMCPoints[i].pzIn << " " << endl;
      fpdata << vMCPoints[i].xOut << " ";
      fpdata << vMCPoints[i].yOut << " ";
      fpdata << vMCPoints[i].zOut << "  ";
      fpdata << vMCPoints[i].pxOut << " ";
      fpdata << vMCPoints[i].pyOut << " ";
      fpdata << vMCPoints[i].pzOut << " " << endl;

      fpdata << vMCPoints[i].p << "  ";
      fpdata << vMCPoints[i].q << " ";
      fpdata << vMCPoints[i].mass << " ";
      fpdata << vMCPoints[i].time << "   ";

      fpdata << vMCPoints[i].pdg << " ";
      fpdata << vMCPoints[i].ID << " ";
      fpdata << vMCPoints[i].mother_ID << " ";
      fpdata << vMCPoints[i].iStation << endl;

      const int nhits = vMCPoints[i].hitIds.size();
      fpdata << nhits << endl << "   ";
      for (int k = 0; k < nhits; k++) {
        fpdata << vMCPoints[i].hitIds[k] << " ";
      };
      fpdata << endl;
    };
    if (fVerbose >= 4) {
      cout << "vMCPoints[" << n << "]"
           << " have been written." << endl;
    }
    // write vMCTracks  . without Points
    n = vMCTracks.size();  // number of elements
    fpdata << n << endl;
    for (int i = 0; i < n; i++) {
      fpdata << vMCTracks[i].x << " ";
      fpdata << vMCTracks[i].y << " ";
      fpdata << vMCTracks[i].z << "  ";
      fpdata << vMCTracks[i].px << " ";
      fpdata << vMCTracks[i].py << " ";
      fpdata << vMCTracks[i].pz << " ";
      fpdata << vMCTracks[i].p << "  ";
      fpdata << vMCTracks[i].q << " ";
      fpdata << vMCTracks[i].mass << " ";
      fpdata << vMCTracks[i].time << "   ";

      fpdata << vMCTracks[i].pdg << " ";
      fpdata << vMCTracks[i].ID << " ";
      fpdata << vMCTracks[i].mother_ID << endl;

      int nhits = vMCTracks[i].Hits.size();
      fpdata << "   " << nhits << endl << "   ";
      for (int k = 0; k < nhits; k++) {
        fpdata << vMCTracks[i].Hits[k] << " ";
      };
      fpdata << endl;

      const int nPoints = vMCTracks[i].Points.size();
      fpdata << nPoints << endl << "   ";
      for (int k = 0; k < nPoints; k++) {
        fpdata << vMCTracks[i].Points[k] << " ";
      };
      fpdata << endl;

      fpdata << vMCTracks[i].nMCContStations << " ";
      fpdata << vMCTracks[i].nHitContStations << " ";
      fpdata << vMCTracks[i].maxNStaMC << " ";
      fpdata << vMCTracks[i].maxNSensorMC << " ";
      fpdata << vMCTracks[i].maxNStaHits << " ";
      fpdata << vMCTracks[i].nStations << endl;
    };
    if (fVerbose >= 4) {
      cout << "vMCTracks[" << n << "]"
           << " have been written." << endl;
    }
    // write vHitMCRef
    n = vHitMCRef.size();  // number of elements
    fpdata << n << endl;
    for (int i = 0; i < n; i++) {
      fpdata << vHitMCRef[i] << endl;
    };
    if (fVerbose >= 4) {
      cout << "vHitMCRef[" << n << "]"
           << " have been written." << endl;
    }
    // write vHitStore
    n = vHitStore.size();  // number of elements
    fpdata << n << endl;
    for (int i = 0; i < n; i++) {
      fpdata << vHitStore[i].ExtIndex << "  ";
      fpdata << vHitStore[i].iStation << "  ";

      fpdata << vHitStore[i].x << " ";
      fpdata << vHitStore[i].y << endl;
    };
    if (fVerbose >= 4) {
      cout << "vHitStore[" << n << "]"
           << " have been written." << endl;
    }
    // write vHits
    n = vHits.size();  // number of elements
    fpdata << n << endl;
    for (int i = 0; i < n; i++) {
      fpdata << vHits[i].hitId << " ";
      fpdata << vHits[i].extIndex << endl;

      const int nPoints = vHits[i].mcPointIds.size();
      fpdata << nPoints << endl << "   ";
      for (int k = 0; k < nPoints; k++) {
        fpdata << vHits[i].mcPointIds[k] << " ";
      };
      fpdata << endl;
    };
    if (fVerbose >= 4) {
      cout << "vHits[" << n << "]"
           << " have been written." << endl;
    }
    fpdata.close();
  }
  cout << "-I- CbmL1: Data for performance of event number " << vNEvent << " have been written in file " << fpdata_name
       << endl;
  vNEvent++;
}  // void CbmL1::WriteSTAPPerfData()

std::istream& CbmL1::eatwhite(std::istream& is)  // skip spaces
{
  char c;
  while (is.get(c)) {
    if (isspace(c) == 0) {
      is.putback(c);
      break;
    }
  }
  return is;
}

//void CbmL1::ReadSTAPGeoData(L1Vector<float> geo_, int &size)
//void CbmL1::ReadSTAPGeoData(L1Vector<fscal> geo_, int &size)
void CbmL1::ReadSTAPGeoData(L1Vector<fscal>& geo_, int& size)
{
  TString fgeo_name = fSTAPDataDir + "geo_algo.txt";
  std::ifstream fgeo(fgeo_name);

  cout << "-I- CbmL1: Read geometry from file " << fgeo_name << endl;
  int i;
  for (i = 0; !fgeo.eof(); i++) {
    fscal tmp;
    fgeo >> tmp >> eatwhite;
    cout << " geo_[" << i << "]=" << geo_[i] << " tmp= " << tmp << endl;
    geo_[i] = tmp;
  };
  size = i;
  fgeo.close();
}  // void CbmL1::ReadSTAPGeoData(void* geo_, int &size)

void CbmL1::ReadSTAPAlgoData()
{
  static int nEvent = 1;
  static std::fstream fadata;
  TString fadata_name = fSTAPDataDir + "data_algo.txt";
  //  if (nEvent <= maxNEvent){
  if (1) {
    if (nEvent == 1) fadata.open(fadata_name, std::fstream::in);

    if (algo->vHits) algo->vHits->clear();
    algo->fNstrips = 0;
    if (algo->fStripFlag) algo->fStripFlag->clear();

    // check correct position in file
    char s[] = "Event:  ";
    int nEv;
    fadata >> s;
    fadata >> nEv;
    if (nEv != nEvent) cout << "-E- CbmL1: Can't read event number " << nEvent << " from file " << fadata_name << endl;

    int n;  // number of elements
    // read algo->vStrips
    fadata >> n;
    cout << " n " << n << endl;
    algo->fNstrips = n;
    if (fVerbose >= 4) {
      cout << "vStrips[" << n << "]"
           << " have been read." << endl;
    }
    // read algo->fStripFlag
    fadata >> n;
    for (int i = 0; i < n; i++) {
      int element;
      fadata >> element;
      algo->fStripFlag->push_back(static_cast<unsigned char>(element));
    }
    if (fVerbose >= 4) {
      cout << "fStripFlag[" << n << "]"
           << " have been read." << endl;
    }
    // read algo->vHits
    fadata >> n;
    int element_f;  // for convert
    int element_b;
    int element_n;
    for (int i = 0; i < n; i++) {
      L1Hit element;
      fadata >> element_f >> element_b >> element_n >> element.z >> element.u >> element.v >> element.t;
      element.f = static_cast<L1HitIndex_t>(element_f);
      element.b = static_cast<L1HitIndex_t>(element_b);
      algo->vHits->push_back(element);
    }
    if (fVerbose >= 4) {
      cout << "vHits[" << n << "]"
           << " have been read." << endl;
    }
    // read HitsStartIndex and HitsStopIndex
    n = 20;  // TODO: Why 20? (S.Zh.)
    for (int i = 0; i < n; i++) {
      int tmp;
      fadata >> tmp;
      if (int(L1Constants::size::kMaxNstations) + 1 > i) (const_cast<unsigned int&>(algo->HitsStartIndex[i]) = tmp);
    }
    for (int i = 0; i < n; i++) {
      int tmp;
      fadata >> tmp;
      if (int(L1Constants::size::kMaxNstations) + 1 > i) (const_cast<unsigned int&>(algo->HitsStopIndex[i]) = tmp);
    }

    cout << "-I- CbmL1: CATrackFinder data for event " << nEvent << " has been read from file " << fadata_name
         << " successfully." << endl;
  }
  nEvent++;
}  // void CbmL1::ReadSTAPAlgoData()

void CbmL1::ReadSTAPPerfData()
{
  static int nEvent = 1;
  static std::fstream fpdata;
  TString fpdata_name = fSTAPDataDir + "data_perfo.txt";
  //  if (nEvent <= maxNEvent){
  if (1) {
    if (nEvent == 1) { fpdata.open(fpdata_name, std::fstream::in); };

    vMCPoints.clear();
    vMCTracks.clear();
    vHitMCRef.clear();
    vHitStore.clear();
    vHits.clear();
    dFEI2vMCPoints.clear();
    dFEI2vMCTracks.clear();
    // check if it is right position in file
    char s[] = "EVENT:     ";  // buffer
    int nEv  = 0;              // event number
    fpdata >> s;
    fpdata >> nEv;

    if (nEv != nEvent)
      cout << "-E- CbmL1: Performance: can't read event number " << nEvent << " from file "
           << "data_perfo.txt" << endl;
    // vMCPoints
    int n;  // number of elements
    fpdata >> n;
    for (int i = 0; i < n; i++) {
      CbmL1MCPoint element;

      fpdata >> element.xIn;
      fpdata >> element.yIn;
      fpdata >> element.zIn;
      fpdata >> element.pxIn;
      fpdata >> element.pyIn;
      fpdata >> element.pzIn;

      fpdata >> element.xOut;
      fpdata >> element.yOut;
      fpdata >> element.zOut;
      fpdata >> element.pxOut;
      fpdata >> element.pyOut;
      fpdata >> element.pzOut;

      fpdata >> element.p;
      fpdata >> element.q;
      fpdata >> element.mass;
      fpdata >> element.time;

      fpdata >> element.pdg;
      fpdata >> element.ID;
      fpdata >> element.mother_ID;
      fpdata >> element.iStation;

      int nhits;
      fpdata >> nhits;
      for (int k = 0; k < nhits; k++) {
        int helement;
        fpdata >> helement;
        element.hitIds.push_back(helement);
      };

      vMCPoints.push_back(element);
    };
    if (fVerbose >= 4) {
      cout << "vMCPoints[" << n << "]"
           << " have been read." << endl;
    }
    // vMCTracks . without Points
    fpdata >> n;
    for (int i = 0; i < n; i++) {
      CbmL1MCTrack element;

      fpdata >> element.x;
      fpdata >> element.y;
      fpdata >> element.z;
      fpdata >> element.px;
      fpdata >> element.py;
      fpdata >> element.pz;
      fpdata >> element.p;
      fpdata >> element.q;
      fpdata >> element.mass;
      fpdata >> element.time;

      fpdata >> element.pdg;
      fpdata >> element.ID;
      fpdata >> element.mother_ID;

      int nhits;
      fpdata >> nhits;
      for (int k = 0; k < nhits; k++) {
        int helement;
        fpdata >> helement;
        element.Hits.push_back(helement);
      };
      fpdata >> nhits;
      for (int k = 0; k < nhits; k++) {
        int helement;
        fpdata >> helement;
        element.Points.push_back(helement);
      };

      fpdata >> element.nMCContStations;
      fpdata >> element.nHitContStations;
      fpdata >> element.maxNStaMC;
      fpdata >> element.maxNSensorMC;
      fpdata >> element.maxNStaHits;
      fpdata >> element.nStations;

      element.CalculateIsReconstructable();
      vMCTracks.push_back(element);
    };
    if (fVerbose >= 4) {
      cout << "vMCTracks[" << n << "]"
           << " have been read." << endl;
    }
    // vHitMCRef
    fpdata >> n;
    vHitMCRef.reserve(n);
    for (int i = 0; i < n; i++) {
      int element;
      fpdata >> element;
      vHitMCRef.push_back(element);
    };
    if (fVerbose >= 4) {
      cout << "vHitMCRef[" << n << "]"
           << " have been read." << endl;
    }
    // vHitStore
    fpdata >> n;
    vHitStore.reserve(n);
    for (int i = 0; i < n; i++) {
      CbmL1HitStore element;
      fpdata >> element.ExtIndex;
      fpdata >> element.iStation;

      fpdata >> element.x;
      fpdata >> element.y;
      vHitStore.push_back(element);
    };
    if (fVerbose >= 4) {
      cout << "vHitStore[" << n << "]"
           << " have been read." << endl;
    }
    // vHits
    fpdata >> n;
    for (int i = 0; i < n; i++) {
      CbmL1Hit element;
      fpdata >> element.hitId;
      fpdata >> element.extIndex;

      int nPoints;
      fpdata >> nPoints;
      for (int k = 0; k < nPoints; k++) {
        int id;
        fpdata >> id;
        element.mcPointIds.push_back(id);
      };
      vHits.push_back(element);
    };
    if (fVerbose >= 4) {
      cout << "vHits[" << n << "]"
           << " have been read." << endl;
    }

    //    if (nEvent == maxNEvent) { // file open on begin of all work class and close at end
    //       fpdata.close();
    //       cout << " -I- Performance: data read from file " << "data_perfo.txt" << " successfully"<< endl;
    //     }
    cout << "-I- CbmL1: L1Performance data for event " << nEvent << " has been read from file " << fpdata_name
         << " successfully." << endl;

  }  // if (nEvent <= maxNEvent)
  nEvent++;
}  // void CbmL1::ReadSTAPPerfData()

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
    FileGeo << NStation << endl;

    const int M = 5;  // polinom order
    const int N = (M + 1) * (M + 2) / 2;

    for (Int_t ist = 0; ist < NStation; ist++) {
      fscal f_phi, f_sigma, b_phi, b_sigma;

      double C[3][N];
      double z = 0;
      double Xmax, Ymax;
      if (ist < NMvdStations) {
        CbmKFTube& t = CbmKF::Instance()->vMvdMaterial[ist];
        f_phi        = 0;
        f_sigma      = 5.e-4;
        b_phi        = 3.14159265358 / 2.;
        b_sigma      = 5.e-4;
        z            = t.z;
        Xmax = Ymax = t.R;
      }
      else {
        CbmStsStation* station = CbmStsSetup::Instance()->GetStation(ist - NMvdStations);
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
      if (ist < NMvdStations) {
        CbmKFTube& t = CbmKF::Instance()->vMvdMaterial[ist];
        FileGeo << t.z << " ";
        FileGeo << t.dz << " ";
        FileGeo << t.RadLength << " ";
      }
      else if (ist < (NStsStations + NMvdStations)) {
        CbmStsStation* station = CbmStsSetup::Instance()->GetStation(ist - NMvdStations);
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

  for (L1Vector<CbmL1Track>::iterator RecTrack = vRTracks.begin(); RecTrack != vRTracks.end(); ++RecTrack) {
    if (RecTrack->IsGhost()) continue;

    CbmL1MCTrack* MCTrack = RecTrack->GetMCTrack();
    if (!(MCTrack->IsPrimary())) continue;

    int NHits = (RecTrack->Hits).size();
    float x[20], y[20], z[20];
    int st[20];
    int jHit = 0;
    for (int iHit = 0; iHit < NHits; iHit++) {
      CbmL1HitStore& h = vHitStore[RecTrack->Hits[iHit]];
      st[jHit]         = h.iStation;
      if (h.ExtIndex < 0) {
        CbmMvdHit* MvdH = (CbmMvdHit*) listMvdHits->At(-h.ExtIndex - 1);
        x[jHit]         = MvdH->GetX();
        y[jHit]         = MvdH->GetY();
        z[jHit]         = MvdH->GetZ();
        jHit++;
      }
      else {
        CbmStsHit* StsH = (CbmStsHit*) listStsHits->At(h.ExtIndex);
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
      CbmL1MCPoint& MCPoint = vMCPoints[MCTrack->Points[iPoint]];
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

    result.resize(fpInitManager->GetNstationsGeometry(detectorID));
    TString stationNamePrefix = "Radiation Thickness [%], Station";

    // NOTE: Loop over geometry stations. We probably do not know which stations are active/inactive (S.Zharko)
    for (int iSt = 0; iSt < fpInitManager->GetNstationsGeometry(detectorID); ++iSt) {
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
          result[iSt].SetRadThick(iBinX, iBinY, 0.01 * hStaRadLen->GetBinContent(iBinX, iBinY));
        }  // iBinX
      }    // iBinY
      LOG(info) << "- station " << iSt;
    }  // iSt
    gFile      = oldFile;
    gDirectory = oldDir;
  }  // if mat budget file found
  else {
    LOG(warn) << "No material budget file is found for " << GetDetectorName(detectorID);
  }
  return result;
}
