/*
 * NicaFemtoSourceModelGaus.h
 *
 *  Created on: 1 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOSOURCEMODELGAUSS_H_
#define NICAFEMTOSOURCEMODELGAUSS_H_

#include "NicaFemtoPair.h"
#include "NicaFemtoSourceModel.h"

/**
 * class that generate 1-dim gaussian source
 */
class NicaFemtoSourceModelGauss1D : public NicaFemtoSourceModel1D {
 public:
  NicaFemtoSourceModelGauss1D();
  /**
   * copy ctor
   * @param model
   */
  NicaFemtoSourceModelGauss1D(const NicaFemtoSourceModelGauss1D &model);
  NicaFemtoSourceModel *MakeCopy() const;
  void GenerateCoordinates(NicaFemtoPair *Pair);
  virtual ~NicaFemtoSourceModelGauss1D();
  ClassDef(NicaFemtoSourceModelGauss1D, 1)
};

/**
 * class that generate 3-dimenional gaussian source
 */
class NicaFemtoSourceModelGauss3D : public NicaFemtoSourceModel3D {
 public:
  /**
   * default ctor
   */
  NicaFemtoSourceModelGauss3D();
  /**
   * copy ctor
   * @param model
   */
  NicaFemtoSourceModelGauss3D(const NicaFemtoSourceModelGauss3D &model);
  NicaFemtoSourceModel *MakeCopy() const;
  void GenerateCoordinates(NicaFemtoPair *Pair);
  virtual ~NicaFemtoSourceModelGauss3D();
  ClassDef(NicaFemtoSourceModelGauss3D, 1)
};

#endif /* NICAFEMTOSOURCEMODELGAUSS_H_ */
