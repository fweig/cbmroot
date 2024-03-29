/* Copyright (C) 2014-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer], Florian Uhlig, Volker Friese */

// -------------------------------------------------------------------------
// -----                    CbmMvdDigitizer source file                -----
// -------------------------------------------------------------------------
#include "CbmMvdDigitizer.h"

#include "CbmMatch.h"                   // for CbmMatch
#include "CbmMvdDetector.h"             // for CbmMvdDetector
#include "CbmMvdDigi.h"                 // for CbmMvdDigi
#include "CbmMvdPileupManager.h"        // for CbmMvdPileupManager
#include "CbmMvdPoint.h"                // for CbmMvdPoint
#include "CbmMvdSensor.h"               // for CbmMvdSensor
#include "CbmMvdSensorDigitizerTask.h"  // for CbmMvdSensorDigitizerTask

#include <FairRootManager.h>  // for FairRootManager
#include <Logger.h>           // for Logger, LOG

#include <TClonesArray.h>  // for TClonesArray
#include <TObject.h>       // for TObject
#include <TRandom.h>       // for TRandom
#include <TRandom3.h>      // for gRandom
#include <TStopwatch.h>    // for TStopwatch

#include <cassert>   // for assert
#include <iomanip>   // for setprecision, setw, __iom_t5
#include <iostream>  // for operator<<, basic_ostream, endl
#include <map>       // for __map_iterator, map, operator!=
#include <string>    // for allocator, char_traits
#include <vector>    // for vector

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
  fTmpDigi  = new TClonesArray("CbmMvdDigi", 1000);
  fTmpMatch = new TClonesArray("CbmMatch", 1000);
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

  fTmpDigi  = new TClonesArray("CbmMvdDigi", 1000);
  fTmpMatch = new TClonesArray("CbmMatch", 1000);
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

  // Add points from BG and Delta files to the array if needed
  BuildEvent();

  Int_t nPoints      = fInputPoints->GetEntriesFast();
  Int_t nDigis       = 0;
  CbmMvdPoint* point = 0;
  fTmpDigi->Clear();
  fTmpMatch->Clear();


  if (fInputPoints->GetEntriesFast() > 0) {
    LOG(debug) << "//----------------------------------------//";
    LOG(debug) << fName << ": Send Input";


    Int_t nTargetPlugin = DetectPlugin(100);

    // Distribute the points from the input array to the sensors
    for (Int_t i = 0; i < nPoints; i++) {
      point = (CbmMvdPoint*) fInputPoints->At(i);
      fDetector->SendInputToSensorPlugin(point->GetDetectorID(), nTargetPlugin, static_cast<TObject*>(point));
    }

    // Execute the plugged digitizer plugin for all sensors
    LOG(debug) << fName << ": Execute DigitizerPlugin Nr. " << fDigiPluginNr;
    fDetector->Exec(fDigiPluginNr);
    LOG(debug) << fName << ": End Chain";


    // --- Send produced digis to DAQ
    //fTmpDigi  = fDetector->GetOutputDigis();
    //fTmpMatch = fDetector->GetOutputDigiMatchs();

    fDetector->GetOutputArray(nTargetPlugin, fTmpDigi);
    //LOG(debug) << "CbmMvdDigitizer::Exec() - GetOutputArray completed";

    fDetector->GetMatchArray(nTargetPlugin, fTmpMatch);
    //LOG(debug) << "CbmMvdDigitizer::Exec() - GetMatchArray data completed";

    //LOG(debug) << "Length of Digi Array " << fTmpDigi->GetEntriesFast();
    //LOG(debug) << "Length of Digi Match Array " << fTmpMatch->GetEntriesFast();

    Int_t nEntries = fTmpDigi->GetEntriesFast();
    for (Int_t index = 0; index < nEntries; index++) {

      CbmMvdDigi* digi  = dynamic_cast<CbmMvdDigi*>(fTmpDigi->At(index));
      CbmMvdDigi* digi1 = new CbmMvdDigi(*digi);
      assert(digi1);
      fDigiVect.push_back(digi1);
      //      // Create new digi in the output digi outputfrom the one in the internal TClonesArray
      //      fDigiVect.emplace_back(new CbmMvdDigi(*(static_cast<CbmMvdDigi*>(fTmpDigi->At(index)))));

      //     CbmMatch match{*(dynamic_cast<CbmMatch*>(fTmpMatch->At(index)))};
      //     CbmMatch* match1 = new CbmMatch(match);
      CbmMatch* match  = dynamic_cast<CbmMatch*>(fTmpMatch->At(index));
      CbmMatch* match1 = new CbmMatch(*match);
      fMatchVect.push_back(match1);
      //      // Create new match in the output match vector from the one in the internal TClonesArray
      //      fMatchVect.emplace_back(new CbmMatch(*(static_cast<CbmMatch*>(fTmpMatch->At(index)))));

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
    //LOG(dfVerbose)  << "Start writing DigiMatchs";
    //fDigiMatch->AbsorbObjects(fDetector->GetOutputDigiMatchs(),0,fDetector->GetOutputDigiMatchs()->GetEntriesFast()-1);
    //if(fVerbose) LOG(debug) << "Total of " << fDigiMatch->GetEntriesFast() << " digisMatch in this Event";
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
  LOG(info) << GetName() << ": Initialisation...";

  // **********  RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    LOG(fatal) << "No FairRootManager!";
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
      LOG(error) << "Pileup events needed, but no background file name given! ";
      return kERROR;
    }
    fPileupManager = new CbmMvdPileupManager(fBgFileName, fInputBranchName, fBgBufferSize);
    if (fPileupManager->GetNEvents() < 2 * fNPileup) {
      LOG(error) << "The size of your BG-File is insufficient to perform the requested pileup";
      LOG(error) << " You need at least events > 2* fNPileup but there are only" << fNPileup << ", events in file "
                 << fPileupManager->GetNEvents();
      LOG(fatal) << "The size of your BG-File is insufficient";
      return kFATAL;
    }
  }

  // **********   Create delta electron manager if required
  if (fNDeltaElect >= 1 && !(fDeltaManager) && fMode == 0) {
    if (fDeltaFileName == "") {
      LOG(error) << "Delta events needed, but no delta elector file name given! ";
      return kERROR;
    }
    fDeltaManager = new CbmMvdPileupManager(fDeltaFileName, fInputBranchName, fDeltaBufferSize);
    if (fDeltaManager->GetNEvents() < 2 * fNDeltaElect) {
      LOG(error) << "The size of your Delta-File is insufficient to perform the requested pileup";
      LOG(error) << " You need at least events > 2* fNDeltaElect but there are only" << fNDeltaElect
                 << ", events in file " << fDeltaManager->GetNEvents();
      LOG(fatal) << "The size of your Delta-File is insufficient";
      return kFATAL;
    }
  }


  // Add the digitizer plugin to all sensors
  std::map<int, CbmMvdSensor*>& sensorMap = fDetector->GetSensorMap();
  UInt_t plugincount                      = fDetector->GetPluginCount();

  for (auto itr = sensorMap.begin(); itr != sensorMap.end(); itr++) {
    CbmMvdSensorDigitizerTask* digiTask = new CbmMvdSensorDigitizerTask();
    if (fNoiseSensors) digiTask->SetProduceNoise();
    itr->second->AddPlugin(digiTask);
    itr->second->SetDigiPlugin(plugincount);
  }
  fDetector->SetSensorArrayFilled(kTRUE);
  fDetector->SetPluginCount(plugincount + 1);
  fDigiPluginNr = (UInt_t)(fDetector->GetPluginArraySize());

  if (fShowDebugHistos) fDetector->ShowDebugHistos();
  fDetector->Init();

  // --- Read list of inactive channels
  if (!fInactiveChannelFileName.IsNull()) {
    LOG(info) << GetName() << ": Reading inactive channels from " << fInactiveChannelFileName;
    auto result = ReadInactiveChannels();
    if (!result.second) {
      LOG(error) << GetName() << ": Error in reading from file! Task will be inactive.";
      return kFATAL;
    }
    LOG(info) << GetName() << ": " << std::get<0>(result) << " lines read from file, " << fInactiveChannels.size()
              << " channels set inactive";
  }

  // Screen output
  LOG(info) << GetName() << " initialised with parameters: ";
  //PrintParameters();


  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdDigitizer::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdDigitizer::Finish()
{  //Int_t i = DetectPlugin (100);
  //LOG(debug) << "CbmMvdDigitizer::Finish() Autodetect: " << i <<" Manual Detect " << fDigiPluginNr;
  // LOG(debug) << "finishing";
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

Int_t CbmMvdDigitizer::DetectPlugin(Int_t pluginID)
{

  CbmMvdDetector* detector = CbmMvdDetector::Instance();
  return detector->DetectPlugin(pluginID);
}

void CbmMvdDigitizer::PrintParameters() const { LOG(info) << ParametersToString(); }

// -----   Private method PrintParameters   --------------------------------

std::string CbmMvdDigitizer::ParametersToString() const
{
  std::stringstream ss;
  ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
  ss << "============================================================" << endl;
  ss << "============== Parameters MvdDigitizer =====================" << endl;
  ss << "============================================================" << endl;
  ss << "=============== End Task ===================================" << endl;
  return ss.str();
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
