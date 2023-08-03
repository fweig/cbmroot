/*
 * NicaEventAnaChain.h
 *
 *  Created on: 26 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAEVENTANACHAIN_H_
#define NICAEVENTANACHAIN_H_

#include "NicaEventAna.h"
/**
 * Class for making analysis that require the same event selection algorithm.
 * Any event cuts in added classes "subtasks" will be ignored, only EventCuts
 * added directly to this class will be used. Classes that require track
 * buffering (like NicaTwoTrack ) cannot be used as subtasks.
 */
class NicaEventAnaChain : public NicaEventAna {
  Int_t fTaskNo;
  NicaEventAna **fTask;  //[fTaskNo]
  void GoToDir(TString name);

 protected:
  /**
   * remove task from chain
   * @param no
   */
  virtual void RemoveTask(Int_t no);
  virtual void ProcessEvent();
  virtual NicaPackage *Report() const;
  /**
   * overwrite ana data holders with this data holders
   * @param ana
   */
  virtual void LinkTask(NicaEventAna *ana) const;
  /**
   * disconnect ana data holders with this data holders
   * @param ana
   */
  virtual void UnlinkTask(NicaEventAna *ana) const;
  /**
   * synchronize cut monitors and cuts
   * @param ana
   * @param end - if yes then links to higher cuts are linked - this value
   * should be true before ana->Init() and false later
   */
  virtual void SynchronizeCutContainers(NicaEventAna *ana, Bool_t end) const;
  virtual InitStatus Init();

 public:
  NicaEventAnaChain();
  virtual void Exec(Option_t *opt);
  /**
   * add subtask
   * @param ana subtask
   */
  virtual void AddAnalysis(NicaEventAna *ana);
  virtual void FinishTask();
  virtual ~NicaEventAnaChain();
  ClassDef(NicaEventAnaChain, 1)
};

#endif /* NICAEVENTANACHAIN_H_ */
