/*
 * NicaTrackMtCut.h
 *
 *  Created on: 20-05-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICATRACKMTCUT_H_
#define NICATRACKMTCUT_H_
#include <TDatabasePDG.h>
#include "NicaTrackCut.h"
/**
 * check transerse mass of track
 */
class NicaTrackMtCut : public NicaTrackCut {
  Bool_t fMassFixed;
  Double_t fMass2;
  TDatabasePDG *fPdg;

 public:
  NicaTrackMtCut();
  /**
   * copy constructor
   * @param cut cut to make copy form
   */
  NicaTrackMtCut(const NicaTrackMtCut &cut);
  /**
   * fix mass used to calculate Mt
   * @param mass mass of particle
   */
  void FixMass(Double_t mass);
  virtual Bool_t Pass(NicaTrack *track);
  virtual Bool_t Init(const Int_t task_id = 0);
  ;
  virtual NicaPackage *Report() const;
  virtual ~NicaTrackMtCut();
  ClassDef(NicaTrackMtCut, 1)
};

#endif /* NICATRACKMTCUT_H_ */
