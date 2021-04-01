/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmMcbm2019CheckDigisSts.h"

#include "CbmDigiManager.h"
#include "CbmStsDigi.h"

#include "FairRootManager.h"
#include "FairRunOnline.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include <TDirectory.h>
#include <TFile.h>

#include <iomanip>
#include <iostream>
using std::fixed;
using std::setprecision;

// ---- Default constructor -------------------------------------------
CbmMcbm2019CheckDigisSts::CbmMcbm2019CheckDigisSts()
  : FairTask("CbmMcbm2019CheckDigisSts") {}

// ---- Destructor ----------------------------------------------------
CbmMcbm2019CheckDigisSts::~CbmMcbm2019CheckDigisSts() {}

// ----  Initialisation  ----------------------------------------------
void CbmMcbm2019CheckDigisSts::SetParContainers() {
  // Load all necessary parameter containers from the runtime data base
  /*
   FairRunAna* ana = FairRunAna::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   <CbmMcbm2019CheckDigisStsDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
   */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmMcbm2019CheckDigisSts::Init() {
  // Digi Manager
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  // Get a pointer to the previous already existing data level
  if (!fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    LOG(info) << "No TClonesArray with STS digis found.";
  }

  CreateHistos();

  return kSUCCESS;
}


void CbmMcbm2019CheckDigisSts::CreateHistos() {
  Double_t dZoomDuration = (fuStopTs - fuStartTs) * fdTsLength;
  fDigisPerAsicEvo       = new TH2F("fDigisPerAsicEvo",
                              "Digis per Asic evo; Time [ ns ]; ASIC []",
                              dZoomDuration / 1000,
                              0,
                              dZoomDuration,
                              kuMaxNbAsics,
                              0,
                              kuMaxNbAsics);

  for (UInt_t uAsic = 0; uAsic < kuMaxNbAsics; ++uAsic)
    for (UInt_t uChan = 0; uChan < kuNbChansAsic; ++uChan) {
      fdLastStsDigi[uAsic][uChan]       = 0.0;
      fdLastStsDigiPulser[uAsic][uChan] = 0.0;
    }  // loop on channel and asic

  fSameChanDigisDistEvo = new TH2F(
    "fSameChanDigisDistEvo",
    "Time dist of digis in same chan evo; Time [ ns ]; Same chan dist [ ns ]",
    5000,
    0,
    500000.,
    1000,
    0.,
    10000.);

  std::cout << Form("TS with jump %5u, first TS time off %12.0f, start TS %5u "
                    "stop TS %5u, TS length %9.0f Start time %12.0f",
                    fuTsJump,
                    fdFirstTsOffs,
                    fuStartTs,
                    fuStopTs,
                    fdTsLength,
                    fdStartTime)
            << std::endl;
}
// ---- ReInit  -------------------------------------------------------
InitStatus CbmMcbm2019CheckDigisSts::ReInit() { return kSUCCESS; }

// ---- Exec ----------------------------------------------------------
void CbmMcbm2019CheckDigisSts::Exec(Option_t* /*option*/) {
  LOG(debug) << "executing TS " << fNrTs;

  if (0 < fNrTs && 0 == fNrTs % 1000)
    LOG(info) << Form("Processing TS %6d", fNrTs);

  /// Zoom in on jump
  if (fNrTs < fuStartTs || fuStopTs < fNrTs) {
    fNrTs++;
    return;
  }  // if( fNrTs < fuStartTs || fuStopTs < fNrTs )

  Int_t nrStsDigis = fDigiMan->GetNofDigis(ECbmModuleId::kSts);
  LOG(debug) << "StsDigis: " << nrStsDigis;


  for (Int_t iSts = 0; iSts < nrStsDigis; ++iSts) {

    const CbmStsDigi* Digi = fDigiMan->Get<CbmStsDigi>(iSts);

    Double_t dTime = Digi->GetTime();
    Double_t dAdc  = Digi->GetCharge();
    UInt_t uChan   = Digi->GetChannel();
    UInt_t uAsic   = uChan / kuNbChansAsic;
    uChan %= kuNbChansAsic;

    Double_t dTimeSinceStart = dTime - fdStartTime;
    fDigisPerAsicEvo->Fill(dTimeSinceStart, uAsic);

    if (0 == iSts)
      std::cout << Form("Much first hit in TS %5d: asic %2u chan %3u time "
                        "%12.0f T0 time %12.0f check time %12.0f ADC %2.0f",
                        fNrTs,
                        uAsic,
                        uChan,
                        dTime,
                        dTime - fdFirstTsOffs,
                        dTimeSinceStart,
                        dAdc)
                << std::endl;

    if (fdDigiDistStart < dTimeSinceStart && dTimeSinceStart < fdDigiDistStop) {
      //      std::cout << Form( "Sts hit in TS %5d: asic %2u chan %3u T0 time %12.0f ADC %2.0f",
      //                            fNrTs, uAsic, uChan, (dTimeSinceStart - fdDigiDistStart), dAdc )
      //              << std::endl;
      Double_t dTimeDistLastDigi =
        dTimeSinceStart - fdLastStsDigi[uAsic][uChan];
      fSameChanDigisDistEvo->Fill(dTimeSinceStart - fdDigiDistStart,
                                  dTimeDistLastDigi < 10000 ? dTimeDistLastDigi
                                                            : 9999);
    }
    /*
    if( 0.0 == fdLastStsDigi[ uAsic ][ uChan ] )
       std::cout << Form( "Sts first hit in TS %5d: asic %2u chan %3u T0 time %12.0f ADC %2.0f",
                               fNrTs, uAsic, uChan, dTime - fdFirstTsOffs, dAdc )
                 << std::endl;
*/
    fdLastStsDigi[uAsic][uChan] = dTimeSinceStart;

    //    if( 9 != uAsic )
    if (9 != uAsic || uChan < 63) continue;
    if (fuMaxAdcPulserSts < Digi->GetCharge()
        || Digi->GetCharge() < fuMinAdcPulserSts)
      continue;

    std::cout << Form("Sts pulser in TS %5d: chan %3u T0 time %12.0f time "
                      "start %12.0f ADC %2.0f dt %12.0f",
                      fNrTs,
                      uChan,
                      dTime,
                      dTimeSinceStart,
                      dAdc,
                      dTime - fdLastStsDigiPulser[uAsic][uChan])
              << std::endl;
    fdLastStsDigiPulser[uAsic][uChan] = dTime;
  }  // for (Int_t iSts = 0; iSts < nrStsDigis; ++iSts)

  fNrTs++;
}

// ---- Finish --------------------------------------------------------
void CbmMcbm2019CheckDigisSts::Finish() { WriteHistos(); }

void CbmMcbm2019CheckDigisSts::WriteHistos() {
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* outfile = TFile::Open(fOutFileName, "RECREATE");

  fDigisPerAsicEvo->Write();
  fSameChanDigisDistEvo->Write();

  outfile->Close();
  delete outfile;

  gFile      = oldFile;
  gDirectory = oldDir;
}

ClassImp(CbmMcbm2019CheckDigisSts)
