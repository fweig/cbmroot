// ------------------------------------------------------------------
// -----                     TTofTdcBoard                       -----
// -----              Created 12/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofVftxBoard.h"

// TOF headers
#include "TofTdcDef.h"
#include "TofVftxDef.h"
#include "TTofVftxData.h"

// FAIR headers
#include "FairLogger.h"

// ROOT headers
#include "TClonesArray.h"

// C/C++ headers

/************************** TTofTdcBoard ****************************/
TTofVftxBoard::TTofVftxBoard():
   TTofTdcBoard( toftdc::vftx, vftxtdc::kuNbChan ),
   fuTriggerType(0)
{
   CreateDataArray();
}
TTofVftxBoard::~TTofVftxBoard()
{
}

void TTofVftxBoard::Clear( Option_t *option )
{
   TTofTdcBoard::Clear( option );
   fuTriggerType = 0;
}

// TClonesArray versions
Bool_t TTofVftxBoard::CreateDataArray()
{
   LOG(debug)<<" TTofVftxBoard::CreateDataArray ";
   if( 0 < GetChannelNb() && NULL == fDataCollection )
      fDataCollection = new TClonesArray( "TTofVftxData", vftxtdc::kuNbMulti * GetChannelNb() );
      else return kFALSE;
      
   return kTRUE;
}
Bool_t TTofVftxBoard::ResizeArray()
{ 
   if( 0 < GetChannelNb() && NULL != fDataCollection )
   {
      fDataCollection->Expand( vftxtdc::kuNbMulti*GetChannelNb() );
      return kTRUE;
   } // if( 0 < GetChannelNb()  && NULL != fDataCollection )
      else return kFALSE;
}
UInt_t TTofVftxBoard::AddData( TTofVftxData & dataIn )
{
   if( NULL != fDataCollection )
   {
      Int_t iNextIndex = fDataCollection->GetEntriesFast();
      if( iNextIndex < static_cast<Int_t>(vftxtdc::kuNbMulti * GetChannelNb()) )
      {
         TTofVftxData * dataSlot = (TTofVftxData *)fDataCollection->ConstructedAt( iNextIndex );
         *dataSlot = dataIn;
      } // if( iNextIndex < toftdc::kuDefNbMulti * GetChannelNb() )
      return fDataCollection->GetEntriesFast();
   } // if( NULL != fDataCollection )
      else return 0;
}
TTofVftxData * TTofVftxBoard::GetDataPtr( UInt_t uDataIndex )
{
   if( NULL != fDataCollection )
   {
      if( static_cast<Int_t>(uDataIndex) < fDataCollection->GetEntriesFast() )
         return (TTofVftxData *)fDataCollection->At( uDataIndex );
         else return NULL;
   } // if( NULL != fDataCollection )
      else return NULL;
}
// ! no check on valid index in this one, Probably stupid method
const TTofVftxData & TTofVftxBoard::GetData( UInt_t uDataIndex )        const
{
   return *( (TTofVftxData *)fDataCollection->At( uDataIndex ) );
}
