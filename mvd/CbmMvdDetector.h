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


/// includes from c
//#include <vector>

/// includes from ROOT
#include "TClonesArray.h"
#include "TNamed.h"

/// includes from CbmRoot
#include "CbmMvdStationPar.h"
//#include "CbmMvdDigi.h"
#include "tools/CbmMvdHelper.h"

class CbmMvdPoint;
class CbmMvdSensorDataSheet;
class CbmMvdSensorPlugin;
class CbmMvdSensor;

class CbmDigiManager;

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
  void SendInput(TClonesArray* input);
  void SendInputDigis(CbmDigiManager* digiMan);
  void SendInputDigis(TClonesArray* digis);
  void SendInputDigisToHits(TClonesArray* digis);
  void SendInputCluster(TClonesArray* cluster);
  // void SendClonesArray(TClonesArray* addedStructures, Int_t dataLevel=0){;}

  TClonesArray* GetOuput() { return 0; }
  TClonesArray* GetOutputHits();
  TClonesArray* GetOutputDigis();
  TClonesArray* GetOutputDigiMatchs();
  TClonesArray* GetOutputCluster();  //khun
  TClonesArray* GetOutputArray(Int_t nPlugin);
  // TClonesArray* GetClonesArray(Int_t dataLevel){ return 0;}
  Int_t GetSensorArraySize() { return (fSensorArray->GetEntriesFast()); }
  UInt_t GetPluginArraySize() { return fPluginCount - 1; }


  TClonesArray* GetCurrentEvent();
  /** Initialisation */

  void AddSensor(TString clearName,
                 TString fullName,
                 TString nodeName,
                 CbmMvdSensorDataSheet* sensorData,
                 Int_t sensorNr,
                 Int_t volumeId,
                 Double_t sensorStartTime,
                 Int_t stationNr);
  void AddPlugin(CbmMvdSensorPlugin* plugin);
  void BuildDebugHistograms() { ; };
  void Init();
  void SetMisalignment(Float_t misalignment[3]) {
    for (Int_t i = 0; i < 3; i++)
      fepsilon[i] = misalignment[i];
  };
  void SetParameterFile(CbmMvdStationPar* parameter) {
    fParameter = parameter;
  };
  void ShowDebugHistos();
  /** Data Processing */


  void ExecChain();          //Processes the full execution chain
  void Exec(UInt_t nLevel);  //Processes Element nLevel of the chain
  // void ExecTo(UInt_t nLevel){;}; // Processes Elements to a given Level of Plugins
  void ExecFrom(
    UInt_t nLevel);  //Preocesses Elements from a given level till the end

  /** Finish */

  void Finish();
  // void StoreDebugHistograms(TString fileName){;}

  /** Accessors */
  CbmMvdStationPar* GetParameterFile() { return fParameter; };
  CbmMvdSensor* GetSensor(UInt_t nSensor) {
    return (CbmMvdSensor*) fSensorArray->At(nSensor);
  };
  void PrintParameter() { fParameter->Print(); };

  void SetProduceNoise();

  static void SetSensorTyp(CbmMvdSensorTyp typ) { fSensorTyp = typ; };

private:
  static CbmMvdSensorTyp fSensorTyp;

  TClonesArray* fSensorArray;
  TClonesArray* fSensorIDArray;  //Array of Sensor ID
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

  ClassDef(CbmMvdDetector, 1);
};

#endif
