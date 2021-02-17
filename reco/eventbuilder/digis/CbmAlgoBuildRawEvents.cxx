/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmAlgoBuildRawEvents.h"

/// CBM headers
#include "CbmEvent.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

#include "TimesliceMetaData.h"

/// FAIRROOT headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"

Bool_t CbmAlgoBuildRawEvents::InitAlgo()
{
  LOG(info) << "CbmAlgoBuildRawEvents::InitAlgo => Starting sequence";

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  /// Check if reference detector data are available
  if (kFALSE == CheckDataAvailable(fRefDet)) { LOG(fatal) << "No digi input for reference detector, stopping there!"; }

  /// Check if data for detectors in selection list are available
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if (kFALSE == CheckDataAvailable(*det)) {
      LOG(fatal) << "No digi input for one of selection detector, stopping there!";
    }
  }

  /// Access the TS metadata to know TS start time if needed
  if (fdTsStartTime < 0 || fdTsLength < 0 || fdTsOverLength < 0) {
    fTimeSliceMetaDataArray = dynamic_cast<TClonesArray*>(ioman->GetObject("TimesliceMetaData"));
    if (!fTimeSliceMetaDataArray) {
      LOG(fatal) << "No TS metadata input found"
                 << " => Please check in the unpacking macro if the following line was "
                    "present!"
                 << std::endl
                 << "source->SetWriteOutputFlag(kTRUE);  // For writing TS metadata";
    }
  }

  if (fbFillHistos) { CreateHistograms(); }
  LOG(info) << "CbmAlgoBuildRawEvents::InitAlgo => Done";
  return kTRUE;
}

void CbmAlgoBuildRawEvents::Finish() { LOG(info) << "Total errors: " << fuErrors; }

void CbmAlgoBuildRawEvents::ClearEventVector()
{
  /// Need to delete the object the pointer points to first
  int counter = 0;
  for (CbmEvent* event : fEventVector) {
    LOG(debug) << "Event " << counter << " has " << event->GetNofData() << " digis";
    delete event;
    counter++;
  }
  fEventVector.clear();
}

void CbmAlgoBuildRawEvents::ProcessTs()
{
  LOG_IF(info, fuNrTs % 1000 == 0) << "Begin of TS " << fuNrTs;
  InitTs();
  InitSeedWindow();
  BuildEvents();

  /// Store last event with trigger if not done by other seed
  if (nullptr != fCurrentEvent) {
    /// TODO: store start time of current event ?
    //        fCurrentEvent->SetStartTime( fPrevTime ); // Replace Seed time with time of first digi in event?
    fCurrentEvent->SetEndTime(fdPrevEvtEndTime);
    fEventVector.push_back(fCurrentEvent);
    fuCurEv++;

    /// Prevent building over TS edge
    fCurrentEvent = nullptr;
  }

  LOG(debug) << "Found " << fEventVector.size() << " triggered events";
  if (fbFillHistos) { FillHistos(); }
  fuNrTs++;
}

void CbmAlgoBuildRawEvents::InitTs()
{
  /// Reset TS based variables (analysis per TS = no building over the border)
  /// Reference detector
  fRefDet.fuStartIndex = 0;
  fRefDet.fuEndIndex   = 0;
  /// Loop on detectors in selection list
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    (*det).fuStartIndex = 0;
    (*det).fuEndIndex   = 0;
  }
}

void CbmAlgoBuildRawEvents::InitSeedWindow()
{
  /// Access the TS metadata if needed to know TS start time and overlap size
  Double_t dTsStartTime  = fdTsStartTime;
  Double_t dOverlapStart = fdTsStartTime + fdTsLength;
  Double_t dOverlapSize  = fdTsOverLength;

  if (fdTsStartTime < 0 || fdTsLength < 0 || fdTsOverLength < 0) {
    pTsMetaData = dynamic_cast<TimesliceMetaData*>(fTimeSliceMetaDataArray->At(0));
    if (nullptr == pTsMetaData)
      LOG(fatal) << Form("CbmAlgoBuildRawEvents::LoopOnSeeds => "
                         "No TS metadata found for TS %6u.",
                         fuNrTs);
    dTsStartTime  = pTsMetaData->GetStartTime();
    dOverlapStart = pTsMetaData->GetOverlapStartTime();
    dOverlapSize  = pTsMetaData->GetOverlapDuration();
  }

  /// Print warning in first TS if time window borders out of potential overlap
  if ((0.0 < fdEarliestTimeWinBeg && dOverlapSize < fdLatestTimeWinEnd) || (dOverlapSize < fdWidestTimeWinRange)) {
    LOG(warning) << "CbmAlgoBuildRawEvents::LoopOnSeeds => "
                 << Form("Event window not fitting in TS overlap, risk of "
                         "incomplete events: %f %f %f %f",
                         fdEarliestTimeWinBeg, fdLatestTimeWinEnd, fdWidestTimeWinRange, dOverlapSize);
  }  // if end of event window does not fit in overlap for a seed at edge of TS core

  /// Define an acceptance window for the seeds in order to use the overlap
  /// part of the TS to avoid incomplete events
  if (fbIgnoreTsOverlap) {
    fdSeedWindowBeg = dTsStartTime;
    fdSeedWindowEnd = dOverlapStart;
  }
  else {
    fdSeedWindowBeg = dTsStartTime + (0.0 < fdEarliestTimeWinBeg ? 0.0 : -fdEarliestTimeWinBeg);
    fdSeedWindowEnd = dOverlapStart + (0.0 < fdEarliestTimeWinBeg ? 0.0 : -fdEarliestTimeWinBeg);
  }
}

void CbmAlgoBuildRawEvents::BuildEvents()
{
  /// Call LoopOnSeed with proper template argument
  switch (fRefDet.detId) {
    case ECbmModuleId::kSts: {
      LoopOnSeeds<CbmStsDigi>();
      break;
    }
    case ECbmModuleId::kMuch: {
      if (fbUseMuchBeamtimeDigi) { LoopOnSeeds<CbmMuchBeamTimeDigi>(); }
      else {
        LoopOnSeeds<CbmMuchDigi>();
      }
      break;
    }
    case ECbmModuleId::kTrd: {
      LoopOnSeeds<CbmTrdDigi>();
      break;
    }
    case ECbmModuleId::kTof: {
      LoopOnSeeds<CbmTofDigi>();
      break;
    }
    case ECbmModuleId::kRich: {
      LoopOnSeeds<CbmRichDigi>();
      break;
    }
    case ECbmModuleId::kPsd: {
      LoopOnSeeds<CbmPsdDigi>();
      break;
    }
    case ECbmModuleId::kT0: {
      LoopOnSeeds<CbmTofDigi>();
      break;
    }
    default: {
      LOG(fatal) << "CbmAlgoBuildRawEvents::BuildEvents => "
                 << "Trying to search event seeds with unsupported det: " << fRefDet.sName;
      break;
    }
  }  // switch( *det )
}

template<class DigiSeed>
void CbmAlgoBuildRawEvents::LoopOnSeeds()
{
  if (ECbmModuleId::kT0 == fRefDet.detId) {
    const UInt_t uNbRefDigis = fT0DigiVec->size();
    /// Loop on size of vector
    for (UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi) {
      LOG(debug) << Form("Checking seed %6u / %6u", uDigi, uNbRefDigis);
      Double_t dTime = fT0DigiVec->at(uDigi).GetTime();
      /// Check Seed and build event if needed
      CheckSeed(dTime, uDigi);
    }
  }
  else {
    const UInt_t uNbRefDigis = (0 < GetNofDigis(fRefDet.detId) ? GetNofDigis(fRefDet.detId) : 0);
    /// Loop on size of vector
    for (UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi) {
      LOG(debug) << Form("Checking seed %6u / %6u", uDigi, uNbRefDigis);
      const DigiSeed* pDigi = GetDigi<DigiSeed>(uDigi);
      const Double_t dTime  = pDigi->GetTime();

      /// Check if seed in acceptance window
      if (dTime < fdSeedWindowBeg) { continue; }
      else if (fdSeedWindowEnd < dTime) {
        break;
      }
      /// Check Seed and build event if needed
      CheckSeed(dTime, uDigi);
    }
  }
}

void CbmAlgoBuildRawEvents::CheckSeed(Double_t dSeedTime, UInt_t uSeedDigiIdx)
{
  /// If previous event valid and event overlap not allowed, check if we are in overlap
  /// and react accordingly
  if (nullptr != fCurrentEvent
      && (EOverlapModeRaw::AllowOverlap != fOverMode || dSeedTime - fdPrevEvtTime < fRefDet.GetTimeWinRange())
      && dSeedTime - fdPrevEvtTime < fdWidestTimeWinRange) {
    /// Within overlap range
    switch (fOverMode) {
      case EOverlapModeRaw::NoOverlap: {
        /// No overlap allowed => reject
        LOG(debug1) << "Reject seed due to overlap";
        return;
        break;
      }
      case EOverlapModeRaw::MergeOverlap: {
        /// Merge overlap mode => do nothing and go on filling current event
        break;
      }
      case EOverlapModeRaw::AllowOverlap: {
        /// In allow overlap mode => reject only if reference det is in overlap
        /// to avoid cloning events due to single seed cluster
        LOG(debug1) << "Reject seed because part of cluster of previous one";
        return;
        break;
      }
    }
  }  // if( prev Event exists and mode forbiden overlap present )
  else {
    /// Out of overlap range or in overlap allowed mode
    /// => store previous event if not empty and create new one
    if (nullptr != fCurrentEvent) {
      /// TODO: store start time of current event ?
      //        fCurrentEvent->SetStartTime( fPrevTime ); // Replace Seed time with time of first digi in event?
      fCurrentEvent->SetEndTime(fdPrevEvtEndTime);
      fEventVector.push_back(fCurrentEvent);
      fuCurEv++;
    }
    fCurrentEvent = new CbmEvent(fuCurEv, dSeedTime, 0.);
  }  // else of if( prev Event exists and mode forbiden overlap present )

  /// If window open for reference detector, search for other reference Digis matching it
  /// Otherwise only add the current seed
  if (fRefDet.fdTimeWinBeg < fRefDet.fdTimeWinEnd) {
    SearchMatches(dSeedTime, fRefDet);
    /// Also add the seed if the window starts after the seed
    if (0 < fRefDet.fdTimeWinBeg) AddDigiToEvent(fRefDet, uSeedDigiIdx);
  }
  else {
    AddDigiToEvent(fRefDet, uSeedDigiIdx);
  }

  /// Search for matches for each detector in selection list
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    SearchMatches(dSeedTime, *det);
  }

  CheckTriggerCondition(dSeedTime);
}

void CbmAlgoBuildRawEvents::SearchMatches(Double_t dSeedTime, RawEventBuilderDetector& detMatch)
{
  switch (detMatch.detId) {
    case ECbmModuleId::kSts: {
      SearchMatches<CbmStsDigi>(dSeedTime, detMatch);
      break;
    }
    case ECbmModuleId::kMuch: {
      if (fbUseMuchBeamtimeDigi) { SearchMatches<CbmMuchBeamTimeDigi>(dSeedTime, detMatch); }
      else {
        SearchMatches<CbmMuchDigi>(dSeedTime, detMatch);
      }
      break;
    }
    case ECbmModuleId::kTrd: {
      SearchMatches<CbmTrdDigi>(dSeedTime, detMatch);
      break;
    }
    case ECbmModuleId::kTof: {
      SearchMatches<CbmTofDigi>(dSeedTime, detMatch);
      break;
    }
    case ECbmModuleId::kRich: {
      SearchMatches<CbmRichDigi>(dSeedTime, detMatch);
      break;
    }
    case ECbmModuleId::kPsd: {
      SearchMatches<CbmPsdDigi>(dSeedTime, detMatch);
      break;
    }
    case ECbmModuleId::kT0: {
      SearchMatches<CbmTofDigi>(dSeedTime, detMatch);
      break;
    }
    default: {
      LOG(fatal) << "CbmAlgoBuildRawEvents::LoopOnSeeds => "
                 << "Trying to search matches with unsupported det: " << detMatch.sName << std::endl
                 << "You may want to add support for it in the method.";
      break;
    }
  }
}

template<class DigiCheck>
void CbmAlgoBuildRawEvents::SearchMatches(Double_t dSeedTime, RawEventBuilderDetector& detMatch)
{
  /// This algo relies on time sorted vectors for the selected detectors
  UInt_t uLocalIndexStart = detMatch.fuStartIndex;
  UInt_t uLocalIndexEnd   = detMatch.fuStartIndex;

  /// Check the Digis until out of window
  if (ECbmModuleId::kT0 == detMatch.detId) {

    /// Loop on size of vector
    const UInt_t uNbSelDigis = fT0DigiVec->size();
    for (UInt_t uDigi = detMatch.fuStartIndex; uDigi < uNbSelDigis; ++uDigi) {
      const Double_t dTime     = fT0DigiVec->at(uDigi).GetTime();
      const Double_t dTimeDiff = dTime - dSeedTime;

      /// Check if within time window, update start/stop indices if needed
      if (dTimeDiff < detMatch.fdTimeWinBeg) {
        ++uLocalIndexStart;
        continue;
      }
      else if (detMatch.fdTimeWinEnd < dTimeDiff) {
        /// Store as end the first digi out of window to avoid double counting in case of
        /// merged overlap event mode
        uLocalIndexEnd = uDigi;
        break;
      }
      AddDigiToEvent(detMatch, uDigi);
      if (fdPrevEvtEndTime < dTime) fdPrevEvtEndTime = dTime;
    }

    /// catch the case where we reach the end of the vector before being out of the time window
    if (uLocalIndexEnd < uLocalIndexStart) uLocalIndexEnd = uNbSelDigis;
  }
  else {
    const UInt_t uNbSelDigis = (0 < GetNofDigis(detMatch.detId) ? GetNofDigis(detMatch.detId) : 0);
    /// Loop on size of vector
    for (UInt_t uDigi = detMatch.fuStartIndex; uDigi < uNbSelDigis; ++uDigi) {
      const DigiCheck* pDigi   = GetDigi<DigiCheck>(uDigi);
      const Double_t dTime     = pDigi->GetTime();
      const Double_t dTimeDiff = dTime - dSeedTime;
      LOG(debug4) << detMatch.sName << Form(" => Checking match %6u / %6u, dt %f", uDigi, uNbSelDigis, dTimeDiff);

      /// Check if within time window, update start/stop indices if needed
      if (dTimeDiff < detMatch.fdTimeWinBeg) {
        ++uLocalIndexStart;
        continue;
      }
      else if (detMatch.fdTimeWinEnd < dTimeDiff) {
        /// Store as end the first digi out of window to avoid double counting in case of
        /// merged overlap event mode
        uLocalIndexEnd = uDigi;
        break;
      }
      AddDigiToEvent(detMatch, uDigi);
      if (fdPrevEvtEndTime < dTime) fdPrevEvtEndTime = dTime;
    }
    /// catch the case where we reach the end of the vector before being out of the time window
    if (uLocalIndexEnd < uLocalIndexStart) uLocalIndexEnd = uNbSelDigis;
  }

  /// Update the StartIndex and EndIndex for the next event seed
  detMatch.fuStartIndex = uLocalIndexStart;
  detMatch.fuEndIndex   = uLocalIndexEnd;
}

void CbmAlgoBuildRawEvents::AddDigiToEvent(RawEventBuilderDetector& det, Int_t _entry)
{
  fCurrentEvent->AddData(det.dataType, _entry);
}

void CbmAlgoBuildRawEvents::CheckTriggerCondition(Double_t dSeedTime)
{
  /// Check if event is filling trigger conditions and clear it if not
  if (HasTrigger(fCurrentEvent)) {
    fdPrevEvtTime = dSeedTime;

    /// In case of NoOverlap or MergeOverlap, we can and should start checking the next window
    /// from end of current window in order to save CPU and avoid duplicating
    if (EOverlapModeRaw::NoOverlap == fOverMode || EOverlapModeRaw::MergeOverlap == fOverMode) {
      /// Update reference detector
      fRefDet.fuStartIndex = fRefDet.fuEndIndex;
      /// Loop on selection detectors
      for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
        (*det).fuStartIndex = (*det).fuEndIndex;
      }
    }
  }
  else {
    LOG(debug1) << "Reject seed due to Trigger requirements";
    delete fCurrentEvent;
    fCurrentEvent = nullptr;  /// delete does NOT set a pointer to nullptr...
  }
}

Bool_t CbmAlgoBuildRawEvents::HasTrigger(CbmEvent* event)
{
  /// Check first reference detector
  if (kFALSE == CheckTriggerConditions(event, fRefDet)) { return kFALSE; }
  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if (kFALSE == CheckTriggerConditions(event, *det)) return kFALSE;
  }
  /// All Ok, trigger is there
  return kTRUE;
}

Bool_t CbmAlgoBuildRawEvents::CheckTriggerConditions(CbmEvent* event, RawEventBuilderDetector& det)
{
  /// Check if both Trigger conditions disabled for this detector
  if (0 == det.fuTriggerMinDigis && det.fiTriggerMaxDigis < 0) { return kTRUE; }

  /// Check if detector present
  if (ECbmModuleId::kT0 == det.detId) {
    /// FIXME: special case to be removed once T0 supported by DigiManager
    if (!(fT0DigiVec)) {
      LOG(warning) << "Event does not have digis storage for T0"
                   << " while the following trigger minimum are defined: " << det.fuTriggerMinDigis << " "
                   << det.fiTriggerMaxDigis;
      return kFALSE;
    }
  }
  else {
    if (!DetIsPresent(det.detId)) {
      LOG(warning) << "Event does not have digis storage for " << det.sName
                   << " while the following trigger min/max are defined: " << det.fuTriggerMinDigis << " "
                   << det.fiTriggerMaxDigis;
      return kFALSE;
    }
  }

  /// Check trigger rejection by minimal number or absence
  const Int_t iNbDigis = event->GetNofData(det.dataType);
  if ((-1 == iNbDigis) || (static_cast<UInt_t>(iNbDigis) < det.fuTriggerMinDigis)) {
    LOG(debug2) << "Event does not have enough digis: " << iNbDigis << " vs " << det.fuTriggerMinDigis << " for "
                << det.sName;
    return kFALSE;
  }

  /// Check trigger rejection by maximal number
  else if (0 < det.fiTriggerMaxDigis && det.fiTriggerMaxDigis < iNbDigis) {
    LOG(debug2) << "Event Has too many digis: " << iNbDigis << " vs " << det.fiTriggerMaxDigis << " for " << det.sName;
    return kFALSE;
  }
  else {
    return kTRUE;
  }
}

//----------------------------------------------------------------------

Bool_t CbmAlgoBuildRawEvents::CheckDataAvailable(RawEventBuilderDetector& det)
{
  if (ECbmModuleId::kT0 == det.detId) {
    if (!fT0DigiVec) {
      LOG(info) << "No T0 digi input found.";
      return kFALSE;
    }
  }
  else if (ECbmModuleId::kSts == det.detId) {
    if (!fStsDigis) {
      LOG(info) << "No " << det.sName << " digi input found.";
      return kFALSE;
    }
  }
  else if (ECbmModuleId::kMuch == det.detId) {
    if (!fMuchDigis && !fMuchBeamTimeDigis) {
      LOG(info) << "No " << det.sName << " digi input found.";
      return kFALSE;
    }
  }
  else if (ECbmModuleId::kTrd == det.detId) {
    if (!fTrdDigis) {
      LOG(info) << "No " << det.sName << " digi input found.";
      return kFALSE;
    }
  }
  else if (ECbmModuleId::kTof == det.detId) {
    if (!fTofDigis) {
      LOG(info) << "No " << det.sName << " digi input found.";
      return kFALSE;
    }
  }
  else if (ECbmModuleId::kRich == det.detId) {
    if (!fRichDigis) {
      LOG(info) << "No " << det.sName << " digi input found.";
      return kFALSE;
    }
  }
  else if (ECbmModuleId::kPsd == det.detId) {
    if (!fPsdDigis) {
      LOG(info) << "No " << det.sName << " digi input found.";
      return kFALSE;
    }
  }
  return kTRUE;
}

UInt_t CbmAlgoBuildRawEvents::GetNofDigis(ECbmModuleId detId)
{
  switch (detId) {
    case ECbmModuleId::kSts: {
      return fStsDigis->size();
    }
    case ECbmModuleId::kMuch: {
      if (fbUseMuchBeamtimeDigi) { return fMuchBeamTimeDigis->size(); }
      else {
        return fMuchDigis->size();
      }
    }
    case ECbmModuleId::kTrd: {
      return fTrdDigis->size();
    }
    case ECbmModuleId::kTof: {
      return fTofDigis->size();
    }
    case ECbmModuleId::kRich: {
      return fRichDigis->size();
    }
    case ECbmModuleId::kPsd: {
      return fPsdDigis->size();
    }
    case ECbmModuleId::kT0: {
      return fT0DigiVec->size();  //what to do here? Not in digi manager.
    }
    default: {
      LOG(fatal) << "CbmAlgoBuildRawEvents::GetNofDigis => "
                 << "Trying to get digi number with unsupported detector.";
      return -1;
    }
  }
}

bool CbmAlgoBuildRawEvents::DetIsPresent(ECbmModuleId detId)
{
  switch (detId) {
    case ECbmModuleId::kSts: {
      return fStsDigis != nullptr;
    }
    case ECbmModuleId::kMuch: {
      if (fbUseMuchBeamtimeDigi) { return fMuchBeamTimeDigis != nullptr; }
      else {
        return fMuchDigis != nullptr;
      }
    }
    case ECbmModuleId::kTrd: {
      return fTrdDigis != nullptr;
    }
    case ECbmModuleId::kTof: {
      return fTofDigis != nullptr;
    }
    case ECbmModuleId::kRich: {
      return fRichDigis != nullptr;
    }
    case ECbmModuleId::kPsd: {
      return fPsdDigis != nullptr;
    }
    case ECbmModuleId::kT0: {
      return fT0DigiVec != nullptr;  //what to do here? Not in digi manager.
    }
    default: {
      LOG(fatal) << "CbmAlgoBuildRawEvents::GetNofDigis => "
                 << "Trying to get digi number with unsupported detector.";
      return -1;
    }
  }
}

template<>
const CbmStsDigi* CbmAlgoBuildRawEvents::GetDigi(UInt_t uDigi)
{
  return &((*fStsDigis)[uDigi]);
}
template<>
const CbmMuchBeamTimeDigi* CbmAlgoBuildRawEvents::GetDigi(UInt_t uDigi)
{
  return &((*fMuchBeamTimeDigis)[uDigi]);
}
template<>
const CbmMuchDigi* CbmAlgoBuildRawEvents::GetDigi(UInt_t uDigi)
{
  return &((*fMuchDigis)[uDigi]);
}
template<>
const CbmTrdDigi* CbmAlgoBuildRawEvents::GetDigi(UInt_t uDigi)
{
  return &((*fTrdDigis)[uDigi]);
}
template<>
const CbmTofDigi* CbmAlgoBuildRawEvents::GetDigi(UInt_t uDigi)
{
  return &((*fTofDigis)[uDigi]);
}
template<>
const CbmRichDigi* CbmAlgoBuildRawEvents::GetDigi(UInt_t uDigi)
{
  return &((*fRichDigis)[uDigi]);
}
template<>
const CbmPsdDigi* CbmAlgoBuildRawEvents::GetDigi(UInt_t uDigi)
{
  return &((*fPsdDigis)[uDigi]);
}

//----------------------------------------------------------------------
void CbmAlgoBuildRawEvents::CreateHistograms()
{
  fhEventTime = new TH1F("hEventTime", "seed time of the events; Seed time [s]; Events", 60000, 0, 600);
  fhEventDt =
    new TH1F("fhEventDt", "interval in seed time of consecutive events; Seed time [s]; Events", 2100, -100.5, 1999.5);
  fhEventSize        = new TH1F("hEventSize", "nb of all  digis in the event; Nb Digis []; Events []", 10000, 0, 10000);
  fhNbDigiPerEvtTime = new TH2I("hNbDigiPerEvtTime",
                                "nb of all  digis per event vs seed time of the events; Seed time "
                                "[s]; Nb Digis []; Events []",
                                600, 0, 600, 1000, 0, 10000);

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    /// In case name not provided, do not create the histo to avoid name conflicts!
    if ("Invalid" == (*det).sName) {
      fvhNbDigiPerEvtTimeDet.push_back(nullptr);
      continue;
    }

    fvhNbDigiPerEvtTimeDet.push_back(new TH2I(Form("hNbDigiPerEvtTime%s", (*det).sName.data()),
                                              Form("nb of %s digis per event vs seed time of the events; Seed time "
                                                   "[s]; Nb Digis []; Events []",
                                                   (*det).sName.data()),
                                              600, 0, 600, 4000, 0, 4000));
  }

  AddHistoToVector(fhEventTime, "evtbuild");
  AddHistoToVector(fhEventDt, "evtbuild");
  AddHistoToVector(fhEventSize, "evtbuild");
  AddHistoToVector(fhNbDigiPerEvtTime, "evtbuild");
  for (std::vector<TH2*>::iterator itHist = fvhNbDigiPerEvtTimeDet.begin(); itHist != fvhNbDigiPerEvtTimeDet.end();
       ++itHist) {
    if (nullptr != (*itHist)) { AddHistoToVector((*itHist), "evtbuild"); }
  }
}

void CbmAlgoBuildRawEvents::FillHistos()
{
  Double_t dPreEvtTime = -1.0;
  for (CbmEvent* evt : fEventVector) {
    fhEventTime->Fill(evt->GetStartTime() * 1e-9);
    if (0.0 <= dPreEvtTime) { fhEventDt->Fill(evt->GetStartTime() - dPreEvtTime); }
    fhEventSize->Fill(evt->GetNofData());
    fhNbDigiPerEvtTime->Fill(evt->GetStartTime() * 1e-9, evt->GetNofData());

    /// Loop on selection detectors
    for (UInt_t uDetIdx = 0; uDetIdx < fvDets.size(); ++uDetIdx) {
      if (nullptr == fvhNbDigiPerEvtTimeDet[uDetIdx]) continue;
      fvhNbDigiPerEvtTimeDet[uDetIdx]->Fill(evt->GetStartTime() * 1e-9, evt->GetNofData(fvDets[uDetIdx].dataType));
    }
    dPreEvtTime = evt->GetStartTime();
  }
}

void CbmAlgoBuildRawEvents::ResetHistograms(Bool_t /*bResetTime*/)
{
  fhEventTime->Reset();
  fhEventDt->Reset();
  fhEventSize->Reset();

  fhNbDigiPerEvtTime->Reset();
  /// Loop on histograms
  for (std::vector<TH2*>::iterator itHist = fvhNbDigiPerEvtTimeDet.begin(); itHist != fvhNbDigiPerEvtTimeDet.end();
       ++itHist) {
    (*itHist)->Reset();
  }
  /*
   if( kTRUE == bResetTime )
   {
      /// Also reset the Start time for the evolution plots!
      fdStartTime = -1.0;
   } 
   */
}

void CbmAlgoBuildRawEvents::SetReferenceDetector(ECbmModuleId refDet, ECbmDataType dataTypeIn, std::string sNameIn,
                                                 UInt_t uTriggerMinDigisIn, Int_t iTriggerMaxDigisIn,
                                                 Double_t fdTimeWinBegIn, Double_t fdTimeWinEndIn)
{
  /// FIXME: Deprecated method to be removed later. For now create temp object.
  SetReferenceDetector(RawEventBuilderDetector(refDet, dataTypeIn, sNameIn, uTriggerMinDigisIn, iTriggerMaxDigisIn,
                                               fdTimeWinBegIn, fdTimeWinEndIn));
}
void CbmAlgoBuildRawEvents::AddDetector(ECbmModuleId selDet, ECbmDataType dataTypeIn, std::string sNameIn,
                                        UInt_t uTriggerMinDigisIn, Int_t iTriggerMaxDigisIn, Double_t fdTimeWinBegIn,
                                        Double_t fdTimeWinEndIn)
{
  /// FIXME: Deprecated method to be removed later. For now create temp object.
  AddDetector(RawEventBuilderDetector(selDet, dataTypeIn, sNameIn, uTriggerMinDigisIn, iTriggerMaxDigisIn,
                                      fdTimeWinBegIn, fdTimeWinEndIn));
}

void CbmAlgoBuildRawEvents::SetReferenceDetector(RawEventBuilderDetector refDetIn)
{
  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if ((*det) == refDetIn) {
      LOG(warning) << "CbmAlgoBuildRawEvents::SetReferenceDetector => "
                      "Reference detector already in selection detector list!"
                   << refDetIn.sName;
      LOG(warning) << "                                                         => "
                      "It will be automatically removed from selection detector list!";
      LOG(warning) << "                                                         => "
                      "Please also remember to update the selection windows to store "
                      "clusters!";
      RemoveDetector(refDetIn);
    }
  }

  if (fRefDet == refDetIn) {
    LOG(warning) << "CbmAlgoBuildRawEvents::SetReferenceDetector => "
                    "Doing nothing, identical reference detector already in use";
  }
  else {
    LOG(info) << "CbmAlgoBuildRawEvents::SetReferenceDetector => "
              << "Replacing " << fRefDet.sName << " with " << refDetIn.sName << " as reference detector";
    LOG(warning) << "                                                         => "
                    "You may want to use AddDetector after this command to add in "
                    "selection "
                 << refDetIn.sName;
    LOG(warning) << "                                                         => "
                    "Please also remember to update the selection windows!";
  }
  fRefDet = refDetIn;

  /// Update the variables storing the earliest and latest time window boundaries
  UpdateTimeWinBoundariesExtrema();
  /// Update the variable storing the size if widest time window for overlap detection
  UpdateWidestTimeWinRange();
}

void CbmAlgoBuildRawEvents::AddDetector(RawEventBuilderDetector selDet)
{
  if (fRefDet == selDet) {
    LOG(fatal) << "CbmAlgoBuildRawEvents::AddDetector => Cannot "
                  "add the reference detector as selection detector!"
               << std::endl
               << "=> Maybe first change the reference detector with "
                  "SetReferenceDetector?";
  }

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if ((*det) == selDet) {
      LOG(warning) << "CbmAlgoBuildRawEvents::AddDetector => "
                      "Doing nothing, selection detector already in list!"
                   << selDet.sName;
      return;
    }
  }
  fvDets.push_back(selDet);

  /// Update the variables storing the earliest and latest time window boundaries
  UpdateTimeWinBoundariesExtrema();
  /// Update the variable storing the size if widest time window for overlap detection
  UpdateWidestTimeWinRange();
}

void CbmAlgoBuildRawEvents::RemoveDetector(RawEventBuilderDetector selDet)
{
  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if ((*det) == selDet) {
      fvDets.erase(det);
      return;
    }
  }
  LOG(warning) << "CbmAlgoBuildRawEvents::RemoveDetector => Doing "
                  "nothing, selection detector not in list!"
               << selDet.sName;
}

void CbmAlgoBuildRawEvents::SetTriggerMinNumber(ECbmModuleId selDet, UInt_t uVal)
{
  /// Check first if reference detector
  if (fRefDet.detId == selDet) {
    fRefDet.fuTriggerMinDigis = uVal;
    LOG(debug) << "Set Trigger min limit for " << fRefDet.sName << " to " << uVal;
    return;
  }

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if ((*det).detId == selDet) {
      (*det).fuTriggerMinDigis = uVal;
      LOG(debug) << "Set Trigger min limit for " << (*det).sName << " to " << uVal;
      return;
    }
  }
  LOG(warning) << "CbmAlgoBuildRawEvents::SetTriggerMinNumber => "
                  "Doing nothing, detector neither reference nor in selection list!"
               << selDet;
}

void CbmAlgoBuildRawEvents::SetTriggerMaxNumber(ECbmModuleId selDet, Int_t iVal)
{
  /// Check first if reference detector
  if (fRefDet.detId == selDet) {
    fRefDet.fiTriggerMaxDigis = iVal;
    LOG(debug) << "Set Trigger min limit for " << fRefDet.sName << " to " << iVal;
    return;
  }

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if ((*det).detId == selDet) {
      (*det).fiTriggerMaxDigis = iVal;
      LOG(debug) << "Set Trigger min limit for " << (*det).sName << " to " << iVal;
      return;
    }
  }
  LOG(warning) << "CbmAlgoBuildRawEvents::SetTriggerMaxNumber => "
                  "Doing nothing, detector neither reference nor in selection list!"
               << selDet;
}

void CbmAlgoBuildRawEvents::SetTriggerWindow(ECbmModuleId selDet, Double_t dWinBeg, Double_t dWinEnd)
{
  /// Check if valid time window: end strictly after beginning
  if (dWinEnd <= dWinBeg) {
    LOG(fatal) << "CbmAlgoBuildRawEvents::SetTriggerWindow => "
                  "Invalid time window: [ "
               << dWinBeg << ", " << dWinEnd << " ]";
  }

  Bool_t bFound = kFALSE;
  /// Check first if reference detector
  if (fRefDet.detId == selDet) {
    fRefDet.fdTimeWinBeg = dWinBeg;
    fRefDet.fdTimeWinEnd = dWinEnd;
    bFound               = kTRUE;
  }

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if ((*det).detId == selDet) {
      (*det).fdTimeWinBeg = dWinBeg;
      (*det).fdTimeWinEnd = dWinEnd;
      bFound              = kTRUE;
    }
  }

  if (kFALSE == bFound) {
    LOG(warning) << "CbmAlgoBuildRawEvents::SetTriggerWindow => "
                    "Doing nothing, detector neither reference nor in selection list!"
                 << selDet;
  }

  /// Update the variables storing the earliest and latest time window boundaries
  UpdateTimeWinBoundariesExtrema();
  /// Update the variable storing the size if widest time window for overlap detection
  UpdateWidestTimeWinRange();
}

void CbmAlgoBuildRawEvents::UpdateTimeWinBoundariesExtrema()
{
  /// Initialize with reference detector
  fdEarliestTimeWinBeg = fRefDet.fdTimeWinBeg;
  fdLatestTimeWinEnd   = fRefDet.fdTimeWinEnd;

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    fdEarliestTimeWinBeg = std::min(fdEarliestTimeWinBeg, (*det).fdTimeWinBeg);
    fdLatestTimeWinEnd   = std::max(fdLatestTimeWinEnd, (*det).fdTimeWinEnd);
  }
}

void CbmAlgoBuildRawEvents::UpdateWidestTimeWinRange()
{
  /// Initialize with reference detector
  fdWidestTimeWinRange = fRefDet.fdTimeWinEnd - fRefDet.fdTimeWinBeg;

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    fdWidestTimeWinRange = std::max(fdWidestTimeWinRange, (*det).fdTimeWinEnd - (*det).fdTimeWinBeg);
  }
}

ClassImp(CbmAlgoBuildRawEvents)
