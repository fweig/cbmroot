/*
 * NicaQAEventTask.h
 *
 *  Created on: 14 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_ANALYSIS_QA_NICAQAEVENTTASK_H_
#define NICAFEMTO_ANALYSIS_QA_NICAQAEVENTTASK_H_

#include <FairTask.h>
#include <TObjArray.h>

#include "NicaEventAna.h"
#include "NicaQAPlot.h"

/**
 * task for simple QA for event, base on event ana task, therefore user
 * can select objects with cuts
 */

class NicaQAEventTask : public NicaEventAna {
  TObjArray* fEventQA;
  NicaQAPlot* fTempEventPlot;
  std::vector<TString> fEventColNames;

protected:
  NicaQAPlot* GetQAPlot(Int_t i) const { return (NicaQAPlot*) fEventQA->UncheckedAt(i); };
  virtual InitStatus Init();
  virtual void ProcessEvent();
  virtual NicaPackage* Report() const;

public:
  NicaQAEventTask();
  void SetEventCollectionNames(const std::initializer_list<TString>& init);
  void SetQAPlot(const NicaQAPlot& plot);
  virtual ~NicaQAEventTask();
  NicaQAEventTask(const NicaQAEventTask& other);
  NicaQAEventTask& operator=(const NicaQAEventTask& other);
  ClassDef(NicaQAEventTask, 1)
};

#endif /* NICAFEMTO_ANALYSIS_QA_NICAQAEVENTTASK_H_ */
