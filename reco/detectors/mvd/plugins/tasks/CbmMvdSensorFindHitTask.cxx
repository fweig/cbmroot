/* Copyright (C) 2013-2019 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer], Florian Uhlig */

// ---------------------------------------------------------------------------------------------
// -----                    CbmMvdSensorFindHitTask source file                            -----
// -----                      Created 11/09/13  by P.Sitzmann                              -----
// -----      				 						   -----
// ---------------------------------------------------------------------------------------------
// Includes from MVD
#include "CbmMvdSensorFindHitTask.h"

#include "CbmMvdCluster.h"
#include "CbmMvdHit.h"
#include "CbmMvdPileupManager.h"
#include "CbmMvdPoint.h"

// Includes from base
#include "CbmMCTrack.h"

#include "FairGeoNode.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

// Includes from ROOT
#include "TArrayD.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TGeoManager.h"
#include "TGeoTube.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom3.h"
#include "TRefArray.h"
#include "TString.h"
#include "TVector3.h"

// Includes from C++
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

using std::fixed;
using std::ios_base;
using std::left;
using std::map;
using std::pair;
using std::right;
using std::setprecision;
using std::setw;
using std::vector;


// -----   Default constructor   ------------------------------------------
CbmMvdSensorFindHitTask::CbmMvdSensorFindHitTask()
  : CbmMvdSensorTask()
  , fAdcDynamic(200)
  , fAdcOffset(0)
  , fAdcBits(1)
  , fAdcSteps(-1)
  , fAdcStepSize(-1.)
  , fDigis(nullptr)
  , fHits(nullptr)
  , fClusters(new TClonesArray("CbmMvdCluster", 10000))
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
  , fMode(0)
  , fCounter(0)
  , fSigmaNoise(15.)
  , fSeedThreshold(1.)
  , fNeighThreshold(1.)
  , fShowDebugHistos(kFALSE)
  , fUseMCInfo(kFALSE)
  , inputSet(kFALSE)
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
  , fDigisInCluster(-1)
  , fAddNoise(kFALSE)
{
  fPluginIDNumber = 400;
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMvdSensorFindHitTask::CbmMvdSensorFindHitTask(Int_t iMode)
  : CbmMvdSensorTask()
  , fAdcDynamic(200)
  , fAdcOffset(0)
  , fAdcBits(1)
  , fAdcSteps(-1)
  , fAdcStepSize(-1.)
  , fDigis(nullptr)
  , fHits(nullptr)
  , fClusters(new TClonesArray("CbmMvdCluster", 100))
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
  , fSigmaNoise(15.)
  , fSeedThreshold(1.)
  , fNeighThreshold(1.)
  , fShowDebugHistos(kFALSE)
  , fUseMCInfo(kFALSE)
  , inputSet(kFALSE)
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
  , fDigisInCluster(-1)
  , fAddNoise(kFALSE)
{
  fPluginIDNumber = 400;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdSensorFindHitTask::~CbmMvdSensorFindHitTask()
{

  if (fHits) {
    fHits->Delete();
    delete fHits;
  }

  if (fClusters) {
    fClusters->Delete();
    delete fClusters;
  }

  if (fInputBuffer) {
    fInputBuffer->Delete();
    delete fInputBuffer;
  }
}
// -------------------------------------------------------------------------

// -----    Virtual private method Init   ----------------------------------
void CbmMvdSensorFindHitTask::InitTask(CbmMvdSensor* mysensor)
{


  fSensor = mysensor;
  //LOG(debug) << GetName() << ": Initialisation of sensor " << fSensor->GetName();
  fInputBuffer  = new TClonesArray("CbmMvdDigi", 100);
  fOutputBuffer = new TClonesArray("CbmMvdHit", 100);
  fHits         = new TClonesArray("CbmMvdHit", 100);


  //Add charge collection histograms
  fPixelChargeHistos = new TObjArray();

  fTotalChargeInNpixelsArray = new TObjArray();

  fAdcSteps    = (Int_t) TMath::Power(2, fAdcBits);
  fAdcStepSize = fAdcDynamic / fAdcSteps;

  fGausArray = new Float_t[fGausArrayLimit];
  for (Int_t i = 0; i < fGausArrayLimit; i++) {
    fGausArray[i] = gRandom->Gaus(0, fSigmaNoise);
  };
  fGausArrayIt = 0;


  initialized = kTRUE;

  //LOG(debug) << "-Finished- " << GetName() << ": Initialisation of sensor " << fSensor->GetName();
}
// -------------------------------------------------------------------------

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdSensorFindHitTask::ReInit()
{
  LOG(info) << "CbmMvdSensorFindHitTask::ReInt---------------";
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Virtual public method ExecChain   --------------
void CbmMvdSensorFindHitTask::ExecChain() { Exec(); }
// -------------------------------------------------------------------------

// -----   Virtual public method Exec   --------------
void CbmMvdSensorFindHitTask::Exec()
{
  // if(!inputSet)
  //  {
  //  fInputBuffer->Clear();
  //  fInputBuffer->AbsorbObjects(fPreviousPlugin->GetOutputArray());
  //  LOG(debug) << "absorbt object from previous plugin at " << fSensor->GetName() << " got " << fInputBuffer->GetEntriesFast() << " entrys";
  //  }
  if (fInputBuffer->GetEntriesFast() > 0) {
    fHits->Clear("C");
    fOutputBuffer->Clear();
    fClusters->Clear("C");
    inputSet                    = kFALSE;
    vector<Int_t>* clusterArray = new vector<Int_t>;

    CbmMvdDigi* digi = nullptr;

    Int_t iDigi = 0;
    digi        = (CbmMvdDigi*) fInputBuffer->At(iDigi);


    if (!digi) { LOG(error) << "CbmMvdSensorFindHitTask - Fatal: No Digits found in this event."; }

    Int_t nDigis = fInputBuffer->GetEntriesFast();

    //LOG(debug) << "working with " << nDigis << " entries";


    if (fAddNoise == kTRUE) {
      // Generate random number and call it noise
      // add the noise to the charge of the digis

      LOG(info) << "CbmMvdSensorFindHitTask: Calling method AddNoiseToDigis()..."

        for (iDigi = 0; iDigi < nDigis; iDigi++)
      {

        digi = (CbmMvdDigi*) fInputBuffer->At(iDigi);
        AddNoiseToDigis(digi);
      }
    }

    if (fMode == 1) {
      // GenerateFakeDigis(pixelSizeX, pixelSizeY); // -------- Create Fake Digis -
      //LOG(debug) << "generate fake digis";
    }


    nDigis             = fInputBuffer->GetEntriesFast();
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
      // LOG(debug) << "registerde pixel x:" << digi->GetPixelX() << " y:" << digi->GetPixelY();
      fDigiMap[a] = k;
    };


    LOG(debug) << GetName() << ": VolumeId " << fSensor->GetVolumeId();

    for (iDigi = 0; iDigi < nDigis; iDigi++) {

      LOG_If(debug, iDigi % 10000 == 0) << GetName() << " Digi:" << iDigi;

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

      LOG(debug) << "CbmMvdSensorFindHitTask: Checking for seed pixels...";

      if ((GetAdcCharge(digi->GetCharge()) >= fSeedThreshold) && (pixelUsed->At(iDigi) == kFALSE)) {
        clusterArray->clear();
        clusterArray->push_back(iDigi);


        pixelUsed->AddAt(1, iDigi);

        pair<Int_t, Int_t> a(digi->GetPixelX(), digi->GetPixelY());
        fDigiMapIt = fDigiMap.find(a);
        fDigiMap.erase(fDigiMapIt);

        for (ULong64_t iCluster = 0; iCluster < clusterArray->size(); iCluster++) {

          LOG(debug) << " CbmMvdSensorFindHitTask: Calling method CheckForNeighbours()...";

          CheckForNeighbours(clusterArray, iCluster, pixelUsed);
        }

        //Calculate the center of gravity of all pixels in the cluster.
        TVector3 pos(0, 0, 0);
        TVector3 dpos(0, 0, 0);

        LOG(debug) << " CbmMvdSensorFindHitTask: Calling method CreateHit()...";

        CreateHit(clusterArray, pos, dpos);


      }       // if AdcCharge>threshold
      else {  //LOG(debug) << "pixel is with " <<  digi->GetCharge() << " under Threshold or used";
      }
    }  // loop on digis


    //----------------------------------------------------------------------------------
    //------------- End of Detector Loops ----------------------------------------------
    //----------------------------------------------------------------------------------

    // LOG(debug) << "End of task " << GetName() << ": Event Nr: " << fNEvent << ", nDIGIS: "<<nDigis << ", nHits:"<<fHits->GetEntriesFast();


    /*LOG(debug) <<  "registered " << fHits->GetEntriesFast() 
	         << " new hits out of " << fInputBuffer->GetEntriesFast() 
	         << " Digis on sensor " 
	         << fSensor->GetName(); */

    delete pixelUsed;
    clusterArray->clear();
    delete clusterArray;
    fInputBuffer->Clear();

    fDigiMap.clear();
  }
  else {  //LOG(debug) << "No input found.";
  }
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
void CbmMvdSensorFindHitTask::AddNoiseToDigis(CbmMvdDigi* digi)
{
  Double_t noise = fGausArray[fGausArrayIt++];  // noise is simulated by a gauss
  if (fGausArrayIt - 2 > fGausArrayLimit) { fGausArrayIt = 0; };
  Double_t charge = digi->GetCharge() + noise;
  digi->SetCharge((int) charge);
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//void CbmMvdSensorFindHitTask::GenerateFakeDigis( Double_t pixelSizeX, Double_t pixelSizeY){

//max index of pixels
//Int_t nx = TMath::Nint(2*fLayerRadius/pixelSizeX);
//Int_t ny = TMath::Nint(2*fLayerRadius/pixelSizeY);

//cdritsa: parametrise geometry: 15/12/08
/*     Double_t layerRadius = station->GetRmax();
//     Double_t layerRadiusInner = station->GetRmin();

    Int_t nx = int(2*layerRadius/pixelSizeX);
    Int_t ny = int(2*layerRadius/pixelSizeY);

    Double_t x;
    Double_t y;
    Double_t distance2;
    Double_t noise;
    Double_t r2       = layerRadius*layerRadius;
    Double_t r2_inner = layerRadiusInner*layerRadiusInner;

    for( Int_t i=0; i<nx; i++){

	x = (i+0.5)*pixelSizeX - layerRadius;

	for( Int_t j=0; j<ny; j++){

	    y = (j+0.5)*pixelSizeY - layerRadius;

	    distance2 =  x*x + y*y ;


	    if(  distance2>r2 || distance2<r2_inner )  continue;

	   noise  = fGausArray[fGausArrayIt++]; // noise is simulated by a gauss
    	   if (fGausArrayIt-2>fGausArrayLimit){fGausArrayIt=0;}; 

	    if ( noise>fSeedThreshold && //pixel is not used ???){
                 Int_t nDigis = fInputBuffer->GetEntriesFast();
		 CbmMvdDigi* fakeDigi= 
		       new ((*fInputBuffer)[nDigis]) CbmMvdDigi(station->GetVolumeId(), i,j, noise, pixelSizeX,pixelSizeY);
		 
		 Int_t data[5]; 
		 Float_t data2[5];
		 
		
		       
		       
		}
	}
    }



}*/

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

void CbmMvdSensorFindHitTask::CheckForNeighbours(vector<Int_t>* clusterArray, Int_t clusterDigi, TArrayS* pixelUsed)
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

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void CbmMvdSensorFindHitTask::CreateHit(vector<Int_t>* clusterArray, TVector3& pos, TVector3& dpos)
{

  //loop on cluster array elements
  //calculate the CoG for this cluster

  Int_t clusterSize = clusterArray->size();
  Int_t digiIndex   = 0;
  //LOG(debug) << "try to create hit from " << clusterSize << " pixels";
  CbmMvdDigi* pixelInCluster = (CbmMvdDigi*) fInputBuffer->At(clusterArray->at(digiIndex));
  Int_t thisRefID            = pixelInCluster->GetRefId();
  while (thisRefID < 0 && digiIndex < clusterSize) {
    pixelInCluster = (CbmMvdDigi*) fInputBuffer->At(clusterArray->at(digiIndex));
    thisRefID      = pixelInCluster->GetRefId();
    digiIndex++;
  }
  if (thisRefID < 0)
    LOG(fatal) << "RefID of this digi is -1 this should not happend checked " << digiIndex << " digis in this Cluster";

  // Calculate the center of gravity of the charge of a cluster

  ComputeCenterOfGravity(clusterArray, pos, dpos);


  Int_t indexX, indexY;
  //Double_t x,y;
  Double_t local[2];
  local[0] = pos.X();
  local[1] = pos.Y();
  //LOG(debug) << "found center of gravity at: " << local[0] << " , " << local[1];

  fSensor->TopToPixel(local, indexX, indexY);

  //LOG(debug) << "Center is on pixel: " << indexX << " , " << indexY;
  //Fill HitClusters

  Int_t i          = 0;
  Int_t* digiArray = new Int_t[fDigisInCluster];

  for (i = 0; i < fDigisInCluster; i++) {
    digiArray[i] = 0;
  };

  Int_t latestClusterIndex = -1;
  Int_t digisInArray       = 0;

  CbmMvdCluster* latestCluster = nullptr;
  Int_t nClusters              = -1;

  for (i = 0; i < clusterSize; i++) {

    digiArray[i % fDigisInCluster] = clusterArray->at(i);
    digisInArray                   = digisInArray + 1;

    if (digisInArray == fDigisInCluster) {  // intermediate buffer full

      nClusters = fClusters->GetEntriesFast();
      CbmMvdCluster* clusterNew =
        new ((*fClusters)[nClusters]) CbmMvdCluster(digiArray, digisInArray, clusterSize, latestClusterIndex);
      if (latestCluster) { latestCluster->SetNeighbourUp(nClusters); }
      latestCluster      = clusterNew;
      latestClusterIndex = nClusters;
      digisInArray       = 0;
    }
  }

  if (digisInArray != 0) {
    nClusters = fClusters->GetEntriesFast();
    CbmMvdCluster* clusterNew =
      new ((*fClusters)[nClusters]) CbmMvdCluster(digiArray, digisInArray, clusterSize, latestClusterIndex);
    clusterNew->SetNeighbourUp(-1);
    if (latestCluster) { latestCluster->SetNeighbourUp(nClusters); };
  };


  // Save hit into array
  Int_t nHits = fHits->GetEntriesFast();
  //LOG(debug) << "adding new hit to fHits at X: " << pos.X() << " , Y: "<< pos.Y() << " , Z: " << pos.Z() << " , ";
  new ((*fHits)[nHits]) CbmMvdHit(fSensor->GetStationNr(), pos, dpos, indexX, indexY, nClusters, 0);
  CbmMvdHit* currentHit = new CbmMvdHit;
  currentHit            = (CbmMvdHit*) fHits->At(nHits);
  currentHit->SetTime(fSensor->GetCurrentEventTime());
  currentHit->SetTimeError(fSensor->GetIntegrationtime() / 2);
  currentHit->SetRefId(thisRefID);
  if (pixelInCluster->GetRefId() < 0)
    LOG(info) << "new hit with refID " << pixelInCluster->GetRefId() << " to hit " << nHits;

  nHits = fOutputBuffer->GetEntriesFast();
  new ((*fOutputBuffer)[nHits]) CbmMvdHit(fSensor->GetStationNr(), pos, dpos, indexX, indexY, nClusters, 0);
  currentHit = (CbmMvdHit*) fOutputBuffer->At(nHits);
  currentHit->SetTime(fSensor->GetCurrentEventTime());
  currentHit->SetTimeError(fSensor->GetIntegrationtime() / 2);
  currentHit->SetRefId(pixelInCluster->GetRefId());

  delete[] digiArray;
}

//--------------------------------------------------------------------------

void CbmMvdSensorFindHitTask::UpdateDebugHistos(vector<Int_t>* clusterArray, Int_t seedIndexX, Int_t seedIndexY)
{
  /************************************************************
    Algorithm for cluster shapes

     ************************************************************/


  Float_t chargeArray3D[fChargeArraySize][fChargeArraySize];
  Float_t chargeArray[fChargeArraySize * fChargeArraySize];
  Short_t seedPixelOffset = fChargeArraySize / 2;  // 3 for 7, 2 for 5
  Float_t xCentralTrack   = 0.0;
  Float_t yCentralTrack   = 0.0;
  Float_t clusterCharge   = 0;

  Int_t clusterSize = clusterArray->size();

  for (Int_t k = 0; k < fChargeArraySize; k++) {
    for (Int_t j = 0; j < fChargeArraySize; j++) {
      chargeArray3D[k][j] = gRandom->Gaus(0, fSigmaNoise);
    }
  }

  for (Int_t k = 0; k < clusterSize; k++) {
    CbmMvdDigi* digi = (CbmMvdDigi*) fInputBuffer->At(clusterArray->at(k));

    clusterCharge = clusterCharge + digi->GetCharge();

    Int_t relativeX = digi->GetPixelX() + seedPixelOffset - seedIndexX;
    Int_t relativeY = digi->GetPixelY() + seedPixelOffset - seedIndexY;

    //for debugging
    //LOG(debug) << relativeX << " " << relativeY << " " <<digi->GetPixelX()<< " " << seedIndexX;


    if (relativeX >= 0 && relativeX < fChargeArraySize && relativeY >= 0 && relativeY < fChargeArraySize) {
      chargeArray3D[relativeX][relativeY] = digi->GetCharge();
    }

    if ((relativeX - seedPixelOffset == 0) && (relativeY - seedPixelOffset == 0)) {  //seed digiArray
    }
  }

  //for debugging
  //for(Int_t i=0;i<fChargeArraySize;i++)
  //{for (Int_t j=0;j<fChargeArraySize;j++) {LOG(debug)  << chargeArray3D[i][j] << " " ;}
  //}

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

  if (fHitPosX - xCentralTrack > 0.003 && fHitPosZ < 6) { fBadHitHisto->Fill(fHitPosX, fHitPosY); }

  fResolutionHistoX->Fill(fHitPosX - xCentralTrack);
  fResolutionHistoY->Fill(fHitPosY - yCentralTrack);


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
  };
}


//--------------------------------------------------------------------------

void CbmMvdSensorFindHitTask::ComputeCenterOfGravity(vector<Int_t>* clusterArray, TVector3& pos, TVector3& dpos)
{
  Double_t numeratorX  = 0;
  Double_t numeratorY  = 0;
  Double_t denominator = 0;
  Double_t pixelSizeX  = 0;
  Double_t pixelSizeY  = 0;
  Int_t charge;
  Int_t xIndex;
  Int_t yIndex;
  Double_t x, y;
  Double_t layerPosZ = fSensor->GetZ();
  CbmMvdDigi* pixelInCluster;
  Double_t lab[3] = {0, 0, 0};

  Int_t clusterSize = clusterArray->size();

  for (Int_t iCluster = 0; iCluster < clusterSize; iCluster++) {

    pixelInCluster = (CbmMvdDigi*) fInputBuffer->At(clusterArray->at(iCluster));


    charge     = GetAdcCharge(pixelInCluster->GetCharge());
    xIndex     = pixelInCluster->GetPixelX();
    yIndex     = pixelInCluster->GetPixelY();
    pixelSizeX = pixelInCluster->GetPixelSizeX();
    pixelSizeY = pixelInCluster->GetPixelSizeY();

    LOG(debug) << "CbmMvdSensorFindHitTask:: iCluster= " << iCluster << " , clusterSize= " << clusterSize;
    LOG(debug) << "CbmMvdSensorFindHitTask::xIndex " << xIndex << " , yIndex " << yIndex
               << " , charge = " << pixelInCluster->GetAdcCharge(fAdcDynamic, fAdcOffset, fAdcBits);
  }

  fSensor->PixelToTop(xIndex, yIndex, lab);

  x = lab[0];
  y = lab[1];

  //LOG(debug) << "x = " << x << " y = " << y;
  //Calculate x,y coordinates of the pixel in the detector ref frame
  //Double_t x = ( 0.5+double(xIndex) )*pixelSizeX;
  //Double_t y = ( 0.5+double(yIndex) )*pixelSizeY;

  Double_t xc = x * charge;
  Double_t yc = y * charge;


  numeratorX += xc;
  numeratorY += yc;
  denominator += charge;
}

LOG(debug) << "CbmMvdSensorFindHitTask::=========================";
LOG(debug) << "CbmMvdSensorFindHitTask::numeratorX: " << numeratorX << " , numeratorY: " << numeratorY
           << ", denominator: " << denominator;

//Calculate x,y coordinates of the pixel in the laboratory ref frame
if (denominator != 0) {
  fHitPosX = (numeratorX / denominator);
  fHitPosY = (numeratorY / denominator);
  fHitPosZ = layerPosZ;
}
else {
  fHitPosX = 0;
  fHitPosY = 0;
  fHitPosZ = 0;
}
LOG(debug) << "CbmMvdSensorFindHitTask::-----------------------------------";
LOG(debug) << "CbmMvdSensorFindHitTask::X hit= " << fHitPosX << " Y hit= " << fHitPosY << " Z hit= " << fHitPosZ;
LOG(debug) << "CbmMvdSensorFindHitTask::-----------------------------------";

// pos = center of gravity (labframe), dpos uncertainty
pos.SetXYZ(fHitPosX, fHitPosY, fHitPosZ);
dpos.SetXYZ(fHitPosErrX, fHitPosErrY, fHitPosErrZ);
}

//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
void CbmMvdSensorFindHitTask::Finish()
{
  if (fShowDebugHistos) {
    LOG(info) << "============================================================";
    LOG(info) << GetName() << "::Finish: Total events skipped: " << fCounter;
    LOG(info) << "============================================================";
    LOG(info) << "Parameters used";
    LOG(info) << "Gaussian noise [electrons]	: " << fSigmaNoise;
    LOG(info) << "Noise simulated [Bool]	: " << fAddNoise;
    LOG(info) << "Threshold seed [ADC]       : " << fSeedThreshold;
    LOG(info) << "Threshold neighbours [ADC]	: " << fNeighThreshold;
    LOG(info) << "ADC - Bits			: " << fAdcBits;
    LOG(info) << "ADC - Dynamic [electrons]	: " << fAdcDynamic;
    LOG(info) << "ADC - Offset [electrons]	: " << fAdcOffset;
    LOG(info) << "============================================================";


    TH1F* histo;
    TH2F* clusterShapeHistogram;


    TCanvas* canvas2 = new TCanvas("HitFinderCharge", "HitFinderCharge");
    //LOG(debug) <<fChargeArraySize;
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
        // 	  Double_t MPV=fitFunction->GetParameter(1);
        clusterShapeHistogram->Fill(i % fChargeArraySize, i / fChargeArraySize, charge);
        //canvas2->cd(i);
        //histo->Draw();
      }
    }

    clusterShapeHistogram->Draw("Lego2");
    canvas2->cd(2);
    histo = (TH1F*) fPixelChargeHistos->At(24);
    histo->Draw();
    //LOG(debug) <<"Mean charge" << histo->GetMean();
    /*    
      TCanvas* canvas=new TCanvas("HitFinderCanvas","HitFinderCanvas");
      canvas->Divide (2,3);
      canvas->cd(1);
      fResolutionHistoX->Draw();
      fResolutionHistoX->Write();
      canvas->cd(2);
      fResolutionHistoY->Draw();
      fResolutionHistoY->Write();
      canvas->cd(3);
      ((TH1F*)fPixelChargeHistos->At(49))->Draw();
      ((TH1F*)fPixelChargeHistos->At(49))->Fit("landau");
      canvas->cd(4);
      fFullClusterHisto->Draw();
      canvas->cd(5);
      ((TH1F*)fTotalChargeInNpixelsArray->At(0))->Draw();
      ((TH1F*)fTotalChargeInNpixelsArray->At(0))->Fit("landau");
	//fResolutionHistoMergedX->Write();
      canvas->cd(6);
      clusterShapeHistogram->Draw("Lego2");
	//fResolutionHistoMergedY->Draw();
	//fResolutionHistoMergedY->Write();*/
  }
}
//--------------------------------------------------------------------------

Int_t CbmMvdSensorFindHitTask::GetAdcCharge(Float_t charge)
{

  Int_t adcCharge;

  if (charge < fAdcOffset) { return 0; };

  adcCharge = int((charge - fAdcOffset) / fAdcStepSize);
  if (adcCharge > fAdcSteps - 1) { adcCharge = fAdcSteps - 1; }

  return adcCharge;
}


// -----   Private method Reset   ------------------------------------------
void CbmMvdSensorFindHitTask::Reset()
{
  fHits->Clear("C");
  fClusters->Clear("C");
}

// -------------------------------------------------------------------------


ClassImp(CbmMvdSensorFindHitTask);
