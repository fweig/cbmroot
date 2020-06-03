// -------------------------------------------------------------------------
// -----                      CbmTrdPoint source file                  -----
// -----                   Created 27/07/04  by V. Friese              -----
// -------------------------------------------------------------------------

#include "CbmTrdPoint.h"

#include <FairMCPoint.h>  // for FairMCPoint
#include <FairLogger.h>   // for Logger, LOG

#include <TVector3.h>     // for TVector3

// -----   Default constructor   -------------------------------------------
CbmTrdPoint::CbmTrdPoint() 
  : FairMCPoint(),
    fX_out(0.),
    fY_out(0.),
    fZ_out(0.),
    fPx_out(0.),
    fPy_out(0.),
    fPz_out(0.)
{ 
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
CbmTrdPoint::CbmTrdPoint(Int_t trackID, Int_t detID, const TVector3& posIn,
			 const TVector3& momIn, const TVector3& posOut,  const TVector3& momOut,
                         Double_t tof, Double_t length, Double_t eLoss)
  : FairMCPoint(trackID, detID, posIn, momIn, tof, length, eLoss),
  fX_out(posOut.X()),
  fY_out(posOut.Y()),
  fZ_out(posOut.Z()),
  fPx_out(momOut.Px()),
  fPy_out(momOut.Py()),
  fPz_out(momOut.Pz())
{ 
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTrdPoint::~CbmTrdPoint() { }
// -------------------------------------------------------------------------



// -----   Public method Print   -------------------------------------------
void CbmTrdPoint::Print(const Option_t* /*opt*/) const {
  LOG(info) << "TRD point for track " << fTrackID 
            << " in detector " << fDetectorID;
  LOG(info) << "    Position In (" << fX << ", " << fY << ", " << fZ
            << ") cm";
  LOG(info) << "    Momentum In (" << fPx << ", " << fPy << ", " << fPz
            << ") GeV";
  LOG(info) << "    Position Out (" << fX_out << ", " << fY_out << ", " << fZ_out
            << ") cm";
  LOG(info) << "    Momentum Out (" << fPx_out << ", " << fPy_out << ", " << fPz_out
            << ") GeV";
  LOG(info) << "    Time " << fTime << " ns,  Length " << fLength 
            << " cm,  Energy loss " << fELoss*1.0e06 << " keV";
}
// -------------------------------------------------------------------------


ClassImp(CbmTrdPoint)
