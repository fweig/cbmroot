// -------------------------------------------------------------------------
// -----                      CbmPsdPoint source file                 -----
// -----                   Created 28/07/04  by V. Friese              -----
// -------------------------------------------------------------------------

#include "CbmPsdPoint.h"

#include <FairMCPoint.h>  // for FairMCPoint
#include <Logger.h>       // for Logger, LOG

#include <sstream>  // for stringstream

// -----   Default constructor   -------------------------------------------
CbmPsdPoint::CbmPsdPoint() : FairMCPoint(), fModuleID(0) {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmPsdPoint::CbmPsdPoint(Int_t trackID,
                         Int_t detID,
                         TVector3 pos,
                         TVector3 mom,
                         Double_t tof,
                         Double_t length,
                         Double_t eLoss)
  : FairMCPoint(trackID, detID, pos, mom, tof, length, eLoss), fModuleID(0) {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmPsdPoint::~CbmPsdPoint() {}
// -------------------------------------------------------------------------


// -----   Public method Print   -------------------------------------------
void CbmPsdPoint::Print(const Option_t* /*opt*/) const {
  LOG(info) << ToString();
}

std::string CbmPsdPoint::ToString() const {
  std::stringstream ss;
  ss << "PSD point for track " << fTrackID << " in detector " << fDetectorID
     << "\n"
     << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm\n"
     << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV\n"
     << "    Time " << fTime << " ns,  Length " << fLength
     << " cm,  Energy loss " << fELoss * 1.0e06 << " keV";
  return ss.str();
}
// -------------------------------------------------------------------------

ClassImp(CbmPsdPoint)
