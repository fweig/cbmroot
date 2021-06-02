/** @file CbmMcbm2020TrdTshiftPar.cxx
  * @copyright Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pascal Raisig, Florian Uhlig [committer] **/

#include "CbmMcbm2020TrdTshiftPar.h"

// FairRoot
#include "FairLogger.h"
#include "FairParamList.h"

// ROOT
#include "THnSparse.h"

// C/C++
#include <iomanip>
#include <iostream>

// #define CTAVM CbmTrdAnalysisVarManager // See comments in GetNevents() et al.
// #define CTAH CbmTrdAnalysisHisto // See comments in GetNevents() et al.

CbmMcbm2020TrdTshiftPar::CbmMcbm2020TrdTshiftPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
  , fNtimeslices(1)
{
  detName = "TRD";
}

CbmMcbm2020TrdTshiftPar::~CbmMcbm2020TrdTshiftPar() { clear(); }

// ----- Public method clear ----
void CbmMcbm2020TrdTshiftPar::clear()

{
  status = kFALSE;
  resetInputVersions();
}

// ----- Public method printparams ----
void CbmMcbm2020TrdTshiftPar::printparams()

{
  std::cout << "CbmMcbm2020TrdTshiftPar::printparams() " << &fTimeshifts << std::endl;
  size_t ntimeshifts = (fTimeshifts.GetSize() / (fgNchannels + 1));
  std::cout << "ParSet has " << ntimeshifts << " timeshift changes stored" << std::endl;

  for (size_t ishiftblock = 0; ishiftblock < ntimeshifts; ishiftblock++) {
    std::cout << "Shiftblock of event " << fTimeshifts[ishiftblock * (fgNchannels + 1)] << std::endl;
    for (size_t ichannel = 0; ichannel < fgNchannels; ichannel++) {
      if (ichannel % 6 == 0) std::cout << std::endl;
      std::cout << " Ch " << ichannel << " shift " << fTimeshifts[(ishiftblock * (fgNchannels + 1)) + ichannel];
    }
  }
}

// ---- putParams ----
void CbmMcbm2020TrdTshiftPar::putParams(FairParamList* l)
{
  if (!l) return;
  l->add(pararraynames[0].data(), fNtimeslices);
  l->add(pararraynames[1].data(), fTimeshifts);
}

// ---- getParams ----
Bool_t CbmMcbm2020TrdTshiftPar::getParams(FairParamList* l)
{
  if (!l) return kFALSE;

  if (!l->fill(pararraynames[0].data(), &fNtimeslices)) return kFALSE;
  if (!l->fill(pararraynames[1].data(), &fTimeshifts)) return kFALSE;

  // Create a map from the list imported from the par file
  size_t nthShift = 0;

  size_t nentries   = fTimeshifts.GetSize();
  size_t ichannel   = 0;
  size_t itimeslice = 0;
  for (size_t ientry = 0; ientry < nentries; ientry++) {
    // We have a chain of one eventId value + fgNchannels shift values
    nthShift = ientry / (fgNchannels + 1);

    // Look for the first value in the chain, which is the eventId
    if ((ientry - nthShift * (fgNchannels + 1)) == 0) {
      // Before starting the extraction of the next chain, emplace the previous chain to the map
      if (ientry != 0) {
        auto tspair = std::pair<size_t, std::vector<Int_t>>(itimeslice, fvecCurrentTimeshifts);
        fmapTimeshifts.emplace(tspair);
      }
      itimeslice = fTimeshifts[ientry];
      fvecCurrentTimeshifts.clear();
      fvecCurrentTimeshifts.resize(fgNchannels);
    }
    else {
      ichannel                        = ientry - 1 - nthShift * (fgNchannels + 1);
      fvecCurrentTimeshifts[ichannel] = fTimeshifts[ientry];
    }
  }
  // Now we have to fill the blank spots in the map, since, we do not now if the timeslices are accessed in a completely ordered manor
  for (itimeslice = 0; itimeslice < static_cast<size_t>(std::abs(fNtimeslices[0])); itimeslice++) {
    auto itspair = fmapTimeshifts.find(itimeslice);

    if (itspair != fmapTimeshifts.end()) { continue; }
    else {
      // Get previous timeshift vector to add it also to the current pair
      itspair--;

      auto newtspair = std::pair<size_t, std::vector<Int_t>>(itimeslice, itspair->second);
      fmapTimeshifts.emplace(newtspair);
    }
  }
  return kTRUE;
}

// ---- GetTimeshifts ----
bool CbmMcbm2020TrdTshiftPar::GetTimeshifts(std::shared_ptr<TH2> timeshiftHisto)
{

  ///< Extract the timeshift values from a histo containing the information and write them to fTimeshifts.

  fNtimeslices[0] = timeshiftHisto->GetNbinsX();
  size_t nshifts  = 1;
  fvecCurrentTimeshifts.clear();
  fvecCurrentTimeshifts.resize(0, 0);
  fvecCurrentTimeshifts.resize(fgNchannels, 0);
  Int_t tshift   = 0;
  size_t tsidx   = 0;
  size_t ientry  = 0;
  size_t eventId = 0;

  // We write at least one shift value to fTimeshifts. Thus, we resize it to one valueset
  size_t nentries = nshifts * (fgNchannels + 1);
  fTimeshifts.Set(nentries);

  bool didChange = true;
  for (size_t ievent = 1; ievent < static_cast<size_t>(std::abs(fNtimeslices[0])); ievent++) {
    tsidx = timeshiftHisto->GetXaxis()->GetBinLowEdge(ievent);
    for (size_t ichannel = 0; ichannel < fgNchannels; ichannel++) {
      tshift = (Int_t) timeshiftHisto->GetBinContent(ievent, ichannel);
      if (tshift != fvecCurrentTimeshifts[ichannel]) {
        didChange                       = true;
        fvecCurrentTimeshifts[ichannel] = tshift;
      }
    }
    if (didChange) {
      // First resize the fTimeshifts array
      nshifts++;
      nentries = nshifts * (fgNchannels + 1);
      fTimeshifts.Set(nentries);

      // Then write the eventId correlated to the tsIdx to the array
      // For mCbm2020 a timeslice contains 10 µslices, the tsidx jumps accordingly by 10, e.g. tsidx0 = 7 -> tsidx = 17
      eventId             = tsidx / 10;
      fTimeshifts[ientry] = eventId;
      ientry++;
      // Followed by the channelwise shift values
      for (size_t ichannel = 0; ichannel < fgNchannels; ichannel++) {
        tshift = fvecCurrentTimeshifts[ichannel];
        // The above represents the timeshift in multiples of a µSlice length (for mCbm 2020 1280 ns) since here we have time to do the calculation of the actual timeshift, we do it here and not in the unpacker algo.
        tshift *= fgMicroSliceLength;
        fTimeshifts.AddAt(tshift, ientry);
        ientry++;
      }
    }
    // reset didChange value
    didChange = false;
  }
  return fTimeshifts.GetSize() > 0;
}

// ---- GetTimeshiftsVec ----
std::vector<Int_t> CbmMcbm2020TrdTshiftPar::GetTimeshiftsVec(size_t tsidx)
{
  auto pair = fmapTimeshifts.find(tsidx);
  return pair->second;
}

// ---- GetNEvents ----
double CbmMcbm2020TrdTshiftPar::GetNEvents(std::shared_ptr<TFile> mcbmanafile)
{
  ///< Extract the number of events from a mcbmana task output file, which has to contain the required histogram.

  TH1* histo = nullptr;

  // We have to do some hardcoding here, since the TrdAnalysisFramework is not yet in the common master. The handling with TAF in place will be given commented out

  //First get the required histogram from the mcbmana file
  // std::vector<CTAVM::eVars> varvec = {CTAVM::eVars::kRunId, CTAVM::eVars::kEnd, CTAVM::eVars::kEnd};
  // auto weight                      = CTAVM::eVars::kNEvents;
  // auto fillstation = CTAH::eFillStation::kRunInfo;
  // auto htype       = (int) CTAH::eHistoType::kGlobal;
  // histo = CTAH::GetHistoFromFile(varvec, fillstation, htype, mcbmanafile, weight);

  std::string hpath = "FillStation-RunInfo/FullTrd/RunId_wNEvents-RunInfo";
  histo             = (TH1*) mcbmanafile->Get(hpath.data());
  if (!histo) {
    LOG(fatal) << " CbmMcbm2020TrdTshiftPar::GetNEvents " << hpath.data() << " not found in the file" << std::endl;
  }
  double nevents = histo->GetBinContent(histo->GetMaximumBin());
  return nevents;
}

// ---- GetCalibHisto ----
std::shared_ptr<TH3> CbmMcbm2020TrdTshiftPar::GetCalibHisto(std::shared_ptr<TFile> mcbmanafile)
{
  ///< Extract the required base histogram from a mcbmana task output file.
  THnSparse* hsparse = nullptr;

  // std::vector<CTAVM::eVars> varvec = {CTAVM::eVars::kTsSourceTsIndex, CTAVM::eVars::kDigiTrdChannel, CTAVM::eVars::kDigiDtCorrSlice};
  // auto fillstation = CTAH::eFillStation::kTrdT0Digi;
  // auto htype       = (int) CTAH::eHistoType::kGlobal;
  // histo =
  //   (THnSparseD*) CTAH::GetHistoFromFile(varvec, fillstation, htype, mcbmanafile);

  std::string hpath = "FillStation-TrdT0Digi/FullTrd/"
                      "TsSourceTsIndex_DigiTrdChannel_DigiDtCorrSlice-TrdT0Digi";
  hsparse = (THnSparse*) mcbmanafile->Get(hpath.data());

  if (!hsparse) {
    LOG(fatal) << " CbmMcbm2020TrdTshiftPar::GetCalibHisto " << hpath.data() << " not found in the file" << std::endl;
  }

  auto nevents = GetNEvents(mcbmanafile);

  // auto tsaxis = CTAH::GetVarAxis(hsparse, CTAVM::eVars::kTsSourceTsIndex);
  auto tsaxis = hsparse->GetAxis(0);  // For now we know that the TsIndex is on the X-Axis
  tsaxis->SetRangeUser(0.0, (double) nevents);
  auto temphisto = hsparse->Projection(0, 1, 2);
  auto histo     = std::make_shared<TH3D>(*temphisto);
  delete temphisto;
  histo->SetName("calibbasehisto");
  histo->SetTitle("calibbasehisto");
  // We need the uniqueIDs for the correct handling within TAF
  histo->GetXaxis()->SetUniqueID(hsparse->GetAxis(0)->GetUniqueID());
  histo->GetYaxis()->SetUniqueID(hsparse->GetAxis(1)->GetUniqueID());
  histo->GetZaxis()->SetUniqueID(hsparse->GetAxis(2)->GetUniqueID());

  return histo;
}

// ---- GetCalibHisto ----
std::shared_ptr<TH2> CbmMcbm2020TrdTshiftPar::GetCalibHisto(std::shared_ptr<TH3> calibbasehisto)
{
  ///< Extract the timeshiftHisto from the calibbase histogram. The calibbase histogram is a TH3* with the tsIdx, the module channels and the timeshifts on the axes.

  // Get the x-axis definitions
  size_t nevents    = calibbasehisto->GetNbinsX();
  size_t firstTsIdx = calibbasehisto->GetXaxis()->GetBinLowEdge(calibbasehisto->GetXaxis()->GetFirst());
  size_t lastTsIdx  = calibbasehisto->GetXaxis()->GetBinUpEdge(calibbasehisto->GetXaxis()->GetLast());

  // Get the y-axis definitions
  size_t nchannels    = calibbasehisto->GetNbinsY();
  size_t firstChannel = calibbasehisto->GetYaxis()->GetBinLowEdge(calibbasehisto->GetYaxis()->GetFirst());
  size_t lastChannel  = calibbasehisto->GetYaxis()->GetBinUpEdge(calibbasehisto->GetYaxis()->GetLast());

  std::shared_ptr<TH2I> calibhisto = std::make_shared<TH2I>("calibhisto", "calibhisto", nevents, firstTsIdx, lastTsIdx,
                                                            nchannels, firstChannel, lastChannel);


  for (size_t itsidx = 1; itsidx < nevents; itsidx++) {
    for (size_t ichannel = 1; ichannel < nchannels; ichannel++) {
      auto dominantshift = GetDominantShift(calibbasehisto, itsidx, ichannel) < 255
                             ? GetDominantShift(calibbasehisto, itsidx, ichannel)
                             : calibhisto->GetBinContent(itsidx - 1, ichannel);
      if (itsidx - 1 == 0) dominantshift = 0;
      calibhisto->SetBinContent(itsidx, ichannel, dominantshift);
    }
  }
  return calibhisto;
}

// ---- GetDominantShift ----
Int_t CbmMcbm2020TrdTshiftPar::GetDominantShift(std::shared_ptr<TH3> calibbasehisto, size_t itsidx, size_t ichannel)
{
  auto hdomshift = calibbasehisto->ProjectionZ("domshift", itsidx, itsidx, ichannel, ichannel);

  // Scale histo to one
  hdomshift->Scale(1. / hdomshift->Integral());

  auto domshift  = hdomshift->GetBinCenter(hdomshift->GetMaximumBin());
  auto dominance = hdomshift->GetMaximum();

  delete hdomshift;

  // Only add dominant slices with at least 50% of the entries in the dominant slice

  if (dominance > 0.5)
    // if (dominance > 0.25)
    return domshift;
  else
    return 255;
}

// ---- FillTimeshiftArray ----
bool CbmMcbm2020TrdTshiftPar::FillTimeshiftArray(std::shared_ptr<TFile> mcbmanafile)
{
  ///< Extract the timeshift values from a TAF output file that contains the required histograms and write them to fTimeshifts.

  auto calibbasehisto = GetCalibHisto(mcbmanafile);
  auto calibhisto     = GetCalibHisto(calibbasehisto);
  return GetTimeshifts(calibhisto);
}

ClassImp(CbmMcbm2020TrdTshiftPar)
