/**
 * \file CbmStripHit.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 **/
#include "CbmStripHit.h"

#include <TVector3.h>  // for TVector3

#include <sstream>  // for operator<<, basic_ostream, stringstream

using std::endl;
using std::stringstream;


CbmStripHit::CbmStripHit() : CbmStripHit(-1, 0., 0., 0., 0., 0., 0., -1) {}

CbmStripHit::CbmStripHit(Int_t address, const TVector3& pos, const TVector3& err, Int_t refId, Double_t time,
                         Double_t timeError)
  : CbmStripHit(address, pos.X(), pos.Y(), pos.Z(), err.X(), err.Y(), err.Z(), refId, time, timeError)
{
}


CbmStripHit::CbmStripHit(Int_t address, Double_t u, Double_t phi, Double_t z, Double_t du, Double_t dphi, Double_t dz,
                         Int_t refId, Double_t time, Double_t timeError)
  : CbmHit(kSTRIPHIT, z, dz, refId, address, time, timeError)
  , fU(u)
  , fDu(du)
  , fPhi(phi)
  , fDphi(dphi)
{
}

CbmStripHit::~CbmStripHit() {}

std::string CbmStripHit::ToString() const
{
  stringstream ss;
  ss << "CbmStripHit: address=" << GetAddress() << " pos=(" << GetU() << "," << GetPhi() << "," << GetZ() << ") err=("
     << GetDu() << "," << GetDphi() << "," << GetDz() << ") refId=" << GetRefId() << endl;
  return ss.str();
}

ClassImp(CbmStripHit);
