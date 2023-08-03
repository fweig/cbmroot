/*
 * NicaCorrFit3DCFBowlerSinyukovExtra.h
 *
 *  Created on: 31 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFIT3DCFBOWLERSINYUKOVEXTRA_H_
#define NICACORRFIT3DCFBOWLERSINYUKOVEXTRA_H_

#include "NicaCorrFit3DCFBowlerSinyukov.h"

/**
 * for calcuation of radii in eliptical shape - assumes that BS-correction
 * scales eliptical
 */
class NicaCorrFit3DCFBowlerSinyukovEllipse
    : public NicaCorrFit3DCFBowlerSinyukov {
 protected:
  /**
   * calculate "eliptical Rinv"
   * @param xout
   * @param xside
   * @param xlong
   * @param kout
   * @param kside
   * @param klong
   * @return
   */
  Double_t GetRinv(Double_t xout, Double_t xside, Double_t xlong, Double_t kout,
                   Double_t kside, Double_t klong) const;
  virtual Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  NicaCorrFit3DCFBowlerSinyukovEllipse();
  virtual ~NicaCorrFit3DCFBowlerSinyukovEllipse();
  ClassDef(NicaCorrFit3DCFBowlerSinyukovEllipse, 1)
};

class NicaCorrFit3DCFBowlerSinyukovClassic
    : public NicaCorrFit3DCFBowlerSinyukov {
 protected:
  virtual Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  NicaCorrFit3DCFBowlerSinyukovClassic();
  static Int_t RCoul() { return 6; };
  virtual ~NicaCorrFit3DCFBowlerSinyukovClassic();
  ClassDef(NicaCorrFit3DCFBowlerSinyukovClassic, 1)
};

#endif /* NICACORRFIT3DCFBOWLERSINYUKOVEXTRA_H_ */
