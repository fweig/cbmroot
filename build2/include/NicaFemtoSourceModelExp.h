/*
 * NicaFemtoSourceModelExp.h
 *
 *  Created on: 1 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOSOURCEMODELEXP_H_
#define NICAFEMTOSOURCEMODELEXP_H_

#include "NicaFemtoPair.h"
#include "NicaFemtoSourceModel.h"

/**
 * class that generate 1-dim exponential source with r = exp(-r/tau)
 */

class NicaFemtoSourceModelExp1D : public NicaFemtoSourceModel1D {
 public:
  NicaFemtoSourceModelExp1D();
  /**
   * copy ctor
   * @param model
   */
  NicaFemtoSourceModelExp1D(const NicaFemtoSourceModelExp1D &model);
  NicaFemtoSourceModel *MakeCopy() const;
  void GenerateCoordinates(NicaFemtoPair *Pair);
  virtual ~NicaFemtoSourceModelExp1D();
  ClassDef(NicaFemtoSourceModelExp1D, 1)
};

#endif /* NICAFEMTOSOURCEMODELEXP_H_ */
