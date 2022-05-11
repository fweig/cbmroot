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
#include "CbmMuchGeoScheme.h"
#include "CbmMuchModuleGem.h"
#include "CbmMuchPad.h"
#include "CbmMuchStation.h"
#include "CbmMvdDetector.h"
#include "CbmMvdStationPar.h"
#include "CbmSetup.h"

#include <boost/filesystem.hpp>
// TODO: include of CbmSetup.h creates problems on Mac
// #include "CbmSetup.h"
#include "CbmMCDataObject.h"
#include "CbmStsFindTracks.h"
#include "CbmStsHit.h"
#include "CbmStsParSetModule.h"
#include "CbmStsParSetSensor.h"
#include "CbmStsParSetSensorCond.h"
#include "CbmStsSetup.h"
#include "CbmStsStation.h"
#include "CbmTofCell.h"
#include "CbmTofDigiBdfPar.h"
#include "CbmTofDigiPar.h"     // in tof/TofParam
#include "CbmTrdParModDigi.h"  // for CbmTrdModule
#include "CbmTrdParSetDigi.h"  // for CbmTrdParSetDigi

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

  static L1Algo algo_static _fvecalignment; // TODO: gAlgo

//L1AlgoInputData* fData_static _fvecalignment;

CbmL1* CbmL1::fInstance = 0;


CbmL1::CbmL1() : FairTask("L1")
{
  if (!fInstance) fInstance = this;
  if (!fpInitManager) { fpInitManager = algo_static.GetInitManager(); }
}

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
}

CbmL1::~CbmL1()
{
  if (fInstance == this) fInstance = nullptr;
}

void CbmL1::CheckDetectorPresence()
{
  Bool_t IsMuch = CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch);
  Bool_t IsTrd  = CbmSetup::Instance()->IsActive(ECbmModuleId::kTrd);
  Bool_t IsTof  = CbmSetup::Instance()->IsActive(ECbmModuleId::kTof);
  //Bool_t IsSts        = CbmSetup::Instance()->IsActive(ECbmModuleId::kSts);
  Bool_t IsMvd = CbmSetup::Instance()->IsActive(ECbmModuleId::kMvd);

  /*
  TObjArray* topNodes = gGeoManager->GetTopNode()->GetNodes();

  for (Int_t iTopNode = 0; iTopNode < topNodes->GetEntriesFast(); iTopNode++) {
    TGeoNode* topNode = static_cast<TGeoNode*>(topNodes->At(iTopNode));

    if (TString(topNode->GetName()).Contains("much")) IsMuch = 1;
    if (TString(topNode->GetName()).Contains("trd")) IsTrd = 1;
    if (TString(topNode->GetName()).Contains("tof")) IsTof = 1;
    //if (TString(topNode->GetName()).Contains("sts")) IsSts = 1;
    if (TString(topNode->GetName()).Contains("mvd")) IsMvd = 1;
  }
  */

  fUseMUCH = (fUseMUCH && IsMuch);
  fUseTRD  = fUseTRD && IsTrd;
  fUseTOF  = fUseTOF && IsTof;
  fUseMVD  = fUseMVD && IsMvd;
}


void CbmL1::SetParContainers()
{
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  fDigiPar            = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
  //  fTrdDigiPar = (CbmTrdDigiPar*)(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdDigiPar"));
  fTrdDigiPar = (CbmTrdParSetDigi*) (rtdb->getContainer("CbmTrdParSetDigi"));

  // Trigger loading of STS parameters
  // If L1 runs standalone the parameters are not required by any STS task
  // but they are needed by CbmStsSetup
  fStsParSetModule     = dynamic_cast<CbmStsParSetModule*>(rtdb->getContainer("CbmStsParSetModule"));
  fStsParSetSensor     = dynamic_cast<CbmStsParSetSensor*>(rtdb->getContainer("CbmStsParSetSensor"));
  fStsParSetSensorCond = dynamic_cast<CbmStsParSetSensorCond*>(rtdb->getContainer("CbmStsParSetSensorCond"));

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

  FairRootManager* fManger = FairRootManager::Instance();

  FairRunAna* Run = FairRunAna::Instance();
  {
    fUseMVD                    = 1;
    CbmStsFindTracks* FindTask = L1_DYNAMIC_CAST<CbmStsFindTracks*>(Run->GetTask("STSFindTracks"));
    if (FindTask) fUseMVD = FindTask->MvdUsage();
    // TODO: include of CbmSetup.h creates problems on Mac
    // if (!CbmSetup::Instance()->IsActive(ECbmModuleId::kMvd)) { fUseMVD = false; }
    // N Mvd stations is read from the KF material
    if (CbmKF::Instance()->vMvdMaterial.size() == 0) { fUseMVD = false; }
  }

  fHistoDir = gROOT->mkdir("L1");


  // turn on reconstruction in sub-detectors

  fUseMUCH = 0;
  fUseTRD  = 0;
  fUseTOF  = 0;

  if (fTrackingMode == L1Algo::TrackingMode::kMcbm) {
    fUseMUCH = 1;
    fUseTRD  = 1;
    fUseTOF  = 1;
  }


  if (fTrackingMode == L1Algo::TrackingMode::kGlobal) {
    fUseMUCH = 0;
    fUseTRD  = 1;
    fUseTOF  = 0;
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
    fTimeSlice = (CbmTimeSlice*) fManger->GetObject("TimeSlice.");
    if (fTimeSlice == NULL) LOG(fatal) << GetName() << ": No time slice branch in the tree!";

  }  //? time-slice mode

  else  // event mode
    LOG(info) << GetName() << ": running in event mode.";


  listStsClusters     = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("StsCluster"));
  listStsHitMatch     = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("StsHitMatch"));
  listStsClusterMatch = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("StsClusterMatch"));

  listStsHits = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("StsHit"));

  if (!fUseMUCH) {
    fMuchPixelHits = 0;

    fDigisMuch       = 0;
    fDigiMatchesMuch = 0;
    fClustersMuch    = 0;

    fMuchPoints        = 0;
    listMuchHitMatches = 0;
  }
  else {


    fMuchPixelHits = (TClonesArray*) fManger->GetObject("MuchPixelHit");
  }

  if (!fUseTRD) {
    fTrdPoints     = 0;
    fTrdHitMatches = 0;
    fTrdPoints     = 0;
    listTrdHits    = 0;
  }
  else {

    listTrdHits = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("TrdHit"));
  }

  if (!fUseTOF) {
    fTofPoints         = 0;
    fTofHitDigiMatches = 0;
    fTofHits           = 0;
  }
  else {
    fTofHits = (TClonesArray*) fManger->GetObject("TofHit");
  }

  if (fPerformance) {
    CbmMCDataManager* mcManager = (CbmMCDataManager*) fManger->GetObject("MCDataManager");
    if (NULL == mcManager) LOG(fatal) << GetName() << ": No CbmMCDataManager!";

    fStsPoints = mcManager->InitBranch("StsPoint");

    fMcEventHeader = mcManager->GetObject("MCEventHeader.");

    fMCTracks = mcManager->InitBranch("MCTrack");

    if (NULL == fStsPoints) LOG(fatal) << GetName() << ": No StsPoint data!";
    if (NULL == fMCTracks) LOG(fatal) << GetName() << ": No MCTrack data!";
    if (NULL == fMcEventHeader) LOG(fatal) << GetName() << ": No MC event header data!";

    if (!fLegacyEventMode) {
      fEventList = (CbmMCEventList*) fManger->GetObject("MCEventList.");
      if (NULL == fEventList) LOG(fatal) << GetName() << ": No MCEventList data!";
    }

    if (fUseMVD) {
      fMvdPoints         = mcManager->InitBranch("MvdPoint");
      listMvdDigiMatches = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("MvdDigiMatch"));
      listMvdHitMatches  = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("MvdHitMatch"));
      if (!listMvdHitMatches) { LOG(error) << "No listMvdHitMatches provided, performance is not done correctly"; }
    }

    if (!fUseTRD) {
      fTrdPoints     = 0;
      fTrdHitMatches = 0;
      fTrdPoints     = 0;
    }
    else {
      fTrdHitMatches = (TClonesArray*) fManger->GetObject("TrdHitMatch");
      fTrdPoints     = mcManager->InitBranch("TrdPoint");
    }

    if (!fUseMUCH) {
      fMuchPoints        = 0;
      listMuchHitMatches = 0;
    }
    else {

      fDigisMuch         = (TClonesArray*) fManger->GetObject("MuchDigi");
      fDigiMatchesMuch   = (TClonesArray*) fManger->GetObject("MuchDigiMatch");
      fClustersMuch      = (TClonesArray*) fManger->GetObject("MuchCluster");
      fMuchPoints        = mcManager->InitBranch("MuchPoint");
      listMuchHitMatches = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("MuchPixelHitMatch"));
    }

    if (!fUseTOF) {
      fTofPoints         = 0;
      fTofHitDigiMatches = 0;
    }
    else {
      fTofPoints         = mcManager->InitBranch("TofPoint");
      fTofHitDigiMatches = static_cast<TClonesArray*>(fManger->GetObject("TofHitMatch"));
    }
  }
  else {
  }
  if (!fUseMVD) { listMvdHits = 0; }
  else {
    listMvdHits = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("MvdHit"));
  }

  NMvdStations  = 0;
  NStsStations  = 0;
  NMuchStations = 0;
  NTrdStations  = 0;
  NTOFStation   = 0;
  NStation      = 0;

  // TODO: Replace algo initialization in the constructor (S.Zharko)
  algo = &algo_static;


  L1Vector<fscal> geo("geo");
  geo.reserve(10000);

  {  // initialize field in the target region
    assert(CbmKF::Instance()->vTargets.size() > 0);
    auto& target      = CbmKF::Instance()->vTargets[0];
    algo->fCbmTargetX = target.x;
    algo->fCbmTargetY = target.y;
    algo->fCbmTargetZ = target.z;

    for (int i = 0; i < 3; i++) {
      Double_t point[3] = {0., 0., target.z + 2.5 * i};
      Double_t B[3]     = {0., 0., 0.};
      if (CbmKF::Instance()->GetMagneticField()) { CbmKF::Instance()->GetMagneticField()->GetFieldValue(point, B); }
      geo.push_back(point[2]);
      geo.push_back(B[0]);
      geo.push_back(B[1]);
      geo.push_back(B[2]);
    }
  }


  CbmMuchGeoScheme* fGeoScheme = CbmMuchGeoScheme::Instance();

  if (fUseMUCH) {
    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    TFile* file         = new TFile(fMuchDigiFile, "READ");
    TObjArray* stations = (TObjArray*) file->Get("stations");
    fGeoScheme->Init(stations, 0);
    for (int iStation = 0; iStation < fGeoScheme->GetNStations(); iStation++) {
      const CbmMuchStation* station = fGeoScheme->GetStation(iStation);
      int nLayers                   = station->GetNLayers();
      NMuchStations += nLayers;
    }

    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;
  }

  // count TRD stations
  if (fUseTRD) {
    Int_t layerCounter  = 0;
    TObjArray* topNodes = gGeoManager->GetTopNode()->GetNodes();
    for (Int_t iTopNode = 0; iTopNode < topNodes->GetEntriesFast(); iTopNode++) {
      TGeoNode* topNode = static_cast<TGeoNode*>(topNodes->At(iTopNode));
      if (TString(topNode->GetName()).Contains("trd")) {
        TObjArray* layers = topNode->GetNodes();
        for (Int_t iLayer = 0; iLayer < layers->GetEntriesFast(); iLayer++) {
          TGeoNode* layer = static_cast<TGeoNode*>(layers->At(iLayer));
          if (TString(layer->GetName()).Contains("layer")) { layerCounter++; }
        }
      }
    }
    NTrdStations = layerCounter;

    if ((fTrackingMode == L1Algo::TrackingMode::kMcbm) && (fMissingHits)) { NTrdStations = NTrdStations - 1; }
  }

  vector<float> TofStationZ;
  vector<int> TofStationN;
  if (fUseTOF) {
    NTOFStation = fTofDigiBdfPar->GetNbTrackingStations();
    TofStationZ.resize(NTOFStation, 0);
    TofStationN.resize(NTOFStation, 0);

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
    for (Int_t i = 0; i < NTOFStation; i++)
      TofStationZ[i] = TofStationZ[i] / TofStationN[i];
  }

  CbmStsSetup* stsSetup = CbmStsSetup::Instance();
  if (!stsSetup->IsInit()) { stsSetup->Init(nullptr); }
  if (!stsSetup->IsModuleParsInit()) { stsSetup->SetModuleParameters(fStsParSetModule); }
  if (!stsSetup->IsSensorParsInit()) { stsSetup->SetSensorParameters(fStsParSetSensor); }
  if (!stsSetup->IsSensorCondInit()) { stsSetup->SetSensorConditions(fStsParSetSensorCond); }

  NMvdStations = (fUseMVD) ? CbmKF::Instance()->vMvdMaterial.size() : 0;
  NStsStations = CbmStsSetup::Instance()->GetNofStations();
  NStation     = NMvdStations + NStsStations + NMuchStations + NTrdStations + NTOFStation;


  // field
  const int M = 5;   // polinom order
  const int N = 21;  //(M+1)*(M+2)/2;

  //   { // field at the z=0 plane
  //     const double Xmax = 10, Ymax = 10;
  //     const double z = 0.;
  //     double dx = 1.; // step for the field approximation
  //     double dy = 1.;
  //     if( dx > Xmax/N/2 ) dx = Xmax/N/4.;
  //     if( dy > Ymax/N/2 ) dy = Ymax/N/4.;
  //
  //     TMatrixD A(N,N);
  //     TVectorD b0(N), b1(N), b2(N);
  //     for( int i=0; i<N; i++){
  //       for( int j=0; j<N; j++) A(i,j)==0.;
  //       b0(i)=b1(i)=b2(i) = 0.;
  //     }
  //     for( double x=-Xmax; x<=Xmax; x+=dx )
  //       for( double y=-Ymax; y<=Ymax; y+=dy )
  //     {
  //       double r = sqrt(fabs(x*x/Xmax/Xmax+y/Ymax*y/Ymax));
  //       if( r>1. ) continue;
  //       Double_t w = 1./(r*r+1);
  //       Double_t p[3] = { x, y, z};
  //       Double_t B[3] = {0.,0.,0.};
  //       CbmKF::Instance()->GetMagneticField()->GetFieldValue(p, B);
  //       TVectorD m(N);
  //       m(0)=1;
  //       for( int i=1; i<=M; i++){
  //         int k = (i-1)*(i)/2;
  //         int l = i*(i+1)/2;
  //         for( int j=0; j<i; j++ ) m(l+j) = x*m(k+j);
  //         m(l+i) = y*m(k+i-1);
  //       }
  //
  //       TVectorD mt = m;
  //       for( int i=0; i<N; i++){
  //         for( int j=0; j<N;j++) A(i,j)+=w*m(i)*m(j);
  //         b0(i)+=w*B[0]*m(i);
  //         b1(i)+=w*B[1]*m(i);
  //         b2(i)+=w*B[2]*m(i);
  //       }
  //     }
  //     double det;
  //     A.Invert(&det);
  //     TVectorD c0 = A*b0, c1 = A*b1, c2 = A*b2;
  //
  //     targetFieldSlice = new L1FieldSlice;
  //     for(int i=0; i<N; i++){
  //       targetFieldSlice->cx[i] = c0(i);
  //       targetFieldSlice->cy[i] = c1(i);
  //       targetFieldSlice->cz[i] = c2(i);
  //     }
  //
  //   } // target field

  int NMvdStationsTemp  = NMvdStations;
  int NStsStationsTemp  = NStsStations;
  int NTrdStationsTemp  = NTrdStations;
  int NMuchStationsTemp = NMuchStations;
  int NTOFStationTemp   = NTOFStation;
  int NStationTemp      = NStation;

  vector<int> StationIdxConverter;

  auto& target = CbmKF::Instance()->vTargets[0];

  for (Int_t ist = 0; ist < NStation; ist++) {
    if (ist < NMvdStations) {
      CbmMvdDetector* mvdDetector     = CbmMvdDetector::Instance();
      CbmMvdStationPar* mvdStationPar = mvdDetector->GetParameterFile();
      CbmKFTube& t                    = CbmKF::Instance()->vMvdMaterial[ist];

      if (target.z < t.z) {
        StationIdxConverter.push_back(ist);
        StationIdxReverseConverter.push_back(StationIdxConverter.size() - 1);
      }
      else {
        NMvdStationsTemp--;
        NStationTemp--;
        StationIdxReverseConverter.push_back(-1);
      }
    }

    if (ist >= NMvdStations && ist < (NMvdStations + NStsStations)) {
      CbmStsStation* station = CbmStsSetup::Instance()->GetStation(ist - NMvdStations);
      if (target.z < station->GetZ()) {
        StationIdxConverter.push_back(ist);
        StationIdxReverseConverter.push_back(StationIdxConverter.size() - 1);
      }
      else {
        NStsStationsTemp--;
        NStationTemp--;
        StationIdxReverseConverter.push_back(-1);
      }
    }

    if ((ist < (NMvdStations + NStsStations + NMuchStations)) && (ist >= (NMvdStations + NStsStations))) {

      int iStation            = (ist - NMvdStations - NStsStations) / 3;
      CbmMuchStation* station = (CbmMuchStation*) fGeoScheme->GetStation(iStation);
      CbmMuchLayer* layer     = station->GetLayer((ist - NMvdStations - NStsStations) % 3);
      if (target.z < layer->GetZ()) {
        StationIdxConverter.push_back(ist);
        StationIdxReverseConverter.push_back(StationIdxConverter.size() - 1);
      }
      else {
        NTrdStationsTemp--;
        NStationTemp--;
        StationIdxReverseConverter.push_back(-1);
      }
    }

    if ((ist < (NMvdStations + NStsStations + NTrdStations + NMuchStations))
        && (ist >= (NMvdStations + NStsStations + NMuchStations))) {

      int num  = ist - NMvdStations - NStsStations - NMuchStations;
      int skip = num;

      if ((fTrackingMode == L1Algo::TrackingMode::kMcbm) && (fMissingHits))
        if (num > 0) skip++;
      int ModuleId = fTrdDigiPar->GetModuleId(skip);

      CbmTrdParModDigi* module = (CbmTrdParModDigi*) fTrdDigiPar->GetModulePar(ModuleId);

      if (target.z < module->GetZ()) {
        StationIdxConverter.push_back(ist);
        StationIdxReverseConverter.push_back(StationIdxConverter.size() - 1);
      }
      else {
        NMuchStationsTemp--;
        NStationTemp--;
        StationIdxReverseConverter.push_back(-1);
      }
    }

    if ((ist < (NMvdStations + NStsStations + NTrdStations + NMuchStations + NTOFStation))
        && (ist >= (NMvdStations + NStsStations + NMuchStations + NTrdStations))) {

      int num = ist - (NMvdStations + NStsStations + NTrdStations + NMuchStations);

      if (target.z < TofStationZ[num]) {
        StationIdxConverter.push_back(ist);
        StationIdxReverseConverter.push_back(StationIdxConverter.size() - 1);
      }
      else {
        NTOFStationTemp--;
        NStationTemp--;
        StationIdxReverseConverter.push_back(-1);
      }
    }
  }

  NMvdStationsGeom  = NMvdStations;
  NStsStationsGeom  = NStsStations;
  NTrdStationsGeom  = NTrdStations;
  NMuchStationsGeom = NMuchStations;
  NTOFStationGeom   = NTOFStation;
  NStationGeom      = NStation;

  NMvdStations  = NMvdStationsTemp;
  NStsStations  = NStsStationsTemp;
  NTrdStations  = NTrdStationsTemp;
  NMuchStations = NMuchStationsTemp;
  NTOFStation   = NTOFStationTemp;
  NStation      = NStationTemp;

  geo.push_back(NStation);
  geo.push_back(NMvdStations);
  geo.push_back(NStsStations);


  for (Int_t ist = 0; ist < NStation; ist++) {

    double z    = 0;
    double Xmax = 0, Ymax = 0;
    if (ist < NMvdStations) {

      CbmMvdDetector* mvdDetector     = CbmMvdDetector::Instance();
      CbmMvdStationPar* mvdStationPar = mvdDetector->GetParameterFile();

      CbmKFTube& t = CbmKF::Instance()->vMvdMaterial[StationIdxConverter[ist]];
      geo.push_back(1);
      geo.push_back(t.z);
      geo.push_back(t.dz);
      geo.push_back(t.r);
      geo.push_back(t.R);
      geo.push_back(t.RadLength);

      fscal f_phi = 0, f_sigma = mvdStationPar->GetXRes(ist) / 10000, b_phi = 3.14159265358 / 2.,
            b_sigma = mvdStationPar->GetYRes(ist) / 10000, dt = 1000;
      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);
      geo.push_back(dt);
      z    = t.z;
      Xmax = Ymax = t.R;

      LOG(info) << "L1: Mvd station " << ist << " at z " << t.z;
    }


    if (ist >= NMvdStations && ist < (NMvdStations + NStsStations)) {

      CbmStsStation* station = CbmStsSetup::Instance()->GetStation(StationIdxConverter[ist] - NMvdStationsGeom);
      geo.push_back(0);
      geo.push_back(station->GetZ());
      geo.push_back(station->GetSensorD());
      geo.push_back(0);
      geo.push_back(station->GetYmax() < station->GetXmax() ? station->GetXmax() : station->GetYmax());
      geo.push_back(station->GetRadLength());

      double Pi = 3.14159265358;

      fscal f_phi, f_sigma, b_phi, b_sigma;  // angle and sigma front/back  side
      f_phi = station->GetSensorRotation();
      b_phi = f_phi;
      f_phi += station->GetSensorStereoAngle(0) * Pi / 180.;
      b_phi += station->GetSensorStereoAngle(1) * Pi / 180.;
      f_sigma  = station->GetSensorPitch(0) / TMath::Sqrt(12);
      b_sigma  = f_sigma;
      fscal dt = 5;
      //f_sigma *= cos(f_phi);  // TODO: think about this
      //b_sigma *= cos(b_phi);

      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);
      geo.push_back(dt);
      z = station->GetZ();

      Xmax = station->GetXmax();
      Ymax = station->GetYmax();

      LOG(info) << "L1: Sts station " << ist - NMvdStations << " at z " << station->GetZ();
    }

    if ((ist < (NMvdStations + NStsStations + NMuchStations)) && (ist >= (NMvdStations + NStsStations))) {

      int iStation = (StationIdxConverter[ist] - NMvdStationsGeom - NStsStationsGeom) / 3;


      CbmMuchStation* station = (CbmMuchStation*) fGeoScheme->GetStation(iStation);

      CbmMuchLayer* layer = station->GetLayer((StationIdxConverter[ist] - NMvdStationsGeom - NStsStationsGeom) % 3);

      z = layer->GetZ();

      geo.push_back(2);
      geo.push_back(z);
      geo.push_back(layer->GetDz());
      geo.push_back(10);
      geo.push_back(100);  //station->GetRmax()
      geo.push_back(0);

      // fscal f_phi = 0, f_sigma = 0.1, b_phi = 3.14159265358 / 2., b_sigma = 0.1;
      fscal f_phi = 0, f_sigma = 0.35, b_phi = 3.14159265358 / 2., b_sigma = 0.35, dt = 3.9;
      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);
      geo.push_back(dt);


      Xmax = 100;  //station->GetRmax();
      Ymax = 100;  //station->GetRmax();

      LOG(info) << "L1: Much station " << iStation << " at z " << z << endl;
    }

    //     int num = 0;

    if ((ist < (NMvdStations + NStsStations + NTrdStations + NMuchStations))
        && (ist >= (NMvdStations + NStsStations + NMuchStations))) {

      int num = StationIdxConverter[ist] - NMvdStationsGeom - NStsStationsGeom - NMuchStationsGeom;

      int skip = num;

      if ((fTrackingMode == L1Algo::TrackingMode::kMcbm) && (fMissingHits))
        if (num > 0) skip++;
      int ModuleId = fTrdDigiPar->GetModuleId(skip);

      CbmTrdParModDigi* module = (CbmTrdParModDigi*) fTrdDigiPar->GetModulePar(ModuleId);

      //   if (!true_station[ist]) continue;

      if (num == 0 || num == 2 || num == 4) geo.push_back(3);
      if (num == 1 || num == 3) geo.push_back(6);

      float stationZ = module->GetZ();

      geo.push_back(stationZ);

      geo.push_back(2 * module->GetSizeZ());
      geo.push_back(0);
      geo.push_back(2 * module->GetSizeX());
      geo.push_back(1.6);

      //  fscal f_phi = 0, f_sigma = 1., b_phi = 3.14159265358 / 2., b_sigma = 1.;
      fscal f_phi = 0, f_sigma = 0.15, b_phi = 3.14159265358 / 2., b_sigma = 0.15, dt = 10;
      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);
      geo.push_back(dt);
      Xmax = module->GetSizeX();
      Ymax = module->GetSizeY();
      LOG(info) << "L1: Trd station " << num << " at z " << stationZ << endl;
    }


    if ((ist < (NMvdStations + NStsStations + NTrdStations + NMuchStations + NTOFStation))
        && (ist >= (NMvdStations + NStsStations + NMuchStations + NTrdStations))) {

      geo.push_back(4);

      int num = StationIdxConverter[ist] - (NMvdStationsGeom + NStsStationsGeom + NTrdStationsGeom + NMuchStationsGeom);

      geo.push_back(TofStationZ[num]);

      geo.push_back(10);   /// materialInfo.thick
      geo.push_back(0);    /// Rmin
      geo.push_back(150);  /// Rmax
      geo.push_back(2);    /// materialInfo.RL

      // fscal f_phi = 0, f_sigma = 1 / 10000, b_phi = 3.14159265358 / 2., b_sigma = 1 / 10000;
      fscal f_phi = 0, f_sigma = 0.42, b_phi = 3.14159265358 / 2., b_sigma = 0.23, dt = 0.075;
      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);
      geo.push_back(dt);
      Xmax = Ymax = 20;
      LOG(info) << "L1: Tof station " << num << " at z " << z << endl;
    }


    double dx = 1.;  // step for the field approximation
    double dy = 1.;

    if (dx > Xmax / N / 2) dx = Xmax / N / 4.;
    if (dy > Ymax / N / 2) dy = Ymax / N / 4.;

    double C[3][N];
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


    if (CbmKF::Instance()->GetMagneticField()) {
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
    }

    geo.push_back(N);
    for (int k = 0; k < 3; k++) {
      for (int j = 0; j < N; j++) {
        geo.push_back(C[k][j]);
      }
    }
  }

  // TODO: replace these values to L1Parameters (S.Zh.)
  geo.push_back(fTrackingLevel);
  geo.push_back(fMomentumCutOff);
  geo.push_back(fGhostSuppression);

  {
    if (fSTAPDataMode % 2 == 1) {  // 1,3
      WriteSTAPGeoData(geo);
    };
    //if(fSTAPDataMode >= 2){ // 2,3
    //  int ind2, ind = geo.size();
    //  ReadSTAPGeoData(geo, ind2);
    //  if (ind2 != ind)  LOG(error) << "-E- CbmL1: Read geometry from file " << fSTAPDataDir + "geo_algo.txt was NOT successful.";
    //};
  }

  if (fSTAPDataMode >= 2) {  // 2,3
    int ind2, ind = geo.size();
    ReadSTAPGeoData(geo, ind2);
    if (ind2 != ind)
      LOG(error) << "-E- CbmL1: Read geometry from file " << fSTAPDataDir + "geo_algo.txt was NOT successful.";
  }

  //algo->SetParameters(fParameters);


  /********************************************************************************************************************
   *                     EXPERIMENTAL FEATURE: usage of L1InitManager for L1Algo initialization                       *
   ********************************************************************************************************************/
  {  //L1Algo new init start

    // Step 1: Initialize magnetic field function
    // Set magnetic field slices
    auto fieldGetterFcn = [](const double(&inPos)[3], double(&outB)[3]) {
      CbmKF::Instance()->GetMagneticField()->GetFieldValue(inPos, outB);
    };
    fpInitManager->SetFieldFunction(fieldGetterFcn);

    // Step 2: Initialize target
    auto& target = CbmKF::Instance()->vTargets[0];
    fpInitManager->SetTargetPosition(target.x, target.y, target.z);

    // Step 3: Initialize primary vertex field
    fpInitManager->InitTargetField(2.5);


    // Step 4: initialize IDs of detectors active in tracking
    // TODO: temporary for tests, must be initialized somewhere in run_reco.C or similar (S.Zh.)
    fActiveTrackingDetectorIDs = {L1DetectorID::kMvd, L1DetectorID::kSts};
    fpInitManager->SetActiveDetectorIDs(fActiveTrackingDetectorIDs);

    constexpr double PI = 3.14159265358;  // TODO: why cmath is not used? (S.Zh.)

    // Step 5: initialize number of stations for each detector ID
    fpInitManager->SetStationsNumberCrosscheck(L1DetectorID::kMvd, NMvdStations);
    fpInitManager->SetStationsNumberCrosscheck(L1DetectorID::kSts, NStsStations);
    fpInitManager->SetStationsNumberCrosscheck(L1DetectorID::kMuch, NMuchStations);
    fpInitManager->SetStationsNumberCrosscheck(L1DetectorID::kTrd, NTrdStations);
    fpInitManager->SetStationsNumberCrosscheck(L1DetectorID::kTof, NTOFStation);

    // Step 6: setup station info

    /*** Setup MVD stations info ***/
    for (int iSt = 0; iSt < NMvdStations; ++iSt) {  // NOTE: example using in-stack defined objects
      CbmMvdDetector* mvdDetector     = CbmMvdDetector::Instance();
      CbmMvdStationPar* mvdStationPar = mvdDetector->GetParameterFile();

      CbmKFTube& t     = CbmKF::Instance()->vMvdMaterial[iSt];
      auto stationInfo = L1BaseStationInfo(L1DetectorID::kMvd, iSt);
      stationInfo.SetStationType(1);
      // MVD // TODO: to be exchanged with specific flags (timeInfo, fieldInfo etc.) (S.Zh.)
      stationInfo.SetTimeInfo(0);
      stationInfo.SetZ(t.z);
      stationInfo.SetMaterial(t.dz, t.RadLength);
      stationInfo.SetXmax(t.R);
      stationInfo.SetYmax(t.R);
      stationInfo.SetRmin(t.r);
      stationInfo.SetRmax(t.R);
      fscal mvdFrontPhi   = 0;
      fscal mvdBackPhi    = PI / 2.;
      fscal mvdFrontSigma = mvdStationPar->GetXRes(iSt) / 10000;
      fscal mvdBackSigma  = mvdStationPar->GetYRes(iSt) / 10000;
      stationInfo.SetFrontBackStripsGeometry(mvdFrontPhi, mvdFrontSigma, mvdBackPhi, mvdBackSigma);
      fpInitManager->AddStation(stationInfo);
    }

    // Setup STS stations info
    for (int iSt = 0; iSt < NStsStations; ++iSt) {  // NOTE: example using smart pointers
      auto cbmSts = CbmStsSetup::Instance()->GetStation(iSt);
      std::unique_ptr<L1BaseStationInfo> stationInfo(new L1BaseStationInfo(L1DetectorID::kSts, iSt));
      // TODO: replace with std::make_unique, when C++14 is avaliable!!!! (S.Zh.)
      // auto stsStation = std::make_unique<L1BaseStationInfo>(L1DetectorID::kSts, iSt);
      stationInfo->SetStationType(0);  // STS
      stationInfo->SetTimeInfo(1);

      // Setup station geometry and material
      stationInfo->SetZ(cbmSts->GetZ());
      double stsXmax = cbmSts->GetXmax();
      double stsYmax = cbmSts->GetYmax();
      stationInfo->SetXmax(stsXmax);
      stationInfo->SetYmax(stsYmax);
      stationInfo->SetRmin(0);
      stationInfo->SetRmax(stsXmax > stsYmax ? stsXmax : stsYmax);
      stationInfo->SetMaterial(cbmSts->GetSensorD(), cbmSts->GetRadLength());

      // Setup strips geometry
      fscal stsFrontPhi   = cbmSts->GetSensorRotation() + cbmSts->GetSensorStereoAngle(0) * PI / 180.;
      fscal stsBackPhi    = cbmSts->GetSensorRotation() + cbmSts->GetSensorStereoAngle(1) * PI / 180.;
      fscal stsFrontSigma = cbmSts->GetSensorPitch(0) / sqrt(12);
      fscal stsBackSigma  = stsFrontSigma;
      stationInfo->SetFrontBackStripsGeometry(stsFrontPhi, stsFrontSigma, stsBackPhi, stsBackSigma);
      fpInitManager->AddStation(stationInfo);
    }

    // Setup MuCh stations info
    for (int iSt = 0; iSt < NMuchStations; ++iSt) {
      int muchStationID       = iSt / 3;
      int muchLayerID         = iSt % 3;
      CbmMuchStation* station = (CbmMuchStation*) fGeoScheme->GetStation(muchStationID);
      CbmMuchLayer* layer     = station->GetLayer(muchLayerID);

      auto stationInfo = L1BaseStationInfo(L1DetectorID::kMuch, iSt);
      stationInfo.SetStationType(2);
      // MVD // TODO: to be exchanged with specific flags (timeInfo, fieldInfo etc.) (S.Zh.)
      stationInfo.SetTimeInfo(1);
      stationInfo.SetZ(layer->GetZ());
      stationInfo.SetMaterial(layer->GetDz(), 10);  // TODO: Why rad len is 0????? (S.Zh.)
      stationInfo.SetXmax(100.);
      stationInfo.SetYmax(100.);
      stationInfo.SetRmin(0.);
      stationInfo.SetRmax(100.);
      fscal muchFrontPhi   = 0;
      fscal muchBackPhi    = PI / 2.;
      fscal muchFrontSigma = 0.1;
      fscal muchBackSigma  = 0.1;
      stationInfo.SetFrontBackStripsGeometry(muchFrontPhi, muchFrontSigma, muchBackPhi, muchBackSigma);
      fpInitManager->AddStation(stationInfo);
    }

    // Setup TRD stations info
    for (int iSt = 0; iSt < NTrdStations; ++iSt) {
      int skip = iSt;  // temporary solution to remove TRD with id == 1 wrom mCBM
      if ((fTrackingMode == L1Algo::TrackingMode::kMcbm) && (fMissingHits)) {
        if (iSt > 0) { skip++; }
      }
      int trdModuleID          = fTrdDigiPar->GetModuleId(skip);
      CbmTrdParModDigi* module = (CbmTrdParModDigi*) fTrdDigiPar->GetModulePar(trdModuleID);
      auto stationInfo         = L1BaseStationInfo(L1DetectorID::kTrd, skip);
      int stationType          = (iSt == 1 || iSt == 3) ? 6 : 3;
      stationInfo.SetStationType(stationType);
      stationInfo.SetTimeInfo(1);
      stationInfo.SetZ(module->GetZ());
      stationInfo.SetMaterial(2. * module->GetSizeZ(), 10.);
      stationInfo.SetXmax(module->GetSizeX());
      stationInfo.SetYmax(module->GetSizeY());
      stationInfo.SetRmin(0.);
      stationInfo.SetRmax(2. * module->GetSizeX());  // TODO: Why multiplied with 2.?
      fscal trdFrontPhi   = 0;
      fscal trdBackPhi    = PI / 2.;
      fscal trdFrontSigma = 1.;
      fscal trdBackSigma  = 1.;
      stationInfo.SetFrontBackStripsGeometry(trdFrontPhi, trdFrontSigma, trdBackPhi, trdBackSigma);
      fpInitManager->AddStation(stationInfo);
    }

    // Setup TOF stations info
    for (int iSt = 0; iSt < NTOFStation; ++iSt) {
      auto stationInfo = L1BaseStationInfo(L1DetectorID::kTof, iSt);
      stationInfo.SetStationType(4);
      stationInfo.SetTimeInfo(1);
      stationInfo.SetZ(TofStationZ[iSt]);
      stationInfo.SetMaterial(10., 10.);  // TODO: add Tof width dz and rad. len
      stationInfo.SetXmax(20.);
      stationInfo.SetYmax(20.);
      stationInfo.SetRmin(0.);
      stationInfo.SetRmax(150.);
      fscal tofFrontPhi   = 0;
      fscal tofBackPhi    = PI / 2.;
      fscal tofFrontSigma = 1.;
      fscal tofBackSigma  = 1.;
      stationInfo.SetFrontBackStripsGeometry(tofFrontPhi, tofFrontSigma, tofBackPhi, tofBackSigma);
      fpInitManager->AddStation(stationInfo);
    }
    //initMan->PrintStations(/*vebosity = */ 1);

    // Step 7: initialize iterations and form a vector of them
    {
      // TODO: Possible, it will be more convenient to create a vector of these iterations, access its elements
      //       by the enumeration item
      // TODO: Need to provide a selection: default iterations input (these hardcoded ones), input from file or input
      //       from runing macro
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
      trackingIterAllSecE.SetTargetPosSigmaXY(10, 10);

      // Select default track finder
      if (fTrackingMode == L1Algo::TrackingMode::kMcbm) {
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

      // Set special cuts
    }
  }  // L1Algo new init: end
  /********************************************************************************************************************
   ********************************************************************************************************************/

  algo->Init(geo, fUseHitErrors, fTrackingMode, fMissingHits);
  geo.clear();

  algo->fRadThick.reset(algo->NStations);

  // Read STS  MVD TRD MuCh ToF Radiation Thickness table
  TString stationName = "Radiation Thickness [%], Station";
  if (fUseMVD) {
    if (fMvdMatBudgetFileName != "") {
      /// Save old global file and folder pointer to avoid messing with FairRoot
      TFile* oldFile     = gFile;
      TDirectory* oldDir = gDirectory;
      TFile* rlFile      = new TFile(fMvdMatBudgetFileName);
      cout << "MVD Material budget file is " << fMvdMatBudgetFileName << ".\n";
      for (int j = 0, iSta = 0; iSta < algo->NMvdStations; iSta++, j++) {
        TString stationNameMvd = stationName;
        stationNameMvd += j;
        TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameMvd);
        if (!hStaRadLen) {
          cout << "L1: incorrect " << fMvdMatBudgetFileName << " file. No " << stationNameMvd << "\n";
          exit(1);
        }
        const int NBins  = hStaRadLen->GetNbinsX();            // should be same in Y
        const float RMax = hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
        algo->fRadThick[iSta].SetBins(NBins, RMax);
        algo->fRadThick[iSta].table.resize(NBins);
        double sumRF = 0., nRF = 0.;
        for (int iB = 0; iB < NBins; iB++) {
          algo->fRadThick[iSta].table[iB].resize(NBins);
          for (int iB2 = 0; iB2 < NBins; iB2++) {
            algo->fRadThick[iSta].table[iB][iB2] = 0.01 * hStaRadLen->GetBinContent(iB, iB2);
            // Correction for holes in material map
            if (algo->fRadThick[iSta].table[iB][iB2] < algo->vStations[iSta].materialInfo.RadThick[0])
              if (iB2 > 0 && iB2 < NBins - 1)
                algo->fRadThick[iSta].table[iB][iB2] = TMath::Min(0.01 * hStaRadLen->GetBinContent(iB, iB2 - 1),
                                                                  0.01 * hStaRadLen->GetBinContent(iB, iB2 + 1));
            // Correction for the incorrect harcoded value of RadThick of MVD stations
            if (algo->fRadThick[iSta].table[iB][iB2] < 0.0015) algo->fRadThick[iSta].table[iB][iB2] = 0.0015;
            //              algo->fRadThick[iSta].table[iB][iB2] = algo->vStations[iSta].materialInfo.RadThick[0];
            sumRF += algo->fRadThick[iSta].table[iB][iB2];
            nRF++;
          }
        }
        cout << " iSta " << iSta << " average RadThick in the material map " << sumRF / nRF << endl;
      }
      rlFile->Close();
      rlFile->Delete();
      /// Restore old global file and folder pointer to avoid messing with FairRoot
      gFile      = oldFile;
      gDirectory = oldDir;
    }
    else {
      LOG(warn) << "No MVD material budget file is found. Homogenious budget "
                   "will be used";
      for (int iSta = 0; iSta < algo->NMvdStations; iSta++) {
        algo->fRadThick[iSta].SetBins(1,
                                      100);  // mvd should be in +-100 cm square
        algo->fRadThick[iSta].table.resize(1);
        algo->fRadThick[iSta].table[0].resize(1);
        algo->fRadThick[iSta].table[0][0] = algo->vStations[iSta].materialInfo.RadThick[0];
      }
    }
  }
  if (fStsMatBudgetFileName != "") {
    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;
    TFile* rlFile      = new TFile(fStsMatBudgetFileName);
    cout << "STS Material budget file is " << fStsMatBudgetFileName << ".\n";
    for (int j = 1, iSta = algo->NMvdStations; iSta < (algo->NMvdStations + NStsStations); iSta++, j++) {
      TString stationNameSts = stationName;
      stationNameSts += j;
      TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameSts);
      if (!hStaRadLen) {
        cout << "L1: incorrect " << fStsMatBudgetFileName << " file. No " << stationNameSts << "\n";
        exit(1);
      }
      const int NBins  = hStaRadLen->GetNbinsX();            // should be same in Y
      const float RMax = hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
      algo->fRadThick[iSta].SetBins(NBins, RMax);
      algo->fRadThick[iSta].table.resize(NBins);
      double sumRF = 0., nRF = 0.;
      for (int iB = 0; iB < NBins; iB++) {
        algo->fRadThick[iSta].table[iB].resize(NBins);
        for (int iB2 = 0; iB2 < NBins; iB2++) {
          algo->fRadThick[iSta].table[iB][iB2] = 0.01 * hStaRadLen->GetBinContent(iB, iB2);
          if (algo->fRadThick[iSta].table[iB][iB2] < algo->vStations[iSta].materialInfo.RadThick[0])
            algo->fRadThick[iSta].table[iB][iB2] = algo->vStations[iSta].materialInfo.RadThick[0];
          // cout << " iSta " << iSta << " iB " << iB << "iB2 " << iB2 << " RadThick " << algo->fRadThick[iSta].table[iB][iB2] << endl;
          sumRF += algo->fRadThick[iSta].table[iB][iB2];
          nRF++;
        }
      }
      cout << " iSta " << iSta << " average RadThick in the material map " << sumRF / nRF << endl;
    }
    rlFile->Close();
    rlFile->Delete();
    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;
  }
  else {
    LOG(warn) << "No STS material budget file is found. Homogenious budget "
                 "will be used";
    for (int iSta = algo->NMvdStations; iSta < (algo->NMvdStations + NStsStations); iSta++) {
      algo->fRadThick[iSta].SetBins(1, 100);
      algo->fRadThick[iSta].table.resize(1);
      algo->fRadThick[iSta].table[0].resize(1);
      algo->fRadThick[iSta].table[0][0] = algo->vStations[iSta].materialInfo.RadThick[0];
    }
  }

  if (fUseMUCH)
    if (fMuchMatBudgetFileName != "") {
      /// Save old global file and folder pointer to avoid messing with FairRoot
      TFile* oldFile     = gFile;
      TDirectory* oldDir = gDirectory;
      TFile* rlFile      = new TFile(fMuchMatBudgetFileName);
      cout << "Much Material budget file is " << fMuchMatBudgetFileName << ".\n";
      for (int j = 1, iSta = (NStsStations + NMvdStations); iSta < (NStsStations + NMvdStations + NMuchStations);
           iSta++, j++) {
        TString stationNameSts = stationName;
        stationNameSts += j;
        TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameSts);
        if (!hStaRadLen) {
          cout << "L1: incorrect " << fMuchMatBudgetFileName << " file. No " << stationNameSts << "\n";
          exit(1);
        }


        const int NBins  = hStaRadLen->GetNbinsX();            // should be same in Y
        const float RMax = hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
        algo->fRadThick[iSta].SetBins(NBins, RMax);
        algo->fRadThick[iSta].table.resize(NBins);

        float hole = 0.15;

        for (int iB = 0; iB < NBins; iB++) {
          algo->fRadThick[iSta].table[iB].resize(NBins);
          for (int iB2 = 0; iB2 < NBins; iB2++) {
            algo->fRadThick[iSta].table[iB][iB2] = 0.01 * hStaRadLen->GetBinContent(iB, iB2);
            // Correction for holes in material map
            //if(algo->fRadThick[iSta].table[iB][iB2] < algo->vStations[iSta].materialInfo.RadThick[0])

            if (iB2 > 0 && iB2 < NBins - 1)
              algo->fRadThick[iSta].table[iB][iB2] = TMath::Min(0.01 * hStaRadLen->GetBinContent(iB, iB2 - 1),
                                                                0.01 * hStaRadLen->GetBinContent(iB, iB2 + 1));
            // Correction for the incorrect harcoded value of RadThick of MVD stations

            if (algo->fRadThick[iSta].table[iB][iB2] > 0.0015) hole = algo->fRadThick[iSta].table[iB][iB2];
            if (algo->fRadThick[iSta].table[iB][iB2] < 0.0015) algo->fRadThick[iSta].table[iB][iB2] = hole;
            //              algo->fRadThick[iSta].table[iB][iB2] = algo->vStations[iSta].materialInfo.RadThick[0];
          }
        }
      }
      rlFile->Close();
      rlFile->Delete();
      /// Restore old global file and folder pointer to avoid messing with FairRoot
      gFile      = oldFile;
      gDirectory = oldDir;
    }
    else {
      LOG(warn) << "No Much material budget file is found. Homogenious budget "
                   "will be used";
      for (int iSta = (NStsStations + NMvdStations); iSta < (NStsStations + NMvdStations + NMuchStations); iSta++) {
        algo->fRadThick[iSta].SetBins(1, 100);
        algo->fRadThick[iSta].table.resize(1);
        algo->fRadThick[iSta].table[0].resize(1);
        algo->fRadThick[iSta].table[0][0] = algo->vStations[iSta].materialInfo.RadThick[0];
      }
    }

  if (fUseTRD)
    if (fTrdMatBudgetFileName != "") {
      /// Save old global file and folder pointer to avoid messing with FairRoot
      TFile* oldFile     = gFile;
      TDirectory* oldDir = gDirectory;
      TFile* rlFile      = new TFile(fTrdMatBudgetFileName);
      cout << "TRD Material budget file is " << fTrdMatBudgetFileName << ".\n";
      for (int j = 1, iSta = (NStsStations + NMvdStations + NMuchStations);
           iSta < (NStsStations + NMvdStations + NMuchStations + NTrdStations); iSta++, j++) {
        TString stationNameSts = stationName;
        int skipStation        = j;
        if (fMissingHits)
          if (skipStation == 2) skipStation = 3;
        if (fMissingHits)
          if (skipStation == 3) skipStation = 4;
        stationNameSts += skipStation;
        TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameSts);
        if (!hStaRadLen) {
          cout << "L1: incorrect " << fTrdMatBudgetFileName << " file. No " << stationNameSts << "\n";
          exit(1);
        }


        const int NBins  = hStaRadLen->GetNbinsX();            // should be same in Y
        const float RMax = hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
        algo->fRadThick[iSta].SetBins(NBins, RMax);
        algo->fRadThick[iSta].table.resize(NBins);
        float hole = 0.15;

        for (int iB = 0; iB < NBins; iB++) {
          algo->fRadThick[iSta].table[iB].resize(NBins);
          for (int iB2 = 0; iB2 < NBins; iB2++) {
            algo->fRadThick[iSta].table[iB][iB2] = 0.01 * hStaRadLen->GetBinContent(iB, iB2);
            // Correction for holes in material map
            //if(algo->fRadThick[iSta].table[iB][iB2] < algo->vStations[iSta].materialInfo.RadThick[0])

            if (iB2 > 0 && iB2 < NBins - 1)
              algo->fRadThick[iSta].table[iB][iB2] = TMath::Min(0.01 * hStaRadLen->GetBinContent(iB, iB2 - 1),
                                                                0.01 * hStaRadLen->GetBinContent(iB, iB2 + 1));
            // Correction for the incorrect harcoded value of RadThick of MVD stations

            if (algo->fRadThick[iSta].table[iB][iB2] > 0.0015) hole = algo->fRadThick[iSta].table[iB][iB2];
            if (algo->fRadThick[iSta].table[iB][iB2] < 0.0015) algo->fRadThick[iSta].table[iB][iB2] = hole;
            //              algo->fRadThick[iSta].table[iB][iB2] = algo->vStations[iSta].materialInfo.RadThick[0];
          }
        }
      }
      rlFile->Close();
      rlFile->Delete();
      /// Restore old global file and folder pointer to avoid messing with FairRoot
      gFile      = oldFile;
      gDirectory = oldDir;
    }
    else {
      LOG(warn) << "No TRD material budget file is found. Homogenious budget "
                   "will be used";
      for (int iSta = (NStsStations + NMvdStations + NMuchStations);
           iSta < (NStsStations + NMvdStations + NMuchStations + NTrdStations); iSta++) {
        algo->fRadThick[iSta].SetBins(1, 100);
        algo->fRadThick[iSta].table.resize(1);
        algo->fRadThick[iSta].table[0].resize(1);
        algo->fRadThick[iSta].table[0][0] = algo->vStations[iSta].materialInfo.RadThick[0];
        cout << "TRD material: " << algo->vStations[iSta].materialInfo.RadThick[0] << endl;
      }
    }

  if (fUseTOF)
    if (fTofMatBudgetFileName != "") {
      /// Save old global file and folder pointer to avoid messing with FairRoot
      TFile* oldFile     = gFile;
      TDirectory* oldDir = gDirectory;
      TFile* rlFile      = new TFile(fTofMatBudgetFileName);
      cout << "TOF Material budget file is " << fTofMatBudgetFileName << ".\n";
      for (int j = 1, iSta = (NStsStations + NMvdStations + NMuchStations + NTrdStations);
           iSta < (NStsStations + NMvdStations + NMuchStations + NTrdStations + NTOFStation); iSta++, j++) {
        TString stationNameSts = stationName;
        stationNameSts += j;
        TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameSts);
        if (!hStaRadLen) {
          cout << "L1: incorrect " << fTofMatBudgetFileName << " file. No " << stationNameSts << "\n";
          exit(1);
        }


        const int NBins  = hStaRadLen->GetNbinsX();            // should be same in Y
        const float RMax = hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
        algo->fRadThick[iSta].SetBins(NBins, RMax);
        algo->fRadThick[iSta].table.resize(NBins);

        for (int iB = 0; iB < NBins; iB++) {
          algo->fRadThick[iSta].table[iB].resize(NBins);
          float hole = 0.0015;
          for (int iB2 = 0; iB2 < NBins; iB2++) {
            algo->fRadThick[iSta].table[iB][iB2] = 0.01 * hStaRadLen->GetBinContent(iB, iB2);
            // Correction for holes in material map
            //if(algo->fRadThick[iSta].table[iB][iB2] < algo->vStations[iSta].materialInfo.RadThick[0])

            if (iB2 > 0 && iB2 < NBins - 1)
              algo->fRadThick[iSta].table[iB][iB2] = TMath::Min(0.01 * hStaRadLen->GetBinContent(iB, iB2 - 1),
                                                                0.01 * hStaRadLen->GetBinContent(iB, iB2 + 1));
            // Correction for the incorrect harcoded value of RadThick of MVD stations

            if (algo->fRadThick[iSta].table[iB][iB2] > 0.0015) hole = algo->fRadThick[iSta].table[iB][iB2];
            if (algo->fRadThick[iSta].table[iB][iB2] < 0.0015) algo->fRadThick[iSta].table[iB][iB2] = hole;
            //              algo->fRadThick[iSta].table[iB][iB2] = algo->vStations[iSta].materialInfo.RadThick[0];
          }
        }
      }
      rlFile->Close();
      rlFile->Delete();
      /// Restore old global file and folder pointer to avoid messing with FairRoot
      gFile      = oldFile;
      gDirectory = oldDir;
    }
    else {
      LOG(warn) << "No TOF material budget file is found. Homogenious budget "
                   "will be used";
      for (int iSta = (NStsStations + NMvdStations + NMuchStations + NTrdStations);
           iSta < (NStsStations + NMvdStations + NMuchStations + NTrdStations + NTOFStation); iSta++) {
        algo->fRadThick[iSta].SetBins(1, 100);
        algo->fRadThick[iSta].table.resize(1);
        algo->fRadThick[iSta].table[0].resize(1);
        algo->fRadThick[iSta].table[0][0] = algo->vStations[iSta].materialInfo.RadThick[0];
      }
    }
  return kSUCCESS;
}


void CbmL1::Exec(Option_t* /*option*/) {}

void CbmL1::Reconstruct(CbmEvent* event)
{
  fVerbose = 11; // TODO: Remove it (tmp)! (S.Zharko)
  static int nevent = 0;
  vFileEvent.clear();

  L1Vector<std::pair<double, int>> SortStsHits("CbmL1::SortStsHits");
  SortStsHits.reserve(listStsHits->GetEntriesFast());

  float start_t = 10000000000;

  // TODO: move these values to CbmL1Parameters namespace (S.Zharko)
  bool areDataLeft = true;   // whole TS processed?
  float TsStart    = 0;      // starting time of sub-TS
  float TsLength   = 10000;  // length of sub-TS
  float TsOverlap  = 15;     // min length of overlap region
  int FstHitinTs   = 0;      // 1st hit index in TS

  /// sort input hits by time
  for (Int_t j = 0; j < listStsHits->GetEntriesFast(); j++) {
    CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(j));
    double t      = sh->GetTime();
    if (t < start_t) start_t = t;
    SortStsHits.push_back(std::pair<double, int>(t, j));
  }

  TsStart = start_t;  ///reco TS start time is set to smallest hit time

  std::sort(SortStsHits.begin(), SortStsHits.end());
  StsIndex.clear();
  StsIndex.reserve(SortStsHits.size());
  for (unsigned int i = 0; i < SortStsHits.size(); i++) {
    int j = SortStsHits[i].second;
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
    if (listMvdHits) {
      nHits += listMvdHits->GetEntriesFast();
      nSta += NMvdStations;
    }
    if (listStsHits) {
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

      algo->SetData(fData->GetStsHits(), fData->GetNStsStrips(), fData->GetSFlag(), fData->GetStsHitsStartIndex(),
                    fData->GetStsHitsStopIndex());
    }
    else {
      ReadEvent(fData, TsStart, TsLength, TsOverlap, FstHitinTs, areDataLeft, event);
    }

    if constexpr (0) {  // correct hits on MC // dbg
      TRandom3 random;
      L1Vector<int> strips("CbmL1::strips");
      for (unsigned int iH = 0; iH < (*algo->vStsHits).size(); ++iH) {
        L1Hit& h = const_cast<L1Hit&>((*algo->vStsHits)[iH]);
#ifdef USE_EVENT_NUMBER
        h.n = -1;
#endif
        if (vStsHits[iH].mcPointIds.size() == 0) continue;

        const CbmL1MCPoint& mcp = vMCPoints[vStsHits[iH].mcPointIds[0]];

#ifdef USE_EVENT_NUMBER
        h.n = mcp.event;
#endif
        const int ista       = (*algo->fStripFlag)[h.f] / 4;
        const L1Station& sta = algo->vStations[ista];
        if (std::find(strips.begin(), strips.end(), h.f) != strips.end()) {  // separate strips

          (*algo->fStripFlag).push_back((*algo->fStripFlag)[h.f]);

          h.f = algo->NStsStrips;
          algo->NStsStrips++;
        }
        strips.push_back(h.f);
        if (std::find(strips.begin(), strips.end(), h.b) != strips.end()) {
          (*algo->fStripFlag).push_back((*algo->fStripFlag)[h.b]);
          h.b = algo->NStsStrips;
          algo->NStsStrips++;
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

    for (unsigned int iH = 0; iH < (*algo->vStsHits).size(); ++iH) {
#ifdef USE_EVENT_NUMBER
      L1Hit& h = const_cast<L1Hit&>((*algo->vStsHits)[iH]);
      h.n      = -1;
#endif
      if (vStsHits[iH].mcPointIds.size() == 0) continue;
#ifdef USE_EVENT_NUMBER
      const CbmL1MCPoint& mcp = vMCPoints[vStsHits[iH].mcPointIds[0]];
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
      L1FieldValue b = algo->GetVtxFieldValue();

      if ((fabs(b.x[0]) < 0.0000001) && (fabs(b.y[0]) < 0.0000001) && (fabs(b.z[0]) < 0.0000001)) {
        algo->KFTrackFitter_simple();
      }
      else {
        if (fTrackingMode == L1Algo::TrackingMode::kGlobal || fTrackingMode == L1Algo::TrackingMode::kMcbm) {
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
      t.StsHits.clear();
      t.fTrackTime = it->fTrackTime;

      L1Vector<int> StsHitsLocal("CbmL1::StsHitsLocal");
      StsHitsLocal.reserve(it->NHits);

      for (int i = 0; i < it->NHits; i++) {
        int start_hit1 = start_hit;
        if (algo->fRecoHits[start_hit1] > vStsHits.size() - 1) start_hit++;
        else if (!fLegacyEventMode) {
          t.StsHits.push_back(((*algo->vStsHits)[algo->fRecoHits[start_hit]]).ID);
        }
        else {
          t.StsHits.push_back(algo->fRecoHits[start_hit]);
        }
        StsHitsLocal.push_back(algo->fRecoHits[start_hit++]);
      }

      t.mass        = algo->fDefaultMass;  // pion mass
      t.is_electron = 0;

      t.SetId(vRTracksCur.size());
      //  CbmL1Track* prtra = &t;

      int indd         = 0;
      bool isInOverlap = 0;


      for (unsigned int i = 0; i < StsHitsLocal.size(); i++) {
        //      if ((*ih) > int(vStsHits.size() - 1)) {
        //         indd = 1;
        //         break;
        //       }

        if (vStsHits[StsHitsLocal[i]].t >= (TsStart + TsLength - TsOverlap)) {
          isInOverlap = 1;
          if (TsStart_new > vStsHits[StsHitsLocal[i]].t) TsStart_new = vStsHits[StsHitsLocal[i]].t;
        }

        int nMCPoints = vStsHits[StsHitsLocal[i]].mcPointIds.size();
        for (int iP = 0; iP < nMCPoints; iP++) {
          int iMC = vStsHits[StsHitsLocal[i]].mcPointIds[iP];
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
        for (unsigned int i = 0; i < StsHitsLocal.size(); i++) {
          algo->SetFUnUsed(const_cast<unsigned char&>((*algo->fStripFlag)[vStsHits[StsHitsLocal[i]].f]));
          algo->SetFUnUsed(const_cast<unsigned char&>((*algo->fStripFlag)[vStsHits[StsHitsLocal[i]].b]));
        }
      }
      vRTracksCur.push_back(t);
    }


    for (int i = 0; i < listStsHits->GetEntriesFast(); i++) {

      CbmStsHit* sh = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(StsIndex[i]));
      float time    = sh->GetTime();

      if (TsStart_new <= time) {
        FstHitinTs = i;
        break;
      }
    };

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

    for (unsigned int iHit = 0; iHit < vRTracksCur[iTrack].StsHits.size(); iHit++)
      if (!fLegacyEventMode) vRTracksCur[iTrack].StsHits[iHit] = SortedIndex[vRTracksCur[iTrack].StsHits[iHit]];

    vRTracks.push_back(vRTracksCur[iTrack]);
  }


  if ((fPerformance) && (fSTAPDataMode < 2)) { InputPerformance(); }


  for (unsigned int iH = 0; iH < (*algo->vStsHits).size(); ++iH) {
#ifdef USE_EVENT_NUMBER
    L1Hit& h = const_cast<L1Hit&>((*algo->vStsHits)[iH]);
    h.n      = -1;
#endif
    if (vStsHits[iH].mcPointIds.size() == 0) continue;
#ifdef USE_EVENT_NUMBER
    const CbmL1MCPoint& mcp = vMCPoints[vStsHits[iH].mcPointIds[0]];
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

    if (MC.StsHits.size() < 4) continue;

    L1Track algoTr;
    algoTr.NHits = 0;

    L1Vector<int> hitIndices("CbmL1::hitIndices", algo->NStations, -1);

    for (unsigned int iH = 0; iH < MC.StsHits.size(); iH++) {
      const int hitI      = MC.StsHits[iH];
      const CbmL1Hit& hit = vStsHits[hitI];

      const int iStation = vMCPoints[hit.mcPointIds[0]].iStation;

      if (iStation >= 0) hitIndices[iStation] = hitI;
    }


    for (int iH = 0; iH < algo->NStations; iH++) {
      const int hitI = hitIndices[iH];
      if (hitI < 0) continue;

      // algo->fRecoHits.push_back(hitI);
      algoTr.NHits++;
    }


    if (algoTr.NHits < 3) continue;

    for (int iH = 0; iH < algo->NStations; iH++) {
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
    // write vStsStrips
    int n = algo->NStsStrips;
    fadata << n << endl;
    if (fVerbose >= 4) {
      cout << "vStsStrips[" << n << "]"
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
    // write vStsHits
    n = (*algo->vStsHits).size();
    fadata << n << endl;
    for (int i = 0; i < n; i++) {
      const L1Hit& h = (*algo->vStsHits)[i];
      fadata << static_cast<int>(h.f) << " ";
      fadata << static_cast<int>(h.b) << " ";
#ifdef USE_EVENT_NUMBER
      fadata << static_cast<unsigned short int>(h.n) << " ";
#endif
      fadata << h.z << " ";
      fadata << h.u << " ";
      fadata << h.v << " ";
      // fadata  << (*algo->vStsHits)[i].time << endl;
      fadata << h.t << endl;
    };
    if (fVerbose >= 4) {
      cout << "vStsHits[" << n << "]"
           << " have been written." << endl;
    }
    // write StsHitsStartIndex and StsHitsStopIndex
    n = 20;
    for (int i = 0; i < n; i++) {
      if (int(L1Parameters::kMaxNstations) + 1 > i) { fadata << algo->StsHitsStartIndex[i] << endl; }
      else {
        fadata << 0 << endl;
      }
    };
    for (int i = 0; i < n; i++) {
      if (int(L1Parameters::kMaxNstations) + 1 > i) fadata << algo->StsHitsStopIndex[i] << endl;
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

      int nhits = vMCTracks[i].StsHits.size();
      fpdata << "   " << nhits << endl << "   ";
      for (int k = 0; k < nhits; k++) {
        fpdata << vMCTracks[i].StsHits[k] << " ";
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
    // write vStsHits
    n = vStsHits.size();  // number of elements
    fpdata << n << endl;
    for (int i = 0; i < n; i++) {
      fpdata << vStsHits[i].hitId << " ";
      fpdata << vStsHits[i].extIndex << endl;

      const int nPoints = vStsHits[i].mcPointIds.size();
      fpdata << nPoints << endl << "   ";
      for (int k = 0; k < nPoints; k++) {
        fpdata << vStsHits[i].mcPointIds[k] << " ";
      };
      fpdata << endl;
    };
    if (fVerbose >= 4) {
      cout << "vStsHits[" << n << "]"
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

    if (algo->vStsHits) algo->vStsHits->clear();
    algo->NStsStrips = 0;
    if (algo->fStripFlag) algo->fStripFlag->clear();

    // check correct position in file
    char s[] = "Event:  ";
    int nEv;
    fadata >> s;
    fadata >> nEv;
    if (nEv != nEvent) cout << "-E- CbmL1: Can't read event number " << nEvent << " from file " << fadata_name << endl;

    int n;  // number of elements
    // read algo->vStsStrips
    fadata >> n;
    cout << " n " << n << endl;
    algo->NStsStrips = n;
    if (fVerbose >= 4) {
      cout << "vStsStrips[" << n << "]"
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
    // read algo->vStsHits
    fadata >> n;
    int element_f;  // for convert
    int element_b;
    int element_n;
    for (int i = 0; i < n; i++) {
      L1Hit element;
      fadata >> element_f >> element_b >> element_n >> element.z >> element.u >> element.v >> element.t;
      element.f = static_cast<L1HitIndex_t>(element_f);
      element.b = static_cast<L1HitIndex_t>(element_b);
      algo->vStsHits->push_back(element);
    }
    if (fVerbose >= 4) {
      cout << "vStsHits[" << n << "]"
           << " have been read." << endl;
    }
    // read StsHitsStartIndex and StsHitsStopIndex
    n = 20;  // TODO: Why 20? (S.Zh.)
    for (int i = 0; i < n; i++) {
      int tmp;
      fadata >> tmp;
      if (int(L1Parameters::kMaxNstations) + 1 > i) (const_cast<unsigned int&>(algo->StsHitsStartIndex[i]) = tmp);
    }
    for (int i = 0; i < n; i++) {
      int tmp;
      fadata >> tmp;
      if (int(L1Parameters::kMaxNstations) + 1 > i) (const_cast<unsigned int&>(algo->StsHitsStopIndex[i]) = tmp);
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
    vStsHits.clear();
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
        element.StsHits.push_back(helement);
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
    // vStsHits
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
      vStsHits.push_back(element);
    };
    if (fVerbose >= 4) {
      cout << "vStsHits[" << n << "]"
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
  static bool first = 1;

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

    int NHits = (RecTrack->StsHits).size();
    float x[20], y[20], z[20];
    int st[20];
    int jHit = 0;
    for (int iHit = 0; iHit < NHits; iHit++) {
      CbmL1HitStore& h = vHitStore[RecTrack->StsHits[iHit]];
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
