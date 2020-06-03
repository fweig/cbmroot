/**
 *  CbmStsDigiSource.cpp
 *
 * @since 2019-08-21
 * @author F. Uhlig
 */


#include "CbmStsDigiSource.h"
#include "CbmMQDefs.h"

#include "CbmDigiManager.h"
#include "CbmStsDigi.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h" // device->fConfig

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairFileSource.h"


#include <boost/archive/binary_oarchive.hpp>

#include <stdio.h>
#include <ctime>
#include <thread> // this_thread::sleep_for
#include <chrono>
#include <stdexcept>

using namespace std;

struct InitTaskError : std::runtime_error { using std::runtime_error::runtime_error; };


CbmStsDigiSource::CbmStsDigiSource()
  : FairMQDevice()
  , fMaxEvents(0)
  , fFileName("")
  , fInputFileList()
  , fFileCounter(0)
  , fEventNumber(0)
  , fEventCounter(0)
  , fMessageCounter(0)
  , fTime()
{
}

void CbmStsDigiSource::InitTask()
try
{
  // Get the values from the command line options (via fConfig)
  fFileName = fConfig->GetValue<string>("filename");
  fMaxEvents = fConfig->GetValue<uint64_t>("max-events");
  
  
  LOG(info) << "Filename: " << fFileName;
  LOG(info) << "MaxEvents: " << fMaxEvents;
  
  // Get the information about created channels from the device
  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are possible/allowed
  // for the device
  // The idea is to check at initilization if the devices are
  // properly connected. For the time beeing this is done with a
  // nameing convention. It is not avoided that someone sends other
  // data on this channel.
  int noChannel = fChannels.size();
  LOG(info) << "Number of defined output channels: " << noChannel;
  for(auto const &entry : fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
  }
  
  FairRootManager* rootman = FairRootManager::Instance();
  
  
  if ( 0 != fFileName.size() ) {
    LOG(info) << "Open the ROOT input file " << fFileName;
    // Check if the input file exist
    FILE* inputFile = fopen(fFileName.c_str(), "r");
    if ( ! inputFile )  {
      throw InitTaskError("Input file doesn't exist.");
    }
    fclose(inputFile);
    FairFileSource* source = new FairFileSource(fFileName); 
    if ( !source) {
      throw InitTaskError("Could not open input file.");
    }
    rootman->SetSource(source);
    rootman->InitSource();
    CbmDigiManager* digiMan = CbmDigiManager::Instance();
    digiMan->Init();
    if ( ! digiMan->IsPresent(ECbmModuleId::kSts) ) {
        throw InitTaskError("No StsDigi branch in input!");
    }
  } else {
      throw InitTaskError("No input file specified");
  }


  Int_t MaxAllowed=FairRootManager::Instance()->CheckMaxEventNo(fMaxEvents);
  if ( MaxAllowed != -1 ) {	
    if (fMaxEvents == 0) {
       fMaxEvents = MaxAllowed;
    } else {
      if (static_cast<Int_t>(fMaxEvents) > MaxAllowed) {
        LOG(warn) << "-------------------Warning---------------------------";
        LOG(warn) << " File has less events than requested!!";
        LOG(warn) << " File contains : " << MaxAllowed  << " Events";
        LOG(warn) << " Requested number of events = " <<  fMaxEvents <<  " Events";
        LOG(warn) << " The number of events is set to " << MaxAllowed << " Events";
        LOG(warn) << "-----------------------------------------------------";
        fMaxEvents = MaxAllowed;
      }
    }
    LOG(info) << "After checking, the run will run from event 0 " << " to " << fMaxEvents << ".";
  } else {
   LOG(info) << "continue running without stop";
  }


  fTime = std::chrono::steady_clock::now();
} catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmStsDigiSource::IsChannelNameAllowed(std::string channelName)
{
  if ( std::find(fAllowedChannels.begin(), fAllowedChannels.end(),
		 channelName) != fAllowedChannels.end() ) {
    LOG(info) << "Channel name " << channelName
              << " found in list of allowed channel names.";
    return true;
  } else {
    LOG(info) << "Channel name " << channelName
              << " not found in list of allowed channel names.";
    LOG(error) << "Stop device.";
    return false;
  }
}

bool CbmStsDigiSource::ConditionalRun()
{

   Int_t readEventReturn = FairRootManager::Instance()->ReadEvent(fEventCounter);
   LOG(info) <<"Return value: " << readEventReturn;

   if ( readEventReturn != 0 ) {
     LOG(warn) << "FairRootManager::Instance()->ReadEvent(" << fEventCounter << ") returned " << readEventReturn << ". Breaking the event loop";
     CalcRuntime();
     return false;
   }

   for (Int_t index = 0; index < CbmDigiManager::Instance()->GetNofDigis(ECbmModuleId::kSts); index++) {   
     const CbmStsDigi* stsDigi = CbmDigiManager::Instance()->Get<CbmStsDigi>(index);
     PrintStsDigi(stsDigi);
   }

   if (fEventCounter % 10000 == 0)  LOG(info) << "Analyse Event " << fEventCounter;
   fEventCounter++;
  

   LOG(info) << "Counter: " << fEventCounter << " Events: " << fMaxEvents;    
   if (fEventCounter < fMaxEvents) {
    return true;
   } else {
      CalcRuntime();
     return false;
   }
}


CbmStsDigiSource::~ CbmStsDigiSource()
{
}

void CbmStsDigiSource::CalcRuntime()
{
  std::chrono::duration<double> run_time =  
    std::chrono::steady_clock::now() - fTime;
  
  LOG(info) << "Runtime: " << run_time.count();
  LOG(info) << "No more input data";
}



void CbmStsDigiSource::PrintStsDigi(const CbmStsDigi* digi)
{
  LOG(info) << digi->ToString();
}
