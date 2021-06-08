/* Copyright (C) 2011-2020 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Florian Uhlig */

/**
 * \file CbmLitReport.h
 * \brief Base class for reports.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */

#ifndef CBMREPORT_H_
#define CBMREPORT_H_

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Option_t
#include <TObject.h>     // for TObject

#include <iostream>  // for string, ostream
#include <vector>    // for vector

class CbmReportElement;
class TCanvas;

/**
 * \enumeration ReportType
 * \brief Enumeration defines different report types.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */
enum ReportType
{
  kCoutReport,
  kHtmlReport,
  kLatexReport,
  kTextReport
};

/**
 * \class CbmReport
 * \brief Base class for reports.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */
class CbmReport : public TObject {
public:
  /**
    * \brief Constructor.
    */
  CbmReport();

  /**
    * \brief Destructor.
    */
  virtual ~CbmReport();

  /**
    * \brief Accessor to CbmReportElement object.
    * User has to write the report using available tags from CbmReportElement class.
    */
  const CbmReportElement* R() const { return fR; }

  /**
    * \brief All text output goes to this stream.
    */
  std::ostream& Out() const { return *fOut; }

  /* Setters */
  void SetReportName(const std::string& name) { fReportName = name; }
  void SetReportTitle(const std::string& title) { fReportTitle = title; }
  void SetOutputDir(const std::string& outputDir) { fOutputDir = outputDir; }

  /* Accessors */
  const std::string& GetReportName() const { return fReportName; }
  const std::string& GetReportTitle() const { return fReportTitle; }
  const std::string& GetOutputDir() const { return fOutputDir; }

protected:
  /**
    * \brief Pure abstract function which is called from public Create() function.
    * This function has to write report using Out() for output stream and
    * R() for report elements. This function is called for each report type.
    */
  virtual void Create() = 0;

  /**
    * \brief Pure abstract function which is called from public Create() function.
    * This function has to draw all necessary histograms and graphs.
    * It is called only once before report creation.
    */
  virtual void Draw() = 0;

  /**
    * \brief Inherited from TObject.
    * This method is implemented in order to avoid warnings.
    */
  void Draw(Option_t*) { ; }


  /**
    * \brief Create all available report types.
    */
  void CreateReports();

  /**
    * \brief Create canvas and put it to vector of TCanvases.
    * Canvases created with this function will be automatically saved to image and
    * printed in the report.
    */
  TCanvas* CreateCanvas(const char* name, const char* title, Int_t ww, Int_t wh);

  /**
    * \brief Save all canvases to images.
    */
  void SaveCanvasesAsImages() const;

  /**
    * \brief Write canvases to file.
    */
  void WriteCanvases() const;

  /**
    * \brief Print images created from canvases in the report.
    */
  void PrintCanvases() const;

private:
  /**
    * \brief Create concrete CbmReportElement instance based on report type.
    * \param[in] reportType Type of the report to be produced.
    */
  void CreateReportElement(ReportType reportType);

  /**
    * \brief Delete report element. Normally should be called at the end of Create function.
    */
  void DeleteReportElement();

  std::string fReportName;     // Name of report
  std::string fReportTitle;    // Title of report
  std::string fOutputDir;      // Output directory for the report files
  ReportType fReportType;      // Current report type
  CbmReportElement* fR;        // Report element tool
  mutable std::ostream* fOut;  // Output stream

  // Storage for TCanvas. All Canvases in this vector will be automatically saved
  // to image and printed in the report.
  // User can use CreateCanvas function which automatically push created canvas in this vector.
  std::vector<TCanvas*> fCanvases;

  //private:

  CbmReport(const CbmReport&);
  CbmReport& operator=(const CbmReport&);

  ClassDef(CbmReport, 1)
};

#endif /* CBMREPORT_H_ */
