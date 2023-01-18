/* Copyright (C) 2014-2021 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                     CbmMvdDetector source file                  -----
// -----                  Created 31/01/11  by M. Deveaux              -----
// -------------------------------------------------------------------------

#include "CbmMvdDetector.h"

/// includes from CbmRoot
#include "CbmDigiManager.h"
#include "CbmMvdDetectorId.h"
#include "CbmMvdDigi.h"
#include "CbmMvdSensor.h"
#include "plugins/CbmMvdSensorPlugin.h"
#include "plugins/tasks/CbmMvdSensorTask.h"
#include "tools/CbmMvdGeoHandler.h"





/// includes from FairRoot
#include <Logger.h>

/// includes from Root
#include "TGeoBBox.h"
#include "TGeoMatrix.h"
#include "TGeoVolume.h"
#include "TRandom.h"
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
  const TString digitizername   = "CbmMvdSensorDigitizerTask";    //done
  const TString digitizerTBname = "CbmMvdSensorDigitizerTBTask";
  //  const TString findername      = "CbmMvdSensorFindHitTask";
  //const TString framename = "CbmMvdSensorFrameBuffer";
  //const TString trackingname = "CbmMvdSensorTrackingBuffer";
  const TString clustername   = "CbmMvdSensorClusterfinderTask";  //khun //done
  const TString hitname       = "CbmMvdSensorHitfinderTask";      //khun
  const TString digitohitname = "CbmMvdSensorDigiToHitTask";

  for (Int_t i = 0; i < nSensors; i++) {


    if (plugin->GetPluginType() == task) {

      if (plugin->ClassName() == digitizername) {
        LOG(fatal) << "Should never come here. Tasks are now added in CbmMvdDigitizer class";
      }
      else if (plugin->ClassName() == digitizerTBname) {
        LOG(fatal) << "Should never come here. Tasks are now added in CbmMvdDigitizerTB class";
      }

      //Re-enable cluster and hit finder in addition to khun
      else if (plugin->ClassName() == clustername) {
        LOG(fatal) << "Should never come here. Tasks are now added in CbmMvdClusterfinder class";
      }
      else if (plugin->ClassName() == hitname) {
        LOG(fatal) << "Should never come here. Tasks are now added in CbmMvdHitfinder class";
      }
      //end: re-enable cluster and hit finder in addition to khun

      else if (plugin->ClassName() == digitohitname) {
        LOG(fatal) << "Should never come here. Tasks are now added in CbmMvdDigiToHit class";
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
void CbmMvdDetector::SendInputToSensorPlugin(Int_t detectorid, Int_t nPlugin, TObject* input)
{
  fSensorMap[detectorid]->SendInputToPlugin(nPlugin,input);
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

void CbmMvdDetector::GetOutputArray(Int_t nPlugin, TClonesArray* outputArray){
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  TClonesArray* tmpArray;


  for (Int_t i = 0; i < nSensors; i++) {
    sensor       = (CbmMvdSensor*) fSensorArray->At(i);
    tmpArray=sensor->GetOutputArray(nPlugin);
    Int_t length = tmpArray->GetEntriesFast();
    //cout<< "CbmMvdDetector::GetOutputArray - Length = " << length << endl;
    if (length >= 0) {
      outputArray->AbsorbObjects(tmpArray);
    }
  }
}

//-----------------------------------------------------------------------
void CbmMvdDetector::GetMatchArray(Int_t nPlugin, TClonesArray* matchArray){
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  TClonesArray* tmpArray;


  for (Int_t i = 0; i < nSensors; i++) {
    sensor       = (CbmMvdSensor*) fSensorArray->At(i);
    tmpArray=sensor->GetMatchArray(nPlugin);
    Int_t length = tmpArray->GetEntriesFast();

    if (length >= 0) {
      matchArray->AbsorbObjects(tmpArray);
    }
  }
}

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
