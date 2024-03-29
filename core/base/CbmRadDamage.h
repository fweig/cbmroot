/* Copyright (C) 2011-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmRadDamage.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 6 December 2011
 **/

#ifndef CBMRADDAMAGE_H
#define CBMRADDAMAGE_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t
#include <TObject.h>     // for TObject

#include <map>  // for map

/** @class CbmRadDamage CbmRadDamage.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 6 December 2011
 ** @brief Tool class to calculate the non-ionising radiation damage
 **
 ** This class provides some functionality to calculate the effects
 ** of non-ionising irradiation on Silicon.
 **/
class CbmRadDamage : public TObject {


public:
  /**   Constructor   **/
  CbmRadDamage();


  /**   Destructor   **/
  virtual ~CbmRadDamage();


  /** Get leakage current
   ** @param[in] fluence       Fluence [neq/cm^2]
   ** @param[in] volume        Volume [cm^3]
   ** @param[in] temperature   Temeprature [K]
   ** @return    Leakage current [A]
   **
   ** Calculates the leakage current in Silicon as function of irradiation 
   ** fluence, volume and temperature.
   **
   ** Formulae provided by S. Chatterji.
   **/
  Double_t GetLeakageCurrent(Double_t fluence, Double_t volume, Double_t temperatur);

  /**  Get NIEL factor
   *@param type   Particle Id (PDG code)
   *@param energy Energy in GeV
   *@return NIEL factor
   **
   ** Calculates the NIEL factor for a given particle type and energy
   ** from interpolation of the values tabulated in
   ** http://sesam.desy.de/members/gunnar/Si-dfuncs.htm
   **
   ** Will return 0 for particle types other than proton, neutron,
   ** pion or electron, or if the energy is below the lowest tabulated
   ** value. Will return an asymtotiv value if the energy is above the
   ** highest tabulted value.
   **/
  Double_t GetNiel(Int_t type, Double_t energy);


  /** Get full depletion voltage
   ** @param fluence  Fluence [neq/cm^2]
   ** @param d        Thickness [cm]
   ** @return Full depletion voltage [V]
   **
   ** Calculates the full depletion voltage of Silicon as function of
   ** non-ionising dose and thickness.
   **
   ** Formulae and values provided by S. Chatterji.
   **/
  Double_t GetVfd(Double_t fluence, Double_t d);


private:
  /** Tables of NIEL factors as fnct. of energy **/
  std::map<Double_t, Double_t> niel_neutron;
  std::map<Double_t, Double_t> niel_proton;
  std::map<Double_t, Double_t> niel_pion;
  std::map<Double_t, Double_t> niel_electron;


  /** Proportionality constant of leakage current and fluence **/
  Double_t fIAlpha;  //  [A/cm]


  /** Constants for temperature dependence of leakage current **/
  Double_t fEGap0;      // [eV]
  Double_t fEGapAlpha;  // [ev?k]
  Double_t fEGapBeta;   // [K]


  /** Constants for effective doping concentration
   ** as function of fluence
   **/
  Double_t fNeff0;   // [cm^-3]
  Double_t fNeffC;   // [cm^2]
  Double_t fNeffGc;  // [1/cm]


  /** Permittivity of Silicon **/
  Double_t fEpsilon;  // [F/cm]


  /** Initialisation
   **
   ** Reads in tabulated NIEL factors from file
   ** and sets proper constants for Silicon.
   **/
  void Init();

  /** Read data from file
   *@param file  File name (in directory input)
   *@param map   Map to be filled
   **/
  void ReadData(const char* file, std::map<Double_t, Double_t>& map);


  ClassDef(CbmRadDamage, 1);
};

#endif
