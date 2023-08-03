/*
 * NicaTrackBoosteMomentaCuts.h
 *
 *  Created on: 2 sie 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKBOOSTEMOMENTACUTS_H_
#define NICATRACKBOOSTEMOMENTACUTS_H_

#include "NicaTrackComplexCut.h"

class NicaTrackBoostedPCut : public NicaTrackCut {
 protected:
  TVector3 fBoost;

 public:
  NicaTrackBoostedPCut();
  void SetBoostVector(TVector3 boost) { fBoost = boost; };
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackBoostedPCut(){};
  ClassDef(NicaTrackBoostedPCut, 1)
};

class NicaTrackBoostedKinematcisCut : public NicaTrackCut {
 protected:
  TVector3 fBoost;

 public:
  NicaTrackBoostedKinematcisCut();
  void SetBoostVector(TVector3 boost) { fBoost = boost; };
  static Int_t Px() { return 0; };
  static Int_t Py() { return 1; };
  static Int_t Pz() { return 2; };
  static Int_t Pt() { return 3; };
  static Int_t Eta() { return 4; };
  static Int_t Rapidity() { return 5; };
  static Int_t P() { return 6; }
  Bool_t Init(Int_t format_id = 0);
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackBoostedKinematcisCut();
  ClassDef(NicaTrackBoostedKinematcisCut, 1)
};

class NicaTrackBoostedDeltaPCut : public NicaTrackCut {
 protected:
  TVector3 fBoost;

 public:
  NicaTrackBoostedDeltaPCut();
  void SetBoostVector(TVector3 boost) { fBoost = boost; };
  Bool_t Init(Int_t format_id = 0);
  virtual Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackBoostedDeltaPCut(){};
  ClassDef(NicaTrackBoostedDeltaPCut, 1)
};

#endif /* NICAFEMTO_CUTS_TRACKCUTS_KINEMATICS_NICATRACKBOOSTEMOMENTACUTS_H_ */
