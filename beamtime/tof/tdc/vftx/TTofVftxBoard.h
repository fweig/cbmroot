// ------------------------------------------------------------------
// -----                     TTofVftxBoard                       -----
// -----              Created 12/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFVFTXBOARD_H_
#define TTOFVFTXBOARD_H_

#include "TTofTdcBoard.h"

class TTofVftxData;

class TTofVftxBoard : public TTofTdcBoard
{
   public:
      TTofVftxBoard();
      ~TTofVftxBoard();
      
      virtual void Clear(Option_t *option = "");
       
      void SetTriggerType( UInt_t uTriggerTypeIn){ fuTriggerType = uTriggerTypeIn; };
      UInt_t GetTriggerType(){ return fuTriggerType; };
      
      // TClones array version
      // All these functions have to be implemented in the derived classes
      // With the proper Data objects
      virtual Bool_t CreateDataArray();
      virtual Bool_t ResizeArray();
      UInt_t AddData( TTofVftxData & dataIn );
      TTofVftxData * GetDataPtr( UInt_t uDataIndex );
      // ! no check on valid index in this one, Probably stupid method
      const TTofVftxData & GetData( UInt_t uDataIndex )        const; 
      
   private:
      UInt_t fuTriggerType;   // Specific to VFTX TDCs, 1 = Normal Trigger, 2 = Sync trigger, still there?
      
   ClassDef(TTofVftxBoard, 1)
};

#endif // TTOFVFTXBOARD_H_ 
