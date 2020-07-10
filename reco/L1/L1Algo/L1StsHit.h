#ifndef _L1StsHit_h_
#define _L1StsHit_h_

#include "L1Strip.h"


//struct L1Branch;
typedef unsigned /*short*/ int THitI;  // strip index type
typedef unsigned short int TZPosI;     // strip z-coor index type


class L1StsHit {
public:
  TStripI f, b;  // front and back strip indices
  TZPosI iz;     // index of z coor. in L1Algo::vStsZPos

  float dx, dy, dxy;
  float du, dv;
  bool used;
  float t_reco;
  float t_er;
  int ista;
  //   int track, n;
  float x, y;


  L1StsHit()
    : f(0)
    , b(0)
    , iz(0)
    , dx(0.)
    , dy(0.)
    , dxy(0.)
    , du(0.)
    , dv(0.)
    , used(false)
    , t_reco(0.f)
    , t_er(0.)
    , ista(-1)
    , x(0.)
    , y(0.) {}
  L1StsHit(L1StsHit& h, /*short*/ int sh, /*short*/ int shB)
    : f(h.f + sh)
    , b(h.b + shB)
    , iz(0)
    , dx(0.)
    , dy(0.)
    , dxy(0.)
    , du(0.)
    , dv(0.)
    , used(false)
    , t_reco(0.f)
    , t_er(0.)
    , ista(-1)
    , x(0.)
    , y(0.) {}
};

#endif
