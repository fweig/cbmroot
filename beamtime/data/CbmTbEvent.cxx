/**@file CbmTbEvent.cxx
 **@date 26.06.2009
 **@author V. Friese <v.friese@gsi.de>
 **/


#include "CbmTbEvent.h"
#include "CbmModuleList.h"

#include "FairLogger.h"

#include <iomanip>

using std::fixed;
using std::setprecision;

// -----   Constructor  ------------------------------------------------------ 
CbmTbEvent::CbmTbEvent()
  : FairEventHeader(),
    fTimeStart(0.),          
    fTimeStop(0.),           
    fEpochTime(0.),          
    fNofData(0),            
    fTimeBc(),
    fNofDigis(),
    fNofAux(0),
    fEventType(0),
    fTimeSum(0.)
{
  Clear();
}
// ---------------------------------------------------------------------------

// -----   Reset event   -----------------------------------------------------
void CbmTbEvent::Clear(Option_t*) {
  fEventTime          = 0.;
  fTimeStart     = 0.;
  fTimeStop      = 0.;
  fEpochTime     = 0.;
  fNofData       = 0;
  fNofAux        = 0;
  for (ECbmModuleId iSys = ECbmModuleId::kRef; iSys < ECbmModuleId::kNofSystems; ++iSys) 
    fNofDigis[iSys] = 0;
  fTimeBc.clear();
  fEventType     = 0;
  fTimeSum       = 0.;
 
}
// ---------------------------------------------------------------------------

// -----   Print   -----------------------------------------------------------
void CbmTbEvent::Print(Option_t* /*opt*/) const { 

  std::stringstream ss;
  ss << "Cbm Testbeam Event: " << setprecision(9) << fixed << "Event time " 
     << fEventTime << " s, duration " << setprecision(0) << GetDuration()
     << " ns, Beam " << fTimeBc.size();
  for (ECbmModuleId iSys = ECbmModuleId::kRef; iSys < ECbmModuleId::kNofSystems; ++iSys) {
  	if ( fNofDigis.at(iSys) ) {
  		TString systemName = CbmModuleList::GetModuleNameCaps(iSys);
  		ss << ", " << systemName << " " << fNofDigis.at(iSys);
  	}
  }
  LOG(info) << ss.str() << ", AUX " << fNofAux;
}
// ---------------------------------------------------------------------------

ClassImp(CbmTbEvent)







