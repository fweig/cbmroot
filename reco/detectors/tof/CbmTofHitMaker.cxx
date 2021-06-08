/* Copyright (C) 2018-2021 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: nh, Norbert Herrmann [committer] */

/** @file CbmTofHitMaker.cxx
 ** @author nh
 ** @date01.12.2018
 ** adopted from
 ** @file CbmTofEventClusterizer.cxx
 ** @file CbmTofTestBeamClusterizer.cxx
 ** @file CbmTofSimpClusterizer.cxx
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 23.08.2013
 **/

#include "CbmTofHitMaker.h"

// TOF Classes and includes
#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmMatch.h"
#include "CbmTofAddress.h"          // in cbmdata/tof
#include "CbmTofCell.h"             // in tof/TofData
#include "CbmTofCreateDigiPar.h"    // in tof/TofTools
#include "CbmTofDetectorId_v12b.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v21a.h"  // in cbmdata/tof
#include "CbmTofDigi.h"             // in cbmdata/tof
#include "CbmTofDigiBdfPar.h"       // in tof/TofParam
#include "CbmTofDigiPar.h"          // in tof/TofParam
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofHit.h"              // in cbmdata/tof
#include "CbmTofPoint.h"            // in cbmdata/tof
#include "CbmVertex.h"

#include "TTrbHeader.h"

// CBMroot classes and includes
#include "CbmMCTrack.h"

// FAIR classes and includes
#include "FairEventHeader.h"
#include "FairLogger.h"
#include "FairRootFileSink.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ROOT Classes and includes
#include "TClonesArray.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TF2.h"
#include "TFitResult.h"
#include "TGeoManager.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TLine.h"
#include "TMath.h"
#include "TMinuit.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TVector3.h"

// Constants definitions
#include "CbmTofClusterizersDef.h"

// C++ Classes and includes
// Globals
#include <vector>

static Int_t iNbTs = 0;

static Bool_t bAddBeamCounterSideDigi = kTRUE;
static TRandom3* fRndm                = new TRandom3();

//   std::vector< CbmTofPoint* > vPtsRef;

CbmTofHitMaker* CbmTofHitMaker::fInstance = 0;

/************************************************************************************/
CbmTofHitMaker::CbmTofHitMaker() : CbmTofHitMaker("TofHitMaker", 0, 0)
{
  // if ( !fInstance ) fInstance = this;
}

CbmTofHitMaker::CbmTofHitMaker(const char* name, Int_t verbose, Bool_t writeDataInOut)
  : FairTask(TString(name), verbose)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fDigiPar(NULL)
  , fChannelInfo(NULL)
  , fDigiBdfPar(NULL)
  , fTrbHeader(NULL)
  , fTofPointsColl(NULL)
  , fMcTracksColl(NULL)
  , fDigiMan(nullptr)
  , fEventsColl(nullptr)
  , fbWriteHitsInOut(writeDataInOut)
  , fbWriteDigisInOut(writeDataInOut)
  , fTofHitsColl(NULL)
  , fTofDigiMatchColl(NULL)
  , fTofHitsCollOut(NULL)
  , fTofDigiMatchCollOut(NULL)
  , fiNbHits(0)
  , fVerbose(verbose)
  , fStorDigi()
  , fStorDigiInd()
  , vDigiIndRef()
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
  , fhEvCluMul(NULL)
  , fhRpcDigiCor()
  , fhRpcDigiMul()
  , fhRpcDigiStatus()
  , fhRpcDigiDTLD()
  , fhRpcDigiDTFD()
  , fhRpcDigiDTMul()
  , fhRpcCluMul()
  , fhRpcCluRate()
  , fhRpcCluRate10s()
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
  , fhTRpcCluTofOff()
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
  , fvDeadStrips()
  , fvTimeLastDigi()
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
  , fIdMode(0)
  , fDutId(0)
  , fDutSm(0)
  , fDutRpc(0)
  , fDutAddr(0)
  , fSelId(0)
  , fSelSm(0)
  , fSelRpc(0)
  , fSelAddr(0)
  , fiBeamRefType(0)
  , fiBeamRefSm(0)
  , fiBeamRefDet(0)
  , fiBeamRefAddr(0)
  , fiBeamRefMulMax(1)
  , fiBeamAddRefMul(0)
  , fSel2Id(-1)
  , fSel2Sm(0)
  , fSel2Rpc(0)
  , fSel2Addr(0)
  , fSel2MulMax(1)
  , fDetIdIndexMap()
  , fviDetId()
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
  , fdToDAv(0.033)  // in ns/cm
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
  , fdEvent(0)
  , fbSwapChannelSides(kFALSE)
  , fiOutputTreeEntry(0)
  , fiFileIndex(0)
  , fbAlternativeBranchNames(kFALSE)
{
  if (!fInstance) fInstance = this;
}

CbmTofHitMaker::~CbmTofHitMaker()
{
  if (fGeoHandler) delete fGeoHandler;
  if (fInstance == this) fInstance = 0;
  //   DeleteHistos(); // <-- if needed  ?
}

/************************************************************************************/
// FairTasks inherited functions
InitStatus CbmTofHitMaker::Init()
{
  LOG(info) << "CbmTofHitMaker initializing... expect Digis in ns units! ";

  if (kFALSE == RegisterInputs()) return kFATAL;

  if (kFALSE == RegisterOutputs()) return kFATAL;

  if (kFALSE == InitParameters()) return kFATAL;

  if (kFALSE == LoadGeometry()) return kFATAL;

  if (kFALSE == InitCalibParameter()) return kFATAL;

  if (kFALSE == CreateHistos()) return kFATAL;
  return kSUCCESS;
}

void CbmTofHitMaker::SetParContainers()
{
  LOG(info) << "=> Get the digi parameters for tof";
  //LOG(warning)<<"Return without action";
  //return;
  // Get Base Container
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));

  LOG(info) << "found " << fDigiPar->GetNrOfModules() << " cells ";
  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
}

void CbmTofHitMaker::Exec(Option_t* option)
{

  if (fTofCalDigiVecOut) fTofCalDigiVecOut->clear();
  if (fEventsColl) {
    LOG(info) << "CbmTofHitMaker::Exec => New timeslice " << iNbTs << " with " << fEventsColl->GetEntriesFast()
              << " events, " << fDigiMan->GetNofDigis(ECbmModuleId::kTof) << " TOF digis + "
              << fDigiMan->GetNofDigis(ECbmModuleId::kT0) << " T0 digis ";
    iNbTs++;

    Int_t iNbHits     = 0;
    Int_t iNbCalDigis = 0;
    fTofDigiMatchCollOut->Delete();  // costly, FIXME
    fTofHitsCollOut->Delete();       // costly, FIXME
    //fTofDigiMatchCollOut->Clear("C"); // not sufficient, memory leak
    for (Int_t iEvent = 0; iEvent < fEventsColl->GetEntriesFast(); iEvent++) {
      CbmEvent* tEvent = dynamic_cast<CbmEvent*>(fEventsColl->At(iEvent));
      fTofDigiVec.clear();
      //if (fTofDigisColl) fTofDigisColl->Clear("C");
      //Int_t iNbDigis=0;  (VF) not used
      LOG(debug) << "TS event " << iEvent << " with " << tEvent->GetNofData(ECbmDataType::kT0Digi) << " T0 and "
                 << tEvent->GetNofData(ECbmDataType::kTofDigi) << " Tof digis ";

      for (Int_t iDigi = 0; iDigi < tEvent->GetNofData(ECbmDataType::kT0Digi); iDigi++) {
        Int_t iDigiIndex       = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kT0Digi, iDigi));
        const CbmTofDigi tDigi = fT0DigiVec->at(iDigiIndex);
        if (tDigi.GetType() != 5)
          LOG(fatal) << "Wrong T0 type " << tDigi.GetType() << ", Addr 0x" << std::hex << tDigi.GetAddress();
        fTofDigiVec.push_back(CbmTofDigi(tDigi));
      }
      for (Int_t iDigi = 0; iDigi < tEvent->GetNofData(ECbmDataType::kTofDigi); iDigi++) {
        Int_t iDigiIndex        = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofDigi, iDigi));
        const CbmTofDigi* tDigi = fDigiMan->Get<CbmTofDigi>(iDigiIndex);
        fTofDigiVec.push_back(CbmTofDigi(*tDigi));
        //new((*fTofDigisColl)[iNbDigis++]) CbmTofDigi(*tDigi);
      }

      ExecEvent(option);

      // --- In event-by-event mode: copy caldigis, hits and matches to output array and register them to event

      // Int_t iDigi0=iNbCalDigis; //starting index of current event  (VF) not used
      for (UInt_t index = 0; index < fTofCalDigiVec->size(); index++) {
        //      for (Int_t index = 0; index < fTofCalDigisColl->GetEntriesFast(); index++){
        CbmTofDigi* tDigi = &(fTofCalDigiVec->at(index));
        //CbmTofDigi* tDigi = dynamic_cast<CbmTofDigi*>(fTofCalDigisColl->At(index));
        tEvent->AddData(ECbmDataType::kTofCalDigi, iNbCalDigis);
        fTofCalDigiVecOut->push_back(CbmTofDigi(*tDigi));
        iNbCalDigis++;
        //new((*fTofCalDigisCollOut)[iNbCalDigis++]) CbmTofDigi(*tDigi);
      }

      for (Int_t index = 0; index < fTofHitsColl->GetEntriesFast(); index++) {
        CbmTofHit* pHit = (CbmTofHit*) fTofHitsColl->At(index);
        new ((*fTofHitsCollOut)[iNbHits]) CbmTofHit(*pHit);
        tEvent->AddData(ECbmDataType::kTofHit, iNbHits);

        CbmMatch* pDigiMatch = (CbmMatch*) fTofDigiMatchColl->At(index);
        // update content of match object, not necessary if event definition  is kept !
        /*
				 for (Int_t iLink=0; iLink<pDigiMatch->GetNofLinks(); iLink++) {  // loop over digis
				 CbmLink Link = pDigiMatch->GetLink(iLink);
				 Link.SetIndex(Link.GetIndex()+iDigi0);
				 }
				 */
        new ((*fTofDigiMatchCollOut)[iNbHits]) CbmMatch(*pDigiMatch);

        iNbHits++;
      }
      //fTofDigisColl->Delete();
      fTofDigiVec.clear();
      //fTofCalDigi->Delete();//Clear("C"); //otherwise memoryleak! FIXME
      fTofCalDigiVec->clear();
      fTofHitsColl->Clear("C");
      fTofDigiMatchColl->Delete();  //Clear("C");
    }
  }
  else {
    // fTofDigisColl=fTofRawDigisColl;
    // (VF) This does not work here. The digi manager does not foresee to add
    // new data to the input array. So, I here copy the input digis into
    // the array fTofDigisColl. Not very efficient, but temporary only, until
    // also the internal data representations are changed to std::vectors.

    fTofDigiVec.clear();
    //if (fTofDigisColl) fTofDigisColl->Clear("C");
    // Int_t iNbDigis=0; (VF) not used
    for (Int_t iDigi = 0; iDigi < fDigiMan->GetNofDigis(ECbmModuleId::kTof); iDigi++) {
      const CbmTofDigi* tDigi = fDigiMan->Get<CbmTofDigi>(iDigi);
      fTofDigiVec.push_back(CbmTofDigi(*tDigi));
      //new((*fTofDigisColl)[iNbDigis++]) CbmTofDigi(*tDigi);
    }
    ExecEvent(option);
  }
}

void CbmTofHitMaker::ExecEvent(Option_t* /*option*/)
{
  // Clear output arrays
  //fTofCalDigisColl->Delete(); //otherwise memoryleak if 'CbmDigi::fMatch' points to valid MC match objects (simulation)! FIXME
  fTofCalDigiVec->clear();
  fTofHitsColl->Clear("C");
  //fTofHitsColl->Delete();  // Computationally costly!, but hopefully safe
  //for (Int_t i=0; i<fTofDigiMatchColl->GetEntriesFast(); i++) ((CbmMatch *)(fTofDigiMatchColl->At(i)))->ClearLinks();  // FIXME, try to tamper memory leak (did not help)
  //fTofDigiMatchColl->Clear("C+L");  // leads to memory leak
  fTofDigiMatchColl->Delete();
  FairRootFileSink* bla = (FairRootFileSink*) FairRootManager::Instance()->GetSink();
  if (bla) fiOutputTreeEntry = ((FairRootFileSink*) FairRootManager::Instance()->GetSink())->GetOutTree()->GetEntries();

  fiNbHits = 0;

  fStart.Set();

  BuildClusters();

  MergeClusters();

  fStop.Set();

  fdEvent++;
  FillHistos();

  //   fTofDigisColl->RemoveAll();
}

/************************************************************************************/
void CbmTofHitMaker::Finish()
{
  if (fdEvent < 100) return;  // don't save histos with insufficient statistics
  WriteHistos();
  // Prevent them from being sucked in by the CbmHadronAnalysis WriteHistograms method
  // DeleteHistos();
  if (fdMemoryTime > 0.) CleanLHMemory();
}

void CbmTofHitMaker::Finish(Double_t calMode)
{
  if (fdEvent < 100) return;  // don't save histos with insufficient statistics
  SetCalMode(calMode);
  WriteHistos();
}

/************************************************************************************/
// Functions common for all clusters approximations
Bool_t CbmTofHitMaker::RegisterInputs()
{
  FairRootManager* fManager = FairRootManager::Instance();

  if (NULL == fManager) {
    LOG(error) << "CbmTofHitMaker::RegisterInputs => Could not find "
                  "FairRootManager!!!";
    return kFALSE;
  }  // if( NULL == fTofDigisColl)

  fEventsColl = dynamic_cast<TClonesArray*>(fManager->GetObject("Event"));
  if (NULL == fEventsColl) fEventsColl = dynamic_cast<TClonesArray*>(fManager->GetObject("CbmEvent"));

  if (NULL == fEventsColl) LOG(info) << "CbmEvent not found in input file, assume eventwise input";

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(error) << GetName() << ": No Tof digi input!";
    return kFALSE;
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kT0)) { LOG(info) << GetName() << ": separate T0 digi input!"; }
  else {
    fT0DigiVec = fManager->InitObjectAs<std::vector<CbmTofDigi> const*>("T0Digi");
    if (!fT0DigiVec) { LOG(info) << "No T0 digi input vector found."; }
  }  // if( ! fT0DigiVec )

  fTrbHeader = (TTrbHeader*) fManager->GetObject("TofTrbHeader.");
  if (NULL == fTrbHeader) {
    LOG(info) << "CbmTofHitMaker::RegisterInputs => Could not get "
                 "TofTrbHeader Object";
  }

  if (NULL == fEventsColl) {
    //fTofDigisColl = new TClonesArray("CbmTofDigi");
  }
  else {
    // time based input
    LOG(info) << "CbmEvent found in input file, assume time based input";
    //fTofDigisColl = new TClonesArray("CbmTofDigi");
  }

  return kTRUE;
}
Bool_t CbmTofHitMaker::RegisterOutputs()
{
  FairRootManager* rootMgr = FairRootManager::Instance();
  // FairRunAna* ana = FairRunAna::Instance();  (VF) not used

  rootMgr->InitSink();

  //fTofCalDigisColl = new TClonesArray("CbmTofDigi");
  fTofCalDigiVec = new std::vector<CbmTofDigi>();

  fTofHitsColl = new TClonesArray("CbmTofHit");

  fTofDigiMatchColl = new TClonesArray("CbmMatch", 100);

  TString tHitBranchName;
  TString tHitDigiMatchBranchName;

  if (fbAlternativeBranchNames) {
    tHitBranchName          = "ATofHit";
    tHitDigiMatchBranchName = "ATofDigiMatch";
  }
  else {
    tHitBranchName          = "TofHit";
    tHitDigiMatchBranchName = "TofHitCalDigiMatch";
  }

  if (NULL == fEventsColl) {
    // Flag check to control whether digis are written in output root file
    //rootMgr->Register( "TofCalDigi","Tof", fTofCalDigisColl, fbWriteDigisInOut);
    rootMgr->RegisterAny("TofCalDigi", fTofCalDigiVec, fbWriteDigisInOut);

    // Flag check to control whether digis are written in output root file
    rootMgr->Register(tHitBranchName, "Tof", fTofHitsColl, fbWriteHitsInOut);

    rootMgr->Register(tHitDigiMatchBranchName, "Tof", fTofDigiMatchColl, fbWriteHitsInOut);
  }
  else {  // CbmEvent - mode
    //fTofCalDigisCollOut  = new TClonesArray("CbmTofDigi");
    fTofCalDigiVecOut    = new std::vector<CbmTofDigi>();
    fTofHitsCollOut      = new TClonesArray("CbmTofHit");
    fTofDigiMatchCollOut = new TClonesArray("CbmMatch", 100);
    //rootMgr->Register( "TofCalDigi","Tof", fTofCalDigisCollOut, fbWriteDigisInOut);
    rootMgr->RegisterAny("TofCalDigi", fTofCalDigiVecOut, fbWriteDigisInOut);
    rootMgr->Register(tHitBranchName, "Tof", fTofHitsCollOut, fbWriteHitsInOut);
    rootMgr->Register(tHitDigiMatchBranchName, "Tof", fTofDigiMatchCollOut, fbWriteHitsInOut);
  }
  LOG(info) << "out branches: " << tHitBranchName << ", " << tHitDigiMatchBranchName;
  return kTRUE;
}
Bool_t CbmTofHitMaker::InitParameters()
{

  // Initialize the TOF GeoHandler
  Bool_t isSimulation = kFALSE;
  LOG(info) << "CbmTofHitMaker::InitParameters - Geometry, Mapping, ...  ??";

  // Get Base Container
  FairRun* ana        = FairRun::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  Int_t iGeoVersion = fGeoHandler->Init(isSimulation);
  if (k14a > iGeoVersion) {
    LOG(error) << "CbmTofHitMaker::InitParameters => Only compatible "
                  "with geometries after v14a !!!";
    return kFALSE;
  }
  if (iGeoVersion == k14a) fTofId = new CbmTofDetectorId_v14a();
  else
    fTofId = new CbmTofDetectorId_v21a();

  // create digitization parameters from geometry file
  CbmTofCreateDigiPar* tofDigiPar = new CbmTofCreateDigiPar("TOF Digi Producer", "TOF task");
  LOG(info) << "Create DigiPar ";
  tofDigiPar->Init();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
  if (0 == fDigiPar) {
    LOG(error) << "CbmTofHitMaker::InitParameters => Could not obtain "
                  "the CbmTofDigiPar ";
    return kFALSE;
  }

  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
  if (0 == fDigiBdfPar) {
    LOG(error) << "CbmTofHitMaker::InitParameters => Could not obtain "
                  "the CbmTofDigiBdfPar ";
    return kFALSE;
  }

  rtdb->initContainers(ana->GetRunId());

  LOG(info) << "CbmTofHitMaker::InitParameter: currently " << fDigiPar->GetNrOfModules() << " digi cells ";

  fdMaxTimeDist  = fDigiBdfPar->GetMaxTimeDist();     // in ns
  fdMaxSpaceDist = fDigiBdfPar->GetMaxDistAlongCh();  // in cm

  if (fMaxTimeDist != fdMaxTimeDist) {
    fdMaxTimeDist  = fMaxTimeDist;  // modify default
    fdMaxSpaceDist = fdMaxTimeDist * fDigiBdfPar->GetSignalSpeed()
                     * 0.5;  // cut consistently on positions (with default signal velocity)
  }

  LOG(info) << " BuildCluster with MaxTimeDist " << fdMaxTimeDist << ", MaxSpaceDist " << fdMaxSpaceDist;

  if (fiCluMulMax == 0) fiCluMulMax = 100;
  if (fOutHstFileName == "") { fOutHstFileName = "./tofEventClust.hst.root"; }

  LOG(info) << " Hst Output filename = " << fOutHstFileName;

  LOG(info) << "<I>  BeamRefType = " << fiBeamRefType << ", Sm " << fiBeamRefSm << ", Det " << fiBeamRefDet
            << ", MulMax " << fiBeamRefMulMax;

  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofHitMaker::InitCalibParameter()
{
  // dimension and initialize calib parameter
  // Int_t iNbDet     = fDigiBdfPar->GetNbDet();  (VF) not used
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();

  if (fTotMean != 0.) fdTTotMean = fTotMean;  // adjust target mean for TOT

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
            fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] = 0.;  // initialize
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
            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]    = 0.;  //initialize
            fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] = 1.;  //initialize
            fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]  = 0.;  //initialize
            fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][iSide].resize(nbClWalkBinX);
            for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
              fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][iSide][iWx] = 0.;
            }
          }
        }
      }
    }
  }
  LOG(info) << "CbmTofHitMaker::InitCalibParameter: defaults set";

  /// Save old global file and folder pointer to avoid messing with FairRoot
  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  if (0 < fCalMode) {
    LOG(info) << "CbmTofHitMaker::InitCalibParameter: read histos from "
              << "file " << fCalParFileName;

    // read parameter from histos
    if (fCalParFileName.IsNull()) return kTRUE;

    fCalParFile = new TFile(fCalParFileName, "");
    if (NULL == fCalParFile) {
      LOG(fatal) << "CbmTofHitMaker::InitCalibParameter: "
                 << "file " << fCalParFileName << " does not exist!";
      return kTRUE;
    }
    /*
		 gDirectory->Print();
		 fCalParFile->cd();
		 fCalParFile->ls();
		 */
    for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
      Int_t iNbSm     = fDigiBdfPar->GetNbSm(iSmType);
      Int_t iNbRpc    = fDigiBdfPar->GetNbRpc(iSmType);
      TProfile* hSvel = (TProfile*) gDirectory->FindObjectAny(Form("cl_SmT%01d_Svel", iSmType));

      // copy Histo to memory
      TDirectory* curdir = gDirectory;
      if (NULL != hSvel) {
        gDirectory->cd(oldDir->GetPath());
        // TProfile *hSvelmem = (TProfile *)hSvel->Clone();  (VF) not used
        gDirectory->cd(curdir->GetPath());
      }
      else {
        LOG(info) << "Svel histogram not found for module type " << iSmType;
      }

      for (Int_t iPar = 0; iPar < 4; iPar++) {
        TProfile* hFparcur = (TProfile*) gDirectory->FindObjectAny(Form("cl_SmT%01d_Fpar%1d", iSmType, iPar));
        if (NULL != hFparcur) {
          gDirectory->cd(oldDir->GetPath());
          // TProfile *hFparmem = (TProfile *)hFparcur->Clone();  (VF) not used
          gDirectory->cd(curdir->GetPath());
        }
      }

      for (Int_t iSm = 0; iSm < iNbSm; iSm++)
        for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {

          // update default parameter
          if (NULL != hSvel) {
            Double_t Vscal = 1.;  //hSvel->GetBinContent(iSm*iNbRpc+iRpc+1);
            if (Vscal == 0.) Vscal = 1.;
            fDigiBdfPar->SetSigVel(iSmType, iSm, iRpc, fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * Vscal);
            LOG(info) << "Modify " << iSmType << iSm << iRpc << " Svel by " << Vscal << " to "
                      << fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);
          }
          TH2F* htempPos_pfx =
            (TH2F*) gDirectory->FindObjectAny(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Pos_pfx", iSmType, iSm, iRpc));
          TH2F* htempTOff_pfx =
            (TH2F*) gDirectory->FindObjectAny(Form("cl_CorSmT%01d_sm%03d_rpc%03d_TOff_pfx", iSmType, iSm, iRpc));
          TH1D* htempTot_Mean =
            (TH1D*) gDirectory->FindObjectAny(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Mean", iSmType, iSm, iRpc));
          TH1D* htempTot_Off =
            (TH1D*) gDirectory->FindObjectAny(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Off", iSmType, iSm, iRpc));
          if (NULL != htempPos_pfx && NULL != htempTOff_pfx && NULL != htempTot_Mean && NULL != htempTot_Off) {
            Int_t iNbCh    = fDigiBdfPar->GetNbChan(iSmType, iRpc);
            Int_t iNbinTot = htempTot_Mean->GetNbinsX();
            for (Int_t iCh = 0; iCh < iNbCh; iCh++) {

              for (Int_t iSide = 0; iSide < 2; iSide++) {
                Double_t TotMean = htempTot_Mean->GetBinContent(iCh * 2 + 1 + iSide);  //nh +1 empirical(?)
                if (0.001 < TotMean) { fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] *= fdTTotMean / TotMean; }
                fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] = htempTot_Off->GetBinContent(iCh * 2 + 1 + iSide);
              }

              Double_t YMean = ((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1);
              Double_t TMean = ((TProfile*) htempTOff_pfx)->GetBinContent(iCh + 1);
              //Double_t dTYOff=YMean/fDigiBdfPar->GetSignalSpeed() ;
              Double_t dTYOff = YMean / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] += -dTYOff + TMean;
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] += +dTYOff + TMean;

              if (5 == iSmType || 8 == iSmType) {  // for PAD counters
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]    = fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1] = fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]  = fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][0];
              }

              LOG(debug) << "CbmTofHitMaker::InitCalibParameter:"
                         << " SmT " << iSmType << " Sm " << iSm << " Rpc " << iRpc << " Ch " << iCh
                         << Form(": YMean %f, TMean %f", YMean, TMean) << " -> "
                         << Form(" %f, %f, %f, %f ", fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                                 fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1],
                                 fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                                 fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1])
                         << ", NbinTot " << iNbinTot;

              TH1D* htempWalk0 = (TH1D*) gDirectory->FindObjectAny(
                Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px", iSmType, iSm, iRpc, iCh));
              TH1D* htempWalk1 = (TH1D*) gDirectory->FindObjectAny(
                Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px", iSmType, iSm, iRpc, iCh));
              if (NULL != htempWalk0 && NULL != htempWalk1) {  // reinitialize Walk array
                LOG(debug) << "Initialize Walk correction for "
                           << Form(" SmT%01d_sm%03d_rpc%03d_Ch%03d", iSmType, iSm, iRpc, iCh);
                if (htempWalk0->GetNbinsX() != nbClWalkBinX)
                  LOG(error) << "CbmTofHitMaker::InitCalibParameter: "
                                "Inconsistent Walk histograms";
                for (Int_t iBin = 0; iBin < nbClWalkBinX; iBin++) {
                  fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iBin] = htempWalk0->GetBinContent(iBin + 1);
                  fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iBin] = htempWalk1->GetBinContent(iBin + 1);
                  if (iCh == 5 && iBin == 10)  // debugging
                    LOG(info) << Form("Read New SmT%01d_sm%03d_rpc%03d_Ch%03d bin %d walk %f ", iSmType, iSm, iRpc, iCh,
                                      iBin, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iBin]);
                  if (5 == iSmType || 8 == iSmType) {  // Pad structure
                    fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iBin] =
                      fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iBin];
                  }
                }
              }
              else {
                LOG(info) << "No Walk histograms for TSRC " << iSmType << iSm << iRpc << iCh;
              }
            }
          }
          else {
            LOG(warning) << " Calibration histos " << Form("cl_SmT%01d_sm%03d_rpc%03d_XXX", iSmType, iSm, iRpc)
                         << " not found. ";
          }
          for (Int_t iSel = 0; iSel < iNSel; iSel++) {
            TH1D* htmpDelTof = (TH1D*) gDirectory->FindObjectAny(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
            if (NULL == htmpDelTof) {
              LOG(debug) << " Histos " << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel)
                         << " not found. ";
              continue;
            }
            LOG(debug) << " Load DelTof from histos "
                       << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel) << ".";
            for (Int_t iBx = 0; iBx < nbClDelTofBinX; iBx++) {
              fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] += htmpDelTof->GetBinContent(iBx + 1);
            }

            // copy Histo to memory
            // TDirectory * curdir = gDirectory;
            gDirectory->cd(oldDir->GetPath());
            TH1D* h1DelTof =
              (TH1D*) htmpDelTof->Clone(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));

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
  LOG(info) << "CbmTofHitMaker::InitCalibParameter: initialization done";
  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofHitMaker::LoadGeometry()
{
  LOG(info) << "CbmTofHitMaker::LoadGeometry starting for  " << fDigiBdfPar->GetNbDet() << " described detectors, "
            << fDigiPar->GetNrOfModules() << " geometrically known cells ";

  Int_t iNrOfCells = fDigiPar->GetNrOfModules();
  LOG(info) << "Digi Parameter container contains " << iNrOfCells << " cells.";
  for (Int_t icell = 0; icell < iNrOfCells; ++icell) {

    Int_t cellId = fDigiPar->GetCellId(icell);  // cellId is assigned in CbmTofCreateDigiPar
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
    LOG(debug) << "-I- InitPar " << icell << " Id: " << Form("0x%08x", cellId) << " " << cell << " tmcs: " << smtype
               << " " << smodule << " " << module << " " << cell << " x=" << Form("%6.2f", x)
               << " y=" << Form("%6.2f", y) << " z=" << Form("%6.2f", z) << " dx=" << dx << " dy=" << dy;

    TGeoNode* fNode =  // prepare local->global trafo
      gGeoManager->FindNode(fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
    LOG(debug2) << Form(" Node at (%6.1f,%6.1f,%6.1f) : 0x%p", fChannelInfo->GetX(), fChannelInfo->GetY(),
                        fChannelInfo->GetZ(), fNode);
    if (icell == 0) {
      TGeoHMatrix* cMatrix = gGeoManager->GetCurrentMatrix();
      fNode->Print();
      fDigiPar->GetNode(cellId)->Print();
      cMatrix->Print();
    }
  }

  Int_t iNbDet = fDigiBdfPar->GetNbDet();
  fvDeadStrips.resize(iNbDet);
  fvTimeLastDigi.resize(iNbDet);
  fvTimeFirstDigi.resize(iNbDet);
  fvMulDigi.resize(iNbDet);

  for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
    Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);
    Int_t iSmType   = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSmId     = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpcId    = CbmTofAddress::GetRpcId(iUniqueId);
    LOG(info) << " DetIndx " << iDetIndx << "(" << iNbDet << "), SmType " << iSmType << ", SmId " << iSmId << ", RpcId "
              << iRpcId << " => UniqueId " << Form("0x%08x ", iUniqueId)
              << Form(" Svel %6.6f, DeadStrips 0x%08x ", fDigiBdfPar->GetSigVel(iSmType, iSmId, iRpcId),
                      fvDeadStrips[iDetIndx]);
    Int_t iNbChan = fDigiBdfPar->GetNbChan(iSmType, iRpcId);
    fvTimeLastDigi[iDetIndx].resize(iNbChan * 2);
    for (Int_t iCh = 0; iCh < iNbChan * 2; iCh++)
      fvTimeLastDigi[iDetIndx][iCh] = 0.;
    fvTimeFirstDigi[iDetIndx].resize(iNbChan * 2);
    fvMulDigi[iDetIndx].resize(iNbChan * 2);
    for (Int_t iCh = 0; iCh < iNbChan * 2; iCh++) {
      fvTimeFirstDigi[iDetIndx][iCh] = 0.;
      fvMulDigi[iDetIndx][iCh]       = 0.;
    }

    Int_t iCell = -1;
    while (kTRUE) {
      Int_t iUCellId = CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, ++iCell, 0, iSmType);
      fChannelInfo   = fDigiPar->GetCell(iUCellId);
      if (NULL == fChannelInfo) break;
      LOG(debug3) << " Cell " << iCell << Form(" 0x%08x ", iUCellId) << Form(", fCh 0x%p ", fChannelInfo)
                  << ", x: " << fChannelInfo->GetX() << ", y: " << fChannelInfo->GetY()
                  << ", z: " << fChannelInfo->GetZ();
    }
  }

  //   return kTRUE;

  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();

  if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
    fStorDigi.resize(iNbSmTypes);
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
      fStorDigi[iSmType].resize(iNbSm * iNbRpc);
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
            LOG(warning) << "CbmTofHitMaker::LoadGeometry: StoreDigi "
                            "without channels "
                         << Form("SmTy %3d, Sm %3d, NbRpc %3d, Rpc, %3d ", iSmType, iSm, iNbRpc, iRpc);
          }
          LOG(debug1) << "CbmTofHitMaker::LoadGeometry: StoreDigi with "
                      << Form(" %3d %3d %3d %3d %5d ", iSmType, iSm, iNbRpc, iRpc, iNbChan);
          fStorDigi[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
          fvLastHits[iSmType][iSm][iRpc].resize(iNbChan);
        }  // for( Int_t iRpc = 0; iRpc < iNbRpc; iRpc++ )
      }    // for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
    }      // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
  }        // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )

  return kTRUE;
}
Bool_t CbmTofHitMaker::DeleteGeometry()
{
  LOG(info) << "CbmTofHitMaker::DeleteGeometry starting";
  return kTRUE;
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();
  if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
    for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
      Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
      Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
      for (Int_t iSm = 0; iSm < iNbSm; iSm++) {
        for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
          fStorDigi[iSmType][iSm * iNbRpc + iRpc].clear();
          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc].clear();
        }
      }  // for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
      fStorDigi[iSmType].clear();
      fStorDigiInd[iSmType].clear();
    }  // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
    fStorDigi.clear();
    fStorDigiInd.clear();
  }  // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
  return kTRUE;
}
/************************************************************************************/
// Histogramming functions
Bool_t CbmTofHitMaker::CreateHistos()
{
  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  TDirectory* oldir = gDirectory;
  gROOT->cd();
  fhClustBuildTime =
    new TH1I("TofClustBuildTime", "Time needed to build clusters in each event; Time [s]", 100, 0.0, 4.0);
  gDirectory->cd(oldir->GetPath());
  // <= To prevent histos from being sucked in by the param file of the TRootManager!

  return kTRUE;
}

Bool_t CbmTofHitMaker::FillHistos()
{
  fhClustBuildTime->Fill(fStop.GetSec() - fStart.GetSec() + (fStop.GetNanoSec() - fStart.GetNanoSec()) / 1e9);
  return kTRUE;
}

Bool_t CbmTofHitMaker::WriteHistos()
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* fHist;
  fHist = new TFile(fOutHstFileName, "RECREATE");
  fHist->cd();
  fhClustBuildTime->Write();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  fHist->Close();

  return kTRUE;
}
Bool_t CbmTofHitMaker::DeleteHistos()
{
  delete fhClustBuildTime;
  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofHitMaker::BuildClusters()
{
  //gGeoManager->SetTopVolume( gGeoManager->FindVolumeFast("tof_v14a") );
  gGeoManager->CdTop();

  //  if(NULL == fTofDigisColl) {
  if (fTofDigiVec.empty()) {
    LOG(info) << " No RawDigis defined ! Check! ";
    return kFALSE;
  }
  fiNevtBuild++;
  LOG(debug) << "Build clusters from "
             //            <<fTofDigisColl->GetEntriesFast()<<" digis in event "<<fiNevtBuild;
             << fTofDigiVec.size() << " digis in event " << fiNevtBuild;

  fTRefHits = 0.;

  Int_t iNbTofDigi = fTofDigiVec.size();
  //Int_t iNbTofDigi = fTofDigisColl->GetEntriesFast();
  if (iNbTofDigi > 100000) {
    LOG(warning) << "Too many digis in event " << fiNevtBuild;
    return kFALSE;
  }
  if (bAddBeamCounterSideDigi) {
    // Duplicate type "5" - digis
    // Int_t iNbDigi=iNbTofDigi;
    for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
      CbmTofDigi* pDigi = &(fTofDigiVec.at(iDigInd));
      //CbmTofDigi *pDigi = (CbmTofDigi*) fTofDigisColl->At( iDigInd );
      if (pDigi->GetType() == 5) {
        if (pDigi->GetSide() == 1) {
          bAddBeamCounterSideDigi = kFALSE;  // disable for current data set
          LOG(info) << "Start counter digi duplication disabled";
          break;
        }
        fTofDigiVec.push_back(CbmTofDigi(*pDigi));
        CbmTofDigi* pDigiN = &(fTofDigiVec.back());
        //	 CbmTofDigi *pDigiN  = new((*fTofDigisColl)[iNbDigi++]) CbmTofDigi( *pDigi );
        pDigiN->SetAddress(pDigi->GetSm(), pDigi->GetRpc(), pDigi->GetChannel(), (0 == pDigi->GetSide()) ? 1 : 0,
                           pDigi->GetType());
        LOG(debug) << "Duplicated digi " << fTofDigiVec.size() << " with address 0x" << std::hex
                   << pDigiN->GetAddress();
      }
    }
    iNbTofDigi = fTofDigiVec.size();
    //iNbTofDigi = fTofDigisColl->GetEntriesFast(); // Update
  }

  if (kTRUE) {
    for (UInt_t iDetIndx = 0; iDetIndx < fvTimeFirstDigi.size(); iDetIndx++)
      for (UInt_t iCh = 0; iCh < fvTimeFirstDigi[iDetIndx].size(); iCh++) {
        fvTimeFirstDigi[iDetIndx][iCh] = 0.;
        fvMulDigi[iDetIndx][iCh]       = 0.;
      }

    for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
      //CbmTofDigi *pDigi = (CbmTofDigi*) fTofDigisColl->At( iDigInd );
      CbmTofDigi* pDigi = &(fTofDigiVec.at(iDigInd));
      Int_t iDetIndx    = fDigiBdfPar->GetDetInd(pDigi->GetAddress());

      LOG(debug) << iDigInd << " " << pDigi << Form(" Address : 0x%08x ", pDigi->GetAddress()) << " SmT "
                 << pDigi->GetType() << " Sm " << pDigi->GetSm() << " Rpc " << pDigi->GetRpc() << " Ch "
                 << pDigi->GetChannel() << " S " << pDigi->GetSide() << ", DetIndx " << iDetIndx << " : "
                 << pDigi->ToString()
        //         <<" Time "<<pDigi->GetTime()
        //         <<" Tot " <<pDigi->GetTot()
        ;

      if (fDigiBdfPar->GetNbDet() - 1 < iDetIndx || iDetIndx < 0) {
        LOG(debug) << Form(" Wrong DetIndx %d >< %d ", iDetIndx, fDigiBdfPar->GetNbDet());
        break;
      }

      size_t iDigiCh = pDigi->GetChannel() * 2 + pDigi->GetSide();
      if (iDigiCh < fvTimeLastDigi[iDetIndx].size()) {
        fvTimeLastDigi[iDetIndx][iDigiCh] = pDigi->GetTime();

        if (fvTimeFirstDigi[iDetIndx][iDigiCh] != 0.) { fvMulDigi[iDetIndx][iDigiCh]++; }
        else {
          fvTimeFirstDigi[iDetIndx][iDigiCh] = pDigi->GetTime();
          fvMulDigi[iDetIndx][iDigiCh]++;
        }
      }
    }
    for (UInt_t iDetIndx = 0; iDetIndx < fvTimeFirstDigi.size(); iDetIndx++)
      for (UInt_t iCh = 0; iCh < fvTimeFirstDigi[iDetIndx].size(); iCh++) {
        if (fvTimeFirstDigi[iDetIndx][iCh] != 0.) fhRpcDigiDTMul[iDetIndx]->Fill(iCh, fvMulDigi[iDetIndx][iCh]);
      }
  }  // kTRUE end

  // Calibrate RawDigis
  if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
    CbmTofDigi* pDigi;
    // CbmTofDigi *pCalDigi=NULL;  (VF) not used
    CalibRawDigis();

    // Then loop over the digis array and store the Digis in separate vectors for
    // each RPC modules

    //      iNbTofDigi = fTofCalDigisColl->GetEntriesFast();
    iNbTofDigi = fTofCalDigiVec->size();
    for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
      //         pDigi = (CbmTofDigi*) fTofCalDigisColl->At( iDigInd );
      pDigi = &(fTofCalDigiVec->at(iDigInd));
      LOG(debug1) << "AC "  // After Calibration
                  << Form("0x%08x", pDigi->GetAddress()) << " TSRC " << pDigi->GetType() << pDigi->GetSm()
                  << pDigi->GetRpc() << Form("%2d", (Int_t) pDigi->GetChannel()) << " " << pDigi->GetSide() << " "
                  << Form("%f", pDigi->GetTime()) << " " << pDigi->GetTot();

      if (fDigiBdfPar->GetNbSmTypes() > pDigi->GetType()  // prevent crash due to misconfiguration
          && fDigiBdfPar->GetNbSm(pDigi->GetType()) > pDigi->GetSm()
          && fDigiBdfPar->GetNbRpc(pDigi->GetType()) > pDigi->GetRpc()
          && fDigiBdfPar->GetNbChan(pDigi->GetType(), pDigi->GetRpc()) > pDigi->GetChannel()) {
        fStorDigi[pDigi->GetType()][pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType()) + pDigi->GetRpc()]
                 [pDigi->GetChannel()]
                   .push_back(pDigi);
        fStorDigiInd[pDigi->GetType()][pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType()) + pDigi->GetRpc()]
                    [pDigi->GetChannel()]
                      .push_back(iDigInd);
      }
      else {
        LOG(info) << "Skip2 Digi "
                  << " Type " << pDigi->GetType() << " " << fDigiBdfPar->GetNbSmTypes() << " Sm " << pDigi->GetSm()
                  << " " << fDigiBdfPar->GetNbSm(pDigi->GetType()) << " Rpc " << pDigi->GetRpc() << " "
                  << fDigiBdfPar->GetNbRpc(pDigi->GetType()) << " Ch " << pDigi->GetChannel() << " "
                  << fDigiBdfPar->GetNbChan(pDigi->GetType(), 0);
      }
    }  // for( Int_t iDigInd = 0; iDigInd < nTofDigi; iDigInd++ )

    // inspect digi array
    Int_t iNbDet = fDigiBdfPar->GetNbDet();
    for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
      Int_t iDetId    = fviDetId[iDetIndx];
      Int_t iSmType   = CbmTofAddress::GetSmType(iDetId);
      Int_t iSm       = CbmTofAddress::GetSmId(iDetId);
      Int_t iRpc      = CbmTofAddress::GetRpcId(iDetId);
      Int_t iNbStrips = fDigiBdfPar->GetNbChan(iSmType, iRpc);
      for (Int_t iStrip = 0; iStrip < iNbStrips; iStrip++) {
        Int_t iDigiMul = fStorDigi[iSmType][iSm * fDigiBdfPar->GetNbRpc(iSmType) + iRpc][iStrip].size();
        //LOG(info)<<"Inspect TSRC "<<iSmType<<iSm<<iRpc<<iStrip<<" with "<<iNbStrips<<" strips: Mul "<<iDigiMul;
        if (iDigiMul > 0) {
          fhRpcDigiMul[iDetIndx]->Fill(iStrip, iDigiMul);
          if (iDigiMul == 1) {
            fhRpcDigiStatus[iDetIndx]->Fill(iStrip, 0);
            if (iStrip > 0)
              if (fStorDigi[iSmType][iSm * fDigiBdfPar->GetNbRpc(iSmType) + iRpc][iStrip - 1].size() > 1) {
                fhRpcDigiStatus[iDetIndx]->Fill(iStrip, 1);
                if (TMath::Abs(
                      fStorDigi[iSmType][iSm * fDigiBdfPar->GetNbRpc(iSmType) + iRpc][iStrip][0]->GetTime()
                      - fStorDigi[iSmType][iSm * fDigiBdfPar->GetNbRpc(iSmType) + iRpc][iStrip - 1][0]->GetTime())
                    < fMaxTimeDist)
                  fhRpcDigiStatus[iDetIndx]->Fill(iStrip, 3);
              }
            if (iStrip < iNbStrips - 2) {
              if (fStorDigi[iSmType][iSm * fDigiBdfPar->GetNbRpc(iSmType) + iRpc][iStrip + 1].size() > 1) {
                fhRpcDigiStatus[iDetIndx]->Fill(iStrip, 2);
                if (TMath::Abs(
                      fStorDigi[iSmType][iSm * fDigiBdfPar->GetNbRpc(iSmType) + iRpc][iStrip][0]->GetTime()
                      - fStorDigi[iSmType][iSm * fDigiBdfPar->GetNbRpc(iSmType) + iRpc][iStrip + 1][0]->GetTime())
                    < fMaxTimeDist)
                  fhRpcDigiStatus[iDetIndx]->Fill(iStrip, 4);
              }
            }
          }
        }
      }
    }

    BuildHits();

  }  // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
  else {
    LOG(error) << " Compressed Digis not implemented ... ";
    return kFALSE;  // not implemented properly yet
  }
  return kTRUE;
}

Bool_t CbmTofHitMaker::MergeClusters()
{
  // Merge clusters from neigbouring Rpc within a (Super)Module
  if (NULL == fTofHitsColl) {
    LOG(info) << " No Hits defined ! Check! ";
    return kFALSE;
  }
  // inspect hits
  for (Int_t iHitInd = 0; iHitInd < fTofHitsColl->GetEntriesFast(); iHitInd++) {
    CbmTofHit* pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
    if (NULL == pHit) continue;

    Int_t iDetId  = (pHit->GetAddress() & DetMask);
    Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
    Int_t iNbRpc  = fDigiBdfPar->GetNbRpc(iSmType);
    if (iSmType != 5 && iSmType != 8) continue;  // only merge diamonds and Pad
    LOG(debug) << "MergeClusters: in SmT " << iSmType << " for " << iNbRpc << " Rpcs";

    if (iNbRpc > 1) {  // check for possible mergers
      Int_t iSm    = CbmTofAddress::GetSmId(iDetId);
      Int_t iRpc   = CbmTofAddress::GetRpcId(iDetId);
      Int_t iChId  = pHit->GetAddress();
      fChannelInfo = fDigiPar->GetCell(iChId);
      Int_t iCh    = CbmTofAddress::GetChannelId(iChId);
      LOG(debug) << "MergeClusters: Check for mergers in "
                 << Form(" SmT %d, Sm %d, Rpc %d, Ch %d - hit %d", iSmType, iSm, iRpc, iCh, iHitInd);
      for (Int_t iHitInd2 = iHitInd + 1; iHitInd2 < fTofHitsColl->GetEntriesFast(); iHitInd2++) {
        CbmTofHit* pHit2 = (CbmTofHit*) fTofHitsColl->At(iHitInd2);
        if (NULL == pHit2) continue;
        Int_t iDetId2  = (pHit2->GetAddress() & DetMask);
        Int_t iSmType2 = CbmTofAddress::GetSmType(iDetId2);
        if (iSmType2 == iSmType) {
          Int_t iSm2 = CbmTofAddress::GetSmId(iDetId2);
          if (iSm2 == iSm || iSmType == 5) {
            Int_t iRpc2 = CbmTofAddress::GetRpcId(iDetId2);
            if (TMath::Abs(iRpc - iRpc2) == 1 || iSm2 != iSm) {  // Found neighbour
              Int_t iChId2 = pHit2->GetAddress();
              // CbmTofCell  *fChannelInfo2 = fDigiPar->GetCell( iChId2 );  (VF) not used
              Int_t iCh2     = CbmTofAddress::GetChannelId(iChId2);
              Double_t xPos  = pHit->GetX();
              Double_t yPos  = pHit->GetY();
              Double_t tof   = pHit->GetTime();
              Double_t xPos2 = pHit2->GetX();
              Double_t yPos2 = pHit2->GetY();
              Double_t tof2  = pHit2->GetTime();
              LOG(debug) << "MergeClusters: Found hit in neighbour "
                         << Form(" SmT %d, Sm %d, Rpc %d, Ch %d - hit %d", iSmType2, iSm2, iRpc2, iCh2, iHitInd2)
                         << Form(" DX %6.1f, DY %6.1f, DT %6.1f", xPos - xPos2, yPos - yPos2, tof - tof2);

              if (TMath::Abs(xPos - xPos2) < fdCaldXdYMax * 2. && TMath::Abs(yPos - yPos2) < fdCaldXdYMax * 2.
                  && TMath::Abs(tof - tof2) < fMaxTimeDist) {

                CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
                Double_t dTot       = 0;
                for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink += 2) {  // loop over digis
                  CbmLink L0      = digiMatch->GetLink(iLink);
                  UInt_t iDigInd0 = L0.GetIndex();
                  UInt_t iDigInd1 = (digiMatch->GetLink(iLink + 1)).GetIndex();
                  //                    if (iDigInd0 < fTofCalDigisColl->GetEntriesFast() && iDigInd1 < fTofCalDigisColl->GetEntriesFast()){
                  if (iDigInd0 < fTofCalDigiVec->size() && iDigInd1 < fTofCalDigiVec->size()) {
                    //                      CbmTofDigi *pDig0 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
                    //                      CbmTofDigi *pDig1 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd1));
                    CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
                    CbmTofDigi* pDig1 = &(fTofCalDigiVec->at(iDigInd1));
                    dTot += pDig0->GetTot();
                    dTot += pDig1->GetTot();
                  }
                }

                CbmMatch* digiMatch2 = (CbmMatch*) fTofDigiMatchColl->At(iHitInd2);
                Double_t dTot2       = 0;
                for (Int_t iLink = 0; iLink < digiMatch2->GetNofLinks(); iLink += 2) {  // loop over digis
                  CbmLink L0      = digiMatch2->GetLink(iLink);
                  UInt_t iDigInd0 = L0.GetIndex();
                  UInt_t iDigInd1 = (digiMatch2->GetLink(iLink + 1)).GetIndex();
                  //                    if (iDigInd0 < fTofCalDigisColl->GetEntriesFast() && iDigInd1 < fTofCalDigisColl->GetEntriesFast()){
                  if (iDigInd0 < fTofCalDigiVec->size() && iDigInd1 < fTofCalDigiVec->size()) {
                    //                      CbmTofDigi *pDig0 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
                    //                      CbmTofDigi *pDig1 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd1));
                    CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
                    CbmTofDigi* pDig1 = &(fTofCalDigiVec->at(iDigInd1));
                    dTot2 += pDig0->GetTot();
                    dTot2 += pDig1->GetTot();
                    digiMatch->AddLink(CbmLink(pDig0->GetTot(), iDigInd0, fiOutputTreeEntry, fiFileIndex));
                    digiMatch->AddLink(CbmLink(pDig1->GetTot(), iDigInd1, fiOutputTreeEntry, fiFileIndex));
                  }
                }
                LOG(debug) << "MergeClusters: Found merger in neighbour "
                           << Form(" SmT %d, Sm %d, Rpc %d, Ch %d - hit %d(%d)", iSmType2, iSm2, iRpc2, iCh2, iHitInd2,
                                   fTofHitsColl->GetEntriesFast())
                           << Form(" DX %6.1f, DY %6.1f, DT %6.1f", xPos - xPos2, yPos - yPos2, tof - tof2)
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
                LOG(debug) << "MergeClusters: Compress TClonesArrays to " << fTofHitsColl->GetEntriesFast() << ", "
                           << fTofDigiMatchColl->GetEntriesFast();
                /*
								 for(Int_t i=iHitInd2; i<fTofHitsColl->GetEntriesFast(); i++){ // update RefLinks
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

static Double_t f1_xboxe(double* x, double* par)
{
  double xx    = x[0];
  double wx    = 1. - par[4] * TMath::Power(xx + par[5], 2);
  double xboxe = par[0] * 0.25 * (1. + TMath::Erf((xx + par[1] - par[3]) / par[2]))
                 * (1. + TMath::Erf((-xx + par[1] + par[3]) / par[2]));
  return xboxe * wx;
}

void CbmTofHitMaker::fit_ybox(const char* hname)
{
  TH1* h1;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (NULL != h1) { fit_ybox(h1, 0.); }
}

void CbmTofHitMaker::fit_ybox(TH1* h1, Double_t ysize)
{
  Double_t* fpar = NULL;
  fit_ybox(h1, ysize, fpar);
}

void CbmTofHitMaker::fit_ybox(TH1* h1, Double_t ysize, Double_t* fpar = NULL)
{
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

  h1->Fit("YBox", "QM0");

  double res[10];
  double err[10];
  res[9] = f1->GetChisquare();

  for (int i = 0; i < 6; i++) {
    res[i] = f1->GetParameter(i);
    err[i] = f1->GetParError(i);
    //cout << " FPar "<< i << ": " << res[i] << ", " << err[i] << endl;
  }
  LOG(debug) << "YBox Fit of " << h1->GetName() << " ended with chi2 = " << res[9]
             << Form(", strip length %7.2f +/- %5.2f, position resolution "
                     "%7.2f +/- %5.2f at y_cen = %7.2f +/- %5.2f",
                     2. * res[1], 2. * err[1], res[2], err[2], res[3], err[3]);
}

void CbmTofHitMaker::CheckLHMemory()
{
  if (fvLastHits.size() != static_cast<size_t>(fDigiBdfPar->GetNbSmTypes()))
    LOG(fatal) << Form("Inconsistent LH Smtype size %lu, %d ", fvLastHits.size(), fDigiBdfPar->GetNbSmTypes());

  for (Int_t iSmType = 0; iSmType < fDigiBdfPar->GetNbSmTypes(); iSmType++) {
    if (fvLastHits[iSmType].size() != static_cast<size_t>(fDigiBdfPar->GetNbSm(iSmType)))
      LOG(fatal) << Form("Inconsistent LH Sm size %lu, %d T %d", fvLastHits[iSmType].size(),
                         fDigiBdfPar->GetNbSm(iSmType), iSmType);
    for (Int_t iSm = 0; iSm < fDigiBdfPar->GetNbSm(iSmType); iSm++) {
      if (fvLastHits[iSmType][iSm].size() != static_cast<size_t>(fDigiBdfPar->GetNbRpc(iSmType)))
        LOG(fatal) << Form("Inconsistent LH Rpc size %lu, %d TS %d%d ", fvLastHits[iSmType][iSm].size(),
                           fDigiBdfPar->GetNbRpc(iSmType), iSmType, iSm);
      for (Int_t iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
        if (fvLastHits[iSmType][iSm][iRpc].size() != static_cast<size_t>(fDigiBdfPar->GetNbChan(iSmType, iRpc)))
          LOG(fatal) << Form("Inconsistent LH RpcChannel size %lu, %d TSR %d%d%d ",
                             fvLastHits[iSmType][iSm][iRpc].size(), fDigiBdfPar->GetNbChan(iSmType, iRpc), iSmType, iSm,
                             iRpc);
        for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpc); iCh++)
          if (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 0) {
            CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
            Int_t iAddr = fTofId->SetDetectorInfo(xDetInfo);
            if (fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress() != iAddr)
              LOG(fatal) << Form("Inconsistent address for Ev %8.0f in list of size %lu for "
                                 "TSRC %d%d%d%d: 0x%08x, time  %f",
                                 fdEvent, fvLastHits[iSmType][iSm][iRpc][iCh].size(), iSmType, iSm, iRpc, iCh,
                                 fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress(),
                                 fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime());
          }
      }
    }
  }
  LOG(debug) << Form("LH check passed for event %8.0f", fdEvent);
}

void CbmTofHitMaker::CleanLHMemory()
{
  if (fvLastHits.size() != static_cast<size_t>(fDigiBdfPar->GetNbSmTypes()))
    LOG(fatal) << Form("Inconsistent LH Smtype size %lu, %d ", fvLastHits.size(), fDigiBdfPar->GetNbSmTypes());
  for (Int_t iSmType = 0; iSmType < fDigiBdfPar->GetNbSmTypes(); iSmType++) {
    if (fvLastHits[iSmType].size() != static_cast<size_t>(fDigiBdfPar->GetNbSm(iSmType)))
      LOG(fatal) << Form("Inconsistent LH Sm size %lu, %d T %d", fvLastHits[iSmType].size(),
                         fDigiBdfPar->GetNbSm(iSmType), iSmType);
    for (Int_t iSm = 0; iSm < fDigiBdfPar->GetNbSm(iSmType); iSm++) {
      if (fvLastHits[iSmType][iSm].size() != static_cast<size_t>(fDigiBdfPar->GetNbRpc(iSmType)))
        LOG(fatal) << Form("Inconsistent LH Rpc size %lu, %d TS %d%d ", fvLastHits[iSmType][iSm].size(),
                           fDigiBdfPar->GetNbRpc(iSmType), iSmType, iSm);
      for (Int_t iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
        if (fvLastHits[iSmType][iSm][iRpc].size() != static_cast<size_t>(fDigiBdfPar->GetNbChan(iSmType, iRpc)))
          LOG(fatal) << Form("Inconsistent LH RpcChannel size %lu, %d TSR %d%d%d ",
                             fvLastHits[iSmType][iSm][iRpc].size(), fDigiBdfPar->GetNbChan(iSmType, iRpc), iSmType, iSm,
                             iRpc);
        for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpc); iCh++)
          while (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 0) {
            CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
            Int_t iAddr = fTofId->SetDetectorInfo(xDetInfo);
            if (fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress() != iAddr)
              LOG(fatal) << Form("Inconsistent address for Ev %8.0f in list of size %lu for "
                                 "TSRC %d%d%d%d: 0x%08x, time  %f",
                                 fdEvent, fvLastHits[iSmType][iSm][iRpc][iCh].size(), iSmType, iSm, iRpc, iCh,
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

Bool_t CbmTofHitMaker::AddNextChan(Int_t iSmType, Int_t iSm, Int_t iRpc, Int_t iLastChan, Double_t dLastPosX,
                                   Double_t dLastPosY, Double_t dLastTime, Double_t dLastTotS)
{
  // Int_t iNbSm  = fDigiBdfPar->GetNbSm(  iSmType);  (VF) not used
  Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
  Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
  // Int_t iChType = fDigiBdfPar->GetChanType( iSmType, iRpc );  (VF) not used
  Int_t iDetId   = CbmTofAddress::GetUniqueAddress(iSm, iRpc, 0, 0, iSmType);
  Int_t iDetIndx = fDetIdIndexMap[iDetId];  // Detector Index

  Int_t iCh   = iLastChan + 1;
  Int_t iChId = CbmTofAddress::GetUniqueAddress(iSm, iRpc, iCh, 0, iSmType);

  while (fvDeadStrips[iDetIndx] & (1 << iCh)) {
    LOG(debug) << "Skip channel " << iCh << " of detector " << Form("0x%08x", iDetId);
    iCh++;
    iLastChan++;
    if (iCh >= iNbCh) return kFALSE;
  }
  LOG(debug1) << Form("Inspect channel TSRC %d%d%d%d at time %f, pos %f, size ", iSmType, iSm, iRpc, iCh, dLastTime,
                      dLastPosY)
              << fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size();
  if (iCh == iNbCh) return kFALSE;
  if (0 == fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) return kFALSE;
  if (0 < fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size())
    fhNbDigiPerChan->Fill(fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size());
  if (1 < fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {
    Bool_t AddedHit = kFALSE;
    for (size_t i1 = 0; i1 < fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size() - 1; i1++) {
      if (AddedHit) break;
      size_t i2 = i1 + 1;
      while (!AddedHit && i2 < fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {
        LOG(debug1) << "check digi pair " << i1 << "," << i2 << " with size "
                    << fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size();

        if ((fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][i1])->GetSide()
            == (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][i2])->GetSide()) {
          i2++;
          continue;
        }  // endif same side
           // 2 Digis, both sides present
        CbmTofDigi* xDigiA = fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][i1];
        CbmTofDigi* xDigiB = fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][i2];
        Double_t dTime     = 0.5 * (xDigiA->GetTime() + xDigiB->GetTime());
        if (TMath::Abs(dTime - dLastTime) < fdMaxTimeDist) {
          CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
          iChId        = fTofId->SetDetectorInfo(xDetInfo);
          fChannelInfo = fDigiPar->GetCell(iChId);
          gGeoManager->FindNode(fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());

          Double_t dTimeDif = xDigiA->GetTime() - xDigiB->GetTime();
          Double_t dPosY    = 0.;
          if (1 == xDigiA->GetSide()) dPosY = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * dTimeDif * 0.5;
          else
            dPosY = -fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * dTimeDif * 0.5;

          if (TMath::Abs(dPosY - dLastPosY) < fdMaxSpaceDist) {  // append digi pair to current cluster

            Double_t dNClHits = (Double_t)(vDigiIndRef.size() / 2);
            Double_t dPosX    = ((Double_t)(-iNbCh / 2 + iCh) + 0.5) * fChannelInfo->GetSizex();
            Double_t dTotS    = xDigiA->GetTot() + xDigiB->GetTot();
            Double_t dNewTotS = (dLastTotS + dTotS);
            dLastPosX         = (dLastPosX * dLastTotS + dPosX * dTotS) / dNewTotS;
            dLastPosY         = (dLastPosY * dLastTotS + dPosY * dTotS) / dNewTotS;
            dLastTime         = (dLastTime * dLastTotS + dTime * dTotS) / dNewTotS;
            dLastTotS         = dNewTotS;
            // attach selected digis from pool
            Int_t Ind1 = fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][i1];
            Int_t Ind2 = fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][i2];
            vDigiIndRef.push_back(Ind1);
            vDigiIndRef.push_back(Ind2);
            // remove selected digis from pool
            fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin()
                                                               + i1);
            fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
              fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + i1);

            std::vector<int>::iterator it;
            it = find(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin(),
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].end(), Ind2);
            if (it != fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].end()) {
              auto ipos = it - fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin();
              LOG(debug1) << "Found i2 " << i2 << " with Ind2 " << Ind2 << " at position " << ipos;
              fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin()
                                                                 + ipos);
              fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + ipos);
            }
            else {
              LOG(fatal) << " Did not find  i2 " << i2 << " with Ind2 " << Ind2;
            }

            //if(iCh == iNbCh-1) break;  //Last strip reached
            if (iCh != (iNbCh - 1)
                && AddNextChan(iSmType, iSm, iRpc, iCh, dLastPosX, dLastPosY, dLastTime, dLastTotS)) {
              LOG(debug1) << "Added Strip " << iCh << " to cluster of size " << dNClHits;
              return kTRUE;  // signal hit was already added
            }
            AddedHit = kTRUE;
          }  //TMath::Abs(dPosY - dLastPosY) < fdMaxSpaceDist
        }    //TMath::Abs(dTime-dLastTime)<fdMaxTimeDist)
        i2++;
      }  //  while(i2 < fStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size()-1 )
    }    // end for i1
  }      // end if size
  Double_t hitpos_local[3] = {3 * 0.};
  hitpos_local[0]          = dLastPosX;
  hitpos_local[1]          = dLastPosY;
  hitpos_local[2]          = 0.;
  /*
	 if( 5 == iSmType || 8 == iSmType) { // for PAD counters
	 hitpos_local[0] = (gRandom->Rndm()-0.5)*fChannelInfo->GetSizex()*0.5;
	 hitpos_local[1] = (gRandom->Rndm()-0.5)*fChannelInfo->GetSizey()*0.5;
	 }
	 */
  Double_t hitpos[3] = {3 * 0.};
  if (5 != iSmType) {  // Diamond beam counter always at (0,0,0)
    /*TGeoNode*    cNode   = */ gGeoManager->GetCurrentNode();
    /*TGeoHMatrix* cMatrix = */ gGeoManager->GetCurrentMatrix();
    gGeoManager->LocalToMaster(hitpos_local, hitpos);
  }
  TVector3 hitPos(hitpos[0], hitpos[1], hitpos[2]);
  TVector3 hitPosErr(0.5, 0.5, 0.5);  // FIXME including positioning uncertainty
  Int_t iChm = floor(dLastPosX / fChannelInfo->GetSizex()) + iNbCh / 2;
  if (iChm < 0) iChm = 0;
  if (iChm > iNbCh - 1) iChm = iNbCh - 1;
  iDetId = CbmTofAddress::GetUniqueAddress(iSm, iRpc, iChm, 0, iSmType);

  Int_t iNbChanInHit = vDigiIndRef.size() / 2;

  TString cstr = "Save A-Hit ";
  cstr += Form(" %3d %3d 0x%08x %3d 0x%08x %8.2f %6.2f",  // %3d %3d
               fiNbHits, iNbChanInHit, iDetId, iLastChan,
               0,  //vPtsRef.size(),vPtsRef[0])
               dLastTime, dLastPosY);
  cstr += Form(", DigiSize: %lu ", vDigiIndRef.size());
  cstr += ", DigiInds: ";

  fviClusterMul[iSmType][iSm][iRpc]++;

  for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
    cstr += Form(" %d (M,%d)", vDigiIndRef.at(i), fviClusterMul[iSmType][iSm][iRpc]);
  }
  LOG(debug) << cstr;

  CbmTofHit* pHit = new CbmTofHit(iDetId, hitPos,
                                  hitPosErr,  //local detector coordinates
                                  fiNbHits,   // this number is used as reference!!
                                  dLastTime,
                                  vDigiIndRef.size(),  // number of linked digis =  2*CluSize
                                  //vPtsRef.size(), // flag  = number of TofPoints generating the cluster
                                  Int_t(dLastTotS * 10.));  //channel -> Tot
  // output hit
  new ((*fTofHitsColl)[fiNbHits]) CbmTofHit(*pHit);
  if (fdMemoryTime > 0.) {  // memorize hit
    LH_store(iSmType, iSm, iRpc, iChm, pHit);
  }
  else {
    pHit->Delete();
  }
  CbmMatch* digiMatch = new ((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();
  for (size_t i = 0; i < vDigiIndRef.size(); i++) {
    Double_t dTot = (fTofCalDigiVec->at(vDigiIndRef.at(i))).GetTot();
    digiMatch->AddLink(CbmLink(dTot, vDigiIndRef.at(i), fiOutputTreeEntry, fiFileIndex));
  }
  fiNbHits++;
  vDigiIndRef.clear();

  return kTRUE;
}

void CbmTofHitMaker::LH_store(Int_t iSmType, Int_t iSm, Int_t iRpc, Int_t iChm, CbmTofHit* pHit)
{

  if (fvLastHits[iSmType][iSm][iRpc][iChm].size() == 0) fvLastHits[iSmType][iSm][iRpc][iChm].push_back(pHit);
  else {
    Double_t dLastTime = pHit->GetTime();
    if (dLastTime >= fvLastHits[iSmType][iSm][iRpc][iChm].back()->GetTime()) {
      fvLastHits[iSmType][iSm][iRpc][iChm].push_back(pHit);
      LOG(debug) << Form(" Store LH from Ev  %8.0f for TSRC %d%d%d%d, size %lu, addr 0x%08x, "
                         "time %f, dt %f",
                         fdEvent, iSmType, iSm, iRpc, iChm, fvLastHits[iSmType][iSm][iRpc][iChm].size(),
                         pHit->GetAddress(), dLastTime,
                         dLastTime - fvLastHits[iSmType][iSm][iRpc][iChm].front()->GetTime());
    }
    else {
      if (dLastTime
          >= fvLastHits[iSmType][iSm][iRpc][iChm].front()->GetTime()) {  // hit has to be inserted in the proper place
        std::list<CbmTofHit*>::iterator it;
        for (it = fvLastHits[iSmType][iSm][iRpc][iChm].begin(); it != fvLastHits[iSmType][iSm][iRpc][iChm].end(); ++it)
          if ((*it)->GetTime() > dLastTime) break;
        fvLastHits[iSmType][iSm][iRpc][iChm].insert(--it, pHit);
        Double_t deltaTime = dLastTime - (*it)->GetTime();
        LOG(debug) << Form("Hit inserted into LH from Ev  %8.0f for TSRC "
                           "%d%d%d%d, size %lu, addr 0x%08x, delta time %f  ",
                           fdEvent, iSmType, iSm, iRpc, iChm, fvLastHits[iSmType][iSm][iRpc][iChm].size(),
                           pHit->GetAddress(), deltaTime);
      }
      else {  // this hit is first
        Double_t deltaTime = dLastTime - fvLastHits[iSmType][iSm][iRpc][iChm].front()->GetTime();
        LOG(debug) << Form("first LH from Ev  %8.0f for TSRC %d%d%d%d, size "
                           "%lu, addr 0x%08x, delta time %f ",
                           fdEvent, iSmType, iSm, iRpc, iChm, fvLastHits[iSmType][iSm][iRpc][iChm].size(),
                           pHit->GetAddress(), deltaTime);
        if (deltaTime == 0.) {
          // remove hit, otherwise double entry?
          pHit->Delete();
        }
        else {
          fvLastHits[iSmType][iSm][iRpc][iChm].push_front(pHit);
        }
      }
    }
  }
}

Bool_t CbmTofHitMaker::BuildHits()
{
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
  //vPtsRef.clear();
  vDigiIndRef.clear();
  // CbmTofCell *fTrafoCell=NULL;  (VF) not used
  // Int_t iTrafoCell=-1;  (VF) not used
  Int_t iNbChanInHit = 0;
  // Last Channel Temp variables
  Int_t iLastChan    = -1;
  Double_t dLastPosX = 0.0;  // -> Comment to remove warning because set but never used
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
          LOG(debug2) << "RPC - Loop  " << Form(" %3d %3d %3d %3d ", iSmType, iSm, iRpc, iChType);
          fviClusterMul[iSmType][iSm][iRpc] = 0;
          Int_t iChId                       = 0;
          Int_t iDetId                      = CbmTofAddress::GetUniqueAddress(iSm, iRpc, 0, 0, iSmType);
          ;
          Int_t iDetIndx = fDetIdIndexMap[iDetId];  // Detector Index
          if (0 == iChType) {
            // Don't spread clusters over RPCs!!!
            dWeightedTime = 0.0;
            dWeightedPosX = 0.0;
            dWeightedPosY = 0.0;
            dWeightedPosZ = 0.0;
            dWeightsSum   = 0.0;
            iNbChanInHit  = 0;
            //vPtsRef.clear();
            // For safety reinitialize everything
            iLastChan = -1;
            //                  dLastPosX = 0.0; // -> Comment to remove warning because set but never used
            dLastPosY = 0.0;
            dLastTime = 0.0;
            LOG(debug2) << "ChanOrient "
                        << Form(" %3d %3d %3d %3d %3d ", iSmType, iSm, iRpc, fDigiBdfPar->GetChanOrient(iSmType, iRpc),
                                iNbCh);

            if (1 == fDigiBdfPar->GetChanOrient(iSmType, iRpc)) {
              // Horizontal strips => X comes from left right time difference
            }       // if( 1 == fDigiBdfPar->GetChanOrient( iSmType, iRpc ) )
            else {  // Vertical strips => Y comes from bottom top time difference
              for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
                LOG(debug3) << "VDigisize "
                            << Form(" T %3d Sm %3d R %3d Ch %3d Size %3lu ", iSmType, iSm, iRpc, iCh,
                                    fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size());
                if (0 == fStorDigi[iSmType][iSm * iNbRpc + iRpc].size()) continue;
                if (fvDeadStrips[iDetIndx] & (1 << iCh)) continue;  // skip over dead channels
                if (0 < fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size())
                  fhNbDigiPerChan->Fill(fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size());

                while (1 < fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {

                  while ((fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0])->GetSide()
                         == (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])->GetSide()) {
                    // Not one Digi of each end!
                    fiNbSameSide++;
                    if (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size() > 2) {
                      LOG(debug) << "SameSide Digis! on TSRC " << iSmType << iSm << iRpc << iCh << ", Times: "
                                 << Form("%f", (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0])->GetTime()) << ", "
                                 << Form("%f", (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])->GetTime())
                                 << ", DeltaT "
                                 << (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])->GetTime()
                                      - (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0])->GetTime()
                                 << ", array size: " << fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size();
                      if (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][2]->GetSide()
                          == fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0]->GetSide()) {
                        LOG(debug) << "3 consecutive SameSide Digis! on TSRC " << iSmType << iSm << iRpc << iCh
                                   << ", Times: " << (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0])->GetTime()
                                   << ", " << (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])->GetTime()
                                   << ", DeltaT "
                                   << (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])->GetTime()
                                        - (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0])->GetTime()
                                   << ", array size: " << fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size();
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      }
                      else {
                        if (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][2]->GetTime()
                              - fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0]->GetTime()
                            > fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][2]->GetTime()
                                - fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1]->GetTime()) {
                          fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                        }
                        else {
                          LOG(debug) << Form("Ev %8.0f, digis not properly time ordered, TSRCS "
                                             "%d%d%d%d%d ",
                                             fdEvent, iSmType, iSm, iRpc, iCh,
                                             (Int_t) fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0]->GetSide());
                          fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + 1);
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + 1);
                        }
                      }
                    }
                    else {
                      LOG(debug2) << "SameSide Erase fStor entries(d) " << iSmType << ", SR " << iSm * iNbRpc + iRpc
                                  << ", Ch" << iCh;
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    }
                    if (2 > fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) break;
                    continue;
                  }  // same condition side end

                  LOG(debug2) << "digis processing for "
                              << Form(" SmT %3d Sm %3d Rpc %3d Ch %3d # %3lu ", iSmType, iSm, iRpc, iCh,
                                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size());
                  if (2 > fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {
                    LOG(debug) << Form("Leaving digi processing for TSRC %d%d%d%d, size  %3lu", iSmType, iSm, iRpc, iCh,
                                       fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size());
                    break;
                  }
                  /* Int_t iLastChId = iChId; // Save Last hit channel*/

                  // 2 Digis = both sides present
                  CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
                  iChId          = fTofId->SetDetectorInfo(xDetInfo);
                  Int_t iUCellId = CbmTofAddress::GetUniqueAddress(iSm, iRpc, iCh, 0, iSmType);
                  LOG(debug1) << Form(" TSRC %d%d%d%d size %3lu ", iSmType, iSm, iRpc, iCh,
                                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size())
                              << Form(" ChId: 0x%08x 0x%08x ", iChId, iUCellId);
                  fChannelInfo = fDigiPar->GetCell(iChId);

                  if (NULL == fChannelInfo) {
                    LOG(error) << "CbmTofHitMaker::BuildClusters: no "
                                  "geometry info! "
                               << Form(" %3d %3d %3d %3d 0x%08x 0x%08x ", iSmType, iSm, iRpc, iCh, iChId, iUCellId);
                    break;
                  }

                  TGeoNode* fNode =  // prepare local->global trafo
                    gGeoManager->FindNode(fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
                  LOG(debug2) << Form(" Node at (%6.1f,%6.1f,%6.1f) : %p", fChannelInfo->GetX(), fChannelInfo->GetY(),
                                      fChannelInfo->GetZ(), fNode);
                  //          fNode->Print();

                  CbmTofDigi* xDigiA = fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                  CbmTofDigi* xDigiB = fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1];

                  LOG(debug2) << "    " << xDigiA->ToString();
                  LOG(debug2) << "    " << xDigiB->ToString();

                  dTimeDif = (xDigiA->GetTime() - xDigiB->GetTime());
                  if (5 == iSmType && dTimeDif != 0.) {
                    // FIXME -> Overflow treatment in calib/tdc/TMbsCalibTdcTof.cxx
                    LOG(debug) << "CbmTofHitMaker::BuildClusters: "
                                  "Diamond hit in "
                               << iSm << " with inconsistent digits " << xDigiA->GetTime() << ", " << xDigiB->GetTime()
                               << " -> " << dTimeDif;
                    LOG(debug) << "    " << xDigiA->ToString();
                    LOG(debug) << "    " << xDigiB->ToString();
                  }
                  if (1 == xDigiA->GetSide())
                    // 0 is the top side, 1 is the bottom side
                    dPosY = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * dTimeDif * 0.5;
                  else
                    // 0 is the bottom side, 1 is the top side
                    dPosY = -fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * dTimeDif * 0.5;

                  while (TMath::Abs(dPosY) > fChannelInfo->GetSizey() * fPosYMaxScal
                         && fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size() > 2) {
                    LOG(debug) << "Hit candidate outside correlation window, check for "
                                  "better possible digis, "
                               << " mul " << fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size();

                    CbmTofDigi* xDigiC = fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][2];
                    Double_t dPosYN    = 0.;
                    Double_t dTimeDifN = 0;
                    if (xDigiC->GetSide() == xDigiA->GetSide()) dTimeDifN = xDigiC->GetTime() - xDigiB->GetTime();
                    else
                      dTimeDifN = xDigiA->GetTime() - xDigiC->GetTime();

                    if (1 == xDigiA->GetSide()) dPosYN = fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * dTimeDifN * 0.5;
                    else
                      dPosYN = -fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * dTimeDifN * 0.5;

                    if (TMath::Abs(dPosYN) < TMath::Abs(dPosY)) {
                      LOG(debug) << "Replace digi on side " << xDigiC->GetSide() << ", yPosNext " << dPosYN
                                 << " old: " << dPosY;
                      dTimeDif = dTimeDifN;
                      dPosY    = dPosYN;
                      if (xDigiC->GetSide() == xDigiA->GetSide()) {
                        xDigiA = xDigiC;
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      }
                      else {
                        xDigiB = xDigiC;
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          ++(fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + 1));
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          ++(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + 1));
                      }
                    }
                    else
                      break;
                  }  //while loop end

                  if (xDigiA->GetSide() == xDigiB->GetSide()) {
                    LOG(fatal) << "Wrong combinations of digis " << fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                               << "," << fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1];
                  }

                  if (TMath::Abs(dPosY) > fChannelInfo->GetSizey() * fPosYMaxScal) {  // remove both digis
                    fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    continue;
                  }
                  // The "Strip" time is the mean time between each end
                  dTime = 0.5 * (xDigiA->GetTime() + xDigiB->GetTime());

                  // Weight is the total charge => sum of both ends ToT
                  dTotS = xDigiA->GetTot() + xDigiB->GetTot();

                  // use local coordinates, (0,0,0) is in the center of counter  ?
                  dPosX = ((Double_t)(-iNbCh / 2 + iCh) + 0.5) * fChannelInfo->GetSizex();
                  dPosZ = 0.;

                  LOG(debug1) << "NbChanInHit  "
                              << Form(" %3d %3d %3d %3d %3d 0x%p %1.0f Time %f PosX %f "
                                      "PosY %f Svel %f ",
                                      iNbChanInHit, iSmType, iRpc, iCh, iLastChan, xDigiA, xDigiA->GetSide(), dTime,
                                      dPosX, dPosY, fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc))
                    //  << Form( ", Offs %f, %f ",fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][0],
                    //                            fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][1])
                    ;

                  // Now check if a hit/cluster is already started
                  if (0 < iNbChanInHit) {
                    if (iLastChan == iCh - 1) {
                      fhDigTimeDifClust->Fill(dTime - dLastTime);
                      fhDigSpacDifClust->Fill(dPosY - dLastPosY);
                      fhDigDistClust->Fill(dPosY - dLastPosY, dTime - dLastTime);
                    }
                    // if( iLastChan == iCh - 1 )
                    // a cluster is already started => check distance in space/time
                    // For simplicity, just check along strip direction for now
                    // and break cluster when a not fired strip is found
                    if (TMath::Abs(dTime - dLastTime) < fdMaxTimeDist && iLastChan == iCh - 1
                        && TMath::Abs(dPosY - dLastPosY) < fdMaxSpaceDist) {
                      // Add to cluster/hit
                      dWeightedTime += dTime * dTotS;
                      dWeightedPosX += dPosX * dTotS;
                      dWeightedPosY += dPosY * dTotS;
                      dWeightedPosZ += dPosZ * dTotS;
                      dWeightsSum += dTotS;
                      iNbChanInHit += 1;

                      vDigiIndRef.push_back((Int_t)(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]));
                      vDigiIndRef.push_back((Int_t)(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]));

                      LOG(debug1) << " Add Digi and erase fStor entries(a): NbChanInHit " << iNbChanInHit << ", "
                                  << iSmType << ", SR " << iSm * iNbRpc + iRpc << ", Ch" << iCh;

                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());

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
                      /*
											 if( 5 == iSmType || 8 == iSmType) { // for PAD counters
											 hitpos_local[0] = (gRandom->Rndm()-0.5)*fChannelInfo->GetSizex();
											 hitpos_local[1] = (gRandom->Rndm()-0.5)*fChannelInfo->GetSizey();
											 }
											 */
                      Double_t hitpos[3] = {3 * 0.};
                      if (5 != iSmType) {
                        /*TGeoNode*    cNode   =*/gGeoManager->GetCurrentNode();
                        /*TGeoHMatrix* cMatrix =*/gGeoManager->GetCurrentMatrix();
                        //cNode->Print();
                        //cMatrix->Print();

                        gGeoManager->LocalToMaster(hitpos_local, hitpos);
                      }
                      LOG(debug1) << Form(" LocalToMaster: (%6.1f,%6.1f,%6.1f) "
                                          "->(%6.1f,%6.1f,%6.1f)",
                                          hitpos_local[0], hitpos_local[1], hitpos_local[2], hitpos[0], hitpos[1],
                                          hitpos[2]);

                      TVector3 hitPos(hitpos[0], hitpos[1], hitpos[2]);

                      // Simple errors, not properly done at all for now
                      // Right way of doing it should take into account the weight distribution
                      // and real system time resolution
                      TVector3 hitPosErr(0.5, 0.5, 0.5);  // including positioning uncertainty
                      /*
											 TVector3 hitPosErr( fChannelInfo->GetSizex()/TMath::Sqrt(12.0),   // Single strips approximation
											 0.5, // Use generic value
											 1.);

											 */
                      //fDigiBdfPar->GetFeeTimeRes() * fDigiBdfPar->GetSigVel(iSmType,iRpc), // Use the electronics resolution
                      //fDigiBdfPar->GetNbGaps( iSmType, iRpc)*
                      //fDigiBdfPar->GetGapSize( iSmType, iRpc)/ //10.0 / // Change gap size in cm
                      //TMath::Sqrt(12.0) ); // Use full RPC thickness as "Channel" Z size
                      // Int_t iDetId = vPtsRef[0]->GetDetectorID();// detID = pt->GetDetectorID() <= from TofPoint
                      // calc mean ch from dPosX=((Double_t)(-iNbCh/2 + iCh)+0.5)*fChannelInfo->GetSizex();
                      Int_t iChm = floor(dWeightedPosX / fChannelInfo->GetSizex()) + iNbCh / 2;
                      if (iChm < 0) iChm = 0;
                      if (iChm > iNbCh - 1) iChm = iNbCh - 1;
                      iDetId       = CbmTofAddress::GetUniqueAddress(iSm, iRpc, iChm, 0, iSmType);
                      Int_t iRefId = 0;  // Index of the correspondng TofPoint
                      //		       if(NULL != fTofPointsColl) {
                      //iRefId = fTofPointsColl->IndexOf( vPtsRef[0] );
                      //}
                      TString sRef = "";
                      for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
                        sRef += Form(" %d, (M%d)", vDigiIndRef.at(i), fviClusterMul[iSmType][iSm][iRpc]);
                      }
                      LOG(debug) << "Save Hit  "
                                 << Form(" %3d %3d 0x%08x %3d %3d %3d %f %f", fiNbHits, iNbChanInHit, iDetId, iChm,
                                         iLastChan, iRefId, dWeightedTime, dWeightedPosY)
                                 << ", DigiSize: " << vDigiIndRef.size() << ", DigiInds: " << sRef;

                      fviClusterMul[iSmType][iSm][iRpc]++;

                      if (vDigiIndRef.size() < 2) {
                        LOG(warning) << "Digi refs for Hit " << fiNbHits << ":        vDigiIndRef.size()";
                      }
                      if (fiNbHits > 0) {
                        CbmTofHit* pHitL = (CbmTofHit*) fTofHitsColl->At(fiNbHits - 1);
                        if (iDetId == pHitL->GetAddress() && dWeightedTime == pHitL->GetTime()) {
                          LOG(debug) << "Store Hit twice? "
                                     << " fiNbHits " << fiNbHits << ", "
                                     << Form("0x%08x, MatchCollSize %d, IndRefSize %lu ", iDetId,
                                             fTofDigiMatchColl->GetEntriesFast(), vDigiIndRef.size());

                          for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
                            if (vDigiIndRef.at(i) < (Int_t) fTofCalDigiVec->size()) {
                              CbmTofDigi* pDigiC = &(fTofCalDigiVec->at(vDigiIndRef.at(i)));
                              LOG(debug) << " Digi " << i << " " << pDigiC->ToString();
                            }
                            else {
                              LOG(fatal) << "Insufficient CalDigiVec size for i " << i << ", Ind " << vDigiIndRef.at(i);
                            }
                          }

                          if (NULL == fTofDigiMatchColl) assert("No DigiMatchColl");
                          CbmMatch* digiMatchL = NULL;
                          if (fTofDigiMatchColl->GetEntriesFast() >= fiNbHits - 1) {
                            digiMatchL = (CbmMatch*) fTofDigiMatchColl->At(fiNbHits - 1);
                          }
                          else {
                            LOG(fatal) << "DigiMatchColl has insufficient size " << fTofDigiMatchColl->GetEntriesFast();
                          }

                          if (NULL != digiMatchL)
                            for (Int_t i = 0; i < digiMatchL->GetNofLinks(); i++) {
                              CbmLink L0 = digiMatchL->GetLink(i);
                              LOG(debug) << "report link " << i << "(" << digiMatchL->GetNofLinks() << "), ind "
                                         << L0.GetIndex();
                              Int_t iDigIndL = L0.GetIndex();
                              if (iDigIndL >= (Int_t) vDigiIndRef.size()) {
                                if (iDetId != fiBeamRefAddr) {
                                  LOG(warn) << Form("Invalid DigiRefInd for det 0x%08x", iDetId);
                                  continue;
                                }
                              }
                              if (vDigiIndRef.at(iDigIndL) >= (Int_t) fTofCalDigiVec->size()) {
                                LOG(warn) << "Invalid CalDigiInd";
                                continue;
                              }
                              CbmTofDigi* pDigiC = &(fTofCalDigiVec->at(vDigiIndRef.at(iDigIndL)));
                              LOG(debug) << " DigiL " << pDigiC->ToString();
                            }
                          else {
                            LOG(warn) << "Invalid digMatch Link at Index " << fiNbHits - 1;
                          }
                        }
                        LOG(debug) << "Current HitsColl length " << fTofHitsColl->GetEntriesFast();
                      }
                      CbmTofHit* pHit =
                        new CbmTofHit(iDetId, hitPos,
                                      hitPosErr,  //local detector coordinates
                                      fiNbHits,   // this number is used as reference!!
                                      dWeightedTime,
                                      vDigiIndRef.size(),  // number of linked digis =  2*CluSize
                                      //vPtsRef.size(), // flag  = number of TofPoints generating the cluster
                                      Int_t(dWeightsSum * 10.));  //channel -> Tot
                      //0) ; //channel
                      // output hit
                      new ((*fTofHitsColl)[fiNbHits]) CbmTofHit(*pHit);
                      // memorize hit
                      if (fdMemoryTime > 0.) { LH_store(iSmType, iSm, iRpc, iChm, pHit); }
                      else {
                        pHit->Delete();
                      }
                      /*
											 new((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();
											 CbmMatch* digiMatch = (CbmMatch *)fTofDigiMatchColl->At(fiNbHits);
											 */
                      CbmMatch* digiMatch = new ((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();
                      for (size_t i = 0; i < vDigiIndRef.size(); i++) {
                        Double_t dTot = (fTofCalDigiVec->at(vDigiIndRef.at(i))).GetTot();
                        digiMatch->AddLink(CbmLink(dTot, vDigiIndRef.at(i), fiOutputTreeEntry, fiFileIndex));
                      }

                      fiNbHits++;
                      // For Histogramming
                      fviClusterSize[iSmType][iRpc].push_back(iNbChanInHit);
                      //fviTrkMul[iSmType][iRpc].push_back( vPtsRef.size() );
                      fvdX[iSmType][iRpc].push_back(dWeightedPosX);
                      fvdY[iSmType][iRpc].push_back(dWeightedPosY);
                      /*  no TofPoint available for data!
											 fvdDifX[iSmType][iRpc].push_back( vPtsRef[0]->GetX() - dWeightedPosX);
											 fvdDifY[iSmType][iRpc].push_back( vPtsRef[0]->GetY() - dWeightedPosY);
											 fvdDifCh[iSmType][iRpc].push_back( fGeoHandler->GetCell( vPtsRef[0]->GetDetectorID() ) -1 -iLastChan );
											 */
                      //vPtsRef.clear();
                      vDigiIndRef.clear();

                      // Start a new hit
                      dWeightedTime = dTime * dTotS;
                      dWeightedPosX = dPosX * dTotS;
                      dWeightedPosY = dPosY * dTotS;
                      dWeightedPosZ = dPosZ * dTotS;
                      dWeightsSum   = dTotS;
                      iNbChanInHit  = 1;
                      // Save pointer on CbmTofPoint
                      // vPtsRef.push_back( (CbmTofPoint*)(xDigiA->GetLinks()) );
                      // Save next digi address
                      LOG(debug2) << " Next fStor Digi " << iSmType << ", SR " << iSm * iNbRpc + iRpc << ", Ch" << iCh
                                  << ", Dig0 " << (fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]) << ", Dig1 "
                                  << (fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]);

                      vDigiIndRef.push_back((Int_t)(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]));
                      vDigiIndRef.push_back((Int_t)(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]));
                      LOG(debug2) << " Erase fStor entries(b) " << iSmType << ", SR " << iSm * iNbRpc + iRpc << ", Ch"
                                  << iCh;
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());

                      if (kTRUE == fDigiBdfPar->ClustUseTrackId()) {
                        //    if( ((CbmTofPoint*)(xDigiA->GetLinks()))->GetTrackID() !=
                        //        ((CbmTofPoint*)(xDigiB->GetLinks()))->GetTrackID() )
                        // if other side come from a different Track,
                        // also save the pointer on CbmTofPoint
                        //  vPtsRef.push_back( (CbmTofPoint*)(xDigiB->GetLinks()) );
                      }  // if( kTRUE == fDigiBdfPar->ClustUseTrackId() )
                         //else if( xDigiA->GetLinks() != xDigiB->GetLinks() )
                         // if other side come from a different TofPoint,
                         // also save the pointer on CbmTofPoint
                      //    vPtsRef.push_back( (CbmTofPoint*)(xDigiB->GetLinks()) );
                    }  // else of if current Digis compatible with last fired chan
                  }    // if( 0 < iNbChanInHit)
                  else {
                    LOG(debug) << Form("1.Hit on TSRC %d%d%d%d, time: %f, PosY %f, Tdif %f ", iSmType, iSm, iRpc, iCh,
                                       dTime, dPosY, dTimeDif);

                    // first fired strip in this RPC
                    dWeightedTime = dTime * dTotS;
                    dWeightedPosX = dPosX * dTotS;
                    dWeightedPosY = dPosY * dTotS;
                    dWeightedPosZ = dPosZ * dTotS;
                    dWeightsSum   = dTotS;
                    iNbChanInHit  = 1;
                    // Save pointer on CbmTofPoint
                    //if(NULL != fTofPointsColl)
                    //                                    vPtsRef.push_back( (CbmTofPoint*)(xDigiA->GetLinks()) );
                    vDigiIndRef.push_back((Int_t)(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]));
                    vDigiIndRef.push_back((Int_t)(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]));

                    LOG(debug2) << " Erase fStor entries(c) " << iSmType << ", SR " << iSm * iNbRpc + iRpc << ", Ch"
                                << iCh;
                    fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin());

                    if (kTRUE == fDigiBdfPar->ClustUseTrackId()) {
                      // if( ((CbmTofPoint*)(xDigiA->GetLinks()))->GetTrackID() !=
                      //     ((CbmTofPoint*)(xDigiB->GetLinks()))->GetTrackID() )
                      // if other side come from a different Track,
                      // also save the pointer on CbmTofPoint
                      // vPtsRef.push_back( (CbmTofPoint*)(xDigiB->GetLinks()) );
                    }  // if( kTRUE == fDigiBdfPar->ClustUseTrackId() )
                       // else if( xDigiA->GetLinks() != xDigiB->GetLinks() )
                       // if other side come from a different TofPoint,
                       // also save the pointer on CbmTofPoint
                    //   vPtsRef.push_back( (CbmTofPoint*)(xDigiB->GetLinks()) );
                  }  // else of if( 0 < iNbChanInHit)
                  iLastChan = iCh;
                  dLastPosX = dPosX;
                  dLastPosY = dPosY;
                  dLastTime = dTime;
                  if (AddNextChan(iSmType, iSm, iRpc, iLastChan, dLastPosX, dLastPosY, dLastTime, dWeightsSum)) {
                    iNbChanInHit = 0;  // cluster already stored
                  }
                }  // while( 1 < fStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size() )
                fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].clear();
                fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].clear();
              }  // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
              LOG(debug2) << "finished V-RPC"
                          << Form(" %3d %3d %3d %d %f %fx", iSmType, iSm, iRpc, fTofHitsColl->GetEntriesFast(),
                                  dLastPosX, dLastPosY);
            }  // else of if( 1 == fDigiBdfPar->GetChanOrient( iSmType, iRpc ) )
          }    // if( 0 == iChType)
          else {
            LOG(error) << "=> Cluster building "
                       << "from digis to hits not implemented for pads, Sm type " << iSmType << " Rpc " << iRpc;
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
              /*
							 if( 5 == iSmType || 8 == iSmType) { // for PAD counters
							 hitpos_local[0] = (gRandom->Rndm()-0.5)*fChannelInfo->GetSizex();
							 hitpos_local[1] = (gRandom->Rndm()-0.5)*fChannelInfo->GetSizey();
							 }
							 */
              Double_t hitpos[3] = {3 * 0.};
              if (5 != iSmType) {
                /*TGeoNode*       cNode=*/gGeoManager->GetCurrentNode();
                /*TGeoHMatrix* cMatrix =*/gGeoManager->GetCurrentMatrix();
                //cNode->Print();
                //cMatrix->Print();
                gGeoManager->LocalToMaster(hitpos_local, hitpos);
              }
              LOG(debug1) << Form(" LocalToMaster for V-node: "
                                  "(%6.1f,%6.1f,%6.1f) ->(%6.1f,%6.1f,%6.1f)",
                                  hitpos_local[0], hitpos_local[1], hitpos_local[2], hitpos[0], hitpos[1], hitpos[2]);

              TVector3 hitPos(hitpos[0], hitpos[1], hitpos[2]);
              // Event errors, not properly done at all for now
              // Right way of doing it should take into account the weight distribution
              // and real system time resolution
              TVector3 hitPosErr(0.5, 0.5, 0.5);  // including positioning uncertainty
              /*
							 TVector3 hitPosErr( fChannelInfo->GetSizex()/TMath::Sqrt(12.0),   // Single strips approximation
							 0.5, // Use generic value
							 1.);
							 */
              //                fDigiBdfPar->GetFeeTimeRes() * fDigiBdfPar->GetSigVel(iSmType,iRpc), // Use the electronics resolution
              //                fDigiBdfPar->GetNbGaps( iSmType, iRpc)*
              //                fDigiBdfPar->GetGapSize( iSmType, iRpc)/10.0 / // Change gap size in cm
              //                TMath::Sqrt(12.0) ); // Use full RPC thickness as "Channel" Z size
              //                     cout<<"a "<<vPtsRef.size()<<endl;
              //                     cout<<"b "<<vPtsRef[0]<<endl;
              //                     cout<<"c "<<vPtsRef[0]->GetDetectorID()<<endl;
              //                     Int_t iDetId = vPtsRef[0]->GetDetectorID();// detID = pt->GetDetectorID() <= from TofPoint
              //                     Int_t iDetId = iChId;
              Int_t iChm = floor(dWeightedPosX / fChannelInfo->GetSizex()) + iNbCh / 2;
              if (iChm < 0) iChm = 0;
              if (iChm > iNbCh - 1) iChm = iNbCh - 1;
              iDetId       = CbmTofAddress::GetUniqueAddress(iSm, iRpc, iChm, 0, iSmType);
              Int_t iRefId = 0;  // Index of the correspondng TofPoint
              //if(NULL != fTofPointsColl) iRefId = fTofPointsColl->IndexOf( vPtsRef[0] );
              TString cstr = "Save V-Hit  ";
              cstr += Form(" %3d %3d 0x%08x %3d 0x%08x %8.2f %6.2f",  // %3d %3d
                           fiNbHits, iNbChanInHit, iDetId, iLastChan,
                           iRefId,  //vPtsRef.size(),vPtsRef[0])
                           dWeightedTime, dWeightedPosY);

              cstr += Form(", DigiSize: %lu ", vDigiIndRef.size());
              cstr += ", DigiInds: ";

              fviClusterMul[iSmType][iSm][iRpc]++;

              for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
                cstr += Form(" %d (M,%d)", vDigiIndRef.at(i), fviClusterMul[iSmType][iSm][iRpc]);
              }
              LOG(debug) << cstr;

              if (vDigiIndRef.size() < 2) {
                LOG(warning) << "Digi refs for Hit " << fiNbHits << ":        vDigiIndRef.size()";
              }
              if (fiNbHits > 0) {
                CbmTofHit* pHitL = (CbmTofHit*) fTofHitsColl->At(fiNbHits - 1);
                if (iDetId == pHitL->GetAddress() && dWeightedTime == pHitL->GetTime())
                  LOG(debug) << "Store Hit twice? "
                             << " fiNbHits " << fiNbHits << ", " << Form("0x%08x", iDetId);
              }

              CbmTofHit* pHit = new CbmTofHit(iDetId, hitPos,
                                              hitPosErr,  //local detector coordinates
                                              fiNbHits,   // this number is used as reference!!
                                              dWeightedTime,
                                              vDigiIndRef.size(),  // number of linked digis =  2*CluSize
                                              //vPtsRef.size(), // flag  = number of TofPoints generating the cluster
                                              Int_t(dWeightsSum * 10.));  //channel -> Tot
              //                0) ; //channel
              //                vDigiIndRef);
              // output hit
              new ((*fTofHitsColl)[fiNbHits]) CbmTofHit(*pHit);
              // memorize hit
              if (fdMemoryTime > 0.) { LH_store(iSmType, iSm, iRpc, iChm, pHit); }
              else {
                pHit->Delete();
              }
              /*
							 new((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();
							 CbmMatch* digiMatch = (CbmMatch *)fTofDigiMatchColl->At(fiNbHits);
							 */
              CbmMatch* digiMatch = new ((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();

              for (size_t i = 0; i < vDigiIndRef.size(); i++) {
                Double_t dTot = fTofCalDigiVec->at(vDigiIndRef.at(i)).GetTot();
                digiMatch->AddLink(CbmLink(dTot, vDigiIndRef.at(i), fiOutputTreeEntry, fiFileIndex));
              }

              fiNbHits++;
              // For Histogramming
              fviClusterSize[iSmType][iRpc].push_back(iNbChanInHit);
              //fviTrkMul[iSmType][iRpc].push_back( vPtsRef.size() );
              fvdX[iSmType][iRpc].push_back(dWeightedPosX);
              fvdY[iSmType][iRpc].push_back(dWeightedPosY);
              /*
							 fvdDifX[iSmType][iRpc].push_back( vPtsRef[0]->GetX() - dWeightedPosX);
							 fvdDifY[iSmType][iRpc].push_back( vPtsRef[0]->GetY() - dWeightedPosY);
							 fvdDifCh[iSmType][iRpc].push_back( fGeoHandler->GetCell( vPtsRef[0]->GetDetectorID() ) -1 -iLastChan );
							 */
              //vPtsRef.clear();
              vDigiIndRef.clear();
            }  // else of if( 1 == fDigiBdfPar->GetChanOrient( iSmType, iRpc ) )
          }    // if( 0 < iNbChanInHit)
          LOG(debug2) << " Fini-A " << Form(" %3d %3d %3d M%3d", iSmType, iSm, iRpc, fviClusterMul[iSmType][iSm][iRpc]);
        }  // for each sm/rpc pair
      LOG(debug2) << " Fini-B " << Form(" %3d ", iSmType);
    }  // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
  }
  return kTRUE;
}

Bool_t CbmTofHitMaker::CalibRawDigis()
{
  CbmTofDigi* pDigi;
  CbmTofDigi* pCalDigi = NULL;
  Int_t iDigIndCal     = -1;
  // channel deadtime map
  std::map<Int_t, Double_t> mChannelDeadTime;

  Int_t iNbTofDigi = fTofDigiVec.size();
  //Int_t iNbTofDigi = fTofDigisColl->GetEntriesFast();
  for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
    pDigi = &(fTofDigiVec.at(iDigInd));
    //pDigi = (CbmTofDigi*) fTofDigisColl->At( iDigInd );

    if (fbSwapChannelSides && 5 != pDigi->GetType() && 8 != pDigi->GetType()) {
      pDigi->SetAddress(pDigi->GetSm(), pDigi->GetRpc(), pDigi->GetChannel(), (0 == pDigi->GetSide()) ? 1 : 0,
                        pDigi->GetType());
    }

    Int_t iAddr = pDigi->GetAddress();

    LOG(debug1) << "BC "  // Before Calibration
                << Form("0x%08x", pDigi->GetAddress()) << " TSRC " << pDigi->GetType() << pDigi->GetSm()
                << pDigi->GetRpc() << Form("%2d", (Int_t) pDigi->GetChannel()) << " " << pDigi->GetSide() << " "
                << Form("%f", pDigi->GetTime()) << " " << pDigi->GetTot();
    /*
		 if (pDigi->GetType() == 5
		 || pDigi->GetType()
		 == 8)  // for Pad counters generate fake digi to mockup a strip
		 if (pDigi->GetSide() == 1)
		 continue;  // skip one side to avoid double entries
		 */
    Bool_t bValid = kTRUE;
    std::map<Int_t, Double_t>::iterator it;
    it = mChannelDeadTime.find(iAddr);
    if (it != mChannelDeadTime.end()) {
      LOG(debug1) << "CCT found valid ChannelDeadtime entry " << mChannelDeadTime[iAddr] << ", DeltaT "
                  << pDigi->GetTime() - mChannelDeadTime[iAddr];
      if ((bValid = (pDigi->GetTime() > mChannelDeadTime[iAddr] + fdChannelDeadtime))) {

        //       pCalDigi = new((*fTofCalDigisColl)[++iDigIndCal]) CbmTofDigi( *pDigi );
        fTofCalDigiVec->push_back(CbmTofDigi(*pDigi));
        pCalDigi = &(fTofCalDigiVec->back());
        iDigIndCal++;
      }
    }
    else {
      fTofCalDigiVec->push_back(CbmTofDigi(*pDigi));
      pCalDigi = &(fTofCalDigiVec->back());
      iDigIndCal++;
      // pCalDigi = new((*fTofCalDigisColl)[++iDigIndCal]) CbmTofDigi( *pDigi );
    }
    mChannelDeadTime[iAddr] = pDigi->GetTime();
    if (!bValid) continue;

    LOG(debug1) << "DC "  // After deadtime check. before Calibration
                << Form("0x%08x", pDigi->GetAddress()) << " TSRC " << pDigi->GetType() << pDigi->GetSm()
                << pDigi->GetRpc() << Form("%2d", (Int_t) pDigi->GetChannel()) << " " << pDigi->GetSide() << " "
                << Form("%f", pDigi->GetTime()) << " " << pDigi->GetTot();

    if (fbPs2Ns) {
      pCalDigi->SetTime(pCalDigi->GetTime() / 1000.);  // for backward compatibility
      pCalDigi->SetTot(pCalDigi->GetTot() / 1000.);    // for backward compatibility
    }

    if (fDigiBdfPar->GetNbSmTypes() > pDigi->GetType()  // prevent crash due to misconfiguration
        && fDigiBdfPar->GetNbSm(pDigi->GetType()) > pDigi->GetSm()
        && fDigiBdfPar->GetNbRpc(pDigi->GetType()) > pDigi->GetRpc()
        && fDigiBdfPar->GetNbChan(pDigi->GetType(), pDigi->GetRpc()) > pDigi->GetChannel()) {

      LOG(debug2) << " CluCal-Init: " << pDigi->ToString();
      // apply calibration vectors
      pCalDigi->SetTime(pCalDigi->GetTime() -  // calibrate Digi Time
                        fvCPTOff[pDigi->GetType()][pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                                                   + pDigi->GetRpc()][pDigi->GetChannel()][pDigi->GetSide()]);
      LOG(debug2) << " CluCal-TOff: " << pCalDigi->ToString();

      Double_t dTot = pCalDigi->GetTot() + fRndm->Uniform(0, 1) -  // subtract Offset
                      fvCPTotOff[pDigi->GetType()][pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                                                   + pDigi->GetRpc()][pDigi->GetChannel()][pDigi->GetSide()];
      if (dTot < 0.001) dTot = 0.001;
      pCalDigi->SetTot(dTot *  // calibrate Digi ToT
                       fvCPTotGain[pDigi->GetType()][pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                                                     + pDigi->GetRpc()][pDigi->GetChannel()][pDigi->GetSide()]);

      // walk correction
      Double_t dTotBinSize = (fdTOTMax - fdTOTMin) / nbClWalkBinX;
      Int_t iWx            = (Int_t)((pCalDigi->GetTot() - fdTOTMin) / dTotBinSize);
      if (0 > iWx) iWx = 0;
      if (iWx >= nbClWalkBinX) iWx = nbClWalkBinX - 1;
      Double_t dDTot = (pCalDigi->GetTot() - fdTOTMin) / dTotBinSize - (Double_t) iWx - 0.5;
      Double_t dWT =
        fvCPWalk[pCalDigi->GetType()][pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                      + pCalDigi->GetRpc()][pCalDigi->GetChannel()][pCalDigi->GetSide()][iWx];
      if (dDTot > 0) {                 // linear interpolation to next bin
        if (iWx < nbClWalkBinX - 1) {  // linear interpolation to next bin

          dWT +=
            dDTot
            * (fvCPWalk[pCalDigi->GetType()][pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                             + pCalDigi->GetRpc()][pCalDigi->GetChannel()][pCalDigi->GetSide()][iWx + 1]
               - fvCPWalk[pCalDigi->GetType()]
                         [pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType()) + pCalDigi->GetRpc()]
                         [pCalDigi->GetChannel()][pCalDigi->GetSide()][iWx]);  //memory leak???
        }
      }
      else  // dDTot < 0,  linear interpolation to next bin
      {
        if (0 < iWx) {  // linear interpolation to next bin
          dWT -=
            dDTot
            * (fvCPWalk[pCalDigi->GetType()][pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                             + pCalDigi->GetRpc()][pCalDigi->GetChannel()][pCalDigi->GetSide()][iWx - 1]
               - fvCPWalk[pCalDigi->GetType()]
                         [pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType()) + pCalDigi->GetRpc()]
                         [pCalDigi->GetChannel()][pCalDigi->GetSide()][iWx]);  //memory leak???
        }
      }

      pCalDigi->SetTime(pCalDigi->GetTime() - dWT);  // calibrate Digi Time
      LOG(debug2) << " CluCal-Walk: " << pCalDigi->ToString();

      if (1) {  //pDigi->GetType()==7 && pDigi->GetSm()==0){
        LOG(debug)
          << "BuildClusters: CalDigi " << Form("%02d TSRCS  ", iDigIndCal) << pCalDigi->GetType() << pCalDigi->GetSm()
          << pCalDigi->GetRpc() << Form("%02d", Int_t(pCalDigi->GetChannel())) << pCalDigi->GetSide()
          << Form(", T %15.3f", pCalDigi->GetTime()) << ", Tot " << pCalDigi->GetTot() << ", TotGain "
          << fvCPTotGain[pCalDigi->GetType()][pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                              + pCalDigi->GetRpc()][pCalDigi->GetChannel()][pCalDigi->GetSide()]
          << ", TotOff "
          << fvCPTotOff[pCalDigi->GetType()][pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                             + pCalDigi->GetRpc()][pCalDigi->GetChannel()][pCalDigi->GetSide()]
          << ", Walk " << iWx << ": "
          << fvCPWalk[pCalDigi->GetType()][pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                           + pCalDigi->GetRpc()][pCalDigi->GetChannel()][pCalDigi->GetSide()][iWx];

        LOG(debug1)
          << " dDTot " << dDTot << " BinSize: " << dTotBinSize << ", CPWalk "
          << fvCPWalk[pCalDigi->GetType()][pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                           + pCalDigi->GetRpc()][pCalDigi->GetChannel()][pCalDigi->GetSide()][iWx - 1]
          << ", "
          << fvCPWalk[pCalDigi->GetType()][pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                           + pCalDigi->GetRpc()][pCalDigi->GetChannel()][pCalDigi->GetSide()][iWx]
          << ", "
          << fvCPWalk[pCalDigi->GetType()][pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
                                           + pCalDigi->GetRpc()][pCalDigi->GetChannel()][pCalDigi->GetSide()][iWx + 1]
          << " -> dWT = " << dWT;
      }
    }
    else {
      LOG(info) << "Skip1 Digi "
                << " Type " << pDigi->GetType() << " " << fDigiBdfPar->GetNbSmTypes() << " Sm " << pDigi->GetSm() << " "
                << fDigiBdfPar->GetNbSm(pDigi->GetType()) << " Rpc " << pDigi->GetRpc() << " "
                << fDigiBdfPar->GetNbRpc(pDigi->GetType()) << " Ch " << pDigi->GetChannel() << " "
                << fDigiBdfPar->GetNbChan(pDigi->GetType(), 0);
    }

    if (0)  // (bAddBeamCounterSideDigi)
      if (pCalDigi->GetType() == 5
          || pCalDigi->GetType() == 8) {  // for Pad counters generate fake digi to mockup a strip
        LOG(debug) << "add Pad counter 2. Side digi for 0x" << std::hex << pCalDigi->GetAddress();
        fTofCalDigiVec->push_back(CbmTofDigi(*pCalDigi));
        CbmTofDigi* pCalDigi2 = &(fTofCalDigiVec->back());
        iDigIndCal++;
        //      CbmTofDigi *pCalDigi2 = new((*fTofCalDigisColl)[++iDigIndCal]) CbmTofDigi( *pCalDigi );
        if (pCalDigi->GetSide() == 0)
          pCalDigi2->SetAddress(pCalDigi->GetSm(), pCalDigi->GetRpc(), pCalDigi->GetChannel(), 1, pCalDigi->GetType());
        else
          pCalDigi2->SetAddress(pCalDigi->GetSm(), pCalDigi->GetRpc(), pCalDigi->GetChannel(), 0, pCalDigi->GetType());
      }

  }  // for( Int_t iDigInd = 0; iDigInd < nTofDigi; iDigInd++ )

  //  iNbTofDigi = fTofCalDigisColl->GetEntriesFast();  // update because of added duplicted digis
  iNbTofDigi = fTofCalDigiVec->size();  // update because of added duplicted digis
  //if(fTofCalDigisColl->IsSortable())
  //    LOG(debug)<<"CbmTofHitMaker::BuildClusters: Sort "<<fTofCalDigisColl->GetEntriesFast()<<" calibrated digis ";
  LOG(debug) << "CbmTofHitMaker::BuildClusters: Sort " << fTofCalDigiVec->size() << " calibrated digis ";
  if (iNbTofDigi > 1) {
    //    fTofCalDigisColl->Sort(iNbTofDigi); // Time order again, in case modified by the calibration
    /// Sort the buffers of hits due to the time offsets applied
    std::sort(fTofCalDigiVec->begin(), fTofCalDigiVec->end(),
              [](const CbmTofDigi& a, const CbmTofDigi& b) -> bool { return a.GetTime() < b.GetTime(); });
    //    std::sort(fTofCalDigiVec->begin(), fTofCalDigiVec->end());
    //    if(!fTofCalDigisColl->IsSorted()){
    //    if ( ! std::is_sorted(fTofCalDigiVec->begin(), fTofCalDigiVec->end()))
    if (!std::is_sorted(fTofCalDigiVec->begin(), fTofCalDigiVec->end(),
                        [](const CbmTofDigi& a, const CbmTofDigi& b) -> bool { return a.GetTime() < b.GetTime(); }))
      LOG(warning) << "CbmTofHitMaker::BuildClusters: Sorting not successful ";
  }
  //  }

  return kTRUE;
}

void CbmTofHitMaker::SetDeadStrips(Int_t iDet, Int_t ival)
{
  if (fvDeadStrips.size() < static_cast<size_t>(iDet + 1)) fvDeadStrips.resize(iDet + 1);
  fvDeadStrips[iDet] = ival;
}
