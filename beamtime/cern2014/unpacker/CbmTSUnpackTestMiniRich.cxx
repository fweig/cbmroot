#include "CbmTSUnpackTestMiniRich.h"

// ROOT
#include "TClonesArray.h"
#include "TH1D.h"
#include "TH2D.h"

// FairRoot
#include "FairLogger.h"
#include "FairRootManager.h"

// Temporary
#include "Support.hpp"
#include "Timeslice.hpp"

// Project
#include "CbmTrbRawMessage.h"
#include "CbmTimeSlice.h"

#define RISINGEDGEID 1
#define FALLINGEDGEID 0

CbmTSUnpackTestMiniRich::CbmTSUnpackTestMiniRich() :
	CbmTSUnpack(),
  fCTSch1Fine(0.),
  fEventsCounter(0),
	fCurMSid(0),
  fInSubSubEvent(kFALSE),
  fCurEpochCounter(0)
{
}

CbmTSUnpackTestMiniRich::~CbmTSUnpackTestMiniRich()
{
}

Bool_t CbmTSUnpackTestMiniRich::Init()
{
	LOG(info) << "Initializing test mRICH unpacker";

	FairRootManager* ioman = FairRootManager::Instance();
	if (ioman == NULL) {
		LOG(fatal) << "No FairRootManager instance";
	}

	fTrbRawMessages = new TClonesArray("CbmTrbRawMessage");
	ioman->Register("fTrbRawMessages", "Raw TRB messages", fTrbRawMessages, kTRUE);

	// Hardcode TRB addresses
	this->InitTRBaddresses();

	// Init histograms
	this->InitHistograms();

	return kTRUE;
}

void CbmTSUnpackTestMiniRich::InitTRBaddresses(void)
{
	fNtdcs = 7;
	fAddresses.insert(std::pair<Int_t, Int_t>(0xc000, 0));
	fAddresses.insert(std::pair<Int_t, Int_t>(0x7000, 1));
	fAddresses.insert(std::pair<Int_t, Int_t>(0x7001, 2));
	fAddresses.insert(std::pair<Int_t, Int_t>(0x7100, 3));
	fAddresses.insert(std::pair<Int_t, Int_t>(0x7101, 4));
	fAddresses.insert(std::pair<Int_t, Int_t>(0x7110, 5));
	fAddresses.insert(std::pair<Int_t, Int_t>(0x7111, 6));

	fLastFirstChTime_CTS = 0.;
	for (Int_t i=0; i<fNtdcs; i++) {
		fCh0time.push_back(0.);
		fPrevCh0time.push_back(0.);
	}
}

void CbmTSUnpackTestMiniRich::InitHistograms(void)
{
	Double_t refValue = 102400.;
	fhZeroChDeltaT = new TH2D("fhZeroChDeltaT", "fhZeroChDeltaT;TDC;ns", fNtdcs, 0., fNtdcs, 4000, refValue-20., refValue+20.);
	fhFirstChDeltaT_CTS = new TH1D("fhFirstChDeltaT_CTS", "fhFirstChDeltaT_CTS;ns", 4000, 102400-20., 102400+20.);

	Double_t refValue3 = 0.;
	Double_t refWidth3 = 1000.;
	fhCTSCh1E0DeltaT = new TH1D("fhCTSCh1E0DeltaT", "fhCTSCh1E0DeltaT", 4000, refValue3-refWidth3/2., refValue3+refWidth3/2.);
	fhCTSCh1E1DeltaT = new TH1D("fhCTSCh1E1DeltaT", "fhCTSCh1E1DeltaT", 4000, refValue3-refWidth3/2., refValue3+refWidth3/2.);
	fhCTSCh2E0DeltaT = new TH1D("fhCTSCh2E0DeltaT", "fhCTSCh2E0DeltaT", 4000, refValue3-refWidth3/2., refValue3+refWidth3/2.);
	fhCTSCh2E1DeltaT = new TH1D("fhCTSCh2E1DeltaT", "fhCTSCh2E1DeltaT", 4000, refValue3-refWidth3/2., refValue3+refWidth3/2.);

	Double_t nbins = 5000;
	Double_t refValue2 = 0.;
	Double_t histoWidth = 1000.;
	for (Int_t i=0; i<fNtdcs; i++) {
		TString histoName;
		TString histoTitle;

		histoName.Form("fhDeltaT_LE_%d", i);
		histoTitle.Form("fhDeltaT_LE_%d;ch;ns", i);
		fhDeltaT_LE.push_back(new TH2D(histoName, histoTitle, 33, 0., 33., nbins, refValue2-histoWidth/2., refValue2+histoWidth/2.));
		histoName.Form("fhDeltaT_FE_%d", i);
		histoTitle.Form("fhDeltaT_FE_%d;ch;ns", i);
		fhDeltaT_FE.push_back(new TH2D(histoName, histoTitle, 33, 0., 33., nbins, refValue2-histoWidth/2., refValue2+histoWidth/2.));
		histoName.Form("fhDeltaT_LE_corr_%d", i);
		histoTitle.Form("fhDeltaT_LE_corr_%d;ch;ns", i);
		fhDeltaT_LE_corr.push_back(new TH2D(histoName, histoTitle, 33, 0., 33., 4000, 0., 400.));
		histoName.Form("fhDeltaT_FE_corr_%d", i);
		histoTitle.Form("fhDeltaT_FE_corr_%d;ch;ns", i);
		fhDeltaT_FE_corr.push_back(new TH2D(histoName, histoTitle, 33, 0., 33., 4000, 0., 400.));

		histoName.Form("fhFine_LE_%d", i);
		histoTitle.Form("fhFine_LE_%d;ch;fine time counter value", i);
		fhFine_LE.push_back(new TH2D(histoName, histoTitle, 33, 0., 33., 1025, 0., 1025.));
		histoName.Form("fhFine_FE_%d", i);
		histoTitle.Form("fhFine_FE_%d;ch;fine time counter value", i);
		fhFine_FE.push_back(new TH2D(histoName, histoTitle, 33, 0., 33., 1025, 0., 1025.));

		histoName.Form("fhToT_%d", i);
		histoTitle.Form("fhToT_%d;ch;ns", i);
		fhToT.push_back(new TH2D(histoName, histoTitle, 33, 0., 33., 2000, -10., 10.));
	}

}

void CbmTSUnpackTestMiniRich::WriteHistograms(void)
{
	fhZeroChDeltaT->Write();
	fhFirstChDeltaT_CTS->Write();

	fhCTSCh1E0DeltaT->Write();
	fhCTSCh1E1DeltaT->Write();
	fhCTSCh2E0DeltaT->Write();
	fhCTSCh2E1DeltaT->Write();

	for (Int_t i=0; i<fNtdcs; i++) {
		fhDeltaT_LE[i]->Write();
		fhDeltaT_FE[i]->Write();
		fhDeltaT_LE_corr[i]->Write();
		fhDeltaT_FE_corr[i]->Write();
	}

	for (Int_t i=0; i<fNtdcs; i++) {
		fhFine_LE[i]->Write();
	}

	for (Int_t i=0; i<fNtdcs; i++) {
		fhFine_FE[i]->Write();
	}

	for (Int_t i=0; i<fNtdcs; i++) {
		fhToT[i]->Write();
	}
}

Bool_t CbmTSUnpackTestMiniRich::DoUnpack(const fles::Timeslice& ts, size_t component)
{
	/*if (fEventsCounter%100 == 0)*/
	{
		LOG(info) << "Processing timeslice " << fEventsCounter;
	}
	fEventsCounter++;

	LOG(debug) << "Components:  " << ts.num_components();
	LOG(debug) << "Microslices: " << ts.num_microslices(component);

	/*const uint64_t compSize = ts.size_component(component);
	LOG(debug) << "Component " << component << " has size " << compSize;*/

	for (size_t iMS = 0; iMS < ts.num_microslices(component); ++iMS)
	{
		fCurMSid = iMS;
		LOG(debug) << "=======================================================";
		const fles::MicrosliceView mv = ts.get_microslice(component, iMS);
		const fles::MicrosliceDescriptor& msDesc = mv.desc();
		const uint8_t* msContent = mv.content();
		LOG(debug) << "msDesc.size=" << msDesc.size;
		LOG(debug) << "msDesc.idx=" << msDesc.idx;
		//PrintRaw(msDesc.size, msContent);
		//LOG(debug) << "=======================================================";
		ProcessMicroslice(msDesc.size, msContent);
		//LOG(debug) << "=======================================================";
	}
        return kTRUE;
}

void CbmTSUnpackTestMiniRich::Reset()
{
	fTrbRawMessages->Clear();
}

void CbmTSUnpackTestMiniRich::Finish()
{
	this->WriteHistograms();
}

void CbmTSUnpackTestMiniRich::ProcessMicroslice(size_t const size, uint8_t const * const ptr)
{
	if (size == 0) return;

	fGwordCnt = 0; //TODO testing

	Int_t offset; // offset in bytes
	Int_t* dataPtr;

	offset = 0; SwapBytes(4, ptr+offset);
	LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
	            << "Reserved 0000 0000";

	offset = 4; SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t mbsNumber = (Int_t)(dataPtr[0] & 0xffffff);
	LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
	            << "mbsNumber = " << mbsNumber;


	// We suppose that the first word is
	// "HadesTransportUnitQueue - Length"
	offset = 0+8; SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	fTRBeventSize1 = (Int_t)(dataPtr[0]);
	LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
	            << "HadesTransportUnitQueue - Length = " << fTRBeventSize1;

	if (*dataPtr >= 0 && uint(*dataPtr) == 0x80030000) {
		LOG(info) << "dataPtr == 0x80030000";
		exit(EXIT_FAILURE);
	}


	// We suppose that the second word is
	// "HadesTransportUnitQueue - Decoder  (Seems to be allways the same)"
	offset = 4+8; SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t dcdr = (Int_t)(dataPtr[0]);
	if (dcdr == 0x00030062) {
		LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "HadesTransportUnitQueue - Decoder = " << dcdr;
	} else {
		LOG(warning) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "er" << "\t"
		             << "HadesTransportUnitQueue - Decoder = " << dcdr
		             << " is not 0x00030062 (196706)";
	}

	// We suppose that the third word is
	// TRB event length (in bytes)
	// It should be 8 less than the size specified two words ago
	offset = 8+8; SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	fTRBeventSize2 = (Int_t)(dataPtr[0]);
	if (fTRBeventSize2 == fTRBeventSize1-8) {
		LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "TRB event - Length = " << fTRBeventSize2
		            << " == " << fTRBeventSize1 << "-8";
	} else {
		LOG(debug) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "er" << "\t"
		           << "TRB event - Length = " << fTRBeventSize2
		           << " != " << fTRBeventSize1 << "-8=" << fTRBeventSize1-8;
	}

	ProcessTRBevent(fTRBeventSize2, ptr+offset);
}

Int_t CbmTSUnpackTestMiniRich::ProcessTRBevent(size_t const size, uint8_t const * const ptr)
{
	Int_t offset; // offset in bytes
	Int_t* dataPtr;

	// Skip first word (already processed outside)

	// We suppose that the second word consists of
	// 0002 - number of following word till the Event Data Starts (should be the same)
	// 00<TriggerType>1 - value in [7:4] defines TriggerType
	offset = 4; SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t checkSize = (Int_t)((dataPtr[0] >> 16) & 0xffff);
	fTriggerType = (Int_t)((dataPtr[0] >> 4) & 0xf);
	if (checkSize == 2) {
		LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "checkSize == 2" << "\t"
		            << "trigger type = " << fTriggerType;
	} else {
		LOG(warning) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "er" << "\t"
		             << "checkSize != 2" << "\t"
		             << "trigger type = " << fTriggerType;
	}

	/*for (size_t iWord=2; iWord<size; iWord++) {
		offset = iWord*4;
		LOG(debug4) << "\t" << GetWordHexRepr(ptr+offset);
	}*/

	// We suppose that the third word consists of
	// 0000 <SubEventId>
	offset = 8; SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t checkBytes = (Int_t)((dataPtr[0] >> 16) & 0xffff);
	fSubEvId = (Int_t)((dataPtr[0]) & 0xffff);
	if (checkBytes == 0) {
		LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "checkBytes == 0" << "\t"
		            << "subevent ID = 0x" << GetHexRepresentation(2, ptr+offset);
	} else {
		LOG(warning) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "er" << "\t"
		             << "checkBytes != 0" << "\t"
		             << "subevent ID = 0x" << GetHexRepresentation(2, ptr+offset);
	}

	// We suppose that the fourth word is the trigger number
	offset = 12; SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	fTriggerNum = (Int_t)(dataPtr[0]);
	LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
				<< "trigger num = " << fTriggerNum;

	offset = 16;

	// Int_t iIter = 0;  (VF) unused

	while (static_cast<size_t>(offset) < size) {

		//std::cout << "SE iteration " << iIter++ << "\toffset=" << offset << "\tsize=" << size << std::endl;

		// We suppose that the fifth word is the header of the subevent
		// <Length> <HubId>
		SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
		fSubEvSize = (Int_t)((dataPtr[0] >> 16) & 0xffff);
		fHubId = (Int_t)((dataPtr[0]) & 0xffff);
		LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
					<< "hub ID = 0x" << GetHexRepresentation(2, ptr+offset) << "\t"
					<< "subevent size = " << fSubEvSize;

		//FIXME change from 0xc001 to 0xc000 at some point
		if ((fHubId == 0xc001) || (fHubId == 0xc000)) {
			fSubSubEvId = fHubId;
			offset += (4 + ProcessCTSsubevent(fSubEvSize*4, ptr+offset));
			// In principle, should be reset here for safety
			fSubSubEvId = 0;
		} else if (fHubId == 0x5555) {
			//TODO one could implement additional checks here about the
			// words coming after the "event end" but we skip everything by now.
			offset += (4 + ProcessSKIPsubevent(fSubEvSize*4, ptr+offset));
		} else {
			offset += (4 + ProcessTRBsubevent(fSubEvSize*4, ptr+offset));
		}
	}

	//TODO implement checks
	//std::cout << "Done processing TRB event. offset=" << offset << "\tsize=" << size << std::endl;

	return size; //TODO check
}

Int_t CbmTSUnpackTestMiniRich::ProcessTRBsubevent(size_t const size, uint8_t const * const ptr)
{
	//std::cout << "ProcessTRBsubevent size=" << size << " bytes" << std::endl;

	Int_t offset; // offset in bytes
	Int_t* dataPtr;

	// Skip first word (already processed outside)

	// Int_t iIter = 0;  (VF) unused
	offset = 4;

	while (static_cast<size_t>(offset) < size) {

		//std::cout << "SSE iteration " << iIter++ << "\toffset=" << offset << "\tsize=" << size << std::endl;

		// We suppose that the second word is the header of the subsubevent
		// <Length> <SubSubEv.Id>
		SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
		fSubSubEvSize = (Int_t)((dataPtr[0] >> 16) & 0xffff);
		fSubSubEvId = (Int_t)((dataPtr[0]) & 0xffff);
		LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
					<< "subsubevent ID (FPGA ID) = 0x" << GetHexRepresentation(2, ptr+offset) << "\t"
					<< "subsubevent size = " << fSubSubEvSize;

		// Add 4 bytes which correspond to the header word
		offset += (4 + ProcessTRBsubsubevent(fSubSubEvSize*4, ptr+offset+4)); // MAIN CALL HERE

		// In principle, should be reset here for safety
		fSubSubEvId = 0;

	}


	//TODO implement checks
	//std::cout << "Done processing TRB subevent. offset=" << offset << "\tsize=" << size << std::endl;

	return size; //TODO check
}

Int_t CbmTSUnpackTestMiniRich::ProcessCTSsubevent(size_t const size, uint8_t const * const ptr)
{
	//std::cout << "ProcessCTSsubevent size=" << size << " bytes" << std::endl;

	Int_t offset; // offset in bytes
	Int_t* dataPtr;

	// Skip first word (already processed outside)

	// We suppose that the second word is the header of the subsubevent
	offset = 4; SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
	            << "CTS header";

/* (FU) not used
	Short_t trigState[16];
	for (Int_t i=0; i<16; i++) {
		trigState[i] = ((*dataPtr >> i) & 0x1); // 16 x 1 bit
	}
*/

	Short_t nInp = ((*dataPtr >> 16) & 0xf); // 4 bits
	Short_t nTrigCh = ((*dataPtr >> 20) & 0x1f); // 5 bits
	Short_t inclLastIdle = ((*dataPtr >> 25) & 0x1); // 1 bit
	Short_t inclTrigInfo = ((*dataPtr >> 26) & 0x1); // 1 bit
	Short_t inclTS = ((*dataPtr >> 27) & 0x1); // 1 bit
	Short_t ETM = ((*dataPtr >> 28) & 0x3); // 2 bits

	// in words (not bytes)
	Short_t CTSinfo_size = nInp*2 + nTrigCh*2 + inclLastIdle*2 + inclTrigInfo*3 + inclTS;
	switch (ETM) {
		case 0:
			break;
		case 1:
			CTSinfo_size += 1;
			break;
		case 2:
			CTSinfo_size += 4;
			break;
		case 3:
			LOG(debug) << "ETM == 3";
			//TODO implement
			break;
	}

	LOG(debug) << "CTS information size (extracted from the CTS header): " << CTSinfo_size;

	offset = 8;

	while (offset-8 < CTSinfo_size*4) {

		SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
		ULong_t MSidx = 102400UL * ((ULong_t)(*dataPtr) - 1);
		LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset) << "\t" << MSidx << "\t" << "ok" << "\t"
		            << "CTS information";

		offset += 4;
	}

	// size - full size including CTS header word, CTS informations words (CTSinfo_size) and TCD data
	// Thus TDC data size = full size - 1 word (header) - CTSinfo_size words (CTS informations)
	offset += (ProcessTRBsubsubevent((size-(1+CTSinfo_size)*4), ptr+offset)); // MAIN CALL HERE

	//TODO implement checks
	//std::cout << "Done processing CTS subevent. offset=" << offset << "\tsize=" << size << std::endl;

	return size; //TODO check
}

Int_t CbmTSUnpackTestMiniRich::ProcessSKIPsubevent(size_t const size, uint8_t const * const ptr)
{
	//std::cout << "ProcessSKIPsubevent size=" << size << " bytes" << std::endl;

	Int_t offset; // offset in bytes
//	Int_t* dataPtr; (FU) not used

	// Skip first word (already processed outside)

	// Int_t iIter = 0;   (VF) unused
	offset = 4;

	while (static_cast<size_t>(offset) < size+4) {

		SwapBytes(4, ptr+offset); 
//              dataPtr = (Int_t*)(ptr+offset); (FU) not used
		LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr+offset);

		offset += 4;
	}

	//TODO implement checks
	//std::cout << "Done processing SKIP subevent. offset=" << offset << "\tsize=" << size << std::endl;

	return size; //TODO check
}

Int_t CbmTSUnpackTestMiniRich::ProcessTRBsubsubevent(size_t const size, uint8_t const * const ptr)
{
	//std::cout << "ProcessTRBsubsubevent size=" << size  << " bytes" << std::endl;

	fCurEpochCounter = 0; //TODO check
	fInSubSubEvent = kFALSE; //TODO check

	Int_t foundCh0times = this->GetCh0Time(size, ptr);

	if (foundCh0times == 0) {
		LOG(error) << "No (ch==0 && edge==1) messages found.";
	} else if (foundCh0times > 1) {
		LOG(error) << "More than one (ch==0 && edge==1) message found.";
	} /*else {
		LOG(debug3) << "Exactly one (ch==0 && edge==1) message found.";
	}*/

	// To be on the safe side - reset again
	fCurEpochCounter = 0; //TODO check

	for (size_t iWord=0; iWord<size/4; iWord++) {
		////SwapBytes(4, ptr+iWord*4); //now implemented in GetCh0Time() which is called right before
		ProcessTDCword(ptr+iWord*4); // MAIN CALL HERE
	}

	//TODO Implement checks that the first word was the header and the last word was the trailer

	return size; //TODO check
}

Int_t CbmTSUnpackTestMiniRich::GetCh0Time(size_t const size, uint8_t const * const ptrStart)
{
	Int_t nFound = 0; // Counter of (ch==0 && edge==1) messages found in this subsubevent

	Int_t tdcIndex = fAddresses[fSubSubEvId];

	for (size_t iWord=0; iWord<size/4; iWord++)
	{
		uint8_t const * ptr = ptrStart+iWord*4;

		SwapBytes(4, ptr); //!!!!!!!

		Int_t* tdcDataPtr = (Int_t*)ptr;
		Int_t tdcData = tdcDataPtr[0];
		Int_t tdcTimeDataMarker = (tdcData >> 31) & 0x1; // 1 bit

		if (tdcTimeDataMarker == 0x1) { // Process only TIME messages, by now ignore anything else (HEADER, TRAILER...)

			Int_t channel = (tdcData >> 22) & 0x7f; // 7 bits
			Int_t fine = (tdcData >> 12) & 0x3ff; // 10 bits
			Int_t edge = (tdcData >> 11) & 0x1; // 1 bit
			Int_t coarse = (tdcData) & 0x7ff; // 11 bits
			Int_t epoch = fCurEpochCounter;

			Double_t fullTime = (Double_t)epoch*2048.*5. + (Double_t)(coarse)*5. - (Double_t)(fine)*0.005;

			LOG(debug4) << "[pre] " << GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "TIMESTAMP" << "\t"
					  << "ch=" << channel << "\t"
					  << "edge=" << edge << "\t"
					  << "epoch=" << epoch << "\t"
					  << "coarse=" << coarse << "\t"
					  << "fine=" << fine << "\t"
					  << "full=" << fullTime;

			if (fSubSubEvId == 0xc000 && channel == 1 && edge == RISINGEDGEID) {
				fCTSch1Fine = (Double_t)(fine)*0.005;
			}

			if (channel == 0 && edge == RISINGEDGEID) {
				nFound++;

				fhZeroChDeltaT->Fill(tdcIndex, (fullTime - fPrevCh0time[tdcIndex]));

				fPrevCh0time[tdcIndex] = fCh0time[tdcIndex];
				fCh0time[tdcIndex] = fullTime; // only last found time is saved if multiple messages are found in the subsubsevent

				LOG(debug4) << "[pre] hit! fCh0time=" << fCh0time[tdcIndex] << "\tfPrevCh0time=" << fPrevCh0time[tdcIndex];
			}

			/*if (channel == 2 && edge == FALLINGEDGEID) {
				fasd = fullTime - fPrevCh0time[tdcIndex];
				LOG(debug4) << "fasd=" << fasd;
			}*/

			if (fSubSubEvId == 0xc000) {
				if (channel == 1) {
					if (edge == 0) {
						fCTSCh1E0dT_prev = fCTSCh1E0dT;
						fCTSCh1E0dT = fullTime - fCh0time[0];
						fhCTSCh1E0DeltaT->Fill(fCTSCh1E0dT);
					} else {
						fCTSCh1E1dT_prev = fCTSCh1E1dT;
						fCTSCh1E1dT = fullTime - fCh0time[0];
						fhCTSCh1E1DeltaT->Fill(fCTSCh1E1dT);
					}
				} else if (channel == 2) {
					if (edge == 0) {
						fCTSCh2E0dT_prev = fCTSCh2E0dT;
						fCTSCh2E0dT = fullTime - fCh0time[0];
						fhCTSCh2E0DeltaT->Fill(fCTSCh2E0dT);
					} else {
						fCTSCh2E1dT_prev = fCTSCh2E1dT;
						fCTSCh2E1dT = fullTime - fCh0time[0];
						fhCTSCh2E1DeltaT->Fill(fCTSCh2E1dT);
					}
				}
			}

		} else { // Process only EPOCH messages, by now ignore anything else (HEADER, TRAILER...)

			UInt_t tdcMarker = (tdcData >> 29) & 0x7; // 3 bits

			if (tdcMarker == 0x3) { // EPOCH counter
				fCurEpochCounter = (tdcData) & 0xfffffff; // 28 bits
			}
		}
	}

	return nFound;
}

void CbmTSUnpackTestMiniRich::ProcessTDCword(uint8_t const * const ptr)
{
	Int_t* tdcDataPtr = (Int_t*)ptr;
	Int_t tdcData = tdcDataPtr[0];
	Int_t tdcTimeDataMarker = (tdcData >> 31) & 0x1; // 1 bit

	if (tdcTimeDataMarker == 0x1) {

		Int_t channel = (tdcData >> 22) & 0x7f; // 7 bits
		Int_t fine = (tdcData >> 12) & 0x3ff; // 10 bits
		Int_t edge = (tdcData >> 11) & 0x1; // 1 bit
		Int_t coarse = (tdcData) & 0x7ff; // 11 bits
		Int_t epoch = fCurEpochCounter;

		Double_t fullTime = (Double_t)epoch*2048.*5. + (Double_t)(coarse)*5. - (Double_t)(fine)*0.005;

		LOG(debug4) << "[" << fGwordCnt << "]\t" << GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "TIMESTAMP" << "\t"
		          << "ch=" << channel << "\t"
		          << "edge=" << edge << "\t"
		          << "epoch=" << epoch << "\t"
		          << "coarse=" << coarse << "\t"
		          << "fine=" << fine << "\t"
		          << "full=" << fullTime;

		fGwordCnt++;

		// Fine time couter value distribution
		LOG(debug4) << std::hex << fSubSubEvId << std::dec;
		Int_t tdcIndex = fAddresses[fSubSubEvId];
		LOG(debug4) << "TDC 0x" << std::hex << fSubSubEvId << std::dec << " index=" << tdcIndex;

		// --------------------------------------------------------------------
		if (edge == FALLINGEDGEID) {
			fhFine_FE[tdcIndex]->Fill(channel, fine);
		} else {
			fhFine_LE[tdcIndex]->Fill(channel, fine);
		}
		// --------------------------------------------------------------------

		// --------------------------------------------------------------------
		Double_t curDiff2 = fullTime - fLastFirstChTime_CTS;
		if (fSubSubEvId == 0xc000 && channel == 1 && edge == RISINGEDGEID) {
			fhFirstChDeltaT_CTS->Fill(curDiff2);
			LOG(debug4) << "CTS ch " << channel << " deltaT=" << curDiff2;
			fLastFirstChTime_CTS = fullTime;
		}
		// --------------------------------------------------------------------

		// --------------------------------------------------------------------
		Double_t curDiff = fullTime - fPrevCh0time[tdcIndex];
		if (channel != 0) {
			LOG(debug4) << "TDC 0x" << std::hex << fSubSubEvId << std::dec << " (" << tdcIndex << ")" << "\t"
			            << "ch=" << channel << "\t"
			            << "edge=" << edge << "\t"
						<< "time=" << fullTime << "\t"
			            << "diff=" << curDiff;
			if (edge == FALLINGEDGEID) {
				fhDeltaT_FE[tdcIndex]->Fill(channel, curDiff);
				fhDeltaT_FE_corr[tdcIndex]->Fill(channel, curDiff - fCTSCh2E1dT); // fCTSch1Fine
			} else { // LEADING
				fhDeltaT_LE[tdcIndex]->Fill(channel, curDiff);
				fhDeltaT_LE_corr[tdcIndex]->Fill(channel, curDiff - fCTSCh2E1dT); // fCTSch1Fine
			}
		}
		// --------------------------------------------------------------------

		WriteEdge(fSubSubEvId, channel, edge, epoch, coarse, fine);

	} else {

		UInt_t tdcMarker = (tdcData >> 29) & 0x7; // 3 bits

		if (tdcMarker == 0x0) { // TDC trailer
			if (fInSubSubEvent) {
				LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "TDC TRAILER";
				fInSubSubEvent = kFALSE;
			} else {
				LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr) << "\t" << "er" << "\t" << "UNKNOWN (TDC TRAILER not after header)";
				exit(EXIT_FAILURE);
			}
		} else if (tdcMarker == 0x1) { // TDC header
		//	UInt_t randomCode = (tdcData >> 16) & 0xff; // 8 bits
		//	UInt_t errorBits = (tdcData) & 0xffff; //16 bits
			if (!fInSubSubEvent) {
				LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "TDC HEADER";
				fInSubSubEvent = kTRUE;
			} else {
				LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr) << "\t" << "er" << "\t" << "UNKNOWN (TDC HEADER not after trailer)";
			}
		} else if (tdcMarker == 0x2) { // DEBUG
		//	UInt_t debugMode = (tdcData >> 24) & 0x1f; // 5 bits
		//	UInt_t debugBits = (tdcData) & 0xffffff; // 24 bits
			LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "DEBUG";
		} else if (tdcMarker == 0x3) { // EPOCH counter
			fCurEpochCounter = (tdcData) & 0xfffffff; // 28 bits
			LOG(debug4) << "[" << fGwordCnt << "]\t" << GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "EPOCH\t" << fCurEpochCounter;
			fGwordCnt++;
		} else {
			if (tdcTimeDataMarker != 0x1) {
				LOG(debug4) << "[" << fGwordCnt++ << "]\t" << GetWordHexRepr(ptr) << "\t" << "er" << "\t" << "UNKNOWN";
			}
		}
	}
}

void CbmTSUnpackTestMiniRich::WriteEdge(CbmTrbRawMessage* p_obj)
{
	Int_t nEdges = fTrbRawMessages->GetEntriesFast();
	new ((*fTrbRawMessages)[nEdges]) CbmTrbRawMessage(*p_obj);
}

void CbmTSUnpackTestMiniRich::WriteEdge(Int_t febID, Int_t ch, Int_t edge, Int_t epoch, Int_t coarse, Int_t fine)
{
	Int_t nEdges = fTrbRawMessages->GetEntriesFast();
	new ((*fTrbRawMessages)[nEdges]) CbmTrbRawMessage(0x30, febID, ch, epoch, coarse, fine, edge, 0., fCurMSid);
}

ClassImp(CbmTSUnpackTestMiniRich)
