/*
 * NicaV0CandidateHelix.h
 *
 *  Created on: 26 mar 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAV0CANDIDATEHELIX_H_
#define NICAV0CANDIDATEHELIX_H_

#include "NicaV0Candidate.h"
/**
 * create V0 candidate for experiments with constant magnetic field
 * where helix track can be used
 */
class NicaV0CandidateHelix : public NicaV0Candidate {
public:
  NicaV0CandidateHelix();
  void MakeV0(Double_t mPos, Double_t mNeg, Int_t pidThis, Int_t pidPos, Int_t pidNeg);
  virtual ~NicaV0CandidateHelix();
  ClassDef(NicaV0CandidateHelix, 1)
};

#endif /* NICAV0CANDIDATEHELIX_H_ */
