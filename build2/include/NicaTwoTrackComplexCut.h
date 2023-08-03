/*
 * NicaTwoTrackComplexCut.h
 *
 *  Created on: 24 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATWOTRACKCOMPLEXCUT_H_
#define NICATWOTRACKCOMPLEXCUT_H_
#include "NicaTwoTrackCut.h"
class NicaTwoTrackComplexCut : public NicaTwoTrackCut {
  NicaTwoTrackCut* fRealCut;
  NicaTwoTrackCut* fImgCut;
  NicaTwoTrack* fRealPair;
  NicaTwoTrack* fImgPair;
  Int_t fStep;
  ULong64_t fNullObjects;
  Bool_t fAcceptNulls;

public:
  NicaTwoTrackComplexCut();
  NicaTwoTrackComplexCut(const NicaTwoTrackCut* real, const NicaTwoTrackCut* img);
  NicaTwoTrackComplexCut(const NicaTwoTrackCut& real, const NicaTwoTrackCut& img);
  NicaTwoTrackComplexCut(const NicaTwoTrackComplexCut& other);
  virtual Bool_t Pass(NicaTwoTrack* pair);
  virtual Bool_t Init(Int_t task_id);
  virtual NicaPackage* Report() const;
  virtual TString CutName(Option_t* opt = "") const;
  NicaTwoTrackCut* GetRealCut() const { return fRealCut; };
  NicaTwoTrackCut* GetImgCut() const { return fImgCut; };
  virtual ~NicaTwoTrackComplexCut();
  ClassDef(NicaTwoTrackComplexCut, 1)
};

class NicaTwoTrackRealCut : public NicaTwoTrackCut {
  NicaTwoTrackCut* fRealCut;
  NicaTwoTrack* fPair;

public:
  NicaTwoTrackRealCut();
  NicaTwoTrackRealCut(const NicaTwoTrackCut* real);
  NicaTwoTrackRealCut(const NicaTwoTrackCut& real);
  NicaTwoTrackRealCut(const NicaTwoTrackRealCut& other);
  virtual Bool_t Pass(NicaTwoTrack* track);
  virtual Bool_t Init(Int_t task_id);
  virtual NicaPackage* Report() const;
  virtual TString CutName(Option_t* opt = "") const;
  NicaTwoTrackCut* GetRealCut() const { return fRealCut; };
  virtual ~NicaTwoTrackRealCut();
  ClassDef(NicaTwoTrackRealCut, 1)
};

class NicaTwoTrackImaginaryCut : public NicaTwoTrackCut {
  NicaTwoTrackCut* fImgCut;
  NicaTwoTrack* fPair;
  ULong64_t fNullObjects;
  Bool_t fAcceptNulls;

public:
  NicaTwoTrackImaginaryCut();
  NicaTwoTrackImaginaryCut(const NicaTwoTrackCut* real);
  NicaTwoTrackImaginaryCut(const NicaTwoTrackCut& real);
  NicaTwoTrackImaginaryCut(const NicaTwoTrackImaginaryCut& other);
  void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
  virtual Bool_t Pass(NicaTwoTrack* track);
  virtual Bool_t Init(Int_t task_id);
  virtual NicaPackage* Report() const;
  virtual TString CutName(Option_t* opt = "") const;
  NicaTwoTrackCut* GetImgCut() const { return fImgCut; };
  virtual ~NicaTwoTrackImaginaryCut();
  ClassDef(NicaTwoTrackImaginaryCut, 1)
};

#endif /* NICATWOTRACKCOMPLEXCUT_H_ */
