/**
 * CbmDeviceStsLocalReco.cxx
 *
 * @since 2019-03-26
 * @author F. Uhlig
 */

#include "CbmDeviceStsLocalReco.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig

CbmDeviceStsLocalReco::CbmDeviceStsLocalReco()
  : FairMQDevice(), fMaxTimeslices {0}, fNumMessages {0} {}

void CbmDeviceStsLocalReco::InitTask() {
  //    fMaxTimeslices = fConfig->GetValue<uint64_t>("max-timeslices");
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceStsLocalReco::HandleData(FairMQMessagePtr& msg, int /*index*/) {
  return true;
}

CbmDeviceStsLocalReco::~CbmDeviceStsLocalReco() {}
