/* Copyright (C) 2017-2019 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                  CbmMvdSensorDigitizerTBTask source file              -----
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


#include "CbmMvdSensorDigitizerTBTask.h"

#include "CbmMvdPileupManager.h"
#include "CbmMvdPoint.h"

#include "FairRuntimeDb.h"

#include "TClonesArray.h"
#include "TObjArray.h"

// Includes from FairRoot
#include "FairEventHeader.h"
#include "FairMCEventHeader.h"
#include "FairRunAna.h"
#include "FairRunSim.h"
#include <Logger.h>


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

//---------   Default constructor   ------------------------------------------
CbmMvdSensorDigitizerTBTask::CbmMvdSensorDigitizerTBTask(){
fPluginIDNumber= 1100
}

// -----   Standard constructor   ------------------------------------------
CbmMvdSensorDigitizerTBTask::CbmMvdSensorDigitizerTBTask()
  : CbmMvdSensorTask()
  , fEpiTh()
  , fSegmentLength()
  , fDiffusionCoefficient()
  , fElectronsPerKeV()
  , fWidthOfCluster()
  , fPixelSizeX()
  , fPixelSizeY()
  , fCutOnDeltaRays()
  , fChargeThreshold()
  , fFanoSilicium()
  , fEsum()
  , fSegmentDepth()
  , fCurrentTotalCharge()
  , fCurrentParticleMass()
  , fCurrentParticleMomentum()
  , fCurrentParticlePdg()
  , fLorentzY0()
  , fLorentzXc()
  , fLorentzW()
  , fLorentzA()
  , fLorentzNorm()
  , fLandauMPV()
  , fLandauSigma()
  , fLandauGain()
  , fLandauRandom(new TRandom3())
  , fPixelSize()
  , fPar0()
  , fPar1()
  , fPar2()
  , fCompression()
  , fResolutionHistoX()
  , fResolutionHistoY()
  , fNumberOfSegments()
  , fCurrentLayer()
  , fEvent()
  , fVolumeId()
  , fNPixelsX()
  , fNPixelsY()
  , fPixelCharge()
  , fDigis()
  , fDigiMatch()
  , fproduceNoise()
  , fPixelChargeShort()
  , fPixelScanAccelerator()
  , fChargeMap()
  , fChargeMapIt()
  , fsensorDataSheet()
  , fSigmaX()
  , fSigmaY()
  , fReadoutTime()
  , fEfficiency()
  , fMergeDist()
  , fFakeRate()
  , fInputPoints()
  , fRandGen()
  , fTimer()
  , fNEvents()
  , fNPoints()
  , fNReal()
  , fNBg()
  , fNFake()
  , fNLost()
  , fNMerged()
  , fTime()
  , fReadoutLast()
  , fReadoutCurrent()
  , fReadoutNext()
  , fSignalPoints()
{
  LOG(debug) << "Starting CbmMvdSensorDigitizerTBTask::CbmMvdSensorDigitizerTBTask() ";

  fRandGen.SetSeed(2736);
  fEvent       = 0;
  fTime        = 0.;
  fSigmaX      = 0.0005;
  fSigmaY      = 0.0005;
  fReadoutTime = 0.00005;

  fSegmentLength           = 0.0001;
  fDiffusionCoefficient    = 0.0055;      // correspondes to the sigma of the gauss with the max drift length
  fElectronsPerKeV         = 276;         //3.62 eV for e-h creation
  fWidthOfCluster          = 3.5;         // in sigmas
  fCutOnDeltaRays          = 0.00169720;  //MeV
  fChargeThreshold         = 100.;        //electrons change 1 to 10
  fFanoSilicium            = 0.115;
  fEsum                    = 0;
  fSegmentDepth            = 0;
  fCurrentTotalCharge      = 0;
  fCurrentParticleMass     = 0;
  fCurrentParticleMomentum = 0;
  fPixelScanAccelerator    = 0;

  fPixelSize = 0.0025;

  fLorentzY0 = -6.1;
  fLorentzXc = 0.;
  fLorentzW  = 1.03;
  fLorentzA  = 477.2;

  //fLorentzNorm=0.00013010281679422413;
  fLorentzNorm = 1;

  fproduceNoise = kFALSE;

  fReadoutLast    = -1.;
  fReadoutCurrent = 0.;

  fPluginIDNumber= 1100
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdSensorDigitizerTBTask::~CbmMvdSensorDigitizerTBTask()
{

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
InitStatus CbmMvdSensorDigitizerTBTask::ReadSensorInformation()
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
  LOG(debug) << fPar0;
  fPar1 = sensorData->GetLorentzPar1();
  LOG(debug) << fPar1;
  fPar2 = sensorData->GetLorentzPar2();
  LOG(debug) << fPar2;

  fLandauMPV   = sensorData->GetLandauMPV();
  fLandauSigma = sensorData->GetLandauSigma();
  fLandauGain  = sensorData->GetLandauGain();
  fEpiTh       = sensorData->GetEpiThickness();

  return kSUCCESS;
}
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
void CbmMvdSensorDigitizerTBTask::SetInputArray(TClonesArray* inputStream)
{

  Int_t i       = 0;
  Int_t nInputs = inputStream->GetEntriesFast();
  while (nInputs > i) {
    new ((*fInputPoints)[fInputPoints->GetEntriesFast()]) CbmMvdPoint(*((CbmMvdPoint*) inputStream->At(i)));
    ++i;
  }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void CbmMvdSensorDigitizerTBTask::SetInput(CbmMvdPoint* point)
{

  new ((*fInputPoints)[fInputPoints->GetEntriesFast()]) CbmMvdPoint(*((CbmMvdPoint*) point));
}
// -----------------------------------------------------------------------------

// -------------- public method ExecChain   ------------------------------------
void CbmMvdSensorDigitizerTBTask::ExecChain() { Exec(); }
// -----------------------------------------------------------------------------

// -----   Virtual public method Exec   ------------------------------------
void CbmMvdSensorDigitizerTBTask::Exec()
{

  if (fPreviousPlugin) {
    fInputPoints->Delete();
    fInputPoints->AbsorbObjects(fPreviousPlugin->GetOutputArray());
  }
  fOutputBuffer->Clear();
  Int_t inputNr = 0;
  Int_t eventNr = 0;
  // Int_t nDigis = 0;  (VF) not used
  GetEventInfo(inputNr, eventNr, fTime);

  fReadoutCurrent = fSensor->GetReadoutTime(fTime);
  fReadoutLast    = fReadoutCurrent - fSensor->GetIntegrationtime();
  if (fInputPoints->GetEntriesFast() > 0) {

    for (Int_t iPoint = 0; iPoint < fInputPoints->GetEntriesFast(); iPoint++) {

      CbmMvdPoint* point = (CbmMvdPoint*) fInputPoints->At(iPoint);

      if (!point) {
        cout << "-W-" << GetName() << ":: Exec:" << endl;
        cout << "    -received bad MC-Point. Ignored." << endl;
        continue;
      }
      if (point->GetStationNr() != fSensor->GetSensorNr()) {
        cout << "-W-" << GetName() << ":: Exec:" << endl;
        cout << "    -received bad MC-Point which doesn't belong here. Ignored." << endl;
        continue;
      }
      //The digitizer acts only on particles, which crossed the station.
      //Particles generated in the sensor or being absorbed in this sensor are ignored
      if (TMath::Abs(point->GetZOut() - point->GetZ()) < 0.9 * fEpiTh) {
        LOG(debug) << "hit not on chip with thickness " << 0.9 * 2 * fSensor->GetDZ();
        LOG(debug) << "hit not on chip with length " << TMath::Abs(point->GetZOut() - point->GetZ());
        continue;
      }
      // Reject for the time being light nuclei (no digitization modell yet)
      if (point->GetPdgCode() > 100000) { continue; }
      ProduceIonisationPoints(point);
      ProducePixelCharge(point);
    }  //loop on MCpoints

    for (Int_t i = 0; i < fPixelCharge->GetEntriesFast(); ++i) {
      CbmMvdPixelCharge* pixel = (CbmMvdPixelCharge*) fPixelCharge->At(i);

      if (pixel->GetCharge() > fChargeThreshold && pixel->GetTime() <= fReadoutLast) {
        /* Not used
		nDigis = fOutputBuffer->GetEntriesFast();

		CbmMvdDigi* digi = new ((*fOutputBuffer)[nDigis]) CbmMvdDigi (fSensor->GetSensorNr(),
						    pixel->GetX(), pixel->GetY(), pixel->GetCharge(),
						    fPixelSizeX, fPixelSizeY, pixel->GetTime(), 
                                                    pixel->GetFrame());
    */


        CbmMatch* match = new CbmMatch();
        for (Int_t iLink = 0; iLink < pixel->GetNContributors(); iLink++) {
          if (pixel->GetTrackID()[iLink] > -1)
            match->AddLink((Double_t) pixel->GetPointWeight()[iLink], pixel->GetPointID()[iLink], eventNr, inputNr);
          else
            match->AddLink((Double_t) pixel->GetPointWeight()[iLink], pixel->GetPointID()[iLink]);
        }

        //digi->SetMatch(match);
        std::pair<Int_t, Int_t> thispoint                          = std::make_pair(pixel->GetX(), pixel->GetY());
        std::pair<std::pair<Int_t, Int_t>, Double_t> thisTimePoint = std::make_pair(thispoint, pixel->GetTime());

        fChargeMap.erase(thisTimePoint);
        fPixelCharge->RemoveAt(i);
      }
      else {
        ;
      }
    }
  }

  else {
    ;
  }

  fPixelCharge->Compress();
  fInputPoints->Delete();
  fSignalPoints.clear();

}  // end of exec
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdSensorDigitizerTBTask::GetEventInfo(Int_t& inputNr, Int_t& eventNr, Double_t& eventTime)
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
void CbmMvdSensorDigitizerTBTask::ProduceIonisationPoints(CbmMvdPoint* point)
{
  /** Produces ionisation points along track segment within 
   ** the active Silicon layer.
   **/
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
    cout << "-W- " << GetName() << " : rawlength > 1.0e+3 : " << rawLength << endl;
    trackLength = 1.0e+3;
  }

  //Smear the energy on each track segment
  Double_t charge = fLandauRandom->Landau(fLandauGain, fLandauSigma / fLandauMPV);

  if (charge > (12000 / fLandauMPV)) { charge = 12000 / fLandauMPV; }  //limit Random generator behaviour
  //Translate the charge to normalized energy

  //     cout << endl << "charge after random generator " << charge << endl;
  Double_t dEmean = charge / (fElectronsPerKeV * 1e6);
  //   cout << endl << "dEmean " << dEmean << endl;
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
    cout << "-W- " << GetName() << " Length of track in detector (z-direction) is 0!!!" << endl;
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
    //cout << endl << "charge " << charge << endl;
    sPoint->sigmaX     = fPixelSize;
    sPoint->sigmaY     = fPixelSize;
    sPoint->charge     = charge;
    totalSegmentCharge = totalSegmentCharge + charge;
  }
}
// -------------------------------------------------------------------------

void CbmMvdSensorDigitizerTBTask::ProducePixelCharge(CbmMvdPoint* point)
{

  fCurrentTotalCharge = 0.0;

  CbmMvdPixelCharge* pixel;

  pair<Int_t, Int_t> thispoint;
  pair<pair<Int_t, Int_t>, Double_t> thisTimePoint;

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

  if (fNumberOfSegments < 2) {
    Fatal("-E- CbmMvdDigitizer: ", "fNumberOfSegments < 2, this makes no sense, check parameters.");
  }

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
  // loop over all pads of interest.
  fPixelChargeShort.clear();
  Int_t ix, iy;
  Double_t ROTime = fSensor->GetReadoutTime(fTime + point->GetTime());
  for (ix = ixLo; ix < ixUp + 1; ix++) {
    for (iy = iyLo; iy < iyUp + 1; iy++) {
      Double_t Current[3];
      fSensor->PixelToLocal(ix, iy, Current);
      pixel = nullptr;
      for (Int_t i = 0; i < fNumberOfSegments; ++i) {
        if (ix < lowerXArray[i] || iy < lowerYArray[i] || ix > upperXArray[i] || iy > upperYArray[i]) { continue; }

        sPoint  = &fSignalPoints[i];
        xCentre = sPoint->x;  //of segment
        yCentre = sPoint->y;  // idem
        sigmaX  = sPoint->sigmaX;
        sigmaY  = sPoint->sigmaY;
        fCurrentTotalCharge += sPoint->charge;

        //compute the charge distributed to this pixel by this segment
        Float_t numerator = sPoint->charge * fLorentzNorm * (0.5 * fPar0 * fPar1 / TMath::Pi());
        Float_t maxCount  = TMath::Max(1.e-10, (((Current[0] - xCentre) * (Current[0] - xCentre))
                                               + ((Current[1] - yCentre) * (Current[1] - yCentre)))
                                                  / fPixelSizeX / fPixelSizeY
                                                + 0.25 * fPar1 * fPar1);

        Float_t totCharge = (numerator / maxCount + fPar2);

        if (totCharge < 1) { continue; }
        if (!pixel) {
          thispoint     = std::make_pair(ix, iy);
          thisTimePoint = std::make_pair(thispoint, ROTime);
          fChargeMapIt  = fChargeMap.find(thisTimePoint);
          if (fChargeMapIt == fChargeMap.end()) {
            pixel = new ((*fPixelCharge)[fPixelCharge->GetEntriesFast()])
              CbmMvdPixelCharge(totCharge, ix, iy, point->GetPointId(), point->GetTrackID(),
                                (point->GetX() + point->GetXOut()) / 2, (point->GetY() + point->GetXOut()) / 2, ROTime);
            fChargeMap[thisTimePoint] = pixel;
          }
          else {
            pixel = fChargeMapIt->second;
            pixel->AddCharge(totCharge);
          }
          fPixelChargeShort.push_back(pixel);
        }
        else {
          pixel->AddCharge(totCharge);
        }
      }  // end for (track segments)
    }    //for y
  }      // for x

  std::vector<CbmMvdPixelCharge*>::size_type vectorSize = fPixelChargeShort.size();

  for (ULong64_t f = 0; f < vectorSize; f++) {
    CbmMvdPixelCharge* pixelCharge = fPixelChargeShort.at(f);
    if (pixelCharge) {
      pixelCharge->DigestCharge(((float) (point->GetX() + point->GetXOut()) / 2),
                                ((float) (point->GetY() + point->GetYOut()) / 2), point->GetPointId(),
                                point->GetTrackID());
    }
    else {
      cout << endl << "Warning working on broken pixel " << endl;
    }
  }

  delete[] lowerXArray;
  delete[] upperXArray;
  delete[] lowerYArray;
  delete[] upperYArray;
}
// -------------------------------------------------------------------------

// -----    Virtual private method Init   ----------------------------------
void CbmMvdSensorDigitizerTBTask::InitTask(CbmMvdSensor* mySensor)
{

  //Read information on the sensor von data base
  fSensor = mySensor;

  // cout << "-I- " << GetName() << ": Initialisation of sensor " << fSensor->GetName() << endl;

  fDigis     = new TClonesArray("CbmMvdDigi", 10000);
  fDigiMatch = new TClonesArray("CbmMatch", 10000);

  fOutputBuffer = new TClonesArray("CbmMvdDigi", 10000);
  fInputPoints  = new TClonesArray("CbmMvdPoint", 10000);

  fPixelCharge = new TClonesArray("CbmMvdPixelCharge", 100000);

  if (!fSensor) {
    Fatal(GetName(), "Fatal error: Init(CbmMvdSensor*) called without valid pointer, "
                     "don't know how to proceed.");
  };

  ReadSensorInformation();

  fPreviousPlugin = NULL;
  initialized     = kTRUE;
}
// -------------------------------------------------------------------------

// -----   Virtual public method Reinit   ----------------------------------
void CbmMvdSensorDigitizerTBTask::ReInit(CbmMvdSensor* sensor)
{

  delete fOutputBuffer;

  InitTask(sensor);
}
// -------------------------------------------------------------------------

// -----   Virtual method Finish   -----------------------------------------
void CbmMvdSensorDigitizerTBTask::Finish() {}
// -------------------------------------------------------------------------

// -----   Private method Reset   ------------------------------------------
void CbmMvdSensorDigitizerTBTask::Reset() {}
// -------------------------------------------------------------------------

// -----   Private method PrintParameters   --------------------------------
void CbmMvdSensorDigitizerTBTask::PrintParameters() {}
// -------------------------------------------------------------------------

ClassImp(CbmMvdSensorDigitizerTBTask);
