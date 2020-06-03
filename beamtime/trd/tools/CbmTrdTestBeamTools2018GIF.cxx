/*First Version of TestBeam tools, developed for the SPS2016 beamtime and the legacy DAQ-chain.
 *Intended for the F/MS Prototypes.
 *For usage with Bucharest, modifications of the functions GetChannelMap and GetRowID are neccessary.
*/

#include "CbmTrdTestBeamTools2018GIF.h"
#include "FairLogger.h"
#include "CbmTrdAddress.h"
#include <algorithm>

ClassImp(CbmTrdTestBeamTools2018GIF)

CbmTrdTestBeamTools2018GIF::CbmTrdTestBeamTools2018GIF () : CbmTrdTestBeamTools()
{
  LOG(info) << TString("Default Constructor of ")+TString(this->GetName());
  Instance(this);
}
