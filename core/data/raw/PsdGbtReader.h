// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                       PsdGbtDataReader                            -----
// -----              Created 14.09.2019 by N.Karpushkin                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef PSD_GBT_READER_H_
#define PSD_GBT_READER_H_

#include <stdint.h>  // for uint64_t, uint32_t
#include <vector>    // for vector

#include "PsdGbtDataFormat.h"  // for PsdHitData, PsdHitHeader, PsdEventHead...

namespace PsdData {
  class PsdGbtReader {

  public:
    PsdGbtReader() {};
    PsdGbtReader(const uint64_t* input) {
      buffer         = input;
      gbt_word_index = 0;
    }

    ~PsdGbtReader();

    PsdEventHeaderAB EvHdrAb;
    PsdEventHeaderAC EvHdrAc;
    PsdHitHeader HitHdr;
    PsdHitData HitData;

    std::vector<PsdHitHeader> VectHitHdr;
    std::vector<PsdHitData> VectHitData;

    void SetInput(const uint64_t* input) {
      buffer         = input;
      gbt_word_index = 0;
    }
    void SetPrintOutMode(bool mode) { PrintOut = mode; }
    void ReadEventHeaderAbFles();
    void ReadEventHeaderAcFles();
    void ReadHitHeaderFles();
    void ReadHitDataFles();
    int ReadEventFles();

    //Getters
    uint32_t GetTotalGbtWordsRead() { return gbt_word_index; }

  private:
    const uint64_t* buffer;

    bool PrintOut           = false;
    uint32_t gbt_word_index = 0;
    int buffer_shift        = 0;
  };
}  // namespace PsdData

#endif  // PSD_GBT_READER_H_
