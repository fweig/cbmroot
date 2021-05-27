#ifndef CBMTRDHITDENSITYQA_H_
#define CBMTRDHITDENSITYQA_H_

#include "FairTask.h"

// #include "CbmTrdDigitizerMathieson.h"
// #include "CbmTrdHitProducerCluster.h"
// #include "TLegend.h"
// #include "TClonesArray.h"
// #include "TSystem.h"
// #include "TVector3.h"
// #include <vector>
#include <list>
// #include <map>
#include <fstream>

class CbmTrdParSetAsic;
class CbmTrdParSetDigi;
class CbmTrdParSetGeo;
class CbmTrdGeoHandler;
class TH2I;
class TH1I;
class TH1D;
class TH1F;
class TClonesArray;

// typedef std::list<MyPoint*> MyPointList;
// typedef std::map<Int_t, MyPointList*> MyPointListMap;

class CbmTrdHitDensityQa : public FairTask {
public:
  // ---- Default constructor -------------------------------------------
  CbmTrdHitDensityQa();
  CbmTrdHitDensityQa(Double_t TriggerThreshold, Double_t EventRate, Double_t ScaleCentral2mBias);
  // ---- Destructor ----------------------------------------------------
  virtual ~CbmTrdHitDensityQa();
  /** Initialisation **/
  virtual InitStatus ReInit();
  virtual InitStatus Init();
  virtual void SetParContainers();

  /** Executed task **/
  virtual void Exec(Option_t* option);

  /** Virtual method EndOfEvent
   **
   ** Reset hit collection at the
   ** end of the event.
   **/
  virtual void FinishEvent();

  /** Finish task **/
  virtual void Finish();
  void Register();
  void SetTriggerThreshold(Double_t triggerthreshold);
  void SetNeighbourTrigger(Bool_t trigger);
  void SetPlotResults(Bool_t plotResults);
  void SetScaleCentral2mBias(Double_t scaling);
  void SetTriggerMaxScale(Double_t max);
  void SetTriggerMinScale(Double_t min);
  void SetLogScale(Bool_t logScale);
  void SetRatioTwoFiles(Bool_t ratioPlot);

private:
  Double_t TriggerRate2DataRate(Double_t triggerrate);
  Double_t DataRate2TriggerRate(Double_t datarate);
  Double_t TriggerCount2TriggerRate(Double_t count);
  Double_t TriggerRate2TriggerCount(Double_t rate);

  std::ofstream myfile;

  Double_t fmin, fmax;
  Bool_t flogScale;
  Double_t fBitPerHit;
  TH1F* h1DataModule;
  TH1F* h1OptLinksModule;

  Bool_t fNeighbourTrigger;
  Bool_t fPlotResults;
  Bool_t fRatioTwoFiles;

  TClonesArray* fClusters;

  CbmTrdParSetAsic* fAsicPar;     //!
  CbmTrdParSetDigi* fDigiPar;     //!
  CbmTrdParSetGeo* fGeoPar;       //!
  CbmTrdGeoHandler* fGeoHandler;  //!

  Int_t fStation;
  Int_t fLayer;
  Int_t fModuleID;

  TH1I* fEventCounter;

  Double_t fTriggerThreshold;    // SIS300:1E-6
  Double_t fEventRate;           //SIS300:1E7
  Double_t fScaleCentral2mBias;  //SIS300:1/4

  std::map<Int_t, Int_t> fUsedDigiMap;
  std::map<Int_t, TH2I*> fModuleHitMap;
  std::map<Int_t, TH2I*>::iterator fModuleHitMapIt;
  std::map<Int_t, TH1D*> fModuleHitASICMap;
  std::map<Int_t, TH1D*>::iterator fModuleHitASICMapIt;
  CbmTrdHitDensityQa(const CbmTrdHitDensityQa&);
  CbmTrdHitDensityQa& operator=(const CbmTrdHitDensityQa&);

  ClassDef(CbmTrdHitDensityQa, 3);
};
#endif
