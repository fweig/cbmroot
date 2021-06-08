/* Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: V. Vovchenko, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                FairUrqmdGenerator source file                  -----
// -----                Created 24/01/14  by V. Vovchenko -----
//                      modified 09/2017 by I.Vassiliev
//-------------------------------------------------------------------------
#include "CbmPhsdGenerator.h"

#include "FairMCEventHeader.h"
#include "FairPrimaryGenerator.h"

#include "TDatabasePDG.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"
#include "TObjArray.h"
#include "TPDGCode.h"
#include "TParticle.h"
#include "TParticlePDG.h"
#include "TRandom.h"
#include "TString.h"
#include "TVirtualMCStack.h"

#include <cstring>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

const Double_t kProtonMass = 0.938271998;


// -----   Default constructor   ------------------------------------------
CbmPhsdGenerator::CbmPhsdGenerator()
  : FairGenerator()
  ,
  //fInputFile(nullptr),
  fBaryonsFile(nullptr)
  , fMesonsFile(nullptr)
  , fParticleTable()
  , fFileName(nullptr)
{
}
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
CbmPhsdGenerator::CbmPhsdGenerator(const char* fileNameInput, const char* fileNameBaryons, const char* fileNameMesons)
  : FairGenerator()
  , fBaryonsFile(nullptr)
  , fMesonsFile(nullptr)
  , fDatFile(nullptr)
  , fParticleTable()
{
  //  cout << "-I CbmPhsdGenerator: Opening input file " << fileNameInput << endl;
  cout << "-I CbmPhsdGenerator: Opening HSD Baryons file " << fileNameBaryons << endl;
  fBaryonsFile = fopen(fileNameBaryons, "r");
  if (!fBaryonsFile) { Fatal("FairHSDgenerator", "Cannot open HSD Baryons file."); }
  cout << "-I CbmPhsdGenerator: Opening HSD Mesons file " << fileNameMesons << endl;
  fMesonsFile = fopen(fileNameMesons, "r");
  if (!fMesonsFile) { Fatal("FairHSDgenerator", "Cannot open HSD Mesons file."); }
  ReadConversionTable();
  ReadCollisionData(fileNameInput);
  nextBaryon.init = false;
  nextMeson.init  = false;
  nextEvent       = 1;
  fReadDat        = false;
}
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
CbmPhsdGenerator::CbmPhsdGenerator(const char* fileNameInput, const char* fileNameDat)
  : FairGenerator()
  , fBaryonsFile(nullptr)
  , fMesonsFile(nullptr)
  , fDatFile(nullptr)
  , fParticleTable()
{
  //  cout << "-I CbmPhsdGenerator: Opening input file " << fileNameInput << endl;
  cout << "-I CbmPhsdGenerator: Opening phsd.dat file " << fileNameDat << endl;
  fDatFile = fopen(fileNameDat, "r");
  if (!fDatFile) { Fatal("CbmPhsdGenerator", "Cannot open phsd.dat file."); }
  ReadCollisionData(fileNameInput);
  nextEvent = 1;
  fReadDat  = true;
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
CbmPhsdGenerator::~CbmPhsdGenerator()
{
  //  cout<<"Enter Destructor of CbmPhsdGenerator"<<endl;
  /*if ( fInputFile ) {
    fclose(fInputFile);
    fInputFile = nullptr;
  }*/
  if (fBaryonsFile) {
    fclose(fBaryonsFile);
    fBaryonsFile = nullptr;
  }
  if (fMesonsFile) {
    fclose(fMesonsFile);
    fMesonsFile = nullptr;
  }
  if (fDatFile) {
    fclose(fDatFile);
    fDatFile = nullptr;
  }
  fParticleTable.clear();
  //  cout<<"Leave Destructor of CbmPhsdGenerator"<<endl;
}
// ------------------------------------------------------------------------


// -----   Public method ReadEvent   --------------------------------------
Bool_t CbmPhsdGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  if (fReadDat) return ReadEventDat(primGen);
  else
    return ReadEvent300(primGen);
}
// ------------------------------------------------------------------------


// -----   Public method ReadEventDat   -----------------------------------
Bool_t CbmPhsdGenerator::ReadEventDat(FairPrimaryGenerator* primGen)
{

  // ---> Check for input file
  if (!fDatFile) {
    cout << "-E CbmPhsdGenerator: phsd.dat input file is not open! " << endl;
    return kFALSE;
  }


  int nTracks;
  double b;

  if (fscanf(fDatFile, "%d %*d %*d %lf%*[^\n]%*c", &nTracks, &b) == EOF) {
    cout << "-E- CbmPhsdGenerator::ReadEvent: "
         << "No more events!" << endl;
    return kFALSE;
  }

  fscanf(fDatFile, "%*[^\n]%*c");

  cout << "Event: " << nextEvent << "  nTracks: " << nTracks << "\n";


  //int pid = 0;

  // Set event id and impact parameter in MCEvent if not yet done
  FairMCEventHeader* event = primGen->GetEvent();
  if (event && (!event->IsSet())) {
    event->SetEventID(nextEvent);
    event->SetB(b);
    event->MarkSet(kTRUE);
  }

  for (int i = 0; i < nTracks; ++i) {
    int ttype, tchr;
    double tmppz, tmppx, tmppy, tmpp0;
    fscanf(fDatFile, "%d%d%lf%lf%lf%lf%*[^\n]%*c", &ttype, &tchr, &tmppx, &tmppy, &tmppz, &tmpp0);

    Int_t pdgID = ttype;

    //    cout << "-I CbmPhsdGenerator: PID   " << pdgID << endl;

    Double_t eBeam   = ekin + kProtonMass;
    Double_t pBeam   = TMath::Sqrt(eBeam * eBeam - kProtonMass * kProtonMass);
    Double_t betaCM  = pBeam / (eBeam + kProtonMass);
    Double_t gammaCM = TMath::Sqrt(1. / (1. - betaCM * betaCM));

    // Lorentztransformation to lab

    Double_t px   = Double_t(tmppx);
    Double_t py   = Double_t(tmppy);
    Double_t pz   = Double_t(tmppz);
    Double_t e    = Double_t(tmpp0);
    Double_t mass = sqrt(e * e - px * px - py * py - pz * pz);
    pz            = gammaCM * (pz + betaCM * e);
    Double_t ee   = sqrt(mass * mass + px * px + py * py + pz * pz);

    TVector3 aa(px, py, pz);
    TLorentzVector pp;
    pp.SetPx(px);
    pp.SetPy(py);
    pp.SetPz(pz);
    pp.SetE(ee);

    // Give track to PrimaryGenerator
    primGen->AddTrack(pdgID, px, py, pz, 0., 0., 0.);
  }

  cout << "-I CbmPhsdGenerator: Event " << nextEvent << ",  b = " << b << " fm,  multiplicity " << nTracks
       << ", ekin: " << ekin << endl;

  nextEvent++;

  return kTRUE;
}
// ------------------------------------------------------------------------


// -----   Public method ReadEvent300   -----------------------------------
Bool_t CbmPhsdGenerator::ReadEvent300(FairPrimaryGenerator* primGen)
{

  // ---> Check for input file
  if (!fBaryonsFile) {
    cout << "-E CbmPhsdGenerator: Baryons input file is not open! " << endl;
    return kFALSE;
  }

  if (!fMesonsFile) {
    cout << "-E CbmPhsdGenerator: Mesons input file is not open! " << endl;
    return kFALSE;
  }

  // ---> Check for primary generator
  if (!primGen) {
    cout << "-E- CbmPhsdGenerator::ReadEvent: "
         << "No PrimaryGenerator!" << endl;
    return kFALSE;
  }

  if (!nextBaryon.init && feof(fBaryonsFile)) {
    cout << "-E- CbmPhsdGenerator::ReadEvent: "
         << "No more events!" << endl;
    return kFALSE;
  }

  /*if (!nextMeson.init && feof(fMesonsFile)) {
	cout << "-E- CbmPhsdGenerator::ReadEvent: "
         << "No more events!" << endl;
    return kFALSE;
  }*/

  if (!nextBaryon.init) {
    fscanf(fBaryonsFile, "%d %d %d %d %lf %lf %lf %lf %lf", &nextBaryon.id, &nextBaryon.charge, &nextBaryon.ISUB,
           &nextBaryon.IRUN, &nextBaryon.px, &nextBaryon.py, &nextBaryon.pz, &nextBaryon.p0, &nextBaryon.b);
    fscanf(fBaryonsFile, "%*[^\n]%*c");
    nextBaryon.globalEvent = (nextBaryon.ISUB - 1) * IRUNS + nextBaryon.IRUN;
    nextBaryon.init        = 1;
  }

  if (!nextMeson.init && !feof(fMesonsFile)) {
    fscanf(fMesonsFile, "%d %d %d %d %lf %lf %lf %lf %lf", &nextMeson.id, &nextMeson.charge, &nextMeson.ISUB,
           &nextMeson.IRUN, &nextMeson.px, &nextMeson.py, &nextMeson.pz, &nextMeson.p0, &nextMeson.b);
    fscanf(fMesonsFile, "%*[^\n]%*c");
    nextMeson.globalEvent = (nextMeson.ISUB - 1) * IRUNS + nextMeson.IRUN;
    nextMeson.init        = 1;
  }


  int nTracks = 0, pid = 0;
  double b = nextBaryon.b;

  // Set event id and impact parameter in MCEvent if not yet done
  FairMCEventHeader* event = primGen->GetEvent();
  if (event && (!event->IsSet())) {
    event->SetEventID(nextEvent);
    event->SetB(b);
    event->MarkSet(kTRUE);
  }

  // Read all baryons from current event until we reach the next event in fort.300 or end of file
  while (nextBaryon.globalEvent == nextEvent) {
    // Convert HSD type and charge to unique pid identifier which is based on
    // HSD particle id and charge, calculated separately for baryons, anti-baryons and mesons
    if (nextBaryon.id >= 0) { pid = nextBaryon.id * 10 + (2 + nextBaryon.charge); }
    // antibaryons
    else {
      pid = -(-nextBaryon.id * 10 + (2 - nextBaryon.charge));
    }

    // Convert Unique PID into PDG particle code
    if (fParticleTable.find(pid) == fParticleTable.end()) {
      cout << "-W CbmPhsdGenerator: PID " << nextBaryon.id << " charge " << nextBaryon.charge << " not found in table ("
           << pid << ")" << endl;
      if (feof(fBaryonsFile)) {
        nextBaryon.init = 0;
        break;
      }
      fscanf(fBaryonsFile, "%d %d %d %d %lf %lf %lf %lf %lf", &nextBaryon.id, &nextBaryon.charge, &nextBaryon.ISUB,
             &nextBaryon.IRUN, &nextBaryon.px, &nextBaryon.py, &nextBaryon.pz, &nextBaryon.p0, &nextBaryon.b);
      fscanf(fBaryonsFile, "%*[^\n]%*c");
      nextBaryon.globalEvent = (nextBaryon.ISUB - 1) * IRUNS + nextBaryon.IRUN;
      continue;
    }
    Int_t pdgID = fParticleTable[pid];

    Double_t eBeam   = ekin + kProtonMass;
    Double_t pBeam   = TMath::Sqrt(eBeam * eBeam - kProtonMass * kProtonMass);
    Double_t betaCM  = pBeam / (eBeam + kProtonMass);
    Double_t gammaCM = TMath::Sqrt(1. / (1. - betaCM * betaCM));

    // Lorentztransformation to lab

    Double_t px   = Double_t(nextBaryon.px);
    Double_t py   = Double_t(nextBaryon.py);
    Double_t pz   = Double_t(nextBaryon.pz);
    Double_t e    = Double_t(nextBaryon.p0);
    Double_t mass = sqrt(e * e - px * px - py * py - pz * pz);
    pz            = gammaCM * (pz + betaCM * e);
    Double_t ee   = sqrt(mass * mass + px * px + py * py + pz * pz);

    TVector3 aa(px, py, pz);
    TLorentzVector pp;
    pp.SetPx(px);
    pp.SetPy(py);
    pp.SetPz(pz);
    pp.SetE(ee);

    // Give track to PrimaryGenerator
    primGen->AddTrack(pdgID, px, py, pz, 0., 0., 0.);

    nTracks++;

    if (feof(fBaryonsFile)) {
      nextBaryon.init = 0;
      break;
    }

    fscanf(fBaryonsFile, "%d %d %d %d %lf %lf %lf %lf %lf", &nextBaryon.id, &nextBaryon.charge, &nextBaryon.ISUB,
           &nextBaryon.IRUN, &nextBaryon.px, &nextBaryon.py, &nextBaryon.pz, &nextBaryon.p0, &nextBaryon.b);
    fscanf(fBaryonsFile, "%*[^\n]%*c");
    nextBaryon.globalEvent = (nextBaryon.ISUB - 1) * IRUNS + nextBaryon.IRUN;
  }

  // Read all mesons from current event until we reach the next event in fort.301 or end of file
  while (nextMeson.globalEvent == nextEvent) {
    // Convert HSD type and charge to unique pid identifier which is based on
    // HSD particle id and charge, calculated separately for baryons, anti-baryons and mesons
    {
      pid = 1000 + nextMeson.id * 10 + (2 + nextMeson.charge);
    }

    // Convert Unique PID into PDG particle code
    if (fParticleTable.find(pid) == fParticleTable.end()) {
      cout << "-W CbmPhsdGenerator: PID " << nextMeson.id << " charge " << nextMeson.charge << " not found in table ("
           << pid << ")" << endl;
      fscanf(fMesonsFile, "%d %d %d %d %lf %lf %lf %lf %lf", &nextMeson.id, &nextMeson.charge, &nextMeson.ISUB,
             &nextMeson.IRUN, &nextMeson.px, &nextMeson.py, &nextMeson.pz, &nextMeson.p0, &nextMeson.b);
      fscanf(fMesonsFile, "%*[^\n]%*c");
      nextMeson.globalEvent = (nextMeson.ISUB - 1) * IRUNS + nextMeson.IRUN;
      continue;
    }
    Int_t pdgID = fParticleTable[pid];

    Double_t eBeam   = ekin + kProtonMass;
    Double_t pBeam   = TMath::Sqrt(eBeam * eBeam - kProtonMass * kProtonMass);
    Double_t betaCM  = pBeam / (eBeam + kProtonMass);
    Double_t gammaCM = TMath::Sqrt(1. / (1. - betaCM * betaCM));

    // Lorentztransformation to lab

    Double_t px   = Double_t(nextMeson.px);
    Double_t py   = Double_t(nextMeson.py);
    Double_t pz   = Double_t(nextMeson.pz);
    Double_t e    = Double_t(nextMeson.p0);
    Double_t mass = sqrt(e * e - px * px - py * py - pz * pz);
    pz            = gammaCM * (pz + betaCM * e);
    Double_t ee   = sqrt(mass * mass + px * px + py * py + pz * pz);

    TVector3 aa(px, py, pz);
    TLorentzVector pp;
    pp.SetPx(px);
    pp.SetPy(py);
    pp.SetPz(pz);
    pp.SetE(ee);

    // Give track to PrimaryGenerator
    primGen->AddTrack(pdgID, px, py, pz, 0., 0., 0.);

    nTracks++;

    if (feof(fMesonsFile)) {
      nextMeson.init = 0;
      break;
    }

    fscanf(fMesonsFile, "%d %d %d %d %lf %lf %lf %lf %lf", &nextMeson.id, &nextMeson.charge, &nextMeson.ISUB,
           &nextMeson.IRUN, &nextMeson.px, &nextMeson.py, &nextMeson.pz, &nextMeson.p0, &nextMeson.b);
    fscanf(fMesonsFile, "%*[^\n]%*c");
    nextMeson.globalEvent = (nextMeson.ISUB - 1) * IRUNS + nextMeson.IRUN;
  }

  cout << "-I CbmPhsdGenerator: Event " << nextEvent << ",  b = " << b << " fm,  multiplicity " << nTracks
       << ", ekin: " << ekin << endl;

  nextEvent++;

  return kTRUE;
}
// ------------------------------------------------------------------------

/**
// -----   Public method ReadEvent   --------------------------------------
Bool_t CbmPhsdGenerator::SkipEvents(Int_t count)
{
  if (count<=0) { return kTRUE; }

  for(Int_t ii=0; ii<count; ii++) {
    // ---> Check for input file
    if ( ! fInputFile ) {
      cout << "-E FairUrqmdGenerator: Input file not open! " << endl;
      return kFALSE;
    }

    // ---> Define event variables to be read from file
    int evnr=0, ntracks=0, aProj=0, zProj=0, aTarg=0, zTarg=0;
    float b = 0., ekin = 0.;

    // ---> Read and check first event header line from input file
    char read[200];
    fgets(read, 200, fInputFile);
    if ( feof(fInputFile) ) {
      cout << "-I FairUrqmdGenerator : End of input file reached." << endl;
      fclose(fInputFile);
      fInputFile = nullptr;
      return kFALSE;
    }
    if ( read[0] != 'U' ) {
      cout << "-E FairUrqmdGenerator: Wrong event header" << endl;
      return kFALSE;
    }

    // ---> Read rest of event header
    fgets(read, 26, fInputFile);
    fscanf(fInputFile, "%d", &aProj);
    fscanf(fInputFile, "%d", &zProj);
    fgets(read, 25, fInputFile);
    fscanf(fInputFile, "%d", &aTarg);
    fscanf(fInputFile, "%d", &zTarg);
    fgets(read, 200, fInputFile);
    fgets(read, 200, fInputFile);
    fgets(read, 36, fInputFile);
    fscanf(fInputFile, "%f", &b);
    fgets(read, 200, fInputFile);
    fgets(read, 39, fInputFile);
    fscanf(fInputFile, "%e", &ekin);
    fgets(read, 200, fInputFile);
    fgets(read, 7, fInputFile);
    fscanf(fInputFile, "%d", &evnr);
    fgets(read, 200, fInputFile);
    for (int iline=0; iline<8; iline++)  { fgets(read, 200,fInputFile); }
    fscanf(fInputFile, "%d", &ntracks);
    fgets(read, 200, fInputFile);
    fgets(read, 200, fInputFile);

    cout << "-I FairUrqmdGenerator: Event " << evnr << " skipped!" << endl;

    // ---> Loop over tracks in the current event
    for(int itrack=0; itrack<ntracks; itrack++) {

      // Read momentum and PID from file
      fgets(read, 81, fInputFile);
      fgets(read, 200, fInputFile);
    }
  }
  return kTRUE;
}**/
// ------------------------------------------------------------------------

// -----   Private method ReadConversionTable   ---------------------------
void CbmPhsdGenerator::ReadConversionTable()
{

  TString work           = getenv("VMCWORKDIR");
  TString fileName       = work + "/input/hsd_pdg.dat";
  std::ifstream* pdgconv = new std::ifstream(fileName.Data());

  if (!(*pdgconv).is_open()) {
    Fatal("CbmPhsdGenerator", "Particle table for conversion was not found!");
    //cout << "-W CbmPhsdGenerator: Particle table for conversion was not found!" << endl;
  }

  Int_t index = 0;
  Int_t pdgId = 0;

  string tmpStr;

  while (!pdgconv->eof()) {
    index = pdgId = 0;
    *pdgconv >> index >> pdgId;
    std::getline(*pdgconv, tmpStr);
    fParticleTable[index] = pdgId;
  }

  pdgconv->close();
  delete pdgconv;

  cout << "-I CbmPhsdGenerator: Particle table for conversion from "
       << "HSD loaded" << endl;
}
// ------------------------------------------------------------------------

// -----   Private method ReadCollisionData   ----------------------------
void CbmPhsdGenerator::ReadCollisionData(const char* fileNameInput)
{

  std::ifstream* inputf = new std::ifstream(fileNameInput);

  //  Int_t index = 0;
  //  Int_t pdgId = 0;

  //TString val, name, tname;
  string val, name, tname, tmpStr;

  while (!inputf->eof()) {
    //    index =pdgId =0 ;
    //*inputf >> index >> pdgId ;
    *inputf >> val >> name;
    tname = name.substr(0, 6);
    //cout << tname << endl;
    if (tname == "MASSTA") At = atoi(val.substr(0, val.size() - 1).c_str());
    else if (tname == "MSTAPR")
      Zt = atoi(val.substr(0, val.size() - 1).c_str());
    else if (tname == "MASSPR")
      Ap = atoi(val.substr(0, val.size() - 1).c_str());
    else if (tname == "MSPRPR")
      Zp = atoi(val.substr(0, val.size() - 1).c_str());
    tname = name.substr(0, 4);
    if (tname == "ELAB") ekin = atof(val.substr(0, val.size() - 1).c_str());
    tname = name.substr(0, 5);
    if (tname == "ISUBS") ISUBS = atof(val.substr(0, val.size() - 1).c_str());
    tname = name.substr(0, 3);
    if (tname == "NUM") IRUNS = atof(val.substr(0, val.size() - 1).c_str());
    std::getline(*inputf, tmpStr);
  }

  inputf->close();
  delete inputf;

  cout << "-I CbmPhsdGenerator: Collision data from "
       << "HSD loaded" << endl;
}
// ------------------------------------------------------------------------


ClassImp(CbmPhsdGenerator);
