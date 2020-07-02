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
#include "TimesliceMetaData.h"
#include "CbmStsDigi.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmTrdDigi.h"
#include "CbmTofDigi.h"
#include "CbmRichDigi.h"
#include "CbmPsdDigi.h"

#include "CbmDigiManager.h"

/// FAIRROOT headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "TClonesArray.h"
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"
#include "THttpServer.h"

/// C/C++ headers

// ---- Default constructor --------------------------------------------
CbmMcbm2019TimeWinEventBuilderAlgo::CbmMcbm2019TimeWinEventBuilderAlgo()
{
}

// ---- Destructor -----------------------------------------------------
CbmMcbm2019TimeWinEventBuilderAlgo::~CbmMcbm2019TimeWinEventBuilderAlgo()
{
}

// ---- Init -----------------------------------------------------------
Bool_t CbmMcbm2019TimeWinEventBuilderAlgo::InitAlgo()
{
   LOG(info) << "CbmMcbm2019TimeWinEventBuilderAlgo::InitAlgo => Starting sequence";

   // Get a handle from the IO manager
   FairRootManager* ioman = FairRootManager::Instance();

   // Get a pointer to the previous already existing data level
   fDigiMan = CbmDigiManager::Instance();
   fDigiMan->UseMuchBeamTimeDigi();
   fDigiMan->Init();

   // T0 is not included in DigiManager
   fT0DigiVec = ioman->InitObjectAs<std::vector<CbmTofDigi> const *>("T0Digi");
   if( ! fT0DigiVec )
   {
      LOG(info) << "No T0 digi input.";
      if( ECbmModuleId::kT0 == fRefDet )
      {
         LOG(fatal) << "No digi input for reference detector, stopping there!";
      } // if( ECbmModuleId::kT0 == fRefDet )
   } // if( ! fT0DigiVec )

   if( ! fDigiMan->IsPresent(ECbmModuleId::kSts) )
   {
      LOG(info) << "No STS digi input.";
      if( ECbmModuleId::kSts == fRefDet )
      {
         LOG(fatal) << "No digi input for reference detector, stopping there!";
      } // if( ECbmModuleId::kT0 == fRefDet )
   } // if( ! fDigiMan->IsPresent(ECbmModuleId::kSts) )

   if( ! fDigiMan->IsPresent(ECbmModuleId::kMuch) )
   {
      LOG(info) << "No MUCH digi input.";
      if( ECbmModuleId::kMuch == fRefDet )
      {
         LOG(fatal) << "No digi input for reference detector, stopping there!";
      } // if( ECbmModuleId::kT0 == fRefDet )
   } // if( ! fDigiMan->IsPresent(ECbmModuleId::kMuch) )

   if( ! fDigiMan->IsPresent(ECbmModuleId::kTrd) )
   {
      LOG(info) << "No TRD digi input.";
      if( ECbmModuleId::kTrd == fRefDet )
      {
         LOG(fatal) << "No digi input for reference detector, stopping there!";
      } // if( ECbmModuleId::kT0 == fRefDet )
   } // if( ! fDigiMan->IsPresent(ECbmModuleId::kTrd) )

   if( ! fDigiMan->IsPresent(ECbmModuleId::kTof) )
   {
      LOG(info) << "No TOF digi input.";
      if( ECbmModuleId::kTof == fRefDet )
      {
         LOG(fatal) << "No digi input for reference detector, stopping there!";
      } // if( ECbmModuleId::kT0 == fRefDet )
   } // if( ! fDigiMan->IsPresent(ECbmModuleId::kTof) )

   if( ! fDigiMan->IsPresent(ECbmModuleId::kRich) )
   {
      LOG(info) << "No RICH digi input.";
      if( ECbmModuleId::kRich == fRefDet )
      {
         LOG(fatal) << "No digi input for reference detector, stopping there!";
      } // if( ECbmModuleId::kT0 == fRefDet )
   } // if( ! fDigiMan->IsPresent(ECbmModuleId::kRich) )

   if( ! fDigiMan->IsPresent(ECbmModuleId::kPsd) )
   {
      LOG(info) << "No PSD digi input.";
      if( ECbmModuleId::kPsd == fRefDet )
      {
         LOG(fatal) << "No digi input for reference detector, stopping there!";
      } // if( ECbmModuleId::kT0 == fRefDet )
   } // if( ! fDigiMan->IsPresent(ECbmModuleId::kPsd) )

   /// Access the TS metadata to know TS start tim
   fTimeSliceMetaDataArray = dynamic_cast<TClonesArray*>(ioman->GetObject("TimesliceMetaData"));
   if ( ! fTimeSliceMetaDataArray )
      LOG(fatal) << "No TS metadata input found";

   /// Store the time window for the reference detector
   switch( fRefDet )
   {
      case ECbmModuleId::kSts:
      {
         fdRefTimeWinBeg = fdStsTimeWinBeg;
         fdRefTimeWinEnd = fdStsTimeWinEnd;
         break;
      } // case ECbmModuleId::kSts:
      case ECbmModuleId::kMuch:
      {
         fdRefTimeWinBeg = fdMuchTimeWinBeg;
         fdRefTimeWinEnd = fdMuchTimeWinEnd;
         break;
      } // case ECbmModuleId::kMuch:
      case ECbmModuleId::kTrd:
      {
         fdRefTimeWinBeg = fdTrdTimeWinBeg;
         fdRefTimeWinEnd = fdTrdTimeWinEnd;
         break;
      } // case ECbmModuleId::kTrd:
      case ECbmModuleId::kTof:
      {
         fdRefTimeWinBeg = fdTofTimeWinBeg;
         fdRefTimeWinEnd = fdTofTimeWinEnd;
         break;
      } // case ECbmModuleId::kTof:
      case ECbmModuleId::kRich:
      {
         fdRefTimeWinBeg = fdRichTimeWinBeg;
         fdRefTimeWinEnd = fdRichTimeWinEnd;
         break;
      } // case ECbmModuleId::kRich:
      case ECbmModuleId::kPsd:
      {
         fdRefTimeWinBeg = fdPsdTimeWinBeg;
         fdRefTimeWinEnd = fdPsdTimeWinEnd;
         break;
      } // case ECbmModuleId::kPsd:
      case ECbmModuleId::kT0:
      {
         fdRefTimeWinBeg = fdT0TimeWinBeg;
         fdRefTimeWinEnd = fdT0TimeWinEnd;
         break;
      } // case ECbmModuleId::kT0:
      default:
      {
         LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::Init => "
                      << "Trying to use unsupported detectore as reference: "
                      << fRefDet;
         break;
      } // default:
   } // switch( fRefDet )

   if( fbFillHistos )
   {
      CreateHistograms();
   } // if( fbFillHistos )

   LOG(info) << "CbmMcbm2019TimeWinEventBuilderAlgo::InitAlgo => Done";

   return kTRUE;
}

// ---- ProcessTs ------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::ProcessTs()
{
   LOG_IF(info, fuNrTs%1000 == 0) <<"Begin of TS " << fuNrTs;

   InitTs();

   BuildEvents();

   /// Store last event with trigger if not done by other seed
   if( nullptr != fCurrentEvent )
   {
      /// TODO: store start time of current event ?
      //        fCurrentEvent->SetStartTime( fPrevTime ); // Replace Seed time with time of first digi in event?
      fCurrentEvent->SetEndTime( fdPrevEvtEndTime );
      fEventVector.push_back( fCurrentEvent );
      fuCurEv++;

      /// Prevent building over TS edge
      fCurrentEvent = nullptr;
   } // if( nullptr != fCurrentEvent )

   LOG(debug) << "Found " << fEventVector.size() << " triggered events";

   if( fbFillHistos )
   {
      FillHistos();
   } // if( fbFillHistos )

   fuNrTs++;
}
void CbmMcbm2019TimeWinEventBuilderAlgo::ClearEventVector()
{
   /// Need to delete the object the pointer points to first
   int counter = 0;
   for( CbmEvent* event: fEventVector )
   {
      LOG(debug) << "Event " << counter << " has " << event->GetNofData() << " digis";
      delete event;
      counter++;
   } // for( CbmEvent* event: fEventVector)

  fEventVector.clear();
}
// ---- Finish ---------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::Finish()
{
   LOG(info) << "Total errors: " << fuErrors;
}

// ---------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::InitTs()
{
   /// Reset TS based variables (analysis per TS = no building over the border)
   fuStartIndexT0   = 0;
   fuStartIndexSts  = 0;
   fuStartIndexMuch = 0;
   fuStartIndexTrd  = 0;
   fuStartIndexTof  = 0;
   fuStartIndexRich = 0;
   fuStartIndexPsd  = 0;
   fuEndIndexT0   = 0;
   fuEndIndexSts  = 0;
   fuEndIndexMuch = 0;
   fuEndIndexTrd  = 0;
   fuEndIndexTof  = 0;
   fuEndIndexRich = 0;
   fuEndIndexPsd  = 0;
}

void CbmMcbm2019TimeWinEventBuilderAlgo::BuildEvents()
{
  /// Call LoopOnSeed with proper template argument
  switch( fRefDet )
  {
    case ECbmModuleId::kSts:
    {
      LoopOnSeeds< CbmStsDigi >();
      break;
    } // case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch:
    {
      LoopOnSeeds< CbmMuchBeamTimeDigi >();
      break;
    } // case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:
    {
      LoopOnSeeds< CbmTrdDigi >();
      break;
    } // case ECbmModuleId::kTrd:
    case ECbmModuleId::kTof:
    {
      LoopOnSeeds< CbmTofDigi >();
      break;
    } // case ECbmModuleId::kTof:
    case ECbmModuleId::kRich:
    {
      LoopOnSeeds< CbmRichDigi >();
      break;
    } // case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd:
    {
      LoopOnSeeds< CbmPsdDigi >();
      break;
    } // case ECbmModuleId::kPsd:
    case ECbmModuleId::kT0:
    {
      LoopOnSeeds< CbmTofDigi >();
      break;
    } // case ECbmModuleId::kT0:
    default:
    {
      LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::BuildEvents => "
                   << "Trying to search event seeds with unsupported det: "
                   << fRefDet;
      break;
    } // default:
  } // switch( *det )
}

template< class DigiSeed >
void CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds()
{
   pTsMetaData = dynamic_cast<TimesliceMetaData*>(fTimeSliceMetaDataArray->At(0));
   if( nullptr == pTsMetaData )
      LOG(fatal) << Form( "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => No TS metadata found for TS %6u.", fuNrTs );

  /// Print warning in first TS if time window borders out of potential overlap
   if( ( 0.0 < fdEarliestTimeWinBeg && pTsMetaData->GetOverlapDuration() < fdLatestTimeWinEnd ) ||
       ( pTsMetaData->GetOverlapDuration() < fdWidestTimeWinRange ) )
   {
      LOG(warning) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                   << Form( "Event window not fitting in TS overlap, risk of incomplete events: %f %f %f %llu",
                            fdEarliestTimeWinBeg, fdLatestTimeWinEnd, fdWidestTimeWinRange, pTsMetaData->GetOverlapDuration() );
   } // if end of event window does not fit in overlap for a seed at edge of TS core

   /// Define an acceptance window for the seeds in order to use the overlap
   /// part of the TS to avoid incomplete events
   Double_t dSeedWindowBeg = pTsMetaData->GetStartTime() + ( 0.0 < fdEarliestTimeWinBeg ? 0.0 : -fdEarliestTimeWinBeg );
   Double_t dSeedWindowEnd = pTsMetaData->GetOverlapStartTime() + ( 0.0 < fdEarliestTimeWinBeg ? 0.0 : -fdEarliestTimeWinBeg );
   if( fbIgnoreTsOverlap )
   {
      dSeedWindowBeg = pTsMetaData->GetStartTime();
      dSeedWindowEnd = pTsMetaData->GetOverlapStartTime();
   } // if( fbIgnoreTsOverlap )

  switch( fRefDet )
  {
    case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:
    case ECbmModuleId::kTof:
    case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd:
    {
      UInt_t uNbRefDigis = ( 0 < fDigiMan->GetNofDigis( fRefDet ) ? fDigiMan->GetNofDigis( fRefDet ) : 0 );
      /// Loop on size of vector
      for( UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi )
      {
        LOG( debug ) << Form( "Checking seed %6u / %6u", uDigi, uNbRefDigis );
        const DigiSeed * pDigi = fDigiMan->Get< DigiSeed >( uDigi );
        /// Check that _entry is not out of range
        if( nullptr != pDigi )
        {
          Double_t dTime = pDigi->GetTime();

          /// Check if seed in acceptance window
          if( dTime < dSeedWindowBeg )
          {
            continue;
          } // if( dTime < dSeedWindowBeg )
            else if( dSeedWindowEnd < dTime )
            {
               break;
            } // else if( dSeedWindowEnd < dTime )

          /// Check Seed and build event if needed
          CheckSeed( dTime, uDigi );
        } // if( nullptr != pDigi )
      } // for( UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi )
      break;
    } // Digi containers controlled by DigiManager
    case ECbmModuleId::kT0:
    {
      if ( fT0DigiVec )
      {
        /// Loop on size of vector
        UInt_t uNbRefDigis = fT0DigiVec->size();
        /// Loop on size of vector
        for( UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi )
        {
          LOG( debug ) << Form( "Checking seed %6u / %6u", uDigi, uNbRefDigis );

          Double_t dTime = fT0DigiVec->at( uDigi ).GetTime();

          /// Check Seed and build event if needed
          CheckSeed( dTime, uDigi );
        } // for( UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi )
      } // if ( fT0DigiVec )
        else LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                          << "T0 as reference detector but vector not found!";
      break;
    } // case ECbmModuleId::kT0
    default:
    {
      LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => Unknow reference detector enum! " << fRefDet;
      break;
    } // default:
  } // switch( fRefDet )
}

void CbmMcbm2019TimeWinEventBuilderAlgo::CheckSeed( Double_t dSeedTime, UInt_t uSeedDigiIdx )
{
  /// If previous event valid and event overlap not allowed, check if we are in overlap
  /// and react accordingly
  if( nullptr != fCurrentEvent &&
      EOverlapMode::AllowOverlap != fOverMode &&
      dSeedTime - fdPrevEvtTime < fdWidestTimeWinRange )
  {
    /// Within overlap range
    switch( fOverMode )
    {
      case EOverlapMode::NoOverlap:
      {
        /// No overlap allowed => reject
        LOG( debug1 ) << "Reject seed due to overlap";
        return;
        break;
      } // case EOverlapMode::NoOverlap:
      case EOverlapMode::MergeOverlap:
      {
        /// Merge overlap mode => do nothing and go on filling current event
        break;
      } // case EOverlapMode::MergeOverlap:
      case EOverlapMode::AllowOverlap:
      {
        /// Not in Merge overlap mode => should have been catched before, nothing to do
        break;
      } // case EOverlapMode::AllowOverlap:
    } // switch( fOverMode )
  } // if( prev Event exists and overlap not allowed and overlap present )
    else
    {
      /// Out of overlap range or in overlap allowed mode
      /// => store previous event if not empty and create new one
      if( nullptr != fCurrentEvent )
      {
        /// TODO: store start time of current event ?
//        fCurrentEvent->SetStartTime( fPrevTime ); // Replace Seed time with time of first digi in event?
        fCurrentEvent->SetEndTime( fdPrevEvtEndTime );
        fEventVector.push_back( fCurrentEvent );
        fuCurEv++;
      } // if( nullptr != fCurrentEvent )
      fCurrentEvent = new CbmEvent( fuCurEv, dSeedTime, 0.);
    } // else of if( prev Event exists and overlap not allowed and overlap present )

  /// If window open for reference detector, search for other reference Digis matching it
  /// Otherwise only add the current seed
  if( fdRefTimeWinBeg < fdRefTimeWinEnd )
  {
    switch( fRefDet )
    {
      case ECbmModuleId::kSts:
      {
        SearchMatches< CbmStsDigi >( dSeedTime, fRefDet, fuStartIndexSts );
        break;
      } // case ECbmModuleId::kSts:
      case ECbmModuleId::kMuch:
      {
        SearchMatches< CbmMuchBeamTimeDigi >( dSeedTime, fRefDet, fuStartIndexMuch );
        break;
      } // case ECbmModuleId::kMuch:
      case ECbmModuleId::kTrd:
      {
        SearchMatches< CbmTrdDigi >( dSeedTime, fRefDet, fuStartIndexTrd );
        break;
      } // case ECbmModuleId::kTrd:
      case ECbmModuleId::kTof:
      {
        SearchMatches< CbmTofDigi >( dSeedTime, fRefDet, fuStartIndexTof );
        break;
      } // case ECbmModuleId::kTof:
      case ECbmModuleId::kRich:
      {
        SearchMatches< CbmRichDigi >( dSeedTime, fRefDet, fuStartIndexRich );
        break;
      } // case ECbmModuleId::kRich:
      case ECbmModuleId::kPsd:
      {
        SearchMatches< CbmPsdDigi >( dSeedTime, fRefDet, fuStartIndexPsd );
        break;
      } // case ECbmModuleId::kPsd:
      case ECbmModuleId::kT0:
      {
        SearchMatches< CbmTofDigi >( dSeedTime, fRefDet, fuStartIndexT0 );
        break;
      } // case ECbmModuleId::kT0:
      default:
      {
        LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                     << "Trying to search matches with unsupported det: "
                     << fRefDet;
        break;
      } // default:
    } // switch( fRefDet )

    /// Also add the seed if the window starts after the seed
    if( 0 < fdRefTimeWinBeg )
      AddDigiToEvent( fRefDet, uSeedDigiIdx );
  } // if( fdRefTimeWinBeg < fdRefTimeWinEnd )
    else
    {
      AddDigiToEvent( fRefDet, uSeedDigiIdx );
    } // else of if( fdRefTimeWinBeg < fdRefTimeWinEnd )

  /// Search for matches for each detector in selection list
  for( std::vector< ECbmModuleId >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  {
    switch( *det )
    {
      case ECbmModuleId::kSts:
      {
        SearchMatches< CbmStsDigi >( dSeedTime, *det, fuStartIndexSts );
        break;
      } // case ECbmModuleId::kSts:
      case ECbmModuleId::kMuch:
      {
        SearchMatches< CbmMuchBeamTimeDigi >( dSeedTime, *det, fuStartIndexMuch );
        break;
      } // case ECbmModuleId::kMuch:
      case ECbmModuleId::kTrd:
      {
        SearchMatches< CbmTrdDigi >( dSeedTime, *det, fuStartIndexTrd );
        break;
      } // case ECbmModuleId::kTrd:
      case ECbmModuleId::kTof:
      {
        SearchMatches< CbmTofDigi >( dSeedTime, *det, fuStartIndexTof );
        break;
      } // case ECbmModuleId::kTof:
      case ECbmModuleId::kRich:
      {
        SearchMatches< CbmRichDigi >( dSeedTime, *det, fuStartIndexRich );
        break;
      } // case ECbmModuleId::kRich:
      case ECbmModuleId::kPsd:
      {
        SearchMatches< CbmPsdDigi >( dSeedTime, *det, fuStartIndexPsd );
        break;
      } // case ECbmModuleId::kPsd:
      case ECbmModuleId::kT0:
      {
        SearchMatches< CbmTofDigi >( dSeedTime, *det, fuStartIndexT0 );
        break;
      } // case ECbmModuleId::kT0:
      default:
      {
        LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                     << "Trying to search matches with unsupported det: "
                     << *det;
        break;
      } // default:
    } // switch( *det )
  } // for( std::vector< ECbmModuleId >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  /// Check if event is filling trigger conditions and clear it if not
  if( HasTrigger( fCurrentEvent ) )
  {
    fdPrevEvtTime = dSeedTime;

    /// In case of NoOverlap or MergeOverlap, we can and should start checking teh next window
    /// from end of current window in order to save CPU and avoid duplicating
    if( EOverlapMode::NoOverlap == fOverMode ||
        EOverlapMode::MergeOverlap == fOverMode )
    {
      fuStartIndexT0   = fuEndIndexT0;
      fuStartIndexSts  = fuEndIndexSts;
      fuStartIndexMuch = fuEndIndexMuch;
      fuStartIndexTrd  = fuEndIndexTrd;
      fuStartIndexTof  = fuEndIndexTof;
      fuStartIndexRich = fuEndIndexRich;
      fuStartIndexPsd  = fuEndIndexPsd;
    } // If no overlap or merge overlap
  } // if( !HasTrigger( fCurrentEvent ) )
    else
    {
      LOG( debug1 ) << "Reject seed due to Trigger requirements";
      delete fCurrentEvent;
      fCurrentEvent = nullptr; /// delete does NOT set a pointer to nullptr...
    } // else of if( !HasTrigger( fCurrentEvent ) )
}

template < class DigiCheck >
void CbmMcbm2019TimeWinEventBuilderAlgo::SearchMatches( Double_t dSeedTime, ECbmModuleId detMatch, UInt_t & uStartIndex )
{
  /// This algo relies on time sorted vectors for the selected detectors
  UInt_t uLocalIndexStart = uStartIndex;
  UInt_t uLocalIndexEnd   = uStartIndex;

  /// FIXME: Use method parameters instead to save 1 switch?
  Double_t dTimeWinBeg = 0;
  Double_t dTimeWinEnd = 0;
  switch( detMatch )
  {
    case ECbmModuleId::kSts:
    {
      dTimeWinBeg = fdStsTimeWinBeg;
      dTimeWinEnd = fdStsTimeWinEnd;
      break;
    } // case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch:
    {
      dTimeWinBeg = fdMuchTimeWinBeg;
      dTimeWinEnd = fdMuchTimeWinEnd;
      break;
    } // case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:
    {
      dTimeWinBeg = fdTrdTimeWinBeg;
      dTimeWinEnd = fdTrdTimeWinEnd;
      break;
    } // case ECbmModuleId::kTrd:
    case ECbmModuleId::kTof:
    {
      dTimeWinBeg = fdTofTimeWinBeg;
      dTimeWinEnd = fdTofTimeWinEnd;
      break;
    } // case ECbmModuleId::kTof:
    case ECbmModuleId::kRich:
    {
      dTimeWinBeg = fdRichTimeWinBeg;
      dTimeWinEnd = fdRichTimeWinEnd;
      break;
    } // case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd:
    {
      dTimeWinBeg = fdPsdTimeWinBeg;
      dTimeWinEnd = fdPsdTimeWinEnd;
      break;
    } // case ECbmModuleId::kPsd:
    case ECbmModuleId::kT0:
    {
      dTimeWinBeg = fdT0TimeWinBeg;
      dTimeWinEnd = fdT0TimeWinEnd;
      break;
    } // case ECbmModuleId::kT0:
    default:
    {
      LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::SearchMatches => "
                   << "Trying to search matches with unsupported det: "
                   << detMatch;
      break;
    } // default:
  } // switch( detMatch )

  /// Check the Digis until out of window
  switch( detMatch )
  {
    case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:
    case ECbmModuleId::kTof:
    case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd:
    {
      UInt_t uNbSelDigis = ( 0 < fDigiMan->GetNofDigis( detMatch ) ? fDigiMan->GetNofDigis( detMatch ) : 0 );
      /// Loop on size of vector
      for( UInt_t uDigi = uStartIndex; uDigi < uNbSelDigis; ++uDigi )
      {
        const DigiCheck * pDigi = fDigiMan->Get< DigiCheck >( uDigi );
        /// Check that _entry is not out of range
        if( nullptr != pDigi )
        {
          Double_t dTime     = pDigi->GetTime();
          Double_t dTimeDiff = dTime - dSeedTime;

          LOG( debug4 ) << detMatch << Form( " => Checking match %6u / %6u, dt %f", uDigi, uNbSelDigis, dTimeDiff );

          /// Check if within time window, update start/stop indices if needed
          if( dTimeDiff < dTimeWinBeg )
          {
            ++uLocalIndexStart;
            continue;
          } // if( dTimeDiff < dTimeWinBeg )
            else if( dTimeWinEnd < dTimeDiff )
            {
              /// Store as end the first digi out of window to avoid double counting in case of
              /// merged overlap event mode
              uLocalIndexEnd = uDigi;
              break;
            } // else if( dTimeWinEnd < dTimeDiff ) of if( dTimeDiff < dTimeWinBeg )

            AddDigiToEvent( detMatch, uDigi );

            if( fdPrevEvtEndTime < dTime )
              fdPrevEvtEndTime = dTime;
        } // if( nullptr != pDigi )
      } // for( UInt_t uDigi = 0; uDigi < uNbSelDigis; ++uDigi )

      /// catch the case where we reach the end of the vector before being out of the time window
      if( uLocalIndexEnd < uLocalIndexStart )
        uLocalIndexEnd = uNbSelDigis;

      break;
    } // Digi containers controlled by DigiManager
    case ECbmModuleId::kT0:
    {
      if ( fT0DigiVec )
      {
        /// Loop on size of vector
        UInt_t uNbSelDigis = fT0DigiVec->size();
        /// Loop on size of vector
        for( UInt_t uDigi = uStartIndex; uDigi < uNbSelDigis; ++uDigi )
        {
          Double_t dTime = fT0DigiVec->at( uDigi ).GetTime();

          Double_t dTimeDiff = dTime - dSeedTime;

          /// Check if within time window, update start/stop indices if needed
          if( dTimeDiff < dTimeWinBeg )
          {
            ++uLocalIndexStart;
            continue;
          } // if( dTimeDiff < dTimeWinBeg )
            else if( dTimeWinEnd < dTimeDiff )
            {
              /// Store as end the first digi out of window to avoid double counting in case of
              /// merged overlap event mode
              uLocalIndexEnd = uDigi;
              break;
            } // else if( dTimeWinEnd < dTimeDiff ) of if( dTimeDiff < dTimeWinBeg )

            AddDigiToEvent( detMatch, uDigi );

            if( fdPrevEvtEndTime < dTime )
              fdPrevEvtEndTime = dTime;
        } // for( UInt_t uDigi = 0; uDigi < uNbSelDigis; ++uDigi )

        /// catch the case where we reach the end of the vector before being out of the time window
        if( uLocalIndexEnd < uLocalIndexStart )
          uLocalIndexEnd = uNbSelDigis;
      } // if ( fT0DigiVec )
        else LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::SearchMatches => "
                          << "T0 as selection detector but vector not found!";

      break;
    } // case ECbmModuleId::kT0
    default:
    {
      return;
      break;
    } // default:
  } // switch( detMatch )

  /// Update the StartIndex and EndIndex for the next event seed
  switch( detMatch )
  {
    case ECbmModuleId::kSts:
    {
      fuStartIndexSts = uLocalIndexStart;
      fuEndIndexSts   = uLocalIndexEnd;
      break;
    } // case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch:
    {
      fuStartIndexMuch = uLocalIndexStart;
      fuEndIndexMuch   = uLocalIndexEnd;
      break;
    } // case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:
    {
      fuStartIndexTrd = uLocalIndexStart;
      fuEndIndexTrd   = uLocalIndexEnd;
      break;
    } // case ECbmModuleId::kTrd:
    case ECbmModuleId::kTof:
    {
      fuStartIndexTof = uLocalIndexStart;
      fuEndIndexTof   = uLocalIndexEnd;
      break;
    } // case ECbmModuleId::kTof:
    case ECbmModuleId::kRich:
    {
      fuStartIndexRich = uLocalIndexStart;
      fuEndIndexRich   = uLocalIndexEnd;
      break;
    } // case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd:
    {
      fuStartIndexPsd = uLocalIndexStart;
      fuEndIndexPsd   = uLocalIndexEnd;
      break;
    } // case ECbmModuleId::kPsd:
    case ECbmModuleId::kT0:
    {
      fuStartIndexT0 = uLocalIndexStart;
      fuEndIndexT0   = uLocalIndexEnd;
      break;
    } // case ECbmModuleId::kT0:
    default:
    {
      return;
      break;
    } // default:
  } // switch( detMatch )
}

void CbmMcbm2019TimeWinEventBuilderAlgo::AddDigiToEvent( ECbmModuleId _system, Int_t _entry )
{
  // Fill digi index into event
  switch (_system) {
    case ECbmModuleId::kMvd:  fCurrentEvent->AddData( ECbmDataType::kMvdDigi,  _entry ); break;
    case ECbmModuleId::kSts:  fCurrentEvent->AddData( ECbmDataType::kStsDigi,  _entry ); break;
    case ECbmModuleId::kRich: fCurrentEvent->AddData( ECbmDataType::kRichDigi, _entry ); break;
    case ECbmModuleId::kMuch: fCurrentEvent->AddData( ECbmDataType::kMuchDigi, _entry ); break;
    case ECbmModuleId::kTrd:  fCurrentEvent->AddData( ECbmDataType::kTrdDigi,  _entry ); break;
    case ECbmModuleId::kTof:  fCurrentEvent->AddData( ECbmDataType::kTofDigi,  _entry ); break;
    case ECbmModuleId::kPsd:  fCurrentEvent->AddData( ECbmDataType::kPsdDigi,  _entry ); break;
    case ECbmModuleId::kT0:   fCurrentEvent->AddData( ECbmDataType::kT0Digi,   _entry ); break;
    default:
    break;
  }
}

Bool_t CbmMcbm2019TimeWinEventBuilderAlgo::HasTrigger(CbmEvent* event)
{
  Bool_t hasTrigger{ kTRUE };
  if ( (fT0DigiVec ) && fuTriggerMinT0Digis > 0) {
    Int_t iNbDigis = event->GetNofData( ECbmDataType::kT0Digi );
    hasTrigger = hasTrigger && ( -1 != iNbDigis ) &&
                 ( static_cast< UInt_t >( iNbDigis ) >= fuTriggerMinT0Digis );
    if( !hasTrigger )
    {
       LOG( debug2 ) << "Event does not have enough T0 digis: " << iNbDigis
                    << " vs " << fuTriggerMinT0Digis;
       return hasTrigger;
    } // if( !hasTrigger )
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kSts) && fuTriggerMinStsDigis > 0) {
    Int_t iNbDigis = event->GetNofData( ECbmDataType::kStsDigi );
    hasTrigger = hasTrigger && ( -1 != iNbDigis ) &&
                 ( static_cast< UInt_t >( iNbDigis ) >= fuTriggerMinStsDigis );
    if( !hasTrigger )
    {
       LOG( debug2 ) << "Event does not have enough STS digis: " << iNbDigis
                    << " vs " << fuTriggerMinStsDigis;
       return hasTrigger;
    } // if( !hasTrigger )
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kMuch) && fuTriggerMinMuchDigis > 0) {
    Int_t iNbDigis = event->GetNofData( ECbmDataType::kMuchDigi );
    hasTrigger = hasTrigger && ( -1 != iNbDigis ) &&
                 ( static_cast< UInt_t >( iNbDigis ) >= fuTriggerMinMuchDigis );
    if( !hasTrigger )
    {
       LOG( debug2 ) << "Event does not have enough MUCH digis: " << iNbDigis
                    << " vs " << fuTriggerMinMuchDigis;
       return hasTrigger;
    } // if( !hasTrigger )
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kTrd) && fuTriggerMinTrdDigis > 0) {
    Int_t iNbDigis = event->GetNofData( ECbmDataType::kTrdDigi );
    hasTrigger = hasTrigger && ( -1 != iNbDigis ) &&
                 ( static_cast< UInt_t >( iNbDigis ) >= fuTriggerMinTrdDigis );
    if( !hasTrigger )
    {
       LOG( debug2 ) << "Event does not have enough TRD digis: " << iNbDigis
                    << " vs " << fuTriggerMinTrdDigis;
       return hasTrigger;
    } // if( !hasTrigger )
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kTof) && fuTriggerMinTofDigis > 0) {
    Int_t iNbDigis = event->GetNofData( ECbmDataType::kTofDigi );
    hasTrigger = hasTrigger && ( -1 != iNbDigis ) &&
                 ( static_cast< UInt_t >( iNbDigis ) >= fuTriggerMinTofDigis);
    if( !hasTrigger )
    {
       LOG( debug2 ) << "Event does not have enough TOF digis: " << iNbDigis
                    << " vs " << fuTriggerMinTofDigis;
       return hasTrigger;
    } // if( !hasTrigger )
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kRich) && fuTriggerMinRichDigis > 0) {
    Int_t iNbDigis = event->GetNofData( ECbmDataType::kRichDigi );
    hasTrigger = hasTrigger && ( -1 != iNbDigis ) &&
                 ( static_cast< UInt_t >( iNbDigis ) >= fuTriggerMinRichDigis );
    if( !hasTrigger )
    {
       LOG( debug2 ) << "Event does not have enough RICH digis: " << iNbDigis
                    << " vs " << fuTriggerMinRichDigis;
       return hasTrigger;
    } // if( !hasTrigger )
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kPsd) && fuTriggerMinPsdDigis > 0) {
    Int_t iNbDigis = event->GetNofData( ECbmDataType::kPsdDigi );
    hasTrigger = hasTrigger && ( -1 != iNbDigis ) &&
                 ( static_cast< UInt_t >( iNbDigis ) >= fuTriggerMinPsdDigis );
    if( !hasTrigger )
    {
       LOG( debug2 ) << "Event does not have enough PSD digis: " << iNbDigis
                    << " vs " << fuTriggerMinPsdDigis;
       return hasTrigger;
    } // if( !hasTrigger )
  }

  return hasTrigger;
}

//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::CreateHistograms()
{
  fhEventTime = new TH1F( "hEventTime", "seed time of the events; Seed time [s]; Events",
                        60000, 0, 600 );
  fhEventDt   = new TH1F( "fhEventDt", "interval in seed time of consecutive events; Seed time [s]; Events",
                        2100, -100.5, 1999.5);
  fhEventSize = new TH1F( "hEventSize",
                         "nb of all  digis in the event; Nb Digis []; Events []",
                         10000, 0, 10000 );
  fhNbDigiPerEvtTime= new TH2I( "hNbDigiPerEvtTime",
                                "nb of all  digis per event vs seed time of the events; Seed time [s]; Nb Digis []; Events []",
                                 600, 0,   600,
                                1000, 0, 10000 );

  fhNbDigiPerEvtTimeT0   = new TH2I( "hNbDigiPerEvtTimeT0",
                                     "nb of T0   digis per event vs seed time of the events; Seed time [s]; Nb Digis []; Events []",
                                      600, 0,   600,
                                     4000, 0,  4000 );
  fhNbDigiPerEvtTimeSts  = new TH2I( "hNbDigiPerEvtTimeSts",
                                     "nb of STS  digis per event vs seed time of the events; Seed time [s]; Nb Digis []; Events []",
                                      600, 0,   600,
                                     4000, 0,  4000 );
  fhNbDigiPerEvtTimeMuch = new TH2I( "hNbDigiPerEvtTimeMuch",
                                     "nb of MUCH digis per event vs seed time of the events; Seed time [s]; Nb Digis []; Events []",
                                      600, 0,   600,
                                     4000, 0,  4000 );
  fhNbDigiPerEvtTimeTrd  = new TH2I( "hNbDigiPerEvtTimeTrd",
                                     "nb of TRD  digis per event vs seed time of the events; Seed time [s]; Nb Digis []; Events []",
                                      600, 0,   600,
                                     4000, 0,  4000 );
  fhNbDigiPerEvtTimeTof  = new TH2I( "hNbDigiPerEvtTimeTof",
                                     "nb of TOF  digis per event vs seed time of the events; Seed time [s]; Nb Digis []; Events []",
                                      600, 0,   600,
                                     4000, 0,  4000 );
  fhNbDigiPerEvtTimeRich = new TH2I( "hNbDigiPerEvtTimeRich",
                                     "nb of RICH digis per event vs seed time of the events; Seed time [s]; Nb Digis []; Events []",
                                      600, 0,   600,
                                     4000, 0,  4000 );
  fhNbDigiPerEvtTimePsd  = new TH2I( "hNbDigiPerEvtTimePsd",
                                     "nb of PSD  digis per event vs seed time of the events; Seed time [s]; Nb Digis []; Events []",
                                      600, 0,   600,
                                     4000, 0,  4000 );

  AddHistoToVector( fhEventTime,            "evtbuild" );
  AddHistoToVector( fhEventDt,              "evtbuild" );
  AddHistoToVector( fhEventSize,            "evtbuild" );
  AddHistoToVector( fhNbDigiPerEvtTime,     "evtbuild" );
  AddHistoToVector( fhNbDigiPerEvtTimeT0,   "evtbuild" );
  AddHistoToVector( fhNbDigiPerEvtTimeSts,  "evtbuild" );
  AddHistoToVector( fhNbDigiPerEvtTimeMuch, "evtbuild" );
  AddHistoToVector( fhNbDigiPerEvtTimeTrd,  "evtbuild" );
  AddHistoToVector( fhNbDigiPerEvtTimeTof,  "evtbuild" );
  AddHistoToVector( fhNbDigiPerEvtTimeRich, "evtbuild" );
  AddHistoToVector( fhNbDigiPerEvtTimePsd,  "evtbuild" );
}
void CbmMcbm2019TimeWinEventBuilderAlgo::FillHistos()
{
  Double_t dPreEvtTime = -1.0;
  for( CbmEvent * evt: fEventVector )
  {
    fhEventTime->Fill( evt->GetStartTime() * 1e-9 );
    if( 0.0 <= dPreEvtTime )
    {
      fhEventDt->Fill( evt->GetStartTime() - dPreEvtTime );
    } // if( 0.0 <= dPreEvtTime )
    fhEventSize->Fill( evt->GetNofData() );
    fhNbDigiPerEvtTime->Fill( evt->GetStartTime() * 1e-9, evt->GetNofData() );

    fhNbDigiPerEvtTimeT0  ->Fill( evt->GetStartTime() * 1e-9, evt->GetNofData( ECbmDataType::kT0Digi ) );
    fhNbDigiPerEvtTimeSts ->Fill( evt->GetStartTime() * 1e-9, evt->GetNofData( ECbmDataType::kStsDigi ) );
    fhNbDigiPerEvtTimeMuch->Fill( evt->GetStartTime() * 1e-9, evt->GetNofData( ECbmDataType::kMuchDigi ) );
    fhNbDigiPerEvtTimeTrd ->Fill( evt->GetStartTime() * 1e-9, evt->GetNofData( ECbmDataType::kTrdDigi ) );
    fhNbDigiPerEvtTimeTof ->Fill( evt->GetStartTime() * 1e-9, evt->GetNofData( ECbmDataType::kTofDigi ) );
    fhNbDigiPerEvtTimeRich->Fill( evt->GetStartTime() * 1e-9, evt->GetNofData( ECbmDataType::kRichDigi ) );
    fhNbDigiPerEvtTimePsd ->Fill( evt->GetStartTime() * 1e-9, evt->GetNofData( ECbmDataType::kPsdDigi ) );

    dPreEvtTime = evt->GetStartTime();
  } // for( CbmEvent * evt: fEventVector )
}
void CbmMcbm2019TimeWinEventBuilderAlgo::ResetHistograms( Bool_t /*bResetTime*/ )
{
   fhEventTime->Reset();
   fhEventDt  ->Reset();
   fhEventSize->Reset();
   fhNbDigiPerEvtTime->Reset();
   fhNbDigiPerEvtTimeT0  ->Reset();
   fhNbDigiPerEvtTimeSts ->Reset();
   fhNbDigiPerEvtTimeMuch->Reset();
   fhNbDigiPerEvtTimeTrd ->Reset();
   fhNbDigiPerEvtTimeTof ->Reset();
   fhNbDigiPerEvtTimeRich->Reset();
   fhNbDigiPerEvtTimePsd ->Reset();

/*
   if( kTRUE == bResetTime )
   {
      /// Also reset the Start time for the evolution plots!
      fdStartTime = -1.0;
   } // if( kTRUE == bResetTime )
*/
}
//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector( ECbmModuleId selDet )
{
  /// FIXME: This is not true in case TOF is used as reference !!!!!
  if( fRefDet == selDet )
  {
    LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector => Cannot add the reference detector as selection detector!";
  } // if( fRefDet == selDet )

  for( std::vector< ECbmModuleId >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  {
    if( *det  == selDet )
    {
      LOG( warning ) << "CbmMcbm2019TimeWinEventBuilderAlgo::AddDetector => Doing nothing, selection detector already in list!" << selDet;
      return;
    } // if( *det  == selDet )
  } // for( std::vector< ECbmModuleId >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  fvDets.push_back( selDet );
}
void CbmMcbm2019TimeWinEventBuilderAlgo::RemoveDetector( ECbmModuleId selDet )
{
  for( std::vector< ECbmModuleId >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  {
    if( *det  == selDet )
    {
      fvDets.erase( det );
      return;
    } // if( *det  == selDet )
  } // for( std::vector< ECbmModuleId >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  LOG( warning ) << "CbmMcbm2019TimeWinEventBuilderAlgo::RemoveDetector => Doing nothing, selection detector not in list!" << selDet;
}
//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerMinNumber( ECbmModuleId selDet, UInt_t uVal )
{
  /// Store in corresponding members
  std::string sDet = "";
  switch( selDet )
  {
    case ECbmModuleId::kSts:
    {
      fuTriggerMinStsDigis  = uVal;
      sDet = "STS";
      break;
    }
    case ECbmModuleId::kRich:
    {
      fuTriggerMinRichDigis = uVal;
      sDet = "RICH";
      break;
    }
    case ECbmModuleId::kMuch:
    {
      fuTriggerMinMuchDigis  = uVal;
      sDet = "MUCH";
      break;
    }
    case ECbmModuleId::kTrd:
    {
      fuTriggerMinTrdDigis  = uVal;
      sDet = "TRD";
      break;
    }
    case ECbmModuleId::kTof:
    {
      fuTriggerMinTofDigis = uVal;
      sDet = "TOF";
      break;
    }
    case ECbmModuleId::kPsd:
    {
      fuTriggerMinPsdDigis  = uVal;
      sDet = "PSD";
      break;
    }
    case ECbmModuleId::kT0:
    {
      fuTriggerMinT0Digis   = uVal;
      sDet = "T0";
      break;
    }
    default:
    {
      LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerMinNumber => "
                   << "Unsupported or unknow detector enum";
      break;
    }
  } // switch( det )

  LOG( debug ) << "Set Trigger min limit for " << sDet << " to " << uVal;
}
void CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerWindow( ECbmModuleId det, Double_t dWinBeg, Double_t dWinEnd )
{
  /// Check if valid time window: end strictly after beginning
  if( dWinEnd <= dWinBeg )
    LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerWindow => Invalid time window: [ "
                 << dWinBeg << ", " << dWinEnd << " ]";

  std::string sDet = "";
  /// Store in corresponding members
  switch( det )
  {
    case ECbmModuleId::kSts:
    {
      fdStsTimeWinBeg  = dWinBeg;
      fdStsTimeWinEnd  = dWinEnd;
      sDet = "STS";
      break;
    }
    case ECbmModuleId::kRich:
    {
      fdRichTimeWinBeg = dWinBeg;
      fdRichTimeWinEnd = dWinEnd;
      sDet = "RICH";
      break;
    }
    case ECbmModuleId::kMuch:
    {
      fdMuchTimeWinBeg = dWinBeg;
      fdMuchTimeWinEnd = dWinEnd;
      sDet = "MUCH";
      break;
    }
    case ECbmModuleId::kTrd:
    {
      fdTrdTimeWinBeg  = dWinBeg;
      fdTrdTimeWinEnd  = dWinEnd;
      sDet = "TRD";
      break;
    }
    case ECbmModuleId::kTof:
    {
      fdTofTimeWinBeg  = dWinBeg;
      fdTofTimeWinEnd  = dWinEnd;
      sDet = "TOF";
      break;
    }
    case ECbmModuleId::kPsd:
    {
      fdPsdTimeWinBeg  = dWinBeg;
      fdPsdTimeWinEnd  = dWinEnd;
      sDet = "PSD";
      break;
    }
    case ECbmModuleId::kT0:
    {
      fdT0TimeWinBeg   = dWinBeg;
      fdT0TimeWinEnd   = dWinEnd;
      sDet = "T0";
      break;
    }
    default:
    {
      LOG( fatal ) << "CbmMcbm2019TimeWinEventBuilderAlgo::SetTriggerWindow => "
                   << "Unsupported or unknow detector enum";
      break;
    }
  } // switch( det )

  LOG( debug ) << "Set Trigger window for " << sDet
               << " to [ " << dWinBeg << "; " << dWinEnd << " ]";

  /// Update the variables storing the earliest and latest time window boundaries
  UpdateTimeWinBoundariesExtrema();
  /// Update the variable storing the size if widest time window for overlap detection
  UpdateWidestTimeWinRange();
}
void CbmMcbm2019TimeWinEventBuilderAlgo::UpdateTimeWinBoundariesExtrema()
{
  fdEarliestTimeWinBeg = std::min( fdStsTimeWinBeg,  std::min(
                                   fdMuchTimeWinBeg, std::min(
                                   fdTrdTimeWinBeg,  std::min(
                                   fdTofTimeWinBeg,  std::min(
                                   fdRichTimeWinBeg, std::min(
                                   fdPsdTimeWinBeg,
                                   fdT0TimeWinBeg    ) ) ) ) ) );
  fdLatestTimeWinEnd = std::max( fdStsTimeWinEnd,  std::max(
                                   fdMuchTimeWinEnd, std::max(
                                   fdTrdTimeWinEnd,  std::max(
                                   fdTofTimeWinEnd,  std::max(
                                   fdRichTimeWinEnd, std::max(
                                   fdPsdTimeWinEnd,
                                   fdT0TimeWinEnd    ) ) ) ) ) );
}
void CbmMcbm2019TimeWinEventBuilderAlgo::UpdateWidestTimeWinRange()
{
  Double_t fdStsTimeWinRange  = fdStsTimeWinEnd  - fdStsTimeWinBeg;
  Double_t fdMuchTimeWinRange = fdMuchTimeWinEnd - fdMuchTimeWinBeg;
  Double_t fdTrdTimeWinRange  = fdTrdTimeWinEnd  - fdTrdTimeWinBeg;
  Double_t fdTofTimeWinRange  = fdTofTimeWinEnd  - fdTofTimeWinBeg;
  Double_t fdRichTimeWinRange = fdRichTimeWinEnd - fdRichTimeWinBeg;
  Double_t fdPsdTimeWinRange  = fdPsdTimeWinEnd  - fdPsdTimeWinBeg;
  Double_t fdT0TimeWinRange   = fdT0TimeWinEnd   - fdT0TimeWinBeg;

  fdWidestTimeWinRange = std::max( fdStsTimeWinRange,  std::max(
                                   fdMuchTimeWinRange, std::max(
                                   fdTrdTimeWinRange,  std::max(
                                   fdTofTimeWinRange,  std::max(
                                   fdRichTimeWinRange, std::max(
                                   fdPsdTimeWinRange,
                                   fdT0TimeWinRange    ) ) ) ) ) );
}
//----------------------------------------------------------------------

ClassImp(CbmMcbm2019TimeWinEventBuilderAlgo)
