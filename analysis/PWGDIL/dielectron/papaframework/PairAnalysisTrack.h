/* Copyright (C) 2015-2019 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Julian Book [committer] */

#ifndef PAIRANALYSISTRACK_H
#define PAIRANALYSISTRACK_H

//#############################################################
//#                                                           #
//#         Class PairAnalysisTrack                             #
//#         Class for management of the gobal track           #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include "CbmDefs.h"

#include <TLorentzVector.h>
#include <TNamed.h>
//#include "CbmTrackMatchNew.h"

class FairTrackParam;
class CbmTrack;
class CbmGlobalTrack;
class CbmStsTrack;
class CbmMuchTrack;
class CbmTrdTrack;
class CbmRichRing;
class CbmTofHit;
class CbmMCTrack;
class CbmTrackMatchNew;
class CbmKFVertex;

class TParticle;
//class TObjArray;
//class TProcessID;
//class FairRootManager;

class PairAnalysisTrack : public TNamed {
public:
  PairAnalysisTrack();
  PairAnalysisTrack(const char* name, const char* title);
  PairAnalysisTrack(CbmKFVertex* vtx, CbmGlobalTrack* gtrk, CbmStsTrack* ststrk, CbmMuchTrack* muchtrk,
                    CbmTrdTrack* trdtrk, CbmRichRing* richring, CbmTofHit* tofhit, CbmMCTrack* mctrk,
                    CbmTrackMatchNew* stsmatch, CbmTrackMatchNew* muchmatch, CbmTrackMatchNew* trdMatch,
                    CbmTrackMatchNew* richMatch, FairTrackParam* richproj, Int_t gIndex);
  PairAnalysisTrack(TParticle* fastTrk, CbmMCTrack* mctrk);

  virtual ~PairAnalysisTrack();
  PairAnalysisTrack(const PairAnalysisTrack& track);

  // setter
  void SetPdgCode(Int_t pdg) { fPdgCode = pdg; }
  void SetLabel(Int_t lbl) { fLabel = lbl; }
  void SetWeight(Double_t wght) { fWeight = wght; }
  void SetMassHypo(Int_t pdg1, Int_t pdg2, Bool_t refitMassAssump);

  // track getters
  TLorentzVector* GetPosition() { return &fPosition; }
  TLorentzVector* GetMomentum() { return &fMomentum; }
  CbmGlobalTrack* GetGlobalTrack() const { return fGlblTrack; }
  Int_t GetGlobalIndex() const { return fGlblTrackIndex; }
  CbmStsTrack* GetStsTrack() const { return fStsTrack; }
  CbmMuchTrack* GetMuchTrack() const { return fMuchTrack; }
  CbmTrdTrack* GetTrdTrack() const { return fTrdTrack; }
  CbmRichRing* GetRichRing() const { return fRichRing; }
  CbmTofHit* GetTofHit() const { return fTofHit; }
  CbmTrack* GetTrack(ECbmModuleId det) const;

  FairTrackParam* GetRichProj() const { return fRichProj; }
  FairTrackParam* GetMvdEntrance() const { return fMvdEntrance; }
  // mc track
  CbmMCTrack* GetMCTrack() const { return fMCTrack; }
  CbmTrackMatchNew* GetTrackMatch(ECbmModuleId det) const;

  CbmKFVertex* GetPrimaryKFVertex() const { return fPrimVertex; }
  // default kinematics
  Double_t Px() const { return fMomentum.Px(); }
  Double_t Py() const { return fMomentum.Py(); }
  Double_t Pz() const { return fMomentum.Pz(); }
  Double_t Pt() const { return fMomentum.Pt(); }
  Double_t P() const { return fMomentum.P(); }

  Double_t Xv() const { return fPosition.X(); }
  Double_t Yv() const { return fPosition.Y(); }
  Double_t Zv() const { return fPosition.Z(); }

  Double_t OneOverPt() const { return 1. / fMomentum.Pt(); }
  Double_t Phi() const { return fMomentum.Phi(); }
  Double_t Theta() const { return fMomentum.Theta(); }

  Double_t E() const { return fMomentum.Energy(); }
  Double_t M() const { return fMomentum.M(); }

  //  Double_t Eta()        const { return fMomentum.Eta(); }
  Double_t Y() const { return fMomentum.Rapidity(); }

  Double_t ChiToVertex() const { return fChi2Vtx; }
  Short_t Charge() const { return fCharge; }
  Int_t PdgCode() const { return fPdgCode; }
  Int_t GetLabel() const { return fLabel; }
  Double_t GetWeight() const { return fWeight; }

private:
  void Refit(Int_t pidHypo);
  void CalculateChi2Vtx();

  CbmKFVertex* fPrimVertex   = NULL;  // primary vertex
  CbmGlobalTrack* fGlblTrack = NULL;  // global track
  Int_t fGlblTrackIndex      = -1;    // global track
  CbmStsTrack* fStsTrack     = NULL;  // sts track
  CbmMuchTrack* fMuchTrack   = NULL;  // much track
  CbmTrdTrack* fTrdTrack     = NULL;  // trd track
  CbmRichRing* fRichRing     = NULL;  // rich ring
  CbmTofHit* fTofHit         = NULL;  // tof hit

  CbmMCTrack* fMCTrack              = NULL;  // matched mc track
  CbmTrackMatchNew* fStsTrackMatch  = NULL;  // sts track match
  CbmTrackMatchNew* fMuchTrackMatch = NULL;  // much track match
  CbmTrackMatchNew* fTrdTrackMatch  = NULL;  // trd track match
  CbmTrackMatchNew* fRichRingMatch  = NULL;  // rich ring match

  FairTrackParam* fRichProj    = NULL;  // rich ring projection
  FairTrackParam* fMvdEntrance = NULL;  // mvd entrance

  TLorentzVector fMomentum;  // momentum vector
  TLorentzVector fPosition;  // position vector
  Double_t fChi2Vtx = -1.;   // chi2 to vertex in sigma
  Short_t fCharge   = 0;     // charge
  Int_t fPdgCode    = 0;     // pdg code
  Int_t fLabel      = -1;    // MC label
  Double_t fWeight  = 1.;    // weighting
  Int_t fMultiMatch = 0;     // MC label for n-times matched tracks

  Bool_t fFastTrack = kFALSE;  // fast simulation track identifier

  PairAnalysisTrack& operator=(const PairAnalysisTrack& c);
  ClassDef(PairAnalysisTrack, 6)  // Track structure
};


#endif
