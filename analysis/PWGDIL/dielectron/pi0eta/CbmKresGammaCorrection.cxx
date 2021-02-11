/**
 *    file CbmKresGammaCorrection.cxx
 *
 *    author Ievgenii Kres
 *    date 04.04.2017
 *    modified 30.01.2020
 *
 *    Class for the correction of direct photons.
 *    Different cases are considered depending on the lepton identification in the RICH (for gammas): 0 out of 2, at least 1 out of 2, and 2 out of 2.
 *    Correction is done for different rapidity and pt phase spaces. The correction in files are done on independent simulation of evenly distributed photons over the full acceptance.
 *
 *    The root files for this analysis are not allowed to load in the trunk !!!. One can find them in my folder on kronos:
 *    /lustre/nyx/cbm/users/ikres/cbmroot230419/analysis/conversion2/
 *
 **/

#include "CbmKresGammaCorrection.h"
#include "TFile.h"
#include "TH2D.h"
#include "TMath.h"
#include "TSystem.h"

#include <iostream>


using namespace std;

CbmKresGammaCorrection::CbmKresGammaCorrection()
  : fHistoList_factors()
  , Correction_factros_all(nullptr)
  , Correction_factros_two(nullptr)
  , Correction_factros_onetwo(nullptr) {}

CbmKresGammaCorrection::~CbmKresGammaCorrection() {}

void CbmKresGammaCorrection::Init(std::vector<std::vector<double>>& vect_all,
                                  std::vector<std::vector<double>>& vect_two,
                                  std::vector<std::vector<double>>& vect_onetwo,
                                  double OA,
                                  double IM) {
  InitHistograms();

  string Correction_path =
    string(gSystem->Getenv("VMCWORKDIR"))
    + Form("/analysis/conversion2/Correction_pi0_g_OA%i_IM%i_num2.root",
           (int) OA,
           (int) IM);  /// most recent files for conversion
  cout << "file is " << Correction_path << endl;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* fcorrection = new TFile(
    Correction_path
      .c_str());  // file with almost ?? Mio photons, homogeneously distributed over interested region
  // rapidity graphs
  TH2D* mc = (TH2D*) fcorrection->Get(
    "conversionKres/General/MC_info/MC_Direct_photons_Pt_vs_rap_est");
  TH2D* all = (TH2D*) fcorrection->Get(
    "conversionKres/direct photons/Both/all/Ph_pt_vs_rap_est_all_Both");
  TH2D* two = (TH2D*) fcorrection->Get(
    "conversionKres/direct photons/Both/two/Ph_pt_vs_rap_est_two_Both");
  TH2D* onetwo = (TH2D*) fcorrection->Get(
    "conversionKres/direct photons/Both/onetwo/Ph_pt_vs_rap_est_onetwo_Both");

  std::vector<double> rapidity_column;

  for (int ix = 1; ix <= 10; ix++) {
    rapidity_column.clear();
    for (int iy = 1; iy <= 30; iy++) {
      double cont_reco = all->GetBinContent(ix, iy);
      double mc_cont   = mc->GetBinContent(ix, iy);
      if (mc_cont == 0) mc_cont = 1;
      double eff = cont_reco / mc_cont;
      rapidity_column.push_back(eff);
      double content = 0;
      if (eff != 0) content = 1 / eff;
      Correction_factros_all->SetBinContent(ix, iy, content);
      // cout << "ix = " << ix << "; iy = " << iy << "; content = " << eff << endl;
      // cout << "all: rap_bin = " << ix-1 << "; pt_bin = " << iy-1 << "; cont_reco = " << cont_reco << ";   mc_cont = " << mc_cont << ";   eff = " << eff << endl;
    }
    vect_all.push_back(rapidity_column);
  }

  for (int ix = 1; ix <= 10; ix++) {
    rapidity_column.clear();
    for (int iy = 1; iy <= 30; iy++) {
      double cont_reco = two->GetBinContent(ix, iy);
      double mc_cont   = mc->GetBinContent(ix, iy);
      if (mc_cont == 0) mc_cont = 1;
      double eff = cont_reco / mc_cont;
      rapidity_column.push_back(eff);
      double content = 0;
      if (eff != 0) content = 1 / eff;
      Correction_factros_two->SetBinContent(ix, iy, content);
      // cout << "two: rap_bin = " << ix-1 << "; pt_bin = " << iy-1 << "; cont_reco = " << cont_reco << ";   mc_cont = " << mc_cont << ";   eff = " << eff << endl;
    }
    vect_two.push_back(rapidity_column);
  }

  for (int ix = 1; ix <= 10; ix++) {
    rapidity_column.clear();
    for (int iy = 1; iy <= 30; iy++) {
      double cont_reco = onetwo->GetBinContent(ix, iy);
      double mc_cont   = mc->GetBinContent(ix, iy);
      if (mc_cont == 0) mc_cont = 1;
      double eff = cont_reco / mc_cont;
      rapidity_column.push_back(eff);
      double content = 0;
      if (eff != 0) content = 1 / eff;
      Correction_factros_onetwo->SetBinContent(ix, iy, content);
      // cout << "onetwo: rap_bin = " << ix-1 << "; pt_bin = " << iy-1 << "; cont_reco = " << cont_reco << ";   mc_cont = " << mc_cont << ";   eff = " << eff << endl;
    }
    vect_onetwo.push_back(rapidity_column);
  }

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmKresGammaCorrection::Finish() {
  gDirectory->mkdir("Correction factors");
  gDirectory->cd("Correction factors");
  for (UInt_t i = 0; i < fHistoList_factors.size(); i++) {
    fHistoList_factors[i]->Write();
  }
  gDirectory->cd("..");
}

void CbmKresGammaCorrection::InitHistograms() {
  Correction_factros_all =
    new TH2D("Correction_factros_all",
             "Correction_factros_all; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_factors.push_back(Correction_factros_all);
  Correction_factros_two =
    new TH2D("Correction_factros_two",
             "Correction_factros_two; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_factors.push_back(Correction_factros_two);
  Correction_factros_onetwo =
    new TH2D("Correction_factros_onetwo",
             "Correction_factros_onetwo; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_factors.push_back(Correction_factros_onetwo);
}
