// -----------------------------------------------------------------------------
// ----- TTofTrbTdcBoard source file                                        -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-04-06                                 -----
// -----                                                                   -----
// ----- based on TTofVftxBoard by P.-A. Loizeau                            -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/tdc/vftx/TTofVftxBoard.cxx                       -----
// ----- revision 20754, 2013-07-17                                        -----
// -----------------------------------------------------------------------------

#include "TTofTrbTdcBoard.h"

// TOF headers
#include "TofTdcDef.h"
#include "TofTrbTdcDef.h"
#include "TTofTrbTdcData.h"

// FAIR headers
#include "FairLogger.h"

// ROOT headers
#include "TClonesArray.h"

TTofTrbTdcBoard::TTofTrbTdcBoard():
   TTofTdcBoard( toftdc::trb, trbtdc::kuNbChan ),
   fRefChannelData(TTofTrbTdcData()),
   fbIsCalibData(kFALSE),
   fbIsChannelBufferIssue(kFALSE)
{
   CreateDataArray();
}

TTofTrbTdcBoard::~TTofTrbTdcBoard()
{
}

void TTofTrbTdcBoard::Clear( Option_t *option )
{
   TTofTdcBoard::Clear( option );
   fRefChannelData.Clear( option );
   fbIsCalibData = kFALSE;
   fbIsChannelBufferIssue = kFALSE;
}

// TClonesArray versions
Bool_t TTofTrbTdcBoard::CreateDataArray()
{
   LOG(debug)<<" TTofTrbTdcBoard::CreateDataArray ";
   if( 0 < GetChannelNb() && NULL == fDataCollection )
      fDataCollection = new TClonesArray( "TTofTrbTdcData", trbtdc::kuNbMulti * GetChannelNb() );
      else return kFALSE;
      
   return kTRUE;
}
Bool_t TTofTrbTdcBoard::ResizeArray()
{ 
   if( 0 < GetChannelNb() && NULL != fDataCollection )
   {
      fDataCollection->Expand( trbtdc::kuNbMulti*GetChannelNb() );
      return kTRUE;
   } // if( 0 < GetChannelNb()  && NULL != fDataCollection )
      else return kFALSE;
}
UInt_t TTofTrbTdcBoard::AddData( TTofTrbTdcData & dataIn )
{
   if( NULL !=  fDataCollection )
   {
      Int_t iNextIndex = fDataCollection->GetEntriesFast();
      // Limiting the TClonesArray to trbtdc::kuNbMulti*trbtdc::kuNbChan entries
      // might lead to unwanted data discard if the system is readout periodically
      // without a trigger window enabled. The TClonesArray automatically expands
      // if the original number of entries is exceeded by an out-of-range call
      // of TClonesArray::ConstructedAt(). Due to the initial reservation of
      // memory for trbtdc::kuNbMulti*trbtdc::kuNbChan hits per TDC per event
      // we will most likely not face any performance penalty due to array
      // expansion in triggered readout mode.
//      if( iNextIndex < static_cast<Int_t>(trbtdc::kuNbMulti * GetChannelNb()) )
//      {
         TTofTrbTdcData * dataSlot = (TTofTrbTdcData *)fDataCollection->ConstructedAt( iNextIndex );
         // (implicit) copy assignment operator
         *dataSlot = dataIn;
//      } // if( iNextIndex < toftdc::kuDefNbMulti * GetChannelNb() )
      return fDataCollection->GetEntriesFast();
   } // if( NULL !=  fDataCollection )
      else return 0;
}
TTofTrbTdcData * TTofTrbTdcBoard::GetDataPtr( UInt_t uDataIndex )
{
   if( NULL !=  fDataCollection )
   {
      if( static_cast<Int_t>(uDataIndex) < fDataCollection->GetEntriesFast() )
         return (TTofTrbTdcData *)fDataCollection->At( uDataIndex );
         else return NULL;
   } // if( NULL !=  fDataCollection )
      else return NULL;
}
// ! no check on valid index in this one, Probably stupid method
const TTofTrbTdcData & TTofTrbTdcBoard::GetData( UInt_t uDataIndex )        const
{
   return *( (TTofTrbTdcData *)fDataCollection->At( uDataIndex ) );
}
