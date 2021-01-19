/** @file CbmTransport.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 31.01.2019
 **/

#include "CbmTransport.h"

#include "TDatabasePDG.h"
#include "TG4RunConfiguration.h"
#include "TGeant3.h"
#include "TGeant3TGeo.h"
#include "TGeant4.h"
#include "TGeoManager.h"
#include "TPythia6Decayer.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TString.h"
#include "TSystem.h"
#include "TVector3.h"
#include "TVirtualMC.h"
#include <array>
#include <boost/filesystem.hpp>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

#include "FairLogger.h"
#include "FairMonitor.h"
#include "FairParRootFileIo.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "FairSystemInfo.h"
#include "FairUrqmdGenerator.h"

#include "CbmBeamProfile.h"
#include "CbmEventGenerator.h"
#include "CbmFieldMap.h"
#include "CbmFieldPar.h"
#include "CbmFileUtils.h"
#include "CbmPlutoGenerator.h"
#include "CbmSetup.h"
#include "CbmStack.h"
#include "CbmTarget.h"
#include "CbmUnigenGenerator.h"

#include "CbmGeant3Settings.h"
#include "CbmGeant4Settings.h"

using std::stringstream;


// -----   Constructor   ----------------------------------------------------
CbmTransport::CbmTransport()
  : TNamed("CbmTransport", "Transport Run")
  , fSetup(CbmSetup::Instance())
  , fField(nullptr)
  , fTarget()
  , fEventGen(new CbmEventGenerator())
  , fEventFilter(new CbmMCEventFilter())
  , fRun(new FairRunSim())
  , fOutFileName()
  , fParFileName()
  , fGeoFileName()
  , fGenerators()
  , fRealTimeInit(0.)
  , fRealTimeRun(0.)
  , fCpuTime(0.)
  , fEngine(kGeant3)
  , fStackFilter(new CbmStackFilter())
  , fGenerateRunInfo(kFALSE)
  , fStoreTrajectories(kFALSE) {
  // TODO: I do not like instantiating FairRunSim from this constructor;
  // It should be done in Run(). However, the presence of a FairRunSim
  // is required by CbmUnigenGenerator. Not a good construction; should
  // be done better.

  // Initialisation of the TDatabasePDG. This is done here because in the
  // course of the run, new particles may be added. The method
  // ReadPDGTable, however, is not executed from the constructor, but only
  // from GetParticle(), if the particle list is not empty.
  // So, if one adds particles before the first call to GetParticle(),
  // the particle table is never loaded.
  // TDatabasePDG is a singleton, but there is no way to check whether
  // it has already read the particle table file, nor to see if there are
  // any contents, nor to clean the particle list. A truly remarkable
  // implementation.
  auto pdgdb = TDatabasePDG::Instance();
  pdgdb->ReadPDGTable();

  // By default, vertex smearing along the beam is activated
  fEventGen->SmearVertexZ(kTRUE);
  fRun->SetGenerator(fEventGen);  // has to be available for some generators
}
// --------------------------------------------------------------------------


// -----   Destructor   -----------------------------------------------------
CbmTransport::~CbmTransport() {}
// --------------------------------------------------------------------------


// -----   Add a file-based input   -----------------------------------------
void CbmTransport::AddInput(const char* fileName, ECbmGenerator genType) {

  FairGenerator* generator = NULL;

  if (genType == kUrqmd) {
    if (gSystem->AccessPathName(fileName)) {
      LOG(fatal) << GetName() << ": Input file " << fileName << " not found!";
      return;
    }
  } else {
    if (!Cbm::File::IsRootFile(fileName)) {
      LOG(fatal) << GetName() << ": Input file " << fileName << " not found!";
      return;
    }
  }

  switch (genType) {
    case kUnigen: generator = new CbmUnigenGenerator(TString(fileName)); break;
    case kUrqmd: generator = new FairUrqmdGenerator(fileName); break;
    case kPluto: generator = new CbmPlutoGenerator(fileName); break;
  }

  assert(generator);
  fEventGen->AddGenerator(generator);
}
// --------------------------------------------------------------------------


// -----   Add a generator-based input   ------------------------------------
void CbmTransport::AddInput(FairGenerator* generator) {
  assert(generator);
  fEventGen->AddGenerator(generator);
}
// --------------------------------------------------------------------------


// -----   Configure the TVirtualMC   ---------------------------------------
void CbmTransport::ConfigureVMC() {

  std::cout << std::endl;
  LOG(info) << GetName() << ": Configuring VMC...";
  TVirtualMC* vmc = nullptr;

  if (fEngine == kGeant3) {
    TString* gModel = fRun->GetGeoModel();
    if (strncmp(gModel->Data(), "TGeo", 4) == 0) {
      LOG(info) << GetName() << ": Create TGeant3TGeo";
      vmc = new TGeant3TGeo("C++ Interface to Geant3 with TGeo");
    }  //? Geant3 with TGeo
    else {
      LOG(info) << GetName() << ": Create TGeant3";
      vmc = new TGeant3("C++ Interface to Geant3");
    }  //? Native Geant3
    if (!fGeant3Settings) { fGeant3Settings = new CbmGeant3Settings(); }
    fGeant3Settings->Init(vmc);
  }  //? Geant3

  else if (fEngine == kGeant4) {
    LOG(info) << GetName() << ": Create TGeant4";
    if (!fGeant4Settings) { fGeant4Settings = new CbmGeant4Settings(); }
    std::array<std::string, 3> runConfigSettings =
      fGeant4Settings->GetG4RunConfig();
    TG4RunConfiguration* runConfig = new TG4RunConfiguration(
      runConfigSettings[0], runConfigSettings[1], runConfigSettings[2]);
    vmc = new TGeant4("TGeant4", "C++ Interface to Geant4", runConfig);
    fGeant4Settings->Init(vmc);
  }  //? Geant4

  else
    LOG(fatal) << GetName() << ": unknown transport engine!";

  // Create stack
  std::unique_ptr<CbmStack> stack(new CbmStack());
  stack->SetFilter(fStackFilter);
  if (vmc) vmc->SetStack(stack.release());
}
// --------------------------------------------------------------------------


// -----   Force creation of event vertex at a given z position   -----------
void CbmTransport::ForceVertexAtZ(Double_t zVertex) {
  assert(fEventGen);
  fEventGen->ForceVertexAtZ(zVertex);
}
// --------------------------------------------------------------------------


// -----   Force creation of event vertex in the target   -------------------
void CbmTransport::ForceVertexInTarget(Bool_t choice) {
  assert(fEventGen);
  fEventGen->ForceVertexInTarget(choice);
}
// --------------------------------------------------------------------------


// -----   Force user-defined single-mode decays   --------------------------
void CbmTransport::ForceUserDecays() {

  assert(gMC);
  auto pdgdb = TDatabasePDG::Instance();

  // --- Setting user decays does not work with TGeant4
  if ((!fDecayModes.empty()) && fEngine == kGeant4)
    LOG(fatal) << GetName()
               << ": Forcing decay modes is not possible with TGeant4!";

  for (auto& decay : fDecayModes) {

    Int_t pdg         = decay.first;
    UInt_t nDaughters = decay.second.size();
    stringstream log;
    log << GetName() << ": Force decay " << pdgdb->GetParticle(pdg)->GetName()
        << " -> ";

    // First check whether VMC knows the particle. Not all particles
    // in TDatabasePDG are necessarily defined in VMC.
    // This check is there because the call to TVirtualMC::SetUserDecay
    // has no return value signifying success. If the particle is not
    // found, just an error message is printed, which most likely
    // goes unnoticed by the user.
    // The access to ParticleMCTpye seems to me the only way to check.
    // No method like Bool_t CheckParticle(Int_t) is there, which any
    // sensible programmer would have put.
    if (gMC->ParticleMCType(pdg)
        == kPTUndefined) {  // At least TGeant3 delivers that
      LOG(info) << log.str();
      LOG(fatal) << GetName() << ": PDG " << pdg << " not in VMC!";
      continue;
    }

    // For up to three daughters, the native decayer is used
    if (nDaughters <= 3) {
      Float_t branch[6] = {100., 0., 0., 0., 0., 0.};  // branching ratios
      Int_t mode[6][3]  = {{0, 0, 0},
                          {0, 0, 0},
                          {0, 0, 0},
                          {0, 0, 0},
                          {0, 0, 0},
                          {0, 0, 0}};  // decay modes
      for (UInt_t iDaughter = 0; iDaughter < nDaughters; iDaughter++) {
        mode[0][iDaughter] = decay.second[iDaughter];
        log << pdgdb->GetParticle(decay.second[iDaughter])->GetName() << " ";
      }
      Bool_t success = gMC->SetDecayMode(pdg, branch, mode);
      if (!success) {
        LOG(info) << log.str();
        LOG(fatal) << GetName() << ": Setting decay mode failed!";
      }
      log << ", using native decayer.";
    }  //? not more than three daughters

    // For more than three daughters, we must use TPythia6 as external decayer
    else {
      auto p6decayer = TPythia6Decayer::Instance();
      Int_t daughterPdg[nDaughters];
      Int_t multiplicity[nDaughters];
      for (UInt_t iDaughter = 0; iDaughter < nDaughters; iDaughter++) {
        daughterPdg[iDaughter]  = decay.second[iDaughter];
        multiplicity[iDaughter] = 1;
        log << pdgdb->GetParticle(decay.second[iDaughter])->GetName() << " ";
      }  //# daughters
      p6decayer->ForceParticleDecay(pdg, daughterPdg, multiplicity, nDaughters);
      // We have to tell the VMC to use the Pythia decayer for this particle
      gMC->SetUserDecay(pdg);
    }  //? more than three daughters

    LOG(info) << log.str();
  }  //# user-defined decay modes
}
// --------------------------------------------------------------------------

// -----   Initialisation of event generator   ------------------------------
void CbmTransport::InitEventGenerator() {

  // --- Set the target properties to the event generator
  fEventGen->SetTarget(fTarget);

  // --- Log output
  std::cout << std::endl;
  LOG(info) << "-----   Settings for event generator";
  fEventGen->Print();
  LOG(info) << "-----   End settings for event generator";
  std::cout << std::endl;

  // --- If a target is specified, check its consistency with the beam
  // --- profile. The average beam must hit the target surface.
  if (fTarget) {
    if (!fEventGen->GetBeamProfile().CheckWithTarget(*fTarget)) {
      LOG(fatal) << GetName()
                 << ": Beam profile is not consistent with target!";
    }  //? Target not consistent with beam
  }    //? Target specified
}
// --------------------------------------------------------------------------


// -----   Set the source the setup will be loaded from   -------------------
void CbmTransport::SetSetupSource(ECbmSetupSource setupSource) {
  fSetup->SetSetupSource(setupSource);
}
// --------------------------------------------------------------------------


// -----   Load a standard setup   ------------------------------------------
void CbmTransport::LoadSetup(const char* setupName) {
  fSetup->LoadSetup(setupName);
}
// --------------------------------------------------------------------------


// -----   Register ions to the TDatabsePDG   -------------------------------
void CbmTransport::RegisterIons() {

  // TODO: Better would be loading the additional particles from a text file.
  // TDatabasePDG reads the particle definitions from pdg_table.txt
  // (in $SIMPATH/share/root/etc). The method TDatabase::ReadPDGTable()
  // is triggered on first call to TDatabasePDG::GetParticle(Int_t), if the
  // database is still empty.
  // We could call TDatabasePDG::ReadPDGTable(name_of_cbm_file) after the
  // first initialisation of the database; the there defined particles would
  // be added on top of the existing ones.

  // Particle database and variables
  TDatabasePDG* pdgdb = TDatabasePDG::Instance();
  const char* name    = "";
  Int_t code          = 0;
  Double_t mass       = 0.;
  Bool_t stable       = kTRUE;
  Double_t charge     = 0.;

  // --- deuteron and anti-deuteron
  name   = "d+";
  code   = 1000010020;
  mass   = 1.876124;
  stable = kTRUE;
  charge = 1.;
  pdgdb->AddParticle(name, name, mass, stable, 0., charge, "Ion", code);
  pdgdb->AddAntiParticle("d-", -1 * code);

  // --- tritium and anti-tritium
  name   = "t+";
  code   = 1000010030;
  mass   = 2.809432;
  stable = kTRUE;
  charge = 1.;
  pdgdb->AddParticle(name, name, mass, stable, 0., charge, "Ion", code);
  pdgdb->AddAntiParticle("t-", -1 * code);

  // --- Helium_3 and its anti-nucleus
  name   = "He3+";
  code   = 1000020030;
  mass   = 2.809413;
  stable = kTRUE;
  charge = 2.;
  pdgdb->AddParticle(name, name, mass, stable, 0., charge, "Ion", code);
  pdgdb->AddAntiParticle("He3-", -1 * code);

  // --- Helium_4 and its anti-nucleus
  name   = "He4+";
  code   = 1000020040;
  mass   = 3.7284;
  stable = kTRUE;
  charge = 2.;
  pdgdb->AddParticle(name, name, mass, stable, 0., charge, "Ion", code);
  pdgdb->AddAntiParticle("He3-", -1 * code);
}
// --------------------------------------------------------------------------


// -----   Register radiation length   --------------------------------------
void CbmTransport::RegisterRadLength(Bool_t choice) {
  assert(fRun);
  fRun->SetRadLenRegister(choice);
  LOG(info) << GetName() << ": Radiation length register is enabled";
}
// --------------------------------------------------------------------------


// -----   Create and register the setup modules   --------------------------
void CbmTransport::RegisterSetup() { fSetup->RegisterSetup(); }
// --------------------------------------------------------------------------


// -----   Set correct decay modes for pi0 and eta   ------------------------
void CbmTransport::PiAndEtaDecay(TVirtualMC* vmc) {

  assert(vmc);
  LOG(info) << GetName() << ": Set decay modes for pi0 and eta";

  if (fEngine == kGeant4) {
    std::cout << std::endl << std::endl;
    LOG(warn) << "***********************************";
    LOG(warn) << "***********************************";
    LOG(warn) << GetName() << ": User decay modes cannot be set with TGeant4!";
    LOG(warn) << GetName()
              << ": Built-in decay modes for pi0 and eta will be used!";
    LOG(warn) << "***********************************";
    LOG(warn) << "***********************************";
    std::cout << std::endl << std::endl;
    return;
  }

  TGeant3* gMC3 = static_cast<TGeant3*>(vmc);
  assert(vmc);

  // Decay modes for eta mesons (PDG 2016)
  Int_t modeEta[6];  // decay modes
  Float_t brEta[6];  // branching ratios in %

  // --- eta -> gamma gamma
  modeEta[0] = 101;
  brEta[0]   = 39.41;

  // --- eta -> pi0 pi0 pi0
  modeEta[1] = 70707;
  brEta[1]   = 32.68;

  // --- eta -> pi+ pi- pi0
  modeEta[2] = 80907;
  brEta[2]   = 22.92;

  // --- eta -> pi+ pi- gamma
  modeEta[3] = 80901;
  brEta[3]   = 4.22;

  // --- eta -> e+ e- gamma
  modeEta[4] = 30201;
  brEta[4]   = 0.69;

  // --- eta -> pi0 gamma gamma
  modeEta[5] = 10107;
  brEta[5]   = 2.56e-2;

  // --- Set the eta decays
  gMC3->Gsdk(17, brEta, modeEta);

  // --- Decay modes for pi0
  Int_t modePi[6];  // decay modes
  Float_t brPi[6];  // branching ratios in %

  // --- pi0 -> gamma gamma
  modePi[0] = 101;
  brPi[0]   = 98.823;

  // --- pi0 -> e+ e- gamma
  modePi[1] = 30201;
  brPi[1]   = 1.174;

  // --- No other channels for pi0
  for (Int_t iMode = 2; iMode < 6; iMode++) {
    modePi[iMode] = 0;
    brPi[iMode]   = 0.;
  }

  // --- Set the pi0 decays
  gMC3->Gsdk(7, brPi, modePi);
}
// --------------------------------------------------------------------------


// -----   Execute transport run   ------------------------------------------
void CbmTransport::Run(Int_t nEvents) {

  // --- Timer
  TStopwatch timer;

  // --- Check presence of required requisites
  if (fOutFileName.IsNull())
    LOG(fatal) << GetName() << ": No output file specified!";
  if (fParFileName.IsNull())
    LOG(fatal) << GetName() << ": No parameter file specified!";
  std::cout << std::endl << std::endl;


  // --- Add some required particles to the TDatabasePDG
  RegisterIons();


  // --- Set transport engine
  const char* engineName = "";
  switch (fEngine) {
    case kGeant3: engineName = "TGeant3"; break;
    case kGeant4: engineName = "TGeant4"; break;
    default: {
      LOG(warn) << GetName() << ": Unknown transport engine ";
      engineName = "TGeant3";
      break;
    }
  }  //? engine
  LOG(info) << GetName() << ": Using engine " << engineName;
  fRun->SetName(engineName);


  // --- Set output file name
  fRun->SetOutputFile(fOutFileName);

  // --- Create and register the setup modules, field and media with FairRoot
  RegisterSetup();

  // --- Create and register the target
  if (fTarget) {
    LOG(info) << fTarget->ToString();
    fRun->AddModule(fTarget.get());
  } else
    LOG(warn) << GetName() << ": No target defined!";


  // --- Create the magnetic field
  LOG(info) << GetName() << ": Register magnetic field";
  LOG(info) << fField;
  if (!fField) fField = fSetup->CreateFieldMap();
  fField->Print("");
  fRun->SetField(fField);


  // --- Initialise the event generator
  InitEventGenerator();


  // --- Trigger generation of run info
  fRun->SetGenerateRunInfo(fGenerateRunInfo);


  // --- Trigger storage of trajectories, if chosen
  fRun->SetStoreTraj(fStoreTrajectories);


  // --- Set VMC configuration
  std::function<void()> f = std::bind(&CbmTransport::ConfigureVMC, this);
  fRun->SetSimSetup(f);


  // --- Set event filter task
  fRun->AddTask(fEventFilter.get());


  // --- Initialise run
  fRun->Init();


  // --- Force user-defined decays. This has to happen after FairRunSim::Init()
  // because otherwise there seem to be no particles in GEANT.
  ForceUserDecays();


  // --- Set correct decay modes for pi0 and eta
  // --- This is needed only when using Geant3
  if (fEngine == kGeant3) PiAndEtaDecay(gMC);


  // --- Runtime database
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  CbmFieldPar* fieldPar =
    static_cast<CbmFieldPar*>(rtdb->getContainer("CbmFieldPar"));
  fieldPar->SetParameters(fField);
  fieldPar->setChanged();
  FairParRootFileIo* parOut = new FairParRootFileIo(kTRUE);
  parOut->open(fParFileName.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();


  // --- Measure time for initialisation
  timer.Stop();
  fRealTimeInit = timer.RealTime();

  TFile* old         = gFile;
  TDirectory* oldDir = gDirectory;

  // Write Transport Settings to the output file
  TFile* outfile = fRun->GetOutputFile();
  outfile->cd();

  LOG(info) << "Here I am";
  if (fEngine == kGeant3) {
    LOG(info) << "Write Geant3Settings";
    fGeant3Settings->Write();
  } else if (fEngine == kGeant4) {
    LOG(info) << "Write Geant4Settings";
    fGeant4Settings->Write();
  }
  gFile      = old;
  gDirectory = oldDir;

  // --- Start run
  timer.Start(kFALSE);  // without reset
  fRun->Run(nEvents);
  timer.Stop();
  fRealTimeRun = timer.RealTime() - fRealTimeInit;
  fCpuTime     = timer.CpuTime();

  // --- Create a geometry file if required
  if (!fGeoFileName.IsNull()) fRun->CreateGeometryFile(fGeoFileName);


  // --- Screen log
  std::cout << std::endl;
  LOG(info) << GetName() << ": Run finished successfully.";
  LOG(info) << GetName() << ": Wall time for Init : " << fRealTimeInit << " s ";
  LOG(info) << GetName() << ": Wall time for Run  : " << fRealTimeRun << " s ("
            << fRealTimeRun / fEventFilter->GetNofInputEvents()
            << " s / event)";
  LOG(info) << GetName() << ": Output file    : " << fOutFileName;
  LOG(info) << GetName() << ": Parameter file : " << fParFileName;
  if (!fGeoFileName.IsNull())
    LOG(info) << GetName() << ": Geometry file  : " << fGeoFileName;
  std::cout << std::endl << std::endl;


  // --- Remove TGeoManager
  // To avoid crashes when exiting. Reason for this behaviour is unknown.
  if (gGeoManager) {
    if (gROOT->GetVersionInt() >= 60602) {
      gGeoManager->GetListOfVolumes()->Delete();
      gGeoManager->GetListOfShapes()->Delete();
    }
    delete gGeoManager;
  }
}
// --------------------------------------------------------------------------


// -----   Set the beam angle distribution   --------------------------------
void CbmTransport::SetBeamAngle(Double_t x0,
                                Double_t y0,
                                Double_t sigmaX,
                                Double_t sigmaY) {
  assert(fEventGen);
  fEventGen->SetBeamAngle(x0, y0, sigmaX, sigmaY);
}
// --------------------------------------------------------------------------


// -----   Set the beam position   ------------------------------------------
void CbmTransport::SetBeamPosition(Double_t x0,
                                   Double_t y0,
                                   Double_t sigmaX,
                                   Double_t sigmaY,
                                   Double_t z) {
  assert(fEventGen);
  fEventGen->SetBeamPosition(x0, y0, sigmaX, sigmaY, z);
}
// --------------------------------------------------------------------------


// -----   Set a decay mode   -----------------------------------------------
void CbmTransport::SetDecayMode(Int_t pdg,
                                UInt_t nDaughters,
                                Int_t* daughterPdg) {

  if (fDecayModes.count(pdg) != 0) {
    LOG(fatal) << GetName() << ": User decay mode for PDG " << pdg
               << " is already defined!";
    return;
  }

  for (UInt_t iDaughter = 0; iDaughter < nDaughters; iDaughter++) {
    fDecayModes[pdg].push_back(daughterPdg[iDaughter]);
  }
}
// --------------------------------------------------------------------------


// -----   Set geometry file name   -----------------------------------------
void CbmTransport::SetGeoFileName(TString fileName) {

  // Check for the directory
  std::string name = fileName.Data();
  Int_t found      = name.find_last_of("/");
  if (found >= 0) {
    TString geoDir = name.substr(0, found);
    if (gSystem->AccessPathName(geoDir.Data())) {
      LOG(error) << GetName() << ": Directory for geometry file " << geoDir
                 << " does not exist; the file will not be created.";
      return;
    }  //? Directory of geometry file does not exist
  }    //? File name contains directory path

  fGeoFileName = fileName;
}
// --------------------------------------------------------------------------


// -----   Set parameter file name   ----------------------------------------
void CbmTransport::SetParFileName(TString fileName) {

  // --- If file does not exist, check the directory
  if (gSystem->AccessPathName(fileName)) {
    std::string name = fileName.Data();
    Int_t found      = name.find_last_of("/");
    if (found >= 0) {
      TString parDir = name.substr(0, found);
      if (gSystem->AccessPathName(parDir.Data())) {
        LOG(fatal) << GetName() << ": Parameter directory " << parDir
                   << " does not exist!";
        return;
      }  //? Directory of parameter file does not exist
    }    //? File name contains directory path
  }      //? Parameter file does not exist

  fParFileName = fileName;
}
// --------------------------------------------------------------------------


// -----   Set random event plane generation   ------------------------------
void CbmTransport::SetRandomEventPlane(Double_t phiMin, Double_t phiMax) {
  fEventGen->SetEventPlane(phiMin, phiMax);
}
// --------------------------------------------------------------------------


// -----   Set output file name   -------------------------------------------
void CbmTransport::SetOutFileName(TString fileName) {

  // Check for the directory
  std::string name = fileName.Data();
  Int_t found      = name.find_last_of("/");
  if (found >= 0) {
    TString outDir = name.substr(0, found);
    if (gSystem->AccessPathName(outDir.Data())) {
      LOG(fatal) << GetName() << ": Output directory " << outDir
                 << " does not exist!";
      return;
    }  //? Directory of output file does not exist
  }    //? File name contains directory path

  fOutFileName = fileName;
}
// --------------------------------------------------------------------------


// -----   Define the target   ----------------------------------------------
void CbmTransport::SetTarget(const char* medium,
                             Double_t thickness,
                             Double_t diameter,
                             Double_t x,
                             Double_t y,
                             Double_t z,
                             Double_t angle) {

  fTarget.reset(new CbmTarget(medium, thickness, diameter));
  fTarget->SetPosition(x, y, z);
  fTarget->SetRotation(angle);
}
// --------------------------------------------------------------------------


// -----   Enable vertex distribution in x and y   --------------------------
void CbmTransport::SetVertexSmearXY(Bool_t choice) {
  assert(fEventGen);
  fEventGen->SmearGausVertexXY(choice);
}
// --------------------------------------------------------------------------


// -----   Enable vertex distribution z   -----------------------------------
void CbmTransport::SetVertexSmearZ(Bool_t choice) {
  assert(fEventGen);
  fEventGen->SmearVertexZ(choice);
}
// --------------------------------------------------------------------------

ClassImp(CbmTransport);
