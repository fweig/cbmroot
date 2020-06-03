#include "CbmMcbm2018UnpackerAlgoTrdR.h"
#include "CbmTrdHardwareSetupR.h"
#include "CbmTrdParSpadic.h"
#include "CbmTrdAddress.h"

#include "FairLogger.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairParAsciiFileIo.h"

#include "TObjString.h"
#include "TSystem.h"

#include <iostream>
#include <map>

CbmMcbm2018UnpackerAlgoTrdR::CbmMcbm2018UnpackerAlgoTrdR()
    : CbmStar2019Algo(),
      //fRawToDigi( new CbmTrdRawToDigiR ),
      fdMsSizeInCC(0),
      fbMonitorMode(kFALSE),
      fbDebugMonitorMode(kFALSE),
      fbWriteOutput(kTRUE),
      fbDebugWriteOutput(kFALSE),
      fbBaselineAvg(kFALSE),
      fTrdDigiVector(nullptr),
      fTrdRawMessageVector(nullptr),
      fSpadicInfoMsgVector(nullptr),
      //fHistoMap(),
      fHistoArray(),
      fNbTimeslices(0),
      fCurrTsIdx(0),
      fMsIndex(0),
      fTsStartTime(0.0),
      fTsStopTimeCore(0.0),
      fMsTime(0.0),
      fSpadicEpoch(0),
      fLastFulltime(0),
      fNbSpadicRawMsg(0),
      fNbWildRda(0),
      fNbSpadicErrorMsg(0),
      fNbUnkownWord(0),
      fNbSpadicEpochMsg(0)
      , fParContList(nullptr)
      , fRefGeoTag("trd_v18q_mcbm")
      , fAsicPar(nullptr)
      , fDigiPar(nullptr)
      , fGasPar(nullptr)
      , fGainPar(nullptr)
      , fSpadicMap()
      , fAsicChannelMap()
      , fIsFirstChannelsElinkEven(false)
{
}

CbmMcbm2018UnpackerAlgoTrdR::~CbmMcbm2018UnpackerAlgoTrdR()
{
    if (fTrdRawMessageVector != nullptr)
    {
        delete fTrdRawMessageVector;
    }
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::Init()
{
    LOG(debug) << "Initializing CbmMcbm2018UnpackerAlgoTrdR";
    //fRawToDigi->Init();
    fbIgnoreOverlapMs = kTRUE ;
    fdMsSizeInNs = 1.28e6; //FIXME time should come from parameter file
    fdMsSizeInCC = fdMsSizeInNs / 62.5;

    return kTRUE ;
}

void CbmMcbm2018UnpackerAlgoTrdR::Reset()
{
    if(fTrdDigiVector)          fTrdDigiVector->clear();
    if(fTrdRawMessageVector)    fTrdRawMessageVector->clear();
}

void CbmMcbm2018UnpackerAlgoTrdR::Finish()
{
    LOG(info) << "Finish of CbmMcbm2018UnpackerAlgoTrdR. Unpacked \n "
              << fNbTimeslices << " Timeslices with \n "
              << fNbSpadicRawMsg << " Spadic Raw Messages,\n "
              << fNbSpadicEpochMsg << " Spadic Epoch Messages,\n "
              << fNbSpadicErrorMsg << " Spadic Info Messages,\n "
              << fNbWildRda << " Unexpected RDA Words and\n "
              << fNbUnkownWord << " Unknown Words.";
}

// ---- InitContainers  ----------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoTrdR::InitContainers()
{
    LOG(debug) << "Initializing Containers of CbmMcbm2018UnpackerAlgoTrdR";

    Bool_t initOK = ReInitContainers();

    return initOK;

}

// ---- ReInitContainers  ----------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoTrdR::ReInitContainers()
{
    LOG(debug) << "(Re-)Initializing Containers of CbmMcbm2018UnpackerAlgoTrdR";

    Bool_t initOk = kTRUE;

    if( !(fAsicPar = (CbmTrdParSetAsic*) fParContList->FindObject("CbmTrdParSetAsic")) )
    {
        LOG(warning) << "CbmTrdParSetAsic not found";
        initOk = kFALSE;
    }
    if( !(fDigiPar = (CbmTrdParSetDigi*) fParContList->FindObject("CbmTrdParSetDigi")) )
    {
        LOG(warning) << "CbmTrdParSetDigi not found";
        initOk = kFALSE;
    }
    if( !(fGasPar = (CbmTrdParSetGas*) fParContList->FindObject("CbmTrdParSetGas")) )
    {
        LOG(warning) << "CbmTrdParSetGas not found";
        initOk = kFALSE;
    }
    if( !(fGainPar = (CbmTrdParSetGain*) fParContList->FindObject("CbmTrdParSetGain")) )
    {
        LOG(warning) << "CbmTrdParSetGain not found";
        initOk = kFALSE;
    }

    if(initOk) initOk = InitParameters();

    return initOk;

}

// ---- GetParList  ----------------------------------------------------
TList *CbmMcbm2018UnpackerAlgoTrdR::GetParList()
{
    if(fParContList)
    {
        return fParContList;
    }
    else // create list with default parameters for trd
    {
        fParContList = new TList();

        fAsicPar = new CbmTrdParSetAsic();
        fDigiPar = new CbmTrdParSetDigi();
        fGasPar = new CbmTrdParSetGas();
        fGainPar = new CbmTrdParSetGain();

        fParContList->Add(fAsicPar);
        fParContList->Add(fDigiPar);
        fParContList->Add(fGasPar);
        fParContList->Add(fGainPar);

        return fParContList;
    }
}

// ---- InitParameters ----------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoTrdR::InitParameters()
{
    Bool_t initOk = kTRUE;

    // Initialize the spadic address map
    CbmTrdHardwareSetupR hwSetup;
    fSpadicMap = hwSetup.CreateHwToSwAsicAddressTranslatorMap(true);
    initOk &= !(fSpadicMap.empty()); // at least check that the loaded map is not empty
    fAsicChannelMap = hwSetup.CreateAsicChannelMap(true);
    initOk &= !(fAsicChannelMap.empty()); // at least check that the loaded map is not empty
    if(initOk)  LOG(debug) << "CbmMcbm2018UnpackerAlgoTrdR - Successfully initialized Spadic hardware address map";
    return initOk;

}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::ProcessTs(const fles::Timeslice &ts)
{
    fCurrTsIdx = ts.index();
    fTsStartTime = static_cast<Double_t>(ts.descriptor(0, 0).idx);

    /// On first TS, extract the TS parameters from header (by definition stable over time).
    if (0 == fNbTimeslices)
    {
        fuNbCoreMsPerTs = ts.num_core_microslices();
        fuNbOverMsPerTs = ts.num_microslices(0) - ts.num_core_microslices();
        fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
        fdTsFullSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs + fuNbOverMsPerTs);
        LOG(info) << "CbmMcbm2018UnpackerAlgoTrdR::ProcessTs :";
        LOG(info) << "Timeslice parameters: each TS has "
                  << fuNbCoreMsPerTs << " Core MS and "
                  << fuNbOverMsPerTs << " Overlap MS, for a core duration of "
                  << fdTsCoreSizeInNs << " ns and a full duration of "
                  << fdTsFullSizeInNs << " ns";

        /// Ignore overlap ms if flag set by user
        fuNbMsLoop = fuNbCoreMsPerTs;
        if (kFALSE == fbIgnoreOverlapMs)
            fuNbMsLoop += fuNbOverMsPerTs;
        LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
    }

    /// Loop over core microslices (and overlap ones if chosen)
    for (UInt_t MsIndex = 0; MsIndex < fuNbMsLoop; MsIndex++)
    {
        /// Loop over registered components
        for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx)
        {
            UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];

            if (kFALSE == ProcessMs(ts, uMsComp, MsIndex))
            {
                LOG(error) << "Failed to process ts " << fCurrTsIdx << " MS " << MsIndex
                           << " for component " << uMsComp;
                return kFALSE;
            }
        }
    }

    /// Sort the output vector according to the time.
    std::sort(fTrdDigiVector->begin(), fTrdDigiVector->end(),
        [](const CbmTrdDigi & a, const CbmTrdDigi & b) -> bool
        {
          return a.GetTime() < b.GetTime();
        });

    fNbTimeslices++;
    return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::ProcessMs(const fles::Timeslice &ts, size_t uMsCompIdx, size_t uMsIdx)
{
    fles::MicrosliceDescriptor msDesc = ts.descriptor(uMsCompIdx, uMsIdx ) ;
//    uint16_t msEquipmentID = msDesc.eq_id;   ///< Equipment identifier. Specifies the FLES input link. #FU 27.03.20 unused
    uint32_t msSize = msDesc.size;     ///< Content size. This is the size (in bytes) of the microslice data content.
//    uint64_t msTime = msDesc.idx;      ///< Start time of the microslice in ns since global time zero. #FU 27.03.20 unused
    uint32_t msNbWords = ( msSize - (msSize % kBytesPerWord )) / kBytesPerWord ;   ///< Number of complete Words in the input MS buffer.

    const uint8_t * msPointer = reinterpret_cast<const uint8_t*>( ts.content( uMsCompIdx, uMsIdx ) );
    const uint64_t * msContent = reinterpret_cast<const uint64_t*>(msPointer);   ///< Spadic Messages are 64bit Words.

    /// Loop over all 64bit-Words in the current Microslice
    for (uint32_t iWord = 0; iWord < msNbWords; iWord++)
    {
        uint64_t curWord = static_cast<uint64_t>(msContent[iWord]);
        Spadic::MsMessageType wordType = GetMessageType(curWord);

        if (wordType == Spadic::MsMessageType::kSOM)
        {
            //LOG(info) << "New Spadic Message!" ; //debug
            CbmTrdRawMessageSpadic raw = CreateRawMessage(curWord, msDesc);
            uint8_t nSamples = raw.GetNrSamples();
            /// If the new Message has more than 3 Samples, we need to read in the next words that contain the remaining samples.
            if (nSamples > 3)
            {
                uint8_t nRda = GetNumRda(nSamples);
                uint8_t curSample = 3; // first 3 samples are in som
                /// Loop over the rda words
                for (uint8_t iRda = 0; iRda < nRda; iRda++)
                {
                    ++iWord;
                    curWord = static_cast<uint64_t>(msContent[(iWord)]);
                    if ( GetMessageType(curWord) != Spadic::MsMessageType::kRDA )
                    {
                        LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  Incomplete Spadic Message! RDA Word missing, Microslice corrupted." ;
                        return kFALSE ;
                    }
                    /// Loop over Samples. There are max 7 samples per word.
                    for ( uint8_t j=0 ; curSample<nSamples && curSample<32 && j<7 ; curSample++, j++ )
                    {
                        raw.SetSample( ExtractSample(curWord,curSample), curSample );
                    }
                }
            }
            fNbSpadicRawMsg++;

            /// Message should now be complete. TODO: Generate Digi and save raw message if needed.
            std::shared_ptr<CbmTrdDigi> digi = MakeDigi(raw);
            fTrdDigiVector->push_back(*digi);

            /// Save raw message:
            if ( fbDebugWriteOutput && (fTrdRawMessageVector != nullptr) )
            {
                fTrdRawMessageVector->push_back(raw) ;
            }


            /// Fill histograms:
            if (fbMonitorMode || fbDebugMonitorMode )
            {
                TString HistName = "RawMessage_Signalshape_all";
                for (unsigned int i = 0; i < raw.GetSamples().size(); i++)
                {
                    //fHistoMap.at(HistName.Data())->Fill(i, raw.GetSamples()[i]);
                    ((TH2I*)fHistoArray.FindObject(HistName.Data()))->Fill(i, raw.GetSamples()[i]) ;
                }

                if (raw.GetChannelId() == 9 && raw.GetElinkId() == 0 )//&& ( iWord >= msNbWords-10  || iWord <= 10 ) )
                {
                    HistName = "Delta_T";
                    Long64_t dt = ((Long64_t)raw.GetFullTime() - (Long64_t)fLastFulltime);
                    //if ( dt != 0 )
                    //if ( abs(dt) > 2048 ) LOG(warning) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  |dt| = " << dt ;
                    //if (dt<0)
                        //std::cout << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  dt = " << dt << std::endl ;
                    /*std::cout << raw.GetFullTime() << std::endl;
                    std::cout << fLastFulltime << std::endl; */
                    ((TH1I *)fHistoArray.FindObject(HistName.Data()))->Fill(dt);
                    fLastFulltime = raw.GetFullTime();
                }

                if (raw.GetHitType() < 2 && !raw.GetMultiHit())
                {
                    HistName = "RawMessage_Signalshape_filtered";
                    for (unsigned int i = 0; i < raw.GetSamples().size(); i++)
                    {
                        //fHistoMap.at(HistName.Data())->Fill(i, raw.GetSamples()[i]);
                        ((TH2I*)fHistoArray.FindObject(HistName.Data()))->Fill(i, raw.GetSamples()[i]) ;
                    }
                }
            }


        } // endif (wordType == kSOM )

        if ( wordType == Spadic::MsMessageType::kRDA )
        {
            LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  Unexpected RDA Word. Microslice corrupted." ;
            fNbWildRda++;
            return kFALSE ;
        }

        if ( wordType == Spadic::MsMessageType::kINF )
        {
            /// Save info message if needed.
            if (fbDebugWriteOutput && (fSpadicInfoMsgVector != nullptr))
            {
                fSpadicInfoMsgVector->push_back(std::make_pair(fLastFulltime, curWord));
            }
            fNbSpadicErrorMsg++;

            Spadic::MsInfoType infoType = GetInfoType(curWord);
            TString HistName = "Spadic_Info_Types";
            ((TH2I*)fHistoArray.FindObject(HistName.Data()))->Fill(fLastFulltime, (Int_t)infoType) ;

/* FU 270320 Only for debug purpose see Redmine issue #1653
            Int_t channel = 0;
            if ( infoType != Spadic::MsInfoType::kMIS )
            {
                uint64_t mask = 0x0F;
                channel = (Int_t)( curWord & mask );
            }

            uint8_t elink = 0;
            uint64_t mask = 0x3F ;
            elink = (uint8_t)((curWord >> 54 ) & mask );
*/
        }

        if ( wordType == Spadic::MsMessageType::kNUL )
        {
            if ( iWord != (msNbWords-1) ) // last word in Microslice is 0.
            {
                LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  Null Word but not at end of Microslice." ;
            }
        }
        if ( wordType == Spadic::MsMessageType::kUNK )
        {
            LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  Unknown Word. Microslice corrupted." ;
            fNbUnkownWord++;
            //return kFALSE ;
        }

        if (wordType == Spadic::MsMessageType::kEPO)
        {
            uint64_t mask = 0x3FFFFFFF;
            mask = mask << 32;
            uint64_t uTS_MSB = (uint64_t)((curWord & mask) >> 32);
            Long64_t dt_epoch = uTS_MSB - fSpadicEpoch;
            if ( dt_epoch != 1 ) LOG(warning) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  dt_epoch = " << dt_epoch ;

            //fLastFulltime = uTS_MSB;
            fNbSpadicEpochMsg++;
            fSpadicEpoch = uTS_MSB;
        }

    } // end for (uint32_t iWord = 0; iWord < msNbWords; iWord++)

    return kTRUE;
}

void CbmMcbm2018UnpackerAlgoTrdR::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
    /// Check for duplicates and ignore if it is the case.
    for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
        if (component == fvMsComponentsList[uCompIdx])
            return;

    /// Add to lis
    fvMsComponentsList.push_back(component);

    LOG(info) << "CbmMcbm2018UnpackerAlgoTrdR::AddMsComponentToList => Component "
              << component << " with detector ID 0x"
              << std::hex << usDetectorId << std::dec << " added to list";
}

void CbmMcbm2018UnpackerAlgoTrdR::SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb)
{
    /// Set Number of Core Microslices per Timeslice
    fuNbCoreMsPerTs = uCoreMsNb;
    /// Set Number of Overlap Microslices per Timeslice
    fuNbOverMsPerTs = uOverlapMsNb;
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::CreateHistograms()
{
    if (!fbMonitorMode && !fbDebugMonitorMode )
        return kFALSE;
    /*
    TString HistName = "RawMessage_Signalshape_all";
    fHistoMap.insert(std::make_pair(HistName.Data(), std::make_shared<TH2I>(TH2I(HistName.Data(), HistName.Data(), 32, -0.5, 31.5, 512, -256.5, 255.5))));
    AddHistoToVector(fHistoMap.at(HistName.Data()).get(), "");

    HistName = "RawMessage_Signalshape_filtered";
    fHistoMap.insert(std::make_pair(HistName.Data(), std::make_shared<TH2I>(TH2I(HistName.Data(), HistName.Data(), 32, -0.5, 31.5, 512, -256.5, 255.5))));
    AddHistoToVector(fHistoMap.at(HistName.Data()).get(), "");
    */
    fHistoArray.SetOwner(kTRUE);

    TString HistName = "RawMessage_Signalshape_all";
    fHistoArray.Add(new TH2I(HistName.Data(), HistName.Data(), 32, -0.5, 31.5, 512, -256.5, 255.5) );
    AddHistoToVector( (TH2I*)fHistoArray.Last(), "");

    HistName = "RawMessage_Signalshape_filtered";
    fHistoArray.Add(new TH2I(HistName.Data(), HistName.Data(), 32, -0.5, 31.5, 512, -256.5, 255.5) );
    AddHistoToVector( (TH2I*)fHistoArray.Last(), "");

    HistName = "Delta_T";
    fHistoArray.Add(new TH1I(HistName.Data(), HistName.Data(), 202500, -2500, 200000) );
    AddHistoToVector( (TH1I*)fHistoArray.Last(), "");

    HistName = "Spadic_Info_Types";
    fHistoArray.Add(new TH2I(HistName.Data(), HistName.Data(), 500000, 0, 5e9, 5, -0.5, 4.5) );
    ((TH2I*)fHistoArray.Last())->SetXTitle("t /Clockcycles") ;
    ((TH2I*)fHistoArray.Last())->SetYTitle("messagetype") ;
    ((TH2I*)fHistoArray.Last())->GetYaxis()->SetBinLabel(1,"BOM") ;
    ((TH2I*)fHistoArray.Last())->GetYaxis()->SetBinLabel(2,"MSB") ;
    ((TH2I*)fHistoArray.Last())->GetYaxis()->SetBinLabel(3,"BUF") ;
    ((TH2I*)fHistoArray.Last())->GetYaxis()->SetBinLabel(4,"UNU") ;
    ((TH2I*)fHistoArray.Last())->GetYaxis()->SetBinLabel(5,"MIS") ;
    AddHistoToVector( (TH2I*)fHistoArray.Last(), "");

    if (fbDebugMonitorMode)
    {
        //create additional histograms here.
    }

    return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::FillHistograms()
{
    // Histograms are being filled in the main message loop in CbmMcbm2018UnpackerAlgoTrdR::ProcessMs.
  return kTRUE; 
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::ResetHistograms()
{
    /*
    for ( auto &it : fHistoMap )
    {
        it.second->Reset() ;
    }
    */

    for ( auto it = fHistoArray.begin() ; it != fHistoArray.end() ; ++it )
    {
        ((TH2I*)*it)->Reset();
    }
    return kTRUE;

}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::SetDigiOutputPointer(std::vector<CbmTrdDigi> *const pVector)
{
    if (nullptr == fTrdDigiVector)
    {
        fTrdDigiVector = pVector;
        return kTRUE;
    }
    else
    {
        return kFALSE;
    }
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::SetRawOutputPointer(std::vector<CbmTrdRawMessageSpadic> *const pVector, std::vector< std::pair<std::uint64_t, std::uint64_t> > *const qVector)
{
    Bool_t ret = 1;
    if (nullptr == fTrdRawMessageVector)
    {
        fTrdRawMessageVector = pVector;
    }
    else
    {
        ret &= 0;
    }

    if (qVector != nullptr && fSpadicInfoMsgVector == nullptr)
    {
        fSpadicInfoMsgVector = qVector;
    }
    else
    {
        ret &= 0;
    }
    return ret;
}

// --------- private: -----------

std::shared_ptr<CbmTrdDigi> CbmMcbm2018UnpackerAlgoTrdR::MakeDigi(CbmTrdRawMessageSpadic raw)
{
    Int_t digiAddress = -1;
    Float_t digiCharge = (Float_t)raw.GetMaxAdc();
    ULong64_t digiTime = raw.GetFullTime();
    Int_t digiTriggerType = raw.GetHitType() ; // Spadic::TriggerType
    Int_t digiErrClass = 0;

    std::uint64_t spadicHwAddress(0);
    spadicHwAddress = (raw.GetElinkId())
                    + (CbmTrdParAsic::kCriIdPosition * raw.GetCriId())
                    + (CbmTrdParAsic::kCrobIdPosition * raw.GetCrobId());
    Int_t asicAddress(0);
    asicAddress = (fSpadicMap.find(spadicHwAddress))->second;
    Int_t uniqueModuleId = asicAddress / 1000;
    Int_t layerId(CbmTrdAddress::GetLayerId(uniqueModuleId));
    Int_t moduleId(CbmTrdAddress::GetModuleId(uniqueModuleId));
    Int_t asicChannelId(0);
    asicChannelId = (raw.GetElinkId() % 2) == fIsFirstChannelsElinkEven ? raw.GetChannelId() : raw.GetChannelId() + NSPADICCH/2;
    digiAddress = (fAsicChannelMap.find(asicAddress))->second.at(asicChannelId);

    std::shared_ptr<CbmTrdDigi> digi = std::make_shared<CbmTrdDigi> ( CbmTrdDigi(digiAddress, digiCharge, digiTime, digiTriggerType, digiErrClass) );

    digi->SetAddress(CbmTrdAddress::GetAddress(layerId, moduleId, 0, 0, 0)); // TODO this could be skipped is shifting the address components back and forth, however the digi setter would need to be enhanced


    // Int_t channelAddress = digi->GetAddressChannel();
    // Int_t address = digi->GetAddress();
    // printf("HelloWorld -- We have a digi with spadicHwAddress %lu - criId %d - eLinkId %d - asicAddress %d - layerId %d - moduleId %d - channelId %d -\n digiAddress %d -- channelAddress %d - fullAddress %d\n", spadicHwAddress, raw.GetCriId(), raw.GetElinkId(), asicAddress, layerId, moduleId, asicChannelId, digiAddress, channelAddress, address);

    return digi;
}

Spadic::MsMessageType CbmMcbm2018UnpackerAlgoTrdR::GetMessageType(const uint64_t msg)
{
    if (( msg >> 61) == 1) // SOM  001. ....
    {
        return Spadic::MsMessageType::kSOM;
    }
    else if (( msg >> 63) == 1) // RDA  1... ....
    {
        return Spadic::MsMessageType::kRDA;
    }
    else if (( msg >> 62) == 1) // Epoch 01.. ....
    {
        return Spadic::MsMessageType::kEPO;
    }
    else if (( msg >> 60) == 1) // Spadic Info Message 0001 ....
    {
        return Spadic::MsMessageType::kINF;
    }
    else if (msg == 0 )  // Last Word in a Microslice is 0
    {
        return Spadic::MsMessageType::kNUL;
    }
    else  // not a spadic message
    {
        return Spadic::MsMessageType::kUNK;
    }
}

Spadic::MsInfoType CbmMcbm2018UnpackerAlgoTrdR::GetInfoType(const uint64_t msg)
{
    uint64_t mask = 0x000FFFFF;

    if (((msg & mask) >> 18) == 3) // BOM
    {
        return Spadic::MsInfoType::kBOM;
    }
    if (((msg & mask) >> 17) == 2) // MSB
    {
        return Spadic::MsInfoType::kMSB;
    }
    if (((msg & mask) >> 17) == 3) // BUF
    {
        return Spadic::MsInfoType::kBUF;
    }
    if (((msg & mask) >> 17) == 4) // UNU
    {
        return Spadic::MsInfoType::kUNU;
    }
    if (((msg & mask) >> 17) == 5) // MIS
    {
        return Spadic::MsInfoType::kMIS;
    }
    else
    {
        LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::GetInfoType] unknown type!" ;
        return Spadic::MsInfoType::kMSB;
    }
}

CbmTrdRawMessageSpadic CbmMcbm2018UnpackerAlgoTrdR::CreateRawMessage(const uint64_t word, fles::MicrosliceDescriptor msDesc)
{
    if ( GetMessageType(word) != Spadic::MsMessageType::kSOM )
    {
        LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::CreateRawMessage]   Not a SOM word!" ;
        return  CbmTrdRawMessageSpadic() ;
    }
    ///Extract Metadata
    uint8_t elinkId = 0, chId = 0, crobId = 0;
    uint16_t criId = msDesc.eq_id;
    // char crobId = msDesc.crob_id; // TODO this needs to be implemented into microslice! - PR 03.2020
    uint8_t hitType = 0, nSamples = 0;
    bool multihit = false;
    uint16_t timestamp = 0;
//    uint64_t ts_overlap = 0;  ///< 3bit of th spadic ts and the epoch counter should be the same (FU 270320 Redmine issue#1563)
    //extract elinkid
    uint64_t mask = 0x3F;
    mask = mask << 55;
    elinkId = (char)((word & mask) >> 55);
    //extract chID
    mask = 0xF;
    mask = mask << 51;
    chId = (char)((word & mask) >> 51);
    //extract timestamp
    mask = 0x7FF;
    mask = mask << 35;
    timestamp = (uint16_t)((word & mask) >> 35);
    // extract timestamp overlap
    mask = 0xFF;
    mask = mask << 46;
//    ts_overlap = (uint64_t)((word & mask) >> 46); (FU 270320 Redmine issue#1563)
    //extract hitType
    mask = 0x3;
    mask = mask << 33;
    hitType = (uint8_t)((word & mask) >> 33);
    //extract MultiHi
    mask = 0x1;
    mask = mask << 32;
    multihit = (bool)((word & mask) >> 32);
    //extract nrSamples
    mask = 0x1F;
    mask = mask << 27;
    nSamples = (uint8_t)((word & mask) >> 27);
    nSamples += 1; //spadic counts from 0 to 31

    // get the correct fulltime
    uint64_t fulltime = timestamp + (fSpadicEpoch * fdMsSizeInCC); // this is in units of clock cycles
    //uint64_t epoch = fSpadicEpoch >> 3 ;
    //uint64_t fulltime = timestamp + (ts_overlap << 11 ) + (epoch << 14); // this is in units of clock cycles
/*
    mask = 0x7;
    uint64_t compare = (mask & fSpadicEpoch) ;
    //LOG(info) << "[CbmMcbm2018UnpackerAlgoTrdR::CreateRawMessage] ts_overlap = " << (UInt_t)ts_overlap ;
    if ( compare != (uint64_t)ts_overlap ) LOG(warning) << "[CbmMcbm2018UnpackerAlgoTrdR::CreateRawMessage] " << compare << " != " << (UInt_t)ts_overlap ;
*/


    // put the first 3 samples, contained in som, into the message.
    std::vector<int16_t> samples;
    for (int i = 0; i < nSamples && i < 3; i++)
    {
        samples.push_back(ExtractSample(word, i, multihit));
    }

    // Create message
    CbmTrdRawMessageSpadic retval(chId, elinkId, crobId, criId, hitType, nSamples, multihit, fulltime, samples);
    return retval;
}

Int_t CbmMcbm2018UnpackerAlgoTrdR::GetNumRda(Int_t nsamples)
{
    if (nsamples < 4)
        return 0;
    else if (nsamples < 11)
        return 1;
    else if (nsamples < 18)
        return 2;
    else if (nsamples < 25)
        return 3;
    else if (nsamples < 32)
        return 4;
    else
        return 5;
}

int16_t CbmMcbm2018UnpackerAlgoTrdR::ExtractSample(const uint64_t word, uint8_t sample, Bool_t multihit )
{
    const std::array<uint16_t, 32> indices{{4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0}};
    uint64_t mask = 0x1FF;
    if ( !((GetMessageType(word) == Spadic::MsMessageType::kSOM) || (GetMessageType(word) == Spadic::MsMessageType::kRDA) ))
    {
        LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ExtractSample]  Wrong Message Type!";
        return -256;
    }

    if ((GetMessageType(word) == Spadic::MsMessageType::kSOM) && (sample > 2))
    {
        LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ExtractSample]  Wrong sample index!";
        return -256;
    }
    if ((GetMessageType(word) == Spadic::MsMessageType::kRDA) && (sample < 3 || sample > 31))
    {
        LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ExtractSample]  Wrong sample index!";
        return -256;
    }

    uint16_t index = indices[sample];

    mask = mask << (9 * (6 - index));
    uint64_t temp = word & mask;
    temp = temp >> (6 - index) * 9;
    if ( fbBaselineAvg && (sample == 0) && !(multihit) )
    {
        /** When the average baseline feature of the spadic22 is activated,
        *   the value of samples[0] is always lower than -128, so we know it is 10-------
        *   Because of this it is possible to increase the precision by two bits,
        *   by cutting the two MSBs off and shifting everything two bits to the left.
        *   Here we need to undo this operation by shifting two bits righ
        *   and setting the MSB to 1 (negative sign) and the second msb to 0 (value < -128 ).
        **/
        temp = temp >> 2;
        temp ^= (-0 ^ temp) & (1 << 7);
        temp ^= (-1 ^ temp) & (1 << 8);
    }
    struct { signed int x : 9; } s;
    int16_t result = s.x = temp;
    return result;
}

ClassImp(CbmMcbm2018UnpackerAlgoTrdR)
