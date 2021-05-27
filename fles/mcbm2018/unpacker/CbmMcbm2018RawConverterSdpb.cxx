// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018RawConverterSdpb                   -----
// -----               Created 28.06.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018RawConverterSdpb.h"

#include "CbmFormatMsHeaderPrintout.h"
#include "CbmMcbm2018StsPar.h"
#include "CbmStsDigi.h"

#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include <iomanip>

CbmMcbm2018RawConverterSdpb::CbmMcbm2018RawConverterSdpb(UInt_t /*uNbGdpb*/)
  : CbmMcbmUnpack()
  , fParCList(nullptr)
  , fUnpackPar(nullptr)
  , fuNrOfDpbs(0)
  , fDpbIdIndexMap()
  , fvbMaskedComponents()
  , fvMsComponentsList()
  , fuNbCoreMsPerTs(0)
  , fuNbOverMsPerTs(0)
  , fuNbMsLoop(0)
  , fbIgnoreOverlapMs(kFALSE)
  , fdMsSizeInNs(-1.0)
  , fdTsCoreSizeInNs(-1.0)
  , fulCurrentTsIdx(0)
  , fulCurrentMsIdx(0)
  , fdTsStartTime(0.0)
  , fdTsStopTimeCore(0.0)
  , fdMsTime(0.0)
  , fuMsIndex(0)
  , fuCurrentEquipmentId(0)
  , fuCurrDpbId(0)
  , fuCurrDpbIdx(0)
  , fvSdpbMessages()
{
}

CbmMcbm2018RawConverterSdpb::~CbmMcbm2018RawConverterSdpb() {}

Bool_t CbmMcbm2018RawConverterSdpb::Init()
{
  LOG(info) << "CbmMcbm2018RawConverterSdpb::Init";
  LOG(info) << "Initializing mCBM sDPB 2018 Raw Messages Converter";

  return kTRUE;
}

void CbmMcbm2018RawConverterSdpb::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();

  if (nullptr == fParCList) fParCList = new TList();
  fUnpackPar = new CbmMcbm2018StsPar("CbmMcbm2018StsPar");
  fParCList->Add(fUnpackPar);

  for (Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);

    std::string sParamName {tempObj->GetName()};
    FairParGenericSet* newObj =
      dynamic_cast<FairParGenericSet*>(FairRun::Instance()->GetRuntimeDb()->getContainer(sParamName.data()));

    if (nullptr == newObj) {
      LOG(error) << "Failed to obtain parameter container " << sParamName << ", for parameter index " << iparC;
      return;
    }  // if( nullptr == newObj )

    fParCList->AddAt(newObj, iparC);
    //      delete tempObj;
  }  // for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
}

Bool_t CbmMcbm2018RawConverterSdpb::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  fUnpackPar = dynamic_cast<CbmMcbm2018StsPar*>(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018StsPar"));
  if (nullptr == fUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmMcbm2018StsPar";
    return kFALSE;
  }  // if( nullptr == fUnpackPar )

  fuNrOfDpbs = fUnpackPar->GetNrOfDpbs();
  LOG(info) << "Nr. of STS DPBs:       " << fuNrOfDpbs;

  fDpbIdIndexMap.clear();
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    fDpbIdIndexMap[fUnpackPar->GetDpbId(uDpb)] = uDpb;
    LOG(info) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = 0x" << std::setw(4) << std::hex
              << fUnpackPar->GetDpbId(uDpb) << std::dec << " => " << fDpbIdIndexMap[fUnpackPar->GetDpbId(uDpb)];
  }  // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  /// Initialize and register output arrays with the FairRoot IO manager
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { LOG(fatal) << "No FairRootManager instance"; }

  fvSdpbMessages.resize(fuNrOfDpbs);
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    fvSdpbMessages[uDpb] = new std::vector<stsxyter::Message>;
    if (NULL == fvSdpbMessages[uDpb]) {
      LOG(fatal) << "Failed creating the sDPB messages vector ";
    }  // if( NULL == fvSdpbMessages[ uDpb ] )
    ioman->RegisterAny(Form("CbmSdpbMsg%03u", uDpb), fvSdpbMessages[uDpb], kTRUE);
  }  // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  return kTRUE;
}

Bool_t CbmMcbm2018RawConverterSdpb::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();

  return kTRUE;
}

void CbmMcbm2018RawConverterSdpb::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  LOG(info) << "CbmMcbm2018RawConverterSdpb::AddMsComponentToList => Component " << component << " with detector ID 0x"
            << std::hex << usDetectorId << std::dec << " added to list";
}

Bool_t CbmMcbm2018RawConverterSdpb::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  fulCurrentTsIdx = ts.index();
  fdTsStartTime   = static_cast<Double_t>(ts.descriptor(0, 0).idx);

  /// Ignore First TS as first MS is typically corrupt
  if (0 == fulCurrentTsIdx) { return kTRUE; }  // if( 0 == fulCurrentTsIdx )

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsFullSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs + fuNbOverMsPerTs);
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a core duration of " << fdTsCoreSizeInNs << " ns and a full duration of "
              << fdTsFullSizeInNs << " ns";

    /// Ignore overlap ms if flag set by user
    fuNbMsLoop = fuNbCoreMsPerTs;
    if (kFALSE == fbIgnoreOverlapMs) fuNbMsLoop += fuNbOverMsPerTs;
    LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
  }  // if( -1.0 == fdTsCoreSizeInNs )

  /// Compute time of TS core end
  fdTsStopTimeCore = fdTsStartTime + fdTsCoreSizeInNs;

  /// Loop over registered components
  for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx) {
    /// Loop over core microslices (and overlap ones if chosen)
    for (fuMsIndex = 0; fuMsIndex < fuNbMsLoop; fuMsIndex++) {
      UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];

      auto msDescriptor        = ts.descriptor(uMsComp, fuMsIndex);
      fuCurrentEquipmentId     = msDescriptor.eq_id;
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsComp, fuMsIndex));

      uint32_t uSize  = msDescriptor.size;
      fulCurrentMsIdx = msDescriptor.idx;
      //         Double_t dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
      LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
                 << " has size: " << uSize;

      if (0 == fvbMaskedComponents.size()) fvbMaskedComponents.resize(ts.num_components(), kFALSE);

      fuCurrDpbId = static_cast<uint32_t>(fuCurrentEquipmentId & 0xFFFF);

      /// Check if this sDPB ID was declared in parameter file and stop there if not
      auto it = fDpbIdIndexMap.find(fuCurrDpbId);
      if (it == fDpbIdIndexMap.end()) {
        if (kFALSE == fvbMaskedComponents[uMsCompIdx]) {
          LOG(info) << "-------------------------------------------------------"
                       "--------";
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
          LOG(warning) << "Could not find the sDPB index for AFCK id 0x" << std::hex << fuCurrDpbId << std::dec
                       << " in timeslice " << fulCurrentTsIdx << " in microslice " << fuMsIndex << " component "
                       << uMsCompIdx << "\n"
                       << "If valid this index has to be added in the STS "
                          "parameter file in the DbpIdArray field";
          fvbMaskedComponents[uMsCompIdx] = kTRUE;

          /// If first TS being analyzed, we are probably detecting STS/MUCH boards with same sysid
          /// => Do not report the MS as bad, just ignore it
          if (1 == fulCurrentTsIdx) return kTRUE;
        }  // if( kFALSE == fvbMaskedComponents[ uMsComp ] )
        else
          return kTRUE;

        return kFALSE;
      }  // if( it == fDpbIdIndexMap.end() )
      else
        fuCurrDpbIdx = fDpbIdIndexMap[fuCurrDpbId];

      /// If not integer number of message in input buffer, print warning/error
      if (0 != (uSize % kuBytesPerMessage))
        LOG(error) << "The input microslice buffer does NOT "
                   << "contain only complete sDPB messages!";

      /// Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages = (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

      /// Prepare variables for the loop on contents
      const uint32_t* pInBuff = reinterpret_cast<const uint32_t*>(msContent);
      for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
        /// Fill message
        uint32_t ulData = static_cast<uint32_t>(pInBuff[uIdx]);
        stsxyter::Message mess(static_cast<uint32_t>(ulData & 0xFFFFFFFF));

        /// Store the message in the output vector
        /// ==> This message is not context free and needs previous ones to be interpreted!!!
        fvSdpbMessages[fuCurrDpbIdx]->push_back(mess);
      }  // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
    }    // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )
  }      // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )

  if (0 == fulCurrentTsIdx % 10000) LOG(info) << "Processed TS " << fulCurrentTsIdx;

  return kTRUE;
}

void CbmMcbm2018RawConverterSdpb::Reset()
{
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb)
    fvSdpbMessages[uDpb]->clear();
}

void CbmMcbm2018RawConverterSdpb::Finish() {}

ClassImp(CbmMcbm2018RawConverterSdpb)
