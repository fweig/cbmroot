#include "CbmTrdParSetGain.h"

#include <FairParamList.h>  // for FairParamList
#include <Logger.h>         // for LOG, Logger

#include <TArrayI.h>  // for TArrayI

#include <map>      // for map, map<>::iterator, operator!=, __m...
#include <utility>  // for pair

CbmTrdParSetGain::CbmTrdParSetGain(const char* name, const char* title, const char* context)
  : CbmTrdParSet(name, title, context)
{
}

//_____________________________________________________________________
void CbmTrdParSetGain::putParams(FairParamList* l)
{
  if (!l) return;
  LOG(info) << GetName() << "::putParams(FairParamList*)";

  TArrayI moduleId(fNrOfModules);
  Int_t idx(0);
  for (std::map<Int_t, CbmTrdParMod*>::iterator imod = fModuleMap.begin(); imod != fModuleMap.end(); imod++) {
    moduleId[idx++] = imod->first;
  }
  l->add("NrOfModules", fNrOfModules);
  l->add("ModuleIdArray", moduleId);
}
ClassImp(CbmTrdParSetGain)
