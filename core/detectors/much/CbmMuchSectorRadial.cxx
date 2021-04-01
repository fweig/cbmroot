/** CbmMuchSectorRadial.cxx
 *@author  E.Kryshen <e.kryshen@gsi.de>
 *@since   6.02.12
 *@version 1.0
 **
 ** This class describes the digitization scheme for a ring of pads
 **
 **/
#include "CbmMuchSectorRadial.h"

#include "CbmMuchAddress.h"    // for CbmMuchAddress, kMuchChannel
#include "CbmMuchPadRadial.h"  // for CbmMuchPadRadial

#include <Logger.h>  // for Logger, LOG

#include <TMath.h>  // for Ceil, Floor, Pi, TwoPi

#include <vector>  // for vector


// -----   Default constructor   -------------------------------------------
CbmMuchSectorRadial::CbmMuchSectorRadial()
  : CbmMuchSector(), fR1(0.), fR2(0.), fPhi1(0.), fPhi2(0.), fPadDphi(0.) {}
// -------------------------------------------------------------------------


// -----  Standard constructor  --------------------------------------------
CbmMuchSectorRadial::CbmMuchSectorRadial(Int_t detId,
                                         Int_t secId,
                                         Double_t r1,
                                         Double_t r2,
                                         Double_t phi1,
                                         Double_t phi2)
  : CbmMuchSector(detId, secId, 0)
  , fR1(r1)
  , fR2(r2)
  , fPhi1(0.)
  , fPhi2(0.)
  , fPadDphi((r2 - r1) / r1) {
  if (phi2 < phi1) LOG(error) << "Panic!";
  fNChannels = 2 * TMath::Ceil((phi2 - phi1) / 2 / fPadDphi);
  fPhi1      = (phi1 + phi2) / 2 - fNChannels / 2 * fPadDphi;
  fPhi2      = (phi1 + phi2) / 2 + fNChannels / 2 * fPadDphi;
}
// -----------------o--------------------------------------------------------


// -----   Public method GetChannel   --------------------------------------
Int_t CbmMuchSectorRadial::GetPadIndexByPhi(Double_t phi) {
  if (fPhi2 > TMath::Pi() && phi < fPhi1) phi += TMath::TwoPi();
  if (phi < fPhi1 || phi > fPhi2) return -1;
  return TMath::Floor((fPhi2 - phi) / fPadDphi);
}
// -------------------------------------------------------------------------


// -----   Public method GetChannel   --------------------------------------
CbmMuchPadRadial* CbmMuchSectorRadial::GetPadByPhi(Double_t phi) {
  Int_t i = GetPadIndexByPhi(phi);
  //  printf("i=%i\n",i);
  if (i == -1) return nullptr;

  return (CbmMuchPadRadial*) GetPadByChannelIndex(i);
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchSectorRadial::AddPads() {
  for (Int_t iChannel = 0; iChannel < fNChannels; iChannel++) {
    UInt_t address =
      CbmMuchAddress::SetElementId(fAddress, kMuchChannel, iChannel);
    Double_t r1, r2, phi1, phi2;
    r1              = fR1;
    r2              = fR2;
    phi1            = fPhi2 - iChannel * fPadDphi;
    phi2            = phi1 - fPadDphi;
    CbmMuchPad* pad = new CbmMuchPadRadial(address, r1, r2, phi1, phi2);
    fPads.push_back(pad);
    LOG(debug) << "iChannel=" << iChannel << " fPads.size()=" << fPads.size()
               << " fNChannels=" << fNChannels;
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMuchSectorRadial::DrawPads() {
  for (Int_t iChannel = 0; iChannel < fNChannels; iChannel++) {
    CbmMuchPadRadial* pad = (CbmMuchPadRadial*) fPads[iChannel];
    pad->DrawPad();
    //     pad->Draw();
  }
}
// -------------------------------------------------------------------------


ClassImp(CbmMuchSectorRadial)
