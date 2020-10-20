#ifndef _L1StsHit_h_
#define _L1StsHit_h_

#include "L1Strip.h"


//struct L1Branch;
typedef unsigned /*short*/ int THitI;  // strip index type
typedef unsigned short int TZPosI;     // strip z-coor index type


class L1StsHit {
public:
  //  float dx, dy, dxy;
  TStripI f, b;  // front and back strip indices
  float du, dv;
  float t_reco;
  float t_er;
  // int ista;
  //int track, n;
  // float x, y;


  TZPosI iz;  // index of z coor. in L1Algo::vStsZPos


  L1StsHit() : f(0), b(0), du(0.), dv(0.), t_reco(0.f), t_er(0.), iz(0) {}
  L1StsHit(L1StsHit& h, /*short*/ int sh, /*short*/ int shB)
    : f(h.f + sh), b(h.b + shB), du(0.), dv(0.), t_reco(0.f), t_er(0.), iz(0) {}
};

#endif
