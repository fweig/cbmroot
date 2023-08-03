/*
 * NicaFemtoWeightSupressed.h
 *
 *  Created on: 22 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOWEIGHTGENERATORSUPRESSED_H_
#define NICAFEMTOWEIGHTGENERATORSUPRESSED_H_

#include "NicaFemtoWeightGenerator.h"

/**
 * class for setting weight with supression factor (1-lambda)
 */
class NicaFemtoWeightGeneratorSupressed : public NicaFemtoWeightGenerator {
  NicaFemtoWeightGenerator *fWeightGenerator;
  Double_t fSupressionFactor;

 public:
  NicaFemtoWeightGeneratorSupressed()
      : fWeightGenerator(nullptr), fSupressionFactor(0.0){};
  /**
   * default c-tor
   * @param weight - code for real weight calculation
   */
  NicaFemtoWeightGeneratorSupressed(const NicaFemtoWeightGenerator &weight);
  /**
   * copy constructor
   * @param other
   */
  NicaFemtoWeightGeneratorSupressed(
      const NicaFemtoWeightGeneratorSupressed &other);
  /**
   * set supression factor (1-lambda)
   * @param factor
   */
  void SetSupressionFactor(Double_t factor);
  virtual Double_t GenerateWeight(NicaFemtoPair *pair);
  NicaFemtoWeightGeneratorSupressed &operator=(
      const NicaFemtoWeightGeneratorSupressed &other);
  virtual NicaFemtoWeightGeneratorSupressed *MakeCopy() const {
    return new NicaFemtoWeightGeneratorSupressed(*this);
  };
  virtual NicaPackage *Report() const;
  virtual ~NicaFemtoWeightGeneratorSupressed();
  ClassDef(NicaFemtoWeightGeneratorSupressed, 1)
};

#endif /* NICAFEMTOWEIGHTGENERATORSUPRESSED_H_ */
