
#include "StsXyterBetaMessage.h"

// tools


// C++11

// C/C++
#include <iomanip>
#include <vector>

// Namespace
using namespace stsxyter;    // Class own namespace
// Namespaces alias
//namespace sxm = stsxyter::BetaMessage;


//************************* Messages OP ************************************//
//----------------------------------------------------------------------------
bool     BetaMessage::PrintMess(std::ostream& os, BetaMessagePrintMask ctrl ) const
{
   bool bPrintHex    = static_cast< bool >(ctrl & BetaMessagePrintMask::msg_print_Hex);
   bool bPrintHuman  = static_cast< bool >(ctrl & BetaMessagePrintMask::msg_print_Human);
   bool bPrintPrefix = static_cast< bool >(ctrl & BetaMessagePrintMask::msg_print_Prefix);
   bool bPrintData   = static_cast< bool >(ctrl & BetaMessagePrintMask::msg_print_Data);


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

   if( bPrintHuman )
   {
      switch( GetMessType() )
      {
         case BetaMessType::Hit :
         {
            std::cout << " Ch: "      << std::setw(3) << GetHitChannel()
                      << " Adc: "     << std::setw(2) << GetHitAdc()
                      << " Ts Full: " << std::setw(4) << GetHitTimeFull()
                      << " Ts Over: " << std::hex     << GetHitTimeOver() << std::dec
                      << " Ts: "      << std::setw(3) << GetHitTime()
                      << " Missed? "  << IsHitMissedEvts();
            break;
         } // case BetaMessType::Hit
         case BetaMessType::TsMsb :
         {
            std::cout << " TS_MSB: " << std::setw(2) <<  GetTsMsbVal()
                      << " (Over= " << std::hex     << (GetTsMsbVal()) << std::dec << ")";
            break;
         } // case BetaMessType::TsMsb
         default:
         {
         } // default
      } // switch( GetMessType() )
   } // if( bPrintHuman )

   if( bPrintPrefix )
   {
      switch( GetMessType() )
      {
         case BetaMessType::Hit :
         {
            break;
         } // case BetaMessType::Hit
         case BetaMessType::TsMsb :
         {
            break;
         } // case BetaMessType::TsMsb
         default:
         {
         } // default
      } // switch( GetMessType() )
   } // if( bPrintPrefix )

   if( bPrintData )
   {
      switch( GetMessType() )
      {
         case BetaMessType::Hit :
         {
            break;
         } // case BetaMessType::Hit
         case BetaMessType::TsMsb :
         {
            break;
         } // case BetaMessType::TsMsb
         default:
         {
         } // default
      } // switch( GetMessType() )
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
//----------------------------------------------------------------------------
//**************************************************************************//
