/*
 * NicaFemtoFreezoutsAna.h
 *
 *  Created on: 15-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOFREEZOUTSANA_H_
#define NICAFEMTOFREEZOUTSANA_H_

#define SIMPLIFIED_PRF_COMPUTING_IN_FREEZOUT
#include <FairTask.h>
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH3.h>

#include "NicaCut.h"
#include "NicaFemtoConst.h"
#include "NicaFemtoFastCut.h"
#include "NicaFemtoFreezoutGenerator.h"
#include "NicaHistogramManager.h"
#include "NicaTwoTrackAna.h"

#include "NicaFemtoBasicAna.h"

/**
 * basic class for obtaint source emission function
 */
class NicaFemtoFreezoutsAna : public NicaTwoTrackAna {
  /**
   * assumed masses of particles (if <0 then value from data format is used)
   */
  Int_t fPdg1, fPdg2;
  Double_t fX, fY, fZ, fT, fCut;
  NicaFemtoFastCut* fFastCut;
  ENicaFemtoKinematics fKinematicsMode;
  Bool_t fIgnoreSign;
  Bool_t fUseFakeMomenta;
  NicaFemtoPair* fFemtoPair;
  Int_t fBins[3];
  Double_t fHistoMin[3];
  Double_t fHistoMax[3];
  NicaFemtoFreezoutGenerator* fFreezoutGenerator;
  NicaHistogramManager_3_3D<TH3F>* fHistograms;
  void ComputePRF();
  void ComputeLCMS();
  void ComputePRFL();

protected:
  virtual void ProcessFemtoPair();
  virtual void PreprocessFemtoPair();
  virtual void ProcessPair();
  virtual void ProcessPair2() { ProcessPair(); };
  virtual NicaPackage* Report() const;
  virtual InitStatus Init();

public:
  NicaFemtoFreezoutsAna();
  /**
   * cop constructor
   * @param ana
   */
  NicaFemtoFreezoutsAna(const NicaFemtoFreezoutsAna& ana);
  /**
   * set PID (if no MC mass/energy is avaiable
   * @param pdg
   */
  void SetPdg(Int_t pdg) { fPdg1 = pdg; };
  /**
   * set PID (if no MC mass/energy is avaiable for  nonidentical pairs
   * @param pdg
   */
  void SetPdg(Int_t pdg1, Int_t pdg2) {
    fPdg1 = pdg1;
    fPdg2 = pdg2;
  };
  void SetOption(Option_t* opt);
  /**
   * set cut on momentum difference particles (k* in prf or qinv in lcms frame)
   * @param cut momentu cut
   */
  void SetMomentumCut(Double_t cut);
  void IgnoreSign() { fIgnoreSign = kTRUE; };
  void SetFrameLCMS() { SetOption("lcms"); };
  void SetFramePRF() { SetOption("prf"); };
  void SetAxes(Int_t bins, Double_t min, Double_t max);
  void SetOutAxis(Int_t bins, Double_t min, Double_t max);
  void SetSideAxis(Int_t bins, Double_t min, Double_t max);
  void SetLongAxis(Int_t bins, Double_t min, Double_t max);
  virtual void AddCut(const NicaCut& cut, Option_t* opt = " ");
  /**
   * set freezout generator (this is optional)
   * @param freez
   */
  void SetFreezoutGenerator(const NicaFemtoFreezoutGenerator& freez) { fFreezoutGenerator = freez.MakeCopy(); };
  void SetFastPairCut(const NicaFemtoFastCut& cut) { fFastCut = cut.MakeCopy(); };
  virtual ~NicaFemtoFreezoutsAna();
  ClassDef(NicaFemtoFreezoutsAna, 1)
};

#endif /* NICAFEMTOFREEZOUTSANA_H_ */
