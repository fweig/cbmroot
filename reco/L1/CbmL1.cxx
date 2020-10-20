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

#include "CbmKFVertex.h"
#include "CbmL1PFFitter.h"

#include "CbmMCDataManager.h"
#include "L1Algo/L1Algo.h"
#include "L1Algo/L1Branch.h"
#include "L1Algo/L1Field.h"
#include "L1Algo/L1StsHit.h"
#include "L1AlgoInputData.h"


#include "CbmKF.h"
#include "CbmStsFindTracks.h"
#include "CbmStsParSetModule.h"
#include "CbmStsParSetSensor.h"
#include "CbmStsParSetSensorCond.h"
#include "CbmStsSetup.h"
#include "CbmStsStation.h"
#include "FairEventHeader.h"
#include "FairRunAna.h"

#include "CbmTrdParModDigi.h"  // for CbmTrdModule
#include "CbmTrdParSetDigi.h"  // for CbmTrdParSetDigi

#include "CbmTofCell.h"
#include "CbmTofDigiPar.h"  // in tof/TofParam

#include "TGeoArb8.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoManager.h"

#include "CbmMuchGeoScheme.h"
#include "CbmMuchModuleGem.h"
#include "CbmMuchPad.h"
#include "CbmMuchStation.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include <list>

#include "L1Event.h"
#include "TMatrixD.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TVectorD.h"
#include <TFile.h>

#include "CbmMvdDetector.h"
#include "CbmMvdStationPar.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::ios;
using std::vector;

#include "KFTopoPerformance.h"

ClassImp(CbmL1)

  static L1Algo algo_static _fvecalignment;

//L1AlgoInputData* fData_static _fvecalignment;

CbmL1* CbmL1::fInstance = 0;


CbmL1::CbmL1()
  : FairTask("L1")
  , algo(0)
  ,  // for access to L1 Algorithm from L1::Instance
  fDigiFile()
  , fUseHitErrors(0)
  , fmCBMmode(0)
  , fGlobalMode(0)
  , vRTracks()
  ,  // reconstructed tracks
  vFileEvent()
  , vHitStore()
  , vMCPoints()
  , nMvdPoints(0)
  , vMCPoints_in_Time_Slice()
  , NStation(0)
  , NMvdStations(0)
  , NStsStations(0)
  , NMuchStations(0)
  , NTrdStations(0)
  , NTOFStation(0)
  ,  // number of detector stations (all\sts\mvd)
  fPerformance(0)
  , fSTAPDataMode(0)
  , fSTAPDataDir("")
  ,

  fTrackingLevel(2)
  ,  // really doesn't used
  fMomentumCutOff(0.1)
  ,  // really doesn't used
  fGhostSuppression(1)
  ,  // really doesn't used
  fUseMVD(0)
  ,  // really doesn't used
  fUseMUCH(0)
  , fUseTRD(0)
  , fUseTOF(0)
  ,

  PrimVtx()
  , fTimeSlice(nullptr)
  , fEventList(nullptr)
  , listStsDigi()
  , fStsPoints(0)
  , fMCTracks(0)
  , fMvdPoints(0)
  ,
  //listMCTracks (0),
  //listStsDigi(0),
  listStsPts(0)
  , listStsDigiMatch(0)
  , listStsClusters(0)
  , listStsHits(0)
  , listStsHitMatch(0)
  , listStsClusterMatch(0)
  ,

  listMvdPts(0)
  , listMvdHits(0)
  , listMvdDigiMatches(0)
  , listMvdHitMatches(0)
  ,

  nMuchPoints(0)
  , fMuchPoints(nullptr)
  , listMuchHitMatches(nullptr)
  , fDigiMatchesMuch(nullptr)
  , fClustersMuch(nullptr)
  , fMuchPixelHits(nullptr)
  , fDigisMuch(nullptr)
  , fTrdDigiPar(nullptr)
  , fTrdModuleInfo(nullptr)
  , fTrdPoints(nullptr)
  , listTrdHits(nullptr)
  , fTrdHitMatches(nullptr)
  , fTofPoints(nullptr)
  , fTofHitDigiMatches(nullptr)
  , fTofHits(nullptr)
  , fDigiPar(nullptr)
  ,

  fPerfFile(nullptr)
  , fHistoDir(nullptr)
  , vStsHits()
  , vMCTracks()
  , vHitMCRef()
  , dFEI2vMCPoints()
  , dFEI2vMCTracks()
  , fData(nullptr)
  , histodir(nullptr)
  , fFindParticlesMode()
  , fStsMatBudgetFileName("")
  , fMvdMatBudgetFileName("")
  , fMuchMatBudgetFileName("")
  , fTrdMatBudgetFileName("")
  , fTofMatBudgetFileName("")
  , fExtrapolateToTheEndOfSTS(false)
  , fTimesliceMode(0)
  , fTopoPerformance(nullptr)
  , fEventEfficiency() {
  if (!fInstance) fInstance = this;
}

CbmL1::CbmL1(const char* name,
             Int_t iVerbose,
             Int_t _fPerformance,
             int fSTAPDataMode_,
             TString fSTAPDataDir_,
             int findParticleMode_)
  : FairTask(name, iVerbose)
  , algo(0)
  ,  // for access to L1 Algorithm from L1::Instance
  fDigiFile()
  , fUseHitErrors(0)
  , fmCBMmode(0)
  , fGlobalMode(0)
  , vRTracks()
  ,  // reconstructed tracks
  vFileEvent()
  , vHitStore()
  , vMCPoints()
  , nMvdPoints(0)
  , vMCPoints_in_Time_Slice()
  , NStation(0)
  , NMvdStations(0)
  , NStsStations(0)
  , NMuchStations(0)
  , NTrdStations(0)
  , NTOFStation(0)
  ,  // number of detector stations (all\sts\mvd)
  fPerformance(_fPerformance)
  , fSTAPDataMode(fSTAPDataMode_)
  , fSTAPDataDir(fSTAPDataDir_)
  ,

  fTrackingLevel(2)
  ,  // really doesn't used
  fMomentumCutOff(0.1)
  ,  // really doesn't used
  fGhostSuppression(1)
  ,  // really doesn't used
  fUseMVD(0)
  ,  // really doesn't used
  fUseMUCH(0)
  , fUseTRD(0)
  , fUseTOF(0)
  ,


  PrimVtx()
  , fTimeSlice(nullptr)
  , fEventList(nullptr)
  , listStsDigi(0)
  , fStsPoints(0)
  , fMCTracks(0)
  , fMvdPoints(NULL)
  ,
  //listMCTracks (0),

  listStsPts(0)
  , listStsDigiMatch(0)
  , listStsClusters(0)
  , listStsHits(0)
  , listStsHitMatch(0)
  , listStsClusterMatch(0)
  ,

  listMvdPts(0)
  , listMvdHits(0)
  , listMvdDigiMatches(0)
  , listMvdHitMatches(0)
  ,

  nMuchPoints(0)
  , fMuchPoints(nullptr)
  , listMuchHitMatches(nullptr)
  , fDigiMatchesMuch(nullptr)
  , fClustersMuch(nullptr)
  , fMuchPixelHits(nullptr)
  , fDigisMuch(nullptr)
  , fTrdDigiPar(nullptr)
  , fTrdModuleInfo(nullptr)
  , fTrdPoints(nullptr)
  , listTrdHits(nullptr)
  , fTrdHitMatches(nullptr)
  , fTofPoints(nullptr)
  , fTofHitDigiMatches(nullptr)
  , fTofHits(nullptr)
  , fDigiPar(nullptr)
  ,

  fPerfFile(nullptr)
  , fHistoDir(nullptr)
  , vStsHits()
  , vMCTracks()
  , vHitMCRef()
  , dFEI2vMCPoints()
  , dFEI2vMCTracks()
  , fData(nullptr)
  , histodir(nullptr)
  , fFindParticlesMode(findParticleMode_)
  , fStsMatBudgetFileName("")
  , fMvdMatBudgetFileName("")
  , fMuchMatBudgetFileName("")
  , fTrdMatBudgetFileName("")
  , fTofMatBudgetFileName("")
  , fExtrapolateToTheEndOfSTS(false)
  , fTimesliceMode(0)
  , fTopoPerformance(nullptr)
  , fEventEfficiency() {
  if (!fInstance) fInstance = this;
}

CbmL1::~CbmL1() {
  if (fInstance == this) fInstance = 0;
}


void CbmL1::SetParContainers() {
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  fDigiPar            = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
  //  fTrdDigiPar = (CbmTrdDigiPar*)(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdDigiPar"));
  fTrdDigiPar = (CbmTrdParSetDigi*) (rtdb->getContainer("CbmTrdParSetDigi"));

  // Trigger loading of STS parameters
  // If L1 runs standalone the parameters are not required by any STS task
  // but they are needed by CbmStsSetup
  fStsParSetModule =
    dynamic_cast<CbmStsParSetModule*>(rtdb->getContainer("CbmStsParSetModule"));
  fStsParSetSensor =
    dynamic_cast<CbmStsParSetSensor*>(rtdb->getContainer("CbmStsParSetSensor"));
  fStsParSetSensorCond = dynamic_cast<CbmStsParSetSensorCond*>(
    rtdb->getContainer("CbmStsParSetSensorCond"));
}


InitStatus CbmL1::ReInit() {
  SetParContainers();
  return Init();
}

InitStatus CbmL1::Init() {
  fData = new L1AlgoInputData();

  if (fVerbose > 1) {
    char y[20] = " [0;33;44m";         // yellow
    char Y[20] = " [1;33;44m";         // yellow bold
    char W[20] = " [1;37;44m";         // white bold
    char o[20] = " [0m\n";             // color off
    Y[0] = y[0] = W[0] = o[0] = 0x1B;  // escape character

    cout << endl << endl;
    cout << "  " << W
         << "                                                                 "
         << o;
    cout << "  " << W
         << "  ===////======================================================  "
         << o;
    cout << "  " << W
         << "  =                                                           =  "
         << o;
    cout << "  " << W << "  =                   " << Y << "L1 on-line finder"
         << W << "                       =  " << o;
    cout << "  " << W
         << "  =                                                           =  "
         << o;
    cout << "  " << W << "  =  " << W << "Cellular Automaton 3.1 Vector" << y
         << " with " << W << "KF Quadro" << y << " technology" << W << "  =  "
         << o;
    cout << "  " << W
         << "  =                                                           =  "
         << o;
    cout << "  " << W << "  =  " << y << "Designed for CBM collaboration" << W
         << "                           =  " << o;
    cout << "  " << W << "  =  " << y << "All rights reserved" << W
         << "                                      =  " << o;
    cout << "  " << W
         << "  =                                                           =  "
         << o;
    cout << "  " << W
         << "  ========================================================////=  "
         << o;
    cout << "  " << W
         << "                                                                 "
         << o;
    cout << endl << endl;
  }

  FairRootManager* fManger = FairRootManager::Instance();

  FairRunAna* Run = FairRunAna::Instance();
  {
    fUseMVD = 1;
    CbmStsFindTracks* FindTask =
      L1_DYNAMIC_CAST<CbmStsFindTracks*>(Run->GetTask("STSFindTracks"));
    if (FindTask) fUseMVD = FindTask->MvdUsage();
  }

  fUseMVD = 0;

  histodir = gROOT->mkdir("L1");


  // turn on reconstruction in sub-detectors

  fUseMUCH = 0;
  fUseTRD  = 0;
  fUseTOF  = 0;

  if (fmCBMmode) {
    fUseMUCH = 1;
    fUseTRD  = 1;
    fUseTOF  = 1;
  }


  if (fGlobalMode) {
    fUseMUCH = 1;
    fUseTRD  = 1;
    fUseTOF  = 1;
  }


  fStsPoints  = 0;
  fMvdPoints  = 0;
  fMuchPoints = 0;
  fTrdPoints  = 0;
  fTofPoints  = 0;
  fMCTracks   = 0;


  listStsClusters = 0;
  listStsDigi.clear();
  vFileEvent.clear();


  if (fTimesliceMode) {  //  Time-slice mode selected
    LOG(info) << GetName() << ": running in time-slice mode.";
    fTimeSlice = NULL;
    fTimeSlice = (CbmTimeSlice*) fManger->GetObject("TimeSlice.");
    if (fTimeSlice == NULL)
      LOG(fatal) << GetName() << ": No time slice branch in tree!";

  }  //? time-slice mode

  else  // event mode
    LOG(info) << GetName() << ": running in event mode.";


  listStsClusters =
    L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("StsCluster"));
  listStsHitMatch =
    L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("StsHitMatch"));
  listStsClusterMatch =
    L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("StsClusterMatch"));

  listStsHits = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("StsHit"));

  if (!fUseMUCH) {
    fMuchPixelHits = 0;

    fDigisMuch       = 0;
    fDigiMatchesMuch = 0;
    fClustersMuch    = 0;

    fMuchPoints        = 0;
    listMuchHitMatches = 0;

  } else {


    fMuchPixelHits = (TClonesArray*) fManger->GetObject("MuchPixelHit");
  }

  if (!fUseTRD) {
    fTrdPoints     = 0;
    fTrdHitMatches = 0;
    fTrdPoints     = 0;
    listTrdHits    = 0;

  } else {

    listTrdHits = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("TrdHit"));
  }

  if (!fUseTOF) {
    fTofPoints         = 0;
    fTofHitDigiMatches = 0;
    fTofHits           = 0;

  } else {


    fTofHits = (TClonesArray*) fManger->GetObject("TofHit");
  }

  if (fPerformance) {
    CbmMCDataManager* mcManager =
      (CbmMCDataManager*) fManger->GetObject("MCDataManager");
    if (NULL == mcManager) LOG(fatal) << GetName() << ": No CbmMCDataManager!";

    fStsPoints = mcManager->InitBranch("StsPoint");
    if (!fTimesliceMode) fMvdPoints = mcManager->InitBranch("MvdPoint");
    fMCTracks = mcManager->InitBranch("MCTrack");
    if (NULL == fStsPoints) LOG(fatal) << GetName() << ": No StsPoint data!";
    if (NULL == fMCTracks) LOG(fatal) << GetName() << ": No MCTrack data!";

    listStsPts = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("StsPoint"));

    if (fTimesliceMode) {
      fEventList = (CbmMCEventList*) fManger->GetObject("MCEventList.");
      if (NULL == fEventList)
        LOG(fatal) << GetName() << ": No MCEventList data!";
    }

    if (!fUseMVD) {
      listMvdPts        = 0;
      listMvdHitMatches = 0;
    } else {
      listMvdPts =
        L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("MvdPoint"));
      listMvdDigiMatches =
        L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("MvdDigiMatch"));
      listMvdHitMatches =
        L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("MvdHitMatch"));

      if (!listMvdHitMatches && listMvdPts)
        LOG(error)
          << "No listMvdHitMatches provided, performance is not done correctly";
    }

    if (!fUseTRD) {
      fTrdPoints     = 0;
      fTrdHitMatches = 0;
      fTrdPoints     = 0;

    } else {
      fTrdHitMatches = (TClonesArray*) fManger->GetObject("TrdHitMatch");
      fTrdPoints     = mcManager->InitBranch("TrdPoint");
    }

    if (!fUseMUCH) {
      fMuchPoints        = 0;
      listMuchHitMatches = 0;

    } else {

      fDigisMuch       = (TClonesArray*) fManger->GetObject("MuchDigi");
      fDigiMatchesMuch = (TClonesArray*) fManger->GetObject("MuchDigiMatch");
      fClustersMuch    = (TClonesArray*) fManger->GetObject("MuchCluster");
      fMuchPoints      = mcManager->InitBranch("MuchPoint");
      listMuchHitMatches =
        L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("MuchPixelHitMatch"));
    }

    if (!fUseTOF) {
      fTofPoints         = 0;
      fTofHitDigiMatches = 0;
    } else {

      fTofPoints = mcManager->InitBranch("TofPoint");
      fTofHitDigiMatches =
        static_cast<TClonesArray*>(fManger->GetObject("TofCalDigiMatch"));
    }

  } else {
    listMvdPts         = 0;
    listMvdHitMatches  = 0;
    fTrdPoints         = 0;
    fTrdHitMatches     = 0;
    fTrdPoints         = 0;
    fMuchPoints        = 0;
    listMuchHitMatches = 0;
    fTofPoints         = 0;
    fTofHitDigiMatches = 0;
  }
  if (!fUseMVD) {
    listMvdHits = 0;
  } else {
    listMvdHits = L1_DYNAMIC_CAST<TClonesArray*>(fManger->GetObject("MvdHit"));
  }

  NMvdStations  = 0;
  NStsStations  = 0;
  NMuchStations = 0;
  NTrdStations  = 0;
  NTOFStation   = 0;
  NStation      = 0;

  if (fUseTOF) NTOFStation = 1;

  algo = &algo_static;

  vector<fscal> geo;
  geo.clear();

  for (int i = 0; i < 3; i++) {
    Double_t point[3] = {0, 0, 2.5 * i};
    Double_t B[3]     = {0, 0, 0};
    if (CbmKF::Instance()->GetMagneticField())
      CbmKF::Instance()->GetMagneticField()->GetFieldValue(point, B);
    geo.push_back(2.5 * i);
    geo.push_back(B[0]);
    geo.push_back(B[1]);
    geo.push_back(B[2]);
  }


  CbmMuchGeoScheme* fGeoScheme = CbmMuchGeoScheme::Instance();

  if (fUseMUCH) {
    TFile* file         = new TFile(fDigiFile);
    TObjArray* stations = (TObjArray*) file->Get("stations");
    fGeoScheme->Init(stations, 0);
    for (int iStation = 0; iStation < fGeoScheme->GetNStations(); iStation++) {
      const CbmMuchStation* station = fGeoScheme->GetStation(iStation);
      int nLayers                   = station->GetNLayers();
      NMuchStations += nLayers;
    }
  }

  // count TRD stations
  if (fUseTRD) {
    Int_t layerCounter  = 0;
    TObjArray* topNodes = gGeoManager->GetTopNode()->GetNodes();
    for (Int_t iTopNode = 0; iTopNode < topNodes->GetEntriesFast();
         iTopNode++) {
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
  }

  // count ToF parameters

  float z_average = 0;

  float z_min = 100000;
  float z_max = 0;

  float r_max = 0;
  float r_min = 100000;

  if (fUseTOF) {

    Int_t nrOfCells = fDigiPar->GetNrOfModules();

    for (Int_t icell = 0; icell < nrOfCells; ++icell) {

      Int_t cellId          = fDigiPar->GetCellId(icell);
      CbmTofCell* fCellInfo = fDigiPar->GetCell(cellId);

      Double_t x = fCellInfo->GetX();
      Double_t y = fCellInfo->GetY();
      Double_t z = fCellInfo->GetZ();
      //       Double_t dx = fCellInfo->GetSizex();
      //       Double_t dy = fCellInfo->GetSizey();

      if (z < z_min) z_min = z;
      if (z > z_max) z_max = z;

      if (x < r_min) r_min = x;
      if (x > r_max) r_max = x;

      if (y < r_min) r_min = y;
      if (y > r_max) r_max = y;


      z_average += z;
    }
    z_average = z_average / nrOfCells;
  }

  CbmStsSetup* stsSetup = CbmStsSetup::Instance();
  if (!stsSetup->IsInit()) { stsSetup->Init(nullptr); }
  if (!stsSetup->IsModuleParsInit()) {
    stsSetup->SetModuleParameters(fStsParSetModule);
  }
  if (!stsSetup->IsSensorParsInit()) {
    stsSetup->SetSensorParameters(fStsParSetSensor);
  }
  if (!stsSetup->IsSensorCondInit()) {
    stsSetup->SetSensorConditions(fStsParSetSensorCond);
  }

  NMvdStations = (fUseMVD) ? CbmKF::Instance()->vMvdMaterial.size() : 0;
  NStsStations = CbmStsSetup::Instance()->GetNofStations();
  NStation =
    NMvdStations + NStsStations + NMuchStations + NTrdStations + NTOFStation;
  geo.push_back(NStation);
  geo.push_back(NMvdStations);
  geo.push_back(NStsStations);

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

  for (Int_t ist = 0; ist < NStation; ist++) {
    double C[3][N];
    double z    = 0;
    double Xmax = 0, Ymax = 0;
    if (ist < NMvdStations) {


      CbmMvdDetector* mvdDetector     = CbmMvdDetector::Instance();
      CbmMvdStationPar* mvdStationPar = mvdDetector->GetParameterFile();


      CbmKFTube& t = CbmKF::Instance()->vMvdMaterial[ist];
      geo.push_back(1);
      geo.push_back(t.z);
      geo.push_back(t.dz);
      geo.push_back(t.r);
      geo.push_back(t.R);
      geo.push_back(t.RadLength);

      fscal f_phi = 0, f_sigma = mvdStationPar->GetXRes(ist) / 10000,
            b_phi   = 3.14159265358 / 2.,
            b_sigma = mvdStationPar->GetYRes(ist) / 10000;
      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);
      z    = t.z;
      Xmax = Ymax = t.R;
    }


    if (ist >= NMvdStations && ist < (NMvdStations + NStsStations)) {
      CbmStsStation* station =
        CbmStsSetup::Instance()->GetStation(ist - NMvdStations);
      geo.push_back(0);
      geo.push_back(station->GetZ());
      geo.push_back(station->GetSensorD());
      geo.push_back(0);
      geo.push_back(station->GetYmax() < station->GetXmax()
                      ? station->GetXmax()
                      : station->GetYmax());
      geo.push_back(station->GetRadLength());

      double Pi = 3.14159265358;

      fscal f_phi, f_sigma, b_phi, b_sigma;  // angle and sigma front/back  side
      f_phi = station->GetSensorRotation();
      b_phi = f_phi;
      f_phi += station->GetSensorStereoAngle(0) * Pi / 180.;
      b_phi += station->GetSensorStereoAngle(1) * Pi / 180.;
      f_sigma = station->GetSensorPitch(0) / TMath::Sqrt(12);
      b_sigma = f_sigma;
      //f_sigma *= cos(f_phi);  // TODO: think about this
      //b_sigma *= cos(b_phi);

      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);
      z = station->GetZ();

      Xmax = station->GetXmax();
      Ymax = station->GetYmax();
    }

    if ((ist < (NMvdStations + NStsStations + NMuchStations))
        && (ist >= (NMvdStations + NStsStations))) {

      int iStation = (ist - NMvdStations - NStsStations) / 3;


      CbmMuchStation* station =
        (CbmMuchStation*) fGeoScheme->GetStation(iStation);

      CbmMuchLayer* layer =
        station->GetLayer((ist - NMvdStations - NStsStations) % 3);

      //  CbmMuchModuleGem* module = (CbmMuchModuleGem*)  CbmMuchGeoScheme::Instance()->GetModule(0,0,0,0);

      // vector<CbmMuchPad*> pads = module->GetPads();

      z = layer->GetZ();

      geo.push_back(2);
      geo.push_back(z);
      geo.push_back(layer->GetDz());
      geo.push_back(0);
      geo.push_back(100);  //station->GetRmax()
      geo.push_back(0);

      fscal f_phi = 0, f_sigma = 0.1, b_phi = 3.14159265358 / 2., b_sigma = 0.1;
      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);


      Xmax = 100;  //station->GetRmax();
      Ymax = 100;  //station->GetRmax();
    }

    //     int num = 0;

    if ((ist < (NMvdStations + NStsStations + NTrdStations + NMuchStations))
        && (ist >= (NMvdStations + NStsStations + NMuchStations))) {

      int num = ist - NMvdStations - NStsStations - NMuchStations;

      //   if (num == 0) continue;//true_station = 0;
      //   if (!true_station) continue;

      //      Int_t nrModules = fTrdDigiPar->GetNrOfModules();

      int ModuleId = fTrdDigiPar->GetModuleId(num);

      CbmTrdParModDigi* module =
        (CbmTrdParModDigi*) fTrdDigiPar->GetModulePar(ModuleId);

      //   if (!true_station[ist]) continue;

      if (num == 0 || num == 2 || num == 4) geo.push_back(3);
      if (num == 1 || num == 3) geo.push_back(6);
      geo.push_back(module->GetZ());

      geo.push_back(2 * module->GetSizeZ());
      geo.push_back(0);
      geo.push_back(2 * module->GetSizeX());
      geo.push_back(10);

      fscal f_phi = 0, f_sigma = 1 / 10000, b_phi = 3.14159265358 / 2.,
            b_sigma = 1 / 10000;
      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);
      Xmax = Ymax = 20;
    }

    if ((ist < (NMvdStations + NStsStations + NTrdStations + NMuchStations
                + NTOFStation))
        && (ist
            >= (NMvdStations + NStsStations + NMuchStations + NTrdStations))) {

      geo.push_back(4);
      geo.push_back(z_average);
      geo.push_back(z_max - z_min);
      geo.push_back(0);
      geo.push_back(r_max);
      geo.push_back(10);

      fscal f_phi = 0, f_sigma = 1 / 10000, b_phi = 3.14159265358 / 2.,
            b_sigma = 1 / 10000;
      geo.push_back(f_phi);
      geo.push_back(f_sigma);
      geo.push_back(b_phi);
      geo.push_back(b_sigma);
      Xmax = Ymax = 20;
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
      LOG(error) << "-E- CbmL1: Read geometry from file "
                 << fSTAPDataDir + "geo_algo.txt was NOT successful.";
  }

  algo->Init(geo, fUseHitErrors, fmCBMmode);
  geo.clear();


  algo->fRadThick.resize(algo->NStations);

  // Read STS  MVD TRD MuCh ToF Radiation Thickness table
  TString stationName = "Radiation Thickness [%], Station";
  if (fUseMVD) {
    if (fMvdMatBudgetFileName != "") {
      TFile* oldfile = gFile;
      TFile* rlFile  = new TFile(fMvdMatBudgetFileName);
      cout << "MVD Material budget file is " << fMvdMatBudgetFileName << ".\n";
      for (int j = 0, iSta = 0; iSta < algo->NMvdStations; iSta++, j++) {
        TString stationNameMvd = stationName;
        stationNameMvd += j;
        TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameMvd);
        if (!hStaRadLen) {
          cout << "L1: incorrect " << fMvdMatBudgetFileName << " file. No "
               << stationNameMvd << "\n";
          exit(1);
        }
        const int NBins = hStaRadLen->GetNbinsX();  // should be same in Y
        const float RMax =
          hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
        algo->fRadThick[iSta].SetBins(NBins, RMax);
        algo->fRadThick[iSta].table.resize(NBins);

        for (int iB = 0; iB < NBins; iB++) {
          algo->fRadThick[iSta].table[iB].resize(NBins);
          for (int iB2 = 0; iB2 < NBins; iB2++) {
            algo->fRadThick[iSta].table[iB][iB2] =
              0.01 * hStaRadLen->GetBinContent(iB, iB2);
            // Correction for holes in material map
            if (algo->fRadThick[iSta].table[iB][iB2]
                < algo->vStations[iSta].materialInfo.RadThick[0])
              if (iB2 > 0 && iB2 < NBins - 1)
                algo->fRadThick[iSta].table[iB][iB2] =
                  TMath::Min(0.01 * hStaRadLen->GetBinContent(iB, iB2 - 1),
                             0.01 * hStaRadLen->GetBinContent(iB, iB2 + 1));
            // Correction for the incorrect harcoded value of RadThick of MVD stations
            if (algo->fRadThick[iSta].table[iB][iB2] < 0.0015)
              algo->fRadThick[iSta].table[iB][iB2] = 0.0015;
            //              algo->fRadThick[iSta].table[iB][iB2] = algo->vStations[iSta].materialInfo.RadThick[0];
          }
        }
      }
      rlFile->Close();
      rlFile->Delete();
      gFile = oldfile;
    } else {
      LOG(warn) << "No MVD material budget file is found. Homogenious budget "
                   "will be used";
      for (int iSta = 0; iSta < algo->NMvdStations; iSta++) {
        cout << iSta << endl;
        algo->fRadThick[iSta].SetBins(1,
                                      100);  // mvd should be in +-100 cm square
        algo->fRadThick[iSta].table.resize(1);
        algo->fRadThick[iSta].table[0].resize(1);
        algo->fRadThick[iSta].table[0][0] =
          algo->vStations[iSta].materialInfo.RadThick[0];
      }
    }
  }
  if (fStsMatBudgetFileName != "") {
    TFile* oldfile = gFile;
    TFile* rlFile  = new TFile(fStsMatBudgetFileName);
    cout << "STS Material budget file is " << fStsMatBudgetFileName << ".\n";
    for (int j = 1, iSta = algo->NMvdStations;
         iSta < (algo->NMvdStations + NStsStations);
         iSta++, j++) {
      TString stationNameSts = stationName;
      stationNameSts += j;
      TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameSts);
      if (!hStaRadLen) {
        cout << "L1: incorrect " << fStsMatBudgetFileName << " file. No "
             << stationNameSts << "\n";
        exit(1);
      }
      const int NBins = hStaRadLen->GetNbinsX();  // should be same in Y
      const float RMax =
        hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
      algo->fRadThick[iSta].SetBins(NBins, RMax);
      algo->fRadThick[iSta].table.resize(NBins);

      for (int iB = 0; iB < NBins; iB++) {
        algo->fRadThick[iSta].table[iB].resize(NBins);
        for (int iB2 = 0; iB2 < NBins; iB2++) {
          algo->fRadThick[iSta].table[iB][iB2] =
            0.01 * hStaRadLen->GetBinContent(iB, iB2);
          if (algo->fRadThick[iSta].table[iB][iB2]
              < algo->vStations[iSta].materialInfo.RadThick[0])
            algo->fRadThick[iSta].table[iB][iB2] =
              algo->vStations[iSta].materialInfo.RadThick[0];
          // cout << " iSta " << iSta << " iB " << iB << "iB2 " << iB2 << " RadThick " << algo->fRadThick[iSta].table[iB][iB2] << endl;
        }
      }
    }
    rlFile->Close();
    rlFile->Delete();
    gFile = oldfile;
  } else {
    LOG(warn) << "No STS material budget file is found. Homogenious budget "
                 "will be used";
    for (int iSta = algo->NMvdStations;
         iSta < (algo->NMvdStations + NStsStations);
         iSta++) {
      algo->fRadThick[iSta].SetBins(1, 100);
      algo->fRadThick[iSta].table.resize(1);
      algo->fRadThick[iSta].table[0].resize(1);
      algo->fRadThick[iSta].table[0][0] =
        algo->vStations[iSta].materialInfo.RadThick[0];
    }
  }

  if (fUseMUCH)
    if (fMuchMatBudgetFileName != "") {
      TFile* oldfile = gFile;
      TFile* rlFile  = new TFile(fMuchMatBudgetFileName);
      cout << "Much Material budget file is " << fMuchMatBudgetFileName
           << ".\n";
      for (int j = 1, iSta = (NStsStations + NMvdStations);
           iSta < (NStsStations + NMvdStations + NMuchStations);
           iSta++, j++) {
        TString stationNameSts = stationName;
        stationNameSts += j;
        TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameSts);
        if (!hStaRadLen) {
          cout << "L1: incorrect " << fMuchMatBudgetFileName << " file. No "
               << stationNameSts << "\n";
          exit(1);
        }


        const int NBins = hStaRadLen->GetNbinsX();  // should be same in Y
        const float RMax =
          hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
        algo->fRadThick[iSta].SetBins(NBins, RMax);
        algo->fRadThick[iSta].table.resize(NBins);

        for (int iB = 0; iB < NBins; iB++) {
          algo->fRadThick[iSta].table[iB].resize(NBins);
          float hole = 0.15;
          for (int iB2 = 0; iB2 < NBins; iB2++) {
            algo->fRadThick[iSta].table[iB][iB2] =
              0.01 * hStaRadLen->GetBinContent(iB, iB2);
            // Correction for holes in material map
            //if(algo->fRadThick[iSta].table[iB][iB2] < algo->vStations[iSta].materialInfo.RadThick[0])

            if (iB2 > 0 && iB2 < NBins - 1)
              algo->fRadThick[iSta].table[iB][iB2] =
                TMath::Min(0.01 * hStaRadLen->GetBinContent(iB, iB2 - 1),
                           0.01 * hStaRadLen->GetBinContent(iB, iB2 + 1));
            // Correction for the incorrect harcoded value of RadThick of MVD stations

            if (algo->fRadThick[iSta].table[iB][iB2] > 0.0015)
              hole = algo->fRadThick[iSta].table[iB][iB2];
            if (algo->fRadThick[iSta].table[iB][iB2] < 0.0015)
              algo->fRadThick[iSta].table[iB][iB2] = hole;
            //              algo->fRadThick[iSta].table[iB][iB2] = algo->vStations[iSta].materialInfo.RadThick[0];
          }
        }
      }
      rlFile->Close();
      rlFile->Delete();
      gFile = oldfile;
    } else {
      LOG(warn) << "No Much material budget file is found. Homogenious budget "
                   "will be used";
      for (int iSta = (NStsStations + NMvdStations);
           iSta < (NStsStations + NMvdStations + NMuchStations);
           iSta++) {
        algo->fRadThick[iSta].SetBins(1, 100);
        algo->fRadThick[iSta].table.resize(1);
        algo->fRadThick[iSta].table[0].resize(1);
        algo->fRadThick[iSta].table[0][0] =
          algo->vStations[iSta].materialInfo.RadThick[0];
      }
    }

  if (fUseTRD)
    if (fTrdMatBudgetFileName != "") {
      TFile* oldfile = gFile;
      TFile* rlFile  = new TFile(fTrdMatBudgetFileName);
      cout << "TRD Material budget file is " << fTrdMatBudgetFileName << ".\n";
      for (int j = 1, iSta = (NStsStations + NMvdStations + NMuchStations);
           iSta < (NStsStations + NMvdStations + NMuchStations + NTrdStations);
           iSta++, j++) {
        TString stationNameSts = stationName;
        stationNameSts += j;
        TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameSts);
        if (!hStaRadLen) {
          cout << "L1: incorrect " << fTrdMatBudgetFileName << " file. No "
               << stationNameSts << "\n";
          exit(1);
        }


        const int NBins = hStaRadLen->GetNbinsX();  // should be same in Y
        const float RMax =
          hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
        algo->fRadThick[iSta].SetBins(NBins, RMax);
        algo->fRadThick[iSta].table.resize(NBins);

        for (int iB = 0; iB < NBins; iB++) {
          algo->fRadThick[iSta].table[iB].resize(NBins);
          float hole = 0.15;
          for (int iB2 = 0; iB2 < NBins; iB2++) {
            algo->fRadThick[iSta].table[iB][iB2] =
              0.01 * hStaRadLen->GetBinContent(iB, iB2);
            // Correction for holes in material map
            //if(algo->fRadThick[iSta].table[iB][iB2] < algo->vStations[iSta].materialInfo.RadThick[0])

            if (iB2 > 0 && iB2 < NBins - 1)
              algo->fRadThick[iSta].table[iB][iB2] =
                TMath::Min(0.01 * hStaRadLen->GetBinContent(iB, iB2 - 1),
                           0.01 * hStaRadLen->GetBinContent(iB, iB2 + 1));
            // Correction for the incorrect harcoded value of RadThick of MVD stations

            if (algo->fRadThick[iSta].table[iB][iB2] > 0.0015)
              hole = algo->fRadThick[iSta].table[iB][iB2];
            if (algo->fRadThick[iSta].table[iB][iB2] < 0.0015)
              algo->fRadThick[iSta].table[iB][iB2] = hole;
            //              algo->fRadThick[iSta].table[iB][iB2] = algo->vStations[iSta].materialInfo.RadThick[0];
          }
        }
      }
      rlFile->Close();
      rlFile->Delete();
      gFile = oldfile;
    } else {
      LOG(warn) << "No TRD material budget file is found. Homogenious budget "
                   "will be used";
      for (int iSta = (NStsStations + NMvdStations + NMuchStations);
           iSta < (NStsStations + NMvdStations + NMuchStations + NTrdStations);
           iSta++) {
        algo->fRadThick[iSta].SetBins(1, 100);
        algo->fRadThick[iSta].table.resize(1);
        algo->fRadThick[iSta].table[0].resize(1);
        algo->fRadThick[iSta].table[0][0] =
          algo->vStations[iSta].materialInfo.RadThick[0];
      }
    }

  if (fUseTOF)
    if (fTofMatBudgetFileName != "") {
      TFile* oldfile = gFile;
      TFile* rlFile  = new TFile(fTofMatBudgetFileName);
      cout << "TOF Material budget file is " << fTofMatBudgetFileName << ".\n";
      for (int j = 1,
               iSta =
                 (NStsStations + NMvdStations + NMuchStations + NTrdStations);
           iSta < (NStsStations + NMvdStations + NMuchStations + NTrdStations
                   + NTOFStation);
           iSta++, j++) {
        TString stationNameSts = stationName;
        stationNameSts += j;
        TProfile2D* hStaRadLen = (TProfile2D*) rlFile->Get(stationNameSts);
        if (!hStaRadLen) {
          cout << "L1: incorrect " << fTofMatBudgetFileName << " file. No "
               << stationNameSts << "\n";
          exit(1);
        }


        const int NBins = hStaRadLen->GetNbinsX();  // should be same in Y
        const float RMax =
          hStaRadLen->GetXaxis()->GetXmax();  // should be same as min
        algo->fRadThick[iSta].SetBins(NBins, RMax);
        algo->fRadThick[iSta].table.resize(NBins);

        for (int iB = 0; iB < NBins; iB++) {
          algo->fRadThick[iSta].table[iB].resize(NBins);
          float hole = 0.0015;
          for (int iB2 = 0; iB2 < NBins; iB2++) {
            algo->fRadThick[iSta].table[iB][iB2] =
              0.01 * hStaRadLen->GetBinContent(iB, iB2);
            // Correction for holes in material map
            //if(algo->fRadThick[iSta].table[iB][iB2] < algo->vStations[iSta].materialInfo.RadThick[0])

            if (iB2 > 0 && iB2 < NBins - 1)
              algo->fRadThick[iSta].table[iB][iB2] =
                TMath::Min(0.01 * hStaRadLen->GetBinContent(iB, iB2 - 1),
                           0.01 * hStaRadLen->GetBinContent(iB, iB2 + 1));
            // Correction for the incorrect harcoded value of RadThick of MVD stations

            if (algo->fRadThick[iSta].table[iB][iB2] > 0.0015)
              hole = algo->fRadThick[iSta].table[iB][iB2];
            if (algo->fRadThick[iSta].table[iB][iB2] < 0.0015)
              algo->fRadThick[iSta].table[iB][iB2] = hole;
            //              algo->fRadThick[iSta].table[iB][iB2] = algo->vStations[iSta].materialInfo.RadThick[0];
          }
        }
      }
      rlFile->Close();
      rlFile->Delete();
      gFile = oldfile;
    } else {
      LOG(warn) << "No TOF material budget file is found. Homogenious budget "
                   "will be used";
      for (int iSta =
             (NStsStations + NMvdStations + NMuchStations + NTrdStations);
           iSta < (NStsStations + NMvdStations + NMuchStations + NTrdStations
                   + NTOFStation);
           iSta++) {
        algo->fRadThick[iSta].SetBins(1, 100);
        algo->fRadThick[iSta].table.resize(1);
        algo->fRadThick[iSta].table[0].resize(1);
        algo->fRadThick[iSta].table[0][0] =
          algo->vStations[iSta].materialInfo.RadThick[0];
      }
    }
  return kSUCCESS;
}


void CbmL1::Exec(Option_t* /*option*/) {}

void CbmL1::Reconstruct(CbmEvent* event) {
  static int nevent = 0;
  vFileEvent.clear();

  if (fTimesliceMode && fPerformance) {

    int nofEvents = fEventList->GetNofEvents();
    for (int iE = 0; iE < nofEvents; iE++) {

      int fileId  = fEventList->GetFileIdByIndex(iE);
      int eventId = fEventList->GetEventIdByIndex(iE);
      vFileEvent.insert(DFSET::value_type(fileId, eventId));
    }

  } else {
    Int_t iFile  = FairRunAna::Instance()->GetEventHeader()->GetInputFileId();
    Int_t iEvent = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
    vFileEvent.insert(DFSET::value_type(iFile, iEvent));
  }


  if (fVerbose > 1)
    cout << endl << "CbmL1::Exec event " << ++nevent << " ..." << endl << endl;
#ifdef _OPENMP
  omp_set_num_threads(1);
#endif
  // repack data

  fData->Clear();

  if (fSTAPDataMode >= 2) {  // 2,3
    fData->ReadHitsFromFile(fSTAPDataDir.Data(), 1, fVerbose);

    algo->SetData(fData->GetStsHits(),
                  fData->GetStsStrips(),
                  fData->GetStsStripsB(),
                  fData->GetStsZPos(),
                  fData->GetSFlag(),
                  fData->GetSFlagB(),
                  fData->GetStsHitsStartIndex(),
                  fData->GetStsHitsStopIndex());
  } else
    ReadEvent(fData, event);

  if (0) {  // correct hits on MC // dbg
    TRandom3 random;
    vector<int> sF, sB, zP;
    sF.clear();
    sB.clear();
    zP.clear();
    for (unsigned int iH = 0; iH < (*algo->vStsHits).size(); ++iH) {
      L1StsHit& h = const_cast<L1StsHit&>((*algo->vStsHits)[iH]);
#ifdef USE_EVENT_NUMBER
      h.n = -1;
#endif
      if (vStsHits[iH].mcPointIds.size() == 0) continue;

      const CbmL1MCPoint& mcp = vMCPoints[vStsHits[iH].mcPointIds[0]];

#ifdef USE_EVENT_NUMBER
      h.n = mcp.event;
#endif
      const int ista       = (*algo->vSFlag)[h.f] / 4;
      const L1Station& sta = algo->vStations[ista];
      if (std::find(sF.begin(), sF.end(), h.f)
          != sF.end()) {  // separate strips

        const_cast<vector<unsigned char>*>(algo->vSFlag)
          ->push_back((*algo->vSFlag)[h.f]);

        h.f = (*algo->vStsStrips).size();

        const_cast<std::vector<L1Strip>*>(algo->vStsStrips)
          ->push_back(L1Strip());
      }
      sF.push_back(h.f);
      if (std::find(sB.begin(), sB.end(), h.b) != sB.end()) {
        const_cast<vector<unsigned char>*>(algo->vSFlagB)
          ->push_back((*algo->vSFlagB)[h.b]);
        h.b = (*algo->vStsStripsB).size();

        const_cast<std::vector<L1Strip>*>(algo->vStsStripsB)
          ->push_back(L1Strip());
      }
      sB.push_back(h.b);
      if (std::find(zP.begin(), zP.end(), h.iz)
          != zP.end()) {  // TODO why do we need it??gives prob=0
        h.iz = (*algo->vStsZPos).size();
        const_cast<std::vector<float>*>(algo->vStsZPos)->push_back(0);
      }
      zP.push_back(h.iz);

      const fscal idet = 1
                         / (sta.xInfo.sin_phi * sta.yInfo.sin_phi
                            - sta.xInfo.cos_phi * sta.yInfo.cos_phi)[0];

#if 1  // GAUSS
      const_cast<std::vector<L1Strip>*>(algo->vStsStrips)->at(h.f) =
        idet * (+sta.yInfo.sin_phi[0] * mcp.x - sta.xInfo.cos_phi[0] * mcp.y)
        + random.Gaus(0, sqrt(sta.frontInfo.sigma2)[0]);
      const_cast<L1Strip&>((*algo->vStsStripsB)[h.b]) =
        idet * (-sta.yInfo.cos_phi[0] * mcp.x + sta.xInfo.sin_phi[0] * mcp.y)
        + random.Gaus(0, sqrt(sta.backInfo.sigma2)[0]);
#else  // UNIFORM
      (*algo->vStsStrips)[h.f] =
        idet * (+sta.yInfo.sin_phi[0] * mcp.x - sta.xInfo.cos_phi[0] * mcp.y)
        + random.Uniform(-sqrt(sta.frontInfo.sigma2)[0] * 3.5,
                         sqrt(sta.frontInfo.sigma2)[0] * 3.5);
      (*algo->vStsStripsB)[h.b] =
        idet * (-sta.yInfo.cos_phi[0] * mcp.x + sta.xInfo.sin_phi[0] * mcp.y)
        + random.Uniform(-sqrt(sta.backInfo.sigma2)[0] * 3.5,
                         sqrt(sta.backInfo.sigma2)[0] * 3.5);
#endif
      const_cast<float&>((*algo->vStsZPos)[h.iz]) = mcp.z;
    }
  }


  if (fPerformance) {
    HitMatch();
    // calculate the max number of Hits\mcPoints on continuous(consecutive) stations

    for (vector<CbmL1MCTrack>::iterator it = vMCTracks.begin();
         it != vMCTracks.end();
         ++it)
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
    L1StsHit& h = const_cast<L1StsHit&>((*algo->vStsHits)[iH]);
    h.n         = -1;
#endif
    if (vStsHits[iH].mcPointIds.size() == 0) continue;
#ifdef USE_EVENT_NUMBER
    const CbmL1MCPoint& mcp = vMCPoints[vStsHits[iH].mcPointIds[0]];
    h.n                     = mcp.event;
#endif
  }

  for (vector<CbmL1MCTrack>::iterator i = vMCTracks.begin();
       i != vMCTracks.end();
       ++i) {
    CbmL1MCTrack& MC = *i;

    if (!MC.IsReconstructable()) continue;
    if (!(MC.ID >= 0)) continue;

    if (MC.StsHits.size() < 4) continue;
    vector<int> hitIndices(algo->NStations, -1);

    for (unsigned int iH = 0; iH < MC.StsHits.size(); iH++) {
      const int hitI   = MC.StsHits[iH];
      CbmL1StsHit& hit = const_cast<CbmL1StsHit&>(vStsHits[hitI]);

      hit.event = MC.iEvent;

      // const int iStation = vMCPoints[hit.mcPointIds[0]].iStation;
      // hitIndices[iStation] = hitI;
    }
  }


  if (fVerbose > 1) cout << "L1 Track finder..." << endl;
  algo->CATrackFinder();
  // IdealTrackFinder();


  if (fVerbose > 1) cout << "L1 Track finder ok" << endl;
  //  algo->L1KFTrackFitter( fExtrapolateToTheEndOfSTS );


  if (fmCBMmode || fGlobalMode) {

    L1FieldValue fB0 = algo->GetVtxFieldValue();

    if ((fabs(fB0.x[0]) < 0.0000001) && (fabs(fB0.y[0]) < 0.0000001)
        && (fabs(fB0.z[0]) < 0.0000001))
      algo->KFTrackFitter_simple();

    else
      algo->L1KFTrackFitterMuch();
  } else {

    L1FieldValue fB0 = algo->GetVtxFieldValue();

    if ((fabs(fB0.x[0]) < 0.0000001) && (fabs(fB0.y[0]) < 0.0000001)
        && (fabs(fB0.z[0]) < 0.0000001))
      algo->KFTrackFitter_simple();

    else
      algo->L1KFTrackFitter();
  }


  if (fVerbose > 1) cout << "L1 Track fitter  ok" << endl;

  // save recontstructed tracks
  vRTracks.clear();
  int start_hit = 0;

  for (vector<L1Track>::iterator it = algo->vTracks.begin();
       it != (algo->vTracks.begin() + algo->NTracksIsecAll);
       it++) {

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

    for (int i = 0; i < it->NHits; i++) {
      int start_hit1 = start_hit;

      if (algo->vRecoHits[start_hit1] > vStsHits.size() - 1)
        start_hit++;
      else
        t.StsHits.push_back(algo->vRecoHits[start_hit++]);
    }
    t.mass        = 0.1395679;  // pion mass
    t.is_electron = 0;

    t.SetId(vRTracks.size());
    CbmL1Track* prtra = &t;

    int indd = 0;

    for (vector<int>::iterator ih = (prtra->StsHits).begin();
         ih != (prtra->StsHits).end();
         ++ih) {
      if ((*ih) > int(vStsHits.size() - 1)) {
        indd = 1;
        break;
      }
      int nMCPoints = vStsHits[*ih].mcPointIds.size();
      for (int iP = 0; iP < nMCPoints; iP++) {
        int iMC = vStsHits[*ih].mcPointIds[iP];
        if (iMC > int(vMCPoints.size() - 1)) {
          //           cout << " iMC " << iMC << " vMCPoints.size() " <<  vMCPoints.size() << endl;
          indd = 1;
        }
      }
    }

    if (indd == 1) continue;

    vRTracks.push_back(t);
  }
  // output performance
  if (fPerformance) {
    if (fVerbose > 1) cout << "Performance..." << endl;
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
  if (fVerbose > 1) cout << "End of L1" << endl;

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
}

// -----   Finish CbmStsFitPerformanceTask task   -----------------------------
void CbmL1::Finish() {
  TDirectory* curr   = gDirectory;
  TFile* currentFile = gFile;

  // Open output file and write histograms
  TFile* outfile = new TFile("L1_histo.root", "RECREATE");
  outfile->cd();
  writedir2current(histodir);
  outfile->Close();
  outfile->Delete();

  gFile      = currentFile;
  gDirectory = curr;
}


void CbmL1::writedir2current(TObject* obj) {
  if (!obj->IsFolder())
    obj->Write();
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

void CbmL1::IdealTrackFinder() {
  algo->vTracks.clear();
  algo->vRecoHits.clear();

  for (vector<CbmL1MCTrack>::iterator i = vMCTracks.begin();
       i != vMCTracks.end();
       ++i) {
    CbmL1MCTrack& MC = *i;

    if (!MC.IsReconstructable()) continue;
    if (!(MC.ID >= 0)) continue;

    if (MC.StsHits.size() < 4) continue;

    L1Track algoTr;
    algoTr.NHits = 0;

    vector<int> hitIndices(algo->NStations, -1);

    for (unsigned int iH = 0; iH < MC.StsHits.size(); iH++) {
      const int hitI         = MC.StsHits[iH];
      const CbmL1StsHit& hit = vStsHits[hitI];
      const int iStation     = vMCPoints[hit.mcPointIds[0]].iStation;

      if (iStation >= 0) hitIndices[iStation] = hitI;
    }


    for (int iH = 0; iH < algo->NStations; iH++) {
      const int hitI = hitIndices[iH];
      if (hitI < 0) continue;

      // algo->vRecoHits.push_back(hitI);
      algoTr.NHits++;
    }


    if (algoTr.NHits < 3) continue;

    for (int iH = 0; iH < algo->NStations; iH++) {
      const int hitI = hitIndices[iH];
      if (hitI < 0) continue;
      algo->vRecoHits.push_back(hitI);
    }


    algoTr.Momentum  = MC.p;
    algoTr.TFirst[0] = MC.x;
    algoTr.TFirst[1] = MC.y;
    algoTr.TFirst[2] = MC.px / MC.pz;
    algoTr.TFirst[3] = MC.py / MC.pz;
    algoTr.TFirst[4] = MC.q / MC.p;
    algoTr.TFirst[5] = MC.z;

    algo->vTracks.push_back(algoTr);
  }
  algo->NTracksIsecAll = algo->vTracks.size();
};  // void CbmL1::IdealTrackFinder()


/// -----   STandAlone Package service-functions  -----------------------------

void CbmL1::WriteSTAPGeoData(const vector<float>& geo_) {
  // write geo in file
  TString fgeo_name = fSTAPDataDir + "geo_algo.txt";
  ofstream fgeo(fgeo_name);
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
  fstream fadata;

  TString fadata_name = fSTAPDataDir + "data_algo.txt";
  //    if ( vNEvent <= maxNEvent ) {
  if (1) {

    if (vNEvent == 1)
      fadata.open(fadata_name, fstream::out);  // begin new file
    else
      fadata.open(fadata_name, fstream::out | fstream::app);

    fadata << "Event:"
           << " ";
    fadata << vNEvent << endl;
    // write vStsStrips
    int n = (*algo->vStsStrips).size();  // number of elements
    fadata << n << endl;
    for (int i = 0; i < n; i++) {
      fadata << (*algo->vStsStrips)[i] << endl;
    };
    if (fVerbose >= 4)
      cout << "vStsStrips[" << n << "]"
           << " have been written." << endl;
    // write vStsStripsB
    n = (*algo->vStsStripsB).size();
    fadata << n << endl;
    for (int i = 0; i < n; i++) {
      fadata << (*algo->vStsStripsB)[i] << endl;
    };
    if (fVerbose >= 4)
      cout << "vStsStripsB[" << n << "]"
           << " have been written." << endl;
    // write vStsZPos
    n = (*algo->vStsZPos).size();
    fadata << n << endl;
    for (int i = 0; i < n; i++) {
      fadata << (*algo->vStsZPos)[i] << endl;
    };
    if (fVerbose >= 4)
      cout << "vStsZPos[" << n << "]"
           << " have been written." << endl;
    // write vSFlag
    n = (*algo->vSFlag).size();
    fadata << n << endl;
    unsigned char element;
    for (int i = 0; i < n; i++) {
      element = (*algo->vSFlag)[i];
      fadata << static_cast<int>(element) << endl;
    };
    if (fVerbose >= 4)
      cout << "vSFlag[" << n << "]"
           << " have been written." << endl;
    // write vSFlagB
    n = (*algo->vSFlagB).size();
    fadata << n << endl;
    for (int i = 0; i < n; i++) {
      element = (*algo->vSFlagB)[i];
      fadata << static_cast<int>(element) << endl;
    };
    if (fVerbose >= 4)
      cout << "vSFlagB[" << n << "]"
           << " have been written." << endl;
    // write vStsHits
    n = (*algo->vStsHits).size();
    fadata << n << endl;
    for (int i = 0; i < n; i++) {
      fadata << static_cast<int>((*algo->vStsHits)[i].f) << " ";
      fadata << static_cast<int>((*algo->vStsHits)[i].b) << " ";
#ifdef USE_EVENT_NUMBER
      fadata << static_cast<unsigned short int>((*algo->vStsHits)[i].n) << " ";
#endif
      fadata << static_cast<int>((*algo->vStsHits)[i].iz) << " ";
      // fadata  << (*algo->vStsHits)[i].time << endl;

      fadata << (*algo->vStsHits)[i].t_reco << endl;
    };
    if (fVerbose >= 4)
      cout << "vStsHits[" << n << "]"
           << " have been written." << endl;
    // write StsHitsStartIndex and StsHitsStopIndex
    n = 20;
    for (int i = 0; i < n; i++) {
      if (int(algo->MaxNStations) + 1 > i)
        fadata << algo->StsHitsStartIndex[i] << endl;
      else
        fadata << 0 << endl;
    };
    for (int i = 0; i < n; i++) {
      if (int(algo->MaxNStations) + 1 > i)
        fadata << algo->StsHitsStopIndex[i] << endl;
      else
        fadata << 0 << endl;
    };


    fadata.close();
  }
  cout << "-I- CbmL1: CATrackFinder data for event number " << vNEvent
       << " have been written in file " << fadata_name << endl;
  vNEvent++;
}  // void CbmL1::WriteSTAPAlgoData()


void CbmL1::WriteSTAPPerfData()  // must be called after ReadEvent
{
  fstream fpdata;
  fpdata << setprecision(8);

  static int vNEvent = 1;

  TString fpdata_name = fSTAPDataDir + "data_perfo.txt";
  // write data for performance in file
  //   if ( vNEvent <= maxNEvent )  {
  if (1) {

    if (vNEvent == 1)
      fpdata.open(fpdata_name, fstream::out);  // begin new file
    else
      fpdata.open(fpdata_name, fstream::out | fstream::app);

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
    if (fVerbose >= 4)
      cout << "vMCPoints[" << n << "]"
           << " have been written." << endl;

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
    if (fVerbose >= 4)
      cout << "vMCTracks[" << n << "]"
           << " have been written." << endl;

    // write vHitMCRef
    n = vHitMCRef.size();  // number of elements
    fpdata << n << endl;
    for (int i = 0; i < n; i++) {
      fpdata << vHitMCRef[i] << endl;
    };
    if (fVerbose >= 4)
      cout << "vHitMCRef[" << n << "]"
           << " have been written." << endl;

    // write vHitStore
    n = vHitStore.size();  // number of elements
    fpdata << n << endl;
    for (int i = 0; i < n; i++) {
      fpdata << vHitStore[i].ExtIndex << "  ";
      fpdata << vHitStore[i].iStation << "  ";

      fpdata << vHitStore[i].x << " ";
      fpdata << vHitStore[i].y << endl;
    };
    if (fVerbose >= 4)
      cout << "vHitStore[" << n << "]"
           << " have been written." << endl;

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
    if (fVerbose >= 4)
      cout << "vStsHits[" << n << "]"
           << " have been written." << endl;

    fpdata.close();
  }
  cout << "-I- CbmL1: Data for performance of event number " << vNEvent
       << " have been written in file " << fpdata_name << endl;
  vNEvent++;
}  // void CbmL1::WriteSTAPPerfData()

istream& CbmL1::eatwhite(istream& is)  // skip spaces
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

//void CbmL1::ReadSTAPGeoData(vector<float> geo_, int &size)
//void CbmL1::ReadSTAPGeoData(vector<fscal> geo_, int &size)
void CbmL1::ReadSTAPGeoData(vector<fscal>& geo_, int& size) {
  TString fgeo_name = fSTAPDataDir + "geo_algo.txt";
  ifstream fgeo(fgeo_name);

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

void CbmL1::ReadSTAPAlgoData() {
  static int nEvent = 1;
  static fstream fadata;
  TString fadata_name = fSTAPDataDir + "data_algo.txt";
  //  if (nEvent <= maxNEvent){
  if (1) {
    if (nEvent == 1) fadata.open(fadata_name, fstream::in);

    if (algo->vStsHits)
      const_cast<std::vector<L1StsHit>*>(algo->vStsHits)->clear();
    if (algo->vStsStrips)
      const_cast<std::vector<L1Strip>*>(algo->vStsStrips)->clear();
    if (algo->vStsStripsB)
      const_cast<std::vector<L1Strip>*>(algo->vStsStripsB)->clear();
    if (algo->vStsZPos)
      const_cast<std::vector<float>*>(algo->vStsZPos)->clear();
    if (algo->vSFlag) const_cast<vector<unsigned char>*>(algo->vSFlag)->clear();
    if (algo->vSFlagB)
      const_cast<vector<unsigned char>*>(algo->vSFlagB)->clear();

    // check correct position in file
    char s[] = "Event:  ";
    int nEv;
    fadata >> s;
    fadata >> nEv;
    if (nEv != nEvent)
      cout << "-E- CbmL1: Can't read event number " << nEvent << " from file "
           << fadata_name << endl;

    int n;  // number of elements
    // read algo->vStsStrips
    fadata >> n;
    cout << " n " << n << endl;
    for (int i = 0; i < n; i++) {
      fscal element;
      fadata >> element;
      const_cast<std::vector<L1Strip>*>(algo->vStsStrips)->push_back(element);
    }
    if (fVerbose >= 4)
      cout << "vStsStrips[" << n << "]"
           << " have been read." << endl;
    // read algo->vStsStripsB
    fadata >> n;
    for (int i = 0; i < n; i++) {
      fscal element;
      fadata >> element;
      const_cast<std::vector<L1Strip>*>(algo->vStsStripsB)->push_back(element);
    }
    if (fVerbose >= 4)
      cout << "vStsStripsB[" << n << "]"
           << " have been read." << endl;
    // read algo->vStsZPos
    fadata >> n;
    for (int i = 0; i < n; i++) {
      fscal element;
      fadata >> element;
      const_cast<std::vector<float>*>(algo->vStsZPos)->push_back(element);
    }
    if (fVerbose >= 4)
      cout << "vStsZPos[" << n << "]"
           << " have been read." << endl;
    // read algo->vSFlag
    fadata >> n;
    for (int i = 0; i < n; i++) {
      int element;
      fadata >> element;
      const_cast<vector<unsigned char>*>(algo->vSFlag)
        ->push_back(static_cast<unsigned char>(element));
    }
    if (fVerbose >= 4)
      cout << "vSFlag[" << n << "]"
           << " have been read." << endl;
    // read algo->vSFlagB
    fadata >> n;
    for (int i = 0; i < n; i++) {
      int element;
      fadata >> element;
      const_cast<std::vector<L1Strip>*>(algo->vStsStripsB)
        ->push_back(static_cast<unsigned char>(element));
    }
    if (fVerbose >= 4)
      cout << "vSFlagB[" << n << "]"
           << " have been read." << endl;
    // read algo->vStsHits
    fadata >> n;
    int element_f;  // for convert
    int element_b;
    int element_n;
    int element_iz;
    float time;
    for (int i = 0; i < n; i++) {
      L1StsHit element;
      fadata >> element_f >> element_b >> element_n >> element_iz >> time;
      element.f  = static_cast<THitI>(element_f);
      element.b  = static_cast<THitI>(element_b);
      element.iz = static_cast<TZPosI>(element_iz);

      element.t_reco = time;
      const_cast<std::vector<L1StsHit>*>(algo->vStsHits)->push_back(element);
    }
    if (fVerbose >= 4)
      cout << "vStsHits[" << n << "]"
           << " have been read." << endl;
    // read StsHitsStartIndex and StsHitsStopIndex
    n = 20;
    for (int i = 0; i < n; i++) {
      int tmp;
      fadata >> tmp;
      if (int(algo->MaxNStations) + 1 > i)
        (const_cast<unsigned int&>(algo->StsHitsStartIndex[i]) = tmp);
    }
    for (int i = 0; i < n; i++) {
      int tmp;
      fadata >> tmp;
      if (int(algo->MaxNStations) + 1 > i)
        (const_cast<unsigned int&>(algo->StsHitsStopIndex[i]) = tmp);
    }

    cout << "-I- CbmL1: CATrackFinder data for event " << nEvent
         << " has been read from file " << fadata_name << " successfully."
         << endl;
  }
  nEvent++;
}  // void CbmL1::ReadSTAPAlgoData()

void CbmL1::ReadSTAPPerfData() {
  static int nEvent = 1;
  static fstream fpdata;
  TString fpdata_name = fSTAPDataDir + "data_perfo.txt";
  //  if (nEvent <= maxNEvent){
  if (1) {
    if (nEvent == 1) { fpdata.open(fpdata_name, fstream::in); };

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
      cout << "-E- CbmL1: Performance: can't read event number " << nEvent
           << " from file "
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
    if (fVerbose >= 4)
      cout << "vMCPoints[" << n << "]"
           << " have been read." << endl;

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
    if (fVerbose >= 4)
      cout << "vMCTracks[" << n << "]"
           << " have been read." << endl;

    // vHitMCRef
    fpdata >> n;
    for (int i = 0; i < n; i++) {
      int element;
      fpdata >> element;
      vHitMCRef.push_back(element);
    };
    if (fVerbose >= 4)
      cout << "vHitMCRef[" << n << "]"
           << " have been read." << endl;

    // vHitStore
    fpdata >> n;
    for (int i = 0; i < n; i++) {
      CbmL1HitStore element;
      fpdata >> element.ExtIndex;
      fpdata >> element.iStation;

      fpdata >> element.x;
      fpdata >> element.y;
      vHitStore.push_back(element);
    };
    if (fVerbose >= 4)
      cout << "vHitStore[" << n << "]"
           << " have been read." << endl;

    // vStsHits
    fpdata >> n;
    for (int i = 0; i < n; i++) {
      CbmL1StsHit element;
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
    if (fVerbose >= 4)
      cout << "vStsHits[" << n << "]"
           << " have been read." << endl;


    //    if (nEvent == maxNEvent) { // file open on begin of all work class and close at end
    //       fpdata.close();
    //       cout << " -I- Performance: data read from file " << "data_perfo.txt" << " successfully"<< endl;
    //     }
    cout << "-I- CbmL1: L1Performance data for event " << nEvent
         << " has been read from file " << fpdata_name << " successfully."
         << endl;

  }  // if (nEvent <= maxNEvent)
  nEvent++;
}  // void CbmL1::ReadSTAPPerfData()

void CbmL1::WriteSIMDKFData() {
  static bool first = 1;

  /// Write geometry info
  if (first) {
    FairField* dMF = CbmKF::Instance()->GetMagneticField();

    fstream FileGeo;
    FileGeo.open("geo.dat", ios::out);

    fstream FieldCheck;
    FieldCheck.open("field.dat", ios::out);

    Double_t bfg[3], rfg[3];

    rfg[0] = 0.;
    rfg[1] = 0.;
    rfg[2] = 0.;
    dMF->GetFieldValue(rfg, bfg);
    FileGeo << rfg[2] << " " << bfg[0] << " " << bfg[1] << " " << bfg[2] << " "
            << endl;

    rfg[0] = 0.;
    rfg[1] = 0.;
    rfg[2] = 2.5;
    dMF->GetFieldValue(rfg, bfg);
    FileGeo << rfg[2] << " " << bfg[0] << " " << bfg[1] << " " << bfg[2] << " "
            << endl;

    rfg[0] = 0.;
    rfg[1] = 0.;
    rfg[2] = 5.0;
    dMF->GetFieldValue(rfg, bfg);
    FileGeo << rfg[2] << " " << bfg[0] << " " << bfg[1] << " " << bfg[2] << " "
            << endl
            << endl;
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
      } else {
        CbmStsStation* station =
          CbmStsSetup::Instance()->GetStation(ist - NMvdStations);
        f_phi     = station->GetSensorRotation();
        b_phi     = f_phi;
        double Pi = 3.14159265358;
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
      } else if (ist < (NStsStations + NMvdStations)) {
        CbmStsStation* station =
          CbmStsSetup::Instance()->GetStation(ist - NMvdStations);
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
  fstream Tracks, McTracksCentr, McTracksIn, McTracksOut;
  if (first) {
    Tracks.open("tracks.dat", fstream::out);
    McTracksCentr.open("mctrackscentr.dat", fstream::out);
    McTracksIn.open("mctracksin.dat", fstream::out);
    McTracksOut.open("mctracksout.dat", fstream::out);
    first = 0;
  } else {
    Tracks.open("tracks.dat", fstream::out | fstream::app);
    McTracksCentr.open("mctrackscentr.dat", fstream::out | fstream::app);
    McTracksIn.open("mctracksin.dat", fstream::out | fstream::app);
    McTracksOut.open("mctracksout.dat", fstream::out | fstream::app);
  }

  for (vector<CbmL1Track>::iterator RecTrack = vRTracks.begin();
       RecTrack != vRTracks.end();
       ++RecTrack) {
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
      } else {
        CbmStsHit* StsH = (CbmStsHit*) listStsHits->At(h.ExtIndex);
        x[jHit]         = StsH->GetX();
        y[jHit]         = StsH->GetY();
        z[jHit]         = StsH->GetZ();
        jHit++;
      }
    }

    Tracks << TrNumber << " " << MCTrack->x << " " << MCTrack->y << " "
           << MCTrack->z << " " << MCTrack->px << " " << MCTrack->py << " "
           << MCTrack->pz << " " << MCTrack->q << " " << NHits << endl;

    float AngleXAxis = 0, AngleYAxis = 0;
    for (int i = 0; i < NHits; i++)
      Tracks << "     " << st[i] << " " << x[i] << " " << y[i] << " " << z[i]
             << " " << AngleXAxis << " " << AngleYAxis << endl;
    Tracks << endl;

    int NMCPoints = (MCTrack->Points).size();

    McTracksIn << TrNumber << " " << MCTrack->x << " " << MCTrack->y << " "
               << MCTrack->z << " " << MCTrack->px << " " << MCTrack->py << " "
               << MCTrack->pz << " " << MCTrack->q << " " << NMCPoints << endl;
    McTracksOut << TrNumber << " " << MCTrack->x << " " << MCTrack->y << " "
                << MCTrack->z << " " << MCTrack->px << " " << MCTrack->py << " "
                << MCTrack->pz << " " << MCTrack->q << " " << NMCPoints << endl;
    McTracksCentr << TrNumber << " " << MCTrack->x << " " << MCTrack->y << " "
                  << MCTrack->z << " " << MCTrack->px << " " << MCTrack->py
                  << " " << MCTrack->pz << " " << MCTrack->q << " " << NMCPoints
                  << endl;

    for (int iPoint = 0; iPoint < NMCPoints; iPoint++) {
      CbmL1MCPoint& MCPoint = vMCPoints[MCTrack->Points[iPoint]];
      McTracksIn << "     " << MCPoint.iStation << " " << MCPoint.xIn << " "
                 << MCPoint.yIn << " " << MCPoint.zIn << " " << MCPoint.pxIn
                 << " " << MCPoint.pyIn << " " << MCPoint.pzIn << endl;
      McTracksOut << "     " << MCPoint.iStation << " " << MCPoint.xOut << " "
                  << MCPoint.yOut << " " << MCPoint.zOut << " " << MCPoint.pxOut
                  << " " << MCPoint.pyOut << " " << MCPoint.pzOut << endl;
      McTracksCentr << "     " << MCPoint.iStation << " " << MCPoint.x << " "
                    << MCPoint.y << " " << MCPoint.z << " " << MCPoint.px << " "
                    << MCPoint.py << " " << MCPoint.pz << endl;
    }
    McTracksIn << endl;
    McTracksOut << endl;
    McTracksCentr << endl;

    TrNumber++;
  }
}
