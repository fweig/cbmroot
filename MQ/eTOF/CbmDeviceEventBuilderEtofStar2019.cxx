/** @file CbmDeviceEventBuilderEtofStar2019.cxx
  * @copyright Copyright (C) 2019-2021 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer] **/

/**
 * CbmDeviceEventBuilderEtofStar2019.cxx
 */

#include "CbmDeviceEventBuilderEtofStar2019.h"

#include "CbmMQDefs.h"
#include "CbmStar2019EventBuilderEtofAlgo.h"
#include "CbmStar2019TofPar.h"

#include "StorableTimeslice.hpp"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"
#include "FairRuntimeDb.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include "TROOT.h"
#include "TString.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

// include this header to serialize vectors
#include <boost/serialization/vector.hpp>

#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;

//static Int_t iMess=0;
const Int_t DetMask = 0x003FFFFF;
static uint fiSelectComponents {0};

CbmDeviceEventBuilderEtofStar2019::CbmDeviceEventBuilderEtofStar2019()
  :  //CbmDeviceUnpackTofMcbm2018(),
  fNumMessages(0)
  , fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fbSandboxMode(kFALSE)
  , fbEventDumpEna(kFALSE)
  , fParCList(nullptr)
  , fulTsCounter(0)
  , fNumEvt(0)
  , fEventBuilderAlgo(nullptr)
  , fTimer()
  , fUnpackPar(nullptr)
  , fpBinDumpFile(nullptr)
{
  fEventBuilderAlgo = new CbmStar2019EventBuilderEtofAlgo();
}

CbmDeviceEventBuilderEtofStar2019::~CbmDeviceEventBuilderEtofStar2019() { delete fEventBuilderAlgo; }

void CbmDeviceEventBuilderEtofStar2019::InitTask()
try {
  // Get the information about created channels from the device
  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are possible/allowed
  // for the device
  // The idea is to check at initilization if the devices are
  // properly connected. For the time beeing this is done with a
  // nameing convention. It is not avoided that someone sends other
  // data on this channel.
  //logger::SetLogLevel("INFO");

  int noChannel = fChannels.size();
  LOG(info) << "Number of defined channels: " << noChannel;
  for (auto const& entry : fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
    if (entry.first == "syscmd") {
      OnData(entry.first, &CbmDeviceEventBuilderEtofStar2019::HandleMessage);
      continue;
    }
    //if(entry.first != "etofevts") OnData(entry.first, &CbmDeviceEventBuilderEtofStar2019::HandleData);
    if (entry.first != "etofevts") OnData(entry.first, &CbmDeviceEventBuilderEtofStar2019::HandleParts);
    else {
      fChannelsToSend[0].push_back(entry.first);
      LOG(info) << "Init to send data to channel " << fChannelsToSend[0][0];
    }
  }
  InitContainers();
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmDeviceEventBuilderEtofStar2019::IsChannelNameAllowed(std::string channelName)
{
  for (auto const& entry : fAllowedChannels) {
    LOG(info) << "Inspect " << entry;
    std::size_t pos1 = channelName.find(entry);
    if (pos1 != std::string::npos) {
      const vector<std::string>::const_iterator pos =
        std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos - fAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName << " found in list of allowed channel names at position " << idx;
      return true;
    }
  }
  LOG(info) << "Channel name " << channelName << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

Bool_t CbmDeviceEventBuilderEtofStar2019::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmDeviceEventBuilderEtofStar2019.";
  //  FairRuntimeDb* fRtdb = FairRuntimeDb::instance();

  // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
  // Should only be used for small data because of the cost of an additional copy
  std::string message {"CbmStar2019TofPar,111"};
  LOG(info) << "Requesting parameter container CbmStar2019TofPar, sending message: " << message;

  FairMQMessagePtr req(NewSimpleMessage("CbmStar2019TofPar,111"));
  FairMQMessagePtr rep(NewMessage());

  if (Send(req, "parameters") > 0) {
    if (Receive(rep, "parameters") >= 0) {
      if (rep->GetSize() != 0) {
        CbmMQTMessage tmsg(rep->GetData(), rep->GetSize());
        fUnpackPar = dynamic_cast<CbmStar2019TofPar*>(tmsg.ReadObject(tmsg.GetClass()));
        LOG(info) << "Received unpack parameter from parmq server: " << fUnpackPar;
        fUnpackPar->Print();
      }
      else {
        LOG(error) << "Received empty reply. Parameter not available";
      }
    }
  }


  SetParContainers();

  Bool_t initOK = kTRUE;
  initOK &= fEventBuilderAlgo->InitContainers();
  initOK &= ReInitContainers();  // needed for TInt parameters
  fEventBuilderAlgo->SetAddStatusToEvent(true);

  if (kTRUE == fbMonitorMode) {  //  CreateHistograms();
    initOK &= fEventBuilderAlgo->CreateHistograms();

    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos = fEventBuilderAlgo->GetHistoVector();
    /* FIXME
      /// Register the histos in the HTTP server
      THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
      for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )
      {
         server->Register( Form( "/%s", vHistos[ uHisto ].second.data() ), vHistos[ uHisto ].first );
      } // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

      server->RegisterCommand("/Reset_EvtBuild_Hist", "bStarEtof2019EventBuilderResetHistos=kTRUE");
      server->Restrict("/Reset_EvtBuild_Hist", "allow=admin");
     */
  }  // if( kTRUE == fbMonitorMode )

  return initOK;
}

void CbmDeviceEventBuilderEtofStar2019::SetParContainers()
{
  FairRuntimeDb* fRtdb = FairRuntimeDb::instance();

  fParCList = fEventBuilderAlgo->GetParList();

  LOG(info) << "Setting parameter containers for " << fParCList->GetEntries() << " entries ";

  for (Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);

    std::string sParamName {tempObj->GetName()};

    FairParGenericSet* newObj = dynamic_cast<FairParGenericSet*>(fRtdb->getContainer(sParamName.data()));
    LOG(info) << " - Get " << sParamName.data() << " at " << newObj;
    if (nullptr == newObj) {

      LOG(error) << "Failed to obtain parameter container " << sParamName << ", for parameter index " << iparC;
      return;
    }  // if( nullptr == newObj )
    if (iparC == 0) {
      newObj = (FairParGenericSet*) fUnpackPar;
      LOG(info) << " - Mod " << sParamName.data() << " to " << newObj;
    }
    fParCList->AddAt(newObj, iparC);
    //      delete tempObj;
  }  // for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
}

void CbmDeviceEventBuilderEtofStar2019::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  fEventBuilderAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmDeviceEventBuilderEtofStar2019::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  if (0 == fulTsCounter) {
    LOG(info) << "FIXME ===> Jumping 1st TS as corrupted with current FW + "
                 "FLESNET combination";
    fulTsCounter++;
    return kTRUE;
  }  // if( 0 == fulTsCounter )
  if (kFALSE == fEventBuilderAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in event builder algorithm class";
    return kTRUE;
  }  // if( kFALSE == fEventBuilderAlgo->ProcessTs( ts ) )

  std::vector<CbmTofStarSubevent2019>& eventBuffer = fEventBuilderAlgo->GetEventBuffer();

  for (UInt_t uEvent = 0; uEvent < eventBuffer.size(); ++uEvent) {
    /// Send the sub-event to the STAR systems
    Int_t iBuffSzByte = 0;
    void* pDataBuff   = eventBuffer[uEvent].BuildOutput(iBuffSzByte);
    if (NULL != pDataBuff) {
      /// Valid output, do stuff with it!
      //         Bool_t fbSendEventToStar = kFALSE;
      if (kFALSE == fbSandboxMode) {
        /*
             ** Function to send sub-event block to the STAR DAQ system
             *       trg_word received is packed as:
             *
             *       trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
             */
        /*
            star_rhicf_write( eventBuffer[ uEvent ].GetTrigger().GetStarTrigerWord(),
                              pDataBuff, iBuffSzByte );
	   */

        SendSubevent(eventBuffer[uEvent].GetTrigger().GetStarTrigerWord(), (char*) pDataBuff, iBuffSzByte, 0);

      }  // if( kFALSE == fbSandboxMode )

      LOG(debug) << "Sent STAR event with size " << iBuffSzByte << " Bytes"
                 << " and token " << eventBuffer[uEvent].GetTrigger().GetStarToken();
    }  // if( NULL != pDataBuff )
    else
      LOG(error) << "Invalid STAR SubEvent Output, can only happen if trigger "
                 << " object was not set => Do Nothing more with it!!! ";
  }  // for( UInt_t uEvent = 0; uEvent < eventBuffer.size(); ++uEvent )

  return kTRUE;
}


Bool_t CbmDeviceEventBuilderEtofStar2019::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for CbmDeviceEventBuilderEtofStar2019";
  Bool_t initOK = fEventBuilderAlgo->ReInitContainers();
  return initOK;
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceEventBuilderEtofStar2019::HandleData(FairMQMessagePtr& msg, int /*index*/)
{
  // Don't do anything with the data
  // Maybe add an message counter which counts the incomming messages and add
  // an output
  fNumMessages++;
  LOG(debug) << "Received message number " << fNumMessages << " with size " << msg->GetSize();

  std::string msgStr(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  fles::StorableTimeslice component {0};
  inputArchive >> component;

  CheckTimeslice(component);

  DoUnpack(component, 0);

  //  if(fNumMessages%10000 == 0) LOG(info)<<"Processed "<<fNumMessages<<" time slices";

  return true;
}

static Double_t dctime = 0.;

bool CbmDeviceEventBuilderEtofStar2019::HandleParts(FairMQParts& parts, int /*index*/)
{
  // Don't do anything with the data
  // Maybe add an message counter which counts the incomming messages and add
  // an output
  fNumMessages++;
  LOG(debug) << "Received message number " << fNumMessages << " with " << parts.Size() << " parts";

  fles::StorableTimeslice ts {0};  // rename ??? FIXME

  switch (fiSelectComponents) {
    case 0: {
      std::string msgStr(static_cast<char*>(parts.At(0)->GetData()), (parts.At(0))->GetSize());
      std::istringstream iss(msgStr);
      boost::archive::binary_iarchive inputArchive(iss);
      inputArchive >> ts;
      CheckTimeslice(ts);
      if (1 == fNumMessages) {
        LOG(info) << "Initialize TS components list to " << ts.num_components();
        for (size_t c {0}; c < ts.num_components(); c++) {
          auto systemID = static_cast<int>(ts.descriptor(c, 0).sys_id);
          LOG(info) << "Found systemID: " << std::hex << systemID << std::dec;
          fEventBuilderAlgo->AddMsComponentToList(c, systemID);  // TOF data
        }
      }
    } break;
    case 1: {
      fles::StorableTimeslice component {0};

      uint ncomp = parts.Size();
      for (uint i = 0; i < ncomp; i++) {
        std::string msgStr(static_cast<char*>(parts.At(i)->GetData()), (parts.At(i))->GetSize());
        std::istringstream iss(msgStr);
        boost::archive::binary_iarchive inputArchive(iss);
        //fles::StorableTimeslice component{i};
        inputArchive >> component;

        CheckTimeslice(component);
        fEventBuilderAlgo->AddMsComponentToList(i, 0x60);  // TOF data
        LOG(debug) << "HandleParts message " << fNumMessages << " with indx " << component.index();
      }
    } break;
    default:;
  }

  if (kFALSE == fEventBuilderAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in event builder algorithm class";
    return kTRUE;
  }  // if( kFALSE == fEventBuilderAlgo->ProcessTs( ts ) )

  std::vector<CbmTofStarSubevent2019>& eventBuffer = fEventBuilderAlgo->GetEventBuffer();
  LOG(debug) << "Process time slice " << fNumMessages << " with " << eventBuffer.size() << " events";

  //if(fNumMessages%10000 == 0) LOG(info)<<"Processed "<<fNumMessages<<" time slices";

  for (UInt_t uEvent = 0; uEvent < eventBuffer.size(); ++uEvent) {
    /// Send the sub-event to the STAR systems
    Int_t iBuffSzByte = 0;
    void* pDataBuff   = eventBuffer[uEvent].BuildOutput(iBuffSzByte);
    if (NULL != pDataBuff) {
      /// Valid output, do stuff with it!
      // Send to Star TriggerHandler, TBD
      if (kFALSE == fbSandboxMode) {
        /*
             ** Function to send sub-event block to the STAR DAQ system
             *       trg_word received is packed as:
             *
             *       trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
             */
        /*
            star_rhicf_write( eventBuffer[ uEvent ].GetTrigger().GetStarTrigerWord(),
                              pDataBuff, iBuffSzByte );
	   */
      }  // if( kFALSE == fbSandboxMode )
      SendSubevent(eventBuffer[uEvent].GetTrigger().GetStarTrigerWord(), (char*) pDataBuff, iBuffSzByte, 0);

      LOG(debug) << "Sent STAR event " << uEvent << " with size " << iBuffSzByte << " Bytes"
                 << ", token " << eventBuffer[uEvent].GetTrigger().GetStarToken() << ", TrigWord "
                 << eventBuffer[uEvent].GetTrigger().GetStarTrigerWord();
    }
  }

  if (0 == fulTsCounter % 10000) {
    LOG(info) << "Processed " << fulTsCounter << " TS,  CPUtime: " << dctime / 10. << " ms/TS";
    dctime = 0.;
  }
  fulTsCounter++;
  return true;
}

bool CbmDeviceEventBuilderEtofStar2019::HandleMessage(FairMQMessagePtr& msg, int /*index*/)
{
  const char* cmd    = (char*) (msg->GetData());
  const char cmda[4] = {*cmd};
  LOG(info) << "Handle message " << cmd << ", " << cmd[0];
  cbm::mq::LogState(this);

  // only one implemented so far "Stop"

  if (strcmp(cmda, "STOP")) {
    LOG(info) << "STOP";
    cbm::mq::ChangeState(this, cbm::mq::Transition::Ready);
    cbm::mq::LogState(this);
    cbm::mq::ChangeState(this, cbm::mq::Transition::DeviceReady);
    cbm::mq::LogState(this);
    cbm::mq::ChangeState(this, cbm::mq::Transition::Idle);
    cbm::mq::LogState(this);
    cbm::mq::ChangeState(this, cbm::mq::Transition::End);
    cbm::mq::LogState(this);
  }
  return true;
}


bool CbmDeviceEventBuilderEtofStar2019::CheckTimeslice(const fles::Timeslice& ts)
{
  if (0 == ts.num_components()) {
    LOG(error) << "No Component in TS " << ts.index();
    return 1;
  }
  auto tsIndex = ts.index();

  LOG(debug) << "Found " << ts.num_components() << " different components in timeslice " << tsIndex;

  /*
  for (size_t c = 0; c < ts.num_components(); ++c) {
    LOG(debug) << "Found " << ts.num_microslices(c)
              << " microslices in component " << c;
    LOG(debug) << "Component " << c << " has a size of "
              << ts.size_component(c) << " bytes";
    LOG(debug) << "Sys ID: Ox" << std::hex << static_cast<int>(ts.descriptor(0,0).sys_id)
            << std::dec;

    for (size_t m = 0; m < ts.num_microslices(c); ++m) {
      PrintMicroSliceDescriptor(ts.descriptor(c,m));
    }
  }
*/
  return true;
}

bool CbmDeviceEventBuilderEtofStar2019::SendEvent(std::vector<Int_t> vdigi, int idx)
{
  LOG(debug) << "Send Data for event " << fNumEvt << " with size " << vdigi.size() << Form(" at %p ", &vdigi);
  //  LOG(debug) << "EventHeader: "<< fEventHeader[0] << " " << fEventHeader[1] << " " << fEventHeader[2] << " " << fEventHeader[3];

  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << vdigi;
  std::string* strMsg = new std::string(oss.str());

  FairMQParts parts;
  parts.AddPart(NewMessage(
    const_cast<char*>(strMsg->c_str()),  // data
    strMsg->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsg));  // object that manages the data

  LOG(debug) << "Send data to channel " << idx << " " << fChannelsToSend[idx][0];


  //  if (Send(msg, fChannelsToSend[idx][0]) < 0) {
  if (Send(parts, fChannelsToSend[idx][0]) < 0) {
    LOG(error) << "Problem sending data " << fChannelsToSend[idx][0];
    return false;
  }
  fNumEvt++;
  //if(fNumEvt==100) FairMQStateMachine::ChangeState(PAUSE); //sleep(10000); // Stop for debugging ...
  return true;
}

bool CbmDeviceEventBuilderEtofStar2019::SendSubevent(uint trig, char* pData, int nData, int idx)
{

  LOG(debug) << "SendSubevent " << fNumEvt << ", TrigWord " << trig << " with size " << nData << Form(" at %p ", pData);

  std::stringstream ossE;
  boost::archive::binary_oarchive oaE(ossE);
  oaE << trig;
  std::string* strMsgE = new std::string(ossE.str());

  /*
  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << cData;
  std::string* strMsg = new std::string(oss.str());
  */

  std::string* strMsg = new std::string(pData, nData);

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

  LOG(debug) << "Send data to channel " << idx << " " << fChannelsToSend[idx][0];


  //  if (Send(msg, fChannelsToSend[idx][0]) < 0) {
  if (Send(parts, fChannelsToSend[idx][0]) < 0) {
    LOG(error) << "Problem sending data " << fChannelsToSend[idx][0];
    return false;
  }
  fNumEvt++;
  //if(fNumEvt==100) FairMQStateMachine::ChangeState(PAUSE); //sleep(10000); // Stop for debugging ...
  return true;
}

void CbmDeviceEventBuilderEtofStar2019::Reset() {}

void CbmDeviceEventBuilderEtofStar2019::Finish()
{
  if (NULL != fpBinDumpFile) {
    LOG(info) << "Closing binary file used for event dump.";
    fpBinDumpFile->close();
  }  // if( NULL != fpBinDumpFile )

  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos = fEventBuilderAlgo->GetHistoVector();

    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    /// (Re-)Create ROOT file to store the histos
    TFile* histoFile = nullptr;

    // open separate histo file in recreate mode
    histoFile = new TFile("data/eventBuilderMonHist.root", "RECREATE");
    histoFile->cd();

    /// Register the histos in the HTTP server
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      /// Make sure we end up in chosen folder
      gDirectory->mkdir(vHistos[uHisto].second.data());
      gDirectory->cd(vHistos[uHisto].second.data());

      /// Write plot
      vHistos[uHisto].first->Write();

      histoFile->cd();
    }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;

    histoFile->Close();
  }  // if( kTRUE == fbMonitorMode )
}
