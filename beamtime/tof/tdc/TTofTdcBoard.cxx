// ------------------------------------------------------------------
// -----                     TTofTdcBoard                       -----
// -----              Created 12/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofTdcBoard.h"

// TOF headers
#include "TTofTdcData.h"
#include "TofTdcDef.h"

// FAIR headers
#include "FairLogger.h"

// ROOT headers
#include "TClonesArray.h"

// C/C++ headers
      
/************************** TTofTdcBoard ****************************/
TTofTdcBoard::TTofTdcBoard():
   fDataCollection(NULL),
   fbValidData(kTRUE),
   fuTdcType(0),
   fuChannelNumber(0),
   fuTriggerTime(0)
{
}
TTofTdcBoard::TTofTdcBoard( UInt_t uType, UInt_t uChNb ) :
   fDataCollection(NULL),
   fbValidData(kTRUE),
   fuTdcType(uType),
   fuChannelNumber(uChNb),
   fuTriggerTime(0)
{
}
TTofTdcBoard::~TTofTdcBoard()
{
   if( 0 < fuChannelNumber  && NULL != fDataCollection )
      fDataCollection->Delete();
}
      
void TTofTdcBoard::Clear(Option_t *option)
{
   fbValidData = kTRUE; // Default, unpacker has to take care of invalidating stuff
   TObject::Clear( option );
   fuTriggerTime = 0;
   if( NULL != fDataCollection )
      fDataCollection->Clear("C");
}

void TTofTdcBoard::SetChannelNb( UInt_t uChNb )
{ 
   fuChannelNumber = uChNb; 
   ResizeArray();
}
      
Bool_t TTofTdcBoard::CreateDataArray()
{
   LOG(debug)<<" TTofTdcBoard::CreateDataArray ";
   if( 0 < fuChannelNumber  && NULL == fDataCollection )
      fDataCollection = new TClonesArray( "TTofTdcData", toftdc::kuDefNbMulti * fuChannelNumber );
      else return kFALSE;
      
   return kTRUE;
}
Bool_t TTofTdcBoard::ResizeArray()
{ 
   if( 0 < fuChannelNumber  && NULL != fDataCollection )
   {
      fDataCollection->Expand( toftdc::kuDefNbMulti*fuChannelNumber );
      return kTRUE;
   } // if( 0 < fuChannelNumber  && NULL != fDataCollection )
      else return kFALSE;
}

UInt_t TTofTdcBoard::AddData( TTofTdcData & dataIn )
{
   if( NULL != fDataCollection )
   {
      Int_t iNextIndex = fDataCollection->GetEntriesFast();
      if( iNextIndex < static_cast<Int_t>(toftdc::kuDefNbMulti * fuChannelNumber) )
      {
         TTofTdcData * dataSlot = (TTofTdcData *)fDataCollection->ConstructedAt( iNextIndex );
         *dataSlot = dataIn;
      } // if( iNextIndex < toftdc::kuDefNbMulti * fuChannelNumber )
      return fDataCollection->GetEntriesFast();
   } // if( NULL != fDataCollection )
      else return 0;
}
TTofTdcData * TTofTdcBoard::GetDataPtr( UInt_t uDataIndex )
{
   if( NULL != fDataCollection )
   {
      if( static_cast<Int_t>(uDataIndex) < fDataCollection->GetEntriesFast() )
         return (TTofTdcData *)fDataCollection->At( uDataIndex );
         else return NULL;
   } // if( NULL != fDataCollection )
      else return NULL;
}
const TTofTdcData & TTofTdcBoard::GetData( UInt_t uDataIndex ) const
{
   return *( (TTofTdcData *)fDataCollection->At( uDataIndex ) );
}
UInt_t TTofTdcBoard::GetDataNb() const
{
   if( NULL != fDataCollection )
      return fDataCollection->GetEntriesFast();
      else return 0;
}
void TTofTdcBoard::SortData()
{
   if( NULL != fDataCollection )
      fDataCollection->Sort();
}
