#ifndef CBMTRDPARSETGAIN_H
#define CBMTRDPARSETGAIN_H

#include <Rtypes.h>        // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>    // for Bool_t, kTRUE

#include "CbmTrdParSet.h"  // for CbmTrdParSet

class FairParamList;

/**
  * \brief Describe TRD module working settings (HV, etc)
  **/
class FairParamList;
class CbmTrdGainMod;
class CbmTrdParSetGain : public CbmTrdParSet 
{
public:

  /** Standard constructor **/
  CbmTrdParSetGain(const char* name    = "CbmTrdParSetGain",
    const char* title   = "TRD GAIN parameters",
    const char* context = "Default");

  /** \brief Destructor **/
  virtual ~CbmTrdParSetGain() {;}
  
  /** \brief Reset all parameters **/
  virtual void  clear() {;}

  void          putParams(FairParamList*);
  Bool_t        getParams(FairParamList*) { return kTRUE;}

private:  

  ClassDef(CbmTrdParSetGain,1);
};
#endif
