#ifndef CBMTRDQABASELINE_H
#define CBMTRDQABASELINE_H

#include "CbmTrdQABase.h"

class CbmTrdQABaseline : public CbmTrdQABase
{
      protected:
    /** Constructor with parameters (Optional) **/
    CbmTrdQABaseline(TString ClassName);
    public:

    virtual void CreateHistograms();
    virtual void Exec(Option_t*);
    /** Default constructor **/
    CbmTrdQABaseline():CbmTrdQABaseline("CbmTrdQABaseline"){};

    ClassDef(CbmTrdQABaseline,1);
};

#endif
