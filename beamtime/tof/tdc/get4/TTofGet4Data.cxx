// ------------------------------------------------------------------
// -----                     TTofGet4Data                       -----
// -----              Created 22/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofGet4Data.h"

// Tof headers
#include "TofGet4Def.h"

/************************** TTofTdcBoard ****************************/
TTofGet4Data::TTofGet4Data() :
   TTofTdcData(),
   fuFulltime(0),
   fdFullTime(0),
   fuGet4Epoch(0),
   fuGet4EpochCycle(0),
   fbIs32Bit(kFALSE)
{
}
TTofGet4Data::TTofGet4Data( UInt_t uChan, UInt_t  uFt,
                            UInt_t uCoarseT,
                            ULong64_t uFulltime,  Double_t dFullTime,
                            UInt_t uGet4Epoch, UInt_t uGet4EpCycle,
                            UInt_t uTot, Bool_t bIs32Bit) :
   TTofTdcData(uChan, uFt, uCoarseT,  uTot),
   fuFulltime(uFulltime),
   fdFullTime(dFullTime),
   fuGet4Epoch(uGet4Epoch),
   fuGet4EpochCycle(uGet4EpCycle),
   fbIs32Bit(bIs32Bit)
{
}

TTofGet4Data::~TTofGet4Data()
{
}

void TTofGet4Data::Clear(Option_t *option)
{
   TTofTdcData::Clear(option);
   fuFulltime       = 0;
   fdFullTime       = 0;
   fuGet4Epoch      = 0;
   fuGet4EpochCycle = 0;
   fbIs32Bit        = kFALSE;
}

// Define < operator for vector sorting
Bool_t TTofGet4Data::operator <( const TTofGet4Data& rhs) const
{
   if( 0.0 < rhs.GetFullTimeD() && 0.0 < this->GetFullTimeD() )
   {
      // Full time was assigned in both data!
      if( this->GetFullTimeD() < rhs.GetFullTimeD() )
         // hit ... rhs
         return kTRUE;
      else
         // rhs ... hit or rhs == hit
         return kFALSE;
   } // if( 0.0 < rhs.GetFullTimeD && 0.0 < this->GetFullTimeD() )
      else
      {
         // At least one of the Full time is not available
         if( rhs.GetCoarseTime() < get4tdc::kuCoarseOverflowTest &&
               this->GetCoarseTime() >= rhs.GetCoarseTime() + get4tdc::kuCoarseOverflowTest )
            // Corse counter overflow between this hit and checked one
            // hit ... Ovflw ... rhs
            return kTRUE;
         else if( this->GetCoarseTime() < get4tdc::kuCoarseOverflowTest &&
               this->GetCoarseTime() + get4tdc::kuCoarseOverflowTest <= rhs.GetCoarseTime() )
            // Corse counter overflow between checked hit and this one
            // rhs ... Ovflw ... hit
            return kFALSE;
         else if( this->GetCoarseTime() < rhs.GetCoarseTime() )
            // CT hit ... rhs
            return kTRUE;
         else if( this->GetCoarseTime() > rhs.GetCoarseTime() )
            // CT rhs ... hit
            return kFALSE;
         else if( this->GetFineTime() < rhs.GetFineTime())
            // CT rhs = hit
            // FT hit ... rhs
            return kTRUE;
         // CT rhs = hit
         // FT rhs ... hit or rhs = hit
         else return kFALSE;
      } // else of if( 0.0 < rhs.GetFullTimeD && 0.0 < this->GetFullTimeD() )
}  

Bool_t TTofGet4Data::operator <( const TTofTdcData& rhs) const
{
   // TTofTdcData does not have full time => rely on generic method
   if( rhs.GetCoarseTime() < get4tdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= rhs.GetCoarseTime() + get4tdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return kTRUE;
   else if( this->GetCoarseTime() < get4tdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + get4tdc::kuCoarseOverflowTest <= rhs.GetCoarseTime() )
      // Corse counter overflow between checked hit and this one
      // rhs ... Ovflw ... hit
      return kFALSE;
   else if( this->GetCoarseTime() < rhs.GetCoarseTime() )
      // CT hit ... rhs
      return kTRUE;
   else if( this->GetCoarseTime() > rhs.GetCoarseTime() )
      // CT rhs ... hit
      return kFALSE;
   else if( this->GetFineTime() < rhs.GetFineTime() )
      // CT rhs = hit
      // FT hit ... rhs
      return kTRUE;
   // CT rhs = hit
   // FT rhs ... hit or rhs = hit
   else return kFALSE;
}  
Int_t	TTofGet4Data::Compare(      const TObject*  obj) const
{
   return Compare( (TTofGet4Data*)  obj);
}
Int_t	TTofGet4Data::Compare(      const TTofGet4Data* obj) const
{
   if( 0.0 < obj->GetFullTimeD() && 0.0 < this->GetFullTimeD() )
   {
      // Full time was assigned in both data!
      if( this->GetFullTimeD() < obj->GetFullTimeD() )
         // hit ... rhs
         return -1;
      else if( this->GetFullTimeD() > obj->GetFullTimeD() )
         // rhs ... hit or rhs == hit
         return  1;
      else
         // rhs == hit
         return  0;
   } // if( 0.0 < rhs.GetFullTimeD && 0.0 < this->GetFullTimeD() )
      else
      {
         // At least one of the Full time is not available
         if( obj->GetCoarseTime() < get4tdc::kuCoarseOverflowTest &&
               this->GetCoarseTime() >= obj->GetCoarseTime() + get4tdc::kuCoarseOverflowTest )
            // Corse counter overflow between this hit and checked one
            // hit ... Ovflw ... rhs
            return -1;
         else if( this->GetCoarseTime() < get4tdc::kuCoarseOverflowTest &&
               this->GetCoarseTime() + get4tdc::kuCoarseOverflowTest <= obj->GetCoarseTime() )
            // Corse counter overflow between checked hit and this one
            // rhs ... Ovflw ... hit
            return 1;
         else if( this->GetCoarseTime() < obj->GetCoarseTime() )
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
            return   1;
         // CT rhs = hit
         // FT rhs = hit
         else return 0;
      } // else of if( 0.0 < rhs.GetFullTimeD && 0.0 < this->GetFullTimeD() )
}
Int_t	TTofGet4Data::Compare(      const TTofTdcData*  obj) const
{
   // TTofTdcData does not have full time => rely on generic method

   if( obj->GetCoarseTime() < get4tdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= obj->GetCoarseTime() + get4tdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return -1;
   else if( this->GetCoarseTime() < get4tdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + get4tdc::kuCoarseOverflowTest <= obj->GetCoarseTime() )
      // Corse counter overflow between checked hit and this one
      // rhs ... Ovflw ... hit
      return 1;
   else if( this->GetCoarseTime() < obj->GetCoarseTime() )
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
