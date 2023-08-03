/*
 * NicaEventImpactParameterCut.h
 *
 *  Created on: 15-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAEVENTIMPACTPARAMETERCUT_H_
#define NICAEVENTIMPACTPARAMETERCUT_H_

#include "NicaEventCut.h"
/**
 * check impact parameter value
 */
class NicaEventImpactParameterCut : public NicaEventCut {
 protected:
 public:
  NicaEventImpactParameterCut();
  virtual Bool_t Pass(NicaEvent *event);
  virtual Bool_t Init(Int_t task_id);
  virtual ~NicaEventImpactParameterCut();
  ClassDef(NicaEventImpactParameterCut, 1)
};

/**
 * check centrality of event by simple calculations that use impact parameter
 */
class NicaEventImpactCentralityCut : public NicaEventCut {
  Double_t fBMax;
  Double_t fAlpha;
  Double_t ImpactToCentrality(const Double_t impact);

 public:
  NicaEventImpactCentralityCut();
  /**
   * set maximum expected impact parameter for given kind of collisions
   * @param bmax maximum expected impact parameter
   */
  void SetBMax(Double_t bmax);
  Bool_t Init(Int_t task_id);
  virtual Bool_t Pass(NicaEvent *event);
  virtual ~NicaEventImpactCentralityCut();
  ClassDef(NicaEventImpactCentralityCut, 1)
};
#endif /* NICAEVENTIMPACTPARAMETERCUT_H_ */
