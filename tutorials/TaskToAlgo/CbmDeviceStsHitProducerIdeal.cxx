/**
 * CbmDeviceStsHitProducerIdeal.cxx
 *
 * @since 2019-12-06
 * @author F. Uhlig
 */

#include "CbmDeviceStsHitProducerIdeal.h"

#include "CbmMQDefs.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmTrdParSetGas.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"
#include "FairRunAna.h"

#include "TList.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <stdexcept>
#include <string>
#include <vector>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

//using namespace std;
using std::string;

CbmDeviceStsHitProducerIdeal::CbmDeviceStsHitProducerIdeal()
  : fMaxEvents {0}
  , fNumMessages {0}
  , fRunId {"0"}
  , fvmcworkdir {""}
  , fTrdGasPar {nullptr}
{
}

CbmDeviceStsHitProducerIdeal::~CbmDeviceStsHitProducerIdeal() {}


void CbmDeviceStsHitProducerIdeal::InitTask()
try {

  fMaxEvents = fConfig->GetValue<uint64_t>("max-events");

  LOG(info) << "MaxEvents: " << fMaxEvents;

  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are allowed
  // Connnect channels which delivers StsPoints with the proper
  // function handling the data
  fChan.CheckChannels(this);
  fComponentsToSend = fChan.GetComponentsToSend();
  fChannelsToSend   = fChan.GetChannelsToSend();

  for (auto const& entry : fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    if (entry.first.compare("StsPoint")) LOG(info) << "Connect Channel " << entry.first << "with data type StsPoint";
    OnData(entry.first, &CbmDeviceStsHitProducerIdeal::HandleData);
  }
  // Initialize the algorithm and get the proper parameter containers
  fAlgo->Init();
  InitContainers();
}
catch (InitTaskError& e) {
  LOG(ERROR) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

Bool_t CbmDeviceStsHitProducerIdeal::InitContainers()
{
  Bool_t initOK {kTRUE};

  fRunId      = fConfig->GetValue<string>("run-id");
  fvmcworkdir = fConfig->GetValue<string>("vmcworkdir");
  fMaxEvents  = fConfig->GetValue<uint64_t>("max-events");

  LOG(INFO) << "Init parameter containers for CbmDeviceStsHitProducerIdeal.";


  TList* fParCList = fAlgo->GetParList();

  for (int iparC = 0; iparC < fParCList->GetEntries(); iparC++) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);
    std::string paramName {tempObj->GetName()};

    // NewSimpleMessage create s a copy of the data and takes care of its destruction (after the transfer takes place).
    // Should only be used for small data because of the cost of an additional copy

    // Her must come the proper Runid
    std::string message = paramName + ",111";
    LOG(info) << "Requesting parameter container " << paramName << ", sending message: " << message;

    FairMQMessagePtr req(NewSimpleMessage(message));
    FairMQMessagePtr rep(NewMessage());

    FairParGenericSet* newObj = nullptr;

    if (Send(req, "parameters") > 0) {
      if (Receive(rep, "parameters") >= 0) {
        if (rep->GetSize() != 0) {
          CbmMQTMessage tmsg(rep->GetData(), rep->GetSize());
          newObj = static_cast<FairParGenericSet*>(tmsg.ReadObject(tmsg.GetClass()));
          LOG(info) << "Received unpack parameter from the server:";
          newObj->print();
        }
        else {
          LOG(error) << "Received empty reply. Parameter not available";
        }  // if (rep->GetSize() != 0)
      }    // if (Receive(rep, "parameters") >= 0)
    }      // if (Send(req, "parameters") > 0)
    fParCList->AddAt(newObj, iparC);
    delete tempObj;
  }  // for ( int iparC = 0; iparC < fParCList->GetEntries(); iparC++ )

  // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
  // Should only be used for small data because of the cost of an additional copy

  initOK = fAlgo->InitContainers();

  return initOK;
  return true;
}


// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceStsHitProducerIdeal::HandleData(FairMQMessagePtr& msg, int /*index*/)
{

  fNumMessages++;
  LOG(DEBUG) << "Received message number " << fNumMessages << " with size " << msg->GetSize();


  // Unpack the message into a vector of CbmStsPoints
  std::string msgStr(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  std::vector<CbmStsPoint> points;
  inputArchive >> points;

  // Pass the vector to the algorithm
  // Get the vector with the newly created data objects from the algorithm
  std::vector<CbmStsHit> hits = fAlgo->ProcessInputData(points);

  // Event summary
  LOG(info) << "Out of " << points.size() << " StsPoints, " << hits.size() << " Hits created.";


  if (fNumMessages % 10000 == 0) LOG(INFO) << "Processed " << fNumMessages << " time slices";

  // Send the data to a consumer
  SendData();

  return true;
}


bool CbmDeviceStsHitProducerIdeal::SendData() { return true; }

Bool_t CbmDeviceStsHitProducerIdeal::DoWork() { return true; }

void CbmDeviceStsHitProducerIdeal::Finish() {}
