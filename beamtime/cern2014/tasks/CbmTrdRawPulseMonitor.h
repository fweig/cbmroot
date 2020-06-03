#ifndef CBMTRDRAWPULSEMONITOR_H
#define CBMTRDRAWPULSEMONITOR_H

#include "FairTask.h"
#include "CbmHistManager.h"
#include "CbmSpadicRawMessage.h"
#include "TClonesArray.h"
#include "TCanvas.h"

class CbmTrdRawPulseMonitor : public FairTask
{
  public:

    /** Default constructor **/
    CbmTrdRawPulseMonitor();

    /** Constructor with parameters (Optional) **/
    //  CbmTrdRawPulseMonitor(Int_t verbose);


    /** Destructor **/
    ~CbmTrdRawPulseMonitor();


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

  private:

    /** Input array from previous already existing data level **/
    TClonesArray* fRawSpadic;
   /** Output array **/
    TCanvas *fMonitor;
    TCanvas *fRatio;
    TH1I* fRawpulse;
    TH1I* fS_N;
    TH2I* fSignalMap;

    CbmHistManager* fHM;

    Int_t fMessageCounter;
    Int_t fContainerCounter;

    CbmTrdRawPulseMonitor(const CbmTrdRawPulseMonitor&);
    CbmTrdRawPulseMonitor operator=(const CbmTrdRawPulseMonitor&);

    ClassDef(CbmTrdRawPulseMonitor,2);
};

#endif
