/*
 * NicaTwoTrackRadialDistanceCut.h
 *
 *  Created on: 13-11-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 *		modified AliFemtoTwoTrackCutRadialDistance
 */
#ifndef NICATWOTRACKRADIALDISTANCECUT_H_
#define NICATWOTRACKRADIALDISTANCECUT_H_
#include <TDatabasePDG.h>
#include <TVector2.h>
#include "NicaTwoTrackCut.h"

#define mag_const 0.15
/**
 * check radial distance cut in barbell like detector where is constant magnetic
 * field in Z direction
 */
class NicaTwoTrackRadialDistanceCut : public NicaTwoTrackCut {
  Double_t fMinRad;
  Double_t fMaxRad;
  TDatabasePDG *fPDG;
  Double_t fMagSign, fMagField;
  Double_t fMagConst;

 public:
  NicaTwoTrackRadialDistanceCut();
  /**
   * set TPC/detector radius
   * @param min inner radius
   * @param max outer radius
   */
  void SetRadius(Double_t min, Double_t max);
  /**
   * set magnetic field
   * @param field magnetic field
   */
  void SetMagFiled(Double_t field);
  /**
   * set magnetic field sign
   * @param sign
   */
  void SetMagFieldSign(Double_t sign);
  Bool_t Init(Int_t task_id = 0);
  Bool_t Pass(NicaTwoTrack *pair);
  virtual ~NicaTwoTrackRadialDistanceCut();
  ClassDef(NicaTwoTrackRadialDistanceCut, 1)
};

#endif /* NICATWOTRACKRADIALDISTANCECUT_H_ */
