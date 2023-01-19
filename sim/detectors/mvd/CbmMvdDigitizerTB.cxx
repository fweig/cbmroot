/* Copyright (C) 2017-2019 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdDigitizerTB source file                -----
// -------------------------------------------------------------------------

// Includes from MVD
#include "CbmMvdDigitizerTB.h"

#include "CbmMvdDetector.h"
#include "CbmMvdPoint.h"
#include "CbmMvdSensorDigitizerTBTask.h"
#include "CbmMvdGeoHandler.h"
#include "CbmDaqBuffer.h"

#include "FairModule.h"
#include "FairRootManager.h"
#include <Logger.h>

// Includes from ROOT
#include "TClonesArray.h"
#include "TStopwatch.h"


#include <iomanip>
#include <iostream>
#include <vector>


using namespace ::std;

// -----   Default constructor   ------------------------------------------
CbmMvdDigitizerTB::CbmMvdDigitizerTB() : CbmMvdDigitizerTB("MVDDigitizerTB", 0, 0) {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdDigitizerTB::CbmMvdDigitizerTB(const char* name, Int_t iMode, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fMode(iMode)
  , eventNumber(0)
  , fShowDebugHistos(kFALSE)
  , fNoiseSensors(kFALSE)
  , fDetector(nullptr)
  , fInputPoints(nullptr)
  , fTracks(nullptr)
  , fDigis(nullptr)
  , fDigiMatch(nullptr)
  , fPerformanceDigi()
  , fDigiPluginNr(0)
  , fFakeRate(-1.)
  , epsilon()
  , fBranchName("MvdPoint")
  , fTimer()
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdDigitizerTB::~CbmMvdDigitizerTB()
{

  if (fDigis) {
    fDigis->Delete();
    delete fDigis;
  }
}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdDigitizerTB::Exec(Option_t* /*opt*/)
{

  fTimer.Start();

  fDigis->Clear();

  if (fInputPoints->GetEntriesFast() > 0) {

    LOG(debug) << "Send Input";
    fDetector->SendInput(fInputPoints);
    LOG(debug) << "Execute DigitizerPlugin Nr. " << fDigiPluginNr;
    fDetector->Exec(fDigiPluginNr);
    LOG(debug) << "End Chain";
    LOG(debug) << "Start writing Digis";
    fDigis->AbsorbObjects(fDetector->GetOutputDigis());
    LOG(debug) << "Total of " << fDigis->GetEntriesFast() << " digis in this Event";
    for (Int_t i = 0; i < fDigis->GetEntriesFast(); ++i) {
      CbmMvdDigi* digi = static_cast<CbmMvdDigi*>(fDigis->At(i)->Clone());
      CbmDaqBuffer::Instance()->InsertData(digi);
    }
  }
  // --- Event log
  LOG(info) << "+ " << setw(20) << GetName() << ": Event " << setw(6) << right << eventNumber << ", real time " << fixed
            << setprecision(6) << fTimer.RealTime() << " s, digis: " << fDigis->GetEntriesFast();
  fTimer.Stop();

  ++eventNumber;
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdDigitizerTB::Init()
{
  cout << "-I- " << GetName() << ": Initialisation..." << endl;
  cout << endl;
  cout << "---------------------------------------------" << endl;
  cout << "-I- Initialising " << GetName() << " ...." << endl;

  eventNumber = 0;

  // **********  RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- " << GetName() << "::Init: No FairRootManager!" << endl;
    return kFATAL;
  }

  // **********  Get input arrays
  fInputPoints = (TClonesArray*) ioman->GetObject(fBranchName);
  fTracks      = (TClonesArray*) ioman->GetObject("MCTrack");

  if (!fInputPoints) {
    LOG(error) << "No MvdPoint branch found. There was no MVD in the "
                  "simulation. Switch this task off";
    return kERROR;
  }


  // **********  Register output array
  fDigis = new TClonesArray("CbmMvdDigi", 10000);
  // ioman->Register("MvdDigi", "Mvd Digis", fDigis, IsOutputBranchPersistent("MvdDigi"));

  fDigiMatch = new TClonesArray("CbmMatch", 100000);
  //ioman->Register("MvdDigiMatch", "Mvd DigiMatches", fDigiMatch, IsOutputBranchPersistent("MvdDigiMatch"));

  fDetector = CbmMvdDetector::Instance();

  // Add the digitizer plugin to all sensors
  std::map<int, CbmMvdSensor*>& sensorMap = fDetector->GetSensorMap();
  UInt_t plugincount=fDetector->GetPluginCount();

  for (auto itr = sensorMap.begin();
              itr != sensorMap.end(); itr++) {
    CbmMvdSensorDigitizerTBTask* digiTask = new CbmMvdSensorDigitizerTBTask();

    itr->second->AddPlugin(digiTask);
    itr->second->SetDigiPlugin(plugincount);
  }
  fDetector->SetSensorArrayFilled(kTRUE);
  fDetector->SetPluginCount(plugincount+1);
  fDigiPluginNr = (UInt_t)(fDetector->GetPluginArraySize());

  fDetector->Init();

  // Screen output
  cout << GetName() << " initialised with parameters: " << endl;
  //PrintParameters();
  cout << "---------------------------------------------" << endl;


  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdDigitizerTB::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdDigitizerTB::Finish()
{
  // cout<< endl << "finishing" << endl;
  fDetector->Finish();
  PrintParameters();
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdDigitizerTB::Reset() { fDigis->Delete(); }
// -------------------------------------------------------------------------

// -----   Private method GetMvdGeometry   ---------------------------------
void CbmMvdDigitizerTB::GetMvdGeometry() {}
// -------------------------------------------------------------------------


// -----   Private method PrintParameters   --------------------------------
void CbmMvdDigitizerTB::PrintParameters()
{

  using namespace std;

  cout.setf(ios_base::fixed, ios_base::floatfield);
  cout << "============================================================" << endl;
  cout << "============== Parameters MvdDigitizer =====================" << endl;
  cout << "============================================================" << endl;
  cout << "=============== End Task ===================================" << endl;
}
// -------------------------------------------------------------------------

ClassImp(CbmMvdDigitizerTB);
