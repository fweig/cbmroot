/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmMcbm2019CheckPulser.h"

#include "CbmStsDigi.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmRichDigi.h"
#include "CbmTofDigi.h"
#include "CbmPsdDigi.h"
#include "CbmDigiManager.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "THttpServer.h"
#include <TFile.h>


#include <iomanip>
#include <iostream>
#include <type_traits>
using std::fixed;
using std::setprecision;

// ---- Default constructor -------------------------------------------
CbmMcbm2019CheckPulser::CbmMcbm2019CheckPulser()
  : FairTask("CbmMcbm2019CheckPulser")
{
}

// ---- Destructor ----------------------------------------------------
CbmMcbm2019CheckPulser::~CbmMcbm2019CheckPulser()
{
}

// ----  Initialisation  ----------------------------------------------
void CbmMcbm2019CheckPulser::SetParContainers()
{
  // Load all necessary parameter containers from the runtime data base
  /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <CbmMcbm2019CheckPulserDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmMcbm2019CheckPulser::Init()
{

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Digi manager
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->UseMuchBeamTimeDigi();
  fDigiMan->Init();

  // T0 is not included in DigiManager; have to take care here
  // Try to find a vector branch for the digi
  fT0DigiVector = ioman->InitObjectAs<std::vector<CbmTofDigi> const *>("T0Digi");
  if ( ! fT0DigiVector ) {
    LOG(info) << "No T0 digi vector found; trying TClonesArray";
    if ( std::is_convertible<TObject*, CbmTofDigi*>::value ) {
      fT0DigiArray = dynamic_cast<TClonesArray*>(ioman->GetObject("T0Digi"));
      if ( ! fT0DigiArray ) LOG(info) << "No T0 digi input found.";
    } //? CbmTofDigi derives from TObject
  } //? No vector for T0 digis

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kSts) ) {
    LOG(info) << "No STS digi input found.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kMuch) ) {
    LOG(info) << "No MUCH digi input found.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kTof) ) {
    LOG(info) << "No TOF digi input found.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kRich) ) {
    LOG(info) << "No RICH digi input found.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kPsd) ) {
    LOG(info) << "No PSD digi input found.";
  }

  CreateHistos();

  return kSUCCESS;
}

Int_t CbmMcbm2019CheckPulser::CalcNrBins(Int_t offsetRange) {

  if (offsetRange < 251) {
    Double_t dClocks = offsetRange;
    dClocks /= 6.25;
    return (dClocks * 112 * 2); /// T0/TOF FTS bining
  } else if (offsetRange < 501) {
    fBinWidth = 1;
  } else if (offsetRange < 1001) {
    fBinWidth = 5;
  } else if (offsetRange < 10001) {
    fBinWidth = 10;
  } else if (offsetRange < 100001) {
    fBinWidth = 100;
  } else {
    fBinWidth = 100;
  }

  return (offsetRange/fBinWidth *2);
}

void CbmMcbm2019CheckPulser::CreateHistos()
{
   /// Logarithmic bining for self time comparison
   // Number of log bins =
   //      9 for the sub-unit decade
   //    + 9 for each unit of each decade * 10 for the subdecade range
   //    + 1 for the closing bin top edge
   const Int_t iNbDecadesLog     = 9;
   const Int_t iNbStepsDecade    = 9;
   const Int_t iNbSubStepsInStep = 1;
   const Int_t iNbBinsLog = iNbStepsDecade
                           + iNbStepsDecade * iNbSubStepsInStep * iNbDecadesLog
                           + 1;
   Double_t dBinsLog[iNbBinsLog];
      // First fill sub-unit decade
   for( Int_t iSubU = 0; iSubU < iNbStepsDecade; iSubU ++ )
      dBinsLog[ iSubU ] = 0.1 * ( 1 + iSubU );
   std::cout << std::endl;
      // Then fill the main decades
   Double_t dSubstepSize = 1.0 / iNbSubStepsInStep;
   for( Int_t iDecade = 0; iDecade < iNbDecadesLog; iDecade ++)
   {
      Double_t dBase = std::pow( 10, iDecade );
      Int_t iDecadeIdx = iNbStepsDecade
                       + iDecade * iNbStepsDecade * iNbSubStepsInStep;
      for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
      {
         Int_t iStepIdx = iDecadeIdx + iStep * iNbSubStepsInStep;
         for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
         {
            dBinsLog[ iStepIdx + iSubStep ] = dBase * (1 + iStep)
                                             + dBase * dSubstepSize * iSubStep;
         } // for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
      } // for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
   } // for( Int_t iDecade = 0; iDecade < iNbDecadesLog; iDecade ++)
   dBinsLog[ iNbBinsLog - 1 ] = std::pow( 10, iNbDecadesLog );

  Int_t nrOfBinsSts =  CalcNrBins(fStsOffsetRange);
  // T0 vs. Sts
  fT0StsDiff = new TH1F("fT0StsDiff","T0-Sts;time diff [ns];Counts",
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);

  fT0StsDiffEvo = new TH2F("fT0StsDiffEvo","T0-Sts;TS; time diff [ns];Counts",
       1000, 0, 10000,
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);


  Int_t nrOfBinsMuch =  CalcNrBins(fMuchOffsetRange);
  // T0 vs. Much
  fT0MuchDiff = new TH1F("fT0MuchDiff","T0-Much;time diff [ns];Counts",
			 nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);

  fT0MuchDiffEvo = new TH2F("fT0MuchDiffEvo","T0-Much;TS; time diff [ns];Counts",
       1000, 0, 10000,
			 nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);


  Int_t nrOfBinsTof =  CalcNrBins(fTofOffsetRange);
  // To vs. Tof
  fT0TofDiff = new TH1F("fT0TofDiff","T0-Tof;time diff [ns];Counts",
			nrOfBinsTof, -fTofOffsetRange, fTofOffsetRange);

  fT0TofDiffEvo = new TH2F("fT0TofDiffEvo","T0-Tof;TS; time diff [ns];Counts",
       1000, 0, 10000,
			nrOfBinsTof, -fTofOffsetRange, fTofOffsetRange);


  Int_t nrOfBinsRich =  CalcNrBins(fRichOffsetRange);
  // To vs. Rich
  fT0RichDiff = new TH1F("fT0RichDiff","T0-Rich;time diff [ns];Counts",
			nrOfBinsRich, -fRichOffsetRange, fRichOffsetRange);

  fT0RichDiffEvo = new TH2F("fT0RichDiffEvo","T0-Rich;TS; time diff [ns];Counts",
       1000, 0, 10000,
			nrOfBinsRich, -fRichOffsetRange, fRichOffsetRange);

  Int_t nrOfBinsPsd =  CalcNrBins(fPsdOffsetRange);
  // To vs. Rich
  fT0PsdDiff = new TH1F("fT0PsdDiff","T0-Psd;time diff [ns];Counts",
			nrOfBinsPsd, -fPsdOffsetRange, fPsdOffsetRange);

  fT0PsdDiffEvo = new TH2F("fT0PsdDiffEvo","T0-Psd;TS; time diff [ns];Counts",
       1000, 0, 10000,
			nrOfBinsPsd, -fPsdOffsetRange, fPsdOffsetRange);

  fT0PsdDiffCharge = new TH2F("fT0PsdDiffCharge","T0-Psd;time diff [ns]; Charge [a.u]; ;Counts",
			nrOfBinsPsd, -fPsdOffsetRange, fPsdOffsetRange,
      7000, 0, 70000
      );


  // T0 vs. Sts
  fT0StsDiffEvoLong = new TH2F("fT0StsDiffEvoLong","T0-Sts;TS; time diff [ns];Counts",
       1800, 0, 180000,
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);
  // T0 vs. Much
  fT0MuchDiffEvoLong = new TH2F("fT0MuchDiffEvoLong","T0-Much;TS; time diff [ns];Counts",
       1800, 0, 180000,
			 nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);
  // To vs. Tof
  fT0TofDiffEvoLong = new TH2F("fT0TofDiffEvoLong","T0-Tof;TS; time diff [ns];Counts",
       1800, 0, 180000,
			nrOfBinsTof, -fTofOffsetRange, fTofOffsetRange);
  // To vs. Rich
  fT0RichDiffEvoLong = new TH2F("fT0RichDiffEvoLong","T0-Rich;TS; time diff [ns];Counts",
       1800, 0, 180000,
			nrOfBinsRich, -fRichOffsetRange, fRichOffsetRange);

  // To vs. Psd
  fT0PsdDiffEvoLong = new TH2F("fT0PsdDiffEvoLong","T0-Psd;TS; time diff [ns];Counts",
       1800, 0, 180000,
			nrOfBinsPsd, -fPsdOffsetRange, fPsdOffsetRange);


  // T0 vs. Sts
  fT0StsMeanEvo = new TProfile("fT0StsMeanEvo","T0-Sts; time in run [s]; Mean time diff [ns]",
       4320, 0, 4320 );
  // T0 vs. Much
  fT0MuchMeanEvo = new TProfile("fT0MuchMeanEvo","T0-Much; time in run [s]; Mean time diff [ns]",
       4320, 0, 4320 );
  // To vs. Tof
  fT0TofMeanEvo = new TProfile("fT0TofMeanEvo","T0-Tof; time in run [s]; Mean time diff [ns]",
       4320, 0, 4320 );
  // To vs. Rich
  fT0RichMeanEvo = new TProfile("fT0RichMeanEvo","T0-Rich; time in run [s]; Mean time diff [ns]",
       4320, 0, 4320);
  // To vs. Psd
  fT0PsdMeanEvo = new TProfile("fT0PsdMeanEvo","T0-Psd; time in run [s]; Mean time diff [ns]",
       4320, 0, 4320);
//       4320, 0, 259200);


  // T0 vs. STS for the different DPBs
  fT0StsDpbDiff = new TH2F("fT0StsDpbDiff","T0-Much;DPB; time diff [ns];Counts",
       2, -0.5, 1.5, nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);

  for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )
  {
    fT0StsDpbDiffEvo[uStsDpb] = new TH2F( Form("fT0StsDpbDiffEvo%02u", uStsDpb),
      Form( "T0-STS DPB %02u;TS; time diff [ns];Counts", uStsDpb),
       1800, 0, 180000,
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);
    fStsDpbCntsEvo[uStsDpb] = new TH1F( Form("fStsDpbCntsEvo%02u", uStsDpb),
      Form( "Time STS DPB %02u;TS; Hit Counts", uStsDpb),
       1800, 0, 180000);
  } // for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )

  // T0 vs. Much for the different DPBs/AFCK
  fT0MuchRocDiff = new TH2F("fT0MuchRocDiff","T0-Much;AFCK; time diff [ns];Counts",
       kuMaxNbMuchDpbs, -0.5, kuMaxNbMuchDpbs - 0.5, nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);

  // T0 vs. Much for the different ASICs
  fT0MuchAsicDiff = new TH2F("fT0MuchAsicDiff","T0-Much;ASIC; time diff [ns];Counts",
       kuMaxNbMuchAsics, -0.5, kuMaxNbMuchAsics - 0.5, nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);

  for( UInt_t uMuchAsic = 0; uMuchAsic < kuMaxNbMuchAsics; ++uMuchAsic )
    fT0MuchAsicDiffEvo[uMuchAsic] = new TH2F( Form("fT0MuchAsicDiffEvo%02u", uMuchAsic),
      Form( "T0-Much ASIC %02u;TS; time diff [ns];Counts", uMuchAsic),
       1800, 0, 180000,
			nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);

  // T0 vs. T0
  fT0T0Diff = new TH1F("fT0T0Diff","T0-T0_prev;time diff [ns];Counts",
		       iNbBinsLog - 1, dBinsLog);
  // sts vs. Sts
  fStsStsDiff = new TH1F("fStsStsDiff","Sts-Sts_prev;time diff [ns];Counts",
		       iNbBinsLog - 1, dBinsLog);
  // Much vs. Much
  fMuchMuchDiff = new TH1F("fMuchMuchDiff","Much-Much_prev;time diff [ns];Counts",
		       iNbBinsLog - 1, dBinsLog);
  // Tof vs. Tof
  fTofTofDiff = new TH1F("fTofTofDiff","Tof-Tof_prev;time diff [ns];Counts",
		       iNbBinsLog - 1, dBinsLog);
  // Rich vs. Rich
  fRichRichDiff = new TH1F("fRichRichDiff","Rich-Rich_prev;time diff [ns];Counts",
		       iNbBinsLog - 1, dBinsLog);
  // Psd vs. Psd
  fPsdPsdDiff = new TH1F("fPsdPsdDiff","Psd-Psd_prev;time diff [ns];Counts",
		       iNbBinsLog - 1, dBinsLog);

  fT0Address = new TH1F("fT0Address","T0 address;address;Counts",
                        1000000, 0, 1000000.);

  fT0Channel = new TH1F("fT0Channel","T0 channel;channel nr;Counts",
                        100, -0.5, 99.5);

  /// Register the histos in the HTTP server
  FairRunOnline* run = FairRunOnline::Instance();
  if (run) {
    THttpServer* server = run->GetHttpServer();
    if( nullptr != server ) {
      server->Register("/CheckTiming", fT0StsDiff);
      server->Register("/CheckTiming", fT0MuchDiff);
      server->Register("/CheckTiming", fT0TofDiff);
      server->Register("/CheckTiming", fT0RichDiff);
      server->Register("/CheckTiming", fT0PsdDiff);
      server->Register("/CheckTiming", fT0StsDiffEvo);
      server->Register("/CheckTiming", fT0MuchDiffEvo);
      server->Register("/CheckTiming", fT0TofDiffEvo);
      server->Register("/CheckTiming", fT0RichDiffEvo);
      server->Register("/CheckTiming", fT0PsdDiffEvo);
      server->Register("/CheckTiming", fT0StsDiffEvoLong);
      server->Register("/CheckTiming", fT0MuchDiffEvoLong);
      server->Register("/CheckTiming", fT0TofDiffEvoLong);
      server->Register("/CheckTiming", fT0RichDiffEvoLong);
      server->Register("/CheckTiming", fT0PsdDiffEvoLong);

      server->Register("/CheckTiming", fT0StsMeanEvo);
      server->Register("/CheckTiming", fT0MuchMeanEvo);
      server->Register("/CheckTiming", fT0TofMeanEvo);
      server->Register("/CheckTiming", fT0RichMeanEvo);
      server->Register("/CheckTiming", fT0PsdMeanEvo);

      server->Register("/CheckTiming", fT0T0Diff);
      server->Register("/CheckTiming", fStsStsDiff);
      server->Register("/CheckTiming", fMuchMuchDiff);
      server->Register("/CheckTiming", fTofTofDiff);
      server->Register("/CheckTiming", fRichRichDiff);
      server->Register("/CheckTiming", fPsdPsdDiff);

      server->Register("/CheckTiming", fT0StsDpbDiff);
      for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )
        server->Register("/CheckTiming/STS", fT0StsDpbDiffEvo[uStsDpb]);

      server->Register("/CheckTiming", fT0MuchRocDiff);
      server->Register("/CheckTiming", fT0MuchAsicDiff);
      for( UInt_t uMuchAsic = 0; uMuchAsic < kuMaxNbMuchAsics; ++uMuchAsic )
        server->Register("/CheckTiming/MUCH", fT0MuchAsicDiffEvo[uMuchAsic]);
    }
  }
/*
  fDigisPerAsicEvo = new TH2F( "fDigisPerAsicEvo",
      "Digis per Asic evo; Time [ ns ]; ASIC []",
       5 * 10240, 0, 5 * 10240000.,
			kuMaxNbMuchAsics, 0, kuMaxNbMuchAsics);
  for( UInt_t uAsic = 0; uAsic < kuMaxNbMuchAsics; ++uAsic )
    for( UInt_t uChan = 0; uChan < kuNbChanSMX; ++uChan )
      {
         fdLastMuchDigi[ uAsic ][ uChan ] = 0.0;
         fdLastMuchDigiPulser[ uAsic ][ uChan ] = 0.0;
      } // loop on channel and asic

  fSameChanDigisDistEvo = new TH2F( "fSameChanDigisDistEvo",
      "Time dist of digis in same chan evo; Time [ ns ]; Same chan dist [ ns ]",
       5000, 0, 500000.,
       1000, 0., 10000. );

  fDigiTimeEvoT0 = new TH2F( "fDigiTimeEvoT0",
      "Time of digi in T0 vs ts index; TS [ ]; Digi time [ ns ]",
       10000, 0., 30000.,
       10000, 0., 300.e9 );
  fDigiTimeEvoSts = new TH2F( "fDigiTimeEvoSts",
      "Time of digi in Sts vs ts index; TS [ ]; Digi time [ ns ]",
       10000, 0., 30000.,
       10000, 0., 300.e9 );
  fDigiTimeEvoMuch = new TH2F( "fDigiTimeEvoMuch",
      "Time of digi in Much vs ts index; TS [ ]; Digi time [ ns ]",
       10000, 0., 30000.,
       10000, 0., 300.e9 );
  fDigiTimeEvoTof = new TH2F( "fDigiTimeEvoTof",
      "Time of digi in Tof vs ts index; TS [ ]; Digi time [ ns ]",
       10000, 0., 30000.,
       10000, 0., 300.e9 );
*/
}
// ---- ReInit  -------------------------------------------------------
InitStatus CbmMcbm2019CheckPulser::ReInit()
{
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void CbmMcbm2019CheckPulser::Exec(Option_t* /*option*/)
{
  LOG(debug) << "executing TS " << fNrTs;

  if( 0 < fNrTs && 0 == fNrTs % 1000 )
    LOG(info) << Form( "Processing TS %6d", fNrTs );

/*
  /// Zoom in on Much jump in run 401
  if( fNrTs < 17600 || 17650 < fNrTs )
//  if( fNrTs < 17634 || 17637 < fNrTs )
    return;
*/

  CheckInterSystemOffset();
/*
  Int_t nrT0Digis=fT0Digis->GetEntriesFast();
  for (Int_t iT0 = 0; iT0 < nrT0Digis; ++iT0)
  {
      CbmDigi* T0Digi = static_cast<CbmDigi*>(fT0Digis->At(iT0));

      Double_t T0Time = T0Digi->GetTime();
      fDigiTimeEvoT0->Fill( fNrTs, T0Time );
  }

  Int_t nrStsDigis{0};
  if (fStsDigis) {
    nrStsDigis=fStsDigis->GetEntriesFast();
    LOG(debug) << "StsDigis: " << nrStsDigis;
    for (Int_t iMuch = 0; iMuch < nrStsDigis; ++iMuch) {

      CbmDigi* Digi = static_cast<CbmDigi*>(fStsDigis->At(iMuch));

      Double_t dTime = Digi->GetTime();
      fDigiTimeEvoSts->Fill( fNrTs, dTime );
    }
  }

  Int_t nrMuchDigis{0};
  if (fMuchDigis) {
    nrMuchDigis=fMuchDigis->GetEntriesFast();
    LOG(debug) << "MuchDigis: " << nrMuchDigis;
    for (Int_t iMuch = 0; iMuch < nrMuchDigis; ++iMuch) {

      CbmDigi* Digi = static_cast<CbmDigi*>(fMuchDigis->At(iMuch));

      Double_t dTime = Digi->GetTime();
      fDigiTimeEvoMuch->Fill( fNrTs, dTime );
    }
  }

  Int_t nrTofDigis{0};
  if (fTofDigis) {
    nrTofDigis=fTofDigis->GetEntriesFast();
    LOG(debug) << "TofDigis: " << nrTofDigis;
    for (Int_t iMuch = 0; iMuch < nrTofDigis; ++iMuch) {

      CbmDigi* Digi = static_cast<CbmDigi*>(fTofDigis->At(iMuch));

      Double_t dTime = Digi->GetTime();
      fDigiTimeEvoTof->Fill( fNrTs, dTime );
    }
  }
*/
  fNrTs++;

  if( 0 < fNrTs && 0 == fNrTs % 90000 )
    WriteHistos();
}

void CbmMcbm2019CheckPulser::CheckInterSystemOffset()
{
  LOG(debug) <<"Begin";
  Int_t nrT0Digis = 0;
  if ( fT0DigiVector ) nrT0Digis = fT0DigiVector->size();
  else if ( fT0DigiArray ) nrT0Digis = fT0DigiArray->GetEntriesFast();
  LOG(debug) << "T0Digis: " << nrT0Digis;

  Int_t nrStsDigis = fDigiMan->GetNofDigis(ECbmModuleId::kSts);
  Int_t nrMuchDigis = fDigiMan->GetNofDigis(ECbmModuleId::kMuch);
  Int_t nrTofDigis = fDigiMan->GetNofDigis(ECbmModuleId::kTof);
  Int_t nrRichDigis = fDigiMan->GetNofDigis(ECbmModuleId::kRich);
  Int_t nrPsdDigis = fDigiMan->GetNofDigis(ECbmModuleId::kPsd);

//  if (nrT0Digis < 100000) {
  if (nrT0Digis < 1000000) {
    /// Re-initialize array references
    fPrevT0FirstDigiSts = 0.;
    fPrevT0FirstDigiMuch = 0.;
    fPrevT0FirstDigiTof = 0.;
    fPrevT0FirstDigiRich = 0.;
    fPrevT0FirstDigiPsd = 0.;

    for (Int_t iT0 = 0; iT0 < nrT0Digis; ++iT0) {

      if (iT0%1000 == 0) LOG(debug) << "Executing entry " << iT0;

      const CbmTofDigi* T0Digi = nullptr;
      if ( fT0DigiVector) T0Digi = &(fT0DigiVector->at(iT0));
      else if ( fT0DigiArray ) T0Digi = dynamic_cast<CbmTofDigi*>(fT0DigiArray->At(iT0));
      assert(T0Digi);

      /// Keep only pulser Digis
      if( fuMaxTotPulserT0 < T0Digi->GetCharge() || T0Digi->GetCharge() < fuMinTotPulserT0 )
        continue;

      Double_t T0Time = T0Digi->GetTime();
      Int_t T0Address = T0Digi->GetAddress();

      /// Keep only pulser Digis
      if( 0x00005006 != T0Address && 0x04005006 != T0Address )
        continue;

      fT0Address->Fill(T0Address);
/*
      std::cout << Form( "T0 pulser in TS %5d: address 0x%08X T0 time %12.0f dt %12.0f",
                         fNrTs, T0Address, T0Time, T0Time - fdLastT0DigiPulser )
                << std::endl;
*/
      fdLastT0DigiPulser = T0Time;

      fT0Channel->Fill(T0Digi->GetChannel());

      if (nrStsDigis > 0 && nrStsDigis < 1000000 && fuMinAdcPulserSts < fuMaxAdcPulserSts )
        fPrevT0FirstDigiSts = FillSystemOffsetHistos<CbmStsDigi>(fT0StsDiff, fT0StsDiffEvo,
                                                      fT0StsDiffEvoLong, fT0StsMeanEvo, fT0StsDpbDiff,
                                                      T0Time, fStsOffsetRange, fPrevT0FirstDigiSts,
                                                      ECbmModuleId::kSts);
      if (nrMuchDigis > 0 && nrMuchDigis < 1000000 && fuMinAdcPulserMuch < fuMaxAdcPulserMuch )
        fPrevT0FirstDigiMuch = FillSystemOffsetHistos<CbmMuchBeamTimeDigi>(fT0MuchDiff, fT0MuchDiffEvo,
                                                      fT0MuchDiffEvoLong, fT0MuchMeanEvo, fT0MuchRocDiff,
                                                      T0Time, fMuchOffsetRange, fPrevT0FirstDigiMuch,
                                                      ECbmModuleId::kMuch);
      if (nrTofDigis > 0 && nrTofDigis < 1000000 && fuMinTotPulserTof < fuMaxTotPulserTof )
        fPrevT0FirstDigiTof = FillSystemOffsetHistos<CbmTofDigi>(fT0TofDiff, fT0TofDiffEvo,
                                                      fT0TofDiffEvoLong, fT0TofMeanEvo, nullptr,
                                                      T0Time, fTofOffsetRange, fPrevT0FirstDigiTof,
                                                      ECbmModuleId::kTof);
      if (nrRichDigis > 0 && nrRichDigis < 1000000 && fuMinTotPulserRich < fuMaxTotPulserRich )
        fPrevT0FirstDigiRich = FillSystemOffsetHistos<CbmRichDigi>(fT0RichDiff, fT0RichDiffEvo,
                                                      fT0RichDiffEvoLong, fT0RichMeanEvo, nullptr,
                                                      T0Time, fRichOffsetRange, fPrevT0FirstDigiRich,
                                                      ECbmModuleId::kRich);
      if (nrPsdDigis > 0 && nrPsdDigis < 1000000 && fuMinAdcPulserPsd < fuMaxAdcPulserPsd )
        fPrevT0FirstDigiPsd = FillSystemOffsetHistos<CbmPsdDigi>(fT0PsdDiff, fT0PsdDiffEvo,
                                                      fT0PsdDiffEvoLong, fT0PsdMeanEvo, nullptr,
                                                      T0Time, fPsdOffsetRange, fPrevT0FirstDigiPsd,
                                                      ECbmModuleId::kPsd);
    }
  }
/*
  for (Int_t iMuch = 0; iMuch < nrMuchDigis; ++iMuch) {

    CbmMuchBeamTimeDigi* Digi = static_cast<CbmMuchBeamTimeDigi*>(fMuchDigis->At(iMuch));

    Double_t dTime = Digi->GetTime();
    Double_t dAdc  = Digi->GetCharge();
    UInt_t uAsic = Digi->GetNxId();
    UInt_t uChan = Digi->GetNxCh();

    Double_t dStartJump = (17633. + 1969.) * 10240000.;
    Double_t dTimeSinceStart = dTime - dStartJump;
    fDigisPerAsicEvo->Fill( dTimeSinceStart, uAsic );

    if( 19132000. < dTimeSinceStart && dTimeSinceStart < 19600000 )
    {
//      std::cout << Form( "Much hit in TS %5d: asic %2u chan %3u T0 time %12.0f ADC %2.0f",
//                            fNrTs, uAsic, uChan, (dTimeSinceStart - 19132000), dAdc )
//              << std::endl;
      Double_t dTimeDistLastDigi = dTimeSinceStart - fdLastMuchDigi[ uAsic ][ uChan ];
      fSameChanDigisDistEvo->Fill( dTimeSinceStart - 19132000,
                                   dTimeDistLastDigi < 10000 ? dTimeDistLastDigi : 9999 );
    }
    fdLastMuchDigi[ uAsic ][ uChan ] = dTimeSinceStart;

    if( ( kuMaxNbMuchAsics == fuMuchAsic && uAsic == fuMuchAsic ) &&
        ( kuNbChanSMX == fuMuchFirstCha || uChan >= fuMuchFirstCha ) &&
        ( kuNbChanSMX == fuMuchLastChan || fuMuchLastChan <= uChan )
      )
      continue;
    if( fuMaxAdcPulserMuch < Digi->GetCharge() || Digi->GetCharge() < fuMinAdcPulserMuch )
      continue;

        if( 32 != uChan )
          continue;
    std::cout << Form( "Much pulser in TS %5d: chan %3u T0 time %12.0f ADC %2.0f dt %12.0f",
                       fNrTs, uChan, dTime, dAdc, dTime - fdLastMuchDigiPulser[ uAsic ][ uChan ] )
              << std::endl;
    fdLastMuchDigiPulser[ uAsic ][ uChan ] = dTime;
  }
*/
}

template <class Digi>
Int_t CbmMcbm2019CheckPulser::FillSystemOffsetHistos(TH1* histo,
                                             TH2* histoEvo, TH2* histoEvoLong, TProfile * profMeanEvo,
                                             TH2* histoAFCK,
                                             const Double_t T0Time, const Int_t offsetRange,
                                             Int_t iStartDigi,
                                             ECbmModuleId iDetId )
{

  Int_t nrDigis = fDigiMan->GetNofDigis(iDetId);
  Int_t iFirstDigiInWin = iStartDigi;

  for (Int_t i = iStartDigi; i < nrDigis; ++i) {

    const Digi* digi = fDigiMan->Get<Digi>(i);

    switch( iDetId )
    {
      case ECbmModuleId::kSts: ///< Silicon Tracking System
      {
        if( fuMaxAdcPulserSts < digi->GetCharge() || digi->GetCharge() < fuMinAdcPulserSts )
          continue;
        break;
      } // case ECbmModuleId::kSts:
      case ECbmModuleId::kMuch:        ///< Muon detection system
      {
        const CbmMuchBeamTimeDigi* muchDigi{nullptr};
        try {
          muchDigi =
              boost::any_cast<const CbmMuchBeamTimeDigi*>( digi );
        } catch( ... ) {
            LOG( fatal ) << "Failed boost any_cast in CbmMcbm2019CheckPulser::FillSystemOffsetHistos for a digi of type "
                         << Digi::GetClassName();
        } // try/catch
        assert(muchDigi);
        UInt_t uAsic = muchDigi->GetNxId();
        UInt_t uChan = muchDigi->GetNxCh();

        if( ( kuMaxNbMuchAsics != fuMuchAsic && uAsic != fuMuchAsic ) ||
            ( kuNbChanSMX != fuMuchFirstCha && uChan < fuMuchFirstCha ) ||
            ( kuNbChanSMX != fuMuchLastChan && fuMuchLastChan < uChan )
          )
          continue;

        if( fuMaxAdcPulserMuch < digi->GetCharge() || digi->GetCharge() < fuMinAdcPulserMuch )
          continue;
        break;


      } // case ECbmModuleId::kMuch:
      case ECbmModuleId::kTof:         ///< Time-of-flight Detector
      {
        if( fuMaxTotPulserTof < digi->GetCharge() || digi->GetCharge() < fuMinTotPulserTof )
          continue;
        break;
      } // case ECbmModuleId::kTof:
      case ECbmModuleId::kRich:        ///< Ring-Imaging Cherenkov Detector
      {
        if( fuMaxTotPulserRich < digi->GetCharge() || digi->GetCharge() < fuMinTotPulserRich )
          continue;
        break;
      } // case ECbmModuleId::kRich:
      case ECbmModuleId::kPsd:         ///< Projectile spectator detector
      {
        if( fuMaxAdcPulserPsd < digi->GetCharge() || digi->GetCharge() < fuMinAdcPulserPsd )
          continue;
        break;
      } // case ECbmModuleId::kPsd:
      default:
        return 0;
    } // switch( iDetId )

    Double_t diffTime = T0Time - digi->GetTime();
//    profMeanEvo->Fill( T0Time * 1e-9 - fdStartTime, diffTime );

    if (diffTime > offsetRange) {
      ++ iFirstDigiInWin; // Update Index of first digi in Win to next digi
      continue; // not yet in interesting range
    } // if (diffTime > offsetRange)
    if (diffTime < -offsetRange) break;     // already past interesting range

    histo->Fill(diffTime);
    histoEvo->Fill(fNrTs, diffTime);
    histoEvoLong->Fill(fNrTs, diffTime);

    if( -1 == fdStartTime )
      fdStartTime = T0Time * 1e-9;
    profMeanEvo->Fill( T0Time * 1e-9 - fdStartTime, diffTime );
/*
    if (ECbmModuleId::kMuch == iDetId )
      std::cout << Form( "MUCH coinc in TS %5d: %7.2f T0 time %12.0f", fNrTs, diffTime, T0Time )
                << std::endl;
*/
    if( ECbmModuleId::kPsd == iDetId )
      fT0PsdDiffCharge->Fill( diffTime, digi->GetCharge() );

    /// STS DPB mapping: ladder 1 is in DPB 1
    if (ECbmModuleId::kSts == iDetId && histoAFCK) {
      UInt_t uDPB = ( 0 < (digi->GetAddress() & 0x00000400 ) );
      histoAFCK->Fill(uDPB, diffTime);
      if( uDPB < kuMaxNbStsDpbs )
        fT0StsDpbDiffEvo[ uDPB ]->Fill(fNrTs, diffTime);
    } // if (ECbmModuleId::kSts == iDetId && histoAFCK)

    /// MUCH DPB mapping
    if (ECbmModuleId::kMuch == iDetId && histoAFCK) {
      const CbmMuchBeamTimeDigi* muchDigi{nullptr};
      try {
        muchDigi =
            boost::any_cast<const CbmMuchBeamTimeDigi*>( digi );
      } catch( ... ) {
          LOG( fatal ) << "Failed boost any_cast in CbmCheckTiming::FillSystemOffsetHistos for a digi of type "
                       << Digi::GetClassName();
      } // try/catch
      assert(muchDigi);
      UInt_t afck = muchDigi->GetRocId();
      UInt_t asic = muchDigi->GetNxId();
      histoAFCK->Fill(afck, diffTime);
      fT0MuchAsicDiff->Fill(asic, diffTime);
      if( asic < kuMaxNbMuchAsics )
        fT0MuchAsicDiffEvo[ asic ]->Fill(fNrTs, diffTime);
    } // if (ECbmModuleId::kMuch == iDetId && histoAFCK)
  }

  return iFirstDigiInWin;
}

// ---- Finish --------------------------------------------------------
void CbmMcbm2019CheckPulser::Finish()
{
  WriteHistos();

}

void CbmMcbm2019CheckPulser::WriteHistos()
{
  TFile* old = gFile;
  TFile* outfile = TFile::Open(fOutFileName,"RECREATE");

  fT0StsDiff->Write();
  fT0MuchDiff->Write();
  fT0TofDiff->Write();
  fT0RichDiff->Write();
  fT0PsdDiff->Write();
  fT0PsdDiffCharge->Write();

  fT0StsDiffEvo->Write();
  fT0MuchDiffEvo->Write();
  fT0TofDiffEvo->Write();
  fT0RichDiffEvo->Write();
  fT0PsdDiffEvo->Write();

  fT0StsDiffEvoLong->Write();
  fT0MuchDiffEvoLong->Write();
  fT0TofDiffEvoLong->Write();
  fT0RichDiffEvoLong->Write();
  fT0PsdDiffEvoLong->Write();

  fT0StsMeanEvo->Write();
  fT0MuchMeanEvo->Write();
  fT0TofMeanEvo->Write();
  fT0RichMeanEvo->Write();
  fT0PsdMeanEvo->Write();

  fT0T0Diff->Write();
  fStsStsDiff->Write();
  fMuchMuchDiff->Write();
  fTofTofDiff->Write();
  fRichRichDiff->Write();
  fPsdPsdDiff->Write();

  fT0Address->Write();
  fT0Channel->Write();

  fT0StsDpbDiff->Write();
/*
  for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )
  {
    fT0StsDpbDiffEvo[uStsDpb]->Write();
    fStsDpbCntsEvo[uStsDpb]->Write();
  }
*/
  fT0MuchRocDiff->Write();
  fT0MuchAsicDiff->Write();
/*
  for( UInt_t uMuchAsic = 0; uMuchAsic < kuMaxNbMuchAsics; ++uMuchAsic )
    fT0MuchAsicDiffEvo[uMuchAsic]->Write();
*/
/*
  fDigisPerAsicEvo->Write();
  fSameChanDigisDistEvo->Write();

     fDigiTimeEvoT0 ->Write();
     fDigiTimeEvoSts ->Write();
     fDigiTimeEvoMuch->Write();
     fDigiTimeEvoTof->Write();
*/
  outfile->Close();
  delete outfile;

  gFile = old;
}

ClassImp(CbmMcbm2019CheckPulser)
