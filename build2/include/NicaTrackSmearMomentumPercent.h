/*
 * NicaTrackSmearMomentumPercent.h
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKSMEARMOMENTUMPERCENT_H_
#define NICATRACKSMEARMOMENTUMPERCENT_H_

#include <FairTask.h>

#include "NicaSmearAlgorithm.h"

/**
 * class for smearing momenta of track
 */
class NicaTrackSmearMomentumPercent : public NicaTrackSmear {
  Double_t fPxSmear;
  Double_t fPySmear;
  Double_t fPzSmear;
  Double_t fPtSmear;
  Int_t fSmearMode;

 public:
  NicaTrackSmearMomentumPercent();
  /**
   * copy constructor
   * @param obj
   */
  NicaTrackSmearMomentumPercent(const NicaTrackSmearMomentumPercent& obj);
  /**
   * set momenum smear  only px an py are smeared
   * @param pt transverse momentum smear in percent
   */
  void SetMomentumSmear(Double_t pt);
  /**
   * set momentum smear
   * @param pt smear on pt (in percent)
   * @param pz smear on pz (in percent)
   */
  void SetMomentumSmear(Double_t pt, Double_t pz);
  /**
   * set momentu smear
   * @param px px smear on pt (in percent)
   * @param py py smear on pt (in percent)
   * @param pz pt smear on pt (in percent)
   */
  void SetMomentumSmear(Double_t px, Double_t py, Double_t pz);
  InitStatus Init();
  void ModifyTrack(NicaSmearedTrack* mod);
  virtual NicaSmearAlgorithm* MakeCopy() const {
    return new NicaTrackSmearMomentumPercent(*this);
  };
  virtual NicaPackage* Report() const;
  virtual ~NicaTrackSmearMomentumPercent();
  ClassDef(NicaTrackSmearMomentumPercent, 1)
};

#endif /* NICATRACKSMEARMOMENTUMPERCENT_H_ */
