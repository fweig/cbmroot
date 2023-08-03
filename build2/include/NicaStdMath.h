/*
 * NicaStdMath.h
 *
 *  Created on: 16 lip 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICASTDMATH_H_
#define NICASTDMATH_H_
#include <TMath.h>
#include <TVector3.h>

namespace NicaStd {
  /**
   * calculate the parameters of fuction f(x) = a^2 + bx +c
   * @param x1
   * @param x2
   * @param x3
   * @param y1
   * @param y2
   * @param y3
   * @param a
   * @param b
   * @param c
   */
  void FitParabola(Double_t x1,
                   Double_t x2,
                   Double_t x3,
                   Double_t y1,
                   Double_t y2,
                   Double_t y3,
                   Double_t& a,
                   Double_t& b,
                   Double_t& c);
  /**
   * calculate the solution of equation f(x) 0= a^2 + bx + c
   *
   * @param a
   * @param b
   * @param c
   * @param y
   * @param x1 first solution
   * @param y1 second solution
   * @return 0 if no solution 1 if one solution exists, 2 if two solution exists
   */
  Int_t SolveParabola(Double_t a, Double_t b, Double_t c, Double_t& x1, Double_t& x2);
  /**
   * calculate the position of bin when convert from 3D to 1D array
   * @param nbinsX
   * @param nBinsY
   * @param binX
   * @param binY
   * @param binZ
   * @param root_number - set true to convert from TH3D (first bin with number 1)
   * @return
   */
  Int_t Bin3dToBin1d(Int_t nbinsX, Int_t nBinsY, Int_t binX, Int_t binY, Int_t binZ, Bool_t root_number = kTRUE);
  /**
   * calculate the position of bin in 3D histogram when convert from 1D array
   * @param nbinsX
   * @param nBinsY
   * @param binX
   * @param binY
   * @param binZ
   * @param root_number set true to convert in TH3D cooridiantes (first bin with number 1)
   * @return
   */
  TVector3 Bin1dToBin3d(Int_t nbinsX, Int_t nBinsY, Int_t bin, Bool_t root_number = kTRUE);
  /**
   * round given value to the closest step
   * @param points number of points
   * @param min lower edge of values
   * @param max higher edge of values
   * @param val value to round
   * @param type '=' round to the neares, '+' round to the upper edge , '-' round to the lower value in range
   * @return rounded value
   */
  Double_t Discretize(Int_t points, Double_t min, Double_t max, Double_t val, Char_t type);
}  // namespace NicaStd

#endif /* NICAFEMTO_FEATURES_STD_NICASTDMATH_H_ */
