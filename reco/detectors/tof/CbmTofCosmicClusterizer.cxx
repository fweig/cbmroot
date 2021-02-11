/** @file CbmTofCosmicClusterizer.cxx
 ** author nh
 ** adopted from
 ** @file CbmTofSimpClusterizer.cxx
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 23.08.2013
 **/

#include "CbmTofCosmicClusterizer.h"

// TOF Classes and includes
#include "CbmMatch.h"
#include "CbmTofAddress.h"          // in cbmdata/tof
#include "CbmTofCell.h"             // in tof/TofData
#include "CbmTofDetectorId_v12b.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofDigi.h"             // in cbmdata/tof
#include "CbmTofDigiBdfPar.h"       // in tof/TofParam
#include "CbmTofDigiPar.h"          // in tof/TofParam
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofHit.h"              // in cbmdata/tof
#include "CbmTofPoint.h"            // in cbmdata/tof

#include "TTrbHeader.h"

// CBMroot classes and includes
#include "CbmMCTrack.h"

// FAIR classes and includes
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ROOT Classes and includes
#include "TClonesArray.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TF2.h"
#include "TGeoManager.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TLine.h"
#include "TMath.h"
#include "TMinuit.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TVector3.h"
#include <TFile.h>

// Constants definitions
#include "CbmTofClusterizersDef.h"

// C++ Classes and includes
// Globals
#include <vector>

static Int_t iIndexDut            = 0;
static Double_t StartAnalysisTime = 0.;
const Double_t cLight             = 29.9792;  // in cm/ns


CbmTofCosmicClusterizer* CbmTofCosmicClusterizer::fInstance = 0;

/************************************************************************************/
CbmTofCosmicClusterizer::CbmTofCosmicClusterizer()
  : CbmTofCosmicClusterizer("CosmicClusterizer", 0, 0) {
  if (!fInstance) fInstance = this;
}

CbmTofCosmicClusterizer::CbmTofCosmicClusterizer(const char* name,
                                                 Int_t verbose,
                                                 Bool_t writeDataInOut)
  : FairTask(TString(name), verbose)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fDigiPar(NULL)
  , fChannelInfo(NULL)
  , fDigiBdfPar(NULL)
  , fTrbHeader(NULL)
  , fTofPointsColl(NULL)
  , fMcTracksColl(NULL)
  , fTofDigisColl(NULL)
  , fbWriteHitsInOut(writeDataInOut)
  , fbWriteDigisInOut(writeDataInOut)
  , fTofCalDigisColl(NULL)
  , fTofHitsColl(NULL)
  , fTofDigiMatchColl(NULL)
  , fiNbHits(0)
  , fVerbose(verbose)
  , fStorDigiExp()
  , fStorDigiInd()
  , fviClusterMul()
  , fviClusterSize()
  , fviTrkMul()
  , fvdX()
  , fvdY()
  , fvdDifX()
  , fvdDifY()
  , fvdDifCh()
  , fhClustBuildTime(NULL)
  , fhHitsPerTracks(NULL)
  , fhPtsPerHit(NULL)
  , fhTimeResSingHits(NULL)
  , fhTimeResSingHitsB(NULL)
  , fhTimePtVsHits(NULL)
  , fhClusterSize(NULL)
  , fhClusterSizeType(NULL)
  , fhTrackMul(NULL)
  , fhClusterSizeMulti(NULL)
  , fhTrk1MulPos(NULL)
  , fhHiTrkMulPos(NULL)
  , fhAllTrkMulPos(NULL)
  , fhMultiTrkProbPos(NULL)
  , fhDigSpacDifClust(NULL)
  , fhDigTimeDifClust(NULL)
  , fhDigDistClust(NULL)
  , fhClustSizeDifX(NULL)
  , fhClustSizeDifY(NULL)
  , fhChDifDifX(NULL)
  , fhChDifDifY(NULL)
  , fhCluMulCorDutSel(NULL)
  , fhRpcDigiCor()
  , fhRpcCluMul()
  , fhRpcCluRate()
  , fhRpcCluPosition()
  , fhRpcCluPositionEvol()
  , fhRpcCluTimeEvol()
  , fhRpcCluDelPos()
  , fhRpcCluDelMatPos()
  , fhRpcCluTOff()
  , fhRpcCluDelTOff()
  , fhRpcCluDelMatTOff()
  , fhRpcCluTrms()
  , fhRpcCluTot()
  , fhRpcCluSize()
  , fhRpcCluAvWalk()
  , fhRpcCluAvLnWalk()
  , fhRpcCluWalk()
  , fhSmCluPosition()
  , fhSmCluTOff()
  , fhSmCluSvel()
  , fhSmCluFpar()
  , fhRpcDTLastHits()
  , fhRpcDTLastHits_Tot()
  , fhRpcDTLastHits_CluSize()
  , fhTRpcCluMul()
  , fhTRpcCluPosition()
  , fhTRpcCluTOff()
  , fhTRpcCluTot()
  , fhTRpcCluSize()
  , fhTRpcCluAvWalk()
  , fhTRpcCluDelTof()
  , fhTRpcCludXdY()
  , fhTRpcCluWalk()
  , fhTRpcCluWalk2()
  , fhTSmCluPosition()
  , fhTSmCluTOff()
  , fhTSmCluTRun()
  , fhTRpcCluTOffDTLastHits()
  , fhTRpcCluTotDTLastHits()
  , fhTRpcCluSizeDTLastHits()
  , fhTRpcCluMemMulDTLastHits()
  , fhSeldT()
  , fvCPDelTof()
  , fvCPTOff()
  , fvCPTotGain()
  , fvCPTotOff()
  , fvCPWalk()
  , fvLastHits()
  , fiNbSameSide(0)
  , fhNbSameSide(NULL)
  , fhNbDigiPerChan(NULL)
  , fStart()
  , fStop()
  , dTRef(0.)
  , fdTRefMax(0.)
  , fCalMode(0)
  , fCalSel(0)
  , fCalSmAddr(0)
  , fdCaldXdYMax(0.)
  , fiCluMulMax(0)
  , fTRefMode(0)
  , fTRefHits(0)
  , fDutId(0)
  , fDutSm(0)
  , fDutRpc(0)
  , fDutAddr(0)
  , fSelId(0)
  , fSelSm(0)
  , fSelRpc(0)
  , fSelAddr(0)
  , fSel2Id(0)
  , fSel2Sm(0)
  , fSel2Rpc(0)
  , fSel2Addr(0)
  , fSel2MulMax(1)
  , fDetIdIndexMap()
  , fviDetId()
  , vDigiIndRef()
  , fPosYMaxScal(0.)
  , fTRefDifMax(0.)
  , fTotMax(0.)
  , fTotMin(0.)
  , fTotOff(0.)
  , fTotMean(0.)
  , fdDelTofMax(60.)
  , fTotPreRange(0.)
  , fMaxTimeDist(0.)
  , fdChannelDeadtime(0.)
  , fdMemoryTime(0.)
  , fdYFitMin(1.E6)
  , fdTimePeriod(0.)
  , fiCorMode(0)
  , fEnableMatchPosScaling(kTRUE)
  , fEnableAvWalk(kFALSE)
  , fbPs2Ns(kFALSE)
  , fCalParFileName("")
  , fOutHstFileName("")
  , fCalParFile(NULL)
  , fiNevtBuild(0)
  , fiMsgCnt(100)
  , fdTOTMax(50.)
  , fdTOTMin(0.)
  , fdTTotMean(2.)
  , fdMaxTimeDist(0.)
  , fdMaxSpaceDist(0.)
  , fdEvent(0) {
  if (!fInstance) fInstance = this;
}

CbmTofCosmicClusterizer::~CbmTofCosmicClusterizer() {
  if (fGeoHandler) delete fGeoHandler;
  if (fInstance == this) fInstance = 0;
  //   DeleteHistos(); // <-- if needed  ?
}

/************************************************************************************/
// FairTasks inherited functions
InitStatus CbmTofCosmicClusterizer::Init() {
  LOG(info)
    << "CbmTofCosmicClusterizer initializing... expect Digis in ns units! ";

  if (kFALSE == RegisterInputs()) return kFATAL;

  if (kFALSE == RegisterOutputs()) return kFATAL;

  if (kFALSE == InitParameters()) return kFATAL;

  if (kFALSE == LoadGeometry()) return kFATAL;

  if (kFALSE == InitCalibParameter()) return kFATAL;

  if (kFALSE == CreateHistos()) return kFATAL;

  fDutAddr  = CbmTofAddress::GetUniqueAddress(fDutSm, 0, 0, 0, fDutId);
  fSelAddr  = CbmTofAddress::GetUniqueAddress(fSelSm, 0, 0, 0, fSelId);
  fSel2Addr = CbmTofAddress::GetUniqueAddress(fSel2Sm, 0, 0, 0, fSel2Id);
  iIndexDut = fDigiBdfPar->GetDetInd(fDutAddr);

  LOG(info) << "Init() completed for " << iNSel << " triggers"
            << ", Dut " << fDutId << ", " << fDutSm << ", " << fDutRpc
            << Form(", 0x%08x", fDutAddr) << ", Sel " << fSelId << ", "
            << fSelSm << ", " << fSelRpc << Form(", 0x%08x", fSelAddr)
            << ", Sel2 " << fSel2Id << ", " << fSel2Sm << ", " << fSel2Rpc
            << Form(", 0x%08x", fSel2Addr);

  return kSUCCESS;
}


void CbmTofCosmicClusterizer::SetParContainers() {
  LOG(info) << "=> Get the digi parameters for tof";
  LOG(warning) << "Return without action";
  return;
  // Get Base Container
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));

  LOG(info) << "found " << fDigiPar->GetNrOfModules() << " cells ";
  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
}


void CbmTofCosmicClusterizer::Exec(Option_t* /*option*/) {
  // Clear output arrays
  fTofCalDigisColl->Clear("C");
  fTofHitsColl->Clear("C");
  //fTofHitsColl->Delete();  // Computationally costly!, but hopefully safe
  //for (Int_t i=0; i<fTofDigiMatchColl->GetEntries(); i++) ((CbmMatch *)(fTofDigiMatchColl->At(i)))->ClearLinks();  // FIXME, try to tamper memory leak (did not help)
  //fTofDigiMatchColl->Clear("C+L");  // leads to memory leak
  fTofDigiMatchColl->Delete();

  fiNbHits = 0;

  LOG(debug) << "CbmTofCosmicClusterizer::Exec => New event";
  fStart.Set();

  BuildClusters();

  MergeClusters();

  fStop.Set();

  fdEvent++;
  FillHistos();

  //   fTofDigisColl->RemoveAll();
}

/************************************************************************************/
void CbmTofCosmicClusterizer::Finish() {
  WriteHistos();
  // Prevent them from being sucked in by the CbmHadronAnalysis WriteHistograms method
  // DeleteHistos();
  if (fdMemoryTime > 0.) CleanLHMemory();
}

void CbmTofCosmicClusterizer::Finish(Double_t calMode) {
  SetCalMode(calMode);
  WriteHistos();
}

/************************************************************************************/
// Functions common for all clusters approximations
Bool_t CbmTofCosmicClusterizer::RegisterInputs() {
  FairRootManager* fManager = FairRootManager::Instance();

  if (NULL == fManager) {
    LOG(error) << "CbmTofCosmicClusterizer::RegisterInputs => Could not find "
                  "FairRootManager!!!";
    return kFALSE;
  }  // if( NULL == fTofDigisColl)

  /*
   fTofPointsColl  = (TClonesArray *) fManager->GetObject("TofPoint");
   if( NULL == fTofPointsColl)
   {
      LOG(error)<<"CbmTofCosmicClusterizer::RegisterInputs => Could not get the TofPoint TClonesArray!!!";
      return kFALSE;
   } // if( NULL == fTofPointsColl)

   fMcTracksColl   = (TClonesArray *) fManager->GetObject("MCTrack");
   if( NULL == fMcTracksColl)
   {
      LOG(error)<<"CbmTofCosmicClusterizer::RegisterInputs => Could not get the MCTrack TClonesArray!!!";
      return kFALSE;
   } // if( NULL == fMcTracksColl)
   */

  fTofDigisColl = (TClonesArray*) fManager->GetObject("CbmTofDigi");

  if (NULL == fTofDigisColl)
    fTofDigisColl = (TClonesArray*) fManager->GetObject("CbmTofDigi");

  if (NULL == fTofDigisColl)
    fTofDigisColl = (TClonesArray*) fManager->GetObject("TofDigiExp");

  if (NULL == fTofDigisColl)
    fTofDigisColl = (TClonesArray*) fManager->GetObject("TofDigi");

  if (NULL == fTofDigisColl) {
    LOG(error) << "CbmTofCosmicClusterizer::RegisterInputs => Could not get "
                  "the CbmTofDigi TClonesArray!!!";
    return kFALSE;
  }  // if( NULL == fTofDigisColl)


  fTrbHeader = (TTrbHeader*) fManager->GetObject("TofTrbHeader.");
  if (NULL == fTrbHeader) {
    LOG(info) << "CbmTofCosmicClusterizer::RegisterInputs => Could not get the "
                 "TofTrbHeader Object";
  }

  return kTRUE;
}
Bool_t CbmTofCosmicClusterizer::RegisterOutputs() {
  FairRootManager* rootMgr = FairRootManager::Instance();

  fTofCalDigisColl = new TClonesArray("CbmTofDigi");

  // Flag check to control whether digis are written in output root file
  rootMgr->Register("TofCalDigi", "Tof", fTofCalDigisColl, fbWriteDigisInOut);


  fTofHitsColl = new TClonesArray("CbmTofHit");

  // Flag check to control whether digis are written in output root file
  rootMgr->Register("TofHit", "Tof", fTofHitsColl, fbWriteHitsInOut);

  fTofDigiMatchColl = new TClonesArray("CbmMatch", 100);
  rootMgr->Register("TofDigiMatch", "Tof", fTofDigiMatchColl, fbWriteHitsInOut);

  return kTRUE;
}
Bool_t CbmTofCosmicClusterizer::InitParameters() {

  // Initialize the TOF GeoHandler
  Bool_t isSimulation = kFALSE;
  LOG(info)
    << "CbmTofCosmicClusterizer::InitParameters - Geometry, Mapping, ...  ??";

  // Get Base Container
  FairRun* ana        = FairRun::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  Int_t iGeoVersion = fGeoHandler->Init(isSimulation);
  if (k14a > iGeoVersion) {
    LOG(error) << "CbmTofCosmicClusterizer::InitParameters => Only compatible "
                  "with geometries after v14a !!!";
    return kFALSE;
  }
  fTofId = new CbmTofDetectorId_v14a();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
  if (0 == fDigiPar) {
    LOG(error) << "CbmTofCosmicClusterizer::InitParameters => Could not obtain "
                  "the CbmTofDigiPar ";
    return kFALSE;
  }

  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
  if (0 == fDigiBdfPar) {
    LOG(error) << "CbmTofCosmicClusterizer::InitParameters => Could not obtain "
                  "the CbmTofDigiBdfPar ";
    return kFALSE;
  }

  rtdb->initContainers(ana->GetRunId());

  LOG(info) << "CbmTofCosmicClusterizer::InitParameter: currently "
            << fDigiPar->GetNrOfModules() << " digi cells ";


  fdMaxTimeDist  = fDigiBdfPar->GetMaxTimeDist();     // in ns
  fdMaxSpaceDist = fDigiBdfPar->GetMaxDistAlongCh();  // in cm

  if (fMaxTimeDist != fdMaxTimeDist) {
    fdMaxTimeDist = fMaxTimeDist;  // modify default
    fdMaxSpaceDist =
      fdMaxTimeDist * fDigiBdfPar->GetSignalSpeed()
      * 0.5;  // cut consistently on positions (with default signal velocity)
  }

  LOG(info) << " BuildCluster with MaxTimeDist " << fdMaxTimeDist
            << ", MaxSpaceDist " << fdMaxSpaceDist;

  if (fiCluMulMax == 0) fiCluMulMax = 100;
  if (fOutHstFileName == "") { fOutHstFileName = "./tofCosmicClust.hst.root"; }

  LOG(info) << " Hst Output filename = " << fOutHstFileName;

  LOG(info) << "<I>  Sel2Type = " << fSel2Id << ", Sm " << fSel2Sm << ", Det "
            << fSel2Rpc << ", MulMax " << fSel2MulMax;

  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofCosmicClusterizer::InitCalibParameter() {
  // dimension and initialize calib parameter
  // Int_t iNbDet     = fDigiBdfPar->GetNbDet();   (VF) not used
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();

  if (fTotMean != 0.) fdTTotMean = fTotMean;  // adjust target mean for TTT

  fvCPTOff.resize(iNbSmTypes);
  fvCPTotGain.resize(iNbSmTypes);
  fvCPTotOff.resize(iNbSmTypes);
  fvCPWalk.resize(iNbSmTypes);
  fvCPDelTof.resize(iNbSmTypes);
  for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
    Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
    Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
    fvCPTOff[iSmType].resize(iNbSm * iNbRpc);
    fvCPTotGain[iSmType].resize(iNbSm * iNbRpc);
    fvCPTotOff[iSmType].resize(iNbSm * iNbRpc);
    fvCPWalk[iSmType].resize(iNbSm * iNbRpc);
    fvCPDelTof[iSmType].resize(iNbSm * iNbRpc);
    for (Int_t iSm = 0; iSm < iNbSm; iSm++) {
      for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
        //          LOG(info)<<Form(" fvCPDelTof resize for SmT %d, R %d, B %d ",iSmType,iNbSm*iNbRpc,nbClDelTofBinX)
        //           ;
        fvCPDelTof[iSmType][iSm * iNbRpc + iRpc].resize(nbClDelTofBinX);
        for (Int_t iBx = 0; iBx < nbClDelTofBinX; iBx++) {
          // LOG(info)<<Form(" fvCPDelTof for SmT %d, R %d, B %d",iSmType,iSm*iNbRpc+iRpc,iBx);
          fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx].resize(iNSel);
          for (Int_t iSel = 0; iSel < iNSel; iSel++)
            fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] =
              0.;  // initialize
        }

        Int_t iNbChan = fDigiBdfPar->GetNbChan(iSmType, iRpc);
        fvCPTOff[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        fvCPTotGain[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        fvCPTotOff[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        fvCPWalk[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        Int_t nbSide = 2 - fDigiBdfPar->GetChanType(iSmType, iRpc);
        for (Int_t iCh = 0; iCh < iNbChan; iCh++) {
          fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          for (Int_t iSide = 0; iSide < nbSide; iSide++) {
            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] =
              0.;  //initialize
            fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] =
              1.;  //initialize
            fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] =
              0.;  //initialize
            fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][iSide].resize(
              nbClWalkBinX);
            for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
              fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][iSide][iWx] = 0.;
            }
          }
        }
      }
    }
  }

  LOG(info) << "CbmTofCosmicClusterizer::InitCalibParameter: defaults set";

  /// Save old global file and folder pointer to avoid messing with FairRoot
  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  if (0 < fCalMode) {
    LOG(info)
      << "CbmTofCosmicClusterizer::InitCalibParameter: read histos from "
      << "file " << fCalParFileName;

    // read parameter from histos
    if (fCalParFileName.IsNull()) return kTRUE;

    fCalParFile = new TFile(fCalParFileName, "");
    if (NULL == fCalParFile) {
      LOG(fatal) << "CbmTofCosmicClusterizer::InitCalibParameter: "
                 << "file " << fCalParFileName << " does not exist!";
      return kTRUE;
    }
    /*
    gDirectory->Print();
    fCalParFile->cd();
    fCalParFile->ls();
    */
    for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
      Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
      Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
      TProfile* hSvel =
        (TProfile*) gDirectory->FindObjectAny(Form("cl_SmT%01d_Svel", iSmType));

      // copy Histo to memory
      TDirectory* curdir = gDirectory;
      if (NULL != hSvel) {
        gDirectory->cd(oldDir->GetPath());
        // TProfile *hSvelmem = (TProfile *)hSvel->Clone();   (VF) not used
        gDirectory->cd(curdir->GetPath());
      } else {
        LOG(info) << "Svel histogram not found for module type " << iSmType;
      }

      for (Int_t iPar = 0; iPar < 4; iPar++) {
        TProfile* hFparcur = (TProfile*) gDirectory->FindObjectAny(
          Form("cl_SmT%01d_Fpar%1d", iSmType, iPar));
        if (NULL != hFparcur) {
          gDirectory->cd(oldDir->GetPath());
          // TProfile *hFparmem = (TProfile *)hFparcur->Clone();   (VF) not used
          gDirectory->cd(curdir->GetPath());
        }
      }

      for (Int_t iSm = 0; iSm < iNbSm; iSm++)
        for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
          // update default parameter
          if (NULL != hSvel) {
            Double_t Vscal = 1.;  //hSvel->GetBinContent(iSm*iNbRpc+iRpc+1);
            if (Vscal == 0.) Vscal = 1.;
            fDigiBdfPar->SetSigVel(iSmType,
                                   iSm,
                                   iRpc,
                                   fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)
                                     * Vscal);
            LOG(info) << "Modify " << iSmType << iSm << iRpc << " Svel by "
                      << Vscal << " to "
                      << fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);
          }
          TH2F* htempPos_pfx = (TH2F*) gDirectory->FindObjectAny(
            Form("cl_CorSmT%01d_sm%03d_rpc%03d_Pos_pfx", iSmType, iSm, iRpc));
          TH2F* htempTOff_pfx = (TH2F*) gDirectory->FindObjectAny(
            Form("cl_CorSmT%01d_sm%03d_rpc%03d_TOff_pfx", iSmType, iSm, iRpc));
          TH1D* htempTot_Mean = (TH1D*) gDirectory->FindObjectAny(
            Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Mean", iSmType, iSm, iRpc));
          TH1D* htempTot_Off = (TH1D*) gDirectory->FindObjectAny(
            Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Off", iSmType, iSm, iRpc));
          if (NULL != htempPos_pfx && NULL != htempTOff_pfx
              && NULL != htempTot_Mean && NULL != htempTot_Off) {
            Int_t iNbCh    = fDigiBdfPar->GetNbChan(iSmType, iRpc);
            Int_t iNbinTot = htempTot_Mean->GetNbinsX();
            for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
              Double_t YMean = ((TProfile*) htempPos_pfx)
                                 ->GetBinContent(iCh + 1);  //nh +1 empirical(?)
              Double_t TMean =
                ((TProfile*) htempTOff_pfx)->GetBinContent(iCh + 1);
              //Double_t dTYOff=YMean/fDigiBdfPar->GetSignalSpeed() ;
              Double_t dTYOff =
                YMean / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] += -dTYOff + TMean;
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] += +dTYOff + TMean;

              for (Int_t iSide = 0; iSide < 2; iSide++) {
                Double_t TotMean = htempTot_Mean->GetBinContent(
                  iCh * 2 + 1 + iSide);  //nh +1 empirical(?)
                if (0.001 < TotMean) {
                  fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] *=
                    fdTTotMean / TotMean;
                }
                fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] =
                  htempTot_Off->GetBinContent(iCh * 2 + 1 + iSide);
              }

              if (5 == iSmType || 8 == iSmType) {
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] =
                  fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1] =
                  fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] =
                  fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][0];
              }

              LOG(debug) << "CbmTofCosmicClusterizer::InitCalibParameter:"
                         << " SmT " << iSmType << " Sm " << iSm << " Rpc "
                         << iRpc << " Ch " << iCh
                         << Form(": YMean %f, TMean %f", YMean, TMean) << " -> "
                         << Form(
                              " %f, %f, %f, %f ",
                              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1],
                              fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                              fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1])
                         << ", NbinTot " << iNbinTot;

              TH1D* htempWalk0 = (TH1D*) gDirectory->FindObjectAny(
                Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",
                     iSmType,
                     iSm,
                     iRpc,
                     iCh));
              TH1D* htempWalk1 = (TH1D*) gDirectory->FindObjectAny(
                Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",
                     iSmType,
                     iSm,
                     iRpc,
                     iCh));
              if (NULL != htempWalk0
                  && NULL != htempWalk1) {  // reinitialize Walk array
                LOG(debug) << "Initialize Walk correction for "
                           << Form(" SmT%01d_sm%03d_rpc%03d_Ch%03d",
                                   iSmType,
                                   iSm,
                                   iRpc,
                                   iCh);
                if (htempWalk0->GetNbinsX() != nbClWalkBinX)
                  LOG(error) << "CbmTofCosmicClusterizer::InitCalibParameter: "
                                "Inconsistent Walk histograms";
                for (Int_t iBin = 0; iBin < nbClWalkBinX; iBin++) {
                  fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iBin] =
                    htempWalk0->GetBinContent(iBin + 1);
                  fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iBin] =
                    htempWalk1->GetBinContent(iBin + 1);
                  LOG(debug1) << Form(
                    " SmT%01d_sm%03d_rpc%03d_Ch%03d bin %d walk %f ",
                    iSmType,
                    iSm,
                    iRpc,
                    iCh,
                    iBin,
                    fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iBin]);
                  if (5 == iSmType || 8 == iSmType) {  // Pad structure
                    fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iBin] =
                      fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iBin];
                  }
                }
              }
            }
          } else {
            LOG(warning) << " Calibration histos "
                         << Form("cl_SmT%01d_sm%03d_rpc%03d_XXX",
                                 iSmType,
                                 iSm,
                                 iRpc)
                         << " not found. ";
          }
          for (Int_t iSel = 0; iSel < iNSel; iSel++) {
            TH1D* htmpDelTof = (TH1D*) gDirectory->FindObjectAny(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                   iSmType,
                   iSm,
                   iRpc,
                   iSel));
            if (NULL == htmpDelTof) {
              LOG(debug) << " Histos "
                         << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                                 iSmType,
                                 iSm,
                                 iRpc,
                                 iSel)
                         << " not found. ";
              continue;
            }
            LOG(debug) << " Load DelTof from histos "
                       << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                               iSmType,
                               iSm,
                               iRpc,
                               iSel)
                       << ".";
            for (Int_t iBx = 0; iBx < nbClDelTofBinX; iBx++) {
              fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] +=
                htmpDelTof->GetBinContent(iBx + 1);
            }

            // copy Histo to memory
            // TDirectory * curdir = gDirectory;
            gDirectory->cd(oldDir->GetPath());
            TH1D* h1DelTof = (TH1D*) htmpDelTof->Clone(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                   iSmType,
                   iSm,
                   iRpc,
                   iSel));

            LOG(debug) << " copy histo " << h1DelTof->GetName() << " to directory " << oldDir->GetName();

            gDirectory->cd(curdir->GetPath());
          }
        }
    }
  }
  //   fCalParFile->Delete();
  /// Restore old global file and folder pointer to avoid messing with FairRoot
  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gFile      = oldFile;
  gDirectory = oldDir;
  LOG(info)
    << "CbmTofCosmicClusterizer::InitCalibParameter: initialization done";
  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofCosmicClusterizer::LoadGeometry() {
  LOG(info) << "CbmTofCosmicClusterizer::LoadGeometry starting for  "
            << fDigiBdfPar->GetNbDet() << " described detectors, "
            << fDigiPar->GetNrOfModules() << " geometrically known modules ";

  //fTRefMode=0x00002016;  // use plastic 1 as time reference

  Int_t iNrOfCells = fDigiPar->GetNrOfModules();
  LOG(info) << "Digi Parameter container contains " << iNrOfCells << " cells.";
  for (Int_t icell = 0; icell < iNrOfCells; ++icell) {

    Int_t cellId =
      fDigiPar->GetCellId(icell);  // cellId is assigned in CbmTofCreateDigiPar
    fChannelInfo = fDigiPar->GetCell(cellId);

    Int_t smtype  = fGeoHandler->GetSMType(cellId);
    Int_t smodule = fGeoHandler->GetSModule(cellId);
    Int_t module  = fGeoHandler->GetCounter(cellId);
    Int_t cell    = fGeoHandler->GetCell(cellId);

    Double_t x  = fChannelInfo->GetX();
    Double_t y  = fChannelInfo->GetY();
    Double_t z  = fChannelInfo->GetZ();
    Double_t dx = fChannelInfo->GetSizex();
    Double_t dy = fChannelInfo->GetSizey();
    LOG(debug) << "-I- InitPar " << icell << " Id: " << Form("0x%08x", cellId)
               << " " << cell << " tmcs: " << smtype << " " << smodule << " "
               << module << " " << cell << " x=" << Form("%6.2f", x)
               << " y=" << Form("%6.2f", y) << " z=" << Form("%6.2f", z)
               << " dx=" << dx << " dy=" << dy;

    TGeoNode* fNode =  // prepare local->global trafo
      gGeoManager->FindNode(
        fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
    LOG(debug2) << Form(" Node at (%6.1f,%6.1f,%6.1f) : 0x%p",
                        fChannelInfo->GetX(),
                        fChannelInfo->GetY(),
                        fChannelInfo->GetZ(),
                        fNode);
    //      fNode->Print();
  }

  Int_t iNbDet = fDigiBdfPar->GetNbDet();
  for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
    Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);
    Int_t iSmType   = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSmId     = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpcId    = CbmTofAddress::GetRpcId(iUniqueId);
    LOG(info) << " DetIndx " << iDetIndx << "(" << iNbDet << "), SmType "
              << iSmType << ", SmId " << iSmId << ", RpcId " << iRpcId
              << " => UniqueId " << Form("0x%08x ", iUniqueId)
              << Form(" Svel %6.6f ",
                      fDigiBdfPar->GetSigVel(iSmType, iSmId, iRpcId));
    Int_t iCell = -1;
    while (kTRUE) {
      Int_t iUCellId =
        CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, ++iCell, 0, iSmType);
      fChannelInfo = fDigiPar->GetCell(iUCellId);
      if (NULL == fChannelInfo) break;
      LOG(debug3) << " Cell " << iCell << Form(" 0x%08x ", iUCellId)
                  << Form(", fCh 0x%p ", fChannelInfo)
                  << ", x: " << fChannelInfo->GetX()
                  << ", y: " << fChannelInfo->GetY()
                  << ", z: " << fChannelInfo->GetZ();
    }
  }

  //   return kTRUE;

  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();

  if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
    fStorDigiExp.resize(iNbSmTypes);
    fStorDigiInd.resize(iNbSmTypes);
    fviClusterSize.resize(iNbSmTypes);
    fviTrkMul.resize(iNbSmTypes);
    fvdX.resize(iNbSmTypes);
    fvdY.resize(iNbSmTypes);
    fvdDifX.resize(iNbSmTypes);
    fvdDifY.resize(iNbSmTypes);
    fvdDifCh.resize(iNbSmTypes);
    fviClusterMul.resize(iNbSmTypes);
    fvLastHits.resize(iNbSmTypes);

    for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
      Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
      Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
      fStorDigiExp[iSmType].resize(iNbSm * iNbRpc);
      fStorDigiInd[iSmType].resize(iNbSm * iNbRpc);
      fviClusterSize[iSmType].resize(iNbRpc);
      fviTrkMul[iSmType].resize(iNbRpc);
      fvdX[iSmType].resize(iNbRpc);
      fvdY[iSmType].resize(iNbRpc);
      fvdDifX[iSmType].resize(iNbRpc);
      fvdDifY[iSmType].resize(iNbRpc);
      fvdDifCh[iSmType].resize(iNbRpc);
      for (Int_t iSm = 0; iSm < iNbSm; iSm++) {
        fviClusterMul[iSmType].resize(iNbSm);
        fvLastHits[iSmType].resize(iNbSm);
        for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
          fviClusterMul[iSmType][iSm].resize(iNbRpc);
          fvLastHits[iSmType][iSm].resize(iNbRpc);
          Int_t iNbChan = fDigiBdfPar->GetNbChan(iSmType, iRpc);
          if (iNbChan == 0) {
            LOG(warning) << "CbmTofCosmicClusterizer::LoadGeometry: StoreDigi "
                            "without channels "
                         << Form("SmTy %3d, Sm %3d, NbRpc %3d, Rpc, %3d ",
                                 iSmType,
                                 iSm,
                                 iNbRpc,
                                 iRpc);
          }
          LOG(debug1)
            << "CbmTofCosmicClusterizer::LoadGeometry: StoreDigi with "
            << Form(
                 " %3d %3d %3d %3d %5d ", iSmType, iSm, iNbRpc, iRpc, iNbChan);
          fStorDigiExp[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
          fvLastHits[iSmType][iSm][iRpc].resize(iNbChan);
        }  // for( Int_t iRpc = 0; iRpc < iNbRpc; iRpc++ )
      }    // for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
    }      // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
  }        // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )

  return kTRUE;
}
Bool_t CbmTofCosmicClusterizer::DeleteGeometry() {
  LOG(info) << "CbmTofCosmicClusterizer::DeleteGeometry starting";
  return kTRUE;
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();
  if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
    for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
      Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
      Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
      for (Int_t iSm = 0; iSm < iNbSm; iSm++) {
        for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
          fStorDigiExp[iSmType][iSm * iNbRpc + iRpc].clear();
          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc].clear();
        }
      }  // for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
      fStorDigiExp[iSmType].clear();
      fStorDigiInd[iSmType].clear();
    }  // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
    fStorDigiExp.clear();
    fStorDigiInd.clear();
  }  // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )

  return kTRUE;
}
/************************************************************************************/
// Histogramming functions
Bool_t CbmTofCosmicClusterizer::CreateHistos() {
  /// Save old global file and folder pointer to avoid messing with FairRoot
  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fhClustBuildTime =
    new TH1I("TofCosmicClus_ClustBuildTime",
             "Time needed to build clusters in each event; Time [s]",
             4000,
             0.0,
             4.0);

  /*   TH2* hTemp = 0;*/

  // Sm related distributions
  fhSmCluPosition.resize(fDigiBdfPar->GetNbSmTypes());
  fhSmCluTOff.resize(fDigiBdfPar->GetNbSmTypes());
  fhSmCluSvel.resize(fDigiBdfPar->GetNbSmTypes());
  fhSmCluFpar.resize(fDigiBdfPar->GetNbSmTypes());
  fhTSmCluPosition.resize(fDigiBdfPar->GetNbSmTypes());
  fhTSmCluTOff.resize(fDigiBdfPar->GetNbSmTypes());
  fhTSmCluTRun.resize(fDigiBdfPar->GetNbSmTypes());

  for (Int_t iS = 0; iS < fDigiBdfPar->GetNbSmTypes(); iS++) {
    Double_t YSCAL = 50.;
    if (fPosYMaxScal != 0.) YSCAL = fPosYMaxScal;
    Int_t iUCellId = CbmTofAddress::GetUniqueAddress(0, 0, 0, 0, iS);
    fChannelInfo   = fDigiPar->GetCell(iUCellId);
    if (NULL == fChannelInfo) {
      LOG(warning) << "No DigiPar for SmType "
                   << Form("%d, 0x%08x", iS, iUCellId);
      continue;
    }
    Double_t YDMAX = TMath::Max(2., fChannelInfo->GetSizey()) * YSCAL;

    fhSmCluPosition[iS] =
      new TH2F(Form("cl_SmT%01d_Pos", iS),
               Form("Clu position of SmType %d; Sm+Rpc# []; ypos [cm]", iS),
               fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
               0,
               fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
               99,
               -YDMAX,
               YDMAX);

    Double_t TSumMax = 1.E3;
    if (fTRefDifMax != 0.) TSumMax = fTRefDifMax;
    fhSmCluTOff[iS] =
      new TH2F(Form("cl_SmT%01d_TOff", iS),
               Form("Clu TimeZero in SmType %d; Sm+Rpc# []; TOff [ns]", iS),
               fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
               0,
               fDigiBdfPar->GetNbSm(iS),  //*fDigiBdfPar->GetNbRpc(iS),
               99,
               -TSumMax,
               TSumMax);

    TProfile* hSvelcur =
      (TProfile*) gDirectory->FindObjectAny(Form("cl_SmT%01d_Svel", iS));
    if (NULL == hSvelcur) {
      fhSmCluSvel[iS] = new TProfile(
        Form("cl_SmT%01d_Svel", iS),
        Form("Clu Svel in SmType %d; Sm+Rpc# []; v/v_{nominal} ", iS),
        fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
        0,
        fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
        0.8,
        1.2);
      fhSmCluSvel[iS]->Sumw2();
    } else
      fhSmCluSvel[iS] = (TProfile*) hSvelcur->Clone();

    fhSmCluFpar[iS].resize(4);
    for (Int_t iPar = 0; iPar < 4; iPar++) {
      TProfile* hFparcur = (TProfile*) gDirectory->FindObjectAny(
        Form("cl_SmT%01d_Fpar%1d", iS, iPar));
      if (NULL == hFparcur) {
        LOG(info) << "Histo " << Form("cl_SmT%01d_Fpar%1d", iS, iPar)
                  << " not found, recreate ...";
        fhSmCluFpar[iS][iPar] = new TProfile(
          Form("cl_SmT%01d_Fpar%1d", iS, iPar),
          Form("Clu Fpar %d in SmType %d; Sm+Rpc# []; value ", iPar, iS),
          fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
          0,
          fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
          -100.,
          100.);
      } else
        fhSmCluFpar[iS][iPar] = (TProfile*) hFparcur->Clone();
    }

    fhTSmCluPosition[iS].resize(iNSel);
    fhTSmCluTOff[iS].resize(iNSel);
    fhTSmCluTRun[iS].resize(iNSel);
    for (Int_t iSel = 0; iSel < iNSel; iSel++) {  // Loop over selectors
      fhTSmCluPosition[iS][iSel] =
        new TH2F(Form("cl_TSmT%01d_Sel%02d_Pos", iS, iSel),
                 Form("Clu position of SmType %d under Selector %02d; Sm+Rpc# "
                      "[]; ypos [cm]",
                      iS,
                      iSel),
                 fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
                 0,
                 fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
                 99,
                 -YDMAX,
                 YDMAX);
      fhTSmCluTOff[iS][iSel] =
        new TH2F(Form("cl_TSmT%01d_Sel%02d_TOff", iS, iSel),
                 Form("Clu TimeZero in SmType %d under Selector %02d; Sm+Rpc# "
                      "[]; TOff [ns]",
                      iS,
                      iSel),
                 fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
                 0,
                 fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
                 99,
                 -TSumMax,
                 TSumMax);
      fhTSmCluTRun[iS][iSel] =
        new TH2F(Form("cl_TSmT%01d_Sel%02d_TRun", iS, iSel),
                 Form("Clu TimeZero in SmType %d under Selector %02d; Event# "
                      "[]; TMean [ns]",
                      iS,
                      iSel),
                 100,
                 0,
                 MaxNbEvent,
                 99,
                 -TSumMax,
                 TSumMax);
    }
  }

  // RPC related distributions
  Int_t iNbDet = fDigiBdfPar->GetNbDet();
  LOG(info) << " Define Clusterizer histos for " << iNbDet << " detectors ";

  fhRpcDigiCor.resize(iNbDet);
  fhRpcCluMul.resize(iNbDet);
  fhRpcCluRate.resize(iNbDet);
  fhRpcCluPosition.resize(iNbDet);
  fhRpcCluPositionEvol.resize(iNbDet);
  fhRpcCluTimeEvol.resize(iNbDet);
  fhRpcCluDelPos.resize(iNbDet);
  fhRpcCluDelMatPos.resize(iNbDet);
  fhRpcCluTOff.resize(iNbDet);
  fhRpcCluDelTOff.resize(iNbDet);
  fhRpcCluDelMatTOff.resize(iNbDet);
  fhRpcCluTrms.resize(iNbDet);
  fhRpcCluTot.resize(iNbDet);
  fhRpcCluSize.resize(iNbDet);
  fhRpcCluAvWalk.resize(iNbDet);
  fhRpcCluAvLnWalk.resize(iNbDet);
  fhRpcCluWalk.resize(iNbDet);
  fhRpcDTLastHits.resize(iNbDet);
  fhRpcDTLastHits_Tot.resize(iNbDet);
  fhRpcDTLastHits_CluSize.resize(iNbDet);
  fviDetId.resize(iNbDet);

  fDetIdIndexMap.clear();

  if (fTotMax != 0.) fdTOTMax = fTotMax;
  if (fTotMin != 0.) fdTOTMin = fTotMin;

  for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
    Int_t iUniqueId           = fDigiBdfPar->GetDetUId(iDetIndx);
    fDetIdIndexMap[iUniqueId] = iDetIndx;
    fviDetId[iDetIndx]        = iUniqueId;

    Int_t iSmType = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSmId   = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpcId  = CbmTofAddress::GetRpcId(iUniqueId);
    Int_t iUCellId =
      CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, 0, 0, iSmType);
    fChannelInfo = fDigiPar->GetCell(iUCellId);
    if (NULL == fChannelInfo) {
      LOG(warning) << "No DigiPar for Det " << Form("0x%08x", iUCellId);
      continue;
    }
    LOG(info) << "DetIndx " << iDetIndx << ", SmType " << iSmType << ", SmId "
              << iSmId << ", RpcId " << iRpcId << " => UniqueId "
              << Form("(0x%08x, 0x%08x)", iUniqueId, iUCellId) << ", dx "
              << fChannelInfo->GetSizex() << ", dy " << fChannelInfo->GetSizey()
              << Form(" ChPoi: %p ", fChannelInfo) << ", nbCh "
              << fDigiBdfPar->GetNbChan(iSmType, 0);

    // check access to all channel infos
    for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpcId); iCh++) {
      Int_t iCCellId =
        CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, iCh, 0, iSmType);
      fChannelInfo = fDigiPar->GetCell(iCCellId);
      if (NULL == fChannelInfo)
        LOG(warning) << Form(
          "missing ChannelInfo for ch %d addr 0x%08x", iCh, iCCellId);
    }

    fChannelInfo           = fDigiPar->GetCell(iUCellId);
    fhRpcDigiCor[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_DigiCor", iSmType, iSmId, iRpcId),
      Form(
        "Digi Correlation of Rpc #%03d in Sm %03d of type %d; digi 0; digi 1",
        iRpcId,
        iSmId,
        iSmType),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      fDigiBdfPar->GetNbChan(iSmType, iRpcId));

    fhRpcCluMul[iDetIndx] = new TH1F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_Mul", iSmType, iSmId, iRpcId),
      Form("Clu multiplicity of Rpc #%03d in Sm %03d of type %d; M []; Cnts",
           iRpcId,
           iSmId,
           iSmType),
      2. + 2. * fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      2. + 2. * fDigiBdfPar->GetNbChan(iSmType, iRpcId));

    fhRpcCluRate[iDetIndx] = new TH1F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_rate", iSmType, iSmId, iRpcId),
      Form("Clu rate of Rpc #%03d in Sm %03d of type %d; Time (s); Rate (Hz)",
           iRpcId,
           iSmId,
           iSmType),
      3600.,
      0.,
      3600.);

    fhRpcDTLastHits[iDetIndx] = new TH1F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_DTLastHits", iSmType, iSmId, iRpcId),
      Form("Clu #DeltaT to last hits  of Rpc #%03d in Sm %03d of type %d; log( "
           "#DeltaT (ns)); counts",
           iRpcId,
           iSmId,
           iSmType),
      100.,
      0.,
      10.);

    fhRpcDTLastHits_Tot[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_Tot_DTLH", iSmType, iSmId, iRpcId),
      Form("Clu Tot of Rpc #%03d in Sm %03d of type %d; log(#DeltaT (ns)); TOT "
           "[a.u.]",
           iRpcId,
           iSmId,
           iSmType),
      100,
      0.,
      10.,
      100,
      fdTOTMin,
      4. * fdTOTMax);

    fhRpcDTLastHits_CluSize[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_CluSize_DTLH", iSmType, iSmId, iRpcId),
      Form("Clu Size of Rpc #%03d in Sm %03d of type %d; log(#DeltaT (ns)); "
           "CluSize []",
           iRpcId,
           iSmId,
           iSmType),
      100,
      0.,
      10.,
      16,
      0.5,
      16.5);

    Double_t YSCAL = 50.;
    if (fPosYMaxScal != 0.) YSCAL = fPosYMaxScal;
    Double_t YDMAX = TMath::Max(2., fChannelInfo->GetSizey()) * YSCAL;
    fhRpcCluPosition[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_Pos", iSmType, iSmId, iRpcId),
      Form(
        "Clu position of Rpc #%03d in Sm %03d of type %d; Strip []; ypos [cm]",
        iRpcId,
        iSmId,
        iSmType),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      99,
      -YDMAX,
      YDMAX);

    fhRpcCluPositionEvol[iDetIndx] = new TProfile(
      Form("cl_SmT%01d_sm%03d_rpc%03d_PosEvol", iSmType, iSmId, iRpcId),
      Form("Clu position of Rpc #%03d in Sm %03d of type %d; Analysis Time "
           "[s]; ypos [cm]",
           iRpcId,
           iSmId,
           iSmType),
      1000,
      0.,
      1.E5,
      -100.,
      100.);

    fhRpcCluTimeEvol[iDetIndx] = new TProfile(
      Form("cl_SmT%01d_sm%03d_rpc%03d_TimeEvol", iSmType, iSmId, iRpcId),
      Form("Clu time of Rpc #%03d in Sm %03d of type %d; Analysis Time [s]; dT "
           "[ns]",
           iRpcId,
           iSmId,
           iSmType),
      1000,
      0.,
      1.E5,
      -10.,
      10.);

    fhRpcCluDelPos[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DelPos", iSmType, iSmId, iRpcId),
               Form("Clu position difference of Rpc #%03d in Sm %03d of type "
                    "%d; Strip []; #Deltaypos(clu) [cm]",
                    iRpcId,
                    iSmId,
                    iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId),
               0,
               fDigiBdfPar->GetNbChan(iSmType, iRpcId),
               99,
               -10.,
               10.);

    fhRpcCluDelMatPos[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_DelMatPos", iSmType, iSmId, iRpcId),
      Form("Matched Clu position difference of Rpc #%03d in Sm %03d of type "
           "%d; Strip []; #Deltaypos(mat) [cm]",
           iRpcId,
           iSmId,
           iSmType),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      99,
      -5.,
      5.);

    Double_t TSumMax = 1.E3;
    if (fTRefDifMax != 0.) TSumMax = fTRefDifMax;
    fhRpcCluTOff[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_TOff", iSmType, iSmId, iRpcId),
      Form(
        "Clu TimeZero of Rpc #%03d in Sm %03d of type %d; Strip []; TOff [ns]",
        iRpcId,
        iSmId,
        iSmType),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      99,
      -TSumMax,
      TSumMax);

    fhRpcCluDelTOff[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_DelTOff", iSmType, iSmId, iRpcId),
      Form("Clu TimeZero Difference of Rpc #%03d in Sm %03d of type %d; Strip "
           "[]; #DeltaTOff(clu) [ns]",
           iRpcId,
           iSmId,
           iSmType),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      99,
      -0.6,
      0.6);

    fhRpcCluDelMatTOff[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_DelMatTOff", iSmType, iSmId, iRpcId),
      Form("Clu TimeZero Difference of Rpc #%03d in Sm %03d of type %d; Strip "
           "[]; #DeltaTOff(mat) [ns]",
           iRpcId,
           iSmId,
           iSmType),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      99,
      -0.6,
      0.6);

    fhRpcCluTrms[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_Trms", iSmType, iSmId, iRpcId),
      Form(
        "Clu Time RMS of Rpc #%03d in Sm %03d of type %d; Strip []; Trms [ns]",
        iRpcId,
        iSmId,
        iSmType),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      99,
      0.,
      0.5);

    fhRpcCluTot[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_Tot", iSmType, iSmId, iRpcId),
      Form(
        "Clu Tot of Rpc #%03d in Sm %03d of type %d; StripSide []; TOT [a.u.]",
        iRpcId,
        iSmId,
        iSmType),
      2 * fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      2 * fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      100,
      fdTOTMin,
      fdTOTMax);

    fhRpcCluSize[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_Size", iSmType, iSmId, iRpcId),
      Form(
        "Clu size of Rpc #%03d in Sm %03d of type %d; Strip []; size [strips]",
        iRpcId,
        iSmId,
        iSmType),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      16,
      0.5,
      16.5);

    // Walk histos
    fhRpcCluAvWalk[iDetIndx] = new TH2D(
      Form("cl_SmT%01d_sm%03d_rpc%03d_AvWalk", iSmType, iSmId, iRpcId),
      Form("Walk in SmT%01d_sm%03d_rpc%03d_AvWalk", iSmType, iSmId, iRpcId),
      nbClWalkBinX,
      fdTOTMin,
      fdTOTMax,
      nbClWalkBinY,
      -TSumMax,
      TSumMax);

    fhRpcCluAvLnWalk[iDetIndx] = new TH2D(
      Form("cl_SmT%01d_sm%03d_rpc%03d_AvLnWalk", iSmType, iSmId, iRpcId),
      Form("Walk in SmT%01d_sm%03d_rpc%03d_AvLnWalk", iSmType, iSmId, iRpcId),
      nbClWalkBinX,
      TMath::Log10(fdTOTMax / 50.),
      TMath::Log10(fdTOTMax),
      nbClWalkBinY,
      -TSumMax,
      TSumMax);

    fhRpcCluWalk[iDetIndx].resize(fDigiBdfPar->GetNbChan(iSmType, iRpcId));
    for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpcId); iCh++) {
      fhRpcCluWalk[iDetIndx][iCh].resize(2);
      for (Int_t iSide = 0; iSide < 2; iSide++) {
        fhRpcCluWalk[iDetIndx][iCh][iSide] =
          new TH2D(Form("cl_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Walk",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iCh,
                        iSide),
                   Form("Walk in SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Walk",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iCh,
                        iSide),
                   nbClWalkBinX,
                   fdTOTMin,
                   fdTOTMax,
                   nbClWalkBinY,
                   -TSumMax,
                   TSumMax);
      }
      /*
         (fhRpcCluWalk[iDetIndx]).push_back( hTemp );
       */
    }
  }

  // Clusterizing monitoring

  LOG(info) << " Define Clusterizer monitoring histos ";

  fhCluMulCorDutSel =
    new TH2F(Form("hCluMulCorDutSel"),
             Form("Cluster Multiplicity Correlation of Dut %d%d%d with Sel "
                  "%d%d%d; Mul(Sel); Mul(Dut)",
                  fDutId,
                  fDutSm,
                  fDutRpc,
                  fSelId,
                  fSelSm,
                  fSelRpc),
             32,
             0,
             32,
             32,
             0,
             32);

  fhDigSpacDifClust =
    new TH1I("Clus_DigSpacDifClust",
             "Space difference along channel direction between Digi pairs on "
             "adjacent channels; PosCh i - Pos Ch i+1 [cm]",
             5000,
             -10.0,
             10.0);
  fhDigTimeDifClust =
    new TH1I("Clus_DigTimeDifClust",
             "Time difference between Digi pairs on adjacent channels; "
             "0.5*(tDigiA + tDigiA)chi - 0.5*(tDigiA + tDigiA)chi+1 [ns]",
             5000,
             -5.0,
             5.0);
  fhDigDistClust = new TH2I(
    "Clus_DigDistClust",
    "Distance between Digi pairs on adjacent channels; PosCh i - Pos Ch i+1 "
    "[cm along ch]; 0.5*(tDigiA + tDigiA)chi - 0.5*(tDigiA + tDigiA)chi+1 [ns]",
    5000,
    -10.0,
    10.0,
    2000,
    -5.0,
    5.0);
  fhClustSizeDifX =
    new TH2I("Clus_ClustSizeDifX",
             "Position difference between Point and Hit as function of Cluster "
             "Size; Cluster Size [Strips]; dX [cm]",
             100,
             0.5,
             100.5,
             500,
             -50,
             50);
  fhClustSizeDifY =
    new TH2I("Clus_ClustSizeDifY",
             "Position difference between Point and Hit as function of Cluster "
             "Size; Cluster Size [Strips]; dY [cm]",
             100,
             0.5,
             100.5,
             500,
             -50,
             50);
  fhChDifDifX     = new TH2I("Clus_ChDifDifX",
                         "Position difference between Point and Hit as "
                         "function of Channel dif; Ch Dif [Strips]; dX [cm]",
                         101,
                         -50.5,
                         50.5,
                         500,
                         -50,
                         50);
  fhChDifDifY     = new TH2I("Clus_ChDifDifY",
                         "Position difference between Point and Hit as "
                         "function of Channel Dif; Ch Dif [Strips]; dY [cm]",
                         101,
                         -50.5,
                         50.5,
                         500,
                         -50,
                         50);
  fhNbSameSide    = new TH1I("Clus_NbSameSide",
                          "How many time per event the 2 digis on a channel "
                          "were of the same side ; Counts/Event []",
                          500,
                          0.0,
                          500.0);
  fhNbDigiPerChan = new TH1I("Clus_NbDigiPerChan",
                             "Nb of Digis per channel; Nb Digis []",
                             100,
                             0.0,
                             100.0);

  // Trigger selected histograms
  if (0 < iNSel) {

    fhSeldT.resize(iNSel);
    for (Int_t iSel = 0; iSel < iNSel; iSel++) {
      fhSeldT[iSel] = new TH2F(Form("cl_dt_Sel%02d", iSel),
                               Form("Selector time %02d; dT [ns]", iSel),
                               99,
                               -50.,
                               50.,
                               16,
                               -0.5,
                               15.5);
    }

    fhTRpcCluMul.resize(iNbDet);
    fhTRpcCluPosition.resize(iNbDet);
    fhTRpcCluTOff.resize(iNbDet);
    fhTRpcCluTot.resize(iNbDet);
    fhTRpcCluSize.resize(iNbDet);
    fhTRpcCluAvWalk.resize(iNbDet);
    fhTRpcCluDelTof.resize(iNbDet);
    fhTRpcCludXdY.resize(iNbDet);
    fhTRpcCluWalk.resize(iNbDet);
    fhTRpcCluWalk2.resize(iNbDet);
    fhTRpcCluTOffDTLastHits.resize(iNbDet);
    fhTRpcCluTotDTLastHits.resize(iNbDet);
    fhTRpcCluSizeDTLastHits.resize(iNbDet);
    fhTRpcCluMemMulDTLastHits.resize(iNbDet);

    for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
      Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);
      Int_t iSmType   = CbmTofAddress::GetSmType(iUniqueId);
      Int_t iSmId     = CbmTofAddress::GetSmId(iUniqueId);
      Int_t iRpcId    = CbmTofAddress::GetRpcId(iUniqueId);
      Int_t iUCellId =
        CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, 0, 0, iSmType);
      fChannelInfo = fDigiPar->GetCell(iUCellId);
      if (NULL == fChannelInfo) {
        LOG(warning) << "No DigiPar for Det " << Form("0x%08x", iUCellId);
        continue;
      }
      LOG(debug1) << "DetIndx " << iDetIndx << ", SmType " << iSmType
                  << ", SmId " << iSmId << ", RpcId " << iRpcId
                  << " => UniqueId "
                  << Form("(0x%08x, 0x%08x)", iUniqueId, iUCellId) << ", dx "
                  << fChannelInfo->GetSizex() << ", dy "
                  << fChannelInfo->GetSizey()
                  << Form(" poi: 0x%p ", fChannelInfo) << ", nbCh "
                  << fDigiBdfPar->GetNbChan(iSmType, iRpcId);

      fhTRpcCluMul[iDetIndx].resize(iNSel);
      fhTRpcCluPosition[iDetIndx].resize(iNSel);
      fhTRpcCluTOff[iDetIndx].resize(iNSel);
      fhTRpcCluTot[iDetIndx].resize(iNSel);
      fhTRpcCluSize[iDetIndx].resize(iNSel);
      fhTRpcCluAvWalk[iDetIndx].resize(iNSel);
      fhTRpcCluDelTof[iDetIndx].resize(iNSel);
      fhTRpcCludXdY[iDetIndx].resize(iNSel);
      fhTRpcCluWalk[iDetIndx].resize(iNSel);
      fhTRpcCluWalk2[iDetIndx].resize(iNSel);
      fhTRpcCluTOffDTLastHits[iDetIndx].resize(iNSel);
      fhTRpcCluTotDTLastHits[iDetIndx].resize(iNSel);
      fhTRpcCluSizeDTLastHits[iDetIndx].resize(iNSel);
      fhTRpcCluMemMulDTLastHits[iDetIndx].resize(iNSel);

      for (Int_t iSel = 0; iSel < iNSel; iSel++) {
        fhTRpcCluMul[iDetIndx][iSel] =
          new TH1F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Mul",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("Clu multiplicity of Rpc #%03d in Sm %03d of type %d "
                        "under Selector %02d; M []; cnts",
                        iRpcId,
                        iSmId,
                        iSmType,
                        iSel),
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId),
                   0.,
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId));

        if (NULL == fhTRpcCluMul[iDetIndx][iSel])
          LOG(fatal) << " Histo not generated !";

        Double_t YSCAL = 50.;
        if (fPosYMaxScal != 0.) YSCAL = fPosYMaxScal;
        Double_t YDMAX = TMath::Max(2., fChannelInfo->GetSizey()) * YSCAL;
        fhTRpcCluPosition[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Pos",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("Clu position of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; Strip []; ypos [cm]",
                        iRpcId,
                        iSmId,
                        iSmType,
                        iSel),
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId),
                   0,
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId),
                   100,
                   -YDMAX,
                   YDMAX);

        Double_t TSumMax = 1.E4;
        if (fTRefDifMax != 0.) TSumMax = fTRefDifMax;
        fhTRpcCluTOff[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_TOff",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("Clu TimeZero of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; Strip []; TOff [ns]",
                        iRpcId,
                        iSmId,
                        iSmType,
                        iSel),
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId),
                   0,
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId),
                   999,
                   -TSumMax,
                   TSumMax);

        if (fTotMax != 0.) fdTOTMax = fTotMax;
        fhTRpcCluTot[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Tot",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("Clu Tot of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; StripSide []; TOT [a.u.]",
                        iRpcId,
                        iSmId,
                        iSmType,
                        iSel),
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2,
                   0,
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2,
                   100,
                   fdTOTMin,
                   fdTOTMax);

        fhTRpcCluSize[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Size",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("Clu size of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; Strip []; size [strips]",
                        iRpcId,
                        iSmId,
                        iSmType,
                        iSel),
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId),
                   0,
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId),
                   16,
                   0.5,
                   16.5);

        // Walk histos
        fhTRpcCluAvWalk[iDetIndx][iSel] = new TH2D(
          Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_AvWalk",
               iSmType,
               iSmId,
               iRpcId,
               iSel),
          Form("Walk in SmT%01d_sm%03d_rpc%03d_Sel%02d_AvWalk; TOT; T-TSel",
               iSmType,
               iSmId,
               iRpcId,
               iSel),
          nbClWalkBinX,
          fdTOTMin,
          fdTOTMax,
          nbClWalkBinY,
          -TSumMax,
          TSumMax);

        // Tof Histos
        fhTRpcCluDelTof[iDetIndx][iSel] = new TH2F(
          Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
               iSmType,
               iSmId,
               iRpcId,
               iSel),
          Form("SmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof; DistSel; T-TSel",
               iSmType,
               iSmId,
               iRpcId,
               iSel),
          //Form("SmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof; TRef-TSel; T-TSel", iSmType, iSmId, iRpcId, iSel),
          //nbClDelTofBinX,-fdDelTofMax,fdDelTofMax,nbClDelTofBinY,-TSumMax,TSumMax);
          nbClDelTofBinX,
          0.,
          fdDelTofMax,
          nbClDelTofBinY,
          -TSumMax,
          TSumMax);  // x-axis is distance of hits

        // Position deviation histos
        fhTRpcCludXdY[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_dXdY",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("SmT%01d_sm%03d_rpc%03d_Sel%02d_dXdY; #Delta x [cm]; "
                        "#Delta y [cm];",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   nbCldXdYBinX,
                   -dXdYMax,
                   dXdYMax,
                   nbCldXdYBinY,
                   -dXdYMax,
                   dXdYMax);

        fhTRpcCluWalk2[iDetIndx][iSel] =
          new TH3F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Walk2",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("Walk in SmT%01d_sm%03d_rpc%03d_Sel%02d_Walk2",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   nbClWalkBinX,
                   fdTOTMin,
                   fdTOTMax,
                   nbClWalkBinX,
                   fdTOTMin,
                   fdTOTMax,
                   nbClWalkBinY,
                   -TSumMax,
                   TSumMax);

        fhTRpcCluWalk[iDetIndx][iSel].resize(
          fDigiBdfPar->GetNbChan(iSmType, iRpcId));

        for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpcId);
             iCh++) {

          fhTRpcCluWalk[iDetIndx][iSel][iCh].resize(2);
          for (Int_t iSide = 0; iSide < 2; iSide++) {
            fhTRpcCluWalk[iDetIndx][iSel][iCh][iSide] = new TH2D(
              Form("cl_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Sel%02d_Walk",
                   iSmType,
                   iSmId,
                   iRpcId,
                   iCh,
                   iSide,
                   iSel),
              Form("Walk in SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Sel%02d_Walk",
                   iSmType,
                   iSmId,
                   iRpcId,
                   iCh,
                   iSide,
                   iSel),
              nbClWalkBinX,
              fdTOTMin,
              fdTOTMax,
              nbClWalkBinY,
              -TSumMax,
              TSumMax);
          }
        }

        fhTRpcCluTOffDTLastHits[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_TOff_DTLH",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("Clu TimeZero of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; log(#DeltaT (ns)); TOff [ns]",
                        iRpcId,
                        iSmId,
                        iSmType,
                        iSel),
                   100,
                   0.,
                   10.,
                   99,
                   -TSumMax,
                   TSumMax);

        fhTRpcCluTotDTLastHits[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Tot_DTLH",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("Clu Tot of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; log(#DeltaT (ns)); TOT [a.u.]",
                        iRpcId,
                        iSmId,
                        iSmType,
                        iSel),
                   100,
                   0.,
                   10.,
                   100,
                   fdTOTMin,
                   fdTOTMax);

        fhTRpcCluSizeDTLastHits[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Size_DTLH",
                        iSmType,
                        iSmId,
                        iRpcId,
                        iSel),
                   Form("Clu size of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; log(#DeltaT (ns)); size [strips]",
                        iRpcId,
                        iSmId,
                        iSmType,
                        iSel),
                   100,
                   0.,
                   10.,
                   10,
                   0.5,
                   10.5);

        fhTRpcCluMemMulDTLastHits[iDetIndx][iSel] = new TH2F(
          Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_MemMul_DTLH",
               iSmType,
               iSmId,
               iRpcId,
               iSel),
          Form("Clu Memorized Multiplicity of Rpc #%03d in Sm %03d of type %d "
               "under Selector %02d; log(#DeltaT (ns)); TOff [ns]",
               iRpcId,
               iSmId,
               iSmType,
               iSel),
          100,
          0.,
          10.,
          10,
          0,
          10);
      }
    }
  }
  // MC reference
  fhHitsPerTracks =
    new TH1I("Clus_TofHitPerTrk",
             "Mean Number of TofHit per Mc Track; Nb TofHits/Nb MC Tracks []",
             2000,
             0.0,
             20.0);
  if (kFALSE == fDigiBdfPar->ClustUseTrackId())
    fhPtsPerHit = new TH1I("Clus_TofPtsPerHit",
                           "Distribution of the Number of MCPoints associated "
                           "to each TofHit; Nb MCPoint []",
                           20,
                           0.0,
                           20.0);
  if (kTRUE == fDigiBdfPar->ClustUseTrackId()) {
    fhTimeResSingHits =
      new TH1I("Clus_TofTimeResClust",
               "Time resolution for TofHits containing Digis from a single MC "
               "Track; t(1st Mc Point) -tTofHit [ns]",
               10000,
               -25.0,
               25.0);
    fhTimeResSingHitsB =
      new TH2I("Clus_TofTimeResClustB",
               "Time resolution for TofHits containing Digis from a single MC "
               "Track; (1st Mc Point) -tTofHit [ns]",
               5000,
               -25.0,
               25.0,
               6,
               0,
               6);
    fhTimePtVsHits =
      new TH2I("Clus_TofTimePtVsHit",
               "Time resolution for TofHits containing Digis from a single MC "
               "Track; t(1st Mc Point) -tTofHit [ns]",
               2000,
               0.0,
               50.0,
               2000,
               0.0,
               50.0);
  } else {
    fhTimeResSingHits =
      new TH1I("Clus_TofTimeResClust",
               "Time resolution for TofHits containing Digis from a single "
               "TofPoint; tMcPoint -tTofHit [ns]",
               10000,
               -25.0,
               25.0);
    fhTimeResSingHitsB =
      new TH2I("Clus_TofTimeResClustB",
               "Time resolution for TofHits containing Digis from a single "
               "TofPoint; tMcPoint -tTofHit [ns]",
               5000,
               -25.0,
               25.0,
               6,
               0,
               6);
    fhTimePtVsHits = new TH2I("Clus_TofTimePtVsHit",
                              "Time resolution for TofHits containing Digis "
                              "from a single TofPoint; tMcPoint -tTofHit [ps]",
                              2000,
                              0.0,
                              50.0,
                              2000,
                              0.0,
                              50.0);
  }  // else of if( kTRUE == fDigiBdfPar->ClustUseTrackId() )
  fhClusterSize = new TH1I("Clus_ClusterSize",
                           "Cluster Size distribution; Cluster Size [Strips]",
                           100,
                           0.5,
                           100.5);
  fhClusterSizeType =
    new TH2I("Clus_ClusterSizeType",
             "Cluster Size distribution in each (SM type, Rpc) pair; Cluster "
             "Size [Strips]; 10*SM Type + Rpc Index []",
             100,
             0.5,
             100.5,
             40 * fDigiBdfPar->GetNbSmTypes(),
             0.0,
             40 * fDigiBdfPar->GetNbSmTypes());
  if (kTRUE == fDigiBdfPar->ClustUseTrackId()) {
    fhTrackMul = new TH1I(
      "Clus_TrackMul",
      "Number of MC tracks generating the cluster; MC Tracks multiplicity []",
      100,
      0.5,
      100.5);
    fhClusterSizeMulti = new TH2I(
      "Clus_ClusterSizeMulti",
      "Cluster Size distribution as function of Number of MC tracks generating "
      "the cluster; Cluster Size [Strips]; MC tracks mul. []",
      100,
      0.5,
      100.5,
      100,
      0.5,
      100.5);
    fhTrk1MulPos   = new TH2D("Clus_Trk1MulPos",
                            "Position of Clusters with only 1 MC tracks "
                            "generating the cluster; X [cm]; Y [cm]",
                            1500,
                            -750,
                            750,
                            1000,
                            -500,
                            500);
    fhHiTrkMulPos  = new TH2D("Clus_HiTrkMulPos",
                             "Position of Clusters with >1 MC tracks "
                             "generating the cluster; X [cm]; Y [cm]",
                             1500,
                             -750,
                             750,
                             1000,
                             -500,
                             500);
    fhAllTrkMulPos = new TH2D(
      "Clus_AllTrkMulPos",
      "Position of all clusters generating the cluster; X [cm]; Y [cm]",
      1500,
      -750,
      750,
      1000,
      -500,
      500);
    fhMultiTrkProbPos =
      new TH2D("Clus_MultiTrkProbPos",
               "Probability of having a cluster with multiple tracks as "
               "function of position; X [cm]; Y [cm]; Prob. [%]",
               1500,
               -750,
               750,
               1000,
               -500,
               500);
  }  // if( kTRUE == fDigiBdfPar->ClustUseTrackId() )

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gFile      = oldFile;
  gDirectory = oldDir;

  return kTRUE;
}

Bool_t CbmTofCosmicClusterizer::FillHistos() {
  fhClustBuildTime->Fill(fStop.GetSec() - fStart.GetSec()
                         + (fStop.GetNanoSec() - fStart.GetNanoSec()) / 1e9);
  Int_t iNbTofHits = fTofHitsColl->GetEntries();
  CbmTofHit* pHit;
  //gGeoManager->SetTopVolume( gGeoManager->FindVolumeFast("tof_v14a") );
  gGeoManager->CdTop();

  if (0 < iNbTofHits) {
    /* Double_t    dCluMul=0.;*/
    Bool_t BSel[iNSel];
    Double_t dTTrig[iNSel];
    CbmTofHit* pTrig[iNSel];
    CbmTofHit* pRef;
    Double_t ddXdZ[iNSel];
    Double_t ddYdZ[iNSel];
    Double_t dSel2dXdYMin[iNSel];

    Int_t iSel2Mul = 0;

    if (0 < iNSel) {  // check software triggers

      LOG(debug) << "CbmTofCosmicClusterizer::FillHistos: Muls: "
                 << fviClusterMul[fDutId][fDutSm][fDutRpc] << ", "
                 << fviClusterMul[fSelId][fSelSm][fSelRpc];
      // monitor multiplicities
      Int_t iNbDet = fDigiBdfPar->GetNbDet();
      for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
        Int_t iDetId  = fviDetId[iDetIndx];
        Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
        Int_t iSm     = CbmTofAddress::GetSmId(iDetId);
        Int_t iRpc    = CbmTofAddress::GetRpcId(iDetId);
        //LOG(info) << Form(" indx %d, Id 0x%08x, TSR %d %d %d", iDetIndx, iDetId, iSmType, iSm, iRpc)
        //          ;
        if (NULL != fhRpcCluMul[iDetIndx])
          fhRpcCluMul[iDetIndx]->Fill(fviClusterMul[iSmType][iSm][iRpc]);  //
      }

      fhCluMulCorDutSel->Fill(fviClusterMul[fSelId][fSelSm][fSelRpc],
                              fviClusterMul[fDutId][fDutSm][fDutRpc]);

      // do input distributions first
      for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
        pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
        if (NULL == pHit) continue;
        if (StartAnalysisTime == 0.) {
          StartAnalysisTime = pHit->GetTime();
          LOG(info) << "StartAnalysisTime set to " << StartAnalysisTime
                    << " ns. ";
        }
        Int_t iDetId = (pHit->GetAddress() & DetMask);

        std::map<UInt_t, UInt_t>::iterator it = fDetIdIndexMap.find(iDetId);
        if (it == fDetIdIndexMap.end())
          continue;                   // continue for invalid detector index
        Int_t iDetIndx = it->second;  //fDetIdIndexMap[iDetId];

        Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
        Int_t iSm     = CbmTofAddress::GetSmId(iDetId);
        Int_t iRpc    = CbmTofAddress::GetRpcId(iDetId);
        Int_t iCh     = CbmTofAddress::GetChannelId(pHit->GetAddress());

        Double_t dTimeAna = (pHit->GetTime() - StartAnalysisTime) / 1.E9;
        LOG(debug) << "TimeAna " << StartAnalysisTime << ", "
                   << Form(" Addr 0x%08x, t %f, tspill %f ",
                           pHit->GetAddress(),
                           pHit->GetTime(),
                           dTimeAna);
        fhRpcCluRate[iDetIndx]->Fill(dTimeAna);

        if (fdMemoryTime > 0.
            && fvLastHits[iSmType][iSm][iRpc][iCh].size() == 0)
          LOG(fatal) << Form(" <E> hit not stored in memory for TSRC %d%d%d%d",
                             iSmType,
                             iSm,
                             iRpc,
                             iCh);

        //CheckLHMemory();

        if (fvLastHits[iSmType][iSm][iRpc][iCh].size()
            > 1) {  // check for outdated hits
          //std::list<CbmTofHit *>::iterator it0=fvLastHits[iSmType][iSm][iRpc][iCh].begin();
          //std::list<CbmTofHit *>::iterator itL=fvLastHits[iSmType][iSm][iRpc][iCh].end();
          //CbmTofHit* pH0 = *it0;
          //CbmTofHit* pHL = *(--itL);
          CbmTofHit* pH0 = fvLastHits[iSmType][iSm][iRpc][iCh].front();
          CbmTofHit* pHL = fvLastHits[iSmType][iSm][iRpc][iCh].back();
          if (pH0->GetTime() > pHL->GetTime())
            LOG(warning) << Form("Invalid time ordering in ev %8.0f in list of "
                                 "size %lu for TSRC %d%d%d%d: Delta t %f  ",
                                 fdEvent,
                                 fvLastHits[iSmType][iSm][iRpc][iCh].size(),
                                 iSmType,
                                 iSm,
                                 iRpc,
                                 iCh,
                                 pHL->GetTime() - pH0->GetTime());

          //       while( (*((std::list<CbmTofHit *>::iterator) fvLastHits[iSmType][iSm][iRpc][iCh].begin()))->GetTime()+fdMemoryTime < pHit->GetTime()
          while (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 2.
                 || fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime()
                        + fdMemoryTime
                      < pHit->GetTime())

          {
            LOG(debug)
              << " pop from list size "
              << fvLastHits[iSmType][iSm][iRpc][iCh].size()
              << Form(" outdated hits for ev %8.0f in TSRC %d%d%d%d",
                      fdEvent,
                      iSmType,
                      iSm,
                      iRpc,
                      iCh)
              << Form(
                   " with tHit - tLast %f ",
                   pHit->GetTime()
                     - fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime())
              //(*((std::list<CbmTofHit *>::iterator) fvLastHits[iSmType][iSm][iRpc][iCh].begin()))->GetTime())
              ;
            if (fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress()
                != pHit->GetAddress())
              LOG(fatal) << Form(
                "Inconsistent address in list of size %lu for TSRC %d%d%d%d: "
                "0x%08x, time  %f",
                fvLastHits[iSmType][iSm][iRpc][iCh].size(),
                iSmType,
                iSm,
                iRpc,
                iCh,
                fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress(),
                fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime());
            fvLastHits[iSmType][iSm][iRpc][iCh].front()->Delete();
            fvLastHits[iSmType][iSm][iRpc][iCh].pop_front();
          }
        }  //fvLastHits[iSmType][iSm][iRpc][iCh].size()>1)

        // plot remaining time difference to previous hits
        if (fvLastHits[iSmType][iSm][iRpc][iCh].size()
            > 1) {  // check for previous hits in memory time interval
          CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
          Double_t dTotSum    = 0.;
          for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks();
               iLink += 2) {  // loop over digis
            CbmLink L0        = digiMatch->GetLink(iLink);
            Int_t iDigInd0    = L0.GetIndex();
            Int_t iDigInd1    = (digiMatch->GetLink(iLink + 1)).GetIndex();
            CbmTofDigi* pDig0 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
            CbmTofDigi* pDig1 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd1));
            dTotSum += pDig0->GetTot() + pDig1->GetTot();
          }

          std::list<CbmTofHit*>::iterator itL =
            fvLastHits[iSmType][iSm][iRpc][iCh].end();
          itL--;
          for (size_t iH = 0;
               iH < fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1;
               iH++) {
            itL--;
            fhRpcDTLastHits[iDetIndx]->Fill(
              TMath::Log10(pHit->GetTime() - (*itL)->GetTime()));
            fhRpcDTLastHits_CluSize[iDetIndx]->Fill(
              TMath::Log10(pHit->GetTime() - (*itL)->GetTime()),
              digiMatch->GetNofLinks() / 2.);
            fhRpcDTLastHits_Tot[iDetIndx]->Fill(
              TMath::Log10(pHit->GetTime() - (*itL)->GetTime()), dTotSum);
          }
        }
      }  // iHitInd loop end

      // do reference first
      dTRef     = dDoubleMax;
      fTRefHits = 0;
      pRef      = NULL;

      Double_t dMatXYScal =
        fhRpcCluPosition[iIndexDut]->GetYaxis()->GetXmax() / 30.;

      for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
        pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
        if (NULL == pHit) continue;
        Int_t iModId =
          (pHit->GetAddress() & ModMask);  // select Sel2 module hits

        if (fSel2Addr == iModId) {
          // Check Tot
          CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
          Double_t TotSum     = 0.;
          for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks();
               iLink += 2) {                             // loop over digis
            CbmLink L0     = digiMatch->GetLink(iLink);  //vDigish.at(ivDigInd);
            Int_t iDigInd0 = L0.GetIndex();
            if (iDigInd0 < fTofCalDigisColl->GetEntries()) {
              CbmTofDigi* pDig0 =
                (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
              TotSum += pDig0->GetTot();
            }
          }
          TotSum /= (0.5 * digiMatch->GetNofLinks());
          if (TotSum > fhRpcCluTot[iIndexDut]->GetYaxis()->GetXmax())
            continue;  // ignore too large clusters

          fTRefHits = 1;
          if (pHit->GetTime() < dTRef) {
            dTRef = pHit->GetTime();
            pRef  = pHit;
          }
          iSel2Mul++;
        }
      }
      LOG(debug) << "CbmTofCosmicClusterizer::FillHistos: Sel2Mul: "
                 << iSel2Mul;

      if (iSel2Mul > fSel2MulMax)
        return kFALSE;  // don't fill histos with ambigious reference
      if (iSel2Mul == 0)
        return kFALSE;  // don't fill histos without reference time

      for (Int_t iSel = 0; iSel < iNSel; iSel++) {
        BSel[iSel]         = kFALSE;
        pTrig[iSel]        = NULL;
        Int_t iDutMul      = 0;
        Int_t iRefMul      = 0;
        Int_t iR0          = 0;
        Int_t iRl          = 0;
        ddXdZ[iSel]        = 0.;
        ddYdZ[iSel]        = 0.;
        dSel2dXdYMin[iSel] = 1.E300;

        switch (iSel) {
          case 0:  //  Detector under Test (Dut) && Sel2
            iRl = fviClusterMul[fDutId][fDutSm].size();
            if (fDutRpc > -1) { iR0 = fDutRpc; }
            for (Int_t iRpc = iR0; iRpc < iRl; iRpc++)
              iDutMul += fviClusterMul[fDutId][fDutSm][iRpc];
            LOG(debug) << "selector 0: DutMul "
                       << fviClusterMul[fDutId][fDutSm][fDutRpc] << ", "
                       << iDutMul << ", Sel2Mul " << iSel2Mul
                       << " TRef: " << dTRef;
            if (iDutMul > 0 && iDutMul < fiCluMulMax) {
              dTTrig[iSel] = dDoubleMax;
              for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
                pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
                if (NULL == pHit) continue;
                Int_t iModId = (pHit->GetAddress() & ModMask);
                if (fDutAddr == iModId) {
                  if (pHit->GetTime() < dTTrig[iSel]) {
                    dTTrig[iSel] = pHit->GetTime();
                    pTrig[iSel]  = pHit;
                    BSel[iSel]   = kTRUE;
                  }
                }
              }
              LOG(debug) << Form(
                "Found selector 0 with mul %d from 0x%08x at %f ",
                iDutMul,
                pTrig[iSel]->GetAddress(),
                dTTrig[iSel]);
            }
            break;

          case 1:  // MRef & Sel2
            iRl = fviClusterMul[fSelId][fSelSm].size();
            if (fSelRpc > -1) { iR0 = fSelRpc; }
            for (Int_t iRpc = iR0; iRpc < iRl; iRpc++)
              iRefMul += fviClusterMul[fSelId][fSelSm][iRpc];
            LOG(debug) << "selector 1: RefMul "
                       << fviClusterMul[fSelId][fSelSm][fSelRpc] << ", "
                       << iRefMul << ", Sel2Mul " << iSel2Mul;
            if (iRefMul > 0 && iRefMul < fiCluMulMax) {
              LOG(debug1) << "Found selector 1";
              dTTrig[iSel] = dDoubleMax;
              for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
                pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
                if (NULL == pHit) continue;
                Int_t iModId = (pHit->GetAddress() & ModMask);
                if (fSelAddr == iModId) {
                  if (pHit->GetTime() < dTTrig[iSel]) {
                    dTTrig[iSel] = pHit->GetTime();
                    pTrig[iSel]  = pHit;
                    BSel[iSel]   = kTRUE;
                  }
                }
              }
              LOG(debug) << Form(
                "Found selector 1 with mul %d from 0x%08x at %f ",
                iRefMul,
                pTrig[iSel]->GetAddress(),
                dTTrig[iSel]);
            }
            break;

          default:
            LOG(info) << "CbmTofCosmicClusterizer::FillHistos: selection not "
                         "implemented "
                      << iSel;
            ;
        }  // switch end
        if (fTRefMode > 10) {
          if (pRef == NULL) return kFALSE;
          dTTrig[iSel] = dTRef;
          pTrig[iSel]  = pRef;
        }
      }  // iSel - loop end

      if (fSel2Id > 0) {  // confirm selector by independent match
        for (Int_t iSel = 0; iSel < iNSel; iSel++) {
          if (BSel[iSel]) {
            BSel[iSel] = kFALSE;
            //if(fviClusterMul[fSel2Id][fSel2Sm][fSel2Rpc] > 0 && fviClusterMul[fSel2Id][fSel2Sm][fSel2Rpc] < fiCluMulMax)
            for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
              pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
              if (NULL == pHit) continue;
              Int_t iModId = (pHit->GetAddress() & ModMask);
              if (fSel2Addr == iModId) {
                Double_t dzscal = 1.;
                if (fEnableMatchPosScaling)
                  dzscal = pHit->GetZ() / pTrig[iSel]->GetZ();
                Double_t dSEl2dXdz = (pHit->GetX() - pTrig[iSel]->GetX())
                                     / (pHit->GetZ() - pTrig[iSel]->GetZ());
                Double_t dSEl2dYdz = (pHit->GetY() - pTrig[iSel]->GetY())
                                     / (pHit->GetZ() - pTrig[iSel]->GetZ());

                if (TMath::Sqrt(
                      TMath::Power(pHit->GetX() - dzscal * pTrig[iSel]->GetX(),
                                   2.)
                      + TMath::Power(
                        pHit->GetY() - dzscal * pTrig[iSel]->GetY(), 2.))
                    < fdCaldXdYMax * dMatXYScal) {
                  BSel[iSel] = kTRUE;
                  Double_t dX2Y2 =
                    TMath::Sqrt(dSEl2dXdz * dSEl2dXdz + dSEl2dYdz * dSEl2dYdz);
                  if (dX2Y2 < dSel2dXdYMin[iSel]) {
                    ddXdZ[iSel]        = dSEl2dXdz;
                    ddYdZ[iSel]        = dSEl2dYdz;
                    dSel2dXdYMin[iSel] = dX2Y2;
                  }
                  break;
                }
              }
            }
          }  // BSel condition end
        }    // iSel lopp end
      }      // Sel2Id condition end

      UInt_t uTriggerPattern = 0;
      if (NULL != fTrbHeader)
        uTriggerPattern = fTrbHeader->GetTriggerPattern();
      else {
        for (Int_t iSel = 0; iSel < iNSel; iSel++)
          if (BSel[iSel]) {
            uTriggerPattern |=
              (0x1 << (iSel * 3
                       + CbmTofAddress::GetRpcId(pTrig[iSel]->GetAddress()
                                                 & DetMask)));
          }
      }
      for (Int_t iSel = 0; iSel < iNSel; iSel++) {
        if (BSel[iSel]) {
          if (dTRef != 0. && fTRefHits > 0) {
            for (UInt_t uChannel = 0; uChannel < 16; uChannel++) {
              if (uTriggerPattern & (0x1 << uChannel)) {
                fhSeldT[iSel]->Fill(dTTrig[iSel] - dTRef, uChannel);
              }
            }
          }
        }
      }
    }  // 0<iNSel software trigger check end

    for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
      pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
      if (NULL == pHit) continue;

      Int_t iDetId                          = (pHit->GetAddress() & DetMask);
      std::map<UInt_t, UInt_t>::iterator it = fDetIdIndexMap.find(iDetId);
      if (it == fDetIdIndexMap.end())
        continue;                   // continue for invalid detector index
      Int_t iDetIndx = it->second;  //fDetIdIndexMap[iDetId];

      Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
      Int_t iSm     = CbmTofAddress::GetSmId(iDetId);
      Int_t iRpc    = CbmTofAddress::GetRpcId(iDetId);
      Int_t iNbRpc  = fDigiBdfPar->GetNbRpc(iSmType);

      if (fviClusterMul[iSmType][iSm][iRpc] > fiCluMulMax)
        continue;  // skip this rpc

      if (-1 < fviClusterMul[iSmType][iSm][iRpc]) {
        for (Int_t iSel = 0; iSel < iNSel; iSel++)
          if (BSel[iSel]) {
            Double_t w = fviClusterMul[iSmType][iSm][iRpc];
            if (w == 0.)
              w = 1.;
            else
              w = 1. / w;
            fhTRpcCluMul[iDetIndx][iSel]->Fill(
              fviClusterMul[iSmType][iSm][iRpc], w);
          }
      }

      Int_t iChId  = pHit->GetAddress();
      fChannelInfo = fDigiPar->GetCell(iChId);
      Int_t iCh    = CbmTofAddress::GetChannelId(iChId);
      if (NULL == fChannelInfo) {
        LOG(error) << "Invalid Channel Pointer for ChId "
                   << Form(" 0x%08x ", iChId) << ", Ch " << iCh;
        continue;
      }
      /*TGeoNode *fNode=*/  // prepare global->local trafo
      gGeoManager->FindNode(
        fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());

      LOG(debug1) << "Hit info: "
                  << Form(" 0x%08x %d %f %f %f %f %f %d",
                          iChId,
                          iCh,
                          pHit->GetX(),
                          pHit->GetY(),
                          pHit->GetTime(),
                          fChannelInfo->GetX(),
                          fChannelInfo->GetY(),
                          iHitInd);

      Double_t hitpos[3];
      hitpos[0] = pHit->GetX();
      hitpos[1] = pHit->GetY();
      hitpos[2] = pHit->GetZ();
      Double_t hitpos_local[3];
      TGeoNode* cNode = gGeoManager->GetCurrentNode();
      gGeoManager->MasterToLocal(hitpos, hitpos_local);
      LOG(debug1) << Form(
        " MasterToLocal for node %p: (%6.1f,%6.1f,%6.1f) ->(%6.1f,%6.1f,%6.1f)",
        cNode,
        hitpos[0],
        hitpos[1],
        hitpos[2],
        hitpos_local[0],
        hitpos_local[1],
        hitpos_local[2]);

      fhRpcCluPosition[iDetIndx]->Fill(
        (Double_t) iCh, hitpos_local[1]);  //pHit->GetY()-fChannelInfo->GetY());
      fhSmCluPosition[iSmType]->Fill((Double_t)(iSm * iNbRpc + iRpc),
                                     hitpos_local[1]);

      Double_t dTimeAna = (pHit->GetTime() - StartAnalysisTime) / 1.E9;
      if (pHit != pRef)
        fhRpcCluTimeEvol[iDetIndx]->Fill(dTimeAna, pHit->GetTime() - dTRef);
      fhRpcCluPositionEvol[iDetIndx]->Fill(dTimeAna, hitpos_local[1]);

      for (Int_t iSel = 0; iSel < iNSel; iSel++)
        if (BSel[iSel]) {
          fhTRpcCluPosition[iDetIndx][iSel]->Fill(
            (Double_t) iCh,
            hitpos_local[1]);  //pHit->GetY()-fChannelInfo->GetY());
          fhTSmCluPosition[iSmType][iSel]->Fill((Double_t)(iSm * iNbRpc + iRpc),
                                                hitpos_local[1]);
        }

      if (TMath::Abs(hitpos_local[1]) > fChannelInfo->GetSizey() * fPosYMaxScal)
        continue;
      LOG(debug1) << " TofDigiMatchColl entries:"
                  << fTofDigiMatchColl->GetEntries();

      if (iHitInd > fTofDigiMatchColl->GetEntries()) {
        LOG(error) << " Inconsistent DigiMatches for Hitind " << iHitInd
                   << ", TClonesArraySize: " << fTofDigiMatchColl->GetEntries();
      }

      CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
      LOG(debug1) << " got " << digiMatch->GetNofLinks() << " matches for iCh "
                  << iCh << " at iHitInd " << iHitInd;

      fhRpcCluSize[iDetIndx]->Fill((Double_t) iCh,
                                   digiMatch->GetNofLinks() / 2.);

      for (Int_t iSel = 0; iSel < iNSel; iSel++)
        if (BSel[iSel]) {
          fhTRpcCluSize[iDetIndx][iSel]->Fill((Double_t) iCh,
                                              digiMatch->GetNofLinks() / 2.);
          if (fvLastHits[iSmType][iSm][iRpc][iCh].size()
              > 1) {  // check for previous hits in memory time interval
            std::list<CbmTofHit*>::iterator itL =
              fvLastHits[iSmType][iSm][iRpc][iCh].end();
            itL--;
            for (size_t iH = 0;
                 iH < fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1;
                 iH++) {
              itL--;
              fhTRpcCluSizeDTLastHits[iDetIndx][iSel]->Fill(
                TMath::Log10(pHit->GetTime() - (*itL)->GetTime()),
                digiMatch->GetNofLinks() / 2.);
            }
          }
        }

      Double_t TotSum = 0.;
      for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks();
           iLink++) {                                // loop over digis
        CbmLink L0     = digiMatch->GetLink(iLink);  //vDigish.at(ivDigInd);
        Int_t iDigInd0 = L0.GetIndex();
        if (iDigInd0 < fTofCalDigisColl->GetEntries()) {
          CbmTofDigi* pDig0 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
          TotSum += pDig0->GetTot();
        }
      }
      Double_t dMeanTimeSquared = 0.;
      Double_t dNstrips         = 0.;

      Double_t dDelTof = 0.;
      Double_t dTcor[iNSel];
      Double_t dTTcor[iNSel];
      Double_t dZsign[iNSel];
      Double_t dzscal = 1.;
      Double_t dDist  = 0.;

      for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks();
           iLink += 2) {                             // loop over digis
        CbmLink L0     = digiMatch->GetLink(iLink);  //vDigish.at(ivDigInd);
        Int_t iDigInd0 = L0.GetIndex();
        Int_t iDigInd1 =
          (digiMatch->GetLink(iLink + 1)).GetIndex();  //vDigish.at(ivDigInd+1);
        //LOG(debug1)<<" " << iDigInd0<<", "<<iDigInd1;

        if (iDigInd0 < fTofCalDigisColl->GetEntries()
            && iDigInd1 < fTofCalDigisColl->GetEntries()) {
          CbmTofDigi* pDig0 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
          CbmTofDigi* pDig1 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd1));
          if ((Int_t) pDig0->GetType() != iSmType) {
            LOG(error) << Form(" Wrong Digi SmType for Tofhit %d in iDetIndx "
                               "%d, Ch %d with %3.0f strips at Indx %d, %d",
                               iHitInd,
                               iDetIndx,
                               iCh,
                               dNstrips,
                               iDigInd0,
                               iDigInd1);
          }
          LOG(debug1) << " fhRpcCluTot:  Digi 0 " << iDigInd0 << ": Ch "
                      << pDig0->GetChannel() << ", Side " << pDig0->GetSide()
                      << ", StripSide "
                      << (Double_t) iCh * 2. + pDig0->GetSide() << " Digi 1 "
                      << iDigInd1 << ": Ch " << pDig1->GetChannel() << ", Side "
                      << pDig1->GetSide() << ", StripSide "
                      << (Double_t) iCh * 2. + pDig1->GetSide() << ", Tot0 "
                      << pDig0->GetTot() << ", Tot1 " << pDig1->GetTot();

          fhRpcCluTot[iDetIndx]->Fill(
            pDig0->GetChannel() * 2. + pDig0->GetSide(), pDig0->GetTot());
          fhRpcCluTot[iDetIndx]->Fill(
            pDig1->GetChannel() * 2. + pDig1->GetSide(), pDig1->GetTot());

          Int_t iCh0 = pDig0->GetChannel();
          Int_t iCh1 = pDig1->GetChannel();
          Int_t iS0  = pDig0->GetSide();
          Int_t iS1  = pDig1->GetSide();
          if (iCh0 != iCh1 || iS0 == iS1) {
            LOG(fatal) << Form(
              " MT2 for Tofhit %d in iDetIndx %d, Ch %d from %3.0f strips: ",
              iHitInd,
              iDetIndx,
              iCh,
              dNstrips)
                       << Form(" Dig0: Ind %d, Ch %d, Side %d, T: %6.1f ",
                               iDigInd0,
                               iCh0,
                               iS0,
                               pDig0->GetTime())
                       << Form(" Dig1: Ind %d, Ch %d, Side %d, T: %6.1f ",
                               iDigInd1,
                               iCh1,
                               iS1,
                               pDig1->GetTime());
            continue;
          }

          if (0 > iCh0 || fDigiBdfPar->GetNbChan(iSmType, iRpc) <= iCh0) {
            LOG(error) << Form(
              " Wrong Digi for Tofhit %d in iDetIndx %d, Ch %d at Indx %d, %d "
              "from %3.0f strips:  %d, %d, %d, %d",
              iHitInd,
              iDetIndx,
              iCh,
              iDigInd0,
              iDigInd1,
              dNstrips,
              iCh0,
              iCh1,
              iS0,
              iS1);
            continue;
          }

          if (
            digiMatch->GetNofLinks()
            > 2)  //&& digiMatch->GetNofLinks()<8 ) // FIXME: hardcoded limits on CluSize
          {
            dNstrips += 1.;
            dMeanTimeSquared += TMath::Power(
              0.5 * (pDig0->GetTime() + pDig1->GetTime()) - pHit->GetTime(), 2);
            //             fhRpcCluAvWalk[iDetIndx]->Fill(0.5*(pDig0->GetTot()+pDig1->GetTot()),
            //                        0.5*(pDig0->GetTime()+pDig1->GetTime())-pHit->GetTime());


            //Double_t dTotWeigth=(pDig0->GetTot()+pDig1->GetTot())/TotSum;    (VF) not used
            //Double_t dCorWeigth=1.-dTotWeigth;
            Double_t dCorWeigth = 1.;
            Double_t dTDigi =
              0.5 * (pDig0->GetTime() + pDig1->GetTime()) - pHit->GetTime();

            fhRpcCluAvLnWalk[iDetIndx]->Fill(
              TMath::Log10(0.5 * (pDig0->GetTot() + pDig1->GetTot())), dTDigi);

            fhRpcCluDelTOff[iDetIndx]->Fill(pDig0->GetChannel(),
                                            dCorWeigth * dTDigi);

            Double_t dDelPos = 0.5 * (pDig0->GetTime() - pDig1->GetTime())
                               * fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);
            if (0 == pDig0->GetSide()) dDelPos *= -1.;
            fhRpcCluDelPos[iDetIndx]->Fill(
              pDig0->GetChannel(), dCorWeigth * (dDelPos - hitpos_local[1]));

            fhRpcCluWalk[iDetIndx][iCh0][iS0]->Fill(pDig0->GetTot(), dTDigi);
            //pDig0->GetTime()-(pHit->GetTime() -(1.-2.*pDig0->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc)));

            fhRpcCluWalk[iDetIndx][iCh1][iS1]->Fill(pDig1->GetTot(), dTDigi);
            //pDig1->GetTime()-(pHit->GetTime()-(1.-2.*pDig1->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc)));

            fhRpcCluAvWalk[iDetIndx]->Fill(pDig0->GetTot(), dTDigi);
            fhRpcCluAvWalk[iDetIndx]->Fill(pDig1->GetTot(), dTDigi);

          }  // end of Clustersize > 1 condition

          LOG(debug1) << " fhRpcCluTot: Digi 0 " << iDigInd0 << ": Ch "
                      << pDig0->GetChannel() << ", Side " << pDig0->GetSide()
                      << ", StripSide "
                      << (Double_t) iCh * 2. + pDig0->GetSide() << " Digi 1 "
                      << iDigInd1 << ": Ch " << pDig1->GetChannel() << ", Side "
                      << pDig1->GetSide() << ", StripSide "
                      << (Double_t) iCh * 2. + pDig1->GetSide();

          // Fill event selected (triggered) histograms

          for (Int_t iSel = 0; iSel < iNSel; iSel++)
            if (BSel[iSel]) {
              if (NULL == pHit || NULL == pTrig[iSel]) {
                LOG(info) << " invalid pHit, iSel " << iSel << ", iDetIndx "
                          << iDetIndx;
                break;
              }
              if (pHit->GetAddress() == pTrig[iSel]->GetAddress()) continue;

              fhTRpcCluTot[iDetIndx][iSel]->Fill(
                pDig0->GetChannel() * 2. + pDig0->GetSide(), pDig0->GetTot());
              fhTRpcCluTot[iDetIndx][iSel]->Fill(
                pDig1->GetChannel() * 2. + pDig1->GetSide(), pDig1->GetTot());
              if (fvLastHits[iSmType][iSm][iRpc][iCh].size()
                  > 1) {  // check for previous hits in memory time interval
                std::list<CbmTofHit*>::iterator itL =
                  fvLastHits[iSmType][iSm][iRpc][iCh].end();
                itL--;
                for (size_t iH = 0;
                     iH < fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1;
                     iH++) {
                  itL--;
                  fhTRpcCluTotDTLastHits[iDetIndx][iSel]->Fill(
                    TMath::Log10(pHit->GetTime() - (*itL)->GetTime()),
                    pDig0->GetTot());
                  fhTRpcCluTotDTLastHits[iDetIndx][iSel]->Fill(
                    TMath::Log10(pHit->GetTime() - (*itL)->GetTime()),
                    pDig1->GetTot());
                }
              }
              if (iLink == 0) {  // Fill histo only once (for 1. digi entry)
                if (fEnableMatchPosScaling)
                  dzscal = pHit->GetZ() / pTrig[iSel]->GetZ();
                fhTRpcCludXdY[iDetIndx][iSel]->Fill(
                  pHit->GetX() - dzscal * pTrig[iSel]->GetX(),
                  pHit->GetY() - dzscal * pTrig[iSel]->GetY());
                // calculate spatial distance to trigger hit
                dDist = TMath::Sqrt(
                  TMath::Power(pHit->GetX() - pTrig[iSel]->GetX(), 2.)
                  + TMath::Power(pHit->GetY() - pTrig[iSel]->GetY(), 2.)
                  + TMath::Power(pHit->GetZ() - pTrig[iSel]->GetZ(), 2.));
                dDelTof =
                  dDist / cLight;  // correct to first order for speed of light
                pHit->GetZ() < pTrig[iSel]->GetZ() ? dZsign[iSel] = -1.
                                                   : dZsign[iSel] = 1.;
                if ((pHit->GetAddress() & DetMask)
                    == (pTrig[iSel]->GetAddress() & DetMask))
                  dZsign[iSel] = 0.;
                dTTcor[iSel] =
                  dDelTof * dZsign[iSel];  // store timing correction
              }
              //// look for geometrical match  with selector hit
              //if(  iSmType==fSel2Id      // to get entries in diamond/Sel2 histos
              if (iSmType == 5  // FIXME, to get entries in diamond histos
                  || TMath::Sqrt(
                       TMath::Power(pHit->GetX() - dzscal * pTrig[iSel]->GetX(),
                                    2.)
                       + TMath::Power(
                         pHit->GetY() - dzscal * pTrig[iSel]->GetY(), 2.))
                       < fdCaldXdYMax) {
                if (!fEnableMatchPosScaling && dSel2dXdYMin[iSel] < 1.E300)
                  if (TMath::Sqrt(
                        TMath::Power(
                          pHit->GetX()
                            - (pTrig[iSel]->GetX()
                               + ddXdZ[iSel]
                                   * (pHit->GetZ() - (pTrig[iSel]->GetZ()))),
                          2.)
                        + TMath::Power(
                          pHit->GetY()
                            - (pTrig[iSel]->GetY()
                               + ddYdZ[iSel]
                                   * (pHit->GetZ() - (pTrig[iSel]->GetZ()))),
                          2.))
                      > 0.5 * fdCaldXdYMax)
                    continue;  // FIXME: refine position selection cut in cosmic measurement
                if (
                  dTRef
                  != 0.) {  //&& TMath::Abs(dTRef-dTTrig[iSel])<fdDelTofMax) { // correct times for DelTof - velocity spread
                  if (
                    iLink
                    == 0) {  // do calculations only once (at 1. digi entry) // interpolate!
                    // determine correction value
                    //if(fSel2Addr  != iDetId) // do not do this for reference counter itself
                    dTcor[iSel] = 0.;  // precaution
                    dDelTof     = 0.;  //re
                    if (
                      fTRefMode
                      < 11)  // ??, old comment: do not do this for trigger counter itself
                    {
                      //Double_t dTentry=dTRef-dTTrig[iSel]+fdDelTofMax;
                      //Int_t iBx = dTentry/2./fdDelTofMax*nbClDelTofBinX;
                      //if(iBx<0) iBx=0;
                      //if(iBx>nbClDelTofBinX-1) iBx=nbClDelTofBinX-1;
                      //Double_t dBinWidth=2.*fdDelTofMax/nbClDelTofBinX;

                      Double_t dTentry = dDist;
                      Int_t iBx        = dTentry / fdDelTofMax * nbClDelTofBinX;
                      if (iBx < 0) iBx = 0;
                      if (iBx > nbClDelTofBinX - 1) iBx = nbClDelTofBinX - 1;
                      Double_t dBinWidth = fdDelTofMax / nbClDelTofBinX;

                      Double_t dDTentry =
                        dTentry - ((Double_t) iBx) * dBinWidth;
                      Int_t iBx1          = 0;
                      dDTentry < 0 ? iBx1 = iBx - 1 : iBx1 = iBx + 1;
                      Double_t w0 = 1. - TMath::Abs(dDTentry) / dBinWidth;
                      Double_t w1 = 1. - w0;
                      if (iBx1 < 0) iBx1 = 0;
                      if (iBx1 > nbClDelTofBinX - 1) iBx1 = nbClDelTofBinX - 1;
                      dDelTof =
                        fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] * w0
                        + fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx1][iSel]
                            * w1;
                      //dDelTof *= dDist; // has to be consistent with fhTRpcCluDelTof filling
                      LOG(debug1)
                        << Form(" DelTof for SmT %d, Sm %d, R %d, T %d, dTRef "
                                "%6.1f, Bx %d, Bx1 %d, DTe %6.1f -> DelT %6.1f",
                                iSmType,
                                iSm,
                                iRpc,
                                iSel,
                                dTRef - dTTrig[iSel],
                                iBx,
                                iBx1,
                                dDTentry,
                                dDelTof);
                    }

                    dTTcor[iSel] +=
                      dDelTof * dZsign[iSel];  // store timing correction
                    dTcor[iSel] =
                      (pHit->GetTime() - dTTrig[iSel]) - dTTcor[iSel];
                    // Double_t dAvTot=0.5*(pDig0->GetTot()+pDig1->GetTot());   (VF) not used
                  }  // if(iLink==0)

                  LOG(debug) << Form(
                    "TRpcCluWalk for Ev %d, Link %d(%d), Sel %d, TSR %d%d%d, "
                    "Ch %d,%d, S %d,%d Tcor %8.3f, LocT %6.1f, W-ent: "
                    "%6.0f,%6.0f",
                    fiNevtBuild,
                    iLink,
                    (Int_t) digiMatch->GetNofLinks(),
                    iSel,
                    iSmType,
                    iSm,
                    iRpc,
                    iCh0,
                    iCh1,
                    iS0,
                    iS1,
                    dTcor[iSel],
                    hitpos_local[1]
                      / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc),
                    fhTRpcCluWalk[iDetIndx][iSel][iCh0][iS0]->GetEntries(),
                    fhTRpcCluWalk[iDetIndx][iSel][iCh1][iS1]->GetEntries());

                  if (fhTRpcCluWalk[iDetIndx][iSel][iCh0][iS0]->GetEntries()
                      != fhTRpcCluWalk[iDetIndx][iSel][iCh1][iS1]->GetEntries())
                    LOG(error)
                      << Form(" Inconsistent walk histograms -> debugging "
                              "necessary ... for %d, %d, %d, %d, %d, %d, %d ",
                              fiNevtBuild,
                              iDetIndx,
                              iSel,
                              iCh0,
                              iCh1,
                              iS0,
                              iS1);

                  LOG(debug)
                    << Form("TRpcCluWalk values side %d: %5.2f, %7.3f, %6.3f,  "
                            "  side %d:  %5.2f, %7.3f, %6.3f",
                            iS0,
                            pDig0->GetTot(),
                            pDig0->GetTime()
                              - TMath::Floor(pDig0->GetTime() / 1000.) * 1000.,
                            dTcor[iSel]
                              + (1. - 2. * pDig0->GetSide()) * hitpos_local[1]
                                  / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc),
                            iS1,
                            pDig1->GetTot(),
                            pDig1->GetTime()
                              - TMath::Floor(pDig1->GetTime() / 1000.) * 1000.,
                            dTcor[iSel]
                              + (1. - 2. * pDig1->GetSide()) * hitpos_local[1]
                                  / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc));

                  fhTRpcCluWalk[iDetIndx][iSel][iCh0][iS0]->Fill(
                    pDig0->GetTot(),
                    //(pDig0->GetTime()+((1.-2.*pDig0->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc))-dTTrig[iSel])-dTTcor[iSel]);
                    // dTcor[iSel]+(1.-2.*pDig0->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc));
                    dTcor[iSel]);
                  fhTRpcCluWalk[iDetIndx][iSel][iCh1][iS1]->Fill(
                    pDig1->GetTot(),
                    //(pDig1->GetTime()+((1.-2.*pDig1->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc))-dTTrig[iSel])-dTTcor[iSel]);
                    //dTcor[iSel]+(1.-2.*pDig1->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc));
                    dTcor[iSel]);

                  fhTRpcCluWalk2[iDetIndx][iSel]->Fill(
                    pDig0->GetTot(), pDig1->GetTot(), dTcor[iSel]);

                  fhTRpcCluAvWalk[iDetIndx][iSel]->Fill(
                    pDig0->GetTot(),
                    //(pDig0->GetTime()+((1.-2.*pDig0->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc))-dTTrig[iSel])-dTTcor[iSel]);
                    //dTcor[iSel]+(1.-2.*pDig0->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc));
                    dTcor[iSel]);
                  fhTRpcCluAvWalk[iDetIndx][iSel]->Fill(
                    pDig1->GetTot(),
                    //(pDig1->GetTime()+((1.-2.*pDig1->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc))-dTTrig[iSel])-dTTcor[iSel]);
                    //dTcor[iSel]+(1.-2.*pDig1->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc));
                    dTcor[iSel]);

                  if (iLink == 0) {  // Fill histo only once (for 1. digi entry)
                    fhTRpcCluDelTof[iDetIndx][iSel]->Fill(dDist, dTcor[iSel]);
                    //fhTRpcCluDelTof[iDetIndx][iSel]->Fill(dTRef-dTTrig[iSel],dTcor[iSel]/dDist);
                    //fhTRpcCluDelTof[iDetIndx][iSel]->Fill(dTRef-dTTrig[iSel],dTcor[iSel]);
                    //fhTSmCluTOff[iSmType][iSel]->Fill((Double_t)(iSm*iNbRpc+iRpc),dTcor[iSel]);
                    fhTSmCluTOff[iSmType][iSel]->Fill((Double_t)(iSm),
                                                      dTcor[iSel]);
                    fhTSmCluTRun[iSmType][iSel]->Fill(fdEvent, dTcor[iSel]);
                    if (
                      iDetId
                      != (pTrig[iSel]->GetAddress()
                          & DetMask)) {  // transform matched hit-pair back into detector frame
                      hitpos[0] = pHit->GetX() - dzscal * pTrig[iSel]->GetX()
                                  + fChannelInfo->GetX();
                      hitpos[1] = pHit->GetY() - dzscal * pTrig[iSel]->GetY()
                                  + fChannelInfo->GetY();
                      hitpos[2] = pHit->GetZ();
                      gGeoManager->MasterToLocal(
                        hitpos, hitpos_local);  //  transform into local frame
                      fhRpcCluDelMatPos[iDetIndx]->Fill((Double_t) iCh,
                                                        hitpos_local[1]);
                      fhRpcCluDelMatTOff[iDetIndx]->Fill(
                        (Double_t) iCh,
                        (pHit->GetTime() - dTTrig[iSel]) - dTTcor[iSel]);
                    }
                  }  // iLink==0 condition end
                }    // position condition end
              }      // Match condition end
            }        // closing of selector loop
        } else {
          LOG(error)
            << "CbmTofCosmicClusterizer::FillHistos: invalid digi index "
            << iDetIndx << " digi0,1" << iDigInd0 << ", " << iDigInd1
            << " - max:" << fTofCalDigisColl->GetEntries()
            //                       << " in event " << XXX
            ;
        }
      }  // iLink digi loop end;

      if (1 < dNstrips) {
        //           Double_t dVar=dMeanTimeSquared/dNstrips - TMath::Power(pHit->GetTime(),2);
        Double_t dVar = dMeanTimeSquared / (dNstrips - 1);
        //if(dVar<0.) dVar=0.;
        Double_t dTrms = TMath::Sqrt(dVar);
        LOG(debug) << Form(" Trms for Tofhit %d in iDetIndx %d, Ch %d from "
                           "%3.0f strips: %6.3f ns",
                           iHitInd,
                           iDetIndx,
                           iCh,
                           dNstrips,
                           dTrms);
        fhRpcCluTrms[iDetIndx]->Fill((Double_t) iCh, dTrms);
        pHit->SetTimeError(dTrms);
      }

      LOG(debug1) << " Fill Time of iDetIndx " << iDetIndx << ", hitAddr "
                  << Form(
                       " %08x, y = %5.2f", pHit->GetAddress(), hitpos_local[1])
                  << " for |y| <"
                  << fhRpcCluPosition[iDetIndx]->GetYaxis()->GetXmax();
      if (TMath::Abs(hitpos_local[1])
          < (fhRpcCluPosition[iDetIndx]->GetYaxis()->GetXmax())) {
        if (dTRef != 0. && fTRefHits == 1) {
          // correct for distance
          dDist = TMath::Sqrt(TMath::Power(pHit->GetX() - pRef->GetX(), 2.)
                              + TMath::Power(pHit->GetY() - pRef->GetY(), 2.)
                              + TMath::Power(pHit->GetZ() - pRef->GetZ(), 2.));
          dDelTof =
            dDist / cLight;  // correct to first order for speed of light
          Double_t dZsgn = 1.;
          if (pHit->GetZ() < pRef->GetZ()) dZsgn = -1.;
          dDelTof *= dZsgn;  // store timing correction

          fhRpcCluTOff[iDetIndx]->Fill((Double_t) iCh,
                                       (pHit->GetTime() - dTRef) - dDelTof);
          //fhSmCluTOff[iSmType]->Fill((Double_t)(iSm*iNbRpc+iRpc),pHit->GetTime()-dTRef-dDelTof);
          fhSmCluTOff[iSmType]->Fill((Double_t)(iSm),
                                     (pHit->GetTime() - dTRef) - dDelTof);

          for (Int_t iSel = 0; iSel < iNSel; iSel++)
            if (BSel[iSel]) {
              LOG(debug1) << " TRpcCluTOff " << iDetIndx << ", Sel " << iSel
                          << Form(", Dt %7.3f, LHsize %lu ",
                                  (pHit->GetTime() - dTTrig[iSel]),
                                  fvLastHits[iSmType][iSm][iRpc][iCh].size());
              if (pHit->GetAddress() == pTrig[iSel]->GetAddress()) continue;

              if (fvLastHits[iSmType][iSm][iRpc][iCh].size()
                  > 1) {  // check for previous hits in memory time interval
                std::list<CbmTofHit*>::iterator itL =
                  fvLastHits[iSmType][iSm][iRpc][iCh].end();
                itL--;
                for (size_t iH = 0;
                     iH < fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1;
                     iH++) {
                  itL--;
                  LOG(debug1)
                    << Form(" %f,", pHit->GetTime() - (*itL)->GetTime());
                }
              }
              LOG(debug1);
              // fill Time Offset histograms with DelTof correction
              fhTRpcCluTOff[iDetIndx][iSel]->Fill(
                (Double_t) iCh,
                (pHit->GetTime() - dTTrig[iSel])
                  - dTTcor[iSel]);  //  DEC2017 modified !
              if (fvLastHits[iSmType][iSm][iRpc][iCh].size()
                  > 1) {  // check for previous hits in memory time interval
                std::list<CbmTofHit*>::iterator itL =
                  fvLastHits[iSmType][iSm][iRpc][iCh].end();
                itL--;
                for (Int_t iH = 0; iH < 1; iH++) {  // use only last hit
                  //  for(Int_t iH=0; iH<fvLastHits[iSmType][iSm][iRpc][iCh].size()-1; iH++){//fill for all memorized hits
                  itL--;
                  Double_t dTsinceLast = pHit->GetTime() - (*itL)->GetTime();
                  if (dTsinceLast > fdMemoryTime)
                    LOG(fatal) << Form("Invalid Time since last hit on channel "
                                       "TSRC %d%d%d%d: %f > %f",
                                       iSmType,
                                       iSm,
                                       iRpc,
                                       iCh,
                                       dTsinceLast,
                                       fdMemoryTime);

                  fhTRpcCluTOffDTLastHits[iDetIndx][iSel]->Fill(
                    TMath::Log10(dTsinceLast),
                    (pHit->GetTime() - dTTrig[iSel]));
                  fhTRpcCluMemMulDTLastHits[iDetIndx][iSel]->Fill(
                    TMath::Log10(dTsinceLast),
                    fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1);
                }
              }
            }
        }
      }
    }

    for (Int_t iSmType = 0; iSmType < fDigiBdfPar->GetNbSmTypes(); iSmType++) {
      for (Int_t iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
        LOG(debug1) << "CbmTofCosmicClusterizer::FillHistos:  "
                    << Form(" %3d %3d %3lu ",
                            iSmType,
                            iRpc,
                            fviClusterSize[iSmType][iRpc].size());

        for (UInt_t uCluster = 0;
             uCluster < fviClusterSize[iSmType][iRpc].size();
             uCluster++) {
          LOG(debug2) << "CbmTofCosmicClusterizer::FillHistos:  "
                      << Form(" %3d %3d %3d ", iSmType, iRpc, uCluster);

          fhClusterSize->Fill(fviClusterSize[iSmType][iRpc][uCluster]);
          fhClusterSizeType->Fill(fviClusterSize[iSmType][iRpc][uCluster],
                                  40 * iSmType
                                    + iRpc);  //FIXME - hardwired constant

          if (kFALSE)  //  1 == fviTrkMul[iSmType][iRpc][uCluster] )
          {
            fhClustSizeDifX->Fill(fviClusterSize[iSmType][iRpc][uCluster],
                                  fvdDifX[iSmType][iRpc][uCluster]);
            fhClustSizeDifY->Fill(fviClusterSize[iSmType][iRpc][uCluster],
                                  fvdDifY[iSmType][iRpc][uCluster]);

            if (1 == fviClusterSize[iSmType][iRpc][uCluster]) {
              fhChDifDifX->Fill(fvdDifCh[iSmType][iRpc][uCluster],
                                fvdDifX[iSmType][iRpc][uCluster]);
              fhChDifDifY->Fill(fvdDifCh[iSmType][iRpc][uCluster],
                                fvdDifY[iSmType][iRpc][uCluster]);
            }
          }
        }  // for( UInt_t uCluster = 0; uCluster < fviClusterSize[iSmType][iRpc].size(); uCluster++ )
        fviClusterSize[iSmType][iRpc].clear();
        fviTrkMul[iSmType][iRpc].clear();
        fvdX[iSmType][iRpc].clear();
        fvdY[iSmType][iRpc].clear();
        fvdDifX[iSmType][iRpc].clear();
        fvdDifY[iSmType][iRpc].clear();
        fvdDifCh[iSmType][iRpc].clear();
      }  // for( Int_t iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc( iSmType); iRpc++ )
    }
    fhNbSameSide->Fill(fiNbSameSide);
  }

  return kTRUE;
}

Bool_t CbmTofCosmicClusterizer::WriteHistos() {
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* fHist;
  fHist = new TFile(fOutHstFileName, "RECREATE");
  fHist->cd();
  //   fhClustBuildTime->Write();

  for (Int_t iDetIndx = 0; iDetIndx < fDigiBdfPar->GetNbDet(); iDetIndx++) {
    if (NULL == fhRpcCluMul[iDetIndx]) continue;
    //     fhRpcCluMul[iDetIndx]->Write();
    //     fhRpcCluRate[iDetIndx]->Write();
    //     fhRpcCluPosition[iDetIndx]->Write();
    //     fhRpcCluDelPos[iDetIndx]->Write();
    //     fhRpcCluTOff[iDetIndx]->Write();
    //     fhRpcCluDelTOff[iDetIndx]->Write();
    //     fhRpcCluTrms[iDetIndx]->Write();
    //     fhRpcCluTot[iDetIndx]->Write();
    //     fhRpcCluAvWalk[iDetIndx]->Write();
    //     fhRpcCluAvLnWalk[iDetIndx]->Write();
    //     fhRpcDTLastHits[iDetIndx]->Write();
    //     fhRpcDTLastHits_Tot[iDetIndx]->Write();
    //     fhRpcDTLastHits_CluSize[iDetIndx]->Write();

    LOG(debug) << "Write triggered Histos for Det Ind " << iDetIndx
               << Form(", UID 0x%08x", fDigiBdfPar->GetDetUId(iDetIndx));
    for (Int_t iSel = 0; iSel < iNSel;
         iSel++) {  // Save trigger selected histos
      if (NULL == fhTRpcCluMul[iDetIndx][iSel]) continue;
      //       fhTRpcCluMul[iDetIndx][iSel]->Write();
      //       fhTRpcCluPosition[iDetIndx][iSel]->Write();
      //       fhTRpcCluTOff[iDetIndx][iSel]->Write();
      //       fhTRpcCluTot[iDetIndx][iSel]->Write();
      //       fhTRpcCluAvWalk[iDetIndx][iSel]->Write();
    }

    Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);
    Int_t iSmType   = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSm       = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpc      = CbmTofAddress::GetRpcId(iUniqueId);
    Int_t iSmAddr   = iUniqueId & ModMask;

    Int_t iNent = 0;
    if (fCalSel > -1) {
      if (NULL == fhTRpcCluAvWalk[iDetIndx][fCalSel]) continue;
      iNent = (Int_t) fhTRpcCluAvWalk[iDetIndx][fCalSel]->GetEntries();
    } else {
      if (NULL == fhRpcCluAvWalk[iDetIndx]) continue;
      iNent = (Int_t) fhRpcCluAvWalk[iDetIndx]->GetEntries();
    }
    if (0 == iNent) {
      LOG(info) << "WriteHistos: No entries in Walk histos for "
                << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc;
      // continue;
    }

    TH2* htempPos           = NULL;
    TProfile* htempPos_pfx  = NULL;
    TH1* htempPos_py        = NULL;
    TProfile* htempTOff_pfx = NULL;
    TH1* htempTOff_px       = NULL;
    TProfile* hAvPos_pfx    = NULL;
    TProfile* hAvTOff_pfx   = NULL;
    TH2* htempTOff =
      NULL;  // -> Comment to remove warning because set but never used
    TH2* htempTot          = NULL;
    TProfile* htempTot_pfx = NULL;
    TH1* htempTot_Mean     = NULL;
    TH1* htempTot_Off      = NULL;

    if (-1 < fCalSel) {
      htempPos = fhRpcCluPosition
        [iDetIndx];  // use untriggered distributions for position
      htempPos_pfx = fhRpcCluPosition[iDetIndx]->ProfileX(
        "_pfx", 1, fhRpcCluPosition[iDetIndx]->GetNbinsY());
      //htempPos      = fhTRpcCluPosition[iDetIndx][fCalSel];
      //htempPos_pfx  = fhTRpcCluPosition[iDetIndx][fCalSel]->ProfileX("_pfx",1,fhTRpcCluPosition[iDetIndx][fCalSel]->GetNbinsY());
      htempTOff = fhTRpcCluTOff
        [iDetIndx]
        [fCalSel];  // -> Comment to remove warning because set but never used
      htempTOff_pfx = htempTOff->ProfileX(
        "_pfx", 1, fhTRpcCluTOff[iDetIndx][fCalSel]->GetNbinsY());
      htempTOff_px = htempTOff->ProjectionX(
        "_px", 1, fhTRpcCluTOff[iDetIndx][fCalSel]->GetNbinsY());
      htempTot     = fhTRpcCluTot[iDetIndx][fCalSel];
      htempTot_pfx = fhTRpcCluTot[iDetIndx][fCalSel]->ProfileX(
        "_pfx", 1, fhTRpcCluTot[iDetIndx][fCalSel]->GetNbinsY());
      hAvPos_pfx = fhTSmCluPosition[iSmType][fCalSel]->ProfileX(
        "_pfx", 1, fhTSmCluPosition[iSmType][fCalSel]->GetNbinsY());
      hAvTOff_pfx = fhTSmCluTOff[iSmType][fCalSel]->ProfileX(
        "_pfx", 1, fhTSmCluTOff[iSmType][fCalSel]->GetNbinsY(), "s");
    } else  // all triggers
    {
      htempPos     = fhRpcCluPosition[iDetIndx];
      htempTot     = fhRpcCluTot[iDetIndx];
      htempTot_pfx = fhRpcCluTot[iDetIndx]->ProfileX(
        "_pfx", 1, fhRpcCluTot[iDetIndx]->GetNbinsY());
      hAvPos_pfx = fhSmCluPosition[iSmType]->ProfileX(
        "_pfx", 1, fhSmCluPosition[iSmType]->GetNbinsY());
      hAvTOff_pfx = fhSmCluTOff[iSmType]->ProfileX(
        "_pfx", 1, fhSmCluTOff[iSmType]->GetNbinsY());
      if (-1 == fCalSel) {  // take corrections from untriggered distributions
        htempPos_pfx = fhRpcCluPosition[iDetIndx]->ProfileX(
          "_pfx", 1, fhRpcCluPosition[iDetIndx]->GetNbinsY());
        //       htempTOff     = fhRpcCluTOff[iDetIndx]; // -> Comment to remove warning because set but never used
        htempTOff_pfx = fhRpcCluTOff[iDetIndx]->ProfileX(
          "_pfx", 1, fhRpcCluTOff[iDetIndx]->GetNbinsY(), "s");
        htempTOff_px = fhRpcCluTOff[iDetIndx]->ProjectionX(
          "_px", 1, fhRpcCluTOff[iDetIndx]->GetNbinsY());
      } else {
        if (-2 == fCalSel) {  //take corrections from Cluster deviations
          htempPos_pfx = fhRpcCluDelPos[iDetIndx]->ProfileX(
            "_pfx", 1, fhRpcCluDelPos[iDetIndx]->GetNbinsY());
          //         htempTOff     = fhRpcCluDelTOff[iDetIndx]; // -> Comment to remove warning because set but never used
          htempTOff_pfx = fhRpcCluDelTOff[iDetIndx]->ProfileX(
            "_pfx", 1, fhRpcCluDelTOff[iDetIndx]->GetNbinsY());
          htempTOff_px = fhRpcCluDelTOff[iDetIndx]->ProjectionX(
            "_px", 1, fhRpcCluDelTOff[iDetIndx]->GetNbinsY());
        } else {
          if (
            -3
            == fCalSel) {  // take corrections from deviations to matched trigger hit
            htempPos_pfx = fhRpcCluDelMatPos[iDetIndx]->ProfileX(
              "_pfx", 1, fhRpcCluDelMatPos[iDetIndx]->GetNbinsY());
            //           htempTOff     = fhRpcCluDelMatTOff[iDetIndx]; // -> Comment to remove warning because set but never used
            htempTOff_pfx = fhRpcCluDelMatTOff[iDetIndx]->ProfileX(
              "_pfx", 1, fhRpcCluDelMatTOff[iDetIndx]->GetNbinsY());
            htempTOff_px = fhRpcCluDelMatTOff[iDetIndx]->ProjectionX(
              "_px", 1, fhRpcCluDelMatTOff[iDetIndx]->GetNbinsY());
          }
        }
      }
    }

    if (NULL == htempPos_pfx) {
      LOG(info) << "WriteHistos: Projections not available, continue ";
      continue;
    }

    htempTot_Mean = htempTot_pfx->ProjectionX("_Mean");
    htempTot_Off  = htempTot_pfx->ProjectionX("_Off");

    htempPos_pfx->SetName(
      Form("cl_CorSmT%01d_sm%03d_rpc%03d_Pos_pfx", iSmType, iSm, iRpc));
    htempTOff_pfx->SetName(
      Form("cl_CorSmT%01d_sm%03d_rpc%03d_TOff_pfx", iSmType, iSm, iRpc));
    htempTot_pfx->SetName(
      Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_pfx", iSmType, iSm, iRpc));
    htempTot_Mean->SetName(
      Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Mean", iSmType, iSm, iRpc));
    htempTot_Off->SetName(
      Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Off", iSmType, iSm, iRpc));
    hAvPos_pfx->SetName(Form("cl_CorSmT%01d_Pos_pfx", iSmType));
    hAvTOff_pfx->SetName(Form("cl_CorSmT%01d_TOff_pfx", iSmType));

    switch (fCalMode % 10) {
      case 0: {                 // Initialize
        htempTot_Off->Reset();  // prepare TotOffset histo
        TH1* hbins[200];
        Int_t nbins = htempTot->GetNbinsX();
        for (int i = 0; i < nbins; i++) {
          hbins[i] = htempTot->ProjectionY(Form("bin%d", i + 1), i + 1, i + 1);
          /*         Double_t Ymax=hbins[i]->GetMaximum();*/
          Int_t iBmax   = hbins[i]->GetMaximumBin();
          TAxis* xaxis  = hbins[i]->GetXaxis();
          Double_t Xmax = xaxis->GetBinCenter(iBmax);
          Double_t XOff = Xmax - fTotPreRange;
          XOff          = (Double_t)(Int_t) XOff;
          if (XOff < 0) XOff = 0;
          htempTot_Off->SetBinContent(i + 1, XOff);
          Double_t Xmean = htempTot_Mean->GetBinContent(i + 1);
          if (Xmean < XOff) {
            LOG(warning) << "Inconsistent  Tot numbers for "
                         << Form(
                              "SmT%01d_sm%03d_rpc%03d bin%d: mean %f, Off %f",
                              iSmType,
                              iSm,
                              iRpc,
                              i,
                              Xmean,
                              XOff);
          }
          htempTot_Mean->SetBinContent(i + 1, (Xmean - XOff));
          if (htempTot_Mean->GetBinContent(i + 1) != (Xmean - XOff))
            LOG(warning)
              << "Tot numbers not stored properly for "
              << Form("SmT%01d_sm%03d_rpc%03d bin%d: mean %f, target %f",
                      iSmType,
                      iSm,
                      iRpc,
                      i,
                      htempTot_Mean->GetBinContent(i + 1),
                      Xmean - XOff);
        }
        htempPos_pfx->Write();
        htempTOff_pfx->Write();
        //       htempTot_pfx->Write();
        htempTot_Mean->Write();
        htempTot_Off->Write();
      } break;
      case 1:  //save offsets, update walks
      {
        Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
        Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
        LOG(info) << "WriteHistos: restore Offsets and Gains and save Walk for "
                  << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc
                  << " and calSmAddr = "
                  << Form(" 0x%08x ", TMath::Abs(fCalSmAddr));
        htempPos_pfx->Reset();  //reset to restore means of original histos
        htempTOff_pfx->Reset();
        htempTot_Mean->Reset();
        htempTot_Off->Reset();
        for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
          Double_t YMean = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          Double_t TMean = 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          htempPos_pfx->Fill(iCh, YMean);
          if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
            LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh
                       << " got " << htempPos_pfx->GetBinContent(iCh) << ","
                       << htempPos_pfx->GetBinContent(iCh + 1) << ","
                       << htempPos_pfx->GetBinContent(iCh + 2) << ", expected "
                       << YMean;
          }
          htempTOff_pfx->Fill(iCh, TMean);

          for (Int_t iSide = 0; iSide < 2; iSide++) {
            htempTot_Mean->SetBinContent(
              iCh * 2 + 1 + iSide,
              fdTTotMean
                / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh]
                             [iSide]);  //nh +1 empirical(?)
            htempTot_Off->SetBinContent(
              iCh * 2 + 1 + iSide,
              fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
          }
        }

        LOG(debug1) << " Offset, gain restoring done ... ";
        htempPos_pfx->Write();
        htempTOff_pfx->Write();
        //        htempTot_pfx->Write();
        htempTot_Mean->Write();
        htempTot_Off->Write();

        for (Int_t iSel = 0; iSel < iNSel; iSel++) {
          // Store DelTof corrections
          TDirectory* curdir = gDirectory;
          gROOT->cd();  //
          TH1D* hCorDelTof = (TH1D*) gDirectory->FindObjectAny(
            Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                 iSmType,
                 iSm,
                 iRpc,
                 iSel));
          gDirectory->cd(curdir->GetPath());
          if (NULL != hCorDelTof) {
            TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                   iSmType,
                   iSm,
                   iRpc,
                   iSel));
            hCorDelTofout->Write();
          } else {
            LOG(debug) << " No CorDelTof histo "
                       << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                               iSmType,
                               iSm,
                               iRpc,
                               iSel);
          }
        }

        if (
          (fCalSmAddr < 0 && TMath::Abs(fCalSmAddr) != iSmAddr)
          || fCalSmAddr
               == iSmAddr)  // select detectors for determination of walk correction
        {

          LOG(info) << "restore Offsets and Gains and update Walk for "
                    << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc
                    << " with " << fDigiBdfPar->GetNbChan(iSmType, iRpc)
                    << " channels";
          for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpc);
               iCh++) {
            TH2* h2tmp0;
            TH2* h2tmp1;
            if (!fEnableAvWalk) {
              if (-1 < fCalSel) {
                h2tmp0 = fhTRpcCluWalk[iDetIndx][fCalSel][iCh][0];
                h2tmp1 = fhTRpcCluWalk[iDetIndx][fCalSel][iCh][1];
              } else {  // take correction from deviation within clusters
                h2tmp0 = fhRpcCluWalk[iDetIndx][iCh][0];
                h2tmp1 = fhRpcCluWalk[iDetIndx][iCh][1];
              }
            } else {  // go for averages (low statistics)
              if (-1 < fCalSel) {
                h2tmp0 = fhTRpcCluAvWalk[iDetIndx][fCalSel];
                h2tmp1 = fhTRpcCluAvWalk[iDetIndx][fCalSel];
              } else {  // take correction from deviation within clusters
                h2tmp0 = fhRpcCluAvWalk[iDetIndx];
                h2tmp1 = fhRpcCluAvWalk[iDetIndx];
              }
            }
            if (NULL == h2tmp0) {
              LOG(info) << Form("WriteHistos: Walk histo not available for SmT "
                                "%d, Sm %d, Rpc %d, Ch %d",
                                iSmType,
                                iSm,
                                iRpc,
                                iCh);
              continue;
            }
            Int_t iNEntries = h2tmp0->GetEntries();
            //if(iCh==0)  // condition to print message only once
            LOG(info) << Form(" Update Walk correction for SmT %d, Sm %d, Rpc "
                              "%d, Ch %d, Sel%d: Entries %d",
                              iSmType,
                              iSm,
                              iRpc,
                              iCh,
                              fCalSel,
                              iNEntries);

            //         h2tmp0->Write();
            //         h2tmp1->Write();
            if (-1 < iNEntries) {  // always done
              TProfile* htmp0 =
                h2tmp0->ProfileX("_pfx", 1, h2tmp0->GetNbinsY());
              TProfile* htmp1 =
                h2tmp1->ProfileX("_pfx", 1, h2tmp1->GetNbinsY());
              TH1D* h1tmp0 = h2tmp0->ProjectionX("_px", 1, h2tmp0->GetNbinsY());
              TH1D* h1tmp1 = h2tmp1->ProjectionX("_px", 1, h2tmp1->GetNbinsY());
              TH1D* h1ytmp0 =
                h2tmp0->ProjectionY("_py", 1, nbClWalkBinX);  // preserve means
              TH1D* h1ytmp1    = h2tmp1->ProjectionY("_py", 1, nbClWalkBinX);
              Double_t dWMean0 = h1ytmp0->GetMean();
              Double_t dWMean1 = h1ytmp1->GetMean();
              Double_t dWMean  = 0.5 * (dWMean0 + dWMean1);
              Int_t iWalkUpd   = 2;  // Walk update mode flag
              //if(5==iSmType || 8==iSmType || 2==iSmType) iWalkUpd=0; // keep both sides consistent for diamonds and pads
              if (5 == iSmType || 8 == iSmType)
                iWalkUpd =
                  0;  // keep both sides consistent for diamonds and pads (Cern2016)
              for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
                switch (iWalkUpd) {
                  case 0:
                    if (h1tmp0->GetBinContent(iWx + 1) > WalkNHmin
                        && h1tmp1->GetBinContent(iWx + 1) > WalkNHmin) {
                      // preserve y - position (difference) on average
                      Double_t dWcor =
                        (((TProfile*) htmp0)->GetBinContent(iWx + 1)
                         + ((TProfile*) htmp1)->GetBinContent(iWx + 1))
                        * 0.5;
                      fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx] +=
                        dWcor - dWMean;
                      fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx] +=
                        dWcor - dWMean;
                    }
                    break;
                  case 1:
                    if (h1tmp0->GetBinContent(iWx + 1) > WalkNHmin
                        && h1tmp1->GetBinContent(iWx + 1) > WalkNHmin) {
                      Double_t dWcor0 =
                        ((TProfile*) htmp0)->GetBinContent(iWx + 1) - dWMean0;
                      Double_t dWcor1 =
                        ((TProfile*) htmp1)->GetBinContent(iWx + 1) - dWMean1;
                      Double_t dWcor = 0.5 * (dWcor0 + dWcor1);
                      fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx] +=
                        dWcor;  //-dWMean0;
                      fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx] +=
                        dWcor;  //-dWMean1;

                      if (iCh == 10 && iSmType == 0 && iSm == 1
                          && h1tmp0->GetBinContent(iWx + 1) > WalkNHmin)
                        LOG(debug)
                          << "Update Walk Sm = " << iSm << "(" << iNbRpc
                          << "), Rpc " << iRpc << ", Bin " << iWx << ", "
                          << h1tmp0->GetBinContent(iWx + 1) << " cts: "
                          << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]
                          << " + "
                          << ((TProfile*) htmp0)->GetBinContent(iWx + 1)
                          << " - " << dWMean0 << " -> " << dWcor - dWMean0
                          << ", S1: "
                          << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]
                          << " + "
                          << ((TProfile*) htmp1)->GetBinContent(iWx + 1)
                          << " - " << dWMean1 << " -> " << dWcor - dWMean1;
                    }
                    break;
                  case 2:
                    if (h1tmp0->GetBinContent(iWx + 1) > WalkNHmin
                        && h1tmp1->GetBinContent(iWx + 1) > WalkNHmin) {
                      Double_t dWcor0 =
                        ((TProfile*) htmp0)->GetBinContent(iWx + 1) - dWMean0;
                      Double_t dWcor1 =
                        ((TProfile*) htmp1)->GetBinContent(iWx + 1) - dWMean1;
                      //Double_t dWcor = 0.5*(dWcor0 + dWcor1);
                      fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx] +=
                        dWcor0;
                      fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx] +=
                        dWcor1;

                      if (iCh == 10 && iSmType == 0
                          && h1tmp0->GetBinContent(iWx + 1) > WalkNHmin)
                        LOG(info)
                          << "Update Walk Sm = " << iSm << "(" << iNbRpc
                          << "), Rpc " << iRpc << ", Bin " << iWx << ", "
                          << h1tmp0->GetBinContent(iWx + 1) << " cts: "
                          << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]
                          << " + "
                          << ((TProfile*) htmp0)->GetBinContent(iWx + 1)
                          << " - " << dWMean0 << " -> " << dWcor0 - dWMean0
                          << ", S1: "
                          << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]
                          << " + "
                          << ((TProfile*) htmp1)->GetBinContent(iWx + 1)
                          << " - " << dWMean1 << " -> " << dWcor1 - dWMean1;
                    }
                    break;

                  default:;
                }
              }
              h1tmp0->Reset();
              h1tmp1->Reset();
              for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
                h1tmp0->SetBinContent(
                  iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
                h1tmp1->SetBinContent(
                  iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
                Int_t iTry = 3;
                while (
                  iTry-- > 0
                  && h1tmp0->GetBinContent(iWx + 1)
                       != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
                  h1tmp0->SetBinContent(
                    iWx + 1,
                    fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
                }
                if (iTry == 0)
                  LOG(error)
                    << "writing not successful for " << h1tmp0->GetName()
                    << ", attempts left: " << iTry << ", iWx " << iWx
                    << ", got " << h1tmp0->GetBinContent(iWx + 1)
                    << ", expected "
                    << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
                iTry = 3;
                while (
                  iTry-- > 0
                  && h1tmp1->GetBinContent(iWx + 1)
                       != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]) {
                  h1tmp1->SetBinContent(
                    iWx + 1,
                    fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
                }
                if (iTry == 0)
                  LOG(error)
                    << "writing not successful for " << h1tmp1->GetName()
                    << ", attempts left: " << iTry << ", iWx " << iWx
                    << ", got " << h1tmp1->GetBinContent(iWx + 1)
                    << ", expected "
                    << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx];
              }

              h1tmp0->SetName(
                Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",
                     iSmType,
                     iSm,
                     iRpc,
                     iCh));
              h1tmp0->Smooth(iNWalkSmooth);
              h1tmp0->Write();
              h1tmp1->SetName(
                Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",
                     iSmType,
                     iSm,
                     iRpc,
                     iCh));
              h1tmp1->Smooth(iNWalkSmooth);
              h1tmp1->Write();
            }
          }
        } else {  // preserve whatever is there for fCalSmAddr !
          for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpc);
               iCh++)  // restore old values
          {
            // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
            // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
            TH1D* h1tmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX(
              "_px", 1, nbClWalkBinY);
            TH1D* h1tmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX(
              "_px", 1, nbClWalkBinY);
            for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
              h1tmp0->SetBinContent(
                iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
              h1tmp1->SetBinContent(
                iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
              if (h1tmp0->GetBinContent(iWx + 1)
                  != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
                LOG(error)
                  << "WriteHistos: restore unsuccessful! iWx " << iWx << " got "
                  << h1tmp0->GetBinContent(iWx + 1) << ", expected "
                  << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
              }
            }
            h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",
                                 iSmType,
                                 iSm,
                                 iRpc,
                                 iCh));
            //          htmp0->Write();
            h1tmp0->Write();
            h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",
                                 iSmType,
                                 iSm,
                                 iRpc,
                                 iCh));
            //          htmp1->Write();
            h1tmp1->Write();
          }
        }
      } break;

      case 2:  //update time offsets from positions and times with Sm averages, save walks and DELTOF
      {
        Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
        Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);

        if ((fCalSmAddr < 0)
            || (fCalSmAddr
                != iSmAddr)) {  // select detectors for updating offsets
          LOG(debug) << "WriteHistos: (case 2) update Offsets and keep Gains, "
                        "Walk and DELTOF for "
                     << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc "
                     << iRpc;
          Int_t iB        = iSm * iNbRpc + iRpc;
          Double_t dVscal = 1.;
          if (NULL != fhSmCluSvel[iSmType])
            dVscal =
              fhSmCluSvel[iSmType]->GetBinContent(iSm * iNbRpc + iRpc + 1);
          if (dVscal == 0.) dVscal = 1.;

          Double_t YMean = ((TProfile*) hAvPos_pfx)
                             ->GetBinContent(iB + 1);  //nh +1 empirical(?)
          htempPos_py =
            htempPos->ProjectionY(Form("%s_py", htempPos->GetName()), 1, iNbCh);
          if (htempPos_py->GetEntries() > fdYFitMin && fPosYMaxScal < 1.1) {
            LOG(debug1) << Form("Determine YMean in %s by fit to %d entries",
                                htempPos->GetName(),
                                (Int_t) htempPos_py->GetEntries());
            CbmTofDetectorInfo xDetInfo(
              ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, 0);
            Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
            fChannelInfo = fDigiPar->GetCell(iChId);
            if (NULL == fChannelInfo) {
              LOG(warning) << Form("invalid ChannelInfo for 0x%08x", iChId);
              continue;
            }
            fit_ybox(htempPos_py, 0.5 * fChannelInfo->GetSizey());
            TF1* ff = htempPos_py->GetFunction("YBox");
            if (NULL != ff) {
              LOG(info) << "FRes YBox " << htempPos_py->GetEntries()
                        << " entries in TSR " << iSmType << iSm << iRpc
                        << ", chi2 " << ff->GetChisquare() / ff->GetNDF()
                        << Form(
                             ", striplen (%5.2f), %4.2f: %7.2f +/- %5.2f, pos "
                             "res %5.2f +/- %5.2f at y_cen = %5.2f +/- %5.2f",
                             fChannelInfo->GetSizey(),
                             dVscal,
                             2. * ff->GetParameter(1),
                             2. * ff->GetParError(1),
                             ff->GetParameter(2),
                             ff->GetParError(2),
                             ff->GetParameter(3),
                             ff->GetParError(3));

              if (TMath::Abs(fChannelInfo->GetSizey()
                             - 2. * ff->GetParameter(1))
                      / fChannelInfo->GetSizey()
                    < 0.2
                  && TMath::Abs(ff->GetParError(1) / ff->GetParameter(1)) < 0.2)
              //	 &&  ff->GetChisquare() < 500.)   //FIXME - constants!
              {
                if (TMath::Abs(ff->GetParameter(3) - YMean)
                    < 0.5 * fChannelInfo->GetSizey()) {
                  YMean       = ff->GetParameter(3);
                  Double_t dV = dVscal * fChannelInfo->GetSizey()
                                / (2. * ff->GetParameter(1));
                  fhSmCluSvel[iSmType]->Fill((Double_t)(iSm * iNbRpc + iRpc),
                                             dV);
                }
              }
            }
          }


          Double_t TMean  = ((TProfile*) hAvTOff_pfx)->GetBinContent(iSm + 1);
          Double_t TWidth = ((TProfile*) hAvTOff_pfx)->GetBinError(iSm + 1);
          Double_t dTYOff = YMean / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);


          //if (fSel2Addr == iSmAddr) TMean=0.; // don't shift reference counter
          //if (fSel2Id == iSmType && fSel2Sm == iSm && fSel2Rpc == iRpc) TMean=0.; // don't shift reference counter

          LOG(debug) << Form("<ICor> Correct %d %d %d by TMean=%8.2f, "
                             "TYOff=%8.2f, TWidth=%8.2f, ",
                             iSmType,
                             iSm,
                             iRpc,
                             TMean,
                             dTYOff,
                             TWidth);


          for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // update Offset and Gain
          {
            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] += -dTYOff + TMean;
            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] += +dTYOff + TMean;

            LOG(debug) << "FillCalHist:"
                       << " SmT " << iSmType << " Sm " << iSm << " Rpc " << iRpc
                       << " Ch " << iCh << ": YMean " << YMean << ", TMean "
                       << TMean << " -> "
                       << Form(
                            " %f, %f, %f, %f ",
                            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1],
                            fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                            fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
          }  // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
        }
        htempPos_pfx->Reset();  //reset to store new values
        htempTOff_pfx->Reset();
        htempTot_Mean->Reset();
        htempTot_Off->Reset();
        for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // store new values
        {
          Double_t YMean = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          Double_t TMean = 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          htempPos_pfx->Fill(iCh, YMean);
          if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
            LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh
                       << " got " << htempPos_pfx->GetBinContent(iCh) << ","
                       << htempPos_pfx->GetBinContent(iCh + 1) << ","
                       << htempPos_pfx->GetBinContent(iCh + 2) << ", expected "
                       << YMean;
          }
          htempTOff_pfx->Fill(iCh, TMean);

          for (Int_t iSide = 0; iSide < 2; iSide++) {
            htempTot_Mean->SetBinContent(
              iCh * 2 + 1 + iSide,
              fdTTotMean
                / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh]
                             [iSide]);  //nh +1 empirical(?)
            htempTot_Off->SetBinContent(
              iCh * 2 + 1 + iSide,
              fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
          }
        }  // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )

        LOG(debug1) << " Updating done ... write to file ";
        htempPos_pfx->Write();
        htempTOff_pfx->Write();
        //        htempTot_pfx->Write();
        htempTot_Mean->Write();
        htempTot_Off->Write();

        // store old DELTOF histos
        LOG(debug) << " Copy old DelTof histos from " << gDirectory->GetName()
                   << " to file ";

        for (Int_t iSel = 0; iSel < iNSel; iSel++) {
          // Store DelTof corrections
          TDirectory* curdir = gDirectory;
          gROOT->cd();  //
          TH1D* hCorDelTof = (TH1D*) gDirectory->FindObjectAny(
            Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                 iSmType,
                 iSm,
                 iRpc,
                 iSel));
          gDirectory->cd(curdir->GetPath());
          if (NULL != hCorDelTof) {
            TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                   iSmType,
                   iSm,
                   iRpc,
                   iSel));
            hCorDelTofout->Write();
          } else {
            LOG(debug) << " No CorDelTof histo "
                       << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                               iSmType,
                               iSm,
                               iRpc,
                               iSel);
          }
        }

        // store walk histos
        for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // store new values
        {
          // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
          // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
          TH1D* h1tmp0 =
            fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX("_px", 1, nbClWalkBinY);
          TH1D* h1tmp1 =
            fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX("_px", 1, nbClWalkBinY);
          for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
            h1tmp0->SetBinContent(
              iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
            h1tmp1->SetBinContent(
              iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
            if (h1tmp0->GetBinContent(iWx + 1)
                != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
              LOG(error) << "WriteHistos: restore unsuccessful! iWx " << iWx
                         << " got " << h1tmp0->GetBinContent(iWx + 1)
                         << ", expected "
                         << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
            }
          }
          h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",
                               iSmType,
                               iSm,
                               iRpc,
                               iCh));
          //          htmp0->Write();
          h1tmp0->Write();
          h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",
                               iSmType,
                               iSm,
                               iRpc,
                               iCh));
          //          htmp1->Write();
          h1tmp1->Write();
        }
      } break;

      case 3:  //update offsets, gains, save walks and DELTOF
      {
        Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
        Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
        if ((fCalSmAddr < 0)
            || (fCalSmAddr
                != iSmAddr)) {  // select detectors for updating offsets
          LOG(info) << Form("calMode==3: update Offsets and Gains, keep Walk "
                            "and DelTof for 0x%08x, ",
                            iSmAddr)
                    << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc
                    << " with " << iNbCh << " channels "
                    << " using selector " << fCalSel;
          /*
         Double_t dTRefMean=0.;
         if (5 == iSmType && fTRefMode%10 == iSm){   // reference counter
           dTRefMean=htempTOff->GetMean(2);
         }
         */
          Double_t dVscal = 1.;
          Double_t dVW    = 1.;
          if (0)  // NULL != fhSmCluSvel[iSmType])
          {
            dVscal =
              fhSmCluSvel[iSmType]->GetBinContent(iSm * iNbRpc + iRpc + 1);
            if (dVscal == 0.) dVscal = 1.;
            dVW = fhSmCluSvel[iSmType]->GetBinEffectiveEntries(iSm * iNbRpc
                                                               + iRpc + 1);
            dVW *= 50.;  //(Double_t)iNbCh;
            if (dVW < 0.1) dVW = 0.1;
          }

          // determine average values
          htempPos_py =
            htempPos->ProjectionY(Form("%s_py", htempPos->GetName()), 1, iNbCh);
          LOG(info) << Form("Inspect histo %s with %d entries",
                            htempPos->GetName(),
                            (Int_t) htempPos_py->GetEntries());
          Double_t dYMeanAv  = 0.;
          Double_t dYMeanFit = 0.;
          if (htempPos_py->GetEntries() > fdYFitMin && fPosYMaxScal < 1.1) {
            dYMeanAv = htempPos_py->GetMean();
            LOG(debug1) << Form("Determine YMeanAv in %s by fit to %d entries",
                                htempPos->GetName(),
                                (Int_t) htempPos_py->GetEntries());
            CbmTofDetectorInfo xDetInfo(
              ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, 0);
            Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
            fChannelInfo = fDigiPar->GetCell(iChId);
            if (NULL == fChannelInfo) {
              LOG(warning) << Form("invalid ChannelInfo for 0x%08x", iChId);
              continue;
            }
            fit_ybox(htempPos_py, 0.5 * fChannelInfo->GetSizey());
            TF1* ff = htempPos_py->GetFunction("YBox");
            if (NULL != ff) {
              if (TMath::Abs(fChannelInfo->GetSizey()
                             - 2. * ff->GetParameter(1))
                      / fChannelInfo->GetSizey()
                    < 0.2
                  && TMath::Abs(ff->GetParError(1) / ff->GetParameter(1))
                       < 0.2) {
                Double_t dV = dVscal * fChannelInfo->GetSizey()
                              / (2. * ff->GetParameter(1));
                LOG(info) << "FAvRes YBox " << htempPos_py->GetEntries()
                          << " entries in TSR " << iSmType << iSm << iRpc
                          << ", stat: " << gMinuit->fCstatu << ", chi2 "
                          << ff->GetChisquare() / ff->GetNDF()
                          << Form(", striplen (%5.2f): %7.2f+/-%5.2f, pos res "
                                  "%5.2f+/-%5.2f at y_cen = %5.2f+/-%5.2f",
                                  fChannelInfo->GetSizey(),
                                  2. * ff->GetParameter(1),
                                  2. * ff->GetParError(1),
                                  ff->GetParameter(2),
                                  ff->GetParError(2),
                                  ff->GetParameter(3),
                                  ff->GetParError(3));
                if (TMath::Abs(ff->GetParameter(3) - dYMeanAv)
                    < 0.5 * fChannelInfo->GetSizey()) {
                  dYMeanFit = ff->GetParameter(3);
                  fhSmCluSvel[iSmType]->Fill(
                    (Double_t)(iSm * iNbRpc + iRpc), dV, dVW);
                  for (Int_t iPar = 0; iPar < 4; iPar++)
                    fhSmCluFpar[iSmType][iPar]->Fill(
                      (Double_t)(iSm * iNbRpc + iRpc),
                      ff->GetParameter(2 + iPar));
                }
              } else {
                LOG(info)
                  << "FAvBad YBox " << htempPos_py->GetEntries()
                  << " entries in TSR " << iSmType << iSm << iRpc << ", chi2 "
                  << ff->GetChisquare()

                  << Form(", striplen (%5.2f), %4.2f: %7.2f +/- %5.2f, pos res "
                          "%5.2f +/- %5.2f at y_cen = %5.2f +/- %5.2f",
                          fChannelInfo->GetSizey(),
                          dVscal,
                          2. * ff->GetParameter(1),
                          2. * ff->GetParError(1),
                          ff->GetParameter(2),
                          ff->GetParError(2),
                          ff->GetParameter(3),
                          ff->GetParError(3));
              }
            } else {
              LOG(info) << "FAvFailed for TSR " << iSmType << iSm << iRpc;
            }
          }
          Double_t dYShift = dYMeanFit - dYMeanAv;

          if (!(fSel2Addr == iSmAddr
                && fTRefMode > 10))  // condition parameter update
            for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // update Offset and Gain
            {
              Double_t YMean = ((TProfile*) htempPos_pfx)
                                 ->GetBinContent(iCh + 1);  //set default
              YMean += dYShift;

              htempPos_py = htempPos->ProjectionY(
                Form("%s_py%02d", htempPos->GetName(), iCh), iCh + 1, iCh + 1);
              if (htempPos_py->GetEntries() > fdYFitMin
                  && fPosYMaxScal < -1.1) {  //disabled
                LOG(debug1) << Form(
                  "Determine YMean in %s of channel %d by fit to %d entries",
                  htempPos->GetName(),
                  iCh,
                  (Int_t) htempPos_py->GetEntries());
                CbmTofDetectorInfo xDetInfo(
                  ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
                Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
                fChannelInfo = fDigiPar->GetCell(iChId);
                if (NULL == fChannelInfo) {
                  LOG(warning) << Form("invalid ChannelInfo for 0x%08x", iChId);
                  continue;
                }
                Double_t fp[4] = {1., 3 * 0.};  // initialize fit parameter
                for (Int_t iPar = 2; iPar < 4; iPar++)
                  if (NULL != fhSmCluFpar[iSmType][iPar])
                    fp[iPar] = fhSmCluFpar[iSmType][iPar]->GetBinContent(
                      iSm * iNbRpc + iRpc + 1);
                //LOG(info) << Form("Call yFit with %6.3f, %6.3f, %6.3f, %6.3f",fp[0],fp[1],fp[2],fp[3])
                //          ;
                Double_t* fpp = &fp[0];
                fit_ybox(htempPos_py, 0.5 * fChannelInfo->GetSizey(), fpp);
                TF1* ff = htempPos_py->GetFunction("YBox");
                if (NULL != ff) {
                  if (TMath::Abs(fChannelInfo->GetSizey()
                                 - 2. * ff->GetParameter(1))
                          / fChannelInfo->GetSizey()
                        < 0.1
                      && TMath::Abs(ff->GetParError(1) / ff->GetParameter(1))
                           < 0.05)
                  //&&  ff->GetChisquare() < 200.)   //FIXME - constants!
                  {
                    if (TMath::Abs(ff->GetParameter(3) - YMean)
                        < 0.5 * fChannelInfo->GetSizey()) {
                      YMean       = ff->GetParameter(3);
                      Double_t dV = dVscal * fChannelInfo->GetSizey()
                                    / (2. * ff->GetParameter(1));
                      fhSmCluSvel[iSmType]->Fill(
                        (Double_t)(iSm * iNbRpc + iRpc), dV, dVW);
                      LOG(info)
                        << "FRes YBox " << htempPos_py->GetEntries()
                        << " entries in " << iSmType << iSm << iRpc << iCh
                        << ", chi2 " << ff->GetChisquare()
                        << Form(", striplen (%5.2f), %4.2f -> %4.2f,  %4.1f: "
                                "%7.2f+/-%5.2f, pos res %5.2f+/-%5.2f at y_cen "
                                "= %5.2f+/-%5.2f",
                                fChannelInfo->GetSizey(),
                                dVscal,
                                dV,
                                dVW,
                                2. * ff->GetParameter(1),
                                2. * ff->GetParError(1),
                                ff->GetParameter(2),
                                ff->GetParError(2),
                                ff->GetParameter(3),
                                ff->GetParError(3));
                      for (Int_t iPar = 0; iPar < 4; iPar++)
                        fhSmCluFpar[iSmType][iPar]->Fill(
                          (Double_t)(iSm * iNbRpc + iRpc),
                          ff->GetParameter(2 + iPar));
                    }
                  } else {
                    //YMean=0.;  // no new info available - did not help!
                    LOG(info)
                      << "FBad YBox " << htempPos_py->GetEntries()
                      << " entries in " << iSmType << iSm << iRpc << iCh
                      << ", chi2 " << ff->GetChisquare()
                      << Form(", striplen (%5.2f), %4.2f: %7.2f +/- %5.2f, pos "
                              "res %5.2f +/- %5.2f at y_cen = %5.2f +/- %5.2f",
                              fChannelInfo->GetSizey(),
                              dVscal,
                              2. * ff->GetParameter(1),
                              2. * ff->GetParError(1),
                              ff->GetParameter(2),
                              ff->GetParError(2),
                              ff->GetParameter(3),
                              ff->GetParError(3));
                  }
                }
              }

              Double_t TMean =
                ((TProfile*) htempTOff_pfx)->GetBinContent(iCh + 1);
              Double_t dTYOff =
                YMean / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);

              /*
	  if (fSel2Addr == iSmAddr) {
	    //if (fSel2Id == iSmType && fSel2Sm == iSm && fSel2Rpc == iRpc) {
	    // don't shift reference counter on average
            //TMean-=((TProfile *)hAvTOff_pfx)->GetBinContent(iSm+1);
          }
	  */

              if (htempTOff_px->GetBinContent(iCh + 1) > WalkNHmin) {
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] +=
                  -dTYOff + TMean;
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] +=
                  +dTYOff + TMean;
                if (iCh == 10)
                  LOG(info)
                    << Form("Calib: TSRC %d%d%d%d, hits %6.0f, dTY  %8.3f, TM "
                            "%8.3f -> new Off %8.3f,%8.3f ",
                            iSmType,
                            iSm,
                            iRpc,
                            iCh,
                            htempTOff_px->GetBinContent(iCh + 1),
                            dTYOff,
                            TMean,
                            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
              }
              /*
           Double_t TotMean=((TProfile *)htempTot_pfx)->GetBinContent(iCh+1);  //nh +1 empirical(!)
          if(0.001 < TotMean){
            fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][0] *= fdTTotMean / TotMean;
            fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][1] *= fdTTotMean / TotMean;
          }
          */
              for (Int_t iSide = 0; iSide < 2; iSide++) {
                Int_t ib  = iCh * 2 + 1 + iSide;
                TH1* hbin = htempTot->ProjectionY(Form("bin%d", ib), ib, ib);
                if (100 > hbin->GetEntries())
                  continue;  //request min number of entries
                             /*            Double_t Ymax=hbin->GetMaximum();*/
                Int_t iBmax  = hbin->GetMaximumBin();
                TAxis* xaxis = hbin->GetXaxis();
                Double_t Xmax =
                  xaxis->GetBinCenter(iBmax)
                  / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide];
                Double_t XOff = Xmax - fTotPreRange;
                if (
                  0) {  //TMath::Abs(XOff - fvCPTotOff[iSmType][iSm*iNbRpc+iRpc][iCh][iSide])>100){
                  LOG(warning)
                    << "XOff changed for "
                    << Form(
                         "SmT%01d_sm%03d_rpc%03d_Side%d: XOff %f, old %f",
                         iSmType,
                         iSm,
                         iRpc,
                         iSide,
                         XOff,
                         fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
                }
                //            Double_t TotMean=htempTot_Mean->GetBinContent(ib);
                Double_t TotMean = hbin->GetMean();
                if (15 == iSmType) {
                  LOG(warning)
                    << "Gain for "
                    << Form(
                         "SmT%01d_sm%03d_rpc%03d_Side%d: TotMean %f, prof %f, "
                         "gain %f, modg %f ",
                         iSmType,
                         iSm,
                         iRpc,
                         iSide,
                         TotMean,
                         htempTot_Mean->GetBinContent(ib),
                         fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide],
                         fdTTotMean / TotMean);
                }
                if (0.001 < TotMean) {
                  fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] *=
                    fdTTotMean / TotMean;
                }
              }
              if (5 == iSmType
                  && fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                       != fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh]
                                  [1]) {  // diamond
                LOG(warning)
                  << "CbmTofCosmicClusterizer::FillCalHist:"
                  << " SmT " << iSmType << " Sm " << iSm << " Rpc " << iRpc
                  << " Ch " << iCh << ": YMean " << YMean << ", TMean " << TMean
                  << " -> "
                  << Form(" %f %f %f %f ",
                          fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                          fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1],
                          fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                          fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
                Double_t dTOff =
                  0.5
                  * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                     + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
                Double_t dGain =
                  0.5
                  * (fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                     + fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]    = dTOff;
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]    = dTOff;
                fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0] = dGain;
                fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1] = dGain;
              }  // diamond warning end
            }    // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
        }        // iSmType selection condition

        htempPos_pfx->Reset();  //reset to store new values
        htempTOff_pfx->Reset();
        htempTot_Mean->Reset();
        htempTot_Off->Reset();
        for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // store new values
        {
          Double_t YMean = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          Double_t TMean = 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          htempPos_pfx->Fill(iCh, YMean);
          if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
            LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh
                       << " got " << htempPos_pfx->GetBinContent(iCh) << ","
                       << htempPos_pfx->GetBinContent(iCh + 1) << ","
                       << htempPos_pfx->GetBinContent(iCh + 2) << ", expected "
                       << YMean;
          }
          htempTOff_pfx->Fill(iCh, TMean);

          for (Int_t iSide = 0; iSide < 2; iSide++) {
            htempTot_Mean->SetBinContent(
              iCh * 2 + 1 + iSide,
              fdTTotMean
                / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
            htempTot_Off->SetBinContent(
              iCh * 2 + 1 + iSide,
              fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
          }
          //         htempTot_pfx->Fill(iCh,fdTTotMean/fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][1]);
        }  // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )


        LOG(debug1) << " Updating done ... write to file ";
        htempPos_pfx->Write();
        htempTOff_pfx->Write();
        //        htempTot_pfx->Write();
        htempTot_Mean->Write();
        htempTot_Off->Write();

        // store old DELTOF histos
        LOG(debug) << " Copy old DelTof histos from " << gDirectory->GetName()
                   << " to file ";

        for (Int_t iSel = 0; iSel < iNSel; iSel++) {
          // Store DelTof corrections
          TDirectory* curdir = gDirectory;
          gROOT->cd();  //
          TH1D* hCorDelTof = (TH1D*) gDirectory->FindObjectAny(
            Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                 iSmType,
                 iSm,
                 iRpc,
                 iSel));
          gDirectory->cd(curdir->GetPath());
          if (NULL != hCorDelTof) {
            TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                   iSmType,
                   iSm,
                   iRpc,
                   iSel));
            hCorDelTofout->Write();
          } else {
            LOG(debug) << " No CorDelTof histo "
                       << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                               iSmType,
                               iSm,
                               iRpc,
                               iSel);
          }
        }

        LOG(debug) << " Store old walk histos to file ";
        // store walk histos
        for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // restore old values
        {
          if (NULL == fhRpcCluWalk[iDetIndx][iCh][0]) break;
          // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
          // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
          TH1D* h1tmp0 =
            fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX("_px", 1, nbClWalkBinY);
          TH1D* h1tmp1 =
            fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX("_px", 1, nbClWalkBinY);
          for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
            h1tmp0->SetBinContent(
              iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
            h1tmp1->SetBinContent(
              iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
            if (h1tmp0->GetBinContent(iWx + 1)
                != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
              LOG(error) << "WriteHistos: restore unsuccessful! iWx " << iWx
                         << " got " << h1tmp0->GetBinContent(iWx + 1)
                         << ", expected "
                         << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
            }
          }
          h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",
                               iSmType,
                               iSm,
                               iRpc,
                               iCh));
          //          htmp0->Write();
          h1tmp0->Write();
          h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",
                               iSmType,
                               iSm,
                               iRpc,
                               iCh));
          //          htmp1->Write();
          h1tmp1->Write();
        }
      } break;

      case 4:  //update DelTof, save offsets, gains and walks
      {
        Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
        Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
        LOG(debug)
          << "WriteHistos: restore Offsets, Gains and Walk, save DelTof for "
          << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc;
        htempPos_pfx->Reset();  //reset to restore mean of original histos
        htempTOff_pfx->Reset();
        htempTot_Mean->Reset();
        htempTot_Off->Reset();
        for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
          Double_t YMean = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          Double_t TMean = 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          htempPos_pfx->Fill(iCh, YMean);
          if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
            LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh
                       << " got " << htempPos_pfx->GetBinContent(iCh) << ","
                       << htempPos_pfx->GetBinContent(iCh + 1) << ","
                       << htempPos_pfx->GetBinContent(iCh + 2) << ", expected "
                       << YMean;
          }
          htempTOff_pfx->Fill(iCh, TMean);

          for (Int_t iSide = 0; iSide < 2; iSide++) {
            htempTot_Mean->SetBinContent(
              iCh * 2 + 1 + iSide,
              fdTTotMean
                / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh]
                             [iSide]);  //nh +1 empirical(?)
            htempTot_Off->SetBinContent(
              iCh * 2 + 1 + iSide,
              fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
          }
        }

        LOG(debug1) << " Restoring of Offsets and Gains done ... ";
        htempPos_pfx->Write();
        htempTOff_pfx->Write();
        //        htempTot_pfx->Write();
        htempTot_Mean->Write();
        htempTot_Off->Write();

        // restore walk histos
        for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // restore old values
        {
          if (NULL == fhRpcCluWalk[iDetIndx][iCh][0]) break;
          // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
          // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
          TH1D* h1tmp0 =
            fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX("_px", 1, nbClWalkBinY);
          TH1D* h1tmp1 =
            fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX("_px", 1, nbClWalkBinY);
          for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
            h1tmp0->SetBinContent(
              iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
            h1tmp1->SetBinContent(
              iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
            if (h1tmp0->GetBinContent(iWx + 1)
                != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
              LOG(error) << "WriteHistos: restore unsuccessful! iWx " << iWx
                         << " got " << h1tmp0->GetBinContent(iWx + 1)
                         << ", expected "
                         << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
            }
          }
          h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",
                               iSmType,
                               iSm,
                               iRpc,
                               iCh));
          //          htmp0->Write();
          h1tmp0->Write();
          h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",
                               iSmType,
                               iSm,
                               iRpc,
                               iCh));
          //          htmp1->Write();
          h1tmp1->Write();
        }

        // generate/update DelTof corrections
        if (
          (fCalSmAddr < 0 && -fCalSmAddr != iSmAddr)
          || (fCalSmAddr
              == iSmAddr))  // select detectors for determination of DelTof correction
        {
          // if( fSel2Id == iSmType ) continue;  // no DelTof correction for Diamonds

          for (Int_t iSel = 0; iSel < iNSel; iSel++) {
            TH2* h2tmp = fhTRpcCluDelTof[iDetIndx][iSel];
            if (NULL == h2tmp) {
              LOG(debug) << Form(
                "WriteHistos:  histo not available for SmT %d, Sm %d, Rpc %d",
                iSmType,
                iSm,
                iRpc);
              break;
            }
            Int_t iNEntries = h2tmp->GetEntries();

            //          h2tmp->Write();
            TProfile* htmp = h2tmp->ProfileX("_pfx", 1, h2tmp->GetNbinsY());
            TH1D* h1tmp    = h2tmp->ProjectionX("_px", 1, h2tmp->GetNbinsY());
            /*          TH1D *h1ytmp   = h2tmp->ProjectionY("_py",1,h2tmp->GetNbinsX());*/
            Double_t dDelMean =
              0.;  //h1ytmp->GetMean();// inspect normalization, interferes with mode 3 for diamonds, nh, 10.01.2015 (?)
            for (Int_t iBx = 0; iBx < nbClDelTofBinX; iBx++) {
              Double_t dNEntries = h1tmp->GetBinContent(iBx + 1);
              if (dNEntries
                  > WalkNHmin)  // modify, request sufficient # of entries
                fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] +=
                  ((TProfile*) htmp)->GetBinContent(iBx + 1);
              dDelMean += fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel];
            }
            dDelMean /= (Double_t) nbClDelTofBinX;

            LOG(debug) << Form(" Update DelTof correction for SmT %d, Sm %d, "
                               "Rpc %d, Sel%d: Entries %d, Mean shift %6.1f",
                               iSmType,
                               iSm,
                               iRpc,
                               iSel,
                               iNEntries,
                               dDelMean);

            for (Int_t iBx = 0; iBx < nbClDelTofBinX; iBx++) {
              //h1tmp->SetBinContent(iBx+1,fvCPDelTof[iSmType][iSm*iNbRpc+iRpc][iBx][iSel]-dDelMean);
              h1tmp->SetBinContent(
                iBx + 1, fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel]);
            }
            h1tmp->SetName(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                                iSmType,
                                iSm,
                                iRpc,
                                iSel));
            h1tmp->Write();
          }
        } else {  // copy existing histograms
          for (Int_t iSel = 0; iSel < iNSel; iSel++) {
            // Store DelTof corrections
            TDirectory* curdir = gDirectory;
            gROOT->cd();  //
            TH1D* hCorDelTof = (TH1D*) gDirectory->FindObjectAny(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                   iSmType,
                   iSm,
                   iRpc,
                   iSel));
            gDirectory->cd(curdir->GetPath());
            if (NULL != hCorDelTof) {
              TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
                Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                     iSmType,
                     iSm,
                     iRpc,
                     iSel));
              LOG(debug) << " Save existing CorDelTof histo "
                         << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                                 iSmType,
                                 iSm,
                                 iRpc,
                                 iSel);
              hCorDelTofout->Write();
            } else {
              LOG(debug) << " No CorDelTof histo "
                         << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                                 iSmType,
                                 iSm,
                                 iRpc,
                                 iSel);
            }
          }
        }
      } break;
      case 5:  //update offsets (from positions only), gains, save walks and DELTOF
      {
        Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
        Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
        if ((fCalSmAddr < 0)
            || (fCalSmAddr
                != iSmAddr)) {  // select detectors for updating offsets
          LOG(info) << "WriteHistos (calMode==3): update Offsets and Gains, "
                       "keep Walk and DelTof for "
                    << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc
                    << " with " << iNbCh << " channels "
                    << " using selector " << fCalSel;

          for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // update Offset and Gain
          {
            Double_t YMean = ((TProfile*) htempPos_pfx)
                               ->GetBinContent(iCh + 1);  //nh +1 empirical(?)
            Double_t TMean = 0.;
            Double_t dTYOff =
              YMean / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);


            if (htempTOff_px->GetBinContent(iCh + 1) > WalkNHmin) {
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] += -dTYOff + TMean;
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] += +dTYOff + TMean;
            }
            LOG(debug3) << Form(
              "Calib: TSRC %d%d%d%d, hits %6.0f, new Off %8.0f,%8.0f ",
              iSmType,
              iSm,
              iRpc,
              iCh,
              htempTOff_px->GetBinContent(iCh + 1),
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]);

            /*
           Double_t TotMean=((TProfile *)htempTot_pfx)->GetBinContent(iCh+1);  //nh +1 empirical(!)
          if(0.001 < TotMean){
            fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][0] *= fdTTotMean / TotMean;
            fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][1] *= fdTTotMean / TotMean;
          }
          */
            for (Int_t iSide = 0; iSide < 2; iSide++) {
              Int_t ib  = iCh * 2 + 1 + iSide;
              TH1* hbin = htempTot->ProjectionY(Form("bin%d", ib), ib, ib);
              if (100 > hbin->GetEntries())
                continue;  //request min number of entries
                           /*            Double_t Ymax=hbin->GetMaximum();*/
              Int_t iBmax  = hbin->GetMaximumBin();
              TAxis* xaxis = hbin->GetXaxis();
              Double_t Xmax =
                xaxis->GetBinCenter(iBmax)
                / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide];
              Double_t XOff = Xmax - fTotPreRange;
              if (
                0) {  //TMath::Abs(XOff - fvCPTotOff[iSmType][iSm*iNbRpc+iRpc][iCh][iSide])>100){
                LOG(warning)
                  << "XOff changed for "
                  << Form("SmT%01d_sm%03d_rpc%03d_Side%d: XOff %f, old %f",
                          iSmType,
                          iSm,
                          iRpc,
                          iSide,
                          XOff,
                          fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
              }
              //            Double_t TotMean=htempTot_Mean->GetBinContent(ib);
              Double_t TotMean = hbin->GetMean();
              if (15 == iSmType) {
                LOG(warning)
                  << "Gain for "
                  << Form("SmT%01d_sm%03d_rpc%03d_Side%d: TotMean %f, prof %f, "
                          "gain %f, modg %f ",
                          iSmType,
                          iSm,
                          iRpc,
                          iSide,
                          TotMean,
                          htempTot_Mean->GetBinContent(ib),
                          fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide],
                          fdTTotMean / TotMean);
              }
              if (0.001 < TotMean) {
                fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] *=
                  fdTTotMean / TotMean;
              }
            }
            if (5 == iSmType
                && fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                     != fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh]
                                [1]) {  // diamond
              LOG(warning)
                << "CbmTofCosmicClusterizer::FillCalHist:"
                << " SmT " << iSmType << " Sm " << iSm << " Rpc " << iRpc
                << " Ch " << iCh << ": YMean " << YMean << ", TMean " << TMean
                << " -> "
                << Form(" %f %f %f %f ",
                        fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                        fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1],
                        fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                        fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
              Double_t dTOff =
                0.5
                * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                   + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
              Double_t dGain =
                0.5
                * (fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                   + fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]    = dTOff;
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]    = dTOff;
              fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0] = dGain;
              fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1] = dGain;
            }  // diamond warning end
          }    // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
        }      // iSmType selection condition

        htempPos_pfx->Reset();  //reset to store new values
        htempTOff_pfx->Reset();
        htempTot_Mean->Reset();
        htempTot_Off->Reset();
        for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // store new values
        {
          Double_t YMean = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          Double_t TMean = 0.5
                           * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]
                              + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
          htempPos_pfx->Fill(iCh, YMean);
          if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
            LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh
                       << " got " << htempPos_pfx->GetBinContent(iCh) << ","
                       << htempPos_pfx->GetBinContent(iCh + 1) << ","
                       << htempPos_pfx->GetBinContent(iCh + 2) << ", expected "
                       << YMean;
          }
          htempTOff_pfx->Fill(iCh, TMean);

          for (Int_t iSide = 0; iSide < 2; iSide++) {
            htempTot_Mean->SetBinContent(
              iCh * 2 + 1 + iSide,
              fdTTotMean
                / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
            htempTot_Off->SetBinContent(
              iCh * 2 + 1 + iSide,
              fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
          }
          //         htempTot_pfx->Fill(iCh,fdTTotMean/fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][1]);
        }  // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )


        LOG(debug1) << " Updating done ... write to file ";
        htempPos_pfx->Write();
        htempTOff_pfx->Write();
        //        htempTot_pfx->Write();
        htempTot_Mean->Write();
        htempTot_Off->Write();

        // store old DELTOF histos
        LOG(debug) << " Copy old DelTof histos from " << gDirectory->GetName()
                   << " to file ";

        for (Int_t iSel = 0; iSel < iNSel; iSel++) {
          // Store DelTof corrections
          TDirectory* curdir = gDirectory;
          gROOT->cd();  //
          TH1D* hCorDelTof = (TH1D*) gDirectory->FindObjectAny(
            Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                 iSmType,
                 iSm,
                 iRpc,
                 iSel));
          gDirectory->cd(curdir->GetPath());
          if (NULL != hCorDelTof) {
            TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                   iSmType,
                   iSm,
                   iRpc,
                   iSel));
            hCorDelTofout->Write();
          } else {
            LOG(debug) << " No CorDelTof histo "
                       << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                               iSmType,
                               iSm,
                               iRpc,
                               iSel);
          }
        }

        LOG(debug) << " Store old walk histos to file ";
        // store walk histos
        for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // restore old values
        {
          if (NULL == fhRpcCluWalk[iDetIndx][iCh][0]) break;
          // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
          // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);   (VF) not used
          TH1D* h1tmp0 =
            fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX("_px", 1, nbClWalkBinY);
          TH1D* h1tmp1 =
            fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX("_px", 1, nbClWalkBinY);
          for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
            h1tmp0->SetBinContent(
              iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
            h1tmp1->SetBinContent(
              iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
            if (h1tmp0->GetBinContent(iWx + 1)
                != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
              LOG(error) << "WriteHistos: restore unsuccessful! iWx " << iWx
                         << " got " << h1tmp0->GetBinContent(iWx + 1)
                         << ", expected "
                         << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
            }
          }
          h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px",
                               iSmType,
                               iSm,
                               iRpc,
                               iCh));
          //          htmp0->Write();
          h1tmp0->Write();
          h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",
                               iSmType,
                               iSm,
                               iRpc,
                               iCh));
          //          htmp1->Write();
          h1tmp1->Write();
        }
      } break;


      default:
        LOG(debug) << "WriteHistos: update mode " << fCalMode
                   << " not yet implemented";
    }
  }

  //   fhCluMulCorDutSel->Write();

  //   fhDigSpacDifClust->Write();
  //   fhDigTimeDifClust->Write();
  //   fhDigDistClust->Write();

  //   fhClustSizeDifX->Write();
  //   fhClustSizeDifY->Write();

  //   fhChDifDifX->Write();
  //   fhChDifDifY->Write();

  //   fhNbSameSide->Write();
  //   fhNbDigiPerChan->Write();


  //   fhHitsPerTracks->Write();
  if (kFALSE == fDigiBdfPar->ClustUseTrackId())
    //      fhPtsPerHit->Write();
    //   fhTimeResSingHits->Write();
    //   fhTimeResSingHitsB->Write();
    //   fhTimePtVsHits->Write();
    //   fhClusterSize->Write();
    //   fhClusterSizeType->Write();
    if (kTRUE == fDigiBdfPar->ClustUseTrackId()) {
      //      fhTrackMul->Write();
      //      fhClusterSizeMulti->Write();
      //      fhTrk1MulPos->Write();
      //      fhHiTrkMulPos->Write();
      //      fhAllTrkMulPos->Write();
      //      fhMultiTrkProbPos->Divide( fhHiTrkMulPos, fhAllTrkMulPos);
      //      fhMultiTrkProbPos->Scale( 100.0 );
      //      fhMultiTrkProbPos->Write();
    }  // if( kTRUE == fDigiBdfPar->ClustUseTrackId() )

  for (Int_t iS = 0; iS < fDigiBdfPar->GetNbSmTypes(); iS++) {
    if (NULL == fhSmCluPosition[iS]) continue;
    //     fhSmCluPosition[iS]->Write();
    //     fhSmCluTOff[iS]->Write();
    fhSmCluSvel[iS]->Write();
    for (Int_t iPar = 0; iPar < 4; iPar++)
      fhSmCluFpar[iS][iPar]->Write();

    for (Int_t iSel = 0; iSel < iNSel; iSel++) {  // Loop over selectors
      //       fhTSmCluPosition[iS][iSel]->Write();
      //       fhTSmCluTOff[iS][iSel]->Write();
      //       fhTSmCluTRun[iS][iSel]->Write();
    }
  }

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  fHist->Close();

  return kTRUE;
}
Bool_t CbmTofCosmicClusterizer::DeleteHistos() {
  delete fhClustBuildTime;
  delete fhHitsPerTracks;
  delete fhPtsPerHit;
  delete fhTimeResSingHits;
  delete fhTimeResSingHitsB;
  delete fhTimePtVsHits;
  delete fhClusterSize;
  delete fhClusterSizeType;

  if (kTRUE == fDigiBdfPar->ClustUseTrackId()) {
    delete fhTrackMul;
    delete fhClusterSizeMulti;
    delete fhTrk1MulPos;
    delete fhHiTrkMulPos;
    delete fhAllTrkMulPos;
    delete fhMultiTrkProbPos;
  }
  delete fhDigSpacDifClust;
  delete fhDigTimeDifClust;
  delete fhDigDistClust;

  delete fhClustSizeDifX;
  delete fhClustSizeDifY;

  delete fhChDifDifX;
  delete fhChDifDifY;

  delete fhNbSameSide;
  delete fhNbDigiPerChan;

  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofCosmicClusterizer::BuildClusters() {
  /*
    * FIXME: maybe use the 2D distance (X/Y) as criteria instead of the distance long channel
    * direction
    */
  Int_t iMess = 0;
  //gGeoManager->SetTopVolume( gGeoManager->FindVolumeFast("tof_v14a") );
  gGeoManager->CdTop();

  if (NULL == fTofDigisColl) {
    LOG(info) << " No CalDigis defined ! Check! ";
    return kFALSE;
  }
  fiNevtBuild++;
  LOG(debug) << "CbmTofCosmicClusterizer::BuildClusters from "
             << fTofDigisColl->GetEntries() << " digis in event "
             << fiNevtBuild;

  fTRefHits = 0.;

  Int_t iNbTofDigi = fTofDigisColl->GetEntries();
  if (kTRUE) {
    for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
      CbmTofDigi* pDigi = (CbmTofDigi*) fTofDigisColl->At(iDigInd);
      LOG(debug) << "#"
                 << iDigInd
                 //      <<Form(" Address : 0x%08x ",pDigi->GetAddress())
                 << " TSR " << pDigi->GetType() << pDigi->GetSm()
                 << pDigi->GetRpc() << " Ch " << pDigi->GetChannel() << " S "
                 << pDigi->GetSide() << " : " << pDigi->ToString()
        //      <<" Time "<<pDigi->GetTime()
        //      <<" Tot " <<pDigi->GetTot()
        ;

      Int_t iDetIndx = fDigiBdfPar->GetDetInd(pDigi->GetAddress());

      if (fDigiBdfPar->GetNbDet() - 1 < iDetIndx || iDetIndx < 0) {
        LOG(debug) << Form(
          " Wrong DetIndx %d >< %d,0 ", iDetIndx, fDigiBdfPar->GetNbDet());
        break;
      }

      if (NULL == fhRpcDigiCor[iDetIndx]) {
        if (100 < iMess++)
          LOG(warning) << Form(
            " DigiCor Histo for  DetIndx %d derived from 0x%08x not found",
            iDetIndx,
            pDigi->GetAddress());
        continue;
      }

      Double_t dTDifMin     = dDoubleMax;
      CbmTofDigi* pDigi2Min = NULL;
      //       for (Int_t iDigI2 =iDigInd+1; iDigI2<iNbTofDigi;iDigI2++){
      for (Int_t iDigI2 = 0; iDigI2 < iNbTofDigi; iDigI2++) {
        CbmTofDigi* pDigi2 = (CbmTofDigi*) fTofDigisColl->At(iDigI2);
        if (iDetIndx == fDigiBdfPar->GetDetInd(pDigi2->GetAddress())) {
          if (0. == pDigi->GetSide() && 1. == pDigi2->GetSide()) {
            fhRpcDigiCor[iDetIndx]->Fill(pDigi->GetChannel(),
                                         pDigi2->GetChannel());
          } else {
            if (1. == pDigi->GetSide() && 0. == pDigi2->GetSide()) {
              fhRpcDigiCor[iDetIndx]->Fill(pDigi2->GetChannel(),
                                           pDigi->GetChannel());
            }
          }
          if (pDigi->GetSide() != pDigi2->GetSide()) {
            if (pDigi->GetChannel() == pDigi2->GetChannel()) {
              Double_t dTDif = TMath::Abs(pDigi->GetTime() - pDigi2->GetTime());
              if (dTDif < dTDifMin) {
                dTDifMin  = dTDif;
                pDigi2Min = pDigi2;
              }
            } else if (
              TMath::Abs(pDigi->GetChannel() - pDigi2->GetChannel())
              == 1) {  // opposite side missing, neighbouring channel has hit on opposite side // FIXME
              // check that same side digi of neighbouring channel is absent
              LOG(debug) << Form("Missing digi cor %d for TSRC %d%d%d%d ?",
                                 fiCorMode,
                                 (Int_t) pDigi->GetType(),
                                 (Int_t) pDigi->GetSm(),
                                 (Int_t) pDigi->GetRpc(),
                                 (Int_t) pDigi->GetChannel());
              Int_t iDigI3 = 0;
              for (; iDigI3 < iNbTofDigi; iDigI3++) {
                CbmTofDigi* pDigi3 = (CbmTofDigi*) fTofDigisColl->At(iDigI3);
                if (iDetIndx == fDigiBdfPar->GetDetInd(pDigi3->GetAddress()))
                  if (pDigi3->GetSide() == pDigi->GetSide()
                      && pDigi2->GetChannel() == pDigi3->GetChannel())
                    break;
              }
              if (iDigI3 == iNbTofDigi)  // same side neighbour did not fire
              {
                //Int_t fiCorMode=2; // Missing hit correction mode
                switch (fiCorMode) {
                  case 0:  // no action
                    break;
                  case 1:  // shift found hit
                    LOG(debug2) << Form("shift channel %d%d%d%d%d and  ",
                                        (Int_t) pDigi->GetType(),
                                        (Int_t) pDigi->GetSm(),
                                        (Int_t) pDigi->GetRpc(),
                                        (Int_t) pDigi->GetChannel(),
                                        (Int_t) pDigi->GetSide())
                                << Form(" %d%d%d%d%d ",
                                        (Int_t) pDigi2->GetType(),
                                        (Int_t) pDigi2->GetSm(),
                                        (Int_t) pDigi2->GetRpc(),
                                        (Int_t) pDigi2->GetChannel(),
                                        (Int_t) pDigi2->GetSide());
                    //if(pDigi->GetTime() < pDigi2->GetTime())
                    if (pDigi->GetSide() == 0)
                      pDigi2->SetAddress(pDigi->GetSm(),
                                         pDigi->GetRpc(),
                                         pDigi->GetChannel(),
                                         1 - pDigi->GetSide(),
                                         pDigi->GetType());
                    else
                      pDigi->SetAddress(pDigi2->GetSm(),
                                        pDigi2->GetRpc(),
                                        pDigi2->GetChannel(),
                                        1 - pDigi2->GetSide(),
                                        pDigi2->GetType());

                    LOG(debug2) << Form("resultchannel %d%d%d%d%d and  ",
                                        (Int_t) pDigi->GetType(),
                                        (Int_t) pDigi->GetSm(),
                                        (Int_t) pDigi->GetRpc(),
                                        (Int_t) pDigi->GetChannel(),
                                        (Int_t) pDigi->GetSide())
                                << Form(" %d%d%d%d%d ",
                                        (Int_t) pDigi2->GetType(),
                                        (Int_t) pDigi2->GetSm(),
                                        (Int_t) pDigi2->GetRpc(),
                                        (Int_t) pDigi2->GetChannel(),
                                        (Int_t) pDigi2->GetSide());
                    break;
                  case 2:  // insert missing hits

                    CbmTofDigi* pDigiN =
                      new ((*fTofDigisColl)[iNbTofDigi++]) CbmTofDigi(*pDigi);
                    pDigiN->SetAddress(pDigi->GetSm(),
                                       pDigi->GetRpc(),
                                       pDigi2->GetChannel(),
                                       pDigi->GetSide(),
                                       pDigi->GetType());
                    pDigiN->SetTot(pDigi2->GetTot());
                    LOG(debug) << Form("InsertDigi  TSRCS %d%d%d%d%d and  ",
                                       (Int_t) pDigiN->GetType(),
                                       (Int_t) pDigiN->GetSm(),
                                       (Int_t) pDigiN->GetRpc(),
                                       (Int_t) pDigiN->GetChannel(),
                                       (Int_t) pDigiN->GetSide());

                    CbmTofDigi* pDigiN2 =
                      new ((*fTofDigisColl)[iNbTofDigi++]) CbmTofDigi(*pDigi2);
                    pDigiN2->SetAddress(pDigi2->GetSm(),
                                        pDigi2->GetRpc(),
                                        pDigi->GetChannel(),
                                        pDigi2->GetSide(),
                                        pDigi2->GetType());
                    pDigiN2->SetTot(pDigi->GetTot());
                    LOG(debug) << Form("InsertDigi2 TSRCS %d%d%d%d%d and  ",
                                       (Int_t) pDigiN2->GetType(),
                                       (Int_t) pDigiN2->GetSm(),
                                       (Int_t) pDigiN2->GetRpc(),
                                       (Int_t) pDigiN2->GetChannel(),
                                       (Int_t) pDigiN2->GetSide());
                    break;
                }
              }
            }
          }
        }
      }

      if (pDigi2Min != NULL) {
        CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof,
                                    pDigi->GetType(),
                                    pDigi->GetSm(),
                                    pDigi->GetRpc(),
                                    0,
                                    pDigi->GetChannel());
        Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
        fChannelInfo = fDigiPar->GetCell(iChId);
        if (NULL == fChannelInfo) {
          LOG(warning) << Form("BuildClusters: invalid ChannelInfo for 0x%08x",
                               iChId);
          continue;
        }
        if (fDigiBdfPar->GetSigVel(
              pDigi->GetType(), pDigi->GetSm(), pDigi->GetRpc())
              * dTDifMin * 0.5
            < fPosYMaxScal * fChannelInfo->GetSizey()) {
          //check consistency
          if (8 == pDigi->GetType() || 5 == pDigi->GetType()) {
            if (pDigi->GetTime() != pDigi2Min->GetTime()) {
              if (fiMsgCnt-- > 0) {
                LOG(warning)
                  << " BuildClusters: Inconsistent duplicated digis in event "
                  << fiNevtBuild << ", Ind: " << iDigInd;
                LOG(warning) << "   " << pDigi->ToString();
                LOG(warning) << "   " << pDigi2Min->ToString();
              }
              pDigi2Min->SetTot(pDigi->GetTot());
              pDigi2Min->SetTime(pDigi->GetTime());
            }
          }

          // average ToTs! temporary fix, FIXME
          /*
         Double_t dAvTot=0.5*(pDigi->GetTot()+pDigi2Min->GetTot());
         pDigi->SetTot(dAvTot);
         pDigi2Min->SetTot(dAvTot);
         LOG(debug)<<" BuildClusters: TDif "<<dTDifMin<<", Average Tot "<<dAvTot;
         LOG(debug)<<"      "<<pDigi->ToString();
         LOG(debug)<<"      "<<pDigi2Min->ToString();
         */
        }
      }
    }
  }

  // First Time order the Digis array
  // fTofDigisColl->Sort();

  // Then loop over the digis array and store the Digis in separate vectors for
  // each RPC modules

  // Calibrate RawDigis
  if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
    CbmTofDigi* pDigi;
    CbmTofDigi* pCalDigi = NULL;
    Int_t iDigIndCal     = -1;
    // channel deadtime map
    std::map<Int_t, Double_t> mChannelDeadTime;

    for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
      pDigi       = (CbmTofDigi*) fTofDigisColl->At(iDigInd);
      Int_t iAddr = pDigi->GetAddress();

      LOG(debug1) << "BC "  // Before Calibration
                  << Form("0x%08x", pDigi->GetAddress()) << " TSRC "
                  << pDigi->GetType() << pDigi->GetSm() << pDigi->GetRpc()
                  << Form("%2d", (Int_t) pDigi->GetChannel()) << " "
                  << pDigi->GetSide() << " " << Form("%f", pDigi->GetTime())
                  << " " << pDigi->GetTot();

      if (pDigi->GetType() == 5
          || pDigi->GetType()
               == 8)  // for Pad counters generate fake digi to mockup a strip
        if (pDigi->GetSide() == 1)
          continue;  // skip one side to avoid double entries

      Bool_t bValid = kTRUE;
      std::map<Int_t, Double_t>::iterator it;
      it = mChannelDeadTime.find(iAddr);
      if (it != mChannelDeadTime.end()) {
        LOG(debug1) << "CCT found valid ChannelDeadtime entry "
                    << mChannelDeadTime[iAddr] << ", DeltaT "
                    << pDigi->GetTime() - mChannelDeadTime[iAddr];
        if ((bValid = (pDigi->GetTime()
                       > mChannelDeadTime[iAddr] + fdChannelDeadtime)))
          pCalDigi = new ((*fTofCalDigisColl)[++iDigIndCal]) CbmTofDigi(*pDigi);
      } else {
        pCalDigi = new ((*fTofCalDigisColl)[++iDigIndCal]) CbmTofDigi(*pDigi);
      }
      mChannelDeadTime[iAddr] = pDigi->GetTime();
      if (!bValid) continue;


      if (fbPs2Ns) {
        pCalDigi->SetTime(pCalDigi->GetTime()
                          / 1000.);  // for backward compatibility
        pCalDigi->SetTot(pCalDigi->GetTot()
                         / 1000.);  // for backward compatibility
      }

      if (fdTimePeriod > 0.) {
        pCalDigi->SetTime(TimeInPeriod(pCalDigi->GetTime()));  // for debugging
      }

      LOG(debug1) << "DC "  // After deadtime check. before Calibration
                  << Form("0x%08x", pDigi->GetAddress()) << " TSRC "
                  << pDigi->GetType() << pDigi->GetSm() << pDigi->GetRpc()
                  << Form("%2d", (Int_t) pDigi->GetChannel()) << " "
                  << pDigi->GetSide() << " " << Form("%f", pDigi->GetTime())
                  << " " << pDigi->GetTot();

      if (fDigiBdfPar->GetNbSmTypes()
            > pDigi->GetType()  // prevent crash due to misconfiguration
          && fDigiBdfPar->GetNbSm(pDigi->GetType()) > pDigi->GetSm()
          && fDigiBdfPar->GetNbRpc(pDigi->GetType()) > pDigi->GetRpc()
          && fDigiBdfPar->GetNbChan(pDigi->GetType(), pDigi->GetRpc())
               > pDigi->GetChannel()) {

        LOG(debug2) << " CluCal-Init: " << pDigi->ToString();
        // apply calibration vectors
        pCalDigi->SetTime(
          pCalDigi->GetTime() -  // calibrate Digi Time
          fvCPTOff[pDigi->GetType()]
                  [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                   + pDigi->GetRpc()][pDigi->GetChannel()][pDigi->GetSide()]);
        LOG(debug2) << " CluCal-TOff: " << pCalDigi->ToString();

        Double_t dTot =
          pCalDigi->GetTot() -  // subtract Offset
          fvCPTotOff[pDigi->GetType()]
                    [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                     + pDigi->GetRpc()][pDigi->GetChannel()][pDigi->GetSide()];
        if (dTot < 0.001) dTot = 0.001;
        pCalDigi->SetTot(
          dTot *  // calibrate Digi ToT
          fvCPTotGain[pDigi->GetType()]
                     [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                      + pDigi->GetRpc()][pDigi->GetChannel()]
                     [pDigi->GetSide()]);

        // walk correction
        Double_t dTotBinSize = (fdTOTMax - fdTOTMin) / nbClWalkBinX;
        Int_t iWx = (Int_t)((pCalDigi->GetTot() - fdTOTMin) / dTotBinSize);
        if (0 > iWx) iWx = 0;
        if (iWx >= nbClWalkBinX) iWx = nbClWalkBinX - 1;
        Double_t dDTot =
          (pCalDigi->GetTot() - fdTOTMin) / dTotBinSize - (Double_t) iWx - 0.5;
        Double_t dWT = fvCPWalk[pCalDigi->GetType()]
                               [pCalDigi->GetSm()
                                  * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                               [pCalDigi->GetSide()][iWx];
        if (dDTot > 0) {                 // linear interpolation to next bin
          if (iWx < nbClWalkBinX - 1) {  // linear interpolation to next bin

            dWT += dDTot
                   * (fvCPWalk[pCalDigi->GetType()]
                              [pCalDigi->GetSm()
                                 * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                               + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                              [pCalDigi->GetSide()][iWx + 1]
                      - fvCPWalk[pCalDigi->GetType()]
                                [pCalDigi->GetSm()
                                   * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                 + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                                [pCalDigi->GetSide()][iWx]);  //memory leak???
          }
        } else  // dDTot < 0,  linear interpolation to next bin
        {
          if (0 < iWx) {  // linear interpolation to next bin
            dWT -= dDTot
                   * (fvCPWalk[pCalDigi->GetType()]
                              [pCalDigi->GetSm()
                                 * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                               + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                              [pCalDigi->GetSide()][iWx - 1]
                      - fvCPWalk[pCalDigi->GetType()]
                                [pCalDigi->GetSm()
                                   * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                 + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                                [pCalDigi->GetSide()][iWx]);  //memory leak???
          }
        }

        pCalDigi->SetTime(pCalDigi->GetTime() - dWT);  // calibrate Digi Time
        LOG(debug2) << " CluCal-Walk: " << pCalDigi->ToString();

        if (0) {  //pDigi->GetType()==7 && pDigi->GetSm()==0){
          LOG(info)
            << "CbmTofCosmicClusterizer::BuildClusters: CalDigi " << iDigIndCal
            << ",  T " << pCalDigi->GetType() << ", Sm " << pCalDigi->GetSm()
            << ", R " << pCalDigi->GetRpc() << ", Ch " << pCalDigi->GetChannel()
            << ", S " << pCalDigi->GetSide() << ", T " << pCalDigi->GetTime()
            << ", Tot " << pCalDigi->GetTot() << ", TotGain "
            << fvCPTotGain[pCalDigi->GetType()]
                          [pCalDigi->GetSm()
                             * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                           + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                          [pCalDigi->GetSide()]
            << ", TotOff "
            << fvCPTotOff[pCalDigi->GetType()]
                         [pCalDigi->GetSm()
                            * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                          + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                         [pCalDigi->GetSide()]
            << ", Walk " << iWx << ": "
            << fvCPWalk[pCalDigi->GetType()]
                       [pCalDigi->GetSm()
                          * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                        + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                       [pCalDigi->GetSide()][iWx];

          LOG(info) << " dDTot " << dDTot << " BinSize: " << dTotBinSize
                    << ", CPWalk "
                    << fvCPWalk[pCalDigi->GetType()]
                               [pCalDigi->GetSm()
                                  * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                               [pCalDigi->GetSide()][iWx - 1]
                    << ", "
                    << fvCPWalk[pCalDigi->GetType()]
                               [pCalDigi->GetSm()
                                  * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                               [pCalDigi->GetSide()][iWx]
                    << ", "
                    << fvCPWalk[pCalDigi->GetType()]
                               [pCalDigi->GetSm()
                                  * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                + pCalDigi->GetRpc()][pCalDigi->GetChannel()]
                               [pCalDigi->GetSide()][iWx + 1]
                    << " -> dWT = " << dWT;
        }
      } else {
        LOG(info) << "Skip1 Digi "
                  << " Type " << pDigi->GetType() << " "
                  << fDigiBdfPar->GetNbSmTypes() << " Sm " << pDigi->GetSm()
                  << " " << fDigiBdfPar->GetNbSm(pDigi->GetType()) << " Rpc "
                  << pDigi->GetRpc() << " "
                  << fDigiBdfPar->GetNbRpc(pDigi->GetType()) << " Ch "
                  << pDigi->GetChannel() << " "
                  << fDigiBdfPar->GetNbChan(pDigi->GetType(), 0);
      }
      if (pCalDigi->GetType() == 5
          || pCalDigi->GetType()
               == 8) {  // for Pad counters generate fake digi to mockup a strip
        CbmTofDigi* pCalDigi2 =
          new ((*fTofCalDigisColl)[++iDigIndCal]) CbmTofDigi(*pCalDigi);
        if (pCalDigi->GetSide() == 0)
          pCalDigi2->SetAddress(pCalDigi->GetSm(),
                                pCalDigi->GetRpc(),
                                pCalDigi->GetChannel(),
                                1,
                                pCalDigi->GetType());
        else
          pCalDigi2->SetAddress(pCalDigi->GetSm(),
                                pCalDigi->GetRpc(),
                                pCalDigi->GetChannel(),
                                0,
                                pCalDigi->GetType());
        ;
      }
    }  // for( Int_t iDigInd = 0; iDigInd < nTofDigi; iDigInd++ )

    iNbTofDigi = fTofCalDigisColl
                   ->GetEntries();  // update because of added duplicted digis
    if (fTofCalDigisColl->IsSortable())
      LOG(debug) << "CbmTofCosmicClusterizer::BuildClusters: Sort "
                 << fTofCalDigisColl->GetEntries() << " calibrated digis ";
    if (iNbTofDigi > 1) {
      fTofCalDigisColl->Sort(
        iNbTofDigi);  // Time order again, in case modified by the calibration
      if (!fTofCalDigisColl->IsSorted()) {
        LOG(warning)
          << "CbmTofCosmicClusterizer::BuildClusters: Sorting not successful ";
      }
    }

    // Store CalDigis in vectors
    for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
      pDigi = (CbmTofDigi*) fTofCalDigisColl->At(iDigInd);
      LOG(debug1) << "AC "  // After Calibration
                  << Form("0x%08x", pDigi->GetAddress()) << " TSRC "
                  << pDigi->GetType() << pDigi->GetSm() << pDigi->GetRpc()
                  << Form("%2d", (Int_t) pDigi->GetChannel()) << " "
                  << pDigi->GetSide() << " " << Form("%f", pDigi->GetTime())
                  << " " << pDigi->GetTot();

      if (fDigiBdfPar->GetNbSmTypes()
            > pDigi->GetType()  // prevent crash due to misconfiguration
          && fDigiBdfPar->GetNbSm(pDigi->GetType()) > pDigi->GetSm()
          && fDigiBdfPar->GetNbRpc(pDigi->GetType()) > pDigi->GetRpc()
          && fDigiBdfPar->GetNbChan(pDigi->GetType(), pDigi->GetRpc())
               > pDigi->GetChannel()) {
        fStorDigiExp[pDigi->GetType()]
                    [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                     + pDigi->GetRpc()][pDigi->GetChannel()]
                      .push_back(pDigi);
        fStorDigiInd[pDigi->GetType()]
                    [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                     + pDigi->GetRpc()][pDigi->GetChannel()]
                      .push_back(iDigInd);
      } else {
        LOG(info) << "Skip2 Digi "
                  << " Type " << pDigi->GetType() << " "
                  << fDigiBdfPar->GetNbSmTypes() << " Sm " << pDigi->GetSm()
                  << " " << fDigiBdfPar->GetNbSm(pDigi->GetType()) << " Rpc "
                  << pDigi->GetRpc() << " "
                  << fDigiBdfPar->GetNbRpc(pDigi->GetType()) << " Ch "
                  << pDigi->GetChannel() << " "
                  << fDigiBdfPar->GetNbChan(pDigi->GetType(), 0);
      }
    }  // for( Int_t iDigInd = 0; iDigInd < nTofDigi; iDigInd++ )

  }  // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
  else {
    return kFALSE;  // not implemented properly yet
                    /*
         CbmTofDigi *pDigi;
         for( Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++ )
         {
            pDigi = (CbmTofDigi*) fTofDigisColl->At( iDigInd );
            fStorDigi[pDigi->GetType()]
                     [pDigi->GetSm()*fDigiBdfPar->GetNbRpc( pDigi->GetType()) + pDigi->GetRpc()]
                     [pDigi->GetChannel()].push_back(pDigi);
         fStorDigiInd[pDigi->GetType()]
                     [pDigi->GetSm()*fDigiBdfPar->GetNbRpc( pDigi->GetType()) + pDigi->GetRpc()]
                     [pDigi->GetChannel()].push_back(iDigInd);
         } // for( Int_t iDigInd = 0; iDigInd < nTofDigi; iDigInd++ )
	*/
  }                 // else of if( kTRUE == fDigiBdfPar->UseExpandedDigi() )


  // Then build clusters inside each RPC module
  // Assume only 0 or 1 Digi per channel/side in each event
  // Use simplest method possible, scan direction independent:
  // a) Loop over channels in the RPC starting from 0
  //   * If strips
  //     i) Loop over Digis to check if both ends of the channel have a Digi
  //    ii) Reconstruct a mean channel time and a mean position
  //     + If a Hit is currently filled & the mean position (space, time) is less than XXX from last channel position
  //   iii) Add the mean channel time and the mean position to the ones of the hit
  //     + else
  //   iii) Use nb of strips in cluster to cal. the hit mean time and pos (charge/tot weighting)
  //    iv) Save the hit
  //     v) Start a new hit with current channel
  //   * else (pads)
  //     i) Loop over Digis to find if this channel fired
  //    ii) FIXME: either scan all other channels to check for matching Digis or have more than 1 hit open
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();
  // Hit variables
  Double_t dWeightedTime = 0.0;
  Double_t dWeightedPosX = 0.0;
  Double_t dWeightedPosY = 0.0;
  Double_t dWeightedPosZ = 0.0;
  Double_t dWeightsSum   = 0.0;
  vDigiIndRef.clear();
  // CbmTofCell *fTrafoCell=NULL;   (VF) not used
  // Int_t iTrafoCell=-1;   (VF) not used
  Int_t iNbChanInHit = 0;
  // Last Channel Temp variables
  Int_t iLastChan = -1;
  Double_t dLastPosX =
    0.0;  // -> Comment to remove warning because set but never used
  Double_t dLastPosY = 0.0;
  Double_t dLastTime = 0.0;
  // Channel Temp variables
  Double_t dPosX    = 0.0;
  Double_t dPosY    = 0.0;
  Double_t dPosZ    = 0.0;
  Double_t dTime    = 0.0;
  Double_t dTimeDif = 0.0;
  Double_t dTotS    = 0.0;
  fiNbSameSide      = 0;
  if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
    for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
      Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
      Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
      for (Int_t iSm = 0; iSm < iNbSm; iSm++)
        for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
          Int_t iNbCh   = fDigiBdfPar->GetNbChan(iSmType, iRpc);
          Int_t iChType = fDigiBdfPar->GetChanType(iSmType, iRpc);
          LOG(debug2) << "RPC - Loop  "
                      << Form(" %3d %3d %3d %3d ", iSmType, iSm, iRpc, iChType);
          fviClusterMul[iSmType][iSm][iRpc] = 0;
          Int_t iChId                       = 0;
          if (0 == iChType) {
            // Don't spread clusters over RPCs!!!
            dWeightedTime = 0.0;
            dWeightedPosX = 0.0;
            dWeightedPosY = 0.0;
            dWeightedPosZ = 0.0;
            dWeightsSum   = 0.0;
            iNbChanInHit  = 0;
            // For safety reinitialize everything
            iLastChan = -1;
            //                  dLastPosX = 0.0; // -> Comment to remove warning because set but never used
            dLastPosY = 0.0;
            dLastTime = 0.0;
            LOG(debug2) << "ChanOrient "
                        << Form(" %3d %3d %3d %3d %3d ",
                                iSmType,
                                iSm,
                                iRpc,
                                fDigiBdfPar->GetChanOrient(iSmType, iRpc),
                                iNbCh);

            if (1 == fDigiBdfPar->GetChanOrient(iSmType, iRpc)) {
              // Horizontal strips => X comes from left right time difference
            }  // if( 1 == fDigiBdfPar->GetChanOrient( iSmType, iRpc ) )
            else {
              // Vertical strips => Y comes from bottom top time difference
              for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
                LOG(debug3)
                  << "VDigisize "
                  << Form(
                       " T %3d Sm %3d R %3d Ch %3d Size %3lu ",
                       iSmType,
                       iSm,
                       iRpc,
                       iCh,
                       fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size());
                if (0 == fStorDigiExp[iSmType][iSm * iNbRpc + iRpc].size())
                  continue;
                if (0 < fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size())
                  fhNbDigiPerChan->Fill(
                    fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size());

                while (
                  1 < fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {

                  while ((fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][0])
                           ->GetSide()
                         == (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][1])
                              ->GetSide()) {
                    // Not one Digi of each end!
                    fiNbSameSide++;
                    if (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size()
                        > 2) {
                      LOG(debug)
                        << "SameSide Digis! on TSRC " << iSmType << iSm << iRpc
                        << iCh << ", Times: "
                        << Form("%f",
                                (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                                             [0])
                                  ->GetTime())
                        << ", "
                        << Form("%f",
                                (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                                             [1])
                                  ->GetTime())
                        << ", DeltaT "
                        << (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][1])
                               ->GetTime()
                             - (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                                            [0])
                                 ->GetTime()
                        << ", array size: "
                        << fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                             .size();
                      if (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][2]
                            ->GetSide()
                          == fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                               ->GetSide()) {
                        LOG(debug)
                          << "3 consecutive SameSide Digis! on TSRC " << iSmType
                          << iSm << iRpc << iCh << ", Times: "
                          << (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                                          [0])
                               ->GetTime()
                          << ", "
                          << (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                                          [1])
                               ->GetTime()
                          << ", DeltaT "
                          << (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                                          [1])
                                 ->GetTime()
                               - (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc]
                                              [iCh][0])
                                   ->GetTime()
                          << ", array size: "
                          << fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                               .size();
                        fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                            .begin());
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                            .begin());
                      } else {
                        if (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][2]
                                ->GetTime()
                              - fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                                            [0]
                                              ->GetTime()
                            > fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][2]
                                  ->GetTime()
                                - fStorDigiExp[iSmType][iSm * iNbRpc + iRpc]
                                              [iCh][1]
                                                ->GetTime()) {
                          fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                              .begin());
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                              .begin());
                        } else {
                          LOG(debug) << Form(
                            "Ev %8.0f, digis not properly time ordered, TSRCS "
                            "%d%d%d%d%d ",
                            fdEvent,
                            iSmType,
                            iSm,
                            iRpc,
                            iCh,
                            (Int_t)
                              fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                                ->GetSide());
                          fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                              .begin()
                            + 1);
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                              .begin()
                            + 1);
                        }
                      }
                    } else {
                      LOG(debug2)
                        << "SameSide Erase fStor entries(d) " << iSmType
                        << ", SR " << iSm * iNbRpc + iRpc << ", Ch" << iCh;
                      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());
                    }
                    if (2 > fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                              .size())
                      break;
                    continue;
                  }  // same condition side end

                  LOG(debug2)
                    << "digis processing for "
                    << Form(" SmT %3d Sm %3d Rpc %3d Ch %3d # %3lu ",
                            iSmType,
                            iSm,
                            iRpc,
                            iCh,
                            fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                              .size());
                  if (2 > fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                            .size()) {
                    LOG(debug) << Form(
                      "Leaving digi processing for TSRC %d%d%d%d, size  %3lu",
                      iSmType,
                      iSm,
                      iRpc,
                      iCh,
                      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size());
                    break;
                  }
                  /* Int_t iLastChId = iChId; // Save Last hit channel*/

                  // 2 Digis = both sides present
                  CbmTofDetectorInfo xDetInfo(
                    ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
                  iChId = fTofId->SetDetectorInfo(xDetInfo);
                  Int_t iUCellId =
                    CbmTofAddress::GetUniqueAddress(iSm, iRpc, iCh, 0, iSmType);
                  LOG(debug1) << Form(
                    " TSRC %d%d%d%d size %3lu ",
                    iSmType,
                    iSm,
                    iRpc,
                    iCh,
                    fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size())
                              << Form(" ChId: 0x%08x 0x%08x ", iChId, iUCellId);
                  fChannelInfo = fDigiPar->GetCell(iChId);

                  if (NULL == fChannelInfo) {
                    LOG(error) << "CbmTofCosmicClusterizer::BuildClusters: no "
                                  "geometry info! "
                               << Form(" %3d %3d %3d %3d 0x%08x 0x%08x ",
                                       iSmType,
                                       iSm,
                                       iRpc,
                                       iCh,
                                       iChId,
                                       iUCellId);
                    break;
                  }

                  TGeoNode* fNode =  // prepare local->global trafo
                    gGeoManager->FindNode(fChannelInfo->GetX(),
                                          fChannelInfo->GetY(),
                                          fChannelInfo->GetZ());
                  LOG(debug2) << Form(" Node at (%6.1f,%6.1f,%6.1f) : %p",
                                      fChannelInfo->GetX(),
                                      fChannelInfo->GetY(),
                                      fChannelInfo->GetZ(),
                                      fNode);
                  //          fNode->Print();

                  CbmTofDigi* xDigiA =
                    fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                  CbmTofDigi* xDigiB =
                    fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][1];

                  LOG(debug2) << "    " << xDigiA->ToString();
                  LOG(debug2) << "    " << xDigiB->ToString();

                  dTimeDif = (xDigiA->GetTime() - xDigiB->GetTime());
                  if (5 == iSmType && dTimeDif != 0.) {
                    // FIXME -> Overflow treatment in calib/tdc/TMbsCalibTdcTof.cxx
                    LOG(debug) << "CbmTofCosmicClusterizer::BuildClusters: "
                                  "Diamond hit in "
                               << iSm << " with inconsistent digits "
                               << xDigiA->GetTime() << ", " << xDigiB->GetTime()
                               << " -> " << dTimeDif;
                    LOG(debug) << "    " << xDigiA->ToString();
                    LOG(debug) << "    " << xDigiB->ToString();
                  }
                  if (1 == xDigiA->GetSide())
                    // 0 is the top side, 1 is the bottom side
                    dPosY = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)
                            * dTimeDif * 0.5;
                  else
                    // 0 is the bottom side, 1 is the top side
                    dPosY = -fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)
                            * dTimeDif * 0.5;

                  if (TMath::Abs(dPosY) > fChannelInfo->GetSizey()
                      && fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size()
                           > 2) {
                    LOG(debug)
                      << "Hit candidate outside correlation window, check for "
                         "better possible digis, "
                      << " mul "
                      << fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size();

                    CbmTofDigi* xDigiC =
                      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][2];
                    Double_t dPosYN    = 0.;
                    Double_t dTimeDifN = 0;
                    if (xDigiC->GetSide() == xDigiA->GetSide())
                      dTimeDifN = xDigiC->GetTime() - xDigiB->GetTime();
                    else
                      dTimeDifN = xDigiA->GetTime() - xDigiC->GetTime();

                    if (1 == xDigiA->GetSide())
                      dPosYN = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)
                               * dTimeDifN * 0.5;
                    else
                      dPosYN = -fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)
                               * dTimeDifN * 0.5;

                    if (TMath::Abs(dPosYN) < TMath::Abs(dPosY)) {
                      LOG(debug)
                        << "Replace digi on side " << xDigiC->GetSide()
                        << ", yPosNext " << dPosYN << " old: " << dPosY;
                      dTimeDif = dTimeDifN;
                      dPosY    = dPosYN;
                      if (xDigiC->GetSide() == xDigiA->GetSide()) {
                        xDigiA = xDigiC;
                        fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                            .begin());
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                            .begin());
                      } else {
                        xDigiB = xDigiC;
                        fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          ++(fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                               .begin()
                             + 1));
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          ++(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                               .begin()
                             + 1));
                      }
                    }
                  }

                  if (xDigiA->GetSide() == xDigiB->GetSide()) {
                    LOG(fatal)
                      << "Wrong combinations of digis "
                      << fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                      << ","
                      << fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1];
                  }
                  // The "Strip" time is the mean time between each end
                  dTime = 0.5 * (xDigiA->GetTime() + xDigiB->GetTime());

                  // Weight is the total charge => sum of both ends ToT
                  dTotS = xDigiA->GetTot() + xDigiB->GetTot();


                  // use local coordinates, (0,0,0) is in the center of counter  ?
                  dPosX = ((Double_t)(-iNbCh / 2 + iCh) + 0.5)
                          * fChannelInfo->GetSizex();
                  dPosZ = 0.;

                  LOG(debug1)
                    << "NbChanInHit  "
                    << Form(" %3d %3d %3d %3d %3d 0x%p %1.0f Time %f PosX %f "
                            "PosY %f Svel %f ",
                            iNbChanInHit,
                            iSmType,
                            iRpc,
                            iCh,
                            iLastChan,
                            xDigiA,
                            xDigiA->GetSide(),
                            dTime,
                            dPosX,
                            dPosY,
                            fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc))
                    //  << Form( ", Offs %f, %f ",fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][0],
                    //                            fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][1])
                    ;

                  // Now check if a hit/cluster is already started
                  if (0 < iNbChanInHit) {
                    if (iLastChan == iCh - 1) {
                      fhDigTimeDifClust->Fill(dTime - dLastTime);
                      fhDigSpacDifClust->Fill(dPosY - dLastPosY);
                      fhDigDistClust->Fill(dPosY - dLastPosY,
                                           dTime - dLastTime);
                    }
                    // if( iLastChan == iCh - 1 )
                    // a cluster is already started => check distance in space/time
                    // For simplicity, just check along strip direction for now
                    // and break cluster when a not fired strip is found
                    if (TMath::Abs(dTime - dLastTime) < fdMaxTimeDist
                        && iLastChan == iCh - 1
                        && TMath::Abs(dPosY - dLastPosY) < fdMaxSpaceDist) {
                      // Add to cluster/hit
                      dWeightedTime += dTime * dTotS;
                      dWeightedPosX += dPosX * dTotS;
                      dWeightedPosY += dPosY * dTotS;
                      dWeightedPosZ += dPosZ * dTotS;
                      dWeightsSum += dTotS;
                      iNbChanInHit += 1;

                      vDigiIndRef.push_back((Int_t)(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]));
                      vDigiIndRef.push_back((Int_t)(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]));

                      LOG(debug1)
                        << " Add Digi and erase fStor entries(a): NbChanInHit "
                        << iNbChanInHit << ", " << iSmType << ", SR "
                        << iSm * iNbRpc + iRpc << ", Ch" << iCh;

                      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());
                      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());

                    }  // if current Digis compatible with last fired chan
                    else {
                      // Save Hit
                      dWeightedTime /= dWeightsSum;
                      dWeightedPosX /= dWeightsSum;
                      dWeightedPosY /= dWeightsSum;
                      dWeightedPosZ /= dWeightsSum;
                      //  TVector3 hitPosLocal(dWeightedPosX, dWeightedPosY, dWeightedPosZ);
                      //TVector3 hitPos;
                      Double_t hitpos_local[3];
                      hitpos_local[0] = dWeightedPosX;
                      hitpos_local[1] = dWeightedPosY;
                      hitpos_local[2] = dWeightedPosZ;

                      Double_t hitpos[3];
                      TGeoNode* cNode = gGeoManager->GetCurrentNode();
                      /*TGeoHMatrix* cMatrix =*/gGeoManager->GetCurrentMatrix();
                      //cNode->Print();
                      //cMatrix->Print();

                      gGeoManager->LocalToMaster(hitpos_local, hitpos);
                      LOG(debug1)
                        << Form(" LocalToMaster for node %p: "
                                "(%6.1f,%6.1f,%6.1f) ->(%6.1f,%6.1f,%6.1f)",
                                cNode,
                                hitpos_local[0],
                                hitpos_local[1],
                                hitpos_local[2],
                                hitpos[0],
                                hitpos[1],
                                hitpos[2]);

                      TVector3 hitPos(hitpos[0], hitpos[1], hitpos[2]);

                      // Simple errors, not properly done at all for now
                      // Right way of doing it should take into account the weight distribution
                      // and real system time resolution
                      TVector3 hitPosErr(
                        0.5, 0.5, 0.5);  // including positioning uncertainty
                                         /*
                                    TVector3 hitPosErr( fChannelInfo->GetSizex()/TMath::Sqrt(12.0),   // Single strips approximation
                                       0.5, // Use generic value
                                       1.);

                                    */                                       // fDigiBdfPar->GetFeeTimeRes() * fDigiBdfPar->GetSigVel(iSmType,iRpc), // Use the electronics resolution
                      //fDigiBdfPar->GetNbGaps( iSmType, iRpc)*
                      //fDigiBdfPar->GetGapSize( iSmType, iRpc)/ //10.0 / // Change gap size in cm
                      //TMath::Sqrt(12.0) ); // Use full RPC thickness as "Channel" Z size

                      // calc mean ch from dPosX=((Double_t)(-iNbCh/2 + iCh)+0.5)*fChannelInfo->GetSizex();

                      Int_t iChm =
                        floor(dWeightedPosX / fChannelInfo->GetSizex())
                        + iNbCh / 2;
                      if (iChm < 0) iChm = 0;
                      if (iChm > iNbCh - 1) iChm = iNbCh - 1;
                      Int_t iDetId = CbmTofAddress::GetUniqueAddress(
                        iSm, iRpc, iChm, 0, iSmType);
                      Int_t iRefId = 0;  // Index of the correspondng TofPoint

                      LOG(debug) << "Save Hit  "
                                 << Form(" %3d %3d 0x%08x %3d %3d %3d %f %f",
                                         fiNbHits,
                                         iNbChanInHit,
                                         iDetId,
                                         iChm,
                                         iLastChan,
                                         iRefId,
                                         dWeightedTime,
                                         dWeightedPosY)
                                 << ", DigiSize: " << vDigiIndRef.size()
                                 << ", DigiInds: ";

                      fviClusterMul[iSmType][iSm][iRpc]++;

                      for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
                        LOG(debug) << " " << vDigiIndRef.at(i) << "(M"
                                   << fviClusterMul[iSmType][iSm][iRpc] << ")";
                      }
                      LOG(debug);

                      if (vDigiIndRef.size() < 2) {
                        LOG(warning) << "Digi refs for Hit " << fiNbHits
                                     << ":        vDigiIndRef.size()";
                      }
                      if (fiNbHits > 0) {
                        CbmTofHit* pHitL =
                          (CbmTofHit*) fTofHitsColl->At(fiNbHits - 1);
                        if (iDetId == pHitL->GetAddress()
                            && dWeightedTime == pHitL->GetTime()) {
                          LOG(debug) << "Store Hit twice? "
                                     << " fiNbHits " << fiNbHits << ", "
                                     << Form("0x%08x", iDetId);

                          for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
                            CbmTofDigi* pDigiC =
                              (CbmTofDigi*) fTofCalDigisColl->At(
                                vDigiIndRef.at(i));
                            LOG(debug) << " Digi  " << pDigiC->ToString();
                          }
                          CbmMatch* digiMatchL =
                            (CbmMatch*) fTofDigiMatchColl->At(fiNbHits - 1);
                          for (Int_t i = 0; i < digiMatchL->GetNofLinks();
                               i++) {
                            CbmLink L0     = digiMatchL->GetLink(i);
                            Int_t iDigIndL = L0.GetIndex();
                            CbmTofDigi* pDigiC =
                              (CbmTofDigi*) fTofCalDigisColl->At(iDigIndL);
                            LOG(debug) << " DigiL " << pDigiC->ToString();
                          }
                        }
                      }
                      CbmTofHit* pHit = new CbmTofHit(
                        iDetId,
                        hitPos,
                        hitPosErr,  //local detector coordinates
                        fiNbHits,   // this number is used as reference!!
                        dWeightedTime,
                        vDigiIndRef
                          .size(),  // number of linked digis =  2*CluSize
                        Int_t(dWeightsSum * 10.));  //channel -> Tot
                      //0) ; //channel
                      // output hit
                      new ((*fTofHitsColl)[fiNbHits]) CbmTofHit(*pHit);
                      // memorize hit
                      if (fdMemoryTime > 0.) {
                        LH_store(iSmType, iSm, iRpc, iChm, pHit);
                      } else {
                        pHit->Delete();
                      }
                      /*
				    new((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();
				    CbmMatch* digiMatch = (CbmMatch *)fTofDigiMatchColl->At(fiNbHits);
				    */
                      CbmMatch* digiMatch =
                        new ((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();
                      for (size_t i = 0; i < vDigiIndRef.size(); i++) {
                        Double_t dTot = ((CbmTofDigi*) (fTofCalDigisColl->At(
                                           vDigiIndRef.at(i))))
                                          ->GetTot();
                        digiMatch->AddLink(CbmLink(dTot, vDigiIndRef.at(i)));
                      }

                      fiNbHits++;
                      // For Histogramming
                      fviClusterSize[iSmType][iRpc].push_back(iNbChanInHit);
                      fvdX[iSmType][iRpc].push_back(dWeightedPosX);
                      fvdY[iSmType][iRpc].push_back(dWeightedPosY);

                      vDigiIndRef.clear();

                      // Start a new hit
                      dWeightedTime = dTime * dTotS;
                      dWeightedPosX = dPosX * dTotS;
                      dWeightedPosY = dPosY * dTotS;
                      dWeightedPosZ = dPosZ * dTotS;
                      dWeightsSum   = dTotS;
                      iNbChanInHit  = 1;
                      // Save pointer on CbmTofPoint
                      // Save next digi address
                      LOG(debug2)
                        << " Next fStor Digi " << iSmType << ", SR "
                        << iSm * iNbRpc + iRpc << ", Ch" << iCh << ", Dig0 "
                        << (fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0])
                        << ", Dig1 "
                        << (fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]);

                      vDigiIndRef.push_back((Int_t)(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]));
                      vDigiIndRef.push_back((Int_t)(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]));
                      LOG(debug2)
                        << " Erase fStor entries(b) " << iSmType << ", SR "
                        << iSm * iNbRpc + iRpc << ", Ch" << iCh;
                      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());
                      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());


                    }  // else of if current Digis compatible with last fired chan
                  }    // if( 0 < iNbChanInHit)
                  else {
                    LOG(debug) << Form("1.Hit on channel %d, time: %f, PosY %f",
                                       iCh,
                                       dTime,
                                       dPosY);

                    // first fired strip in this RPC
                    dWeightedTime = dTime * dTotS;
                    dWeightedPosX = dPosX * dTotS;
                    dWeightedPosY = dPosY * dTotS;
                    dWeightedPosZ = dPosZ * dTotS;
                    dWeightsSum   = dTotS;
                    iNbChanInHit  = 1;

                    vDigiIndRef.push_back((Int_t)(
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]));
                    vDigiIndRef.push_back((Int_t)(
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]));

                    LOG(debug2)
                      << " Erase fStor entries(c) " << iSmType << ", SR "
                      << iSm * iNbRpc + iRpc << ", Ch" << iCh;
                    fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());

                  }  // else of if( 0 < iNbChanInHit)
                  iLastChan = iCh;
                  dLastPosX = dPosX;
                  dLastPosY = dPosY;
                  dLastTime = dTime;
                  if (AddNextChan(iSmType,
                                  iSm,
                                  iRpc,
                                  iLastChan,
                                  dLastPosX,
                                  dLastPosY,
                                  dLastTime,
                                  dWeightsSum)) {
                    iNbChanInHit = 0;  // cluster already stored
                  }
                }  // while( 1 < fStorDigiExp[iSmType][iSm*iNbRpc+iRpc][iCh].size() )
                fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].clear();
                fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].clear();
              }  // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
              LOG(debug2) << "finished V-RPC"
                          << Form(" %3d %3d %3d %d %f %fx",
                                  iSmType,
                                  iSm,
                                  iRpc,
                                  fTofHitsColl->GetEntries(),
                                  dLastPosX,
                                  dLastPosY);
            }  // else of if( 1 == fDigiBdfPar->GetChanOrient( iSmType, iRpc ) )
          }    // if( 0 == iChType)
          else {
            LOG(error)
              << "=> Cluster building "
              << "from digis to hits not implemented for pads, Sm type "
              << iSmType << " Rpc " << iRpc;
            return kFALSE;
          }  // else of if( 0 == iChType)

          // Now check if another hit/cluster is started
          // and save it if it's the case
          if (0 < iNbChanInHit) {
            LOG(debug1) << "Process cluster " << iNbChanInHit;

            // Check orientation to properly assign errors
            if (1 == fDigiBdfPar->GetChanOrient(iSmType, iRpc)) {
              LOG(debug1) << "H-Hit ";
            }  // if( 1 == fDigiBdfPar->GetChanOrient( iSmType, iRpc ) )
            else {
              LOG(debug2) << "V-Hit ";
              // Save Hit
              dWeightedTime /= dWeightsSum;
              dWeightedPosX /= dWeightsSum;
              dWeightedPosY /= dWeightsSum;
              dWeightedPosZ /= dWeightsSum;
              //TVector3 hitPos(dWeightedPosX, dWeightedPosY, dWeightedPosZ);

              Double_t hitpos_local[3] = {3 * 0.};
              hitpos_local[0]          = dWeightedPosX;
              hitpos_local[1]          = dWeightedPosY;
              hitpos_local[2]          = dWeightedPosZ;

              Double_t hitpos[3];
              TGeoNode* cNode = gGeoManager->GetCurrentNode();
              /*TGeoHMatrix* cMatrix =*/gGeoManager->GetCurrentMatrix();
              //cNode->Print();
              //cMatrix->Print();

              gGeoManager->LocalToMaster(hitpos_local, hitpos);
              LOG(debug1) << Form(" LocalToMaster for V-node %p: "
                                  "(%6.1f,%6.1f,%6.1f) ->(%6.1f,%6.1f,%6.1f)",
                                  cNode,
                                  hitpos_local[0],
                                  hitpos_local[1],
                                  hitpos_local[2],
                                  hitpos[0],
                                  hitpos[1],
                                  hitpos[2]);

              TVector3 hitPos(hitpos[0], hitpos[1], hitpos[2]);
              // Cosmic errors, not properly done at all for now
              // Right way of doing it should take into account the weight distribution
              // and real system time resolution
              TVector3 hitPosErr(
                0.5, 0.5, 0.5);  // including positioning uncertainty
                                 /*
                     TVector3 hitPosErr( fChannelInfo->GetSizex()/TMath::Sqrt(12.0),   // Single strips approximation
                                       0.5, // Use generic value
                                       1.);
                     */
              //                fDigiBdfPar->GetFeeTimeRes() * fDigiBdfPar->GetSigVel(iSmType,iRpc), // Use the electronics resolution
              //                fDigiBdfPar->GetNbGaps( iSmType, iRpc)*
              //                fDigiBdfPar->GetGapSize( iSmType, iRpc)/10.0 / // Change gap size in cm
              //                TMath::Sqrt(12.0) ); // Use full RPC thickness as "Channel" Z size

              Int_t iChm =
                floor(dWeightedPosX / fChannelInfo->GetSizex()) + iNbCh / 2;
              if (iChm < 0) iChm = 0;
              if (iChm > iNbCh - 1) iChm = iNbCh - 1;
              Int_t iDetId =
                CbmTofAddress::GetUniqueAddress(iSm, iRpc, iChm, 0, iSmType);
              Int_t iRefId = 0;  // Index of the correspondng TofPoint
              LOG(debug) << "Save V-Hit  "
                         << Form(" %3d %3d 0x%08x %3d 0x%08x",  // %3d %3d
                                 fiNbHits,
                                 iNbChanInHit,
                                 iDetId,
                                 iLastChan,
                                 iRefId)
                         //   dWeightedTime,dWeightedPosY)
                         << ", DigiSize: " << vDigiIndRef.size();
              LOG(debug) << ", DigiInds: ";

              fviClusterMul[iSmType][iSm][iRpc]++;

              for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
                LOG(debug) << " " << vDigiIndRef.at(i) << "(M"
                           << fviClusterMul[iSmType][iSm][iRpc] << ")";
              }
              LOG(debug);

              if (vDigiIndRef.size() < 2) {
                LOG(warning) << "Digi refs for Hit " << fiNbHits
                             << ":        vDigiIndRef.size()";
              }
              if (fiNbHits > 0) {
                CbmTofHit* pHitL = (CbmTofHit*) fTofHitsColl->At(fiNbHits - 1);
                if (iDetId == pHitL->GetAddress()
                    && dWeightedTime == pHitL->GetTime())
                  LOG(debug) << "Store Hit twice? "
                             << " fiNbHits " << fiNbHits << ", "
                             << Form("0x%08x", iDetId);
              }

              CbmTofHit* pHit = new CbmTofHit(
                iDetId,
                hitPos,
                hitPosErr,  //local detector coordinates
                fiNbHits,   // this number is used as reference!!
                dWeightedTime,
                vDigiIndRef.size(),  // number of linked digis =  2*CluSize
                Int_t(dWeightsSum * 10.));  //channel -> Tot
              //                0) ; //channel
              //                vDigiIndRef);
              // output hit
              new ((*fTofHitsColl)[fiNbHits]) CbmTofHit(*pHit);
              // memorize hit
              if (fdMemoryTime > 0.) {
                LH_store(iSmType, iSm, iRpc, iChm, pHit);
              } else {
                pHit->Delete();
              }
              /*
		     new((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();
		     CbmMatch* digiMatch = (CbmMatch *)fTofDigiMatchColl->At(fiNbHits);
		     */
              CbmMatch* digiMatch =
                new ((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();

              for (size_t i = 0; i < vDigiIndRef.size(); i++) {
                Double_t dTot =
                  ((CbmTofDigi*) (fTofCalDigisColl->At(vDigiIndRef.at(i))))
                    ->GetTot();
                digiMatch->AddLink(CbmLink(dTot, vDigiIndRef.at(i)));
              }

              fiNbHits++;
              // For Histogramming
              fviClusterSize[iSmType][iRpc].push_back(iNbChanInHit);
              fvdX[iSmType][iRpc].push_back(dWeightedPosX);
              fvdY[iSmType][iRpc].push_back(dWeightedPosY);
              vDigiIndRef.clear();
            }  // else of if( 1 == fDigiBdfPar->GetChanOrient( iSmType, iRpc ) )
          }    // if( 0 < iNbChanInHit)
          LOG(debug2) << " Fini-A "
                      << Form(" %3d %3d %3d M%3d",
                              iSmType,
                              iSm,
                              iRpc,
                              fviClusterMul[iSmType][iSm][iRpc]);
        }  // for each sm/rpc pair
      LOG(debug2) << " Fini-B " << Form(" %3d ", iSmType);
    }  // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
  }    // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
  else {
    LOG(error) << " Compressed Digis not implemented ... ";
  }
  return kTRUE;
}

Bool_t CbmTofCosmicClusterizer::MergeClusters() {
  // Merge clusters from neigbouring Rpc within a (Super)Module
  if (NULL == fTofHitsColl) {
    LOG(info) << " No Hits defined ! Check! ";
    return kFALSE;
  }
  // inspect hits
  for (Int_t iHitInd = 0; iHitInd < fTofHitsColl->GetEntries(); iHitInd++) {
    CbmTofHit* pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
    if (NULL == pHit) continue;

    Int_t iDetId  = (pHit->GetAddress() & DetMask);
    Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
    Int_t iNbRpc  = fDigiBdfPar->GetNbRpc(iSmType);
    if (iSmType != 5 && iSmType != 8) continue;  // only merge diamonds and Pad
    LOG(debug) << "MergeClusters: in SmT " << iSmType << " for " << iNbRpc
               << " Rpcs";

    if (iNbRpc > 1) {  // check for possible mergers
      Int_t iSm    = CbmTofAddress::GetSmId(iDetId);
      Int_t iRpc   = CbmTofAddress::GetRpcId(iDetId);
      Int_t iChId  = pHit->GetAddress();
      fChannelInfo = fDigiPar->GetCell(iChId);
      Int_t iCh    = CbmTofAddress::GetChannelId(iChId);
      LOG(debug) << "MergeClusters: Check for mergers in "
                 << Form(" SmT %d, Sm %d, Rpc %d, Ch %d - hit %d",
                         iSmType,
                         iSm,
                         iRpc,
                         iCh,
                         iHitInd);
      for (Int_t iHitInd2 = iHitInd + 1; iHitInd2 < fTofHitsColl->GetEntries();
           iHitInd2++) {
        CbmTofHit* pHit2 = (CbmTofHit*) fTofHitsColl->At(iHitInd2);
        if (NULL == pHit2) continue;
        Int_t iDetId2  = (pHit2->GetAddress() & DetMask);
        Int_t iSmType2 = CbmTofAddress::GetSmType(iDetId2);
        if (iSmType2 == iSmType) {
          Int_t iSm2 = CbmTofAddress::GetSmId(iDetId2);
          if (iSm2 == iSm || iSmType == 5) {
            Int_t iRpc2 = CbmTofAddress::GetRpcId(iDetId2);
            if (TMath::Abs(iRpc - iRpc2) == 1
                || iSm2 != iSm) {  // Found neighbour
              Int_t iChId2 = pHit2->GetAddress();
              // CbmTofCell  *fChannelInfo2 = fDigiPar->GetCell( iChId2 );   (VF) not used
              Int_t iCh2     = CbmTofAddress::GetChannelId(iChId2);
              Double_t xPos  = pHit->GetX();
              Double_t yPos  = pHit->GetY();
              Double_t tof   = pHit->GetTime();
              Double_t xPos2 = pHit2->GetX();
              Double_t yPos2 = pHit2->GetY();
              Double_t tof2  = pHit2->GetTime();
              LOG(debug) << "MergeClusters: Found hit in neighbour "
                         << Form(" SmT %d, Sm %d, Rpc %d, Ch %d - hit %d",
                                 iSmType2,
                                 iSm2,
                                 iRpc2,
                                 iCh2,
                                 iHitInd2)
                         << Form(" DX %6.1f, DY %6.1f, DT %6.1f",
                                 xPos - xPos2,
                                 yPos - yPos2,
                                 tof - tof2);


              if (TMath::Abs(xPos - xPos2) < fdCaldXdYMax * 2.
                  && TMath::Abs(yPos - yPos2) < fdCaldXdYMax * 2.
                  && TMath::Abs(tof - tof2) < fMaxTimeDist) {

                CbmMatch* digiMatch =
                  (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
                Double_t dTot = 0;
                for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks();
                     iLink += 2) {  // loop over digis
                  CbmLink L0     = digiMatch->GetLink(iLink);
                  Int_t iDigInd0 = L0.GetIndex();
                  Int_t iDigInd1 = (digiMatch->GetLink(iLink + 1)).GetIndex();
                  if (iDigInd0 < fTofCalDigisColl->GetEntries()
                      && iDigInd1 < fTofCalDigisColl->GetEntries()) {
                    CbmTofDigi* pDig0 =
                      (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
                    CbmTofDigi* pDig1 =
                      (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd1));
                    dTot += pDig0->GetTot();
                    dTot += pDig1->GetTot();
                  }
                }

                CbmMatch* digiMatch2 =
                  (CbmMatch*) fTofDigiMatchColl->At(iHitInd2);
                Double_t dTot2 = 0;
                for (Int_t iLink = 0; iLink < digiMatch2->GetNofLinks();
                     iLink += 2) {  // loop over digis
                  CbmLink L0     = digiMatch2->GetLink(iLink);
                  Int_t iDigInd0 = L0.GetIndex();
                  Int_t iDigInd1 = (digiMatch2->GetLink(iLink + 1)).GetIndex();
                  if (iDigInd0 < fTofCalDigisColl->GetEntries()
                      && iDigInd1 < fTofCalDigisColl->GetEntries()) {
                    CbmTofDigi* pDig0 =
                      (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
                    CbmTofDigi* pDig1 =
                      (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd1));
                    dTot2 += pDig0->GetTot();
                    dTot2 += pDig1->GetTot();
                    digiMatch->AddLink(CbmLink(pDig0->GetTot(), iDigInd0));
                    digiMatch->AddLink(CbmLink(pDig1->GetTot(), iDigInd1));
                  }
                }
                LOG(debug) << "MergeClusters: Found merger in neighbour "
                           << Form(" SmT %d, Sm %d, Rpc %d, Ch %d - hit %d(%d)",
                                   iSmType2,
                                   iSm2,
                                   iRpc2,
                                   iCh2,
                                   iHitInd2,
                                   fTofHitsColl->GetEntries())
                           << Form(" DX %6.1f, DY %6.1f, DT %6.1f",
                                   xPos - xPos2,
                                   yPos - yPos2,
                                   tof - tof2)
                           << Form(" Tots %6.1f - %6.1f", dTot, dTot2);
                Double_t dTotSum = dTot + dTot2;
                Double_t dxPosM  = (xPos * dTot + xPos2 * dTot2) / dTotSum;
                Double_t dyPosM  = (yPos * dTot + yPos2 * dTot2) / dTotSum;
                Double_t dtofM   = (tof * dTot + tof2 * dTot2) / dTotSum;
                pHit->SetX(dxPosM);
                pHit->SetY(dyPosM);
                pHit->SetTime(dtofM);

                // remove merged hit at iHitInd2 and update digiMatch

                fTofHitsColl->RemoveAt(iHitInd2);
                fTofDigiMatchColl->RemoveAt(iHitInd2);
                fTofDigiMatchColl->Compress();
                fTofHitsColl->Compress();
                LOG(debug) << "MergeClusters: Compress TClonesArrays to "
                           << fTofHitsColl->GetEntries() << ", "
                           << fTofDigiMatchColl->GetEntries();
                /*
                  for(Int_t i=iHitInd2; i<fTofHitsColl->GetEntries(); i++){ // update RefLinks
                     CbmTofHit *pHiti = (CbmTofHit*) fTofHitsColl->At( i );
                    pHiti->SetRefId(i);
                  }
		  */
                //check merged hit (cluster)
                //pHit->Print();
              }
            }
          }
        }
      }
    }
  }
  return kTRUE;
}

static Double_t f1_xboxe(double* x, double* par) {
  double xx    = x[0];
  double wx    = 1. - par[4] * TMath::Power(xx + par[5], 2);
  double xboxe = par[0] * 0.25
                 * (1. + TMath::Erf((xx + par[1] - par[3]) / par[2]))
                 * (1. + TMath::Erf((-xx + par[1] + par[3]) / par[2]));
  return xboxe * wx;
}

void CbmTofCosmicClusterizer::fit_ybox(const char* hname) {
  TH1* h1;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (NULL != h1) { fit_ybox(h1, 0.); }
}

void CbmTofCosmicClusterizer::fit_ybox(TH1* h1, Double_t ysize) {
  Double_t* fpar = NULL;
  fit_ybox(h1, ysize, fpar);
}

void CbmTofCosmicClusterizer::fit_ybox(TH1* h1,
                                       Double_t ysize,
                                       Double_t* fpar = NULL) {
  TAxis* xaxis  = h1->GetXaxis();
  Double_t Ymin = xaxis->GetXmin();
  Double_t Ymax = xaxis->GetXmax();
  TF1* f1       = new TF1("YBox", f1_xboxe, Ymin, Ymax, 6);
  Double_t yini = (h1->GetMaximum() + h1->GetMinimum()) * 0.5;
  if (ysize == 0.) ysize = Ymax * 0.8;
  f1->SetParameters(yini, ysize * 0.5, 1., 0., 0., 0.);
  //  f1->SetParLimits(1,ysize*0.8,ysize*1.2);
  f1->SetParLimits(2, 0.2, 3.);
  f1->SetParLimits(3, -4., 4.);
  if (fpar != NULL) {
    Double_t fp[4];
    for (Int_t i = 0; i < 4; i++)
      fp[i] = *fpar++;
    for (Int_t i = 0; i < 4; i++)
      f1->SetParameter(2 + i, fp[i]);
    LOG(debug) << "Ini Fpar for " << h1->GetName() << " with "
               << Form(" %6.3f %6.3f %6.3f %6.3f ", fp[0], fp[1], fp[2], fp[3]);
  }

  h1->Fit("YBox", "Q");

  double res[10];
  double err[10];
  res[9] = f1->GetChisquare();

  for (int i = 0; i < 6; i++) {
    res[i] = f1->GetParameter(i);
    err[i] = f1->GetParError(i);
    //cout << " FPar "<< i << ": " << res[i] << ", " << err[i] << endl;
  }
  LOG(debug) << "YBox Fit of " << h1->GetName()
             << " ended with chi2 = " << res[9]
             << Form(", strip length %7.2f +/- %5.2f, position resolution "
                     "%7.2f +/- %5.2f at y_cen = %7.2f +/- %5.2f",
                     2. * res[1],
                     2. * err[1],
                     res[2],
                     err[2],
                     res[3],
                     err[3]);
}

void CbmTofCosmicClusterizer::CheckLHMemory() {
  if (fvLastHits.size() != static_cast<size_t>(fDigiBdfPar->GetNbSmTypes()))
    LOG(fatal) << Form("Inconsistent LH Smtype size %lu, %d ",
                       fvLastHits.size(),
                       fDigiBdfPar->GetNbSmTypes());

  for (Int_t iSmType = 0; iSmType < fDigiBdfPar->GetNbSmTypes(); iSmType++) {
    if (fvLastHits[iSmType].size()
        != static_cast<size_t>(fDigiBdfPar->GetNbSm(iSmType)))
      LOG(fatal) << Form("Inconsistent LH Sm size %lu, %d T %d",
                         fvLastHits[iSmType].size(),
                         fDigiBdfPar->GetNbSm(iSmType),
                         iSmType);
    for (Int_t iSm = 0; iSm < fDigiBdfPar->GetNbSm(iSmType); iSm++) {
      if (fvLastHits[iSmType][iSm].size()
          != static_cast<size_t>(fDigiBdfPar->GetNbRpc(iSmType)))
        LOG(fatal) << Form("Inconsistent LH Rpc size %lu, %d TS %d%d ",
                           fvLastHits[iSmType][iSm].size(),
                           fDigiBdfPar->GetNbRpc(iSmType),
                           iSmType,
                           iSm);
      for (Int_t iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
        if (fvLastHits[iSmType][iSm][iRpc].size()
            != static_cast<size_t>(fDigiBdfPar->GetNbChan(iSmType, iRpc)))
          LOG(fatal) << Form(
            "Inconsistent LH RpcChannel size %lu, %d TSR %d%d%d ",
            fvLastHits[iSmType][iSm][iRpc].size(),
            fDigiBdfPar->GetNbChan(iSmType, iRpc),
            iSmType,
            iSm,
            iRpc);
        for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpc); iCh++)
          if (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 0) {
            CbmTofDetectorInfo xDetInfo(
              ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
            Int_t iAddr = fTofId->SetDetectorInfo(xDetInfo);
            if (fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress()
                != iAddr)
              LOG(fatal) << Form(
                "Inconsistent address for Ev %8.0f in list of size %lu for "
                "TSRC %d%d%d%d: 0x%08x, time  %f",
                fdEvent,
                fvLastHits[iSmType][iSm][iRpc][iCh].size(),
                iSmType,
                iSm,
                iRpc,
                iCh,
                fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress(),
                fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime());
          }
      }
    }
  }
  LOG(debug) << Form("LH check passed for event %8.0f", fdEvent);
}

void CbmTofCosmicClusterizer::CleanLHMemory() {
  if (fvLastHits.size() != static_cast<size_t>(fDigiBdfPar->GetNbSmTypes()))
    LOG(fatal) << Form("Inconsistent LH Smtype size %lu, %d ",
                       fvLastHits.size(),
                       fDigiBdfPar->GetNbSmTypes());
  for (Int_t iSmType = 0; iSmType < fDigiBdfPar->GetNbSmTypes(); iSmType++) {
    if (fvLastHits[iSmType].size()
        != static_cast<size_t>(fDigiBdfPar->GetNbSm(iSmType)))
      LOG(fatal) << Form("Inconsistent LH Sm size %lu, %d T %d",
                         fvLastHits[iSmType].size(),
                         fDigiBdfPar->GetNbSm(iSmType),
                         iSmType);
    for (Int_t iSm = 0; iSm < fDigiBdfPar->GetNbSm(iSmType); iSm++) {
      if (fvLastHits[iSmType][iSm].size()
          != static_cast<size_t>(fDigiBdfPar->GetNbRpc(iSmType)))
        LOG(fatal) << Form("Inconsistent LH Rpc size %lu, %d TS %d%d ",
                           fvLastHits[iSmType][iSm].size(),
                           fDigiBdfPar->GetNbRpc(iSmType),
                           iSmType,
                           iSm);
      for (Int_t iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
        if (fvLastHits[iSmType][iSm][iRpc].size()
            != static_cast<size_t>(fDigiBdfPar->GetNbChan(iSmType, iRpc)))
          LOG(fatal) << Form(
            "Inconsistent LH RpcChannel size %lu, %d TSR %d%d%d ",
            fvLastHits[iSmType][iSm][iRpc].size(),
            fDigiBdfPar->GetNbChan(iSmType, iRpc),
            iSmType,
            iSm,
            iRpc);
        for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpc); iCh++)
          while (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 0) {
            CbmTofDetectorInfo xDetInfo(
              ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
            Int_t iAddr = fTofId->SetDetectorInfo(xDetInfo);
            if (fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress()
                != iAddr)
              LOG(fatal) << Form(
                "Inconsistent address for Ev %8.0f in list of size %lu for "
                "TSRC %d%d%d%d: 0x%08x, time  %f",
                fdEvent,
                fvLastHits[iSmType][iSm][iRpc][iCh].size(),
                iSmType,
                iSm,
                iRpc,
                iCh,
                fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress(),
                fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime());
            fvLastHits[iSmType][iSm][iRpc][iCh].front()->Delete();
            fvLastHits[iSmType][iSm][iRpc][iCh].pop_front();
          }
      }
    }
  }
  LOG(info) << Form("LH cleaning done after %8.0f events", fdEvent);
}

Bool_t CbmTofCosmicClusterizer::AddNextChan(Int_t iSmType,
                                            Int_t iSm,
                                            Int_t iRpc,
                                            Int_t iLastChan,
                                            Double_t dLastPosX,
                                            Double_t dLastPosY,
                                            Double_t dLastTime,
                                            Double_t dLastTotS) {
  // Int_t iNbSm  = fDigiBdfPar->GetNbSm(  iSmType);   (VF) not used
  Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
  Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
  // Int_t iChType = fDigiBdfPar->GetChanType( iSmType, iRpc );   (VF) not used

  Int_t iCh = iLastChan + 1;
  LOG(debug) << Form("Inspect channel TSRC %d%d%d%d at time %f, pos %f, size ",
                     iSmType,
                     iSm,
                     iRpc,
                     iCh,
                     dLastTime,
                     dLastPosY)
             << fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size();
  if (iCh == iNbCh) return kFALSE;
  if (0 == fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size())
    return kFALSE;
  if (0 < fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size())
    fhNbDigiPerChan->Fill(
      fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size());
  if (1 < fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {
    Bool_t AddedHit = kFALSE;
    for (size_t i1 = 0;
         i1 < fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size() - 1;
         i1++) {
      if (AddedHit) break;
      size_t i2 = i1 + 1;
      while (!AddedHit
             && i2 < fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {
        LOG(debug) << "check digi pair " << i1 << "," << i2 << " with size "
                   << fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].size();

        if ((fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][i1])->GetSide()
            == (fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][i2])
                 ->GetSide()) {
          i2++;
          continue;
        }  // endif same side
           // 2 Digis, both sides present
        CbmTofDigi* xDigiA =
          fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][i1];
        CbmTofDigi* xDigiB =
          fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh][i2];
        Double_t dTime = 0.5 * (xDigiA->GetTime() + xDigiB->GetTime());
        if (TMath::Abs(dTime - dLastTime) < fdMaxTimeDist) {
          CbmTofDetectorInfo xDetInfo(
            ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
          Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
          fChannelInfo = fDigiPar->GetCell(iChId);
          gGeoManager->FindNode(
            fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());

          Double_t dTimeDif = xDigiA->GetTime() - xDigiB->GetTime();
          Double_t dPosY    = 0.;
          if (1 == xDigiA->GetSide())
            dPosY = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * dTimeDif * 0.5;
          else
            dPosY =
              -fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * dTimeDif * 0.5;

          if (TMath::Abs(dPosY - dLastPosY)
              < fdMaxSpaceDist) {  // append digi pair to current cluster

            Double_t dNClHits = (Double_t)(vDigiIndRef.size() / 2);
            Double_t dPosX =
              ((Double_t)(-iNbCh / 2 + iCh) + 0.5) * fChannelInfo->GetSizex();
            Double_t dTotS    = xDigiA->GetTot() + xDigiB->GetTot();
            Double_t dNewTotS = (dLastTotS + dTotS);
            dLastPosX = (dLastPosX * dLastTotS + dPosX * dTotS) / dNewTotS;
            dLastPosY = (dLastPosY * dLastTotS + dPosY * dTotS) / dNewTotS;
            dLastTime = (dLastTime * dLastTotS + dTime * dTotS) / dNewTotS;
            dLastTotS = dNewTotS;
            // attach selected digis from pool
            Int_t Ind1 = fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][i1];
            Int_t Ind2 = fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][i2];
            vDigiIndRef.push_back(Ind1);
            vDigiIndRef.push_back(Ind2);
            // remove selected digis from pool
            fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
              fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + i1);
            fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
              fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + i1);

            std::vector<int>::iterator it;
            it = find(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin(),
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].end(),
                      Ind2);
            if (it != fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].end()) {
              auto ipos =
                it - fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin();
              LOG(debug) << "Found i2 " << i2 << " with Ind2 " << Ind2
                         << " at position " << ipos;
              fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                fStorDigiExp[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + ipos);
              fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + ipos);
            } else {
              LOG(fatal) << " Did not find  i2 " << i2 << " with Ind2 " << Ind2;
            }

            //if(iCh == iNbCh-1) break;  //Last strip reached
            if (iCh != (iNbCh - 1)
                && AddNextChan(iSmType,
                               iSm,
                               iRpc,
                               iCh,
                               dLastPosX,
                               dLastPosY,
                               dLastTime,
                               dLastTotS)) {
              LOG(debug) << "Added Strip " << iCh << " to cluster of size "
                         << dNClHits;
              return kTRUE;  // signal hit was already added
            }
            AddedHit = kTRUE;
          }  //TMath::Abs(dPosY - dLastPosY) < fdMaxSpaceDist
        }    //TMath::Abs(dTime-dLastTime)<fdMaxTimeDist)
        i2++;
      }  //  while(i2 < fStorDigiExp[iSmType][iSm*iNbRpc+iRpc][iCh].size()-1 )
    }    // end for i1
  }      // end if size
  Double_t hitpos_local[3] = {3 * 0.};
  hitpos_local[0]          = dLastPosX;
  hitpos_local[1]          = dLastPosY;
  hitpos_local[2]          = 0.;
  Double_t hitpos[3];
  /*TGeoNode*    cNode   = */ gGeoManager->GetCurrentNode();
  /*TGeoHMatrix* cMatrix = */ gGeoManager->GetCurrentMatrix();
  gGeoManager->LocalToMaster(hitpos_local, hitpos);
  TVector3 hitPos(hitpos[0], hitpos[1], hitpos[2]);
  TVector3 hitPosErr(0.5, 0.5, 0.5);  // FIXME including positioning uncertainty
  Int_t iChm = floor(dLastPosX / fChannelInfo->GetSizex()) + iNbCh / 2;
  if (iChm < 0) iChm = 0;
  if (iChm > iNbCh - 1) iChm = iNbCh - 1;
  Int_t iDetId = CbmTofAddress::GetUniqueAddress(iSm, iRpc, iChm, 0, iSmType);

  Int_t iNbChanInHit = vDigiIndRef.size() / 2;
  fviClusterMul[iSmType][iSm][iRpc]++;

  LOG(debug) << "Save A-Hit "
             << Form("%2d %2d 0x%08x %3d t %f, y %f ",
                     fiNbHits,
                     iNbChanInHit,
                     iDetId,
                     iLastChan,
                     dLastTime,
                     dLastPosY)
             << ", DigiSize: " << vDigiIndRef.size();
  LOG(debug) << ", DigiInds: ";
  for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
    LOG(debug) << " " << vDigiIndRef.at(i) << "(M"
               << fviClusterMul[iSmType][iSm][iRpc] << ")";
  }
  LOG(debug);

  CbmTofHit* pHit =
    new CbmTofHit(iDetId,
                  hitPos,
                  hitPosErr,  //local detector coordinates
                  fiNbHits,   // this number is used as reference!!
                  dLastTime,
                  vDigiIndRef.size(),  // number of linked digis =  2*CluSize
                  Int_t(dLastTotS * 10.));  //channel -> Tot
  // output hit
  new ((*fTofHitsColl)[fiNbHits]) CbmTofHit(*pHit);
  if (fdMemoryTime > 0.) {  // memorize hit
    LH_store(iSmType, iSm, iRpc, iChm, pHit);
  } else {
    pHit->Delete();
  }
  CbmMatch* digiMatch = new ((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();
  for (size_t i = 0; i < vDigiIndRef.size(); i++) {
    Double_t dTot =
      ((CbmTofDigi*) (fTofCalDigisColl->At(vDigiIndRef.at(i))))->GetTot();
    digiMatch->AddLink(CbmLink(dTot, vDigiIndRef.at(i)));
  }
  fiNbHits++;
  vDigiIndRef.clear();

  return kTRUE;
}

void CbmTofCosmicClusterizer::LH_store(Int_t iSmType,
                                       Int_t iSm,
                                       Int_t iRpc,
                                       Int_t iChm,
                                       CbmTofHit* pHit) {

  if (fvLastHits[iSmType][iSm][iRpc][iChm].size() == 0)
    fvLastHits[iSmType][iSm][iRpc][iChm].push_back(pHit);
  else {
    Double_t dLastTime = pHit->GetTime();
    if (dLastTime >= fvLastHits[iSmType][iSm][iRpc][iChm].back()->GetTime()) {
      fvLastHits[iSmType][iSm][iRpc][iChm].push_back(pHit);
      LOG(debug) << Form(
        " Store LH from Ev  %8.0f for TSRC %d%d%d%d, size %lu, addr 0x%08x, "
        "time %f, dt %f",
        fdEvent,
        iSmType,
        iSm,
        iRpc,
        iChm,
        fvLastHits[iSmType][iSm][iRpc][iChm].size(),
        pHit->GetAddress(),
        dLastTime,
        dLastTime - fvLastHits[iSmType][iSm][iRpc][iChm].front()->GetTime());
    } else {
      if (dLastTime
          >= fvLastHits[iSmType][iSm][iRpc][iChm]
               .front()
               ->GetTime()) {  // hit has to be inserted in the proper place
        std::list<CbmTofHit*>::iterator it;
        for (it = fvLastHits[iSmType][iSm][iRpc][iChm].begin();
             it != fvLastHits[iSmType][iSm][iRpc][iChm].end();
             ++it)
          if ((*it)->GetTime() > dLastTime) break;
        fvLastHits[iSmType][iSm][iRpc][iChm].insert(--it, pHit);
        Double_t deltaTime = dLastTime - (*it)->GetTime();
        LOG(debug) << Form("Hit inserted into LH from Ev  %8.0f for TSRC "
                           "%d%d%d%d, size %lu, addr 0x%08x, delta time %f  ",
                           fdEvent,
                           iSmType,
                           iSm,
                           iRpc,
                           iChm,
                           fvLastHits[iSmType][iSm][iRpc][iChm].size(),
                           pHit->GetAddress(),
                           deltaTime);
      } else {  // this hit is first
        Double_t deltaTime =
          dLastTime - fvLastHits[iSmType][iSm][iRpc][iChm].front()->GetTime();
        LOG(debug) << Form("first LH from Ev  %8.0f for TSRC %d%d%d%d, size "
                           "%lu, addr 0x%08x, delta time %f ",
                           fdEvent,
                           iSmType,
                           iSm,
                           iRpc,
                           iChm,
                           fvLastHits[iSmType][iSm][iRpc][iChm].size(),
                           pHit->GetAddress(),
                           deltaTime);
        if (deltaTime == 0.) {
          // remove hit, otherwise double entry?
          pHit->Delete();
        } else {
          fvLastHits[iSmType][iSm][iRpc][iChm].push_front(pHit);
        }
      }
    }
  }
}

Double_t CbmTofCosmicClusterizer::TimeInPeriod(Double_t dTime) {
  Double_t dP     = dTime / fdTimePeriod;
  Long_t iP       = TMath::Floor(dP);
  Double_t dPtime = dTime - (Double_t) iP * fdTimePeriod;
  // LOG(debug1)<<Form(" %f, %d -> %f ",dTime,iP,dPtime);
  return dPtime;
}
