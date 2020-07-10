/**
 * \file CbmSimulationReport.h
 * \brief Base class for simulation reports.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#ifndef CBMSIMULATIONREPORT_H_
#define CBMSIMULATIONREPORT_H_

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

#include <string>  // for string

#include "CbmDrawHist.h"     // for kLinear, HistScale
#include "CbmHistManager.h"  // for CbmHistManager
#include "CbmReport.h"       // for CbmReport

class TH1;
class TH2;
class TH3;

/**
 * \class CbmSimulationReport
 * \brief Base class for simulation reports.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
class CbmSimulationReport : public CbmReport {
public:
  /**
    * \brief Constructor.
    */
  CbmSimulationReport();

  /**
    * \brief Destructor.
    */
  virtual ~CbmSimulationReport();

  /**
    * \brief Main function which creates report data.
    *
    * Non virtual interface pattern is used here.
    * User always creates simulation report using this public non virtual method.
    * In order to configure concrete report generation class
    * user has to implement protected Create() method.
    *
    * \param[in] histManager Pointer to histogram manager for which report is created.
    * \param[in] outputDir Path to directory for output results.
    */
  void Create(CbmHistManager* histManager, const std::string& outputDir);

  /**
    * \brief Main function which creates report data.
    *
    * Same pattern is used here.
    *
    * \param[in] fileName Name of the file with histograms.
    * \param[in] outputDir Path to directory for output results.
    */
  void Create(const std::string& fileName, const std::string& outputDir);

  /**
    * \brief Inherited from CbmReport. Pure abstract function which is called from public Create() function.
    */
  virtual void Create() = 0;

  /**
    * \brief Return pointer to TH1 histogram.
    * \param[in] name Name of histogram.
    * \return pointer to TH1 histogram.
    */
  TH1* H1(const std::string& name) const { return HM()->H1(name); }

  /**
    * \brief Return pointer to TH2 histogram.
    * \param[in] name Name of histogram.
    * \return pointer to TH2 histogram.
    */
  TH2* H2(const std::string& name) const { return HM()->H2(name); }

  /**
    * \brief Return pointer to TH3 histogram.
    * \param[in] name Name of histogram.
    * \return pointer to TH3 histogram.
    */
  TH3* H3(const std::string& name) const { return HM()->H3(name); }

  /**
    * \brief Return pointer to Histogram manager.
    */
  CbmHistManager* HM() const { return fHM; }

  /**
    * \brief Select by pattern TH1 histograms and draw each histogram on separate canvas.
    * \param[in] histNamePattern Name pattern for histogram.
    */
  void DrawH1ByPattern(const std::string& histNamePattern);

  /**
    * \brief Select by pattern TH1 histograms and draw all histograms on the same canvas.
    * \param[in] histNamePattern Name pattern for histogram.
    */
  void DrawH1ByPattern(const std::string& histNamePattern,
                       std::string (*labelFormatter)(const std::string&,
                                                     const CbmHistManager*));

  /**
    * \brief Select by pattern TH2 histograms and draw each histogram on separate canvas.
    * \param[in] histNamePattern Name pattern for histogram.
    * \param[in] logx Specify logarithmic or linear scale for X axis.
    * \param[in] logy Specify logarithmic or linear scale for Y axis.
    * \param[in] logz Specify logarithmic or linear scale for Z axis.
    * \param[in] drawOpt Other drawing options (see ROOT documentation for details).
    */
  void DrawH2ByPattern(const std::string& histNamePattern,
                       HistScale logx             = kLinear,
                       HistScale logy             = kLinear,
                       HistScale logz             = kLinear,
                       const std::string& drawOpt = "");

private:
  CbmHistManager* fHM;  // Histogram manager

  CbmSimulationReport(const CbmSimulationReport&);
  CbmSimulationReport& operator=(const CbmSimulationReport&);

  ClassDef(CbmSimulationReport, 1)
};

#endif /* CBMSIMULATIONREPORT_H_ */
