/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmMcbm2019TimeWinEventBuilderAlgo.h"

/// CBM headers
#include "CbmEvent.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"
#include "TimesliceMetaData.h"

#include "CbmDigiManager.h"

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

/// C/C++ headers

// ---- Default constructor --------------------------------------------
CbmMcbm2019TimeWinEventBuilderAlgo::CbmMcbm2019TimeWinEventBuilderAlgo() {}

// ---- Destructor -----------------------------------------------------
CbmMcbm2019TimeWinEventBuilderAlgo::~CbmMcbm2019TimeWinEventBuilderAlgo() {}

// ---- Init -----------------------------------------------------------
Bool_t CbmMcbm2019TimeWinEventBuilderAlgo::InitAlgo() {
  LOG(info)
    << "CbmMcbm2019TimeWinEventBuilderAlgo::InitAlgo => Starting sequence";

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Get a pointer to the previous already existing data level
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->UseMuchBeamTimeDigi();
  fDigiMan->Init();

  /// Check if reference detector data are available
  if (kFALSE == CheckDataAvailable(fRefDet)) {
    LOG(fatal) << "No digi input for reference detector, stopping there!";
  }  // if( kFALSE == CheckDataAvailable( fRefDet ) )

  /// Check if data for detectors in selection list are available
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    if (kFALSE == CheckDataAvailable(*det)) {
      LOG(fatal)
        << "No digi input for one of selection detector, stopping there!";
    }  // if( kFALSE == CheckDataAvailable( *det ) )
  }  // for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det)

  /// Access the TS metadata to know TS start tim
  fTimeSliceMetaDataArray =
    dynamic_cast<TClonesArray*>(ioman->GetObject("TimesliceMetaData"));
  if (!fTimeSliceMetaDataArray) {
    LOG(fatal)
      << "No TS metadata input found"
      << " => Please check in the unpacking macro if the following line was "
         "present!"
      << std::endl
      << "source->SetWriteOutputFlag(kTRUE);  // For writing TS metadata";
  }  // if (!fTimeSliceMetaDataArray)

  if (fbFillHistos) { CreateHistograms(); }  // if( fbFillHistos )

  LOG(info) << "CbmMcbm2019TimeWinEventBuilderAlgo::InitAlgo => Done";

  return kTRUE;
}

// ---- ProcessTs ------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::ProcessTs() {
  LOG_IF(info, fuNrTs % 1000 == 0) << "Begin of TS " << fuNrTs;

  InitTs();

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
  }  // if( nullptr != fCurrentEvent )

  LOG(debug) << "Found " << fEventVector.size() << " triggered events";

  if (fbFillHistos) { FillHistos(); }  // if( fbFillHistos )

  fuNrTs++;
}
void CbmMcbm2019TimeWinEventBuilderAlgo::ClearEventVector() {
  /// Need to delete the object the pointer points to first
  int counter = 0;
  for (CbmEvent* event : fEventVector) {
    LOG(debug) << "Event " << counter << " has " << event->GetNofData()
               << " digis";
    delete event;
    counter++;
  }  // for( CbmEvent* event: fEventVector)

  fEventVector.clear();
}
// ---- Finish ---------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::Finish() {
  LOG(info) << "Total errors: " << fuErrors;
}

// ---------------------------------------------------------------------
Bool_t CbmMcbm2019TimeWinEventBuilderAlgo::CheckDataAvailable(
  EventBuilderDetector& det) {
  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  if (ECbmModuleId::kT0 == det.detId) {
    // T0 is not included in DigiManager
    fT0DigiVec = ioman->InitObjectAs<std::vector<CbmTofDigi> const*>("T0Digi");
    if (!fT0DigiVec) {
      LOG(info) << "No T0 digi input found.";
      return kFALSE;
    }  // if( ! fT0DigiVec )
  }    // if( ECbmModuleId::kT0 == det.detId )
  else {
    if (!fDigiMan->IsPresent(det.detId)) {
      LOG(info) << "No " << det.sName << " digi input found.";
      return kFALSE;
    }  // if( ! fDigiMan->IsPresent(ECbmModuleId::kSts) )
  }    // else of if( ECbmModuleId::kT0 == det.detId )

  return kTRUE;
}
// ---------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::InitTs() {
  /// Reset TS based variables (analysis per TS = no building over the border)
  /// Reference detector
  fRefDet.fuStartIndex = 0;
  fRefDet.fuEndIndex   = 0;
  /// Loop on detectors in selection list
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    (*det).fuStartIndex = 0;
    (*det).fuEndIndex   = 0;
  }  // for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det)
}

void CbmMcbm2019TimeWinEventBuilderAlgo::BuildEvents() {
  /// Call LoopOnSeed with proper template argument
  switch (fRefDet.detId) {
    case ECbmModuleId::kSts: {
      LoopOnSeeds<CbmStsDigi>();
      break;
    }  // case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch: {
      LoopOnSeeds<CbmMuchBeamTimeDigi>();
      break;
    }  // case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd: {
      LoopOnSeeds<CbmTrdDigi>();
      break;
    }  // case ECbmModuleId::kTrd:
    case ECbmModuleId::kTof: {
      LoopOnSeeds<CbmTofDigi>();
      break;
    }  // case ECbmModuleId::kTof:
    case ECbmModuleId::kRich: {
      LoopOnSeeds<CbmRichDigi>();
      break;
    }  // case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd: {
      LoopOnSeeds<CbmPsdDigi>();
      break;
    }  // case ECbmModuleId::kPsd:
    case ECbmModuleId::kT0: {
      LoopOnSeeds<CbmTofDigi>();
      break;
    }  // case ECbmModuleId::kT0:
    default: {
      LOG(fatal) << "CbmMcbm2019TimeWinEventBuilderAlgo::BuildEvents => "
                 << "Trying to search event seeds with unsupported det: "
                 << fRefDet.sName;
      break;
    }  // default:
  }    // switch( *det )
}

template<class DigiSeed>
void CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds() {
  pTsMetaData =
    dynamic_cast<TimesliceMetaData*>(fTimeSliceMetaDataArray->At(0));
  if (nullptr == pTsMetaData)
    LOG(fatal) << Form("CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                       "No TS metadata found for TS %6u.",
                       fuNrTs);

  /// Print warning in first TS if time window borders out of potential overlap
  if ((0.0 < fdEarliestTimeWinBeg
       && pTsMetaData->GetOverlapDuration() < fdLatestTimeWinEnd)
      || (pTsMetaData->GetOverlapDuration() < fdWidestTimeWinRange)) {
    LOG(warning) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                 << Form("Event window not fitting in TS overlap, risk of "
                         "incomplete events: %f %f %f %llu",
                         fdEarliestTimeWinBeg,
                         fdLatestTimeWinEnd,
                         fdWidestTimeWinRange,
                         pTsMetaData->GetOverlapDuration());
  }  // if end of event window does not fit in overlap for a seed at edge of TS core

  /// Define an acceptance window for the seeds in order to use the overlap
  /// part of the TS to avoid incomplete events
  Double_t dSeedWindowBeg =
    pTsMetaData->GetStartTime()
    + (0.0 < fdEarliestTimeWinBeg ? 0.0 : -fdEarliestTimeWinBeg);
  Double_t dSeedWindowEnd =
    pTsMetaData->GetOverlapStartTime()
    + (0.0 < fdEarliestTimeWinBeg ? 0.0 : -fdEarliestTimeWinBeg);
  if (fbIgnoreTsOverlap) {
    dSeedWindowBeg = pTsMetaData->GetStartTime();
    dSeedWindowEnd = pTsMetaData->GetOverlapStartTime();
  }  // if( fbIgnoreTsOverlap )

  if (ECbmModuleId::kT0 == fRefDet.detId) {
    if (fT0DigiVec) {
      /// Loop on size of vector
      UInt_t uNbRefDigis = fT0DigiVec->size();
      /// Loop on size of vector
      for (UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi) {
        LOG(debug) << Form("Checking seed %6u / %6u", uDigi, uNbRefDigis);

        Double_t dTime = fT0DigiVec->at(uDigi).GetTime();

        /// Check Seed and build event if needed
        CheckSeed(dTime, uDigi);
      }  // for( UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi )
    }    // if ( fT0DigiVec )
    else
      LOG(fatal) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                 << "T0 as reference detector but vector not found!";
  }  // if (ECbmModuleId::kT0 == fRefDet.detId)
  else {
    UInt_t uNbRefDigis = (0 < fDigiMan->GetNofDigis(fRefDet.detId)
                            ? fDigiMan->GetNofDigis(fRefDet.detId)
                            : 0);
    /// Loop on size of vector
    for (UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi) {
      LOG(debug) << Form("Checking seed %6u / %6u", uDigi, uNbRefDigis);
      const DigiSeed* pDigi = fDigiMan->Get<DigiSeed>(uDigi);
      /// Check that _entry is not out of range
      if (nullptr != pDigi) {
        Double_t dTime = pDigi->GetTime();

        /// Check if seed in acceptance window
        if (dTime < dSeedWindowBeg) {
          continue;
        }  // if( dTime < dSeedWindowBeg )
        else if (dSeedWindowEnd < dTime) {
          break;
        }  // else if( dSeedWindowEnd < dTime )

        /// Check Seed and build event if needed
        CheckSeed(dTime, uDigi);
      }  // if( nullptr != pDigi )
    }    // for( UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi )
  }  // else of if (ECbmModuleId::kT0 == fRefDet.detId) => Digi containers controlled by DigiManager
}

void CbmMcbm2019TimeWinEventBuilderAlgo::CheckSeed(Double_t dSeedTime,
                                                   UInt_t uSeedDigiIdx) {
  /// If previous event valid and event overlap not allowed, check if we are in overlap
  /// and react accordingly
  if (nullptr != fCurrentEvent
      && (EOverlapMode::AllowOverlap != fOverMode
          || dSeedTime - fdPrevEvtTime < fRefDet.GetTimeWinRange())
      && dSeedTime - fdPrevEvtTime < fdWidestTimeWinRange) {
    /// Within overlap range
    switch (fOverMode) {
      case EOverlapMode::NoOverlap: {
        /// No overlap allowed => reject
        LOG(debug1) << "Reject seed due to overlap";
        return;
        break;
      }  // case EOverlapMode::NoOverlap:
      case EOverlapMode::MergeOverlap: {
        /// Merge overlap mode => do nothing and go on filling current event
        break;
      }  // case EOverlapMode::MergeOverlap:
      case EOverlapMode::AllowOverlap: {
        /// In allow overlap mode => reject only if reference det is in overlap
        /// to avoid cloning events due to single seed cluster
        LOG(debug1) << "Reject seed because part of cluster of previous one";
        return;
        break;
      }  // case EOverlapMode::AllowOverlap:
    }    // switch( fOverMode )
  }      // if( prev Event exists and mode forbiden overlap present )
  else {
    /// Out of overlap range or in overlap allowed mode
    /// => store previous event if not empty and create new one
    if (nullptr != fCurrentEvent) {
      /// TODO: store start time of current event ?
      //        fCurrentEvent->SetStartTime( fPrevTime ); // Replace Seed time with time of first digi in event?
      fCurrentEvent->SetEndTime(fdPrevEvtEndTime);
      fEventVector.push_back(fCurrentEvent);
      fuCurEv++;
    }  // if( nullptr != fCurrentEvent )
    fCurrentEvent = new CbmEvent(fuCurEv, dSeedTime, 0.);
  }  // else of if( prev Event exists and mode forbiden overlap present )

  /// If window open for reference detector, search for other reference Digis matching it
  /// Otherwise only add the current seed
  if (fRefDet.fdTimeWinBeg < fRefDet.fdTimeWinEnd) {
    switch (fRefDet.detId) {
      case ECbmModuleId::kSts: {
        SearchMatches<CbmStsDigi>(dSeedTime, fRefDet);
        break;
      }  // case ECbmModuleId::kSts:
      case ECbmModuleId::kMuch: {
        SearchMatches<CbmMuchBeamTimeDigi>(dSeedTime, fRefDet);
        break;
      }  // case ECbmModuleId::kMuch:
      case ECbmModuleId::kTrd: {
        SearchMatches<CbmTrdDigi>(dSeedTime, fRefDet);
        break;
      }  // case ECbmModuleId::kTrd:
      case ECbmModuleId::kTof: {
        SearchMatches<CbmTofDigi>(dSeedTime, fRefDet);
        break;
      }  // case ECbmModuleId::kTof:
      case ECbmModuleId::kRich: {
        SearchMatches<CbmRichDigi>(dSeedTime, fRefDet);
        break;
      }  // case ECbmModuleId::kRich:
      case ECbmModuleId::kPsd: {
        SearchMatches<CbmPsdDigi>(dSeedTime, fRefDet);
        break;
      }  // case ECbmModuleId::kPsd:
      case ECbmModuleId::kT0: {
        SearchMatches<CbmTofDigi>(dSeedTime, fRefDet);
        break;
      }  // case ECbmModuleId::kT0:
      default: {
        LOG(fatal) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                   << "Trying to search matches with unsupported det: "
                   << fRefDet.sName << std::endl
                   << "You may want to add support for it in the method.";
        break;
      }  // default:
    }    // switch( fRefDet )

    /// Also add the seed if the window starts after the seed
    if (0 < fRefDet.fdTimeWinBeg) AddDigiToEvent(fRefDet, uSeedDigiIdx);
  }  // if( fdRefTimeWinBeg < fdRefTimeWinEnd )
  else {
    AddDigiToEvent(fRefDet, uSeedDigiIdx);
  }  // else of if( fdRefTimeWinBeg < fdRefTimeWinEnd )

  /// Search for matches for each detector in selection list
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    switch ((*det).detId) {
      case ECbmModuleId::kSts: {
        SearchMatches<CbmStsDigi>(dSeedTime, *det);
        break;
      }  // case ECbmModuleId::kSts:
      case ECbmModuleId::kMuch: {
        SearchMatches<CbmMuchBeamTimeDigi>(dSeedTime, *det);
        break;
      }  // case ECbmModuleId::kMuch:
      case ECbmModuleId::kTrd: {
        SearchMatches<CbmTrdDigi>(dSeedTime, *det);
        break;
      }  // case ECbmModuleId::kTrd:
      case ECbmModuleId::kTof: {
        SearchMatches<CbmTofDigi>(dSeedTime, *det);
        break;
      }  // case ECbmModuleId::kTof:
      case ECbmModuleId::kRich: {
        SearchMatches<CbmRichDigi>(dSeedTime, *det);
        break;
      }  // case ECbmModuleId::kRich:
      case ECbmModuleId::kPsd: {
        SearchMatches<CbmPsdDigi>(dSeedTime, *det);
        break;
      }  // case ECbmModuleId::kPsd:
      case ECbmModuleId::kT0: {
        SearchMatches<CbmTofDigi>(dSeedTime, *det);
        break;
      }  // case ECbmModuleId::kT0:
      default: {
        LOG(fatal) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                   << "Trying to search matches with unsupported det: "
                   << (*det).sName << std::endl
                   << "You may want to add support for it in the method.";
        break;
      }  // default:
    }    // switch( *det )
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  /// Check if event is filling trigger conditions and clear it if not
  if (HasTrigger(fCurrentEvent)) {
    fdPrevEvtTime = dSeedTime;

    /// In case of NoOverlap or MergeOverlap, we can and should start checking the next window
    /// from end of current window in order to save CPU and avoid duplicating
    if (EOverlapMode::NoOverlap == fOverMode
        || EOverlapMode::MergeOverlap == fOverMode) {

      /// Update reference detector
      fRefDet.fuStartIndex = fRefDet.fuEndIndex;

      /// Loop on selection detectors
      for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
           det != fvDets.end();
           ++det) {
        (*det).fuStartIndex = (*det).fuEndIndex;
      }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
    }    // If no overlap or merge overlap
  }      // if( !HasTrigger( fCurrentEvent ) )
  else {
    LOG(debug1) << "Reject seed due to Trigger requirements";
    delete fCurrentEvent;
    fCurrentEvent = nullptr;  /// delete does NOT set a pointer to nullptr...
  }                           // else of if( !HasTrigger( fCurrentEvent ) )
}

template<class DigiCheck>
void CbmMcbm2019TimeWinEventBuilderAlgo::SearchMatches(
  Double_t dSeedTime,
  EventBuilderDetector& detMatch) {
  /// This algo relies on time sorted vectors for the selected detectors
  UInt_t uLocalIndexStart = detMatch.fuStartIndex;
  UInt_t uLocalIndexEnd   = detMatch.fuStartIndex;

  /// Check the Digis until out of window
  if (ECbmModuleId::kT0 == detMatch.detId) {
    if (fT0DigiVec) {
      /// Loop on size of vector
      UInt_t uNbSelDigis = fT0DigiVec->size();
      /// Loop on size of vector
      for (UInt_t uDigi = detMatch.fuStartIndex; uDigi < uNbSelDigis; ++uDigi) {
        Double_t dTime = fT0DigiVec->at(uDigi).GetTime();

        Double_t dTimeDiff = dTime - dSeedTime;

        /// Check if within time window, update start/stop indices if needed
        if (dTimeDiff < detMatch.fdTimeWinBeg) {
          ++uLocalIndexStart;
          continue;
        }  // if( dTimeDiff < detMatch.fdTimeWinBeg )
        else if (detMatch.fdTimeWinEnd < dTimeDiff) {
          /// Store as end the first digi out of window to avoid double counting in case of
          /// merged overlap event mode
          uLocalIndexEnd = uDigi;
          break;
        }  // else if( detMatch.fdTimeWinEnd < dTimeDiff ) of if( dTimeDiff < detMatch.fdTimeWinBeg )

        AddDigiToEvent(detMatch, uDigi);

        if (fdPrevEvtEndTime < dTime) fdPrevEvtEndTime = dTime;
      }  // for( UInt_t uDigi = 0; uDigi < uNbSelDigis; ++uDigi )

      /// catch the case where we reach the end of the vector before being out of the time window
      if (uLocalIndexEnd < uLocalIndexStart) uLocalIndexEnd = uNbSelDigis;
    }  // if ( fT0DigiVec )
    else
      LOG(fatal) << "CbmMcbm2019TimeWinEventBuilderAlgo::SearchMatches => "
                 << "T0 as selection detector but vector not found!";
  }  // if( ECbmModuleId::kT0 == detMatch.detId )
  else {
    UInt_t uNbSelDigis = (0 < fDigiMan->GetNofDigis(detMatch.detId)
                            ? fDigiMan->GetNofDigis(detMatch.detId)
                            : 0);
    /// Loop on size of vector
    for (UInt_t uDigi = detMatch.fuStartIndex; uDigi < uNbSelDigis; ++uDigi) {
      const DigiCheck* pDigi = fDigiMan->Get<DigiCheck>(uDigi);
      /// Check that _entry is not out of range
      if (nullptr != pDigi) {
        Double_t dTime     = pDigi->GetTime();
        Double_t dTimeDiff = dTime - dSeedTime;

        LOG(debug4) << detMatch.sName
                    << Form(" => Checking match %6u / %6u, dt %f",
                            uDigi,
                            uNbSelDigis,
                            dTimeDiff);

        /// Check if within time window, update start/stop indices if needed
        if (dTimeDiff < detMatch.fdTimeWinBeg) {
          ++uLocalIndexStart;
          continue;
        }  // if( dTimeDiff < detMatch.fdTimeWinBeg )
        else if (detMatch.fdTimeWinEnd < dTimeDiff) {
          /// Store as end the first digi out of window to avoid double counting in case of
          /// merged overlap event mode
          uLocalIndexEnd = uDigi;
          break;
        }  // else if( detMatch.fdTimeWinEnd < dTimeDiff ) of if( dTimeDiff < detMatch.fdTimeWinBeg )

        AddDigiToEvent(detMatch, uDigi);

        if (fdPrevEvtEndTime < dTime) fdPrevEvtEndTime = dTime;
      }  // if( nullptr != pDigi )
    }    // for( UInt_t uDigi = 0; uDigi < uNbSelDigis; ++uDigi )

    /// catch the case where we reach the end of the vector before being out of the time window
    if (uLocalIndexEnd < uLocalIndexStart) uLocalIndexEnd = uNbSelDigis;
  }  // else of if( ECbmModuleId::kT0 == detMatch.detId ) => Digi containers controlled by DigiManager

  /// Update the StartIndex and EndIndex for the next event seed
  detMatch.fuStartIndex = uLocalIndexStart;
  detMatch.fuEndIndex   = uLocalIndexEnd;
}

void CbmMcbm2019TimeWinEventBuilderAlgo::AddDigiToEvent(
  EventBuilderDetector& det,
  Int_t _entry) {
  fCurrentEvent->AddData(det.dataType, _entry);
}

Bool_t CbmMcbm2019TimeWinEventBuilderAlgo::HasTrigger(CbmEvent* event) {
  /// Check first reference detector
  if (kFALSE == CheckTriggerConditions(event, fRefDet)) {
    return kFALSE;
  }  // if (kFALSE == CheckTriggerConditions(event, fRefDet) )

  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    if (kFALSE == CheckTriggerConditions(event, *det)) return kFALSE;
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  /// All Ok, trigger is there
  return kTRUE;
}

Bool_t CbmMcbm2019TimeWinEventBuilderAlgo::CheckTriggerConditions(
  CbmEvent* event,
  EventBuilderDetector& det) {
  /// Check if both Trigger conditions disabled for this detector
  if (0 == det.fuTriggerMinDigis && det.fiTriggerMaxDigis < 0) {
    return kTRUE;
  }  // if( 0 == det.fuTriggerMinDigis && det.fiTriggerMaxDigis < 0 )

  /// Check if detector present
  if (ECbmModuleId::kT0 == det.detId) {
    /// FIXME: special case to be removed once T0 supported by DigiManager
    if (!(fT0DigiVec)) {
      LOG(warning) << "Event does not have digis storage for T0"
                   << " while the following trigger minimum are defined: "
                   << det.fuTriggerMinDigis << " " << det.fiTriggerMaxDigis;
      return kFALSE;
    }  // if( !(fT0DigiVec) )
  }    // if( ECbmDataType::kT0Digi == det.detId )
  else {
    if (!fDigiMan->IsPresent(det.detId)) {
      LOG(warning) << "Event does not have digis storage for " << det.sName
                   << " while the following trigger min/max are defined: "
                   << det.fuTriggerMinDigis << " " << det.fiTriggerMaxDigis;
      return kFALSE;
    }  // if( !fDigiMan->IsPresent( det ) )
  }    // else of if( ECbmDataType::kT0Digi == det )

  /// Check trigger rejection by minimal number or absence
  Int_t iNbDigis = event->GetNofData(det.dataType);
  if ((-1 == iNbDigis)
      || (static_cast<UInt_t>(iNbDigis) < det.fuTriggerMinDigis)) {
    LOG(debug2) << "Event does not have enough digis: " << iNbDigis << " vs "
                << det.fuTriggerMinDigis << " for " << det.sName;
    return kFALSE;
  }  // if((-1 == iNbDigis) || (static_cast<UInt_t>(iNbDigis) < det.fuTriggerMinDigis))
  /// Check trigger rejection by maximal number
  else if (det.fiTriggerMaxDigis < iNbDigis) {
    LOG(debug2) << "Event Has too many digis: " << iNbDigis << " vs "
                << det.fiTriggerMaxDigis << " for " << det.sName;
    return kFALSE;
  }  // else if( iNbDigis < det.fiTriggerMaxDigis )
  else {
    return kTRUE;
  }  // else of else if( iNbDigis < det.fiTriggerMaxDigis )
}
//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::CreateHistograms() {
  /// FIXME: Disable clang formatting for histograms declaration for now
  /* clang-format off */
  fhEventTime = new TH1F("hEventTime",
                         "seed time of the events; Seed time [s]; Events",
                         60000, 0, 600);
  fhEventDt   = new TH1F( "fhEventDt",
                          "interval in seed time of consecutive events; Seed time [s]; Events",
                          2100, -100.5, 1999.5);
  fhEventSize =
    new TH1F("hEventSize",
             "nb of all  digis in the event; Nb Digis []; Events []",
             10000, 0, 10000);
  fhNbDigiPerEvtTime =
    new TH2I("hNbDigiPerEvtTime",
             "nb of all  digis per event vs seed time of the events; Seed time "
             "[s]; Nb Digis []; Events []",
              600, 0,   600,
             1000, 0, 10000);

  /// Loop on selection detectors
  for (std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    /// In case name not provided, do not create the histo to avoid name conflicts!
    if( "Invalid" == (*det).sName )
    {
      fvhNbDigiPerEvtTimeDet.push_back( nullptr );
      continue;
    } // if( "Invalid" == (*det).sName )

    fvhNbDigiPerEvtTimeDet.push_back(
      new TH2I( Form( "hNbDigiPerEvtTime%s", (*det).sName.data() ),
                Form( "nb of %s digis per event vs seed time of the events; Seed time "
                      "[s]; Nb Digis []; Events []",
                      (*det).sName.data() ),
                 600, 0,  600,
                4000, 0, 4000) );
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  AddHistoToVector(fhEventTime,            "evtbuild");
  AddHistoToVector(fhEventDt,              "evtbuild");
  AddHistoToVector(fhEventSize,            "evtbuild");
  AddHistoToVector(fhNbDigiPerEvtTime,     "evtbuild");
  for (std::vector<TH2*>::iterator itHist = fvhNbDigiPerEvtTimeDet.begin();
       itHist != fvhNbDigiPerEvtTimeDet.end();
       ++itHist) {
    if( nullptr != (*itHist) )
    {
      AddHistoToVector((*itHist),   "evtbuild");
    } // if( nullptr != (*itHist) )
  }  // for( std::vector<TH2*>::iterator itHist = fvhNbDigiPerEvtTimeDet.begin(); itHist != fvhNbDigiPerEvtTimeDet.end(); ++itHist )

  /// FIXME: Re-enable clang formatting after histograms declaration
  /* clang-format on */
}
void CbmMcbm2019TimeWinEventBuilderAlgo::FillHistos() {
  Double_t dPreEvtTime = -1.0;
  for (CbmEvent* evt : fEventVector) {
    fhEventTime->Fill(evt->GetStartTime() * 1e-9);
    if (0.0 <= dPreEvtTime) {
      fhEventDt->Fill(evt->GetStartTime() - dPreEvtTime);
    }  // if( 0.0 <= dPreEvtTime )
    fhEventSize->Fill(evt->GetNofData());
    fhNbDigiPerEvtTime->Fill(evt->GetStartTime() * 1e-9, evt->GetNofData());

    /// Loop on selection detectors
    for (UInt_t uDetIdx = 0; uDetIdx < fvDets.size(); ++uDetIdx) {
      if (nullptr == fvhNbDigiPerEvtTimeDet[uDetIdx]) continue;

      fvhNbDigiPerEvtTimeDet[uDetIdx]->Fill(
        evt->GetStartTime() * 1e-9, evt->GetNofData(fvDets[uDetIdx].dataType));
    }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

    dPreEvtTime = evt->GetStartTime();
  }  // for( CbmEvent * evt: fEventVector )
}
void CbmMcbm2019TimeWinEventBuilderAlgo::ResetHistograms(
  Bool_t /*bResetTime*/) {
  fhEventTime->Reset();
  fhEventDt->Reset();
  fhEventSize->Reset();

  fhNbDigiPerEvtTime->Reset();
  /// Loop on histograms
  for (std::vector<TH2*>::iterator itHist = fvhNbDigiPerEvtTimeDet.begin();
       itHist != fvhNbDigiPerEvtTimeDet.end();
       ++itHist) {
    (*itHist)->Reset();
  }  // for( std::vector<TH2*>::iterator itHist = fvhNbDigiPerEvtTimeDet.begin(); itHist != fvhNbDigiPerEvtTimeDet.end(); ++itHist )

  /*
   if( kTRUE == bResetTime )
   {
      /// Also reset the Start time for the evolution plots!
      fdStartTime = -1.0;
   } // if( kTRUE == bResetTime )
*/
}
//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::SetReferenceDetector(
  ECbmModuleId refDet,
  ECbmDataType dataTypeIn,
  std::string sNameIn,
  UInt_t uTriggerMinDigisIn,
  Int_t iTriggerMaxDigisIn,
  Double_t fdTimeWinBegIn,
  Double_t fdTimeWinEndIn) {

  /// FIXME: Deprecated method to be removed later. For now create temp object.
  SetReferenceDetector(EventBuilderDetector(refDet,
                                            dataTypeIn,
                                            sNameIn,
                                            uTriggerMinDigisIn,
                                            iTriggerMaxDigisIn,
                                            fdTimeWinBegIn,
                                            fdTimeWinEndIn));
}
void CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector(ECbmModuleId selDet,
                                                     ECbmDataType dataTypeIn,
                                                     std::string sNameIn,
                                                     UInt_t uTriggerMinDigisIn,
                                                     Int_t iTriggerMaxDigisIn,
                                                     Double_t fdTimeWinBegIn,
                                                     Double_t fdTimeWinEndIn) {

  /// FIXME: Deprecated method to be removed later. For now create temp object.
  AddDetector(EventBuilderDetector(selDet,
                                   dataTypeIn,
                                   sNameIn,
                                   uTriggerMinDigisIn,
                                   iTriggerMaxDigisIn,
                                   fdTimeWinBegIn,
                                   fdTimeWinEndIn));
}
//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::SetReferenceDetector(
  ECbmModuleId refDet) {
  switch (refDet) {
    case ECbmModuleId::kSts: {
      SetReferenceDetector(kEventBuilderDetSts);
      break;
    }  // case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch: {
      SetReferenceDetector(kEventBuilderDetMuch);
      break;
    }  // case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd: {
      SetReferenceDetector(kEventBuilderDetTrd);
      break;
    }  // case ECbmModuleId::kTrd:
    case ECbmModuleId::kTof: {
      SetReferenceDetector(kEventBuilderDetTof);
      break;
    }  // case ECbmModuleId::kTof:
    case ECbmModuleId::kRich: {
      SetReferenceDetector(kEventBuilderDetRich);
      break;
    }  // case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd: {
      SetReferenceDetector(kEventBuilderDetPsd);
      break;
    }  // case ECbmModuleId::kPsd:
    case ECbmModuleId::kT0: {
      SetReferenceDetector(kEventBuilderDetT0);
      break;
    }  // case ECbmModuleId::kT0:
    default: {
      LOG(fatal)
        << "CbmMcbm2019TimeWinEventBuilderAlgo::SetReferenceDetector => "
        << "Trying to change reference to unsupported det: " << refDet
        << std::endl
        << "This method is deprecated and will be removed soon, "
        << "please use the full version of it.";
      break;
    }  // default:
  }    // switch( refDet )

  LOG(warning)
    << "CbmMcbm2019TimeWinEventBuilderAlgo::SetReferenceDetector => "
    << "Changing reference detector with deprecated method to: "
    << fRefDet.sName << std::endl
    << "This method is deprecated, does not set all parameters "
    << "and will be removed soon, please use the full version of it.";
}
void CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector(ECbmModuleId selDet) {
  switch (selDet) {
    case ECbmModuleId::kSts: {
      AddDetector(kEventBuilderDetSts);
      break;
    }  // case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch: {
      AddDetector(kEventBuilderDetMuch);
      break;
    }  // case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd: {
      AddDetector(kEventBuilderDetTrd);
      break;
    }  // case ECbmModuleId::kTrd:
    case ECbmModuleId::kTof: {
      AddDetector(kEventBuilderDetTof);
      break;
    }  // case ECbmModuleId::kTof:
    case ECbmModuleId::kRich: {
      AddDetector(kEventBuilderDetRich);
      break;
    }  // case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd: {
      AddDetector(kEventBuilderDetPsd);
      break;
    }  // case ECbmModuleId::kPsd:
    case ECbmModuleId::kT0: {
      AddDetector(kEventBuilderDetT0);
      break;
    }  // case ECbmModuleId::kT0:
    default: {
      LOG(fatal) << "CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector => "
                 << "Trying to change reference to unsupported det: " << selDet
                 << std::endl
                 << "This method is deprecated and will be removed soon, "
                 << "please use the full version of it.";
      break;
    }  // default:
  }    // switch( selDet )

  LOG(warning)
    << "CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector => "
    << "Changing reference detector with deprecated method for: " << selDet
    << std::endl
    << "This method is deprecated, does not set all parameters "
    << "and will be removed soon, please use the full version of it.";
}

void CbmMcbm2019TimeWinEventBuilderAlgo::RemoveDetector(ECbmModuleId selDet) {
  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    if ((*det).detId == selDet) {
      fvDets.erase(det);
      return;
    }  // if( (*det).detId  == selDet )
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  LOG(warning) << "CbmMcbm2019TimeWinEventBuilderAlgo::RemoveDetector => Doing "
                  "nothing, selection detector not in list!"
               << selDet;
}
//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::SetReferenceDetector(
  EventBuilderDetector refDetIn) {
  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    if ((*det) == refDetIn) {
      LOG(warning)
        << "CbmMcbm2019TimeWinEventBuilderAlgo::SetReferenceDetector => "
           "Reference detector already in selection detector list!"
        << refDetIn.sName;
      LOG(warning)
        << "                                                         => "
           "It will be automatically removed from selection detector list!";
      LOG(warning)
        << "                                                         => "
           "Please also remember to update the selection windows to store "
           "clusters!";
      RemoveDetector(refDetIn);
    }  // if( (*det)  == refDetIn )
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  if (fRefDet == refDetIn) {
    LOG(warning)
      << "CbmMcbm2019TimeWinEventBuilderAlgo::SetReferenceDetector => "
         "Doing nothing, identical reference detector already in use";
  }  // if( fRefDet == refDetIn )
  else {
    LOG(info) << "CbmMcbm2019TimeWinEventBuilderAlgo::SetReferenceDetector => "
              << "Replacing " << fRefDet.sName << " with " << refDetIn.sName
              << " as reference detector";
    LOG(warning)
      << "                                                         => "
         "You may want to use AddDetector after this command to add in "
         "selection "
      << refDetIn.sName;
    LOG(warning)
      << "                                                         => "
         "Please also remember to update the selection windows!";
  }  // else of if( fRefDet == refDetIn )
  fRefDet = refDetIn;

  /// Update the variables storing the earliest and latest time window boundaries
  UpdateTimeWinBoundariesExtrema();
  /// Update the variable storing the size if widest time window for overlap detection
  UpdateWidestTimeWinRange();
}
void CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector(
  EventBuilderDetector selDet) {
  if (fRefDet == selDet) {
    LOG(fatal) << "CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector => Cannot "
                  "add the reference detector as selection detector!"
               << std::endl
               << "=> Maybe first change the reference detector with "
                  "SetReferenceDetector?";
  }  // if( fRefDet == selDet )

  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    if ((*det) == selDet) {
      LOG(warning) << "CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector => "
                      "Doing nothing, selection detector already in list!"
                   << selDet.sName;
      return;
    }  // if( (*det)  == selDet )
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  fvDets.push_back(selDet);

  /// Update the variables storing the earliest and latest time window boundaries
  UpdateTimeWinBoundariesExtrema();
  /// Update the variable storing the size if widest time window for overlap detection
  UpdateWidestTimeWinRange();
}
void CbmMcbm2019TimeWinEventBuilderAlgo::RemoveDetector(
  EventBuilderDetector selDet) {
  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    if ((*det) == selDet) {
      fvDets.erase(det);
      return;
    }  // if( (*det)  == selDet )
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  LOG(warning) << "CbmMcbm2019TimeWinEventBuilderAlgo::RemoveDetector => Doing "
                  "nothing, selection detector not in list!"
               << selDet.sName;
}
//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerMinNumber(
  ECbmModuleId selDet,
  UInt_t uVal) {
  /// Check first if reference detector
  if (fRefDet.detId == selDet) {
    fRefDet.fuTriggerMinDigis = uVal;

    LOG(debug) << "Set Trigger min limit for " << fRefDet.sName << " to "
               << uVal;

    return;
  }  // if( fRefDet == selDet )

  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    if ((*det).detId == selDet) {
      (*det).fuTriggerMinDigis = uVal;

      LOG(debug) << "Set Trigger min limit for " << (*det).sName << " to "
                 << uVal;

      return;
    }  // if( (*det).detId  == selDet )
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  LOG(warning)
    << "CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerMinNumber => "
       "Doing nothing, detector neither reference nor in selection list!"
    << selDet;
}
void CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerMaxNumber(
  ECbmModuleId selDet,
  Int_t iVal) {
  /// Check first if reference detector
  if (fRefDet.detId == selDet) {
    fRefDet.fiTriggerMaxDigis = iVal;

    LOG(debug) << "Set Trigger min limit for " << fRefDet.sName << " to "
               << iVal;

    return;
  }  // if( fRefDet == selDet )

  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    if ((*det).detId == selDet) {
      (*det).fiTriggerMaxDigis = iVal;

      LOG(debug) << "Set Trigger min limit for " << (*det).sName << " to "
                 << iVal;

      return;
    }  // if( (*det).detId  == selDet )
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  LOG(warning)
    << "CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerMaxNumber => "
       "Doing nothing, detector neither reference nor in selection list!"
    << selDet;
}
void CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerWindow(ECbmModuleId selDet,
                                                          Double_t dWinBeg,
                                                          Double_t dWinEnd) {
  /// Check if valid time window: end strictly after beginning
  if (dWinEnd <= dWinBeg)
    LOG(fatal) << "CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerWindow => "
                  "Invalid time window: [ "
               << dWinBeg << ", " << dWinEnd << " ]";

  Bool_t bFound = kFALSE;
  /// Check first if reference detector
  if (fRefDet.detId == selDet) {
    fRefDet.fdTimeWinBeg = dWinBeg;
    fRefDet.fdTimeWinEnd = dWinEnd;

    bFound = kTRUE;
  }  // if( fRefDet == selDet )

  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    if ((*det).detId == selDet) {
      (*det).fdTimeWinBeg = dWinBeg;
      (*det).fdTimeWinEnd = dWinEnd;

      bFound = kTRUE;
    }  // if( (*det).detId  == selDet )
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  if (kFALSE == bFound) {
    LOG(warning)
      << "CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerWindow => "
         "Doing nothing, detector neither reference nor in selection list!"
      << selDet;
  }  // if( kFALSE == bFound )

  /// Update the variables storing the earliest and latest time window boundaries
  UpdateTimeWinBoundariesExtrema();
  /// Update the variable storing the size if widest time window for overlap detection
  UpdateWidestTimeWinRange();
}
void CbmMcbm2019TimeWinEventBuilderAlgo::UpdateTimeWinBoundariesExtrema() {
  /// Initialize with reference detector
  fdEarliestTimeWinBeg = fRefDet.fdTimeWinBeg;
  fdLatestTimeWinEnd   = fRefDet.fdTimeWinEnd;

  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    fdEarliestTimeWinBeg = std::min(fdEarliestTimeWinBeg, (*det).fdTimeWinBeg);
    fdLatestTimeWinEnd   = std::max(fdLatestTimeWinEnd, (*det).fdTimeWinEnd);
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
}
void CbmMcbm2019TimeWinEventBuilderAlgo::UpdateWidestTimeWinRange() {
  /// Initialize with reference detector
  fdWidestTimeWinRange = fRefDet.fdTimeWinEnd - fRefDet.fdTimeWinBeg;

  /// Loop on selection detectors
  for (std::vector<EventBuilderDetector>::iterator det = fvDets.begin();
       det != fvDets.end();
       ++det) {
    fdWidestTimeWinRange =
      std::max(fdWidestTimeWinRange, (*det).fdTimeWinEnd - (*det).fdTimeWinBeg);
  }  // for( std::vector< EventBuilderDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
}
//----------------------------------------------------------------------

ClassImp(CbmMcbm2019TimeWinEventBuilderAlgo)
