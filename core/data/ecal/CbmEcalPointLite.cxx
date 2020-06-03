#include "CbmEcalPointLite.h"

#include "FairLogger.h"

void CbmEcalPointLite::Print(const Option_t* /*opt*/) const
{
  LOG(info) << "CbmEcalPointLite: DetectorID=" << GetDetectorID() 
            << ", TrackID=" << GetTrackID() 
            << ", Eloss=" << GetEnergyLoss() 
            << ", Time=" << GetTime();
}

ClassImp(CbmEcalPointLite)
