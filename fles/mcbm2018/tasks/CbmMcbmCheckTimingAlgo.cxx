/* Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Andreas Redelbach */

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

  /// Try to get the 2021 Event header which is containing the Timeslice info
  /// If not present, we have "old" data and will simply catch it with the nullptr value
  auto eh = FairRun::Instance()->GetEventHeader();
  LOG(info) << "CbmMcbmCheckTimingAlgo => EventHeader ptr: " << eh << " (" << eh->IsA()->GetName() << ")";
  fCbmTsEventHeader = dynamic_cast<CbmTsEventHeader*>(eh);
  if (nullptr != fCbmTsEventHeader) {
    LOG(info) << "CbmMcbmCheckTimingAlgo => Using the index from the TS Event header for Evo plots";
  }

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
  else if (!fDigiMan->IsPresent(detToCheck.detId)) {
    LOG(fatal) << "No " << detToCheck.sName << " digis found while it should be used. Stopping there!";
  }  // else if ( ! fDigiMan->IsPresent( detToCheck.detId ) ) of if( ECbmModuleId::kT0 == detToCheck.detId )
}

void CbmMcbmCheckTimingAlgo::CreateHistos()
{
   /// Logarithmic bining for self time comparison
  uint32_t iNbBinsLog = 0;
    /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
  std::vector<double> dBinsLogVector = GenerateLogBinArray( 9, 9, 1, iNbBinsLog );
  double* dBinsLog = dBinsLogVector.data();

  for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )
  {
    for (uint i(0); i < (*det).uNdiv; i++) {
      fvhDetSelfDiff[(*det).detId].push_back(
        new TH1D(Form("h%sSelfDiff%d", (*det).sName.data(), i),
                 Form("time difference between consecutivs %s (%s)  Digis;time diff [ns];Counts", (*det).sName.data(),
                      (*det).vName[i].data()),
                 iNbBinsLog, dBinsLog));

      fvhDetToRefDiff[(*det).detId].push_back(
        new TH1D(Form("h%s%sDiff%d", (*det).sName.data(), fRefDet.sName.data(), i),
                 Form("%s(%s) - %s time difference;time diff [ns];Counts", (*det).sName.data(), (*det).vName[i].data(),
                      fRefDet.sName.data()),
                 (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd));

      fvhDetToRefDiffRefCharge[(*det).detId].push_back(
        new TH2F(Form("h%s%sDiffRefCharge%d", (*det).sName.data(), fRefDet.sName.data(), i),
                 Form("%s(%s) - %s;time diff [ns]; %s Charge [a.u]; Counts", (*det).sName.data(),
                      (*det).vName[i].data(), fRefDet.sName.data(), fRefDet.sName.data()),
                 (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd, 256, 0, 256));

      fvhDetToRefDiffDetCharge[(*det).detId].push_back(
        new TH2F(Form("h%s%sDiffDetCharge%d", (*det).sName.data(), fRefDet.sName.data(), i),
                 Form("%s(%s) - %s;time diff [ns]; %s(%s) Charge [a.u]; Counts", (*det).sName.data(),
                      (*det).vName[i].data(), fRefDet.sName.data(), (*det).sName.data(), (*det).vName[i].data()),
                 (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd, 256, 0, 256));

      fvhDetToRefDiffEvo[(*det).detId].push_back(
        new TH2F(Form("h%s%sDiffEvo%d", (*det).sName.data(), fRefDet.sName.data(), i),
                 Form("%s(%s) - %s;TS; time diff [ns];Counts", (*det).sName.data(), (*det).vName[i].data(),
                      fRefDet.sName.data()),
                 10000, -0.5, 10000 - 0.5, (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd));

      fvhDetToRefDiffEvoLong[(*det).detId].push_back(
        new TH2F(Form("h%s%sDiffEvoLong%d", (*det).sName.data(), fRefDet.sName.data(), i),
                 Form("%s(%s) - %s;TS; time diff [ns];Counts", (*det).sName.data(), (*det).vName[i].data(),
                      fRefDet.sName.data()),
                 1800, 0, 18000, (*det).uRangeNbBins, (*det).dTimeRangeBeg, (*det).dTimeRangeEnd));
    }
    LOG( info ) << "Created histos for " << (*det).sName;
  } // for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  /// Add reference detector digi to digi time difference histo at end of vector
  fvhDetSelfDiff[fRefDet.detId].push_back(
    new TH1D(Form("h%sSelfDiff", fRefDet.sName.data()),
             Form("time difference between consecutivs %s  Digis;time diff [ns];Counts", fRefDet.sName.data()),
             iNbBinsLog, dBinsLog));

  /// Register the histos in the HTTP server
  FairRunOnline* run = FairRunOnline::Instance();
  if( run )
  {
    THttpServer* server = run->GetHttpServer();
    if( nullptr != server )
    {
      /// Register histos for all checked detectors
      for (auto uDetIdx : fvDets) {
        server->Register("/CheckTiming/SelfDiff", fvhDetSelfDiff[uDetIdx.detId][0]);
        server->Register("/CheckTiming/RefDiff", fvhDetToRefDiff[uDetIdx.detId][0]);
        server->Register("/CheckTiming/DiffCharge", fvhDetToRefDiffRefCharge[uDetIdx.detId][0]);
        server->Register("/CheckTiming/DiffCharge", fvhDetToRefDiffDetCharge[uDetIdx.detId][0]);
        server->Register("/CheckTiming/DiffEvo", fvhDetToRefDiffEvo[uDetIdx.detId][0]);
        server->Register("/CheckTiming/DiffEvo", fvhDetToRefDiffEvoLong[uDetIdx.detId][0]);
      }  // for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

      /// Register the histo for reference detector digi to digi time difference
      server->Register("/CheckTiming/SelfDiff", fvhDetSelfDiff[fRefDet.detId][0]);
    } // if( nullptr != server )
  } // if( run )
}
// ---- ReInit  -------------------------------------------------------
Bool_t CbmMcbmCheckTimingAlgo::ReInit() { return kTRUE; }

// ---- Exec ----------------------------------------------------------
void CbmMcbmCheckTimingAlgo::ProcessTs()
{
  LOG(info) << "executing TS " << fuNbTs << " index ["
            << (fCbmTsEventHeader != nullptr ? fCbmTsEventHeader->GetTsIndex() : -1) << "]";

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
    }
    else {
      dRefTime   = fDigiMan->Get<DigiRef>(uDigi)->GetTime();
      dRefCharge = fDigiMan->Get<DigiRef>(uDigi)->GetCharge();
      uRefAddress = fDigiMan->Get<DigiRef>(uDigi)->GetAddress();
    }  // else of if( ECbmModuleId::kT0 == fRefDet.detId )

    /// Fill self time difference histo
    (fvhDetSelfDiff[fRefDet.detId])[0]->Fill(dRefTime - fRefDet.dPrevTime);
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
  ECbmModuleId edetId = fvDets[uDetIdx].detId;
  switch (edetId) {
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
    default: {
      uNbDigis = fDigiMan->GetNofDigis(edetId);
      break;
    }  // default:
  }    // switch( fRefDet.detId )

  UInt_t uFirstDigiInWin = fvDets[uDetIdx].iPrevRefFirstDigi;

  std::vector<Double_t> vSelDiff;
  for (UInt_t uDigiIdx = fvDets[uDetIdx].iPrevRefFirstDigi; uDigiIdx < uNbDigis; ++uDigiIdx) {
    Double_t dTime   = 0;
    Double_t dCharge = 0;
    UInt_t uAddress  = 0;
    const void* digi(nullptr);
    if (ECbmModuleId::kT0 == edetId) {
      dTime   = fpT0DigiVec->at(uDigiIdx).GetTime();
      dCharge = fpT0DigiVec->at(uDigiIdx).GetCharge();
    }  // if( ECbmModuleId::kT0 == fRefDet.detId )
    else {
      digi     = fDigiMan->Get<Digi>(uDigiIdx);
      dTime   = fDigiMan->Get<Digi>(uDigiIdx)->GetTime();
      dCharge = fDigiMan->Get<Digi>(uDigiIdx)->GetCharge();
      uAddress = fDigiMan->Get<Digi>(uDigiIdx)->GetAddress();
    }  // else of if( ECbmModuleId::kT0 == fRefDet.detId )

    /// Fill self correlation histo while avoiding double counting due to
    /// the "smart looping"
    if (fvDets[uDetIdx].dPrevTime <= dTime) {
      vSelDiff.push_back(dTime - fvDets[uDetIdx].dPrevTime);
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

    for (uint uid(0); uid < fvDets[uDetIdx].uNdiv; uid++) {
      if (digi && !CheckCondition(uDetIdx, uid, digi)) continue;

      /// Fill histos
      for (auto dt : vSelDiff)
        fvhDetSelfDiff[edetId][uid]->Fill(dt);
      vSelDiff.clear();

      fvhDetToRefDiff[edetId][uid]->Fill(dDiffTime);
      fvhDetToRefDiffRefCharge[edetId][uid]->Fill(dDiffTime, dRefCharge);
      fvhDetToRefDiffDetCharge[edetId][uid]->Fill(dDiffTime, dCharge);
      if (nullptr == fCbmTsEventHeader) {
        fvhDetToRefDiffEvo[edetId][uid]->Fill(fuNbTs, dDiffTime);
        fvhDetToRefDiffEvoLong[edetId][uid]->Fill(fuNbTs, dDiffTime);
      }
      else {
        fvhDetToRefDiffEvo[edetId][uid]->Fill(fCbmTsEventHeader->GetTsIndex(), dDiffTime);
        fvhDetToRefDiffEvoLong[edetId][uid]->Fill(fCbmTsEventHeader->GetTsIndex(), dDiffTime);
      }
      break;
    }
  }  // for( UInt_t uDigiIdx = fvDets[ uDetIdx ].iPrevRefFirstDigi; uDigiIdx < uNbDigis; ++uDigiIdx )

  /// Store earliest possible starting index for next reference digi (time sorted!)
  fvDets[uDetIdx].iPrevRefFirstDigi = uFirstDigiInWin;
}

// ---- CheckCondition ------------------------------------------------
bool CbmMcbmCheckTimingAlgo::CheckCondition(UInt_t uDetId, UInt_t uCond, const void* digi)
{
  CheckTimingDetector uDet = fvDets[uDetId];
  if (uCond >= uDet.uNdiv) return false;
  switch (uDet.detId) {
    case ECbmModuleId::kSts: return CheckCondition(&uDet, uCond, static_cast<const CbmStsDigi*>(digi));
    case ECbmModuleId::kMuch: return CheckCondition(&uDet, uCond, static_cast<const CbmMuchDigi*>(digi));
    case ECbmModuleId::kTrd: return CheckCondition(&uDet, uCond, static_cast<const CbmTrdDigi*>(digi));
    case ECbmModuleId::kTof: return CheckCondition(&uDet, uCond, static_cast<const CbmTofDigi*>(digi));
    case ECbmModuleId::kRich: return CheckCondition(&uDet, uCond, static_cast<const CbmRichDigi*>(digi));
    case ECbmModuleId::kPsd: return CheckCondition(&uDet, uCond, static_cast<const CbmPsdDigi*>(digi));
    default: break;
  }
  return true;
}
bool CbmMcbmCheckTimingAlgo::CheckCondition(CheckTimingDetector* /*det*/, UInt_t /*uCond*/, const CbmStsDigi* /*digi*/)
{
  return true;
}
bool CbmMcbmCheckTimingAlgo::CheckCondition(CheckTimingDetector* /*det*/, UInt_t /*uCond*/, const CbmMuchDigi* /*digi*/)
{
  return true;
}
bool CbmMcbmCheckTimingAlgo::CheckCondition(CheckTimingDetector* det, UInt_t uCond, const CbmTrdDigi* digi)
{
  try {
    int moduleId = std::stoi(det->vName[uCond]);
    if (digi->GetAddressModule() != moduleId) return false;
  }
  catch (const std::invalid_argument& ia) {
    LOG(warning) << "Trd condition " << det->vName[uCond] << " not implemented. Skipped";
    return false;
  }
  return true;
}
bool CbmMcbmCheckTimingAlgo::CheckCondition(CheckTimingDetector* /*det*/, UInt_t /*uCond*/, const CbmTofDigi* /*digi*/)
{
  return true;
}
bool CbmMcbmCheckTimingAlgo::CheckCondition(CheckTimingDetector* /*det*/, UInt_t /*uCond*/, const CbmRichDigi* /*digi*/)
{
  return true;
}
bool CbmMcbmCheckTimingAlgo::CheckCondition(CheckTimingDetector* /*det*/, UInt_t /*uCond*/, const CbmPsdDigi* /*digi*/)
{
  return true;
}

// ---- Finish --------------------------------------------------------
void CbmMcbmCheckTimingAlgo::Finish() { LOG(info) << Form("Checked %6d Timeslices", fuNbTs); }

void CbmMcbmCheckTimingAlgo::WriteHistos()
{
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* outfile = TFile::Open(fOutFileName, "RECREATE");

  for (auto uDet : fvDets) {
    LOG(debug) << "Saving histos for " << uDet.sName;
    outfile->mkdir(uDet.sName.data());
    outfile->cd(uDet.sName.data());
    for (uint id(0); id < uDet.uNdiv; id++)
      fvhDetSelfDiff[uDet.detId][id]->Write();
    for (uint id(0); id < uDet.uNdiv; id++)
      fvhDetToRefDiffRefCharge[uDet.detId][id]->Write();
    for (uint id(0); id < uDet.uNdiv; id++)
      fvhDetToRefDiffDetCharge[uDet.detId][id]->Write();
    for (uint id(0); id < uDet.uNdiv; id++)
      fvhDetToRefDiffEvo[uDet.detId][id]->Write();
    for (uint id(0); id < uDet.uNdiv; id++)
      fvhDetToRefDiffEvoLong[uDet.detId][id]->Write();
    for (uint id(0); id < uDet.uNdiv; id++) {
      LOG(debug) << "WriteHistos, Det, entries = " << uDet.sName << "   "
                 << fvhDetToRefDiff[uDet.detId][id]->GetEntries();
      LOG(info) << "Saved histos for " << uDet.sName << "(" << uDet.vName[id] << ")";
      DetPeakPosSingle =
        fvhDetToRefDiff[uDet.detId][id]->GetMaximumBin() * fvhDetToRefDiff[uDet.detId][id]->GetBinWidth(1)
        + fvhDetToRefDiff[uDet.detId][id]->GetXaxis()->GetXmin();
      DetAverageSingle = (fvhDetToRefDiff[uDet.detId][id]->Integral()) / (fvhDetToRefDiff[uDet.detId][id]->GetNbinsX());

      switch (uDet.detId) {
        case ECbmModuleId::kSts: {
          if (DetAverageSingle > 0) {
            TF1* gs_sts = new TF1("gs_sts", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fStsPeakWidthNs,
                                  DetPeakPosSingle + 2 * fStsPeakWidthNs);
            gs_sts->SetParameters(DetAverageSingle, DetPeakPosSingle, fStsPeakWidthNs, DetAverageSingle);
            fvhDetToRefDiff[uDet.detId][id]->Fit("gs_sts", "R");
            TF1* fitresult_sts = fvhDetToRefDiff[uDet.detId][id]->GetFunction("gs_sts");
            LOG(debug) << uDet.sName << " parameters from Gauss fit = " << fitresult_sts->GetParameter(0) << ",  "
                       << fitresult_sts->GetParameter(1) << ",  " << fitresult_sts->GetParameter(2);
          }
          break;
        }
        case ECbmModuleId::kMuch: {
          if (DetAverageSingle > 0) {
            TF1* gs_much = new TF1("gs_much", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fMuchPeakWidthNs,
                                   DetPeakPosSingle + 2 * fMuchPeakWidthNs);
            gs_much->SetParameters(DetAverageSingle, DetPeakPosSingle, fMuchPeakWidthNs, DetAverageSingle);
            fvhDetToRefDiff[uDet.detId][id]->Fit("gs_much", "R");
            TF1* fitresult_much = fvhDetToRefDiff[uDet.detId][id]->GetFunction("gs_much");
            LOG(debug) << uDet.sName << " parameters from Gauss fit = " << fitresult_much->GetParameter(0) << ",  "
                       << fitresult_much->GetParameter(1) << ",  " << fitresult_much->GetParameter(2);
          }
          break;
        }
        case ECbmModuleId::kTrd: {
          if (DetAverageSingle > 0) {
            TF1* gs_trd = new TF1("gs_trd", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fTrdPeakWidthNs,
                                  DetPeakPosSingle + 2 * fTrdPeakWidthNs);
            gs_trd->SetParameters(0.7 * DetAverageSingle, DetPeakPosSingle, fTrdPeakWidthNs, DetAverageSingle);
            fvhDetToRefDiff[uDet.detId][id]->Fit("gs_trd", "R");
            TF1* fitresult_trd = fvhDetToRefDiff[uDet.detId][id]->GetFunction("gs_trd");
            LOG(debug) << uDet.sName << " parameters from Gauss fit = " << fitresult_trd->GetParameter(0) << ",  "
                       << fitresult_trd->GetParameter(1) << ",  " << fitresult_trd->GetParameter(2);
          }
          break;
        }
        case ECbmModuleId::kT0: {
          if (DetAverageSingle > 0) {
            TF1* gs_tof = new TF1("gs_tof", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fTofPeakWidthNs,
                                  DetPeakPosSingle + 2 * fTofPeakWidthNs);
            gs_tof->SetParameters(DetAverageSingle, DetPeakPosSingle, fTofPeakWidthNs, DetAverageSingle);
            fvhDetToRefDiff[uDet.detId][id]->Fit("gs_tof", "R");
            TF1* fitresult_tof = fvhDetToRefDiff[uDet.detId][id]->GetFunction("gs_tof");
            LOG(debug) << uDet.sName << " parameters from Gauss fit = " << fitresult_tof->GetParameter(0) << ",  "
                       << fitresult_tof->GetParameter(1) << ",  " << fitresult_tof->GetParameter(2);
          }
          break;
        }
        case ECbmModuleId::kTof: {
          if (DetAverageSingle > 0) {
            TF1* gs_tof = new TF1("gs_tof", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fTofPeakWidthNs,
                                  DetPeakPosSingle + 2 * fTofPeakWidthNs);
            gs_tof->SetParameters(DetAverageSingle, DetPeakPosSingle, fTofPeakWidthNs, DetAverageSingle);
            fvhDetToRefDiff[uDet.detId][id]->Fit("gs_tof", "R");
            TF1* fitresult_tof = fvhDetToRefDiff[uDet.detId][id]->GetFunction("gs_tof");
            LOG(debug) << uDet.sName << " parameters from Gauss fit = " << fitresult_tof->GetParameter(0) << ",  "
                       << fitresult_tof->GetParameter(1) << ",  " << fitresult_tof->GetParameter(2);
          }
          break;
        }
        case ECbmModuleId::kRich: {
          if (DetAverageSingle > 0) {
            TF1* gs_rich = new TF1("gs_rich", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fRichPeakWidthNs,
                                   DetPeakPosSingle + 2 * fRichPeakWidthNs);
            gs_rich->SetParameters(0.5 * DetAverageSingle, DetPeakPosSingle, fRichPeakWidthNs, DetAverageSingle);
            fvhDetToRefDiff[uDet.detId][id]->Fit("gs_rich", "R");
            TF1* fitresult_rich = fvhDetToRefDiff[uDet.detId][id]->GetFunction("gs_rich");
            LOG(debug) << uDet.sName << " parameters from Gauss fit = " << fitresult_rich->GetParameter(0) << ",  "
                       << fitresult_rich->GetParameter(1) << ",  " << fitresult_rich->GetParameter(2);
          }
          break;
        }
        case ECbmModuleId::kPsd: {
          if (DetAverageSingle > 0) {
            TF1* gs_psd = new TF1("gs_psd", "gaus(0)+pol0(3)", DetPeakPosSingle - 2 * fPsdPeakWidthNs,
                                  DetPeakPosSingle + 2 * fPsdPeakWidthNs);
            gs_psd->SetParameters(DetAverageSingle, DetPeakPosSingle, fPsdPeakWidthNs, DetAverageSingle);
            fvhDetToRefDiff[uDet.detId][id]->Fit("gs_psd", "R");
            TF1* fitresult_psd = fvhDetToRefDiff[uDet.detId][id]->GetFunction("gs_psd");
            LOG(debug) << uDet.sName << " parameters from Gauss fit = " << fitresult_psd->GetParameter(0) << ",  "
                       << fitresult_psd->GetParameter(1) << ",  " << fitresult_psd->GetParameter(2);
          }
          break;
        }
        default: {
          LOG(info) << "Detector ID for fitting is not valid.";
          break;
        }
      }
      fvhDetToRefDiff[uDet.detId][id]->Write();  //At the end in order to include fitting results in histos
    }
    outfile->cd();
  }  // for( std::vector< CheckTimingDetector >::iterator det = fvDets.begin(); det != fvDets.end(); ++det )

  /// Register the histo for reference detector digi to digi time difference
  outfile->mkdir(fRefDet.sName.data());
  outfile->cd(fRefDet.sName.data());
  for (uint id(0); id < fRefDet.uNdiv; id++)
    fvhDetSelfDiff[fRefDet.detId][id]->Write();
  outfile->cd();

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

void CbmMcbmCheckTimingAlgo::SetDetectorDifferential(ECbmModuleId detIn, std::vector<std::string> vName)
{
  bool found(false);
  std::vector<CheckTimingDetector>::iterator det;
  for (det = fvDets.begin(); det != fvDets.end(); ++det) {
    if ((*det).detId != detIn) continue;
    found        = true;
    (*det).vName = vName;
    (*det).uNdiv = vName.size();
    break;
  }

  if (!found)
    LOG(warning) << "CbmMcbmCheckTimingAlgo::SetDetectorDifferential => Detector not in the "
                    "list, Nothing done at this point!";
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
