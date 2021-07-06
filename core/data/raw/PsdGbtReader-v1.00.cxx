// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                       PsdGbtDataReader                            -----
// -----              Created 14.09.2019 by N.Karpushkin                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "PsdGbtReader-v1.00.h"

namespace PsdDataV100
{

  PsdGbtReader::~PsdGbtReader()
  {
    MsHdr.clear();
    PackHdr.clear();
    HitHdr.clear();
    HitData.clear();
    VectPackHdr.clear();
    VectHitHdr.clear();
    VectHitData.clear();
  }

  void PsdGbtReader::ReadMsHeader()
  {
    MsHdr.clear();
    save_buffer << std::hex << std::setfill('0') << std::setw(16) << buffer[word_index] << std::endl
                << std::setfill('0') << std::setw(16) << buffer[word_index + 1] << std::endl;

    MsHdr.uMagicWord   = (buffer[word_index] >> 32) & 0xff;
    MsHdr.ulMicroSlice = ((buffer[word_index] & 0xffffff) << 40) | (buffer[word_index + 1] & 0xffffffffff);
    word_index += 2;

    if (print) MsHdr.printout();
  }

  void PsdGbtReader::ReadPackHeader()
  {
    PackHdr.clear();
    save_buffer << std::hex << std::setfill('0') << std::setw(16) << buffer[word_index] << std::endl;

    buffer_shift        = 0;
    PackHdr.uHitsNumber = (buffer[word_index] >> buffer_shift) & (((static_cast<uint16_t>(1)) << PackHdr.HNs) - 1);
    buffer_shift += PackHdr.HNs + PackHdr.E0s;
    PackHdr.uLinkIndex = (buffer[word_index] >> buffer_shift) & (((static_cast<uint16_t>(1)) << PackHdr.LIs) - 1);
    buffer_shift += PackHdr.LIs;
    PackHdr.uMagicWord = (buffer[word_index] >> buffer_shift) & (((static_cast<uint16_t>(1)) << PackHdr.MWs) - 1);
    word_index++;

    if (PackHdr.uMagicWord != 0xb) {
      if (print) PackHdr.printout();
      return;
    }

    save_buffer << std::hex << std::setfill('0') << std::setw(16) << buffer[word_index] << std::endl;
    buffer_shift     = 0;
    PackHdr.uAdcTime = (buffer[word_index] >> buffer_shift) & (((static_cast<uint64_t>(1)) << PackHdr.TMs) - 1);
    buffer_shift += PackHdr.TMs;
    PackHdr.uTotalWords = (buffer[word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << PackHdr.TWs) - 1);
    word_index++;

    if (print) PackHdr.printout();
  }

  void PsdGbtReader::ReadHitHeader()
  {
    HitHdr.clear();
    save_buffer << std::hex << std::setfill('0') << std::setw(16) << buffer[word_index] << std::endl
                << std::setfill('0') << std::setw(16) << buffer[word_index + 1] << std::endl;

    buffer_shift     = 8;
    HitHdr.uFeeAccum = (buffer[word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << HitHdr.FAs) - 1);
    buffer_shift += HitHdr.FAs;
    HitHdr.uWfmWords = (buffer[word_index] >> buffer_shift) & (((static_cast<uint16_t>(1)) << HitHdr.WWs) - 1);
    buffer_shift += HitHdr.WWs;
    HitHdr.uHitChannel = (buffer[word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << HitHdr.HCs) - 1);
    word_index++;

    buffer_shift      = 0;
    HitHdr.uZeroLevel = (buffer[word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << HitHdr.ZLs) - 1);
    buffer_shift += HitHdr.ZLs;
    HitHdr.uSignalCharge = (buffer[word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << HitHdr.SCs) - 1);
    word_index++;

    if (print) HitHdr.printout();
  }

  void PsdGbtReader::ReadHitData()
  {
    save_buffer << std::hex << std::setfill('0') << std::setw(16) << buffer[word_index] << std::endl;

    uint16_t wfm_point = 0;
    wfm_point          = ((buffer[word_index] >> 8) & 0xffff);
    HitData.uWfm.push_back(wfm_point);
    wfm_point = ((buffer[word_index] & 0xff) << 8) | ((buffer[word_index + 1] >> 32) & 0xff);
    HitData.uWfm.push_back(wfm_point);
    word_index++;

    save_buffer << std::hex << std::setfill('0') << std::setw(16) << buffer[word_index] << std::endl;
    wfm_point = ((buffer[word_index] >> 16) & 0xffff);
    HitData.uWfm.push_back(wfm_point);
    wfm_point = (buffer[word_index] & 0xffff);
    HitData.uWfm.push_back(wfm_point);
    word_index++;
  }

  void PsdGbtReader::ReadMsTrailer()
  {
    save_buffer << std::hex << std::setfill('0') << std::setw(16) << buffer[word_index] << std::endl;

    MsTrlr.uEmpty0 = buffer[word_index];
    word_index++;

    if (print) MsTrlr.printout();
  }

  int PsdGbtReader::ReadMs()
  {
    save_buffer.str("");
    save_buffer.clear();

    //bool word_is_Ms_header = false;
    //ReadMsHeader();
    //word_is_Ms_header = (MsHdr.uMagicWord == 0xa0);

    //if(word_is_Ms_header) { ms_hdrs_read++; }
    //else { words_missed++; return 4; }

    bool word_is_Pack_header = true;
    VectPackHdr.clear();
    VectHitHdr.clear();
    VectHitData.clear();

    while (word_is_Pack_header) {
      ReadPackHeader();

      if (PackHdr.uMagicWord != 0xb) {
        word_is_Pack_header = false;
        if (print) printf("End of microslice\n");
        word_index -= 1;
        break;  //return 1;
      }
      else {
        //hit loop
        for (int hit_iter = 0; hit_iter < PackHdr.uHitsNumber; hit_iter++) {
          ReadHitHeader();
          if (HitHdr.uHitChannel > 32) return 2;

          VectHitHdr.emplace_back(HitHdr);
          VectPackHdr.emplace_back(PackHdr);  //for convenient use of uAdcTime with each hit

          HitData.clear();
          if (HitHdr.uWfmWords > 10) return 3;
          for (int wfm_word_iter = 0; wfm_word_iter < HitHdr.uWfmWords - 1; wfm_word_iter++)
            ReadHitData();

          VectHitData.emplace_back(HitData);
          if (print) HitData.printout();

        }  //hit loop
      }
    }

    ReadMsTrailer();
    if (MsTrlr.uEmpty0 == 0) ms_ends_read++;
    //else

    return 0;
  }


  void PsdGbtReader::PrintSaveBuff() { printf("%s\n", save_buffer.str().c_str()); }

  void PsdGbtReader::PrintOut()
  {
    MsHdr.printout();
    for (int hit_iter = 0; hit_iter < (int) VectPackHdr.size(); hit_iter++) {
      VectPackHdr.at(hit_iter).printout();
      VectHitHdr.at(hit_iter).printout();
      VectHitData.at(hit_iter).printout();
    }
    PackHdr.printout();
    MsTrlr.printout();
  }


}  // namespace PsdDataV100
