/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel, Sergey Gorbunov [committer], Maksym Zyzak, Valentina Akishina */

/// @file L1Branch.h
/// @author Sergey Gorbunov

#ifndef L1Branch_h
#define L1Branch_h

#include "L1Def.h"
#include "L1Hit.h"
#include "L1Vector.h"

///
/// L1Branch class describes a search branch of the CA tracker
///
struct L1Branch {
  /// default constructor
  L1Branch() { fHits.reserve(25); }

  unsigned char ista {0};
  char Momentum {0};
  char NHits {0};
  fscal chi2 {0.};
  int fID {0};
  bool fIsAlive {0};
  L1Vector<L1HitIndex_t> fHits {"L1Branch::fHits"};

  //     static bool compareCand(const L1Branch *a, const L1Branch *b){
  //
  //     if (a->Lengtha != b->Lengtha) return (a->Lengtha > b->Lengtha);
  //
  //     if (a->ista != b->ista ) return (a->ista  < b->ista );
  //
  //   if (a->chi2  != b->chi2 )return (a->chi2  < b->chi2 );
  // //return (a->chi2  < b->chi2 );
  //   //   return (a->CandIndex < b->CandIndex );
  //        return (a->CandIndex > b->CandIndex );
  //
  //   }

  //     inline static  bool compareCand(const L1Branch &a, const L1Branch &b){
  //
  //
  //
  //     if (a.Lengtha != b.Lengtha) return (a.Lengtha > b.Lengtha);
  //
  //
  //     // if (a.time_dif != b.time_dif) return (a.time_dif < b.time_dif);
  //
  //
  //     if (a.ista != b.ista ) return (a.ista  < b.ista );
  //
  //
  //
  //     if (a.chi2 != b.chi2 ) return (a.chi2  < b.chi2 );
  //
  //       return (a.CandIndex  < b.CandIndex );
  //     //   return (a->CandIndex < b->CandIndex );
  //    // return (a.CandIndex < b.CandIndex );
  //
  //   }

  inline static bool compareCand(const L1Branch& a, const L1Branch& b)
  {

    if (a.NHits != b.NHits) return (a.NHits > b.NHits);

    if (a.ista != b.ista) return (a.ista < b.ista);

    return (a.chi2 < b.chi2);
  }


  //    static bool compareChi2(const L1Branch &a, const L1Branch &b){
  //    return (a.Quality > b.Quality );
  //  }

  //      static bool compareChi2(const L1Branch &a, const L1Branch &b){
  //          if (a.Lengtha != b.Lengtha) return (a.Lengtha > b.Lengtha);
  //
  //           if (a.chi2  != b.chi2 )return (a.chi2  < b.chi2 );
  //     //return (a->chi2  < b->chi2 );
  //     //   return (a->CandIndex < b->CandIndex );
  //     return (a.CandIndex >= b.CandIndex );
  //     }


  // static bool compareChi2(const L1Branch &a, const L1Branch &b){
  // return (a.Quality > b.Quality );
  //}

  void Set(unsigned char iStation, unsigned char Length, float Chi2, float Qp, int ID)
  {
    NHits = Length;
    ista  = iStation;
    //  iN = ( (static_cast<unsigned char>( Chi2 ))<<3 ) + (Level%8);
    //unsigned short int ista_l = 16-iStation;
    // float tmp = sqrt(Chi2) / 3.5 * 255;
    // if (tmp > 255) tmp = 255;
    // unsigned short int chi_2 = 255 - static_cast<unsigned char>( tmp );
    // Quality = (Length<<12) + (ista_l<<8) + chi_2;
    Momentum = 1.0 / fabs(Qp);
    //    chi2 = chi_2;
    chi2 = Chi2;
    fID      = ID;
    fIsAlive = true;
  }

  //   void SetLength( unsigned char Length ){
  //     Quality += - (Quality*(4096)) + (Length/(4096));
  //   }

  //  static bool compareChi2Q(const L1Branch &a, const L1Branch &b){
  ////      return (a.Quality > b.Quality);
  ////
  //     if (a.Lengtha != b.Lengtha) return (a.Lengtha > b.Lengtha);
  //
  //    if (a.ista != b.ista ) return (a.ista  < b.ista );
  //
  //    return (a.chi2 < b.chi2 );
  //  }
  //  static bool comparePChi2(const L1Branch *a, const L1Branch *b){
  //    return compareChi2(*a,*b);
  //  }
  static bool compareMomentum(const L1Branch& a, const L1Branch& b) { return (a.Momentum > b.Momentum); }
  static bool comparePMomentum(const L1Branch* a, const L1Branch* b) { return compareMomentum(*a, *b); }
};

#endif
