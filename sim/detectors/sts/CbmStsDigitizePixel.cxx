/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

/** @file CbmStsDigitizePixel.cxx
 ** @author Sergey Gorbunov
 ** @date 09.12.2021
 **/

// Include class header
#include "CbmStsDigitizePixel.h"

// Includes from C++
#include <cassert>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// Includes from ROOT
#include "TClonesArray.h"
#include "TGeoBBox.h"
#include "TGeoMatrix.h"
#include "TGeoPhysicalNode.h"
#include "TGeoVolume.h"
#include <TMCProcess.h>

// Includes from FairRoot
#include "FairEventHeader.h"
#include "FairField.h"
#include "FairLink.h"
#include "FairMCEventHeader.h"
#include "FairMCPoint.h"
#include "FairRunAna.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

// Includes from CbmRoot
#include "CbmStsDigi.h"
#include "CbmStsParSetModule.h"
#include "CbmStsPoint.h"
#include "CbmStsSetup.h"

// Includes from STS
#include "CbmStsModule.h"
#include "CbmStsParAsic.h"
#include "CbmStsParModule.h"
#include "CbmStsParSensor.h"
#include "CbmStsParSensorCond.h"
#include "CbmStsParSetModule.h"
#include "CbmStsParSetSensor.h"
#include "CbmStsParSetSensorCond.h"
#include "CbmStsParSim.h"
#include "CbmStsPhysics.h"
#include "CbmStsSensor.h"
#include "CbmStsSetup.h"
#include "CbmStsSimSensorFactory.h"


using std::fixed;
using std::left;
using std::right;
using std::setprecision;
using std::setw;
using std::string;
using std::stringstream;

using namespace CbmSts;

ClassImp(CbmStsDigitizePixel);

// -----   Standard constructor   ------------------------------------------
CbmStsDigitizePixel::CbmStsDigitizePixel(Double_t pitchXcm, Double_t pitchYcm, Double_t pitchTns)
  : CbmDigitize<CbmStsDigi>("StsDigitizePixel")
  , fPitchXcm(pitchXcm)
  , fPitchYcm(pitchYcm)
  , fPitchTns(pitchTns)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsDigitizePixel::~CbmStsDigitizePixel() {}
// -------------------------------------------------------------------------


// -----   Finish run    ---------------------------------------------------
void CbmStsDigitizePixel::Finish() {}
// -------------------------------------------------------------------------


// -----   Get parameter container from runtime DB   -----------------------
void CbmStsDigitizePixel::SetParContainers()
{
  assert(FairRunAna::Instance());
  FairRuntimeDb* rtdb = FairRunAna::Instance()->GetRuntimeDb();
  fParSim             = static_cast<CbmStsParSim*>(rtdb->getContainer("CbmStsParSim"));
  fParSetModule       = static_cast<CbmStsParSetModule*>(rtdb->getContainer("CbmStsParSetModule"));
  fParSetSensor       = static_cast<CbmStsParSetSensor*>(rtdb->getContainer("CbmStsParSetSensor"));
  fParSetCond         = static_cast<CbmStsParSetSensorCond*>(rtdb->getContainer("CbmStsParSetSensorCond"));
}
// -------------------------------------------------------------------------


// -----   Initialisation    -----------------------------------------------
InitStatus CbmStsDigitizePixel::Init()
{

  // --- Instantiate StsPhysics
  CbmStsPhysics::Instance();

  // Initialise the STS setup interface from TGeoManager
  fSetup = CbmStsSetup::Instance();
  if (!fSetup->IsInit()) { fSetup->Init(nullptr); }

  // --- Initialise parameters
  InitParams();

  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Get input array (CbmStsPoint)
  fPoints = (TClonesArray*) ioman->GetObject("StsPoint");
  assert(fPoints);

  // always create matches

  SetCreateMatches(kTRUE);

  RegisterOutput();

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Private method ReInit   -----------------------------------------
InitStatus CbmStsDigitizePixel::ReInit() { return Init(); }
// -------------------------------------------------------------------------


// -----   Task execution   ------------------------------------------------
void CbmStsDigitizePixel::Exec(Option_t* /*opt*/)
{
  TStopwatch timer;

  // ---  Get current event time.
  GetEventInfo();

  // -----   Process points from MC event    ---------------------------------

  assert(fPoints);

  std::vector<std::pair<double, int>> sortedPoints;
  sortedPoints.reserve(fPoints->GetEntriesFast());

  for (Int_t iPoint = 0; iPoint < fPoints->GetEntriesFast(); iPoint++) {
    const CbmStsPoint* point = (const CbmStsPoint*) fPoints->At(iPoint);
    sortedPoints.push_back(std::make_pair(point->GetTime(), iPoint));
  }

  std::sort(sortedPoints.begin(), sortedPoints.end(),
            [](std::pair<double, int>& left, std::pair<double, int>& right) { return left.first < right.first; });

  for (UInt_t iSorted = 0; iSorted < sortedPoints.size(); iSorted++) {
    Int_t iPoint             = sortedPoints[iSorted].second;
    const CbmStsPoint* point = (const CbmStsPoint*) fPoints->At(iPoint);

    UInt_t address   = static_cast<UInt_t>(point->GetDetectorID());
    UShort_t channel = 0;
    double timef     = fCurrentEventTime + point->GetTime();
    timef            = (floor(timef / fPitchTns) + 0.5) * fPitchTns;
    Long64_t time    = Long64_t(round(timef));
    UShort_t adc     = 30;
    assert(time >= 0);

    // Create digi and (if required) match and send them to DAQ
    // The time is sent separately from the digi in double precision.
    // It will be set in the digi later, when creating the timestamp for the time slice.

    CbmStsDigi* digi    = new CbmStsDigi(address, channel, 0, adc);
    CbmMatch* digiMatch = nullptr;
    if (fCreateMatches) {
      digiMatch = new CbmMatch;
      digiMatch->AddLink(CbmLink(1., iPoint, fCurrentMCEntry, fCurrentInput));
    }
    SendData(time, digi, digiMatch);
  }

  timer.Stop();
  if (fCreateMatches) { std::cout << "create matches" << std::endl; }
  // --- Event log
  LOG(info) << left << setw(15) << GetName() << "[" << fixed << setprecision(3) << timer.RealTime() << " s]"
            << " Points processed " << fPoints->GetEntriesFast();
}
// -------------------------------------------------------------------------


// -----   Initialise parameters   -----------------------------------------
void CbmStsDigitizePixel::InitParams()
{

  // --- The parameter containers are completely initialised here.
  // --- Any contents possibly obtained from the runtimeDb are ignored
  // --- and overwritten.

  // --- Simulation settings
  assert(fParSim);

  // --- Simulation settings
  {
    fParSim->SetEventMode(fEventMode);         // from CbmDigitizeBase
    fParSim->SetGenerateNoise(fProduceNoise);  // from CbmDigitizeBase
    if (fEventMode) fParSim->SetGenerateNoise(kFALSE);
    fParSim->setChanged();
    fParSim->setInputVersion(-2, 1);
    LOG(info) << GetName() << "--- Settings: " << fParSim->ToString();
  }


  {  // --- Module parameters

    UInt_t nChannels     = 2048;  // Number of module readout channels
    UInt_t nAsicChannels = 128;   // Number of readout channels per ASIC

    // --- ASIC parameters
    UShort_t nAdc      = 32;                     // Number of ADC channels (5 bit)
    Double_t dynRange  = 75000.;                 // Dynamic range [e]
    Double_t threshold = 3000.;                  // Threshold [e]
    Double_t timeResol = fPitchTns / sqrt(12.);  // Time resolution [ns]
    Double_t deadTime  = 800.;                   // Channel dead time [ns]
    Double_t noiseRms  = 1000.;                  // RMS of noise [e]
    Double_t znr       = 3.9789e-3;              // Zero-crossing noise rate [1/ns]
    CbmStsParAsic userParAsic(nAsicChannels, nAdc, dynRange, threshold, timeResol, deadTime, noiseRms, znr);

    CbmStsParModule userParModule(nChannels, nAsicChannels);
    userParModule.SetAllAsics(userParAsic);

    assert(fParSetModule);
    fParSetModule->clear();
    for (Int_t iModule = 0; iModule < fSetup->GetNofModules(); iModule++) {
      UInt_t address = fSetup->GetModule(iModule)->GetAddress();
      fParSetModule->SetParModule(address, userParModule);
    }

    fParSetModule->setChanged();
    fParSetModule->setInputVersion(-2, 1);
    LOG(info) << GetName() << "--- Using global ASIC parameters: \n       " << userParAsic.ToString();
    LOG(info) << GetName() << "--- Module parameters: " << fParSetModule->ToString();
  }

  // --- Sensor parameters
  {
    assert(fParSetSensor);
    fParSetSensor->clear();

    // --- Sensor parameters

    for (Int_t iSensor = 0; iSensor < fSetup->GetNofSensors(); iSensor++) {
      CbmStsSensor* sensor = fSetup->GetSensor(iSensor);
      UInt_t address       = sensor->GetAddress();
      TGeoBBox* box        = dynamic_cast<TGeoBBox*>(sensor->GetPnode()->GetShape());
      assert(box);
      Double_t lX = 2. * box->GetDX();
      Double_t lY = 2. * box->GetDY();
      Double_t lZ = 2. * box->GetDZ();
      Double_t dY = lY;

      // Create a sensor parameter object and add it to the container
      CbmStsParSensor par(CbmStsSensorClass::kDssdStereo);

      // sensor parameters currently not used by the tracker

      par.SetPar(0, lX);     // Extension in x
      par.SetPar(1, lY);     // Extension in y
      par.SetPar(2, lZ);     // Extension in z
      par.SetPar(3, dY);     // Active size in y
      par.SetPar(4, 1024.);  // Number of strips front side
      par.SetPar(5, 1024.);  // Number of strips back side

      // sensor parameters used by the tracker

      par.SetPar(6, fPitchXcm);  // Strip pitch front side
      par.SetPar(7, fPitchYcm);  // Strip pitch back side
      par.SetPar(8, 0.);         // Stereo angle front side [deg]
      par.SetPar(9, 90);         // Stereo angle back side [deg]

      fParSetSensor->SetParSensor(address, par);
    }

    LOG(info) << GetName() << "--- Sensor parameters: " << fParSetSensor->ToString();
    fParSetSensor->setChanged();
    fParSetSensor->setInputVersion(-2, 1);
    fParSetSensor->setDescription("Experimental STS Pixels");
  }
}
// -------------------------------------------------------------------------
