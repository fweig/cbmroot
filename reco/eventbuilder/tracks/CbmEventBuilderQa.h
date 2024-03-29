/* Copyright (C) 2017 IKF-UFra, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina , Maksym Zyzak, Valentina Akishina [committer] */

/** @file CbmEventBuilderQa.h
 ** @author Valentina Akishina <v.akishina@gsi.de>, Maksym Zyzak <m.zyzak@gsi.de>
 ** @date 14.03.2017
 **/

#ifndef CbmEventBuilderQa_H
#define CbmEventBuilderQa_H

#include "CbmEvent.h"
#include "CbmStsTrack.h"

#include "FairTask.h"

#include "TString.h"

#include <map>
#include <vector>

class TClonesArray;
class CbmMCEventList;
#include "CbmMCDataArray.h"

class TClonesArray;
class TFile;
class TDirectory;
class TH1F;
class TH2F;
class TObject;

class CbmEventBuilderQa : public FairTask {
public:
  // Constructors/Destructors ---------
  CbmEventBuilderQa(const char* name = "CbmEventBuilderQa", Int_t iVerbose = 0,
                    TString outFileName = "CbmEventBuilderQa.root");
  const CbmEventBuilderQa& operator=(const CbmEventBuilderQa&) = delete;
  CbmEventBuilderQa(const CbmEventBuilderQa&)                  = delete;
  ~CbmEventBuilderQa();

  void SetStsTrackBranchName(const TString& name) { fStsTrackBranchName = name; }
  void SetGlobalTrackBranchName(const TString& name) { fGlobalTrackBranchName = name; }
  void SetTofBranchName(const TString& name) { fTofBranchName = name; }
  void SetMCTrackBranchName(const TString& name) { fMCTracksBranchName = name; }
  void SetTrackMatchBranchName(const TString& name) { fStsTrackMatchBranchName = name; }
  void SetMuchTrackMatchBranchName(const TString& name) { fMuchTrackMatchBranchName = name; }
  void SetTrdBranchName(const TString& name) { fTrdBranchName = name; }
  void SetRichBranchName(const TString& name) { fRichBranchName = name; }
  void SetMuchTrackBranchName(const TString& name) { fMuchTrackBranchName = name; }

  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();

private:
  std::vector<std::vector<std::vector<int>>> fPointsInTracks;


  struct SortEvents {
    CbmEvent* Event;
    CbmStsTrack track;

    SortEvents() : Event(nullptr), track() {}
  };

  static bool CompareTrackTime(const SortEvents& a, const SortEvents& b)
  {
    return (a.track.GetStartTime() < b.track.GetStartTime());
  }

  bool CalculateIsReconstructable(const int iMCFile, const int iMCEvent,
                                  const int iMCTrack);  // bool CbmL1MCTrack::IsReconstructable()

  void WriteHistosCurFile(TObject* obj);

  //names of input branches
  TString fStsTrackBranchName;
  TString fGlobalTrackBranchName;
  TString fRichBranchName;
  TString fTrdBranchName;
  TString fTofBranchName;
  TString fMuchTrackBranchName;
  TString fMCTracksBranchName;
  TString fStsTrackMatchBranchName;
  TString fRichRingMatchBranchName;
  TString fTrdTrackMatchBranchName;
  TString fTofHitMatchBranchName;
  TString fMuchTrackMatchBranchName;

  static const int fNTimeHistos = 27;
  TH1F* fTimeHisto[fNTimeHistos];


  TClonesArray* fStsDigis;     ///< Input array (class CbmStsDigi)
  TClonesArray* fStsTracks;    ///< Input array (class CbmStsDigi)
  CbmMCDataArray* fMCTracks;   ///< Input array (class CbmStsDigi)
  TClonesArray* fStsHits;      ///< Input array (class CbmStsDigi)
  CbmMCDataArray* fMvdPoints;  ///< Input array (class CbmStsDigi)
  CbmMCDataArray* fStsPoints;  ///< Input array (class CbmStsDigi)
  TClonesArray* fEvents;       ///< Output array (class CbmEvent)
  TClonesArray* fStsTrackMatchArray;
  TClonesArray* fStsHitMatch;

  CbmMCEventList* fEventList;

  //output file with histograms
  TString fOutFileName;
  TFile* fOutFile;
  TDirectory* fHistoDir;

  //histograms
  //STS
  static const int NStsHisto = 3;
  TH1F* hStsHisto[8][NStsHisto];  //All tracks, electrons, muons, pion, kaon, protons, fragments, ghost
  TH1F* hStsFitHisto[8][10];
  //Much
  static const int NMuchHisto = 5;
  TH1F* hMuchHisto[3][NMuchHisto];  //Muons, Background, Ghost
    //RICH
  static const int NRichRingHisto2D = 3;  // r, axis a, axis b
  TH2F* hRichRingHisto2D
    [10]
    [NRichRingHisto2D];  //All tracks, electrons, muons, pions, kaons, protons, fragments, mismatch, ghost track, ghost ring

  ClassDef(CbmEventBuilderQa, 1);
};

#endif
