// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmCheckDataFormatGdpb2018                          -----
// -----               Created 10.07.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmCheckDataFormatGdpb2018.h"
#include "CbmMcbm2018TofPar.h"

#include "CbmFormatMsHeaderPrintout.h"

#include "CbmHistManager.h"

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

Bool_t bCheckFormatGdpbResetHistos = kFALSE;
Bool_t bCheckFormatGdpbSaveHistos  = kFALSE;

CbmCheckDataFormatGdpb2018::CbmCheckDataFormatGdpb2018()
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
  , fuMinNbGdpb(0)
  , fuCurrNbGdpb(0)
  , fUnpackPar()
  , fuNrOfGdpbs(0)
  , fuNrOfFeePerGdpb(0)
  , fuNrOfGet4PerFee(0)
  , fuNrOfChannelsPerGet4(0)
  , fuNrOfChannelsPerFee(0)
  , fuNrOfGet4(0)
  , fuNrOfGet4PerGdpb(0)
  , fuNrOfChannelsPerGdpb(0)
  , fuDiamondDpbIdx(10000)
  ,  // Crazy default value => should never make troubles given the price
  fsHistoFilename("data/HistosCheckGdpb.root")
  , fulCurrentTsIndex(0)
  , fuCurrentMs(0)
  , fuCurrentMsSysId(0)
  , fdMsIndex(0)
  , fuGdpbId(0)
  , fuGdpbNr(0)
  , fuGet4Id(0)
  , fuGet4Nr(0)
  , fiEquipmentId(0)
  , fvulCurrentEpoch()
  ,
  /*
    fvbFirstEpochSeen(),
    fvulCurrentEpochCycle(),
    fvulCurrentEpochFull(),
*/
  fulCurrentEpochTime(0)
  , fGdpbIdIndexMap()
  , fcMsSizeAll(NULL)
  , fdStartTimeMsSz(-1.)
  , fvhMsSzPerLink(12, NULL)
  , fvhMsSzTimePerLink(12, NULL)
  , fhMessType(NULL)
  , fhSysMessType(NULL)
  , fhGet4MessType(NULL)
  , fhGet4ChanScm(NULL)
  , fhGet4ChanErrors(NULL)
  , fhGet4EpochFlags(NULL)
  , fhGdpbMessType(NULL)
  , fhGdpbSysMessType(NULL)
  , fhGdpbSysMessPattType(NULL)
  , fhGdpbEpochFlags(NULL)
  , fhGdpbEpochSyncEvo(NULL)
  , fhGdpbEpochMissEvo(NULL)
  , fvhGdpbGet4MessType()
  , fhPatternMissmatch(NULL)
  , fhPatternEnable(NULL)
  , fhPatternResync(NULL)
  , fvuGdpbNbEpochPerMs()
  , fvvuChanNbHitsPerMs()
  , fhEpochsPerMs_gDPB()
  , fhEpochsPerMsPerTs_gDPB()
  , fhEpochsDiff_gDPB()
  , fhEpochsDiffPerTs_gDPB()
  , fhEpochsJumpBitsPre_gDPB()
  , fhEpochsJumpBitsNew_gDPB()
  , fhEpochsJumpDigitsPre_gDPB()
  , fhEpochsJumpDigitsNew_gDPB()
  , fhStartEpochPerMs_gDPB()
  , fhCloseEpochPerMs_gDPB()
  , fhHitsPerMsFirstChan_gDPB()
  , fvhChannelRatePerMs_gDPB()
  , fcFormatGdpb()
  , fTimeLastHistoSaving() {}

CbmCheckDataFormatGdpb2018::~CbmCheckDataFormatGdpb2018() {}

Bool_t CbmCheckDataFormatGdpb2018::Init() {
  LOG(info) << "Initializing Get4 monitor";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }  // if( ioman == NULL )

  return kTRUE;
}

void CbmCheckDataFormatGdpb2018::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar =
    (CbmMcbm2018TofPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer(
      "CbmMcbm2018TofPar"));
}

Bool_t CbmCheckDataFormatGdpb2018::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();
  Bool_t initOK = ReInitContainers();

  CreateHistograms();

  fvulCurrentEpoch.resize(fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvulCurrentEpoch[uGdpb] = 0;
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  return initOK;
}

Bool_t CbmCheckDataFormatGdpb2018::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();

  fuNrOfGdpbs = fUnpackPar->GetNrOfGdpbs();
  LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;
  fuMinNbGdpb = fuNrOfGdpbs;

  fuNrOfFeePerGdpb = fUnpackPar->GetNrOfFeesPerGdpb();
  LOG(info) << "Nr. of FEEs per Tof GDPB: " << fuNrOfFeePerGdpb;

  fuNrOfGet4PerFee = fUnpackPar->GetNrOfGet4PerFee();
  LOG(info) << "Nr. of GET4 per Tof FEE: " << fuNrOfGet4PerFee;

  fuNrOfChannelsPerGet4 = fUnpackPar->GetNrOfChannelsPerGet4();
  LOG(info) << "Nr. of channels per GET4: " << fuNrOfChannelsPerGet4;

  fuNrOfChannelsPerFee = fuNrOfGet4PerFee * fuNrOfChannelsPerGet4;
  LOG(info) << "Nr. of channels per FEE: " << fuNrOfChannelsPerFee;

  fuNrOfGet4 = fuNrOfGdpbs * fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
  LOG(info) << "Nr. of GET4s: " << fuNrOfGet4;

  fuNrOfGet4PerGdpb = fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
  LOG(info) << "Nr. of GET4s per GDPB: " << fuNrOfGet4PerGdpb;

  fuNrOfChannelsPerGdpb = fuNrOfGet4PerGdpb * fuNrOfChannelsPerGet4;
  LOG(info) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;

  fGdpbIdIndexMap.clear();
  for (UInt_t i = 0; i < fuNrOfGdpbs; ++i) {
    fGdpbIdIndexMap[fUnpackPar->GetGdpbId(i)] = i;
    LOG(info) << "GDPB Id of TOF  " << i << " : " << std::hex
              << fUnpackPar->GetGdpbId(i) << std::dec;
  }  // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

  fuNrOfGbtx = fUnpackPar->GetNrOfGbtx();
  LOG(info) << "Nr. of GBTx: " << fuNrOfGbtx;

  fuTotalMsNb      = fUnpackPar->GetNbMsTot();
  fuOverlapMsNb    = fUnpackPar->GetNbMsOverlap();
  fuCoreMs         = fuTotalMsNb - fuOverlapMsNb;
  fdMsSizeInNs     = fUnpackPar->GetSizeMsInNs();
  fdTsCoreSizeInNs = fdMsSizeInNs * fuCoreMs;
  LOG(info) << "Timeslice parameters: " << fuTotalMsNb
            << " MS per link, of which " << fuOverlapMsNb
            << " overlap MS, each MS is " << fdMsSizeInNs << " ns";

  return kTRUE;
}


void CbmCheckDataFormatGdpb2018::AddMsComponentToList(
  size_t component,
  UShort_t /*usDetectorId*/) {
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  /// Create MS size monitoring histos
  if (NULL == fvhMsSzPerLink[component]) {
    TString sMsSzName = Form("MsSz_link_%02lu", component);
    TString sMsSzTitle =
      Form("Size of MS from link %02lu; Ms Size [bytes]", component);
    fvhMsSzPerLink[component] =
      new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 160000, 0., 20000.);

    sMsSzName  = Form("MsSzTime_link_%02lu", component);
    sMsSzTitle = Form(
      "Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]",
      component);
    fvhMsSzTimePerLink[component] = new TProfile(
      sMsSzName.Data(), sMsSzTitle.Data(), 100 * 1800, 0., 2 * 1800);
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
    if (server) {
      server->Register("/FlibRaw", fvhMsSzPerLink[component]);
      server->Register("/FlibRaw", fvhMsSzTimePerLink[component]);
    }  // if( server )
    if (NULL != fcMsSizeAll) {
      fcMsSizeAll->cd(1 + component);
      gPad->SetLogy();
      fvhMsSzTimePerLink[component]->Draw("hist le0");
    }  // if( NULL != fcMsSizeAll )
    LOG(info) << "Added MS size histo for component (link): " << component;
  }  // if( NULL == fvhMsSzPerLink[ component ] )
}
void CbmCheckDataFormatGdpb2018::SetNbMsInTs(size_t uCoreMsNb,
                                             size_t uOverlapMsNb) {
  fuNbCoreMsPerTs = uCoreMsNb;
  fuNbOverMsPerTs = uOverlapMsNb;

  //   UInt_t uNbMsTotal = fuNbCoreMsPerTs + fuNbOverMsPerTs;
}

void CbmCheckDataFormatGdpb2018::CreateHistograms() {
  LOG(info) << "create Histos for " << fuNrOfGdpbs << " gDPBs ";

  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  TString name {""};
  TString title {""};
  UInt_t uHistoryHistoSize = 1800;

  /*******************************************************************/
  name  = "hMessageType";
  title = "Nb of message for each type; Type";
  // Test Big Data readout with plotting
  fhMessType = new TH1I(name,
                        title,
                        1 + gdpbv100::MSG_STAR_TRI_D,
                        0.,
                        1 + gdpbv100::MSG_STAR_TRI_D);
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_HIT, "HIT");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_EPOCH, "EPOCH");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SLOWC, "SLOWC");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SYST, "SYST");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_A, "TRI_A");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_B, "TRI_B");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_C, "TRI_C");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_D, "TRI_D");

  /*******************************************************************/
  name          = "hSysMessType";
  title         = "Nb of system message for each type; System Type";
  fhSysMessType = new TH1I(
    name, title, 1 + gdpbv100::SYS_PATTERN, 0., 1 + gdpbv100::SYS_PATTERN);
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_ERROR,
                                         "GET4 ERROR");
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GDPB_UNKWN,
                                         "UNKW GET4 MSG");
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_SYNC_MISS,
                                         "SYS_GET4_SYNC_MISS");
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_PATTERN,
                                         "SYS_PATTERN");

  /*******************************************************************/
  name           = "hGet4MessType";
  title          = "Nb of message for each type per GET4; GET4 chip # ; Type";
  fhGet4MessType = new TH2I(name, title, fuNrOfGet4, 0., fuNrOfGet4, 4, 0., 4.);
  fhGet4MessType->GetYaxis()->SetBinLabel(1, "DATA 32b");
  fhGet4MessType->GetYaxis()->SetBinLabel(2, "EPOCH");
  fhGet4MessType->GetYaxis()->SetBinLabel(3, "S.C. M");
  fhGet4MessType->GetYaxis()->SetBinLabel(4, "ERROR");

  /*******************************************************************/
  name          = "hGet4ChanScm";
  title         = "SC messages per GET4 channel; GET4 channel # ; SC type";
  fhGet4ChanScm = new TH2I(name,
                           title,
                           2 * fuNrOfGet4 * fuNrOfChannelsPerGet4,
                           0.,
                           fuNrOfGet4 * fuNrOfChannelsPerGet4,
                           5,
                           0.,
                           5.);
  fhGet4ChanScm->GetYaxis()->SetBinLabel(1, "Hit Scal");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(2, "Deadtime");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(3, "SPI");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(4, "SEU Scal");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(5, "START");

  /*******************************************************************/
  name             = "hGet4ChanErrors";
  title            = "Error messages per GET4 channel; GET4 channel # ; Error";
  fhGet4ChanErrors = new TH2I(name,
                              title,
                              fuNrOfGet4 * fuNrOfChannelsPerGet4,
                              0.,
                              fuNrOfGet4 * fuNrOfChannelsPerGet4,
                              21,
                              0.,
                              21.);
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(1, "0x00: Readout Init    ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(2, "0x01: Sync            ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(3, "0x02: Epoch count sync");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(4, "0x03: Epoch           ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(5, "0x04: FIFO Write      ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(6, "0x05: Lost event      ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(7, "0x06: Channel state   ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(8, "0x07: Token Ring state");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(9, "0x08: Token           ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    12, "0x0b: DLL Lock error  ");  // <- From GET4 v1.2
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    13, "0x0c: DLL Reset invoc.");  // <- From GET4 v1.2
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    17, "0x14: Add. Rising edge");  // <- From GET4 v1.3
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    18, "0x15: Unpaired Falling");  // <- From GET4 v1.3
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    19, "0x16: Sequence error  ");  // <- From GET4 v1.3
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(20, "0x7f: Unknown         ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(21, "Corrupt/unsuprtd error");

  /*******************************************************************/
  name  = "hGet4EpochFlags";
  title = "Epoch flags per GET4; GET4 chip # ; Type";
  fhGet4EpochFlags =
    new TH2I(name, title, fuNrOfGet4, 0., fuNrOfGet4, 4, 0., 4.);
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(2, "Ep LOSS");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(3, "Da LOSS");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(4, "MISSMAT");

  /*******************************************************************/
  name  = "hGdpbMessageType";
  title = "Nb of message for each type per Gdpb; Type; Gdpb Idx []";
  // Test Big Data readout with plotting
  fhGdpbMessType = new TH2I(name,
                            title,
                            1 + gdpbv100::MSG_STAR_TRI_D,
                            0.,
                            1 + gdpbv100::MSG_STAR_TRI_D,
                            fuNrOfGdpbs,
                            -0.5,
                            fuNrOfGdpbs - 0.5);
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_HIT, "HIT");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_EPOCH, "EPOCH");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SLOWC, "SLOWC");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SYST, "SYST");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_A,
                                          "TRI_A");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_B,
                                          "TRI_B");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_C,
                                          "TRI_C");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_D,
                                          "TRI_D");

  /*******************************************************************/
  name = "hGdpbSysMessType";
  title =
    "Nb of system message for each type per Gdpb; System Type; Gdpb Idx []";
  fhGdpbSysMessType = new TH2I(name,
                               title,
                               1 + gdpbv100::SYS_PATTERN,
                               0.,
                               1 + gdpbv100::SYS_PATTERN,
                               fuNrOfGdpbs,
                               -0.5,
                               fuNrOfGdpbs - 0.5);
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_ERROR,
                                             "GET4 ERROR");
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GDPB_UNKWN,
                                             "UNKW GET4 MSG");
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_SYNC_MISS,
                                             "SYS_GET4_SYNC_MISS");
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_PATTERN,
                                             "SYS_PATTERN");

  /*******************************************************************/
  name = "hGdpbSysMessPattType";
  title =
    "Nb of pattern message for each type per Gdpb; Pattern Type; Gdpb Idx []";
  fhGdpbSysMessPattType = new TH2I(name,
                                   title,
                                   1 + gdpbv100::PATT_RESYNC,
                                   0.,
                                   1 + gdpbv100::PATT_RESYNC,
                                   fuNrOfGdpbs,
                                   -0.5,
                                   fuNrOfGdpbs - 0.5);
  fhGdpbSysMessPattType->GetXaxis()->SetBinLabel(1 + gdpbv100::PATT_MISSMATCH,
                                                 "PATT_MISSMATCH");
  fhGdpbSysMessPattType->GetXaxis()->SetBinLabel(1 + gdpbv100::PATT_ENABLE,
                                                 "PATT_ENABLE");
  fhGdpbSysMessPattType->GetXaxis()->SetBinLabel(1 + gdpbv100::PATT_RESYNC,
                                                 "PATT_RESYNC");

  /*******************************************************************/
  name  = "hGdpbEpochFlags";
  title = "Epoch flags per gDPB; gDPB # ; Type";
  fhGdpbEpochFlags =
    new TH2I(name, title, fuNrOfGdpbs, 0., fuNrOfGdpbs, 4, 0., 4.);
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(2, "Ep LOSS");
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(3, "Da LOSS");
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(4, "MISSMAT");

  /*******************************************************************/
  name  = Form("hGdpbEpochSyncEvo");
  title = Form("Epoch SYNC per second and gDPB; Time[s];  gDPB #; SYNC Nb");
  fhGdpbEpochSyncEvo = new TH2D(name.Data(),
                                title.Data(),
                                uHistoryHistoSize,
                                0,
                                uHistoryHistoSize,
                                fuNrOfGdpbs,
                                0.,
                                fuNrOfGdpbs);

  /*******************************************************************/
  name = Form("hGdpbEpochMissEvo");
  title =
    Form("Epoch Missmatch per second and gDPB; Time[s];  gDPB #; Missmatch Nb");
  fhGdpbEpochMissEvo = new TH2D(name.Data(),
                                title.Data(),
                                uHistoryHistoSize,
                                0,
                                uHistoryHistoSize,
                                fuNrOfGdpbs,
                                0.,
                                fuNrOfGdpbs);


  /*******************************************************************/
  name  = "hPatternMissmatch";
  title = "Missmatch pattern integral per Gdpb; ASIC Pattern []; Gdpb Idx []";
  fhPatternMissmatch = new TH2I(name,
                                title,
                                fuNrOfGet4PerGdpb,
                                0.,
                                fuNrOfGet4PerGdpb,
                                fuNrOfGdpbs,
                                -0.5,
                                fuNrOfGdpbs - 0.5);
  name               = "hPatternEnable";
  title = "Enable pattern integral per Gdpb; ASIC Pattern []; Gdpb Idx []";
  fhPatternEnable = new TH2I(name,
                             title,
                             fuNrOfGet4PerGdpb,
                             0.,
                             fuNrOfGet4PerGdpb,
                             fuNrOfGdpbs,
                             -0.5,
                             fuNrOfGdpbs - 0.5);
  name            = "hPatternResync";
  title = "Resync pattern integral per Gdpb; ASIC Pattern []; Gdpb Idx []";
  fhPatternResync = new TH2I(name,
                             title,
                             fuNrOfGet4PerGdpb,
                             0.,
                             fuNrOfGet4PerGdpb,
                             fuNrOfGdpbs,
                             -0.5,
                             fuNrOfGdpbs - 0.5);


  /*******************************************************************/
  fvuGdpbNbEpochPerMs.resize(fuNrOfGdpbs, 0);
  fvvuChanNbHitsPerMs.resize(fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    /*******************************************************************/
    name  = Form("hGdpbGet4MessType_%02u", uGdpb);
    title = Form(
      "Nb of message for each type per GET4 in gDPB %02u; GET4 chip # ; Type",
      uGdpb);
    fvhGdpbGet4MessType.push_back(new TH2I(
      name, title, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb, 4, 0., 4.));
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(1, "DATA 32b");
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(2, "EPOCH");
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(3, "S.C. M");
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(4, "ERROR");

    /*******************************************************************/
    name  = Form("hGdpbGet4ChanErrors_%02u", uGdpb);
    title = Form(
      "Error messages per GET4 channel in gDPB %02u; GET4 channel # ; Error",
      uGdpb);
    fvhGdpbGet4ChanErrors.push_back(new TH2I(name,
                                             title,
                                             fuNrOfChannelsPerGdpb,
                                             0.,
                                             fuNrOfChannelsPerGdpb,
                                             22,
                                             0.,
                                             22.));
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      1, "0x00: Readout Init    ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      2, "0x01: Sync            ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      3, "0x02: Epoch count sync");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      4, "0x03: Epoch           ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      5, "0x04: FIFO Write      ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      6, "0x05: Lost event      ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      7, "0x06: Channel state   ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      8, "0x07: Token Ring state");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      9, "0x08: Token           ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      10, "0x09: Error Readout   ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      11, "0x0a: SPI             ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      12, "0x0b: DLL Lock error  ");  // <- From GET4 v1.2
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      13, "0x0c: DLL Reset invoc.");  // <- From GET4 v1.2
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      14, "0x11: Overwrite       ");  // <- From GET4 v1.0 to 1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      15, "0x12: ToT out of range");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      16, "0x13: Event Discarded ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      17, "0x14: Add. Rising edge");  // <- From GET4 v1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      18, "0x15: Unpaired Falling");  // <- From GET4 v1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      19, "0x16: Sequence error  ");  // <- From GET4 v1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      20, "0x17: Epoch Overflow  ");  // <- From GET4 v2.0
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      21, "0x7f: Unknown         ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      22, "Corrupt/unsuprtd error");

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    fvvuChanNbHitsPerMs[uGdpb].resize(fuNrOfChannelsPerGdpb, 0);
    fhEpochsPerMs_gDPB.push_back(new TH1D(
      Form("hEpochsPerMs_gDPB%02u", uGdpb),
      Form("Epoch Counts per MS in gDPB %02u; Epochs/MS []; MS nb[]", uGdpb),
      1000,
      -0.5,
      999.5));
    fhEpochsPerMsPerTs_gDPB.push_back(new TH2D(
      Form("hEpochsPerMsPerTs_gDPB%02u", uGdpb),
      Form("Epoch Counts per MS in gDPB %02u; TS []; Epochs/MS []; MS nb[]",
           uGdpb),
      10000,
      0,
      10000,
      100,
      -0.5,
      99.5));
    fhEpochsDiff_gDPB.push_back(
      new TH1D(Form("hEpochsDiff_gDPB%02u", uGdpb),
               Form("Epoch index difference per MS in gDPB %02u; Ep(N) - Ep(N "
                    "- 1) []; MS nb[]",
                    uGdpb),
               1001,
               -500.5,
               500.5));
    fhEpochsDiffPerTs_gDPB.push_back(
      new TH2D(Form("hEpochsDiffPerTs_gDPB%02u", uGdpb),
               Form("Epoch index difference per MS in gDPB %02u; TS []; Ep(N) "
                    "- Ep(N - 1) []; MS nb[]",
                    uGdpb),
               10000,
               0,
               10000,
               101,
               -50.5,
               50.5));
    fhEpochsJumpBitsPre_gDPB.push_back(
      new TH2D(Form("hEpochsJumpBitsPre_gDPB%02u", uGdpb),
               Form("Bits value in previous epoch when diff not 1 in gDPB "
                    "%02u; TS []; Bit []; Value[]",
                    uGdpb),
               32,
               -0.5,
               31.5,
               2,
               -0.5,
               1.5));
    fhEpochsJumpBitsNew_gDPB.push_back(
      new TH2D(Form("hEpochsJumpBitsNew_gDPB%02u", uGdpb),
               Form("Bits value in new epoch when diff not 1 in gDPB %02u; TS "
                    "[]; Bit []; Value[]",
                    uGdpb),
               32,
               -0.5,
               31.5,
               2,
               -0.5,
               1.5));
    fhEpochsJumpDigitsPre_gDPB.push_back(
      new TH2D(Form("hEpochsJumpDigitsPre_gDPB%02u", uGdpb),
               Form("Digits value in previous epoch when diff not 1 in gDPB "
                    "%02u; TS []; Digit []; Value[]",
                    uGdpb),
               10,
               -0.5,
               9.5,
               10,
               -0.5,
               9.5));
    fhEpochsJumpDigitsNew_gDPB.push_back(
      new TH2D(Form("hEpochsJumpDigitsNew_gDPB%02u", uGdpb),
               Form("Digits value in new epoch when diff not 1 in gDPB %02u; "
                    "TS []; Digit []; Value[]",
                    uGdpb),
               10,
               -0.5,
               9.5,
               10,
               -0.5,
               9.5));
    fhStartEpochPerMs_gDPB.push_back(
      new TH2D(Form("fhStartEpochPerMs_gDPB%02u", uGdpb),
               Form("MS start with Epoch in gDPB %02u?; TS []; 1st Msg is "
                    "Epoch? []; MS nb[]",
                    uGdpb),
               10000,
               0,
               10000,
               2,
               -0.5,
               1.5));
    fhCloseEpochPerMs_gDPB.push_back(
      new TH2D(Form("fhCloseEpochPerMs_gDPB%02u", uGdpb),
               Form("MS close with Epoch in gDPB %02u?; TS []; Last Msg is "
                    "Epoch? []; MS nb[]",
                    uGdpb),
               10000,
               0,
               10000,
               2,
               -0.5,
               1.5));
    fhHitsPerMsFirstChan_gDPB.push_back(
      new TH2D(Form("hHitsPerMsFirstChan_gDPB%02u", uGdpb),
               Form("Hit Counts per MS in first channel in gDPB %02u; TS []; "
                    "Hits/MS []; MS nb[]",
                    uGdpb),
               10000,
               0,
               10000,
               150,
               -0.5,
               149.5));
    fvhChannelRatePerMs_gDPB.push_back(
      new TProfile2D(Form("hChannelRatePerMs_gDPB%02u", uGdpb),
                     Form("Mean Hit count per MS and channel vs Time in gDPB "
                          "%02u; TS []; Channel []; <Hits/Ms> []",
                          uGdpb),
                     10000,
                     0,
                     10000,
                     fuNrOfChannelsPerGdpb,
                     -0.5,
                     fuNrOfChannelsPerGdpb - 0.5));
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  if (server) {
    server->Register("/TofRaw", fhMessType);
    server->Register("/TofRaw", fhSysMessType);
    server->Register("/TofRaw", fhGet4MessType);
    server->Register("/TofRaw", fhGet4ChanScm);
    server->Register("/TofRaw", fhGet4ChanErrors);
    server->Register("/TofRaw", fhGet4EpochFlags);

    server->Register("/TofRaw", fhGdpbMessType);
    server->Register("/TofRaw", fhGdpbSysMessType);
    server->Register("/TofRaw", fhGdpbSysMessPattType);
    server->Register("/TofRaw", fhGdpbEpochFlags);
    server->Register("/TofRaw", fhGdpbEpochSyncEvo);
    server->Register("/TofRaw", fhGdpbEpochMissEvo);

    server->Register("/TofRaw", fhPatternMissmatch);
    server->Register("/TofRaw", fhPatternEnable);
    server->Register("/TofRaw", fhPatternResync);

    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      server->Register("/TofRaw", fvhGdpbGet4MessType[uGdpb]);
      server->Register("/TofRaw", fvhGdpbGet4ChanErrors[uGdpb]);

      server->Register("/Tof_Ep_FineCount", fhEpochsPerMs_gDPB[uGdpb]);
      server->Register("/Tof_Ep_FineCount", fhEpochsPerMsPerTs_gDPB[uGdpb]);
      server->Register("/Tof_Ep_FineCount", fhEpochsDiff_gDPB[uGdpb]);
      server->Register("/Tof_Ep_FineCount", fhEpochsDiffPerTs_gDPB[uGdpb]);
      server->Register("/Tof_Ep_FineCount", fhEpochsJumpBitsPre_gDPB[uGdpb]);
      server->Register("/Tof_Ep_FineCount", fhEpochsJumpBitsNew_gDPB[uGdpb]);
      server->Register("/Tof_Ep_FineCount", fhEpochsJumpDigitsPre_gDPB[uGdpb]);
      server->Register("/Tof_Ep_FineCount", fhEpochsJumpDigitsNew_gDPB[uGdpb]);
      server->Register("/Tof_Ep_FineCount", fhStartEpochPerMs_gDPB[uGdpb]);
      server->Register("/Tof_Ep_FineCount", fhCloseEpochPerMs_gDPB[uGdpb]);

      server->Register("/ChanFineRate", fhHitsPerMsFirstChan_gDPB[uGdpb]);
      server->Register("/ChanFineRate", fvhChannelRatePerMs_gDPB[uGdpb]);
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

    server->RegisterCommand("/Reset_All_eTOF",
                            "bCheckFormatGdpbResetHistos=kTRUE");
    server->RegisterCommand("/Save_All_eTof",
                            "bCheckFormatGdpbSaveHistos=kTRUE");

    server->Restrict("/Reset_All_eTof", "allow=admin");
    server->Restrict("/Save_All_eTof", "allow=admin");
  }  // if( server )

  /** Create summary Canvases for STAR 2018 **/
  Double_t w = 10;
  Double_t h = 10;
  fcSummary  = new TCanvas("cSummary", "gDPB Monitoring Summary");
  fcSummary->Divide(2, 3);

  // 1st Column: Messages types
  fcSummary->cd(1);
  gPad->SetLogy();
  fhMessType->Draw();

  fcSummary->cd(2);
  gPad->SetLogy();
  fhSysMessType->Draw();

  fcSummary->cd(3);
  gPad->SetLogz();
  fhGet4MessType->Draw("colz");

  // 2nd Column: GET4 Errors + Epoch flags + SCm
  fcSummary->cd(4);
  gPad->SetLogz();
  fhGet4ChanErrors->Draw("colz");

  fcSummary->cd(5);
  gPad->SetLogz();
  fhGet4EpochFlags->Draw("colz");

  fcSummary->cd(6);
  fhGet4ChanScm->Draw("colz");

  server->Register("/canvases", fcSummary);
  /*****************************/

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fcFormatGdpb.push_back(
      new TCanvas(Form("cFormatGdpb%02u", uGdpb),
                  Form("gDPB %02u Data Format Check", uGdpb)));
    fcFormatGdpb[uGdpb]->Divide(4, 2);

    fcFormatGdpb[uGdpb]->cd(1);
    gPad->SetGridx();
    gPad->SetGridy();
    fhEpochsPerMsPerTs_gDPB[uGdpb]->Draw("colz");

    fcFormatGdpb[uGdpb]->cd(2);
    gPad->SetGridx();
    gPad->SetGridy();
    fhEpochsDiffPerTs_gDPB[uGdpb]->Draw("colz");

    fcFormatGdpb[uGdpb]->cd(3);
    gPad->SetGridx();
    gPad->SetGridy();
    fhStartEpochPerMs_gDPB[uGdpb]->Draw("colz");

    fcFormatGdpb[uGdpb]->cd(4);
    gPad->SetGridx();
    gPad->SetGridy();
    fhCloseEpochPerMs_gDPB[uGdpb]->Draw("colz");

    fcFormatGdpb[uGdpb]->cd(5);
    gPad->SetGridx();
    gPad->SetGridy();
    fhHitsPerMsFirstChan_gDPB[uGdpb]->Draw("colz");

    fcFormatGdpb[uGdpb]->cd(6);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhChannelRatePerMs_gDPB[uGdpb]->Draw("colz");

    fcFormatGdpb[uGdpb]->cd(7);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhEpochsJumpBitsPre_gDPB[uGdpb]->Draw("colz");

    fcFormatGdpb[uGdpb]->cd(8);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhEpochsJumpBitsNew_gDPB[uGdpb]->Draw("colz");

    server->Register("/canvases", fcFormatGdpb[uGdpb]);
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  /*****************************/

  /** Recovers/Create Ms Size Canvas for STAR 2018 **/
  // Try to recover canvas in case it was created already by another monitor
  // If not existing, create it
  fcMsSizeAll = dynamic_cast<TCanvas*>(gROOT->FindObject("cMsSizeAll"));
  if (NULL == fcMsSizeAll) {
    fcMsSizeAll =
      new TCanvas("cMsSizeAll", "Evolution of MS size in last 300 s", w, h);
    fcMsSizeAll->Divide(4, 3);
    LOG(info) << "Created MS size canvas in TOF monitor";
  }  // if( NULL == fcMsSizeAll )
  else
    LOG(info) << "Recovered MS size canvas in TOF monitor";

  server->Register("/canvases", fcMsSizeAll);

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmCheckDataFormatGdpb2018::DoUnpack(const fles::Timeslice& ts,
                                            size_t component) {
  if (bCheckFormatGdpbResetHistos) {
    LOG(info) << "Reset eTOF STAR histos ";
    ResetAllHistos();
    bCheckFormatGdpbResetHistos = kFALSE;
  }  // if( bCheckFormatGdpbResetHistos )
  if (bCheckFormatGdpbSaveHistos) {
    LOG(info) << "Start saving eTOF STAR histos ";
    SaveAllHistos("data/histos_Shift_StarTof.root");
    bCheckFormatGdpbSaveHistos = kFALSE;
  }  // if( bSaveStsHistos )

  /// Periodically save the histograms
  std::chrono::time_point<std::chrono::system_clock> timeCurrent =
    std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds =
    timeCurrent - fTimeLastHistoSaving;
  if (0 == fTimeLastHistoSaving.time_since_epoch().count()) {
    fTimeLastHistoSaving = timeCurrent;
  }  // if( 0 == fTimeLastHistoSaving.time_since_epoch().count() )
  else if (300 < elapsed_seconds.count()) {
    std::time_t cTimeCurrent =
      std::chrono::system_clock::to_time_t(timeCurrent);
    char tempBuff[80];
    std::strftime(tempBuff, 80, "%F %T", localtime(&cTimeCurrent));
    fTimeLastHistoSaving = timeCurrent;
    SaveAllHistos("data/histosCheckGdpbAuto.root");
  }  // else if( 300 < elapsed_seconds.count() )

  LOG(debug1) << "Timeslice contains " << ts.num_microslices(component)
              << "microslices.";

  /// Ignore First TS as first MS is typically corrupt
  if (0 == ts.index()) return kTRUE;

  /// Ignore overlap ms if flag set by user
  UInt_t uNbMsLoop = fuNbCoreMsPerTs;
  if (kFALSE == fbIgnoreOverlapMs) uNbMsLoop += fuNbOverMsPerTs;

  /// Store the TS index for later use
  //   UInt_t uTsIndexHeader = ts.index();
  UInt_t uTsIndexHeader = fulCurrentTsIndex;

  Int_t messageType = -111;
  //   Double_t dTsStartTime = -1;

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
      constexpr uint32_t kuBytesPerMessage = 8;

      UInt_t uMsComp    = fvMsComponentsList[uMsCompIdx];
      auto msDescriptor = ts.descriptor(uMsComp, uMsIdx);
      fiEquipmentId     = msDescriptor.eq_id;
      fdMsIndex         = static_cast<double>(msDescriptor.idx);
      fuCurrentMsSysId  = static_cast<unsigned int>(msDescriptor.sys_id);
      const uint8_t* msContent =
        reinterpret_cast<const uint8_t*>(ts.content(uMsComp, uMsIdx));

      uint32_t size = msDescriptor.size;
      //    fulLastMsIdx = msDescriptor.idx;
      if (size > 0)
        LOG(debug) << "Microslice: " << msDescriptor.idx
                   << " has size: " << size;

      //         if( 0 == uMsIdx && 0 == uMsCompIdx )
      //            dTsStartTime = (1e-9) * fdMsIndex;

      if (fdStartTimeMsSz < 0) fdStartTimeMsSz = (1e-9) * fdMsIndex;
      fvhMsSzPerLink[uMsComp]->Fill(size);
      fvhMsSzTimePerLink[uMsComp]->Fill((1e-9) * fdMsIndex - fdStartTimeMsSz,
                                        size);

      // If not integer number of message in input buffer, print warning/error
      if (0 != (size % kuBytesPerMessage))
        LOG(error) << "The input microslice buffer does NOT "
                   << "contain only complete nDPB messages!";

      // Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages =
        (size - (size % kuBytesPerMessage)) / kuBytesPerMessage;

      // Get the gDPB ID from the MS header
      fuGdpbId = fiEquipmentId;

      /// Check if this gDPB ID was declared in parameter file and stop there if not
      auto it = fGdpbIdIndexMap.find(fuGdpbId);
      if (it == fGdpbIdIndexMap.end()) {
        LOG(info)
          << "---------------------------------------------------------------";
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
        LOG(error) << "Could not find the gDPB index for AFCK id 0x" << std::hex
                   << fuGdpbId << std::dec << " in timeslice "
                   << fulCurrentTsIndex << " in microslice " << fdMsIndex
                   << " component " << uMsCompIdx << "\n"
                   << "If valid this index has to be added in the TOF "
                      "parameter file in the RocIdArray field";
        continue;
      }  // if( it == fGdpbIdIndexMap.end() )
      else
        fuGdpbNr = fGdpbIdIndexMap[fuGdpbId];

      // Prepare variables for the loop on contents
      const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);
      for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
        // Fill message
        uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);

        /// Catch the Epoch cycle block which is always the first 64b of the MS
        if (0 == uIdx) {
          continue;
        }  // if( 0 == uIdx && kFALSE == fbOldFwData )

        gdpbv100::Message mess(ulData);

        // Detect message types
        // and fill the corresponding histogram
        messageType = mess.getMessageType();
        fhMessType->Fill(messageType);
        fhGdpbMessType->Fill(messageType, fuGdpbNr);

        fuGet4Id = fUnpackPar->ElinkIdxToGet4Idx(mess.getGdpbGenChipId());
        /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
        if (fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId)
          fuGet4Id = mess.getGdpbGenChipId();
        fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;

        if (fuNrOfGet4PerGdpb <= fuGet4Id && !mess.isStarTrigger()
            && (gdpbv100::kuChipIdMergedEpoch != fuGet4Id))
          LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id
                       << " VS " << fuNrOfGet4PerGdpb << " set in parameters.";

        if (1 == uIdx)
          fhStartEpochPerMs_gDPB[fuGdpbNr]->Fill(
            uTsIndexHeader, gdpbv100::MSG_EPOCH == messageType);
        if (uNbMessages - 1 == uIdx)
          fhCloseEpochPerMs_gDPB[fuGdpbNr]->Fill(
            uTsIndexHeader, gdpbv100::MSG_EPOCH == messageType);

        switch (messageType) {
          case gdpbv100::MSG_HIT: {
            if (mess.getGdpbHitIs24b()) {
              fhGet4MessType->Fill(fuGet4Nr, 4);
              fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 4);
              /// Should never happen!
            }  // if( getGdpbHitIs24b() )
            else {
              fhGet4MessType->Fill(fuGet4Nr, 0);
              fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 0);

              UInt_t uChannel   = mess.getGdpbHitChanId();
              UInt_t uChannelNr = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
              fvvuChanNbHitsPerMs[fuGdpbNr][uChannelNr]++;
            }  // else of if( getGdpbHitIs24b() )
            break;
          }  // case gdpbv100::MSG_HIT:
          case gdpbv100::MSG_EPOCH: {
            if (gdpbv100::kuChipIdMergedEpoch == fuGet4Id) {
              if (1 == mess.getGdpbEpSync()) {
                fhGdpbEpochFlags->Fill(fuGdpbNr, 0);
                fhGdpbEpochSyncEvo->Fill((1e-9) * fdMsIndex - fdStartTimeMsSz,
                                         fuGdpbNr);
              }  // if (1 == mess.getGdpbEpSync())

              if (1 == mess.getGdpbEpDataLoss())
                fhGdpbEpochFlags->Fill(fuGdpbNr, 1);

              if (1 == mess.getGdpbEpEpochLoss())
                fhGdpbEpochFlags->Fill(fuGdpbNr, 2);

              if (1 == mess.getGdpbEpMissmatch()) {
                fhGdpbEpochFlags->Fill(fuGdpbNr, 3);
                fhGdpbEpochMissEvo->Fill((1e-9) * fdMsIndex - fdStartTimeMsSz,
                                         fuGdpbNr);
              }  // if (1 == mess.getGdpbEpMissmatch())

              for (uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb;
                   uGet4Index++) {
                fuGet4Id = uGet4Index;
                fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;
                gdpbv100::Message tmpMess(mess);
                tmpMess.setGdpbGenChipId(uGet4Index);

                fhGet4MessType->Fill(fuGet4Nr, 1);
                fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 1);
              }  // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGetIndex ++ )

              /// Count the epochs
              fvuGdpbNbEpochPerMs[fuGdpbNr]++;

              /// Check that the epochs are always increasing by 1 in each gDPB
              ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();
              Long64_t iEpDiff    = ulEpochNr;
              iEpDiff -= fvulCurrentEpoch[fuGdpbNr];
              fhEpochsDiff_gDPB[fuGdpbNr]->Fill(iEpDiff);
              fhEpochsDiffPerTs_gDPB[fuGdpbNr]->Fill(uTsIndexHeader, iEpDiff);

              if (1 != iEpDiff && 0 < fvulCurrentEpoch[fuGdpbNr]) {
                for (UInt_t uBit = 0; uBit < 32; ++uBit) {
                  fhEpochsJumpBitsPre_gDPB[fuGdpbNr]->Fill(
                    uBit, (fvulCurrentEpoch[fuGdpbNr] >> uBit) & 0x1);
                  fhEpochsJumpBitsNew_gDPB[fuGdpbNr]->Fill(
                    uBit, (ulEpochNr >> uBit) & 0x1);
                }  // for( UInt_t uBit = 0; uBit < 32; ++uBit )

                UInt_t uPower = 1;
                for (UInt_t uDigit = 0; uDigit < 10; ++uDigit) {
                  fhEpochsJumpDigitsPre_gDPB[fuGdpbNr]->Fill(
                    uDigit,
                    TMath::Floor((fvulCurrentEpoch[fuGdpbNr] % (10 * uPower))
                                 / uPower));
                  fhEpochsJumpDigitsNew_gDPB[fuGdpbNr]->Fill(
                    uDigit, TMath::Floor((ulEpochNr % (10 * uPower)) / uPower));
                  uPower *= 10;
                }  // for( UInt_t uDigit = 0; uDigit < 10; ++uDigit )
                   /*
                         LOG(info) << Form( " Non consecutive epochs for Gdpb %2u in TS %8u MS %4u Msg %6u",
                                             fuGdpbNr, uTsIndexHeader, uMsIdx, uIdx )
                                   << Form( " : old Ep %08llx new Ep %08llx Diff %lld ", fvulCurrentEpoch[ fuGdpbNr ], ulEpochNr, iEpDiff )
                                   << std::endl;
*/
              }    // if( 1 != iEpDiff )

              /// Store new epoch index
              fvulCurrentEpoch[fuGdpbNr] = ulEpochNr;
            }  // if this epoch message is a merged one valid for all chips
            else {
              /// Should never happen!
            }  // if single chip epoch message
            break;
          }  // case gdpbv100::MSG_EPOCH:
          case gdpbv100::MSG_SLOWC: {
            fhGet4MessType->Fill(fuGet4Nr, 2);
            fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 2);
            break;
          }  // case gdpbv100::MSG_SLOWC:
          case gdpbv100::MSG_SYST: {
            fhSysMessType->Fill(mess.getGdpbSysSubType());
            fhGdpbSysMessType->Fill(mess.getGdpbSysSubType(), fuGdpbNr);
            if (gdpbv100::SYS_GET4_ERROR == mess.getGdpbSysSubType()) {
              fhGet4MessType->Fill(fuGet4Nr, 3);
              fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 3);

              //                     UInt_t uFeeNr   = (fuGet4Id / fuNrOfGet4PerFee);

              Int_t dGdpbChId =
                fuGet4Id * fuNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();
              Int_t dFullChId =
                fuGet4Nr * fuNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();
              switch (mess.getGdpbSysErrData()) {
                case gdpbv100::GET4_V2X_ERR_READ_INIT:
                  fhGet4ChanErrors->Fill(dFullChId, 0);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 0);
                  break;
                case gdpbv100::GET4_V2X_ERR_SYNC:
                  fhGet4ChanErrors->Fill(dFullChId, 1);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 1);
                  break;
                case gdpbv100::GET4_V2X_ERR_EP_CNT_SYNC:
                  fhGet4ChanErrors->Fill(dFullChId, 2);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 2);
                  break;
                case gdpbv100::GET4_V2X_ERR_EP:
                  fhGet4ChanErrors->Fill(dFullChId, 3);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 3);
                  break;
                case gdpbv100::GET4_V2X_ERR_FIFO_WRITE:
                  fhGet4ChanErrors->Fill(dFullChId, 4);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 4);
                  break;
                case gdpbv100::GET4_V2X_ERR_LOST_EVT:
                  fhGet4ChanErrors->Fill(dFullChId, 5);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 5);
                  break;
                case gdpbv100::GET4_V2X_ERR_CHAN_STATE:
                  fhGet4ChanErrors->Fill(dFullChId, 6);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 6);
                  break;
                case gdpbv100::GET4_V2X_ERR_TOK_RING_ST:
                  fhGet4ChanErrors->Fill(dFullChId, 7);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 7);
                  break;
                case gdpbv100::GET4_V2X_ERR_TOKEN:
                  fhGet4ChanErrors->Fill(dFullChId, 8);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 8);
                  break;
                case gdpbv100::GET4_V2X_ERR_READOUT_ERR:
                  fhGet4ChanErrors->Fill(dFullChId, 9);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 9);
                  break;
                case gdpbv100::GET4_V2X_ERR_SPI:
                  fhGet4ChanErrors->Fill(dFullChId, 10);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 10);
                  break;
                case gdpbv100::GET4_V2X_ERR_DLL_LOCK:
                  fhGet4ChanErrors->Fill(dFullChId, 11);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 11);
                  break;
                case gdpbv100::GET4_V2X_ERR_DLL_RESET:
                  fhGet4ChanErrors->Fill(dFullChId, 12);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 12);
                  break;
                case gdpbv100::GET4_V2X_ERR_TOT_OVERWRT:
                  fhGet4ChanErrors->Fill(dFullChId, 13);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 13);
                  break;
                case gdpbv100::GET4_V2X_ERR_TOT_RANGE:
                  fhGet4ChanErrors->Fill(dFullChId, 14);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 14);
                  break;
                case gdpbv100::GET4_V2X_ERR_EVT_DISCARD:
                  fhGet4ChanErrors->Fill(dFullChId, 15);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 15);
                  break;
                case gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG:
                  fhGet4ChanErrors->Fill(dFullChId, 16);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 16);
                  break;
                case gdpbv100::GET4_V2X_ERR_UNPAIR_FALL:
                  fhGet4ChanErrors->Fill(dFullChId, 17);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 17);
                  break;
                case gdpbv100::GET4_V2X_ERR_SEQUENCE_ER:
                  fhGet4ChanErrors->Fill(dFullChId, 18);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 18);
                  break;
                case gdpbv100::GET4_V2X_ERR_EPOCH_OVERF:
                  fhGet4ChanErrors->Fill(dFullChId, 19);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 19);
                  break;
                case gdpbv100::GET4_V2X_ERR_UNKNOWN:
                  fhGet4ChanErrors->Fill(dFullChId, 20);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 20);
                  break;
                default:  // Corrupt error or not yet supported error
                  fhGet4ChanErrors->Fill(dFullChId, 21);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 21);
                  break;
              }  // Switch( mess.getGdpbSysErrData() )
            }  // if( gdpbv100::SYSMSG_GET4_EVENT == mess.getGdpbSysSubType() )
            if (gdpbv100::SYS_PATTERN == mess.getGdpbSysSubType()) {
              fhGdpbSysMessPattType->Fill(mess.getGdpbSysPattType(), fuGdpbNr);
            }  // if( gdpbv100::SYS_PATTERN == mess.getGdpbSysSubType() )
            break;
          }  // case gdpbv100::MSG_SYST:
          case gdpbv100::MSG_STAR_TRI_A:
          case gdpbv100::MSG_STAR_TRI_B:
          case gdpbv100::MSG_STAR_TRI_C:
          case gdpbv100::MSG_STAR_TRI_D:
            fhGet4MessType->Fill(fuGet4Nr, 5);
            fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 4);
            break;
          default:
            LOG(error) << "Message type " << std::hex << std::setw(2)
                       << static_cast<uint16_t>(messageType)
                       << " not included in Get4 unpacker.";
        }  // switch( mess.getMessageType() )
      }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
    }  // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )

    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      fhEpochsPerMs_gDPB[uGdpb]->Fill(fvuGdpbNbEpochPerMs[uGdpb]);
      fhEpochsPerMsPerTs_gDPB[uGdpb]->Fill(uTsIndexHeader,
                                           fvuGdpbNbEpochPerMs[uGdpb]);
      fhHitsPerMsFirstChan_gDPB[uGdpb]->Fill(uTsIndexHeader,
                                             fvvuChanNbHitsPerMs[uGdpb][0]);
      for (UInt_t uChan = 0; uChan < fuNrOfChannelsPerGdpb; ++uChan) {
        fvhChannelRatePerMs_gDPB[uGdpb]->Fill(
          uTsIndexHeader, uChan, 1.0 * fvvuChanNbHitsPerMs[uGdpb][uChan]);
        fvvuChanNbHitsPerMs[uGdpb][uChan] = 0;
      }  // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerGdpb; ++uChan )
      fvuGdpbNbEpochPerMs[uGdpb] = 0;
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  }    // for( UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx ++ )

  fulCurrentTsIndex++;

  return kTRUE;
}

void CbmCheckDataFormatGdpb2018::Reset() {}

void CbmCheckDataFormatGdpb2018::Finish() { SaveAllHistos(fsHistoFilename); }

void CbmCheckDataFormatGdpb2018::SaveAllHistos(TString sFileName) {
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile   = NULL;
  if ("" != sFileName) {
    // open separate histo file in recreate mode
    histoFile = new TFile(sFileName, "RECREATE");
    histoFile->cd();
  }  // if( "" != sFileName )

  gDirectory->mkdir("Tof_Raw_gDPB");
  gDirectory->cd("Tof_Raw_gDPB");

  fhMessType->Write();
  fhSysMessType->Write();
  fhGet4MessType->Write();
  fhGet4ChanScm->Write();
  fhGet4ChanErrors->Write();
  fhGet4EpochFlags->Write();

  fhGdpbMessType->Write();
  fhGdpbSysMessType->Write();
  fhGdpbSysMessPattType->Write();
  fhGdpbEpochFlags->Write();
  fhGdpbEpochSyncEvo->Write();
  fhGdpbEpochMissEvo->Write();

  fhPatternMissmatch->Write();
  fhPatternEnable->Write();
  fhPatternResync->Write();

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhGdpbGet4MessType[uGdpb]->Write();
    fvhGdpbGet4ChanErrors[uGdpb]->Write();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )


  gDirectory->mkdir("Tof_Ep_FineCount");
  gDirectory->cd("Tof_Ep_FineCount");
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fhEpochsPerMs_gDPB[uGdpb]->Write();
    fhEpochsPerMsPerTs_gDPB[uGdpb]->Write();
    fhEpochsDiff_gDPB[uGdpb]->Write();
    fhEpochsDiffPerTs_gDPB[uGdpb]->Write();
    fhEpochsJumpBitsPre_gDPB[uGdpb]->Write();
    fhEpochsJumpBitsNew_gDPB[uGdpb]->Write();
    fhEpochsJumpDigitsPre_gDPB[uGdpb]->Write();
    fhEpochsJumpDigitsNew_gDPB[uGdpb]->Write();
    fhStartEpochPerMs_gDPB[uGdpb]->Write();
    fhCloseEpochPerMs_gDPB[uGdpb]->Write();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  gDirectory->cd("..");

  gDirectory->mkdir("Tof_Chan_FineCount");
  gDirectory->cd("Tof_Chan_FineCount");
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fhHitsPerMsFirstChan_gDPB[uGdpb]->Write();
    fvhChannelRatePerMs_gDPB[uGdpb]->Write();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  gDirectory->cd("..");

  gDirectory->mkdir("Flib_Raw");
  gDirectory->cd("Flib_Raw");
  for (UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++) {
    if (NULL == fvhMsSzPerLink[uLinks]) continue;

    fvhMsSzPerLink[uLinks]->Write();
    fvhMsSzTimePerLink[uLinks]->Write();
  }  // for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )

  TH1* pMissedTsH1 = dynamic_cast<TH1*>(gROOT->FindObjectAny("Missed_TS"));
  if (NULL != pMissedTsH1) pMissedTsH1->Write();

  TProfile* pMissedTsEvoP =
    dynamic_cast<TProfile*>(gROOT->FindObjectAny("Missed_TS_Evo"));
  if (NULL != pMissedTsEvoP) pMissedTsEvoP->Write();

  gDirectory->cd("..");

  gDirectory->mkdir("canvases");
  gDirectory->cd("canvases");
  fcSummary->Write();
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fcFormatGdpb[uGdpb]->Write();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  gDirectory->cd("..");

  if ("" != sFileName) {
    // Restore original directory position
    histoFile->Close();
    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;
  }  // if( "" != sFileName )
}

void CbmCheckDataFormatGdpb2018::ResetAllHistos() {
  LOG(info) << "Reseting all TOF histograms.";

  fhMessType->Reset();
  fhSysMessType->Reset();
  fhGet4MessType->Reset();
  fhGet4ChanScm->Reset();
  fhGet4ChanErrors->Reset();
  fhGet4EpochFlags->Reset();

  fhGdpbMessType->Reset();
  fhGdpbSysMessType->Reset();
  fhGdpbSysMessPattType->Reset();
  fhGdpbEpochFlags->Reset();
  fhGdpbEpochSyncEvo->Reset();
  fhGdpbEpochMissEvo->Reset();

  fhPatternMissmatch->Reset();
  fhPatternEnable->Reset();
  fhPatternResync->Reset();


  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhGdpbGet4MessType[uGdpb]->Reset();
    fvhGdpbGet4ChanErrors[uGdpb]->Reset();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fhEpochsPerMs_gDPB[uGdpb]->Reset();
    fhEpochsPerMsPerTs_gDPB[uGdpb]->Reset();
    fhEpochsDiff_gDPB[uGdpb]->Reset();
    fhEpochsDiffPerTs_gDPB[uGdpb]->Reset();
    fhEpochsJumpBitsPre_gDPB[uGdpb]->Reset();
    fhEpochsJumpBitsNew_gDPB[uGdpb]->Reset();
    fhEpochsJumpDigitsPre_gDPB[uGdpb]->Reset();
    fhEpochsJumpDigitsNew_gDPB[uGdpb]->Reset();
    fhStartEpochPerMs_gDPB[uGdpb]->Reset();
    fhCloseEpochPerMs_gDPB[uGdpb]->Reset();
    fhHitsPerMsFirstChan_gDPB[uGdpb]->Reset();
    fvhChannelRatePerMs_gDPB[uGdpb]->Reset();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  for (UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++) {
    if (NULL == fvhMsSzPerLink[uLinks]) continue;

    fvhMsSzPerLink[uLinks]->Reset();
    fvhMsSzTimePerLink[uLinks]->Reset();
  }  // for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )

  fdStartTimeMsSz = -1;
}
ClassImp(CbmCheckDataFormatGdpb2018)
