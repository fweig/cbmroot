#include "CbmTrdModuleAbstract.h"

//_______________________________________________________________________________
CbmTrdModuleAbstract::CbmTrdModuleAbstract()
  : TNamed()
  ,fModAddress(0)
  ,fLayerId(-1)
  ,fRotation(0)
  ,fDigiPar(nullptr)
  ,fChmbPar(nullptr)
  ,fAsicPar(nullptr)
  ,fGainPar(nullptr)
  ,fGeoPar(nullptr)
{
}

//_______________________________________________________________________________
CbmTrdModuleAbstract::CbmTrdModuleAbstract(Int_t mod, Int_t ly, Int_t rot)
  : TNamed("CbmTrdModule", "Abstract TRD module implementation")
  ,fModAddress(mod)
  ,fLayerId(ly)
  ,fRotation(rot)
  ,fDigiPar(nullptr)
  ,fChmbPar(nullptr)
  ,fAsicPar(nullptr)
  ,fGainPar(nullptr)
  ,fGeoPar(nullptr)
{
}

//_______________________________________________________________________________
CbmTrdModuleAbstract::~CbmTrdModuleAbstract()
{
  if(fAsicPar) delete fAsicPar;
}

//_______________________________________________________________________________
void CbmTrdModuleAbstract::LocalToMaster(Double_t in[3], Double_t out[3])
{
  if(!fGeoPar) return;
  fGeoPar->LocalToMaster(in, out);
}

ClassImp(CbmTrdModuleAbstract)
