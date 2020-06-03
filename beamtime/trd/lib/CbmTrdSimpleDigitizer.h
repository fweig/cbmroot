#ifndef CBMTRDSIMPLEDIGITIZER_H
#define CBMTRDSIMPLEDIGITIZER_H


#include "CbmTrdQABase.h"
#include <boost/circular_buffer.hpp>
#include <cstdint>
#include <algorithm>
#include <cmath>

class CbmTrdSimpleDigitizer : public CbmTrdQABase
{
  void StatelessDigitizer();
 protected:
  std::vector<CbmTrdDigi>* fDigis;
 public:
  CbmTrdSimpleDigitizer();
  CbmTrdSimpleDigitizer(const CbmTrdSimpleDigitizer&) = delete;
  CbmTrdSimpleDigitizer& operator=(const CbmTrdSimpleDigitizer&) = delete;
  //  ~CbmTrdSimpleDigitizer();
  InitStatus Init();
  void Exec(Option_t*);
  void CreateHistograms();
    
ClassDef(CbmTrdSimpleDigitizer,2);
};

#endif

