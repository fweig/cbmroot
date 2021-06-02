/** @file CbmAnaJpsiSuperEvent.h
  * @copyright Copyright (C) 2015-2016 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors slebedev, Semen Lebedev [committer], Florian Uhlig, Adrian Amatus Weber **/

/*
 * CbmAnaJpsiSuperEvent.h
 *
 *  Created on: Jun 25, 2015
 *      Author: slebedev
 */

#ifndef ANALYSIS_JPSIANA_CBMANAJPSISUPEREVENT_H_
#define ANALYSIS_JPSIANA_CBMANAJPSISUPEREVENT_H_

#include "CbmAnaJpsiCandidate.h"
#include "CbmAnaJpsiCuts.h"
#include "CbmAnaJpsiHist.h"
#include "CbmAnaJpsiKinematicParams.h"
#include "CbmHistManager.h"
#include "CbmSimulationReport.h"

#include "TH1D.h"
#include "TObject.h"
#include "TSystem.h"

#include <string>
#include <vector>

class CbmAnaJpsiSuperEvent : public TObject {

public:
  CbmAnaJpsiSuperEvent();

  virtual ~CbmAnaJpsiSuperEvent();

  /*
	 * \brief Add file for super event.
	 */
  void AddFile(const std::string& fileName) { fFileNames.push_back(fileName); }

  /*
	 * \brief Set output file.
	 */
  void SetOutputFile(const std::string& fileName) { fOutputFile = fileName; }

  /*
	 * \brief Set to true if you want to do super event after ptcut.
	 * This will speed up calculation time.
	 */
  void SetRunAfterPtCut(Bool_t b) { fRunAfterPtCut = b; }

  void SetRunAfterIdCut(Bool_t b) { fRunAfterIdCut = b; }

  void Run();

private:
  std::vector<std::string> fFileNames;
  std::vector<CbmAnaJpsiCandidate> fMinusCandidates;
  std::vector<CbmAnaJpsiCandidate> fPlusCandidates;

  std::string fOutputFile;

  CbmHistManager* fHM;

  CbmAnaJpsiCuts fCuts;

  Bool_t fRunAfterPtCut;
  Bool_t fRunAfterIdCut;

  void InitHist();

  void ReadCandidates();

  void DoSuperEvent();

  void Draw(Option_t* = "");

  CbmAnaJpsiSuperEvent(const CbmAnaJpsiSuperEvent&);
  CbmAnaJpsiSuperEvent operator=(const CbmAnaJpsiSuperEvent&);

  ClassDef(CbmAnaJpsiSuperEvent, 1);
};

#endif /* ANALYSIS_JPSIANA_CBMANAJPSISUPEREVENT_H_ */
