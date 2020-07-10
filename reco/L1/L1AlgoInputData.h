#ifndef _L1AlgoInputData_h
#define _L1AlgoInputData_h

#include "CbmL1Def.h"
#include "L1Strip.h"
#include "L1StsHit.h"

#include <fstream>
#include <iostream>
#include <vector>

using std::istream;
using std::vector;

class L1AlgoInputData {

public:
  L1AlgoInputData()
    : vStsHits()
    , vStsStrips()
    , vStsStripsB()
    , vStsZPos()
    , vSFlag()
    , vSFlagB()
  //  MaxNStations(12)

  {
    for (int i = 0; i < MaxNStations + 1; ++i)
      StsHitsStartIndex[i] = 0;
    for (int i = 0; i < MaxNStations + 1; ++i)
      StsHitsStopIndex[i] = 0;
  }
  ~L1AlgoInputData() {};

  /*
const  vector< L1StsHit >      & GetStsHits() const { return vStsHits; }
  vector< L1Strip >       & GetStsStrips()   { return vStsStrips; }
  vector< L1Strip >       & GetStsStripsB()  { return vStsStripsB; }
  vector< fscal >         & GetStsZPos()     { return vStsZPos; }
  vector< unsigned char > & GetSFlag()       { return vSFlag; }
  vector< unsigned char > & GetSFlagB()      { return vSFlagB; }
  THitI* GetStsHitsStartIndex()  { return StsHitsStartIndex; }
  THitI* GetStsHitsStopIndex()   { return StsHitsStopIndex; }

*/


  const vector<L1StsHit>& GetStsHits() const { return vStsHits; }
  const vector<L1Strip>& GetStsStrips() const { return vStsStrips; }
  const vector<L1Strip>& GetStsStripsB() const { return vStsStripsB; }
  const vector<fscal>& GetStsZPos() const { return vStsZPos; }
  const L1Vector<unsigned char>& GetSFlag() const { return vSFlag; }
  const L1Vector<unsigned char>& GetSFlagB() const { return vSFlagB; }
  const THitI* GetStsHitsStartIndex() const { return StsHitsStartIndex; }
  const THitI* GetStsHitsStopIndex() const { return StsHitsStopIndex; }


  bool ReadHitsFromFile(const char work_dir[100],
                        const int maxNEvent,
                        const int iVerbose);
  //  void PrintHits();

  /// redefine new\delete for use alignment memmory
  void* operator new(size_t size, void* ptr) {
    return ::operator new(size, ptr);
  }
  void* operator new[](size_t size, void* ptr) {
    return ::operator new(size, ptr);
  }
  void* operator new(size_t size) { return _mm_malloc(size, 16); }
  void* operator new[](size_t size) { return _mm_malloc(size, 16); }
  void operator delete(void* ptr, size_t) { _mm_free(ptr); }
  void operator delete[](void* ptr, size_t) { _mm_free(ptr); }

  L1AlgoInputData(const L1AlgoInputData& a);
  const L1AlgoInputData& operator=(const L1AlgoInputData& a);


  void Clear() {

    vStsHits.clear();
    vStsStrips.clear();
    vStsStripsB.clear();
    vStsZPos.clear();
    vSFlag.clear();
    vSFlagB.clear();

    {
      for (int i = 0; i < MaxNStations + 1; ++i)
        StsHitsStartIndex[i] = 0;
      for (int i = 0; i < MaxNStations + 1; ++i)
        StsHitsStopIndex[i] = 0;
    }
  }


  // private:
  // functionality
  static istream& eatwhite(istream& is);  // skip spaces
                                          /// read data from data_algo.txt
  // data
  enum { MaxNStations = 25 };
  vector<L1StsHit>
    vStsHits;  // hits as a combination of front-, backstrips and z-position
  vector<L1Strip>
    vStsStrips,            // strips positions created from hits. Front strips
    vStsStripsB;           // back strips
  vector<fscal> vStsZPos;  // all possible z-positions of hits

  L1Vector<unsigned char>
    vSFlag,  // information of hits station & using hits in tracks;
    vSFlagB;
  THitI StsHitsStartIndex[MaxNStations + 1],
    StsHitsStopIndex[MaxNStations + 1];  // station-bounders in vStsHits array

} _fvecalignment;

#endif  // #ifndef _L1AlgoInputData_h
