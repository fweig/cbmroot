/*
 * NicaEventComplexCut.h
 *
 *  Created on: 24 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAROOT_CUTS_EVENTCUTS_NICAEVENTCOMPLEXCUT_H_
#define NICAROOT_CUTS_EVENTCUTS_NICAEVENTCOMPLEXCUT_H_

#include "NicaEventCut.h"

class NicaEventComplexCut : public NicaEventCut {
  NicaEventCut* fRealCut;
  NicaEventCut* fImgCut;
  Int_t fStep;
  ULong64_t fNullObjects;
  Bool_t fAcceptNulls;

public:
  NicaEventComplexCut();
  NicaEventComplexCut(const NicaEventCut* real, const NicaEventCut* img);
  NicaEventComplexCut(const NicaEventCut& real, const NicaEventCut& img);
  NicaEventComplexCut(const NicaEventComplexCut& other);
  void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
  virtual Bool_t Pass(NicaEvent* pair);
  virtual Bool_t Init(Int_t task_id);
  virtual NicaPackage* Report() const;
  virtual TString CutName(Option_t* opt = "") const;
  NicaEventCut* GetRealCut() const { return fRealCut; };
  NicaEventCut* GetImgCut() const { return fImgCut; };
  virtual ~NicaEventComplexCut();
  ClassDef(NicaEventComplexCut, 1)
};

class NicaEventRealCut : public NicaEventCut {
  NicaEventCut* fRealCut;

public:
  NicaEventRealCut();
  NicaEventRealCut(const NicaEventCut* real);
  NicaEventRealCut(const NicaEventCut& real);
  NicaEventRealCut(const NicaEventRealCut& other);
  virtual Bool_t Pass(NicaEvent* event);
  virtual Bool_t Init(Int_t task_id);
  virtual NicaPackage* Report() const;
  virtual TString CutName(Option_t* opt = "") const;
  NicaEventCut* GetRealCut() const { return fRealCut; };
  virtual ~NicaEventRealCut();
  ClassDef(NicaEventRealCut, 1)
};

class NicaEventImaginaryCut : public NicaEventCut {
  NicaEventCut* fImgCut;
  ULong64_t fNullObjects;
  Bool_t fAcceptNulls;

public:
  NicaEventImaginaryCut();
  NicaEventImaginaryCut(const NicaEventCut* img);
  NicaEventImaginaryCut(const NicaEventCut& img);
  NicaEventImaginaryCut(const NicaEventImaginaryCut& other);
  void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
  virtual Bool_t Pass(NicaEvent* event);
  virtual Bool_t Init(Int_t task_id);
  virtual NicaPackage* Report() const;
  virtual TString CutName(Option_t* opt = "") const;
  NicaEventCut* GetImgCut() const { return fImgCut; };
  virtual ~NicaEventImaginaryCut();
  ClassDef(NicaEventImaginaryCut, 1)
};

#endif /* NICAROOT_CUTS_EVENTCUTS_NICAEVENTCOMPLEXCUT_H_ */
