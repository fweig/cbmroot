/* Copyright (C) 2018-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaCbmTrack.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATRECOTRACK_H_
#define NICACBMATRECOTRACK_H_

#include "CbmGlobalTrack.h"
#include "CbmHelix.h"

#include "AnalysisTree/Detector.hpp"
#include "AnalysisTree/Particle.hpp"
#include "NicaCbmEventInterface.h"
#include "NicaExpTrack.h"
#include "NicaToFTrack.h"

class CbmHelix;
class NicaCbmTrackInterface;
class NicaCbmTrack : public NicaExpTrack {
  NicaToFTrack* fTofTrack;
  CbmHelix* fHelix;
  Float_t fChi2Vertex;
  Int_t fMvdHits;

public:
  NicaCbmTrack();
  NicaCbmTrack(const NicaCbmTrack& other);
  NicaCbmTrack& operator=(const NicaCbmTrack& other);
  NicaToFTrack* GetTofTrack() { return fTofTrack; };
  CbmHelix* GetHelix() const { return fHelix; };
  Double_t GetVertexChi2() const { return fChi2Vertex; };
  Int_t GetNMvdHits() const { return fMvdHits; };
  void SetVertexChi2(Double_t v) { fChi2Vertex = v; };
  void SetNMvdHits(Int_t h) { fMvdHits = h; };
  virtual void CopyData(NicaTrack* other);
  virtual TObject* GetDetTrack(const UInt_t detID) const;
  virtual ~NicaCbmTrack();
  ClassDef(NicaCbmTrack, 1)
};

#endif /* NICACBMATRECOTRACK_H_ */
