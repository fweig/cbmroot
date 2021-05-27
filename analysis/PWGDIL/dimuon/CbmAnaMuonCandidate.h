/** @file CbmAnaMuonCandidate.h
  * @copyright Copyright (C) 2009-2020 Petersburg Nuclear Physics Institute named by B.P.Konstantinov of National Research Centre "Kurchatov Institute", Gatchina
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Anna Senger, Evgeny Kryshen [orginator] **/

//----------------------------------------
//
// 2019 A. Senger a.senger@gsi.de
//
//----------------------------------------

#ifndef CBMANAMUONCANDIDATE_H
#define CBMANAMUONCANDIDATE_H

#include "CbmKFTrack.h"

#include "TLorentzVector.h"
#define NPLANES 31

class CbmAnaMuonCandidate : public TObject {
public:
  CbmAnaMuonCandidate();
  virtual ~CbmAnaMuonCandidate() {};

  void SetMomentum(TLorentzVector mom) { fMom = TLorentzVector(mom); }
  void SetSign(Int_t sign) { fSign = sign; }

  void SetNStsHits(Int_t nHits) { fNStsHits = nHits; }
  void SetNMuchHits(Int_t nHits) { fNMuchHits = nHits; }
  void SetNTrdHits(Int_t nHits) { fNTrdHits = nHits; }
  void SetNTofHits(Int_t nHits) { fNTofHits = nHits; }

  void SetChiToVertex(Double_t chi) { fChiToVertex = chi; }

  void SetChiMuch(Double_t chi) { fChiMuch = chi; }
  void SetChiSts(Double_t chi) { fChiSts = chi; }
  void SetChiTrd(Double_t chi) { fChiTrd = chi; }
  void SetChiGlobal(Double_t chi) { fChiGlobal = chi; }

  void SetTrueMu(Int_t mu) { fMu = mu; }
  void SetStsPdg(Int_t pdg) { fPdg = pdg; }
  void SetAnnId(Double_t tID) { fId = tID; }

  void SetTofM(Double_t m) { fM = m; }

  TLorentzVector* GetMomentum() { return &fMom; }
  Double_t GetSign() { return fSign; }

  Int_t GetNMuchHits() { return fNMuchHits; }
  Int_t GetNStsHits() { return fNStsHits; }
  Int_t GetNTrdHits() { return fNTrdHits; }
  Int_t GetNTofHits() { return fNTofHits; }

  Double_t GetChiToVertex() { return fChiToVertex; }

  Double_t GetChiMuch() { return fChiMuch; }
  Double_t GetChiSts() { return fChiSts; }
  Double_t GetChiTrd() { return fChiTrd; }
  Double_t GetChiGlobal() { return fChiGlobal; }

  Int_t GetTrueMu() { return fMu; }
  Int_t GetStsPdg() { return fPdg; }
  Double_t GetAnnId() { return fId; }

  Double_t GetTofM() { return fM; }

private:
  CbmKFTrack track;

  TLorentzVector fMom;

  Int_t fNStsHits;
  Int_t fNMuchHits;
  Int_t fNTrdHits;
  Int_t fNTofHits;

  Double_t fChiToVertex;

  Int_t fSign;

  Double_t fChiMuch;
  Double_t fChiSts;
  Double_t fChiTrd;
  Double_t fChiGlobal;

  Int_t fMu;
  Int_t fPdg;
  Double_t fId;
  Double_t fM;

  ClassDef(CbmAnaMuonCandidate, 2);
};

#endif
