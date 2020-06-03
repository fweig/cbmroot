// ------------------------------------------------------------------
// -----                     TTofCalibScaler                    -----
// -----              Created 08/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFCALIBSSCALER_H_
#define TTOFCALIBSSCALER_H_

// ROOT headers
#include "TObject.h"

// C/C++ headers
#include <vector>

class TTofCalibScaler : public TObject
{
   public:
      TTofCalibScaler();
      TTofCalibScaler( UInt_t uType );
      TTofCalibScaler( UInt_t uType, UInt_t uScalNb, UInt_t uChNb );
      ~TTofCalibScaler();
      
      virtual void Clear(Option_t *option = "");

      void SetPresentFlag( Bool_t bDataThere ) { fbScalerFoundInEvent = bDataThere; };
      void SetType( UInt_t uType ); // Set the scaler type from list defined in TofScalerDef.h, then the scaler size from other Def headers
      void SetScalerNumber( UInt_t uScalNb ); // if 0 < fuChannelNumber, update fvuDataCollection size or create it
      void SetChannelNumber( UInt_t uChNb );  // if 0 < fuScalerNumber, update fvuDataCollection size or create it
      void SetBoardCharac( UInt_t uScalNb, UInt_t uChNb); // update fvuDataCollection size or create it
      void SetScalerValue( UInt_t uChan, Double_t dVal, UInt_t uScaler = 0 );
      void SetRefClk( Double_t dRefClk )          { fdReferenceClock = dRefClk; };
      void SetTimeToFirst( Double_t dTimeFirst )  { fdTimeSinceFirst = dTimeFirst; };
      void SetTimeToLast( Double_t dTimeLast )    { fdTimeSinceLast = dTimeLast; };

      Bool_t   IsUpdated()          const { return fbScalerFoundInEvent; };
      UInt_t   GetScalerType()      const { return fuScalerType; };
      UInt_t   GetScalerNumber()    const { return fuScalerNumber; };
      UInt_t   GetChannelNumber()   const { return fuChannelNumber; };
      Double_t GetRefClk()          const { return fdReferenceClock; };
      Double_t GetTimeToFirst()     const { return fdTimeSinceFirst; };
      Double_t GetTimeToLast()      const { return fdTimeSinceLast; };
      Double_t GetScalerValue( UInt_t uChannel, UInt_t uScaler = 0 ) const;
      
   private:
      Bool_t fbScalerFoundInEvent;
      UInt_t fuScalerType;
      UInt_t fuScalerNumber;
      UInt_t fuChannelNumber;
      
      /* Internal 781250 Hz reference clock for rates calculation */
      Double_t fdReferenceClock;
      
      /* Time since the first event, calculated using the reference clock values and frequency */
      Double_t fdTimeSinceFirst;
      /* Time since the last event, calculated using the reference clock values and frequency */
      Double_t fdTimeSinceLast;
      
      std::vector< std::vector< Double_t > > fvdDataCollection;
      
   ClassDef(TTofCalibScaler, 1)
};

#endif // TTOFCALIBSSCALER_H_   
