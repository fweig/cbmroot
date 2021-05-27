/** @file CbmAnaJpsiSuperEventReport.h
  * @copyright Copyright (C) 2015-2016 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev [orginator], Adrian Amatus Weber, Florian Uhlig **/

#ifndef CBMANAJPSISUPEREVENTREPORT_H
#define CBMANAJPSISUPEREVENTREPORT_H

#include "CbmAnaJpsiHist.h"
#include "CbmSimulationReport.h"

#include "TSystem.h"

#include <string>

class CbmAnaJpsiSuperEventReport : public CbmSimulationReport {
public:
  /**
    * \brief Constructor.
    */
  CbmAnaJpsiSuperEventReport();

  /**
    * \brief Destructor.
    */
  virtual ~CbmAnaJpsiSuperEventReport();

  /*
    * \brief Create report.
    */
  void Create(const std::string& fileEventByEvent, const std::string& fileSuperEvent, const std::string& outputDir);

protected:
  /**
    * \brief Inherited from CbmSimulationReport.
    */
  virtual void Create();

  /**
    * \brief Inherited from CbmSimulationReport.
    */
  virtual void Draw();

  /*
    * \brief Draw comparison between superevent and event-by-event
    */
  void DrawComparison();

  void DrawMinvSignalBg();

  double SignalOverBg(int step);

  void SignalOverBgAllSteps();

  void DrawPairSource();

  void DrawMinvMismatchPt();

  void DrawMinvDiffPtBins();

  CbmHistManager* fHMSuperEvent;    // histogram manager for super event histograms
  CbmHistManager* fHMEventByEvent;  //histogram manager for event-by-event histograms

private:
  CbmAnaJpsiSuperEventReport(const CbmAnaJpsiSuperEventReport&);
  CbmAnaJpsiSuperEventReport operator=(const CbmAnaJpsiSuperEventReport&);

  ClassDef(CbmAnaJpsiSuperEventReport, 1);
};

#endif
