// ------------------------------------------------------------------
// -----                    TMbsCalibTof                       -----
// -----              Created 19/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TMbsCalibTof.h"

// General Unpack headers
#include "TMbsUnpackTofPar.h"

// ToF specific headers
#include "TMbsCalibTdcTof.h"
#include "TMbsCalibTofPar.h"
#include "TofTdcDef.h"
#include "TofCaenDef.h"
#include "TofVftxDef.h"
#include "TofTrbTdcDef.h"
#include "TofGet4Def.h"
#include "TTofTdcBoard.h"
#include "TTofTdcData.h"
#include "TTofCalibData.h"
#include "TTofCalibScaler.h"
#include "TMbsCalibScalTof.h"
#include "TTofTriglogBoard.h"

// FAIR headers
#include "FairLogger.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"

// ROOT headers
#include "Riostream.h" // for cout, endl, etc...
#include "TClonesArray.h"
#include "TH2.h"
#include "TH1.h"
#include "TTimeStamp.h"
#include <TFile.h>

TMbsCalibTof::TMbsCalibTof() : 
   FairTask("MbsCalibTof"),
   fMbsUnpackPar(NULL),
   fMbsCalibPar(NULL),
   fTdcCalibrator(NULL),
   fScalerCalibrator(NULL),
   fTriglogBoardCollection(NULL),
   fbSaveCalibScalers(kFALSE),
   fdScalersEvoRangeUser(-1), 
   fdScalersEvoBinSzUser(-1),
   fbSaveCalibTdcs(kFALSE),
   fsTdcCalibOutFoldername("./"),
   fsTdcCalibFilename(""),
   fbTdcCalibGsiSep14Fix(kFALSE),
   fbTdcRefMoniMode(kFALSE)
{
}

TMbsCalibTof::TMbsCalibTof(const char* name, Int_t /*mode*/, Int_t verbose) :
   FairTask(name, verbose),
   fMbsUnpackPar(0),
   fMbsCalibPar(NULL),
   fTdcCalibrator(NULL),
   fScalerCalibrator(NULL),
   fTriglogBoardCollection(NULL),
   fbSaveCalibScalers(kFALSE),
   fdScalersEvoRangeUser(-1), 
   fdScalersEvoBinSzUser(-1),
   fbSaveCalibTdcs(kFALSE),
   fsTdcCalibOutFoldername("./"),
   fsTdcCalibFilename(""),
   fbTdcCalibGsiSep14Fix(kFALSE),
   fbTdcRefMoniMode(kFALSE)
{
}
      
TMbsCalibTof::~TMbsCalibTof()
{
   DeleteHistograms();
   LOG(info)<<"**** TMbsCalibTof: Delete instance ";
}

// --------------------------------------------------
// Fairtask specific functions
void TMbsCalibTof::SetParContainers()
{
   InitParameters();
}
InitStatus TMbsCalibTof::ReInit()
{
   LOG(info)<<"**** TMbsCalibTof: Reinitialize the unpack parameters for tof ";
   if( kFALSE == InitParameters() )
      return kFATAL;
   fMbsCalibPar->printParams();
   
   return kSUCCESS;
}

InitStatus TMbsCalibTof::Init()
{
   if( kFALSE == InitParameters() )
      return kFATAL;
   fMbsCalibPar->printParams();
   
   // Create the proper Calibrators and number of output objects for each boards type
   if( kFALSE == InitCalibrators() )
      return kFATAL;    
   
   return kSUCCESS;
}
void TMbsCalibTof::Exec(Option_t* /*option*/)
{   
   // Run processing functions of all enables calibrators
   ExecCalibrators();
}
void TMbsCalibTof::Finish()
{
   CloseCalibrators();
      
   WriteHistogramms();
}

// ------------------------------------------------------------------
Bool_t TMbsCalibTof::InitParameters()
{
   // Get Base Container
   FairRun* ana = FairRun::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   // Unpack parameter
   fMbsUnpackPar = (TMbsUnpackTofPar*) (rtdb->getContainer("TMbsUnpackTofPar"));
   if( 0 == fMbsUnpackPar )
   {
      LOG(error)<<"TMbsCalibTof::InitParameters => Could not obtain the TMbsUnpackTofPar ";
      return kFALSE;
   }
      
   // Calibration parameter
   fMbsCalibPar = (TMbsCalibTofPar*) (rtdb->getContainer("TMbsCalibTofPar"));
   if( 0 == fMbsCalibPar )
   {
      LOG(error)<<"TMbsCalibTof::InitParameters => Could not obtain the TMbsCalibTofPar ";
      return kFALSE; 
   }

   // PAL: Added to recover loading of parameters before initialization
//   rtdb->initContainers(  ana->GetRunId() );

   return kTRUE;
}
Bool_t TMbsCalibTof::InitCalibrators()
{
   // Scalers
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
   {
      fScalerCalibrator = new TMbsCalibScalTof( fMbsUnpackPar, fMbsCalibPar );
      fScalerCalibrator->RegisterInput();
      fScalerCalibrator->SetSaveScalers( fbSaveCalibScalers );
      fScalerCalibrator->SetHistoUserAxis( fdScalersEvoRangeUser, fdScalersEvoBinSzUser );
      fScalerCalibrator->RegisterOutput();
      fScalerCalibrator->CreateHistogramms();
      if( kFALSE == fScalerCalibrator->InitScalersCalib() )
         return kFALSE;
   } // if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
      
   // TDCs
   if( 0 < fMbsUnpackPar->GetNbActiveTdcs() )
   {
      fTdcCalibrator = new TMbsCalibTdcTof( fMbsUnpackPar, fMbsCalibPar );
      fTdcCalibrator->RegisterInput();
      fTdcCalibrator->SetSaveTdcs( fbSaveCalibTdcs );
      fTdcCalibrator->SetCalibFilename(fsTdcCalibFilename);
      fTdcCalibrator->SetCalibOutFolder(fsTdcCalibOutFoldername);
      fTdcCalibrator->SetSep14Fix(fbTdcCalibGsiSep14Fix);
      fTdcCalibrator->EnaTdcRefMoniMode(fbTdcRefMoniMode);
      fTdcCalibrator->RegisterOutput();
      fTdcCalibrator->CreateHistogramms();
      if( kFALSE == fTdcCalibrator->InitiTdcCalib() )
         return kFALSE;
   } // if( 0 < fMbsUnpackPar->GetNbActiveTdcs() )

   // Event rejection
   // E.g.: pulser event used to Empty the VFTX buffers
   if( 1 == fMbsUnpackPar->OnlyOneTriglog() &&
       -1 < fMbsUnpackPar->GetTriggerToReject() )
   {
      LOG(info)<<"TMbsCalibTof::RegisterInput => Load the TofTriglog TClonesArray for trigger rejection!!!";
      FairRootManager* rootMgr = FairRootManager::Instance();
      fTriglogBoardCollection = (TClonesArray*) rootMgr->GetObject("TofTriglog");
      if( NULL == fTriglogBoardCollection)
      {
         LOG(error)<<"TMbsCalibTof::RegisterInput => Could not get the TofTriglog TClonesArray!!!";
         return kFALSE;
      } // if( NULL == fTriglogBoardCollection)
   } // if trigger rejection enabled and trigger board enabled

   return kTRUE;
}
Bool_t TMbsCalibTof::ExecCalibrators()
{
   // Scalers
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
      fScalerCalibrator->ClearCalib();
   // TDCs
   if( 0 < fMbsUnpackPar->GetNbActiveTdcs() )
      fTdcCalibrator->ClearCalib();

   // Event rejection
   // E.g.: pulser event used to Empty the VFTX buffers
   if( 1 == fMbsUnpackPar->OnlyOneTriglog() &&
       -1 < fMbsUnpackPar->GetTriggerToReject() )
   {
//      if( NULL == fTriglogBoardCollection )
//         return kFALSE;

      TTofTriglogBoard * xTriglogBoard = (TTofTriglogBoard*) fTriglogBoardCollection->At(0); // Always only 1 TRIGLOG board!

//      if( NULL == xTriglogBoard )
//         return kFALSE;
//      if( 1 == ( 0x1 & ( (xTriglogBoard->GetTriggPatt()) >> (fMbsUnpackPar->GetTriggerToReject()) ) ) )
      // Reject selected trigger only when alone (accept when both not rejected trigger and rejected trigger are present)
      if( (xTriglogBoard->GetTriggPatt()) == ( 1ul << (fMbsUnpackPar->GetTriggerToReject()) ) )
         // Jump this event !
         return kTRUE;
   } // if trigger rejection enabled and trigger board enabled

   // Scalers
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
   {
//      fScalerCalibrator->ClearCalib();
      fScalerCalibrator->CalibScalers();
      fScalerCalibrator->FillHistograms();
   } // if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
      
   // TDCs
   if( 0 < fMbsUnpackPar->GetNbActiveTdcs() )
   {
//      fTdcCalibrator->ClearCalib();
      fTdcCalibrator->CalibTdc();
      fTdcCalibrator->FillHistograms();
   } // if( 0 < fMbsUnpackPar->GetNbActiveTdcs() )
   
   return kTRUE;
}
Bool_t TMbsCalibTof::CloseCalibrators()
{
   // Scalers
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
      fScalerCalibrator->CloseScalersCalib();
      
   // TDCs
   if( 0 < fMbsUnpackPar->GetNbActiveTdcs() )
      fTdcCalibrator->CloseTdcCalib();
   
   return kTRUE;
}
// ------------------------------------------------------------------
Bool_t TMbsCalibTof::CreateHistogramms()
{
   // Calibrator initalisation functions should take care of creating their own
   // => for the task own histos
   return kTRUE;
}
Bool_t TMbsCalibTof::FillHistograms()
{
   // Calibrator filling functions should take care of creating their own
   // => for the task own histos
   return kTRUE;
}
void TMbsCalibTof::WriteHistogramms()
{
   TDirectory * oldir = gDirectory;
   TFile *fHist = new TFile("./tofMbsCal.hst.root","RECREATE");
   
   // Scalers
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
      fScalerCalibrator->WriteHistogramms( fHist );
      
   // TDCs
   if( 0 < fMbsUnpackPar->GetNbActiveTdcs() )
      fTdcCalibrator->WriteHistogramms( fHist );
   
   // Task own histos
      // TODO if necessary
      
   gDirectory->cd( oldir->GetPath() );
   
   fHist->Close();
}
void TMbsCalibTof::DeleteHistograms()
{
   // Calibrator destructors should take care of destroying their own
   // => for the task own histos
}
// ------------------------------------------------------------------
Bool_t TMbsCalibTof::RegisterInput()
{
   // Scalers
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
      if( kFALSE == fScalerCalibrator->RegisterInput() )
         return kFALSE;
      
   // TDCs
   if( 0 < fMbsUnpackPar->GetNbActiveTdcs() )
      if( kFALSE == fTdcCalibrator->RegisterInput() )
         return kFALSE;

   return kTRUE;
}
Bool_t TMbsCalibTof::RegisterOutput()
{
   // Scalers
   if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
   {
      fScalerCalibrator->SetSaveScalers( fbSaveCalibScalers );
      fScalerCalibrator->SetHistoUserAxis( fdScalersEvoRangeUser, fdScalersEvoBinSzUser );
      fScalerCalibrator->RegisterOutput();
   } // if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )
      
   // TDCs
   if( 0 < fMbsUnpackPar->GetNbActiveTdcs() )
   {
      fTdcCalibrator->SetSaveTdcs( fbSaveCalibTdcs );
      fTdcCalibrator->SetCalibFilename(fsTdcCalibFilename);
      fTdcCalibrator->SetCalibOutFolder(fsTdcCalibOutFoldername);
      fTdcCalibrator->RegisterOutput();
   } // if( 0 < fMbsUnpackPar->GetNbActiveScalersB() )

   return kTRUE;
}
Bool_t TMbsCalibTof::ClearOutput()
{
   return kTRUE;
}
// ------------------------------------------------------------------
