/* Copyright (C) 2014-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer], Florian Uhlig, Volker Friese */

// -------------------------------------------------------------------------
// -----                    CbmMvdDigitizer source file                -----
// -------------------------------------------------------------------------

// Includes from MVD
#include "CbmMvdDigitizer.h"

#include "CbmMatch.h"
#include "CbmMvdDetector.h"
#include "CbmMvdDigi.h"
#include "CbmMvdPileupManager.h"
#include "CbmMvdPoint.h"
#include "SensorDataSheets/CbmMvdMimosa26AHR.h"
#include "plugins/tasks/CbmMvdSensorDigitizerTask.h"
#include "tools/CbmMvdGeoHandler.h"

// Includes from FAIR
#include "FairModule.h"
#include "FairRootManager.h"
#include <Logger.h>

// Includes from ROOT
#include "TClonesArray.h"
#include "TStopwatch.h"

// Includes from C++
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

// -----   Default constructor   ------------------------------------------
CbmMvdDigitizer::CbmMvdDigitizer()
  : CbmDigitize<CbmMvdDigi>("MvdDigitize")
  , fMode(0)
  , fShowDebugHistos(kFALSE)
  , fNoiseSensors(kFALSE)
  , fDetector(nullptr)
  , fInputPoints(nullptr)
  , fMcPileUp(nullptr)
  , fTmpMatch(nullptr)
  , fTmpDigi(nullptr)
  , fDigiVect()
  , fMatchVect()
  , fPerformanceDigi()
  , fDigiPluginNr(0)
  , fFakeRate(-1.)
  , fNPileup(-1)
  , fNDeltaElect(-1)
  , fDeltaBufferSize(-1)
  , fBgBufferSize(-1)
  , epsilon()
  , fInputBranchName("")
  , fBgFileName("")
  , fDeltaFileName("")
  , fTimer()
  , fPileupManager(nullptr)
  , fDeltaManager(nullptr)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdDigitizer::CbmMvdDigitizer(const char* name, Int_t iMode, Int_t /*iVerbose*/)
  : CbmDigitize<CbmMvdDigi>(name)
  , fMode(iMode)
  , fShowDebugHistos(kFALSE)
  , fNoiseSensors(kFALSE)
  , fDetector(nullptr)
  , fInputPoints(nullptr)
  , fMcPileUp(nullptr)
  , fTmpMatch(nullptr)
  , fTmpDigi(nullptr)
  , fDigiVect()
  , fMatchVect()
  , fPerformanceDigi()
  , fDigiPluginNr(0)
  , fFakeRate(-1.)
  , fNPileup(0)
  , fNDeltaElect(0)
  , fDeltaBufferSize(-1)
  , fBgBufferSize(-1)
  , epsilon()
  , fInputBranchName("MvdPoint")
  , fBgFileName("")
  , fDeltaFileName("")
  , fTimer()
  , fPileupManager(nullptr)
  , fDeltaManager(nullptr)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdDigitizer::~CbmMvdDigitizer()
{

  if (fMcPileUp) {
    fMcPileUp->Delete();
    delete fMcPileUp;
  }
  delete fPileupManager;
  delete fDeltaManager;
}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdDigitizer::Exec(Option_t* /*opt*/)
{
  // --- Start timer
  using namespace std;

  fTimer.Start();
  GetEventInfo();  // event number and time

  BuildEvent();
  Int_t nPoints = fInputPoints->GetEntriesFast();
  Int_t nDigis  = 0;
  if (fInputPoints->GetEntriesFast() > 0) {
    LOG(debug) << "//----------------------------------------//";
    LOG(debug) << fName << ": Send Input";
    fDetector->SendInput(fInputPoints);
    LOG(debug) << fName << ": Execute DigitizerPlugin Nr. " << fDigiPluginNr;
    fDetector->Exec(fDigiPluginNr);
    LOG(debug) << fName << ": End Chain";

    // --- Send produced digis to DAQ
    fTmpDigi  = fDetector->GetOutputDigis();
    fTmpMatch = fDetector->GetOutputDigiMatchs();

    Int_t nEntries = fTmpDigi->GetEntriesFast();
    for (Int_t index = 0; index < nEntries; index++) {

      CbmMvdDigi* digi  = dynamic_cast<CbmMvdDigi*>(fTmpDigi->At(index));
      CbmMvdDigi* digi1 = new CbmMvdDigi(*digi);
      assert(digi1);
      fDigiVect.push_back(digi1);

      //     CbmMatch match{*(dynamic_cast<CbmMatch*>(fTmpMatch->At(index)))};
      //     CbmMatch* match1 = new CbmMatch(match);
      CbmMatch* match  = dynamic_cast<CbmMatch*>(fTmpMatch->At(index));
      CbmMatch* match1 = new CbmMatch(*match);
      fMatchVect.push_back(match1);

      //digi1->SetMatch(match1);
      SendData(digi1->GetTime(), digi1, match1);
      nDigis++;
    }


    /*
   for (Int_t index = 0; index < fTmpDigi->GetEntriesFast(); index++) {

     LOG(info) << "Size: " << fTmpDigi->GetEntriesFast() << ", "
               << fTmpDigi->GetEntries();

     CbmMvdDigi* digi = dynamic_cast<CbmMvdDigi*>(fTmpDigi->Remove(fTmpDigi->At(index)));
     digi->Print();
     fDigiVect.push_back(digi);

     CbmMatch* match = dynamic_cast<CbmMatch*>(fTmpMatch->Remove(fTmpMatch->At(index)));
     match->Print();
     fMatchVect.push_back(match);

     digi->SetMatch(match);
     SendDigi(digi);
     nDigis++;
   }
*/
    // TODO: (VF) There seem to be no entries in the match array, nor matches
    // attached to the digi object
    LOG(debug) << fName << ": Sent " << nDigis << " digis to DAQ";

    //fDigis->AbsorbObjects(fDetector->GetOutputDigis(),0,fDetector->GetOutputArray(fDigiPluginNr)->GetEntriesFast()-1);
    //LOG(debug) << "Total of " << fDigis->GetEntriesFast() << " digis in this Event";
    //if(fVerbose) cout << "Start writing DigiMatchs" << endl;
    //fDigiMatch->AbsorbObjects(fDetector->GetOutputDigiMatchs(),0,fDetector->GetOutputDigiMatchs()->GetEntriesFast()-1);
    //if(fVerbose) cout << "Total of " << fDigiMatch->GetEntriesFast() << " digisMatch in this Event" << endl;
    LOG(debug) << "//----------------------------------------//";
    //LOG(info) << "+ " << setw(20) << GetName() << ": Created: "
    //          << fDigis->GetEntriesFast() << " digis in "
    //          << fixed << setprecision(6) << fTimer.RealTime() << " s";
  }

  // --- Event log
  fTimer.Stop();
  LOG(info) << "+ " << setw(15) << GetName() << ": Event " << setw(6) << right << fCurrentEvent << " at " << fixed
            << setprecision(3) << fCurrentEventTime << " ns, points: " << nPoints << ", digis: " << nDigis
            << ". Exec time " << setprecision(6) << fTimer.RealTime() << " s.";
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdDigitizer::Init()
{
  cout << "-I- " << GetName() << ": Initialisation..." << endl;
  cout << endl;
  cout << "---------------------------------------------" << endl;
  cout << "-I- Initialising " << GetName() << " ...." << endl;

  // **********  RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- " << GetName() << "::Init: No FairRootManager!" << endl;
    return kFATAL;
  }

  // **********  Get input arrays
  fInputPoints = (TClonesArray*) ioman->GetObject("MvdPoint");

  if (!fInputPoints) {
    LOG(error) << "No MvdPoint branch found. There was no MVD in the "
                  "simulation. Switch this task off";
    return kERROR;
  }


  // **********  Register output arrays
  RegisterOutput();
  fMcPileUp = new TClonesArray("CbmMvdPoint", 10000);
  ioman->Register("MvdPileUpMC", "Mvd MC Points after Pile Up", fMcPileUp, IsOutputBranchPersistent("MvdPileUpMC"));

  fDetector = CbmMvdDetector::Instance();

  // **********  Create pileup manager if necessary
  if (fNPileup >= 1 && !(fPileupManager) && fMode == 0) {
    if (fBgFileName == "") {
      cout << "-E- " << GetName() << "::Init: Pileup events needed, but no "
           << " background file name given! " << endl;
      return kERROR;
    }
    fPileupManager = new CbmMvdPileupManager(fBgFileName, fInputBranchName, fBgBufferSize);
    if (fPileupManager->GetNEvents() < 2 * fNPileup) {
      cout << "-E- " << GetName()
           << ": The size of your BG-File is insufficient to perform the "
              "requested pileup"
           << endl;
      cout << "    You need at least events > 2* fNPileup." << endl;
      cout << "    Detected: fPileUp = " << fNPileup << ", events in file " << fPileupManager->GetNEvents() << endl;
      Fatal(GetName(), "The size of your BG-File is insufficient");
      return kERROR;
    }
  }

  // **********   Create delta electron manager if required
  if (fNDeltaElect >= 1 && !(fDeltaManager) && fMode == 0) {
    if (fDeltaFileName == "") {
      cout << "-E- " << GetName() << "::Init: Pileup events needed, but no "
           << " background file name given! " << endl;
      return kERROR;
    }
    fDeltaManager = new CbmMvdPileupManager(fDeltaFileName, fInputBranchName, fDeltaBufferSize);
    if (fDeltaManager->GetNEvents() < 2 * fNDeltaElect) {
      cout << "-E- " << GetName()
           << ": The size of your Delta-File is insufficient to perform the "
              "requested pileup"
           << endl;
      cout << "    You need at least events > 2* fNDeltaElect." << endl;
      cout << "    Detected: fNDeltaElect = " << fNDeltaElect << ", events in file " << fDeltaManager->GetNEvents()
           << endl;
      Fatal(GetName(), "The size of your Delta-File is insufficient");
      return kERROR;
    }
  }
  CbmMvdSensorDigitizerTask* digiTask = new CbmMvdSensorDigitizerTask();

  fDetector->AddPlugin(digiTask);
  fDigiPluginNr = (UInt_t)(fDetector->GetPluginArraySize());
  if (fShowDebugHistos) fDetector->ShowDebugHistos();
  fDetector->Init();

  if (fNoiseSensors) fDetector->SetProduceNoise();

  // Screen output
  cout << GetName() << " initialised with parameters: " << endl;
  //PrintParameters();
  cout << "---------------------------------------------" << endl;


  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdDigitizer::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdDigitizer::Finish()
{
  // cout<< endl << "finishing" << endl;
  fDetector->Finish();
  PrintParameters();
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdDigitizer::Reset() {}
// -------------------------------------------------------------------------


// -----   Reset output arrays   -------------------------------------------
void CbmMvdDigitizer::ResetArrays()
{
  if (fMcPileUp) fMcPileUp->Delete();
  if (fTmpMatch) fTmpMatch->Delete();
  if (fTmpDigi) fTmpDigi->Delete();
  fDigiVect.clear();
  fMatchVect.clear();
}
// -------------------------------------------------------------------------


// -----   Private method GetMvdGeometry   ---------------------------------
void CbmMvdDigitizer::GetMvdGeometry() {}
// -------------------------------------------------------------------------


// -----   Private method PrintParameters   --------------------------------
void CbmMvdDigitizer::PrintParameters()
{

  using namespace std;

  cout.setf(ios_base::fixed, ios_base::floatfield);
  cout << "============================================================" << endl;
  cout << "============== Parameters MvdDigitizer =====================" << endl;
  cout << "============================================================" << endl;
  cout << "=============== End Task ===================================" << endl;
}
// -------------------------------------------------------------------------

// ---------------  Build Event  ------------------------------------------------------
void CbmMvdDigitizer::BuildEvent()
{

  // Some frequently used variables
  CbmMvdPoint* point = nullptr;
  Int_t nOrig        = 0;
  Int_t nPile        = 0;
  Int_t nElec        = 0;

  // ----- First treat standard input file
  nOrig = fInputPoints->GetEntriesFast();


  // ----- Then treat event pileup
  if (fNPileup > 0) {

    // --- Vector of available background events from pile-up.
    // --- Each event is used only once.
    Int_t nBuffer = fPileupManager->GetNEvents();
    vector<Int_t> freeEvents(nBuffer);
    for (Int_t i = 0; i < nBuffer; i++)
      freeEvents[i] = i;

    // --- Loop over pile-up events
    for (Int_t iBg = 0; iBg < fNPileup; iBg++) {

      // Select random event from vector and remove it after usage
      Int_t index = gRandom->Integer(freeEvents.size());

      Int_t iEvent         = freeEvents[index];
      TClonesArray* points = fPileupManager->GetEvent(iEvent);
      freeEvents.erase(freeEvents.begin() + index);

      // Add points from this event to the input arrays
      for (Int_t iPoint = 0; iPoint < points->GetEntriesFast(); iPoint++) {
        point = (CbmMvdPoint*) points->At(iPoint);
        point->SetTrackID(-2);
        nPile++;
        new ((*fInputPoints)[fInputPoints->GetEntriesFast()]) CbmMvdPoint(*((CbmMvdPoint*) points->At(iPoint)));
      }


    }  // Pileup event loop

  }  // Usage of pile-up


  // ----- Finally, treat delta electrons
  if (fNDeltaElect > 0) {

    // --- Vector of available delta events.
    // --- Each event is used only once.
    Int_t nDeltaBuffer = fDeltaManager->GetNEvents();
    vector<Int_t> freeDeltaEvents(nDeltaBuffer);
    for (Int_t i = 0; i < nDeltaBuffer; i++)
      freeDeltaEvents[i] = i;

    // --- Loop over delta events
    for (Int_t it = 0; it < fNDeltaElect; it++) {

      // Select random event from vector and remove it after usage
      Int_t indexD          = gRandom->Integer(freeDeltaEvents.size());
      Int_t iEventD         = freeDeltaEvents[indexD];
      TClonesArray* pointsD = fDeltaManager->GetEvent(iEventD);
      freeDeltaEvents.erase(freeDeltaEvents.begin() + indexD);

      // Add points from this event to the input arrays
      for (Int_t iPoint = 0; iPoint < pointsD->GetEntriesFast(); iPoint++) {
        point = (CbmMvdPoint*) pointsD->At(iPoint);
        point->SetTrackID(-3);  // Mark the points as delta electron
        new ((*fInputPoints)[fInputPoints->GetEntriesFast()]) CbmMvdPoint(*((CbmMvdPoint*) pointsD->At(iPoint)));
        nElec++;
      }


    }  // Delta electron event loop

  }  // Usage of delta

  for (Int_t i = 0; i < fInputPoints->GetEntriesFast(); i++)
    new ((*fMcPileUp)[i]) CbmMvdPoint(*((CbmMvdPoint*) fInputPoints->At(i)));


  // ----- At last: Screen output
  LOG(debug) << "+ " << GetName() << "::BuildEvent: original " << nOrig << ", pileup " << nPile << ", delta " << nElec
             << ", total " << nOrig + nPile + nElec << " MvdPoints";
}
// -----------------------------------------------------------------------------


ClassImp(CbmMvdDigitizer);
