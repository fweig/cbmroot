 // ------------------------------------------------------------------
// -----                     TTofCalibData                       -----
// -----              Created 17/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofCalibData.h"

/************************** TTofCalibData ****************************/
TTofCalibData::TTofCalibData() : 
   TObject(),
   fbFilled(kTRUE),
   fuTdcType(0),
   fuTdcBoard(0),
   fuTdcChannel(0),
   fuEdge(0),
   fdCalibTime(0),
   fdTimeOverThreshold(0)
{
   
}
TTofCalibData::TTofCalibData( UInt_t uType, UInt_t uBoard, UInt_t uChan, 
             Double_t dCalibT,  Double_t dTot, UInt_t uEdge ): 
   TObject(),
   fbFilled(kTRUE),
   fuTdcType(uType),
   fuTdcBoard(uBoard),
   fuTdcChannel(uChan),
   fuEdge(uEdge),
   fdCalibTime(dCalibT),
   fdTimeOverThreshold(dTot)
{
}
             
TTofCalibData::~TTofCalibData()
{
}

void TTofCalibData::Clear(Option_t *option)
{
   TObject::Clear( option );
   SetData( 0, 0, 0, 0.0, 0.0);
   ClearFilled();
} 

void TTofCalibData::SetData( UInt_t uType, UInt_t uBoard, UInt_t uChan,
                     Double_t dCalibT,  Double_t dTot,
                     UInt_t uEdge )
{
   SetType( uType );
   SetBoard( uBoard );
   SetChannel( uChan );
   SetEdge( uEdge );
   SetTime( dCalibT );
   SetTot( dTot );
   SetFilled();
}
Int_t	TTofCalibData::Compare(      const TObject*  obj) const
{
   return Compare( (TTofCalibData*)  obj);
}
Int_t	TTofCalibData::Compare(      const TTofCalibData*  obj) const
{
   // Class using only Time!
   if( this->GetTime() < obj->GetTime() )
      // hit ... rhs
      return -1;
   else if( this->GetTime() > obj->GetTime() )
      // rhs ... hit
      return 1;
   // rhs = hit
   else if( this->GetEdge() < obj->GetEdge() )
      // rhs is Falling edge while hit is Rising edge
      return -1;
   else return 0;
}

Double_t TTofCalibData::GetTimeDifference( const Double_t & dTimeIn )
{
   return dTimeIn - this->GetTime();
}
Double_t TTofCalibData::GetTimeDifference( const TTofCalibData & dataIn )
{
   return dataIn.GetTime() - this->GetTime();
}

