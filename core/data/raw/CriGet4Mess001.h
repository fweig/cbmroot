/* Copyright (C) 2018-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifndef CRI_GET4_MESS_V0_01_DEF_H
#define CRI_GET4_MESS_V0_01_DEF_H

#include <iostream>

#include <stdint.h>

namespace critof001
{
  // Size of one clock cycle (=1 coarse bin)
  const double kdClockCycleSize   = 6250.0;                     //[ps]
  const double kdClockCycleSizeNs = kdClockCycleSize / 1000.0;  //[ns]
  // TODO:For now make 100ps default, maybe need later an option for it
  const double kdTotBinSize = 50.0;  //ps

  const uint32_t kuFineTime   = 0x0000007F;  // Fine Counter value
  const uint32_t kuFtShift    = 0;           // Fine Counter offset
  const uint32_t kuCoarseTime = 0x0007FF80;  // Coarse Counter value
  const uint32_t kuCtShift    = 7;           // Coarse Counter offset
  const uint32_t kuCtSize     = 12;          // Coarse Counter size in bits

  const uint32_t kuFineCounterSize    = ((kuFineTime >> kuFtShift) + 1);
  const uint32_t kuCoarseCounterSize  = ((kuCoarseTime >> kuCtShift) + 1);
  const uint32_t kuCoarseOverflowTest = kuCoarseCounterSize / 2;  // Limit for overflow check
  const uint32_t kuTotCounterSize     = 256;

  const double kdFtSize   = kuFineCounterSize;
  const double kdFtBinsNb = 112.;

  // Nominal bin size of NL are neglected
  const double kdBinSize = kdClockCycleSize / kdFtBinsNb;
  // Epoch Size in bins
  const uint32_t kuEpochInBins = kuCoarseCounterSize * kdFtBinsNb;
  // Epoch Size in ps
  // alternatively: (kiCoarseTime>>kiCtShift + 1)*kdClockCycleSize
  const double kdEpochInPs = static_cast<double>(kuCoarseCounterSize) * kdClockCycleSize;
  const double kdEpochInNs = kdEpochInPs / 1000.0;
  const double kuEpochInNs = static_cast<uint64_t>(kdEpochInNs); /// Works as epoch integer in ns!

  // Epoch counter size in epoch
  const uint32_t kuEpochCounterSz = 0xFFFFFF;
  // Epoch counter size in epoch
  const uint64_t kulEpochCycleEp = static_cast<uint64_t>(kuEpochCounterSz + 1);
  // Epoch counter size in bin
  const uint64_t kulEpochCycleBins = kulEpochCycleEp * kuEpochInBins;
  // Epoch counter size in ns (fits in 64b unsigned as epoch is an integer in ns)
  const uint64_t kulEpochCycleInNs = kulEpochCycleBins * kuEpochInNs;
  // Epoch counter size in s
  const double kdEpochCycleInS = static_cast<double>(kuEpochCounterSz + 1) * (kdEpochInNs / 1e9);

  // Epoch Cycle MS start message size in bits
  const uint64_t kulEpochCycleFieldSz = 0x1FFFFF;  // 21 bits

  const uint32_t kuChipIdMergedEpoch = 255;  // 0xFF

  const uint32_t kuFeePulserChannel     = 3;  // Channel where a pulser can be set ON at 20 ns 500 Hz
  const uint32_t kuFeePulserChannelDiam = 0;  // Channel where a pulser can be set ON at 20 ns 500 Hz

  const uint64_t kuEndOfMsMask   = 0xFFFFFFFFFFFF;
  const uint64_t kuEndOfMsMarker = 0xdeadbeeeeeef;

  enum MessageTypes
  {
    MSG_HIT        = 0,
    MSG_EPOCH      = 1,
    MSG_SLOWC      = 2,
    MSG_SYST       = 3
  };

  enum SysMessageTypes
  {
    SYS_GET4_ERROR     = 0,  // GET4 error event
    SYS_GDPB_UNKWN     = 1,  // Raw data from gDPB in case of unknown message type from GET4
    SYS_GET4_SYNC_MISS = 2,  // Added when GET4 is missing the SYNC flag when it is expected
    //      SYS_SYNC_ERROR     = 3  // added to data stream when the closy-sync-strobe does not match the gDPB 160MHz timestamp counter
    SYS_PATTERN = 3  // added to data stream when one of the ASIC patterns (missmatch, enable, resync) changed
  };

  enum PattMessageTypes
  {
    PATT_MISSMATCH = 0,  // Missmatch pattern, 1 bit per ASIC
    PATT_ENABLE    = 1,  // Enable pattern, 1 bit per ASIC
    PATT_RESYNC    = 2,  // Resync request pattern, 1 bit per ASIC
    PATT_STATUS    = 3   // Status pattern, 1 bit per ASIC (SW only)
  };

  enum MessagePrintMask
  {
    msg_print_Prefix = 1,
    msg_print_Data   = 2,
    msg_print_Hex    = 4,
    msg_print_Human  = 8
  };

  enum MessagePrintType
  {
    msg_print_Cout    = 1,
    msg_print_FairLog = 2,
    msg_print_File    = 3
  };

  enum Get4Message32bSlC
  {
    GET4_32B_SLC_SCALER    = 0,
    GET4_32B_SLC_DEADT     = 1,
    GET4_32B_SLC_SPIREAD   = 2,
    GET4_32B_SLC_START_SEU = 3
  };

  enum Get4Message32bErrors
  {
    GET4_V2X_ERR_READ_INIT   = 0x00,
    GET4_V2X_ERR_SYNC        = 0x01,
    GET4_V2X_ERR_EP_CNT_SYNC = 0x02,
    GET4_V2X_ERR_EP          = 0x03,
    GET4_V2X_ERR_FIFO_WRITE  = 0x04,
    GET4_V2X_ERR_LOST_EVT    = 0x05,
    GET4_V2X_ERR_CHAN_STATE  = 0x06,
    GET4_V2X_ERR_TOK_RING_ST = 0x07,
    GET4_V2X_ERR_TOKEN       = 0x08,
    GET4_V2X_ERR_READOUT_ERR = 0x09,
    GET4_V2X_ERR_SPI         = 0x0A,
    GET4_V2X_ERR_DLL_LOCK    = 0x0B,
    GET4_V2X_ERR_DLL_RESET   = 0x0C,
    GET4_V2X_ERR_TOT_OVERWRT = 0x11,  // Not there anymore in manual for Get4 v2.00?
    GET4_V2X_ERR_TOT_RANGE   = 0x12,
    GET4_V2X_ERR_EVT_DISCARD = 0x13,
    GET4_V2X_ERR_ADD_RIS_EDG = 0x14,
    GET4_V2X_ERR_UNPAIR_FALL = 0x15,
    GET4_V2X_ERR_SEQUENCE_ER = 0x16,
    GET4_V2X_ERR_EPOCH_OVERF = 0x17,  // New in manual for Get4 v2.00, no description?
    GET4_V2X_ERR_UNKNOWN     = 0x7F
  };

  class Message {

  protected:
    uint64_t data;  // main and only storage field for the message

  public:
    Message() : data(0) {}

    Message(const Message& src) : data(src.data) {}

    Message(uint64_t dataIn) : data(dataIn) {}

    ~Message() {};

    void assign(const Message& src) { data = src.data; }

    Message& operator=(const Message& src)
    {
      assign(src);
      return *this;
    }

    inline void reset() { data = 0; }

    inline uint64_t getData() const { return data; }
    inline void setData(uint64_t value) { data = value; }

    inline uint64_t getFieldLong(uint32_t shift, uint32_t len) const
    {
      return (data >> shift) & (((static_cast<uint64_t>(1)) << len) - 1);
    }

    inline uint32_t getField(uint32_t shift, uint32_t len) const
    {
      return (data >> shift) & (((static_cast<uint64_t>(1)) << len) - 1);
    }

    inline void setField(uint32_t shift, uint32_t len, uint32_t value)
    {
      uint64_t mask = (((static_cast<uint64_t>(1)) << len) - 1);
      data          = (data & ~(mask << shift)) | ((static_cast<uint64_t>(value) & mask) << shift);
    }

    inline void setFieldLong(uint32_t shift, uint32_t len, uint64_t value)
    {
      uint64_t mask = (((static_cast<uint64_t>(1)) << len) - 1);
      data          = (data & ~(mask << shift)) | ((value & mask) << shift);
    }

    inline uint8_t getBit(uint32_t shift) const { return (data >> shift) & 1; }

    inline void setBit(uint32_t shift, uint8_t value)
    {
      data = value ? (data | ((static_cast<uint64_t>(1)) << shift)) : (data & ~((static_cast<uint64_t>(1)) << shift));
    }


    inline uint32_t getFieldBE(uint32_t shift, uint32_t len) const
    {
      return (dataBE() >> shift) & (((static_cast<uint32_t>(1)) << len) - 1);
    }
    inline uint8_t getBitBE(uint32_t shift) const { return (dataBE() >> shift) & 1; }
    inline uint64_t dataBE() const
    {
      return ((data & 0x00000000000000FF) << 56) + ((data & 0x000000000000FF00) << 40)
             + ((data & 0x0000000000FF0000) << 24) + ((data & 0x00000000FF000000) << 8)
             + ((data >> 8) & 0x00000000FF000000) + ((data >> 24) & 0x0000000000FF0000)
             + ((data >> 40) & 0x000000000000FF00) + ((data >> 56) & 0x00000000000000FF);
    }

    // --------------------------- common fields ---------------------------------

    //! Returns the message type. Valid for all message types. 2 bit
    inline uint8_t getMessageType() const { return getField(0, 4); }

    //! Sets the message type field in the current message
    inline void setMessageType(uint8_t v) { setField(0, 4, v); }

    // ---------- All types access methods ------------------------------------
    inline uint16_t getDebugTimer() const { return getField(48, 16); }
    inline void setDebugTimer(uint32_t v) { setField(48, 16, v); }
    inline uint16_t getGet4Idx() const { return getField(40, 8); }
    inline void setGet4Id(uint32_t v) { setField(40, 8, v); }

    // ---------- Get4 Hit access methods -------------------------------------
    inline uint16_t getGdpbHitChanId() const { return getField(32, 2); }
    inline uint32_t getGdpbHitFullTs() const { return getField(13, 19); }
    inline bool getGdpbHit32DllLck() const { return getBit(12); }
    inline uint16_t getGdpbHit32Tot() const { return getField(4, 8); }

    // ---------- Get4 Epoch access methods ----------------------------------
    inline uint32_t getGdpbEpEpochNb() const { return getField(8, 24); }
    inline bool getGdpbEpSync() const { return getBit(7); }
    inline bool getGdpbEpDataLoss() const { return getBit(6); }
    inline bool getGdpbEpEpochLoss() const { return getBit(5); }
    inline bool getGdpbEpMissmatch() const { return getBit(4); }

    // ---------- Get4 gDPB 24b/32b Slow cont. access methods -----------------
    inline uint32_t getGdpbSlcMess() const { return getField(4, 29); }
    inline uint32_t getGdpbSlcChan() const { return getField(31, 2); }
    inline uint32_t getGdpbSlcEdge() const { return getBit(30); }
    inline uint32_t getGdpbSlcType() const { return getField(28, 2); }
    inline uint32_t getGdpbSlcData() const { return getField(4, 24); }

    // ---------- Get4 gDPB System Msg access methods -------------------------
    inline uint16_t getGdpbSysSubType() const { return getField(38, 2); }
    inline bool getGdpbSysLinkId() const { return getBit(37); }
    // ---------- Get4 gDPB 24b/32b Errors access methods ---------------------
    inline bool getGdpbSysErrRoType() const { return getBit(36); }
    inline uint16_t getGdpbSysErrUnused() const { return getField(32, 4); }
    inline uint16_t getGdpbSysErrInfo() const { return getField(11, 21); }
    inline uint16_t getGdpbSysErrChanId() const { return getField(12, 2); }
    inline bool getGdpbSysErrEdge() const { return getBit(11); }
    inline uint16_t getGdpbSysErrData() const { return getField(4, 7); }
    // ---------- Get4 gDPB unknown msg type access methods -------------------
    inline uint32_t getGdpbSysUnkwData() const { return getField(4, 32); }
    // ---------- FW error msg type access methods ----------------------------
    inline uint32_t getGdpbSysFwErrResync() const { return getBit(36); }
    // ---------- ASIC Pattern messages access methods ------------------------
    inline uint16_t getGdpbSysPattType() const { return getField(46, 2); }
    inline uint16_t getGdpbSysPattIndex() const { return getField(40, 4); }
    inline uint32_t getGdpbSysPattPattern() const { return getField(4, 32); }

    // ---------- Get4 gDPB 24b/32b Epoch setter methods ----------------------
    inline void setGdpbEpEpochNb(uint32_t v) { setField(8, 31, v); }

    // ---------- Get4 gDPB System Msg access methods -------------------------
    inline void setGdpbSysSubType(uint16_t v) { setField(38, 2, v); }
    // ---------- ASIC Pattern messages access methods ------------------------
    inline void setGdpbSysPattType(uint16_t v) { setField(46, 2, v); }
    inline void setGdpbSysPattIndex(uint16_t v) { setField(40, 4, v); }
    inline void setGdpbSysPattPattern(uint32_t v) { setField(4, 32, v); }

    // ---------- Common functions -----------------------
    //! Returns \a true is message type is #MSG_HIT (Get4 hit data)
    inline bool isHitMsg() const { return getMessageType() == MSG_HIT; }
    //! Returns \a true is message type is #MSG_EPOCH (epoch2 marker)
    inline bool isEpochMsg() const { return getMessageType() == MSG_EPOCH; }
    //! Returns \a true is message type is #MSG_SLOWC (GET4 Slow Control)
    inline bool isGet4SlCtrMsg() const { return getMessageType() == MSG_SLOWC; }
    //! Returns \a true is message type is #MSG_ERROR (error message)
    inline bool isSystem() const { return getMessageType() == MSG_SYST; }
    //! Returns \a true is message type is an EndOfMs marker
    inline bool isEndOfMs() const { return kuEndOfMsMarker == (data & kuEndOfMsMask); }

    void printDataCout(unsigned kind = msg_print_Prefix | msg_print_Data, uint32_t epoch = 0) const;
    void printDataLog(unsigned kind = msg_print_Prefix | msg_print_Data, uint32_t epoch = 0) const;

    void printData(unsigned outType = msg_print_Cout, unsigned kind = msg_print_Human, uint32_t epoch = 0,
                   std::ostream& os = std::cout) const;

    uint64_t getMsgFullTime(uint64_t epoch) const;

    double getMsgFullTimeD(uint64_t epoch) const;

    //! Expanded timestamp for 160 MHz * 19 bit (12 + 7) epochs
    inline static uint64_t FullTimeStamp(uint64_t epoch, uint32_t ts) { return (epoch << 19) | (ts & 0x7ffff); }


    static uint64_t CalcDistance(uint64_t start, uint64_t stop);

    static double CalcDistanceD(double start, double stop);

    bool operator<(const critof001::Message& other) const;
    bool operator==(const critof001::Message& other) const;
    bool operator!=(const critof001::Message& other) const;
  };

  class FullMessage : public Message {
  protected:
    uint64_t fulExtendedEpoch;  // Epoch of the message, extended with 32b epoch cycle counter

  public:
    FullMessage() : Message(), fulExtendedEpoch(0) {}

    FullMessage(const Message& src, uint64_t uEpIn = 0) : Message(src), fulExtendedEpoch(uEpIn) {}

    FullMessage(const FullMessage& src) : Message(src), fulExtendedEpoch(src.fulExtendedEpoch) {}

    void assign(const FullMessage& src)
    {
      Message::assign(src);
      fulExtendedEpoch = src.fulExtendedEpoch;
    }

    FullMessage& operator=(const FullMessage& src)
    {
      assign(src);
      return *this;
    }

    bool operator<(const FullMessage& other) const;

    inline void reset()
    {
      Message::reset();
      fulExtendedEpoch = 0;
    }

    inline uint64_t getExtendedEpoch() const { return fulExtendedEpoch; }

    inline double GetFullTimeNs() const { return getMsgFullTimeD(fulExtendedEpoch); }

    void PrintMessage(unsigned outType = msg_print_Cout, unsigned kind = msg_print_Human) const;
  };

}  // namespace critof001


#endif  // CRI_GET4_MESS_V0_01_DEF_H
