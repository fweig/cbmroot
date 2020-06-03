// ------------------------------------------------------------------
// -----                     TTofGet4Data                       -----
// -----              Created 22/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFGET4DATA_H_
#define TTOFGET4DATA_H_

#include "TTofTdcData.h"

class TTofGet4Data : public TTofTdcData
{
   public:
      TTofGet4Data();
      TTofGet4Data( UInt_t uChan, UInt_t  uFt,
                      UInt_t uCoarseT,
                      ULong64_t uFulltime = 0,  Double_t dFullTime = 0.0,
                      UInt_t uGet4Epoch = 0, UInt_t uGet4EpCycle = 0,
                      UInt_t uTot = 0, Bool_t bIs32Bit = kFALSE );
      ~TTofGet4Data();
      
      virtual void Clear(Option_t *option = "");
                   
      // copy constructor, used by vector to copy content
      TTofGet4Data(const TTofGet4Data& src) :
         TTofTdcData(src),
         fuFulltime(src.fuFulltime),
         fdFullTime(src.fdFullTime),
         fuGet4Epoch(src.fuGet4Epoch),
         fuGet4EpochCycle(src.fuGet4EpochCycle),
         fbIs32Bit(src.fbIs32Bit) {};
      // Comparison operator, needed for time sorting
      // To be implemeted for each TDC type as need coarse counter size
      virtual Bool_t operator <( const TTofGet4Data& rhs) const;
      virtual Bool_t operator <( const TTofTdcData& rhs) const; 
      
      virtual Int_t	Compare(      const TObject* obj) const;
      virtual Int_t	Compare(      const TTofGet4Data* obj) const;
      virtual Int_t	Compare(      const TTofTdcData*  obj) const;
      virtual Bool_t	IsSortable() const { return kTRUE; };
      
      void     SetFullTime( ULong64_t ts)
      {
         fuFulltime=ts;
         fdFullTime = (Double_t) ts;
      };
      void     SetFullTimeD( Double_t ts)
      {
         fdFullTime = ts;
         fuFulltime = (ULong64_t)ts;
      };
      void     SetEpoch(       UInt_t uGet4Epoch) { fuGet4Epoch = uGet4Epoch; };
      void     SetEpochCycle( UInt_t uGet4EpCycle) { fuGet4EpochCycle = uGet4EpCycle; };
      void     Set32bFlag(     UInt_t u32Bit){ fbIs32Bit = (1 == u32Bit) ? kTRUE: kFALSE; };
      void     Set32bFlag(     Bool_t b32Bit = kTRUE){ fbIs32Bit = b32Bit; };

      ULong64_t GetFullTime()   const { return fuFulltime; };
      Double_t  GetFullTimeD()  const { return fdFullTime; };
      UInt_t    GetEpoch()       const { return fuGet4Epoch; };
      UInt_t    GetEpochCycle() const { return fuGet4EpochCycle; };
      Bool_t    Is32Bit( )       const { return fbIs32Bit; };

   private:
      // Full time stamp without correction, usefull for time ordering and data selection
      ULong64_t fuFulltime;
      Double_t  fdFullTime;
      
      // Get4 epoch2 counter and cycles of this counter
      UInt_t    fuGet4Epoch;
      UInt_t    fuGet4EpochCycle;

      /*
       * Flag indicating that this data point was from a GET4 readout in 32b mode
       * => Tot calculated on chip and included in data!
       */
      Bool_t    fbIs32Bit;
   ClassDef(TTofGet4Data, 1)
};

#endif // TTOFGET4DATA_H_
