/* Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Dominik Smith */

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
#include <FairLogger.h>
#include <FairRootManager.h>
#include <FairRunOnline.h>

/// FAIRSOFT headers (geant, boost, ...)
#include <TCanvas.h>
#include <TClonesArray.h>
#include <TFolder.h>
#include <TH1.h>
#include <TH2.h>
#include <THttpServer.h>
#include <TStopwatch.h>

template<>
void CbmAlgoBuildRawEvents::LoopOnSeeds<Double_t>();

Bool_t CbmAlgoBuildRawEvents::InitAlgo()
{
  LOG(info) << "CbmAlgoBuildRawEvents::InitAlgo => Starting sequence";

  if (fbGetTimings) {
    fTimer = new TStopwatch;
    fTimer->Start();
  }

  /// Check if reference detector is set and seed data are available,
  /// otherwise look for explicit seed times
  if (fRefDet.detId == ECbmModuleId::kNotExist) {
    if (fSeedTimes == nullptr) {
      LOG(fatal) << "No reference detector set and no seed times supplied, stopping there!";
    }
  }
  else {
    if (fSeedTimes != nullptr) {
      LOG(fatal) << "Cannot have explicit seed times and reference detector, stopping there!";
    }
    if (kFALSE == CheckDataAvailable(fRefDet)) {
      LOG(fatal) << "Reference detector set but no digi input found, stopping there!";
    }
  }

  /// Check if data for detectors in selection list are available
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if (kFALSE == CheckDataAvailable(*det)) {
      LOG(fatal) << "No digi input for one of selection detector, stopping there!";
    }
  }

  /// Access the TS metadata to know TS start time if needed
  if (fbUseTsMetaData) {
    if (!fTimeSliceMetaDataArray) {
      LOG(fatal) << "No TS metadata input found"
                 << " => Please check in the unpacking macro if the following line was "
                    "present!"
                 << std::endl
                 << "source->SetWriteOutputFlag(kTRUE);  // For writing TS metadata";
    }
  }
  if (fbFillHistos) { CreateHistograms(); }
  if (fTimer != nullptr) {
    fTimer->Stop();
    Double_t rtime = fTimer->RealTime();
    Double_t ctime = fTimer->CpuTime();
    LOG(info) << "CbmAlgoBuildRawEvents::Init(): Real time " << rtime << " s, CPU time " << ctime << " s";
  }

  LOG(info) << "CbmAlgoBuildRawEvents::InitAlgo => Done";
  return kTRUE;
}

void CbmAlgoBuildRawEvents::Finish()
{
  if (fbGetTimings) { PrintTimings(); }
}

void CbmAlgoBuildRawEvents::PrintTimings()
{
  if (fTimer == nullptr) { LOG(fatal) << "Trying to print timings but timer not set"; }
  else {
    Double_t rtime = fTimer->RealTime();
    Double_t ctime = fTimer->CpuTime();
    LOG(info) << "CbmAlgoBuildRawEvents: Real time " << rtime << " s, CPU time " << ctime << " s";
  }
}

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
  if (fTimer != nullptr) { fTimer->Start(kFALSE); }
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
  if (fTimer != nullptr) { fTimer->Stop(); }

  fuNrTs++;
}

void CbmAlgoBuildRawEvents::InitTs()
{
  /// Reset TS based variables (analysis per TS = no building over the border)

  /// Reference detector
  if (fRefDet.detId != ECbmModuleId::kNotExist) {
    fRefDet.fuStartIndex = 0;
    fRefDet.fuEndIndex   = 0;
  }
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

  if (fbUseTsMetaData) {
    const TimesliceMetaData* pTsMetaData = dynamic_cast<TimesliceMetaData*>(fTimeSliceMetaDataArray->At(0));
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
    case ECbmModuleId::kTrd2d:  // Same data storage as trd 1d
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
    case ECbmModuleId::kNotExist: {  //explicit seed times
      LoopOnSeeds<Double_t>();
      break;
    }
    default: {
      LOG(fatal) << "CbmAlgoBuildRawEvents::BuildEvents => "
                 << "Trying to search event seeds with unsupported det: " << fRefDet.sName;
      break;
    }
  }
}

template<>
void CbmAlgoBuildRawEvents::LoopOnSeeds<Double_t>()
{
  if (ECbmModuleId::kNotExist == fRefDet.detId) {
    const UInt_t uNbSeeds = fSeedTimes->size();
    /// Loop on size of vector
    for (UInt_t uSeed = 0; uSeed < uNbSeeds; ++uSeed) {
      LOG(debug) << Form("Checking seed %6u / %6u", uSeed, uNbSeeds);
      Double_t dTime = fSeedTimes->at(uSeed);

      /// Check if seed in acceptance window (is this needed here?)
      if (dTime < fdSeedWindowBeg) { continue; }
      else if (fdSeedWindowEnd < dTime) {
        break;
      }
      /// Check Seed and build event if needed
      CheckSeed(dTime, uSeed);
    }
  }
  else {
    LOG(fatal) << "Trying to read explicit seeds while reference detector is set.";
  }
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
    const UInt_t uNbRefDigis = GetNofDigis(fRefDet.detId);
    /// Loop on size of vector
    for (UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi) {
      LOG(debug) << Form("Checking seed %6u / %6u", uDigi, uNbRefDigis);

      const DigiSeed* pDigi = GetDigi<DigiSeed>(uDigi);

      // filter T0 digis from Tof (remove this line if T0 properly implemented)
      if (fRefDet.detId == ECbmModuleId::kTof && pDigi->GetAddress() == fuT0Address) { continue; }

      const Double_t dTime = pDigi->GetTime();

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

Double_t CbmAlgoBuildRawEvents::GetSeedTimeWinRange()
{
  if (ECbmModuleId::kNotExist != fRefDet.detId) { return fRefDet.GetTimeWinRange(); }
  else {
    return fdSeedTimeWinEnd - fdSeedTimeWinBeg;
  }
}

void CbmAlgoBuildRawEvents::CheckSeed(Double_t dSeedTime, UInt_t uSeedDigiIdx)
{
  /// If previous event valid and event overlap not allowed, check if we are in overlap
  /// and react accordingly
  if (nullptr != fCurrentEvent
      && (EOverlapModeRaw::AllowOverlap != fOverMode || dSeedTime - fdPrevEvtTime < GetSeedTimeWinRange())
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

  if (fRefDet.detId != ECbmModuleId::kNotExist) {
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
    case ECbmModuleId::kTrd2d:  // Same data storage as trd 1d
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
    const UInt_t uNbSelDigis = GetNofDigis(detMatch.detId);
    /// Loop on size of vector
    for (UInt_t uDigi = detMatch.fuStartIndex; uDigi < uNbSelDigis; ++uDigi) {
      const DigiCheck* pDigi = GetDigi<DigiCheck>(uDigi);

      // Filter TRD2D digis if 1D and reverse
      if (detMatch.detId == ECbmModuleId::kTrd) {
        const CbmTrdDigi* pTrdDigi = GetDigi<CbmTrdDigi>(uDigi);
        if (pTrdDigi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP) {  //
          continue;
        }
      }
      else if (detMatch.detId == ECbmModuleId::kTrd2d) {
        const CbmTrdDigi* pTrdDigi = GetDigi<CbmTrdDigi>(uDigi);
        if (pTrdDigi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kSPADIC) {  //
          continue;
        }
      }

      // filter T0 digis from Tof (remove this line if T0 properly implemented)
      //if (detMatch.detId == ECbmModuleId::kTof && pDigi->GetAddress() == fuT0Address) { continue; }

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
      if (fRefDet.detId != ECbmModuleId::kNotExist) { fRefDet.fuStartIndex = fRefDet.fuEndIndex; }
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
  if (fRefDet.detId != ECbmModuleId::kNotExist) {
    if (kFALSE == CheckTriggerConditions(event, fRefDet)) { return kFALSE; }
  }
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
  if (!CheckDataAvailable(det.detId)) {
    LOG(warning) << "Event does not have digis storage for " << det.sName
                 << " while the following trigger min/max are defined: " << det.fuTriggerMinDigis << " "
                 << det.fiTriggerMaxDigis;
    return kFALSE;
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
  if (!CheckDataAvailable(det.detId)) {
    LOG(info) << "No " << det.sName << " digi input found.";
    return kFALSE;
  }
  return kTRUE;
}

bool CbmAlgoBuildRawEvents::CheckDataAvailable(ECbmModuleId detId)
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
    case ECbmModuleId::kTrd2d:  // Same data storage as trd 1d
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
      return fT0DigiVec != nullptr;
    }
    default: {
      LOG(fatal) << "CbmAlgoBuildRawEvents::CheckDataAvailable => "
                 << "Unsupported detector.";
      return -1;
    }
  }
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
    case ECbmModuleId::kTrd2d:  // Same data storage as trd 1d
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
  outFolder = new TFolder("AlgoBuildRawEventsHist", " AlgoBuildRawEvents Histos");
  outFolder->Clear();

  fhEventTime = new TH1F("hEventTime", "seed time of the events; Seed time within TS [s]; Events", 100000, 0, 0.2);
  // fhEventTime->SetCanExtend(TH1::kAllAxes);  // Breaks the MQ histogram server as cannot be merged!

  fhEventDt =
    new TH1F("fhEventDt", "interval in seed time of consecutive events; Seed time dt [ns]; Events", 10000, 0, 100000);
  // fhEventDt->SetCanExtend(TH1::kAllAxes);  // Breaks the MQ histogram server as cannot be merged!

  fhEventSize = new TH1F("hEventSize", "nb of all  digis in the event; Nb Digis []; Events []", 10000, 0, 10000);
  // fhEventSize->SetCanExtend(TH1::kAllAxes);  // Breaks the MQ histogram server as cannot be merged!

  fhNbDigiPerEvtTime = new TH2I("hNbDigiPerEvtTime",
                                "nb of all  digis per event vs seed time of the events; Seed time "
                                "[s]; Nb Digis []; Events []",
                                1000, 0, 0.2, 5000, 0, 5000);
  // fhNbDigiPerEvtTime->SetCanExtend(TH2::kAllAxes);  // Breaks he MQ histogram server as cannot be merged!

  AddHistoToVector(fhEventTime, "evtbuild");
  AddHistoToVector(fhEventDt, "evtbuild");
  AddHistoToVector(fhEventSize, "evtbuild");
  AddHistoToVector(fhNbDigiPerEvtTime, "evtbuild");
  outFolder->Add(fhEventTime);
  outFolder->Add(fhEventDt);
  outFolder->Add(fhEventSize);
  outFolder->Add(fhNbDigiPerEvtTime);

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    /// In case name not provided, do not create the histo to avoid name conflicts!
    if ("Invalid" == (*det).sName) {
      fvhNbDigiPerEvtTimeDet.push_back(nullptr);
      fvhNbDigiPerEvtDet.push_back(nullptr);
      continue;
    }
    TH2I* hNbDigiPerEvtTimeDet = new TH2I(Form("hNbDigiPerEvtTime%s", (*det).sName.data()),
                                          Form("nb of %s digis per event vs seed time of the events; Seed time in TS "
                                               "[s]; Nb Digis []; Events []",
                                               (*det).sName.data()),
                                          1000, 0, 0.2, 5000, 0, 5000);
    // hNbDigiPerEvtTimeDet->SetCanExtend(TH2::kAllAxes);   // Breaks he MQ histogram server as cannot be merged!
    fvhNbDigiPerEvtTimeDet.push_back(hNbDigiPerEvtTimeDet);

    TH1I* hNbDigiPerEvtDet =
      new TH1I(Form("hNbDigiPerEvt%s", (*det).sName.data()),
               Form("nb of %s digis per event; Nb Digis []", (*det).sName.data()), 10000, 0, 10000);
    fvhNbDigiPerEvtDet.push_back(hNbDigiPerEvtDet);

    TH1I* hTDiff =
      new TH1I(Form("hTDiff%s", (*det).sName.data()),
               Form("#DeltaT of %s digis to seed time of event;#DeltaT (ns); Counts []", (*det).sName.data()), 200,
               (*det).fdTimeWinBeg, (*det).fdTimeWinEnd);
    fvhTDiff.push_back(hTDiff);
  }

  /// Same plots for the reference detector
  TH1I* hNbDigiPerEvtDet =
    new TH1I(Form("hNbDigiPerEvt%s", fRefDet.sName.data()),
             Form("nb of %s digis per event; Nb Digis []", fRefDet.sName.data()), 10000, 0, 10000);
  fvhNbDigiPerEvtDet.push_back(hNbDigiPerEvtDet);

  TH1I* hTDiff =
    new TH1I(Form("hTDiff%s", fRefDet.sName.data()),
             Form("#DeltaT of %s digis to seed time of event;#DeltaT (ns); Counts []", fRefDet.sName.data()), 200,
             fRefDet.fdTimeWinBeg, fRefDet.fdTimeWinEnd);  // FIXME, adjust to configured window
  fvhTDiff.push_back(hTDiff);

  for (std::vector<TH2*>::iterator itHist = fvhNbDigiPerEvtTimeDet.begin(); itHist != fvhNbDigiPerEvtTimeDet.end();
       ++itHist) {
    if (nullptr != (*itHist)) {
      AddHistoToVector((*itHist), "evtbuild");
      outFolder->Add((*itHist));
    }
  }

  for (std::vector<TH1*>::iterator itHist = fvhNbDigiPerEvtDet.begin(); itHist != fvhNbDigiPerEvtDet.end(); ++itHist) {
    if (nullptr != (*itHist)) {
      AddHistoToVector((*itHist), "evtbuild");
      outFolder->Add((*itHist));
    }
  }
  for (std::vector<TH1*>::iterator itHist = fvhTDiff.begin(); itHist != fvhTDiff.end(); ++itHist) {
    if (nullptr != (*itHist)) {
      AddHistoToVector((*itHist), "evtbuild");
      outFolder->Add((*itHist));
    }
  }

  /// Canvases creation
  // std::vector<std::pair<TCanvas*, std::string>> vCanvases = {};

  Double_t w         = 10;
  Double_t h         = 10;
  TCanvas* fcSummary = new TCanvas("cEvBSummary", "EvB monitoring plots", w, h);
  fcSummary->Divide(2, 2);

  fcSummary->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  fhEventTime->Draw("hist");

  fcSummary->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  gPad->SetLogy();
  fhEventDt->Draw("hist");

  fcSummary->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhEventSize->Draw("hist");

  fcSummary->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  fhNbDigiPerEvtTime->Draw("colz");


  /// Add canvas pointers to the canvas vector
  AddCanvasToVector(fcSummary, "canvases");

  // ------------------------ //
  TCanvas* fcNbDigi = new TCanvas("cEvBNbDigi", "EvB NbDigi evolution ", w, h);
  if (fvhNbDigiPerEvtDet.size() <= 6) {  //
    fcNbDigi->Divide(2, 3);
  }
  else {  //
    fcNbDigi->Divide(3, 3);
  }
  int iPad = 1;
  for (std::vector<TH1*>::iterator itHist = fvhNbDigiPerEvtDet.begin(); itHist != fvhNbDigiPerEvtDet.end(); ++itHist) {
    if (nullptr != (*itHist)) {
      fcNbDigi->cd(iPad++);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      (*itHist)->Draw();  //"colz");
    }
  }
  AddCanvasToVector(fcNbDigi, "canvases");

  // ------------------------ //
  TCanvas* fcTdif = new TCanvas("cEvBTdif", "EvB Time Difference plots", w, h);
  if (fvhNbDigiPerEvtDet.size() <= 6) {  //
    fcTdif->Divide(2, 3);
  }
  else {  //
    fcTdif->Divide(3, 3);
  }
  iPad = 1;
  for (std::vector<TH1*>::iterator itHist = fvhTDiff.begin(); itHist != fvhTDiff.end(); ++itHist) {
    if (nullptr != (*itHist)) {
      fcTdif->cd(iPad++);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      (*itHist)->Draw();
    }
  }
  AddCanvasToVector(fcTdif, "canvases");
}

void CbmAlgoBuildRawEvents::FillHistos()
{
  Double_t dPreEvtTime = -1.0;
  for (CbmEvent* evt : fEventVector) {
    fhEventTime->Fill(evt->GetStartTime() * 1e-9);
    if (0.0 <= dPreEvtTime) { fhEventDt->Fill((evt->GetStartTime() - dPreEvtTime) * 1e-9); }

    fhEventSize->Fill(evt->GetNofData());
    fhNbDigiPerEvtTime->Fill(evt->GetStartTime() * 1e-9, evt->GetNofData());

    /// Loop on selection detectors
    uint32_t uNbDataTrd1d = 0;
    uint32_t uNbDataTrd2d = 0;
    for (UInt_t uDetIdx = 0; uDetIdx < fvDets.size(); ++uDetIdx) {
      if (nullptr == fvhNbDigiPerEvtDet[uDetIdx]) continue;

      fvhNbDigiPerEvtDet[uDetIdx]->Fill(TMath::Max(0, evt->GetNofData(fvDets[uDetIdx].dataType)));
      if (nullptr == fvhTDiff[uDetIdx]) continue;
      for (int idigi = 0; idigi < evt->GetNofData(fvDets[uDetIdx].dataType); ++idigi) {
        double dTimeDiff = 1.E30;
        uint idx         = evt->GetIndex(fvDets[uDetIdx].dataType, idigi);
        switch (fvDets[uDetIdx].dataType) {
          case ECbmDataType::kT0Digi: {
            if (fT0DigiVec->size() <= idx) continue;
            dTimeDiff = fT0DigiVec->at(idx).GetTime() - evt->GetStartTime();
            break;
          }
          case ECbmDataType::kStsDigi: {
            auto pDigi = GetDigi<CbmStsDigi>(idx);
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            break;
          }
          case ECbmDataType::kMuchDigi: {
            if (fbUseMuchBeamtimeDigi) {
              auto pDigi = GetDigi<CbmMuchBeamTimeDigi>(idx);
              if (nullptr == pDigi) continue;
              dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            }
            else {
              auto pDigi = GetDigi<CbmMuchDigi>(idx);
              if (nullptr == pDigi) continue;
              dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            }
            break;
          }
          case ECbmDataType::kTofDigi: {
            auto pDigi = GetDigi<CbmTofDigi>(idx);
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            break;
          }
          case ECbmDataType::kTrdDigi: {
            auto pDigi = GetDigi<CbmTrdDigi>(idx);
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            if (pDigi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kSPADIC) {
              if (fvDets[uDetIdx].sName == "kTrd2D") continue;
              ++uNbDataTrd1d;
            }
            else if (pDigi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP) {
              if (fvDets[uDetIdx].sName == "kTrd") continue;
              ++uNbDataTrd2d;
            }
            break;
          }
          case ECbmDataType::kRichDigi: {
            auto pDigi = GetDigi<CbmRichDigi>(idx);  // FIXME, need to find the proper digi template
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            break;
          }
          case ECbmDataType::kPsdDigi: {
            auto pDigi = GetDigi<CbmPsdDigi>(idx);  // FIXME, need to find the proper digi template
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            break;
          }
          default: LOG(error) << "Unkown dataType " << fvDets[uDetIdx].dataType;
        }

        if (dTimeDiff < 1.E30) fvhTDiff[uDetIdx]->Fill(dTimeDiff);
      }
    }
    /// Reference detector
    uint32_t uRefDetIdx = fvDets.size();
    if (nullptr != fvhNbDigiPerEvtDet[uRefDetIdx]) {

      fvhNbDigiPerEvtDet[uRefDetIdx]->Fill(TMath::Max(0, evt->GetNofData(fRefDet.dataType)));
      if (nullptr == fvhTDiff[uRefDetIdx]) continue;
      for (int idigi = 0; idigi < evt->GetNofData(fRefDet.dataType); ++idigi) {
        double dTimeDiff = 1.E30;
        uint idx         = evt->GetIndex(fRefDet.dataType, idigi);
        switch (fRefDet.dataType) {
          case ECbmDataType::kT0Digi: {
            if (fT0DigiVec->size() <= idx) continue;
            dTimeDiff = fT0DigiVec->at(idx).GetTime() - evt->GetStartTime();
            break;
          }
          case ECbmDataType::kStsDigi: {
            auto pDigi = GetDigi<CbmStsDigi>(idx);
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            break;
          }
          case ECbmDataType::kMuchDigi: {
            if (fbUseMuchBeamtimeDigi) {
              auto pDigi = GetDigi<CbmMuchBeamTimeDigi>(idx);
              if (nullptr == pDigi) continue;
              dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            }
            else {
              auto pDigi = GetDigi<CbmMuchDigi>(idx);
              if (nullptr == pDigi) continue;
              dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            }
            break;
          }
          case ECbmDataType::kTofDigi: {
            auto pDigi = GetDigi<CbmTofDigi>(idx);
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            break;
          }
          case ECbmDataType::kTrdDigi: {
            auto pDigi = GetDigi<CbmTrdDigi>(idx);
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            if (pDigi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kSPADIC) {
              if (fRefDet.sName == "kTrd2D") continue;
              ++uNbDataTrd1d;
            }
            else if (pDigi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP) {
              if (fRefDet.sName == "kTrd") continue;
              ++uNbDataTrd2d;
            }
            break;
          }
          case ECbmDataType::kRichDigi: {
            auto pDigi = GetDigi<CbmRichDigi>(idx);  // FIXME, need to find the proper digi template
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            break;
          }
          case ECbmDataType::kPsdDigi: {
            auto pDigi = GetDigi<CbmPsdDigi>(idx);  // FIXME, need to find the proper digi template
            if (nullptr == pDigi) continue;
            dTimeDiff = pDigi->GetTime() - evt->GetStartTime();
            break;
          }
          default: LOG(error) << "Unkown dataType " << fRefDet.dataType;
        }

        if (dTimeDiff < 1.E30) fvhTDiff[uRefDetIdx]->Fill(dTimeDiff);
      }
    }

    /// Re-Loop on selection detectors due to detectors with same data type
    for (UInt_t uDetIdx = 0; uDetIdx < fvDets.size(); ++uDetIdx) {
      if (nullptr == fvhNbDigiPerEvtTimeDet[uDetIdx]) continue;

      if (fvDets[uDetIdx].sName == "kTrd") {
        fvhNbDigiPerEvtTimeDet[uDetIdx]->Fill(evt->GetStartTime() * 1e-9, uNbDataTrd1d);
      }
      else if (fvDets[uDetIdx].sName == "kTrd2D") {
        fvhNbDigiPerEvtTimeDet[uDetIdx]->Fill(evt->GetStartTime() * 1e-9, uNbDataTrd2d);
      }
      else {
        fvhNbDigiPerEvtTimeDet[uDetIdx]->Fill(evt->GetStartTime() * 1e-9, evt->GetNofData(fvDets[uDetIdx].dataType));
      }
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

  for (std::vector<TH1*>::iterator itHist = fvhNbDigiPerEvtDet.begin(); itHist != fvhNbDigiPerEvtDet.end(); ++itHist) {
    (*itHist)->Reset();
  }

  for (std::vector<TH1*>::iterator itHist = fvhTDiff.begin(); itHist != fvhTDiff.end(); ++itHist) {
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
      break;
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
  /// Initialize with reference detector or explicit times
  if (fRefDet.detId != ECbmModuleId::kNotExist) {
    fdEarliestTimeWinBeg = fRefDet.fdTimeWinBeg;
    fdLatestTimeWinEnd   = fRefDet.fdTimeWinEnd;
  }
  else {
    fdEarliestTimeWinBeg = fdSeedTimeWinBeg;
    fdLatestTimeWinEnd   = fdSeedTimeWinEnd;
  }

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    fdEarliestTimeWinBeg = std::min(fdEarliestTimeWinBeg, (*det).fdTimeWinBeg);
    fdLatestTimeWinEnd   = std::max(fdLatestTimeWinEnd, (*det).fdTimeWinEnd);
  }
}

void CbmAlgoBuildRawEvents::UpdateWidestTimeWinRange()
{
  /// Initialize with reference detector
  fdWidestTimeWinRange = GetSeedTimeWinRange();

  /// Loop on selection detectors
  for (std::vector<RawEventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    fdWidestTimeWinRange = std::max(fdWidestTimeWinRange, (*det).fdTimeWinEnd - (*det).fdTimeWinBeg);
  }
}

ClassImp(CbmAlgoBuildRawEvents)
