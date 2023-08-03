/*
 * NicaTrackCutComplex.h
 *
 *  Created on: 9 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKCOMPLEXCUT_H_
#define NICATRACKCOMPLEXCUT_H_

#include "NicaTrackCut.h"
#include "NicaTrackVirtualCut.h"

class NicaTrackComplexCut : public NicaTrackCut {
  NicaTrackCut* fRealCut;
  NicaTrackCut* fImgCut;
  Int_t fStep;
  ULong64_t fNullObjects;
  Bool_t fAcceptNulls;

public:
  /**
   * dummy ct-ro only for ROOT
   */
  NicaTrackComplexCut();
  /**
   * default constructor
   * @param real - real cut
   * @param img imaginary cut
   */
  NicaTrackComplexCut(const NicaTrackCut* real, const NicaTrackCut* img);
  NicaTrackComplexCut(const NicaTrackCut& real, const NicaTrackCut& img);
  NicaTrackComplexCut(NicaTrackCut* cut);
  NicaTrackComplexCut(const NicaTrackComplexCut& other);
  void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
  virtual Bool_t Pass(NicaTrack* track);
  virtual Bool_t Init(Int_t task_id);
  virtual NicaPackage* Report() const;
  virtual TString CutName(Option_t* opt = "") const;
  /**
   *
   * @return "real" part of cut
   */
  NicaTrackCut* GetRealCut() const { return fRealCut; };
  /**
   *
   * @return "imaginary" part of cut
   */
  NicaTrackCut* GetImgCut() const { return fImgCut; };
  virtual ~NicaTrackComplexCut();
  ClassDef(NicaTrackComplexCut, 1)
};

class NicaTrackRealCut : public NicaTrackCut {
  NicaTrackCut* fRealCut;

public:
  NicaTrackRealCut();
  NicaTrackRealCut(const NicaTrackCut* real);
  NicaTrackRealCut(const NicaTrackCut& real);
  NicaTrackRealCut(const NicaTrackRealCut& other);
  virtual Bool_t Pass(NicaTrack* track);
  virtual Bool_t Init(Int_t task_id);
  virtual NicaPackage* Report() const;
  virtual TString CutName(Option_t* opt = "") const;
  NicaTrackCut* GetRealCut() const { return fRealCut; };
  virtual ~NicaTrackRealCut();
  ClassDef(NicaTrackRealCut, 1)
};

class NicaTrackImaginaryCut : public NicaTrackCut {
  NicaTrackCut* fImgCut;
  ULong64_t fNullObjects;
  Bool_t fAcceptNulls;

public:
  NicaTrackImaginaryCut();
  NicaTrackImaginaryCut(const NicaTrackCut* img);
  NicaTrackImaginaryCut(const NicaTrackCut& img);
  NicaTrackImaginaryCut(const NicaTrackImaginaryCut& other);
  /**
   * accept tracks without imaginary tracks
   * @param accept
   */
  void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
  virtual Bool_t Pass(NicaTrack* track);
  virtual Bool_t Init(Int_t task_id);
  virtual NicaPackage* Report() const;
  virtual TString CutName(Option_t* opt = "") const;
  NicaTrackCut* GetImgCut() const { return fImgCut; };
  virtual ~NicaTrackImaginaryCut();
  ClassDef(NicaTrackImaginaryCut, 1)
};

#endif /* NICATRACKCOMPLEXCUT_H_ */
