/**
 * \file CbmSimulationReport.cxx
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmSimulationReport.h"

#include "CbmDrawHist.h"     // for DrawH1, kLinear, DrawH2, HistScale
#include "CbmHistManager.h"  // for CbmHistManager

#include <RtypesCore.h>  // for UInt_t
#include <TDirectory.h>  // for TDirectory, gDirectory
#include <TFile.h>       // for TFile, gFile
#include <TH1.h>         // for TH1
#include <TH2.h>         // for TH2

#include <fstream>  // for string, ofstream
#include <string>   // for operator+
#include <vector>   // for vector, __vector_base<>::value_type

#include <assert.h>  // for assert

using std::ofstream;
using std::string;
using std::vector;

CbmSimulationReport::CbmSimulationReport() : CbmReport(), fHM(nullptr) {}

CbmSimulationReport::~CbmSimulationReport() {}

void CbmSimulationReport::Create(CbmHistManager* histManager, const string& outputDir)
{
  assert(histManager != nullptr);
  fHM = histManager;
  SetOutputDir(outputDir);
  CreateReports();
}

void CbmSimulationReport::Create(const string& fileName, const string& outputDir)
{
  assert(fHM == nullptr);
  fHM = new CbmHistManager();

  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* file = new TFile(fileName.c_str());
  fHM->ReadFromFile(file);
  SetOutputDir(outputDir);
  CreateReports();
  //   delete fHM;
  //   delete file;

  // shouldn't the file be closed ????
  //  file->Close();
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmSimulationReport::DrawH1ByPattern(const string& histNamePattern)
{
  vector<TH1*> histos = HM()->H1Vector(histNamePattern);
  UInt_t nofHistos    = histos.size();
  if (nofHistos < 1) return;
  for (UInt_t iHist = 0; iHist < nofHistos; iHist++) {
    TH1* hist         = histos[iHist];
    string canvasName = GetReportName() + hist->GetName();
    //      TCanvas* canvas = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
    CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
    DrawH1(hist, kLinear, kLinear);
  }
}

void CbmSimulationReport::DrawH1ByPattern(const string& histNamePattern,
                                          string (*labelFormatter)(const string&, const CbmHistManager*))
{
  vector<TH1*> histos = HM()->H1Vector(histNamePattern);
  UInt_t nofHistos    = histos.size();
  if (nofHistos < 1) return;
  string canvasName = GetReportName() + histos[0]->GetName();
  //   TCanvas* canvas = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 600, 500);
  CreateCanvas(canvasName.c_str(), canvasName.c_str(), 600, 500);

  vector<string> labels(nofHistos);
  for (UInt_t iHist = 0; iHist < nofHistos; iHist++) {
    string name   = histos[iHist]->GetName();
    labels[iHist] = labelFormatter(name, HM());
  }

  DrawH1(histos, labels, kLinear, kLinear, true, 0.3, 0.3, 0.85, 0.6, "PE1");
}

void CbmSimulationReport::DrawH2ByPattern(const string& histNamePattern, HistScale logx, HistScale logy, HistScale logz,
                                          const string& drawOpt)
{
  vector<TH2*> histos = HM()->H2Vector(histNamePattern);
  UInt_t nofHistos    = histos.size();
  if (nofHistos < 1) return;
  for (UInt_t iHist = 0; iHist < nofHistos; iHist++) {
    TH2* hist         = histos[iHist];
    string canvasName = GetReportName() + hist->GetName();
    //        TCanvas* canvas = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
    CreateCanvas(canvasName.c_str(), canvasName.c_str(), 800, 500);
    DrawH2(hist, logx, logy, logz, drawOpt);
  }
}

ClassImp(CbmSimulationReport)
