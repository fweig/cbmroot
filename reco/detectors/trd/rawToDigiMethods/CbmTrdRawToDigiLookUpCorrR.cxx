/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmTrdRawToDigiLookUpCorrR.h"

#include "CbmTrdSpadic.h"

#include <FairLogger.h>

#include <RtypesCore.h>
#include <TFile.h>
#include <TH2.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>

//---- CbmTrdRawToDigiLookUpCorrR ----
CbmTrdRawToDigiLookUpCorrR::CbmTrdRawToDigiLookUpCorrR(std::string infile, eLookUpModes mode)
  : CbmTrdRawToDigiBaseR()
  , fLookUpMode(mode)
  , fLookUpFilename(infile)
{
  prepareLookUpTables(infile);
}

// ---- Init ----
bool CbmTrdRawToDigiLookUpCorrR::Init(std::string infile)
{
  bool initOk = true;

  fLookUpFilename = infile.empty() ? fLookUpFilename : infile;

  fMaxAdcLookUpMap.clear();
  fTimeshiftLookUpMap.clear();

  prepareLookUpTables(fLookUpFilename);

  // Could be used for checks within prepareLookUpTables
  return initOk;
}


// ---- GetBinTimeShift ----
ULong64_t CbmTrdRawToDigiLookUpCorrR::GetBinTimeShift(const std::vector<std::int16_t>* samples)
{
  Int_t baseline   = GetBaseline(samples);
  fcorrFirstSample = samples->at(fFirstLookUpSamplePos) - baseline;
  fcorrFirstSample = fcorrFirstSample > fSpadic->GetClippingStart() ? fSpadic->GetClippingStart() : fcorrFirstSample;
  fcorrFirstSample = fcorrFirstSample < 0 ? 0 : fcorrFirstSample;

  fcorrSecondSample = samples->at(fSecondLookUpSamplePos) - baseline;
  fcorrSecondSample = fcorrSecondSample > fSpadic->GetClippingStart() ? fSpadic->GetClippingStart() : fcorrSecondSample;
  fcorrSecondSample = fcorrSecondSample < 0 ? 0 : fcorrSecondSample;

  auto timeshift = fTimeshiftLookUpMap[fcorrFirstSample][fcorrSecondSample];

  return timeshift;
}

// --- GetCharge ----
Float_t CbmTrdRawToDigiLookUpCorrR::GetMaxAdcValue(const std::vector<std::int16_t>* /* samples*/)
{
  Double_t charge = 0.0;

  charge = fMaxAdcLookUpMap[fCurrentTimeshift][fcorrSecondSample];


  // Remark: Due to the fact, that we store the charge UInt_t in the Digi values below 0 are not allowed.
  // In this case the above only appears if the baseline fluctuated above all values in the applied peaking range. This can only happen for forced neighbor triggers with a deposited charged that can not be separated from the baseline.
  return charge;
}

// ---- prepareLookUpTables ----
void CbmTrdRawToDigiLookUpCorrR::prepareLookUpTables(std::string infile)
{
  switch (fLookUpMode) {
    case eLookUpModes::kTwoSamplesDynamicAnalytic: createAnalyticLookUpTables(); break;
    case eLookUpModes::kTwoSamplesFileInput:
      if (infile.empty())
        LOG(fatal) << "CbmTrdRawToDigiLookUpCorrR::prepareLookUpTables: Look up mode that reqiures an input file "
                      "requested with empty infile name string!";
      loadLookUpTables(infile);
      break;
  }
}

// ---- loadLookUpTables ----
void CbmTrdRawToDigiLookUpCorrR::loadLookUpTables(std::string infile)
{
  auto oldFile = gFile;

  TFile file(infile.data(), "READ");
  if (file.IsOpen()) {
    std::string hname  = Form("Timeshift_Map_Fst%d_Snd%d", fFirstLookUpSamplePos, fSecondLookUpSamplePos);
    TH2* hTimeshiftMap = (TH2*) file.Get(hname.data());
    if (!hTimeshiftMap)
      LOG(fatal) << "CbmTrdRawToDigiLookUpCorrR::loadLookUpTables: Look up mode that reqiures Timeshift Map "
                    "histogram requested, but the map "
                 << hname << " was not found in the given file(" << infile << ")!";
    hname           = Form("MaxAdc_Map_Fst%d_Snd%d", fFirstLookUpSamplePos, fSecondLookUpSamplePos);
    TH2* hMaxAdcMap = (TH2*) file.Get(hname.data());
    if (!hMaxAdcMap)
      LOG(fatal) << "CbmTrdRawToDigiLookUpCorrR::loadLookUpTables: Look up mode that reqiures MaxAdc Map "
                    "histogram requested, but the map "
                 << hname << " was not found in the given file(" << infile << ")!";

    // TH2* hMaxAdcMap = (TH2*) file.Get("MAX ADC");
    // TH2* hAsymMap   = (TH2*) file.Get("ASYM MAP");

    if (hMaxAdcMap && hTimeshiftMap) {
      auto h = hMaxAdcMap;
      for (Int_t ibinx = 1; ibinx <= h->GetNbinsX(); ibinx++) {
        for (Int_t ibiny = 1; ibiny <= h->GetNbinsY(); ibiny++) {
          fMaxAdcLookUpMap[h->GetXaxis()->GetBinCenter(ibinx)][h->GetYaxis()->GetBinCenter(ibiny)] =
            h->GetBinContent(ibinx, ibiny);
        }
      }

      h = hTimeshiftMap;
      for (Int_t ibinx = 1; ibinx <= h->GetNbinsX(); ibinx++) {
        for (Int_t ibiny = 1; ibiny <= h->GetNbinsY(); ibiny++) {
          fTimeshiftLookUpMap[h->GetXaxis()->GetBinCenter(ibinx)][h->GetYaxis()->GetBinCenter(ibiny)] =
            h->GetBinContent(ibinx, ibiny);
        }
      }
    }
    file.Close();
  }
  else {
    LOG(fatal) << "CbmTrdRawToDigiLookUpCorrR::loadLookUpTables: Look up mode that reqiures an input file "
                  "requested with a wrong file path! Filepath = "
               << infile;
  }
  gFile = oldFile;
}

// ---- loadLookUpTables ----
void CbmTrdRawToDigiLookUpCorrR::createAnalyticLookUpTables()
{
  UInt_t chargeFirstSample  = 0;
  UInt_t chargeSecondSample = 0;

  for (Int_t timeshift = 0; timeshift < fSpadic->GetClockCycle(); timeshift += 1) {
    fSpadic->SetParameter(CbmTrdSpadic::eResponsePars::kBinTimeshift, timeshift);
    for (UInt_t maxAdc = 1; maxAdc < fSpadic->GetDynamicRange() * 2; maxAdc++) {
      // Set the max adc related input charge for the spadic response
      fSpadic->SetParameter(CbmTrdSpadic::eResponsePars::kInputCharge, maxAdc);
      // Get the response for the sample positions
      chargeFirstSample = fSpadic->Response(fFirstLookUpSamplePos);
      // second sample position
      chargeSecondSample = fSpadic->Response(fSecondLookUpSamplePos);
      auto innerpair     = std::make_pair(chargeSecondSample, timeshift);
      auto innermap      = fTimeshiftLookUpMap.find(chargeFirstSample);
      if (innermap != fTimeshiftLookUpMap.end()) { innermap->second.emplace(innerpair); }
      else {
        std::map<UInt_t, ULong64_t> newinnermap = {};
        newinnermap.emplace(innerpair);
        auto outerpair = std::make_pair(chargeFirstSample, newinnermap);
        fTimeshiftLookUpMap.emplace(outerpair);
      }


      // fTimeshiftLookUpMap[chargeFirstSample][chargeSecondSample] = timeshift;
      fMaxAdcLookUpMap[timeshift][chargeSecondSample] = maxAdc;
    }
  }
  ULong64_t timeshift                  = 0;
  std::vector<ULong64_t> timeshiftsvec = {};
  std::vector<UInt_t> emptyChargeVec   = {};

  // Fill up empty bins in the look up. Due to several effects, e.g. noise, real signals can produce other combinations of first and second sample charges than the pure analytical approach. Hence, we the look up also to deliver values for those combinations.
  // Fill up the map, we assume here that the shift does behave steadily, i.e. empty bins after a 62 ns bin will be filled with 62 ns and vise versa empty bins after 0 ns bin will be filled with 0 ns.
  for (auto& innermap : fTimeshiftLookUpMap) {
    for (UInt_t secondsampleCharge = 0; secondsampleCharge < fSpadic->GetDynamicRange(); secondsampleCharge++) {
      if (innermap.second.find(secondsampleCharge) == innermap.second.end())
        emptyChargeVec.emplace_back(secondsampleCharge);
      else {
        timeshift = innermap.second.find(secondsampleCharge)->second;
        for (auto emptycharge : emptyChargeVec) {
          auto pair = std::make_pair(emptycharge, timeshift);
          innermap.second.emplace(pair);
        }
        emptyChargeVec.clear();
      }
    }
    for (auto emptycharge : emptyChargeVec) {
      auto pair = std::make_pair(emptycharge, timeshift);
      innermap.second.emplace(pair);
    }
    emptyChargeVec.clear();
  }
}

ClassImp(CbmTrdRawToDigiLookUpCorrR)
