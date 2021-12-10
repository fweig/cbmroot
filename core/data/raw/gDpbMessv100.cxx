/* Copyright (C) 2018-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "gDpbMessv100.h"

// Specific headers

// C++11 headers
#include <cstdio>
#include <cstring>

#include <cmath>

// std C++ lib headers
#include <iomanip>
#include <iostream>
#include <sstream>

//#include <iostream>
#include <iomanip>

//----------------------------------------------------------------------------
/**
 ** Clone of the functions in the flestool library to avoid circular dependencies
 ** Replaces the following block of code which generate warnings depending on the OS
    Form( "%llx", static_cast<uint64_t>(val) );
    Form( "%lx", static_cast<uint64_t>(val) );
    Form( "%0llx", static_cast<uint64_t>(val) );
    Form( "%0lx", static_cast<uint64_t>(val) );
    Form( "%016llx", static_cast<uint64_t>(val) );
    Form( "%016lx", static_cast<uint64_t>(val) );
 **/
namespace gdpbv100
{
  std::string FormatHexPrintout(uint64_t ulVal, char cFill = 0, uint uWidth = 0, bool bUppercase = false)
  {
    std::stringstream ss;

    /// Set hex printout mode
    ss << std::hex;

    /// Set fill character and/or width if provided by user
    if (0 != cFill) ss << std::setfill(cFill);
    if (0 < uWidth) ss << std::setw(uWidth);
    if (bUppercase) ss << std::uppercase;

    /// push value
    ss << ulVal << std::dec;

    /// Restore fill character if needed
    if (0 != cFill) ss << std::setfill(' ');

    return ss.str();
  }
}  // namespace gdpbv100

//----------------------------------------------------------------------------
//! strict weak ordering operator, assumes same epoch for both messages
bool gdpbv100::Message::operator<(const gdpbv100::Message& other) const
{
  uint64_t uThisTs  = 0;
  uint64_t uOtherTs = 0;

  uint32_t uThisType  = this->getMessageType();
  uint32_t uOtherType = other.getMessageType();

  // if both GET4 hit messages, use the full timestamp info
  if (MSG_HIT == uThisType && MSG_HIT == uOtherType) {
    uThisTs  = this->getGdpbHitFullTs();
    uOtherTs = other.getGdpbHitFullTs();
    return uThisTs < uOtherTs;
  }  // both GET4 hit (32b or 24b)

  // First find the timestamp of the current message
  if (MSG_HIT == uThisType) { uThisTs = (this->getGdpbHitFullTs()); }  // if Hit GET4 message (24 or 32b)
  else
    uThisTs = 0;

  // Then find the timestamp of the current message
  if (MSG_HIT == uOtherType) { uOtherTs = (this->getGdpbHitFullTs()); }  // if Hit GET4 message (24 or 32b)
  else
    uOtherTs = 0;

  return uThisTs < uOtherTs;
}
//----------------------------------------------------------------------------
//! equality operator, assumes same epoch for both messages
bool gdpbv100::Message::operator==(const gdpbv100::Message& other) const { return this->data == other.data; }
//----------------------------------------------------------------------------
//! inequality operator, assumes same epoch for both messages
bool gdpbv100::Message::operator!=(const gdpbv100::Message& other) const { return this->data != other.data; }
//----------------------------------------------------------------------------
//! Returns expanded and adjusted time of message (in ns)
uint64_t gdpbv100::Message::getMsgFullTime(uint64_t epoch) const { return std::round(getMsgFullTimeD(epoch)); }
//----------------------------------------------------------------------------
//! Returns expanded and adjusted time of message in double (in ns)
double gdpbv100::Message::getMsgFullTimeD(uint64_t epoch) const
{
  switch (getMessageType()) {
    case MSG_HIT: {
      if (getGdpbHitIs24b())
        return (static_cast<double_t>(FullTimeStamp(epoch, (getGdpbHitCoarse() << 7)))
                + (static_cast<double_t>(getGdpbHitFineTs() - 8.) * gdpbv100::kdFtSize / gdpbv100::kdFtBinsNb))
               * (gdpbv100::kdClockCycleSizeNs / gdpbv100::kdFtSize);
      else
        return (gdpbv100::kdEpochInNs * static_cast<double_t>(epoch)
                + static_cast<double_t>(getGdpbHitFullTs()) * gdpbv100::kdClockCycleSizeNs / gdpbv100::kdFtBinsNb);
    }  // case MSG_HIT:
    case MSG_EPOCH: return gdpbv100::kdEpochInNs * static_cast<double_t>(getGdpbEpEpochNb());
    case MSG_SLOWC:
    case MSG_SYST:
    case MSG_STAR_TRI_A:
    case MSG_STAR_TRI_B:
    case MSG_STAR_TRI_C:
    case MSG_STAR_TRI_D: return gdpbv100::kdEpochInNs * static_cast<double_t>(epoch);
    default: return 0.0;
  }  // switch( getMessageType() )

  // If not already dealt with => unknown type
  return 0.0;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//! Returns the time difference between two expanded time stamps

uint64_t gdpbv100::Message::CalcDistance(uint64_t start, uint64_t stop)
{
  if (start > stop) {
    stop += 0x3FFFFFFFFFFFLLU;
    if (start > stop) {
      printf("Epochs overflow error in CalcDistance\n");
      return 0;
    }
  }

  return stop - start;
}


//----------------------------------------------------------------------------
//! Returns the time difference between two expanded time stamps

double gdpbv100::Message::CalcDistanceD(double start, double stop)
{
  if (start > stop) {
    stop += 0x3FFFFFFFFFFFLLU;
    if (start > stop) {
      printf("Epochs overflow error in CalcDistanceD\n");
      return 0.;
    }
  }

  return stop - start;
}

//----------------------------------------------------------------------------
//! Print message in human readable format to \a cout.
/*!
 * Prints a one line representation of the message in to \a cout.
 * See printData(std::ostream&, unsigned, uint32_t) const for full
 * documentation.
 */

void gdpbv100::Message::printDataCout(unsigned kind, uint32_t epoch) const { printData(msg_print_Cout, kind, epoch); }

//----------------------------------------------------------------------------
//! Print message in human readable format to the Fairroot logger.
/*!
 * Prints a one line representation of the message in to the Fairroot logger.
 * TODO: Add coloring of possible
 * See printData(std::ostream&, unsigned, uint32_t) const for full
 * documentation.
 */

void gdpbv100::Message::printDataLog(unsigned kind, uint32_t epoch) const { printData(msg_print_FairLog, kind, epoch); }

//----------------------------------------------------------------------------
//! Print message in binary or human readable format to a stream.
/*!
 * Prints a one line representation of the message in to a stream, selected by \a outType.
 * The stream is \a cout if \a outType is false and \a FairLogger if \a outType is true.
 * The parameter \a kind is mask with 4 bits
 * \li gdpbv100::msg_print_Prefix (1) - message type
 * \li gdpbv100::msg_print_Data   (2) - print all message specific data fields
 * \li gdpbv100::msg_print_Hex    (4) - print data as hex dump
 * \li gdpbv100::msg_print_Human  (8) - print in human readable format
 *
 * If bit msg_print_Human in \a kind is not set, raw format
 * output is generated. All data fields are shown in hexadecimal.
 * This is the format of choice when chasing hardware problems at the bit level.
 *
 * If bit msg_print_Human is set, a more human readable output is generated.
 * The timestamp is shown as fully extended and adjusted time as
 * returned by the getMsgFullTime(uint32_t) const method.
 * All data fields are represented in decimal.
 *
 * \param os output stream
 * \param kind mask determing output format
 * \param epoch current epoch number (from last epoch message)
 *
 */

//void gdpbv100::Message::printData(std::ostream& os, unsigned kind, uint32_t epoch) const
void gdpbv100::Message::printData(unsigned outType, unsigned kind, uint32_t epoch, std::ostream& os) const
{
  char buf[256];
  if (kind & msg_print_Hex) {
    const uint8_t* arr = reinterpret_cast<const uint8_t*>(&data);
    snprintf(buf, sizeof(buf),
             "BE= %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X LE= "
             "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X ",
             arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[7], arr[6], arr[5], arr[4], arr[3],
             arr[2], arr[1], arr[0]);

    if (msg_print_Cout == outType) std::cout << buf;
    else if (msg_print_File == outType)
      os << buf;

    snprintf(buf, sizeof(buf), " ");
  }

  if (kind & msg_print_Human) {
    double timeInSec = getMsgFullTimeD(epoch) / 1.e9;
    //      int fifoFill = 0;

    switch (getMessageType()) {
      case MSG_EPOCH:
        snprintf(buf, sizeof(buf), "Msg:%u ", getMessageType());

        if (msg_print_Cout == outType) std::cout << buf;
        else if (msg_print_File == outType)
          os << buf;

        snprintf(buf, sizeof(buf),
                 "EPOCH @%17.11f Get4:%2d Epoche2:%10u 0x%08x Sync:%x "
                 "Dataloss:%x Epochloss:%x Epochmissmatch:%x",
                 timeInSec, getGdpbGenChipId(), getGdpbEpEpochNb(), getGdpbEpEpochNb(), getGdpbEpSync(),
                 getGdpbEpDataLoss(), getGdpbEpEpochLoss(), getGdpbEpMissmatch());

        if (msg_print_Cout == outType) std::cout << buf << std::endl;
        else if (msg_print_File == outType)
          os << buf << std::endl;
        break;
      case MSG_HIT:
        snprintf(buf, sizeof(buf), "Msg:%u ", getMessageType());

        if (msg_print_Cout == outType) std::cout << buf;
        else if (msg_print_File == outType)
          os << buf;

        if (getGdpbHitIs24b()) {
          snprintf(buf, sizeof(buf), "Get4 24b @%17.11f Get4:%2d Chn:%3d Edge:%1d Ts:%7d", timeInSec,
                   getGdpbGenChipId(), getGdpbHitChanId(), getGdpbHit24Edge(), getGdpbHitFullTs());
        }  // if( getGdpbHitIs24b() )
        else {
          snprintf(buf, sizeof(buf), "Get4 24b @%17.11f Get4:%2d Chn:%3d Dll:%1d Ts:%7d", timeInSec, getGdpbGenChipId(),
                   getGdpbHitChanId(), getGdpbHit32DllLck(), getGdpbHitFullTs());
        }  // else of if( getGdpbHitIs24b() )

        if (msg_print_Cout == outType) std::cout << buf << std::endl;
        else if (msg_print_File == outType)
          os << buf << std::endl;
        break;
      default:
        kind = kind & ~msg_print_Human;
        if (kind == 0) kind = msg_print_Prefix | msg_print_Data;
    }

    // return, if message was correctly printed in human-readable form
    if (kind & msg_print_Human) return;
  }

  if (kind & msg_print_Prefix) {
    snprintf(buf, sizeof(buf), "Msg:%2u ", getMessageType());

    if (msg_print_Cout == outType) std::cout << buf;
    else if (msg_print_File == outType)
      os << buf;
  }

  if (kind & msg_print_Data) {
    //      const uint8_t* arr = reinterpret_cast<const uint8_t*> ( &data );
    switch (getMessageType()) {
      case MSG_HIT: {
        if (getGdpbHitIs24b()) {
          snprintf(buf, sizeof(buf), "Get4 24 bits, Get4:0x%04x Chn:%1x Edge:%1x Ts:0x%03x", getGdpbGenChipId(),
                   getGdpbHitChanId(), getGdpbHit24Edge(), getGdpbHitFullTs());
        }  // if( getGdpbHitIs24b() )
        else {
          snprintf(buf, sizeof(buf),
                   "Get4 32 bits, Get4:0x%04x Channel %1d Ts:0x%03x Ft:0x%02x "
                   "Tot:0x%02x  Dll %1d",
                   getGdpbGenChipId(), getGdpbHitChanId(), getGdpbHitCoarse(), getGdpbHitFineTs(), getGdpbHit32Tot(),
                   getGdpbHit32DllLck());
        }  // else of if( getGdpbHitIs24b() )
        break;
      }  // case MSG_HIT:
      case MSG_EPOCH: {
        snprintf(buf, sizeof(buf),
                 "Get4:0x%04x Link: %1u Epoch:0x%08x Sync:%x Dataloss:%x "
                 "Epochloss:%x Epochmissmatch:%x",
                 getGdpbGenChipId(), getGdpbEpLinkId(), getGdpbEpEpochNb(), getGdpbEpSync(), getGdpbEpDataLoss(),
                 getGdpbEpEpochLoss(), getGdpbEpMissmatch());
        break;
      }  // case MSG_EPOCH:
      case MSG_SLOWC: {
        // GET4 slow control message, new "true" ROC support
        snprintf(buf, sizeof(buf),
                 "Get4 Slow control, Get4:0x%04x => Chan:%01d Edge:%01d "
                 "Type:%01x Data:0x%06x",
                 getGdpbGenChipId(), 0x0, 0x0, 0x0, getGdpbSlcData());
        break;
      }  // case MSG_SLOWC:
      case MSG_SYST: {
        // GET4 system message, new "true" ROC support
        char sysbuf[256];

        switch (getGdpbSysSubType()) {
          case SYS_GET4_ERROR: {
            snprintf(sysbuf, sizeof(sysbuf),
                     "Get4:0x%04x Ch:0x%01x Edge:%01x Unused:%06x "
                     "ErrCode:0x%02x - GET4 V1 Error Event",
                     getGdpbGenChipId(), getGdpbSysErrChanId(), getGdpbSysErrEdge(), getGdpbSysErrUnused(),
                     getGdpbSysErrData());
            break;
          }  //
          case SYS_GDPB_UNKWN:
            snprintf(sysbuf, sizeof(sysbuf), "Unknown GET4 message, data: 0x%08x", getGdpbSysUnkwData());
            break;
          case SYS_GET4_SYNC_MISS:
            if (getGdpbSysFwErrResync())
              snprintf(sysbuf, sizeof(sysbuf), "GET4 Resynchronization: Get4:0x%04x", getGdpbGenChipId());
            else
              snprintf(sysbuf, sizeof(sysbuf), "GET4 SYNC synchronization error");
            break;
          case SYS_PATTERN:
            snprintf(sysbuf, sizeof(sysbuf), "Pattern message => Type %d, Index %2d, Pattern 0x%08X",
                     getGdpbSysPattType(), getGdpbSysPattIndex(), getGdpbSysPattPattern());
            break;
          default: snprintf(sysbuf, sizeof(sysbuf), "unknown system message type %u", getGdpbSysSubType());
        }  // switch( getGdpbSysSubType() )
        snprintf(buf, sizeof(buf), "%s", sysbuf);

        break;
      }  // case MSG_SYST:
      case MSG_STAR_TRI_A:
      case MSG_STAR_TRI_B:
      case MSG_STAR_TRI_C:
      case MSG_STAR_TRI_D: {
        // STAR trigger token, spread over 4 messages
        switch (getStarTrigMsgIndex()) {
          case 0: {
            snprintf(buf, sizeof(buf),
                     //                    "STAR token A, gDPB TS MSB bits: 0x%010llx000000",
                     //                    getGdpbTsMsbStarA() );
                     "STAR token A, gDPB TS MSB bits: 0x%s000000",
                     FormatHexPrintout(getGdpbTsMsbStarA(), 10, '0').c_str());
            break;
          }  // case 1st message:
          case 1: {
            snprintf(
              buf, sizeof(buf),
              //                    "STAR token B, gDPB TS LSB bits: 0x0000000000%06llx, STAR TS MSB bits: 0x%04llx000000000000",
              //                    getGdpbTsLsbStarB(), getStarTsMsbStarB() );
              "STAR token B, gDPB TS LSB bits: 0x0000000000%s, STAR TS MSB "
              "bits: 0x%s000000000000",
              FormatHexPrintout(getGdpbTsLsbStarB(), 6, '0').c_str(),
              FormatHexPrintout(getStarTsMsbStarB(), 4, '0').c_str());
            break;
          }  // case 2nd message:
          case 2: {
            snprintf(
              buf, sizeof(buf),
              //                    "STAR token C,                                     , STAR TS Mid bits: 0x0000%010llx00",
              //                    getStarTsMidStarC() );
              "STAR token C,                                     , STAR TS Mid "
              "bits: 0x0000%s00",
              FormatHexPrintout(getStarTsMidStarC(), 10, '0').c_str());
            break;
          }  // case 3rd message:
          case 3: {
            snprintf(
              buf, sizeof(buf),
              //                    "STAR token D,                                     , STAR TS LSB bits: 0x00000000000000%02llx"
              "STAR token D,                                     , STAR TS LSB "
              "bits: 0x00000000000000%s"
              ", Token: %03x, DAQ: %1x; TRG:%1x",
              //                    getStarTsLsbStarD(),
              FormatHexPrintout(getStarTsLsbStarD(), 2, '0').c_str(), getStarTokenStarD(), getStarDaqCmdStarD(),
              getStarTrigCmdStarD());
            break;
          }  // case 4th message:
        }    // switch( getStarTrigMsgIndex() )

        break;
      }  // case MSG_STAR_TRI_A || MSG_STAR_TRI_B || MSG_STAR_TRI_C || MSG_STAR_TRI_D:
      default:
        snprintf(buf, sizeof(buf), "Error - unexpected MessageType: %1x, full data %08X::%08X", getMessageType(),
                 getField(32, 32), getField(0, 32));
    }
  }

  if (msg_print_Cout == outType) std::cout << buf << std::endl;
  else if (msg_print_File == outType)
    os << buf << std::endl;
}
//----------------------------------------------------------------------------
//! strict weak ordering operator, including epoch for both messages
bool gdpbv100::FullMessage::operator<(const FullMessage& other) const
{
  if (other.fulExtendedEpoch == this->fulExtendedEpoch)
    // Same epoch => use Message (base) class ordering operator
    return this->Message::operator<(other);
  else
    return this->fulExtendedEpoch < other.fulExtendedEpoch;
}
//----------------------------------------------------------------------------
void gdpbv100::FullMessage::PrintMessage(unsigned outType, unsigned kind) const
{
  std::cout << "Full epoch = " << std::setw(9) << fulExtendedEpoch << " ";
  printDataCout(outType, kind);
}
