#ifndef _L1StsHit_h_
#define _L1StsHit_h_

//struct L1Branch;
typedef unsigned /*short*/ int THitI;    // strip index type
typedef unsigned short int TZPosI;       // strip z-coor index type
typedef unsigned /*short*/ int TStripI;  // strip index type

class L1StsHit {
public:
  //  float dx, dy, dxy;
  TStripI f, b;  // front and back strip indices
  float du, dv;
  float t_reco;
  float t_er;
  int ID;
  // int ista;
  //int track, n;
  float u = 0, v = 0;

  TZPosI iz;  // index of z coor. in L1Algo::vStsZPos

  L1StsHit() : f(0), b(0), du(0.), dv(0.), t_reco(0.f), t_er(0.), ID(0.), iz(0) {}
};

#endif
