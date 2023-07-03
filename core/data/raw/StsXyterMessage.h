/* Copyright (C) 2017-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/************************************************************
 *
 ************************************************************/

#ifndef STSXYTERMESSAGE_H
#define STSXYTERMESSAGE_H

// C/C++ headers
#include <cstdint>
#include <iostream>

#include <xpu/device.h>

namespace stsxyter
{

// tools: Use to this allow bitwise operations on C++11 enums
// This needs to be included in the same namespace to work...
#include "bitmask_operators.hpp"

  /// Constants and other auxiliary stuff
  /// Field full info object
  struct MessField {
    uint16_t fusPos;  //! Start bit index
    uint16_t fusLen;  //! Field length

    constexpr MessField(uint16_t usPos, uint16_t usLen) : fusPos(usPos), fusLen(usLen) {};
  };
  /// Message types
  enum class MessType : uint16_t
  {
    Dummy,
    Hit,
    TsMsb,
    Epoch,
    Status,
    Empty,
    EndOfMs
  };

  /// Non-hit Message sub-types
  enum class MessSubType : uint16_t
  {
    TsMsb  = 0,
    Epoch  = 1,
    Status = 2,
    Empty  = 3
  };
  /// Printout control
  enum class MessagePrintMask : uint16_t
  {
    msg_print_Prefix = (0x1 << 0),
    msg_print_Data   = (0x1 << 1),
    msg_print_Hex    = (0x1 << 2),
    msg_print_Human  = (0x1 << 3)
  };
  ENABLE_BITMASK_OPERATORS(stsxyter::MessagePrintMask)  // Preproc macro!
                                                        /// MS error flags
  enum class MsErrorFlags : uint16_t
  {
    MsErrOutFifoAlmostFull = (0x1 << 0),
    MsErrOutFifoOverflow   = (0x1 << 1),
    MsErrTimeoutBinReadout = (0x1 << 2),
    MsErrBinOverflow       = (0x1 << 3)
  };

  /// Fields position (Start bit index)
  static constexpr uint16_t kusPosNotHitFlag = 31;
  static constexpr uint16_t kusPosLinkIndex  = 22;
  // Hit message
  static constexpr uint16_t kusPosHitChannel = 15;
  static constexpr uint16_t kusPosHitAdc     = 10;
  static constexpr uint16_t kusPosHitTsOver  = 9;
  static constexpr uint16_t kusPosHitTs      = 1;
  static constexpr uint16_t kusPosHitEmFlag  = 0;
  // Non-hit messages
  static constexpr uint16_t kusPosSubtype = 29;
  // TS_MSB message
  static constexpr uint16_t kusPosTsMsbVal = 0;
  // Epoch
  static constexpr uint16_t kusPosEpochVal = 0;
  // Status
  static constexpr uint16_t kusPosStatLinkId = 20;
  static constexpr uint16_t kusPosStatSxTs   = 14;
  static constexpr uint16_t kusPosStatStatus = 10;
  static constexpr uint16_t kusPosStatDpbTs  = 1;
  static constexpr uint16_t kusPosStatCpFlag = 0;
  // Empty/End of MS
  static constexpr uint16_t kusPosEmptyFlag = 28;
  static constexpr uint16_t kusPosMsErrType = 1;
  static constexpr uint16_t kusPosMsErrFlag = 0;

  /// Fields length (bits)
  static constexpr uint16_t kusLenNotHitFlag = 1;
  static constexpr uint16_t kusLenLinkIndex  = 9;
  // Hit message
  static constexpr uint16_t kusLenHitChannel = 7;
  static constexpr uint16_t kusLenHitAdc     = 5;
  static constexpr uint16_t kusLenHitTsFull  = 9;  // Includes 1 bit overlap with TS message ?
  static constexpr uint16_t kusLenHitTsOver  = 0;  // 0 bit overlap with TS message
  static constexpr uint16_t kusLenHitTs      = 9;  // No overlap in this version of FW
  static constexpr uint16_t kusLenHitEmFlag  = 1;
  // Other message
  static constexpr uint16_t kusLenSubtype = 2;
  // TS_MSB message
  static constexpr uint16_t kusLenTsMsbVal        = 22;
  static constexpr uint16_t kusLenTsMsbValBinning = 29;
  // Epoch
  static constexpr uint16_t kusLenEpochVal = 29;
  // Status
  static constexpr uint16_t kusLenStatLinkId = 9;
  static constexpr uint16_t kusLenStatSxTs   = 6;
  static constexpr uint16_t kusLenStatStatus = 4;
  static constexpr uint16_t kusLenStatDpbTs  = 9;
  static constexpr uint16_t kusLenStatCpFlag = 1;
  // Empty/End of MS
  static constexpr uint16_t kusLenEmptyFlag = 1;
  static constexpr uint16_t kusLenMsErrType = 4;
  static constexpr uint16_t kusLenMsErrFlag = 1;

  /// Fields Info
  static constexpr MessField kFieldLinkIndex(kusPosLinkIndex, kusLenLinkIndex);
  static constexpr MessField kFieldNotHitFlag(kusPosNotHitFlag, kusLenNotHitFlag);
  // Hit message
  static constexpr MessField kFieldHitChannel(kusPosHitChannel, kusLenHitChannel);
  static constexpr MessField kFieldHitAdc(kusPosHitAdc, kusLenHitAdc);
  static constexpr MessField kFieldHitTsFull(kusPosHitTs, kusLenHitTsFull);
  static constexpr MessField kFieldHitTsOver(kusPosHitTsOver, kusLenHitTsOver);
  static constexpr MessField kFieldHitTs(kusPosHitTs, kusLenHitTs);
  static constexpr MessField kFieldHitEmFlag(kusPosHitEmFlag, kusLenHitEmFlag);
  // Non-hit messages
  static constexpr MessField kFieldSubtype(kusPosSubtype, kusLenSubtype);
  // TS_MSB message
  static constexpr MessField kFieldTsMsbVal(kusPosTsMsbVal, kusLenTsMsbVal);
  static constexpr MessField kFieldTsMsbValBinning(kusPosTsMsbVal, kusLenTsMsbValBinning);
  // Epoch message
  static constexpr MessField kFieldEpochVal(kusPosEpochVal, kusLenEpochVal);
  // Status
  static constexpr MessField kFieldStatLinkId(kusPosStatLinkId, kusLenStatLinkId);
  static constexpr MessField kFieldStatSxTs(kusPosStatSxTs, kusLenStatSxTs);
  static constexpr MessField kFieldStatStatus(kusPosStatStatus, kusLenStatStatus);
  static constexpr MessField kFieldStatDpbTs(kusPosStatDpbTs, kusLenStatDpbTs);
  static constexpr MessField kFieldStatCpFlag(kusPosStatCpFlag, kusLenStatCpFlag);
  // Empty/End of MS
  static constexpr MessField kFieldEmptyFlag(kusPosEmptyFlag, kusLenEmptyFlag);
  static constexpr MessField kFieldMsErrFlag(kusPosMsErrFlag, kusLenMsErrFlag);
  static constexpr MessField kFieldMsErrType(kusPosMsErrType, kusLenMsErrType);

  /// Status/properties constants
  static constexpr uint32_t kuHitNbAdcBins  = (0 < kusLenHitAdc ? 1 << kusLenHitAdc : 0);
  static constexpr uint32_t kuHitNbTsBins   = (0 < kusLenHitTs ? 1 << kusLenHitTs : 0);
  static constexpr uint32_t kuHitNbOverBins = (0 < kusLenHitTsOver ? 1 << kusLenHitTsOver : 0);
  static constexpr uint32_t kuTsMsbNbTsBins = (0 < kusLenTsMsbVal ? 1 << kusLenTsMsbVal : 0);
  static constexpr uint64_t kulTsCycleNbBins =
    static_cast<uint64_t>(kuTsMsbNbTsBins) * static_cast<uint64_t>(kuHitNbTsBins);
  static constexpr uint16_t kusMaskTsMsbOver = (1 << kusLenHitTsOver) - 1;
  static constexpr uint32_t kulClockCycleNom = 25;  ///< Clock cycle nominator [ns], equivalent to 2*160 MHz clock
  static constexpr uint32_t kulClockCycleDen = 8;   ///< Clock cycle denominator, equivalent to 2*160 MHz clock
  static constexpr double kdClockCycleNs = static_cast<double>(kulClockCycleNom) / kulClockCycleDen;  // ns, not rounded

  // Binning FW adds 1 bit to TS in HIT message => Quick and dirty hack is a factor 2!!!
  static constexpr uint32_t kuHitNbTsBinsBinning   = 1 << 10;
  static constexpr uint32_t kuTsMsbNbTsBinsBinning = 1 << kusLenTsMsbValBinning;
  static constexpr uint64_t kulTsCycleNbBinsBinning =
    static_cast<uint64_t>(kuTsMsbNbTsBinsBinning) * static_cast<uint64_t>(kuHitNbTsBinsBinning);

  class Message {
  private:
  protected:
    uint32_t fuData;  // main and only storage field for the message

  public:
    XPU_H XPU_D Message() : fuData(0) {}

    XPU_H XPU_D Message(const Message& src) : fuData(src.fuData) {}

    XPU_H XPU_D Message(uint32_t uDataIn) : fuData(uDataIn) {}

    XPU_H XPU_D ~Message() {};

    XPU_H XPU_D void assign(const Message& src) { fuData = src.fuData; }

    XPU_H XPU_D Message& operator=(const Message& src)
    {
      assign(src);
      return *this;
    }

    XPU_D inline void reset() { fuData = 0; }

    // --------------------------- Accessors ---------------------------------
    XPU_D inline uint32_t GetData() const { return fuData; }
    XPU_D inline void SetData(uint32_t uValue) { fuData = uValue; }

    XPU_D inline uint32_t GetField(uint32_t uShift, uint32_t uLen) const
    {
      return (fuData >> uShift) & (((static_cast<uint32_t>(1)) << uLen) - 1);
    }

    XPU_D inline uint8_t GetBit(uint32_t uShift) const { return (fuData >> uShift) & 1; }

    XPU_D inline uint32_t GetFieldBE(uint32_t uShift, uint32_t uLen) const
    {
      return (DataBE() >> uShift) & (((static_cast<uint32_t>(1)) << uLen) - 1);
    }
    XPU_D inline uint8_t GetBitBE(uint32_t uShift) const { return (DataBE() >> uShift) & 1; }
    XPU_D inline uint32_t DataBE() const
    {
      return ((fuData & 0x000000FF) << 24) + ((fuData & 0x0000FF00) << 8) + ((fuData >> 8) & 0x0000FF00)
             + ((fuData >> 24) & 0x000000FF);
    }

    // --------------------------- Setters ---------------------------------------
    XPU_D inline void SetField(uint32_t uShift, uint32_t uLen, uint32_t uValue)
    {
      fuData = (fuData & ~((((static_cast<uint32_t>(1)) << uLen) - 1) << uShift))
               | ((static_cast<uint64_t>(uValue)) << uShift);
    }

    XPU_D inline void SetBit(uint32_t uShift, uint8_t uValue)
    {
      fuData =
        uValue ? (fuData | ((static_cast<uint32_t>(1)) << uShift)) : (fuData & ~((static_cast<uint32_t>(1)) << uShift));
    }

    // --------------------------- Simplified Acc_or/Setters ---------------------
    XPU_D inline uint32_t GetField(MessField field) const { return GetField(field.fusPos, field.fusLen); }
    XPU_D inline uint8_t GetBit(MessField field) const { return GetBit(field.fusPos); }
    XPU_D inline bool GetFlag(MessField field) const { return (1 == GetBit(field.fusPos)); }
    XPU_D inline uint32_t GetFieldBE(MessField field) const { return GetFieldBE(field.fusPos, field.fusLen); }
    XPU_D inline uint8_t GetBitBE(MessField field) const { return GetBitBE(field.fusPos); }

    XPU_D inline void SetField(MessField field, uint32_t uValue) { SetField(field.fusPos, field.fusLen, uValue); }
    XPU_D inline void SetBit(MessField field, uint8_t ucValue) { SetBit(field.fusPos, ucValue); }

    // --------------------------- common fields ---------------------------------
    //! For all data: Returns the (global) index of the eLink on which the message was received (n bit field)
    XPU_D inline uint16_t GetLinkIndex() const { return GetField(kFieldLinkIndex); }
    //! Check if the message if a Hit Message
    XPU_D inline bool IsHit() const { return !GetFlag(kFieldNotHitFlag); }
    //! Check if the message if a Dummy Hit Message
    XPU_D inline bool IsDummy() const { return IsHit() && (0 == GetHitAdc()); }
    //! Check if the message if a Ts_Msb
    XPU_D inline bool IsTsMsb() const { return (!IsHit() && MessSubType::TsMsb == GetSubType()); }
    //! Returns the message type, see enum MessType
    XPU_D inline MessType GetMessType() const
    {
      return !GetFlag(kFieldNotHitFlag) ? (0 == GetHitAdc() ? MessType::Dummy : MessType::Hit)
                                        : (MessSubType::TsMsb == GetSubType()
                                             ? MessType::TsMsb
                                             : (MessSubType::Epoch == GetSubType()
                                                  ? MessType::Epoch
                                                  : (MessSubType::Status == GetSubType()
                                                       ? MessType::Status
                                                       : (IsEmptyMsg() ? MessType::Empty : MessType::EndOfMs))));
    }

    // ------------------------ Hit message fields -------------------------------
    //! For Hit data: Returns StsXYTER channel number (7 bit field)
    XPU_D inline uint16_t GetHitChannel() const { return GetField(kFieldHitChannel); }

    //! For Hit data: Returns ADC value (5 bit field)
    XPU_D inline uint16_t GetHitAdc() const { return GetField(kFieldHitAdc); }

    //! For Hit data: Returns Full timestamp (10 bit field including 2 bits overlap)
    XPU_D inline uint16_t GetHitTimeFull() const { return GetField(kFieldHitTsFull); }

    //! For Hit data: Returns timestamp overlap bits (2 bits field, overlap with 2 LSBs of TS_MSB message)
    XPU_D inline uint16_t GetHitTimeOver() const { return GetField(kFieldHitTsOver); }

    //! For Hit data: Returns timestamp (8 bit field, 2 MSB bits overlap removed)
    XPU_D inline uint16_t GetHitTime() const { return GetField(kFieldHitTs); }

    //! For Hit data: Returns timestamp (9 bit field + 1b extra on b30)
    /// => Quick and dirty hack for binning FW!!!
    XPU_D inline uint16_t GetHitTimeBinning() const
    {
      return ((static_cast<uint16_t>(GetBit(30)) << 9) + GetField(kFieldHitTs));
    }

    //! For Hit data: Returns (global) index of the eLink on which the message was received (6 bit field)
    /// => Quick and dirty hack for binning FW!!!
    XPU_D inline uint16_t GetLinkIndexHitBinning() const { return GetField(kFieldLinkIndex) & 0x3F; }

    //! For Hit data: Returns Missed event flag (1 bit field)
    XPU_D inline bool IsHitMissedEvts() const { return GetFlag(kFieldHitEmFlag); }

    //! For Hit data: Sets StsXYTER channel number (7 bit field)
    XPU_D inline void SetHitChannel(uint16_t usVal) { SetField(kFieldHitChannel, usVal); }

    //! For Hit data: Sets ADC value (5 bit field)
    XPU_D inline void SetHitAdc(uint16_t usVal) { SetField(kFieldHitAdc, usVal); }

    //! For Hit data: Sets Full timestamp (10 bit field including 2 bits overlap)
    XPU_D inline void SetHitTimeFull(uint16_t usVal) { SetField(kFieldHitTsFull, usVal); }

    //! For Hit data: Sets timestamp overlap bits (2 bits field, overlap with 2 LSBs of TS_MSB message)
    XPU_D inline void SetHitTimeOver(uint16_t usVal) { SetField(kFieldHitTsOver, usVal); }

    //! For Hit data: Sets Full timestamp (8 bit field, 2 MSB bits overlap removed)
    XPU_D inline void SetHitTime(uint16_t usVal) { SetField(kFieldHitTs, usVal); }

    //! For Hit data: Sets Missed event flag (1 bit field)
    XPU_D inline void SetHitMissEvtsFlag(bool bVal) { SetBit(kFieldHitEmFlag, bVal); }

    // ----------------------- Non-Hit message fields ----------------------------
    //! For non-Hit data: Returns subtype (2 bit field)
    //         inline MessSubType GetSubType() const { return static_cast< uint16_t >( GetField( kFieldSubtype ) ); }
    XPU_D MessSubType GetSubType() const
    {
      switch (GetField(kFieldSubtype)) {
        case static_cast<uint16_t>(MessSubType::TsMsb): return MessSubType::TsMsb;
        case static_cast<uint16_t>(MessSubType::Epoch): return MessSubType::Epoch;
        case static_cast<uint16_t>(MessSubType::Status): return MessSubType::Status;
        default: return MessSubType::Empty;
      }  // switch( static_cast< uint16_t>( GetField( kFieldSubtype ) ) )
    }

    // ------------------------ TS_MSB message fields ----------------------------
    //! For TS MSB data: Returns the TS MSB 22 bit field)
    XPU_D inline uint32_t GetTsMsbVal() const { return GetField(kFieldTsMsbVal); }

    //! For TS MSB data: Returns the TS MSB 29 bit field)
    XPU_D inline uint32_t GetTsMsbValBinning() const { return GetField(kFieldTsMsbValBinning); }

    //! For TS MSB data: Sets the TS MSB (22 bit field)
    XPU_D inline void SetTsMsbVal(uint32_t uVal) { SetField(kFieldTsMsbVal, uVal); }

    // ------------------------ Epoch message fields -----------------------------
    //! For Epoch data: Returns the Epoch (29 bit field)
    XPU_D inline uint32_t GetEpochVal() const { return GetField(kFieldEpochVal); }

    //! For Epoch data: Sets the Epoch (29 bit field)
    XPU_D inline void SetEpochVal(uint32_t uVal) { SetField(kFieldEpochVal, uVal); }

    // ------------------------ Status message fields ----------------------------
    //! For Status data: Returns the Link Inedx (9 bit field)
    XPU_D inline uint16_t GetStatusLink() const { return GetField(kFieldStatLinkId); }

    //! For Status data: Returns the SMX TS from ACK frame (6 bit field)
    XPU_D inline uint16_t GetStatusSxTs() const { return GetField(kFieldStatSxTs); }

    //! For Status data: Returns the Status field from ACK frame (4 bit field)
    XPU_D inline uint16_t GetStatusStatus() const { return GetField(kFieldStatStatus); }

    //! For Status data: Returns the DPB TS when frame received (9 bit field)
    XPU_D inline uint16_t GetStatusDpbTs() const { return GetField(kFieldStatDpbTs); }

    //! For Status data: Returns the CP flag from ACK frame (1 bit field)
    XPU_D inline bool IsCpFlagOn() const { return GetField(kFieldStatCpFlag); }

    //! For Status data: Sets the Status (9 bit field)
    XPU_D inline void SetStatusLink(uint16_t usVal) { SetField(kFieldStatLinkId, usVal); }

    //! For Status data: Sets the Status (6 bit field)
    XPU_D inline void SetStatusSxTs(uint16_t usVal) { SetField(kFieldStatSxTs, usVal); }

    //! For Status data: Sets the Status (4 bit field)
    XPU_D inline void SetStatusStatus(uint16_t usVal) { SetField(kFieldStatStatus, usVal); }

    //! For Status data: Sets the Status (9 bit field)
    XPU_D inline void SetStatusDpbTs(uint16_t usVal) { SetField(kFieldStatDpbTs, usVal); }

    //! For Status data: Sets the Status (1 bit field)
    XPU_D inline void SetCpFlag(bool bVal) { SetField(kFieldStatCpFlag, bVal); }

    // -------------------- Empty/End of MS message fields -----------------------
    //! For Empty/End of MS data: Check if this is an empty message (1 bit field)
    XPU_D inline bool IsEmptyMsg() const { return GetField(kFieldEmptyFlag); }

    //! For End of MS data: Returns the MS error flag (1 bit field)
    XPU_D inline bool IsMsErrorFlagOn() const { return GetField(kFieldMsErrFlag); }

    //! For End of MS data: Returns the MS error type field (2 bit field)
    XPU_D inline uint16_t GetMsErrorType() const { return GetField(kFieldMsErrType); }

    //! For Empty/End of MS data data: Sets the Empty message flag (1 bit field)
    XPU_D inline void SetEmptyMsgFlag(bool bVal) { SetField(kFieldEmptyFlag, bVal); }

    //! For End of MS data data: Sets the MS Error flag (1 bit field)
    XPU_D inline void SetMsErrorFlag(bool bVal) { SetField(kFieldMsErrFlag, bVal); }

    //! For End of MS data: Sets the MS error type field (2 bit field)
    XPU_D inline void SetMsErrorType(uint16_t usVal) { SetField(kFieldMsErrType, usVal); }

    // ------------------------ General OP ---------------------------------------
    bool PrintMess(std::ostream& os, MessagePrintMask ctrl = MessagePrintMask::msg_print_Human,
                   bool bBinning = true) const;


    static std::string PrintMessType(MessType type);
  };
}  // namespace stsxyter
#endif  // STSXYTERMESSAGE_H
