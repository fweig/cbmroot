/* Copyright (C) 2014-2021 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                     CbmMvdDetector source file                  -----
// -----                  Created 31/01/11  by M. Deveaux              -----
// -------------------------------------------------------------------------

#include "CbmMvdDetector.h"

/// includes from CbmRoot
#include "CbmMvdDigi.h"
#include "CbmMvdSensor.h"
#include "plugins/CbmMvdSensorPlugin.h"
#include "plugins/buffers/CbmMvdSensorBuffer.h"
#include "plugins/buffers/CbmMvdSensorFrameBuffer.h"
#include "plugins/buffers/CbmMvdSensorTrackingBuffer.h"
#include "plugins/tasks/CbmMvdSensorClusterfinderTask.h"  //khun
#include "plugins/tasks/CbmMvdSensorDigiToHitTask.h"
#include "plugins/tasks/CbmMvdSensorDigitizerTBTask.h"
//#include "plugins/tasks/CbmMvdSensorDigitizerTask.h"
//#include "plugins/tasks/CbmMvdSensorFindHitTask.h"
#include "CbmDigiManager.h"
#include "plugins/tasks/CbmMvdSensorHitfinderTask.h"  //khun
#include "plugins/tasks/CbmMvdSensorTask.h"
#include "tools/CbmMvdGeoHandler.h"

#include "CbmMvdDetectorId.h"

/// includes from FairRoot
#include <Logger.h>

/// includes from Root
#include "TGeoBBox.h"
#include "TGeoMatrix.h"
#include "TGeoVolume.h"
#include "TString.h"

/// includes from c
#include <iostream>

using std::cout;
using std::endl;

//_____________________________________________________________________________
CbmMvdDetector* CbmMvdDetector::fInstance = 0;

CbmMvdSensorTyp CbmMvdDetector::fSensorTyp = CbmMvdSensorTyp::MIMOSIS;

//_____________________________________________________________________________
CbmMvdDetector* CbmMvdDetector::Instance()
{
  if (fInstance) return fInstance;
  else {
    fInstance                    = new CbmMvdDetector("A");
    CbmMvdGeoHandler* mvdHandler = new CbmMvdGeoHandler();
    mvdHandler->SetSensorTyp(fSensorTyp);
    mvdHandler->Init();
    mvdHandler->Fill();
    mvdHandler->PrintGeoParameter();
    return fInstance;
  }
}

// -----   Default constructor   -------------------------------------------
CbmMvdDetector::CbmMvdDetector()
  : TNamed()
  , fSensorArray(nullptr)
  , fSensorMap()
  , fSensorIDArray(nullptr)
  , fPluginCount(0)
  , foutput(nullptr)
  , foutputHits(nullptr)
  , foutputDigis(nullptr)
  , foutputCluster(nullptr)
  ,  //khun
  foutputDigiMatchs(nullptr)
  , foutputHitMatchs(nullptr)
  , fcurrentEvent(nullptr)
  , fepsilon()
  , fDigiPlugin(-1)
  , fHitPlugin(-1)
  , fClusterPlugin(-1)
  , fSensorArrayFilled(kFALSE)
  , initialized(kFALSE)
  , fFinished(kFALSE)
  , fName("")
  , fParameter(nullptr)
{

  Fatal(GetName(), " - Do not use standard constructor");
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMvdDetector::CbmMvdDetector(const char* name)
  : TNamed()
  , fSensorArray(new TClonesArray("CbmMvdSensor", 10))
  , fSensorMap()
  , fSensorIDArray(nullptr)
  , fPluginCount(0)
  , foutput(nullptr)
  , foutputHits(nullptr)
  , foutputDigis(nullptr)
  , foutputCluster(nullptr)
  ,  //khun
  foutputDigiMatchs(nullptr)
  , foutputHitMatchs(nullptr)
  , fcurrentEvent(nullptr)
  , fepsilon()
  , fDigiPlugin(-1)
  , fHitPlugin(-1)
  , fClusterPlugin(-1)
  , fSensorArrayFilled(kFALSE)
  , initialized(kFALSE)
  , fFinished(kFALSE)
  , fName(name)
  , fParameter(nullptr)
{

  if (fInstance) { Fatal(GetName(), " - Error, singleton does already exist."); }
  else {
    fInstance = this;
  };
  fepsilon[0] = fepsilon[1] = fepsilon[2] = 0;
  fName                                   = name;
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMvdDetector::~CbmMvdDetector() {}
//-----------------------------------------------------------------------

// -------Setters -----------------------------------------------------

void CbmMvdDetector::AddSensor(TString clearName, TString fullName, TString nodeName, CbmMvdSensorDataSheet* sensorData,
                               Int_t sensorNr, Int_t volumeId, Double_t sensorStartTime, Int_t stationNr)
{
  /**
   *
   * new sensor is registered in sensor array
   *
   * **/

  TString myname;

  if (fSensorArrayFilled) { Fatal(GetName(), " - Error, must add all sensors before adding plugins."); }


  Int_t nSensors = fSensorArray->GetEntriesFast();

  myname = clearName;
  myname += nSensors;

  new ((*fSensorArray)[nSensors])
    CbmMvdSensor(myname, sensorData, fullName, nodeName, sensorNr, volumeId, sensorStartTime);
  //CbmMvdSensor(const char* name, CbmMvdSensorDataSheet* dataSheet, TString volName,
  //TString nodeName, Int_t stationNr, Int_t volumeId, Double_t sensorStartTime);

  CbmMvdSensor* sensor = (CbmMvdSensor*) fSensorArray->At(nSensors);
  sensor->SetDataSheet(sensorData);
  sensor->SetStation(stationNr);

  // calculate the detectorId from the running sensor number
  CbmMvdDetectorId tmp;
  int detectorId = tmp.DetectorId(sensorNr);

  // Add sensor to SensorMap
  fSensorMap[detectorId] = sensor;

  Float_t misalignment[3], randArray[3];
  //    TRandom3* rand = new TRandom3(0);
  gRandom->RndmArray(3, randArray);
  misalignment[0] = ((2 * randArray[0]) - 1) * fepsilon[0];
  misalignment[1] = ((2 * randArray[0]) - 1) * fepsilon[1];
  misalignment[2] = ((2 * randArray[0]) - 1) * fepsilon[2];
  sensor->SetMisalignment(misalignment);
  LOG(debug1) << "new sensor " << myname << " to detector added at station: " << stationNr;
}

// ----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CbmMvdDetector::AddPlugin(CbmMvdSensorPlugin* plugin)
{
  /**
 * if there is a new buffer or task typ you have to insert it here
 * or you can't use it.
 */
  fSensorArrayFilled = kTRUE;

  CbmMvdSensor* sensor;
  Int_t nSensors                = fSensorArray->GetEntriesFast();
  const TString digitizername   = "CbmMvdSensorDigitizerTask";
  const TString digitizerTBname = "CbmMvdSensorDigitizerTBTask";
  //  const TString findername      = "CbmMvdSensorFindHitTask";
  //const TString framename = "CbmMvdSensorFrameBuffer";
  //const TString trackingname = "CbmMvdSensorTrackingBuffer";
  const TString clustername   = "CbmMvdSensorClusterfinderTask";  //khun
  const TString hitname       = "CbmMvdSensorHitfinderTask";      //khun
  const TString digitohitname = "CbmMvdSensorDigiToHitTask";

  for (Int_t i = 0; i < nSensors; i++) {


    if (plugin->GetPluginType() == task) {

      if (plugin->ClassName() == digitizername) {
        LOG(fatal) << "Should never come here. Tasks are now added in CbmMvdDigitizer class";
/*
        CbmMvdSensorDigitizerTask* digiTask = new CbmMvdSensorDigitizerTask();
        sensor                              = (CbmMvdSensor*) fSensorArray->At(i);
        sensor->AddPlugin(digiTask);
        sensor->SetDigiPlugin(fPluginCount);

        //cout <<  "Adding Task CbmMvdSensorDigitizerTask at Sensor " << sensor->GetName() << endl;
*/
      }
      else if (plugin->ClassName() == digitizerTBname) {
        CbmMvdSensorDigitizerTBTask* digiTask = new CbmMvdSensorDigitizerTBTask();
        sensor                                = (CbmMvdSensor*) fSensorArray->At(i);
        sensor->AddPlugin(digiTask);
        sensor->SetDigiPlugin(fPluginCount);

        //cout <<  "Adding Task CbmMvdSensorDigitizerTask at Sensor " << sensor->GetName() << endl;
        /*      } else if (plugin->ClassName() == findername) {
        CbmMvdSensorFindHitTask* findTask = new CbmMvdSensorFindHitTask();
        sensor                            = (CbmMvdSensor*) fSensorArray->At(i);
        sensor->AddPlugin(findTask);
        sensor->SetHitPlugin(fPluginCount);
        // cout <<  "Adding Task CbmMvdSensorFindHitTask at Sensor " << sensor->GetName() << endl;
*/
      }

      //Re-enable cluster and hit finder in addition to khun
      else if (plugin->ClassName() == clustername) {
        CbmMvdSensorClusterfinderTask* clusterTask = new CbmMvdSensorClusterfinderTask();
        sensor                                     = (CbmMvdSensor*) fSensorArray->At(i);
        sensor->AddPlugin(clusterTask);
        sensor->SetClusterPlugin(fPluginCount);
        // cout <<  "Adding Task CbmMvdSensorClusterfinderTask at Sensor " << sensor->GetName() << endl;
      }
      else if (plugin->ClassName() == hitname) {
        CbmMvdSensorHitfinderTask* hitTask = new CbmMvdSensorHitfinderTask();
        sensor                             = (CbmMvdSensor*) fSensorArray->At(i);
        sensor->AddPlugin(hitTask);
        sensor->SetHitPlugin(fPluginCount);
        // cout <<  "Adding Task CbmMvdSensorHitfinderTask at Sensor " << sensor->GetName() << endl;
      }
      //end: re-enable cluster and hit finder in addition to khun

      else if (plugin->ClassName() == digitohitname) {
        CbmMvdSensorDigiToHitTask* digitohitTask = new CbmMvdSensorDigiToHitTask();
        sensor                                   = (CbmMvdSensor*) fSensorArray->At(i);
        sensor->AddPlugin(digitohitTask);
        sensor->SetHitPlugin(fPluginCount);
        // cout <<  "Adding Task CbmMvdSensorClusterfinderTask at Sensor " << sensor->GetName() << endl;
      }
      else {
        cout << endl << "task not included yet, adding standart task." << endl;
        CbmMvdSensorTask* task = new CbmMvdSensorTask();
        sensor                 = (CbmMvdSensor*) fSensorArray->At(i);
        sensor->AddPlugin(task);
      }
      //data parallelizm requires that each sensor get its own task object
    }

    else {
      cout << "Invalide" << endl;
    }
  };
  fPluginCount++;
};

//----------------------------------------------------------------------

Int_t CbmMvdDetector::DetectPlugin(Int_t pluginID)
{
  // Detects the position of a plugin with a given Plugin-ID (set in the plugin implementation constructor) in the plugin-array of the sensors

  Int_t nDigitizerPlugin=-1;

  if (!fSensorArrayFilled) {cout << "-W - CbmMvdDetector::DetectPlugin: You tried to access sensor plugins while the detector is not initialized yet." << endl;  return -1;}
  CbmMvdSensor* sensor=GetSensor(0);
  TObjArray* pluginArray= sensor->GetPluginArray();

  Int_t nPlugin=pluginArray->GetEntries();
  for(Int_t i=0; i<nPlugin;i++) {
    CbmMvdSensorPlugin* plugin= (CbmMvdSensorPlugin*) pluginArray->At(i);
    // cout << "- I - CbmMvdDetector::DetectPlugin: PlugInID = " << plugin->GetPluginIDNumber() << " Position: "<< i << endl;
    if (pluginID==plugin->GetPluginIDNumber()){return i;}
  }


 return -1;
}

//-----------------------------------------------------------------------
void CbmMvdDetector::Init()
{

  /**
   *
   * Initialisation method
   *
   * **/


  if (!initialized) {
    foutput           = new TClonesArray("CbmMvdPoint", 1000);
    fcurrentEvent     = new TClonesArray("CbmMvdPoint", 1000);
    foutputDigis      = new TClonesArray("CbmMvdDigi", 1000);
    foutputDigiMatchs = new TClonesArray("CbmMatch", 1000);
    foutputHits       = new TClonesArray("CbmMvdHit", 1000);
    foutputCluster    = new TClonesArray("CbmMvdCluster", 1000);  //khun
  }

  Int_t nSensors = fSensorArray->GetEntriesFast();
  if (nSensors <= 0) LOG(fatal) << "CbmMvdDetector could not load Sensors from Geometry!";
  CbmMvdSensor* sensor;

  for (Int_t j = 0; j < nSensors; j++) {

    sensor = (CbmMvdSensor*) fSensorArray->At(j);
    LOG(debug1) << "Init Sensor " << sensor->GetName();
    sensor->Init();
  }

  initialized = kTRUE;
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void CbmMvdDetector::ShowDebugHistos()
{

  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;

  for (Int_t j = 0; j < nSensors; j++) {
    sensor = (CbmMvdSensor*) fSensorArray->At(j);
    sensor->ShowDebugHistos();
  }
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void CbmMvdDetector::SetProduceNoise()
{
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;

  for (Int_t j = 0; j < nSensors; j++) {
    sensor = (CbmMvdSensor*) fSensorArray->At(j);
    sensor->SetProduceNoise();
  }
}
//-----------------------------------------------------------------------
void CbmMvdDetector::SendInputToSensorPlugin(Int_t detectorid, Int_t nPlugin, TObject* input)
{
  fSensorMap[detectorid]->SendInputToPlugin(nPlugin,input);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
/*
void CbmMvdDetector::SendInputToSensorPlugin(Int_t nSensor, Int_t nPlugin, TObject* input)
{
  CbmMvdSensor* sensor=(CbmMvdSensor*)fSensorArray->At(nSensor);
  sensor->SendInputToPlugin(nPlugin,input);
}
*/
//-----------------------------------------------------------------------

/*
void CbmMvdDetector::SendInput(TClonesArray* input)
{


  CbmMvdPoint* point;
  Int_t nEntries = input->GetEntriesFast();
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  Bool_t send = kFALSE;
  for (Int_t i = 0; i < nEntries; i++) {
    point = (CbmMvdPoint*) input->At(i);
    point->SetPointId(i);
    for (Int_t k = 0; k < nSensors; k++) {
      sensor = (CbmMvdSensor*) fSensorArray->At(k);

      if (point->GetDetectorID() == sensor->GetDetectorID()) {
        sensor->SendInputToPlugin(sensor->GetDigiPlugin(),(TObject*) point);
        send = true;
      }
    }
    if (!send) LOG(warn) << "Point not send to any sensor: " << point->GetDetectorID();
  }
}
*/
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void CbmMvdDetector::SendInputNeutral(TClonesArray* input)
{
  /**
   *
   * Sending event to sensors,
   * each sensor gets only his own points
   *
   *
   *
   * **/
/*

  TObject* point;
  Int_t nEntries = input->GetEntriesFast();
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  Bool_t send = kFALSE;
  for (Int_t i = 0; i < nEntries; i++) {
    point = (CbmMvdPoint*) input->At(i);
    point->SetPointId(i);
    for (Int_t k = 0; k < nSensors; k++) {
      sensor = (CbmMvdSensor*) fSensorArray->At(k);

      if (point->GetDetectorID() == sensor->GetDetectorID()) {
        sensor->SendInput(point);
        send = true;
      }
    }
    if (!send) LOG(warn) << "Point not send to any sensor: " << point->GetDetectorID();
  }

*/
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void CbmMvdDetector::SendInputDigis(CbmDigiManager* digiMan)
{
  /**
   *
   * Sending event to sensors,
   * each sensor gets only his own points
   *
   *
   *
   * **/
  assert(digiMan);
  assert(digiMan->IsPresent(ECbmModuleId::kMvd));


  CbmMvdDigi* digi;

  Int_t nSensors = fSensorArray->GetEntriesFast();
  Int_t nEntries = digiMan->GetNofDigis(ECbmModuleId::kMvd);
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nEntries; i++) {
    digi = new CbmMvdDigi(*(digiMan->Get<CbmMvdDigi>(i)));
    digi->SetRefId(i);
    for (Int_t k = 0; k < nSensors; k++) {
      sensor = (CbmMvdSensor*) fSensorArray->At(k);

      if (digi->GetDetectorId() == sensor->GetDetectorID()) { sensor->SendInputDigi(digi); }
    }
  }
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CbmMvdDetector::SendInputDigis(TClonesArray* digis)
{
  /**
   *
   * Sending event to sensors,
   * each sensor gets only his own points
   *
   *
   *
   * **/


  CbmMvdDigi* digi;

  Int_t nSensors = fSensorArray->GetEntriesFast();
  Int_t nEntries = digis->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nEntries; i++) {
    digi = (CbmMvdDigi*) digis->At(i);
    digi->SetRefId(i);
    for (Int_t k = 0; k < nSensors; k++) {
      sensor = (CbmMvdSensor*) fSensorArray->At(k);

      if (digi->GetDetectorId() == sensor->GetDetectorID()) { sensor->SendInputDigi(digi); }
    }
  }
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void CbmMvdDetector::SendInputDigisToHits(TClonesArray* digis)
{
  /**
   *
   * Sending event to sensors,
   * each sensor gets only his own points
   *
   * Version for DigiToHit
   *
   * **/


  CbmMvdDigi* digi;
  Int_t nEntries = digis->GetEntriesFast();
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nEntries; i++) {
    digi = (CbmMvdDigi*) digis->At(i);
    digi->SetRefId(i);
    for (Int_t k = 0; k < nSensors; k++) {
      sensor = (CbmMvdSensor*) fSensorArray->At(k);

      if (digi->GetDetectorId() == sensor->GetDetectorID()) { sensor->SendInputDigiToHit(digi); }
    }
  }
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CbmMvdDetector::SendInputCluster(TClonesArray* clusters)
{

  /**
   *
   * Sending event to sensors,
   * each sensor gets only his own points
   *
   *
   *
   * **/
  CbmMvdCluster* cluster;
  Int_t nEntries = clusters->GetEntriesFast();
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nEntries; i++) {
    cluster = (CbmMvdCluster*) clusters->At(i);
    cluster->SetRefId(i);
    for (Int_t k = 0; k < nSensors; k++) {
      sensor = (CbmMvdSensor*) fSensorArray->At(k);

      if (cluster->GetSensorNr() == sensor->GetSensorNr()) {
        LOG(debug2) << "Sending cluster to sensor: " << sensor->GetSensorNr();
        sensor->SendInputCluster(cluster);
      }
    }
  }
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void CbmMvdDetector::ExecChain()
{
  /**
   *
   * method to execute plugin chain on sensors
   *
   * **/

  foutput->Clear();
  fcurrentEvent->Clear();
  foutputDigis->Clear();
  foutputDigiMatchs->Clear();
  foutputHits->Clear();
  foutputCluster->Clear();  //khun

  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nSensors; i++) {
    sensor = (CbmMvdSensor*) fSensorArray->At(i);
    //cout << "I------ Send Chain to " << sensor->GetName() << endl;
    sensor->ExecChain();
    //cout << "I------ finished Chain at "<< sensor->GetName() <<endl<< endl;
  };
}

//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CbmMvdDetector::Exec(UInt_t nLevel)
{
  /**
   *
   * execute spezific plugin on all sensors
   *
   * **/

  foutput->Clear();
  fcurrentEvent->Clear();
  foutputDigis->Clear();
  foutputDigiMatchs->Clear();
  foutputHits->Clear();
  foutputCluster->Clear();  //khun
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nSensors; i++) {
    sensor = (CbmMvdSensor*) fSensorArray->At(i);
    sensor->Exec(nLevel);
  }
}

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

void CbmMvdDetector::ExecFrom(UInt_t nLevel)
{

  /**
   *
   * execute chain from a spezific plugin on all sensors
   *
   * **/
  foutput->Clear();
  fcurrentEvent->Clear();
  foutputDigis->Clear();
  foutputDigiMatchs->Clear();
  foutputHits->Clear();
  foutputCluster->Clear();  //khun

  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nSensors; i++) {
    sensor = (CbmMvdSensor*) fSensorArray->At(i);
    sensor->ExecFrom(nLevel);
  }
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
TClonesArray* CbmMvdDetector::GetCurrentEvent()
{

  /**
   * Method used for debugging, Plugins have to hold there output until next call
   */
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nSensors; i++) {
    sensor = (CbmMvdSensor*) fSensorArray->At(i);
    // foutput = sensor->GetOutputArray(0);
    fcurrentEvent->AbsorbObjects(sensor->GetOutputArray(0));
  }

  return (fcurrentEvent);
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

TClonesArray* CbmMvdDetector::GetOutputHits()
{

  /**
   * method used to write hits to hd
   */

  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nSensors; i++) {
    sensor       = (CbmMvdSensor*) fSensorArray->At(i);
    Int_t length = sensor->GetOutputArrayLen(sensor->GetHitPlugin());
    if (length >= 0) {
      foutputHits->AbsorbObjects(sensor->GetOutputBuffer(), 0, sensor->GetOutputBuffer()->GetEntriesFast() - 1);
    }
  }

  return (foutputHits);
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
TClonesArray* CbmMvdDetector::GetOutputDigis()
{

  /**
   * method used to write digis to hd
   */

  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nSensors; i++) {
    sensor       = (CbmMvdSensor*) fSensorArray->At(i);
    fDigiPlugin  = sensor->GetDigiPlugin();
    Int_t length = sensor->GetOutputArrayLen(fDigiPlugin);
    if (length >= 0) { foutputDigis->AbsorbObjects(sensor->GetOutputArray(fDigiPlugin)); }
  }

  return (foutputDigis);
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
TClonesArray* CbmMvdDetector::GetOutputDigiMatchs()
{

  /**
   * method used to write digiMatches to hd
   */
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nSensors; i++) {
    sensor      = (CbmMvdSensor*) fSensorArray->At(i);
    fDigiPlugin = sensor->GetDigiPlugin();
    //    Int_t length = sensor->GetOutputArrayLen(fDigiPlugin);
    foutputDigiMatchs->AbsorbObjects(sensor->GetOutputMatch());
  }
  return (foutputDigiMatchs);
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// khun /*
TClonesArray* CbmMvdDetector::GetOutputCluster()
{
  /**
   * method used to write Cluster to hd
   */
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nSensors; i++) {
    sensor         = (CbmMvdSensor*) fSensorArray->At(i);
    fClusterPlugin = sensor->GetClusterPlugin();
    Int_t length   = sensor->GetOutputArrayLen(fClusterPlugin);
    if (length >= 0) foutputCluster->AbsorbObjects(sensor->GetOutputArray(fClusterPlugin), 0, length);  //khun
  }

  return (foutputCluster);
};
// khun */

//-----------------------------------------------------------------------


//-----------------------------------------------------------------------

TClonesArray* CbmMvdDetector::GetOutputArray(Int_t nPlugin)
{

  /**
   * method used to write processed events to hd
   */

  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  for (Int_t i = 0; i < nSensors; i++) {
    sensor       = (CbmMvdSensor*) fSensorArray->At(i);
    Int_t length = sensor->GetOutputArrayLen(nPlugin);
    if (length >= 0) {
      foutputDigis->AbsorbObjects(sensor->GetOutputArray(nPlugin), 0, length);
      foutputDigiMatchs->AbsorbObjects(sensor->GetOutputMatch(), 0, length);
    }
  }
  return (foutputDigis);
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void CbmMvdDetector::Finish()
{
  if (!fFinished) {
    Int_t nSensors = fSensorArray->GetEntriesFast();
    CbmMvdSensor* sensor;
    for (Int_t i = 0; i < nSensors; i++) {
      sensor = (CbmMvdSensor*) fSensorArray->At(i);
      sensor->Finish();
    }
    fFinished = kTRUE;
  }
}
//-----------------------------------------------------------------------

ClassImp(CbmMvdDetector)
