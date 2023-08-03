/*
 * NicaTwoTrackAnaChain.h
 *
 *  Created on: 26 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATWOTRACKANACHAIN_H_
#define NICATWOTRACKANACHAIN_H_

#include "NicaTwoTrackAna.h"

/**
 * class for making pair analysis with the same  event, track and pair
 * selection. Any cuts added to subtask will be ignored only cuts added directly
 * to this class will be used in subtasks in chain.
 */
class NicaTwoTrackAnaChain : public NicaTwoTrackAna {
  Int_t fTaskNo;
  NicaTwoTrackAna **fTask;  //[fTaskNo]
 protected:
  void RemoveTask(Int_t no);
  void ProcessEvent();
  void ProcessPair();
  void ProcessPair2();
  void ProcessPair_Mixed();
  void ProcessPair2_Mixed();
  void ProcessPair_Perfect();
  void ProcessPair2_Perfect();
  void ProcessPair_Rotated();
  void ProcessPair2_Rotated();
  void ProcessPair_Hemisphere();
  void ProcessPair2_Hemisphere();
  void ProcessPair_ChargedId();
  void ProcessPair_ChargedNId();
  void ProcessPair_Charged2();
  void ProcessPair2_Charged2();
  void ProcessPair_Charged3();
  void ProcessPair2_Charged3();
  void ProcessTrack();
  virtual void FinishEventNonIdentical();
  virtual void FinishEventIdentical();
  virtual NicaPackage *Report() const;
  /**
   * overwrite ana data holders with this data holders
   * @param ana
   */
  virtual void LinkTask(NicaTwoTrackAna *ana) const;
  /**
   * disconnect ana data holders with this data holders
   * @param ana
   */
  virtual void UnlinkTask(NicaTwoTrackAna *ana) const;
  /**
   * synchronize cut monitors and cuts
   * @param ana
   * @param end - if yes then links to higher cuts are linked - this value
   * should be true before ana->Init() and false later
   */
  virtual void SynchronizeCutContainers(NicaTwoTrackAna *ana, Bool_t end) const;
  virtual InitStatus Init();

 public:
  NicaTwoTrackAnaChain();
  /**
   * default constructor
   * @param use_background true if background must be used
   */
  NicaTwoTrackAnaChain(Bool_t use_background);
  /**
   * add subtask to chain
   * @param ana
   */
  virtual void AddAnalysis(NicaTwoTrackAna *ana);
  virtual void FinishTask();
  virtual ~NicaTwoTrackAnaChain();
  ClassDef(NicaTwoTrackAnaChain, 1)
};

#endif /* NICATWOTRACKANACHAIN_H_ */
