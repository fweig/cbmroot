/** @file CbmMuchRecoDefs.h
** @author Florian Uhlig <f.uhlig@gsi.de>
** @date 20.09.19
**
** Header for definition of much reconstruction related constants
** the data was previously in CbmMuchDigitizeGem.h
**/

#ifndef CBMMUCHRECODEFS_H
#define CBMMUCHRECODEFS_H 1

constexpr double sigma_e[] = {4.06815,
                              -0.225699,
                              0.464502,
                              -0.141208,
                              0.0226821,
                              -0.00195697,
                              6.87497e-05};
constexpr double sigma_mu[] =
  {74.5272, -49.7648, 14.4886, -2.23059, 0.188254, -0.00792744, 0.00011976};
constexpr double sigma_p[] =
  {175.879, -15.016, -34.6513, 13.346, -2.08732, 0.153678, -0.00440115};
constexpr double mpv_e[] =
  {14.654, -0.786582, 2.32435, -0.875594, 0.167237, -0.0162335, 0.000616855};
constexpr double mpv_mu[] =
  {660.746, -609.335, 249.011, -55.6658, 7.04607, -0.472135, 0.0129834};
constexpr double mpv_p[] =
  {4152.73, -3123.98, 1010.85, -178.092, 17.8764, -0.963169, 0.0216643};
constexpr double min_logT_e  = -3.21888;
constexpr double min_logT_mu = -0.916291;
constexpr double min_logT_p  = 1.0986;
constexpr double l_e         = 0.47;
constexpr double l_not_e     = 0.36;
#endif
