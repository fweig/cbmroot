/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

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


// -----   Constructor   ---------------------------------------------------
CbmStsParAsic::CbmStsParAsic(UShort_t nChannels, UShort_t nAdc, double dynRange, double threshold, double timeResol,
                             double deadTime, double noise, double znr)
{
  Set(nChannels, nAdc, dynRange, threshold, timeResol, deadTime, noise, znr);
}
// -------------------------------------------------------------------------


// -----   Copy constructor   ----------------------------------------------
CbmStsParAsic::CbmStsParAsic(const CbmStsParAsic& other)
{
  Set(other.fNofChannels, other.fNofAdc, other.fDynRange, other.fThreshold, other.fTimeResolution, other.fDeadTime,
      other.fNoise, other.fZeroNoiseRate);
  SetTimeOffset(other.fTimeOffset);
  SetWalkCoef(other.fWalkCoef);
}
// -------------------------------------------------------------------------


// -----   Copy assignment operator   --------------------------------------
CbmStsParAsic& CbmStsParAsic::operator=(const CbmStsParAsic& other)
{
  Set(other.fNofChannels, other.fNofAdc, other.fDynRange, other.fThreshold, other.fTimeResolution, other.fDeadTime,
      other.fNoise, other.fZeroNoiseRate);
  SetTimeOffset(other.fTimeOffset);
  SetWalkCoef(other.fWalkCoef);
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
int16_t CbmStsParAsic::ChargeToAdc(double charge) const
{
  if (charge < fThreshold) return -1;                        // Underflow
  if (charge >= fThreshold + fDynRange) return fNofAdc - 1;  // Overflow
  return int16_t((charge - fThreshold) / fDynRange * double(fNofAdc));
}
// -------------------------------------------------------------------------


// -----   Deactivate channels   -------------------------------------------
uint16_t CbmStsParAsic::DeactivateRandomChannels(double fraction)
{

  if (fraction < 0.) return 0;

  // --- Average number of dead channels
  double meanDead = fraction * double(fNofChannels);
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
double CbmStsParAsic::GetNoiseRate() const
{
  if (fNoise == 0.) return 0.;
  double ratio = fThreshold / fNoise;
  return 0.5 * fZeroNoiseRate * TMath::Exp(-0.5 * ratio * ratio);
}
// -------------------------------------------------------------------------


// -----   Random charge of a noise signal   -------------------------------
double CbmStsParAsic::GetRandomNoiseCharge() const
{
  assert(fIsInit);
  return fNoiseCharge->GetRandom();
}
// -------------------------------------------------------------------------


// -----   Initialise the noise charge distribution   ----------------------
void CbmStsParAsic::Init()
{
  if (fNoiseCharge) delete fNoiseCharge;
  fNoiseCharge = new TF1("Noise Charge", "TMath::Gaus(x, [0], [1])", fThreshold, 10. * fNoise);
  fNoiseCharge->SetParameters(0., fNoise);
  fIsInit = kTRUE;
}
// -------------------------------------------------------------------------


// -----   Set the parameters   ---------------------------------------------
void CbmStsParAsic::Set(UShort_t nChannels, UShort_t nAdc, double dynRange, double threshold, double timeResol,
                        double deadTime, double noise, double zeroNoiseRate, std::set<UShort_t> deadChannels)
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


ClassImp(CbmStsParAsic)
