// -----------------------------------------------------------------------------
// ----- TTrbUnpackTof header file                                         -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-03-08                                 -----
// -----                                                                   -----
// ----- based on TTrbUnpackTof by P.-A. Loizeau                           -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/TTrbUnpackTof.h                                 -----
// ----- revision 21379, 2013-08-24                                        -----
// -----------------------------------------------------------------------------

#ifndef TTRBUNPACKTOF_H
#define TTRBUNPACKTOF_H

// STL
#include <map>
#include <utility>

// General FairRoot unpacker of GSI/MBS events
#include "FairUnpack.h"
#include "TTrbHeader.h"

// Parameters
class TMbsUnpackTofPar;
class TMbsCalibTofPar;

// Iterator
namespace hadaq
{
  class TrbIterator;
  struct RawSubevent;
}

// Subunpacker
class TTofTrbTdcUnpacker;

// ROOT
class TClonesArray;
class TH1;
class TH2;

class TTrbUnpackTof : public FairUnpack
{
   public:
      TTrbUnpackTof( Short_t type, Short_t subType, Short_t procId, Short_t subCrate, Short_t control);
      virtual ~TTrbUnpackTof();

      // FairUnpack pure virtual methods
      virtual Bool_t Init();
      virtual Bool_t DoUnpack(Int_t* data, Int_t size);
      virtual void Reset();
      
      void WriteHistograms();

      void SetFineSpillTiming(Bool_t bFineTiming = kTRUE) {fbFineSpillTiming = bFineTiming;}
      void SetInspection(Bool_t bInspection = kTRUE) {fbInspection = bInspection;}

      void   SetSaveRawData( Bool_t bSaveRaw=kTRUE );

      Bool_t IsGoodEventInBuffer() {return fbGoodEventInBuffer;}

   protected:
      // FairUnpack pure virtual method
      // neither called by a FairSource daughter class nor by FairRunOnline
      // why made pure virtual in FairUnpack?
      virtual void Register();
    
   private:
      TTrbUnpackTof(const TTrbUnpackTof&);
      TTrbUnpackTof operator=(const TTrbUnpackTof&);

      // Methods
      Bool_t InitParameters();
      Bool_t CreateSubunpackers();
      Bool_t RegisterOutput();
      Bool_t ClearOutput();

      void DataErrorHandling(UInt_t uSubeventId, UInt_t uErrorPattern);

      // Parameters
      TMbsUnpackTofPar * fMbsUnpackPar;
      TMbsCalibTofPar * fMbsCalibPar;

      Int_t fiNbEvents;
      Int_t fiCurrentEventNumber;
      Int_t fiPreviousEventNumber;
      Int_t fiFirstEventNumber;
      Int_t fiEventNumberOverflows;

      Bool_t fbFineSpillTiming;
      Bool_t fbInspection;

      // Iterator
      hadaq::TrbIterator * fTrbIterator;
      
      // Subunpacker
      TTofTrbTdcUnpacker * fTrbTdcUnpacker;

      // Output objects
      TClonesArray * fTrbTdcBoardCollection;
      Bool_t         fbSaveRawTdcBoards;

      // Map to delay TDC data unpacking until a decision is made based on the CTS trigger pattern
      // key:   active TDC index
      // value: pair of hadaq rawsubevent pointer and subsubevent data index
      //        (input to TTofTrbTdcUnpacker::ProcessData)
      std::map<Int_t,std::pair<hadaq::RawSubevent*,UInt_t> > fTdcUnpackMap;

      void CreateHistograms();
//      void DeleteHistograms();
      void FillRunTimeHistograms();

      void CheckEventBuffer();

      UInt_t fuInDataTrbSebNb;
      UInt_t fuActiveTrbTdcNb;

      // Histograms
      TH1* fTrbTriggerPattern;
      TH1* fTrbTriggerType;
      TH1* fCtsBusyTime;
      TH1* fCtsIdleTime;
      TH1* fCtsIdleTimeSpill;
      TH1* fCtsSpillLength;
      TH1* fCtsTriggerDistance;
      TH1* fItcAssertions;
      TH1* fItcEvents;
      TH1* fHadaqEventTime;
      TH1* fTrbEventNumberJump;
      TH1* fCtsTriggerCycles;
      TH1* fCtsTriggerAccepted;
      TH1* fHadaqEventsRecorded;
      TH1* fHadaqTimeInSpill;
      TH1* fCtsTimeInSpill;
      TH2* fEventSkipsInSpill;
      TTrbHeader* fTrbHeader;
      std::vector<TH1*> fTrbSubeventSize;
      std::vector<TH1*> fTrbSubeventStatus;
      std::vector<TH1*> fTrbTdcWords;
      std::vector<TH1*> fTrbTdcProcessStatus;

      std::vector<TH2*> fTrbSubeventSizeRunTime;

      // CTS counters
      UInt_t* fuTrigChanEdgeCounter;
      UInt_t* fuTrigChanClockCounter;
      UInt_t* fuTrigInputEdgeCounter;
      UInt_t* fuTrigInputClockCounter;
      Long64_t fiAllPossibleTriggers;
      Long64_t fiAcceptedTriggers;
      Long64_t fliAllPossibleTriggersOverflows;
      Long64_t fliAcceptedTriggersOverflows;

      // HADAQ event timestamp
      Int_t fiHadaqLastEventTime;
      Int_t fiHadaqFirstEventTime;
      Int_t fiHadaqSpillStartTime;
      Int_t fiHadaqCoarseSpillStartTime;

      // CTS trigger timestamp
      Long64_t fiCtsLastEventTime;
      Long64_t fiCtsFirstEventTime;
      Bool_t fbNextSpillToStart;
      Bool_t fbHadaqBufferDelay;

      Bool_t fbCtsAvailable;
      Bool_t fbGoodEventInBuffer;

      std::vector<UShort_t> fuSubeventSizes;

      ClassDef(TTrbUnpackTof,1)
};

#endif
