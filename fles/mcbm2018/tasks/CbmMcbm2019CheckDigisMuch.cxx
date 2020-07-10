/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmMcbm2019CheckDigisMuch.h"

#include "CbmDigiManager.h"
#include "CbmMuchBeamTimeDigi.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include <TFile.h>

#include <iomanip>
#include <iostream>
using std::fixed;
using std::setprecision;

// ---- Default constructor -------------------------------------------
CbmMcbm2019CheckDigisMuch::CbmMcbm2019CheckDigisMuch()
  : FairTask("CbmMcbm2019CheckDigisMuch") {}

// ---- Destructor ----------------------------------------------------
CbmMcbm2019CheckDigisMuch::~CbmMcbm2019CheckDigisMuch() {}

// ----  Initialisation  ----------------------------------------------
void CbmMcbm2019CheckDigisMuch::SetParContainers() {
  // Load all necessary parameter containers from the runtime data base
  /*
   FairRunAna* ana = FairRunAna::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   <CbmMcbm2019CheckDigisMuchDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
   */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmMcbm2019CheckDigisMuch::Init() {

  // Digi Manager
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->UseMuchBeamTimeDigi();
  fDigiMan->Init();

  // Get a pointer to the previous already existing data level
  if (!fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    LOG(info) << "No MUCH digis found.";
  }

  CreateHistos();

  return kSUCCESS;
}


void CbmMcbm2019CheckDigisMuch::CreateHistos() {
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
      fdLastMuchDigi[uAsic][uChan]       = 0.0;
      fdLastMuchDigiPulser[uAsic][uChan] = 0.0;
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

  fDigisPerChanEvo =
    new TH2F("fDigisPerChanEvo",
             "Time dist of digis in same chan evo; Time [ ns ]; Channel [ ]",
             5000,
             0,
             5000.,
             kuMaxNbAsics * kuNbChansAsic,
             0.,
             kuMaxNbAsics * kuNbChansAsic);

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
InitStatus CbmMcbm2019CheckDigisMuch::ReInit() { return kSUCCESS; }

// ---- Exec ----------------------------------------------------------
void CbmMcbm2019CheckDigisMuch::Exec(Option_t* /*option*/) {
  LOG(debug) << "executing TS " << fNrTs;

  if (0 < fNrTs && 0 == fNrTs % 1000)
    LOG(info) << Form("Processing TS %6d", fNrTs);

  /// Zoom in on jump
  if (fNrTs < fuStartTs || fuStopTs < fNrTs) {
    fNrTs++;
    return;
  }  // if( fNrTs < fuStartTs || fuStopTs < fNrTs )

  Int_t nrMuchDigis = fDigiMan->GetNofDigis(ECbmModuleId::kMuch);
  LOG(debug) << GetName() << ": MuchDigis: " << nrMuchDigis;

  for (Int_t iMuch = 0; iMuch < nrMuchDigis; ++iMuch) {

    const CbmMuchBeamTimeDigi* Digi = fDigiMan->Get<CbmMuchBeamTimeDigi>(iMuch);

    Double_t dTime = Digi->GetTime();
    Double_t dAdc  = Digi->GetAdc();
    UInt_t uAsic   = Digi->GetNxId();
    UInt_t uChan   = Digi->GetNxCh();

    Double_t dTimeSinceStart = dTime - fdStartTime;
    fDigisPerAsicEvo->Fill(dTimeSinceStart, uAsic);

    if (0 == iMuch)
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
      //      std::cout << Form( "Much hit in TS %5d: asic %2u chan %3u T0 time %12.0f ADC %2.0f",
      //                            fNrTs, uAsic, uChan, (dTimeSinceStart - fdDigiDistStart), dAdc )
      //              << std::endl;
      Double_t dTimeDistLastDigi =
        dTimeSinceStart - fdLastMuchDigi[uAsic][uChan];
      fSameChanDigisDistEvo->Fill(dTimeSinceStart - fdDigiDistStart,
                                  dTimeDistLastDigi < 10000 ? dTimeDistLastDigi
                                                            : 9999);

      fDigisPerChanEvo->Fill(dTimeSinceStart - fdDigiDistStart,
                             uAsic * kuNbChansAsic + uChan);
    }
    /*
    if( 0.0 == fdLastMuchDigi[ uAsic ][ uChan ] )
       std::cout << Form( "Much first hit in TS %5d: asic %2u chan %3u T0 time %12.0f ADC %2.0f",
                               fNrTs, uAsic, uChan, dTime - fdFirstTsOffs, dAdc )
                 << std::endl;
*/
    fdLastMuchDigi[uAsic][uChan] = dTimeSinceStart;

    //    if( 9 != uAsic )
    if (9 != uAsic || uChan < 63) continue;
    if (fuMaxAdcPulserMuch < Digi->GetAdc()
        || Digi->GetAdc() < fuMinAdcPulserMuch)
      continue;

    std::cout << Form("Much pulser in TS %5d: chan %3u T0 time %12.0f time "
                      "start %12.0f ADC %2.0f dt %12.0f",
                      fNrTs,
                      uChan,
                      dTime,
                      dTimeSinceStart,
                      dAdc,
                      dTime - fdLastMuchDigiPulser[uAsic][uChan])
              << std::endl;
    fdLastMuchDigiPulser[uAsic][uChan] = dTime;
  }  // for (Int_t iMuch = 0; iMuch < nrMuchDigis; ++iMuch)

  fNrTs++;
}

// ---- Finish --------------------------------------------------------
void CbmMcbm2019CheckDigisMuch::Finish() { WriteHistos(); }

void CbmMcbm2019CheckDigisMuch::WriteHistos() {
  TFile* old     = gFile;
  TFile* outfile = TFile::Open(fOutFileName, "RECREATE");

  fDigisPerAsicEvo->Write();
  fSameChanDigisDistEvo->Write();
  fDigisPerChanEvo->Write();

  outfile->Close();
  delete outfile;

  gFile = old;
}

ClassImp(CbmMcbm2019CheckDigisMuch)
