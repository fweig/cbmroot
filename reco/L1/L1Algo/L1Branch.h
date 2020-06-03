/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  L1 branch class
 *
 *====================================================================
 */

#ifndef L1Branch_H
#define L1Branch_H

#include "L1StsHit.h"
#include <vector>
#include "../CbmL1Def.h"

class L1Triplet;


struct L1Branch
{
  L1Branch(): n(0), Quality(0),CandIndex(0),Momentum(0),chi2(0),NHits(0),Lengtha(0),ista(0), location(0),StsHits(){
   //  L1Branch():Momentum(0),chi2(0),NHits(0),Lengtha(0),ista(0) , StsHits(){
     StsHits.resize(12);
    // CandToCand.resize(200);
  //   CandToCand2.resize(200);
    //  StsHits.reserve(8);
   
    
  }

  
  int n;
  unsigned short int Quality;
//   fscal Quality;
   int CandIndex; 

//   bool SecondBest;
//   bool ThirdBest;
  fscal Momentum, chi2;
  char NHits; 
  char Lengtha;
  unsigned char ista;
  int location;
 // int check;
  
//  float time_dif;


//   L1Branch* BestCandidateP;
//   L1Branch* SecondBestCandidateP;
  
    L1Vector<THitI> StsHits;

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
    
  inline static  bool compareCand(const L1Branch &a, const L1Branch &b){     
      
    if (a.Lengtha != b.Lengtha) return (a.Lengtha > b.Lengtha);    
    
    if (a.ista != b.ista ) return (a.ista  < b.ista );
    
    else return (a.chi2  < b.chi2 );
  }
  
  
  
    static bool compareChi2(const L1Branch &a, const L1Branch &b){
    return (a.Quality > b.Quality );
  }
    
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

  void Set( unsigned char iStation, unsigned char Length, float Chi2, float Qp ){
    Lengtha = Length;
    ista = iStation;
    unsigned short int ista_l = 16-iStation;
    float tmp = sqrt(Chi2)/3.5*255;
    if( tmp>255 ) tmp = 255;
    unsigned short int chi_2 = 255 - static_cast<unsigned char>( tmp );
    Quality = (Length<<12) + (ista_l<<8) + chi_2;
    Momentum = 1.0/fabs(Qp);
//    chi2 = chi_2;
    chi2 = Chi2;
  }

//   void SetLength( unsigned char Length ){
//     Quality += - (Quality*(4096)) + (Length/(4096));
//   }

  static bool compareChi2Q(const L1Branch &a, const L1Branch &b){
//      return (a.Quality > b.Quality);
//     
     if (a.Lengtha != b.Lengtha) return (a.Lengtha > b.Lengtha);
   
    if (a.ista != b.ista ) return (a.ista  < b.ista );
   
    return (a.chi2 < b.chi2 );
  }
  static bool comparePChi2(const L1Branch *a, const L1Branch *b){
    return compareChi2(*a,*b);
  }
  static bool compareMomentum(const L1Branch &a, const L1Branch &b){
    return (a.Momentum > b.Momentum );
  }
  static bool comparePMomentum(const L1Branch *a, const L1Branch *b){
    return compareMomentum(*a,*b);
  }

};

#endif
