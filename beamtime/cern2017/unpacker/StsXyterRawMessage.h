/************************************************************
 *
 ************************************************************/

#ifndef STSXYTERRAWMESSAGE_H
#define STSXYTERRAWMESSAGE_H

// C/C++ headers
#include <stdint.h>
#include <iostream>

// Switch to VHDL-like bitwise CRC if defined
// CrcCpp implementation otherwise
#define USE_VHDL_CRC

namespace stsxyter {

// tools: Use to this allow bitwise operations on C++11 enums
// This needs to be included in the same namespace to work...
#include "bitmask_operators.hpp"

   /// Constants and other auxiliary stuff
      /// Field full info object
   struct RawMessField
   {
      uint16_t     fusPos; //! Start bit index
      uint16_t     fusLen; //! Field length

      RawMessField( uint16_t usPos, uint16_t usLen ) :
         fusPos( usPos ),
         fusLen( usLen )
      { };
   };
      /// Raw Message types
   enum class RawMessType : uint16_t
   {
      Dummy,
      Hit,
      TsMsb,
      ReadDataAck,
      Ack
   };
      /// Printout control
   enum class RawMessagePrintMask : uint16_t
   {
      msg_print_Prefix = (0x1 << 0),
      msg_print_Data   = (0x1 << 1),
      msg_print_Hex    = (0x1 << 2),
      msg_print_Human  = (0x1 << 3),
      msg_print_Crc    = (0x1 << 4)
   };
   ENABLE_BITMASK_OPERATORS( stsxyter::RawMessagePrintMask ) // Preproc macro!

      /// Fields position (Start bit index)
   static const uint16_t kusRawPosLinkIndex  = 24;
   static const uint16_t kusRawPosNotHitFlag = 23;
      // Hit message
   static const uint16_t kusRawPosHitChannel = 16;
   static const uint16_t kusRawPosHitAdc     = 11;
   static const uint16_t kusRawPosHitTsOver  =  9;
   static const uint16_t kusRawPosHitTs      =  1;
   static const uint16_t kusRawPosHitEmFlag  =  0;
      // Non-hit messages
   static const uint16_t kusRawPosTsMsbFlag  = 22;
         // TS_MSB message
   static const uint16_t kusRawPosTsMsbValA  = 16;
   static const uint16_t kusRawPosTsMsbValB  = 10;
   static const uint16_t kusRawPosTsMsbValC  =  4;
   static const uint16_t kusRawPosTsMsbCrc   =  0;
         // Control messages
   static const uint16_t kusRawPosRdDataFlag = 21;
            // Read Data Ack message
   static const uint16_t kusRawPosRdDataVal  =  6;
   static const uint16_t kusRawPosRdDataSeqN =  3;
   static const uint16_t kusRawPosRdDataCrc  =  0;
            // Ack messages
   static const uint16_t kusRawPosAckValAck  = 19;
   static const uint16_t kusRawPosAckSeqNb   = 15;
   static const uint16_t kusRawPosAckCpFlag  = 14;
   static const uint16_t kusRawPosAckStatus  = 10;
   static const uint16_t kusRawPosAckTime    =  4;
   static const uint16_t kusRawPosAckCrc     =  0;

      /// Fields length (bits)
   static const uint16_t kusRawLenLinkIndex  =  8;
   static const uint16_t kusRawLenRawMess    = 24;
   static const uint16_t kusRawLenNotHitFlag =  1;
      // Hit message
   static const uint16_t kusRawLenHitChannel =  7;
   static const uint16_t kusRawLenHitAdc     =  5;
   static const uint16_t kusRawLenHitTsFull  = 10; // Includes 2 bit overlap with TS message
   static const uint16_t kusRawLenHitTsOver  =  2; // 2 bit overlap with TS message
   static const uint16_t kusRawLenHitTs      =  8;
   static const uint16_t kusRawLenHitEmFlag  =  1;
      // Other message
   static const uint16_t kusRawLenTsMsbFlag  =  1;
         // TS_MSB message
   static const uint16_t kusRawLenTsMsbValA  =  6;
   static const uint16_t kusRawLenTsMsbValB  =  6;
   static const uint16_t kusRawLenTsMsbValC  =  6;
   static const uint16_t kusRawLenTsMsbCrc   =  4;
         // Control messages
   static const uint16_t kusRawLenRdDataFlag =  1;
            // Read Data Ack message
   static const uint16_t kusRawLenRdDataVal  = 15;
   static const uint16_t kusRawLenRdDataSeqN =  3;
   static const uint16_t kusRawLenRdDataCrc  =  3;
            // Ack messages
   static const uint16_t kusRawLenAckValAck  =  2;
   static const uint16_t kusRawLenAckSeqNb   =  4;
   static const uint16_t kusRawLenAckCpFlag  =  1;
   static const uint16_t kusRawLenAckStatus  =  4;
   static const uint16_t kusRawLenAckTime    =  6;
   static const uint16_t kusRawLenAckCrc     =  4;

      /// Fields Info
   static const RawMessField kRawFieldLinkIndex(  kusRawPosLinkIndex,  kusRawLenLinkIndex );
   static const RawMessField kRawFieldNotHitFlag( kusRawPosNotHitFlag, kusRawLenNotHitFlag );
      // Hit message
   static const RawMessField kRawFieldHitChannel( kusRawPosHitChannel, kusRawLenHitChannel );
   static const RawMessField kRawFieldHitAdc    ( kusRawPosHitAdc,     kusRawLenHitAdc );
   static const RawMessField kRawFieldHitTsFull ( kusRawPosHitTs,      kusRawLenHitTsFull );
   static const RawMessField kRawFieldHitTsOver ( kusRawPosHitTsOver,  kusRawLenHitTsOver );
   static const RawMessField kRawFieldHitTs     ( kusRawPosHitTs,      kusRawLenHitTs );
   static const RawMessField kRawFieldHitEmFlag ( kusRawPosHitEmFlag,  kusRawLenHitEmFlag );
      // Non-hit messages
   static const RawMessField kRawFieldTsMsbFlag ( kusRawPosTsMsbFlag,  kusRawLenTsMsbFlag );
         // TS_MSB message
   static const RawMessField kRawFieldTsMsbValA ( kusRawPosTsMsbValA,  kusRawLenTsMsbValA );
   static const RawMessField kRawFieldTsMsbValB ( kusRawPosTsMsbValB,  kusRawLenTsMsbValB );
   static const RawMessField kRawFieldTsMsbValC ( kusRawPosTsMsbValC,  kusRawLenTsMsbValC );
   static const RawMessField kRawFieldTsMsbCrc  ( kusRawPosTsMsbCrc,   kusRawLenTsMsbCrc );
         // Control messages
   static const RawMessField kRawFieldRdDataFlag( kusRawPosRdDataFlag, kusRawLenRdDataFlag );
            // Read Data Ack message
   static const RawMessField kRawFieldRdDataVal ( kusRawPosRdDataVal,  kusRawLenRdDataVal );
   static const RawMessField kRawFieldRdDataSeqN( kusRawPosRdDataSeqN, kusRawLenRdDataSeqN );
   static const RawMessField kRawFieldRdDataCrc ( kusRawPosRdDataCrc,  kusRawLenRdDataCrc );
            // Ack messages
   static const RawMessField kRawFieldAckValAck ( kusRawPosAckValAck,  kusRawLenAckValAck );
   static const RawMessField kRawFieldAckSeqNb  ( kusRawPosAckSeqNb,   kusRawLenAckSeqNb );
   static const RawMessField kRawFieldAckCpFlag ( kusRawPosAckCpFlag,  kusRawLenAckCpFlag);
   static const RawMessField kRawFieldAckStatus ( kusRawPosAckStatus,  kusRawLenAckStatus );
   static const RawMessField kRawFieldAckTime   ( kusRawPosAckTime,    kusRawLenAckTime );
   static const RawMessField kRawFieldAckCrc    ( kusRawPosAckCrc,     kusRawLenAckCrc );

      /// Status/properties constants
   static const uint32_t  kuRawHitNbAdcBins   = 1 << kusRawLenHitAdc;
   static const uint32_t  kuRawHitNbTsBins    = 1 << kusRawLenHitTs;
   static const uint32_t  kuRawHitNbOverBins  = 1 << kusRawLenHitTsOver;
   static const uint32_t  kuRawTsMsbNbTsBins  = 1 << kusRawLenTsMsbValA;
   static const uint32_t  kuRawTsCycleNbBins  = kuRawTsMsbNbTsBins * kuRawHitNbTsBins;
   static const uint16_t  kusRawInvalidTsMsb  = kuRawTsMsbNbTsBins;
   static const uint16_t  kusRawMaskTsMsbOver = (1 << kusRawLenHitTsOver) - 1;
   static const double    kdRawClockCycleNs   = 6.25; // ns, equivalent to 160 MHz clock

   class RawMessage {
      private:
#ifndef USE_VHDL_CRC
         uint16_t CalcCrc3Pol5Len21( uint32_t uVal ) const;
         uint16_t CalcCrc4Pol9Len20( uint32_t uVal ) const;
#endif // not USE_VHDL_CRC
         uint16_t VhdlCalcCrc3Pol5Len21( uint32_t uVal ) const;
         uint16_t VhdlCalcCrc4Pol9Len20( uint32_t uVal ) const;


      protected:
         uint32_t fuData;   // main and only storage field for the message

      public:
         RawMessage() : fuData(0) {}

         RawMessage(const RawMessage& src) : fuData(src.fuData) {}

         RawMessage( uint32_t uDataIn ) : fuData( uDataIn ) {}

         virtual ~RawMessage() {};

         void assign(const RawMessage& src) { fuData = src.fuData; }

         RawMessage& operator=(const RawMessage& src) { assign(src); return *this; }

         inline void reset() { fuData = 0; }

         // --------------------------- Accessors ---------------------------------
         inline uint32_t GetData() const { return fuData; }
         inline void     SetData( uint32_t uValue) { fuData = uValue; }

         inline uint32_t GetField(uint32_t uShift, uint32_t uLen) const
            { return ( fuData >> uShift) & ( ( (static_cast<uint32_t>( 1 ) ) << uLen ) - 1); }

         inline uint8_t GetBit(uint32_t uShift) const
            { return ( fuData >> uShift) & 1; }

         inline uint32_t GetFieldBE(uint32_t uShift, uint32_t uLen) const
            { return ( DataBE() >> uShift ) & ( ( ( static_cast<uint32_t>( 1 ) ) << uLen ) - 1 ); }
         inline uint8_t GetBitBE(uint32_t uShift) const
            { return (DataBE() >> uShift) & 1; }
         inline uint32_t DataBE() const
            { return ( ( fuData        & 0x000000FF ) << 24) +
                     ( ( fuData        & 0x0000FF00 ) <<  8) +
                     ( ( fuData >>  8) & 0x0000FF00 ) +
                     ( ( fuData >> 24) & 0x000000FF );
                     }

         // --------------------------- Setters ---------------------------------------
         inline void     SetField(uint32_t uShift, uint32_t uLen, uint32_t uValue)
            { fuData =  ( fuData & ~( ( ( ( static_cast< uint32_t >( 1 ) ) << uLen) - 1) << uShift ) )
                      | ( ( static_cast< uint64_t >( uValue ) ) << uShift ); }

         inline void    SetBit(uint32_t uShift, uint8_t uValue)
            { fuData = uValue ? ( fuData |  ( ( static_cast< uint32_t >( 1 ) ) << uShift ) ) :
                                ( fuData & ~( ( static_cast< uint32_t >( 1 ) ) << uShift ) ) ; }

         // --------------------------- Simplified Acc_or/Setters ---------------------
         inline uint32_t GetField(   RawMessField field ) const { return GetField(   field.fusPos, field.fusLen ); }
         inline uint8_t  GetBit(     RawMessField field ) const { return GetBit(     field.fusPos ); }
         inline bool     GetFlag(    RawMessField field ) const { return ( 1 == GetBit( field.fusPos ) ); }
         inline uint32_t GetFieldBE( RawMessField field ) const { return GetFieldBE( field.fusPos, field.fusLen ); }
         inline uint8_t  GetBitBE(   RawMessField field ) const { return GetBitBE(   field.fusPos ); }

         inline void     SetField( RawMessField field, uint32_t uValue  ) { SetField( field.fusPos, field.fusLen, uValue ); }
         inline void     SetBit(   RawMessField field, uint8_t  ucValue ) { SetBit(   field.fusPos, ucValue ); }

         // --------------------------- common fields ---------------------------------
         //! For all data: Returns the (global) index of the eLink on which the message was received (n bit field)
         inline uint16_t GetLinkIndex()   const { return GetField( kRawFieldLinkIndex ); }
         //! Check if the message if a Hit Message
         inline bool     IsHit()         const { return !GetFlag( kRawFieldNotHitFlag ); }
         //! Check if the message if a Dummy Hit Message
         inline bool     IsDummy()       const { return IsHit() && ( 0 == GetHitAdc() ); }
         //! Check if the message if a Ts_Msb
         inline bool     IsTsMsb()       const { return (!IsHit() ) && GetFlag( kRawFieldTsMsbFlag ); }
         //! Check if the message if a Hit Message
         inline bool     IsReadDataAck() const { return (!IsHit() ) && (!IsTsMsb() )
                                                                    && GetFlag( kRawFieldRdDataFlag ); }
         //! Returns the message type, see enum MessType
         inline RawMessType GetRawMessType() const { return !GetFlag( kRawFieldNotHitFlag ) ? ( 0 == GetHitAdc() ? RawMessType::Dummy :
                                                                                                                RawMessType::Hit ) :
                                                       GetFlag( kRawFieldTsMsbFlag )  ? RawMessType::TsMsb       :
                                                       GetFlag( kRawFieldRdDataFlag ) ? RawMessType::ReadDataAck :
                                                                                     RawMessType::Ack; }

         // ------------------------ Hit message fields -------------------------------
         //! For Hit data: Returns StsXYTER channel number (7 bit field)
         inline uint16_t GetHitChannel()   const { return GetField( kRawFieldHitChannel ); }

         //! For Hit data: Returns ADC value (5 bit field)
         inline uint16_t GetHitAdc()       const { return GetField( kRawFieldHitAdc ); }

         //! For Hit data: Returns Full timestamp (10 bit field including 2 bits overlap)
         inline uint16_t GetHitTimeFull()  const { return GetField( kRawFieldHitTsFull ); }

         //! For Hit data: Returns timestamp overlap bits (2 bits field, overlap with 2 LSBs of TS_MSB message)
         inline uint16_t GetHitTimeOver()  const { return GetField( kRawFieldHitTsOver ); }

         //! For Hit data: Returns timestamp (8 bit field, 2 MSB bits overlap removed)
         inline uint16_t GetHitTime()      const { return GetField( kRawFieldHitTs ); }

         //! For Hit data: Returns Missed event flag (1 bit field)
         inline bool     IsHitMissedEvts() const { return GetFlag(  kRawFieldHitEmFlag ); }

         //! For Hit data: Sets StsXYTER channel number (7 bit field)
         inline void SetHitChannel( uint16_t usVal )  { SetField( kRawFieldHitChannel, usVal ); }

         //! For Hit data: Sets ADC value (5 bit field)
         inline void SetHitAdc(  uint16_t usVal )     { SetField( kRawFieldHitAdc,     usVal ); }

         //! For Hit data: Sets Full timestamp (10 bit field including 2 bits overlap)
         inline void SetHitTimeFull( uint16_t usVal ) { SetField( kRawFieldHitTsFull,  usVal ); }

         //! For Hit data: Sets timestamp overlap bits (2 bits field, overlap with 2 LSBs of TS_MSB message)
         inline void SetHitTimeOver( uint16_t usVal ) { SetField( kRawFieldHitTsOver,  usVal ); }

         //! For Hit data: Sets Full timestamp (8 bit field, 2 MSB bits overlap removed)
         inline void SetHitTime( uint16_t usVal )     { SetField( kRawFieldHitTs,      usVal ); }

         //! For Hit data: Sets Missed event flag (1 bit field)
         inline void  SetHitMissEvtsFlag( bool bVal)  { SetBit(   kRawFieldHitEmFlag, bVal ); }

         // ------------------------ TS_MSB message fields ----------------------------
         //! For TS MSB data: Returns 1st copy of the TS MSB (6 bit field)
         inline uint16_t GetTsMsbValA() const { return GetField( kRawFieldTsMsbValA ); }
         //! For TS MSB data: Returns 2nd copy of the TS MSB (6 bit field)
         inline uint16_t GetTsMsbValB() const { return GetField( kRawFieldTsMsbValB ); }
         //! For TS MSB data: Returns 3rd copy of the TS MSB (6 bit field)
         inline uint16_t GetTsMsbValC() const { return GetField( kRawFieldTsMsbValC ); }
         //! For TS MSB data: Returns message CRC (4 bit field)
         inline uint16_t GetTsMsbCrc()  const { return GetField( kRawFieldTsMsbCrc ); }
         //! For TS MSB data: Returns data protected by CRC (20 bit field)
         inline uint32_t GetTsMsbCrcInput() const { return GetField( kRawFieldTsMsbValC.fusPos,
                                                                     kusRawLenRawMess - kRawFieldTsMsbCrc.fusLen ); }

         //! For TS MSB data: Sets 1st copy of the TS MSB (6 bit field)
         inline void SetTsMsbValA( uint16_t usVal ) { SetField( kRawFieldTsMsbValA, usVal ); }
         //! For TS MSB data: Sets 2nd copy of the TS MSB (6 bit field)
         inline void SetTsMsbValB( uint16_t usVal ) { SetField( kRawFieldTsMsbValB, usVal ); }
         //! For TS MSB data: Sets 3rd copy of the TS MSB (6 bit field)
         inline void SetTsMsbValC( uint16_t usVal ) { SetField( kRawFieldTsMsbValC, usVal ); }
         //! For TS MSB data: Sets message CRC (4 bit field)
         inline void SetTsMsbCRC(  uint16_t usVal ) { SetField( kRawFieldTsMsbCrc, usVal ); }

         // ------------------------ Read Data Ack message fields ---------------------
         //! For Read Data Ack data: Returns read value (15 bit field)
         inline uint16_t GetRdDataVal()  const { return GetField( kRawFieldRdDataVal ); }
         //! For Read Data Ack data: Returns Sequence Number (3 bit field)
         inline uint16_t GetRdDataSeqN() const { return GetField( kRawFieldRdDataSeqN ); }
         //! For Read Data Ack data: Returns message CRC (3 bit field)
         inline uint16_t GetRdDataCrc()  const { return GetField( kRawFieldRdDataCrc ); }
         //! For Read Data Ack data: Returns data protected by CRC (21 bit field)
         inline uint32_t GetRdDataCrcInput() const { return GetField( kRawFieldRdDataSeqN.fusPos,
                                                                      kusRawLenRawMess - kRawFieldRdDataCrc.fusLen ); }

         //! For Read Data Ack data: Sets read value (15 bit field)
         inline void SetRdDataVal(  uint16_t usVal ) { SetField( kRawFieldRdDataVal,  usVal ); }
         //! For Read Data Ack data: Sets Sequence Number (3 bit field)
         inline void SetRdDataSeqN( uint16_t usVal ) { SetField( kRawFieldRdDataSeqN, usVal ); }
         //! For Read Data Ack data: Sets message CRC (3 bit field)
         inline void SetRdDataCrc(  uint16_t usVal ) { SetField( kRawFieldRdDataCrc,  usVal ); }

         // ------------------------ Ack message fields -------------------------------
         //! For Ack data: Returns ACK value (2 bit field)
         inline uint16_t GetAckVal()    const { return GetField( kRawFieldAckValAck ); }
         //! For Ack data: Returns Sequence Number (4 bit field)
         inline uint16_t GetAckSeqNb()  const { return GetField( kRawFieldAckSeqNb ); }
         //! For Ack data: Returns config parity flag (1 bit field)
         inline uint16_t GetAckCpFlag() const { return GetFlag(  kRawFieldAckCpFlag ); }
         //! For Ack data: Returns ACK status (4 bit field)
         inline uint16_t GetAckStatus() const { return GetField( kRawFieldAckStatus ); }
         //! For Ack data: Returns ACK time (6 bit field)
         inline uint16_t GetAckTime()   const { return GetField( kRawFieldAckTime ); }
         //! For Ack data: Returns message CRC (4 bit field)
         inline uint16_t GetAckCrc()    const { return GetField( kRawFieldAckCrc ); }
         //! For Ack data: Returns data protected by CRC (20 bit field)
         inline uint32_t GetAckCrcInput() const { return GetField( kRawFieldAckTime.fusPos,
                                                                   kusRawLenRawMess - kRawFieldAckCrc.fusLen ); }

         //! For Ack data: Sets ACK value (2 bit field)
         inline void SetAckVal(    uint16_t usVal ) { SetField( kRawFieldAckValAck, usVal ); }
         //! For Ack data: Sets Sequence Number (4 bit field)
         inline void SetAckSeqNb(  uint16_t usVal ) { SetField( kRawFieldAckSeqNb,  usVal ); }
         //! For Ack data: Sets config parity flag (1 bit field)
         inline void SetAckCpFlag( uint16_t usVal ) { SetBit(   kRawFieldAckCpFlag, usVal ); }
         //! For Ack data: Sets ACK status (4 bit field)
         inline void SetAckStatus( uint16_t usVal ) { SetField( kRawFieldAckStatus, usVal ); }
         //! For Ack data: Sets ACK time (6 bit field)
         inline void SetAckTime(   uint16_t usVal ) { SetField( kRawFieldAckTime,   usVal ); }
         //! For Ack data: Sets message CRC (4 bit field)
         inline void SetAckCrc(    uint16_t usVal ) { SetField( kRawFieldAckCrc,    usVal ); }

         // ------------------------ Hit message OP -----------------------------------
         // ------------------------ TS_MSB message OP --------------------------------
         //! For TS MSB data: computes message 4b 0x9 CRC
         uint16_t TsMsbCrcCalc()   const;
         //! For TS MSB data: check if 4b 0x9 CRC is ok
         bool     TsMsbCrcCheck()  const;
         //! For TS MSB data: check if CRC is ok and all 3 TS copies agree
         bool     TsMsbTsCheck()   const;
         //! For TS MSB data: count how many values are agreeing, possible are 1 (none), 2 or 3
         uint16_t GetTsMsbMajCnt() const;
         //! For TS MSB data: check TS integrity and try to correct by majority if corrupted
         bool     GetTsMsbValCorr( uint16_t & uVal ) const;
         // ------------------------ Read Data Ack message OP -------------------------
         //! For Read Data Sck data: computes message 3b 0x5 CRC
         uint16_t RdDataCrcCalc()  const;
         //! For Read Data Sck data: check if 3b 0x5 CRC is ok
         bool     RdDataCrcCheck() const;
         // ------------------------ Ack message OP -----------------------------------
         //! For Ack data: computes message 4b 0x9 CRC
         uint16_t AckCrcCalc()     const;
         //! For Ack data: check if 4b 0x9 CRC is ok
         bool     AckCrcCheck()    const;

         // ------------------------ General OP ---------------------------------------
         bool PrintMess( std::ostream& os, RawMessagePrintMask ctrl = RawMessagePrintMask::msg_print_Human ) const;
   };
}
#endif // STSXYTERRAWMESSAGE_H
