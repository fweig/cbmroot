/*First Version of TestBeam tools, developed for the SPS2016 beamtime and the legacy DAQ-chain.
 *Intended for the F/MS Prototypes.
 *For usage with Bucharest, modifications of the functions GetChannelMap and GetRowID are neccessary.
*/

#include "CbmTrdTestBeamToolsBuch2016.h"
#include "FairLogger.h"
#include "CbmTrdAddress.h"
#include <algorithm>

ClassImp(CbmTrdTestBeamToolsBuch2016)

CbmTrdTestBeamToolsBuch2016::CbmTrdTestBeamToolsBuch2016 () : CbmTrdTestBeamTools()
{
  LOG(info) << TString("Default Constructor of ")+TString(this->GetName());
  Instance(this);
}
