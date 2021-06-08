/* Copyright (C) 2019 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors:  */

// -------------------------------------------------------------------------
// -----                   CbmMvdSensorClusterfinderTask header file   -----
// -----                   27.11.18 Edited by K. Hunold                -----
// -------------------------------------------------------------------------


#ifndef CBMMVDSENSORDIGITOHITTASK_H
#define CBMMVDSENSORDIGITOHITTASK_H 1

#include "CbmMvdCluster.h"
#include "CbmMvdDigi.h"
#include "CbmMvdHit.h"

#include "TArrayS.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom3.h"
#include "TRefArray.h"
#include "TStopwatch.h"
#include "TString.h"

#include <list>
#include <map>
#include <numeric>
#include <utility>
#include <vector>
//#include <Vc/Vc>

#include "CbmMvdSensor.h"
#include "CbmMvdSensorTask.h"

class TClonesArray;
class TRandom3;

class CbmMvdSensorDigiToHitTask : public CbmMvdSensorTask {

public:
  /** Default constructor **/
  CbmMvdSensorDigiToHitTask();


  /** Standard constructor
     *@param name  Task name
     *@param mode  0 = no fake digis generation, 1 = generate fake digis
     **/
  CbmMvdSensorDigiToHitTask(Int_t mode, Int_t iVerbose);


  /** Destructor **/
  virtual ~CbmMvdSensorDigiToHitTask();

  /** Task execution **/
  void ExecChain();
  void Exec();

  /** Intialisation **/
  void InitTask(CbmMvdSensor* mySensor);

  virtual void SetInputDigi(CbmMvdDigi* digi)
  {
    new ((*fInputBuffer)[fInputBuffer->GetEntriesFast()]) CbmMvdDigi(*((CbmMvdDigi*) digi));
    inputSet = kTRUE;
  }


  /** Modifiers **/
  void SetSigmaNoise(Double_t sigmaNoise, Bool_t addNoise)
  {
    fSigmaNoise = sigmaNoise;
    fAddNoise   = addNoise;
  }
  void SetSeedThreshold(Double_t seedCharge) { fSeedThreshold = seedCharge; }
  void SetNeighbourThreshold(Double_t neighCharge) { fNeighThreshold = neighCharge; }


  void SetAdcDynamic(Int_t adcDynamic) { fAdcDynamic = adcDynamic; };
  void SetAdcOffset(Int_t adcOffset) { fAdcOffset = adcOffset; };
  void SetAdcBits(Int_t adcBits) { fAdcBits = adcBits; };
  float GetAdcCharge(Float_t charge);

  /**Detector Spatial resolution.
    Correlated with number of adc bits*/
  void SetHitPosErrX(Double_t errorX) { fHitPosErrX = errorX; }
  void SetHitPosErrY(Double_t errorY) { fHitPosErrY = errorY; }
  void SetHitPosErrZ(Double_t errorZ) { fHitPosErrZ = errorZ; }

  void UpdateDebugHistos(CbmMvdCluster* cluster);

  //protected:
protected:
  // ----------   Protected data members  ------------------------------------
  Int_t fAdcDynamic;
  Int_t fAdcOffset;
  Int_t fAdcBits;
  TCanvas* c1;


  std::map<std::pair<Int_t, Int_t>, Int_t> fDigiMap;
  std::map<std::pair<Int_t, Int_t>, Int_t>::iterator fDigiMapIt;

private:
  Int_t fVerbose;
  Double_t fSigmaNoise;
  Double_t fSeedThreshold;
  Double_t fNeighThreshold;

  Bool_t inputSet;

  Double_t fLayerRadius;
  Double_t fLayerRadiusInner;
  Double_t fLayerPosZ;
  Double_t fHitPosX;
  Double_t fHitPosY;
  Double_t fHitPosZ;
  Double_t fHitPosErrX;
  Double_t fHitPosErrY;
  Double_t fHitPosErrZ;


  static const Short_t fChargeArraySize = 5;  //must be an odd number >3, recommended numbers= 5 or 7

  Bool_t fAddNoise;

  // -----   Private methods   ---------------------------------------------


  /** Clear the arrays **/
  void Reset() { ; };

  /** Virtual method Finish **/
  void Finish();

  /** Reinitialisation **/
  Bool_t ReInit();


  /** Get MVD geometry parameters from database
     **@value Number of MVD stations
     **/
  Int_t GetMvdGeometry() { return 0; };


private:
  CbmMvdSensorDigiToHitTask(const CbmMvdSensorDigiToHitTask&);
  CbmMvdSensorDigiToHitTask operator=(const CbmMvdSensorDigiToHitTask&);

  ClassDef(CbmMvdSensorDigiToHitTask, 1);
};


#endif
