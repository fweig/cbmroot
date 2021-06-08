/* Copyright (C) 2007-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Etienne Bechtel */

// -------------------------------------------------------------------------
// -----                    CbmTrdSetTracksPidLike source file         -----
// -----                  Created 25/02/07 by F.Uhlig                  -----
// -----                  Updated 31/08/2016  by J. Book               -----
// -------------------------------------------------------------------------
#include "CbmTrdSetTracksPidLike.h"

#include "CbmGlobalTrack.h"
#include "CbmTrdGas.h"
#include "CbmTrdHit.h"
#include "CbmTrdTrack.h"

#include "FairRootManager.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TKey.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TROOT.h"
#include "TString.h"
#include <TFile.h>

#include <iostream>

// -----   Default constructor   -------------------------------------------
CbmTrdSetTracksPidLike::CbmTrdSetTracksPidLike() : CbmTrdSetTracksPidLike("TrdPidLI", "TrdPidLI") {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTrdSetTracksPidLike::CbmTrdSetTracksPidLike(const char* name, const char*) : FairTask(name) {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTrdSetTracksPidLike::~CbmTrdSetTracksPidLike() {}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmTrdSetTracksPidLike::SetParContainers() {}
// -------------------------------------------------------------------------


// -----  RaedData -------------------------------------------------
Bool_t CbmTrdSetTracksPidLike::ReadData()
{
  //
  // Read the TRD dEdx histograms.
  //

  // Get the name of the input file from CbmTrdGas

  // This file stores all information about the gas layer of the TRD
  // and can construct the required file name

  if (fFileName.IsNull()) {
    CbmTrdGas* fTrdGas = CbmTrdGas::Instance();
    if (fTrdGas == 0) {
      fTrdGas = new CbmTrdGas();
      fTrdGas->Init();
    }
    fFileName = fTrdGas->GetFileName("Like");
  }

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  // Open ROOT file with the histograms
  TFile* histFile = new TFile(fFileName, "READ");
  if (!histFile || !histFile->IsOpen()) {
    Error("ReadData", "Could not open input file: %s", fFileName.Data());
    return kFALSE;
  }
  else {
    Info("ReadData", "input file %s opened", fFileName.Data());
  }

  gROOT->cd();

  TH2D* h[10];
  TObjArray* inArr = NULL;

  if (fMCinput) {  /// mc pid method
    if (fMomDep) {
      std::vector<TString> histnames {"MC_electron_p_eloss", "MC_pion_p_eloss", "MC_kaon_p_eloss", "MC_proton_p_eloss",
                                      "MC_muon_p_eloss"};
      inArr = new TObjArray(histnames.size());
      for (size_t i = 0; i < histnames.size(); i++) {
        h[i] = (TH2D*) histFile->Get(histnames[i]);
        if (!h[i]) {
          Info("ReadData", "no input histogram %s", histnames[i].Data());
          continue;
        }

        //set name and title
        h[i]->SetNameTitle(histnames[i], histnames[i]);

        //normalize each momentum bin to 1
        for (Int_t x = 1; x <= h[i]->GetNbinsX(); x++) {
          Double_t sum = 0.;
          for (Int_t y = 1; y <= h[i]->GetNbinsY(); y++)
            sum += h[i]->GetBinContent(x, y);
          for (Int_t y = 1; y <= h[i]->GetNbinsY(); y++)
            if (sum > 0) h[i]->SetBinContent(x, y, h[i]->GetBinContent(x, y) / sum);
        }

        inArr->Add(h[i]);
      }
    }
    if (!fMomDep) {
      std::vector<TString> histnames {"MC_electron_eloss", "MC_pion_eloss", "MC_kaon_eloss", "MC_proton_eloss",
                                      "MC_muon_eloss"};
      inArr = new TObjArray(histnames.size());
      for (size_t i = 0; i < histnames.size(); i++) {
        h[i] = (TH2D*) histFile->Get(histnames[i]);
        if (!h[i]) {
          Info("ReadData", "no input histogram %s", histnames[i].Data());
          continue;
        }

        //set name and title
        h[i]->SetNameTitle(histnames[i], histnames[i]);

        //normalize spectrum to 1
        h[i]->Scale(1. / h[i]->Integral());

        inArr->Add(h[i]);
      }
    }
  }
  else {  /// data driven method
    if (fMomDep) {
      std::vector<TString> histnames {"ELE_electron_p_eloss", "PIO_pion_p_eloss", "ELE_kaon_p_eloss",
                                      "ELE_proton_p_eloss", "ELE_muon_p_eloss"};
      inArr = new TObjArray(histnames.size());
      for (size_t i = 0; i < histnames.size(); i++) {
        h[i] = (TH2D*) histFile->Get(histnames[i]);
        h[i]->SetNameTitle(histnames[i], histnames[i]);
        if (!h[i]) {
          Info("ReadData", "no input histogram %s", histnames[i].Data());
          continue;
        }

        //set name and title
        h[i]->SetNameTitle(histnames[i], histnames[i]);

        //normalize each momentum bin to 1
        for (Int_t x = 1; x <= h[i]->GetNbinsX(); x++) {
          Double_t sum = 0.;
          for (Int_t y = 1; y <= h[i]->GetNbinsY(); y++)
            sum += h[i]->GetBinContent(x, y);
          for (Int_t y = 1; y <= h[i]->GetNbinsY(); y++)
            if (sum > 0) h[i]->SetBinContent(x, y, h[i]->GetBinContent(x, y) / sum);
        }

        inArr->Add(h[i]);
      }
    }
    if (!fMomDep) {
      std::vector<TString> histnames {"ELE_electron_eloss", "PIO_pion_eloss", "ELE_kaon_eloss", "ELE_proton_eloss",
                                      "ELE_muon_eloss"};
      inArr = new TObjArray(histnames.size());
      for (size_t i = 0; i < histnames.size(); i++) {
        h[i] = (TH2D*) histFile->Get(histnames[i]);
        if (!h[i]) {
          Info("ReadData", "no input histogram %s", histnames[i].Data());
          continue;
        }

        //set name and title
        h[i]->SetNameTitle(histnames[i], histnames[i]);

        //normalize spectrum to 1
        h[i]->Scale(1. / h[i]->Integral());

        inArr->Add(h[i]);
      }
    }
  }

  Int_t particle = 0;
  for (Int_t i = 0; i < inArr->GetEntriesFast(); i++) {

    TH1* hist    = (TH1*) inArr->At(i)->Clone();
    TString name = hist->GetTitle();

    if (hist->GetEntries() < 1000) Info("ReadData", "input histogram is almost empty for %s", name.Data());

    // check particles
    if (name.Contains("electron")) particle = CbmTrdSetTracksPidLike::kElectron;
    else if (name.Contains("pion"))
      particle = CbmTrdSetTracksPidLike::kPion;
    else if (name.Contains("kaon"))
      particle = CbmTrdSetTracksPidLike::kKaon;
    else if (name.Contains("proton"))
      particle = CbmTrdSetTracksPidLike::kProton;
    else if (name.Contains("muon"))
      particle = CbmTrdSetTracksPidLike::kMuon;
    else
      continue;

    // add to hist array
    Info("ReadData", "particle histogram %s added to array at %d", name.Data(), particle);

    fHistdEdx->AddAt(hist, particle);
  }

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  /// clean up
  histFile->Close();
  delete histFile;

  return kTRUE;
}

//_________________________________________________________________________
// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmTrdSetTracksPidLike::Init()
{

  //
  // Initalize data members
  //

  /// input array
  fHistdEdx = new TObjArray(fgkNParts);
  fHistdEdx->SetOwner();

  // Read the data from ROOT file. In case of problems return kFATAL;
  if (!ReadData()) return kFATAL;

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    Error("Init", "RootManager not instantised!");
    return kFATAL;
  }

  // Get GlobalTack array
  fglobalTrackArray = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (!fglobalTrackArray) {
    Error("Init", "No GlobalTack array!");
    return kFATAL;
  }

  // Get TrdTrack array
  fTrackArray = (TClonesArray*) ioman->GetObject("TrdTrack");  //=>SG
  if (!fTrackArray) {
    Error("Init", "No TrdTrack array!");
    return kFATAL;
  }

  // Get TrdTrack array
  fTrdHitArray = (TClonesArray*) ioman->GetObject("TrdHit");  //=>SG
  if (!fTrdHitArray) {
    Error("Init", "No TrdHit array!");
    return kFATAL;
  }

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Public method Exec   --------------------------------------------
void CbmTrdSetTracksPidLike::Exec(Option_t*)
{

  Double_t momentum;
  Double_t prob[fgkNParts];
  Double_t probTotal;


  if (!fTrackArray) return;

  Int_t nTracks = fglobalTrackArray->GetEntriesFast();
  /// loop over global tracks
  for (Int_t iTrack = 0; iTrack < nTracks; iTrack++) {

    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fglobalTrackArray->At(iTrack);

    Int_t trdTrackIndex = gTrack->GetTrdTrackIndex();
    if (trdTrackIndex == -1) {
      //  cout <<" -W- CbmTrdSetTracksPidLike::Exec : no Trd track"<<endl;
      continue;
    }

    /// get trd track
    CbmTrdTrack* pTrack = (CbmTrdTrack*) fTrackArray->At(trdTrackIndex);
    if (!pTrack) {
      Warning("Exec", "No Trd track pointer");
      continue;
    }

    /// only trd tracks with mimimum 1 reconstructed point
    if (pTrack->GetNofHits() < 1) continue;
    else
      fNofTracks++;


    probTotal = 0.0;
    for (Int_t iSpecies = 0; iSpecies < fgkNParts; iSpecies++) {
      prob[iSpecies] = 1.0;
    }

    /// Get the momentum from the first trd station
    if (TMath::Abs(pTrack->GetParamFirst()->GetQp()) > 0.) {
      momentum = TMath::Abs(1. / (pTrack->GetParamFirst()->GetQp()));
    }
    else if (TMath::Abs(pTrack->GetParamLast()->GetQp()) > 0.) {
      momentum = TMath::Abs(1. / (pTrack->GetParamLast()->GetQp()));
    }
    else {
      Warning("Exec", "Could not assign any momentum to the track, use p=0.");
      momentum = 0.;
    }


    Double_t dEdx  = 0.;
    Double_t dEsum = 0.;

    /// loop over all hits
    for (Int_t iTRD = 0; iTRD < pTrack->GetNofHits(); iTRD++) {
      Int_t index       = pTrack->GetHitIndex(iTRD);
      CbmTrdHit* trdHit = (CbmTrdHit*) fTrdHitArray->At(index);

      dEdx = trdHit->GetELoss() * 1.e+6;    //GeV->keV
      dEsum += trdHit->GetELoss() * 1.e+6;  //GeV->keV

      for (Int_t iSpecies = 0; iSpecies < fgkNParts; iSpecies++) {

        prob[iSpecies] *= GetProbability(iSpecies, momentum, dEdx);
        //	if(iSpecies==0) std::cout<<momentum<<"   " << dEdx<<"   " << GetProbability(iSpecies, momentum, dEdx)<<std::endl;
      }  //loop species
    }    // loop TRD hits

    /// calculate denominator for reasonable probabilities
    for (Int_t iSpecies = 0; iSpecies < fgkNParts; iSpecies++) {
      if (prob[iSpecies] >= 0. && prob[iSpecies] <= 1.) probTotal += prob[iSpecies];
    }

    /// normalize to 1
    for (Int_t iSpecies = 0; iSpecies < fgkNParts; iSpecies++) {
      if (probTotal > 0) {
        //	std::cout<<iSpecies<<"   " << probTotal<<"   " << prob[iSpecies]<<std::endl;
        prob[iSpecies] /= probTotal;
      }
      else {
        prob[iSpecies] = -1.5;
      }
    }


    pTrack->SetELoss(dEsum);

    /// fill track values
    pTrack->SetPidLikeEL(prob[CbmTrdSetTracksPidLike::kElectron]);
    pTrack->SetPidLikePI(prob[CbmTrdSetTracksPidLike::kPion]);
    pTrack->SetPidLikeKA(prob[CbmTrdSetTracksPidLike::kKaon]);
    pTrack->SetPidLikePR(prob[CbmTrdSetTracksPidLike::kProton]);
    pTrack->SetPidLikeMU(prob[CbmTrdSetTracksPidLike::kMuon]);
  }
}
// -------------------------------------------------------------------------

Double_t CbmTrdSetTracksPidLike::GetProbability(Int_t k, Double_t mom, Double_t dedx) const
{
  //
  // Gets the Probability of having dedx at a given momentum (mom)
  // and particle type k from the precalculated de/dx distributions
  //

  /// useless protection
  if (k < 0 || k > fgkNParts) { return -999.; }

  /// histogram has TRD momentum at inner point vs. dedx sinal [keV]
  TH1* hist = (TH1*) fHistdEdx->At(k);
  if (!hist) { return -999.; }

  /// check for entries/ non-empty histograms
  if (hist->GetEntries() < 1000.) { return -999.; }

  Int_t ndim = hist->GetDimension();

  Float_t maxY = hist->GetYaxis()->GetXmax();
  Float_t maxX = hist->GetXaxis()->GetXmax();

  /// check for overlow
  Bool_t overflowY = (dedx > maxY);
  Bool_t overflowX = (ndim == 1 ? (dedx > maxX) : (mom > maxX));

  /// use bin width of last bin (correct in case of logarithmic, arbitrary binnning)
  Float_t binwidthY = (ndim == 1 ? 0. : hist->GetYaxis()->GetBinWidth(hist->GetNbinsY()));
  Float_t binwidthX = hist->GetXaxis()->GetBinWidth(hist->GetNbinsX());

  /// find bin depending on overflow in X,Y
  Int_t bin = 0;
  if (ndim == 1) {  // 1-dimensional input histograms
    hist->FindBin((overflowX ? maxX - binwidthX : dedx));
  }
  else {  // 2-dimensional input histograms
    hist->FindBin((overflowX ? maxX - binwidthX : mom), (overflowY ? maxY - binwidthY : dedx));
  }

  /// interpolate empty bins or overflow bins
  if (TMath::Abs(hist->GetBinContent(bin)) < 1.e-15) {
    Double_t con = -999.;
    if (ndim == 1) {  // 1-dimensional input histograms
      con = hist->Interpolate((overflowX ? maxX - binwidthX : dedx));
    }
    else {  // 2-dimensional input histograms
      con = ((TH2*) hist)->Interpolate((overflowX ? maxX - binwidthX : mom), (overflowY ? maxY - binwidthY : dedx));
    }
    return con;
  }
  else {
    return hist->GetBinContent(bin);
  }
}

// -----   Public method Finish   ------------------------------------------
void CbmTrdSetTracksPidLike::Finish() {}
// -------------------------------------------------------------------------


ClassImp(CbmTrdSetTracksPidLike)
