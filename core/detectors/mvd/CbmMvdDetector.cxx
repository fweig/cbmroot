/* Copyright (C) 2014-2021 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                     CbmMvdDetector source file                  -----
// -----                  Created 31/01/11  by M. Deveaux              -----
// -------------------------------------------------------------------------

#include "CbmMvdDetector.h"

#include "CbmMvdDetectorId.h"    // for CbmMvdDetectorId
#include "CbmMvdGeoHandler.h"    // for CbmMvdGeoHandler
#include "CbmMvdHelper.h"        // for CbmMvdSensorTyp
#include "CbmMvdSensor.h"        // for CbmMvdSensor
#include "CbmMvdSensorPlugin.h"  // for CbmMvdSensorPlugin, MvdSensorPluginType
#include "CbmMvdSensorTask.h"    // for CbmMvdSensorTask

#include <Logger.h>  // for LOG, Logger

#include <Rtypes.h>        // for TGenericClassInfo
#include <TClonesArray.h>  // for TClonesArray
#include <TNamed.h>        // for TNamed
#include <TObjArray.h>     // for TObjArray
#include <TRandom.h>       // for TRandom, gRandom
#include <TString.h>       // for TString, operator==, operator<<

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

Int_t CbmMvdDetector::DetectPlugin(Int_t pluginID)
{
  // Detects the position of a plugin with a given Plugin-ID (set in the plugin implementation constructor) in the plugin-array of the sensors


  if (!fSensorArrayFilled) {
    LOG(warning)
      << "CbmMvdDetector::DetectPlugin: You tried to access sensor plugins while the detector is not initialized yet.";
    return -1;
  }
  CbmMvdSensor* sensor   = GetSensor(0);
  TObjArray* pluginArray = sensor->GetPluginArray();

  Int_t nPlugin = pluginArray->GetEntries();
  for (Int_t i = 0; i < nPlugin; i++) {
    CbmMvdSensorPlugin* plugin = (CbmMvdSensorPlugin*) pluginArray->At(i);
    // LOG(info) <<"CbmMvdDetector::DetectPlugin: PlugInID = " << plugin->GetPluginIDNumber() << " Position: "<< i;
    if (pluginID == plugin->GetPluginIDNumber()) { return i; }
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
  fSensorMap[detectorid]->SendInputToPlugin(nPlugin, input);
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
    //LOG(info) << "Send Chain to " << sensor->GetName() << endl;
    sensor->ExecChain();
    //LOG(info) << "finished Chain at "<< sensor->GetName() <<endl<< endl;
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

void CbmMvdDetector::GetOutputArray(Int_t nPlugin, TClonesArray* outputArray)
{
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  TClonesArray* tmpArray;


  for (Int_t i = 0; i < nSensors; i++) {
    sensor       = (CbmMvdSensor*) fSensorArray->At(i);
    tmpArray     = sensor->GetOutputArray(nPlugin);
    Int_t length = tmpArray->GetEntriesFast();
    //LOG(info)<< "CbmMvdDetector::GetOutputArray - Length = " << length;
    if (length >= 0) { outputArray->AbsorbObjects(tmpArray); }
  }
}

//-----------------------------------------------------------------------
void CbmMvdDetector::GetMatchArray(Int_t nPlugin, TClonesArray* matchArray)
{
  Int_t nSensors = fSensorArray->GetEntriesFast();
  CbmMvdSensor* sensor;
  TClonesArray* tmpArray;


  for (Int_t i = 0; i < nSensors; i++) {
    sensor       = (CbmMvdSensor*) fSensorArray->At(i);
    tmpArray     = sensor->GetMatchArray(nPlugin);
    Int_t length = tmpArray->GetEntriesFast();

    if (length >= 0) { matchArray->AbsorbObjects(tmpArray); }
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
