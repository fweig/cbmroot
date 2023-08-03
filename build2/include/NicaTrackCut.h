/*
 * NicaTrackCut.h
 *
 *  Created on: 21-06-2013
 *      Author: Daniel Wielanek
 */

#ifndef NICATRACKCUT_H_
#define NICATRACKCUT_H_

#include "NicaCut.h"
/**
 * abstract class for all track cuts
 */
class NicaTrack;
class NicaTrackComplexCut;
class NicaTrackCut : public NicaCut {
 protected:
 public:
  /**
   * default constructor
   * @param i number of checked parameters
   */
  NicaTrackCut(const Int_t i = 1);
  virtual ~NicaTrackCut();
  /**
   *
   * @param track
   * @return true if track pass, false otherwise
   */
  virtual Bool_t Pass(NicaTrack* track) = 0;
  /**
   *
   * @param opt - option of making complex cut, by default
   * cut contains imaginary and real part (like complex(this,this), however by
   * specify "re" or "im" you can enforce creation cut (this,virtual) or
   * (virtual, this) rescpectively
   * @return complex cut from this cut
   * i
   */
  NicaTrackComplexCut* MakeComplexCut(Option_t* opt) const;
  virtual NicaPackage* Report() const;
  ClassDef(NicaTrackCut, 1)
};

#endif /* NICATRACKCUT_H_ */
