/*
 * NicaFemtoFreezoutGenerator.h
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAFEMTOFREEZOUTGENERATOR_H_
#define NICAFEMTOFREEZOUTGENERATOR_H_

#include "NicaFemtoSourceModel.h"

/**
 * class for creating given coordinates in lab frame
 */
class NicaFemtoFreezoutGenerator : public TObject {
protected:
  /**
   * model that generate coordinates in pair frame
   */
  NicaFemtoSourceModel* fModel;
  /**
   * boost coordinates from model to lab and set them to pair
   * @param pair pair than need coordinates
   */
  virtual void Boost(NicaFemtoPair* pair) = 0;

public:
  NicaFemtoFreezoutGenerator();
  NicaFemtoFreezoutGenerator(const NicaFemtoSourceModel& source);
  /**
   * copy ctor
   * @param generator
   */
  NicaFemtoFreezoutGenerator(const NicaFemtoFreezoutGenerator& generator);
  /**
   * make copy of this object
   * @return
   */
  virtual NicaFemtoFreezoutGenerator* MakeCopy() const = 0;
  /**
   * defalut initializer
   */
  void Init();
  /**
   *
   * @param model used source model
   */
  void SetSourceModel(const NicaFemtoSourceModel& model);
  /**
   *
   * @return source model if present
   */
  NicaFemtoSourceModel* GetSourceModel() const { return fModel; };
  /**
   * generate freezout coordinates and set them to pair
   * @param Pair
   */
  void GenerateFreezoutCooordinates(NicaFemtoPair* Pair);
  virtual void Print(Option_t* option = "") const;
  /**
   * create package with report
   * @return package with report
   */
  virtual NicaPackage* Report() const;
  virtual ~NicaFemtoFreezoutGenerator();
  ClassDef(NicaFemtoFreezoutGenerator, 1)
};
/**
 * used to boost source according to LCMS frame
 */
class NicaFemtoFreezoutGeneratorLCMS : public NicaFemtoFreezoutGenerator {
protected:
  void Boost(NicaFemtoPair* pair);

public:
  NicaFemtoFreezoutGeneratorLCMS();
  NicaFemtoFreezoutGeneratorLCMS(const NicaFemtoSourceModel& model);
  /**
   * copy ctor
   * @param generator
   */
  NicaFemtoFreezoutGeneratorLCMS(const NicaFemtoFreezoutGeneratorLCMS& generator);
  NicaFemtoFreezoutGenerator* MakeCopy() const;
  virtual NicaPackage* Report() const;
  virtual ~NicaFemtoFreezoutGeneratorLCMS();
  ClassDef(NicaFemtoFreezoutGeneratorLCMS, 1)
};

/**
 * used to boost source according to PRF frame
 */
class NicaFemtoFreezoutGeneratorPRF : public NicaFemtoFreezoutGenerator {
protected:
  void Boost(NicaFemtoPair* pair);

public:
  NicaFemtoFreezoutGeneratorPRF();
  /**
   * copy ctor
   * @param generator
   */
  NicaFemtoFreezoutGeneratorPRF(const NicaFemtoFreezoutGeneratorPRF& generator);
  NicaFemtoFreezoutGeneratorPRF(const NicaFemtoSourceModel& model);
  NicaFemtoFreezoutGenerator* MakeCopy() const;
  virtual NicaPackage* Report() const;
  virtual ~NicaFemtoFreezoutGeneratorPRF();
  ClassDef(NicaFemtoFreezoutGeneratorPRF, 1)
};

#endif /* NICAFEMTOFREEZOUTGENERATOR_H_ */
