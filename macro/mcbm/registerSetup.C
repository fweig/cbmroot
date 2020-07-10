/** Macro registerSetup
 **
 ** Creates the CBM modules defined in the CbmSetup singleton
 ** and registers them to the FairRunSim instance.
 **
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date  5 February 2016
 **/


void registerSetup() {

  // --- Get the setup singleton. Check whether it was defined (has at
  // --- least one module).
  CbmSetup* setup = CbmSetup::Instance();
  if (!setup->GetNofModules()) {
    std::cerr << "-E- registerSetup: setup " << setup->GetTitle()
              << " is empty!" << std::endl;
    return;
  }

  // --- Get the FairRunSim instance
  FairRunSim* run = FairRunSim::Instance();
  if (!run) {
    std::cout << "-E- registerSetup: No FairRunSim instance!" << std::endl;
    return;
  }

  // --- Common variables
  TString geoTag;
  TString fileName;
  Bool_t isActive;

  // --- Register cave
  std::cout << "-I- registerSetup: Registering CAVE" << std::endl;
  FairModule* cave = new CbmCave("CAVE");
  cave->SetGeometryFileName("cave_mcbm.geo");
  //	cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  // --- Register magnet
  if (setup->GetGeoFileName(kMagnet, fileName)) {
    setup->GetGeoTag(kMagnet, geoTag);
    std::cout << "-I- registerSetup: Registering MAGNET " << geoTag << " using "
              << fileName << std::endl;
    FairModule* magnet = new CbmMagnet("MAGNET");
    magnet->SetGeometryFileName(fileName.Data());
    run->AddModule(magnet);
  }

  // --- Register beam pipe
  if (setup->GetGeoFileName(kPipe, fileName)) {
    setup->GetGeoTag(kPipe, geoTag);
    std::cout << "-I- registerSetup: Registering PIPE " << geoTag << " using "
              << fileName << std::endl;
    FairModule* pipe = new CbmPipe("PIPE");
    pipe->SetGeometryFileName(fileName.Data());
    run->AddModule(pipe);
  }

  // --- Register other modules
  for (Int_t moduleId = kRef; moduleId < kLastModule; moduleId++) {
    // Magnet and pipe are already registered
    if (moduleId == kMagnet || moduleId == kPipe) continue;
    if (setup->GetGeoTag(moduleId, geoTag)) {
      setup->GetGeoFileName(moduleId, fileName);
      isActive = setup->IsActive(moduleId);
      std::cout << "-I- registerSetup: Registering "
                << CbmModuleList::GetModuleNameCaps(moduleId) << " " << geoTag
                << (isActive ? " -ACTIVE- " : " - INACTIVE- ") << " using "
                << fileName << std::endl;
      FairModule* module = NULL;
      switch (moduleId) {
        case kMvd: {
          module = new CbmMvd("MVD", isActive);
          module->SetMotherVolume("pipevac1");
          break;
        }
        case kSts: module = new CbmStsMC(isActive); break;
        case kRich: module = new CbmRich("RICH", isActive); break;
        case kMuch: module = new CbmMuch("MUCH", isActive); break;
        case kShield: module = new CbmShield("SHIELD"); break;
        case kTrd: module = new CbmTrd("TRD", isActive); break;
        case kTof:
          module = new CbmTof("TOF", isActive);
          break;
          //				case kEcal: module = new CbmEcal("Ecal", isActive); break;
        case kPsd: {
          CbmPsdv1* psd = new CbmPsdv1("PSD", isActive);
          psd->SetZposition(setup->GetPsdPositionZ());
          psd->SetXshift(setup->GetPsdPositionX());
          psd->SetGeoFile(fileName);
          module = (FairModule*) psd;
          break;
        }
        case kPlatform: module = new CbmPlatform("PLATFORM"); break;
        default:
          std::cout << "-E- registerSetup: Unknown module ID " << moduleId
                    << std::endl;
          break;
      }  //? known moduleId
      if (module) {
        if (moduleId != kPsd) module->SetGeometryFileName(fileName.Data());
        run->AddModule(module);
      }  //? valid module pointer
    }    //? module in setup
  }      //# moduleId
}
