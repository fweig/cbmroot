/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, David Emschermann [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                       PsdGbtDataFormat                            -----
// -----              Created 14.09.2019 by N.Karpushkin                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef PSD_GBT_DATA_FORMAT_H_
#define PSD_GBT_DATA_FORMAT_H_

#include <vector>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace PsdData
{

  struct PsdEventHeaderAB {
    enum bitFieldSizes
    {
      MWs = 8,  //! MagicWord size in bits
      HNs = 8,  //! HitsNumber size in bits
      MSs = 64  //! MicroSlice size in bits
    };

    uint8_t uMagicWordAB : MWs;   //! Should be AB
    uint8_t uHitsNumber : HNs;    //! Total number of hits
    uint64_t ulMicroSlice : MSs;  //! Epoch

    void printout()
    {
      printf("magic word AB: %u; hits number: %u; microslice: %llu\n", uMagicWordAB, uHitsNumber,
             static_cast<long long unsigned int>(ulMicroSlice));
    }

    void clear()
    {
      uMagicWordAB = 0;
      uHitsNumber  = 0;
      ulMicroSlice = 0;
    }

    PsdEventHeaderAB() { clear(); }
  };


  struct PsdEventHeaderAC {
    enum bitFieldSizes
    {
      MWs = 8,   //! MagicWord size in bits
      PVs = 8,   //! PacketVersion size in bits
      E0s = 32,  //! Empty bits size in bits
      TMs = 32   //! ADC Time size in bits
    };

    uint8_t uMagicWordAC : MWs;    //! Should be AC
    uint8_t uPacketVersion : PVs;  //! Version of gbt package
    uint32_t uEmpty0 : E0s;        //! Empty bits
    uint32_t uAdcTime : TMs;       //! ADC Time of threshold cross from the begining of TS

    void printout()
    {
      printf("magic word AC: %u; packet version: %u; ADC time in microslice: %u\n", uMagicWordAC, uPacketVersion,
             uAdcTime);
    }

    void clear()
    {
      uMagicWordAC   = 0;
      uPacketVersion = 0;
      uEmpty0        = 0;
      uAdcTime       = 0;
    }

    PsdEventHeaderAC() { clear(); }
  };


  struct PsdHitHeader {
    enum bitFieldSizes
    {
      WPSs = 8,   //! Waveform points size in bits
      HCs  = 8,   //! Hit channel size in bits
      E0s  = 32,  //! Empty bits size in bits
      SCs  = 16,  //! Signal charge size in bits
      ZLs  = 16   //! ZeroLevel size in bits
    };

    uint8_t uWfmPoints : WPSs;     //! Total waveform points per hit
    uint8_t uHitChannel : HCs;     //! Hit channel
    uint32_t uEmpty0 : E0s;        //! Empty bits
    uint16_t uSignalCharge : SCs;  //! Waveform integral above ZeroLevel
    uint16_t uZeroLevel : ZLs;     //! Waveform ZeroLevel

    void printout()
    {
      printf("waveform points: %u; hit channel: %u; signal charge: %u; zero "
             "level: %u\n",
             uWfmPoints, uHitChannel, uSignalCharge, uZeroLevel);
    }

    void clear()
    {
      uWfmPoints    = 0;
      uHitChannel   = 0;
      uEmpty0       = 0;
      uSignalCharge = 0;
      uZeroLevel    = 0;
    }

    PsdHitHeader() { clear(); }
  };


  struct PsdHitData {
    enum bitFieldSizes
    {
      E0s = 16,  //! Empty bits size in bits
      WPs = 16   //! Waveform point size in bits
    };

    uint16_t uEmpty0 : E0s;      //! Empty bits
    std::vector<uint16_t> uWfm;  //! Waveform vector

    void printout()
    {
      printf("waveform: ");
      for (uint8_t iter = 0; iter < uWfm.size(); iter++)
        printf("%u ", uWfm.at(iter));
      printf("\n");
    }

    void clear()
    {
      uEmpty0 = 0;
      uWfm.clear();
    }

    PsdHitData() { clear(); }
  };

}  // namespace PsdData


#endif /* PSD_GBT_DATA_FORMAT_H_ */
