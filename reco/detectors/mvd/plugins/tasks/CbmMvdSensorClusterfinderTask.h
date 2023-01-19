/* Copyright (C) 2014-2017 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdSensorClusterfinderTask header file            -----
// -----                   Created 11/09/13 P.Sitzmann                 -----
// -------------------------------------------------------------------------


#ifndef CBMMVDSENSORCLUSTERFINDERTASK_H
#define CBMMVDSENSORCLUSTERFINDERTASK_H 1


#include "CbmMvdCluster.h"
#include "CbmMvdDigi.h"
#include "CbmMvdSensor.h"
#include "CbmMvdSensorTask.h"

#include "TArrayS.h"
#include "TCanvas.h"
#include <TClonesArray.h>
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
#include <utility>
#include <vector>

class TClonesArray;
class TRandom3;

class CbmMvdSensorClusterfinderTask : public CbmMvdSensorTask {

public:
  /** Default constructor **/
  CbmMvdSensorClusterfinderTask();


  /** Standard constructor
     *@param name  Task name
     *@param mode  0 = no fake digis generation, 1 = generate fake digis
     **/
  CbmMvdSensorClusterfinderTask(Int_t mode, Int_t iVerbose);


  /** Destructor **/
  virtual ~CbmMvdSensorClusterfinderTask();


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

  virtual void SetInput(TObject* digi)
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
  Int_t GetAdcCharge(Float_t charge);

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
  Int_t fAdcSteps;
  Int_t fAddress;
  Float_t fAdcStepSize;

  TClonesArray* fDigis;
  TObjArray* fPixelChargeHistos;
  TObjArray* fTotalChargeInNpixelsArray;

  // Debug Histograms
  TH1F* fResolutionHistoX;
  TH1F* fResolutionHistoY;
  TH1F* fResolutionHistoCleanX;
  TH1F* fResolutionHistoCleanY;
  TH1F* fResolutionHistoMergedX;
  TH1F* fResolutionHistoMergedY;
  TH2F* fBadHitHisto;
  Float_t* fGausArray;
  Int_t fGausArrayIt;
  Int_t fGausArrayLimit;

  std::map<std::pair<Int_t, Int_t>, Int_t> fDigiMap;
  std::map<std::pair<Int_t, Int_t>, Int_t>::iterator fDigiMapIt;


  TH2F* h;
  TH2F* h3;
  TH1F* h1;
  TH1F* h2;
  TH1F* Qseed;
  TH1F* fFullClusterHisto;

  TCanvas* c1;

private:
  Int_t fNEvent;
  Int_t fMode;
  Int_t fCounter;
  Int_t fVerbose;
  Double_t fSigmaNoise;
  Double_t fSeedThreshold;
  Double_t fNeighThreshold;
  Bool_t fUseMCInfo;
  Bool_t inputSet;

  std::map<std::pair<Int_t, Int_t>, Int_t> ftempPixelMap;  //* couples Pixel(x,y) with collected charge


  Double_t fLayerRadius;
  Double_t fLayerRadiusInner;
  Double_t fLayerPosZ;
  Double_t fHitPosX;
  Double_t fHitPosY;
  Double_t fHitPosZ;
  Double_t fHitPosErrX;
  Double_t fHitPosErrY;
  Double_t fHitPosErrZ;

  TString fBranchName;

  static const Short_t fChargeArraySize = 5;  //must be an odd number >3, recommended numbers= 5 or 7

  Bool_t fAddNoise;

  // -----   Private methods   ---------------------------------------------


  /** Clear the arrays **/
  void Reset() { ; };

  /** Virtual method Finish **/
  void Finish();


  void CheckForNeighbours(std::vector<Int_t>* clusterArray, Int_t clusterDigi, TArrayS* pixelUsed);

  /** Reinitialisation **/
  Bool_t ReInit();


  /** Get MVD geometry parameters from database
     **@value Number of MVD stations
     **/
  Int_t GetMvdGeometry() { return 0; };


private:
  CbmMvdSensorClusterfinderTask(const CbmMvdSensorClusterfinderTask&);
  CbmMvdSensorClusterfinderTask operator=(const CbmMvdSensorClusterfinderTask&);

  ClassDef(CbmMvdSensorClusterfinderTask, 1);
};


#endif
