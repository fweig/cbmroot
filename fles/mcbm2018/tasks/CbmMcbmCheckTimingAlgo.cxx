/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmMcbmCheckTimingAlgo.h"

#include "CbmDigiManager.h"
#include "CbmFlesHistosTools.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

#include "FairRootManager.h"
#include "FairRunOnline.h"
#include <Logger.h>

#include "TF1.h"
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
CbmMcbmCheckTimingAlgo::CbmMcbmCheckTimingAlgo() {}

// ---- Destructor ----------------------------------------------------
CbmMcbmCheckTimingAlgo::~CbmMcbmCheckTimingAlgo() {}

// ----  Initialisation  ----------------------------------------------
void CbmMcbmCheckTimingAlgo::SetParContainers()
{
  // Load all necessary parameter containers from the runtime data base
  /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <CbmMcbmCheckTimingAlgoDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
Bool_t CbmMcbmCheckTimingAlgo::Init()
{
  /// Check if all required data input storage are present
  /// Reference detector
  CheckDataPresence(fRefDet);
  /// Checked detectors
  for (std::vector<CheckTimingDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    CheckDataPresence(*det);
  }  // for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  CreateHistos();

  return kTRUE;
}

void CbmMcbmCheckTimingAlgo::CheckDataPresence(CheckTimingDetector detToCheck)
{
  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();
  fDigiMan               = CbmDigiManager::Instance();
  fDigiMan->UseMuchBeamTimeDigi();
  fDigiMan->Init();

  /// Handle special case for T0 as not yet supported in DigiManager
  if (ECbmModuleId::kT0 == detToCheck.detId) {
    // Get a pointer to the previous already existing data level
    fpT0DigiVec = ioman->InitObjectAs<std::vector<CbmTofDigi> const*>("T0Digi");
    if (!fpT0DigiVec) {
      LOG(fatal) << "No storage with T0 digis found while it should be used. "
                    "Stopping there!";
    }  // if ( ! fpT0DigiVec )
  }    // if( ECbmModuleId::kT0 == detToCheck.detId )
  /// Handle special case for TRD-2D as not yet supported in DigiManager
  if (ECbmModuleId::kTrd2d == detToCheck.detId) {
    // Get a pointer to the previous already existing data level
    fpTrd2dDigiVec = ioman->InitObjectAs<std::vector<CbmTrdDigi> const*>("TrdFaspDigi");
    if (!fpTrd2dDigiVec) {
      LOG(fatal) << "No storage with TRD-2D digis found while it should be used. "
                    "Stopping there!";
    }  // if ( ! fpTrd2dDigiVec )
  }    // if( ECbmModuleId::kTrd2d == detToCheck.detId )
  else if (!fDigiMan->IsPresent(detToCheck.detId)) {
    LOG(fatal) << "No " << detToCheck.sName << " digis found while it should be used. Stopping there!";
  }  // else if ( ! fDigiMan->IsPresent( detToCheck.detId ) ) of if( ECbmModuleId::kT0 == detToCheck.detId )
}

void CbmMcbmCheckTimingAlgo::CreateHistos()
{
  /// FIXME: Disable clang formatting for histograms declaration for now
  /* clang-format off */

   /// Logarithmic bining for self time comparison
  uint32_t iNbBinsLog = 0;
    /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
  std::vector<double> dBinsLogVector = GenerateLogBinArray( 9, 9, 1, iNbBinsLog );
  double* dBinsLog = dBinsLogVector.data();

  for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  {
    fvhDetSelfDiff.push_back( new TH1D( Form( "h%sSelfDiff", (*det).sName.data() ),
                                        Form( "time difference between consecutivs %s  Digis;time diff [ns];Counts",
                                              (*det).sName.data() ),
                                        iNbBinsLog, dBinsLog )
                            );

    fvhDetToRefDiff.push_back( new TH1D( Form( "h%s%sDiff", (*det).sName.data(), fRefDet.sName.data() ),
                                        Form( "%s - %s time difference;time diff [ns];Counts",
                                              (*det).sName.data(), fRefDet.sName.data() ),
                                        (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd )
                             );

    fvhDetToRefDiffRefCharge.push_back( new TH2F( Form( "h%s%sDiffRefCharge", (*det).sName.data(), fRefDet.sName.data() ),
                                                  Form( "%s - %s;time diff [ns]; %s Charge [a.u]; Counts",
                                                        (*det).sName.data(), fRefDet.sName.data(), fRefDet.sName.data() ),
                                                  (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd,
                                                  256, 0, 256 )
                                      );
    fvhDetToRefDiffDetCharge.push_back( new TH2F( Form( "h%s%sDiffDetCharge", (*det).sName.data(), fRefDet.sName.data() ),
                                                  Form( "%s - %s;time diff [ns]; %s Charge [a.u]; Counts",
                                                        (*det).sName.data(), fRefDet.sName.data(), (*det).sName.data() ),
                                                  (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd,
                                                  256, 0, 256 )
                                      );
    fvhDetToRefDiffEvo.push_back( new TH2F( Form( "h%s%sDiffEvo", (*det).sName.data(), fRefDet.sName.data() ),
                                            Form( "%s - %s;TS; time diff [ns];Counts",
                                                  (*det).sName.data(), fRefDet.sName.data() ),
                                            1000, 0, 10000,
                                            (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd )
                                );
    fvhDetToRefDiffEvoLong.push_back( new TH2F( Form( "h%s%sDiffEvoLong", (*det).sName.data(), fRefDet.sName.data() ),
                                                Form( "%s - %s;TS; time diff [ns];Counts",
                                                      (*det).sName.data(), fRefDet.sName.data() ),
                                                1800, 0, 18000,
                                                (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd )
                                    );
    LOG( info ) << "Created histos for " << (*det).sName;
  } // for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  /// Add reference detector digi to digi time difference histo at end of vector
  fvhDetSelfDiff.push_back( new TH1D( Form( "h%sSelfDiff", fRefDet.sName.data() ),
                                      Form( "time difference between consecutivs %s  Digis;time diff [ns];Counts",
                                            fRefDet.sName.data() ),
                                      iNbBinsLog, dBinsLog )
                          );

  /// Register the histos in the HTTP server
  FairRunOnline* run = FairRunOnline::Instance();
  if( run )
  {
    THttpServer* server = run->GetHttpServer();
    if( nullptr != server )
    {
      /// Register histos for all checked detectors
      for( UInt_t uDetIdx = 0; uDetIdx < fvDets.size(); ++uDetIdx )
      {
        server->Register("/CheckTiming/SelfDiff", fvhDetSelfDiff[ uDetIdx ] );
        server->Register("/CheckTiming/RefDiff", fvhDetToRefDiff[ uDetIdx ] );
        server->Register("/CheckTiming/DiffCharge", fvhDetToRefDiffRefCharge[ uDetIdx ] );
        server->Register("/CheckTiming/DiffCharge", fvhDetToRefDiffDetCharge[ uDetIdx ] );
        server->Register("/CheckTiming/DiffEvo", fvhDetToRefDiffEvo[ uDetIdx ] );
        server->Register("/CheckTiming/DiffEvo", fvhDetToRefDiffEvoLong[ uDetIdx ] );
      } // for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

      /// Register the histo for reference detector digi to digi time difference
      server->Register("/CheckTiming/SelfDiff", fvhDetSelfDiff[ fvDets.size() ] );
    } // if( nullptr != server )
  } // if( run )

  /// FIXME: Re-enable clang formatting after histograms declaration
  /* clang-format on */
}
// ---- ReInit  -------------------------------------------------------
Bool_t CbmMcbmCheckTimingAlgo::ReInit() { return kTRUE; }

// ---- Exec ----------------------------------------------------------
void CbmMcbmCheckTimingAlgo::ProcessTs()
{
  LOG(debug) << "executing TS " << fuNbTs;

  switch (fRefDet.detId) {
    case ECbmModuleId::kSts: {
      CheckInterSystemOffset<CbmStsDigi>();
      break;
    }  // case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch: {
      CheckInterSystemOffset<CbmMuchBeamTimeDigi>();
      break;
    }  // case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd: {
      CheckInterSystemOffset<CbmTrdDigi>();
      break;
    }  // case ECbmModuleId::kTrd:
    case ECbmModuleId::kTrd2d: {
      CheckInterSystemOffset<CbmTrdDigi>();
      break;
    }  // case ECbmModuleId::kTrd2d:
    case ECbmModuleId::kTof: {
      CheckInterSystemOffset<CbmTofDigi>();
      break;
    }  // case ECbmModuleId::kTof:
    case ECbmModuleId::kRich: {
      CheckInterSystemOffset<CbmRichDigi>();
      break;
    }  // case ECbmModuleId::kRich:
    case ECbmModuleId::kPsd: {
      CheckInterSystemOffset<CbmPsdDigi>();
      break;
    }  // case ECbmModuleId::kPsd:
    case ECbmModuleId::kT0: {
      CheckInterSystemOffset<CbmTofDigi>();
      break;
    }  // case ECbmModuleId::kT0:
    default: {
      LOG(fatal) << "CbmMcbm2019TimeWinEventBuilderAlgo::LoopOnSeeds => "
                 << "Trying to search matches with unsupported det: " << fRefDet.sName;
      break;
    }  // default:
  }    // switch( fRefDet )

  fuNbTs++;
}

template<class DigiRef>
void CbmMcbmCheckTimingAlgo::CheckInterSystemOffset()
{
  UInt_t uNbRefDigis = 0;
  switch (fRefDet.detId) {
    case ECbmModuleId::kNotExist: {
      LOG(fatal) << "CbmMcbmCheckTimingAlgo::Exec => Unknow reference detector enum! " << fRefDet.sName;
      break;
    }  // Digi containers controlled by DigiManager
    case ECbmModuleId::kT0: {
      uNbRefDigis = fpT0DigiVec->size();
      break;
    }  // case ECbmModuleId::kT0
    case ECbmModuleId::kTrd2d: {
      uNbRefDigis = fpTrd2dDigiVec->size();
      break;
    }  // case ECbmModuleId::kTrd2d
    default: {
      uNbRefDigis = fDigiMan->GetNofDigis(fRefDet.detId);
      break;
    }  // default:
  }    // switch( fRefDet.detId )

  /// Re-initialize array references
  for (std::vector<CheckTimingDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    (*det).iPrevRefFirstDigi = 0;
  }  // for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  for (UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi) {
    //LOG(debug) << Form("Checking seed %6u / %6u", uDigi, uNbRefDigis);

    Double_t dRefTime   = 0;
    Double_t dRefCharge = 0;
    UInt_t uRefAddress  = 0;
    if (ECbmModuleId::kT0 == fRefDet.detId) {
      dRefTime   = fpT0DigiVec->at(uDigi).GetTime();
      dRefCharge = fpT0DigiVec->at(uDigi).GetCharge();
    }  // if( ECbmModuleId::kT0 == fRefDet.detId )
    else if (ECbmModuleId::kTrd2d == fRefDet.detId) {
      dRefTime = fpTrd2dDigiVec->at(uDigi).GetTime();
      //dRefCharge = fpTrd2dDigiVec->at(uDigi).GetCharge();
      dRefCharge = 1;
    }  // else if( ECbmModuleId::kTrd2d == fRefDet.detId )
    else {
      dRefTime   = fDigiMan->Get<DigiRef>(uDigi)->GetTime();
      dRefCharge = fDigiMan->Get<DigiRef>(uDigi)->GetCharge();
      uRefAddress = fDigiMan->Get<DigiRef>(uDigi)->GetAddress();
    }  // else of if( ECbmModuleId::kT0 == fRefDet.detId )

    /// Fill self time difference histo
    fvhDetSelfDiff[fvDets.size()]->Fill(dRefTime - fRefDet.dPrevTime);
    fRefDet.dPrevTime = dRefTime;

    /// Charge cut if defined!
    if (fRefDet.uChargeCutMin != fRefDet.uChargeCutMax) {
      if (fRefDet.uChargeCutMin < fRefDet.uChargeCutMax) {
        /// Cut charges between Min and Max to reject pulser
        if (fRefDet.uChargeCutMin < dRefCharge && dRefCharge < fRefDet.uChargeCutMax) {
          continue;
        }  // if( fRefDet.uChargeCutMin < dRefCharge && dRefCharge < fRefDet.uChargeCutMax )
      }    // if( fRefDet.uChargeCutMin < fRefDet.uChargeCutMax )
      else {
        /// Select charges between Max and Min to select pulser (Min and Max swapped!!)
        if (fRefDet.uChargeCutMin < dRefCharge || dRefCharge < fRefDet.uChargeCutMax) {
          continue;
        }  // if( fRefDet.uChargeCutMin < dRefCharge || dRefCharge < fRefDet.uChargeCutMax )
        /// Psd Pulser selection
        if (ECbmModuleId::kPsd == fRefDet.detId && CbmPsdAddress::GetSectionId(uRefAddress) != 10) continue;
      }    // else of if( fRefDet.uChargeCutMin < fRefDet.uChargeCutMax )
    }      // if( fRefDet.uChargeCutMin =! fRefDet.uChargeCutMax )

    /// Fill time difference for each check detector defined in list
    for (UInt_t uDetIdx = 0; uDetIdx < fvDets.size(); ++uDetIdx) {
      switch (fvDets[uDetIdx].detId) {
        case ECbmModuleId::kSts: {
          FillTimeOffsetHistos<CbmStsDigi>(dRefTime, dRefCharge, uDetIdx);
          break;
        }  // case ECbmModuleId::kSts:
        case ECbmModuleId::kMuch: {
          FillTimeOffsetHistos<CbmMuchBeamTimeDigi>(dRefTime, dRefCharge, uDetIdx);
          break;
        }  // case ECbmModuleId::kMuch:
        case ECbmModuleId::kTrd: {
          FillTimeOffsetHistos<CbmTrdDigi>(dRefTime, dRefCharge, uDetIdx);
          break;
        }  // case ECbmModuleId::kTrd:
        case ECbmModuleId::kTrd2d: {
          FillTimeOffsetHistos<CbmTrdDigi>(dRefTime, dRefCharge, uDetIdx);
          break;
        }  // case ECbmModuleId::kTrd:
        case ECbmModuleId::kTof: {
          FillTimeOffsetHistos<CbmTofDigi>(dRefTime, dRefCharge, uDetIdx);
          break;
        }  // case ECbmModuleId::kTof:
        case ECbmModuleId::kRich: {
          FillTimeOffsetHistos<CbmRichDigi>(dRefTime, dRefCharge, uDetIdx);
          break;
        }  // case ECbmModuleId::kRich:
        case ECbmModuleId::kPsd: {
          FillTimeOffsetHistos<CbmPsdDigi>(dRefTime, dRefCharge, uDetIdx);
          break;
        }  // case ECbmModuleId::kPsd:
        case ECbmModuleId::kT0: {
          FillTimeOffsetHistos<CbmTofDigi>(dRefTime, dRefCharge, uDetIdx);
          break;
        }  // case ECbmModuleId::kT0:
        default: {
          LOG(fatal) << "CbmMcbmCheckTimingAlgo::CheckInterSystemOffset => "
                     << "Trying to search matches with unsupported det: " << fvDets[uDetIdx].sName;
          break;
        }  // default:
      }    // switch( fvDets[ uDetIdx ].detId )
    }      // for( UInt_t uDetIdx = 0; uDetIdx < fvDets.size(); ++uDetIdx )
  }        // for( UInt_t uDigi = 0; uDigi < uNbRefDigis; ++uDigi )
}

template<class Digi>
void CbmMcbmCheckTimingAlgo::FillTimeOffsetHistos(const Double_t dRefTime, const Double_t dRefCharge, UInt_t uDetIdx)
{
  UInt_t uNbDigis = 0;
  switch (fvDets[uDetIdx].detId) {
    case ECbmModuleId::kNotExist: {
      LOG(fatal) << "CbmMcbmCheckTimingAlgo::FillTimeOffsetHistos => Unknow "
                    "detector enum! "
                 << fRefDet.sName;
      break;
    }  // Digi containers controlled by DigiManager
    case ECbmModuleId::kT0: {
      uNbDigis = fpT0DigiVec->size();
      break;
    }  // case ECbmModuleId::kT0
    case ECbmModuleId::kTrd2d: {
      uNbDigis = fpTrd2dDigiVec->size();
      break;
    }  // case ECbmModuleId::kTrd2d
    default: {
      uNbDigis = fDigiMan->GetNofDigis(fvDets[uDetIdx].detId);
      break;
    }  // default:
  }    // switch( fRefDet.detId )

  UInt_t uFirstDigiInWin = fvDets[uDetIdx].iPrevRefFirstDigi;

  for (UInt_t uDigiIdx = fvDets[uDetIdx].iPrevRefFirstDigi; uDigiIdx < uNbDigis; ++uDigiIdx) {
    Double_t dTime   = 0;
    Double_t dCharge = 0;
    UInt_t uAddress  = 0;
    if (ECbmModuleId::kT0 == fvDets[uDetIdx].detId) {
      dTime   = fpT0DigiVec->at(uDigiIdx).GetTime();
      dCharge = fpT0DigiVec->at(uDigiIdx).GetCharge();
    }  // if( ECbmModuleId::kT0 == fRefDet.detId )
    else if (ECbmModuleId::kTrd2d == fvDets[uDetIdx].detId) {
      dTime = fpTrd2dDigiVec->at(uDigiIdx).GetTime();
      // dCharge = fpTrd2dDigiVec->at(uDigiIdx).GetCharge();
      dCharge = 1;
    }  // else if (ECbmModuleId::kTrd2d == fvDets[uDetIdx].detId)
    else {
      dTime   = fDigiMan->Get<Digi>(uDigiIdx)->GetTime();
      dCharge = fDigiMan->Get<Digi>(uDigiIdx)->GetCharge();
      uAddress = fDigiMan->Get<Digi>(uDigiIdx)->GetAddress();
    }  // else of if( ECbmModuleId::kT0 == fRefDet.detId )

    /// Fill self correlation histo while avoiding double counting due to
    /// the "smart looping"
    if (fvDets[uDetIdx].dPrevTime <= dTime) {
      fvhDetSelfDiff[uDetIdx]->Fill(dTime - fvDets[uDetIdx].dPrevTime);
      fvDets[uDetIdx].dPrevTime = dTime;
    }  // if( fvDets[ uDetIdx ].dPrevTime < dTime )

    Double_t dDiffTime = dTime - dRefTime;

    if (dDiffTime < fvDets[uDetIdx].dTimeRangeBeg) {
      ++uFirstDigiInWin;  // Update Index of first digi in Win to next digi
      continue;           // not yet in interesting range
    }                     // if (diffTime > offsetRange)
    if (fvDets[uDetIdx].dTimeRangeEnd < dDiffTime) {
      /// already past interesting range
      break;
    }  // if( fvDets[ uDetIdx ].dTimeRangeEnd < dDiffTime )

    /// Charge cut if defined!
    if (fvDets[uDetIdx].uChargeCutMin != fvDets[uDetIdx].uChargeCutMax) {
      if (fvDets[uDetIdx].uChargeCutMin < fvDets[uDetIdx].uChargeCutMax) {
        /// Cut charges between Min and Max to reject pulser
        if (fvDets[uDetIdx].uChargeCutMin < dCharge && dCharge < fvDets[uDetIdx].uChargeCutMax) {
          continue;
        }  // if( fvDets[ uDetIdx ].uChargeCutMin < dCharge && dCharge < fvDets[ uDetIdx ].uChargeCutMax )
      }    // if( fvDets[ uDetIdx ].uChargeCutMin < fvDets[ uDetIdx ].uChargeCutMax )
      else {
        /// Select charges between Max and Min to select pulser (Min and Max swapped!!)
        if (fvDets[uDetIdx].uChargeCutMin < dCharge || dCharge < fvDets[uDetIdx].uChargeCutMax) {
          continue;
        }  // if( fvDets[ uDetIdx ].uChargeCutMin < dCharge || dCharge < fvDets[ uDetIdx ].uChargeCutMax )
        /// Psd Pulser selection
        if (ECbmModuleId::kPsd == fvDets[uDetIdx].detId && CbmPsdAddress::GetSectionId(uAddress) != 10) continue;
      }    // else of if( fvDets[ uDetIdx ].uChargeCutMin < fvDets[ uDetIdx ].uChargeCutMax )
    }      // if( fvDets[ uDetIdx ].uChargeCutMin != fvDets[ uDetIdx ].uChargeCutMax )

    /// Fill histos
    fvhDetToRefDiff[uDetIdx]->Fill(dDiffTime);
    fvhDetToRefDiffRefCharge[uDetIdx]->Fill(dDiffTime, dRefCharge);
    fvhDetToRefDiffDetCharge[uDetIdx]->Fill(dDiffTime, dCharge);
    fvhDetToRefDiffEvo[uDetIdx]->Fill(fuNbTs, dDiffTime);
    fvhDetToRefDiffEvoLong[uDetIdx]->Fill(fuNbTs, dDiffTime);
  }  // for( UInt_t uDigiIdx = fvDets[ uDetIdx ].iPrevRefFirstDigi; uDigiIdx < uNbDigis; ++uDigiIdx )

  /// Store earliest possible starting index for next reference digi (time sorted!)
  fvDets[uDetIdx].iPrevRefFirstDigi = uFirstDigiInWin;
}

// ---- Finish --------------------------------------------------------
void CbmMcbmCheckTimingAlgo::Finish() { LOG(info) << Form("Checked %6d Timeslices", fuNbTs); }

void CbmMcbmCheckTimingAlgo::WriteHistos()
{
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* outfile = TFile::Open(fOutFileName, "RECREATE");

  for (UInt_t uDetIdx = 0; uDetIdx < fvDets.size(); ++uDetIdx) {
    LOG(debug) << "Saving histos for " << fvDets[uDetIdx].sName;
    fvhDetSelfDiff[uDetIdx]->Write();
    fvhDetToRefDiffRefCharge[uDetIdx]->Write();
    fvhDetToRefDiffDetCharge[uDetIdx]->Write();
    fvhDetToRefDiffEvo[uDetIdx]->Write();
    fvhDetToRefDiffEvoLong[uDetIdx]->Write();
    LOG(debug) << "WriteHistos, uDetIdx, Det, entries = " << uDetIdx << "  " << fvDets[uDetIdx].sName << "   "
               << fvhDetToRefDiff[uDetIdx]->GetEntries();
    LOG(info) << "Saved histos for " << fvDets[uDetIdx].sName;
    DetPeakPosSingle = fvhDetToRefDiff[uDetIdx]->GetMaximumBin() * fvhDetToRefDiff[uDetIdx]->GetBinWidth(1)
                       + fvhDetToRefDiff[uDetIdx]->GetXaxis()->GetXmin();
    DetAverageSingle = (fvhDetToRefDiff[uDetIdx]->Integral()) / (fvhDetToRefDiff[uDetIdx]->GetNbinsX());
    switch (fvDets[uDetIdx].detId) {
      case ECbmModuleId::kSts: {
        if (DetAverageSingle > 0) {
          TF1* gs_sts = new TF1("gs_sts", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fStsPeakWidthNs,
                                DetPeakPosSingle + 2 * fStsPeakWidthNs);
          gs_sts->SetParameters(DetAverageSingle, DetPeakPosSingle, fStsPeakWidthNs, DetAverageSingle);
          fvhDetToRefDiff[uDetIdx]->Fit("gs_sts", "R");
          TF1* fitresult_sts = fvhDetToRefDiff[uDetIdx]->GetFunction("gs_sts");
          LOG(debug) << fvDets[uDetIdx].sName << " parameters from Gauss fit = " << fitresult_sts->GetParameter(0)
                     << ",  " << fitresult_sts->GetParameter(1) << ",  " << fitresult_sts->GetParameter(2);
        }
        break;
      }
      case ECbmModuleId::kMuch: {
        if (DetAverageSingle > 0) {
          TF1* gs_much = new TF1("gs_much", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fMuchPeakWidthNs,
                                 DetPeakPosSingle + 2 * fMuchPeakWidthNs);
          gs_much->SetParameters(DetAverageSingle, DetPeakPosSingle, fMuchPeakWidthNs, DetAverageSingle);
          fvhDetToRefDiff[uDetIdx]->Fit("gs_much", "R");
          TF1* fitresult_much = fvhDetToRefDiff[uDetIdx]->GetFunction("gs_much");
          LOG(debug) << fvDets[uDetIdx].sName << " parameters from Gauss fit = " << fitresult_much->GetParameter(0)
                     << ",  " << fitresult_much->GetParameter(1) << ",  " << fitresult_much->GetParameter(2);
        }
        break;
      }
      case ECbmModuleId::kTrd: {
        if (DetAverageSingle > 0) {
          TF1* gs_trd = new TF1("gs_trd", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fTrdPeakWidthNs,
                                DetPeakPosSingle + 2 * fTrdPeakWidthNs);
          gs_trd->SetParameters(0.7 * DetAverageSingle, DetPeakPosSingle, fTrdPeakWidthNs, DetAverageSingle);
          fvhDetToRefDiff[uDetIdx]->Fit("gs_trd", "R");
          TF1* fitresult_trd = fvhDetToRefDiff[uDetIdx]->GetFunction("gs_trd");
          LOG(debug) << fvDets[uDetIdx].sName << " parameters from Gauss fit = " << fitresult_trd->GetParameter(0)
                     << ",  " << fitresult_trd->GetParameter(1) << ",  " << fitresult_trd->GetParameter(2);
        }
        break;
      }
      case ECbmModuleId::kTrd2d: {
        if (DetAverageSingle > 0) {
          TF1* gs_trd2d = new TF1("gs_trd2d", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fTrd2dPeakWidthNs,
                                  DetPeakPosSingle + 2 * fTrd2dPeakWidthNs);
          gs_trd2d->SetParameters(0.7 * DetAverageSingle, DetPeakPosSingle, fTrd2dPeakWidthNs, DetAverageSingle);
          fvhDetToRefDiff[uDetIdx]->Fit("gs_trd2d", "R");
          TF1* fitresult_trd2d = fvhDetToRefDiff[uDetIdx]->GetFunction("gs_trd2d");
          LOG(debug) << fvDets[uDetIdx].sName << " parameters from Gauss fit = " << fitresult_trd2d->GetParameter(0)
                     << ",  " << fitresult_trd2d->GetParameter(1) << ",  " << fitresult_trd2d->GetParameter(2);
        }
        break;
      }
      case ECbmModuleId::kT0: {
        if (DetAverageSingle > 0) {
          TF1* gs_tof = new TF1("gs_tof", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fTofPeakWidthNs,
                                DetPeakPosSingle + 2 * fTofPeakWidthNs);
          gs_tof->SetParameters(DetAverageSingle, DetPeakPosSingle, fTofPeakWidthNs, DetAverageSingle);
          fvhDetToRefDiff[uDetIdx]->Fit("gs_tof", "R");
          TF1* fitresult_tof = fvhDetToRefDiff[uDetIdx]->GetFunction("gs_tof");
          LOG(debug) << fvDets[uDetIdx].sName << " parameters from Gauss fit = " << fitresult_tof->GetParameter(0)
                     << ",  " << fitresult_tof->GetParameter(1) << ",  " << fitresult_tof->GetParameter(2);
        }
        break;
      }
      case ECbmModuleId::kTof: {
        if (DetAverageSingle > 0) {
          TF1* gs_tof = new TF1("gs_tof", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fTofPeakWidthNs,
                                DetPeakPosSingle + 2 * fTofPeakWidthNs);
          gs_tof->SetParameters(DetAverageSingle, DetPeakPosSingle, fTofPeakWidthNs, DetAverageSingle);
          fvhDetToRefDiff[uDetIdx]->Fit("gs_tof", "R");
          TF1* fitresult_tof = fvhDetToRefDiff[uDetIdx]->GetFunction("gs_tof");
          LOG(debug) << fvDets[uDetIdx].sName << " parameters from Gauss fit = " << fitresult_tof->GetParameter(0)
                     << ",  " << fitresult_tof->GetParameter(1) << ",  " << fitresult_tof->GetParameter(2);
        }
        break;
      }
      case ECbmModuleId::kRich: {
        if (DetAverageSingle > 0) {
          TF1* gs_rich = new TF1("gs_rich", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fRichPeakWidthNs,
                                 DetPeakPosSingle + 2 * fRichPeakWidthNs);
          gs_rich->SetParameters(0.5 * DetAverageSingle, DetPeakPosSingle, fRichPeakWidthNs, DetAverageSingle);
          fvhDetToRefDiff[uDetIdx]->Fit("gs_rich", "R");
          TF1* fitresult_rich = fvhDetToRefDiff[uDetIdx]->GetFunction("gs_rich");
          LOG(debug) << fvDets[uDetIdx].sName << " parameters from Gauss fit = " << fitresult_rich->GetParameter(0)
                     << ",  " << fitresult_rich->GetParameter(1) << ",  " << fitresult_rich->GetParameter(2);
        }
        break;
      }
      case ECbmModuleId::kPsd: {
        if (DetAverageSingle > 0) {
          TF1* gs_psd = new TF1("gs_psd", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fPsdPeakWidthNs,
                                DetPeakPosSingle + 2 * fPsdPeakWidthNs);
          gs_psd->SetParameters(DetAverageSingle, DetPeakPosSingle, fPsdPeakWidthNs, DetAverageSingle);
          fvhDetToRefDiff[uDetIdx]->Fit("gs_psd", "R");
          TF1* fitresult_psd = fvhDetToRefDiff[uDetIdx]->GetFunction("gs_psd");
          LOG(debug) << fvDets[uDetIdx].sName << " parameters from Gauss fit = " << fitresult_psd->GetParameter(0)
                     << ",  " << fitresult_psd->GetParameter(1) << ",  " << fitresult_psd->GetParameter(2);
        }
        break;
      }
      default: {
        LOG(info) << "Detector ID for fitting is not valid.";
        break;
      }
    }
    fvhDetToRefDiff[uDetIdx]->Write();  //At the end in order to include fitting results in histos
  }  // for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  /// Register the histo for reference detector digi to digi time difference
  fvhDetSelfDiff[fvDets.size()]->Write();

  outfile->Close();
  delete outfile;

  gFile      = oldFile;
  gDirectory = oldDir;
}

// ---- Finish --------------------------------------------------------
void CbmMcbmCheckTimingAlgo::SetReferenceDetector(ECbmModuleId refDetIn, std::string sNameIn, Double_t dTimeRangeBegIn,
                                                  Double_t dTimeRangeEndIn, UInt_t uRangeNbBinsIn,
                                                  UInt_t uChargeCutMinIn, UInt_t uChargeCutMaxIn)
{
  fRefDet.detId         = refDetIn;
  fRefDet.sName         = sNameIn;
  fRefDet.dTimeRangeBeg = dTimeRangeBegIn;
  fRefDet.dTimeRangeEnd = dTimeRangeEndIn;
  fRefDet.uRangeNbBins  = uRangeNbBinsIn;
  fRefDet.uChargeCutMin = uChargeCutMinIn;
  fRefDet.uChargeCutMax = uChargeCutMaxIn;
}
void CbmMcbmCheckTimingAlgo::AddCheckDetector(ECbmModuleId detIn, std::string sNameIn, Double_t dTimeRangeBegIn,
                                              Double_t dTimeRangeEndIn, UInt_t uRangeNbBinsIn, UInt_t uChargeCutMinIn,
                                              UInt_t uChargeCutMaxIn)
{
  std::vector<CheckTimingDetector>::iterator det;
  for (det = fvDets.begin(); det != fvDets.end(); ++det) {
    if ((*det).detId == detIn) {
      LOG(warning) << "CbmMcbmCheckTimingAlgo::AddCheckDetector => Detector already in "
                      "list, this call will only update the parameters!";

      (*det).dTimeRangeBeg = dTimeRangeBegIn;
      (*det).dTimeRangeEnd = dTimeRangeEndIn;
      (*det).uRangeNbBins  = uRangeNbBinsIn;
      (*det).uChargeCutMin = uChargeCutMinIn;
      (*det).uChargeCutMax = uChargeCutMaxIn;

      break;
    }  // if( (*det).detId == detIn )
  }    // for( det = fvDets.begin(); det != fvDets.end(); ++det )

  if (fvDets.end() == det) {
    fvDets.push_back(CheckTimingDetector(detIn, sNameIn));
    det = fvDets.end();
    det--;
    (*det).dTimeRangeBeg = dTimeRangeBegIn;
    (*det).dTimeRangeEnd = dTimeRangeEndIn;
    (*det).uRangeNbBins  = uRangeNbBinsIn;
    (*det).uChargeCutMin = uChargeCutMinIn;
    (*det).uChargeCutMax = uChargeCutMaxIn;
  }  // if( fvDets.end() == det )
}

void CbmMcbmCheckTimingAlgo::RemoveCheckDetector(ECbmModuleId detIn)
{
  for (std::vector<CheckTimingDetector>::iterator det = fvDets.begin(); det != fvDets.end(); ++det) {
    if ((*det).detId == detIn) {
      fvDets.erase(det);
      break;
    }  // if( (*det).detId == detIn )
  }    // for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
}

ClassImp(CbmMcbmCheckTimingAlgo)
