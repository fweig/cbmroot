/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaCbmDstAnaTreeContainer.h
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATCONTAINER_H_
#define NICACBMATCONTAINER_H_

#include <TNamed.h>

#include "AnalysisTree/Detector.hpp"
#include "AnalysisTree/EventHeader.hpp"
#include "AnalysisTree/Matching.hpp"

class TChain;

struct AnaTreeRecoIds {
  Int_t vtx_px, vtx_py, vtx_pz;
  Int_t vtx_dcax, vtx_dcay, vtx_dcaz;
  Int_t vtx_vtxchi2, vtx_chi2;
  Int_t vtx_q, vtx_nhits, vtx_mvdhits;
  Int_t vtx_x, vtx_cx0, vtx_cov1;
  Int_t tof_mass2;
};

class CbmAnaTreeRecoSourceContainer : public TNamed {
  AnalysisTree::EventHeader* fEvent;
  AnalysisTree::TrackDetector* fVtxTracks;
  AnalysisTree::Matching* fVtx2Tof;
  AnalysisTree::HitDetector* fTofHits;
  AnalysisTree::Matching* fVtx2Mc;
  AnaTreeRecoIds fVtxIds;

public:
  CbmAnaTreeRecoSourceContainer()
    : fEvent(nullptr)
    , fVtxTracks(nullptr)
    , fVtx2Tof(nullptr)
    , fTofHits(nullptr)
    , fVtx2Mc(nullptr) {};
  Bool_t ConnectToTree(TChain* tree);
  inline AnalysisTree::EventHeader* GetEventHeader() const { return fEvent; };
  inline AnalysisTree::TrackDetector* GetVtxTracks() const { return fVtxTracks; };
  inline AnalysisTree::HitDetector* GetTofHits() const { return fTofHits; };
  inline AnalysisTree::Matching* GetVtx2ToFMatch() const { return fVtx2Tof; };
  inline AnalysisTree::Matching* GetVtx2Sim() const { return fVtx2Mc; };
  inline AnaTreeRecoIds& GetFieldIds() { return fVtxIds; }
  virtual ~CbmAnaTreeRecoSourceContainer()
  {
    if (fEvent) {
      delete fEvent;
      delete fVtxTracks;
      delete fTofHits;
      delete fVtx2Tof;
      delete fVtx2Mc;
    }
  }
  ClassDef(CbmAnaTreeRecoSourceContainer, 1)
};

struct AnaTreeMcIds {
  Int_t px, py, pz;
  Int_t pdg, motherId, mass;
  Int_t event_b;
  Int_t event_psi;
  Int_t freezX, freezY, freezZ, freezT;
};

class CbmAnaTreeMcSourceContainer : public TNamed {
  AnalysisTree::EventHeader* fEvent;
  AnalysisTree::Particles* fParticles;
  AnaTreeMcIds fIds;

public:
  CbmAnaTreeMcSourceContainer() : fEvent(nullptr), fParticles(nullptr) {};
  Bool_t ConnectToTree(TChain* tree);
  inline AnaTreeMcIds& GetFieldIds() { return fIds; };
  inline AnalysisTree::EventHeader* GetEventHeader() const { return fEvent; };
  inline AnalysisTree::Particles* GetParticles() const { return fParticles; };
  virtual ~CbmAnaTreeMcSourceContainer()
  {
    if (fEvent) {
      delete fEvent;
      delete fParticles;
    }
  };
  ClassDef(CbmAnaTreeMcSourceContainer, 1)
};

#endif /* NICACBMATCONTAINER_H_ */
