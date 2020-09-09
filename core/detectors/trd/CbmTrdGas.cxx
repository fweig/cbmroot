/**
 * \file CbmTrdGas.cxx
 * \brief Container for gas properties of TRD.
 */
#include "CbmTrdGas.h"

#include <Logger.h>  // for LOG, Logger

#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TGeoBBox.h>           // for TGeoBBox
#include <TGeoManager.h>        // for TGeoManager, gGeoManager
#include <TGeoMaterial.h>       // for TGeoMixture
#include <TGeoVolume.h>         // for TGeoVolume
#include <TMath.h>              // for Nint
#include <TObjArray.h>          // for TObjArray, TObjArrayIter
#include <TObject.h>            // for TObject
#include <TString.h>            // for TString, operator+, operator<<

#include <stdlib.h>  // for getenv

CbmTrdGas* CbmTrdGas::fgInstance = 0;

CbmTrdGas::CbmTrdGas()
  : TObject()
  , fDetType(-1)
  , fGasThick(0.)
  , fPercentNobleGas(0.)
  , fPercentCO2(0.)
  , fNobleGasType(-1)
  , fFileNameLike("")
  , fFileNameANN("") {
  if (fgInstance) {
    LOG(fatal) << "CbmTrdGas::CbmTrdGas Singleton instance already exists.";
  }
  fgInstance = this;
}

CbmTrdGas::~CbmTrdGas() {}

void CbmTrdGas::Init() {
  // Read MWPC gas properties from geometry. This enforce that gas
  // mixture simulation and reconstruction is the same. This is important
  // because dE/dx is calculated in the simulation and TR is calculated
  // in the reconstruction.

  // Get pointer to gas volume
  TGeoVolume* fm =
    (TGeoVolume*) gGeoManager->GetListOfVolumes()->FindObject("gas");
  if (!fm) {
    TObjArray* volList = gGeoManager->GetListOfVolumes();
    TObjArrayIter iter(volList);
    TGeoVolume* vol = nullptr;
    LOG(error) << "********** List of available volumes ************ ";
    while ((vol = (TGeoVolume*) iter.Next())) {
      LOG(error) << vol->GetName();
    }
    LOG(error) << "***************** End of List ******************* \n"
               << " -E- Could not find volume <gas>. \n"
               << " -E- If there is no list above this text then probably \n"
               << " -E- the geometry was not loaded in the macro.\n"
               << " -E- Please do it with fRun->LoadGeometry(). \n "
               << " -E- If you see a list probably the names of the \n"
               << " -E- volumes have changed and CbmTrdRadiator has to \n"
               << " -E- be changed accordingly. ";
    LOG(fatal) << "CbmTrdGas::Init: No volumes defined.";
  }

  // check if detector is of GSI or Muenster/Bucarest type
  // the type is coded in the master volume name
  TGeoVolume* fm1 =
    (TGeoVolume*) gGeoManager->GetListOfVolumes()->FindObject("trd1mb");

  // get pointer to shape of gas volume
  TGeoBBox* shape = (TGeoBBox*) fm->GetShape();

  if (fm1) {  // MB type
    fDetType = 1;
    // only halve thickness of gas layer tacken into account because the
    // total absorbed TR is calculated in two steps since the real chamber
    // consists of two gas volumes with both halve the thickness as from
    // geometry
    fGasThick = 2 * (shape->GetDZ());
    LOG(info) << "CbmTrdGas::Init: Detector type : double sided geometry (1) ";
    LOG(info) << "CbmTrdGas::Init: Gas thickness : " << fGasThick << " cm";
  } else {  // GSI type
    fDetType  = 0;
    fGasThick = 2 * (shape->GetDZ());
    LOG(info) << "CbmTrdGas::Init: Detector type : standard GSI geometry (2) ";
    LOG(info) << "CbmTrdGas::Init: Gas thickness : " << fGasThick << " cm";
  }

  // Get all the necessary properties of the gas mixture
  TGeoMixture* mixt = (TGeoMixture*) fm->GetMaterial();
  Int_t nmixt       = mixt->GetNelements();
  if (nmixt != 3) {
    LOG(error) << "CbmTrdGas::Init: This is not a mixture composed out of "
                  "three different elements.";
    LOG(error)
      << "CbmTrdGas::Init: Don't know what to do, so stop execution here.";
    LOG(fatal) << "CbmTrdGas::Init: Unknown gas mixture.";
  }

  Bool_t foundCarbon = kFALSE;
  Bool_t foundOxygen = kFALSE;
  Int_t carbon       = 0;
  Int_t oxygen       = 0;
  Int_t noblegas     = 0;

  Double_t* elem   = mixt->GetZmixt();
  Double_t* weight = mixt->GetWmixt();
  Double_t* amixt  = mixt->GetAmixt();

  for (Int_t i = 0; i < nmixt; i++) {
    if (elem[i] == 6.0) {
      carbon      = i;
      foundCarbon = kTRUE;
    } else if (elem[i] == 8.0) {
      oxygen      = i;
      foundOxygen = kTRUE;
    } else
      noblegas = i;
  }
  if (!(foundCarbon && foundOxygen)) {
    LOG(error) << "CbmTrdGas::Init: This gas mixture has no CO2 admixture \n"
               << "CbmTrdGas::Init: If you want to use this mixture you have "
                  "to change \n"
               << "CbmTrdGas::Init: CbmTrdRadiator to be consistent \n";
    LOG(fatal) << "CbmTrdGas::Init: Unknown gas mixture.";
  }
  if (elem[noblegas] != 54) {
    LOG(error) << "CbmTrdGas::Init:  This gas mixture has no Xe admixture \n"
               << "CbmTrdGas::Init:  If you want to use this mixture you have "
                  "to change \n"
               << "CbmTrdGas::Init:  CbmTrdRadiator to be consistent";
    LOG(fatal) << "CbmTrdGas::Init: Unknown gas mixture.";
  } else {
    fNobleGasType = 1;
  }

  Double_t massC  = amixt[carbon];
  Double_t massO  = amixt[oxygen];
  Double_t massXe = amixt[noblegas];
  Double_t x      = weight[noblegas];
  Double_t percentNoblegas =
    100
    * (((massC * x) + (2 * massO * x))
       / (massXe + massC * x + 2 * massO * x - massXe * x));

  fPercentNobleGas = TMath::Nint(percentNoblegas) / 100.;
  fPercentCO2      = 1 - fPercentNobleGas;

  if (elem[noblegas] == 54) {
    LOG(info) << "CbmTrdGas::Init: Percent  (Xe) : "
              << (fPercentNobleGas * 100);
  }
  LOG(info) << "CbmTrdGas::Init: Percent (CO2) : " << (fPercentCO2 * 100);

  SetFileName();
}

TString CbmTrdGas::GetFileName(TString method) const {
  if (method.Contains("Like")) {
    return fFileNameLike;
  } else if (method.Contains("ANN")) {
    return fFileNameANN;
  } else {
    LOG(error)
      << "CbmTrdGas::GetFileName: Electron ID method " << method
      << " not known. \n"
      << "CbmTrdGas::GetFileName: The input must be either Like or ANN";
    return "";
  }
}

void CbmTrdGas::SetFileName() {
  Int_t fraction  = TMath::Nint(fPercentNobleGas * 100);
  Int_t thickness = TMath::Nint(fGasThick * 10);

  const char* detector = "";
  if (fDetType == 0) {
    detector = "GSI";
  } else if (fDetType == 1) {
    detector = "MB";
  } else {
    LOG(error) << "CbmTrdGas::SetFileName: Detector type " << fDetType
               << " not known";
    LOG(error) << "CbmTrdGas::SetFileName: Stop execution of program due to "
                  "initialization error.";
    LOG(fatal) << "CbmTrdGas::SetFileName: Unknown detector type.";
  }
  const char* gastype = "";
  if (fNobleGasType == 1) {
    gastype = "Xenon";
  } else {
    LOG(error) << "CbmTrdGas::SetFileName: Gas type " << fNobleGasType
               << " not known";
    LOG(error) << "CbmTrdGas::SetFileName: Stop execution of program due to "
                  "initialization error.";
    LOG(fatal) << "CbmTrdGas::SetFileName: Unknown gas type.";
  }

  TString path = getenv("VMCWORKDIR");
  path         = path + "/parameters/trd/";
  fFileNameLike.Form(
    "Likelihood_%s_%d_%s_%d.root", gastype, fraction, detector, thickness);
  fFileNameANN.Form(
    "ANN_%s_%d_%s_%d.root", gastype, fraction, detector, thickness);

  fFileNameLike = path + fFileNameLike;
  fFileNameANN  = path + fFileNameANN;
}

ClassImp(CbmTrdGas)
