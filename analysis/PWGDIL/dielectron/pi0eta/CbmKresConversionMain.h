/** @file CbmKresConversionMain.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifndef CBM_KRES_CONVERSION_MAIN
#define CBM_KRES_CONVERSION_MAIN

#include "CbmKFParticleFinder.h"
#include "CbmKFParticleFinderQA.h"
#include "CbmKresConversionCorrectedPhotons.h"
#include "CbmKresConversionGeneral.h"
#include "CbmKresConversionKF.h"
#include "CbmKresConversionManual.h"
#include "CbmKresConversionManualmbias1.h"
#include "CbmKresConversionManualmbias2.h"
#include "CbmKresConversionManualmbias3.h"
#include "CbmKresConversionManualmbias4.h"
#include "CbmKresConversionPhotons.h"
#include "CbmKresConversionReconstruction.h"
#include "CbmKresEta.h"
#include "CbmKresEtaMCAnalysis.h"
#include "CbmKresTemperature.h"

#include "FairMCEventHeader.h"
#include "FairTask.h"

#include <TClonesArray.h>

using namespace std;

class CbmKresConversionMain : public FairTask {

public:
  //***** brief Standard constructor.
  CbmKresConversionMain();

  //***** brief disallow Copy constructor.
  CbmKresConversionMain(const CbmKresConversionMain&) = delete;

  //***** brief disallow Assignment operator.
  CbmKresConversionMain& operator=(const CbmKresConversionMain&) = delete;

  //***** brief Standard destructor.
  virtual ~CbmKresConversionMain();

  //***** brief Inherited from FairTask.
  virtual InitStatus Init();

  //***** brief Inherited from FairTask.
  virtual void Exec(Option_t* option);

  virtual void Finish();

  void SetKF(CbmKFParticleFinder* kfparticle, CbmKFParticleFinderQA* kfparticleQA);


private:
  void InitHistograms();

  Int_t DoKresGeneral;
  Int_t DoKresReconstruction;
  Int_t DoKresKF;
  Int_t DoKresManual;
  Int_t DoKresManualmbias;
  Int_t DoKresTemperature;
  Int_t DoKresPhotons;
  Int_t DoKresCorrectedPhotons;
  Int_t DoKresEtaMCAnalysis;
  Int_t DoKresEta;


  CbmKresConversionGeneral* fKresGeneral;
  CbmKresConversionReconstruction* fKresReco;
  CbmKresConversionKF* fKresKF;
  CbmKresConversionManual* fKresManual;
  CbmKFParticleFinder* fKFparticle;
  CbmKFParticleFinderQA* fKFparticleFinderQA;

  CbmKresConversionManualmbias1* fKresManualmbiasPart1;
  CbmKresConversionManualmbias2* fKresManualmbiasPart2;
  CbmKresConversionManualmbias3* fKresManualmbiasPart3;
  CbmKresConversionManualmbias4* fKresManualmbiasPart4;

  CbmKresTemperature* fKresTemperature;
  CbmKresConversionPhotons* fKresPhotons;
  CbmKresConversionCorrectedPhotons* fKresCorrectedPhotons;
  CbmKresEtaMCAnalysis* fKresEtaMCAnalysis;
  CbmKresEta* fKresEta;


  Int_t fEventNum;
  Double_t OpeningAngleCut;
  Double_t GammaInvMassCut;
  Int_t fRealPID;


  ClassDef(CbmKresConversionMain, 1)
};

#endif
