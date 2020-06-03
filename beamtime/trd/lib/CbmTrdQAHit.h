#ifndef CBMTRDQAHIT_H
#define CBMTRDQAHIT_H

#include "CbmTrdQABase.h"

/** @Class CbmTrdQAHit
 ** @author Philipp Munkes <p.munkes@uni-muenster.de>
 ** @date 10th April 2017
 ** @brief Example of a derived class for CBM TRD Testbeam analysis.
 **
 ** This class is a collection of analysis for Hit messages.
 **/

class CbmTrdQAHit : public CbmTrdQABase
{
      protected:
    /** Constructor with parameters (Optional) **/
    CbmTrdQAHit(TString ClassName);
    public:

    virtual void CreateHistograms();
    virtual void Exec(Option_t*);
    /** Default constructor **/
    CbmTrdQAHit():CbmTrdQAHit("CbmTrdQAHit"){};

    ClassDef(CbmTrdQAHit,1);
};

#endif
