/** @file CbmTofPoint.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @author Christian Simon <c.simon@physi.uni-heidelberg.de>
 ** @since 16.06.2014
 ** @date 11.04.2017
 **/

#include "CbmTofPoint.h"

#include <FairMCPoint.h>  // for FairMCPoint

#include <TVector3.h>  // for TVector3

#include <bitset>   // for bitset
#include <cassert>  // for assert
#include <limits>   // for numeric_limits, numeric_limits<>::digits
#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits

using std::endl;
using std::string;
using std::stringstream;


// -----   Default constructor   -------------------------------------------
CbmTofPoint::CbmTofPoint() : FairMCPoint(), fNofCells(0), fGapMask(0) {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTofPoint::CbmTofPoint(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom, Double_t tof, Double_t length,
                         Double_t eLoss)
  : FairMCPoint(trackID, detID, pos, mom, tof, length, eLoss)
  , fNofCells(0)
  , fGapMask(0)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTofPoint::~CbmTofPoint() {}
// -------------------------------------------------------------------------


// -----   Get the number of gaps   ----------------------------------------
Int_t CbmTofPoint::GetNGaps() const
{
  Int_t iNGaps(0);

  for (Int_t iGapBit = 0; iGapBit < std::numeric_limits<UShort_t>::digits; iGapBit++) {
    if (fGapMask & (0x1 << iGapBit)) { iNGaps++; }
  }

  return iNGaps;
}
// -------------------------------------------------------------------------


// -----   Get the index of the first gap   --------------------------------
Int_t CbmTofPoint::GetFirstGap() const
{
  for (Int_t iGapBit = 0; iGapBit < std::numeric_limits<UShort_t>::digits; iGapBit++) {
    if (fGapMask & (0x1 << iGapBit)) { return iGapBit; }
  }

  return -1;
}
// -------------------------------------------------------------------------


// -----   Get the index of the last gap   ---------------------------------
Int_t CbmTofPoint::GetLastGap() const
{
  Int_t iLastGap(-1);

  for (Int_t iGapBit = 0; iGapBit < std::numeric_limits<UShort_t>::digits; iGapBit++) {
    if (fGapMask & (0x1 << iGapBit)) { iLastGap = iGapBit; }
  }

  return iLastGap;
}
// -------------------------------------------------------------------------


// -----   Add one gap to the gap mask   -----------------------------------
void CbmTofPoint::SetGap(Int_t iGap)
{
  assert(0 <= iGap && std::numeric_limits<UShort_t>::digits > iGap);
  fGapMask |= 0x1 << iGap;
}
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
string CbmTofPoint::ToString() const
{
  stringstream ss;
  ss << "STofPoint: track ID " << fTrackID << ", detector ID " << fDetectorID << "\n";
  ss << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm \n";
  ss << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV \n";
  ss << "    Time " << fTime << " ns,  Length " << fLength << " cm,  Energy loss " << fELoss * 1.0e06 << " keV \n";
  ss << "    Number of cells " << fNofCells << ", gap mask "
     << std::bitset<std::numeric_limits<UShort_t>::digits>(fGapMask) << endl;
  return ss.str();
}
// -------------------------------------------------------------------------

ClassImp(CbmTofPoint)
