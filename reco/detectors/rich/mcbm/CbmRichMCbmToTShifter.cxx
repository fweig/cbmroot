#include "CbmRichMCbmToTShifter.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TH1.h"
#include "TH1D.h"
#include "TLine.h"
#include "TMarker.h"
#include "TMath.h"
#include "TStyle.h"
#include <TFile.h>


#include "CbmDrawHist.h"
#include "CbmRichConverter.h"
#include "CbmRichDigi.h"
#include "CbmRichGeoManager.h"
#include "CbmRichPoint.h"
#include "TLatex.h"

#include "CbmDigiManager.h"
#include "CbmHistManager.h"
#include "CbmUtils.h"

#include <boost/assign/list_of.hpp>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

using boost::assign::list_of;

CbmRichMCbmToTShifter::CbmRichMCbmToTShifter()
  : FairTask("CbmRichMCbmToTShifter")
  , fEventNum(0)
  , fOutputDir("result_ToTOffset")
  , fhTotMap()
  , fGeneratePDFs(false)
  , fShowTdcId(false) {
  std::cout << "CbmRichMCbmToTShifter::Constructor.." << std::endl;
}

InitStatus CbmRichMCbmToTShifter::Init() {
  std::cout << "CbmRichMCbmToTShifter::Init" << std::endl;

  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) {
    Fatal("CbmRichMCbmToTShifter::Init", "RootManager not instantised!");
  }

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) {
    Fatal("CbmRichMCbmToTShifter::Init", "No Rich Digis!");
  }

  return kSUCCESS;
}


void CbmRichMCbmToTShifter::Exec(Option_t* /*option*/) {
  fEventNum++;
  int nofDigis = fDigiMan->GetNofDigis(ECbmModuleId::kRich);

  for (int i = 0; i < nofDigis; ++i) {
    const CbmRichDigi* digi = fDigiMan->Get<CbmRichDigi>(i);
    TH1* h                  = GetTotH1(getDirichAddress(digi->GetAddress()),
                      getDirichChannel(digi->GetAddress()));
    if (h != nullptr) h->Fill(digi->GetToT());
  }
}


void CbmRichMCbmToTShifter::Finish() {

  std::cout << "Drawing Hists..." << std::endl;
  std::stringstream s;
  Int_t mean_cnt = 0;
  Double_t mean  = 0.;
  for (auto const& outer : fhTotMap) {
    for (auto const& inner : outer.second) {
      mean += static_cast<Double_t>(inner.second->GetEntries());
      mean_cnt++;
    }
  }

  if (mean_cnt != 0) mean /= mean_cnt;

  if (fShowTdcId)
    s << "TDC 0x" << std::hex << 0xc000 << "  " << std::dec << " !";
  s << printEmpty() << " \\" << std::endl;
  if (fShowTdcId)
    s << "TDC 0x" << std::hex << 0xc001 << "  " << std::dec << " !";
  s << printEmpty() << " \\" << std::endl;

  auto it      = std::begin(fhTotMap);
  uint32_t cnt = 0;
  for (auto const& outer : fhTotMap) {
    int tdc    = outer.first;
    TCanvas* c = new TCanvas(
      Form("fhToT_%x", outer.first), Form("fhToT_%x", outer.first), 2000, 2000);
    c->Divide(6, 6);
    while (
      calcDirichAddr(cnt) < static_cast<uint16_t>(
        tdc)) {  // this dirich is not in use; fill up the parameter file with printEmpty
      if (fShowTdcId)
        s << "TDC 0x" << std::hex << calcDirichAddr(cnt) << "  " << std::dec
          << " !";
      s << printEmpty();
      if (std::next(it) != fhTotMap.end()) s << " \\" << std::endl;
      cnt++;
    }
    if (fShowTdcId)
      s << "TDC 0x" << std::hex << outer.first << "  " << std::dec << " !";
    s << "  0.00";
    for (int i = 0; i < 32; ++i) {
      c->cd(1 + i);
      TH1* h = GetTotH1(tdc, i + 1);
      if (h != nullptr) {
        h->Draw();
        if (GetMaxH1(h) < 20 || h->GetEntries() < mean * 0.1) {
          s << "  0.00";
        } else {
          s << "  " << GetMaxH1(h) - 25.00;
        }
      }
    }

    cnt++;

    if (it == fhTotMap.begin()) {
      if (fGeneratePDFs) c->Print("ToTs/Tdc_all.pdf(", "pdf");
      s << " \\" << std::endl;

    } else if (std::next(it) == fhTotMap.end()) {

      if (fGeneratePDFs) c->Print("ToTs/Tdc_all.pdf)", "pdf");
      if (cnt == 71) s << std::endl;
    } else {
      if (fGeneratePDFs) c->Print("ToTs/Tdc_all.pdf", "pdf");
      s << " \\" << std::endl;
    }

    ++it;
  }

  //fill up till end of SetupSize

  for (uint16_t i = cnt; i < 72; ++i) {
    s << " \\" << std::endl;  // from last output

    if (fShowTdcId)
      s << "TDC 0x" << std::hex << calcDirichAddr(i) << "  " << std::dec
        << " !";
    s << printEmpty();
  }

  std::cout << s.str() << std::endl;
}


TH1* CbmRichMCbmToTShifter::GetTotH1(Int_t tdc, Int_t channel) {
  TH1* h = fhTotMap[tdc][channel];
  if (h == nullptr) {
    TString name, title, subFolder;
    name.Form("ToT_tdc_0x%x_ch%u", tdc, channel);
    title.Form("%s;ToT [ns];Entries", name.Data());
    h                      = new TH1D(name, title, 500, -1., 49.);
    fhTotMap[tdc][channel] = h;
  }
  return h;
}

Double_t CbmRichMCbmToTShifter::GetMaxH1(TH1* h) {
  Double_t max = 0.0;
  Int_t b      = 0;
  for (Int_t i = 1; i < h->GetNbinsX(); ++i) {
    Double_t val = h->GetBinContent(i);
    if (val > max) {
      max = val;
      b   = i;
    }
  }
  return h->GetBinCenter(b);
}


std::string CbmRichMCbmToTShifter::printEmpty() {
  std::string s = "";
  for (uint16_t i = 0; i < 33; ++i) {
    s += "  0.00";
  }
  return s;
}

ClassImp(CbmRichMCbmToTShifter)
