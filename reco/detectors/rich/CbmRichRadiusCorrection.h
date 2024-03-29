/* Copyright (C) 2012-2021 UGiessen/JINR-LIT, Giessen/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

/**
* \file CbmRichRadiusCorrection.h
*
* \brief This class performs a correction of A and B parameters for ellipse fit or
* radius correction for circle fit.
*
* \author Semen Lebedev
* \date 2012
**/
#ifndef CBM_RICH_RADIUS_CORRECTION
#define CBM_RICH_RADIUS_CORRECTION

#include "CbmRichRingLight.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TSystem.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

/**
* \file CbmRichRadiusCorrection.h
*
* \brief This class performs a radius correction.
*
* \author Semen Lebedev
* \date 2012
**/
class CbmRichRadiusCorrection {
public:
  /**
    * \brief Perform A and B parameters correction.
    * \param[in] RICH ring.
    */
  static void DoCorrection(CbmRichRingLight* ring)
  {
    if (NULL == fhMapAaxisXY || NULL == fhMapBaxisXY) { Init(); }
    if (NULL == fhMapAaxisXY || NULL == fhMapBaxisXY) return;

    double centerX = ring->GetCenterX();
    double centerY = ring->GetCenterY();
    double axisA   = ring->GetAaxis() + fhMapAaxisXY->GetBinContent(fhMapAaxisXY->FindBin(centerX, centerY));
    double axisB   = ring->GetBaxis() + fhMapBaxisXY->GetBinContent(fhMapBaxisXY->FindBin(centerX, centerY));

    ring->SetAaxis(axisA);
    ring->SetBaxis(axisB);
  }

private:
  /**
    * \brief Initialize histograms for radius correction procedure.
    * This procedure will be invoked automatically before first correction
    * is made.
    */
  static void Init()
  {
    string fileName = gSystem->Getenv("VMCWORKDIR");
    fileName += "/parameters/rich/radius_correction_map_compact.root";

    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    TFile* file = new TFile(fileName.c_str(), "READ");

    if (NULL == file || !file->IsOpen()) {
      cout << " -E- Read correction maps " << endl;
      cout << " -E- Could not open input file " << fileName << endl;
      return;
    }
    else {
      cout << " -I- Map Correction input file: " << fileName << endl;
    }

    gROOT->cd();

    fhMapAaxisXY = (TH2D*) file->Get("fh_mapaxisAXY")->Clone();
    fhMapBaxisXY = (TH2D*) file->Get("fh_mapaxisBXY")->Clone();

    file->Close();
    delete file;

    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;
  }

  static TH2D* fhMapAaxisXY;
  static TH2D* fhMapBaxisXY;
};

#endif
