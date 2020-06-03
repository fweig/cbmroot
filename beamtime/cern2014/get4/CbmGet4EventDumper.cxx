// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmGet4EventDumper                          -----
// -----                    Created 16.04.2015 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmGet4EventDumper.h"

// TOF Classes and includes
#include "CbmGet4EventBuffer.h"
#include "CbmGet4v1xDef.h"
#include "CbmGet4v1xHackDef.h"
#include "TTofGet4Board.h"
#include "TTofGet4Data.h"

// CBMroot classes and includes

// FAIR classes and includes
//#include "FairRunOnline.h"
//#include "FairEventHeader.h"
#include "FairLogger.h"

// ROOT Classes and includes
#include "TH1.h"
#include "TROOT.h"
#include "TClonesArray.h"

// C++ std headers

//___________________________________________________________________
//

// ------------------------------------------------------------------
CbmGet4EventDumper::CbmGet4EventDumper()
  : FairTask("CbmGet4EventDumper"),
    fuInputEvents(0),
    fuOutputEvents(0),
    fInputBuffer(NULL),
    fEvtHeader(NULL),
    fIOMan(NULL),
    fGet4BoardCollection(NULL),
    fuNbGet4(0),
    fvbActiveChips(),
    fuNbEvtBuffEmptyCall(0),
    fbWriteDataInCbmOut(kFALSE),
    fdMaxTimePerLoopS(0.0),
    fStart(),
    fStop()
{
  LOG(info)<<" CbmGet4EventDumper: Task started";
}
CbmGet4EventDumper::CbmGet4EventDumper(const char* name, Int_t verbose)
  : FairTask(name, verbose),
    fuInputEvents(0),
    fuOutputEvents(0),
    fInputBuffer(NULL),
    fEvtHeader(NULL),
    fIOMan(NULL),
    fGet4BoardCollection(NULL),
    fuNbGet4(0),
    fvbActiveChips(),
    fuNbEvtBuffEmptyCall(0),
    fbWriteDataInCbmOut(kFALSE),
    fdMaxTimePerLoopS(0.0),
    fStart(),
    fStop()
{
   LOG(info)<<" CbmGet4EventDumper: Task started";
}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
CbmGet4EventDumper::~CbmGet4EventDumper()
{
}
// ------------------------------------------------------------------
/************************************************************************************/
void CbmGet4EventDumper::SetActiveGet4( UInt_t uChipsIndex, Bool_t bActiveFlag)
{
   if( 0 == fvbActiveChips.size() && 0 < fuNbGet4 )
   {
      fvbActiveChips.resize(fuNbGet4);
      for( UInt_t uChip = 0; uChip < fuNbGet4; uChip++)
         fvbActiveChips[uChip] = kTRUE;
   } // if( 0 == fvbActiveChips.size()

   if( uChipsIndex < fuNbGet4 )
      fvbActiveChips[uChipsIndex] = bActiveFlag;
      else LOG(error)<<" CbmGet4EventDumper::SetActiveGet4 => Invalid chip index "
                     << uChipsIndex ;
}
/************************************************************************************/
// FairTasks inherited functions
InitStatus CbmGet4EventDumper::Init()
{
   if( kFALSE == RegisterInputs() )
      return kFATAL;

   if( kFALSE == RegisterOutputs() )
      return kFATAL;

   if( kFALSE == CreateHistos() )
      return kFATAL;

   // Common variables initial values, mostly in case some are missed in ctor
   fuInputEvents   = 0;
   fuOutputEvents  = 0;

   return kSUCCESS;
}

void CbmGet4EventDumper::Exec(Option_t* /*option*/)
{
   // Task execution
   LOG(debug)<<" CbmGet4EventDumper => New loop";

   // Initialize the looping time start point
   fStart.Set();

   Int_t iNbHitsDumped = 0;

   // We try to always dump at least 1 event per execution loop
   // as the FairRunOnline is always inserting a new "event" per loop
   // to the output tree (see l278 to 281)
   if( 0 < fInputBuffer->GetSize()  )
   {
      // From FairRunOnline EventLoop method
      fIOMan->StoreWriteoutBufferData(fIOMan->GetEventTime());

      iNbHitsDumped = this->ReadEvent();
      fuOutputEvents ++;

      // Fill histos if needed
      FillHistos();

      // From FairRunOnline EventLoop method
      fIOMan->Fill();
      fIOMan->DeleteOldWriteoutBufferData();
   } // if( 0 < fInputBuffer->GetSize()  )
   /*
      else if( kTRUE == fbWriteDataInCbmOut)
         LOG(info)<<" CbmGet4EventDumper => No event to dump in this execution loop\n"
                  <<"                    => Empty event in output tree!"
                  ;
   */

   // Now we dump events from as many available events as possible
   // while keeping enough events in buffer to account for some
   // execution loops without events
   while( fuNbEvtBuffEmptyCall < fInputBuffer->GetSize()  )
   {
      iNbHitsDumped += this->ReadEventMore();
      fuOutputEvents ++;

      // Fill histos if needed
      FillHistos();

      if( 0 == ( fuOutputEvents%100000 ) && 0 < fuOutputEvents )
      {
         LOG(info)<< "CbmGet4EventDumper::Exec : "
                   << fuOutputEvents << " events dumped.";
      } // if( 0 == ( fuOutputEvents%100 ) && 0 < fuOutputEvents )

      // if total looping time larger than limit: stop looping for now
      // We can start on the backlog later
      fStop.Set();
      Double_t dLooptTime = fStop.GetSec() - fStart.GetSec()
                          + (fStop.GetNanoSec() - fStart.GetNanoSec())/1e9;
      if( fdMaxTimePerLoopS < dLooptTime )
         break;
   } // while( fuNbEvtBuffEmptyCall < fInputBuffer->GetSize()  )

   // Update execution loop counter
   fuInputEvents++;
}

void CbmGet4EventDumper::Finish()
{
   LOG(info)<<"CbmGet4EventDumper::Finish up with "
            << fuInputEvents << " loops done and "
            << fuOutputEvents << " events dumped" ;

   WriteHistos();

   DeleteHistos();
}

/************************************************************************************/
Bool_t   CbmGet4EventDumper::RegisterInputs()
{
   fInputBuffer = CbmGet4EventBuffer::Instance();

   if(NULL == fInputBuffer)
      return kFALSE;

   return kTRUE;
}
Bool_t   CbmGet4EventDumper::RegisterOutputs()
{
   // Recover the pointer on the FairRoot IO manager
   fIOMan     = FairRootManager::Instance();

   // Now recover the pointer on the event header
   fEvtHeader = dynamic_cast<FairEventHeader*> (fIOMan->GetObject("EventHeader."));
   if( NULL == fEvtHeader )
   {
      LOG(warning)<<"CbmGet4EventDumper::RegisterOutputs: No event Header was found!!!";
      return kFALSE;
   }


   if( 0 < fuNbGet4 )
   {
      fGet4BoardCollection = new TClonesArray( "TTofGet4Board", fuNbGet4);
      fIOMan->Register( "TofGet4Tdc","TofUnpack",fGet4BoardCollection, fbWriteDataInCbmOut);
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )

   if(NULL == fGet4BoardCollection)
   {
      LOG(error)<<"CbmGet4EventDumper::RegisterOutputs: no TOF GET4 TDC array created! ";
      fuNbGet4 = 0;
      return kFALSE;
   } // if(NULL == fGet4BoardCollection)

   return kTRUE;
}

Bool_t   CbmGet4EventDumper::ClearOutput()
{
   LOG(debug)<<"Clear GET4";
   if( 0 < fuNbGet4 && NULL != fGet4BoardCollection )
      fGet4BoardCollection->Clear("C");
   return kTRUE;
}
/************************************************************************************/
Int_t    CbmGet4EventDumper::ReadEvent()
{
   Int_t iNbHitsDumped = 0;

   // Clear storage
   ClearOutput();

   // First obtain a pointer on an input event
   std::pair< CbmGet4EventHeader, std::vector< get4v1x::FullMessage >* > pvEvent = fInputBuffer->GetNextEvent();
/*
 * Missing:
 * 1) Trigger time in Event buffer => Done
 * 2) Trigger type and trigger epoch in output event => needed?
 *
 */
   // Initialize all board objects
   std::vector<TTofGet4Board *> vGet4Board;
   vGet4Board.resize( fuNbGet4, NULL );
   for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++ )
   {
      vGet4Board[uGet4] = dynamic_cast<TTofGet4Board*> ( fGet4BoardCollection->ConstructedAt(uGet4) );
      vGet4Board[uGet4]->SetTriggerFullTime(
                  ( ( (pvEvent.first).fuTriggerExtEp ) & 0xFFFFFFFF ) *get4v1x::kdEpochInPs +
                  (pvEvent.first).fuTriggerTs    *get4v1x::kdClockCycleSize ); // in ps
      vGet4Board[uGet4]->SetTriggerTime( (pvEvent.first).fuTriggerTs );   // in clock cycles
         // This class uses unsigned integer => remapped trigger index
         //          <  -256: unknown trigger           => 512
         //     -256 to   -1: multiplicity triggers     => 256 to 511
         //     0    <      : usual coincidence trigger => 0 to 255, hope nobody implement more than that
      vGet4Board[uGet4]->SetTriggIndx( 0 <= (pvEvent.first).fiTriggerType?
                                 static_cast<UInt_t>( (pvEvent.first).fiTriggerType ) :
                                 -256 < (pvEvent.first).fiTriggerType?
                                       static_cast<UInt_t>( 512 + (pvEvent.first).fiTriggerType ) : 512 );
   } // for( UInt_t uGet4 = 0; uGet4 < fuNbGet4; uGet4++ )


   // Do stuff with our hits
   for( UInt_t uHit = 0; uHit < (pvEvent.second)->size(); uHit++ )
   {
      get4v1x::FullMessage mess = (pvEvent.second)->at(uHit);

      uint8_t  cRocId    = mess.getRocNumber();
      uint8_t  cChipId   = mess.getGet4V10R32ChipId();
      uint32_t uChipFullId = cChipId + get4v1x::kuMaxGet4PerRoc*cRocId;

      TTofGet4Data data(
            mess.getGet4V10R32HitChan(),
            mess.getGet4V10R32HitFt(),
            mess.getGet4V10R32HitTs(),
            mess.getMsgFullTime( mess.getExtendedEpoch() & 0xFFFFFFFF ),
            mess.getMsgFullTimeD( mess.getExtendedEpoch() & 0xFFFFFFFF ),
            ( mess.getExtendedEpoch() & 0xFFFFFFFF ),
            ( ( mess.getExtendedEpoch() >> 32 ) & 0xFFFFFFFF ),
            mess.getGet4V10R32HitTot(),
            kTRUE );
      vGet4Board[ uChipFullId ]->AddData( data );
   } // for( Int_t iHit = 0; iHit < (pvEvent.second)->size(); iHit++ )
   vGet4Board.clear();
/*
   TTofGet4Board * Get4Board = (TTofGet4Board*) fGet4BoardCollection->ConstructedAt(uGet4);
   // Get4Board->SetTriggerFullTime (not needed?)
   // Get4Board->SetTriggerTime
   // Get4Board->SetTriggIndx
   // Get4Board->SetValid
   // Get4Board->AddData

   // TTofGet4Data( UInt_t uChan, UInt_t  uFt,
   //      UInt_t uCoarseT,
   //      ULong64_t uFulltime = 0,  Double_t dFullTime = 0.0,
   //      UInt_t uGet4Epoch = 0, UInt_t uGet4EpCycle = 0,
   //      UInt_t uTot = 0, Bool_t bIs32Bit = kFALSE );
*/
   // Now clear the input event object and delete it
   if( 0 < (pvEvent.second)->size() )
      (pvEvent.second)->clear();
   delete (pvEvent.second);

   return iNbHitsDumped;
}
// ------------------------------------------------------------------
Int_t    CbmGet4EventDumper::ReadEventMore()
{
   Int_t iNbHitsDumped = 0;

   // From FairRunOnline EventLoop method
   fIOMan->StoreWriteoutBufferData(fIOMan->GetEventTime());

   iNbHitsDumped = this->ReadEvent();

   // From FairRunOnline EventLoop method
   fIOMan->Fill();
   fIOMan->DeleteOldWriteoutBufferData();

   return iNbHitsDumped;
}
/************************************************************************************/
// ------------------------------------------------------------------
Bool_t CbmGet4EventDumper::CreateHistos()
{
  // Create histogramms

   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !

   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!

   return kTRUE;
}

// ------------------------------------------------------------------
Bool_t CbmGet4EventDumper::FillHistos()
{
   return kTRUE;
}
// ------------------------------------------------------------------

Bool_t CbmGet4EventDumper::WriteHistos()
{
/*
   // Write histogramms to the file
   TDirectory * oldir = gDirectory;
   TFile *fHist = new TFile("./get4EventDumper.hst.root","RECREATE");
   fHist->cd();

   fhMaxMulPerEpochDist->Write();
   fHist->Close();
*/
   return kTRUE;
}
Bool_t   CbmGet4EventDumper::DeleteHistos()
{
   return kTRUE;
}

ClassImp(CbmGet4EventDumper)
