/*
 * NicaEventMultiplicityCut.h
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAEVENTMULTIPLICITYCUT_H_
#define NICAEVENTMULTIPLICITYCUT_H_

#include <TDatabasePDG.h>
#include "NicaEventCut.h"
#include "NicaTrack.h"
/**
 * check total number of tracks in event
 */
class NicaEventMultiplicityCut : public NicaEventCut {
 protected:
 public:
  NicaEventMultiplicityCut();
  virtual Bool_t Pass(NicaEvent *event);
  virtual ~NicaEventMultiplicityCut();
  ClassDef(NicaEventMultiplicityCut, 1)
};

/**
 * check multiplicity of charged particles in given range for given event
 */
class NicaEventMultiChargedCut : public NicaEventCut {
  Double_t fScale;
  Double_t fMinEta, fMaxEta, fRange;
  TDatabasePDG *fPDG;

 public:
  NicaEventMultiChargedCut();
  Bool_t Pass(NicaEvent *event);
  /**
   * set pseudorpaidity range for pairs calucalted as charged
   * @param min minimum rapidity
   * @param max maximum rapidity
   */
  void SetAveragingRange(Double_t min, Double_t max);
  /**
   * takes into account detector inefficiency, number of charged tracks is
   * multiplied by this value
   * @param scale scale for charged multiplicity value
   */
  void SetEfficiencyScale(Double_t scale);
  virtual Bool_t Init(Int_t task_id = 0);
  virtual ~NicaEventMultiChargedCut();
  virtual NicaPackage *Report() const;
  ClassDef(NicaEventMultiChargedCut, 1)
};

#endif /* NICAEVENTMULTIPLICITYCUT_H_ */
