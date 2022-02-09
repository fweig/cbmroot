/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: P.-A. Loizeau [committer] */

#include <cstdlib>
#include <iostream>

#include <cmath>

uint32_t check_radlen(std::string fileName = "test.geo.root", double_t dTolerance = 0.1)
{
  /// Instantiate a FairRun to get the unit convention in ROOT Geo classes forced to ROOT one
  /// (fix for the bug present between ROOT v6.18 and 6.25)
  FairRunAna* fRun = new FairRunAna();

  TString myName = "check_radlen";

  TFile file(fileName.c_str(), "READ");
  if (!file.IsOpen()) {
    std::cerr << "Was a root geo file supplied?" << std::endl;
    exit(1);
  };
  gROOT->cd();

  file.GetObject("FAIRGeom", gGeoManager);
  if (nullptr == gGeoManager) {
    std::cout << "No geo manager found in file, probably this is a standalone geometry file" << std::endl;

    std::string geo_tag = fileName.substr(fileName.find_last_of('/') + 1);
    geo_tag             = geo_tag.substr(0, geo_tag.find_first_of('.'));
    std::cout << "Trying to load volume with geo tag extracted from file name: " << geo_tag << std::endl;

    TGeoVolume* pDetVol = nullptr;
    file.GetObject(geo_tag.c_str(), pDetVol);
    if (nullptr == pDetVol) {
      std::cout << "Could not load the volume with a geo_tag matching the filename, file is not proper CBMROOT geometry"
                << std::endl;
      exit(1);
    }

    std::cerr << "Method to load the materials from a -not simulated- geometry NOT YET IMPLEMENTED" << std::endl;
    exit(1);
  }

  gROOT->cd();

  TList* plMats = gGeoManager->GetListOfMaterials();

  uint32_t uNbMats = plMats->GetEntries();
  uint32_t uNbSkip = 0;
  uint32_t uNbFail = 0;
  uint32_t uNbOkay = 0;
  for (const auto&& pObj : *plMats) {
    TGeoMaterial* material = dynamic_cast<TGeoMaterial*>(pObj);

    std::string sRes = "SKIP \t";
    double_t dA      = material->GetA();
    double_t dZ      = material->GetZ();
    double_t dRho    = material->GetDensity();
    double_t dRadLen = material->GetRadLen();

    double_t dExpRadLen = 0.;
    double_t dDiff      = 0.;
    double_t dDeviation = 0.;

    if (dZ < 1) {
      /// Special case for "vacuum" and "dummy"
      uNbSkip++;
    }
    else {
      dExpRadLen = (716.4 * dA / (dZ * (dZ + 1) * std::log(287 / std::sqrt(dZ))) / dRho);
      dDiff      = dExpRadLen - dRadLen;
      dDeviation = std::fabs(dDiff) / dRadLen;
      if (dDeviation < dTolerance) {
        sRes = "OKAY \t";
        uNbOkay++;
      }
      else {
        sRes = "FAIL \t";
        uNbFail++;
      }
    }

    std::cout << sRes;
    if (material->IsMixture()) {
      std::cout << Form("Mixture  \t%20s \tindex = %i \tAeff = %7g \tZeff = %7g \trho = %8g \tradlen = %8g \t",
                        material->GetName(), material->GetIndex(), dA, dZ, dRho, dRadLen);
    }
    else {
      std::cout << Form("Material \t%20s \tindex = %i \tA    = %7g \tZ    = %7g \trho = %8g \tradlen = %8g \t",
                        material->GetName(), material->GetIndex(), dA, dZ, dRho, dRadLen);
    }
    std::cout << Form("Expected radlen = %8g \t Dev  = %8g \t Diff = %8g", dExpRadLen, dDeviation, dDiff) << std::endl;
  }

  gROOT->cd();
  RemoveGeoManager();
  file.Close();

  std::cout << uNbFail << " failures and " << uNbSkip << " skipped in " << uNbMats << " materials" << std::endl;

  std::cout << "Program Ends" << std::endl;

  return uNbFail;
}  // End of macro
