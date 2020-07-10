/** @file CbmStsParSetSensor.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 31.03.2020
 **/
#include "CbmStsParSetSensor.h"

#include <FairLogger.h>  // for LOG, Logger

#include <cassert>  // for assert
#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits

ClassImp(CbmStsParSetSensor)

  // -----   Constructor   ----------------------------------------------------
  CbmStsParSetSensor::CbmStsParSetSensor(const char* name,
                                         const char* title,
                                         const char* context)
  : FairParGenericSet(name, title, context) {
  LOG(info) << "Constructor";
}
// --------------------------------------------------------------------------


// -----   Destructor   -----------------------------------------------------
CbmStsParSetSensor::~CbmStsParSetSensor() {}
// --------------------------------------------------------------------------


// -----   Reset   ----------------------------------------------------------
void CbmStsParSetSensor::clear() {
  fUseGlobal = kFALSE;
  fParams.clear();
  status = kFALSE;
  resetInputVersions();
}
// --------------------------------------------------------------------------


// -----   Read parameters from ASCII file   --------------------------------
Bool_t CbmStsParSetSensor::getParams(FairParamList*) {
  LOG(fatal) << GetName() << ": ASCII input is not defined!";
  return kFALSE;
}
// --------------------------------------------------------------------------


// -----   Get parameters of a sensor   -------------------------------------
const CbmStsParSensor& CbmStsParSetSensor::GetParSensor(UInt_t address) {
  if (fUseGlobal) return fGlobalParams;
  assert(fParams.count(address));
  return fParams[address];
}
// --------------------------------------------------------------------------


// -----   Write parameters from ASCII file   -------------------------------
void CbmStsParSetSensor::putParams(FairParamList*) {
  LOG(fatal) << GetName() << ": ASCII output is not defined!";
}
// --------------------------------------------------------------------------


// -----   Set sensor parameters   ------------------------------------------
void CbmStsParSetSensor::SetParSensor(UInt_t address,
                                      const CbmStsParSensor& par) {
  if (fParams.count(address))
    LOG(fatal) << GetName() << ": Replacing parameters for sensor address "
               << address;
  fParams[address] = par;
}
// --------------------------------------------------------------------------


// -----   Info to string   ------------------------------------------------
std::string CbmStsParSetSensor::ToString() const {
  std::stringstream ss;
  if (fUseGlobal)
    ss << "(Global) " << fGlobalParams.ToString();
  else {
    if (fParams.empty())
      ss << "Empty";
    else
      ss << "Parameters for " << fParams.size() << " sensors";
  }
  return ss.str();
}
// -------------------------------------------------------------------------
