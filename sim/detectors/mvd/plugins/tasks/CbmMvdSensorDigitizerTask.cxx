/* Copyright (C) 2014-2019 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                  CbmMvdSensorDigitizerTask source file              -----
// -----                  Created 02.02.2012 by M. Deveaux            -----
// -------------------------------------------------------------------------
/**
 *
 * ____________________________________________________________________________________________
 * --------------------------------------------------------------------------------------------
 * adaptation for CBM: C.Dritsa
  * Acknowlegments to:
 *	Rita de Masi (IPHC, Strasbourg), M.Deveaux (IKF, Frankfurt), V.Friese (GSI, Darmstadt)
 *   Code tuning and maintainance M.Deveaux 01/07/2010
 *   Redesign as plugin: M. Deveaux 02.02.2012   
 * ____________________________________________________________________________________________
 * --------------------------------------------------------------------------------------------
 **/
#include "CbmMvdSensorDigitizerTask.h"

#include "CbmMatch.h"               // for CbmMatch
#include "CbmMvdDigi.h"             // for CbmMvdDigi
#include "CbmMvdPileupManager.h"    // for CbmMvdPileupManager
#include "CbmMvdPixelCharge.h"      // for CbmMvdPixelCharge
#include "CbmMvdPoint.h"            // for CbmMvdPoint
#include "CbmMvdSensor.h"           // for CbmMvdSensor
#include "CbmMvdSensorDataSheet.h"  // for CbmMvdSensorDataSheet
#include "CbmMvdSensorPlugin.h"     // for CbmMvdSensorPlugin

#include <FairEventHeader.h>  // for FairEventHeader
#include <FairRootManager.h>  // for FairRootManager
#include <FairRunAna.h>       // for FairRunAna
#include <FairRunSim.h>       // for FairRunSim
#include <Logger.h>           // for LOG, Logger

#include <TCanvas.h>       // for TCanvas
#include <TClonesArray.h>  // for TClonesArray
#include <TH1.h>           // for TH1F
#include <TH2.h>           // for TH2F
#include <TMath.h>         // for Pi, ATan
#include <TMathBase.h>     // for Abs, Max
#include <TRandom.h>       // for TRandom
#include <TRefArray.h>     // for TRefArray
#include <TVector3.h>      // for TVector3, operator*, operator+

#include <iomanip>  // for operator<<, setprecision, setw
#include <map>      // for map, operator==, __map_iterator
#include <ostream>  // for operator<<, basic_ostream, endl
#include <vector>   // for allocator, vector

#include <cmath>  // for sqrt


using std::endl;
using std::ios_base;
using std::pair;
using std::setprecision;
using std::setw;


// -----   Default constructor   ------------------------------------------
CbmMvdSensorDigitizerTask::CbmMvdSensorDigitizerTask()
  : CbmMvdSensorTask()
  , fcurrentFrameNumber(0)
  , fEpiTh(0.0014)
  , fSegmentLength(0.0001)
  , fDiffusionCoefficient(0.0055)
  , fElectronsPerKeV(276.)
  , fWidthOfCluster(3.5)
  , fPixelSizeX(0.0030)
  , fPixelSizeY(0.0030)
  , fCutOnDeltaRays(0.00169720)
  , fChargeThreshold(100.)
  , fFanoSilicium(0.115)
  , fEsum(0.)
  , fSegmentDepth(0.)
  , fCurrentTotalCharge(0.)
  , fCurrentParticleMass(0.)
  , fCurrentParticleMomentum(0.)
  , fCurrentParticlePdg(0)
  , fRandomGeneratorTestHisto(nullptr)
  , fPosXY(nullptr)
  , fpZ(nullptr)
  , fPosXinIOut(nullptr)
  , fAngle(nullptr)
  , fSegResolutionHistoX(nullptr)
  , fSegResolutionHistoY(nullptr)
  , fSegResolutionHistoZ(nullptr)
  , fTotalChargeHisto(nullptr)
  , fTotalSegmentChargeHisto(nullptr)
  , fLorentzY0(-6.1)
  , fLorentzXc(0.)
  , fLorentzW(1.03)
  , fLorentzA(477.2)
  , fLorentzNorm(1.)
  , fLandauMPV(877.4)
  , fLandauSigma(204.93)
  , fLandauGain(3.3)
  , fLandauRandom(new TRandom3())
  , fPixelSize(0.0025)
  , fPar0(520.)
  , fPar1(0.34)
  , fPar2(-1.2)
  , fCompression(1.)
  , fResolutionHistoX(nullptr)
  , fResolutionHistoY(nullptr)
  , fNumberOfSegments(0)
  , fCurrentLayer(0)
  , fEvent(0)
  , fVolumeId(0)
  , fNPixelsX(0)
  , fNPixelsY(0)
  , fPixelCharge(new TClonesArray("CbmMvdPixelCharge"))
  , fDigis(nullptr)
  , fDigiMatch(nullptr)
  , fproduceNoise(kFALSE)
  , fPixelChargeShort()
  , fPixelScanAccelerator(nullptr)
  , fChargeMap()
  , fChargeMapIt()
  , fsensorDataSheet(nullptr)
  , fMode(0)
  , fSigmaX(0.0005)
  , fSigmaY(0.0005)
  , fReadoutTime(0.00005)
  , fEfficiency(-1.)
  , fMergeDist(-1.)
  , fFakeRate(-1.)
  , fNPileup(0)
  , fNDeltaElect(0)
  , fDeltaBufferSize(0)
  , fBgBufferSize(0)
  , fBranchName("")
  , fBgFileName("")
  , fDeltaFileName("")
  , fInputPoints(nullptr)
  , fPoints(new TRefArray())
  , fRandGen()
  , fTimer()
  , fPileupManager(nullptr)
  , fDeltaManager(nullptr)
  , fNEvents(0)
  , fNPoints(0.)
  , fNReal(0.)
  , fNBg(0.)
  , fNFake(0.)
  , fNLost(0.)
  , fNMerged(0.)
  , fTime(0.)
  , fSignalPoints()
  , h_trackLength(nullptr)
  , h_numSegments(nullptr)
  , h_LengthVsAngle(nullptr)
  , h_LengthVsEloss(nullptr)
  , h_ElossVsMomIn(nullptr)
{
  fRandGen.SetSeed(2736);
  fproduceNoise   = kFALSE;
  fPluginIDNumber = 100;
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMvdSensorDigitizerTask::CbmMvdSensorDigitizerTask(Int_t iMode)
  : CbmMvdSensorTask()
  , fcurrentFrameNumber(0)
  , fEpiTh(0.0014)
  , fSegmentLength(0.0001)
  , fDiffusionCoefficient(0.0055)
  , fElectronsPerKeV(276.)
  , fWidthOfCluster(3.5)
  , fPixelSizeX(0.0030)
  , fPixelSizeY(0.0030)
  , fCutOnDeltaRays(0.00169720)
  , fChargeThreshold(100.)
  , fFanoSilicium(0.115)
  , fEsum(0.)
  , fSegmentDepth(0.)
  , fCurrentTotalCharge(0.)
  , fCurrentParticleMass(0.)
  , fCurrentParticleMomentum(0.)
  , fCurrentParticlePdg(0)
  , fRandomGeneratorTestHisto(nullptr)
  , fPosXY(nullptr)
  , fpZ(nullptr)
  , fPosXinIOut(nullptr)
  , fAngle(nullptr)
  , fSegResolutionHistoX(nullptr)
  , fSegResolutionHistoY(nullptr)
  , fSegResolutionHistoZ(nullptr)
  , fTotalChargeHisto(nullptr)
  , fTotalSegmentChargeHisto(nullptr)
  , fLorentzY0(-6.1)
  , fLorentzXc(0.)
  , fLorentzW(1.03)
  , fLorentzA(477.2)
  , fLorentzNorm(1.)
  , fLandauMPV(877.4)
  , fLandauSigma(204.93)
  , fLandauGain(3.3)
  , fLandauRandom(new TRandom3())
  , fPixelSize(0.0025)
  , fPar0(520.)
  , fPar1(0.34)
  , fPar2(-1.2)
  , fCompression(1.)
  , fResolutionHistoX(nullptr)
  , fResolutionHistoY(nullptr)
  , fNumberOfSegments(0)
  , fCurrentLayer(0)
  , fEvent(0)
  , fVolumeId(0)
  , fNPixelsX(0)
  , fNPixelsY(0)
  , fPixelCharge(new TClonesArray("CbmMvdPixelCharge", 100000))
  , fDigis(nullptr)
  , fDigiMatch(nullptr)
  , fproduceNoise(kFALSE)
  , fPixelChargeShort()
  , fPixelScanAccelerator(nullptr)
  , fChargeMap()
  , fChargeMapIt()
  , fsensorDataSheet(nullptr)
  , fMode(iMode)
  , fSigmaX(0.0005)
  , fSigmaY(0.0005)
  , fReadoutTime(0.00005)
  , fEfficiency(-1.)
  , fMergeDist(-1.)
  , fFakeRate(-1.)
  , fNPileup(0)
  , fNDeltaElect(0)
  , fDeltaBufferSize(0)
  , fBgBufferSize(0)
  , fBranchName("MvdPoint")
  , fBgFileName("")
  , fDeltaFileName("")
  , fInputPoints(nullptr)
  , fPoints(new TRefArray())
  , fRandGen()
  , fTimer()
  , fPileupManager(nullptr)
  , fDeltaManager(nullptr)
  , fNEvents(0)
  , fNPoints(0.)
  , fNReal(0.)
  , fNBg(0.)
  , fNFake(0.)
  , fNLost(0.)
  , fNMerged(0.)
  , fTime(0.)
  , fSignalPoints()
  , h_trackLength(nullptr)
  , h_numSegments(nullptr)
  , h_LengthVsAngle(nullptr)
  , h_LengthVsEloss(nullptr)
  , h_ElossVsMomIn(nullptr)

{
  fPluginIDNumber = 100;
  LOG(debug) << "Starting CbmMvdSensorDigitizerTask::CbmMvdSensorDigitizerTask() ";

  fRandGen.SetSeed(2736);
  fEvent       = 0;
  fTime        = 0.;
  fSigmaX      = 0.0005;
  fSigmaY      = 0.0005;
  fReadoutTime = 0.00005;


  fEpiTh                   = 0.0014;
  fSegmentLength           = 0.0001;
  fDiffusionCoefficient    = 0.0055;  // correspondes to the sigma of the gauss with the max drift length
  fElectronsPerKeV         = 276;     //3.62 eV for e-h creation
  fWidthOfCluster          = 3.5;     // in sigmas
  fPixelSizeX              = 0.0025;  // in cm
  fPixelSizeY              = 0.0025;
  fCutOnDeltaRays          = 0.00169720;  //MeV
  fChargeThreshold         = 100.;        //electrons change 1 to 10
  fFanoSilicium            = 0.115;
  fEsum                    = 0;
  fSegmentDepth            = 0;
  fCurrentTotalCharge      = 0;
  fCurrentParticleMass     = 0;
  fCurrentParticleMomentum = 0;
  fPixelScanAccelerator    = 0;


  fPixelSize   = 0.0025;
  fPar0        = 520.;
  fPar1        = 0.34;
  fPar2        = -1.2;
  fLandauMPV   = 877.4;
  fLandauSigma = 204.93;
  fLandauGain  = 3.3;

  fLorentzY0 = -6.1;
  fLorentzXc = 0.;
  fLorentzW  = 1.03;
  fLorentzA  = 477.2;


  fCompression = 1.;


  //fLorentzNorm=0.00013010281679422413;
  fLorentzNorm = 1;


  fcurrentFrameNumber = 0;

  fproduceNoise = kFALSE;
}


// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMvdSensorDigitizerTask::~CbmMvdSensorDigitizerTask()
{

  if (fPileupManager) delete fPileupManager;
  if (fDeltaManager) delete fDeltaManager;
  if (fInputPoints) {
    fInputPoints->Delete();
    delete fInputPoints;
  }
  if (fOutputBuffer) {
    fOutputBuffer->Delete();
    delete fOutputBuffer;
  }
}
// ------------------------------------------------------------------------


// -----    Virtual private method ReadSensorInformation   ----------------
InitStatus CbmMvdSensorDigitizerTask::ReadSensorInformation()
{

  CbmMvdSensorDataSheet* sensorData;
  sensorData = fSensor->GetDataSheet();
  if (!sensorData) { return kERROR; }

  fPixelSizeX = sensorData->GetPixelPitchX();
  fPixelSizeY = sensorData->GetPixelPitchY();
  fNPixelsX   = sensorData->GetNPixelsX();
  fNPixelsY   = sensorData->GetNPixelsY();

  fChargeThreshold = sensorData->GetChargeThreshold();

  fPar0 = sensorData->GetLorentzPar0();
  fPar1 = sensorData->GetLorentzPar1();
  fPar2 = sensorData->GetLorentzPar2();  //LOG(debug) << " LorentzPar2 is now set to " << fPar2;

  fLandauMPV   = sensorData->GetLandauMPV();     //LOG(debug)<< " Landau MPV is now set to " << fLandauMPV;
  fLandauSigma = sensorData->GetLandauSigma();   //LOG(debug) << " Landau Sigma is now set to " << fLandauSigma;
  fLandauGain  = sensorData->GetLandauGain();    //LOG(debug) << " Landau Gain is now set to " << fLandauGain;
  fEpiTh       = sensorData->GetEpiThickness();  //LOG(debug) << " Epitaxial thickness is now set to " << fEpiTh;
  fCompression = fPixelSizeX / fPixelSizeY;

  if (fCompression != 1)
    //LOG(debug) << "working with non uniform pixels";
    if (fCompression <= 0) fCompression = 1;
  return kSUCCESS;
}


// -----------------------------------------------------------------------------

void CbmMvdSensorDigitizerTask::SetInputArray(TClonesArray* inputStream)
{

  Int_t i       = 0;
  Int_t nInputs = inputStream->GetEntriesFast();
  while (nInputs > i) {
    new ((*fInputPoints)[fInputPoints->GetEntriesFast()]) CbmMvdPoint(*((CbmMvdPoint*) inputStream->At(i)));

    //LOG(debug) << "New Input registered";
    i++;
  }
}
//-----------------------------------------------------------------------------

void CbmMvdSensorDigitizerTask::SetInput(TObject* point)
{

  new ((*fInputPoints)[fInputPoints->GetEntriesFast()]) CbmMvdPoint(*((CbmMvdPoint*) point));
}


// -------------- public method ExecChain   ------------------------------------
void CbmMvdSensorDigitizerTask::ExecChain()
{

  //   LOG(debug) << "start Digitizer on sensor " << fSensor->GetName();


  Exec();
}

// -----   Virtual public method Exec   ------------------------------------
void CbmMvdSensorDigitizerTask::Exec()
{

  if (fPreviousPlugin) {
    fInputPoints->Delete();
    fInputPoints->AbsorbObjects(fPreviousPlugin->GetOutputArray());
  }
  fOutputBuffer->Clear("C");
  fDigis->Clear("C");
  fDigiMatch->Clear("C");
  if (fInputPoints->GetEntriesFast() > 0) {

    for (Int_t iPoint = 0; iPoint < fInputPoints->GetEntriesFast(); iPoint++) {

      CbmMvdPoint* point = (CbmMvdPoint*) fInputPoints->At(iPoint);

      if (!point) {
        LOG(warning) << "    -received bad MC-Point. Ignored.";
        continue;
      }
      if (point->GetStationNr() != fSensor->GetSensorNr()) {
        LOG(warning) << "    -received bad MC-Point which doesn't belong here. Ignored.";
        continue;
      }
      fcurrentFrameNumber = point->GetFrame();
      //The digitizer acts only on particles, which crossed the station.
      //Particles generated in the sensor or being absorbed in this sensor are ignored
      if (TMath::Abs(point->GetZOut() - point->GetZ()) < 0.9 * fEpiTh) {
        LOG(debug) << "hit not on chip with thickness " << 0.9 * 2 * fSensor->GetDZ();
        LOG(debug) << "hit not on chip with length " << TMath::Abs(point->GetZOut() - point->GetZ());
        continue;
      }
      // Reject for the time being light nuclei (no digitization modell yet)
      if (point->GetPdgCode() > 100000) { continue; }
      // Digitize the point
      //LOG(debug) << " found point make digi";
      ProduceIonisationPoints(point);
      ProducePixelCharge(point);
    }  //loop on MCpoints

    Int_t nDigis = 0;
    GetEventInfo(fInputNr, fEventNr, fEventTime);

    if (fproduceNoise) ProduceNoise();

    for (Int_t i = 0; i < fPixelCharge->GetEntriesFast(); i++) {
      CbmMvdPixelCharge* pixel = (CbmMvdPixelCharge*) fPixelCharge->At(i);

      if (pixel->GetCharge() > fChargeThreshold) {
        nDigis = fDigis->GetEntriesFast();

        new ((*fDigis)[nDigis])
          CbmMvdDigi(fSensor->GetSensorNr(), pixel->GetX(), pixel->GetY(), pixel->GetCharge(), fPixelSizeX, fPixelSizeY,
                     fEventTime + pixel->GetPixelTime(), pixel->GetFrame());


        new ((*fOutputBuffer)[nDigis])
          CbmMvdDigi(fSensor->GetSensorNr(), pixel->GetX(), pixel->GetY(), pixel->GetCharge(), fPixelSizeX, fPixelSizeY,
                     fEventTime + pixel->GetPixelTime(), pixel->GetFrame());

        new ((*fDigiMatch)[nDigis]) CbmMatch();
        CbmMatch* match = (CbmMatch*) fDigiMatch->At(nDigis);
        for (Int_t iLink = 0; iLink < pixel->GetNContributors(); iLink++) {
          if (pixel->GetTrackID()[iLink] > -1)
            match->AddLink((Double_t) pixel->GetPointWeight()[iLink], pixel->GetPointID()[iLink], fEventNr, fInputNr);
          else
            match->AddLink((Double_t) pixel->GetPointWeight()[iLink], pixel->GetPointID()[iLink]);
        }
      }
      else {
        //LOG(debug) << "charge under threshold, digi rejected";
      }
    }
    //LOG(debug) << nDigis <<" new Digis at on Sensor " << fSensor->GetSensorNr() << " from " << fInputPoints->GetEntriesFast()<< " McPoints";
  }

  else {
    //LOG(debug)<< "No input found on Sensor " << fSensor->GetSensorNr() << " from " << fInputPoints->GetEntriesFast()<< " McPoints";
  }

  fPixelCharge->Delete();
  fChargeMap.clear();
  fInputPoints->Delete();
  fSignalPoints.clear();

}  // end of exec

// -------------------------------------------------------------------------


Int_t CbmMvdSensorDigitizerTask::GetPixelCharge(CbmMvdPixelCharge* /*myPixel*/, Double_t readoutTime)
{

  /**
 * Get the analog signal created by each signal. Assumption: Signal rise and signal fall follows exponential. Contributions of the individual hits add up.
 **/

  Int_t pixelCharge            = 0;
  Double_t pixelSignalRiseTime = fsensorDataSheet->GetSignalRiseTime();
  Double_t pixelSignalFallTime = fsensorDataSheet->GetSignalFallTime();
  Int_t nHits                  = 0;  //=myPixel->GetNHits();

  for (Int_t hitNr = 0; hitNr < nHits; hitNr++) {
    Int_t hitCharge = 0;  //=pixel->GetHitCharge(hitNr);
    Int_t hitTime   = 0;  //=pixel->GetHitTime(hitNr);

    pixelCharge = pixelCharge
                  + hitCharge
                      * (1
                         - TMath::Exp(-(readoutTime - hitTime)
                                      / pixelSignalRiseTime));  //exponential signal rise of the analog charge
    pixelCharge = pixelCharge
                  - hitCharge
                      * (1
                         - TMath::Exp(-(readoutTime - hitTime)
                                      / pixelSignalFallTime));  //exponential signal fall of the analog charge
  }

  return pixelCharge;
}

// ------------------------------------------------------------------------------


Bool_t CbmMvdSensorDigitizerTask::GetSignalAboveThreshold(CbmMvdPixelCharge* myPixel, Double_t readoutTime)
{
  /**
   * Checks if pixel is above threshold.
   **/

  //  CbmMvdSensor* mySensor; // not used FU 12.05.23


  return (GetPixelCharge(myPixel, readoutTime) > fsensorDataSheet->GetAnalogThreshold());
}

// ------------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMvdSensorDigitizerTask::GetEventInfo(Int_t& inputNr, Int_t& eventNr, Double_t& eventTime)
{

  // --- The event number is taken from the FairRootManager
  eventNr = FairRootManager::Instance()->GetEntryNr();

  // --- In a FairRunAna, take the information from FairEventHeader
  if (FairRunAna::Instance()) {
    FairEventHeader* event = FairRunAna::Instance()->GetEventHeader();
    inputNr                = event->GetInputFileId();
    eventTime              = event->GetEventTime();
  }

  // --- In a FairRunSim, the input number and event time are always zero.
  else {
    if (!FairRunSim::Instance()) LOG(fatal) << GetName() << ": neither SIM nor ANA run.";
    inputNr   = 0;
    eventTime = 0.;
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdSensorDigitizerTask::ProduceIonisationPoints(CbmMvdPoint* point)
{
  /** Produces ionisation points along track segment within 
   ** the active Silicon layer.
   **/

  //Option_t* opt1;
  //LOG(debug) << "Computing Point ";
  //point->Print(opt1);

  // Int_t pdgCode = point->GetPdgCode();

  //Transform coordinates of the point into sensor frame

  Double_t globalPositionIn[3]  = {point->GetX(), point->GetY(), point->GetZ()};
  Double_t globalPositionOut[3] = {point->GetXOut(), point->GetYOut(), point->GetZOut()};

  Double_t localPositionIn[3] = {0, 0, 0};

  Double_t localPositionOut[3] = {0, 0, 0};

  fSensor->TopToLocal(globalPositionIn, localPositionIn);
  fSensor->TopToLocal(globalPositionOut, localPositionOut);

  Int_t pixelX, pixelY;
  fSensor->LocalToPixel(&localPositionIn[0], pixelX, pixelY);

  // Copy results into variables used by earlier versions

  Double_t entryX = localPositionIn[0];
  Double_t exitX  = localPositionOut[0];
  Double_t entryY = localPositionIn[1];
  Double_t exitY  = localPositionOut[1];
  Double_t entryZ = localPositionIn[2];
  Double_t exitZ  = localPositionOut[2];

  /**

    Create vector entryDet a (x1,y1,z1) = entry in detector
    Create vector exitDet  b (x2,y2,z2) = exit from detector

    Substract   b-a and get the vector "c" giving the direction of the particle.

    Scale the vector c (draw the 3D schema and check the similar triangles)

    Add vector a.

    The result is a vector with starting point [(x,y,z)entry in detector]
    and end point [(x,y,z)entry in the epi layer]

    same for defining exit from epi layer.
    **/


  // entry and exit from the epi layer ( det ref frame ) :
  Double_t entryZepi = -fEpiTh / 2;
  Double_t exitZepi  = fEpiTh / 2;


  TVector3 a(entryX, entryY, entryZ);  // entry in the detector
  TVector3 b(exitX, exitY, exitZ);     // exit from the detector
  TVector3 c;

  c = b - a;  // AB in schema

  TVector3 d;
  TVector3 e;

  Double_t scale1 = (entryZepi - entryZ) / (exitZ - entryZ);
  Double_t scale2 = (exitZepi - entryZ) / (exitZ - entryZ);


  d = c * scale1;
  e = c * scale2;

  TVector3 entryEpiCoord;
  TVector3 exitEpiCoord;

  entryEpiCoord = d + a;
  exitEpiCoord  = e + a;


  //Get x and y coordinates at the ENTRY of the epi layer
  Double_t entryXepi = entryEpiCoord.X();
  Double_t entryYepi = entryEpiCoord.Y();
  entryZepi          = entryEpiCoord.Z();

  //Get x and y coordinates at the EXIT of the epi layer
  Double_t exitXepi = exitEpiCoord.X();
  Double_t exitYepi = exitEpiCoord.Y();
  exitZepi          = exitEpiCoord.Z();


  Double_t lx = -(entryXepi - exitXepi);  //length of segment x-direction
  Double_t ly = -(entryYepi - exitYepi);
  Double_t lz = -(entryZepi - exitZepi);


  //-----------------------------------------------------------


  Double_t rawLength   = sqrt(lx * lx + ly * ly + lz * lz);  //length of the track inside the epi-layer, in cm
  Double_t trackLength = 0;

  if (rawLength < 1.0e+3) { trackLength = rawLength; }

  else {
    LOG(warning) << GetName() << " : rawlength > 1.0e+3 : " << rawLength;
    trackLength = 1.0e+3;
  }

  //Smear the energy on each track segment
  Double_t charge = fLandauRandom->Landau(fLandauGain, fLandauSigma / fLandauMPV);

  //if (fShowDebugHistos )  LOG(debug) << "charge " << charge << endl;

  if (charge > (12000 / fLandauMPV)) { charge = 12000 / fLandauMPV; }  //limit Random generator behaviour

  if (fShowDebugHistos) { fRandomGeneratorTestHisto->Fill(charge * fLandauMPV); }
  //Translate the charge to normalized energy

  //     LOG(debug) << "charge after random generator " << charge;
  Double_t dEmean = charge / (fElectronsPerKeV * 1e6);
  //   LOG(debug) << "dEmean " << dEmean;
  fNumberOfSegments = int(trackLength / fSegmentLength) + 1;

  dEmean = dEmean * ((Double_t) trackLength / fEpiTh);  //scale the energy to the track length

  dEmean = dEmean / ((Double_t) fNumberOfSegments);  // From this point dEmean corresponds to the E lost per segment.


  fSignalPoints.resize(fNumberOfSegments);

  fEsum = 0.0;

  //Double_t segmentLength_update = trackLength/((Double_t)fNumberOfSegments);

  if (lz != 0) {
    /**
	 condition added 05/08/08 because if lz=0 then there is no segment
         projection (=fSegmentDepth)
	 **/
    fSegmentDepth = fEpiTh / ((Double_t) fNumberOfSegments);
  }
  else {  //condition added 05/08/08
    fSegmentDepth = 0;
    LOG(warning) << GetName() << " Length of track in detector (z-direction) is 0!!!";
  }


  Double_t x = 0, y = 0, z = 0;

  Double_t xDebug = 0, yDebug = 0, zDebug = 0;
  Float_t totalSegmentCharge = 0;

  for (int i = 0; i < fNumberOfSegments; ++i) {

    z = -fEpiTh / 2 + ((double) (i) + 0.5) * fSegmentDepth;  //middle position of the segment; zdirection
    x = entryXepi
        + ((double) (i) + 0.5) * (lx / ((Double_t) fNumberOfSegments));  //middle position of the segment; xdirection
    y = entryYepi
        + ((double) (i) + 0.5) * (ly / ((Double_t) fNumberOfSegments));  //middle position of the segment; ydirection

    if (fShowDebugHistos) {
      xDebug = xDebug + x;
      yDebug = yDebug + y;
      zDebug = zDebug + z;
    };

    SignalPoint* sPoint = &fSignalPoints[i];

    fEsum         = fEsum + dEmean;
    sPoint->eloss = dEmean;
    sPoint->x     = x;  //here the coordinates x,y,z are given in the sensor reference frame.
    sPoint->y     = y;
    sPoint->z     = z;
    charge        = 1.0e+6 * dEmean * fElectronsPerKeV;
    //LOG(debug) << "charge " << charge;
    sPoint->sigmaX     = fPixelSize;
    sPoint->sigmaY     = fPixelSize;
    sPoint->charge     = charge;
    totalSegmentCharge = totalSegmentCharge + charge;
  }
  //if (fShowDebugHistos  ) LOG(debug) << "totalSegmentCharge " << totalSegmentCharge << endl;
  if (fShowDebugHistos) {
    fTotalSegmentChargeHisto->Fill(totalSegmentCharge * fLandauMPV);
    fSegResolutionHistoX->Fill(xDebug / fNumberOfSegments - (point->GetX() + point->GetXOut()) / 2 - fSensor->GetX());
    fSegResolutionHistoY->Fill(yDebug / fNumberOfSegments - (point->GetY() + point->GetYOut()) / 2 - fSensor->GetY());
    fSegResolutionHistoZ->Fill(zDebug / fNumberOfSegments - (point->GetZ() + point->GetZOut()) / 2 - fSensor->GetZ());
  }
}


// -------------------------------------------------------------------------

void CbmMvdSensorDigitizerTask::ProducePixelCharge(CbmMvdPoint* point)
{
  /** Simulation of fired pixels. Each fired pixel is considered
     * as SimTrackerHit
     */

  fCurrentTotalCharge = 0.0;


  // MDx - Variables needed in order to compute a "Monte Carlo Center of Gravity" of the cluster

  Float_t xCharge = 0., yCharge = 0., totClusterCharge = 0.;
  CbmMvdPixelCharge* pixel;

  pair<Int_t, Int_t> thispoint;

  Double_t xCentre, yCentre, sigmaX, sigmaY, xLo, xUp, yLo, yUp;

  SignalPoint* sPoint;
  sPoint = &fSignalPoints[0];

  xCentre = sPoint->x;  //of segment
  yCentre = sPoint->y;  /// idem
  sigmaX  = sPoint->sigmaX;
  sigmaY  = sPoint->sigmaY;

  xLo = sPoint->x - fWidthOfCluster * sigmaX;
  xUp = sPoint->x + fWidthOfCluster * sigmaX;
  yLo = sPoint->y - fWidthOfCluster * sigmaY;
  yUp = sPoint->y + fWidthOfCluster * sigmaY;

  if (fNumberOfSegments < 2) { LOG(fatal) << "fNumberOfSegments < 2, this makes no sense, check parameters."; }

  Int_t* lowerXArray = new Int_t[fNumberOfSegments];
  Int_t* upperXArray = new Int_t[fNumberOfSegments];
  Int_t* lowerYArray = new Int_t[fNumberOfSegments];
  Int_t* upperYArray = new Int_t[fNumberOfSegments];

  Int_t ixLo, ixUp, iyLo, iyUp;


  Double_t minCoord[] = {xLo, yLo};
  Double_t maxCoord[] = {xUp, yUp};


  fSensor->LocalToPixel(minCoord, lowerXArray[0], lowerYArray[0]);
  fSensor->LocalToPixel(maxCoord, upperXArray[0], upperYArray[0]);


  if (lowerXArray[0] < 0) lowerXArray[0] = 0;
  if (lowerYArray[0] < 0) lowerYArray[0] = 0;
  if (upperXArray[0] > fNPixelsX) upperXArray[0] = fNPixelsX;
  if (upperYArray[0] > fNPixelsY) upperYArray[0] = fNPixelsY;

  ixLo = lowerXArray[0];
  iyLo = lowerYArray[0];
  ixUp = upperXArray[0];
  iyUp = upperYArray[0];


  for (Int_t i = 1; i < fNumberOfSegments; i++) {

    sPoint = &fSignalPoints[i];
    sigmaX = sPoint->sigmaX;
    sigmaY = sPoint->sigmaY;

    minCoord[0] = sPoint->x - fWidthOfCluster * sigmaX;
    minCoord[1] = sPoint->y - fWidthOfCluster * sigmaY;
    maxCoord[0] = sPoint->x + fWidthOfCluster * sigmaX;
    maxCoord[1] = sPoint->y + fWidthOfCluster * sigmaY;


    fSensor->LocalToPixel(minCoord, lowerXArray[i], lowerYArray[i]);
    fSensor->LocalToPixel(maxCoord, upperXArray[i], upperYArray[i]);

    if (lowerXArray[i] < 0) lowerXArray[i] = 0;
    if (lowerYArray[i] < 0) lowerYArray[i] = 0;

    if (upperXArray[i] > fNPixelsX) upperXArray[i] = fNPixelsX;
    if (upperYArray[i] > fNPixelsY) upperYArray[i] = fNPixelsY;

    if (ixLo > lowerXArray[i]) { ixLo = lowerXArray[i]; }
    if (ixUp < upperXArray[i]) { ixUp = upperXArray[i]; }
    if (iyLo > lowerYArray[i]) { iyLo = lowerYArray[i]; }
    if (iyUp < upperYArray[i]) { iyUp = upperYArray[i]; }
  }


  //LOG(debug) << "Scanning from x= " << ixLo << " to " <<ixUp <<" and  y= "<<iyLo<< " to " << iyUp;

  // loop over all pads of interest.
  fPixelChargeShort.clear();
  Int_t ix, iy;


  for (ix = ixLo; ix < ixUp + 1; ix++) {

    for (iy = iyLo; iy < iyUp + 1; iy++) {

      //calculate the position of the current pixel in the lab-system

      Double_t Current[3];
      fSensor->PixelToLocal(ix, iy, Current);
      pixel = nullptr;  //decouple pixel-pointer from previous pixel

      //loop over segments, check if the pad received some charge
      for (Int_t i = 0; i < fNumberOfSegments; ++i) {
        // LOG(debug) << "check segment nr. " << i << " from " << fNumberOfSegments;
        // ignore pads, which are out of reach for this segments
        if (ix < lowerXArray[i]) { continue; }
        if (iy < lowerYArray[i]) { continue; }
        if (ix > upperXArray[i]) { continue; }
        if (iy > upperYArray[i]) { continue; }

        // LOG(debug) << "found vallied pad " << i;
        sPoint = &fSignalPoints[i];

        xCentre = sPoint->x;  //of segment
        yCentre = sPoint->y;  // idem
        sigmaX  = sPoint->sigmaX;
        sigmaY  = sPoint->sigmaY;

        fCurrentTotalCharge += sPoint->charge;

        //compute the charge distributed to this pixel by this segment
        Float_t totCharge = (sPoint->charge * fLorentzNorm * (0.5 * fPar0 * fPar1 / TMath::Pi())
                               / TMath::Max(1.e-10, (((Current[0] - xCentre) * (Current[0] - xCentre))
                                                     + ((Current[1] - yCentre) * (Current[1] - yCentre)))
                                                        / fPixelSize / fPixelSize
                                                      + 0.25 * fPar1 * fPar1)
                             + fPar2);


        if (totCharge < 1) {

          // LOG(debug) << "charge is " << totCharge << " < 1 electron thus charge is negligible";
          continue;
        }  //ignore negligible charge (< 1 electron)
        if (!pixel) {
          // LOG(debug) << "charge is " << totCharge << " > 1 electron thus pixel is firred at "<< ix << " " << iy;
          // Look for pixel in charge map if not yet linked.
          thispoint = std::make_pair(ix, iy);
          // LOG(debug) << "creat pair at "<< ix << " " << iy;
          fChargeMapIt = fChargeMap.find(thispoint);
          // LOG(debug) << "found pair at "<< ix << " " << iy;
          // LOG(debug) << "Map size is now " << fChargeMap.size();
          // Pixel not yet in map -> Add new pixel
          if (fChargeMapIt == fChargeMap.end()) {
            pixel = new ((*fPixelCharge)[fPixelCharge->GetEntriesFast()]) CbmMvdPixelCharge(
              totCharge, ix, iy, point->GetPointId(), point->GetTrackID(), (point->GetX() + point->GetXOut()) / 2,
              (point->GetY() + point->GetXOut()) / 2, point->GetTime(), point->GetFrame());
            //LOG(debug) << "new charched pixel with charge " << totCharge << " at " << ix << " " << iy;
            //  					  fPixelChargeShort.push_back(pixel);
            // 				LOG(debug) << "added pixel to ChargeShort vector ";

            fChargeMap[thispoint] = pixel;
          }

          // Pixel already in map -> Add charge
          else {
            pixel = fChargeMapIt->second;
            //if ( ! pixel ) Fatal("AddChargeToPixel", "Zero pointer in charge map!");
            pixel->AddCharge(totCharge);
            //  					if(pixel->GetCharge()>150)
            // 					{LOG(debug) << "added charge to pixel summing up to "<< pixel->GetCharge();}
          }
          fPixelChargeShort.push_back(pixel);
        }
        else {  //pixel already linked => add charge only
          pixel->AddCharge(totCharge);
          // 				LOG(debug) <<"put charge";
        }


        if (fShowDebugHistos) {
          xCharge          = xCharge + Current[0] * totCharge;
          yCharge          = yCharge + Current[1] * totCharge;
          totClusterCharge = totClusterCharge + totCharge;
        }  // end if
      }    // end for (track segments)


    }  //for y
  }    // for x
       //    LOG(debug) << "End of loops ";
  std::vector<CbmMvdPixelCharge*>::size_type vectorSize = fPixelChargeShort.size();

  for (ULong64_t f = 0; f < vectorSize; f++) {
    CbmMvdPixelCharge* pixelCharge = fPixelChargeShort.at(f);
    if (pixelCharge) {
      pixelCharge->DigestCharge(((float) (point->GetX() + point->GetXOut()) / 2),
                                ((float) (point->GetY() + point->GetYOut()) / 2), fEventTime + point->GetTime(),
                                point->GetPointId(), point->GetTrackID());
    }
    else {
      LOG(warning) << "Warning working on broken pixel ";
    }
  }

  if (fShowDebugHistos) {
    //LOG(debug)  << "produced " << fPixelChargeShort.size() << " Digis with total charge of " << totClusterCharge;
    TVector3 momentum, position;
    if (totClusterCharge > 0) fTotalChargeHisto->Fill(totClusterCharge);
    point->Position(position);
    point->Momentum(momentum);
    fPosXY->Fill(position.X(), position.Y());
    fpZ->Fill(momentum.Z());
    fPosXinIOut->Fill(point->GetZ() - point->GetZOut());
    fAngle->Fill(TMath::ATan(momentum.Pt() / momentum.Pz()) * 180 / TMath::Pi());
  }

  delete[] lowerXArray;
  delete[] upperXArray;
  delete[] lowerYArray;
  delete[] upperYArray;


}  //end of function


// -------------------------------------------------------------------------

void CbmMvdSensorDigitizerTask::ProduceNoise()
{
  Int_t fmaxNoise = 100;
  Int_t noiseHits = (Int_t)(gRandom->Rndm(fmaxNoise) * fmaxNoise);
  Int_t xPix, yPix;
  CbmMvdPixelCharge* pixel;
  pair<Int_t, Int_t> thispoint;

  for (Int_t i = 0; i <= noiseHits; i++) {
    xPix = gRandom->Integer(fNPixelsX);
    yPix = gRandom->Integer(fNPixelsY);

    Double_t Current[3];
    fSensor->PixelToLocal(xPix, yPix, Current);

    thispoint = std::make_pair(xPix, yPix);

    fChargeMapIt = fChargeMap.find(thispoint);

    if (fChargeMapIt == fChargeMap.end()) {
      pixel = new ((*fPixelCharge)[fPixelCharge->GetEntriesFast()])
        CbmMvdPixelCharge(1000, xPix, yPix, 0, -4, Current[0], Current[1]);  // TODO: Add time
      pixel->DigestCharge(Current[0], Current[1], fEventTime, 0, -4);
      fChargeMap[thispoint] = pixel;
    }
    else {
      pixel = fChargeMapIt->second;
      pixel->AddCharge(1000);  // TODO: Add time
      pixel->DigestCharge(Current[0], Current[1], fEventTime, 0, -4);
    }
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------


// -----   Virtual private method SetParContainers   -----------------------
void CbmMvdSensorDigitizerTask::SetParContainers()
{
  //    FairRunAna*    ana  = FairRunAna::Instance();
  //    FairRuntimeDb* rtdb = ana->GetRuntimeDb();
}
// -------------------------------------------------------------------------


// -----    Virtual private method Init   ----------------------------------
void CbmMvdSensorDigitizerTask::InitTask(CbmMvdSensor* mySensor)
{

  //Read information on the sensor von data base
  fSensor = mySensor;

  // LOG(info) << GetName() << ": Initialisation of sensor " << fSensor->GetName();

  fDigis     = new TClonesArray("CbmMvdDigi", 100);
  fDigiMatch = new TClonesArray("CbmMatch", 100);

  fOutputBuffer = new TClonesArray("CbmMvdDigi", 100);
  fInputPoints  = new TClonesArray("CbmMvdPoint", 100);

  if (!fSensor) { LOG(fatal) << "Init(CbmMvdSensor*) called without valid pointer, don't know how to proceed."; };

  ReadSensorInformation();

  // Initialize histogramms used for debugging

  if (fShowDebugHistos) {
    LOG(info) << "Show debug histos in this Plugin";
    fRandomGeneratorTestHisto = new TH1F("TestHisto", "TestHisto", 1000, 0, 12000);
    fResolutionHistoX         = new TH1F("DigiResolutionX", "DigiResolutionX", 1000, -.005, .005);
    fResolutionHistoY         = new TH1F("DigiResolutionY", "DigiResolutionY", 1000, -.005, .005);
    fPosXY                    = new TH2F("DigiPointXY", "DigiPointXY", 100, -6, 6, 100, -6, 6);
    fpZ                       = new TH1F("DigiPointPZ", "DigiPointPZ", 1000, -0.5, 0.5);
    fPosXinIOut               = new TH1F("DigiZInOut", "Digi Z InOut", 1000, -0.04, 0.04);
    fAngle                    = new TH1F("DigiAngle", "DigiAngle", 1000, 0, 90);
    fSegResolutionHistoX      = new TH1F("SegmentResolutionX", "SegmentResolutionX", 1000, -.005, .005);
    fSegResolutionHistoY      = new TH1F("SegmentResolutionY", "SegmentResolutionY", 1000, -.005, .005);
    fSegResolutionHistoZ      = new TH1F("SegmentResolutionZ", "SegmentResolutionZ", 1000, -.005, .005);
    fTotalChargeHisto         = new TH1F("TotalChargeHisto", "TotalChargeHisto", 1000, 0, 12000);
    fTotalSegmentChargeHisto  = new TH1F("TotalSegmentChargeHisto", "TotalSegmentChargeHisto", 1000, 0, 12000);
  }

  /** Screen output **/
  LOG(info) << GetName() << " initialised with parameters: ";
  PrintParameters();

  fPreviousPlugin = nullptr;
  initialized     = kTRUE;
}
// -------------------------------------------------------------------------


// -----   Virtual public method Reinit   ----------------------------------
void CbmMvdSensorDigitizerTask::ReInit(CbmMvdSensor* sensor)
{

  delete fOutputBuffer;

  InitTask(sensor);
}
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdSensorDigitizerTask::Finish()
{


  // PrintParameters();


  if (fShowDebugHistos) {
    TCanvas* c = new TCanvas("DigiCanvas", "DigiCanvas", 150, 10, 800, 600);
    c->Divide(3, 3);
    c->cd(1);
    fResolutionHistoX->Draw();
    fResolutionHistoX->Write();
    c->cd(2);
    fResolutionHistoY->Draw();
    fResolutionHistoY->Write();
    c->cd(3);
    fPosXY->Draw();
    fPosXY->Write();
    c->cd(4);
    fpZ->Draw();
    fpZ->Write();
    c->cd(5);
    fPosXinIOut->Draw();
    fPosXinIOut->Write();
    c->cd(6);
    fAngle->Draw();
    fAngle->Write();
    c->cd(7);
    //fSegResolutionHistoX->Draw();
    fSegResolutionHistoX->Write();
    fTotalSegmentChargeHisto->Draw();
    fTotalSegmentChargeHisto->Write();
    c->cd(8);
    fRandomGeneratorTestHisto->Draw();
    fRandomGeneratorTestHisto->Write();

    fSegResolutionHistoY->Write();
    c->cd(9);
    fTotalChargeHisto->Draw();
    fTotalChargeHisto->Write();
    LOG(info) << "CbmMvdDigitizerL::Finish - Fit of the total cluster charge";
    fTotalChargeHisto->Fit("landau");
    // new TCanvas();
    //fTotalChargeHisto->Draw();
  };
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdSensorDigitizerTask::Reset() {}
// -------------------------------------------------------------------------

void CbmMvdSensorDigitizerTask::PrintParameters() const { LOG(info) << ToString(); }

// -----   Private method PrintParameters   --------------------------------
std::string CbmMvdSensorDigitizerTask::ToString() const
{

  std::stringstream ss;
  ss.setf(ios_base::fixed, ios_base::floatfield);
  ss << "============================================================" << endl;
  ss << "============== Parameters of the Lorentz - Digitizer =======" << endl;
  ss << "============================================================" << endl;


  ss << "Pixel Size X               : " << setw(8) << setprecision(2) << fPixelSizeX * 10000. << " mum" << endl;
  ss << "Pixel Size Y               : " << setw(8) << setprecision(2) << fPixelSizeY * 10000. << " mum" << endl;
  ss << "Epitaxial layer thickness  : " << setw(8) << setprecision(2) << fEpiTh * 10000. << " mum" << endl;
  ss << "Segment Length             : " << setw(8) << setprecision(2) << fSegmentLength * 10000. << " mum" << endl;
  ss << "Diffusion Coefficient      : " << setw(8) << setprecision(2) << fDiffusionCoefficient * 10000. << " mum"
     << endl;
  ss << "Width of Cluster           : " << setw(8) << setprecision(2) << fWidthOfCluster << " * sigma " << endl;
  ss << "ElectronsPerKeV 3.62 eV/eh : " << setw(8) << setprecision(2) << fElectronsPerKeV << endl;
  ss << "CutOnDeltaRays             : " << setw(8) << setprecision(8) << fCutOnDeltaRays << " MeV " << endl;
  ss << "ChargeThreshold            : " << setw(8) << setprecision(2) << fChargeThreshold << endl;
  ss << "Pileup: " << fNPileup << endl;
  ss << "Delta - Pileup: " << fNDeltaElect << endl;
  ss << "=============== End Parameters Digitizer ===================" << endl;
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdSensorDigitizerTask);
