/* Copyright (C) 2017-2018 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "StsXyterMessage.h"

// tools


// C++11

// C/C++
#include <iomanip>

// Namespace
using namespace stsxyter;  // Class own namespace
// Namespaces alias
//namespace sxm = stsxyter::Message;

//************************* Messages OP ************************************//
//----------------------------------------------------------------------------
MessSubType Message::GetSubType() const
{
  switch (GetField(kFieldSubtype)) {
    case static_cast<uint16_t>(MessSubType::TsMsb): return MessSubType::TsMsb;
    case static_cast<uint16_t>(MessSubType::Epoch): return MessSubType::Epoch;
    case static_cast<uint16_t>(MessSubType::Status): return MessSubType::Status;
    default: return MessSubType::Empty;
  }  // switch( static_cast< uint16_t>( GetField( kFieldSubtype ) ) )
}
//----------------------------------------------------------------------------
bool Message::PrintMess(std::ostream& os, MessagePrintMask ctrl, bool bBinning) const
{
  bool bPrintHex    = static_cast<bool>(ctrl & MessagePrintMask::msg_print_Hex);
  bool bPrintHuman  = static_cast<bool>(ctrl & MessagePrintMask::msg_print_Human);
  bool bPrintPrefix = static_cast<bool>(ctrl & MessagePrintMask::msg_print_Prefix);
  bool bPrintData   = static_cast<bool>(ctrl & MessagePrintMask::msg_print_Data);


  if (bPrintHex) {
    const uint8_t* cArrByt = reinterpret_cast<const uint8_t*>(&fuData);

    // Set fill char to 0 and read previous state
    char cPrev = os.fill('0');
    os << "BE = " << std::hex << std::setw(2) << static_cast<uint16_t>(cArrByt[0]) << ":" << std::setw(2)
       << static_cast<uint16_t>(cArrByt[1]) << ":" << std::setw(2) << static_cast<uint16_t>(cArrByt[2]) << ":"
       << std::setw(2) << static_cast<uint16_t>(cArrByt[3]) << " LE = " << std::hex << std::setw(2)
       << static_cast<uint16_t>(cArrByt[3]) << ":" << std::setw(2) << static_cast<uint16_t>(cArrByt[2]) << ":"
       << std::setw(2) << static_cast<uint16_t>(cArrByt[1]) << ":" << std::setw(2) << static_cast<uint16_t>(cArrByt[0])
       << std::dec << " => ";

    // Restore fill char to original
    os.fill(cPrev);
  }  // if( ctrl & msg_print_Hex )

  if (bPrintHuman) {
    switch (GetMessType()) {
      case MessType::Dummy: {
        os << " Dummy Hit ";
        break;
      }  // case MessType::TsMsb
      case MessType::Hit: {
        os << " Hit => "
           << " Lnk: " << std::setw(3) << (bBinning ? GetLinkIndexHitBinning() : GetLinkIndex())
           << " Ch: " << std::setw(3) << GetHitChannel() << " Adc: " << std::setw(2) << GetHitAdc()
           << " Ts Full: " << std::setw(4) << GetHitTimeFull() << " Ts Over: " << std::hex << GetHitTimeOver()
           << std::dec << " Ts: " << std::setw(3) << (bBinning ? GetHitTimeBinning() : GetHitTime()) << " Missed? "
           << IsHitMissedEvts();
        break;
      }  // case MessType::Hit
      case MessType::TsMsb: {
        os << " TS_MSB => " << std::setw(12) << (bBinning ? GetTsMsbValBinning() : GetTsMsbVal());
        break;
      }  // case MessType::TsMsb
      case MessType::Epoch: {
        os << " Epoch => " << std::setw(12) << GetEpochVal();
        break;
      }  // case MessType::TsMsb
      case MessType::Status: {
        os << " Status => "
           << " Lnk: " << std::setw(2) << GetStatusLink() << " Smx TS: " << std::setw(2) << GetStatusSxTs()
           << " Status: 0x" << std::setw(4) << std::hex << GetStatusStatus() << std::dec << " Dpb TS: " << std::setw(3)
           << GetStatusDpbTs() << " CP flag: " << IsCpFlagOn();
        break;
      }  // case MessType::TsMsb
      case MessType::Empty: {
        os << " Empty ";
        break;
      }  // case MessType::TsMsb
      default: {
      }  // default
    }    // switch( GetMessType() )
  }      // if( bPrintHuman )

  if (bPrintPrefix) {
    switch (GetMessType()) {
      case MessType::Dummy: {
        break;
      }  // case MessType::Dummy
      case MessType::Hit: {
        break;
      }  // case MessType::Hit
      case MessType::TsMsb: {
        break;
      }  // case MessType::TsMsb
      case MessType::Epoch: {
        break;
      }  // case MessType::Epoch
      case MessType::Status: {
        break;
      }  // case MessType::Status
      case MessType::Empty: {
        break;
      }  // case MessType::Empty
      default: {
      }  // default
    }    // switch( GetMessType() )
  }      // if( bPrintPrefix )

  if (bPrintData) {
    switch (GetMessType()) {
      case MessType::Dummy: {
        break;
      }  // case MessType::Dummy
      case MessType::Hit: {
        break;
      }  // case MessType::Hit
      case MessType::TsMsb: {
        break;
      }  // case MessType::TsMsb
      case MessType::Epoch: {
        break;
      }  // case MessType::Epoch
      case MessType::Status: {
        break;
      }  // case MessType::Status
      case MessType::Empty: {
        break;
      }  // case MessType::Empty
      default: {
      }  // default
    }    // switch( GetMessType() )
  }      // if( bPrintData )

  // Finish with a new line => 1 line per message printout
  os << std::endl;

  return true;
}
//----------------------------------------------------------------------------
std::string Message::PrintMessType(MessType type)
{
  switch (type) {
    case MessType::Dummy: return "Dummy";
    case MessType::Hit: return "Hit";
    case MessType::TsMsb: return "TsMsb";
    case MessType::Epoch: return "Epoch";
    case MessType::Status: return "Status";
    case MessType::Empty: return "Empty";
    case MessType::EndOfMs: return "EndOfMs";
    default: return "Unknown";
  }
}
//----------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//**************************************************************************//


//*********************** Private methods **********************************//
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//**************************************************************************//
