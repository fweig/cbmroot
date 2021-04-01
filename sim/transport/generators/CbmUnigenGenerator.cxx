/** @file CbmUnigenGenerator.h
 ** @author D. Kresan <d.kresan@gsi.de>
 ** @since 4 July 2006
 **
 ** Revision 4 September 2019 by V. Friese <v.friese@gsi.de>
 **/


#include "CbmUnigenGenerator.h"

#include "FairMCEventHeader.h"
#include "FairPrimaryGenerator.h"
#include "FairRunSim.h"
#include <Logger.h>

#include "TFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TVector3.h"

#include <cassert>
#include <sstream>

#include "UEvent.h"
#include "UParticle.h"
#include "URun.h"


// -----   Constructor   ----------------------------------------------------
CbmUnigenGenerator::CbmUnigenGenerator(const char* fileName, EMode mode)
  : FairGenerator("UnigenGenerator", "CBM generator")
  , fFileName(fileName)
  , fMode(mode)
  , fPhi(0.)
  , fIsInit(kFALSE)
  , fFile(nullptr)
  , fTree(nullptr)
  , fCurrentEntry(-1)
  , fEvent(new UEvent())
  , fNofPrimaries(0)
  , fNofEvents(0)
  , fBetaCM(0.)
  , fGammaCM(1.)
  , fIonMap() {
  LOG(debug) << GetName() << ": Constructor";
  if (mode == kRotateFixed)
    LOG(fatal) << GetName() << ": When choosing "
               << "mode kRotateFixed, a rotation angle must be specified!";
  if (fileName[0] == '\0') return;
  if (!Init())
    LOG(fatal) << GetName() << ": Error in intialisation! Aborting...";
}
// --------------------------------------------------------------------------


// -----   Constructor   ----------------------------------------------------
CbmUnigenGenerator::CbmUnigenGenerator(const char* fileName,
                                       EMode mode,
                                       Double_t phi)
  : FairGenerator("UnigenGenerator", "CBM generator")
  , fFileName(fileName)
  , fMode(mode)
  , fPhi(phi)
  , fIsInit(kFALSE)
  , fFile(nullptr)
  , fTree(nullptr)
  , fCurrentEntry(-1)
  , fEvent(new UEvent())
  , fNofPrimaries(0)
  , fNofEvents(0)
  , fBetaCM(0.)
  , fGammaCM(1.)
  , fIonMap() {
  LOG(debug) << GetName() << ": Constructor";
  if (fileName[0] == '\0') return;
  if (!Init())
    LOG(fatal) << GetName() << ": Error in intialisation! Aborting...";
}
// --------------------------------------------------------------------------


// -----   Destructor   -----------------------------------------------------
CbmUnigenGenerator::~CbmUnigenGenerator() {
  LOG(debug) << GetName() << ": Destructor";
  CloseInput();
  if (fEvent) delete fEvent;
}
// --------------------------------------------------------------------------


// -----   Add a primary to the event generator   ---------------------------
void CbmUnigenGenerator::AddPrimary(FairPrimaryGenerator* primGen,
                                    Int_t pdgCode,
                                    const TVector3& momentum) {
  primGen->AddTrack(
    pdgCode, momentum.Px(), momentum.Py(), momentum.Pz(), 0., 0., 0.);
  fNofPrimaries++;
}
// --------------------------------------------------------------------------


// -----   Close the input file   -------------------------------------------
void CbmUnigenGenerator::CloseInput() {
  if (!fFile) return;
  LOG(INFO) << GetName() << ": Closing input file " << fFileName;
  fFile->Close();
  fFile = nullptr;
}
// --------------------------------------------------------------------------


// -----   Read next entry from tree   --------------------------------------
Bool_t CbmUnigenGenerator::GetNextEntry() {

  if (++fCurrentEntry >= fTree->GetEntries()) {
    if (fMode == kReuseEvents) {
      LOG(info) << GetName()
                << ": End of input tree - restarting with first entry";
      fCurrentEntry = 0;
    }  //? Re-use entries
    else {
      LOG(info) << GetName() << ": End of input tree reached";
      return kFALSE;
    }  //? Do not re-use entries
  }    //? End of input tree

  // Read entry
  Int_t result = fTree->GetEntry(fCurrentEntry);
  if (result <= 0) {
    LOG(ERROR) << GetName() << ": Error reading entry " << fCurrentEntry
               << " (returns " << result << ")!";
    return kFALSE;
  }

  return kTRUE;
}
// --------------------------------------------------------------------------


// -----   Open the input file   --------------------------------------------
Bool_t CbmUnigenGenerator::Init() {

  // --- Check for file being already initialised
  if (fIsInit) {
    LOG(warning) << GetName() << ": Already initialised!";
    return kTRUE;
  }

  LOG(info) << GetName() << ": Initialising...";
  std::stringstream ss;
  switch (fMode) {
    case kStandard: ss << " standard; rotate event plane to zero"; break;
    case kRotateFixed:
      ss << " rotate event plane by fixed angle " << fPhi;
      break;
    case kNoRotation: ss << " no event plane rotation"; break;
    case kReuseEvents:
      ss << " re-use events if necessary; random event plane angle";
      break;
    default: ss << "unkown"; break;
  }
  LOG(INFO) << GetName() << ": Mode " << ss.str();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  // --- Try to open file
  fFile = new TFile(fFileName, "READ");

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  if (!fFile->IsOpen()) {
    LOG(error) << GetName() << ": Could not open input file " << fFileName;
    return kFALSE;
  }
  LOG(info) << GetName() << ": Open input file " << fFileName;

  // --- Get and print run description
  URun* run = dynamic_cast<URun*>(fFile->Get("run"));
  if (run == nullptr) {
    LOG(error) << GetName() << ": No run description in input file!";
    fFile->Close();
    return kFALSE;
  }
  LOG(info) << GetName() << ": Run description";
  run->Print();

  // --- Lorentz transformation to the lab (target) system
  Double_t mProt = 0.938272;
  Double_t pTarg = run->GetPTarg();  // target momentum per nucleon
  Double_t pProj = run->GetPProj();  // projectile momentum per nucleon
  Double_t eTarg = TMath::Sqrt(pProj * pProj + mProt * mProt);
  Double_t eProj = TMath::Sqrt(pTarg * pTarg + mProt * mProt);
  fBetaCM        = pTarg / eTarg;
  fGammaCM       = 1. / TMath::Sqrt(1. - fBetaCM * fBetaCM);
  Double_t pBeam = fGammaCM * (pProj - fBetaCM * eProj);
  LOG(info) << GetName() << ": sqrt(s_NN) = " << run->GetNNSqrtS()
            << " GeV, p_beam = " << pBeam << " GeV/u";
  LOG(info) << GetName() << ": Lorentz transformation to lab system: "
            << " beta " << fBetaCM << ", gamma " << fGammaCM;

  // --- Get input tree and connect event object to its branch
  fTree = dynamic_cast<TTree*>(fFile->Get("events"));
  if (fTree == nullptr) {
    LOG(error) << GetName() << ": No event tree in input file!";
    fFile->Close();
    return kFALSE;
  }
  fTree->SetBranchAddress("event", &fEvent);
  LOG(info) << GetName() << ": " << fTree->GetEntries()
            << " events in input tree";

  // --- Register ions found in the input file
  Int_t nIons = RegisterIons();
  LOG(info) << GetName() << ": " << nIons << " ions registered.";

  fIsInit = kTRUE;
  return kTRUE;
}
// --------------------------------------------------------------------------


// -----   Process a composite particle   -----------------------------------
void CbmUnigenGenerator::ProcessIon(FairPrimaryGenerator* primGen,
                                    Int_t pdgCode,
                                    const TVector3& momentum) {

  assert(pdgCode >= 1e9);  // Should be an ion PDG

  // Since hyper-nuclei are not (yet) supported by FairRoot, their PDG
  // is replaced by that of the non-strange analogue.
  Int_t ionPdg = pdgCode;
  if (GetIonLambdas(pdgCode)) {
    ionPdg = pdgCode - GetIonLambdas(pdgCode) * kPdgLambda;
    LOG(warn) << GetName() << ": Replacing hypernucleus (PDG " << pdgCode
              << ") by PDG " << ionPdg;
  }  //? Lambdas in ion

  // Charged ions can be registered
  if (GetIonCharge(ionPdg)) AddPrimary(primGen, ionPdg, momentum);

  // Neutral ions are not supported by GEANT4.
  // They are thus decomposed into neutrons (as an approximation)
  else {
    Int_t mass          = GetIonMass(ionPdg);
    TVector3 neutronMom = momentum * (1. / Double_t(mass));
    for (Int_t iNeutron = 0; iNeutron < mass; iNeutron++)
      AddPrimary(primGen, 2112, neutronMom);
    LOG(warn) << GetName() << ": Neutral fragment with PDG " << ionPdg
              << " is split into " << mass << " neutrons";
  }  //? Neutral ion
}
// --------------------------------------------------------------------------


// -----   Read input event   -----------------------------------------------
Bool_t CbmUnigenGenerator::ReadEvent(FairPrimaryGenerator* primGen) {

  // Init must have been called before
  assert(fIsInit);

  // Get next entry from tree
  fNofEvents++;
  if (!GetNextEntry()) return kFALSE;

  // Event properties
  Int_t eventId   = fEvent->GetEventNr();  // Generator event ID
  Int_t nPart     = fEvent->GetNpa();      // Number of particles in event
  Double_t impact = fEvent->GetB();        // Impact parameter
  Double_t phi1   = fEvent->GetPhi();  // Event plane angle (generator) [rad]
  fNofPrimaries   = 0;                 // Number of registered primaries

  // Event rotation angle to be applied on the input momentum vectors
  Double_t phi2 = 0.;
  switch (fMode) {
    case kStandard: phi2 = -1. * phi1; break;
    case kRotateFixed: phi2 = fPhi; break;
    case kNoRotation: phi2 = 0.; break;
    case kReuseEvents: phi2 = gRandom->Uniform(0., 2 * TMath::Pi()); break;
    default: phi2 = 0.; break;
  }  //? mode

  // Loop over particles in current event
  for (Int_t iPart = 0; iPart < fEvent->GetNpa(); iPart++) {
    UParticle* particle = fEvent->GetParticle(iPart);

    // Lorentz transformation into lab (target) frame
    Double_t pz = fGammaCM * (particle->Pz() - fBetaCM * particle->E());
    TVector3 momentum(particle->Px(), particle->Py(), pz);

    // Apply event rotation if needed
    if (phi2 != 0.) momentum.RotateZ(phi2);

    // Normal particle
    if (particle->GetPdg() < 1e9)  // elementary particle
      AddPrimary(primGen, particle->GetPdg(), momentum);
    else
      ProcessIon(primGen, particle->GetPdg(), momentum);

  }  //# Particles

  // Set event information in the MC event header
  FairMCEventHeader* eventHeader = primGen->GetEvent();
  assert(eventHeader);
  if (eventHeader->IsSet()) {
    LOG(warning) << GetName() << ": Event header is already set; "
                 << " event info will not be stored";
  } else {
    eventHeader->SetEventID(eventId);   // Generator event ID
    eventHeader->SetB(fEvent->GetB());  // Impact parameter
    eventHeader->SetRotZ(phi1 + phi2);  // Event plane angle
    eventHeader->MarkSet(kTRUE);
  }

  // Info to screen
  LOG(info) << GetName() << ": Event ID " << eventId << ", particles " << nPart
            << ", primaries " << fNofPrimaries << ", b = " << impact
            << " fm, phi (source) = " << phi1
            << " rad , phi (generated) = " << phi2 << " rad";

  return kTRUE;
}
// --------------------------------------------------------------------------


// -----   Register ions to the run   ---------------------------------------
Int_t CbmUnigenGenerator::RegisterIons() {

  LOG(debug) << GetName() << ": Registering ions";
  UParticle* particle {nullptr};
  Int_t nIons {0};
  fIonMap.clear();

  // --- Event loop
  for (Int_t iEvent = 0; iEvent < fTree->GetEntries(); ++iEvent) {
    fTree->GetEntry(iEvent);

    // --- Particle loop
    for (Int_t iParticle = 0; iParticle < fEvent->GetNpa(); ++iParticle) {
      particle = fEvent->GetParticle(iParticle);

      Int_t pdgCode = particle->GetPdg();
      if (pdgCode > 1e9) {  // ion

        // For ions the PDG code is +-10LZZZAAAI,
        // where A = n_Lambda + n_proton + n_neutron, Z = n_proton, L = n_Lambda
        // and I = 0 - ground state, I>0 - excitations
        const Int_t nLambda = GetIonLambdas(pdgCode);
        const Int_t charge  = GetIonCharge(pdgCode);
        const Int_t mass    = GetIonMass(pdgCode);

        // Neutral ions are skipped (not present in G4IonTable)
        if (charge == 0) continue;

        // Add ion to ion map
        TString ionName = Form("Ion_%d_%d_%d", mass, charge, nLambda);
        if (fIonMap.find(ionName) == fIonMap.end()) {  // new ion
          fIonMap[ionName] = new FairIon(ionName, charge, mass, charge);
          nIons++;
          LOG(debug) << GetName() << ": Adding new ion " << ionName << ", PDG "
                     << pdgCode << " (mass " << mass << ", charge " << charge
                     << ", nLambda " << nLambda << ") from event " << iEvent
                     << " at index " << iParticle;
        }  //? new ion

      }  //? ion
    }    //# particles
  }      //# events

  FairRunSim* run = FairRunSim::Instance();
  for (const auto& entry : fIonMap)
    run->AddNewIon(entry.second);

  return fIonMap.size();
}
// ------------------------------------------------------------------------

ClassImp(CbmUnigenGenerator);
