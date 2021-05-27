/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmMcbmCheckTimingTask.h"

/// CBM headers

/// FAIRROOT headers
#include <Logger.h>

/// FAIRSOFT headers (geant, boost, ...)
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include <TDirectory.h>
#include <TFile.h>

/// C/C++ headers

// ---- Default constructor -------------------------------------------
CbmMcbmCheckTimingTask::CbmMcbmCheckTimingTask() : FairTask("CbmMcbmCheckTimingTask")
{
  /// Create Algo. To be made generic/switchable when more event building algo are available!
  fpAlgo = new CbmMcbmCheckTimingAlgo();
}

// ---- Destructor ----------------------------------------------------
CbmMcbmCheckTimingTask::~CbmMcbmCheckTimingTask() {}

// ----  Initialisation  ----------------------------------------------
void CbmMcbmCheckTimingTask::SetParContainers()
{
  /// Nothing to do
}

// ---- Init ----------------------------------------------------------
InitStatus CbmMcbmCheckTimingTask::Init()
{
  /// Call Algo Init method
  if (kTRUE == fpAlgo->Init()) return kSUCCESS;
  else
    return kFATAL;
}

// ---- ReInit  -------------------------------------------------------
InitStatus CbmMcbmCheckTimingTask::ReInit() { return kSUCCESS; }

// ---- Exec ----------------------------------------------------------
void CbmMcbmCheckTimingTask::Exec(Option_t* /*option*/)
{
  LOG(debug2) << "CbmMcbmCheckTimingTask::Exec => Starting sequence";
  /// Call Algo ProcessTs method
  fpAlgo->ProcessTs();

  LOG(debug2) << "CbmMcbmCheckTimingTask::Exec => Done";
}


// ---- Finish --------------------------------------------------------
void CbmMcbmCheckTimingTask::Finish()
{
  SaveHistos();

  /// Call Algo finish method
  fpAlgo->Finish();
}
//----------------------------------------------------------------------
void CbmMcbmCheckTimingTask::SaveHistos()
{
  fpAlgo->WriteHistos();
  /*
   /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
   std::vector< std::pair< TNamed *, std::string > > vHistos = fpAlgo->GetHistoVector();

   /// Save old global file and folder pointer to avoid messing with FairRoot
   TFile* oldFile     = gFile;
   TDirectory* oldDir = gDirectory;

   /// (Re-)Create ROOT file to store the histos
   TFile* histoFile   = nullptr;

   /// open separate histo file in recreate mode
   histoFile = new TFile( fsOutFileName , "RECREATE");
   histoFile->cd();

   /// Save all plots and create folders if needed
   for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )
   {
      /// Make sure we end up in chosen folder
      TString sFolder = vHistos[ uHisto ].second.data();
      if( nullptr == gDirectory->Get( sFolder ) )
         gDirectory->mkdir( sFolder );
      gDirectory->cd( sFolder );

      /// Write plot
      vHistos[ uHisto ].first->Write();

      histoFile->cd();
   } // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

  /// Restore original directory position
  gFile      = oldFile;
  gDirectory = oldDir;
*/
}
//----------------------------------------------------------------------
void CbmMcbmCheckTimingTask::SetOutFilename(TString sNameIn)
{
  fsOutFileName = sNameIn;
  fpAlgo->SetOutFilename(fsOutFileName);
}
//----------------------------------------------------------------------
void CbmMcbmCheckTimingTask::SetReferenceDetector(ECbmModuleId refDetIn, std::string sNameIn, Double_t dTimeRangeBegIn,
                                                  Double_t dTimeRangeEndIn, UInt_t uRangeNbBinsIn,
                                                  UInt_t uChargeCutMinIn, UInt_t uChargeCutMaxIn)
{
  fpAlgo->SetReferenceDetector(refDetIn, sNameIn, dTimeRangeBegIn, dTimeRangeEndIn, uRangeNbBinsIn, uChargeCutMinIn,
                               uChargeCutMaxIn);
}
void CbmMcbmCheckTimingTask::AddCheckDetector(ECbmModuleId detIn, std::string sNameIn, Double_t dTimeRangeBegIn,
                                              Double_t dTimeRangeEndIn, UInt_t uRangeNbBinsIn, UInt_t uChargeCutMinIn,
                                              UInt_t uChargeCutMaxIn)
{
  fpAlgo->AddCheckDetector(detIn, sNameIn, dTimeRangeBegIn, dTimeRangeEndIn, uRangeNbBinsIn, uChargeCutMinIn,
                           uChargeCutMaxIn);
}

void CbmMcbmCheckTimingTask::RemoveCheckDetector(ECbmModuleId detIn) { fpAlgo->RemoveCheckDetector(detIn); }

//----------------------------------------------------------------------
ClassImp(CbmMcbmCheckTimingTask)
