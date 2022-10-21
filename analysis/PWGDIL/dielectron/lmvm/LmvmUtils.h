/* Copyright (C) 2015-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Semen Lebedev */

#ifndef LMVM_UTILS_H
#define LMVM_UTILS_H

#include "LmvmDef.h"

class CbmKFVertex;
class LmvmCand;
class CbmMCTrack;
class TClonesArray;
class CbmStsTrack;

class LmvmUtils {
public:
  LmvmUtils() { ; }
  virtual ~LmvmUtils() { ; }

  /*
	 * Calculates and set track parameters to LmvmCand.
	 * The following parameters are set: fChi2sts, fChi2Prim, fPosition, fMomentum, fMass, fCharge, fEnergy, fRapidity
	 */
  static void CalculateAndSetTrackParams(LmvmCand* cand, CbmStsTrack* stsTrack, CbmKFVertex& kfVertex);

  /*
	 * Armenteros - Podolansky plot
	 */
  static void CalculateArmPodParams(LmvmCand* cand1, LmvmCand* cand2, double& alpha, double& ptt);

  static ELmvmSrc GetMcSrc(CbmMCTrack* mctrack, TClonesArray* mcTracks);

  /*
	 * \brief Return true if MC track is signal primary electron.
	 */
  static bool IsMcSignalEl(const CbmMCTrack* mct);

  /*
	 * \brief Return true if MC track is electron from gamma conversion.
	 */
  static bool IsMcGammaEl(const CbmMCTrack* mct, TClonesArray* mcTracks);

  /*
	 * \brief Return true if MC track is electron from Pi0 dalitz decay.
	 */
  static bool IsMcPi0El(const CbmMCTrack* mct, TClonesArray* mcTracks);

  /*
	 * \brief Return true if MC track is electron from Eta decay.
	 */
  static bool IsMcEtaEl(const CbmMCTrack* mct, TClonesArray* mcTracks);

  static bool IsMcPairSignal(const CbmMCTrack* mctP, const CbmMCTrack* mctM);

  static bool IsMcPairPi0(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks);

  static bool IsMcPairEta(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks);

  static bool IsMcPairGamma(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks);

  static bool IsMcPairBg(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks);

  static ELmvmSrc GetMcPairSrc(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks);


  static bool IsMcPairSignal(const LmvmCand& candP, const LmvmCand& candM);

  static bool IsMcPairPi0(const LmvmCand& candP, const LmvmCand& candM);

  static bool IsMcPairEta(const LmvmCand& candP, const LmvmCand& candM);

  static bool IsMcPairGamma(const LmvmCand& candP, const LmvmCand& candM);

  static bool IsMcPairBg(const LmvmCand& candP, const LmvmCand& candM);

  static ELmvmSrc GetMcPairSrc(const LmvmCand& candP, const LmvmCand& candM);

  static ELmvmBgPairSrc GetBgPairSrc(const LmvmCand& candP, const LmvmCand& candM);

  static bool IsMismatch(const LmvmCand& cand);

  static bool IsGhost(const LmvmCand& cand);

  static double Distance(double x1, double y1, double x2, double y2);

  static double Distance2(double x1, double y1, double x2, double y2);

  static void IsElectron(int globalTrackIndex, double momentum, double momentumCut, LmvmCand* cand);

  static void IsRichElectron(int globalTrackIndex, double momentum, LmvmCand* cand);

  static void IsTrdElectron(int globalTrackIndex, double momentum, LmvmCand* cand);

  static void IsTofElectron(int globalTrackIndex, double momentum, LmvmCand* cand);

  static void IsElectronMc(LmvmCand* cand, TClonesArray* mcTracks, double pionMisidLevel);

  static std::string GetChargeStr(const LmvmCand* cand);

  static std::string GetChargeStr(const CbmMCTrack* mct);
  
  static double GetMassScaleInmed(double minv);
  static double GetMassScaleQgp(double minv);  
 
  ClassDef(LmvmUtils, 1);  

private: 
 
  static constexpr double fMinvArray[170] = {
        0.0195, 0.0395, 0.0595, 0.0795, 0.0995, 0.1195, 0.1395, 0.1595, 0.1795, 0.1995, 0.2195, 0.2395, 0.2595, 0.2795,
        0.2995, 0.3195, 0.3395, 0.3595, 0.3795, 0.3995, 0.4195, 0.4395, 0.4595, 0.4795, 0.4995, 0.5195, 0.5395, 0.5595,
        0.5795, 0.5995, 0.6195, 0.6395, 0.6595, 0.6795, 0.6995, 0.7195, 0.7395, 0.7595, 0.7795, 0.7995, 0.8195, 0.8395,
        0.8595, 0.8795, 0.8995, 0.9195, 0.9395, 0.9595, 0.9795, 0.9995, 1.0195, 1.0395, 1.0595, 1.0795, 1.0995, 1.1195,
        1.1395, 1.1595, 1.1795, 1.1995, 1.2195, 1.2395, 1.2595, 1.2795, 1.2995, 1.3195, 1.3395, 1.3595, 1.3795, 1.3995,
        1.4195, 1.4395, 1.4595, 1.4795, 1.4995, 1.5195, 1.5395, 1.5595, 1.5795, 1.5995, 1.6195, 1.6395, 1.6595, 1.6795,
        1.6995, 1.7195, 1.7395, 1.7595, 1.7795, 1.7995, 1.8195, 1.8395, 1.8595, 1.8795, 1.8995, 1.9195, 1.9395, 1.9595,
        1.9795, 1.9995, 2.0195, 2.0395, 2.0595, 2.0795, 2.0995, 2.1195, 2.1395, 2.1595, 2.1795, 2.1995, 2.2195, 2.2395,
        2.2595, 2.2795, 2.2995, 2.3195, 2.3395, 2.3595, 2.3795, 2.3995, 2.4195, 2.4395, 2.4595, 2.4795, 2.4995, 2.5195,
        2.5395, 2.5595, 2.5795, 2.5995, 2.6195, 2.6395, 2.6595, 2.6795, 2.6995, 2.7195, 2.7395, 2.7595, 2.7795, 2.7995,
        2.8195, 2.8395, 2.8595, 2.8795, 2.8995, 2.9195, 2.9395, 2.9595, 2.9795, 2.9995, 3.0195, 3.0395, 3.0595, 3.0795,
        3.0995, 3.1195, 3.1395, 3.1595, 3.1795, 3.1995, 3.2195, 3.2395, 3.2595, 3.2795, 3.2995, 3.3195, 3.3395, 3.3595,
        3.3795, 3.3995};

  static constexpr double fScaleArrayInmed[170] = { // for 12 AGeV
        41.706,     18.918,     11.465,     8.4388,     5.9176,     4.9025,     3.8087,     3.0387,     2.5856,
        2.1142,     1.7603,     1.5327,     1.28,       1.1579,     1.0367,     0.89355,    0.81317,    0.71582,
        0.65863,    0.59678,    0.53702,    0.45378,    0.41238,    0.37502,    0.33593,    0.28791,    0.26352,
        0.23939,    0.21167,    0.19479,    0.19204,    0.17492,    0.15811,    0.15479,    0.14935,    0.13803,
        0.1354,     0.11993,    0.1046,     0.08226,    0.073183,   0.055433,   0.043467,   0.033975,   0.028025,
        0.021504,   0.016863,   0.014108,   0.01094,    0.0088095,  0.007324,   0.0057162,  0.0046817,  0.0037459,
        0.0030017,  0.0024459,  0.0020671,  0.0016089,  0.0013754,  0.0011223,  0.00096256, 0.00081647, 0.00072656,
        0.00060776, 0.00051243, 0.00045705, 0.00039636, 0.00036259, 0.00033248, 0.0002953,  0.00027328, 0.00023776,
        0.00022163, 0.00019852, 0.000186,   0.00016846, 0.00015469, 0.00014169, 0.00013343, 0.00011594, 0.00010722,
        0.00010205, 9.1907e-05, 8.3718e-05, 7.5457e-05, 6.7192e-05, 6.2202e-05, 5.7372e-05, 4.8314e-05, 4.5502e-05,
        4.1334e-05, 3.7429e-05, 3.2131e-05, 3.0103e-05, 2.6125e-05, 2.3601e-05, 2.1167e-05, 1.94e-05,   1.7025e-05,
        1.5496e-05, 1.3704e-05, 1.1866e-05, 1.1135e-05, 9.8842e-06, 8.9101e-06, 7.9225e-06, 7.0706e-06, 6.3536e-06,
        5.3786e-06, 4.7179e-06, 4.2128e-06, 4.0015e-06, 3.4118e-06, 3.1864e-06, 2.734e-06,  2.3844e-06, 2.173e-06,
        1.8774e-06, 1.6468e-06, 1.501e-06,  1.3597e-06, 1.2113e-06, 1.0384e-06, 9.4105e-07, 8.4223e-07, 7.434e-07,
        6.5049e-07, 5.8824e-07, 5.3603e-07, 4.6756e-07, 4.1173e-07, 3.5872e-07, 3.2764e-07, 2.9889e-07, 2.5989e-07,
        2.219e-07,  1.9468e-07, 1.816e-07,  1.5707e-07, 1.3565e-07, 1.2619e-07, 1.0919e-07, 1.0071e-07, 8.4632e-08,
        7.6459e-08, 6.829e-08,  6.2046e-08, 5.5335e-08, 4.5937e-08, 4.2426e-08, 3.567e-08,  3.4051e-08, 2.9627e-08,
        2.5249e-08, 2.2767e-08, 2.1054e-08, 1.7873e-08, 1.574e-08,  1.3713e-08, 1.23e-08,   1.1045e-08, 9.5536e-09,
        8.5859e-09, 7.7217e-09, 6.9958e-09, 6.0992e-09, 5.3453e-09, 4.7659e-09, 4.3313e-09, 3.6575e-09};

  static constexpr double fScaleArrayQgp[170] = { // for 12 AGeV
        39.496,     17.961,     11.024,     8.2093,     5.8331,     4.8995,     3.8612,     3.1258,     2.7006,
        2.2465,     1.908,      1.699,      1.4435,     1.3253,     1.2059,     1.049,      0.96753,    0.86685,
        0.81407,    0.75959,    0.70663,    0.61951,    0.58586,    0.55534,    0.51902,    0.46377,    0.4415,
        0.41412,    0.37414,    0.34883,    0.34494,    0.31141,    0.2762,     0.26331,    0.24693,    0.22286,
        0.21697,    0.1972,     0.1841,     0.16097,    0.16352,    0.14345,    0.13096,    0.11911,    0.11399,
        0.10111,    0.0913,     0.08764,    0.077745,   0.071417,   0.067561,   0.05987,    0.055543,   0.050193,
        0.045244,   0.04128,    0.03898,    0.03365,    0.031622,   0.028217,   0.026215,   0.023919,   0.022648,
        0.019915,   0.017524,   0.016145,   0.014357,   0.013362,   0.012368,   0.011036,   0.010198,   0.0088275,
        0.0081762,  0.0072697,  0.00675,    0.0060424,  0.0054788,  0.0049588,  0.0046174,  0.0039685,  0.00363,
        0.0034204,  0.0030534,  0.0027606,  0.0024723,  0.0021893,  0.0020174,  0.0018545,  0.0015584,  0.0014661,
        0.0013315,  0.0012065,  0.0010375,  0.00097456, 0.00084865, 0.00076982, 0.00069371, 0.00063931, 0.00056442,
        0.00051712, 0.00046054, 0.00040174, 0.00037996, 0.00034009, 0.00030921, 0.00027738, 0.00024981, 0.00022659,
        0.00019366, 0.00017153, 0.00015469, 0.00014841, 0.00012783, 0.00012061, 0.00010456, 9.2145e-05, 8.4856e-05,
        7.4087e-05, 6.5675e-05, 6.0496e-05, 5.5386e-05, 4.9865e-05, 4.3202e-05, 3.9571e-05, 3.5821e-05, 3.201e-05,
        2.8322e-05, 2.5886e-05, 2.384e-05,  2.1016e-05, 1.8703e-05, 1.6467e-05, 1.5199e-05, 1.4011e-05, 1.2311e-05,
        1.0621e-05, 9.4155e-06, 8.874e-06,  7.7548e-06, 6.7662e-06, 6.3589e-06, 5.5585e-06, 5.1791e-06, 4.3965e-06,
        4.012e-06,  3.6195e-06, 3.3215e-06, 2.9918e-06, 2.5084e-06, 2.3397e-06, 1.9865e-06, 1.915e-06,  1.6826e-06,
        1.448e-06,  1.3183e-06, 1.231e-06,  1.0551e-06, 9.3811e-07, 8.2511e-07, 7.4714e-07, 6.7735e-07, 5.9142e-07,
        5.3654e-07, 4.8709e-07, 4.4543e-07, 3.9199e-07, 3.4674e-07, 3.1203e-07, 2.862e-07,  2.4391e-07};

};

#endif
