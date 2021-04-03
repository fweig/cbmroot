/** @file CbmTofEventClusterizer.cxx
 ** @author nh
 ** @date 13.07.2018
 ** adopted from
 ** @file CbmTofTestBeamClusterizer.cxx
 ** @file CbmTofSimpClusterizer.cxx
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 23.08.2013
 **/

#include "CbmTofEventClusterizer.h"

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
#include "FairRootFileSink.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

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

static Int_t iIndexDut            = 0;
static Double_t StartAnalysisTime = 0.;
// const  Double_t cLight=29.9792; // in cm/ns  (VF) not used
static Int_t SelMask          = DetMask;
static Double_t fdStartAna10s = 0.;
static Double_t dTLEvt        = 0.;
static Int_t iNSpill          = 0;
static Int_t iNbTs            = 0;

const Double_t fdSpillDuration = 2.;   // in seconds
const Double_t fdSpillBreak    = 0.9;  // in seconds

static Bool_t bAddBeamCounterSideDigi = kTRUE;
static TRandom3* fRndm                = new TRandom3();

//   std::vector< CbmTofPoint* > vPtsRef;

CbmTofEventClusterizer* CbmTofEventClusterizer::fInstance = 0;

/************************************************************************************/
CbmTofEventClusterizer::CbmTofEventClusterizer() : CbmTofEventClusterizer("TestbeamClusterizer", 0, 0)
{
  // if ( !fInstance ) fInstance = this;
}

CbmTofEventClusterizer::CbmTofEventClusterizer(const char* name, Int_t verbose, Bool_t writeDataInOut)
  : FairTask(TString(name), verbose)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fDigiPar(NULL)
  , fChannelInfo(NULL)
  , fDigiBdfPar(NULL)
  , fTrbHeader(NULL)
  , fTofDigiPointMatches(NULL)
  , fDigiMan(nullptr)
  , fEventsColl(nullptr)
  , fbWriteHitsInOut(writeDataInOut)
  , fbWriteDigisInOut(writeDataInOut)
  , fTofHitsColl(NULL)
  , fTofDigiMatchColl(NULL)
  , fTofHitsCollOut(NULL)
  , fTofDigiMatchCollOut(NULL)
  , fTofDigiPointMatchesOut(nullptr)
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
  , fDutId(-1)
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

CbmTofEventClusterizer::~CbmTofEventClusterizer()
{
  if (fGeoHandler) delete fGeoHandler;
  if (fInstance == this) fInstance = 0;
  //   DeleteHistos(); // <-- if needed  ?
}

/************************************************************************************/
// FairTasks inherited functions
InitStatus CbmTofEventClusterizer::Init()
{
  LOG(info) << "CbmTofEventClusterizer initializing... expect Digis in ns units! ";

  if (kFALSE == RegisterInputs()) return kFATAL;

  if (kFALSE == RegisterOutputs()) return kFATAL;

  if (kFALSE == InitParameters()) return kFATAL;

  if (kFALSE == LoadGeometry()) return kFATAL;

  if (kFALSE == InitCalibParameter()) return kFATAL;

  if (kFALSE == CreateHistos()) return kFATAL;
  switch (fIdMode) {
    case 0:
      fDutAddr = CbmTofAddress::GetUniqueAddress(fDutSm, fDutRpc, 0, 0, fDutId);
      fSelAddr = CbmTofAddress::GetUniqueAddress(fSelSm, fSelRpc, 0, 0, fSelId);
      if (fSel2Id > -1) fSel2Addr = CbmTofAddress::GetUniqueAddress(fSel2Sm, fSel2Rpc, 0, 0, fSel2Id);
      fiBeamRefAddr = CbmTofAddress::GetUniqueAddress(fiBeamRefSm, fiBeamRefDet, 0, 0, fiBeamRefType);
      break;
    case 1:
      SelMask  = ModMask;
      fDutAddr = CbmTofAddress::GetUniqueAddress(fDutSm, 0, 0, 0, fDutId);
      fSelAddr = CbmTofAddress::GetUniqueAddress(fSelSm, 0, 0, 0, fSelId);
      if (fSel2Id > -1) fSel2Addr = CbmTofAddress::GetUniqueAddress(fSel2Sm, 0, 0, 0, fSel2Id);
      fiBeamRefAddr = CbmTofAddress::GetUniqueAddress(fiBeamRefSm, 0, 0, 0, fiBeamRefType);
      break;
  }
  iIndexDut = fDigiBdfPar->GetDetInd(fDutAddr);

  return kSUCCESS;
}

void CbmTofEventClusterizer::SetParContainers()
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

void CbmTofEventClusterizer::Exec(Option_t* option)
{
  if (nullptr != fTofDigiPointMatchesOut) fTofDigiPointMatchesOut->clear();

  if (fTofCalDigiVecOut) fTofCalDigiVecOut->clear();
  if (fEventsColl) {
    LOG(info) << "CbmTofEventClusterizer::Exec => New timeslice " << iNbTs << " with " << fEventsColl->GetEntriesFast()
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

      Int_t iDigi0 = iNbCalDigis;  //starting index of current event
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

        TString cstr = Form("Modify for hit %d, (%d", index, iNbHits);
        cstr += Form(") %d links by iDigi0 %d: ", (Int_t) pDigiMatch->GetNofLinks(), iDigi0);

        // update content of match object to TS array
        CbmMatch* mDigiMatch = new CbmMatch();
        for (Int_t iLink = 0; iLink < pDigiMatch->GetNofLinks(); iLink++) {
          CbmLink Link = pDigiMatch->GetLink(iLink);
          Link.SetIndex(Link.GetIndex() + iDigi0);
          cstr += Form(" %d", (Int_t) Link.GetIndex());
          mDigiMatch->AddLink(Link);
        }
        //delete pDigiMatch;

        LOG(debug) << cstr;

        new ((*fTofDigiMatchCollOut)[iNbHits]) CbmMatch(*mDigiMatch);
        delete mDigiMatch;
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

void CbmTofEventClusterizer::ExecEvent(Option_t* /*option*/)
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
void CbmTofEventClusterizer::Finish()
{
  if (fdEvent < 100) return;  // don't save histos with insufficient statistics
  WriteHistos();
  // Prevent them from being sucked in by the CbmHadronAnalysis WriteHistograms method
  // DeleteHistos();
  if (fdMemoryTime > 0.) CleanLHMemory();
  delete fDigiPar;
}

void CbmTofEventClusterizer::Finish(Double_t calMode)
{
  if (fdEvent < 100) return;  // don't save histos with insufficient statistics
  SetCalMode(calMode);
  Finish();
}

/************************************************************************************/
// Functions common for all clusters approximations
Bool_t CbmTofEventClusterizer::RegisterInputs()
{
  FairRootManager* fManager = FairRootManager::Instance();

  if (NULL == fManager) {
    LOG(error) << "CbmTofEventClusterizer::RegisterInputs => Could not find "
                  "FairRootManager!!!";
    return kFALSE;
  }  // if( NULL == fTofDigisColl)

  fTofDigiPointMatches = fManager->InitObjectAs<std::vector<CbmMatch> const*>("TofDigiMatch");
  if (NULL == fTofDigiPointMatches) LOG(info) << "No tof digi to point matches in the input file";
  else
    LOG(info) << "Found tof digi to point matches in the input file";

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
    LOG(info) << "CbmTofEventClusterizer::RegisterInputs => Could not get "
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
Bool_t CbmTofEventClusterizer::RegisterOutputs()
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

    if (NULL != fTofDigiPointMatches) {
      fTofDigiPointMatchesOut = new std::vector<CbmMatch>();
      rootMgr->RegisterAny("TofCalDigiMatch", fTofDigiPointMatchesOut, fbWriteDigisInOut);
    }
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

    if (NULL != fTofDigiPointMatches) {
      fTofDigiPointMatchesOut = new std::vector<CbmMatch>();
      rootMgr->RegisterAny("TofCalDigiMatch", fTofDigiPointMatchesOut, fbWriteDigisInOut);
    }
  }
  LOG(info) << "out branches: " << tHitBranchName << ", " << tHitDigiMatchBranchName;
  return kTRUE;
}
Bool_t CbmTofEventClusterizer::InitParameters()
{

  // Initialize the TOF GeoHandler
  Bool_t isSimulation = kFALSE;
  LOG(info) << "CbmTofEventClusterizer::InitParameters - Geometry, Mapping, ...  ??";

  // Get Base Container
  FairRun* ana        = FairRun::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  Int_t iGeoVersion = fGeoHandler->Init(isSimulation);
  if (k14a > iGeoVersion) {
    LOG(error) << "CbmTofEventClusterizer::InitParameters => Only compatible "
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
    LOG(error) << "CbmTofEventClusterizer::InitParameters => Could not obtain "
                  "the CbmTofDigiPar ";
    return kFALSE;
  }

  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
  if (0 == fDigiBdfPar) {
    LOG(error) << "CbmTofEventClusterizer::InitParameters => Could not obtain "
                  "the CbmTofDigiBdfPar ";
    return kFALSE;
  }

  rtdb->initContainers(ana->GetRunId());

  LOG(info) << "CbmTofEventClusterizer::InitParameter: currently " << fDigiPar->GetNrOfModules() << " digi cells ";

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
  /*
	 if(fiBeamRefAddr == 0) {  // initialize defaults of sep14
	 fiBeamRefType  = 5;
	 fiBeamRefSm    = 1;
	 fiBeamRefDet   = 0;
	 fiBeamAddRefMul= 0;
	 }
	 if(fSelId == 0) {  // initialize defaults of sep14
	 fSelId=4;
	 }
	 */

  LOG(info) << "<I>  BeamRefType = " << fiBeamRefType << ", Sm " << fiBeamRefSm << ", Det " << fiBeamRefDet
            << ", MulMax " << fiBeamRefMulMax;

  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofEventClusterizer::InitCalibParameter()
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
  LOG(info) << "CbmTofEventClusterizer::InitCalibParameter: defaults set";

  /// Save old global file and folder pointer to avoid messing with FairRoot
  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  if (0 < fCalMode) {
    LOG(info) << "CbmTofEventClusterizer::InitCalibParameter: read histos from "
              << "file " << fCalParFileName;

    // read parameter from histos
    if (fCalParFileName.IsNull()) return kTRUE;

    fCalParFile = new TFile(fCalParFileName, "");
    if (NULL == fCalParFile) {
      LOG(fatal) << "CbmTofEventClusterizer::InitCalibParameter: "
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

              Double_t YMean = ((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1);  //nh +1 empirical(?)
              Double_t TMean = ((TProfile*) htempTOff_pfx)->GetBinContent(iCh + 1);
              //Double_t dTYOff=YMean/fDigiBdfPar->GetSignalSpeed() ;
              Double_t dTYOff = YMean / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] += -dTYOff + TMean;
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] += +dTYOff + TMean;
              /*
							 if (iSmType==6 && iSm==0 && iRpc==1) {
							 LOG(info) << "Skip loading other calib parameters for TSR "<<iSmType<<iSm<<iRpc
							 ;
							 continue; // skip for inspection
							 }
							 */
              if (5 == iSmType || 8 == iSmType) {  // for PAD counters
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]    = fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1] = fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]  = fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][0];
              }
              //if(iSmType==0 && iSm==4 && iRpc==2 && iCh==26)
              LOG(info) << "InitCalibParameter:"
                        << " TSRC " << iSmType << iSm << iRpc << iCh << Form(": YMean %f, TMean %f", YMean, TMean)
                        << " -> "
                        << Form(" TOff %f, %f, TotG %f, %f ", fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
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
                  LOG(error) << "CbmTofEventClusterizer::InitCalibParameter: "
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

  LOG(info) << "CbmTofEventClusterizer::InitCalibParameter: initialization done";
  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofEventClusterizer::LoadGeometry()
{
  LOG(info) << "CbmTofEventClusterizer::LoadGeometry starting for  " << fDigiBdfPar->GetNbDet()
            << " described detectors, " << fDigiPar->GetNrOfModules() << " geometrically known cells ";

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
            LOG(warning) << "CbmTofEventClusterizer::LoadGeometry: StoreDigi "
                            "without channels "
                         << Form("SmTy %3d, Sm %3d, NbRpc %3d, Rpc, %3d ", iSmType, iSm, iNbRpc, iRpc);
          }
          LOG(debug1) << "CbmTofEventClusterizer::LoadGeometry: StoreDigi with "
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
Bool_t CbmTofEventClusterizer::DeleteGeometry()
{
  LOG(info) << "CbmTofEventClusterizer::DeleteGeometry starting";
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
Bool_t CbmTofEventClusterizer::CreateHistos()
{
  TDirectory* oldir = gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gROOT->cd();                     // <= To prevent histos from being sucked in by the param file of the TRootManager !
  fhClustBuildTime =
    new TH1I("TofEventClus_ClustBuildTime", "Time needed to build clusters in each event; Time [s]", 4000, 0.0, 4.0);
  // RPC related distributions
  Int_t iNbDet = fDigiBdfPar->GetNbDet();
  fviDetId.resize(iNbDet);

  fDetIdIndexMap.clear();
  for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
    Int_t iUniqueId           = fDigiBdfPar->GetDetUId(iDetIndx);
    fDetIdIndexMap[iUniqueId] = iDetIndx;
    fviDetId[iDetIndx]        = iUniqueId;
  }

  if (fDutId < 0) return kTRUE;

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

    /*
		 Int_t iUCellId  = CbmTofAddress::GetUniqueAddress(0,0,0,0,iS);
		 fChannelInfo = fDigiPar->GetCell(iUCellId);
		 */

    Int_t iUCellId(0);
    fChannelInfo = NULL;

    // Cover the case that the geometry file does not contain the module
    // indexed with 0 of a certain module type BUT modules with higher indices.
    for (Int_t iSM = 0; iSM < fDigiBdfPar->GetNbSm(iS); iSM++) {
      iUCellId     = CbmTofAddress::GetUniqueAddress(iSM, 0, 0, 0, iS);
      fChannelInfo = fDigiPar->GetCell(iUCellId);

      // Retrieve geometry information from the first module of a certain
      // module type that is found in the geometry file.
      if (NULL != fChannelInfo) { break; }
    }

    if (NULL == fChannelInfo) {
      LOG(warning) << "No DigiPar for SmType " << Form("%d, 0x%08x", iS, iUCellId);
      continue;
    }
    Double_t YDMAX = TMath::Max(2., fChannelInfo->GetSizey()) * YSCAL;

    fhSmCluPosition[iS] =
      new TH2F(Form("cl_SmT%01d_Pos", iS), Form("Clu position of SmType %d; Sm+Rpc# []; ypos [cm]", iS),
               fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 0,
               fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 99, -YDMAX, YDMAX);

    Double_t TSumMax = 1.E3;
    if (fTRefDifMax != 0.) TSumMax = fTRefDifMax;
    fhSmCluTOff[iS] =
      new TH2F(Form("cl_SmT%01d_TOff", iS), Form("Clu TimeZero in SmType %d; Sm+Rpc# []; TOff [ns]", iS),
               fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 0,
               fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 99, -TSumMax * 2., TSumMax * 2.);

    TProfile* hSvelcur = (TProfile*) gDirectory->FindObjectAny(Form("cl_SmT%01d_Svel", iS));
    if (NULL == hSvelcur) {
      fhSmCluSvel[iS] =
        new TProfile(Form("cl_SmT%01d_Svel", iS), Form("Clu Svel in SmType %d; Sm+Rpc# []; v/v_{nominal} ", iS),
                     fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 0,
                     fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 0.8, 1.2);
      fhSmCluSvel[iS]->Sumw2();
    }
    else
      fhSmCluSvel[iS] = (TProfile*) hSvelcur->Clone();

    fhSmCluFpar[iS].resize(4);
    for (Int_t iPar = 0; iPar < 4; iPar++) {
      TProfile* hFparcur = (TProfile*) gDirectory->FindObjectAny(Form("cl_SmT%01d_Fpar%1d", iS, iPar));
      if (NULL == hFparcur) {
        LOG(info) << "Histo " << Form("cl_SmT%01d_Fpar%1d", iS, iPar) << " not found, recreate ...";
        fhSmCluFpar[iS][iPar] = new TProfile(Form("cl_SmT%01d_Fpar%1d", iS, iPar),
                                             Form("Clu Fpar %d in SmType %d; Sm+Rpc# []; value ", iPar, iS),
                                             fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 0,
                                             fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), -100., 100.);
      }
      else
        fhSmCluFpar[iS][iPar] = (TProfile*) hFparcur->Clone();
    }

    fhTSmCluPosition[iS].resize(iNSel);
    fhTSmCluTOff[iS].resize(iNSel);
    fhTSmCluTRun[iS].resize(iNSel);
    for (Int_t iSel = 0; iSel < iNSel; iSel++) {  // Loop over selectors
      fhTSmCluPosition[iS][iSel] = new TH2F(Form("cl_TSmT%01d_Sel%02d_Pos", iS, iSel),
                                            Form("Clu position of SmType %d under Selector %02d; Sm+Rpc# "
                                                 "[]; ypos [cm]",
                                                 iS, iSel),
                                            fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 0,
                                            fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 99, -YDMAX, YDMAX);
      fhTSmCluTOff[iS][iSel]     = new TH2F(Form("cl_TSmT%01d_Sel%02d_TOff", iS, iSel),
                                        Form("Clu TimeZero in SmType %d under Selector %02d; Sm+Rpc# "
                                             "[]; TOff [ns]",
                                             iS, iSel),
                                        fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 0,
                                        fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS), 99, -TSumMax, TSumMax);
      fhTSmCluTRun[iS][iSel]     = new TH2F(Form("cl_TSmT%01d_Sel%02d_TRun", iS, iSel),
                                        Form("Clu TimeZero in SmType %d under Selector %02d; Event# "
                                             "[]; TMean [ns]",
                                             iS, iSel),
                                        100, 0, MaxNbEvent, 99, -TSumMax, TSumMax);
    }
  }

  LOG(info) << " Define Clusterizer histos for " << iNbDet << " detectors ";

  fhRpcDigiCor.resize(iNbDet);
  fhRpcDigiMul.resize(iNbDet);
  fhRpcDigiStatus.resize(iNbDet);
  fhRpcDigiDTLD.resize(iNbDet);
  fhRpcDigiDTFD.resize(iNbDet);
  fhRpcDigiDTMul.resize(iNbDet);
  fhRpcCluMul.resize(iNbDet);
  fhRpcCluRate.resize(iNbDet);
  fhRpcCluRate10s.resize(iNbDet);
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

  if (fTotMax != 0.) fdTOTMax = fTotMax;
  if (fTotMin != 0.) fdTOTMin = fTotMin;

  for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
    Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);

    Int_t iSmType  = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSmId    = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpcId   = CbmTofAddress::GetRpcId(iUniqueId);
    Int_t iUCellId = CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, 0, 0, iSmType);
    fChannelInfo   = fDigiPar->GetCell(iUCellId);
    if (NULL == fChannelInfo) {
      LOG(warning) << "No DigiPar for Det " << Form("0x%08x", iUCellId);
      continue;
    }
    LOG(info) << "DetIndx " << iDetIndx << ", SmType " << iSmType << ", SmId " << iSmId << ", RpcId " << iRpcId
              << " => UniqueId " << Form("(0x%08x, 0x%08x)", iUniqueId, iUCellId) << ", dx " << fChannelInfo->GetSizex()
              << ", dy " << fChannelInfo->GetSizey() << ", z " << fChannelInfo->GetZ()
              << Form(" ChPoi: %p ", fChannelInfo) << ", nbCh " << fDigiBdfPar->GetNbChan(iSmType, 0);

    // check access to all channel infos
    for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpcId); iCh++) {
      Int_t iCCellId = CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, iCh, 0, iSmType);
      fChannelInfo   = fDigiPar->GetCell(iCCellId);
      if (NULL == fChannelInfo) LOG(warning) << Form("missing ChannelInfo for ch %d addr 0x%08x", iCh, iCCellId);
    }

    fChannelInfo = fDigiPar->GetCell(iUCellId);
    fhRpcDigiCor[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DigiCor", iSmType, iSmId, iRpcId),
               Form("Digi Correlation of Rpc #%03d in Sm %03d of type %d; digi 0; digi 1", iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId));

    fhRpcDigiMul[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DigiMul", iSmType, iSmId, iRpcId),
               Form("Digi Multiplicity of Rpc #%03d in Sm %03d of type %d; strip #; "
                    "digi mul",
                    iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 20, 0, 20.);

    fhRpcDigiStatus[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DigiStatus", iSmType, iSmId, iRpcId),
               Form("Digi Status of Rpc #%03d in Sm %03d of type %d; strip #; digi status", iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 10, 0, 10.);

    const Int_t NLogbin = 40;
    Double_t edge[NLogbin + 1];
    for (Int_t i = 0; i < NLogbin + 1; i++)
      edge[i] = TMath::Power(2, i);
    fhRpcDigiDTLD[iDetIndx] = new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DigiDTLD", iSmType, iSmId, iRpcId),
                                       Form("Time distance to last digi of Rpc #%03d in Sm %03d of type %d; "
                                            "channel; t_{digi} - t_{previous digi} (ns)",
                                            iRpcId, iSmId, iSmType),
                                       fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2, 0,
                                       fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2, NLogbin, edge);

    fhRpcDigiDTFD[iDetIndx] = new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DigiDTFD", iSmType, iSmId, iRpcId),
                                       Form("Time distance to first digi of Rpc #%03d in Sm %03d of type %d; "
                                            "channel; t_{digi} - t_{first digi} (ns)",
                                            iRpcId, iSmId, iSmType),
                                       fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2, 0,
                                       fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2, 500., 0., 55.8);

    fhRpcDigiDTMul[iDetIndx] = new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DigiDTMul", iSmType, iSmId, iRpcId),
                                        Form("Multiplicity of digi of Rpc #%03d in Sm %03d of type %d; "
                                             "channel; Multiplicity",
                                             iRpcId, iSmId, iSmType),
                                        fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2, 0,
                                        fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2, 20., 0.5, 20.5);

    fhRpcCluMul[iDetIndx] =
      new TH1F(Form("cl_SmT%01d_sm%03d_rpc%03d_Mul", iSmType, iSmId, iRpcId),
               Form("Clu multiplicity of Rpc #%03d in Sm %03d of type %d; M []; Cnts", iRpcId, iSmId, iSmType),
               2. + 2. * fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, 2. + 2. * fDigiBdfPar->GetNbChan(iSmType, iRpcId));

    fhRpcCluRate[iDetIndx] =
      new TH1D(Form("cl_SmT%01d_sm%03d_rpc%03d_rate", iSmType, iSmId, iRpcId),
               Form("Clu rate of Rpc #%03d in Sm %03d of type %d; Time (s); Rate (Hz)", iRpcId, iSmId, iSmType), 36000.,
               0., 3600.);

    fhRpcCluRate10s[iDetIndx] = new TH1D(Form("cl_SmT%01d_sm%03d_rpc%03d_rate10s", iSmType, iSmId, iRpcId),
                                         Form("            Clu rate of Rpc #%03d in Sm %03d of type %d in last "
                                              "10s; Time (s); Counts per 100 #mus",
                                              iRpcId, iSmId, iSmType),
                                         100000, 0., 10.);

    fhRpcDTLastHits[iDetIndx] = new TH1F(Form("cl_SmT%01d_sm%03d_rpc%03d_DTLastHits", iSmType, iSmId, iRpcId),
                                         Form("Clu #DeltaT to last hits  of Rpc #%03d in Sm %03d of type %d; log( "
                                              "#DeltaT (ns)); counts",
                                              iRpcId, iSmId, iSmType),
                                         100., 0., 10.);

    fhRpcDTLastHits_Tot[iDetIndx] = new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Tot_DTLH", iSmType, iSmId, iRpcId),
                                             Form("Clu Tot of Rpc #%03d in Sm %03d of type %d; log(#DeltaT (ns)); TOT "
                                                  "[a.u.]",
                                                  iRpcId, iSmId, iSmType),
                                             100, 0., 10., 100, fdTOTMin, 4. * fdTOTMax);

    fhRpcDTLastHits_CluSize[iDetIndx] = new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_CluSize_DTLH", iSmType, iSmId, iRpcId),
                                                 Form("Clu Size of Rpc #%03d in Sm %03d of type %d; log(#DeltaT (ns)); "
                                                      "CluSize []",
                                                      iRpcId, iSmId, iSmType),
                                                 100, 0., 10., 16, 0.5, 16.5);

    Double_t YSCAL = 50.;
    if (fPosYMaxScal != 0.) YSCAL = fPosYMaxScal;
    Double_t YDMAX = TMath::Max(2., fChannelInfo->GetSizey()) * YSCAL;
    fhRpcCluPosition[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Pos", iSmType, iSmId, iRpcId),
               Form("Clu position of Rpc #%03d in Sm %03d of type %d; Strip []; ypos [cm]", iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 99, -YDMAX, YDMAX);

    fhRpcCluPositionEvol[iDetIndx] = new TProfile(Form("cl_SmT%01d_sm%03d_rpc%03d_PosEvol", iSmType, iSmId, iRpcId),
                                                  Form("Clu position of Rpc #%03d in Sm %03d of type %d; Analysis Time "
                                                       "[s]; ypos [cm]",
                                                       iRpcId, iSmId, iSmType),
                                                  1000, -1., 1.E4, -100., 100.);

    fhRpcCluTimeEvol[iDetIndx] = new TProfile(Form("cl_SmT%01d_sm%03d_rpc%03d_TimeEvol", iSmType, iSmId, iRpcId),
                                              Form("Clu time of Rpc #%03d in Sm %03d of type %d; Analysis Time [s]; dT "
                                                   "[ns]",
                                                   iRpcId, iSmId, iSmType),
                                              1000, -1., 1.E4, -10., 10.);

    fhRpcCluDelPos[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DelPos", iSmType, iSmId, iRpcId),
               Form("Clu position difference of Rpc #%03d in Sm %03d of type "
                    "%d; Strip []; #Deltaypos(clu) [cm]",
                    iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 99, -10., 10.);

    fhRpcCluDelMatPos[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DelMatPos", iSmType, iSmId, iRpcId),
               Form("Matched Clu position difference of Rpc #%03d in Sm %03d of type "
                    "%d; Strip []; #Deltaypos(mat) [cm]",
                    iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 99, -5., 5.);

    Double_t TSumMax = 1.E3;
    if (fTRefDifMax != 0.) TSumMax = fTRefDifMax;
    fhRpcCluTOff[iDetIndx] = new TH2F(
      Form("cl_SmT%01d_sm%03d_rpc%03d_TOff", iSmType, iSmId, iRpcId),
      Form("Clu TimeZero of Rpc #%03d in Sm %03d of type %d; Strip []; TOff [ns]", iRpcId, iSmId, iSmType),
      fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 99, -TSumMax, TSumMax);

    fhRpcCluDelTOff[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DelTOff", iSmType, iSmId, iRpcId),
               Form("Clu TimeZero Difference of Rpc #%03d in Sm %03d of type %d; Strip "
                    "[]; #DeltaTOff(clu) [ns]",
                    iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 99, -0.6, 0.6);

    fhRpcCluDelMatTOff[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_DelMatTOff", iSmType, iSmId, iRpcId),
               Form("Clu TimeZero Difference of Rpc #%03d in Sm %03d of type %d; Strip "
                    "[]; #DeltaTOff(mat) [ns]",
                    iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 99, -0.6, 0.6);

    fhRpcCluTrms[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Trms", iSmType, iSmId, iRpcId),
               Form("Clu Time RMS of Rpc #%03d in Sm %03d of type %d; Strip []; Trms [ns]", iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 99, 0., 0.5);

    fhRpcCluTot[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Tot", iSmType, iSmId, iRpcId),
               Form("Clu Tot of Rpc #%03d in Sm %03d of type %d; StripSide []; TOT [a.u.]", iRpcId, iSmId, iSmType),
               2 * fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, 2 * fDigiBdfPar->GetNbChan(iSmType, iRpcId), 100,
               fdTOTMin, fdTOTMax);

    fhRpcCluSize[iDetIndx] =
      new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Size", iSmType, iSmId, iRpcId),
               Form("Clu size of Rpc #%03d in Sm %03d of type %d; Strip []; size [strips]", iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 16, 0.5, 16.5);

    // Walk histos
    fhRpcCluAvWalk[iDetIndx] =
      new TH2D(Form("cl_SmT%01d_sm%03d_rpc%03d_AvWalk", iSmType, iSmId, iRpcId),
               Form("Walk in SmT%01d_sm%03d_rpc%03d_AvWalk; Tot [a.u.];  #DeltaT [ns]", iSmType, iSmId, iRpcId),
               nbClWalkBinX, fdTOTMin, fdTOTMax, nbClWalkBinY, -TSumMax, TSumMax);

    fhRpcCluAvLnWalk[iDetIndx] =
      new TH2D(Form("cl_SmT%01d_sm%03d_rpc%03d_AvLnWalk", iSmType, iSmId, iRpcId),
               Form("Walk in SmT%01d_sm%03d_rpc%03d_AvLnWalk; log_{10}Tot [a.u.];  "
                    "#DeltaT [ns]",
                    iSmType, iSmId, iRpcId),
               nbClWalkBinX, TMath::Log10(fdTOTMax / 50.), TMath::Log10(fdTOTMax), nbClWalkBinY, -TSumMax, TSumMax);

    fhRpcCluWalk[iDetIndx].resize(fDigiBdfPar->GetNbChan(iSmType, iRpcId));
    for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpcId); iCh++) {
      fhRpcCluWalk[iDetIndx][iCh].resize(2);
      for (Int_t iSide = 0; iSide < 2; iSide++) {
        fhRpcCluWalk[iDetIndx][iCh][iSide] =
          new TH2D(Form("cl_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Walk", iSmType, iSmId, iRpcId, iCh, iSide),
                   Form("Walk in SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Walk; Tot "
                        "[a.u.];  #DeltaT [ns]",
                        iSmType, iSmId, iRpcId, iCh, iSide),
                   nbClWalkBinX, fdTOTMin, fdTOTMax, nbClWalkBinY, -TSumMax, TSumMax);
      }
      /*
			 (fhRpcCluWalk[iDetIndx]).push_back( hTemp );
			 */
    }
  }

  // Clusterizing monitoring

  LOG(info) << " Define Clusterizer monitoring histos ";

  fhCluMulCorDutSel = new TH2F(Form("hCluMulCorDutSel"),
                               Form("Cluster Multiplicity Correlation of Dut %d%d%d with Sel "
                                    "%d%d%d; Mul(Sel); Mul(Dut)",
                                    fDutId, fDutSm, fDutRpc, fSelId, fSelSm, fSelRpc),
                               32, 0, 32, 32, 0, 32);

  fhEvCluMul = new TH2F(Form("hEvCluMul"), Form("Time Evolution of Cluster Multiplicity in Event; Time (s); Mul"), 1800,
                        0, 1800, 100, 0, 100);

  fhDigSpacDifClust = new TH1I("Clus_DigSpacDifClust",
                               "Space difference along channel direction between Digi pairs on "
                               "adjacent channels; PosCh i - Pos Ch i+1 [cm]",
                               5000, -10.0, 10.0);
  fhDigTimeDifClust = new TH1I("Clus_DigTimeDifClust",
                               "Time difference between Digi pairs on adjacent channels; "
                               "0.5*(tDigiA + tDigiA)chi - 0.5*(tDigiA + tDigiA)chi+1 [ns]",
                               5000, -5.0, 5.0);
  fhDigDistClust    = new TH2I("Clus_DigDistClust",
                            "Distance between Digi pairs on adjacent channels; PosCh i - Pos Ch i+1 "
                            "[cm along ch]; 0.5*(tDigiA + tDigiA)chi - 0.5*(tDigiA + tDigiA)chi+1 [ns]",
                            5000, -10.0, 10.0, 2000, -5.0, 5.0);
  fhClustSizeDifX   = new TH2I("Clus_ClustSizeDifX",
                             "Position difference between Point and Hit as function of Cluster "
                             "Size; Cluster Size [Strips]; dX [cm]",
                             100, 0.5, 100.5, 500, -50, 50);
  fhClustSizeDifY   = new TH2I("Clus_ClustSizeDifY",
                             "Position difference between Point and Hit as function of Cluster "
                             "Size; Cluster Size [Strips]; dY [cm]",
                             100, 0.5, 100.5, 500, -50, 50);
  fhChDifDifX       = new TH2I("Clus_ChDifDifX",
                         "Position difference between Point and Hit as "
                         "function of Channel dif; Ch Dif [Strips]; dX [cm]",
                         101, -50.5, 50.5, 500, -50, 50);
  fhChDifDifY       = new TH2I("Clus_ChDifDifY",
                         "Position difference between Point and Hit as "
                         "function of Channel Dif; Ch Dif [Strips]; dY [cm]",
                         101, -50.5, 50.5, 500, -50, 50);
  fhNbSameSide      = new TH1I("Clus_NbSameSide",
                          "How many time per event the 2 digis on a channel "
                          "were of the same side ; Counts/Event []",
                          500, 0.0, 500.0);
  fhNbDigiPerChan   = new TH1I("Clus_NbDigiPerChan", "Nb of Digis per channel; Nb Digis []", 100, 0.0, 100.0);

  // Trigger selected histograms
  if (0 < iNSel) {

    fhSeldT.resize(iNSel);
    for (Int_t iSel = 0; iSel < iNSel; iSel++) {
      fhSeldT[iSel] = new TH2F(Form("cl_dt_Sel%02d", iSel), Form("Selector time %02d; dT [ns]", iSel), 99,
                               -fdDelTofMax * 10., fdDelTofMax * 10., 16, -0.5, 15.5);
    }

    fhTRpcCluMul.resize(iNbDet);
    fhTRpcCluPosition.resize(iNbDet);
    fhTRpcCluTOff.resize(iNbDet);
    fhTRpcCluTofOff.resize(iNbDet);
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
      Int_t iUCellId  = CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, 0, 0, iSmType);
      fChannelInfo    = fDigiPar->GetCell(iUCellId);
      if (NULL == fChannelInfo) {
        LOG(warning) << "No DigiPar for Det " << Form("0x%08x", iUCellId);
        continue;
      }
      LOG(debug1) << "DetIndx " << iDetIndx << ", SmType " << iSmType << ", SmId " << iSmId << ", RpcId " << iRpcId
                  << " => UniqueId " << Form("(0x%08x, 0x%08x)", iUniqueId, iUCellId) << ", dx "
                  << fChannelInfo->GetSizex() << ", dy " << fChannelInfo->GetSizey()
                  << Form(" poi: 0x%p ", fChannelInfo) << ", nbCh " << fDigiBdfPar->GetNbChan(iSmType, iRpcId);

      fhTRpcCluMul[iDetIndx].resize(iNSel);
      fhTRpcCluPosition[iDetIndx].resize(iNSel);
      fhTRpcCluTOff[iDetIndx].resize(iNSel);
      fhTRpcCluTofOff[iDetIndx].resize(iNSel);
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
          new TH1F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Mul", iSmType, iSmId, iRpcId, iSel),
                   Form("Clu multiplicity of Rpc #%03d in Sm %03d of type %d "
                        "under Selector %02d; M []; cnts",
                        iRpcId, iSmId, iSmType, iSel),
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId) + 2, 0., fDigiBdfPar->GetNbChan(iSmType, iRpcId) + 2);

        if (NULL == fhTRpcCluMul[iDetIndx][iSel]) LOG(fatal) << " Histo not generated !";

        Double_t YSCAL = 50.;
        if (fPosYMaxScal != 0.) YSCAL = fPosYMaxScal;
        Double_t YDMAX                    = TMath::Max(2., fChannelInfo->GetSizey()) * YSCAL;
        fhTRpcCluPosition[iDetIndx][iSel] = new TH2F(
          Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Pos", iSmType, iSmId, iRpcId, iSel),
          Form("Clu position of Rpc #%03d in Sm %03d of type %d under "
               "Selector %02d; Strip []; ypos [cm]",
               iRpcId, iSmId, iSmType, iSel),
          fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 100, -YDMAX, YDMAX);

        Double_t TSumMax = 1.E4;
        if (fTRefDifMax != 0.) TSumMax = fTRefDifMax;
        if (iSmType == 5) TSumMax *= 2.;
        fhTRpcCluTOff[iDetIndx][iSel] = new TH2F(
          Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_TOff", iSmType, iSmId, iRpcId, iSel),
          Form("Clu TimeZero of Rpc #%03d in Sm %03d of type %d under "
               "Selector %02d; Strip []; TOff [ns]",
               iRpcId, iSmId, iSmType, iSel),
          fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 999, -TSumMax, TSumMax);

        fhTRpcCluTofOff[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_TofOff", iSmType, iSmId, iRpcId, iSel),
                   Form("Clu TimeDeviation of Rpc #%03d in Sm %03d of type %d "
                        "under Selector %02d; Strip []; TOff [ns]",
                        iRpcId, iSmId, iSmType, iSel),
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 999,
                   -TSumMax * 4., TSumMax * 4.);

        if (fTotMax != 0.) fdTOTMax = fTotMax;
        fhTRpcCluTot[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Tot", iSmType, iSmId, iRpcId, iSel),
                   Form("Clu Tot of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; StripSide []; TOT [a.u.]",
                        iRpcId, iSmId, iSmType, iSel),
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2, 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId) * 2, 100,
                   fdTOTMin, fdTOTMax);

        fhTRpcCluSize[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Size", iSmType, iSmId, iRpcId, iSel),
                   Form("Clu size of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; Strip []; size [strips]",
                        iRpcId, iSmId, iSmType, iSel),
                   fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 16, 0.5, 16.5);

        // Walk histos
        fhTRpcCluAvWalk[iDetIndx][iSel] =
          new TH2D(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_AvWalk", iSmType, iSmId, iRpcId, iSel),
                   Form("Walk in SmT%01d_sm%03d_rpc%03d_Sel%02d_AvWalk; TOT; T-TSel", iSmType, iSmId, iRpcId, iSel),
                   nbClWalkBinX, fdTOTMin, fdTOTMax, nbClWalkBinY, -TSumMax, TSumMax);

        // Tof Histos
        fhTRpcCluDelTof[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSmId, iRpcId, iSel),
                   Form("SmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof; TRef-TSel; T-TSel", iSmType, iSmId, iRpcId, iSel),
                   nbClDelTofBinX, -fdDelTofMax, fdDelTofMax, nbClDelTofBinY, -TSumMax, TSumMax);

        // Position deviation histos
        fhTRpcCludXdY[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_dXdY", iSmType, iSmId, iRpcId, iSel),
                   Form("SmT%01d_sm%03d_rpc%03d_Sel%02d_dXdY; #Delta x [cm]; "
                        "#Delta y [cm];",
                        iSmType, iSmId, iRpcId, iSel),
                   nbCldXdYBinX, -dXdYMax, dXdYMax, nbCldXdYBinY, -dXdYMax, dXdYMax);

        fhTRpcCluWalk2[iDetIndx][iSel] = new TH3F(
          Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Walk2", iSmType, iSmId, iRpcId, iSel),
          Form("SmT%01d_sm%03d_rpc%03d_Sel%02d_Walk2; Tot_1; Tot_2; #Delta t[ns]", iSmType, iSmId, iRpcId, iSel),
          nbClWalkBinX, fdTOTMin, fdTOTMax, nbClWalkBinX, fdTOTMin, fdTOTMax, nbClWalkBinY, -TSumMax, TSumMax);

        fhTRpcCluWalk[iDetIndx][iSel].resize(fDigiBdfPar->GetNbChan(iSmType, iRpcId));

        for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpcId); iCh++) {
          fhTRpcCluWalk[iDetIndx][iSel][iCh].resize(2);
          for (Int_t iSide = 0; iSide < 2; iSide++) {
            fhTRpcCluWalk[iDetIndx][iSel][iCh][iSide] = new TH2D(
              Form("cl_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Sel%02d_Walk", iSmType, iSmId, iRpcId, iCh, iSide, iSel),
              Form("Walk in SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Sel%02d_Walk", iSmType, iSmId, iRpcId, iCh, iSide,
                   iSel),
              nbClWalkBinX, fdTOTMin, fdTOTMax, nbClWalkBinY, -TSumMax, TSumMax);
          }
        }

        fhTRpcCluTOffDTLastHits[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_TOff_DTLH", iSmType, iSmId, iRpcId, iSel),
                   Form("Clu TimeZero of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; log(#DeltaT (ns)); TOff [ns]",
                        iRpcId, iSmId, iSmType, iSel),
                   100, 0., 10., 99, -TSumMax, TSumMax);

        fhTRpcCluTotDTLastHits[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Tot_DTLH", iSmType, iSmId, iRpcId, iSel),
                   Form("Clu Tot of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; log(#DeltaT (ns)); TOT [a.u.]",
                        iRpcId, iSmId, iSmType, iSel),
                   100, 0., 10., 100, fdTOTMin, fdTOTMax);

        fhTRpcCluSizeDTLastHits[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Size_DTLH", iSmType, iSmId, iRpcId, iSel),
                   Form("Clu size of Rpc #%03d in Sm %03d of type %d under "
                        "Selector %02d; log(#DeltaT (ns)); size [strips]",
                        iRpcId, iSmId, iSmType, iSel),
                   100, 0., 10., 10, 0.5, 10.5);

        fhTRpcCluMemMulDTLastHits[iDetIndx][iSel] =
          new TH2F(Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_MemMul_DTLH", iSmType, iSmId, iRpcId, iSel),
                   Form("Clu Memorized Multiplicity of Rpc #%03d in Sm %03d of type %d "
                        "under Selector %02d; log(#DeltaT (ns)); TOff [ns]",
                        iRpcId, iSmId, iSmType, iSel),
                   100, 0., 10., 10, 0, 10);
      }
    }
  }
  // MC reference
  fhHitsPerTracks =
    new TH1I("Clus_TofHitPerTrk", "Mean Number of TofHit per Mc Track; Nb TofHits/Nb MC Tracks []", 2000, 0.0, 20.0);
  if (kFALSE == fDigiBdfPar->ClustUseTrackId())
    fhPtsPerHit = new TH1I("Clus_TofPtsPerHit",
                           "Distribution of the Number of MCPoints associated "
                           "to each TofHit; Nb MCPoint []",
                           20, 0.0, 20.0);
  if (kTRUE == fDigiBdfPar->ClustUseTrackId()) {
    fhTimeResSingHits  = new TH1I("Clus_TofTimeResClust",
                                 "Time resolution for TofHits containing Digis from a single MC "
                                 "Track; t(1st Mc Point) -tTofHit [ns]",
                                 10000, -25.0, 25.0);
    fhTimeResSingHitsB = new TH2I("Clus_TofTimeResClustB",
                                  "Time resolution for TofHits containing Digis from a single MC "
                                  "Track; (1st Mc Point) -tTofHit [ns]",
                                  5000, -25.0, 25.0, 6, 0, 6);
    fhTimePtVsHits     = new TH2I("Clus_TofTimePtVsHit",
                              "Time resolution for TofHits containing Digis from a single MC "
                              "Track; t(1st Mc Point) -tTofHit [ns]",
                              2000, 0.0, 50.0, 2000, 0.0, 50.0);
  }
  else {
    fhTimeResSingHits  = new TH1I("Clus_TofTimeResClust",
                                 "Time resolution for TofHits containing Digis from a single "
                                 "TofPoint; tMcPoint -tTofHit [ns]",
                                 10000, -25.0, 25.0);
    fhTimeResSingHitsB = new TH2I("Clus_TofTimeResClustB",
                                  "Time resolution for TofHits containing Digis from a single "
                                  "TofPoint; tMcPoint -tTofHit [ns]",
                                  5000, -25.0, 25.0, 6, 0, 6);
    fhTimePtVsHits     = new TH2I("Clus_TofTimePtVsHit",
                              "Time resolution for TofHits containing Digis "
                              "from a single TofPoint; tMcPoint -tTofHit [ps]",
                              2000, 0.0, 50.0, 2000, 0.0, 50.0);
  }  // else of if( kTRUE == fDigiBdfPar->ClustUseTrackId() )
  fhClusterSize = new TH1I("Clus_ClusterSize", "Cluster Size distribution; Cluster Size [Strips]", 100, 0.5, 100.5);
  fhClusterSizeType =
    new TH2I("Clus_ClusterSizeType",
             "Cluster Size distribution in each (SM type, Rpc) pair; Cluster "
             "Size [Strips]; 10*SM Type + Rpc Index []",
             100, 0.5, 100.5, 40 * fDigiBdfPar->GetNbSmTypes(), 0.0, 40 * fDigiBdfPar->GetNbSmTypes());
  if (kTRUE == fDigiBdfPar->ClustUseTrackId()) {
    fhTrackMul = new TH1I("Clus_TrackMul", "Number of MC tracks generating the cluster; MC Tracks multiplicity []", 100,
                          0.5, 100.5);
    fhClusterSizeMulti = new TH2I("Clus_ClusterSizeMulti",
                                  "Cluster Size distribution as function of Number of MC tracks generating "
                                  "the cluster; Cluster Size [Strips]; MC tracks mul. []",
                                  100, 0.5, 100.5, 100, 0.5, 100.5);
    fhTrk1MulPos       = new TH2D("Clus_Trk1MulPos",
                            "Position of Clusters with only 1 MC tracks "
                            "generating the cluster; X [cm]; Y [cm]",
                            1500, -750, 750, 1000, -500, 500);
    fhHiTrkMulPos      = new TH2D("Clus_HiTrkMulPos",
                             "Position of Clusters with >1 MC tracks "
                             "generating the cluster; X [cm]; Y [cm]",
                             1500, -750, 750, 1000, -500, 500);
    fhAllTrkMulPos    = new TH2D("Clus_AllTrkMulPos", "Position of all clusters generating the cluster; X [cm]; Y [cm]",
                              1500, -750, 750, 1000, -500, 500);
    fhMultiTrkProbPos = new TH2D("Clus_MultiTrkProbPos",
                                 "Probability of having a cluster with multiple tracks as "
                                 "function of position; X [cm]; Y [cm]; Prob. [%]",
                                 1500, -750, 750, 1000, -500, 500);
  }  // if( kTRUE == fDigiBdfPar->ClustUseTrackId() )

  gDirectory->cd(oldir->GetPath());  // <= To prevent histos from being sucked in by the param file of the TRootManager!

  return kTRUE;
}

Bool_t CbmTofEventClusterizer::FillHistos()
{
  fhClustBuildTime->Fill(fStop.GetSec() - fStart.GetSec() + (fStop.GetNanoSec() - fStart.GetNanoSec()) / 1e9);

  if (fDutId < 0) return kTRUE;

  Int_t iNbTofHits = fTofHitsColl->GetEntriesFast();
  CbmTofHit* pHit;
  //gGeoManager->SetTopVolume( gGeoManager->FindVolumeFast("tof_v14a") );
  gGeoManager->CdTop();

  if (0 < iNbTofHits) {
    Double_t dCluMul = 0.;
    Bool_t BSel[iNSel];
    Double_t dTTrig[iNSel];
    CbmTofHit* pTrig[iNSel];
    Double_t ddXdZ[iNSel];
    Double_t ddYdZ[iNSel];
    Double_t dSel2dXdYMin[iNSel];

    Int_t iBeamRefMul    = 0;
    Int_t iBeamAddRefMul = 0;
    CbmTofHit* pBeamRef  = NULL;

    if (0 < iNSel) {  // check software triggers

      LOG(debug) << "CbmTofEventClusterizer::FillHistos() for " << iNSel << " triggers"
                 << ", Dut " << fDutId << ", " << fDutSm << ", " << fDutRpc << Form(", 0x%08x", fDutAddr) << ", Sel "
                 << fSelId << ", " << fSelSm << ", " << fSelRpc << Form(", 0x%08x", fSelAddr) << ", Sel2 " << fSel2Id
                 << ", " << fSel2Sm << ", " << fSel2Rpc << Form(", 0x%08x", fSel2Addr);
      LOG(debug) << "CbmTofEventClusterizer::FillHistos: Muls: " << fviClusterMul[fDutId][fDutSm][fDutRpc] << ", "
                 << fviClusterMul[fSelId][fSelSm][fSelRpc];

      // monitor multiplicities
      Int_t iDetMul = 0;
      Int_t iNbDet  = fDigiBdfPar->GetNbDet();
      for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
        Int_t iDetId  = fviDetId[iDetIndx];
        Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
        Int_t iSm     = CbmTofAddress::GetSmId(iDetId);
        Int_t iRpc    = CbmTofAddress::GetRpcId(iDetId);
        //LOG(info) << Form(" indx %d, Id 0x%08x, TSR %d %d %d", iDetIndx, iDetId, iSmType, iSm, iRpc)
        //          ;
        if (NULL != fhRpcCluMul[iDetIndx]) {
          if (fviClusterMul[iSmType][iSm][iRpc] > 0) iDetMul++;
          dCluMul += fviClusterMul[iSmType][iSm][iRpc];                    // total hit multiplicity in event
          fhRpcCluMul[iDetIndx]->Fill(fviClusterMul[iSmType][iSm][iRpc]);  //
        }
      }

      fhCluMulCorDutSel->Fill(fviClusterMul[fSelId][fSelSm][fSelRpc], fviClusterMul[fDutId][fDutSm][fDutRpc]);

      // do input distributions first
      for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
        pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
        if (NULL == pHit) continue;
        if (StartAnalysisTime == 0.) {
          StartAnalysisTime = pHit->GetTime();
          LOG(info) << "StartAnalysisTime set to " << StartAnalysisTime / 1.E9 << " s. ";
          fdStartAna10s = StartAnalysisTime;
        }
        Int_t iDetId = (pHit->GetAddress() & DetMask);

        std::map<UInt_t, UInt_t>::iterator it = fDetIdIndexMap.find(iDetId);
        if (it == fDetIdIndexMap.end()) continue;  // continue for invalid detector index
        Int_t iDetIndx = it->second;               //fDetIdIndexMap[iDetId];

        Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
        Int_t iSm     = CbmTofAddress::GetSmId(iDetId);
        Int_t iRpc    = CbmTofAddress::GetRpcId(iDetId);
        Int_t iCh     = CbmTofAddress::GetChannelId(pHit->GetAddress());

        Double_t dTimeAna = (pHit->GetTime() - StartAnalysisTime) / 1.E9;
        //LOG(debug)<<"TimeAna "<<StartAnalysisTime<<", "<< pHit->GetTime()<<", "<<dTimeAna;
        fhRpcCluRate[iDetIndx]->Fill(dTimeAna, 1. / fhRpcCluRate[iDetIndx]->GetBinWidth(1));

        // deal with spill structure
        Double_t dTimeAna10s = pHit->GetTime() - fdStartAna10s;
        if (iHitInd == 0) {
          fhEvCluMul->Fill(dTimeAna, dCluMul);
          if (iDetMul > 5
              && fviClusterMul[fiBeamRefType][fiBeamRefSm][fiBeamRefDet] > 0) {  // FIXME: hardwired constants
            if (dTLEvt == 0) dTLEvt = pHit->GetTime();
            Double_t dDTLEvt = pHit->GetTime() - dTLEvt;
            dTLEvt           = pHit->GetTime();
            if (dDTLEvt > fdSpillBreak * 1.E9 && dTimeAna10s > fdSpillDuration * 1.E9) {
              //if( dDTLEvt> 5.E8 && dTimeAna10s > 10.) {
              fdStartAna10s = pHit->GetTime();
              iNSpill++;
              LOG(info) << "Resetting 10s rate histo  for spill " << iNSpill << " at " << fdStartAna10s / 1.E9
                        << "s after " << dDTLEvt / 1.E9 << " s without events";
              for (Int_t iDet = 0; iDet < fDigiBdfPar->GetNbDet(); iDet++) {
                if (NULL == fhRpcCluRate10s[iDet]) continue;
                fhRpcCluRate10s[iDet]->Reset("ICES");
              }
              dTimeAna10s = 0.;
            }
          }
        }

        if (fdStartAna10s > 0.) fhRpcCluRate10s[iDetIndx]->Fill(dTimeAna10s / 1.E9, 1.);
        //       fhRpcCluRate10s[iDetIndx]->Fill(dTimeAna10s/1.E9,1./fhRpcCluRate10s[iDetIndx]->GetBinWidth(1));

        if (fdMemoryTime > 0. && fvLastHits[iSmType][iSm][iRpc][iCh].size() == 0)
          LOG(fatal) << Form(" <E> hit not stored in memory for TSRC %d%d%d%d", iSmType, iSm, iRpc, iCh);

        //CheckLHMemory();

        if (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 1) {  // check for outdated hits
          //std::list<CbmTofHit *>::iterator it0=fvLastHits[iSmType][iSm][iRpc][iCh].begin();
          //std::list<CbmTofHit *>::iterator itL=fvLastHits[iSmType][iSm][iRpc][iCh].end();
          //CbmTofHit* pH0 = *it0;
          //CbmTofHit* pHL = *(--itL);
          CbmTofHit* pH0 = fvLastHits[iSmType][iSm][iRpc][iCh].front();
          CbmTofHit* pHL = fvLastHits[iSmType][iSm][iRpc][iCh].back();
          if (pH0->GetTime() > pHL->GetTime())
            LOG(warning) << Form("Invalid time ordering in ev %8.0f in list of "
                                 "size %lu for TSRC %d%d%d%d: Delta t %f  ",
                                 fdEvent, fvLastHits[iSmType][iSm][iRpc][iCh].size(), iSmType, iSm, iRpc, iCh,
                                 pHL->GetTime() - pH0->GetTime());

          //       while( (*((std::list<CbmTofHit *>::iterator) fvLastHits[iSmType][iSm][iRpc][iCh].begin()))->GetTime()+fdMemoryTime < pHit->GetTime()
          while (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 2.
                 || fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime() + fdMemoryTime < pHit->GetTime())

          {
            LOG(debug) << " pop from list size " << fvLastHits[iSmType][iSm][iRpc][iCh].size()
                       << Form(" outdated hits for ev %8.0f in TSRC %d%d%d%d", fdEvent, iSmType, iSm, iRpc, iCh)
                       << Form(" with tHit - tLast %f ",
                               pHit->GetTime() - fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime())
              //(*((std::list<CbmTofHit *>::iterator) fvLastHits[iSmType][iSm][iRpc][iCh].begin()))->GetTime())
              ;
            if (fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress() != pHit->GetAddress())
              LOG(fatal) << Form("Inconsistent address in list of size %lu for TSRC %d%d%d%d: "
                                 "0x%08x, time  %f",
                                 fvLastHits[iSmType][iSm][iRpc][iCh].size(), iSmType, iSm, iRpc, iCh,
                                 fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetAddress(),
                                 fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime());
            fvLastHits[iSmType][iSm][iRpc][iCh].front()->Delete();
            fvLastHits[iSmType][iSm][iRpc][iCh].pop_front();
          }
        }  //fvLastHits[iSmType][iSm][iRpc][iCh].size()>1)

        // plot remaining time difference to previous hits
        if (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 1) {  // check for previous hits in memory time interval
          CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
          Double_t dTotSum    = 0.;
          for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink += 2) {  // loop over digis
            CbmLink L0        = digiMatch->GetLink(iLink);
            Int_t iDigInd0    = L0.GetIndex();
            Int_t iDigInd1    = (digiMatch->GetLink(iLink + 1)).GetIndex();
            CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
            CbmTofDigi* pDig1 = &(fTofCalDigiVec->at(iDigInd1));
            //CbmTofDigi *pDig0 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
            //CbmTofDigi *pDig1 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd1));
            dTotSum += pDig0->GetTot() + pDig1->GetTot();
          }

          std::list<CbmTofHit*>::iterator itL = fvLastHits[iSmType][iSm][iRpc][iCh].end();
          itL--;
          for (size_t iH = 0; iH < fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1; iH++) {
            itL--;
            fhRpcDTLastHits[iDetIndx]->Fill(TMath::Log10(pHit->GetTime() - (*itL)->GetTime()));
            fhRpcDTLastHits_CluSize[iDetIndx]->Fill(TMath::Log10(pHit->GetTime() - (*itL)->GetTime()),
                                                    digiMatch->GetNofLinks() / 2.);
            fhRpcDTLastHits_Tot[iDetIndx]->Fill(TMath::Log10(pHit->GetTime() - (*itL)->GetTime()), dTotSum);
          }
        }
      }  // iHitInd loop end

      // do reference first
      dTRef     = dDoubleMax;
      fTRefHits = 0;

      for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
        pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
        if (NULL == pHit) continue;
        Int_t iDetId = (pHit->GetAddress() & SelMask);

        if (fiBeamRefAddr == iDetId) {
          if (fviClusterMul[fiBeamRefType][fiBeamRefSm][fiBeamRefDet] > fiBeamRefMulMax) break;
          // Check Tot
          CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
          Double_t TotSum     = 0.;
          for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink += 2) {  // loop over digis
            CbmLink L0      = digiMatch->GetLink(iLink);                         //vDigish.at(ivDigInd);
            UInt_t iDigInd0 = L0.GetIndex();
            //           if (iDigInd0 < fTofCalDigisColl->GetEntriesFast()){
            if (iDigInd0 < fTofCalDigiVec->size()) {
              //            CbmTofDigi *pDig0 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
              CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
              TotSum += pDig0->GetTot();
            }
          }
          TotSum /= (0.5 * digiMatch->GetNofLinks());
          if (TotSum > fhRpcCluTot[iIndexDut]->GetYaxis()->GetXmax()) continue;  // ignore too large clusters

          fTRefHits = 1;
          if (pHit->GetTime() < dTRef) {
            dTRef    = pHit->GetTime();
            pBeamRef = pHit;
          }
          iBeamRefMul++;
        }
        else {  //additional reference type multiplicity
          if (fiBeamRefType == CbmTofAddress::GetSmType(iDetId)) iBeamAddRefMul++;
        }
      }
      LOG(debug) << "CbmTofEventClusterizer::FillHistos: BRefMul: " << iBeamRefMul << ", " << iBeamAddRefMul;

      if (iBeamRefMul == 0) return kFALSE;                  // don't fill histos without reference time
      if (iBeamAddRefMul < fiBeamAddRefMul) return kFALSE;  // ask for confirmation by other beam counters
      if (NULL == pBeamRef) return kFALSE;                  // should never happen

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
        dTTrig[iSel]       = dDoubleMax;

        switch (iSel) {
          case 0:  //  Detector under Test (Dut) && Diamonds,BeamRef
            iRl = fviClusterMul[fDutId][fDutSm].size();
            if (fIdMode == 0 && fDutRpc > -1) {
              iR0 = fDutRpc;
              iRl = fDutRpc + 1;
            }
            for (Int_t iRpc = iR0; iRpc < iRl; iRpc++)
              iDutMul += fviClusterMul[fDutId][fDutSm][iRpc];
            LOG(debug) << "Selector 0: DutMul " << fviClusterMul[fDutId][fDutSm][fDutRpc] << ", " << iDutMul
                       << ", BRefMul " << iBeamRefMul << " TRef: " << dTRef << ", BeamAddRefMul " << iBeamAddRefMul
                       << ", " << fiBeamAddRefMul;

            if (iDutMul > 0 && iDutMul < fiCluMulMax) {
              LOG(debug1) << "Found selector 0, NbHits  " << iNbTofHits;
              for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
                pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
                if (NULL == pHit) continue;

                Int_t iDetId = (pHit->GetAddress() & SelMask);
                LOG(debug1) << Form(" Det 0x%08x, Dut 0x%08x, T %f, TTrig %f", iDetId, fDutAddr, pHit->GetTime(),
                                    dTTrig[iSel]);
                //if( fDutId == CbmTofAddress::GetSmType( iDetId ))
                if (fDutAddr == iDetId) {
                  if (pHit->GetTime() < dTTrig[iSel]) {
                    if (TMath::Abs(pBeamRef->GetTime() - pHit->GetTime()) < fdDelTofMax) {
                      //                       < fhTRpcCluTOff[iIndexDut][iSel]->GetYaxis()->GetXmax()) {
                      dTTrig[iSel] = pHit->GetTime();
                      pTrig[iSel]  = pHit;
                      BSel[iSel]   = kTRUE;
                    }
                  }
                }
              }
              if (BSel[iSel])
                LOG(debug) << Form("Found selector 0 with mul %d from 0x%08x at %f ", iDutMul,
                                   pTrig[iSel]->GetAddress(), dTTrig[iSel]);
            }
            break;

          case 1:  // MRef & BRef
            iRl = fviClusterMul[fSelId][fSelSm].size();
            if (fIdMode == 0 && fSelRpc > -1) {
              iR0 = fSelRpc;
              iRl = fSelRpc + 1;
            }
            for (Int_t iRpc = iR0; iRpc < iRl; iRpc++)
              iRefMul += fviClusterMul[fSelId][fSelSm][iRpc];
            LOG(debug) << "CbmTofEventClusterizer::FillHistos(): selector 1: RefMul "
                       << fviClusterMul[fSelId][fSelSm][fSelRpc] << ", " << iRefMul << ", BRefMul " << iBeamRefMul;
            if (iRefMul > 0 && iRefMul < fiCluMulMax) {
              LOG(debug1) << "CbmTofEventClusterizer::FillHistos(): Found "
                             "selector 1, BeamRef at"
                          << pBeamRef;
              dTTrig[iSel] = dDoubleMax;
              for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
                pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
                if (NULL == pHit) continue;

                Int_t iDetId = (pHit->GetAddress() & SelMask);
                if (fSelAddr == iDetId) {
                  LOG(debug1) << "Check hit  " << iHitInd << ", sel   " << iSel << ", t: " << pHit->GetTime() << ", TT "
                              << dTTrig[iSel];

                  if (pHit->GetTime() < dTTrig[iSel]) {
                    if (TMath::Abs(pBeamRef->GetTime() - pHit->GetTime()) < fdDelTofMax) {
                      //              < fhTRpcCluTOff[iIndexDut][iSel]->GetYaxis()->GetXmax()) {
                      dTTrig[iSel] = pHit->GetTime();
                      pTrig[iSel]  = pHit;
                      BSel[iSel]   = kTRUE;
                      LOG(debug1) << "Accept hit  " << iHitInd << ", sel   " << iSel << ", t: " << pHit->GetTime()
                                  << ", TT " << dTTrig[iSel];
                    }
                  }
                }
              }
              if (BSel[iSel])
                LOG(debug) << Form("Found selector 1 with mul %d from 0x%08x at %f ", iRefMul,
                                   pTrig[iSel]->GetAddress(), dTTrig[iSel]);
            }
            break;

          default:
            LOG(info) << "CbmTofEventClusterizer::FillHistos: selection not "
                         "implemented "
                      << iSel;
            ;
        }  // switch end
        if (fTRefMode > 10) {
          dTTrig[iSel]  = dTRef;
          Int_t iReqMul = (fTRefMode - fTRefMode % 10) / 10;
          if (iDetMul < iReqMul) BSel[iSel] = 0;
        }
      }  // iSel - loop end

      LOG(debug1) << "selector loop passed, continue with Sel2Id " << fSel2Id;

      if (fSel2Id > -1) {  // confirm selector by independent match
        for (Int_t iSel = 0; iSel < iNSel; iSel++) {
          if (BSel[iSel]) {
            BSel[iSel] = kFALSE;
            if (fviClusterMul[fSel2Id][fSel2Sm][fSel2Rpc] > 0
                && fviClusterMul[fSel2Id][fSel2Sm][fSel2Rpc] < fiCluMulMax)
              for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
                pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
                if (NULL == pHit) continue;
                Int_t iDetId = (pHit->GetAddress() & SelMask);
                if (fSel2Addr == iDetId) {
                  Double_t dzscal = 1.;
                  if (fEnableMatchPosScaling) dzscal = pHit->GetZ() / pTrig[iSel]->GetZ();
                  Double_t dSEl2dXdz = (pHit->GetX() - pTrig[iSel]->GetX()) / (pHit->GetZ() - pTrig[iSel]->GetZ());
                  Double_t dSEl2dYdz = (pHit->GetY() - pTrig[iSel]->GetY()) / (pHit->GetZ() - pTrig[iSel]->GetZ());

                  if (iDetId == fiBeamRefAddr
                      || (TMath::Sqrt(TMath::Power(pHit->GetX() - dzscal * pTrig[iSel]->GetX(), 2.)
                                      + TMath::Power(pHit->GetY() - dzscal * pTrig[iSel]->GetY(), 2.))
                          < fdCaldXdYMax)) {
                    BSel[iSel]     = kTRUE;
                    Double_t dX2Y2 = TMath::Sqrt(dSEl2dXdz * dSEl2dXdz + dSEl2dYdz * dSEl2dYdz);
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

      /*
			 // find the best dTRef
			 fTRefHits=0;
			 dTRef=0.;     // invalidate old value
			 Double_t dRefChi2=dDoubleMax;
			 for( Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++)
			 {
			 pHit = (CbmTofHit*) fTofHitsColl->At( iHitInd );
			 if (NULL==pHit) continue;
			 Int_t iDetId = (pHit->GetAddress() & SelMask);

			 if( fiBeamRefType == CbmTofAddress::GetSmType( iDetId )){
			 if(fiBeamRefSm   == CbmTofAddress::GetSmId( iDetId ))
			 {
			 Double_t dDT2=0.;
			 Double_t dNT=0.;
			 for (Int_t iSel=0; iSel<iNSel; iSel++){
			 if(BSel[iSel]){
			 dDT2 += TMath::Power(pHit->GetTime()-dTTrig[iSel],2);
			 dNT++;
			 }
			 }
			 if( dNT > 0)
			 if( dDT2/dNT < dRefChi2 )
			 {
			 fTRefHits=1;
			 dTRef = pHit->GetTime();
			 dRefChi2 = dDT2/dNT;
			 }
			 }
			 }
			 }
			 */

      LOG(debug1) << "Generate trigger pattern";
      UInt_t uTriggerPattern = 1;
      if (NULL != fTrbHeader) uTriggerPattern = fTrbHeader->GetTriggerPattern();
      else {
        for (Int_t iSel = 0; iSel < iNSel; iSel++)
          if (BSel[iSel]) {
            uTriggerPattern |= (0x1 << (iSel * 3 + CbmTofAddress::GetRpcId(pTrig[iSel]->GetAddress() & DetMask)));
          }
      }

      LOG(debug1) << "Inspect trigger pattern";
      for (Int_t iSel = 0; iSel < iNSel; iSel++) {
        if (BSel[iSel]) {
          if (dTRef != 0. && fTRefHits > 0) {
            for (UInt_t uChannel = 0; uChannel < 16; uChannel++) {
              if (uTriggerPattern & (0x1 << uChannel)) { fhSeldT[iSel]->Fill(dTTrig[iSel] - dTRef, uChannel); }
            }
          }
        }
      }
    }  // 0<iNSel software triffer check end

    LOG(debug1) << "start filling histos ";

    for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
      pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
      if (NULL == pHit) continue;

      Int_t iDetId = (pHit->GetAddress() & DetMask);
      LOG(debug1) << "Process Hit " << iHitInd << ", DetId " << iDetId;

      std::map<UInt_t, UInt_t>::iterator it = fDetIdIndexMap.find(iDetId);
      if (it == fDetIdIndexMap.end()) continue;  // continue for invalid detector index
      Int_t iDetIndx = it->second;               //fDetIdIndexMap[iDetId];

      Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
      Int_t iSm     = CbmTofAddress::GetSmId(iDetId);
      Int_t iRpc    = CbmTofAddress::GetRpcId(iDetId);
      Int_t iNbRpc  = fDigiBdfPar->GetNbRpc(iSmType);
      if (-1 < fviClusterMul[iSmType][iSm][iRpc]) {
        for (Int_t iSel = 0; iSel < iNSel; iSel++)
          if (BSel[iSel]) {
            Double_t w = fviClusterMul[iSmType][iSm][iRpc];
            if (w == 0.) w = 1.;
            else
              w = 1. / w;
            fhTRpcCluMul[iDetIndx][iSel]->Fill(fviClusterMul[iSmType][iSm][iRpc], w);
          }
      }

      if (fviClusterMul[iSmType][iSm][iRpc] > fiCluMulMax) continue;  // skip this event
      if (iBeamRefMul == 0) break;

      Int_t iChId  = pHit->GetAddress();
      fChannelInfo = fDigiPar->GetCell(iChId);
      Int_t iCh    = CbmTofAddress::GetChannelId(iChId);
      if (NULL == fChannelInfo) {
        LOG(error) << "Invalid Channel Pointer for ChId " << Form(" 0x%08x ", iChId) << ", Ch " << iCh;
        continue;
      }
      /*TGeoNode *fNode=*/  // prepare global->local trafo
      gGeoManager->FindNode(fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());

      LOG(debug1) << "Hit info: "
                  << Form(" 0x%08x %d %f %f %f %f %f %d", iChId, iCh, pHit->GetX(), pHit->GetY(), pHit->GetTime(),
                          fChannelInfo->GetX(), fChannelInfo->GetY(), iHitInd);

      Double_t hitpos[3];
      hitpos[0] = pHit->GetX();
      hitpos[1] = pHit->GetY();
      hitpos[2] = pHit->GetZ();
      Double_t hitpos_local[3];
      TGeoNode* cNode = gGeoManager->GetCurrentNode();
      gGeoManager->MasterToLocal(hitpos, hitpos_local);
      LOG(debug1) << Form(" MasterToLocal for %d, %d%d%d, node %p: "
                          "(%6.1f,%6.1f,%6.1f) ->(%6.1f,%6.1f,%6.1f)",
                          iDetIndx, iSmType, iSm, iRpc, cNode, hitpos[0], hitpos[1], hitpos[2], hitpos_local[0],
                          hitpos_local[1], hitpos_local[2]);
      Bool_t bFillPos = kTRUE;
      //if( fCalMode/10 > 4 && pHit->GetClusterSize() < 3 ) bFillPos=kFALSE;
      if (bFillPos) {
        fhRpcCluPosition[iDetIndx]->Fill((Double_t) iCh,
                                         hitpos_local[1]);  //pHit->GetY()-fChannelInfo->GetY());
        fhSmCluPosition[iSmType]->Fill((Double_t)(iSm * iNbRpc + iRpc), hitpos_local[1]);
      }
      for (Int_t iSel = 0; iSel < iNSel; iSel++)
        if (BSel[iSel]) {
          fhTRpcCluPosition[iDetIndx][iSel]->Fill((Double_t) iCh,
                                                  hitpos_local[1]);  //pHit->GetY()-fChannelInfo->GetY());
          fhTSmCluPosition[iSmType][iSel]->Fill((Double_t)(iSm * iNbRpc + iRpc), hitpos_local[1]);
        }

      if (TMath::Abs(hitpos_local[1]) > fChannelInfo->GetSizey() * fPosYMaxScal) continue;

      Double_t dTimeAna = (pHit->GetTime() - StartAnalysisTime) / 1.E9;
      if (dTRef != 0.) fhRpcCluTimeEvol[iDetIndx]->Fill(dTimeAna, pHit->GetTime() - dTRef);
      fhRpcCluPositionEvol[iDetIndx]->Fill(dTimeAna, hitpos_local[1]);
      //LOG(info) << "Fill TEvol at " << dTimeAna ;

      LOG(debug1) << " TofDigiMatchColl entries:" << fTofDigiMatchColl->GetEntriesFast();

      if (iHitInd > fTofDigiMatchColl->GetEntriesFast()) {
        LOG(error) << " Inconsistent DigiMatches for Hitind " << iHitInd
                   << ", TClonesArraySize: " << fTofDigiMatchColl->GetEntriesFast();
      }

      CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
      LOG(debug1) << " got " << digiMatch->GetNofLinks() << " matches for iCh " << iCh << " at iHitInd " << iHitInd;

      fhRpcCluSize[iDetIndx]->Fill((Double_t) iCh, digiMatch->GetNofLinks() / 2.);

      for (Int_t iSel = 0; iSel < iNSel; iSel++)
        if (BSel[iSel]) {
          fhTRpcCluSize[iDetIndx][iSel]->Fill((Double_t) iCh, digiMatch->GetNofLinks() / 2.);
          if (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 1) {  // check for previous hits in memory time interval
            std::list<CbmTofHit*>::iterator itL = fvLastHits[iSmType][iSm][iRpc][iCh].end();
            itL--;
            for (size_t iH = 0; iH < fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1; iH++) {
              itL--;
              fhTRpcCluSizeDTLastHits[iDetIndx][iSel]->Fill(TMath::Log10(pHit->GetTime() - (*itL)->GetTime()),
                                                            digiMatch->GetNofLinks() / 2.);
            }
          }
        }

      Double_t TotSum = 0.;
      for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink++) {  // loop over digis
        CbmLink L0      = digiMatch->GetLink(iLink);                      //vDigish.at(ivDigInd);
        UInt_t iDigInd0 = L0.GetIndex();
        //         if (iDigInd0 < fTofCalDigisColl->GetEntriesFast()){
        if (iDigInd0 < fTofCalDigiVec->size()) {
          CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
          //         CbmTofDigi *pDig0 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
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
      //Double_t dDist=0.;

      for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink += 2) {  // loop over digis
        CbmLink L0      = digiMatch->GetLink(iLink);                         //vDigish.at(ivDigInd);
        UInt_t iDigInd0 = L0.GetIndex();
        UInt_t iDigInd1 = (digiMatch->GetLink(iLink + 1)).GetIndex();  //vDigish.at(ivDigInd+1);
        //LOG(debug1)<<" " << iDigInd0<<", "<<iDigInd1;

        //       if (iDigInd0 < fTofCalDigisColl->GetEntriesFast() && iDigInd1 < fTofCalDigisColl->GetEntriesFast()){
        if (iDigInd0 < fTofCalDigiVec->size() && iDigInd1 < fTofCalDigiVec->size()) {
          //         CbmTofDigi *pDig0 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd0));
          //         CbmTofDigi *pDig1 = (CbmTofDigi*) (fTofCalDigisColl->At(iDigInd1));
          CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
          CbmTofDigi* pDig1 = &(fTofCalDigiVec->at(iDigInd1));
          if ((Int_t) pDig0->GetType() != iSmType) {
            LOG(error) << Form(" Wrong Digi SmType for Tofhit %d in iDetIndx "
                               "%d, Ch %d with %3.0f strips at Indx %d, %d",
                               iHitInd, iDetIndx, iCh, dNstrips, iDigInd0, iDigInd1);
          }
          LOG(debug1) << " fhRpcCluTot:  Digi 0 " << iDigInd0 << ": Ch " << pDig0->GetChannel() << ", Side "
                      << pDig0->GetSide() << ", StripSide " << (Double_t) iCh * 2. + pDig0->GetSide() << " Digi 1 "
                      << iDigInd1 << ": Ch " << pDig1->GetChannel() << ", Side " << pDig1->GetSide() << ", StripSide "
                      << (Double_t) iCh * 2. + pDig1->GetSide() << ", Tot0 " << pDig0->GetTot() << ", Tot1 "
                      << pDig1->GetTot();

          fhRpcCluTot[iDetIndx]->Fill(pDig0->GetChannel() * 2. + pDig0->GetSide(), pDig0->GetTot());
          fhRpcCluTot[iDetIndx]->Fill(pDig1->GetChannel() * 2. + pDig1->GetSide(), pDig1->GetTot());

          Int_t iCh0 = pDig0->GetChannel();
          Int_t iCh1 = pDig1->GetChannel();
          Int_t iS0  = pDig0->GetSide();
          Int_t iS1  = pDig1->GetSide();
          if (iCh0 != iCh1 || iS0 == iS1) {
            LOG(error) << Form(" MT2 for Tofhit %d in iDetIndx %d, Ch %d from "
                               "in event %f, ",
                               iHitInd, iDetIndx, iCh, fdEvent)
                       << Form(" Dig0: Ind %d, Ch %d, Side %d, T: %6.1f ", iDigInd0, iCh0, iS0, pDig0->GetTime())
                       << Form(" Dig1: Ind %d, Ch %d, Side %d, T: %6.1f ", iDigInd1, iCh1, iS1, pDig1->GetTime());
            continue;
          }

          if (0 > iCh0 || fDigiBdfPar->GetNbChan(iSmType, iRpc) <= iCh0) {
            LOG(error) << Form(" Wrong Digi for Tofhit %d in iDetIndx %d, Ch %d at Indx %d, %d "
                               "from %3.0f strips:  %d, %d, %d, %d",
                               iHitInd, iDetIndx, iCh, iDigInd0, iDigInd1, dNstrips, iCh0, iCh1, iS0, iS1);
            continue;
          }

          if (digiMatch->GetNofLinks() > 2)  //&& digiMatch->GetNofLinks()<8 ) // FIXME: hardcoded limits on CluSize
          {
            dNstrips += 1.;
            dMeanTimeSquared += TMath::Power(0.5 * (pDig0->GetTime() + pDig1->GetTime()) - pHit->GetTime(), 2);
            //             fhRpcCluAvWalk[iDetIndx]->Fill(0.5*(pDig0->GetTot()+pDig1->GetTot()),
            //                        0.5*(pDig0->GetTime()+pDig1->GetTime())-pHit->GetTime());

            fhRpcCluAvLnWalk[iDetIndx]->Fill(TMath::Log10(0.5 * (pDig0->GetTot() + pDig1->GetTot())),
                                             0.5 * (pDig0->GetTime() + pDig1->GetTime()) - pHit->GetTime());

            Double_t dTotWeigth = (pDig0->GetTot() + pDig1->GetTot()) / TotSum;
            Double_t dCorWeigth = 1. - dTotWeigth;

            fhRpcCluDelTOff[iDetIndx]->Fill(
              pDig0->GetChannel(), dCorWeigth * (0.5 * (pDig0->GetTime() + pDig1->GetTime()) - pHit->GetTime()));

            Double_t dDelPos = 0.5 * (pDig0->GetTime() - pDig1->GetTime()) * fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);
            if (0 == pDig0->GetSide()) dDelPos *= -1.;
            fhRpcCluDelPos[iDetIndx]->Fill(pDig0->GetChannel(), dCorWeigth * (dDelPos - hitpos_local[1]));

            fhRpcCluWalk[iDetIndx][iCh0][iS0]->Fill(
              pDig0->GetTot(),
              pDig0->GetTime()
                - (pHit->GetTime()
                   - (1. - 2. * pDig0->GetSide()) * hitpos_local[1] / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)));

            fhRpcCluWalk[iDetIndx][iCh1][iS1]->Fill(
              pDig1->GetTot(),
              pDig1->GetTime()
                - (pHit->GetTime()
                   - (1. - 2. * pDig1->GetSide()) * hitpos_local[1] / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)));

            fhRpcCluAvWalk[iDetIndx]->Fill(pDig0->GetTot(), pDig0->GetTime()
                                                              - (pHit->GetTime()
                                                                 - (1. - 2. * pDig0->GetSide()) * hitpos_local[1]
                                                                     / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)));
            fhRpcCluAvWalk[iDetIndx]->Fill(pDig1->GetTot(), pDig1->GetTime()
                                                              - (pHit->GetTime()
                                                                 - (1. - 2. * pDig1->GetSide()) * hitpos_local[1]
                                                                     / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)));
          }  // end of Clustersize > 1 condition

          LOG(debug1) << " fhTRpcCluTot: Digi 0 " << iDigInd0 << ": Ch " << pDig0->GetChannel() << ", Side "
                      << pDig0->GetSide() << ", StripSide " << (Double_t) iCh * 2. + pDig0->GetSide() << " Digi 1 "
                      << iDigInd1 << ": Ch " << pDig1->GetChannel() << ", Side " << pDig1->GetSide() << ", StripSide "
                      << (Double_t) iCh * 2. + pDig1->GetSide();

          for (Int_t iSel = 0; iSel < iNSel; iSel++)
            if (BSel[iSel]) {
              if (NULL == pHit || NULL == pTrig[iSel]) {
                LOG(info) << " invalid pHit, iSel " << iSel << ", iDetIndx " << iDetIndx;
                break;
              }
              if (pHit->GetAddress() == pTrig[iSel]->GetAddress()) continue;

              fhTRpcCluTot[iDetIndx][iSel]->Fill(pDig0->GetChannel() * 2. + pDig0->GetSide(), pDig0->GetTot());
              fhTRpcCluTot[iDetIndx][iSel]->Fill(pDig1->GetChannel() * 2. + pDig1->GetSide(), pDig1->GetTot());
              if (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 1) {  // check for previous hits in memory time interval
                std::list<CbmTofHit*>::iterator itL = fvLastHits[iSmType][iSm][iRpc][iCh].end();
                itL--;
                for (size_t iH = 0; iH < fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1; iH++) {
                  itL--;
                  fhTRpcCluTotDTLastHits[iDetIndx][iSel]->Fill(TMath::Log10(pHit->GetTime() - (*itL)->GetTime()),
                                                               pDig0->GetTot());
                  fhTRpcCluTotDTLastHits[iDetIndx][iSel]->Fill(TMath::Log10(pHit->GetTime() - (*itL)->GetTime()),
                                                               pDig1->GetTot());
                }
              }
              if (iLink == 0) {  // Fill histo only once (for 1. digi entry)
                if (fEnableMatchPosScaling) dzscal = pHit->GetZ() / pTrig[iSel]->GetZ();
                fhTRpcCludXdY[iDetIndx][iSel]->Fill(pHit->GetX() - dzscal * pTrig[iSel]->GetX(),
                                                    pHit->GetY() - dzscal * pTrig[iSel]->GetY());
                dZsign[iSel] = 1.;
                if (pHit->GetZ() < pTrig[iSel]->GetZ()) dZsign[iSel] = -1.;
              }
              //// look for geometrical match  with selector hit
              if (iSmType == fiBeamRefType  // to get entries in diamond/BeamRef histos
                  || TMath::Sqrt(TMath::Power(pHit->GetX() - dzscal * pTrig[iSel]->GetX(), 2.)
                                 + TMath::Power(pHit->GetY() - dzscal * pTrig[iSel]->GetY(), 2.))
                       < fdCaldXdYMax) {
                if (!fEnableMatchPosScaling && dSel2dXdYMin[iSel] < 1.E300)
                  if (TMath::Sqrt(
                        TMath::Power(pHit->GetX()
                                       - (pTrig[iSel]->GetX() + ddXdZ[iSel] * (pHit->GetZ() - (pTrig[iSel]->GetZ()))),
                                     2.)
                        + TMath::Power(pHit->GetY()
                                         - (pTrig[iSel]->GetY() + ddYdZ[iSel] * (pHit->GetZ() - (pTrig[iSel]->GetZ()))),
                                       2.))
                      > 0.5 * fdCaldXdYMax)
                    continue;      // refine position selection cut in cosmic measurement
                dTcor[iSel] = 0.;  // precaution
                if (dTRef != 0.
                    && TMath::Abs(dTRef - dTTrig[iSel]) < fdDelTofMax) {  // correct times for DelTof - velocity spread
                  if (iLink == 0) {  // do calculations only once (at 1. digi entry) // interpolate!
                    // calculate spatial distance to trigger hit
                    /*
										 dDist=TMath::Sqrt(TMath::Power(pHit->GetX()-pTrig[iSel]->GetX(),2.)
										 +TMath::Power(pHit->GetY()-pTrig[iSel]->GetY(),2.)
										 +TMath::Power(pHit->GetZ()-pTrig[iSel]->GetZ(),2.));
										 */
                    // determine correction value
                    //if(fiBeamRefAddr  != iDetId) // do not do this for reference counter itself
                    if (fTRefMode < 11)  // do not do this for trigger counter itself
                    {
                      Double_t dTentry = dTRef - dTTrig[iSel] + fdDelTofMax;
                      Int_t iBx        = dTentry / 2. / fdDelTofMax * nbClDelTofBinX;
                      if (iBx < 0) iBx = 0;
                      if (iBx > nbClDelTofBinX - 1) iBx = nbClDelTofBinX - 1;
                      Double_t dBinWidth = 2. * fdDelTofMax / nbClDelTofBinX;
                      Double_t dDTentry  = dTentry - ((Double_t) iBx) * dBinWidth;
                      Int_t iBx1         = 0;
                      dDTentry < 0 ? iBx1 = iBx - 1 : iBx1 = iBx + 1;
                      Double_t w0 = 1. - TMath::Abs(dDTentry) / dBinWidth;
                      Double_t w1 = 1. - w0;
                      if (iBx1 < 0) iBx1 = 0;
                      if (iBx1 > nbClDelTofBinX - 1) iBx1 = nbClDelTofBinX - 1;
                      dDelTof = fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] * w0
                                + fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx1][iSel] * w1;
                      //dDelTof *= dDist; // has to be consistent with fhTRpcCluDelTof filling
                      LOG(debug1) << Form(" DelTof for SmT %d, Sm %d, R %d, T %d, dTRef "
                                          "%6.1f, Bx %d, Bx1 %d, DTe %6.1f -> DelT %6.1f",
                                          iSmType, iSm, iRpc, iSel, dTRef - dTTrig[iSel], iBx, iBx1, dDTentry, dDelTof);
                    }
                    dTTcor[iSel] = dDelTof * dZsign[iSel];
                    dTcor[iSel]  = pHit->GetTime() - dDelTof - dTTrig[iSel];
                    // Double_t dAvTot=0.5*(pDig0->GetTot()+pDig1->GetTot());   (VF) not used
                  }  // if(iLink==0)
                  if (dTcor[iSel] == 0.) continue;
                  LOG(debug) << Form(" TRpcCluWalk for Ev %d, Link %d(%d), Sel %d, TSR %d%d%d, "
                                     "Ch %d,%d, S %d,%d T %f, DelTof %6.1f, W-ent:  %6.0f,%6.0f",
                                     fiNevtBuild, iLink, (Int_t) digiMatch->GetNofLinks(), iSel, iSmType, iSm, iRpc,
                                     iCh0, iCh1, iS0, iS1, dTTrig[iSel], dDelTof,
                                     fhTRpcCluWalk[iDetIndx][iSel][iCh0][iS0]->GetEntries(),
                                     fhTRpcCluWalk[iDetIndx][iSel][iCh1][iS1]->GetEntries());

                  if (fhTRpcCluWalk[iDetIndx][iSel][iCh0][iS0]->GetEntries()
                      != fhTRpcCluWalk[iDetIndx][iSel][iCh1][iS1]->GetEntries())
                    LOG(error) << Form(" Inconsistent walk histograms -> debugging "
                                       "necessary ... for %d, %d, %d, %d, %d, %d, %d ",
                                       fiNevtBuild, iDetIndx, iSel, iCh0, iCh1, iS0, iS1);

                  LOG(debug1) << Form(
                    " TRpcCluWalk values side %d: %f, %f, side %d:  %f, %f ", iS0, pDig0->GetTot(),
                    pDig0->GetTime()
                      + ((1. - 2. * pDig0->GetSide()) * hitpos_local[1] / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc))
                      - dTTcor[iSel] - dTTrig[iSel],
                    iS1, pDig1->GetTot(),
                    pDig1->GetTime()
                      + ((1. - 2. * pDig1->GetSide()) * hitpos_local[1] / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc))
                      - dTTcor[iSel] - dTTrig[iSel]);

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

                  fhTRpcCluWalk2[iDetIndx][iSel]->Fill(pDig0->GetTot(), pDig1->GetTot(), dTcor[iSel]);

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
                    //fhTRpcCluDelTof[iDetIndx][iSel]->Fill(dTRef-dTTrig[iSel],dTcor[iSel]/dDist);
                    fhTRpcCluDelTof[iDetIndx][iSel]->Fill(dTRef - dTTrig[iSel], dTcor[iSel]);
                    fhTSmCluTOff[iSmType][iSel]->Fill((Double_t)(iSm * iNbRpc + iRpc), dTcor[iSel]);
                    fhTSmCluTRun[iSmType][iSel]->Fill(fdEvent, dTcor[iSel]);
                    if (iDetId
                        != (pTrig[iSel]->GetAddress()
                            & DetMask)) {  // transform matched hit-pair back into detector frame
                      hitpos[0] = pHit->GetX() - dzscal * pTrig[iSel]->GetX() + fChannelInfo->GetX();
                      hitpos[1] = pHit->GetY() - dzscal * pTrig[iSel]->GetY() + fChannelInfo->GetY();
                      hitpos[2] = pHit->GetZ();
                      gGeoManager->MasterToLocal(hitpos,
                                                 hitpos_local);  //  transform into local frame
                      fhRpcCluDelMatPos[iDetIndx]->Fill((Double_t) iCh, hitpos_local[1]);
                      fhRpcCluDelMatTOff[iDetIndx]->Fill((Double_t) iCh,
                                                         (pHit->GetTime() - dTTrig[iSel]) - dTTcor[iSel]);
                    }
                  }  // iLink==0 condition end
                }    // position condition end
              }      // Match condition end
            }        // closing of selector loop
        }            // digi index range check condition end
        else {
          LOG(error) << "CbmTofEventClusterizer::FillHistos: invalid digi index " << iDetIndx << " digi0,1" << iDigInd0
                     << ", " << iDigInd1
                     << " - max:"
                     //                       << fTofCalDigisColl->GetEntriesFast()
                     << fTofCalDigiVec->size()
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
                           iHitInd, iDetIndx, iCh, dNstrips, dTrms);
        fhRpcCluTrms[iDetIndx]->Fill((Double_t) iCh, dTrms);
        pHit->SetTimeError(dTrms);
      }

      LOG(debug1) << " Fill Time of iDetIndx " << iDetIndx << ", hitAddr "
                  << Form(" %08x, y = %5.2f", pHit->GetAddress(), hitpos_local[1]) << " for |y| <"
                  << fhRpcCluPosition[iDetIndx]->GetYaxis()->GetXmax();

      if (TMath::Abs(hitpos_local[1]) < (fhRpcCluPosition[iDetIndx]->GetYaxis()->GetXmax())) {
        if (dTRef != 0. && fTRefHits == 1) {
          fhRpcCluTOff[iDetIndx]->Fill((Double_t) iCh, pHit->GetTime() - dTRef);
          fhSmCluTOff[iSmType]->Fill((Double_t)(iSm * iNbRpc + iRpc), pHit->GetTime() - dTRef);

          for (Int_t iSel = 0; iSel < iNSel; iSel++)
            if (BSel[iSel]) {
              LOG(debug1) << " TRpcCluTOff " << iDetIndx << ", Sel " << iSel
                          << Form(", Dt %7.3f, LHsize %lu ", pHit->GetTime() - dTTrig[iSel],
                                  fvLastHits[iSmType][iSm][iRpc][iCh].size());
              if (pHit->GetAddress() == pTrig[iSel]->GetAddress()) continue;

              if (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 1) {  // check for previous hits in memory time interval
                std::list<CbmTofHit*>::iterator itL = fvLastHits[iSmType][iSm][iRpc][iCh].end();
                itL--;
                for (size_t iH = 0; iH < fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1; iH++) {
                  itL--;
                  LOG(debug1) << Form(" %f,", pHit->GetTime() - (*itL)->GetTime());
                }
              }
              // fill Time Offset histograms without velocity spread (DelTof) correction
              if (pBeamRef != NULL)
                if (TMath::Abs(pBeamRef->GetTime() - pTrig[iSel]->GetTime()) < fdDelTofMax) {
                  //	     if(TMath::Abs(pBeamRef->GetTime()-pTrig[iSel]->GetTime()) < fhTRpcCluTOff[iIndexDut][iSel]->GetYaxis()->GetXmax()) {
                  /*
									 if(  iSmType==fiBeamRefType  ||
									 TMath::Sqrt(TMath::Power(pHit->GetX()-dzscal*pTrig[iSel]->GetX(),2.)
									 +TMath::Power(pHit->GetY()-dzscal*pTrig[iSel]->GetY(),2.))<fdCaldXdYMax
									 * fhTRpcCluTOff[iIndexDut][iSel]->GetYaxis()->GetXmax())
									 */
                  fhTRpcCluTOff[iDetIndx][iSel]->Fill((Double_t) iCh,
                                                      pHit->GetTime()
                                                        - dTTrig[iSel]);  // -dTTcor[iSel] only valid for matches
                  if (digiMatch->GetNofLinks() > 0)
                    fhTRpcCluTofOff[iDetIndx][iSel]->Fill((Double_t) iCh,
                                                          pHit->GetTime()
                                                            - dTTrig[iSel]);  // valid for beam experiments
                  //  pHit->GetTime()-pBeamRef->GetTime());  // shift cluster time to beamcounter  time
                  // pHit->GetTime()-pBeamRef->GetTime()-fdToDAv*pTrig[iSel]->GetR());// valid for beam experiments
                }

              if (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 1) {  // check for previous hits in memory time interval
                std::list<CbmTofHit*>::iterator itL = fvLastHits[iSmType][iSm][iRpc][iCh].end();
                itL--;
                for (Int_t iH = 0; iH < 1; iH++) {  // use only last hit
                  //  for(Int_t iH=0; iH<fvLastHits[iSmType][iSm][iRpc][iCh].size()-1; iH++){//fill for all memorized hits
                  itL--;
                  Double_t dTsinceLast = pHit->GetTime() - (*itL)->GetTime();
                  if (dTsinceLast > fdMemoryTime)
                    LOG(fatal) << Form("Invalid Time since last hit on channel "
                                       "TSRC %d%d%d%d: %f > %f",
                                       iSmType, iSm, iRpc, iCh, dTsinceLast, fdMemoryTime);

                  fhTRpcCluTOffDTLastHits[iDetIndx][iSel]->Fill(TMath::Log10(dTsinceLast),
                                                                pHit->GetTime() - dTTrig[iSel]);
                  fhTRpcCluMemMulDTLastHits[iDetIndx][iSel]->Fill(TMath::Log10(dTsinceLast),
                                                                  fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1);
                }
              }
            }
        }
      }
    }  // iHitInd hit loop end

    for (Int_t iSmType = 0; iSmType < fDigiBdfPar->GetNbSmTypes(); iSmType++) {
      for (Int_t iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
        LOG(debug1) << "CbmTofEventClusterizer::FillHistos:  "
                    << Form(" %3d %3d %3lu ", iSmType, iRpc, fviClusterSize[iSmType][iRpc].size());

        for (UInt_t uCluster = 0; uCluster < fviClusterSize[iSmType][iRpc].size(); uCluster++) {
          LOG(debug2) << "CbmTofEventClusterizer::FillHistos:  " << Form(" %3d %3d %3d ", iSmType, iRpc, uCluster);

          fhClusterSize->Fill(fviClusterSize[iSmType][iRpc][uCluster]);
          fhClusterSizeType->Fill(fviClusterSize[iSmType][iRpc][uCluster],
                                  40 * iSmType + iRpc);  //FIXME - hardwired constant
          if (kFALSE)                                    // kTRUE == fDigiBdfPar->ClustUseTrackId() )
          {
            fhTrackMul->Fill(fviTrkMul[iSmType][iRpc][uCluster]);
            fhClusterSizeMulti->Fill(fviClusterSize[iSmType][iRpc][uCluster], fviTrkMul[iSmType][iRpc][uCluster]);
            if (1 == fviTrkMul[iSmType][iRpc][uCluster])
              fhTrk1MulPos->Fill(fvdX[iSmType][iRpc][uCluster], fvdY[iSmType][iRpc][uCluster]);
            if (1 < fviTrkMul[iSmType][iRpc][uCluster])
              fhHiTrkMulPos->Fill(fvdX[iSmType][iRpc][uCluster], fvdY[iSmType][iRpc][uCluster]);
            fhAllTrkMulPos->Fill(fvdX[iSmType][iRpc][uCluster], fvdY[iSmType][iRpc][uCluster]);
          }            // if( kTRUE == fDigiBdfPar->ClustUseTrackId() )
          if (kFALSE)  //  1 == fviTrkMul[iSmType][iRpc][uCluster] )
          {
            fhClustSizeDifX->Fill(fviClusterSize[iSmType][iRpc][uCluster], fvdDifX[iSmType][iRpc][uCluster]);
            fhClustSizeDifY->Fill(fviClusterSize[iSmType][iRpc][uCluster], fvdDifY[iSmType][iRpc][uCluster]);
            if (1 == fviClusterSize[iSmType][iRpc][uCluster]) {
              fhChDifDifX->Fill(fvdDifCh[iSmType][iRpc][uCluster], fvdDifX[iSmType][iRpc][uCluster]);
              fhChDifDifY->Fill(fvdDifCh[iSmType][iRpc][uCluster], fvdDifY[iSmType][iRpc][uCluster]);
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
  }  // if(0<iNbTofHits) end

  return kTRUE;
}

Bool_t CbmTofEventClusterizer::WriteHistos()
{
  if (fDutId < 0) return kTRUE;
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;
  TFile* fHist;
  fHist = new TFile(fOutHstFileName, "RECREATE");
  fHist->cd();
  Double_t TBeamRefMean = 0.;  // weighted mean of all BeamRef counter channels
  for (Int_t iFindT0 = 0; iFindT0 < 2; iFindT0++)
    for (Int_t iDetIndx = 0; iDetIndx < fDigiBdfPar->GetNbDet(); iDetIndx++) {
      if (NULL == fhRpcCluMul[iDetIndx]) continue;

      Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);

      if (iFindT0 == 0 && fiBeamRefAddr != iUniqueId) continue;

      Int_t iSmAddr = iUniqueId & SelMask;
      Int_t iSmType = CbmTofAddress::GetSmType(iUniqueId);
      Int_t iSm     = CbmTofAddress::GetSmId(iUniqueId);
      Int_t iRpc    = CbmTofAddress::GetRpcId(iUniqueId);
      fCalSmAddr &= SelMask;

      Int_t iNent = 0;
      if (fCalSel > -1) {
        if (NULL == fhTRpcCluAvWalk[iDetIndx][fCalSel]) continue;
        iNent = (Int_t) fhTRpcCluAvWalk[iDetIndx][fCalSel]->GetEntries();
      }
      else {
        if (NULL == fhRpcCluAvWalk[iDetIndx]) continue;
        iNent = (Int_t) fhRpcCluAvWalk[iDetIndx]->GetEntries();
      }
      if (0 == iNent) {
        LOG(debug) << "WriteHistos: No entries in Walk histos for "
                   << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc;
        // continue;
      }

      //     if(-1<fCalSmAddr && fcalType != iSmAddr) continue;
      TH2* htempPos           = NULL;
      TProfile* htempPos_pfx  = NULL;
      TH1* htempPos_py        = NULL;
      TProfile* htempTOff_pfx = NULL;
      TH1* htempTOff_px       = NULL;
      TProfile* hAvPos_pfx    = NULL;
      TProfile* hAvTOff_pfx   = NULL;
      TH2* htempTOff          = NULL;  // -> Comment to remove warning because set but never used
      TH2* htempTot           = NULL;
      TProfile* htempTot_pfx  = NULL;
      TH1* htempTot_Mean      = NULL;
      TH1* htempTot_Off       = NULL;

      if (-1 < fCalSel) {
        htempPos     = fhRpcCluPosition[iDetIndx];  // use untriggered distributions for position
        htempPos_pfx = fhRpcCluPosition[iDetIndx]->ProfileX("_pfx", 1, fhRpcCluPosition[iDetIndx]->GetNbinsY());
        //htempPos      = fhTRpcCluPosition[iDetIndx][fCalSel];
        //htempPos_pfx  = fhTRpcCluPosition[iDetIndx][fCalSel]->ProfileX("_pfx",1,fhTRpcCluPosition[iDetIndx][fCalSel]->GetNbinsY());
        htempTOff = fhTRpcCluTOff[iDetIndx][fCalSel];
        if (fIdMode == 1) htempTOff = fhTRpcCluTofOff[iDetIndx][fCalSel];  //DEV! for init_calib_all
        htempTOff_pfx = htempTOff->ProfileX("_pfx", 1, fhTRpcCluTOff[iDetIndx][fCalSel]->GetNbinsY());
        htempTOff_px  = htempTOff->ProjectionX("_px", 1, fhTRpcCluTOff[iDetIndx][fCalSel]->GetNbinsY());
        for (Int_t iCh = 0; iCh < htempTOff->GetNbinsX(); iCh++) {  // use peak value to prevent out of update range
          TH1* htempTOff_py = htempTOff->ProjectionY("_py", iCh + 1, iCh + 1);
          Double_t Ymax     = htempTOff_py->GetMaximum();
          if (Ymax > 0.) {
            Int_t iBmax       = htempTOff_py->GetMaximumBin();
            Double_t dTOffmax = htempTOff_py->GetXaxis()->GetBinCenter(iBmax);
            if (TMath::Abs(dTOffmax) > 0.3 * htempTOff_py->GetXaxis()->GetXmax()) {
              LOG(debug) << "Use Maximum of TOff in ch " << iCh << " of histo " << htempTOff->GetName() << ": "
                         << dTOffmax << ", " << htempTOff_py->GetXaxis()->GetXmax() << " instead of "
                         << htempTOff_pfx->GetBinContent(iCh + 1);
              htempTOff_pfx->SetBinContent(iCh + 1, dTOffmax);
              htempTOff_pfx->SetBinEntries(iCh + 1, 1);
            }
          }
        }
        htempTot = fhTRpcCluTot[iDetIndx][fCalSel];
        htempTot_pfx =
          fhTRpcCluTot[iDetIndx][fCalSel]->ProfileX("_pfx", 1, fhTRpcCluTot[iDetIndx][fCalSel]->GetNbinsY());
        hAvPos_pfx =
          fhTSmCluPosition[iSmType][fCalSel]->ProfileX("_pfx", 1, fhTSmCluPosition[iSmType][fCalSel]->GetNbinsY());
        hAvTOff_pfx =
          fhTSmCluTOff[iSmType][fCalSel]->ProfileX("_pfx", 1, fhTSmCluTOff[iSmType][fCalSel]->GetNbinsY(), "s");
      }
      else  // all triggers
      {
        htempPos     = fhRpcCluPosition[iDetIndx];
        htempTot     = fhRpcCluTot[iDetIndx];
        htempTot_pfx = fhRpcCluTot[iDetIndx]->ProfileX("_pfx", 1, fhRpcCluTot[iDetIndx]->GetNbinsY());
        hAvPos_pfx   = fhSmCluPosition[iSmType]->ProfileX("_pfx", 1, fhSmCluPosition[iSmType]->GetNbinsY());
        hAvTOff_pfx  = fhSmCluTOff[iSmType]->ProfileX("_pfx", 1, fhSmCluTOff[iSmType]->GetNbinsY());
        switch (fCalSel) {
          case -1:  // take corrections from untriggered distributions
            htempPos_pfx = fhRpcCluPosition[iDetIndx]->ProfileX("_pfx", 1, fhRpcCluPosition[iDetIndx]->GetNbinsY());
            //       htempTOff     = fhRpcCluTOff[iDetIndx]; // -> Comment to remove warning because set but never used
            htempTOff_pfx = fhRpcCluTOff[iDetIndx]->ProfileX("_pfx", 1, fhRpcCluTOff[iDetIndx]->GetNbinsY(), "s");
            htempTOff_px  = fhRpcCluTOff[iDetIndx]->ProjectionX("_px", 1, fhRpcCluTOff[iDetIndx]->GetNbinsY());
            break;

          case -2:  //take corrections from Cluster deviations
            htempPos_pfx = fhRpcCluDelPos[iDetIndx]->ProfileX("_pfx", 1, fhRpcCluDelPos[iDetIndx]->GetNbinsY());
            //         htempTOff     = fhRpcCluDelTOff[iDetIndx]; // -> Comment to remove warning because set but never used
            htempTOff_pfx = fhRpcCluDelTOff[iDetIndx]->ProfileX("_pfx", 1, fhRpcCluDelTOff[iDetIndx]->GetNbinsY());
            htempTOff_px  = fhRpcCluDelTOff[iDetIndx]->ProjectionX("_px", 1, fhRpcCluDelTOff[iDetIndx]->GetNbinsY());
            break;

          case -3:  // take corrections from deviations to matched trigger hit
            htempPos_pfx = fhRpcCluDelMatPos[iDetIndx]->ProfileX("_pfx", 1, fhRpcCluDelMatPos[iDetIndx]->GetNbinsY());
            //           htempTOff     = fhRpcCluDelMatTOff[iDetIndx]; // -> Comment to remove warning because set but never used
            htempTOff_pfx =
              fhRpcCluDelMatTOff[iDetIndx]->ProfileX("_pfx", 1, fhRpcCluDelMatTOff[iDetIndx]->GetNbinsY());
            htempTOff_px =
              fhRpcCluDelMatTOff[iDetIndx]->ProjectionX("_px", 1, fhRpcCluDelMatTOff[iDetIndx]->GetNbinsY());
            break;

          case -4:  // shift all detectors without match requirement to beam counter times
          {
            Int_t iCalSel = 0;
            htempPos_pfx  = fhTRpcCluPosition[iDetIndx][iCalSel]->ProfileX(
              "_pfx", 1, fhTRpcCluPosition[iDetIndx][iCalSel]->GetNbinsY());
            htempTOff_pfx = fhTRpcCluTOff[iDetIndx][iCalSel]->ProfileX(
              "_pfx", 1, fhTRpcCluTofOff[iDetIndx][iCalSel]->GetNbinsY(), "s");
            htempTOff_px = fhTRpcCluTofOff[iDetIndx][iCalSel]->ProjectionX(
              "_px", 1, fhTRpcCluTofOff[iDetIndx][iCalSel]->GetNbinsY());
          } break;

          case -5:  // shift all detectors without match requirement to beam counter times
          {
            Int_t iCalSel = 1;
            htempPos_pfx  = fhTRpcCluPosition[iDetIndx][iCalSel]->ProfileX(
              "_pfx", 1, fhTRpcCluPosition[iDetIndx][iCalSel]->GetNbinsY());
            htempTOff_pfx = fhTRpcCluTOff[iDetIndx][iCalSel]->ProfileX(
              "_pfx", 1, fhTRpcCluTofOff[iDetIndx][iCalSel]->GetNbinsY(), "s");
            htempTOff_px = fhTRpcCluTofOff[iDetIndx][iCalSel]->ProjectionX(
              "_px", 1, fhTRpcCluTofOff[iDetIndx][iCalSel]->GetNbinsY());
          } break;
        }
      }

      if (NULL == htempPos_pfx) {
        LOG(debug) << "WriteHistos: Projections not available, continue ";
        continue;
      }

      htempTot_Mean = htempTot_pfx->ProjectionX("_Mean");
      htempTot_Off  = htempTot_pfx->ProjectionX("_Off");

      htempPos_pfx->SetName(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Pos_pfx", iSmType, iSm, iRpc));
      htempTOff_pfx->SetName(Form("cl_CorSmT%01d_sm%03d_rpc%03d_TOff_pfx", iSmType, iSm, iRpc));
      htempTot_pfx->SetName(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_pfx", iSmType, iSm, iRpc));
      htempTot_Mean->SetName(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Mean", iSmType, iSm, iRpc));
      htempTot_Off->SetName(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Off", iSmType, iSm, iRpc));
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
                           << Form("SmT%01d_sm%03d_rpc%03d bin%d: mean %f, Off %f", iSmType, iSm, iRpc, i, Xmean, XOff);
            }
            htempTot_Mean->SetBinContent(i + 1, (Xmean - XOff));
            if (htempTot_Mean->GetBinContent(i + 1) != (Xmean - XOff))
              LOG(warning) << "Tot numbers not stored properly for "
                           << Form("SmT%01d_sm%03d_rpc%03d bin%d: mean %f, target %f", iSmType, iSm, iRpc, i,
                                   htempTot_Mean->GetBinContent(i + 1), Xmean - XOff);
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
          LOG(debug) << "WriteHistos: restore Offsets and Gains and save Walk for "
                     << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc
                     << " and calSmAddr = " << Form(" 0x%08x ", TMath::Abs(fCalSmAddr));
          htempPos_pfx->Reset();  //reset to restore means of original histos
          htempTOff_pfx->Reset();
          htempTot_Mean->Reset();
          htempTot_Off->Reset();
          for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
            Double_t YMean =
              fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
              * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            Double_t TMean =
              0.5 * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            htempPos_pfx->Fill(iCh, YMean);
            if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
              LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh << " got "
                         << htempPos_pfx->GetBinContent(iCh) << "," << htempPos_pfx->GetBinContent(iCh + 1) << ","
                         << htempPos_pfx->GetBinContent(iCh + 2) << ", expected " << YMean;
            }
            htempTOff_pfx->Fill(iCh, TMean);

            for (Int_t iSide = 0; iSide < 2; iSide++) {
              htempTot_Mean->SetBinContent(
                iCh * 2 + 1 + iSide,
                fdTTotMean / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);  //nh +1 empirical(?)
              htempTot_Off->SetBinContent(iCh * 2 + 1 + iSide, fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
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
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
            gDirectory->cd(curdir->GetPath());
            if (NULL != hCorDelTof) {
              TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
                Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
              hCorDelTofout->Write();
            }
            else {
              LOG(debug) << " No CorDelTof histo "
                         << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel);
            }
          }

          if ((fCalSmAddr < 0 && TMath::Abs(fCalSmAddr) != iSmAddr)
              || fCalSmAddr == iSmAddr)  // select detectors for determination of walk correction
          {

            LOG(debug) << "WriteHistos: restore Offsets and Gains and update Walk for "
                       << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc << " with "
                       << fDigiBdfPar->GetNbChan(iSmType, iRpc) << " channels";
            for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpc); iCh++) {
              TH2* h2tmp0;
              TH2* h2tmp1;
              if (!fEnableAvWalk) {
                if (-1 < fCalSel) {
                  h2tmp0 = fhTRpcCluWalk[iDetIndx][fCalSel][iCh][0];
                  h2tmp1 = fhTRpcCluWalk[iDetIndx][fCalSel][iCh][1];
                }
                else {  // take correction from deviation within clusters
                  h2tmp0 = fhRpcCluWalk[iDetIndx][iCh][0];
                  h2tmp1 = fhRpcCluWalk[iDetIndx][iCh][1];
                }
              }
              else {  // go for averages (low statistics)
                if (-1 < fCalSel) {
                  h2tmp0 = fhTRpcCluAvWalk[iDetIndx][fCalSel];
                  h2tmp1 = fhTRpcCluAvWalk[iDetIndx][fCalSel];
                }
                else {  // take correction from deviation within clusters
                  h2tmp0 = fhRpcCluAvWalk[iDetIndx];
                  h2tmp1 = fhRpcCluAvWalk[iDetIndx];
                }
              }
              if (NULL == h2tmp0) {
                LOG(debug) << Form("WriteHistos: Walk histo not available for "
                                   "SmT %d, Sm %d, Rpc %d, Ch %d",
                                   iSmType, iSm, iRpc, iCh);
                continue;
              }
              Int_t iNEntries = h2tmp0->GetEntries();
              if (iCh == 0)  // condition to print message only once
                LOG(debug) << Form(" Update Walk correction for SmT %d, Sm %d, "
                                   "Rpc %d, Ch %d, Sel%d: Entries %d",
                                   iSmType, iSm, iRpc, iCh, fCalSel, iNEntries);

              //         h2tmp0->Write();
              //         h2tmp1->Write();
              if (-1 < iNEntries) {  // always done
                TProfile* htmp0  = h2tmp0->ProfileX("_pfx", 1, h2tmp0->GetNbinsY());
                TProfile* htmp1  = h2tmp1->ProfileX("_pfx", 1, h2tmp1->GetNbinsY());
                TH1D* h1tmp0     = h2tmp0->ProjectionX("_px", 1, h2tmp0->GetNbinsY());
                TH1D* h1tmp1     = h2tmp1->ProjectionX("_px", 1, h2tmp1->GetNbinsY());
                TH1D* h1ytmp0    = h2tmp0->ProjectionY("_py", 1,
                                                    nbClWalkBinX);  // preserve means
                TH1D* h1ytmp1    = h2tmp1->ProjectionY("_py", 1, nbClWalkBinX);
                Double_t dWMean0 = h1ytmp0->GetMean();
                Double_t dWMean1 = h1ytmp1->GetMean();
                Double_t dWMean  = 0.5 * (dWMean0 + dWMean1);
                Int_t iWalkUpd   = 2;  // Walk update mode flag
                //if(5==iSmType || 8==iSmType || 2==iSmType) iWalkUpd=0; // keep both sides consistent for diamonds and pads
                if (5 == iSmType || 8 == iSmType)
                  iWalkUpd = 0;  // keep both sides consistent for diamonds and pads (Cern2016)
                for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
                  switch (iWalkUpd) {
                    case 0:
                      if (h1tmp0->GetBinContent(iWx + 1) > WalkNHmin && h1tmp1->GetBinContent(iWx + 1) > WalkNHmin) {
                        // preserve y - position (difference) on average
                        Double_t dWcor =
                          (((TProfile*) htmp0)->GetBinContent(iWx + 1) + ((TProfile*) htmp1)->GetBinContent(iWx + 1))
                          * 0.5;
                        fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx] += dWcor - dWMean;
                        fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx] += dWcor - dWMean;
                        LOG(debug) << Form("Walk for TSR %d%d%d%d Tot %d set to %f", iSmType, iSm, iRpc, iCh, iWx,
                                           fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
                      }
                      break;
                    case 1:
                      if (h1tmp0->GetBinContent(iWx + 1) > WalkNHmin && h1tmp1->GetBinContent(iWx + 1) > WalkNHmin) {
                        Double_t dWcor0 = ((TProfile*) htmp0)->GetBinContent(iWx + 1) - dWMean0;
                        Double_t dWcor1 = ((TProfile*) htmp1)->GetBinContent(iWx + 1) - dWMean1;
                        Double_t dWcor  = 0.5 * (dWcor0 + dWcor1);
                        fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx] += dWcor;  //-dWMean0;
                        fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx] += dWcor;  //-dWMean1;

                        if (iCh == 0 && iSmType == 9 && iSm == 0 && h1tmp0->GetBinContent(iWx + 1) > WalkNHmin)
                          LOG(debug) << "Update Walk Sm = " << iSm << "(" << iNbRpc << "), Rpc " << iRpc << ", Bin "
                                     << iWx << ", " << h1tmp0->GetBinContent(iWx + 1)
                                     << " cts: " << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx] << " + "
                                     << ((TProfile*) htmp0)->GetBinContent(iWx + 1) << " - " << dWMean0 << " -> "
                                     << dWcor - dWMean0
                                     << ", S1: " << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx] << " + "
                                     << ((TProfile*) htmp1)->GetBinContent(iWx + 1) << " - " << dWMean1 << " -> "
                                     << dWcor - dWMean1;
                      }
                      break;
                    case 2:
                      if (h1tmp0->GetBinContent(iWx + 1) > WalkNHmin && h1tmp1->GetBinContent(iWx + 1) > WalkNHmin) {
                        Double_t dWcor0 = ((TProfile*) htmp0)->GetBinContent(iWx + 1) - dWMean0;
                        Double_t dWcor1 = ((TProfile*) htmp1)->GetBinContent(iWx + 1) - dWMean1;
                        //Double_t dWcor = 0.5*(dWcor0 + dWcor1);
                        fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx] += dWcor0;
                        fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx] += dWcor1;
                      }
                      break;

                    default:;
                  }
                }
                h1tmp0->Reset();
                h1tmp1->Reset();
                for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
                  h1tmp0->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
                  h1tmp1->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
                  Int_t iTry = 3;
                  while (iTry-- > 0
                         && h1tmp0->GetBinContent(iWx + 1) != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
                    h1tmp0->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
                  }
                  if (iTry == 0)
                    LOG(error) << "writing not successful for " << h1tmp0->GetName() << ", attempts left: " << iTry
                               << ", iWx " << iWx << ", got " << h1tmp0->GetBinContent(iWx + 1) << ", expected "
                               << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
                  iTry = 3;
                  while (iTry-- > 0
                         && h1tmp1->GetBinContent(iWx + 1) != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]) {
                    h1tmp1->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
                  }
                  if (iTry == 0)
                    LOG(error) << "writing not successful for " << h1tmp1->GetName() << ", attempts left: " << iTry
                               << ", iWx " << iWx << ", got " << h1tmp1->GetBinContent(iWx + 1) << ", expected "
                               << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx];
                }

                h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px", iSmType, iSm, iRpc, iCh));
                h1tmp0->Smooth(iNWalkSmooth);
                h1tmp0->Write();
                h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px", iSmType, iSm, iRpc, iCh));
                h1tmp1->Smooth(iNWalkSmooth);
                h1tmp1->Write();
              }
            }
          }
          else {  // preserve whatever is there for fCalSmAddr !
            for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpc); iCh++)  // restore old values
            {
              // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
              // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
              TH1D* h1tmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX("_px", 1, nbClWalkBinY);
              TH1D* h1tmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX("_px", 1, nbClWalkBinY);
              for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
                h1tmp0->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
                h1tmp1->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
                if (h1tmp0->GetBinContent(iWx + 1) != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
                  LOG(error) << "WriteHistos: restore unsuccessful! iWx " << iWx << " got "
                             << h1tmp0->GetBinContent(iWx + 1) << ", expected "
                             << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
                }
              }
              h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px", iSmType, iSm, iRpc, iCh));
              //          htmp0->Write();
              h1tmp0->Write();
              h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px", iSmType, iSm, iRpc, iCh));
              //          htmp1->Write();
              h1tmp1->Write();
            }
          }
        } break;

        case 2:  //update time offsets from positions and times with Sm averages, save walks and DELTOF
        {
          Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
          Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);

          if ((fCalSmAddr < 0) || (fCalSmAddr != iSmAddr)) {  // select detectors for updating offsets
            LOG(debug) << "WriteHistos: (case 2) update Offsets and keep Gains, "
                          "Walk and DELTOF for "
                       << Form(" 0x%08x ", TMath::Abs(fCalSmAddr)) << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc "
                       << iRpc;
            Int_t iB        = iSm * iNbRpc + iRpc;
            Double_t dVscal = 1.;
            if (0)  //NULL != fhSmCluSvel[iSmType])
              dVscal = fhSmCluSvel[iSmType]->GetBinContent(iSm * iNbRpc + iRpc + 1);
            if (dVscal == 0.) dVscal = 1.;

            Double_t YMean = ((TProfile*) hAvPos_pfx)->GetBinContent(iB + 1);  //nh +1 empirical(?)
            htempPos_py    = htempPos->ProjectionY(Form("%s_py", htempPos->GetName()), 1, iNbCh);
            if (htempPos_py->GetEntries() > fdYFitMin && fPosYMaxScal < 1.1) {
              LOG(debug1) << Form("Determine YMean in %s by fit to %d entries", htempPos->GetName(),
                                  (Int_t) htempPos_py->GetEntries());
              CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, 0);
              Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
              fChannelInfo = fDigiPar->GetCell(iChId);
              if (NULL == fChannelInfo) {
                LOG(warning) << Form("invalid ChannelInfo for 0x%08x", iChId);
                continue;
              }
              fit_ybox(htempPos_py, fChannelInfo->GetSizey());
              TF1* ff = htempPos_py->GetFunction("YBox");
              if (NULL != ff) {
                LOG(info) << "FRes YBox " << htempPos_py->GetEntries() << " entries in TSR " << iSmType << iSm << iRpc
                          << ", chi2 " << ff->GetChisquare() / ff->GetNDF()
                          << Form(", striplen (%5.2f), %4.2f: %7.2f +/- %5.2f, pos "
                                  "res %5.2f +/- %5.2f at y_cen = %5.2f +/- %5.2f",
                                  fChannelInfo->GetSizey(), dVscal, 2. * ff->GetParameter(1), 2. * ff->GetParError(1),
                                  ff->GetParameter(2), ff->GetParError(2), ff->GetParameter(3), ff->GetParError(3));

                if (TMath::Abs(fChannelInfo->GetSizey() - 2. * ff->GetParameter(1)) / fChannelInfo->GetSizey() < 0.2
                    && TMath::Abs(ff->GetParError(1) / ff->GetParameter(1)) < 0.2)
                //	 &&  ff->GetChisquare() < 500.)   //FIXME - constants!
                {
                  if (TMath::Abs(ff->GetParameter(3) - YMean) < 0.5 * fChannelInfo->GetSizey()) {
                    YMean       = ff->GetParameter(3);
                    Double_t dV = dVscal * fChannelInfo->GetSizey() / (2. * ff->GetParameter(1));
                    fhSmCluSvel[iSmType]->Fill((Double_t)(iSm * iNbRpc + iRpc), dV);
                  }
                }
              }
            }

            TH1* hAvTOff_py   = fhSmCluTOff[iSmType]->ProjectionY("_py", iB + 1, iB + 1);
            Double_t Ymax     = hAvTOff_py->GetMaximum();
            Double_t dTOffmax = 0.;
            if (Ymax > 0.) {
              Int_t iBmax = hAvTOff_py->GetMaximumBin();
              dTOffmax    = hAvTOff_py->GetXaxis()->GetBinCenter(iBmax);
            }
            Double_t TMean = ((TProfile*) hAvTOff_pfx)->GetBinContent(iB + 1);
            if (TMath::Abs(dTOffmax - TMean) > 2. * TMean) {
              TMean = dTOffmax;
              LOG(debug) << "Use peak position for TOff of TSR " << iSmType << iSm << iRpc << ", B= " << iB << ": "
                         << TMean;
            }
            Double_t TWidth = ((TProfile*) hAvTOff_pfx)->GetBinError(iB + 1);
            Double_t dTYOff = YMean / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);

            if (fiBeamRefAddr == iUniqueId) TMean = 0.;  // don't shift reference counter
            LOG(debug) << Form("<ICor> Correct TSR %d%d%d by TMean=%8.2f, "
                               "TYOff=%8.2f, TWidth=%8.2f, ",
                               iSmType, iSm, iRpc, TMean, dTYOff, TWidth);

            for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // update Offset and Gain
            {
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] += -dTYOff + TMean;
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] += +dTYOff + TMean;

              LOG(debug) << "FillCalHist:"
                         << " SmT " << iSmType << " Sm " << iSm << " Rpc " << iRpc << " Ch " << iCh << ": YMean "
                         << YMean << ", TMean " << TMean << " -> "
                         << Form(" %f, %f, %f, %f ", fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
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
            Double_t YMean =
              fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
              * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            Double_t TMean =
              0.5 * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            htempPos_pfx->Fill(iCh, YMean);
            if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
              LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh << " got "
                         << htempPos_pfx->GetBinContent(iCh) << "," << htempPos_pfx->GetBinContent(iCh + 1) << ","
                         << htempPos_pfx->GetBinContent(iCh + 2) << ", expected " << YMean;
            }
            htempTOff_pfx->Fill(iCh, TMean);

            for (Int_t iSide = 0; iSide < 2; iSide++) {
              htempTot_Mean->SetBinContent(
                iCh * 2 + 1 + iSide,
                fdTTotMean / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);  //nh +1 empirical(?)
              htempTot_Off->SetBinContent(iCh * 2 + 1 + iSide, fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
            }
          }  // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )

          LOG(debug1) << " Updating done ... write to file ";
          htempPos_pfx->Write();
          htempTOff_pfx->Write();
          //        htempTot_pfx->Write();
          htempTot_Mean->Write();
          htempTot_Off->Write();

          // store old DELTOF histos
          LOG(debug) << " Copy old DelTof histos from " << gDirectory->GetName() << " to file ";

          for (Int_t iSel = 0; iSel < iNSel; iSel++) {
            // Store DelTof corrections
            TDirectory* curdir = gDirectory;
            gROOT->cd();  //
            TH1D* hCorDelTof = (TH1D*) gDirectory->FindObjectAny(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
            gDirectory->cd(curdir->GetPath());
            if (NULL != hCorDelTof) {
              TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
                Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
              hCorDelTofout->Write();
            }
            else {
              LOG(debug) << " No CorDelTof histo "
                         << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel);
            }
          }

          // store walk histos
          for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // store new values
          {
            // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
            // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
            TH1D* h1tmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX("_px", 1, nbClWalkBinY);
            TH1D* h1tmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX("_px", 1, nbClWalkBinY);
            for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
              h1tmp0->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
              h1tmp1->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
              if (h1tmp0->GetBinContent(iWx + 1) != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
                LOG(error) << "WriteHistos: restore unsuccessful! iWx " << iWx << " got "
                           << h1tmp0->GetBinContent(iWx + 1) << ", expected "
                           << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
              }
            }
            h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px", iSmType, iSm, iRpc, iCh));
            //          htmp0->Write();
            h1tmp0->Write();
            h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px", iSmType, iSm, iRpc, iCh));
            //          htmp1->Write();
            h1tmp1->Write();
          }
        } break;

        case 3:  //update offsets, gains, save walks and DELTOF
        {
          Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
          Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
          if ((fCalSmAddr < 0) || (fCalSmAddr != iSmAddr)) {  // select detectors for updating offsets
            LOG(info) << "WriteHistos (calMode==3): update Offsets and Gains, "
                         "keep Walk and DelTof for "
                      << Form("Addr 0x%08x ", TMath::Abs(fCalSmAddr)) << "TSR " << iSmType << iSm << iRpc << " with "
                      << iNbCh << " channels "
                      << " using selector " << fCalSel;

            Double_t dVscal = 1.;
            Double_t dVW    = 1.;
            if (0)  // NULL != fhSmCluSvel[iSmType])
            {
              dVscal = fhSmCluSvel[iSmType]->GetBinContent(iSm * iNbRpc + iRpc + 1);
              if (dVscal == 0.) dVscal = 1.;
              dVW = fhSmCluSvel[iSmType]->GetBinEffectiveEntries(iSm * iNbRpc + iRpc + 1);
              dVW *= 50.;  //(Double_t)iNbCh;
              if (dVW < 0.1) dVW = 0.1;
            }

            // determine average values
            htempPos_py        = htempPos->ProjectionY(Form("%s_py", htempPos->GetName()), 1, iNbCh);
            Double_t dYMeanAv  = 0.;
            Double_t dYMeanFit = 0.;
            Double_t dYLenFit  = 0.;
            if (htempPos_py->GetEntries() > fdYFitMin && fPosYMaxScal < 1.1) {
              dYMeanAv = htempPos_py->GetMean();
              LOG(debug1) << Form("Determine YMeanAv in %s by fit to %d entries", htempPos->GetName(),
                                  (Int_t) htempPos_py->GetEntries());
              CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, 0);
              Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
              fChannelInfo = fDigiPar->GetCell(iChId);
              if (NULL == fChannelInfo) {
                LOG(warning) << Form("invalid ChannelInfo for 0x%08x", iChId);
                continue;
              }
              fit_ybox(htempPos_py, fChannelInfo->GetSizey());
              TF1* ff = htempPos_py->GetFunction("YBox");
              if (NULL != ff) {
                if (TMath::Abs(fChannelInfo->GetSizey() - 2. * ff->GetParameter(1)) / fChannelInfo->GetSizey() < 0.2
                    && TMath::Abs(ff->GetParError(1) / ff->GetParameter(1)) < 0.2) {
                  Double_t dV = dVscal * fChannelInfo->GetSizey() / (2. * ff->GetParameter(1));
                  LOG(info) << "FAvRes YBox " << htempPos_py->GetEntries() << " entries in TSR " << iSmType << iSm
                            << iRpc << ", stat: " << gMinuit->fCstatu
                            << Form(", chi2 %6.2f, striplen (%5.2f): "
                                    "%7.2f+/-%5.2f, pos res "
                                    "%5.2f+/-%5.2f at y_cen = %5.2f+/-%5.2f",
                                    ff->GetChisquare() / ff->GetNDF(), fChannelInfo->GetSizey(),
                                    2. * ff->GetParameter(1), 2. * ff->GetParError(1), ff->GetParameter(2),
                                    ff->GetParError(2), ff->GetParameter(3), ff->GetParError(3));
                  if (TMath::Abs(ff->GetParameter(3) - dYMeanAv) < 0.5 * fChannelInfo->GetSizey()) {
                    dYMeanFit = ff->GetParameter(3);
                    dYLenFit  = 2. * ff->GetParameter(1);
                    fhSmCluSvel[iSmType]->Fill((Double_t)(iSm * iNbRpc + iRpc), dV, dVW);
                    for (Int_t iPar = 0; iPar < 4; iPar++)
                      fhSmCluFpar[iSmType][iPar]->Fill((Double_t)(iSm * iNbRpc + iRpc), ff->GetParameter(2 + iPar));
                  }
                }
                else {
                  LOG(info) << "FAvBad YBox " << htempPos_py->GetEntries() << " entries in " << iSmType << iSm << iRpc
                            << ", chi2 " << ff->GetChisquare()
                            << Form(", striplen (%5.2f), %4.2f: %7.2f +/- %5.2f, pos res "
                                    "%5.2f +/- %5.2f at y_cen = %5.2f +/- %5.2f",
                                    fChannelInfo->GetSizey(), dVscal, 2. * ff->GetParameter(1), 2. * ff->GetParError(1),
                                    ff->GetParameter(2), ff->GetParError(2), ff->GetParameter(3), ff->GetParError(3));
                }
              }
              else {
                LOG(info) << "FAvFailed for TSR " << iSmType << iSm << iRpc;
              }
            }
            Double_t dYShift = dYMeanFit - dYMeanAv;
            LOG(debug) << Form("CalibY for TSR %d%d%d: DY %5.2f, Fit %5.2f, Av %5.2f ", iSmType, iSm, iRpc, dYShift,
                               dYMeanFit, dYMeanAv);
            for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // update Offset and Gain
            {
              Double_t YMean = ((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1);  //set default
              YMean += dYShift;

              if (fPosYMaxScal < 1.1) {  //disable by adding "-" sign
                htempPos_py = htempPos->ProjectionY(Form("%s_py%02d", htempPos->GetName(), iCh), iCh + 1, iCh + 1);
                if (htempPos_py->GetEntries() > fdYFitMin) {
                  LOG(debug1) << Form("Determine YMean in %s of channel %d by "
                                      "length fit with %6.3f to %d entries",
                                      htempPos->GetName(), iCh, dYLenFit, (Int_t) htempPos_py->GetEntries());
                  CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
                  Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
                  fChannelInfo = fDigiPar->GetCell(iChId);
                  if (NULL == fChannelInfo) {
                    LOG(warning) << Form("invalid ChannelInfo for 0x%08x", iChId);
                    continue;
                  }
                  Double_t fp[4] = {1., 3 * 0.};  // initialize fit parameter
                  if (0)
                    for (Int_t iPar = 2; iPar < 4; iPar++)
                      if (NULL != fhSmCluFpar[iSmType][iPar])
                        fp[iPar] = fhSmCluFpar[iSmType][iPar]->GetBinContent(iSm * iNbRpc + iRpc + 1);
                  //LOG(info) << Form("Call yFit with %6.3f, %6.3f, %6.3f, %6.3f",fp[0],fp[1],fp[2],fp[3])
                  //           ;
                  Double_t* fpp = &fp[0];
                  fit_ybox(htempPos_py, dYLenFit, fpp);
                  TF1* ff = htempPos_py->GetFunction("YBox");
                  if (NULL != ff) {
                    LOG(debug1) << Form("FitPar1 %6.3f Err %6.3f, Par3 %6.3f Err %6.3f ", ff->GetParameter(1),
                                        ff->GetParError(1), ff->GetParameter(3), ff->GetParError(3));
                    if (TMath::Abs(fChannelInfo->GetSizey() - 2. * ff->GetParameter(1)) / fChannelInfo->GetSizey() < 0.1
                        && TMath::Abs(ff->GetParError(1) / ff->GetParameter(1)) < 0.05)
                    //&&  ff->GetChisquare() < 200.)   //FIXME - constants!
                    {
                      if (TMath::Abs(ff->GetParameter(3) - dYMeanFit) < 0.5 * fChannelInfo->GetSizey()) {
                        YMean       = ff->GetParameter(3);
                        Double_t dV = dVscal * fChannelInfo->GetSizey() / (2. * ff->GetParameter(1));
                        fhSmCluSvel[iSmType]->Fill((Double_t)(iSm * iNbRpc + iRpc), dV, dVW);
                        LOG(info) << "FRes YBox " << htempPos_py->GetEntries() << " entries in TSRC " << iSmType << iSm
                                  << iRpc << iCh << ", chi2 " << ff->GetChisquare()
                                  << Form(", striplen (%5.2f), %4.2f -> %4.2f,  "
                                          "%4.1f: %7.2f+/-%5.2f, pos res "
                                          "%5.2f+/-%5.2f at y_cen = %5.2f+/-%5.2f",
                                          fChannelInfo->GetSizey(), dVscal, dV, dVW, 2. * ff->GetParameter(1),
                                          2. * ff->GetParError(1), ff->GetParameter(2), ff->GetParError(2),
                                          ff->GetParameter(3), ff->GetParError(3));
                        for (Int_t iPar = 0; iPar < 4; iPar++)
                          fhSmCluFpar[iSmType][iPar]->Fill((Double_t)(iSm * iNbRpc + iRpc), ff->GetParameter(2 + iPar));
                      }
                    }
                    else {
                      YMean = dYMeanFit;  // no new info available
                      LOG(info) << "FBad YBox " << htempPos_py->GetEntries() << " entries in TSRC " << iSmType << iSm
                                << iRpc << iCh << ", chi2 " << ff->GetChisquare()
                                << Form(", striplen (%5.2f), %4.2f: %7.2f +/- %5.2f, pos "
                                        "res %5.2f +/- %5.2f at y_cen = %5.2f +/- %5.2f",
                                        fChannelInfo->GetSizey(), dVscal, 2. * ff->GetParameter(1),
                                        2. * ff->GetParError(1), ff->GetParameter(2), ff->GetParError(2),
                                        ff->GetParameter(3), ff->GetParError(3));
                    }
                  }
                }
              }  // ybox - fit end

              Double_t TMean = ((TProfile*) htempTOff_pfx)->GetBinContent(iCh + 1);
              if (kTRUE) {  // fit gaussian around most abundant bin
                TH1* hTy = (TH1*) htempTOff->ProjectionY(Form("%s_py%d", htempTOff->GetName(), iCh), iCh + 1, iCh + 1);
                if (hTy->GetEntries() > WalkNHmin) {
                  Double_t dFMean    = hTy->GetBinCenter(hTy->GetMaximumBin());
                  Double_t dFLim     = 2.0;  // CAUTION, fixed numeric value
                  Double_t dBinSize  = hTy->GetBinWidth(1);
                  dFLim              = TMath::Max(dFLim, 5. * dBinSize);
                  TFitResultPtr fRes = hTy->Fit("gaus", "SQM0", "", dFMean - dFLim, dFMean + dFLim);
                  if (TMath::Abs(TMean - fRes->Parameter(1)) > 5.)
                    LOG(warn) << "CalibF "
                              << Form("TSRC %d%d%d%d gaus %8.2f %8.2f %8.2f for "
                                      "TM %8.2f, YM %6.2f",
                                      iSmType, iSm, iRpc, iCh, fRes->Parameter(0), fRes->Parameter(1),
                                      fRes->Parameter(2), TMean, YMean);
                  TMean = fRes->Parameter(1);  //overwrite mean
                }
              }
              Double_t dTYOff = YMean / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);

              if (fiBeamRefAddr == iUniqueId) {
                if (iFindT0 == 1) continue;  // action already done
                // don't shift time of reference counter on average
                if (iCh == 0) {
                  Double_t dW = 0.;
                  for (Int_t iRefCh = 0; iRefCh < iNbCh; iRefCh++) {
                    Double_t dWCh = ((TH1*) htempTOff_px)->GetBinContent(iRefCh + 1);
                    if (0 < dWCh) {
                      dW += dWCh;
                      if (dWCh > WalkNHmin) {
                        TH1* hTy          = (TH1*) htempTOff->ProjectionY(Form("%s_py%d", htempTOff->GetName(), iRefCh),
                                                                 iRefCh + 1, iRefCh + 1);
                        Double_t dFMean   = hTy->GetBinCenter(hTy->GetMaximumBin());
                        Double_t dFLim    = 0.5;  // CAUTION, fixed numeric value
                        Double_t dBinSize = hTy->GetBinWidth(1);
                        dFLim             = TMath::Max(dFLim, 5. * dBinSize);
                        TFitResultPtr fRes = hTy->Fit("gaus", "SQM0", "", dFMean - dFLim, dFMean + dFLim);
                        LOG(debug) << "CalibC "
                                   << Form(" TSRC %d%d%d%d gaus %8.2f %8.2f %8.2f ", iSmType, iSm, iRpc, iRefCh,
                                           fRes->Parameter(0), fRes->Parameter(1), fRes->Parameter(2));
                        TBeamRefMean += fRes->Parameter(1) * dWCh;  //overwrite mean
                      }
                      else {
                        TBeamRefMean += ((TProfile*) htempTOff_pfx)->GetBinContent(iRefCh + 1) * dWCh;
                      }
                      TBeamRefMean += dWCh *  // enforce <offset>=0
                                      fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                    }
                  }
                  if (dW > 0.) {
                    TBeamRefMean /= dW;
                    // TBD apply offset all other detectors since beam counter will not be shifted
                    LOG(info) << "<I> T0 shift all offsets by " << TBeamRefMean;
                  }
                  else
                    TBeamRefMean = 0.;
                }
                if (htempTOff_px->GetBinContent(iCh + 1) > 0.)
                  LOG(debug) << Form("CalibA %d,%2d,%2d: TSRC %d%d%d%d, hits %6.0f, TM %8.3f , "
                                     "TAV  %8.3f, TWM %8.3f, TOff %8.3f,  TOffnew  %8.3f, ",
                                     fCalMode, fCalSel, fTRefMode, iSmType, iSm, iRpc, iCh,
                                     htempTOff_px->GetBinContent(iCh + 1), TMean,
                                     ((TProfile*) hAvTOff_pfx)->GetBinContent(iSm * iNbRpc + iRpc + 1), TBeamRefMean,
                                     fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                                     fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] + TMean - TBeamRefMean);
                //            TMean-=((TProfile *)hAvTOff_pfx)->GetBinContent(iSm*iNbRpc+iRpc+1);
                TMean -= TBeamRefMean;
              }  // beam counter end
              else {
                TMean += TBeamRefMean;
              }
              if (htempTOff_px->GetBinContent(iCh + 1) > WalkNHmin) {
                Double_t dOff0 = fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                Double_t dOff1 = fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1];
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] += -dTYOff + TMean;
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] += +dTYOff + TMean;
                //if(iSmType==0 && iSm==4 && iRpc==2 && iCh==26)
                LOG(debug) << Form("CalibB %d,%2d,%2d: TSRC %d%d%d%d, hits %6.0f, YM %6.3f"
                                   ", dTY %6.3f, TM %8.3f, Off %8.3f,%8.3f -> %8.3f,%8.3f ",
                                   fCalMode, fCalSel, fTRefMode, iSmType, iSm, iRpc, iCh,
                                   htempTOff_px->GetBinContent(iCh + 1), YMean, dTYOff, TMean, dOff0, dOff1,
                                   fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                                   fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
              }
              /*
					 Double_t TotMean=((TProfile *)htempTot_pfx)->GetBinContent(iCh+1);
					 if(0.001 < TotMean){
					 fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][0] *= fdTTotMean / TotMean;
					 fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][1] *= fdTTotMean / TotMean;
					 }
					 */
              if (fCalMode < 90)  // keep digi TOT calibration in last step
                for (Int_t iSide = 0; iSide < 2; iSide++) {
                  Int_t ib  = iCh * 2 + 1 + iSide;
                  TH1* hbin = htempTot->ProjectionY(Form("bin%d", ib), ib, ib);
                  if (100 > hbin->GetEntries()) continue;  //request min number of entries
                  /*            Double_t Ymax=hbin->GetMaximum();*/
                  Int_t iBmax   = hbin->GetMaximumBin();
                  TAxis* xaxis  = hbin->GetXaxis();
                  Double_t Xmax = xaxis->GetBinCenter(iBmax) / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide];
                  Double_t XOff = Xmax - fTotPreRange;
                  if (0) {  //TMath::Abs(XOff - fvCPTotOff[iSmType][iSm*iNbRpc+iRpc][iCh][iSide])>100){
                    LOG(warning) << "XOff changed for "
                                 << Form("SmT%01d_sm%03d_rpc%03d_Side%d: XOff %f, old %f", iSmType, iSm, iRpc, iSide,
                                         XOff, fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
                  }
                  //            Double_t TotMean=htempTot_Mean->GetBinContent(ib);
                  Double_t TotMean = hbin->GetMean();
                  if (15 == iSmType) {
                    LOG(warning) << "Gain for "
                                 << Form("SmT%01d_sm%03d_rpc%03d_Side%d: TotMean %f, prof %f, "
                                         "gain %f, modg %f ",
                                         iSmType, iSm, iRpc, iSide, TotMean, htempTot_Mean->GetBinContent(ib),
                                         fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide], fdTTotMean / TotMean);
                  }
                  if (0.001 < TotMean) {
                    fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] *= fdTTotMean / TotMean;
                  }
                }
              if (5 == iSmType
                  && fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                       != fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]) {  // diamond
                LOG(warning) << "CbmTofEventClusterizer::FillCalHist:"
                             << " SmT " << iSmType << " Sm " << iSm << " Rpc " << iRpc << " Ch " << iCh << ": YMean "
                             << YMean << ", TMean " << TMean << " -> "
                             << Form(" %f %f %f %f ", fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                                     fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1],
                                     fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                                     fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
                Double_t dTOff =
                  0.5
                  * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
                Double_t dGain = 0.5
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

          Double_t TOff0_mean = 0.;
          Double_t TOff1_mean = 0.;
          for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // determine means
          {
            TOff0_mean += fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0];
            TOff1_mean += fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1];
          }
          TOff0_mean /= (Double_t) iNbCh;
          TOff1_mean /= (Double_t) iNbCh;

          const Double_t TMaxDev = htempTOff->GetYaxis()->GetXmax();
          for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // remove outlyers
          {
            if (TMath::Abs(TOff0_mean - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]) > TMaxDev)
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] = TOff0_mean;
            if (TMath::Abs(TOff1_mean - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]) > TMaxDev)
              fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] = TOff1_mean;
          }

          for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // store new values
          {
            Double_t YMean =
              fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
              * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            Double_t TMean =
              0.5 * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            htempPos_pfx->Fill(iCh, YMean);
            if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
              LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh << " got "
                         << htempPos_pfx->GetBinContent(iCh) << "," << htempPos_pfx->GetBinContent(iCh + 1) << ","
                         << htempPos_pfx->GetBinContent(iCh + 2) << ", expected " << YMean;
            }
            htempTOff_pfx->Fill(iCh, TMean);

            LOG(debug) << Form("CalibU %d,%2d,%2d: TSRC %d%d%d%d, hits %6.0f, YM %6.3f"
                               ", TM %8.3f, OffM %8.3f,%8.3f ",
                               fCalMode, fCalSel, fTRefMode, iSmType, iSm, iRpc, iCh,
                               htempTOff_px->GetBinContent(iCh + 1), YMean, TMean, TOff0_mean, TOff1_mean);

            for (Int_t iSide = 0; iSide < 2; iSide++) {
              htempTot_Mean->SetBinContent(iCh * 2 + 1 + iSide,
                                           fdTTotMean / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
              htempTot_Off->SetBinContent(iCh * 2 + 1 + iSide, fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
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
          LOG(debug) << " Copy old DelTof histos from " << gDirectory->GetName() << " to file ";

          for (Int_t iSel = 0; iSel < iNSel; iSel++) {
            // Store DelTof corrections
            TDirectory* curdir = gDirectory;
            gROOT->cd();  //
            TH1D* hCorDelTof = (TH1D*) gDirectory->FindObjectAny(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
            gDirectory->cd(curdir->GetPath());
            if (NULL != hCorDelTof) {
              TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
                Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
              hCorDelTofout->Write();
            }
            else {
              LOG(debug) << " No CorDelTof histo "
                         << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel);
            }
          }

          LOG(debug) << " Store old walk histos to file ";
          // store walk histos
          for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // restore old values
          {
            if (NULL == fhRpcCluWalk[iDetIndx][iCh][0]) break;
            // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
            // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
            TH1D* h1tmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX("_px", 1, nbClWalkBinY);
            TH1D* h1tmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX("_px", 1, nbClWalkBinY);
            for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
              h1tmp0->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
              h1tmp1->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
              if (h1tmp0->GetBinContent(iWx + 1) != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
                LOG(error) << "WriteHistos: restore unsuccessful! iWx " << iWx << " got "
                           << h1tmp0->GetBinContent(iWx + 1) << ", expected "
                           << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
              }
            }
            h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px", iSmType, iSm, iRpc, iCh));
            //          htmp0->Write();
            h1tmp0->Write();
            h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px", iSmType, iSm, iRpc, iCh));
            //          htmp1->Write();
            h1tmp1->Write();
          }
        } break;
        case 4:  //update DelTof, save offsets, gains and walks
        {
          Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
          Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
          LOG(debug) << "WriteHistos: restore Offsets, Gains and Walk, save DelTof for "
                     << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc;
          htempPos_pfx->Reset();  //reset to restore mean of original histos
          htempTOff_pfx->Reset();
          htempTot_Mean->Reset();
          htempTot_Off->Reset();
          for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
            Double_t YMean =
              fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
              * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            Double_t TMean =
              0.5 * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            htempPos_pfx->Fill(iCh, YMean);
            if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
              LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh << " got "
                         << htempPos_pfx->GetBinContent(iCh) << "," << htempPos_pfx->GetBinContent(iCh + 1) << ","
                         << htempPos_pfx->GetBinContent(iCh + 2) << ", expected " << YMean;
            }
            htempTOff_pfx->Fill(iCh, TMean);

            for (Int_t iSide = 0; iSide < 2; iSide++) {
              htempTot_Mean->SetBinContent(
                iCh * 2 + 1 + iSide,
                fdTTotMean / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);  //nh +1 empirical(?)
              htempTot_Off->SetBinContent(iCh * 2 + 1 + iSide, fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
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
            // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
            // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
            TH1D* h1tmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX("_px", 1, nbClWalkBinY);
            TH1D* h1tmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX("_px", 1, nbClWalkBinY);
            for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
              h1tmp0->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
              h1tmp1->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
              if (h1tmp0->GetBinContent(iWx + 1) != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
                LOG(error) << "WriteHistos: restore unsuccessful! iWx " << iWx << " got "
                           << h1tmp0->GetBinContent(iWx + 1) << ", expected "
                           << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
              }
            }
            h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px", iSmType, iSm, iRpc, iCh));
            //          htmp0->Write();
            h1tmp0->Write();
            h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px", iSmType, iSm, iRpc, iCh));
            //          htmp1->Write();
            h1tmp1->Write();
          }

          // generate/update DelTof corrections
          if ((fCalSmAddr < 0 && -fCalSmAddr != iSmAddr)
              || (fCalSmAddr == iSmAddr))  // select detectors for determination of DelTof correction
          {
            if (fiBeamRefAddr == iSmAddr) continue;  // no DelTof correction for Diamonds

            for (Int_t iSel = 0; iSel < iNSel; iSel++) {
              TH2* h2tmp = fhTRpcCluDelTof[iDetIndx][iSel];
              if (NULL == h2tmp) {
                LOG(debug) << Form("WriteHistos:  histo not available for SmT %d, Sm %d, Rpc %d", iSmType, iSm, iRpc);
                break;
              }
              Int_t iNEntries = h2tmp->GetEntries();

              //          h2tmp->Write();
              TProfile* htmp = h2tmp->ProfileX("_pfx", 1, h2tmp->GetNbinsY());
              TH1D* h1tmp    = h2tmp->ProjectionX("_px", 1, h2tmp->GetNbinsY());
              /*          TH1D *h1ytmp   = h2tmp->ProjectionY("_py",1,h2tmp->GetNbinsX());*/
              Double_t dDelMean =
                0.;  //h1ytmp->GetMean();// inspect normalization, interferes with mode 3 for diamonds, nh, 10.01.2015 (?)
              Double_t dNEntriesSum = 0.;
              for (Int_t iBx = 0; iBx < nbClDelTofBinX; iBx++) {
                Double_t dNEntries = h1tmp->GetBinContent(iBx + 1);
                if (dNEntries > WalkNHmin)  // modify, request sufficient # of entries
                  fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] += ((TProfile*) htmp)->GetBinContent(iBx + 1);
                dDelMean += fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] * dNEntries;
                dNEntriesSum += dNEntries;
              }
              dDelMean /= dNEntriesSum;

              LOG(debug) << Form(" Update DelTof correction for SmT %d, Sm %d, "
                                 "Rpc %d, Sel%d: Entries %d, Mean shift %6.1f",
                                 iSmType, iSm, iRpc, iSel, iNEntries, dDelMean);

              for (Int_t iBx = 0; iBx < nbClDelTofBinX; iBx++) {
                h1tmp->SetBinContent(iBx + 1, fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iSel] - dDelMean);
                //h1tmp->SetBinContent(iBx+1,fvCPDelTof[iSmType][iSm*iNbRpc+iRpc][iBx][iSel]);
              }
              h1tmp->SetName(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
              h1tmp->Write();
            }
          }
          else {  // copy existing histograms
            for (Int_t iSel = 0; iSel < iNSel; iSel++) {
              // Store DelTof corrections
              TDirectory* curdir = gDirectory;
              gROOT->cd();  //
              TH1D* hCorDelTof = (TH1D*) gDirectory->FindObjectAny(
                Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
              gDirectory->cd(curdir->GetPath());
              if (NULL != hCorDelTof) {
                TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
                  Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
                LOG(debug) << " Save existing CorDelTof histo "
                           << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel);
                hCorDelTofout->Write();
              }
              else {
                LOG(debug) << " No CorDelTof histo "
                           << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel);
              }
            }
          }
        } break;
        case 5:  //update offsets (from positions only), gains, save walks and DELTOF
        {
          Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
          Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
          if ((fCalSmAddr < 0) || (fCalSmAddr != iSmAddr)) {  // select detectors for updating offsets
            LOG(debug) << "WriteHistos (calMode==5): update Offsets and Gains, "
                          "keep Walk and DelTof for "
                       << "Smtype" << iSmType << ", Sm " << iSm << ", Rpc " << iRpc << " with " << iNbCh << " channels "
                       << " using selector " << fCalSel;

            for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // update Offset and Gain
            {
              Double_t YMean  = ((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1);  //nh +1 empirical(?)
              Double_t TMean  = 0.;
              Double_t dTYOff = YMean / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);

              if (htempTOff_px->GetBinContent(iCh + 1) > WalkNHmin) {
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] += -dTYOff + TMean;
                fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] += +dTYOff + TMean;
              }
              LOG(debug3) << Form("Calib: TSRC %d%d%d%d, hits %6.0f, new Off %8.0f,%8.0f ", iSmType, iSm, iRpc, iCh,
                                  htempTOff_px->GetBinContent(iCh + 1), fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
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
                if (100 > hbin->GetEntries()) continue;  //request min number of entries
                /*            Double_t Ymax=hbin->GetMaximum();*/
                Int_t iBmax   = hbin->GetMaximumBin();
                TAxis* xaxis  = hbin->GetXaxis();
                Double_t Xmax = xaxis->GetBinCenter(iBmax) / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide];
                Double_t XOff = Xmax - fTotPreRange;
                if (0) {  //TMath::Abs(XOff - fvCPTotOff[iSmType][iSm*iNbRpc+iRpc][iCh][iSide])>100){
                  LOG(warning) << "XOff changed for "
                               << Form("SmT%01d_sm%03d_rpc%03d_Side%d: XOff %f, old %f", iSmType, iSm, iRpc, iSide,
                                       XOff, fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
                }
                //            Double_t TotMean=htempTot_Mean->GetBinContent(ib);
                Double_t TotMean = hbin->GetMean();
                if (15 == iSmType) {
                  LOG(warning) << "Gain for "
                               << Form("SmT%01d_sm%03d_rpc%03d_Side%d: TotMean %f, prof %f, "
                                       "gain %f, modg %f ",
                                       iSmType, iSm, iRpc, iSide, TotMean, htempTot_Mean->GetBinContent(ib),
                                       fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide], fdTTotMean / TotMean);
                }
                if (0.001 < TotMean) { fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] *= fdTTotMean / TotMean; }
              }
              if (5 == iSmType
                  && fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                       != fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]) {  // diamond
                LOG(warning) << "CbmTofEventClusterizer::FillCalHist:"
                             << " SmT " << iSmType << " Sm " << iSm << " Rpc " << iRpc << " Ch " << iCh << ": YMean "
                             << YMean << ", TMean " << TMean << " -> "
                             << Form(" %f %f %f %f ", fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                                     fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1],
                                     fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][0],
                                     fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
                Double_t dTOff =
                  0.5
                  * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0] + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1]);
                Double_t dGain = 0.5
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
            Double_t YMean =
              fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc) * 0.5
              * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] - fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            Double_t TMean =
              0.5 * (fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][1] + fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][0]);
            htempPos_pfx->Fill(iCh, YMean);
            if (((TProfile*) htempPos_pfx)->GetBinContent(iCh + 1) != YMean) {
              LOG(error) << "WriteHistos: restore unsuccessful! ch " << iCh << " got "
                         << htempPos_pfx->GetBinContent(iCh) << "," << htempPos_pfx->GetBinContent(iCh + 1) << ","
                         << htempPos_pfx->GetBinContent(iCh + 2) << ", expected " << YMean;
            }
            htempTOff_pfx->Fill(iCh, TMean);

            for (Int_t iSide = 0; iSide < 2; iSide++) {
              htempTot_Mean->SetBinContent(iCh * 2 + 1 + iSide,
                                           fdTTotMean / fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
              htempTot_Off->SetBinContent(iCh * 2 + 1 + iSide, fvCPTotOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]);
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
          LOG(debug) << " Copy old DelTof histos from " << gDirectory->GetName() << " to file ";

          for (Int_t iSel = 0; iSel < iNSel; iSel++) {
            // Store DelTof corrections
            TDirectory* curdir = gDirectory;
            gROOT->cd();  //
            TH1D* hCorDelTof = (TH1D*) gDirectory->FindObjectAny(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
            gDirectory->cd(curdir->GetPath());
            if (NULL != hCorDelTof) {
              TH1D* hCorDelTofout = (TH1D*) hCorDelTof->Clone(
                Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel));
              hCorDelTofout->Write();
            }
            else {
              LOG(debug) << " No CorDelTof histo "
                         << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof", iSmType, iSm, iRpc, iSel);
            }
          }

          LOG(debug) << " Store old walk histos to file ";
          // store walk histos
          for (Int_t iCh = 0; iCh < iNbCh; iCh++)  // restore old values
          {
            if (NULL == fhRpcCluWalk[iDetIndx][iCh][0]) break;
            // TProfile *htmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
            // TProfile *htmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProfileX("_pfx",1,nbClWalkBinY);  (VF) not used
            TH1D* h1tmp0 = fhRpcCluWalk[iDetIndx][iCh][0]->ProjectionX("_px", 1, nbClWalkBinY);
            TH1D* h1tmp1 = fhRpcCluWalk[iDetIndx][iCh][1]->ProjectionX("_px", 1, nbClWalkBinY);
            for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
              h1tmp0->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]);
              h1tmp1->SetBinContent(iWx + 1, fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iWx]);
              if (h1tmp0->GetBinContent(iWx + 1) != fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx]) {
                LOG(error) << "WriteHistos: restore unsuccessful! iWx " << iWx << " got "
                           << h1tmp0->GetBinContent(iWx + 1) << ", expected "
                           << fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iWx];
              }
            }
            h1tmp0->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px", iSmType, iSm, iRpc, iCh));
            //          htmp0->Write();
            h1tmp0->Write();
            h1tmp1->SetName(Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px", iSmType, iSm, iRpc, iCh));
            //          htmp1->Write();
            h1tmp1->Write();
          }
        } break;

        default: LOG(debug) << "WriteHistos: update mode " << fCalMode << " not yet implemented";
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
Bool_t CbmTofEventClusterizer::DeleteHistos()
{
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
Bool_t CbmTofEventClusterizer::BuildClusters()
{
  Int_t iMess = 0;
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
    LOG(warning) << "Too many TOF digis in event " << fiNevtBuild;
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

        if (NULL != fTofDigiPointMatches) {  // copy MC Match Object
          const CbmMatch digiMatch = (CbmMatch) fTofDigiPointMatches->at(iDigInd);
          ((std::vector<CbmMatch>*) fTofDigiPointMatches)->push_back((CbmMatch) digiMatch);
        }
      }
    }
    iNbTofDigi = fTofDigiVec.size();
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
      Int_t iDetIndx    = fDigiBdfPar->GetDetInd(pDigi->GetAddress() & DetMask);

      LOG(debug) << "RawDigi" << iDigInd << " " << pDigi << Form(" Address : 0x%08x ", pDigi->GetAddress()) << " TSRCS "
                 << pDigi->GetType() << pDigi->GetSm() << pDigi->GetRpc() << pDigi->GetChannel() << pDigi->GetSide()
                 << ", DetIndx " << iDetIndx << " : " << pDigi->ToString()
        //         <<" Time "<<pDigi->GetTime()
        //         <<" Tot " <<pDigi->GetTot()
        ;

      if (fDigiBdfPar->GetNbDet() - 1 < iDetIndx || iDetIndx < 0) {
        LOG(debug) << Form(" Wrong DetIndx %d >< %d ", iDetIndx, fDigiBdfPar->GetNbDet());
        break;
      }

      CbmTofDigi* pDigi2Min = NULL;
      Double_t dTDifMin     = dDoubleMax;

      if (fDutId > -1) {
        if (fhRpcDigiCor.size() > 0) {
          if (NULL == fhRpcDigiCor[iDetIndx]) {
            if (100 < iMess++)
              LOG(warning) << Form(" DigiCor Histo for  DetIndx %d derived from 0x%08x not found", iDetIndx,
                                   pDigi->GetAddress());
            continue;
          }
        }
        else
          break;

        size_t iDigiCh = pDigi->GetChannel() * 2 + pDigi->GetSide();
        if (iDigiCh < fvTimeLastDigi[iDetIndx].size()) {
          if (fvTimeLastDigi[iDetIndx][iDigiCh] > 0) {
            if (kTRUE) {  // fdStartAna10s > 0.) {
                          //Double_t dTimeAna10s = (pDigi->GetTime() - fdStartAna10s) / 1.E9;
                          //if (dTimeAna10s < fdSpillDuration)
              fhRpcDigiDTLD[iDetIndx]->Fill(iDigiCh, (pDigi->GetTime() - fvTimeLastDigi[iDetIndx][iDigiCh]));
            }
          }
          fvTimeLastDigi[iDetIndx][iDigiCh] = pDigi->GetTime();

          if (fvTimeFirstDigi[iDetIndx][iDigiCh] != 0.) {
            fhRpcDigiDTFD[iDetIndx]->Fill(iDigiCh, (pDigi->GetTime() - fvTimeFirstDigi[iDetIndx][iDigiCh]));
            fvMulDigi[iDetIndx][iDigiCh]++;
          }
          else {
            fvTimeFirstDigi[iDetIndx][iDigiCh] = pDigi->GetTime();
            fvMulDigi[iDetIndx][iDigiCh]++;
          }
        }

        //       for (Int_t iDigI2 =iDigInd+1; iDigI2<iNbTofDigi;iDigI2++){
        for (Int_t iDigI2 = 0; iDigI2 < iNbTofDigi; iDigI2++) {
          CbmTofDigi* pDigi2 = &(fTofDigiVec.at(iDigI2));
          //         CbmTofDigi *pDigi2 = (CbmTofDigi*) fTofDigisColl->At( iDigI2 );
          // Fill digi correlation histogram per counter
          if (iDetIndx == fDigiBdfPar->GetDetInd(pDigi2->GetAddress())) {
            if (0. == pDigi->GetSide() && 1. == pDigi2->GetSide()) {
              fhRpcDigiCor[iDetIndx]->Fill(pDigi->GetChannel(), pDigi2->GetChannel());
            }
            else {
              if (1. == pDigi->GetSide() && 0. == pDigi2->GetSide()) {
                fhRpcDigiCor[iDetIndx]->Fill(pDigi2->GetChannel(), pDigi->GetChannel());
              }
            }
            if (pDigi->GetSide() != pDigi2->GetSide()) {
              if (pDigi->GetChannel() == pDigi2->GetChannel()) {
                Double_t dTDif = TMath::Abs(pDigi->GetTime() - pDigi2->GetTime());
                if (dTDif < dTDifMin) {
                  dTDifMin  = dTDif;
                  pDigi2Min = pDigi2;
                }
              }
              else if (TMath::Abs(pDigi->GetChannel() - pDigi2->GetChannel())
                       == 1) {  // opposite side missing, neighbouring channel has hit on opposite side // FIXME
                // check that same side digi of neighbouring channel is absent
                Int_t iDigI3 = 0;
                for (; iDigI3 < iNbTofDigi; iDigI3++) {
                  CbmTofDigi* pDigi3 = &(fTofDigiVec.at(iDigI3));
                  //       CbmTofDigi *pDigi3 = (CbmTofDigi*) fTofDigisColl->At( iDigI3 );
                  if (pDigi3->GetSide() == pDigi->GetSide() && pDigi2->GetChannel() == pDigi3->GetChannel()) break;
                }
                if (iDigI3 == iNbTofDigi)  // same side neighbour did not fire
                {
                  Int_t iCorMode = 0;  // Missing hit correction mode
                  switch (iCorMode) {
                    case 0:  // no action
                      break;
                    case 1:  // shift found hit
                      LOG(debug2) << Form("shift channel %d%d%d%d%d and  ", (Int_t) pDigi->GetType(),
                                          (Int_t) pDigi->GetSm(), (Int_t) pDigi->GetRpc(), (Int_t) pDigi->GetChannel(),
                                          (Int_t) pDigi->GetSide())
                                  << Form(" %d%d%d%d%d ", (Int_t) pDigi2->GetType(), (Int_t) pDigi2->GetSm(),
                                          (Int_t) pDigi2->GetRpc(), (Int_t) pDigi2->GetChannel(),
                                          (Int_t) pDigi2->GetSide());
                      //if(pDigi->GetTime() < pDigi2->GetTime())
                      if (pDigi->GetSide() == 0)
                        pDigi2->SetAddress(pDigi->GetSm(), pDigi->GetRpc(), pDigi->GetChannel(), 1 - pDigi->GetSide(),
                                           pDigi->GetType());
                      else
                        pDigi->SetAddress(pDigi2->GetSm(), pDigi2->GetRpc(), pDigi2->GetChannel(),
                                          1 - pDigi2->GetSide(), pDigi2->GetType());

                      LOG(debug2) << Form("resultchannel %d%d%d%d%d and  ", (Int_t) pDigi->GetType(),
                                          (Int_t) pDigi->GetSm(), (Int_t) pDigi->GetRpc(), (Int_t) pDigi->GetChannel(),
                                          (Int_t) pDigi->GetSide())
                                  << Form(" %d%d%d%d%d ", (Int_t) pDigi2->GetType(), (Int_t) pDigi2->GetSm(),
                                          (Int_t) pDigi2->GetRpc(), (Int_t) pDigi2->GetChannel(),
                                          (Int_t) pDigi2->GetSide());
                      break;
                    case 2:  // insert missing hits
                      fTofDigiVec.push_back(CbmTofDigi(*pDigi));
                      CbmTofDigi* pDigiN = &(fTofDigiVec.back());
                      //		     CbmTofDigi *pDigiN  = new((*fTofDigisColl)[iNbTofDigi++]) CbmTofDigi( *pDigi );
                      pDigiN->SetAddress(pDigi->GetSm(), pDigi->GetRpc(), pDigi2->GetChannel(), pDigi->GetSide(),
                                         pDigi->GetType());
                      pDigiN->SetTot(pDigi2->GetTot());

                      fTofDigiVec.push_back(CbmTofDigi(*pDigi2));
                      CbmTofDigi* pDigiN2 = &(fTofDigiVec.back());
                      //		     CbmTofDigi *pDigiN2 = new((*fTofDigisColl)[iNbTofDigi++]) CbmTofDigi( *pDigi2 );
                      pDigiN2->SetAddress(pDigi2->GetSm(), pDigi2->GetRpc(), pDigi->GetChannel(), pDigi2->GetSide(),
                                          pDigi2->GetType());
                      pDigiN2->SetTot(pDigi->GetTot());

                      break;
                  }
                }
              }
            }
          }
        }
      }

      if (pDigi2Min != NULL) {
        CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof, pDigi->GetType(), pDigi->GetSm(), pDigi->GetRpc(), 0,
                                    pDigi->GetChannel());
        Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
        fChannelInfo = fDigiPar->GetCell(iChId);
        if (NULL == fChannelInfo) {
          LOG(warning) << Form("BuildClusters: invalid ChannelInfo for 0x%08x", iChId);
          continue;
        }
        if (fDigiBdfPar->GetSigVel(pDigi->GetType(), pDigi->GetSm(), pDigi->GetRpc()) * dTDifMin * 0.5
            < fPosYMaxScal * fChannelInfo->GetSizey()) {
          //check consistency
          if (8 == pDigi->GetType() || 5 == pDigi->GetType()) {
            if (pDigi->GetTime() != pDigi2Min->GetTime()) {
              if (fiMsgCnt-- > 0) {
                LOG(warning) << " BuildClusters: Inconsistent duplicated digis in event " << fiNevtBuild
                             << ", Ind: " << iDigInd;  // << "CTyp: " << pDigi->GetCounterType;
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
					 LOG(debug)<<"      "<<pDigi->ToString() ;
					 LOG(debug)<<"      "<<pDigi2Min->ToString() ;
					 */
        }
      }
    }
    for (UInt_t iDetIndx = 0; iDetIndx < fvTimeFirstDigi.size(); iDetIndx++)
      for (UInt_t iCh = 0; iCh < fvTimeFirstDigi[iDetIndx].size(); iCh++) {
        if (fvTimeFirstDigi[iDetIndx][iCh] != 0.) fhRpcDigiDTMul[iDetIndx]->Fill(iCh, fvMulDigi[iDetIndx][iCh]);
      }
  }  // kTRUE end

  // Calibrate RawDigis
  if (kTRUE) {
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
    if (fDutId > -1) {
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
    }
    BuildHits();

  }  // if( kTRUE ) obsolete )

  return kTRUE;
}

Bool_t CbmTofEventClusterizer::MergeClusters()
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

void CbmTofEventClusterizer::fit_ybox(const char* hname)
{
  TH1* h1;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (NULL != h1) { fit_ybox(h1, 0.); }
}

void CbmTofEventClusterizer::fit_ybox(TH1* h1, Double_t ysize)
{
  Double_t* fpar = NULL;
  fit_ybox(h1, ysize, fpar);
}

void CbmTofEventClusterizer::fit_ybox(TH1* h1, Double_t ysize, Double_t* fpar = NULL)
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
    f1->SetParLimits(1, ysize * 0.5, ysize * 0.5);
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

void CbmTofEventClusterizer::CheckLHMemory()
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

void CbmTofEventClusterizer::CleanLHMemory()
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

Bool_t CbmTofEventClusterizer::AddNextChan(Int_t iSmType, Int_t iSm, Int_t iRpc, Int_t iLastChan, Double_t dLastPosX,
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
    if (fDutId > -1) fhNbDigiPerChan->Fill(fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size());
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

void CbmTofEventClusterizer::LH_store(Int_t iSmType, Int_t iSm, Int_t iRpc, Int_t iChm, CbmTofHit* pHit)
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

Bool_t CbmTofEventClusterizer::BuildHits()
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
  if (kTRUE) {
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
                  if (fDutId > -1) fhNbDigiPerChan->Fill(fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size());

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
                    LOG(error) << "CbmTofEventClusterizer::BuildClusters: no "
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
                    LOG(debug) << "CbmTofEventClusterizer::BuildClusters: "
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
                    LOG(debug1) << "Remove digis on TSRC " << iSmType << iSm << iRpc << iCh << " with dPosY " << dPosY
                                << " > " << fChannelInfo->GetSizey();
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
                      if (fDutId > -1) {
                        fhDigTimeDifClust->Fill(dTime - dLastTime);
                        fhDigSpacDifClust->Fill(dPosY - dLastPosY);
                        fhDigDistClust->Fill(dPosY - dLastPosY, dTime - dLastTime);
                      }
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
              iDetId = CbmTofAddress::GetUniqueAddress(iSm, iRpc, iChm, 0, iSmType);
              //Int_t iRefId = 0;  // Index of the correspondng TofPoint
              //if(NULL != fTofPointsColl) iRefId = fTofPointsColl->IndexOf( vPtsRef[0] );
              TString cstr = "Save V-Hit  ";
              cstr += Form(" %3d %3d 0x%08x TSR %d%d%d Ch %2d %8.2f %6.2f",  // %3d %3d
                           fiNbHits, iNbChanInHit, iDetId, iSmType, iSm, iRpc, iChm, dWeightedTime, dWeightedPosY);

              cstr += Form(", DigiSize: %lu (%3lu)", vDigiIndRef.size(), fTofCalDigiVec->size());
              cstr += ", DigiInds: ";

              fviClusterMul[iSmType][iSm][iRpc]++;

              for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
                cstr += Form(" %3d (M,%2d)", vDigiIndRef.at(i), fviClusterMul[iSmType][iSm][iRpc]);
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

Bool_t CbmTofEventClusterizer::CalibRawDigis()
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
  //    LOG(debug)<<"CbmTofEventClusterizer::BuildClusters: Sort "<<fTofCalDigisColl->GetEntriesFast()<<" calibrated digis ";
  LOG(debug) << "CbmTofEventClusterizer::BuildClusters: Sort " << fTofCalDigiVec->size() << " calibrated digis ";
  if (iNbTofDigi > 1) {
    std::vector<CbmTofDigi>* tTofCalDigiVec = nullptr;
    if (NULL != fTofDigiPointMatches) {  // temporary copy
      tTofCalDigiVec = new std::vector<CbmTofDigi>(*fTofCalDigiVec);
    }

    //    fTofCalDigisColl->Sort(iNbTofDigi); // Time order again, in case modified by the calibration
    /// Sort the buffers of hits due to the time offsets applied
    std::sort(fTofCalDigiVec->begin(), fTofCalDigiVec->end(),
              [](const CbmTofDigi& a, const CbmTofDigi& b) -> bool { return a.GetTime() < b.GetTime(); });
    //    std::sort(fTofCalDigiVec->begin(), fTofCalDigiVec->end());
    //    if(!fTofCalDigisColl->IsSorted()){
    //    if ( ! std::is_sorted(fTofCalDigiVec->begin(), fTofCalDigiVec->end()))
    if (!std::is_sorted(fTofCalDigiVec->begin(), fTofCalDigiVec->end(),
                        [](const CbmTofDigi& a, const CbmTofDigi& b) -> bool { return a.GetTime() < b.GetTime(); }))
      LOG(warning) << "CbmTofEventClusterizer::BuildClusters: Sorting not successful ";

    if (NULL != fTofDigiPointMatches) {  // generate updated MC point Match Collection
      UInt_t iDigiOrg = 0;
      //LOG(info)<<Form("Fill MC Point Matches for %3lu, %3lu digis ",fTofCalDigiVec->size(),tTofCalDigiVec->size());
      for (UInt_t iDigi = 0; iDigi < fTofCalDigiVec->size(); iDigi++) {
        // find original Digi
        CbmTofDigi* outDigi = &(fTofCalDigiVec->at(iDigi));
        Bool_t bFound       = kFALSE;
        while (!bFound) {
          for (; iDigiOrg < tTofCalDigiVec->size(); iDigiOrg++) {
            CbmTofDigi* orgDigi = &(tTofCalDigiVec->at(iDigiOrg));
            if (outDigi->GetAddress() == orgDigi->GetAddress()) {
              CbmMatch digiMatch = (CbmMatch) fTofDigiPointMatches->at(iDigiOrg);
              ((std::vector<CbmMatch>*) fTofDigiPointMatchesOut)->push_back((CbmMatch) digiMatch);
              LOG(debug) << Form("Copy MC Point Match for 0x%08x, time %8.2f from %3d to %3d ", orgDigi->GetAddress(),
                                 outDigi->GetTime(), iDigiOrg, iDigi);
              bFound = kTRUE;
              break;
            }
            if (iDigiOrg == tTofCalDigiVec->size() - 1) iDigiOrg = 0;
          }
        }
      }
      delete tTofCalDigiVec;  // cleanup temporary vector
    }
  }

  return kTRUE;
}

void CbmTofEventClusterizer::SetDeadStrips(Int_t iDet, Int_t ival)
{
  if (fvDeadStrips.size() < static_cast<size_t>(iDet + 1)) fvDeadStrips.resize(iDet + 1);
  fvDeadStrips[iDet] = ival;
}
