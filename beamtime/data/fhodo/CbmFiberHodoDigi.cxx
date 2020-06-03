#include "CbmFiberHodoDigi.h"

#include <sstream>
using std::endl;
using std::stringstream;

CbmFiberHodoDigi::CbmFiberHodoDigi() 
  : fAddress(-1),
    fCharge(-1.),
    fTime(0)
{
}

CbmFiberHodoDigi::CbmFiberHodoDigi(
      Int_t address,
      Double_t charge,
      ULong_t time)
  : fAddress(address),
    fCharge(charge),
    fTime(time)
{
}

CbmFiberHodoDigi::~CbmFiberHodoDigi()
{
}

std::string CbmFiberHodoDigi::ToString() const {
   stringstream ss;
   ss << "CbmFiberHodoDigi: address=" << fAddress << " charge=" << fCharge
         << " time=" << fTime << endl;
   return ss.str();
}

ClassImp(CbmFiberHodoDigi)
