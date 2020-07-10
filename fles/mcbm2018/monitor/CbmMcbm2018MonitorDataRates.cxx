// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmMcbm2018MonitorDataRates                    -----
// -----               Created 26.03.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorDataRates.h"

#include "CbmFlesHistosTools.h"
#include "CbmFormatMsHeaderPrintout.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include "Rtypes.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "THttpServer.h"
#include "TMath.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include <TFile.h>

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdint.h>

Bool_t bMcbmMoniDataRateResetHistos = kFALSE;
Bool_t bMcbmMoniDataRateSaveHistos  = kFALSE;

CbmMcbm2018MonitorDataRates::CbmMcbm2018MonitorDataRates()
  : CbmMcbmUnpack()
  , fvMsComponentsList()
  , fuNbCoreMsPerTs(0)
  , fuNbOverMsPerTs(0)
  , fbIgnoreOverlapMs(kFALSE)
  , fuMsAcceptsPercent(100)
  , fuTotalMsNb(0)
  , fuOverlapMsNb(0)
  , fuCoreMs(0)
  , fdMsSizeInNs(0.0)
  , fdTsCoreSizeInNs(0.0)
  , fsHistoFilename("data/HistosMonitorDataRate.root")
  , fuNbFlimLinks(16)
  , fulCurrentTsIndex(0)
  , fuCurrentMs(0)
  , fuCurrentMsSysId(0)
  , fdMsIndex(0)
  , fiEquipmentId(0)
  , fdStartTimeMsSz(-1.)
  , fuHistoryHistoSize(1800)
  , fvuTsSzLink(fuNbFlimLinks, 0)
  , fcMsSizeAll(nullptr)
  , fhDataRateTimeAllLinks(nullptr)
  , fvhDataRateTimePerLink(fuNbFlimLinks, nullptr)
  , fvhTsSzPerLink(fuNbFlimLinks, nullptr)
  , fvhTsSzTimePerLink(fuNbFlimLinks, nullptr)
  , fvhMsSzPerLink(fuNbFlimLinks, nullptr)
  , fvhMsSzTimePerLink(fuNbFlimLinks, nullptr)
  , fvhMsMessPerLink(fuNbFlimLinks, nullptr)
  , fvhMsMessTimePerLink(fuNbFlimLinks, nullptr)
  , fvhMsMeanChDataPerLink(fuNbFlimLinks, nullptr)
  , fvhMsMeanChDataTimePerLink(fuNbFlimLinks, nullptr) {}

CbmMcbm2018MonitorDataRates::~CbmMcbm2018MonitorDataRates() {}

Bool_t CbmMcbm2018MonitorDataRates::Init() {
  LOG(info) << "Initializing Get4 monitor";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }  // if( ioman == NULL )

  return kTRUE;
}

void CbmMcbm2018MonitorDataRates::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();
}

Bool_t CbmMcbm2018MonitorDataRates::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();
  Bool_t initOK = ReInitContainers();

  CreateHistograms();

  return initOK;
}

Bool_t CbmMcbm2018MonitorDataRates::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();
  return kTRUE;
}


void CbmMcbm2018MonitorDataRates::AddMsComponentToList(
  size_t component,
  UShort_t /*usDetectorId*/) {
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  UInt_t uComp = component;
  if (fuNbFlimLinks <= uComp) {
    fuNbFlimLinks = uComp + 1;
    fvhDataRateTimePerLink.resize(fuNbFlimLinks, nullptr);
    fvhTsSzPerLink.resize(fuNbFlimLinks, nullptr);
    fvhTsSzTimePerLink.resize(fuNbFlimLinks, nullptr);
    fvhMsSzPerLink.resize(fuNbFlimLinks, nullptr);
    fvhMsSzTimePerLink.resize(fuNbFlimLinks, nullptr);
    fvhMsMessPerLink.resize(fuNbFlimLinks, nullptr);
    fvhMsMessTimePerLink.resize(fuNbFlimLinks, nullptr);
    fvhMsMeanChDataPerLink.resize(fuNbFlimLinks, nullptr);
    fvhMsMeanChDataTimePerLink.resize(fuNbFlimLinks, nullptr);

    /// Logarithmic bining
    uint32_t iNbBinsLog = 0;
    /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
    std::vector<double> dBinsLogVector =
      GenerateLogBinArray(9, 9, 10, iNbBinsLog);
    double* dBinsLog = dBinsLogVector.data();
    //      double * dBinsLog = GenerateLogBinArray( 9, 9, 10, iNbBinsLog );

    /// Create Data Rate monitoring histo
    fvhDataRateTimePerLink[uComp] = new TH1D(
      Form("DataRateTime_link_%02u", uComp),
      Form(
        "Data Rate vs time for DPB of link %02u; Time[s] ; DataRate [bytes/s]",
        uComp),
      fuHistoryHistoSize,
      0.,
      fuHistoryHistoSize);

    /// Create TS size monitoring histos
    fvhTsSzPerLink[uComp] =
      new TH1F(Form("TsSz_link_%02u", uComp),
               Form("Size of TS from link %02u; Ts Size [bytes]", uComp),
               iNbBinsLog,
               dBinsLog);

    fvhTsSzTimePerLink[uComp] = new TProfile(
      Form("TsSzTime_link_%02u", uComp),
      Form("Size of TS vs time for DPB of link %02u; Time[s] ; Ts Size [bytes]",
           uComp),
      100 * fuHistoryHistoSize,
      0.,
      fuHistoryHistoSize);

    TString sMsSzName = Form("MsSz_link_%02u", uComp);
    TString sMsSzTitle =
      Form("Size of MS from link %02u; Ms Size [bytes]", uComp);
    fvhMsSzPerLink[uComp] =
      new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 160000, 0., 20000.);

    sMsSzName = Form("MsSzTime_link_%02u", uComp);
    sMsSzTitle =
      Form("Size of MS vs time for DPB of link %02u; Time[s] ; Ms Size [bytes]",
           uComp);
    fvhMsSzTimePerLink[uComp] = new TProfile(sMsSzName.Data(),
                                             sMsSzTitle.Data(),
                                             100 * fuHistoryHistoSize,
                                             0.,
                                             fuHistoryHistoSize);

    sMsSzName = Form("MsMess_link_%02u", uComp);
    sMsSzTitle =
      Form("Messages Number of MS from link %02u; Mess Nb []", uComp);
    fvhMsMessPerLink[uComp] =
      new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 5000, 0., 5000.);

    sMsSzName  = Form("MsMessTime_link_%02u", uComp);
    sMsSzTitle = Form("Messages Number of MS vs time for DPB of link %02u; "
                      "Time[s] ; Mess Nb []",
                      uComp);
    fvhMsMessTimePerLink[uComp] = new TProfile(sMsSzName.Data(),
                                               sMsSzTitle.Data(),
                                               100 * fuHistoryHistoSize,
                                               0.,
                                               fuHistoryHistoSize);

    sMsSzName  = Form("MsMeanChData_link_%02u", uComp);
    sMsSzTitle = Form(
      "Mean data size per channels of MS from link %02u; Mean Ch Data [bytes]",
      uComp);
    fvhMsMeanChDataPerLink[uComp] =
      new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 5000, 0., 5000.);

    sMsSzName  = Form("MsMeanChDataTime_link_%02u", uComp);
    sMsSzTitle = Form("Mean data size per channel of MS vs time for DPB of "
                      "link %02u; Time[s] ; Mean Ch Data[bytes]",
                      uComp);
    fvhMsMeanChDataTimePerLink[uComp] = new TH1D(sMsSzName.Data(),
                                                 sMsSzTitle.Data(),
                                                 fuHistoryHistoSize,
                                                 0.,
                                                 fuHistoryHistoSize);

    /// Cleanup array of log bins
    //      delete dBinsLog;

  }  // if( fuNbFlimLinks <= uComp )

  LOG(info) << "Added MS size histo for component (link): " << component;
}
void CbmMcbm2018MonitorDataRates::SetNbMsInTs(size_t uCoreMsNb,
                                              size_t uOverlapMsNb) {
  fuNbCoreMsPerTs = uCoreMsNb;
  fuNbOverMsPerTs = uOverlapMsNb;

  //   UInt_t uNbMsTotal = fuNbCoreMsPerTs + fuNbOverMsPerTs;
}

void CbmMcbm2018MonitorDataRates::CreateHistograms() {
  LOG(info) << "create Histos ";

  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  /// Logarithmic bining
  uint32_t iNbBinsLog = 0;
  /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
  std::vector<double> dBinsLogVector =
    GenerateLogBinArray(9, 9, 10, iNbBinsLog);
  double* dBinsLog = dBinsLogVector.data();
  //   double * dBinsLog = GenerateLogBinArray( 9, 9, 10, iNbBinsLog );

  fhDataRateTimeAllLinks =
    new TH1D("DataRateTime_all",
             "Data Rate vs time for all DPBs; Time[s] ; DataRate [MB/s]",
             fuHistoryHistoSize,
             0.,
             fuHistoryHistoSize);

  for (UInt_t uComp = 0; uComp < fuNbFlimLinks; ++uComp) {
    /// Create Data Rate monitoring histo
    fvhDataRateTimePerLink[uComp] = new TH1D(
      Form("DataRateTime_link_%02u", uComp),
      Form("Data Rate vs time for DPB of link %02u; Time[s] ; DataRate [MB/s]",
           uComp),
      fuHistoryHistoSize,
      0.,
      fuHistoryHistoSize);

    /// Create TS size monitoring histos
    fvhTsSzPerLink[uComp] =
      new TH1F(Form("TsSz_link_%02u", uComp),
               Form("Size of TS from link %02u; Ts Size [bytes]", uComp),
               iNbBinsLog,
               dBinsLog);

    fvhTsSzTimePerLink[uComp] = new TProfile(
      Form("TsSzTime_link_%02u", uComp),
      Form("Size of TS vs time for DPB of link %02u; Time[s] ; Ts Size [bytes]",
           uComp),
      100 * fuHistoryHistoSize,
      0.,
      fuHistoryHistoSize);

    /// Create MS size monitoring histos
    TString sMsSzName = Form("MsSz_link_%02u", uComp);
    TString sMsSzTitle =
      Form("Size of MS from link %02u; Ms Size [bytes]", uComp);
    fvhMsSzPerLink[uComp] =
      new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), iNbBinsLog, dBinsLog);

    sMsSzName = Form("MsSzTime_link_%02u", uComp);
    sMsSzTitle =
      Form("Size of MS vs time for DPB of link %02u; Time[s] ; Ms Size [bytes]",
           uComp);
    fvhMsSzTimePerLink[uComp] = new TProfile(sMsSzName.Data(),
                                             sMsSzTitle.Data(),
                                             100 * fuHistoryHistoSize,
                                             0.,
                                             fuHistoryHistoSize);

    sMsSzName = Form("MsMess_link_%02u", uComp);
    sMsSzTitle =
      Form("Messages Number of MS from link %02u; Mess Nb []", uComp);
    fvhMsMessPerLink[uComp] =
      new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 5000, 0., 5000.);

    sMsSzName  = Form("MsMessTime_link_%02u", uComp);
    sMsSzTitle = Form("Messages Number of MS vs time for DPB of link %02u; "
                      "Time[s] ; Mess Nb []",
                      uComp);
    fvhMsMessTimePerLink[uComp] = new TProfile(sMsSzName.Data(),
                                               sMsSzTitle.Data(),
                                               100 * fuHistoryHistoSize,
                                               0.,
                                               fuHistoryHistoSize);

    sMsSzName  = Form("MsMeanChData_link_%02u", uComp);
    sMsSzTitle = Form(
      "Mean data size per channels of MS from link %02u; Mean Ch Data [bytes]",
      uComp);
    fvhMsMeanChDataPerLink[uComp] =
      new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 5000, 0., 5000.);

    sMsSzName  = Form("MsMeanChDataTime_link_%02u", uComp);
    sMsSzTitle = Form("Mean data size per channel of MS vs time for DPB of "
                      "link %02u; Time[s] ; Mean Ch Data[bytes]",
                      uComp);
    fvhMsMeanChDataTimePerLink[uComp] = new TH1D(sMsSzName.Data(),
                                                 sMsSzTitle.Data(),
                                                 fuHistoryHistoSize,
                                                 0.,
                                                 fuHistoryHistoSize);

    /// Cleanup array of log bins
    //      delete dBinsLog;
  }  // for( UInt_t uComp = 0; uComp < fuNbFlimLinks; ++uComp )

  /// Create Ms Size Canvas
  Double_t w = 10;
  Double_t h = 10;

  fcDataRateTimeAll =
    new TCanvas("cDataRateTimeAll", "Data Rate per link", w, h);
  fcDataRateTimeAll->Divide(4, 4);

  fcTsSizeAll = new TCanvas("cTsSizeAll", "TS size per link", w, h);
  fcTsSizeAll->Divide(4, 4);
  fcTsSizeTimeAll =
    new TCanvas("cTsSizeTimeAll", "Evolution of TS size per link", w, h);
  fcTsSizeTimeAll->Divide(4, 4);

  fcMsSizeAll = new TCanvas("cMsSizeAll", "MS size per link", w, h);
  fcMsSizeAll->Divide(4, 4);
  fcMsSizeTimeAll =
    new TCanvas("cMsSizeTimeAll", "Evolution of MS size per link", w, h);
  fcMsSizeTimeAll->Divide(4, 4);

  fcMsMessAll = new TCanvas("cMsMessAll", "MS message number per link", w, h);
  fcMsMessAll->Divide(4, 4);
  fcMsMessTimeAll = new TCanvas(
    "cMsMessTimeAll", "Evolution of MS message number per link", w, h);
  fcMsMessTimeAll->Divide(4, 4);

  fcMsDataChAll = new TCanvas(
    "fcMsDataChAll", "Mean data per channel in each MS, per link", w, h);
  fcMsDataChAll->Divide(4, 4);
  fcMsDataChTimeAll = new TCanvas(
    "fcMsDataChTimeAll", "Evolution of Mean data per channel per link", w, h);
  fcMsDataChTimeAll->Divide(4, 4);

  for (UInt_t uComp = 0; uComp < fuNbFlimLinks; ++uComp)
    if (nullptr != fvhMsSzTimePerLink[uComp]) {
      fcDataRateTimeAll->cd(1 + uComp);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhDataRateTimePerLink[uComp]->Draw("hist");

      fcTsSizeAll->cd(1 + uComp);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhTsSzPerLink[uComp]->Draw("hist");

      fcTsSizeTimeAll->cd(1 + uComp);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhTsSzTimePerLink[uComp]->Draw("hist");

      fcMsSizeAll->cd(1 + uComp);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhMsSzPerLink[uComp]->Draw("hist");

      fcMsSizeTimeAll->cd(1 + uComp);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhMsSzTimePerLink[uComp]->Draw("hist");

      fcMsMessAll->cd(1 + uComp);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhMsMessPerLink[uComp]->Draw("hist");

      fcMsMessTimeAll->cd(1 + uComp);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhMsMessTimePerLink[uComp]->Draw("hist");

      fcMsDataChAll->cd(1 + uComp);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhMsMeanChDataPerLink[uComp]->Draw("hist");

      fcMsDataChTimeAll->cd(1 + uComp);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhMsMeanChDataTimePerLink[uComp]->Draw("hist");
    }  // if( nullptr != fvhMsSzTimePerLink[ uComp ] )

  LOG(info) << "Created MS size canvas";

  if (server) {
    server->Register("/FlibRaw", fhDataRateTimeAllLinks);
    for (UInt_t uComp = 0; uComp < fuNbFlimLinks; ++uComp) {
      server->Register("/FlibRaw", fvhDataRateTimePerLink[uComp]);
      server->Register("/FlibRaw", fvhTsSzPerLink[uComp]);
      server->Register("/FlibRaw", fvhTsSzTimePerLink[uComp]);
      server->Register("/FlibRaw", fvhMsSzPerLink[uComp]);
      server->Register("/FlibRaw", fvhMsSzTimePerLink[uComp]);
      server->Register("/FlibRaw", fvhMsMessPerLink[uComp]);
      server->Register("/FlibRaw", fvhMsMessTimePerLink[uComp]);
      server->Register("/FlibRaw", fvhMsMeanChDataPerLink[uComp]);
      server->Register("/FlibRaw", fvhMsMeanChDataTimePerLink[uComp]);
    }  // for( UInt_t uComp = 0; uComp < fuNbFlimLinks; ++uComp )

    server->Register("/canvases", fcDataRateTimeAll);
    server->Register("/canvases", fcTsSizeAll);
    server->Register("/canvases", fcTsSizeTimeAll);
    server->Register("/canvases", fcMsSizeAll);
    server->Register("/canvases", fcMsSizeTimeAll);
    server->Register("/canvases", fcMsMessAll);
    server->Register("/canvases", fcMsMessTimeAll);
    server->Register("/canvases", fcMsDataChAll);
    server->Register("/canvases", fcMsDataChTimeAll);

    server->RegisterCommand("/Reset_All_Hist",
                            "bMcbmMoniDataRateResetHistos=kTRUE");
    server->RegisterCommand("/Save_All_Hist",
                            "bMcbmMoniDataRateSaveHistos=kTRUE");

    server->Restrict("/Reset_All_Hist", "allow=admin");
    server->Restrict("/Save_All_Hist", "allow=admin");
  }  // if( server )

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmMcbm2018MonitorDataRates::DoUnpack(const fles::Timeslice& ts,
                                             size_t /*component*/) {
  if (bMcbmMoniDataRateResetHistos) {
    LOG(info) << "Reset eTOF STAR histos ";
    ResetAllHistos();
    bMcbmMoniDataRateResetHistos = kFALSE;
  }  // if( bMcbmMoniDataRateResetHistos )
  if (bMcbmMoniDataRateSaveHistos) {
    LOG(info) << "Start saving Data Rates Moni histos ";
    SaveAllHistos("data/Histos_Shift_DataRates.root");
    bMcbmMoniDataRateSaveHistos = kFALSE;
  }  // if( bSaveStsHistos )

  /// Ignore First TS as first MS is typically corrupt
  if (0 == ts.index()) return kTRUE;

  /// Ignore overlap ms if flag set by user
  UInt_t uNbMsLoop = fuNbCoreMsPerTs;
  if (kFALSE == fbIgnoreOverlapMs) uNbMsLoop += fuNbOverMsPerTs;

  //   Int_t messageType = -111;
  //   Double_t dTsStartTime = -1;

  /// Reset counters
  for (UInt_t uComp = 0; uComp < fuNbFlimLinks; ++uComp)
    fvuTsSzLink[uComp] = 0;

  /// Loop over core microslices (and overlap ones if chosen)
  for (UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx++) {
    if (fuMsAcceptsPercent < uMsIdx) continue;

    fuCurrentMs = uMsIdx;

    if (0 == fulCurrentTsIndex && 0 == uMsIdx) {
      for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size();
           ++uMsCompIdx) {
        UInt_t uMsComp    = fvMsComponentsList[uMsCompIdx];
        auto msDescriptor = ts.descriptor(uMsComp, uMsIdx);
        /*
            LOG(info) << "hi hv eqid flag si sv idx/start        crc      size     offset";
            LOG(info) << Form( "%02x %02x %04x %04x %02x %02x %016llx %08x %08x %016llx",
                            static_cast<unsigned int>(msDescriptor.hdr_id),
                            static_cast<unsigned int>(msDescriptor.hdr_ver), msDescriptor.eq_id, msDescriptor.flags,
                            static_cast<unsigned int>(msDescriptor.sys_id),
                            static_cast<unsigned int>(msDescriptor.sys_ver), msDescriptor.idx, msDescriptor.crc,
                            msDescriptor.size, msDescriptor.offset );
*/
        LOG(info) << FormatMsHeaderPrintout(msDescriptor);
      }  // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )
    }    // if( 0 == fulCurrentTsIndex && 0 == uMsIdx )

    /// Loop over registered components
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size();
         ++uMsCompIdx) {
      //         constexpr uint32_t kuBytesPerMessage = 8;

      UInt_t uMsComp    = fvMsComponentsList[uMsCompIdx];
      auto msDescriptor = ts.descriptor(uMsComp, uMsIdx);
      UInt_t uSysId     = msDescriptor.sys_id;
      fiEquipmentId     = msDescriptor.eq_id;
      fdMsIndex         = static_cast<double>(msDescriptor.idx) * (1e-9);
      fuCurrentMsSysId  = static_cast<unsigned int>(msDescriptor.sys_id);
      //         const uint8_t* msContent = reinterpret_cast<const uint8_t*>( ts.content( uMsComp, uMsIdx ) );

      uint32_t size    = msDescriptor.size;
      Double_t dSizeMb = size;
      dSizeMb          = dSizeMb / 1024 / 1024;
      fvuTsSzLink[uMsComp] += size;

      //         if( 0 == uMsIdx && 0 == uMsCompIdx )
      //            dTsStartTime = fdMsIndex;

      if (fdStartTimeMsSz < 0) fdStartTimeMsSz = fdMsIndex;
      fvhMsSzPerLink[uMsComp]->Fill(size);
      if (fuHistoryHistoSize < fdMsIndex - fdStartTimeMsSz) {
        // Reset the evolution Histogram and the start time when we reach the end of the range
        fvhMsSzTimePerLink[uMsComp]->Reset();
        fvhMsMessTimePerLink[uMsComp]->Reset();
        fvhMsMeanChDataTimePerLink[uMsComp]->Reset();
        fdStartTimeMsSz = fdMsIndex;
      }  // if( fuHistoryHistoSize < fdMsIndex - fdStartTimeMsSz )
      fhDataRateTimeAllLinks->Fill(fdMsIndex - fdStartTimeMsSz, dSizeMb);
      fvhDataRateTimePerLink[uMsComp]->Fill(fdMsIndex - fdStartTimeMsSz,
                                            dSizeMb);
      fvhMsSzTimePerLink[uMsComp]->Fill(fdMsIndex - fdStartTimeMsSz, size);

      // Compute the number of complete messages in the input microslice buffer, depending on sysid
      uint32_t uNbMessages = 0;
      switch (uSysId) {
        case kuSysIdSts:
          uNbMessages =
            (size - (size % kuBytesPerMessageSts)) / kuBytesPerMessageSts;
          break;
        case kuSysIdRich:
          uNbMessages =
            (size - (size % kuBytesPerMessageRich)) / kuBytesPerMessageRich;
          break;
        case kuSysIdMuch:
          uNbMessages =
            (size - (size % kuBytesPerMessageMuch)) / kuBytesPerMessageMuch;
          break;
        case kuSysIdTof:
          uNbMessages =
            (size - (size % kuBytesPerMessageTof)) / kuBytesPerMessageTof;
          break;
        case kuSysIdT0:
          uNbMessages =
            (size - (size % kuBytesPerMessageT0)) / kuBytesPerMessageT0;
          break;
        default: uNbMessages = (size - (size % 4)) / 4;
      }  // switch( uSysId )
      fvhMsMessPerLink[uMsComp]->Fill(uNbMessages);
      fvhMsMessTimePerLink[uMsComp]->Fill(fdMsIndex - fdStartTimeMsSz,
                                          uNbMessages);

      /// Normalize data size with number of channels
      if (fmChannelsPerEqId.end() != fmChannelsPerEqId.find(fiEquipmentId)) {
        Double_t dMeanDataPerChan = size;
        dMeanDataPerChan /= fmChannelsPerEqId[fiEquipmentId];
        fvhMsMeanChDataPerLink[uMsComp]->Fill(dMeanDataPerChan);
        fvhMsMeanChDataTimePerLink[uMsComp]->Fill(fdMsIndex - fdStartTimeMsSz,
                                                  dMeanDataPerChan);
      }  // if( fDpbIdIndexMap.end() != fmChannelsPerEqId.find( fiEquipmentId ) )
    }  // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )
  }    // for( UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx ++ )

  /// Fill TS plots
  for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size();
       ++uMsCompIdx) {
    UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];
    fvhTsSzPerLink[uMsComp]->Fill(fvuTsSzLink[uMsComp]);
    fvhTsSzTimePerLink[uMsComp]->Fill(fdMsIndex - fdStartTimeMsSz,
                                      fvuTsSzLink[uMsComp]);
  }  // for( UInt_t uComp = 0; uComp < fuNbFlimLinks; ++uComp )

  fulCurrentTsIndex++;

  return kTRUE;
}

void CbmMcbm2018MonitorDataRates::Reset() {}

void CbmMcbm2018MonitorDataRates::Finish() { SaveAllHistos(fsHistoFilename); }

void CbmMcbm2018MonitorDataRates::SaveAllHistos(TString sFileName) {
  TDirectory* oldDir = NULL;
  TFile* histoFile   = NULL;
  if ("" != sFileName) {
    // Store current directory position to allow restore later
    oldDir = gDirectory;
    // open separate histo file in recreate mode
    histoFile = new TFile(sFileName, "RECREATE");
    histoFile->cd();
  }  // if( "" != sFileName )

  gDirectory->mkdir("Flib_Raw");
  gDirectory->cd("Flib_Raw");
  fhDataRateTimeAllLinks->Write();
  for (UInt_t uLinks = 0; uLinks < fuNbFlimLinks; uLinks++)
    if (nullptr != fvhMsSzTimePerLink[uLinks]) {
      fvhDataRateTimePerLink[uLinks]->Write();
      fvhTsSzPerLink[uLinks]->Write();
      fvhTsSzTimePerLink[uLinks]->Write();
      fvhMsSzPerLink[uLinks]->Write();
      fvhMsSzTimePerLink[uLinks]->Write();
      fvhMsMessPerLink[uLinks]->Write();
      fvhMsMessTimePerLink[uLinks]->Write();
      fvhMsMeanChDataPerLink[uLinks]->Write();
      fvhMsMeanChDataTimePerLink[uLinks]->Write();
    }  // if( nullptr != fvhMsSzTimePerLink[ uComp ] )
  fcMsSizeAll->Write();
  fcMsSizeTimeAll->Write();
  fcMsMessAll->Write();
  fcMsMessTimeAll->Write();
  fcMsDataChAll->Write();
  fcMsDataChTimeAll->Write();

  TH1* pMissedTsH1 = dynamic_cast<TH1*>(gROOT->FindObjectAny("Missed_TS"));
  if (NULL != pMissedTsH1) pMissedTsH1->Write();

  TProfile* pMissedTsEvoP =
    dynamic_cast<TProfile*>(gROOT->FindObjectAny("Missed_TS_Evo"));
  if (NULL != pMissedTsEvoP) pMissedTsEvoP->Write();

  gDirectory->cd("..");


  if ("" != sFileName) {
    // Restore original directory position
    histoFile->Close();
    oldDir->cd();
  }  // if( "" != sFileName )
}

void CbmMcbm2018MonitorDataRates::ResetAllHistos() {
  LOG(info) << "Reseting all histograms.";

  fhDataRateTimeAllLinks->Reset();
  for (UInt_t uLinks = 0; uLinks < fuNbFlimLinks; uLinks++)
    if (nullptr != fvhMsSzTimePerLink[uLinks]) {
      fvhDataRateTimePerLink[uLinks]->Reset();
      fvhTsSzPerLink[uLinks]->Reset();
      fvhTsSzTimePerLink[uLinks]->Reset();
      fvhMsSzPerLink[uLinks]->Reset();
      fvhMsSzTimePerLink[uLinks]->Reset();
      fvhMsMessPerLink[uLinks]->Reset();
      fvhMsMessTimePerLink[uLinks]->Reset();
      fvhMsMeanChDataPerLink[uLinks]->Reset();
      fvhMsMeanChDataTimePerLink[uLinks]->Reset();
    }  // if( nullptr != fvhMsSzTimePerLink[ uComp ] )

  fdStartTimeMsSz = -1;
}
ClassImp(CbmMcbm2018MonitorDataRates)
