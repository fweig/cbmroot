#include "CbmTrdMCQa.h"

#include "CbmHistManager.h"
#include "CbmSimulationReport.h"
#include "CbmTrdAddress.h"
#include "CbmTrdPoint.h"
//#include "CbmTrdMCQaReport.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TF1.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "TProfile.h"
#include "TProfile2D.h"

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

CbmTrdMCQa::CbmTrdMCQa()
  : FairTask()
  , fHM(new CbmHistManager())
  , fTrdPoints(NULL)
  , fMCTracks(NULL)
  , fNofStation(10) {}

CbmTrdMCQa::~CbmTrdMCQa() {
  if (fHM) delete fHM;
}

InitStatus CbmTrdMCQa::Init() {
  LOG(info) << "Trd Setup consist of " << fNofStation << " stations.";

  ReadDataBranches();
  CreateHistograms();
  return kSUCCESS;
}

void CbmTrdMCQa::ReadDataBranches() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) LOG(fatal) << "No FairRootManager!";

  fTrdPoints = dynamic_cast<TClonesArray*>(ioman->GetObject("TrdPoint"));
  if (NULL == fTrdPoints) LOG(error) << "No TrdPoint array!";

  fMCTracks = dynamic_cast<TClonesArray*>(ioman->GetObject("MCTrack"));
  if (NULL == fMCTracks) LOG(error) << "No MCTrack array!";
}

void CbmTrdMCQa::CreateHistograms() {
  CreateNofObjectsHistograms();
  CreatePointHistograms();
  fHM->Create1<TH1F>("h_trd_EventNo_MCQa", "h_trd_EventNo_MCQa", 1, 0, 1.);
}

void CbmTrdMCQa::CreateNofObjectsHistograms() {

  Int_t nofBins = 100;
  Double_t minX = -0.5;
  Double_t maxX = 99.5;
  string name   = "h_trd_NofObjects_";
  fHM->Create1<TH1F>(name + "Points",
                     name + "Points;Objects per event;Entries",
                     nofBins,
                     minX,
                     maxX);

  nofBins = fNofStation;
  minX    = -0.5;
  maxX    = static_cast<Float_t>(fNofStation) - 0.5;
  fHM->Create1<TH1F>(name + "Points_Station",
                     name + "Points_Station;Station number;Objects per event",
                     nofBins,
                     minX,
                     maxX);
}

void CbmTrdMCQa::CreatePointHistograms() {
  for (Int_t stationId = 0; stationId < fNofStation; stationId++) {
    Int_t nofBins = 100;
    Double_t minX = -0.5;
    Double_t maxX = 99.5;
    fHM->Create1<TH1F>(
      Form("h_trd_MultPoints_Station%i", stationId),
      Form("Mult, Station %i;Objects per event;Entries", stationId),
      nofBins,
      minX,
      maxX);

    fHM->Create2<TH2F>(Form("h_trd_PointsMap_Station%i", stationId),
                       Form("TrdPoint, Station %i;x, cm;y, cm", stationId),
                       120,
                       -60.,
                       60.,
                       120,
                       -60.,
                       60.);
    fHM->Create2<TH2F>(
      Form("h_trd_PointsMapEvent_Station%i", stationId),
      Form("TrdPoint/cm^{2}, Station %i;x, cm;y, cm", stationId),
      120,
      -60.,
      60.,
      120,
      -60.,
      60.);
    fHM->Create2<TH2F>(
      Form("h_trd_PointsMapRate_Station%i", stationId),
      Form("TrdPoint/cm^{2}/s, Station %i;x, cm;y, cm", stationId),
      120,
      -60.,
      60.,
      120,
      -60.,
      60.);
    fHM->Create1<TH1F>(Form("h_trd_XPos_Station%i", stationId),
                       "X position;x, cm; Entries",
                       1200,
                       -60.,
                       60.);
    fHM->Create1<TH1F>(Form("h_trd_YPos_Station%i", stationId),
                       "Y position;y, cm; Entries",
                       1200,
                       -60.,
                       60.);
  }
  fHM->Create1<TH1F>(
    "h_trd_XPos", "X position;x, cm; Entries", 1200, -60., 60.);
  fHM->Create1<TH1F>(
    "h_trd_YPos", "Y position;y, cm; Entries", 1200, -60., 60.);
}

void CbmTrdMCQa::Exec(Option_t*) {
  ProcessPoints(fTrdPoints);
  fHM->H1("h_trd_EventNo_MCQa")->Fill(0.5);
}

void CbmTrdMCQa::ProcessPoints(const TClonesArray* points) {

  fHM->H1("h_trd_NofObjects_Points")->Fill(points->GetEntriesFast());

  Double_t pointX = 0.;
  Double_t pointY = 0.;
  //  Double_t pointZ=0.;

  std::map<Int_t, vector<Int_t>> used_map;

  for (Int_t iPoint = 0; iPoint < points->GetEntriesFast(); iPoint++) {
    const CbmTrdPoint* trdPoint =
      static_cast<const CbmTrdPoint*>(points->At(iPoint));
    Int_t stationId = CbmTrdAddress::GetLayerId(trdPoint->GetModuleAddress());
    ;
    fHM->H1("h_trd_NofObjects_Points_Station")->Fill(stationId);

    pointX = trdPoint->GetXIn();
    pointY = trdPoint->GetYIn();
    //    pointZ = trdPoint -> GetZIn();

    fHM->H1(Form("h_trd_XPos_Station%i", stationId))->Fill(pointX);
    fHM->H1(Form("h_trd_YPos_Station%i", stationId))->Fill(pointY);

    fHM->H2(Form("h_trd_PointsMap_Station%i", stationId))->Fill(pointX, pointY);
    fHM->H2(Form("h_trd_PointsMapEvent_Station%i", stationId))
      ->Fill(pointX, pointY);
    fHM->H2(Form("h_trd_PointsMapRate_Station%i", stationId))
      ->Fill(pointX, pointY);
    fHM->H1("h_trd_XPos")->Fill(pointX);
    fHM->H1("h_trd_YPos")->Fill(pointY);

    Int_t mcTrackID = trdPoint->GetTrackID();

    if (std::find(
          used_map[stationId].begin(), used_map[stationId].end(), mcTrackID)
        == used_map[stationId].end()) {
      used_map[stationId].push_back(mcTrackID);
    }
  }
  fHM->H1(Form("h_trd_MultPoints_Station%i", 0))->Fill(used_map[0].size());
  fHM->H1(Form("h_trd_MultPoints_Station%i", 1))->Fill(used_map[1].size());
  fHM->H1(Form("h_trd_MultPoints_Station%i", 2))->Fill(used_map[2].size());
  fHM->H1(Form("h_trd_MultPoints_Station%i", 3))->Fill(used_map[3].size());
}

void CbmTrdMCQa::Finish() {

  Int_t nofEvents = fHM->H1("h_trd_EventNo_MCQa")->GetEntries();
  // Do here some scaling of the histograms to have MCPoint per cm^2

  Int_t xbins  = (fHM->H2("h_trd_PointsMap_Station0"))->GetXaxis()->GetNbins();
  Float_t xmax = fHM->H2("h_trd_PointsMap_Station0")->GetXaxis()->GetXmax();
  Float_t xmin = fHM->H2("h_trd_PointsMap_Station0")->GetXaxis()->GetXmin();
  Float_t scaleX = static_cast<Float_t>(xbins) / (xmax - xmin);

  Int_t ybins    = fHM->H2("h_trd_PointsMap_Station0")->GetYaxis()->GetNbins();
  Int_t ymax     = fHM->H2("h_trd_PointsMap_Station0")->GetYaxis()->GetXmax();
  Int_t ymin     = fHM->H2("h_trd_PointsMap_Station0")->GetYaxis()->GetXmin();
  Float_t scaleY = static_cast<Float_t>(ybins) / (ymax - ymin);

  Float_t scale = scaleX * scaleY;
  scale         = 1.;

  for (Int_t i = 0; i < fNofStation; ++i) {
    fHM->Scale(Form("h_trd_PointsMapEvent_Station%i", i), scale / nofEvents);
    fHM->Scale(Form("h_trd_PointsMapRate_Station%i", i),
               10000000. * scale / nofEvents);
  }

  gDirectory->mkdir("QA/TrdMCQa");
  gDirectory->cd("QA/TrdMCQa");
  fHM->WriteToFile();
  gDirectory->cd("../..");
  //    CbmSimulationReport* report = new CbmTrdMCQaReport(fSetup, fDigitizer);
  //    report -> Create(fHM, fOutputDir);
  //    delete report;

  // Compare results with defined benchmark results and raise an ERROR if there are differences
  // Either get default histogramms from a benchmark qa file or as parameters from the parameter container
}

ClassImp(CbmTrdMCQa);
