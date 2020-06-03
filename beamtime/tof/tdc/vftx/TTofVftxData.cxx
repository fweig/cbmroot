// ------------------------------------------------------------------
// -----                     TTofVftxData                       -----
// -----              Created 12/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofVftxData.h"

// Tof headers
#include "TofVftxDef.h"

/************************** TTofTdcBoard ****************************/
TTofVftxData::TTofVftxData() : 
   TTofTdcData(),
   fbFutureBit(0)
{
}
TTofVftxData::TTofVftxData( UInt_t uChan, UInt_t  uFt, 
                            UInt_t uCoarseT,  UInt_t uTot,
                            UInt_t uFutBit ) : 
   TTofTdcData(uChan, uFt, uCoarseT,  uTot),
   fbFutureBit(uFutBit)
{
}

TTofVftxData::~TTofVftxData()
{
}

void TTofVftxData::Clear(Option_t *option)
{
   TTofTdcData::Clear(option);
   fbFutureBit = 0;
}

// Define < operator for vector sorting
Bool_t TTofVftxData::operator <( const TTofVftxData& rhs) const
{
   if( rhs.GetCoarseTime() < vftxtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= rhs.GetCoarseTime() + vftxtdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return kTRUE;
   else if( this->GetCoarseTime() < vftxtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + vftxtdc::kuCoarseOverflowTest <= rhs.GetCoarseTime() )
      // Corse counter overflow between checked hit and this one
      // rhs ... Ovflw ... hit
      return kFALSE;
   else if( this->GetCoarseTime() < rhs.GetCoarseTime() )
      // CT hit ... rhs
      return kTRUE;
   else if( this->GetCoarseTime() > rhs.GetCoarseTime() )
      // CT rhs ... hit
      return kFALSE;
   else if( this->GetFineTime() > rhs.GetFineTime())
      // Comparator inverted as FineTime is counting
      // Backward from next epoch!!!!!
      // CT rhs = hit
      // FT hit ... rhs
      return kTRUE;
   // CT rhs = hit
   // FT rhs ... hit or rhs = hit
   else return kFALSE;
}  

Bool_t TTofVftxData::operator <( const TTofTdcData& rhs) const
{
   if( rhs.GetCoarseTime() < vftxtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= rhs.GetCoarseTime() + vftxtdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return kTRUE;
   else if( this->GetCoarseTime() < vftxtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + vftxtdc::kuCoarseOverflowTest <= rhs.GetCoarseTime() )
      // Corse counter overflow between checked hit and this one
      // rhs ... Ovflw ... hit
      return kFALSE;
   else if( this->GetCoarseTime() < rhs.GetCoarseTime() )
      // CT hit ... rhs
      return kTRUE;
   else if( this->GetCoarseTime() > rhs.GetCoarseTime() )
      // CT rhs ... hit
      return kFALSE;
   else if( this->GetFineTime() > rhs.GetFineTime())
      // Comparator inverted as FineTime is counting
      // Backward from next epoch!!!!!
      // CT rhs = hit
      // FT hit ... rhs
      return kTRUE;
   // CT rhs = hit
   // FT rhs ... hit or rhs = hit
   else return kFALSE;
}  
Int_t	TTofVftxData::Compare(      const TObject*  obj) const
{
   return Compare( (TTofVftxData*)  obj);
}
Int_t	TTofVftxData::Compare(      const TTofVftxData* obj) const
{
   if( obj->GetCoarseTime() < vftxtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= obj->GetCoarseTime() + vftxtdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return -1;
   else if( this->GetCoarseTime() < vftxtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + vftxtdc::kuCoarseOverflowTest <= obj->GetCoarseTime() )
      // Corse counter overflow between checked hit and this one
      // rhs ... Ovflw ... hit
      return 1;
   else if( this->GetCoarseTime() < obj->GetCoarseTime() )
      // CT hit ... rhs
      return -1;
   else if( this->GetCoarseTime() > obj->GetCoarseTime() )
      // CT rhs ... hit
      return 1;
   else if( this->GetFineTime() > obj->GetFineTime())
      // Comparator inverted as FineTime is counting
      // Backward from next epoch!!!!!
      // CT rhs = hit
      // FT hit ... rhs
      return -1;
   else if( this->GetFineTime() < obj->GetFineTime())
      // Comparator inverted as FineTime is counting
      // Backward from next epoch!!!!!
      // CT rhs = hit
      // FT rhs ... hit
      return 1;
   // CT rhs = hit
   // FT rhs = hit
   else return 0;
}
Int_t	TTofVftxData::Compare(      const TTofTdcData*  obj) const
{
   if( obj->GetCoarseTime() < vftxtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= obj->GetCoarseTime() + vftxtdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return -1;
   else if( this->GetCoarseTime() < vftxtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + vftxtdc::kuCoarseOverflowTest <= obj->GetCoarseTime() )
      // Corse counter overflow between checked hit and this one
      // rhs ... Ovflw ... hit
      return 1;
   else if( this->GetCoarseTime() < obj->GetCoarseTime() )
      // CT hit ... rhs
      return -1;
   else if( this->GetCoarseTime() > obj->GetCoarseTime() )
      // CT rhs ... hit
      return 1;
   else if( this->GetFineTime() > obj->GetFineTime())
      // Comparator inverted as FineTime is counting
      // Backward from next epoch!!!!!
      // CT rhs = hit
      // FT hit ... rhs
      return -1;
   else if( this->GetFineTime() < obj->GetFineTime())
      // Comparator inverted as FineTime is counting
      // Backward from next epoch!!!!!
      // CT rhs = hit
      // FT rhs ... hit
      return 1;
   // CT rhs = hit
   // FT rhs = hit
   else return 0;
}
