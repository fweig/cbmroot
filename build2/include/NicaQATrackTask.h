/*
 * NicaQATrackTask.h
 *
 *  Created on: 14 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_ANALYSIS_QA_NICAQATRACKTASK_H_
#define NICAFEMTO_ANALYSIS_QA_NICAQATRACKTASK_H_

#include "NicaQAPlot.h"
#include "NicaTrackAna.h"

#include <TObjArray.h>

/**
 * class for QA plots, works similar to the standard NicaTrackAnalysis
 */

class NicaQATrackTask : public NicaTrackAna {
  TObjArray* fEventQA;
  NicaQAPlot* fTempEventPlot;
  TObjArray* fTrackQA;
  NicaQAPlot* fTempTrackPlot;
  std::vector<TString> fEventColNames;
  std::vector<TString> fTrackColNames;

protected:
  virtual InitStatus Init();
  virtual void ProcessEvent();
  virtual void ProcessTrack();
  virtual void LinkCollections();
  virtual NicaPackage* Report() const;
  NicaQAPlot* GetEventQAPlot(Int_t i) const { return (NicaQAPlot*) fEventQA->UncheckedAt(i); };
  NicaQAPlot* GetTrackQAPlot(Int_t i) const { return (NicaQAPlot*) fTrackQA->UncheckedAt(i); };

public:
  NicaQATrackTask();
  void SetEventCollectionNames(const std::initializer_list<TString>& init);
  void SetTrackCollectionNames(const std::initializer_list<TString>& init);
  void SetQAPlot(const NicaQAPlot& plot);
  virtual ~NicaQATrackTask();
  NicaQATrackTask(const NicaQATrackTask& other);
  NicaQATrackTask& operator=(const NicaQATrackTask& other);
  ClassDef(NicaQATrackTask, 1)
};

#endif /* NICAFEMTO_ANALYSIS_QA_NICAQATRACKTASK_H_ */
