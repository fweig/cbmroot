// -----------------------------------------------------------------------------
// ----- TTofTrbTdcBoard header file                                       -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-04-05                                 -----
// -----                                                                   -----
// ----- based on TTofVftxBoard by P.-A. Loizeau                           -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/tdc/vftx/TTofVftxBoard.h                        -----
// ----- revision 20754, 2013-07-17                                        -----
// -----------------------------------------------------------------------------

#ifndef TTOFTRBTDCBOARD_H_
#define TTOFTRBTDCBOARD_H_

#include "TTofTdcBoard.h"

//class TTofTrbTdcData;
#include "TTofTrbTdcData.h"

class TTofTrbTdcBoard : public TTofTdcBoard
{
   public:
      TTofTrbTdcBoard();
      ~TTofTrbTdcBoard();
      
      virtual void Clear(Option_t *option = "");
       
      TTofTrbTdcData& GetRefChannelData() { return fRefChannelData; };

      void SetCalibData( Bool_t bIsCalibTrigger ) { fbIsCalibData = bIsCalibTrigger; };
      Bool_t IsCalibData() { return fbIsCalibData; };

      void SetChannelBufferIssue() { fbIsChannelBufferIssue = kTRUE; };
      Bool_t GetChannelBufferIssue() { return fbIsChannelBufferIssue; };

      // TClones array version
      // All these functions have to be implemented in the derived classes
      // With the proper Data objects
      virtual Bool_t CreateDataArray();
      virtual Bool_t ResizeArray();
      UInt_t AddData( TTofTrbTdcData & dataIn );
      TTofTrbTdcData * GetDataPtr( UInt_t uDataIndex );
      // ! no check on valid index in this one, Probably stupid method
      const TTofTrbTdcData & GetData( UInt_t uDataIndex ) const; 
      
   private:
      TTofTrbTdcData fRefChannelData;
      Bool_t fbIsCalibData;
      Bool_t fbIsChannelBufferIssue;
      
   ClassDef(TTofTrbTdcBoard, 1)
};

#endif // TTOFTRBTDCBOARD_H_
