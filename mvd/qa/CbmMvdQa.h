// -------------------------------------------------------------------------
// -----              CbmMvdQa  header file                            -----
// -----              Created 12/01/15  by P. Sitzmann                 -----
// -------------------------------------------------------------------------


/**  CbmMvdQa.h
 *@author P.Sitzmann <p.sitzmann@gsi.de>
 *
 *  Selection of Qa functions for the mvd
 *
 **/


#ifndef CBMMVDQA_H
#define CBMMVDQA_H 1

#include "FairTask.h"


class TClonesArray;
class TH1F;
class TH2F;

class FairTrackParam;

class CbmMvdDetector;
//class CbmLitTrackParam;
class CbmVertex;
class CbmStsTrack;


class CbmMvdQa : public FairTask
{
public:
  CbmMvdQa();
  
  CbmMvdQa(const char* name, Int_t iMode = 0, Int_t iVerbose = 0);

  CbmMvdQa(const CbmMvdQa&) = delete; 
  CbmMvdQa& operator=(const CbmMvdQa&) = delete; 

  ~CbmMvdQa();

  void     SetMinHitReq(Int_t nrOfHits){fminHitReq = nrOfHits;};
  void     SetMatches(Int_t MCtrackID, CbmStsTrack* stsTrack);
  void     Exec(Option_t* opt); 

  InitStatus Init();

  void Finish();
   
  void SetUseMcQa(){useMcQa = kTRUE;};
  void SetUseDigiQa(){useDigiQa = kTRUE;};
  void SetUseHitQa(){useHitQa = kTRUE;};
  void SetUseTrackQa(){useTrackQa = kTRUE;};

  void SetOutFile(TFile* outFile){foutFile = outFile;};
  void SetDrawOutput(){fdraw = kTRUE;};

private:

  TFile* foutFile;

  Int_t fNHitsOfLongTracks;
  Int_t fEventNumber;
  Int_t fminHitReq;
  Float_t fMvdRecoRatio;
  Int_t fBadTrack;
  Int_t fUsedTracks;
  Int_t fnrOfMergedHits;

  Int_t fFirstMvdPos;



  Int_t fnrTrackslowP;
  Int_t fnrTracksHighP;
  Int_t flow;
  Int_t fmid;
  Int_t fhigh;

  TClonesArray* fStsTrackArray;
  TClonesArray* fStsTrackArrayP;
  TClonesArray* fStsTrackArrayN;
  TClonesArray* fStsTrackMatches;
  TClonesArray* fGlobalTrackArray;
 

  TClonesArray* fListMCTracks;
  TClonesArray* fMCTrackArrayP;
  TClonesArray* fMCTrackArrayN;

  TClonesArray* fMcPoints;
  TClonesArray* fMvdDigis;
  TClonesArray* fMvdCluster;
  TClonesArray* fMvdHits;
  TClonesArray* fMvdHitMatchArray;
    TClonesArray* fMvdDigiMatchArray;
  TClonesArray* fBadTracks;

  TClonesArray* fInfoArray;

  static const Int_t f1FSize = 34;
  static const Int_t f2FSize = 12;

  TH1F* fMC1F[30];
  TH2F* fMC2F[30];

  TH1F* fDigi1F[30];
  TH2F* fDigi2F[30];

  TH1F* fHits1F[30];
  TH2F* fHits2F[30];

  TH1F* fTracks1F[f1FSize];
  TH2F* fTracks2F[f2FSize];

  CbmVertex* fPrimVtx;
  CbmVertex* fSecVtx;

  CbmMvdDetector* fDetector;

  Bool_t useMcQa;
  Bool_t useDigiQa;
  Bool_t useHitQa;
  Bool_t useTrackQa;

  Int_t fMode;

  Bool_t fdraw;

 // Double_t GetImpactParameterRadius(CbmLitTrackParam etrack);
 // Double_t GetImpactParameterX(CbmLitTrackParam etrack);
 // Double_t GetImpactParameterY(CbmLitTrackParam etrack);

 // Double_t GetTransverseMomentum(CbmLitTrackParam etrack);
 // Double_t GetMomentum(CbmLitTrackParam etrack);
 // Double_t GetMomentumZ(CbmLitTrackParam etrack);
 // Double_t GetAngle(CbmLitTrackParam etrack);

  void SetupHistograms();
  void SetupMCHistograms();
  void SetupDigiHistograms();
  void SetupHitHistograms();
  void SetupTrackHistograms();

  void ExecDigiQa();
  void ExecHitQa();
  void ExecMCQa();
  void ExecTrackQa();

  void FinishMCQa();
  void FinishDigiQa();
  void FinishHitQa();
  void FinishTrackQa();

  Bool_t HasHitFirstMvd(CbmStsTrack* stsTrack);
  Bool_t HasHitFirstTrue(Int_t MCtrackID, CbmStsTrack* stsTrack);

  void GetFirstMCPos(CbmStsTrack* stsTrack, Float_t* pos);
  void GetFirstMvdHitPos(CbmStsTrack* stsTrack, Float_t* hitPos);


 ClassDef(CbmMvdQa,1);
};


#endif
