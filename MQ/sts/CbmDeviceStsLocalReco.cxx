/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/**
 * CbmDeviceStsLocalReco.cxx
 *
 * @since 2019-03-26
 * @author F. Uhlig
 */

#include "CbmDeviceStsLocalReco.h"

#include "CbmBsField.h"
#include "CbmFieldConst.h"
#include "CbmFieldMap.h"
#include "CbmFieldMapDistorted.h"
#include "CbmFieldMapSym1.h"
#include "CbmFieldMapSym2.h"
#include "CbmFieldMapSym3.h"
#include "CbmFieldPar.h"
#include "CbmMQDefs.h"
#include "CbmStsDigitizeParameters.h"

#include "FairField.h"
#include "FairGeoParSet.h"
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairRunAna.h"

//#include "FairParGenericSet.h"
//#include "RootSerializer.h"

#include "TGeoManager.h"
#include "TSystem.h"

/*
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TNamed.h"
*/

#include <boost/archive/binary_iarchive.hpp>

#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

//using namespace std;
using std::string;

CbmDeviceStsLocalReco::CbmDeviceStsLocalReco()
  : fMaxTimeslices {0}
  , fNumMessages {0}
  , fRunId {"0"}
  , fvmcworkdir {""}
  , fDigiPar {nullptr}
  , fGeoPar {nullptr}
  , fFieldPar {nullptr}  //  , fParCList{nullptr}
{
}

CbmDeviceStsLocalReco::~CbmDeviceStsLocalReco()
{
  if (gGeoManager) {
    gGeoManager->GetListOfVolumes()->Delete();
    gGeoManager->GetListOfShapes()->Delete();
  }
}


void CbmDeviceStsLocalReco::InitTask()
try {
  fMaxTimeslices = fConfig->GetValue<uint64_t>("max-timeslices");
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
    OnData(entry.first, &CbmDeviceStsLocalReco::HandleData);
  }
  InitContainers();
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}


bool CbmDeviceStsLocalReco::IsChannelNameAllowed(std::string channelName)
{
  for (auto const& entry : fAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1 != std::string::npos) {
      const std::vector<std::string>::const_iterator pos =
        std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const std::vector<std::string>::size_type idx = pos - fAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName << " found in list of allowed channel names at position " << idx;
      return true;
    }
  }
  LOG(info) << "Channel name " << channelName << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

Bool_t CbmDeviceStsLocalReco::InitContainers()
{
  Bool_t initOK {kTRUE};


  fRunId         = fConfig->GetValue<string>("run-id");
  fvmcworkdir    = fConfig->GetValue<string>("vmcworkdir");
  fMaxTimeslices = fConfig->GetValue<uint64_t>("max-timeslices");

  LOG(info) << "Init parameter containers for CbmDeviceStsLocalReco.";

  // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
  // Should only be used for small data because of the cost of an additional copy

  std::string message {"CbmStsDigitizeParameters,"};
  message += fRunId;
  LOG(info) << "Requesting parameter container CbmStsDigitizeParameters, "
               "sending message: "
            << message;

  FairMQMessagePtr req(NewSimpleMessage(message));
  FairMQMessagePtr rep(NewMessage());

  if (Send(req, "parameters") > 0) {
    if (Receive(rep, "parameters") >= 0) {
      if (rep->GetSize() != 0) {
        CbmMqTMessage tmsg(rep->GetData(), rep->GetSize());
        fDigiPar = dynamic_cast<CbmStsDigitizeParameters*>(tmsg.ReadObject(tmsg.GetClass()));
        LOG(info) << "Received unpack parameter from parmq server: " << fDigiPar;
        // TODO: check if fDigiPar is properly initialized from the file
        fDigiPar->Print();
        LOG(info) << fDigiPar->ToString();
      }
      else {
        throw InitTaskError("Received empty reply. Parameter not available");
      }
    }
  }

  std::string message1 {"FairGeoParSet,"};
  message1 += fRunId;
  LOG(info) << "Requesting parameter container FairGeoParSet, sending message: " << message1;

  FairMQMessagePtr req1(NewSimpleMessage(message1));
  FairMQMessagePtr rep1(NewMessage());

  if (Send(req1, "parameters") > 0) {
    if (Receive(rep1, "parameters") >= 0) {
      if (rep1->GetSize() != 0) {
        CbmMqTMessage tmsg(rep1->GetData(), rep1->GetSize());
        fGeoPar = static_cast<FairGeoParSet*>(tmsg.ReadObject(tmsg.GetClass()));
        LOG(info) << "Received unpack parameter from parmq server: " << fGeoPar;
        fGeoPar->Print();
        if (!gGeoManager) { throw InitTaskError("No gGeoManager found in FairGeoParSet"); }
        else {
          gGeoManager->Print();
        }
      }
      else {
        throw InitTaskError("Received empty reply. Parameter not available");
      }
    }
  }

  std::string message2 {"CbmFieldPar,"};
  message2 += fRunId;
  LOG(info) << "Requesting parameter container CbmFieldPar, sending message: " << message2;

  FairMQMessagePtr req2(NewSimpleMessage(message2));
  FairMQMessagePtr rep2(NewMessage());

  if (Send(req2, "parameters") > 0) {
    if (Receive(rep2, "parameters") >= 0) {
      if (rep2->GetSize() != 0) {
        CbmMqTMessage tmsg(rep2->GetData(), rep2->GetSize());
        fFieldPar = static_cast<CbmFieldPar*>(tmsg.ReadObject(tmsg.GetClass()));
        LOG(info) << "Received unpack parameter from parmq server: " << fGeoPar;
        if (-1 == fFieldPar->GetType()) { throw InitTaskError("No field parameters available!"); }
        else {
          fFieldPar->Print();
          LOG(info) << "Before creating the field";
          FairField* field = createField();
          LOG(info) << "After creating the field";
          FairRunAna* run = new FairRunAna();
          run->SetField(field);
        }
      }
      else {
        LOG(error) << "Received empty reply. Parameter not available";
      }
    }
  }

  return initOK;
  return true;
}


FairField* CbmDeviceStsLocalReco::createField()
{
  FairField* fMagneticField {nullptr};

  // Instantiate correct field type
  Int_t fType = fFieldPar->GetType();
  gSystem->Setenv("VMCWORKDIR", fvmcworkdir.c_str());
  if (fType == 0) fMagneticField = new CbmFieldConst(fFieldPar);
  else if (fType == 1)
    fMagneticField = new CbmFieldMap(fFieldPar);
  else if (fType == 2)
    fMagneticField = new CbmFieldMapSym2(fFieldPar);
  else if (fType == 3)
    fMagneticField = new CbmFieldMapSym3(fFieldPar);
  else if (fType == 4)
    fMagneticField = new CbmFieldMapDistorted(fFieldPar);
  else if (fType == 5)
    fMagneticField = new CbmFieldMapSym1(fFieldPar);
  else if (fType == 6)
    fMagneticField = new CbmBsField(fFieldPar);
  else {
    std::stringstream ss;
    ss << "Unknown field type " << fType;
    throw InitTaskError(ss.str());
  }
  LOG(info) << "New field at " << fMagneticField << ", type " << fType;

  // Initialise field
  if (fMagneticField) {
    fMagneticField->Init();
    fMagneticField->Print("");
  }

  LOG(info) << "Before return";
  return fMagneticField;
}


// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceStsLocalReco::HandleData(FairMQMessagePtr& msg, int /*index*/)
{
  // Don't do anything with the data
  // Maybe add an message counter which counts the incomming messages and add
  // an output
  fNumMessages++;
  LOG(debug) << "Received message number " << fNumMessages << " with size " << msg->GetSize();

  std::string msgStr(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  DoWork();

  if (fNumMessages % 10000 == 0) LOG(info) << "Processed " << fNumMessages << " time slices";

  SendData();

  return true;
}


bool CbmDeviceStsLocalReco::SendData() { return true; }

Bool_t CbmDeviceStsLocalReco::DoWork() { return true; }

void CbmDeviceStsLocalReco::Finish() {}
