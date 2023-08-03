/*
 * NicaEventSmearTask.h
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICASMOOTHTASK_H_
#define NICASMOOTHTASK_H_

#include <FairTask.h>
#include <Rtypes.h>
#include <RtypesCore.h>

#include "NicaSmearAlgorithm.h"
#include "NicaTrackAna.h"

/**
 * task for smearing events, later such "smeared" events can be used in analysis
 * as normal events but usually some parameters are modified by this class to
 * simulate detector response
 */
class NicaSmearTask : public NicaTrackAna {
 protected:
  /**
   * output format ID
   */
  Int_t fOutputFormatID;
  /**
   * if true then smeared events are saved
   */
  Bool_t fSave;
  /**
   * pointer to smearing algorithm
   */
  NicaEventSmear *fEventAlgorithm;
  /**
   * track smearing algorithm
   */
  NicaTrackSmear *fTrackAlgorithm;
  /**
   * pointer to current smeared track
   * **/
  NicaSmearedTrack *fCurrentTrackSmeared;
  /**
   * pointer to current smeared event
   **/
  NicaSmearedEvent *fCurrentEventSmeared;
  virtual void ProcessEvent();
  virtual void CheckCutContainerCollections();
  virtual InitStatus Init();
  NicaPackage *Report() const;

 public:
  NicaSmearTask();
  /**
   * if called then smeared events will be saved in output tree
   */
  void SaveEvents();
  /**
   * for adding objects like in NicaTrack @see NicaTrackAna::Add
   * here you can also add smering algorithm
   * @param obj
   * @param opt
   */
  using FairTask::Add;
  virtual void SetTrackSmearAlgo(NicaTrackSmear &smear) {
    fTrackAlgorithm = (NicaTrackSmear *)smear.MakeCopy();
  }
  virtual void SetEventSmearAlgo(NicaEventSmear &smear) {
    fEventAlgorithm = (NicaEventSmear *)smear.MakeCopy();
  }
  virtual void Exec(Option_t *opt);
  virtual ~NicaSmearTask();
  ClassDef(NicaSmearTask, 1)
};

#endif /* NICASMOOTHTASK_H_ */
