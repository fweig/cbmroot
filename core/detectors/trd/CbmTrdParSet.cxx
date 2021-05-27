#include "CbmTrdParSet.h"

#include "CbmTrdParMod.h"  // for CbmTrdParMod

#include <FairParGenericSet.h>  // for FairParGenericSet
#include <FairParamList.h>      // for FairParamList

#include <TGenericClassInfo.h>  // for TGenericClassInfo

#include <utility>  // for pair

#include <stdio.h>  // for printf

using std::map;
//_______________________________________________________________________________
CbmTrdParSet::CbmTrdParSet(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
  , fNrOfModules(0)
  , fModuleMap()
{
  //printf("%s (%s, %s, %s)\n", GetName(), name, title, context);
}

//_______________________________________________________________________________
CbmTrdParSet::~CbmTrdParSet()
{
  for (map<Int_t, CbmTrdParMod*>::iterator imod = fModuleMap.begin(); imod != fModuleMap.end(); imod++)
    delete imod->second;
  fModuleMap.clear();
}

//_______________________________________________________________________________
Int_t CbmTrdParSet::GetModuleId(Int_t i) const
{
  if (i < 0 || i >= fNrOfModules) return -1;
  Int_t j(0);
  for (map<Int_t, CbmTrdParMod*>::const_iterator imod = fModuleMap.begin(); imod != fModuleMap.end(); imod++, j++) {
    if (j < i) continue;
    return imod->first;
  }
  return -1;
}

//_______________________________________________________________________________
const CbmTrdParMod* CbmTrdParSet::GetModulePar(Int_t detId) const
{
  map<Int_t, CbmTrdParMod*>::const_iterator imod = fModuleMap.find(detId);
  if (imod == fModuleMap.end()) return nullptr;
  return imod->second;
}

//_______________________________________________________________________________
Bool_t CbmTrdParSet::getParams(FairParamList* l)
{
  if (!l) return kFALSE;
  l->print();
  return kTRUE;
}


//_______________________________________________________________________________
void CbmTrdParSet::putParams(FairParamList* /*l*/)
{
  printf("%s(%s)::putParams not implemented \n", GetName(), GetTitle());
}

//_______________________________________________________________________________
void CbmTrdParSet::addParam(CbmTrdParMod* mod)
{
  fModuleMap[mod->GetModuleId()] = mod;
  fNrOfModules++;
}

ClassImp(CbmTrdParSet)
