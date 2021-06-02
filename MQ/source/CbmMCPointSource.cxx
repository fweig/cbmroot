/** @file CbmMCPointSource.cxx
  * @copyright Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

/**
 *  CbmMCPointSource.cpp
 *
 * @since 2019-12-02
 * @author F. Uhlig
 */

#include "CbmMCPointSource.h"

#include "CbmMQDefs.h"
#include "CbmMuchPoint.h"
#include "CbmMvdPoint.h"
#include "CbmRichPoint.h"
#include "CbmStsPoint.h"
#include "CbmTofPoint.h"
#include "CbmTrdPoint.h"
//#include "CbmEcalPoint.h"
#include "CbmPsdPoint.h"

#include "FairFileSource.h"
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairRootManager.h"
#include "FairRunAna.h"

#include "TClonesArray.h"

//#include <boost/archive/binary_oarchive.hpp>
// include this header to serialize vectors
//#include <boost/serialization/vector.hpp>

#include <thread>  // this_thread::sleep_for

#include <chrono>
#include <ctime>
#include <stdexcept>

#include <stdio.h>

using namespace std;

struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};


void CbmMCPointSource::InitTask()
try {
  // Get the values from the command line options (via fConfig)
  fFileName  = fConfig->GetValue<string>("filename");
  fMaxEvents = fConfig->GetValue<uint64_t>("max-events");


  LOG(info) << "Filename: " << fFileName;
  LOG(info) << "MaxEvents: " << fMaxEvents;

  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are allowed
  fChan.CheckChannels(this);
  fComponentsToSend = fChan.GetComponentsToSend();
  fChannelsToSend   = fChan.GetChannelsToSend();

  for (auto const& value : fComponentsToSend) {
    if (value > 1) {
      throw InitTaskError("Sending same data to more than one output channel "
                          "not implemented yet.");
    }
  }

  // Here we need to create an instance of FairRunAna to avoid a crash when creating the FairRootmanager
  // This is only a workaround since we actually don't need FairRunAna and the underlying problem has to
  // be fixed in FairRoot. The command ana->SetContainerStatic() is only
  // used to silence a compiler warning
  FairRunAna* ana = new FairRunAna();
  ana->SetContainerStatic();
  FairRootManager* rootman = FairRootManager::Instance();

  if (0 != fFileName.size()) {
    LOG(info) << "Open the ROOT input file " << fFileName;
    // Check if the input file exist
    FILE* inputFile = fopen(fFileName.c_str(), "r");
    if (!inputFile) { throw InitTaskError("Input file doesn't exist."); }
    fclose(inputFile);
    FairFileSource* source = new FairFileSource(fFileName);
    if (!source) { throw InitTaskError("Could not open input file."); }
    rootman->SetSource(source);
    rootman->InitSource();


    for (unsigned i = 0; i < fComponentsToSend.size(); i++) {
      if (1 == fComponentsToSend.at(i)) {  // there is a device connected which consumes data of this type
        std::vector<std::string> channel_name = fChannelsToSend.at(i);
        LOG(info) << channel_name.at(0);
        ConnectChannelIfNeeded(i, channel_name.at(0), "MvdPoint", rootman);
        ConnectChannelIfNeeded(i, channel_name.at(0), "StsPoint", rootman);
        ConnectChannelIfNeeded(i, channel_name.at(0), "RichPoint", rootman);
        ConnectChannelIfNeeded(i, channel_name.at(0), "MuchPoint", rootman);
        ConnectChannelIfNeeded(i, channel_name.at(0), "TrdPoint", rootman);
        ConnectChannelIfNeeded(i, channel_name.at(0), "TofPoint", rootman);
        //        ConnectChannelIfNeeded(i, channel_name.at(0), "EcalPoint", rootman);
        ConnectChannelIfNeeded(i, channel_name.at(0), "PsdPoint", rootman);
      }
      else {
        fArrays.at(i) = nullptr;
      }
    }
  }
  else {
    throw InitTaskError("No input file specified");
  }

  Int_t MaxAllowed = FairRootManager::Instance()->CheckMaxEventNo(fMaxEvents);
  if (MaxAllowed != -1) {
    if (fMaxEvents == 0) { fMaxEvents = MaxAllowed; }
    else {
      if (static_cast<Int_t>(fMaxEvents) > MaxAllowed) {
        LOG(warn) << "-------------------Warning---------------------------";
        LOG(warn) << " File has less events than requested!!";
        LOG(warn) << " File contains : " << MaxAllowed << " Events";
        LOG(warn) << " Requested number of events = " << fMaxEvents << " Events";
        LOG(warn) << " The number of events is set to " << MaxAllowed << " Events";
        LOG(warn) << "-----------------------------------------------------";
        fMaxEvents = MaxAllowed;
      }
    }
    LOG(info) << "After checking, the run will run from event 0 "
              << " to " << fMaxEvents << ".";
  }
  else {
    LOG(info) << "continue running without stop";
  }


  fTime = std::chrono::steady_clock::now();
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

void CbmMCPointSource::ConnectChannelIfNeeded(int chan_number, std::string channel_name, std::string branchname,
                                              FairRootManager* rootman)
{
  if (0 == channel_name.compare(branchname)) {
    LOG(info) << "Found expected data type " << branchname;
    TClonesArray* arr = static_cast<TClonesArray*>(rootman->GetObject(branchname.c_str()));
    if (!arr) {
      LOG(info) << "Consuming device connected but no " << branchname << " array in input file!";
      fComponentsToSend.at(chan_number) = 0;  // Don't send to connected device since needed data is not in input
    }
    fArrays.at(chan_number) = arr;
  }
}


bool CbmMCPointSource::ConditionalRun()
{

  Int_t readEventReturn = FairRootManager::Instance()->ReadEvent(fEventCounter);
  //  LOG(info) <<"Return value: " << readEventReturn;

  if (readEventReturn != 0) {
    LOG(warn) << "FairRootManager::Instance()->ReadEvent(" << fEventCounter << ") returned " << readEventReturn
              << ". Breaking the event loop";
    CalcRuntime();
    return false;
  }

  for (unsigned i = 0; i < fComponentsToSend.size(); i++) {
    bool result = true;
    if (1 == fComponentsToSend.at(i)) {  // there is a device connected which consumes data of this type

      if (0 == fChannelsToSend.at(i).at(0).compare("MvdPoint")) {
        result = ConvertAndSend<CbmMvdPoint>(fArrays.at(i), i);
      }
      if (0 == fChannelsToSend.at(i).at(0).compare("StsPoint")) {
        result = ConvertAndSend<CbmStsPoint>(fArrays.at(i), i);
      }
      if (0 == fChannelsToSend.at(i).at(0).compare("RichPoint")) {
        result = ConvertAndSend<CbmRichPoint>(fArrays.at(i), i);
      }
      if (0 == fChannelsToSend.at(i).at(0).compare("MuchPoint")) {
        result = ConvertAndSend<CbmMuchPoint>(fArrays.at(i), i);
      }
      if (0 == fChannelsToSend.at(i).at(0).compare("TrdPoint")) {
        result = ConvertAndSend<CbmTrdPoint>(fArrays.at(i), i);
      }
      if (0 == fChannelsToSend.at(i).at(0).compare("TofPoint")) {
        result = ConvertAndSend<CbmTofPoint>(fArrays.at(i), i);
      }
      /*
      if (0 == fChannelsToSend.at(i).at(0).compare("EcalPoint")) {
	result = ConvertAndSend<CbmEcalPoint>(fArrays.at(i),i );
      }
*/
      if (0 == fChannelsToSend.at(i).at(0).compare("PsdPoint")) {
        result = ConvertAndSend<CbmPsdPoint>(fArrays.at(i), i);
      }

      if (!result) {
        LOG(error) << "Problem sending data";
        return false;
      }
    }
  }

  if (fEventCounter % 10000 == 0) LOG(info) << "Analyse Event " << fEventCounter;
  fEventCounter++;


  //  LOG(info) << "Counter: " << fEventCounter << " Events: " << fMaxEvents;
  if (fEventCounter < fMaxEvents) { return true; }
  else {
    CalcRuntime();
    return false;
  }
}

CbmMCPointSource::~CbmMCPointSource() {}

void CbmMCPointSource::CalcRuntime()
{
  std::chrono::duration<double> run_time = std::chrono::steady_clock::now() - fTime;

  LOG(info) << "Runtime: " << run_time.count();
  LOG(info) << "No more input data";
}
