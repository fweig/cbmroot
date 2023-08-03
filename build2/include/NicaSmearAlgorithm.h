/*
 * NicaSmothAlgorithm.h
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICASMOTHALGORITHM_H_
#define NICASMOTHALGORITHM_H_

#include <FairTask.h>
#include <TObject.h>


class NicaPackage;
class NicaSmearedEvent;
class NicaSmearedTrack;

/**
 * abstract class for smearing algorithms
 */

class NicaSmearAlgorithm : public TObject {
 public:
  NicaSmearAlgorithm(){};
  /**
   * initalize this class
   * @return status of initialization
   */
  virtual InitStatus Init() { return kSUCCESS; };
  /**
   *
   * @return package with information about this object
   */
  virtual NicaPackage* Report() const;
  /**
   * make copy of this object
   * @return
   */
  virtual NicaSmearAlgorithm* MakeCopy() const = 0;
  virtual ~NicaSmearAlgorithm(){};
  ClassDef(NicaSmearAlgorithm, 1)
};

/**
 * abstract class for modifying events - later such events can be used as
 * "reconstructed" if user use smeared format, currently only event reaction
 * plane can be replaced and track momenta
 */
class NicaEventSmear : public NicaSmearAlgorithm {
 public:
  NicaEventSmear();
  /**
   * modify event parameters
   * @param mod event to modify
   */
  virtual void ModifyEvent(NicaSmearedEvent* mod) = 0;
  virtual NicaPackage* Report() const;
  virtual ~NicaEventSmear();
  ClassDef(NicaEventSmear, 1)
};

/**
 * class that don't modify event
 */
class NicaEventSmearVirtual : public NicaEventSmear {
 public:
  NicaEventSmearVirtual();
  virtual void ModifyEvent(NicaSmearedEvent* mod);
  virtual NicaSmearAlgorithm* MakeCopy() const;
  virtual NicaPackage* Report() const;
  virtual ~NicaEventSmearVirtual();
  ClassDef(NicaEventSmearVirtual, 1)
};

/**
 * abstract class for modifying tracks (smearing) @see NicaEventSmear, currently
 * only track momenta can be smeared
 */
class NicaTrackSmear : public NicaSmearAlgorithm {
 public:
  NicaTrackSmear();
  /**
   * modify track parameters
   * @param mod smeared track
   */
  virtual void ModifyTrack(NicaSmearedTrack* mod) = 0;
  virtual ~NicaTrackSmear();
  ClassDef(NicaTrackSmear, 1)
};

/**
 * class that don't modify track
 */
class NicaTrackSmearVirtual : public NicaTrackSmear {
 public:
  NicaTrackSmearVirtual();
  virtual void ModifyTrack(NicaSmearedTrack* mod);
  virtual NicaSmearAlgorithm* MakeCopy() const;
  virtual ~NicaTrackSmearVirtual();
  ClassDef(NicaTrackSmearVirtual, 1)
};

#endif /* NICASMOTHALGORITHM_H_ */
