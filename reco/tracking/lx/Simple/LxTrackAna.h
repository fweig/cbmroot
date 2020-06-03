#ifndef LXTRACKANA_INCLUDED
#define LXTRACKANA_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "FairTask.h"
#include "TClonesArray.h"
#include "CbmMuchPixelHit.h"
#include "LxSettings.h"
#include <list>
#include <vector>
#include "LxTrackAnaSegments.h"

struct LxSimplePoint
{
  scaltype x;
  scaltype y;
  scaltype z;
  scaltype tx;
  scaltype ty;
  LxSimplePoint() : x(0), y(0), z(0), tx(0), ty(0) {}
  LxSimplePoint(scaltype X, scaltype Y, scaltype Z, scaltype Tx, scaltype Ty) : x(X), y(Y), z(Z), tx(Tx), ty(Ty) {}
};

struct LxSimpleTrack
{
  Int_t pdgCode;
  Int_t motherId;
  scaltype p;
  scaltype pt;
  scaltype px;
  scaltype py;
  scaltype pz;
  scaltype e;
  scaltype charge;
  LxSimpleTrack(Int_t pdgc, Int_t mid, scaltype P, scaltype Pt, scaltype Px, scaltype Py, scaltype Pz, scaltype E) : pdgCode(pdgc),
      motherId(mid), p(P), pt(Pt), px(Px), py(Py), pz(Pz), e(E), charge(0), linkedMuchTrack(0, 0), linkedStsTrack(0) {}
  std::list<LxSimplePoint> stsPoints[LXSTSSTATIONS];
  std::list<LxSimplePoint> muchPoints[LXSTATIONS][LXLAYERS];
  std::list<LxSimplePoint> muchMCPts[LXSTATIONS][LXLAYERS];// These array is used for storing MUCH MC points when the 'main' array contains hits.
  std::pair<LxSimpleTrack*, scaltype> linkedMuchTrack;
  std::list<std::pair<LxSimpleTrack*, scaltype> > linkedStsTracks;// The front() contains STS track with the minimal chi2.
  LxSimpleTrack* linkedStsTrack;
  void RebindMuchTrack();
};

class LxTrackAna : public FairTask
{
public:
  LxTrackAna();
  ~LxTrackAna();
  InitStatus Init();// Inherited virtual.
  void Exec(Option_t* opt);// Inherited virtual.
  void FinishTask();// Inherited virtual.
  bool GetUseHitsInStat() const { return useHitsInStat; }
  void SetUseHitsInStat(bool v) { useHitsInStat = v; }
  bool GetAveragePoints() const { return averagePoints; }
  void SetAveragePoints(bool v) { averagePoints = v; }
  bool GetDontTouchNonPrimary() const { return dontTouchNonPrimary; }
  void SetDontTouchNonPrimary(bool v) { dontTouchNonPrimary = v; }
  bool GetUseChargeSignInCuts() const { return useChargeSignInCuts; }
  void SetUseChargeSignInCuts(bool v) { useChargeSignInCuts = v; }
  bool GetBuildConnectStat() const { return buildConnectStat; }
  void SetBuildConnectStat(bool v) { buildConnectStat = v; }
  bool GetBuildBgrInvMass() const { return buildBgrInvMass; }
  void SetBuildBgrInvMass(bool v) { buildBgrInvMass = v; }
  bool GetBuildSigInvMass() const { return buildSigInvMass; }
  void SetBuildSigInvMass(bool v) { buildSigInvMass = v; }
  bool GetJoinData() const { return joinData; }
  void SetJoinData(bool v) { joinData = v; }
  bool GetBuildNearestHitDist() const { return buildNearestHitDist; }
  void SetBuildNearestHitDist(bool v) { buildNearestHitDist = v; }
  bool GetCropHits() const { return cropHits; }
  void SetCropHits(bool v) { cropHits = v; }
  bool GetBuildSegmentsStat() const { return buildSegmentsStat; }
  void SetBuildSegmentsStat(bool v) { buildSegmentsStat = v; }
  TString GetParticleType() const { return particleType; }
  void SetParticleType(TString v)
  {
    particleType = v;
    segmentsAnalyzer.SetParticleType(v);
  }

private:
  void Clean();
  void AveragePoints();
  void BuildStatistics();
  void Connect(bool useCuts);
  void Connect(LxSimpleTrack* muchTrack, LxSimplePoint muchPt0, scaltype txMuch, scaltype tyMuch,
      bool useCuts);

  TClonesArray* listMCTracks;
  TClonesArray* listStsPts;
  TClonesArray* listMuchPts;
  TClonesArray* listMuchPixelHits;
  TClonesArray* listMuchClusters;
  TClonesArray* listMuchPixelDigiMatches;
  std::vector<LxSimpleTrack*> allTracks;
  std::list<LxSimpleTrack*> posTracks;
  std::list<LxSimpleTrack*> negTracks;
  TTree* superEventTracks;
  LxSimpleTrack superEventBrachTrack;
  bool useHitsInStat;
  bool averagePoints;
  bool dontTouchNonPrimary;
  bool useChargeSignInCuts;
  bool buildConnectStat;
  bool buildBgrInvMass;
  bool buildSigInvMass;
  bool joinData;
  bool buildNearestHitDist;
  bool cropHits;
  bool buildSegmentsStat;
  TString particleType;
  LxTrackAnaSegments segmentsAnalyzer;

  friend class LxTrackAnaSegments;

  ClassDef(LxTrackAna, 1);
};

#endif//LXTRACKANA_INCLUDED
