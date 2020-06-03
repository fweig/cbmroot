#ifndef CBMRICHTRBUNPACK2_H
#define CBMRICHTRBUNPACK2_H

#include <TString.h>
#include "FairSource.h"

#include "CbmHistManager.h"
#include "CbmDrawHist.h"
#include "CbmTrbRawMessage.h"

#include <list>
#include <map>
#include <set>
#include <vector>

class CbmRawSubEvent;
class TClonesArray;
class CbmTrbIterator;

struct eventDesc
{
        eventDesc(): begin(), end(), firstTimestamp(-111.), ready(kFALSE) {}	
	std::multimap<Double_t, CbmTrbRawMessage*>::iterator begin;
	std::multimap<Double_t, CbmTrbRawMessage*>::iterator end;
	Double_t firstTimestamp;
	Bool_t ready;
};

class CbmRichTrbUnpack2 : public FairSource
{
public: // methods

   CbmRichTrbUnpack2();
   CbmRichTrbUnpack2(TString hldFileName);
   virtual ~CbmRichTrbUnpack2();

   Bool_t Init();
   Int_t ReadEvent(UInt_t);

   Source_Type GetSourceType() {return kONLINE;}

    virtual void SetParUnpackers() {;}

    virtual Bool_t InitUnpackers() {return kTRUE;}

    virtual Bool_t ReInitUnpackers() {return kTRUE;}
    
   void Close();
   void Reset();

   // Add filename to the list of input files to be processed during the run
   void AddInputFile(TString filename) { fHldFiles.push_back(filename); }

   void SetOutHistoFile(TString filename) {fOutHistoFile = filename;}

private: // methods

   // Try to read the next input file. Return 0 in case of success, otherwize 1.
   UInt_t ReadNextInputFileToMemory();

   Int_t ReadOneRawEvent();
   void PushEvent(std::multimap<Double_t, CbmTrbRawMessage*>::iterator firstEdge,
                  std::multimap<Double_t, CbmTrbRawMessage*>::iterator lastEdge);

   void ProcessSubEvent(CbmRawSubEvent* subEvent);
   void ProcessSubSubEvent(UInt_t* data, UInt_t size, UInt_t subEventId);
   void ProcessSkip(UInt_t* data, UInt_t size, UInt_t subEventId);

   Double_t GetFullTime(UInt_t tdcId, UInt_t channel, UInt_t epoch, UInt_t coarse, UInt_t fine);

   // Overloaded method
   Double_t GetFullTime(CbmTrbRawMessage rawMess);

   //TODO implement or get rid
   CbmRichTrbUnpack2(const CbmRichTrbUnpack2&);
   CbmRichTrbUnpack2 operator=(const CbmRichTrbUnpack2&);

private: // data members

   TString fOutHistoFile;

   // List of input HLD files to be processed
   std::list<TString> fHldFiles;

   // Counter of processed raw events
   UInt_t fNofRawEvents;

   // Limiter on the number of raw events to be processed
   UInt_t fMaxNofRawEvents;

   // Counter of timestamps being unpacked
   UInt_t fTimestampsUnpacked;

   // Counter of erroneous timestamps
   UInt_t fTimestampsNotUnpacked;

   // Counter of sync timestamps unpacked
   UInt_t fSyncTimestampsUnpacked;

   // Counter of skipped timestamps (not sent to further analysis)
   UInt_t fSkippedTimestamps;

   // Counter of pushed timestamps
   UInt_t fPushedTimestamps;

   // Pointer and the size of the data in the memory (read from file) which is being processed
   Char_t* fDataPointer;
   UInt_t fDataSize;

   // Iterator over the CbmNet messages - ?
   CbmTrbIterator* fTrbIter;

   // Number of pushed events
   UInt_t fNumPushed;

   //Between-TDC synchronization
   Double_t fLastSyncTime;
   Double_t fMainSyncTime;
   Double_t fCorr;
   Bool_t fMainSyncDefined;

   CbmHistManager* fHM;

   std::multimap<Double_t, CbmTrbRawMessage*> fMessageBuffer;
   std::vector<eventDesc> fEventStack;

   Bool_t inGarbCollMode;
   Bool_t inFinalCleanup;
   UInt_t fCurNeventsInBuf;

   // Output raw hits
   TClonesArray* fTrbRawHits;

   ClassDef(CbmRichTrbUnpack2,1)
};

#endif // CBMRICHTRBUNPACK2_H
