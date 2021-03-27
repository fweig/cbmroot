// ------------------------------------------------------------------------
// -----                       CbmTrdRadiator source file             -----
// -----                  Created 10/11/04  by M.Kalisky              -----
// ------------------------------------------------------------------------
#include "CbmTrdRadiator.h"

#include "CbmTrdGas.h"    // for CbmTrdGas
#include "CbmTrdPoint.h"  // for CbmTrdPoint

#include <FairLogger.h>  // for Logger, LOG

#include <TFile.h>        // for TFile, gFile
#include <TGeoManager.h>  // for TGeoManager, gGeoManager
#include <TH1.h>          // for TH1D
#include <TMath.h>        // for Exp, Sqrt, Cos, Pi
#include <TMathBase.h>    // for Abs
#include <TObjString.h>   // for TObjString
#include <TRandom.h>      // for TRandom
#include <TRandom3.h>     // for gRandom
#include <TVector3.h>     // for TVector3

#include <iomanip>  // for setprecision, __iom_t5

#include <stdio.h>   // for printf, sprintf
#include <stdlib.h>  // for exit

using std::setprecision;

// -----  Default constructor  ------------------------------------------------
CbmTrdRadiator::CbmTrdRadiator(Bool_t SimpleTR, Int_t Nfoils, Float_t FoilThick, Float_t GapThick, TString material,
                               TString window)  // If "Kapton" is set, 0.05 mu aluminum will be added
  : fWindowFoil(window)
  , fRadType("")
  , fDetType(-1)
  , fFirstPass(kTRUE)
  , fSimpleTR(SimpleTR)
  , fNFoils(Nfoils)
  , fFoilThick(FoilThick)
  , fGapThick(GapThick)
  , fFoilMaterial(material)
  , fGasThick(-1)
  , fFoilDens(-1.)
  , fGapDens(-1.)
  , fFoilOmega(-1.)
  , fGapOmega(-1.)
  , fnPhotonCorr(0.65)
  , fFoilThickCorr(1.)
  , fGapThickCorr(1.)
  , fGasThickCorr(1.)
  , fnTRprod(-1)
  , fWinDens(-1.)
  , fWinThick(-1.)
  , WINDOW()
  , fCom1(-1.)
  , fCom2(-1.)
  , fSpBinWidth((Float_t) fSpRange / (Float_t) fSpNBins)
  , fSigma(nullptr)
  , fSigmaWin(nullptr)
  , fSigmaDet(nullptr)
  , fSpectrum(nullptr)
  , fWinSpectrum(nullptr)
  , fDetSpectrumA(nullptr)
  , fDetSpectrum(nullptr)
  , fTrackMomentum(nullptr)
  , fFinal()
  , fnTRabs()
  , fnTRab(-1.)
  , fELoss(-1.)
  , fMom(-1., -1., -1.)
{
  for (Int_t i = 0; i < fNMom; i++) {
    fFinal[i] = nullptr;
  }
}
//-----------------------------------------------------------------------------

// -----  Constructor using predefined radiator prototype------------------------------------
CbmTrdRadiator::CbmTrdRadiator(Bool_t SimpleTR, TString prototype,
                               TString window)  // If "Kapton" is set, 0.05 mu aluminum will be added
  : fWindowFoil(window)
  , fRadType(prototype)
  , fDetType(-1)
  , fFirstPass(kTRUE)
  , fSimpleTR(SimpleTR)
  , fNFoils(337)
  , fFoilThick(0.0012)
  , fGapThick(0.09)
  , fFoilMaterial("")
  , fGasThick(-1)
  , fFoilDens(-1.)
  , fGapDens(-1.)
  , fFoilOmega(-1.)
  , fGapOmega(-1.)
  , fnPhotonCorr(0.65)
  , fFoilThickCorr(1.)
  , fGapThickCorr(1.)
  , fGasThickCorr(1.)
  , fnTRprod(-1)
  , fWinDens(-1.)
  , fWinThick(-1.)
  , WINDOW()
  , fCom1(-1.)
  , fCom2(-1.)
  , fSpBinWidth((Float_t) fSpRange / (Float_t) fSpNBins)
  , fSigma(nullptr)
  , fSigmaWin(nullptr)
  , fSigmaDet(nullptr)
  , fSpectrum(nullptr)
  , fWinSpectrum(nullptr)
  , fDetSpectrumA(nullptr)
  , fDetSpectrum(nullptr)
  , fTrackMomentum(nullptr)
  , fFinal()
  , fnTRabs()
  , fnTRab(-1.)
  , fELoss(-1.)
  , fMom(-1., -1., -1.)
{
  for (Int_t i = 0; i < fNMom; i++) {
    fFinal[i] = nullptr;
  }
}
//-----------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmTrdRadiator::~CbmTrdRadiator()
{
  //    FairRootManager *fManager = FairRootManager::Instance();
  //  fManager->Write();
  if (fSpectrum) {
    LOG(info) << " -I DELETING fSpectrum ";
    delete fSpectrum;
  }
  if (fWinSpectrum) delete fWinSpectrum;
  if (fDetSpectrum) delete fDetSpectrum;
  if (fDetSpectrumA) delete fDetSpectrumA;

  for (Int_t i = 0; i < fNMom; i++) {
    if (fFinal[i]) delete fFinal[i];
  }
}

// ----- Create Histogramms --------------------------------------------------
void CbmTrdRadiator::CreateHistograms()
{

  // Create the needed histograms

  fSpBinWidth = (Float_t) fSpRange / (Float_t) fSpNBins;

  Float_t SpLower = 1.0 - 0.5 * fSpBinWidth;
  Float_t SpUpper = SpLower + (Float_t) fSpRange;

  Char_t name[50];

  if (fSpectrum) delete fSpectrum;
  fSpectrum = new TH1D("fSpectrum", "TR spectrum", fSpNBins, SpLower, SpUpper);

  if (fWinSpectrum) delete fWinSpectrum;
  fWinSpectrum = new TH1D("fWinSpectrum", "TR spectrum in Mylar", fSpNBins, SpLower, SpUpper);

  if (fDetSpectrum) delete fDetSpectrum;
  fDetSpectrum = new TH1D("fDetSpectrum", "TR spectrum escaped from detector", fSpNBins, SpLower, SpUpper);

  if (fDetSpectrumA) delete fDetSpectrumA;
  fDetSpectrumA = new TH1D("fDetSpectrumA", "TR spectrum absorbed in detector", fSpNBins, SpLower, SpUpper);

  for (Int_t i = 0; i < fNMom; i++) {
    sprintf(name, "fFinal%d", i + 1);
    //LOG(info) <<"name : "<<name;
    if (fFinal[i]) delete fFinal[i];
    fFinal[i] = new TH1D(name, name, fSpNBins, SpLower, SpUpper);
  }
}


// ----- Init function ----------------------------------------------------
void CbmTrdRadiator::Init()
{
  LOG(info) << "================CbmTrdRadiator===============";
  CreateHistograms();

  //Check if a radiator type has been set
  if (fRadType == "") {
    //Set fnPhotonCorr also if no prototype has been specified
    fnPhotonCorr = 0.65;
    //Check specified radiator types
  }
  else if (fRadType == "tdr18") {  //Default TDR18 radiator 30cm Box + 1.5 cm in carbon grid
    fFoilMaterial = "pefoam20";
    fNFoils       = 337;
    fGapThick     = 900 * 0.0001;
    fFoilThick    = 12 * 0.0001;
    fnPhotonCorr  = 0.65;
  }
  else if (fRadType == "B++" || fRadType == "K++") {
    fFoilMaterial = "pokalon";
    fNFoils       = 350;
    fGapThick     = 0.07;
    fFoilThick    = 0.0024;
    fnPhotonCorr  = 0.65;
  }
  else if (fRadType == "G30") {
    fFoilMaterial = "pefiber";
    fNFoils       = 1791;
    fGapThick     = 50 * 0.0001;
    fFoilThick    = 17 * 0.0001;
    fnPhotonCorr  = 0.65;
  }
  else {  // If radiator typ is neither empty nor one of the above (because of typo or else) set defaults

    LOG(warn) << "CbmTrdRadiator::Init : *********** Radiator prototype not known ***********";
    LOG(warn) << "CbmTrdRadiator::Init :             switch to default parameters            ";
    fFoilMaterial = "pefoam20";
    fNFoils       = 337;
    fGapThick     = 900 * 0.0001;
    fFoilThick    = 12 * 0.0001;
    fnPhotonCorr  = 0.65;
  }


  //Material has been specified now, either in the constructor or set by radiator type above. Now, go through materials and set properties accordingly


  // material dependent parameters for the gas between the foils of the radiator
  //Properties of the air gaps between foils
  fGapDens  = 0.001205;  // [g/cm3]
  fGapOmega = 0.7;       //plasma frequency  ( from Anton )
  //material = fFoilMaterial;
  if (fFoilMaterial == "pefoam20") {
    // (polyethylen foam foil)
    fFoilDens  = 0.90;   // [g/cm3 ]
    fFoilOmega = 20.64;  //plasma frequency ( from Cyrano )
  }
  else if (fFoilMaterial == "polyethylen") {
    // (polyethylen)
    fFoilDens  = 0.92;  // [g/cm3 ]
    fFoilOmega = 20.9;  //plasma frequency ( from Anton )
  }
  else if (fFoilMaterial == "pefiber") {
    // (polyethylen)
    fFoilDens  = 0.90;   // [g/cm3 ]
    fFoilOmega = 20.64;  //plasma frequency ( from Cyrano )
  }
  else if (fFoilMaterial == "mylar") {
    // (Mylar)
    fFoilDens  = 1.393;  // [g/cm3 ]
    fFoilOmega = 24.53;  //plasma frequency ( from Cyrano )
  }
  else if (fFoilMaterial == "pokalon") {
    // (PokalonN470)
    fFoilDens  = 1.150;  // [g/cm3 ]
    fFoilOmega = 22.43;  //plasma frequency ( from Cyrano )
  }
  else {  // If material is empty or one of the above (because of typo or else) set default
    LOG(warn) << "CbmTrdRadiator::Init : *********** Radiator material not known ***********";
    LOG(warn) << "CbmTrdRadiator::Init :             switch to default material            ";
    // (polyethylen foam foil)
    fFoilMaterial = "pefoam20";
    fFoilDens     = 0.90;   // [g/cm3 ]
    fFoilOmega    = 20.64;  //plasma frequency ( from Cyrano )
  }

  // foil between radiator and gas chamber
  if (fWindowFoil == "Kapton") {  // 0.05 mu Aluminum is added in SigmaWin() for Kapton
    fWinDens  = 1.42;             //[g/cm3]  ?? define values ?? default mylar
    fWinThick = 0.0025;           // [cm]  ?? define values ?? default mylar
  }
  else if (fWindowFoil != "Mylar") {
    if (!WINDOW.Init(fWindowFoil)) fWindowFoil = "Mylar";
  }
  if (fWindowFoil == "Mylar") {
    fWinDens  = 1.39;    //[g/cm3]
    fWinThick = 0.0100;  //[cm]
  }
  if (!WINDOW.fN) {
    LOG(info) << "CbmTrdRadiator::Init : Window type         : " << fWindowFoil;
    LOG(info) << "CbmTrdRadiator::Init : Window density      : " << fWinDens << " g/cm^3";
    LOG(info) << "CbmTrdRadiator::Init : Window thickness    : " << fWinThick << " cm";
  }


  LOG(info) << "CbmTrdRadiator::Init : Prototype           : " << fRadType;
  LOG(info) << "CbmTrdRadiator::Init : Scaling factor      : " << fnPhotonCorr;
  LOG(info) << "CbmTrdRadiator::Init : Foil material       : " << fFoilMaterial;
  LOG(info) << "CbmTrdRadiator::Init : Nr. of foils        : " << fNFoils;
  LOG(info) << "CbmTrdRadiator::Init : Foil thickness      : " << setprecision(4) << fFoilThick << " cm";
  LOG(info) << "CbmTrdRadiator::Init : Gap thickness       : " << fGapThick << " cm";
  LOG(info) << "CbmTrdRadiator::Init : Simple TR production: " << setprecision(2) << fSimpleTR;
  LOG(info) << "CbmTrdRadiator::Init : Foil plasm. frequ.  : " << fFoilOmega << " keV";
  LOG(info) << "CbmTrdRadiator::Init : Gap plasm. frequ.   : " << fGapOmega << " keV";


  // Get all the gas properties from CbmTrdGas
  CbmTrdGas* fTrdGas = CbmTrdGas::Instance();
  if (fTrdGas == 0) {
    fTrdGas = new CbmTrdGas();
    fTrdGas->Init();
  }

  fCom2     = fTrdGas->GetNobleGas();
  fCom1     = fTrdGas->GetCO2();
  fDetType  = fTrdGas->GetDetType();
  fGasThick = fTrdGas->GetGasThick();

  LOG(info) << "CbmTrdRadiator::Init : Detector noble gas   : " << fTrdGas->GetNobleGas();
  LOG(info) << "CbmTrdRadiator::Init : Detector quenching gas: " << fTrdGas->GetCO2();
  LOG(info) << "CbmTrdRadiator::Init : Detector type        : " << fTrdGas->GetDetType();
  LOG(info) << "CbmTrdRadiator::Init : Detector gas thick.  : " << fTrdGas->GetGasThick() << " cm";

  //LOG(info)  << "************* End of Trd Radiator Init **************";
  // If simplified version is used one didn't calculate the TR
  // for each CbmTrdPoint in full glory, but create at startup
  // some histograms for several momenta which are later used to
  // get the TR much faster.
  if (fSimpleTR == kTRUE) {

    ProduceSpectra();

    TFile* f1 = new TFile("TRhistos.root", "recreate");

    for (Int_t i = 0; i < fNMom; i++) {
      fFinal[i]->Write();
    }
    f1->Close();
    f1->Delete();
  }
}

//----------------------------------------------------------------------------
CbmTrdRadiator::CbmTrdEntranceWindow::CbmTrdEntranceWindow(TString def) : fN(0)
{
  memset(fThick, 0, NCOMPONENTS * sizeof(Float_t));
  memset(fDens, 0, NCOMPONENTS * sizeof(Float_t));
  memset(GetMu, 0, NCOMPONENTS * sizeof(GetMuPtr));
  Init(def);
}

//----------------------------------------------------------------------------
Bool_t CbmTrdRadiator::CbmTrdEntranceWindow::Init(TString def)
{
  TObjArray* mats = def.Tokenize(";");
  for (fN = 0; fN < mats->GetEntriesFast(); fN++) {
    TObjString* mat = (TObjString*) (*mats)[fN];
    TString mname   = mat->String();
    if (mname.CompareTo("Al") == 0) {
      fMat[fN]  = "Al";
      fDens[fN] = 2.7;
      GetMu[fN] = CbmTrdRadiator::GetMuAl;
    }
    else if (mname.CompareTo("C") == 0) {
      fMat[fN]  = "C";
      fDens[fN] = 2.267;
      GetMu[fN] = CbmTrdRadiator::GetMuC;
    }
    else if (mname.CompareTo("HC") == 0) {
      fMat[fN]  = "HC";
      fDens[fN] = 4.8e-2;  //  48 Kg/m^3
      GetMu[fN] = CbmTrdRadiator::GetMuAir;
    }
    else if (mname.CompareTo("Po") == 0) {
      fMat[fN]  = "Po";
      fDens[fN] = 0.92;
      GetMu[fN] = CbmTrdRadiator::GetMuPo;
    }
    else if (mname.CompareTo("Pok") == 0) {
      fMat[fN]  = "Pok";
      fDens[fN] = 1.150;
      GetMu[fN] = CbmTrdRadiator::GetMuPok;
    }
    else if (mname.CompareTo("Ka") == 0) {
      fMat[fN]  = "Ka";
      fDens[fN] = 1.39;
      GetMu[fN] = CbmTrdRadiator::GetMuKa;
    }
    else if (mname.CompareTo("Air") == 0) {
      fMat[fN]  = "Air";
      fDens[fN] = 1.205e-3;
      GetMu[fN] = CbmTrdRadiator::GetMuAir;
    }
    else if (mname.CompareTo("Xe") == 0) {
      fMat[fN]  = "Xe";
      fDens[fN] = 5.9e-3;
      GetMu[fN] = CbmTrdRadiator::GetMuXe;
    }
    else if (mname.CompareTo("CO2") == 0) {
      fMat[fN]  = "CO2";
      fDens[fN] = 1.9767e-3;
      GetMu[fN] = CbmTrdRadiator::GetMuCO2;
    }
    else if (mname.CompareTo("My") == 0) {
      fMat[fN]  = "My";
      fDens[fN] = 1.393;
      GetMu[fN] = CbmTrdRadiator::GetMuMy;
    }
    else {
      LOG(warn) << "CbmTrdEntranceWindow::Init : material " << mname.Data() << " not in the DB. Can't create "
                << def.Data() << " entrance window structure. Default to Mylar.";
      fN = 0;
      return kFALSE;
    }
    LOG(info) << "CbmTrdEntranceWindow::Init : C" << fN << " type      : " << fMat[fN];
    LOG(info) << "CbmTrdEntranceWindow::Init : density      : " << fDens[fN] << " g/cm^3";
    LOG(info) << "CbmTrdEntranceWindow::Init : thickness    : " << fThick[fN] << " cm";
  }
  mats->Delete();
  delete mats;
  return kTRUE;
}

//----------------------------------------------------------------------------
void CbmTrdRadiator::SetEWwidths(Int_t n, Float_t* w)
{
  if (n > NCOMPONENTS) {
    LOG(warn) << "CbmTrdEntranceWindow : can support only up to " << NCOMPONENTS
              << " plane-parallel elements. Please check your simulations";
    n = NCOMPONENTS;
  }
  memcpy(WINDOW.fThick, w, n * sizeof(Float_t));
}

//---- Lattice Hit------------------------------------------------------------
Bool_t CbmTrdRadiator::LatticeHit(const CbmTrdPoint* point)
{
  //printf("---------------------------------------------------\n");
  Double_t point_in[3];
  Double_t point_out[3];
  if (nullptr != point) {
    point_in[0] = point->GetXIn();
    point_in[1] = point->GetYIn();
    point_in[2] = point->GetZIn();

    point_out[0]               = point->GetXOut();
    point_out[1]               = point->GetYOut();
    point_out[2]               = point->GetZOut();
    Double_t back_direction[3] = {(point_in[0] - point_out[0]), (point_in[1] - point_out[1]),
                                  (point_in[2] - point_out[2])};
    if (back_direction[2] > 0) {
      LOG(debug2) << "CbmTrdRadiator::LatticeHit: MC-track points towards target!";
      //for(Int_t i = 0; i < 3; i++)
      //printf("%i:  in:%8.2f  out:%8.2f direction:%8.2f\n",i,point_in[i],point_out[i],back_direction[i]);
      return false;
    }
    Double_t trackLength = TMath::Sqrt(back_direction[0] * back_direction[0] + back_direction[1] * back_direction[1]
                                       + back_direction[2] * back_direction[2]);

    trackLength *= 10.;  // cm -> mm to get a step width of 1mm
    //printf("track length:%7.2fmm\n",trackLength);
    gGeoManager->FindNode((point_out[0] + point_in[0]) / 2, (point_out[1] + point_in[1]) / 2,
                          (point_out[2] + point_in[2]) / 2);

    Double_t pos[3] = {point_in[0], point_in[1], point_in[2]};  // start at entrance point

    for (Int_t i = 0; i < 3; i++) {
      back_direction[i] /= trackLength;
      //printf("%i:  in:%8.2f  out:%8.2f  start:%8.2f direction:%8.2f\n",i,point_in[i],point_out[i],pos[i],back_direction[i]);
    }
    if (TString(gGeoManager->GetPath()).Contains("gas")) {
      Int_t stepCount = 0;
      while (
        /*(!TString(gGeoManager->GetPath()).Contains("lattice") || !TString(gGeoManager->GetPath()).Contains("radiator")) &&*/
        pos[2] >= point_in[2] - 5 && stepCount < 50) {
        stepCount++;
        //printf("step%i\n",stepCount);
        for (Int_t i = 0; i < 3; i++) {
          pos[i] += back_direction[i];
          //printf("%8.2f   ",pos[i]);
        }
        //printf("   %s\n",TString(gGeoManager->GetPath()).Data());
        gGeoManager->FindNode(pos[0], pos[1], pos[2]);
        if (TString(gGeoManager->GetPath()).Contains("radiator")) {
          //printf ("%s false\n",TString(gGeoManager->GetPath()).Data());
          return false;
        }
        else if (TString(gGeoManager->GetPath()).Contains("lattice")) {
          //printf ("%s true <----------------------------------------\n",TString(gGeoManager->GetPath()).Data());
          return true;
        }
        else if (TString(gGeoManager->GetPath()).Contains("frame")) {
          //printf ("%s true <----------------------------------------\n",TString(gGeoManager->GetPath()).Data());
          return true;
        }
        else {
          //printf ("%s\n",TString(gGeoManager->GetPath()).Data());
        }
      }
    }
    else {
      LOG(error) << "CbmTrdRadiator::LatticeHit: MC-track not in TRD! Node:" << TString(gGeoManager->GetPath()).Data()
                 << " gGeoManager->MasterToLocal() failed!";
      return false;
    }
  }
  else {
    LOG(error) << "CbmTrdRadiator::LatticeHit: CbmTrdPoint == nullptr!";
    return false;
  }
  return kTRUE;
}
//----------------------------------------------------------------------------
// ---- Spectra Production ---------------------------------------------------
void CbmTrdRadiator::ProduceSpectra()
{

  fTrackMomentum = new Double_t[fNMom];

  Double_t trackMomentum[fNMom] = {0.1, 0.25, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

  for (Int_t imom = 0; imom < fNMom; imom++) {
    fTrackMomentum[imom] = trackMomentum[imom];
  }

  for (Int_t i = 0; i < fNMom; i++) {

    fMom.SetXYZ(0.0, 0.0, fTrackMomentum[i]);

    ProcessTR();
    fnTRabs[i] = fnTRab;

    // copy the content of the current fDetSpectrumA into appropriate
    // fFinal

    Float_t tmp = 0;
    for (Int_t j = 0; j < fSpNBins; j++) {
      tmp = fDetSpectrumA->GetBinContent(j + 1);
      fFinal[i]->SetBinContent(j + 1, tmp);
    }
    fFinal[i]->SetTitle(Form("%s-momentum%.2f", fFinal[i]->GetTitle(), fTrackMomentum[i]));
  }
}

//----------------------------------------------------------------------------

// ---- GetTR  ---------------------------------------------------------------
Float_t CbmTrdRadiator::GetTR(TVector3 Mom)
{

  /*
   * Simplified simulation of the TR
   */

  if (fSimpleTR == kTRUE) {

    Float_t m = Mom.Mag();

    // Find correct spectra
    Int_t i = 0;
    while (m > fTrackMomentum[i]) {
      i++;
      if (i == fNMom) {
        i--;
        break;
      }
    }

    ELoss(i);
  }

  /*
   * Full simulation of the TR production
   */
  else {

    fMom = Mom;
    ProcessTR();
  }

  return fELoss;
}

//----------------------------------------------------------------------------

//----- main TR processing function ------------------------------
void CbmTrdRadiator::ProcessTR()
{

  // Compute the angle normalization  factor -
  // for different angles the detector thicknesses are different

  Float_t fNormPhi = fMom.Mag() / fMom.Pz();

  // Correct the thickness according to this factor

  fFoilThickCorr = TMath::Abs(fFoilThick * fNormPhi);
  fGapThickCorr  = TMath::Abs(fGapThick * fNormPhi);
  fGasThickCorr  = TMath::Abs(fGasThick * fNormPhi);

  fFirstPass = true;
  // compute the TR spectra in the radiator
  TRspectrum();
  // compute the TR spectra in the mylar foil
  WinTRspectrum();
  // compute the TR spectra in the detector
  DetTRspectrum();
  // Loop over photons and compute the E losses
  ELoss(-1);


  if (fDetType == 1) {  // if detector is MB type = dual-sided MWPC
    fFirstPass           = false;
    Float_t energiefirst = fELoss;
    Float_t fTRAbsfirst  = fnTRab;

    // compute the TR spectra in the mylar foil between the two gas layers
    WinTRspectrum();
    // compute the TR spectra in the second halve of the detector
    DetTRspectrum();
    // Loop over photons and compute the E losses
    ELoss(-1);
    fELoss += energiefirst;
    fnTRab += fTRAbsfirst;
  }
}
//----------------------------------------------------------------------------

// ---- Compute the energy losses --------------------------------------------
Int_t CbmTrdRadiator::ELoss(Int_t index)
{

  // calculate the energy loss of the TR photons in the
  // detector gas. fNTRab is the number of TR photons
  // absorbed in the gas. Since this number is low the
  // actual number comes from a poisson distribution.

  Int_t nPhoton;
  Int_t maxPhoton  = 50;
  Double32_t eLoss = 0;
  Float_t eTR      = 0;

  if (-1 == index) {
    if (0 == fDetSpectrumA) {
      LOG(error) << " -Err :: No input spectra ";
      fELoss = -1;
      return 1;
    }
    nPhoton = gRandom->Poisson(fnTRab * fnPhotonCorr);
    if (nPhoton > maxPhoton) nPhoton = maxPhoton;
    for (Int_t i = 0; i < nPhoton; i++) {
      // energy of the photon
      eTR = fDetSpectrumA->GetRandom();
      eLoss += eTR;
    }
  }
  else {
    if (0 == fFinal[index]) {
      LOG(error) << " -Err :: No input spectra ";
      fELoss = -1;
      return 1;
    }
    nPhoton = gRandom->Poisson(fnTRabs[index] * fnPhotonCorr);
    if (nPhoton > maxPhoton) nPhoton = maxPhoton;
    for (Int_t i = 0; i < nPhoton; i++) {
      // energy of the photon
      eTR = fFinal[index]->GetRandom();
      eLoss += eTR;
    }
  }

  // keV->GeV  & set the result
  fELoss = eLoss / 1000000.0;

  return 0;
}

//----------------------------------------------------------------------------

//----- TR spectrum ---------------------------------------------------
Int_t CbmTrdRadiator::TRspectrum()
{

  // calculate the number of TR photons in the radiator which are not
  // absorbed in the radiator.
  // Formulas are tacken from M. Castellano et al.
  // Computer Physics Communications 61 (1990)


  // Where does this values come from, put them in the header file
  const Float_t kAlpha = 0.0072973;  //  1/137
  const Int_t kSumMax  = 30;

  // calculate the gamma of the particle
  Float_t gamma = GammaF();

  fSpectrum->Reset();
  fDetSpectrum->Reset();
  fDetSpectrumA->Reset();

  Float_t kappa = fGapThickCorr / fFoilThickCorr;

  SetSigma(1);

  Float_t stemp = 0;

  // Loop over energy
  // stemp is the total energy of all created photons
  for (Int_t iBin = 0; iBin < fSpNBins; iBin++) {

    // keV -> eV
    Float_t energyeV = (fSpBinWidth * iBin + 1.0) * 1e3;

    Float_t csFoil = fFoilOmega / energyeV;
    Float_t csGap  = fGapOmega / energyeV;

    Float_t rho1 = energyeV * fFoilThickCorr * 1e4 * 2.5 * (1.0 / (gamma * gamma) + csFoil * csFoil);
    Float_t rho2 = energyeV * fFoilThickCorr * 1e4 * 2.5 * (1.0 / (gamma * gamma) + csGap * csGap);

    // Calculate the sum over the production angle tetan
    Float_t sum = 0;
    for (Int_t iSum = 0; iSum < kSumMax; iSum++) {
      Float_t tetan = (TMath::Pi() * 2.0 * (iSum + 1) - (rho1 + kappa * rho2)) / (kappa + 1.0);
      if (tetan < 0.0) { tetan = 0.0; }
      Float_t aux = 1.0 / (rho1 + tetan) - 1.0 / (rho2 + tetan);
      sum += tetan * (aux * aux) * (1.0 - TMath::Cos(rho1 + tetan));
    }
    Float_t conv = 1.0 - TMath::Exp(-fNFoils * fSigma[iBin]);

    // eV -> keV
    Float_t energykeV = energyeV * 0.001;

    // dN / domega
    Float_t wn = kAlpha * 4.0 / (fSigma[iBin] * (kappa + 1.0)) * conv * sum / energykeV;
    /*
    wn  = 4.0 * kAlpha 
      / (energykeV * (kappa + 1.0)) 
      * (1.0 - TMath::Exp(Double_t(-fNFoils) * fSigma[iBin])) / (1.0 - TMath::Exp(-fSigma[iBin]))
      * sum;
      */
    // save the result
    fSpectrum->SetBinContent(iBin + 1, wn);
    // compute the integral
    stemp += wn;
  }

  // <nTR> (binsize corr.)
  //Float_t nTR0 = stemp * fSpBinWidth;
  //LOG(info) << " No. of photons after radiator" << nTR0;

  // compute the spectra behind the mylar foil (absorption)
  //  WinTRspectrum();

  return 1;
}

//------------------------------------------------------------------

//----- WinTRspectrum -----------------------------------------------
Int_t CbmTrdRadiator::WinTRspectrum()
{
  //
  // Computes the spectrum after absorption in Mylar foil
  //

  fWinSpectrum->Reset();

  SetSigma(2);

  Float_t stemp = 0;
  for (Int_t iBin = 0; iBin < fSpNBins; iBin++) {

    Float_t sp = 0;
    if (fFirstPass == true) { sp = fSpectrum->GetBinContent(iBin + 1); }
    else {
      sp = fDetSpectrum->GetBinContent(iBin + 1);
    }
    // compute the absorption coefficient
    Float_t conv = TMath::Exp(-fSigmaWin[iBin]);
    Float_t wn   = sp * conv;

    fWinSpectrum->SetBinContent(iBin + 1, wn);

    stemp += wn;
  }

  //fnTRprod = stemp * fSpBinWidth;
  //LOG(info) << "No. of photons after My = "<< fnTRprod;

  // compute the spectrum absorbed in the D & escaped from the D
  //    DetTRspectrum();
  return 1;
}
//----------------------------------------------------------------------------

//----- DetTRspectrum ------------------------------------------------
Int_t CbmTrdRadiator::DetTRspectrum()
{
  //
  // Computes the spectrum absorbed and passed in the gas mixture
  //

  SetSigma(3);
  Float_t stemp  = 0;
  Float_t stemp2 = 0;
  for (Int_t iBin = 0; iBin < fSpNBins; iBin++) {

    //passed spectrum
    Float_t sp   = 0;
    sp           = fWinSpectrum->GetBinContent(iBin + 1);
    Float_t conv = TMath::Exp(-fSigmaDet[iBin]);
    Float_t wn   = sp * conv;

    fDetSpectrum->SetBinContent(iBin + 1, wn);
    stemp += wn;

    // absorbed spectrum
    Float_t conv2 = 1 - TMath::Exp(-fSigmaDet[iBin]);
    Float_t wn2   = sp * conv2;

    fDetSpectrumA->SetBinContent(iBin + 1, wn2);
    stemp2 += wn2;
  }

  //Float_t nTR1 = stemp * fSpBinWidth;
  Float_t nTR2 = stemp2 * fSpBinWidth;

  // Save the number of photons absorbed
  fnTRab = nTR2;

  //LOG(info) << " No. of photons  escaped: " << nTR1;
  //LOG(info) << " No. of photnos  absorbed in the gas: " <<nTR2;

  return 1;
}
//----------------------------------------------------------------------------

//----- Gamma factor--------------------------------------------------
Float_t CbmTrdRadiator::GammaF()
{

  // put this in the header ?
  Float_t massE = 5.1099907e-4;  // GeV/c2

  Float_t p2     = fMom.Mag2();
  Float_t result = TMath::Sqrt(p2 + massE * massE) / massE;

  return result;
}
//----------------------------------------------------------------------------

//----- SetSigma---------------------------------------------------------
void CbmTrdRadiator::SetSigma(Int_t SigmaT)
{
  //
  // Sets the absorbtion crosssection for the energies of the TR spectrum
  //


  if (SigmaT == 1) {
    if (fSigma) delete[] fSigma;
    fSigma = new Float_t[fSpNBins];
    for (Int_t iBin = 0; iBin < fSpNBins; iBin++) {
      Float_t energykeV = iBin * fSpBinWidth + 1.0;
      fSigma[iBin]      = Sigma(energykeV);
    }
  }

  if (SigmaT == 2) {
    if (fSigmaWin) delete[] fSigmaWin;
    fSigmaWin = new Float_t[fSpNBins];
    for (Int_t iBin = 0; iBin < fSpNBins; iBin++) {
      Float_t energykeV = iBin * fSpBinWidth + 1.0;
      fSigmaWin[iBin]   = SigmaWin(energykeV);
    }
  }

  if (SigmaT == 3) {
    if (fSigmaDet) delete[] fSigmaDet;
    fSigmaDet = new Float_t[fSpNBins];
    for (Int_t iBin = 0; iBin < fSpNBins; iBin++) {
      Float_t energykeV = iBin * fSpBinWidth + 1.0;
      fSigmaDet[iBin]   = SigmaDet(energykeV);
    }
  }
}
//----------------------------------------------------------------------------

//----- Sigma-------------------------------------------------------------
Float_t CbmTrdRadiator::Sigma(Float_t energykeV)
{
  //
  // Calculates the absorbtion crosssection for a one-foil-one-gap-radiator
  //

  // keV -> MeV
  Float_t energyMeV = energykeV * 0.001;
  Float_t foil      = 0.0;

  if (fFoilMaterial == "polyethylen") foil = GetMuPo(energyMeV) * fFoilDens * fFoilThickCorr;
  else if (fFoilMaterial == "pefoam20")
    foil = GetMuPo(energyMeV) * fFoilDens * fFoilThickCorr;
  else if (fFoilMaterial == "pefiber")
    foil = GetMuPo(energyMeV) * fFoilDens * fFoilThickCorr;
  else if (fFoilMaterial == "mylar")
    foil = GetMuMy(energyMeV) * fFoilDens * fFoilThickCorr;
  else if (fFoilMaterial == "pokalon")
    foil = GetMuPok(energyMeV) * fFoilDens * fFoilThickCorr;
  else
    LOG(error) << "ERROR:: unknown radiator material";


  if (energyMeV >= 0.001) {
    Float_t result = (foil + (GetMuAir(energyMeV) * fGapDens * fGapThickCorr));
    return result;
  }
  else {
    return 1e6;
  }
}
//----------------------------------------------------------------------------

//----- SigmaWin -------------------------------------------------------
Float_t CbmTrdRadiator::SigmaWin(Float_t energykeV)
{
  //
  // Calculates the absorbtion crosssection for a one-foil
  //

  // keV -> MeV
  Float_t energyMeV = energykeV * 0.001;
  if (energyMeV >= 0.001) {
    if (fWindowFoil == "Kapton")  // aluminized kapton
      return GetMuKa(energyMeV) * fWinDens * fWinThick + GetMuAl(energyMeV) * 2.70 /*[g/cm^3]*/ * 5E-6 /*[cm]*/;
    else if (fWindowFoil == "Mylar")  // mylar foil
      return GetMuMy(energyMeV) * fWinDens * fWinThick;
    else {  // general sandwich structure
      Float_t sigma(0.);
      for (Int_t iwin(0); iwin < WINDOW.fN; iwin++)
        sigma += WINDOW.GetMu[iwin](energyMeV) * WINDOW.fDens[iwin] * WINDOW.fThick[iwin];
      return sigma;
    }
  }
  else
    return 1e6;
}
//----------------------------------------------------------------------------

//----- SigmaDet --------------------------------------------------------
Float_t CbmTrdRadiator::SigmaDet(Float_t energykeV)
{
  //
  //Calculates the absorbtion crosssection for a choosed gas
  //

  // keV -> MeV
  Float_t energyMeV = energykeV * 0.001;

  if (energyMeV >= 0.001) {
    // densities for CO2 and Xe changed. think density for CO2 is just
    // a typo. where the difference for Xe comes from i don't know
    // Values are from http://pdg.lbl.gov/AtomicNuclearProperties/
    // return(GetMuCO2(energyMeV) * 0.001806  * fGasThick * fCom1 +
    // GetMuXe(energyMeV) * 0.00589 * fGasThick * fCom2);
    return (GetMuCO2(energyMeV) * 0.00184 * fGasThick * fCom1 + GetMuXe(energyMeV) * 0.00549 * fGasThick * fCom2);
  }
  else {
    return 1e6;
  }
}
//----------------------------------------------------------------------------

//----- GetMuPok --------------------------------------------------------
Float_t CbmTrdRadiator::GetMuPok(Float_t energyMeV)
{
  //
  // Returns the photon absorbtion cross section for pokalon N470
  //
  return Interpolate(energyMeV, fEn_pok, fMu_pok, fKN_pok);
}
//----------------------------------------------------------------------------
//----- GetMuKa --------------------------------------------------------
Float_t CbmTrdRadiator::GetMuKa(Float_t energyMeV)
{
  //
  // Returns the photon absorbtion cross section for kapton
  //
  return Interpolate(energyMeV, fEn_ka, fMu_ka, fKN_ka);
}
//----------------------------------------------------------------------------

//----- GetMuAl --------------------------------------------------------
Float_t CbmTrdRadiator::GetMuAl(Float_t energyMeV)
{
  //
  // Returns the photon absorbtion cross section for Al
  //
  return Interpolate(energyMeV, fEn_al, fMu_al, fKN_al);
}
//----------------------------------------------------------------------------

//----- GetMuC --------------------------------------------------------
Float_t CbmTrdRadiator::GetMuC(Float_t energyMeV)
{
  //
  // Returns the photon absorbtion cross section for Carbon
  //
  return Interpolate(energyMeV, fEn_c, fMu_c, fKN_c);
}
//----------------------------------------------------------------------------

//----- GetMuPo --------------------------------------------------------
Float_t CbmTrdRadiator::GetMuPo(Float_t energyMeV)
{
  //
  // Returns the photon absorbtion cross section for polypropylene
  //
  return Interpolate(energyMeV, fEn_po, fMu_po, fKN_po);
}
//----------------------------------------------------------------------------

//----- GetMuAir --------------------------------------------------------
Float_t CbmTrdRadiator::GetMuAir(Float_t energyMeV)
{
  //
  // Returns the photon absorbtion cross section for Air
  //
  return Interpolate(energyMeV, fEn_air, fMu_air, fKN_air);
}
//----------------------------------------------------------------------------

//----- GetMuXe --------------------------------------------------------
Float_t CbmTrdRadiator::GetMuXe(Float_t energyMeV)
{
  //
  // Returns the photon absorbtion cross section for xenon
  //
  return Interpolate(energyMeV, fEn_xe, fMu_xe, fKN_xe);
}
//----------------------------------------------------------------------------

//----- GetMuCO2 ------------------------------------------------------
Float_t CbmTrdRadiator::GetMuCO2(Float_t energyMeV)
{
  //
  // Returns the photon absorbtion cross section for CO2
  //
  return Interpolate(energyMeV, fEn_co2, fMu_co2, fKN_co2);
}
//----------------------------------------------------------------------------

//----- GetMuMy -------------------------------------------------------
Float_t CbmTrdRadiator::GetMuMy(Float_t energyMeV)
{
  //
  // Returns the photon absorbtion cross section for mylar
  //
  return Interpolate(energyMeV, fEn_my, fMu_my, fKN_my);
}
//----------------------------------------------------------------------------

//----- Interpolate ------------------------------------------------------
Float_t CbmTrdRadiator::Interpolate(Float_t energyMeV, const Float_t* en, const Float_t* mu, Int_t n)
{
  //
  // Interpolates the photon absorbtion cross section
  // for a given energy <energyMeV>.
  //

  Float_t de  = 0;
  Int_t index = 0;
  Int_t istat = Locate(en, n, energyMeV, index, de);
  if (istat == 0) {
    //       Float_t result = (mu[index] - de * (mu[index]   - mu[index+1])
    //       / (en[index+1] - en[index]  ));
    //       return result;
    Float_t result =
      (TMath::Log(mu[index]) - de * (TMath::Log(mu[index]) - TMath::Log(mu[index + 1])) / (en[index + 1] - en[index]));
    return TMath::Exp(result);
  }
  else {
    return 0.0;
  }
}
//----------------------------------------------------------------------------

//----- Locate ------------------------------------------------------------
Int_t CbmTrdRadiator::Locate(const Float_t* xv, Int_t n, Float_t xval, Int_t& kl, Float_t& dx)
{
  //
  // Locates a point (xval) in a 1-dim grid (xv(n))
  //

  if (xval >= xv[n - 1]) return 1;
  if (xval < xv[0]) return -1;

  Int_t km;
  Int_t kh = n - 1;

  kl = 0;
  while (kh - kl > 1) {
    if (xval < xv[km = (kl + kh) / 2]) kh = km;
    else
      kl = km;
  }
  if (xval < xv[kl] || xval > xv[kl + 1] || kl >= n - 1) {
    printf("Locate failed xv[%d] %f xval %f xv[%d] %f!!!\n", kl, xv[kl], xval, kl + 1, xv[kl + 1]);
    exit(1);
  }

  dx = xval - xv[kl];
  if (xval == 0.001) LOG(info) << "Locat = 0";
  return 0;
}
//----------------------------------------------------------------------------
// Photon absorption cross sections

// Pokalon N470
constexpr Float_t CbmTrdRadiator::fEn_pok[46];
constexpr Float_t CbmTrdRadiator::fMu_pok[46];

// Kapton
constexpr Float_t CbmTrdRadiator::fEn_ka[46];
constexpr Float_t CbmTrdRadiator::fMu_ka[46];

// Aluminum
constexpr Float_t CbmTrdRadiator::fEn_al[48];
constexpr Float_t CbmTrdRadiator::fMu_al[48];

// Polypropylene/Polyethylene
constexpr Float_t CbmTrdRadiator::fEn_po[36];
constexpr Float_t CbmTrdRadiator::fMu_po[36];

// Carbon
constexpr Float_t CbmTrdRadiator::fEn_c[46];
constexpr Float_t CbmTrdRadiator::fMu_c[46];

// Air
constexpr Float_t CbmTrdRadiator::fEn_air[38];
constexpr Float_t CbmTrdRadiator::fMu_air[38];

// Xenon
constexpr Float_t CbmTrdRadiator::fEn_xe[48];
constexpr Float_t CbmTrdRadiator::fMu_xe[48];

// CO2
constexpr Float_t CbmTrdRadiator::fEn_co2[36];
constexpr Float_t CbmTrdRadiator::fMu_co2[36];

// Mylar
constexpr Float_t CbmTrdRadiator::fEn_my[36];
constexpr Float_t CbmTrdRadiator::fMu_my[36];


ClassImp(CbmTrdRadiator)
