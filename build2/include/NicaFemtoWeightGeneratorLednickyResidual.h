/*
 * NicaFemtoWeightGeneratorLednickyResidual.h
 *
 *  Created on: 2 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOWEIGHTGENERATORLEDNICKYRESIDUAL_H_
#define NICAFEMTOWEIGHTGENERATORLEDNICKYRESIDUAL_H_

#include <Rtypes.h>
#include <RtypesCore.h>

#include "NicaFemtoWeightGeneratorLednicky.h"

class NicaTrack;
class NicaTwoTrack;

class NicaFemtoWeightGeneratorLednickyResidual
    : public NicaFemtoWeightGeneratorLednicky {
  NicaTrack *fParentTrack1;
  NicaTrack *fParentTrack2;
  NicaFemtoPair *fPairResid;
  NicaTwoTrack *fTrackPair;

 public:
  NicaFemtoWeightGeneratorLednickyResidual();
  NicaFemtoWeightGeneratorLednickyResidual(
      const NicaFemtoWeightGeneratorLednickyResidual &other);
  virtual Double_t GenerateWeight(NicaFemtoPair *pair);
  virtual void Init(Int_t task_id);
  virtual NicaFemtoWeightGeneratorLednickyResidual *MakeCopy() const {
    return new NicaFemtoWeightGeneratorLednickyResidual(*this);
  }
  virtual ~NicaFemtoWeightGeneratorLednickyResidual();
  ClassDef(NicaFemtoWeightGeneratorLednickyResidual, 1)
};

#endif /* NICAFEMTOWEIGHTGENERATORLEDNICKYRESIDUAL_H_ */
