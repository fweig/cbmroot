/** @file CbmMvd.cxx
  * @copyright Copyright (C) 2004-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer], Florian Uhlig, Philipp Sitzmann **/

// -------------------------------------------------------------------------
// -----                        CbmMvd source file                     -----
// -----                  Created 26/07/04  by V. Friese               -----
// -------------------------------------------------------------------------

#include "CbmMvd.h"

#include "CbmMvdPoint.h"
#include "CbmStack.h"
#include "tools/CbmMvdGeoHandler.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"

#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TList.h"
#include "TObjArray.h"
#include "TParticle.h"
#include "TString.h"
#include "TVirtualMC.h"
#include "TVirtualMCStack.h"

#include <iostream>
using std::cout;
using std::endl;


// -----   Default constructor   -------------------------------------------
CbmMvd::CbmMvd()
  //  : CbmMvd("MVD", kTRUE, kMvd)
  : FairDetector("MVD", kTRUE, ToIntegralType(ECbmModuleId::kMvd))
  , fTrackID(0)
  , fPdg(0)
  , fVolumeID(0)
  , fPosIn(0.0, 0.0, 0.0, 0.0)
  , fPosOut(0.0, 0.0, 0.0, 0.0)
  , fMomIn(0.0, 0.0, 0.0, 0.0)
  , fMomOut(0.0, 0.0, 0.0, 0.0)
  , fTime(0.)
  , fLength(0.)
  , fELoss(0.)
  , fPosIndex(0)
  , fCollection(new TClonesArray("CbmMvdPoint"))
  , kGeoSaved(kFALSE)
  , fGeoPar(new TList())
  , fStationMap()
  , fmvdHandler(nullptr)
{
  ResetParameters();
  fGeoPar->SetName(GetName());
  fVerboseLevel = 1;
  fmvdHandler   = new CbmMvdGeoHandler();
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMvd::CbmMvd(const char* name, Bool_t active)
  : FairDetector(name, active, ToIntegralType(ECbmModuleId::kMvd))
  , fTrackID(0)
  , fPdg(0)
  , fVolumeID(0)
  , fPosIn(0.0, 0.0, 0.0, 0.0)
  , fPosOut(0.0, 0.0, 0.0, 0.0)
  , fMomIn(0.0, 0.0, 0.0, 0.0)
  , fMomOut(0.0, 0.0, 0.0, 0.0)
  , fTime(0.)
  , fLength(0.)
  , fELoss(0.)
  , fPosIndex(0)
  , fCollection(new TClonesArray("CbmMvdPoint"))
  , kGeoSaved(kFALSE)
  , fGeoPar(new TList())
  , fStationMap()
  , fmvdHandler(nullptr)
{
  fGeoPar->SetName(GetName());
  fVerboseLevel = 1;
  fmvdHandler   = new CbmMvdGeoHandler();
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMvd::~CbmMvd()
{
  if (fGeoPar) {
    fGeoPar->Delete();
    delete fGeoPar;
  }
  if (fCollection) {
    fCollection->Delete();
    delete fCollection;
  }
}
// -------------------------------------------------------------------------


// -----   Virtual public method ProcessHits  ------------------------------
Bool_t CbmMvd::ProcessHits(FairVolume* vol)
{


  // Store track parameters at entrance of sensitive volume
  if (gMC->IsTrackEntering()) {
    fPdg    = gMC->TrackPid();
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Set additional parameters at exit of active volume. Create CbmMvdPoint.
  if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared()) {
    fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
    const char* address = gMC->CurrentVolPath();

    TString stAdd(address);

    if (stAdd.Contains("/MVDscripted_0")) { fVolumeID = fmvdHandler->GetIDfromPath(stAdd); }
    else {
      fVolumeID = vol->getMCid();
    }
    if (fELoss == 0.) return kFALSE;
    AddHit(fTrackID, fPdg, fStationMap[fVolumeID], TVector3(fPosIn.X(), fPosIn.Y(), fPosIn.Z()),
           TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()), TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
           TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()), fTime, fLength, fELoss);

    // Increment number of MvdPoints for this track
    CbmStack* stack = (CbmStack*) gMC->GetStack();
    stack->AddPoint(ECbmModuleId::kMvd);

    ResetParameters();
  }

  return kTRUE;
}
// -------------------------------------------------------------------------


// -----   Public method BeginEvent   --------------------------------------
void CbmMvd::BeginEvent() {}
// -------------------------------------------------------------------------


// -----   Virtual public method EndOfEvent   ------------------------------
void CbmMvd::EndOfEvent()
{
  if (fVerboseLevel) Print();
  //  fCollection->Clear();
  fCollection->Delete();
  ResetParameters();
}
// -------------------------------------------------------------------------


// -----   Virtual public method Register   --------------------------------
void CbmMvd::Register() { FairRootManager::Instance()->Register("MvdPoint", GetName(), fCollection, kTRUE); }
// -------------------------------------------------------------------------


// -----   Virtual public method GetCollection   ---------------------------
TClonesArray* CbmMvd::GetCollection(Int_t iColl) const
{
  if (iColl == 0) return fCollection;
  else
    return NULL;
}
// -------------------------------------------------------------------------


// -----   Virtual public method Print   -----------------------------------
void CbmMvd::Print(Option_t*) const
{
  Int_t nHits = fCollection->GetEntriesFast();
  LOG(info) << fName << ": " << nHits << " points registered in this event.";
}
// -------------------------------------------------------------------------


// -----   Virtual public method Reset   -----------------------------------
void CbmMvd::Reset()
{
  //  fCollection->Clear();
  fCollection->Delete();
  ResetParameters();
}
// -------------------------------------------------------------------------


// -----   Virtual public method CopyClones   ------------------------------
void CbmMvd::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(info) << "CbmMvd: " << nEntries << " entries to add.";
  TClonesArray& clref   = *cl2;
  CbmMvdPoint* oldpoint = NULL;
  for (Int_t i = 0; i < nEntries; i++) {
    oldpoint    = (CbmMvdPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) CbmMvdPoint(*oldpoint);
    fPosIndex++;
  }
  LOG(info) << "CbmMvd: " << cl2->GetEntriesFast() << " merged entries.";
}
// -------------------------------------------------------------------------

// --------Pulic method ConstructGeometry()-----------------------------------------------------------------
void CbmMvd::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if (fileName.EndsWith(".root")) {
    LOG(info) << "Constructing MVD  geometry from ROOT  file " << fileName.Data();
    ConstructRootGeometry();
  }
  else if (fileName.EndsWith(".geo")) {
    LOG(fatal) << "Don't use old .geo style geometrys for the MVD. Please use "
                  "a .root geometry";
  }
  else
    LOG(fatal) << "Geometry format of MVD file " << fileName.Data() << " not supported.";
}


// -----   Virtual public method ConstructAsciiGeometry   -----------------------
void CbmMvd::ConstructAsciiGeometry() {}
// -------------------------------------------------------------------------

// --------    Public method ConstructRootGeometry     ---------------------
void CbmMvd::ConstructRootGeometry(TGeoMatrix*)  // added 05.05.14 by P. Sitzmann
{
  FairDetector::ConstructRootGeometry();
  fmvdHandler->Init(kTRUE);
  fmvdHandler->Fill();
  fStationMap = fmvdHandler->GetMap();
  if (fStationMap.size() == 0) LOG(fatal) << "Tried to load MVD Geometry, but didn't succeed to load Sensors";
  LOG(debug) << "filled mvd StationMap with: " << fStationMap.size() << " new Sensors";
}
// -------------------------------------------------------------------------


// -----   Private method AddHit   --------------------------------------------
CbmMvdPoint* CbmMvd::AddHit(Int_t trackID, Int_t pdg, Int_t sensorNr, TVector3 posIn, TVector3 posOut, TVector3 momIn,
                            TVector3 momOut, Double_t time, Double_t length, Double_t eLoss)
{
  TClonesArray& clref = *fCollection;
  Int_t size          = clref.GetEntriesFast();

  LOG(debug2) << "CbmMvd: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z() << ") cm,  sensor "
              << sensorNr << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV";
  return new (clref[size]) CbmMvdPoint(trackID, pdg, sensorNr, posIn, posOut, momIn, momOut, time, length, eLoss);
}
// ----------------------------------------------------------------------------

Bool_t CbmMvd::CheckIfSensitive(std::string name)
{
  TString tsname = name;
  if (tsname.Contains("sensorActive") || tsname.Contains("MimosaActive")
      || (tsname.Contains("mvdstation") && !(tsname.Contains("PartAss")))) {
    LOG(debug) << "*** Register " << tsname << " as active volume.";
    return kTRUE;
  }
  else if (tsname.EndsWith("-P0")) {
    // if(fVerboseLevel>1)
    LOG(debug) << "*** Register " << tsname << " as active volume.";

    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------


ClassImp(CbmMvd)
