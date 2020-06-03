/* Generated by Together */

#include "CbmMagnet.h"

#include <TString.h>

//#include "TList.h"
//#include "TObjArray.h"
#include "TGeoMatrix.h"

CbmMagnet::CbmMagnet()
  : FairModule("CbmMagnet", "CbmMagnet"),
    frot(NULL),
    fposrot(NULL)
{
}

CbmMagnet::CbmMagnet(const char *name, const char *title, Double_t px, Double_t py, Double_t pz,
							Double_t rx, Double_t ry, Double_t rz)
  : FairModule(name, title),
    frot(new TGeoRotation("", rx, ry, rz)),
    fposrot(new TGeoCombiTrans(px, py, pz, frot))
{
}

CbmMagnet::~CbmMagnet()
{
}

void CbmMagnet::ConstructGeometry()
{
  TString fileName=GetGeometryFileName();

  if (fileName.EndsWith(".root"))	{

    LOG(info) << "Constructing MAGNET        from ROOT  file "
              << fileName.Data();

    // Quick and dirty fix: The top magnet volume of magnet_v18a is centred
    // at the origin of the GCS. It has to be shifted by 40 cm downstream
    // (centre coordinates (0, 0, 40) cm) in order to be at the proper place.
    // TODO: We have to urgently find a convention, such that this is not arbitrary.
    if ( fileName.Contains("magnet_v18a.geo.root") ) {
      LOG(info) << "Magnet with shift 40 cm";
      TGeoTranslation* trans = new TGeoTranslation();
      trans->SetTranslation(0., 0., 40.);
      ConstructRootGeometry(trans);
    } //? v18a

    // Similar fix for v18b (for muon setup, without field clamps). This
    // geometry is internally shifted by 80 cm downstream (along z). It has
    // thus to be shifted by -40 cm in z to be in the proper position.
    else if ( fileName.Contains("magnet_v18b.geo.root") ) {
      LOG(info) << "Constructing magnet with shift -40 cm";
      TGeoTranslation* trans = new TGeoTranslation();
      trans->SetTranslation(0., 0., -40.);
      ConstructRootGeometry(trans);
    } //? v18b

    else ConstructRootGeometry();  //? not v18a or v18b; no explicite shift


  } else if (fileName.EndsWith(".gdml")) {
    LOG(info) << "Constructing MAGNET        from GDML  file " << fileName.Data();
    ConstructGDMLGeometry(fposrot);
  } else {
    LOG(fatal) << "Geometry format of MAGNET file " << fileName.Data()
	       << " not supported.";
  }
}

Bool_t CbmMagnet::CheckIfSensitive(std::string) 
{
  // There are no sensitive volumes in the magnet
  return kFALSE;
}


ClassImp(CbmMagnet)
