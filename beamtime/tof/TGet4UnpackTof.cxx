// ------------------------------------------------------------------
// -----                    TGet4UnpackTof                      -----
// -----              Created 24/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TGet4UnpackTof.h"

// General MBS headers
#include "TofDef.h"
#include "TMbsUnpackTofPar.h"

// ToF specific headers
#include "TTofGet4Unpacker.h"
#include "TTofGet4Board.h"
#include "TTofGet4Data.h"

// ROOT headers
#include "TClonesArray.h"
#include "TFile.h"
#include "TROOT.h"
#include "TDirectory.h"

// FAIR headers
#include "FairLogger.h"
//#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

TGet4UnpackTof::TGet4UnpackTof() :
   FairUnpack( 10, 1, roc3587::proc_RocEvent, 0, roc::formatOptic2 ),
   fiVerbosity(0),
   fMbsUnpackPar(0),
   fiNbEvents(0),
   fiFirstEventNumber(0),
   fiLastEventNumber(0),
   fGet4Unp(NULL),
   fGet4BoardCollection(NULL)
{
}

TGet4UnpackTof::TGet4UnpackTof( Int_t verbose ) :
   FairUnpack( 10, 1, roc3587::proc_RocEvent, 0, roc::formatOptic2 ),
   fiVerbosity(verbose),
   fMbsUnpackPar(0),
   fiNbEvents(0),
   fiFirstEventNumber(0),
   fiLastEventNumber(0),
   fGet4Unp(NULL),
   fGet4BoardCollection(NULL)
{
}

TGet4UnpackTof::TGet4UnpackTof( Int_t type, Int_t subType, Short_t sRocId, Int_t verbose,
                                Short_t procId, Short_t control ) :
   FairUnpack( type, subType, procId, sRocId, control ),
   fiVerbosity(verbose),
   fMbsUnpackPar(0),
   fiNbEvents(0),
   fiFirstEventNumber(0),
   fiLastEventNumber(0),
   fGet4Unp(NULL),
   fGet4BoardCollection(NULL)
{
}

TGet4UnpackTof::~TGet4UnpackTof()
{
   LOG(info)<<"**** TGet4UnpackTof: Delete instance ";

   if( 0 <  fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
      delete fGet4Unp;
}

// --------------------------------------------------
// FairUnpack specific functions
Bool_t TGet4UnpackTof::Init()
{
   LOG(info)<<"**** TGet4UnpackTof: Init Params ";
   if( kFALSE == InitParameters() )
      return kFALSE;
   LOG(info)<<"**** TGet4UnpackTof: Params Initialized ";

   // Create the proper Unpackers and number of output objects for each boards type
//   if( kFALSE == RegisterOutput() )
//      return kFALSE;
   Register(); // FairUnpack Style, I feel unsafe as no check on output Array validity!
   if( kFALSE == CreateUnpackers() )
      return kFALSE;

   return kTRUE;
}
Bool_t TGet4UnpackTof::DoUnpack(Int_t* data, Int_t size)
{
/*   Int_t  * pData   = data;*/

   LOG(debug)<<"TGet4UnpackTof::ProcessSubevent => Found ROC subevent! LEN"
            <<size;

   if( 0 <  fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
      fGet4Unp->ProcessGet4( (UInt_t*)data , size );

   FillHistograms();

   fiNbEvents++;
   LOG(debug)<<"GET4 unpacked!";
   
   return kTRUE;
}
void TGet4UnpackTof::Reset()
{
   ClearOutput();
}
void TGet4UnpackTof::Register()
{
   RegisterOutput();
}
Bool_t TGet4UnpackTof::Finish()
{
   WriteHistogramms();
   return kTRUE;
}

// ------------------------------------------------------------------
Bool_t TGet4UnpackTof::InitParameters()
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
Bool_t TGet4UnpackTof::CreateHistogramms()
{
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager!

   if( 0 <  fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
      fGet4Unp->CreateHistos();

   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!

   return kTRUE;
}
Bool_t TGet4UnpackTof::FillHistograms()
{
   if( 0 <  fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
      fGet4Unp->FillHistos();

   return kTRUE;
}
void TGet4UnpackTof::WriteHistogramms()
{
   if( 0 <  fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
   {
      TDirectory * oldir = gDirectory;
      TFile *fHist = new TFile("./tofRocUnp.hst.root","RECREATE");

      fGet4Unp->WriteHistos( fHist );

      gDirectory->cd( oldir->GetPath() );
      fHist->Close();
   } // if( 0 <  fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
}
void TGet4UnpackTof::DeleteHistograms()
{
   if( 0 <  fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
      fGet4Unp->DeleteHistos();
}
// ------------------------------------------------------------------
Bool_t TGet4UnpackTof::CreateUnpackers()
{
   if( 0 == fMbsUnpackPar->GetNbActiveBoards() )
   {
      LOG(error)<<"TGet4UnpackTof::CreateUnpackers => No active boards => No unpacker !?!?! ";
      return kFALSE;
   }

   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
   {
      fGet4Unp = new TTofGet4Unpacker( fMbsUnpackPar );
      fGet4Unp->Init();
   }

   return kTRUE;
}
Bool_t TGet4UnpackTof::RegisterOutput()
{
   if( 0 == fMbsUnpackPar->GetNbActiveBoards() )
   {
      LOG(error)<<"TGet4UnpackTof::RegisterOutput => No active boards => No output objects !?!?! ";
      return kFALSE;
   }
   // Not sure here what is the best way to group the TDC objects......
   // The vector of data in the board objects kill probably the TClonesArray purpose?
   // Maybe better to have big fixed size array and TTofVftxData::Clear calls in TTofVftxBoard combined
   // with ConstructedAt access in TTofVftxUnpacker
   FairRootManager *fManager = FairRootManager::Instance();
      // TDC
         // GET4
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
   {
      fGet4BoardCollection = new TClonesArray( "TTofGet4Board",
                                                fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ));
//      fManager->Register("TofGet4Tdc","TofUnpack",fGet4BoardCollection, kTRUE);
      fManager->Register( "TofGet4Tdc","TofUnpack",fGet4BoardCollection,
                          fMbsUnpackPar->WriteDataInCbmOut());
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )

   return kTRUE;
}
Bool_t TGet4UnpackTof::ClearOutput()
{
   LOG(debug)<<"Clear GET4";
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
      fGet4BoardCollection->Clear("C");
   return kTRUE;
}
// ------------------------------------------------------------------
