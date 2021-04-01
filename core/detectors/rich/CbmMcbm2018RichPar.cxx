/**

	TODO: If the TBR addess is not provided in the par file but found in the data file,
	The std::map::at will throw out an exception which is not currectly caught.

*/

#include "CbmMcbm2018RichPar.h"

// FairRoot
#include "FairParamList.h"
#include <Logger.h>

// C/C++
#include <iomanip>

//#include <Logger.h> //TODO delete

CbmMcbm2018RichPar::CbmMcbm2018RichPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
{
  detName = "RICH";
}

CbmMcbm2018RichPar::~CbmMcbm2018RichPar() {}

void CbmMcbm2018RichPar::putParams(FairParamList* l)
{
  if (!l) return;

  l->add("TRBaddresses", fTRBaddresses);
  l->add("ToTshifts", fToTshifts);
}

Bool_t CbmMcbm2018RichPar::getParams(FairParamList* l)
{
  if (!l) return kFALSE;

  if (!l->fill("TRBaddresses", &fTRBaddresses)) return kFALSE;
  if (!l->fill("ToTshifts", &fToTshifts)) return kFALSE;

  // Create a map from the list imported from the par file
  Int_t siz = fTRBaddresses.GetSize();
  for (Int_t i = 0; i < siz; i++) {
    fTRBaddrMap.insert(std::pair<Int_t, Int_t>(fTRBaddresses[i], i));
  }

  // Create a map from the lists imported from the par file
  for (Int_t i = 0; i < siz; i++) {
    for (Int_t ch = 0; ch <= 32; ch++) {
      fToTshiftMap.insert(std::pair<Int_t, Double_t>(i * 33 + ch, fToTshifts[i * 33 + ch]));
    }
  }

  return kTRUE;
}

Int_t CbmMcbm2018RichPar::GetAddressIdx(Int_t addr) const
{
  //TODO catch exception only for map
  try {
    Int_t idx = fTRBaddrMap.at(addr);
    return idx;
  }
  catch (...) {
    LOG(warning) << "CbmMcbm2018RichPar::GetAddressIdx => Unknown TRB address 0x" << std::hex << std::setw(4) << addr
                 << ", probably corrupted data!";
    return -1;
  }
}

Int_t CbmMcbm2018RichPar::GetAddress(Int_t ind) const
{
  if (ind < 0 || ind >= fTRBaddresses.GetSize()) return -1;
  return fTRBaddresses[ind];
}

ClassImp(CbmMcbm2018RichPar)
