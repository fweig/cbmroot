/* Copyright (C) 2022 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci [committer] */

#include "CbmTrdUnpackFaspMonitor.h"

#include "CbmTrdDigi.h"
#include "CbmTrdParFasp.h"

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

//_________________________________________________________________________
CbmTrdUnpackFaspMonitor::CbmTrdUnpackFaspMonitor(/* args */) {}

//_________________________________________________________________________
CbmTrdUnpackFaspMonitor::~CbmTrdUnpackFaspMonitor() {}

//_________________________________________________________________________
void CbmTrdUnpackFaspMonitor::FillHistos(CbmTrdDigi* digi)
{
  auto moduleid = digi->GetAddressModule();
  for (auto pair : fDigiHistoMap) {
    auto modulepair = pair.second.find(moduleid);
    auto histo      = modulepair->second;
    fillHisto(digi, pair.first, moduleid, histo);
  }
}

//_________________________________________________________________________
void CbmTrdUnpackFaspMonitor::Finish()
{
  LOG(info) << Class_Name() << "::Finish() - ";

  if (fDoWriteToFile) {
    size_t nhistos = 0;

    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    /// (Re-)Create ROOT file to store the histos
    TFile histofile(fOutfilename.data(), "UPDATE");

    nhistos += writeHistosToFile(&fDigiHistoMap, &histofile);
    nhistos += writeHistosToFile(&fRawHistoMap, &histofile);

    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;

    // histofile->Close();
    histofile.Close();

    LOG(info) << Class_Name() << "::Finish() nHistos " << nhistos << " written to " << fOutfilename.data();
  }
}

//_________________________________________________________________________
Bool_t CbmTrdUnpackFaspMonitor::Init()
{

  if (!fModuleDef.size()) return kFALSE;

  // Check if there is a HttpServer and if so we later on register the created histograms in it.
  if (FairRun::Instance()->IsA() == FairRunOnline::Class()) {
    auto run = static_cast<FairRunOnline*>(FairRun::Instance());

    fHistoServer = run->GetHttpServer();
  }

  // Purge the module list. Remove non-FASP modules
  auto it = fModuleDef.begin();
  while (it != fModuleDef.end()) {
    size_t nchs = std::get<0>((*it).second).size();
    if (nchs == 0) it = fModuleDef.erase(it);
    else
      it++;
  }
  createHistos();

  return kTRUE;
}

//_________________________________________________________________________
void CbmTrdUnpackFaspMonitor::addParam(uint32_t madd, const CbmTrdParSetAsic* asics)
{
  auto moduleDef = fModuleDef.find(madd);
  if (moduleDef == fModuleDef.end()) {
    LOG(warning) << Class_Name() << "::addParam(CbmTrdParSetAsic*) Module " << madd << " not in the list ! Skip.";
    return;
  }

  std::vector<int32_t> padToFaspMapping(NFASPMOD * NFASPCH, -1);
  std::vector<Int_t> add;
  asics->GetAsicAddresses(&add);
  for (auto afasp : add) {
    CbmTrdParFasp* fasp = (CbmTrdParFasp*) asics->GetModulePar(afasp);

    int faspid(afasp % 1000), ich(faspid * 100);
    for (auto ach : fasp->GetChannelAddresses()) {
      padToFaspMapping[ach] = ich;
      ich++;
    }
  }
  std::get<0>((*moduleDef).second) = padToFaspMapping;
}

//_________________________________________________________________________
void CbmTrdUnpackFaspMonitor::addParam(uint32_t madd, CbmTrdParModDigi* pp)
{
  auto moduleDef = fModuleDef.find(madd);
  if (moduleDef == fModuleDef.end())
    fModuleDef.emplace(madd, std::make_tuple(std::vector<int32_t>(), pp->GetNofColumns(), pp->GetNofRows()));
  else {
    LOG(warning) << Class_Name() << "::addParam(CbmTrdParModDigi*) Module " << madd
                 << " already loaded ! Replacing pad-plane definition.";
    std::get<1>((*moduleDef).second) = pp->GetNofColumns();
    std::get<2>((*moduleDef).second) = pp->GetNofRows();
  }
}

//_________________________________________________________________________
void CbmTrdUnpackFaspMonitor::createHisto(eDigiHistos kHisto)
{
  std::string histoname         = "";
  std::shared_ptr<TH1> newhisto = nullptr;

  for (auto moduleDef : fModuleDef) {
    auto modId = moduleDef.first;
    auto ncols = std::get<1>(moduleDef.second);
    auto nrows = std::get<2>(moduleDef.second);
    auto nchs  = nrows * ncols;

    histoname = "Fasp_" + getTypeName(kHisto) + "_" + getHistoName(kHisto);
    switch (kHisto) {
      case eDigiHistos::kMap:
        newhisto = std::make_shared<TH2I>(histoname.data(), Form("%s %d", histoname.data(), modId), 2 * ncols, -0.5,
                                          (ncols - 0.5), nrows, -0.5, (nrows - 0.5));
        newhisto->SetXTitle("COL (pad)");
        newhisto->SetYTitle("ROW (pad)");
        newhisto->SetZTitle("Yield");
        break;

      case eDigiHistos::kCharge:
        newhisto = std::make_shared<TH2I>(histoname.data(), Form("%s %d", histoname.data(), modId), 2 * nchs, -0.5,
                                          (nchs - 0.5), 4095, 0.5, 4095.5);
        newhisto->SetXTitle("Pad-Id");
        newhisto->SetYTitle("Sgn [ADU]");
        newhisto->SetZTitle("Yield");
        break;
      case eDigiHistos::kChannel:
        newhisto = std::make_shared<TH2I>(histoname.data(), Form("%s %d", histoname.data(), modId), NFASPMOD, -0.5,
                                          (NFASPMOD - 0.5), NFASPCH, -0.5, NFASPCH - 0.5);
        newhisto->SetXTitle("FASP-Id");
        newhisto->SetYTitle("FASP-Ch");
        newhisto->SetZTitle("Yield");
        break;
      case eDigiHistos::kDigiDeltaT:
        newhisto = std::make_shared<TH2I>(histoname.data(), Form("%s %d", histoname.data(), modId), 2 * nchs, -0.5,
                                          (nchs - 0.5), 1000, 0, 1e2);
        newhisto->SetXTitle("Pad-Id");
        newhisto->SetYTitle("Rate_{SGN} [kHz]");
        newhisto->SetZTitle("Yield");
        break;
      default: return;
    }
    LOG(debug) << Class_Name() << "::CreateHisto() HistoDigi " << static_cast<size_t>(kHisto) << " Module " << modId
               << " initialized as " << histoname.data();
    if (newhisto) { addHistoToMap<eDigiHistos>(newhisto, &fDigiHistoMap, modId, kHisto); }
  }
}

//_________________________________________________________________________
void CbmTrdUnpackFaspMonitor::fillHisto(CbmTrdDigi* d, eDigiHistos kHisto, std::uint32_t moduleid,
                                        std::shared_ptr<TH1> histo)
{
  auto modDef = fModuleDef[moduleid];
  int pad = d->GetAddressChannel(), ncol = std::get<1>(modDef), row = pad / ncol, col = pad % ncol, dtime,
      cht = 2 * pad, chr = cht + 1;
  double t, r            = d->GetCharge(t, dtime);
  switch (kHisto) {
    case eDigiHistos::kMap:
      if (t > 0) histo->Fill(col - 0.25, row);
      if (r > 0) histo->Fill(col + 0.25, row);
      break;

    case eDigiHistos::kCharge:
      if (t > 0) histo->Fill(pad - 0.25, t);
      if (r > 0) histo->Fill(pad + 0.25, r);
      break;

    case eDigiHistos::kChannel: {
      int fasp, faspid = -1, faspch = -1;
      if (t > 0) {
        if ((fasp = std::get<0>(modDef).at(cht)) < 0) {
          LOG(debug) << Class_Name() << "::fillHisto() Missing FASP params for pad-T " << pad;
        }
        else {
          faspid = fasp / 100;
          faspch = fasp % 100;
          histo->Fill(faspid, faspch);
        }
      }

      if (r > 0) {
        if ((fasp = std::get<0>(modDef).at(chr)) < 0) {
          LOG(debug) << Class_Name() << "::fillHisto() Missing FASP params for pad-R " << pad;
        }
        else {
          faspid = fasp / 100;
          faspch = fasp % 100;
          histo->Fill(faspid, faspch);
        }
      }
      break;
    }
    case eDigiHistos::kDigiDeltaT:
      if (t > 0) histo->Fill(pad - 0.25, fFaspInvClk / getDeltaT(moduleid, cht, d->GetTimeDAQ()));
      if (r > 0) histo->Fill(pad + 0.25, fFaspInvClk / getDeltaT(moduleid, chr, d->GetTimeDAQ() + dtime));
      break;

    default: return;
  }
}

//_________________________________________________________________________
std::uint64_t CbmTrdUnpackFaspMonitor::getDeltaT(uint32_t modid, int32_t ch, uint64_t daqt)
{
  auto modulevecpair = fLastDigiTimeMap.find(modid);
  if (modulevecpair == fLastDigiTimeMap.end()) {
    auto modDef = fModuleDef[modid];
    int ncol = std::get<1>(modDef), nrow = std::get<2>(modDef), nchs = 2 * ncol * nrow;
    std::vector<size_t> channelsvec(nchs, 0);

    if (ch > nchs) return 0;
    channelsvec.at(ch) = daqt;
    auto pair          = std::make_pair(modid, channelsvec);
    fLastDigiTimeMap.emplace(pair);
    return daqt;
  }
  else {
    auto prevtime                = modulevecpair->second.at(ch);
    modulevecpair->second.at(ch) = daqt;
    auto dt                      = daqt - prevtime;
    return dt;
  }
}

ClassImp(CbmTrdUnpackFaspMonitor)
