/** @file CbmReport.cxx
  * @copyright Copyright (C) 2011-2020 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [committer], Florian Uhlig **/

/**
 * \file CbmReport.cxx
 * \brief Base class for reports.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmReport.h"

#include "CbmHtmlReportElement.h"   // for CbmHtmlReportElement
#include "CbmLatexReportElement.h"  // for CbmLatexReportElement
#include "CbmReportElement.h"       // for CbmReportElement
#include "CbmTextReportElement.h"   // for CbmTextReportElement
#include "CbmUtils.h"               // for SaveCanvasAsImage

#include <TCanvas.h>  // for TCanvas
#include <TObject.h>  // for TObject

#include <fstream>  // for ofstream
#include <string>   // for operator+, operator==

using std::ofstream;
using std::string;

CbmReport::CbmReport()
  : TObject()
  , fReportName("qa_report")
  , fReportTitle("QA report")
  , fOutputDir("./")
  , fReportType(kCoutReport)
  , fR(nullptr)
  , fOut(nullptr)
  , fCanvases()
{
}

CbmReport::~CbmReport() {}

void CbmReport::CreateReportElement(ReportType reportType)
{
  fReportType = reportType;
  if (nullptr != fR) delete fR;
  if (nullptr != fOut && fReportType != kCoutReport) delete fOut;
  if (reportType == kLatexReport) {
    fR   = new CbmLatexReportElement();
    fOut = new ofstream(string(GetOutputDir() + fReportName + ".tex").c_str());
  }
  else if (reportType == kHtmlReport) {
    fR   = new CbmHtmlReportElement();
    fOut = new ofstream(string(GetOutputDir() + fReportName + ".html").c_str());
  }
  else if (reportType == kTextReport) {
    fR   = new CbmTextReportElement();
    fOut = new ofstream(string(GetOutputDir() + fReportName + ".txt").c_str());
  }
  else if (reportType == kCoutReport) {
    fR   = new CbmTextReportElement();
    fOut = &std::cout;
  }
}

void CbmReport::DeleteReportElement()
{
  //  if (nullptr != fR) delete fR;
  //  if (nullptr != fOut && fReportType != kCoutReport) delete fOut;
}

void CbmReport::CreateReports()
{
  Draw();  // User has to implement this function!
  SaveCanvasesAsImages();
  //   WriteCanvases();

  CreateReportElement(kHtmlReport);
  Create();  // User has to implement this function!
  DeleteReportElement();

  CreateReportElement(kLatexReport);
  Create();  // User has to implement this function!
  DeleteReportElement();

  CreateReportElement(kTextReport);
  Create();  // User has to implement this function!
  DeleteReportElement();

  CreateReportElement(kCoutReport);
  Create();  // User has to implement this function!
  DeleteReportElement();
}

TCanvas* CbmReport::CreateCanvas(const char* name, const char* title, Int_t ww, Int_t wh)
{
  TCanvas* canvas = new TCanvas(name, title, ww, wh);
  fCanvases.push_back(canvas);
  return canvas;
}

void CbmReport::SaveCanvasesAsImages() const
{
  if (GetOutputDir() == "") return;
  Int_t nofCanvases = fCanvases.size();
  for (Int_t i = 0; i < nofCanvases; i++) {
    TCanvas* canvas = fCanvases[i];
    Cbm::SaveCanvasAsImage(canvas, GetOutputDir() + "/png/", "png");
    Cbm::SaveCanvasAsImage(canvas, GetOutputDir() + "/eps/", "eps");
  }
}

void CbmReport::WriteCanvases() const
{
  if (GetOutputDir() == "") return;
  Int_t nofCanvases = fCanvases.size();
  for (Int_t i = 0; i < nofCanvases; i++) {
    fCanvases[i]->Write();
  }
}

void CbmReport::PrintCanvases() const
{
  Int_t nofCanvases = fCanvases.size();
  for (Int_t i = 0; i < nofCanvases; i++) {
    TCanvas* canvas = fCanvases[i];
    Out() << R()->Image(canvas->GetName(), string("png/" + string(canvas->GetName())).c_str());
  }
}

ClassImp(CbmReport)
