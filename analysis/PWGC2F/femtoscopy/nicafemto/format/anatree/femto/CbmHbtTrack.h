/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * CbmHbtTrack.h
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMHBTTRACK_H_
#define CBMHBTTRACK_H_

#include "CbmHelix.h"

#include <TVector3.h>

#include "NicaExpTrack.h"


class CbmHbtTrack : public NicaExpTrack {
  TVector3* fPosAt;
  TVector3* fMomAt;
  CbmHelix* fHelix;
  /* true if hbt values are calculated */
  Double_t fRPos;
  Double_t fRMom;

public:
  CbmHbtTrack();
  TVector3* GetPosAtR() const { return fPosAt; };
  TVector3* GetMomAtR() const { return fMomAt; };
  CbmHelix* GetHelix() const { return fHelix; };
  void CalculatePosAt(Double_t R);
  void CalculateMomAt(Double_t R);
  virtual void CopyData(NicaTrack* other);
  CbmHbtTrack(const CbmHbtTrack& other);
  CbmHbtTrack& operator=(const CbmHbtTrack& other);
  virtual ~CbmHbtTrack();
  ClassDef(CbmHbtTrack, 1)
};

#endif /* CBMHBTTRACK_H_ */
