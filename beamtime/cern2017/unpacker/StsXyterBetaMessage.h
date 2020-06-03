/************************************************************
 *
 ************************************************************/

#ifndef STSXYTERBETAMESSAGE_H
#define STSXYTERBETAMESSAGE_H

// C/C++ headers
#include <stdint.h>
#include <iostream>

namespace stsxyter {

// tools: Use to this allow bitwise operations on C++11 enums
// This needs to be included in the same namespace to work...
#include "bitmask_operators.hpp"

   /// Constants and other auxiliary stuff
      /// Field full info object
   struct BetaMessField
   {
      uint16_t     fusPos; //! Start bit index
      uint16_t     fusLen; //! Field length

      BetaMessField( uint16_t usPos, uint16_t usLen ) :
         fusPos( usPos ),
         fusLen( usLen )
      { };
   };
      /// Message types
   enum class BetaMessType : uint16_t
   {
      Dummy,
      Hit,
      TsMsb
   };
      /// Printout control
   enum class BetaMessagePrintMask : uint16_t
   {
      msg_print_Prefix = (0x1 << 0),
      msg_print_Data   = (0x1 << 1),
      msg_print_Hex    = (0x1 << 2),
      msg_print_Human  = (0x1 << 3)
   };
   ENABLE_BITMASK_OPERATORS( stsxyter::BetaMessagePrintMask ) // Preproc macro!

      /// Fields position (Start bit index)
   static const uint16_t kusBetaPosNotHitFlag = 31;
   static const uint16_t kusBetaPosLinkIndex  = 22;
      // Hit message
   static const uint16_t kusBetaPosHitChannel = 15;
   static const uint16_t kusBetaPosHitAdc     = 10;
   static const uint16_t kusBetaPosHitTsOver  =  9;
   static const uint16_t kusBetaPosHitTs      =  1;
   static const uint16_t kusBetaPosHitEmFlag  =  0;
      // Non-hit messages
   static const uint16_t kusBetaPosTsMsbFlag  = 22;
         // TS_MSB message
   static const uint16_t kusBetaPosTsMsbVal   =  0;

      /// Fields length (bits)
   static const uint16_t kusBetaLenNotHitFlag =  1;
   static const uint16_t kusBetaLenLinkIndex  =  9;
      // Hit message
   static const uint16_t kusBetaLenHitChannel =  7;
   static const uint16_t kusBetaLenHitAdc     =  5;
   static const uint16_t kusBetaLenHitTsFull  =  9; // Includes 1 bit overlap with TS message ?
   static const uint16_t kusBetaLenHitTsOver  =  1; // 2 bit overlap with TS message
   static const uint16_t kusBetaLenHitTs      =  8;
   static const uint16_t kusBetaLenHitEmFlag  =  1;
      // Other message
   static const uint16_t kusBetaLenTsMsbFlag  =  1;
         // TS_MSB message
   static const uint16_t kusBetaLenTsMsbVal   = 13;

      /// Fields Info
   static const BetaMessField kBetaFieldLinkIndex(  kusBetaPosLinkIndex,  kusBetaLenLinkIndex );
   static const BetaMessField kBetaFieldNotHitFlag( kusBetaPosNotHitFlag, kusBetaLenNotHitFlag );
      // Hit message
   static const BetaMessField kBetaFieldHitChannel( kusBetaPosHitChannel, kusBetaLenHitChannel );
   static const BetaMessField kBetaFieldHitAdc    ( kusBetaPosHitAdc,     kusBetaLenHitAdc );
   static const BetaMessField kBetaFieldHitTsFull ( kusBetaPosHitTs,      kusBetaLenHitTsFull );
   static const BetaMessField kBetaFieldHitTsOver ( kusBetaPosHitTsOver,  kusBetaLenHitTsOver );
   static const BetaMessField kBetaFieldHitTs     ( kusBetaPosHitTs,      kusBetaLenHitTs );
   static const BetaMessField kBetaFieldHitEmFlag ( kusBetaPosHitEmFlag,  kusBetaLenHitEmFlag );
      // Non-hit messages
   static const BetaMessField kBetaFieldTsMsbFlag ( kusBetaPosTsMsbFlag,  kusBetaLenTsMsbFlag );
         // TS_MSB message
   static const BetaMessField kBetaFieldTsMsbVal  ( kusBetaPosTsMsbVal,   kusBetaLenTsMsbVal );

      /// Status/properties constants
   static const uint32_t  kuBetaHitNbAdcBins   = 1 << kusBetaLenHitAdc;
   static const uint32_t  kuBetaHitNbTsBins    = 1 << kusBetaLenHitTs;
   static const uint32_t  kuBetaHitNbOverBins  = 1 << kusBetaLenHitTsOver;
   static const uint32_t  kuBetaTsMsbNbTsBins  = 1 << kusBetaLenTsMsbVal;
   static const uint32_t  kuBetaTsCycleNbBins  = kuBetaTsMsbNbTsBins * kuBetaHitNbTsBins;
   static const uint16_t  kusBetaInvalidTsMsb  = kuBetaTsMsbNbTsBins;
   static const uint16_t  kusBetaMaskTsMsbOver = (1 << kusBetaLenHitTsOver) - 1;
   static const double    kdBetaClockCycleNs   = 3.125; // ns, equivalent to 2*160 MHz clock

   class BetaMessage {
      private:


      protected:
         uint32_t fuData;   // main and only storage field for the message

      public:
         BetaMessage() : fuData(0) {}

         BetaMessage(const BetaMessage& src) : fuData(src.fuData) {}

         BetaMessage( uint32_t uDataIn ) : fuData( uDataIn ) {}

         virtual ~BetaMessage() {};

         void assign(const BetaMessage& src) { fuData = src.fuData; }

         BetaMessage& operator=(const BetaMessage& src) { assign(src); return *this; }

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
         inline uint32_t GetField(   BetaMessField field ) const { return GetField(   field.fusPos, field.fusLen ); }
         inline uint8_t  GetBit(     BetaMessField field ) const { return GetBit(     field.fusPos ); }
         inline bool     GetFlag(    BetaMessField field ) const { return ( 1 == GetBit( field.fusPos ) ); }
         inline uint32_t GetFieldBE( BetaMessField field ) const { return GetFieldBE( field.fusPos, field.fusLen ); }
         inline uint8_t  GetBitBE(   BetaMessField field ) const { return GetBitBE(   field.fusPos ); }

         inline void     SetField( BetaMessField field, uint32_t uValue  ) { SetField( field.fusPos, field.fusLen, uValue ); }
         inline void     SetBit(   BetaMessField field, uint8_t  ucValue ) { SetBit(   field.fusPos, ucValue ); }

         // --------------------------- common fields ---------------------------------
         //! For all data: Returns the (global) index of the eLink on which the message was received (n bit field)
         inline uint16_t GetLinkIndex()   const { return GetField( kBetaFieldLinkIndex ); }
         //! Check if the message if a Hit Message
         inline bool     IsHit()         const { return !GetFlag( kBetaFieldNotHitFlag ); }
         //! Check if the message if a Dummy Hit Message
         inline bool     IsDummy()       const { return IsHit() && ( 0 == GetHitAdc() ); }
         //! Check if the message if a Ts_Msb
         inline bool     IsTsMsb()       const { return (!IsHit() ); }
         //! Returns the message type, see enum BetaMessType
         inline BetaMessType GetMessType() const { return !GetFlag( kBetaFieldNotHitFlag ) ? ( 0 == GetHitAdc() ? BetaMessType::Dummy :
                                                                                                          BetaMessType::Hit ) :
                                                                                     BetaMessType::TsMsb; }

         // ------------------------ Hit message fields -------------------------------
         //! For Hit data: Returns StsXYTER channel number (7 bit field)
         inline uint16_t GetHitChannel()   const { return GetField( kBetaFieldHitChannel ); }

         //! For Hit data: Returns ADC value (5 bit field)
         inline uint16_t GetHitAdc()       const { return GetField( kBetaFieldHitAdc ); }

         //! For Hit data: Returns Full timestamp (10 bit field including 2 bits overlap)
         inline uint16_t GetHitTimeFull()  const { return GetField( kBetaFieldHitTsFull ); }

         //! For Hit data: Returns timestamp overlap bits (2 bits field, overlap with 2 LSBs of TS_MSB message)
         inline uint16_t GetHitTimeOver()  const { return GetField( kBetaFieldHitTsOver ); }

         //! For Hit data: Returns timestamp (8 bit field, 2 MSB bits overlap removed)
         inline uint16_t GetHitTime()      const { return GetField( kBetaFieldHitTs ); }

         //! For Hit data: Returns Missed event flag (1 bit field)
         inline bool     IsHitMissedEvts() const { return GetFlag(  kBetaFieldHitEmFlag ); }

         //! For Hit data: Sets StsXYTER channel number (7 bit field)
         inline void SetHitChannel( uint16_t usVal )  { SetField( kBetaFieldHitChannel, usVal ); }

         //! For Hit data: Sets ADC value (5 bit field)
         inline void SetHitAdc(  uint16_t usVal )     { SetField( kBetaFieldHitAdc,     usVal ); }

         //! For Hit data: Sets Full timestamp (10 bit field including 2 bits overlap)
         inline void SetHitTimeFull( uint16_t usVal ) { SetField( kBetaFieldHitTsFull,  usVal ); }

         //! For Hit data: Sets timestamp overlap bits (2 bits field, overlap with 2 LSBs of TS_MSB message)
         inline void SetHitTimeOver( uint16_t usVal ) { SetField( kBetaFieldHitTsOver,  usVal ); }

         //! For Hit data: Sets Full timestamp (8 bit field, 2 MSB bits overlap removed)
         inline void SetHitTime( uint16_t usVal )     { SetField( kBetaFieldHitTs,      usVal ); }

         //! For Hit data: Sets Missed event flag (1 bit field)
         inline void  SetHitMissEvtsFlag( bool bVal)  { SetBit(   kBetaFieldHitEmFlag, bVal ); }

         // ------------------------ TS_MSB message fields ----------------------------
         //! For TS MSB data: Returns the TS MSB 13 bit field)
         inline uint16_t GetTsMsbVal() const { return GetField( kBetaFieldTsMsbVal ); }

         //! For TS MSB data: Sets the TS MSB (13 bit field)
         inline void SetTsMsbVal( uint16_t usVal ) { SetField( kBetaFieldTsMsbVal, usVal ); }

         // ------------------------ General OP ---------------------------------------
         bool PrintMess( std::ostream& os, BetaMessagePrintMask ctrl = BetaMessagePrintMask::msg_print_Human ) const;
   };
}
#endif // STSXYTERBETAMESSAGE_H
