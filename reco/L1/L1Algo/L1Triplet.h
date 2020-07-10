#ifndef L1Triplet_H
#define L1Triplet_H
#include "../CbmL1Def.h"
class L1Triplet {
public:
  // static bool compareChain(const L1Triplet *a, const L1Triplet *b){
  // return a->chi2Chain < b->chi2Chain;
  // }
  //  int sta;
  //  int por;
  //  int tr;
  //  int loc;

private:
  THitI w0;          // left hit (16b)  index in vStsHits array
  THitI w1;          // middle hit(16b)
  THitI w2;          // right hit(16b)
                     // unsigned char      b0; // chi2(5b) + level(3b)
  unsigned char bl;  // level
  fscal b1;          // qp (8b)
  unsigned char st;  // staL (4b), staM-1-staL (2b), staR-2-staL (2b)
  fscal chi2double;


public:
  //  std::vector<unsigned int> neighbours;

  static bool compare(const L1Triplet* a,
                      const L1Triplet* b) {  // sort in back order
    return ((a->GetLHit() > b->GetLHit()))
           || ((a->GetLHit() == b->GetLHit()) && (a->GetMHit() < b->GetMHit()))
           || ((a->GetLHit() == b->GetLHit()) && (a->GetMHit() == b->GetMHit())
               && (a->GetRHit() < b->GetRHit()));
    // return a.GetLHit() >  b.GetLHit();
  }

  static bool compare(const L1Triplet& a,
                      const L1Triplet& b) {  // sort in back order
    return ((a.GetLHit() > b.GetLHit()))
           || ((a.GetLHit() == b.GetLHit()) && (a.GetMHit() < b.GetMHit()))
           || ((a.GetLHit() == b.GetLHit()) && (a.GetMHit() == b.GetMHit())
               && (a.GetRHit() < b.GetRHit()));
    // return a.GetLHit() >  b.GetLHit();
  }

  //   static bool compareLevel(const L1Triplet *a, const L1Triplet *b){ // sort in back order
  //     return   ( a->GetLevel() >  b->GetLevel() );
  //
  //   }

  static bool compareLevel(const L1Triplet& a,
                           const L1Triplet& b) {  // sort in back order
    return (a.GetLevel() > b.GetLevel());
  }

  static bool compareLevelT(const L1Triplet a,
                            const L1Triplet b) {  // sort in back order
    return (a.GetLevel() > b.GetLevel());
  }


  fscal Cqp;
  //   fscal time;
  //   fscal n;
  unsigned int first_neighbour;
  unsigned int last_neighbour;


  L1Triplet()
    : w0()
    , w1()
    , w2()
    , bl()
    , b1(0)
    , st()
    , chi2double(0)
    , Cqp(0)
    , first_neighbour(0)
    , last_neighbour(0) {};

  void Set(unsigned int iHitL,
           unsigned int iHitM,
           unsigned int iHitR,
           unsigned int iStaL,
           unsigned int iStaM,
           unsigned int iStaR,
           unsigned char Level,
           fscal Qp,
           fscal Chi2,
           fscal /*time_*/ = 0,
           fscal _Cqp      = 0,
           int /*_n*/      = 0) {
    w0 = iHitL;
    w1 = iHitM;
    w2 = iHitR;

    //     Chi2 = sqrt(fabs(Chi2))*31./3.5 ; // 3.5 == 31
    chi2double = Chi2;
    //     if( Chi2>31 || !finite(Chi2) ) Chi2 = 31;
    //     b0 = ( (static_cast<unsigned char>( Chi2 ))<<3 ) + (Level%8);
    b1 = Qp;
    bl = Level;

    st = (iStaL << 4) + ((iStaM - iStaL - 1) << 2) + (iStaR - iStaL - 2);
    //  time = time_;
    Cqp = _Cqp;
    //  n = _n;
  }

  void SetLevel(unsigned char Level) { bl = Level; }

  THitI GetLHit() const { return w0; }
  THitI GetMHit() const { return w1; }
  THitI GetRHit() const { return w2; }

  // unsigned int GetFirstNeighbour() const {
  //   return w1;
  // }
  // unsigned int GetNNeighbours() const {
  //   return w2;
  // }

  unsigned char GetLevel() const {
    //    return b0%8;
    return bl;
  }

  fscal GetQp() const { return b1; }

  fscal GetChi2() const {
    //  float x = (b0>>3)*3.5/31.;
    //  return x*x;
    return chi2double;
  }

  fscal Time() const {
    //  float x = (b0>>3)*3.5/31.;
    //  return x*x;
    // return time;
    return -111.;
  }

  fscal GetQpOrig() { return b1; }

  int GetLSta() const { return st >> 4; }

  int GetMSta() const { return ((st % 16) >> 2) + GetLSta() + 1; }

  int GetRSta() const { return (st % 4) + GetLSta() + 2; }
};

#endif
