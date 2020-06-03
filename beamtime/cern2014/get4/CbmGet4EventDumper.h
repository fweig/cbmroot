// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmGet4EventDumper                          -----
// -----                    Created 16.04.2015 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMGET4EVENTDUMPER_H
#define CBMGET4EVENTDUMPER_H

// Specific headers

// FAIR/CBMROOT headers
#include "FairTask.h"
#include "FairEventHeader.h"// compilation error of dictionary if not explicitly included, not sure why

// ROOT headers
#include "TTimeStamp.h"

// C++ std headers
#include <vector>

class CbmGet4EventBuffer;
//class FairRootManager;
//class FairEventHeader;
class TClonesArray;

class CbmGet4EventDumper : public FairTask
{
   public:
      CbmGet4EventDumper();
      CbmGet4EventDumper(const char* name, Int_t verbose = 1);
      virtual ~CbmGet4EventDumper();

      virtual InitStatus Init();
      virtual void Exec(Option_t* option);
      virtual void Finish();

      // Setters
      void SetMaxTimePerLoop( Double_t dTimeLimitIn ) { fdMaxTimePerLoopS = dTimeLimitIn; }
      inline void SetGet4Nb( UInt_t uNbChipsIn) { fuNbGet4 = uNbChipsIn; }
             void SetActiveGet4( UInt_t uChipsIndex, Bool_t bActiveFlag = kTRUE);
      inline void SetNbEvtBuffEmptyCall( UInt_t uNbEvt = 0 ){ fuNbEvtBuffEmptyCall = uNbEvt; };
      inline void SetDataWriting( Bool_t bWriteOn = kTRUE ){ fbWriteDataInCbmOut = bWriteOn; };

      // Getters
      Double_t GetMaxTimePerLoop() const { return fdMaxTimePerLoopS; }

   protected:

   private:
      // To get rid of warning due to pointer member
      // Implement properly if needed
      CbmGet4EventDumper(const CbmGet4EventDumper&);
      CbmGet4EventDumper& operator=(const CbmGet4EventDumper&);
  
      Bool_t   RegisterInputs();
      Bool_t   RegisterOutputs();
      Bool_t   ClearOutput();

      Int_t    ReadEvent();
      Int_t    ReadEventMore();

      Bool_t   CreateHistos();
      Bool_t   FillHistos();
      Bool_t   WriteHistos();
      Bool_t   DeleteHistos();

//      CbmGet4EventDumper(const CbmGet4EventDumper&);
//      CbmGet4EventDumper operator=(const CbmGet4EventDumper&);

      // Counters
      UInt_t              fuInputEvents; // Number of processed input "events"
      UInt_t              fuOutputEvents; // Number of processed output "events"

      // Class Members: Input and output
      CbmGet4EventBuffer* fInputBuffer; // Event buffer
      FairEventHeader   * fEvtHeader;   // Event header to keep another track of event info
      FairRootManager   * fIOMan;       // FairRoot IO manager, to control output array and event header
      TClonesArray      * fGet4BoardCollection; // Collection of GET4 board objects, updated on each event

      // Settings
      UInt_t fuNbGet4;
      std::vector< Bool_t  > fvbActiveChips; // Active flag to avoid mapping need (one per GET4 chip, def. kTRUE)
      UInt_t fuNbEvtBuffEmptyCall;  // nb of events to keep if possible to deal w/ execution call w/o events
      Bool_t fbWriteDataInCbmOut; // enable data writing in output tree file, otherwise jump emptying input buffer

      // Control
      Double_t   fdMaxTimePerLoopS;
      TTimeStamp fStart;
      TTimeStamp fStop;
      /*************************************************************/

   ClassDef(CbmGet4EventDumper, 1)
};

#endif // CBMGET4EVENTDUMPER_H
