/*
 * NicaFemtoYlm.h
 *
 *  Created on: 28 kwi 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 *
 *		Derived from AliROOT AliFemtoYlm
 */
#ifndef NICAFEMTOYLM_H_
#define NICAFEMTOYLM_H_

#include <TObject.h>
#include <complex>

class NicaFemtoYlm : public TObject {
 private:
  static std::complex<double> Ceiphi(double phi);
  static NicaFemtoYlm *fgInstance;
  static double *fgPrefactors;
  static int *fgPrefshift;
  static int *fgPlmshift;
  static std::complex<double> *fgYlms;  //!
 public:
  NicaFemtoYlm();
  virtual ~NicaFemtoYlm();
  static NicaFemtoYlm *Instance();
  NicaFemtoYlm(const NicaFemtoYlm &aYlm);
  NicaFemtoYlm &operator=(const NicaFemtoYlm &aYlm);
  static double Legendre(int ell, int emm, double ctheta);
  static void LegendreUpToYlm(int lmax, double ctheta, double *lbuf);
  static std::complex<double> Ylm(int ell, int m, double theta, double phi);
  static std::complex<double> Ylm(int ell, int m, double x, double y, double z);
  static std::complex<double> *YlmUpToL(int lmax, double x, double y, double z);
  static std::complex<double> *YlmUpToL(int lmax, double ctheta, double phi);
  static double ReYlm(int ell, int m, double theta, double phi);
  static double ReYlm(int ell, int m, double x, double y, double z);
  static double ImYlm(int ell, int m, double theta, double phi);
  static double ImYlm(int ell, int m, double x, double y, double z);
  static void InitializeYlms();
  ClassDef(NicaFemtoYlm, 1)
};

#endif /* NICAROOT_ANALYSIS_FEMTO_BASE_NICAFEMTOYLM_H_ */
