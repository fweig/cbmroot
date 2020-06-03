#ifndef CBMTRDQAEFFICIENCY_H
#define CBMTRDQAEFFICIENCY_H

#include "CbmTrdQABase.h"

/** @Class CbmTrdQAEfficiency
 ** @author Philipp Munkes <p.munkes@uni-muenster.de>
 ** @date 05th June 2017
 ** @brief Class for the investigation of the SPADIC under Load.
 **
 **/

class CbmTrdQAEfficiency : public CbmTrdQABase
{
      protected:
    /** Constructor with parameters (Optional) **/
    CbmTrdQAEfficiency(TString ClassName);
    public:

    virtual void CreateHistograms();
    virtual void Exec(Option_t*);
    /** Default constructor **/
    CbmTrdQAEfficiency():CbmTrdQAEfficiency("CbmTrdQAEfficiency"){};

    ClassDef(CbmTrdQAEfficiency,1);
};

#endif
