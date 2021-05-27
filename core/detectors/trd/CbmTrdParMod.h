#ifndef CBMTRDPARMOD_H
#define CBMTRDPARMOD_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t
#include <TNamed.h>      // for TNamed

/** \brief Definition of generic parameters for one TRD module **/
class CbmTrdParMod : public TNamed {
public:
  CbmTrdParMod(const char* name = "CbmTrdParMod", const char* title = "TRD generic module definition");
  virtual ~CbmTrdParMod();

  virtual Int_t GetModuleId() const { return fModuleId; }

  virtual void SetModuleId(Int_t m) { fModuleId = m; }

protected:
  Int_t fModuleId;  ///< module id
private:
  ClassDef(CbmTrdParMod,
           1)  // Definition of generic parameters for one TRD module
};

#endif
