/** @file CbmGeoSetupProvider.cxx
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#include "geosetup/CbmGeoSetupProvider.h"

#include "CbmCave.h"
#include "CbmDefs.h"
#include "CbmMagnet.h"
#include "CbmMuch.h"
#include "CbmMvd.h"
#include "CbmPipe.h"
#include "CbmRich.h"
#include "CbmStsMC.h"
#include "CbmTarget.h"
#include "CbmTof.h"
#include "CbmTrd.h"

#include "FairModule.h"
#include "FairRunSim.h"
#include <Logger.h>
//#include "CbmEcal.h"
#include "CbmPsdMC.h"
//#include "CbmShield.h"
#include "CbmPlatform.h"

#include <boost/algorithm/string.hpp>

ClassImp(CbmGeoSetupProvider);

namespace {  // anonymous namespace
  /// Module loading order is important for proper geometry initialization
  /// While detector modules might share the same level of hierarchy,
  /// cave should be the parent for all; beam pipe - for MVD
  std::vector<ECbmModuleId> GetModuleLoadingOrder() {
    return {
      ECbmModuleId::kCave,
      ECbmModuleId::kMagnet,
      ECbmModuleId::kPipe,
      ECbmModuleId::kTarget,
      //      ECbmModuleId::kMvd, ECbmModuleId::kSts, ECbmModuleId::kRich, ECbmModuleId::kMuch, ECbmModuleId::kTrd, ECbmModuleId::kTof, ECbmModuleId::kEcal, ECbmModuleId::kPsd,
      ECbmModuleId::kMvd,
      ECbmModuleId::kSts,
      ECbmModuleId::kRich,
      ECbmModuleId::kMuch,
      ECbmModuleId::kTrd,
      ECbmModuleId::kTof,
      ECbmModuleId::kPsd,
      //      ECbmModuleId::kHodo, ECbmModuleId::kShield, ECbmModuleId::kPlatform };
      ECbmModuleId::kHodo,
      ECbmModuleId::kPlatform};
  }
}  // end anonymous namespace


void CbmGeoSetupProvider::SetModuleTag(ECbmModuleId moduleId,
                                       std::string tag,
                                       Bool_t active) {
  CbmGeoSetupModule module = GetModuleByTag(moduleId, tag);
  module.SetActive(active);
  fSetup.GetModuleMap()[moduleId] = module;
}

void CbmGeoSetupProvider::RemoveModule(ECbmModuleId moduleId) {
  fSetup.GetModuleMap().erase(moduleId);
}

void CbmGeoSetupProvider::SetFieldTag(std::string tag) {
  CbmGeoSetupField field = GetFieldByTag(tag);

  // copy over the scale and position of the field previously set
  field.SetScale(fSetup.GetField().GetScale());
  field.SetMatrix(fSetup.GetField().GetMatrix());
  fSetup.SetField(field);
}

void CbmGeoSetupProvider::RegisterSetup() {
  if (!fSetup.GetModuleMap().size()) {
    LOG(error) << "-E- RegisterSetup: setup " << fSetup.GetName()
               << " is empty!";
    return;
  }

  // --- Get the FairRunSim instance
  FairRunSim* run = FairRunSim::Instance();
  if (!run) {
    LOG(error) << "-E- RegisterSetup: No FairRunSim instance!";
    return;
  }

  // register media
  run->SetMaterials(fSetup.GetMedia().GetFilePath().c_str());

  for (ECbmModuleId moduleId : GetModuleLoadingOrder()) {
    auto& moduleMap = fSetup.GetModuleMap();
    if (moduleMap.find(moduleId) == moduleMap.end()) continue;

    auto& geoModule      = fSetup.GetModuleMap()[moduleId];
    std::string fileName = geoModule.GetFilePath();

    Bool_t isActive       = geoModule.GetActive();
    std::string geoTag    = geoModule.GetTag();
    std::string modulName = geoModule.GetName();

    std::vector<std::string> _geom;
    std::vector<std::string> _tag;
    boost::split(_geom, fileName, [](char c) { return c == ':'; });
    boost::split(_tag, geoTag, [](char c) { return c == ':'; });
    int counter {0};
    for (auto& string : _geom) {

      LOG(info) << "-I- RegisterSetup: Registering " << modulName << " "
                << _tag[counter]
                << ((moduleId >= ECbmModuleId::kMvd
                     && moduleId <= ECbmModuleId::kPsd)
                      ? (isActive ? " -ACTIVE- " : " - INACTIVE- ")
                      : "")
                << " using " << string;

      FairModule* fairModule = NULL;
      switch (moduleId) {
        case ECbmModuleId::kCave: fairModule = new CbmCave("CAVE"); break;
        case ECbmModuleId::kMagnet: fairModule = new CbmMagnet("MAGNET"); break;
        case ECbmModuleId::kPipe: {
          std::string volname {"PIPE"};
          volname += std::to_string(counter);
          fairModule = new CbmPipe(volname.c_str());
          break;
        }
        case ECbmModuleId::kTarget: fairModule = new CbmTarget(); break;
        case ECbmModuleId::kMvd:
          fairModule = new CbmMvd("MVD", isActive);
          break;
        case ECbmModuleId::kSts: fairModule = new CbmStsMC(isActive); break;
        case ECbmModuleId::kRich:
          fairModule = new CbmRich("RICH", isActive);
          break;
        case ECbmModuleId::kMuch:
          fairModule = new CbmMuch("MUCH", isActive);
          break;
        case ECbmModuleId::kTrd:
          fairModule = new CbmTrd("TRD", isActive);
          break;
        case ECbmModuleId::kTof:
          fairModule = new CbmTof("TOF", isActive);
          break;
          //        case ECbmModuleId::kEcal:     fairModule = new CbmEcal("Ecal", isActive); break;
        case ECbmModuleId::kPsd:
          fairModule = new CbmPsdMC(isActive);
          break;
          //        case ECbmModuleId::kShield:   fairModule = new CbmShield("SHIELD"); break;
        case ECbmModuleId::kPlatform:
          fairModule = new CbmPlatform("PLATFORM");
          break;
        case ECbmModuleId::kHodo: fairModule = new CbmStsMC(isActive); break;
        default:
          LOG(error) << "-E- RegisterSetup: Unknown fairModule ID " << moduleId;
          break;
      }
      counter++;

      if (fairModule) {
        if (moduleId == ECbmModuleId::kMvd)
          fairModule->SetMotherVolume("pipevac1");
        fairModule->SetGeometryFileName(string.c_str());
        run->AddModule(fairModule);
      }
    }
  }
}

void CbmGeoSetupProvider::Reset() { fSetup = CbmGeoSetup(); }

CbmGeoSetupModule CbmGeoSetupProvider::GetDefaultCaveModule() {
  std::string geoFilePath =
    std::string(gSystem->Getenv("VMCWORKDIR")) + "/geometry/cave.geo";

  CbmGeoSetupModule module;
  module.SetName("CAVE");
  module.SetFilePath("cave.geo");
  module.SetTag("default");
  module.SetModuleId(ECbmModuleId::kCave);

  return module;
}
