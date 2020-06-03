/** @file CbmStsSensor.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 03.05.2013
 **/
#include "CbmStsSensor.h"

#include <ostream>              // for operator<<, stringstream, basic_ostream


// -----   Constructor   ---------------------------------------------------
CbmStsSensor::CbmStsSensor(UInt_t address, TGeoPhysicalNode* node,
                           CbmStsElement* mother) :
    CbmStsElement(address, kStsSensor, node, mother)
{
}
// -------------------------------------------------------------------------

// -----   Get the unique address from the sensor name (static)   ----------
UInt_t CbmStsSensor::GetAddressFromName(TString name) {

  Int_t unit    = 10 * ( name[5]  - '0') + name[6]  - '0' - 1;
  Int_t ladder  = 10 * ( name[9]  - '0') + name[10] - '0' - 1;
  Int_t hLadder = ( name[11] == 'U' ? 0 : 1);
  Int_t module  = 10 * ( name[14] - '0') + name[15] - '0' - 1;
  Int_t sensor  = 10 * ( name[18] - '0') + name[19] - '0' - 1;

  return CbmStsAddress::GetAddress(unit, ladder, hLadder, module, sensor);
}
// -------------------------------------------------------------------------



// -----   String output   -------------------------------------------------
std::string CbmStsSensor::ToString() const {
    std::stringstream ss;
    ss << GetName() << ", address " << CbmStsAddress::ToString(fAddress);
    return ss.str();
}
// -------------------------------------------------------------------------

ClassImp(CbmStsSensor)
