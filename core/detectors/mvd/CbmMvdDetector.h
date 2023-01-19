/* Copyright (C) 2014-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Michael Deveaux, Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                      CbmMvdDetector header file              -----
// -----                  Created 02/12/08  by M. Deveaux             -----
// -------------------------------------------------------------------------


/** CbmMvdDetector.h
 *@author M.Deveaux <deveaux@physik.uni-frankfurt.de>
 **
 ** Singleton holding information on all sensors of the MVD.
 ** User interface to the MVD-Software
 **
 **/


#ifndef CBMMVDDETECTOR_H
#define CBMMVDDETECTOR_H 1

#include "CbmMvdStationPar.h"  // for CbmMvdStationPar

#include <Rtypes.h>            // for ClassDef
#include <RtypesCore.h>        // for Int_t, UInt_t, Bool_t, Float_t, Double_t
#include <TClonesArray.h>      // for TClonesArray
#include <TNamed.h>            // for TNamed
#include <TString.h>           // for TString

#include <map>                 // for map

class CbmMvdSensor;
class CbmMvdSensorDataSheet;
class CbmMvdSensorPlugin;
class TBuffer;
class TClass;
class TMemberInspector;
class TObject;

enum class CbmMvdSensorTyp;

class CbmMvdDetector : public TNamed {

private:
  CbmMvdDetector();
  CbmMvdDetector(const char* name);

public:
  /**
   * static instance
   */
  static CbmMvdDetector* Instance();

  /** Destructor **/
  virtual ~CbmMvdDetector();

  /** Data interface */
  void SendInputToSensorPlugin(Int_t detectorid, Int_t nPlugin, TObject* input);

  void GetOutputArray(Int_t nPlugin,TClonesArray* outputArray);
  void GetMatchArray(Int_t nPlugin, TClonesArray* matchArray);

  std::map<int, CbmMvdSensor*>& GetSensorMap() { return fSensorMap;};
  CbmMvdSensor* GetSensor(Int_t nSensor) {return (CbmMvdSensor*) fSensorArray->At(nSensor);}
  Int_t GetSensorArraySize() { return (fSensorArray->GetEntriesFast()); }

  Int_t GetPluginArraySize() { return fPluginCount - 1; }
  UInt_t GetPluginCount() { return fPluginCount; }
  void SetPluginCount(UInt_t count) { fPluginCount=count; }
  Int_t  DetectPlugin(Int_t pluginID);

  /** Initialisation */
  void AddSensor(TString clearName, TString fullName, TString nodeName, CbmMvdSensorDataSheet* sensorData,
                 Int_t sensorNr, Int_t volumeId, Double_t sensorStartTime, Int_t stationNr);
  static void SetSensorTyp(CbmMvdSensorTyp typ) { fSensorTyp = typ; };
  void SetSensorArrayFilled(Bool_t value = kTRUE) {fSensorArrayFilled=value;}
  void Init();

  void SetMisalignment(Float_t misalignment[3])
  {
    for (Int_t i = 0; i < 3; i++)
      fepsilon[i] = misalignment[i];
  };

  void BuildDebugHistograms() { ; };
  void ShowDebugHistos();

  /** Data Processing */
  void ExecChain();          //Processes the full execution chain
  void Exec(UInt_t nLevel);  //Processes Element nLevel of the chain
  void ExecFrom(UInt_t nLevel);  //Preocesses Elements from a given level till the end

  /** Finish */
  void Finish();

  /** Parameters */
  void SetParameterFile(CbmMvdStationPar* parameter) { fParameter = parameter; };
  CbmMvdStationPar* GetParameterFile() { return fParameter; };
  void PrintParameter() { fParameter->Print(); };


private:
  static CbmMvdSensorTyp fSensorTyp;

  TClonesArray* fSensorArray;
  std::map<int, CbmMvdSensor*> fSensorMap;
  UInt_t fPluginCount;
  TClonesArray* foutput;
  TClonesArray* foutputHits;
  TClonesArray* foutputDigis;
  TClonesArray* foutputCluster;  //khun
  TClonesArray* foutputDigiMatchs;
  TClonesArray* foutputHitMatchs;
  TClonesArray* fcurrentEvent;
  Float_t fepsilon[3];
  /** Data members */

  Int_t fDigiPlugin;
  Int_t fHitPlugin;
  Int_t fClusterPlugin;


  static CbmMvdDetector* fInstance;
  Bool_t fSensorArrayFilled;
  Bool_t initialized;
  Bool_t fFinished;

  TString fName;

  CbmMvdStationPar* fParameter;


  CbmMvdDetector(const CbmMvdDetector&);
  CbmMvdDetector operator=(const CbmMvdDetector&);

  ClassDef(CbmMvdDetector, 3);
};

#endif
