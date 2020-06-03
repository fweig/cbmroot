/**
 * CbmMcbm2018UnpackerAlgoRich
 * E. Ovcharenko, Mar 2019
 * based on other detectors' classes by P.-A. Loizeau
 */

/**

Consider two consequent microslices A and B.
Each microslice contains one CTS subevent which contains at least 3 timestamp messages.
A microslice may also contain TRB subevents, each containing also one timestamp message from ch0.

Microslice A
===================
CTS   | ch 0 re      = AC0R (stands for ms A, Cts, ch 0, Rising edge)
CTS   | ch 2 fe
CTS   | ch 2 re      = AC2R (stands for ms A, Cts, ch 2, Rising edge)
-------------------
TDC K | ch 0 re
-------------------
TDC Z | ch 0 re      = AZ0R (stands for ms A, tdc Z, ch 0, Rising edge))
      | ch X re
      | ch X fe
      | ...
-------------------
...
===================

Microslice B (next after A)
===================
CTS   | ch 0 re
CTS   | ch 2 fe
CTS   | ch 2 re
-------------------
TDC L | ch 0 re
-------------------
TDC Z | ch 0 re
      | ch Y re      = (T - AZ0R) + corr
      | ch Y fe
      | ...
-------------------
...
===================

corr = -(AC2R-AC0R)

Uncorrected full time in ns of the TIMESTAMP message calculated as
T = epoch*2048.*5. + (coarse)*5. - fine*0.005

Full corrected global time is then computed by adding the microslice
index from the descriptor to the corrected time:

fullTimeCorr = (T - AZ0R) - (AC2R-AC0R) + MSidx

*/

//TODO: Check that to 'real' actions are performed in the lines which are intended for debug output only.
//      i.e. LOG(XXXX) << ...

#include "CbmMcbm2018UnpackerAlgoRich.h"

// ROOT
#include <TList.h>
#include <TCanvas.h>
// FairRoot
#include <FairLogger.h>

// CbmRoot
#include "CbmMcbm2018RichPar.h"

CbmMcbm2018UnpackerAlgoRich::CbmMcbm2018UnpackerAlgoRich() :
	CbmStar2019Algo(),
	fbMonitorMode(kFALSE),
	fbDebugMonitorMode(kFALSE),
	fbDoToTCorr(kTRUE),
        fSkipMs(kFALSE),
	fdTimeOffsetNs( 0.0 ),
	fRICHcompIdx(6), //TODO experimentally obtained value
	fUnpackPar(nullptr),
	fTScounter(0),
	fCurMSid(0),
	fGwordCnt(0),
	fInSubSubEvent(kFALSE),
	fCurEpochCounter(0),
	fSubSubEvId(0),
	fLastCTSch0_re_time(0.),
	fLastCTSch2_re_time(0.),
	fLastCTSch2_fe_time(0.),
	fPrevLastCTSch0_re_time(0.),
	fPrevLastCTSch2_re_time(0.),
	fPrevLastCTSch2_fe_time(0.),/*,
	fhTDCch0re_minusCTSch0re(nullptr),
	fhTDCch0re_minusCTSch2re(nullptr),
	fhTDCch0re_minusCTSch2fe(nullptr),
	fhTDCch0re_minusPrevCTSch0re(nullptr),
	fhTDCch0re_minusPrevCTSch2re(nullptr),
	fhTDCch0re_minusPrevCTSch2fe(nullptr)*/
	fhTotMap(),
	fhTot2dMap()
{
	this->Init(); //TODO why this is not called by the framework?
}

CbmMcbm2018UnpackerAlgoRich::~CbmMcbm2018UnpackerAlgoRich()
{
}

Bool_t CbmMcbm2018UnpackerAlgoRich::Init()
{
	LOG(info) << "Initializing mCBM RICH 2019 unpacker algo";
	return kTRUE;
}

void CbmMcbm2018UnpackerAlgoRich::Reset()
{
}

void CbmMcbm2018UnpackerAlgoRich::Finish()
{
}

Bool_t CbmMcbm2018UnpackerAlgoRich::InitContainers()
{
	LOG(info) << "Init parameter containers for CbmMcbm2018UnpackerAlgoRich";
	Bool_t initOK = ReInitContainers();

	return initOK;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::ReInitContainers()
{
	LOG(info) << "ReInit parameter containers for CbmMcbm2018UnpackerAlgoRich";

	fUnpackPar = (CbmMcbm2018RichPar*)fParCList->FindObject("CbmMcbm2018RichPar");
	if (fUnpackPar == nullptr) {
		return kFALSE;
	}

	Bool_t initOK = InitParameters();

	return initOK;
}

TList* CbmMcbm2018UnpackerAlgoRich::GetParList()
{
	if (fParCList == nullptr) {
		fParCList = new TList();
	}
	fUnpackPar = new CbmMcbm2018RichPar("CbmMcbm2018RichPar");
	fParCList->Add(fUnpackPar);

	return fParCList;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::InitParameters()
{
	InitStorage();
	return kTRUE;
}

void CbmMcbm2018UnpackerAlgoRich::InitStorage()
{
	fLastCh0_re_time.Set(fUnpackPar->GetNaddresses()); // Set the size of the array
	fPrevLastCh0_re_time.Set(fUnpackPar->GetNaddresses()); // Set the size of the array
}

/**
  Copied from other detectors without any brain effort...
  A little bug-fix added
**/
void CbmMcbm2018UnpackerAlgoRich::AddMsComponentToList( size_t component, UShort_t usDetectorId )
{
   /// Check for duplicates and ignore if it is the case
   for( UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx ) {
      if( component == fvMsComponentsList[ uCompIdx ] ) {
         return;
      }
   }

   /// Add to list
   fvMsComponentsList.push_back( component );

   if (fvMsComponentsList.size() == 1) {
       fRICHcompIdx = component;
   } else {
   	LOG(WARN) << "fvMsComponentsList.size() > 1 for RICH. Unpacking may not work due to implementation limitations.";
   }

   LOG(info) << "CbmMcbm2018UnpackerAlgoRich::AddMsComponentToList => Component "
             << component << " with detector ID 0x"
             << std::hex << usDetectorId << std::dec << " added to list";
}

Bool_t CbmMcbm2018UnpackerAlgoRich::ProcessTs( const fles::Timeslice& /*ts*/ )
{
	LOG(debug2) << "CbmMcbm2018UnpackerAlgoRich::ProcessTs(ts)";
/*
	//TODO: shortcut. We love shortcuts, right?
	if (fvMsComponentsList.size() == 1) {
		this->ProcessTs(ts, fvMsComponentsList.at(0));
	}

	//TODO: implement case when multiple components have to be processed
*/
	return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::ProcessTs( const fles::Timeslice& ts, size_t component )
{
   /// Ignore First TS as first MS is typically corrupt
   if( 0 == ts.index() )
   {
      return kTRUE;
   } // if( 0 == ts.index() )

	LOG(debug2) << "CbmMcbm2018UnpackerAlgoRich::ProcessTs(ts, " << component << ")";

	//TODO: skip if this method was called for a wrong component
	//if (component != fRICHcompIdx) return kTRUE;
	//FIXME: this is really nasty...
//	component = fRICHcompIdx;
   if( 1 != fvMsComponentsList.size() )
   {
      /// If no RICH component, do nothing!
      if( 0 == fvMsComponentsList.size() )
         return kTRUE;

      /// If multiple RICH components, fail the run
      TString sCompList = "";
      for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )
         sCompList += Form( " %2lu ", fvMsComponentsList[ uMsCompIdx ] );
      LOG(fatal) << "CbmMcbm2018UnpackerAlgoRich::ProcessTs => More than 1 component in list, unpacking impossible! List is "
                 << sCompList;
   } // if( 1 != fvMsComponentsList.size() )
   component = fvMsComponentsList[ 0 ];

	LOG(debug) << "Components:  " << ts.num_components();
	LOG(debug) << "Microslices: " << ts.num_microslices(component);

	const uint64_t compSize = ts.size_component(component);
	LOG(debug) << "Component " << component << " has size " << compSize;

   /// On first TS, extract the TS parameters from header (by definition stable over time)
   if( -1.0 == fdTsCoreSizeInNs )
   {
      fuNbCoreMsPerTs = ts.num_core_microslices();
      fuNbOverMsPerTs = ts.num_microslices( 0 ) - ts.num_core_microslices();
      fdTsCoreSizeInNs = fdMsSizeInNs * ( fuNbCoreMsPerTs );
      fdTsFullSizeInNs = fdMsSizeInNs * ( fuNbCoreMsPerTs + fuNbOverMsPerTs );
      LOG(info) << "Timeslice parameters: each TS has "
                << fuNbCoreMsPerTs << " Core MS and "
                << fuNbOverMsPerTs << " Overlap MS, for a core duration of "
                << fdTsCoreSizeInNs << " ns and a full duration of "
                << fdTsFullSizeInNs << " ns";

      /// Ignore overlap ms if flag set by user
      fuNbMsLoop = fuNbCoreMsPerTs;
      if( kFALSE == fbIgnoreOverlapMs )
         fuNbMsLoop += fuNbOverMsPerTs;
      LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
   } // if( -1.0 == fdTsCoreSizeInNs )

	for (size_t iMS = 0; iMS < fuNbMsLoop; ++iMS)
	{
		fCurMSid = iMS;
		LOG(debug) << "=======================================================";
		const fles::MicrosliceView mv = ts.get_microslice(component, iMS);
		const fles::MicrosliceDescriptor& msDesc = mv.desc();
		////const uint8_t* msContent = mv.content();
		LOG(debug) << "msDesc.size=" << msDesc.size;
		fCurMSidx = msDesc.idx;
		LOG(debug) << "msDesc.idx=" << msDesc.idx;
		////mRichSupport::PrintRaw(msDesc.size, msContent);//TODO delete
		////LOG(debug) << "=======================================================";
		////////////////////////////////
		ProcessMs(ts, component, iMS);//
		////////////////////////////////
		LOG(debug) << "=======================================================";
	}

	///////////////
	FinalizeTs();//
	///////////////

	if( 0 == fTScounter % 1000 ) {
		LOG(info) << "Processed " << fTScounter << " TS";
	}

	fTScounter++;

	/// Sort the buffers of hits due to the time offsets applied
	std::sort(fDigiVect.begin(), fDigiVect.end(),
		[](const CbmRichDigi & a, const CbmRichDigi & b) -> bool
		{
			return a.GetTime() < b.GetTime();
		});

   fhVectorSize->Fill( ts.index(), fDigiVect.size() );
   fhVectorCapacity->Fill( ts.index(), fDigiVect.capacity() );

   if( fuTsMaxVectorSize < fDigiVect.size() )
   {
      fuTsMaxVectorSize = fDigiVect.size() * fdCapacityIncFactor;
      fDigiVect.shrink_to_fit();
      fDigiVect.reserve( fuTsMaxVectorSize );
   } // if( fuTsMaxVectorSize < fDigiVect.size() )

	return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::ProcessMs( const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx )
{
	const fles::MicrosliceView mv = ts.get_microslice(uMsCompIdx, uMsIdx);
	const fles::MicrosliceDescriptor& msDesc = mv.desc();
	const uint8_t* ptr = mv.content();
	const size_t size = msDesc.size;

	if (size == 0) return kTRUE;

	fGwordCnt = 0; //TODO check that this global word counter works properly

	Int_t offset; // offset in bytes
	Int_t* dataPtr;

	offset = 0; mRichSupport::SwapBytes(4, ptr+offset);
	LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
	            << "Reserved 0000 0000";
	fGwordCnt++;

	offset = 4; mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t mbsNumber = (Int_t)(dataPtr[0] & 0xffffff);
	LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
	            << "mbsNumber = " << mbsNumber;
	fGwordCnt++;

	// We suppose that the first word is
	// "HadesTransportUnitQueue - Length"
	offset = 0+8; mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t TRBeventSize1 = (Int_t)(dataPtr[0]);
	LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
	            << "HadesTransportUnitQueue - Length = " << TRBeventSize1;
	fGwordCnt++;

	if (*dataPtr > 0 && UInt_t(*dataPtr) == 0x80030000) {
		LOG(info) << "dataPtr == 0x80030000";
		exit(EXIT_FAILURE);
	}

	// We suppose that the second word is
	// "HadesTransportUnitQueue - Decoder  (Seems to be allways the same)"
	offset = 4+8; mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t dcdr = (Int_t)(dataPtr[0]);
	if (dcdr == 0x00030062) {
		LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "HadesTransportUnitQueue - Decoder = " << dcdr;
		fGwordCnt++;
	} else {
		LOG(warning) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "er" << "\t"
		             << "HadesTransportUnitQueue - Decoder = " << dcdr
		             << " is not 0x00030062 (196706) => 0x" << std::hex << dcdr << std::dec;
		fGwordCnt++;

                /// Probably corrupted MS, stop there and skip remaining data
		fSkipMs = kTRUE;
		return kFALSE;
	}

	// We suppose that the third word is
	// TRB event length (in bytes)
	// It should be 8 less than the size specified two words ago
	offset = 8+8; mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t TRBeventSize2 = (Int_t)(dataPtr[0]);
	if (TRBeventSize2 == TRBeventSize1-8) {
		LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "TRB event - Length = " << TRBeventSize2
		            << " == " << TRBeventSize1 << "-8";
		fGwordCnt++;
	} else {
		LOG(debug) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "er" << "\t"
		           << "TRB event - Length = " << TRBeventSize2
		           << " != " << TRBeventSize1 << "-8=" << TRBeventSize1-8;
		fGwordCnt++;
	}

	/////////////////////////////////////////////
	ProcessTRBevent(TRBeventSize2, ptr+offset);//
	/////////////////////////////////////////////

	if (fSkipMs == kTRUE) {
	  // problem in data; delete vectors.
	  fDigiVect.clear();
	  fSkipMs = kFALSE;
	}


	return kTRUE;
}

Int_t CbmMcbm2018UnpackerAlgoRich::ProcessTRBevent(size_t const size, uint8_t const * const ptr)
{
	Int_t offset; // offset in bytes
	Int_t* dataPtr;

	// We assume that the TRB event header is 4 words and
	// the first word is already processed outside of this method

	//////////////////////////////////
	ProcessTRBeventHeader(4*4, ptr);//
	//////////////////////////////////

	offset = 16; // start from after the TRB event header

	// 1. Search for the CTS subevent and extract reference time

	while (static_cast<size_t>(offset) < size) {
		/// Escape bad MS before doing anything
		if (fSkipMs == kTRUE) break;

		// Duplicate the header word in order to avoid corrupting (by bytes swapping)
		// the original data
		dataPtr = (Int_t*)(ptr+offset);
		Int_t headerCopy = *dataPtr;
		mRichSupport::SwapBytes(4, (uint8_t*)&headerCopy);
		dataPtr = &headerCopy;

                Int_t SubEvSize = (Int_t)((dataPtr[0] >> 16) & 0xffff);
                Int_t HubId = (Int_t)((dataPtr[0]) & 0xffff);

		// Process only CTS subevent
		//FIXME change from 0xc001 to 0xc000 at some point // ?
		if ((HubId == 0xc001) || (HubId == 0xc000)) {

			// Not a very nice shortcut
			// The global counter of the words is incremented for the CTS subevent header here
			// However for the TRB subevent headers it is incremented in the second run,
			// where only TRB subevent headers are processed and the CTS subevents are skipped
			LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr((uint8_t*)&headerCopy) << "\t" << "ok" << "\t"
			            << "hub ID = 0x" << mRichSupport::GetHexRepresentation(2, (uint8_t*)&headerCopy) << "\t"
			            << "subevent size = " << SubEvSize;
			fGwordCnt++;

			fSubSubEvId = HubId;
			//////////////////////////////////////////////////////////////
			offset += (4 + ProcessCTSsubevent(SubEvSize*4, ptr+offset));//
			//////////////////////////////////////////////////////////////
			// In principle, should be reset here for safety
			fSubSubEvId = 0;
		} else {
			// Skip all other subevents
			offset += (4 + SubEvSize*4);
		}
	}

	offset = 16; // start from after the TRB event header again

	// 2. Process TRB subsubevents

	//Int_t iIter = 0;
	while (static_cast<size_t>(offset) < size) {
		/// Escape bad MS before doing anything
		if (fSkipMs == kTRUE) break;

		//std::cout << "SE iteration " << iIter++ << "\toffset=" << offset << "\tsize=" << size << std::endl;

		// We suppose that the fifth word is the header of the subevent
		// <Length> <HubId>
		mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
		Int_t SubEvSize = (Int_t)((dataPtr[0] >> 16) & 0xffff);
		Int_t HubId = (Int_t)((dataPtr[0]) & 0xffff);

		//FIXME change from 0xc001 to 0xc000 at some point // ?
		if ((HubId == 0xc001) || (HubId == 0xc000)) {
			////fSubSubEvId = HubId;
			//////////////////////////////////////////////////////////////////
			////offset += (4 + ProcessCTSsubevent(SubEvSize*4, ptr+offset));//
			//////////////////////////////////////////////////////////////////
			////// In principle, should be reset here for safety
			////fSubSubEvId = 0;

			// Skip CTS subevent as it has been already processed during the first run
			offset += (4 + SubEvSize*4);
		} else if (HubId == 0x5555) {
			LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
			            << "hub ID = 0x" << mRichSupport::GetHexRepresentation(2, ptr+offset) << "\t"
			            << "subevent size = " << SubEvSize;
			fGwordCnt++;
			//TODO one could implement additional checks here about the
			// words coming after the "event end" but we skip everything by now.
			///////////////////////////////////////////////////////////////
			offset += (4 + ProcessSKIPsubevent(SubEvSize*4, ptr+offset));//
			///////////////////////////////////////////////////////////////
		} else {
			LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
			            << "hub ID = 0x" << mRichSupport::GetHexRepresentation(2, ptr+offset) << "\t"
			            << "subevent size = " << SubEvSize;
			fGwordCnt++;
			//////////////////////////////////////////////////////////////
			offset += (4 + ProcessTRBsubevent(SubEvSize*4, ptr+offset));//
			//////////////////////////////////////////////////////////////
		}
	}

	////LOG(debug4) <<  "Done processing TRB event. offset=" << offset << "\tsize=" << size;
	//TODO implement checks
	if (size !=static_cast<size_t>(offset)) {
		LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich::ProcessTRBevent() warning:"
		          << "Number of processed bytes is not equal to the expected size. This should not happen. ("
                          << size << " VS " << offset << ")";
	}

	return size; //TODO check
}

// Process TRB event header.
// Input arguments are the size of the TRB event header (16 bytes) and the pointer to the first word.
// Note that the first word can already be analysed outside of this method.
// Return number of bytes processed. For this particular method the value of the input 'size' argument
// is returned as we expect that the TRB header is always 16 bytes.
Int_t CbmMcbm2018UnpackerAlgoRich::ProcessTRBeventHeader(size_t const size, uint8_t const * const ptr)
{
	Int_t offset; // offset in bytes
	Int_t* dataPtr;

	// Skip first word (already processed outside)
	//offset = 0;
	// do nothing

	// We suppose that the second word consists of
	// 0002 - number of following word till the Event Data Starts (should be the same)
	// 00<TriggerType>1 - value in [7:4] defines TriggerType
	offset = 4; mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t checkSize = (Int_t)((dataPtr[0] >> 16) & 0xffff);
	Int_t triggerType = (Int_t)((dataPtr[0] >> 4) & 0xf);
	if (checkSize == 2) {
		LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "checkSize == 2" << "\t"
		            << "trigger type = " << triggerType;
		fGwordCnt++;
	} else {
		LOG(warning) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "er" << "\t"
		             << "checkSize != 2 (" << checkSize << ")\t"
		             << "trigger type = " << triggerType;
		fGwordCnt++;

                /// Probably corrupted MS, stop there and skip remaining data
		fSkipMs = kTRUE;
		return 0;
	}

	/*for (size_t iWord=2; iWord<size; iWord++) {
		offset = iWord*4;
		LOG(debug4) << "\t" << GetWordHexRepr(ptr+offset);
	}*/

	// We suppose that the third word consists of
	// 0000 <SubEventId>
	offset = 8; mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	Int_t checkBytes = (Int_t)((dataPtr[0] >> 16) & 0xffff);
//	Int_t SubEvId = (Int_t)((dataPtr[0]) & 0xffff);
	if (checkBytes == 0) {
		LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "checkBytes == 0" << "\t"
		            << "subevent ID = 0x" << mRichSupport::GetHexRepresentation(2, ptr+offset);
		fGwordCnt++;
	} else {
		LOG(warning) << "[" << fGwordCnt++ << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "er" << "\t"
		             << "checkBytes != 0 (" << checkBytes << ")\t"
		             << "subevent ID = 0x" << mRichSupport::GetHexRepresentation(2, ptr+offset);
		fGwordCnt++;

                /// Probably corrupted MS, stop there and skip remaining data
		fSkipMs = kTRUE;
		return 0;
	}

	// We suppose that the fourth word is the trigger number
	offset = 12; mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	UInt_t TriggerNum = (UInt_t)(dataPtr[0]);
	LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
	            << "trigger num = " << TriggerNum;
	fGwordCnt++;

	return size;
}

Int_t CbmMcbm2018UnpackerAlgoRich::ProcessSKIPsubevent(size_t const size, uint8_t const * const ptr)
{
	////LOG(debug4) << "ProcessSKIPsubevent size=" << size << " bytes";

	Int_t offset; // offset in bytes
//	Int_t* dataPtr; (FU) not used

	// Skip first word (already processed outside)
	offset = 4;

	while (static_cast<size_t>(offset) < size+4) {
		mRichSupport::SwapBytes(4, ptr+offset);
//                dataPtr = (Int_t*)(ptr+offset); (FU) not used
		LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset);
		fGwordCnt++;

		offset += 4;
	}

	////LOG(debug4) << "Done processing SKIP subevent. offset=" << offset << "\tsize=" << size;
	//TODO implement checks
	if (size != static_cast<size_t>(offset-4)) {
		LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich::ProcessSKIPsubevent() warning:"
		          << "Number of processed bytes is not equal to the expected size. This should not happen.";
	}

	return size; //TODO check
}

Int_t CbmMcbm2018UnpackerAlgoRich::ProcessCTSsubevent(size_t const size, uint8_t const * const ptr)
{
	////LOG(debug4) << "ProcessCTSsubevent size=" << size << " bytes";

	Int_t offset; // offset in bytes
	Int_t* dataPtr;

	// Skip first word (already processed outside)

	// We suppose that the second word is the header of the subsubevent
	offset = 4; mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
	LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
	            << "CTS header";
	fGwordCnt++;

/*      (FU) not used
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
		mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
		ULong_t MSidx = 102400UL * ((ULong_t)(*dataPtr) - 1);
		LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "CTS information" << " MSidx=" << MSidx;
		fGwordCnt++;

		offset += 4;
	}

	// size - full size including CTS header word, CTS informations words (CTSinfo_size) and TCD data
	// Thus TDC data size = full size - 1 word (header) - CTSinfo_size words (CTS informations)
	///////////////////////////////////////////////////////////////////////////
	offset += (ProcessTRBsubsubevent((size-(1+CTSinfo_size)*4), ptr+offset));//
	///////////////////////////////////////////////////////////////////////////

	////LOG(debug4) << "Done processing CTS subevent. offset-4=" << offset-4 << "\tsize=" << size;
	//TODO implement checks
	if (size != static_cast<size_t>(offset-4)) {
		LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich::ProcessCTSsubevent() warning:"
		          << "Number of processed bytes is not equal to the expected size. This should not happen.";
	}

	return size; //TODO check
}

Int_t CbmMcbm2018UnpackerAlgoRich::ProcessTRBsubevent(size_t const size, uint8_t const * const ptr)
{
	////LOG(debug4) << "ProcessTRBsubevent size=" << size << " bytes";

	Int_t offset; // offset in bytes
	Int_t* dataPtr;

	// Skip first word (already processed outside)
	offset = 4;

	//Int_t iIter = 0;
	while (static_cast<size_t>(offset) < size) {
		if (fSkipMs == kTRUE) break;
		//std::cout << "SSE iteration " << iIter++ << "\toffset=" << offset << "\tsize=" << size << std::endl;

		// We suppose that the second word is the header of the subsubevent
		// <Length> <SubSubEv.Id>
		mRichSupport::SwapBytes(4, ptr+offset); dataPtr = (Int_t*)(ptr+offset);
		Int_t SubSubEvSize = (Int_t)((dataPtr[0] >> 16) & 0xffff);
		fSubSubEvId = (Int_t)((dataPtr[0]) & 0xffff);
		LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr+offset) << "\t" << "ok" << "\t"
		            << "subsubevent ID (FPGA ID) = 0x" << mRichSupport::GetHexRepresentation(2, ptr+offset) << "\t"
		            << "subsubevent size = " << SubSubEvSize;
		fGwordCnt++;

      if( size + 4 < static_cast<size_t>( offset + 4 + SubSubEvSize*4 ) ) {
         LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich::ProcessTRBsubevent() warning:"
                   << "SubEvent out of bounds. This should not happen. ("
                             << size << " VS " << ( offset + 4 + SubSubEvSize*4 ) << ")";
         /// Probably corrupted MS, stop there and skip remaining data
         fSkipMs = kTRUE;
      }

		// Add 4 bytes which correspond to the header word
		//////////////////////////////////////////////////////////////////////
		offset += (4 + ProcessTRBsubsubevent(SubSubEvSize*4, ptr+offset+4));//
		//////////////////////////////////////////////////////////////////////

		// In principle, should be reset here for safety
		fSubSubEvId = 0;
	}

	////LOG(debug4) << "Done processing TRB subevent. offset-4=" << offset-4 << "\tsize=" << size;
	//TODO implement checks
	if (size != static_cast<size_t>(offset-4)) {
		LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich::ProcessTRBsubevent() warning:"
		          << "Number of processed bytes is not equal to the expected size. This should not happen. ("
                          << size << " VS " << (offset - 4) << ")";
      /// Probably corrupted MS, stop there and skip remaining data
		fSkipMs = kTRUE;
	}

	return size; //TODO check
}

Int_t CbmMcbm2018UnpackerAlgoRich::ProcessTRBsubsubevent(size_t const size, uint8_t const * const ptr)
{
	////LOG(debug4) << "ProcessTRBsubsubevent size=" << size  << " bytes";

	Int_t offset = 0; // offset in bytes

	fCurEpochCounter = 0; //TODO check
	fInSubSubEvent = kFALSE; //TODO check

	for (size_t iWord=0; iWord<size/4; iWord++) {
		if (fSkipMs == kTRUE) break;
		mRichSupport::SwapBytes(4, ptr+iWord*4);
		//////////////////////////////
		ProcessTDCword(ptr+iWord*4);//
		//////////////////////////////
		offset += 4;
	}
        if (fSkipMs == kTRUE) return 0;
	////LOG(debug4) << "Done processing TRB subsubevent. offset=" << offset << "\tsize=" << size;
	//TODO Implement checks that the first word was the header and the last word was the trailer
	if (size != static_cast<size_t>(offset)) {
		LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich::ProcessTRBsubsubevent() warning:"
		          << "Number of processed bytes is not equal to the expected size. This should not happen.";
      /// Probably corrupted MS, stop there and skip remaining data
		fSkipMs = kTRUE;
	}

	return size; //TODO check
}

void CbmMcbm2018UnpackerAlgoRich::ProcessTDCword(uint8_t const * const ptr)
{
	Int_t* tdcDataPtr = (Int_t*)ptr;
	Int_t tdcData = tdcDataPtr[0];
	Int_t tdcTimeDataMarker = (tdcData >> 31) & 0x1; // 1 bit

	if (tdcTimeDataMarker == 0x1) {
		////////////////////////////////
		ProcessTimestampWord(tdcData);//
		////////////////////////////////
	} else {
		UInt_t tdcMarker = (tdcData >> 29) & 0x7; // 3 bits

		if (tdcMarker == 0x0) { // TDC trailer
			if (fInSubSubEvent) {
				fInSubSubEvent = kFALSE; // go out of InSubSubEvent state
				LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "TDC TRAILER";
				fGwordCnt++;
			} else {
				LOG(info) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t" << "er" << "\t" << "UNKNOWN (TDC TRAILER not after header)";
				fGwordCnt++;
				fSkipMs = kTRUE;
				//exit(EXIT_FAILURE); //TODO probably one should get rid of explicit EXIT calls not to ruin unpacking of other detectors?
			}
		} else if (tdcMarker == 0x1) { // TDC header
		//	UInt_t randomCode = (tdcData >> 16) & 0xff; // 8 bits
		//	UInt_t errorBits = (tdcData) & 0xffff; //16 bits
			if (!fInSubSubEvent) {
				fInSubSubEvent = kTRUE; // go into InSubSubEvent state
				LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "TDC HEADER";
				fGwordCnt++;
			} else {
				LOG(info) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t" << "er" << "\t" << "UNKNOWN (TDC HEADER not after trailer)";
				fGwordCnt++;
				fSkipMs = kTRUE;
				//exit(EXIT_FAILURE); //TODO probably one should get rid of explicit EXIT calls not to ruin unpacking of other detectors?
			}
		} else if (tdcMarker == 0x2) { // DEBUG
		//	UInt_t debugMode = (tdcData >> 24) & 0x1f; // 5 bits
		//	UInt_t debugBits = (tdcData) & 0xffffff; // 24 bits
			LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "DEBUG";
			fGwordCnt++;
			// currently no actions if a DEBUG message is encountered.
		} else if (tdcMarker == 0x3) { // EPOCH counter
			fCurEpochCounter = (tdcData) & 0xfffffff; // 28 bits
			LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t" << "ok" << "\t" << "EPOCH\t" << fCurEpochCounter;
			fGwordCnt++;
		} else {
			if (tdcTimeDataMarker != 0x1) {
				LOG(error) << "[" << fGwordCnt++ << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t" << "er" << "\t" << "UNKNOWN";
			}
		}
	}
}

void CbmMcbm2018UnpackerAlgoRich::ProcessTimestampWord(Int_t tdcData)
{
	Int_t channel = (tdcData >> 22) & 0x7f; // 7 bits
	Int_t fine = (tdcData >> 12) & 0x3ff; // 10 bits
	Int_t edge = (tdcData >> 11) & 0x1; // 1 bit
	Int_t coarse = (tdcData) & 0x7ff; // 11 bits
	Int_t epoch = fCurEpochCounter;

	//TODO move full time calculation outside
	Double_t fullTime = (Double_t)epoch*2048.*5. + (Double_t)(coarse)*5. - (Double_t)(fine)*0.005;

	LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr((uint8_t*)&tdcData) << "\t" << "ok" << "\t" << "TIMESTAMP" << "\t"
	            << "ch=" << channel << "\t"
	            << "edge=" << edge << "\t"
	            << "epoch=" << epoch << "\t"
	            << "coarse=" << coarse << "\t"
	            << "fine=" << fine << "\t"
	            << "full=" << fullTime;
	fGwordCnt++;

	// Storing reference times
	// =======================

	////LOG(debug4) << "fSubSubEvId=0x" << std::hex << fSubSubEvId << std::dec;
	Int_t idx = fUnpackPar->GetAddressIdx(fSubSubEvId);
	if( -1 == idx ) {
                /// Probably corrupted MS, stop there and skip remaining data
		fSkipMs = kTRUE;
		return;
	}
	////LOG(debug4) << "fSubSubEvId=0x" << std::hex << fSubSubEvId << std::dec << " idx=" << idx;

	if ((fSubSubEvId == 0xc000) || (fSubSubEvId == 0xc001)) {
		// if CTS
		if ((channel == 0) && (edge == RISINGEDGEID)) {
			fPrevLastCTSch0_re_time = fLastCTSch0_re_time;
			fLastCTSch0_re_time = fullTime;
			////LOG(debug4) << "Storing full time for the CTS ch=0 edge=RISINGEDGEID";
		} else if ((channel == 2) && (edge == RISINGEDGEID)) {
			fPrevLastCTSch2_re_time = fLastCTSch2_re_time;
			fLastCTSch2_re_time = fullTime;
			////LOG(debug4) << "Storing full time for the CTS ch=2 edge=RISINGEDGEID";
		} else if ((channel == 2) && (edge == FALLINGEDGEID)) {
			fPrevLastCTSch2_fe_time = fLastCTSch2_fe_time;
			fLastCTSch2_fe_time = fullTime;
			////LOG(debug4) << "Storing full time for the CTS ch=2 edge=FALLINGEDGEID";
		}
	} else {
		// if not CTS (which means TRB)
		if ((channel == 0) && (edge == RISINGEDGEID)) {
			fPrevLastCh0_re_time[idx] = fLastCh0_re_time[idx];
			fLastCh0_re_time[idx] = fullTime;
			////LOG(debug4) << "Storing full time for TDC 0x" << std::hex << fSubSubEvId << std::dec << " ch=0 edge=RISINGEDGEID";
		}
	}

	// Calculation of corrected time
	// =============================
	Double_t fullTimeCorr = 0.;
	if (!((fSubSubEvId == 0xc000) || (fSubSubEvId == 0xc001))) {
		if (channel != 0) {
			Double_t dT = fullTime - fPrevLastCh0_re_time[idx];
			Double_t corr = fPrevLastCTSch2_re_time - fPrevLastCTSch0_re_time;
			fullTimeCorr = dT - corr;
		}
	}

	// Filling histograms
	// ==================
	if (fbMonitorMode == kTRUE)
	{
		if (!((fSubSubEvId == 0xc000) || (fSubSubEvId == 0xc001))) {
			// if not CTS (which means TRB)
			if ((channel == 0) && (edge == RISINGEDGEID)) {
/*				Double_t dT1 = fullTime - fLastCTSch0_re_time;
				Double_t dT2 = fullTime - fLastCTSch2_re_time;
				Double_t dT3 = fullTime - fLastCTSch2_fe_time;
				fhTDCch0re_minusCTSch0re->Fill(idx, dT1);
				fhTDCch0re_minusCTSch2re->Fill(idx, dT2);
				fhTDCch0re_minusCTSch2fe->Fill(idx, dT3);

				Double_t dT4 = fullTime - fPrevLastCTSch0_re_time;
				Double_t dT5 = fullTime - fPrevLastCTSch2_re_time;
				Double_t dT6 = fullTime - fPrevLastCTSch2_fe_time;
				fhTDCch0re_minusPrevCTSch0re->Fill(idx, dT4);
				fhTDCch0re_minusPrevCTSch2re->Fill(idx, dT5);
				fhTDCch0re_minusPrevCTSch2fe->Fill(idx, dT6);

				LOG(debug4) << "dT1=" << dT1 << "\tdT2=" << dT2 << "\tdT3=" << dT3
				            << "\tdT4=" << dT4 << "\tdT5=" << dT5 << "\tdT6=" << dT6;
*/
			}

			if ((channel != 0) && (edge == RISINGEDGEID)) {
				/*Double_t dT7 = fullTime - fLastCh0_re_time[idx];
				TH2D* h1 = fhTDCre_minusTDCch0re.at(idx);
				h1->Fill(channel, dT7);*/

				Double_t dT8 = fullTime - fPrevLastCh0_re_time[idx];
				/*TH2D* h2 = fhTDCre_minusPrevTDCch0re.at(idx);
				h2->Fill(channel, dT8);*/

				Double_t corr1 = fPrevLastCTSch2_re_time - fPrevLastCTSch0_re_time;
				Double_t correctedT1 = dT8 + corr1;
				Double_t correctedT2 = dT8 - corr1;
/*
//				TH2D* h3 = fhTDCre_corrected1.at(idx);
//				h3->Fill(channel, correctedT1);
//				TH2D* h4 = fhTDCre_corrected2.at(idx);
//				h4->Fill(channel, correctedT2);
*/
				LOG(debug4) /*<< "dT7=" << dT7*/ << "\tdT8=" << dT8
				            << "\tcorr1=" << corr1
				            << "\tcorrectedT1=" << correctedT1
				            << "\tcorrectedT2=" << correctedT2;
			}
		}
	}

	if (edge == RISINGEDGEID) {
		this->ProcessRisingEdge(fSubSubEvId, channel, fullTimeCorr);
	} else {
		this->ProcessFallingEdge(fSubSubEvId, channel, fullTimeCorr);
	}
}

void CbmMcbm2018UnpackerAlgoRich::ProcessRisingEdge(Int_t subSubEvId, Int_t channel, Double_t time)
{
	////LOG(debug4) << "CbmMcbm2018UnpackerAlgoRich::ProcessRisingEdge()";

	//TODO: not a very nice hack.
	// All messages from ch0 are skipped. Though, probably, that is corect.
	if (channel == 0) return;

	// Also skip everything from CST
	if ((subSubEvId == 0xc000) || (subSubEvId == 0xc001)) return;

	fRisingEdgesBuf.push_back( CbmMcbmRichEdge(subSubEvId, channel, time) );
}

void CbmMcbm2018UnpackerAlgoRich::ProcessFallingEdge(Int_t subSubEvId, Int_t channel, Double_t time)
{
	////LOG(debug4) << "CbmMcbm2018UnpackerAlgoRich::ProcessFallingEdge()";

	// Skip everything from CST
	if ((subSubEvId == 0xc000) || (subSubEvId == 0xc001)) return;

	Bool_t reFound = kFALSE;

	std::vector<CbmMcbmRichEdge>::iterator reIter = fRisingEdgesBuf.begin();

	while (reIter != fRisingEdgesBuf.end()) {
		if (((*reIter).fSubSubEventID == subSubEvId) && ((*reIter).fChannel == channel)) {
			Double_t reTime = (*reIter).fTime;
			// Found corresponding rising edge
			Double_t ToT      = time - reTime;

			if ((ToT >= TOTMIN) && (ToT <= TOTMAX)) {
				// Time-over-threshold is within allowed range

				reFound = kTRUE;

				LOG(debug4) << "Found pair for FPGA ID 0x"
				            << std::hex << subSubEvId << std::dec
				            << "\tch=" << channel
				            << "\tToT=" << ToT;

				//TODO implement
				// Writing output digi
				//////////////////////////////////////////////////
				if (fbMonitorMode) {
					TH1D* h = GetTotH1(subSubEvId, channel);
					if (h != nullptr) h->Fill(ToT);

					TH2D* h2 = GetTotH2(subSubEvId);
					if (h2 != nullptr) h2->Fill(channel, ToT);
				}
				WriteOutputDigi(subSubEvId, channel, reTime, ToT, fCurMSidx);//
				//////////////////////////////////////////////////

				reIter = fRisingEdgesBuf.erase(reIter);
				continue; // Take care. This has to be the last operation in this block

			} else {
				//TODO: exception. By now we can just do nothing
			}
		} // end of if condition

		// This construction is a little bit tricky.
		// The iterator is either incremented here or (if a pair was found)
		// incremented using erase call, followed by the continue.
		++reIter;
	} // end of for loop

	if (reFound == kFALSE) {
		// Corresponding rising edge not found - store the falling edge in the bufer
		fFallingEdgesBuf.push_back( CbmMcbmRichEdge(subSubEvId, channel, time) );
	}

}

void CbmMcbm2018UnpackerAlgoRich::WriteOutputDigi(Int_t fpgaID, Int_t channel, Double_t time, Double_t tot, uint64_t MSidx)
{
	Double_t ToTcorr = fbDoToTCorr ? fUnpackPar->GetToTshift(fpgaID, channel) : 0.;
	Int_t pixelUID = this->GetPixelUID(fpgaID, channel);
        //check ordering
        Double_t finalTime = time+(Double_t)MSidx - fdTimeOffsetNs;

        Double_t lastTime  = 0.;

        if (fDigiVect.size() < 1) {
            fDigiVect.emplace_back(pixelUID, finalTime, tot - ToTcorr);
        } else {
                lastTime = fDigiVect[fDigiVect.size()-1].GetTime();
                if (lastTime > finalTime)  {
                        for (int i = fDigiVect.size()-1; i >=0;i--) {
                                lastTime = fDigiVect[i].GetTime();
                                if (lastTime <= finalTime) {
                                       // LOG(info) << " before:"<< fDigiVect.size();
                                        fDigiVect.emplace(fDigiVect.begin()+i+1,pixelUID, finalTime, tot - ToTcorr);
                                       // LOG(info) << fDigiVect.size();
                                        break;
                                }
                        }
                } else {
                    fDigiVect.emplace_back(pixelUID, finalTime, tot - ToTcorr);
                }
        }
	LOG(debug4) << "CbmMcbm2018UnpackerAlgoRich::WriteOutputDigi fDigiVect.size="
	            << fDigiVect.size();
}

void CbmMcbm2018UnpackerAlgoRich::FinalizeTs()
{
//          for (int i = 0; i < fDigiVect.size();++i) {
//                 LOG(info) << "CbmMcbm2018UnpackerAlgoRich::Final Vector: "
//                     << i+1 <<"/"<<fDigiVect.size()
// 	            << "\t" << std::setprecision(15)<< fDigiVect[i].GetTime();
//
//
//         }
	LOG(debug4) << "CbmMcbm2018UnpackerAlgoRich::FinalizeTs: "
	            << fRisingEdgesBuf.size() << " entries in fRisingEdgesBuf"
	            << "\t" << fFallingEdgesBuf.size() << " entries in fFallingEdgesBuf";

	// Clear rising edges buffer
	LOG(debug4) << "Rising edges: ----------------------------------------------------------";
	std::vector<CbmMcbmRichEdge>::iterator reIter = fRisingEdgesBuf.begin();
	while (reIter != fRisingEdgesBuf.end()) {
		LOG(debug4) << "FPGA=0x" << std::hex << (*reIter).fSubSubEventID << std::dec
		            << "\tch=" << (*reIter).fChannel;
		++reIter;
	}
	fRisingEdgesBuf.clear();

	// Clear falling edges buffer
	LOG(debug4) << "Falling edges: ---------------------------------------------------------";
	std::vector<CbmMcbmRichEdge>::iterator feIter = fFallingEdgesBuf.begin();
	while (feIter != fFallingEdgesBuf.end()) {
		LOG(debug4) << "FPGA=0x" << std::hex << (*feIter).fSubSubEventID << std::dec
		            << "\tch=" << (*feIter).fChannel;
		++feIter;
	}
	fFallingEdgesBuf.clear();

	LOG(debug4) << "---------------------------------------------------------";
}

Bool_t CbmMcbm2018UnpackerAlgoRich::CreateHistograms()
{
	Int_t nTDCs = fUnpackPar->GetNaddresses();
//	std::vector<TCanvas*> fcToT2d;
/*
	fhTDCch0re_minusCTSch0re = new TH2D("fhTDCch0re_minusCTSch0re", "TDC ch0 re - CTS ch0 re;TDC index;ns", nTDCs, 0, nTDCs, 1000, -500., 500.);
	fhTDCch0re_minusCTSch2re = new TH2D("fhTDCch0re_minusCTSch2re", "TDC ch0 re - CTS ch2 re;TDC index;ns", nTDCs, 0, nTDCs, 1000, -500., 500.);
	fhTDCch0re_minusCTSch2fe = new TH2D("fhTDCch0re_minusCTSch2fe", "TDC ch0 re - CTS ch2 fe;TDC index;ns", nTDCs, 0, nTDCs, 1000, -500., 500.);

	AddHistoToVector(fhTDCch0re_minusCTSch0re, "");
	AddHistoToVector(fhTDCch0re_minusCTSch2re, "");
	AddHistoToVector(fhTDCch0re_minusCTSch2fe, "");

	fhTDCch0re_minusPrevCTSch0re = new TH2D("fhTDCch0re_minusPrevCTSch0re", "TDC ch0 re - prev CTS ch0 re;TDC index;ns", nTDCs, 0, nTDCs, 1000, -500., 500.);
	fhTDCch0re_minusPrevCTSch2re = new TH2D("fhTDCch0re_minusPrevCTSch2re", "TDC ch0 re - prev CTS ch2 re;TDC index;ns", nTDCs, 0, nTDCs, 1000, -500., 500.);
	fhTDCch0re_minusPrevCTSch2fe = new TH2D("fhTDCch0re_minusPrevCTSch2fe", "TDC ch0 re - prev CTS ch2 fe;TDC index;ns", nTDCs, 0, nTDCs, 1000, -500., 500.);

	AddHistoToVector(fhTDCch0re_minusPrevCTSch0re, "");
	AddHistoToVector(fhTDCch0re_minusPrevCTSch2re, "");
	AddHistoToVector(fhTDCch0re_minusPrevCTSch2fe, "");
*/
	for (Int_t iTDC=0; iTDC<nTDCs; iTDC++) {
		TString histoName;
		TString histoTitle;
		TString subFolder;
/*
		histoName.Form("fhTDC%dre_minusTDC%dch0re", iTDC, iTDC);
		histoTitle.Form("TDC %d re - TDC %d ch0 re;channel;ns", iTDC, iTDC);
		TH2D* h1 = new TH2D(histoName, histoTitle, 32, 0., 32., 1200, 0., 600.);
		fhTDCre_minusTDCch0re.push_back(h1);
		AddHistoToVector(h1);

		histoName.Form("fhTDC%dre_minusPrevTDC%dch0re", iTDC, iTDC);
		histoTitle.Form("TDC %d re - prev. TDC %d ch0 re;channel;ns", iTDC, iTDC);
		TH2D* h2 = new TH2D(histoName, histoTitle, 32, 0., 32., 1200, 0., 600.);
		fhTDCre_minusPrevTDCch0re.push_back(h2);
		AddHistoToVector(h2);
*/
/*		histoName.Form("fhTDC%dre_corrected1", iTDC);
		histoTitle.Form("TDC %d re corrected1;channel;ns", iTDC);
		TH2D* h3 = new TH2D(histoName, histoTitle, 32, 0., 32., 1200, 0., 600.);
		fhTDCre_corrected1.push_back(h3);
		AddHistoToVector(h3);

		histoName.Form("fhTDC%dre_corrected2", iTDC);
		histoTitle.Form("TDC %d re corrected2;channel;ns", iTDC);
		TH2D* h4 = new TH2D(histoName, histoTitle, 32, 0., 32., 1200, 0., 600.);
		fhTDCre_corrected2.push_back(h4);
		AddHistoToVector(h4);
*/
		// TODO
		//workaround we need to init all histograms for ToT here. Otherwise they will not be added to monitoring.
		for (Int_t iCh = 0; iCh <= 32; iCh++) {
			Int_t tdc = fUnpackPar->GetAddress(iTDC);
			GetTotH1(tdc, iCh);
		}
 		{
		  Int_t tdc = fUnpackPar->GetAddress(iTDC);
		  GetTotH2(tdc);
		}

		   /*******************************************************************/

		   /// Map of hits over T0 detector and same vs time in run
		{//if (iTDC == 0){
		   Double_t w = 10;
		   Double_t h = 10;

		   TCanvas *c;
		   TString canvasName;
		   TString canvasTitle;
		   Int_t tdc = fUnpackPar->GetAddress(iTDC);
		   canvasName.Form("cToT2d_TDC_0x%4x",tdc);
		   canvasTitle.Form("ToTs of TDC 0x%4x",tdc);
		   c = new TCanvas( canvasName, canvasTitle, w, h);
//		   fcHitMaps->Divide( 2 );
//		   fcHitMaps->cd( 1 );
//		   gPad->SetGridx();
//		   gPad->SetGridy();
//		   gPad->SetLogy();
//		   fhChannelMap->Draw();
//		   fcHitMaps->cd( 2 );
//		   gPad->SetGridx();
//		   gPad->SetGridy();
//		   gPad->SetLogz();
                   TH2D *h2 = GetTotH2(tdc);
		   h2->Draw("colz");
		   fcTot2d.push_back(c);
		   AddCanvasToVector( c, "ToT_Canvases" );

		}
   /*******************************************************************/

	}


   fhVectorSize = new TH1I( "fhVectorSize", "Size of the vector VS TS index; TS index; Size [bytes]",
                            10000, 0., 10000. );
   fhVectorCapacity = new TH1I( "fhVectorCapacity", "Size of the vector VS TS index; TS index; Size [bytes]",
                            10000, 0., 10000. );
   AddHistoToVector( fhVectorSize, "" );
   AddHistoToVector( fhVectorCapacity, "" );

	return kTRUE;
}

TH1D* CbmMcbm2018UnpackerAlgoRich::GetTotH1(Int_t tdc, Int_t channel)
{
	TH1D* h = fhTotMap[tdc][channel];
	if (h == nullptr) {
		TString name, title, subFolder;
		name.Form("ToT_tdc0x%x_ch%u", tdc, channel);
		title.Form("%s;ToT [ns];Entries", name.Data());
		subFolder.Form("ToT/tdc0x%x", tdc);
		h = new TH1D(name, title, 100, -1., 49.);
		AddHistoToVector(h, std::string(subFolder.Data()));
		fhTotMap[tdc][channel] = h;
	}
	return h;
}

TH2D* CbmMcbm2018UnpackerAlgoRich::GetTotH2(Int_t tdc)
{
	TH2D* h = fhTot2dMap[tdc];
	if (h == nullptr) {
		TString name, title, subFolder;
		name.Form("ToT_2d_tdc0x%x", tdc);
		title.Form("%s;channels;ToT [ns]", name.Data());
		subFolder.Form("ToT2d");
		h = new TH2D(name, title,33,0,32, 200, -1., 49.);
		AddHistoToVector(h, std::string(subFolder.Data()));
		fhTot2dMap[tdc] = h;
	}
	return h;
}


/*
Bool_t CbmMcbm2018UnpackerAlgoRich::FillHistograms()
{
	return kTRUE;
}
*/
Bool_t CbmMcbm2018UnpackerAlgoRich::ResetHistograms()
{
	//TODO: do something?
	return kTRUE;
}

ClassImp(CbmMcbm2018UnpackerAlgoRich)
