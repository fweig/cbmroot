/*
 * NicaV0Candidate.h
 *
 *  Created on: 26 mar 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAV0CANDIDATE_H_
#define NICAV0CANDIDATE_H_

#include "NicaTwoTrack.h"
#include "NicaV0Track.h"
/**
 * abstract class for V0 candidate
 */
class NicaV0Candidate : public NicaTwoTrack {
protected:
  NicaV0Track fV0Track;

public:
  NicaV0Candidate();
  /**
   * make NicaV0Track from this class
   * @param mPos mass of positive daughter
   * @param mNeg mass of negative daughter
   */
  virtual void MakeV0(Double_t mPos, Double_t mNeg, Int_t pidThis, Int_t pidPos, Int_t pidNeg) = 0;
  /**
   *
   * @return reference to V0Track
   */
  const NicaV0Track& GetV0Track() { return fV0Track; };
  virtual ~NicaV0Candidate();
  ClassDef(NicaV0Candidate, 1)
};
#endif /* NICAV0CANDIDATE_H_ */
