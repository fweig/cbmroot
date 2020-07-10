#ifndef CBMTRDPARSETGEO_H
#define CBMTRDPARSETGEO_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Bool_t, Option_t

#include "CbmTrdParSet.h"  // for CbmTrdParSet

class CbmTrdParSetGeo : public CbmTrdParSet {
public:
  CbmTrdParSetGeo(const char* name    = "CbmTrdParSetGeo",
                  const char* title   = "Trd Geometry Parameters",
                  const char* context = "TestDefaultContext");
  virtual ~CbmTrdParSetGeo(void);
  /** \brief Fill map with full geometrical description for each detector to be distributed to all processing modules
   */
  Bool_t Init();
  virtual void Print(Option_t* opt = "") const;

private:
  ClassDef(CbmTrdParSetGeo, 1)
};

#endif /* !CBMTRDPARSETGEO_H */
