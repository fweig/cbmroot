// -------------------------------------------------------------------------
// -----                CbmPlutoGenerator source file                  -----
// -----          Created 13/07/04  by V. Friese / D.Bertini           -----
// -------------------------------------------------------------------------
#include "CbmPlutoGenerator.h"

#include "FairLogger.h"
#include "FairPrimaryGenerator.h"  // for FairPrimaryGenerator

#include "PDataBase.h"    // for PDataBase
#include "PParticle.h"    // for PParticle
#include "PStaticData.h"  // for PStaticData

#include "TArchiveFile.h"    // for TArchiveFile
#include "TChain.h"          // for TChain
#include "TClonesArray.h"    // for TClonesArray
#include "TDatabasePDG.h"    // for TDatabasePDG
#include "TFile.h"           // for TFile
#include "TLorentzVector.h"  // for TLorentzVector
#include "TTree.h"           // for TTree
#include "TVector3.h"        // for TVector3
#include <iosfwd>            // for ostream

//#include <stddef.h>                     // for NULL
#include <iostream>  // for operator<<, basic_ostream, etc
#include <sys/stat.h>

// -----   Default constructor   ------------------------------------------
CbmPlutoGenerator::CbmPlutoGenerator()
  : FairGenerator()
  , fdata(makeStaticData())
  , fbase(makeDataBase())
  , iEvent(0)
  , fFileName(nullptr)
  , fInputChain(nullptr)
  , fParticles(nullptr)
  , fPDGmanual(0) {}
// ------------------------------------------------------------------------

// -----   Standard constructor   -----------------------------------------
CbmPlutoGenerator::CbmPlutoGenerator(const Char_t* fileName)
  : FairGenerator()
  , fdata(makeStaticData())
  , fbase(makeDataBase())
  , iEvent(0)
  , fFileName(fileName)
  , fInputChain(nullptr)
  , fParticles(new TClonesArray("PParticle", 100))
  , fPDGmanual(0) {
  fInputChain = new TChain("data");

  if (CheckFileExist(fileName)) {
    fInputChain->Add(fileName);
    fInputChain->SetBranchAddress("Particles", &fParticles);
    LOG(info) << "CbmPlutoGenerator: Add file " << fileName
              << " to input chain";
  } else {
    LOG(fatal) << "Problem opening file " << fileName;
  }
}
// ------------------------------------------------------------------------

// -----  Constructor with file list   -----------------------------------------
CbmPlutoGenerator::CbmPlutoGenerator(std::vector<std::string> fileNames)
  : FairGenerator()
  , fdata(makeStaticData())
  , fbase(makeDataBase())
  , iEvent(0)
  , fFileName()
  , fInputChain(nullptr)
  , fParticles(new TClonesArray("PParticle", 100))
  , fPDGmanual(0) {
  fInputChain = new TChain("data");
  for (auto& name : fileNames) {
    if (CheckFileExist(name)) {
      fInputChain->Add(name.c_str());
      LOG(info) << "CbmPlutoGenerator: Add file " << name << " to input chain";
    } else {
      LOG(fatal) << "Problem opening file " << name;
    }
  }

  fInputChain->SetBranchAddress("Particles", &fParticles);
}


// -----   Destructor   ---------------------------------------------------
CbmPlutoGenerator::~CbmPlutoGenerator() {
  // remove Pluto database
  delete fdata;
  delete fbase;
  CloseInput();
}
// ------------------------------------------------------------------------


// -----   Public method ReadEvent   --------------------------------------
Bool_t CbmPlutoGenerator::ReadEvent(FairPrimaryGenerator* primGen) {

  // Check for input file
  if (!fInputChain) {
    LOG(error) << "CbmPlutoGenerator: Input file not open!";
    return kFALSE;
  }

  // Check for number of events in input file
  if (iEvent > fInputChain->GetEntries()) {
    LOG(error) << "CbmPlutoGenerator: No more events in input file!";
    CloseInput();
    return kFALSE;
  }
  fInputChain->GetEntry(iEvent++);

  // Get PDG database
  TDatabasePDG* dataBase = TDatabasePDG::Instance();

  // Get number of particle in TClonesrray
  Int_t nParts = fParticles->GetEntriesFast();

  // Loop over particles in TClonesArray
  for (Int_t iPart = 0; iPart < nParts; iPart++) {
    PParticle* part = (PParticle*) fParticles->At(iPart);

    Int_t* pdgType = 0x0;
    Bool_t found = fbase->GetParamInt("pid", part->ID(), "pythiakf", &pdgType);
    // TODO: replace by fdata->GetParticleKF(part->ID()); as soon as FairSoft uses pluto version 5.43 or higher and remove fbase

    // Check if particle type is known to database
    if (!found) {
      LOG(warn) << "CbmPlutoGenerator: Unknown type " << part->ID()
                << ", skipping particle.";
      continue;
    }
    LOG(info) << iPart << " Particle (geant " << part->ID() << " PDG "
              << *pdgType << " -> "
              << dataBase->GetParticle(*pdgType)->GetName();

    // set PDG by hand for pluto dilepton pairs and other not defined codes in pluto
    Int_t dielectron = 99009911;
    Int_t dimuon     = 99009913;
    if (fPDGmanual && *pdgType == 0) {
      pdgType = &fPDGmanual;
      LOG(warn) << "PDG code changed by user defintion to " << *pdgType;
    } else if (part->ID() == 51)
      pdgType = &dielectron;
    else if (part->ID() == 50)
      pdgType = &dimuon;

    // get the mother
    Int_t parIdx = part->GetParentIndex();
    // get daughter
    Int_t idx = part->GetDaughterIndex();

    TLorentzVector mom = part->Vect4();
    Double_t px        = mom.Px();
    Double_t py        = mom.Py();
    Double_t pz        = mom.Pz();
    Double_t ee        = mom.E();

    TVector3 vertex = part->getVertex();
    Double_t vx     = vertex.x();
    Double_t vy     = vertex.y();
    Double_t vz     = vertex.z();

    Bool_t wanttracking = kTRUE;
    if (idx > -1) wanttracking = kFALSE;  // only tracking for decay products
    Int_t parent = parIdx;
    LOG(info) << "Add particle with parent at index " << parIdx
              << " and do tracking " << wanttracking;

    // Give track to PrimaryGenerator
    primGen->AddTrack(
      *pdgType, px, py, pz, vx, vy, vz, parent, wanttracking, ee);

  }  //  Loop over particle in event

  return kTRUE;
}
// ------------------------------------------------------------------------


// -----   Private method CloseInput   ------------------------------------
void CbmPlutoGenerator::CloseInput() {
  if (fInputChain) {
    LOG(info) << "CbmPlutoGenerator: Closing input file " << fFileName;
    delete fInputChain;
  }
  fInputChain = nullptr;
}
// ------------------------------------------------------------------------

Bool_t CbmPlutoGenerator::CheckFileExist(std::string filename) {

  // In case the filename contains a hash (e.g. multi.zip#file.root) assume that the hash
  // separates the name of a zipfile (multi.zip) which contains the real root file
  // (file.root). Split the filename at the # in the name of the zipfile and
  // the name of the contained root file.
  std::string checkFilename {""};
  std::string membername {""};
  std::size_t found = filename.find("#");
  if (found != std::string::npos) {
    checkFilename = filename.substr(0, found);
    membername    = filename.substr(found + 1);
  } else {
    checkFilename = filename;
  }

  Bool_t wasfound = kFALSE;

  // Check if the file exist
  // In case of a root file contained in a zip archive check if the zip file
  // exist
  struct stat buffer;
  if (stat(checkFilename.c_str(), &buffer) == 0) {
    wasfound = kTRUE;
  } else {
    wasfound = kFALSE;
    LOG(error) << "Input File " << checkFilename << " not found";
  }

  // In case of a zip archive check if the archive contains the root file
  if (membername.compare("") != 0) {
    TFile* fzip = TFile::Open(checkFilename.c_str());
    if (fzip->IsOpen()) {
      TArchiveFile* archive = fzip->GetArchive();
      if (archive) {
        TObjArray* members = archive->GetMembers();
        if (members->FindObject(membername.c_str()) == 0) {
          LOG(error) << "File " << membername << " not found in zipfile "
                     << checkFilename;
          wasfound = kFALSE;
        } else {
          LOG(info) << "File " << membername << " found in zipfile "
                    << checkFilename;
          wasfound = kTRUE;
        }
      } else {
        LOG(error) << "Zipfile " << checkFilename
                   << " does not contain an archive";
        wasfound = kFALSE;
      }
      fzip->Close();
      delete fzip;
    } else {
      LOG(error) << "Could not open zipfile " << checkFilename;
      wasfound = kFALSE;
    }
  }

  return wasfound;
}

ClassImp(CbmPlutoGenerator)
