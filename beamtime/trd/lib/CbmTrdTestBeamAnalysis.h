#ifndef CBMTRDTESTBEAMANALYSIS_H
#define CBMTRDTESTBEAMANALYSIS_H

#include "FairTask.h"

#include "CbmHistManager.h"

#include "TClonesArray.h"
#include "TNtuple.h"

class CbmTrdTestBeamAnalysis : public FairTask
{
  public:

  enum EBaseMetod { kFirstTimeBin=0, kLastTimeBins, kNoise };

    /** Default constructor **/
    CbmTrdTestBeamAnalysis();

    /** Constructor with parameters (Optional) **/
    //  CbmTrdTestBeamAnalysis(Int_t verbose);


    /** Destructor **/
    ~CbmTrdTestBeamAnalysis();


    /** Initiliazation of task at the beginning of a run **/
    virtual InitStatus Init();

    /** ReInitiliazation of task when the runID changes **/
    virtual InitStatus ReInit();


    /** Executed for each event. **/
    virtual void Exec(Option_t* opt);

    /** Load the parameter container from the runtime database **/
    virtual void SetParContainers();

    /** Finish task called at the end of the run **/
    virtual void Finish();

    /** Setter **/
    void SetTimeBins(Int_t nbins)               { fTimeBins = nbins; }

    /** Selections **/
    void SetSysCoreSelection(Int_t detID)       { fSysCore=detID; }
    void SetSpadicSelection( Int_t spadic)      { fSpadic=spadic; }
 
    /** Baseline method **/
    void SetBaselineMethod(  EBaseMetod method, Int_t nbins=1) { fBaseMethod=method; fBaselineBins=nbins; }

  private:

    Int_t fTimeBins;

    TString GetSysCore(Int_t eqID);
    Int_t   GetSysCoreID(Int_t eqID);

    TString GetSpadic(Int_t sourceA);
    Int_t   GetSpadicID(Int_t sourceA);

    /** Input array from previous already existing data level **/
    TClonesArray* fRawSpadic;

    CbmHistManager* fHM;

    // syscore and spadic
    Int_t fSysCore;
    Int_t fSpadic;

    // timecounter
    Int_t fTimeCounter;

    //Baseline
    EBaseMetod fBaseMethod; // base line method
    Int_t fBaselineBins;    // bins used for baseline calculation
    Int_t fBaseline[32];    // base line values

    // Noise Counter
    Int_t fNoiseCounter;

    // ntuple for timecounting
    TNtuple *fNT;

    /** Output array to  new data level**/
    //  TClonesArray* <OutputDataLevel>;

    void CreateHistograms();

    CbmTrdTestBeamAnalysis(const CbmTrdTestBeamAnalysis&);
    CbmTrdTestBeamAnalysis operator=(const CbmTrdTestBeamAnalysis&);

    ClassDef(CbmTrdTestBeamAnalysis,1);
};

#endif
