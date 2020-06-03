#include "CbmRichTrbUnpack2.h"

//TODO check which headers are really needed

#include <iostream>
#include <map>

#include <TClonesArray.h>
#include <TFile.h>

#include "FairLogger.h"
#include "FairRootManager.h"

#include "CbmTrbIterator.h"
#include "CbmTrbCalibrator.h"
#include "CbmRichTrbParam.h"
#include "CbmRawEvent.h"

// Uncomment if you want to have excessive printout (do not execute on many events, may produce Gb's of output)
//#define DEBUGPRINT

#define EVENTBUFMIN 200
#define EVENTBUFLIMIT 500

CbmRichTrbUnpack2::CbmRichTrbUnpack2() :
      FairSource(),
      fOutHistoFile(),
      fHldFiles(),
      fNofRawEvents(0),
      fMaxNofRawEvents(2000000000),
      fTimestampsUnpacked(0),
      fTimestampsNotUnpacked(0),
      fSyncTimestampsUnpacked(0),
      fSkippedTimestamps(0),
      fPushedTimestamps(0),
      fDataPointer(NULL),
      fDataSize(0),
      fTrbIter(NULL),
      fNumPushed(0),
      fLastSyncTime(0.),
      fMainSyncTime(0.),
      fCorr(0.),
      fMainSyncDefined(kFALSE),
      fHM(NULL),
      fMessageBuffer(),
      fEventStack(),
      inGarbCollMode(kFALSE),
      inFinalCleanup(kFALSE),
      fCurNeventsInBuf(),
      fTrbRawHits(new TClonesArray("CbmTrbRawMessage", 10))
{
}

CbmRichTrbUnpack2::CbmRichTrbUnpack2(TString hldFileName) :
      FairSource(),
      fOutHistoFile(),
      fHldFiles(),
      fNofRawEvents(0),
      fMaxNofRawEvents(2000000000),
      fTimestampsUnpacked(0),
      fTimestampsNotUnpacked(0),
      fSyncTimestampsUnpacked(0),
      fSkippedTimestamps(0),
      fPushedTimestamps(0),
      fDataPointer(NULL),
      fDataSize(0),
      fTrbIter(NULL),
      fNumPushed(0),
      fLastSyncTime(0.),
      fMainSyncTime(0.),
      fCorr(0.),
      fMainSyncDefined(kFALSE),
      fHM(NULL),
      fMessageBuffer(),
      fEventStack(),
      inGarbCollMode(kFALSE),
      inFinalCleanup(kFALSE),
      fCurNeventsInBuf(),
      fTrbRawHits(new TClonesArray("CbmTrbRawMessage", 10))
{
   fHldFiles.push_back(hldFileName);
}

CbmRichTrbUnpack2::~CbmRichTrbUnpack2()
{
}

Bool_t CbmRichTrbUnpack2::Init()
{
	LOG(info) << "CbmRichTrbUnpack::Init()";

	FairRootManager* ioman = FairRootManager::Instance();
	if (ioman == NULL) {
		LOG(fatal) << "No FairRootManager instance";
	}
	ioman->Register("CbmTrbRawMessage", "TRB raw messages", fTrbRawHits, kTRUE);

	fHM = new CbmHistManager();

   fHM->Create1<TH1D>("fMessagesInTDC", "Total number of messages in TDC", 64, 0., 64.);
   fHM->Create1<TH1D>("fMessagesInChannel", "Total number of messages in channel", 2118, 0., 2118.); // should be 2112
   fHM->Create2<TH2D>("fHeatMapLeadingEdges", "Heat map built by leading edges", 32, 0., 32., 32, 0., 32.);

	// Read the first file
	ReadNextInputFileToMemory();

	return kTRUE;
}

void CbmRichTrbUnpack2::Close()
{
	/*
	if (!fMessageBuffer.empty()) {
		printf ("Leftovers in the buffer:\n");
		for (auto iter : fMessageBuffer)
		{
			printf ("%f\n", this->GetFullTime(*iter.second));
		}
	}
	*/

	LOG(info) << "[CbmRichTrbUnpack2] left in the event building buffer: " << fMessageBuffer.size();

	LOG(info) << "[CbmRichTrbUnpack2] Total " << fTimestampsUnpacked-fTimestampsNotUnpacked-fSyncTimestampsUnpacked-fSkippedTimestamps
				<< " timestamps unpacked not incl. " << fSyncTimestampsUnpacked
				<< " sync messages and not incl. " << fSkippedTimestamps
				<< " skipped (noise) messages and not incl. " << fTimestampsNotUnpacked
				<< " with fine time counter error.";
	LOG(info) << "[CbmRichTrbUnpack2] Total " << fPushedTimestamps << " pushed messages.";

	TFile* curHistoFile = new TFile(fOutHistoFile, "UPDATE");
	LOG(info) << "Opening file " << fOutHistoFile << " for histograms";
	fHM->WriteToFile();
	curHistoFile->Close();

}

void CbmRichTrbUnpack2::Reset()
{
	fTrbRawHits->Clear();
}

UInt_t CbmRichTrbUnpack2::ReadNextInputFileToMemory()
{
	if (fHldFiles.size() > 0) {
		TString hldFileName = fHldFiles.front();
		fHldFiles.pop_front();

		std::streampos size;
		std::ifstream file (hldFileName.Data(), std::ios::in | std::ios::binary | std::ios::ate);
		if (file.is_open()) {
			size = file.tellg();

			if (fDataPointer)
				delete [] fDataPointer;
			fDataPointer = new Char_t[size];

			file.seekg (0, std::ios::beg);
			file.read (fDataPointer, size);
			file.close();
			LOG(info) << "The entire file " << hldFileName << " content is in memory (" << size / (1024 * 1024) << " MB)";
		} else {
			LOG(fatal) << "Unable to open file " << hldFileName;
		}
		fDataSize = (UInt_t) size;

		if (fTrbIter)
			delete fTrbIter;
		fTrbIter = new CbmTrbIterator((void*) fDataPointer, fDataSize);

		return 0; // There is data available
	} else {
		if (fDataPointer)
			delete [] fDataPointer;
		if (fTrbIter)
			delete fTrbIter;
		return 1; // no more data
	}
}

// Read one raw 'event' into the buffer
Int_t CbmRichTrbUnpack2::ReadOneRawEvent()
{
	// Try to extract next event from the Iterator. If no events left - go out of the loop

	CbmRawEvent* rawEvent = fTrbIter->NextEvent();
	if (rawEvent == NULL) {  // no more data
		if (this->ReadNextInputFileToMemory() == 1) {
			return 1;
		} else {
			rawEvent = fTrbIter->NextEvent();
		}
	}

//   if (rawEvent->Size() > 8000) continue; // TODO uncomment?

	// End run if exceed the limit of total number of raw hits
	if (fNofRawEvents >= fMaxNofRawEvents)
		return 1; // no more data

	if (fNofRawEvents % 1000 == 0)
		LOG(info) << "Raw event # " << fNofRawEvents/1000 << "k" << "---------------------------------------------------";

#ifdef DEBUGPRINT
	//rawEvent->PrintTime();
	rawEvent->Print();
#endif

	// Loop over subevents
	while (true) {
		CbmRawSubEvent* rawSubEvent = fTrbIter->NextSubEvent();
		if (rawSubEvent == NULL)
			break;
#ifdef DEBUGPRINT
   	rawSubEvent->PrintHeaderOnly();  // PrintHeaderOnly() or Print()
#endif
		ProcessSubEvent(rawSubEvent);

	}

	fNofRawEvents++;

	return 0; // still some data
}

// Actually not _read_ event but produce (build) one event from the buffer of raw events
Int_t CbmRichTrbUnpack2::ReadEvent(UInt_t)
{
	// This shit is quite complicated...
	// So, there are two modes - standard and "garbage collation"
	// In standard mode we fill the buffer with the edges until
	// there are at least EVENTBUFMIN recognisable events.
	// Then we flush first event to the output.
	// Next time when CbmRoot calls this method we will already
	// have EVENTBUFMIN-1 events in the buffer. It will be again
	// filled up. Then at some moment the number of event in the buffer
	// may get quite high (EVENTBUFLIMIT) - we switch to "garbage
	// collection" mode. In this mode we don't read any more data but
	// simply flush event by event from the front of the buffer until
	// the number of event in the buffer is again EVENTBUFMIN.
	// Then, of cause, switch back to standard mode.
	// Additional to that at some moment the source of edges
	// may get empty. In this case we flush everything.

	Int_t goOn;

	std::multimap<Double_t, CbmTrbRawMessage*>::iterator iter;
	std::multimap<Double_t, CbmTrbRawMessage*>::iterator curFirst;
	Double_t winLeft, winRight;

	// Standard mode
	if (!inGarbCollMode)
	{

		UInt_t eventInd = 0;

		// Fill the buffer
		do {
			// Per-se read the "DAQ-event"
			goOn = this->ReadOneRawEvent();

			if (!fMessageBuffer.empty()) {
				eventInd = 0;

				// Start the first event with the first edge in the buffer
				curFirst = fMessageBuffer.begin();
				winLeft = this->GetFullTime(*curFirst->second);
				winRight = winLeft + EVENTTIMEWINDOW;

				// Loop over the rest of the edges
				for (iter = fMessageBuffer.begin(); iter != fMessageBuffer.end(); ++iter) {

					// Switch to next event.
					// Here we count the number of events in the buffer
					// The problem is that reading next "DAQ-event"
					// not necessarily adds real event - it may add some edges
					// to any existing event - still in the buffer (or already flushed).
					if (this->GetFullTime(*iter->second) > winRight) {
						curFirst = iter;
						winLeft = this->GetFullTime(*curFirst->second);
						winRight = winLeft + EVENTTIMEWINDOW;
						eventInd++;
					}

					//printf ("%d: tdc 0x%04x ch %d\t%f\n", eventInd, iter->second->GetSourceAddress(), iter->second->GetChannelID(), this->GetFullTime(*iter->second));

				}
			}
		} while (goOn != 1 && eventInd < EVENTBUFMIN-1);

		//printf ("eventInd=%d\n", eventInd);

		// There are no more edges in the source - go to the third mode - cleanup
		if (goOn == 1) {
			inGarbCollMode = kTRUE;
			fCurNeventsInBuf = eventInd+1;
			inFinalCleanup = kTRUE;
			printf ("inFinalCleanup = kTRUE\n");
			return 0;
		} else {
			curFirst = fMessageBuffer.begin();
			winLeft = this->GetFullTime(*curFirst->second);
			winRight = winLeft + EVENTTIMEWINDOW;
			for (iter = fMessageBuffer.begin(); iter != fMessageBuffer.end(); ++iter) {

				// switch to next event
				if (this->GetFullTime(*iter->second) > winRight) {
					this->PushEvent(fMessageBuffer.begin(), iter);
					break;
				}

			}

			// Switch to garbage collection mode
			if (eventInd > EVENTBUFLIMIT || goOn == 1) {
				inGarbCollMode = kTRUE;
				fCurNeventsInBuf = eventInd+1;
				return 0;
			}
		}

	} else {
		// "Garbage collection" mode
		curFirst = fMessageBuffer.begin();
		winLeft = this->GetFullTime(*curFirst->second);
		winRight = winLeft + EVENTTIMEWINDOW;
		//printf ("Garbage collection\twinLeft=%f\tfMessageBuffer size: %d\n", winLeft, fMessageBuffer.size());
		for (iter = fMessageBuffer.begin(); iter != fMessageBuffer.end(); ++iter) {

			if (this->GetFullTime(*iter->second) > winRight) {
				this->PushEvent(fMessageBuffer.begin(), iter);
				//printf ("fCurNeventsInBuf-- = %d--\n", fCurNeventsInBuf);
				fCurNeventsInBuf--;
				if (!inFinalCleanup && fCurNeventsInBuf == EVENTBUFMIN)
				{
					inGarbCollMode = kFALSE;
					return 0;
				}
				//if ( inFinalCleanup && fCurNeventsInBuf == 0) return 1;
				return 0;

			}
		}

		// Very end comes here
		this->PushEvent(fMessageBuffer.begin(), fMessageBuffer.end());
		return 1;
	}

	return goOn; // 0 - still some data, 1 - no more events
}

void CbmRichTrbUnpack2::PushEvent(std::multimap<Double_t, CbmTrbRawMessage*>::iterator firstEdge,
									std::multimap<Double_t, CbmTrbRawMessage*>::iterator lastEdge)
{
	std::multimap<Double_t, CbmTrbRawMessage*>::iterator iter;
	Int_t ind;
	Int_t counter=0;
	for (iter = firstEdge; iter != lastEdge; iter++)
	{
		counter++;
	}

	// Here we skip single-hit events
	if (counter > 0) {
		for (iter = firstEdge; iter != lastEdge; /**/)
		{
#ifdef DEBUGPRINT
			//if (iter->second->GetSourceAddress() == 0x0110 ||
			//	iter->second->GetSourceAddress() == 0x0111)
			{
				printf ("Pushing: tdc 0x%04x ch %d\t%f\n", iter->second->GetSourceAddress(), iter->second->GetChannelID(), this->GetFullTime(*iter->second));
			}
#endif

			ind = fTrbRawHits->GetEntriesFast();
			new ((*fTrbRawHits)[ind]) CbmTrbRawMessage();
			static_cast<CbmTrbRawMessage*>(fTrbRawHits->At(ind))->CopyMessage(iter->second);

			fPushedTimestamps++;

			fMessageBuffer.erase(iter++);
		}

		fNumPushed++;
		//LOG(info) << "Pushed " << fNumPushed << " events";
	} else {
		for (iter = firstEdge; iter != lastEdge; /**/)
		{
#ifdef DEBUGPRINT
			printf ("Skipping(erasing): %f\n", this->GetFullTime(*iter->second));
#endif
			fSkippedTimestamps++;
			fMessageBuffer.erase(iter++);
		}
	}

/*
	LOG(info) << "[CbmRichTrbUnpack2] Total " << fTimestampsUnpacked-fTimestampsNotUnpacked << " timestamps unpacked (not incl. "
			<< fTimestampsNotUnpacked << " with fine time counter error).";
	LOG(info) << "[CbmRichTrbUnpack2] " << fMessageBuffer.size() << " edges in the buffer.";
*/
}

void CbmRichTrbUnpack2::ProcessSubEvent(CbmRawSubEvent* subEvent) {
	UInt_t tdcDataIndex = 0;
	UInt_t tdcData = 0x0;
	//	UInt_t subEventId = subEvent->SubId();

	fLastSyncTime = 0.;
	fCorr = 0.;
//	fMainSyncDefined = kFALSE;
//	fMainSyncTime = 0.;

	// Loop over subsubevents
	while (true) {
		tdcData = subEvent->SubDataValue(tdcDataIndex);
		UInt_t tdcNofWords = (tdcData >> 16) & 0xffff;
		UInt_t subSubEventId = tdcData & 0xffff;
#ifdef DEBUGPRINT
//	printf("0x%08x - SUBSUBEVENT HEADER    tdcNofWords = %i, ID = 0x%04x\n", tdcData, tdcNofWords, subSubEventId);
#endif

		if (subSubEventId == 0x7000 || subSubEventId == 0x7001 || subSubEventId == 0x7002 || subSubEventId == 0x7003) {
			tdcDataIndex++;
			continue;
		}

		// read TDC words to array
		UInt_t dataArray[tdcNofWords];
		for (UInt_t i = 0; i < tdcNofWords; i++) {
			tdcDataIndex++;
			tdcData = subEvent->SubDataValue(tdcDataIndex);
			dataArray[i] = tdcData;
		}

		if (subSubEventId == 0x5555) {
			ProcessSkip(dataArray, tdcNofWords, subSubEventId);
			break;
		}

		//FIXME temporarily skip messages from beam detector (tdc=0x0111)
//		if (subSubEventId != 0x7005 && subSubEventId != 0x0112/* && subSubEventId != 0x0111 && subSubEventId != 0x0110*/) {	// For beamtime data
		if (subSubEventId == 0x0010 || subSubEventId == 0x0011 || subSubEventId == 0x0012 || subSubEventId == 0x0013) {		// For Wuppertal Lab data
			ProcessSubSubEvent(dataArray, tdcNofWords, subSubEventId);
		} else {
			ProcessSkip(dataArray, tdcNofWords, subSubEventId);
		}

		tdcDataIndex++;
	}

}

void CbmRichTrbUnpack2::ProcessSkip(UInt_t* /*data*/, UInt_t /*size*/, UInt_t /*subEventId*/)
{
#ifdef DEBUGPRINT
//	for (UInt_t i=0; i<size; i++) { printf ("0x%08x - skip\n", data[i]); }
#endif
}

void CbmRichTrbUnpack2::ProcessSubSubEvent(UInt_t* data, UInt_t size, UInt_t subEventId) {
	CbmRichTrbParam* param = CbmRichTrbParam::Instance();

	UInt_t tdcId = subEventId;

	UInt_t curEpochCounter = 0;

	for (UInt_t i = 0; i < size; i++) {
		UInt_t tdcData = data[i];
		UInt_t tdcTimeDataMarker = (tdcData >> 31) & 0x1;

		if (tdcTimeDataMarker == 0x1)  // TIME DATA
		{
			fTimestampsUnpacked++;

			UInt_t channel = (tdcData >> 22) & 0x7f;
			UInt_t fine = (tdcData >> 12) & 0x3ff;
			UInt_t edge = (tdcData >> 11) & 0x1;
			UInt_t coarse = (tdcData) & 0x7ff;
			UInt_t epoch = curEpochCounter;

#ifdef DEBUGPRINT
//temporary condition
//if (!param->IsSyncChannel(channel)) {
			printf("0x%08x - TIMESTAMP", tdcData);
//}
#endif

			// Fix 0x3ff fine time counter value occurence - skip such a timestamp
			if (fine == 0x3ff) {
#ifdef DEBUGPRINT
				printf(" 0x3ff fine time counter value detected, skipping timestamp. tdc 0x00%x  ch %d\n", tdcId, channel);
#endif
				fTimestampsNotUnpacked++;
				return;
			}

			Double_t fullTime = GetFullTime(tdcId, channel, epoch, coarse, fine);

         if (tdcId != 0x0110 && tdcId != 0x0111 && tdcId != 0x0112 && tdcId != 0x0113 && tdcId != 0xc000) {
            if (channel != 0) {
               fHM->H1("fMessagesInTDC")->Fill(param->TDCidToInteger(tdcId));
               fHM->H1("fMessagesInChannel")->Fill(param->TDCandCHtoInteger(tdcId, channel));
/*
            if (param->TDCandCHtoInteger(tdcId, channel) == 2048) {
               printf ("tdc 0x%04x ch %d edge %d epoch %08x coarse %08x fine %08x\n", tdcId, channel, edge, epoch, coarse, fine);
            }
*/
               if (param->IsLeadingEdgeChannel(channel)) {
                  CbmRichHitInfo* hitInfo = param->GetRichHitInfo(tdcId, channel);
                  fHM->H2("fHeatMapLeadingEdges")->Fill(hitInfo->GetXPixel(), hitInfo->GetYPixel());
               }
            }
         }

			if (param->IsSyncChannel(channel)) {     // SYNC MESSAGE PROCESSING
				//CbmTrbCalibrator::Instance()->AddFineTime(tdcId, channel, fine);    //TODO check - needed or not?

				// Between-TDC synchronization
				fLastSyncTime = fullTime;

				// Here one can implement the case when the previous sync (10ms earlier) comes among the current messages.

				// 10 kHz - 100 000 ns, thres should be ~90 000
				// 5 kHz - 200 000 ns, thres should be ~150 000
				// 1.1 kHz - 909 090 ns, thres should be ~800 000
				if (fLastSyncTime-fMainSyncTime > 150000) {
					fMainSyncDefined = kFALSE;
				}

				if (!fMainSyncDefined) {
					fMainSyncDefined = kTRUE;
					fMainSyncTime = fullTime;
				}
				fCorr = fLastSyncTime-fMainSyncTime;
/*
				switch (tdcId) {
					case 0x0011: fHM->H1("fhCorrection_0x0011")->Fill(fCorr); break;
					case 0x0012: fHM->H1("fhCorrection_0x0012")->Fill(fCorr); break;
					case 0x0013: fHM->H1("fhCorrection_0x0013")->Fill(fCorr); break;
					default: break;
				}
*/
#ifdef DEBUGPRINT
				printf("   --- SYNC  - tdc 0x%04x ch %d edge %d epoch %08x coarse %08x fine %08x = %f\tcorrection=%f\n",
					tdcId, channel, edge, epoch, coarse, fine, fullTime/*+fCorr*/, fCorr);
#endif
				fSyncTimestampsUnpacked++;

			} else if (param->IsLeadingEdgeChannel(channel)) { // LEADING EDGE PROCESSING
#ifdef DEBUGPRINT
				printf("   --- LEAD  - tdc 0x%04x ch %d edge %d epoch %08x coarse %08x fine %08x = %f\tcorrection=%f\n",
					tdcId, channel, edge, epoch, coarse, fine, fullTime/*+fCorr*/, fCorr);
#endif
				CbmTrbCalibrator::Instance()->AddFineTime(tdcId, channel, fine);
			} else {                                 // TRAILING EDGE PROCESSING
#ifdef DEBUGPRINT
				printf("   --- TRAIL - tdc 0x%04x ch %d edge %d epoch %08x coarse %08x fine %08x = %f\tcorrection=%f\n",
					tdcId, channel, edge, epoch, coarse, fine, fullTime/*+fCorr*/, fCorr);
#endif
				CbmTrbCalibrator::Instance()->AddFineTime(tdcId, channel, fine);
			}
/******* obsolete
			// Important to keep it here - after fCorr computation
			new ((*fTrbRawHits)[fTrbRawHits->GetEntriesFast()]) CbmTrbRawMessage(
					0, tdcId, channel, epoch, coarse, fine, edge, fCorr);
*/
			//TODO development workaround
			//FIXME - here we skip sync messages so that they don't appear in event building
			//if (!param->IsSyncChannel(channel))
			{
				fMessageBuffer.insert( std::pair<Double_t, CbmTrbRawMessage*> (fullTime, new CbmTrbRawMessage(0, tdcId, channel, epoch, coarse, fine, edge, fCorr)));
				//fMessageBuffer.insert( new CbmTrbRawMessage(0, tdcId, channel, epoch, coarse, fine, edge, fCorr) );
			}

		} // if TIME DATA

		UInt_t tdcMarker = (tdcData >> 29) & 0x7; //3 bits
		if (tdcMarker == 0x1) { // TDC header
		  //	UInt_t randomCode = (tdcData >> 16) & 0xff; // 8bits
		  //	UInt_t errorBits = (tdcData) & 0xffff; //16 bits
#ifdef DEBUGPRINT
//			printf("0x%08x - TDC HEADER randomCode:0x%02x, errorBits:0x%04x\n", tdcData, randomCode, errorBits);
#endif
		} else if (tdcMarker == 0x2) { // DEBUG
		  //	UInt_t debugMode = (tdcData >> 24) & 0x1f; //5 bits
		  //	UInt_t debugBits = (tdcData) & 0xffffff; //24 bits
#ifdef DEBUGPRINT
//			printf("0x%08x - DEBUG debugMode:%i, debugBits:0x%06x\n", tdcData, debugMode, debugBits);
#endif
		} else if (tdcMarker == 0x3) { // EPOCH counter
			curEpochCounter = (tdcData) & 0xfffffff; //28 bits
#ifdef DEBUGPRINT
//			printf("0x%08x - EPOCH COUNTER epochCounter:0x%07x\n", tdcData, curEpochCounter);
#endif
		} else {
#ifdef DEBUGPRINT
			if (tdcTimeDataMarker != 0x1) printf("0x%08x - UNKNOWN\n", tdcData);
#endif
		}

	} // for loop

}

Double_t CbmRichTrbUnpack2::GetFullTime(UInt_t tdcId, UInt_t channel, UInt_t epoch, UInt_t coarse, UInt_t fine)
{
// This piece of code may be useful for debugging if at some moment calibrator class would start crashing.
// Otherwise keep it commented.
/*
   UShort_t tailSize = 30;
   Double_t fineTime = 0.;

   if (fine < tailSize) {    // Left tail - 0 ns
      fineTime = 0.;
   } else if (fine > 512-tailSize) {     // Right tail - 5 ns
      fineTime = 5.;
   } else {                   // Middle - linear function
      fineTime = (Double_t)(fine-tailSize) * 5. / (512. - 2.*tailSize);
   }

   return epoch * TRB_TDC3_COARSEUNIT * TRB_TDC3_COARSEBINS + coarse * TRB_TDC3_COARSEUNIT - fineTime;
*/
	return CbmTrbCalibrator::Instance()->GetFullTime(tdcId, channel, epoch, coarse, fine);
}

Double_t CbmRichTrbUnpack2::GetFullTime(CbmTrbRawMessage rawMess)
{
	UInt_t epoch = rawMess.GetEpochMarker();
	UInt_t coarse = rawMess.GetTDCcoarse();
	UInt_t fine = rawMess.GetTDCfine();
	UInt_t channel = rawMess.GetChannelID();
	UInt_t tdcid = rawMess.GetSourceAddress();

	return this->GetFullTime(tdcid, channel, epoch, coarse, fine);
}

ClassImp(CbmRichTrbUnpack2)
