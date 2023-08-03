/*
 * NicaFemtoSourceModelGausKt.h
 *
 *  Created on: 1 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOSOURCEMODELGAUSMT_H_
#define NICAFEMTOSOURCEMODELGAUSMT_H_

#include "NicaFemtoPair.h"
#include "NicaFemtoSourceModel.h"

class NicaFemtoSourceModelGausMt : public NicaFemtoSourceModel3D {
 public:
  NicaFemtoSourceModelGausMt();
  /**
   * copy ctor
   * @param model
   */
  NicaFemtoSourceModelGausMt(const NicaFemtoSourceModelGausMt& model);
  void SetOutParams(Double_t a, Double_t b);
  void SetSideParams(Double_t a, Double_t b);
  void SetLongParams(Double_t a, Double_t b);
  void SetM2(Double_t par);
  virtual NicaFemtoSourceModel* MakeCopy() const;
  virtual void GenerateCoordinates(NicaFemtoPair* Pair);
  virtual NicaPackage* Report() const;
  virtual ~NicaFemtoSourceModelGausMt();
  ClassDef(NicaFemtoSourceModelGausMt, 1)
};

#endif /* NICAFEMTOSOURCEMODELGAUSMT_H_ */
