#ifndef CBMTSUNPACKTESTMINIRICH_H
#define CBMTSUNPACKTESTMINIRICH_H

#include "CbmTSUnpack.h" // mother class

// STD
#include <map>
#include <vector>

// ROOT
#include <TH2D.h>

class TClonesArray;
class CbmTrbRawMessage;

class CbmTSUnpackTestMiniRich : public CbmTSUnpack
{
public:
	CbmTSUnpackTestMiniRich();
	virtual ~CbmTSUnpackTestMiniRich();

	virtual Bool_t Init();

	void InitTRBaddresses(void);

	void InitHistograms(void);

	void WriteHistograms(void);

	virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

	virtual void Reset();

	virtual void Finish();

	virtual void SetParContainers() {;}

	virtual void FillOutput(boost::any) {;}

private: // histograms

	TH2D* fhZeroChDeltaT;
	TH1D* fhFirstChDeltaT_CTS;

	TH1D* fhCTSCh1E0DeltaT;
	TH1D* fhCTSCh1E1DeltaT;
	TH1D* fhCTSCh2E0DeltaT;
	TH1D* fhCTSCh2E1DeltaT;

	std::vector<TH2D*> fhDeltaT_LE;
	std::vector<TH2D*> fhDeltaT_FE;
	std::vector<TH2D*> fhDeltaT_LE_corr;
	std::vector<TH2D*> fhDeltaT_FE_corr;
	std::vector<TH2D*> fhFine_LE;
	std::vector<TH2D*> fhFine_FE;
	std::vector<TH2D*> fhToT;

private:

	Int_t fNtdcs;

	std::map<Int_t, Int_t> fAddresses;

	Double_t fLastFirstChTime_CTS;

	std::vector<Double_t> fCh0time;

	std::vector<Double_t> fPrevCh0time;

	Double_t fCTSch1Fine;

	Double_t fCTSCh1E0dT;
	Double_t fCTSCh1E1dT;
	Double_t fCTSCh2E0dT;
	Double_t fCTSCh2E1dT;

	Double_t fCTSCh1E0dT_prev;
	Double_t fCTSCh1E1dT_prev;
	Double_t fCTSCh2E0dT_prev;
	Double_t fCTSCh2E1dT_prev;

private:

	void ProcessMicroslice(size_t const size, uint8_t const * const ptr);

	/**
	 * Including header
	 */
	Int_t ProcessTRBevent(size_t const size, uint8_t const * const ptr);

	/**
	 * Including header
	 * Return number of processed bytes
	 */
	Int_t ProcessTRBsubevent(size_t const size, uint8_t const * const ptr);

	/**
	 * Including TDC header, but not including TRB subsubevent header
	 * Return number of processed bytes
	 */
	Int_t ProcessTRBsubsubevent(size_t const size, uint8_t const * const ptr);

	Int_t ProcessCTSsubevent(size_t const size, uint8_t const * const ptr);

	Int_t ProcessSKIPsubevent(size_t const size, uint8_t const * const ptr);

	Int_t GetCh0Time(size_t const size, uint8_t const * const ptrStart);

	void ProcessTDCword(uint8_t const * const ptr);

	/**
	 * Write unpacked edge into the output collection
	 */
	void WriteEdge(CbmTrbRawMessage* p_obj);

	/**
	 * Write unpacked edge into the output collection
	 */
	void WriteEdge(Int_t febID, Int_t ch, Int_t edge, Int_t epoch, Int_t coarse, Int_t fine);

private:

	Int_t fEventsCounter;

	Int_t fCurMSid;
	Int_t fGwordCnt; // global word counter within current microslice

	Bool_t fInSubSubEvent;

	UInt_t fCurEpochCounter;
	Int_t fTRBeventSize1;
	Int_t fTRBeventSize2;
	Int_t fTriggerType;
	Int_t fSubEvId;
	Int_t fTriggerNum;
	Int_t fSubEvSize;
	Int_t fHubId;
	Int_t fSubSubEvSize;
	Int_t fSubSubEvId;

	// Output collection
	TClonesArray* fTrbRawMessages;

	ClassDef(CbmTSUnpackTestMiniRich, 1);
};

#endif // CBMTSUNPACKTESTMINIRICH_H
