/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer], Valentina Akishina */

#ifndef _L1AlgoInputData_h
#define _L1AlgoInputData_h

#include <fstream>
#include <iostream>

#include "L1Def.h"
#include "L1Hit.h"
#include "L1Vector.h"

using std::istream;

class L1AlgoInputData {

public:
  L1AlgoInputData()  = default;
  ~L1AlgoInputData() = default;

  L1Vector<L1Hit>& GetStsHits() { return vStsHits; }
  int GetNStsStrips() const { return NStsStrips; }
  L1Vector<fscal>& GetStsZPos() { return vStsZPos; }
  L1Vector<unsigned char>& GetSFlag() { return fStripFlag; }
  const THitI* GetStsHitsStartIndex() const { return StsHitsStartIndex; }
  const THitI* GetStsHitsStopIndex() const { return StsHitsStopIndex; }


  bool ReadHitsFromFile(const char work_dir[100], const int maxNEvent, const int iVerbose);
  //  void PrintHits();

  /// redefine new\delete for use alignment memmory
  void* operator new(size_t size, void* ptr) { return ::operator new(size, ptr); }
  void* operator new[](size_t size, void* ptr) { return ::operator new(size, ptr); }
  void* operator new(size_t size) { return _mm_malloc(size, 16); }
  void* operator new[](size_t size) { return _mm_malloc(size, 16); }
  void operator delete(void* ptr, size_t) { _mm_free(ptr); }
  void operator delete[](void* ptr, size_t) { _mm_free(ptr); }

  L1AlgoInputData(const L1AlgoInputData& a);
  const L1AlgoInputData& operator=(const L1AlgoInputData& a);


  void Clear()
  {

    vStsHits.clear();
    NStsStrips = 0;
    vStsZPos.clear();
    fStripFlag.clear();

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
  enum
  {
    MaxNStations = 25
  };
  L1Vector<L1Hit> vStsHits {"L1AlgoInputData::vStsHits"};  // hits as a combination of front-, backstrips and z-position
  int NStsStrips {0};                                      // Number of strips in sts
  L1Vector<fscal> vStsZPos {"L1AlgoInputData::vStsZPos"};  // all possible z-positions of hits

  L1Vector<unsigned char> fStripFlag {
    "L1AlgoInputData::fStripFlag"};  // information of hits station & using hits in tracks;

  THitI StsHitsStartIndex[MaxNStations + 1] {0};  // station-bounders in vStsHits array
  THitI StsHitsStopIndex[MaxNStations + 1] {0};   // station-bounders in vStsHits array

} _fvecalignment;

#endif  // #ifndef _L1AlgoInputData_h
