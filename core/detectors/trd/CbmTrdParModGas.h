/** @file CbmTrdParModGas.h
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator], Alexandru Bercuci **/

#ifndef CBMTRDPARMODGAS_H
#define CBMTRDPARMODGAS_H

#include "CbmTrdParMod.h"  // for CbmTrdParMod

#include <Rtypes.h>      // for CLRBIT, SETBIT, TESTBIT, THashConsistencyH...
#include <RtypesCore.h>  // for Double_t, Int_t, Float_t, Char_t, UShort_t
#include <TString.h>     // for TString

class TDirectory;
class TH2F;

#define NSHELLS 3  // no of atomic shells to be considered in the PE effect

/** \brief Definition of gas parameters for one TRD module **/
class CbmTrdParModGas : public CbmTrdParMod {
public:
  enum CbmTrdParModGasDef
  {
    kDetType = 0 /** Detector type (GSI=0, MB=1) */
      ,
    kNobleGasType /** Type of noble gas (Xe=0, Ar=1)*/
      ,
    kPID /** PID Method (ANN=0, Likelihood=1)*/
  };
  CbmTrdParModGas(const char* title = "TRD gas properties definition");
  virtual ~CbmTrdParModGas();

  Double_t GetCO2() const { return fPercentCO2; }
  const Char_t* GetDetName() const { return (GetDetType() ? "MB" : "GSI"); }
  Int_t GetDetType() const { return TESTBIT(fConfig, kDetType); }
  Double_t GetDriftTime(Double_t y0, Double_t z0) const;
  void GetElectricPotential(Int_t& ua, Int_t& ud) const
  {
    ua = fUa;
    ud = fUd;
  }
  /** \brief Get binding energy for the working gas
   * \param[in] shell shell id in capitals e.g. K, L, M ... \sa GetPEshell()
   * \param[in] main switch between main and escape peak [main=true]
   */
  Float_t GetBindingEnergy(const Char_t shell = 'K', Bool_t main = kTRUE) const;
  /** \brief Convert Energy debposit in keV to pad-plane charge taking into account the gas gain
   * \param[in] ekev Energy deposit in keV
   * \return charge at FASP input in fC
   */
  Float_t EkevFC(Float_t ekev) const;
  TString GetFileName() const { return fFileNamePID; }
  Double_t GetNobleGas() const { return 1. - fPercentCO2; }
  const Char_t* GetNobleGasName() const { return (GetNobleGasType() - 1 ? "Ar" : "Xe"); }
  Int_t GetNobleGasType() const { return TESTBIT(fConfig, kNobleGasType) ? 2 : 1; }
  /** \brief Get branching ration for radiative process on the
   * \param[in] shell shell id in capitals e.g. K, L, M ... \sa GetPEshell()
   */
  Float_t GetNonIonizingBR(const Char_t shell = 'K') const;
  /** \brief Get first atomic shell which can be excited by PE effect
   * \param[in] Ex energy of the incident X [keV]
   * \return atomic shell name
   */
  Char_t GetPEshell(Float_t Ex) const;
  Int_t GetPidType() const { return TESTBIT(fConfig, kPID); }
  /** \brief Get atomic shell index
   * \param[in] shell shell name
   */
  Int_t GetShellId(const Char_t shell) const;
  UShort_t GetUanode() const { return fUa; }
  UShort_t GetUdrift() const { return fUd; }
  virtual void Print(Option_t* opt = "") const;
  /**
   * \brief Get prompt signal for track segment in the y-z plane
   * \param y0 entry point on y in amplification cell coordinates (+- anode pitch/2)
   * \param z0 entry point on z (+- chmb_height/2)
   * \param dzdy Track angle in the y-z plane
   * \param dy track span on y
   * \sa CbmTrdModuleSimT::ScanPadPlane() CbmTrdModuleSimT::AddDigi()
   * \author A.Bercuci <abercuci@niham.nipne.ro>
   **/
  Double_t ScanDriftTime(Double_t y0, Double_t z0, Double_t dzdy, Double_t dy) const;

  /**
   * \brief Load drift map for current settings from repository
   * \param[in] hm histogram obtained as interpolation
   * \param[in] d ROOT directory of CbmRoot
   * \sa CbmTrdParSetGas::GetDriftMap()
   **/
  void SetDriftMap(TH2F* hm, TDirectory* d);
  void SetDw(Double_t dw) { fDw = dw; }
  void SetGasThick(Double_t sz) { fGasThick = sz; }
  void SetDetType(Int_t gsi = 0) { gsi ? SETBIT(fConfig, kDetType) : CLRBIT(fConfig, kDetType); }
  void SetCO2(Double_t p) { fPercentCO2 = p; }
  void SetFileName(const Char_t* fn) { fFileNamePID = fn; }
  void SetNobleGas(Double_t p) { fPercentCO2 = 1 - p; }
  void SetNobleGasType(Int_t ar = 1) { ar ? SETBIT(fConfig, kNobleGasType) : CLRBIT(fConfig, kNobleGasType); }
  void SetPidType(Int_t like = 1) { like ? SETBIT(fConfig, kPID) : CLRBIT(fConfig, kPID); }
  void SetElectricPotential(Int_t ua, Int_t ud)
  {
    fUa = ua;
    fUd = ud;
  }

private:
  CbmTrdParModGas(const CbmTrdParModGas& ref);
  const CbmTrdParModGas& operator=(const CbmTrdParModGas& ref);

  Int_t fConfig;         ///< config TRD gas see CbmTrdParModGasDef
  UShort_t fUa;          ///< anode voltage
  UShort_t fUd;          ///< drift voltage
  Double_t fDw;          ///< anode wire pitch
  Double_t fGasThick;    ///< gas thickness
  Double_t fPercentCO2;  ///< Percentage of CO2
  TH2F* fDriftMap;       ///< drift time map for one amplification cell
  TString fFileNamePID;  ///< filename for PID database

  static Float_t fgkBindingEnergy[2][NSHELLS];  ///< binding energy in keV for first atomic shells of Ar and Xe
  static Float_t fgkBR[2][NSHELLS - 1];         ///< branching ratio for non-ionizing decay of Ar and Xe
  static Float_t fgkWi[3];                      ///< average energy to produce one electron-ion pair for ar, xe and co2
  static Float_t fgkGGainUaPar[2];              ///< gas gaian parametrization on Ua for Ar on Buch detector
  static Float_t fgkE0;                         ///< min energy [ADC ch] which can be measured
  ClassDef(CbmTrdParModGas,
           1)  // Definition of gas parameters for one TRD module
};

#endif
