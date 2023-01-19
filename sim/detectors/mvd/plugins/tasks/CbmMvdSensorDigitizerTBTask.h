/* Copyright (C) 2017 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Michael Deveaux, Philipp Sitzmann [committer] */

// ------------------------------------------------------------------------
// -----                  CbmMvdSensorDigitizerTBTask header file       -----
// -----                   Created 02/02/12  by M. Deveaux            -----
// ------------------------------------------------------------------------

/**  CbmMvdSensorDigitizerTBTask.h
 **  @author M.Deveaux <M.Deveaux@gsi.de>
 **  Acknowlegments to: C.Dritsa
 **
 **
 **/

#ifndef CBMMVDSENSORDIGITIZERTBTASK_H
#define CBMMVDSENSORDIGITIZERTBTASK_H 1


#include "CbmMvdSensor.h"

#include "FairTask.h"
//#include "omp.h"
#include "CbmMatch.h"
#include "CbmMvdDigi.h"
#include "CbmMvdPileupManager.h"
#include "CbmMvdPixelCharge.h"
#include "CbmMvdPoint.h"
#include "CbmMvdSensorTask.h"

#include "FairTask.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TH2F.h"
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


class CbmMvdSensorDigitizerTBTask : public CbmMvdSensorTask {

public:
  /** Default constructor **/
  CbmMvdSensorDigitizerTBTask();

  /** Destructor **/
  virtual ~CbmMvdSensorDigitizerTBTask();

  /** Intialisation **/
  virtual void InitTask(CbmMvdSensor* mySensor);

  /** fill buffer **/
  void SetInputArray(TClonesArray* inputStream);

  /** Execute **/
  void Exec();
  void ExecChain();

  TClonesArray* GetOutputArray() { return fOutputBuffer; };
  TClonesArray* GetMatchArray() { return fDigiMatch; };
  TClonesArray* GetWriteArray() { return fDigis; };

  InitStatus ReadSensorInformation();
  void ProduceIonisationPoints(CbmMvdPoint* point);
  void ProducePixelCharge(CbmMvdPoint* point);

  /** Modifiers **/
  void SetSegmentLength(Double_t segmentLength) { fSegmentLength = segmentLength; }
  void SetDiffusionCoef(Double_t diffCoeff) { fDiffusionCoefficient = diffCoeff; }
  void SetElectronsPerKeV(Double_t electronsPerKeV) { fElectronsPerKeV = electronsPerKeV; }
  void SetWidthOfCluster(Double_t widthOfCluster) { fWidthOfCluster = widthOfCluster; }
  void SetCutOnDeltaRays(Double_t cutOnDeltaRays) { fCutOnDeltaRays = cutOnDeltaRays; }
  void SetChargeThreshold(Float_t chargeThreshold) { fChargeThreshold = chargeThreshold; }

  void GetEventInfo(Int_t& inputNr, Int_t& eventNr, Double_t& eventTime);

private:
  Double_t fEpiTh;
  Double_t fSegmentLength;

  Double_t fDiffusionCoefficient;
  Double_t fElectronsPerKeV;
  Double_t fWidthOfCluster;
  Double_t fPixelSizeX;
  Double_t fPixelSizeY;
  Double_t fCutOnDeltaRays;
  Float_t fChargeThreshold;
  Double_t fFanoSilicium;

  Double_t fEsum;
  Double_t fSegmentDepth;
  Double_t fCurrentTotalCharge;
  Double_t fCurrentParticleMass;
  Double_t fCurrentParticleMomentum;
  Int_t fCurrentParticlePdg;

  Double_t fLorentzY0;
  Double_t fLorentzXc;
  Double_t fLorentzW;
  Double_t fLorentzA;
  Double_t fLorentzNorm;

  Double_t fLandauMPV;
  Double_t fLandauSigma;
  Double_t fLandauGain;
  TRandom3* fLandauRandom;

  Double_t fPixelSize;
  Double_t fPar0;
  Double_t fPar1;
  Double_t fPar2;

  Double_t fCompression;

  TH1F* fResolutionHistoX;
  TH1F* fResolutionHistoY;

  Int_t fNumberOfSegments;
  Int_t fCurrentLayer;
  Int_t fEvent;
  Int_t fVolumeId;
  Int_t fNPixelsX;
  Int_t fNPixelsY;

  TClonesArray* fPixelCharge;

  TClonesArray* fDigis;

  TClonesArray* fDigiMatch;

  Bool_t fproduceNoise;

  std::vector<CbmMvdPixelCharge*> fPixelChargeShort;

  TObjArray* fPixelScanAccelerator;
  std::map<std::pair<std::pair<Int_t, Int_t>, Double_t>, CbmMvdPixelCharge*> fChargeMap;
  std::map<std::pair<std::pair<Int_t, Int_t>, Double_t>, CbmMvdPixelCharge*>::iterator fChargeMapIt;


  CbmMvdSensorDataSheet* fsensorDataSheet;

  /** MAPS properties **/
  Double_t fSigmaX, fSigmaY;  // MAPS resolution in [cm]
  Double_t fReadoutTime;      // MAPS readout time in [s]
  Double_t fEfficiency;       // MAPS detection efficiency
  Double_t fMergeDist;        // Merging distance
  Double_t fFakeRate;         // Fake hit rate

  /** IO arrays **/
  TClonesArray* fInputPoints;  // Array of MCPoints (input)

  /** Random generator and Stopwatch **/
  TRandom3 fRandGen;
  TStopwatch fTimer;

  /** Counters **/
  Int_t fNEvents;
  Double_t fNPoints;
  Double_t fNReal;
  Double_t fNBg;
  Double_t fNFake;
  Double_t fNLost;
  Double_t fNMerged;
  Double_t fTime;
  Double_t fReadoutLast;
  Double_t fReadoutCurrent;
  Double_t fReadoutNext;

  // -----   Private methods   ---------------------------------------------

  struct SignalPoint {
    double x;
    double y;
    double z;
    double sigmaX;
    double sigmaY;
    double charge;
    double eloss;
  };


  typedef std::vector<SignalPoint> SignalPointVec;  //!

  SignalPointVec fSignalPoints;  //!

  /** Reinitialisation **/
  virtual void ReInit(CbmMvdSensor* mySensor);


  /** Virtual method Finish **/
  virtual void Finish();


  /** Register the output arrays to the IOManager **/
  void Register();


  /** Clear the hit arrays **/
  void Reset();


  /** Print digitisation parameters **/
  void PrintParameters();


  /** Get MVD geometry parameters from database 
   **@value Number of MVD stations
   **/
  Int_t GetMvdGeometry();

  CbmMvdSensorDigitizerTBTask(const CbmMvdSensorDigitizerTBTask&);
  CbmMvdSensorDigitizerTBTask operator=(const CbmMvdSensorDigitizerTBTask&);

  ClassDef(CbmMvdSensorDigitizerTBTask, 1);
};


#endif
