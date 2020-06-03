// ------------------------------------------------------------------
// -----                     TTofTdcData                       -----
// -----              Created 17/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofTdcData.h"

/************************** TTofTdcData ****************************/
TTofTdcData::TTofTdcData() : 
   fuTdcChannel(0),
   fuEdge(0),
   fuFineTime(0),
   fuCoarseTime(0),
   fuTimeOverThreshold(0)
{
}
TTofTdcData::TTofTdcData( UInt_t uChan, UInt_t  uFt, 
             UInt_t uCoarseT,  UInt_t uTot, UInt_t uEdge ) :
   fuTdcChannel(uChan),
   fuEdge(uEdge),
   fuFineTime(uFt),
   fuCoarseTime(uCoarseT),
   fuTimeOverThreshold(uTot)
{
}
             
TTofTdcData::~TTofTdcData()
{
}

void TTofTdcData::Clear(Option_t *option)
{
   TObject::Clear( option );
   SetData( 0, 0, 0, 0);
} 
void TTofTdcData::SetData( UInt_t uChan, UInt_t  uFt, 
                     UInt_t uCoarseT,  UInt_t uTot,
                     UInt_t uEdge )
{
   SetChannel( uChan );
   SetEdge( uEdge );
   SetFineTime( uFt );
   SetCoarseTime( uCoarseT );
   SetTot( uTot );
}
Int_t	TTofTdcData::Compare(      const TObject*  obj) const
{
   return Compare( (TTofTdcData*)  obj);
}
Int_t	TTofTdcData::Compare(      const TTofTdcData*  obj) const
{
   // No Check on CT overflow as this is determined by TDC type 
   // => Need to be overriden in Child classes!!!!
   if( this->GetCoarseTime() < obj->GetCoarseTime() )
      // CT hit ... rhs
      return -1;
   else if( this->GetCoarseTime() > obj->GetCoarseTime() )
      // CT rhs ... hit
      return 1;
   else if( this->GetFineTime() < obj->GetFineTime())
      // CT rhs = hit
      // FT hit ... rhs
      return -1;
   else if( this->GetFineTime() > obj->GetFineTime())
      // CT rhs = hit
      // FT rhs ... hit
      return 1;
   // CT rhs = hit
   // FT rhs = hit 
   else return 0;
}
