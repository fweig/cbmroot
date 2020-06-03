// ----------------------------------------------------------------------------
// -----                    CbmMvdDigitizerTB header file                   -----
// -----                   Created by P.Sitzmann (p.sitzmann(att)gsi.de    -----
// ----------------------------------------------------------------------------

#ifndef CBMMVDDIGITIZERTB_H
#define CBMMVDDIGITIZERTB_H 1

#include "FairTask.h"

#include "TString.h"
#include "TStopwatch.h"

class FairTask;
class CbmMvdDetector;
class CbmMvdPileupManager;

using std::pair;


class CbmMvdDigitizerTB : public FairTask
{
 
 public:

  /** Default constructor **/  
  CbmMvdDigitizerTB();


  /** Standard constructor 
  *@param name  Task name
  *@param mode  0 = MAPS, 1 = Ideal
  **/
  CbmMvdDigitizerTB(const char* name,
		    Int_t mode = 0, Int_t iVerbose = 1);


  /** Destructor **/
  ~CbmMvdDigitizerTB();

  void Exec(Option_t* opt);
  void SetMisalignment(Float_t misalignment[3]){for(Int_t i = 0; i<3; i++) epsilon[i]=misalignment[i];} // set the misalignment in cm
  void ShowDebugHistograms() {fShowDebugHistos = kTRUE;}
  void SetProduceNoise(){fNoiseSensors = kTRUE;};

private:
/** Hit producer mode (0 = MAPS, 1 = Ideal) **/
    Int_t fMode;
    Int_t eventNumber;

  Bool_t fShowDebugHistos;
  Bool_t fNoiseSensors;

  CbmMvdDetector* fDetector;
 
  TClonesArray* fInputPoints;
  TClonesArray* fTracks;
 TClonesArray* fDigis;
 TClonesArray* fDigiMatch;

 std::pair<Float_t, Int_t> fPerformanceDigi;

 UInt_t fDigiPluginNr;

 Double_t fFakeRate;          // Fake hit rate

 Float_t epsilon[3];

 TString	fBranchName;   // Name of input branch (MvdPoint)

 TStopwatch     fTimer;        ///< ROOT timer

// -----   Private methods   ---------------------------------------------
 /** Intialisation **/
  virtual InitStatus Init();


  /** Reinitialisation **/
  virtual InitStatus ReInit();


  /** Virtual method Finish **/
  virtual void Finish();


  /** Register the output arrays to the IOManager **/
  void Register();

  void GetMvdGeometry();


  /** Clear the hit arrays **/
  void Reset();	 


  /** Print digitisation parameters **/
  void PrintParameters();

private:
  CbmMvdDigitizerTB(const CbmMvdDigitizerTB&);
  CbmMvdDigitizerTB operator=(const CbmMvdDigitizerTB&);

ClassDef(CbmMvdDigitizerTB,1);
};
    
    
#endif   		     
