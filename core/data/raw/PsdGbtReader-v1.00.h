// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                       PsdGbtDataReader                            -----
// -----              Created 14.09.2019 by N.Karpushkin                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef PSD_GBT_READER_V100_H
#define PSD_GBT_READER_V100_H

#include <cstdint>  // for uint16_t, uint64_t, uint32_t
#include <iomanip>  // for setw, setfill
#include <sstream>  // for sstream
#include <string>   // for string
#include <vector>   // for vector

#include <stdint.h>  // for uint64_t, uint32_t

#include "PsdGbtDataFormat-v1.00.h"
namespace PsdDataV100
{

  class PsdGbtReader {
  public:
    PsdGbtReader() {};
    PsdGbtReader(const uint64_t* input)
    {
      buffer     = input;
      word_index = 0;
    }

    void SetInput(const uint64_t* input)
    {
      buffer     = input;
      word_index = 0;
    }
    std::stringstream save_buffer;

    struct PsdMsHeader MsHdr;
    struct PsdPackHeader PackHdr;
    struct PsdHitHeader HitHdr;
    struct PsdHitData HitData;
    struct PsdMsTrailer MsTrlr;

    std::vector<struct PsdPackHeader> VectPackHdr;
    std::vector<struct PsdHitHeader> VectHitHdr;
    std::vector<struct PsdHitData> VectHitData;

    void ReadMsHeader();
    void ReadPackHeader();
    void ReadHitHeader();
    void ReadHitData();
    void ReadMsTrailer();
    int ReadMs();

    void PrintSaveBuff();
    void PrintOut();

    //Getters
    uint32_t GetTotalGbtWordsRead() { return word_index; }

    void SetPrintOutMode(bool mode) { print = mode; }
    ~PsdGbtReader();

    int word_index   = 0;
    int words_missed = 0;
    int ms_hdrs_read = 0;
    int ms_ends_read = 0;

  private:
    const uint64_t* buffer;

    bool print       = false;
    int buffer_shift = 0;
  };
}  // namespace PsdDataV100

#endif  // PSD_GBT_READER_V100_H
