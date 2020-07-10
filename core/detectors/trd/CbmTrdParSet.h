#ifndef CBMTRDPARSET_H
#define CBMTRDPARSET_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Bool_t

#include <map>  // for map

#include "FairParGenericSet.h"  // for FairParGenericSet

class CbmTrdParMod;
class FairParamList;

/**
  * \brief Describe a set of TRD module working parameters
  **/
class FairParamList;
class CbmTrdParMod;
class CbmTrdParSet : public FairParGenericSet {
public:
  /** Standard constructor **/
  CbmTrdParSet(const char* name    = "CbmTrdParSet",
               const char* title   = "TRD parameters",
               const char* context = "Default");

  /** \brief Destructor **/
  virtual ~CbmTrdParSet();

  /** \brief Reset all parameters **/
  virtual void clear() { ; }

  virtual Int_t GetModuleId(Int_t i) const;
  virtual const CbmTrdParMod* GetModulePar(Int_t detId) const;
  virtual Int_t GetNrOfModules() const { return fNrOfModules; }
  std::map<Int_t, CbmTrdParMod*> GetModuleMap() { return fModuleMap; }
  virtual void addParam(CbmTrdParMod* mod);
  virtual Bool_t getParams(FairParamList*);
  virtual void putParams(FairParamList*);

protected:
  Int_t fNrOfModules;  ///< no of modules in the current run
  /** Map of parameters for each TRD Module organized as function of Module unique Id **/
  std::map<Int_t, CbmTrdParMod*> fModuleMap;

  ClassDef(CbmTrdParSet, 1);
};
#endif
