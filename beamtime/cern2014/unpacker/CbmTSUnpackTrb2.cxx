/*
 * CbmTSUnpackTrb2.cxx
 *
 *  Created on: Mar 24, 2015
 *      Author: evovch
 */

#include "CbmTSUnpackTrb2.h"

#include <TClonesArray.h>

#include "FairLogger.h"
#include "FairRootManager.h"

#include "CbmTrbIterator.h"
#include "CbmTrbCalibrator.h"
#include "CbmRichTrbParam.h"

#include "CbmRawEvent.h"
#include "CbmRawSubEvent.h"
#include "CbmTrbRawMessage.h"

// Uncoment if you want to have excessive printout (do not execute on many events, may produce Gb's of output)
//#define DEBUGPRINT

CbmTSUnpackTrb2::CbmTSUnpackTrb2() :
	CbmTSUnpack(),
	fLink(0),
	fTrbBridge(new TrbBridge()),
	fTrbEventList(),
    fData(),
    fDataSize(0),
    fTrbIter(NULL),
    fRawEvent(NULL),
    fRawSubEvent(NULL),
	fTimestampsUnpacked(0),
	fTrbRawHits(new TClonesArray("CbmTrbRawMessage", 10)),

	fLastSyncTime(0.),
	fMainSyncTime(0.),
	fCorr(0.),
	fMainSyncDefined(kFALSE),

	startTime(0.),
	startTimeKnown(kFALSE),
	hitDistrInTime(new TH1D("hitDistrInTime", "hitDistrInTime", 1200, 0., 120.)),
	hitDistrInTimeOneSpill(new TH1D("hitDistrInTimeOneSpill", "hitDistrInTimeOneSpill", 5000, 3., 5.))

{

}

CbmTSUnpackTrb2::~CbmTSUnpackTrb2()
{

}

Bool_t CbmTSUnpackTrb2::Init()
{
	LOG(debug) << "Initializing";

	FairRootManager* ioman = FairRootManager::Instance();
	if (ioman == NULL) {
		LOG(fatal) << "No FairRootManager instance";
	}
	ioman->Register("CbmTrbRawMessage", "TRB raw messages", fTrbRawHits, kTRUE);

	return kTRUE;
}

Bool_t CbmTSUnpackTrb2::DoUnpack(const fles::Timeslice& ts, size_t component)
{
	//LOG(info) << "Unpacking Trb Data";
	//LOG(info) << "--- Number of microslices: " << ts.num_microslices(component);

	fLink = ts.descriptor(component, 0).eq_id;

	Int_t skipMS = 1; //TODO: should be set from macro according to the flib settings
	fTrbEventList = fTrbBridge->extractEventsFromTimeslice(ts, skipMS);

	LOG(debug) << "Found " << fTrbEventList.size() << " TRB events in time slice";

	// Loop over all TRB events which are in the timeslice
	while ( !fTrbEventList.empty() ) {
		fData = fTrbEventList.front();
		fDataSize = fData.size();
		UnpackTrbEvents();
		fTrbEventList.pop_front();
	}

	return kTRUE;
}

void CbmTSUnpackTrb2::UnpackTrbEvents()
{
	fTrbIter = new CbmTrbIterator(static_cast<void*>(fData.data()), fDataSize);

	// Loop over TRB events
	while (true) {

		// Try to extract next event from the Iterator. If no events left - go out of the loop
		fRawEvent = fTrbIter->NextEvent();
		if (fRawEvent == NULL) break;
#ifdef DEBUGPRINT
		//if(gLogger->IsLogNeeded( fair::Severity::debug ))
		{
			fRawEvent->Print();
		}
#endif

//		fSynchRefTime = -1.;

		// Loop over TRB subevents
		while (true){
			fRawSubEvent = fTrbIter->NextSubEvent();
			if (fRawSubEvent == NULL) break;
#ifdef DEBUGPRINT
			if(gLogger->IsLogNeeded( fair::Severity::debug )) {
				fRawSubEvent->Print();
			}
#endif
			ProcessSubEvent(fRawSubEvent);
		}

	}
}

void CbmTSUnpackTrb2::Reset()
{
	fTrbRawHits->Clear();
}

void CbmTSUnpackTrb2::Finish()
{
	LOG(info) << "CbmTSUnpackTrb2::Finish";
}

void CbmTSUnpackTrb2::ProcessSubEvent(CbmRawSubEvent* subEvent) {
	UInt_t tdcDataIndex = 0;
	UInt_t tdcData = 0x0;
	UInt_t subEventId = subEvent->SubId();

	fLastSyncTime = 0.;
	fMainSyncTime = 0.;
	fCorr = 0.;
	fMainSyncDefined = kFALSE;

	// Loop over subsubevents
	while (true) {
		tdcData = subEvent->SubDataValue(tdcDataIndex);
		UInt_t tdcNofWords = (tdcData >> 16) & 0xffff;
		UInt_t subSubEventId = tdcData & 0xffff;
#ifdef DEBUGPRINT
      printf("%08x - SUBSUBEVENT HEADER    tdcNofWords = %i, ID = 0x%04x\n", tdcData, tdcNofWords, subSubEventId);
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

		if (subSubEventId != 0x7005 && subSubEventId != 0x0112) {
			ProcessSubSubEvent(dataArray, tdcNofWords, subSubEventId);
		} else {
			ProcessSkip(dataArray, tdcNofWords, subSubEventId);
		}

		tdcDataIndex++;
	}

}

void CbmTSUnpackTrb2::ProcessSkip(UInt_t* data, UInt_t size, UInt_t subEventId)
{
#ifdef DEBUGPRINT
	for (UInt_t i=0; i<size; i++) {
		//printf ("%08x - skip\n", data[i]);
	}
#endif
}

void CbmTSUnpackTrb2::ProcessSubSubEvent(UInt_t* data, UInt_t size, UInt_t subEventId) {
	CbmRichTrbParam* param = CbmRichTrbParam::Instance();

	UInt_t tdcId = subEventId;

	UInt_t curEpochCounter = 0;

	for (UInt_t i = 0; i < size; i++) {
		UInt_t tdcData = data[i];
		UInt_t tdcTimeDataMarker = (tdcData >> 31) & 0x1;

		if (tdcTimeDataMarker == 0x1)  // TIME DATA
		{
#ifdef DEBUGPRINT
			printf("%08x - TIMESTAMP", tdcData);
#endif
         fTimestampsUnpacked++;

			UInt_t channel = (tdcData >> 22) & 0x7f;
			UInt_t fine = (tdcData >> 12) & 0x3ff;
			UInt_t edge = (tdcData >> 11) & 0x1;
			UInt_t coarse = (tdcData) & 0x7ff;
			UInt_t epoch = curEpochCounter;

			if (param->IsSyncChannel(channel)) {     // SYNC MESSAGE PROCESSING
				//CbmTrbCalibrator::Instance()->AddFineTime(tdcId, channel, fine);    //TODO check - needed or not?

				// Between-TDC synchronization
				fLastSyncTime = GetFullTime(tdcId, channel, epoch, coarse, fine);
				if (!fMainSyncDefined) {
					fMainSyncDefined = kTRUE;
					fMainSyncTime = GetFullTime(tdcId, channel, epoch, coarse, fine);
				}
				fCorr = fMainSyncTime-fLastSyncTime;

#ifdef DEBUGPRINT
				printf("   --- SYNC  - tdc 0x%04x ch %d edge %d epoch %08x coarse %08x fine %08x = %f\tcorrection=%f\n",
					tdcId, channel, edge, epoch, coarse, fine, GetFullTime(tdcId, channel, epoch, coarse, fine)+fCorr, fCorr);
#endif
			} else if (param->IsLeadingEdgeChannel(channel)) { // LEADING EDGE PROCESSING
				CbmTrbCalibrator::Instance()->AddFineTime(tdcId, channel, fine);
#ifdef DEBUGPRINT
				printf("   --- LEAD  - tdc 0x%04x ch %d edge %d epoch %08x coarse %08x fine %08x = %f\tcorrection=%f\n",
					tdcId, channel, edge, epoch, coarse, fine, GetFullTime(tdcId, channel, epoch, coarse, fine)+fCorr, fCorr);
#endif
				Double_t ftime = GetFullTime(tdcId, channel, epoch, coarse, fine)+fCorr;

				if (!startTimeKnown) { startTime = ftime; startTimeKnown = kTRUE; }

				hitDistrInTime->Fill((ftime - startTime) / 1000000000);
				if ((ftime - startTime)/1000000000 > 3. && (ftime - startTime)/1000000000 < 5.) {
					hitDistrInTimeOneSpill->Fill((ftime - startTime)/1000000000);
				}

				//if (tdcId == 0x0110 && channel == 7) printf ("  LED pulse\n");

			} else {                                 // TRAILING EDGE PROCESSING
				CbmTrbCalibrator::Instance()->AddFineTime(tdcId, channel, fine);
#ifdef DEBUGPRINT
				printf("   --- TRAIL - tdc 0x%04x ch %d edge %d epoch %08x coarse %08x fine %08x = %f\tcorrection=%f\n",
					tdcId, channel, edge, epoch, coarse, fine, GetFullTime(tdcId, channel, epoch, coarse, fine)+fCorr, fCorr);
#endif

				Double_t ftime = GetFullTime(tdcId, channel, epoch, coarse, fine)+fCorr;

				hitDistrInTime->Fill((ftime - startTime) / 1000000000);
				if ((ftime - startTime)/1000000000 > 3. && (ftime - startTime)/1000000000 < 5.) {
					hitDistrInTimeOneSpill->Fill((ftime - startTime)/1000000000);
				}

			}

			// Important to keep it here - after fCorr computation
			new ((*fTrbRawHits)[fTrbRawHits->GetEntriesFast()]) CbmTrbRawMessage(
					0, tdcId, channel, epoch, coarse, fine, edge, fCorr);

		} // if TIME DATA

		UInt_t tdcMarker = (tdcData >> 29) & 0x7; //3 bits
		if (tdcMarker == 0x1) { // TDC header
			UInt_t randomCode = (tdcData >> 16) & 0xff; // 8bits
			UInt_t errorBits = (tdcData) & 0xffff; //16 bits
#ifdef DEBUGPRINT
			printf("%08x - TDC HEADER randomCode:0x%02x, errorBits:0x%04x\n", tdcData, randomCode, errorBits);
#endif
		} else if (tdcMarker == 0x2) { // DEBUG
			UInt_t debugMode = (tdcData >> 24) & 0x1f; //5 bits
			UInt_t debugBits = (tdcData) & 0xffffff; //24 bits
#ifdef DEBUGPRINT
			printf("%08x - DEBUG debugMode:%i, debugBits:0x%06x\n", tdcData, debugMode, debugBits);
#endif
		} else if (tdcMarker == 0x3) { // EPOCH counter
			curEpochCounter = (tdcData) & 0xfffffff; //28 bits
#ifdef DEBUGPRINT
//			printf("%08x - EPOCH COUNTER epochCounter:0x%07x\n", tdcData, curEpochCounter);
#endif
		} else {
#ifdef DEBUGPRINT
			if (tdcTimeDataMarker != 0x1) printf("%08x\n", tdcData);
#endif
		}

	} // for loop

}

Double_t CbmTSUnpackTrb2::GetFullTime(UInt_t tdcId, UInt_t channel, UInt_t epoch, UInt_t coarse, UInt_t fine)
{
	return CbmTrbCalibrator::Instance()->GetFullTime(tdcId, channel, epoch, coarse, fine);
}

ClassImp(CbmTSUnpackTrb2)
