/*
 * NicaCorrFitSmear1DCF.h
 *
 *  Created on: 4 sty 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFITSMEAR1DCF_H_
#define NICACORRFITSMEAR1DCF_H_


#include "NicaCorrFitKisiel.h"

class NicaCorrFitSmear1DCF : public NicaCorrFitKisiel {
 protected:
  Bool_t fSmeared;
  TH2D *fSmearingMap;
  virtual void Check();
  void RescaleSmearingMap();

 public:
  NicaCorrFitSmear1DCF();
  void LoadSmearingMap(TH2D *map);
  TH2D *GetSmearedMap() const { return fSmearingMap; };
  virtual ~NicaCorrFitSmear1DCF();
  ClassDef(NicaCorrFitSmear1DCF, 1)
};

#endif /* NICACORRFITSMEAR1DCF_H_ */
