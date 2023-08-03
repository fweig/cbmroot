/*
 * NicaTwoTrackDphiDetaCut.h
 *
 *  Created on: 19 kwi 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATWOTRACKDPHIDETACUT_H_
#define NICATWOTRACKDPHIDETACUT_H_
#include <TDatabasePDG.h>
#include <TVector2.h>

#include "NicaTwoTrackCut.h"

/** check if pair is inside the square with given d-phi* and d-eta*/

class NicaTwoTrackDphiDetaCut : public NicaTwoTrackCut {
 public:
  NicaTwoTrackDphiDetaCut();
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackDphiDetaCut();
  ClassDef(NicaTwoTrackDphiDetaCut, 1)
};

/** check if pair is inside the square with given d-phi* and d-eta*/

class NicaTwoTrackDphistarDetaCut : public NicaTwoTrackCut {
  Double_t fMinRad;
  Double_t fMaxRad;
  TDatabasePDG *fPDG;
  Double_t fMagSign, fMagField, fMagScale;
  const Double_t fMagConst;

 public:
  NicaTwoTrackDphistarDetaCut();
  /**
   * set detector radii
   * @param min inner radius in m
   * @param max outer radius in m
   */
  void SetRadius(Double_t min, Double_t max) {
    fMinRad = min;
    fMaxRad = max;
  }
  /**
   *
   * @param sign magnetic field sing
   */
  void SetMagSing(Double_t sign);
  /**
   *
   * @param field magnetic field
   */
  void SetMagField(Double_t field);
  virtual Bool_t Pass(NicaTwoTrack *pair);
  virtual Bool_t Init(Int_t task_id = 0);
  virtual NicaPackage *Report() const;
  virtual ~NicaTwoTrackDphistarDetaCut();
  ClassDef(NicaTwoTrackDphistarDetaCut, 1)
};

#endif /* NICATWOTRACKDPHIDETACUT_H_ */
