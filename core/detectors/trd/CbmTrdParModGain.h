#ifndef CBMTRDPARMODGAIN_H
#define CBMTRDPARMODGAIN_H

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

#include "CbmTrdParMod.h"  // for CbmTrdParMod

/** \brief Definition of gain parameters for one TRD module **/
class CbmTrdParModGain : public CbmTrdParMod {
public:
  CbmTrdParModGain(const char* name  = "CbmTrdParModGain",
                   const char* title = "TRD gain conversion");
  virtual ~CbmTrdParModGain() { ; }

private:
  ClassDef(CbmTrdParModGain,
           1)  // Definition of gain parameters for one TRD module
};

#endif
