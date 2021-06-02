/** @file CbmTof.cxx
  * @copyright Copyright (C) 2004-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors V. Friese, Christian Simon, Denis Bertini [committer], Florian Uhlig **/

// -------------------------------------------------------------------------
// -----                       CbmTof source file                      -----
// -----                  Created 28/07/04  by V. Friese               -----
// -------------------------------------------------------------------------

#include "CbmTof.h"

#include "CbmGeometryUtils.h"
#include "CbmStack.h"
#include "CbmTofGeoHandler.h"
#include "CbmTofPoint.h"

#include <FairGeoBuilder.h>
#include <FairGeoMedia.h>
#include <FairMCEventHeader.h>
#include <FairRunSim.h>

#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoPhysicalNode.h"
#include "TGeoVolume.h"
#include "TKey.h"
#include "TObjArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include <TFile.h>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <iostream>

using std::cout;
using std::endl;

// -----   Default constructor   -------------------------------------------
CbmTof::CbmTof()
  : FairDetector("TOF", kTRUE, ToIntegralType(ECbmModuleId::kTof))
  , fTrackID(-1)
  , fVolumeID(-1)
  , fPos()
  , fMom()
  , fTime(-1.)
  , fLength(-1.)
  , fELoss(-1.)
  , fPosIndex(0)
  , fTofCollection(new TClonesArray("CbmTofPoint"))
  , fGeoHandler(new CbmTofGeoHandler())
  , fCombiTrans()
  , fVolumeName("")
  , fbOnePointPerTrack(kFALSE)
  , fbIsNewTrack(kFALSE)
  , fTofNodePath("")
  , fCurrentNodePath("")
  , fCurrentModuleType(0)
  , fCurrentModuleIndex(0)
  , fCurrentCounterIndex(0)
  , fActiveCounters()
  , fInactiveCounters()
  , fInactiveCounterIDs()
  , fCountersInBeam()
  , fOutputTreeEntry(0)
  , fbProcessAnyTrack(kFALSE)
  , fbAllCountersInactive(kFALSE)
{
  fVerboseLevel = 1;
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTof::CbmTof(const char* name, Bool_t active)
  : FairDetector(name, active, ToIntegralType(ECbmModuleId::kTof))
  , fTrackID(-1)
  , fVolumeID(-1)
  , fPos()
  , fMom()
  , fTime(-1.)
  , fLength(-1.)
  , fELoss(-1.)
  , fPosIndex(0)
  , fTofCollection(new TClonesArray("CbmTofPoint"))
  , fGeoHandler(new CbmTofGeoHandler())
  , fCombiTrans()
  , fVolumeName("")
  , fbOnePointPerTrack(kFALSE)
  , fbIsNewTrack(kFALSE)
  , fTofNodePath("")
  , fCurrentNodePath("")
  , fCurrentModuleType(0)
  , fCurrentModuleIndex(0)
  , fCurrentCounterIndex(0)
  , fActiveCounters()
  , fInactiveCounters()
  , fInactiveCounterIDs()
  , fCountersInBeam()
  , fOutputTreeEntry(0)
  , fbProcessAnyTrack(kFALSE)
  , fbAllCountersInactive(kFALSE)
{
  fVerboseLevel = 1;
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTof::~CbmTof()
{
  if (fTofCollection) {
    fTofCollection->Delete();
    delete fTofCollection;
  }
  if (fGeoHandler) { delete fGeoHandler; }

  for (auto& CounterInBeam : fCountersInBeam) {
    if ((CounterInBeam.second).second) { delete (CounterInBeam.second).second; }
  }
}
// -------------------------------------------------------------------------

void CbmTof::Initialize()
{
  FairDetector::Initialize();

  // Initialize the CbmTofGeoHandler helper class from the
  // TVirtualMC interface
  Bool_t isSimulation = kTRUE;
  /*Int_t bla =*/fGeoHandler->Init(isSimulation);

  if (fbOnePointPerTrack) {
    for (auto itInactiveCounter = fInactiveCounters.cbegin(); itInactiveCounter != fInactiveCounters.cend();) {
      if (fActiveCounters.find(*itInactiveCounter) != fActiveCounters.end()) {
        itInactiveCounter = fInactiveCounters.erase(itInactiveCounter);
      }
      else {
        // FIXME: Actually, a volume marked insensitive by 'FairModule::CheckifSensitive'
        // should not be called 'FairDetector::ProcessHits' for. But the method
        // 'FairMCApplication::Stepping' calls the latter for all volumes that share
        // the same 'TGeoVolume::fNumber' with any volume marked sensitive. As all
        // "Cell" volumes in the ToF geometry share the same volume number, namely "4",
        // a single cell marked sensitive by 'CheckIfSensitive' results in all ToF
        // cells being marked sensitive in 'FairMCApplication::Stepping'. For this reason,
        // for each call of 'ProcessHits' the counter address needs to be checked against
        // sensitivity.
        CbmTofDetectorInfo tCounterInfo(ECbmModuleId::kTof, std::get<0>(*itInactiveCounter),
                                        std::get<1>(*itInactiveCounter), std::get<2>(*itInactiveCounter), 0, 0);
        fInactiveCounterIDs.emplace(fGeoHandler->GetDetIdPointer()->SetDetectorInfo(tCounterInfo));

        ++itInactiveCounter;
      }
    }
  }
}

void CbmTof::PreTrack()
{
  if (fbOnePointPerTrack) { fbIsNewTrack = kTRUE; }
}

void CbmTof::FinishEvent()
{
  // method is called right before FairRootManager::Fill, once per FairDetector per event

  if (fbOnePointPerTrack) {
    // loop over all MC points (1 point per counter) created by all tracks in the event
    for (Int_t iPoint = 0; iPoint < fTofCollection->GetEntriesFast(); iPoint++) {
      CbmTofPoint* tCurrentPoint = dynamic_cast<CbmTofPoint*>(fTofCollection->At(iPoint));

      // average all point properties except for the energy loss in the gaps
      Int_t iNCells = tCurrentPoint->GetNCells();
      tCurrentPoint->SetTime(tCurrentPoint->GetTime() / iNCells);
      tCurrentPoint->SetLength(tCurrentPoint->GetLength() / iNCells);
      tCurrentPoint->SetX(tCurrentPoint->GetX() / iNCells);
      tCurrentPoint->SetY(tCurrentPoint->GetY() / iNCells);
      tCurrentPoint->SetZ(tCurrentPoint->GetZ() / iNCells);
      tCurrentPoint->SetPx(tCurrentPoint->GetPx() / iNCells);
      tCurrentPoint->SetPy(tCurrentPoint->GetPy() / iNCells);
      tCurrentPoint->SetPz(tCurrentPoint->GetPz() / iNCells);

      if (1 == tCurrentPoint->GetNLinks()) {
        // NOTE: 'FairMultiLinkedData_Interface::GetLink' returns a copy of the 'FairLink' object!
        FairLink tLinkToTrack = tCurrentPoint->GetLink(0);
        tLinkToTrack.SetLink(0, fOutputTreeEntry, tLinkToTrack.GetType(), tLinkToTrack.GetIndex());
        tCurrentPoint->SetLink(tLinkToTrack);
      }

      //      LOG(info)<<Form("ToF point in detector 0x%.8x at time %f ns", tCurrentPoint->GetDetectorID(), tCurrentPoint->GetTime());
    }
  }

  // Extract collision vertex information from the MC event header
  FairMCEventHeader* tEventHeader = FairRunSim::Instance()->GetMCEventHeader();

  Double_t dTargetVertexT = tEventHeader->GetT();  // [s]
  // Define a beam line parallel to Z through the collision vertex and another point shifted along Z
  Double_t dGlobalTargetCoordinates[3]  = {tEventHeader->GetX(), tEventHeader->GetY(), tEventHeader->GetZ()};
  Double_t dGlobalTargetCoordinates1[3] = {tEventHeader->GetX(), tEventHeader->GetY(), tEventHeader->GetZ() + 1.};
  Double_t dGlobalCounterCoordinates[3] = {0., 0., 0.};
  Double_t dLocalTargetCoordinates[3]   = {0., 0., 0.};
  Double_t dLocalTargetCoordinates1[3]  = {0., 0., 0.};
  Double_t dLocalCounterCoordinates[3]  = {0., 0., 0.};

  TGeoPhysicalNode* tCurrentNode(NULL);
  Int_t iModuleType(0);
  Int_t iModuleIndex(0);
  Int_t iCounterIndex(0);
  Int_t iUniqueCounterId(0);
  CbmTofPoint* tBeamPoint(NULL);

  // Loop over all counters that are eligible for beam points
  for (auto const& CounterInBeam : fCountersInBeam) {
    iModuleType   = std::get<0>(CounterInBeam.first);
    iModuleIndex  = std::get<1>(CounterInBeam.first);
    iCounterIndex = std::get<2>(CounterInBeam.first);

    // Create the unique counter ID
    CbmTofDetectorInfo tCounterInfo(ECbmModuleId::kTof, iModuleType, iModuleIndex, iCounterIndex, 0, 0);
    iUniqueCounterId = fGeoHandler->GetDetIdPointer()->SetDetectorInfo(tCounterInfo);

    tCurrentNode = (CounterInBeam.second).second;

    // Transform the two points defining the beam line into the local coordinate system of the counter
    tCurrentNode->GetMatrix()->MasterToLocal(dGlobalTargetCoordinates, dLocalTargetCoordinates);
    tCurrentNode->GetMatrix()->MasterToLocal(dGlobalTargetCoordinates1, dLocalTargetCoordinates1);

    // Calculate the intersection point between the beam line and the counter plane in the local c.s.
    dLocalCounterCoordinates[0] = dLocalTargetCoordinates[0]
                                  - dLocalTargetCoordinates[2]
                                      * (dLocalTargetCoordinates1[0] - dLocalTargetCoordinates[0])
                                      / (dLocalTargetCoordinates1[2] - dLocalTargetCoordinates[2]);
    dLocalCounterCoordinates[1] = dLocalTargetCoordinates[1]
                                  - dLocalTargetCoordinates[2]
                                      * (dLocalTargetCoordinates1[1] - dLocalTargetCoordinates[1])
                                      / (dLocalTargetCoordinates1[2] - dLocalTargetCoordinates[2]);

    // Check if the intersection point is contained in the actual counter volume (i.e. glass plate/gap)
    // If yes, create a CbmTofPoint for the beam traversing the counter
    if (tCurrentNode->GetShape()->Contains(dLocalCounterCoordinates)) {
      // Transform the local beam point into the global coordinate system
      tCurrentNode->GetMatrix()->LocalToMaster(dLocalCounterCoordinates, dGlobalCounterCoordinates);

      // Retrieve the beam momentum in laboratory to calculate the time of flight between the
      // collision vertex in the target and the (global) beam point in the counter
      Double_t dBeamMomentumLab = FairRunSim::Instance()->GetBeamMom();
      Double_t dCounterBeamTime = dTargetVertexT;
      // Distance between the collision vertex point and the beam point in the counter
      // N.B.: The beam line is assumed to be parallel to Z
      Double_t dCounterTargetDistance = dGlobalCounterCoordinates[2] - dGlobalTargetCoordinates[2];

      if (0. < dBeamMomentumLab) {
        Double_t dBeamVelocityLab = dBeamMomentumLab
                                    / TMath::Sqrt(TMath::Power(dBeamMomentumLab, 2.) + TMath::Power(0.938271998, 2.))
                                    * TMath::Ccgs();  // [cm/s]
        dCounterBeamTime += dCounterTargetDistance / dBeamVelocityLab;
      }

      // Create the beam point in the CbmTofPoint collection
      tBeamPoint = new ((*fTofCollection)[fTofCollection->GetEntriesFast()]) CbmTofPoint();
      tBeamPoint->SetDetectorID(iUniqueCounterId);
      tBeamPoint->SetEventID(fOutputTreeEntry);        // set to the exact same value in 'CbmMCPointBuffer::Fill'
      tBeamPoint->SetTime(dCounterBeamTime * 1.0e09);  // [ns]
      tBeamPoint->SetLength(dCounterTargetDistance);
      tBeamPoint->SetX(dGlobalCounterCoordinates[0]);
      tBeamPoint->SetY(dGlobalCounterCoordinates[1]);
      tBeamPoint->SetZ(dGlobalCounterCoordinates[2]);
      tBeamPoint->SetPz(dBeamMomentumLab);
      tBeamPoint->SetNCells(1);
    }
  }

  fOutputTreeEntry++;
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t CbmTof::ProcessHits(FairVolume* /*vol*/)
{
  if (fbOnePointPerTrack) {
    // create/update CbmTofPoint objects for any charged particle or geantinos/rootinos
    if (fbProcessAnyTrack || 0 != gMC->TrackCharge() || 0 == gMC->TrackPid()) {
      Int_t iCounterID = fGeoHandler->GetUniqueCounterId();

      // If the current volume is marked insensitive, do not process the MC hit.
      if (fInactiveCounterIDs.find(iCounterID) != fInactiveCounterIDs.end()) { return kTRUE; }

      Int_t iTrackID = gMC->GetStack()->GetCurrentTrackNumber();

      Double_t dTrackEnergyDeposit = gMC->Edep();

      CbmTofPoint* tCounterPoint(0);
      Bool_t bCounterPointExists = kFALSE;

      // scan the MC point array only if an existing point may be found
      if (!fbIsNewTrack) {
        // loop over all MC points (1 point per counter) created by all tracks in the event so far
        // in reverse order to find the proper MC point immediately
        for (Int_t iPoint = fTofCollection->GetEntriesFast() - 1; iPoint >= 0; iPoint--) {
          tCounterPoint = dynamic_cast<CbmTofPoint*>(fTofCollection->At(iPoint));

          if (tCounterPoint->GetDetectorID() == iCounterID && tCounterPoint->GetTrackID() == iTrackID) {
            bCounterPointExists = kTRUE;
            break;
          }
        }
      }
      else {
        fbIsNewTrack = kFALSE;
      }

      // first step of the track in the current gas gap (cell)
      if (gMC->IsTrackEntering()) {
        Double_t dTrackTime   = gMC->TrackTime() * 1.0e09;
        Double_t dTrackLength = gMC->TrackLength();
        Double_t dTrackPositionX(0.);
        Double_t dTrackPositionY(0.);
        Double_t dTrackPositionZ(0.);
        gMC->TrackPosition(dTrackPositionX, dTrackPositionY, dTrackPositionZ);
        Double_t dTrackMomentumX(0.);
        Double_t dTrackMomentumY(0.);
        Double_t dTrackMomentumZ(0.);
        Double_t dTrackEnergy(0.);
        gMC->TrackMomentum(dTrackMomentumX, dTrackMomentumY, dTrackMomentumZ, dTrackEnergy);

        if (bCounterPointExists) {
          tCounterPoint->SetTime(tCounterPoint->GetTime() + dTrackTime);
          tCounterPoint->SetLength(tCounterPoint->GetLength() + dTrackLength);
          tCounterPoint->SetEnergyLoss(tCounterPoint->GetEnergyLoss() + dTrackEnergyDeposit);
          tCounterPoint->SetX(tCounterPoint->GetX() + dTrackPositionX);
          tCounterPoint->SetY(tCounterPoint->GetY() + dTrackPositionY);
          tCounterPoint->SetZ(tCounterPoint->GetZ() + dTrackPositionZ);
          tCounterPoint->SetPx(tCounterPoint->GetPx() + dTrackMomentumX);
          tCounterPoint->SetPy(tCounterPoint->GetPy() + dTrackMomentumY);
          tCounterPoint->SetPz(tCounterPoint->GetPz() + dTrackMomentumZ);
          tCounterPoint->SetNCells(tCounterPoint->GetNCells() + 1);
        }
        else {
          tCounterPoint = new ((*fTofCollection)[fTofCollection->GetEntriesFast()]) CbmTofPoint();
          tCounterPoint->SetTrackID(iTrackID);
          tCounterPoint->SetDetectorID(iCounterID);
          tCounterPoint->SetEventID(fOutputTreeEntry);  // set to the exact same value in 'CbmMCPointBuffer::Fill'

          tCounterPoint->SetTime(dTrackTime);
          tCounterPoint->SetLength(dTrackLength);
          tCounterPoint->SetEnergyLoss(dTrackEnergyDeposit);
          tCounterPoint->SetX(dTrackPositionX);
          tCounterPoint->SetY(dTrackPositionY);
          tCounterPoint->SetZ(dTrackPositionZ);
          tCounterPoint->SetPx(dTrackMomentumX);
          tCounterPoint->SetPy(dTrackMomentumY);
          tCounterPoint->SetPz(dTrackMomentumZ);
          tCounterPoint->SetNCells(1);

          // Increment number of tof points for TParticle
          CbmStack* stack = dynamic_cast<CbmStack*>(gMC->GetStack());
          stack->AddPoint(ECbmModuleId::kTof);
        }

        tCounterPoint->SetGap(fGeoHandler->GetGap(iCounterID));
      }
      else {
        tCounterPoint->SetEnergyLoss(tCounterPoint->GetEnergyLoss() + dTrackEnergyDeposit);
      }
    }
  }
  else {
    // Set parameters at entrance of volume. Reset ELoss.
    if (gMC->IsTrackEntering()) {
      fELoss  = 0.;
      fTime   = gMC->TrackTime() * 1.0e09;
      fLength = gMC->TrackLength();
      gMC->TrackPosition(fPos);
      gMC->TrackMomentum(fMom);
    }

    // Sum energy loss for all steps in the active volume
    fELoss += gMC->Edep();

    // Create CbmTofPoint at exit of active volume
    if (((0 == gMC->GetStack()->GetCurrentTrack()->GetPdgCode()) ||  // Add geantinos/rootinos
         (gMC->TrackCharge() != 0))
        && (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared())) {

      fTrackID = gMC->GetStack()->GetCurrentTrackNumber();

      fVolumeID = fGeoHandler->GetUniqueDetectorId();

      LOG(debug2) << "CbmTof::TID: " << fTrackID;
      LOG(debug2) << " TofVol: " << fVolumeID;
      LOG(debug2) << " DetSys: " << fGeoHandler->GetDetSystemId(fVolumeID);
      LOG(debug2) << " SMtype: " << fGeoHandler->GetSMType(fVolumeID);
      LOG(debug2) << " SModule: " << fGeoHandler->GetSModule(fVolumeID);
      LOG(debug2) << " Counter: " << fGeoHandler->GetCounter(fVolumeID);
      LOG(debug2) << " Gap: " << fGeoHandler->GetGap(fVolumeID);
      LOG(debug2) << " Cell: " << fGeoHandler->GetCell(fVolumeID);
      LOG(debug2) << Form(" x: %6.2f", fPos.X());
      LOG(debug2) << Form(" y: %6.2f", fPos.Y());
      LOG(debug2) << Form(" z: %6.2f", fPos.Z());
      //   LOG(debug2)<<"Region: "<<fGeoHandler->GetRegion(fVolumeID);
      //   LOG(debug2)<<"*************";

      //fVolumeID = ((region-1)<<24) + ((module-1)<<14) + ((cell-1)<<4) + (gap-1);

      AddHit(fTrackID, fVolumeID, TVector3(fPos.X(), fPos.Y(), fPos.Z()), TVector3(fMom.Px(), fMom.Py(), fMom.Pz()),
             fTime, fLength, fELoss);

      // Increment number of tof points for TParticle
      CbmStack* stack = (CbmStack*) gMC->GetStack();
      stack->AddPoint(ECbmModuleId::kTof);

      ResetParameters();
    }
  }

  return kTRUE;
}
// -------------------------------------------------------------------------


// -----   Public method EndOfEvent   --------------------------------------
void CbmTof::EndOfEvent()
{
  if (fVerboseLevel) Print();
  fTofCollection->Delete();
  fPosIndex = 0;
}
// -------------------------------------------------------------------------


// -----   Public method Register   ----------------------------------------
void CbmTof::Register() { FairRootManager::Instance()->Register("TofPoint", "Tof", fTofCollection, kTRUE); }
// -------------------------------------------------------------------------


// -----   Public method GetCollection   -----------------------------------
TClonesArray* CbmTof::GetCollection(Int_t iColl) const
{
  if (iColl == 0) return fTofCollection;
  else
    return NULL;
}
// -------------------------------------------------------------------------


// -----   Public method Print   -------------------------------------------
void CbmTof::Print(Option_t*) const
{
  Int_t nHits = fTofCollection->GetEntriesFast();
  LOG(info) << fName << ": " << nHits << " points registered in this event.";

  if (fVerboseLevel > 1)
    for (Int_t i = 0; i < nHits; i++)
      (*fTofCollection)[i]->Print();
}
// -------------------------------------------------------------------------


// -----   Public method Reset   -------------------------------------------
void CbmTof::Reset()
{
  fTofCollection->Delete();
  ResetParameters();
}
// -------------------------------------------------------------------------


// -----   Public method CopyClones   --------------------------------------
void CbmTof::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(info) << "CbmTof: " << nEntries << " entries to add.";
  TClonesArray& clref   = *cl2;
  CbmTofPoint* oldpoint = NULL;
  for (Int_t i = 0; i < nEntries; i++) {
    oldpoint    = (CbmTofPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) CbmTofPoint(*oldpoint);
    fPosIndex++;
  }
  LOG(info) << "CbmTof: " << cl2->GetEntriesFast() << " merged entries.";
}
// -------------------------------------------------------------------------
void CbmTof::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if (fileName.EndsWith(".root")) {
    ConstructRootGeometry();
    // Create TGeoPhysicalNode objects for those counters eligible for beam points
    CreateInBeamNodes();
  }
  else {
    LOG(fatal) << "Geometry format " << fileName << " not supported.";
  }
}

void CbmTof::ConstructRootGeometry(TGeoMatrix*)
{
  if (Cbm::GeometryUtils::IsNewGeometryFile(fgeoName)) {
    LOG(info) << "Importing TOF geometry from ROOT file " << fgeoName.Data();
    Cbm::GeometryUtils::ImportRootGeometry(fgeoName, this, fCombiTrans);
  }
  else {
    LOG(info) << "Constructing TOF geometry from ROOT file " << fgeoName.Data();
    FairModule::ConstructRootGeometry();
  }
}

// -----   Public method SetCounterActive   --------------------------------
void CbmTof::SetCounterActive(Int_t iModuleType, Int_t iModuleIndex, Int_t iCounterIndex)
{
  fActiveCounters.emplace(std::make_tuple(iModuleType, iModuleIndex, iCounterIndex));
}
// -------------------------------------------------------------------------


// -----   Public method SetCounterInactive   ------------------------------
void CbmTof::SetCounterInactive(Int_t iModuleType, Int_t iModuleIndex, Int_t iCounterIndex)
{
  fInactiveCounters.emplace(std::make_tuple(iModuleType, iModuleIndex, iCounterIndex));
}
// -------------------------------------------------------------------------


// -----   Public method SetCounterInBeam   --------------------------------
void CbmTof::SetCounterInBeam(Int_t iModuleType, Int_t iModuleIndex, Int_t iCounterIndex)
{
  fCountersInBeam[std::make_tuple(iModuleType, iModuleIndex, iCounterIndex)];
}
// -------------------------------------------------------------------------


// -----   Private method CreateInBeamNodes   ------------------------------
void CbmTof::CreateInBeamNodes()
{
  for (auto& CounterInBeam : fCountersInBeam) {
    (CounterInBeam.second).second = new TGeoPhysicalNode(((CounterInBeam.second).first).Data());
  }
}
// -------------------------------------------------------------------------

// -----   Private method CheckIfSensitive   -------------------------------
Bool_t CbmTof::CheckIfSensitive(std::string name)
{
  // If the current Cell volume belongs to a counter declared inactive w.r.t.
  // Monte Carlo point creation, it is not declared sensitive
  TString tsname = name;
  if (tsname.Contains("Cell")) {
    // FIXME: This does not work at the moment (see comment in the 'Initialize' method).
    for (auto const& InactiveCounter : fInactiveCounters) {
      if (std::get<0>(InactiveCounter) == fCurrentModuleType && std::get<1>(InactiveCounter) == fCurrentModuleIndex
          && std::get<2>(InactiveCounter) == fCurrentCounterIndex) {
        return kFALSE;
      }
    }

    return kTRUE;
  }
  return kFALSE;
}
// -------------------------------------------------------------------------


// -----   Private method AddHit   -----------------------------------------
CbmTofPoint* CbmTof::AddHit(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom, Double_t time, Double_t length,
                            Double_t eLoss)
{
  TClonesArray& clref = *fTofCollection;
  Int_t size          = clref.GetEntriesFast();
  return new (clref[size]) CbmTofPoint(trackID, detID, pos, mom, time, length, eLoss);
}
// -------------------------------------------------------------------------

ClassImp(CbmTof)
