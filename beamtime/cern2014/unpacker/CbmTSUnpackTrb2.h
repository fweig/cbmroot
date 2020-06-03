/*
 * CbmTSUnpackTrb2.h
 *
 *  Created on: Mar 24, 2015
 *      Author: evovch
 */

#ifndef FLES_READER_UNPACKER_CBMTSUNPACKTRB2_H_
#define FLES_READER_UNPACKER_CBMTSUNPACKTRB2_H_

#include "CbmTSUnpack.h"

#include "TH1.h"

#include "Timeslice.hpp"
#include "Message.hpp"
#include "TrbBridge.hpp"

class CbmTrbIterator;
class CbmRawEvent;
class CbmRawSubEvent;

class CbmTSUnpackTrb2: public CbmTSUnpack {
public:
	CbmTSUnpackTrb2();
	virtual ~CbmTSUnpackTrb2();

	virtual Bool_t Init();

	virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

	void UnpackTrbEvents();

	virtual void Reset();

	virtual void Finish();

private:

	Int_t fLink;

	TrbBridge* fTrbBridge;
	std::list<std::vector<uint32_t>> fTrbEventList;
	std::vector<uint32_t> fData;
	size_t fDataSize;

	CbmTrbIterator* fTrbIter; //!
	CbmRawEvent* fRawEvent;  //!
	CbmRawSubEvent* fRawSubEvent; //!

	//Between-TDC synchronization
	Double_t fLastSyncTime;
	Double_t fMainSyncTime;
	Double_t fCorr;
	Bool_t fMainSyncDefined;

	// Histo for many-seconds hit distribution in time
	TH1D* hitDistrInTime;
	TH1D* hitDistrInTimeOneSpill;
	Double_t startTime;
	Bool_t startTimeKnown;

	// Output raw hits
	TClonesArray* fTrbRawHits;

	// Counter of timestamps being unpacked
	UInt_t fTimestampsUnpacked;

	void ProcessSubEvent(CbmRawSubEvent* subEvent);
	void ProcessSubSubEvent(UInt_t* data, UInt_t size, UInt_t subEventId);
	void ProcessSkip(UInt_t* data, UInt_t size, UInt_t subEventId);

	Double_t GetFullTime(UInt_t tdcId, UInt_t channel, UInt_t epoch, UInt_t coarse, UInt_t fine);

	ClassDef(CbmTSUnpackTrb2, 1)
};

#endif /* FLES_READER_UNPACKER_CBMTSUNPACKTRB2_H_ */
