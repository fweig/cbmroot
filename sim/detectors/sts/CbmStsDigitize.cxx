/* Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsDigitize.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.05.2014
 **/

// Include class header
#include "CbmStsDigitize.h"

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
#include "CbmMCTrack.h"
#include "CbmStsDigi.h"
#include "CbmStsPoint.h"

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


// -----   Standard constructor   ------------------------------------------
CbmStsDigitize::CbmStsDigitize()
  : CbmDigitize<CbmStsDigi>("StsDigitize")
  , fIsInitialised(kFALSE)
  ,
  //fModuleParameterMap(),
  fSetup(nullptr)
  , fPoints(nullptr)
  , fTracks(nullptr)
  , fTimer()
  , fSensorDinact(0.12)
  , fSensorPitch(0.0058)
  , fSensorStereoF(0.)
  , fSensorStereoB(7.5)
  , fSensorParameterFile()
  , fSensorConditionFile()
  , fModuleParameterFile()
  , fTimePointLast(-1.)
  , fTimeDigiFirst(-1.)
  , fTimeDigiLast(-1.)
{
  ResetCounters();
  SetGlobalDefaults();
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsDigitize::~CbmStsDigitize() {}
// -------------------------------------------------------------------------


// -----   Content of analogue buffers   -----------------------------------
Int_t CbmStsDigitize::BufferSize() const
{
  Int_t nSignals = 0;
  Int_t nSigModule;
  Double_t t1Module;
  Double_t t2Module;

  for (auto& entry : fModules) {
    entry.second->BufferStatus(nSigModule, t1Module, t2Module);
    nSignals += nSigModule;
  }  //# modules

  return nSignals;
}
// -------------------------------------------------------------------------


// -----   Print the status of the analogue buffers   ----------------------
string CbmStsDigitize::BufferStatus() const
{

  Int_t nSignals = 0;
  Double_t t1    = -1;
  Double_t t2    = -1.;

  Int_t nSigModule;
  Double_t t1Module;
  Double_t t2Module;

  for (auto& entry : fModules) {
    entry.second->BufferStatus(nSigModule, t1Module, t2Module);
    if (nSigModule) {
      nSignals += nSigModule;
      t1 = t1 < 0. ? t1Module : TMath::Min(t1, t1Module);
      t2 = TMath::Max(t2, t2Module);
    }  //? signals in module buffer?
  }    //# modules in setup

  std::stringstream ss;
  ss << nSignals << (nSignals == 1 ? " signal " : " signals ") << "in analogue buffers";
  if (nSignals) ss << " ( from " << fixed << setprecision(3) << t1 << " ns to " << t2 << " ns )";
  return ss.str();
}
// -------------------------------------------------------------------------


// -----   Create a digi object   ------------------------------------------
void CbmStsDigitize::CreateDigi(Int_t address, UShort_t channel, Long64_t time, UShort_t adc, const CbmMatch& match)
{
  assert(time >= 0);

  // Update times of first and last digi
  fTimeDigiFirst = fNofDigis ? TMath::Min(fTimeDigiFirst, Double_t(time)) : time;
  fTimeDigiLast  = TMath::Max(fTimeDigiLast, Double_t(time));

  // Create digi and (if required) match and send them to DAQ
  // Digi time is set later, when creating the timestamp
  CbmStsDigi* digi = new CbmStsDigi(address, channel, 0, adc);
  if (fCreateMatches) {
    CbmMatch* digiMatch = new CbmMatch(match);
    SendData(time, digi, digiMatch);
  }
  else
    SendData(time, digi);
  fNofDigis++;
}
// -------------------------------------------------------------------------


// -----   Task execution   ------------------------------------------------
void CbmStsDigitize::Exec(Option_t* /*opt*/)
{

  // --- Start timer and reset counters
  fTimer.Start();
  ResetCounters();

  // --- For debug: status of analogue buffers
  if (fair::Logger::Logging(fair::Severity::debug)) {
    std::cout << std::endl;
    LOG(debug) << GetName() << ": " << BufferStatus();
  }

  // --- Store previous event time.  Get current event time.
  Double_t eventTimePrevious = fCurrentEventTime;
  GetEventInfo();

  // --- Generate noise from previous to current event time
  if (fParSim->Noise()) {
    Int_t nNoise         = 0;
    Double_t tNoiseStart = fNofEvents ? eventTimePrevious : 1000.;
    Double_t tNoiseEnd   = fCurrentEventTime;
    for (auto& entry : fModules)
      nNoise += entry.second->GenerateNoise(tNoiseStart, tNoiseEnd);
    fNofNoiseTot += Double_t(nNoise);
    LOG(info) << "+ " << setw(20) << GetName() << ": Generated  " << nNoise << " noise signals from t = " << tNoiseStart
              << " ns to " << tNoiseEnd << " ns";
  }

  // --- Analogue response: Process the input array of StsPoints
  ProcessMCEvent();
  LOG(debug) << GetName() << ": " << fNofSignalsF + fNofSignalsB << " signals generated ( " << fNofSignalsF << " / "
             << fNofSignalsB << " )";
  // --- For debug: status of analogue buffers
  if (fair::Logger::Logging(fair::Severity::debug)) { LOG(debug) << GetName() << ": " << BufferStatus(); }

  // --- Readout time: in stream mode the time of the current event.
  // --- Analogue buffers will be digitised for signals at times smaller than
  // --- that time minus a safety margin depending on the module properties
  // --- (dead time and time resolution). In event mode, the readout time
  // --- is set to -1., meaning to digitise everything in the readout buffers.
  Double_t readoutTime = fEventMode ? -1. : fCurrentEventTime;

  // --- Digital response: Process buffers of all modules
  ProcessAnalogBuffers(readoutTime);

  // --- Check status of analogue module buffers
  if (fair::Logger::Logging(fair::Severity::debug)) { LOG(debug) << GetName() << ": " << BufferStatus(); }

  // --- Event log
  LOG(info) << left << setw(15) << GetName() << "[" << fixed << setprecision(3) << fTimer.RealTime() << " s]"
            << " Points: processed " << fNofPointsProc << ", ignored " << fNofPointsIgno
            << ", signals: " << fNofSignalsF << " / " << fNofSignalsB << ", digis: " << fNofDigis;

  // --- Counters
  fTimer.Stop();
  fNofEvents++;
  fNofPointsProcTot += fNofPointsProc;
  fNofPointsIgnoTot += fNofPointsIgno;
  fNofSignalsFTot += fNofSignalsF;
  fNofSignalsBTot += fNofSignalsB;
  fNofDigisTot += fNofDigis;
  fTimeTot += fTimer.RealTime();
}
// -------------------------------------------------------------------------


// -----   Finish run    ---------------------------------------------------
void CbmStsDigitize::Finish()
{

  // --- Start timer and reset counters
  fTimer.Start();
  ResetCounters();

  // --- In event-by-event mode, the analogue buffers should be empty.
  if (fEventMode) {
    if (BufferSize()) {
      LOG(info) << fName << BufferStatus();
      LOG(fatal) << fName << ": Non-empty analogue buffers at end of event "
                 << " in event-by-event mode!";
    }  //? buffers not empty
  }    //? event-by-event mode

  // ---  In time-based mode: process the remaining signals in the buffers
  else {
    std::cout << std::endl;
    LOG(info) << GetName() << ": Finish run";
    LOG(info) << GetName() << ": " << BufferStatus();
    LOG(info) << GetName() << ": Processing analogue buffers";

    // --- Process buffers of all modules
    for (auto& entry : fModules)
      entry.second->ProcessAnalogBuffer(-1.);

    // --- Screen output
    stringstream ss;
    ss << GetName() << ": " << fNofDigis << (fNofDigis == 1 ? " digi " : " digis ") << "created and sent to DAQ ";
    if (fNofDigis)
      ss << "( from " << fixed << setprecision(3) << fTimeDigiFirst << " ns to " << fTimeDigiLast << " ns )";
    LOG(info) << ss.str();
    LOG(info) << GetName() << ": " << BufferStatus();
  }

  fTimer.Stop();
  fNofPointsProcTot += fNofPointsProc;
  fNofPointsIgnoTot += fNofPointsIgno;
  fNofSignalsFTot += fNofSignalsF;
  fNofSignalsBTot += fNofSignalsB;
  fNofDigisTot += fNofDigis;
  fTimeTot += fTimer.RealTime();

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Events processed       : " << fNofEvents;
  LOG(info) << "Points processed / evt : " << fixed << setprecision(1) << fNofPointsProcTot / Double_t(fNofEvents);
  LOG(info) << "Points ignored / evt   : " << fixed << setprecision(1) << fNofPointsIgnoTot / Double_t(fNofEvents);
  LOG(info) << "Signals / event        : " << fNofSignalsFTot / Double_t(fNofEvents) << " / "
            << fNofSignalsBTot / Double_t(fNofEvents);
  LOG(info) << "StsDigi / event        : " << fNofDigisTot / Double_t(fNofEvents);
  LOG(info) << "Digis per point        : " << setprecision(6) << fNofDigisTot / fNofPointsProcTot;
  LOG(info) << "Digis per signal       : " << fNofDigisTot / (fNofSignalsFTot + fNofSignalsBTot);
  LOG(info) << "Noise digis / event    : " << fNofNoiseTot / Double_t(fNofEvents);
  LOG(info) << "Noise fraction         : " << fNofNoiseTot / fNofDigisTot;
  LOG(info) << "Real time per event    : " << fTimeTot / Double_t(fNofEvents) << " s";
  LOG(info) << "=====================================";
}
// -------------------------------------------------------------------------


// -----   Get parameter container from runtime DB   -----------------------
void CbmStsDigitize::SetParContainers()
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
InitStatus CbmStsDigitize::Init()
{

  // Screen output
  std::cout << std::endl;
  LOG(info) << "==========================================================";
  LOG(info) << GetName() << ": Initialisation \n\n";

  // Initialise the STS setup interface from TGeoManager
  fSetup = CbmStsSetup::Instance();
  fSetup->Init();

  // --- Instantiate StsPhysics
  CbmStsPhysics::Instance();

  // --- Initialise parameters
  InitParams();

  // Instantiate modules
  UInt_t nModules = InitModules();
  LOG(info) << GetName() << ": Created " << nModules << " modules";

  // Instantiate sensors
  UInt_t nSensors = InitSensors();
  LOG(info) << GetName() << ": Created " << nSensors << " sensors";


  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Get input array (CbmStsPoint)
  fPoints = (TClonesArray*) ioman->GetObject("StsPoint");
  assert(fPoints);

  // --- Get input array (CbmMCTrack)
  fTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  assert(fTracks);

  RegisterOutput();

  // --- Screen output
  LOG(info) << GetName() << ": Initialisation successful";
  LOG(info) << "==========================================================";
  std::cout << std::endl;

  // Set static initialisation flag
  fIsInitialised = kTRUE;

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Instantiation of modules   --------------------------------------
UInt_t CbmStsDigitize::InitModules()
{

  UInt_t nModules = 0;
  fModules.clear();

  for (Int_t iModule = 0; iModule < fSetup->GetNofModules(); iModule++) {
    CbmStsElement* geoModule = fSetup->GetModule(iModule);
    assert(geoModule);
    UInt_t address          = geoModule->GetAddress();
    auto& modulePar         = fParSetModule->GetParModule(address);
    CbmStsSimModule* module = new CbmStsSimModule(geoModule, &modulePar, this);
    auto result             = fModules.insert({address, module});
    assert(result.second);  // If false, module was already in map
    nModules++;
  }  //# modules in setup

  assert(nModules == fModules.size());
  return nModules;
}
// -------------------------------------------------------------------------


// -----   Initialise parameters   -----------------------------------------
void CbmStsDigitize::InitParams()
{

  // --- The parameter containers are completely initialised here.
  // --- Any contents possibly obtained from the runtimeDb are ignored
  // --- and overwritten.

  // --- Simulation settings
  assert(fParSim);
  assert(fUserParSim);
  *fParSim = *fUserParSim;                   // adapt user settings
  fParSim->SetEventMode(fEventMode);         // from CbmDigitizeBase
  fParSim->SetGenerateNoise(fProduceNoise);  // from CbmDigitizeBase
  /*  fParSim->SetProcesses(fUserParSim->ELossModel(),
                        fUserParSim->LorentzShift(),
                        fUserParSim->Diffusion(),
                        fUserParSim->CrossTalk());
                        */
  if (fEventMode) fParSim->SetGenerateNoise(kFALSE);
  fParSim->setChanged();
  fParSim->setInputVersion(-2, 1);
  LOG(info) << "--- Settings: " << fParSim->ToString();

  // --- Module parameters (global for the time being)
  assert(fParSetModule);
  fParSetModule->clear();
  assert(fUserParModule);
  assert(fUserParAsic);
  fUserParModule->SetAllAsics(*fUserParAsic);
  for (Int_t iModule = 0; iModule < fSetup->GetNofModules(); iModule++) {
    UInt_t address = fSetup->GetModule(iModule)->GetAddress();
    fParSetModule->SetParModule(address, *fUserParModule);
  }
  UInt_t deactivated = 0;
  if (fUserFracDeadChan > 0.) { deactivated = fParSetModule->DeactivateRandomChannels(fUserFracDeadChan); }
  fParSetModule->setChanged();
  fParSetModule->setInputVersion(-2, 1);
  LOG(info) << "--- Using global ASIC parameters: \n       " << fUserParAsic->ToString();
  LOG(info) << "--- Module parameters: " << fParSetModule->ToString();
  LOG(info) << "--- Deactive channels: " << deactivated << " " << fUserFracDeadChan;

  // --- Sensor parameters
  // TODO: The code above is highly unsatisfactory. A better implementation
  // would use data sheets with the actual sensor design parameters.
  assert(fParSetSensor);
  fParSetSensor->clear();
  assert(fUserParSensor);
  for (Int_t iSensor = 0; iSensor < fSetup->GetNofSensors(); iSensor++) {
    CbmStsSensor* sensor = fSetup->GetSensor(iSensor);
    UInt_t address       = sensor->GetAddress();
    TGeoBBox* box        = dynamic_cast<TGeoBBox*>(sensor->GetPnode()->GetShape());
    assert(box);
    Double_t lX = 2. * box->GetDX();
    Double_t lY = 2. * box->GetDY();
    Double_t lZ = 2. * box->GetDZ();
    Double_t dX = lX - 2. * fUserDinactive;
    Double_t dY = lY - 2. * fUserDinactive;

    // In general, the number of strips is given by the active size in x
    // divided by the strip pitch.
    Double_t pitchF   = fUserParSensor->GetPar(6);
    Double_t pitchB   = fUserParSensor->GetPar(7);
    Double_t nStripsF = dX / pitchF;
    Double_t nStripsB = dX / pitchB;

    // The stereo sensors with 6.2092 cm width have 1024 strips à 58 mum.
    if (fUserParSensor->GetClass() == CbmStsSensorClass::kDssdStereo && TMath::Abs(lX - 6.2092) < 0.0001
        && TMath::Abs(pitchF - 0.0058) < 0.0001) {
      nStripsF = 1024.;
      nStripsB = 1024.;
    }

    // Same for sensors with 6.2000 cm width
    if (fUserParSensor->GetClass() == CbmStsSensorClass::kDssdStereo && TMath::Abs(lX - 6.2) < 0.0001
        && TMath::Abs(pitchF - 0.0058) < 0.0001) {
      nStripsF = 1024.;
      nStripsB = 1024.;
    }

    // Create a sensor parameter object and add it to the container
    CbmStsParSensor par(fUserParSensor->GetClass());
    par.SetPar(0, lX);        // Extension in x
    par.SetPar(1, lY);        // Extension in y
    par.SetPar(2, lZ);        // Extension in z
    par.SetPar(3, dY);        // Active size in y
    par.SetPar(4, nStripsF);  // Number of strips front side
    par.SetPar(5, nStripsB);  // Number of strips back side
    for (UInt_t parIndex = 6; parIndex < 10; parIndex++) {
      par.SetPar(parIndex, fUserParSensor->GetPar(parIndex));
    }  //# parameters 6 - 9 (pitches and stereo angles) set by user or default
    fParSetSensor->SetParSensor(address, par);

  }  //# sensors in setup
  LOG(info) << "--- Sensor parameters: " << fParSetSensor->ToString();
  fParSetSensor->setChanged();
  fParSetSensor->setInputVersion(-2, 1);

  // --- Sensor conditions
  assert(fParSetCond);
  fParSetCond->clear();
  fParSetCond->SetGlobalPar(*fUserParCond);
  LOG(info) << "--- Sensor conditions: " << fParSetCond->ToString();
  fParSetCond->setChanged();
  fParSetCond->setInputVersion(-2, 1);
}
// -------------------------------------------------------------------------


// -----   Instantiation of sensors   --------------------------------------
UInt_t CbmStsDigitize::InitSensors()
{

  UInt_t nSensors = 0;
  fSensors.clear();

  fSensorFactory = new CbmStsSimSensorFactory();
  for (Int_t iSensor = 0; iSensor < fSetup->GetNofSensors(); iSensor++) {

    // --- Sensor and module elements in setup
    CbmStsElement* geoSensor = fSetup->GetSensor(iSensor);
    assert(geoSensor);  // Valid geo sensor
    UInt_t sensAddress       = geoSensor->GetAddress();
    CbmStsElement* geoModule = geoSensor->GetMother();
    assert(geoModule);  // Valid geo mother module
    UInt_t moduAddress = geoModule->GetAddress();

    // --- Simulation module
    auto moduIt = fModules.find(moduAddress);
    assert(moduIt != fModules.end());
    assert(moduIt->second);  // Valid sim module

    // --- Sensor parameters
    const CbmStsParSensor& sensorPar = fParSetSensor->GetParSensor(sensAddress);

    // --- Create simulation sensor accordoing to its class
    auto result = fSensors.insert(std::make_pair(sensAddress, fSensorFactory->CreateSensor(sensorPar)));
    assert(result.second);  // If false, sensor was already in map
    auto& sensor = result.first->second;
    assert(sensor);  // Valid sensor pointer

    // Assign setup element and module
    sensor->SetElement(geoSensor);
    sensor->SetModule(moduIt->second);

    // Assign simulation settings
    sensor->SetSimSettings(fParSim);

    // Set sensor conditions
    assert(fParSetCond);
    const CbmStsParSensorCond& cond = fParSetCond->GetParSensor(sensAddress);
    sensor->SetConditions(&cond);

    // Get the magnetic field in the sensor centre
    Double_t bx = 0.;
    Double_t by = 0.;
    Double_t bz = 0.;
    if (FairRun::Instance()->GetField()) {
      Double_t local[3] = {0., 0., 0.};  // sensor centre in local C.S.
      Double_t global[3];                // sensor centre in global C.S.
      geoSensor->GetPnode()->GetMatrix()->LocalToMaster(local, global);
      Double_t field[3] = {0., 0., 0.};  // magnetic field components
      FairRun::Instance()->GetField()->Field(global, field);
      bx = field[0] / 10.;  // kG->T
      by = field[1] / 10.;  // kG->T
      bz = field[2] / 10.;  // kG->T
    }                       //? field present
    sensor->SetField(bx, by, bz);

    // Initialise sensor
    assert(sensor->Init());

    nSensors++;
  }  //# sensors in setup

  assert(nSensors == fSensors.size());
  return nSensors;
}
// -------------------------------------------------------------------------


// -----   Initialisation of setup    --------------------------------------
void CbmStsDigitize::InitSetup()
{

  // Initialise the STS setup interface from TGeoManager
  fSetup = CbmStsSetup::Instance();
  fSetup->Init();

  // Set parameters to modules and sensor
  fSetup->SetModuleParameters(fParSetModule);
  fSetup->SetSensorParameters(fParSetSensor);
  fSetup->SetSensorConditions(fParSetCond);

  // Individual configuration
  //fSetup->SetModuleParameterMap(fModuleParameterMap);
}
// -------------------------------------------------------------------------


// -----   Process the analogue buffers of all modules   -------------------
void CbmStsDigitize::ProcessAnalogBuffers(Double_t readoutTime)
{

  // --- Process analogue buffers of all modules
  for (auto& it : fModules)
    it.second->ProcessAnalogBuffer(readoutTime);
}
// -------------------------------------------------------------------------


// -----   Process points from MC event    ---------------------------------
void CbmStsDigitize::ProcessMCEvent()
{

  // --- Loop over all StsPoints and execute the ProcessPoint method
  assert(fPoints);
  for (Int_t iPoint = 0; iPoint < fPoints->GetEntriesFast(); iPoint++) {
    const CbmStsPoint* point = (const CbmStsPoint*) fPoints->At(iPoint);
    CbmLink* link            = new CbmLink(1., iPoint, fCurrentMCEntry, fCurrentInput);

    // --- Ignore points from secondaries if the respective flag is set
    if (fParSim->OnlyPrimaries()) {
      Int_t iTrack = point->GetTrackID();
      if (iTrack >= 0) {  // MC track is present
        CbmMCTrack* track = (CbmMCTrack*) fTracks->At(iTrack);
        assert(track);
        if (track->GetGeantProcessId() != kPPrimary) {
          fNofPointsIgno++;
          continue;
        }
      }  //? MC track present
    }    //? discard secondaries

    // --- Ignore points with unphysical time
    // Such instances were observed using Geant4 in the fairsoft jun19 version.
    // The cut at 1 ms from the event start is somehow arbitrary, but should suit the purpose.
    // If not cut here, the time range for the StsDigi (2^31 ns) might be exceeded in
    // flexible time slices or in event-by-event simulation.
    if (point->GetTime() > 1.e6) {
      fNofPointsIgno++;
      continue;
    }

    // --- Process the StsPoint
    ProcessPoint(point, fCurrentEventTime, link);
    fNofPointsProc++;
    delete link;
  }  //# StsPoints
}
// -------------------------------------------------------------------------


// -----  Process a StsPoint   ---------------------------------------------
void CbmStsDigitize::ProcessPoint(const CbmStsPoint* point, Double_t eventTime, CbmLink* link)
{

  // --- Get the sensor the point is in
  UInt_t address = static_cast<UInt_t>(point->GetDetectorID());
  assert(fSensors.count(address));
  auto& sensor = fSensors.find(address)->second;
  assert(sensor);
  Int_t status = sensor->ProcessPoint(point, eventTime, link);

  // --- Statistics
  Int_t nSignalsF = status / 1000;
  Int_t nSignalsB = status - 1000 * nSignalsF;
  LOG(debug2) << GetName() << ": Produced signals: " << nSignalsF + nSignalsB << " ( " << nSignalsF << " / "
              << nSignalsB << " )";
  fNofSignalsF += nSignalsF;
  fNofSignalsB += nSignalsB;
}
// -------------------------------------------------------------------------


// -----   Private method ReInit   -----------------------------------------
InitStatus CbmStsDigitize::ReInit()
{

  fSetup = CbmStsSetup::Instance();

  return kERROR;
}
// -------------------------------------------------------------------------


// -----   Reset event counters   ------------------------------------------
void CbmStsDigitize::ResetCounters()
{
  fTimeDigiFirst = fTimeDigiLast = -1.;
  fNofPointsProc                 = 0;
  fNofPointsIgno                 = 0;
  fNofSignalsF                   = 0;
  fNofSignalsB                   = 0;
  fNofDigis                      = 0;
}
// -------------------------------------------------------------------------


// -----   Global default values for parameters   --------------------------
void CbmStsDigitize::SetGlobalDefaults()
{

  // The global default values cannot be directly stored in the parameter
  // containers, since these are not yet initialised from the database.

  // --- Protect against setting after initialisation
  assert(!fIsInitialised);

  // --- Simulation settings
  if (fUserParSim) delete fUserParSim;
  fUserParSim            = new CbmStsParSim();
  Bool_t eventMode       = kFALSE;
  CbmStsELoss eLossModel = CbmStsELoss::kUrban;
  Bool_t lorentzShift    = kTRUE;
  Bool_t diffusion       = kTRUE;
  Bool_t crossTalk       = kTRUE;
  Bool_t generateNoise   = kTRUE;
  fUserParSim->SetEventMode(eventMode);
  fUserParSim->SetProcesses(eLossModel, lorentzShift, diffusion, crossTalk);
  fUserParSim->SetGenerateNoise(generateNoise);
  // Note that the run mode and the generation of noise are centrally set
  // through the base class CbmDigitizeBase, such that the settings here
  // will be overwritten later (in Init).

  // --- Module parameters
  if (fUserParModule) delete fUserParModule;
  UInt_t nChannels     = 2048;  // Number of module readout channels
  UInt_t nAsicChannels = 128;   // Number of readout channels per ASIC
  fUserParModule       = new CbmStsParModule(nChannels, nAsicChannels);

  // --- ASIC parameters
  if (fUserParAsic) delete fUserParAsic;
  UShort_t nAdc      = 32;         // Number of ADC channels (5 bit)
  Double_t dynRange  = 75000.;     // Dynamic range [e]
  Double_t threshold = 3000.;      // Threshold [e]
  Double_t timeResol = 5.;         // Time resolution [ns]
  Double_t deadTime  = 800.;       // Channel dead time [ns]
  Double_t noiseRms  = 1000.;      // RMS of noise [e]
  Double_t znr       = 3.9789e-3;  // Zero-crossing noise rate [1/ns]
  fUserParAsic       = new CbmStsParAsic(nAsicChannels, nAdc, dynRange, threshold, timeResol, deadTime, noiseRms, znr);
  // --- Sensor parameters
  // --- Here, only the default pitch and stereo angles are defined. The
  // --- other parameters are extracted from the geometry.
  if (fUserParSensor) delete fUserParSensor;
  CbmStsSensorClass sClass = CbmStsSensorClass::kDssdStereo;
  fUserParSensor           = new CbmStsParSensor(sClass);
  Double_t pitchF          = 0.0058;  // Strip pitch front side
  Double_t pitchB          = 0.0058;  // Strip pitch back side
  Double_t stereoF         = 0.;      // Stereo angle front side
  Double_t stereoB         = 7.5;     // Stereo angle back side [deg]
  fUserParSensor->SetPar(6, pitchF);
  fUserParSensor->SetPar(7, pitchB);
  fUserParSensor->SetPar(8, stereoF);
  fUserParSensor->SetPar(9, stereoB);
  fUserDinactive = 0.12;  // Size of inactive sensor border [cm]

  // --- Sensor conditions
  if (fUserParCond) delete fUserParCond;
  Double_t vFd         = 70.;   // Full-depletion voltage
  Double_t vBias       = 140.;  // Bias voltage
  Double_t temperature = 268.;  // Temperature
  Double_t cCoupling   = 17.5;  // Coupling capacitance [pF]
  Double_t cInterstrip = 1.;    // Inter-strip capacitance
  fUserParCond         = new CbmStsParSensorCond(vFd, vBias, temperature, cCoupling, cInterstrip);
}
// -------------------------------------------------------------------------


// -----   Set the global module parameters   ------------------------------
void CbmStsDigitize::SetGlobalAsicParams(UShort_t nChannels, UShort_t nAdc, Double_t dynRange, Double_t threshold,
                                         Double_t timeResolution, Double_t deadTime, Double_t noise,
                                         Double_t zeroNoiseRate)
{
  assert(!fIsInitialised);
  assert(nAdc > 0);
  if (fUserParAsic) delete fUserParAsic;
  fUserParAsic =
    new CbmStsParAsic(nChannels, nAdc, dynRange, threshold, timeResolution, deadTime, noise, zeroNoiseRate);
}
// -------------------------------------------------------------------------


// -----   Set the global module parameters   ------------------------------
void CbmStsDigitize::SetGlobalModuleParams(UInt_t nChannels, UInt_t nAsicChannels)
{
  assert(!fIsInitialised);

  if (fUserParModule) delete fUserParModule;
  fUserParModule = new CbmStsParModule(nChannels, nAsicChannels);
}
// -------------------------------------------------------------------------


// -----   Set the global sensor conditions   ------------------------------
void CbmStsDigitize::SetGlobalSensorConditions(Double_t vFd, Double_t vBias, Double_t temperature, Double_t cCoupling,
                                               Double_t cInterstrip)
{
  assert(!fIsInitialised);

  if (fUserParCond) delete fUserParCond;
  fUserParCond = new CbmStsParSensorCond(vFd, vBias, temperature, cCoupling, cInterstrip);
}
// -------------------------------------------------------------------------


// -----   Set sensor parameter file   -------------------------------------
void CbmStsDigitize::SetModuleParameterFile(const char* fileName)
{

  assert(!fIsInitialised);
  fModuleParameterFile = fileName;
}
// -------------------------------------------------------------------------


// -----   Set physical processes for the analogue response  ---------------
void CbmStsDigitize::SetProcesses(CbmStsELoss eLossModel, Bool_t useLorentzShift, Bool_t useDiffusion,
                                  Bool_t useCrossTalk)
{
  if (fIsInitialised) {
    LOG(error) << GetName() << ": physics processes must be set before "
               << "initialisation! Statement will have no effect.";
    return;
  }

  fParSim->SetProcesses(eLossModel, useLorentzShift, useDiffusion, useCrossTalk);
}
// -------------------------------------------------------------------------


// -----   Set sensor condition file   -------------------------------------
void CbmStsDigitize::SetSensorConditionFile(const char* fileName)
{

  if (fIsInitialised) {
    LOG(fatal) << GetName() << ": sensor conditions must be set before initialisation!";
    return;
  }
  fSensorConditionFile = fileName;
}
// -------------------------------------------------------------------------


// -----   Set sensor parameter file   -------------------------------------
void CbmStsDigitize::SetSensorParameterFile(const char* fileName)
{

  if (fIsInitialised) {
    LOG(fatal) << GetName() << ": sensor parameters must be set before initialisation!";
    return;
  }
  fSensorParameterFile = fileName;
}
// -------------------------------------------------------------------------


// -----   Usage of primary tracks only   ----------------------------------
void CbmStsDigitize::UseOnlyPrimaries(Bool_t flag) { fUserParSim->SetOnlyPrimaries(flag); }
// -------------------------------------------------------------------------

ClassImp(CbmStsDigitize)
