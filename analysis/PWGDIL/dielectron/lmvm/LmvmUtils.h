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

  static void IsElectronMc(LmvmCand* cand, TClonesArray* mcTracks, double pionMisidLevel);

  static std::string GetChargeStr(const LmvmCand* cand);

  static std::string GetChargeStr(const CbmMCTrack* mct);

  ClassDef(LmvmUtils, 1);
};

#endif
