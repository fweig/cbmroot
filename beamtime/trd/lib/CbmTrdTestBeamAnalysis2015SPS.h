#ifndef CbmTrdTestBeamAnalysis2015SPS_H
#define CbmTrdTestBeamAnalysis2015SPS_H

#include "FairTask.h"

#include "CbmHistManager.h"

#include "TClonesArray.h"

class CbmTrdTestBeamAnalysis2015SPS : public FairTask
{
 public:
 
 enum EBaseMetod { kFirstTimeBin=0, kLastTimeBins, kNoise };

  CbmTrdTestBeamAnalysis2015SPS();
  ~CbmTrdTestBeamAnalysis2015SPS();

  virtual InitStatus Init();

  virtual InitStatus ReInit();

  virtual void Exec(Option_t* opt);

  virtual void SetParContainers();

  virtual void Finish();

  virtual void FinishEvent();
  
  void SetRun(Int_t run)               { fRun = run; std::cout << "  SetRun(" << run << ")" << std::endl;}
  
  void SetRewriteSpadicName(Bool_t rewrite)  { fRewriteSpadicName = rewrite; }
  void SetPlotSignalShape(Bool_t plot)  { fPlotSignalShape = plot; }
  void SetPlotClusterFinder(Bool_t plot)  { fPlotClusterFinder = plot; }

 private:

  Int_t   fTimeBins;
  Int_t   fRun;
  Int_t   fSpadics;
  Bool_t  fRewriteSpadicName;
  Bool_t  fPlotSignalShape;
  Bool_t  fPlotClusterFinder;

  Int_t   GetSysCoreID(Int_t eqID);
  Int_t   GetSpadicID(Int_t sourceA);

  TString GetSpadicName(Int_t eqID,Int_t sourceA);
  TString RewriteSpadicName(TString spadicName);

  TClonesArray* fRawSpadic;

  CbmHistManager* fHM;
  
    // timecounter
    Int_t fTimeCounter;
  
  
  //Baseline
    EBaseMetod fBaseMethod; // base line method
    Int_t fBaselineBins;    // bins used for baseline calculation
    Int_t fBaseline[32];    // base line values
  
  // Noise Counter
    Int_t fNoiseCounter;

    // ntuple for timecounting
    //TNtuple *fNT;

  //*/
  
  Int_t fNrTimeSlices;

  ULong_t fTimeMax;
  ULong_t fTimeMin;

  void CreateHistograms();

  Int_t GetChannelOnPadPlane(Int_t SpadicChannel);

  CbmTrdTestBeamAnalysis2015SPS(const CbmTrdTestBeamAnalysis2015SPS&);
  CbmTrdTestBeamAnalysis2015SPS operator=(const CbmTrdTestBeamAnalysis2015SPS&);

  ClassDef(CbmTrdTestBeamAnalysis2015SPS,1);
};

#endif


