/* Copyright (C) 2004-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig [committer], Andrey Lebedev, Adrian Meyer-Ahrens, David Emschermann */

/**
 * \file CbmTrd.cxx
 * \author V.Friese <v.friese@gsi.de>
 * \date 27.07.2004
 **/

#include "CbmTrd.h"

#include "CbmGeometryUtils.h"
#include "CbmStack.h"
#include "CbmTrdGeoHandler.h"
#include "CbmTrdPoint.h"

#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoMedia.h"
#include "FairGeoNode.h"
#include "FairGeoVolume.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TVirtualMC.h"

// -----   Default constructor   ----------------------------------------------
CbmTrd::CbmTrd()
  : FairDetector("TRD", kTRUE, ToIntegralType(ECbmModuleId::kTrd))
  , fPosIn()
  , fMomIn()
  , fPosOut()
  , fMomOut()
  , fTime(0)
  , fLength(0)
  , fELoss(0)
  , fPosIndex(0)
  , fTrdPoints(new TClonesArray("CbmTrdPoint"))
  , fGeoHandler(new CbmTrdGeoHandler())
  , fUseGlobalPhysicsProcesses(kFALSE)
  , fCombiTrans(nullptr)
{
  fVerboseLevel = 1;
}
// ----------------------------------------------------------------------------


// -----   Standard constructor   ---------------------------------------------
CbmTrd::CbmTrd(const char* name, Bool_t active)
  : FairDetector(name, active, ToIntegralType(ECbmModuleId::kTrd))
  , fPosIn()
  , fMomIn()
  , fPosOut()
  , fMomOut()
  , fTime(0)
  , fLength(0)
  , fELoss(0)
  , fPosIndex(0)
  , fTrdPoints(new TClonesArray("CbmTrdPoint"))
  , fGeoHandler(new CbmTrdGeoHandler())
  , fUseGlobalPhysicsProcesses(kFALSE)
  , fCombiTrans(nullptr)
{
  fVerboseLevel = 1;
}
// ----------------------------------------------------------------------------


// -----   Destructor   -------------------------------------------------------
CbmTrd::~CbmTrd()
{
  if (fTrdPoints) {
    fTrdPoints->Delete();
    delete fTrdPoints;
  }
  if (fGeoHandler) { delete fGeoHandler; }
}
// ----------------------------------------------------------------------------


// -----   Initialize   -------------------------------------------------------
void CbmTrd::Initialize()
{
  FairDetector::Initialize();

  // Initialize the CbmTrdGeoHandler helper class from the
  // TVirtualMC interface
  Bool_t isSimulation = kTRUE;
  fGeoHandler->Init(isSimulation);
}
// ----------------------------------------------------------------------------


// -----   SetSpecialPhysicsCuts   --------------------------------------------
void CbmTrd::SetSpecialPhysicsCuts()
{
  FairRun* fRun = FairRun::Instance();

  // Setting the properties for the TRDgas is only tested
  // with TGeant3, so we check for the simulation engine
  // and change the standard properties only for TGeant3
  if (strcmp(fRun->GetName(), "TGeant3") == 0) {

    // Get Material Id and some material properties from
    // the geomanager
    Int_t mat            = gGeoManager->GetMaterialIndex("TRDgas");
    TGeoMaterial* trdgas = gGeoManager->GetMaterial(mat);
    trdgas->Print();
    Double_t mass   = trdgas->GetA();
    Double_t charge = trdgas->GetZ();

    // Get the material properties for material with id+1
    // (of-by-one problem) from the Virtual Monte Carlo
    Int_t matIdVMC = mat + 1;

    TString name   = "";
    Double_t a     = 0.;
    Double_t z     = 0.;
    Double_t dens  = 0.;
    Double_t radl  = 0.;
    Double_t inter = 0.;
    TArrayD par;

    gMC->GetMaterial(matIdVMC, name, a, z, dens, radl, inter, par);

    // Check if the material properties are the same for TGeoManager
    // and TVirtualMC. If not stop the execution of the simulation.
    // This is done to stop the program whenever the
    // of-by-one problem is fixed in the Virtual Monte Carlo
    if ((TMath::Abs(mass - a) > 0.0001) || (TMath::Abs(charge - z)) > 0.0001) {
      LOG(fatal) << "**********************************";
      LOG(fatal) << TMath::Abs(mass - a) << " , " << TMath::Abs(charge - z);
      LOG(fatal) << "Parameters from Geomanager:";
      trdgas->Print();
      LOG(fatal) << "Parameters from Virtual Montecarlo:";
      LOG(fatal) << "Name " << name << " Aeff=" << a << " Zeff=" << z;
      Fatal("CbmTrd::SetSpecialPhysicsCuts", "Material parameters different between TVirtualMC and TGeomanager");
    }

    // Set new properties, physics cuts etc. for the TRDgas
    if (!fUseGlobalPhysicsProcesses) {
      LOG(info) << "Using special parameters for TRDgas";
      gMC->Gstpar(matIdVMC, "STRA", 1.0);
      gMC->Gstpar(matIdVMC, "PAIR", 1.0);
      gMC->Gstpar(matIdVMC, "COMP", 1.0);
      gMC->Gstpar(matIdVMC, "PHOT", 1.0);
      gMC->Gstpar(matIdVMC, "LOSS", 2.0);
      //gMC->Gstpar(matIdVMC,"PFIS",0.0);
      gMC->Gstpar(matIdVMC, "ANNI", 1.0);
      gMC->Gstpar(matIdVMC, "BREM", 1.0);
      gMC->Gstpar(matIdVMC, "HADR", 1.0);
      //gMC->Gstpar(matIdVMC,"MUNU",1.0);
      gMC->Gstpar(matIdVMC, "DCAY", 1.0);
      gMC->Gstpar(matIdVMC, "MULS", 1.0);
      //gMC->Gstpar(matIdVMC,"LABS",0.0);
      gMC->Gstpar(matIdVMC, "DRAY", 1.0);
      gMC->Gstpar(matIdVMC, "RAYL", 1.0);
    }
    else
      LOG(info) << "!! Using global parameters for TRDgas";

    // cut values
    gMC->Gstpar(matIdVMC, "CUTELE", 10.e-6);
    gMC->Gstpar(matIdVMC, "CUTGAM", 10.e-6);
    gMC->Gstpar(matIdVMC, "CUTNEU", 10.e-4);
    gMC->Gstpar(matIdVMC, "CUTHAD", 10.e-4);
    gMC->Gstpar(matIdVMC, "CUTMUO", 10.e-6);
    gMC->Gstpar(matIdVMC, "BCUTE", 10.e-6);
    gMC->Gstpar(matIdVMC, "BCUTM", 10.e-6);
    gMC->Gstpar(matIdVMC, "DCUTE", 10.e-6);
    gMC->Gstpar(matIdVMC, "DCUTM", 10.e-6);
    gMC->Gstpar(matIdVMC, "PPCUTM", -1.);
  }
}
// ----------------------------------------------------------------------------


// -----   Public method ProcessHits   ----------------------------------------
Bool_t CbmTrd::ProcessHits(FairVolume*)
{
  // Set parameters at entrance of volume. Reset ELoss.
  if (gMC->IsTrackEntering()) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Create CbmTrdPoint at exit of active volume
  if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared()) {

    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);

    if (fELoss == 0.) return kFALSE;  // no neutrals

    Int_t trackId       = gMC->GetStack()->GetCurrentTrackNumber();
    Int_t moduleAddress = fGeoHandler->GetModuleAddress();

    Int_t size = fTrdPoints->GetEntriesFast();
    new ((*fTrdPoints)[size])
      CbmTrdPoint(trackId, moduleAddress, TVector3(fPosIn.X(), fPosIn.Y(), fPosIn.Z()),
                  TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()), TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
                  TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()), fTime, fLength, fELoss);

    // Increment number of trd points in TParticle
    CbmStack* stack = (CbmStack*) gMC->GetStack();
    stack->AddPoint(ECbmModuleId::kTrd);

    ResetParameters();
  }

  return kTRUE;
}
// ----------------------------------------------------------------------------


// -----   Public method EndOfEvent   -----------------------------------------
void CbmTrd::EndOfEvent()
{
  if (fVerboseLevel) Print();
  fTrdPoints->Delete();
  fPosIndex = 0;
}
// ----------------------------------------------------------------------------


// -----   Public method Register   -------------------------------------------
void CbmTrd::Register() { FairRootManager::Instance()->Register("TrdPoint", "Trd", fTrdPoints, kTRUE); }
// ----------------------------------------------------------------------------


// -----   Public method GetCollection   --------------------------------------
TClonesArray* CbmTrd::GetCollection(Int_t iColl) const
{
  if (iColl == 0) return fTrdPoints;
  else
    return NULL;
}
// ----------------------------------------------------------------------------


// -----   Public method Print   ----------------------------------------------
void CbmTrd::Print(Option_t*) const
{
  Int_t nHits = fTrdPoints->GetEntriesFast();
  LOG(info) << fName << ": " << nHits << " points registered in this event.";

  if (fVerboseLevel > 1) {
    for (Int_t i = 0; i < nHits; i++) {
      (*fTrdPoints)[i]->Print();
    }
  }
}
// ----------------------------------------------------------------------------


// -----   Public method Reset   ----------------------------------------------
void CbmTrd::Reset()
{
  fTrdPoints->Delete();
  ResetParameters();
}
// ----------------------------------------------------------------------------


// -----   Public method CopyClones   -----------------------------------------
void CbmTrd::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(info) << "CbmTrd: " << nEntries << " entries to add.";
  TClonesArray& clref = *cl2;
  for (Int_t i = 0; i < nEntries; i++) {
    CbmTrdPoint* oldpoint = (CbmTrdPoint*) cl1->At(i);
    Int_t index           = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) CbmTrdPoint(*oldpoint);
    fPosIndex++;
  }
  LOG(info) << "CbmTrd: " << cl2->GetEntriesFast() << " merged entries.";
}
// ----------------------------------------------------------------------------


// -----  ConstructGeometry  --------------------------------------------------
void CbmTrd::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if (fileName.EndsWith(".root")) { ConstructRootGeometry(); }
  else {
    LOG(fatal) << "Geometry format of TRD file " << fileName.Data() << " not supported.";
  }
}
// ----------------------------------------------------------------------------

//__________________________________________________________________________
void CbmTrd::ConstructRootGeometry(TGeoMatrix*)
{
  if (Cbm::GeometryUtils::IsNewGeometryFile(fgeoName)) {
    LOG(info) << "Importing TRD geometry from ROOT file " << fgeoName.Data();
    Cbm::GeometryUtils::ImportRootGeometry(fgeoName, this, fCombiTrans);
  }
  else {
    LOG(info) << "Constructing TRD geometry from ROOT file " << fgeoName.Data();
    FairModule::ConstructRootGeometry();
  }
}

// -----   CheckIfSensitive   -------------------------------------------------
Bool_t CbmTrd::CheckIfSensitive(std::string name)
{
  TString tsname = name;
  if (tsname.EqualTo("gas")) {
    LOG(debug) << "CbmTrd::CheckIfSensitive: Register active volume: " << tsname;
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------

ClassImp(CbmTrd)
