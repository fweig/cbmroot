// ------------------------------------------------------------------
// -----                     TTofGet4Board                       -----
// -----              Created 22/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFGET4BOARD_H_
#define TTOFGET4BOARD_H_

#include "TTofTdcBoard.h"

class TTofGet4Board;
class TTofGet4Data;

class TTofGet4Board : public TTofTdcBoard
{
   public:
      TTofGet4Board();
      ~TTofGet4Board();
      
      virtual void Clear(Option_t *option = "");

      void    SetTriggerFullTime( Double_t dTriggerTimeIn){ fdTriggerFullTime = dTriggerTimeIn; };
      void    SetSyncEventFlag( Bool_t bSyncEvent = kTRUE){ fbMbsSyncedEvent = bSyncEvent; };
      void    SetSyncEvtNb( UInt_t uSyncEvtNbIn ){ fuSyncEventNumber = uSyncEvtNbIn; };
      void    SetDataEventFlag( Bool_t bDataEvent = kTRUE ){ fbDataTriggered = bDataEvent; };
      void    SetTriggIndx( UInt_t uTrigIndx ){ fuTriggerIndex = uTrigIndx; };

      Double_t GetTriggerFullTime() const { return fdTriggerFullTime; };
      Bool_t   IsSyncTriggered()     const { return fbMbsSyncedEvent; };
      UInt_t   GetSyncEvtNb()        const { return fuSyncEventNumber; };
      Bool_t   IsDataTriggered()     const { return fbDataTriggered; };
      UInt_t   GetTriggIndx()        const { return fuTriggerIndex; };
      
      // TClones array version
      // All these functions have to be implemented in the derived classes
      // With the proper Data objects
      virtual Bool_t CreateDataArray();
      virtual Bool_t ResizeArray();
      UInt_t AddData( TTofGet4Data & dataIn );
      TTofGet4Data * GetDataPtr( UInt_t uDataIndex );
      // ! no check on valid index in this one, Probably stupid method
      const TTofGet4Data & GetData( UInt_t uDataIndex )        const;
      
   private:
      
      /** Last time of trigger signal (defined by parameter)
       ** Can be sync or aux message, depending on TGet4Parameter::triggerSignal setup if pseudo-trigger mode
       ** Time of the main trigger channel if self-trigger mode
       ** Should be the same time for all Boards of a same ROC!!!!
       **/
      Double_t fdTriggerFullTime;

      /*
      // Event number of the corresponding MBS event
      UInt_t   fuMbsEventNumber;
      // Index of the real event inside MBS event
      UInt_t   fuEventNbInsideMbsEvt;
      // Total event number
      UInt_t   fuGlobalEventNumber;
      */

      // Flag indicating that this real event is the one synchronized to MBS
      Bool_t   fbMbsSyncedEvent;
      // Event Number from the sync message
      UInt_t   fuSyncEventNumber;

      // Flag indicating that this real event was made after finding coincidence in data
      Bool_t   fbDataTriggered;
      // Index of the Data trigger making this event
      UInt_t   fuTriggerIndex;

   ClassDef(TTofGet4Board, 1)
};

#endif // TTOFGET4BOARD_H_
