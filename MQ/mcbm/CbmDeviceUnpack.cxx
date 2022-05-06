/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * CbmDeviceUnpack.cxx
 *
 * @since 2020-05-04
 * @author P.-A. Loizeau
 */

#include "CbmDeviceUnpack.h"

#include "CbmFlesCanvasTools.h"
#include "CbmMQDefs.h"
#include "CbmMuchDigi.h"
#include "CbmPsdUnpackConfig.h"
#include "CbmRichUnpackConfig.h"
#include "CbmSetup.h"
#include "CbmStsUnpackConfig.h"
#include "CbmTofUnpackConfig.h"
#include "CbmTrdUnpackConfig.h"
#include "CbmTrdUnpackFaspConfig.h"

#include "StorableTimeslice.hpp"
#include "TimesliceMetaData.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TNamed.h"

#include "BoostSerializer.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <utility>

#include "RootSerializer.h"
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;

//Bool_t bMcbm2018MonitorTaskT0ResetHistos = kFALSE;

CbmDeviceUnpack::CbmDeviceUnpack() {}

void CbmDeviceUnpack::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDeviceUnpack.";
  fsSetupName              = fConfig->GetValue<std::string>("Setup");
  fuRunId                  = fConfig->GetValue<uint32_t>("RunId");
  fbIgnoreOverlapMs        = fConfig->GetValue<bool>("IgnOverMs");
  fbOutputFullTimeSorting  = fConfig->GetValue<bool>("FullTimeSort");
  fvsSetTimeOffs           = fConfig->GetValue<std::vector<std::string>>("SetTimeOffs");
  fsChannelNameDataInput   = fConfig->GetValue<std::string>("TsNameIn");
  fsChannelNameDataOutput  = fConfig->GetValue<std::string>("TsNameOut");
  fuPublishFreqTs          = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime         = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime         = fConfig->GetValue<double_t>("PubTimeMax");
  fsChannelNameHistosInput = fConfig->GetValue<std::string>("ChNameIn");
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

Bool_t CbmDeviceUnpack::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmDeviceUnpack.";

  // ----- FIXME: Environment settings? or binary option?
  TString srcDir = std::getenv("VMCWORKDIR");  // top source directory, standard C++ library
  //  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  // -----   CbmSetup   -----------------------------------------------------
  // TODO: support for multiple setups on Par Server? with request containing setup name?
  CbmSetup* cbmsetup = CbmSetup::Instance();
  FairMQMessagePtr req(NewSimpleMessage("setup"));
  FairMQMessagePtr rep(NewMessage());

  if (Send(req, "parameters") > 0) {
    if (Receive(rep, "parameters") >= 0) {
      if (0 != rep->GetSize()) {
        CbmSetupStorable* exchangableSetup;

        CbmMqTMessage tmsg(rep->GetData(), rep->GetSize());
        exchangableSetup = dynamic_cast<CbmSetupStorable*>(tmsg.ReadObject(tmsg.GetClass()));

        if (nullptr != exchangableSetup) {
          /// Prevent clang format single line if
          cbmsetup->LoadStoredSetup(exchangableSetup);
        }
        else {
          LOG(error) << "Received corrupt reply. Setup not available";
          throw InitTaskError("Setup not received from par-server.");
        }
      }  // if( 0 !=  rep->GetSize() )
      else {
        LOG(error) << "Received empty reply. Setup not available";
        throw InitTaskError("Setup not received from par-server.");
      }  // else of if( 0 !=  rep->GetSize() )
    }    // if( Receive( rep, "parameters" ) >= 0)
  }      // if( Send(req, "parameters") > 0 )
  // ------------------------------------------------------------------------

  /// Initialize the UnpackerConfigs objects and their "user options"
  // ---- STS ----
  std::shared_ptr<CbmStsUnpackConfig> stsconfig = nullptr;
  TString stsSetupTag                           = "";
  cbmsetup->GetGeoTag(ECbmModuleId::kSts, stsSetupTag);
  if ("" != stsSetupTag) {
    LOG(info) << "From received setup, using STS tag: " << stsSetupTag;
    stsconfig = std::make_shared<CbmStsUnpackConfig>(std::string(fsSetupName), fuRunId);
    if (stsconfig) {
      // stsconfig->SetDebugState();
      stsconfig->SetDoWriteOutput();
      stsconfig->SetDoWriteOptOutA("StsDigiPulser");
      std::string parfilesbasepathSts = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
      stsconfig->SetParFilesBasePath(parfilesbasepathSts);
      /// Enable duplicates rejection, Ignores the ADC for duplicates check
      stsconfig->SetDuplicatesRejection(true, true);
      /// Enable Monitor plots
      //      stsconfig->SetMonitor(GetStsMonitor(outfilename, true)); // FIXME: Unsupported for now
      stsconfig->SetSystemTimeOffset(-2221);  // [ns] value to be updated

      stsconfig->SetMinAdcCut(1, 1);
      stsconfig->SetMinAdcCut(2, 1);
      stsconfig->SetMinAdcCut(3, 1);
      stsconfig->SetMinAdcCut(4, 1);

      stsconfig->MaskNoisyChannel(3, 56);
      stsconfig->MaskNoisyChannel(3, 75);
      stsconfig->MaskNoisyChannel(3, 79);
      stsconfig->MaskNoisyChannel(3, 85);
      stsconfig->MaskNoisyChannel(7, 123);
      stsconfig->MaskNoisyChannel(7, 124);
      stsconfig->MaskNoisyChannel(7, 125);
      stsconfig->MaskNoisyChannel(7, 158);
      stsconfig->MaskNoisyChannel(7, 159);
      stsconfig->MaskNoisyChannel(7, 162);
      stsconfig->MaskNoisyChannel(7, 715);
      stsconfig->MaskNoisyChannel(9, 709);
      stsconfig->MaskNoisyChannel(12, 119);

      // Time Walk correction
      std::map<uint32_t, CbmStsParModule> walkMap;
      auto parAsic = new CbmStsParAsic(128, 31, 31., 1., 5., 800., 1000., 3.9789e-3);

      // Module params: number of channels, number of channels per ASIC
      auto parMod = new CbmStsParModule(2048, 128);

      // default
      double p0 = 0, p1 = 0, p2 = 0, p3 = 0;
      parAsic->SetWalkCoef({p0, p1, p2, p3});
      parMod->SetAllAsics(*parAsic);

      walkMap[0x10107C02] = CbmStsParModule(*parMod);  // Make a copy for storage
      walkMap[0x101FFC02] = CbmStsParModule(*parMod);  // Make a copy for storage

      /// To be replaced by a storage in a new parameter class later
      int sensor, asic;
      std::ifstream asicTimeWalk_par(Form("%s/mStsAsicTimeWalk.par", parfilesbasepathSts.data()));
      while (asicTimeWalk_par >> std::hex >> sensor >> std::dec >> asic >> p0 >> p1 >> p2 >> p3) {
        std::cout << Form("Setting time-walk parametersfor: module %x, ASIC %u\n", sensor, asic);
        parAsic->SetWalkCoef({p0, p1, p2, p3});

        if (walkMap.find(sensor) == walkMap.end()) { walkMap[sensor] = CbmStsParModule(*parMod); }
        walkMap[sensor].SetAsic(asic, *parAsic);
      }

      stsconfig->SetWalkMap(walkMap);
    }
  }  // if ("" != stsSetupTag)
  // -------------
  // ---- TRD ----
  std::shared_ptr<CbmTrdUnpackConfig> trd1Dconfig = nullptr;
  TString trdsetuptag                             = "";
  cbmsetup->GetGeoTag(ECbmModuleId::kTrd, trdsetuptag);
  if ("" != trdsetuptag) {
    LOG(info) << "From received setup, using TRD tag: " << trdsetuptag;
    // trd1Dconfig = std::make_shared<CbmTrdUnpackConfig>(trdsetuptag.Data(), fuRunId);
    trd1Dconfig = std::make_shared<CbmTrdUnpackConfig>(trdsetuptag.Data(), 3);
    if (trd1Dconfig) {
      trd1Dconfig->SetDoWriteOutput();
      // Activate the line below to write Trd1D digis to a separate "TrdSpadicDigi" branch. Can be used to separate between Fasp and Spadic digis
      // trd1Dconfig->SetOutputBranchName("TrdSpadicDigi");
      // trd1Dconfig->SetDoWriteOptOutA(CbmTrdRawMessageSpadic::GetBranchName());
      // trd1Dconfig->SetDoWriteOptOutB("SpadicInfoMessages"); // SpadicInfoMessages

      std::string parfilesbasepathTrd = Form("%s/parameters/trd", srcDir.Data());
      trd1Dconfig->SetParFilesBasePath(parfilesbasepathTrd);
      // trd1Dconfig->SetMonitor(GetTrdMonitor(outfilename));  // FIXME: Unsupported for now
      // Get the spadic configuration true = avg baseline active / false plain sample 0
      trd1Dconfig->SetSpadicObject(GetTrdSpadic(true));
      trd1Dconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
    }
  }  // if ("" != trdsetuptag)
  // -------------
  // ---- TRDFASP2D ----
  std::shared_ptr<CbmTrdUnpackFaspConfig> trdfasp2dconfig = nullptr;
  if ("" != trdsetuptag) {
    trdfasp2dconfig = std::make_shared<CbmTrdUnpackFaspConfig>(trdsetuptag.Data(), 3);
    if (trdfasp2dconfig) {
      // trdfasp2dconfig->SetDebugState();
      trdfasp2dconfig->SetDoWriteOutput();
      // Activate the line below to write Trd1D digis to a separate "TrdFaspDigi" branch. Can be used to separate between Fasp and Spadic digis
      trdfasp2dconfig->SetOutputBranchName("TrdFaspDigi");
      std::string parfilesbasepathTrdfasp2d = Form("%s/parameters/trd", srcDir.Data());
      trdfasp2dconfig->SetParFilesBasePath(parfilesbasepathTrdfasp2d);
      trdfasp2dconfig->SetSystemTimeOffset(-1800);  // [ns] value to be updated
    }
  }  // if ("" != trdsetuptag)
  // -------------
  // ---- TOF ----
  std::shared_ptr<CbmTofUnpackConfig> tofconfig = nullptr;
  TString tofSetupTag                           = "";
  cbmsetup->GetGeoTag(ECbmModuleId::kTof, tofSetupTag);
  if ("" != tofSetupTag) {
    LOG(info) << "From received setup, using TOF tag: " << tofSetupTag;
    tofconfig = std::make_shared<CbmTofUnpackConfig>("", fuRunId);
    if (tofconfig) {
      // tofconfig->SetDebugState();
      tofconfig->SetDoWriteOutput();
      // tofconfig->SetDoWriteOptOutA("CbmTofErrors");
      std::string parfilesbasepathTof = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
      tofconfig->SetParFilesBasePath(parfilesbasepathTof);
      tofconfig->SetSystemTimeOffset(-1220);  // [ns] value to be updated
      if (fuRunId <= 1659) {
        /// Switch ON the -4 offset in epoch count (hack for Spring-Summer 2021)
        tofconfig->SetFlagEpochCountHack2021();
      }
    }
  }  // if ("" != tofSetupTag)
  // -------------
  // ---- RICH ----
  std::shared_ptr<CbmRichUnpackConfig> richconfig = nullptr;
  TString richSetupTag                            = "";
  cbmsetup->GetGeoTag(ECbmModuleId::kRich, richSetupTag);
  if ("" != richSetupTag) {
    LOG(info) << "From received setup, using RICH tag: " << richSetupTag;
    richconfig = std::make_shared<CbmRichUnpackConfig>("", fuRunId);
    if (richconfig) {
      if (1904 < fuRunId) {
        /// Switch to new unpacking algo starting from first combined cosmics run in 2022
        richconfig->SetUnpackerVersion(CbmRichUnpackerVersion::v03);
      }
      richconfig->SetDebugState();
      richconfig->SetDoWriteOutput();
      std::string parfilesbasepathRich = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
      richconfig->SetParFilesBasePath(parfilesbasepathRich);
      richconfig->SetSystemTimeOffset(256000 - 1200);  // [ns] 1 MS and additional correction
      if (1588 == fuRunId) richconfig->MaskDiRICH(0x7150);
    }
  }  // if ("" != richSetupTag)
  // -------------
  // ---- PSD ----
  std::shared_ptr<CbmPsdUnpackConfig> psdconfig = nullptr;
  TString psdSetupTag                           = "";
  cbmsetup->GetGeoTag(ECbmModuleId::kPsd, psdSetupTag);
  if ("" != psdSetupTag) {
    LOG(info) << "From received setup, using PSD tag: " << psdSetupTag;
    psdconfig = std::make_shared<CbmPsdUnpackConfig>("", fuRunId);
    if (psdconfig) {
      // psdconfig->SetDebugState();
      psdconfig->SetDoWriteOutput();
      // psdconfig->SetDoWriteOptOutA("CbmPsdDsp");
      std::string parfilesbasepathPsd = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
      psdconfig->SetParFilesBasePath(parfilesbasepathPsd);
      psdconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
    }
  }  // if ("" != psdSetupTag)
  // -------------

  /// Enable full time sorting instead of time sorting per FLIM link
  if (stsconfig) SetUnpackConfig(stsconfig);
  if (trd1Dconfig) SetUnpackConfig(trd1Dconfig);
  if (trdfasp2dconfig) SetUnpackConfig(trdfasp2dconfig);
  if (tofconfig) SetUnpackConfig(tofconfig);
  if (richconfig) SetUnpackConfig(richconfig);
  if (psdconfig) SetUnpackConfig(psdconfig);

  /// Load time offsets
  for (std::vector<std::string>::iterator itStrOffs = fvsSetTimeOffs.begin(); itStrOffs != fvsSetTimeOffs.end();
       ++itStrOffs) {
    size_t charPosDel = (*itStrOffs).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info) << "CbmDeviceUnpack::InitContainers => "
                << "Trying to set trigger window with invalid option pattern, ignored! "
                << " (Should be ECbmModuleId,dWinBeg,dWinEnd but instead found " << (*itStrOffs) << " )";
    }  // if( std::string::npos == charPosDel )

    /// Detector Enum Tag
    std::string sSelDet = (*itStrOffs).substr(0, charPosDel);
    /// Min number
    charPosDel++;
    int32_t iOffset = std::stoi((*itStrOffs).substr(charPosDel));

    if ("kSTS" == sSelDet && fStsConfig) {  //
      fStsConfig->SetSystemTimeOffset(iOffset);
    }  // if( "kSTS"  == sSelDet && fStsConfig)
       /*
    else if ("kMUCH" == sSelDet) {
      fUnpAlgoMuch->SetSystemTimeOffset(iOffset);
    }  // else if( "kMUCH" == sSelDet )
*/
    else if ("kTRD" == sSelDet && fTrd1DConfig) {
      fTrd1DConfig->SetSystemTimeOffset(iOffset);
    }  // else if( "kTRD"  == sSelDet && fTrd2DConfig )
    else if ("kTRD2D" == sSelDet && fTrd2DConfig) {
      fTrd2DConfig->SetSystemTimeOffset(iOffset);
    }  // else if( "kTRD"  == sSelDet && fTrd2DConfig )
    else if ("kTOF" == sSelDet && fTofConfig) {
      fTofConfig->SetSystemTimeOffset(iOffset);
    }  // else if( "kTOF"  == sSelDet && fTofConfig)
    else if ("kRICH" == sSelDet && fRichConfig) {
      fRichConfig->SetSystemTimeOffset(iOffset);
    }  // else if( "kRICH" == sSelDet && fRichConfig)
    else if ("kPSD" == sSelDet && fPsdConfig) {
      fPsdConfig->SetSystemTimeOffset(iOffset);
    }  // else if( "kPSD"  == sSelDet )
    else {
      LOG(info) << "CbmDeviceUnpack::InitContainers => Trying to set time "
                   "offset for unsupported detector, ignored! "
                << (sSelDet);
      continue;
    }  // else of detector enum detection
  }  // for( std::vector< std::string >::iterator itStrAdd = fvsAddDet.begin(); itStrAdd != fvsAddDet.end(); ++itStrAdd )

  Bool_t initOK = kTRUE;
  // --- Sts
  if (fStsConfig) {
    fStsConfig->InitOutput();
    //    RegisterOutputs( ioman, fStsConfig ); /// Framework bound work = kept in this Task
    fStsConfig->SetDoIgnoreOverlappMs(fbIgnoreOverlapMs);
    fStsConfig->SetAlgo();
    initOK &= InitParameters(fStsConfig->GetParContainerRequest());  /// Framework bound work = kept in this Device
    fStsConfig->InitAlgo();
    //    initPerformanceMaps(fkFlesSts, "STS");
  }
  // --- Tof
  if (fTofConfig) {
    fTofConfig->InitOutput();
    //    RegisterOutputs( ioman, fTofConfig ); /// Framework bound work = kept in this Task
    fTofConfig->SetDoIgnoreOverlappMs(fbIgnoreOverlapMs);
    fTofConfig->SetAlgo();
    initOK &= InitParameters(fTofConfig->GetParContainerRequest());  /// Framework bound work = kept in this Device
    LOG(info) << "TOF call InitAlgo()";
    fTofConfig->InitAlgo();
    //    initPerformanceMaps(fkFlesTof, "TOF");
  }
  // --- Trd
  if (fTrd1DConfig) {
    fTrd1DConfig->InitOutput();
    //    RegisterOutputs( ioman, fTrd1DConfig ); /// Framework bound work = kept in this Task
    fTrd1DConfig->SetDoIgnoreOverlappMs(fbIgnoreOverlapMs);
    fTrd1DConfig->SetAlgo();
    initOK &= InitParameters(fTrd1DConfig->GetParContainerRequest());  /// Framework bound work = kept in this Device
    fTrd1DConfig->InitAlgo();
    //    initPerformanceMaps(fkFlesTrd, "TRD1D");
  }
  // --- TRD2D
  if (fTrd2DConfig) {
    if (fTrd1DConfig && (fTrd2DConfig->GetOutputBranchName() == fTrd1DConfig->GetOutputBranchName())) {
      LOG(info) << fTrd2DConfig->GetName() << "::Init() ---------------------------------";
      fTrd2DConfig->SetOutputVec(fTrd1DConfig->GetOutputVec());
    }
    else {
      fTrd2DConfig->InitOutput();
      //      RegisterOutputs( ioman, fTrd2DConfig ); /// Framework bound work = kept in this Task
    }
    fTrd2DConfig->SetDoIgnoreOverlappMs(fbIgnoreOverlapMs);
    fTrd2DConfig->SetAlgo();
    initOK &= InitParameters(fTrd2DConfig->GetParContainerRequest());  /// Framework bound work = kept in this Device
    fTrd2DConfig->InitAlgo();
    //    initPerformanceMaps(fkFlesTrd2D, "TRD2D");
  }
  // This is an ugly work around, because the TRD and TRD2D want to access the same vector and there is no
  // function to retrieve a writeable vector<obj> from the FairRootManager, especially before the branches
  // are created, as far as I am aware.
  // The second option workaround is in in Init() to look for the fasp config and create a separate branch
  // for fasp created CbmTrdDigis PR 072021
  // --- Rich
  if (fRichConfig) {
    fRichConfig->InitOutput();
    //    RegisterOutputs( ioman, fRichConfig ); /// Framework bound work = kept in this Task
    fRichConfig->SetDoIgnoreOverlappMs(fbIgnoreOverlapMs);
    fRichConfig->SetAlgo();
    initOK &= InitParameters(fRichConfig->GetParContainerRequest());  /// Framework bound work = kept in this Device
    fRichConfig->InitAlgo();
    //    initPerformanceMaps(fkFlesRich, "RICH");
  }
  // --- Psd
  if (fPsdConfig) {
    fPsdConfig->InitOutput();
    //    RegisterOutputs( ioman, fPsdConfig ); /// Framework bound work = kept in this Task
    fPsdConfig->SetDoIgnoreOverlappMs(fbIgnoreOverlapMs);
    fPsdConfig->SetAlgo();
    initOK &= InitParameters(fPsdConfig->GetParContainerRequest());  /// Framework bound work = kept in this Device
    fPsdConfig->InitAlgo();
    //    initPerformanceMaps(fkFlesPsd, "PSD");
  }

  /// Event header object
  fCbmTsEventHeader = new CbmTsEventHeader();

  return initOK;
}

Bool_t
CbmDeviceUnpack::InitParameters(std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>* reqparvec)
{
  LOG(info) << "CbmDeviceUnpack::InitParameters";
  if (!reqparvec) {
    LOG(info) << "CbmDeviceUnpack::InitParameters - empty requirements vector no parameters initialized.";
    return kTRUE;
  }

  // Now get the actual ascii files and init the containers with the asciiIo
  for (auto& pair : *reqparvec) {
    /*
    auto filepath = pair.first;
    auto parset   = pair.second;
    FairParAsciiFileIo asciiInput;
    if (!filepath.empty()) {
      if (asciiInput.open(filepath.data())) { parset->init(&asciiInput); }
    }
    * */
    std::string paramName {pair.second->GetName()};
    // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
    // Should only be used for small data because of the cost of an additional copy

    // Here must come the proper Runid
    std::string message = paramName + ",111";
    LOG(info) << "Requesting parameter container " << paramName << ", sending message: " << message;

    FairMQMessagePtr req(NewSimpleMessage(message));
    FairMQMessagePtr rep(NewMessage());

    FairParGenericSet* newObj = nullptr;

    if (Send(req, "parameters") > 0) {
      if (Receive(rep, "parameters") >= 0) {
        if (0 != rep->GetSize()) {
          CbmMqTMessage tmsg(rep->GetData(), rep->GetSize());
          newObj = static_cast<FairParGenericSet*>(tmsg.ReadObject(tmsg.GetClass()));
          LOG(info) << "Received unpack parameter from the server: " << newObj->GetName();
          newObj->print();
        }  // if( 0 !=  rep->GetSize() )
        else {
          LOG(error) << "Received empty reply. Parameter not available";
          return kFALSE;
        }                       // else of if( 0 !=  rep->GetSize() )
      }                         // if( Receive( rep, "parameters" ) >= 0)
    }                           // if( Send(req, "parameters") > 0 )
    pair.second.reset(newObj);  /// Potentially unsafe reasignment of raw pointer to the shared pointer?
    //delete newObj;
  }
  return kTRUE;
}

bool CbmDeviceUnpack::InitHistograms()
{
  /// Histos creation and obtain pointer on them
  /// Trigger histo creation on all associated algos
  // ALGO: bool initOK = fMonitorAlgo->CreateHistograms();
  bool initOK = true;

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  // ALGO: std::vector<std::pair<TNamed*, std::string>> vHistos = fMonitorAlgo->GetHistoVector();
  std::vector<std::pair<TNamed*, std::string>> vHistos = {};
  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  // ALGO: std::vector<std::pair<TCanvas*, std::string>> vCanvases = fMonitorAlgo->GetCanvasVector();
  std::vector<std::pair<TCanvas*, std::string>> vCanvases = {};

  /// Add pointers to each histo in the histo array
  /// Create histo config vector
  /// ===> Use an std::vector< std::pair< std::string, std::string > > with < Histo name, Folder >
  ///      and send it through a separate channel using the BoostSerializer
  for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
    //         LOG(info) << "Registering  " << vHistos[ uHisto ].first->GetName()
    //                   << " in " << vHistos[ uHisto ].second.data()
    //                   ;
    fArrayHisto.Add(vHistos[uHisto].first);
    std::pair<std::string, std::string> psHistoConfig(vHistos[uHisto].first->GetName(), vHistos[uHisto].second);
    fvpsHistosFolder.push_back(psHistoConfig);

    LOG(info) << "Config of hist  " << psHistoConfig.first.data() << " in folder " << psHistoConfig.second.data();
  }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

  /// Create canvas config vector
  /// ===> Use an std::vector< std::pair< std::string, std::string > > with < Canvas name, config >
  ///      and send it through a separate channel using the BoostSerializer
  for (UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv) {
    //         LOG(info) << "Registering  " << vCanvases[ uCanv ].first->GetName()
    //                   << " in " << vCanvases[ uCanv ].second.data();
    std::string sCanvName = (vCanvases[uCanv].first)->GetName();
    std::string sCanvConf = GenerateCanvasConfigString(vCanvases[uCanv].first);

    std::pair<std::string, std::string> psCanvConfig(sCanvName, sCanvConf);

    fvpsCanvasConfig.push_back(psCanvConfig);

    LOG(info) << "Config string of Canvas  " << psCanvConfig.first.data() << " is " << psCanvConfig.second.data();
  }  //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )

  return initOK;
}

// Method called by run loop and requesting new data from the TS source whenever
bool CbmDeviceUnpack::ConditionalRun()
{
  /// First request a new TS (full one)
  std::string message = "full";
  LOG(debug) << "Requesting new TS by sending message: full" << message;
  FairMQMessagePtr req(NewSimpleMessage(message));
  FairMQMessagePtr rep(NewMessage());

  if (Send(req, fsChannelNameDataInput) <= 0) {
    LOG(error) << "Failed to send the request! message was " << message;
    return false;
  }  // if (Send(req, fsChannelNameDataInput) <= 0)
  else if (Receive(rep, fsChannelNameDataInput) < 0) {
    LOG(error) << "Failed to receive a reply to the request! message was " << message;
    return false;
  }  // else if (Receive(rep, fsChannelNameDataInput) < 0)
  else if (rep->GetSize() == 0) {
    LOG(error) << "Received empty reply. Something went wrong with the timeslice generation! message was " << message;
    return false;
  }  // else if (rep->GetSize() == 0)

  /// Message received, do Algo related Initialization steps if needed
  if (0 == fulNumMessages) {
    try {
      InitContainers();
    }
    catch (InitTaskError& e) {
      LOG(error) << e.what();
      ChangeState(fair::mq::Transition::ErrorFound);
    }
  }  // if( 0 == fulNumMessages)

  if (0 == fulNumMessages) InitHistograms();

  fulNumMessages++;
  LOG(debug) << "Received message number " << fulNumMessages << " with size " << rep->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  std::string msgStr(static_cast<char*>(rep->GetData()), rep->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  /// Create an empty TS and fill it with the incoming message
  fles::StorableTimeslice ts {0};
  inputArchive >> ts;

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdMsSizeInNs     = (ts.descriptor(0, fuNbCoreMsPerTs).idx - ts.descriptor(0, 0).idx) / fuNbCoreMsPerTs;
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsOverSizeInNs = fdMsSizeInNs * (fuNbOverMsPerTs);
    fdTsFullSizeInNs = fdTsCoreSizeInNs + fdTsOverSizeInNs;
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a MS duration of " << fdMsSizeInNs << " ns, a core duration of " << fdTsCoreSizeInNs
              << " ns and a full duration of " << fdTsFullSizeInNs << " ns";
    fTsMetaData = new TimesliceMetaData(ts.descriptor(0, 0).idx, fdTsCoreSizeInNs, fdTsOverSizeInNs, ts.index());
  }  // if( -1.0 == fdTsCoreSizeInNs )
  else {
    /// Update only the fields changing from TS to TS
    fTsMetaData->SetStartTime(ts.descriptor(0, 0).idx);
    fTsMetaData->SetIndex(ts.index());
  }

  /// Process the Timeslice
  DoUnpack(ts, 0);

  LOG(debug) << "Unpack: Sending TS index " << ts.index();
  /// Send digi vectors to ouput
  if (!SendUnpData()) return false;
  LOG(debug) << "Unpack: Sent TS index " << ts.index();

  // Reset the event header for a new timeslice
  fCbmTsEventHeader->Reset();

  // Reset the unpackers for a new timeslice, e.g. clear the output vectors
  // ---- Sts ----
  if (fStsConfig) fStsConfig->Reset();
  // ---- Tof ----
  if (fTofConfig) fTofConfig->Reset();
  // ---- Trd ----
  if (fTrd1DConfig) fTrd1DConfig->Reset();
  // ---- Trd2D ----
  if (fTrd2DConfig) fTrd2DConfig->Reset();
  // ---- Rich ----
  if (fRichConfig) fRichConfig->Reset();
  // ---- Psd ----
  if (fPsdConfig) fPsdConfig->Reset();

  /// Send histograms each 100 time slices. Should be each ~1s
  /// Use also runtime checker to trigger sending after M s if
  /// processing too slow or delay sending if processing too fast
  std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
  std::chrono::duration<double_t> elapsedSeconds    = currentTime - fLastPublishTime;
  if ((fdMaxPublishTime < elapsedSeconds.count())
      || (0 == fulNumMessages % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count())) {
    if (!fbConfigSent) {
      // Send the configuration only once per run!
      fbConfigSent = SendHistoConfAndData();
    }  // if( !fbConfigSent )
    else
      SendHistograms();

    fLastPublishTime = std::chrono::system_clock::now();
  }  // if( ( fdMaxPublishTime < elapsedSeconds.count() ) || ( 0 == fulNumMessages % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count() ) )

  return true;
}

bool CbmDeviceUnpack::SendUnpData()
{
  FairMQParts parts;

  /// Prepare serialized versions of the TS Event header
  FairMQMessagePtr messTsHeader(NewMessage());
  //  Serialize<RootSerializer>(*messTsHeader, fCbmTsEventHeader);
  RootSerializer().Serialize(*messTsHeader, fCbmTsEventHeader);

  parts.AddPart(std::move(messTsHeader));

  // ---- T0 ----
  std::stringstream ossT0;
  boost::archive::binary_oarchive oaT0(ossT0);
  //  oaT0 << fUnpAlgoTof->GetOptOutAVec();
  oaT0 << (std::vector<CbmTofDigi>());
  std::string* strMsgT0 = new std::string(ossT0.str());

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgT0->c_str()),  // data
    strMsgT0->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgT0));  // object that manages the data

  // ---- Sts ----
  std::stringstream ossSts;
  boost::archive::binary_oarchive oaSts(ossSts);
  if (fStsConfig) {  //
    oaSts << *(fStsConfig->GetOutputVec());
  }
  std::string* strMsgSts = new std::string(ossSts.str());

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgSts->c_str()),  // data
    strMsgSts->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgSts));  // object that manages the data

  // ---- Much ----
  std::stringstream ossMuch;
  boost::archive::binary_oarchive oaMuch(ossMuch);
  //  oaMuch << fUnpAlgoMuch->GetVector();
  oaMuch << (std::vector<CbmMuchDigi>());
  std::string* strMsgMuch = new std::string(ossMuch.str());

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgMuch->c_str()),  // data
    strMsgMuch->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgMuch));  // object that manages the data


  // ---- Trd ----
  std::stringstream ossTrd;
  boost::archive::binary_oarchive oaTrd(ossTrd);
  if (fTrd1DConfig || fTrd2DConfig) {  //
    oaTrd << *(fTrd1DConfig ? fTrd1DConfig->GetOutputVec() : fTrd2DConfig->GetOutputVec());
  }
  std::string* strMsgTrd = new std::string(ossTrd.str());

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgTrd->c_str()),  // data
    strMsgTrd->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgTrd));  // object that manages the data

  // ---- Tof ----
  std::stringstream ossTof;
  boost::archive::binary_oarchive oaTof(ossTof);
  if (fTofConfig) {  //
    oaTof << *(fTofConfig->GetOutputVec());
  }
  std::string* strMsgTof = new std::string(ossTof.str());

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgTof->c_str()),  // data
    strMsgTof->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgTof));  // object that manages the data

  // ---- Rich ----
  std::stringstream ossRich;
  boost::archive::binary_oarchive oaRich(ossRich);
  if (fRichConfig) {  //
    oaRich << *(fRichConfig->GetOutputVec());
  }
  std::string* strMsgRich = new std::string(ossRich.str());

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgRich->c_str()),  // data
    strMsgRich->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgRich));  // object that manages the data

  // ---- Psd ----
  std::stringstream ossPsd;
  boost::archive::binary_oarchive oaPsd(ossPsd);
  if (fPsdConfig) {  //
    oaPsd << *(fPsdConfig->GetOutputVec());
  }
  std::string* strMsgPsd = new std::string(ossPsd.str());

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgPsd->c_str()),  // data
    strMsgPsd->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgPsd));  // object that manages the data

  /// Prepare serialized versions of the TS Meta
  /// FIXME: only for TS duration and overlap, should be sent to parameter service instead as stable values in run
  ///        Index and start time are already included in the TsHeader object!
  FairMQMessagePtr messTsMeta(NewMessage());
  //  Serialize<RootSerializer>(*messTsMeta, fTsMetaData);
  RootSerializer().Serialize(*messTsMeta, fTsMetaData);
  parts.AddPart(std::move(messTsMeta));

  if (Send(parts, fsChannelNameDataOutput) < 0) {
    LOG(error) << "Problem sending data to " << fsChannelNameDataOutput;
    return false;
  }

  return true;
}


bool CbmDeviceUnpack::SendHistoConfAndData()
{
  /// Prepare multiparts message and header
  std::pair<uint32_t, uint32_t> pairHeader(fvpsHistosFolder.size(), fvpsCanvasConfig.size());
  FairMQMessagePtr messageHeader(NewMessage());
  //  Serialize<BoostSerializer<std::pair<uint32_t, uint32_t>>>(*messageHeader, pairHeader);
  BoostSerializer<std::pair<uint32_t, uint32_t>>().Serialize(*messageHeader, pairHeader);
  FairMQParts partsOut;
  partsOut.AddPart(std::move(messageHeader));

  for (UInt_t uHisto = 0; uHisto < fvpsHistosFolder.size(); ++uHisto) {
    /// Serialize the vector of histo config into a single MQ message
    FairMQMessagePtr messageHist(NewMessage());
    //    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageHist, fvpsHistosFolder[uHisto]);
    BoostSerializer<std::pair<std::string, std::string>>().Serialize(*messageHist, fvpsHistosFolder[uHisto]);

    partsOut.AddPart(std::move(messageHist));
  }  // for (UInt_t uHisto = 0; uHisto < fvpsHistosFolder.size(); ++uHisto)

  /// Catch case where no histos are registered!
  /// => Add empty message
  if (0 == fvpsHistosFolder.size()) {
    FairMQMessagePtr messageHist(NewMessage());
    partsOut.AddPart(std::move(messageHist));
  }

  for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv) {
    /// Serialize the vector of canvas config into a single MQ message
    FairMQMessagePtr messageCan(NewMessage());
    //    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageCan, fvpsCanvasConfig[uCanv]);
    BoostSerializer<std::pair<std::string, std::string>>().Serialize(*messageCan, fvpsCanvasConfig[uCanv]);

    partsOut.AddPart(std::move(messageCan));
  }  // for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv)

  /// Catch case where no Canvases are registered!
  /// => Add empty message
  if (0 == fvpsCanvasConfig.size()) {
    FairMQMessagePtr messageHist(NewMessage());
    partsOut.AddPart(std::move(messageHist));
  }

  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr msgHistos(NewMessage());
  //  Serialize<RootSerializer>(*msgHistos, &fArrayHisto);
  RootSerializer().Serialize(*msgHistos, &fArrayHisto);
  partsOut.AddPart(std::move(msgHistos));

  /// Send the multi-parts message to the common histogram messages queue
  if (Send(partsOut, fsChannelNameHistosInput) < 0) {
    LOG(error) << "CbmTsConsumerReqDevExample::SendHistoConfAndData => Problem sending data";
    return false;
  }  // if( Send( partsOut, fsChannelNameHistosInput ) < 0 )

  /// Reset the histograms after sending them (but do not reset the time)
  // ALGO: fMonitorAlgo->ResetHistograms(kFALSE);

  return true;
}

bool CbmDeviceUnpack::SendHistograms()
{
  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr message(NewMessage());
  //  Serialize<RootSerializer>(*message, &fArrayHisto);
  RootSerializer().Serialize(*message, &fArrayHisto);

  /// Send message to the common histogram messages queue
  if (Send(message, fsChannelNameHistosInput) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }  // if( Send( message, fsChannelNameHistosInput ) < 0 )

  /// Reset the histograms after sending them (but do not reset the time)
  // ALGO: fMonitorAlgo->ResetHistograms(kFALSE);

  return true;
}


CbmDeviceUnpack::~CbmDeviceUnpack()
{
  if (fStsConfig) fStsConfig->GetUnpacker()->Finish();
  if (fTofConfig) fTofConfig->GetUnpacker()->Finish();
  if (fTrd1DConfig) fTrd1DConfig->GetUnpacker()->Finish();
  if (fTrd2DConfig) fTrd2DConfig->GetUnpacker()->Finish();
  if (fRichConfig) fRichConfig->GetUnpacker()->Finish();
  if (fPsdConfig) fPsdConfig->GetUnpacker()->Finish();
}

Bool_t CbmDeviceUnpack::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  fulTsCounter++;
  // Prepare timeslice
  //  const fles::Timeslice& timeslice = *ts;

  fCbmTsEventHeader->SetTsIndex(ts.index());
  fCbmTsEventHeader->SetTsStartTime(ts.start_time());

  uint64_t nComponents = ts.num_components();
  // if (fDoDebugPrints) LOG(info) << "Unpack: TS index " << ts.index() << " components " << nComponents;
  LOG(debug) << "Unpack: TS index " << ts.index() << " components " << nComponents;

  for (uint64_t component = 0; component < nComponents; component++) {
    auto systemId = static_cast<std::uint16_t>(ts.descriptor(component, 0).sys_id);

    switch (systemId) {
      case fkFlesSts: {
        if (fStsConfig) {
          fCbmTsEventHeader->AddNDigisSts(
            unpack(systemId, &ts, component, fStsConfig, fStsConfig->GetOptOutAVec(), fStsConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesTof: {
        if (fTofConfig) {
          fCbmTsEventHeader->AddNDigisTof(
            unpack(systemId, &ts, component, fTofConfig, fTofConfig->GetOptOutAVec(), fTofConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesTrd: {
        if (fTrd1DConfig) {
          fCbmTsEventHeader->AddNDigisTrd1D(unpack(systemId, &ts, component, fTrd1DConfig,
                                                   fTrd1DConfig->GetOptOutAVec(), fTrd1DConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesTrd2D: {
        if (fTrd2DConfig) {
          fCbmTsEventHeader->AddNDigisTrd2D(unpack(systemId, &ts, component, fTrd2DConfig,
                                                   fTrd2DConfig->GetOptOutAVec(), fTrd2DConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesRich: {
        if (fRichConfig) {
          fCbmTsEventHeader->AddNDigisRich(
            unpack(systemId, &ts, component, fRichConfig, fRichConfig->GetOptOutAVec(), fRichConfig->GetOptOutBVec()));
        }
        break;
      }
      case fkFlesPsd: {
        if (fPsdConfig) {
          fCbmTsEventHeader->AddNDigisPsd(
            unpack(systemId, &ts, component, fPsdConfig, fPsdConfig->GetOptOutAVec(), fPsdConfig->GetOptOutBVec()));
        }
        break;
      }
      default: {
        if (fDoDebugPrints) LOG(error) << "Unpack: Unknown system ID " << systemId << " for component " << component;
        break;
      }
    }
  }

  if (fbOutputFullTimeSorting) {
    /// Time sort the output vectors of all unpackers present
    if (fStsConfig && fStsConfig->GetOutputVec()) { timesort(fStsConfig->GetOutputVec()); }
    if (fTofConfig && fTofConfig->GetOutputVec()) { timesort(fTofConfig->GetOutputVec()); }
    if (fTrd1DConfig && fTrd1DConfig->GetOutputVec()) { timesort(fTrd1DConfig->GetOutputVec()); }
    if (fTrd2DConfig && fTrd2DConfig->GetOutputVec()) { timesort(fTrd2DConfig->GetOutputVec()); }
    if (fRichConfig && fRichConfig->GetOutputVec()) { timesort(fRichConfig->GetOutputVec()); }
    if (fPsdConfig && fPsdConfig->GetOutputVec()) { timesort(fPsdConfig->GetOutputVec()); }

    /// Time sort the output vectors of all unpackers present
    if (fStsConfig && fStsConfig->GetOptOutAVec()) { timesort(fStsConfig->GetOptOutAVec()); }
    if (fTofConfig && fTofConfig->GetOptOutAVec()) { timesort(fTofConfig->GetOptOutAVec()); }
    if (fTrd1DConfig && fTrd1DConfig->GetOptOutAVec()) { timesort(fTrd1DConfig->GetOptOutAVec()); }
    if (fTrd2DConfig && fTrd2DConfig->GetOptOutAVec()) { timesort(fTrd2DConfig->GetOptOutAVec()); }
    if (fRichConfig && fRichConfig->GetOptOutAVec()) { timesort(fRichConfig->GetOptOutAVec()); }
    if (fPsdConfig && fPsdConfig->GetOptOutAVec()) { timesort(fPsdConfig->GetOptOutAVec()); }
  }

  if (0 == fulTsCounter % 10000) LOG(info) << "Processed " << fulTsCounter << " time slices";

  return kTRUE;
}
/**
 * @brief Get the Trd Spadic
 * @return std::shared_ptr<CbmTrdSpadic>
*/
std::shared_ptr<CbmTrdSpadic> CbmDeviceUnpack::GetTrdSpadic(bool useAvgBaseline)
{
  auto spadic = std::make_shared<CbmTrdSpadic>();
  spadic->SetUseBaselineAverage(useAvgBaseline);
  spadic->SetMaxAdcToEnergyCal(1.0);

  return spadic;
}

void CbmDeviceUnpack::Finish() {}
