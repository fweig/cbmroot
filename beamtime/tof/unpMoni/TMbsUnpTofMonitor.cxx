// ------------------------------------------------------------------
// -----                     TMbsUnpTofMonitor                  -----
// -----              Created 16/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------  

#include "TMbsUnpTofMonitor.h"

// General MBS headers
#include "TofDef.h"
#include "TMbsUnpackTofPar.h"
#include "TMbsCalibTofPar.h"

// ToF specific headers
#include "TTofTriglogUnpacker.h"
#include "TTofScomUnpacker.h"
#include "TTofScal2014Unpacker.h"
#include "TTofTriglogScalUnpacker.h"
#include "TTofVftxUnpacker.h"
#include "TTofTrbTdcUnpacker.h"

// ROOT headers
#include "TClonesArray.h"
#include "TFile.h"
//#include "TTimeStamp.h"

// FAIR headers
#include "FairLogger.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"

TMbsUnpTofMonitor::TMbsUnpTofMonitor() : 
   FairTask("MbsUnpackTofCustom"),
   fMbsUnpackPar(NULL),
   fMbsCalibPar(NULL),
   fTriglogUnp(NULL),
   fScomUnp(NULL),
   fScal2014Unp(NULL),
   fTrloScalUnp(NULL),
   fVftxUnp(NULL),
   fTrbTdcUnp(NULL),
   fTriglogBoardCollection(NULL),
   fScalerBoardCollection(NULL),
   fVftxBoardCollection(NULL),
   fTrbTdcBoardCollection(NULL),
   fiNbEvents(0),
   fiFirstEventNumber(0),
   fiLastEventNumber(0),
   fFirstCheck(),
   fLastCheck(),
   fCheck(),
   fdScalersEvoRangeUser(-1),
   fdScalersEvoBinSzUser(-1)
{
}

TMbsUnpTofMonitor::TMbsUnpTofMonitor(const char* name, Int_t verbose) :
   FairTask(name, verbose),
   fMbsUnpackPar(0),
   fMbsCalibPar(0),
   fTriglogUnp(NULL),
   fScomUnp(NULL),
   fScal2014Unp(NULL),
   fTrloScalUnp(NULL),
   fVftxUnp(NULL),
   fTrbTdcUnp(NULL),
   fTriglogBoardCollection(NULL),
   fScalerBoardCollection(NULL),
   fVftxBoardCollection(NULL),
   fTrbTdcBoardCollection(NULL),
   fiNbEvents(0),
   fiFirstEventNumber(0),
   fiLastEventNumber(0),
   fFirstCheck(),
   fLastCheck(),
   fCheck(),
   fdScalersEvoRangeUser(-1),
   fdScalersEvoBinSzUser(-1)
{
}
      
TMbsUnpTofMonitor::~TMbsUnpTofMonitor()
{      
   LOG(info)<<"**** TMbsUnpTofMonitor: Delete instance ";
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      delete fTriglogUnp;
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scalormu ) )
      delete fScomUnp;
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scaler2014 ) )
      delete fScal2014Unp;
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglogscal ) )
      delete fTrloScalUnp;
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::vftx ) )
      delete fVftxUnp;
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::trbtdc ) )
      delete fTrbTdcUnp;
}

// --------------------------------------------------
// Fairtask specific functions
void TMbsUnpTofMonitor::SetParContainers()
{
   InitParameters();
}
InitStatus TMbsUnpTofMonitor::ReInit()
{
   LOG(info)<<"**** TMbsUnpTofMonitor: Reinitialize the unpack parameters for tof ";
   if( kFALSE == InitParameters() )
      return kFATAL;  
      
   return kSUCCESS;
}

InitStatus TMbsUnpTofMonitor::Init()
{
   if( kFALSE == InitParameters() )
      return kFATAL;  
   
   // Create the proper Unpackers and number of output objects for each boards type
   if( kFALSE == CreateUnpackers() )
      return kFATAL;
   if( kFALSE == CreateHistogramms() )
      return kFATAL;
   
   fLastCheck.Set();
   fFirstCheck.Set();
   
   return kSUCCESS;
}
void TMbsUnpTofMonitor::Exec(Option_t* /*option*/)
{
   // No output => no clear needed
   
   if( 0 < fiNbEvents && 0 == fiNbEvents%10000 )
   {
      fCheck.Set();
      TString sFormat = Form("Event: %8d is MBS event %8d dt %6.3fs Since start %5lds",
//                              fiNbEvents, fiLastEventNumber, fCheck.Get() - fLastCheck.Get(),
//                              fCheck.Get() - fFirstCheck.Get());
                              fiNbEvents, fiLastEventNumber,
                              (Double_t)(fCheck.GetSec() - fLastCheck.GetSec())
                               + ( (Double_t)fCheck.GetNanoSec() - (Double_t)fLastCheck.GetNanoSec())/1e9,
                              fCheck.GetSec() - fFirstCheck.GetSec());
      LOG(info)<<sFormat;
      fLastCheck.Set();
   }
   
   FillHistograms();
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->UpdateStats();
      
   fiNbEvents++;
}
void TMbsUnpTofMonitor::Finish()
{
   fCheck.Set();
   LOG(info)<<"Last Event: "<<fiNbEvents<<" with MBS event nb "<<fiLastEventNumber
            << Form(" After %9.3fs",  (Double_t)(fCheck.GetSec() - fFirstCheck.GetSec())
                                    + ( (Double_t)fCheck.GetNanoSec() - (Double_t)fFirstCheck.GetNanoSec())/1e9);
   WriteHistogramms();
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->FinishTriglog();
}
// ------------------------------------------------------------------
Bool_t TMbsUnpTofMonitor::InitParameters()
{
   // Get Base Container
   FairRun* ana = FairRun::Instance();
   if( 0 == ana )
      return kFALSE;
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();
   if( 0 == rtdb )
      return kFALSE;

   // Unpacker parameter
   fMbsUnpackPar = (TMbsUnpackTofPar*) (rtdb->getContainer("TMbsUnpackTofPar"));
   if( 0 == fMbsUnpackPar )
      return kFALSE;

   // Call needed here to make available calibration parameters
   // in the TRB unpack stage
   fMbsCalibPar = (TMbsCalibTofPar*) (rtdb->getContainer("TMbsCalibTofPar"));
   if( 0 == fMbsCalibPar )
      return kFALSE;

   return kTRUE;
} 
// ------------------------------------------------------------------
Bool_t TMbsUnpTofMonitor::CreateHistogramms()
{
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->CreateHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scalormu ) )
      fScomUnp->CreateHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scaler2014 ) )
      fScal2014Unp->CreateHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglogscal ) )
      fTrloScalUnp->CreateHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::vftx ) )
      fVftxUnp->CreateHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::trbtdc ) )
      fTrbTdcUnp->CreateHistos();
   
   return kTRUE;
}
Bool_t TMbsUnpTofMonitor::FillHistograms()
{
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->FillHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scalormu ) )
      fScomUnp->FillHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scaler2014 ) )
      fScal2014Unp->FillHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglogscal ) )
      fTrloScalUnp->FillHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::vftx ) )
      fVftxUnp->FillHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::trbtdc ) )
      fTrbTdcUnp->FillHistos();
   
   return kTRUE;
}
Bool_t TMbsUnpTofMonitor::WriteHistogramms()
{
   TDirectory * oldir = gDirectory;
   TFile *fHist = new TFile("./tofMbsUnp.hst.root","RECREATE");
   
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->WriteHistos( fHist );
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scalormu ) )
      fScomUnp->WriteHistos( fHist );
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scaler2014 ) )
      fScal2014Unp->WriteHistos( fHist );
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglogscal ) )
      fTrloScalUnp->WriteHistos( fHist );
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::vftx ) )
      fVftxUnp->WriteHistos( fHist );
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::trbtdc ) )
      fTrbTdcUnp->WriteHistos( fHist );
      
   gDirectory->cd( oldir->GetPath() );
   fHist->Close();
   
   return kTRUE;
}
Bool_t TMbsUnpTofMonitor::DeleteHistograms()
{
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp->DeleteHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scalormu ) )
      fScomUnp->DeleteHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scaler2014 ) )
      fScal2014Unp->DeleteHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglogscal ) )
      fTrloScalUnp->DeleteHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::vftx ) )
      fVftxUnp->DeleteHistos();
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::trbtdc ) )
      fTrbTdcUnp->DeleteHistos();
      
   return kTRUE;
}
// ------------------------------------------------------------------
Bool_t TMbsUnpTofMonitor::CreateUnpackers()
{
   if( 0 == fMbsUnpackPar->GetNbActiveBoards() )
   {
      LOG(error)<<"TMbsUnpTofMonitor::CreateUnpackers => No active boards => No unpacker !?!?! ";
      return kFALSE;
   }
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglog ) )
      fTriglogUnp = new TTofTriglogUnpacker( fMbsUnpackPar );
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scalormu ) )
      fScomUnp    = new TTofScomUnpacker( fMbsUnpackPar );
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::scaler2014 ) )
      fScal2014Unp = new TTofScal2014Unpacker( fMbsUnpackPar );
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglogscal ) )
   {
      fTrloScalUnp = new TTofTriglogScalUnpacker( fMbsUnpackPar );
      fTrloScalUnp->SetHistoUserAxis( fdScalersEvoRangeUser, fdScalersEvoBinSzUser );
   } // if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::triglogscal ) )
/*
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::caenV1290 ) )
      fV1290Unp = new TTofV1290Unpacker( fMbsUnpackPar );
*/
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::vftx ) )
      fVftxUnp = new TTofVftxUnpacker( fMbsUnpackPar );
/*
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::get4 ) )
      fGet4Unp = new TTofGet4Unpacker( fMbsUnpackPar );
*/
   if( 0 < fMbsUnpackPar->GetNbActiveBoards( tofMbs::trbtdc ) )
      fTrbTdcUnp = new TTofTrbTdcUnpacker( fMbsUnpackPar, fMbsCalibPar );

   return kTRUE;
}
