// ------------------------------------------------------------------
// -----                    TTriglogUnpackTof                   -----
// -----              Created 07/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TTriglogUnpackTof.h"

// SubEvent ProcId from ROC library
#include "commons.h"

// General MBS headers
#include "TofDef.h"
#include "TMbsUnpackTofPar.h"

// ToF specific headers
#include "TTofTriglogUnpacker.h"
#include "TTofTriglogBoard.h"
#include "TTofScalerBoard.h"

// ROOT headers
#include "TClonesArray.h"
#include "TFile.h"
#include "TDatime.h"
#include "TROOT.h"
#include "TDirectory.h"

// FAIR headers
#include "FairLogger.h"
//#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

TTriglogUnpackTof::TTriglogUnpackTof() : 
   FairUnpack( 10, 1, roc3587::proc_Triglog, 0, 9 ),
   fiVerbosity(0),
   fMbsUnpackPar(0),
   fiNbEvents(0),
   fiFirstEventNumber(0),
   fiLastEventNumber(0),
   fLastCheck(),
   fCheck(),
   fTriglogUnp(NULL),
   fTriglogBoardCollection(NULL),
   fScalerBoardCollection(NULL),
   fbSaveTriglogBoard(kFALSE),
   fbSaveScalerBoards(kFALSE)
{
}

TTriglogUnpackTof::TTriglogUnpackTof( Int_t verbose ) : 
   FairUnpack( 10, 1, roc3587::proc_Triglog, 0, 9 ),
   fiVerbosity(verbose),
   fMbsUnpackPar(0),
   fiNbEvents(0),
   fiFirstEventNumber(0),
   fiLastEventNumber(0),
   fLastCheck(),
   fCheck(),
   fTriglogUnp(NULL),
   fTriglogBoardCollection(NULL),
   fScalerBoardCollection(NULL),
   fbSaveTriglogBoard(kFALSE),
   fbSaveScalerBoards(kFALSE)
{
}

TTriglogUnpackTof::TTriglogUnpackTof( Int_t type, Int_t subType, Short_t procId, Int_t verbose,
                                         Short_t subCrate, Short_t control) :
   FairUnpack( type, subType, procId, subCrate, control  ),
   fiVerbosity(verbose),
   fMbsUnpackPar(0),
   fiNbEvents(0),
   fiFirstEventNumber(0),
   fiLastEventNumber(0),
   fLastCheck(),
   fCheck(),
   fTriglogUnp(NULL),
   fTriglogBoardCollection(NULL),
   fScalerBoardCollection(NULL),
   fbSaveTriglogBoard(kFALSE),
   fbSaveScalerBoards(kFALSE)
{
}
      
TTriglogUnpackTof::~TTriglogUnpackTof()
{      
   LOG(info)<<"**** TTriglogUnpackTof: Delete instance ";
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      delete fTriglogUnp;
}

// --------------------------------------------------
// FairUnpack specific functions
Bool_t TTriglogUnpackTof::Init()
{
   LOG(info)<<"**** TTriglogUnpackTof: Init Params ";
   if( kFALSE == InitParameters() )
      LOG(error)<<"**** TTriglogUnpackTof: Failed to Initialize params";
//      return kFALSE;
   LOG(info)<<"**** TTriglogUnpackTof: Params Initialized ";
   
   // Create the proper Unpackers and number of output objects for each boards type
//   if( kFALSE == RegisterOutput() )
//      return kFALSE;
   Register(); // FairUnpack Style, I feel unsafe as no check on output Array validity!
   if( kFALSE == CreateUnpackers() )
      LOG(error)<<"**** TTriglogUnpackTof: Failed to create unpackers ";
//      return kFALSE;
   
   return kTRUE;
}
Bool_t TTriglogUnpackTof::DoUnpack(Int_t* data, Int_t size)
{   
   Int_t  * pData   = data;
//   UInt_t  uNbWords = size/2 - 1; // <= Somehow FAIRROOT transfers the size in words from the subevt header and not the size in longwords reads by the f_evt_get_subevent function
// Changed in git commit 0c0bd037c201d3496f9d5d7c133874382e885677 to fairroot LMD source
// TODO: Make sure the same change is applied to FairMbsStreamSource !!!!
   UInt_t  uNbWords = size;

   
   LOG(debug)<<"TTriglogUnpackTof::DoUnpack => Found TRIGLOG subevent! LEN"
            <<uNbWords;
            
   if( 3 < fiVerbosity )
   {
      TString sPrintEvent = "";
      LOG(debug)<<"----------------------------------------------------------------";
      LOG(debug)<<"Size: "<<uNbWords;
      for( UInt_t uWdInd = 0; uWdInd < uNbWords; uWdInd++ )
      {
         if( 0 < uWdInd && 0 == uWdInd%8 )
         {
            LOG(debug)<<sPrintEvent;
            sPrintEvent = "";
         } // if( 0 < uWdInd && 0 == uWdInd%8 )
         sPrintEvent += Form("%08x ", pData[uWdInd]);
      } // for( UInt_t uWdInd = 0; uWdInd < uNbWords; uWdInd++ )
      LOG(debug)<<sPrintEvent;
      LOG(debug)<<"----------------------------------------------------------------";
   } // if( 3 < fiVerbosity )

   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->ProcessTriglog( (UInt_t*)data , uNbWords ); 
   
   fiNbEvents++;
   LOG(debug)<<"TRIGLOG unpacked!";
   return kTRUE;
}
void TTriglogUnpackTof::Reset()
{
   ClearOutput();
}
void TTriglogUnpackTof::Register()
{
   if( kFALSE == RegisterOutput() )
      LOG(error)<<"**** TTriglogUnpackTof: Failed to create outputs ";
}
Bool_t TTriglogUnpackTof::Finish()
{   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->FinishTriglog();
      else LOG(info)<<"No Triglog summary as 0 Triglog board active";
      
   return kTRUE;
}

// ------------------------------------------------------------------
Bool_t TTriglogUnpackTof::InitParameters()
{
   // Get Base Container
   FairRun* ana = FairRun::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   // Unpacker parameter
   fMbsUnpackPar = (TMbsUnpackTofPar*) (rtdb->getContainer("TMbsUnpackTofPar"));
   if( 0 == fMbsUnpackPar )
      return kFALSE;

   // PAL: Added to recover loading of parameters before initialization
//   rtdb->initContainers(  ana->GetRunId() );

   fMbsUnpackPar->printParams();
     
   return kTRUE;
} 
// ------------------------------------------------------------------
Bool_t TTriglogUnpackTof::CreateHistogramms()
{
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager!
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->CreateHistos();
   
   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
   
   return kTRUE;
}
Bool_t TTriglogUnpackTof::FillHistograms()
{
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->FillHistos();
   
   return kTRUE;
}
void TTriglogUnpackTof::WriteHistogramms()
{
   // There should always be only 1 active TRIGLOG board at a time
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
   {
      TDirectory * oldir = gDirectory;
      TFile *fHist = new TFile("./tofMbsUnp.hst.root","RECREATE");
      
      if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
         fTriglogUnp->WriteHistos( fHist );
         
      gDirectory->cd( oldir->GetPath() );
      fHist->Close();
   } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
}
void TTriglogUnpackTof::DeleteHistograms()
{
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->DeleteHistos();
}
// ------------------------------------------------------------------
Bool_t TTriglogUnpackTof::CreateUnpackers()
{
   if( 0 == fMbsUnpackPar->GetNbActiveBoards() )
   {
      LOG(error)<<"TTriglogUnpackTof::CreateUnpackers => No active boards => No unpacker !?!?! ";
      return kFALSE;
   }
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp = new TTofTriglogUnpacker( fMbsUnpackPar );
      
   return kTRUE;
}
Bool_t TTriglogUnpackTof::RegisterOutput()
{
   if( 0 == fMbsUnpackPar->GetNbActiveBoards() )
   {
      LOG(error)<<"TTriglogUnpackTof::RegisterOutput => No active boards => No output objects !?!?! ";
      return kFALSE;
   }
   // Not sure here what is the best way to group the TDC objects......
   // The vector of data in the board objects kill probably the TClonesArray purpose?
   // Maybe better to have big fixed size array and TTofVftxData::Clear calls in TTofVftxBoard combined
   // with ConstructedAt access in TTofVftxUnpacker
   FairRootManager *fManager = FairRootManager::Instance();
      // Scalers
         // TRIGLOG 
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
   {
      // There should always be maximum one trigger board active at a time!!!
      fTriglogBoardCollection = new TClonesArray( "TTofTriglogBoard", 1 );
//      fManager->Register("TofTriglog","TofUnpack",fTriglogBoardCollection, kTRUE);
      fManager->Register( "TofTriglog","TofUnpack",fTriglogBoardCollection,
                          fMbsUnpackPar->WriteDataInCbmOut() || fbSaveTriglogBoard);
      
      fScalerBoardCollection = new TClonesArray( "TTofScalerBoard", 
                                                  fMbsUnpackPar->GetNbActiveScalersB());
//      fManager->Register("TofRawScalers","TofUnpack",fScalerBoardCollection, kTRUE);
      fManager->Register( "TofRawScalers","TofUnpack",fScalerBoardCollection,
                          fMbsUnpackPar->WriteDataInCbmOut() || fbSaveScalerBoards);
   } // if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      else if( 1 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      {
         LOG(error)<<"TMbsUnpackTofCustom::RegisterOutput => More than 1 TRIGLOG board active!!!!! recheck your VME address matrix ";
         return kFALSE;
      } // else if( 1 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )

   return kTRUE;
}
Bool_t TTriglogUnpackTof::ClearOutput()
{
   LOG(debug)<<"Clear TRIGLOG";
   if( 1 == fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogBoardCollection->Clear("C");
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
      fScalerBoardCollection->Clear("C");
   return kTRUE;
}
void TTriglogUnpackTof::SetSaveTriglog( Bool_t bSaveTrlo )
{
   fbSaveTriglogBoard = bSaveTrlo;
   LOG(info)<<"TTriglogUnpackTof => Enable the saving of raw triglog data in analysis output file";

}
void TTriglogUnpackTof::SetSaveScalers( Bool_t bSaveScal )
{
   fbSaveScalerBoards = bSaveScal;
   LOG(info)<<"TTriglogUnpackTof => Enable the saving of raw scaler data in analysis output file";

}
// ------------------------------------------------------------------
