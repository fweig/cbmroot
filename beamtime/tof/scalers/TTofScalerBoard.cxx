// ------------------------------------------------------------------
// -----                     TTofScalerBoard                       -----
// -----              Created 21/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofScalerBoard.h"

// TOF headers
#include "TofScalerDef.h"
#include "TofTriglogDef.h"
#include "TofScomDef.h"
#include "TofScal2014Def.h"
#include "TofOrGenDef.h"

// FAIR headers
#include "FairLogger.h"

// ROOT headers

// C/C++ headers
      
/************************** TTofScalerBoard ****************************/
TTofScalerBoard::TTofScalerBoard():
   fbScalerFoundInEvent(kFALSE),
   fuScalerType(0),
   fuScalerNumber(0),
   fuChannelNumber(0),
   fuReferenceClock(0),
   fvuDataCollection()
{
   fvuDataCollection.clear();
}
TTofScalerBoard::TTofScalerBoard( UInt_t uType ) :
   fbScalerFoundInEvent(kFALSE),
   fuScalerType(uType),
   fuScalerNumber(0),
   fuChannelNumber(0),
   fuReferenceClock(0),
   fvuDataCollection()
{
   switch( fuScalerType )
   { 
      case tofscaler::undef :
         fuScalerNumber  = 0;
         fuChannelNumber = 0;
         fvuDataCollection.clear();
         break;
      case tofscaler::triglog :
      case tofscaler::triglogscal :
         fuScalerNumber  = triglog::kuNbScalers;
         fuChannelNumber = triglog::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scalormu :
         fuScalerNumber  = scalormu::kuNbScalers;
         fuChannelNumber = scalormu::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scalormubig :
         fuScalerNumber  = scalormuBig::kuNbScalers;
         fuChannelNumber = scalormuBig::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scaler2014 :
         fuScalerNumber  = scaler2014::kuNbScalers;
         fuChannelNumber = scaler2014::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber );
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::orgen :
         fuScalerNumber  = orgen::kuNbScalers;
         fuChannelNumber = orgen::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber );
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      default:
         fuScalerNumber = 0;
         fuChannelNumber = 0;
         fvuDataCollection.clear();
         break;
   } // switch( fuScalerType )
}
TTofScalerBoard::TTofScalerBoard( UInt_t uType, UInt_t uScalNb, UInt_t uChNb ) :
   fbScalerFoundInEvent(kFALSE),
   fuScalerType(uType),
   fuScalerNumber(uScalNb),
   fuChannelNumber(uChNb),
   fuReferenceClock(0),
   fvuDataCollection()
{
   if( 0 < fuScalerNumber && 0 < fuChannelNumber )
   {
      fvuDataCollection.resize( fuScalerNumber ); 
      for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
         fvuDataCollection[uScaler].resize( fuChannelNumber );
   } // if( 0 < fuScalerNumber && 0 < fuChannelNumber )
}
TTofScalerBoard::~TTofScalerBoard()
{
   for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
      fvuDataCollection[uScaler].clear();
      
   fvuDataCollection.clear();
}
      
void TTofScalerBoard::Clear(Option_t *option)
{
   TObject::Clear( option );
   fbScalerFoundInEvent = kFALSE;
   fuReferenceClock = 0;
   for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
      for( UInt_t uCh = 0; uCh < fuChannelNumber; uCh ++)
      fvuDataCollection[uScaler][uCh] = 0;
}
 // Set the scaler type from list defined in TofScalerDef.h, then the scaler size from other Def headers
void TTofScalerBoard::SetType( UInt_t uType )
{
   fuScalerType = uType;
   switch( fuScalerType )
   { 
      case tofscaler::undef :
         fuScalerNumber  = 0;
         fuChannelNumber = 0;
         fvuDataCollection.clear();
         break;
      case tofscaler::triglog :
      case tofscaler::triglogscal :
         fuScalerNumber  = triglog::kuNbScalers;
         fuChannelNumber = triglog::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scalormu :
         fuScalerNumber  = scalormu::kuNbScalers;
         fuChannelNumber = scalormu::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scalormubig :
         fuScalerNumber  = scalormuBig::kuNbScalers;
         fuChannelNumber = scalormuBig::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scaler2014 :
         fuScalerNumber  = scaler2014::kuNbScalers;
         fuChannelNumber = scaler2014::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber );
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::orgen :
         fuScalerNumber  = orgen::kuNbScalers;
         fuChannelNumber = orgen::kuNbChan;
         fvuDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvuDataCollection[uScaler].resize( fuChannelNumber );
         break;
      default:
         fuScalerNumber = 0;
         fuChannelNumber = 0;
         fvuDataCollection.clear();
         break;
   } // switch( fuScalerType )
}
// if 0 < fuChannelNumber, update fvuDataCollection size or create it
void TTofScalerBoard::SetScalerNumber( UInt_t uScalNb )
{
   fuScalerNumber = uScalNb;
   if( 0 < fuScalerNumber && 0 < fuChannelNumber )
   {
      fvuDataCollection.resize( fuScalerNumber ); 
      for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
         fvuDataCollection[uScaler].resize( fuChannelNumber );
   } // if( 0 < fuScalerNumber && 0 < fuChannelNumber )
} 
// if 0 < fuScalerNumber, update fvuDataCollection size or create it
void TTofScalerBoard::SetChannelNumber( UInt_t uChNb )
{
   fuChannelNumber = uChNb;
   if( 0 < fuScalerNumber && 0 < fuChannelNumber )
   {
      fvuDataCollection.resize( fuScalerNumber ); 
      for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
         fvuDataCollection[uScaler].resize( fuChannelNumber );
   } // if( 0 < fuScalerNumber && 0 < fuChannelNumber )
}  
// update fvuDataCollection size or create it
void TTofScalerBoard::SetBoardCharac( UInt_t uScalNb, UInt_t uChNb)
{
   fuScalerNumber = uScalNb;
   fuChannelNumber = uChNb;
   if( 0 < fuScalerNumber && 0 < fuChannelNumber )
   {
      fvuDataCollection.resize( fuScalerNumber ); 
      for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
         fvuDataCollection[uScaler].resize( fuChannelNumber );
   } // if( 0 < fuScalerNumber && 0 < fuChannelNumber )
} 
void TTofScalerBoard::SetScalerValue( UInt_t uChan, UInt_t uVal, UInt_t uScaler)
{
   if( uScaler < fuScalerNumber && uChan < fuChannelNumber )
      fvuDataCollection[uScaler][uChan] = uVal;
}

UInt_t TTofScalerBoard::GetScalerValue( UInt_t uChannel, UInt_t uScaler) const
{
   if( uScaler < fuScalerNumber && uChannel < fuChannelNumber )
      return fvuDataCollection[uScaler][uChannel];
      else return 0;
}
