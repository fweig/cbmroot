/* Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak, Volker Friese [committer] */

//-----------------------------------------------------------
//-----------------------------------------------------------

// Cbm Headers ----------------------
#include "CbmKFParticleFinderQa.h"

#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmTrack.h"
#include "CbmTrackMatchNew.h"

#include "FairRunAna.h"

//KF Particle headers
#include "KFMCTrack.h"
#include "KFParticleMatch.h"
#include "KFParticleTopoReconstructor.h"
#include "KFTopoPerformance.h"

//ROOT headers
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TObject.h"
#include "TSystem.h"

//c++ and std headers
#include <iomanip>
#include <iostream>
#include <vector>

#include <cmath>
using std::vector;

CbmKFParticleFinderQa::CbmKFParticleFinderQa(const char* name, Int_t iVerbose, const KFParticleTopoReconstructor* tr,
                                             TString outFileName)
  : FairTask(name, iVerbose)
  , fOutFileName(outFileName)
{
  for (Int_t i = 0; i < 5; i++)
    fTime[i] = 0;

  fTopoPerformance = new KFTopoPerformance;
  fTopoPerformance->SetTopoReconstructor(tr);

  TFile* curFile           = gFile;
  TDirectory* curDirectory = gDirectory;

  if (!(fOutFileName == "")) fOutFile = new TFile(fOutFileName.Data(), "RECREATE");
  else {
    fOutFile = gFile;
  }
  fTopoPerformance->CreateHistos("KFTopoReconstructor", fOutFile, tr->GetKFParticleFinder()->GetReconstructionList());

  gFile      = curFile;
  gDirectory = curDirectory;
}

CbmKFParticleFinderQa::~CbmKFParticleFinderQa()
{
  if (fTopoPerformance) delete fTopoPerformance;

  if (fSaveParticles) fRecParticles->Delete();
  if (fSaveMCParticles) {
    fMCParticles->Delete();
    fMatchParticles->Delete();
  }
}

InitStatus CbmKFParticleFinderQa::Init()
{
  //Get ROOT Manager
  FairRootManager* ioman = FairRootManager::Instance();

  if (ioman == 0) {
    Error("CbmKFParticleFinderQa::Init", "RootManager not instantiated!");
    return kERROR;
  }

  // check the mode
  fLegacyEventMode = 0;
  if (!ioman->CheckBranch("CbmEvent") && ioman->CheckBranch("MCTrack")) { fLegacyEventMode = 1; }

  // MC Tracks
  if (!fLegacyEventMode) {
    FairRootManager* fManger    = FairRootManager::Instance();
    CbmMCDataManager* mcManager = (CbmMCDataManager*) fManger->GetObject("MCDataManager");
    if (mcManager == 0) Error("CbmKFParticleFinderQa::Init", "MC Data Manager not found!");

    fMCTrackArray = mcManager->InitBranch("MCTrack");

    if (fMCTrackArray == 0) {
      Error("CbmKFParticleFinderQa::Init", "mc track array not found!");
      return kERROR;
    }

    fMcEventList = (CbmMCEventList*) ioman->GetObject("MCEventList.");
    if (fMcEventList == 0) {
      Error("CbmKFParticleFinderQa::Init", "MC Event List not found!");
      return kERROR;
    }

    fRecoEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
    if (nullptr == fRecoEvents) { LOG(warn) << GetName() << ": No event array! Will process entire tree."; }
  }
  else {
    fMCTrackArrayEvent = (TClonesArray*) ioman->GetObject("MCTrack");
  }

  // Track match
  fTrackMatchArray = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (fTrackMatchArray == 0) {
    Error("CbmKFParticleFinderQa::Init", "track match array not found!");
    return kERROR;
  }

  if (fSaveParticles) {
    // create and register TClonesArray with output reco particles
    fRecParticles = new TClonesArray("KFParticle", 100);
    ioman->Register("RecoParticles", "KFParticle", fRecParticles, IsOutputBranchPersistent("RecoParticles"));
  }

  if (fSaveMCParticles) {
    // create and register TClonesArray with output MC particles
    fMCParticles = new TClonesArray("KFMCParticle", 100);
    ioman->Register("KFMCParticles", "KFParticle", fMCParticles, IsOutputBranchPersistent("KFMCParticles"));

    // create and register TClonesArray with matching between reco and MC particles
    fMatchParticles = new TClonesArray("KFParticleMatch", 100);
    ioman->Register("KFParticleMatch", "KFParticle", fMatchParticles, IsOutputBranchPersistent("KFParticleMatch"));
  }

  return kSUCCESS;
}

void CbmKFParticleFinderQa::Exec(Option_t* /*opt*/)
{
  if (fSuperEventAnalysis) {
    LOG(error) << GetName() << " SuperEventAnalysis option currently doesn't work";
    return;
  }

  if (fSaveParticles) fRecParticles->Delete();
  if (fSaveMCParticles) {
    fMCParticles->Delete();
    fMatchParticles->Delete();
  }

  // make sure that the number of events in time slice is 1

  int nRecoEvents = 1;
  int nMCEvents   = 1;

  if (!fLegacyEventMode) {
    if (fRecoEvents) { nRecoEvents = fRecoEvents->GetEntriesFast(); }
    nMCEvents = fMcEventList->GetNofEvents();
  }

  if (nMCEvents != 1 || nRecoEvents != 1) {
    LOG(warning) << GetName() << " the task doesn't properly work with more than one event in the time slice";
  }

  vector<KFMCTrack> mcTracks;
  vector<int> mcToKFmcMap[nMCEvents];

  int mcIndexOffset = 0;

  for (int iMCEvent = 0; iMCEvent < nMCEvents; iMCEvent++) {

    CbmLink mcEventLink = fMcEventList->GetEventLinkByIndex(iMCEvent);
    int nMCTracks       = 0;
    if (!fLegacyEventMode) {
      nMCTracks = fMCTrackArray->Size(mcEventLink);
      mcToKFmcMap[iMCEvent].resize(nMCTracks, -1);
    }
    else {
      nMCTracks = fMCTrackArrayEvent->GetEntriesFast();
    }

    for (Int_t iMC = 0; iMC < nMCTracks; iMC++) {

      CbmMCTrack* cbmMCTrack;
      CbmLink mcTrackLink = mcEventLink;
      mcTrackLink.SetIndex(iMC);
      if (!fLegacyEventMode) { cbmMCTrack = (CbmMCTrack*) fMCTrackArray->Get(mcTrackLink); }
      else {
        cbmMCTrack = (CbmMCTrack*) fMCTrackArrayEvent->At(iMC);
      }

      KFMCTrack kfMCTrack;
      kfMCTrack.SetX(cbmMCTrack->GetStartX());
      kfMCTrack.SetY(cbmMCTrack->GetStartY());
      kfMCTrack.SetZ(cbmMCTrack->GetStartZ());
      kfMCTrack.SetPx(cbmMCTrack->GetPx());
      kfMCTrack.SetPy(cbmMCTrack->GetPy());
      kfMCTrack.SetPz(cbmMCTrack->GetPz());

      Int_t pdg  = cbmMCTrack->GetPdgCode();
      Double_t q = 1;
      if (pdg < 9999999 && ((TParticlePDG*) TDatabasePDG::Instance()->GetParticle(pdg)))
        q = TDatabasePDG::Instance()->GetParticle(pdg)->Charge() / 3.0;
      else if (pdg == 1000010020)
        q = 1;
      else if (pdg == -1000010020)
        q = -1;
      else if (pdg == 1000010030)
        q = 1;
      else if (pdg == -1000010030)
        q = -1;
      else if (pdg == 1000020030)
        q = 2;
      else if (pdg == -1000020030)
        q = -2;
      else if (pdg == 1000020040)
        q = 2;
      else if (pdg == -1000020040)
        q = -2;
      else
        q = 0;
      Double_t p = cbmMCTrack->GetP();

      if (cbmMCTrack->GetMotherId() >= 0) kfMCTrack.SetMotherId(cbmMCTrack->GetMotherId() + mcIndexOffset);
      else
        kfMCTrack.SetMotherId(-iMCEvent - 1);
      kfMCTrack.SetQP(q / p);
      kfMCTrack.SetPDG(pdg);
      kfMCTrack.SetNMCPoints(0);

      if (!fLegacyEventMode) { mcToKFmcMap[iMCEvent][iMC] = mcTracks.size(); }
      mcTracks.push_back(kfMCTrack);
    }

    mcIndexOffset += nMCTracks;
  }

  Int_t ntrackMatches = fTrackMatchArray->GetEntriesFast();
  vector<int> trackMatch(ntrackMatches, -1);

  for (int iTr = 0; iTr < ntrackMatches; iTr++) {

    CbmTrackMatchNew* stsTrackMatch = (CbmTrackMatchNew*) fTrackMatchArray->At(iTr);

    if (stsTrackMatch->GetNofLinks() == 0) continue;
    Float_t bestWeight  = 0.f;
    Float_t totalWeight = 0.f;
    Int_t mcTrackId     = -1;
    CbmLink link;

    for (int iLink = 0; iLink < stsTrackMatch->GetNofLinks(); iLink++) {
      totalWeight += stsTrackMatch->GetLink(iLink).GetWeight();
      if (stsTrackMatch->GetLink(iLink).GetWeight() > bestWeight) {
        bestWeight   = stsTrackMatch->GetLink(iLink).GetWeight();
        int iMCTrack = stsTrackMatch->GetLink(iLink).GetIndex();
        link         = stsTrackMatch->GetLink(iLink);
        if (!fLegacyEventMode) {
          int eventIndex = fMcEventList->GetEventIndex(link);
          if (eventIndex >= 0) { mcTrackId = mcToKFmcMap[eventIndex][iMCTrack]; }
        }
        else {
          mcTrackId = stsTrackMatch->GetLink(iLink).GetIndex();
        }
      }
    }

    if (mcTrackId < 0) continue;

    if (bestWeight / totalWeight < 0.7) continue;
    //       if(mcTrackId >= nMCTracks || mcTrackId < 0)
    //       {
    //         std::cout << "Sts Matching is wrong!    StsTackId = " << mcTrackId << " N mc tracks = " << nMCTracks << std::endl;
    //         continue;
    //       }

    if (TMath::Abs(mcTracks[mcTrackId].PDG()) > 4000
        && !(TMath::Abs(mcTracks[mcTrackId].PDG()) == 1000010020 || TMath::Abs(mcTracks[mcTrackId].PDG()) == 1000010030
             || TMath::Abs(mcTracks[mcTrackId].PDG()) == 1000020030
             || TMath::Abs(mcTracks[mcTrackId].PDG()) == 1000020040))
      continue;

    mcTracks[mcTrackId].SetReconstructed();
    trackMatch[iTr] = mcTrackId;
  }

  fTopoPerformance->SetMCTracks(mcTracks);
  fTopoPerformance->SetTrackMatch(trackMatch);

  fTopoPerformance->CheckMCTracks();
  fTopoPerformance->MatchTracks();
  fTopoPerformance->FillHistos();

  fNEvents++;
  fTime[4] += fTopoPerformance->GetTopoReconstructor()->Time();
  for (int iT = 0; iT < 4; iT++)
    fTime[iT] += fTopoPerformance->GetTopoReconstructor()->StatTime(iT);
  if (fNEvents % fPrintFrequency == 0) {
    std::cout << "Topo reconstruction time"
              << " Real = " << std::setw(10) << fTime[4] / fNEvents * 1.e3 << " ms" << std::endl;
    std::cout << "    Init                " << fTime[0] / fNEvents * 1.e3 << " ms" << std::endl;
    std::cout << "    PV Finder           " << fTime[1] / fNEvents * 1.e3 << " ms" << std::endl;
    std::cout << "    Sort Tracks         " << fTime[2] / fNEvents * 1.e3 << " ms" << std::endl;
    std::cout << "    KF Particle Finder  " << fTime[3] / fNEvents * 1.e3 << " ms" << std::endl;
  }

  // save particles to a ROOT file
  if (fSaveParticles) {
    for (unsigned int iP = 0; iP < fTopoPerformance->GetTopoReconstructor()->GetParticles().size(); iP++) {
      new ((*fRecParticles)[iP]) KFParticle(fTopoPerformance->GetTopoReconstructor()->GetParticles()[iP]);
    }
  }

  if (fSaveMCParticles) {
    for (unsigned int iP = 0; iP < fTopoPerformance->GetTopoReconstructor()->GetParticles().size(); iP++) {
      new ((*fMatchParticles)[iP]) KFParticleMatch();
      KFParticleMatch* p = (KFParticleMatch*) (fMatchParticles->At(iP));

      Short_t matchType = 0;
      int iMCPart       = -1;
      if (!(fTopoPerformance->ParticlesMatch()[iP].IsMatchedWithPdg()))  //background
      {
        if (fTopoPerformance->ParticlesMatch()[iP].IsMatched()) {
          iMCPart   = fTopoPerformance->ParticlesMatch()[iP].GetBestMatchWithPdg();
          matchType = 1;
        }
      }
      else {
        iMCPart   = fTopoPerformance->ParticlesMatch()[iP].GetBestMatchWithPdg();
        matchType = 2;
      }

      p->SetMatch(iMCPart);
      p->SetMatchType(matchType);
    }

    for (unsigned int iP = 0; iP < fTopoPerformance->MCParticles().size(); iP++) {
      new ((*fMCParticles)[iP]) KFMCParticle(fTopoPerformance->MCParticles()[iP]);
    }
  }
}

void CbmKFParticleFinderQa::Finish()
{
  if (fSuperEventAnalysis) {
    fTopoPerformance->SetPrintEffFrequency(1);

    vector<KFMCTrack> mcTracks(0);
    Int_t ntrackMatches = fTopoPerformance->GetTopoReconstructor()->GetParticles().size();
    vector<int> trackMatch(ntrackMatches, -1);

    fTopoPerformance->SetMCTracks(mcTracks);
    fTopoPerformance->SetTrackMatch(trackMatch);
    fTopoPerformance->CheckMCTracks();
    fTopoPerformance->MatchTracks();
    fTopoPerformance->FillHistos();

    fTime[4] += fTopoPerformance->GetTopoReconstructor()->Time();
    for (int iT = 0; iT < 4; iT++)
      fTime[iT] += fTopoPerformance->GetTopoReconstructor()->StatTime(iT);

    std::cout << "Topo reconstruction time"
              << " Real = " << std::setw(10) << fTime[4] * 1.e3 << " ms" << std::endl;
    std::cout << "    Init                " << fTime[0] * 1.e3 << " ms" << std::endl;
    std::cout << "    PV Finder           " << fTime[1] * 1.e3 << " ms" << std::endl;
    std::cout << "    Sort Tracks         " << fTime[2] * 1.e3 << " ms" << std::endl;
    std::cout << "    KF Particle Finder  " << fTime[3] * 1.e3 << " ms" << std::endl;
  }

  TDirectory* curr   = gDirectory;
  TFile* currentFile = gFile;
  // Open output file and write histograms

  fOutFile->cd();
  WriteHistosCurFile(fTopoPerformance->GetHistosDirectory());

  if (fCheckDecayQA && fDecayToAnalyse > -1) {
    if (fDecayToAnalyse < 0) Error("CbmKFParticleFinderQa::Finish", "Please specify the decay to be analysed.");
    else
      CheckDecayQA();
  }

  if (!(fOutFileName == "")) {
    fOutFile->Close();
    fOutFile->Delete();
  }
  gFile      = currentFile;
  gDirectory = curr;

  std::fstream eff(fEfffileName.Data(), std::fstream::out);
  eff << fTopoPerformance->fParteff;
  eff.close();
}

void CbmKFParticleFinderQa::WriteHistosCurFile(TObject* obj)
{
  if (!obj->IsFolder()) obj->Write();
  else {
    TDirectory* cur    = gDirectory;
    TFile* currentFile = gFile;

    TDirectory* sub = cur->GetDirectory(obj->GetName());
    sub->cd();
    TList* listSub = (static_cast<TDirectory*>(obj))->GetList();
    TIter it(listSub);
    while (TObject* obj1 = it())
      WriteHistosCurFile(obj1);
    cur->cd();
    gFile      = currentFile;
    gDirectory = cur;
  }
}

void CbmKFParticleFinderQa::SetPrintEffFrequency(Int_t n)
{
  fTopoPerformance->SetPrintEffFrequency(n);
  fPrintFrequency = n;
}

void CbmKFParticleFinderQa::FitDecayQAHistograms(float sigma[14], const bool saveReferenceResults) const
{
  static const int nParameters       = 7;
  TString parameterName[nParameters] = {"X", "Y", "Z", "Px", "Py", "Pz", "E"};

  TH1F* histogram[nParameters * 2];
  TF1* fit[nParameters * 2];

  for (int iParameter = 0; iParameter < nParameters; iParameter++) {
    TString cloneResidualName = TString("hResidual") + parameterName[iParameter];
    histogram[iParameter] =
      (TH1F*) (fTopoPerformance->GetDecayResidual(fDecayToAnalyse, iParameter)->Clone(cloneResidualName.Data()));
    fit[iParameter] =
      new TF1(TString("fitResidual") + parameterName[iParameter], "gaus", histogram[iParameter]->GetXaxis()->GetXmin(),
              histogram[iParameter]->GetXaxis()->GetXmax());
    fit[iParameter]->SetLineColor(kRed);
    histogram[iParameter]->Fit(TString("fitResidual") + parameterName[iParameter], "QN", "",
                               histogram[iParameter]->GetXaxis()->GetXmin(),
                               histogram[iParameter]->GetXaxis()->GetXmax());
    sigma[iParameter] = fit[iParameter]->GetParameter(2);

    TString clonePullName = TString("hPull") + parameterName[iParameter];
    histogram[iParameter + nParameters] =
      (TH1F*) (fTopoPerformance->GetDecayPull(fDecayToAnalyse, iParameter)->Clone(clonePullName.Data()));
    fit[iParameter + nParameters] = new TF1(TString("fitPull") + parameterName[iParameter], "gaus",
                                            histogram[iParameter + nParameters]->GetXaxis()->GetXmin(),
                                            histogram[iParameter + nParameters]->GetXaxis()->GetXmax());
    fit[iParameter + nParameters]->SetLineColor(kRed);
    histogram[iParameter + nParameters]->Fit(TString("fitPull") + parameterName[iParameter], "QN", "",
                                             histogram[iParameter + nParameters]->GetXaxis()->GetXmin(),
                                             histogram[iParameter + nParameters]->GetXaxis()->GetXmax());
    sigma[iParameter + nParameters] = fit[iParameter + nParameters]->GetParameter(2);
  }

  if (saveReferenceResults) {
    TCanvas fitCanvas("fitCanvas", "fitCanvas", 1600, 800);
    fitCanvas.Divide(4, 4);

    int padMap[nParameters * 2] = {1, 2, 3, 9, 10, 11, 12, 5, 6, 7, 13, 14, 15, 16};
    for (int iHisto = 0; iHisto < nParameters * 2; iHisto++) {
      fitCanvas.cd(padMap[iHisto]);
      histogram[iHisto]->Draw();
      fit[iHisto]->Draw("same");
    }

    TString canvasFile = TString("FitQA_") + fTopoPerformance->fParteff.partName[fDecayToAnalyse] + TString(".pdf");
    fitCanvas.SaveAs(canvasFile.Data());
  }

  for (int iHisto = 0; iHisto < nParameters * 2; iHisto++)
    if (fit[iHisto]) delete fit[iHisto];
}

void CbmKFParticleFinderQa::CheckDecayQA()
{
  float sigma[14];
  FitDecayQAHistograms(sigma, true);

  TString referenceFileName =
    fReferenceResults + TString("/qa_") + fTopoPerformance->fParteff.partName[fDecayToAnalyse] + TString(".root");
  TString qaFileName = TString("qa_") + fTopoPerformance->fParteff.partName[fDecayToAnalyse] + TString(".root");

  int iQAFile = 2;
  while (!gSystem->AccessPathName(qaFileName)) {
    qaFileName = TString("qa_") + fTopoPerformance->fParteff.partName[fDecayToAnalyse];
    qaFileName += iQAFile;
    qaFileName += TString(".root");
    iQAFile++;
  }

  TFile* curFile           = gFile;
  TDirectory* curDirectory = gDirectory;
  TFile* qaFile            = new TFile(qaFileName.Data(), "RECREATE");

  TString qaHistoName = TString("qa_") + fTopoPerformance->fParteff.partName[fDecayToAnalyse];
  TH1F* qaHisto       = new TH1F(qaHistoName.Data(), qaHistoName.Data(), 16, 0, 16);

  TString binLabel[16] = {"#sigma_{x}",     "#sigma_{y}",     "#sigma_{z}",         "#sigma_{p_{x}}",
                          "#sigma_{p_{y}}", "#sigma_{p_{z}}", "#sigma_{E}",         "P_{x}",
                          "P_{y}",          "P_{z}",          "P_{p_{x}}",          "P_{p_{y}}",
                          "P_{p_{z}}",      "P_{E}",          "#varepsilon_{4#pi}", "#varepsilon_{KFP}"};
  for (int iBin = 0; iBin < 16; iBin++)
    qaHisto->GetXaxis()->SetBinLabel(iBin + 1, binLabel[iBin].Data());

  for (int iSigma = 0; iSigma < 14; iSigma++)
    qaHisto->SetBinContent(iSigma + 1, sigma[iSigma]);

  qaHisto->SetBinContent(15, fTopoPerformance->fParteff.GetTotal4piEfficiency(fDecayToAnalyse));
  qaHisto->SetBinContent(16, fTopoPerformance->fParteff.GetTotalKFPEfficiency(fDecayToAnalyse));

  qaHisto->Write();

  //compare with the reference results
  TFile* referenceFile = new TFile(referenceFileName.Data(), "READ");
  if (referenceFile->IsOpen()) {
    TH1F* referenceHisto = referenceFile->Get<TH1F>(qaHistoName);
    if (referenceHisto) {
      fTestOk = true;
      for (int iBin = 1; iBin <= 7; iBin++)
        fTestOk &=
          fabs(referenceHisto->GetBinContent(iBin) - qaHisto->GetBinContent(iBin)) / referenceHisto->GetBinContent(iBin)
          < 0.25;
      for (int iBin = 8; iBin <= 14; iBin++)
        fTestOk &=
          fabs(referenceHisto->GetBinContent(iBin) - qaHisto->GetBinContent(iBin)) / referenceHisto->GetBinContent(iBin)
          < 0.25;
      for (int iBin = 15; iBin <= 16; iBin++)
        fTestOk &=
          fabs(referenceHisto->GetBinContent(iBin) - qaHisto->GetBinContent(iBin)) / referenceHisto->GetBinContent(iBin)
          < 0.1;
    }
    referenceFile->Close();
    referenceFile->Delete();
  }
  else {
    LOG(error) << "Could not open file " << referenceFileName << " with reference histograms";
  }

  if (qaFile) {
    qaFile->Close();
    qaFile->Delete();
  }

  gFile      = curFile;
  gDirectory = curDirectory;
}

ClassImp(CbmKFParticleFinderQa);
