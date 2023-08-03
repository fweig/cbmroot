/*
 * NicaTrackAna.h
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATRACKANA_H_
#define NICATRACKANA_H_

#include <FairTask.h>

#include "NicaEventAna.h"

class NicaTrack;

class NicaTrackAnaChain;
/**
 * base class for making track analysis
 */
class NicaTrackAna : public NicaEventAna {
  friend class NicaTrackAnaChain;

 protected:
  /**
   * total number of track collections
   */
  Int_t fTrackCollectionsNo;
  /**
   * current track collection
   */
  Int_t fCurrentTrackCollectionID;
  /**
   * track index in loop over tracks
   */
  Int_t fTrackIndex;
  /**
   * pointer to currently processed track
   */
  NicaTrack *fCurrentTrack;
  virtual void ProcessEvent();
  //--- for overwrite by user ---------------------------//
  /**
   * method for processing track, should be overwritten in derived class if they
   * process tracks
   */
  virtual void ProcessTrack();
  virtual void LinkCollections();
  virtual void CheckCutContainerCollections();
  virtual InitStatus Init();
  virtual NicaPackage *Report() const;

 public:
  NicaTrackAna();
  /**
   * copy ctor
   * @param ana
   */
  NicaTrackAna(const NicaTrackAna &ana);
  NicaTrackAna &operator=(const NicaTrackAna &other);
  virtual void SetOption(Option_t *option);
  virtual void Exec(Option_t *opt);
  virtual void FinishTask();
  virtual ~NicaTrackAna();
  ClassDef(NicaTrackAna, 1)
};
#endif /* NICATRACKANA_H_ */
