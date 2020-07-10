/**
 * \file CbmRichGeoTestOpt.cxx
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2019
 */
#include "CbmRichGeoTestOpt.h"

#include "CbmDrawHist.h"
#include "utils/CbmUtils.h"

#include "TBox.h"
#include "TFile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TText.h"
#include <iostream>
#include <utility>

using namespace std;

CbmRichGeoTestOpt::CbmRichGeoTestOpt()
  : fReferenceInd(-1), fDrawReference(false), fHM(nullptr), fOutputDir("") {}

CbmRichGeoTestOpt::~CbmRichGeoTestOpt() {}

void CbmRichGeoTestOpt::SetFilePathes(vector<string> geoTestBoxPathes,
                                      vector<string> geoTestOmega3Pathes,
                                      vector<string> geoTestOmega8Pathes,
                                      vector<string> urqmdTestPathes,
                                      vector<string> recoQaBoxPathes,
                                      vector<string> recoQaUrqmdPathes) {
  fGeoTestBoxPathes    = geoTestBoxPathes;
  fGeoTestOmega3Pathes = geoTestOmega3Pathes;
  fGeoTestOmega8Pathes = geoTestOmega8Pathes;
  fUrqmdTestPathes     = urqmdTestPathes;
  fRecoQaBoxPathes     = recoQaBoxPathes;
  fRecoQaUrqmdPathes   = recoQaUrqmdPathes;
}

int CbmRichGeoTestOpt::GetNofFiles() { return fGeoTestBoxPathes.size(); }

string CbmRichGeoTestOpt::GetFilePath(CbmRichGeoTestOptFileEnum fileEnum,
                                      int iFile) {
  string path = "";
  if (fileEnum == kGeoTestBoxFile) path = fGeoTestBoxPathes[iFile];
  if (fileEnum == kGeoTestOmega3File) path = fGeoTestOmega3Pathes[iFile];
  if (fileEnum == kGeoTestOmega8File) path = fGeoTestOmega8Pathes[iFile];
  if (fileEnum == kUrqmdTestFile) path = fUrqmdTestPathes[iFile];
  if (fileEnum == kRecoQaBoxFile) path = fRecoQaBoxPathes[iFile];
  if (fileEnum == kRecoQaUrqmdFile) path = fRecoQaUrqmdPathes[iFile];

  return path;
}

string CbmRichGeoTestOpt::GetFileEnumName(CbmRichGeoTestOptFileEnum fileEnum) {
  string path = "";
  if (fileEnum == kGeoTestBoxFile) return "kGeoTestBoxFile";
  if (fileEnum == kGeoTestOmega3File) return "kGeoTestOmega3File";
  if (fileEnum == kGeoTestOmega8File) return "kGeoTestOmega8File";
  if (fileEnum == kUrqmdTestFile) return "kUrqmdTestFile";
  if (fileEnum == kRecoQaBoxFile) return "kRecoQaBoxFile";
  if (fileEnum == kRecoQaUrqmdFile) return "kRecoQaUrqmdFile";

  return path;
}

pair<double, double>
CbmRichGeoTestOpt::H1MeanRms(CbmRichGeoTestOptFileEnum fileEnum,
                             int iFile,
                             const string& histName) {
  string path = GetFilePath(fileEnum, iFile);
  if (path == "") return make_pair(0., 0.);
  TFile* file = new TFile(path.c_str(), "READ");
  if (file == nullptr) return make_pair(0., 0.);
  TH1D* hist = (TH1D*) file->Get(histName.c_str());
  if (hist == nullptr) return make_pair(0., 0.);
  double mean = hist->GetMean();
  double rms  = hist->GetRMS();
  file->Close();
  delete file;
  return make_pair(mean, rms);
}

pair<double, double>
CbmRichGeoTestOpt::H2ProjYMeanRms(CbmRichGeoTestOptFileEnum fileEnum,
                                  int iFile,
                                  const string& histName) {
  string path = GetFilePath(fileEnum, iFile);
  if (path == "") return make_pair(0., 0.);
  TFile* file = new TFile(path.c_str(), "READ");
  if (file == nullptr) return make_pair(0., 0.);
  TH2D* hist = (TH2D*) file->Get(histName.c_str());
  if (hist == nullptr) return make_pair(0., 0.);
  TH1D* py = hist->ProjectionY((histName + to_string(iFile) + "_py").c_str());
  double mean = py->GetMean();
  double rms  = py->GetRMS();
  file->Close();
  delete file;
  return make_pair(mean, rms);
}

double CbmRichGeoTestOpt::HEntries(CbmRichGeoTestOptFileEnum fileEnum,
                                   int iFile,
                                   const string& histName) {
  string path = GetFilePath(fileEnum, iFile);
  if (path == "") return 0.;
  TFile* file = new TFile(path.c_str(), "READ");
  if (file == nullptr) return 0.;
  TH1* hist = (TH1*) file->Get(histName.c_str());
  if (hist == nullptr) return 0.;
  double entries = hist->GetEntries();  //hist->Integral();
  file->Close();
  delete file;
  return entries;
}


void CbmRichGeoTestOpt::DrawMeanRms(CbmRichGeoTestOptFileEnum fileEnum,
                                    const string& histName,
                                    CbmRichGeoTestOptHistEnum histEnum,
                                    const string& titleY,
                                    double minY,
                                    double maxY,
                                    int nofParts,
                                    int nofFilesPart) {
  string canvasName = "richgeoopt_sim_" + GetFileEnumName(fileEnum) + "_"
                      + histName + to_string(nofParts);
  fHM->CreateCanvas(canvasName, canvasName, 900, 600);
  vector<TH1*> hists;
  vector<string> legend;
  double refValue = 0.;
  for (int iP = 0; iP < nofParts; iP++) {
    string histNameInd = GetFileEnumName(fileEnum) + "_sim_" + histName + "_"
                         + to_string(nofParts) + "_" + to_string(iP);
    fHM->Create1<TH1D>(histNameInd,
                       histNameInd + ";Geometry;" + titleY,
                       nofFilesPart,
                       .5,
                       nofFilesPart + .5);
    for (int iF = 0; iF < nofFilesPart; iF++) {
      double value = 0.;
      int ind      = iP * nofFilesPart + iF;
      if (histEnum == kH1MeanHist)
        value = H1MeanRms(fileEnum, ind, histName).first;
      else if (histEnum == kH1RmsHist)
        value = H1MeanRms(fileEnum, ind, histName).second;
      else if (histEnum == kH2MeanHist)
        value = H2ProjYMeanRms(fileEnum, ind, histName).first;
      else if (histEnum == kH2RmsHist)
        value = H2ProjYMeanRms(fileEnum, ind, histName).second;
      else if (histEnum == kHEntriesHist)
        value =
          HEntries(fileEnum, ind, histName) / 1000.;  // TODO: get nof events

      if (ind + 1 == fReferenceInd) refValue = value;
      fHM->H1(histNameInd)->SetBinContent(iF + 1, value);
    }
    hists.push_back(fHM->H1(histNameInd));
    legend.push_back(to_string(iP));
  }

  DrawManyH1(hists, legend, minY, maxY);
  DrawReferenceLineH1(refValue);
}


void CbmRichGeoTestOpt::DrawMeanEff(CbmRichGeoTestOptFileEnum fileEnum,
                                    const string& histName1,
                                    const string& histName2,
                                    const string& titleY,
                                    double minY,
                                    double maxY,
                                    int nofParts,
                                    int nofFilesPart,
                                    double effCoeff) {
  string canvasName = "richgeoopt_sim_" + GetFileEnumName(fileEnum) + "_"
                      + histName1 + "_" + histName2 + to_string(nofParts);
  fHM->CreateCanvas(canvasName, canvasName, 900, 600);
  vector<TH1*> hists;
  vector<string> legend;
  double refValue = 0.;
  for (int iP = 0; iP < nofParts; iP++) {
    string histEffName = GetFileEnumName(fileEnum) + "_sim_" + histName1 + "_"
                         + histName2 + "_" + to_string(nofParts) + "_"
                         + to_string(iP);
    //int nofFilesPart = (GetNofFiles() / nofParts) + 1;
    fHM->Create1<TH1D>(histEffName,
                       histEffName + ";Geometry;" + titleY,
                       nofFilesPart,
                       .5,
                       nofFilesPart + .5);
    for (int iF = 0; iF < nofFilesPart; iF++) {
      int ind         = iP * nofFilesPart + iF;
      double entries1 = HEntries(fileEnum, ind, histName1);
      double entries2 = HEntries(fileEnum, ind, histName2);
      double value    = (entries2 != 0) ? effCoeff * entries1 / entries2 : 0.;
      if (ind + 1 == fReferenceInd) refValue = value;
      fHM->H1(histEffName)->SetBinContent(iF + 1, value);
    }
    hists.push_back(fHM->H1(histEffName));
    legend.push_back(to_string(iP));
  }
  DrawManyH1(hists, legend, minY, maxY);
  DrawReferenceLineH1(refValue);
}

void CbmRichGeoTestOpt::DrawMeanRms2D(CbmRichGeoTestOptFileEnum fileEnum,
                                      const string& histName,
                                      CbmRichGeoTestOptHistEnum histEnum,
                                      const string& titleZ,
                                      double minZ,
                                      double maxZ,
                                      int precision) {
  string canvasName =
    "richgeoopt_sim_" + GetFileEnumName(fileEnum) + "_" + histName + "_2d";
  TCanvas* c = fHM->CreateCanvas(canvasName, canvasName, 2000, 1000);
  c->Divide(4, 2);
  vector<string> camTilt = {
    "0 deg", "3 deg", "6 deg", "9 deg", "12 deg", "15 deg", "18 deg", "21 deg"};
  int nofParts         = camTilt.size();
  int nofFilesPart     = GetNofFiles() / nofParts;
  int nofX             = 9;
  int nofY             = 9;
  double shift         = 0.5 * 200. / 10.;
  double refBinCenterX = -1., refBinCenterY = -1.;
  double refBinWidthX = -1., refBinWidthY = -1.;
  for (int iP = 0; iP < nofParts; iP++) {
    string histNameInd = GetFileEnumName(fileEnum) + "_sim_" + histName + "_"
                         + to_string(iP) + "_2d";
    fHM->Create2<TH2D>(histNameInd,
                       histNameInd + ";Shift CamY [mm];Shift CamZ [mm];"
                         + titleZ,
                       nofX,
                       -100. - shift,
                       100 + shift,
                       nofY,
                       -100. - shift,
                       100 + shift);
    for (int iX = 0; iX < nofX; iX++) {
      for (int iY = 0; iY < nofY; iY++) {
        double value = 0.;
        int ind      = iP * nofFilesPart + iX * nofX + iY;
        //cout << iP << " " << iX << " " << iY << " " << ind << endl;
        if (histEnum == kH1MeanHist)
          value = H1MeanRms(fileEnum, ind, histName).first;
        else if (histEnum == kH1RmsHist)
          value = H1MeanRms(fileEnum, ind, histName).second;
        else if (histEnum == kH2MeanHist)
          value = H2ProjYMeanRms(fileEnum, ind, histName).first;
        else if (histEnum == kH2RmsHist)
          value = H2ProjYMeanRms(fileEnum, ind, histName).second;
        else if (histEnum == kHEntriesHist)
          value =
            HEntries(fileEnum, ind, histName) / 1000.;  // TODO: get nof events
        if (ind + 1 == fReferenceInd) {
          refBinCenterX =
            fHM->H2(histNameInd)->GetXaxis()->GetBinCenter(iX + 1);
          refBinCenterY =
            fHM->H2(histNameInd)->GetYaxis()->GetBinCenter(iY + 1);
          refBinWidthX = fHM->H2(histNameInd)->GetXaxis()->GetBinWidth(iX + 1);
          refBinWidthY = fHM->H2(histNameInd)->GetYaxis()->GetBinWidth(iY + 1);
        }
        fHM->H2(histNameInd)->SetBinContent(iX + 1, iY + 1, value);
      }
    }
    c->cd(iP + 1);
    DrawH2(fHM->H2(histNameInd), kLinear, kLinear, kLinear, "COLZ");
    DrawTextLabelsH2(fHM->H2(histNameInd), precision);
    fHM->H2(histNameInd)->GetZaxis()->SetRangeUser(minZ, maxZ);
    fHM->H2(histNameInd)->SetMarkerSize(1.2);
    DrawTextOnPad(camTilt[iP], 0.1, 0.9, 0.9, 0.99);
    if (refBinWidthX > 0. && refBinWidthY > 0.) {
      DrawReferenceBoxH2(
        refBinCenterX, refBinCenterY, refBinWidthX, refBinWidthY);
      refBinWidthX = -1.;
      refBinWidthY = -1.;
    }
  }
}

void CbmRichGeoTestOpt::DrawMeanEff2D(CbmRichGeoTestOptFileEnum fileEnum,
                                      const string& histName1,
                                      const string& histName2,
                                      const string& titleZ,
                                      double minZ,
                                      double maxZ,
                                      double effCoeff,
                                      int precision) {
  string canvasName = "richgeoopt_sim_" + GetFileEnumName(fileEnum) + "_"
                      + histName1 + "_" + histName2 + "_2d";
  TCanvas* c = fHM->CreateCanvas(canvasName, canvasName, 2000, 1000);
  c->Divide(4, 2);
  vector<string> camTilt = {
    "0 deg", "3 deg", "6 deg", "9 deg", "12 deg", "15 deg", "18 deg", "21 deg"};
  int nofParts         = camTilt.size();
  int nofFilesPart     = GetNofFiles() / nofParts;
  int nofX             = 9;
  int nofY             = 9;
  double shift         = 0.5 * 200. / 10.;
  double refBinCenterX = -1., refBinCenterY = -1.;
  double refBinWidthX = -1., refBinWidthY = -1.;
  for (int iP = 0; iP < nofParts; iP++) {
    string histNameInd = GetFileEnumName(fileEnum) + "_sim_" + histName1 + "_"
                         + histName2 + "_" + to_string(iP) + "_2d";
    fHM->Create2<TH2D>(histNameInd,
                       histNameInd + ";Shift CamY [mm];Shift CamZ [mm];"
                         + titleZ,
                       nofX,
                       -100. - shift,
                       100 + shift,
                       nofY,
                       -100. - shift,
                       100 + shift);
    for (int iX = 0; iX < nofX; iX++) {
      for (int iY = 0; iY < nofY; iY++) {
        int ind = iP * nofFilesPart + iX * nofX + iY;
        //cout << iP << " " << iX << " " << iY << " " << ind << endl;
        double entries1 = HEntries(fileEnum, ind, histName1);
        double entries2 = HEntries(fileEnum, ind, histName2);
        double value    = (entries2 != 0) ? effCoeff * entries1 / entries2 : 0.;
        if (ind + 1 == fReferenceInd) {
          refBinCenterX =
            fHM->H2(histNameInd)->GetXaxis()->GetBinCenter(iX + 1);
          refBinCenterY =
            fHM->H2(histNameInd)->GetYaxis()->GetBinCenter(iY + 1);
          refBinWidthX = fHM->H2(histNameInd)->GetXaxis()->GetBinWidth(iX + 1);
          refBinWidthY = fHM->H2(histNameInd)->GetYaxis()->GetBinWidth(iY + 1);
        }
        fHM->H2(histNameInd)->SetBinContent(iX + 1, iY + 1, value);
      }
    }
    c->cd(iP + 1);
    DrawH2(fHM->H2(histNameInd), kLinear, kLinear, kLinear, "COLZ");
    DrawTextLabelsH2(fHM->H2(histNameInd), precision);
    fHM->H2(histNameInd)->SetMarkerSize(1.2);
    fHM->H2(histNameInd)->GetZaxis()->SetRangeUser(minZ, maxZ);
    DrawTextOnPad(camTilt[iP], 0.1, 0.9, 0.9, 0.99);
    if (refBinWidthX > 0. && refBinWidthY > 0.) {
      DrawReferenceBoxH2(
        refBinCenterX, refBinCenterY, refBinWidthX, refBinWidthY);
      refBinWidthX = -1.;
      refBinWidthY = -1.;
    }
  }
}

void CbmRichGeoTestOpt::DrawTextLabelsH2(TH2* h, int precision) {
  for (int i = 1; i <= h->GetNbinsX(); i++) {
    for (int j = 1; j <= h->GetNbinsY(); j++) {
      auto t = new TText(
        h->GetXaxis()->GetBinCenter(i),
        h->GetYaxis()->GetBinCenter(j),
        Cbm::NumberToString(h->GetBinContent(i, j), precision).c_str());
      t->SetTextAlign(22);
      t->SetTextFont(43);
      t->SetTextSize(12);
      t->SetTextColor(kBlack);
      t->Draw();
    }
  }
}

void CbmRichGeoTestOpt::DrawManyH1(const vector<TH1*>& hist,
                                   const vector<string>& legend,
                                   double minY,
                                   double maxY) {
  vector<string> camTilt = {
    "0 deg", "3 deg", "6 deg", "9 deg", "12 deg", "15 deg", "18 deg", "21 deg"};
  bool drawLegend = (hist.size() == 1) ? false : true;
  DrawH1(hist,
         (hist.size() == camTilt.size()) ? camTilt : legend,
         kLinear,
         kLinear,
         drawLegend,
         0.9,
         0.75,
         0.99,
         0.99);
  gPad->SetLeftMargin(0.1);
  gPad->SetRightMargin(0.01);
  hist[0]->GetYaxis()->SetTitleOffset(0.8);
  hist[0]->GetYaxis()->SetRangeUser(minY, maxY);
  DrawLines(false, true, minY, maxY);
}

void CbmRichGeoTestOpt::DrawLines(bool drawCamTilt,
                                  bool drawCamY,
                                  double minY,
                                  double maxY) {
  double nMinY = minY + 0.8 * (maxY - minY);
  double nMaxY = maxY;
  // cam Y
  if (drawCamY) {
    for (int i = 1; i <= 72; i++) {
      TLine* line = new TLine(i * 9 + .5, nMinY, i * 9 + .5, nMaxY);
      line->SetLineColor(kGreen + 2);
      line->SetLineWidth(2);
      line->Draw();
    }
  }

  // cam Tilt
  if (drawCamTilt) {
    for (int i = 1; i <= 8; i++) {
      TLine* line = new TLine(i * 81 + .5, nMinY, i * 81 + .5, nMaxY);
      line->SetLineColor(kBlue + 2);
      line->SetLineWidth(2);
      line->Draw();
    }
  }
}

void CbmRichGeoTestOpt::DrawReferenceLineH1(double value) {
  if (!fDrawReference) return;

  TLine* line = new TLine(0., value, 1e5, value);
  line->SetLineColor(kGreen + 4);
  line->SetLineWidth(1);
  line->Draw();
}

void CbmRichGeoTestOpt::DrawReferenceBoxH2(double centerX,
                                           double centerY,
                                           double widthX,
                                           double widthY) {
  if (!fDrawReference) return;

  TBox* box = new TBox(centerX - 0.5 * widthX,
                       centerY - 0.5 * widthY,
                       centerX + 0.5 * widthX,
                       centerY + 0.5 * widthY);
  box->SetLineColor(kGreen + 4);
  box->SetLineWidth(2);
  box->SetFillStyle(0);
  box->Draw();
}

void CbmRichGeoTestOpt::Draw(Option_t*) {
  fHM = new CbmHistManager();

  SetDefaultDrawStyle();
  //    int nofFilesPartAll = GetNofFiles() / 1.;
  int nofFilesPartCamTilt = GetNofFiles() / 8;

  //1D
  DrawMeanRms(kGeoTestBoxFile,
              "fhNofHits_hits",
              kH1MeanHist,
              "#hits in ring",
              27,
              31,
              8,
              nofFilesPartCamTilt);
  DrawMeanRms(kGeoTestBoxFile,
              "fhBoverAVsMom_points",
              kH2MeanHist,
              "B/A (hit fit)",
              0.88,
              0.95,
              8,
              nofFilesPartCamTilt);
  DrawMeanRms(kGeoTestBoxFile,
              "fhDRVsNofHits",
              kH2RmsHist,
              "dR.RMS [cm]",
              0.29,
              0.4,
              8,
              nofFilesPartCamTilt);
  DrawMeanRms(kGeoTestBoxFile,
              "fhRadiusVsNofHits",
              kH2RmsHist,
              "Radius.RMS [cm]",
              0.3,
              0.6,
              8,
              nofFilesPartCamTilt);
  DrawMeanEff(kGeoTestBoxFile,
              "fhAccMomEl",
              "fhMcMomEl",
              "Geometrical acceptance [%]",
              88,
              92,
              8,
              nofFilesPartCamTilt,
              100.);
  DrawMeanRms(kUrqmdTestFile,
              "fh_nof_hits_per_event",
              kH1MeanHist,
              "Nof hits per event",
              600,
              850,
              8,
              nofFilesPartCamTilt);
  DrawMeanRms(kUrqmdTestFile,
              "fh_gamma_nontarget_mom",
              kHEntriesHist,
              "e^{#pm}_{not target} from #gamma per event",
              20,
              24,
              8,
              nofFilesPartCamTilt);
  DrawMeanEff(kRecoQaBoxFile,
              "hte_Rich_Rich_Electron_Rec_p",
              "hte_Rich_Rich_Electron_Acc_p",
              "Ring reco efficiency [%]",
              84,
              98,
              8,
              nofFilesPartCamTilt,
              100.);
  DrawMeanEff(kRecoQaBoxFile,
              "hte_StsRich_StsRich_Electron_Rec_p",
              "hte_StsRich_StsRich_Electron_Acc_p",
              "STS-RICH matching efficiency [%]",
              80,
              90,
              8,
              nofFilesPartCamTilt,
              100.);
  DrawMeanEff(kRecoQaUrqmdFile,
              "hte_Rich_Rich_Electron_Rec_p",
              "hte_Rich_Rich_Electron_Acc_p",
              "Ring reco efficiency [%]",
              86,
              100,
              8,
              nofFilesPartCamTilt,
              100.);
  DrawMeanEff(kRecoQaUrqmdFile,
              "hte_StsRich_StsRich_Electron_Rec_p",
              "hte_StsRich_StsRich_Electron_Acc_p",
              "STS-RICH matching efficiency [%]",
              82,
              94,
              8,
              nofFilesPartCamTilt,
              100.);
  DrawMeanEff(kRecoQaUrqmdFile,
              "hps_Rich_All_RecPions_p",
              "hps_Rich_All_Rec_p",
              "Pion Suppression (RICH)",
              100,
              240,
              8,
              nofFilesPartCamTilt,
              1.);
  DrawMeanEff(kRecoQaUrqmdFile,
              "hpe_StsRich_StsRich_Electron_Rec_p",
              "hpe_StsRich_StsRich_Electron_Acc_p",
              "El id (RICH) [%]",
              80,
              92,
              8,
              nofFilesPartCamTilt,
              100.);
  DrawMeanEff(kGeoTestOmega3File,
              "fhAccMomEl",
              "fhMcMomEl",
              "Geometrical acceptance [%]",
              45,
              52,
              8,
              nofFilesPartCamTilt,
              100.);
  DrawMeanEff(kGeoTestOmega8File,
              "fhAccMomEl",
              "fhMcMomEl",
              "Geometrical acceptance [%]",
              52,
              58,
              8,
              nofFilesPartCamTilt,
              100.);

  //2D
  DrawMeanRms2D(
    kGeoTestBoxFile, "fhNofHits_hits", kH1MeanHist, "#hits in ring", 27, 31, 1);
  DrawMeanRms2D(kGeoTestBoxFile,
                "fhBoverAVsMom_points",
                kH2MeanHist,
                "B/A (point fit)",
                0.88,
                0.95,
                1);
  DrawMeanRms2D(
    kGeoTestBoxFile, "fhDRVsNofHits", kH2RmsHist, "dR.RMS [cm]", 0.29, 0.4, 1);
  DrawMeanRms2D(kGeoTestBoxFile,
                "fhRadiusVsNofHits",
                kH2RmsHist,
                "Radius.RMS [cm]",
                0.3,
                0.6,
                1);
  DrawMeanEff2D(kGeoTestBoxFile,
                "fhAccMomEl",
                "fhMcMomEl",
                "Geometrical acceptance [%]",
                88,
                92,
                100.,
                1);
  DrawMeanRms2D(kUrqmdTestFile,
                "fh_nof_hits_per_event",
                kH1MeanHist,
                "Nof hits per event",
                600,
                850,
                0);
  DrawMeanRms2D(kUrqmdTestFile,
                "fh_gamma_nontarget_mom",
                kHEntriesHist,
                "e^{#pm}_{not target} from #gamma per event",
                20,
                24,
                1);
  DrawMeanEff2D(kRecoQaBoxFile,
                "hte_Rich_Rich_Electron_Rec_p",
                "hte_Rich_Rich_Electron_Acc_p",
                "Ring reco efficiency [%]",
                84,
                98,
                100.,
                1);
  DrawMeanEff2D(kRecoQaBoxFile,
                "hte_StsRich_StsRich_Electron_Rec_p",
                "hte_StsRich_StsRich_Electron_Acc_p",
                "STS-RICH matching efficiency [%]",
                80,
                90,
                100.,
                1);
  DrawMeanEff2D(kRecoQaUrqmdFile,
                "hte_Rich_Rich_Electron_Rec_p",
                "hte_Rich_Rich_Electron_Acc_p",
                "Ring reco efficiency [%]",
                86,
                100,
                100.,
                1);
  DrawMeanEff2D(kRecoQaUrqmdFile,
                "hte_StsRich_StsRich_Electron_Rec_p",
                "hte_StsRich_StsRich_Electron_Acc_p",
                "STS-RICH matching efficiency [%]",
                82,
                94,
                100.,
                1);
  DrawMeanEff2D(kRecoQaUrqmdFile,
                "hps_Rich_All_RecPions_p",
                "hps_Rich_All_Rec_p",
                "Pion Suppression (RICH)",
                100,
                240,
                1.,
                0);
  DrawMeanEff2D(kRecoQaUrqmdFile,
                "hpe_StsRich_StsRich_Electron_Rec_p",
                "hpe_StsRich_StsRich_Electron_Acc_p",
                "El id (RICH) [%]",
                80,
                92,
                100.,
                1);
  DrawMeanEff2D(kGeoTestOmega3File,
                "fhAccMomEl",
                "fhMcMomEl",
                "Geometrical acceptance [%]",
                45,
                52,
                100.,
                1);
  DrawMeanEff2D(kGeoTestOmega8File,
                "fhAccMomEl",
                "fhMcMomEl",
                "Geometrical acceptance [%]",
                52,
                58,
                100.,
                1);

  fHM->SaveCanvasToImage(fOutputDir);
}


ClassImp(CbmRichGeoTestOpt)
