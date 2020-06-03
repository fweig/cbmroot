#ifndef LXCATRIPLETS_INCLUDED
#define LXCATRIPLETS_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "Rtypes.h"
#include <list>
#include "LxSettings.h"
#include <cmath>
#include "LxMC.h"

struct LxTriplet;
#ifdef USE_SEGMENTS
struct LxSegment;
#endif//USE_SEGMENTS
struct LxLayer;
struct LxTrack;

struct LxPoint
{
  Double_t x, y, z;
  Double_t dx, dy, dz;
  Double_t dx2, dy2, dz2;
  bool used;
  LxLayer* layer;
  Int_t hitId;
  LxTrack* track;
  std::list<LxTriplet*> triplets;
#ifdef USE_SEGMENTS
  std::list<LxSegment*> segments;
#endif//USE_SEGMENTS
#ifdef MAKE_EFF_CALC
  std::list<LxMCPoint*> mcPoints;
#endif//MAKE_EFF_CALC

  LxPoint(Double_t X, Double_t Y, Double_t Z, Double_t Dx, Double_t Dy, Double_t Dz, LxLayer* Layer, Int_t HitId) : x(X), y(Y), z(Z),
      dx(Dx), dy(Dy), dz(Dz), dx2(Dx * Dx), dy2(Dy * Dy), dz2(Dz * Dz), used(false), layer(Layer), hitId(HitId), track(0) {}
  ~LxPoint();
};

struct LxTriplet
{
  LxPoint* left;
  LxPoint* center;
  LxPoint* right;
  Double_t deltaZ;
  Double_t deltaZ2;
  Double_t tx;
  Double_t ty;
  Double_t dtx2;
  Double_t dty2;
  Double_t dtx;
  Double_t dty;
  Double_t chi2;
#ifdef USE_SEGMENTS
  std::list<LxSegment*> neighbours;
#else//USE_SEGMENTS
  std::list<std::pair<LxTriplet*, Double_t> > neighbours;
#endif//USE_SEGMENTS

  LxTriplet(LxPoint* l, LxPoint* c, LxPoint* r, Double_t c2) : left(l), center(c), right(r),
      deltaZ(r->z - l->z), deltaZ2(deltaZ * deltaZ), tx((r->x - l->x) / deltaZ), ty((r->y - l->y) / deltaZ), dtx2((l->dx2 + r->dx2) / deltaZ2),
      dty2((l->dy2 + r->dy2) / deltaZ2), dtx(sqrt(dtx2)), dty(sqrt(dty2)), chi2(c2) {}
};

#ifdef USE_SEGMENTS
struct LxSegment
{
  LxPoint* source;
  LxPoint* end;
  Double_t tx;
  Double_t ty;
  Double_t dtx;
  Double_t dty;
  Double_t dtx2;
  Double_t dty2;
  Double_t chi2;
  std::list<LxTriplet*> neighbours;

  LxSegment(LxPoint* s, LxPoint* e, Double_t Tx, Double_t Ty, Double_t Dtx2, Double_t Dty2, Double_t Chi2) : source(s), end(e),
      tx(Tx), ty(Ty), dtx2(Dtx2), dty2(Dty2), dtx(sqrt(Dtx2)), dty(sqrt(Dty2)), chi2(Chi2) {}
};
#endif//USE_SEGMENTS

struct LxStation;

typedef void* kdt_points_handle;

struct LxLayer
{
  kdt_points_handle points_handle;
  Double_t zCoord;
  LxStation* station;
  Int_t layerNumber;

  LxLayer(LxStation* Station, int LayerNumber);
  ~LxLayer();
  void Clear();
};

struct LxSpace;

struct LxStation
{
  LxSpace* space;
  Int_t stationNumber;
  Double_t zCoord;
  LxLayer* layers[LXLAYERS];
  Double_t xDispLeft;
  Double_t yDispLeft;
  Double_t xDispRight;
  Double_t yDispRight;
  Double_t xDispLeft2;
  Double_t yDispLeft2;
  Double_t xDispRight2;
  Double_t yDispRight2;

  Double_t xDispRL;
  Double_t yDispRL;
  Double_t xDispRL2;
  Double_t yDispRL2;

#ifdef OUT_DISP_BY_TRIPLET_DIR
  Double_t xOutDispTriplet;
  Double_t yOutDispTriplet;
  Double_t xOutDispTriplet2;
  Double_t yOutDispTriplet2;
#else//OUT_DISP_BY_TRIPLET_DIR
  Double_t xOutDispVertex;
  Double_t yOutDispVertex;
  Double_t xOutDispVertex2;
  Double_t yOutDispVertex2;
#endif//OUT_DISP_BY_TRIPLET_DIR

#ifdef USE_SEGMENTS
  Double_t txBreakLeft;
  Double_t tyBreakLeft;
  Double_t txBreakRight;
  Double_t tyBreakRight;
  Double_t txBreakLeft2;
  Double_t tyBreakLeft2;
  Double_t txBreakRight2;
  Double_t tyBreakRight2;

  Double_t txInterStationBreak;
  Double_t tyInterStationBreak;
  Double_t txInterStationBreak2;
  Double_t tyInterStationBreak2;
#else//USE_SEGMENTS
  Double_t txInterTripletBreak;
  Double_t tyInterTripletBreak;
  Double_t txInterTripletBreak2;
  Double_t tyInterTripletBreak2;
#endif//USE_SEGMENTS

  Double_t errCoeffTripletRX;
  Double_t errCoeffTripletRX2;
  Double_t errCoeffTripletRY;
  Double_t errCoeffTripletRY2;
  Double_t errCoeffTripletRLX;
  Double_t errCoeffTripletRLX2;
  Double_t errCoeffTripletRLY;
  Double_t errCoeffTripletRLY2;
  Double_t errCoeffInterTripletX;
  Double_t errCoeffInterTripletX2;
  Double_t errCoeffInterTripletY;
  Double_t errCoeffInterTripletY2;
  Double_t errCoeffInterTripletTx;
  Double_t errCoeffInterTripletTx2;
  Double_t errCoeffInterTripletTy;
  Double_t errCoeffInterTripletTy2;

  LxStation(LxSpace* sp, Int_t stN);
  ~LxStation();
  void Clear();
  void BuildTriplets();
#ifdef USE_SEGMENTS
  void BuildSegments();
#else//USE_SEGMENTS
  void ConnectTriplets();
#endif//USE_SEGMENTS
};

struct LxTrackCandidate
{
#ifdef USE_SEGMENTS
  LxTriplet* begin;
  std::pair<LxSegment*, LxTriplet*> branches[LXSTATIONS - 1];
#else//USE_SEGMENTS
  LxTriplet* branches[LXSTATIONS];
#endif//USE_SEGMENTS
  Double_t chi2;
  Int_t length;

#ifdef USE_SEGMENTS
  LxTrackCandidate(LxTriplet* Begin, std::pair<LxSegment*, LxTriplet*>* Branches, Double_t Chi2) : begin(Begin), chi2(Chi2)
  {
    for (Int_t i = 0; i < LXSTATIONS - 1; ++i)
      branches[i] = Branches[i];
  }
#else//USE_SEGMENTS
  LxTrackCandidate(Int_t len, LxTriplet** Branches, Double_t Chi2) : chi2(Chi2), length(len)
  {
    for (Int_t i = 0; i < length; ++i)
      branches[i] = Branches[i];
  }
#endif//USE_SEGMENTS
};

struct LxExtTrack
{
  CbmStsTrack* track;
  Int_t extId;
  LxMCTrack* mcTrack;
  std::pair<LxTrack*, Double_t> recoTrack;

  LxExtTrack() : track(0), extId(-1), mcTrack(0), recoTrack(0, 0) {}
};

struct LxTrack
{
#ifdef USE_SEGMENTS
  LxTriplet* begin;
  std::pair<LxSegment*, LxTriplet*> branches[LXSTATIONS - 1];
#else//USE_SEGMENTS
  LxTriplet* branches[LXSTATIONS];
#endif//USE_SEGMENTS
  Double_t chi2;
  LxExtTrack* externalTrack;
  std::list<std::pair<LxExtTrack*, Double_t> > extTrackCandidates;
  bool matched;
  LxMCTrack* mcTrack;
  LxPoint* points[LXSTATIONS * LXLAYERS];
#ifdef CALC_LINK_WITH_STS_EFF
  std::list<LxMCTrack*> mcTracks;
#endif//CALC_LINK_WITH_STS_EFF
  bool clone;
  Int_t length;

  explicit LxTrack(LxTrackCandidate* trackCandidate) :
#ifdef USE_SEGMENTS
      begin(trackCandidate->begin),
#endif//USE_SEGMENTS
      chi2(trackCandidate->chi2), externalTrack(0), matched(false), mcTrack(0), clone(false), length(trackCandidate->length)
  {
#ifdef USE_SEGMENTS
    begin->left->used = true;
    begin->left->track = this;
    begin->center->used = true;
    begin->center->track = this;
    begin->right->used = true;
    begin->right->track = this;
    points[(LXSTATIONS - 1) * LXLAYERS] = begin->left;
    points[(LXSTATIONS - 1) * LXLAYERS + 1] = begin->center;
    points[(LXSTATIONS - 1) * LXLAYERS + 2] = begin->right;
#endif//USE_SEGMENTS

#ifdef USE_SEGMENTS
    for (Int_t i = 0; i < LXSTATIONS - 1; ++i)
#else//USE_SEGMENTS
    for (Int_t i = 0; i < length; ++i)
#endif//USE_SEGMENTS
    {
      branches[i] = trackCandidate->branches[i];
#ifdef USE_SEGMENTS
      LxTriplet* triplet = branches[i].second;
#else//USE_SEGMENTS
      LxTriplet* triplet = branches[i];
#endif//USE_SEGMENTS
      triplet->left->used = true;
      triplet->left->track = this;
      triplet->center->used = true;
      triplet->center->track = this;
      triplet->right->used = true;
      triplet->right->track = this;
      points[i * LXLAYERS] = triplet->left;
      points[i * LXLAYERS + 1] = triplet->center;
      points[i * LXLAYERS + 2] = triplet->right;
    }

    for (Int_t i = length; i < LXSTATIONS; ++i)
    {
      branches[i] = 0;
      points[i * LXLAYERS] = 0;
      points[i * LXLAYERS + 1] = 0;
      points[i * LXLAYERS + 2] = 0;
    }
  }

  void Rebind();
};

struct LxSpace
{
  LxStation* stations[LXSTATIONS];
  std::list<LxTrack*> tracks;
  std::list<LxExtTrack> extTracks;
  Int_t stationsInAlgo;

  LxSpace();
  ~LxSpace();
  void Clear();
  void BuildTriplets();
#ifdef USE_SEGMENTS
  void BuildSegments();
  void BuildCandidates(LxTriplet* begin, LxTriplet* triplet, std::pair<LxSegment*, LxTriplet*>* branches,
      std::list<LxTrackCandidate*>& candidates, Int_t level, Double_t chi2);
#else//USE_SEGMENTS
  void ConnectTriplets();
  void BuildCandidates(Int_t endStNum, LxTriplet* triplet, LxTriplet** branches,
      std::list<LxTrackCandidate*>& candidates, Int_t level, Double_t chi2);
#endif//USE_SEGMENTS
  void RemoveClones();
  void Reconstruct();
  void JoinExtTracks();
  void FitTracks();
  LxPoint* AddPoint(Int_t stationNumber, Int_t layerNumber, Int_t hitId, Double_t x, Double_t y, Double_t z, Double_t dx, Double_t dy, Double_t dz);
};

#endif//LXCATRIPLETS_INCLUDED
