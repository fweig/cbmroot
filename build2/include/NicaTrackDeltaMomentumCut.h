/*
 * NicaTrackDeltaCut.h
 *
 *  Created on: 20 gru 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKDELTACUT_H_
#define NICATRACKDELTACUT_H_

#include "NicaTrackCut.h"

#include <TLorentzVector.h>

class NicaTrackDeltaMomentumCut : public NicaTrackCut {
 protected:
  TLorentzVector *fMCMom;    //!
  TLorentzVector *fRecoMom;  //!
 public:
  NicaTrackDeltaMomentumCut(Int_t params = 1);
  static Int_t P() { return 0; }
  static Int_t Pt() { return 1; };
  static Int_t Phi() { return 2; }
  static Int_t Theta() { return 3; };
  static Int_t Pz() { return 4; }
  Bool_t Init(Int_t format_id = 0);
  virtual ~NicaTrackDeltaMomentumCut();
  ClassDef(NicaTrackDeltaMomentumCut, 1)
};
/**
 * standard cut for analysis of differences betwen reconstructed and MC momenta
 * using differences between given values e.g pt_reco-pt_sim
 */
class NicaTrackDeltaMomCut : public NicaTrackDeltaMomentumCut {
 protected:
  /**
   * fill momentum fields
   * @param track
   * @return true if there is imaginary part
   */
  Bool_t GetMom(NicaTrack *track);

 public:
  NicaTrackDeltaMomCut(Int_t params = 5);
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaMomCut();
  ClassDef(NicaTrackDeltaMomCut, 1)
};
/**
 * standard cut for analysis of differences betwen reconstructed and MC momenta
 * by using vector difference eg. (vec(preco)-vect(psim)).pt
 */

class NicaTrackDeltaVectMomCut : public NicaTrackDeltaMomentumCut {
 protected:
  TLorentzVector fDiff;
  /**
   * fill momentu fields
   * @param track
   * @return true if there is imaginary part
   */
  Bool_t GetMom(NicaTrack *track);

 public:
  NicaTrackDeltaVectMomCut(Int_t params = 5);
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackDeltaVectMomCut();
  ClassDef(NicaTrackDeltaVectMomCut, 1)
};

#endif /* NICATRACKDELTACUT_H_ */
