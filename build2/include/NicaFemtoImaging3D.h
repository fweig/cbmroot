/*
 * NicaFemtoImaging3D.h
 *
 *  Created on: 21 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOIMAGING3D_H_
#define NICAFEMTOIMAGING3D_H_

#include "NicaFemto3DCF.h"
#include "NicaFemtoImaging.h"

class NicaFemtoImaging3D {
  Double_t fQMax[3];
  Double_t fQMin[3];
  Double_t fRMax[3];
  Double_t fRMin[3];
  Int_t fBins[3];
  ENicaFemtoKinematics fKinematics;
  NicaFemto3DCF *fCF;
  TH3D *fR;
  TH3D *fSource;
  /**
   * check if required resolution can be achieved with current data, and create
   * histogram with source function
   */
  virtual void Init();
  /**
   * calculate inversed transform if fast then only partial inversion is
   * computed (see InverseFast)
   */
  virtual void InverseTransofrm(Option_t *opt = "");

 public:
  NicaFemtoImaging3D();
  /**
   * inverse correlation function and return source image
   * @param cf
   * @param opt if "fast" then fast inversion is made (partial inversion)
   */
  TH3D *Inverse(NicaFemto3DCF *cf, Option_t *opt = "");
  /**
   *
   * @param bins bin numer
   * @param min minum value
   * @param max maximum value
   * @param opt "x" for out, "y" for side, "z" for long
   */
  void SetRAxis(Int_t bins, Double_t min, Double_t max, Option_t *opt);
  /**
   * set maximu momentum difference used form imaging
   * @param qmax
   * @param opt
   */
  void SetQMax(Double_t qmax, Option_t *opt);
  void SetQMin(Double_t qmin, Option_t *opt);
  virtual ~NicaFemtoImaging3D();
};

#endif /* NICAFEMTOIMAGING3D_H_ */
