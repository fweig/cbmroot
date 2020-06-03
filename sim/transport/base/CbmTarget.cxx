/** @file CbmTarget.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 08.10.2013
 **/

#include "CbmTarget.h"

#include <sstream>
#include "TGeoManager.h"
#include "TGeoMatrix.h"

#define RESET "\033[0m"
#define RED   "\033[1m\033[31m"
#define GREEN "\033[32m"


// -----   Default constructor   --------------------------------------------
CbmTarget::CbmTarget() :
	FairModule("Target", "CBM target"),
	fZ(0),
	fThickness(0.),
	fDiameter(0.),
	fDensity(0.),
	fPosX(0.), fPosY(0.), fPosZ(0.),
	fRotY(0.),
	fMaterial(""),
	fBuildFromFile(kFALSE) {
}
// --------------------------------------------------------------------------



// -----   Constructor with file name   -------------------------------------
CbmTarget::CbmTarget(const char* fileName) :
  FairModule("Target", "CBM target"),
	fZ(0.),
	fThickness(0.),
	fDiameter(0.),
	fDensity(0.),
	fPosX(0.), fPosY(0.), fPosZ(0.),
	fRotY(0.),
	fMaterial(""),
	fBuildFromFile(kTRUE) {
	SetGeometryFileName(fileName);
}
// --------------------------------------------------------------------------



// -----   Constructor with properties   ------------------------------------
CbmTarget::CbmTarget(const char* element, Double_t thickness,
										 Double_t diameter, Double_t density) :
  FairModule("Target", "CBM target"),
	fZ(0),
	fThickness(thickness),
	fDiameter(diameter),
	fDensity(density),
	fPosX(0.), fPosY(0.), fPosZ(0.),
	fRotY(0.),
	fMaterial(element),
	fBuildFromFile(kFALSE) {
}
// --------------------------------------------------------------------------



// -----   Constructor with properties   ------------------------------------
CbmTarget::CbmTarget(Int_t z, Double_t thickness, 
										 Double_t diameter, Double_t density) :
  FairModule("Target", "CBM target"),
	fZ(z),
	fThickness(thickness),
	fDiameter(diameter),
	fDensity(density),
	fPosX(0.), fPosY(0.), fPosZ(0.),
	fRotY(0.),
	fMaterial(""),
	fBuildFromFile(kFALSE) {
}
// --------------------------------------------------------------------------



// -----   Construct the geometry   -----------------------------------------
void CbmTarget::ConstructGeometry() {

	std::cout << std::endl;

	// --- Construct from ROOT file if specified
	if ( fBuildFromFile ) {
		if ( ! fgeoName.EndsWith(".geo.root") ) {
			LOG(INFO)  << GetName() << ": geometry file is " << fgeoName;
			LOG(FATAL) << GetName() << ": only ROOT geometry files are supported!";
			return;
		}
		LOG(INFO) << GetName() << ": Constructing geometry from " << fgeoName;
		ConstructRootGeometry();
		return;
	}

	LOG(INFO) << GetName() << ": Constructing geometry...";

	// --- Get TGeoManager instance
	TGeoManager* geoMan = gGeoManager;
	assert(geoMan);

	// --- Get target element
	TGeoElement* targElem = NULL;
	if ( fZ ) {
		targElem = geoMan->GetElementTable()->GetElement(fZ);
		if ( ! targElem ) {
			LOG(FATAL) << GetName() << ": Unknown element " << fZ;
			return;
		}
		fMaterial = targElem->GetTitle();
	}
	else if ( ! fMaterial.IsNull() ) {
		targElem = geoMan->GetElementTable()->FindElement(fMaterial.Data());
		if ( ! targElem ) {
			LOG(FATAL) << GetName() << ": Unknown element " << fMaterial;
		}
		fZ = targElem->Z();
	}
	else {
		LOG(FATAL) << GetName() << ": Target material not specified!";
		return;
	}
	LOG(INFO) << GetName() << ": Use material " << fMaterial << ", z =  " << fZ;

	// --- Get density, if not set through the constructor
	fDensity = fDensity > 0. ? fDensity : GetStandardDensity(fZ);
	if ( fDensity < 0. ) {
		LOG(FATAL) << GetName() << ": No standard density for element "
				<< fMaterial << " available: density must be set explicitly.";
		return;
	}

	// --- Create target medium
	TGeoMaterial* targMat = new TGeoMaterial("targetMaterial",
																					 targElem, fDensity);
	if ( fair::Logger::Logging("DEBUG") ) targMat->Print();
	// The medium ID (second argument) has no meaning for transport
	TGeoMedium* targMedium = new TGeoMedium("targetMedium", 9999, targMat);
	targMedium->SetParam(0, 1.);     // is active
	targMedium->SetParam(1, 1.);     // is in magnetic field
	targMedium->SetParam(2, 20.);    // max. field [kG]
	targMedium->SetParam(6, 0.001);  // boundary crossing precisison [cm]

	// --- Get mother node
	TGeoNode* motherNode = geoMan->FindNode(fPosX, fPosY, fPosZ);
	if ( ! motherNode ) {
		LOG(FATAL) << GetName() << ": No mother node found at target position!";
		return;
	}
	LOG(INFO) << GetName() << ": Mother node is " << motherNode->GetName();

	// Construct the transformation matrix for positioning of the target
	// in its mother volume. The matrix is the inverse of the global
	// transformation matrix of the mother node (thus assuring that the
	// target is correctly placed w.r.t. the global c.s.) plus a translation
	// in the global c.s. defined by the desired target position.
	TGeoHMatrix r1 = geoMan->GetCurrentMatrix()->Inverse();
	TGeoTranslation r2 = TGeoTranslation(fPosX, fPosY, fPosZ);
	TGeoRotation* target_rotation = new TGeoRotation();
	target_rotation->RotateY(fRotY*TMath::RadToDeg());
	TGeoHMatrix* targetMatrix = new TGeoHMatrix("targetToGlobal");
	*targetMatrix = r1 * r2 * *target_rotation;

	// --- Create target volume and add it as node to the mother volume
	TGeoVolume* target = geoMan->MakeTube("target", targMedium, 0.,
																				fDiameter/2., fThickness/2.);
	motherNode->GetVolume()->AddNode(target, 0, targetMatrix);

	// --- Check the resulting transformation from target to global
	TGeoNode* testNode = geoMan->FindNode(fPosX, fPosY, fPosZ);
	LOG(DEBUG) << GetName() << ": Test node is " << testNode->GetName();
	TGeoHMatrix* testMatrix = geoMan->GetCurrentMatrix();
	if ( fair::Logger::Logging("DEBUG") ) testMatrix->Print();

	std::cout << std::endl;
}
// --------------------------------------------------------------------------



// -----   Normal vector   --------------------------------------------------
TVector3 CbmTarget::GetNormal() const {
	return TVector3(TMath::Sin(fRotY), 0., TMath::Cos(fRotY));
}
// --------------------------------------------------------------------------



// -----   Get the density at standard conditions   -------------------------
Double_t CbmTarget::GetStandardDensity(Int_t charge) const {

	// TODO: Better implementation with array or the like

	switch (charge) {
		case  4: return  1.848; break;  // Beryllium
		case  6: return  2.260; break;  // Carbon
		case 28: return  8.908; break;  // Nickel
		case 47: return 10.490; break;  // Silver
		case 49: return  7.310; break;  // Indium
		case 79: return 19.320; break;  // Gold
		case 82: return 11.342; break;  // Lead
		default: return -1.; break;     // Not found
	}

	return -1.;
}
// --------------------------------------------------------------------------



// -----   Downstream surface centre   --------------------------------------
TVector3 CbmTarget::GetSurfaceCentreDown() const {
	return TVector3(fPosX + 0.5 * fThickness * TMath::Sin(fRotY),
									fPosY,
									fPosZ + 0.5 * fThickness * TMath::Cos(fRotY));
}
// --------------------------------------------------------------------------



// -----   Upstream surface centre   ----------------------------------------
TVector3 CbmTarget::GetSurfaceCentreUp() const {
	return TVector3(fPosX - 0.5 * fThickness * TMath::Sin(fRotY),
									fPosY,
									fPosZ - 0.5 * fThickness * TMath::Cos(fRotY));
}
// --------------------------------------------------------------------------



// -----   Set a geometry file (overloaded from base class)   ---------------
void CbmTarget::SetGeometryFileName(TString name, TString version) {
	fBuildFromFile = kTRUE;
	LOG(INFO) << "Using target file name " << name;
	return FairModule::SetGeometryFileName(name, version);
}
// --------------------------------------------------------------------------



// -----   Info   -----------------------------------------------------------
std::string CbmTarget::ToString() const {

	std::stringstream ss;

	if ( fBuildFromFile ) ss << GetName() << ": Geometry file " << fgeoName;

	else {
		ss << GetName() << ": Material " << fMaterial;
		if ( fDensity >= 0. ) ss << ", density " << fDensity << " g/cm^3, ";
		else ss << ", standard density, ";
		ss << "thickness " << fThickness * 10000. << " mum, diameter "
			 << fDiameter << " cm, position (" << fPosX << ", " << fPosY
			 << ", " << fPosZ << ") cm, rotation (y) " << fRotY << " rad";
	} //? Not build from geometry file

	return ss.str();
}
// --------------------------------------------------------------------------


ClassImp(CbmTarget)



