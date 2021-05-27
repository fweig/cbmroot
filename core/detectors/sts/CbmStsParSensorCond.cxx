/** @file CbmStsParSensorCond.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 28.06.2014
 **/
#include "CbmStsParSensorCond.h"

#include <iostream>  // for operator<<, basic_ostream, stringstream
#include <sstream>   // for stringstream
#include <string>    // for char_traits

#include <cmath>  // for pow

using std::string;
using std::stringstream;

ClassImp(CbmStsParSensorCond)

  // -----   Default constructor   -------------------------------------------
  CbmStsParSensorCond::CbmStsParSensorCond()
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmStsParSensorCond::CbmStsParSensorCond(Double_t vFd, Double_t vBias, Double_t temperature, Double_t cCoupling,
                                         Double_t cInterstrip)
  : fVfd(vFd)
  , fVbias(vBias)
  , fTemperature(temperature)
  , fCcoupling(cCoupling)
  , fCinterstrip(cInterstrip)
{
  Init();
}
// -------------------------------------------------------------------------


// -----   Copy constructor   ----------------------------------------------
CbmStsParSensorCond::CbmStsParSensorCond(const CbmStsParSensorCond& other)
  : fVfd(other.fVfd)
  , fVbias(other.fVbias)
  , fTemperature(other.fTemperature)
  , fCcoupling(other.fCcoupling)
  , fCinterstrip(other.fCinterstrip)
{
  Init();
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsParSensorCond::~CbmStsParSensorCond() {}
// -------------------------------------------------------------------------


// -----  Calculate Hall mobility parameters   -----------------------------
void CbmStsParSensorCond::Init()
{

  // Cross-talk coefficient
  fCrossTalkCoeff = 0.;
  if (fCinterstrip + fCcoupling != 0.) fCrossTalkCoeff = fCinterstrip / (fCinterstrip + fCcoupling);

  // These are the parameters needed for the calculation of the Hall
  // mobility, i.e. the mobility of charge carriers in the silicon
  // in the presence of a magnetic field. They depend on the temperature.
  // Values and formulae are taken from
  // V. Bartsch et al., Nucl. Instrum. Methods A 497 (2003) 389

  assert(fTemperature > 0.);

  // --- Electrons
  fMuLowE = 1417. * pow(fTemperature / 300., -2.2);  // cm^2 / (V s)
  fBetaE  = 1.109 * pow(fTemperature / 300., 0.66);
  fVsatE  = 1.07e7 * pow(fTemperature / 300., 0.87);  // cm / s
  fRhallE = 1.15;

  // --- Holes
  fMuLowH = 470.5 * pow(fTemperature / 300., -2.5);  // cm^2 / (V s)
  fBetaH  = 1.213 * pow(fTemperature / 300., 0.17);
  fVsatH  = 0.837e7 * pow(fTemperature / 300., 0.52);  // cm / s
  fRhallH = 0.7;

  fIsInit = kTRUE;
}
// -------------------------------------------------------------------------


// -----   Hall mobility   -------------------------------------------------
Double_t CbmStsParSensorCond::GetHallMobility(Double_t eField, Int_t chargeType) const
{

  assert(fIsInit);
  assert(chargeType == 0 || chargeType == 1);

  Double_t muLow = 0.;  // mobility at low electric field
  Double_t beta  = 0.;  // exponent
  Double_t vSat  = 0.;  // saturation velocity
  Double_t rHall = 0.;  // Hall scattering factor

  if (chargeType == 0) {  // electrons
    muLow = fMuLowE;
    beta  = fBetaE;
    vSat  = fVsatE;
    rHall = fRhallE;
  }       //? electron
  else {  // holes
    muLow = fMuLowH;
    beta  = fBetaH;
    vSat  = fVsatH;
    rHall = fRhallH;
  }  //? holes

  Double_t factor = pow(muLow * eField / vSat, beta);
  Double_t muHall = rHall * muLow / pow(1 + factor, 1. / beta);
  return muHall;
}
// -------------------------------------------------------------------------


// -----   Copy assignment operator   --------------------------------------
CbmStsParSensorCond& CbmStsParSensorCond::operator=(const CbmStsParSensorCond& other)
{
  fVfd         = other.fVfd;
  fTemperature = other.fTemperature;
  fCcoupling   = other.fCcoupling;
  fCinterstrip = other.fCinterstrip;
  Init();
  return *this;
}
// -------------------------------------------------------------------------


// -----   Set condition parameters   --------------------------------------
void CbmStsParSensorCond::SetParams(Double_t vFd, Double_t vBias, Double_t temperature, Double_t cCoupling,
                                    Double_t cInterstrip)
{
  fVfd         = vFd;
  fVbias       = vBias;
  fTemperature = temperature;
  fCcoupling   = cCoupling;
  fCinterstrip = cInterstrip;
  Init();
}
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
string CbmStsParSensorCond::ToString() const
{
  stringstream ss;
  if (fIsInit) {
    ss << "VFD " << fVfd << " V | V(bias) " << fVbias << " V | T " << fTemperature << " K | C(coupl.) " << fCcoupling
       << " pF | C(int.) " << fCinterstrip << " pF | cross-talk coeff. " << fCrossTalkCoeff;
  }
  else {
    ss << "VFD " << fVfd << " V | V(bias) " << fVbias << " V | T " << fTemperature << " K | C(coupl.) " << fCcoupling
       << " pF | C(int.) " << fCinterstrip << " pF | not initialised!";
  }
  return ss.str();
}
// -------------------------------------------------------------------------
