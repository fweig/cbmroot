
//  ----------------------------------------------------------------------
//  -----                         CbmKFTofHit                        -----
//  -----               Created 12/12/2005  by D.Kresan              -----
//  ----------------------------------------------------------------------


/** CbmKFTofHit
 *@author D.Kresan <D.Kresan@gsi.de>
 ** Interface class, representing CbmTofHit in Kalman Filter.
 **/


#ifndef CBMKFTOFHIT
#define CBMKFTOFHIT 1

#include "CbmKFHit.h"
#include "CbmKFUMeasurement.h"

class CbmKFWall;
class CbmTofHit;

class CbmKFTofHit : public CbmKFHit {

public:

    /** Default constructor **/
    CbmKFTofHit(): FitPoint(), wall(0) {};

    /** Destructor **/
    ~CbmKFTofHit() {};

    /** Measurement point **/
//    CbmKFPixelMeasurement FitPoint;
    CbmKFUMeasurement FitPoint;

    /** Material **/
    CbmKFWall* wall;

    /** Copy data from CbmTofHit **/
    void Create(CbmTofHit* hit);

    /** Filter **/
    Int_t Filter(CbmKFTrackInterface& track, Bool_t downstream, Double_t& QP0);

  const CbmKFTofHit& operator=(const CbmKFTofHit& a) {
    wall = a.wall;
    FitPoint = a.FitPoint;
    return *this;
  };

  CbmKFTofHit(const CbmKFTofHit& a):
    FitPoint(a.FitPoint),
    wall(a.wall)    
  {};
  
  ClassDef(CbmKFTofHit, 1);

};


#endif


