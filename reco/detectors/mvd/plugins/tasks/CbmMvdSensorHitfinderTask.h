/* Copyright (C) 2014-2016 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdSensorHitfinderTask header file            -----
// -----                   Created 11/09/13 P.Sitzmann                 -----
// -------------------------------------------------------------------------


#ifndef CBMMVDSENSORHITFINDERTASK_H
#define CBMMVDSENSORHITFINDERTASK_H 1

#include "CbmMvdCluster.h"     // for CbmMvdCluster
#include "CbmMvdSensorTask.h"  // for CbmMvdSensorTask

#include <FairTask.h>  // for InitStatus

#include <Rtypes.h>        // for ClassDef
#include <RtypesCore.h>    // for Int_t, Double_t, Bool_t, Float_t, kTRUE
#include <TClonesArray.h>  // for TClonesArray
#include <TString.h>       // for TString

#include <map>      // for map
#include <utility>  // for pair

class CbmMvdSensor;
class TBuffer;
class TCanvas;
class TClass;
class TH1F;
class TH2F;
class TMemberInspector;
class TObjArray;
class TObject;
class TVector3;

class CbmMvdSensorHitfinderTask : public CbmMvdSensorTask {

public:
  /** Default constructor **/
  CbmMvdSensorHitfinderTask();


  /** Standard constructor
     *@param name  Task name
     *@param mode  0 = no fake digis generation, 1 = generate fake digis
     **/
  CbmMvdSensorHitfinderTask(Int_t mode);


  /** Destructor **/
  virtual ~CbmMvdSensorHitfinderTask();


  /** Task execution **/
  void ExecChain();
  void Exec();

  /** Intialisation **/
  void InitTask(CbmMvdSensor* mySensor);

  void SetInputCluster(CbmMvdCluster* cluster)
  {
    new ((*fInputBuffer)[fInputBuffer->GetEntriesFast()]) CbmMvdCluster(*((CbmMvdCluster*) cluster));
    inputSet = kTRUE;
  };

  void SetInput(TObject* cluster)
  {
    new ((*fInputBuffer)[fInputBuffer->GetEntriesFast()]) CbmMvdCluster(*((CbmMvdCluster*) cluster));
    inputSet = kTRUE;
  };

  /** Accessors **/
  //Double_t GetSigmaX()        const { return fSigmaX;     };

  /** Modifiers **/
  void SetSigmaNoise(Double_t sigmaNoise, Bool_t addNoise)
  {
    fSigmaNoise = sigmaNoise;
    fAddNoise   = addNoise;
  }
  void SetSeedThreshold(Double_t seedCharge) { fSeedThreshold = seedCharge; }
  void SetNeighbourThreshold(Double_t neighCharge) { fNeighThreshold = neighCharge; }
  /**

        proba(%) to have 1 fake hit  | Number of sigmas (of noise distr)
       ------------------------------|-----------------------------------
                            31.7     |   1
                            4.5      |   2
                            0.27     |   3
                            6.3e-3   |   4
                            5.7e-5   |   5
			    2.0e-7   |   6

    */


  void SetAdcDynamic(Int_t adcDynamic) { fAdcDynamic = adcDynamic; };
  void SetAdcOffset(Int_t adcOffset) { fAdcOffset = adcOffset; };
  void SetAdcBits(Int_t adcBits) { fAdcBits = adcBits; };
  Int_t GetAdcCharge(Float_t charge);

  /**Detector Spatial resolution.
    Correlated with number of adc bits*/
  void SetHitPosErrX(Double_t errorX) { fHitPosErrX = errorX; }
  void SetHitPosErrY(Double_t errorY) { fHitPosErrY = errorY; }
  void SetHitPosErrZ(Double_t errorZ) { fHitPosErrZ = errorZ; }
  void ShowDebugHistograms() { fShowDebugHistos = kTRUE; }


  //protected:
protected:
  // ----------   Protected data members  ------------------------------------

  Int_t fAdcDynamic;
  Int_t fAdcOffset;
  Int_t fAdcBits;
  Int_t fAdcSteps;
  Float_t fAdcStepSize;

  TClonesArray* fClusters;


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
  Double_t fSigmaNoise;
  Double_t fSeedThreshold;
  Double_t fNeighThreshold;
  Bool_t fShowDebugHistos;
  Bool_t fUseMCInfo;
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

  TString fBranchName;

  Int_t fDigisInCluster;
  static const Short_t fChargeArraySize = 7;  //must be an odd number >3, recommended numbers= 5 or 7

  Bool_t fAddNoise;

  /** Map of MC Volume Id to MvdStation **/
  //!


  // -----   Private methods   ---------------------------------------------


  /** Clear the arrays **/
  void Reset();

  /** Virtual method Finish **/
  void Finish();


  /** Reinitialisation **/
  InitStatus ReInit();


  /** Get MVD geometry parameters from database
     **@value Number of MVD stations
     **/
  Int_t GetMvdGeometry();


  void SetMvdGeometry(Int_t detId);

  // void GenerateFakeDigis(Double_t pixelSizeX, Double_t pixelSizeY);
  // void CheckForNeighbours(vector<Int_t>* clusterArray, Int_t clusterDigi, TArrayS* pixelUsed);

  void CreateHit(CbmMvdCluster* clusterArray, TVector3& pos, TVector3& dpos);
  void ComputeCenterOfGravity(CbmMvdCluster* clusterArray, TVector3& pos, TVector3& dpos);
  // void UpdateDebugHistos(vector<Int_t>* clusterArray, Int_t seedIndexX, Int_t seedIndexY);

private:
  CbmMvdSensorHitfinderTask(const CbmMvdSensorHitfinderTask&);
  CbmMvdSensorHitfinderTask operator=(const CbmMvdSensorHitfinderTask&);

  ClassDef(CbmMvdSensorHitfinderTask, 1);
};


#endif

/* fGausArray=new Float_t[fGausArrayLimit];
    for(Int_t i=0;i<fGausArrayLimit;i++){fGausArray[i]=gRandom->Gaus(0, fSigmaNoise);};
    fGausArrayIt=0;
*/
