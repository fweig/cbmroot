#include "CbmRichRecoTbQa.h"

#include "CbmDigiManager.h"
#include "CbmDrawHist.h"
#include "CbmGlobalTrack.h"
#include "CbmHistManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichDigi.h"
#include "CbmRichDraw.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichUtil.h"
#include "CbmStsPoint.h"
#include "CbmTrackMatchNew.h"
#include "CbmUtils.h"
#include "elid/CbmLitGlobalElectronId.h"

#include "FairEventHeader.h"
#include "FairMCPoint.h"
#include "FairRunAna.h"
#include "FairRunSim.h"
#include "FairTrackParam.h"
#include <Logger.h>

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TH1.h"
#include "TH1D.h"
#include "TMCProcess.h"
#include "TMarker.h"
#include "TStyle.h"
#include <TFile.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

CbmRichRecoTbQa::CbmRichRecoTbQa()
  : FairTask("CbmRichRecoTbQa")
  , fHM(nullptr)
  , fTimeSliceNum(0.)
  , fNofLogEvents(150)
  , fOutputDir("")
  , fMCTracks(nullptr)
  , fRichPoints(nullptr)
  , fStsPoints(nullptr)
  , fDigiMan(nullptr)
  , fRichHits(nullptr)
  , fRichRings(nullptr)
  , fRichRingMatches(nullptr)
  , fEventList(nullptr)
  , fRecRings()
{
}


InitStatus CbmRichRecoTbQa::Init()
{
  cout << "CbmRichRecoTbQa::Init" << endl;
  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmRichRecoTbQa::Init", "RootManager not instantised!"); }

  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");
  if (mcManager == nullptr) LOG(fatal) << "CbmRichRecoTbQa::ReadAndCreateDataBranches() NULL MCDataManager.";

  fMCTracks   = mcManager->InitBranch("MCTrack");
  fRichPoints = mcManager->InitBranch("RichPoint");
  fStsPoints  = mcManager->InitBranch("StsPoint");

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) { Fatal("CbmRichRecoTbQa::Init", "No Rich hits!"); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) { Fatal("CbmRichRecoTbQa::Init", "No Rich rings!"); }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (nullptr == fRichRingMatches) { Fatal("CbmRichRecoTbQa::Init", "No RichRingMatch!"); }

  fEventList = (CbmMCEventList*) ioman->GetObject("MCEventList.");
  if (nullptr == fEventList) { Fatal("CbmRichRecoTbQa::Init", "No MCEventList!"); }

  InitHistograms();

  return kSUCCESS;
}

void CbmRichRecoTbQa::InitHistograms()
{
  fHM = new CbmHistManager();

  Int_t nBinsLog  = 20000;
  Double_t minLog = 0.;
  Double_t maxLog = 10000.;


  fHM->Create1<TH1D>("fhNofRichDigisPerTS", "fhNofRichDigisPerTS;RICH digis per time slice;Yield", 100, -1, -1.);
  fHM->Create1<TH1D>("fhNofRichHitsPerTS", "fhNofRichHitsPerTS;RICH hits per time slice;Yield", 100, -1., -1.);
  fHM->Create1<TH1D>("fhNofRichRingsPerTS", "fhNofRichRingsPerTS;RICH rings per time slice;Yield", 100, -1., -1.);

  fHM->Create1<TH1D>("fhRichDigiTimeLog", "fhRichDigiTimeLog;RICH digi time [ns];Yield", nBinsLog, minLog, maxLog);

  fHM->Create1<TH1D>("fhRichRingTimeLog", "fhRichRingTimeLog;RICH ring time [ns];Yield", nBinsLog, minLog, maxLog);

  fHM->Create1<TH1D>("fhRichPointTime", "fhRichPointTime;RICH MC point time [ns];Yield", 100, 0., 100.);
  fHM->Create1<TH1D>("fhRichPointTimeChPhoton", "fhRichPointTimeChPhoton;RICH MC point time [ns];Yield", 100, 0., 100.);
  fHM->Create1<TH1D>("fhRichPointTimeNotChPhoton", "fhRichPointTimeNotChPhoton;RICH MC point time [ns];Yield", 100, 0.,
                     100.);
  fHM->Create1<TH1D>("fhRichPointTimePrimEl", "fhRichPointTimePrimEl;RICH MC point time [ns];Yield", 100, 0., 100.);
  fHM->Create1<TH1D>("fhRichPointTimeSecEl", "fhRichPointTimeSecEl;RICH MC point time [ns];Yield", 100, 0., 100.);
  fHM->Create1<TH1D>("fhRichPointTimeOther", "fhRichPointTimeOther;RICH MC point time [ns];Yield", 100, 0., 100.);
  fHM->Create1<TH1D>("fhRichPointTimePion", "fhRichPointTimePion;RICH MC point time [ns];Yield", 100, 0., 100.);

  fHM->Create1<TH1D>("fhStsPointTime", "fhStsPointTime;STS MC point time [ns];Yield", 400, 0., 400.);

  // -1 for noise hits
  for (int iEv = -1; iEv < fNofLogEvents; iEv++) {
    string richDigiStr = "fhRichDigiTimeLog_" + to_string(iEv);
    fHM->Create1<TH1D>(richDigiStr, richDigiStr + ";Time [ns];Yield", nBinsLog, minLog, maxLog);

    string richRingStr = "fhRichRingTimeLog_" + to_string(iEv);
    fHM->Create1<TH1D>(richRingStr, richRingStr + ";Time [ns];Yield", nBinsLog, minLog, maxLog);

    string richStr = "fhRichPointTimeLog_" + to_string(iEv);
    fHM->Create1<TH1D>(richStr, richStr + ";Time [ns];Yield", nBinsLog, minLog, maxLog);

    string stsStr = "fhStsPointTimeLog_" + to_string(iEv);
    fHM->Create1<TH1D>(stsStr, stsStr + ";Time [ns];Yield", nBinsLog, minLog, maxLog);
  }

  fHM->Create1<TH1D>("fhTimeBetweenEvents", "fhTimeBetweenEvents;Time between events [ns];Yield", 100, 0., 500.);


  // efficiency histograms
  fHM->Create1<TH1D>("fhMomElAcc", "fhMomElAcc;P [GeV/c];Yield", 10, 0., 10.);
  fHM->Create1<TH1D>("fhMomElRec", "fhMomElRec;P [GeV/c];Yield", 10, 0., 10.);
  fHM->Create1<TH1D>("fhMomRefElAcc", "fhMomRefElAcc;P [GeV/c];Yield", 10, 0., 10.);
  fHM->Create1<TH1D>("fhMomRefElRec", "fhMomRefElRec;P [GeV/c];Yield", 10, 0., 10.);


  fHM->Create1<TH1D>("fhNofHitsElAcc", "fhNofHitsElAcc;Nof hits in ring;Yield", 20, -.5, 39.5);
  fHM->Create1<TH1D>("fhNofHitsElRec", "fhNofHitsElRec;Nof hits in ring;Yield", 20, -.5, 39.5);
  fHM->Create1<TH1D>("fhNofHitsRefElAcc", "fhNofHitsRefElAcc;Nof hits in ring;Yield", 20, -.5, 39.5);
  fHM->Create1<TH1D>("fhNofHitsRefElRec", "fhNofHitsRefElRec;Nof hits in ring;Yield", 20, -.5, 39.5);

  fHM->Create1<TH1D>("fhEventMultElAcc", "fhEventMultElAcc;Nof MC tracks;Yield", 10, 0., 2000);
  fHM->Create1<TH1D>("fhEventMultElRec", "fhEventMultElRec;Nof MC tracks;Yield", 10, 0, 2000);
}

void CbmRichRecoTbQa::Exec(Option_t* /*option*/)
{
  fTimeSliceNum++;
  int nofMcEvents = fEventList->GetNofEvents();

  cout << "CbmRichRecoTbQa, time slice:" << fTimeSliceNum << " nofMcEvents:" << nofMcEvents << endl;

  Process();

  RingRecoEfficiency();
}

void CbmRichRecoTbQa::Process()
{
  Int_t nofRichDigis = fDigiMan->GetNofDigis(ECbmModuleId::kRich);
  Int_t nofRichHits  = fRichHits->GetEntriesFast();
  Int_t nofRichRings = fRichRings->GetEntriesFast();
  fHM->H1("fhNofRichDigisPerTS")->Fill(nofRichDigis);
  fHM->H1("fhNofRichHitsPerTS")->Fill(nofRichHits);
  fHM->H1("fhNofRichRingsPerTS")->Fill(nofRichRings);

  LOG(debug) << "nofRichDigis:" << nofRichDigis;
  for (Int_t iDigi = 0; iDigi < nofRichDigis; iDigi++) {
    const CbmRichDigi* digi   = fDigiMan->Get<CbmRichDigi>(iDigi);
    const CbmMatch* digiMatch = fDigiMan->GetMatch(ECbmModuleId::kRich, iDigi);
    fHM->H1("fhRichDigiTimeLog")->Fill(digi->GetTime());

    Int_t eventNum = digiMatch->GetMatchedLink().GetEntry();
    Int_t index    = digiMatch->GetMatchedLink().GetIndex();

    if (eventNum < 0 || index < 0) { fHM->H1("fhRichDigiTimeLog_-1")->Fill(digi->GetTime()); }
    else {
      if (eventNum < fNofLogEvents) {
        string hName = "fhRichDigiTimeLog_" + to_string(eventNum);
        fHM->H1(hName)->Fill(digi->GetTime());
      }
    }
  }

  for (Int_t iR = 0; iR < nofRichRings; iR++) {
    CbmRichRing* ring           = static_cast<CbmRichRing*>(fRichRings->At(iR));
    CbmTrackMatchNew* ringMatch = static_cast<CbmTrackMatchNew*>(fRichRingMatches->At(iR));
    if (ring == nullptr || ringMatch == nullptr) continue;

    fHM->H1("fhRichRingTimeLog")->Fill(ring->GetTime());

    Int_t eventNum = ringMatch->GetMatchedLink().GetEntry();
    Int_t index    = ringMatch->GetMatchedLink().GetIndex();

    if (eventNum < 0 || index < 0) { fHM->H1("fhRichRingTimeLog_-1")->Fill(ring->GetTime()); }
    else {
      if (eventNum < fNofLogEvents) {
        string hName = "fhRichRingTimeLog_" + to_string(eventNum);
        fHM->H1(hName)->Fill(ring->GetTime());
      }
    }
  }
}

void CbmRichRecoTbQa::ProcessMc()
{
  int fileId = 0;

  //    int nMCEvents = fEventList->GetNofEvents();
  for (int iEv = 0; fEventList->GetEventTime(iEv, fileId) > 0.; iEv++) {
    double dT = (iEv == 0) ? fEventList->GetEventTime(iEv, fileId)
                           : fEventList->GetEventTime(iEv, fileId) - fEventList->GetEventTime(iEv - 1, fileId);
    fHM->H1("fhTimeBetweenEvents")->Fill(dT);
  }

  for (Int_t iEv = 0; fRichPoints->Size(fileId, iEv) >= 0; iEv++) {
    Int_t nofRichPoints = fRichPoints->Size(fileId, iEv);
    for (Int_t iP = 0; iP < nofRichPoints; iP++) {
      CbmRichPoint* point = (CbmRichPoint*) fRichPoints->Get(fileId, iEv, iP);
      fHM->H1("fhRichPointTime")->Fill(point->GetTime());
      if (IsCherenkovPhoton(point, fileId, iEv)) { fHM->H1("fhRichPointTimeChPhoton")->Fill(point->GetTime()); }
      else {
        fHM->H1("fhRichPointTimeNotChPhoton")->Fill(point->GetTime());
      }

      if (IsCherenkovPhotonFromPrimaryElectron(point, fileId, iEv)) {
        fHM->H1("fhRichPointTimePrimEl")->Fill(point->GetTime());
      }
      else if (IsCherenkovPhotonFromPion(point, fileId, iEv)) {
        fHM->H1("fhRichPointTimePion")->Fill(point->GetTime());
      }
      if (IsCherenkovPhotonFromSecondaryElectron(point, fileId, iEv)) {
        fHM->H1("fhRichPointTimeSecEl")->Fill(point->GetTime());
      }
      else {
        fHM->H1("fhRichPointTimeOther")->Fill(point->GetTime());
      }

      if (iEv < fNofLogEvents) {
        string richStr   = "fhRichPointTimeLog_" + to_string(iEv);
        double eventTime = fEventList->GetEventTime(iEv, fileId);
        fHM->H1(richStr)->Fill(eventTime + point->GetTime());
      }
    }
  }


  for (Int_t iEv = 0; fStsPoints->Size(fileId, iEv) >= 0; iEv++) {
    Int_t nofStsPoints = fStsPoints->Size(fileId, iEv);
    for (Int_t j = 0; j < nofStsPoints; j++) {
      const CbmStsPoint* point = (CbmStsPoint*) fStsPoints->Get(fileId, iEv, j);
      fHM->H1("fhStsPointTime")->Fill(point->GetTime());
      if (iEv < fNofLogEvents) {
        string stsStr    = "fhStsPointTimeLog_" + to_string(iEv);
        double eventTime = fEventList->GetEventTime(iEv, fileId);
        fHM->H1(stsStr)->Fill(eventTime + point->GetTime());
      }
    }
  }
}

Int_t CbmRichRecoTbQa::GetNofPrimaryMcTracks(Int_t iEv)
{
  Int_t fileId      = 0;
  Int_t counter     = 0;
  Int_t nofMcTracks = fMCTracks->Size(fileId, iEv);
  for (Int_t j = 0; j < nofMcTracks; j++) {
    const CbmMCTrack* track = static_cast<CbmMCTrack*>(fMCTracks->Get(fileId, iEv, j));
    if (track->GetGeantProcessId() == kPPrimary) counter++;
  }
  return counter;
}

void CbmRichRecoTbQa::RingRecoEfficiency()
{
  map<pair<Int_t, Int_t>, Int_t> nofHitsInRing;
  Int_t nofRichHits = fRichHits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofRichHits; iHit++) {
    const CbmRichHit* hit = static_cast<const CbmRichHit*>(fRichHits->At(iHit));
    if (nullptr == hit) continue;
    vector<pair<Int_t, Int_t>> motherIds =
      CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(fDigiMan, hit, fRichPoints, fMCTracks, -1);
    for (UInt_t i = 0; i < motherIds.size(); i++) {
      nofHitsInRing[motherIds[i]]++;
    }
  }

  Int_t fileId = 0;
  for (Int_t iEv = 0; fMCTracks->Size(fileId, iEv) >= 0; iEv++) {
    Int_t nofMcTracks     = fMCTracks->Size(fileId, iEv);
    Int_t nofPrimMcTracks = GetNofPrimaryMcTracks(iEv);
    for (Int_t j = 0; j < nofMcTracks; j++) {
      const CbmMCTrack* track = static_cast<CbmMCTrack*>(fMCTracks->Get(fileId, iEv, j));
      pair<Int_t, Int_t> val  = std::make_pair(iEv, j);
      Int_t nofRichHitsInRing = nofHitsInRing[val];
      Double_t mom            = track->GetP();
      if (nofRichHitsInRing >= 7 && IsMcPrimaryElectron(track)) {
        fHM->H1("fhMomElAcc")->Fill(mom);
        fHM->H1("fhNofHitsElAcc")->Fill(nofRichHitsInRing);
        fHM->H1("fhEventMultElAcc")->Fill(nofPrimMcTracks);
        if (nofRichHitsInRing >= 15) {
          fHM->H1("fhMomRefElAcc")->Fill(mom);
          fHM->H1("fhNofHitsRefElAcc")->Fill(nofRichHitsInRing);
        }
      }
    }
  }

  Int_t nofRings = fRichRings->GetEntriesFast();
  for (Int_t iRing = 0; iRing < nofRings; iRing++) {
    const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(iRing));
    Bool_t isRichOk                       = richRingMatch->GetTrueOverAllHitsRatio() >= 0.6;

    const CbmMCTrack* track = static_cast<CbmMCTrack*>(
      fMCTracks->Get(fileId, richRingMatch->GetMatchedLink().GetEntry(), richRingMatch->GetMatchedLink().GetIndex()));
    Int_t nofPrimMcTracks = GetNofPrimaryMcTracks(richRingMatch->GetMatchedLink().GetEntry());
    pair<Int_t, Int_t> val =
      std::make_pair(richRingMatch->GetMatchedLink().GetEntry(), richRingMatch->GetMatchedLink().GetIndex());
    Int_t nofRichHitsInRing = nofHitsInRing[val];
    Double_t mom            = track->GetP();
    Bool_t isClone          = (std::find(fRecRings.begin(), fRecRings.end(), val) != fRecRings.end());
    if (nofRichHitsInRing >= 7 && isRichOk && IsMcPrimaryElectron(track) && !isClone) {
      fHM->H1("fhMomElRec")->Fill(mom);
      fHM->H1("fhNofHitsElRec")->Fill(nofRichHitsInRing);
      fHM->H1("fhEventMultElRec")->Fill(nofPrimMcTracks);
      fRecRings.push_back(val);
      if (nofRichHitsInRing >= 15) {
        fHM->H1("fhMomRefElRec")->Fill(mom);
        fHM->H1("fhNofHitsRefElRec")->Fill(nofRichHitsInRing);
      }
    }
  }
}


Bool_t CbmRichRecoTbQa::IsCherenkovPhoton(const CbmRichPoint* point, Int_t fileId, Int_t eventId)
{
  if (point == nullptr) return false;
  Int_t trackId = point->GetTrackID();
  if (trackId < 0) return false;
  CbmMCTrack* p = (CbmMCTrack*) fMCTracks->Get(fileId, eventId, trackId);
  return (p != nullptr && TMath::Abs(p->GetPdgCode()) == 50000050);
}

Bool_t CbmRichRecoTbQa::IsCherenkovPhotonFromPrimaryElectron(const CbmRichPoint* point, Int_t fileId, Int_t eventId)
{
  if (point == nullptr) return false;
  Int_t trackId = point->GetTrackID();
  if (trackId < 0) return false;
  CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->Get(fileId, eventId, trackId);
  if (mcTrack == nullptr || TMath::Abs(mcTrack->GetPdgCode()) != 50000050) return false;

  Int_t motherId = mcTrack->GetMotherId();
  if (motherId < 0) return false;
  CbmMCTrack* mcMotherTrack = (CbmMCTrack*) fMCTracks->Get(fileId, eventId, motherId);
  return IsMcPrimaryElectron(mcMotherTrack);
}

Bool_t CbmRichRecoTbQa::IsCherenkovPhotonFromSecondaryElectron(const CbmRichPoint* point, Int_t fileId, Int_t eventId)
{
  if (point == nullptr) return false;
  Int_t trackId = point->GetTrackID();
  if (trackId < 0) return false;
  CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->Get(fileId, eventId, trackId);
  if (mcTrack == nullptr || TMath::Abs(mcTrack->GetPdgCode()) != 50000050) return false;

  Int_t motherId = mcTrack->GetMotherId();
  if (motherId < 0) return false;
  CbmMCTrack* mcMotherTrack = (CbmMCTrack*) fMCTracks->Get(fileId, eventId, motherId);
  if (mcMotherTrack == nullptr) return false;
  int pdg = TMath::Abs(mcMotherTrack->GetPdgCode());
  if (mcMotherTrack->GetGeantProcessId() != kPPrimary && pdg == 11) return true;

  return false;
}

Bool_t CbmRichRecoTbQa::IsMcPrimaryElectron(const CbmMCTrack* mctrack)
{
  if (mctrack == nullptr) return false;
  int pdg = TMath::Abs(mctrack->GetPdgCode());
  if (mctrack->GetGeantProcessId() == kPPrimary && pdg == 11) return true;
  return false;
}

Bool_t CbmRichRecoTbQa::IsMcPion(const CbmMCTrack* mctrack)
{
  if (mctrack == nullptr) return false;
  int pdg = TMath::Abs(mctrack->GetPdgCode());
  if (pdg == 211) return true;
  return false;
}

Bool_t CbmRichRecoTbQa::IsCherenkovPhotonFromPion(const CbmRichPoint* point, Int_t fileId, Int_t eventId)
{
  if (point == nullptr) return false;
  Int_t trackId = point->GetTrackID();
  if (trackId < 0) return false;
  CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->Get(fileId, eventId, trackId);
  if (mcTrack == nullptr || TMath::Abs(mcTrack->GetPdgCode()) != 50000050) return false;

  Int_t motherId = mcTrack->GetMotherId();
  if (motherId < 0) return false;
  CbmMCTrack* mcMotherTrack = (CbmMCTrack*) fMCTracks->Get(fileId, eventId, motherId);
  return IsMcPion(mcMotherTrack);
}

void CbmRichRecoTbQa::DrawHist()
{
  SetDefaultDrawStyle();


  {
    fHM->CreateCanvas("rich_tb_fhTimeBetweenEvents", "rich_tb_fhTimeBetweenEvents", 800, 800);
    DrawH1(fHM->H1("fhTimeBetweenEvents"), kLinear, kLog);
    Double_t min = 0.;
    Double_t max = 10.;
    Double_t partIntegral =
      fHM->H1("fhTimeBetweenEvents")
        ->Integral(fHM->H1("fhTimeBetweenEvents")->FindBin(min), fHM->H1("fhTimeBetweenEvents")->FindBin(max));
    Double_t allIntegral = fHM->H1("fhTimeBetweenEvents")->Integral();
    Double_t ratio       = 100. * partIntegral / allIntegral;
    cout << ratio << "% of all time between events are in range (" << min << " ," << max << ") ns" << endl;
  }


  {
    fHM->CreateCanvas("rich_tb_fhRichPointTime", "rich_tb_fhRichPointTime", 800, 800);
    DrawH1(fHM->H1("fhRichPointTime"), kLinear, kLog);
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_tb_fhRichPointTimeSources", "rich_tb_fhRichPointTimeSources", 1600, 800);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1({fHM->H1("fhRichPointTimeChPhoton"), fHM->H1("fhRichPointTimeNotChPhoton")},
           {"Ch. Photons", "Not Ch. Photons"}, kLinear, kLog);

    c->cd(2);
    DrawH1({fHM->H1("fhRichPointTimeSecEl"), fHM->H1("fhRichPointTimePrimEl"), fHM->H1("fhRichPointTimePion"),
            fHM->H1("fhRichPointTimeOther")},
           {"e^{#pm}_{sec}", "e^{#pm}_{prim}", "#pi^{#pm}", "Other"}, kLinear, kLog);
  }


  {
    fHM->CreateCanvas("rich_tb_fhStsPointTime", "rich_tb_fhStsPointTime", 800, 800);
    DrawH1(fHM->H1("fhStsPointTime"), kLinear, kLog);
  }

  {
    fHM->CreateCanvas("rich_tb_fhRichPointTimeLog", "rich_tb_fhRichPointTimeLog", 1600, 600);
    DrawTimeLog("fhRichPointTimeLog", fNofLogEvents);
  }

  {
    fHM->CreateCanvas("rich_tb_fhStsPointTimeLog", "rich_tb_fhStsPointTimeLog", 1600, 600);
    DrawTimeLog("fhStsPointTimeLog", fNofLogEvents);
  }

  {
    fHM->CreateCanvas("rich_tb_reco_fhRichDigiTimeLogAll", "rich_tb_reco_fhRichDigiTimeLogAll", 1600, 600);
    DrawH1(fHM->H1("fhRichDigiTimeLog"), kLinear, kLog);
    gPad->SetLeftMargin(0.07);
    gPad->SetRightMargin(0.05);
  }

  {
    fHM->CreateCanvas("rich_tb_reco_fhRichDigiTimeLog", "rich_tb_reco_fhRichDigiTimeLog", 1600, 600);
    DrawTimeLog("fhRichDigiTimeLog", fNofLogEvents, true);
  }

  {
    fHM->CreateCanvas("rich_tb_reco_fhRichRingTimeLogAll", "rich_tb_reco_fhRichRingTimeLogAll", 1600, 600);
    DrawH1(fHM->H1("fhRichRingTimeLog"), kLinear, kLog);
    gPad->SetLeftMargin(0.07);
    gPad->SetRightMargin(0.05);
  }

  {
    fHM->CreateCanvas("rich_tb_reco_timelog_hits_rings", "rich_tb_reco_timelog_hits_rings", 1600, 600);
    DrawH1({fHM->H1("fhRichDigiTimeLog"), fHM->H1("fhRichRingTimeLog")}, {"Hits", "Rings"});
    gPad->SetLeftMargin(0.07);
    gPad->SetRightMargin(0.05);
  }

  {
    fHM->CreateCanvas("rich_tb_reco_fhRichRingTimeLog", "rich_tb_reco_fhRichRingTimeLog", 1600, 600);
    DrawTimeLog("fhRichRingTimeLog", fNofLogEvents, true);
  }

  {
    TCanvas* c =
      fHM->CreateCanvas("rich_tb_reco_fhRichObectsPerTimeSlice", "rich_tb_reco_fhRichObectsPerTimeSlice", 1500, 500);
    c->Divide(3, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhNofRichDigisPerTS"), kLinear, kLog);
    c->cd(2);
    DrawH1(fHM->H1("fhNofRichHitsPerTS"), kLinear, kLog);
    c->cd(3);
    DrawH1(fHM->H1("fhNofRichRingsPerTS"), kLinear, kLog);
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_tb_reco_acc_rec", "rich_tb_reco_acc_rec", 1600, 600);
    c->Divide(2, 1);
    c->cd(1);
    Int_t nofAccMom    = fHM->H1("fhMomElAcc")->GetEntries();
    Int_t nofRecMom    = fHM->H1("fhMomElRec")->GetEntries();
    Int_t nofRefAccMom = fHM->H1("fhMomRefElAcc")->GetEntries();
    Int_t nofRefRecMom = fHM->H1("fhMomRefElRec")->GetEntries();
    Double_t effMom    = 100. * (Double_t) nofRecMom / (Double_t) nofAccMom;
    Double_t effRefMom = 100. * (Double_t) nofRefRecMom / (Double_t) nofRefAccMom;
    DrawH1({fHM->H1("fhMomElAcc"), fHM->H1("fhMomElRec")},
           {"Acc (" + to_string(nofAccMom) + ")", "Rec (" + to_string(nofRecMom) + ")"});
    c->cd(2);
    Int_t nofAccNofHits    = fHM->H1("fhNofHitsElAcc")->GetEntries();
    Int_t nofRecNofHits    = fHM->H1("fhNofHitsElRec")->GetEntries();
    Int_t nofRefAccNofHits = fHM->H1("fhNofHitsRefElAcc")->GetEntries();
    Int_t nofRefRecNofHits = fHM->H1("fhNofHitsRefElRec")->GetEntries();
    Double_t effNofHits    = 100. * (Double_t) nofRecNofHits / (Double_t) nofAccNofHits;
    Double_t effRefNofHits = 100. * (Double_t) nofRefRecNofHits / (Double_t) nofRefAccNofHits;
    DrawH1({fHM->H1("fhNofHitsElAcc"), fHM->H1("fhNofHitsElRec")},
           {"Acc (" + to_string(nofAccNofHits) + ")", "Rec" + to_string(nofRecNofHits) + ")"});

    fHM->CreateCanvas("rich_tb_reco_eff_mom", "rich_tb_reco_eff_mom", 800, 800);
    TH1D* hEffMom    = Cbm::DivideH1(fHM->H1("fhMomElRec"), fHM->H1("fhMomElAcc"));
    TH1D* hRefEffMom = Cbm::DivideH1(fHM->H1("fhMomRefElRec"), fHM->H1("fhMomRefElAcc"));
    DrawH1({hEffMom, hRefEffMom},
           {"Electrons (" + Cbm::NumberToString(effMom, 2) + "%)",
            "Reference electrons (" + Cbm::NumberToString(effRefMom, 2) + "%)"},
           kLinear, kLinear, true, 0.5, 0.85, 0.99, 0.99);
    hEffMom->SetMinimum(0.);
    hEffMom->SetMaximum(105.);

    fHM->CreateCanvas("rich_tb_reco_eff_nofHits", "rich_tb_reco_eff_nofHits", 800, 800);
    TH1D* hEffHits    = Cbm::DivideH1(fHM->H1("fhNofHitsElRec"), fHM->H1("fhNofHitsElAcc"));
    TH1D* hRefEffHits = Cbm::DivideH1(fHM->H1("fhNofHitsRefElRec"), fHM->H1("fhNofHitsRefElAcc"));
    DrawH1({hEffHits, hRefEffHits},
           {"Electron (" + Cbm::NumberToString(effNofHits, 2) + "%)",
            "ElectronReference (" + Cbm::NumberToString(effRefNofHits, 2) + "%)"},
           kLinear, kLinear, true, 0.5, 0.9, 0.99, 0.99);
    hEffHits->SetMinimum(0.);
    hEffHits->SetMaximum(105.);

    fHM->CreateCanvas("rich_tb_reco_eff_eventMult", "rich_tb_reco_eff_eventMult", 800, 800);
    TH1D* hEffEventMult = Cbm::DivideH1(fHM->H1("fhEventMultElRec"), fHM->H1("fhEventMultElAcc"));
    DrawH1({hEffEventMult}, {"Electron (" + Cbm::NumberToString(effNofHits, 2) + "%)"}, kLinear, kLinear, true, 0.5,
           0.9, 0.99, 0.99);
    hEffHits->SetMinimum(0.);
    hEffHits->SetMaximum(105.);
  }
}

void CbmRichRecoTbQa::DrawTimeLog(const string& hMainName, Int_t nofLogEvents, bool withNoise)
{
  Double_t max   = std::numeric_limits<Double_t>::min();
  Int_t startInd = (withNoise) ? -1 : 0;
  for (int iEv = startInd; iEv < nofLogEvents; iEv++) {
    string hName  = hMainName + "_" + to_string(iEv);
    string option = ((iEv == 0 && !withNoise) || (iEv == -1 && withNoise)) ? "" : "same";
    int ind       = iEv % 6;
    int color     = (ind == 0)   ? kBlue
                    : (ind == 1) ? kRed + 1
                    : (ind == 2) ? kGreen + 3
                    : (ind == 3) ? kMagenta + 2
                    : (ind == 4) ? kYellow + 2
                                 : kOrange + 8;
    if (ind == -1) color = kAzure + 6;
    max = std::max(max, fHM->H1(hName)->GetMaximum());
    DrawH1(fHM->H1(hName), kLinear, kLog, option, color);
  }
  fHM->H1(hMainName + "_" + to_string(startInd))->SetMaximum(max * 1.10);
  gPad->SetLeftMargin(0.07);
  gPad->SetRightMargin(0.05);
}


void CbmRichRecoTbQa::Finish()
{
  ProcessMc();
  DrawHist();
  fHM->SaveCanvasToImage(fOutputDir);

  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != NULL) {
    outFile->cd();
    fHM->WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());
}


ClassImp(CbmRichRecoTbQa)
