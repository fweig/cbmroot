/* Copyright (C) 2018-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci, Florian Uhlig [committer] */

#include "CbmTrdParModGas.h"

#include <Logger.h>  // for Logger, LOG

#include <TAxis.h>              // for TAxis
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TH2.h>                // for TH2F
#include <TMath.h>              // for Exp
#include <TMathBase.h>          // for Abs
#include <TObjArray.h>          // for TObjArray
#include <TObjString.h>         // for TObjString

#include <stdio.h>  // for printf

#define VERBOSE 0

// binding energy in keV for 'K' 'L' and 'M' shells of Ar and Xe
Float_t CbmTrdParModGas::fgkBindingEnergy[2][NSHELLS] = {
  {34.5, 5.1, 1.1},  // Xe
  {3.2, 0.25, 0.1}   // Ar
};
// binding energy in keV for 'K' 'L' and 'M' shells of Ar and Xe
Float_t CbmTrdParModGas::fgkBR[2][NSHELLS - 1] = {
  {0.11, 0.02},  // Xe
  {0.11, 0.02}   // Ar
};
// average energy to produce one electron-ion pair
Float_t CbmTrdParModGas::fgkWi[3] = {
  23.1,  // Xe
  25.8,  // Ar
  33     // CO2
};
// Bucharest detector gas gain parametrization based on 55Fe measurements with ArCO2(80/20)
Float_t CbmTrdParModGas::fgkGGainUaPar[2] = {-10.1676, 8.3745};
Float_t CbmTrdParModGas::fgkE0            = 866.1047;  // energy offset in ADC ch @ 0 keV

//___________________________________________________________________
CbmTrdParModGas::CbmTrdParModGas(const char* title)
  : CbmTrdParMod("CbmTrdParModGas", title)
  , fConfig(0)
  , fUa(0)
  , fUd(0)
  , fDw(0.3)
  , fGasThick(0.6)
  , fPercentCO2(0.2)
  , fDriftMap(nullptr)
  , fFileNamePID("parameters/trd/Likelihood_Xenon_85_v20b_tdr18_apr21.root")
{
  TString s(title);
  TString name;
  //  Int_t val;
  TObjArray* so = s.Tokenize("/");
  for (Int_t ie(0); ie < so->GetEntriesFast(); ie += 2) {
    name = ((TObjString*) (*so)[ie])->String();
    if (name.EqualTo("Module")) fModuleId = ((TObjString*) (*so)[ie + 1])->String().Atoi();
    else if (name.EqualTo("Ua"))
      fUa = ((TObjString*) (*so)[ie + 1])->String().Atoi();
    else if (name.EqualTo("Ud"))
      fUd = ((TObjString*) (*so)[ie + 1])->String().Atoi();
    else if (name.EqualTo("Gas")) {
      TString gas = ((TObjString*) (*so)[ie + 1])->String();
      if (gas.EqualTo("Ar")) SetNobleGasType(1);
      else if (gas.EqualTo("Xe"))
        SetNobleGasType(0);
      else {
        LOG(warn) << GetName() << ":: gas type \"" << gas << "\" not defined. Default to Xe.";
        SetNobleGasType(0);
      }
    }
  }
  so->Delete();
  delete so;

  if (VERBOSE) printf("Module[%2d] U[%4d %3d]\n", fModuleId, fUa, fUd);
}

//___________________________________________________________________
CbmTrdParModGas::~CbmTrdParModGas()
{
  //  if(fDriftMap) delete fDriftMap;
}

//_______________________________________________________________________________________________
Float_t CbmTrdParModGas::EkevFC(Float_t ekev) const
{
  // Convert energy deposit to no of primary ionizations and apply gas gain.
  // Currently gas gain is evalauted from 55Fe spectrum analysis on ArCO2(80/20)
  Int_t gasId = GetNobleGasType() - 1;
  Float_t wi  = (1. - fPercentCO2) * fgkWi[gasId] + fPercentCO2 * fgkWi[2];

  //gas gain
  // G = G[ev->ADC] * wi[ArCO2]/C[mV->ADC]/A[fC->mV]/e
  // G[ev->ADC] : measured gain based on 55Fe spectrum (expo)
  // wi[ArCO2]  : average energy to produce a ele-ion pair in mixture (27.24 ev)
  // C[mV->ADC] : FASP out [2V] to ADC range [4096 ch] (2)
  // A[fC->mV]  : FASP gain from CADENCE (6)
  // e : 1.6e-4 [fC] electric charge
  Double_t gain = 170.25 * TMath::Exp(fgkGGainUaPar[0] + fgkGGainUaPar[1] * fUa * 1.e-3) / 12.;
  // for Xe correct Ar gain measurements TODO
  if (gasId == 0) gain *= 0.6;

  Float_t efC = gain * ekev * 0.16 / wi;
  if (VERBOSE)
    printf("        ua[V]=%d gain[%5.2e] wi[eV]=%5.2f :: E[keV]=%6.3f E[fC]=%6.2f\n", fUa, gain, wi, ekev, efC);
  return efC;

  //   Double_t eadc = fgkE0 + gain * ekev;
  //   //printf("E = %fkeV %fADC\n", ekev, eadc);
  //   // apply FASP gain -> should be done without intermediate ADC conversion TODO
  //   Double_t sFASP = eadc/2.; // FASP signal [mV]; FASP amplification 1
  //   // FASP gaincharacteristic -> should be defined elsewhere
  //   // data based on CADENCE simulations
  //   Double_t s0FASP = 10, gFASP = 6;
  //   return (sFASP-s0FASP)/gFASP;
}

//_______________________________________________________________________________________________
Float_t CbmTrdParModGas::EfCkeV(Float_t efC) const
{
  /** Convert energy deposit to no of primary ionisations and apply gas gain.
  * Currently gas gain is evaluated from 55Fe spectrum analysis on ArCO2(80/20)
  */

  Int_t gasId = GetNobleGasType() - 1;
  Float_t wi  = (1. - fPercentCO2) * fgkWi[gasId] + fPercentCO2 * fgkWi[2];

  //gas gain
  // G = G[ev->ADC] * wi[ArCO2]/C[mV->ADC]/A[fC->mV]/e
  // G[ev->ADC] : measured gain based on 55Fe spectrum (expo)
  // wi[ArCO2]  : average energy to produce a ele-ion pair in mixture (27.24 ev)
  // C[mV->ADC] : FASP out [2V] to ADC range [4096 ch] (2)
  // A[fC->mV]  : FASP gain from CADENCE (6)
  // e : 1.6e-4 [fC] electric charge
  Double_t gain = 170.25 * TMath::Exp(fgkGGainUaPar[0] + fgkGGainUaPar[1] * fUa * 1.e-3)
                  / 12.;  // for Xe correct Ar gain measurements TODO
  if (gasId == 0) gain *= 0.6;

  Float_t ekev = efC * wi / gain / 0.16;
  if (VERBOSE)
    printf("        ua[V]=%d gain[%5.2e] wi[eV]=%5.2f :: E[keV]=%6.3f E[fC]=%6.2f\n", fUa, gain, wi, ekev, efC);
  return ekev;
}

//_______________________________________________________________________________________________
Int_t CbmTrdParModGas::GetShellId(const Char_t shell) const
{
  /** Return index of atomic shell. 
 * shell name can be 'K', 'L' and 'M'
 */
  switch (shell) {
    case 'K': return 0;
    case 'L': return 1;
    case 'M': return 2;
    default:
      LOG(warn) << GetName() << "::GetShellId: Atomic shell : " << shell << " not defined for gas "
                << (GetNobleGasType() == 2 ? "Ar" : "Xe");
      return -1;
  }
}

//_______________________________________________________________________________________________
Float_t CbmTrdParModGas::GetBindingEnergy(const Char_t shell, Bool_t main) const
{
  Int_t gasId   = GetNobleGasType() - 1;
  Int_t shellId = GetShellId(shell);
  if (shellId < 0) return 0;

  if (!main) return fgkBindingEnergy[gasId][shellId];
  else {
    if (shellId < NSHELLS - 1) return fgkBindingEnergy[gasId][shellId + 1];
    else {
      LOG(warn) << GetName() << "::GetBindingEnergy: Request atomic shell : " << shellId + 1 << " not defined for gas "
                << (gasId ? "Ar" : "Xe");
      return 0;
    }
  }
  return 0;
}

//_______________________________________________________________________________________________
Float_t CbmTrdParModGas::GetNonIonizingBR(const Char_t shell) const
{
  Int_t gasId   = GetNobleGasType() - 1;
  Int_t shellId = GetShellId(shell);
  if (shellId < 0) return 0;

  return fgkBR[gasId][shellId];
}

//_______________________________________________________________________________________________
Char_t CbmTrdParModGas::GetPEshell(Float_t Ex) const
{
  const Char_t shellName[NSHELLS] = {'K', 'L', 'M'};
  Int_t gasId                     = GetNobleGasType() - 1;
  for (Int_t ishell(0); ishell < NSHELLS; ishell++) {
    if (Ex < fgkBindingEnergy[gasId][ishell]) continue;
    return shellName[ishell];
  }
  LOG(debug) << GetName() << "::GetPEshell: Ex[keV] " << Ex
             << " less than highes atomic shell binding energy : " << fgkBindingEnergy[gasId][NSHELLS - 1]
             << " for gas " << (gasId ? "Ar" : "Xe");
  return 0;
}

//_______________________________________________________________________________________________
Double_t CbmTrdParModGas::GetDriftTime(Double_t y0, Double_t z0) const
{
  const TAxis *ay(fDriftMap->GetXaxis()), *az(fDriftMap->GetYaxis());
  Int_t by(ay->FindBin(y0)), bz(az->FindBin(z0));
  Double_t tmin(fDriftMap->GetBinContent(by, bz));
  if (VERBOSE) printf("GetDriftTime :: Start @ dt=%3d [ns]\n", Int_t(tmin));
  return tmin;
}

//_______________________________________________________________________________________________
void CbmTrdParModGas::Print(Option_t* /*opt*/) const
{
  printf("%s @ %4d ", GetName(), fModuleId);
  printf("Type[%s] ", GetDetName());
  printf("%s[%4.1f%%] Ua[V]=%d Ud[V]=%d ", GetNobleGasName(), 1e2 * GetNobleGas(), fUa, fUd);
  printf("Pid Type[%d] DB[%s]\n", GetPidType(), fFileNamePID.Data());
}

//_______________________________________________________________________________________________
Double_t CbmTrdParModGas::ScanDriftTime(Double_t y0, Double_t z0, Double_t dzdy, Double_t dy) const
{
  Double_t y1 = y0 + dy, z1 = z0 + dzdy * dy, dw(fDw), dwh(0.5 * dw);
  //  Double_t dhh(fGasThick);

  if (VERBOSE)
    printf("ScanDriftTime :: Try : [%7.4f %7.4f] => [%7.4f %7.4f] dzdy[%5.2f] "
           "dy[%5.2f]\n",
           y0, z0, y1, z1, dzdy, dy);
  while (y1 < -dwh - 1e-3) {
    y0 += dw;
    y1 += dw;
  }
  while (y1 > dwh + 1.e-3) {
    y0 -= dw;
    y1 -= dw;
  }
  TH2F* h = fDriftMap;
  y1      = y0;
  z1      = z0;
  const TAxis *ay(fDriftMap->GetXaxis()), *az(fDriftMap->GetYaxis());
  Int_t by(ay->FindBin(y1)), bz(az->FindBin(z1)), nby(ay->GetNbins()), nbz(az->GetNbins());
  Float_t dyStep = ay->GetBinWidth(1), tmin(500), tmax(0), tc(0);
  while (by > 0 && by <= nby && bz <= nbz) {
    bz = az->FindBin(z1);
    tc = h->GetBinContent(by, bz);
    //if(VERBOSE) printf("ScanDriftTime :: Do  : y0(%7.4f), z0(%7.4f), by(%3d), bz(%3d) td[ns]=%5.1f  t0[ns]=%5.1f \n", y1, z1, by, bz, tc, tmin);

    if (tc > 0) {
      if (tc < tmin) tmin = tc;
      if (tc > tmax) tmax = tc;
    }
    z1 += TMath::Abs(dzdy) * dyStep;
    if (dzdy > 0) {
      y1 += dyStep;
      by++;
    }
    else {
      y1 -= dyStep;
      by--;
    }
  }
  if (VERBOSE) printf("ScanDriftTime :: Start @ dt=%3d [ns]\n", Int_t(tmin));

  return tmin;
}

//___________________________________________________________________
void CbmTrdParModGas::SetDriftMap(TH2F* hm, TDirectory* d)
{
  /**  
 * Load drift map in the module and get ownership
 */

  if (VERBOSE) printf("CbmTrdParModGas::SetDriftMap : Module[%2d] U[%4d %3d]\n", fModuleId, fUa, fUd);

  fDriftMap = (TH2F*) hm->Clone(Form("trdDM%02d", fModuleId));
  fDriftMap->SetTitle(GetTitle());
  fDriftMap->SetDirectory(d);
}

ClassImp(CbmTrdParModGas)
