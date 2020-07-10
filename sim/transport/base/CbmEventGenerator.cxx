/** @file CbmPrimaryGenerator.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 29 July 2019
 **/

#include "CbmEventGenerator.h"

#include "CbmBeam.h"
#include "FairGenericStack.h"
#include "FairLogger.h"
#include "FairMCEventHeader.h"
#include "TMath.h"
#include "TRandom.h"
#include "TVector3.h"
#include <cassert>


// -----   Constructor   ---------------------------------------------------
CbmEventGenerator::CbmEventGenerator()
  : FairPrimaryGenerator()
  , fBeamProfile()
  , fTarget()
  , fForceVertexInTarget(kTRUE) {
  // Mother class members
  fName      = "EventGenerator";
  fBeamAngle = kTRUE;
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmEventGenerator::~CbmEventGenerator() {}
// -------------------------------------------------------------------------


// -----   Generate the input event   --------------------------------------
Bool_t CbmEventGenerator::GenerateEvent(FairGenericStack* stack) {

  std::cout << std::endl;
  LOG(info) << GetName() << ": Generate event " << ++fEventNr;

  // An MCEventHeader must be present
  assert(fEvent);

  // Set the stack
  assert(stack);
  fStack = stack;

  // Reset MCEventHeader and track counter
  fEvent->Reset();
  fNTracks = 0;

  // Generate the event vertex and the beam angle
  MakeVertex();

  // Generate the event plane angle
  if (fEventPlane) {
    MakeEventPlane();
    LOG(info) << GetName() << ": Rotate event by " << fPhi << " rad";
  }

  // Call the registered generator classes
  fListIter->Reset();
  TObject* item = nullptr;
  while ((item = fListIter->Next())) {
    FairGenerator* generator = dynamic_cast<FairGenerator*>(item);
    assert(generator);
    fMCIndexOffset = fNTracks;  // Number of tracks before call of generator
    if (!generator->ReadEvent(this)) {
      LOG(info) << GetName() << ": ReadEvent failed for generator "
                << generator->GetName();
      return kFALSE;
    }
  }

  // Update event header
  if (!fEvent->IsSet()) fEvent->SetEventID(fEventNr);
  fEvent->SetNPrim(fNTracks);
  fEvent->SetVertex(fVertex);
  fEvent->SetRotX(fBeamAngleX);
  fEvent->SetRotY(fBeamAngleY);
  Double_t phiGen = fEvent->GetRotZ();  // from concrete generators
  fEvent->SetRotZ(phiGen + fPhi);       // total event plane angle

  // Event info to screen
  LOG(info) << GetName() << ": Event ID " << fEvent->GetEventID() << ", tracks "
            << fNTracks << ", vertex (" << fVertex.X() << ", " << fVertex.Y()
            << ", " << fVertex.Z() << ") cm";
  LOG(info) << GetName() << ": Beam angle (" << fBeamAngleX << ", "
            << fBeamAngleY << ") rad, event plane angle " << fPhi
            << " rad (total " << fEvent->GetRotZ() << " rad)";

  // Update global track counter (who knows what it's good for ...)
  fTotPrim += fNTracks;

  return kTRUE;
}
// -------------------------------------------------------------------------


// -----   Generate event vertex   -----------------------------------------
void CbmEventGenerator::MakeVertex() {

  if (fForceVertexInTarget && fTarget)
    MakeVertexInTarget();
  else
    MakeVertexInFocalPlane();
}
// -------------------------------------------------------------------------


// -----   Generate event vertex in the beam focal plane   -----------------
void CbmEventGenerator::MakeVertexInFocalPlane() {

  std::unique_ptr<CbmBeam> beam;
  beam           = fBeamProfile.GenerateBeam();
  fVertex        = beam->GetPosition();
  fBeamAngleX    = beam->GetThetaX();
  fBeamAngleY    = beam->GetThetaY();
  fBeamDirection = beam->GetDirection();
}
// -------------------------------------------------------------------------


// -----   Generate event vertex   -----------------------------------------
void CbmEventGenerator::MakeVertexInTarget() {

  assert(fTarget);

  // Target surface centres and normal vector
  TVector3 surf1 = fTarget->GetSurfaceCentreUp();
  TVector3 surf2 = fTarget->GetSurfaceCentreDown();
  TVector3 norm  = fTarget->GetNormal();

  // Declare some vectors
  TVector3 point1;  // Intersection of beam with first target surface
  TVector3 point2;  // Intersection of beam with second target surface

  // It is possible that a generated beam trajectory misses the target.
  // In that case, the beam will be re-sampled until it hits the target.
  Bool_t isInTarget = kFALSE;
  Int_t nSamples    = 0;
  std::unique_ptr<CbmBeam> beam;
  while (!isInTarget) {

    // Abort if too many beam samples
    if (nSamples > 1000.)
      LOG(fatal) << GetName() << ": Aborting after " << nSamples
                 << " beam samplings. Adjust beam and target.";

    // Sample a beam trajectory
    beam = fBeamProfile.GenerateBeam();
    nSamples++;

    // Intersections of beam trajectory with target surfaces
    point1 = beam->ExtrapolateToPlane(surf1, norm);
    point2 = beam->ExtrapolateToPlane(surf2, norm);

    // Check whether both intersections are inside the target
    // (i.e., the beam crosses both target surfaces)
    Bool_t check1 = ((point1 - surf1).Mag() < 0.5 * fTarget->GetDiameter());
    Bool_t check2 = ((point2 - surf2).Mag() < 0.5 * fTarget->GetDiameter());
    isInTarget    = check1 && check2;

  }  //? Beam not in target

  LOG(debug) << beam->ToString() << ", generated after " << nSamples
             << (nSamples > 1 ? " samplings: " : " sampling");

  // Sample a point between entry and exit of beam in target
  Double_t scale = 0.5;
  if (fSmearVertexZ) scale = gRandom->Uniform();
  fVertex = point1 + scale * (point2 - point1);

  // Set the beam angles (member variables of base class)
  fBeamAngleX    = beam->GetThetaX();
  fBeamAngleY    = beam->GetThetaY();
  fBeamDirection = beam->GetDirection();
}
// -------------------------------------------------------------------------


// -----   Info   ----------------------------------------------------------
void CbmEventGenerator::Print(Option_t*) const {

  LOG(info) << fBeamProfile.ToString();
  if (fTarget) LOG(info) << fTarget->ToString();
  LOG(info) << "Vertex smearing along beam " << (fSmearVertexZ ? "ON" : "OFF");
  if (fEventPlane)
    LOG(info) << "Random event plane angle between " << fPhiMin << " and "
              << fPhiMax << " rad";
  else
    LOG(info) << "Fixed event plane angle = 0";
  LOG(info) << "Number of generators = " << fGenList->GetEntries();
}
// -------------------------------------------------------------------------


// -----   Set beam angle   ------------------------------------------------
void CbmEventGenerator::SetBeamAngle(Double_t meanThetaX,
                                     Double_t meanThetaY,
                                     Double_t sigmaThetaX,
                                     Double_t sigmaThetaY) {
  fBeamProfile.SetAngle(meanThetaX, meanThetaY, sigmaThetaX, sigmaThetaY);
}
// -------------------------------------------------------------------------


// -----   Set beam position   ---------------------------------------------
void CbmEventGenerator::SetBeamPosition(Double_t meanX,
                                        Double_t meanY,
                                        Double_t sigmaX,
                                        Double_t sigmaY,
                                        Double_t zF) {
  fBeamProfile.SetPosition(meanX, meanY, sigmaX, sigmaY, zF);
}
// -------------------------------------------------------------------------


ClassImp(CbmEventGenerator)
