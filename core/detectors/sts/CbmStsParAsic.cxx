/** @file CbmStsParAsic.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.03.2020
 **/
#include "CbmStsParAsic.h"

#include <TF1.h>    // for TF1
#include <TMath.h>  // for Exp
#include <TRandom.h>

#include <sstream>  // for operator<<, basic_ostream, stringstream

#include <assert.h>  // for assert

ClassImp(CbmStsParAsic)

  // -----   Constructor   ---------------------------------------------------
  CbmStsParAsic::CbmStsParAsic(UShort_t nChannels, UShort_t nAdc, Double_t dynRange, Double_t threshold,
                               Double_t timeResol, Double_t deadTime, Double_t noise, Double_t znr)
{
  Set(nChannels, nAdc, dynRange, threshold, timeResol, deadTime, noise, znr);
}
// -------------------------------------------------------------------------


// -----   Copy constructor   ----------------------------------------------
CbmStsParAsic::CbmStsParAsic(const CbmStsParAsic& other)
{
  Set(other.fNofChannels, other.fNofAdc, other.fDynRange, other.fThreshold, other.fTimeResolution, other.fDeadTime,
      other.fNoise, other.fZeroNoiseRate);
}
// -------------------------------------------------------------------------


// -----   Copy assignment operator   --------------------------------------
CbmStsParAsic& CbmStsParAsic::operator=(const CbmStsParAsic& other)
{
  Set(other.fNofChannels, other.fNofAdc, other.fDynRange, other.fThreshold, other.fTimeResolution, other.fDeadTime,
      other.fNoise, other.fZeroNoiseRate);
  return *this;
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsParAsic::~CbmStsParAsic()
{
  if (fNoiseCharge) delete fNoiseCharge;
}
// -------------------------------------------------------------------------


// -----  ADC channel from charge   ----------------------------------------
Short_t CbmStsParAsic::ChargeToAdc(Double_t charge) const
{
  if (charge < fThreshold) return -1;                        // Underflow
  if (charge >= fThreshold + fDynRange) return fNofAdc - 1;  // Overflow
  return Short_t((charge - fThreshold) / fDynRange * Double_t(fNofAdc));
}
// -------------------------------------------------------------------------


// -----   Deactivate channels   -------------------------------------------
UInt_t CbmStsParAsic::DeactivateRandomChannels(Double_t fraction)
{

  if (fraction < 0.) return 0;

  // --- Average number of dead channels
  Double_t meanDead = fraction * Double_t(fNofChannels);
  if (meanDead > fNofChannels) meanDead = fNofChannels;

  // --- Sample actual number of dead channels from Poissonian
  Int_t nDead = gRandom->Poisson(meanDead);

  // --- Deactivate the given number of channels
  Int_t nDeactivated = 0;
  while (nDeactivated < nDead) {
    Int_t channel = Int_t(gRandom->Uniform(0, fNofChannels));
    if (IsChannelActive(channel)) {
      fDeadChannels.insert(channel);
      nDeactivated++;
    }  //? Channel was active
  }    //# Deactivated channels

  assert(nDeactivated == nDead);
  return nDead;
}
// -------------------------------------------------------------------------


// -----   Single-channel noise rate   -------------------------------------
Double_t CbmStsParAsic::GetNoiseRate() const
{
  if (fNoise == 0.) return 0.;
  Double_t ratio = fThreshold / fNoise;
  return 0.5 * fZeroNoiseRate * TMath::Exp(-0.5 * ratio * ratio);
}
// -------------------------------------------------------------------------


// -----   Random charge of a noise signal   -------------------------------
Double_t CbmStsParAsic::GetRandomNoiseCharge() const
{
  assert(fIsInit);
  return fNoiseCharge->GetRandom();
}
// -------------------------------------------------------------------------


// -----   Intialise the noise charge distribution   -----------------------
void CbmStsParAsic::Init()
{
  if (fNoiseCharge) delete fNoiseCharge;
  fNoiseCharge = new TF1("Noise Charge", "TMath::Gaus(x, [0], [1])", fThreshold, 10. * fNoise);
  fNoiseCharge->SetParameters(0., fNoise);
  fIsInit = kTRUE;
}
// -------------------------------------------------------------------------


// -----   Set the parameters   ---------------------------------------------
void CbmStsParAsic::Set(UShort_t nChannels, UShort_t nAdc, Double_t dynRange, Double_t threshold, Double_t timeResol,
                        Double_t deadTime, Double_t noise, Double_t zeroNoiseRate, std::set<UShort_t> deadChannels)
{

  // Assert validity of parameters
  assert(dynRange > 0.);
  assert(threshold > 0.);
  assert(timeResol > 0.);
  assert(deadTime >= 0.);
  assert(noise >= 0.);
  assert(zeroNoiseRate >= 0.);

  fNofChannels    = nChannels;
  fNofAdc         = nAdc;
  fDynRange       = dynRange;
  fThreshold      = threshold;
  fTimeResolution = timeResol;
  fDeadTime       = deadTime;
  fNoise          = noise;
  fZeroNoiseRate  = zeroNoiseRate;
  fDeadChannels   = deadChannels;

  Init();
}
// --------------------------------------------------------------------------


// ----- String output   ----------------------------------------------------
std::string CbmStsParAsic::ToString() const
{
  std::stringstream ss;
  ss << "nAdc " << fNofAdc << " | dynRange " << fDynRange << " e | thresh. " << fThreshold << " e | tResol "
     << fTimeResolution << " ns | deadTime " << fDeadTime << " ns | noise " << fNoise << " e | ZNR " << fZeroNoiseRate
     << "/ns | SCNR " << GetNoiseRate() << "/ns";
  return ss.str();
}
// --------------------------------------------------------------------------
