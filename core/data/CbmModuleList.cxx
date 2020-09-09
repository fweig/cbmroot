/** @file CbmModuleList.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 13.06.2013
 **/
#include "CbmModuleList.h"

#include "CbmDefs.h"  // for ECbmModuleId enumerator

#include <Logger.h>  // for LOG macro

#include <utility>  // for pair

using std::map;

// -----   Initialisation of the list of modules   -------------------------
map<ECbmModuleId, TString> CbmModuleList::DefineModules() {
  map<ECbmModuleId, TString> data;

  data[ECbmModuleId::kRef]      = "ref";
  data[ECbmModuleId::kMvd]      = "mvd";
  data[ECbmModuleId::kSts]      = "sts";
  data[ECbmModuleId::kRich]     = "rich";
  data[ECbmModuleId::kMuch]     = "much";
  data[ECbmModuleId::kTrd]      = "trd";
  data[ECbmModuleId::kTof]      = "tof";
  data[ECbmModuleId::kEcal]     = "ecal";
  data[ECbmModuleId::kPsd]      = "psd";
  data[ECbmModuleId::kHodo]     = "hodo";
  data[ECbmModuleId::kDummyDet] = "dummy";
  data[ECbmModuleId::kT0]       = "t0";
  data[ECbmModuleId::kMagnet]   = "magnet";
  data[ECbmModuleId::kTarget]   = "target";
  data[ECbmModuleId::kPipe]     = "pipe";
  data[ECbmModuleId::kShield]   = "shield";
  data[ECbmModuleId::kPlatform] = "platform";
  data[ECbmModuleId::kCave]     = "cave";

  return data;
}
// -------------------------------------------------------------------------


// -----   Initialise static map -------------------------- ----------------
// This is done by using the copy constructor of std::map, calling the
// method DefineModules, which actually fills the static map.
map<ECbmModuleId, TString>
  CbmModuleList::fModules(CbmModuleList::DefineModules());
// -------------------------------------------------------------------------


// ------  Get module Id from module name  ---------------------------------
ECbmModuleId CbmModuleList::GetModuleId(const char* moduleName) {

  map<ECbmModuleId, TString>::iterator it = fModules.begin();
  while (it != fModules.end()) {
    if (!(it->second).CompareTo(moduleName, TString::kIgnoreCase))
      return it->first;
    it++;
  }
  return ECbmModuleId::kNotExist;
}
// -------------------------------------------------------------------------


// ------   Get module name from module Id   --------------------------------
TString CbmModuleList::GetModuleName(ECbmModuleId moduleId) {
  if (fModules.find(moduleId) == fModules.end()) {
    LOG(error) << "Module List: Illegal module Id " << moduleId;
    return "";
  }
  return fModules.find(moduleId)->second;
}
// -------------------------------------------------------------------------


// ------   Get module name in capitals from module Id   -------------------
TString CbmModuleList::GetModuleNameCaps(ECbmModuleId moduleId) {
  TString name = GetModuleName(moduleId);
  name.ToUpper();
  return name;
}
// -------------------------------------------------------------------------
