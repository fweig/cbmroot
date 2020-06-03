// -----------------------------------------------------------------------------
// ----- TTofTrbTdcData source file                                        -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-04-05                                 -----
// -----                                                                   -----
// ----- based on TTofVftxData by P.-A. Loizeau                            -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/tdc/vftx/TTofVftxData.cxx                       -----
// ----- revision 20754, 2013-07-17                                        -----
// -----------------------------------------------------------------------------

#include "TTofTrbTdcData.h"

// Defines
#include "TofTrbTdcDef.h"

ClassImp(TTofTrbTdcData)

TTofTrbTdcData::TTofTrbTdcData() : 
   TTofTdcData(),
   fuEpoch(0),
   fuFullCoarseTime(0)
{
}

TTofTrbTdcData::TTofTrbTdcData( UInt_t uChan, UInt_t  uFt, 
                                UInt_t uCoarseT,  UInt_t uTot, UInt_t uEdge,
                                UInt_t uEpoch, ULong64_t uFullCoarseTime ) : 
   TTofTdcData(uChan, uFt, uCoarseT,  uTot, uEdge),
   fuEpoch(uEpoch),
   fuFullCoarseTime(uFullCoarseTime)
{
}

TTofTrbTdcData::~TTofTrbTdcData()
{
}

void TTofTrbTdcData::Clear(Option_t *option)
{
   TTofTdcData::Clear(option);
   fuEpoch = 0;
   fuFullCoarseTime = 0;
}

// Define < operator for vector sorting
Bool_t TTofTrbTdcData::operator <( const TTofTrbTdcData& rhs) const
{
   if( rhs.GetCoarseTime() < trbtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= rhs.GetCoarseTime() + trbtdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return kTRUE;
   else if( this->GetCoarseTime() < trbtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + trbtdc::kuCoarseOverflowTest <= rhs.GetCoarseTime() )
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

Bool_t TTofTrbTdcData::operator <( const TTofTdcData& rhs) const
{
   if( rhs.GetCoarseTime() < trbtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= rhs.GetCoarseTime() + trbtdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return kTRUE;
   else if( this->GetCoarseTime() < trbtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + trbtdc::kuCoarseOverflowTest <= rhs.GetCoarseTime() )
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
Int_t	TTofTrbTdcData::Compare(      const TObject*  obj) const
{
   return Compare( (TTofTrbTdcData*)  obj);
}
Int_t	TTofTrbTdcData::Compare(      const TTofTrbTdcData* obj) const
{
   if( obj->GetCoarseTime() < trbtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= obj->GetCoarseTime() + trbtdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return -1;
   else if( this->GetCoarseTime() < trbtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + trbtdc::kuCoarseOverflowTest <= obj->GetCoarseTime() )
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
Int_t	TTofTrbTdcData::Compare(      const TTofTdcData*  obj) const
{
   if( obj->GetCoarseTime() < trbtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() >= obj->GetCoarseTime() + trbtdc::kuCoarseOverflowTest )
      // Corse counter overflow between this hit and checked one
      // hit ... Ovflw ... rhs
      return -1;
   else if( this->GetCoarseTime() < trbtdc::kuCoarseOverflowTest &&
         this->GetCoarseTime() + trbtdc::kuCoarseOverflowTest <= obj->GetCoarseTime() )
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
