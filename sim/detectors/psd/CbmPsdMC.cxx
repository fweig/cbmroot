/** @file CbmPsdMC.cxx
 ** @since 04.02.2005
 ** @author Alla Maevskaya <a.maevskaya@gsi.de>
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 18.09.2017 Major revision: Use geometry file instead of hard-coded geometry.
 **
 ** Former CbmPsdV1.cxx
 **/


#include "CbmPsdMC.h"

#include "CbmGeometryUtils.h"
#include "CbmModuleList.h"
#include "CbmPsdPoint.h"
#include "CbmStack.h"

#include "TGeoManager.h"
#include "TKey.h"
#include "TVirtualMC.h"

#include <cassert>
#include <string>


// -----   Default constructor   -------------------------------------------
CbmPsdMC::CbmPsdMC(Bool_t active, const char* name)
  : FairDetector(name, active, ToIntegralType(ECbmModuleId::kPsd))
  , fPosX(0.)
  , fPosZ(0.)
  , fRotY(0.)
  , fUserPlacement(kFALSE)
  , fPsdPoints(new TClonesArray("CbmPsdPoint"))
  , fTrackID(-3)
  , fAddress(-3)
  , fPos()
  , fMom()
  , fTime(-1.)
  , fLength(-1.)
  , fEloss(-1.)
  , fLayerID(-1)
  , fModuleID(-1)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmPsdMC::~CbmPsdMC()
{
  if (fPsdPoints) {
    fPsdPoints->Delete();
    delete fPsdPoints;
  }
}
// -------------------------------------------------------------------------


// -----   Construct the geometry from file   ------------------------------
void CbmPsdMC::ConstructGeometry()
{


  /*
  LOG(info) << GetName() << ": Constructing geometry from file "
            << fgeoName;

  // --- A TGeoManager should be present
  assert(gGeoManager);

  // --- Only ROOT geometries are supported
  if (  ! GetGeometryFileName().EndsWith(".root") ) {
    LOG(fatal) <<  GetName() << ": Geometry format of file "
               << GetGeometryFileName() << " not supported.";
    return;
  }

  // --- Look for PSD volume and transformation matrix in geometry file
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* geoFile = new TFile(fgeoName);
  assert ( geoFile );
  TKey* key = nullptr;
  TIter keyIter(geoFile->GetListOfKeys());
  Bool_t foundVolume = kFALSE;
  Bool_t foundMatrix = kFALSE;
  std::string volumeName = "";
  TGeoMatrix* transformation = nullptr;

  while ( (key = (TKey*)keyIter() ) ) {

    if ( key->ReadObj()->InheritsFrom("TGeoVolume") ) {
      if ( foundVolume) {
        LOG(fatal) << GetName() << ": More than one TGeoVolume in file! "
                   << volumeName << " " << key->GetName();
        break;
      } //? already found a volume
      volumeName = key->GetName();
      foundVolume = kTRUE;
      continue;
    } //? key inherits from TGeoVolume

    if ( key->ReadObj()->InheritsFrom("TGeoMatrix") ) {
      if ( foundMatrix ) {
        LOG(fatal) << GetName() << ": More than one TGeoMatrix in file! ";
        break;
      } //? already found a matrix
      transformation = dynamic_cast<TGeoMatrix*>(key->ReadObj());
      foundMatrix = kTRUE;
      continue;
    } //? key inherits from TGeoMatrix

  } //# keys in file

  if ( ! foundVolume ) LOG(fatal) << GetName() << ": No TGeoVolume in file " << fgeoName;
  if ( ! foundMatrix ) LOG(fatal) << GetName() << ": No TGeoMatrix in file " << fgeoName;

  // --- Import PSD volume
  TGeoVolume* psdVolume = TGeoVolume::Import(fgeoName, volumeName.c_str());

  // Add PSD to the geometry
  gGeoManager->GetTopVolume()->AddNode(psdVolume, 0, transformation);
  if (air::Logger::Logging(fair::Severity::debug)) {
    psdVolume->Print();
    transformation->Print();
  }

  // Register all sensitive volumes
  for (Int_t i=0; i<psdVolume->GetNdaughters(); ++i)
    RegisterSensitiveVolumes(psdVolume->GetNode(i));
//   RegisterSensitiveVolumes(psdVolume->GetNode(0));

  LOG(debug) << GetName() << ": " << fNbOfSensitiveVol
             << " sensitive volumes";
             *
  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  geoFile->Close();
*/

  LOG(info) << "Importing PSD geometry from ROOT file " << fgeoName.Data();
  Cbm::GeometryUtils::ImportRootGeometry(fgeoName, this);
}
// -------------------------------------------------------------------------


// -----   End of event action   -------------------------------------------
void CbmPsdMC::EndOfEvent()
{
  Print();  // Status output
  fPsdPoints->Delete();
}
// -------------------------------------------------------------------------


// -----   Print   ---------------------------------------------------------
void CbmPsdMC::Print(Option_t*) const
{
  LOG(info) << fName << ": " << fPsdPoints->GetEntriesFast() << " points registered in this event.";
}
// -------------------------------------------------------------------------


// -----   Public method ProcessHits  --------------------------------------
Bool_t CbmPsdMC::ProcessHits(FairVolume*)
{

  // No action for neutral particles
  if (TMath::Abs(gMC->TrackCharge()) <= 0) return kFALSE;

  // --- If this is the first step for the track in the volume:
  //     Reset energy loss and store track parameters
  if (gMC->IsTrackEntering()) {
    fTrackID = gMC->GetStack()->GetCurrentTrackNumber();

    gMC->CurrentVolOffID(1, fLayerID);
    gMC->CurrentVolOffID(3, fModuleID);

    fAddress = fLayerID;
    gMC->TrackPosition(fPos);
    gMC->TrackMomentum(fMom);
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    fEloss  = 0.;
  }  //? track entering

  // --- For all steps within active volume: sum up differential energy loss
  fEloss += gMC->Edep();

  // --- If track is leaving: get track parameters and create CbmstsPoint
  if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared()) {

    // Create CbmPsdPoint
    Int_t size = fPsdPoints->GetEntriesFast();
    CbmPsdPoint* psdPoint =
      new ((*fPsdPoints)[size]) CbmPsdPoint(fTrackID, fAddress, fPos.Vect(), fMom.Vect(), fTime, fLength, fEloss);
    psdPoint->SetModuleID(fModuleID + 1);

    // --- Increment number of PsdPoints for this track in the stack
    CbmStack* stack = dynamic_cast<CbmStack*>(gMC->GetStack());
    assert(stack);
    stack->AddPoint(ECbmModuleId::kPsd);

  }  //? track is exiting or stopped

  return kTRUE;
}
// -------------------------------------------------------------------------


// -----   Register the sensitive volumes   --------------------------------
void CbmPsdMC::RegisterSensitiveVolumes(TGeoNode* node)
{

  TObjArray* daughters = node->GetVolume()->GetNodes();
  for (Int_t iDaughter = 0; iDaughter < daughters->GetEntriesFast(); iDaughter++) {
    TGeoNode* daughter = dynamic_cast<TGeoNode*>(daughters->At(iDaughter));
    assert(daughter);
    if (daughter->GetNdaughters() > 0) RegisterSensitiveVolumes(daughter);
    TGeoVolume* daughterVolume = daughter->GetVolume();
    if (CheckIfSensitive(daughterVolume->GetName())) { AddSensitiveVolume(daughterVolume); }  //? Sensitive volume
  }                                                                                           //# Daughter nodes
}
// -------------------------------------------------------------------------


ClassImp(CbmPsdMC)
