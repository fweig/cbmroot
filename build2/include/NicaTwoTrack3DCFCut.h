/*
 * NicaTwoTrack3DCFCut.h
 *
 *  Created on: 4 sty 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_CUTS_TWOTRACKCUTS_KINEMATICS_NICATWOTRACK3DCFCUT_H_
#define NICAFEMTO_CUTS_TWOTRACKCUTS_KINEMATICS_NICATWOTRACK3DCFCUT_H_

#include "NicaTwoTrackCut.h"
#include "TH3D.h"

class NicaTwoTrack3DCFCut : public NicaTwoTrackCut {
  TH3D* fMap;
  Int_t fPID1, fPID2;
  Int_t fFrame;
  Bool_t fAbs;
  Double_t fM12, fM22;

public:
  NicaTwoTrack3DCFCut();
  void SetHistogram(TH3D const& histo);
  void SetAssumedPID(Int_t pid1, Int_t pid2) {
    fPID1 = pid1;
    fPID2 = pid2;
  };
  void UseLCMS() { fFrame = 0; };
  void UsePRF() { fFrame = 1; };
  void UseAbs(Bool_t abs = kTRUE) { fAbs = kTRUE; };
  virtual Bool_t Init(Int_t);
  virtual NicaCut* MakeCopy() const;
  virtual Bool_t Pass(NicaTwoTrack* pair);
  virtual NicaPackage* Report() const;
  virtual ~NicaTwoTrack3DCFCut();
  NicaTwoTrack3DCFCut(const NicaTwoTrack3DCFCut& other);
  NicaTwoTrack3DCFCut& operator=(const NicaTwoTrack3DCFCut& other);
  ClassDef(NicaTwoTrack3DCFCut, 1)
};


#endif /* NICAFEMTO_CUTS_TWOTRACKCUTS_KINEMATICS_NICATWOTRACK3DCFCUT_H_ */
