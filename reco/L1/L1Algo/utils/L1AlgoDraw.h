/* Copyright (C) 2010-2012 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer] */

#ifndef L1AlgoDraw_h
#define L1AlgoDraw_h 1

#include <map>
#include <vector>

#include "L1Def.h"
#include "L1Hit.h"
#include "L1Station.h"
#include "L1Triplet.h"

class TCanvas;
class L1Algo;

class L1AlgoDraw {
  struct Point {
    double x {0}, y {0}, z {0};
    Point() = default;
    Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};
  };

public:
  L1AlgoDraw();

  void InitL1Draw(L1Algo* algo_);

  void DrawMCTracks();
  void DrawRecoTracks();

  void DrawTriplets(std::vector<L1Triplet>& triplets, const L1HitIndex_t* realIHit);
  void DrawDoublets(std::vector<L1HitIndex_t>* Duplets_hits, std::map<L1HitIndex_t, L1HitIndex_t>* Duplets_start,
                    const int MaxArrSize, L1HitIndex_t* StsHitsStartIndex, unsigned int* realIHit);
  void DrawDoubletsOnSta(int iSta, L1HitIndex_t* Duplets_hits, L1HitIndex_t* Duplets_start, const int MaxArrSize,
                         L1HitIndex_t* StsRestHitsStartIndex, unsigned int* realIHit);

  void DrawTarget();
  void DrawInputHits();  // draw all hits, which TF have gotten
  void DrawRestHits(L1HitIndex_t* StsRestHitsStartIndex, L1HitIndex_t* StsRestHitsStopIndex,
                    unsigned int* realIHit);  // draw only hits which leave on current iteration.

  void DrawInfo();
  void ClearVeiw();
  void SaveCanvas(TString name);
  void DrawAsk();

private:
  L1AlgoDraw(const L1AlgoDraw&);
  L1AlgoDraw& operator=(const L1AlgoDraw&);

  Point GetHitCoor(int ih);

  void DrawTriplet(int il, int im, int ir);
  void DrawDoublet(int il, int ir);

  L1Algo* algo {nullptr};

  std::vector<L1Hit> vStsHits {};
  int StsHitsStartIndex[20] {0};
  int StsHitsStopIndex[20] {0};

  int NStations {0};
  L1Station vStations[20] {};

  int mcolor[10] {5, 7, 3, 8, 6, 2, 4, 1, 9, 14};  // color for hits on i-station
  int StaColor {17};                               // color for stantions
  int hitsMStyle {20};                             // style for hits
  int fakesMStyle {24};                            // style for fakes
  int targetMStyle {29};                           // style for target

  double HitSize {1};  // size of hits

  int fVerbose {0};
  TCanvas* YZ {nullptr};
  TCanvas* YX {nullptr};
  TCanvas* XZ {nullptr};
  TCanvas* XYZ {nullptr};
  bool ask {true};
};


#endif
