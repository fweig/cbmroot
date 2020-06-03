/** @file CbmStsParSetModule.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 31.03.2020
 **/
#include "CbmStsParSetModule.h"

#include "CbmStsParModule.h"    // for CbmStsParModule

#include <FairLogger.h>         // for LOG, Logger

#include <cassert>              // for assert
#include <sstream>              // for operator<<, basic_ostream, stringstream
#include <string>               // for char_traits

ClassImp(CbmStsParSetModule)

// -----   Constructor   ----------------------------------------------------
CbmStsParSetModule::CbmStsParSetModule(const char* name,
                                       const char* title,
                                       const char* context)
: FairParGenericSet(name, title, context)
{
}
// --------------------------------------------------------------------------



// -----   Destructor   -----------------------------------------------------
CbmStsParSetModule::~CbmStsParSetModule()
{
}
// --------------------------------------------------------------------------



// -----   Reset   ----------------------------------------------------------
void CbmStsParSetModule::clear()
{
  fUseGlobal = kFALSE;
  fParams.clear();
  status = kFALSE;
  resetInputVersions();
}
// --------------------------------------------------------------------------



// -----   Read parameters from ASCII file   --------------------------------
Bool_t CbmStsParSetModule::getParams(FairParamList*) {
  LOG(fatal) << GetName() << ": ASCII input is not defined!";
  return kFALSE;
}
// --------------------------------------------------------------------------



// -----   Get condition parameters of a sensor   ---------------------------
const CbmStsParModule& CbmStsParSetModule::GetParModule(UInt_t address) {
  if ( fUseGlobal ) return fGlobalParams;
  assert(fParams.count(address));
  return fParams[address];
}
// --------------------------------------------------------------------------



// -----   Write parameters from ASCII file   -------------------------------
void CbmStsParSetModule::putParams(FairParamList*) {
  LOG(fatal) << GetName() << ": ASCII output is not defined!";
}
// --------------------------------------------------------------------------



// -----   Info to string   ------------------------------------------------
std::string CbmStsParSetModule::ToString() const {
  std::stringstream ss;
  if ( fUseGlobal ) ss << "(Global) " << fGlobalParams.ToString();
  else {
    if ( fParams.empty() ) ss << "Empty";
    else ss << "Parameters for " << fParams.size() << " sensors";
  }
  return ss.str();
}
// -------------------------------------------------------------------------

