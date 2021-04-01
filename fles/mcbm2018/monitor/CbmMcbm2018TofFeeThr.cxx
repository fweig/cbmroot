// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmMcbm2018TofFeeThr                          -----
// -----               Created 10.07.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018TofFeeThr.h"

#include "CbmFormatMsHeaderPrintout.h"
#include "CbmHistManager.h"
#include "CbmMcbm2018TofPar.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

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

Bool_t bMcbm2018TofFeeThrResetHistos = kFALSE;
Bool_t bMcbm2018TofFeeThrSaveHistos  = kFALSE;

CbmMcbm2018TofFeeThr::CbmMcbm2018TofFeeThr()
  : CbmMcbmUnpack()
  , fvMsComponentsList()
  , fuNbCoreMsPerTs(0)
  , fuNbOverMsPerTs(0)
  , fbIgnoreOverlapMs(kFALSE)
  , fsHistoFileFullname("data/TofPulserHistos.root")
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
  , fulCurrentTsIndex(0)
  , fuCurrentMs(0)
  , fuCurrentMsSysId(0)
  , fdMsIndex(0)
  , fuGdpbId(0)
  , fuGdpbNr(0)
  , fuGet4Id(0)
  , fuGet4Nr(0)
  , fiEquipmentId(0)
  , fviMsgCounter(11, 0)
  ,  // length of enum MessageTypes initialized with 0
  fvuPadiToGet4()
  , fvuGet4ToPadi()
  , fvuElinkToGet4()
  , fvuGet4ToElink()
  , fvdPadiThrCodeToValue() {}

CbmMcbm2018TofFeeThr::~CbmMcbm2018TofFeeThr() {}

Bool_t CbmMcbm2018TofFeeThr::Init() {
  LOG(info) << "Initializing Get4 monitor";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }  // if( ioman == NULL )

  return kTRUE;
}

void CbmMcbm2018TofFeeThr::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar =
    (CbmMcbm2018TofPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer(
      "CbmMcbm2018TofPar"));
}

Bool_t CbmMcbm2018TofFeeThr::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();
  Bool_t initOK = ReInitContainers();

  CreateHistograms();

  return initOK;
}

Bool_t CbmMcbm2018TofFeeThr::ReInitContainers() {
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

  /// TODO: move these constants somewhere shared, e.g the parameter file
  fvuPadiToGet4.resize(fuNrOfChannelsPerFee);
  fvuGet4ToPadi.resize(fuNrOfChannelsPerFee);
  /*
   UInt_t uGet4topadi[32] = {
        4,  3,  2,  1,  // provided by Jochen
      24, 23, 22, 21,
       8,  7,  6,  5,
      28, 27, 26, 25,
      12, 11, 10,  9,
      32, 31, 30, 29,
      16, 15, 14, 13,
      20, 19, 18, 17 };
*/
  /// From NH files, for Fall 2018 detectors
  UInt_t uGet4topadi[32] = {4,  3,  2,  1,  // provided by Jochen
                            8,  7,  6,  5,  12, 11, 10, 9,  16, 15,
                            14, 13, 20, 19, 18, 17, 24, 23, 22, 21,
                            28, 27, 26, 25, 32, 31, 30, 29};

  UInt_t uPaditoget4[32] = {4,  3,  2,  1,  // provided by Jochen
                            12, 11, 10, 9,  20, 19, 18, 17, 28, 27,
                            26, 25, 32, 31, 30, 29, 8,  7,  6,  5,
                            16, 15, 14, 13, 24, 23, 22, 21};

  for (UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan) {
    fvuPadiToGet4[uChan] = uPaditoget4[uChan] - 1;
    fvuGet4ToPadi[uChan] = uGet4topadi[uChan] - 1;
  }  // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan )


  /// TODO: move these constants somewhere shared, e.g the parameter file
  fvuElinkToGet4.resize(kuNbGet4PerGbtx);
  fvuGet4ToElink.resize(kuNbGet4PerGbtx);
  UInt_t kuElinkToGet4[kuNbGet4PerGbtx] = {
    27, 2,  7,  3,  31, 26, 30, 1,  33, 37, 32, 13, 9,  14,
    10, 15, 17, 21, 16, 35, 34, 38, 25, 24, 0,  6,  20, 23,
    18, 22, 28, 4,  29, 5,  19, 36, 39, 8,  12, 11};
  UInt_t kuGet4ToElink[kuNbGet4PerGbtx] = {
    24, 7,  1,  3,  31, 33, 25, 2,  37, 12, 14, 39, 38, 11,
    13, 15, 18, 16, 28, 34, 26, 17, 29, 27, 23, 22, 5,  0,
    30, 32, 6,  4,  10, 8,  20, 19, 35, 9,  21, 36};

  for (UInt_t uLinkAsic = 0; uLinkAsic < kuNbGet4PerGbtx; ++uLinkAsic) {
    fvuElinkToGet4[uLinkAsic] = kuElinkToGet4[uLinkAsic];
    fvuGet4ToElink[uLinkAsic] = kuGet4ToElink[uLinkAsic];
  }  // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan )

  /// PADI threshold measures and extrapolated code to value map
  const UInt_t kuNbThrMeasPoints          = 65;
  UInt_t kuThrMeasCode[kuNbThrMeasPoints] = {
    0x000, 0x010, 0x020, 0x030, 0x040, 0x050, 0x060, 0x070, 0x080, 0x090, 0x0A0,
    0x0B0, 0x0C0, 0x0D0, 0x0E0, 0x0F0, 0x100, 0x110, 0x120, 0x130, 0x140, 0x150,
    0x160, 0x170, 0x180, 0x190, 0x1A0, 0x1B0, 0x1C0, 0x1D0, 0x1E0, 0x1F0, 0x200,
    0x210, 0x220, 0x230, 0x240, 0x250, 0x260, 0x270, 0x280, 0x290, 0x2A0, 0x2B0,
    0x2C0, 0x2D0, 0x2E0, 0x2F0, 0x300, 0x310, 0x320, 0x330, 0x340, 0x350, 0x360,
    0x370, 0x380, 0x390, 0x3A0, 0x3B0, 0x3C0, 0x3D0, 0x3E0, 0x3F0, 0x3FF};
  Double_t kdThrMeasVal[kuNbThrMeasPoints] = {
    -652.6, -631.2, -611.4, -590.6, -570.9, -550.0, -529.9, -509.4, -490.6,
    -469.5, -449.3, -428.5, -408.5, -388.2, -367.8, -347.2, -329.2, -308.2,
    -287.5, -266.8, -246.9, -226.0, -205.6, -185.0, -165.7, -144.9, -124.4,
    -103.8, -83.4,  -62.9,  -42.4,  -21.2,  -5.3,   15.5,   36.2,   56.8,
    77.3,   97.8,   118.4,  139.1,  158.7,  179.2,  199.7,  220.2,  240.8,
    261.1,  281.7,  302.2,  321.3,  341.4,  362.0,  382.2,  402.9,  422.8,
    443.4,  463.7,  483.7,  503.7,  524.1,  544.3,  565.0,  585.0,  605.5,
    626.0,  646.1};

  fvdPadiThrCodeToValue.resize(0x3FF + 1, 0.0);
  for (UInt_t uPadiPoint = 0; uPadiPoint < kuNbThrMeasPoints; ++uPadiPoint) {
    fvdPadiThrCodeToValue[kuThrMeasCode[uPadiPoint]] = kdThrMeasVal[uPadiPoint];

    /// Linear extrapolation between measured points
    if (uPadiPoint + 1 < kuNbThrMeasPoints) {
      UInt_t uNbSteps =
        kuThrMeasCode[uPadiPoint + 1] - kuThrMeasCode[uPadiPoint];
      Double_t dValStep =
        (kdThrMeasVal[uPadiPoint + 1] - kdThrMeasVal[uPadiPoint]) / uNbSteps;
      UInt_t uCode = kuThrMeasCode[uPadiPoint];
      for (UInt_t uStep = 1; uStep < uNbSteps; ++uStep) {
        uCode++;
        fvdPadiThrCodeToValue[uCode] =
          kdThrMeasVal[uPadiPoint] + dValStep * uStep;
      }  // for( UInt_t uStep = 1; uStep < uNbSteps; ++uStep)
    }    // if( uPadiPoint + 1 < kuNbThrMeasPoints )
  }  // for( UInt_t uPadiPoint = 0; uPadiPoint < kuNbThrMeasPoints; ++uPadiPoint )


  return kTRUE;
}


void CbmMcbm2018TofFeeThr::AddMsComponentToList(size_t component,
                                                UShort_t /*usDetectorId*/) {
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);
}
void CbmMcbm2018TofFeeThr::SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb) {
  fuNbCoreMsPerTs = uCoreMsNb;
  fuNbOverMsPerTs = uOverlapMsNb;

  //   UInt_t uNbMsTotal = fuNbCoreMsPerTs + fuNbOverMsPerTs;
}

void CbmMcbm2018TofFeeThr::CreateHistograms() {
  LOG(info) << "create Histos for " << fuNrOfGdpbs << " gDPBs ";

  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  fhGdpbAsicSpiCounts = new TH2I(
    "hGdpbAsicSpiCounts",
    "SPI messages count per GDPB and ASIC; ASIC Idx []; GDPB []; SPI msg[]",
    fuNrOfGet4PerGdpb,
    -0.5,
    fuNrOfGet4PerGdpb - 0.5,
    fuNrOfGdpbs,
    -0.5,
    fuNrOfGdpbs - 0.5);

  if (server) {
    server->Register("/", fhGdpbAsicSpiCounts);
    server->RegisterCommand("/Reset_All_eTOF",
                            "bMcbm2018TofFeeThrResetHistos=kTRUE");
    server->RegisterCommand("/Save_All_eTof",
                            "bMcbm2018TofFeeThrSaveHistos=kTRUE");

    server->Restrict("/Reset_All_eTof", "allow=admin");
    server->Restrict("/Save_All_eTof", "allow=admin");
  }  // if( server )

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmMcbm2018TofFeeThr::DoUnpack(const fles::Timeslice& ts,
                                      size_t component) {
  if (bMcbm2018TofFeeThrResetHistos) {
    LOG(info) << "Reset eTOF STAR histos ";
    ResetAllHistos();
    bMcbm2018TofFeeThrResetHistos = kFALSE;
  }  // if( bMcbm2018TofFeeThrResetHistos )
  if (bMcbm2018TofFeeThrSaveHistos) {
    LOG(info) << "Start saving eTOF STAR histos ";
    SaveAllHistos("data/histos_Shift_StarTof.root");
    bMcbm2018TofFeeThrSaveHistos = kFALSE;
  }  // if( bSaveStsHistos )


  LOG(debug1) << "Timeslice contains " << ts.num_microslices(component)
              << "microslices.";

  /// Ignore overlap ms if flag set by user
  UInt_t uNbMsLoop = fuNbCoreMsPerTs;
  if (kFALSE == fbIgnoreOverlapMs) uNbMsLoop += fuNbOverMsPerTs;

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
        LOG(warning) << "Could not find the gDPB index for AFCK id 0x"
                     << std::hex << fuGdpbId << std::dec << " in timeslice "
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
        if (0 == uIdx) { continue; }  // if( 0 == uIdx )

        gdpbv100::Message mess(ulData);

        // Increment counter for different message types
        // and fill the corresponding histogram
        messageType = mess.getMessageType();
        fviMsgCounter[messageType]++;

        ///         fuGet4Id = mess.getGdpbGenChipId();
        fuGet4Id = ConvertElinkToGet4(mess.getGdpbGenChipId());
        /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
        if (0x90 == fuCurrentMsSysId) fuGet4Id = mess.getGdpbGenChipId();
        fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;

        if (fuNrOfGet4PerGdpb <= fuGet4Id && !mess.isStarTrigger()
            && (gdpbv100::kuChipIdMergedEpoch != fuGet4Id))
          LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id
                       << " VS " << fuNrOfGet4PerGdpb << " set in parameters.";

        switch (messageType) {
          case gdpbv100::MSG_HIT:
          case gdpbv100::MSG_EPOCH: break;
          case gdpbv100::MSG_SLOWC: {
            PrintSlcInfo(mess);
            break;
          }  // case gdpbv100::MSG_SLOWC:
          case gdpbv100::MSG_SYST:
          case gdpbv100::MSG_STAR_TRI_A:
          case gdpbv100::MSG_STAR_TRI_B:
          case gdpbv100::MSG_STAR_TRI_C:
          case gdpbv100::MSG_STAR_TRI_D: break;
          default:
            LOG(error) << "Message type " << std::hex << std::setw(2)
                       << static_cast<uint16_t>(messageType)
                       << " not included in Get4 unpacker.";
        }  // switch( mess.getMessageType() )
      }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
    }  // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )
  }    // for( UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx ++ )


  fulCurrentTsIndex++;

  return kTRUE;
}

void CbmMcbm2018TofFeeThr::PrintSlcInfo(gdpbv100::Message mess) {
  if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fuGdpbId)) {
    UInt_t uChan = mess.getGdpbSlcChan();
    UInt_t uEdge = mess.getGdpbSlcEdge();
    UInt_t uData = mess.getGdpbSlcData();
    UInt_t uType = mess.getGdpbSlcType();

    //      Double_t dGdpbChId =  fuGet4Id * fuNrOfChannelsPerGet4 + mess.getGdpbSlcChan() + 0.5 * mess.getGdpbSlcEdge();
    //      Double_t dFullChId =  fuGet4Nr * fuNrOfChannelsPerGet4 + mess.getGdpbSlcChan() + 0.5 * mess.getGdpbSlcEdge();
    Double_t dMessTime = fdMsIndex * 1e-9;

    /// Printout if SPI message!
    if (gdpbv100::GET4_32B_SLC_SPIREAD == uType) {
      fhGdpbAsicSpiCounts->Fill(fuGet4Id, fuGdpbNr);
      LOG(info) << "GET4 Slow Control message, time "
                << Form("%3.3f", dMessTime) << " s "
                << " for board ID " << std::hex << std::setw(4) << fuGdpbId
                << std::dec << "\n"
                << " +++++++ > Chip = " << std::setw(3) << fuGet4Id
                << ", Chan = " << std::setw(1) << uChan
                << ", Edge = " << std::setw(1) << uEdge
                << ", Type = " << std::setw(1) << mess.getGdpbSlcType() << ", "
                << Form("channel  %1u,", (uData >> 10) & 0xF)
                << Form("value 0x%03x ", uData & 0x3FF)
                << Form("level %4.1f ", fvdPadiThrCodeToValue[uData & 0x3FF])
                << Form("(Data = 0x%06x) ", uData);
    }  // if( gdpbv100::GET4_32B_SLC_SPIREAD == uType )
  }
}

void CbmMcbm2018TofFeeThr::Reset() {}

void CbmMcbm2018TofFeeThr::Finish() {
  // Printout some stats on what was unpacked
  TString message_type;
  for (unsigned int i = 0; i < fviMsgCounter.size(); ++i) {
    switch (i) {
      case 0: message_type = "NOP"; break;
      case 1: message_type = "HIT"; break;
      case 2: message_type = "EPOCH"; break;
      case 3: message_type = "SYNC"; break;
      case 4: message_type = "AUX"; break;
      case 5: message_type = "EPOCH2"; break;
      case 6: message_type = "GET4"; break;
      case 7: message_type = "SYS"; break;
      case 8: message_type = "GET4_SLC"; break;
      case 9: message_type = "GET4_32B"; break;
      case 10: message_type = "GET4_SYS"; break;
      default: message_type = "UNKNOWN"; break;
    }  // switch(i)
    LOG(info) << message_type << " messages: " << fviMsgCounter[i];
  }  // for (unsigned int i=0; i< fviMsgCounter.size(); ++i)

  SaveAllHistos(fsHistoFileFullname);
  //   SaveAllHistos();
}

void CbmMcbm2018TofFeeThr::SaveAllHistos(TString sFileName) {
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile   = NULL;
  if ("" != sFileName) {
    // open separate histo file in recreate mode
    histoFile = new TFile(sFileName, "RECREATE");
    histoFile->cd();
  }  // if( "" != sFileName )

  if ("" != sFileName) {
    // Restore original directory position
    histoFile->Close();
  }  // if( "" != sFileName )

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmMcbm2018TofFeeThr::ResetAllHistos() {
  LOG(info) << "Reseting all TOF histograms.";
  fhGdpbAsicSpiCounts->Reset();
}
ClassImp(CbmMcbm2018TofFeeThr)
