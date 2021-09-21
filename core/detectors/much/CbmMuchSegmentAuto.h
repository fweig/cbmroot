/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mikhail Ryzhinskiy [committer], Florian Uhlig, Volker Friese */

/** CbmMuchSegmentAuto.h
 *@author Mikhail Ryzhinskiy <m.ryzhinskiy@gsi.de>
 *@since 20.06.07
 *@version 1.0
 **
 ** class for making parameter file for MUCH digitizer
 **
 **/

#ifndef CBMMUCHSEGMENTAUTO_H
#define CBMMUCHSEGMENTAUTO_H 1

#include <FairTask.h>  // for FairTask, InitStatus

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t, Option_t
#include <TString.h>     // for TString

#include <vector>  // for vector

class CbmGeoMuchPar;
class CbmMuchLayerSide;
class CbmMuchModuleGem;
class CbmMuchSectorRectangular;
class TClonesArray;
class TH1D;
class TObjArray;

class CbmMuchSegmentAuto : public FairTask {
public:
  /** Default constructor **/
  CbmMuchSegmentAuto();

  /** Default constructor **/
  CbmMuchSegmentAuto(const char* digiFileName);

  void SetNStations(Int_t nStations);
  void SetSigmaMin(Double_t* sigmaXmin, Double_t* sigmaYmin);
  void SetSigmaMax(Double_t* sigmaXmax, Double_t* sigmaYmax);
  void SetOccupancyMax(Double_t* occupancyMax);


  virtual InitStatus Init();
  virtual void Exec(Option_t* option);
  virtual void FinishTask();

  /** Destructor **/
  virtual ~CbmMuchSegmentAuto();

private:
  Int_t fEvents;
  TClonesArray* fPoints;
  TH1D** fHistHitDensity;
  Int_t fNStations;
  TObjArray* fStations;

  TString fDigiFileName;        // Digitization file
  CbmGeoMuchPar* fGeoPar;       // Geometry parameters container
  std::vector<Double_t> fExp0;  // Fit parameters for EXP distribution for each station
  std::vector<Double_t> fExp1;  // Fit parameters for EXP distribution for each station

  std::vector<Double_t> fSigmaXmin;     // Minimum sigma in X [mm]
  std::vector<Double_t> fSigmaYmin;     // Minimum sigma in Y [mm]
  std::vector<Double_t> fSigmaXmax;     // Maximum sigma in X [mm]
  std::vector<Double_t> fSigmaYmax;     // Maximum sigma in Y [mm]
  std::vector<Double_t> fOccupancyMax;  // Maximum occupancy

  /** Get parameter containers **/
  virtual void SetParContainers();

  /** Initialization **/
  void InitLayerSide(CbmMuchLayerSide* layerSide);
  void SegmentModule(CbmMuchModuleGem* module);
  void SegmentSector(CbmMuchModuleGem* module, CbmMuchSectorRectangular* sector);
  Bool_t ShouldSegmentByX(CbmMuchSectorRectangular* sector);
  Bool_t ShouldSegmentByY(CbmMuchSectorRectangular* sector);
  Int_t IntersectsRad(CbmMuchSectorRectangular* sector, Double_t radius);
  void DrawSegmentation();
  void Print(Option_t* = "") const;

  CbmMuchSegmentAuto(const CbmMuchSegmentAuto&);
  CbmMuchSegmentAuto& operator=(const CbmMuchSegmentAuto&);

  ClassDef(CbmMuchSegmentAuto, 2)
};

#endif
