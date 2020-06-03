// ------------------------------------------------------------------
// -----                     TTofTdcBoard                       -----
// -----              Created 12/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFTDCBOARD_H_
#define TTOFTDCBOARD_H_

#include <vector>

#include "TObject.h"

class TTofTdcData;
class TClonesArray;

class TTofTdcBoard : public TObject
{
   public:
      TTofTdcBoard();
      TTofTdcBoard( UInt_t uType, UInt_t uChNb );
      ~TTofTdcBoard();
      
      virtual void Clear(Option_t *option = "");
      
      void SetTdcType( UInt_t uType )            { fuTdcType = uType; };
      void SetChannelNb( UInt_t uChNb ); // Maybe call Delete/CreateArray in here to refresh the TClonesArray?
      void SetTriggerTime( UInt_t uTriggerTimeIn){ fuTriggerTime = uTriggerTimeIn; };
      void SetInvalid() { fbValidData = kFALSE; };
      void SetValid()   { fbValidData = kTRUE; };
      
      UInt_t GetTdcType()   const { return fuTdcType; };
      UInt_t GetChannelNb() const { return fuChannelNumber; };
      UInt_t GetTriggerTime() const { return fuTriggerTime; };
      Bool_t IsValid()        const { return fbValidData; };
      
      // All these functions have to be implemented in the derived classes
      // With the proper Data objects
      virtual Bool_t CreateDataArray(); // To be called in derived classes constructors
      virtual Bool_t ResizeArray();
      UInt_t AddData( TTofTdcData & dataIn );
      TTofTdcData * GetDataPtr( UInt_t uDataIndex );
      // ! no check on valid index in this one, Probably stupid method
      const TTofTdcData & GetData( UInt_t uDataIndex )        const; 
      
      // These ones should be ok even for child class (generic TClonesArray methods)
      UInt_t GetDataNb() const;
      virtual void SortData();
      
   protected:
      TClonesArray * fDataCollection;
      
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TTofTdcBoard(const TTofTdcBoard&);
      TTofTdcBoard& operator=(const TTofTdcBoard&);
      
      Bool_t fbValidData;
      UInt_t fuTdcType;
      UInt_t fuChannelNumber;
      UInt_t fuTriggerTime;   // [Clock Cycles] 
      
   ClassDef(TTofTdcBoard, 1)
};

#endif // TTOFTDCBOARD_H_
