// ------------------------------------------------------------------
// -----                     TTofGet4Unpacker                   -----
// -----              Created 22/07/2013 by P.-A. Loizeau       -----
// -----                 Adapted from TGet4v1Proc.h             -----
// -----         in RocLib/beamtime/tof-tdctest rev.4862        -----
// ----- Known limitations:                                     -----
// -----  * Restricted to single ROC runs by the way FAIRROOT   -----
// -----    deals with MBS Sub-evt to unpacker association      -----
// -----      Default = RocEvent type, ROC0, formatOptic2       -----
// -----    And also by the way chosen for FT histos names calib-----
// -----  * 32 bit mode histograms are not save in WriteHisto   -----
// -----    function                                            -----
// -----  * User has to take care of synchronizing options in   -----
// -----    TGet4v1Param and TMbsUnpackTofPar (active chips,    -----
// -----    nb chips, etc...)                                   -----
// ------------------------------------------------------------------ 

#ifndef TTOFGET4UNPACKER_H_
#define TTOFGET4UNPACKER_H_

#include "TObject.h"

#include <vector>

class TMbsUnpackTofPar;

class TClonesArray;
class TDirectory;

// Old Go4 includes
   // ROC
#include "roc/Message.h"
#include "roc/Board.h"
#include "roc/Iterator.h"

// SubEvent ProcId from ROC library
#include "commons.h"

   // C++
#include <vector>

#ifdef HAVE_UINT8_T_HEADER_FILE
#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#else
#include <stdint.h>
#endif

   // GET4
class TGet4v1Param;
#include "TGet4v1Event.h"
#include "TGet4v1Rec.h"

class TH1;
class TH2;

class TTofGet4Unpacker : public TObject
{
   public:
      TTofGet4Unpacker();
      TTofGet4Unpacker( TMbsUnpackTofPar * parIn, Short_t sProcId = roc3587::proc_RocEvent,
                          Short_t sSubCrate = 0, Short_t sControl = roc::formatOptic2 );
      ~TTofGet4Unpacker();
      
      virtual void Clear(Option_t *option);
      
      Bool_t Init();
      Bool_t InitParameters();
      Bool_t InitOutput();
      Bool_t InitConditions();

      void ProcessGet4( UInt_t* pMbsData, UInt_t uLength );
      void ProcessGet4( UInt_t* pMbsData, UInt_t uLength, Char_t cSubCrate, Char_t cControl );
      
      Bool_t CreateHistos();
      void  FillHistos();
      void  WriteHistos( TDirectory* inDir);
      void  DeleteHistos();
      
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TTofGet4Unpacker(const TTofGet4Unpacker&);
      TTofGet4Unpacker& operator=(const TTofGet4Unpacker&);
      
      TMbsUnpackTofPar * fParUnpack;
      Int_t              fiProcId;
      Char_t             fcSubCrate;
      Char_t             fcControl;
      UInt_t             fuNbTdc;
      TClonesArray     * fGet4BoardCollection;
      Bool_t             fbDataSavedForThisEvent;

      roc::MessageFormat ConvertToMsgFormat(const Char_t& cNumFormat);

      /** Method should add message to proper epoch buffer and check if necessary if it fits last trigger */
      void ProcessExtendedMessage(UInt_t uRocId, TGet4v1MessageExtended& extMess);     // <- Done
      void ProcessTriggerMessage(UInt_t uRocId, TGet4v1MessageExtended& extMess,
                                     Int_t iTriggerIndex = -1);                           // <- Done
      /** Method should check if message fits last trigger */
      Bool_t BuildHits(UInt_t uRocId, UInt_t uGet4Id, Bool_t bBufferToBuild);                // <- Done
      Bool_t CheckHit(UInt_t uRocId, UInt_t uGet4Id, Get4v1Hit& hit);                    // <- Done

      void CheckEventClosure();                                                         // <- Done

      /** GET4 unpacking & pre-processing **/
      Bool_t ProcessRocSyncMessage(    UInt_t uRocId, TGet4v1MessageExtended& extMess); // <- Done
      Bool_t ProcessRocEpochMessage(  UInt_t uRocId, TGet4v1MessageExtended& extMess);  // <- Done
      Bool_t ProcessGet4EpochMessage(  UInt_t uRocId, TGet4v1MessageExtended& extMess); // <- Done
      Bool_t ProcessGet4DataMessage(   UInt_t uRocId, TGet4v1MessageExtended& extMess); // <- Done
      Bool_t ProcessGet4ExtSyncMessage(UInt_t uRocId, TGet4v1MessageExtended& extMess); // <- NOT Done
      Bool_t ProcessGet4SuppEpochMessage(  UInt_t uRocId, TGet4v1MessageExtended& extMess); // <- Done
      Bool_t ProcessGet4SuppDataMessage(   UInt_t uRocId, TGet4v1MessageExtended& extMess); // <- Done
      Bool_t ProcessGet4ReprocessSuppData( UInt_t uRocId, UInt_t uGet4Id ); // <- Done

      Int_t Process32BitGet4Message(UInt_t uRocId, TGet4v1MessageExtended& extMess);   // <- Done

      Bool_t fb24bitsReadoutDetected;
      Bool_t fb32bitsReadoutDetected;
      Bool_t AnalyzeAllGet4Channels( Get4v1Event &eventFull );                          // <- Done
      Bool_t AnalyzeAllGet4Channels24Bits( Get4v1Event &eventFull );                   // <- Done
      Bool_t AnalyzeAllGet4Channels32Bits( Get4v1Event &eventFull );                   // <- Done

//      Bool_t PrintRocEpochIndexes(UInt_t uRocId, Int_t uMessagePriority = 0);           // <- Done
//      Bool_t PrintRocEpochCycles(UInt_t uRocId, Int_t uMessagePriority = 0);            // <- Done

      /* after event completion reset until next triggers are found*/
      void ResetTrigger();

      /* after all input buffers are done, we reset end of buffer flag*/
      void ResetEndOfBuffer();

      /* For Debug histograms*/
      void UpdateLeadingDnlHistograms( Get4v1Event &eventFull, UInt_t uRocId = 0 );
      void UpdateTrailingDnlHistograms( Get4v1Event &eventFull, UInt_t uRocId = 0 );

      TGet4v1Param   *fParam;
//      TGet4v1Event *fOutputEvent;  //! shortcut to output event
      Get4v1Event   fCurrentGet4Event;

      Bool_t fIsTimeSorted; // true if messages within mbs container are strictly time sorted

      TH1           *fEvntSize;
      TH1           *fEvtPerProc;
      TH1           *fEvtIndexRoc;
      TH1           *fEvtIndexTrig;
      TH1           *fEvtIndexVme;
      UInt_t uInitialEvent;
      UInt_t uEventCountsRoc;
      UInt_t uEventCountsTrig;
      UInt_t uEventCountsVme;
      TH1           *fTriggerNumber;

      TH1           *fMsgsPerRoc;
      TH1           *fTriggerPerRoc;

      TH1* fDeltaTriggerTime;            // absolute time difference of all hits with respect to global trigger
//      TGo4WinCond* fGlobalTriggerWind;   // time window to assign hits to output event (relative to last global trigger)
//      TGo4WinCond* fGlobalAUXWind;       // time window to assign AUX to output event (relative to last global trigger)
//      TGo4WinCond* fGlobalSelfTriggWind; // time window to detect coincidence of channel in self triggering case
      TimeWindowCond* fGlobalTriggerWind;   // time window to assign hits to output event (relative to last global trigger)
      TimeWindowCond* fGlobalAUXWind;       // time window to assign AUX to output event (relative to last global trigger)
      TimeWindowCond* fGlobalSelfTriggWind; // time window to detect coincidence of channel in self triggering case

      ULong_t fTotaldatasize;

      UInt_t         fCurrEvntTm;

      Double_t       fLastRateTm; // last time of rate measurement
      Double_t       fRate;       // currently calculated rate

      std::vector<TGet4v1Rec> ROC;  //! array of all rocs and get4v1 histograms
      Bool_t fFirstSubEvent;     //! indicate if first subevent is processing

      Bool_t fHasNewGlobalTrigger; //! true if new global trigger was found
      unsigned fTriggerCounter; //! count all trigger messages received


      /** GET4 unpacking & pre-processing **/

      // General counters (allow for example event jumps by stream server)
      UInt_t     printData;
      UInt_t     uNbEvents[get4v10::kuMaxRoc];
      UInt_t     uFirstEventIndex[get4v10::kuMaxRoc];
      UInt_t     uPrevEventIndex[get4v10::kuMaxRoc];
      
      // Real events counters
      UInt_t     uEventInsideMbsEvtCount[get4v10::kuMaxRoc];
      UInt_t     uRealEventsCount[get4v10::kuMaxRoc];
      UInt_t     uNotEmptyEventInsideMbsEvtCount[get4v10::kuMaxRoc];
      UInt_t     uNotEmptyRealEventsCount[get4v10::kuMaxRoc];

      // Variables
      Get4v1Hit fHitTemp;
      // Histograms
      

      // Get4 v1.0

      // 24 bits

      // 32 bits

   ClassDef(TTofGet4Unpacker, 1)
};

#endif // TTOFGET4UNPACKER_H_
