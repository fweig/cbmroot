/*
 * NicaMultiTrackAna.h
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAMULTITRACKANA_H_
#define NICAMULTITRACKANA_H_

#include "NicaTrackAna.h"
/**
 * basic class for processing tracks, or trakc groups where buffering is
 * required
 */
class NicaMultiTrackAna : public NicaTrackAna {
 protected:
  virtual void ProcessTrack();
  virtual void ProcessEvent();
  virtual void InitNewCutContainer();
  virtual InitStatus Init();
  virtual InitStatus CheckFormat();
  virtual NicaPackage *Report() const;

 public:
  NicaMultiTrackAna();
  /**
   * copy ctor
   * @param ana
   */
  NicaMultiTrackAna(const NicaMultiTrackAna &ana);
  NicaMultiTrackAna &operator=(const NicaMultiTrackAna &other);
  /**
   * set size of buffer used in mixing
   * @param to_mix size of buffer
   */
  void SetMixSize(Int_t to_mix);
  /**
   *  set format buffered
   * @param format
   */
  void SetFormatBuffered(NicaEvent *format) { SetFormat(format, ENicaFormatDepth::kBuffered); };
  virtual void FinishTask();
  virtual void SetOption(Option_t *option);
  virtual ~NicaMultiTrackAna();
  ClassDef(NicaMultiTrackAna, 1)
};

#endif /* NICAMULTITRACKANA_H_ */
