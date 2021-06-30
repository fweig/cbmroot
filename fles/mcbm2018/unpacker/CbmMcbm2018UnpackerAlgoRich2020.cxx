/* Copyright (C) 2019-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Egor Ovcharenko [committer], Pierre-Alain Loizeau */

/**
 * CbmMcbm2018UnpackerAlgoRich20202020
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

#include "CbmMcbm2018UnpackerAlgoRich2020.h"

// ROOT
#include <Logger.h>

#include <TCanvas.h>
#include <TList.h>

// CbmRoot
#include "CbmMcbm2018RichPar.h"

#include <iostream>

CbmMcbm2018UnpackerAlgoRich2020::CbmMcbm2018UnpackerAlgoRich2020()
  : CbmStar2019Algo()
  , fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fRawDataMode(kFALSE)
  , fError(kFALSE)
  , fTrbState(TrbNetState::IDLE)
  , fErrorCorr(0)
  , fbDoToTCorr(kTRUE)
  , fSkipMs(kFALSE)
  , fdTimeOffsetNs(0.0)
  , fRICHcompIdx(6)
  ,  //TODO experimentally obtained value
  fUnpackPar(nullptr)
  , fTScounter(0)
  , fCurMSid(0)
  , fGwordCnt(0)
  , fInSubSubEvent(kFALSE)
  , fCurEpochCounter(0)
  , fSubSubEvId(0)
  , fLastCTSch0_re_time(0.)
  , fLastCTSch2_re_time(0.)
  , fLastCTSch2_fe_time(0.)
  , fPrevLastCTSch0_re_time(0.)
  , fPrevLastCTSch2_re_time(0.)
  , fPrevLastCTSch2_fe_time(0.)
  , /*,
	fhTDCch0re_minusCTSch0re(nullptr),
	fhTDCch0re_minusCTSch2re(nullptr),
	fhTDCch0re_minusCTSch2fe(nullptr),
	fhTDCch0re_minusPrevCTSch0re(nullptr),
	fhTDCch0re_minusPrevCTSch2re(nullptr),
	fhTDCch0re_minusPrevCTSch2fe(nullptr)*/
  fMapFEE()
  , fhTotMap()
  , fhTot2dMap()
{
  this->Init();  //TODO why this is not called by the framework?
}

CbmMcbm2018UnpackerAlgoRich2020::~CbmMcbm2018UnpackerAlgoRich2020()
{
  if (nullptr != fParCList) delete fParCList;
  if (nullptr != fUnpackPar) delete fUnpackPar;
}

Bool_t CbmMcbm2018UnpackerAlgoRich2020::Init()
{
  LOG(info) << "Initializing mCBM RICH 2019 unpacker algo";
  //fhDigisInChnl   = new TH2D("fhDigisInChnl","fhDigisInChnl;channel;#Digis;" ,2304 , -0.5, 2303.5,  50, -0.5, 49.5);
  //fhDigisInDiRICH = new TH2D("fhDigisInDiRICH","fhDigisInDiRICH;DiRICH;#Digis;",72 , -0.5, 71.5,  300, -0.5, 299.5);
  return kTRUE;
}

void CbmMcbm2018UnpackerAlgoRich2020::Reset() {}

void CbmMcbm2018UnpackerAlgoRich2020::Finish() {}

Bool_t CbmMcbm2018UnpackerAlgoRich2020::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmMcbm2018UnpackerAlgoRich2020";
  Bool_t initOK = ReInitContainers();

  return initOK;
}

Bool_t CbmMcbm2018UnpackerAlgoRich2020::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for CbmMcbm2018UnpackerAlgoRich2020";

  fUnpackPar = (CbmMcbm2018RichPar*) fParCList->FindObject("CbmMcbm2018RichPar");
  if (fUnpackPar == nullptr) { return kFALSE; }

  Bool_t initOK = InitParameters();

  return initOK;
}

TList* CbmMcbm2018UnpackerAlgoRich2020::GetParList()
{
  if (fParCList == nullptr) { fParCList = new TList(); }
  fUnpackPar = new CbmMcbm2018RichPar("CbmMcbm2018RichPar");
  fParCList->Add(fUnpackPar);

  return fParCList;
}

Bool_t CbmMcbm2018UnpackerAlgoRich2020::InitParameters()
{
  InitStorage();
  return kTRUE;
}

void CbmMcbm2018UnpackerAlgoRich2020::InitStorage()
{
  fLastCh0_re_time.Set(fUnpackPar->GetNaddresses());      // Set the size of the array
  fPrevLastCh0_re_time.Set(fUnpackPar->GetNaddresses());  // Set the size of the array
}

/**
  Copied from other detectors without any brain effort...
  A little bug-fix added
**/
void CbmMcbm2018UnpackerAlgoRich2020::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx) {
    if (component == fvMsComponentsList[uCompIdx]) { return; }
  }

  /// Add to list
  fvMsComponentsList.push_back(component);

  if (fvMsComponentsList.size() == 1) { fRICHcompIdx = component; }
  else {
    LOG(WARN) << "fvMsComponentsList.size() > 1 for RICH. Unpacking may not "
                 "work due to implementation limitations.";
  }

  LOG(info) << "CbmMcbm2018UnpackerAlgoRich2020::AddMsComponentToList => Component " << component
            << " with detector ID 0x" << std::hex << usDetectorId << std::dec << " added to list";
}

Bool_t CbmMcbm2018UnpackerAlgoRich2020::ProcessTs(const fles::Timeslice& /*ts*/)
{
  LOG(debug2) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessTs(ts)";
  /*
	//TODO: shortcut. We love shortcuts, right?
	if (fvMsComponentsList.size() == 1) {
		this->ProcessTs(ts, fvMsComponentsList.at(0));
	}

	//TODO: implement case when multiple components have to be processed
*/
  return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoRich2020::ProcessTs(const fles::Timeslice& ts, size_t component)
{
  /// Ignore First TS as first MS is typically corrupt
  if (0 == ts.index()) { return kTRUE; }  // if( 0 == ts.index() )

  LOG(debug2) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessTs(ts, " << component << ")";

  //TODO: skip if this method was called for a wrong component
  //if (component != fRICHcompIdx) return kTRUE;
  //FIXME: this is really nasty...
  //	component = fRICHcompIdx;
  if (1 != fvMsComponentsList.size()) {
    /// If no RICH component, do nothing!
    if (0 == fvMsComponentsList.size()) return kTRUE;

    /// If multiple RICH components, fail the run
    TString sCompList = "";
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx)
      sCompList += Form(" %2lu ", fvMsComponentsList[uMsCompIdx]);
    LOG(fatal) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessTs => More than 1 "
                  "component in list, unpacking impossible! List is "
               << sCompList;
  }  // if( 1 != fvMsComponentsList.size() )
  component = fvMsComponentsList[0];

  LOG(debug) << "Components:  " << ts.num_components();
  LOG(debug) << "Microslices: " << ts.num_microslices(component);

  const uint64_t compSize = ts.size_component(component);
  LOG(debug) << "Component " << component << " has size " << compSize;

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsFullSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs + fuNbOverMsPerTs);
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a core duration of " << fdTsCoreSizeInNs << " ns and a full duration of "
              << fdTsFullSizeInNs << " ns";

    /// Ignore overlap ms if flag set by user
    fuNbMsLoop = fuNbCoreMsPerTs;
    if (kFALSE == fbIgnoreOverlapMs) fuNbMsLoop += fuNbOverMsPerTs;
    LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
  }  // if( -1.0 == fdTsCoreSizeInNs )

  for (size_t iMS = 0; iMS < fuNbMsLoop; ++iMS) {
    fCurMSid = iMS;
    LOG(debug) << "=======================================================";
    const fles::MicrosliceView mv            = ts.get_microslice(component, iMS);
    const fles::MicrosliceDescriptor& msDesc = mv.desc();
    ////const uint8_t* msContent = mv.content();
    LOG(debug) << "msDesc.size=" << msDesc.size;
    fCurMSidx = msDesc.idx;
    LOG(debug) << "msDesc.idx=" << msDesc.idx;
    ////mRichSupport::PrintRaw(msDesc.size, msContent);//TODO delete
    ////LOG(debug) << "=======================================================";
    ////////////////////////////////
    //ProcessMs(ts, component, iMS);//
    ////////////////////////////////

    if (!fRawDataMode) ProcessMs(ts, component, iMS);
    if (fRawDataMode) DebugMs(ts, component, iMS);

    LOG(debug) << "=======================================================";
  }

  ///////////////
  FinalizeTs();  //
  ///////////////

  if (0 == fTScounter % 1000) { LOG(info) << "Processed " << fTScounter << " TS"; }

  fTScounter++;

  /// Sort the buffers of hits due to the time offsets applied
  std::sort(fDigiVect.begin(), fDigiVect.end(),
            [](const CbmRichDigi& a, const CbmRichDigi& b) -> bool { return a.GetTime() < b.GetTime(); });

  if (fbMonitorMode || fbDebugMonitorMode) {
    fhVectorSize->Fill(ts.index(), fDigiVect.size());
    fhVectorCapacity->Fill(ts.index(), fDigiVect.capacity());
  }  // if( fbMonitorMode || fbDebugMonitorMode )

  if (fuTsMaxVectorSize < fDigiVect.size()) {
    fuTsMaxVectorSize = fDigiVect.size() * fdCapacityIncFactor;
    fDigiVect.shrink_to_fit();
    fDigiVect.reserve(fuTsMaxVectorSize);
  }  // if( fuTsMaxVectorSize < fDigiVect.size() )

  return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoRich2020::ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
{
  const fles::MicrosliceView mv            = ts.get_microslice(uMsCompIdx, uMsIdx);
  const fles::MicrosliceDescriptor& msDesc = mv.desc();
  const uint8_t* ptr                       = mv.content();
  const size_t size                        = msDesc.size;

  if (size == 0) return kTRUE;

  fGwordCnt = 0;  //TODO check that this global word counter works properly

  Int_t offset;  // offset in bytes
  Int_t* dataPtr;

  offset = 0;
  mRichSupport::SwapBytes(4, ptr + offset);
  LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
              << "ok"
              << "\t"
              << "Reserved 0000 0000";
  fGwordCnt++;

  offset = 4;
  mRichSupport::SwapBytes(4, ptr + offset);
  // clang-format off
  dataPtr               = (Int_t*) (ptr + offset);
  Int_t mbsNumber       = (Int_t)(dataPtr[0] & 0xffffff);
  uint8_t mts_error_msg = (uint8_t)((dataPtr[0] >> 24) & 0xff);
  // clang-format on
  ErrorMsg(static_cast<uint16_t>(mts_error_msg), RichErrorType::mtsError);
  LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
              << "ok"
              << "\t"
              << "mbsNumber = " << mbsNumber;
  fGwordCnt++;

  // We suppose that the first word is
  // "HadesTransportUnitQueue - Length"
  offset = 0 + 8;
  mRichSupport::SwapBytes(4, ptr + offset);
  // clang-format off
  dataPtr             = (Int_t*) (ptr + offset);
  Int_t TRBeventSize1 = (Int_t)(dataPtr[0]);
  // clang-format on
  LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
              << "ok"
              << "\t"
              << "HadesTransportUnitQueue - Length = " << TRBeventSize1;
  fGwordCnt++;

  if (*dataPtr > 0 && UInt_t(*dataPtr) == 0x80030000) {
    LOG(info) << "dataPtr == 0x80030000";
    exit(EXIT_FAILURE);
  }

  // We suppose that the second word is
  // "HadesTransportUnitQueue - Decoder  (Seems to be allways the same)"
  offset = 4 + 8;
  mRichSupport::SwapBytes(4, ptr + offset);
  // clang-format off
  dataPtr    = (Int_t*) (ptr + offset);
  Int_t dcdr = (Int_t)(dataPtr[0]);
  // clang-format on
  if (dcdr == 0x00030062) {
    LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                << "ok"
                << "\t"
                << "HadesTransportUnitQueue - Decoder = " << dcdr;
    fGwordCnt++;
  }
  else {
    LOG(warning) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                 << "er"
                 << "\t"
                 << "HadesTransportUnitQueue - Decoder = " << dcdr << " is not 0x00030062 (196706) => 0x" << std::hex
                 << dcdr << std::dec;
    fGwordCnt++;

    /// Probably corrupted MS, stop there and skip remaining data
    fSkipMs = kTRUE;
    return kFALSE;
  }

  // We suppose that the third word is
  // TRB event length (in bytes)
  // It should be 8 less than the size specified two words ago
  offset = 8 + 8;
  mRichSupport::SwapBytes(4, ptr + offset);
  // clang-format off
  dataPtr             = (Int_t*) (ptr + offset);
  Int_t TRBeventSize2 = (Int_t)(dataPtr[0]);
  // clang-format on
  if (TRBeventSize2 == TRBeventSize1 - 8) {
    LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                << "ok"
                << "\t"
                << "TRB event - Length = " << TRBeventSize2 << " == " << TRBeventSize1 << "-8";
    fGwordCnt++;
  }
  else {
    LOG(debug) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
               << "er"
               << "\t"
               << "TRB event - Length = " << TRBeventSize2 << " != " << TRBeventSize1 << "-8=" << TRBeventSize1 - 8;
    fGwordCnt++;
  }

  /////////////////////////////////////////////
  ProcessTRBevent(TRBeventSize2, ptr + offset);  //
  /////////////////////////////////////////////

  // Bytes in a TrbEvent
  if (fbDebugMonitorMode) fhEventSize->Fill(TRBeventSize2);


  if (fSkipMs == kTRUE) {
    // problem in data; delete vectors.
    fDigiVect.clear();
    fSkipMs = kFALSE;
  }


  return kTRUE;
}

Int_t CbmMcbm2018UnpackerAlgoRich2020::ProcessTRBevent(size_t const size, uint8_t const* const ptr)
{
  Int_t offset;  // offset in bytes
  Int_t* dataPtr;

  // We assume that the TRB event header is 4 words and
  // the first word is already processed outside of this method

  //////////////////////////////////
  ProcessTRBeventHeader(4 * 4, ptr);  //
  //////////////////////////////////

  offset = 16;  // start from after the TRB event header

  // 1. Search for the CTS subevent and extract reference time

  while (static_cast<size_t>(offset) < size) {
    /// Escape bad MS before doing anything
    if (fSkipMs == kTRUE) break;

    // Duplicate the header word in order to avoid corrupting (by bytes swapping)
    // the original data
    // clang-format off
    dataPtr          = (Int_t*) (ptr + offset);
    
    Int_t headerCopy = *dataPtr;
    mRichSupport::SwapBytes(4, (uint8_t*) &headerCopy);
    dataPtr = &headerCopy;

    Int_t SubEvSize = (Int_t) ((dataPtr[0] >> 16) & 0xffff);
    Int_t HubId     = (Int_t) ((dataPtr[0]) & 0xffff);
    // clang-format on
    // Process only CTS subevent
    //FIXME change from 0xc001 to 0xc000 at some point // ?
    if ((HubId == 0xc001) || (HubId == 0xc000)) {

      // Not a very nice shortcut
      // The global counter of the words is incremented for the CTS subevent header here
      // However for the TRB subevent headers it is incremented in the second run,
      // where only TRB subevent headers are processed and the CTS subevents are skipped
      LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr((uint8_t*) &headerCopy) << "\t"
                  << "ok"
                  << "\t"
                  << "hub ID = 0x" << mRichSupport::GetHexRepresentation(2, (uint8_t*) &headerCopy) << "\t"
                  << "subevent size = " << SubEvSize;
      fGwordCnt++;

      fSubSubEvId = HubId;
      //////////////////////////////////////////////////////////////
      offset += (4 + ProcessCTSsubevent(SubEvSize * 4, ptr + offset));  //
      //////////////////////////////////////////////////////////////

      //std::cout<<"Words in CTS 0x"<< std::hex << HubId << std::dec <<" : "<< SubEvSize <<std::endl;
      // In principle, should be reset here for safety
      fSubSubEvId = 0;
    }
    else {
      // Skip all other subevents
      offset += (4 + SubEvSize * 4);
    }
  }

  offset = 16;  // start from after the TRB event header again

  // 2. Process TRB subsubevents

  //Int_t iIter = 0;
  while (static_cast<size_t>(offset) < size) {
    /// Escape bad MS before doing anything
    if (fSkipMs == kTRUE) break;

    //std::cout << "SE iteration " << iIter++ << "\toffset=" << offset << "\tsize=" << size << std::endl;

    // We suppose that the fifth word is the header of the subevent
    // <Length> <HubId>
    mRichSupport::SwapBytes(4, ptr + offset);
    // clang-format off
    dataPtr         = (Int_t*) (ptr + offset);
    Int_t SubEvSize = (Int_t)((dataPtr[0] >> 16) & 0xffff);
    Int_t HubId     = (Int_t)((dataPtr[0]) & 0xffff);
    // clang-format on
    //FIXME change from 0xc001 to 0xc000 at some point // ?
    if ((HubId == 0xc001) || (HubId == 0xc000)) {
      ////fSubSubEvId = HubId;
      //////////////////////////////////////////////////////////////////
      ////offset += (4 + ProcessCTSsubevent(SubEvSize*4, ptr+offset));//
      //////////////////////////////////////////////////////////////////
      ////// In principle, should be reset here for safety
      ////fSubSubEvId = 0;

      // Skip CTS subevent as it has been already processed during the first run
      offset += (4 + SubEvSize * 4);
      fLastFeeOnHub = false;
    }
    else if (HubId == 0x5555) {
      LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                  << "ok"
                  << "\t"
                  << "hub ID = 0x" << mRichSupport::GetHexRepresentation(2, ptr + offset) << "\t"
                  << "subevent size = " << SubEvSize;
      fGwordCnt++;
      fLastFeeOnHub = false;
      //TODO one could implement additional checks here about the
      // words coming after the "event end" but we skip everything by now.
      ///////////////////////////////////////////////////////////////
      offset += (4 + ProcessSKIPsubevent(SubEvSize * 4, ptr + offset));  //
      ///////////////////////////////////////////////////////////////
    }
    else if (((HubId >> 8) & 0xFF) == 0x82) {
      LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                  << "ok"
                  << "\t"
                  << "hub ID = 0x" << mRichSupport::GetHexRepresentation(2, ptr + offset) << "\t"
                  << "subevent size = " << SubEvSize;
      fGwordCnt++;
      fLastFeeOnHub = false;
      //std::cout<<"Hub: "<<std::hex<<HubId <<std::dec<<"   Size:"<< SubEvSize<<std::endl;
      //////////////////////////////////////////////////////////////
      offset += (4 + ProcessTRBsubevent(SubEvSize * 4, ptr + offset));  //
      //////////////////////////////////////////////////////////////

      //std::cout<<"Words in Hub 0x"<< std::hex << HubId << std::dec <<" : "<< SubEvSize <<std::endl;
      // Bytes in a Hub
      if (fbDebugMonitorMode) {
        uint16_t combiner_address = ((HubId >> 4) & 0xF) * 3 + (HubId & 0xF);
        fhSubEventSize->Fill(combiner_address, (SubEvSize * 4));
      }
    }
    else {
      LOG(WARN) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                << "ok"
                << "\t"
                << "hub ID = 0x" << mRichSupport::GetHexRepresentation(2, ptr + offset) << "\t"
                << "subevent size = " << SubEvSize << "\n"
                << "This is not a valid Combiner Id!"
                << "\n"
                << "prev prev2:" << mRichSupport::GetWordHexRepr(ptr + offset - 12) << "\n"
                << "prev prev: " << mRichSupport::GetWordHexRepr(ptr + offset - 8) << "\n"
                << "prev:      " << mRichSupport::GetWordHexRepr(ptr + offset - 4) << "\n"
                << "next:      " << mRichSupport::GetWordHexRepr(ptr + offset + 4) << "\n"
                << "next next: " << mRichSupport::GetWordHexRepr(ptr + offset + 8) << "\n";
      //////////////////////////////////////////////////////////////
      offset += (4 + SubEvSize * 4);
      //////////////////////////////////////////////////////////////
    }
  }

  ////LOG(debug4) <<  "Done processing TRB event. offset=" << offset << "\tsize=" << size;
  //TODO implement checks
  if (size != static_cast<size_t>(offset)) {
    LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessTRBevent() warning:"
              << "Number of processed bytes is not equal to the expected size. "
                 "This should not happen. ("
              << size << " VS " << offset << ")";
  }

  return size;  //TODO check
}

// Process TRB event header.
// Input arguments are the size of the TRB event header (16 bytes) and the pointer to the first word.
// Note that the first word can already be analysed outside of this method.
// Return number of bytes processed. For this particular method the value of the input 'size' argument
// is returned as we expect that the TRB header is always 16 bytes.
Int_t CbmMcbm2018UnpackerAlgoRich2020::ProcessTRBeventHeader(size_t const size, uint8_t const* const ptr)
{
  Int_t offset;  // offset in bytes
  Int_t* dataPtr;

  // Skip first word (already processed outside)
  //offset = 0;
  // do nothing

  // We suppose that the second word consists of
  // 0002 - number of following word till the Event Data Starts (should be the same)
  // 00<TriggerType>1 - value in [7:4] defines TriggerType
  offset = 4;
  mRichSupport::SwapBytes(4, ptr + offset);
  // clang-format off
  dataPtr           = (Int_t*) (ptr + offset);
  Int_t checkSize   = (Int_t)((dataPtr[0] >> 16) & 0xffff);
  Int_t triggerType = (Int_t)((dataPtr[0] >> 4) & 0xf);
  // clang-format on
  if (checkSize == 2) {
    LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                << "ok"
                << "\t"
                << "checkSize == 2"
                << "\t"
                << "trigger type = " << triggerType;
    fGwordCnt++;
  }
  else {
    LOG(warning) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                 << "er"
                 << "\t"
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
  offset = 8;
  mRichSupport::SwapBytes(4, ptr + offset);
  // clang-format off
  dataPtr          = (Int_t*) (ptr + offset);
  Int_t checkBytes = (Int_t)((dataPtr[0] >> 16) & 0xffff);
  // clang-format on
  //	Int_t SubEvId = (Int_t)((dataPtr[0]) & 0xffff);
  if (checkBytes == 0) {
    LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                << "ok"
                << "\t"
                << "checkBytes == 0"
                << "\t"
                << "subevent ID = 0x" << mRichSupport::GetHexRepresentation(2, ptr + offset);
    fGwordCnt++;
  }
  else {
    LOG(warning) << "[" << fGwordCnt++ << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                 << "er"
                 << "\t"
                 << "checkBytes != 0 (" << checkBytes << ")\t"
                 << "subevent ID = 0x" << mRichSupport::GetHexRepresentation(2, ptr + offset);
    fGwordCnt++;

    /// Probably corrupted MS, stop there and skip remaining data
    fSkipMs = kTRUE;
    return 0;
  }

  // We suppose that the fourth word is the trigger number
  offset = 12;
  mRichSupport::SwapBytes(4, ptr + offset);
  // clang-format off
  dataPtr           = (Int_t*) (ptr + offset);
  UInt_t TriggerNum = (UInt_t)(dataPtr[0]);
  // clang-format on
  LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
              << "ok"
              << "\t"
              << "trigger num = " << TriggerNum;
  fGwordCnt++;

  return size;
}

Int_t CbmMcbm2018UnpackerAlgoRich2020::ProcessSKIPsubevent(size_t const size, uint8_t const* const ptr)
{
  ////LOG(debug4) << "ProcessSKIPsubevent size=" << size << " bytes";

  Int_t offset;    // offset in bytes
  Int_t* dataPtr;  //(FU) not used
  uint16_t SubEventError = 0;

  // Skip first word (already processed outside)
  offset = 4;

  //Start Error identification
  mRichSupport::SwapBytes(4, ptr + offset);
  // clang-format off
  dataPtr       = (Int_t*) (ptr + offset);
  SubEventError = (uint16_t)((dataPtr[0] >> 16) & 0xffff);
  // clang-format on
  ErrorMsg(static_cast<uint16_t>(SubEventError), RichErrorType::subEventError);

  offset = 8;
  //End Error identification

  while (static_cast<size_t>(offset) < size + 4) {
    mRichSupport::SwapBytes(4, ptr + offset);
    //                dataPtr = (Int_t*)(ptr+offset); (FU) not used
    LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset);
    fGwordCnt++;

    offset += 4;
  }

  ////LOG(debug4) << "Done processing SKIP subevent. offset=" << offset << "\tsize=" << size;
  //TODO implement checks
  if (size != static_cast<size_t>(offset - 4)) {
    LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessSKIPsubevent() warning:"
              << "Number of processed bytes is not equal to the expected size. "
                 "This should not happen.";
  }

  return size;  //TODO check
}

Int_t CbmMcbm2018UnpackerAlgoRich2020::ProcessCTSsubevent(size_t const size, uint8_t const* const ptr)
{
  ////LOG(debug4) << "ProcessCTSsubevent size=" << size << " bytes";

  Int_t offset;  // offset in bytes
  Int_t* dataPtr;

  // Skip first word (already processed outside)

  // We suppose that the second word is the header of the subsubevent
  offset = 4;
  mRichSupport::SwapBytes(4, ptr + offset);
  // clang-format off
  dataPtr = (Int_t*) (ptr + offset);
  // clang-format on
  LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
              << "ok"
              << "\t"
              << "CTS header";
  fGwordCnt++;

  /*      (FU) not used
	Short_t trigState[16];
	for (Int_t i=0; i<16; i++) {
		trigState[i] = ((*dataPtr >> i) & 0x1); // 16 x 1 bit
	}
*/
  Short_t nInp         = ((*dataPtr >> 16) & 0xf);   // 4 bits
  Short_t nTrigCh      = ((*dataPtr >> 20) & 0x1f);  // 5 bits
  Short_t inclLastIdle = ((*dataPtr >> 25) & 0x1);   // 1 bit
  Short_t inclTrigInfo = ((*dataPtr >> 26) & 0x1);   // 1 bit
  Short_t inclTS       = ((*dataPtr >> 27) & 0x1);   // 1 bit
  Short_t ETM          = ((*dataPtr >> 28) & 0x3);   // 2 bits

  // in words (not bytes)
  Short_t CTSinfo_size = nInp * 2 + nTrigCh * 2 + inclLastIdle * 2 + inclTrigInfo * 3 + inclTS;
  switch (ETM) {
    case 0: break;
    case 1: CTSinfo_size += 1; break;
    case 2: CTSinfo_size += 4; break;
    case 3:
      LOG(debug) << "ETM == 3";
      //TODO implement
      break;
  }

  LOG(debug) << "CTS information size (extracted from the CTS header): " << CTSinfo_size;

  offset = 8;

  while (offset - 8 < CTSinfo_size * 4) {
    mRichSupport::SwapBytes(4, ptr + offset);
    // clang-format off
    dataPtr       = (Int_t*) (ptr + offset);
    ULong_t MSidx = 102400UL * ((ULong_t)(*dataPtr) - 1);
    // clang-format on
    LOG(debug) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
               << "ok"
               << "\t"
               << "CTS information"
               << " MSidx=" << MSidx;
    fGwordCnt++;

    offset += 4;
  }

  // size - full size including CTS header word, CTS informations words (CTSinfo_size) and TCD data
  // Thus TDC data size = full size - 1 word (header) - CTSinfo_size words (CTS informations)
  ///////////////////////////////////////////////////////////////////////////
  fChnlMsgCnt.fill(0);
  offset += (ProcessTRBsubsubevent((size - (1 + CTSinfo_size) * 4), ptr + offset, 0, 0));  //
  ///////////////////////////////////////////////////////////////////////////

  ////LOG(debug4) << "Done processing CTS subevent. offset-4=" << offset-4 << "\tsize=" << size;
  //TODO implement checks
  if (size != static_cast<size_t>(offset - 4)) {
    LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessCTSsubevent() warning:"
              << "Number of processed bytes is not equal to the expected size. "
                 "This should not happen.";
  }

  return size;  //TODO check
}

Int_t CbmMcbm2018UnpackerAlgoRich2020::ProcessTRBsubevent(size_t const size, uint8_t const* const ptr)
{
  ////LOG(debug4) << "ProcessTRBsubevent size=" << size << " bytes";

  Int_t offset;  // offset in bytes
  Int_t* dataPtr;

  // Skip first word (already processed outside)
  offset = 4;

  fTdcWordCorrectionCnt = 0;

  findTDCAlignmentError(ptr, size);

  //Int_t iIter = 0;
  while (static_cast<size_t>(offset) < (size - 2)) {  // test for cases with odd number of corrections
    if (fSkipMs == kTRUE) break;
    //std::cout << "SSE iteration " << iIter++ << "\toffset=" << offset << "\tsize=" << size << std::endl;

    //correct for misalignment
    if (fTDCAlignmentErrorPositions.size() > static_cast<unsigned int>(fTdcWordCorrectionCnt)
        && fTDCAlignmentErrorPositions[fTdcWordCorrectionCnt] == offset) {
      //std::cout<<"Correction in DiRICH Header: "<< fTDCAlignmentErrorPositions[fTdcWordCorrectionCnt]<<std::endl;
      offset += 2;
      fTdcWordCorrectionCnt++;
    }

    // We suppose that the second word is the header of the subsubevent
    // <Length> <SubSubEv.Id>
    mRichSupport::SwapBytes(4, ptr + offset);
    // clang-format off
    dataPtr            = (Int_t*) (ptr + offset);
    Int_t SubSubEvSize = (Int_t)((dataPtr[0] >> 16) & 0xffff);
    fSubSubEvId        = (Int_t)((dataPtr[0]) & 0xffff);
    // clang-format on
    //check if it is the last DiRICH in the Hub Data stream
    //TODO CHECK!
    if ((static_cast<size_t>(offset) + SubSubEvSize * 4) >= size) {
      LOG(debug) << "Last DiRICH on HUB";
      fLastFeeOnHub = true;
    }

    if (((fSubSubEvId >> 12) & 0xF) != 0x7) {
      LOG(error) << mRichSupport::GetWordHexRepr(ptr + offset - 12) << "\t"
                 << "er"
                 << "\t"
                 << "ILLEGAL SubSubEvent Id  prev";
      LOG(error) << mRichSupport::GetWordHexRepr(ptr + offset - 8) << "\t"
                 << "er"
                 << "\t"
                 << "ILLEGAL SubSubEvent Id  prev";
      LOG(error) << mRichSupport::GetWordHexRepr(ptr + offset - 4) << "\t"
                 << "er"
                 << "\t"
                 << "ILLEGAL SubSubEvent Id  prev";
      LOG(error) << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
                 << "er"
                 << "\t"
                 << "ILLEGAL SubSubEvent Id  "
                 << "Offset:" << static_cast<size_t>(offset) << "  Size:" << size;
      LOG(error) << mRichSupport::GetWordHexRepr(ptr + offset + 4) << "\t"
                 << "er"
                 << "\t"
                 << "ILLEGAL SubSubEvent Id next";
      LOG(error) << mRichSupport::GetWordHexRepr(ptr + offset + 8) << "\t"
                 << "er"
                 << "\t"
                 << "ILLEGAL SubSubEvent Id next";
      LOG(error) << mRichSupport::GetWordHexRepr(ptr + offset + 12) << "\t"
                 << "er"
                 << "\t"
                 << "ILLEGAL SubSubEvent Id next";
    }

    LOG(debug) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr + offset) << "\t"
               << "ok"
               << "\t"
               << "subsubevent ID (FPGA ID) = 0x" << mRichSupport::GetHexRepresentation(2, ptr + offset) << "\t"
               << "subsubevent size = " << SubSubEvSize << " | HUB Offset:" << static_cast<size_t>(offset)
               << "  Size:" << size;
    fGwordCnt++;

    if (size + 4 < static_cast<size_t>(offset + 4 + SubSubEvSize * 4 - fTdcWordCorrectionCnt * 2)) {
      LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessTRBsubevent() warning:"
                << "SubEvent out of bounds. This should not happen. (" << size << " VS "
                << (offset + 4 + SubSubEvSize * 4 - fTdcWordCorrectionCnt * 2) << ")";

      /// Probably corrupted MS, stop there and skip remaining data
      //fSkipMs = kTRUE;
    }

    fChnlMsgCnt.fill(0);

    // Add 4 bytes which correspond to the header word
    //////////////////////////////////////////////////////////////////////
    offset += (4 + ProcessTRBsubsubevent(SubSubEvSize * 4, ptr + offset + 4, offset + 4, size));  //
    //////////////////////////////////////////////////////////////////////

    //std::cout<<"Words in DiRICH 0x"<< std::hex << fSubSubEvId << std::dec <<" : "<< SubSubEvSize <<std::endl;

    if (fbDebugMonitorMode) {
      //This address calculation is just for mCBM; will be a problem when using full CBM RICH acceptance
      uint16_t DiRICH_address = ((fSubSubEvId >> 8) & 0xF) * 18 + ((fSubSubEvId >> 4) & 0xF) * 2 + (fSubSubEvId & 0xF);
      fhSubSubEventSize->Fill(DiRICH_address,
                              SubSubEvSize);  // Words in a DiRICH

      //Words per channel
      for (size_t i = 1; i < fChnlMsgCnt.size(); ++i) {
        if (fChnlMsgCnt.at(i) > 0) fhChnlSize->Fill(static_cast<int>(i), fChnlMsgCnt.at(i));
      }
    }

    // In principle, should be reset here for safety
    fSubSubEvId = 0;
  }

  if (static_cast<Int_t>(fTDCAlignmentErrorPositions.size()) != fTdcWordCorrectionCnt)
    std::cout << "Missing Correction" << std::endl;

  // 	if (fTDCAlignmentErrorPositions.size() > 0){
  // 		std::cout<<"Offset : "<<offset-4<<"   Size:"<< size <<std::endl;
  // 		std::cout<<"END of Hub    : "<<mRichSupport::GetWordHexRepr(ptr+offset-4)<<std::endl; // Last word, processed as TDCWord
  // 		std::cout<<"END of Hub +1 : "<<mRichSupport::GetWordHexRepr(ptr+offset+0)<<std::endl;
  // 		std::cout<<"END of Hub +2 : "<<mRichSupport::GetWordHexRepr(ptr+offset+4)<<std::endl;
  // 		std::cout<<"END of Hub +3 : "<<mRichSupport::GetWordHexRepr(ptr+offset+8)<<std::endl;
  // 	}

  ////LOG(debug4) << "Done processing TRB subevent. offset-4=" << offset-4 << "\tsize=" << size;
  if (size != static_cast<size_t>(offset - 4)) {
    LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessTRBsubevent() warning:"
              << "Number of processed bytes is not equal to the expected size. "
                 "This should not happen. ("
              << size << " VS " << (offset - 4) << ")"
              << "  Correction: " << fTdcWordCorrectionCnt * 2 << "  fLastFeeOnHub:" << fLastFeeOnHub;

    /// Probably corrupted MS, stop there and skip remaining data
    //fSkipMs = kTRUE;
  }

  fTdcWordCorrectionGlobalCnt += fTdcWordCorrectionCnt;

  return size;  //TODO check
}

Int_t CbmMcbm2018UnpackerAlgoRich2020::ProcessTRBsubsubevent(size_t const size, uint8_t const* const ptr,
                                                             Int_t const hubOffset, size_t const hubSize)
{  //size: Size of Data from DiRICH in Bytes
  ////LOG(debug4) << "ProcessTRBsubsubevent size=" << size  << " bytes";
  Int_t offset                  = 0;       // offset in bytes
  fCurEpochCounter              = 0;       //TODO check
  fInSubSubEvent                = kFALSE;  //TODO check
  fTrbState                     = TrbNetState::IDLE;
  Int_t TdcWordCorrection_local = 0;
  Int_t WordCnt                 = 0;
  bool break_flag               = false;

  for (size_t iWord = 0; iWord < size / 4; iWord++) {  // iWord is size in Lines
    //correct for misalignment
    //hubOffset is pointing to first word after DiRICH address
    if (fTDCAlignmentErrorPositions.size() > static_cast<unsigned int>(fTdcWordCorrectionCnt)
        && fTDCAlignmentErrorPositions[fTdcWordCorrectionCnt] == static_cast<Int_t>(hubOffset + offset + iWord * 4)) {
      //BEGIN DEBUG
      //                 std::cout<<"DEBUG -1: "<< mRichSupport::GetWordHexRepr(ptr-hubOffset+fTDCAlignmentErrorPositions[fTdcWordCorrectionCnt]-4)   << std::endl;
      //                 std::cout<<"DEBUG  0: "<< mRichSupport::GetWordHexRepr(ptr-hubOffset+fTDCAlignmentErrorPositions[fTdcWordCorrectionCnt])   << std::endl;
      //                 std::cout<<"DEBUG +1: "<< mRichSupport::GetWordHexRepr(ptr-hubOffset+fTDCAlignmentErrorPositions[fTdcWordCorrectionCnt]+4)   << std::endl;
      //                 std::cout<<"DEBUG_  : "<< mRichSupport::GetWordHexRepr(ptr+iWord*4+offset)   << std::endl;
      //
      //                 std::cout<<"Correction in DiRICH Header: "<< fTDCAlignmentErrorPositions[fTdcWordCorrectionCnt]<<std::endl;

      //END   DEBUG
      offset += 2;
      fTdcWordCorrectionCnt++;
      TdcWordCorrection_local++;
      // repeat word
      iWord--;
      continue;
    }
    if (fSkipMs == kTRUE) break;


    //if (fTDCAlignmentErrorPositions.size() > 0 && fLastFeeOnHub) std::cout<<"Final Word: "<< mRichSupport::GetWordHexRepr(ptr+iWord*4+offset)<<std::endl;

    //////////////////////////////
    if ((hubSize > 0) && (hubOffset + offset + iWord * 4 > hubSize)) {
      //std::cout<<"BREAKING   : "<<hubOffset+offset+iWord*4 <<" > "<<  hubSize <<" | "<< offset << " | "<< fTdcWordCorrectionCnt <<std::endl;
      break_flag = true;
      break;
    }
    //if (isCTSWord) std::cout<<"TDCWORD: "<<mRichSupport::GetWordHexRepr(ptr+iWord*4+offset)<<std::endl;

    mRichSupport::SwapBytes(4, ptr + iWord * 4 + offset);
    ProcessTDCword(ptr + iWord * 4 + offset, iWord, size);  //

    WordCnt++;

    //std::cout<<"   "<< iWord <<"  "<< WordCnt <<std::endl;
  }  //END of for Loop

  // 	if (fTdcWordCorrectionCnt > 0){
  // 		std::cout<<"LAST Processed Word    : "<<mRichSupport::GetWordHexRepr(ptr+(WordCnt-1)*4+offset)<<std::endl;
  // 	}
  //if (TdcWordCorrection_local != 0) printf(" --- TDC WORD FIX APPLIED ! --- [DiRICH : 0x%4x]\n",fSubSubEvId);

  if (fSkipMs == kTRUE) return 0;

  //TODO Implement checks that the first word was the header and the last word was the trailer

  //if (size != static_cast<size_t>((WordCnt)*4) && fTdcWordCorrectionCnt == 0) {
  if (!((!break_flag && ((size) == static_cast<size_t>((WordCnt) *4)))
        || (break_flag && ((size - (fTdcWordCorrectionCnt * 2)) == static_cast<size_t>((WordCnt) *4))))) {
    LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessTRBsubsubevent() warning:"
              << "Number of processed bytes is not equal to the expected size. "
                 "This should not happen."
              << static_cast<size_t>(WordCnt * 4) << "   " << size;
    /// Probably corrupted MS, stop there and skip remaining data
    //fSkipMs = kTRUE;
  }


  return (WordCnt * 4 + offset);  //TODO check
}

Int_t CbmMcbm2018UnpackerAlgoRich2020::ProcessTDCword(uint8_t const* const ptr, Int_t const word, size_t const size)
{
  // clang-format off
  Int_t* tdcDataPtr       = (Int_t*) ptr;
  Int_t tdcData           = tdcDataPtr[0];
  Int_t tdcTimeDataMarker = (tdcData >> 31) & 0x1;  // 1 bit
  // clang-format on

  bool errorInData = false;

  // A TDC Time i only valid after a EPOCH or another TDC value
  if ((tdcTimeDataMarker == 0x1 && fTrbState == TrbNetState::TDC)
      || (tdcTimeDataMarker == 0x1 && fTrbState == TrbNetState::EPOCH)) {
    UInt_t tdcMarker = (tdcData >> 29) & 0x7;  // 3 bits
    if (tdcMarker == 0x4 || tdcMarker == 0x5) {
      fDebugPrint = 0;
      ////////////////////////////////
      ProcessTimestampWord(tdcData);  //
      ////////////////////////////////
      fTrbState = TrbNetState::TDC;
    }
    else {
      std::cout << "wrong TDC Word!!" << std::endl;
      errorInData = true;
    }
  }
  else {
    UInt_t tdcMarker = (tdcData >> 29) & 0x7;  // 3 bits

    if (tdcMarker == 0x0) {  // TDC trailer
      if (fInSubSubEvent) {
        if (!(fTrbState == TrbNetState::HEADER || fTrbState == TrbNetState::EPOCH || fTrbState == TrbNetState::TDC)) {
          LOG(error) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                     << "er"
                     << "\t"
                     << "ILLEGAL TRAILER Position";
          errorInData = true;
        }
        else if ((size / 4 - static_cast<size_t>(word)) > 1) {
          //Trailer only at end of SubSubEvent!
          LOG(error) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                     << "er"
                     << "\t"
                     << "Trailer only at end of SubSubEvent!" << size / 4 << "  " << static_cast<size_t>(word);
          errorInData = true;
        }
        else {
          fTrbState = TrbNetState::TRAILER;

          LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                      << "ok"
                      << "\t"
                      << "TDC TRAILER";
          //extract TDC Trailer Error
          uint16_t errorBits = (tdcData) &0xffff;  //16 bits
          ErrorMsg(errorBits, RichErrorType::tdcTrailer, fSubSubEvId);
          fInSubSubEvent = kFALSE;  // go out of InSubSubEvent state
          //fGwordCnt++;
          fDebugPrint = 0;
        }
      }
      else {
        LOG(info) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                  << "er"
                  << "\t"
                  << "UNKNOWN (TDC TRAILER not after header)";
        //fSkipMs = kTRUE;
        errorInData = true;
        //exit(EXIT_FAILURE); //TODO probably one should get rid of explicit EXIT calls not to ruin unpacking of other detectors?
      }
    }
    else if (tdcMarker == 0x1) {  // TDC header
      //	UInt_t randomCode = (tdcData >> 16) & 0xff; // 8 bits
      //	UInt_t errorBits = (tdcData) & 0xffff; //16 bits
      if (!fInSubSubEvent) {
        fInSubSubEvent = kTRUE;  // go into InSubSubEvent state

        if (!(fTrbState == TrbNetState::IDLE || fTrbState == TrbNetState::TRAILER)) {
          LOG(error) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                     << "er"
                     << "\t"
                     << "ILLEGAL HEADER Position";
          errorInData = true;
        }
        else if (!((((tdcData >> 8) & 0xFFFFFF) == 0x200096) || (((tdcData >> 8) & 0xFFFFFF) == 0x200095))) {
          LOG(error) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                     << "er"
                     << "\t"
                     << "ILLEGAL HEADER Value";
          errorInData = true;
        }
        else {
          fTrbState = TrbNetState::HEADER;
          //extract TDC Header Error
          uint8_t errorBits = (tdcData) &0xff;  //8 bits
          ErrorMsg(errorBits, RichErrorType::tdcHeader, fSubSubEvId);
          LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                      << "ok"
                      << "\t"
                      << "TDC HEADER";
        }
        //fGwordCnt++;
      }
      else {
        LOG(info) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                  << "er"
                  << "\t"
                  << "UNKNOWN (TDC HEADER not after trailer)";
        errorInData = true;
        //fGwordCnt++;
        //fSkipMs = kTRUE;
        //exit(EXIT_FAILURE); //TODO probably one should get rid of explicit EXIT calls not to ruin unpacking of other detectors?
      }
    }
    else if (tdcMarker == 0x2) {  // DEBUG
      //	UInt_t debugMode = (tdcData >> 24) & 0x1f; // 5 bits
      //	UInt_t debugBits = (tdcData) & 0xffffff; // 24 bits
      //fTrbState = TrbNetState::DEBUG;
      LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                  << "ok"
                  << "\t"
                  << "DEBUG";
      LOG(info) << "DEBUG VALUE [" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr);
      errorInData = true;
      //fGwordCnt++;
      // currently no actions if a DEBUG message is encountered.
    }
    else if (tdcMarker == 0x3) {  // EPOCH counter
      if (!(fTrbState == TrbNetState::HEADER || fTrbState == TrbNetState::TDC || fTrbState == TrbNetState::EPOCH)) {
        LOG(error) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                   << "er"
                   << "\t"
                   << "ILLEGAL EPOCH Position!";
        errorInData = true;
      }
      else if (((tdcData >> 28) & 0xF) != 0x6) {  //EPOCH is always 0x6....
        LOG(error) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                   << "er"
                   << "\t"
                   << "ILLEGAL EPOCH value :";
        errorInData = true;
      }
      else {
        fTrbState        = TrbNetState::EPOCH;
        fDebugPrint      = 0;
        fCurEpochCounter = (tdcData) &0xfffffff;  // 28 bits
        LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                    << "ok"
                    << "\t"
                    << "EPOCH\t" << fCurEpochCounter;
        //fGwordCnt++;
      }
    }
    else {
      if (tdcTimeDataMarker != 0x1) {
        LOG(error) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr(ptr) << "\t"
                   << "er"
                   << "\t"
                   << "UNKNOWN";
        errorInData = true;
      }
    }
  }

  if (errorInData) {
    //Handle error
    fSkipMs = kTRUE;
    fSkipCnt++;
    LOG(error) << " >>> Skipping MicroTS due to error in data! <<<";
  }


  return 0;  //correction;
}

void CbmMcbm2018UnpackerAlgoRich2020::ProcessTimestampWord(Int_t tdcData)
{
  Int_t channel = (tdcData >> 22) & 0x7f;   // 7 bits
  Int_t fine    = (tdcData >> 12) & 0x3ff;  // 10 bits
  Int_t edge    = (tdcData >> 11) & 0x1;    // 1 bit
  Int_t coarse  = (tdcData) &0x7ff;         // 11 bits
  Int_t epoch   = fCurEpochCounter;

  //TODO move full time calculation outside
  // clang-format off
  Double_t fullTime = (Double_t) epoch * 2048. * 5. + (Double_t) (coarse) *5. - (Double_t) (fine) *0.005;
  // clang-format on

  LOG(debug4) << "[" << fGwordCnt << "]\t" << mRichSupport::GetWordHexRepr((uint8_t*) &tdcData) << "\t"
              << "ok"
              << "\t"
              << "TIMESTAMP"
              << "\t"
              << "ch=" << channel << "\t"
              << "edge=" << edge << "\t"
              << "epoch=" << epoch << "\t"
              << "coarse=" << coarse << "\t"
              << "fine=" << fine << "\t"
              << "full=" << fullTime;
  //fGwordCnt++;

  // Storing reference times
  // =======================

  ////LOG(debug4) << "fSubSubEvId=0x" << std::hex << fSubSubEvId << std::dec;
  Int_t idx = fUnpackPar->GetAddressIdx(fSubSubEvId);
  if (-1 == idx) {
    /// Probably corrupted MS, stop there and skip remaining data
    fSkipMs = kTRUE;
    return;
  }
  ////LOG(debug4) << "fSubSubEvId=0x" << std::hex << fSubSubEvId << std::dec << " idx=" << idx;

  if ((fSubSubEvId == 0xc000) || (fSubSubEvId == 0xc001)) {
    // if CTS
    if ((channel == 0) && (edge == RISINGEDGEID)) {
      fPrevLastCTSch0_re_time = fLastCTSch0_re_time;
      fLastCTSch0_re_time     = fullTime;
      ////LOG(debug4) << "Storing full time for the CTS ch=0 edge=RISINGEDGEID";
    }
    else if ((channel == 2) && (edge == RISINGEDGEID)) {
      fPrevLastCTSch2_re_time = fLastCTSch2_re_time;
      fLastCTSch2_re_time     = fullTime;
      ////LOG(debug4) << "Storing full time for the CTS ch=2 edge=RISINGEDGEID";
    }
    else if ((channel == 2) && (edge == FALLINGEDGEID)) {
      fPrevLastCTSch2_fe_time = fLastCTSch2_fe_time;
      fLastCTSch2_fe_time     = fullTime;
      ////LOG(debug4) << "Storing full time for the CTS ch=2 edge=FALLINGEDGEID";
    }
  }
  else {
    // if not CTS (which means TRB)
    if ((channel == 0) && (edge == RISINGEDGEID)) {
      fPrevLastCh0_re_time[idx] = fLastCh0_re_time[idx];
      fLastCh0_re_time[idx]     = fullTime;
      ////LOG(debug4) << "Storing full time for TDC 0x" << std::hex << fSubSubEvId << std::dec << " ch=0 edge=RISINGEDGEID";
    }
  }

  // Calculation of corrected time
  // =============================
  Double_t fullTimeCorr = 0.;
  if (!((fSubSubEvId == 0xc000) || (fSubSubEvId == 0xc001))) {
    if (channel != 0) {
      Double_t dT   = fullTime - fPrevLastCh0_re_time[idx];
      Double_t corr = fPrevLastCTSch2_re_time - fPrevLastCTSch0_re_time;
      fullTimeCorr  = dT - corr;
    }
  }

  // Filling histograms
  // ==================
  if (fbMonitorMode == kTRUE) {
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

        Double_t corr1       = fPrevLastCTSch2_re_time - fPrevLastCTSch0_re_time;
        Double_t correctedT1 = dT8 + corr1;
        Double_t correctedT2 = dT8 - corr1;
        /*
//				TH2D* h3 = fhTDCre_corrected1.at(idx);
//				h3->Fill(channel, correctedT1);
//				TH2D* h4 = fhTDCre_corrected2.at(idx);
//				h4->Fill(channel, correctedT2);
*/
        LOG(debug4)
          /*<< "dT7=" << dT7*/ << "\tdT8=" << dT8 << "\tcorr1=" << corr1 << "\tcorrectedT1=" << correctedT1
                               << "\tcorrectedT2=" << correctedT2;
      }
    }
  }

  if (edge == RISINGEDGEID) { this->ProcessRisingEdge(fSubSubEvId, channel, fullTimeCorr); }
  else {
    this->ProcessFallingEdge(fSubSubEvId, channel, fullTimeCorr);
  }

  fChnlMsgCnt.at(channel)++;
  if (fTrbState == TrbNetState::EPOCH) fChnlMsgCnt.at(channel)++;  // If there was a correp. EPOCH before
}

void CbmMcbm2018UnpackerAlgoRich2020::ProcessRisingEdge(Int_t subSubEvId, Int_t channel, Double_t time)
{
  ////LOG(debug4) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessRisingEdge()";

  //TODO: not a very nice hack.
  // All messages from ch0 are skipped. Though, probably, that is corect.
  if (channel == 0) return;

  // Also skip everything from CST
  if ((subSubEvId == 0xc000) || (subSubEvId == 0xc001)) return;

  fRisingEdgesBuf.push_back(CbmMcbmRichEdge(subSubEvId, channel, time));
}

void CbmMcbm2018UnpackerAlgoRich2020::ProcessFallingEdge(Int_t subSubEvId, Int_t channel, Double_t time)
{
  ////LOG(debug4) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessFallingEdge()";

  // Skip everything from CST
  if ((subSubEvId == 0xc000) || (subSubEvId == 0xc001)) return;

  Bool_t reFound = kFALSE;

  std::vector<CbmMcbmRichEdge>::iterator reIter = fRisingEdgesBuf.begin();

  while (reIter != fRisingEdgesBuf.end()) {
    if (((*reIter).fSubSubEventID == subSubEvId) && ((*reIter).fChannel == channel)) {
      Double_t reTime = (*reIter).fTime;
      // Found corresponding rising edge
      Double_t ToT = time - reTime;

      if ((ToT >= TOTMIN) && (ToT <= TOTMAX)) {
        // Time-over-threshold is within allowed range

        reFound = kTRUE;

        LOG(debug4) << "Found pair for FPGA ID 0x" << std::hex << subSubEvId << std::dec << "\tch=" << channel
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
        WriteOutputDigi(subSubEvId, channel, reTime, ToT, fCurMSidx);  //
        //////////////////////////////////////////////////

        reIter = fRisingEdgesBuf.erase(reIter);
        continue;  // Take care. This has to be the last operation in this block
      }
      else {
        //TODO: exception. By now we can just do nothing
      }
    }  // end of if condition

    // This construction is a little bit tricky.
    // The iterator is either incremented here or (if a pair was found)
    // incremented using erase call, followed by the continue.
    ++reIter;
  }  // end of for loop

  if (reFound == kFALSE) {
    // Corresponding rising edge not found - store the falling edge in the bufer
    fFallingEdgesBuf.push_back(CbmMcbmRichEdge(subSubEvId, channel, time));
  }
}

void CbmMcbm2018UnpackerAlgoRich2020::WriteOutputDigi(Int_t fpgaID, Int_t channel, Double_t time, Double_t tot,
                                                      uint64_t MSidx)
{
  Double_t ToTcorr = fbDoToTCorr ? fUnpackPar->GetToTshift(fpgaID, channel) : 0.;
  Int_t pixelUID   = this->GetPixelUID(fpgaID, channel);
  //check ordering
  Double_t finalTime = time + (Double_t) MSidx - fdTimeOffsetNs;

  Double_t lastTime = 0.;

  if (fDigiVect.size() < 1) { fDigiVect.emplace_back(pixelUID, finalTime, tot - ToTcorr); }
  else {
    lastTime = fDigiVect[fDigiVect.size() - 1].GetTime();
    if (lastTime > finalTime) {
      for (int i = fDigiVect.size() - 1; i >= 0; i--) {
        lastTime = fDigiVect[i].GetTime();
        if (lastTime <= finalTime) {
          // LOG(info) << " before:"<< fDigiVect.size();
          fDigiVect.emplace(fDigiVect.begin() + i + 1, pixelUID, finalTime, tot - ToTcorr);
          // LOG(info) << fDigiVect.size();
          break;
        }
      }
    }
    else {
      fDigiVect.emplace_back(pixelUID, finalTime, tot - ToTcorr);
    }
  }
  LOG(debug4) << "CbmMcbm2018UnpackerAlgoRich2020::WriteOutputDigi fDigiVect.size=" << fDigiVect.size();
}

void CbmMcbm2018UnpackerAlgoRich2020::FinalizeTs()
{
  //          for (int i = 0; i < fDigiVect.size();++i) {
  //                 LOG(info) << "CbmMcbm2018UnpackerAlgoRich2020::Final Vector: "
  //                     << i+1 <<"/"<<fDigiVect.size()
  // 	            << "\t" << std::setprecision(15)<< fDigiVect[i].GetTime();
  //
  //
  //         }
  LOG(debug4) << "CbmMcbm2018UnpackerAlgoRich2020::FinalizeTs: " << fRisingEdgesBuf.size()
              << " entries in fRisingEdgesBuf"
              << "\t" << fFallingEdgesBuf.size() << " entries in fFallingEdgesBuf";

  // Clear rising edges buffer
  LOG(debug4) << "Rising edges: "
                 "----------------------------------------------------------";
  std::vector<CbmMcbmRichEdge>::iterator reIter = fRisingEdgesBuf.begin();
  while (reIter != fRisingEdgesBuf.end()) {
    LOG(debug4) << "FPGA=0x" << std::hex << (*reIter).fSubSubEventID << std::dec << "\tch=" << (*reIter).fChannel;
    ++reIter;
  }
  fRisingEdgesBuf.clear();

  // Clear falling edges buffer
  LOG(debug4) << "Falling edges: "
                 "---------------------------------------------------------";
  std::vector<CbmMcbmRichEdge>::iterator feIter = fFallingEdgesBuf.begin();
  while (feIter != fFallingEdgesBuf.end()) {
    LOG(debug4) << "FPGA=0x" << std::hex << (*feIter).fSubSubEventID << std::dec << "\tch=" << (*feIter).fChannel;
    ++feIter;
  }
  fFallingEdgesBuf.clear();

  LOG(debug4) << "---------------------------------------------------------";
}

Bool_t CbmMcbm2018UnpackerAlgoRich2020::CreateHistograms()
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

  fhTdcErrors = new TH2D("fhTdcErrors", "Errors in TDC msgs;;", nTDCs, -0.5, nTDCs - 0.5, 9, -0.5, 8.5);
  fhTdcErrors->GetYaxis()->SetBinLabel(1, "RingBuffOverw.");
  fhTdcErrors->GetYaxis()->SetBinLabel(2, "noRefTime");
  fhTdcErrors->GetYaxis()->SetBinLabel(3, "refTimePrecedes");
  fhTdcErrors->GetYaxis()->SetBinLabel(4, "trigW/oRefTime");
  fhTdcErrors->GetYaxis()->SetBinLabel(5, "markMisRefTime");
  fhTdcErrors->GetYaxis()->SetBinLabel(6, "multiRefTime");
  fhTdcErrors->GetYaxis()->SetBinLabel(7, "refTime<40ns");
  fhTdcErrors->GetYaxis()->SetBinLabel(8, "noValidation");
  fhTdcErrors->GetYaxis()->SetBinLabel(9, "trigger!=0x1");
  fhTdcErrors->GetXaxis()->LabelsOption("v");
  fhTdcErrors->GetYaxis()->SetTickSize(0.0);
  fhTdcErrors->GetXaxis()->SetTickSize(0.0);
  //fhTdcErrors->SetGrid();

  fhEventErrors = new TH2D("fhEventErrors", "Errors in Event/mts msgs;;", 1, -0.5, 0.5, 13, -0.5, 12.5);
  fhEventErrors->GetYaxis()->SetBinLabel(1, "UDPProblem");
  fhEventErrors->GetYaxis()->SetBinLabel(2, "evNumMism");
  fhEventErrors->GetYaxis()->SetBinLabel(3, "trigMism");
  fhEventErrors->GetYaxis()->SetBinLabel(4, "wrongLength");
  fhEventErrors->GetYaxis()->SetBinLabel(5, "answMissing");
  fhEventErrors->GetYaxis()->SetBinLabel(6, "evRequFail");
  fhEventErrors->GetYaxis()->SetBinLabel(7, "evPartFound");
  fhEventErrors->GetYaxis()->SetBinLabel(8, "sevBuffProb");
  fhEventErrors->GetYaxis()->SetBinLabel(9, "brokenEv");
  fhEventErrors->GetYaxis()->SetBinLabel(10, "ethLinkDwn");
  fhEventErrors->GetYaxis()->SetBinLabel(11, "subEvBuffAlmFull");
  fhEventErrors->GetYaxis()->SetBinLabel(12, "eth/BufProb");
  fhEventErrors->GetYaxis()->SetBinLabel(13, "timingTrigErr");
  fhEventErrors->GetXaxis()->LabelsOption("v");
  fhEventErrors->GetXaxis()->SetTickSize(0.0);
  fhEventErrors->GetYaxis()->SetTickSize(0.0);

  for (Int_t iTDC = 0; iTDC < nTDCs; iTDC++) {
    TString histoName;
    TString histoTitle;
    TString subFolder;

    Int_t Addr    = fUnpackPar->GetAddress(iTDC);
    fMapFEE[Addr] = iTDC;
    fhTdcErrors->GetXaxis()->SetBinLabel(iTDC + 1, Form("0x%4x", Addr));
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
    {  //if (iTDC == 0){
      Double_t w = 10;
      Double_t h = 10;

      TCanvas* c;
      TString canvasName;
      TString canvasTitle;
      Int_t tdc = fUnpackPar->GetAddress(iTDC);
      canvasName.Form("cToT2d_TDC_0x%4x", tdc);
      canvasTitle.Form("ToTs of TDC 0x%4x", tdc);
      c = new TCanvas(canvasName, canvasTitle, w, h);
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
      TH2D* h2 = GetTotH2(tdc);
      h2->Draw("colz");
      fcTot2d.push_back(c);
      AddCanvasToVector(c, "ToT_Canvases");
    }
    /*******************************************************************/
  }


  AddHistoToVector(fhTdcErrors, "");
  AddHistoToVector(fhEventErrors, "");

  fhVectorSize = new TH1I("fhVectorSize", "Size of the vector VS TS index; TS index; Size [bytes]", 10000, 0., 10000.);
  fhVectorCapacity =
    new TH1I("fhVectorCapacity", "Size of the vector VS TS index; TS index; Size [bytes]", 10000, 0., 10000.);
  AddHistoToVector(fhVectorSize, "");
  AddHistoToVector(fhVectorCapacity, "");

  if (fbDebugMonitorMode) {
    fhEventSize = new TH1I("fhEventSize", "Size of the Event from TrbNet; Size [bytes]", 350, 0., 70000.);
    AddHistoToVector(fhEventSize, "");

    fhSubEventSize =
      new TH2I("fhSubEventSize", "fhSubEventSize; HubId ; Size [bytes]; Entries", 6, 0, 6, 10000, 0., 10000.);
    AddHistoToVector(fhSubEventSize, "");

    fhSubSubEventSize =
      new TH2I("fhSubSubEventSize", "fhSubSubEventSize; DiRICH ; Size [words]; Entries", 72, 0, 72, 510, 0., 510.);
    AddHistoToVector(fhSubSubEventSize, "");

    fhChnlSize = new TH2I("fhChnlSize", "fhChnlSize; channel; Size [words]; Entries", 33, 0, 33, 25, 0, 25.);
    AddHistoToVector(fhChnlSize, "");
  }

  return kTRUE;
}

TH1D* CbmMcbm2018UnpackerAlgoRich2020::GetTotH1(Int_t tdc, Int_t channel)
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

TH2D* CbmMcbm2018UnpackerAlgoRich2020::GetTotH2(Int_t tdc)
{
  TH2D* h = fhTot2dMap[tdc];
  if (h == nullptr) {
    TString name, title, subFolder;
    name.Form("ToT_2d_tdc0x%x", tdc);
    title.Form("%s;channels;ToT [ns]", name.Data());
    subFolder.Form("ToT2d");
    h = new TH2D(name, title, 33, 0, 32, 200, -1., 49.);
    AddHistoToVector(h, std::string(subFolder.Data()));
    fhTot2dMap[tdc] = h;
  }
  return h;
}

Bool_t CbmMcbm2018UnpackerAlgoRich2020::DebugMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
{
  const fles::MicrosliceView mv            = ts.get_microslice(uMsCompIdx, uMsIdx);
  const fles::MicrosliceDescriptor& msDesc = mv.desc();
  const uint8_t* ptr                       = mv.content();
  const size_t size                        = msDesc.size;

  if (size == 0) return kTRUE;
  Debug(ptr, size);

  return kTRUE;
}

Int_t CbmMcbm2018UnpackerAlgoRich2020::Debug(const uint8_t* ptr, const size_t size)
{

  if (size == 0) return size;

  //LOG(info)<<"DEBUG MODE IS ACTIVE; Printing raw data:";

  uint8_t nblCnt = 0;
  uint8_t wrdCnt = 0;
  std::cout << std::endl << "SIZE: " << std::dec << size << "Byte" << std::endl;
  for (size_t i = 0; i < size; ++i) {

    //if (wrdCnt == 0) std::cout<<"HEX: ";
    uint8_t* tdcDataPtr = (uint8_t*) (ptr + i);

    if (wrdCnt == 0 && nblCnt == 0) { printf("%08d : ", static_cast<int>(i)); }

    printf("%02x", unsigned(*tdcDataPtr));
    nblCnt++;
    if (nblCnt % 2 == 0) { printf(" "); }
    if (nblCnt % 4 == 0) {
      printf("  ");
      wrdCnt++;
      nblCnt = 0;
    }

    if (wrdCnt == 10) {
      printf("\n");
      wrdCnt = 0;
    }
  }
  printf("\n");
  return size;
}


void CbmMcbm2018UnpackerAlgoRich2020::ErrorMsg(uint16_t errbits, RichErrorType type, uint16_t tdcAddr)
{
  if (fbMonitorMode) {
    switch (type) {
      case RichErrorType::mtsError:
        //UDP problem
        if ((errbits & 0x1) == 1) fhEventErrors->Fill(0.0, 0.0);

        break;

      case RichErrorType::tdcHeader:
        // min. 1 rinǵ buffer overwritten
        if ((errbits & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcAddr], 0.0);

        break;

      case RichErrorType::tdcTrailer:
        // no reference time in trigger handler in TDC
        if (((errbits >> 0) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcAddr], 1.0);

        // reference time precedes a non-timing trigger
        if (((errbits >> 1) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcAddr], 2.0);

        // timing trigger is delivered without a reference time
        if (((errbits >> 2) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcAddr], 3.0);

        // Set with the bit 2 to mark the missing reference time
        if (((errbits >> 3) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcAddr], 4.0);

        // there are more than one detected reference time
        if (((errbits >> 4) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcAddr], 5.0);

        // reference time was too short (<40 ns)
        if (((errbits >> 5) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcAddr], 6.0);

        // no trigger validation arrives from the endpoint after a valid  reference time
        if (((errbits >> 6) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcAddr], 7.0);

        // any timing trigger type except 0x1 is send
        if (((errbits >> 7) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcAddr], 8.0);

        break;

      case RichErrorType::ctsHeader:
        // To be implemented
        break;

      case RichErrorType::ctsTrailer:
        // To be implemented
        break;

      case RichErrorType::subEventError:
        // event number mismatch
        if (((errbits >> 0) & 0x1) == 1) fhEventErrors->Fill(0.0, 1.0);

        // trigger code mismatch
        if (((errbits >> 1) & 0x1) == 1) fhEventErrors->Fill(0.0, 2.0);

        // wrong length
        if (((errbits >> 2) & 0x1) == 1) fhEventErrors->Fill(0.0, 3.0);

        // answer missing
        if (((errbits >> 3) & 0x1) == 1) fhEventErrors->Fill(0.0, 4.0);

        // event number request by CTS was not available (Not found)
        if (((errbits >> 4) & 0x1) == 1) fhEventErrors->Fill(0.0, 5.0);

        // event partially found in data buffer
        if (((errbits >> 5) & 0x1) == 1) fhEventErrors->Fill(0.0, 6.0);

        // Severe Problem with data buffer and/or read-out
        if (((errbits >> 6) & 0x1) == 1) fhEventErrors->Fill(0.0, 7.0);

        // Single broken event
        if (((errbits >> 7) & 0x1) == 1) fhEventErrors->Fill(0.0, 8.0);

        // Ethernet Link down
        if (((errbits >> 8) & 0x1) == 1) fhEventErrors->Fill(0.0, 9.0);

        // SubEvent buffer almost full
        if (((errbits >> 9) & 0x1) == 1) fhEventErrors->Fill(0.0, 10.0);

        // Ethernet/SubEventBuilder error
        if (((errbits >> 10) & 0x1) == 1) fhEventErrors->Fill(0.0, 11.0);

        // Timing trigger error
        if (((errbits >> 11) & 0x1) == 1) fhEventErrors->Fill(0.0, 12.0);

        break;

      default: break;
    }
  }
}

/*
Bool_t CbmMcbm2018UnpackerAlgoRich2020::FillHistograms()
{
	return kTRUE;
}
*/
Bool_t CbmMcbm2018UnpackerAlgoRich2020::ResetHistograms()
{
  //TODO: do something?
  return kTRUE;
}

void CbmMcbm2018UnpackerAlgoRich2020::findTDCAlignmentError(uint8_t const* const ptr, size_t const size)
{

  fTDCAlignmentErrorPositions.clear();

  //     mRichSupport::SwapBytes(4, ptr+size);
  //     if((((((Int_t*)(ptr+size))[0]) >> 28) & 0xF) != 0x0) {
  //         LOG(WARN) << "CbmMcbm2018UnpackerAlgoRich2020::ProcessTRBsubevent() warning:"
  //                   << "End on Hub is not where expected. Is it a Buffer overflow?  LastWord: "<<mRichSupport::GetWordHexRepr(ptr+size);
  //     }
  //     mRichSupport::SwapBytes(4, ptr+size);

  /***
	 * Signature of Error:
	 *  82b7   8ca6
	 *  8297  *34ad*
	 * *34ad*  66af    // data Ptr
	 * *cf8b* *cf8b*
	 *  82c8   cca9
	 */

  //start at 8 to skip header of Hub and first row as this has to be checked
  //stop at size -4 to avoid comparing with following hub

  for (Int_t i = 8; i < static_cast<Int_t>(size - 4); i += 4) {  // i represents bytes (4 per line)
    //TODO: Optimize the swaping
    mRichSupport::SwapBytes(4, ptr + i - 4);
    mRichSupport::SwapBytes(4, ptr + i);
    mRichSupport::SwapBytes(4, ptr + i + 4);
    bool problem = false;
    // clang-format off
    if ((((Int_t*) (ptr + i - 4))[0] & 0xFFFF) == ((((Int_t*) (ptr + i))[0] >> 16) & 0xFFFF)) {
      if ((((Int_t*) (ptr + i + 4))[0] & 0xFFFF) == ((((Int_t*) (ptr + i + 4))[0] >> 16) & 0xFFFF)) {
        //Signature of problem!
        problem = true;
        fTDCAlignmentErrorPositions.push_back(i);
        fTDCAlignmentErrorPositions.push_back(i + 6);
      }
    }
    // clang-format on

    mRichSupport::SwapBytes(4, ptr + i - 4);
    mRichSupport::SwapBytes(4, ptr + i);
    mRichSupport::SwapBytes(4, ptr + i + 4);

    if (problem) i += 8;  //jump after the problem
  }
}

ClassImp(CbmMcbm2018UnpackerAlgoRich2020)
