#ifndef CBMSPADICTRIGGERCOMP_H
#define CBMSPADICTRIGGERCOMP_H

#include "FairTask.h"
#include "CbmSpadicRawMessage.h"
#include "TClonesArray.h"
#include <algorithm>

class CbmSpadicTriggerComp : public FairTask
{
  public:

    /** Default constructor **/
    CbmSpadicTriggerComp();

    /** Constructor with parameters (Optional) **/
    //  CbmSpadicTriggerComp(Int_t verbose);


    /** Destructor **/
    ~CbmSpadicTriggerComp();


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

/*
    * Executed after each event. *
    virtual void FinishEvent();

    * Action at end of run. For this task and all of the subtasks. *
    virtual void FinishTask();
*/
//    inline Int_t GetSpadicID(Int_t sourceA);
    Int_t GetSpadicID(Int_t sourceA);


  private:

    /** Input array from previous already existing data level **/
    TClonesArray* fRawSpadic;
   /** Output array **/
    TClonesArray* fProcSpadic;

    Int_t fMessageCounter;
    Int_t fProcessedMessages;
    Int_t fNrTimeSlices;


  public:

    CbmSpadicTriggerComp(const CbmSpadicTriggerComp&);
    CbmSpadicTriggerComp operator=(const CbmSpadicTriggerComp&);

    ClassDef(CbmSpadicTriggerComp,1);
};

#endif
