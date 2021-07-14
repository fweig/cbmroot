/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmTrdUnpackMonitor.h"

#include "CbmTrdDigi.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdRawMessageSpadic.h"

#include <MicrosliceDescriptor.hpp>

#include <FairRun.h>
#include <FairRunOnline.h>
#include <Logger.h>

#include <RtypesCore.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>

#include <boost/math/special_functions/math_fwd.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include <cmath>

CbmTrdUnpackMonitor::CbmTrdUnpackMonitor(/* args */) {}

CbmTrdUnpackMonitor::~CbmTrdUnpackMonitor() {}

// ---- FillHistos ----
void CbmTrdUnpackMonitor::FillHistos(CbmTrdDigi* digi, CbmTrdRawMessageSpadic* raw)
{
  auto moduleid = digi->GetAddressModule();
  for (auto pair : fDigiHistoMap) {
    auto modulepair = pair.second.find(moduleid);
    auto histo      = modulepair->second;
    fillHisto(digi, pair.first, moduleid, histo);
  }

  if (raw) {
    for (auto pair : fRawHistoMap) {
      auto modulepair = pair.second.find(moduleid);
      auto histo      = modulepair->second;
      fillHisto(raw, pair.first, histo, digi);
    }
  }
}

// ---- FillHistos ----
void CbmTrdUnpackMonitor::FillHisto(Spadic::MsInfoType type, std::uint32_t moduleid)
{
  auto pair = fOtherHistoMap.find(eOtherHistos::kSpadic_Info_Types);
  if (pair == fOtherHistoMap.end()) return;

  auto histo = pair->second.find(moduleid)->second;

  histo->Fill(static_cast<std::uint32_t>(type));
}

// ---- FillHistos ----
void CbmTrdUnpackMonitor::FillHisto(fles::MicrosliceFlags flag, std::uint32_t moduleid)
{
  auto pair = fOtherHistoMap.find(eOtherHistos::kMs_Flags);
  if (pair == fOtherHistoMap.end()) return;

  auto histo = pair->second.find(moduleid)->second;

  histo->Fill(static_cast<std::uint32_t>(flag));
}

void CbmTrdUnpackMonitor::Finish()
{
  LOG(info) << Class_Name() << "::Finish() - ";

  if (fDoWriteToFile) {
    size_t nhistos = 0;

    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    /// (Re-)Create ROOT file to store the histos
    // std::unique_ptr<TFile> histofile(new TFile(fOutfilename.data(), "RECREATE"));
    TFile histofile(fOutfilename.data(), "RECREATE");

    nhistos += writeHistosToFile(&fDigiHistoMap, &histofile);
    nhistos += writeHistosToFile(&fRawHistoMap, &histofile);
    nhistos += writeHistosToFile(&fOtherHistoMap, &histofile);

    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;

    // histofile->Close();
    histofile.Close();

    LOG(info) << Class_Name() << "::Finish() nHistos " << nhistos << " written to " << fOutfilename.data();
  }
}

// ---- Init ----
Bool_t CbmTrdUnpackMonitor::Init(CbmTrdParSetDigi* digiParSet)
{
  auto modulemap = digiParSet->GetModuleMap();
  for (auto modulepair : modulemap) {
    auto parmoddigi = static_cast<CbmTrdParModDigi*>(modulepair.second);

    fModuleIdsVec.emplace_back(modulepair.first);

    fModuleOrientation.emplace(std::pair<std::uint32_t, std::uint8_t>(modulepair.first, parmoddigi->GetOrientation()));
    fModuleNrRows.emplace(std::pair<std::uint32_t, std::uint8_t>(modulepair.first, parmoddigi->GetNofRows()));
    fModuleNrColumns.emplace(std::pair<std::uint32_t, std::uint8_t>(modulepair.first, parmoddigi->GetNofColumns()));
  }

  if (fModuleOrientation.empty() || fModuleNrRows.empty() || fModuleNrColumns.empty()) return kFALSE;

  // Check if there is a HttpServer and if so we later on register the created histograms in it.
  if (FairRun::Instance()->IsA() == FairRunOnline::Class()) {
    auto run = static_cast<FairRunOnline*>(FairRun::Instance());

    fHistoServer = run->GetHttpServer();
  }

  createHistos();

  return kTRUE;
}

// ---- createHisto ----
void CbmTrdUnpackMonitor::createHisto(eDigiHistos kHisto)
{
  std::string histoname         = "";
  std::shared_ptr<TH1> newhisto = nullptr;

  for (auto moduleid : fModuleIdsVec) {
    auto ncols     = fModuleNrColumns.find(moduleid)->second;
    auto nrows     = fModuleNrRows.find(moduleid)->second;
    auto nchannels = nrows * ncols;
    auto rotation  = fModuleOrientation.find(moduleid)->second;

    histoname = "ModuleId_" + std::to_string(moduleid) + "-";
    histoname += getTypeName(kHisto) + "_";
    histoname += getHistoName(kHisto);

    switch (kHisto) {
      case eDigiHistos::kMap:
      case eDigiHistos::kMap_St:
      case eDigiHistos::kMap_Nt:
        //  Check if we have chamber sensitive along the x-axis
        if (rotation % 2 == 0) {
          newhisto = std::make_shared<TH2I>(histoname.data(), histoname.data(), ncols, -0.5, (ncols - 0.5), nrows, -0.5,
                                            (nrows - 0.5));
          newhisto->SetXTitle("Pad column");
          newhisto->SetYTitle("Pad row");
        }
        // If not it is sensitive along the y-axis
        else {
          newhisto = std::make_shared<TH2I>(histoname.data(), histoname.data(), nrows, -0.5, (nrows - 0.5), ncols, -0.5,
                                            (ncols - 0.5));
          newhisto->SetXTitle("Pad row");
          newhisto->SetYTitle("Pad column");
        }
        break;

      case eDigiHistos::kCharge:
      case eDigiHistos::kCharge_St:
      case eDigiHistos::kCharge_Nt:
        newhisto = std::make_shared<TH1I>(histoname.data(), histoname.data(), fSpadic->GetDynamicRange(), 0 - 0.5,
                                          fSpadic->GetDynamicRange() - 0.5);
        newhisto->SetXTitle("MaxAdc [ADC units]");
        newhisto->SetYTitle("Counts");
        break;
      case eDigiHistos::kTriggerType:
        newhisto = std::make_shared<TH1I>(histoname.data(), histoname.data(),
                                          static_cast<Int_t>(CbmTrdDigi::eTriggerType::kNTrg), -0.5,
                                          (static_cast<Int_t>(CbmTrdDigi::eTriggerType::kNTrg) - 0.5));
        newhisto->SetXTitle("CbmTrdDigi eTriggerType");
        newhisto->SetYTitle("Counts");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(CbmTrdDigi::eTriggerType::kSelf) + 1), "St");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(CbmTrdDigi::eTriggerType::kNeighbor) + 1), "Nt");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(CbmTrdDigi::eTriggerType::kMulti) + 1), "Multihit");

        break;
      case eDigiHistos::kChannel:
      case eDigiHistos::kChannel_St:
      case eDigiHistos::kChannel_Nt:
        newhisto = std::make_shared<TH1I>(histoname.data(), histoname.data(), nchannels, -0.5, (nchannels - 0.5));
        newhisto->SetXTitle("ChannelId");
        newhisto->SetYTitle("Counts");
        break;
      case eDigiHistos::kDigiDeltaT:
        newhisto = std::make_shared<TH2I>(histoname.data(), histoname.data(), nchannels, -0.5, (nchannels - 0.5), 3500,
                                          -2e6, 5e6);
        newhisto->SetXTitle("ChannelId");
        newhisto->SetYTitle("#DeltaT(Digi(n)-Digi(n-1)) [ns]");
        break;
        // default: return; break;
    }
    LOG(debug) << Class_Name() << "::CreateHisto() HistoDigi " << static_cast<size_t>(kHisto) << " Module " << moduleid
               << " initialized as " << histoname.data();
    if (newhisto) { addHistoToMap<eDigiHistos>(newhisto, &fDigiHistoMap, moduleid, kHisto); }
  }
}

// ---- createHisto ----
void CbmTrdUnpackMonitor::createHisto(eRawHistos kHisto)
{
  std::string histoname         = "";
  std::shared_ptr<TH1> newhisto = nullptr;

  for (auto moduleid : fModuleIdsVec) {
    auto ncols     = fModuleNrColumns.find(moduleid)->second;
    auto nrows     = fModuleNrRows.find(moduleid)->second;
    auto nchannels = nrows * ncols;

    histoname = "ModuleId_" + std::to_string(moduleid) + "-";
    histoname += getTypeName(kHisto) + "_";
    histoname += getHistoName(kHisto);
    switch (kHisto) {
      case eRawHistos::kSignalshape:
      case eRawHistos::kSignalshape_St:
      case eRawHistos::kSignalshape_Nt:
        newhisto = std::make_shared<TH2I>(histoname.data(), histoname.data(), 32, -0.5, 31.5, 520, -260, 260);
        newhisto->SetXTitle("ADC Sample [CC]");
        newhisto->SetYTitle("ADC Value [a.u.]");
        break;
      case eRawHistos::kMap:
      case eRawHistos::kMap_St:
      case eRawHistos::kMap_Nt:
        newhisto = std::make_shared<TH2I>(histoname.data(), histoname.data(), 42, -0.5, 41.5, 16, -0.5, 15.5);
        newhisto->SetXTitle("ElinkId");
        newhisto->SetYTitle("eLink-ChannelId");
        break;
      case eRawHistos::kElinkId:
        newhisto = std::make_shared<TH1I>(histoname.data(), histoname.data(), 42, -0.5, 41.5);
        newhisto->SetXTitle("ElinkId");
        newhisto->SetYTitle("Counts");
        break;
      case eRawHistos::kSampleDistStdDev:
        newhisto = std::make_shared<TH1F>(histoname.data(), histoname.data(), 200, 0.0, 100.0);
        newhisto->SetXTitle("ADC signal std. deviation [#sigma]");
        newhisto->SetYTitle("Counts");
        break;
      case eRawHistos::kSample0perChannel:
        newhisto = std::make_shared<TH2I>(histoname.data(), histoname.data(), nchannels, -0.5, (nchannels - 0.5), 601,
                                          -300, 300);
        newhisto->SetXTitle("ChannelId");
        newhisto->SetYTitle("ADC Value(Sample-0) [a.u.]");
        break;
      case eRawHistos::kHitType:
        newhisto = std::make_shared<TH1I>(histoname.data(), histoname.data(),
                                          static_cast<Int_t>(Spadic::eTriggerType::kGlobal) + 1, -0.5,
                                          (static_cast<Int_t>(Spadic::eTriggerType::kSandN) + 0.5));
        newhisto->SetXTitle("Spadic::eTriggerType");
        newhisto->SetYTitle("Counts");
        break;
      default: return; break;
    }
    LOG(debug) << Class_Name() << "::CreateHisto() HistoRaw " << static_cast<size_t>(kHisto) << " Module " << moduleid
               << "initialized as " << histoname.data();
    /** @todo the part below is in principle copy paste for all histo types, so we should be able to move this to a single function */
    if (newhisto) { addHistoToMap<eRawHistos>(newhisto, &fRawHistoMap, moduleid, kHisto); }
  }
}

// ---- createHisto ----
void CbmTrdUnpackMonitor::createHisto(eOtherHistos kHisto)
{
  std::string histoname         = "";
  std::shared_ptr<TH1> newhisto = nullptr;

  for (auto moduleid : fModuleIdsVec) {
    histoname = "ModuleId_" + std::to_string(moduleid) + "-";
    histoname += getTypeName(kHisto) + "_";
    histoname += getHistoName(kHisto);
    switch (kHisto) {
      case eOtherHistos::kSpadic_Info_Types: {
        auto nbins = static_cast<std::uint32_t>(Spadic::MsInfoType::kNInfMsgs);
        newhisto   = std::make_shared<TH1I>(histoname.data(), histoname.data(), nbins, -0.5, nbins - 0.5);
        newhisto->SetXTitle("Info message type");
        newhisto->SetYTitle("Counts");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(Spadic::MsInfoType::kBOM) + 1), "BOM");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(Spadic::MsInfoType::kMSB) + 1), "MSB");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(Spadic::MsInfoType::kBUF) + 1), "BUF");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(Spadic::MsInfoType::kUNU) + 1), "UNU");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(Spadic::MsInfoType::kMIS) + 1), "MIS");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(Spadic::MsInfoType::kChannelBuf) + 1), "ChBuf");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(Spadic::MsInfoType::kOrdFifoBuf) + 1), "OrdFifoBuf");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(Spadic::MsInfoType::kChannelBufM) + 1), "ChBufMH");
        ;
        break;
      }
      case eOtherHistos::kMs_Flags: {
        auto nbins = static_cast<std::uint32_t>(fles::MicrosliceFlags::DataError) + 1;
        newhisto   = std::make_shared<TH1I>(histoname.data(), histoname.data(), nbins, -0.5, nbins - 0.5);
        newhisto->SetXTitle("#muSlice info/error flags");
        newhisto->SetYTitle("Counts");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(fles::MicrosliceFlags::CrcValid) + 1), "CrcValid");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(fles::MicrosliceFlags::OverflowFlim) + 1), "OverflowFlim");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(fles::MicrosliceFlags::OverflowUser) + 1), "OverflowUser");
        newhisto->GetXaxis()->SetBinLabel((static_cast<int>(fles::MicrosliceFlags::DataError) + 1), "DataError");
        break;
      }
      default: return; break;
    }
    LOG(debug) << Class_Name() << "::CreateHisto() HistoOther " << static_cast<size_t>(kHisto) << " Module " << moduleid
               << "initialized as " << histoname.data();

    if (newhisto) { addHistoToMap<eOtherHistos>(newhisto, &fOtherHistoMap, moduleid, kHisto); }
  }
}

// ---- createHistos ----
void CbmTrdUnpackMonitor::createHistos()
{
  for (auto kHisto : fActiveDigiHistos) {
    createHisto(kHisto);
  }
  for (auto kHisto : fActiveRawHistos) {
    createHisto(kHisto);
  }
  for (auto kHisto : fActiveOtherHistos) {
    createHisto(kHisto);
  }
}

// ---- fillHisto ----
void CbmTrdUnpackMonitor::fillHisto(CbmTrdDigi* digi, eDigiHistos kHisto, std::uint32_t moduleid,
                                    std::shared_ptr<TH1> histo)
{
  auto triggerpair = digi->GetTriggerPair(digi->GetTriggerType());
  auto triggertype = triggerpair.first;
  auto isMultihit  = triggerpair.second;
  switch (kHisto) {
    case eDigiHistos::kMap_St: {
      if (triggertype != CbmTrdDigi::eTriggerType::kSelf) { return; }
      else {
        auto addresspair = getRowAndCol(moduleid, digi->GetAddressChannel());
        histo->Fill(addresspair.second, addresspair.first);
        break;
      }
    }
    case eDigiHistos::kMap_Nt: {
      if (triggertype != CbmTrdDigi::eTriggerType::kNeighbor) { return; }
      else {
        auto addresspair = getRowAndCol(moduleid, digi->GetAddressChannel());
        histo->Fill(addresspair.second, addresspair.first);
        break;
      }
    }
    case eDigiHistos::kMap: {
      auto addresspair = getRowAndCol(moduleid, digi->GetAddressChannel());
      histo->Fill(addresspair.second, addresspair.first);
      break;
    }

    case eDigiHistos::kCharge: histo->Fill(digi->GetCharge()); break;
    case eDigiHistos::kCharge_St:
      if (triggertype != CbmTrdDigi::eTriggerType::kSelf) { return; }
      histo->Fill(digi->GetCharge());
      break;
    case eDigiHistos::kCharge_Nt:
      if (triggertype != CbmTrdDigi::eTriggerType::kNeighbor) { return; }
      histo->Fill(digi->GetCharge());
      break;

    case eDigiHistos::kChannel: histo->Fill(digi->GetAddressChannel()); break;
    case eDigiHistos::kChannel_St:
      if (triggertype != CbmTrdDigi::eTriggerType::kSelf) { return; }
      histo->Fill(digi->GetAddressChannel());
      break;
    case eDigiHistos::kChannel_Nt:
      if (triggertype != CbmTrdDigi::eTriggerType::kNeighbor) { return; }
      histo->Fill(digi->GetAddressChannel());
      break;

    case eDigiHistos::kTriggerType: {
      if (isMultihit) histo->Fill(static_cast<int>(CbmTrdDigi::eTriggerType::kMulti));
      histo->Fill(static_cast<int>(triggertype));
      break;
    }
    case eDigiHistos::kDigiDeltaT: histo->Fill(digi->GetAddressChannel(), getDeltaT(digi)); break;

    default: return; break;
  }
}

// ---- fillHisto ----
void CbmTrdUnpackMonitor::fillHisto(CbmTrdRawMessageSpadic* raw, eRawHistos kHisto, std::shared_ptr<TH1> histo,
                                    CbmTrdDigi* digi)
{
  auto triggertype = static_cast<Spadic::eTriggerType>(raw->GetHitType());
  switch (kHisto) {
    case eRawHistos::kSignalshape: fillSamplesHisto(histo, raw); break;
    case eRawHistos::kSignalshape_St:
      if (triggertype != Spadic::eTriggerType::kSelf && triggertype != Spadic::eTriggerType::kSandN) return;
      fillSamplesHisto(histo, raw);
      break;
    case eRawHistos::kSignalshape_Nt:
      if (triggertype != Spadic::eTriggerType::kNeigh) return;
      fillSamplesHisto(histo, raw);
      break;
    case eRawHistos::kMap: histo->Fill(raw->GetElinkId(), raw->GetChannelId()); break;
    case eRawHistos::kMap_St:
      if (triggertype != Spadic::eTriggerType::kSelf && triggertype != Spadic::eTriggerType::kSandN) return;
      histo->Fill(raw->GetElinkId(), raw->GetChannelId());
      break;
    case eRawHistos::kMap_Nt:
      if (triggertype != Spadic::eTriggerType::kNeigh) return;
      histo->Fill(raw->GetElinkId(), raw->GetChannelId());
      break;
    case eRawHistos::kElinkId: histo->Fill(raw->GetElinkId()); break;
    case eRawHistos::kSampleDistStdDev: histo->Fill(getSamplesStdDev(raw)); break;
    case eRawHistos::kSample0perChannel: {
      if (!digi) return;
      histo->Fill(digi->GetAddressChannel(), raw->GetSamples()->at(0));
      break;
    }
    case eRawHistos::kHitType: histo->Fill(static_cast<int>(raw->GetHitType())); break;
    default: return; break;
  }
}

// ---- fillSamplesHisto ----
void CbmTrdUnpackMonitor::fillSamplesHisto(std::shared_ptr<TH1> histo, CbmTrdRawMessageSpadic* raw)
{
  for (size_t isample = 0; isample < raw->GetSamples()->size(); isample++) {
    histo->Fill(isample, raw->GetSamples()->at(isample));
  }
}

// ---- getDeltaT ----
std::double_t CbmTrdUnpackMonitor::getDeltaT(CbmTrdDigi* digi)
{
  auto moduleid      = digi->GetAddressModule();
  auto channelid     = digi->GetAddressChannel();
  auto modulevecpair = fLastDigiTimeMap.find(moduleid);
  if (modulevecpair == fLastDigiTimeMap.end()) {
    auto nchannels = fModuleNrColumns.find(moduleid)->second * fModuleNrRows.find(moduleid)->second;
    std::vector<size_t> channelsvec(nchannels, 0);
    channelsvec.at(channelid) = digi->GetTime();
    auto pair                 = std::make_pair(moduleid, channelsvec);
    fLastDigiTimeMap.emplace(pair);
    return digi->GetTime();
    ;
  }
  else {
    auto prevtime                       = modulevecpair->second.at(channelid);
    modulevecpair->second.at(channelid) = digi->GetTime();
    auto dt                             = digi->GetTime() - prevtime;
    return dt;
  }
}

// ---- getHistoName ----
std::string CbmTrdUnpackMonitor::getHistoName(eDigiHistos kHisto)
{
  std::string histoname = "";

  switch (kHisto) {
    case eDigiHistos::kMap: histoname += "Map"; break;
    case eDigiHistos::kMap_St: histoname += "Map_St"; break;
    case eDigiHistos::kMap_Nt: histoname += "Map_Nt"; break;
    case eDigiHistos::kCharge: histoname += "Charge"; break;
    case eDigiHistos::kCharge_St: histoname += "Charge_St"; break;
    case eDigiHistos::kCharge_Nt: histoname += "Charge_Nt"; break;
    case eDigiHistos::kChannel: histoname += "Channel"; break;
    case eDigiHistos::kChannel_St: histoname += "Channel_St"; break;
    case eDigiHistos::kChannel_Nt: histoname += "Channel_Nt"; break;
    case eDigiHistos::kTriggerType: histoname += "TriggerType"; break;
    case eDigiHistos::kDigiDeltaT: histoname += "DigiDeltaT"; break;
  }
  return histoname;
}

// ---- getHistoName ----
std::string CbmTrdUnpackMonitor::getHistoName(eRawHistos kHisto)
{
  std::string histoname = "";

  switch (kHisto) {
    case eRawHistos::kSignalshape: histoname += "Signalshape"; break;
    case eRawHistos::kSignalshape_St: histoname += "Signalshape_St"; break;
    case eRawHistos::kSignalshape_Nt: histoname += "Signalshape_Nt"; break;
    case eRawHistos::kMap: histoname += "Map"; break;
    case eRawHistos::kMap_St: histoname += "Map_St"; break;
    case eRawHistos::kMap_Nt: histoname += "Map_Nt"; break;
    case eRawHistos::kElinkId: histoname += "ElinkId"; break;
    case eRawHistos::kSampleDistStdDev: histoname += "SampleDistStdDev"; break;
    case eRawHistos::kSample0perChannel: histoname += "Sample0perChannel"; break;
    case eRawHistos::kHitType: histoname += "HitType"; break;
  }
  return histoname;
}

// ---- getHistoName ----
std::string CbmTrdUnpackMonitor::getHistoName(eOtherHistos kHisto)
{
  std::string histoname = "";

  switch (kHisto) {
    case eOtherHistos::kSpadic_Info_Types: histoname += "Spadic_Info_Types"; break;
    case eOtherHistos::kMs_Flags: histoname += "Ms_Flags"; break;
  }
  return histoname;
}

// ---- getHistoType ----
std::string CbmTrdUnpackMonitor::getHistoType(std::shared_ptr<TH1> histo)
{
  std::string histoname = histo->GetName();
  // The type follows on the module id separated by the first occurance of "-"
  auto startpos = histoname.find_first_of("-");
  // Now we are positioned at "-" so we move one further to get the correct place
  startpos++;
  // Extract the type from the rest of the string
  auto length    = histoname.find_first_of("_", startpos) - startpos;
  auto histotype = histoname.substr(startpos, length);

  return histotype;
}

// ---- getRowAndCol ----
std::pair<std::uint32_t, std::uint32_t> CbmTrdUnpackMonitor::getRowAndCol(std::uint32_t moduleid,
                                                                          std::uint32_t channelid)
{
  // Check if we have to rotate the address, is true in case of 180(2) or 270(3) degree rotation
  bool doRotate     = fModuleOrientation.find(moduleid)->second / 2;
  bool isYsensitive = fModuleOrientation.find(moduleid)->second % 2;
  auto nrows        = static_cast<std::uint32_t>(fModuleNrRows.find(moduleid)->second);
  auto ncols        = static_cast<std::uint32_t>(fModuleNrColumns.find(moduleid)->second);

  // Potentially rotate the address
  std::uint32_t rotatedid = doRotate ? (channelid * (-1) + (nrows * ncols) - 1) : channelid;

  auto row = rotatedid / ncols;
  auto col = rotatedid % ncols;

  // In case we have a chamber with the columns along x we return row,col
  if (!isYsensitive) return std::make_pair(row, col);
  else
    // In case we have a chamber with the columns along y we return col,row
    return std::make_pair(col, row);
}

// ---- getSamplesStdDev ----
std::float_t CbmTrdUnpackMonitor::getSamplesStdDev(CbmTrdRawMessageSpadic* raw)
{
  std::float_t sum = std::accumulate(raw->GetSamples()->begin(), raw->GetSamples()->end(), 0);

  std::float_t mean = sum / raw->GetNrSamples();

  std::float_t dev = 0;

  for (auto sample : *raw->GetSamples()) {
    dev += (sample - mean) * (sample - mean);
  }
  return std::sqrt(1.0 / raw->GetNrSamples() * dev);
}

ClassImp(CbmTrdUnpackMonitor)
