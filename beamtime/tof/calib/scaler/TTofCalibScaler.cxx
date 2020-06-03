// ------------------------------------------------------------------
// -----                     TTofCalibScaler                    -----
// -----              Created 08/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofCalibScaler.h"

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
      
/************************** TTofCalibScaler ****************************/
TTofCalibScaler::TTofCalibScaler():
   fbScalerFoundInEvent(kFALSE),
   fuScalerType(0),
   fuScalerNumber(0),
   fuChannelNumber(0),
   fdReferenceClock(0),
   fdTimeSinceFirst(0),
   fdTimeSinceLast(0),
   fvdDataCollection()
{
   fvdDataCollection.clear();
}
TTofCalibScaler::TTofCalibScaler( UInt_t uType ) :
   fbScalerFoundInEvent(kFALSE),
   fuScalerType(uType),
   fuScalerNumber(0),
   fuChannelNumber(0),
   fdReferenceClock(0), 
   fdTimeSinceFirst(0),
   fdTimeSinceLast(0) ,
   fvdDataCollection()
{
   switch( fuScalerType )
   { 
      case tofscaler::undef :
         fuScalerNumber  = 0;
         fuChannelNumber = 0;
         fvdDataCollection.clear();
         break;
      case tofscaler::triglog :
      case tofscaler::triglogscal :
         fuScalerNumber  = triglog::kuNbScalers;
         fuChannelNumber = triglog::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scalormu :
         fuScalerNumber  = scalormu::kuNbScalers;
         fuChannelNumber = scalormu::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scalormubig :
         fuScalerNumber  = scalormuBig::kuNbScalers;
         fuChannelNumber = scalormuBig::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scaler2014 :
         fuScalerNumber  = scaler2014::kuNbScalers;
         fuChannelNumber = scaler2014::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber );
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::orgen :
         fuScalerNumber  = orgen::kuNbScalers;
         fuChannelNumber = orgen::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber );
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      default:
         fuScalerNumber = 0;
         fuChannelNumber = 0;
         fvdDataCollection.clear();
         break;
   } // switch( fuScalerType )
}
TTofCalibScaler::TTofCalibScaler( UInt_t uType, UInt_t uScalNb, UInt_t uChNb ) :
   fbScalerFoundInEvent(kFALSE),
   fuScalerType(uType),
   fuScalerNumber(uScalNb),
   fuChannelNumber(uChNb),
   fdReferenceClock(0),
   fdTimeSinceFirst(0),
   fdTimeSinceLast(0),
   fvdDataCollection()
{
   if( 0 < fuScalerNumber && 0 < fuChannelNumber )
   {
      fvdDataCollection.resize( fuScalerNumber ); 
      for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
         fvdDataCollection[uScaler].resize( fuChannelNumber );
   } // if( 0 < fuScalerNumber && 0 < fuChannelNumber )
}
TTofCalibScaler::~TTofCalibScaler()
{
   for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
      fvdDataCollection[uScaler].clear();
      
   fvdDataCollection.clear();
}
      
void TTofCalibScaler::Clear(Option_t *option)
{
   TObject::Clear( option );
   fbScalerFoundInEvent = kFALSE;
   fdReferenceClock = 0;
   fdTimeSinceFirst = 0;
   fdTimeSinceLast = 0;
   for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
      for( UInt_t uCh = 0; uCh < fuChannelNumber; uCh ++)
      fvdDataCollection[uScaler][uCh] = 0;
}
 // Set the scaler type from list defined in TofScalerDef.h, then the scaler size from other Def headers
void TTofCalibScaler::SetType( UInt_t uType )
{
   fuScalerType = uType;
   switch( fuScalerType )
   { 
      case tofscaler::undef :
         fuScalerNumber  = 0;
         fuChannelNumber = 0;
         fvdDataCollection.clear();
         break;
      case tofscaler::triglog :
      case tofscaler::triglogscal :
         fuScalerNumber  = triglog::kuNbScalers;
         fuChannelNumber = triglog::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scalormu :
         fuScalerNumber  = scalormu::kuNbScalers;
         fuChannelNumber = scalormu::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scalormubig :
         fuScalerNumber  = scalormuBig::kuNbScalers;
         fuChannelNumber = scalormuBig::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber ); 
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::scaler2014 :
         fuScalerNumber  = scaler2014::kuNbScalers;
         fuChannelNumber = scaler2014::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber );
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      case tofscaler::orgen :
         fuScalerNumber  = orgen::kuNbScalers;
         fuChannelNumber = orgen::kuNbChan;
         fvdDataCollection.resize( fuScalerNumber );
         for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
            fvdDataCollection[uScaler].resize( fuChannelNumber );
         break;
      default:
         fuScalerNumber = 0;
         fuChannelNumber = 0;
         fvdDataCollection.clear();
         break;
   } // switch( fuScalerType )
}
// if 0 < fuChannelNumber, update fvdDataCollection size or create it
void TTofCalibScaler::SetScalerNumber( UInt_t uScalNb )
{
   fuScalerNumber = uScalNb;
   if( 0 < fuScalerNumber && 0 < fuChannelNumber )
   {
      fvdDataCollection.resize( fuScalerNumber ); 
      for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
         fvdDataCollection[uScaler].resize( fuChannelNumber );
   } // if( 0 < fuScalerNumber && 0 < fuChannelNumber )
} 
// if 0 < fuScalerNumber, update fvdDataCollection size or create it
void TTofCalibScaler::SetChannelNumber( UInt_t uChNb )
{
   fuChannelNumber = uChNb;
   if( 0 < fuScalerNumber && 0 < fuChannelNumber )
   {
      fvdDataCollection.resize( fuScalerNumber ); 
      for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
         fvdDataCollection[uScaler].resize( fuChannelNumber );
   } // if( 0 < fuScalerNumber && 0 < fuChannelNumber )
}  
// update fvdDataCollection size or create it
void TTofCalibScaler::SetBoardCharac( UInt_t uScalNb, UInt_t uChNb)
{
   fuScalerNumber = uScalNb;
   fuChannelNumber = uChNb;
   if( 0 < fuScalerNumber && 0 < fuChannelNumber )
   {
      fvdDataCollection.resize( fuScalerNumber ); 
      for( UInt_t uScaler = 0; uScaler < fuScalerNumber; uScaler ++)
         fvdDataCollection[uScaler].resize( fuChannelNumber );
   } // if( 0 < fuScalerNumber && 0 < fuChannelNumber )
} 
void TTofCalibScaler::SetScalerValue( UInt_t uChan, Double_t dVal, UInt_t uScaler)
{
   if( uScaler < fuScalerNumber && uChan < fuChannelNumber )
      fvdDataCollection[uScaler][uChan] = dVal;
}

Double_t TTofCalibScaler::GetScalerValue( UInt_t uChannel, UInt_t uScaler) const
{
   if( uScaler < fuScalerNumber && uChannel < fuChannelNumber )
      return fvdDataCollection[uScaler][uChannel];
      else return 0.0;
}
