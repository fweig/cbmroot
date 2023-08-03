/*
 * NicaTwoTrack.h
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATWOTRACK_H_
#define NICATWOTRACK_H_

#include <TObject.h>

class NicaTrack;

/**
 * class for storing pair of tracks
 */
class NicaTwoTrack : public TObject {
 public:
  enum PairType { kSignal, kBackground, kMixed, kRotated, kHemishpere, kOther };

 private:
  NicaTrack *fTrack1;
  NicaTrack *fTrack2;
  PairType fType;
  Bool_t fSame;

 public:
  NicaTwoTrack();
  /**
   * build pair from tracks
   * @param track1 first track
   * @param track2 second track
   */
  inline void FastBuild(NicaTrack *track1, NicaTrack *track2) {
    fTrack1 = track1;
    fTrack2 = track2;
  };
  /**
   * swap tracks in pair
   */
  inline void SwapTracks() {
    NicaTrack *temp = fTrack1;
    fTrack1 = fTrack2;
    fTrack2 = temp;
  }
  /**
   * add first track
   * @param track first track
   */
  inline void AddFirstTrack(NicaTrack *track) { fTrack1 = track; };
  /**
   * add second track
   * @param track second  track
   */
  inline void AddSecondTrack(NicaTrack *track) { fTrack2 = track; };
  /**
   * set pair type
   * @param type
   */
  inline void SetPairType(PairType type) { fType = type; }
  /**
   *
   * @return pair type
   */
  inline PairType GetPairType() const { return fType; };
  /**
   *
   * @return pointer to first track (in "fake" format)
   */
  inline NicaTrack *GetTrack1() const { return fTrack1; };
  /**
   *
   * @return pointer to second track (in "fake" format)
   */
  inline NicaTrack *GetTrack2() const { return fTrack2; };
  /**
   * same if particles have the same type
   * @param same
   */
  inline void SetSame(Bool_t same) { fSame = same; };
  /**
   *
   * @return true if both particles are the same type
   */
  inline Bool_t AreSame() const { return fSame; };
  virtual ~NicaTwoTrack();
  ClassDef(NicaTwoTrack, 1)
};

#endif /* NICATWOTRACK_H_ */
