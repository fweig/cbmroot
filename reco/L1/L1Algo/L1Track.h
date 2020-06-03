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
 *  L1 track class
 *
 *====================================================================
 */

#ifndef L1Track_H
#define L1Track_H

class L1Track
{
  public:
  unsigned char NHits;
  unsigned char n;
  float Momentum, fTrackTime;
  fscal TFirst[7], CFirst[21], TLast[7], CLast[21], Tpv[7], Cpv[21], chi2;
  short int NDF;
  
  int FirstHitIndex, LastHitIndex;
  int index;
  int ista;
  
  
  static  bool compareCand(const L1Track &a, const L1Track &b){
      
    if (a.NHits != b.NHits) return (a.NHits > b.NHits);
    
     if (a.ista != b.ista ) return (a.ista  < b.ista );
    
     else return (a.chi2  < b.chi2 );
  }
  
  
  static bool compare(const L1Track &a, const L1Track &b){

    return (a.Cpv[20] <= b.Cpv[20] );
    }

  
};

// #include "cmath"
//   bool operator==(const L1Track &other) const { 
//     cout<<int(NHits)<<" NHits"<<endl;
//      if ((other.NHits==NHits)&&(fabs(other.Momentum-Momentum)<1.e-6)) return true;
//      else return false;
//   }

#endif
