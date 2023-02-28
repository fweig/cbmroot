/* Copyright (C) 2023-2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: S.Gorbunov[committer], N.Bluhme */

/// @file    CbmBbaAlignmentTask.cxx
/// @author  Sergey Gorbunov
/// @author  Nora Bluhme
/// @date    20.01.2023
/// @brief   Task class for alignment
///

// Cbm Headers ----------------------
#include "CbmBbaAlignmentTask.h"

#include "CbmL1PFFitter.h"

// ROOT headers

#include "FairRootManager.h"

#include "TClonesArray.h"
#include "TFile.h"
#include "TH1F.h"
#include "TRandom.h"

//

#include "CbmMvdHit.h"
#include "CbmStsHit.h"
#include "CbmStsSetup.h"
#include "CbmStsTrack.h"

#include "bba/BBA.h"

// c++ and std headers

#include <iostream>


CbmBbaAlignmentTask::CbmBbaAlignmentTask(const char* name, Int_t iVerbose, TString histoFileName)
  : FairTask(name, iVerbose)
  , fHistoFileName(histoFileName)
{
  TFile* curFile           = gFile;
  TDirectory* curDirectory = gDirectory;

  if (!(fHistoFileName == "")) { fHistoFile = new TFile(fHistoFileName.Data(), "RECREATE"); }
  else {
    fHistoFile = gFile;
  }

  fHistoFile->cd();

  fHistoDir = fHistoFile->mkdir("Bba");
  fHistoDir->cd();

  hTestHisto = new TH1F("hTestHisto", "hTestHisto", 100, 0., 1.);

  gFile      = curFile;
  gDirectory = curDirectory;
}


CbmBbaAlignmentTask::~CbmBbaAlignmentTask() {}


InitStatus CbmBbaAlignmentTask::Init()
{
  //Get ROOT Manager
  FairRootManager* ioman = FairRootManager::Instance();

  if (!ioman) {
    LOG(error) << "CbmBbaAlignmentTask::Init :: RootManager not instantiated!";
    return kERROR;
  }

  // Get hits

  fInputMvdHits = (TClonesArray*) ioman->GetObject("MvdHit");
  fInputStsHits = (TClonesArray*) ioman->GetObject("StsHit");

  // Get sts tracks
  fInputStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (!fInputStsTracks) {
    LOG(error) << "CbmBbaAlignmentTask::Init: Sts track-array not found!";
    return kERROR;
  }

  // MC track match
  fInputMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (!fInputMcTracks) {
    Warning("CbmBbaAlignmentTask::Init", "mc track array not found!");
    return kERROR;
  }

  // Track match
  fInputStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (fInputStsTrackMatches == 0) {
    LOG(error) << "CbmBbaAlignmentTask::Init: track match array not found!";
    return kERROR;
  }

  fTracks.clear();
  fMvdHits.clear();
  fStsHits.clear();

  return kSUCCESS;
}

void CbmBbaAlignmentTask::Exec(Option_t* /*opt*/)
{

  std::cout << "BBA: exec event N " << fNEvents << std::endl;

  fNEvents++;

  if ((int) fTracks.size() >= fMaxNtracks) { return; }

  // select STS tracks for alignment and store them

  for (int iTr = 0; iTr < fInputStsTracks->GetEntriesFast(); iTr++) {

    if ((int) fTracks.size() >= fMaxNtracks) { break; }

    CbmStsTrack* stsTrack = ((CbmStsTrack*) fInputStsTracks->At(iTr));

    if (stsTrack->GetNofStsHits() < 8) continue;
    const auto* par = stsTrack->GetParamFirst();
    if (!std::isfinite(par->GetQp())) continue;
    if (fabs(par->GetQp()) > 1.) continue;  // select tracks with min 1 GeV momentum

    CbmStsTrack track;

    // copy track parameters

    track.SetParamFirst(stsTrack->GetParamFirst());
    track.SetParamLast(stsTrack->GetParamLast());
    track.SetChiSq(stsTrack->GetChiSq());
    track.SetNDF(stsTrack->GetNDF());

    // copy hits to the local arrays and set the track hit indices correspondingly

    for (int ih = 0; ih < stsTrack->GetNofMvdHits(); ih++) {
      assert(fInputMvdHits);
      int hitIndex  = stsTrack->GetMvdHitIndex(ih);
      CbmMvdHit hit = *dynamic_cast<CbmMvdHit*>(fInputMvdHits->At(hitIndex));
      track.AddMvdHit(fMvdHits.size());
      fMvdHits.push_back(hit);
    }

    for (int ih = 0; ih < stsTrack->GetNofStsHits(); ih++) {
      assert(fInputStsHits);
      int hitIndex  = stsTrack->GetStsHitIndex(ih);
      CbmStsHit hit = *dynamic_cast<CbmStsHit*>(fInputStsHits->At(hitIndex));
      track.AddStsHit(fStsHits.size());
      fStsHits.push_back(hit);
    }

    fTracks.push_back(track);

  }  // tracks
}

void CbmBbaAlignmentTask::ApplyAlignment(const std::vector<double>& par)
{
  // apply alignment parameters to the hits
  for (unsigned int ih = 0; ih < fStsHits.size(); ih++) {

    const CbmStsHit& hitOrig = fStsHits[ih];

    int iStation = CbmStsSetup::Instance()->GetStationNumber(hitOrig.GetAddress());

    assert(iStation >= 0 && iStation < CbmStsSetup::Instance()->GetNofStations());

    double dx = par[3 * iStation + 0];
    double dy = par[3 * iStation + 1];
    double dz = par[3 * iStation + 2];

    CbmStsHit& hitAligned = fStsHitsAligned[ih];

    hitAligned.SetX(hitOrig.GetX() + dx);
    hitAligned.SetY(hitOrig.GetY() + dy);
    hitAligned.SetZ(hitOrig.GetZ() + dz);
  }
}

double CbmBbaAlignmentTask::CostFunction(const std::vector<double>& par)
{

  // apply new parameters to the hits

  ApplyAlignment(par);

  auto newTracks = fTracks;

  CbmL1PFFitter fitter;
  fitter.Fit(newTracks, fMvdHits, fStsHitsAligned, fTrackPdg);

  double chi2Total  = 0;
  long int ndfTotal = 1;

  int nGoodTracks = 0;
  for (unsigned int iTr = 0; iTr < newTracks.size(); iTr++) {
    if (!std::isfinite(newTracks[iTr].GetChiSq())) continue;
    if (newTracks[iTr].GetChiSq() < 0.) continue;
    if (newTracks[iTr].GetNDF() < 0.) continue;

    nGoodTracks++;
    chi2Total += newTracks[iTr].GetChiSq();
    ndfTotal += newTracks[iTr].GetNDF();
  }

  double cost = chi2Total / ndfTotal;
  std::cout << "BBA: cost function:  n tracks " << nGoodTracks << ", cost " << cost
            << ", diff to ideal cost: " << cost - fCostIdeal << std::endl;
  return cost;
  if (nGoodTracks == (int) fTracks.size()) { return cost; }
  return 1.e30;
}

void CbmBbaAlignmentTask::Finish()
{
  //
  // perform the alignment
  //

  std::cout << "BBA: start the alignment procedure with " << fTracks.size() << " tracks ..." << std::endl;

  // init auxiliary arrays

  fStsHitsAligned = fStsHits;
  fTrackPdg.resize(fTracks.size(), 211);  // pion PDG

  // init alignmemt module

  int nStsStations = CbmStsSetup::Instance()->GetNofStations();
  int nParameters  = 3 * nStsStations;  // x,yz

  std::vector<bba::Parameter> par(nParameters);

  for (int ip = 0; ip < nParameters; ip++) {
    bba::Parameter& p = par[ip];
    p.SetActive(0);
    p.SetValue(0.);
    p.SetBoundaryMin(-2.);  // +-1 cm alignment range
    p.SetBoundaryMax(2.);
    p.SetStepMin(1.e-6);
    p.SetStepMax(0.1);
    p.SetStep(10.e-4);
  }

  par[3 * (nStsStations - 1) + 0].SetActive(0);  // fix the last station
  par[3 * (nStsStations - 1) + 1].SetActive(0);
  par[3 * (nStsStations - 1) + 2].SetActive(0);

  //par[3 * 1 + 1].SetActive(1);

  par[3 * 0 + 0].SetActive(1);
  par[3 * 0 + 1].SetActive(1);
  par[3 * 0 + 2].SetActive(1);

  par[3 * 1 + 0].SetActive(1);
  par[3 * 1 + 1].SetActive(1);
  par[3 * 1 + 2].SetActive(1);

  par[3 * 2 + 0].SetActive(1);
  par[3 * 2 + 1].SetActive(1);
  par[3 * 2 + 2].SetActive(1);

  par[3 * 3 + 0].SetActive(1);
  par[3 * 3 + 1].SetActive(1);
  par[3 * 3 + 2].SetActive(1);

  par[3 * 4 + 0].SetActive(1);
  par[3 * 4 + 1].SetActive(1);
  par[3 * 4 + 2].SetActive(1);

  par[3 * 5 + 0].SetActive(1);
  par[3 * 5 + 1].SetActive(1);
  par[3 * 5 + 2].SetActive(1);

  par[3 * 6 + 0].SetActive(1);
  par[3 * 6 + 1].SetActive(1);
  par[3 * 6 + 2].SetActive(1);

  par[3 * 7 + 0].SetActive(0);
  par[3 * 7 + 1].SetActive(0);
  par[3 * 7 + 2].SetActive(0);


  gRandom->SetSeed(1);

  for (int is = 0; is < nStsStations; is++) {
    bba::Parameter& px = par[3 * is + 0];
    bba::Parameter& py = par[3 * is + 1];
    bba::Parameter& pz = par[3 * is + 2];
    //py.SetActive(1);
    //pz.SetStepMin(1.e-4);
    // +- 0.5 cm misalignment

    //if (px.IsActive()) { px.SetValue(.1); }  //gRandom->Uniform(2 * d) - d); }
    //if (py.IsActive()) { py.SetValue(.1); }  //gRandom->Uniform(2. * d) - d); }
    //if (pz.IsActive()) { pz.SetValue(.1); }  //gRandom->Uniform(2. * d) - d); }
    double d = 0.5;
    if (px.IsActive()) { px.SetValue(gRandom->Uniform(2. * d) - d); }
    if (py.IsActive()) { py.SetValue(gRandom->Uniform(2. * d) - d); }
    if (pz.IsActive()) { pz.SetValue(gRandom->Uniform(2. * d) - d); }
  }

  bba::BBA alignment;

  alignment.setParameters(par);

  auto lambdaCostFunction = [this](const std::vector<double>& p) { return CostFunction(p); };

  alignment.setChi2(lambdaCostFunction);
  alignment.printInfo();

  std::vector<double> parAligned;
  std::vector<double> parMisaligned;
  {
    const std::vector<double>& r = alignment.getResult();
    for (unsigned int i = 0; i < r.size(); i++) {
      parMisaligned.push_back(r[i]);
      parAligned.push_back(0.);
    }
  }

  {
    ApplyAlignment(parMisaligned);

    CbmL1PFFitter fitter;
    auto oldTracks = fTracks;
    auto newTracks = fTracks;
    fitter.Fit(newTracks, fMvdHits, fStsHitsAligned, fTrackPdg);

    double chi2Total  = 0;
    long int ndfTotal = 1;

    int nGoodTracks = 0;
    fTracks.clear();
    for (unsigned int iTr = 0; iTr < newTracks.size(); iTr++) {
      if (!std::isfinite(newTracks[iTr].GetChiSq())) continue;
      if (newTracks[iTr].GetChiSq() < 0.) continue;
      if (newTracks[iTr].GetNDF() < 0.) continue;
      fTracks.push_back(oldTracks[iTr]);
      nGoodTracks++;
      chi2Total += newTracks[iTr].GetChiSq();
      ndfTotal += newTracks[iTr].GetNDF();
    }

    std::cout << "Initial nTracks " << nGoodTracks << " chi2/ndf " << chi2Total / ndfTotal << std::endl;
  }

  fCostIdeal = CostFunction(parAligned);

  std::cout << " cost function for the true parameters: " << fCostIdeal << std::endl;

  alignment.align();

  std::cout << " cost function for the true parameters: " << fCostIdeal << std::endl;

  std::cout << " Misaligned parameters: " << std::endl;
  for (int is = 0; is < nStsStations; is++) {
    const std::vector<double>& r = parMisaligned;
    std::cout << "Sts station " << is << ": x " << r[3 * is + 0] << " y " << r[3 * is + 1] << " z " << r[3 * is + 2]
              << std::endl;
  }

  std::cout << " Alignment results: " << std::endl;

  for (int is = 0; is < nStsStations; is++) {
    const std::vector<double>& r = alignment.getResult();
    std::cout << "Sts station " << is << ": x " << r[3 * is + 0] << " y " << r[3 * is + 1] << " z " << r[3 * is + 2]
              << std::endl;
  }

  // store the histograms

  TDirectory* curr   = gDirectory;
  TFile* currentFile = gFile;
  // Open output file and write histograms

  fHistoFile->cd();
  WriteHistosCurFile(fHistoDir);
  if (!(fHistoFileName == "")) {
    fHistoFile->Close();
    fHistoFile->Delete();
  }
  gFile      = currentFile;
  gDirectory = curr;
}

void CbmBbaAlignmentTask::WriteHistosCurFile(TObject* obj)
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

ClassImp(CbmBbaAlignmentTask);
