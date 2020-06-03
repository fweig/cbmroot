// ------------------------------------------------------------------
// -----                     TTofScalerBoard                       -----
// -----              Created 21/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFSCALERBOARD_H_
#define TTOFSCALERBOARD_H_

// ROOT headers
#include "TObject.h"

// C/C++ headers
#include <vector>

class TTofScalerBoard : public TObject
{
   public:
      TTofScalerBoard();
      TTofScalerBoard( UInt_t uType );
      TTofScalerBoard( UInt_t uType, UInt_t uScalNb, UInt_t uChNb );
      ~TTofScalerBoard();
      
      virtual void Clear(Option_t *option = "");
      
      void SetPresentFlag( Bool_t bDataThere ) { fbScalerFoundInEvent = bDataThere; };
      void SetType( UInt_t uType ); // Set the scaler type from list defined in TofScalerDef.h, then the scaler size from other Def headers
      void SetScalerNumber( UInt_t uScalNb ); // if 0 < fuChannelNumber, update fvuDataCollection size or create it
      void SetChannelNumber( UInt_t uChNb );  // if 0 < fuScalerNumber, update fvuDataCollection size or create it
      void SetBoardCharac( UInt_t uScalNb, UInt_t uChNb); // update fvuDataCollection size or create it
      void SetScalerValue( UInt_t uChan, UInt_t uVal, UInt_t uScaler = 0 );
      void SetRefClk( UInt_t uRefClk )          { fuReferenceClock = uRefClk; };
      
      Bool_t IsUpdated()          const { return fbScalerFoundInEvent; };
      UInt_t GetScalerType()      const { return fuScalerType; };
      UInt_t GetScalerNumber()    const { return fuScalerNumber; };
      UInt_t GetChannelNumber()   const { return fuChannelNumber; };
      UInt_t GetRefClk()          const { return fuReferenceClock; };
      UInt_t GetScalerValue( UInt_t uChannel, UInt_t uScaler = 0 ) const;
      
   protected:
      
   private:
      Bool_t fbScalerFoundInEvent;
      UInt_t fuScalerType;
      UInt_t fuScalerNumber;
      UInt_t fuChannelNumber;
      
      /* Internal 781250 Hz reference clock for rates calculation */
      UInt_t fuReferenceClock;
      
      std::vector< std::vector< UInt_t > > fvuDataCollection;
      
   ClassDef(TTofScalerBoard, 1)
};

#endif // TTOFSCALERBOARD_H_
