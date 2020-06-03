// ------------------------------------------------------------------
// -----                     TTofGet4Board                       -----
// -----              Created 22/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofGet4Board.h"

// TOF headers
#include "TofTdcDef.h"
#include "TofGet4Def.h"
#include "TTofGet4Data.h"

// FAIR headers
#include "FairLogger.h"

// ROOT headers
#include "TClonesArray.h"

// C/C++ headers

/************************** TTofTdcBoard ****************************/
TTofGet4Board::TTofGet4Board():
   TTofTdcBoard( toftdc::get4, get4tdc::kuNbChan ),
   fdTriggerFullTime(0.0),
   fbMbsSyncedEvent(kTRUE),
   fuSyncEventNumber(0),
   fbDataTriggered(kTRUE),
   fuTriggerIndex(0)
{
   CreateDataArray();
}
TTofGet4Board::~TTofGet4Board()
{
}

void TTofGet4Board::Clear( Option_t *option )
{
   TTofTdcBoard::Clear( option );
   fdTriggerFullTime = 0.0;
   fbMbsSyncedEvent  = kTRUE;
   fuSyncEventNumber = 0;
   fbDataTriggered   = kTRUE;
   fuTriggerIndex    = 0;
}

// TClonesArray versions
Bool_t TTofGet4Board::CreateDataArray()
{
   LOG(debug)<<" TTofGet4Board::CreateDataArray ";
   if( 0 < GetChannelNb() && NULL == fDataCollection )
      fDataCollection = new TClonesArray( "TTofGet4Data", get4tdc::kuNbMulti * GetChannelNb() );
      else return kFALSE;
      
   return kTRUE;
}
Bool_t TTofGet4Board::ResizeArray()
{ 
   if( 0 < GetChannelNb() && NULL != fDataCollection )
   {
      fDataCollection->Expand( get4tdc::kuNbMulti*GetChannelNb() );
      return kTRUE;
   } // if( 0 < GetChannelNb()  && NULL != fDataCollection )
      else return kFALSE;
}
UInt_t TTofGet4Board::AddData( TTofGet4Data & dataIn )
{
   if( NULL != fDataCollection )
   {
      Int_t iNextIndex = fDataCollection->GetEntriesFast();
      if( iNextIndex < static_cast<Int_t>(get4tdc::kuNbMulti * GetChannelNb()) )
      {
         TTofGet4Data * dataSlot = (TTofGet4Data *)fDataCollection->ConstructedAt( iNextIndex );
         *dataSlot = dataIn;
      } // if( iNextIndex < toftdc::kuDefNbMulti * GetChannelNb() )
      return fDataCollection->GetEntriesFast();
   } // if( NULL != fDataCollection )
      else return 0;
}
TTofGet4Data * TTofGet4Board::GetDataPtr( UInt_t uDataIndex )
{
   if( NULL != fDataCollection )
   {
      if( static_cast<Int_t>(uDataIndex) < fDataCollection->GetEntriesFast() )
         return (TTofGet4Data *)fDataCollection->At( uDataIndex );
         else return NULL;
   } // if( NULL != fDataCollection )
      else return NULL;
}
// ! no check on valid index in this one, Probably stupid method
const TTofGet4Data & TTofGet4Board::GetData( UInt_t uDataIndex )        const
{
   return *( (TTofGet4Data *)fDataCollection->At( uDataIndex ) );
}
