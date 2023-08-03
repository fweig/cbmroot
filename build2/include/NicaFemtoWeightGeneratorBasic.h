/*
 * NicaFemtoWeightGeneratorBasic.h
 *  derived from ALICE AliFemtoWeightGeneratorBasic
 *  Created on: 21-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAFEMTOWEIGHTGENERATORBASIC_H_
#define NICAFEMTOWEIGHTGENERATORBASIC_H_

#include "NicaFemtoWeightGenerator.h"
/**
 * basic weight generator that take into account only quantum statistic, code
 * derived form AliROOT
 */
class NicaFemtoWeightGeneratorBasic : public NicaFemtoWeightGenerator {
 private:
  NicaFemtoWeightGenerator* GetGenerator() const;

 public:
  /**
   * default ctor
   */
  NicaFemtoWeightGeneratorBasic();
  /**
   * copy ctor
   * @param aModel object to copy
   */
  NicaFemtoWeightGeneratorBasic(const NicaFemtoWeightGeneratorBasic& aModel);
  /**
   * operator =
   * @param aModel
   * @return
   */
  NicaFemtoWeightGeneratorBasic& operator=(
      const NicaFemtoWeightGeneratorBasic& aModel);
  virtual Double_t GenerateWeight(NicaFemtoPair* track);
  virtual NicaFemtoWeightGeneratorBasic* MakeCopy() const {
    return new NicaFemtoWeightGeneratorBasic(*this);
  };
  virtual NicaFemtoWeightGenerator* Clone(const char* newame = "") const;
  virtual NicaPackage* Report() const;
  virtual ~NicaFemtoWeightGeneratorBasic();
  ClassDef(NicaFemtoWeightGeneratorBasic, 1)
};
#endif /* NICAFEMTOWEIGHTGENERATORBASIC_H_ */
