/* Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

/** Macro registerSetup
 **
 ** Creates the CBM modules defined in the CbmSetup singleton
 ** and registers them to the FairRunSim instance.
 **
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date  5 February 2016
 **/

void registerSetup()
{

  // --- Get the setup singleton. Check whether it was defined (has at
  // --- least one module).
  CbmSetup* setup = CbmSetup::Instance();

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
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  // --- Register magnet
  if (setup->GetGeoFileName(ECbmModuleId::kMagnet, fileName)) {
    setup->GetGeoTag(ECbmModuleId::kMagnet, geoTag);
    std::cout << "-I- registerSetup: Registering MAGNET " << geoTag << " using " << fileName << std::endl;
    FairModule* magnet = new CbmMagnet("MAGNET");
    magnet->SetGeometryFileName(fileName.Data());
    run->AddModule(magnet);
  }

  // --- Register beam pipe
  if (setup->GetGeoFileName(ECbmModuleId::kPipe, fileName)) {

    setup->GetGeoTag(ECbmModuleId::kPipe, geoTag);
    std::cout << "-I- registerSetup: Registering PIPE " << geoTag << " using " << fileName << std::endl;

    //  handle more than 1 beampipe;
    FairModule* pipe;
    int pipe_counter = 0;
    char *file, *ptr;
    file = (char*) malloc(500 * sizeof(char));
    strncpy(file, fileName.Data(), 500);

    do {
      ptr  = strchr(file, ':');
      pipe = new CbmPipe();
      if (ptr) *ptr = '\0';
      pipe->SetGeometryFileName(file);
      std::cout << "Beampipe " << pipe_counter << " is " << file << std::endl;
      pipe->SetTitle(("pipe" + std::to_string(pipe_counter)).c_str());
      run->AddModule(pipe);
      pipe_counter++;
      file = (ptr + 1);
    } while (ptr);
  }

  // --- Register other modules
  for (ECbmModuleId moduleId = ECbmModuleId::kRef; moduleId < ECbmModuleId::kLastModule; ++moduleId) {
    // Magnet and pipe are already registered
    if (moduleId == ECbmModuleId::kMagnet || moduleId == ECbmModuleId::kPipe) continue;
    if (setup->GetGeoTag(moduleId, geoTag)) {
      setup->GetGeoFileName(moduleId, fileName);
      isActive = setup->IsActive(moduleId);
      std::cout << "-I- registerSetup: Registering " << CbmModuleList::GetModuleNameCaps(moduleId) << " " << geoTag
                << (isActive ? " -ACTIVE- " : " - INACTIVE- ") << " using " << fileName << std::endl;
      FairModule* module = NULL;
      switch (moduleId) {
        case ECbmModuleId::kMvd: {
          module = new CbmMvd("MVD", isActive);
          module->SetMotherVolume("pipevac1");
          break;
        }
        case ECbmModuleId::kSts: module = new CbmStsMC(isActive); break;
        case ECbmModuleId::kRich: module = new CbmRich("RICH", isActive); break;
        case ECbmModuleId::kMuch:
          module = new CbmMuch("MUCH", isActive);
          break;
          //				case ECbmModuleId::kShield: module = new CbmShield("SHIELD"); break;
        case ECbmModuleId::kTrd: module = new CbmTrd("TRD", isActive); break;
        case ECbmModuleId::kTof:
          module = new CbmTof("TOF", isActive);
          break;
          //				case ECbmModuleId::kEcal: module = new CbmEcal("Ecal", isActive); break;
        case ECbmModuleId::kPsd: module = new CbmPsdMC(isActive); break;
        case ECbmModuleId::kPlatform: module = new CbmPlatform("PLATFORM"); break;
        default: std::cout << "-E- registerSetup: Unknown module ID " << moduleId << std::endl; break;
      }  //? known moduleId
      if (module) {
        module->SetGeometryFileName(fileName.Data());
        run->AddModule(module);
      }  //? valid module pointer
    }    //? module in setup
  }      //# moduleId
}
