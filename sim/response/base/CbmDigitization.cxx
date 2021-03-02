/** @file CbmDigitization.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 18.05.2018
 **/

#include "CbmDigitization.h"

#include "CbmDigitizationSource.h"
#include "CbmMuchDigitizeGem.h"
#include "CbmMvdDigitizer.h"
#include "CbmPsdSimpleDigitizer.h"
#include "CbmRichDigitizer.h"
#include "CbmRunAna.h"
#include "CbmSetup.h"
#include "CbmStsDigitize.h"
#include "CbmTofDigitize.h"
#include "CbmTrdDigitizer.h"
#include "FairFileSource.h"
#include "FairLogger.h"
#include "FairMCEventHeader.h"
#include "FairMonitor.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TROOT.h"
#include <cassert>


// -----   Constructor   ----------------------------------------------------
CbmDigitization::CbmDigitization()
  : TNamed("CbmDigitization", "Digitisation Run")
  , fIsInit(kFALSE)
  , fEventMode(kFALSE)
  , fTimeSliceLength(-1.)
  , fProduceNoise(kTRUE)
  , fCreateMatches(kTRUE)
  , fDigitizers()
  , fDaq(new CbmDaq())
  , fSource(new CbmDigitizationSource())
  , fOutFile()
  , fParRootFile()
  , fParAsciiFiles()
  , fMoniFile()
  , fOverwriteOutput(kFALSE)
  , fGenerateRunInfo(kTRUE)
  , fRun(0) {
  SetDefaultBranches();
}
// --------------------------------------------------------------------------


// -----   Destructor   -----------------------------------------------------
CbmDigitization::~CbmDigitization() {
  LOG(debug) << "Destructing " << fName;
  for (auto it = fDigitizers.begin(); it != fDigitizers.end(); it++) {
    if (it->second) delete it->second;
  }  //# CbmDigitizeInfos
  // CbmDaq and the digitizers are destructed by FairRun.
  if (fSource) delete fSource;
}
// --------------------------------------------------------------------------


// -----   Add an input file   ----------------------------------------------
void CbmDigitization::AddInput(UInt_t inputId,
                               TString fileName,
                               Double_t eventRate,
                               ECbmTreeAccess mode) {
  if (gSystem->AccessPathName(fileName))
    LOG(fatal) << fName << ": input file " << fileName << " does not exist!";
  TChain* chain = new TChain("cbmsim");
  chain->Add(fileName.Data());
  fSource->AddInput(inputId, chain, eventRate, mode);
}
// --------------------------------------------------------------------------


// -----   Add an ASCII parameter file   ------------------------------------
Bool_t CbmDigitization::AddParameterAsciiFile(TString fileName) {
  if (gSystem->AccessPathName(fileName.Data())) {
    LOG(error) << fName << ": Parameter file " << fileName
               << " does not exist!";
    return kFALSE;
  }
  fParAsciiFiles.Add(new TObjString(fileName.Data()));
  LOG(info) << fName << ": Adding parameter file " << fileName;
  return kTRUE;
}
// --------------------------------------------------------------------------


// -----   Check input file   -----------------------------------------------
Int_t CbmDigitization::CheckInput() {

  // --- Check presence of input data branch for the digitizers.
  // --- If the branch is not found, the digitizer will not be instantiated.
  Int_t nBranches = 0;
  for (auto const& entry : fDigitizers) {
    auto setIt = fSource->GetBranchList().find(entry.second->GetBranchName());
    if (setIt != fSource->GetBranchList().end()) {
      LOG(info) << fName << ": Found branch " << entry.second->GetBranchName()
                << " for system "
                << CbmModuleList::GetModuleNameCaps(entry.first);
      entry.second->SetPresent();
      nBranches++;
    }  //? Branch required by digitizer is present in branch list
  }

  // Now we have to do some gymnastics to get the run ID, which is needed
  // to determine the geometry tags, which in turn are needed to register
  // the proper ASCII parameter files. This is rather nasty; the parameter
  // handling is really a pain in the neck.
  CbmMCInput* input = fSource->GetFirstInput();
  assert(input);
  TFile* file = input->GetChain()->GetFile();
  assert(file);
  TTree* tree = dynamic_cast<TTree*>(file->Get("cbmsim"));
  assert(tree);
  FairMCEventHeader* header = new FairMCEventHeader();
  tree->SetBranchAddress("MCEventHeader.", &header);
  tree->GetEntry(0);
  fRun = header->GetRunID();
  LOG(info) << fName << ": Run id is " << fRun;

  return nBranches;
}
// --------------------------------------------------------------------------


// -----   Create the digitisers   ------------------------------------------
Int_t CbmDigitization::CreateDefaultDigitizers() {
  std::cout << "Create default digitisers" << std::endl;

  stringstream ss;

  ss << fName << ": Create default digitisers: ";
  Int_t nDigis = 0;
  for (auto it = fDigitizers.begin(); it != fDigitizers.end(); it++) {

    // --- Skip if marked inactive
    if (!it->second->IsActive()) continue;

    // --- Skip if MC data branch is not present
    if (!it->second->IsPresent()) continue;

    // --- Skip if a digitizer was set explicitly
    if (it->second->GetDigitizer() != nullptr) continue;

    // --- Skip MVD for time-based mode
    if (it->first == ECbmModuleId::kMvd && (!fEventMode)) {
      LOG(info) << "MVD digitizer is not available "
                << "in time-based mode. ";
      continue;
    }
    LOG(info) << "system " << it->first;

    ECbmModuleId system = it->first;
    switch (system) {
      case ECbmModuleId::kMvd:
        fDigitizers[system]->SetDigitizer(new CbmMvdDigitizer());
        ss << "MVD ";
        nDigis++;
        break;
      case ECbmModuleId::kSts:
        fDigitizers[system]->SetDigitizer(new CbmStsDigitize());
        ss << "STS ";
        nDigis++;
        break;
      case ECbmModuleId::kRich:
        fDigitizers[system]->SetDigitizer(new CbmRichDigitizer());
        ss << "RICH ";
        nDigis++;
        break;
      case ECbmModuleId::kMuch:
        fDigitizers[system]->SetDigitizer(new CbmMuchDigitizeGem());
        ss << "MUCH ";
        nDigis++;
        break;
      case ECbmModuleId::kTrd:
        fDigitizers[system]->SetDigitizer(new CbmTrdDigitizer());
        ss << "TRD ";
        nDigis++;
        break;
      case ECbmModuleId::kTof:
        fDigitizers[system]->SetDigitizer(new CbmTofDigitize());
        ss << "TOF ";
        nDigis++;
        break;
      case ECbmModuleId::kPsd:
        fDigitizers[system]->SetDigitizer(new CbmPsdSimpleDigitizer());
        ss << "PSD ";
        nDigis++;
        break;
      default: LOG(fatal) << fName << ": Unknown system " << system; break;
    }  //? system
  }    //# present systems
  LOG(info) << ss.str();

  return nDigis;
}
// --------------------------------------------------------------------------


// -----   Deactivate a system   --------------------------------------------
void CbmDigitization::Deactivate(ECbmModuleId system) {
  if (fDigitizers.find(system) != fDigitizers.end())
    fDigitizers[system]->SetActive(kFALSE);
}
// --------------------------------------------------------------------------


// -----   Deactivate all systems except one   ------------------------------
void CbmDigitization::DeactivateAllBut(ECbmModuleId system) {
  for (auto& entry : fDigitizers)
    entry.second->SetActive(kFALSE);
  if (fDigitizers.find(system) != fDigitizers.end())
    fDigitizers[system]->SetActive(kTRUE);
}
// --------------------------------------------------------------------------


// -----   Embed an input file   --------------------------------------------
void CbmDigitization::EmbedInput(UInt_t inputId,
                                 TString fileName,
                                 UInt_t targetInputId,
                                 ECbmTreeAccess mode) {
  if (gSystem->AccessPathName(fileName))
    LOG(fatal) << fName << ": input file " << fileName << " does not exist!";
  TChain* chain = new TChain("cbmsim");
  chain->Add(fileName.Data());
  fSource->EmbedInput(inputId, chain, targetInputId, mode);
}
// --------------------------------------------------------------------------


// -----   Get a system geometry tag   --------------------------------------
TString CbmDigitization::GetGeoTag(ECbmModuleId system, TGeoManager* geo) {

  assert(geo);
  TString geoTag;
  TString sysName = CbmModuleList::GetModuleName(system);
  Int_t sysLength = sysName.Length() + 1;
  gGeoManager->CdTop();
  TGeoNode* cave = gGeoManager->GetCurrentNode();  // cave
  for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
    TString volName = cave->GetDaughter(iNode)->GetVolume()->GetName();
    if (volName.Contains(sysName.Data(), TString::kIgnoreCase)) {
      geoTag = TString(volName(sysLength, volName.Length() - sysLength));
      break;
    }  //? node is MUCH
  }    //# top level nodes

  return geoTag;
}
// --------------------------------------------------------------------------


// -----   Get a system geometry tag   --------------------------------------
void CbmDigitization::DefaultInit() {

  // --- Run this method only once!
  if (fIsInit) return;

  std::cout << std::endl << std::endl;
  LOG(info) << "===================================================";
  LOG(info) << "========== Initialize with default values =========";
  // --- Look for input branches
  Int_t nBranches = CheckInput();
  TString word    = (nBranches == 1 ? "branch" : "branches");
  LOG(info) << fName << ": " << nBranches << " input " << word << " found";


  // --- Create default digitizers
  Int_t nDigis = CreateDefaultDigitizers();
  word         = (nDigis == 1 ? " digitiser" : " digitisers");
  LOG(info) << fName << ": " << nDigis << word << " instantiated.";


  // --- Extract needed information from runtime database
  FairRuntimeDb* rtdb          = FairRuntimeDb::instance();
  FairParRootFileIo* parIoRoot = new FairParRootFileIo();
  parIoRoot->open(fParRootFile.Data(), "READ");
  rtdb->setFirstInput(parIoRoot);


  // --- Get geometry from runtime database
  rtdb->getContainer("FairGeoParSet");
  rtdb->initContainers(fRun);

  // --- Add default parameter files for TRD and TOF
  TString tofGeo = GetGeoTag(ECbmModuleId::kTof, gGeoManager);
  TString trdGeo = GetGeoTag(ECbmModuleId::kTrd, gGeoManager);
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString parFile;
  if (trdGeo.Length() > 0) {
    parFile = srcDir + "/parameters/trd/trd_" + trdGeo + ".asic.par";
    AddParameterAsciiFile(parFile);
    parFile = srcDir + "/parameters/trd/trd_" + trdGeo + ".digi.par";
    AddParameterAsciiFile(parFile);
    parFile = srcDir + "/parameters/trd/trd_" + trdGeo + ".gain.par";
    AddParameterAsciiFile(parFile);
    parFile = srcDir + "/parameters/trd/trd_" + trdGeo + ".gas.par";
    AddParameterAsciiFile(parFile);
  }
  if (tofGeo.Length() > 0) {
    parFile = srcDir + "/parameters/tof/tof_" + tofGeo + ".digibdf.par";
    AddParameterAsciiFile(parFile);
  }

  delete rtdb;
  delete parIoRoot;


  // --- Delete TGeoManager (will be initialised again from FairRunAna)
  if (gROOT->GetVersionInt() >= 60602) {
    gGeoManager->GetListOfVolumes()->Delete();
    gGeoManager->GetListOfShapes()->Delete();
    delete gGeoManager;
  }  //? ROOT version


  std::cout << std::endl << std::endl;
  LOG(info) << "===================================================";

  fIsInit = kTRUE;
}
// --------------------------------------------------------------------------


// -----   Execute digitisation run   ---------------------------------------
void CbmDigitization::Run(Int_t event1, Int_t event2) {

  // --- Run info
  std::cout << std::endl << std::endl;
  LOG(info) << "===================================================";

  DefaultInit();

  // --- Create CbmRunAna
  std::cout << std::endl;
  CbmRunAna* run = new CbmRunAna();
  run->SetAsync();
  run->SetGenerateRunInfo(fGenerateRunInfo);
  if (fGenerateRunInfo) LOG(info) << fName << ": Run info will be generated.";

  // --- Create DAQ
  if (fEventMode)
    fDaq = new CbmDaq(kTRUE);
  else
    fDaq = new CbmDaq(fTimeSliceLength);


  // --- Register source
  if (fEventMode) fSource->SetEventMode();
  run->SetSource(fSource);


  // --- Set output file
  run->SetOutputFile(fOutFile);
  LOG(info) << fName << ": Output file is " << fOutFile;


  // --- Set monitoring, if chosen
  if (!fMoniFile.IsNull()) {
    FairMonitor::GetMonitor()->EnableMonitor(kTRUE, fMoniFile);
    LOG(info) << fName << ": Monitor is enabled; monitor file is " << fMoniFile;
  }


  // --- Register digitisers
  for (auto it = fDigitizers.begin(); it != fDigitizers.end(); it++) {
    CbmDigitizeBase* digitizer = it->second->GetDigitizer();
    if (it->second->IsActive() && digitizer != nullptr) {
      fDaq->SetDigitizer(it->first, digitizer);
      if (fEventMode) digitizer->SetEventMode();
      digitizer->SetProduceNoise(fProduceNoise);
      digitizer->SetCreateMatches(fCreateMatches);
      run->AddTask(digitizer);
      LOG(info) << fName << ": Added task " << digitizer->GetName();
    }  //? active and digitizer instance present
  }    //# digitizers


  // --- In event-by-event mode: also empty events (time-slices) are stored
  if (fEventMode) StoreAllTimeSlices();


  // --- Register DAQ
  run->AddTask(fDaq);


  // --- Set runtime database
  LOG(info) << fName << ": Setting runtime DB ";
  LOG(info) << fName << ": ROOT I/O is " << fParRootFile;
  FairRuntimeDb* rtdb          = run->GetRuntimeDb();
  FairParRootFileIo* parIoRoot = new FairParRootFileIo();
  parIoRoot->open(fParRootFile.Data(), "UPDATE");
  if (fParAsciiFiles.IsEmpty()) {
    LOG(info) << fName << ": No ASCII input to parameter database";
    rtdb->setFirstInput(parIoRoot);
  }  //? ASCII parameter file list empty
  else {
    FairParAsciiFileIo* parIoAscii = new FairParAsciiFileIo();
    parIoAscii->open(&fParAsciiFiles, "in");
    rtdb->setFirstInput(parIoAscii);
    rtdb->setSecondInput(parIoRoot);
  }  //? ASCII parameter file list not empty
  LOG(info) << "===================================================";


  // --- Initialise run
  std::cout << std::endl << std::endl;
  LOG(info) << "===================================================";
  LOG(info) << fName << ": Initialising run...";
  run->Init();
  rtdb->setOutput(parIoRoot);
  rtdb->saveOutput();
  LOG(info) << fName << ": Initialising run...finished";
  LOG(info) << "===================================================";


  // --- Run digitisation
  std::cout << std::endl << std::endl << std::endl;
  LOG(info) << "===================================================";
  LOG(info) << fName << ": Starting run...";
  if (event2 < 0) {
    if (event1 >= 0)
      run->Run(0, event1 - 1);  // Run event1 events
    else
      run->Run();  // Run all events in input
  } else {
    if (event1 < 0) event1 = 0;
    if (event1 <= event2)
      run->Run(event1, event2);  // Run from event1 to event2
    else
      run->Run(event1, event1);  // Run only event1
  }
  std::cout << std::endl;
  LOG(info) << fName << ": Run finished.";
  LOG(info) << fName << ": Output file is    " << fOutFile;
  LOG(info) << fName << ": Parameter file is " << fParRootFile;
  if (!fMoniFile.IsNull())
    LOG(info) << fName << ": Monitor file is   " << fMoniFile;

  LOG(info) << "===================================================";


  // --- Resource monitoring
  std::cout << std::endl << std::endl;
  LOG(info) << fName << ": CPU consumption";
  if (!fMoniFile.IsNull()) FairMonitor::GetMonitor()->Print();
  std::cout << std::endl;


  // --- Clean up
  // TODO: I confess I do not know why the TGeoManager has to be deleted here.
  // As far as I can see, the same code is called from ~FairRunaAna().
  // But if I do not do it, I get an error like
  // root.exe(11905,0x7fff7d1a1300) malloc: *** error for object 0x7f811d201860:
  // pointer being freed was not allocated
  if (gGeoManager) {
    if (gROOT->GetVersionInt() >= 60602) {
      gGeoManager->GetListOfVolumes()->Delete();
      gGeoManager->GetListOfShapes()->Delete();
    }
    delete gGeoManager;
  }
  delete run;
}
// --------------------------------------------------------------------------


// -----   Set default info   -----------------------------------------------
void CbmDigitization::SetDefaultBranches() {
  fDigitizers[ECbmModuleId::kMvd] =
    new CbmDigitizeInfo(ECbmModuleId::kMvd, "MvdPoint");
  fDigitizers[ECbmModuleId::kSts] =
    new CbmDigitizeInfo(ECbmModuleId::kSts, "StsPoint");
  fDigitizers[ECbmModuleId::kRich] =
    new CbmDigitizeInfo(ECbmModuleId::kRich, "RichPoint");
  fDigitizers[ECbmModuleId::kMuch] =
    new CbmDigitizeInfo(ECbmModuleId::kMuch, "MuchPoint");
  fDigitizers[ECbmModuleId::kTrd] =
    new CbmDigitizeInfo(ECbmModuleId::kTrd, "TrdPoint");
  fDigitizers[ECbmModuleId::kTof] =
    new CbmDigitizeInfo(ECbmModuleId::kTof, "TofPoint");
  fDigitizers[ECbmModuleId::kPsd] =
    new CbmDigitizeInfo(ECbmModuleId::kPsd, "PsdPoint");
}
// --------------------------------------------------------------------------


// -----   Set digitizer explicitly   ---------------------------------------
void CbmDigitization::SetDigitizer(ECbmModuleId system,
                                   CbmDigitizeBase* digitizer,
                                   TString branch,
                                   Bool_t persistent) {

  // Digitizer already present: replace
  if (fDigitizers.find(system) != fDigitizers.end()) {
    CbmDigitizeBase* oldDigitizer = fDigitizers[system]->GetDigitizer();
    if (oldDigitizer != nullptr) {
      LOG(warn) << fName << ": replacing " << oldDigitizer->GetName() << " by "
                << digitizer->GetName();
      delete oldDigitizer;
    }
    if (!branch.IsNull()) fDigitizers[system]->SetBranchName(branch);
    fDigitizers[system]->SetDigitizer(digitizer);
    fDigitizers[system]->SetActive();
    fDigitizers[system]->SetPersistent(persistent);
  }  //? digitizer present

  // Digitizer not yet present: add
  else
    fDigitizers[system] =
      new CbmDigitizeInfo(system, branch, digitizer, kFALSE, kTRUE, persistent);
}
// --------------------------------------------------------------------------


// -----   Set the output file   --------------------------------------------
void CbmDigitization::SetOutputFile(TString path, Bool_t overwrite) {

  // --- Protect against overwriting an existing file
  if ((!gSystem->AccessPathName(path.Data())) && (!overwrite)) {
    LOG(fatal) << fName << ": output file " << path << " already exists!";
    return;
  }

  // --- If the directory does not yet exist, create it
  const char* directory = gSystem->DirName(path.Data());
  if (gSystem->AccessPathName(directory)) {
    Int_t success = gSystem->mkdir(directory, kTRUE);
    if (success == -1)
      LOG(fatal) << fName << ": output directory " << directory
                 << " does not exist and cannot be created!";
    else
      LOG(info) << fName << ": created directory " << directory;
  }

  fOutFile = path;
}
// --------------------------------------------------------------------------


// -----    Set the ROOT parameter file   -----------------------------------
void CbmDigitization::SetParameterRootFile(TString fileName) {
  if (gSystem->AccessPathName(fileName))
    LOG(fatal) << fName << ": parameter file " << fileName
               << " does not exist!";
  fParRootFile = fileName;
}
// --------------------------------------------------------------------------


// -----   Get digitizer pointer if existing   ------------------------------
CbmDigitizeBase* CbmDigitization::GetDigitizer(ECbmModuleId system) {

  // Digitizer already present: return it
  if (fDigitizers.find(system) != fDigitizers.end()) {
    return fDigitizers[system]->GetDigitizer();
  }  //? digitizer present
  // Digitizer not present: return nullptr
  else
    return nullptr;
}
// --------------------------------------------------------------------------


ClassImp(CbmDigitization);
