/*
 * NicaTwoTrackCut.h
 *
 *  Created on: 06-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATWOTRACKCUT_H_
#define NICATWOTRACKCUT_H_

#include "NicaCut.h"

class NicaTwoTrack;

/**
 * basic class for all two-track cuts
 */
class NicaTwoTrackCut : public NicaCut {
 public:
  /**
   * default constructor
   * @param i number of parameter for checking
   */
  NicaTwoTrackCut(const Int_t i = 1);
  virtual ~NicaTwoTrackCut();
  /**
   * check pair
   * @param pair pair to check
   * @return true if pair is fine, false if should be rejected
   */
  virtual Bool_t Pass(NicaTwoTrack* pair) = 0;
  virtual NicaPackage* Report() const;
  ClassDef(NicaTwoTrackCut, 1)
};

#endif /* NICATWOTRACKCUT_H_ */
