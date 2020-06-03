// ------------------------------------------------------------------
// -----                     TTofVftxBoard                       -----
// -----              Created 12/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFVFTXDATA_H_
#define TTOFVFTXDATA_H_

#include "TTofTdcData.h"

class TTofVftxData : public TTofTdcData
{
   public:
      TTofVftxData();
      TTofVftxData( UInt_t uChan, UInt_t  uFt, 
                    UInt_t uCoarseT,  UInt_t uTot = 0,
                    UInt_t uFutBit = 0);
      ~TTofVftxData();
      
      virtual void Clear(Option_t *option = "");
                   
      // copy constructor, used by vector to copy content
      TTofVftxData(const TTofVftxData& src) : 
         TTofTdcData(src),
         fbFutureBit(src.fbFutureBit) {};
      // Comparison operator, needed for time sorting
      // To be implemeted for each TDC type as need coarse counter size
      virtual Bool_t operator <( const TTofVftxData& rhs) const; 
      virtual Bool_t operator <( const TTofTdcData& rhs) const; 
      
      virtual Int_t	Compare(      const TObject* obj) const;
      virtual Int_t	Compare(      const TTofVftxData* obj) const;
      virtual Int_t	Compare(      const TTofTdcData*  obj) const;
      virtual Bool_t	IsSortable() const { return kTRUE; };
               
      void  SetFutureBit( UInt_t uFutBit){ fbFutureBit = (1 == uFutBit) ? kTRUE: kFALSE; };
      Bool_t GetFutureBit( ){ return fbFutureBit; };
      
   private:
      Bool_t fbFutureBit;
      
   ClassDef(TTofVftxData, 1)
};

#endif // TTOFVFTXDATA_H_  
