/**
 * CbmDeviceHitBuilderTof.cxx
 *
 * @since 2018-05-31
 * @author N. Herrmann
 */

#include "CbmDeviceHitBuilderTof.h"

// CBM Classes and includes
#include "CbmDigiManager.h"

// TOF Classes and includes
#include "CbmMatch.h"
#include "CbmTofAddress.h"  // in cbmdata/tof
#include "CbmTofCell.h"     // in tof/TofData
#include "CbmTofClusterizersDef.h"
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

#include "FairEventHeader.h"
#include "FairFileHeader.h"
#include "FairGeoParSet.h"
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairRootFileSink.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

// ROOT Classes and includes
#include "TClonesArray.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TF2.h"
#include "TGeoManager.h"
#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include "TMath.h"
#include "TMinuit.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TVector3.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <chrono>
#include <iomanip>
#include <string>
#include <thread>  // this_thread::sleep_for

#include <stdexcept>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;

// Constants definitions
static Int_t iMess                = 0;
static Int_t iIndexDut            = 0;
static Double_t StartAnalysisTime = 0.;
const Double_t cLight             = 29.9792;  // in cm/ns
static FairRootManager* rootMgr   = NULL;
static Int_t iRunId               = 1;

CbmTofDigi* pRef;
CbmTofDigi* pRefCal;
CbmDigiManager* fDigiMan;

CbmDeviceHitBuilderTof::CbmDeviceHitBuilderTof()
  : fNumMessages(0)
  , fDigi(nullptr)
  , fGeoMan(NULL)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fDigiPar(NULL)
  , fChannelInfo(NULL)
  , fDigiBdfPar(NULL)
  , pDigiIn(NULL)
  , fiNDigiIn(0)
  , fvDigiIn()
  , fEventHeader()
  , fEvtHeader(NULL)
  , fTofHitsColl(NULL)
  , fTofDigiMatchColl(NULL)
  , fTofHitsCollOut(NULL)
  , fTofDigiMatchCollOut(NULL)
  , fiNbHits(0)
  , fiNevtBuild(0)
  , fiMsgCnt(100)
  , fdTOTMax(50.)
  , fdTOTMin(0.)
  , fdTTotMean(2.)
  , fdMaxTimeDist(0.)
  , fdMaxSpaceDist(0.)
  , fdEvent(0)
  , fiMaxEvent(-1)
  , fiRunId(111)
  , fbSwapChannelSides(kFALSE)
  , fiOutputTreeEntry(0)
  , fiFileIndex(0)
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
  , fvCPDelTof()
  , fvCPTOff()
  , fvCPTotGain()
  , fvCPTotOff()
  , fvCPWalk()
  , fvLastHits()
  , fvDeadStrips()
  , fvPulserOffset()
  , fvPulserTimes()
  , fhEvDetMul(NULL)
  , fhPulMul(NULL)
  , fhPulserTimesRaw()
  , fhPulserTimeRawEvo()
  , fhPulserTimesCor()
  , fhDigiTimesRaw()
  , fhDigiTimesCor()
  , fhRpcDigiTot()
  , fhRpcDigiCor()
  , fhRpcCluMul()
  , fhRpcCluRate()
  , fhRpcCluPosition()
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
  , fhTSmCluPosition()
  , fhTSmCluTOff()
  , fhTSmCluTRun()
  , fhTRpcCluTOffDTLastHits()
  , fhTRpcCluTotDTLastHits()
  , fhTRpcCluSizeDTLastHits()
  , fhTRpcCluMemMulDTLastHits()
  , fhSeldT()
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
  , fiMode(0)
  , fiPulserMode(0)
  , fiPulMulMin(0)
  , fiPulDetRef(0)
  , fiPulTotMin(0)
  , fiPulTotMax(1000)
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
  , fEnableMatchPosScaling(kTRUE)
  , fEnableAvWalk(kFALSE)
  , fbPs2Ns(kFALSE)
  , fCalParFileName("")
  , fOutHstFileName("")
  , fOutRootFileName("")
  , fCalParFile(NULL)
  , fOutRootFile(NULL)
  , fRootEvent(NULL) {}

CbmDeviceHitBuilderTof::~CbmDeviceHitBuilderTof() {
  if (NULL != fOutRootFile) {
    LOG(info) << "Finally close root file " << fOutRootFile->GetName();
    fOutRootFile->cd();
    rootMgr->LastFill();
    rootMgr->Write();
    WriteHistograms();
    fOutRootFile->Write();
    fOutRootFile->Close();
  }
}

void CbmDeviceHitBuilderTof::InitTask() try {
  // Get the information about created channels from the device
  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are possible/allowed
  // for the device
  // The idea is to check at initilization if the devices are
  // properly connected. For the time beeing this is done with a
  // nameing convention. It is not avoided that someone sends other
  // data on this channel.
  int noChannel = fChannels.size();
  LOG(info) << "Number of defined input channels: " << noChannel;
  for (auto const& entry : fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    if (!IsChannelNameAllowed(entry.first))
      throw InitTaskError("Channel name does not match.");
    if (entry.first != "syscmd")
      OnData(entry.first, &CbmDeviceHitBuilderTof::HandleData);
    else
      OnData(entry.first, &CbmDeviceHitBuilderTof::HandleMessage);
  }
  InitWorkspace();
  InitContainers();
  LoadGeometry();
  InitRootOutput();
} catch (InitTaskError& e) {
  LOG(error) << e.what();
  //ChangeState(ERROR_FOUND);
  ChangeState(fair::mq::Transition(ERROR_FOUND));
}

bool CbmDeviceHitBuilderTof::IsChannelNameAllowed(std::string channelName) {
  for (auto const& entry : fAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1 != std::string::npos) {
      const vector<std::string>::const_iterator pos =
        std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos - fAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName
                << " found in list of allowed channel names at position "
                << idx;
      return true;
    }
  }
  LOG(info) << "Channel name " << channelName
            << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

Bool_t CbmDeviceHitBuilderTof::InitWorkspace() {
  LOG(info) << "Init work space for CbmDeviceHitBuilderTof.";
  fOutRootFileName = fConfig->GetValue<string>("OutRootFile");
  fiMaxEvent       = fConfig->GetValue<int64_t>("MaxEvent");
  LOG(info) << "Max number of events to be processed: " << fiMaxEvent;
  fiRunId      = fConfig->GetValue<int64_t>("RunId");
  fiMode       = fConfig->GetValue<int64_t>("Mode");
  fiPulserMode = fConfig->GetValue<int64_t>("PulserMode");
  fiPulMulMin  = fConfig->GetValue<uint64_t>("PulMulMin");
  fiPulDetRef  = fConfig->GetValue<uint64_t>("PulDetRef");
  fiPulTotMin  = fConfig->GetValue<uint64_t>("PulTotMin");
  fiPulTotMax  = fConfig->GetValue<uint64_t>("PulTotMax");

  //fTofCalDigisColl     = new TClonesArray("CbmTofDigi", 100);
  //fTofCalDigisCollOut  = new TClonesArray("CbmTofDigi", 100);
  fTofCalDigiVec = new std::vector<CbmTofDigi>();

  fTofHitsColl         = new TClonesArray("CbmTofHit", 100);
  fTofHitsCollOut      = new TClonesArray("CbmTofHit", 100);
  fTofDigiMatchColl    = new TClonesArray("CbmMatch", 100);
  fTofDigiMatchCollOut = new TClonesArray("CbmMatch", 100);

  /*
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(error) << "HitBuilder: No digi input!";
    //return kFALSE;
  }
  */
  if (fOutRootFileName != "") {  // prepare root output

    FairRunOnline* fRun = new FairRunOnline(0);
    rootMgr             = FairRootManager::Instance();
    //fOutRootFile = rootMgr->OpenOutFile(fOutRootFileName);
    if (rootMgr->InitSink()) {
      fRun->SetSink(new FairRootFileSink(fOutRootFileName));
      fOutRootFile = rootMgr->GetOutFile();
      if (NULL == fOutRootFile) LOG(fatal) << "could not open root file";

    } else
      LOG(fatal) << "could not init Sink";
  }

  // steering variables
  fDutId  = fConfig->GetValue<uint64_t>("DutType");
  fDutSm  = fConfig->GetValue<uint64_t>("DutSm");
  fDutRpc = fConfig->GetValue<uint64_t>("DutRpc");

  fSelId  = fConfig->GetValue<uint64_t>("SelType");
  fSelSm  = fConfig->GetValue<uint64_t>("SelSm");
  fSelRpc = fConfig->GetValue<uint64_t>("SelRpc");

  fSel2Id  = fConfig->GetValue<uint64_t>("Sel2Type");
  fSel2Sm  = fConfig->GetValue<uint64_t>("Sel2Sm");
  fSel2Rpc = fConfig->GetValue<uint64_t>("Sel2Rpc");

  fiBeamRefType = fConfig->GetValue<uint64_t>("BRefType");
  fiBeamRefSm   = fConfig->GetValue<uint64_t>("BRefSm");
  fiBeamRefDet  = fConfig->GetValue<uint64_t>("BRefDet");

  return kTRUE;
}

Bool_t CbmDeviceHitBuilderTof::InitRootOutput() {
  if (NULL != fOutRootFile) {
    LOG(info) << "Init Root Output to " << fOutRootFile->GetName();

    /*
    fFileHeader->SetRunId(iRunId);
    rootMgr->WriteFileHeader(fFileHeader);
    */
    rootMgr->InitSink();
    fEvtHeader = new FairEventHeader();
    fEvtHeader->SetRunId(iRunId);
    rootMgr->Register("EventHeader.", "Event", fEvtHeader, kTRUE);
    rootMgr->FillEventHeader(fEvtHeader);

    // rootMgr->Register("CbmTofDigi", "Tof raw Digi", fTofCalDigisColl, kTRUE);
    //    fOutRootFile->cd();
    rootMgr->RegisterAny("TofCalDigi", fTofCalDigiVec, kTRUE);

    TTree* outTree = new TTree(FairRootManager::GetTreeName(), "/cbmout", 99);
    LOG(info) << "define Tree " << outTree->GetName();
    //rootMgr->TruncateBranchNames(outTree, "cbmout");
    //rootMgr->SetOutTree(outTree);
    rootMgr->GetSink()->SetOutTree(outTree);
    rootMgr->WriteFolder();
    LOG(info) << "Initialized outTree with rootMgr at " << rootMgr;
    /*
    fOutRootFile = new TFile(fOutRootFileName,"recreate");
    fRootEvent   = new TTree("CbmEvent","Cbm Event");
    fRootEvent->Branch("CbmDigi",fTofCalDigisColl);
    LOG(info)<<"Open Root Output file " << fOutRootFileName;
    fRootEvent->Write();
    */
  }
  return kTRUE;
}


Bool_t CbmDeviceHitBuilderTof::InitContainers() {
  LOG(info) << "Init parameter containers for CbmDeviceHitBuilderTof.";

  FairRuntimeDb* fRtdb = FairRuntimeDb::instance();
  if (NULL == fRtdb) LOG(error) << "No FairRuntimeDb found";

  // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
  // Should only be used for small data because of the cost of an additional copy

  // Int_t fiRunId=1535700811;  // from *geo*.par.root file
  Int_t NSet = 3;
  std::string parSet[NSet];
  parSet[0]           = "CbmTofDigiPar";
  parSet[1]           = "CbmTofDigiBdfPar";
  parSet[2]           = "FairGeoParSet";
  std::string Channel = "parameters";

  Bool_t isSimulation = kFALSE;
  Int_t iGeoVersion;
  FairParSet* cont;

  for (Int_t iSet = 1; iSet < NSet; iSet++) {
    std::string message = parSet[iSet] + "," + to_string(fiRunId);
    LOG(info) << "Requesting parameter container, sending message: " << message;

    FairMQMessagePtr req(NewSimpleMessage(message));
    //FairMQMessagePtr req(NewSimpleMessage( "CbmTofDigiBdfPar,111" )); //original format
    FairMQMessagePtr rep(NewMessage());
    CbmTofCreateDigiPar* tofDigiPar = NULL;

    if (Send(req, Channel) > 0) {
      if (Receive(rep, Channel) >= 0) {
        if (rep->GetSize() != 0) {
          CbmMQTMessage tmsg(rep->GetData(), rep->GetSize());
          switch (iSet) {
            case 0:
              fDigiPar =
                static_cast<CbmTofDigiPar*>(tmsg.ReadObject(tmsg.GetClass()));
              //fDigiPar->print();
              break;
            case 1:
              fDigiBdfPar = static_cast<CbmTofDigiBdfPar*>(
                tmsg.ReadObject(tmsg.GetClass()));
              //fDigiBdfPar->print();
              LOG(info) << "Calib data file: "
                        << fDigiBdfPar->GetCalibFileName();
              fdMaxTimeDist  = fDigiBdfPar->GetMaxTimeDist();     // in ns
              fdMaxSpaceDist = fDigiBdfPar->GetMaxDistAlongCh();  // in cm

              if (fMaxTimeDist != fdMaxTimeDist) {
                fdMaxTimeDist = fMaxTimeDist;  // modify default
                fdMaxSpaceDist =
                  fdMaxTimeDist * fDigiBdfPar->GetSignalSpeed()
                  * 0.5;  // cut consistently on positions (with default signal velocity)
              }

              break;
            case 2:  // Geometry container
              cont = static_cast<FairParSet*>(tmsg.ReadObject(tmsg.GetClass()));
              cont->init();
              cont->Print();
              //fRtdb->InitContainer(parSet[iSet]);
              if (NULL == fGeoMan)
                fGeoMan = (TGeoManager*) ((FairGeoParSet*) cont)
                            ->GetGeometry();  //crashes
              LOG(info) << "GeoMan: " << fGeoMan << " " << gGeoManager;
              iGeoVersion = fGeoHandler->Init(isSimulation);
              if (k21a > iGeoVersion) {
                LOG(error) << "Incompatible geometry !!!";
                //ChangeState(STOP);
                ChangeState(fair::mq::Transition(STOP));
              }
              switch (iGeoVersion) {
                case k14a: fTofId = new CbmTofDetectorId_v14a(); break;
                case k21a: fTofId = new CbmTofDetectorId_v21a(); break;
              }
              if (NULL == fDigiPar) {
                if (NULL == fRtdb) {
                  LOG(info) << "Instantiate FairRunDb";
                  fRtdb = FairRuntimeDb::instance();
                  assert(fRtdb);
                }
                // create digitization parameters from geometry file
                tofDigiPar =
                  new CbmTofCreateDigiPar("TOF Digi Producer", "TOF task");
                LOG(info) << "Create DigiPar ";
                tofDigiPar->Init();
                fDigiPar =
                  (CbmTofDigiPar*) (fRtdb->getContainer("CbmTofDigiPar"));
                if (NULL == fDigiPar) {
                  LOG(error) << "CbmTofEventClusterizer::InitParameters => "
                                "Could not obtain "
                                "the CbmTofDigiPar ";
                  return kFALSE;
                }
              }
              gGeoManager->Export("HitBuilder.geo.root");
              break;

            case 3:  // Calib
              break;
            default:
              LOG(warn) << "Parameter Set " << iSet << " not implemented ";
          }
          LOG(info) << "Received parameter from server:";
        } else {
          LOG(warn) << "Received empty reply. Parameter not available";
        }
      }
    }
  }
  Bool_t initOK = ReInitContainers();

  CreateHistograms();

  if (!InitCalibParameter())
    return kFALSE;  // ChangeState(PAUSE); // for debugging

  fDutAddr  = CbmTofAddress::GetUniqueAddress(fDutSm, fDutRpc, 0, 0, fDutId);
  fSelAddr  = CbmTofAddress::GetUniqueAddress(fSelSm, fSelRpc, 0, 0, fSelId);
  fSel2Addr = CbmTofAddress::GetUniqueAddress(fSel2Sm, fSel2Rpc, 0, 0, fSel2Id);
  fiBeamRefAddr = CbmTofAddress::GetUniqueAddress(
    fiBeamRefSm, fiBeamRefDet, 0, 0, fiBeamRefType);
  iIndexDut = fDigiBdfPar->GetDetInd(fDutAddr);
  LOG(info) << Form("Use Dut 0x%08x, Sel 0x%08x, Sel2 0x%08x, BRef 0x%08x",
                    fDutAddr,
                    fSelAddr,
                    fSel2Addr,
                    fiBeamRefAddr);
  return initOK;
}

Bool_t CbmDeviceHitBuilderTof::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for CbmDeviceHitBuilderTof.";

  return kTRUE;
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
//bool CbmDeviceHitBuilderTof::HandleData(FairMQMessagePtr& msg, int /*index*/)
bool CbmDeviceHitBuilderTof::HandleData(FairMQParts& parts, int /*index*/) {
  // Don't do anything with the data
  // Maybe add an message counter which counts the incomming messages and add
  // an output
  fNumMessages++;
  LOG(debug) << "Received message " << fNumMessages << " with " << parts.Size()
             << " parts"
             << ", size0: " << parts.At(0)->GetSize();

  std::string msgStrE(static_cast<char*>(parts.At(0)->GetData()),
                      (parts.At(0))->GetSize());
  std::istringstream issE(msgStrE);
  boost::archive::binary_iarchive inputArchiveE(issE);
  inputArchiveE >> fEventHeader;
  LOG(debug) << "EventHeader: " << fEventHeader[0] << " " << fEventHeader[1]
             << " " << fEventHeader[2];

  fiNDigiIn = 0;
  //  LOG(debug) << "Received message # "<<  fNumMessages
  //	     << " with size " << msg->GetSize()<<" at "<< msg->GetData();

  //std::string msgStr(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::string msgStr(static_cast<char*>(parts.At(1)->GetData()),
                     (parts.At(1))->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  std::vector<CbmTofDigi*> vdigi;
  inputArchive >> vdigi;

  /*  ---- for debugging ----------------
  int *pData = static_cast <int *>(msg->GetData());
  for (int iData=0; iData<msg->GetSize()/NBytes; iData++) {
    LOG(info) << Form(" ind %d, poi %p, data: 0x%08x",iData,pData,*pData++);
  }
  */

  //vector descriptor and data separated -> transfer of vectors does not work reliably
  //std::vector<CbmTofDigi>* vdigi = static_cast<std::vector<CbmTofDigi>*>(msg->GetData());
  //  (*vdigi).resize(fiNDigiIn);
  LOG(debug) << "vdigi vector at " << vdigi.data() << " with size "
             << vdigi.size();

  for (UInt_t iDigi = 0; iDigi < vdigi.size(); iDigi++) {
    LOG(debug) << "#" << iDigi << " " << vdigi[iDigi]->ToString();
  }

  /*
  const Int_t iNDigiIn=100;
  std::array<CbmTofDigi,iNDigiIn> *aTofDigi = static_cast<std::array<CbmTofDigi,iNDigiIn>*>(msg->GetData());
  for (int iDigi=0; iDigi<fiNDigiIn; iDigi++) {
    LOG(info) << "#" << iDigi << " " <<(*aTofDigi)[iDigi].ToString();  
  }
  

  pDigiIn=static_cast<CbmTofDigi*> (msg->GetData());
  CbmTofDigi*  pDigi=pDigiIn;
  CbmTofDigi  aTofDigi[fiNDigiIn];
 

  for (int iDigi=0; iDigi<fiNDigiIn; iDigi++) {
  //aTofDigi[iDigi] = *pDigi++; 
    aTofDigi[iDigi] = *pDigi; 
    fvDigiIn[iDigi] = *pDigi; 
    LOG(info) << "#" << iDigi << " at "<<pDigi<< " " <<aTofDigi[iDigi].ToString();  
    // LOG(info) << "#" << iDigi << " at "<<pDigi<< " " <<pDigi->ToString();   // does not work ???
    pDigi++; 
  }
  */

  fiNDigiIn = vdigi.size();
  fvDigiIn.resize(fiNDigiIn);
  for (int iDigi = 0; iDigi < fiNDigiIn; iDigi++) {
    fvDigiIn[iDigi] = *vdigi[iDigi];
    delete vdigi[iDigi];
  }
  vdigi.clear();

  //  for( Int_t i = 0; i < fTofCalDigisColl->GetEntriesFast(); i++ ) ((CbmTofDigi*) fTofCalDigisColl->At(i))->Delete();
  // fTofCalDigisColl->Clear("C");

  //  for( Int_t i = 0; i < fTofHitsColl->GetEntriesFast(); i++ ) ((CbmTofHit*) fTofHitsColl->At(i))->Delete();
  fTofHitsColl->Clear("C");
  //fTofHitsColl->Delete(); // Are the elements deleted?
  //fTofHitsColl         = new TClonesArray("CbmTofHit",100);

  //for( Int_t i = 0; i < fTofDigiMatchColl->GetEntriesFast(); i++ ) ((CbmMatch*) fTofDigiMatchColl->At(i))->Delete();
  fTofDigiMatchColl->Clear("C");

  fiNbHits = 0;
  if (fNumMessages % 10000 == 0)
    LOG(info) << "Processed " << fNumMessages << " messages";
  if (fEventHeader.size() > 3) {
    fhPulMul->Fill((Double_t) fEventHeader[3]);
    if (fEventHeader[3] > 0) {
      //LOG(debug) << "Pulser event found, Mul "<< fEventHeader[3];
      if (!MonitorPulser()) return kFALSE;
      return kTRUE;  // separate events from pulser
    }
  }

  LOG(debug) << " Process msg " << fNumMessages << " at evt " << fdEvent
             << ", PulMode " << fiPulserMode;

  if (fiPulserMode
      > 0) {  // don't process events without valid pulser correction
    if (fvPulserTimes[fiPulDetRef][0].size() == 0) return kTRUE;
  }

  fdEvent++;
  fhEvDetMul->Fill((Double_t) fEventHeader[1]);
  if (!InspectRawDigis()) return kFALSE;

  if (fiPulserMode > 0)
    if (!ApplyPulserCorrection()) return kFALSE;

  if (!BuildClusters()) return kFALSE;
  //if(!MergeClusters())       return kFALSE;

  if (NULL != fOutRootFile) {  // CbmEvent output to root file
    rootMgr->FillEventHeader(fEvtHeader);
    //LOG(info) << "Fill WriteOutBuffer with rootMgr at " << rootMgr;
    fOutRootFile->cd();
    rootMgr->Fill();
    rootMgr->StoreWriteoutBufferData(rootMgr->GetEventTime());
    //rootMgr->StoreAllWriteoutBufferData();
    rootMgr->DeleteOldWriteoutBufferData();
    if ((Int_t) fdEvent == fiMaxEvent) {
      rootMgr->Write();
      WriteHistograms();
      fOutRootFile->Close();
      LOG(info) << "File closed after " << fdEvent << " events. ";
      //ChangeState(STOP);
      ChangeState(fair::mq::Transition(STOP));
    }
  }
  if (!FillHistos())
    return kTRUE;  // event not selected for histogramming, skip sending it
  //if(!SendHits())      return kFALSE;
  if (!SendAll()) return kFALSE;

  return kTRUE;
}

/************************************************************************************/

bool CbmDeviceHitBuilderTof::HandleMessage(FairMQMessagePtr& msg,
                                           int /*index*/) {
  const char* cmd    = (char*) (msg->GetData());
  const char cmda[4] = {*cmd};
  LOG(info) << "Handle message " << cmd << ", " << cmd[0];
  //LOG(info) << "Current State: " << fair::mq::StateMachine::GetCurrentStateName();

  // only one implemented so far "Stop"
  if (NULL != fOutRootFile) {
    LOG(info) << "Close root file " << fOutRootFile->GetName();
    fOutRootFile->cd();
    rootMgr->LastFill();
    rootMgr->Write();
    WriteHistograms();
    fOutRootFile->Write();
    fOutRootFile->Close();
  }

  if (strcmp(cmda, "STOP")) {
    LOG(info) << "STOP";
    /*  Invalid syntax
    ChangeState(internal_READY);
    LOG(info) << "Current State: " <<  FairMQStateMachine::GetCurrentStateName();
    ChangeState(internal_DEVICE_READY);
    LOG(info) << "Current State: " <<  FairMQStateMachine::GetCurrentStateName();
    ChangeState(internal_IDLE);
    LOG(info) << "Current State: " <<  FairMQStateMachine::GetCurrentStateName();
    ChangeState(END);
    LOG(info) << "Current State: " <<  FairMQStateMachine::GetCurrentStateName();
    */
    //    ChangeState(fair::mq::Transition(internal_READY));
    //ChangeState(fair::mq::Transition(internal_DEVICE_READY));
    //ChangeState(fair::mq::Transition(internal_IDLE));
    ChangeState(fair::mq::Transition(STOP));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ChangeState(fair::mq::Transition(END));
  }

  return true;
}

/************************************************************************************/
Bool_t CbmDeviceHitBuilderTof::InitCalibParameter() {
  // dimension and initialize calib parameter
  // Int_t iNbDet     = fDigiBdfPar->GetNbDet();
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();

  fTotMean = 1.;
  fCalMode = -1;

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
        //LOG(info)<<Form(" fvCPDelTof resize for SmT %d, R %d, B %d ",iSmType,iNbSm*iNbRpc,nbClDelTofBinX);
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
  LOG(info) << "defaults set";

  TDirectory* oldir =
    gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  /*
  gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !
  */

  if (0 < fCalMode) {
    fCalParFileName = fDigiBdfPar->GetCalibFileName();
    LOG(info) << "InitCalibParameter: read histos from "
              << "file " << fCalParFileName;

    // read parameter from histos
    if (fCalParFileName.IsNull()) return kTRUE;

    fCalParFile = new TFile(fCalParFileName, "");
    if (NULL == fCalParFile) {
      LOG(error) << "InitCalibParameter: "
                 << "file " << fCalParFileName << " does not exist!";
      //ChangeState(STOP);
      ChangeState(fair::mq::Transition(STOP));
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
        gDirectory->cd(oldir->GetPath());
        //TProfile *hSvelmem =
        (TProfile*) hSvel->Clone();
        gDirectory->cd(curdir->GetPath());
      } else {
        LOG(info) << "Svel histogram not found for module type " << iSmType;
      }

      for (Int_t iPar = 0; iPar < 4; iPar++) {
        TProfile* hFparcur = (TProfile*) gDirectory->FindObjectAny(
          Form("cl_SmT%01d_Fpar%1d", iSmType, iPar));
        if (NULL != hFparcur) {
          gDirectory->cd(oldir->GetPath());
          //TProfile *hFparmem =
          (TProfile*) hFparcur->Clone();
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

              LOG(debug) << "InitCalibParameter:"
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
                  LOG(error)
                    << "InitCalibParameter: Inconsistent Walk histograms";
                for (Int_t iBin = 0; iBin < nbClWalkBinX; iBin++) {
                  fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iBin] =
                    htempWalk0->GetBinContent(iBin + 1);
                  fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iBin] =
                    htempWalk1->GetBinContent(iBin + 1);
                  //LOG(debug)<<Form(" SmT%01d_sm%03d_rpc%03d_Ch%03d bin %d walk %f ",iSmType, iSm, iRpc, iCh, iBin,
                  //		     fvCPWalk[iSmType][iSm*iNbRpc+iRpc][iCh][0][iBin]);
                  if (5 == iSmType || 8 == iSmType) {  // Pad structure
                    fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][1][iBin] =
                      fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][0][iBin];
                  }
                }
              }
            }
          } else {
            LOG(warn) << " Calibration histos "
                      << Form(
                           "cl_SmT%01d_sm%03d_rpc%03d_XXX", iSmType, iSm, iRpc)
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
            gDirectory->cd(oldir->GetPath());
            TH1D* h1DelTof = (TH1D*) htmpDelTof->Clone(
              Form("cl_CorSmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof",
                   iSmType,
                   iSm,
                   iRpc,
                   iSel));

            LOG(debug) << " copy histo " << h1DelTof->GetName()
                       << " to directory " << oldir->GetName();

            gDirectory->cd(curdir->GetPath());
          }
        }
    }
  }
  //   fCalParFile->Delete();
  gDirectory->cd(
    oldir
      ->GetPath());  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  LOG(info) << "InitCalibParameter: initialization done";
  return kTRUE;
}

/************************************************************************************/
// Histogram definitions
void CbmDeviceHitBuilderTof::CreateHistograms() {
  TDirectory* oldir =
    gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gROOT
    ->cd();  // <= To prevent histos from being sucked in by the param file of the TRootManager !
  // process event header info
  fhEvDetMul =
    new TH1F("hEvDetMul", "Detector multiplicity of Selector; Mul", 50, 0, 50);
  fhPulMul = new TH1F("hPulMul", "Pulser multiplicity; Mul", 110, 0, 110);

  Int_t iNDet = 0;
  for (Int_t iModTyp = 0; iModTyp < 10; iModTyp++)
    iNDet += fDigiBdfPar->GetNbSm(iModTyp) * fDigiBdfPar->GetNbRpc(iModTyp);

  fhPulserTimesRaw =
    new TH2F(Form("hPulserTimesRaw"),
             Form("Pulser Times uncorrected; Det# []; t - t0 [ns]"),
             iNDet * 2,
             0,
             iNDet * 2,
             999,
             -100.,
             100.);

  fhPulserTimeRawEvo.resize(iNDet * 2);
  for (Int_t iDetSide = 0; iDetSide < iNDet * 2; iDetSide++) {
    fhPulserTimeRawEvo[iDetSide] = new TProfile(
      Form("hPulserTimeRawEvo_%d", iDetSide),
      Form("Raw Pulser TimeEvolution  %d; PulserEvent# ; DeltaT [ns] ",
           iDetSide),
      1000,
      0.,
      1.E5,
      -100.,
      100.);
  }

  fhPulserTimesCor =
    new TH2F(Form("hPulserTimesCor"),
             Form("Pulser Times corrected; Det# []; t - t0 [ns]"),
             iNDet * 2,
             0,
             iNDet * 2,
             999,
             -10.,
             10.);

  fhDigiTimesRaw =
    new TH2F(Form("hDigiTimesRaw"),
             Form("Digi Times uncorrected; Det# []; t - t0 [ns]"),
             iNDet * 2,
             0,
             iNDet * 2,
             999,
             -100.,
             100.);

  fhDigiTimesCor = new TH2F(Form("hDigiTimesCor"),
                            Form("Digi Times corrected; Det# []; t - t0 [ns]"),
                            iNDet * 2,
                            0,
                            iNDet * 2,
                            999,
                            -100.,
                            100.);

  Int_t iNbDet = fDigiBdfPar->GetNbDet();
  fDetIdIndexMap.clear();
  fhRpcDigiTot.resize(iNbDet);
  fhRpcDigiCor.resize(iNbDet);
  fviDetId.resize(iNbDet);
  for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
    Int_t iUniqueId           = fDigiBdfPar->GetDetUId(iDetIndx);
    fDetIdIndexMap[iUniqueId] = iDetIndx;
    fviDetId[iDetIndx]        = iUniqueId;
    Int_t iSmType             = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSmId               = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpcId              = CbmTofAddress::GetRpcId(iUniqueId);
    fhRpcDigiTot[iDetIndx]    = new TH2F(
      Form("hDigiTot_SmT%01d_sm%03d_rpc%03d", iSmType, iSmId, iRpcId),
      Form("Digi Tot of Rpc #%03d in Sm %03d of type %d; digi 0; digi 1",
           iRpcId,
           iSmId,
           iSmType),
      2 * fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      0,
      2 * fDigiBdfPar->GetNbChan(iSmType, iRpcId),
      256,
      0,
      256);

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
  }

  if (0 == fiMode) return;  // no cluster histograms needed

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
      LOG(warn) << "No DigiPar for SmType " << Form("%d, 0x%08x", iS, iUCellId);
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
               fDigiBdfPar->GetNbSm(iS) * fDigiBdfPar->GetNbRpc(iS),
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
  LOG(info) << " Define Clusterizer histos for " << iNbDet << " detectors ";

  fhRpcCluMul.resize(iNbDet);
  fhRpcCluRate.resize(iNbDet);
  fhRpcCluPosition.resize(iNbDet);
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
    Int_t iSmType   = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSmId     = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpcId    = CbmTofAddress::GetRpcId(iUniqueId);
    Int_t iUCellId =
      CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, 0, 0, iSmType);
    fChannelInfo = fDigiPar->GetCell(iUCellId);
    if (NULL == fChannelInfo) {
      LOG(warn) << "No DigiPar for Det " << Form("0x%08x", iUCellId);
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
        LOG(warn) << Form(
          "missing ChannelInfo for ch %d addr 0x%08x", iCh, iCCellId);
    }

    fChannelInfo = fDigiPar->GetCell(iUCellId);

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
           "[ns]",
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
      Form("Clu Tot of Rpc #%03d in Sm %03d of type %d; StripSide []; TOT [ns]",
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
    fhRpcCluAvWalk[iDetIndx] = new TH2F(
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

  // Trigger selected histograms
  if (0 < iNSel) {

    fhSeldT.resize(iNSel);
    for (Int_t iSel = 0; iSel < iNSel; iSel++) {
      fhSeldT[iSel] = new TH2F(Form("cl_dt_Sel%02d", iSel),
                               Form("Selector time %02d; dT [ns]", iSel),
                               99,
                               -fdDelTofMax * 10.,
                               fdDelTofMax * 10.,
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
        LOG(warn) << "No DigiPar for Det " << Form("0x%08x", iUCellId);
        continue;
      }
      LOG(debug) << "DetIndx " << iDetIndx << ", SmType " << iSmType
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
          LOG(error) << " Histo not generated !";

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
                   99,
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
                        "Selector %02d; StripSide []; TOT [ns]",
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
          Form("SmT%01d_sm%03d_rpc%03d_Sel%02d_DelTof; TRef-TSel; T-TSel",
               iSmType,
               iSmId,
               iRpcId,
               iSel),
          nbClDelTofBinX,
          -fdDelTofMax,
          fdDelTofMax,
          nbClDelTofBinY,
          -TSumMax,
          TSumMax);

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
                        "Selector %02d; log(#DeltaT (ns)); TOT [ns]",
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

  gDirectory->cd(
    oldir
      ->GetPath());  // <= To prevent histos from being sucked in by the param file of the TRootManager!

  return;
}
/************************************************************************************/
void CbmDeviceHitBuilderTof::WriteHistograms() {
  TList* tHistoList(NULL);
  tHistoList = gROOT->GetList();

  TIter next(tHistoList);
  // Write histogramms to the file
  fOutRootFile->cd();
  {
    TH1* h;
    TObject* obj;
    while ((obj = (TObject*) next())) {
      if (obj->InheritsFrom(TH1::Class())) {
        h = (TH1*) obj;
        //         cout << "Write histo " << h->GetTitle() << endl;
        h->Write();
      }
    }
  }
}

/************************************************************************************/
Bool_t CbmDeviceHitBuilderTof::BuildClusters() {
  fiNevtBuild++;
  if (!CalibRawDigis()) return kFALSE;
  if (0 == fiMode) return kTRUE;  // no customer yet
  if (!FillDigiStor()) return kFALSE;
  if (!BuildHits()) return kFALSE;
  return kTRUE;
}

/************************************************************************************/
Bool_t CbmDeviceHitBuilderTof::InspectRawDigis() {
  Int_t iNbTofDigi = fiNDigiIn;
  pRef             = NULL;
  for (Int_t iDigInd = 0; iDigInd < fiNDigiIn; iDigInd++) {
    CbmTofDigi* pDigi = &fvDigiIn[iDigInd];
    //LOG(debug)<<iDigInd<<" "<<pDigi;
    /*
    LOG(debug)<<iDigInd
	      //<<Form(" Address : 0x%08x ",pDigi->GetAddress())
	      <<" TSRC " << pDigi->GetType()
	      << pDigi->GetSm()
	      << pDigi->GetRpc()
	      << pDigi->GetChannel()
	      <<" S " << pDigi->GetSide()
    	      <<" : " << pDigi->ToString();
    */
    Int_t iAddr = pDigi->GetAddress() & DetMask;
    if (iAddr == fiBeamRefAddr) {
      //LOG(debug) << Form("Ref digi found for 0x%08x, Mask  0x%08x ", fiBeamRefAddr, DetMask);
      if (NULL == pRef)
        pRef = pDigi;
      else {
        if (pDigi->GetTime() < pRef->GetTime()) pRef = pDigi;
      }
    }
    Int_t iDetIndx = fDigiBdfPar->GetDetInd(iAddr);

    if (fDigiBdfPar->GetNbDet() - 1 < iDetIndx || iDetIndx < 0) {
      LOG(debug) << Form(
        " Wrong DetIndx %d >< %d,0 ", iDetIndx, fDigiBdfPar->GetNbDet());
      break;
    }

    fhRpcDigiTot[iDetIndx]->Fill(2 * pDigi->GetChannel() + pDigi->GetSide(),
                                 pDigi->GetTot());

    if (NULL == fhRpcDigiCor[iDetIndx]) {
      if (100 < iMess++)
        LOG(warn) << Form(
          " DigiCor Histo for  DetIndx %d derived from 0x%08x not found",
          iDetIndx,
          pDigi->GetAddress());
      continue;
    }

    Double_t dTDifMin     = dDoubleMax;
    CbmTofDigi* pDigi2Min = NULL;
    //       for (Int_t iDigI2 =iDigInd+1; iDigI2<iNbTofDigi;iDigI2++){
    for (Int_t iDigI2 = 0; iDigI2 < iNbTofDigi; iDigI2++) {
      CbmTofDigi* pDigi2 = &fvDigiIn[iDigI2];
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
              //LOG(debug) << "Digi2 found at "<<iDigI2<<" with TDif = "<<dTDifMin<<" ns";
            }
          } else if (
            TMath::Abs(pDigi->GetChannel() - pDigi2->GetChannel())
            == 1) {  // opposite side missing, neighbouring channel has hit on opposite side // FIXME
            // check that same side digi of neighbouring channel is absent
            Int_t iDigI3 = 0;
            for (; iDigI3 < iNbTofDigi; iDigI3++) {
              CbmTofDigi* pDigi3 = &fvDigiIn[iDigI3];
              if (pDigi3->GetSide() == pDigi->GetSide()
                  && pDigi2->GetChannel() == pDigi3->GetChannel())
                break;
            }
            if (iDigI3 == iNbTofDigi) {  // same side neighbour did not fire
              Int_t iCorMode = 0;        // Missing hit correction mode
              switch (iCorMode) {
                case 0:  // no action
                  break;
                case 1:  // shift found hit
                  LOG(debug) << Form("shift channel %d%d%d%d%d and  ",
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

                  LOG(debug) << Form("resultchannel %d%d%d%d%d and  ",
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
                  CbmTofDigi* pDigiN = new CbmTofDigi(*pDigi);
                  pDigiN->SetAddress(pDigi->GetSm(),
                                     pDigi->GetRpc(),
                                     pDigi2->GetChannel(),
                                     pDigi->GetSide(),
                                     pDigi->GetType());
                  pDigiN->SetTot(pDigi2->GetTot());
                  fvDigiIn.push_back(*pDigiN);

                  CbmTofDigi* pDigiN2 = new CbmTofDigi(*pDigi2);
                  pDigiN2->SetAddress(pDigi2->GetSm(),
                                      pDigi2->GetRpc(),
                                      pDigi->GetChannel(),
                                      pDigi2->GetSide(),
                                      pDigi2->GetType());
                  pDigiN2->SetTot(pDigi->GetTot());
                  fvDigiIn.push_back(*pDigiN2);

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
        LOG(warn) << Form("Invalid ChannelInfo for 0x%08x, 0x%08x",
                          iChId, pDigi2Min->GetAddress())
		  << " TSRC " << pDigi->GetType()
		  <<pDigi->GetSm() <<pDigi->GetRpc()<<pDigi->GetChannel();
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
              LOG(warn) << "Inconsistent duplicated digis in event "
                        << fiNevtBuild << ", Ind: " << iDigInd;
              LOG(warn) << "   " << pDigi->ToString();
              LOG(warn) << "   " << pDigi2Min->ToString();
            }
            pDigi2Min->SetTot(pDigi->GetTot());
            pDigi2Min->SetTime(pDigi->GetTime());
          }
        }
      }
    }
  }

  if (NULL != pRef) {
    // LOG(debug) << Form("pRef from 0x%08x ",pRef->GetAddress());

    for (Int_t iDigInd = 0; iDigInd < fiNDigiIn; iDigInd++) {
      CbmTofDigi* pDigi = &fvDigiIn[iDigInd];
      Int_t iAddr       = pDigi->GetAddress() & DetMask;
      Int_t iDet        = fDetIdIndexMap[iAddr];  // Detector Index
      Int_t iSide       = pDigi->GetSide();
      fhDigiTimesRaw->Fill(iDet * 2 + iSide,
                           pDigi->GetTime() - pRef->GetTime());
    }
  }
  return kTRUE;
}

/************************************************************************************/
Bool_t CbmDeviceHitBuilderTof::CalibRawDigis() {
  CbmTofDigi* pDigi;
  CbmTofDigi* pCalDigi = NULL;
  Int_t iDigIndCal     = -1;
  // channel deadtime map
  std::map<Int_t, Double_t> mChannelDeadTime;
  fTofCalDigiVec->clear();

  Int_t iNbTofDigi = fvDigiIn.size();
  for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
    pDigi       = (CbmTofDigi*) &fvDigiIn[iDigInd];
    Int_t iAddr = pDigi->GetAddress();
    /*
    LOG(debug)<<"BC "  // Before Calibration
	      <<Form("0x%08x",pDigi->GetAddress())<<" TSRC "
	      <<pDigi->GetType()
	      <<pDigi->GetSm()
	      <<pDigi->GetRpc()
	      <<Form("%2d",(Int_t)pDigi->GetChannel())<<" "
	      <<pDigi->GetSide()<<" "
	      <<Form("%f",pDigi->GetTime())<<" "
	      <<pDigi->GetTot();
    */
    if (pDigi->GetType() == 5
        || pDigi->GetType()
             == 8)  // for Pad counters generate fake digi to mockup a strip
      if (pDigi->GetSide() == 1)
        continue;  // skip one side to avoid double entries

    Bool_t bValid = kTRUE;
    std::map<Int_t, Double_t>::iterator it;
    it = mChannelDeadTime.find(iAddr);
    if (it != mChannelDeadTime.end()) {
      /*
      LOG(debug)<<"CCT found valid ChannelDeadtime entry "<<mChannelDeadTime[iAddr]
		<<", DeltaT "<<pDigi->GetTime()-mChannelDeadTime[iAddr];
      */
      if ((bValid = (pDigi->GetTime()
                     > mChannelDeadTime[iAddr] + fdChannelDeadtime))) {
        //  pCalDigi =
        //	new ((*fTofCalDigisColl)[++iDigIndCal]) CbmTofDigi(*pDigi);
        fTofCalDigiVec->push_back(CbmTofDigi(*pDigi));
        pCalDigi = &(fTofCalDigiVec->back());
        iDigIndCal++;
      }
    } else {
      // pCalDigi = new ((*fTofCalDigisColl)[++iDigIndCal]) CbmTofDigi(*pDigi);
      fTofCalDigiVec->push_back(CbmTofDigi(*pDigi));
      pCalDigi = &(fTofCalDigiVec->back());
      iDigIndCal++;
    }
    mChannelDeadTime[iAddr] = pDigi->GetTime();
    if (!bValid) continue;
    if (pRef != NULL)
      if (pDigi == pRef) pRefCal = pCalDigi;
    /*
    LOG(debug)<<"DC "  // After deadtime check. before Calibration
	      <<Form("0x%08x",pDigi->GetAddress())<<" TSRC "
	      <<pDigi->GetType()
	      <<pDigi->GetSm()
	      <<pDigi->GetRpc()
	      <<Form("%2d",(Int_t)pDigi->GetChannel())<<" "
	      <<pDigi->GetSide()<<" "
	      <<Form("%f",pDigi->GetTime())<<" "
	      <<pDigi->GetTot();
    */
    if (fbPs2Ns) {
      pCalDigi->SetTime(pCalDigi->GetTime()
                        / 1000.);  // for backward compatibility
      pCalDigi->SetTot(pCalDigi->GetTot()
                       / 1000.);  // for backward compatibility
    }
    if (fDigiBdfPar->GetNbSmTypes()
          > pDigi->GetType()  // prevent crash due to misconfiguration
        && fDigiBdfPar->GetNbSm(pDigi->GetType()) > pDigi->GetSm()
        && fDigiBdfPar->GetNbRpc(pDigi->GetType()) > pDigi->GetRpc()
        && fDigiBdfPar->GetNbChan(pDigi->GetType(), pDigi->GetRpc())
             > pDigi->GetChannel()) {
      // apply calibration vectors
      pCalDigi->SetTime(
        pCalDigi->GetTime() -  // calibrate Digi Time
        fvCPTOff[pDigi->GetType()]
                [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                 + pDigi->GetRpc()][pDigi->GetChannel()][pDigi->GetSide()]);
      //      LOG(debug)<<" CluCal-TOff: "<<pCalDigi->ToString();

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
                    + pDigi->GetRpc()][pDigi->GetChannel()][pDigi->GetSide()]);

      // walk correction
      Double_t dTotBinSize = (fdTOTMax - fdTOTMin) / nbClWalkBinX;
      Int_t iWx = (Int_t)((pCalDigi->GetTot() - fdTOTMin) / dTotBinSize);
      if (0 > iWx) iWx = 0;
      if (iWx >= nbClWalkBinX) iWx = nbClWalkBinX - 1;
      Double_t dDTot =
        (pCalDigi->GetTot() - fdTOTMin) / dTotBinSize - (Double_t) iWx - 0.5;
      Double_t dWT =
        fvCPWalk[pCalDigi->GetType()]
                [pCalDigi->GetSm() * fDigiBdfPar->GetNbRpc(pCalDigi->GetType())
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
      // LOG(debug)<<" CluCal-Walk: "<<pCalDigi->ToString();

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
      //CbmTofDigi* pCalDigi2 =
      //  new ((*fTofCalDigisColl)[++iDigIndCal]) CbmTofDigi(*pCalDigi);
      fTofCalDigiVec->push_back(CbmTofDigi(*pCalDigi));
      CbmTofDigi* pCalDigi2 = &(fTofCalDigiVec->back());
      iDigIndCal++;
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
    }
  }  // for( Int_t iDigInd = 0; iDigInd < nTofDigi; iDigInd++ )

  iNbTofDigi = fTofCalDigiVec->size();
  //  fTofCalDigisColl->GetEntries();  // update because of added duplicted digis
  LOG(debug) << "CbmTofHitBuilder: Sort " << fTofCalDigiVec->size()
             << " calibrated digis ";
  if (iNbTofDigi > 1) {
    //    fTofCalDigisColl->Sort(iNbTofDigi); // Time order again, in case modified by the calibration
    /// Sort the buffers of hits due to the time offsets applied
    std::sort(fTofCalDigiVec->begin(),
              fTofCalDigiVec->end(),
              [](const CbmTofDigi& a, const CbmTofDigi& b) -> bool {
                return a.GetTime() < b.GetTime();
              });
    //    std::sort(fTofCalDigiVec->begin(), fTofCalDigiVec->end());
    //    if(!fTofCalDigisColl->IsSorted()){
    //    if ( ! std::is_sorted(fTofCalDigiVec->begin(), fTofCalDigiVec->end()))
    if (!std::is_sorted(fTofCalDigiVec->begin(),
                        fTofCalDigiVec->end(),
                        [](const CbmTofDigi& a, const CbmTofDigi& b) -> bool {
                          return a.GetTime() < b.GetTime();
                        }))
      LOG(warning) << "CbmTofHitBuilder: Digi sorting not successful ";
  }

  if (NULL != pRef) {
    // LOG(debug) << Form("pRef from 0x%08x ",pRef->GetAddress());

    for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
      // pDigi       = (CbmTofDigi*) fTofCalDigisColl->At(iDigInd);
      pDigi       = &(fTofCalDigiVec->at(iDigInd));
      Int_t iAddr = pDigi->GetAddress() & DetMask;
      Int_t iDet  = fDetIdIndexMap[iAddr];  // Detector Index
      Int_t iSide = pDigi->GetSide();
      fhDigiTimesCor->Fill(iDet * 2 + iSide,
                           pDigi->GetTime() - pRefCal->GetTime());
    }
  }
  return kTRUE;
}

/************************************************************************************/
Bool_t CbmDeviceHitBuilderTof::BuildHits() {
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();
  // Hit variables
  Double_t dWeightedTime = 0.0;
  Double_t dWeightedPosX = 0.0;
  Double_t dWeightedPosY = 0.0;
  Double_t dWeightedPosZ = 0.0;
  Double_t dWeightsSum   = 0.0;
  //vPtsRef.clear();
  vDigiIndRef.clear();
  //CbmTofCell *fTrafoCell=NULL;
  //Int_t iTrafoCell=-1;
  Int_t iNbChanInHit = 0;
  // Last Channel Temp variables
  Int_t iLastChan = -1;
  Double_t dLastPosX =
    0.0;  // -> Comment to remove warning because set but never used
  Double_t dLastPosY = 0.0;
  Double_t dLastTime = 0.0;
  // Channel Temp variables
  Double_t dPosX     = 0.0;
  Double_t dPosY     = 0.0;
  Double_t dPosZ     = 0.0;
  Double_t dTime     = 0.0;
  Double_t dTimeDif  = 0.0;
  Double_t dTotS     = 0.0;
  Int_t fiNbSameSide = 0;

  //  gGeoManager->SetTopVolume( gGeoManager->FindVolumeFast("tof_v14a") );
  gGeoManager->SetTopVolume(gGeoManager->FindVolumeFast("cave"));
  gGeoManager->CdTop();

  if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
    for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
      Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
      Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
      for (Int_t iSm = 0; iSm < iNbSm; iSm++)
        for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
          Int_t iNbCh   = fDigiBdfPar->GetNbChan(iSmType, iRpc);
          Int_t iChType = fDigiBdfPar->GetChanType(iSmType, iRpc);
          /*
	  LOG(debug)<<"RPC - Loop  "
		    << Form(" %3d %3d %3d %3d ",iSmType,iSm,iRpc,iChType);
	  */
          fviClusterMul[iSmType][iSm][iRpc] = 0;
          Int_t iChId                       = 0;
          Int_t iDetId =
            CbmTofAddress::GetUniqueAddress(iSm, iRpc, 0, 0, iSmType);
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
            //LOG(debug2)<<"ChanOrient "
            //	        << Form(" %3d %3d %3d %3d %3d ",iSmType,iSm,iRpc,fDigiBdfPar->GetChanOrient( iSmType, iRpc ),iNbCh);

            if (1 == fDigiBdfPar->GetChanOrient(iSmType, iRpc)) {
              // Horizontal strips => X comes from left right time difference
            }       // if( 1 == fDigiBdfPar->GetChanOrient( iSmType, iRpc ) )
            else {  // Vertical strips => Y comes from bottom top time difference
              for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
                //LOG(debug3)<<"VDigisize "
                //	    << Form(" T %3d Sm %3d R %3d Ch %3d Size %3lu ",
                //		    iSmType,iSm,iRpc,iCh,fStorStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size());
                if (0 == fStorDigi[iSmType][iSm * iNbRpc + iRpc].size())
                  continue;
                if (fvDeadStrips[iDetIndx] & (1 << iCh))
                  continue;  // skip over dead channels
                //if( 0 < fStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size() )
                //  fhNbDigiPerChan->Fill( fStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size() );

                while (1
                       < fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {

                  while (
                    (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0])->GetSide()
                    == (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])
                         ->GetSide()) {
                    // Not one Digi of each end!
                    fiNbSameSide++;
                    if (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()
                        > 2) {
                      LOG(debug)
                        << "SameSide Digis! on TSRC " << iSmType << iSm << iRpc
                        << iCh << ", Times: "
                        << Form(
                             "%f",
                             (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0])
                               ->GetTime())
                        << ", "
                        << Form(
                             "%f",
                             (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])
                               ->GetTime())
                        << ", DeltaT "
                        << (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])
                               ->GetTime()
                             - (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0])
                                 ->GetTime()
                        << ", array size: "
                        << fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size();
                      if (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][2]
                            ->GetSide()
                          == fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                               ->GetSide()) {
                        LOG(debug)
                          << "3 consecutive SameSide Digis! on TSRC " << iSmType
                          << iSm << iRpc << iCh << ", Times: "
                          << (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0])
                               ->GetTime()
                          << ", "
                          << (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])
                               ->GetTime()
                          << ", DeltaT "
                          << (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1])
                                 ->GetTime()
                               - (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh]
                                           [0])
                                   ->GetTime()
                          << ", array size: "
                          << fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh]
                               .size();
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                            .begin());
                      } else {
                        if (fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][2]
                                ->GetTime()
                              - fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                                  ->GetTime()
                            > fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][2]
                                  ->GetTime()
                                - fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh]
                                           [1]
                                             ->GetTime()) {
                          fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh]
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
                              fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0]
                                ->GetSide());
                          fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin()
                            + 1);
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                            fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                              .begin()
                            + 1);
                        }
                      }
                    } else {
                      LOG(debug)
                        << "SameSide Erase fStor entries(d) " << iSmType
                        << ", SR " << iSm * iNbRpc + iRpc << ", Ch" << iCh;
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                          .begin());
                    }
                    if (2 > fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size())
                      break;
                    continue;
                  }  // same condition side end
                  LOG(debug)
                    << "digis processing for "
                    << Form(
                         " SmT %3d Sm %3d Rpc %3d Ch %3d # %3lu ",
                         iSmType,
                         iSm,
                         iRpc,
                         iCh,
                         fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size());
                  if (2 > fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {
                    LOG(debug) << Form(
                      "Leaving digi processing for TSRC %d%d%d%d, size  %3lu",
                      iSmType,
                      iSm,
                      iRpc,
                      iCh,
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size());
                    break;
                  }
                  /* Int_t iLastChId = iChId; // Save Last hit channel*/

                  // 2 Digis = both sides present
                  CbmTofDetectorInfo xDetInfo(
                    ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
                  iChId = fTofId->SetDetectorInfo(xDetInfo);
                  Int_t iUCellId =
                    CbmTofAddress::GetUniqueAddress(iSm, iRpc, iCh, 0, iSmType);
                  LOG(debug)
                    << Form("TSRC %d%d%d%d size %3lu ",
                            iSmType,
                            iSm,
                            iRpc,
                            iCh,
                            fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size())
                    << Form(" ChId: 0x%08x 0x%08x ", iChId, iUCellId);
                  fChannelInfo = fDigiPar->GetCell(iChId);

                  if (NULL == fChannelInfo) {
                    LOG(error) << "BuildHits: no geometry info! "
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
                  //          fNode->Print();
                  if (
                    NULL
                    == fNode) {  // Transformation matrix not available !!!??? - Check
                    LOG(error) << Form("Node at (%6.1f,%6.1f,%6.1f) : %p",
                                       fChannelInfo->GetX(),
                                       fChannelInfo->GetY(),
                                       fChannelInfo->GetZ(),
                                       fNode);
                    //ChangeState(STOP);
                    ChangeState(fair::mq::Transition(STOP));
                  }

                  CbmTofDigi* xDigiA =
                    fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][0];
                  CbmTofDigi* xDigiB =
                    fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][1];

                  dTimeDif = (xDigiA->GetTime() - xDigiB->GetTime());
                  if (5 == iSmType && dTimeDif != 0.) {
                    // FIXME -> Overflow treatment in calib/tdc/TMbsCalibTdcTof.cxx
                    LOG(debug)
                      << "BuildHits: Diamond hit in " << iSm
                      << " with inconsistent digits " << xDigiA->GetTime()
                      << ", " << xDigiB->GetTime() << " -> " << dTimeDif;
                    /*
		    LOG(debug) << "    "<<xDigiA->ToString();
		    LOG(debug) << "    "<<xDigiB->ToString();
		    */
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
                      && fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()
                           > 2) {
                    LOG(debug)
                      << "Hit candidate outside correlation window, check for "
                         "better possible digis, "
                      << " mul "
                      << fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size();

                    CbmTofDigi* xDigiC =
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh][2];
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
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                            .begin());
                      } else {
                        xDigiB = xDigiC;
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(++(
                          fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin()
                          + 1));
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                          ++(fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh]
                               .begin()
                             + 1));
                      }
                    }
                  }
                  if (xDigiA->GetSide() == xDigiB->GetSide()) {
                    LOG(error)
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

                  /*
		  LOG(debug)
		    <<"NbChanInHit  "
		    << Form(" %3d %3d %3d %3d %3d 0x%p %1.0f Time %f PosX %f PosY %f Svel %f ",
			    iNbChanInHit,iSmType,iRpc,iCh,iLastChan,xDigiA,xDigiA->GetSide(),
			    dTime,dPosX,dPosY,fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc))
		    //  << Form( ", Offs %f, %f ",fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][0],
		    //                            fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][1])
		    ;
		  */
                  // Now check if a hit/cluster is already started
                  if (0 < iNbChanInHit) {
                    if (iLastChan == iCh - 1) {
                      /*
		       fhDigTimeDifClust->Fill( dTime - dLastTime );
		       fhDigSpacDifClust->Fill( dPosY - dLastPosY );
		       fhDigDistClust->Fill( dPosY - dLastPosY,
					     dTime - dLastTime );
		      */
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

                      LOG(debug)
                        << " Add Digi and erase fStor entries(a): NbChanInHit "
                        << iNbChanInHit << ", " << iSmType << ", SR "
                        << iSm * iNbRpc + iRpc << ", Ch" << iCh;

                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
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
                      //TGeoNode*         cNode   =
                      gGeoManager->GetCurrentNode();
                      /*TGeoHMatrix* cMatrix =*/gGeoManager->GetCurrentMatrix();
                      //cNode->Print();
                      //cMatrix->Print();
                      gGeoManager->LocalToMaster(hitpos_local, hitpos);
                      /*
		      LOG(debug)<<
			Form("LocalToMaster for node %p: (%6.1f,%6.1f,%6.1f) ->(%6.1f,%6.1f,%6.1f)", 
			     cNode, hitpos_local[0], hitpos_local[1], hitpos_local[2], 
			     hitpos[0], hitpos[1], hitpos[2]);
		      */
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
			
		      */
                      //fDigiBdfPar->GetFeeTimeRes() * fDigiBdfPar->GetSigVel(iSmType,iRpc), // Use the electronics resolution
                      //fDigiBdfPar->GetNbGaps( iSmType, iRpc)*
                      //fDigiBdfPar->GetGapSize( iSmType, iRpc)/ //10.0 / // Change gap size in cm
                      //TMath::Sqrt(12.0) ); // Use full RPC thickness as "Channel" Z size

                      // Int_t iDetId = vPtsRef[0]->GetDetectorID();// detID = pt->GetDetectorID() <= from TofPoint
                      // calc mean ch from dPosX=((Double_t)(-iNbCh/2 + iCh)+0.5)*fChannelInfo->GetSizex();

                      Int_t iChm =
                        floor(dWeightedPosX / fChannelInfo->GetSizex())
                        + iNbCh / 2;
                      if (iChm < 0) iChm = 0;
                      if (iChm > iNbCh - 1) iChm = iNbCh - 1;
                      iDetId = CbmTofAddress::GetUniqueAddress(
                        iSm, iRpc, iChm, 0, iSmType);
                      //Int_t iRefId = 0; // Index of the correspondng TofPoint
                      //		       if(NULL != fTofPointsColl) {
                      //iRefId = fTofPointsColl->IndexOf( vPtsRef[0] );
                      //}
                      /*
		      LOG(debug)<<"Save Hit  "
				<< Form(" %3d %3d 0x%08x %3d %3d %3d %f %f",
					fiNbHits,iNbChanInHit,iDetId,iChm,iLastChan,iRefId,
					dWeightedTime,dWeightedPosY)
				<<", DigiSize: "<<vDigiIndRef.size()
				<<", DigiInds: ";		      
		      for (UInt_t i=0; i<vDigiIndRef.size();i++){
			LOG(debug)<<" "<<vDigiIndRef.at(i)<<"(M"<<fviClusterMul[iSmType][iSm][iRpc]<<")";
		      }
		      */

                      fviClusterMul[iSmType][iSm][iRpc]++;
                      if (vDigiIndRef.size() < 2) {
                        LOG(warn) << "Digi refs for Hit " << fiNbHits << ": "
                                  << vDigiIndRef.size();
                      }
                      if (fiNbHits > 0) {
                        CbmTofHit* pHitL =
                          (CbmTofHit*) fTofHitsColl->At(fiNbHits - 1);
                        if (iDetId == pHitL->GetAddress()
                            && dWeightedTime == pHitL->GetTime()) {
                          LOG(debug) << "Store Hit twice? "
                                     << " fiNbHits " << fiNbHits << ", "
                                     << Form("0x%08x", iDetId);
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
                        //vPtsRef.size(), // flag  = number of TofPoints generating the cluster
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
                      for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
                        Double_t dTot = ((CbmTofDigi*) (&(fTofCalDigiVec->at(
                                           vDigiIndRef.at(i)))))
                                          ->GetTot();
                        digiMatch->AddLink(CbmLink(dTot,
                                                   vDigiIndRef.at(i),
                                                   fiOutputTreeEntry,
                                                   fiFileIndex));
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

                      vDigiIndRef.push_back((Int_t)(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]));
                      vDigiIndRef.push_back((Int_t)(
                        fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]));
                      //LOG(debug2)<<"Erase fStor entries(b) "<<iSmType<<", SR "<<iSm*iNbRpc+iRpc<<", Ch"<<iCh;

                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                        fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
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
                    // Save pointer on CbmTofPoint
                    //if(NULL != fTofPointsColl)
                    //                                    vPtsRef.push_back( (CbmTofPoint*)(xDigiA->GetLinks()) );
                    vDigiIndRef.push_back((Int_t)(
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][0]));
                    vDigiIndRef.push_back((Int_t)(
                      fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh][1]));

                    //LOG(debug)<<"Erase fStor entries(c) "<<iSmType<<", SR "<<iSm*iNbRpc+iRpc<<", Ch"<<iCh;

                    fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
                    fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                      fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin());
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
                }  // while( 1 < fStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size() )
                fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].clear();
                fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].clear();
              }  // for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
              //LOG(debug2)<<"finished V-RPC"
              //		  << Form(" %3d %3d %3d %d %f %fx",iSmType,iSm,iRpc,fTofHitsColl->GetEntries(),dLastPosX,dLastPosY);
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
            /*
	    LOG(debug)<<"Process cluster " 
		      <<iNbChanInHit;
	    */
            // Check orientation to properly assign errors
            if (1 == fDigiBdfPar->GetChanOrient(iSmType, iRpc)) {
              // LOG(debug1)<<"H-Hit ";
            }  // if( 1 == fDigiBdfPar->GetChanOrient( iSmType, iRpc ) )
            else {
              //LOG(debug2)<<"V-Hit ";
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
              //TGeoNode*        cNode=
              gGeoManager->GetCurrentNode();
              /*TGeoHMatrix* cMatrix =*/gGeoManager->GetCurrentMatrix();
              //cNode->Print();
              //cMatrix->Print();

              gGeoManager->LocalToMaster(hitpos_local, hitpos);
              /*
	      LOG(debug)<<
		Form(" LocalToMaster for V-node %p: (%6.1f,%6.1f,%6.1f) ->(%6.1f,%6.1f,%6.1f)", 
		     cNode, hitpos_local[0], hitpos_local[1], hitpos_local[2], 
		     hitpos[0], hitpos[1], hitpos[2])
		;
	      */
              TVector3 hitPos(hitpos[0], hitpos[1], hitpos[2]);
              // Event errors, not properly done at all for now
              // Right way of doing it should take into account the weight distribution
              // and real system time resolution
              TVector3 hitPosErr(
                0.5, 0.5, 0.5);  // including positioning uncertainty
              /*
		TVector3 hitPosErr( fChannelInfo->GetSizex()/TMath::Sqrt(12.0),   // Single strips approximation
		0.5, // Use generic value 
		1.);
	      */
              Int_t iChm =
                floor(dWeightedPosX / fChannelInfo->GetSizex()) + iNbCh / 2;
              if (iChm < 0) iChm = 0;
              if (iChm > iNbCh - 1) iChm = iNbCh - 1;
              iDetId =
                CbmTofAddress::GetUniqueAddress(iSm, iRpc, iChm, 0, iSmType);
              //Int_t iRefId = 0; // Index of the correspondng TofPoint
              //if(NULL != fTofPointsColl) iRefId = fTofPointsColl->IndexOf( vPtsRef[0] );
              /*
	      LOG(debug)<<"Save V-Hit  "
			<< Form(" %3d %3d 0x%08x %3d 0x%08x", // %3d %3d 
				fiNbHits,iNbChanInHit,iDetId,iLastChan,iRefId) //vPtsRef.size(),vPtsRef[0])
		//   dWeightedTime,dWeightedPosY)
			<<", DigiSize: "<<vDigiIndRef.size();	      
	      for (UInt_t i=0; i<vDigiIndRef.size();i++){
		LOG(debug)<<"DigiIndRef "<<i<<" "<<vDigiIndRef.at(i)<<"(M"<<fviClusterMul[iSmType][iSm][iRpc]<<")";
	      }
	      */
              fviClusterMul[iSmType][iSm][iRpc]++;
              if (vDigiIndRef.size() < 2) {
                LOG(warn) << "Digi refs for Hit " << fiNbHits << ":  "
                          << vDigiIndRef.size();
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
                //vPtsRef.size(), // flag  = number of TofPoints generating the cluster
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
              for (UInt_t i = 0; i < vDigiIndRef.size(); i++) {
                Double_t dTot =
                  ((CbmTofDigi*) (&(fTofCalDigiVec->at(vDigiIndRef.at(i)))))
                    ->GetTot();
                digiMatch->AddLink(CbmLink(
                  dTot, vDigiIndRef.at(i), fiOutputTreeEntry, fiFileIndex));
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
        }      // for each sm/rpc pair
    }          // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
  }

  return kTRUE;
}

/************************************************************************************/
Bool_t CbmDeviceHitBuilderTof::MergeClusters() { return kTRUE; }

void CbmDeviceHitBuilderTof::LH_store(Int_t iSmType,
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

void CbmDeviceHitBuilderTof::CheckLHMemory() {
  if ((Int_t) fvLastHits.size() != fDigiBdfPar->GetNbSmTypes())
    LOG(error) << Form("Inconsistent LH Smtype size %lu, %d ",
                       fvLastHits.size(),
                       fDigiBdfPar->GetNbSmTypes());

  for (Int_t iSmType = 0; iSmType < fDigiBdfPar->GetNbSmTypes(); iSmType++) {
    if ((Int_t) fvLastHits[iSmType].size() != fDigiBdfPar->GetNbSm(iSmType))
      LOG(error) << Form("Inconsistent LH Sm size %lu, %d T %d",
                         fvLastHits[iSmType].size(),
                         fDigiBdfPar->GetNbSm(iSmType),
                         iSmType);
    for (Int_t iSm = 0; iSm < fDigiBdfPar->GetNbSm(iSmType); iSm++) {
      if ((Int_t) fvLastHits[iSmType][iSm].size()
          != fDigiBdfPar->GetNbRpc(iSmType))
        LOG(error) << Form("Inconsistent LH Rpc size %lu, %d TS %d%d ",
                           fvLastHits[iSmType][iSm].size(),
                           fDigiBdfPar->GetNbRpc(iSmType),
                           iSmType,
                           iSm);
      for (Int_t iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
        if ((Int_t) fvLastHits[iSmType][iSm][iRpc].size()
            != fDigiBdfPar->GetNbChan(iSmType, iRpc))
          LOG(error) << Form(
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
              LOG(error) << Form(
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

void CbmDeviceHitBuilderTof::CleanLHMemory() {
  if ((Int_t) fvLastHits.size() != fDigiBdfPar->GetNbSmTypes())
    LOG(error) << Form("Inconsistent LH Smtype size %lu, %d ",
                       fvLastHits.size(),
                       fDigiBdfPar->GetNbSmTypes());
  for (Int_t iSmType = 0; iSmType < fDigiBdfPar->GetNbSmTypes(); iSmType++) {
    if ((Int_t) fvLastHits[iSmType].size() != fDigiBdfPar->GetNbSm(iSmType))
      LOG(error) << Form("Inconsistent LH Sm size %lu, %d T %d",
                         fvLastHits[iSmType].size(),
                         fDigiBdfPar->GetNbSm(iSmType),
                         iSmType);
    for (Int_t iSm = 0; iSm < fDigiBdfPar->GetNbSm(iSmType); iSm++) {
      if ((Int_t) fvLastHits[iSmType][iSm].size()
          != fDigiBdfPar->GetNbRpc(iSmType))
        LOG(error) << Form("Inconsistent LH Rpc size %lu, %d TS %d%d ",
                           fvLastHits[iSmType][iSm].size(),
                           fDigiBdfPar->GetNbRpc(iSmType),
                           iSmType,
                           iSm);
      for (Int_t iRpc = 0; iRpc < fDigiBdfPar->GetNbRpc(iSmType); iRpc++) {
        if ((Int_t) fvLastHits[iSmType][iSm][iRpc].size()
            != fDigiBdfPar->GetNbChan(iSmType, iRpc))
          LOG(error) << Form(
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
              LOG(error) << Form(
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

Bool_t CbmDeviceHitBuilderTof::AddNextChan(Int_t iSmType,
                                           Int_t iSm,
                                           Int_t iRpc,
                                           Int_t iLastChan,
                                           Double_t dLastPosX,
                                           Double_t dLastPosY,
                                           Double_t dLastTime,
                                           Double_t dLastTotS) {
  //Int_t iNbSm  = fDigiBdfPar->GetNbSm(  iSmType);
  Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
  Int_t iNbCh  = fDigiBdfPar->GetNbChan(iSmType, iRpc);
  //  Int_t iChType = fDigiBdfPar->GetChanType( iSmType, iRpc );

  Int_t iCh = iLastChan + 1;
  LOG(debug) << Form("Inspect channel TSRC %d%d%d%d at time %f, pos %f, size ",
                     iSmType,
                     iSm,
                     iRpc,
                     iCh,
                     dLastTime,
                     dLastPosY)
             << fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size();
  if (iCh == iNbCh) return kFALSE;
  if (0 == fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) return kFALSE;
  //if( 0 < fStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size() )
  //   fhNbDigiPerChan->Fill( fStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size() );
  if (1 < fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {
    Bool_t AddedHit = kFALSE;
    for (Int_t i1 = 0;
         i1 < (Int_t) fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size() - 1;
         i1++) {
      if (AddedHit) break;
      Int_t i2 = i1 + 1;
      while (
        !AddedHit
        && i2 < (Int_t) fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].size()) {
        // LOG(debug)<<"check digi pair "<<i1<<","<<i2<<" with size "<<fStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size();

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
            fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
              fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + i1);
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
              fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                fStorDigi[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + ipos);
              fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].erase(
                fStorDigiInd[iSmType][iSm * iNbRpc + iRpc][iCh].begin() + ipos);
            } else {
              LOG(error) << " Did not find  i2 " << i2 << " with Ind2 " << Ind2;
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
      }  //  while(i2 < fStorDigi[iSmType][iSm*iNbRpc+iRpc][iCh].size()-1 )
    }    // end for i1
  }      // end if size
  Double_t hitpos_local[3] = {3 * 0.};
  hitpos_local[0]          = dLastPosX;
  hitpos_local[1]          = dLastPosY;
  hitpos_local[2]          = 0.;
  Double_t hitpos[3];

  TGeoNode* cNode = gGeoManager->GetCurrentNode();
  if (NULL == cNode) {  // Transformation matrix not available !!!??? - Check
    LOG(error) << Form("Node at (%6.1f,%6.1f,%6.1f) : %p",
                       fChannelInfo->GetX(),
                       fChannelInfo->GetY(),
                       fChannelInfo->GetZ(),
                       cNode);
    //ChangeState(STOP);
    ChangeState(fair::mq::Transition(STOP));
  }

  /*TGeoHMatrix* cMatrix = */ gGeoManager->GetCurrentMatrix();
  gGeoManager->LocalToMaster(hitpos_local, hitpos);
  TVector3 hitPos(hitpos[0], hitpos[1], hitpos[2]);
  TVector3 hitPosErr(0.5, 0.5, 0.5);  // FIXME including positioning uncertainty
  Int_t iChm = floor(dLastPosX / fChannelInfo->GetSizex()) + iNbCh / 2;
  if (iChm < 0) iChm = 0;
  if (iChm > iNbCh - 1) iChm = iNbCh - 1;
  Int_t iDetId = CbmTofAddress::GetUniqueAddress(iSm, iRpc, iChm, 0, iSmType);

  //  Int_t iNbChanInHit=vDigiIndRef.size()/2;
  fviClusterMul[iSmType][iSm][iRpc]++;
  /*
  LOG(debug)<<"Save A-Hit "
            << Form("%2d %2d 0x%08x %3d t %f, y %f ", 
		    fiNbHits,iNbChanInHit,iDetId,iLastChan,dLastTime,dLastPosY)
            <<", DigiSize: "<<vDigiIndRef.size();
  for (UInt_t i=0; i<vDigiIndRef.size();i++){
    LOG(debug)<<"DigiIndRef "<<i<<" "<<vDigiIndRef.at(i)<<"(M"<<fviClusterMul[iSmType][iSm][iRpc]<<")";
  }
  */
  CbmTofHit* pHit = new CbmTofHit(
    iDetId,
    hitPos,
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
  } else {
    pHit->Delete();
  }
  CbmMatch* digiMatch = new ((*fTofDigiMatchColl)[fiNbHits]) CbmMatch();
  for (Int_t i = 0; i < (Int_t) vDigiIndRef.size(); i++) {
    Double_t dTot =
      ((CbmTofDigi*) (&(fTofCalDigiVec->at(vDigiIndRef.at(i)))))->GetTot();
    digiMatch->AddLink(
      CbmLink(dTot, vDigiIndRef.at(i), fiOutputTreeEntry, fiFileIndex));
  }
  fiNbHits++;
  vDigiIndRef.clear();

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

void CbmDeviceHitBuilderTof::fit_ybox(const char* hname) {
  TH1* h1;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (NULL != h1) { fit_ybox(h1, 0.); }
}

void CbmDeviceHitBuilderTof::fit_ybox(TH1* h1, Double_t ysize) {
  Double_t* fpar = NULL;
  fit_ybox(h1, ysize, fpar);
}

void CbmDeviceHitBuilderTof::fit_ybox(TH1* h1,
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

Bool_t CbmDeviceHitBuilderTof::LoadGeometry() {
  LOG(info) << "LoadGeometry starting for  " << fDigiBdfPar->GetNbDet()
            << " described detectors, " << fDigiPar->GetNrOfModules()
            << " geometrically known modules ";

  //gGeoManager->Export("HitBuilder.loadgeo.root");  // write current status to file

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

    if (NULL == fNode) {  // Transformation matrix not available !!!??? - Check
      LOG(error) << Form("Node at (%6.1f,%6.1f,%6.1f) : %p",
                         fChannelInfo->GetX(),
                         fChannelInfo->GetY(),
                         fChannelInfo->GetZ(),
                         fNode);
      //ChangeState(STOP);
      ChangeState(fair::mq::Transition(STOP));
    }
    if (icell == 0) {
      TGeoHMatrix* cMatrix = gGeoManager->GetCurrentMatrix();
      //cNode->Print();
      cMatrix->Print();
    }
  }

  Int_t iNbDet = fDigiBdfPar->GetNbDet();
  fvDeadStrips.resize(iNbDet);
  fvPulserOffset.resize(iNbDet);
  fvPulserTimes.resize(iNbDet);

  for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
    Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);
    Int_t iSmType   = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSmId     = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpcId    = CbmTofAddress::GetRpcId(iUniqueId);
    LOG(info) << " DetIndx " << iDetIndx << "(" << iNbDet << "), SmType "
              << iSmType << ", SmId " << iSmId << ", RpcId " << iRpcId
              << " => UniqueId " << Form("0x%08x ", iUniqueId)
              << Form(" Svel %6.6f, DeadStrips 0x%08x ",
                      fDigiBdfPar->GetSigVel(iSmType, iSmId, iRpcId),
                      fvDeadStrips[iDetIndx]);

    Int_t iCell = -1;
    while (kTRUE) {
      Int_t iUCellId =
        CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, ++iCell, 0, iSmType);
      fChannelInfo = fDigiPar->GetCell(iUCellId);
      if (NULL == fChannelInfo) break;
    }

    fvPulserOffset[iDetIndx].resize(2);  // provide vector for both sides
    for (Int_t i = 0; i < 2; i++)
      fvPulserOffset[iDetIndx][i] = 0.;  // initialize
    fvPulserTimes[iDetIndx].resize(2);   // provide vector for both sides
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
            LOG(warn) << "LoadGeometry: StoreDigi without channels "
                      << Form("SmTy %3d, Sm %3d, NbRpc %3d, Rpc, %3d ",
                              iSmType,
                              iSm,
                              iNbRpc,
                              iRpc);
          }
          fStorDigi[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
          fvLastHits[iSmType][iSm][iRpc].resize(iNbChan);
        }  // for( Int_t iRpc = 0; iRpc < iNbRpc; iRpc++ )
      }    // for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
    }      // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
  }        // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
  else {
    fStorDigi.resize(iNbSmTypes);
    fStorDigiInd.resize(iNbSmTypes);
    fviClusterSize.resize(iNbSmTypes);
    fviTrkMul.resize(iNbSmTypes);
    fvdX.resize(iNbSmTypes);
    fvdY.resize(iNbSmTypes);
    fvdDifX.resize(iNbSmTypes);
    fvdDifY.resize(iNbSmTypes);
    fvdDifCh.resize(iNbSmTypes);
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
        for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
          Int_t iNbChan = fDigiBdfPar->GetNbChan(iSmType, iRpc);
          fStorDigi[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
          fStorDigiInd[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        }  // for( Int_t iRpc = 0; iRpc < iNbRpc; iRpc++ )
      }    // for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
    }      // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
  }        // else of if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
  return kTRUE;
}

Bool_t CbmDeviceHitBuilderTof::FillDigiStor() {

  // Loop over the digis array and store the Digis in separate vectors for
  // each RPC modules

  CbmTofDigi* pDigi;

  Int_t iNbTofDigi = (Int_t) fTofCalDigiVec->size();
  //fTofCalDigisColl->GetEntriesFast();
  for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; iDigInd++) {
    //pDigi = (CbmTofDigi*) fTofCalDigisColl->At(iDigInd);
    pDigi = &(fTofCalDigiVec->at(iDigInd));

    /*
    LOG(info)<<"AC " // After Calibration
	      <<Form("0x%08x",pDigi->GetAddress())<<" TSRC "
	      <<pDigi->GetType()
	      <<pDigi->GetSm()
	      <<pDigi->GetRpc()
	      <<Form("%2d",(Int_t)pDigi->GetChannel())<<" "
	      <<pDigi->GetSide()<<" "
	      <<Form("%f",pDigi->GetTime())<<" "
	      <<pDigi->GetTot();
    */
    if (fDigiBdfPar->GetNbSmTypes()
          > pDigi->GetType()  // prevent crash due to misconfiguration
        && fDigiBdfPar->GetNbSm(pDigi->GetType()) > pDigi->GetSm()
        && fDigiBdfPar->GetNbRpc(pDigi->GetType()) > pDigi->GetRpc()
        && fDigiBdfPar->GetNbChan(pDigi->GetType(), pDigi->GetRpc())
             > pDigi->GetChannel()) {
      fStorDigi[pDigi->GetType()]
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
  return kTRUE;
}

Bool_t CbmDeviceHitBuilderTof::SendHits() {
  //Output Log
  for (Int_t iHit = 0; iHit < fiNbHits; iHit++) {
    CbmTofHit* pHit = (CbmTofHit*) fTofHitsColl->At(iHit);
    LOG(debug) << Form(
      "Found Hit %d, addr 0x%08x, X %6.2f, Y %6.2f Z %6.2f T %6.2f CLS %d",
      iHit,
      pHit->GetAddress(),
      pHit->GetX(),
      pHit->GetY(),
      pHit->GetZ(),
      pHit->GetTime(),
      pHit->GetFlag());
  }
  // prepare output hit vector
  std::vector<CbmTofHit*> vhit;
  vhit.resize(fiNbHits);
  for (Int_t iHit = 0; iHit < fiNbHits; iHit++) {
    CbmTofHit* pHit = (CbmTofHit*) fTofHitsColl->At(iHit);
    vhit[iHit]      = pHit;
  }

  // prepare output string streams
  std::stringstream ossE;
  boost::archive::binary_oarchive oaE(ossE);
  oaE << fEventHeader;
  std::string* strMsgE = new std::string(ossE.str());

  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << vhit;
  std::string* strMsg = new std::string(oss.str());

  FairMQParts parts;
  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgE->c_str()),  // data
    strMsgE->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgE));  // object that manages the data

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsg->c_str()),  // data
    strMsg->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsg));  // object that manages the data

  if (Send(parts, "tofhits")) {
    LOG(error) << "Problem sending data ";
    return false;
  }

  return kTRUE;
}

Bool_t CbmDeviceHitBuilderTof::SendAll() { return kTRUE; }

Bool_t CbmDeviceHitBuilderTof::FillHistos() {
  Int_t iNbTofHits = fTofHitsColl->GetEntries();
  CbmTofHit* pHit;
  //gGeoManager->SetTopVolume( gGeoManager->FindVolumeFast("tof_v14a") );
  gGeoManager->CdTop();

  if (0 < iNbTofHits) {
    Bool_t BSel[iNSel];
    Double_t dTTrig[iNSel];
    CbmTofHit* pTrig[iNSel];
    Double_t ddXdZ[iNSel];
    Double_t ddYdZ[iNSel];
    Double_t dSel2dXdYMin[iNSel];

    Int_t iBeamRefMul    = 0;
    Int_t iBeamAddRefMul = 0;

    if (0 < iNSel) {  // check software triggers

      LOG(debug) << "FillHistos() for " << iNSel << " triggers"
                 << ", Dut " << fDutId << fDutSm << fDutRpc
                 << Form(", 0x%08x", fDutAddr) << ", Sel " << fSelId << fSelSm
                 << fSelRpc << Form(", 0x%08x", fSelAddr) << ", Sel2 "
                 << fSel2Id << fSel2Sm << fSel2Rpc
                 << Form(", 0x%08x", fSel2Addr);
      /*
      LOG(debug) <<"FillHistos: Muls: "
		 <<fviClusterMul[fDutId][fDutSm][fDutRpc]
		 <<", "<<fviClusterMul[fSelId][fSelSm][fSelRpc]
	;
      */
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

      // do input distributions first
      for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
        pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
        if (NULL == pHit) continue;
        if (StartAnalysisTime == 0.) {
          StartAnalysisTime = pHit->GetTime();
          LOG(info) << "StartAnalysisTime set to " << StartAnalysisTime
                    << " ns. ";
        }
        Int_t iDetId                          = (pHit->GetAddress() & DetMask);
        std::map<UInt_t, UInt_t>::iterator it = fDetIdIndexMap.find(iDetId);
        if (it == fDetIdIndexMap.end())
          continue;                   // continue for invalid detector index
        Int_t iDetIndx = it->second;  //fDetIdIndexMap[iDetId];

        Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
        Int_t iSm     = CbmTofAddress::GetSmId(iDetId);
        Int_t iRpc    = CbmTofAddress::GetRpcId(iDetId);
        Int_t iCh     = CbmTofAddress::GetChannelId(pHit->GetAddress());

        Double_t dTimeAna = (pHit->GetTime() - StartAnalysisTime) / 1.E9;
        //LOG(debug)<<"TimeAna "<<StartAnalysisTime<<", "<< pHit->GetTime()<<", "<<dTimeAna;
        fhRpcCluRate[iDetIndx]->Fill(dTimeAna);

        if (fdMemoryTime > 0.
            && fvLastHits[iSmType][iSm][iRpc][iCh].size() == 0)
          LOG(error) << Form(" <E> hit not stored in memory for TSRC %d%d%d%d",
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
            LOG(warn) << Form("Invalid time ordering in ev %8.0f in list of "
                              "size %lu for TSRC %d%d%d%d: Delta t %f  ",
                              fdEvent,
                              fvLastHits[iSmType][iSm][iRpc][iCh].size(),
                              iSmType,
                              iSm,
                              iRpc,
                              iCh,
                              pHL->GetTime() - pH0->GetTime());
          //while( (*((std::list<CbmTofHit *>::iterator) fvLastHits[iSmType][iSm][iRpc][iCh].begin()))->GetTime()+fdMemoryTime < pHit->GetTime()
          while (fvLastHits[iSmType][iSm][iRpc][iCh].size() > 2.
                 || fvLastHits[iSmType][iSm][iRpc][iCh].front()->GetTime()
                        + fdMemoryTime
                      < pHit->GetTime()) {
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
              LOG(error) << Form(
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
            CbmTofDigi* pDig0 = (CbmTofDigi*) (&(fTofCalDigiVec->at(iDigInd0)));
            CbmTofDigi* pDig1 = (CbmTofDigi*) (&(fTofCalDigiVec->at(iDigInd1)));
            dTotSum += pDig0->GetTot() + pDig1->GetTot();
          }

          std::list<CbmTofHit*>::iterator itL =
            fvLastHits[iSmType][iSm][iRpc][iCh].end();
          itL--;
          for (UInt_t iH = 0;
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
      for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
        pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
        if (NULL == pHit) continue;
        Int_t iDetId = (pHit->GetAddress() & DetMask);

        if (fiBeamRefAddr == iDetId) {
          if (fviClusterMul[fiBeamRefType][fiBeamRefSm][fiBeamRefDet]
              > fiBeamRefMulMax)
            break;
          // Check Tot
          CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
          Double_t TotSum     = 0.;
          for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks();
               iLink += 2) {                             // loop over digis
            CbmLink L0     = digiMatch->GetLink(iLink);  //vDigish.at(ivDigInd);
            Int_t iDigInd0 = L0.GetIndex();
            if (iDigInd0 < (Int_t) fTofCalDigiVec->size()) {
              CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
              TotSum += pDig0->GetTot();
            }
          }
          TotSum /= (0.5 * digiMatch->GetNofLinks());
          if (TotSum > fhRpcCluTot[iIndexDut]->GetYaxis()->GetXmax())
            continue;  // ignore too large clusters

          fTRefHits = 1;
          if (pHit->GetTime() < dTRef) { dTRef = pHit->GetTime(); }
          iBeamRefMul++;
        } else {  //additional reference type multiplicity
          if (fiBeamRefType == CbmTofAddress::GetSmType(iDetId))
            iBeamAddRefMul++;
        }
      }
      LOG(debug) << "FillHistos: BRefMul: " << iBeamRefMul << ", "
                 << iBeamAddRefMul;
      if (iBeamRefMul == 0)
        return kFALSE;  // don't fill histos without reference time
      if (iBeamAddRefMul < fiBeamAddRefMul)
        return kFALSE;  // ask for confirmation by other beam counters

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
          case 0:  //  Detector under Test (Dut) && Diamonds,BeamRef
            iRl = fviClusterMul[fDutId][fDutSm].size();
            if (fDutRpc > -1) {
              iR0 = fDutRpc;
              iRl = fDutRpc + 1;
            }
            for (Int_t iRpc = iR0; iRpc < iRl; iRpc++)
              iDutMul += fviClusterMul[fDutId][fDutSm][iRpc];
            LOG(debug) << "Selector 0: DutMul "
                       << fviClusterMul[fDutId][fDutSm][fDutRpc] << ", "
                       << iDutMul << ", BRefMul " << iBeamRefMul
                       << " TRef: " << dTRef << ", BeamAddRefMul "
                       << iBeamAddRefMul << ", " << fiBeamAddRefMul;
            if (iDutMul > 0 && iDutMul < fiCluMulMax) {
              dTTrig[iSel] = dDoubleMax;
              // LOG(debug1)<<"Found selector 0, NbHits  "<<iNbTofHits;
              for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
                pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
                if (NULL == pHit) continue;
                Int_t iDetId = (pHit->GetAddress() & DetMask);
                // LOG(debug1)<<Form(" Det 0x%08x, Dut 0x%08x, T %f, TTrig %f",
                // 		iDetId, fDutAddr, pHit->GetTime(),  dTTrig[iSel])
                // 		 ;
                //if( fDutId == CbmTofAddress::GetSmType( iDetId ))
                if (fDutAddr == iDetId) {
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

          case 1:  // MRef & BRef
            iRl = fviClusterMul[fSelId][fSelSm].size();
            if (fSelRpc > -1) {
              iR0 = fSelRpc;
              iRl = fSelRpc + 1;
            }
            for (Int_t iRpc = iR0; iRpc < iRl; iRpc++)
              iRefMul += fviClusterMul[fSelId][fSelSm][iRpc];
            LOG(debug) << "FillHistos(): selector 1: RefMul "
                       << fviClusterMul[fSelId][fSelSm][fSelRpc] << ", "
                       << iRefMul << ", BRefMul " << iBeamRefMul;
            if (iRefMul > 0 && iRefMul < fiCluMulMax) {
              // LOG(debug1)<<"FillHistos(): Found selector 1";
              dTTrig[iSel] = dDoubleMax;
              for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
                pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
                if (NULL == pHit) continue;

                Int_t iDetId = (pHit->GetAddress() & DetMask);
                if (fSelAddr == iDetId) {
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
            LOG(info) << "FillHistos: selection not implemented " << iSel;
            ;
        }  // switch end
        if (fTRefMode > 10) { dTTrig[iSel] = dTRef; }
      }  // iSel - loop end

      if (fSel2Id > 0) {  // confirm selector by independent match
        for (Int_t iSel = 0; iSel < iNSel; iSel++) {
          if (BSel[iSel]) {
            BSel[iSel] = kFALSE;
            if (fviClusterMul[fSel2Id][fSel2Sm][fSel2Rpc] > 0
                && fviClusterMul[fSel2Id][fSel2Sm][fSel2Rpc] < fiCluMulMax)
              for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
                pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
                if (NULL == pHit) continue;
                Int_t iDetId = (pHit->GetAddress() & DetMask);
                if (fSel2Addr == iDetId) {
                  Double_t dzscal = 1.;
                  if (fEnableMatchPosScaling)
                    dzscal = pHit->GetZ() / pTrig[iSel]->GetZ();
                  Double_t dSEl2dXdz = (pHit->GetX() - pTrig[iSel]->GetX())
                                       / (pHit->GetZ() - pTrig[iSel]->GetZ());
                  Double_t dSEl2dYdz = (pHit->GetY() - pTrig[iSel]->GetY())
                                       / (pHit->GetZ() - pTrig[iSel]->GetZ());

                  if (TMath::Sqrt(
                        TMath::Power(
                          pHit->GetX() - dzscal * pTrig[iSel]->GetX(), 2.)
                        + TMath::Power(
                          pHit->GetY() - dzscal * pTrig[iSel]->GetY(), 2.))
                      < fdCaldXdYMax) {
                    BSel[iSel]     = kTRUE;
                    Double_t dX2Y2 = TMath::Sqrt(dSEl2dXdz * dSEl2dXdz
                                                 + dSEl2dYdz * dSEl2dYdz);
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
      UInt_t uTriggerPattern = 1;

      for (Int_t iSel = 0; iSel < iNSel; iSel++)
        if (BSel[iSel]) {
          uTriggerPattern |= (0x1 << (iSel * 3
                                      + CbmTofAddress::GetRpcId(
                                        pTrig[iSel]->GetAddress() & DetMask)));
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
    }  // 0<iNSel software triffer check end

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

      if (fviClusterMul[iSmType][iSm][iRpc] > fiCluMulMax)
        continue;  // skip this event
      if (iBeamRefMul == 0) break;

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
      /*
	LOG(debug1)<<"Hit info: "
	<<Form(" 0x%08x %d %f %f %f %f %f %d",iChId,iCh,
	pHit->GetX(),pHit->GetY(),pHit->GetTime(),fChannelInfo->GetX(),fChannelInfo->GetY(), iHitInd )
	;
      */
      Double_t hitpos[3];
      hitpos[0] = pHit->GetX();
      hitpos[1] = pHit->GetY();
      hitpos[2] = pHit->GetZ();
      Double_t hitpos_local[3];
      //TGeoNode* cNode=
      gGeoManager->GetCurrentNode();
      gGeoManager->MasterToLocal(hitpos, hitpos_local);
      /*
	LOG(debug1)<< Form(" MasterToLocal for %d, %d%d%d, node %p: (%6.1f,%6.1f,%6.1f) ->(%6.1f,%6.1f,%6.1f)", 
		      iDetIndx, iSmType, iSm, iRpc,
                 cNode, hitpos[0], hitpos[1], hitpos[2], 
                 hitpos_local[0], hitpos_local[1], hitpos_local[2])
              ;
      */
      fhRpcCluPosition[iDetIndx]->Fill(
        (Double_t) iCh, hitpos_local[1]);  //pHit->GetY()-fChannelInfo->GetY());
      fhSmCluPosition[iSmType]->Fill((Double_t)(iSm * iNbRpc + iRpc),
                                     hitpos_local[1]);

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
      /*
	LOG(debug1)<<" TofDigiMatchColl entries:"
                <<fTofDigiMatchColl->GetEntries()
                ;
      */
      if (iHitInd > fTofDigiMatchColl->GetEntries()) {
        LOG(error) << " Inconsistent DigiMatches for Hitind " << iHitInd
                   << ", TClonesArraySize: " << fTofDigiMatchColl->GetEntries();
      }
      CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
      /*
	LOG(debug1)<<" got "
                <<digiMatch->GetNofLinks()<< " matches for iCh "<<iCh<<" at iHitInd "<<iHitInd
                ;
      */
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
            for (UInt_t iH = 0;
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
        if (iDigInd0 < (Int_t) fTofCalDigiVec->size()) {
          CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
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

      for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks();
           iLink += 2) {                             // loop over digis
        CbmLink L0     = digiMatch->GetLink(iLink);  //vDigish.at(ivDigInd);
        Int_t iDigInd0 = L0.GetIndex();
        Int_t iDigInd1 =
          (digiMatch->GetLink(iLink + 1)).GetIndex();  //vDigish.at(ivDigInd+1);
        //LOG(debug1)<<" " << iDigInd0<<", "<<iDigInd1;

        if (iDigInd0 < (Int_t) fTofCalDigiVec->size()
            && iDigInd1 < (Int_t) fTofCalDigiVec->size()) {
          CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
          CbmTofDigi* pDig1 = &(fTofCalDigiVec->at(iDigInd1));
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
          /*
	    LOG(debug1)<<" fhRpcCluTot:  Digi 0 "<<iDigInd0<<": Ch "<<pDig0->GetChannel()<<", Side "<<pDig0->GetSide()
	               <<", StripSide "<<(Double_t)iCh*2.+pDig0->GetSide() 
                       <<" Digi 1 "<<iDigInd1<<": Ch "<<pDig1->GetChannel()<<", Side "<<pDig1->GetSide()
		       <<" , StripSide "<<(Double_t)iCh*2.+pDig1->GetSide() 
		       <<", Tot0 " << pDig0->GetTot() <<", Tot1 "<<pDig1->GetTot();
	  */
          fhRpcCluTot[iDetIndx]->Fill(
            pDig0->GetChannel() * 2. + pDig0->GetSide(), pDig0->GetTot());
          fhRpcCluTot[iDetIndx]->Fill(
            pDig1->GetChannel() * 2. + pDig1->GetSide(), pDig1->GetTot());

          Int_t iCh0 = pDig0->GetChannel();
          Int_t iCh1 = pDig1->GetChannel();
          Int_t iS0  = pDig0->GetSide();
          Int_t iS1  = pDig1->GetSide();
          if (iCh0 != iCh1 || iS0 == iS1) {
            LOG(error) << Form(
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
            fhRpcCluAvLnWalk[iDetIndx]->Fill(
              TMath::Log10(0.5 * (pDig0->GetTot() + pDig1->GetTot())),
              0.5 * (pDig0->GetTime() + pDig1->GetTime()) - pHit->GetTime());

            Double_t dTotWeigth = (pDig0->GetTot() + pDig1->GetTot()) / TotSum;
            Double_t dCorWeigth = 1. - dTotWeigth;

            fhRpcCluDelTOff[iDetIndx]->Fill(
              pDig0->GetChannel(),
              dCorWeigth
                * (0.5 * (pDig0->GetTime() + pDig1->GetTime())
                   - pHit->GetTime()));

            Double_t dDelPos = 0.5 * (pDig0->GetTime() - pDig1->GetTime())
                               * fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);
            if (0 == pDig0->GetSide()) dDelPos *= -1.;
            fhRpcCluDelPos[iDetIndx]->Fill(
              pDig0->GetChannel(), dCorWeigth * (dDelPos - hitpos_local[1]));

            fhRpcCluWalk[iDetIndx][iCh0][iS0]->Fill(
              pDig0->GetTot(),
              pDig0->GetTime()
                - (pHit->GetTime()
                   - (1. - 2. * pDig0->GetSide()) * hitpos_local[1]
                       / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)));
            fhRpcCluWalk[iDetIndx][iCh1][iS1]->Fill(
              pDig1->GetTot(),
              pDig1->GetTime()
                - (pHit->GetTime()
                   - (1. - 2. * pDig1->GetSide()) * hitpos_local[1]
                       / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)));

            fhRpcCluAvWalk[iDetIndx]->Fill(
              pDig0->GetTot(),
              pDig0->GetTime()
                - (pHit->GetTime()
                   - (1. - 2. * pDig0->GetSide()) * hitpos_local[1]
                       / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)));
            fhRpcCluAvWalk[iDetIndx]->Fill(
              pDig1->GetTot(),
              pDig1->GetTime()
                - (pHit->GetTime()
                   - (1. - 2. * pDig1->GetSide()) * hitpos_local[1]
                       / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)));
          }  // end of Clustersize > 1 condition
          /*
	    LOG(debug1)<<" fhTRpcCluTot: Digi 0 "<<iDigInd0<<": Ch "<<pDig0->GetChannel()<<", Side "<<pDig0->GetSide()
		    <<", StripSide "<<(Double_t)iCh*2.+pDig0->GetSide() 
                    <<" Digi 1 "<<iDigInd1<<": Ch "<<pDig1->GetChannel()<<", Side "<<pDig1->GetSide()
		    <<", StripSide "<<(Double_t)iCh*2.+pDig1->GetSide() 
		    ;
	  */
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
                for (UInt_t iH = 0;
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
                dZsign[iSel] = 1.;
                if (pHit->GetZ() < pTrig[iSel]->GetZ()) dZsign[iSel] = -1.;
              }
              // look for geometrical match  with selector hit
              //if(  iSmType==fiBeamRefType      // to get entries in diamond/BeamRef histos
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
                    continue;  // refine position selection cut in cosmic measurement
                dTcor[iSel] = 0.;  // precaution
                if (
                  dTRef != 0.
                  && TMath::Abs(dTRef - dTTrig[iSel])
                       < fdDelTofMax) {  // correct times for DelTof - velocity spread
                  if (
                    iLink
                    == 0) {  // do calculations only once (at 1. digi entry) // interpolate!
                    // calculate spatial distance to trigger hit
                    /*
		 dDist=TMath::Sqrt(TMath::Power(pHit->GetX()-pTrig[iSel]->GetX(),2.)
                                  +TMath::Power(pHit->GetY()-pTrig[iSel]->GetY(),2.)
	       		          +TMath::Power(pHit->GetZ()-pTrig[iSel]->GetZ(),2.));
		 */
                    // determine correction value
                    //if(fiBeamRefAddr  != iDetId) // do not do this for reference counter itself
                    if (fTRefMode
                        < 11)  // do not do this for trigger counter itself
                    {
                      Double_t dTentry = dTRef - dTTrig[iSel] + fdDelTofMax;
                      Int_t iBx = dTentry / 2. / fdDelTofMax * nbClDelTofBinX;
                      if (iBx < 0) iBx = 0;
                      if (iBx > nbClDelTofBinX - 1) iBx = nbClDelTofBinX - 1;
                      Double_t dBinWidth = 2. * fdDelTofMax / nbClDelTofBinX;
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
                      /*
			LOG(debug1)<<Form(" DelTof for SmT %d, Sm %d, R %d, T %d, dTRef %6.1f, Bx %d, Bx1 %d, DTe %6.1f -> DelT %6.1f",
				     iSmType, iSm, iRpc, iSel, dTRef-dTTrig[iSel], iBx, iBx1, dDTentry, dDelTof)
			      ;
		      */
                    }
                    dTTcor[iSel] = dDelTof * dZsign[iSel];
                    dTcor[iSel]  = pHit->GetTime() - dDelTof - dTTrig[iSel];
                    //		    Double_t dAvTot=0.5*(pDig0->GetTot()+pDig1->GetTot());
                  }  // if(iLink==0)

                  LOG(debug) << Form(
                    " TRpcCluWalk for Ev %d, Link %d(%d), Sel %d, TSR %d%d%d, "
                    "Ch %d,%d, S %d,%d T %f, DelTof %6.1f, W-ent:  %6.0f,%6.0f",
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
                    dTTrig[iSel],
                    dDelTof,
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
                  /*
		    LOG(debug1)<<Form(" TRpcCluWalk values side %d: %f, %f, side %d:  %f, %f ",
				 iS0,pDig0->GetTot(),pDig0->GetTime()
				 +((1.-2.*pDig0->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc))-dTTcor[iSel]-dTTrig[iSel],
				 iS1,pDig1->GetTot(),pDig1->GetTime()
				 +((1.-2.*pDig1->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc))-dTTcor[iSel]-dTTrig[iSel])
			  ;
		  */
                  fhTRpcCluWalk[iDetIndx][iSel][iCh0][iS0]->Fill(
                    pDig0->GetTot(),
                    //(pDig0->GetTime()+((1.-2.*pDig0->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc))-dTTrig[iSel])-dTTcor[iSel]);
                    //dTcor[iSel]+(1.-2.*pDig0->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc));
                    dTcor[iSel]);
                  fhTRpcCluWalk[iDetIndx][iSel][iCh1][iS1]->Fill(
                    pDig1->GetTot(),
                    //(pDig1->GetTime()+((1.-2.*pDig1->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc))-dTTrig[iSel])-dTTcor[iSel]);
                    //dTcor[iSel]+(1.-2.*pDig1->GetSide())*hitpos_local[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc));
                    dTcor[iSel]);

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
                    fhTRpcCluDelTof[iDetIndx][iSel]->Fill(dTRef - dTTrig[iSel],
                                                          dTcor[iSel]);
                    fhTSmCluTOff[iSmType][iSel]->Fill(
                      (Double_t)(iSm * iNbRpc + iRpc), dTcor[iSel]);
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
          LOG(error) << "FillHistos: invalid digi index " << iDetIndx
                     << " digi0,1" << iDigInd0 << ", " << iDigInd1
                     << " - max:" << fTofCalDigiVec->size()
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
      /*
	LOG(debug1)<<" Fill Time of iDetIndx "<<iDetIndx<<", hitAddr "
	        <<Form(" %08x, y = %5.2f",pHit->GetAddress(),hitpos_local[1])
		<<" for |y| <"
                <<fhRpcCluPosition[iDetIndx]->GetYaxis()->GetXmax()
                ;
      */
      if (TMath::Abs(hitpos_local[1])
          < (fhRpcCluPosition[iDetIndx]->GetYaxis()->GetXmax())) {
        if (dTRef != 0. && fTRefHits == 1) {
          fhRpcCluTOff[iDetIndx]->Fill((Double_t) iCh, pHit->GetTime() - dTRef);
          fhSmCluTOff[iSmType]->Fill((Double_t)(iSm * iNbRpc + iRpc),
                                     pHit->GetTime() - dTRef);

          for (Int_t iSel = 0; iSel < iNSel; iSel++)
            if (BSel[iSel]) {
              /*
		LOG(debug1)<<" TRpcCluTOff "<< iDetIndx <<", Sel "<< iSel
                           <<Form(", Dt %7.3f, LHsize %lu ",
			     pHit->GetTime()-dTTrig[iSel],fvLastHits[iSmType][iSm][iRpc][iCh].size());
	     */
              if (pHit->GetAddress() == pTrig[iSel]->GetAddress()) continue;

              if (fvLastHits[iSmType][iSm][iRpc][iCh].size()
                  > 1) {  // check for previous hits in memory time interval
                std::list<CbmTofHit*>::iterator itL =
                  fvLastHits[iSmType][iSm][iRpc][iCh].end();
                itL--;
                for (UInt_t iH = 0;
                     iH < fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1;
                     iH++) {
                  itL--;
                  //LOG(debug1)<<Form(" %f,",pHit->GetTime()-(*itL)->GetTime());
                }
              }

              // fill Time Offset histograms without velocity spread (DelTof) correction
              fhTRpcCluTOff[iDetIndx][iSel]->Fill(
                (Double_t) iCh,
                pHit->GetTime()
                  - dTTrig[iSel]);  // -dTTcor[iSel] only valid for matches
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
                    LOG(error) << Form("Invalid Time since last hit on channel "
                                       "TSRC %d%d%d%d: %f > %f",
                                       iSmType,
                                       iSm,
                                       iRpc,
                                       iCh,
                                       dTsinceLast,
                                       fdMemoryTime);

                  fhTRpcCluTOffDTLastHits[iDetIndx][iSel]->Fill(
                    TMath::Log10(dTsinceLast), pHit->GetTime() - dTTrig[iSel]);
                  fhTRpcCluMemMulDTLastHits[iDetIndx][iSel]->Fill(
                    TMath::Log10(dTsinceLast),
                    fvLastHits[iSmType][iSm][iRpc][iCh].size() - 1);
                }
              }
            }
        }
      }
    }
  }
  return kTRUE;
}

static Int_t iNPulserFound = 0;
Bool_t CbmDeviceHitBuilderTof::MonitorPulser() {
  iNPulserFound++;
  const Int_t iDet0   = fiPulDetRef;  // Define reference detector
  const Double_t Tlim = 0.5;
  Int_t iDigi0        = -1;
  switch (fiPulserMode) {
    case 1:  // mcbm :
      if ((UInt_t) fiNDigiIn
          != fiPulMulMin * 2 + 2) {  // 2 * working RPCs + 1 Diamond
        LOG(debug) << "Incomplete or distorted pulser event " << iNPulserFound
                   << " with " << fiNDigiIn << " digis instead of "
                   << fiPulMulMin * 2 + 2;
        if ((UInt_t) fiNDigiIn < fiPulMulMin * 2) return kTRUE;
      }
      break;
    case 2:  // micro CBM cosmic
      if ((UInt_t) fiNDigiIn < fiPulMulMin * 2) return kTRUE;
      break;
      ;
    default:;
  }

  for (Int_t iDigi = 0; iDigi < (Int_t) fiNDigiIn; iDigi++) {
    Int_t iCh = fvDigiIn[iDigi].GetChannel();
    if (iCh != 0 && iCh != 31) continue;

    Int_t iAddr = fvDigiIn[iDigi].GetAddress() & DetMask;
    Int_t iDet  = fDetIdIndexMap[iAddr];
    if (iDet == iDet0) {
      Int_t iSide = fvDigiIn[iDigi].GetSide();
      if (iSide == 1) {  // for pulser mode 1
        iDigi0 = iDigi;
        break;
      }
    }
  }

  if (iDigi0 == -1) {
    LOG(error) << Form(
      "Pulser reference %d not found in pulser event %d of mul %d, return",
      iDet0,
      iNPulserFound,
      fiNDigiIn);
    return kTRUE;
  }

  if (fvPulserTimes[iDet0][0].size() > 0)
    LOG(debug) << fiNDigiIn << " pulser digis with ref in "
               << Form("0x%08x at %d with deltaT %12.3f",
                       fvDigiIn[iDigi0].GetAddress(),
                       iDigi0,
                       fvDigiIn[iDigi0].GetTime()
                         - fvPulserTimes[iDet0][0].back());

  for (Int_t iDigi = 0; iDigi < fiNDigiIn; iDigi++) {
    Int_t iCh = fvDigiIn[iDigi].GetChannel();

    Int_t iDetIndx =
      fDigiBdfPar->GetDetInd(fvDigiIn[iDigi].GetAddress() & DetMask);
    fhRpcDigiTot[iDetIndx]->Fill(2 * iCh + fvDigiIn[iDigi].GetSide(),
                                 fvDigiIn[iDigi].GetTot());

    Int_t iAddr = fvDigiIn[iDigi].GetAddress() & DetMask;
    Int_t iDet  = fDetIdIndexMap[iAddr];
    Int_t iSide = fvDigiIn[iDigi].GetSide();

    switch (fiPulserMode) {
      case 0:
      case 1:  // mCBM
        if (fvDigiIn[iDigi].GetType() != 5) {
          if (iCh != 0 && iCh != 31) continue;
          if (fvDigiIn[iDigi].GetTot() > fiPulTotMax
              || fvDigiIn[iDigi].GetTot() < fiPulTotMin)
            continue;
        } else {
          LOG(debug) << "Found PulHit of Type 5 in Ch " << iCh;
          //    if (iCh == 5)    iSide=1; // Special case for diamond, pulser for channels 5-8 in channel 5, stored as side 1
          //    if (iCh !=0 && iCh != 5) continue;
          if (iCh > 39) iSide = 1;  // Special case for diamond in Mar2019
          if (iCh != 0 && iCh != 40) continue;
        }
        break;

      case 2:
        if (fvDigiIn[iDigi].GetType() == 8)  // ceramic RPC
          if (fvDigiIn[iDigi].GetRpc() != 7) continue;
        if (iCh != 0 && iCh != 31) continue;
        break;
    }

    if (fvPulserTimes[iDet][iSide].size() == (UInt_t) NPulserTimes)
      fvPulserTimes[iDet][iSide].pop_front();
    if (iDet == iDet0 && 1 == iSide) {
      // check consistency of latest hit
      if (fvPulserTimes[iDet][iSide].size() > 1) {
        Double_t TDif = (fvPulserTimes[iDet][iSide].back()
                         - fvPulserTimes[iDet][iSide].front())
                        / (fvPulserTimes[iDet][iSide].size() - 1);
        if (TMath::Abs(fvDigiIn[iDigi].GetTime()
                       - fvPulserTimes[iDet][iSide].back() - TDif)
            > 1.E3) {  // probably due to parallel processing

          // action, reset
          fvPulserTimes[iDet][iSide].clear();
        } else {
          if (TMath::Abs(fvDigiIn[iDigi].GetTime()
                         - fvPulserTimes[iDet][iSide].back() - TDif)
              > 10.) {
            LOG(info) << Form("Unexpected Pulser Time for event %d, pulser %d: "
                              "%15.1f instead %15.1f, TDif: %10.1f != %10.1f",
                              (int) fdEvent,
                              iNPulserFound,
                              fvDigiIn[iDigi].GetTime(),
                              fvPulserTimes[iDet][iSide].back() + TDif,
                              fvDigiIn[iDigi].GetTime()
                                - fvPulserTimes[iDet][iSide].back(),
                              TDif);
            // append dummy entry
            fvPulserTimes[iDet][iSide].push_back(
              (Double_t)(fvPulserTimes[iDet][iSide].back() + TDif));
            continue;
          }
        }
      }
      // append new entry
      fvPulserTimes[iDet][iSide].push_back(
        (Double_t)(fvDigiIn[iDigi].GetTime()));
    } else {
      Double_t dDeltaT0 =
        (Double_t)(fvDigiIn[iDigi].GetTime() - fvDigiIn[iDigi0].GetTime());

      // check consistency of latest hit
      if (fvPulserOffset[iDet][iSide] != 0.)
        if (TMath::Abs(dDeltaT0 - fvPulserOffset[iDet][iSide]) > Tlim) {
          LOG(info) << "Unexpected pulser offset at ev " << fdEvent
                    << ", pulcnt " << iNPulserFound << " for Det " << iDet
                    << Form(", addr 0x%08x", iAddr) << ", side " << iSide
                    << ": " << dDeltaT0 - fvPulserOffset[iDet][iSide] << " ( "
                    << fvPulserTimes[iDet][iSide].size() << " ) ";
          fvPulserTimes[iDet][iSide].clear();
          // skip this event
          // continue;
        }

      // fill monitoring histo
      if (fvPulserOffset[iDet][iSide] != 0.) {
        fhPulserTimesRaw->Fill(iDet * 2 + iSide,
                               dDeltaT0 - fvPulserOffset[iDet][iSide]);
        fhPulserTimeRawEvo[iDet * 2 + iSide]->Fill(
          iNPulserFound, dDeltaT0 - fvPulserOffset[iDet][iSide]);
      }
      // append new entry
      fvPulserTimes[iDet][iSide].push_back(dDeltaT0);
    }
  }

  for (Int_t iDet = 0; iDet < (Int_t) fvPulserTimes.size(); iDet++) {
    for (Int_t iSide = 0; iSide < 2; iSide++) {
      if (iDet == iDet0 && iSide == 1) continue;  // skip reference counter
      if (fvPulserTimes[iDet][iSide].size() > 0) {
        Double_t Tmean = 0.;
        std::list<Double_t>::iterator it;
        for (it = fvPulserTimes[iDet][iSide].begin();
             it != fvPulserTimes[iDet][iSide].end();
             ++it)
          Tmean += *it;
        Tmean /= fvPulserTimes[iDet][iSide].size();

        if (TMath::Abs(Tmean - fvPulserOffset[iDet][iSide]) > Tlim)
          LOG(debug) << "New pulser offset at ev " << fdEvent << ", pulcnt "
                     << iNPulserFound << " for Det " << iDet << ", side "
                     << iSide << ": " << Tmean << " ( "
                     << fvPulserTimes[iDet][iSide].size() << " ) ";

        fvPulserOffset[iDet][iSide] = Tmean;
      }
    }
  }

  for (Int_t iDigi = 0; iDigi < fiNDigiIn; iDigi++) {

    Int_t iCh   = fvDigiIn[iDigi].GetChannel();
    Int_t iAddr = fvDigiIn[iDigi].GetAddress() & DetMask;
    Int_t iDet  = fDetIdIndexMap[iAddr];
    Int_t iSide = fvDigiIn[iDigi].GetSide();

    switch (fiPulserMode) {
      case 1:
        if (fvDigiIn[iDigi].GetType() != 5) {
          if (iCh != 0 && iCh != 31) continue;
        } else {
          //if (iCh == 5)    iSide=1; // Special case for diamond, pulser for channels 5-8 in channel 5, stored as side 1
          if (iCh > 39) iSide = 1;  // Special case for diamond in Mar2019
          //	if(iCh !=0 && iCh != 5) continue;
          if (iCh != 0 && iCh != 40) continue;
        }
        break;
      case 2:
        if (fvDigiIn[iDigi].GetType() == 8)  // ceramic RPC
          if (fvDigiIn[iDigi].GetRpc() != 7) continue;
        if (iCh != 0 && iCh != 31) continue;
        break;
    }

    Double_t dDeltaT0 =
      (Double_t)(fvDigiIn[iDigi].GetTime() - fvDigiIn[iDigi0].GetTime())
      - fvPulserOffset[iDet][iSide];

    // fill monitoring histo
    fhPulserTimesCor->Fill(iDet * 2 + iSide, dDeltaT0);
  }
  return kTRUE;
}

Bool_t CbmDeviceHitBuilderTof::ApplyPulserCorrection() {
  for (Int_t iDigi = 0; iDigi < fiNDigiIn; iDigi++) {
    Int_t iAddr = fvDigiIn[iDigi].GetAddress() & DetMask;
    Int_t iDet  = fDetIdIndexMap[iAddr];
    Int_t iSide = fvDigiIn[iDigi].GetSide();
    switch (fiPulserMode) {
      case 1:  // diamond has pulser in ch 0 and 5
        if (5 == fvDigiIn[iDigi].GetType()) {
          continue;  // no pulser correction for diamond, mapping to be resolved
          Int_t iCh = fvDigiIn[iDigi].GetChannel();
          if (iCh > 4) iSide = 1;
        }
        break;
      case 2:  // correct all cer pads by same rpc/side 0
        if (8 == fvDigiIn[iDigi].GetType() || 5 == fvDigiIn[iDigi].GetType()) {
          const Int_t iRefAddr = 0x00078006;
          iDet                 = fDetIdIndexMap[iRefAddr];
        }
        break;
    }
    fvDigiIn[iDigi].SetTime(fvDigiIn[iDigi].GetTime()
                            - fvPulserOffset[iDet][iSide]);
  }
  return kTRUE;
}
