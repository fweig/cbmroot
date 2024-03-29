/* Copyright (C) 2014-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                  CbmMvdSensorClusterfinderTask source file    -----
// -----                  Created 03.12.2014 by P. Sitzmann            -----
// -------------------------------------------------------------------------

#include "CbmMvdSensorClusterfinderTask.h"

#include <Logger.h>

#include "TClonesArray.h"
#include "TObjArray.h"

using std::endl;
using std::pair;
using std::vector;

// -----   Default constructor   -------------------------------------------
CbmMvdSensorClusterfinderTask::CbmMvdSensorClusterfinderTask() : CbmMvdSensorClusterfinderTask(0, 0)
{

  fPluginIDNumber = 200;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMvdSensorClusterfinderTask::~CbmMvdSensorClusterfinderTask() { ; }
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMvdSensorClusterfinderTask::CbmMvdSensorClusterfinderTask(Int_t iMode, Int_t iVerbose)
  : CbmMvdSensorTask()
  , fAdcDynamic(200)
  , fAdcOffset(0)
  , fAdcBits(1)
  , fAdcSteps(-1)
  , fAddress(0)
  , fAdcStepSize(-1.)
  , fDigis(nullptr)
  , fPixelChargeHistos(nullptr)
  , fTotalChargeInNpixelsArray(nullptr)
  , fResolutionHistoX(nullptr)
  , fResolutionHistoY(nullptr)
  , fResolutionHistoCleanX(nullptr)
  , fResolutionHistoCleanY(nullptr)
  , fResolutionHistoMergedX(nullptr)
  , fResolutionHistoMergedY(nullptr)
  , fBadHitHisto(nullptr)
  , fGausArray(nullptr)
  , fGausArrayIt(-1)
  , fGausArrayLimit(5000)
  , fDigiMap()
  , fDigiMapIt()
  , h(nullptr)
  , h3(nullptr)
  , h1(nullptr)
  , h2(nullptr)
  , Qseed(nullptr)
  , fFullClusterHisto(nullptr)
  , c1(nullptr)
  , fNEvent(0)
  , fMode(iMode)
  , fCounter(0)
  , fVerbose(iVerbose)
  , fSigmaNoise(15.)
  , fSeedThreshold(1.)
  , fNeighThreshold(1.)
  , fUseMCInfo(kFALSE)
  , inputSet(kFALSE)
  , ftempPixelMap()
  , fLayerRadius(0.)
  , fLayerRadiusInner(0.)
  , fLayerPosZ(0.)
  , fHitPosX(0.)
  , fHitPosY(0.)
  , fHitPosZ(0.)
  , fHitPosErrX(0.0005)
  , fHitPosErrY(0.0005)
  , fHitPosErrZ(0.0)
  , fBranchName("MvdHit")
  , fAddNoise(kFALSE)
{
  fPluginIDNumber = 200;
}
// -------------------------------------------------------------------------

// -----    Virtual private method Init   ----------------------------------
void CbmMvdSensorClusterfinderTask::InitTask(CbmMvdSensor* mysensor)
{


  fSensor = mysensor;
  LOG(debug) << "CbmMvdSensorClusterfinderTask : Initialisation of sensor " << fSensor->GetName();
  fInputBuffer  = new TClonesArray("CbmMvdDigi", 100);
  fOutputBuffer = new TClonesArray("CbmMvdCluster", 100);


  //Add charge collection histograms
  fPixelChargeHistos = new TObjArray();

  fTotalChargeInNpixelsArray = new TObjArray();

  fAdcSteps    = (Int_t) TMath::Power(2, fAdcBits);
  fAdcStepSize = fAdcDynamic / fAdcSteps;

  fAddress = 1000 * fSensor->GetStationNr() + fSensor->GetSensorNr();

  initialized = kTRUE;

  if (fShowDebugHistos) {
    fResolutionHistoX = new TH1F("SinglePointResolution_X", "SinglePointResolution_X", 10000, -0.0100, 0.0100);
    fResolutionHistoY = new TH1F("SinglePointResolution_Y", "SinglePointResolution_Y", 10000, -0.0100, 0.0100);
    fResolutionHistoCleanX =
      new TH1F("SinglePointResolution_X_Clean", "SinglePointResolution_X_Clean", 10000, -0.0100, 0.0100);
    fResolutionHistoCleanY =
      new TH1F("SinglePointResolution_Y_Clean", "SinglePointResolution_Y_Clean", 10000, -0.0100, 0.0100);
    fResolutionHistoMergedX =
      new TH1F("SinglePointResolution_X_Merged", "SinglePointResolution_X_Merged", 10000, -0.0100, 0.0100);
    fResolutionHistoMergedY =
      new TH1F("SinglePointResolution_Y_Merged", "SinglePointResolution_Y_Merged", 10000, -0.0100, 0.0100);
    fBadHitHisto      = new TH2F("BadHits", "Hits above 0.003cm", 1000, -2.5, 2.5, 1000, -2.5, 2.5);
    fFullClusterHisto = new TH1F("ChargeOfAllPixels", "ChargeOfAllPixels", 12000, 0, 12000);
    //}


    //Add charge collection histograms
    fPixelChargeHistos = new TObjArray();
    size_t buf_size    = 20;
    char* histoName    = new char[buf_size];
    TH1F* histo;
    for (Int_t i = 0; i < 49; i++) {
      snprintf(histoName, buf_size - 1, "ChargePixel%i", i + 1);
      histo = new TH1F(histoName, histoName, 200, 0, 200);
      fPixelChargeHistos->AddLast(histo);
    };
    delete[] histoName;

    fTotalChargeInNpixelsArray = new TObjArray();
    buf_size                   = 50;
    char* histoTotalChargeName = new char[buf_size];
    TH1F* histoTotalCharge;
    for (Int_t i = 0; i < 49; i++) {
      snprintf(histoTotalChargeName, buf_size - 1, "totalChargeInNPixels%i", i + 1);
      histoTotalCharge = new TH1F(histoTotalChargeName, histoTotalChargeName, 12000, 0, 12000);
      fTotalChargeInNpixelsArray->AddLast(histoTotalCharge);
    };
    delete[] histoTotalChargeName;

    //Number 49
    histo = new TH1F("ChargePixelSeed", "ChargePixelSeed", 200, 0, 14000);
    fPixelChargeHistos->AddLast(histo);
    //Number 50
    histo = new TH1F("ChargePixel9of49", "ChargePixel 9 Of 49", 200, 0, 14000);
    fPixelChargeHistos->AddLast(histo);
    //Number 51
    histo = new TH1F("ChargePixel25of49", "ChargePixel 25 Of 49", 200, 0, 14000);
    fPixelChargeHistos->AddLast(histo);
    //Number 52
    histo = new TH1F("ChargePixel49of49", "ChargePixel 49 Of 49", 200, 0, 14000);
    fPixelChargeHistos->AddLast(histo);
    //Number 53
    histo = new TH1F("ChargePixel9of49Sorted", "ChargePixel 9 Of 49 Sorted", 200, 0, 14000);
    fPixelChargeHistos->AddLast(histo);
    //Number 54
    histo = new TH1F("ChargePixel25of49Sorted", "ChargePixel 25 Of 49 Sorted", 200, 0, 14000);
    fPixelChargeHistos->AddLast(histo);
    //Number 55
    histo = new TH1F("ChargePixel49of49Sorted", "ChargePixel 49 Of 49 Sorted", 49, 0.5, 49.5);
    fPixelChargeHistos->AddLast(histo);
  }
  //LOG(debug) << "-Finished- " << GetName() << ": Initialisation of sensor " << fSensor->GetName();
}
// -------------------------------------------------------------------------

// -----   Virtual public method Reinit   ----------------------------------
Bool_t CbmMvdSensorClusterfinderTask::ReInit()
{
  LOG(info) << "CbmMvdSensorClusterfinderTask::ReInt---------------";
  return kTRUE;
}
// -------------------------------------------------------------------------

// -----   Virtual public method ExecChain   --------------
void CbmMvdSensorClusterfinderTask::ExecChain() { Exec(); }
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------
void CbmMvdSensorClusterfinderTask::Exec()
{
  LOG(debug) << "CbmMvdSensorClusterfinderTask::Exec - Running Sensor " << fSensor->GetName();
  LOG(debug) << "CbmMvdSensorClusterfinderTask::Exec - InputBufferSize " << fInputBuffer->GetEntriesFast();

  if (fInputBuffer->GetEntriesFast() > 0) {
    fOutputBuffer->Delete();
    inputSet                    = kFALSE;
    vector<Int_t>* clusterArray = new vector<Int_t>;

    CbmMvdDigi* digi;

    Int_t iDigi = 0;
    digi        = (CbmMvdDigi*) fInputBuffer->At(iDigi);


    if (!digi) { LOG(error) << "CbmMvdSensorFindHitTask - Fatal: No Digits found in this event."; }

    Int_t nDigis = fInputBuffer->GetEntriesFast();
    //LOG(debug) << "CbmMvdClusterTrask::Exec(): Received following number of digis: " << nDigis;
    TArrayS* pixelUsed = new TArrayS(nDigis);

    for (iDigi = 0; iDigi < nDigis; iDigi++) {
      pixelUsed->AddAt(0, iDigi);
    }

    fDigiMap.clear();
    Int_t refId;
    for (Int_t k = 0; k < nDigis; k++) {

      digi  = (CbmMvdDigi*) fInputBuffer->At(k);
      refId = digi->GetRefId();
      if (refId < 0) { LOG(fatal) << "RefID of this digi is -1 this should not happend "; }
      //apply fNeighThreshold

      if (GetAdcCharge(digi->GetCharge()) < fNeighThreshold) continue;

      pair<Int_t, Int_t> a(digi->GetPixelX(), digi->GetPixelY());
      //LOG(debug) << "registerde pixel x:" << digi->GetPixelX() << " y:" << digi->GetPixelY();
      fDigiMap[a] = k;
    };


    LOG(debug) << GetName() << ": VolumeId " << fSensor->GetVolumeId();

    //LOG(debug) <<"CbmMvdSensorClusterfinderTask: Working with " << nDigis << " digis";

    for (iDigi = 0; iDigi < nDigis; iDigi++) {

      LOG_IF(debug, iDigi % 10000 == 0) << GetName() << " Digi:" << iDigi;

      digi = (CbmMvdDigi*) fInputBuffer->At(iDigi);
      //LOG(debug) << "working with pixel x:" << digi->GetPixelX() << " y:" << digi->GetPixelY();


      /*
	     ---------------------------------------------------------
	     check if digi is above threshold (define seed pixel)
	     then check for neighbours.
	     Once the cluster is created (seed and neighbours)
	     calculate the position of the hit
	     using center of gravity (CoG) method.
	     ---------------------------------------------------------
	     */

      LOG(debug) << "CbmMvdSensorClusterfinderTask: Checking for seed pixels...";

      if ((GetAdcCharge(digi->GetCharge()) >= fSeedThreshold) && (pixelUsed->At(iDigi) == kFALSE)) {
        clusterArray->clear();
        clusterArray->push_back(iDigi);

        pixelUsed->AddAt(1, iDigi);

        pair<Int_t, Int_t> a(digi->GetPixelX(), digi->GetPixelY());
        fDigiMapIt = fDigiMap.find(a);
        if (fDigiMapIt != fDigiMap.end()) { fDigiMap.erase(fDigiMapIt); };

        for (ULong64_t iCluster = 0; iCluster < clusterArray->size(); iCluster++) {

          LOG(debug) << " CbmMvdSensorClusterfinderTask: Calling method CheckForNeighbours()...";

          CheckForNeighbours(clusterArray, iCluster, pixelUsed);
          //LOG(debug) << "checked for neighbours, create cluster";
        }

        Int_t i = 0;
        Int_t pixelCharge;


        pair<Int_t, Int_t> pixelCoords;
        Int_t clusterSize = clusterArray->size();
        Int_t nClusters   = fOutputBuffer->GetEntriesFast();
        //LOG(debug) << "new cluster: " << nClusters;
        CbmMvdCluster* clusterNew = new ((*fOutputBuffer)[nClusters]) CbmMvdCluster();
        clusterNew->SetAddress(fAddress);

        for (i = 0; i < clusterSize; i++) {
          CbmMvdDigi* digiInCluster = (CbmMvdDigi*) fInputBuffer->At(clusterArray->at(i));
          clusterNew->AddDigi(digiInCluster->GetRefId());
          pixelCoords                = std::make_pair(digiInCluster->GetPixelX(), digiInCluster->GetPixelY());
          pixelCharge                = digiInCluster->GetCharge();
          ftempPixelMap[pixelCoords] = pixelCharge;
        }
        clusterNew->SetPixelMap(ftempPixelMap);
        ftempPixelMap.clear();

        if (fShowDebugHistos) UpdateDebugHistos(clusterNew);

      }       // if AdcCharge>threshold
      else {  //LOG(debug) << "pixel is with " <<  digi->GetCharge() << " under Threshold or used";
      }
    }  // loop on digis


    delete pixelUsed;
    clusterArray->clear();
    delete clusterArray;
    fInputBuffer->Delete();

    fDigiMap.clear();
  }
  else {  //LOG(debug) << "No input found.";
  }
  fInputBuffer->Clear();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdSensorClusterfinderTask::CheckForNeighbours(vector<Int_t>* clusterArray, Int_t clusterDigi,
                                                       TArrayS* pixelUsed)
{
  CbmMvdDigi* seed = (CbmMvdDigi*) fInputBuffer->At(clusterArray->at(clusterDigi));
  //LOG(debug) << "pixel nr. " << clusterDigi << " is seed";


  // Remove Seed Pixel from list of non-used pixels
  Int_t channelX = seed->GetPixelX();
  Int_t channelY = seed->GetPixelY();
  pair<Int_t, Int_t> a(channelX, channelY);

  // Find first neighbour

  a          = std::make_pair(channelX + 1, channelY);
  fDigiMapIt = fDigiMap.find(a);

  if (!(fDigiMapIt == fDigiMap.end())) {
    Int_t i = fDigiMap[a];
    //LOG(debug) << "pixel nr. " << i << " is used";
    // Only digis depassing fNeighThreshold are in the map, no cut required
    clusterArray->push_back(i);

    pixelUsed->AddAt(1, i);      // block pixel for the seed pixel scanner
    fDigiMap.erase(fDigiMapIt);  // block pixel for the neighbour pixel scanner
  }

  a          = std::make_pair(channelX - 1, channelY);
  fDigiMapIt = fDigiMap.find(a);

  if (!(fDigiMapIt == fDigiMap.end())) {
    Int_t i = fDigiMap[a];
    //LOG(debug) << "pixel nr. " << i << " is used";
    // Only digits depassing fNeighThreshold are in the map, no cut required
    clusterArray->push_back(i);
    pixelUsed->AddAt(1, i);      // block pixel for the seed pixel scanner
    fDigiMap.erase(fDigiMapIt);  // block pixel for the neighbour pixel scanner
  }

  a          = std::make_pair(channelX, channelY - 1);
  fDigiMapIt = fDigiMap.find(a);
  if (!(fDigiMapIt == fDigiMap.end())) {
    Int_t i = fDigiMap[a];
    // Only digits depassing fNeighThreshold are in the map, no cut required
    //LOG(debug) << "pixel nr. " << i << " is used";
    clusterArray->push_back(i);
    pixelUsed->AddAt(1, i);      // block pixel for the seed pixel scanner
    fDigiMap.erase(fDigiMapIt);  // block pixel for the neighbour pixel scanner
  }

  a          = std::make_pair(channelX, channelY + 1);
  fDigiMapIt = fDigiMap.find(a);

  if (!(fDigiMapIt == fDigiMap.end())) {
    Int_t i = fDigiMap[a];
    //LOG(debug) << "pixel nr. " << i << " is used";
    // Only digis depassing fNeighThreshold are in the map, no cut required
    clusterArray->push_back(i);
    pixelUsed->AddAt(1, i);      // block pixel for the seed pixel scanner
    fDigiMap.erase(fDigiMapIt);  // block pixel for the neighbour pixel scanner
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Int_t CbmMvdSensorClusterfinderTask::GetAdcCharge(Float_t charge)
{

  Int_t adcCharge;

  if (charge < fAdcOffset) { return 0; };

  adcCharge = int((charge - fAdcOffset) / fAdcStepSize);
  if (adcCharge > fAdcSteps - 1) { adcCharge = fAdcSteps - 1; }

  return adcCharge;
}
// -------------------------------------------------------------------------

void CbmMvdSensorClusterfinderTask::UpdateDebugHistos(CbmMvdCluster* cluster)
{
  /************************************************************
    Algorithm for cluster shapes

     ************************************************************/
  Float_t chargeArray3D[fChargeArraySize][fChargeArraySize];
  Float_t chargeArray[fChargeArraySize * fChargeArraySize];
  Short_t seedPixelOffset = fChargeArraySize / 2;  // 3 for 7, 2 for 5
  Int_t seedIndexX = 0, seedIndexY = 0;
  Float_t seedCharge                                  = 0.;
  Float_t clusterCharge                               = cluster->GetClusterCharge();
  std::map<std::pair<Int_t, Int_t>, Int_t> clusterMap = cluster->GetPixelMap();

  for (Int_t k = 0; k < fChargeArraySize; k++) {
    for (Int_t j = 0; j < fChargeArraySize; j++) {
      chargeArray3D[k][j] = gRandom->Gaus(0, fSigmaNoise);
    }
  }
  for (std::map<std::pair<Int_t, Int_t>, Int_t>::iterator iter = clusterMap.begin(); iter != clusterMap.end(); iter++) {
    if (iter->second > seedCharge) {
      seedCharge = iter->second;
      seedIndexX = iter->first.first;
      seedIndexY = iter->first.second;
    }
  }
  //LOG(debug) << "seed pixel with " << seedCharge << " charge";
  for (std::map<std::pair<Int_t, Int_t>, Int_t>::iterator iter = clusterMap.begin(); iter != clusterMap.end(); iter++) {

    Int_t relativeX = iter->first.first + seedPixelOffset - seedIndexX;
    Int_t relativeY = iter->first.second + seedPixelOffset - seedIndexY;

    if (fVerbose > 1) LOG(debug) << relativeX << " " << relativeY << " " << iter->first.first << " " << seedIndexX;


    if (relativeX >= 0 && relativeX < fChargeArraySize && relativeY >= 0 && relativeY < fChargeArraySize) {
      chargeArray3D[relativeX][relativeY] = iter->second;
    }

    if ((relativeX - seedPixelOffset == 0) && (relativeY - seedPixelOffset == 0)) {  //seed digiArray
    }
  }

  if (fVerbose > 1) {
    std::stringstream ss;
    for (Int_t i = 0; i < fChargeArraySize; i++) {
      for (Int_t j = 0; j < fChargeArraySize; j++) {
        ss << chargeArray3D[i][j] << " ";
      }
      ss << endl;
    }
    LOG(info) << ss.str();
  }
  fFullClusterHisto->Fill(clusterCharge);

  for (Int_t k = 0; k < fChargeArraySize; k++) {
    for (Int_t j = 0; j < fChargeArraySize; j++) {
      chargeArray[fChargeArraySize * k + j] = chargeArray3D[k][j];
    }
  }

  Int_t qSeed = chargeArray3D[seedPixelOffset][seedPixelOffset];
  Int_t q9    = 0;

  for (Int_t k = seedPixelOffset - 1; k < seedPixelOffset + 1; k++) {
    for (Int_t j = seedPixelOffset - 1; j < seedPixelOffset + 1; j++) {
      q9 = q9 + chargeArray3D[k][j];
    }
  };


  if (fChargeArraySize <= 7) {
    for (Int_t i = 0; i < (fChargeArraySize * fChargeArraySize); i++) {
      ((TH1F*) fPixelChargeHistos->At(i))->Fill(chargeArray[i]);
      //LOG(debug) << counter++<<" Charge: " << chargeArray[i];
    };
  };

  //LOG(debug) << "End of Cluster: "<<fChargeArraySize*fChargeArraySize;

  Int_t q25 = 0;
  Int_t q49 = 0;


  for (Int_t k = seedPixelOffset - 2; k < seedPixelOffset + 2; k++) {
    for (Int_t j = seedPixelOffset - 2; j < seedPixelOffset + 2; j++) {
      q25 = q25 + chargeArray3D[k][j];
    }
  };

  if (fChargeArraySize >= 7) {
    for (Int_t k = seedPixelOffset - 3; k < seedPixelOffset + 3; k++) {
      for (Int_t j = seedPixelOffset - 3; j < seedPixelOffset + 3; j++) {
        q49 = q49 + chargeArray3D[k][j];
      }
    }
  }

  ((TH1F*) fPixelChargeHistos->At(49))->Fill(qSeed);
  ((TH1F*) fPixelChargeHistos->At(50))->Fill(q9);
  ((TH1F*) fPixelChargeHistos->At(51))->Fill(q25);
  ((TH1F*) fPixelChargeHistos->At(52))->Fill(q49);


  //Prepare selection of crowns for charge bow histograms


  Int_t orderArray[fChargeArraySize * fChargeArraySize];

  TMath::Sort(fChargeArraySize * fChargeArraySize, chargeArray, orderArray, kTRUE);

  Float_t qSort = 0;
  for (Int_t i = 0; i < 9; i++) {
    qSort += chargeArray[orderArray[i]];
  };
  ((TH1F*) fPixelChargeHistos->At(53))->Fill(qSort);

  for (Int_t i = 9; i < 25; i++) {
    qSort += chargeArray[orderArray[i]];
  };
  ((TH1F*) fPixelChargeHistos->At(54))->Fill(qSort);

  TH1F* histoTotalCharge;
  qSort = 0;
  for (Int_t i = 0; i < fChargeArraySize * fChargeArraySize; i++) {
    qSort += chargeArray[orderArray[i]];
    ((TH1F*) fPixelChargeHistos->At(55))->Fill(i + 1, qSort);
    histoTotalCharge = (TH1F*) fTotalChargeInNpixelsArray->At(i);
    histoTotalCharge->Fill(qSort);
  }
}

//--------------------------------------------------------------------------
void CbmMvdSensorClusterfinderTask::Finish()
{

  if (fShowDebugHistos) {
    LOG(info) << "============================================================";
    LOG(info) << GetName() << "::Finish: Total events skipped: " << fCounter;
    LOG(info) << "============================================================";
    LOG(info) << "Parameters used";
    LOG(info) << "Gaussian noise [electrons]	: " << fSigmaNoise;
    LOG(info) << "Noise simulated [Bool]        : " << fAddNoise;
    LOG(info) << "Threshold seed [ADC]          : " << fSeedThreshold;
    LOG(info) << "Threshold neighbours [ADC]	: " << fNeighThreshold;
    LOG(info) << "ADC - Bits			: " << fAdcBits;
    LOG(info) << "ADC - Dynamic [electrons]	: " << fAdcDynamic;
    LOG(info) << "ADC - Offset [electrons]	: " << fAdcOffset;
    LOG(info) << "============================================================";


    TH1F* histo;
    TH2F* clusterShapeHistogram;
    TCanvas* canvas2 = new TCanvas("HitFinderCharge", "HitFinderCharge");
    canvas2->Divide(2, 2);
    canvas2->cd(1);
    if (fChargeArraySize <= 7) {
      clusterShapeHistogram = new TH2F("MvdClusterShape", "MvdClusterShape", fChargeArraySize, 0, fChargeArraySize,
                                       fChargeArraySize, 0, fChargeArraySize);
      for (Int_t i = 0; i < fChargeArraySize * fChargeArraySize; i++) {
        histo          = (TH1F*) fPixelChargeHistos->At(i);
        Float_t charge = histo->GetMean();
        //LOG(debug) <<i << " Charge " << charge << " xCluster: " << i%fChargeArraySize << " yCluster: " << i/fChargeArraySize;
        //histo->Fit("landau");
        //TF1* fitFunction= histo->GetFunction("landau");
        //Double_t MPV=fitFunction->GetParameter(1);
        clusterShapeHistogram->Fill(i % fChargeArraySize, i / fChargeArraySize, charge);
      }
    }
    clusterShapeHistogram->Draw("Lego2");
    canvas2->cd(2);
    histo = (TH1F*) fPixelChargeHistos->At(50);
    histo->Draw();
    canvas2->cd(3);
    histo = (TH1F*) fPixelChargeHistos->At(51);
    histo->Draw();
    canvas2->cd(4);
    //fFullClusterHisto->Draw();
  }
}
//--------------------------------------------------------------------------
ClassImp(CbmMvdSensorClusterfinderTask)
