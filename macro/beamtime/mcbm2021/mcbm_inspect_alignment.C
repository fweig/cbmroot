/* Copyright (C) 2022 UGiessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Adrian Weber [committer]*/

void initHists(CbmHistManager* fHM, Int_t nofEvents);
void drawHists(CbmHistManager* fHM);
TVector3 extrpolate(CbmTofHit* tofHit, TVector3* vertex, Double_t Z);

void ProjectXY(std::string name, CbmHistManager* fHM, std::vector<TH1*>* HistoX, std::vector<std::string>* stringX,
               std::vector<TH1*>* HistoY, std::vector<std::string>* stringY);

void ProjectAxis(std::string name, CbmHistManager* fHM, char axis, std::vector<TH1*>* Histo,
                 std::vector<std::string>* string);

std::string FitTimeOffset(TH1* hist);

struct ClosestCbmHit {
  Bool_t Existing = false;
  Double_t X      = 10000.0;
  Double_t Y      = 10000.0;
  Double_t Z      = 10000.0;
  Double_t Dist   = 10000.0;
  size_t Indx     = -1;
  Double_t X_pos  = 10000.0;
  Double_t Y_pos  = 10000.0;
};

char outdir[256];

void mcbm_inspect_alignment(Int_t runId = 1588, Int_t nofEvents = 200)
{

  CbmHistManager* fHM = new CbmHistManager();

  TTree* InputTreeRECO;
  TFile* fRECO;

  TVector3 vertexPoint(-0.0, 0.0, 0.0);
  Double_t CutCloseDistStsTrack = 60.0;  // cm

  char name[256], dir[256];

  //sprintf(dir, "/data/cbmroot/cbmsource/macro/beamtime/mcbm2021/rec/digi5");
  //sprintf(dir, "/data/cbmroot/cbmsource/macro/beamtime/mcbm2021/rec/aligndigi5");
  //sprintf(dir, "/data/cbmroot/cbmsource/macro/beamtime/mcbm2021/rec/aligndigi5_2HitTrack");
  //sprintf(dir, "/data/cbmroot/cbmsource/macro/beamtime/mcbm2021/rec/aligndigi2_2HitTrack");
  //sprintf(dir, "/data/cbmroot/cbmsource/macro/beamtime/mcbm2021/rec/tofPar0");
  sprintf(dir, "/data/cbmroot/cbmsource/macro/beamtime/mcbm2021/rec/tofPar2_noAlign_L1");
  //sprintf(dir, "/data/cbmroot/cbmsource/macro/beamtime/mcbm2021/rec/tofPar0_noAlign_5hits");

  sprintf(name, "%s/reco_event_mcbm_%u.root", dir, runId);

  sprintf(outdir, "./results_mTofPar2_AlignmentMacro");

  std::cout << name << std::endl;

  initHists(fHM, nofEvents);

  auto tofHitYMin = 5000.0;
  auto tofHitYMax = -5000.0;

  fRECO = new TFile(name);
  if (!fRECO || fRECO->IsZombie() || fRECO->GetNkeys() < 1 || fRECO->TestBit(TFile::kRecovered)) { fRECO->Close(); }

  InputTreeRECO = (TTree*) fRECO->Get("cbmsim");
  if (InputTreeRECO == nullptr) std::cout << "No cbmsim found" << std::endl;

  TClonesArray* fTofHit   = nullptr;
  TClonesArray* fTofTrack = nullptr;
  TClonesArray* fStsHit   = nullptr;
  TClonesArray* fCbmEvent = nullptr;
  InputTreeRECO->SetBranchAddress("TofHit", &fTofHit);
  InputTreeRECO->SetBranchAddress("TofTracks", &fTofTrack);
  InputTreeRECO->SetBranchAddress("StsHit", &fStsHit);
  InputTreeRECO->SetBranchAddress("CbmEvent", &fCbmEvent);

  auto nTS = InputTreeRECO->GetEntries();
  if (nofEvents > 0 && nTS >= nofEvents) { nTS = nofEvents; }
  std::cout << "Entries: " << InputTreeRECO->GetEntries() << std::endl;

  TFile* oldFileLin    = gFile;
  TDirectory* oldirLin = gDirectory;

  std::string s     = Form("%s/GraphFitsLinear.root", outdir);
  TFile* outFileLin = new TFile(s.c_str(), "RECREATE");


  for (int iTS = 0; iTS < nTS; iTS++) {
    fHM->H1("fhNofTimeslices")->Fill(1);
    InputTreeRECO->GetEntry(iTS);
    std::cout << "Timeslice No. " << iTS << std::endl;
    Int_t nTParts = fTofHit->GetEntriesFast();
    Int_t nEv     = fCbmEvent->GetEntriesFast();

    std::cout << "Events in TS: " << nEv << std::endl;
    fHM->H1("fhEventsPerTS")->Fill(iTS, nEv);

    for (auto iEv = 0; iEv < nEv; iEv++) {
      CbmEvent* ev        = static_cast<CbmEvent*>(fCbmEvent->At(iEv));
      auto eventStartTime = ev->GetStartTime();

      // Hit-Multiplicty Cuts:
      if (ev->GetNofData(ECbmDataType::kTofHit) < 2) continue;

      // Fill Hit Multiplicity:
      fHM->H1("fhMultiplicityStsHits")->Fill(ev->GetNofData(ECbmDataType::kStsHit));
      fHM->H1("fhMultiplicityTofHits")->Fill(ev->GetNofData(ECbmDataType::kTofHit));


      //Correct the TofHits
      std::vector<size_t> tofIndxStation[2];
      for (int j = 0; j < ev->GetNofData(ECbmDataType::kTofHit); j++) {
        auto iTofHit      = ev->GetIndex(ECbmDataType::kTofHit, j);
        CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHit->At(iTofHit));
        auto TofStationNo = (tofHit->GetAddress() >> 4) & 0xf;

        if (TofStationNo == 0) { tofIndxStation[0].push_back(iTofHit); }
        else {
          tofIndxStation[1].push_back(iTofHit);
        }
      }  // TofHits

      for (int j = 0; j < tofIndxStation[1].size(); j++) {
        CbmTofHit* tofHit_stat1 = static_cast<CbmTofHit*>(fTofHit->At(tofIndxStation[1].at(j)));

        ClosestCbmHit closestTofHits;  // Station 0
        for (int k = 0; k < tofIndxStation[0].size(); k++) {
          CbmTofHit* tofHit_stat0 = static_cast<CbmTofHit*>(fTofHit->At(tofIndxStation[0].at(k)));
          TVector3 vExtr          = extrpolate(tofHit_stat1, &vertexPoint, tofHit_stat0->GetZ());

          auto Xextr = vExtr.X();  //tofHit->GetX()* stsHit->GetZ() / tofHit->GetZ();
          auto Yextr = vExtr.Y();  //tofHit->GetY()* stsHit->GetZ() / tofHit->GetZ();

          auto Xdiff = Xextr - tofHit_stat0->GetX();
          auto Ydiff = Yextr - tofHit_stat0->GetY();

          Double_t distTof = TMath::Sqrt(Xdiff * Xdiff + Ydiff * Ydiff);

          if (distTof < closestTofHits.Dist && 0.5 > distTof) {
            closestTofHits.Existing = true;
            closestTofHits.X        = Xdiff;
            closestTofHits.Y        = Ydiff;
            closestTofHits.Z        = tofHit_stat0->GetZ();
            closestTofHits.Dist     = distTof;
            closestTofHits.Indx     = tofIndxStation[0].at(k);
            closestTofHits.X_pos    = tofHit_stat0->GetX();
            closestTofHits.Y_pos    = tofHit_stat0->GetY();
          }
        }  // TofHits

        ClosestCbmHit closestStsHitArray[3];
        ClosestCbmHit closestStsHitStations[2];

        auto nofStsHitsEv = ev->GetNofData(ECbmDataType::kStsHit);
        for (int jSts = 0; jSts < nofStsHitsEv; jSts++) {
          auto iStsHit      = ev->GetIndex(ECbmDataType::kStsHit, jSts);
          CbmStsHit* stsHit = static_cast<CbmStsHit*>(fStsHit->At(iStsHit));

          TVector3 vExtr = extrpolate(tofHit_stat1, &vertexPoint, stsHit->GetZ());

          auto Xextr = vExtr.X();  //tofHit->GetX()* stsHit->GetZ() / tofHit->GetZ();
          auto Yextr = vExtr.Y();  //tofHit->GetY()* stsHit->GetZ() / tofHit->GetZ();

          auto stsUnit    = 0;
          auto stsStation = (stsHit->GetAddress() >> 4) & 0xF;
          auto stsUnitAdd = (stsHit->GetAddress() >> 8) & 0x4;

          // if (stsHit->GetZ() < 28.0+8.5){
          if (stsStation == 0 && stsUnitAdd == 0) { stsUnit = 0; }
          else if (stsStation == 0 && stsUnitAdd != 0) {
            stsUnit = 1;
          }
          else {
            stsStation = 1;
            stsUnit    = 2;
          }

          auto Xdiff = Xextr - stsHit->GetX();
          auto Ydiff = Yextr - stsHit->GetY();

          Double_t distSts = TMath::Sqrt(Xdiff * Xdiff + Ydiff * Ydiff);

          if (distSts < closestStsHitArray[stsUnit].Dist) {
            closestStsHitArray[stsUnit].Existing = true;
            closestStsHitArray[stsUnit].X        = Xdiff;
            closestStsHitArray[stsUnit].Y        = Ydiff;
            closestStsHitArray[stsUnit].Z        = stsHit->GetZ();
            closestStsHitArray[stsUnit].Dist     = distSts;
            closestStsHitArray[stsUnit].Indx     = iStsHit;
            closestStsHitArray[stsUnit].X_pos    = stsHit->GetX();
            closestStsHitArray[stsUnit].Y_pos    = stsHit->GetY();
          }

          if (distSts < closestStsHitStations[stsStation].Dist) {
            closestStsHitStations[stsStation].Existing = true;
            closestStsHitStations[stsStation].X        = Xdiff;
            closestStsHitStations[stsStation].Y        = Ydiff;
            closestStsHitStations[stsStation].Z        = stsHit->GetZ();
            closestStsHitStations[stsStation].Dist     = distSts;
            closestStsHitStations[stsStation].Indx     = iStsHit;
            closestStsHitStations[stsStation].X_pos    = stsHit->GetX();
            closestStsHitStations[stsStation].Y_pos    = stsHit->GetY();
          }
        }  //STS Loop

        std::vector<Double_t> ZArray, XArray, YArray, IndexArray;
        ZArray.push_back(tofHit_stat1->GetZ());
        XArray.push_back(tofHit_stat1->GetX());
        YArray.push_back(tofHit_stat1->GetY());
        IndexArray.push_back(tofIndxStation[1].at(j));
        if (closestTofHits.Existing) {
          ZArray.push_back(closestTofHits.Z);
          XArray.push_back(closestTofHits.X_pos);
          YArray.push_back(closestTofHits.Y_pos);
          IndexArray.push_back(closestTofHits.Indx);
        }
        if (closestStsHitStations[0].Existing) {
          ZArray.push_back(closestStsHitStations[0].Z);
          XArray.push_back(closestStsHitStations[0].X_pos);
          YArray.push_back(closestStsHitStations[0].Y_pos);
          IndexArray.push_back(closestStsHitStations[0].Indx);
        }
        if (closestStsHitStations[1].Existing) {
          ZArray.push_back(closestStsHitStations[1].Z);
          XArray.push_back(closestStsHitStations[1].X_pos);
          YArray.push_back(closestStsHitStations[1].Y_pos);
          IndexArray.push_back(closestStsHitStations[1].Indx);
        }

        if (ZArray.size() > 3) {
          //std::cout << "Event # " << iEv << std::endl;
          //std::cout << "Xpos: "<< closestTofHits.X_pos<< std::endl;
          TGraph* grX  = new TGraph(ZArray.size(), &ZArray[0], &XArray[0]);
          TF1* linFitX = new TF1("f1", "[0]+[1]*x", 0, 0.1);
          grX->Fit(linFitX, "Q");

          TGraph* grY  = new TGraph(ZArray.size(), &ZArray[0], &YArray[0]);
          TF1* linFitY = new TF1("f2", "[0]+[1]*x", 0, 0.1);
          grY->Fit(linFitY, "Q");

          fHM->H1("fhLinearFitChi2X")->Fill(linFitX->GetChisquare());
          fHM->H1("fhLinearFitChi2Y")->Fill(linFitY->GetChisquare());

          if (linFitX->GetChisquare() < 0.5 && linFitY->GetChisquare() < 0.5) {
            fHM->H2("fhLinearFitXY")->Fill(linFitX->GetParameter(0), linFitY->GetParameter(0));
            fHM->H1("fhLinearFitZX")->Fill(linFitX->GetParameter(0));
            fHM->H1("fhLinearFitZY")->Fill(linFitY->GetParameter(0));

            fHM->H2("fhLinearFitXY_.25")
              ->Fill(linFitX->GetParameter(0) + (.25) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (.25) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_.50")
              ->Fill(linFitX->GetParameter(0) + (.50) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (.50) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_.75")
              ->Fill(linFitX->GetParameter(0) + (.75) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (.75) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_1")
              ->Fill(linFitX->GetParameter(0) + (1) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (1) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_2")
              ->Fill(linFitX->GetParameter(0) + (2) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (2) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_4")
              ->Fill(linFitX->GetParameter(0) + (4) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (4) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_6")
              ->Fill(linFitX->GetParameter(0) + (6) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (6) * linFitY->GetParameter(1));

            fHM->H2("fhLinearFitXY_-.5")
              ->Fill(linFitX->GetParameter(0) + (-.5) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (-.5) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_-1")
              ->Fill(linFitX->GetParameter(0) + (-1) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (-1) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_-2")
              ->Fill(linFitX->GetParameter(0) + (-2) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (-2) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_-4")
              ->Fill(linFitX->GetParameter(0) + (-4) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (-4) * linFitY->GetParameter(1));
            fHM->H2("fhLinearFitXY_-6")
              ->Fill(linFitX->GetParameter(0) + (-6) * linFitX->GetParameter(1),
                     linFitY->GetParameter(0) + (-6) * linFitY->GetParameter(1));

            // Good Fit!: Extract informations of this kind of tracks
            const CbmTofHit* tofHit_sel_0 = static_cast<CbmTofHit*>(fTofHit->At(IndexArray.at(0)));
            const CbmTofHit* tofHit_sel_1 = static_cast<CbmTofHit*>(fTofHit->At(IndexArray.at(1)));

            auto selectedTrackTime  = tofHit_sel_0->GetTime() - tofHit_sel_1->GetTime();
            auto selectedTrackXdiff = tofHit_sel_0->GetX() - tofHit_sel_1->GetX();
            auto selectedTrackYdiff = tofHit_sel_0->GetY() - tofHit_sel_1->GetY();
            auto selectedTrackZdiff = tofHit_sel_0->GetZ() - tofHit_sel_1->GetZ();
            auto Rdiff = TMath::Sqrt(selectedTrackXdiff * selectedTrackXdiff + selectedTrackYdiff * selectedTrackYdiff
                                     + selectedTrackZdiff * selectedTrackZdiff);
            auto selectedTrackBeta = (1e7 / (selectedTrackTime / Rdiff)) / TMath::C();

            fHM->H1("fhSimpleTrackTofTime")->Fill(selectedTrackTime);
            fHM->H1("fhSimpleTrackTofDist")->Fill(Rdiff);
            fHM->H1("fhSimpleTrackTofBeta")->Fill(selectedTrackBeta);
          }

          //linFitX->Print();
          if (outFileLin->IsOpen()) {
            if (iEv < 1000 && linFitX->GetChisquare() < .5) grX->Write();
          }
        }
      }  // TofHits


      std::vector<size_t> stsIndxStation[2];
      for (int jSts = 0; jSts < ev->GetNofData(ECbmDataType::kStsHit); jSts++) {

        auto iStsHit      = ev->GetIndex(ECbmDataType::kStsHit, jSts);
        CbmStsHit* stsHit = static_cast<CbmStsHit*>(fStsHit->At(iStsHit));
        auto x            = stsHit->GetX();
        auto y            = stsHit->GetY();
        auto z            = stsHit->GetZ();

        fHM->H1("fhStsHitsZ")->Fill(z);

        auto stsStation = (stsHit->GetAddress() >> 4) & 0xF;
        auto stsUnit    = (stsHit->GetAddress() >> 8) & 0x4;

        if (stsStation == 0 && stsUnit == 0) {
          fHM->H2("fhStsUnit0Hits")->Fill(stsHit->GetX(), stsHit->GetY());
          fHM->H2("fhStsStation0Hits")->Fill(stsHit->GetX(), stsHit->GetY());
          stsIndxStation[0].push_back(iStsHit);
        }
        else if (stsStation == 0 && stsUnit != 0) {
          fHM->H2("fhStsUnit1Hits")->Fill(stsHit->GetX(), stsHit->GetY());
          fHM->H2("fhStsStation0Hits")->Fill(stsHit->GetX(), stsHit->GetY());
          stsIndxStation[0].push_back(iStsHit);
        }
        else {
          fHM->H2("fhStsUnit2Hits")->Fill(stsHit->GetX(), stsHit->GetY());
          stsIndxStation[1].push_back(iStsHit);
        }
      }  // StsHits


      auto nofTofHitsEv = ev->GetNofData(ECbmDataType::kTofHit);
      for (int j = 0; j < nofTofHitsEv; j++) {
        auto iTofHit      = ev->GetIndex(ECbmDataType::kTofHit, j);
        CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHit->At(iTofHit));

        fHM->H1("fhTofHitsTime")->Fill(tofHit->GetTime() - eventStartTime);

        auto TofStationNo = (tofHit->GetAddress() >> 4) & 0xf;
        auto modPos       = (tofHit->GetAddress() >> 15) & 0x1;  // 0  is closer to vertex
        auto modPosY      = (tofHit->GetAddress() >> 16) & 0xf;  // ModPos 0: 0,1,2    ModPos 1: 0,1

        fHM->H1("fhTofHitsZ")->Fill(tofHit->GetZ());
        fHM->H2("fhTofHitsXY")->Fill(tofHit->GetX(), tofHit->GetY());


        if (TofStationNo == 0 && modPos == 0 && modPosY == 1) {
          fHM->H2("fhTofHitsXY_001")->Fill(tofHit->GetX(), tofHit->GetY());
          fHM->H1("fhTofHitsTime_001")->Fill(tofHit->GetTime() - eventStartTime);
          if (tofHitYMin > tofHit->GetY()) tofHitYMin = tofHit->GetY();
          if (tofHitYMax < tofHit->GetY()) tofHitYMax = tofHit->GetY();
        }

        if (TofStationNo == 0) { fHM->H1("fhTofHitsTimeLayer0")->Fill(tofHit->GetTime() - eventStartTime); }

        if (TofStationNo == 1) { fHM->H1("fhTofHitsTimeLayer1")->Fill(tofHit->GetTime() - eventStartTime); }

        ClosestCbmHit closestStsHitArray[3];
        ClosestCbmHit closestStsHitStations[2];

        auto nofStsHitsEv = ev->GetNofData(ECbmDataType::kStsHit);
        for (int jSts = 0; jSts < nofStsHitsEv; jSts++) {
          auto iStsHit      = ev->GetIndex(ECbmDataType::kStsHit, jSts);
          CbmStsHit* stsHit = static_cast<CbmStsHit*>(fStsHit->At(iStsHit));

          TVector3 vExtr = extrpolate(tofHit, &vertexPoint, stsHit->GetZ());

          auto Xextr = vExtr.X();  //tofHit->GetX()* stsHit->GetZ() / tofHit->GetZ();
          auto Yextr = vExtr.Y();  //tofHit->GetY()* stsHit->GetZ() / tofHit->GetZ();

          auto stsUnit = 0;

          auto stsStation = (stsHit->GetAddress() >> 4) & 0xF;
          auto stsUnitAdd = (stsHit->GetAddress() >> 8) & 0x4;

          if (stsStation == 0 && stsUnitAdd == 0) { stsUnit = 0; }
          else if (stsStation == 0 && stsUnitAdd != 0) {
            stsUnit = 1;
          }
          else {
            stsStation = 1;
            stsUnit    = 2;
          }

          auto Xdiff = Xextr - stsHit->GetX();
          auto Ydiff = Yextr - stsHit->GetY();

          Double_t distSts = TMath::Sqrt(Xdiff * Xdiff + Ydiff * Ydiff);

          if (distSts < closestStsHitArray[stsUnit].Dist) {
            closestStsHitArray[stsUnit].Existing = true;
            closestStsHitArray[stsUnit].X        = Xdiff;
            closestStsHitArray[stsUnit].Y        = Ydiff;
            closestStsHitArray[stsUnit].Z        = stsHit->GetZ();
            closestStsHitArray[stsUnit].Dist     = distSts;
            closestStsHitArray[stsUnit].Indx     = iStsHit;
            closestStsHitArray[stsUnit].X_pos    = stsHit->GetX();
            closestStsHitArray[stsUnit].Y_pos    = stsHit->GetY();
          }

          if (distSts < closestStsHitStations[stsStation].Dist) {
            closestStsHitStations[stsStation].Existing = true;
            closestStsHitStations[stsStation].X        = Xdiff;
            closestStsHitStations[stsStation].Y        = Ydiff;
            closestStsHitStations[stsStation].Z        = stsHit->GetZ();
            closestStsHitStations[stsStation].Dist     = distSts;
            closestStsHitStations[stsStation].Indx     = iStsHit;
            closestStsHitStations[stsStation].X_pos    = stsHit->GetX();
            closestStsHitStations[stsStation].Y_pos    = stsHit->GetY();
          }


          fHM->H2(Form("fhTofSts%uXYdiff", stsUnit))->Fill(Xdiff, Ydiff);

          if (TMath::Abs(Xdiff) < 0.2 && TMath::Abs(Ydiff) < 0.2) {
            vExtr = extrpolate(tofHit, &vertexPoint, 8.5);
            fHM->H2(Form("fhTofSts%uXY_8.5cm", stsUnit))->Fill(vExtr.X(), vExtr.Y());
          }
        }  //STS Loop

        for (auto o = 0; o < 3; ++o) {
          if (closestStsHitArray[o].Existing == true && (closestStsHitStations[0].Existing == true)
              && (closestStsHitStations[1].Existing == true)) {
            TVector3 vExtr_tof = extrpolate(tofHit, &vertexPoint, closestStsHitArray[o].Z);
            fHM->H2(Form("fhTof%u%u%uSts%uextr", TofStationNo, modPos, modPosY, o))->Fill(vExtr_tof.X(), vExtr_tof.Y());
            fHM->H2(Form("fhTof%u%u%uSts%uXYdiff", TofStationNo, modPos, modPosY, o))
              ->Fill(closestStsHitArray[o].X, closestStsHitArray[o].Y);
            if (!(modPos == 0 && modPosY == 0))
              fHM->H2(Form("fhTofSts%uXYdiff_close", o))->Fill(closestStsHitArray[o].X, closestStsHitArray[o].Y);
          }
        }

        if ((closestStsHitStations[0].Existing == true) && (closestStsHitStations[1].Existing == true)) {
          fHM->H2(Form("fhTof%u%u%uDiffStation01", TofStationNo, modPos, modPosY))
            ->Fill(closestStsHitStations[0].Dist, closestStsHitStations[1].Dist);
          fHM->H2(Form("fhTof%u%u%uXDiffStation01", TofStationNo, modPos, modPosY))
            ->Fill(closestStsHitStations[0].X, closestStsHitStations[1].X);
          fHM->H2(Form("fhTof%u%u%uYDiffStation01", TofStationNo, modPos, modPosY))
            ->Fill(closestStsHitStations[0].Y, closestStsHitStations[1].Y);

          if (TofStationNo == 0 && modPos == 1 && modPosY == 0) {
            Double_t Y_new = closestStsHitStations[1].Y - closestStsHitStations[0].Y;
            fHM->H2("fhTof010YDiffStation01_pro")->Fill(closestStsHitStations[0].Y, Y_new);
            Double_t Y_ratio = closestStsHitStations[1].Y / closestStsHitStations[0].Y;
            fHM->H2("fhTof010YDiffStation01_ratio")->Fill(closestStsHitStations[0].Y, Y_ratio);
          }
        }

      }  // TofHit Loop


      // Tof Tracks
      ClosestCbmHit closestTrackStsHitStations[2];

      auto nofTofTracksEv = ev->GetNofData(ECbmDataType::kTofTrack);
      fHM->H1("fhTofTrackMulti")->Fill(nofTofTracksEv);

      Int_t Multipl_2hit_Tracks = 0;

      for (int j = 0; j < nofTofTracksEv; j++) {
        auto iTofTrack           = ev->GetIndex(ECbmDataType::kTofTrack, j);
        CbmTofTracklet* tofTrack = static_cast<CbmTofTracklet*>(fTofTrack->At(iTofTrack));

        if (tofTrack->GetNofHits() == 2) Multipl_2hit_Tracks++;

        //Sts Loop
        auto nofStsHitsEv = ev->GetNofData(ECbmDataType::kStsHit);
        for (int jSts = 0; jSts < nofStsHitsEv; jSts++) {
          auto iStsHit      = ev->GetIndex(ECbmDataType::kStsHit, jSts);
          CbmStsHit* stsHit = static_cast<CbmStsHit*>(fStsHit->At(iStsHit));

          auto stsUnit    = 0;
          auto stsStation = (stsHit->GetAddress() >> 4) & 0xF;
          auto stsUnitAdd = (stsHit->GetAddress() >> 8) & 0x4;
          if (stsStation == 0 && stsUnitAdd == 0) { stsUnit = 0; }
          else if (stsStation == 0 && stsUnitAdd != 0) {
            stsUnit = 1;
          }
          else {
            stsStation = 1;
            stsUnit    = 2;
          }

          auto Xdiff       = tofTrack->GetFitX(stsHit->GetZ()) - stsHit->GetX();
          auto Ydiff       = tofTrack->GetFitY(stsHit->GetZ()) - stsHit->GetY();
          Double_t distSts = TMath::Sqrt(Xdiff * Xdiff + Ydiff * Ydiff);

          if (distSts < closestTrackStsHitStations[stsStation].Dist) {
            closestTrackStsHitStations[stsStation].Existing = true;
            closestTrackStsHitStations[stsStation].X        = Xdiff;
            closestTrackStsHitStations[stsStation].Y        = Ydiff;
            closestTrackStsHitStations[stsStation].Z        = stsHit->GetZ();
            closestTrackStsHitStations[stsStation].Dist     = distSts;
            closestTrackStsHitStations[stsStation].Indx     = iStsHit;
            closestTrackStsHitStations[stsStation].X_pos    = stsHit->GetX();
            closestTrackStsHitStations[stsStation].Y_pos    = stsHit->GetY();
          }
        }  // End Sts Loop

        if ((closestTrackStsHitStations[0].Existing == true) && (closestTrackStsHitStations[1].Existing == true)
            && tofTrack->GetNofHits() == 2) {
          //std::cout<<"Station0: " <<  closestStsHitStations[0].Dist << "  Station1: "<< closestStsHitStations[1].Dist << std::endl;
          fHM->H2("fhTofTrackDiffStation01")
            ->Fill(closestTrackStsHitStations[0].Dist, closestTrackStsHitStations[1].Dist);
          fHM->H2("fhTofTrackXDiffStation01")->Fill(closestTrackStsHitStations[0].X, closestTrackStsHitStations[1].X);
          fHM->H2("fhTofTrackYDiffStation01")->Fill(closestTrackStsHitStations[0].Y, closestTrackStsHitStations[1].Y);

          fHM->H2("fhTofTrackXYDiffStation0")->Fill(closestTrackStsHitStations[0].X, closestTrackStsHitStations[0].Y);
          fHM->H2("fhTofTrackXYDiffStation1")->Fill(closestTrackStsHitStations[1].X, closestTrackStsHitStations[1].Y);


          fHM->H2("fhTofTrackXY520cm_2Hits")->Fill(tofTrack->GetFitX(520.0), tofTrack->GetFitY(520.0));
          fHM->H2("fhTofTrackXYVertex_2Hits")->Fill(tofTrack->GetFitX(0.0), tofTrack->GetFitY(0.0));

          // Time Offset Calculation
          for (auto cntStation = 0; cntStation < 2; ++cntStation) {
            for (auto kTrackHits = 0; kTrackHits < tofTrack->GetNofHits(); ++kTrackHits) {
              auto trackHitInd  = tofTrack->GetHitIndex(kTrackHits);
              CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHit->At(trackHitInd));

              if (tofHit == nullptr) continue;

              auto diffX = tofHit->GetX() - closestTrackStsHitStations[cntStation].X_pos;
              auto diffY = tofHit->GetY() - closestTrackStsHitStations[cntStation].Y_pos;
              auto diffZ = tofHit->GetZ() - closestTrackStsHitStations[cntStation].Z;

              auto distance         = TMath::Sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
              Double_t TimeOfFlight = distance * tofTrack->GetTt();  // ns
              if (tofTrack->GetTt() < 0) continue;
              fHM->H1("fhTimeOfFlight")->Fill(TimeOfFlight);

              auto TofStationNo = (tofHit->GetAddress() >> 4) & 0xf;
              fHM->H1(Form("fhTimeOfFlight_%u", TofStationNo))->Fill(TimeOfFlight);

              fHM->H1(Form("fhTimeOfFlight_%u_%u", TofStationNo, cntStation))->Fill(TimeOfFlight);

              fHM->H2(Form("fhTimeOfFlight_%u_diffZ", TofStationNo))->Fill(diffZ, TimeOfFlight);

              // tof hit - TimeOfFlight - stshittime
              CbmStsHit* stsHit = static_cast<CbmStsHit*>(fStsHit->At(closestTrackStsHitStations[cntStation].Indx));
              if (nullptr == stsHit) continue;

              auto timeoffset = tofHit->GetTime() - TimeOfFlight - stsHit->GetTime();
              fHM->H1(Form("fhTimeOffset"))->Fill(timeoffset);
            }
          }
        }

        fHM->H1("fhTofHitsPerTrack")->Fill(tofTrack->GetNofHits());
        //std::cout<< tofTrack->GetT0() << "  " << tofTrack->GetTt() << "  " << tofTrack->GetT0()-tofTrack->GetTt() <<std::endl;
        fHM->H1("fhTofTracksT0")->Fill(tofTrack->GetT0() - eventStartTime);
        fHM->H1("fhTofTracksTt")->Fill(tofTrack->GetTt());             // invers velocity
        fHM->H1("fhTofTracksVelocity")->Fill(1. / tofTrack->GetTt());  // invers velocity
        fHM->H1("fhTofTracksTdiff")->Fill(tofTrack->GetTime() - eventStartTime);
        fHM->H1("fhTofTracksTdiff2")->Fill(tofTrack->GetTime() - tofTrack->GetT0());
        fHM->H1("fhTofTracksTime")->Fill(tofTrack->GetTime());
        fHM->H1("fhTofTracksDistance")->Fill(tofTrack->GetDistance());
        fHM->H2("fhTofTrackXY_basic")->Fill(tofTrack->GetTrackX(), tofTrack->GetTrackY());
        Double_t inVel = 1e7 / (tofTrack->GetTt());  // in m/s
        auto beta      = inVel / TMath::C();
        fHM->H1("fhTofBeta")->Fill(beta);

        fHM->H2("fhTofTrackXYVertex")->Fill(tofTrack->GetFitX(0.0), tofTrack->GetFitY(0.0));
        fHM->H2("fhTofTrackXY520cm")->Fill(tofTrack->GetFitX(520.0), tofTrack->GetFitY(520.0));
        fHM->H2("fhTofTrackXY")->Fill(tofTrack->GetFitX(260.), tofTrack->GetFitY(260.));

        auto nofTrackHits = tofTrack->GetNofHits();
        for (auto kTrackHits = 0; kTrackHits < nofTrackHits; ++kTrackHits) {
          auto trackHitInd  = tofTrack->GetHitIndex(kTrackHits);
          CbmTofHit* tofHit = static_cast<CbmTofHit*>(fTofHit->At(trackHitInd));

          fHM->H1("fhTofTrackHitsTimeDiff")->Fill(tofHit->GetTime() - tofTrack->GetT0());
          fHM->H1("fhTofTrackHitsTime")->Fill(tofHit->GetTime() - eventStartTime);
        }

        Double_t closestStsR = 100000.0;
        Int_t closestStsIndx = -1;

        for (int jSts = 0; jSts < nofStsHitsEv; jSts++) {
          auto iStsHit      = ev->GetIndex(ECbmDataType::kStsHit, jSts);
          CbmStsHit* stsHit = static_cast<CbmStsHit*>(fStsHit->At(iStsHit));

          auto Xdiff = tofTrack->GetFitX(stsHit->GetZ()) - stsHit->GetX();
          auto Ydiff = tofTrack->GetFitY(stsHit->GetZ()) - stsHit->GetY();

          fHM->H2("fhTofTrackStsXYdiff")->Fill(Xdiff, Ydiff);

          auto rStsTofTrack = TMath::Sqrt(Xdiff * Xdiff + Ydiff * Ydiff);
          if (rStsTofTrack < closestStsR) {
            closestStsR    = rStsTofTrack;
            closestStsIndx = iStsHit;
          }
        }  // Sts Hit Loop

        if (closestStsIndx > -1 && closestStsR < CutCloseDistStsTrack) {
          CbmStsHit* closeStsHit = static_cast<CbmStsHit*>(fStsHit->At(closestStsIndx));
          fHM->H1("fhTofTrackStsR")->Fill(closestStsR);

          // 1.) calculate length from T0 to Sts Hit
          auto stsT0Length = std::sqrt(std::pow((closeStsHit->GetX() - tofTrack->GetTrackX()), 2)
                                       + std::pow((closeStsHit->GetY() - tofTrack->GetTrackY()), 2)
                                       + std::pow((closeStsHit->GetZ() - 0.0), 2));
          fHM->H1("fhTofTracksLength")->Fill(stsT0Length);

          // 2.) calculate time in Tof system with length and velocity
          auto stsTrackTime = stsT0Length * tofTrack->GetTt();  // time (of flight) in ns
          fHM->H1("fhStsTrackTime")->Fill(stsTrackTime);

          // 3.) compare to Sts hit Time
          auto diffStsTimes = (tofTrack->GetT0() + stsTrackTime) - closeStsHit->GetTime();
          fHM->H1("fhStsHitStsTrackTimeDiff")->Fill(diffStsTimes);
        }
      }  // Track Loop

      fHM->H1("fhMultiplicityTof2HitsTracks")->Fill(Multipl_2hit_Tracks);
    }  // Event Loop
  }    // TS loop

  outFileLin->Close();
  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile = oldFileLin;
  gDirectory->cd(oldirLin->GetPath());

  fRECO->Close();

  std::cout << "tofHitYMin: " << tofHitYMin << "  tofHitYMax: " << tofHitYMax << std::endl;

  {
    ofstream XY_differences;
    XY_differences.open(Form("%s/XY_differences.dat", outdir));
    // Alignments:
    for (auto l = 0; l < 3; ++l) {
      for (auto i = 0; i < 2; ++i) {
        for (auto m = 0; m < 5; ++m) {
          auto j = 0;
          auto k = 0;

          if (m == 0) {
            j = 0;
            k = 0;
          }
          if (m == 1) {
            j = 1;
            k = 0;
          }
          if (m == 2) {
            j = 0;
            k = 1;
          }
          if (m == 3) {
            j = 1;
            k = 1;
          }
          if (m == 4) {
            j = 0;
            k = 2;
          }

          auto x = fHM->H2(Form("fhTof%u%u%uSts%uXYdiff", i, j, k, l))->GetMean(1);
          auto y = fHM->H2(Form("fhTof%u%u%uSts%uXYdiff", i, j, k, l))->GetMean(2);
          std::cout << " Tof:" << i << j << k << " StS:" << l << "  X:" << x << "  Y:" << y << std::endl;
          XY_differences << " Tof:" << i << j << k << " StS:" << l << "  X:" << x << "  Y:" << y << std::endl;
        }
      }
      std::cout << std::endl;
      XY_differences << std::endl;
    }
    XY_differences.close();
  }


  std::vector<TH1*> fitHistsX, fitHistsY;
  std::vector<std::string> XFitstring, YFitstring;

  for (auto p = 0; p < 3; ++p) {
    ProjectXY(Form("fhTofSts%uXYdiff_close", p), fHM, &fitHistsX, &XFitstring, &fitHistsY, &YFitstring);
  }

  {
    std::vector<std::string> strTofStsHistX, strTofStsHistY;
    ProjectXY("fhTof010Sts0XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof001Sts0XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof011Sts0XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof002Sts0XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);

    ProjectXY("fhTof110Sts0XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof101Sts0XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof111Sts0XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof102Sts0XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);


    ProjectXY("fhTof010Sts1XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof001Sts1XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof011Sts1XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof002Sts1XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);

    ProjectXY("fhTof110Sts1XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof101Sts1XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof111Sts1XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof102Sts1XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);


    ProjectXY("fhTof010Sts2XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof001Sts2XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof011Sts2XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof002Sts2XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);

    ProjectXY("fhTof110Sts2XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof101Sts2XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof111Sts2XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);
    ProjectXY("fhTof102Sts2XYdiff", fHM, &fitHistsX, &strTofStsHistX, &fitHistsY, &strTofStsHistY);

    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "fhTof*10Sts0XYdiff : " << strTofStsHistX.at(0) << "  |  " << strTofStsHistY.at(0) << " || "
              << strTofStsHistX.at(4) << "  |  " << strTofStsHistY.at(4) << std::endl;
    std::cout << "fhTof*01Sts0XYdiff : " << strTofStsHistX.at(1) << "  |  " << strTofStsHistY.at(1) << " || "
              << strTofStsHistX.at(5) << "  |  " << strTofStsHistY.at(5) << std::endl;
    std::cout << "fhTof*11Sts0XYdiff : " << strTofStsHistX.at(2) << "  |  " << strTofStsHistY.at(2) << " || "
              << strTofStsHistX.at(6) << "  |  " << strTofStsHistY.at(6) << std::endl;
    std::cout << "fhTof*02Sts0XYdiff : " << strTofStsHistX.at(3) << "  |  " << strTofStsHistY.at(3) << " || "
              << strTofStsHistX.at(7) << "  |  " << strTofStsHistY.at(7) << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;

    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "fhTof*10Sts1XYdiff : " << strTofStsHistX.at(8) << "  |  " << strTofStsHistY.at(8) << " || "
              << strTofStsHistX.at(12) << "  |  " << strTofStsHistY.at(12) << std::endl;
    std::cout << "fhTof*01Sts1XYdiff : " << strTofStsHistX.at(9) << "  |  " << strTofStsHistY.at(9) << " || "
              << strTofStsHistX.at(13) << "  |  " << strTofStsHistY.at(13) << std::endl;
    std::cout << "fhTof*11Sts1XYdiff : " << strTofStsHistX.at(10) << "  |  " << strTofStsHistY.at(10) << " || "
              << strTofStsHistX.at(14) << "  |  " << strTofStsHistY.at(14) << std::endl;
    std::cout << "fhTof*02Sts1XYdiff : " << strTofStsHistX.at(11) << "  |  " << strTofStsHistY.at(11) << " || "
              << strTofStsHistX.at(15) << "  |  " << strTofStsHistY.at(15) << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;

    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "fhTof*10Sts2XYdiff : " << strTofStsHistX.at(16) << "  |  " << strTofStsHistY.at(16) << " || "
              << strTofStsHistX.at(20) << "  |  " << strTofStsHistY.at(20) << std::endl;
    std::cout << "fhTof*01Sts2XYdiff : " << strTofStsHistX.at(17) << "  |  " << strTofStsHistY.at(17) << " || "
              << strTofStsHistX.at(21) << "  |  " << strTofStsHistY.at(21) << std::endl;
    std::cout << "fhTof*11Sts2XYdiff : " << strTofStsHistX.at(18) << "  |  " << strTofStsHistY.at(18) << " || "
              << strTofStsHistX.at(22) << "  |  " << strTofStsHistY.at(22) << std::endl;
    std::cout << "fhTof*02Sts2XYdiff : " << strTofStsHistX.at(19) << "  |  " << strTofStsHistY.at(19) << " || "
              << strTofStsHistX.at(23) << "  |  " << strTofStsHistY.at(23) << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;
  }
  drawHists(fHM);

  std::cout << "TimeOffset Fit: " << FitTimeOffset(fHM->H1("fhTimeOffset")) << std::endl;

  if (true) {
    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile    = gFile;
    TDirectory* oldir = gDirectory;

    std::string s  = Form("%s/RecoInspectAlignHists.root", outdir);
    TFile* outFile = new TFile(s.c_str(), "RECREATE");
    if (outFile->IsOpen()) {
      fHM->WriteToFile();
      for (auto xhists = 0; xhists < fitHistsX.size(); xhists++)
        fitHistsX.at(xhists)->Write();
      for (auto yhists = 0; yhists < fitHistsY.size(); yhists++)
        fitHistsY.at(yhists)->Write();
      std::cout << "Written to Root-file \"" << s << "\"  ...";
      outFile->Close();
      std::cout << "Done!" << std::endl;
    }
    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile = oldFile;
    gDirectory->cd(oldir->GetPath());
  }

  std::cout << " --------------------------------------------" << std::endl;
  std::cout << "0 | " << XFitstring.at(0) << "  |  " << YFitstring.at(0) << std::endl;
  std::cout << "1 | " << XFitstring.at(1) << "  |  " << YFitstring.at(1) << std::endl;
  std::cout << "2 | " << XFitstring.at(2) << "  |  " << YFitstring.at(2) << std::endl;
}


void initHists(CbmHistManager* fHM, Int_t nofEvents)
{
  fHM->Create1<TH1D>("fhNofTimeslices", "fhNofTimeslices;Entries", 1, 0.5, 1.5);

  fHM->Create2<TH2D>("fhTofHitsXY", "fhTofHitsXY;X [cm];Y [cm];Entries", 200, -100., 100., 200, -100., 100.);
  fHM->Create1<TH1D>("fhTofTrackMulti", "fhTofTrackMulti;Tracks/Event;Entries", 20, -0.5, 19.5);
  fHM->Create1<TH1D>("fhTofHitsZ", "fhTofHitsZ;Z[cm];Entries", 100, 250, 300);
  fHM->Create1<TH1D>("fhTofHitsPerTrack", "fhTofHitsPerTrack;Hits/Track;Entries", 11, -0.5, 10.5);
  fHM->Create1<TH1D>("fhTofTracksT0", "fhTofTracksT0;T0 time [ns];Entries", 100, -50, 50);
  fHM->Create1<TH1D>("fhTofTracksTime", "fhTofTracksTime;Track time [ns];Entries", 30000, -50, 3.0e8);
  fHM->Create1<TH1D>("fhTofTracksTt", "fhTofTracksTt;inv. velocity [ns/cm];Entries", 100, -50, 50);
  fHM->Create1<TH1D>("fhTofTracksVelocity", "fhTofTracksVelocity;velocity [cm/ns];Entries", 100, -50, 50);
  fHM->Create1<TH1D>("fhTofTracksTdiff", "fhTofTracksTdiff;Time [ns];Entries", 100, -50, 50);
  fHM->Create1<TH1D>("fhTofTracksTdiff2", "fhTofTracksTdiff2;Time [ns];Entries", 100, -50, 50);
  fHM->Create1<TH1D>("fhTofTracksLength", "fhTofTracksLength;length [cm];Entries", 400, -50, 350);

  fHM->Create1<TH1D>("fhTofTrackHitsTimeDiff", "fhTofTrackHitsTimeDiff;time [ns];Entries", 100, -50.0, 50.0);
  fHM->Create1<TH1D>("fhTofHitsTime", "fhTofHitsTime;time [ns];Entries", 100, -50.0, 50.0);
  fHM->Create1<TH1D>("fhTofHitsTimeLayer0", "fhTofHitsTimeLayer0;time [ns];Entries", 100, -50.0, 50.0);
  fHM->Create1<TH1D>("fhTofHitsTimeLayer1", "fhTofHitsTimeLayer1;time [ns];Entries", 100, -50.0, 50.0);
  fHM->Create1<TH1D>("fhTofHitsTime_001", "fhTofHitsTime_001;time [ns];Entries", 100, -50.0, 50.0);

  fHM->Create1<TH1D>("fhTofTrackHitsTime", "fhTofTrackHitsTime;time [ns];Entries", 100, -50.0, 50.0);

  fHM->Create1<TH1D>("fhTofBeta", "fhTofBeta;#beta;Entries", 2200, -1.1, 1.1);
  fHM->Create1<TH1D>("fhTofTracksDistance", "fhTofTracksDistance;distance [cm];Entries", 100, -5, 5);

  fHM->Create2<TH2D>("fhTofTrackXYVertex", "fhTofTrackXYVertex;X [cm];Y [cm];Entries", 200, -100, 100, 200, -100, 100);
  fHM->Create2<TH2D>("fhTofTrackXYVertex_2Hits", "fhTofTrackXYVertex_2Hits;X [cm];Y [cm];Entries", 200, -100, 100, 200,
                     -100, 100);
  fHM->Create2<TH2D>("fhTofTrackXY520cm", "fhTofTrackXY520cm;X [cm];Y [cm];Entries", 200, -100, 100, 200, -100, 100);
  fHM->Create2<TH2D>("fhTofTrackXY520cm_2Hits", "fhTofTrackXY520cm_2Hits;X [cm];Y [cm];Entries", 200, -100, 100, 200,
                     -100, 100);
  fHM->Create2<TH2D>("fhTofTrackXY", "fhTofTrackXY;X [cm];Y [cm];Entries", 200, -100, 100, 200, -100, 100);
  fHM->Create2<TH2D>("fhTofTrackXY_basic", "fhTofTrackXY_basic;X [cm];Y [cm];Entries", 200, -100, 100, 200, -100, 100);


  fHM->Create1<TH1D>("fhTofTrackStsR", "fhTofTrackStsR;distance [cm];Entries", 600, 0.0, 60.0);
  fHM->Create1<TH1D>("fhStsHitStsTrackTimeDiff", "fhStsHitStsTrackTimeDiff;time diff [ns];Entries", 1200, -60.0, 60.0);
  fHM->Create1<TH1D>("fhStsTrackTime", "fhStsTrackTime;time from track [ns];Entries", 1200, -60.0, 60.0);
  fHM->Create1<TH1D>("fhStsHitsZ", "fhStsHitsZ; Z [cm];Entries", 400, 20.0, 60.0);

  fHM->Create2<TH2D>("fhTofTrackStsXYdiff", "fhTofTrackStsXYdiff;X_tof-X_Sts [cm];Y_tof-Y_Sts [cm];Entries", 600, -2.95,
                     2.95, 600, -2.95, 2.95);

  for (auto i = 0; i < 2; ++i) {
    for (auto j = 0; j < 2; ++j) {
      for (auto k = 0; k < 3; ++k) {
        if (k == 2 && j == 1) continue;
        fHM->Create2<TH2D>(Form("fhTof%u%u%uDiffStation01", i, j, k),
                           Form("fhTof%u%u%uDiffStation01;Station 0 [cm];Station1 [cm];Entries", i, j, k), 200, -0.5,
                           19.5, 200, -0.5, 19.5);
        fHM->Create2<TH2D>(Form("fhTof%u%u%uXDiffStation01", i, j, k),
                           Form("fhTof%u%u%uXDiffStation01;Station 0 [cm];Station1 [cm];Entries", i, j, k), 110, -5.5,
                           5.5, 110, -5.5, 5.5);
        fHM->Create2<TH2D>(Form("fhTof%u%u%uYDiffStation01", i, j, k),
                           Form("fhTof%u%u%uYDiffStation01;Station 0 [cm];Station1 [cm];Entries", i, j, k), 110, -5.5,
                           5.5, 110, -5.5, 5.5);
        for (auto l = 0; l < 3; ++l) {
          fHM->Create2<TH2D>(Form("fhTof%u%u%uSts%uXYdiff", i, j, k, l),
                             Form("fhTof%u%u%uSts%uXYdiff;X_tof-X_Sts [cm];Y_tof-Y_Sts [cm];Entries", i, j, k, l), 300,
                             -2.95, 2.95, 300, -2.95, 2.95);
          fHM->Create2<TH2D>(Form("fhTof%u%u%uSts%uextr", i, j, k, l),
                             Form("fhTof%u%u%uSts%uextr;X_tof [cm];Y_tof [cm];Entries", i, j, k, l), 600, -29.5, 29.5,
                             600, -29.5, 29.5);
        }
      }
    }
  }


  fHM->Create2<TH2D>("fhTof010YDiffStation01_pro", "fhTof010YDiffStation01_pro;Station 0 [cm];Station1 [cm];Entries",
                     110, -5.5, 5.5, 110, -5.5, 5.5);
  fHM->Create2<TH2D>("fhTof010YDiffStation01_ratio",
                     "fhTof010YDiffStation01_ratio;Station 0 [cm];Station1 [cm];Entries", 110, -5.5, 5.5, 110, -5.5,
                     5.5);

  fHM->Create2<TH2D>("fhTofTrackDiffStation01", "fhTofTrackDiffStation01;Station 0 [cm];Station1 [cm];Entries", 400,
                     -0.5, 39.5, 400, -0.5, 39.5);
  fHM->Create2<TH2D>("fhTofTrackXDiffStation01", "fhTofTrackXDiffStation01;Station 0 [cm];Station1 [cm];Entries", 790,
                     -39.5, 39.5, 790, -39.5, 39.5);
  fHM->Create2<TH2D>("fhTofTrackYDiffStation01", "fhTofTrackYDiffStation01;Station 0 [cm];Station1 [cm];Entries", 790,
                     -39.5, 39.5, 790, -39.5, 39.5);
  fHM->Create2<TH2D>("fhTofTrackXYDiffStation0", "fhTofTrackXYDiffStation0;Xdiff [cm];Ydiff [cm];Entries", 790, -79.5,
                     79.5, 790, -79.5, 79.5);
  fHM->Create2<TH2D>("fhTofTrackXYDiffStation1", "fhTofTrackXYDiffStation1;Xdiff [cm];Ydiff [cm];Entries", 790, -79.5,
                     79.5, 790, -79.5, 79.5);


  fHM->Create2<TH2D>("fhTofSts0XYdiff", "fhTofSts0XYdiff;X_tof-X_Sts [cm];Y_tof-Y_Sts [cm];Entries", 600, -2.95, 2.95,
                     600, -2.95, 2.95);
  fHM->Create2<TH2D>("fhTofSts1XYdiff", "fhTofSts1XYdiff;X_tof-X_Sts [cm];Y_tof-Y_Sts [cm];Entries", 600, -2.95, 2.95,
                     600, -2.95, 2.95);
  fHM->Create2<TH2D>("fhTofSts2XYdiff", "fhTofSts2XYdiff;X_tof-X_Sts [cm];Y_tof-Y_Sts [cm];Entries", 600, -2.95, 2.95,
                     600, -2.95, 2.95);

  fHM->Create2<TH2D>("fhTofSts0XYdiff_close", "fhTofSts0XYdiff_close;X_tof-X_Sts [cm];Y_tof-Y_Sts [cm];Entries", 600,
                     -2.95, 2.95, 600, -2.95, 2.95);
  fHM->Create2<TH2D>("fhTofSts1XYdiff_close", "fhTofSts1XYdiff_close;X_tof-X_Sts [cm];Y_tof-Y_Sts [cm];Entries", 600,
                     -2.95, 2.95, 600, -2.95, 2.95);
  fHM->Create2<TH2D>("fhTofSts2XYdiff_close", "fhTofSts2XYdiff_close;X_tof-X_Sts [cm];Y_tof-Y_Sts [cm];Entries", 600,
                     -2.95, 2.95, 600, -2.95, 2.95);

  fHM->Create2<TH2D>("fhTofSts0XY_8.5cm", "fhTofSts0XY_8.5cm;X [cm];Y [cm];Entries", 100, -1.0, 1.0, 100, -2.5, 2.5);
  fHM->Create2<TH2D>("fhTofSts1XY_8.5cm", "fhTofSts1XY_8.5cm;X [cm];Y [cm];Entries", 100, -1.0, 1.0, 100, -2.5, 2.5);
  fHM->Create2<TH2D>("fhTofSts2XY_8.5cm", "fhTofSts2XY_8.5cm;X [cm];Y [cm];Entries", 100, -1.0, 1.0, 100, -2.5, 2.5);


  fHM->Create2<TH2D>("fhStsUnit0Hits", "fhStsUnit0Hits;X_Sts [cm];Y_Sts [cm];Entries", 600, -30, 30, 600, -30, 30);
  fHM->Create2<TH2D>("fhStsUnit1Hits", "fhStsUnit1Hits;X_Sts [cm];Y_Sts [cm];Entries", 600, -30, 30, 600, -30, 30);
  fHM->Create2<TH2D>("fhStsUnit2Hits", "fhStsUnit2Hits;X_Sts [cm];Y_Sts [cm];Entries", 600, -30, 30, 600, -30, 30);

  fHM->Create2<TH2D>("fhStsStation0Hits", "fhStsStation0Hits;X_Sts [cm];Y_Sts [cm];Entries", 600, -30, 30, 600, -30,
                     30);

  fHM->Create2<TH2D>("fhTofHitsXY_001", "fhTofHitsXY_001;X_tof [cm];Y_tof [cm];Entries", 80, -20, 20, 80, -20, 20);

  fHM->Create1<TH1D>("fhLinearFitZX", "fhLinearFitZX;X@0 [cm];Entries", 400, -2, 2);
  fHM->Create1<TH1D>("fhLinearFitZY", "fhLinearFitZY;Y@0 [cm];Entries", 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY", "fhLinearFitXY;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create1<TH1D>("fhLinearFitChi2X", "fhLinearFitChi2X;Chi2 X-Fit;Entries", 200, 0, 2);
  fHM->Create1<TH1D>("fhLinearFitChi2Y", "fhLinearFitChi2Y;Chi2 Y-Fit;Entries", 200, 0, 2);

  fHM->Create2<TH2D>("fhLinearFitXY_.25", "fhLinearFitXY_.25;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_.50", "fhLinearFitXY_.50;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_.75", "fhLinearFitXY_.75;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_1", "fhLinearFitXY_2;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_2", "fhLinearFitXY_2;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_4", "fhLinearFitXY_4;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_6", "fhLinearFitXY_6;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);

  fHM->Create2<TH2D>("fhLinearFitXY_-.5", "fhLinearFitXY_-.5;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_-1", "fhLinearFitXY_-1;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_-2", "fhLinearFitXY_-2;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_-4", "fhLinearFitXY_-4;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);
  fHM->Create2<TH2D>("fhLinearFitXY_-6", "fhLinearFitXY_-6;X@0 [cm];Y@0 [cm];Entries", 400, -2, 2, 400, -2, 2);

  fHM->Create1<TH1D>("fhTimeOffset", "fhTimeOffset;time [ns];Entries", 800, -40.0, 40.0);
  fHM->Create1<TH1D>("fhTimeOfFlight", "fhTimeOfFlight;time [ns];Entries", 800, -40.0, 40.0);
  fHM->Create1<TH1D>("fhTimeOfFlight_0", "fhTimeOfFlight_0;time [ns];Entries", 800, -40.0, 40.0);
  fHM->Create1<TH1D>("fhTimeOfFlight_1", "fhTimeOfFlight_1;time [ns];Entries", 800, -40.0, 40.0);

  fHM->Create1<TH1D>("fhTimeOfFlight_0_0", "fhTimeOfFlight_0_0;time [ns];Entries", 800, -40.0, 40.0);
  fHM->Create1<TH1D>("fhTimeOfFlight_0_1", "fhTimeOfFlight_0_1;time [ns];Entries", 800, -40.0, 40.0);
  fHM->Create1<TH1D>("fhTimeOfFlight_1_0", "fhTimeOfFlight_1_0;time [ns];Entries", 800, -40.0, 40.0);
  fHM->Create1<TH1D>("fhTimeOfFlight_1_1", "fhTimeOfFlight_1_1;time [ns];Entries", 800, -40.0, 40.0);

  fHM->Create2<TH2D>("fhTimeOfFlight_0_diffZ", "fhTimeOfFlight_0_diffZ;time [ns];Entries", 300, 0., 300., 800, -40.0,
                     40.0);
  fHM->Create2<TH2D>("fhTimeOfFlight_1_diffZ", "fhTimeOfFlight_1_diffZ;time [ns];Entries", 300, 0., 300., 800, -40.0,
                     40.0);

  fHM->Create1<TH1D>("fhSimpleTrackTofTime", "fhSimpleTrackTofTime;time [ns];Entries", 400, -2.0, 2.0);
  fHM->Create1<TH1D>("fhSimpleTrackTofDist", "fhSimpleTrackTofDist;distance [cm];Entries", 150, 10.0, 25.0);
  fHM->Create1<TH1D>("fhSimpleTrackTofBeta", "fhSimpleTrackTofBeta;#beta;Entries", 600, -3.0, 3.0);

  fHM->Create1<TH1D>("fhMultiplicityStsHits", "fhMultiplicityStsHits;Hits/Event;Entries", 20, 0.0, 20.0);
  fHM->Create1<TH1D>("fhMultiplicityTofHits", "fhMultiplicityTofHits;Hits/Event;Entries", 20, 0.0, 20.0);
  fHM->Create1<TH1D>("fhMultiplicityTof2HitsTracks", "fhMultiplicityTof2HitsTracks;2-hit Tracks/Event;Entries", 20, 0.0,
                     20.0);

  auto EventsPerTSBinning = (nofEvents < 200) ? nofEvents : 200;
  fHM->Create1<TH1D>("fhEventsPerTS", "fhEventsPerTS;TS;#Events", EventsPerTSBinning, 0, EventsPerTSBinning);
}

void drawHists(CbmHistManager* fHM)
{

  {
    TCanvas* c = fHM->CreateCanvas("EventsPerTS", "EventsPerTS", 2000, 2000);
    fHM->H1("fhEventsPerTS")->Draw("HIST");

    c->SaveAs(Form("%s/EventsPerTS.png", outdir));
  }

  {
    TCanvas* c = fHM->CreateCanvas("StsUnit0_TofXYDiff", "StsUnit0_TofXYDiff", 2000, 2000);
    c->Divide(4, 5);
    c->cd(1);
    fHM->H2("fhTof000Sts0XYdiff")->Draw("colz");
    c->cd(3);
    fHM->H2("fhTof100Sts0XYdiff")->Draw("colz");

    c->cd(6);
    fHM->H2("fhTof010Sts0XYdiff")->Draw("colz");
    c->cd(8);
    fHM->H2("fhTof110Sts0XYdiff")->Draw("colz");

    c->cd(9);
    fHM->H2("fhTof001Sts0XYdiff")->Draw("colz");
    c->cd(11);
    fHM->H2("fhTof101Sts0XYdiff")->Draw("colz");

    c->cd(14);
    fHM->H2("fhTof011Sts0XYdiff")->Draw("colz");
    c->cd(16);
    fHM->H2("fhTof111Sts0XYdiff")->Draw("colz");

    c->cd(17);
    fHM->H2("fhTof002Sts0XYdiff")->Draw("colz");

    c->cd(19);
    fHM->H2("fhTof102Sts0XYdiff")->Draw("colz");

    c->SaveAs(Form("%s/StsUnit0_TofXYDiff.png", outdir));
  }

  {
    TCanvas* c = fHM->CreateCanvas("StsUnit1_TofXYDiff", "StsUnit1_TofXYDiff", 2000, 2000);
    c->Divide(4, 5);
    c->cd(1);
    fHM->H2("fhTof000Sts1XYdiff")->Draw("colz");
    c->cd(3);
    fHM->H2("fhTof100Sts1XYdiff")->Draw("colz");

    c->cd(6);
    fHM->H2("fhTof010Sts1XYdiff")->Draw("colz");
    c->cd(8);
    fHM->H2("fhTof110Sts1XYdiff")->Draw("colz");

    c->cd(9);
    fHM->H2("fhTof001Sts1XYdiff")->Draw("colz");
    c->cd(11);
    fHM->H2("fhTof101Sts1XYdiff")->Draw("colz");

    c->cd(14);
    fHM->H2("fhTof011Sts1XYdiff")->Draw("colz");
    c->cd(16);
    fHM->H2("fhTof111Sts1XYdiff")->Draw("colz");

    c->cd(17);
    fHM->H2("fhTof002Sts1XYdiff")->Draw("colz");

    c->cd(19);
    fHM->H2("fhTof102Sts1XYdiff")->Draw("colz");

    c->SaveAs(Form("%s/StsUnit1_TofXYDiff.png", outdir));
  }

  {
    TCanvas* c = fHM->CreateCanvas("StsUnit2_TofXYDiff", "StsUnit2_TofXYDiff", 2000, 2000);
    c->Divide(4, 5);
    c->cd(1);
    fHM->H2("fhTof000Sts2XYdiff")->Draw("colz");
    c->cd(3);
    fHM->H2("fhTof100Sts2XYdiff")->Draw("colz");

    c->cd(6);
    fHM->H2("fhTof010Sts2XYdiff")->Draw("colz");
    c->cd(8);
    fHM->H2("fhTof110Sts2XYdiff")->Draw("colz");

    c->cd(9);
    fHM->H2("fhTof001Sts2XYdiff")->Draw("colz");
    c->cd(11);
    fHM->H2("fhTof101Sts2XYdiff")->Draw("colz");

    c->cd(14);
    fHM->H2("fhTof011Sts2XYdiff")->Draw("colz");
    c->cd(16);
    fHM->H2("fhTof111Sts2XYdiff")->Draw("colz");

    c->cd(17);
    fHM->H2("fhTof002Sts2XYdiff")->Draw("colz");

    c->cd(19);
    fHM->H2("fhTof102Sts2XYdiff")->Draw("colz");

    c->SaveAs(Form("%s/StsUnit2_TofXYDiff.png", outdir));
  }

  {
    TCanvas* c = fHM->CreateCanvas("StsUnits_TofXYDiff", "StsUnits_TofXYDiff", 3000, 1000);
    c->Divide(3, 1);
    c->cd(1);
    fHM->H2("fhTofSts0XYdiff")->Draw("colz");
    c->cd(2);
    fHM->H2("fhTofSts1XYdiff")->Draw("colz");
    c->cd(3);
    fHM->H2("fhTofSts2XYdiff")->Draw("colz");

    c->SaveAs(Form("%s/StsUnits_TofXYDiff.png", outdir));
  }

  {
    TCanvas* c = fHM->CreateCanvas("Multiplicities", "Multiplicities", 3000, 1000);
    c->Divide(3, 1);
    c->cd(1);
    fHM->H1("fhMultiplicityStsHits")->Draw("Hist");
    c->cd(2);
    fHM->H1("fhMultiplicityTofHits")->Draw("Hist");
    c->cd(3);
    fHM->H1("fhMultiplicityTof2HitsTracks")->Draw("Hist");

    c->SaveAs(Form("%s/Multiplicities_Sts_Tof.png", outdir));
  }

  {
    TCanvas* c = fHM->CreateCanvas("StsUnits_TofXYDiff_Close", "StsUnits_TofXYDiff_Close", 3000, 1000);
    c->Divide(3, 1);
    c->cd(1);
    fHM->H2("fhTofSts0XYdiff_close")->Draw("colz");
    c->cd(2);
    fHM->H2("fhTofSts1XYdiff_close")->Draw("colz");
    c->cd(3);
    fHM->H2("fhTofSts2XYdiff_close")->Draw("colz");

    c->SaveAs(Form("%s/StsUnits_TofXYDiff_Close.png", outdir));
  }

  {
    TCanvas* c = fHM->CreateCanvas("StsZ", "StsZ", 1000, 1000);
    fHM->H1("fhStsHitsZ")->Draw("HIST");

    c->SaveAs(Form("%s/StsHitsZ.png", outdir));
  }
}

TVector3 extrpolate(CbmTofHit* tofHit, TVector3* vertex, Double_t Z)
{
  TVector3 extVec(0, 0, 0);

  Double_t factor = (Z - vertex->Z()) / (tofHit->GetZ() - vertex->Z());
  Double_t x      = vertex->X() + factor * (tofHit->GetX() - vertex->X());
  Double_t y      = vertex->Y() + factor * (tofHit->GetY() - vertex->Y());
  extVec.SetXYZ(x, y, Z);

  return extVec;
}

void ProjectXY(std::string name, CbmHistManager* fHM, std::vector<TH1*>* HistoX, std::vector<std::string>* stringX,
               std::vector<TH1*>* HistoY, std::vector<std::string>* stringY)
{
  ProjectAxis(name, fHM, 'X', HistoX, stringX);
  ProjectAxis(name, fHM, 'Y', HistoY, stringY);
}

void ProjectAxis(std::string name, CbmHistManager* fHM, char axis, std::vector<TH1*>* Histo,
                 std::vector<std::string>* string)
{
  std::cout << "start Fitting procedure" << std::endl;

  TH1* hist = nullptr;
  if (axis == 'x' || axis == 'X') hist = fHM->H2(name)->ProjectionX();
  if (axis == 'y' || axis == 'Y') hist = fHM->H2(name)->ProjectionY();

  // Find Peak position:
  Int_t binmax   = hist->GetMaximumBin();
  Double_t max_x = hist->GetXaxis()->GetBinCenter(binmax);

  TF1* g1s = new TF1("gS", "gaus(0)", max_x - 0.3, max_x + 0.3);
  TF1* g1b = nullptr;

  if (hist->GetMean() > max_x) { g1b = new TF1("gB", "gaus(0)", max_x + 1, 3); }
  else {
    g1b = new TF1("gB", "gaus(0)", -3, max_x - 1);
  }
  TF1* total1 = new TF1("total", "gaus(0)+gaus(3)", -3, 3);

  Double_t par[6];
  Double_t partotal[6];
  hist->Fit(g1s, "QR");
  hist->Fit(g1b, "QR+");

  g1s->GetParameters(&par[0]);
  g1b->GetParameters(&par[3]);
  total1->SetParameters(par);

  hist->Fit(total1, "QR+");
  total1->GetParameters(&partotal[0]);

  // std::cout << " Mean1:   " << partotal[1] << "  DEV: " << partotal[2] << " Mean2: "  << partotal[4]
  //           << "  DEV: " << partotal[5] << std::endl;
  // std::cout << " MeanSig: " << par[1]       << "  DEV: " << par[2]       << " MeanBG: " << par[4]
  //           << "  DEV: " << par[5]       << std::endl;
  if (std::abs(partotal[2]) < std::abs(partotal[5])) {
    string->emplace_back(Form("%+.4f +- %.4f cm", partotal[1], partotal[2]));
  }
  else {
    string->emplace_back(Form("%+.4f +- %.4f cm", partotal[4], partotal[5]));
  }

  TCanvas* c = nullptr;
  if (axis == 'x' || axis == 'X')
    c = fHM->CreateCanvas(Form("%s_projX", name.c_str()), Form("%s_projX", name.c_str()), 2000, 2000);
  if (axis == 'y' || axis == 'Y')
    c = fHM->CreateCanvas(Form("%s_projY", name.c_str()), Form("%s_projY", name.c_str()), 2000, 2000);
  hist->Draw("HIST");
  total1->Draw("SAME");

  if (axis == 'x' || axis == 'X') c->SaveAs(Form("%s/%s_projX.png", outdir, name.c_str()));
  if (axis == 'y' || axis == 'Y') c->SaveAs(Form("%s/%s_projY.png", outdir, name.c_str()));

  Histo->push_back(hist);
}


std::string FitTimeOffset(TH1* hist)
{
  TF1* g1s = new TF1("gS", "gaus(0)", -20, -4);
  TF1* g1b = new TF1("gB", "gaus(0)", -40, -24);

  TF1* total1 = new TF1("total", "gaus(0)+gaus(3)", -40, 40);

  Double_t par[6];
  Double_t partotal[6];
  hist->Fit(g1s, "QR");
  hist->Fit(g1b, "QR+");

  g1s->GetParameters(&par[0]);
  g1b->GetParameters(&par[3]);
  total1->SetParameters(par);

  hist->Fit(total1, "QR+");
  total1->GetParameters(&partotal[0]);

  //std::string FitResult = Form("%+.4f +- %.4f ns", partotal[1], partotal[2]);

  TCanvas* c = new TCanvas("TimeOffset_Fit", "TimeOffset_Fit", 2000, 2000);
  hist->Draw("HIST");
  total1->Draw("SAME");
  c->SaveAs(Form("%s/TimeOffset_Fit.png", outdir));

  return Form("%+.4f +- %.4f ns", partotal[1], partotal[2]);
}
