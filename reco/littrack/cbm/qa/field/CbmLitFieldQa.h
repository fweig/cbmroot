/* Copyright (C) 2009-2018 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file CbmLitFieldQa.h
 * \brief Field map QA.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 **/

#ifndef CBMLITFIELDQA_H_
#define CBMLITFIELDQA_H_

#include "FairTask.h"

#include <string>
#include <utility>
#include <vector>

class FairField;
class TList;
class CbmHistManager;

using std::string;
using std::vector;

/**
 * \class CbmLitFieldQa
 * \brief Field map QA.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 **/
class CbmLitFieldQa : public FairTask {
public:
  /**
     * \brief Constructor.
     */
  CbmLitFieldQa();

  /**
     * \brief Destructor.
     */
  virtual ~CbmLitFieldQa();

  /**
     * \brief Inherited from FairTask.
     */
  virtual InitStatus Init();

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Exec(Option_t* opt);

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Finish();

  /* Setters */
  void SetSliceZPosition(const vector<Double_t>& zPos) { fZSlicePosition = zPos; }
  //    void SetAcceptanceAngleX(Double_t xangle) { fAcceptanceAngleX = xangle; }
  //    void SetAcceptanceAngleY(Double_t yangle) { fAcceptanceAngleY = yangle; }
  void SetNofBinsX(Int_t nofBinsX) { fNofBinsX = nofBinsX; }
  void SetNofBinsY(Int_t nofBinsY) { fNofBinsY = nofBinsY; }
  void SetZMin(Double_t zMin) { fZMin = zMin; }
  void SetZMax(Double_t zMax) { fZMax = zMax; }
  void SetZStep(Double_t zStep) { fZStep = zStep; }
  void SetMinZFieldIntegral(Double_t minZ) { fMinZFieldIntegral = minZ; }
  void SetMaxZFieldIntegral(Double_t maxZ) { fMaxZFieldIntegral = maxZ; }
  void SetOutputDir(const string& dir) { fOutputDir = dir; }

private:
  /**
     * \brief Create histograms.
     */
  void CreateHistos();

  /**
     * \brief Fill graphs and histos for field map for each field component (Bx, By, Bz, |B|).
     */
  void FillBHistos();

  /**
     * \brief Fill B-field histograms for RICH PMT plane
     */
  void FillRichPmtPlaneBHistos();

  // Pointer to the magnetic field map
  FairField* fField;

  // Slice is defined as rectangle at a certain Z position
  // with upper left corner coordinate equals to [-X, -Y]
  // and lower right corner coordinate equals to [X, Y].
  // Z, X, Y coordinates for each slice.
  // [slice number]
  vector<Double_t> fZSlicePosition;  // Z position of the slice
  vector<Double_t> fXSlicePosition;  // X coordinate of slice
  vector<Double_t> fYSlicePosition;  // Y coordinate of slice

  Int_t fNofSlices;             // Number of slices along Z for field approximation
                                //    Double_t fAcceptanceAngleX; // Acceptance angle for X
                                //    Double_t fAcceptanceAngleY; // Acceptance angle for Y
  Int_t fNofBinsX;              // Number of bins for X
  Int_t fNofBinsY;              // Number of bins for Y
  Double_t fMinZFieldIntegral;  // start Z position for field integral
  Double_t fMaxZFieldIntegral;  // end Z position for field integral

  vector<Double_t> fAlongZAngles;                   // Polar angles [grad]
  vector<std::pair<Double_t, Double_t>> fAlongZXY;  // XY position for plotting field along Z
  Double_t fZMin;                                   // Minimum Z position [cm]
  Double_t fZMax;                                   // Maximum Z position [cm]
  Double_t fZStep;                                  // Step size [cm]

  CbmHistManager* fHM;  // Histogram manager

  // Output directory for images
  string fOutputDir;

  CbmLitFieldQa(const CbmLitFieldQa&);
  CbmLitFieldQa& operator=(const CbmLitFieldQa&);

  ClassDef(CbmLitFieldQa, 1);
};

#endif /* CBMLITFIELDQA_H_ */
