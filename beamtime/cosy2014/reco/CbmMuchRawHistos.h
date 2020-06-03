#ifndef CBMMUCHRAWHISTOS_H
#define CBMMUCHRAWHISTOS_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Option_t, ULong_t
#include <vector>
#include "CbmMuchBeamTimeDigi.h"
#include "FairTask.h"    // for InitStatus, FairTask

#include "CbmHistManager.h" // for RootCling

class TClonesArray;

class CbmMuchRawHistos : public FairTask
{
  public:

    /** Default constructor **/
    CbmMuchRawHistos();

    /** Destructor **/
    ~CbmMuchRawHistos();


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

    const std::vector<CbmMuchBeamTimeDigi>* fDigiVec = nullptr;
    TClonesArray* fDigiArr = nullptr;

    CbmHistManager* fHM;   ///< Histogram manager

    ULong_t fNofEvents;   ///< Event counter

    void CreateHistograms();

    CbmMuchRawHistos(const CbmMuchRawHistos&);
    CbmMuchRawHistos operator=(const CbmMuchRawHistos&);

    ClassDef(CbmMuchRawHistos,1);
};

#endif
