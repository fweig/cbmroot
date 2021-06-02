/** @file PairAnalysisSignalFit.h
  * @copyright Copyright (C) 2015 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Julian Book [committer] **/

#ifndef PAIRANALYSISSIGNALFIT_H
#define PAIRANALYSISSIGNALFIT_H

//#############################################################
//#                                                           #
//#         Class PairAnalysisSignalFit                      #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <TH1F.h>
#include <TString.h>
#include <TVectorT.h>

#include "PairAnalysisFunction.h"
#include "PairAnalysisSignalExt.h"

class PairAnalysisSignalFit : public PairAnalysisSignalExt {
public:
  PairAnalysisSignalFit();
  PairAnalysisSignalFit(const char* name, const char* title);
  PairAnalysisSignalFit(const PairAnalysisSignalFit& c);
  PairAnalysisSignalFit& operator=(const PairAnalysisSignalFit& c);

  virtual ~PairAnalysisSignalFit();

  virtual void Process(TObjArray* const arrhist);
  void ProcessFit(TObjArray* const arrhist);    // fit the SE +- distribution
  void ProcessFitLS(TObjArray* const arrhist);  // substract the fitted SE like-sign background
  void ProcessFitEM(TObjArray* const arrhist);  // substract the fitted ME like-sign background


  virtual void Draw(const Option_t* option = "");
  virtual void Print(Option_t* option = "") const;


protected:
  ClassDef(PairAnalysisSignalFit,
           1)  // class for signal extraction using a combined bgrd+signal fit
};

#endif
