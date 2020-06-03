#ifndef CBMHODORAWHISTOS_H
#define CBMHODORAWHISTOS_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Option_t
#include <FairTask.h>    // for InitStatus, FairTask
#include <vector>

#include "CbmFiberHodoDigi.h"
#include "CbmHistManager.h" // for RootCling

class TClonesArray;  

class CbmHodoRawHistos : public FairTask
{
  public:

    /** Default constructor **/
    CbmHodoRawHistos();

    /** Destructor **/
    ~CbmHodoRawHistos();


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
    //  
    const std::vector<CbmFiberHodoDigi>* fHodoDigi;

    CbmHistManager* fHM;

    void CreateHistograms();

    CbmHodoRawHistos(const CbmHodoRawHistos&);
    CbmHodoRawHistos operator=(const CbmHodoRawHistos&);

    ClassDef(CbmHodoRawHistos,1);
};

#endif
