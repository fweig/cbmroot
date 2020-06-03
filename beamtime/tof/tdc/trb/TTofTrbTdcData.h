// -----------------------------------------------------------------------------
// ----- TTofTrbTdcData header file                                        -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-04-05                                 -----
// -----                                                                   -----
// ----- based on TTofVftxData by P.-A. Loizeau                            -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/tdc/vftx/TTofVftxData.h                         -----
// ----- revision 20754, 2013-07-17                                        -----
// -----------------------------------------------------------------------------

#ifndef TTOFTRBTDCDATA_H_
#define TTOFTRBTDCDATA_H_

#include "TTofTdcData.h"

class TTofTrbTdcData : public TTofTdcData
{
   public:
      TTofTrbTdcData();
      TTofTrbTdcData( UInt_t uChan, UInt_t  uFt, 
                      UInt_t uCoarseT, UInt_t uTot, UInt_t uEdge,
                      UInt_t uEpoch, ULong64_t uFullCoarseTime );
      ~TTofTrbTdcData();
      
      virtual void Clear(Option_t *option = "");
                   
      // copy constructor, used by vector to copy content
      TTofTrbTdcData(const TTofTrbTdcData& src) : 
         TTofTdcData(src),
         fuEpoch(src.fuEpoch),
         fuFullCoarseTime(src.fuFullCoarseTime) {};

      // Comparison operator, needed for time sorting
      // To be implemeted for each TDC type as need coarse counter size
      virtual Bool_t operator <( const TTofTrbTdcData& rhs) const; 
      virtual Bool_t operator <( const TTofTdcData& rhs) const; 
      
      virtual Int_t Compare(      const TObject*      obj) const;
      virtual Int_t Compare(      const TTofTrbTdcData* obj) const;
      virtual Int_t Compare(      const TTofTdcData*  obj) const;

      virtual Bool_t	IsSortable() const { return kTRUE; };

      void SetEpoch( UInt_t uEpoch ) { fuEpoch = uEpoch; };
      UInt_t GetEpoch() const { return fuEpoch; };

      void SetFullCoarseTime( ULong64_t uFullCoarseTime ) { fuFullCoarseTime = uFullCoarseTime; };
      ULong64_t GetFullCoarseTime() const { return fuFullCoarseTime; };

   private:
      UInt_t fuEpoch;
      ULong64_t fuFullCoarseTime;
      
      
   ClassDef(TTofTrbTdcData, 1)
};

#endif
