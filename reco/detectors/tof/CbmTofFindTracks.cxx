// -------------------------------------------------------------------------
// -----                  CbmTofFindTracks source file                -----
// -----                  Created 25/04/15  by N. Herrmann             -----
// -----                  initially following  CbmTrdFindTracks        -----
// -------------------------------------------------------------------------

#include "CbmTofFindTracks.h"
#include "CbmTofAddress.h"  // in cbmdata/tof

#include "CbmEvent.h"
#include "CbmMatch.h"
#include "CbmTofCalibrator.h"
#include "CbmTofCell.h"             // in tof/TofData
#include "CbmTofCreateDigiPar.h"    // in tof/TofTools
#include "CbmTofDetectorId_v12b.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v21a.h"  // in cbmdata/tof
#include "CbmTofDigiBdfPar.h"       // in tof/TofParam
#include "CbmTofDigiPar.h"          // in tof/TofParam
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofHit.h"
#include "CbmTofTrackFinder.h"
#include "CbmTofTrackFinderNN.h"
#include "CbmTofTrackFitter.h"
#include "CbmTofTracklet.h"
#include "CbmTofTrackletTools.h"
#include "CbmVertex.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "TClonesArray.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TGeoManager.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

//const Int_t DetMask = 0x3FFFFF;  // check for consistency with v14a geometry
const Int_t DetMask = 0x1FFFFF;  // check for consistency with v21a geometry

ClassImp(CbmTofFindTracks);

CbmTofFindTracks* CbmTofFindTracks::fInstance = 0;

// -----   Default constructor   -------------------------------------------
CbmTofFindTracks::CbmTofFindTracks()
  : CbmTofFindTracks::CbmTofFindTracks("TofFindTracks", "Main", NULL) {
  if (!fInstance) fInstance = this;
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTofFindTracks::CbmTofFindTracks(const char* name,
                                   const char* /*title*/,
                                   CbmTofTrackFinder* finder)
  : FairTask(name)
  , fFinder(finder)
  , fFitter(NULL)
  , fTrackletTools(NULL)
  , fTofCalibrator(NULL)
  , fEventsColl(NULL)
  , fTofHitArrayIn(NULL)
  , fTofMatchArrayIn(NULL)
  , fTofHitArray(NULL)
  , fTrackArray(NULL)
  , fTrackArrayOut(nullptr)
  , fTofUHitArray(NULL)
  , fMinNofHits(-1)
  , fNofTracks(-1)
  , fNTofStations(-1)
  , fNReqStations(-1)
  , fInspectEvent(kTRUE)
  , fStationType()
  , fStationHMul()
  , fRpcAddr()
  , fMapStationRpcId()
  , fMapRpcIdParInd()
  , fhTrklMul(NULL)
  , fhTrklChi2(NULL)
  , fhAllHitsStation(NULL)
  , fhAllHitsSmTypes(NULL)
  , fhUsedHitsStation(NULL)
  , fhTrackingTimeNhits(NULL)
  , fhTrklMulNhits(NULL)
  , fhTrklMulMaxMM(NULL)
  , fhTrklMul3D(NULL)
  , fhTrklHMul(NULL)
  , fhTrklZ0xHMul(NULL)
  , fhTrklZ0yHMul(NULL)
  , fhTrklTxHMul(NULL)
  , fhTrklTyHMul(NULL)
  , fhTrklTtHMul(NULL)
  , fhTrklVelHMul(NULL)
  , fhTrklT0HMul(NULL)
  , fhTrklT0Mul(NULL)
  , fhTrklDT0SmMis(NULL)
  , fhTrklDT0StMis2(NULL)
  , fhTrklXY0_0(NULL)
  , fhTrklXY0_1(NULL)
  , fhTrklXY0_2(NULL)
  , vhPullX()
  , vhPullY()
  , vhPullZ()
  , vhPullT()
  , vhPullTB()
  , vhResidualTBWalk()
  , vhResidualYWalk()
  , vhXY_AllTracks()
  , vhXY_AllStations()
  , vhXY_MissedStation()
  , vhXY_DX()
  , vhXY_DY()
  , vhXY_DT()
  , vhXY_TOT()
  , vhXY_CSZ()
  , vhUDXDY_DT()
  , vhUCDXDY_DT()
  , fhVTXNorm(NULL)
  , fhVTX_XY0(NULL)
  , fhVTX_DT0_Norm(NULL)
  , fOutHstFileName("")
  , fCalParFileName("")
  , fCalOutFileName("./tofFindTracks.hst.root")
  , fCalParFile(NULL)
  , fhPullT_Smt(NULL)
  , fhPullT_Smt_Off(NULL)
  , fhPullX_Smt(NULL)
  , fhPullX_Smt_Off(NULL)
  , fhPullY_Smt(NULL)
  , fhPullY_Smt_Off(NULL)
  , fhPullZ_Smt(NULL)
  , fhPullZ_Smt_Off(NULL)
  , fhPullT_Smt_Width(NULL)
  , fhPullX_Smt_Width(NULL)
  , fhPullY_Smt_Width(NULL)
  , fhPullZ_Smt_Width(NULL)
  , fhTOff_Smt(NULL)
  , fhTOff_Smt_Off(NULL)
  , fhDeltaTt_Smt(NULL)
  , fhTOff_HMul2(NULL)
  , fiCorMode(0)
  , fiBeamCounter(-1)
  , fiStationMaxHMul(1000)
  , fTtTarg(30.)
  , fVTXNorm(0.)
  , fVTX_T(0.)
  , fVTX_X(0.)
  , fVTX_Y(0.)
  , fVTX_Z(0.)
  , fT0MAX(0.5)
  , fiEvent(0)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fDigiPar(NULL)
  , fDigiBdfPar(NULL)
  , fSIGT(0.1)
  , fSIGX(1.)
  , fSIGY(1.)
  , fSIGZ(1.)
  , fbUseSigCalib(kTRUE)
  , fdRefVelMean(0.)
  , fdRefDVel(1.E7)
  , fStart()
  , fStop()
  , fdTrackingTime(0.)
  , fdBeamMomentumLab(0.)
  , fbRemoveSignalPropagationTime(kFALSE)
  , fiBeamMaxHMul(1000)
  , fiCalOpt(0) {
  if (!fInstance) fInstance = this;
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTofFindTracks::~CbmTofFindTracks() {
  if (fInstance == this) fInstance = 0;
  fTrackArray->Delete();
}
// -------------------------------------------------------------------------


// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmTofFindTracks::Init() {

  // Check for Track finder
  if (!fFinder) {
    cout << "-W- CbmTofFindTracks::Init: No track finder selected!" << endl;
    return kERROR;
  }

  // Check for Track fitter
  if (!fFitter) {
    cout << "-W- CbmTofFindTracks::Init: No track fitter selected!" << endl;
    return kERROR;
  }
  cout << Form("-D- CbmTofFindTracks::Init: track fitter at 0x%p", fFitter)
       << endl;

  fTrackletTools = new CbmTofTrackletTools();  // initialize tools

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- CbmTofFindTracks::Init: "
         << "RootManager not instantiated!" << endl;
    return kFATAL;
  }

  ioman->InitSink();

  fEventsColl = dynamic_cast<TClonesArray*>(ioman->GetObject("Event"));
  if (!fEventsColl)
    fEventsColl = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  if (!fEventsColl) {
    LOG(info) << "CbmEvent not found in input file, assume eventwise input";
  } else {
    fTofHitArray = new TClonesArray("CbmTofHit");
  }

  // Get TOF hit Array
  fTofHitArrayIn = (TClonesArray*) ioman->GetObject("TofHit");
  if (!fTofHitArrayIn) {
    LOG(fatal) << "-W- CbmTofFindTracks::Init: No TofHit array!";
    return kERROR;
  }

  // Get TOF hit Array
  fTofMatchArrayIn = (TClonesArray*) ioman->GetObject("TofCalDigiMatch");
  if (!fTofMatchArrayIn) {
    LOG(fatal) << "CbmTofFindTracks::Init: No TofDigiMatch array!";
    return kERROR;
  }

  // Create and register output TofTrack array
  fTrackArray   = new TClonesArray("CbmTofTracklet", 100);
  fTofUHitArray = new TClonesArray("CbmTofHit", 100);
  //fTrackArray->BypassStreamer(kTRUE);  //needed?
  //ioman->Register("TofTracks", "TOF", fTrackArray, kFALSE); //FIXME
  if (fEventsColl) {
    fTrackArrayOut = new TClonesArray("CbmTofTracklet", 100);
    ioman->Register(
      "TofTracks", "TOF", fTrackArrayOut, kTRUE);  //FIXME, does not work !
  } else {
    ioman->Register(
      "TofTracks", "TOF", fTrackArray, kTRUE);  //FIXME, does not work !
    cout << "-I- CbmTofFindTracks::Init:TofTrack array registered" << endl;

    // Create and register TofUHit array for unused Hits
    ioman->Register("TofUHit", "TOF", fTofUHitArray, kFALSE);
  }
  // Call the Init method of the track finder
  fFinder->Init();

  if (fOutHstFileName == "") { fOutHstFileName = "./FindTofTracks.hst.root"; }
  LOG(info) << "CbmTofFindTracks::Init: Hst Output filename = "
            << fOutHstFileName;

  if (kFALSE == InitParameters()) return kFATAL;

  // default parameters
  // if (fMinNofHits < 1) fMinNofHits=1;

  //fill RpcId - map
  Bool_t bBeamCounter = kFALSE;
  Int_t iRpc          = 0;
  for (Int_t iCell = 0; iCell < fDigiPar->GetNrOfModules(); iCell++) {
    Int_t iCellId = fDigiPar->GetCellId(iCell);
    Int_t iCh     = fTofId->GetCell(iCellId);
    if (0 == iCh) {
      LOG(info) << Form(
        "Init found RpcInd %d at Addr 0x%08x, ModType %d, ModId %d, RpcId %d ",
        iRpc,
        iCellId,
        fTofId->GetSMType(iCellId),
        fTofId->GetSModule(iCellId),
        fTofId->GetCounter(iCellId));
      if (fTofId->GetSMType(iCellId) == 5) {
        bBeamCounter = kTRUE;
        LOG(info) << "Found beam counter in setup!";
      }
      fMapRpcIdParInd[iCellId] = iRpc;
      fRpcAddr.resize(fRpcAddr.size() + 1);
      fRpcAddr.push_back(iCellId);
      iRpc++;
    }
  }
  fStationHMul.resize(fNTofStations + 1);


  LoadCalParameter();

  CreateHistograms();

  if (fiCalOpt > 0) {
    fTofCalibrator = new CbmTofCalibrator();
    if (fTofCalibrator->Init() != kSUCCESS) return kFATAL;
    if (bBeamCounter) {
      fTofCalibrator->SetBeam(bBeamCounter);
      fTofCalibrator->SetR0Lim(
        10.);  // FIXME, hardwired parameter for debugging
      LOG(info) << "Set CbmTofCalibrator::R0Lim to 10.";
    }
  }

  LOG(info) << Form("BeamCounter to be used in tracking: 0x%08x",
                    fiBeamCounter);

  return kSUCCESS;
}
// -------------------------------------------------------------------------
/************************************************************************************/
Bool_t CbmTofFindTracks::LoadCalParameter() {
  if (fCalParFileName.IsNull()) return kTRUE;

  fCalParFile = new TFile(fCalParFileName, "");
  if (NULL == fCalParFile) {
    LOG(error) << "CbmTofFindTracks::LoadCalParameter: "
               << "file " << fCalParFileName << " does not exist!";
    return kTRUE;
  }

  LOG(info) << "CbmTofFindTracks::LoadCalParameter: "
            << " read from file " << fCalParFileName;

  TH1D* fhtmp   = (TH1D*) gDirectory->FindObjectAny(Form("hPullT_Smt_Off"));
  TH1D* fhtmpX  = (TH1D*) gDirectory->FindObjectAny(Form("hPullX_Smt_Off"));
  TH1D* fhtmpY  = (TH1D*) gDirectory->FindObjectAny(Form("hPullY_Smt_Off"));
  TH1D* fhtmpZ  = (TH1D*) gDirectory->FindObjectAny(Form("hPullZ_Smt_Off"));
  TH1D* fhtmpW  = (TH1D*) gDirectory->FindObjectAny(Form("hPullT_Smt_Width"));
  TH1D* fhtmpWX = (TH1D*) gDirectory->FindObjectAny(Form("hPullX_Smt_Width"));
  TH1D* fhtmpWY = (TH1D*) gDirectory->FindObjectAny(Form("hPullY_Smt_Width"));
  TH1D* fhtmpWZ = (TH1D*) gDirectory->FindObjectAny(Form("hPullZ_Smt_Width"));


  gROOT->cd();
  if (NULL == fhtmp) {
    LOG(info) << Form("CbmTofFindTracks::LoadCalParameter: hPullT_Smt_Off")
              << " not found. ";
  } else {
    fhPullT_Smt_Off = (TH1D*) fhtmp->Clone();
  }

  if (NULL == fhtmpX) {
    LOG(info) << Form("CbmTofFindTracks::LoadCalParameter: hPullX_Smt_Off")
              << " not found. ";
  } else {
    fhPullX_Smt_Off = (TH1D*) fhtmpX->Clone();
  }

  if (NULL == fhtmpY) {
    LOG(info) << Form("CbmTofFindTracks::LoadCalParameter: hPullY_Smt_Off")
              << " not found. ";
  } else {
    fhPullY_Smt_Off = (TH1D*) fhtmpY->Clone();
  }

  if (NULL == fhtmpZ) {
    LOG(info) << Form("CbmTofFindTracks::LoadCalParameter: hPullZ_Smt_Off")
              << " not found. ";
  } else {
    fhPullZ_Smt_Off = (TH1D*) fhtmpZ->Clone();
  }

  if (NULL == fhtmpW) {
    LOG(info) << Form("CbmTofFindTracks::LoadCalParameter: hPullT_Smt_Width")
              << " not found. ";
  } else {
    if (fbUseSigCalib) fhPullT_Smt_Width = (TH1D*) fhtmpW->Clone();
  }

  if (NULL == fhtmpWX) {
    LOG(info) << Form("CbmTofFindTracks::LoadCalParameter: hPullX_Smt_Width")
              << " not found. ";
  } else {
    if (fbUseSigCalib) fhPullX_Smt_Width = (TH1D*) fhtmpWX->Clone();
  }

  if (NULL == fhtmpWY) {
    LOG(info) << Form("CbmTofFindTracks::LoadCalParameter: hPullY_Smt_Width")
              << " not found. ";
  } else {
    if (fbUseSigCalib) fhPullY_Smt_Width = (TH1D*) fhtmpWY->Clone();
  }

  if (NULL == fhtmpWZ) {
    LOG(info) << Form("CbmTofFindTracks::LoadCalParameter: hPullZ_Smt_Width")
              << " not found. ";
  } else {
    if (fbUseSigCalib) fhPullZ_Smt_Width = (TH1D*) fhtmpWZ->Clone();
  }

  fCalParFile->Close();

  Double_t nSmt = fMapRpcIdParInd.size();

  if (NULL == fhPullT_Smt_Off) {  // provide default TOffset histogram
    fhPullT_Smt_Off =
      new TH1F(Form("hPullT_Smt_Off"),
               Form("Tracklet PullT vs RpcInd ; RpcInd ; #DeltaT (ns)"),
               nSmt,
               0,
               nSmt);

    // Initialize Parameter
    if (fiCorMode == 3)  // hidden option, FIXME
      for (Int_t iDet = 0; iDet < nSmt; iDet++) {
        std::map<Int_t, Int_t>::iterator it;
        //it = fMapRpcIdParInd.find(iDet);
        for (it = fMapRpcIdParInd.begin(); it != fMapRpcIdParInd.end(); it++) {
          if (it->second == iDet) break;
        }
        LOG(debug1) << Form(" iDet %d -> iUniqueId ?  0x%08x, 0x%08x ",
                            iDet,
                            it->first,
                            it->second);
        Int_t iUniqueId          = it->first;
        CbmTofCell* fChannelInfo = fDigiPar->GetCell(iUniqueId);
        if (NULL != fChannelInfo) {
          Double_t dVal =
            0.;  // FIXME numeric constant in code, default for cosmic
          if (fiBeamCounter != iUniqueId)
            dVal =
              fChannelInfo->GetZ() * fTtTarg;  //  use calibration target value
          fhPullT_Smt_Off->SetBinContent(iDet + 1, dVal);
          LOG(info) << Form("Initialize det 0x%08x at %d, z=%f with TOff %6.2f",
                            iUniqueId,
                            iDet + 1,
                            fChannelInfo->GetZ(),
                            dVal);
        }
      }
  }

  if (NULL == fhPullT_Smt_Width) {  // provide default TWidth histogram
    fhPullT_Smt_Width =
      new TH1F(Form("hPullT_Smt_Width"),
               Form("Tracklet ResiT Width vs RpcInd ; RpcInd ; RMS(T) (ns)"),
               nSmt,
               0,
               nSmt);

    // Initialize Parameter
    for (Int_t iDet = 0; iDet < nSmt; iDet++) {
      fhPullT_Smt_Width->SetBinContent(iDet + 1, fSIGT);
    }
  }

  LOG(info) << "CbmTofFindTracks::LoadCalParameter: fhPullT_Smt_Off at "
            << fhPullT_Smt_Off;

  if (NULL == fhPullX_Smt_Off)  // provide default XOffset histogram
    fhPullX_Smt_Off =
      new TH1F(Form("hPullX_Smt_Off"),
               Form("Tracklet ResiX vs RpcInd ; RpcInd ; #DeltaX (cm)"),
               nSmt,
               0,
               nSmt);
  if (NULL == fhPullX_Smt_Width) {
    fhPullX_Smt_Width =
      new TH1F(Form("hPullX_Smt_Width"),
               Form("Tracklet ResiX Width vs RpcInd ; RpcInd ; RMS(X) (cm)"),
               nSmt,
               0,
               nSmt);
    // Initialize Parameter
    for (Int_t iDet = 0; iDet < nSmt; iDet++) {
      fhPullX_Smt_Width->SetBinContent(iDet + 1, fSIGX);
    }
  }

  if (NULL == fhPullY_Smt_Off)  // provide default YOffset histogram
    fhPullY_Smt_Off =
      new TH1F(Form("hPullY_Smt_Off"),
               Form("Tracklet ResiY vs RpcInd ; RpcInd ; #DeltaY (cm)"),
               nSmt,
               0,
               nSmt);
  if (NULL == fhPullY_Smt_Width) {
    fhPullY_Smt_Width =
      new TH1F(Form("hPullY_Smt_Width"),
               Form("Tracklet ResiY Width vs RpcInd ; RpcInd ; RMS(Y) (cm)"),
               nSmt,
               0,
               nSmt);
    // Initialize Parameter
    for (Int_t iDet = 0; iDet < nSmt; iDet++) {
      fhPullY_Smt_Width->SetBinContent(iDet + 1, fSIGY);
    }
  }

  if (NULL == fhPullZ_Smt_Off)  // provide default TOffset histogram
    fhPullZ_Smt_Off =
      new TH1F(Form("hPullZ_Smt_Off"),
               Form("Tracklet ResiZ vs RpcInd ; RpcInd ; #DeltaZ (cm)"),
               nSmt,
               0,
               nSmt);
  if (NULL == fhPullZ_Smt_Width) {
    fhPullZ_Smt_Width =
      new TH1F(Form("hPullZ_Smt_Width"),
               Form("Tracklet ResiZ Width vs RpcInd ; RpcInd ; RMS(Z) (cm)"),
               nSmt,
               0,
               nSmt);
    // Initialize Parameter
    for (Int_t iDet = 0; iDet < nSmt; iDet++) {
      fhPullZ_Smt_Width->SetBinContent(iDet + 1, fSIGZ);
    }
  }

  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
Bool_t CbmTofFindTracks::InitParameters() {
  // Initialize the TOF GeoHandler
  Bool_t isSimulation = kFALSE;
  Int_t iGeoVersion   = fGeoHandler->Init(isSimulation);
  if (k12b > iGeoVersion) {
    LOG(error) << "CbmTofFindTracks::InitParameters => Only compatible with "
                  "geometries after v12b !!!";
    return kFALSE;
  }

  LOG(info) << "CbmTofFindTracks::InitParameters: GeoVersion " << iGeoVersion;

  switch (iGeoVersion) {
    case k12b: fTofId = new CbmTofDetectorId_v12b(); break;
    case k14a: fTofId = new CbmTofDetectorId_v14a(); break;
    case k21a: fTofId = new CbmTofDetectorId_v21a(); break;
    default:
      LOG(fatal) << "CbmTofFindTracks::InitParameters: Invalid Detector ID "
                 << iGeoVersion;
  }

  // create digitization parameters from geometry file
  CbmTofCreateDigiPar* tofDigiPar =
    new CbmTofCreateDigiPar("TOF Digi Producer", "TOF task");
  LOG(info) << "Create DigiPar ";
  tofDigiPar->Init();

  return kTRUE;
}
// -----  SetParContainers -------------------------------------------------
void CbmTofFindTracks::SetParContainers() {
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  //  rtdb->getContainer("CbmGeoPassivePar");
  //   rtdb->getContainer("CbmGeoStsPar");
  //   rtdb->getContainer("CbmGeoTofPar");
  rtdb->getContainer("FairBaseParSet");
  //    rtdb->getContainer("CbmGeoPassivePar");
  // rtdb->getContainer("CbmGeoStsPar");
  // rtdb->getContainer("CbmGeoRichPar");
  rtdb->getContainer("CbmGeoTofPar");
  // rtdb->getContainer("CbmFieldPar");
  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));

  LOG(info) << "  CbmTofFindTracks::SetParContainers found "
            << fDigiPar->GetNrOfModules() << " cells ";

  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
}
// -------------------------------------------------------------------------

Bool_t CbmTofFindTracks::WriteHistos() {
  if (fiCorMode < 0) return kTRUE;

  LOG(info) << Form("CbmTofFindTracks::WriteHistos: %s, mode = %d",
                    fCalOutFileName.Data(),
                    fiCorMode);

  // Write histogramms to the file
  TDirectory* oldir = gDirectory;
  TFile* fHist      = new TFile(fCalOutFileName, "RECREATE");
  fHist->cd();
  const Double_t RMSmin = 0.03;  // in ns

  switch (fiCorMode) {
    case 0: {
      TProfile* htmp  = fhPullT_Smt->ProfileX();
      TH1D* htmp1D    = htmp->ProjectionX();
      TProfile* hTOff = fhTOff_HMul2->ProfileX();
      TH1D* hTOff1D   = hTOff->ProjectionX();

      Double_t nx = htmp1D->GetNbinsX();
      for (Int_t ix = 1; ix < nx; ix++) {
        Double_t dVal = 0;
        if (fhPullT_Smt_Off != NULL) {
          dVal = fhPullT_Smt_Off->GetBinContent(ix + 1);
        } else {
          fhPullT_Smt_Off = htmp1D;
        }
        TH1D* hTOff1DY =
          fhTOff_HMul2->ProjectionY(Form("_py%d", ix), ix + 1, ix + 1, "");
        Double_t dFMean = 0.;
        if (hTOff1DY->GetEntries() > 100) {
          //Double_t dMean=hTOff1DY->GetMean();
          Int_t iBmax  = hTOff1DY->GetMaximumBin();
          TAxis* xaxis = hTOff1DY->GetXaxis();
          Double_t dMean =
            xaxis->GetBinCenter(iBmax);  //X-value of bin with maximal content
          Double_t dLim = 1000.;         //1.5*hTOff1DY->GetRMS();
          TFitResultPtr fRes =
            hTOff1DY->Fit("gaus", "S", "", dMean - dLim, dMean + dLim);
          dFMean = fRes->Parameter(1);
        }
        dVal -= dFMean;
        LOG(info) << "Init  TOff " << ix << ": Old "
                  << fhPullT_Smt_Off->GetBinContent(ix + 1) << ", Cnts "
                  << hTOff1D->GetBinContent(ix + 1) << ", FitMean " << dFMean
                  << " -> " << dVal;
        fhPullT_Smt_Off->SetBinContent(ix + 1, dVal);
      }
    }

    break;

    case 1:  // correct mean deviation from fit (Pull)
    {
      TProfile* htmp  = fhPullT_Smt->ProfileX();
      TH1D* htmp1D    = htmp->ProjectionX();
      TProfile* hTOff = fhTOff_Smt->ProfileX();
      TH1D* hTOff1D   = hTOff->ProjectionX();

      if (fhPullT_Smt_Off != NULL) {
        Double_t nx = htmp1D->GetNbinsX();
        for (Int_t ix = 0; ix < nx; ix++) {
          Double_t dVal = fhPullT_Smt_Off->GetBinContent(ix + 1);
          dVal -= htmp1D->GetBinContent(ix + 1);

          LOG(debug1) << "Update hPullT_Smt_Off " << ix << ": "
                      << fhPullT_Smt_Off->GetBinContent(ix + 1) << " + "
                      << htmp1D->GetBinContent(ix + 1) << " + "
                      << hTOff1D->GetBinContent(ix + 1) << " -> " << dVal;
          fhPullT_Smt_Off->SetBinContent(ix + 1, dVal);
        }
      } else {
        LOG(warning)
          << "CbmTofFindTracks::WriteHistos: fhPullT_Smt_Off not found ";
      }
    }

    break;

    case 2:  // correct deviation from DeltaTt=0 expectation
    {
      TProfile* htmp  = fhPullT_Smt->ProfileX();
      TH1D* htmp1D    = htmp->ProjectionX();
      TProfile* hTOff = fhTOff_Smt->ProfileX();
      TH1D* hTOff1D   = hTOff->ProjectionX();

      if (fhPullT_Smt_Off != NULL) {
        Double_t nx = htmp1D->GetNbinsX();
        for (Int_t ix = 0; ix < nx; ix++) {
          Double_t dVal = fhPullT_Smt_Off->GetBinContent(ix + 1);
          dVal -= hTOff1D->GetBinContent(ix + 1);
          TH1D* hTOff1DY =
            fhTOff_Smt->ProjectionY(Form("_py%d", ix), ix + 1, ix + 1, "");
          Double_t dFMean = 0.;
          if (hTOff1DY->GetEntries() > 100) {
            //Double_t dMean=hTOff1DY->GetMean();
            Int_t iBmax  = hTOff1DY->GetMaximumBin();
            TAxis* xaxis = hTOff1DY->GetXaxis();
            Double_t dMean =
              xaxis->GetBinCenter(iBmax);  //X-value of bin with maximal content
            Double_t dLim = 1.5 * hTOff1DY->GetRMS();
            TFitResultPtr fRes =
              hTOff1DY->Fit("gaus", "S", "", dMean - dLim, dMean + dLim);
            Int_t iFitStatus = fRes;
            if (iFitStatus == 0) {  // check validity of fit
              dFMean = fRes->Parameter(1);
              dVal +=
                hTOff1D->GetBinContent(ix + 1);  //revert default correction
              dVal -= dFMean;
            }
            LOG(info) << "Update hPullT_Smt_Off Ind " << ix << ": Old "
                      << fhPullT_Smt_Off->GetBinContent(ix + 1) << ", Pull "
                      << htmp1D->GetBinContent(ix + 1) << ", Dev@Peak "
                      << hTOff1D->GetBinContent(ix + 1) << ", FitMean "
                      << dFMean << " -> " << dVal;
          } else {
            LOG(debug1) << "Update hPullT_Smt_Off " << ix
                        << ": insufficient counts: " << hTOff1DY->GetEntries();
          }
          fhPullT_Smt_Off->SetBinContent(ix + 1, dVal);
        }
      } else {
        LOG(warning)
          << "CbmTofFindTracks::WriteHistos: fhPullT_Smt_Off not found ";
      }
    } break;

    case 3:  // correct Time Offset from PullT, extract width
    {
      TProfile* htmp = fhPullT_Smt->ProfileX();
      TH1D* htmp1D   = htmp->ProjectionX();

      if (fhPullT_Smt_Off != NULL) {
        Double_t nx = htmp1D->GetNbinsX();
        for (Int_t ix = 0; ix < nx; ix++) {
          TH1D* hpy = fhPullT_Smt->ProjectionY("_py", ix + 1, ix + 1);
          if (hpy->GetEntries() > 100.) {
            Int_t iBmax  = hpy->GetMaximumBin();
            TAxis* xaxis = hpy->GetXaxis();
            Double_t dMean =
              xaxis->GetBinCenter(iBmax);  //X-value of bin with maximal content
            Double_t dRMS = TMath::Abs(hpy->GetRMS());
            Double_t dLim = 1.5 * dRMS;
            TFitResultPtr fRes =
              hpy->Fit("gaus", "S", "", dMean - dLim, dMean + dLim);
            Double_t dFMean = fRes->Parameter(1);

            Double_t dVal = fhPullT_Smt_Off->GetBinContent(ix + 1);
            dVal -= dFMean;
            TF1* fg              = hpy->GetFunction("gaus");
            Double_t dFMeanError = fg->GetParError(1);
            LOG(info) << "Update hPullT_Smt_Off3 Ind " << ix << ": "
                      << fhPullT_Smt_Off->GetBinContent(ix + 1) << " + "
                      << dFMean << ", Err " << dFMeanError << " -> " << dVal
                      << ", Width " << dRMS << ", Chi2 " << fg->GetChisquare();
            if (dFMeanError < 0.05) {  // FIXME: hardwired constant
              if (dRMS < RMSmin) dRMS = RMSmin;
              if (dRMS > fSIGT * 3.0) dRMS = fSIGT * 3.;
              if (fRpcAddr[ix]
                  != fiBeamCounter)  // don't correct beam counter position
                fhPullT_Smt_Off->SetBinContent(ix + 1, dVal);
              fhPullT_Smt_Width->SetBinContent(ix + 1, dRMS);
            }
          } else {
            LOG(debug1) << "Update hPullT_Smt_Off " << ix
                        << ": insufficient counts: " << hpy->GetEntries();
          }
        }
      } else {
        LOG(warning)
          << "CbmTofFindTracks::WriteHistos: fhPullT_Smt_Off not found ";
      }
    }

    break;

    case 4:  // correct mean deviation from fit (Pull), extract width for x direction
    {
      TProfile* htmp = fhPullX_Smt->ProfileX();
      TH1D* htmp1D   = htmp->ProjectionX();

      if (fhPullX_Smt_Off != NULL) {
        Double_t nx = htmp1D->GetNbinsX();
        for (Int_t ix = 0; ix < nx; ix++) {
          Double_t dVal = fhPullX_Smt_Off->GetBinContent(ix + 1);
          dVal -= htmp1D->GetBinContent(ix + 1);
          if (dVal < -3.)
            dVal =
              -3.;  // limit maximal shift in X, for larger values, change geometry file
          if (dVal > 3.) dVal = 3.;
          //if( fRpcAddr[ix] != fiBeamCounter )  // don't correct beam counter position
          fhPullX_Smt_Off->SetBinContent(ix + 1, dVal);

          TH1D* hpy = fhPullX_Smt->ProjectionY("_py", ix + 1, ix + 1);
          if (hpy->GetEntries() > 100.) {
            Double_t dRMS = TMath::Abs(hpy->GetRMS());
            if (dRMS < fSIGX * 0.5) dRMS = fSIGX * 0.5;
            if (dRMS > fSIGX * 3.0) dRMS = fSIGX * 3.;
            fhPullX_Smt_Width->SetBinContent(ix + 1, dRMS);

            LOG(info) << "Update hPullX_Smt_Off " << ix << ": "
                      << fhPullX_Smt_Off->GetBinContent(ix + 1) << " + "
                      << htmp1D->GetBinContent(ix + 1) << " -> " << dVal
                      << ", Width " << dRMS;
          }
        }
      } else {
        LOG(warning)
          << "CbmTofFindTracks::WriteHistos: fhPullX_Smt_Off not found ";
      }
    }

    break;

    case 5:  // correct mean deviation from fit (Pull), extract width for Y direction
    {
      TProfile* htmp = fhPullY_Smt->ProfileX();
      TH1D* htmp1D   = htmp->ProjectionX();

      if (fhPullY_Smt_Off != NULL) {
        Double_t nx = htmp1D->GetNbinsX();
        for (Int_t ix = 0; ix < nx; ix++) {
          Double_t dVal = fhPullY_Smt_Off->GetBinContent(ix + 1);
          dVal -= htmp1D->GetBinContent(ix + 1);
          //if( fRpcAddr[ix] != fiBeamCounter )  // don't correct beam counter position
          fhPullY_Smt_Off->SetBinContent(ix + 1, dVal);

          TH1D* hpy = fhPullY_Smt->ProjectionY("_py", ix + 1, ix + 1);
          if (hpy->GetEntries() > 100.) {
            Double_t dRMS = TMath::Abs(hpy->GetRMS());
            if (dRMS < fSIGY * 0.5) dRMS = 0.5 * fSIGY;
            if (dRMS > fSIGY * 3.0) dRMS = fSIGY * 3.;
            fhPullY_Smt_Width->SetBinContent(ix + 1, dRMS);

            LOG(debug1) << "Update hPullY_Smt_Off " << ix << ": "
                        << fhPullY_Smt_Off->GetBinContent(ix + 1) << " + "
                        << htmp1D->GetBinContent(ix + 1) << " -> " << dVal
                        << ", Width " << dRMS;
          }
        }
      } else {
        LOG(warning)
          << "CbmTofFindTracks::WriteHistos: fhPullY_Smt_Off not found ";
      }

    }

    break;

    case 6:  // correct mean deviation from fit (Pull), extract width
    {
      TProfile* htmp = fhPullZ_Smt->ProfileX();
      TH1D* htmp1D   = htmp->ProjectionX();

      if (fhPullZ_Smt_Off != NULL) {
        Double_t nx = htmp1D->GetNbinsX();
        for (Int_t ix = 0; ix < nx; ix++) {
          Double_t dVal = fhPullZ_Smt_Off->GetBinContent(ix + 1);
          dVal -= htmp1D->GetBinContent(ix + 1);
          fhPullZ_Smt_Off->SetBinContent(ix + 1, dVal);

          TH1D* hpy = fhPullZ_Smt->ProjectionY("_py", ix + 1, ix + 1);
          if (hpy->GetEntries() > 100.) {
            Double_t dRMS = TMath::Abs(hpy->GetRMS());

            LOG(debug1) << "Update hPullZ_Smt_Off " << ix << ": "
                        << fhPullZ_Smt_Off->GetBinContent(ix + 1) << " + "
                        << htmp1D->GetBinContent(ix + 1) << " -> " << dVal
                        << ", Width " << dRMS;
            if (dRMS < 1.5) dRMS = 1.5;
            fhPullZ_Smt_Width->SetBinContent(ix + 1, dRMS);
          }
        }
      } else {
        LOG(warning)
          << "CbmTofFindTracks::WriteHistos: fhPullZ_Smt_Off not found ";
      }

    } break;

    case 7:  // extract residual widthes in T, X, Y, Z
    {
      for (Int_t iStation = 0;
           iStation < static_cast<Int_t>(fMapRpcIdParInd.size());
           iStation++) {
        TH1D* hResidualT =
          fhPullT_Smt->ProjectionY("_py", iStation + 1, iStation + 1);
        TH1D* hResidualX =
          fhPullX_Smt->ProjectionY("_py", iStation + 1, iStation + 1);
        TH1D* hResidualY =
          fhPullY_Smt->ProjectionY("_py", iStation + 1, iStation + 1);
        TH1D* hResidualZ =
          fhPullZ_Smt->ProjectionY("_py", iStation + 1, iStation + 1);

        if (hResidualT->GetEntries() > 100.) {
          Double_t dRMS = TMath::Abs(hResidualT->GetRMS());

          if (dRMS < RMSmin) dRMS = RMSmin;
          if (dRMS > 3. * fSIGT) dRMS = 3. * fSIGT;

          fhPullT_Smt_Width->SetBinContent(iStation + 1, dRMS);
        }

        if (hResidualX->GetEntries() > 100.) {
          Double_t dRMS = TMath::Abs(hResidualX->GetRMS());

          if (dRMS < 0.5 * fSIGX) dRMS = 0.5 * fSIGX;
          if (dRMS > 3. * fSIGX) dRMS = 3. * fSIGX;

          fhPullX_Smt_Width->SetBinContent(iStation + 1, dRMS);
        }

        if (hResidualY->GetEntries() > 100.) {
          Double_t dRMS = TMath::Abs(hResidualY->GetRMS());

          if (dRMS < 0.5 * fSIGY) dRMS = 0.5 * fSIGY;
          if (dRMS > 3. * fSIGY) dRMS = 3. * fSIGY;

          fhPullY_Smt_Width->SetBinContent(iStation + 1, dRMS);
        }

        if (hResidualZ->GetEntries() > 100.) {
          Double_t dRMS = TMath::Abs(hResidualZ->GetRMS());

          if (dRMS < 1.5) dRMS = 1.5;

          fhPullZ_Smt_Width->SetBinContent(iStation + 1, dRMS);
        }
      }
    } break;

    case 10:  //correct mean deviation from TB - histo of station 0
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19: {
      Int_t iSt     = fiCorMode % 10;
      TString hname = Form("hPull%s_Station_%d", "TB", iSt);
      TH1* h1       = (TH1*) gROOT->FindObjectAny(hname);
      if (h1->GetEntries() > 100) {
        Double_t dFMean = h1->GetMean();
        Double_t dFLim  = 2.5 * h1->GetRMS();
        TFitResultPtr fRes =
          h1->Fit("gaus", "S", "", dFMean - dFLim, dFMean + dFLim);
        Double_t dDOff = fRes->Parameter(1);
        Double_t dSig  = fRes->Parameter(2);
        Int_t iRpcInd  = fMapRpcIdParInd[fMapStationRpcId[iSt]];
        Double_t dVal  = fhPullT_Smt_Off->GetBinContent(iRpcInd + 1);
        dVal -= dDOff;
        LOG(info) << "Update hPullT_Smt_OffP Ind " << iSt << ", Ind " << iRpcInd
                  << ": " << fhPullT_Smt_Off->GetBinContent(iRpcInd + 1)
                  << " - " << dDOff << " -> " << dVal << ", Width " << dSig;
        fhPullT_Smt_Off->SetBinContent(iRpcInd + 1, dVal);
        if (dSig < fSIGT * 0.5) dSig = 0.5 * fSIGT;
        if (dSig > fSIGT * 3.0) dSig = fSIGT * 3.;
        fhPullT_Smt_Width->SetBinContent(iRpcInd + 1, dSig);
      } else {
        LOG(info) << "CbmTofFindTracks::WriteHistos: Too few entries in histo "
                  << hname;
      }
    } break;

    default:;
  }

  if (NULL != fhPullT_Smt_Off) {
    // always extract residual widthes in T, X, Y, Z
    for (Int_t iStation = 0;
         iStation < static_cast<Int_t>(fMapRpcIdParInd.size());
         iStation++) {
      TH1D* hResidualT =
        fhPullT_Smt->ProjectionY("_py", iStation + 1, iStation + 1);
      TH1D* hResidualX =
        fhPullX_Smt->ProjectionY("_py", iStation + 1, iStation + 1);
      TH1D* hResidualY =
        fhPullY_Smt->ProjectionY("_py", iStation + 1, iStation + 1);
      TH1D* hResidualZ =
        fhPullZ_Smt->ProjectionY("_py", iStation + 1, iStation + 1);
      if (hResidualT->GetEntries() > 100.) {
        Double_t dRMS = TMath::Abs(hResidualT->GetRMS());
        if (dRMS < RMSmin) dRMS = RMSmin;
        if (dRMS > 3. * fSIGT) dRMS = 3. * fSIGT;
        fhPullT_Smt_Width->SetBinContent(iStation + 1, dRMS);
      }
      if (hResidualX->GetEntries() > 100.) {
        Double_t dRMS = TMath::Abs(hResidualX->GetRMS());
        if (dRMS < 0.5 * fSIGX) dRMS = 0.5 * fSIGX;
        if (dRMS > 3. * fSIGX) dRMS = 3. * fSIGX;
        fhPullX_Smt_Width->SetBinContent(iStation + 1, dRMS);
      }
      if (hResidualY->GetEntries() > 100.) {
        Double_t dRMS = TMath::Abs(hResidualY->GetRMS());
        if (dRMS < 0.5 * fSIGY) dRMS = 0.5 * fSIGY;
        if (dRMS > 3. * fSIGY) dRMS = 3. * fSIGY;
        fhPullY_Smt_Width->SetBinContent(iStation + 1, dRMS);
      }
      if (hResidualZ->GetEntries() > 100.) {
        Double_t dRMS = TMath::Abs(hResidualZ->GetRMS());
        if (dRMS < 0.1) dRMS = 0.1;
        if (dRMS > 1.0) dRMS = 1.;
        fhPullZ_Smt_Width->SetBinContent(iStation + 1, dRMS);
      }
    }

    fhPullT_Smt_Off->Write();
    fhPullX_Smt_Off->Write();
    fhPullY_Smt_Off->Write();
    fhPullZ_Smt_Off->Write();
    fhPullT_Smt_Width->Write();
    fhPullX_Smt_Width->Write();
    fhPullY_Smt_Width->Write();
    fhPullZ_Smt_Width->Write();
  }
  gDirectory->cd(oldir->GetPath());
  fHist->Close();

  return kTRUE;
}


// -----   Public method Exec   --------------------------------------------
void CbmTofFindTracks::Exec(Option_t* opt) {
  if (!fEventsColl) {
    //    fTofHitArray = (TClonesArray*)fTofHitArrayIn->Clone();
    fTofHitArray = (TClonesArray*) fTofHitArrayIn;
    ExecFind(opt);
  } else {
    Int_t iNbTrks = 0;
    fTrackArrayOut->Delete();  //Clear("C");
    for (Int_t iEvent = 0; iEvent < fEventsColl->GetEntriesFast(); iEvent++) {
      CbmEvent* tEvent = dynamic_cast<CbmEvent*>(fEventsColl->At(iEvent));
      LOG(debug) << "Process event " << iEvent << " with "
                 << tEvent->GetNofData(ECbmDataType::kTofHit) << " hits";

      if (fTofHitArray) fTofHitArray->Clear("C");
      Int_t iNbHits = 0;
      for (Int_t iHit = 0; iHit < tEvent->GetNofData(ECbmDataType::kTofHit);
           iHit++) {
        Int_t iHitIndex =
          static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofHit, iHit));
        CbmTofHit* tHit =
          dynamic_cast<CbmTofHit*>(fTofHitArrayIn->At(iHitIndex));
        new ((*fTofHitArray)[iNbHits++]) CbmTofHit(*tHit);
      }

      ExecFind(opt);

      // --- In event-by-event mode: copy tracks to output array and register them to event
      for (Int_t iTrk = 0; iTrk < fTrackArray->GetEntries(); iTrk++) {
        CbmTofTracklet* pTrk = (CbmTofTracklet*) fTrackArray->At(iTrk);
        new ((*fTrackArrayOut)[iNbTrks]) CbmTofTracklet(*pTrk);
        tEvent->AddData(ECbmDataType::kTofTrack, iNbTrks);
        iNbTrks++;
      }
      fTrackArray->Delete();
    }
  }
}

void CbmTofFindTracks::ExecFind(Option_t* /*opt*/) {
  fiEvent++;
  ResetStationsFired();
  if (NULL != fTofUHitArray) fTofUHitArray->Clear("C");
  if (NULL != fTrackArray) fTrackArray->Delete();  // reset

  // recalibrate hits and count trackable hits
  for (Int_t iHit = 0; iHit < fTofHitArray->GetEntries(); iHit++) {
    CbmTofHit* pHit = (CbmTofHit*) fTofHitArray->At(iHit);
    Int_t iDetId    = (pHit->GetAddress() & DetMask);

    Double_t dSIGX = GetSigX(iDetId);
    if (dSIGX == 0.) dSIGX = fSIGX;
    Double_t dSIGY = GetSigY(iDetId);
    if (dSIGY == 0.) dSIGY = fSIGY;
    Double_t dSIGZ = GetSigZ(iDetId);
    if (dSIGZ == 0.) dSIGZ = fSIGZ;
    TVector3 hitPosErr(dSIGX, dSIGY, dSIGZ);  // include positioning uncertainty
    pHit->SetPositionError(hitPosErr);

    Double_t dSIGT = GetSigT(iDetId);
    if (dSIGT == 0.) dSIGT = fSIGT;
    pHit->SetTimeError(dSIGT);

    if (fiBeamCounter > -1) {
      // set diamond positions to (0,0,0) to allow inclusion in straight line fit
      if ((iDetId & 0x0000F00F) == 0x00005006)  // modify diamond position
      {
        if (0. != fdBeamMomentumLab) {
          Double_t dTargetTimeOffset =
            pHit->GetZ() / fdBeamMomentumLab
            * TMath::Sqrt(TMath::Power(fdBeamMomentumLab, 2.)
                          + TMath::Power(0.938271998, 2.))
            / TMath::Ccgs() * 1.0e09;
          pHit->SetTime(pHit->GetTime() - dTargetTimeOffset);
        }

        TVector3 hitPos(0., 0., 0.);
        // TVector3 hitPos(pHit->GetX(), pHit->GetY(), 0.);
        // TVector3 hitPosErr(1.,1.,5.0);  // including positioning uncertainty
        pHit->SetPosition(hitPos);
        TVector3 hitPosErr0(1., 1., 1.);  // including positioning uncertainty
        pHit->SetPositionError(hitPosErr0);  //
      }
    }

    if (fbRemoveSignalPropagationTime) {
      Int_t iHitAddress   = pHit->GetAddress();
      Int_t iModuleType   = CbmTofAddress::GetSmType(iHitAddress);
      Int_t iModuleIndex  = CbmTofAddress::GetSmId(iHitAddress);
      Int_t iCounterIndex = CbmTofAddress::GetRpcId(iHitAddress);

      CbmTofCell* fChannelInfo = fDigiPar->GetCell(iHitAddress);

      Double_t dSignalPropagationTime =
        0.5
        * (fChannelInfo->GetSizey() >= fChannelInfo->GetSizex()
             ? fChannelInfo->GetSizey()
             : fChannelInfo->GetSizex())
        / fDigiBdfPar->GetSigVel(iModuleType, iModuleIndex, iCounterIndex);

      pHit->SetTime(pHit->GetTime() - dSignalPropagationTime);
    }

    // tune positions and times
    Double_t dTcor = 0.;
    if ((iDetId & 0x0000F00F)
        != 0x00005006) {  // do not modify diamond position
      Int_t iRpcInd = fMapRpcIdParInd[iDetId];
      if (fhPullT_Smt_Off != NULL) {
        dTcor = (Double_t) fhPullT_Smt_Off->GetBinContent(iRpcInd + 1);
        pHit->SetTime(pHit->GetTime() + dTcor);
      }
      if (fhPullX_Smt_Off != NULL) {
        Double_t dXcor = (Double_t) fhPullX_Smt_Off->GetBinContent(iRpcInd + 1);
        pHit->SetX(pHit->GetX() + dXcor);
      }
      if (fhPullY_Smt_Off != NULL) {
        Double_t dYcor = (Double_t) fhPullY_Smt_Off->GetBinContent(iRpcInd + 1);
        pHit->SetY(pHit->GetY() + dYcor);
      }
      if (fhPullZ_Smt_Off != NULL) {
        Double_t dZcor = (Double_t) fhPullZ_Smt_Off->GetBinContent(iRpcInd + 1);
        pHit->SetZ(pHit->GetZ() + dZcor);
      }
    }

    Int_t iSt = GetStationOfAddr(iDetId);
    MarkStationFired(iSt);

    LOG(debug) << Form(
      "Exec found Hit %02d, addr 0x%08x, sta %02d, HM %02d, X %6.2f(%3.2f) Y "
      "%6.2f(%3.2f)  Z %6.2f(%3.2f)  T %6.2f(%3.2f) (%6.2f)",
      iHit,
      pHit->GetAddress(),
      GetStationOfAddr(iDetId),
      fStationHMul[GetStationOfAddr(iDetId)],
      pHit->GetX(),
      pHit->GetDx(),
      pHit->GetY(),
      pHit->GetDy(),
      pHit->GetZ(),
      pHit->GetDz(),
      pHit->GetTime(),
      pHit->GetTimeError(),
      dTcor);
  }

  LOG(debug) << Form("CbmTofFindTracks::Exec NStationsFired %d > %d Min ?",
                     GetNStationsFired(),
                     GetMinNofHits());

  if (GetNStationsFired() < GetMinNofHits()) {
    fInspectEvent = kFALSE;  // mark event as non trackable
  } else
    fInspectEvent = kTRUE;

  CheckMaxHMul();
  // resort Hit array with respect to time, FIXME danger: links to digis become  invalid (???, check!!!)
  // fTofHitArray->Sort(fTofHitArray->GetEntries());  // feature not available

  if (fInspectEvent && fNTofStations > 1) {
    fStart.Set();
    //fTrackArray->Clear("C+C");
    fNofTracks = fFinder->DoFind(fTofHitArray, fTrackArray);
    //  fTrackArray->Compress();
    fStop.Set();
    fdTrackingTime = fStop.GetSec() - fStart.GetSec()
                     + (fStop.GetNanoSec() - fStart.GetNanoSec()) / 1e9;

    LOG(debug) << Form("CbmTofFindTracks::Exec found %d Tracklets in %f sec",
                       fTrackArray->GetEntriesFast(),
                       fdTrackingTime);

    FindVertex();

    FillHistograms();
  }

  FillUHits();  // put unused hits into TClonesArray
}
// -------------------------------------------------------------------------


// -----   Public method Finish   ------------------------------------------
void CbmTofFindTracks::Finish() {
  if (fiEvent < 1000) return;  // preserve calibration histos in event display
  if (fiCalOpt > 0) fTofCalibrator->UpdateCalHist(fiCalOpt);
  WriteHistos();

  LOG(info) << Form(" CbmTofFindTracks::Finished  ");
}
// -------------------------------------------------------------------------

void CbmTofFindTracks::CreateHistograms() {

  TDirectory* oldir =
    gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gROOT
    ->cd();  // <= To prevent histos from being sucked in by the param file of the TRootManager !

  // define histos here

  Double_t nSmt = fMapRpcIdParInd.size();
  LOG(info) << Form(
    " CbmTofFindTracks::CreateHistograms for %d counters, %d stations ",
    (Int_t) nSmt,
    fNTofStations);

  fhTrklMul = new TH1F(
    Form("hTrklMul"), Form("Tracklet Multiplicity; MulTracklet"), 100, 0, 100);

  fhAllHitsStation = new TH1F(Form("hAllHitsStation"),
                              Form("Reconstructed Hits; Station #"),
                              fNTofStations,
                              0,
                              fNTofStations);
  fhAllHitsSmTypes = new TH1F(
    Form("hAllHitsSmTypes"), Form("Reconstructed Hits; SmType #"), 10, 0, 10);

  fhUsedHitsStation =
    new TH1F(Form("hUsedHitsStation"),
             Form("Used (HMul>2) / Reconstructed Hits; Station #"),
             fNTofStations,
             0,
             fNTofStations);

  fhTrklChi2 = new TH2F(Form("hTrklChi2"),
                        Form("Tracklet Chi;  HMul_{Tracklet}; #chi"),
                        8,
                        2,
                        10,
                        100,
                        0,
                        ((CbmTofTrackFinderNN*) fFinder)->GetChiMaxAccept());

  fhTrackingTimeNhits = new TH2F(Form("hTrackingTimeNhits"),
                                 Form("Tracking Time; NHits; #Deltat (s)"),
                                 100,
                                 0,
                                 200,
                                 50,
                                 0,
                                 0.1);

  fhTrklMulNhits = new TH2F(Form("hTrklMulNhits"),
                            Form("Tracklet Multiplicity; NHits; NTracklet"),
                            150,
                            0,
                            150,
                            25,
                            0,
                            25);

  fhTrklMulMaxMM = new TH2F(Form("hTrklMulMaxMax-1"),
                            Form("Tracklet Multiplicity; TMulMax; TMulMax-1"),
                            10,
                            0,
                            10,
                            10,
                            0,
                            10);
  fhTrklMul3D    = new TH3F(Form("hTrklMul3D"),
                         Form("Tracklet Multiplicities; TMul3; TMul4; TMul5"),
                         10,
                         0,
                         10,
                         10,
                         0,
                         10,
                         10,
                         0,
                         10);

  fhTrklHMul =
    new TH2F(Form("hTrklHMul"),
             Form("Tracklet Hit - Multiplicity; HMul_{Tracklet}; Mul_{HMul}"),
             8,
             2,
             10,
             20,
             0,
             20);
  fhTrklZ0xHMul =
    new TH2F(Form("hTrklZ0xHMul"),
             Form("Tracklet Z0x vs. Hit - Multiplicity; HMul_{Tracklet}; Z0x"),
             8,
             2,
             10,
             100,
             -500,
             500);
  fhTrklZ0yHMul =
    new TH2F(Form("hTrklZ0yHMul"),
             Form("Tracklet Z0y vs. Hit - Multiplicity; HMul_{Tracklet}; Z0y"),
             8,
             2,
             10,
             100,
             -300,
             300);

  fhTrklTxHMul =
    new TH2F(Form("hTrklTxHMul"),
             Form("Tracklet Tx vs. Hit - Multiplicity; HMul_{Tracklet}; Tx"),
             8,
             2,
             10,
             100,
             -0.65,
             0.65);

  fhTrklTyHMul =
    new TH2F(Form("hTrklTyHMul"),
             Form("Tracklet Ty vs. Hit - Multiplicity; HMul_{Tracklet}; Ty"),
             8,
             2,
             10,
             100,
             -0.65,
             0.65);
  Double_t TTMAX = 0.2;
  fhTrklTtHMul =
    new TH2F(Form("hTrklTtHMul"),
             Form("Tracklet Tt vs. Hit - Multiplicity; HMul_{Tracklet}; Tt"),
             8,
             2,
             10,
             100,
             -TTMAX,
             TTMAX);
  fhTrklVelHMul = new TH2F(
    Form("hTrklVelHMul"),
    Form("Tracklet Vel vs. Hit - Multiplicity; HMul_{Tracklet}; v (cm/ns)"),
    8,
    2,
    10,
    100,
    0.,
    50.);
  fhTrklT0HMul =
    new TH2F(Form("hTrklT0HMul"),
             Form("Tracklet T0 vs. Hit - Multiplicity; HMul_{Tracklet}; T0"),
             8,
             2,
             10,
             100,
             -0.5,
             0.5);

  fhTrklT0Mul = new TH2F(
    Form("hTrklT0Mul"),
    Form(
      "Tracklet #DeltaT0 vs. Trkl - Multiplicity; Mul_{Tracklet}; #Delta(T0)"),
    10,
    0,
    10,
    100,
    -2.,
    2.);
  fhTrklDT0SmMis = new TH2F(
    Form("hTrklDT0SmMis"),
    Form("Tracklet DeltaT0 vs. Trkl - ID; SmType_{missed}; #Delta(T0)"),
    10,
    0,
    10,
    100,
    -2.,
    2.);
  fhTrklDT0StMis2 = new TH2F(
    Form("hTrklDT0SmMis2"),
    Form("Tracklet DeltaT0 vs. Station - ID; St2_{missed}; #Delta(T0)"),
    50,
    0,
    50,
    100,
    -2.,
    2.);

  Double_t X0MAX = 40.;
  fhTrklXY0_0 =
    new TH2F(Form("hTrklXY0_0"),
             Form("Tracklet XY at z=0 for hmulmax ; x (cm); y (cm)"),
             100,
             -X0MAX,
             X0MAX,
             100,
             -X0MAX,
             X0MAX);
  fhTrklXY0_1 =
    new TH2F(Form("hTrklXY0_1"),
             Form("Tracklet XY at z=0 for hmulmax-1 ; x (cm); y (cm)"),
             100,
             -X0MAX * 2.,
             X0MAX * 2.,
             100,
             -X0MAX * 2.,
             X0MAX * 2.);
  fhTrklXY0_2 =
    new TH2F(Form("hTrklXY0_2"),
             Form("Tracklet XY at z=0 for hmulmax-2 ; x (cm); y (cm)"),
             100,
             -X0MAX * 3.,
             X0MAX * 3.,
             100,
             -X0MAX * 3.,
             X0MAX * 3.);

  Double_t DT0MAX = 5.;
  if (fT0MAX == 0) fT0MAX = DT0MAX;
  fhPullT_Smt =
    new TH2F(Form("hPullT_Smt"),
             Form("Tracklet ResiT vs RpcInd ; RpcInd ; #DeltaT (ns)"),
             nSmt,
             0,
             nSmt,
             501,
             -fT0MAX,
             fT0MAX);
  Double_t DX0MAX = 5.;
  fhPullX_Smt =
    new TH2F(Form("hPullX_Smt"),
             Form("Tracklet ResiX vs RpcInd ; RpcInd ; #DeltaX (cm)"),
             nSmt,
             0,
             nSmt,
             100,
             -DX0MAX,
             DX0MAX);
  Double_t DY0MAX = 5.;
  fhPullY_Smt =
    new TH2F(Form("hPullY_Smt"),
             Form("Tracklet ResiY vs RpcInd ; RpcInd ; #DeltaY (cm)"),
             nSmt,
             0,
             nSmt,
             100,
             -DY0MAX,
             DY0MAX);
  Double_t DZ0MAX = 20.;
  fhPullZ_Smt =
    new TH2F(Form("hPullZ_Smt"),
             Form("Tracklet ResiZ vs RpcInd ; RpcInd ; #DeltaZ (cm)"),
             nSmt,
             0,
             nSmt,
             100,
             -DZ0MAX,
             DZ0MAX);

  fhTOff_Smt   = new TH2F(Form("hTOff_Smt"),
                        Form("Tracklet TOff; RpcInd ; TOff (ns)"),
                        nSmt,
                        0,
                        nSmt,
                        501,
                        -fT0MAX,
                        fT0MAX);
  fhTOff_HMul2 = new TH2F(Form("hTOff_HMul2"),
                          Form("Tracklet TOff(HMul2); RpcInd ; TOff (ns)"),
                          nSmt,
                          0,
                          nSmt,
                          500,
                          -fT0MAX,
                          fT0MAX);

  Double_t DTTMAX = 0.09;
  fhDeltaTt_Smt   = new TH2F(Form("hDeltaTt_Smt"),
                           Form("Tracklet DeltaTt; RpcInd ; #DeltaTt (ns/cm)"),
                           nSmt,
                           0,
                           nSmt,
                           100,
                           -DTTMAX,
                           DTTMAX);

  vhPullX.resize(fNTofStations);
  vhPullY.resize(fNTofStations);
  vhPullZ.resize(fNTofStations);
  vhPullT.resize(fNTofStations);
  vhPullTB.resize(fNTofStations);
  vhResidualTBWalk.resize(fNTofStations);
  vhResidualYWalk.resize(fNTofStations);
  vhXY_AllTracks.resize(fNTofStations);
  vhXY_AllStations.resize(fNTofStations);
  vhXY_MissedStation.resize(fNTofStations);
  vhXY_DX.resize(fNTofStations);
  vhXY_DY.resize(fNTofStations);
  vhXY_DT.resize(fNTofStations);
  vhXY_TOT.resize(fNTofStations);
  vhXY_CSZ.resize(fNTofStations);
  vhUDXDY_DT.resize(fNTofStations);
  vhUCDXDY_DT.resize(fNTofStations);

  for (Int_t iSt = 0; iSt < fNTofStations; iSt++) {
    vhPullX[iSt]          = new TH1F(Form("hPullX_Station_%d", iSt),
                            Form("hResiX_Station_%d;  #DeltaX (cm)", iSt),
                            99,
                            -DX0MAX,
                            DX0MAX);
    vhPullY[iSt]          = new TH1F(Form("hPullY_Station_%d", iSt),
                            Form("hResiY_Station_%d;  #DeltaY (cm)", iSt),
                            99,
                            -DY0MAX,
                            DY0MAX);
    vhPullZ[iSt]          = new TH1F(Form("hPullZ_Station_%d", iSt),
                            Form("hResiZ_Station_%d;  #DeltaZ (cm)", iSt),
                            99,
                            -50.,
                            50.);
    vhPullT[iSt]          = new TH1F(Form("hPullT_Station_%d", iSt),
                            Form("hResiT_Station_%d;  #DeltaT (ns)", iSt),
                            59,
                            -fT0MAX,
                            fT0MAX);
    vhPullTB[iSt]         = new TH1F(Form("hPullTB_Station_%d", iSt),
                             Form("hResiTB_Station_%d;  #DeltaT (ns)", iSt),
                             59,
                             -1.25 * fT0MAX,
                             1.25 * fT0MAX);
    const Double_t TOTmax = 50.;
    vhResidualTBWalk[iSt] =
      new TH2F(Form("hResidualTBWalk_Station_%d", iSt),
               Form("hResidualTBWalk_Station_%d;  #DeltaT (ns)", iSt),
               TOTmax,
               0.,
               TOTmax,
               59,
               -1.25 * fT0MAX,
               1.25 * fT0MAX);
    vhResidualYWalk[iSt] =
      new TH2F(Form("hResidualYWalk_Station_%d", iSt),
               Form("hResidualYWalk_Station_%d;  #DeltaT (ns)", iSt),
               TOTmax,
               0.,
               TOTmax,
               59,
               -DY0MAX,
               DY0MAX);
    Double_t XSIZ            = 16.;
    Int_t Nbins              = 32.;
    CbmTofCell* fChannelInfo = fDigiPar->GetCell(fMapStationRpcId[iSt]);
    if (NULL == fChannelInfo) {
      LOG(fatal) << "Geometry for station " << iSt << ", Rpc "
                 << fMapStationRpcId[iSt] << " not defined ";
      return;
    }
    Int_t NbinsX = 2 * XSIZ / fChannelInfo->GetSizex();
    vhXY_AllTracks[iSt] =
      new TH2F(Form("hXY_AllTracks_%d", iSt),
               Form("hXY_AllTracks_%d;  x(cm); y (cm)", iSt),
               NbinsX,
               -XSIZ,
               XSIZ,
               Nbins,
               -XSIZ,
               XSIZ);
    vhXY_AllStations[iSt] =
      new TH2F(Form("hXY_AllStations_%d", iSt),
               Form("hXY_AllStations_%d;  x(cm); y (cm)", iSt),
               NbinsX,
               -XSIZ,
               XSIZ,
               Nbins,
               -XSIZ,
               XSIZ);
    vhXY_MissedStation[iSt] =
      new TH2F(Form("hXY_MissedStation_%d", iSt),
               Form("hXY_MissedStation_%d;  x(cm); y (cm)", iSt),
               NbinsX,
               -XSIZ,
               XSIZ,
               Nbins,
               -XSIZ,
               XSIZ);
    vhXY_DX[iSt] =
      new TH3F(Form("hXY_DX_%d", iSt),
               Form("hXY_DX_%d;  x(cm); y (cm); #DeltaX (cm)", iSt),
               NbinsX,
               -XSIZ,
               XSIZ,
               Nbins,
               -XSIZ,
               XSIZ,
               Nbins,
               -2.,
               2.);
    vhXY_DY[iSt] =
      new TH3F(Form("hXY_DY_%d", iSt),
               Form("hXY_DY_%d;  x(cm); y (cm); #DeltaY (cm)", iSt),
               NbinsX,
               -XSIZ,
               XSIZ,
               Nbins,
               -XSIZ,
               XSIZ,
               Nbins,
               -2.,
               2.);
    vhXY_DT[iSt] =
      new TH3F(Form("hXY_DT_%d", iSt),
               Form("hXY_DT_%d;  x(cm); y (cm); #DeltaT (ns)", iSt),
               NbinsX,
               -XSIZ,
               XSIZ,
               Nbins,
               -XSIZ,
               XSIZ,
               Nbins,
               -0.5,
               0.5);
    vhXY_TOT[iSt] =
      new TH3F(Form("hXY_TOT_%d", iSt),
               Form("hXY_TOT_%d;  x(cm); y (cm); TOT (a.u.)", iSt),
               NbinsX,
               -XSIZ,
               XSIZ,
               Nbins,
               -XSIZ,
               XSIZ,
               Nbins,
               0.,
               10.);
    vhXY_CSZ[iSt]    = new TH3F(Form("hXY_CSZ_%d", iSt),
                             Form("hXY_CSZ_%d;  x(cm); y (cm); CSZ ()", iSt),
                             NbinsX,
                             -XSIZ,
                             XSIZ,
                             Nbins,
                             -XSIZ,
                             XSIZ,
                             6,
                             1.,
                             7.);
    vhUDXDY_DT[iSt]  = new TH3F(Form("hUDXDY_DT_%d", iSt),
                               Form("Unused missing hit - DXDY_DT_%d;  #Deltax "
                                    "(cm); #Deltay (cm); #DeltaT (ns)",
                                    iSt),
                               11,
                               -3.,
                               3.,
                               11,
                               -3.,
                               3.,
                               101,
                               -50.,
                               50.);
    vhUCDXDY_DT[iSt] = new TH3F(Form("hUCDXDY_DT_%d", iSt),
                                Form("Unused close hit - DXDY_DT_%d;  #Deltax "
                                     "(cm); #Deltay (cm); #DeltaT (ns)",
                                     iSt),
                                11,
                                -3.,
                                3.,
                                11,
                                -3.,
                                3.,
                                101,
                                -50.,
                                50.);
  }


  // vertex histrograms
  Double_t NNORM = 40.;
  fhVTXNorm      = new TH1F(Form("hVTXNorm"),
                       Form("Vertex Normalisation; #_{TrackletHits}"),
                       NNORM,
                       0,
                       NNORM);
  fhVTX_XY0      = new TH2F(Form("hVTX_XY0"),
                       Form("Vertex XY at z=0  ; x (xm); y (cm)"),
                       100,
                       -X0MAX,
                       X0MAX,
                       100,
                       -X0MAX,
                       X0MAX);
  fhVTX_DT0_Norm =
    new TH2F(Form("hVTX_DT0_Norm"),
             Form("Vertex #DeltaT at z=0  ; #_{TrackletHits}; #DeltaT (ns)"),
             NNORM,
             0,
             NNORM,
             100,
             -2.,
             2.);

  gDirectory->cd(
    oldir
      ->GetPath());  // <= To prevent histos from being sucked in by the param file of the TRootManager!
}

void CbmTofFindTracks::FindVertex() {
  fVTX_T   = 0.;  //reset
  fVTX_X   = 0.;
  fVTX_Y   = 0.;
  fVTX_Z   = 0.;
  fVTXNorm = 0.;

  for (Int_t iTrk = 0; iTrk < fTrackArray->GetEntries(); iTrk++) {
    CbmTofTracklet* pTrk = (CbmTofTracklet*) fTrackArray->At(iTrk);
    if (NULL == pTrk) continue;
    Double_t w = pTrk->GetNofHits();
    LOG(debug1) << Form(
      "CbmTofFindTracks::FindVertex: N %3.0f, w %3.0f, min %d",
      fVTXNorm,
      w,
      fMinNofHits);

    if (w > (Double_t)
          fMinNofHits) {  // for further analysis request minimum number of hits
      fVTXNorm += w;
      fVTX_T += w * pTrk->GetFitT(0.);
      fVTX_X += w * pTrk->GetFitX(0.);
      fVTX_Y += w * pTrk->GetFitY(0.);
    }
  }
  if (fVTXNorm > 0.) {
    fVTX_T /= fVTXNorm;
    fVTX_X /= fVTXNorm;
    fVTX_Y /= fVTXNorm;
    fVTX_Z /= fVTXNorm;
  }
  LOG(debug) << Form(
    "CbmTofFindTracks::FindVertex: N %3.0f, T %6.2f, X=%6.2f, Y=%6.2f Z=%6.2f ",
    fVTXNorm,
    fVTX_T,
    fVTX_X,
    fVTX_Y,
    fVTX_Z);
}

static Int_t iWarnNotDefined = 0;

void CbmTofFindTracks::FillHistograms() {
  // Locate reference ("beam counter") hit
  CbmTofHit* pRefHit  = NULL;
  Double_t RefMinTime = 1.E300;
  for (Int_t iHit = 0; iHit < fTofHitArray->GetEntries();
       iHit++) {  // loop over Hits
    CbmTofHit* pHit = (CbmTofHit*) fTofHitArray->At(iHit);
    Int_t iAddr     = (pHit->GetAddress() & DetMask);
    if (fiBeamCounter != -1) {
      if (iAddr == fiBeamCounter)
        if (pHit->GetTime() < RefMinTime) {
          pRefHit    = pHit;
          RefMinTime = pRefHit->GetTime();
        }
    } else {  // take earliest hit as reference
      if (pHit->GetTime() < RefMinTime) {
        pRefHit    = pHit;
        RefMinTime = pRefHit->GetTime();
      }
    }
  }
  if (fiBeamCounter != -1 && NULL == pRefHit) return;

  std::vector<Int_t> HMul;
  HMul.resize(fNTofStations + 1);
  //  HMul.clear();

  fhTrklMul->Fill(fTrackArray->GetEntries());

  Int_t iTMul = 0;
  for (Int_t iTrk = 0; iTrk < fTrackArray->GetEntries(); iTrk++) {
    CbmTofTracklet* pTrk = (CbmTofTracklet*) fTrackArray->At(iTrk);
    if (NULL == pTrk) continue;
    if (pTrk->GetNofHits() > fNTofStations) {
      LOG(error) << "CbmTofFindTracks::FillHistograms: more hits ("
                 << pTrk->GetNofHits() << ") than stations (" << fNTofStations
                 << ")";
      continue;
    }

    HMul[pTrk->GetNofHits()]++;

    if (pTrk->GetNofHits() >= 2) {  // initial offset calibration
      //Int_t iH0  = pTrk->GetStationHitIndex(fMapStationRpcId[0]); // Hit index for station 0 (Diamond)
      //if(iH0<0) continue;                                         // Station 0 not part of tracklet
      //      Int_t iDetId0 = pTrk->GetTofDetIndex(0);                    // DetId of 1. Hit (FU) not used
      //      Int_t iSt0 = GetStationOfAddr(iDetId0);                       // Station of 1. Hit (FU) not used
      CbmTofHit* pHit0 = pTrk->GetTofHitPointer(0);
      Double_t dTRef0  = pHit0->GetTime() - pHit0->GetR() * fTtTarg;

      for (Int_t iH = 1; iH < pTrk->GetNofHits(); iH++) {
        Int_t iDetId    = pTrk->GetTofDetIndex(iH);  // DetId of iH. Hit
        CbmTofHit* pHit = pTrk->GetTofHitPointer(iH);
        Int_t iSt       = GetStationOfAddr(iDetId);  // Station of 1. Hit
        Double_t dTOff  = pHit->GetTime() - pHit->GetR() * fTtTarg - dTRef0;
        LOG(debug1) << Form("<D> CbmTofFindTracks::FillHistograms: iDetId1 "
                            "0x%08x, iST1 = %d with dTOff %f at RpcInd %d",
                            iDetId,
                            iSt,
                            dTOff,
                            fMapRpcIdParInd[fMapStationRpcId[iSt]]);
        fhTOff_HMul2->Fill((Double_t) fMapRpcIdParInd[fMapStationRpcId[iSt]],
                           dTOff);
      }  // loop over tracklets' hits
    }

    if (
      pTrk->GetNofHits()
      > fMinNofHits) {  // for further analysis request at least 3 matched hits
      iTMul++;
      fhTrklChi2->Fill(pTrk->GetNofHits(), pTrk->GetChiSq());

      if (fiCalOpt > 0) fTofCalibrator->FillCalHist(pTrk);

      CbmTofTrackletParam* tPar = pTrk->GetTrackParameter();
      Double_t dTt              = pTrk->GetTt();
      LOG(debug) << Form("Trk %d info: Lz=%6.2f Z0x=%6.2f Z0y=%6.2f Tt=%6.4f",
                         iTrk,
                         tPar->GetLz(),
                         pTrk->GetZ0x(),
                         pTrk->GetZ0y(),
                         dTt)
                 << tPar->ToString();

      fhTrklZ0xHMul->Fill(pTrk->GetNofHits(), pTrk->GetFitX(0.));
      fhTrklZ0yHMul->Fill(pTrk->GetNofHits(), pTrk->GetFitY(0.));
      fhTrklTxHMul->Fill(pTrk->GetNofHits(), tPar->GetTx());
      fhTrklTyHMul->Fill(pTrk->GetNofHits(), tPar->GetTy());
      fhTrklTtHMul->Fill(pTrk->GetNofHits(), dTt);

      switch (GetNReqStations() - pTrk->GetNofHits()) {
        case 0:  // max hit number
          fhTrklXY0_0->Fill(pTrk->GetFitX(0.), pTrk->GetFitY(0.));
          break;
        case 1: fhTrklXY0_1->Fill(pTrk->GetFitX(0.), pTrk->GetFitY(0.)); break;
        case 2: fhTrklXY0_2->Fill(pTrk->GetFitX(0.), pTrk->GetFitY(0.)); break;
        default:;
      }

      if (dTt > 0.)
        for (Int_t iSt = 0; iSt < fNTofStations; iSt++) {
          Int_t iH = pTrk->GetStationHitIndex(
            fMapStationRpcId[iSt]);  // Station Hit index
          if (iH < 0) continue;      // Station not part of tracklet
          fhUsedHitsStation->Fill(iSt);

          if (pTrk->GetNofHits() < GetNReqStations())
            continue;  // fill Pull histos only for complete tracks
          CbmTofHit* pHit = (CbmTofHit*) fTofHitArray->At(iH);
          //if (0 == fMapStationRpcId[iSt]) pHit->SetTime(pTrk->GetT0());  // set time of fake hit, abandoned
          /*
          cout << " -D- CbmTofFindTracks::FillHistograms: "<< iSt <<", "
            <<fMapStationRpcId[iSt]<<", "<< iH <<", "<< iH0 <<", "<<pHit->ToString() << endl; 
          */
          Double_t dDZ =
            pHit->GetZ() - tPar->GetZ();  // z- Distance to reference point
          Double_t dDX =
            pHit->GetX()
            - pTrk->GetFitX(
              pHit->GetZ());  // - tPar->GetX() - tPar->GetTx()*dDZ;
          Double_t dDY =
            pHit->GetY() - pTrk->GetFitY(pHit->GetZ());  // - tPar->GetTy()*dDZ;
          Double_t dDT =
            pHit->GetTime()
            - pTrk->GetFitT(
              pHit->GetZ());  // pTrk->GetTdif(fMapStationRpcId[iSt]);
          Double_t dDTB =
            fTrackletTools->GetTdif(pTrk,
                                    fMapStationRpcId[iSt],
                                    pHit);  // ignore pHit in calc of reference
          Double_t dTOT = pHit->GetCh() / 10.;  // misuse of channel field

          Double_t dZZ = pHit->GetZ() - tPar->GetZy(pHit->GetY());
          LOG(debug) << Form(
            "  St %d Id 0x%08x Hit %2d, Z %6.2f - DX %6.2f, DY %6.2f, "
            "Z %6.2f, DT %6.2f, %6.2f, ZZ %6.2f, Tt %6.4f ",
            iSt,
            fMapStationRpcId[iSt],
            iH,
            pHit->GetZ(),
            dDX,
            dDY,
            dDZ,
            dDT,
            dDTB,
            dZZ,
            dTt) << tPar->ToString();

          vhPullX[iSt]->Fill(dDX);
          vhPullY[iSt]->Fill(dDY);
          vhPullZ[iSt]->Fill(dZZ);
          vhPullT[iSt]->Fill(dDT);
          vhPullTB[iSt]->Fill(dDTB);
          vhResidualTBWalk[iSt]->Fill(dTOT, dDTB);
          vhResidualYWalk[iSt]->Fill(dTOT, dDY);

          fhPullT_Smt->Fill((Double_t) fMapRpcIdParInd[fMapStationRpcId[iSt]],
                            dDT);
          fhPullX_Smt->Fill((Double_t) fMapRpcIdParInd[fMapStationRpcId[iSt]],
                            dDX);
          fhPullY_Smt->Fill((Double_t) fMapRpcIdParInd[fMapStationRpcId[iSt]],
                            dDY);
          /*
          fhPullT_Smt->Fill((Double_t)fMapRpcIdParInd[fMapStationRpcId[iSt]], fTrackletTools->GetTdif(pTrk,fMapStationRpcId[iSt], pHit) );  
          fhPullX_Smt->Fill((Double_t)fMapRpcIdParInd[fMapStationRpcId[iSt]], fTrackletTools->GetXdif(pTrk,fMapStationRpcId[iSt], pHit) );  
          fhPullY_Smt->Fill((Double_t)fMapRpcIdParInd[fMapStationRpcId[iSt]], fTrackletTools->GetYdif(pTrk,fMapStationRpcId[iSt], pHit) );
          */
          fhPullZ_Smt->Fill((Double_t) fMapRpcIdParInd[fMapStationRpcId[iSt]],
                            dZZ);

          Double_t dDeltaTt = dTt - fTtTarg;
          fhDeltaTt_Smt->Fill((Double_t) fMapRpcIdParInd[fMapStationRpcId[iSt]],
                              dDeltaTt);
          //XXX use BRef as Referenz!!!
          if (pRefHit != NULL) {
            Double_t dTOff =
              dDeltaTt *  //pHit->GetR();
              TMath::Sqrt(TMath::Power(pHit->GetX() - pRefHit->GetX(), 2)
                          + TMath::Power(pHit->GetY() - pRefHit->GetY(), 2)
                          + TMath::Power(pHit->GetZ() - pRefHit->GetZ(), 2))
              * TMath::Sign(1, pHit->GetZ() - pRefHit->GetZ());
            fhTOff_Smt->Fill((Double_t) fMapRpcIdParInd[fMapStationRpcId[iSt]],
                             dTOff);
          }
        }

      // extrapolation of tracklet to vertex @ z=0
      //      FairTrackParam paramExtr;
      //      fFitter->Extrapolate(pTrk->GetParamFirst(),0.,&paramExtr);
    }  // condition on NofHits>2 end

    // monitoring of tracklet hits with  selected velocities from reference counters
    if (TMath::Abs(pTrk->GetRefVel((UInt_t)(fNReqStations - 1)) - fdRefVelMean)
        < fdRefDVel) {
      fhTrklVelHMul->Fill(pTrk->GetNofHits(), 1. / pTrk->GetTt());
      for (Int_t iH = 0; iH < pTrk->GetNofHits(); iH++) {
        CbmTofHit* pHit          = pTrk->GetTofHitPointer(iH);
        Int_t iChId              = pHit->GetAddress();
        CbmTofCell* fChannelInfo = fDigiPar->GetCell(iChId);
        if (NULL == fChannelInfo) continue;
        Int_t iAddr              = iChId & DetMask;
        Int_t iSt                = GetStationOfAddr(iAddr);
        Double_t hitpos[3]       = {3 * 0.};
        Double_t hitpos_local[3] = {3 * 0.};
        gGeoManager->FindNode(
          fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
        Int_t iRpcInd = fMapRpcIdParInd[iChId & DetMask];
        hitpos[0] =
          pHit->GetX() - (Double_t) fhPullX_Smt_Off->GetBinContent(iRpcInd + 1);
        hitpos[1] =
          pHit->GetY() - (Double_t) fhPullY_Smt_Off->GetBinContent(iRpcInd + 1);
        hitpos[2] =
          pHit->GetZ() - (Double_t) fhPullZ_Smt_Off->GetBinContent(iRpcInd + 1);
        gGeoManager->MasterToLocal(hitpos, hitpos_local);
        vhXY_AllTracks[iSt]->Fill(hitpos_local[0], hitpos_local[1]);
      }

      if (pTrk->GetNofHits() >= fNReqStations) {  // all possible hits are there
        LOG(debug) << "Complete Tracklet in event " << fiEvent;

        for (Int_t iSt = 0; iSt < fNTofStations; iSt++) {
          Int_t iH = pTrk->GetStationHitIndex(
            fMapStationRpcId[iSt]);  // Station Hit index
          if (iH < 0) {
            LOG(debug) << " Incomplete Tracklet, skip station " << iSt;
            continue;  // Station not part of tracklet
          }
          CbmTofHit* pHit          = (CbmTofHit*) fTofHitArray->At(iH);
          Int_t iChId              = pHit->GetAddress();
          Double_t hitpos[3]       = {3 * 0.};
          Double_t hitpos_local[3] = {3 * 0.};
          CbmTofCell* fChannelInfo = fDigiPar->GetCell(iChId);
          if (NULL == fChannelInfo) {
            //faked hit, take init values
          } else {
            /*	 TGeoNode *fNode=*/  // prepare global->local trafo
            gGeoManager->FindNode(
              fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
            Int_t iRpcInd = fMapRpcIdParInd[iChId & DetMask];
            hitpos[0] =
              pHit->GetX()
              - (Double_t) fhPullX_Smt_Off->GetBinContent(iRpcInd + 1);
            hitpos[1] =
              pHit->GetY()
              - (Double_t) fhPullY_Smt_Off->GetBinContent(iRpcInd + 1);
            hitpos[2] =
              pHit->GetZ()
              - (Double_t) fhPullZ_Smt_Off->GetBinContent(iRpcInd + 1);
            /*	 TGeoNode* cNode= gGeoManager->GetCurrentNode();*/
            gGeoManager->MasterToLocal(hitpos, hitpos_local);
          }
          vhXY_AllStations[iSt]->Fill(hitpos_local[0], hitpos_local[1]);
          Double_t dDX =
            pHit->GetX()
            - pTrk->GetFitX(
              pHit->GetZ());  // - tPar->GetX() - tPar->GetTx()*dDZ;
          Double_t dDY =
            pHit->GetY() - pTrk->GetFitY(pHit->GetZ());  // - tPar->GetTy()*dDZ;
          //Double_t dDT = pHit->GetTime() - pTrk->GetFitT(pHit->GetR()); //pTrk->GetTdif(fMapStationRpcId[iSt]);
          Double_t dDTB =
            fTrackletTools->GetTdif(pTrk,
                                    fMapStationRpcId[iSt],
                                    pHit);  // ignore pHit in calc of reference
          vhXY_DX[iSt]->Fill(hitpos_local[0], hitpos_local[1], dDX);
          vhXY_DY[iSt]->Fill(hitpos_local[0], hitpos_local[1], dDY);
          vhXY_DT[iSt]->Fill(hitpos_local[0], hitpos_local[1], dDTB);
          Double_t dCSZ = ((Double_t)(pHit->GetFlag() % 100)) * 0.5;
          Double_t dTOT = ((Double_t) pHit->GetCh()) * 0.1
                          / dCSZ;  // counteract UHIT flagging
          vhXY_TOT[iSt]->Fill(hitpos_local[0], hitpos_local[1], dTOT);
          vhXY_CSZ[iSt]->Fill(hitpos_local[0], hitpos_local[1], dCSZ);

          // debugging consistency of geometry transformations ....
          if (FairLogger::GetLogger()->IsLogNeeded(fair::Severity::debug)) {
            if (iSt == fNReqStations - 1) {  // treat as if not found
              Int_t iAddr               = fMapStationRpcId[iSt];
              CbmTofCell* fChannelInfoD = fDigiPar->GetCell(iAddr);
              Double_t zPos             = 0;
              Double_t zPosMiss         = -1;
              Double_t hitposD[3];
              Double_t hitpos_localD[3];
              Int_t NIter   = 5;
              Int_t iRpcInd = fMapRpcIdParInd[iAddr];
              Int_t iNbCh =
                fDigiBdfPar->GetNbChan(CbmTofAddress::GetSmType(iAddr),
                                       CbmTofAddress::GetRpcId(iAddr));
              while (zPos != zPosMiss && 0 < NIter--) {
                fChannelInfoD = fDigiPar->GetCell(iAddr);
                gGeoManager->FindNode(fChannelInfoD->GetX(),
                                      fChannelInfoD->GetY(),
                                      fChannelInfoD->GetZ());
                zPos = fChannelInfoD->GetZ()
                       + (Double_t) fhPullZ_Smt_Off->GetBinContent(iRpcInd + 1);
                hitposD[0] =
                  pTrk->GetFitX(zPos)
                  - (Double_t) fhPullX_Smt_Off->GetBinContent(iRpcInd + 1);
                hitposD[1] =
                  pTrk->GetFitY(zPos)
                  - (Double_t) fhPullY_Smt_Off->GetBinContent(iRpcInd + 1);
                hitposD[2] = fChannelInfoD->GetZ();
                /*	   TGeoNode* cNode=*/gGeoManager->GetCurrentNode();
                gGeoManager->MasterToLocal(hitposD, hitpos_localD);
                // Check for consistency of geometry
                Int_t iChTrafo = CbmTofAddress::GetChannelId(iAddr);
                Int_t iChMiss  = hitpos_localD[0] / fChannelInfoD->GetSizex()
                                + (iNbCh - 1) / 2;
                if (iChMiss < 0) iChMiss = 0;
                if (iChMiss > iNbCh - 1) iChMiss = iNbCh - 1;
                assert(fDigiBdfPar);
                if (iChMiss > -1 && iChMiss < iNbCh) {
                  Int_t iAddrMiss = CbmTofAddress::GetUniqueAddress(
                    CbmTofAddress::GetSmId(iAddr),
                    CbmTofAddress::GetRpcId(iAddr),
                    iChMiss,
                    0,
                    CbmTofAddress::GetSmType(iAddr));
                  CbmTofCell* fChannelInfoMiss = fDigiPar->GetCell(iAddrMiss);
                  if (NULL == fChannelInfoMiss) {
                    LOG(fatal)
                      << Form("Geo consistency check 0x%08x, 0x%08x failed at "
                              "St%d, z=%7.2f,%7.2f: iChTrafo %d, Miss %d , "
                              "xloc %6.2f, dx %4.2f",
                              iAddr,
                              iAddrMiss,
                              iSt,
                              zPos,
                              zPosMiss,
                              iChTrafo,
                              iChMiss,
                              hitpos_local[0],
                              fChannelInfo->GetSizex());
                  }
                  zPosMiss =
                    fChannelInfoMiss->GetZ()
                    + (Double_t) fhPullZ_Smt_Off->GetBinContent(iRpcInd + 1);
                  LOG(debug) << Form(
                    "Geo consistency check 0x%08x at St%d, z=%7.2f,%7.2f: "
                    "iChTrafo %d, Miss %d , xloc %6.2f, dx %4.2f",
                    iAddr,
                    iSt,
                    zPos,
                    zPosMiss,
                    iChTrafo,
                    iChMiss,
                    hitpos_localD[0],
                    fChannelInfoD->GetSizex());
                  fChannelInfo = fChannelInfoMiss;
                  iAddr        = iAddrMiss;
                } else
                  zPosMiss = zPos;
                LOG(debug) << Form(
                  "Predicted hit in Last Station 0x%08x at local pos x %6.2f, "
                  "y %6.2f, z   %6.2f, cell %p",
                  iAddr,
                  hitpos_localD[0],
                  hitpos_localD[1],
                  zPos,
                  fChannelInfoD);
                LOG(debug) << Form(
                  "Measured  hit in Last Station 0x%08x at local pos x %6.2f, "
                  "y %6.2f, z   %6.2f, cell %p",
                  pHit->GetAddress(),
                  hitpos_local[0],
                  hitpos_local[1],
                  pHit->GetZ(),
                  fChannelInfo);
              }
            }
          }
        }
      } else {
        if (pTrk->GetNofHits() == fNReqStations - 1) {  // one hit missing
          for (Int_t iSt = 0; iSt < fNTofStations; iSt++) {
            Int_t iH = pTrk->GetStationHitIndex(
              fMapStationRpcId[iSt]);  // Station Hit index
            if (iH < 0) {  // find geo element for the missing Station iSt
              Int_t iAddr = fMapStationRpcId[iSt];
              if (iAddr < 1) continue;
              //Int_t iChId   = CbmTofAddress::GetUniqueAddress(0,0,0,0,iSmType);
              //CbmTofCell* fChannelInfo = fDigiPar->GetCell( iChId );
              CbmTofCell* fChannelInfo = fDigiPar->GetCell(iAddr);
              if (NULL == fChannelInfo) {
                if (iWarnNotDefined++ < 100)
                  LOG(warning) << Form("CbmTofFindTracks::FillHistograms: Cell "
                                       "0x%08x not defined for Station %d",
                                       iAddr,
                                       iSt);
                continue;
              }
              /*	   TGeoNode *fNode=  */  // prepare global->local trafo
              Double_t zPos     = 0;
              Double_t zPosMiss = -1;
              Double_t hitpos[3];
              Double_t hitpos_local[3];
              Int_t NIter   = 5;
              Int_t iRpcInd = fMapRpcIdParInd[iAddr];
              Int_t iNbCh =
                fDigiBdfPar->GetNbChan(CbmTofAddress::GetSmType(iAddr),
                                       CbmTofAddress::GetRpcId(iAddr));
              while (zPos != zPosMiss && 0 < NIter--) {
                fChannelInfo = fDigiPar->GetCell(iAddr);
                gGeoManager->FindNode(fChannelInfo->GetX(),
                                      fChannelInfo->GetY(),
                                      fChannelInfo->GetZ());
                zPos = fChannelInfo->GetZ()
                       + (Double_t) fhPullZ_Smt_Off->GetBinContent(iRpcInd + 1);
                hitpos[0] =
                  pTrk->GetFitX(zPos)
                  - (Double_t) fhPullX_Smt_Off->GetBinContent(iRpcInd + 1);
                hitpos[1] =
                  pTrk->GetFitY(zPos)
                  - (Double_t) fhPullY_Smt_Off->GetBinContent(iRpcInd + 1);
                hitpos[2] = fChannelInfo->GetZ();
                /*	   TGeoNode* cNode=*/gGeoManager->GetCurrentNode();
                gGeoManager->MasterToLocal(hitpos, hitpos_local);
                // Check for consistency of geometry
                Int_t iChTrafo = CbmTofAddress::GetChannelId(iAddr);
                Int_t iChMiss =
                  hitpos_local[0] / fChannelInfo->GetSizex() + (iNbCh - 1) / 2;
                if (iChMiss < 0) iChMiss = 0;
                if (iChMiss > iNbCh - 1) iChMiss = iNbCh - 1;
                assert(fDigiBdfPar);
                if (iChMiss > -1 && iChMiss < iNbCh) {
                  Int_t iAddrMiss = CbmTofAddress::GetUniqueAddress(
                    CbmTofAddress::GetSmId(iAddr),
                    CbmTofAddress::GetRpcId(iAddr),
                    iChMiss,
                    0,
                    CbmTofAddress::GetSmType(iAddr));
                  CbmTofCell* fChannelInfoMiss = fDigiPar->GetCell(iAddrMiss);
                  if (NULL == fChannelInfoMiss) {
                    LOG(fatal)
                      << Form("Geo consistency check 0x%08x, 0x%08x failed at "
                              "St%d, z=%7.2f,%7.2f: iChTrafo %d, Miss %d , "
                              "xloc %6.2f, dx %4.2f",
                              iAddr,
                              iAddrMiss,
                              iSt,
                              zPos,
                              zPosMiss,
                              iChTrafo,
                              iChMiss,
                              hitpos_local[0],
                              fChannelInfo->GetSizex());
                  }
                  zPosMiss =
                    fChannelInfoMiss->GetZ()
                    + (Double_t) fhPullZ_Smt_Off->GetBinContent(iRpcInd + 1);

                  LOG(debug) << Form(
                    "Geo consistency check 0x%08x at St%d, z=%7.2f,%7.2f: "
                    "iChTrafo %d, Miss %d , xloc %6.2f, dx %4.2f",
                    iAddr,
                    iSt,
                    zPos,
                    zPosMiss,
                    iChTrafo,
                    iChMiss,
                    hitpos_local[0],
                    fChannelInfo->GetSizex());
                  fChannelInfo = fChannelInfoMiss;
                  iAddr        = iAddrMiss;
                } else
                  zPosMiss = zPos;
              }
              if (iSt == fNReqStations - 1)
                LOG(debug) << Form("Missing hit in Last Station in event %d at "
                                   "local pos x %6.2f, y %6.2f, z   %6.2f",
                                   fiEvent,
                                   hitpos_local[0],
                                   hitpos_local[1],
                                   zPos);

              vhXY_MissedStation[iSt]->Fill(hitpos_local[0], hitpos_local[1]);

              // correlation analysis
              for (Int_t iTrk1 = iTrk + 1; iTrk1 < fTrackArray->GetEntries();
                   iTrk1++) {
                CbmTofTracklet* pTrk1 =
                  (CbmTofTracklet*) fTrackArray->At(iTrk1);
                if (NULL == pTrk1 || pTrk == pTrk1) continue;
                if (pTrk1->GetNofHits()
                    >= fNReqStations) {  // all possible hits are there
                  fhTrklDT0SmMis->Fill(iSt,
                                       pTrk->GetFitT(0.) - pTrk1->GetFitT(0.));
                } else {
                  if (pTrk1->GetNofHits()
                      == fNReqStations - 1) {  // one hit missing
                    for (Int_t iSt1 = 0; iSt1 < fNTofStations; iSt1++) {
                      Int_t iH1 = pTrk1->GetStationHitIndex(
                        fMapStationRpcId[iSt1]);  // Station Hit index
                      if (
                        iH1
                        < 0) {  // find geo element for the missing Station iSt
                        //Int_t iSmType1 = fMapStationRpcId[iSt1]; (FU) not used
                        //if (iSmType1 < 1) continue;
                        fhTrklDT0StMis2->Fill(Double_t(iSt * 10 + iSt1),
                                              pTrk->GetFitT(0.)
                                                - pTrk1->GetFitT(0.));
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }  // velocity selection end
  }    // loop over tracklets end

  if (HMul.size() > 3)
    fhTrklMulMaxMM->Fill(HMul[fNTofStations], HMul[fNTofStations - 1]);
  if (HMul.size() > 5)
    fhTrklMul3D->Fill(
      HMul[fNTofStations], HMul[fNTofStations - 1], HMul[fNTofStations - 2]);
  fhTrklMulNhits->Fill(fTofHitArray->GetEntries(), iTMul);
  fhTrackingTimeNhits->Fill(fTofHitArray->GetEntries(), fdTrackingTime);

  // print info about special events
  if (0)
    if (5 < fNTofStations)
      if (HMul[6] > 1) {  // temporary
        //if (HMul[fNTofStations]>0)
        //LOG(info)<<"Found "<<HMul[fNTofStations]<<" max length tracklets in event "<<fiEvent
        LOG(info) << "Found " << HMul[6] << " max length tracklets in event "
                  << fiEvent << " within " << fTofHitArray->GetEntries()
                  << " hits ";
        for (Int_t iTrk = 0; iTrk < fTrackArray->GetEntries(); iTrk++) {
          CbmTofTracklet* pTrk = (CbmTofTracklet*) fTrackArray->At(iTrk);
          if (NULL == pTrk) continue;
          pTrk->PrintInfo();
        }
      }
  if (1)
    if (fTrackArray->GetEntries() > 25) {  // temporary
      LOG(info) << "Found  high multiplicity of " << fTrackArray->GetEntries()
                << " in event " << fiEvent << " from "
                << fTofHitArray->GetEntries() << " hits ";
      for (Int_t iTrk = 0; iTrk < fTrackArray->GetEntries(); iTrk++) {
        CbmTofTracklet* pTrk = (CbmTofTracklet*) fTrackArray->At(iTrk);
        if (NULL == pTrk) continue;
        pTrk->PrintInfo();
      }
    }

  if (iTMul > 1) {
    LOG(debug) << Form(
      "CbmTofFindTracks::FillHistograms NTrkl %d(%d) in event %d",
      iTMul,
      fTrackArray->GetEntries(),
      fiEvent);
    for (Int_t iTrk = 0; iTrk < fTrackArray->GetEntries(); iTrk++) {
      CbmTofTracklet* pTrk = (CbmTofTracklet*) fTrackArray->At(iTrk);
      if (NULL == pTrk) continue;
      if (
        pTrk->GetNofHits()
        > fMinNofHits) {  // for further analysis request min # of matched hits
        for (Int_t iTrk1 = iTrk + 1; iTrk1 < fTrackArray->GetEntries();
             iTrk1++) {
          CbmTofTracklet* pTrk1 = (CbmTofTracklet*) fTrackArray->At(iTrk1);
          if (NULL == pTrk1) continue;
          if (
            pTrk1->GetNofHits()
            > fMinNofHits) {  // for further analysis request min # of  matched hits
            //cout << " -D- iT "<<iTrk<<", iT1 "<<iTrk1<<endl;
            fhTrklT0Mul->Fill(iTMul, pTrk->GetFitT(0.) - pTrk1->GetFitT(0.));
          }
        }
      }
    }
  }

  LOG(debug1) << Form("CbmTofFindTracks::FillHistograms: HMul.size() %u ",
                      (UInt_t) HMul.size());
  for (UInt_t uHMul = 2; uHMul < HMul.size(); uHMul++) {
    LOG(debug1) << Form(
      "CbmTofFindTracks::FillHistograms() HMul %u, #%d", uHMul, HMul[uHMul]);
    if (HMul[uHMul] > 0) { fhTrklHMul->Fill(uHMul, HMul[uHMul]); }
  }

  for (Int_t iHit = 0; iHit < fTofHitArray->GetEntries();
       iHit++) {  // loop over Hits
    CbmTofHit* pHit = (CbmTofHit*) fTofHitArray->At(iHit);
    //    Int_t iSmType = CbmTofAddress::GetSmType( pHit->GetAddress() & DetMask ); (FU) not used
    Int_t iAddr = (pHit->GetAddress() & DetMask);
    fhAllHitsSmTypes->Fill(GetStationOfAddr(iAddr));
    //cout << " -D- " << iSmType <<", " << fTypeStation[iSmType] << endl;
    if (GetStationOfAddr(iAddr) > -1)
      fhAllHitsStation->Fill(GetStationOfAddr(iAddr));
  }
  // vertex stuff
  fhVTXNorm->Fill(fVTXNorm);
  if (fVTXNorm > 0.) {
    fhVTX_XY0->Fill(fVTX_X, fVTX_Y);
    for (Int_t iTrk = 0; iTrk < fTrackArray->GetEntries(); iTrk++) {
      CbmTofTracklet* pTrk = (CbmTofTracklet*) fTrackArray->At(iTrk);
      if (NULL == pTrk) continue;
      if (Double_t w = pTrk->GetNofHits() > (Double_t) fMinNofHits) {
        if (fVTXNorm > w) {
          Double_t DeltaT0 =
            pTrk->GetFitT(0.)
            - (fVTXNorm * fVTX_T - w * pTrk->GetFitT(0.)) / (fVTXNorm - w);
          fhVTX_DT0_Norm->Fill(fVTXNorm, DeltaT0);
        }
      }
    }
  }
  if (0 == fMapStationRpcId[0]) {  // Generated Pseudo TofHit at origin
    fTofHitArray->RemoveAt(fTofHitArray->GetEntries() - 1);  // remove added hit
  }
}

void CbmTofFindTracks::SetStations(Int_t ival) {
  fStationType.resize(fNTofStations);
  for (Int_t i = 0; i < 10; i++)
    fTypeStation[i] = -1;  // initialize
  for (Int_t i = 0; i < fNTofStations; i++) {
    Int_t iSm             = ival % 10;
    Int_t iSt             = fNTofStations - 1 - i;
    Int_t iAddr           = CbmTofAddress::GetUniqueAddress(0, 0, 0, 0, iSm);
    fStationType[iSt]     = iSm;
    fTypeStation[iSm]     = iSt;
    fMapStationRpcId[iSt] = iAddr;
    ival                  = (ival - iSm) / 10;
  }
}

void CbmTofFindTracks::SetStation(Int_t iVal,
                                  Int_t iModType,
                                  Int_t iModId,
                                  Int_t iRpcId) {
  Int_t iCenterCh = 0;
  if (NULL != fDigiBdfPar)
    iCenterCh =
      TMath::Floor((fDigiBdfPar->GetNbChan(iModType, iRpcId) - 1) / 2);
  Int_t iAddr =
    CbmTofAddress::GetUniqueAddress(iModId, iRpcId, iCenterCh, 0, iModType);
  fMapStationRpcId[iVal] = iAddr;
}

void CbmTofFindTracks::SetBeamCounter(Int_t iModType,
                                      Int_t iModId,
                                      Int_t iRpcId) {
  fiBeamCounter =
    CbmTofAddress::GetUniqueAddress(iModId, iRpcId, 0, 0, iModType);
}

Int_t CbmTofFindTracks::GetStationOfAddr(Int_t iAddr) {
  std::map<Int_t, Int_t>::iterator it;
  for (it = fMapStationRpcId.begin(); it != fMapStationRpcId.end(); it++)
    //std::map <Int_t, Int_t>::iterator it = fMapStationRpcId.find(iAddr);
    if (it->second == iAddr) break;
  /*
  if(it->first == fMapStationRpcId.size())
  {
    PrintSetup();
    LOG(fatal)<<Form("CbmTofFindTracks::GetStationOfAddr failed for 0x%08x, found Station = %d",iAddr,it->first)
  	     ;
  }
  */
  return it->first;
}

void CbmTofFindTracks::PrintSetup() {
  for (std::map<Int_t, Int_t>::iterator it = fMapStationRpcId.begin();
       it != fMapStationRpcId.end();
       it++) {
    cout << " <I> Tracking station " << it->first << " contains RpcId "
         << Form("0x%08x", it->second) << endl;
  }
}

Double_t CbmTofFindTracks::GetTOff(Int_t iAddr) {
  //cout << Form(" <D> GetTOff for 0x%08x at HistoIndex %d: %7.1f ", iAddr, fMapRpcIdParInd[iAddr],
  //(Double_t)fhPullT_Smt_Off->GetBinContent( fMapRpcIdParInd[iAddr] + 1)) <<endl;
  return (Double_t) fhPullT_Smt_Off->GetBinContent(fMapRpcIdParInd[iAddr] + 1);
}

Double_t CbmTofFindTracks::GetSigT(Int_t iAddr) {
  return (Double_t) fhPullT_Smt_Width->GetBinContent(fMapRpcIdParInd[iAddr]
                                                     + 1);
}

Double_t CbmTofFindTracks::GetSigX(Int_t iAddr) {
  return (Double_t) fhPullX_Smt_Width->GetBinContent(fMapRpcIdParInd[iAddr]
                                                     + 1);
}

Double_t CbmTofFindTracks::GetSigY(Int_t iAddr) {
  return (Double_t) fhPullY_Smt_Width->GetBinContent(fMapRpcIdParInd[iAddr]
                                                     + 1);
}

Double_t CbmTofFindTracks::GetSigZ(Int_t iAddr) {
  return (Double_t) fhPullZ_Smt_Width->GetBinContent(fMapRpcIdParInd[iAddr]
                                                     + 1);
}

Int_t CbmTofFindTracks::GetNStationsFired() {
  Int_t iNSt = 0;
  for (Int_t iSt = 0; iSt < fNTofStations; iSt++) {
    if (fStationHMul[iSt] > 0 && fStationHMul[iSt] < fiStationMaxHMul) iNSt++;
    LOG(debug2) << Form("Station %d, Addr 0x%08x, HMul %d, Max %d, Sum %d",
                        iSt,
                        GetAddrOfStation(iSt),
                        fStationHMul[iSt],
                        fiStationMaxHMul,
                        iNSt);
  }
  return iNSt;
}

void CbmTofFindTracks::ResetStationsFired() {
  for (Int_t iSt = 0; iSt < fNTofStations; iSt++)
    fStationHMul[iSt] = 0;
}

void CbmTofFindTracks::FillUHits() {
  // collect unused hits in active tracking stations
  Int_t iNbUHits = 0;
  for (Int_t iHit = 0; iHit < fTofHitArray->GetEntries(); iHit++) {
    CbmTofHit* pHit = (CbmTofHit*) fTofHitArray->At(iHit);
    Int_t iAddr     = (pHit->GetAddress() & DetMask);
    if (pHit->GetFlag() < 100. && GetStationOfAddr(iAddr) < fNTofStations) {
      if (!CheckHit2Track(pHit))  // check whether hit could belong to any track
        new ((*fTofUHitArray)[iNbUHits++]) CbmTofHit(*pHit);
    }
  }
}

Bool_t CbmTofFindTracks::CheckHit2Track(CbmTofHit* pHit) {
  Int_t iAddr = (pHit->GetAddress() & DetMask);
  Int_t iSt   = GetStationOfAddr(iAddr);
  if (iSt < 0 || iSt >= GetNofStations()) return kFALSE;
  for (Int_t iTrk = 0; iTrk < fTrackArray->GetEntries(); iTrk++) {
    CbmTofTracklet* pTrk = (CbmTofTracklet*) fTrackArray->At(iTrk);
    if (NULL == pTrk) continue;
    Double_t dDX = pHit->GetX() - pTrk->GetFitX(pHit->GetZ());
    Double_t dDY = pHit->GetY() - pTrk->GetFitY(pHit->GetZ());
    Double_t dDT = pHit->GetTime() - pTrk->GetFitT(pHit->GetZ());
    LOG(debug) << Form("Test Trk %d with HMul %d for Addr 0x%08x in station %d "
                       "with dx %5.1f, dy %5.1f, dt %5.1f",
                       iTrk,
                       pTrk->GetNofHits(),
                       iAddr,
                       iSt,
                       dDX,
                       dDY,
                       dDT);
    if (!(pTrk->ContainsAddr(iAddr))) {
      LOG(debug3) << "Fill histo " << vhUDXDY_DT[iSt]->GetName();
      vhUDXDY_DT[iSt]->Fill(dDX, dDY, dDT);
    } else {
      vhUCDXDY_DT[iSt]->Fill(dDX, dDY, dDT);
    }
  }
  return kFALSE;
}

void CbmTofFindTracks::CheckMaxHMul() {
  fInspectEvent = kTRUE;

  for (Int_t iSt = 0; iSt < fNTofStations; iSt++) {
    if (fStationHMul[iSt] > fiStationMaxHMul) {
      fInspectEvent = kFALSE;
    } else {
      if (fMapStationRpcId[iSt] == fiBeamCounter
          && fStationHMul[iSt] > fiBeamMaxHMul) {
        fInspectEvent = kFALSE;
      }
    }
  }
}

void CbmTofFindTracks::PrintMapRpcIdParInd() {
  std::map<Int_t, Int_t>::iterator it = fMapRpcIdParInd.begin();
  while (it != fMapRpcIdParInd.end()) {
    LOG(info) << Form("MapRpcIdParInd: %d, 0x%08x ", it->second, it->first);
    it++;
  }
}
