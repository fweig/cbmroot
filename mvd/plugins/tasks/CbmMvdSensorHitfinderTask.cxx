// ---------------------------------------------------------------------------------------------
// -----                    CbmMvdSensorHitfinderTask source file                            -----
// -----                      Created 11/09/13  by P.Sitzmann                              -----
// -----      				 						   -----
// ---------------------------------------------------------------------------------------------
// Includes from MVD
#include "CbmMvdSensorHitfinderTask.h"

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
#include <TMatrixD.h>

// Includes from C++
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

using std::cout;
using std::endl;
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
CbmMvdSensorHitfinderTask::CbmMvdSensorHitfinderTask()
  : CbmMvdSensorTask()
  , fAdcDynamic(200)
  , fAdcOffset(0)
  , fAdcBits(1)
  , fAdcSteps(-1)
  , fAdcStepSize(-1.)
  , fClusters(new TClonesArray("CbmMvdCluster", 10000))
  , fPixelChargeHistos(NULL)
  , fTotalChargeInNpixelsArray(NULL)
  , fResolutionHistoX(NULL)
  , fResolutionHistoY(NULL)
  , fResolutionHistoCleanX(NULL)
  , fResolutionHistoCleanY(NULL)
  , fResolutionHistoMergedX(NULL)
  , fResolutionHistoMergedY(NULL)
  , fBadHitHisto(NULL)
  , fGausArray(NULL)
  , fGausArrayIt(-1)
  , fGausArrayLimit(5000)
  , fDigiMap()
  , fDigiMapIt()
  , h(NULL)
  , h3(NULL)
  , h1(NULL)
  , h2(NULL)
  , Qseed(NULL)
  , fFullClusterHisto(NULL)
  , c1(NULL)
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
  , fDigisInCluster(0)
  , fAddNoise(kFALSE)
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMvdSensorHitfinderTask::CbmMvdSensorHitfinderTask(Int_t iMode)
  : CbmMvdSensorTask()
  , fAdcDynamic(200)
  , fAdcOffset(0)
  , fAdcBits(1)
  , fAdcSteps(-1)
  , fAdcStepSize(-1.)
  , fClusters(new TClonesArray("CbmMvdCluster", 10000))
  , fPixelChargeHistos(NULL)
  , fTotalChargeInNpixelsArray(NULL)
  , fResolutionHistoX(NULL)
  , fResolutionHistoY(NULL)
  , fResolutionHistoCleanX(NULL)
  , fResolutionHistoCleanY(NULL)
  , fResolutionHistoMergedX(NULL)
  , fResolutionHistoMergedY(NULL)
  , fBadHitHisto(NULL)
  , fGausArray(NULL)
  , fGausArrayIt(-1)
  , fGausArrayLimit(5000)
  , fDigiMap()
  , fDigiMapIt()
  , h(NULL)
  , h3(NULL)
  , h1(NULL)
  , h2(NULL)
  , Qseed(NULL)
  , fFullClusterHisto(NULL)
  , c1(NULL)
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
  , fDigisInCluster(0)
  , fAddNoise(kFALSE)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdSensorHitfinderTask::~CbmMvdSensorHitfinderTask()
{

  if (fOutputBuffer) {
    fOutputBuffer->Delete();
    delete fOutputBuffer;
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
void CbmMvdSensorHitfinderTask::InitTask(CbmMvdSensor* mysensor)
{


  fSensor = mysensor;
  //cout << "-Start- " << GetName() << ": Initialisation of sensor " << fSensor->GetName() << endl;
  fInputBuffer  = new TClonesArray("CbmMvdCluster", 10000);
  fOutputBuffer = new TClonesArray("CbmMvdHit", 10000);


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

  //cout << "-Finished- " << GetName() << ": Initialisation of sensor " << fSensor->GetName() << endl;
}
// -------------------------------------------------------------------------

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdSensorHitfinderTask::ReInit()
{
  cout << "-I- "
       << "CbmMvdSensorHitfinderTask::ReInt---------------" << endl;
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Virtual public method ExecChain   --------------
void CbmMvdSensorHitfinderTask::ExecChain() { Exec(); }
// -------------------------------------------------------------------------

// -----   Virtual public method Exec   --------------
void CbmMvdSensorHitfinderTask::Exec()
{

  // if(!inputSet)
  //  {
  //  fInputBuffer->Clear();
  //  fInputBuffer->AbsorbObjects(fPreviousPlugin->GetOutputArray());
  //  cout << endl << "absorbt object from previous plugin at " << fSensor->GetName() << " got " << fInputBuffer->GetEntriesFast() << " entrys" << endl;
  //  }
  if (fInputBuffer->GetEntriesFast() > 0) {

    fOutputBuffer->Clear();
    inputSet = kFALSE;
    for (Int_t i = 0; i < fInputBuffer->GetEntriesFast(); i++) {
      CbmMvdCluster* cluster = (CbmMvdCluster*) fInputBuffer->At(i);
      TVector3 pos(0, 0, 0);
      TVector3 dpos(0, 0, 0);
      CreateHit(cluster, pos, dpos);
    }
  }
  fInputBuffer->Delete();
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
void CbmMvdSensorHitfinderTask::CreateHit(CbmMvdCluster* cluster, TVector3& pos, TVector3& dpos)
{
  // Calculate the center of gravity of the charge of a cluster

  ComputeCenterOfGravity(cluster, pos, dpos);


  Int_t indexX, indexY;
  //Double_t x,y;
  Double_t local[2];
  local[0] = pos.X();
  local[1] = pos.Y();
  //cout << endl << "found center of gravity at: " << local[0] << " , " << local[1] << endl;

  fSensor->TopToPixel(local, indexX, indexY);

  //cout << endl << "Center is on pixel: " << indexX << " , " << indexY << endl;


  // Save hit into array

  //cout << endl << "adding new hit to fHits at X: " << pos.X() << " , Y: "<< pos.Y() << " , Z: " << pos.Z() << " , from cluster nr " << cluster->GetRefId() ;
  Int_t nHits = fOutputBuffer->GetEntriesFast();
  new ((*fOutputBuffer)[nHits]) CbmMvdHit(fSensor->GetStationNr(), pos, dpos, indexX, indexY, cluster->GetRefId(), 0);
  CbmMvdHit* currentHit = (CbmMvdHit*) fOutputBuffer->At(nHits);
  currentHit->SetTime(fSensor->GetCurrentEventTime());
  currentHit->SetTimeError(fSensor->GetIntegrationtime() / 2);
  currentHit->SetRefId(cluster->GetRefId());
}

//--------------------------------------------------------------------------

/*void CbmMvdSensorHitfinderTask::UpdateDebugHistos(vector<Int_t>* clusterArray, Int_t seedIndexX, Int_t seedIndexY)
{ 
;  
} */


//--------------------------------------------------------------------------

void CbmMvdSensorHitfinderTask::ComputeCenterOfGravity(CbmMvdCluster* cluster, TVector3& pos, TVector3& dpos)
{

  Double_t numeratorX  = 0;
  Double_t numeratorY  = 0;
  Double_t denominator = 0;
  Int_t charge;
  Int_t xIndex;
  Int_t yIndex;
  Double_t x, y;
  Double_t layerPosZ                           = fSensor->GetZ();
  Double_t lab[3]                              = {0, 0, 0};
  std::map<pair<Int_t, Int_t>, Int_t> PixelMap = cluster->GetPixelMap();
  Int_t clusterSize                            = cluster->GetNofDigis();

  UInt_t shape  = 0;
  Int_t xIndex0 = 0;
  Int_t yIndex0 = 0;
  Double_t sigmaIn[3], sigmaOut[3], shiftIn[3], shiftOut[3];

  for (map<pair<Int_t, Int_t>, Int_t>::iterator it = PixelMap.begin(); it != PixelMap.end(); ++it) {
    pair<Int_t, Int_t> pixel = it->first;

    charge = GetAdcCharge(it->second);
    xIndex = pixel.first;
    yIndex = pixel.second;

    // Determine Cluster Shape
    if (PixelMap.size() <= 4) {
      if (it == PixelMap.begin()) {
        xIndex0 = xIndex;
        yIndex0 = yIndex;
      }
      shape += TMath::Power(2, (4 * (yIndex - yIndex0 + 3)) + (xIndex - xIndex0));
    }

    if (gDebug > 0) {
      cout << "-I- "
           << "CbmMvdSensorHitfinderTask:: iCluster= " << cluster->GetRefId() << " , clusterSize= " << clusterSize
           << endl;
      cout << "-I- "
           << "CbmMvdSensorHitfinderTask::xIndex " << xIndex << " , yIndex " << yIndex << " , charge = " << charge
           << endl;
    }

    fSensor->PixelToTop(xIndex, yIndex, lab);

    x = lab[0];
    y = lab[1];

    Double_t xc = x * charge;
    Double_t yc = y * charge;

    numeratorX += xc;
    numeratorY += yc;
    denominator += charge;
  }

  if (gDebug > 0) {
    cout << "-I- "
         << "CbmMvdSensorHitfinderTask::=========================\n " << endl;
    cout << "-I- "
         << "CbmMvdSensorHitfinderTask::numeratorX: " << numeratorX << " , numeratorY: " << numeratorY
         << ", denominator: " << denominator << endl;
  }

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

  if (gDebug > 0) {
    cout << "-I- "
         << "CbmMvdSensorHitfinderTask::-----------------------------------" << endl;
    cout << "-I- "
         << "CbmMvdSensorHitfinderTask::X hit= " << fHitPosX << " Y hit= " << fHitPosY << " Z hit= " << fHitPosZ
         << endl;
    cout << "-I- "
         << "CbmMvdSensorHitfinderTask::-----------------------------------\n"
         << endl;
  }

  // Treat Sigma/Shift of the Cluster according to the Shape
  if (shape == 12288) {
    sigmaIn[0] = 0.00053;
    sigmaIn[1] = 0.00063;
    sigmaIn[2] = 0.;
    shiftIn[0] = -0.00000;
    shiftIn[1] = -0.00001;
    shiftIn[2] = 0.;
  }
  else if (shape == 208896) {
    sigmaIn[0] = 0.00035;
    sigmaIn[1] = 0.00036;
    sigmaIn[2] = 0.;
    shiftIn[0] = -0.00000;
    shiftIn[1] = -0.00002;
    shiftIn[2] = 0.;
  }
  else if (shape == 69632) {
    sigmaIn[0] = 0.00028;
    sigmaIn[1] = 0.00028;
    sigmaIn[2] = 0.;
    shiftIn[0] = -0.00000;
    shiftIn[1] = -0.00002;
    shiftIn[2] = 0.;
  }
  else if (shape == 28672) {
    sigmaIn[0] = 0.00028;
    sigmaIn[1] = 0.00039;
    sigmaIn[2] = 0.;
    shiftIn[0] = -0.00000;
    shiftIn[1] = -0.00001;
    shiftIn[2] = 0.;
  }
  else if (shape == 143360) {
    sigmaIn[0] = 0.00024;
    sigmaIn[1] = 0.00022;
    sigmaIn[2] = 0.;
    shiftIn[0] = +0.00020;
    shiftIn[1] = +0.00008;
    shiftIn[2] = 0.;
  }
  else if (shape == 200704) {
    sigmaIn[0] = 0.00024;
    sigmaIn[1] = 0.00022;
    sigmaIn[2] = 0.;
    shiftIn[0] = -0.00020;
    shiftIn[1] = -0.00011;
    shiftIn[2] = 0.;
  }
  else if (shape == 77824) {
    sigmaIn[0] = 0.00024;
    sigmaIn[1] = 0.00022;
    sigmaIn[2] = 0.;
    shiftIn[0] = -0.00020;
    shiftIn[1] = +0.00008;
    shiftIn[2] = 0.;
  }
  else if (shape == 12800) {
    sigmaIn[0] = 0.00024;
    sigmaIn[1] = 0.00022;
    sigmaIn[2] = 0.;
    shiftIn[0] = +0.00020;
    shiftIn[1] = -0.00011;
    shiftIn[2] = 0.;
  }
  else if (shape == 4096) {
    sigmaIn[0] = 0.00027;
    sigmaIn[1] = 0.00092;
    sigmaIn[2] = 0.;
    shiftIn[0] = +0.00002;
    shiftIn[1] = +0.00004;
    shiftIn[2] = 0.;
  }
  else {
    sigmaIn[0] = 0.00036;
    sigmaIn[1] = 0.00044;
    sigmaIn[2] = 0.;
    shiftIn[0] = -0.00000;
    shiftIn[1] = -0.00002;
    shiftIn[2] = 0.;
  }
  // Consider Sensor Orientation
  TGeoHMatrix* RecoMatrix = fSensor->GetRecoMatrix();
  TGeoHMatrix RotMatrix;
  RotMatrix.SetRotation(RecoMatrix->GetRotationMatrix());

  RotMatrix.LocalToMaster(sigmaIn, sigmaOut);
  RotMatrix.LocalToMaster(shiftIn, shiftOut);

  fHitPosErrX = TMath::Abs(sigmaOut[0]);
  fHitPosErrY = TMath::Abs(sigmaOut[1]);
  fHitPosErrZ = TMath::Abs(sigmaOut[2]);

  fHitPosX += shiftOut[0];
  fHitPosY += shiftOut[1];
  fHitPosZ += shiftOut[2];

  // pos = center of gravity (labframe), dpos uncertaintycout<<setw(10)<<setprecision(2)<< VolumeShape->GetDX();
  pos.SetXYZ(fHitPosX, fHitPosY, fHitPosZ);
  dpos.SetXYZ(fHitPosErrX, fHitPosErrY, fHitPosErrZ);
}

//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
void CbmMvdSensorHitfinderTask::Finish() {}
//--------------------------------------------------------------------------

Int_t CbmMvdSensorHitfinderTask::GetAdcCharge(Float_t charge)
{

  Int_t adcCharge;

  if (charge < fAdcOffset) { return 0; };

  adcCharge = int((charge - fAdcOffset) / fAdcStepSize);
  if (adcCharge > fAdcSteps - 1) { adcCharge = fAdcSteps - 1; }

  return adcCharge;
}


// -----   Private method Reset   ------------------------------------------
void CbmMvdSensorHitfinderTask::Reset() { fClusters->Clear("C"); }

// -------------------------------------------------------------------------


ClassImp(CbmMvdSensorHitfinderTask);
