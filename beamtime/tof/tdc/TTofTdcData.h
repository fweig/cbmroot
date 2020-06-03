// ------------------------------------------------------------------
// -----                     TTofTdcData                       -----
// -----              Created 17/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFTDCDATA_H_
#define TTOFTDCDATA_H_

#include "TObject.h"

class TTofTdcData : public TObject
{
   public:
      TTofTdcData();
      TTofTdcData( UInt_t uChan, UInt_t  uFt, 
                   UInt_t uCoarseT,  UInt_t uTot = 0,
                   UInt_t uEdge = 0 );
      ~TTofTdcData();
      
      virtual void Clear(Option_t *option = "");
                   
      // copy constructor, used by vector to copy content
      TTofTdcData(const TTofTdcData& src) :
         TObject(),
         fuTdcChannel(src.fuTdcChannel), 
         fuEdge(src.fuEdge), 
         fuFineTime(src.fuFineTime), 
         fuCoarseTime(src.fuCoarseTime), 
         fuTimeOverThreshold(src.fuTimeOverThreshold) {};
      // Comparison operator, needed for time sorting
      // To be implemeted for each TDC type as need coarse counter size
//      virtual Bool_t operator <( const TTofTdcData& rhs) const; 
      virtual Int_t	Compare(      const TObject* obj) const;
      virtual Int_t	Compare(      const TTofTdcData*  obj) const;
      virtual Bool_t	IsSortable() const { return kTRUE; };
                   
      void SetData( UInt_t uChan, UInt_t  uFt, 
                     UInt_t uCoarseT,  UInt_t uTot = 0,
                     UInt_t uEdge = 0 );
      void SetChannel( UInt_t uChan ){ fuTdcChannel = uChan; };
      void SetEdge( UInt_t uEdge ){ fuEdge = uEdge; };
      void SetFineTime( UInt_t  uFt ){ fuFineTime = uFt; };
      void SetCoarseTime(  UInt_t uCoarseT ){ fuCoarseTime = uCoarseT; };
      void SetTot( UInt_t uTot ){ fuTimeOverThreshold = uTot; };
      
      UInt_t GetChannel()    const { return fuTdcChannel; };
      UInt_t GetEdge()       const { return fuEdge; };
      UInt_t GetFineTime()   const { return fuFineTime; };
      UInt_t GetCoarseTime() const { return fuCoarseTime; };
      UInt_t GetTot()        const { return fuTimeOverThreshold; };
      
   private:
      UInt_t fuTdcChannel;
      UInt_t fuEdge;
      UInt_t fuFineTime;          // [Time bins]
      UInt_t fuCoarseTime;        // [Clock Cycles]
      UInt_t fuTimeOverThreshold; // [ToT bins]
      
   ClassDef(TTofTdcData, 1)
};

#endif // TTOFTDCDATA_H_ 
