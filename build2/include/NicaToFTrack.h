/*
 * NicaTrackToF.h
 *
 *  Created on: 28 mar 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATOFTRACK_H_
#define NICATOFTRACK_H_

#include "NicaDetectorTrack.h"
/**
 * class for representation of particle registered by Time of Flight detector
 */
class NicaToFTrack : public NicaDetectorTrack {
  Int_t fToFNHits;
  Int_t fToFFlag;
  Float_t fToFTime;
  Float_t fToFMass2;
  Float_t fToFBeta;
  Float_t fToFEnergy;

 public:
  NicaToFTrack();
  /**
   *
   * @return value of beta . mass2, energyfor dummy partile
   */
  static Float_t DummyVal() { return -1E+6; };
  /**
   *
   * @return number of hits in ToF
   */
  inline Int_t GetNHits() const { return fToFNHits; };
  /**
   *
   * @return ToF flag
   */
  inline Int_t GetFlag() const { return fToFFlag; };
  /**
   *
   * @return time of flight
   */
  inline Float_t GetTime() const { return fToFTime; };
  /**
   *
   * @return square of mass (should it be negative number for particles that
   * don't have ToF id?)
   */
  inline Float_t GetMass2() const { return fToFMass2; };
  /**
   *
   * @return beta in ToF
   */
  inline Float_t GetBeta() const { return fToFBeta; }
  /**
   *
   * @return energy from ToF
   */
  inline Float_t GetEnergy() const { return fToFEnergy; };
  /**
   *
   * @param nToFHits set number of ToF hits
   */
  void SetNHits(Int_t nToFHits) { fToFNHits = nToFHits; }
  /**
   *
   * @param tofFlag set ToF flag
   */
  void SetFlag(Int_t tofFlag) { fToFFlag = tofFlag; };
  /**
   *
   * @param toFBeta set ToF beta
   */
  void SetBeta(Float_t toFBeta) { fToFBeta = toFBeta; }
  /**
   *
   * @param toFEnergy set ToF energy
   */
  void SetEnergy(Float_t toFEnergy) { fToFEnergy = toFEnergy; }
  /**
   *
   * @param toFMass2 set tof mass2
   */
  void SetMass2(Float_t toFMass2) { fToFMass2 = toFMass2; }
  /**
   *
   * @param toFTime set time of flight
   */
  void SetTime(Float_t toFTime) { fToFTime = toFTime; }
  virtual ~NicaToFTrack();
  ClassDef(NicaToFTrack, 1)
};

#endif /* NICATOFTRACK_H_ */
