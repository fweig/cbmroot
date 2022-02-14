/* Copyright (C) 2021 Goethe-University, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Adrian Weber [committer]*/

#include "CbmRichUnpackMonitor.h"

#include "MicrosliceDescriptor.hpp"

#include <FairRun.h>
#include <FairRunOnline.h>
#include <Logger.h>

#include <RtypesCore.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <THttpServer.h>
#include <TProfile.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cmath>

CbmRichUnpackMonitor::CbmRichUnpackMonitor(/* args */) : vNbMessType(7, 0), fvpAllHistoPointers()
{
}

CbmRichUnpackMonitor::~CbmRichUnpackMonitor()
{
  for (auto iter = fvpAllHistoPointers.begin(); iter != fvpAllHistoPointers.end();) {
    if (iter->first != nullptr) { delete iter->first; }
    iter = fvpAllHistoPointers.erase(iter);
  }
}

Bool_t CbmRichUnpackMonitor::CreateHistograms(CbmMcbm2018RichPar* pUnpackPar)
{
  TString sHistName {""};
  TString title {""};
  //const UInt_t uNbAsics       = pUnpackPar->GetNrOfAsics();


  /// Create general unpacking histograms
  fhDigisTimeInRun = new TH1I("hRichDigisTimeInRun", "Digis Nb vs Time in Run; Time in run [s]; Digis Nb []", 10, 0, 1);
  fhDigisTimeInRun->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhDigisTimeInRun, "");
  
  fhDigisToT = new TH1F("hDigisToT", "fhDigisToT; ToT [ns]; Entries", 400, 0, 40.);
  fhDigisTimeInRun->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhDigisToT, "");  

  fhVectorSize = new TH1I("fhVectorSize", "Size of the vector VS TS index; TS index; Size [bytes]", 10, 0, 10);
  fhVectorSize->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhVectorSize, "");

  fhVectorCapacity =
    new TH1I("fhVectorCapacity", "Size of the vector VS TS index; TS index; Size [bytes]", 10000, 0., 10000.);
  AddHistoToVector(fhVectorCapacity, "");

  return kTRUE;
}


Bool_t CbmRichUnpackMonitor::ResetHistograms()
{
  fhDigisTimeInRun->Reset();
  fhVectorSize->Reset();
  fhVectorCapacity->Reset();

  return kTRUE;
}

Bool_t CbmRichUnpackMonitor::CreateDebugHistograms(CbmMcbm2018RichPar* pUnpackPar)
{
  return kTRUE;
}

Bool_t CbmRichUnpackMonitor::ResetDebugHistograms()
{
  return kTRUE;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmRichUnpackMonitor::FillPerTimesliceCountersHistos(double_t dTsStartTimeS)
{
  if (0 == dFirstTsStartTime) dFirstTsStartTime = dTsStartTimeS;
  double_t dTimeInRun     = dTsStartTimeS - dFirstTsStartTime;
  double_t dRatio         = 0;
}


// -------------------------------------------------------------------------
void CbmRichUnpackMonitor::PrintDebugInfo(const uint64_t MsStartTime, const size_t NrProcessedTs,
                                         const uint16_t msDescriptorFlags, const uint32_t uSize)
{
}

// ---- Init ----
Bool_t CbmRichUnpackMonitor::Init(CbmMcbm2018RichPar* parset)
{
  /// Trigger histo creation on all associated monitors
  CreateHistograms(parset);
  if (fDebugMode) CreateDebugHistograms(parset);

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = GetHistoVector();

  /// Register the histos in the HTTP server
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (nullptr != server) {
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      server->Register(Form("/%s", vHistos[uHisto].second.data()), vHistos[uHisto].first);
    }
    server->RegisterCommand("/Reset_UnpRich_Hist", "bMcbm2018UnpackerTaskRichResetHistos=kTRUE");
    server->Restrict("/Reset_UnpRich_Hist", "allow=admin");
  }

  return kTRUE;
}

// ---- Finish ----
void CbmRichUnpackMonitor::Finish()
{

  /// Obtain vector of pointers on each histo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = GetHistoVector();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;
  TFile* histoFile   = nullptr;

  // open separate histo file in recreate mode
  histoFile = new TFile(fHistoFileName, "RECREATE");
  histoFile->cd();
  
  /// Write histos to output file
  for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
    /// Make sure we end up in chosen folder
    TString sFolder = vHistos[uHisto].second.data();
    if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
    gDirectory->cd(sFolder);

    /// Write plot
    vHistos[uHisto].first->Write();

    histoFile->cd();
  }

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  histoFile->Close();
  delete histoFile;
}


ClassImp(CbmRichUnpackMonitor)
