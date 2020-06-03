/*First Version of TestBeam tools, developed for the SPS2016 beamtime and the legacy DAQ-chain.
 *Intended for the F/MS Prototypes.
 *For usage with Bucharest, modifications of the functions GetChannelMap and GetRowID are neccessary.
*/

#include "CbmTrdTestBeamTools2017DESY.h"
#include "FairLogger.h"
#include "CbmTrdAddress.h"
#include <algorithm>

ClassImp(CbmTrdTestBeamTools2017DESY)

CbmTrdTestBeamTools2017DESY::CbmTrdTestBeamTools2017DESY () : CbmTrdTestBeamTools()
{
  LOG(info) << TString("Default Constructor of ")+TString(this->GetName());
  Instance(this);
}
