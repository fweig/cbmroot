// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                            CbmSourceLmdNew                        -----
// -----                    Created 10.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmSourceLmdNew.h"
#include <FairSource.h>         // for FairSource
#include <TList.h>              // for TList
#include <TObjString.h>         // for TObjString
#include <TObject.h>            // for TObject
#include <TString.h>            // for TString, operator<<
#include <stddef.h>             // for NULL
#include <iomanip>              // for setw, setprecision, __iom_t6, __iom_t5
#include <ios>                  // for right, fixed
#include "CbmAuxDigi.h"         // for CbmAuxDigi
#include "CbmDaqMap.h"          // for CbmDaqMap
#include "CbmModuleList.h"      // for CbmModuleList
#include "CbmROCUnpack.h"       // for CbmROCUnpack
#include "CbmTbDaqBuffer.h"     // for CbmTbDaqBuffer
#include "CbmTbEvent.h"         // for CbmTbEvent
#include "CbmTbEventBuilder.h"  // for CbmTbEventBuilder
#include <FairEventHeader.h>    // for FairEventHeader
#include <FairLogger.h>         // for FairLogger
#include <FairRunOnline.h>      // for FairRunOnline
#include "roc/Iterator.h"       // for Iterator
#include "roc/Message.h"        // for Message, MSG_AUX, MSG_EPOCH
#include <boost/any.hpp>

using std::map;
using std::setprecision;
using std::setw;
using std::fixed;
using std::right;

CbmSourceLmdNew::CbmSourceLmdNew()
  : FairSource(),
    fInputFileName(""),
    fInputFileList(),
    fFileCounter(0),
    fReadInTimeStep(1e9),
    fPersistence(kTRUE),
    fAuxOffset(1000),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fDaqMap(NULL),
    fRocIter(NULL),
    fEventBuilder(NULL),
    fTimeStart(0),
    fTimeStop(0),
    fCurrentEpochTime(0),
    fStartEpoch(0),
    fCurrentEpoch(),
    fCurrentBufferTime(0),
    fCurrentMessage(NULL),
    fTimeBufferFill(0),
    fTimeBufferOut(0.),
    fCurrentDigi(),
    fNofMessages(0),
    fNofEpochs(0),
    fNofEvents(0),
    fNofMessType(),
    fNofMessRoc(),
    fNofHitMsg(),
    fNofDigis(),
    fNofBLDigis(),
    fNofDiscardedDigis(),
    fNofAux(0),
    fNofDiscardedAux(0),
    fNofIgnoredMessages(0),
    fBaselineDataFill(kFALSE),
    fBaselineDataRetrieve(kFALSE),
    fBaselineRoc(),
  fTriggeredMode(kFALSE),
  fUnpackers(),
  fNofBaselineDigis(0)
{
  // --- Initialise counters
  for (Int_t iType = 0; iType < 8; iType++) { fNofMessType[iType] = 0; }
  for (Int_t iRoc = 0; iRoc < 20; iRoc++)
    for (Int_t iNx = 0; iNx < 5; iNx++ ) {
      fNofMessRoc[iRoc][iNx] = 0;
    }
  for (ECbmModuleId idet = ECbmModuleId::kRef; idet < ECbmModuleId::kNofSystems; ++idet) {
     fNofHitMsg[idet] = 0;
  }
}

CbmSourceLmdNew::CbmSourceLmdNew(const char* inFile)
  : FairSource(),
    fInputFileName(inFile),
    fInputFileList(new TObjString(inFile)),
    fFileCounter(0),
    fReadInTimeStep(1e9),
    fPersistence(kTRUE),
    fAuxOffset(1000),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fDaqMap(NULL),
    fRocIter(NULL),
    fEventBuilder(NULL),
    fTimeStart(0),
    fTimeStop(0),
    fCurrentEpochTime(0),
    fStartEpoch(0),
    fCurrentEpoch(),
    fCurrentBufferTime(0),
    fCurrentMessage(NULL),
    fTimeBufferFill(0),
  fTimeBufferOut(0.),
  fCurrentDigi(),
    fNofMessages(0),
    fNofEpochs(0),
    fNofEvents(0),
    fNofMessType(),
    fNofMessRoc(),
    fNofHitMsg(),
    fNofDigis(),
    fNofBLDigis(),
    fNofDiscardedDigis(),
    fNofAux(0),
    fNofDiscardedAux(0),
    fNofIgnoredMessages(0),
    fBaselineDataFill(kFALSE),
    fBaselineDataRetrieve(kFALSE),
    fBaselineRoc(),
  fTriggeredMode(kFALSE),
  fUnpackers(),
  fNofBaselineDigis(0)
{
  // --- Initialise counters
  for (Int_t iType = 0; iType < 8; iType++) { fNofMessType[iType] = 0; }
  for (Int_t iRoc = 0; iRoc < 20; iRoc++)
    for (Int_t iNx = 0; iNx < 5; iNx++ ) {
      fNofMessRoc[iRoc][iNx] = 0;
    }
  for (ECbmModuleId idet = ECbmModuleId::kRef; idet < ECbmModuleId::kNofSystems; ++idet) {
     fNofHitMsg[idet] = 0;
  }
}

/*
 * PAL: Comment copy constructor as does not seem to be used in any macro
CbmSourceLmdNew::CbmSourceLmdNew(const CbmSourceLmdNew& source)
  : FairSource(source)
{
}
*/

CbmSourceLmdNew::~CbmSourceLmdNew()
{
  //  delete fDaqMap;
}


// -----   Fill the buffer up to a given time   -----------------------------
Bool_t CbmSourceLmdNew::FillBuffer(ULong_t time)
{

  // --- Signal end of file if there are no more messages
  if ( ! fCurrentMessage ) {
    LOG(info) << GetName() << ": End of input file reached.";
    return kFALSE;
  }

  Int_t nMessages = 0;
  while ( fCurrentMessage ) {
    nMessages++;
    
    // Extract needed info from the message
    Int_t msgType = fCurrentMessage->getMessageType();
    Int_t rocId = fCurrentMessage->getRocNumber();
    Int_t nxyId = fCurrentMessage->getNxNumber();
    ULong_t hitTime = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
    
    // --- Jump out of loop of hit time is after time limit
    if (hitTime > time) { break; }

    // Ignore all messages with unknown system ID
    ECbmModuleId systemId = fDaqMap->GetSystemId(rocId);
    if ( systemId == ECbmModuleId::kNotExist ) {
    	fNofIgnoredMessages++;
    	LOG(debug) << "ROC id " << rocId << " system ID " << systemId
    			       << ": ignoring message";
    	GetNextMessage();
    	continue;
    }

    // Count number of messages for the different types
    fNofMessType[msgType]++;
    
    // To define a unique number needed for calling the correct
    // unpacker calculate such a number from the message type (which
    // is 1 for hit messages and the unique system id)
    if ( fCurrentMessage->isHitMsg() ) {
      // --- ROC number defines the system
      fNofHitMsg[systemId]++;
      fNofMessRoc[rocId][nxyId]++;
      if ( systemId >= ECbmModuleId::kRef ) msgType = msgType*10 + ToIntegralType(systemId);
    }
    
    // call the registered unpacker for the 
    // message type of the current message
    std::map<Int_t, CbmROCUnpack*>::iterator it=fUnpackers.find(msgType);
    if (it == fUnpackers.end()) {
      LOG(error) << "Skipping message with unknown type " 
		 << msgType << " ROC " << rocId;
      continue;
    } else {
      it->second->DoUnpack(fCurrentMessage, hitTime);
    }
    
    GetNextMessage();

  } //- message loop
  
  // --- Update buffer fill time
  fTimeBufferFill  = time;
  
  // --- Set buffer retrieval time to one time step before fill time.
  fTimeBufferOut  = Double_t(time - fReadInTimeStep);
  
  // --- End of input: retrieval time equals fill time
  if ( ! fCurrentMessage ) {
    LOG(info) << GetName() << ": End of input reached.";
    fTimeBufferOut = Double_t(time);
  }
  
  // --- Status info
  LOG(info) << GetName() << ": Buffer fill time " << fixed
            << setprecision(3) << Double_t(fTimeBufferFill) * 1.e-9
            << " s, retrieval up to " << fTimeBufferOut * 1.e-9
            << " s, " << nMessages << " messages processed";
  fBuffer->PrintStatus();
  fNofMessages += nMessages;
  
  return kTRUE;
}
// --------------------------------------------------------------------------



// -----   Get next data   --------------------------------------------------
CbmTbDaqBuffer::Data CbmSourceLmdNew::GetNextData()
{

  // --- Retrieve next data from the buffer.
  CbmTbDaqBuffer::Data digi = fBuffer->GetNextData(fTimeBufferOut);

  // --- If no data: fill the buffer with next time step
  // --- N.b.: fCurrentMessage = NULL means no more data in input and
  // ---       retrieval of buffer up to the fill time.
  while (  digi.first.empty()  && fCurrentMessage ) {
    FillBuffer(fTimeBufferFill + fReadInTimeStep);
    digi = fBuffer->GetNextData(fTimeBufferOut);
  }

  // --- If the digi pointer is NULL; the input is exhausted and the buffer
  // --- is empty.
  return digi;
}
// --------------------------------------------------------------------------


Bool_t CbmSourceLmdNew::GetNextMessage() {

  if ( fRocIter->next() ) fCurrentMessage = &fRocIter->msg();

  else  { // No new message

    // Check if there is another file in the list of files
    // and open this file if one exits
    fFileCounter += 1;
    if ( fInputFileList.GetSize() > fFileCounter ) {
    	Bool_t success = OpenInputFileAndGetFirstMessage();
      if (!success) {
      	fCurrentMessage = NULL;
      	return kFALSE;
      }
    }
    else fCurrentMessage = NULL;

  } //? No new message
  return kTRUE;



}
// --------------------------------------------------------------------------



// -----   Initialisation   -------------------------------------------------
Bool_t CbmSourceLmdNew::Init()
{

  LOG(info);
  LOG(info) << "=====================================================";
  LOG(info) << GetName() << ": Initialising ...";

  if ( NULL == fEventBuilder ) {
    LOG(fatal) << "No event builder defined.";
  }

  fEventBuilder->Init();

  if ( NULL == fDaqMap ) {
    LOG(fatal) << "No DaqMap defined.";
  }

  // Call the init of all registered unpackers
  for (std::map<Int_t, CbmROCUnpack*>::iterator it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    LOG(info) << "Initialize " << it->second->GetName() << 
      " for message type " << it->first;
    it->second->Init();
    it->second->SetPersistence(fPersistence);
  }

  Bool_t success=OpenInputFileAndGetFirstMessage();
  if (!success) return kFALSE;

  if ( fCurrentMessage->getMessageType() != roc::MSG_EPOCH ) {
    LOG(error) << GetName() << ": First message is not epoch marker!";
    return kFALSE;
  }

  // --- Get start time and set initial buffer fill time
  Int_t epoch  = fCurrentMessage->getEpochNumber();
  ULong_t time = fCurrentMessage->getMsgFullTime(epoch);
  LOG(info) << GetName() << ": First message: epoch " << epoch << "  time "
            << setprecision(9) << Double_t(time) * 1.e-9 << " s";

  // --- Set initial buffer fill time (rounded to next read-in time step)
  fTimeBufferFill = ULong_t( Double_t(time) / Double_t(fReadInTimeStep) )
                    * fReadInTimeStep;
  fTimeBufferOut  = fTimeBufferFill - fReadInTimeStep;

  // --- Read the first digi from the buffer
  fCurrentDigi = GetNextData();
  if ( fCurrentDigi.first.empty() ) {
    LOG(error) << GetName() << ": No hit data in input!";
    return kFALSE;
  }
  
  LOG(info) << GetName() << ": Initialisation done. ";
  LOG(info) << "=====================================================";
  LOG(info);

  return kTRUE;
}
// --------------------------------------------------------------------------



// -----   Event loop   -----------------------------------------------------
// Action: Digis are retrieved from the buffer one by one.
// If their time is within the association window, they are added to the
// event. If not, the event loop is stopped.
Int_t CbmSourceLmdNew::ReadEvent(UInt_t)
{
  
  // The TClonesArrays and everything else is cleared when FairRunOnline
  // calls the Reset function right before calling ReadEvent
  
  // Loop over digis
  while ( kTRUE ) {
    
    // here one should call a more generic function from a separate
    // class which defines if a digi belonges to the event or not

    if ( fEventBuilder->IsInEvent(fCurrentDigi.first) ) {

      // --- Copy digi to output array
      ECbmModuleId systemId = fCurrentDigi.second; //->GetSystemId();
      Int_t msgType = -1;
      if ( static_cast<ECbmModuleId>(999) == systemId ) { // hardcoded number
	Int_t val = boost::any_cast<CbmAuxDigi*>(fCurrentDigi.first)->GetRocId();
	if ( val == 666 ) {
	  CbmTbEvent* CurrentEvent = 
	    dynamic_cast<CbmTbEvent*> (FairRunOnline::Instance()->GetEventHeader());

	  LOG(info) << "Event type is now: " 
		    << CurrentEvent->GetEventType();
	  CurrentEvent->SetEventType(1);
	  LOG(info) << "Event type is now: " 
		    << CurrentEvent->GetEventType();
	  fBaselineDataRetrieve = kTRUE; 
	  LOG(info) << "In Old Aux RocId: "<< val;
	  //	  FillBaselineDataContainer();
	  fNofEvents++;
          fNofBaselineDigis = fNofDigis[ToIntegralType(ECbmModuleId::kDummyDet)];
          // remove special aux digi which should not end up in output
	  delete boost::any_cast<CbmAuxDigi*>(fCurrentDigi);
	  fCurrentDigi = GetNextData();
	  return 0;
	} else if ( val  == 999 ) {
	  fBaselineDataRetrieve = kFALSE; 
	  LOG(info) << "Aux RocId: "<< val;
	  LOG(info) << "Filling " 
		    << ( fNofDigis[ToIntegralType(ECbmModuleId::kDummyDet)] - fNofBaselineDigis )
		    << " for this baseline event";
          // remove special aux digi which should not end up in output
	  delete boost::any_cast<CbmAuxDigi*>(fCurrentDigi);
	  fCurrentDigi = GetNextData();
	  return 0;
	} else {
	  msgType = 4; // This is an Aux digi
	  fNofAux++;
	}
      } else {
	msgType = ToIntegralType(systemId) + 10; // to get the correct unpacker
      }
      
      std::map<Int_t, CbmROCUnpack*>::iterator it=fUnpackers.find(msgType);
      if (it == fUnpackers.end()) {
	LOG(error) << "Skipping digi with unknown id " 
		   << msgType;
	continue;
      } else {
	it->second->FillOutput(fCurrentDigi);
        if ( kTRUE == fBaselineDataRetrieve ) {
	  fNofDigis[ToIntegralType(ECbmModuleId::kDummyDet)]++;
	  fNofBLDigis[ToIntegralType(systemId)]++;
	} else {
	  fNofDigis[ToIntegralType(systemId)]++;
	}
      }
  
      
      // --- Delete current digi from memory and get next one
      // TODO: Chexk if this is a memory leak
      //      delete fCurrentDigi;
      fCurrentDigi = GetNextData();
      if ( fCurrentDigi.first.empty() ) {
	LOG(info) << "No more input data. End the run.";
	return 1;  // no more data; trigger end of run
      }
    }   //? Digi belongs to event?
    
    // --- If digi does not belong to current event: stop event loop.
    else {
      if ( FairLogger::GetLogger()->IsLogNeeded(fair::Severity::debug1) ) {
	fEventBuilder->PrintCurrentEvent();
      }
      fNofEvents++;
      return 0;
    }

  } //- event loop over digis

  LOG(info) << "===== Leaving Exec. ";

  return 0;
}
// --------------------------------------------------------------------------

void CbmSourceLmdNew::Close()
{

  // Call the finish function for all registered unpackers
  // write histos, etc.
  for (std::map<Int_t, CbmROCUnpack*>::iterator it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    it->second->Finish();
  }

  LOG(info);
  LOG(info) << "=====================================================";
  fBuffer->PrintStatus();
  LOG(info) << GetName() << ": Run Summary";

  LOG(info);
  LOG(info) << "Messages in input: " << fNofMessages;
  Double_t deltaT = (fTimeStop - fTimeStart) / 1.e9;
  LOG(info) << "Start time : " << fTimeStart * 1.e-9 << " s  ";
  LOG(info) << "Stop  time : " << fTimeStop  * 1.e-9 << " s  ";
  LOG(info) << "Duration   : " << deltaT  << " s";
  LOG(info) << "Ignored messages: " << fNofIgnoredMessages;


  const char* names[8] = { "NOP", "HIT", "EPOCH", "SYNC", "AUX", "EPOCH2", "GET4", "SYS" };
  LOG(info);
  LOG(info) << "Messages per message type: ";
  for (Int_t i=0; i<8; i++)
    LOG(info) << setw(6) << names[i] << "  "
              << setw(10) << right << fNofMessType[i];

  LOG(info);
  LOG(info) << "Hit messages per ROC/NXYTER: ";
  for (Int_t iRoc = 0; iRoc < 13; iRoc ++)
    for (Int_t iNx = 0; iNx < 3; iNx +=2 ) {
      LOG(info) << " ROC " << setw(2) << iRoc << "  NXYTER " << iNx << "  Messages "
                << setw(12) << right << fNofMessRoc[iRoc][iNx] << "  Rate "
                << setw(12) << setprecision(4) << fixed  << right
                << Double_t(fNofMessRoc[iRoc][iNx]) / deltaT
                << " /s ";
    }

  LOG(info);
  LOG(info) << "Hit messages and digis per system: ";
  for (ECbmModuleId iSys = ECbmModuleId::kRef; iSys < ECbmModuleId::kNofSystems; ++iSys) {
    TString sysName = CbmModuleList::GetModuleNameCaps(iSys);
    LOG(info) << setw(5) << sysName << ": Messages " << fNofHitMsg[iSys]
              << ", Digis " << fNofDigis[ToIntegralType(iSys)] 
	      << ", BaselineDigis " << fNofBLDigis[ToIntegralType(iSys)] 
	      << ", discarded Digis " << fNofDiscardedDigis[ToIntegralType(iSys)];
  }
  LOG(info) << "AUX  : Messages " << fNofMessType[roc::MSG_AUX] << ", Digis "
            << fNofAux << ", discarded Digis " << fNofDiscardedAux;


  LOG(info) << "Total number of events: " << fNofEvents;

  delete fRocIter;
}

void CbmSourceLmdNew::Reset()
{
  // Call the init of all registered unpackers
  for (std::map<Int_t, CbmROCUnpack*>::iterator it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    it->second->Reset();
  }
  
  fEventBuilder->Reset();
}


void CbmSourceLmdNew::AddBaselineRoc(Int_t rocNr) {
  // fBaselineData is set to kTRUE after the first call of this function
  // so the two rocs which are triggered should be set to basline mode
  if ( fTriggeredMode && !fBaselineDataFill ) {
    fBaselineRoc.insert(11);
    fBaselineRoc.insert(12);
    LOG(info) << "Triggered mode";
    LOG(info) << "ROC 11" << " now in baseline mode";
    LOG(info) << "ROC 12" << " now in baseline mode";
  }
  fBaselineRoc.insert(rocNr);
}

Bool_t CbmSourceLmdNew::RemoveBaselineRoc(Int_t rocNr)
{
  // Triggered mode and triggered rocs still in set
  if (fTriggeredMode && fBaselineRoc.find(11) != fBaselineRoc.end()) {
    fBaselineRoc.erase(11);
    fBaselineRoc.erase(12);
    LOG(info) << "Triggered mode";
    LOG(info) << "ROC 11" << " now in normal mode";
    LOG(info) << "ROC 12" << " now in normal mode";
  }
  fBaselineRoc.erase(rocNr);
  Bool_t empty = fBaselineRoc.empty();  
  return empty;  
}

Bool_t CbmSourceLmdNew::IsBaselineFill(Int_t rocNr)
{
  std::set<Int_t>::iterator it;
  if ( fBaselineRoc.find(rocNr) == fBaselineRoc.end() ) {
    return kFALSE;
  } else {
    return kTRUE;
  }
}

Bool_t CbmSourceLmdNew::OpenInputFileAndGetFirstMessage()
{
  TObjString* tmp =
    dynamic_cast<TObjString*>(fInputFileList.At(fFileCounter));
  fInputFileName = tmp->GetString();
  LOG(info) << GetName() << ": Opening file "
	    << fInputFileName;
  // Normally one should delete the old object before, but this
  // doesn't work and result in a crash when creating a new iterator
  fRocIter = new roc::Iterator(fInputFileName.Data());
  if ( ! fRocIter->next() ) {
    LOG(error) << GetName() << "::Init: input file does not exist "
	       << "or is empty!";
    return kFALSE;
  }
  fCurrentMessage = &fRocIter->msg();

  return kTRUE;
}  


ClassImp(CbmSourceLmdNew)
