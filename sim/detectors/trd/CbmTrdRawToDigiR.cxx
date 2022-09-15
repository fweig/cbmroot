/* Copyright (C) 2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Etienne Bechtel [committer] */

// Includes from TRD
#include "CbmTrdRawToDigiR.h"

#include "CbmTrdAddress.h"
#include "CbmTrdDigi.h"

#include "TMath.h"
//#include "CbmSpadicRawMessage22.h"
#include <TFile.h>
#include <TProfile.h>
#include <TProfile2D.h>

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "assert.h"

using namespace std::chrono;

//_________________________________________________________________________________
CbmTrdRawToDigiR::CbmTrdRawToDigiR()
  : TObject()
  , fSampleMask()
  , fElookupSmall()
  , fElookupAsym()
  , fElookupA()
  , fElookupBig()
  , fElookup()
{
}

//_________________________________________________________________________________
CbmTrdRawToDigiR::CbmTrdRawToDigiR(std::string readfile)
  : TObject()
  , fSampleMask()
  , fElookupSmall()
  , fElookupAsym()
  , fElookupA()
  , fElookupBig()
  , fElookup()
{
  SetReadFile(readfile);
}


//_________________________________________________________________________________
CbmTrdRawToDigiR::CbmTrdRawToDigiR(Double_t cal, Double_t tau, Int_t mode)
  : TObject()
  , fSampleMask()
  , fElookupSmall()
  , fElookupAsym()
  , fElookupA()
  , fElookupBig()
  , fElookup()
{
  SetCalibration(cal);
  SetTau(tau);
  SetRecoMode(mode);
  SetPars(mode, cal, tau, fSampleMask);
}


//_________________________________________________________________________________
CbmTrdRawToDigiR::CbmTrdRawToDigiR(Double_t cal, Double_t tau, std::vector<Int_t> mask)
  : TObject()
  , fSampleMask()
  , fElookupSmall()
  , fElookupAsym()
  , fElookupA()
  , fElookupBig()
  , fElookup()
{
  SetCalibration(cal);
  SetTau(tau);
  SetRecoMask(mask);
  SetPars(fRecoMode, cal, tau, mask);
}


void CbmTrdRawToDigiR::SetPars(Int_t mode, Double_t cal, Double_t tau, std::vector<Int_t> mask)
{

  //default
  if (mode == 0) {
    for (UInt_t i = 0; i <= mask.size(); i++)
      fSampleMask.push_back(mask[i]);
    fCalibration = cal;
    fTau         = tau;
    Double_t sum = 0;
    for (UInt_t i = 0; i < mask.size(); i++)
      sum += fCalibration * CalcResponse(mask[i] * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC));
    fEReco = sum;
    FillLookUps();
  }
  if (mode == 1) {
    for (Int_t i = fMaxBin; i <= fMaxBin; i++)
      fSampleMask.push_back(i);
    fCalibration = cal;
    fTau         = tau;
    fLookUp      = 0;
    Double_t sum = 0;
    for (UInt_t i = 0; i < fSampleMask.size(); i++)
      sum += fCalibration * CalcResponse(fSampleMask[i] * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC));
    fEReco     = sum;
    auto start = high_resolution_clock::now();
    FillLookUps();
    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    if (fDebug)
      std::cout << std::endl
                << std::endl
                << "filling took: " << duration.count() << "  microseconds " << std::endl
                << fEReco << std::endl
                << std::endl
                << std::endl;
  }
  if (mode == 2) {
    Double_t sum = 0;
    for (UInt_t i = 0; i < fSampleMask.size(); i++)
      sum += fCalibration * CalcResponse(fSampleMask[i] * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC));
    fEReco     = sum;
    auto start = high_resolution_clock::now();
    FillLookUps();
    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    if (fDebug)
      std::cout << std::endl
                << std::endl
                << "filling took: " << duration.count() << "  microseconds " << std::endl
                << fEReco << std::endl
                << std::endl
                << std::endl;
  }
}


void CbmTrdRawToDigiR::Init()
{
  //default
  Double_t sum = 0;
  for (UInt_t i = 0; i < fSampleMask.size(); i++)
    sum += fCalibration * CalcResponse(fSampleMask[i] * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC));
  fEReco     = sum;
  auto start = high_resolution_clock::now();
  if (fReadFile == "") FillLookUps(fWriteFile);
  else
    ReadMaps(fReadFile);
  if (fLookUp == 4) FillLookUps("");
  auto stop     = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  if (fDebug)
    std::cout << std::endl
              << std::endl
              << "filling took: " << duration.count() << "  microseconds " << std::endl
              << fEReco << std::endl
              << std::endl
              << std::endl;
  // if(fShapingOrder == 1)     {SetMaxBin(2+fPresamples);SetMinBin(1+fPresamples);}
  // if(fShapingOrder == 2)     SetMaxBin(4+fPresamples);
}


void CbmTrdRawToDigiR::FillLookUps(std::string write)
{

  if (fLookUp == 1) {
    for (Int_t shift = 0.; shift < CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC); shift++) {
      Double_t sum = 0;
      for (Int_t i = fMinBin; i <= fMaxBin; i++)
        sum +=
          fCalibration * CalcResponse(fSampleMask[i] * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC) + shift);
      fEReco = sum;
      Float_t temp =
        fCalibration * CalcResponse(fMaxBin * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC) + shift);

      for (Int_t max = 0; max < fDynamicRange; max++) {
        Float_t energy = max * 1.0 / temp;
        Int_t a =
          energy * fCalibration * CalcResponse(fMinBin * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC) + shift);
        Int_t b = energy * fCalibration
                  * CalcResponse((fHighBin) *CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC) + shift);

        Int_t mina = a - fExtrapolate * a;
        Int_t maxa = a + fExtrapolate * a;
        Int_t minb = b - fExtrapolate * b;
        Int_t maxb = b + fExtrapolate * b;
        if (!(fElookupAsym[max][a][b] > 0)) fElookupAsym[max][a][b] = shift;
        fElookupSmall[shift][max] = energy;
        if (fDebug) fQA->CreateHist("Asym", 512, -12.0, 500.0, 512, -12.0, 500.0);
        if (fDebug && max == 200 && fQA->GetCont2D("Asym", a, b) == 0.) fQA->Fill("Asym", a, b, shift);
        if (fDebug) fQA->CreateHist("Look", 63, -0.5, 62.5, 512, -12.0, 500.0);
        if (fDebug) fQA->Fill("Look", shift, max, energy);
        Int_t extrapolate = 0;
        while (true) {
          extrapolate++;
          if ((b - extrapolate) <= minb || (a - extrapolate) <= mina) break;
          if (fElookupAsym[max][a - extrapolate][b - extrapolate] > 0) continue;
          fElookupAsym[max][a - extrapolate][b - extrapolate] = fElookupAsym[max][a][b];
          if (fDebug && max == 200) fQA->Fill("Asym", a - extrapolate, b - extrapolate, fElookupAsym[max][a][b]);
          Int_t count = 1;
          if (!(fElookupAsym[max][a - extrapolate - count][b - extrapolate] > 0)) {
            fElookupAsym[max][a - extrapolate - count][b - extrapolate] =
              fElookupAsym[max][a - extrapolate][b - extrapolate];
            if (fDebug && max == 200)
              fQA->Fill("Asym", a - extrapolate - count, b - extrapolate, fElookupAsym[max][a][b]);
            count++;
          }
          count = 1;
          if (!(fElookupAsym[max][a - extrapolate + count][b - extrapolate] > 0)) {
            fElookupAsym[max][a - extrapolate + count][b - extrapolate] =
              fElookupAsym[max][a - extrapolate][b - extrapolate];
            if (fDebug && max == 200)
              fQA->Fill("Asym", a - extrapolate + count, b - extrapolate, fElookupAsym[max][a][b]);
            count++;
          }
        }
        extrapolate = 0;
        while (true) {
          extrapolate++;
          if ((b + extrapolate) >= maxb || (a + extrapolate) >= maxa) break;
          if (fElookupAsym[max][a + extrapolate][b + extrapolate] > 0) continue;
          fElookupAsym[max][a + extrapolate][b + extrapolate] = fElookupAsym[max][a][b];
          if (fDebug && max == 200) fQA->Fill("Asym", a + extrapolate, b + extrapolate, fElookupAsym[max][a][b]);
          Int_t count = 1;
          if (!(fElookupAsym[max][a + extrapolate - count][b + extrapolate] > 0)) {
            fElookupAsym[max][a + extrapolate - count][b + extrapolate] =
              fElookupAsym[max][a + extrapolate][b + extrapolate];
            if (fDebug && max == 200)
              fQA->Fill("Asym", a + extrapolate - count, b + extrapolate, fElookupAsym[max][a][b]);
            count++;
          }
          count = 1;
          if (!(fElookupAsym[max][a + extrapolate + count][b + extrapolate] > 0)) {
            fElookupAsym[max][a + extrapolate + count][b + extrapolate] =
              fElookupAsym[max][a + extrapolate][b + extrapolate];
            if (fDebug && max == 200)
              fQA->Fill("Asym", a + extrapolate + count, b + extrapolate, fElookupAsym[max][a][b]);
            count++;
          }
        }
      }
    }
    if (write != "") WriteMaps(write);
  }
  if (fLookUp == 2) {
    for (Int_t shift = 0.; shift < CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC); shift++) {
      Double_t sum = 0;
      for (Int_t i = fMinBin; i <= fMaxBin; i++)
        sum +=
          fCalibration * CalcResponse(fSampleMask[i] * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC) + shift);
      fEReco = sum;
      Float_t temp =
        fCalibration * CalcResponse(fMaxBin * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC) + shift);
      Int_t extrapolate = 0;
      for (Int_t max = 0; max < fDynamicRange; max++) {
        Float_t energy = max * 1.0 / temp;
        Int_t a =
          energy * fCalibration * CalcResponse(fMinBin * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC) + shift);
        Int_t b = energy * fCalibration
                  * CalcResponse((fHighBin) *CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC) + shift);

        Int_t mina = a - fExtrapolate * a;
        Int_t maxa = a + fExtrapolate * a;
        Int_t minb = b - fExtrapolate * b;
        Int_t maxb = b + fExtrapolate * b;
        if (!(fElookupAsym[max][a][b] > 0)) fElookupAsym[max][a][b] = shift;
        sum = 0.;
        for (UInt_t i = 0; i < fSampleMask.size(); i++)
          sum += energy * fCalibration
                 * CalcResponse(fSampleMask[i] * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC) + shift);

        fElookupSmall[shift][sum] = energy;
        if (fDebug) fQA->CreateHist("Asym", 512, -12.0, 500.0, 512, -12.0, 500.0);
        if (fDebug && max == 200 && fQA->GetCont2D("Asym", a, b) == 0.) fQA->Fill("Asym", a, b, shift);
        if (fDebug) fQA->CreateHist("Look", 63, -0.5, 62.5, 512, -12.0, 2000.0);
        if (fDebug) fQA->Fill("Look", shift, sum, energy);
        extrapolate = 0;
        while (true) {
          extrapolate++;
          if ((b - extrapolate) <= minb || (a - extrapolate) <= mina) break;
          if (fElookupAsym[max][a - extrapolate][b - extrapolate] > 0) continue;
          fElookupAsym[max][a - extrapolate][b - extrapolate] = fElookupAsym[max][a][b];
          if (fDebug && max == 200) fQA->Fill("Asym", a - extrapolate, b - extrapolate, fElookupAsym[max][a][b]);
          Int_t count = 1;
          if (!(fElookupAsym[max][a - extrapolate - count][b - extrapolate] > 0)) {
            fElookupAsym[max][a - extrapolate - count][b - extrapolate] =
              fElookupAsym[max][a - extrapolate][b - extrapolate];
            if (fDebug && max == 200)
              fQA->Fill("Asym", a - extrapolate - count, b - extrapolate, fElookupAsym[max][a][b]);
            count++;
          }
          count = 1;
          if (!(fElookupAsym[max][a - extrapolate + count][b - extrapolate] > 0)) {
            fElookupAsym[max][a - extrapolate + count][b - extrapolate] =
              fElookupAsym[max][a - extrapolate][b - extrapolate];
            if (fDebug && max == 200)
              fQA->Fill("Asym", a - extrapolate + count, b - extrapolate, fElookupAsym[max][a][b]);
            count++;
          }
        }
        extrapolate = 0;
        while (true) {
          extrapolate++;
          if ((b + extrapolate) >= maxb || (a + extrapolate) >= maxa) break;
          if (fElookupAsym[max][a + extrapolate][b + extrapolate] > 0) continue;
          fElookupAsym[max][a + extrapolate][b + extrapolate] = fElookupAsym[max][a][b];
          if (fDebug && max == 200) fQA->Fill("Asym", a + extrapolate, b + extrapolate, fElookupAsym[max][a][b]);
          Int_t count = 1;
          if (!(fElookupAsym[max][a + extrapolate - count][b + extrapolate] > 0)) {
            fElookupAsym[max][a + extrapolate - count][b + extrapolate] =
              fElookupAsym[max][a + extrapolate][b + extrapolate];
            if (fDebug && max == 200)
              fQA->Fill("Asym", a + extrapolate - count, b + extrapolate, fElookupAsym[max][a][b]);
            count++;
          }
          count = 1;
          if (!(fElookupAsym[max][a + extrapolate + count][b + extrapolate] > 0)) {
            fElookupAsym[max][a + extrapolate + count][b + extrapolate] =
              fElookupAsym[max][a + extrapolate][b + extrapolate];
            if (fDebug && max == 200)
              fQA->Fill("Asym", a + extrapolate + count, b + extrapolate, fElookupAsym[max][a][b]);
            count++;
          }
        }
      }
    }
    if (write != "") WriteMaps(write);
  }
  if (fLookUp == 4) {
    for (Int_t max = 0; max < fDynamicRange; max++) {
      Float_t energy =
        max / (fCalibration * CalcResponse(fMaxBin * CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC)));
      fElookup[max] = energy;
    }
  }
  else {
    Int_t range = fDynamicRange * fSampleMask.size();
    for (Int_t n = 0; n <= range; n++) {
      Float_t value = n / fEReco;
      fElookup[n]   = value;
    }
  }
}


void CbmTrdRawToDigiR::ReadMaps(std::string file)
{
  if (fLookUp == 3) {
    TFile f(file.data(), "OPEN");
    LOG_IF(fatal, !f.IsOpen()) << "parameter file " << file << " does not exist!";
    TProfile2D* h;
    h = f.Get<TProfile2D>("MAX ADC");
    LOG_IF(fatal, !h) << "No histogram MAX ADC found in file " << file;
    for (Int_t x = 1; x <= h->GetNbinsX(); x++) {
      for (Int_t y = 1; y <= h->GetNbinsY(); y++) {
        fElookupSmall[h->GetXaxis()->GetBinCenter(x)][h->GetYaxis()->GetBinCenter(y)] = h->GetBinContent(x, y);
        if (fDebug) fQA->CreateHist("Read Small", 63, -0.5, 62.5, 512, -12., 500.);
        if (fDebug)
          fQA->Fill("Read Small", h->GetXaxis()->GetBinCenter(x), h->GetYaxis()->GetBinCenter(y),
                    h->GetBinContent(x, y));
      }
    }
    h = f.Get<TProfile2D>("ASYM MAP");
    LOG_IF(fatal, !h) << "No histogram ASYM MAP found in file " << file;
    for (Int_t x = 1; x <= h->GetNbinsX(); x++) {
      for (Int_t y = 1; y <= h->GetNbinsY(); y++) {
        fElookupA[h->GetXaxis()->GetBinCenter(x)][h->GetYaxis()->GetBinCenter(y)] = h->GetBinContent(x, y);
        if (fDebug) fQA->CreateHist("Read Asym", 512, 0., 512., 512, -12., 500.);
        if (fDebug)
          fQA->Fill("Read Asym", h->GetXaxis()->GetBinCenter(x), h->GetYaxis()->GetBinCenter(y),
                    h->GetBinContent(x, y));
      }
    }
    f.Close();
  }
  if (fLookUp == 4) {
    TFile f(file.data(), "OPEN");
    LOG_IF(fatal, !f.IsOpen()) << "parameter file " << file << " does not exist!";
    TProfile2D* h;
    h = f.Get<TProfile2D>("MAX ADC");
    LOG_IF(fatal, !h) << "No histogram MAX ADC found in file " << file;
    for (Int_t x = 1; x <= h->GetNbinsX(); x++) {
      for (Int_t y = 1; y <= h->GetNbinsY(); y++) {
        fElookupSmall[h->GetXaxis()->GetBinCenter(x)][h->GetYaxis()->GetBinCenter(y)] = h->GetBinContent(x, y);
        if (fDebug) fQA->CreateHist("Read Small", 63, -0.5, 62.5, 512, -12., 500.);
        if (fDebug)
          fQA->Fill("Read Small", h->GetXaxis()->GetBinCenter(x), h->GetYaxis()->GetBinCenter(y),
                    h->GetBinContent(x, y));
      }
    }
    h = f.Get<TProfile2D>("ASYM MAP");
    LOG_IF(fatal, !h) << "No histogram ASYM MAP found in file " << file;
    for (Int_t x = 1; x <= h->GetNbinsX(); x++) {
      for (Int_t y = 1; y <= h->GetNbinsY(); y++) {
        fElookupA[h->GetXaxis()->GetBinCenter(x)][h->GetYaxis()->GetBinCenter(y)] = h->GetBinContent(x, y);
        if (fDebug) fQA->CreateHist("Read Asym", 512, 0., 512., 512, -12., 500.);
        if (fDebug)
          fQA->Fill("Read Asym", h->GetXaxis()->GetBinCenter(x), h->GetYaxis()->GetBinCenter(y),
                    h->GetBinContent(x, y));
      }
    }
    f.Close();
  }
}

CbmTrdDigi* CbmTrdRawToDigiR::MakeDigi(std::vector<Int_t> samples, Int_t channel, Int_t uniqueModuleId, ULong64_t time,
                                       Bool_t FN)
{
  Float_t digicharge = 0;
  Int_t samplesum    = 0;
  for (size_t i = 0; i < fSampleMask.size(); i++) {
    samplesum += samples[fSampleMask[i]];
  }
  if (fReadFile == "") {
    if (fLookUp == 1) {
      digicharge = fElookupSmall[fElookupAsym[samples[fMaxBin]][samples[fMinBin]][samples[fHighBin]]][samples[fMaxBin]];
    }
    if (fLookUp == 2) {
      digicharge = fElookupSmall[fElookupAsym[samples[fMaxBin]][samples[fMinBin]][samples[fHighBin]]][samplesum];
    }
    else {
      digicharge = fElookup[samplesum];
    }
  }
  else {
    if (fLookUp == 3) { digicharge = fElookupSmall[fElookupA[samples[fMinBin]][samples[fMaxBin]]][samples[fMaxBin]]; }
    if (fLookUp == 4 && !FN) {
      digicharge = fElookupSmall[fElookupA[samples[fMinBin]][samples[fMaxBin]]][samples[fMaxBin]];
    }
    if (fLookUp == 4 && FN) { digicharge = fElookup[samples[fMaxBin]]; }
  }
  // The triggertype is set later by the class (moduleSimR) using this function
  CbmTrdDigi* digi = new CbmTrdDigi(channel, uniqueModuleId, digicharge, time, CbmTrdDigi::eTriggerType::kNTrg, 0);

  return digi;
}

Float_t CbmTrdRawToDigiR::GetTimeShift(std::vector<Int_t> samples)
{

  Float_t shift = 0;
  if (fReadFile == "") {
    if (fLookUp == 1) { shift = fElookupAsym[samples[fMaxBin]][samples[fMinBin]][samples[fHighBin]]; }
    else {
      return 0.;
    }
  }
  else {
    if (fLookUp == 3) { shift = fElookupA[samples[fMinBin]][samples[fMaxBin]]; }
    if (fLookUp == 4) { shift = fElookupA[samples[fMinBin]][samples[fMaxBin]]; }
  }

  return shift;
}

Double_t CbmTrdRawToDigiR::GetCharge(std::vector<Int_t> samples, Int_t shift)
{

  Float_t charge = 0;
  if (fReadFile == "") {
    if (fLookUp == 1) {
      if (shift > -1) charge = fElookupSmall[shift][samples[fMaxBin]];
      else
        charge = fElookupSmall[fElookupAsym[samples[fMaxBin]][samples[fMinBin]][samples[fHighBin]]][samples[fMaxBin]];
    }
    if (fLookUp == 2) {
      Int_t samplesum = 0;
      for (size_t i = 0; i < fSampleMask.size(); i++) {
        samplesum += samples[fSampleMask[i]];
      }
      if (shift > -1) charge = fElookupSmall[shift][samplesum];
      else
        charge = fElookupSmall[fElookupAsym[samples[fMaxBin]][samples[fMinBin]][samples[fHighBin]]][samplesum];
    }
    else {
      return 0.;
    }
  }
  else {
    if (fLookUp == 3) {
      if (shift > -1) charge = fElookupSmall[shift][samples[fMaxBin]];
      else
        charge = fElookupSmall[fElookupA[samples[fMinBin]][samples[fMaxBin]]][samples[fMaxBin]];
    }
    if (fLookUp == 4) {
      if (shift > -1) charge = fElookupSmall[shift][samples[fMaxBin]];
      else
        charge = fElookupSmall[fElookupA[samples[fMinBin]][samples[fMaxBin]]][samples[fMaxBin]];
    }
  }

  return charge;
}


//_______________________________________________________________________________
Double_t CbmTrdRawToDigiR::CalcResponse(Double_t t)
{

  if (fShapingOrder == 1) return (t / fTau) * TMath::Exp(-(t / fTau));
  if (fShapingOrder == 2) return (t / fTau) * (t / fTau) * TMath::Exp(-(t / fTau));

  return 0.;
}
