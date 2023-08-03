/*
 * NicaTrackAnaChain.h
 *
 *  Created on: 26 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef BASE_NICATRACKANACHAIN_H_
#define BASE_NICATRACKANACHAIN_H_

#include "NicaTrackAna.h"
/**
 * Class for making analysis where track and event selection can be common. All
 * event and track cuts added to subtasks will be ingored, only cuts added
 * directly to chain will be used in this chain.
 */
class NicaTrackAnaChain : public NicaTrackAna {
  Int_t fTaskNo;
  NicaTrackAna **fTask;  //[fTaskNo]
  void GoToDir(TString name);

 protected:
  /**
   * remove subtask
   * @param no subtask number
   */
  virtual void RemoveTask(Int_t no);
  virtual void ProcessEvent();
  virtual NicaPackage *Report() const;
  /**
   * overwrite ana data holders with this data holders
   * @param ana
   */
  void LinkTask(NicaTrackAna *ana) const;
  /**
   * disconnect ana data holders with this data holders
   * @param ana
   */
  void UnlinkTask(NicaTrackAna *ana) const;
  /**
   * synchronize cut monitors and cuts
   * @param ana
   * @param end - if yes then links to higher cuts are copied - this value
   * should be true before ana->Init() and false later
   */
  void SynchronizeCutContainers(NicaTrackAna *ana, Bool_t end) const;
  virtual InitStatus Init();

 public:
  NicaTrackAnaChain();
  virtual void Exec(Option_t *opt);
  /**
   * add subtask to analysis
   * @param ana
   */
  virtual void AddAnalysis(NicaTrackAna *ana);
  virtual void FinishTask();
  virtual ~NicaTrackAnaChain();
  ClassDef(NicaTrackAnaChain, 1)
};

#endif /* BASE_NICATRACKANACHAIN_H_ */
