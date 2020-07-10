#include "CbmStsDigitizeQaReport.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmReportElement.h"
#include "CbmStsModule.h"
#include "CbmStsParAsic.h"
#include "CbmStsParSim.h"
#include "CbmStsSetup.h"
#include "CbmUtils.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TProfile.h"
#include "TStyle.h"
#include <boost/assign/list_of.hpp>

using boost::assign::list_of;
using Cbm::FindAndReplace;
using Cbm::NumberToString;
using Cbm::Split;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

CbmStsDigitizeQaReport::CbmStsDigitizeQaReport(CbmStsSetup* setup,
                                               const CbmStsParSim* settings,
                                               const CbmStsParAsic* asicPar)
  : CbmSimulationReport()
  , fSetup(setup)
  , fSettings(settings)
  , fAsicPar(asicPar) {
  SetReportName("STSdigitize_qa");
}

CbmStsDigitizeQaReport::~CbmStsDigitizeQaReport() {}

void CbmStsDigitizeQaReport::Create() {
  CbmStsELoss eLossModel  = fSettings->ELossModel();
  Bool_t lorentz          = fSettings->LorentzShift();
  Bool_t diffusion        = fSettings->Diffusion();
  Bool_t crossTalk        = fSettings->CrossTalk();
  Double_t dynRange       = fAsicPar->GetDynRange();
  Double_t threshold      = fAsicPar->GetThreshold();
  Int_t nAdc              = fAsicPar->GetNofAdc();
  Double_t timeResolution = fAsicPar->GetTimeResol();
  Double_t deadTime       = fAsicPar->GetDeadTime();
  Double_t noise          = fAsicPar->GetNoise();
  char eLossModelChar[15];
  if (eLossModel == CbmStsELoss::kIdeal) sprintf(eLossModelChar, "ideal");
  if (eLossModel == CbmStsELoss::kUniform) sprintf(eLossModelChar, "uniform");
  if (eLossModel == CbmStsELoss::kUrban) sprintf(eLossModelChar, "non-uniform");
  Out().precision(1);
  Out() << R()->DocumentBegin();
  Out() << R()->Title(0, GetTitle());
  Out() << "Number of events: "
        << HM()->H1("h_EventNo_DigitizeQa")->GetEntries() << endl;
  Out() << endl;
  Out() << "Digitizer parameters: " << endl;
  Out() << "\t noise \t\t\t" << noise << " e" << endl;
  Out() << "\t threshold \t\t" << threshold << " e" << endl;
  Out() << "\t dynamic range \t\t" << dynRange << " e" << endl;
  Out() << "\t number of ADC \t\t" << nAdc << endl;
  Out() << "\t time resolution \t" << timeResolution << " ns" << endl;
  Out() << "\t dead time \t\t" << deadTime << " ns" << endl;
  Out() << endl;
  Out() << "Detector response model takes into account: " << endl;
  Out() << "\t energy loss model: \t" << eLossModelChar << endl;
  Out() << "\t diffusion: \t\t" << (diffusion ? "On" : "Off") << endl;
  Out() << "\t Lorentz shift: \t" << (lorentz ? "On" : "Off") << endl;
  Out() << "\t cross talk: \t\t" << (crossTalk ? "On" : "Off") << endl;

  PrintCanvases();

  Out() << R()->DocumentEnd();
}

void CbmStsDigitizeQaReport::Draw() {
  ScaleHistograms();
  DrawNofObjectsHistograms();
  DrawLogHistograms();
  DrawHistograms();
  DrawH1ByPattern("h_DigiCharge");
  /*DrawH1ByPattern("h_DigisByPoint");
    DrawH1ByPattern("h_PointsInDigi");*/
  Draw2dHistograms();
}

void CbmStsDigitizeQaReport::DrawNofObjectsHistograms() {
  string name = "h_NofObjects_";
  if (!HM()->Exists(name + "Points") && !HM()->Exists(name + "Digis")
      && !HM()->Exists(name + "Digis_Station")
      && !HM()->Exists(name + "Points_Station"))
    return;
  string canvasName = GetReportName() + name;
  TCanvas* canvas =
    CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
  canvas->SetGrid();
  canvas->SetLogy();
  canvas->cd();
  vector<string> labels = list_of("Points")("Digis");
  vector<TH1*> histos =
    list_of(HM()->H1(name + "Points"))(HM()->H1(name + "Digis"));
  DrawH1(histos, labels, kLinear, kLinear, true, 0.5, 0.55, 0.9, 0.75);

  vector<TH1*> histos1 = list_of(HM()->H1(name + "Points_Station"))(
    HM()->H1(name + "Digis_Station"));
  canvasName = GetReportName() + "h_NofObjects_Station";
  TCanvas* canvas1 =
    CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
  canvas1->SetGrid();
  canvas1->cd();
  DrawH1(histos1, labels, kLinear, kLinear, true, 0.5, 0.55, 0.9, 0.75);
}

void CbmStsDigitizeQaReport::Draw2dHistograms() {
  string name = "h_DigisPerChip_Station";
  for (Int_t stationId = 0; stationId < fSetup->GetNofStations(); stationId++) {
    if (!HM()->Exists(Form("h_DigisPerChip_Station%i", stationId))
        && !HM()->Exists(Form("h_PointsMap_Station%i", stationId))
        && !HM()->Exists(Form("h_MeanAngleMap_Station%i", stationId))
        && !HM()->Exists(Form("h_RMSAngleMap_Station%i", stationId)))
      return;
  }
  string canvasName = GetReportName() + name;
  TCanvas* canvas =
    CreateCanvas(canvasName.c_str(), canvasName.c_str(), 2400, 600);
  canvas->Divide(4, 2);
  canvas->SetGrid();

  string name1       = "h_PointsMap_Station";
  string canvasName1 = GetReportName() + name1;
  TCanvas* canvas1 =
    CreateCanvas(canvasName1.c_str(), canvasName1.c_str(), 2400, 600);
  canvas1->Divide(4, 2);
  canvas1->SetGrid();

  string name2       = "h_MeanAngleMap_Station";
  string canvasName2 = GetReportName() + name2;
  TCanvas* canvas2 =
    CreateCanvas(canvasName2.c_str(), canvasName2.c_str(), 2400, 600);
  canvas2->Divide(4, 2);
  canvas2->SetGrid();

  string name3       = "h_RMSAngleMap_Station";
  string canvasName3 = GetReportName() + name3;
  TCanvas* canvas3 =
    CreateCanvas(canvasName3.c_str(), canvasName3.c_str(), 2400, 600);
  canvas3->Divide(4, 2);
  canvas3->SetGrid();

  for (Int_t stationId = 0; stationId < fSetup->GetNofStations(); stationId++) {
    canvas->cd(stationId + 1);
    DrawH2(HM()->H2(Form("%s%i", name.c_str(), stationId)),
           kLinear,
           kLinear,
           kLinear);

    canvas1->cd(stationId + 1);
    DrawH2(HM()->H2(Form("%s%i", name1.c_str(), stationId)),
           kLinear,
           kLinear,
           kLinear);

    canvas2->cd(stationId + 1);
    DrawH2(HM()->H2(Form("%s%i", name2.c_str(), stationId)),
           kLinear,
           kLinear,
           kLinear);

    canvas3->cd(stationId + 1);
    DrawH2(HM()->H2(Form("%s%i", name3.c_str(), stationId)),
           kLinear,
           kLinear,
           kLinear);
  }
}


void CbmStsDigitizeQaReport::DrawLogHistograms() {
  string name = "h_";
  if (!HM()->Exists(name + "PointsInDigiLog")
      && !HM()->Exists(name + "DigisByPointLog"))
    return;
  string canvasName = GetReportName() + name + "PointsInDigiLog";
  TCanvas* canvas =
    CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
  canvas->SetGrid();
  canvas->SetLogy();
  canvas->cd();
  vector<TH1*> histos = list_of(HM()->H1(name + "PointsInDigiLog"));
  vector<string> labels =
    list_of(Form("Points in digi, \n mean = %.2f",
                 HM()->H1(name + "PointsInDigiLog")->GetMean()));
  DrawH1(histos, labels, kLinear, kLinear, true, 0.5, 0.55, 0.9, 0.65);

  canvasName           = GetReportName() + name + "DigisByPointLog";
  vector<TH1*> histos1 = list_of(HM()->H1(name + "DigisByPointLog"));
  vector<string> labels1 =
    list_of(Form("Digis by point, \n mean = %.2f",
                 HM()->H1(name + "DigisByPointLog")->GetMean()));
  TCanvas* canvas1 =
    CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
  canvas1->SetGrid();
  canvas1->SetLogy();
  canvas1->cd();
  DrawH1(histos1, labels1, kLinear, kLinear, true, 0.5, 0.55, 0.9, 0.65);
}

void CbmStsDigitizeQaReport::DrawHistograms() {
  string name = "h_";
  if (!HM()->Exists(name + "PointsInDigi")
      && !HM()->Exists(name + "DigisByPoint"))
    return;
  string canvasName = GetReportName() + name + "PointsInDigi";
  TCanvas* canvas =
    CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
  canvas->SetGrid();
  canvas->cd();
  vector<TH1*> histos = list_of(HM()->H1(name + "PointsInDigi"));
  vector<string> labels =
    list_of(Form("Points in digi, \n mean = %.2f",
                 HM()->H1(name + "PointsInDigi")->GetMean()));
  DrawH1(histos, labels, kLinear, kLinear, true, 0.5, 0.55, 0.9, 0.65);

  canvasName           = GetReportName() + name + "DigisByPoint";
  vector<TH1*> histos1 = list_of(HM()->H1(name + "DigisByPoint"));
  vector<string> labels1 =
    list_of(Form("Digis by point, \n mean = %.2f",
                 HM()->H1(name + "DigisByPoint")->GetMean()));
  TCanvas* canvas1 =
    CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
  canvas1->SetGrid();
  canvas1->cd();
  DrawH1(histos1, labels1, kLinear, kLinear, true, 0.5, 0.55, 0.9, 0.65);
}


void CbmStsDigitizeQaReport::ScaleHistograms() {
  Int_t nofEvents = HM()->H1("h_EventNo_DigitizeQa")->GetEntries();
  if (nofEvents == 0) nofEvents = 1;

  HM()->ScaleByPattern("h_NofObjects_.*_Station_.*", 1. / nofEvents);
  HM()->ShrinkEmptyBinsH1ByPattern("h_NofObjects_.*_Station_.*");
}

ClassImp(CbmStsDigitizeQaReport)
