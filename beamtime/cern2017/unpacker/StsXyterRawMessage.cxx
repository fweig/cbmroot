
#include "StsXyterRawMessage.h"

// tools
#ifndef USE_VHDL_CRC
   #define CRCPP_USE_NAMESPACE
   #define CRCPP_USE_CPP11
   #include "CRCpp_CRC.h"
#endif // not USE_VHDL_CRC

// C++11

// C/C++
#include <iomanip>
#include <vector>

// Namespace
using namespace stsxyter;    // Class own namespace
// Namespaces alias
//namespace sxm = stsxyter::RawMessage;

/*
static const uint16_t kusCrcTable3_x5[ 256 ] =
   {
     0x7,      0x4,      0x1,      0x2,      0x0,      0x3,      0x6,      0x5,
     0x2,      0x1,      0x4,      0x7,      0x5,      0x6,      0x3,      0x0,
     0x6,      0x5,      0x0,      0x3,      0x1,      0x2,      0x7,      0x4,
     0x3,      0x0,      0x5,      0x6,      0x4,      0x7,      0x2,      0x1,
     0x5,      0x6,      0x3,      0x0,      0x2,      0x1,      0x4,      0x7,
     0x0,      0x3,      0x6,      0x5,      0x7,      0x4,      0x1,      0x2,
     0x4,      0x7,      0x2,      0x1,      0x3,      0x0,      0x5,      0x6,
     0x1,      0x2,      0x7,      0x4,      0x6,      0x5,      0x0,      0x3,
     0x3,      0x0,      0x5,      0x6,      0x4,      0x7,      0x2,      0x1,
     0x6,      0x5,      0x0,      0x3,      0x1,      0x2,      0x7,      0x4,
     0x2,      0x1,      0x4,      0x7,      0x5,      0x6,      0x3,      0x0,
     0x7,      0x4,      0x1,      0x2,      0x0,      0x3,      0x6,      0x5,
     0x1,      0x2,      0x7,      0x4,      0x6,      0x5,      0x0,      0x3,
     0x4,      0x7,      0x2,      0x1,      0x3,      0x0,      0x5,      0x6,
     0x0,      0x3,      0x6,      0x5,      0x7,      0x4,      0x1,      0x2,
     0x5,      0x6,      0x3,      0x0,      0x2,      0x1,      0x4,      0x7,
     0x4,      0x7,      0x2,      0x1,      0x3,      0x0,      0x5,      0x6,
     0x1,      0x2,      0x7,      0x4,      0x6,      0x5,      0x0,      0x3,
     0x5,      0x6,      0x3,      0x0,      0x2,      0x1,      0x4,      0x7,
     0x0,      0x3,      0x6,      0x5,      0x7,      0x4,      0x1,      0x2,
     0x6,      0x5,      0x0,      0x3,      0x1,      0x2,      0x7,      0x4,
     0x3,      0x0,      0x5,      0x6,      0x4,      0x7,      0x2,      0x1,
     0x7,      0x4,      0x1,      0x2,      0x0,      0x3,      0x6,      0x5,
     0x2,      0x1,      0x4,      0x7,      0x5,      0x6,      0x3,      0x0,
     0x0,      0x3,      0x6,      0x5,      0x7,      0x4,      0x1,      0x2,
     0x5,      0x6,      0x3,      0x0,      0x2,      0x1,      0x4,      0x7,
     0x1,      0x2,      0x7,      0x4,      0x6,      0x5,      0x0,      0x3,
     0x4,      0x7,      0x2,      0x1,      0x3,      0x0,      0x5,      0x6,
     0x2,      0x1,      0x4,      0x7,      0x5,      0x6,      0x3,      0x0,
     0x7,      0x4,      0x1,      0x2,      0x0,      0x3,      0x6,      0x5,
     0x3,      0x0,      0x5,      0x6,      0x4,      0x7,      0x2,      0x1,
     0x6,      0x5,      0x0,      0x3,      0x1,      0x2,      0x7,      0x4
   };
static const uint16_t kusCrcTable4_x9[ 256 ] =
   {
     0x4,      0x7,      0x2,      0x1,      0x8,      0xb,      0xe,      0xd,
     0xf,      0xc,      0x9,      0xa,      0x3,      0x0,      0x5,      0x6,
     0x1,      0x2,      0x7,      0x4,      0xd,      0xe,      0xb,      0x8,
     0xa,      0x9,      0xc,      0xf,      0x6,      0x5,      0x0,      0x3,
     0xe,      0xd,      0x8,      0xb,      0x2,      0x1,      0x4,      0x7,
     0x5,      0x6,      0x3,      0x0,      0x9,      0xa,      0xf,      0xc,
     0xb,      0x8,      0xd,      0xe,      0x7,      0x4,      0x1,      0x2,
     0x0,      0x3,      0x6,      0x5,      0xc,      0xf,      0xa,      0x9,
     0x3,      0x0,      0x5,      0x6,      0xf,      0xc,      0x9,      0xa,
     0x8,      0xb,      0xe,      0xd,      0x4,      0x7,      0x2,      0x1,
     0x6,      0x5,      0x0,      0x3,      0xa,      0x9,      0xc,      0xf,
     0xd,      0xe,      0xb,      0x8,      0x1,      0x2,      0x7,      0x4,
     0x9,      0xa,      0xf,      0xc,      0x5,      0x6,      0x3,      0x0,
     0x2,      0x1,      0x4,      0x7,      0xe,      0xd,      0x8,      0xb,
     0xc,      0xf,      0xa,      0x9,      0x0,      0x3,      0x6,      0x5,
     0x7,      0x4,      0x1,      0x2,      0xb,      0x8,      0xd,      0xe,
     0xa,      0x9,      0xc,      0xf,      0x6,      0x5,      0x0,      0x3,
     0x1,      0x2,      0x7,      0x4,      0xd,      0xe,      0xb,      0x8,
     0xf,      0xc,      0x9,      0xa,      0x3,      0x0,      0x5,      0x6,
     0x4,      0x7,      0x2,      0x1,      0x8,      0xb,      0xe,      0xd,
     0x0,      0x3,      0x6,      0x5,      0xc,      0xf,      0xa,      0x9,
     0xb,      0x8,      0xd,      0xe,      0x7,      0x4,      0x1,      0x2,
     0x5,      0x6,      0x3,      0x0,      0x9,      0xa,      0xf,      0xc,
     0xe,      0xd,      0x8,      0xb,      0x2,      0x1,      0x4,      0x7,
     0xd,      0xe,      0xb,      0x8,      0x1,      0x2,      0x7,      0x4,
     0x6,      0x5,      0x0,      0x3,      0xa,      0x9,      0xc,      0xf,
     0x8,      0xb,      0xe,      0xd,      0x4,      0x7,      0x2,      0x1,
     0x3,      0x0,      0x5,      0x6,      0xf,      0xc,      0x9,      0xa,
     0x7,      0x4,      0x1,      0x2,      0xb,      0x8,      0xd,      0xe,
     0xc,      0xf,      0xa,      0x9,      0x0,      0x3,      0x6,      0x5,
     0x2,      0x1,      0x4,      0x7,      0xe,      0xd,      0x8,      0xb,
     0x9,      0xa,      0xf,      0xc,      0x5,      0x6,      0x3,      0x0
   }
*/

#ifndef USE_VHDL_CRC
// CRC constants, hopefully computed at compile/initialization time
///< CRC polynomial, 0x5 =  101 (Koopman) =>  1011 = 0x0B (full) =>  011 = 0x03 (Here)
static const CRCPP::CRC::Parameters< std::uint8_t, 3 > par_crc3_5 = { 0x03, 0x01, 0x01, true, true };
static const CRCPP::CRC::Table<      std::uint8_t, 3 > table_crc3_5( par_crc3_5 );
///< CRC polynomial, 0x9 = 1001 (Koopman) => 10011 = 0x13 (full) => 0011 = 0x03 (Here)
static const CRCPP::CRC::Parameters< std::uint8_t, 4 > par_crc4_9 = { 0x03, 0x00, 0x00, true, true };
static const CRCPP::CRC::Table<      std::uint8_t, 4 > table_crc4_9( par_crc4_9 );
#endif // not USE_VHDL_CRC

//************************* Messages OP ************************************//
//+++++++++++++++++++++++++ Hit message OP +++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++ TS_MSB message OP ++++++++++++++++++++++++++++++++
//----------------------------------------------------------------------------
uint16_t RawMessage::TsMsbCrcCalc() const
{
#ifdef USE_VHDL_CRC
   return VhdlCalcCrc4Pol9Len20( GetTsMsbCrcInput() );
#else
   return CalcCrc4Pol9Len20( GetTsMsbCrcInput() );
#endif //USE_VHDL_CRC
}
//----------------------------------------------------------------------------
bool     RawMessage::TsMsbCrcCheck() const
{
   return ( TsMsbCrcCalc() == GetTsMsbCrc() );
}
//----------------------------------------------------------------------------
bool     RawMessage::TsMsbTsCheck() const
{
   uint16_t uValA = GetTsMsbValA();
   return ( TsMsbCrcCheck() &&
            ( GetTsMsbValB() == uValA ) &&
            ( GetTsMsbValC() == uValA ) );
}
//----------------------------------------------------------------------------
uint16_t RawMessage::GetTsMsbMajCnt() const
{
   uint16_t uValA = GetTsMsbValA();
   uint16_t uValB = GetTsMsbValB();
   uint16_t uValC = GetTsMsbValC();

   uint16_t uMajCnt = 1; // 0 pair  => 1 agreeing value (each with itself)
   if( uValA == uValB )
      uMajCnt ++;
   if( uValA == uValC )
      uMajCnt ++;
      else if( uValB == uValC )
         uMajCnt ++;

   return uMajCnt;
}
//----------------------------------------------------------------------------
bool     RawMessage::GetTsMsbValCorr( uint16_t & uVal ) const
{
   uint16_t uValA = GetTsMsbValA();

   // Check if TS integrity ok
   // if yes just give first copy of it and return true
   if( TsMsbTsCheck() )
   {
      uVal = uValA;
      return true;
   } // if( TsMsbTsCheck() )

   uint16_t uValB = GetTsMsbValB();

   // Majority decision:
   // - if any two values are same, give it as 3rd one either minority or same
   // - if no two value identical, give invalid TS bigger than allowed
   if( uValA == uValB )
      uVal = uValA;
      else
      {
         uint16_t uValC = GetTsMsbValC();
         if( uValA == uValC )
            uVal = uValA;
         else if( uValB == uValC )
            uVal = uValB;
            else uVal = kusRawInvalidTsMsb;
      } // else of if( uValA == uValB )

   return false;
}
//----------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++ Read Data Ack message OP +++++++++++++++++++++++++
//----------------------------------------------------------------------------
uint16_t RawMessage::RdDataCrcCalc() const
{
#ifdef USE_VHDL_CRC
   return VhdlCalcCrc3Pol5Len21( GetRdDataCrcInput() );
#else
   return CalcCrc3Pol5Len21( GetRdDataCrcInput() );
#endif //USE_VHDL_CRC
}
//----------------------------------------------------------------------------
bool     RawMessage::RdDataCrcCheck() const
{
   return ( RdDataCrcCalc() == GetRdDataCrc() );
}
//----------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++ Ack message OP +++++++++++++++++++++++++++++++++++
//----------------------------------------------------------------------------
uint16_t RawMessage::AckCrcCalc() const
{
#ifdef USE_VHDL_CRC
   return VhdlCalcCrc4Pol9Len20( GetAckCrcInput() );
#else
   return CalcCrc4Pol9Len20( GetAckCrcInput() );
#endif //USE_VHDL_CRC
}
//----------------------------------------------------------------------------
bool     RawMessage::AckCrcCheck() const
{
   return ( AckCrcCalc() == GetAckCrc() );
}
//----------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++ Ack message OP +++++++++++++++++++++++++++++++++++
//----------------------------------------------------------------------------
bool     RawMessage::PrintMess(std::ostream& os, RawMessagePrintMask ctrl ) const
{
   bool bPrintHex    = static_cast< bool >(ctrl & RawMessagePrintMask::msg_print_Hex);
   bool bPrintCrc    = static_cast< bool >(ctrl & RawMessagePrintMask::msg_print_Crc);
   bool bPrintHuman  = static_cast< bool >(ctrl & RawMessagePrintMask::msg_print_Human);
   bool bPrintPrefix = static_cast< bool >(ctrl & RawMessagePrintMask::msg_print_Prefix);
   bool bPrintData   = static_cast< bool >(ctrl & RawMessagePrintMask::msg_print_Data);


   if( bPrintHex )
   {
      const uint8_t* cArrByt = reinterpret_cast<const uint8_t*> ( &fuData );

      // Set fill char to 0 and read previous state
      char cPrev = os.fill( '0' );
      os << "BE = " << std::hex
         << std::setw(2) << static_cast< uint16_t >( cArrByt[0] ) << ":"
         << std::setw(2) << static_cast< uint16_t >( cArrByt[1] ) << ":"
         << std::setw(2) << static_cast< uint16_t >( cArrByt[2] ) << ":"
         << std::setw(2) << static_cast< uint16_t >( cArrByt[3] )
         << " LE = " << std::hex
         << std::setw(2) << static_cast< uint16_t >( cArrByt[3] ) << ":"
         << std::setw(2) << static_cast< uint16_t >( cArrByt[2] ) << ":"
         << std::setw(2) << static_cast< uint16_t >( cArrByt[1] ) << ":"
         << std::setw(2) << static_cast< uint16_t >( cArrByt[0] )
         << std::dec << " => ";

         // Restore fill char to original
         os.fill( cPrev );
   } // if( ctrl & msg_print_Hex )

   /// Print Link index
   os << " Lnk: " << std::setw( 3 ) << GetLinkIndex();

   /// CRC debugging
   if( bPrintCrc )
   {
      switch( GetRawMessType() )
      {
         case RawMessType::Hit :
         {
            // No CRC => Nothing to check
            os << " -A- Hit                                    "
               << "              ";
            break;
         } // case RawMessType::Hit
         case RawMessType::TsMsb :
         {
            uint16_t usMessCrc = GetTsMsbCrc();
            uint16_t usCrcCal  = TsMsbCrcCalc();
            bool     bCrcCheck = TsMsbCrcCheck();

            os << " -B- TS MSB        CRC: " << std::setw(2) << usMessCrc
               << " Calc: " << std::setw(2) << usCrcCal
               << " check: " << bCrcCheck
               << " Input: " << std::hex << std::setw(6)
               << GetTsMsbCrcInput()
               << std::dec;

            break;
         } // case RawMessType::TsMsb
         case RawMessType::ReadDataAck :
         {
            uint16_t usMessCrc = GetRdDataCrc();
            uint16_t usCrcCal  = RdDataCrcCalc();
            bool     bCrcCheck = RdDataCrcCheck();

            os << " -C- Read Data ACK CRC: " << std::setw(2) << usMessCrc
               << " Calc: " << std::setw(2) << usCrcCal
               << " check: " << bCrcCheck
               << " Input: " << std::hex << std::setw(6)
               << GetRdDataCrcInput()
               << std::dec;
            break;
         } // case RawMessType::ReadDataAck
         case RawMessType::Ack :
         {
            uint16_t usMessCrc = GetAckCrc();
            uint16_t usCrcCal  = AckCrcCalc();
            bool     bCrcCheck = AckCrcCheck();

            os << " -D- ACK           CRC: " << std::setw(2) << usMessCrc
               << " Calc: " << std::setw(2) << usCrcCal
               << " check: " << bCrcCheck
               << " Input: " << std::hex << std::setw(6)
               << GetAckCrcInput()
               << std::dec;
            break;
         } // case RawMessType::Ack
         default:
         {
         } // default
      } // switch( GetRawMessType() )
   } // if( bPrintCrc )

   if( bPrintHuman )
   {
      switch( GetRawMessType() )
      {
         case RawMessType::Hit :
         {
            std::cout << " Ch: "      << std::setw(3) << GetHitChannel()
                      << " Adc: "     << std::setw(2) << GetHitAdc()
                      << " Ts Full: " << std::setw(4) << GetHitTimeFull()
                      << " Ts Over: " << std::hex     << GetHitTimeOver() << std::dec
                      << " Ts: "      << std::setw(3) << GetHitTime()
                      << " Missed? "  << IsHitMissedEvts();
            break;
         } // case RawMessType::Hit
         case RawMessType::TsMsb :
         {
            std::cout << " TS A: " << std::setw(2) <<  GetTsMsbValA()
                      << " (Over= " << std::hex     << (GetTsMsbValA() & 0x3) << std::dec << ")"
                      << " TS B: " << std::setw(2) <<  GetTsMsbValB()
                      << " (Over= " << std::hex     << (GetTsMsbValB() & 0x3) << std::dec << ")"
                      << " TS C: " << std::setw(2) <<  GetTsMsbValC()
                      << " (Over= " << std::hex     << (GetTsMsbValC() & 0x3) << std::dec << ")";
            break;
         } // case RawMessType::TsMsb
         case RawMessType::ReadDataAck :
         {
            break;
         } // case RawMessType::ReadDataAck
         case RawMessType::Ack :
         {
            break;
         } // case RawMessType::Ack
         default:
         {
         } // default
      } // switch( GetRawMessType() )
   } // if( bPrintHuman )

   if( bPrintPrefix )
   {
      switch( GetRawMessType() )
      {
         case RawMessType::Hit :
         {
            break;
         } // case RawMessType::Hit
         case RawMessType::TsMsb :
         {
            break;
         } // case RawMessType::TsMsb
         case RawMessType::ReadDataAck :
         {
            break;
         } // case RawMessType::ReadDataAck
         case RawMessType::Ack :
         {
            break;
         } // case RawMessType::Ack
         default:
         {
         } // default
      } // switch( GetRawMessType() )
   } // if( bPrintPrefix )

   if( bPrintData )
   {
      switch( GetRawMessType() )
      {
         case RawMessType::Hit :
         {
            break;
         } // case RawMessType::Hit
         case RawMessType::TsMsb :
         {
            break;
         } // case RawMessType::TsMsb
         case RawMessType::ReadDataAck :
         {
            break;
         } // case RawMessType::ReadDataAck
         case RawMessType::Ack :
         {
            break;
         } // case RawMessType::Ack
         default:
         {
         } // default
      } // switch( GetRawMessType() )
   } // if( bPrintData )

   // Finish with a new line => 1 line per message printout
   os << std::endl;

   return true;
}
//----------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//**************************************************************************//


//*********************** Private methods **********************************//
//----------------------------------------------------------------------------
#ifndef USE_VHDL_CRC
uint16_t RawMessage::CalcCrc3Pol5Len21( uint32_t uVal ) const
{
   uint32_t uValMask = uVal & ( (1 << 21) - 1 );
   return CRCPP::CRC::Calculate( &uValMask, sizeof(uValMask), table_crc3_5);
}
//----------------------------------------------------------------------------
uint16_t RawMessage::CalcCrc4Pol9Len20( uint32_t uVal ) const
{
   uint32_t uValMask = uVal & ( (1 << 20) - 1 );
   return CRCPP::CRC::Calculate( &uValMask, sizeof(uValMask), table_crc4_9);
}
#endif // not USE_VHDL_CRC
//----------------------------------------------------------------------------
uint16_t RawMessage::VhdlCalcCrc3Pol5Len21( uint32_t uVal ) const
{
/*
  function crc3_d21 (
    constant data_in : std_logic_vector(20 downto 0);
    constant crc_in  : std_logic_vector(2 downto 0))
    return std_logic_vector is

    variable lfsr_q : std_logic_vector(2 downto 0);
    variable lfsr_c : std_logic_vector(2 downto 0);

  begin  -- function crc3_d21

    lfsr_q    := crc_in;
    lfsr_c(0) := lfsr_q(0) xor data_in(0) xor data_in(2) xor data_in(3) xor data_in(4) xor data_in(7) xor data_in(9) xor data_in(10) xor data_in(11) xor data_in(14) xor data_in(16) xor data_in(17) xor data_in(18);
    lfsr_c(1) := lfsr_q(1) xor data_in(0) xor data_in(1) xor data_in(2) xor data_in(5) xor data_in(7) xor data_in(8) xor data_in(9) xor data_in(12) xor data_in(14) xor data_in(15) xor data_in(16) xor data_in(19);
    lfsr_c(2) := lfsr_q(2) xor data_in(1) xor data_in(2) xor data_in(3) xor data_in(6) xor data_in(8) xor data_in(9) xor data_in(10) xor data_in(13) xor data_in(15) xor data_in(16) xor data_in(17) xor data_in(20);
    return lfsr_c;
  end function crc3_d21;
*/
   uint16_t usCrcInit = 0x07;

   const uint16_t usNbBitsCrc =  3;
   const uint16_t usNbBitsVal = 21;
   std::vector< bool > vbCrcInit( usNbBitsCrc, false);
   std::vector< bool > vbVal( usNbBitsVal, false);
   std::vector< bool > vbCrc( usNbBitsCrc, false);

   for( uint16_t uBit = 0; uBit < usNbBitsCrc; uBit ++)
   {
      vbCrcInit[ uBit ] = ( usCrcInit >> uBit ) & 0x1;
      vbCrc[ uBit ]     = false;
   } // for( uint16_t uBit = 0, uBit < usNbBitsCrc; uBit ++)

   for( uint16_t uBit = 0; uBit < usNbBitsVal; uBit ++)
      vbVal[ uBit ] = ( uVal >> uBit ) & 0x1;

   vbCrc[ 0 ] = vbCrcInit[ 0 ] ^ vbVal[ 0 ] ^ vbVal[ 2 ] ^ vbVal[ 3 ] ^ vbVal[ 4 ] ^ vbVal[ 7 ] ^ vbVal[ 9 ] ^ vbVal[ 10 ] ^ vbVal[ 11 ] ^ vbVal[ 14 ] ^ vbVal[ 16 ] ^ vbVal[ 17 ] ^ vbVal[ 18 ];
   vbCrc[ 1 ] = vbCrcInit[ 1 ] ^ vbVal[ 0 ] ^ vbVal[ 1 ] ^ vbVal[ 2 ] ^ vbVal[ 5 ] ^ vbVal[ 7 ] ^ vbVal[ 8 ] ^ vbVal[  9 ] ^ vbVal[ 12 ] ^ vbVal[ 14 ] ^ vbVal[ 15 ] ^ vbVal[ 16 ] ^ vbVal[ 19 ];
   vbCrc[ 2 ] = vbCrcInit[ 2 ] ^ vbVal[ 1 ] ^ vbVal[ 2 ] ^ vbVal[ 3 ] ^ vbVal[ 6 ] ^ vbVal[ 8 ] ^ vbVal[ 9 ] ^ vbVal[ 10 ] ^ vbVal[ 13 ] ^ vbVal[ 15 ] ^ vbVal[ 16 ] ^ vbVal[ 17 ] ^ vbVal[ 20 ];

   uint16_t usCrc     = 0x00;
   for( uint16_t uBit = 0; uBit < usNbBitsCrc; uBit ++)
      usCrc += ( vbCrc[ uBit ] ? ( 0x1 << uBit ) : 0 );

   return usCrc;
}
//----------------------------------------------------------------------------
uint16_t RawMessage::VhdlCalcCrc4Pol9Len20( uint32_t uVal ) const
{
/*
  function crc4_d20 (
    constant data_in : std_logic_vector(19 downto 0);
    constant crc_in  : std_logic_vector(3 downto 0))
    return std_logic_vector is

    variable lfsr_q : std_logic_vector(3 downto 0);
    variable lfsr_c : std_logic_vector(3 downto 0);

  begin  -- function crc16_d24

    lfsr_q := crc_in;

    lfsr_c(0) := lfsr_q(2) xor lfsr_q(3) xor data_in(0) xor data_in(3) xor data_in(4) xor data_in(6) xor data_in(8) xor data_in(9) xor data_in(10) xor data_in(11) xor data_in(15) xor data_in(18) xor data_in(19);
    lfsr_c(1) := lfsr_q(0) xor lfsr_q(2) xor data_in(0) xor data_in(1) xor data_in(3) xor data_in(5) xor data_in(6) xor data_in(7) xor data_in(8) xor data_in(12) xor data_in(15) xor data_in(16) xor data_in(18);
    lfsr_c(2) := lfsr_q(0) xor lfsr_q(1) xor lfsr_q(3) xor data_in(1) xor data_in(2) xor data_in(4) xor data_in(6) xor data_in(7) xor data_in(8) xor data_in(9) xor data_in(13) xor data_in(16) xor data_in(17) xor data_in(19);
    lfsr_c(3) := lfsr_q(1) xor lfsr_q(2) xor data_in(2) xor data_in(3) xor data_in(5) xor data_in(7) xor data_in(8) xor data_in(9) xor data_in(10) xor data_in(14) xor data_in(17) xor data_in(18);

    return lfsr_c;
  end function crc4_d20;
*/
   uint16_t usCrcInit = 0x0F;

   const uint16_t usNbBitsCrc =  4;
   const uint16_t usNbBitsVal = 20;
   std::vector< bool > vbCrcInit( usNbBitsCrc, false);
   std::vector< bool > vbVal( usNbBitsVal, false);
   std::vector< bool > vbCrc( usNbBitsCrc, false);

   for( uint16_t uBit = 0; uBit < usNbBitsCrc; uBit ++)
   {
      vbCrcInit[ uBit ] = ( usCrcInit >> uBit ) & 0x1;
      vbCrc[ uBit ]     = false;
   } // for( uint16_t uBit = 0, uBit < usNbBitsCrc; uBit ++)

   for( uint16_t uBit = 0; uBit < usNbBitsVal; uBit ++)
      vbVal[ uBit ] = ( uVal >> uBit ) & 0x1;

   vbCrc[ 0 ] = vbCrcInit[ 2 ] ^ vbCrcInit[ 3 ]                  ^ vbVal[ 0 ] ^ vbVal[ 3 ] ^ vbVal[ 4 ] ^ vbVal[ 6 ] ^ vbVal[ 8 ] ^ vbVal[ 9 ] ^ vbVal[ 10 ] ^ vbVal[ 11 ] ^ vbVal[ 15 ] ^ vbVal[ 18 ] ^ vbVal[ 19 ];
   vbCrc[ 1 ] = vbCrcInit[ 0 ] ^ vbCrcInit[ 2 ]                  ^ vbVal[ 0 ] ^ vbVal[ 1 ] ^ vbVal[ 3 ] ^ vbVal[ 5 ] ^ vbVal[ 6 ] ^ vbVal[ 7 ] ^ vbVal[  8 ] ^ vbVal[ 12 ] ^ vbVal[ 15 ] ^ vbVal[ 16 ] ^ vbVal[ 18 ];
   vbCrc[ 2 ] = vbCrcInit[ 0 ] ^ vbCrcInit[ 1 ] ^ vbCrcInit[ 3 ] ^ vbVal[ 1 ] ^ vbVal[ 2 ] ^ vbVal[ 4 ] ^ vbVal[ 6 ] ^ vbVal[ 7 ] ^ vbVal[ 8 ] ^ vbVal[  9 ] ^ vbVal[ 13 ] ^ vbVal[ 16 ] ^ vbVal[ 17 ] ^ vbVal[ 19 ];
   vbCrc[ 3 ] = vbCrcInit[ 1 ] ^ vbCrcInit[ 2 ]                  ^ vbVal[ 2 ] ^ vbVal[ 3 ] ^ vbVal[ 5 ] ^ vbVal[ 7 ] ^ vbVal[ 8 ] ^ vbVal[ 9 ] ^ vbVal[ 10 ] ^ vbVal[ 14 ] ^ vbVal[ 17 ] ^ vbVal[ 18 ];

   uint16_t usCrc     = 0x00;
   for( uint16_t uBit = 0; uBit < usNbBitsCrc; uBit ++)
      usCrc += ( vbCrc[ uBit ] ? ( 0x1 << uBit ) : 0 );

   return usCrc;
}
//----------------------------------------------------------------------------
//**************************************************************************//
