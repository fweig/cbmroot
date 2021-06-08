/* Copyright (C) 2019-2020 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig */

// -------------------------------------------------------------------------
// -----                  CbmMvdSensorDigiToHitTask source file        -----
// -----                  27.11.18 Edited by K. Hunold                 -----
// -------------------------------------------------------------------------


#include "CbmMvdSensorDigiToHitTask.h"

#include <Logger.h>

#include "TArrayD.h"
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TGeoTube.h"
#include "TObjArray.h"
#include "TRefArray.h"
#include <TMatrixD.h>

#include <cstring>


using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::vector;

//Initialize Variables
Int_t dth_fAdcSteps(-1);
Int_t dth_fAddress(0);
Float_t dth_fAdcStepSize(-1.);
std::vector<TH1F*> dth_fPixelChargeHistos;
std::vector<TH1F*> dth_fTotalChargeInNpixelsArray;


const constexpr Int_t fCounter(0);
// const constexpr Float_t fSigmaNoise(15.); //unused
const float dth_fSeedThreshold  = 1.;
const float dth_fNeighThreshold = 1.;

// const constexpr Bool_t inputSet(kFALSE);  //unused
std::map<std::pair<Int_t, Int_t>, Int_t> dth_ftempPixelMap;

// const constexpr Bool_t fAddNoise(kFALSE);  //unused


//2 bigger becuase we are also checking -1 and (range + 1) usually 576, 1152
const constexpr int pixelRows = 578;
const constexpr int pixelCols = 1154;

Float_t dth_grid[pixelCols][pixelRows];
std::vector<std::pair<short, short>> dth_clusterArray;
std::vector<std::pair<short, short>> dth_coordArray;
std::vector<int> dth_refIDArray;

//Hitfinder Variables:
// const constexpr Int_t fGausArrayLimit = 5000;  //unused


TVector3 pos;
TVector3 dpos;
Double_t lab[3];
Double_t numeratorX;
Double_t numeratorY;
Double_t denominator;
Int_t xIndex;
Int_t yIndex;
Double_t x, y;
Int_t xIndex0;
Int_t yIndex0;
int ID       = 0;
int counter  = 0;
UInt_t shape = 0;


// -----   Default constructor   -------------------------------------------
CbmMvdSensorDigiToHitTask::CbmMvdSensorDigiToHitTask() : CbmMvdSensorDigiToHitTask(0, 0) {}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMvdSensorDigiToHitTask::~CbmMvdSensorDigiToHitTask()
{
  if (fOutputBuffer) {
    fOutputBuffer->Delete();
    delete fOutputBuffer;
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMvdSensorDigiToHitTask::CbmMvdSensorDigiToHitTask(Int_t /*iMode*/, Int_t iVerbose)
  : CbmMvdSensorTask()
  , fAdcDynamic(200)
  , fAdcOffset(0)
  , fAdcBits(1)
  , fDigiMap()
  , fDigiMapIt()
  , fVerbose(iVerbose)
  ,

  //HitFinder Variables
  fSigmaNoise(15.)
  , fHitPosX(0.)
  , fHitPosY(0.)
  , fHitPosZ(0.)
  , fHitPosErrX(0.0005)
  , fHitPosErrY(0.0005)
  , fHitPosErrZ(0.0)

{
}
// -------------------------------------------------------------------------

// -----    Virtual private method Init   ----------------------------------
void CbmMvdSensorDigiToHitTask::InitTask(CbmMvdSensor* mysensor)
{


  fInputBuffer  = new TClonesArray("CbmMvdDigi", 100);
  fOutputBuffer = new TClonesArray("CbmMvdHit", 100);

  dth_fAdcSteps    = (Int_t) TMath::Power(2, fAdcBits);
  dth_fAdcStepSize = fAdcDynamic / dth_fAdcSteps;
  fSensor          = mysensor;
  dth_fAddress     = 1000 * fSensor->GetStationNr() + fSensor->GetSensorNr();

  std::memset(dth_grid, 0, sizeof(dth_grid));

  initialized = kTRUE;
}
// -------------------------------------------------------------------------

// -----   Virtual public method Reinit   ----------------------------------
Bool_t CbmMvdSensorDigiToHitTask::ReInit()
{
  cout << "-I- "
       << "CbmMvdSensorDigiToHitTask::ReInt---------------" << endl;
  return kTRUE;
}
// -------------------------------------------------------------------------

// -----   Virtual public method ExecChain   --------------
void CbmMvdSensorDigiToHitTask::ExecChain() { Exec(); }
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------
void CbmMvdSensorDigiToHitTask::Exec()
{
  int nDigis = fInputBuffer->GetEntriesFast();
  if (nDigis > 0) {

    fOutputBuffer->Delete();
    inputSet    = kFALSE;
    short iDigi = 0;

    CbmMvdDigi* digi = (CbmMvdDigi*) fInputBuffer->At(iDigi);

    if (!digi) {
      cout << "-E- : CbmMvdSensorFindHitTask - Fatal: No Digits found in this "
              "event."
           << endl;
    }


    dth_clusterArray.reserve(nDigis);
    dth_coordArray.reserve(nDigis);
    dth_refIDArray.reserve(nDigis);

    for (Int_t k = 0; k < nDigis; k++) {
      digi = (CbmMvdDigi*) fInputBuffer->At(k);

      if (digi->GetRefId() < 0) { LOG(fatal) << "RefID of this digi is -1 this should not happend "; }

      //apply fNeighThreshold
      Float_t curr_digi_charge = digi->GetCharge();

      short dth_current_digi_X = digi->GetPixelX();
      short dth_current_digi_Y = digi->GetPixelY();
      dth_coordArray.emplace_back(std::make_pair(dth_current_digi_X, dth_current_digi_Y));

      if (GetAdcCharge(curr_digi_charge) >= dth_fNeighThreshold) {
        //puts index into dth_grid.
        dth_grid[dth_current_digi_X + 1][dth_current_digi_Y + 1] = curr_digi_charge;
      }
    }


    /*___________________________________________________________________________________________________
        ___________________________________________________________________________________________________*/
    for (auto& curr_coord : dth_coordArray) {

      auto& dth_current_digi_X = curr_coord.first;
      auto& dth_current_digi_Y = curr_coord.second;


      auto& root_digi_pos_charge = dth_grid[dth_current_digi_X + 1][dth_current_digi_Y + 1];

      if (GetAdcCharge(root_digi_pos_charge) >= dth_fSeedThreshold) {

        pos         = {0, 0, 0};
        dpos        = {0, 0, 0};
        numeratorX  = 0;
        numeratorY  = 0;
        denominator = 0;
        counter     = 0;
        shape       = 0;

        //"-1" has nothing todo with the incremented Grid-Size
        xIndex0 = dth_current_digi_X - 1;
        yIndex0 = dth_current_digi_Y - 1;

        //setting Values for HitfinderCalculation to default.
        /*___________________________________________________________________________________________________
            ___________________________________________________________________________________________________*/

        dth_clusterArray.clear();
        dth_clusterArray.emplace_back(curr_coord);

        //Calculating Median für the first Element
        lab[0] = 0;
        lab[1] = 0;
        lab[2] = 0;

        shape += TMath::Power(2, (4 * (dth_current_digi_Y - 1 - yIndex0 + 3)) + (dth_current_digi_X - 1 - xIndex0));
        //shape &= 1 << ((4*(dth_current_digi_Y - 1 - yIndex0+3))+(dth_current_digi_X - 1 - xIndex0)) ;
        fSensor->PixelToTop((dth_current_digi_X - 1), (dth_current_digi_Y - 1), lab);

        numeratorX += lab[0] * root_digi_pos_charge;
        numeratorY += lab[1] * root_digi_pos_charge;
        denominator += root_digi_pos_charge;


        root_digi_pos_charge = 0;
        /*___________________________________________________________________________________________________
                ___________________________________________________________________________________________________*/
        for (unsigned short i = 0; i < dth_clusterArray.size(); i++) {

          auto& index = dth_clusterArray[i];


          auto checkNeighbour = [&](short channelX, short channelY) {
            auto& curr_digi_pos_charge = dth_grid[channelX + 1][channelY + 1];

            if (curr_digi_pos_charge != 0) {

              //Calculatin Median
              lab[0] = 0;
              lab[1] = 0;
              lab[2] = 0;

              if (counter <= 3) {
                shape += TMath::Power(2, (4 * (channelY - 1 - yIndex0 + 3)) + (channelX - 1 - xIndex0));
                //shape &= 1 << ((4*(channelY - 1 - yIndex0+3))+(channelX - 1 - xIndex0)) ;
              }

              fSensor->PixelToTop((channelX - 1), (channelY - 1), lab);

              numeratorX += lab[0] * curr_digi_pos_charge;
              numeratorY += lab[1] * curr_digi_pos_charge;
              denominator += curr_digi_pos_charge;
              counter++;

              //Saving current Digi in ClusterArray
              dth_clusterArray.emplace_back(std::make_pair(channelX, channelY));

              //Marking Digi in Grid as used/not relevant anymore
              curr_digi_pos_charge = 0;
            }
          };

          short channelX = index.first;
          short channelY = index.second;

          checkNeighbour(channelX + 1, channelY);
          checkNeighbour(channelX - 1, channelY);
          checkNeighbour(channelX, channelY + 1);
          checkNeighbour(channelX, channelY - 1);
        }


        //Compute Center of Gravity
        //__________________________________________________________________________________________

        Double_t layerPosZ = fSensor->GetZ();
        Double_t sigmaIn[3], sigmaOut[3], shiftIn[3], shiftOut[3];

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


        if (dth_clusterArray.size() > 4) { shape = 0; }


        //_________________________________________________________________________________________
        switch (shape) {
          case 12288: {
            sigmaIn[0] = 0.00053;
            sigmaIn[1] = 0.00063;
            sigmaIn[2] = 0.;
            shiftIn[0] = -0.00000;
            shiftIn[1] = -0.00001;
            shiftIn[2] = 0.;
            break;
          }
          case 208896: {
            sigmaIn[0] = 0.00035;
            sigmaIn[1] = 0.00036;
            sigmaIn[2] = 0.;
            shiftIn[0] = -0.00000;
            shiftIn[1] = -0.00002;
            shiftIn[2] = 0.;
            break;
          }
          case 69632: {
            sigmaIn[0] = 0.00028;
            sigmaIn[1] = 0.00028;
            sigmaIn[2] = 0.;
            shiftIn[0] = -0.00000;
            shiftIn[1] = -0.00002;
            shiftIn[2] = 0.;
            break;
          }
          case 28672: {
            sigmaIn[0] = 0.00028;
            sigmaIn[1] = 0.00039;
            sigmaIn[2] = 0.;
            shiftIn[0] = -0.00000;
            shiftIn[1] = -0.00001;
            shiftIn[2] = 0.;
            break;
          }
          case 143360: {
            sigmaIn[0] = 0.00024;
            sigmaIn[1] = 0.00022;
            sigmaIn[2] = 0.;
            shiftIn[0] = +0.00020;
            shiftIn[1] = +0.00008;
            shiftIn[2] = 0.;
            break;
          }
          case 200704: {
            sigmaIn[0] = 0.00024;
            sigmaIn[1] = 0.00022;
            sigmaIn[2] = 0.;
            shiftIn[0] = -0.00020;
            shiftIn[1] = -0.00011;
            shiftIn[2] = 0.;
            break;
          }
          case 77824: {
            sigmaIn[0] = 0.00024;
            sigmaIn[1] = 0.00022;
            sigmaIn[2] = 0.;
            shiftIn[0] = -0.00020;
            shiftIn[1] = +0.00008;
            shiftIn[2] = 0.;
            break;
          }
          case 12800: {
            sigmaIn[0] = 0.00024;
            sigmaIn[1] = 0.00022;
            sigmaIn[2] = 0.;
            shiftIn[0] = +0.00020;
            shiftIn[1] = -0.00011;
            shiftIn[2] = 0.;
            break;
          }
          case 4096: {
            sigmaIn[0] = 0.00027;
            sigmaIn[1] = 0.00092;
            sigmaIn[2] = 0.;
            shiftIn[0] = +0.00002;
            shiftIn[1] = +0.00004;
            shiftIn[2] = 0.;
            break;
          }
          default: {
            sigmaIn[0] = 0.00036;
            sigmaIn[1] = 0.00044;
            sigmaIn[2] = 0.;
            shiftIn[0] = -0.00000;
            shiftIn[1] = -0.00002;
            shiftIn[2] = 0.;
          }
        }
        //_________________________________________________________________________________________

        // Consider Sensor Orientation

        TGeoHMatrix* RecoMatrix = fSensor->GetRecoMatrix();
        TGeoHMatrix RotMatrix;
        RotMatrix.SetRotation(RecoMatrix->GetRotationMatrix());

        RotMatrix.LocalToMaster(sigmaIn, sigmaOut);
        RotMatrix.LocalToMaster(shiftIn, shiftOut);

        fHitPosX += shiftOut[0];
        fHitPosY += shiftOut[1];
        fHitPosZ += shiftOut[2];

        // pos = center of gravity (labframe), dpos uncertaintycout<<setw(10)<<setprecision(2)<< VolumeShape->GetDX();
        pos.SetXYZ(fHitPosX, fHitPosY, fHitPosZ);
        dpos.SetXYZ(TMath::Abs(sigmaOut[0]), TMath::Abs(sigmaOut[1]), TMath::Abs(sigmaOut[2]));

        //Create Hit
        //__________________________________________________________________________________________

        Int_t indexX, indexY;

        Double_t local[2];
        local[0] = pos.X();
        local[1] = pos.Y();

        fSensor->TopToPixel(local, indexX, indexY);
        Int_t nHits = fOutputBuffer->GetEntriesFast();

        new ((*fOutputBuffer)[nHits]) CbmMvdHit(fSensor->GetStationNr(), pos, dpos, indexX, indexY, ID, 0);
        CbmMvdHit* currentHit = (CbmMvdHit*) fOutputBuffer->At(nHits);
        currentHit->SetTime(fSensor->GetCurrentEventTime());
        currentHit->SetTimeError(fSensor->GetIntegrationtime() / 2);
        currentHit->SetRefId(ID);
        ID++;
      }
    }

    //        Int_t nHits = fOutputBuffer->GetEntriesFast();

    dth_clusterArray.clear();
    fInputBuffer->Delete();
    dth_coordArray.clear();
  }
}


//--------------------------------------------------------------------------


// -------------------------------------------------------------------------
float CbmMvdSensorDigiToHitTask::GetAdcCharge(Float_t curr_charge)
{
  int adcCharge;

  if (curr_charge < fAdcOffset) { return 0; };

  adcCharge = int((curr_charge - fAdcOffset) / dth_fAdcStepSize);

  if (adcCharge > dth_fAdcSteps - 1) { return dth_fAdcSteps - 1; }
  else {
    return adcCharge;
  }
}


//--------------------------------------------------------------------------
void CbmMvdSensorDigiToHitTask::Finish()
{
  if (fShowDebugHistos) {
    cout << "\n============================================================" << endl;
    cout << "-I- " << GetName() << "::Finish: Total events skipped: " << fCounter << endl;
    cout << "============================================================" << endl;
    cout << "-I- Parameters used" << endl;
    cout << "Gaussian noise [electrons]	: " << fSigmaNoise << endl;
    cout << "Noise simulated [Bool]	        : " << fAddNoise << endl;
    cout << "Threshold seed [ADC]            : " << dth_fSeedThreshold << endl;
    cout << "Threshold neighbours [ADC]	: " << dth_fNeighThreshold << endl;
    cout << "ADC - Bits			: " << fAdcBits << endl;
    cout << "ADC - Dynamic [electrons]	: " << fAdcDynamic << endl;
    cout << "ADC - Offset [electrons]	: " << fAdcOffset << endl;
    cout << "============================================================" << endl;


    TH1F* histo;
    TH2F* clusterShapeHistogram;
    TCanvas* canvas2 = new TCanvas("HitFinderCharge", "HitFinderCharge");
    canvas2->Divide(2, 2);
    canvas2->cd(1);
    if (fChargeArraySize <= 7) {
      clusterShapeHistogram = new TH2F("MvdClusterShape", "MvdClusterShape", fChargeArraySize, 0, fChargeArraySize,
                                       fChargeArraySize, 0, fChargeArraySize);
      for (Int_t i = 0; i < fChargeArraySize * fChargeArraySize; i++) {
        histo               = dth_fPixelChargeHistos[i];
        Float_t curr_charge = histo->GetMean();
        //cout <<i << " Charge " << charge << " xCluster: " << i%fChargeArraySize << " yCluster: " << i/fChargeArraySize << endl;
        //histo->Fit("landau");
        //TF1* fitFunction= histo->GetFunction("landau");
        //Double_t MPV=fitFunction->GetParameter(1);
        clusterShapeHistogram->Fill(i % fChargeArraySize, i / fChargeArraySize, curr_charge);
      }
    }
    clusterShapeHistogram->Draw("Lego2");
    canvas2->cd(2);
    histo = dth_fPixelChargeHistos[50];
    histo->Draw();
    canvas2->cd(3);
    histo = dth_fPixelChargeHistos[51];
    histo->Draw();
    canvas2->cd(4);
    //fFullClusterHisto->Draw();
  }
}
//--------------------------------------------------------------------------
ClassImp(CbmMvdSensorDigiToHitTask)
