#include "CbmTrdParMod.h"

#include <Logger.h>  // for LOG

//___________________________________________________________________
CbmTrdParMod::CbmTrdParMod(const char* name, const char* title) : TNamed(name, title), fModuleId(0) {}

//___________________________________________________________________
CbmTrdParMod::~CbmTrdParMod() { LOG(debug) << GetName() << "::delete[" << GetTitle() << "]"; }

ClassImp(CbmTrdParMod)
