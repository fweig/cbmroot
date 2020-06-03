#ifndef L1HitArea_H
#define L1HitArea_H

#include "L1Grid.h"
#include "CbmL1Def.h"

class L1Row;
class L1SliceData;

class L1HitArea
{
  public:

    L1HitArea( const L1Grid & grid, float y, float z, float dy, float dz );

    /**
     * look up the next hit in the requested area.
     * Sets h to the coordinates and returns the index for the hit data
     */
    bool GetNext( THitI& i );

  protected:
    const L1Grid &fGrid;

    unsigned short fBZmax;   // maximal Z bin index
    unsigned short fBDY;     // Y distance of bin indexes
    unsigned int fIndYmin; // minimum index for
    unsigned short fIz;      // current Z bin index (incremented while iterating)
    THitI fHitYlst; // last possible hit index in current z-line
    THitI fIh;      // hit index iterating inside the bins
    int fNy;      // Number of bins in Y direction
};

inline L1HitArea::L1HitArea( const L1Grid & grid, float y, float z, float dy, float dz )
  : fGrid( grid ),
  fBZmax(0),
  fBDY(0),
  fIndYmin(0),
  fIz(0),
  fHitYlst( 0 ),
  fIh( 0 ),
  fNy( fGrid.Ny() )
{
  const float minY = y - dy;
  const float maxY = y + dy;
  const float minZ = z - dz;
  const float maxZ = z + dz;
  unsigned short bYmin, bZmin, bYmax; // boundary bin indexes
  fGrid.GetBinBounded( minY, minZ, bYmin, bZmin );
  fGrid.GetBinBounded( maxY, maxZ, bYmax, fBZmax );

  fBDY = ( bYmax - bYmin + 1 ); // bin index span in y direction

  fIndYmin = ( bZmin * fNy + bYmin ); // same as grid.GetBin(fMinY, fMinZ), i.e. the smallest bin index of interest
  // fIndYmin + fBDY - 1 then is the largest bin index of interest with the same Z

  fGrid.GetBinBounds( fIndYmin, y, dy, z, dz );

  fIz = bZmin;

  fIh = fGrid.FirstHitInBin( fIndYmin );
  fHitYlst = fGrid.FirstHitInBin( fIndYmin + fBDY );

}

inline bool L1HitArea::GetNext( THitI& i )
{
  bool yIndexOutOfRange = fIh >= fHitYlst;     // current y is not in the area
  bool nextZIndexOutOfRange = (fIz >= fBZmax);   // there isn't any new z-line

  if ( yIndexOutOfRange && nextZIndexOutOfRange ) { // all iterators are over the end
    return false;
  }
  
  // at least one entry in the vector has (fIh >= fHitYlst && fIz < fBZmax)
  bool needNextZ = yIndexOutOfRange && !nextZIndexOutOfRange;
  
  // skip as long as fIh is outside of the interesting bin y-index
  while ( ISLIKELY( needNextZ ) ) {   //ISLIKELY to speed the programm and optimise the use of registers
    fIz++;   // get new z-line
      // get next hit
    fIndYmin += fNy;
    fIh = fGrid.FirstHitInBin( fIndYmin ); // get first hit in cell, if z-line is new
    fHitYlst = fGrid.FirstHitInBin( fIndYmin + fBDY );
      
    yIndexOutOfRange = fIh >= fHitYlst;
    nextZIndexOutOfRange = (fIz >= fBZmax);
    needNextZ = yIndexOutOfRange && !nextZIndexOutOfRange;
    
  }

  L1_ASSERT ( fIh < fGrid.FirstHitInBin(fGrid.N()) || yIndexOutOfRange, fIh << " < " << fGrid.FirstHitInBin(fGrid.N()) << " || " <<  yIndexOutOfRange);
  i = fIh; // return
    
  fIh++; // go to next
  return !yIndexOutOfRange;
}

class L1HitAreaTime
{
public:
    
    L1HitAreaTime( const L1Grid & grid, float y, float z, float dy, float dz, float t, float dt );
    
    /**
     * look up the next hit in the requested area.
     * Sets h to the coordinates and returns the index for the hit data
     */
    bool GetNext( THitI& i );
    
protected:
    const L1Grid &fGrid;
    
    unsigned short fBZmax;   // maximal Z bin index
    unsigned short fBDY;     // Y distance of bin indexes
    unsigned int fIndYmin; // minimum index for
    unsigned short fIz;      // current Z bin index (incremented while iterating)
    THitI fHitYlst; // last possible hit index in current z-line
    THitI fIh;      // hit index iterating inside the bins
    int fNy;      // Number of bins in Y direction
    int fNz;
    
    unsigned short fBTmax;   // maximal Z bin index
    unsigned short fIt;      // current Z bin index (incremented while iterating)
    
    unsigned short fBZmin;
    unsigned short fBYmin;
    
    
    unsigned int fIndZmin; // minimum index for
};

inline L1HitAreaTime::L1HitAreaTime( const L1Grid & grid, float y, float z, float dy, float dz, float t, float dt )
: fGrid( grid ),
fBZmax(0),
fBDY(0),
fIndYmin(0),
fIz(0),
fHitYlst( 0 ),
fIh( 0 ),
fNy( fGrid.Ny() ),
fNz( fGrid.Nz() ),
fBTmax(0),
fIt(0),
fBZmin(0),
fBYmin(0),
fIndZmin(0)
{
    const float minY = y - dy;
    const float maxY = y + dy;
    const float minZ = z - dz;
    const float maxZ = z + dz;
    const float minT = t - dt;
     


    const float maxT = t + dt;

    unsigned short bYmin, bZmin, bYmax, bTmin; // boundary bin indexes
    fGrid.GetBinBounded( minY, minZ, minT, bYmin, bZmin, bTmin );
    fGrid.GetBinBounded( maxY, maxZ, maxT, bYmax, fBZmax, fBTmax );
    fBZmin = bZmin;
    fBYmin = bYmin;
    
    
    fBDY = ( bYmax - bYmin + 1 ); // bin index span in y direction
    
    fIndYmin = (bTmin * fNy * fNz + bZmin * fNy + bYmin );
    
    fGrid.GetBinBounds( fIndYmin, y, dy, z, dz, t, dt );
    
    fIz = bZmin;
    
    fIt = bTmin;
    
    fIh = fGrid.FirstHitInBin( fIndYmin );

    fHitYlst = fGrid.FirstHitInBin( fIndYmin + fBDY );

}

inline bool L1HitAreaTime::GetNext( THitI& i )
{
    bool yIndexOutOfRange = fIh >= fHitYlst;     // current y is not in the area
    bool nextZIndexOutOfRange = (fIz >= fBZmax);   // there isn't any new z-line
    bool nextTIndexOutOfRange = (fIt >= fBTmax);   // there isn't any new z-line
    
    
    if ( yIndexOutOfRange && nextZIndexOutOfRange && nextTIndexOutOfRange) { // all iterators are over the end
        return false;
    }
    
    // at least one entry in the vector has (fIh >= fHitYlst && fIz < fBZmax)
    bool needNextZ = yIndexOutOfRange && !nextZIndexOutOfRange;
    bool needNextT = yIndexOutOfRange && nextZIndexOutOfRange && !nextTIndexOutOfRange;

    
    while ( ISLIKELY (( needNextZ ) ||( needNextT ))) {   //ISLIKELY to speed the programm and optimise the use of registers
        
        if (needNextT)    {
            fIt++;
            fIz = fBZmin;
            
            fIndYmin = (fIt * fNy * fNz + fBZmin * fNy + fBYmin );
            fIh = fGrid.FirstHitInBin( fIndYmin ); // get first hit in cell, if z-line is new
            fHitYlst = fGrid.FirstHitInBin( fIndYmin + fBDY );
        }
        else
        {
            
            fIz++;   // get new z-line
            // get next hit
            fIndYmin += fNy;
            fIh = fGrid.FirstHitInBin( fIndYmin ); // get first hit in cell, if z-line is new

            fHitYlst = fGrid.FirstHitInBin( fIndYmin + fBDY );
        }

        yIndexOutOfRange = fIh >= fHitYlst;
        nextZIndexOutOfRange = (fIz >= fBZmax);
        needNextZ = yIndexOutOfRange && !nextZIndexOutOfRange;
        
        nextTIndexOutOfRange = (fIt >= fBTmax);
        needNextT = yIndexOutOfRange && nextZIndexOutOfRange && !nextTIndexOutOfRange;
        
    }

    L1_ASSERT ( fIh < fGrid.FirstHitInBin(fGrid.N()+1) || yIndexOutOfRange, fIh << " < " << fGrid.FirstHitInBin(fGrid.N()+1) << " || " <<  yIndexOutOfRange);
    i = fIh; // return
    
    fIh++; // go to next
    return !yIndexOutOfRange;
}

#endif