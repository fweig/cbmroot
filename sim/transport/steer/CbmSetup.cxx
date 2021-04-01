#/** @file CbmSetup.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 13.06.2013
 **/


#include "CbmSetup.h"

#include "CbmAddress.h"
#include "CbmFieldMap.h"
#include "CbmFieldMapData.h"
#include "CbmFieldMapSym2.h"
#include "CbmFieldMapSym3.h"

#include "FairModule.h"
#include "FairRunSim.h"
#include <Logger.h>

#include "TFile.h"
#include "TGeoMatrix.h"
#include "TGeoNode.h"
#include "TGeoVolume.h"
#include "TKey.h"
#include "TSystem.h"

#include <iomanip>
#include <sstream>

using std::string;
using std::stringstream;

// -----   Initialise static instance   ------------------------------------
CbmSetup* CbmSetup::fgInstance = NULL;
// -------------------------------------------------------------------------


// -----   Clear the setup   -----------------------------------------------
void CbmSetup::Clear(Option_t*) { fProvider->Reset(); }
// -------------------------------------------------------------------------


// -----   Get field map type   --------------------------------------------
CbmFieldMap* CbmSetup::CreateFieldMap() {
  CbmGeoSetupField field = fProvider->GetSetup().GetField();

  // --- Open the map file
  TString fileName = field.GetFilePath();
  fileName         = TString(gSystem->Getenv("VMCWORKDIR")) + "/" + fileName;

  TFile mapFile(fileName);
  if (!mapFile.IsOpen()) {
    LOG(error) << "Could not open field map file " << fileName;
    return NULL;
  }

  // ---   Get map file type
  TString mapName = "field_";
  mapName += field.GetTag().c_str();
  CbmFieldMapData* data = NULL;
  mapFile.GetObject(mapName, data);
  if (!data) {
    LOG(error) << "Could not find CbmFieldMapData object " << mapName
               << " in file " << fileName;
    return NULL;
  }
  Int_t fieldType = data->GetType();

  // --- Instantiate field
  CbmFieldMap* fieldMap = NULL;
  switch (fieldType) {
    case 2: fieldMap = new CbmFieldMapSym2(mapName); break;
    case 3: fieldMap = new CbmFieldMapSym3(mapName); break;
    default: LOG(error) << "Unknown field type " << fieldType;
  }

  // --- Set scale and position of field map
  if (fieldMap) {
    fieldMap->SetScale(field.GetScale());
    fieldMap->SetPosition(field.GetMatrix().GetTranslation()[0],
                          field.GetMatrix().GetTranslation()[1],
                          field.GetMatrix().GetTranslation()[2]);
  }

  return fieldMap;
}
// -------------------------------------------------------------------------


// -----  Get a geometry file name   ---------------------------------------
Bool_t CbmSetup::GetGeoFileName(ECbmModuleId moduleId, TString& fileName) {
  auto& moduleMap = fProvider->GetSetup().GetModuleMap();

  if (moduleMap.find(moduleId) == moduleMap.end()) {
    fileName = "";
    return kFALSE;
  }
  fileName = moduleMap.at(moduleId).GetFilePath();
  return kTRUE;
}
// -------------------------------------------------------------------------


// -----  Get a geometry tag   ---------------------------------------------
Bool_t CbmSetup::GetGeoTag(ECbmModuleId moduleId, TString& tag) {
  auto& moduleMap = fProvider->GetSetup().GetModuleMap();

  if (moduleMap.find(moduleId) == moduleMap.end()) {
    tag = "";
    return kFALSE;
  }
  tag = moduleMap.at(moduleId).GetTag();
  return kTRUE;
}
// -------------------------------------------------------------------------


// -----   Instance   ------------------------------------------------------
CbmSetup* CbmSetup::Instance() {
  if (!fgInstance) fgInstance = new CbmSetup();
  return fgInstance;
}
// -------------------------------------------------------------------------


// -----  Get activity flag   ----------------------------------------------
Bool_t CbmSetup::IsActive(ECbmModuleId moduleId) {
  auto& moduleMap = fProvider->GetSetup().GetModuleMap();

  if (moduleMap.find(moduleId) == moduleMap.end()) return kFALSE;
  return moduleMap.at(moduleId).GetActive();
}
// -------------------------------------------------------------------------


// -----   Remove a module   -----------------------------------------------
void CbmSetup::RemoveModule(ECbmModuleId moduleId) {
  fProvider->RemoveModule(moduleId);
}
// -------------------------------------------------------------------------


// -----   Activate or deactivate a detector   -----------------------------
void CbmSetup::SetActive(ECbmModuleId moduleId, Bool_t active) {

  auto& moduleMap = fProvider->GetSetup().GetModuleMap();

  // Check presence of module in current setup
  if (moduleMap.find(moduleId) == moduleMap.end()) {
    LOG(warn) << "Module " << moduleId << " does not exist in setup!";
    return;
  }

  // Set activity flag
  moduleMap.at(moduleId).SetActive(active);
}
// -------------------------------------------------------------------------


// -----   Set the field map   ---------------------------------------------
void CbmSetup::SetField(const char* tag,
                        Double_t scale,
                        Double_t xPos,
                        Double_t yPos,
                        Double_t zPos) {

  LOG(warn) << GetName() << ": Overriding field map  "
            << fProvider->GetSetup().GetField().GetTag()
            << " (according to magnet geometry) with field map " << tag;

  CbmGeoSetupField field = fProvider->GetFieldByTag(tag);
  field.SetScale(scale);
  field.GetMatrix().SetTranslation(xPos, yPos, zPos);
  fProvider->GetSetup().SetField(field);
}
// -------------------------------------------------------------------------


// -----   Add or replace a module in the setup   --------------------------
void CbmSetup::SetModule(ECbmModuleId moduleId,
                         const char* geoTag,
                         Bool_t active) {

  std::map<ECbmModuleId, CbmGeoSetupModule> modmap =
    fProvider->GetSetup().GetModuleMap();

  if (modmap.find(moduleId) != modmap.end()) {

    CbmGeoSetupModule& module =
      fProvider->GetSetup().GetModuleMap().at(moduleId);
    // Check presence of module in current setup
    LOG(debug) << GetName() << ": Changing module " << moduleId << ": "
               << module.GetTag() << " -> " << geoTag;
  }
  fProvider->SetModuleTag(moduleId, geoTag, active);
}
// -------------------------------------------------------------------------


// -----   Info to string   ------------------------------------------------
string CbmSetup::ToString() const {

  stringstream ss;
  CbmGeoSetup& setup = fProvider->GetSetup();
  ss << std::left << "CBM setup: " << setup.GetName() << ", " << GetNofModules()
     << " modules \n";
  for (auto& it : setup.GetModuleMap()) {
    ECbmModuleId moduleId     = it.first;
    CbmGeoSetupModule& module = it.second;
    ss << "       " << std::setw(8)
       << CbmModuleList::GetModuleNameCaps(moduleId) << ":  " << std::setw(8)
       << module.GetTag();
    if (module.GetActive())
      ss << "  *ACTIVE*  ";
    else
      ss << "            ";
    ss << " using " << module.GetFilePath() << "\n";
  }

  CbmGeoSetupField& field = setup.GetField();
  ss << "       Field   :  " << field.GetTag() << ", Position ( "
     << field.GetMatrix().GetTranslation()[0] << ", "
     << field.GetMatrix().GetTranslation()[1] << ", "
     << field.GetMatrix().GetTranslation()[2] << " ) cm, scaling "
     << field.GetScale() << "\n";

  return ss.str();
}
// -------------------------------------------------------------------------


// -----   Set the source the setup will be loaded from   -------------------
void CbmSetup::SetSetupSource(ECbmSetupSource setupSource) {
  switch (setupSource) {
    case kRepo: SetProvider(new CbmGeoSetupRepoProvider()); break;
    case kDb: SetProvider(new CbmGeoSetupDbProvider()); break;
    default:
      LOG(fatal) << "Invalid value for geo setup provider source "
                 << setupSource;
  }
}
// --------------------------------------------------------------------------


ClassImp(CbmSetup)
