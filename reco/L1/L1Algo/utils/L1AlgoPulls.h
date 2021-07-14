/* Copyright (C) 2010-2017 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer] */

#ifndef L1AlgoPulls_h
#define L1AlgoPulls_h

// #define BUILD_HISTO_FOR_EACH_STANTION

#ifdef BUILD_HISTO_FOR_EACH_STANTION
const int NStations = 8;
#else
const int NStations = 0;
#endif  // BUILD_HISTO_FOR_EACH_STANTION


#include "CbmL1.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TStyle.h"

#include <iostream>
#include <vector>

#include "L1Algo.h"
#include "L1Def.h"
#include "L1TrackPar.h"

using std::cout;
using std::endl;
using std::vector;

struct TL1TrackParameters {
  double x {0.}, y {0.}, tx {0.}, ty {0.}, qp {0.};

  static constexpr int NParameters = 5;

  TL1TrackParameters() {};
  TL1TrackParameters(L1TrackPar& T, int i) : x(T.x[i]), y(T.y[i]), tx(T.tx[i]), ty(T.ty[i]), qp(T.qp[i]) {};
  TL1TrackParameters(CbmL1MCPoint& T) : x(T.x), y(T.y), tx(T.px / T.pz), ty(T.py / T.pz), qp(T.q / T.p) {};

  double operator[](int i)
  {
    switch (i) {
      case 0: return x;
      case 1: return y;
      case 2: return tx;
      case 3: return ty;
      case 4: return qp;
      default: return 0.f;
    };
  }

  TL1TrackParameters operator-(TL1TrackParameters& b)
  {
    TL1TrackParameters c;
    c.x  = x - b.x;
    c.y  = y - b.y;
    c.tx = tx - b.tx;
    c.ty = ty - b.ty;
    c.qp = qp - b.qp;
    return c;
  }

  TL1TrackParameters operator/(TL1TrackParameters& b)
  {
    TL1TrackParameters c;
    c.x  = x / b.x;
    c.y  = y / b.y;
    c.tx = tx / b.tx;
    c.ty = ty / b.ty;
    c.qp = qp / b.qp;
    return c;
  }

  void Print() { cout << x << " " << y << " " << tx << " " << ty << " " << qp << endl; }
};

class L1AlgoPulls {
public:
  L1AlgoPulls() = default;

  void Init();

  //     void AddVec(L1TrackPar& T, THitI ih);
  void AddOne(L1TrackPar& T, int i, THitI ih);
  void Print();  // fast method to see pulls :)
  void Build(bool draw = 1);

  int fNAllPulls {0};  // number of AddOne calls

private:
  L1AlgoPulls(const L1AlgoPulls&);
  L1AlgoPulls& operator=(const L1AlgoPulls&);

  void makeUpHisto(TH1* hist, TString title, float& sigma);

  CbmL1* fL1 {nullptr};
  vector<TL1TrackParameters> fGPulls {};               // pulls for all stations
  vector<TL1TrackParameters> fStaPulls[NStations] {};  // pulls for each station
  TH1F* histoPull[(1 + NStations) * TL1TrackParameters::NParameters] {nullptr};

  vector<TL1TrackParameters> fGRes {};  // residuals for all stations
  TH1F* histoRes[(1 + NStations) * TL1TrackParameters::NParameters] {nullptr};

  static constexpr float TailCut = 5000.;  //
  static constexpr float csCut   = 5.;     // chi-square cut
  static constexpr int textFont  = 22;     // TNewRoman
  TStyle* histoStyle {nullptr};
};


#endif
