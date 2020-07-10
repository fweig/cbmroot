/** @file CbmStsModule.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 14.05.2013
 **/
#include "CbmStsModule.h"

#include "CbmStsAddress.h"    // for GetAddress, SetElementId, ToString
#include "CbmStsParAsic.h"    // for CbmStsParAsic
#include "CbmStsParModule.h"  // for CbmStsParModule
#include "CbmStsSensor.h"     // for CbmStsSensor

#include <FairLogger.h>  // for LOG, Logger

#include <TGeoManager.h>       // for gGeoManager
#include <TGeoNode.h>          // for TGeoNode
#include <TGeoPhysicalNode.h>  // for TGeoPhysicalNode

#include <assert.h>  // for assert
#include <ostream>   // for operator<<, basic_ostream, stringstream
#include <string>    // for string

using namespace std;


// -----   Default constructor   -------------------------------------------
CbmStsModule::CbmStsModule(UInt_t address,
                           TGeoPhysicalNode* node,
                           CbmStsElement* mother)
  : CbmStsElement(address, kStsModule, node, mother) {}
// -------------------------------------------------------------------------


// --- Destructor   --------------------------------------------------------
CbmStsModule::~CbmStsModule() {}
// -------------------------------------------------------------------------


// -----   Get the unique address from the sensor name (static)   ----------
Int_t CbmStsModule::GetAddressFromName(TString name) {

  Bool_t isValid = kTRUE;
  if (name.Length() != 16) isValid = kFALSE;
  if (isValid) {
    if (!name.BeginsWith("STS")) isValid = kFALSE;
    if (name[4] != 'U') isValid = kFALSE;
    if (name[8] != 'L') isValid = kFALSE;
    if (name[13] != 'M') isValid = kFALSE;
  }
  if (!isValid) {
    LOG(fatal) << "GetAddressFromName: Not a valid module name " << name;
    return 0;
  }

  Int_t unit    = 10 * (name[5] - '0') + name[6] - '0' - 1;
  Int_t ladder  = 10 * (name[9] - '0') + name[10] - '0' - 1;
  Int_t hLadder = (name[11] == 'U' ? 0 : 1);
  Int_t module  = 10 * (name[14] - '0') + name[15] - '0' - 1;

  return CbmStsAddress::GetAddress(unit, ladder, hLadder, module);
}
// -------------------------------------------------------------------------


// -----   Initialise daughters from geometry   ----------------------------
void CbmStsModule::InitDaughters() {

  // --- Catch absence of TGeoManager
  assert(gGeoManager);

  // --- Catch physical node not being set
  assert(fNode);

  TGeoNode* moduleNode = fNode->GetNode();  // This node
  TString modulePath   = fNode->GetName();  // Full path to this node

  for (Int_t iNode = 0; iNode < moduleNode->GetNdaughters(); iNode++) {

    // Check name of daughter node for level name
    TString daughterName = moduleNode->GetDaughter(iNode)->GetName();
    if (daughterName.Contains("Sensor", TString::kIgnoreCase)) {

      // Create physical node
      TString daughterPath         = modulePath + "/" + daughterName;
      TGeoPhysicalNode* sensorNode = new TGeoPhysicalNode(daughterPath.Data());

      // Get or create element from setup and add it as daughter
      Int_t address =
        CbmStsAddress::SetElementId(fAddress, kStsSensor, GetNofDaughters());
      CbmStsSensor* sensor = new CbmStsSensor(address, sensorNode, this);
      fDaughters.push_back(sensor);

    }  //? name of daughter node contains "sensor"

  }  //# daughter nodes
}
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
string CbmStsModule::ToString() const {
  stringstream ss;
  ss << GetName() << ", address " << CbmStsAddress::ToString(fAddress);
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmStsModule)
