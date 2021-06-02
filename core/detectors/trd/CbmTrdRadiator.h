/** @file CbmTrdRadiator.h
  * @copyright Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors M.Kalisky, Adrian Meyer-Ahrens, Denis Bertini [committer], Cyrano Bergmann, Florian Uhlig **/

// -------------------------------------------------------------------------
// -----                         CbmTrdRadiator header file       -----
// -----                  Created 10/11/04  by M.Kalisky         -----
// -------------------------------------------------------------------------

/**  CbmTrdRadiator.h
 *@author M.Kalisky <m.kalisky@gsi.de>
 **
 ** Serves for Transition Radiation computation for regular radiator
 ** The output is energy loss of an electron via Transition Radiation
 **/

#ifndef CBMTRDRADIATOR_H
#define CBMTRDRADIATOR_H
#define NCOMPONENTS 10

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Float_t, Int_t, Bool_t, Double_t
#include <TString.h>     // for TString
#include <TVector3.h>    // for TVector3

class CbmTrdPoint;
class TH1D;
typedef Float_t (*GetMuPtr)(Float_t);
class CbmTrdRadiator {

public:
  struct CbmTrdEntranceWindow {
    CbmTrdEntranceWindow(TString def = "");
    Bool_t Init(TString def);

    Int_t fN;                     // no of components
    TString fMat[NCOMPONENTS];    // components' names
    Float_t fThick[NCOMPONENTS];  // thickness [cm] of window components
    Float_t fDens[NCOMPONENTS];   // density [g/cm^3] of window components
    GetMuPtr GetMu[NCOMPONENTS];  // array of mu=f(E) for each component
  };

  /** Default constructor **/
  CbmTrdRadiator(Bool_t SimpleTR = true, Int_t Nfoils = 337, Float_t FoilThick = 0.0012, Float_t GapThick = 0.09,
                 TString material = "pefoam20", TString window = "Kapton");

  /** Constructor using predefined radiator prototype **/
  CbmTrdRadiator(Bool_t SimpleTR, TString prototype, TString window = "Kapton");
  /* // implemented prototypes are:
     "tdr18"		153x2mm PE foam foil layer (146 layers in box, 7 layers in carbon grid)       
     "B++"		POKALON 24µm, 0.7mm gap,  350 foils
     "K++"		POKALON 24µm, 0.7mm gap,  350 foils micro-structured
     "G30"		30xfiber layer
    
     
     For the entrance window the general syntax would be c0;c1;c2;...
     where ci are planar components. e.g. Al;C;Air;C;Al is the TRD-2D sandwich
     There are two shortcuts for the Munster detector namely:
     "Mylar" - simple mylar window
     "Kapton" - Al(50nm)kapton(25µm )
  */

  /** Destructor **/
  virtual ~CbmTrdRadiator();

  /** Create the needed histograms **/
  void CreateHistograms();

  /** Init function **/
  void Init();

  /** Spectra production **/
  void ProduceSpectra();

  /** Final energy loss computation **/
  Int_t ELoss(Int_t index);

  /** Main function for TR production **/
  void ProcessTR();

  /** Compute the TR spectrum **/
  Int_t TRspectrum();

  /** Compute the TR spectrum in the Window foil **/
  Int_t WinTRspectrum();

  /** Compute the TR spectrum in the Detector **/
  Int_t DetTRspectrum();

  /** Compute the gamma factor **/
  Float_t GammaF();

  /** Compute the sigma coeff. for one-gap-one-foil radiator **/
  Float_t Sigma(Float_t energykeV);

  /** Compute the sigma coeff. for the gas-.window foil **/
  Float_t SigmaWin(Float_t energykeV);

  /** Compute the sigma coeff. for the detector gas **/
  Float_t SigmaDet(Float_t energykeV);

  TString fWindowFoil;

  /** Computation of photon absorption cross sections taken from http://physics.nist.gov/PhysRefData/Xcom/html/xcom1.html**/
  static Float_t GetMuAl(Float_t energyMeV);
  static Float_t GetMuPo(Float_t energyMeV);
  static Float_t GetMuPok(Float_t energyMeV);
  static Float_t GetMuKa(Float_t energyMeV);
  static Float_t GetMuAir(Float_t energyMeV);
  static Float_t GetMuXe(Float_t energyMeV);
  static Float_t GetMuCO2(Float_t energyMeV);
  static Float_t GetMuMy(Float_t energyMeV);
  static Float_t GetMuC(Float_t energyMeV);

  /** Calculate the TR for a given momentum **/
  Float_t GetTR(TVector3 mom);

  /** Interpolate between given points of table **/
  static Float_t Interpolate(Float_t energyMeV, const Float_t* en, const Float_t* mu, Int_t n);

  /** Locate a point in 1-dim grid **/
  static Int_t Locate(const Float_t* xv, Int_t n, Float_t xval, Int_t& kl, Float_t& dx);


  /** Setters for private data memebers **/
  void SetNFoils(Int_t n) { fNFoils = n; }
  void SetFoilThick(Float_t t) { fFoilThick = t; }
  void SetGapThick(Float_t t) { fGapThick = t; }
  void SetSigma(Int_t SigmaT);
  /** define plane-parallel Entrance Window section in [cm] **/
  void SetEWwidths(Int_t n, Float_t* w);

  Bool_t LatticeHit(const CbmTrdPoint* point);

private:
  CbmTrdRadiator& operator=(const CbmTrdRadiator&);
  CbmTrdRadiator(const CbmTrdRadiator&);

  /* Input parameters to be set */
  TString fRadType;
  Int_t fDetType;      // 0: GSI  1: Muenster-Bucarest
  Bool_t fFirstPass;   // used for MB-Chamber with two gas gaps
  Bool_t fSimpleTR;    // mode of the TR production
  Int_t fNFoils;       // Number of foils in the radiator stack
  Float_t fFoilThick;  // Thickness of the foils (cm)
  Float_t fGapThick;   // Thickness of gaps between the foils (cm)
  TString fFoilMaterial;
  Float_t fGasThick;  // Thickness of the active gas volume.


  /* Parameters fixed in Init() */

  Float_t fFoilDens;     // Density of the radiator foils (g/cm^3)
  Float_t fGapDens;      // Dens. of gas in the radiator gaps (g/cm^3)
  Float_t fFoilOmega;    // Plasma frequency of the radiator foils
  Float_t fGapOmega;     // Plasma freq. of gas
  Float_t fnPhotonCorr;  // Correction of number of photon production efficiency

  /* Parameters after correction of the angle of the particle */

  Float_t fFoilThickCorr;
  Float_t fGapThickCorr;
  Float_t fGasThickCorr;


  Float_t fnTRprod;  // <nTR> produced
  //  Int_t     fSigmaT;      // set the absorption material

  Float_t fWinDens;
  Float_t fWinThick;
  CbmTrdEntranceWindow WINDOW;  // generic window structure

  Float_t fCom1;  // first component of the gas
  Float_t fCom2;  // second component of the gas

  static const Int_t fSpNBins = 100;  // Number of Bins for histos
  static const Int_t fSpRange = 50;   // Maximum (keV) of eloss spectrum
  Float_t fSpBinWidth;                // Bin width=fSpNBins/fSpRange

  Float_t* fSigma;     //! [fSpNBins] Array of sigma values for the foil of the radiator
  Float_t* fSigmaWin;  //! [fSpNBins] Array of sigma values for the entrance window of detector
  Float_t* fSigmaDet;  //! [fSpNBins] Array of sigma values for the active gas

  TH1D* fSpectrum;      //! TR photon energy spectrum
  TH1D* fWinSpectrum;   //! TR spectra in gas-window foil
  TH1D* fDetSpectrumA;  //! TR absorbed in Detector
  TH1D* fDetSpectrum;   //! TR passed through Detector

  static const Int_t fNMom = 14;  // number of momentum spectra
  Double_t* fTrackMomentum;       //! [fNMom] Track momenta for which spectra
                                  // are available

  TH1D* fFinal[fNMom];     //! Absorption spectra for different momenta
  Float_t fnTRabs[fNMom];  // <nTR> absorbed for differnt momenta
  Float_t fnTRab;          // <nTR> absorbed

  Float_t fELoss;  // The real result - in GeV

  TVector3 fMom;  // momentum of the electron

  /* Photon absorption cross section for pokalon N470 */
  static const Int_t fKN_pok           = 46;
  static constexpr Float_t fEn_pok[46] = {
    1.000E-03, 1.500E-03, 2.000E-03, 3.000E-03, 4.000E-03, 5.000E-03, 6.000E-03, 8.000E-03, 1.000E-02, 1.500E-02,
    2.000E-02, 3.000E-02, 4.000E-02, 5.000E-02, 6.000E-02, 8.000E-02, 1.000E-01, 1.500E-01, 2.000E-01, 3.000E-01,
    4.000E-01, 5.000E-01, 6.000E-01, 8.000E-01, 1.000E+00, 1.022E+00, 1.250E+00, 1.500E+00, 2.000E+00, 2.044E+00,
    3.000E+00, 4.000E+00, 5.000E+00, 6.000E+00, 7.000E+00, 8.000E+00, 9.000E+00, 1.000E+01, 1.100E+01, 1.200E+01,
    1.300E+01, 1.400E+01, 1.500E+01, 1.600E+01, 1.800E+01, 2.000E+01};
  static constexpr Float_t fMu_pok[46] = {
    2.537E+03, 8.218E+02, 3.599E+02, 1.093E+02, 4.616E+01, 2.351E+01, 1.352E+01, 5.675E+00, 2.938E+00, 9.776E-01,
    5.179E-01, 2.847E-01, 2.249E-01, 2.003E-01, 1.866E-01, 1.705E-01, 1.600E-01, 1.422E-01, 1.297E-01, 1.125E-01,
    1.007E-01, 9.193E-02, 8.501E-02, 7.465E-02, 6.711E-02, 6.642E-02, 6.002E-02, 5.463E-02, 4.686E-02, 4.631E-02,
    3.755E-02, 3.210E-02, 2.851E-02, 2.597E-02, 2.409E-02, 2.263E-02, 2.149E-02, 2.056E-02, 1.979E-02, 1.916E-02,
    1.862E-02, 1.816E-02, 1.777E-02, 1.743E-02, 1.687E-02, 1.644E-02};

  /* Photon absorption cross section for kapton */
  static const Int_t fKN_ka           = 46;
  static constexpr Float_t fEn_ka[46] = {
    1.000E-03, 1.500E-03, 2.000E-03, 3.000E-03, 4.000E-03, 5.000E-03, 6.000E-03, 8.000E-03, 1.000E-02, 1.500E-02,
    2.000E-02, 3.000E-02, 4.000E-02, 5.000E-02, 6.000E-02, 8.000E-02, 1.000E-01, 1.500E-01, 2.000E-01, 3.000E-01,
    4.000E-01, 5.000E-01, 6.000E-01, 8.000E-01, 1.000E+00, 1.022E+00, 1.250E+00, 1.500E+00, 2.000E+00, 2.044E+00,
    3.000E+00, 4.000E+00, 5.000E+00, 6.000E+00, 7.000E+00, 8.000E+00, 9.000E+00, 1.000E+01, 1.100E+01, 1.200E+01,
    1.300E+01, 1.400E+01, 1.500E+01, 1.600E+01, 1.800E+01, 2.000E+01};
  static constexpr Float_t fMu_ka[46] = {
    2.731E+03, 8.875E+02, 3.895E+02, 1.185E+02, 5.013E+01, 2.555E+01, 1.470E+01, 6.160E+00, 3.180E+00, 1.043E+00,
    5.415E-01, 2.880E-01, 2.235E-01, 1.973E-01, 1.830E-01, 1.666E-01, 1.560E-01, 1.385E-01, 1.263E-01, 1.095E-01,
    9.799E-02, 8.944E-02, 8.270E-02, 7.262E-02, 6.529E-02, 6.462E-02, 5.839E-02, 5.315E-02, 4.561E-02, 4.507E-02,
    3.659E-02, 3.133E-02, 2.787E-02, 2.543E-02, 2.362E-02, 2.223E-02, 2.114E-02, 2.025E-02, 1.953E-02, 1.893E-02,
    1.842E-02, 1.799E-02, 1.762E-02, 1.730E-02, 1.678E-02, 1.638E-02};

  /* Photon absorption cross section for aluminum */
  static const Int_t fKN_al           = 48;
  static constexpr Float_t fEn_al[48] = {
    1.000E-03, 1.500E-03, 1.560E-03, 1.560E-03, 2.000E-03, 3.000E-03, 4.000E-03, 5.000E-03, 6.000E-03, 8.000E-03,
    1.000E-02, 1.500E-02, 2.000E-02, 3.000E-02, 4.000E-02, 5.000E-02, 6.000E-02, 8.000E-02, 1.000E-01, 1.500E-01,
    2.000E-01, 3.000E-01, 4.000E-01, 5.000E-01, 6.000E-01, 8.000E-01, 1.000E+00, 1.022E+00, 1.250E+00, 1.500E+00,
    2.000E+00, 2.044E+00, 3.000E+00, 4.000E+00, 5.000E+00, 6.000E+00, 7.000E+00, 8.000E+00, 9.000E+00, 1.000E+01,
    1.100E+01, 1.200E+01, 1.300E+01, 1.400E+01, 1.500E+01, 1.600E+01, 1.800E+01, 2.000E+01};
  static constexpr Float_t fMu_al[48] = {
    1.185E+03, 4.023E+02, 3.621E+02, 3.957E+03, 2.263E+03, 7.881E+02, 3.605E+02, 1.934E+02, 1.153E+02, 5.032E+01,
    2.621E+01, 7.955E+00, 3.442E+00, 1.128E+00, 5.684E-01, 3.681E-01, 2.778E-01, 2.018E-01, 1.704E-01, 1.378E-01,
    1.223E-01, 1.042E-01, 9.276E-02, 8.445E-02, 7.802E-02, 6.841E-02, 6.146E-02, 6.080E-02, 5.496E-02, 5.006E-02,
    4.324E-02, 4.277E-02, 3.541E-02, 3.106E-02, 2.836E-02, 2.655E-02, 2.529E-02, 2.437E-02, 2.369E-02, 2.318E-02,
    2.279E-02, 2.249E-02, 2.226E-02, 2.208E-02, 2.195E-02, 2.185E-02, 2.173E-02, 2.168E-02};

  /* Photon absorption cross section for polypropylene/polyethylene */
  static const Int_t fKN_po           = 36;
  static constexpr Float_t fEn_po[36] = {
    1.000E-03, 1.500E-03, 2.000E-03, 3.000E-03, 4.000E-03, 5.000E-03, 6.000E-03, 8.000E-03, 1.000E-02,
    1.500E-02, 2.000E-02, 3.000E-02, 4.000E-02, 5.000E-02, 6.000E-02, 8.000E-02, 1.000E-01, 1.500E-01,
    2.000E-01, 3.000E-01, 4.000E-01, 5.000E-01, 6.000E-01, 8.000E-01, 1.000E+00, 1.250E+00, 1.500E+00,
    2.000E+00, 3.000E+00, 4.000E+00, 5.000E+00, 6.000E+00, 8.000E+00, 1.000E+01, 1.500E+01, 2.000E+01};
  static constexpr Float_t fMu_po[36] = {
    1.894E+03, 5.999E+02, 2.593E+02, 7.743E+01, 3.242E+01, 1.643E+01, 9.432E+00, 3.975E+00, 2.088E+00,
    7.452E-01, 4.315E-01, 2.706E-01, 2.275E-01, 2.084E-01, 1.970E-01, 1.823E-01, 1.719E-01, 1.534E-01,
    1.402E-01, 1.217E-01, 1.089E-01, 9.947E-02, 9.198E-02, 8.078E-02, 7.262E-02, 6.495E-02, 5.910E-02,
    5.064E-02, 4.045E-02, 3.444E-02, 3.045E-02, 2.760E-02, 2.383E-02, 2.145E-02, 1.819E-02, 1.658E-02};

  /* Photon absorption cross section for carbon */
  static const Int_t fKN_c           = 46;
  static constexpr Float_t fEn_c[46] = {
    1.000E-03, 1.500E-03, 2.000E-03, 3.000E-03, 4.000E-03, 5.000E-03, 6.000E-03, 8.000E-03, 1.000E-02, 1.500E-02,
    2.000E-02, 3.000E-02, 4.000E-02, 5.000E-02, 6.000E-02, 8.000E-02, 1.000E-01, 1.500E-01, 2.000E-01, 3.000E-01,
    4.000E-01, 5.000E-01, 6.000E-01, 8.000E-01, 1.000E+00, 1.022E+00, 1.250E+00, 1.500E+00, 2.000E+00, 2.044E+00,
    3.000E+00, 4.000E+00, 5.000E+00, 6.000E+00, 7.000E+00, 8.000E+00, 9.000E+00, 1.000E+01, 1.100E+01, 1.200E+01,
    1.300E+01, 1.400E+01, 1.500E+01, 1.600E+01, 1.800E+01, 2.000E+01};
  static constexpr Float_t fMu_c[46] = {
    2.211e+03, 7.004e+02, 3.026e+02, 9.032e+01, 3.778e+01, 1.912e+01, 1.095e+01, 4.576e+00, 2.373e+00, 8.074e-01,
    4.420e-01, 2.562e-01, 2.076e-01, 1.871e-01, 1.753e-01, 1.610e-01, 1.514e-01, 1.347e-01, 1.229e-01, 1.066e-01,
    9.547e-02, 8.715e-02, 8.058e-02, 7.076e-02, 6.362e-02, 6.296e-02, 5.690e-02, 5.179e-02, 4.443e-02, 4.391e-02,
    3.563e-02, 3.047e-02, 2.708e-02, 2.469e-02, 2.291e-02, 2.154e-02, 2.047e-02, 1.959e-02, 1.888e-02, 1.828e-02,
    1.778e-02, 1.735e-02, 1.698e-02, 1.667e-02, 1.615e-02, 1.575e-02};

  /* Photon absorption cross section for air */
  static const Int_t fKN_air           = 38;
  static constexpr Float_t fEn_air[38] = {
    0.10000E-02, 0.15000E-02, 0.20000E-02, 0.30000E-02, 0.32029E-02, 0.32029E-02, 0.40000E-02, 0.50000E-02,
    0.60000E-02, 0.80000E-02, 0.10000E-01, 0.15000E-01, 0.20000E-01, 0.30000E-01, 0.40000E-01, 0.50000E-01,
    0.60000E-01, 0.80000E-01, 0.10000E+00, 0.15000E+00, 0.20000E+00, 0.30000E+00, 0.40000E+00, 0.50000E+00,
    0.60000E+00, 0.80000E+00, 0.10000E+01, 0.12500E+01, 0.15000E+01, 0.20000E+01, 0.30000E+01, 0.40000E+01,
    0.50000E+01, 0.60000E+01, 0.80000E+01, 0.10000E+02, 0.15000E+02, 0.20000E+02};
  static constexpr Float_t fMu_air[38] = {
    0.35854E+04, 0.11841E+04, 0.52458E+03, 0.16143E+03, 0.15722E+03, 0.14250E+03, 0.77538E+02, 0.40099E+02,
    0.23313E+02, 0.98816E+01, 0.51000E+01, 0.16079E+01, 0.77536E+00, 0.35282E+00, 0.24790E+00, 0.20750E+00,
    0.18703E+00, 0.16589E+00, 0.15375E+00, 0.13530E+00, 0.12311E+00, 0.10654E+00, 0.95297E-01, 0.86939E-01,
    0.80390E-01, 0.70596E-01, 0.63452E-01, 0.56754E-01, 0.51644E-01, 0.44382E-01, 0.35733E-01, 0.30721E-01,
    0.27450E-01, 0.25171E-01, 0.22205E-01, 0.20399E-01, 0.18053E-01, 0.18057E-01};

  /* Photon absorption cross section for xenon */
  static const Int_t fKN_xe           = 48;
  static constexpr Float_t fEn_xe[48] = {
    1.00000E-03, 1.07191E-03, 1.14900E-03, 1.14900E-03, 1.50000E-03, 2.00000E-03, 3.00000E-03, 4.00000E-03,
    4.78220E-03, 4.78220E-03, 5.00000E-03, 5.10370E-03, 5.10370E-03, 5.27536E-03, 5.45280E-03, 5.45280E-03,
    6.00000E-03, 8.00000E-03, 1.00000E-02, 1.50000E-02, 2.00000E-02, 3.00000E-02, 3.45614E-02, 3.45614E-02,
    4.00000E-02, 5.00000E-02, 6.00000E-02, 8.00000E-02, 1.00000E-01, 1.50000E-01, 2.00000E-01, 3.00000E-01,
    4.00000E-01, 5.00000E-01, 6.00000E-01, 8.00000E-01, 1.00000E+00, 1.25000E+00, 1.50000E+00, 2.00000E+00,
    3.00000E+00, 4.00000E+00, 5.00000E+00, 6.00000E+00, 8.00000E+00, 1.00000E+01, 1.50000E+01, 2.00000E+01};
  static constexpr Float_t fMu_xe[48] = {
    9.413E+03, 8.151E+03, 7.035E+03, 7.338E+03, 4.085E+03, 2.088E+03, 7.780E+02, 3.787E+02, 2.408E+02, 6.941E+02,
    6.392E+02, 6.044E+02, 8.181E+02, 7.579E+02, 6.991E+02, 8.064E+02, 6.376E+02, 3.032E+02, 1.690E+02, 5.743E+01,
    2.652E+01, 8.930E+00, 6.129E+00, 3.316E+01, 2.270E+01, 1.272E+01, 7.825E+00, 3.633E+00, 2.011E+00, 7.202E-01,
    3.760E-01, 1.797E-01, 1.223E-01, 9.699E-02, 8.281E-02, 6.696E-02, 5.785E-02, 5.054E-02, 4.594E-02, 4.078E-02,
    3.681E-02, 3.577E-02, 3.583E-02, 3.634E-02, 3.797E-02, 3.987E-02, 4.445E-02, 4.815E-02};

  /* Photon absorption cross section for CO2 */
  static const Int_t fKN_co2           = 36;
  static constexpr Float_t fEn_co2[36] = {0.10000E-02, 0.15000E-02, 0.20000E-02, 0.30000E-02, 0.40000E-02, 0.50000E-02,
                                          0.60000E-02, 0.80000E-02, 0.10000E-01, 0.15000E-01, 0.20000E-01, 0.30000E-01,
                                          0.40000E-01, 0.50000E-01, 0.60000E-01, 0.80000E-01, 0.10000E+00, 0.15000E+00,
                                          0.20000E+00, 0.30000E+00, 0.40000E+00, 0.50000E+00, 0.60000E+00, 0.80000E+00,
                                          0.10000E+01, 0.12500E+01, 0.15000E+01, 0.20000E+01, 0.30000E+01, 0.40000E+01,
                                          0.50000E+01, 0.60000E+01, 0.80000E+01, 0.10000E+02, 0.15000E+02, 0.20000E+02};
  static constexpr Float_t fMu_co2[36] = {0.39383E+04, 0.13166E+04, 0.58750E+03, 0.18240E+03, 0.77996E+02, 0.40024E+02,
                                          0.23116E+02, 0.96997E+01, 0.49726E+01, 0.15543E+01, 0.74915E+00, 0.34442E+00,
                                          0.24440E+00, 0.20589E+00, 0.18632E+00, 0.16578E+00, 0.15394E+00, 0.13558E+00,
                                          0.12336E+00, 0.10678E+00, 0.95510E-01, 0.87165E-01, 0.80587E-01, 0.70769E-01,
                                          0.63626E-01, 0.56894E-01, 0.51782E-01, 0.44499E-01, 0.35839E-01, 0.30825E-01,
                                          0.27555E-01, 0.25269E-01, 0.22311E-01, 0.20516E-01, 0.18184E-01, 0.17152E-01};

  /* Photon absorption cross section for mylar */
  static const Int_t fKN_my           = 36;
  static constexpr Float_t fEn_my[36] = {1.00000E-03, 1.50000E-03, 2.00000E-03, 3.00000E-03, 4.00000E-03, 5.00000E-03,
                                         6.00000E-03, 8.00000E-03, 1.00000E-02, 1.50000E-02, 2.00000E-02, 3.00000E-02,
                                         4.00000E-02, 5.00000E-02, 6.00000E-02, 8.00000E-02, 1.00000E-01, 1.50000E-01,
                                         2.00000E-01, 3.00000E-01, 4.00000E-01, 5.00000E-01, 6.00000E-01, 8.00000E-01,
                                         1.00000E+00, 1.25000E+00, 1.50000E+00, 2.00000E+00, 3.00000E+00, 4.00000E+00,
                                         5.00000E+00, 6.00000E+00, 8.00000E+00, 1.00000E+01, 1.50000E+01, 2.00000E+01};
  static constexpr Float_t fMu_my[36] = {
    2.911E+03, 9.536E+02, 4.206E+02, 1.288E+02, 5.466E+01, 2.792E+01, 1.608E+01, 6.750E+00, 3.481E+00,
    1.132E+00, 5.798E-01, 3.009E-01, 2.304E-01, 2.020E-01, 1.868E-01, 1.695E-01, 1.586E-01, 1.406E-01,
    1.282E-01, 1.111E-01, 9.947E-02, 9.079E-02, 8.395E-02, 7.372E-02, 6.628E-02, 5.927E-02, 5.395E-02,
    4.630E-02, 3.715E-02, 3.181E-02, 2.829E-02, 2.582E-02, 2.257E-02, 2.057E-02, 1.789E-02, 1.664E-02};


  ClassDef(CbmTrdRadiator, 3)
};
#endif
