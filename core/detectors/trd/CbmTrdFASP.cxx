#include "CbmTrdFASP.h"

#include "CbmMatch.h"    // for CbmMatch
#include "CbmTrdDigi.h"  // for CbmTrdDigi, CbmTrdDigi::kFASP

#include <Logger.h>  // for Logger, LOG

#include <TAxis.h>        // for TAxis
#include <TCanvas.h>      // for TCanvas
#include <TGraph.h>       // for TGraph
#include <TH1.h>          // for TH1, TH1F
#include <TLine.h>        // for TLine
#include <TMath.h>        // for Ceil, Floor
#include <TMathBase.h>    // for Min
#include <TString.h>      // for Form, TString
#include <TVirtualPad.h>  // for TVirtualPad

#include <iostream>  // for operator<<, basic_ostream, cout, ostream
#include <utility>   // for pair, make_pair

#include <stdio.h>   // for printf, size_t
#include <string.h>  // for memcpy, memset, strcmp

#define VERBOSE 0
#define DRAW 0

using namespace std;

Bool_t CbmTrdFASP::fgNeighbour = kTRUE;  // by default enable neighbour trigger
// Float_t CbmTrdFASP::fgShaperThr       = 0.2;  // [V]
// Float_t CbmTrdFASP::fgNeighbourThr    = 0.07; // [V]
Float_t CbmTrdFASP::fgShaperThr       = 0.05;   // [V]
Float_t CbmTrdFASP::fgNeighbourThr    = 0.04;   // [V]
Float_t CbmTrdFASP::fgBaseline        = 0.25;   // [V]
Float_t CbmTrdFASP::fgOutGain         = 2.025;  // [V/ 4095 ADC chs]
const Int_t CbmTrdFASP::fgkNclkFT     = 14;     // [clk]
Int_t CbmTrdFASP::fgNclkLG            = 31;     // [clk]
const Int_t CbmTrdFASP::fgkBufferKeep = 400;    // [5*ns]
//___________________________________________________________________
CbmTrdFASP::CbmTrdFASP(UInt_t uslice)
  : TObject()
  , fStartTime(0)
  , fProcTime(0)
  , fCol(-1)
  , fRow(-1)
  , fAsicId(-1)
  , fNraw(0)
  , fDigi(nullptr)
  , fHitThPrev(uslice)
  , fShaper(uslice)
  , fShaperNext(uslice)
  , fDigiProc()
  , fPar(nullptr)
  , fTimeLG(-1)
  , fTimeFT(-1)
  , fTimeDY(-1)
  , fFT(0)
  , fGraphId(0)
  , fOut()
  , fGraphMap()
  , fGthr(nullptr)
  , fMonitor(nullptr) {
  /** Build the FASP simulator for a microslice of 5*uslice [ns]
   */

  if (uslice < 2 * fgkBufferKeep) {
    LOG(warn) << "CbmTrdFASP::CbmTrdFASP() : Buffer should be at least "
              << 2 * fgkBufferKeep << " [5*ns] long. Expand.";
    uslice = 2 * fgkBufferKeep;
  }
  fNphys[0] = 0;
  fNphys[1] = 0;
  fHitThPrev.assign(uslice, 0);
  fShaper.assign(uslice, 0.);
  fShaperNext.assign(uslice, 0.);
  if (DRAW) fOut.assign(uslice + 2, 0.);
  memset(fGraph, 0, NGRAPH * sizeof(TGraph*));
  memset(fGraphShp, 0, NGRAPH * sizeof(TGraph*));
  memset(fGraphPhys, 0, NGRAPH * sizeof(TGraph*));
  SetProcTime();
}

//___________________________________________________________________
CbmTrdFASP::~CbmTrdFASP() {
  Int_t nalloc(0);
  for (Int_t ig(0); ig < NGRAPH; ig++) {
    if (fGraph[ig]) delete fGraph[ig];
    if (fGraphShp[ig]) delete fGraphShp[ig];
    if (fGraphPhys[ig]) delete fGraphPhys[ig];
    nalloc++;
  }
  if (VERBOSE && DRAW) printf("CbmTrdFASP::~CbmTrdFASP() : allocated graphs.");
  if (fMonitor) delete fMonitor;
}

//___________________________________________________________________
void CbmTrdFASP::Clear(Option_t* opt) {
  //  printf("CbmTrdFASP::Clear : Nphys[0] = %d\n", fNphys[0]);

  if (fNphys[0])
    ProcessShaper(
      'R');  // process last rect channel without interference from current tilt
  fHitThPrev.assign(fHitThPrev.size(), 0);  //*sizeof(Bool_t));
  WriteDigi();                              // finalize fDigi list
  if (strcmp(opt, "draw") == 0) Draw();
}

//___________________________________________________________________
void CbmTrdFASP::Draw(Option_t* /*opt*/) {
  //  printf("CbmTrdFASP::Draw : row[%2d] col[%2d] asic[%2d]\n", fRow, fCol, fAsicId);
  if (!DRAW) return;
  if (!fGraphMap.size()) return;

  Int_t nasic(0), last(-1), nlast(0);
  vector<pair<Int_t, Int_t>>::iterator ig = fGraphMap.begin();
  while (ig != fGraphMap.end()) {
    if (last < 0 || ig->first != last) {
      nasic++;
      last  = ig->first;
      nlast = 0;
    }
    nlast++;
    ig++;
  }
  if (nasic <= 1) return;

  TH1* h(nullptr);
  TString st, sch;
  Int_t ch, jg;
  TVirtualPad* c1(nullptr);
  if (!fMonitor) {
    fMonitor = new TCanvas("c", "FASP Simulator", 10, 10, 1500, 2000);
    fMonitor->Divide(2, 8, 1.e-5, 1.e-5);
    for (Int_t ic(0); ic < 16; ic++) {
      c1 = fMonitor->cd(ic + 1);
      c1->SetLeftMargin(0.03580097);
      c1->SetRightMargin(0.006067961);
      c1->SetTopMargin(0.01476793);
    }
    //     fMonitor = new TCanvas("c", "FASP Simulator",10,10,500,1000);
    //     fMonitor->Divide(1,2,1.e-5, 1.e-5);
    //     for(Int_t ic(0); ic<2; ic++){
    //       c1 = fMonitor->cd(ic+1);
    //       c1->SetLeftMargin(0.03580097);
    //       c1->SetRightMargin(0.006067961);
    //       c1->SetTopMargin(0.01476793);
    //     }
    fGthr = new TLine();
    fGthr->SetLineStyle(2);
  }

  ig   = fGraphMap.begin();
  last = ig->first;
  for (Int_t iasic(0); iasic < nasic - 1; iasic++) {
    while (ig != fGraphMap.end()) {
      if (ig->first != last) {  // save graphs
        fMonitor->Modified();
        fMonitor->Update();
        fMonitor->SaveAs(Form("FASP_analog_ASIC%03d.gif", last));
        for (Int_t ic(0); ic < /*2*/ 16; ic++)
          fMonitor->cd(ic + 1)->Clear();
        last = ig->first;
        break;
      }
      jg  = ig->second;
      st  = fGraph[jg]->GetTitle();
      sch = st(st.Sizeof() - 3, 2);
      ch  = sch.Atoi();
      if (ch >= 8)
        c1 = fMonitor->cd(2 + 2 * (ch - 8));  //->SetLogy();
      else
        c1 = fMonitor->cd(1 + 2 * ch);
      fGraph[jg]->Draw("al");
      fGraphShp[jg]->Draw("l");
      fGraphPhys[jg]->Draw("lp");
      fGthr->SetLineColor(kGreen);
      fGthr->DrawLine(0, fgShaperThr + 0.2, 5 * fOut.size(), fgShaperThr + 0.2);
      fGthr->SetLineColor(kRed);
      fGthr->DrawLine(
        0, fgNeighbourThr + 0.2, 5 * fOut.size(), fgNeighbourThr + 0.2);
      h = fGraph[jg]->GetHistogram();
      h->GetYaxis()->SetRangeUser(0.01, 5.3);

      ig++;
    }
  }

  //swap draw buffer
  TGraph* buffer[NGRAPH] = {nullptr};
  memcpy(buffer, fGraph, (jg + 1) * sizeof(TGraph*));
  memcpy(fGraph, &fGraph[jg + 1], nlast * sizeof(TGraph*));
  memcpy(&fGraph[nlast], buffer, (jg + 1) * sizeof(TGraph*));
  memcpy(buffer, fGraphShp, (jg + 1) * sizeof(TGraph*));
  memcpy(fGraphShp, &fGraphShp[jg + 1], nlast * sizeof(TGraph*));
  memcpy(&fGraphShp[nlast], buffer, (jg + 1) * sizeof(TGraph*));
  memcpy(buffer, fGraphPhys, (jg + 1) * sizeof(TGraph*));
  memcpy(fGraphPhys, &fGraphPhys[jg + 1], nlast * sizeof(TGraph*));
  memcpy(&fGraphPhys[nlast], buffer, (jg + 1) * sizeof(TGraph*));
  fGraphId -= jg + 1;
  fGraphMap.clear();
  for (Int_t ich(0); ich < nlast; ich++)
    fGraphMap.push_back(make_pair(last, ich));
}

//___________________________________________________________________
Bool_t CbmTrdFASP::Go(ULong64_t time) {
  /** Start FASP simulator if at least the fProcTime has elapsed from buffer start time.
   * time - event time in ns
   */
  if (fStartTime == 0) {
    fStartTime = time;
    return kFALSE;
  }

  if (time < fStartTime) {
    LOG(warn) << "FASP simulator start time " << fStartTime
              << "[ns] is larger than event time " << time << "[ns]. Skip";
    return kFALSE;
  }

  // check to see if there are at most 2us left
  if (time - fStartTime < fProcTime) return kFALSE;

  return kTRUE;  // go
}

//___________________________________________________________________
void CbmTrdFASP::GetShaperSignal(Double_t charge) {
  /** Retrive linear interpolation of CADENCE simulations around the input charge and store them in the 
   * fSignal array for further processing
   */

  //  printf("CbmTrdFASP::GetShaperSignal(%5.1f)\n", charge);
  Int_t idx0(-1), idx1(0);
  for (Int_t is0(1); is0 < fgkNDB; is0++) {
    if (charge > fgkCharge[is0]) continue;
    idx0 = is0 - 1;
    break;
  }

  if (idx0 < 0) {  // above calibrated region
    if (VERBOSE)
      printf("refMax[%2d]=%5.1f\n", fgkNDB - 1, fgkCharge[fgkNDB - 1]);
    memcpy(fSignal, fgkShaper[fgkNDB - 1], FASP_WINDOW * sizeof(Float_t));
  } else if (idx0 == 0 && charge < fgkCharge[0]) {  // below calibrated region
    if (VERBOSE) printf("refMin[0]={%5.1f}\n", fgkCharge[0]);
    for (Int_t it(0); it < FASP_WINDOW; it++)
      fSignal[it] = charge * fgkShaper[0][it] / fgkCharge[0];
  } else {
    idx1 = idx0 + 1;
    if (VERBOSE)
      printf("ref={%5.1f[%2d] %5.1f[%2d]}\n",
             fgkCharge[idx0],
             idx0,
             fgkCharge[idx1],
             idx1);
    // linear interpolation
    Double_t dq(fgkCharge[idx0] - fgkCharge[idx1]);
    for (Int_t it(0); it < FASP_WINDOW; it++)
      fSignal[it] = (charge * (fgkShaper[idx0][it] - fgkShaper[idx1][it])
                     + fgkShaper[idx1][it] * fgkCharge[idx0]
                     - fgkShaper[idx0][it] * fgkCharge[idx1])
                    / dq;
  }
}

//___________________________________________________________________
void CbmTrdFASP::Init(Int_t /*col*/, CbmTrdParFaspChannel* /*par*/) {}

//___________________________________________________________________
Double_t CbmTrdFASP::MakeOut(Int_t time) {
  if (time <= fTimeFT) return fFT;

  Double_t out(0);
  Int_t i = TMath::Min(Int_t(fShaper.size()), time - 1), j(0);
  while (i >= 0 && j < SHAPER_LUT - 1) {
    j = (time - i) - 1;
    out += 1. * fShaper[i] * fgkShaperLUT[j];
    i--;
  }
  if (fTimeFT < 0 && fTimeDY < 0) return out;

  //decay
  j = time - (fTimeDY - SHAPER_LUT);
  if (j > SHAPER_LUT - 1) return out;
  Double_t decay = fFT * fgkDecayLUT[j];
  if (out > decay) {  // reset
    fTimeDY = -1;
    fFT     = 0;
    decay   = out;
  }
  return decay;
}

//___________________________________________________________________
void CbmTrdFASP::PhysToRaw(
  std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* vdigi,
  Int_t col,
  Int_t row) {
  /** Public interface for converting physical digi to raw digi. See ScanDigi and ScanDigiNE for 
   * the actual algorithms  
   */
  if (fgNeighbour)
    ScanDigiNE(vdigi, col, row);
  else
    ScanDigi(vdigi, col, row);
}

//___________________________________________________________________
void CbmTrdFASP::Print(Option_t* opt) const {
  /** Dump settings for the FASP simulator and optionally the content of the current buffers.
   */

  printf("FASP Simulator : Col[%2d] NeighbourTrigger[%c]\n",
         fCol,
         fgNeighbour ? 'y' : 'n');
  printf("  Main CH      : Trigger[V]=%4.2f Flat-Top[ns]=%5.1f\n",
         fgShaperThr,
         fgkNclkFT * CbmTrdDigi::Clk(CbmTrdDigi::kFASP));
  printf("  Neighbour CH : Trigger[V]=%4.2f Linear-gate[ns]=%5.1f\n",
         fgNeighbourThr,
         fgNclkLG * CbmTrdDigi::Clk(CbmTrdDigi::kFASP));

  if (strcmp(opt, "all") != 0) return;
  Int_t time                         = 0;
  vector<Float_t>::const_iterator it = fShaper.cbegin(),
                                  jt = fShaperNext.cbegin();
  while (it != fShaper.cend()) {
    printf("time[ns]=%4d sgn[mV] : sc(%6.1f) sn(%6.1f)\n",
           time,
           1.e3 * (*it),
           1.e3 * (*jt));

    it++;
    jt++;
    time += 5;
  }
}

//___________________________________________________________________
Int_t CbmTrdFASP::ProcessShaper(Char_t typ) {
  /**  Simulate the logic block of the FASP ASIC and apply shapper forming time (see MakeOut()). 
   * Calculate the following logic signals:
   * 1. [ht] hit-threshold for current and next channels. The hit-threshold for the previous channel is already saved in the fHitThPrev.   
   * 2. [lg_cmd] linear-gate command with a minimum configurable length (see SetLGminLength()). It also takes into account the usage of neighbour trigger (see SetNeighbourTrigger()). 
   * 3. [pk_cmd] peak command
   * 4. reset logical signals
   * 
   * Optionally the analog input and output for the current channel are also saved if DRAW is defined.
   */

  if (VERBOSE)
    printf("    CbmTrdFASP::ProcessShaper(%c) : row[%2d] col[%2d] asic[%4d] "
           "PhysDigi[%d] GraphPtr[%d] ...\n",
           typ,
           fRow,
           fCol,
           fAsicId,
           fNphys[0],
           (DRAW ? fGraphId : -1));
  fTimeFT = -1;
  fTimeLG = -1;
  fFT     = 0;
  // digital signals
  Bool_t ht(0), htf(0),  // hit_threshold level/front for current FASP channel
    ht_next(0), htf_next(0),  // hit_threshold level/front for next FASP channel
    htf_prev(0),              // hit_threshold front for previous FASP channel
    pk_cmd(0),                // peak_command level
    lg_cmd(0),                // linear-gate_command level
    trigger(0);               // trigger type [1] = self [0]=neighbour
  UInt_t n(0),                // no of raw digi found in current shaper
    htime(0);                 // hit time on current channel
  Double_t out, max(-1), old(-1);
  for (size_t i = 1; i < fShaper.size() - 1; i++) {
    // compute hit threshold level/front for current and neighbour channels
    htf_prev = 0;
    htf      = 0;
    htf_next = 0;
    if (/*fShaper[i-1]<fgNeighbourThr && */ fShaper[i] >= fgNeighbourThr)
      htime = i * 5;
    if (fShaper[i - 1] < fgShaperThr && fShaper[i] >= fgShaperThr) {
      ht      = 1;
      htf     = 1;
      trigger = 1;
      if (VERBOSE) printf("      %4lu : HT 1\n", i * 5);
    } else if (fShaper[i - 1] >= fgShaperThr && fShaper[i] < fgShaperThr) {
      ht = 0;
      if (VERBOSE) printf("      %4lu : HT 0\n", i * 5);
    }
    if (fgNeighbour) {  // compute neighbour hit threshold if NE selected
      if (fShaperNext[i - 1] < fgShaperThr && fShaperNext[i] >= fgShaperThr) {
        ht_next  = 1;
        htf_next = 1;
        if (VERBOSE) printf("      %4lu : HT_NEXT 1\n", i * 5);
      } else if (fShaperNext[i - 1] >= fgShaperThr
                 && fShaperNext[i] < fgShaperThr) {
        ht_next = 0;
        if (VERBOSE) printf("      %4lu : HT_NEXT 0\n", i * 5);
      }
      if (!fHitThPrev[i] && fHitThPrev[i + 1]) {
        htf_prev = 1;
        if (VERBOSE) printf("      %4lu : HT_PREV 1\n", i * 5);
      }
      if (VERBOSE && fHitThPrev[i] && !fHitThPrev[i + 1])
        printf("      %4lu : HT_PREV 0\n", i * 5);
    }

    // compute linear gate
    if (fTimeLG < 0) {  // check if linear-gate is closed
      if ((fgNeighbour && (htf_prev || htf || htf_next))
          || (!fgNeighbour && htf)) {
        lg_cmd  = 1;
        fTimeLG = i + 0.2 * fgNclkLG * CbmTrdDigi::Clk(CbmTrdDigi::kFASP);
        if (VERBOSE)
          printf(
            "      %4lu : LG_CMD 1 -> MinGateEnd[ns]=%d\n", i * 5, fTimeLG * 5);
      }
    } else if (static_cast<size_t>(fTimeLG) < i && !ht && !pk_cmd
               && (fgNeighbour && (!fHitThPrev[i] && !ht_next))) {
      lg_cmd  = 0;
      trigger = 0;
      fTimeLG = -1;
      if (VERBOSE) printf("      %4lu : LG_CMD 0\n", i * 5);
    }

    if (!pk_cmd && lg_cmd && fShaper[i] >= fgNeighbourThr
        && fShaper[i] >= fShaper[i - 1] && fShaper[i] > fShaper[i + 1]) {
      pk_cmd = 1;
      if (VERBOSE) printf("      %4lu : PK_CMD 1\n", i * 5);
    }

    out = MakeOut(i);

    if (fTimeFT < 0 && lg_cmd && pk_cmd) {
      if (max > 0 && out > max) {
        max = -1;
        if (VERBOSE) printf("      %4lu : RESET MAX\n", i * 5);
      }
      if (max < 0 && out < old) {
        max = out;
        if (VERBOSE) printf("      %4lu : MAX[V]=%5.2f\n", i * 5, max);
      }
      if (out < max - 0.05) {
        fTimeFT = i + 0.2 * fgkNclkFT * CbmTrdDigi::Clk(CbmTrdDigi::kFASP);
        fTimeDY = -1;
        fFT     = max + 0.01;
        // save data for digi update
        fDigiProc.push_back(make_tuple(
          htime,
          UInt_t((i + 3) * 5),
          UInt_t(4095
                 * TMath::Min(Float_t(1.), (fgBaseline + fFT) / fgOutGain)),
          trigger));
        n++;
        if (VERBOSE)
          printf("      %4lu : HT[ns]=%4d FT[V]=%5.2f CS[ns]+=15 |CS[ns]=%d "
                 "Trig[%c]\n",
                 i * 5,
                 htime,
                 fFT,
                 fTimeFT * 5,
                 trigger ? 'S' : 'N');
      }
    }
    if (fTimeFT > 0 && static_cast<size_t>(fTimeFT) <= i) {
      fTimeDY = fTimeFT + SHAPER_LUT;
      fTimeFT = -1;
      fTimeLG = -1;
      pk_cmd  = 0;
      lg_cmd  = 0;
      trigger = 0;
      max     = -1;
      if (VERBOSE)
        printf("      %4lu : LG_CMD 0\n"
               "      %4lu : PK_CMD 0\n",
               i * 5,
               i * 5);
    }
    if (DRAW) fOut[i + 3] = out;
    old = out;
    if (fgNeighbour) fHitThPrev[i] = ht;  // save hit threshold for next channel
  }

  // save results for draw
  if (DRAW) {
    if (fGraphId < NGRAPH - 1) {
      Int_t ch   = 2 * (fCol % 8) + (typ == 'R' ? 1 : 0), ip(0),
            time = fStartTime;
      if (!fGraph[fGraphId]) {
        fGraph[fGraphId] = new TGraph(fOut.size());
        fGraph[fGraphId]->SetName(Form("g%03d", fGraphId));
        fGraph[fGraphId]->SetFillStyle(0);
        fGraph[fGraphId]->SetMarkerStyle(7);
        fGraph[fGraphId]->SetLineColor(kRed);
        fGraph[fGraphId]->SetMarkerColor(kRed);
        fGraphShp[fGraphId] = (TGraph*) fGraph[fGraphId]->Clone();
        fGraphShp[fGraphId]->SetName(Form("gs%03d", fGraphId));
        fGraphShp[fGraphId]->SetLineColor(kBlack);
        fGraphShp[fGraphId]->SetMarkerColor(kBlack);
      }
      fGraph[fGraphId]->SetTitle(Form(
        "Col[%2d] Row[%2d] ASIC[%3d] Channel = %2d", fCol, fRow, fAsicId, ch));
      fGraphMap.push_back(make_pair(fAsicId, fGraphId));
      vector<Float_t>::const_iterator ot = fOut.cbegin(), st = fShaper.cbegin();
      while (ot != fOut.cend()) {
        fGraph[fGraphId]->SetPoint(ip, time, 0.3 + *ot);
        ot++;
        fGraphShp[fGraphId]->SetPoint(ip, time, 0.2 + *st);
        st++;
        time += 5;
        ip++;
      }
      fGraphId += 1;
    } else
      LOG(warn)
        << "CbmTrdFASP::ProcessShaper : Draw buffer exhausted. Expert setting.";
  }
  // move to the next channel
  memcpy(fShaper.data(), fShaperNext.data(), fShaper.size() * sizeof(Float_t));
  memset(fShaperNext.data(), 0, fShaperNext.size() * sizeof(Float_t));
  fNphys[0] = fNphys[1];
  fNphys[1] = 0;

  return n;
}

//___________________________________________________________________
void CbmTrdFASP::ScanDigi(std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* vdigi,
                          Int_t col,
                          Int_t row) {
  /** Transform point like time distribution of digis into the time dependent analog signal of the first FASP shaper
   */

  CbmTrdDigi* digi(nullptr);
  ULong64_t time;
  Int_t dt, gid, asic = row * 9 + col / 8;
  Double_t t, r;
  if (fAsicId < 0) fAsicId = asic;  // init asic identifier
  fCol    = col;
  fRow    = row;
  fAsicId = asic;
  if (DRAW) {
    gid = fGraphId + 1;
    if (fGraphPhys[gid])
      memset(
        fGraphPhys[gid]->GetY(), 0, fGraphPhys[gid]->GetN() * sizeof(Double_t));
    else {
      fGraphPhys[gid] = new TGraph(fOut.size());
      fGraphPhys[gid]->SetName(Form("gp%03d", gid));
      fGraphPhys[gid]->SetMarkerStyle(20);
      fGraphPhys[gid]->SetLineWidth(2);
      for (UInt_t ip(0), tm(fStartTime / 5); ip < fOut.size(); ip++, tm += 5)
        fGraphPhys[gid]->SetPoint(ip, tm, 0.);
    }
  }
  // process digi
  std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>::iterator iv = vdigi->begin();
  vector<Float_t>::iterator itb;
  while (iv != vdigi->end()) {
    digi = iv->first;
    time = (digi->GetTimeDAQ() - fStartTime) / 5;  // get time in 5ns bins
    if (time + FASP_WINDOW > fShaper.size()) {
      LOG(debug) << "CbmTrdFASP::ScanDigi() : Digi @ row[" << row << "] col["
                 << col << "] time[" << digi->GetTimeDAQ()
                 << "] dows not fit in the current buffer starting @ "
                 << fStartTime << "ns. Skip this time.";
      break;
    }

    r = digi->GetCharge(t, dt);
    t /= 10;
    r /= 10;
    if (VERBOSE)
      printf(
        "  time buffer[5ns]=%4llu / phys[ns]=%llu charge[fC]=%5.1f / %5.1f",
        time,
        digi->GetTimeDAQ(),
        t,
        r);
    // tilt pad channel
    if (t > 0) {
      if (DRAW) fGraphPhys[gid]->SetPoint(time, digi->GetTimeDAQ(), t / 100.);
      GetShaperSignal(t);
      itb = fShaper.begin();
      itb += time;
      for (Int_t it(0); it < FASP_WINDOW && itb != fShaper.end(); it++, itb++)
        (*itb) += fSignal[it];
      fNphys[0]++;
    } else if (VERBOSE)
      printf("\n");
    // rect pad channel
    if (r > 0) {
      if (DRAW) fGraphPhys[gid]->SetPoint(time, digi->GetTimeDAQ(), r / 100.);
      GetShaperSignal(r);
      itb = fShaperNext.begin();
      itb += time + dt;
      for (Int_t it(0); it < FASP_WINDOW && itb != fShaperNext.end();
           it++, itb++)
        (*itb) += fSignal[it];
      fNphys[1]++;
    } else if (VERBOSE)
      printf("\n");
    iv++;
  }
  if (fNphys[0]) fNraw = ProcessShaper('T');  // process tilt
  if (fNphys[1]) ProcessShaper('R');          // process rect
  fDigi = vdigi;
  WriteDigi();
}

//___________________________________________________________________
void CbmTrdFASP::ScanDigiNE(
  std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* vdigi,
  Int_t col,
  Int_t row) {
  /** Transform point like time distribution of digis into the time dependent analog signal 
   * of the first FASP shaper and process output (see ProcessShaper())
   */

  CbmTrdDigi* digi(nullptr);
  ULong64_t time;
  Int_t dt, gid, asic = row * 9 + col / 8;
  Double_t t, r;
  std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>::iterator iv;
  vector<Float_t>::iterator itb;

  if (fAsicId < 0) fAsicId = asic;  // init asic identifier

  // printf("CbmTrdFASP::ScanDigiNE : fCol[%2d] col[%2d] graph[%d]\n", fCol, col, fGraphId);
  // No interference from the previous data
  if (fCol < 0 ||                        // first data in the module
      (fCol >= 0 && col != fCol + 1) ||  // column jump
      (fRow >= 0 && row != fRow)) {      // row jump
    Clear((fAsicId != asic ? "draw" : ""));

    if (DRAW) {
      gid = fGraphId;
      if (fGraphPhys[gid])
        memset(fGraphPhys[gid]->GetY(),
               0,
               fGraphPhys[gid]->GetN() * sizeof(Double_t));
      else {
        fGraphPhys[gid] = new TGraph(fOut.size());
        fGraphPhys[gid]->SetName(Form("gp%03d", gid));
        fGraphPhys[gid]->SetMarkerStyle(20);
        fGraphPhys[gid]->SetLineWidth(2);
        for (UInt_t ip(0), tm(fStartTime / 5); ip < fOut.size(); ip++, tm += 5)
          fGraphPhys[gid]->SetPoint(ip, tm, 0.);
      }
    }

    // load data from current tilt channel
    iv = vdigi->begin();
    while (iv != vdigi->end()) {
      digi = iv->first;
      time = (digi->GetTimeDAQ() - fStartTime)
             / 5;  // get time from buffer start in 5ns bins
      if (time + FASP_WINDOW > fShaper.size()) {
        LOG(debug) << "CbmTrdFASP::ScanDigiNE() : T-Digi @ row[" << row
                   << "] col[" << col << "] time[" << digi->GetTimeDAQ()
                   << "] does not fit in the current buffer starting @ "
                   << fStartTime << "ns. Skip this time.";
        break;
      }
      digi->GetCharge(t, dt);
      t /= 10.;
      if (VERBOSE)
        printf("  [T] time buffer[5ns]=%4llu / phys[ns]=%llu charge[fC]=%5.1f ",
               time,
               digi->GetTimeDAQ(),
               t);
      // tilt pad channel
      if (t > 0) {
        if (DRAW) fGraphPhys[gid]->SetPoint(time, digi->GetTimeDAQ(), t / 100.);
        GetShaperSignal(t);
        itb = fShaper.begin();
        itb += time;
        for (Int_t it(0); it < FASP_WINDOW && itb != fShaper.end(); it++, itb++)
          (*itb) += fSignal[it];
        fNphys[0]++;
      } else if (VERBOSE)
        printf("\n");
      iv++;
    }
  } else {
    if (DRAW) {
      gid = fGraphId + 1;
      if (fGraphPhys[gid])
        memset(fGraphPhys[gid]->GetY(),
               0,
               fGraphPhys[gid]->GetN() * sizeof(Double_t));
      else {
        fGraphPhys[gid] = new TGraph(fOut.size());
        fGraphPhys[gid]->SetName(Form("gp%03d", gid));
        fGraphPhys[gid]->SetMarkerStyle(20);
        fGraphPhys[gid]->SetLineWidth(2);
        for (UInt_t ip(0), tm(fStartTime / 5); ip < fOut.size(); ip++, tm += 5)
          fGraphPhys[gid]->SetPoint(ip, tm, 0.);
      }
    }

    // load tilt digi to account for neighbour trigger
    iv = vdigi->begin();
    while (iv != vdigi->end()) {
      digi = iv->first;
      time = (digi->GetTimeDAQ() - fStartTime)
             / 5;  // get time from buffer start in 5ns bins
      if (time + FASP_WINDOW > fShaper.size()) {
        LOG(debug) << "CbmTrdFASP::ScanDigiNE() : T-Digi @ row[" << row
                   << "] col[" << col << "] time[" << digi->GetTimeDAQ()
                   << "] does not fit in the current buffer starting @ "
                   << fStartTime << "ns. Skip this time.";
        break;
      }
      digi->GetCharge(t, dt);
      t /= 10.;

      if (VERBOSE)
        printf("  [T] time buffer[5ns]=%4llu / phys[ns]=%llu charge[fC]=%5.1f ",
               time,
               digi->GetTimeDAQ(),
               t);
      // tilt pad channel
      if (t > 0) {
        if (DRAW) fGraphPhys[gid]->SetPoint(time, digi->GetTimeDAQ(), t / 100.);
        GetShaperSignal(t);
        itb = fShaperNext.begin();
        itb += time;
        for (Int_t it(0); it < FASP_WINDOW && itb != fShaperNext.end();
             it++, itb++)
          (*itb) += fSignal[it];
        fNphys[1]++;
      } else if (VERBOSE)
        printf("\n");
      iv++;
    }
    ProcessShaper('R');  // process previous rect channel
    WriteDigi();         // finalize fDigi list
  }

  fCol    = col;
  fRow    = row;
  fAsicId = asic;

  if (DRAW) {
    gid = fGraphId + 1;
    if (fGraphPhys[gid])
      memset(
        fGraphPhys[gid]->GetY(), 0, fGraphPhys[gid]->GetN() * sizeof(Double_t));
    else {
      fGraphPhys[gid] = new TGraph(fOut.size());
      fGraphPhys[gid]->SetName(Form("gp%03d", gid));
      fGraphPhys[gid]->SetMarkerStyle(20);
      fGraphPhys[gid]->SetLineWidth(2);
      for (UInt_t ip(0), tm(fStartTime / 5); ip < fOut.size(); ip++, tm += 5)
        fGraphPhys[gid]->SetPoint(ip, tm, 0.);
    }
  }
  iv = vdigi->begin();
  while (iv != vdigi->end()) {
    digi = iv->first;
    time = (digi->GetTimeDAQ() - fStartTime)
           / 5;  // get time from buffer start in 5ns bins
    if (time + FASP_WINDOW > fShaper.size()) {
      LOG(debug) << "CbmTrdFASP::ScanDigiNE() : R-Digi @ row[" << row
                 << "] col[" << col << "] time[" << digi->GetTimeDAQ()
                 << "] dows not fit in the current buffer starting @ "
                 << fStartTime << "ns. Skip this time.";
      break;
    }
    r = digi->GetCharge(t, dt);
    r /= 10.;
    if (VERBOSE)
      printf("  [R] time buffer[5ns]=%4llu / phys[ns]=%llu charge[fC]=%5.1f ",
             time,
             digi->GetTimeDAQ(),
             r);

    // rect pad channel
    if (r > 0) {
      if (DRAW) fGraphPhys[gid]->SetPoint(time, digi->GetTimeDAQ(), r / 100.);
      GetShaperSignal(r);
      itb = fShaperNext.begin();
      itb += time + dt;
      for (Int_t it(0); it < FASP_WINDOW && itb != fShaperNext.end();
           it++, itb++)
        (*itb) += fSignal[it];
      fNphys[1]++;
    } else if (VERBOSE)
      printf("\n");
    iv++;
  }
  fNraw = ProcessShaper('T');  // process tilt on current channel
  fDigi = vdigi;               // save link for further processing
}

//___________________________________________________________________
void CbmTrdFASP::SetProcTime(ULong64_t t) {
  if (t == 0)
    fProcTime = 5 * (fShaper.size() - fgkBufferKeep);
  else
    fProcTime = t - fStartTime;
  //printf("  ProcTime[ns] = %d\n", fProcTime);
}

//___________________________________________________________________
void CbmTrdFASP::WriteDigi() {

  if (!fDigi) return;
  if (VERBOSE)
    printf("      CbmTrdFASP::WriteDigi(T[%d], R[%d]) ...\n",
           fNraw + 1,
           Int_t(fDigiProc.size()) - fNraw + 1);

  vector<tuple<UInt_t, UInt_t, UInt_t, Bool_t>>::iterator
    it = fDigiProc.begin(),  // begin [tilt] iterator
    jt = it + fNraw,         // middle [rect] iterator
    lt = jt;                 // middle iterator
  CbmTrdDigi *digi(nullptr), *digi1(nullptr);
  CbmMatch* dmatch(nullptr);
  ULong64_t time, dtime;
  Int_t dt, ddt, trigger(0);
  Double_t t, r, t0, r0;
  UInt_t hTime,  // hit time [ns]
    csTime,      // CS time  [ns]
    tADC,        // tilt channel signal [ADC]
    rADC;        // rect channel signal [ADC]
  //Bool_t mask(0), pileup(0);
  std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>::iterator jv,
    iv = fDigi->begin();

  while (iv != fDigi->end()) {
    digi   = iv->first;
    dmatch = iv->second;
    time =
      (digi->GetTimeDAQ() - fStartTime);  // digi time in [ns] from buffer start
    // stop digi finalize
    if (time > fProcTime) break;
    r = digi->GetCharge(t, dt);
    r /= 10.;
    t /= 10.;
    if (VERBOSE)
      cout << "        IN : " << digi->ToString() << "         "
           << dmatch->ToString();

    rADC    = 0;
    tADC    = 0;
    dtime   = 0;
    ddt     = 0; /*mask=0; pileup=0;*/
    trigger = 0;
    hTime   = 0;
    while (it != lt) {
      hTime = get<0>(*it);
      if (VERBOSE)
        printf(
          "        [T] htime[ns]=%d FT[ADC]=%4d ... \n", hTime, get<2>(*it));
      if (hTime > time) break;
      it++;
    }
    if (t > 0. && it != lt) {
      csTime = get<1>(*it);
      //printf("match T htime[%d] time[%d]\n", hTime, time);
      if (hTime - time < 400) {  // found converted hit
        if (VERBOSE)
          printf("        [T] ht[ns]=%4d CS[ns]=%4d FT[ADC]=%4u Trg[%s]\n",
                 hTime,
                 csTime,
                 get<2>(*it),
                 (get<3>(*it) ? "S" : "N"));
        dtime = csTime;
        tADC  = get<2>(*it);
        if (get<3>(*it)) trigger |= 1;
        //if(csTime-hTime > 350) pileup = kTRUE; // 350 ns max peak time
        it++;
      }  //else if(t>40.) mask=kTRUE; // hit not converted : possible under threshold, masked
      // 40fC threshold
    }

    time += dt;
    hTime = 0;
    while (jt != fDigiProc.end()) {
      hTime = get<0>(*jt);
      if (VERBOSE)
        printf(
          "        [R] htime[ns]=%d FT[ADC]=%4d ...\n", hTime, get<2>(*jt));
      if (hTime > time) break;
      jt++;
    }
    if (r > 0. && jt != fDigiProc.end()) {
      csTime = get<1>(*jt);
      //printf("match R htime[%d] time[%d]\n", hTime, time);
      if (hTime - time < 400) {  // found converted hit
        if (VERBOSE)
          printf("        [R] ht[ns]=%4d CS[ns]=%4d FT[ADC]=%4u Trg[%s]\n",
                 hTime,
                 csTime,
                 get<2>(*jt),
                 (get<3>(*jt) ? "S" : "N"));
        if (dtime) {
          if (csTime > dtime)
            ddt = TMath::Ceil(Int_t(csTime - dtime)
                              / CbmTrdDigi::Clk(CbmTrdDigi::kFASP));
          else
            ddt = TMath::Floor(Int_t(csTime - dtime)
                               / CbmTrdDigi::Clk(CbmTrdDigi::kFASP));
        } else
          dtime = csTime;
        rADC = get<2>(*jt);
        if (get<3>(*jt)) trigger |= 2;
        //if(csTime-hTime > 350) pileup = kTRUE; // 350 ns max peak time
        jt++;
      }  //else if(r>40.) mask=kTRUE;  // hit not converted : possible under threshold, masked
      // 40fC threshold
    }

    //update digi
    digi->SetMasked(1);  // mark as processed
    if (dtime) {
      digi->SetTime(fStartTime + dtime);
      digi->SetCharge(tADC, rADC, ddt);
      digi->SetTriggerType(trigger);
      if (VERBOSE) cout << "        OUT: " << digi->ToString();
    } else
      digi->SetFlag(0, kTRUE);  // mark for deletion
    if (VERBOSE)
      cout << "    ===================================="
           << "\n";
    iv++;
  }

  // try to merge digits
  ULong64_t time0, time1;
  Char_t type0,  // prompt digi. kTRUE if rectangle
    type1;       // late digi. kTRUE if rectangle
  for (iv = fDigi->begin(); iv != fDigi->end(); iv++) {
    digi = iv->first;
    if (digi->IsFlagged(0)) continue;  // no output
    if (!digi->IsMasked()) break;      // not finalized
    r0 = digi->GetCharge(t0, dt);
    if (r0 > 0. && t0 > 0.) continue;  // already complete
    type0 = (r0 > 0. ? 1 : 0);         // mark type for prompt digi
    time0 = digi->GetTimeDAQ();        // mark time for prompt digi
    for (jv = iv + 1; jv != fDigi->end(); jv++) {
      digi1 = jv->first;
      if (digi1->IsFlagged(0)) continue;  // no output
      r = digi1->GetCharge(t, dt);
      if (r > 0. && t > 0.) continue;  // already complete
      type1 = (r > 0. ? 1 : 0);        // mark type for late digi
      time1 = digi1->GetTimeDAQ();     // mark time for late digi

      // try merge
      if (type0 == type1) break;  // same type
      dt = time1 - time0;
      if (dt > 7) break;  // digits too far to be merged

      if (VERBOSE)
        cout << "MERGE: " << digi->ToString() << "       " << digi1->ToString();
      if (type0) {  // prompt digi rectangle
        digi->SetTime(digi1->GetTime());
        digi->SetCharge(t, r0, -dt);
        Int_t rtrg(digi->GetTriggerType() & 2),
          ttrg(digi1->GetTriggerType() & 1);
        digi->SetTriggerType(rtrg | ttrg);  //merge the triggers
      } else {                              // prompt digi tilt
        digi->SetCharge(t0, r, dt);
        Int_t ttrg(digi->GetTriggerType() & 1),
          rtrg(digi1->GetTriggerType() & 2);
        digi->SetTriggerType(rtrg | ttrg);  //merge the triggers
      }
      if (VERBOSE) cout << "RES  : " << digi->ToString();

      digi1->SetFlag(0, kTRUE);  // mark for deletion
      break;
    }
  }

  // reset processed digi vector
  fDigiProc.clear();
  fNraw = 0;
  fDigi = nullptr;  //remove old link
}

const Float_t CbmTrdFASP::fgkShaperPar[4]          = {0.068, 26.1, 2.61, 50};
const Float_t CbmTrdFASP::fgkShaperLUT[SHAPER_LUT] = {
  7.52e-04, 3.79e-03, 9.02e-03, 1.58e-02, 2.33e-02, 3.10e-02, 3.83e-02,
  4.48e-02, 5.03e-02, 5.46e-02, 5.78e-02, 5.99e-02, 6.10e-02, 6.11e-02,
  6.04e-02, 5.90e-02, 5.71e-02, 5.47e-02, 5.20e-02, 4.91e-02, 4.60e-02,
  4.29e-02, 3.98e-02, 3.67e-02, 3.37e-02, 3.09e-02, 2.81e-02, 2.55e-02,
  2.31e-02, 2.08e-02, 1.87e-02, 1.68e-02, 1.50e-02, 1.34e-02, 1.20e-02,
  1.06e-02, 9.42e-03, 8.34e-03, 7.37e-03, 6.50e-03, 5.72e-03, 5.03e-03,
  4.42e-03, 3.87e-03, 3.39e-03, 2.97e-03, 2.59e-03, 2.26e-03, 1.97e-03,
  1.71e-03, 1.49e-03, 1.29e-03, 1.12e-03, 9.73e-04, 8.43e-04, 7.30e-04,
  6.31e-04, 5.45e-04, 4.71e-04, 4.06e-04, 3.50e-04, 3.01e-04, 2.60e-04,
  2.23e-04, 1.92e-04, 1.65e-04, 1.42e-04, 1.22e-04, 1.04e-04, 8.94e-05,
  7.66e-05, 6.56e-05, 5.61e-05, 4.80e-05, 4.11e-05, 3.51e-05, 3.00e-05,
  2.56e-05, 2.19e-05, 1.86e-05};
const Float_t CbmTrdFASP::fgkDecayLUT[SHAPER_LUT] = {
  1.00e+00, 9.05e-01, 8.19e-01, 7.41e-01, 6.70e-01, 6.07e-01, 5.49e-01,
  4.97e-01, 4.49e-01, 4.07e-01, 3.68e-01, 3.33e-01, 3.01e-01, 2.73e-01,
  2.47e-01, 2.23e-01, 2.02e-01, 1.83e-01, 1.65e-01, 1.50e-01, 1.35e-01,
  1.22e-01, 1.11e-01, 1.00e-01, 9.07e-02, 8.21e-02, 7.43e-02, 6.72e-02,
  6.08e-02, 5.50e-02, 4.98e-02, 4.50e-02, 4.08e-02, 3.69e-02, 3.34e-02,
  3.02e-02, 2.73e-02, 2.47e-02, 2.24e-02, 2.02e-02, 1.83e-02, 1.66e-02,
  1.50e-02, 1.36e-02, 1.23e-02, 1.11e-02, 1.01e-02, 9.10e-03, 8.23e-03,
  7.45e-03, 6.74e-03, 6.10e-03, 5.52e-03, 4.99e-03, 4.52e-03, 4.09e-03,
  3.70e-03, 3.35e-03, 3.03e-03, 2.74e-03, 2.48e-03, 2.24e-03, 2.03e-03,
  1.84e-03, 1.66e-03, 1.50e-03, 1.36e-03, 1.23e-03, 1.11e-03, 1.01e-03,
  9.12e-04, 8.25e-04, 7.47e-04, 6.76e-04, 6.11e-04, 5.53e-04, 5.00e-04,
  4.53e-04, 4.10e-04, 3.71e-04};
const Float_t CbmTrdFASP::fgkCharge[CbmTrdFASP::fgkNDB] = {
  6,   12,  18,  24,  30,  36,  42,  48,  54,  60,  66,  72,  78,  84,
  90,  96,  102, 108, 114, 120, 126, 132, 138, 144, 150, 156, 162, 168,
  180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345, 360, 375,
  390, 405, 420, 435, 450, 465, 480, 495, 510, 525, 540};
const Float_t CbmTrdFASP::fgkShaper[CbmTrdFASP::fgkNDB][FASP_WINDOW] = {
  {// charge = 6 fC
   0.000,  0.000,  0.000,  0.001,  0.002,  0.004,  0.005,  0.007,  0.009,
   0.011,  0.012,  0.014,  0.015,  0.017,  0.018,  0.020,  0.021,  0.022,
   0.023,  0.024,  0.025,  0.025,  0.026,  0.027,  0.027,  0.028,  0.028,
   0.029,  0.029,  0.029,  0.029,  0.029,  0.029,  0.029,  0.029,  0.029,
   0.029,  0.029,  0.029,  0.029,  0.028,  0.028,  0.028,  0.027,  0.027,
   0.027,  0.026,  0.026,  0.025,  0.025,  0.024,  0.024,  0.024,  0.023,
   0.022,  0.022,  0.021,  0.021,  0.021,  0.020,  0.020,  0.019,  0.018,
   0.018,  0.017,  0.017,  0.016,  0.016,  0.016,  0.015,  0.015,  0.014,
   0.014,  0.013,  0.013,  0.012,  0.012,  0.011,  0.011,  0.011,  0.010,
   0.010,  0.009,  0.009,  0.009,  0.008,  0.008,  0.008,  0.007,  0.007,
   0.007,  0.006,  0.006,  0.006,  0.006,  0.005,  0.005,  0.005,  0.005,
   0.004,  0.004,  0.004,  0.004,  0.003,  0.003,  0.003,  0.003,  0.003,
   0.003,  0.002,  0.002,  0.002,  0.002,  0.002,  0.002,  0.002,  0.002,
   0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,
   0.001,  0.001,  0.001,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,
   0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  -0.000, -0.000, -0.000,
   -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000,
   -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000,
   -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000,
   -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000,
   -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000,
   -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000,
   -0.000, -0.000},
  {// charge = 12 fC
   0.000,  0.000,  0.001,  0.002,  0.004,  0.007,  0.011,  0.014,  0.017,
   0.021,  0.024,  0.027,  0.031,  0.034,  0.036,  0.039,  0.041,  0.044,
   0.046,  0.048,  0.049,  0.051,  0.052,  0.053,  0.055,  0.056,  0.056,
   0.057,  0.058,  0.058,  0.058,  0.059,  0.059,  0.059,  0.059,  0.058,
   0.058,  0.058,  0.058,  0.057,  0.057,  0.056,  0.055,  0.055,  0.054,
   0.053,  0.052,  0.052,  0.051,  0.050,  0.049,  0.048,  0.047,  0.046,
   0.045,  0.044,  0.043,  0.042,  0.041,  0.040,  0.039,  0.038,  0.037,
   0.036,  0.035,  0.034,  0.033,  0.032,  0.031,  0.030,  0.029,  0.028,
   0.027,  0.026,  0.025,  0.025,  0.024,  0.023,  0.022,  0.021,  0.020,
   0.020,  0.019,  0.018,  0.017,  0.017,  0.016,  0.015,  0.015,  0.014,
   0.013,  0.013,  0.012,  0.012,  0.011,  0.011,  0.010,  0.010,  0.009,
   0.009,  0.008,  0.008,  0.007,  0.007,  0.007,  0.006,  0.006,  0.006,
   0.005,  0.005,  0.005,  0.004,  0.004,  0.004,  0.003,  0.003,  0.003,
   0.003,  0.003,  0.002,  0.002,  0.002,  0.002,  0.002,  0.002,  0.001,
   0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.000,
   0.000,  0.000,  0.000,  0.000,  0.000,  -0.000, -0.000, -0.000, -0.000,
   -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.000, -0.000, -0.000,
   -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000, -0.000,
   -0.000, -0.000},
  {// charge = 18 fC
   0.000,  -0.000, 0.001,  0.003,  0.007,  0.011,  0.016,  0.021,  0.026,
   0.031,  0.036,  0.041,  0.046,  0.050,  0.054,  0.058,  0.062,  0.065,
   0.068,  0.071,  0.074,  0.076,  0.078,  0.080,  0.082,  0.083,  0.085,
   0.086,  0.086,  0.087,  0.088,  0.088,  0.088,  0.088,  0.088,  0.088,
   0.087,  0.087,  0.086,  0.086,  0.085,  0.084,  0.083,  0.082,  0.081,
   0.080,  0.079,  0.077,  0.076,  0.075,  0.073,  0.072,  0.071,  0.069,
   0.068,  0.066,  0.065,  0.063,  0.062,  0.060,  0.058,  0.057,  0.055,
   0.054,  0.052,  0.051,  0.049,  0.048,  0.046,  0.045,  0.044,  0.042,
   0.041,  0.039,  0.038,  0.037,  0.035,  0.034,  0.033,  0.032,  0.031,
   0.029,  0.028,  0.027,  0.026,  0.025,  0.024,  0.023,  0.022,  0.021,
   0.020,  0.019,  0.018,  0.018,  0.017,  0.016,  0.015,  0.015,  0.014,
   0.013,  0.012,  0.012,  0.011,  0.011,  0.010,  0.009,  0.009,  0.008,
   0.008,  0.007,  0.007,  0.007,  0.006,  0.006,  0.005,  0.005,  0.005,
   0.004,  0.004,  0.004,  0.003,  0.003,  0.003,  0.003,  0.002,  0.002,
   0.002,  0.002,  0.002,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,
   0.000,  0.000,  0.000,  0.000,  0.000,  -0.000, -0.000, -0.000, -0.000,
   -0.000, -0.000, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001},
  {// charge = 24 fC
   0.000,  -0.000, 0.001,  0.004,  0.009,  0.015,  0.021,  0.028,  0.035,
   0.042,  0.049,  0.055,  0.061,  0.067,  0.073,  0.078,  0.083,  0.087,
   0.091,  0.095,  0.099,  0.102,  0.104,  0.107,  0.109,  0.111,  0.113,
   0.114,  0.115,  0.116,  0.117,  0.117,  0.118,  0.118,  0.117,  0.117,
   0.117,  0.116,  0.115,  0.114,  0.113,  0.112,  0.111,  0.109,  0.108,
   0.106,  0.105,  0.103,  0.101,  0.100,  0.098,  0.096,  0.094,  0.092,
   0.090,  0.088,  0.086,  0.084,  0.082,  0.080,  0.078,  0.076,  0.074,
   0.072,  0.070,  0.068,  0.066,  0.064,  0.062,  0.060,  0.058,  0.056,
   0.054,  0.053,  0.051,  0.049,  0.047,  0.046,  0.044,  0.042,  0.041,
   0.039,  0.038,  0.036,  0.035,  0.033,  0.032,  0.031,  0.029,  0.028,
   0.027,  0.026,  0.025,  0.023,  0.022,  0.021,  0.020,  0.019,  0.018,
   0.017,  0.016,  0.016,  0.015,  0.014,  0.013,  0.012,  0.012,  0.011,
   0.011,  0.010,  0.009,  0.009,  0.008,  0.008,  0.007,  0.007,  0.006,
   0.006,  0.005,  0.005,  0.004,  0.004,  0.004,  0.003,  0.003,  0.003,
   0.002,  0.002,  0.002,  0.002,  0.002,  0.001,  0.001,  0.001,  0.001,
   0.001,  0.000,  0.000,  0.000,  0.000,  -0.000, -0.000, -0.000, -0.000,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001},
  {// charge = 30 fC
   0.000,  -0.000, 0.001,  0.005,  0.011,  0.018,  0.026,  0.035,  0.044,
   0.052,  0.061,  0.069,  0.076,  0.084,  0.091,  0.097,  0.103,  0.109,
   0.114,  0.119,  0.123,  0.127,  0.131,  0.134,  0.137,  0.139,  0.141,
   0.143,  0.144,  0.145,  0.146,  0.147,  0.147,  0.147,  0.147,  0.146,
   0.146,  0.145,  0.144,  0.143,  0.141,  0.140,  0.138,  0.137,  0.135,
   0.133,  0.131,  0.129,  0.127,  0.125,  0.122,  0.120,  0.118,  0.115,
   0.113,  0.110,  0.108,  0.105,  0.103,  0.100,  0.097,  0.095,  0.092,
   0.090,  0.087,  0.085,  0.082,  0.080,  0.077,  0.075,  0.073,  0.070,
   0.068,  0.066,  0.063,  0.061,  0.059,  0.057,  0.055,  0.053,  0.051,
   0.049,  0.047,  0.045,  0.044,  0.042,  0.040,  0.038,  0.037,  0.035,
   0.034,  0.032,  0.031,  0.029,  0.028,  0.027,  0.025,  0.024,  0.023,
   0.022,  0.021,  0.020,  0.019,  0.018,  0.017,  0.016,  0.015,  0.014,
   0.013,  0.012,  0.012,  0.011,  0.010,  0.009,  0.009,  0.008,  0.008,
   0.007,  0.007,  0.006,  0.006,  0.005,  0.005,  0.004,  0.004,  0.003,
   0.003,  0.003,  0.002,  0.002,  0.002,  0.002,  0.001,  0.001,  0.001,
   0.001,  0.001,  0.000,  0.000,  0.000,  -0.000, -0.000, -0.000, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001},
  {// charge = 36 fC
   0.000,  -0.000, 0.002,  0.006,  0.013,  0.022,  0.032,  0.042,  0.052,
   0.063,  0.073,  0.082,  0.092,  0.101,  0.109,  0.117,  0.124,  0.131,
   0.137,  0.143,  0.148,  0.153,  0.157,  0.160,  0.164,  0.167,  0.169,
   0.171,  0.173,  0.174,  0.175,  0.176,  0.176,  0.176,  0.176,  0.176,
   0.175,  0.174,  0.173,  0.171,  0.170,  0.168,  0.166,  0.164,  0.162,
   0.160,  0.157,  0.155,  0.152,  0.150,  0.147,  0.144,  0.141,  0.138,
   0.135,  0.132,  0.129,  0.126,  0.123,  0.120,  0.117,  0.114,  0.111,
   0.108,  0.105,  0.102,  0.099,  0.096,  0.093,  0.090,  0.087,  0.084,
   0.082,  0.079,  0.076,  0.074,  0.071,  0.068,  0.066,  0.064,  0.061,
   0.059,  0.057,  0.054,  0.052,  0.050,  0.048,  0.046,  0.044,  0.042,
   0.040,  0.039,  0.037,  0.035,  0.034,  0.032,  0.030,  0.029,  0.027,
   0.026,  0.025,  0.024,  0.022,  0.021,  0.020,  0.019,  0.018,  0.017,
   0.016,  0.015,  0.014,  0.013,  0.012,  0.011,  0.011,  0.010,  0.009,
   0.008,  0.008,  0.007,  0.007,  0.006,  0.006,  0.005,  0.005,  0.004,
   0.004,  0.003,  0.003,  0.003,  0.002,  0.002,  0.002,  0.001,  0.001,
   0.001,  0.001,  0.000,  0.000,  0.000,  -0.000, -0.000, -0.000, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001},
  {// charge = 42 fC
   0.000,  -0.000, 0.002,  0.007,  0.016,  0.026,  0.037,  0.049,  0.061,
   0.073,  0.085,  0.096,  0.107,  0.117,  0.127,  0.136,  0.145,  0.152,
   0.160,  0.166,  0.173,  0.178,  0.183,  0.187,  0.191,  0.194,  0.197,
   0.200,  0.202,  0.203,  0.205,  0.205,  0.206,  0.206,  0.206,  0.205,
   0.204,  0.203,  0.202,  0.200,  0.198,  0.196,  0.194,  0.192,  0.189,
   0.186,  0.184,  0.181,  0.178,  0.174,  0.171,  0.168,  0.165,  0.161,
   0.158,  0.154,  0.151,  0.147,  0.144,  0.140,  0.137,  0.133,  0.129,
   0.126,  0.122,  0.119,  0.115,  0.112,  0.109,  0.105,  0.102,  0.099,
   0.095,  0.092,  0.089,  0.086,  0.083,  0.080,  0.077,  0.074,  0.071,
   0.069,  0.066,  0.063,  0.061,  0.058,  0.056,  0.054,  0.051,  0.049,
   0.047,  0.045,  0.043,  0.041,  0.039,  0.037,  0.035,  0.034,  0.032,
   0.030,  0.029,  0.027,  0.026,  0.024,  0.023,  0.022,  0.021,  0.020,
   0.018,  0.017,  0.016,  0.015,  0.014,  0.013,  0.012,  0.012,  0.011,
   0.010,  0.009,  0.008,  0.008,  0.007,  0.007,  0.006,  0.005,  0.005,
   0.004,  0.004,  0.003,  0.003,  0.003,  0.002,  0.002,  0.002,  0.001,
   0.001,  0.001,  0.001,  0.000,  0.000,  -0.000, -0.000, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001, -0.001,
   -0.001, -0.001},
  {// charge = 48 fC
   0.000,  -0.000, 0.002,  0.008,  0.018,  0.029,  0.042,  0.056,  0.070,
   0.083,  0.097,  0.110,  0.122,  0.134,  0.145,  0.155,  0.165,  0.174,
   0.183,  0.190,  0.197,  0.203,  0.209,  0.214,  0.218,  0.222,  0.226,
   0.228,  0.231,  0.232,  0.234,  0.235,  0.235,  0.235,  0.235,  0.234,
   0.233,  0.232,  0.230,  0.229,  0.227,  0.224,  0.222,  0.219,  0.216,
   0.213,  0.210,  0.206,  0.203,  0.200,  0.196,  0.192,  0.188,  0.184,
   0.180,  0.176,  0.172,  0.168,  0.164,  0.160,  0.156,  0.152,  0.148,
   0.144,  0.140,  0.136,  0.132,  0.128,  0.124,  0.120,  0.116,  0.113,
   0.109,  0.105,  0.102,  0.098,  0.095,  0.091,  0.088,  0.085,  0.082,
   0.078,  0.076,  0.072,  0.070,  0.067,  0.064,  0.061,  0.059,  0.056,
   0.054,  0.051,  0.049,  0.047,  0.045,  0.043,  0.041,  0.039,  0.037,
   0.035,  0.033,  0.031,  0.030,  0.028,  0.026,  0.025,  0.024,  0.022,
   0.021,  0.020,  0.018,  0.017,  0.016,  0.015,  0.014,  0.013,  0.012,
   0.011,  0.011,  0.010,  0.009,  0.008,  0.007,  0.007,  0.006,  0.006,
   0.005,  0.004,  0.004,  0.003,  0.003,  0.003,  0.002,  0.002,  0.002,
   0.001,  0.001,  0.001,  0.000,  0.000,  -0.000, -0.000, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.001, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.001, -0.001, -0.001,
   -0.001, -0.001},
  {// charge = 54 fC
   0.000,  -0.000, 0.002,  0.009,  0.020,  0.033,  0.047,  0.063,  0.078,
   0.094,  0.109,  0.123,  0.137,  0.151,  0.163,  0.175,  0.186,  0.196,
   0.205,  0.214,  0.222,  0.229,  0.235,  0.241,  0.246,  0.250,  0.254,
   0.257,  0.259,  0.261,  0.263,  0.264,  0.264,  0.264,  0.264,  0.263,
   0.262,  0.261,  0.259,  0.257,  0.255,  0.252,  0.249,  0.247,  0.243,
   0.240,  0.236,  0.232,  0.229,  0.225,  0.220,  0.216,  0.212,  0.207,
   0.203,  0.198,  0.194,  0.189,  0.185,  0.180,  0.176,  0.171,  0.166,
   0.162,  0.157,  0.153,  0.148,  0.144,  0.140,  0.135,  0.131,  0.127,
   0.123,  0.118,  0.114,  0.110,  0.107,  0.103,  0.099,  0.095,  0.092,
   0.088,  0.085,  0.082,  0.078,  0.075,  0.072,  0.069,  0.066,  0.063,
   0.060,  0.058,  0.055,  0.053,  0.050,  0.048,  0.046,  0.043,  0.041,
   0.039,  0.037,  0.035,  0.034,  0.031,  0.030,  0.028,  0.027,  0.025,
   0.024,  0.022,  0.021,  0.020,  0.018,  0.017,  0.016,  0.015,  0.014,
   0.013,  0.012,  0.011,  0.010,  0.009,  0.008,  0.008,  0.007,  0.006,
   0.006,  0.005,  0.004,  0.004,  0.003,  0.003,  0.002,  0.002,  0.002,
   0.001,  0.001,  0.001,  0.000,  0.000,  -0.000, -0.000, -0.001, -0.001,
   -0.001, -0.001, -0.001, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002},
  {// charge = 60 fC
   0.000,  -0.000, 0.002,  0.010,  0.022,  0.036,  0.053,  0.070,  0.087,
   0.104,  0.121,  0.137,  0.152,  0.167,  0.181,  0.194,  0.206,  0.218,
   0.228,  0.238,  0.246,  0.254,  0.261,  0.267,  0.273,  0.278,  0.282,
   0.285,  0.288,  0.290,  0.292,  0.293,  0.294,  0.294,  0.293,  0.293,
   0.292,  0.290,  0.288,  0.286,  0.283,  0.280,  0.277,  0.274,  0.270,
   0.266,  0.262,  0.258,  0.254,  0.249,  0.245,  0.240,  0.235,  0.230,
   0.225,  0.220,  0.215,  0.210,  0.205,  0.200,  0.195,  0.190,  0.185,
   0.180,  0.175,  0.170,  0.165,  0.160,  0.155,  0.150,  0.146,  0.141,
   0.136,  0.132,  0.127,  0.123,  0.118,  0.114,  0.110,  0.106,  0.102,
   0.098,  0.094,  0.091,  0.087,  0.083,  0.080,  0.077,  0.073,  0.070,
   0.067,  0.064,  0.061,  0.059,  0.056,  0.053,  0.051,  0.048,  0.046,
   0.044,  0.041,  0.039,  0.037,  0.035,  0.033,  0.031,  0.030,  0.028,
   0.026,  0.025,  0.023,  0.022,  0.020,  0.019,  0.018,  0.016,  0.015,
   0.014,  0.013,  0.012,  0.011,  0.010,  0.009,  0.008,  0.008,  0.007,
   0.006,  0.006,  0.005,  0.004,  0.004,  0.003,  0.003,  0.002,  0.002,
   0.002,  0.001,  0.001,  0.000,  0.000,  -0.000, -0.001, -0.001, -0.001,
   -0.001, -0.001, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002},
  {// charge = 66 fC
   0.000,  -0.000, 0.003,  0.011,  0.024,  0.040,  0.058,  0.076,  0.095,
   0.114,  0.133,  0.151,  0.168,  0.184,  0.199,  0.214,  0.227,  0.239,
   0.251,  0.261,  0.271,  0.279,  0.287,  0.294,  0.300,  0.305,  0.310,
   0.314,  0.317,  0.319,  0.321,  0.322,  0.323,  0.323,  0.323,  0.322,
   0.321,  0.319,  0.317,  0.314,  0.312,  0.308,  0.305,  0.301,  0.297,
   0.293,  0.289,  0.284,  0.279,  0.274,  0.269,  0.264,  0.259,  0.253,
   0.248,  0.243,  0.237,  0.231,  0.226,  0.220,  0.215,  0.209,  0.203,
   0.198,  0.192,  0.187,  0.181,  0.176,  0.171,  0.165,  0.160,  0.155,
   0.150,  0.145,  0.140,  0.135,  0.130,  0.126,  0.121,  0.117,  0.112,
   0.108,  0.104,  0.100,  0.096,  0.092,  0.088,  0.084,  0.081,  0.077,
   0.074,  0.071,  0.067,  0.064,  0.062,  0.059,  0.056,  0.053,  0.050,
   0.048,  0.045,  0.043,  0.041,  0.039,  0.036,  0.035,  0.032,  0.031,
   0.029,  0.027,  0.025,  0.024,  0.022,  0.021,  0.020,  0.018,  0.017,
   0.016,  0.014,  0.013,  0.012,  0.011,  0.010,  0.009,  0.008,  0.008,
   0.007,  0.006,  0.006,  0.005,  0.004,  0.004,  0.003,  0.003,  0.002,
   0.002,  0.001,  0.001,  0.000,  0.000,  -0.000, -0.001, -0.001, -0.001,
   -0.001, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002},
  {// charge = 72 fC
   0.000,  -0.000, 0.003,  0.012,  0.026,  0.044,  0.063,  0.083,  0.104,
   0.124,  0.145,  0.164,  0.183,  0.201,  0.217,  0.233,  0.247,  0.261,
   0.273,  0.285,  0.295,  0.305,  0.313,  0.321,  0.327,  0.333,  0.338,
   0.342,  0.346,  0.348,  0.350,  0.352,  0.353,  0.353,  0.352,  0.351,
   0.350,  0.348,  0.346,  0.343,  0.340,  0.336,  0.333,  0.329,  0.324,
   0.320,  0.315,  0.310,  0.305,  0.299,  0.294,  0.288,  0.282,  0.277,
   0.271,  0.265,  0.259,  0.253,  0.247,  0.240,  0.234,  0.228,  0.222,
   0.216,  0.210,  0.204,  0.198,  0.192,  0.186,  0.180,  0.175,  0.169,
   0.163,  0.158,  0.153,  0.147,  0.142,  0.137,  0.132,  0.127,  0.123,
   0.118,  0.113,  0.109,  0.104,  0.100,  0.096,  0.092,  0.088,  0.084,
   0.081,  0.077,  0.074,  0.070,  0.067,  0.064,  0.061,  0.058,  0.055,
   0.053,  0.049,  0.047,  0.044,  0.042,  0.040,  0.038,  0.035,  0.033,
   0.031,  0.030,  0.028,  0.026,  0.024,  0.023,  0.021,  0.020,  0.018,
   0.017,  0.016,  0.015,  0.013,  0.012,  0.011,  0.010,  0.009,  0.008,
   0.008,  0.007,  0.006,  0.005,  0.005,  0.004,  0.003,  0.003,  0.002,
   0.002,  0.001,  0.001,  0.001,  0.000,  -0.000, -0.001, -0.001, -0.001,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.002,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002},
  {// charge = 78 fC
   0.000,  -0.000, 0.003,  0.013,  0.029,  0.047,  0.068,  0.090,  0.112,
   0.135,  0.156,  0.178,  0.198,  0.217,  0.235,  0.252,  0.268,  0.283,
   0.296,  0.309,  0.320,  0.330,  0.339,  0.347,  0.355,  0.361,  0.366,
   0.371,  0.375,  0.378,  0.380,  0.381,  0.382,  0.382,  0.382,  0.381,
   0.379,  0.377,  0.375,  0.372,  0.368,  0.365,  0.361,  0.356,  0.351,
   0.346,  0.341,  0.336,  0.330,  0.324,  0.318,  0.312,  0.306,  0.300,
   0.293,  0.287,  0.280,  0.274,  0.267,  0.260,  0.254,  0.247,  0.240,
   0.234,  0.227,  0.221,  0.214,  0.208,  0.202,  0.195,  0.189,  0.183,
   0.177,  0.171,  0.165,  0.160,  0.154,  0.149,  0.143,  0.138,  0.133,
   0.128,  0.123,  0.118,  0.113,  0.109,  0.104,  0.100,  0.096,  0.091,
   0.087,  0.084,  0.080,  0.076,  0.073,  0.069,  0.066,  0.063,  0.060,
   0.057,  0.053,  0.051,  0.048,  0.046,  0.043,  0.041,  0.038,  0.036,
   0.034,  0.032,  0.030,  0.028,  0.026,  0.025,  0.023,  0.021,  0.020,
   0.018,  0.017,  0.016,  0.015,  0.013,  0.012,  0.011,  0.010,  0.009,
   0.008,  0.007,  0.007,  0.006,  0.005,  0.004,  0.004,  0.003,  0.002,
   0.002,  0.001,  0.001,  0.001,  0.000,  -0.000, -0.001, -0.001, -0.001,
   -0.002, -0.002, -0.002, -0.002, -0.002, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.002, -0.002, -0.002, -0.002, -0.002, -0.002,
   -0.002, -0.002},
  {// charge = 84 fC
   0.000,  -0.000, 0.003,  0.014,  0.031,  0.051,  0.073,  0.097,  0.121,
   0.145,  0.168,  0.191,  0.213,  0.234,  0.253,  0.271,  0.288,  0.304,
   0.319,  0.332,  0.344,  0.355,  0.365,  0.374,  0.382,  0.389,  0.395,
   0.399,  0.403,  0.407,  0.409,  0.410,  0.411,  0.412,  0.411,  0.410,
   0.408,  0.406,  0.403,  0.400,  0.397,  0.393,  0.388,  0.384,  0.379,
   0.373,  0.368,  0.362,  0.356,  0.349,  0.343,  0.336,  0.330,  0.323,
   0.316,  0.309,  0.302,  0.295,  0.288,  0.280,  0.273,  0.266,  0.259,
   0.252,  0.245,  0.238,  0.231,  0.224,  0.217,  0.210,  0.204,  0.197,
   0.191,  0.184,  0.178,  0.172,  0.166,  0.160,  0.154,  0.149,  0.143,
   0.137,  0.132,  0.127,  0.122,  0.117,  0.112,  0.108,  0.103,  0.099,
   0.094,  0.090,  0.086,  0.082,  0.078,  0.075,  0.071,  0.067,  0.064,
   0.062,  0.058,  0.055,  0.052,  0.049,  0.046,  0.044,  0.041,  0.039,
   0.037,  0.035,  0.032,  0.030,  0.028,  0.026,  0.025,  0.023,  0.021,
   0.020,  0.018,  0.017,  0.016,  0.014,  0.013,  0.012,  0.011,  0.010,
   0.009,  0.008,  0.007,  0.006,  0.005,  0.005,  0.004,  0.003,  0.003,
   0.002,  0.002,  0.001,  0.001,  0.000,  -0.000, -0.001, -0.001, -0.001,
   -0.002, -0.002, -0.002, -0.002, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.002, -0.002, -0.002,
   -0.002, -0.002},
  {// charge = 90 fC
   0.000,  -0.000, 0.004,  0.015,  0.033,  0.054,  0.078,  0.104,  0.129,
   0.155,  0.180,  0.205,  0.228,  0.250,  0.271,  0.291,  0.309,  0.326,
   0.341,  0.356,  0.369,  0.381,  0.391,  0.401,  0.409,  0.416,  0.423,
   0.428,  0.432,  0.436,  0.438,  0.440,  0.441,  0.441,  0.440,  0.439,
   0.438,  0.435,  0.432,  0.429,  0.425,  0.421,  0.416,  0.411,  0.406,
   0.400,  0.394,  0.388,  0.381,  0.374,  0.368,  0.360,  0.353,  0.346,
   0.339,  0.331,  0.323,  0.316,  0.308,  0.301,  0.293,  0.285,  0.277,
   0.270,  0.262,  0.255,  0.247,  0.240,  0.233,  0.225,  0.218,  0.211,
   0.204,  0.198,  0.191,  0.184,  0.178,  0.171,  0.165,  0.159,  0.153,
   0.147,  0.142,  0.136,  0.131,  0.125,  0.120,  0.115,  0.110,  0.105,
   0.101,  0.096,  0.092,  0.088,  0.084,  0.080,  0.076,  0.072,  0.069,
   0.066,  0.062,  0.059,  0.056,  0.053,  0.050,  0.047,  0.044,  0.042,
   0.039,  0.037,  0.035,  0.032,  0.030,  0.029,  0.026,  0.025,  0.023,
   0.021,  0.020,  0.018,  0.017,  0.015,  0.014,  0.013,  0.012,  0.011,
   0.009,  0.008,  0.007,  0.007,  0.006,  0.005,  0.004,  0.003,  0.003,
   0.002,  0.002,  0.001,  0.001,  0.000,  -0.000, -0.001, -0.001, -0.002,
   -0.002, -0.002, -0.002, -0.003, -0.003, -0.003, -0.003, -0.003, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.002, -0.002},
  {// charge = 96 fC
   0.000,  -0.000, 0.004,  0.016,  0.035,  0.058,  0.083,  0.110,  0.138,
   0.165,  0.192,  0.218,  0.243,  0.266,  0.289,  0.310,  0.329,  0.347,
   0.364,  0.379,  0.393,  0.406,  0.417,  0.427,  0.436,  0.444,  0.451,
   0.456,  0.461,  0.465,  0.467,  0.469,  0.470,  0.470,  0.470,  0.469,
   0.467,  0.464,  0.461,  0.458,  0.453,  0.449,  0.444,  0.439,  0.433,
   0.427,  0.420,  0.414,  0.407,  0.399,  0.392,  0.385,  0.377,  0.369,
   0.361,  0.353,  0.345,  0.337,  0.329,  0.321,  0.312,  0.304,  0.296,
   0.288,  0.280,  0.272,  0.264,  0.256,  0.248,  0.241,  0.233,  0.225,
   0.218,  0.211,  0.204,  0.197,  0.190,  0.183,  0.176,  0.170,  0.163,
   0.157,  0.151,  0.145,  0.139,  0.134,  0.128,  0.123,  0.118,  0.113,
   0.108,  0.103,  0.098,  0.094,  0.089,  0.085,  0.081,  0.077,  0.073,
   0.070,  0.066,  0.063,  0.059,  0.056,  0.053,  0.050,  0.047,  0.045,
   0.042,  0.039,  0.037,  0.035,  0.032,  0.030,  0.028,  0.026,  0.025,
   0.023,  0.021,  0.019,  0.018,  0.016,  0.015,  0.014,  0.012,  0.011,
   0.010,  0.009,  0.008,  0.007,  0.006,  0.005,  0.004,  0.004,  0.003,
   0.002,  0.002,  0.001,  0.001,  0.000,  -0.000, -0.001, -0.001, -0.002,
   -0.002, -0.002, -0.003, -0.003, -0.003, -0.003, -0.003, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.003, -0.003, -0.003,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003},
  {// charge = 102 fC
   0.000,  -0.000, 0.004,  0.017,  0.037,  0.061,  0.088,  0.117,  0.146,
   0.175,  0.204,  0.231,  0.258,  0.283,  0.307,  0.329,  0.350,  0.369,
   0.387,  0.403,  0.418,  0.431,  0.443,  0.454,  0.464,  0.472,  0.479,
   0.485,  0.490,  0.494,  0.497,  0.498,  0.500,  0.500,  0.499,  0.498,
   0.496,  0.493,  0.490,  0.486,  0.482,  0.477,  0.472,  0.466,  0.460,
   0.453,  0.447,  0.439,  0.432,  0.425,  0.417,  0.409,  0.401,  0.392,
   0.384,  0.375,  0.367,  0.358,  0.349,  0.341,  0.332,  0.323,  0.315,
   0.306,  0.297,  0.289,  0.280,  0.272,  0.264,  0.256,  0.247,  0.240,
   0.232,  0.224,  0.216,  0.209,  0.202,  0.194,  0.187,  0.180,  0.174,
   0.167,  0.161,  0.154,  0.148,  0.142,  0.136,  0.131,  0.125,  0.120,
   0.114,  0.109,  0.104,  0.100,  0.095,  0.091,  0.086,  0.082,  0.078,
   0.075,  0.070,  0.067,  0.063,  0.060,  0.056,  0.053,  0.050,  0.047,
   0.045,  0.042,  0.039,  0.037,  0.035,  0.032,  0.030,  0.028,  0.026,
   0.024,  0.022,  0.021,  0.019,  0.017,  0.016,  0.015,  0.013,  0.012,
   0.011,  0.010,  0.008,  0.007,  0.007,  0.006,  0.005,  0.004,  0.003,
   0.002,  0.002,  0.001,  0.001,  0.000,  -0.000, -0.001, -0.001, -0.002,
   -0.002, -0.002, -0.003, -0.003, -0.003, -0.003, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003},
  {// charge = 108 fC
   0.000,  -0.000, 0.004,  0.018,  0.039,  0.065,  0.093,  0.124,  0.155,
   0.185,  0.215,  0.245,  0.273,  0.299,  0.324,  0.348,  0.370,  0.390,
   0.409,  0.427,  0.442,  0.457,  0.469,  0.481,  0.491,  0.500,  0.507,
   0.513,  0.519,  0.523,  0.526,  0.528,  0.529,  0.529,  0.529,  0.527,
   0.525,  0.522,  0.519,  0.515,  0.510,  0.505,  0.500,  0.493,  0.487,
   0.480,  0.473,  0.465,  0.458,  0.450,  0.441,  0.433,  0.424,  0.415,
   0.406,  0.397,  0.388,  0.379,  0.370,  0.361,  0.352,  0.342,  0.333,
   0.324,  0.315,  0.306,  0.297,  0.288,  0.279,  0.271,  0.262,  0.254,
   0.245,  0.237,  0.229,  0.221,  0.214,  0.206,  0.198,  0.191,  0.184,
   0.177,  0.170,  0.163,  0.157,  0.150,  0.144,  0.138,  0.132,  0.127,
   0.121,  0.116,  0.111,  0.105,  0.101,  0.096,  0.091,  0.087,  0.083,
   0.079,  0.074,  0.070,  0.067,  0.063,  0.060,  0.056,  0.053,  0.050,
   0.047,  0.044,  0.042,  0.039,  0.036,  0.034,  0.032,  0.030,  0.027,
   0.025,  0.024,  0.022,  0.020,  0.018,  0.017,  0.015,  0.014,  0.013,
   0.011,  0.010,  0.009,  0.008,  0.007,  0.006,  0.005,  0.004,  0.003,
   0.003,  0.002,  0.001,  0.001,  0.000,  -0.001, -0.001, -0.001, -0.002,
   -0.002, -0.003, -0.003, -0.003, -0.003, -0.004, -0.004, -0.004, -0.004,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.003, -0.003, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003},
  {// charge = 114 fC
   0.000,  -0.000, 0.004,  0.019,  0.041,  0.068,  0.098,  0.130,  0.163,
   0.195,  0.227,  0.258,  0.288,  0.316,  0.342,  0.367,  0.390,  0.412,
   0.432,  0.450,  0.467,  0.482,  0.495,  0.507,  0.518,  0.527,  0.535,
   0.542,  0.547,  0.552,  0.555,  0.557,  0.558,  0.559,  0.558,  0.557,
   0.554,  0.551,  0.548,  0.544,  0.539,  0.533,  0.527,  0.521,  0.514,
   0.507,  0.499,  0.491,  0.483,  0.475,  0.466,  0.457,  0.448,  0.439,
   0.429,  0.420,  0.410,  0.400,  0.391,  0.381,  0.371,  0.362,  0.352,
   0.342,  0.333,  0.323,  0.314,  0.304,  0.295,  0.286,  0.277,  0.268,
   0.259,  0.250,  0.242,  0.234,  0.225,  0.217,  0.209,  0.202,  0.194,
   0.187,  0.179,  0.173,  0.166,  0.159,  0.152,  0.146,  0.140,  0.134,
   0.128,  0.122,  0.117,  0.112,  0.106,  0.101,  0.096,  0.092,  0.087,
   0.083,  0.078,  0.074,  0.071,  0.067,  0.063,  0.060,  0.056,  0.053,
   0.050,  0.047,  0.044,  0.041,  0.039,  0.036,  0.034,  0.031,  0.029,
   0.027,  0.025,  0.023,  0.021,  0.019,  0.018,  0.016,  0.015,  0.013,
   0.012,  0.011,  0.009,  0.008,  0.007,  0.006,  0.005,  0.004,  0.003,
   0.003,  0.002,  0.001,  0.001,  0.000,  -0.001, -0.001, -0.002, -0.002,
   -0.002, -0.003, -0.003, -0.003, -0.004, -0.004, -0.004, -0.004, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.006, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.003, -0.003, -0.003, -0.003,
   -0.003, -0.003},
  {// charge = 120 fC
   0.000,  -0.000, 0.005,  0.020,  0.043,  0.072,  0.103,  0.137,  0.171,
   0.205,  0.239,  0.271,  0.302,  0.332,  0.360,  0.386,  0.411,  0.433,
   0.454,  0.474,  0.491,  0.507,  0.521,  0.534,  0.545,  0.555,  0.563,
   0.570,  0.576,  0.581,  0.584,  0.586,  0.588,  0.588,  0.587,  0.586,
   0.584,  0.581,  0.577,  0.572,  0.567,  0.561,  0.555,  0.548,  0.541,
   0.534,  0.526,  0.517,  0.509,  0.500,  0.491,  0.481,  0.472,  0.462,
   0.452,  0.442,  0.432,  0.422,  0.411,  0.401,  0.391,  0.381,  0.371,
   0.360,  0.350,  0.340,  0.330,  0.320,  0.311,  0.301,  0.291,  0.282,
   0.273,  0.264,  0.255,  0.246,  0.237,  0.229,  0.220,  0.212,  0.204,
   0.197,  0.189,  0.182,  0.174,  0.167,  0.160,  0.154,  0.147,  0.141,
   0.135,  0.129,  0.123,  0.117,  0.112,  0.107,  0.101,  0.097,  0.092,
   0.088,  0.082,  0.078,  0.074,  0.070,  0.066,  0.063,  0.059,  0.056,
   0.052,  0.049,  0.046,  0.043,  0.041,  0.038,  0.035,  0.033,  0.031,
   0.028,  0.026,  0.024,  0.022,  0.021,  0.019,  0.017,  0.016,  0.014,
   0.013,  0.011,  0.010,  0.009,  0.008,  0.007,  0.006,  0.005,  0.004,
   0.003,  0.002,  0.001,  0.001,  0.000,  -0.001, -0.001, -0.002, -0.002,
   -0.002, -0.003, -0.003, -0.004, -0.004, -0.004, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.004, -0.004, -0.004,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.003, -0.003,
   -0.003, -0.003},
  {// charge = 126 fC
   0.000,  -0.000, 0.005,  0.021,  0.045,  0.075,  0.108,  0.143,  0.179,
   0.215,  0.250,  0.284,  0.317,  0.348,  0.378,  0.405,  0.431,  0.455,
   0.477,  0.497,  0.515,  0.532,  0.547,  0.560,  0.572,  0.583,  0.591,
   0.599,  0.605,  0.610,  0.613,  0.616,  0.617,  0.617,  0.617,  0.615,
   0.613,  0.610,  0.606,  0.601,  0.596,  0.590,  0.583,  0.576,  0.568,
   0.560,  0.552,  0.543,  0.534,  0.525,  0.515,  0.505,  0.495,  0.485,
   0.475,  0.464,  0.453,  0.443,  0.432,  0.421,  0.411,  0.400,  0.389,
   0.378,  0.368,  0.357,  0.347,  0.336,  0.326,  0.316,  0.306,  0.296,
   0.286,  0.277,  0.267,  0.258,  0.249,  0.240,  0.232,  0.223,  0.215,
   0.206,  0.198,  0.191,  0.183,  0.176,  0.169,  0.161,  0.155,  0.148,
   0.141,  0.135,  0.129,  0.123,  0.118,  0.112,  0.107,  0.101,  0.096,
   0.092,  0.086,  0.082,  0.078,  0.074,  0.070,  0.066,  0.062,  0.058,
   0.055,  0.052,  0.049,  0.045,  0.043,  0.040,  0.037,  0.035,  0.032,
   0.030,  0.027,  0.025,  0.023,  0.021,  0.020,  0.018,  0.016,  0.015,
   0.013,  0.012,  0.010,  0.009,  0.008,  0.007,  0.006,  0.005,  0.004,
   0.003,  0.002,  0.001,  0.001,  0.000,  -0.001, -0.001, -0.002, -0.002,
   -0.003, -0.003, -0.003, -0.004, -0.004, -0.004, -0.005, -0.005, -0.005,
   -0.005, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.003, -0.003},
  {// charge = 132 fC
   0.000,  -0.000, 0.005,  0.022,  0.047,  0.078,  0.113,  0.150,  0.188,
   0.225,  0.262,  0.298,  0.332,  0.365,  0.395,  0.424,  0.451,  0.476,
   0.499,  0.520,  0.540,  0.557,  0.573,  0.587,  0.599,  0.610,  0.619,
   0.627,  0.634,  0.639,  0.642,  0.645,  0.647,  0.647,  0.646,  0.645,
   0.642,  0.639,  0.635,  0.630,  0.624,  0.618,  0.611,  0.604,  0.596,
   0.587,  0.578,  0.569,  0.560,  0.550,  0.540,  0.529,  0.519,  0.508,
   0.497,  0.486,  0.475,  0.464,  0.453,  0.441,  0.430,  0.419,  0.408,
   0.397,  0.385,  0.374,  0.363,  0.353,  0.342,  0.331,  0.321,  0.310,
   0.300,  0.290,  0.280,  0.271,  0.261,  0.252,  0.243,  0.234,  0.225,
   0.216,  0.208,  0.200,  0.192,  0.184,  0.177,  0.169,  0.162,  0.155,
   0.148,  0.142,  0.135,  0.129,  0.123,  0.117,  0.112,  0.106,  0.101,
   0.097,  0.091,  0.086,  0.082,  0.077,  0.073,  0.069,  0.065,  0.061,
   0.058,  0.054,  0.051,  0.048,  0.045,  0.042,  0.039,  0.036,  0.034,
   0.031,  0.029,  0.027,  0.025,  0.022,  0.021,  0.019,  0.017,  0.015,
   0.014,  0.012,  0.011,  0.010,  0.008,  0.007,  0.006,  0.005,  0.004,
   0.003,  0.002,  0.002,  0.001,  0.000,  -0.001, -0.001, -0.002, -0.002,
   -0.003, -0.003, -0.004, -0.004, -0.004, -0.005, -0.005, -0.005, -0.005,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004},
  {// charge = 138 fC
   0.000,  -0.000, 0.005,  0.023,  0.049,  0.081,  0.118,  0.156,  0.196,
   0.235,  0.274,  0.311,  0.347,  0.381,  0.413,  0.443,  0.471,  0.497,
   0.522,  0.544,  0.564,  0.582,  0.599,  0.614,  0.626,  0.638,  0.647,
   0.656,  0.662,  0.668,  0.672,  0.674,  0.676,  0.676,  0.676,  0.674,
   0.671,  0.668,  0.663,  0.658,  0.653,  0.646,  0.639,  0.631,  0.623,
   0.614,  0.605,  0.595,  0.585,  0.575,  0.564,  0.554,  0.543,  0.531,
   0.520,  0.509,  0.497,  0.485,  0.473,  0.462,  0.450,  0.438,  0.426,
   0.415,  0.403,  0.391,  0.380,  0.369,  0.357,  0.346,  0.335,  0.325,
   0.314,  0.303,  0.293,  0.283,  0.273,  0.263,  0.254,  0.244,  0.235,
   0.226,  0.218,  0.209,  0.201,  0.193,  0.185,  0.177,  0.169,  0.162,
   0.155,  0.148,  0.141,  0.135,  0.129,  0.123,  0.117,  0.111,  0.106,
   0.101,  0.095,  0.090,  0.085,  0.081,  0.076,  0.072,  0.068,  0.064,
   0.060,  0.057,  0.053,  0.050,  0.047,  0.044,  0.041,  0.038,  0.035,
   0.033,  0.030,  0.028,  0.026,  0.024,  0.021,  0.020,  0.018,  0.016,
   0.014,  0.013,  0.011,  0.010,  0.009,  0.007,  0.006,  0.005,  0.004,
   0.003,  0.002,  0.002,  0.001,  0.000,  -0.001, -0.001, -0.002, -0.002,
   -0.003, -0.003, -0.004, -0.004, -0.005, -0.005, -0.005, -0.005, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.007, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.004, -0.004, -0.004, -0.004, -0.004,
   -0.004, -0.004},
  {// charge = 144 fC
   0.000,  -0.000, 0.006,  0.024,  0.051,  0.085,  0.123,  0.163,  0.204,
   0.245,  0.285,  0.324,  0.361,  0.397,  0.430,  0.462,  0.491,  0.519,
   0.544,  0.567,  0.588,  0.607,  0.625,  0.640,  0.654,  0.665,  0.676,
   0.684,  0.691,  0.697,  0.701,  0.704,  0.705,  0.706,  0.705,  0.703,
   0.701,  0.697,  0.692,  0.687,  0.681,  0.674,  0.667,  0.659,  0.650,
   0.641,  0.631,  0.621,  0.611,  0.600,  0.589,  0.578,  0.566,  0.555,
   0.543,  0.531,  0.519,  0.506,  0.494,  0.482,  0.470,  0.457,  0.445,
   0.433,  0.421,  0.409,  0.397,  0.385,  0.373,  0.362,  0.350,  0.339,
   0.328,  0.317,  0.306,  0.295,  0.285,  0.275,  0.265,  0.255,  0.246,
   0.236,  0.227,  0.218,  0.210,  0.201,  0.193,  0.185,  0.177,  0.169,
   0.162,  0.155,  0.148,  0.141,  0.134,  0.128,  0.122,  0.116,  0.110,
   0.105,  0.099,  0.094,  0.089,  0.084,  0.080,  0.075,  0.071,  0.067,
   0.063,  0.059,  0.055,  0.052,  0.049,  0.045,  0.042,  0.039,  0.037,
   0.034,  0.031,  0.029,  0.027,  0.025,  0.022,  0.020,  0.019,  0.017,
   0.015,  0.013,  0.012,  0.011,  0.009,  0.008,  0.007,  0.006,  0.004,
   0.003,  0.002,  0.002,  0.001,  0.000,  -0.001, -0.001, -0.002, -0.002,
   -0.003, -0.003, -0.004, -0.004, -0.005, -0.005, -0.005, -0.006, -0.006,
   -0.006, -0.006, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.005, -0.005, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.004, -0.004, -0.004,
   -0.004, -0.004},
  {// charge = 150 fC
   0.000,  -0.000, 0.006,  0.025,  0.053,  0.088,  0.127,  0.169,  0.212,
   0.255,  0.296,  0.337,  0.376,  0.413,  0.448,  0.481,  0.512,  0.540,
   0.566,  0.591,  0.613,  0.633,  0.650,  0.666,  0.681,  0.693,  0.704,
   0.712,  0.720,  0.726,  0.730,  0.733,  0.735,  0.735,  0.734,  0.733,
   0.730,  0.726,  0.721,  0.716,  0.709,  0.702,  0.695,  0.686,  0.677,
   0.668,  0.658,  0.647,  0.637,  0.625,  0.614,  0.602,  0.590,  0.578,
   0.566,  0.553,  0.540,  0.528,  0.515,  0.502,  0.489,  0.476,  0.464,
   0.451,  0.438,  0.426,  0.413,  0.401,  0.389,  0.377,  0.365,  0.353,
   0.341,  0.330,  0.319,  0.308,  0.297,  0.286,  0.276,  0.266,  0.256,
   0.246,  0.237,  0.227,  0.218,  0.209,  0.201,  0.192,  0.184,  0.176,
   0.169,  0.161,  0.154,  0.147,  0.140,  0.133,  0.127,  0.121,  0.115,
   0.110,  0.103,  0.098,  0.093,  0.088,  0.083,  0.078,  0.074,  0.070,
   0.066,  0.062,  0.058,  0.054,  0.051,  0.047,  0.044,  0.041,  0.038,
   0.035,  0.033,  0.030,  0.028,  0.026,  0.023,  0.021,  0.019,  0.017,
   0.016,  0.014,  0.012,  0.011,  0.009,  0.008,  0.007,  0.006,  0.005,
   0.004,  0.003,  0.002,  0.001,  0.000,  -0.001, -0.001, -0.002, -0.003,
   -0.003, -0.004, -0.004, -0.005, -0.005, -0.005, -0.006, -0.006, -0.006,
   -0.006, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.005,
   -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.004,
   -0.004, -0.004},
  {// charge = 156 fC
   0.000,  -0.001, 0.006,  0.025,  0.055,  0.091,  0.132,  0.176,  0.220,
   0.264,  0.308,  0.350,  0.390,  0.429,  0.465,  0.500,  0.532,  0.561,
   0.589,  0.614,  0.637,  0.658,  0.676,  0.693,  0.708,  0.721,  0.732,
   0.741,  0.749,  0.755,  0.759,  0.762,  0.764,  0.765,  0.764,  0.762,
   0.759,  0.755,  0.750,  0.744,  0.738,  0.731,  0.722,  0.714,  0.704,
   0.695,  0.684,  0.673,  0.662,  0.651,  0.639,  0.626,  0.614,  0.601,
   0.588,  0.575,  0.562,  0.549,  0.536,  0.522,  0.509,  0.496,  0.482,
   0.469,  0.456,  0.443,  0.430,  0.417,  0.404,  0.392,  0.379,  0.367,
   0.355,  0.343,  0.332,  0.320,  0.309,  0.298,  0.287,  0.276,  0.266,
   0.256,  0.246,  0.237,  0.227,  0.218,  0.209,  0.200,  0.192,  0.183,
   0.175,  0.168,  0.160,  0.153,  0.146,  0.139,  0.132,  0.126,  0.119,
   0.114,  0.107,  0.102,  0.097,  0.091,  0.086,  0.082,  0.077,  0.072,
   0.068,  0.064,  0.060,  0.056,  0.053,  0.049,  0.046,  0.043,  0.040,
   0.037,  0.034,  0.031,  0.029,  0.027,  0.024,  0.022,  0.020,  0.018,
   0.016,  0.015,  0.013,  0.011,  0.010,  0.008,  0.007,  0.006,  0.005,
   0.004,  0.003,  0.002,  0.001,  0.000,  -0.001, -0.002, -0.002, -0.003,
   -0.003, -0.004, -0.004, -0.005, -0.005, -0.006, -0.006, -0.006, -0.006,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.008, -0.008,
   -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.007,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.004, -0.004},
  {// charge = 162 fC
   0.000,  -0.001, 0.006,  0.026,  0.057,  0.095,  0.137,  0.182,  0.228,
   0.274,  0.319,  0.363,  0.405,  0.445,  0.483,  0.518,  0.552,  0.582,
   0.611,  0.637,  0.661,  0.683,  0.702,  0.719,  0.735,  0.748,  0.760,
   0.769,  0.777,  0.784,  0.788,  0.792,  0.793,  0.794,  0.793,  0.791,
   0.788,  0.784,  0.779,  0.773,  0.766,  0.759,  0.750,  0.741,  0.732,
   0.721,  0.711,  0.699,  0.688,  0.676,  0.663,  0.651,  0.638,  0.624,
   0.611,  0.598,  0.584,  0.570,  0.556,  0.543,  0.529,  0.515,  0.501,
   0.487,  0.474,  0.460,  0.447,  0.433,  0.420,  0.407,  0.394,  0.382,
   0.369,  0.357,  0.345,  0.333,  0.321,  0.309,  0.298,  0.287,  0.276,
   0.266,  0.256,  0.246,  0.236,  0.226,  0.217,  0.208,  0.199,  0.191,
   0.182,  0.174,  0.166,  0.159,  0.151,  0.144,  0.137,  0.131,  0.124,
   0.119,  0.112,  0.106,  0.100,  0.095,  0.090,  0.085,  0.080,  0.075,
   0.071,  0.067,  0.062,  0.059,  0.055,  0.051,  0.048,  0.044,  0.041,
   0.038,  0.035,  0.033,  0.030,  0.028,  0.025,  0.023,  0.021,  0.019,
   0.017,  0.015,  0.013,  0.012,  0.010,  0.009,  0.007,  0.006,  0.005,
   0.004,  0.003,  0.002,  0.001,  0.000,  -0.001, -0.002, -0.002, -0.003,
   -0.003, -0.004, -0.005, -0.005, -0.005, -0.006, -0.006, -0.006, -0.007,
   -0.007, -0.007, -0.007, -0.007, -0.008, -0.008, -0.008, -0.008, -0.008,
   -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008,
   -0.008, -0.008, -0.008, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.007, -0.007, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005},
  {// charge = 168 fC
   0.000,  -0.001, 0.006,  0.027,  0.059,  0.098,  0.142,  0.188,  0.236,
   0.284,  0.330,  0.376,  0.419,  0.461,  0.500,  0.537,  0.572,  0.604,
   0.633,  0.660,  0.685,  0.708,  0.728,  0.746,  0.762,  0.776,  0.788,
   0.798,  0.806,  0.812,  0.817,  0.821,  0.823,  0.823,  0.823,  0.821,
   0.818,  0.813,  0.808,  0.802,  0.795,  0.787,  0.778,  0.769,  0.759,
   0.748,  0.737,  0.725,  0.713,  0.701,  0.688,  0.675,  0.661,  0.648,
   0.634,  0.620,  0.606,  0.592,  0.577,  0.563,  0.549,  0.534,  0.520,
   0.506,  0.491,  0.477,  0.463,  0.450,  0.436,  0.422,  0.409,  0.396,
   0.383,  0.370,  0.357,  0.345,  0.333,  0.321,  0.309,  0.298,  0.287,
   0.276,  0.265,  0.255,  0.245,  0.235,  0.225,  0.216,  0.207,  0.198,
   0.189,  0.181,  0.173,  0.165,  0.157,  0.150,  0.142,  0.136,  0.129,
   0.123,  0.116,  0.110,  0.104,  0.099,  0.093,  0.088,  0.083,  0.078,
   0.074,  0.069,  0.065,  0.061,  0.057,  0.053,  0.050,  0.046,  0.043,
   0.040,  0.037,  0.034,  0.031,  0.029,  0.026,  0.024,  0.022,  0.020,
   0.018,  0.016,  0.014,  0.012,  0.011,  0.009,  0.008,  0.006,  0.005,
   0.004,  0.003,  0.002,  0.001,  0.000,  -0.001, -0.002, -0.002, -0.003,
   -0.004, -0.004, -0.005, -0.005, -0.006, -0.006, -0.006, -0.007, -0.007,
   -0.007, -0.007, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008,
   -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008,
   -0.008, -0.008, -0.008, -0.008, -0.008, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006, -0.006, -0.006, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.005},
  {// charge = 180 fC
   0.000,  -0.001, 0.007,  0.029,  0.062,  0.104,  0.151,  0.201,  0.251,
   0.302,  0.353,  0.401,  0.448,  0.493,  0.535,  0.574,  0.611,  0.646,
   0.677,  0.707,  0.733,  0.757,  0.779,  0.798,  0.816,  0.831,  0.843,
   0.854,  0.863,  0.870,  0.876,  0.879,  0.882,  0.882,  0.881,  0.879,
   0.876,  0.872,  0.866,  0.859,  0.852,  0.843,  0.834,  0.824,  0.813,
   0.802,  0.790,  0.778,  0.765,  0.751,  0.738,  0.723,  0.709,  0.694,
   0.680,  0.665,  0.649,  0.634,  0.619,  0.603,  0.588,  0.573,  0.557,
   0.542,  0.527,  0.512,  0.497,  0.482,  0.467,  0.453,  0.439,  0.424,
   0.410,  0.397,  0.383,  0.370,  0.357,  0.344,  0.332,  0.320,  0.308,
   0.296,  0.284,  0.273,  0.262,  0.252,  0.241,  0.231,  0.221,  0.212,
   0.203,  0.194,  0.185,  0.177,  0.168,  0.160,  0.153,  0.145,  0.138,
   0.132,  0.124,  0.118,  0.112,  0.106,  0.100,  0.094,  0.089,  0.084,
   0.079,  0.074,  0.070,  0.065,  0.061,  0.057,  0.053,  0.049,  0.046,
   0.043,  0.039,  0.036,  0.034,  0.031,  0.028,  0.026,  0.023,  0.021,
   0.019,  0.017,  0.015,  0.013,  0.011,  0.010,  0.008,  0.007,  0.006,
   0.004,  0.003,  0.002,  0.001,  0.000,  -0.001, -0.002, -0.002, -0.003,
   -0.004, -0.005, -0.005, -0.006, -0.006, -0.006, -0.007, -0.007, -0.007,
   -0.008, -0.008, -0.008, -0.008, -0.008, -0.009, -0.009, -0.009, -0.009,
   -0.009, -0.009, -0.009, -0.009, -0.009, -0.009, -0.009, -0.009, -0.009,
   -0.009, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008,
   -0.008, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007,
   -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.006, -0.005, -0.005,
   -0.005, -0.005},
  {// charge = 195 fC
   0.000,  -0.001, 0.007,  0.031,  0.067,  0.112,  0.162,  0.216,  0.271,
   0.326,  0.380,  0.433,  0.484,  0.532,  0.578,  0.621,  0.661,  0.698,
   0.733,  0.764,  0.793,  0.819,  0.843,  0.864,  0.883,  0.899,  0.913,
   0.925,  0.935,  0.943,  0.948,  0.953,  0.955,  0.956,  0.955,  0.953,
   0.949,  0.944,  0.938,  0.931,  0.923,  0.914,  0.904,  0.893,  0.882,
   0.869,  0.856,  0.843,  0.829,  0.814,  0.800,  0.784,  0.769,  0.753,
   0.737,  0.721,  0.704,  0.688,  0.671,  0.654,  0.638,  0.621,  0.604,
   0.588,  0.571,  0.555,  0.539,  0.523,  0.507,  0.491,  0.476,  0.460,
   0.445,  0.430,  0.416,  0.401,  0.387,  0.373,  0.360,  0.347,  0.334,
   0.321,  0.308,  0.296,  0.285,  0.273,  0.262,  0.251,  0.240,  0.230,
   0.220,  0.210,  0.201,  0.192,  0.183,  0.174,  0.166,  0.158,  0.150,
   0.143,  0.135,  0.128,  0.121,  0.115,  0.108,  0.102,  0.096,  0.091,
   0.086,  0.080,  0.076,  0.071,  0.066,  0.062,  0.058,  0.054,  0.050,
   0.046,  0.043,  0.039,  0.036,  0.033,  0.030,  0.028,  0.025,  0.023,
   0.020,  0.018,  0.016,  0.014,  0.012,  0.011,  0.009,  0.007,  0.006,
   0.005,  0.003,  0.002,  0.001,  0.000,  -0.001, -0.002, -0.003, -0.003,
   -0.004, -0.005, -0.006, -0.006, -0.007, -0.007, -0.007, -0.008, -0.008,
   -0.008, -0.009, -0.009, -0.009, -0.009, -0.009, -0.010, -0.010, -0.010,
   -0.010, -0.010, -0.010, -0.010, -0.010, -0.010, -0.010, -0.010, -0.010,
   -0.009, -0.009, -0.009, -0.009, -0.009, -0.009, -0.009, -0.009, -0.008,
   -0.008, -0.008, -0.008, -0.008, -0.008, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.007, -0.007, -0.007, -0.006, -0.006, -0.006, -0.006, -0.006,
   -0.006, -0.006},
  {// charge = 210 fC
   0.000,  -0.001, 0.008,  0.033,  0.071,  0.119,  0.173,  0.230,  0.290,
   0.349,  0.407,  0.464,  0.519,  0.571,  0.620,  0.666,  0.710,  0.750,
   0.787,  0.821,  0.853,  0.881,  0.907,  0.930,  0.950,  0.967,  0.983,
   0.996,  1.006,  1.015,  1.021,  1.026,  1.028,  1.029,  1.028,  1.026,
   1.022,  1.017,  1.011,  1.003,  0.995,  0.985,  0.974,  0.962,  0.950,
   0.937,  0.923,  0.908,  0.893,  0.878,  0.862,  0.845,  0.828,  0.811,
   0.794,  0.777,  0.759,  0.741,  0.723,  0.705,  0.687,  0.669,  0.652,
   0.634,  0.616,  0.598,  0.581,  0.564,  0.546,  0.529,  0.513,  0.496,
   0.480,  0.464,  0.448,  0.433,  0.418,  0.403,  0.388,  0.374,  0.360,
   0.346,  0.333,  0.320,  0.307,  0.294,  0.282,  0.270,  0.259,  0.248,
   0.237,  0.227,  0.216,  0.206,  0.197,  0.188,  0.179,  0.170,  0.161,
   0.154,  0.145,  0.138,  0.131,  0.124,  0.117,  0.110,  0.104,  0.098,
   0.092,  0.087,  0.081,  0.076,  0.071,  0.067,  0.062,  0.058,  0.054,
   0.050,  0.046,  0.043,  0.039,  0.036,  0.033,  0.030,  0.027,  0.025,
   0.022,  0.020,  0.017,  0.015,  0.013,  0.011,  0.010,  0.008,  0.006,
   0.005,  0.004,  0.002,  0.001,  0.000,  -0.001, -0.002, -0.003, -0.004,
   -0.005, -0.005, -0.006, -0.007, -0.007, -0.007, -0.008, -0.008, -0.009,
   -0.009, -0.009, -0.010, -0.010, -0.010, -0.010, -0.010, -0.010, -0.010,
   -0.011, -0.011, -0.011, -0.011, -0.011, -0.010, -0.010, -0.010, -0.010,
   -0.010, -0.010, -0.010, -0.010, -0.010, -0.010, -0.009, -0.009, -0.009,
   -0.009, -0.009, -0.009, -0.008, -0.008, -0.008, -0.008, -0.008, -0.008,
   -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007, -0.006, -0.006,
   -0.006, -0.006},
  {// charge = 225 fC
   0.000,  -0.001, 0.008,  0.035,  0.075,  0.126,  0.183,  0.245,  0.308,
   0.371,  0.434,  0.495,  0.553,  0.609,  0.662,  0.712,  0.758,  0.801,
   0.842,  0.878,  0.912,  0.943,  0.970,  0.995,  1.017,  1.036,  1.052,
   1.066,  1.077,  1.087,  1.094,  1.098,  1.101,  1.102,  1.102,  1.099,
   1.095,  1.090,  1.083,  1.075,  1.066,  1.055,  1.044,  1.032,  1.018,
   1.004,  0.989,  0.974,  0.958,  0.941,  0.924,  0.906,  0.888,  0.870,
   0.852,  0.833,  0.814,  0.795,  0.776,  0.756,  0.737,  0.718,  0.699,
   0.680,  0.661,  0.642,  0.623,  0.605,  0.586,  0.568,  0.550,  0.532,
   0.515,  0.498,  0.481,  0.464,  0.448,  0.432,  0.416,  0.401,  0.386,
   0.371,  0.357,  0.343,  0.329,  0.316,  0.303,  0.290,  0.278,  0.266,
   0.254,  0.243,  0.232,  0.222,  0.211,  0.201,  0.192,  0.183,  0.173,
   0.165,  0.156,  0.148,  0.140,  0.133,  0.126,  0.118,  0.112,  0.105,
   0.099,  0.093,  0.087,  0.082,  0.077,  0.072,  0.067,  0.062,  0.058,
   0.053,  0.049,  0.046,  0.042,  0.039,  0.035,  0.032,  0.029,  0.026,
   0.024,  0.021,  0.019,  0.016,  0.014,  0.012,  0.010,  0.009,  0.007,
   0.005,  0.004,  0.002,  0.001,  0.000,  -0.001, -0.002, -0.003, -0.004,
   -0.005, -0.006, -0.006, -0.007, -0.008, -0.008, -0.009, -0.009, -0.009,
   -0.010, -0.010, -0.010, -0.011, -0.011, -0.011, -0.011, -0.011, -0.011,
   -0.011, -0.011, -0.011, -0.011, -0.011, -0.011, -0.011, -0.011, -0.011,
   -0.011, -0.011, -0.011, -0.011, -0.010, -0.010, -0.010, -0.010, -0.010,
   -0.010, -0.010, -0.009, -0.009, -0.009, -0.009, -0.009, -0.008, -0.008,
   -0.008, -0.008, -0.008, -0.007, -0.007, -0.007, -0.007, -0.007, -0.007,
   -0.007, -0.006},
  {// charge = 240 fC
   0.000,  -0.001, 0.009,  0.036,  0.079,  0.132,  0.193,  0.258,  0.325,
   0.393,  0.460,  0.524,  0.587,  0.647,  0.703,  0.756,  0.806,  0.852,
   0.895,  0.935,  0.971,  1.004,  1.033,  1.060,  1.083,  1.103,  1.121,
   1.136,  1.148,  1.158,  1.166,  1.171,  1.174,  1.175,  1.175,  1.172,
   1.168,  1.162,  1.155,  1.147,  1.137,  1.126,  1.114,  1.101,  1.087,
   1.072,  1.056,  1.039,  1.022,  1.004,  0.986,  0.968,  0.948,  0.929,
   0.909,  0.889,  0.869,  0.849,  0.828,  0.808,  0.788,  0.767,  0.746,
   0.726,  0.706,  0.686,  0.666,  0.646,  0.626,  0.607,  0.588,  0.569,
   0.550,  0.532,  0.514,  0.496,  0.479,  0.462,  0.445,  0.428,  0.412,
   0.397,  0.381,  0.366,  0.352,  0.338,  0.324,  0.310,  0.297,  0.284,
   0.272,  0.260,  0.248,  0.237,  0.226,  0.215,  0.205,  0.196,  0.185,
   0.176,  0.167,  0.158,  0.150,  0.142,  0.134,  0.127,  0.119,  0.113,
   0.106,  0.100,  0.093,  0.088,  0.082,  0.077,  0.071,  0.067,  0.062,
   0.057,  0.053,  0.049,  0.045,  0.041,  0.038,  0.034,  0.031,  0.028,
   0.025,  0.023,  0.020,  0.018,  0.015,  0.013,  0.011,  0.009,  0.007,
   0.006,  0.004,  0.003,  0.001,  0.000,  -0.001, -0.002, -0.003, -0.004,
   -0.005, -0.006, -0.007, -0.007, -0.008, -0.009, -0.009, -0.010, -0.010,
   -0.010, -0.011, -0.011, -0.011, -0.012, -0.012, -0.012, -0.012, -0.012,
   -0.012, -0.012, -0.012, -0.012, -0.012, -0.012, -0.012, -0.012, -0.012,
   -0.012, -0.012, -0.011, -0.011, -0.011, -0.011, -0.011, -0.011, -0.011,
   -0.010, -0.010, -0.010, -0.010, -0.010, -0.009, -0.009, -0.009, -0.009,
   -0.009, -0.008, -0.008, -0.008, -0.008, -0.008, -0.007, -0.007, -0.007,
   -0.007, -0.007},
  {// charge = 255 fC
   0.000,  -0.001, 0.009,  0.038,  0.082,  0.139,  0.203,  0.271,  0.343,
   0.414,  0.485,  0.554,  0.620,  0.683,  0.744,  0.800,  0.853,  0.903,
   0.949,  0.991,  1.029,  1.064,  1.096,  1.124,  1.149,  1.170,  1.189,
   1.204,  1.217,  1.227,  1.234,  1.240,  1.243,  1.244,  1.243,  1.241,
   1.237,  1.231,  1.224,  1.215,  1.205,  1.194,  1.182,  1.168,  1.153,
   1.138,  1.121,  1.104,  1.086,  1.067,  1.048,  1.028,  1.008,  0.988,
   0.967,  0.946,  0.924,  0.903,  0.881,  0.860,  0.838,  0.816,  0.795,
   0.773,  0.751,  0.730,  0.709,  0.688,  0.667,  0.646,  0.626,  0.606,
   0.586,  0.567,  0.548,  0.529,  0.510,  0.492,  0.474,  0.457,  0.440,
   0.423,  0.407,  0.391,  0.375,  0.360,  0.346,  0.331,  0.317,  0.304,
   0.290,  0.277,  0.265,  0.253,  0.241,  0.230,  0.219,  0.209,  0.198,
   0.188,  0.178,  0.169,  0.160,  0.152,  0.143,  0.136,  0.128,  0.120,
   0.113,  0.107,  0.100,  0.094,  0.088,  0.082,  0.076,  0.071,  0.066,
   0.061,  0.057,  0.053,  0.048,  0.044,  0.041,  0.037,  0.034,  0.030,
   0.027,  0.024,  0.022,  0.019,  0.017,  0.014,  0.012,  0.010,  0.008,
   0.006,  0.005,  0.003,  0.002,  0.000,  -0.001, -0.002, -0.003, -0.005,
   -0.006, -0.006, -0.007, -0.008, -0.009, -0.009, -0.010, -0.010, -0.011,
   -0.011, -0.011, -0.012, -0.012, -0.012, -0.012, -0.013, -0.013, -0.013,
   -0.013, -0.013, -0.013, -0.013, -0.013, -0.013, -0.013, -0.013, -0.013,
   -0.012, -0.012, -0.012, -0.012, -0.012, -0.012, -0.012, -0.011, -0.011,
   -0.011, -0.011, -0.011, -0.011, -0.010, -0.010, -0.010, -0.010, -0.010,
   -0.009, -0.009, -0.009, -0.009, -0.008, -0.008, -0.008, -0.008, -0.008,
   -0.007, -0.007},
  {// charge = 270 fC
   0.000,  -0.001, 0.010,  0.039,  0.086,  0.145,  0.212,  0.284,  0.359,
   0.434,  0.509,  0.582,  0.652,  0.720,  0.783,  0.844,  0.900,  0.953,
   1.001,  1.046,  1.087,  1.124,  1.158,  1.187,  1.212,  1.234,  1.252,
   1.267,  1.279,  1.289,  1.295,  1.300,  1.302,  1.303,  1.302,  1.299,
   1.295,  1.289,  1.282,  1.274,  1.264,  1.253,  1.241,  1.228,  1.213,
   1.198,  1.181,  1.164,  1.146,  1.127,  1.107,  1.087,  1.066,  1.045,
   1.023,  1.001,  0.979,  0.957,  0.934,  0.911,  0.889,  0.866,  0.843,
   0.821,  0.798,  0.776,  0.753,  0.731,  0.709,  0.688,  0.666,  0.645,
   0.624,  0.604,  0.584,  0.564,  0.544,  0.525,  0.506,  0.488,  0.470,
   0.452,  0.435,  0.418,  0.401,  0.385,  0.370,  0.354,  0.340,  0.325,
   0.311,  0.297,  0.284,  0.271,  0.259,  0.247,  0.235,  0.225,  0.213,
   0.202,  0.192,  0.182,  0.173,  0.164,  0.155,  0.146,  0.138,  0.130,
   0.123,  0.115,  0.108,  0.102,  0.095,  0.089,  0.083,  0.077,  0.072,
   0.067,  0.062,  0.057,  0.053,  0.049,  0.044,  0.041,  0.037,  0.033,
   0.030,  0.027,  0.024,  0.021,  0.019,  0.016,  0.014,  0.012,  0.009,
   0.007,  0.006,  0.004,  0.002,  0.001,  -0.001, -0.002, -0.003, -0.004,
   -0.005, -0.006, -0.007, -0.008, -0.009, -0.009, -0.010, -0.011, -0.011,
   -0.012, -0.012, -0.012, -0.012, -0.013, -0.013, -0.013, -0.013, -0.013,
   -0.013, -0.014, -0.014, -0.014, -0.014, -0.014, -0.013, -0.013, -0.013,
   -0.013, -0.013, -0.013, -0.013, -0.013, -0.012, -0.012, -0.012, -0.012,
   -0.012, -0.012, -0.011, -0.011, -0.011, -0.011, -0.011, -0.010, -0.010,
   -0.010, -0.010, -0.010, -0.009, -0.009, -0.009, -0.009, -0.008, -0.008,
   -0.008, -0.008},
  {// charge = 285 fC
   0.000,  -0.001, 0.010,  0.041,  0.089,  0.150,  0.220,  0.296,  0.374,
   0.454,  0.533,  0.609,  0.684,  0.755,  0.822,  0.886,  0.946,  1.002,
   1.053,  1.101,  1.144,  1.183,  1.217,  1.246,  1.271,  1.291,  1.308,
   1.321,  1.331,  1.338,  1.343,  1.346,  1.348,  1.347,  1.346,  1.342,
   1.338,  1.332,  1.326,  1.318,  1.309,  1.299,  1.287,  1.275,  1.261,
   1.247,  1.231,  1.214,  1.197,  1.179,  1.159,  1.139,  1.119,  1.097,
   1.076,  1.053,  1.031,  1.008,  0.985,  0.962,  0.939,  0.916,  0.893,
   0.869,  0.846,  0.823,  0.800,  0.777,  0.754,  0.732,  0.709,  0.687,
   0.666,  0.644,  0.623,  0.602,  0.582,  0.562,  0.542,  0.523,  0.504,
   0.485,  0.467,  0.449,  0.432,  0.415,  0.398,  0.382,  0.366,  0.351,
   0.336,  0.321,  0.307,  0.294,  0.280,  0.268,  0.255,  0.244,  0.231,
   0.220,  0.209,  0.199,  0.188,  0.179,  0.169,  0.160,  0.151,  0.143,
   0.135,  0.127,  0.119,  0.112,  0.105,  0.098,  0.092,  0.086,  0.080,
   0.075,  0.069,  0.064,  0.059,  0.055,  0.050,  0.046,  0.042,  0.038,
   0.035,  0.031,  0.028,  0.025,  0.022,  0.019,  0.017,  0.014,  0.012,
   0.010,  0.008,  0.006,  0.004,  0.002,  0.001,  -0.001, -0.002, -0.003,
   -0.004, -0.006, -0.007, -0.007, -0.008, -0.009, -0.010, -0.010, -0.011,
   -0.012, -0.012, -0.012, -0.013, -0.013, -0.013, -0.013, -0.014, -0.014,
   -0.014, -0.014, -0.014, -0.014, -0.014, -0.014, -0.014, -0.014, -0.014,
   -0.014, -0.014, -0.014, -0.013, -0.013, -0.013, -0.013, -0.013, -0.013,
   -0.012, -0.012, -0.012, -0.012, -0.012, -0.011, -0.011, -0.011, -0.011,
   -0.011, -0.010, -0.010, -0.010, -0.010, -0.010, -0.009, -0.009, -0.009,
   -0.009, -0.008},
  {// charge = 300 fC
   0.000,  -0.001, 0.010,  0.042,  0.092,  0.155,  0.228,  0.307,  0.389,
   0.472,  0.555,  0.636,  0.714,  0.789,  0.860,  0.928,  0.991,  1.050,
   1.104,  1.154,  1.199,  1.238,  1.271,  1.298,  1.320,  1.338,  1.352,
   1.362,  1.369,  1.374,  1.377,  1.379,  1.379,  1.378,  1.375,  1.372,
   1.368,  1.363,  1.356,  1.349,  1.341,  1.332,  1.322,  1.311,  1.299,
   1.285,  1.271,  1.256,  1.240,  1.223,  1.205,  1.186,  1.166,  1.146,
   1.125,  1.103,  1.081,  1.058,  1.035,  1.012,  0.989,  0.965,  0.942,
   0.918,  0.895,  0.871,  0.848,  0.824,  0.801,  0.778,  0.755,  0.732,
   0.710,  0.688,  0.666,  0.644,  0.623,  0.602,  0.581,  0.561,  0.541,
   0.522,  0.503,  0.484,  0.466,  0.448,  0.430,  0.413,  0.397,  0.381,
   0.365,  0.349,  0.334,  0.320,  0.306,  0.292,  0.279,  0.266,  0.254,
   0.242,  0.230,  0.218,  0.208,  0.197,  0.187,  0.177,  0.168,  0.158,
   0.150,  0.141,  0.133,  0.125,  0.118,  0.110,  0.104,  0.097,  0.091,
   0.085,  0.079,  0.073,  0.068,  0.063,  0.058,  0.053,  0.049,  0.045,
   0.041,  0.037,  0.034,  0.030,  0.027,  0.024,  0.021,  0.018,  0.016,
   0.013,  0.011,  0.009,  0.007,  0.005,  0.003,  0.002,  0.000,  -0.001,
   -0.003, -0.004, -0.005, -0.006, -0.007, -0.008, -0.009, -0.010, -0.010,
   -0.011, -0.012, -0.012, -0.012, -0.013, -0.013, -0.013, -0.014, -0.014,
   -0.014, -0.014, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015,
   -0.015, -0.014, -0.014, -0.014, -0.014, -0.014, -0.014, -0.014, -0.013,
   -0.013, -0.013, -0.013, -0.013, -0.012, -0.012, -0.012, -0.012, -0.012,
   -0.011, -0.011, -0.011, -0.011, -0.010, -0.010, -0.010, -0.010, -0.010,
   -0.009, -0.009},
  {// charge = 315 fC
   0.000,  -0.001, 0.011,  0.043,  0.094,  0.159,  0.235,  0.317,  0.403,
   0.490,  0.576,  0.661,  0.743,  0.822,  0.897,  0.968,  1.035,  1.097,
   1.154,  1.205,  1.249,  1.287,  1.317,  1.341,  1.359,  1.373,  1.383,
   1.390,  1.395,  1.398,  1.400,  1.400,  1.400,  1.398,  1.396,  1.393,
   1.389,  1.384,  1.378,  1.372,  1.365,  1.357,  1.348,  1.339,  1.328,
   1.317,  1.304,  1.291,  1.276,  1.261,  1.244,  1.227,  1.209,  1.190,
   1.170,  1.149,  1.128,  1.106,  1.084,  1.061,  1.038,  1.015,  0.992,
   0.968,  0.944,  0.921,  0.897,  0.873,  0.850,  0.826,  0.803,  0.779,
   0.756,  0.734,  0.711,  0.689,  0.667,  0.645,  0.624,  0.603,  0.582,
   0.562,  0.542,  0.522,  0.503,  0.485,  0.466,  0.448,  0.431,  0.414,
   0.397,  0.381,  0.365,  0.350,  0.335,  0.320,  0.306,  0.292,  0.279,
   0.267,  0.253,  0.241,  0.230,  0.218,  0.207,  0.197,  0.187,  0.177,
   0.167,  0.158,  0.149,  0.141,  0.133,  0.125,  0.117,  0.110,  0.103,
   0.097,  0.090,  0.084,  0.078,  0.073,  0.067,  0.062,  0.058,  0.053,
   0.049,  0.044,  0.040,  0.037,  0.033,  0.030,  0.026,  0.023,  0.021,
   0.018,  0.015,  0.013,  0.011,  0.008,  0.006,  0.004,  0.003,  0.001,
   -0.000, -0.002, -0.003, -0.004, -0.006, -0.007, -0.008, -0.008, -0.009,
   -0.010, -0.011, -0.011, -0.012, -0.012, -0.013, -0.013, -0.014, -0.014,
   -0.014, -0.014, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015,
   -0.015, -0.015, -0.015, -0.015, -0.015, -0.015, -0.014, -0.014, -0.014,
   -0.014, -0.014, -0.013, -0.013, -0.013, -0.013, -0.013, -0.012, -0.012,
   -0.012, -0.012, -0.012, -0.011, -0.011, -0.011, -0.011, -0.011, -0.010,
   -0.010, -0.010},
  {// charge = 330 fC
   0.000,  -0.001, 0.011,  0.044,  0.096,  0.163,  0.241,  0.326,  0.415,
   0.506,  0.596,  0.685,  0.771,  0.854,  0.933,  1.007,  1.077,  1.142,
   1.201,  1.252,  1.294,  1.328,  1.353,  1.373,  1.387,  1.397,  1.404,
   1.409,  1.412,  1.414,  1.414,  1.414,  1.414,  1.412,  1.410,  1.407,
   1.404,  1.400,  1.395,  1.389,  1.383,  1.377,  1.369,  1.361,  1.352,
   1.342,  1.331,  1.320,  1.307,  1.294,  1.279,  1.263,  1.247,  1.230,
   1.211,  1.192,  1.173,  1.152,  1.131,  1.109,  1.086,  1.064,  1.041,
   1.017,  0.994,  0.970,  0.947,  0.923,  0.899,  0.875,  0.852,  0.828,
   0.805,  0.782,  0.758,  0.736,  0.713,  0.691,  0.669,  0.647,  0.626,
   0.605,  0.584,  0.563,  0.544,  0.524,  0.505,  0.486,  0.468,  0.450,
   0.432,  0.415,  0.398,  0.382,  0.366,  0.351,  0.336,  0.321,  0.307,
   0.294,  0.279,  0.267,  0.254,  0.242,  0.230,  0.219,  0.208,  0.197,
   0.187,  0.177,  0.168,  0.159,  0.150,  0.141,  0.133,  0.125,  0.118,
   0.110,  0.104,  0.097,  0.090,  0.084,  0.078,  0.073,  0.068,  0.062,
   0.058,  0.053,  0.049,  0.044,  0.040,  0.037,  0.033,  0.030,  0.026,
   0.023,  0.020,  0.018,  0.015,  0.013,  0.010,  0.008,  0.006,  0.004,
   0.002,  0.001,  -0.001, -0.002, -0.003, -0.005, -0.006, -0.007, -0.008,
   -0.009, -0.010, -0.010, -0.011, -0.012, -0.012, -0.013, -0.013, -0.014,
   -0.014, -0.014, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015,
   -0.015, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015,
   -0.015, -0.014, -0.014, -0.014, -0.014, -0.014, -0.013, -0.013, -0.013,
   -0.013, -0.013, -0.012, -0.012, -0.012, -0.012, -0.012, -0.011, -0.011,
   -0.011, -0.011},
  {// charge = 345 fC
   0.000,  -0.001, 0.012,  0.045,  0.098,  0.167,  0.247,  0.335,  0.427,
   0.521,  0.615,  0.708,  0.797,  0.884,  0.967,  1.045,  1.118,  1.186,
   1.244,  1.293,  1.331,  1.360,  1.380,  1.395,  1.405,  1.412,  1.417,
   1.420,  1.422,  1.423,  1.424,  1.424,  1.423,  1.422,  1.420,  1.417,
   1.415,  1.411,  1.407,  1.403,  1.398,  1.392,  1.386,  1.379,  1.371,
   1.363,  1.354,  1.344,  1.333,  1.322,  1.309,  1.296,  1.281,  1.266,
   1.249,  1.232,  1.214,  1.195,  1.175,  1.155,  1.133,  1.111,  1.089,
   1.066,  1.043,  1.020,  0.997,  0.973,  0.949,  0.926,  0.902,  0.878,
   0.854,  0.831,  0.807,  0.784,  0.761,  0.738,  0.716,  0.693,  0.671,
   0.649,  0.628,  0.607,  0.586,  0.566,  0.546,  0.526,  0.507,  0.488,
   0.470,  0.452,  0.434,  0.417,  0.400,  0.384,  0.368,  0.352,  0.337,
   0.323,  0.308,  0.294,  0.281,  0.268,  0.255,  0.243,  0.231,  0.220,
   0.209,  0.198,  0.188,  0.178,  0.169,  0.159,  0.150,  0.142,  0.134,
   0.126,  0.118,  0.111,  0.104,  0.097,  0.091,  0.085,  0.079,  0.073,
   0.068,  0.063,  0.058,  0.053,  0.049,  0.044,  0.040,  0.037,  0.033,
   0.030,  0.026,  0.023,  0.020,  0.018,  0.015,  0.013,  0.010,  0.008,
   0.006,  0.004,  0.002,  0.001,  -0.001, -0.002, -0.004, -0.005, -0.006,
   -0.007, -0.008, -0.009, -0.010, -0.011, -0.011, -0.012, -0.012, -0.013,
   -0.013, -0.014, -0.014, -0.015, -0.015, -0.015, -0.015, -0.015, -0.016,
   -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.015, -0.015,
   -0.015, -0.015, -0.015, -0.015, -0.015, -0.014, -0.014, -0.014, -0.014,
   -0.014, -0.013, -0.013, -0.013, -0.013, -0.012, -0.012, -0.012, -0.012,
   -0.012, -0.011},
  {// charge = 360 fC
   0.000,  -0.001, 0.012,  0.046,  0.100,  0.170,  0.252,  0.342,  0.437,
   0.534,  0.632,  0.728,  0.822,  0.912,  0.999,  1.081,  1.157,  1.226,
   1.282,  1.327,  1.360,  1.383,  1.399,  1.410,  1.417,  1.421,  1.425,
   1.427,  1.428,  1.429,  1.430,  1.430,  1.429,  1.428,  1.427,  1.425,
   1.423,  1.420,  1.417,  1.413,  1.409,  1.405,  1.400,  1.394,  1.387,
   1.380,  1.373,  1.364,  1.355,  1.346,  1.335,  1.323,  1.311,  1.297,
   1.283,  1.268,  1.251,  1.234,  1.216,  1.197,  1.178,  1.157,  1.136,
   1.114,  1.092,  1.069,  1.046,  1.023,  1.000,  0.976,  0.952,  0.929,
   0.905,  0.881,  0.857,  0.834,  0.810,  0.787,  0.764,  0.741,  0.718,
   0.696,  0.674,  0.652,  0.631,  0.609,  0.589,  0.568,  0.548,  0.528,
   0.509,  0.490,  0.472,  0.454,  0.436,  0.419,  0.402,  0.386,  0.370,
   0.355,  0.339,  0.324,  0.310,  0.296,  0.283,  0.269,  0.257,  0.245,
   0.233,  0.221,  0.210,  0.200,  0.189,  0.179,  0.170,  0.160,  0.151,
   0.143,  0.134,  0.127,  0.119,  0.112,  0.105,  0.098,  0.091,  0.085,
   0.079,  0.074,  0.068,  0.063,  0.058,  0.053,  0.049,  0.045,  0.041,
   0.037,  0.033,  0.030,  0.026,  0.023,  0.020,  0.018,  0.015,  0.012,
   0.010,  0.008,  0.006,  0.004,  0.002,  0.001,  -0.001, -0.002, -0.004,
   -0.005, -0.006, -0.007, -0.008, -0.009, -0.010, -0.011, -0.012, -0.012,
   -0.013, -0.013, -0.014, -0.014, -0.015, -0.015, -0.015, -0.015, -0.016,
   -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016,
   -0.016, -0.016, -0.016, -0.015, -0.015, -0.015, -0.015, -0.015, -0.015,
   -0.014, -0.014, -0.014, -0.014, -0.013, -0.013, -0.013, -0.013, -0.013,
   -0.012, -0.012},
  {// charge = 375 fC
   0.000,  -0.001, 0.012,  0.047,  0.102,  0.173,  0.256,  0.348,  0.446,
   0.546,  0.647,  0.747,  0.845,  0.939,  1.029,  1.114,  1.193,  1.261,
   1.314,  1.353,  1.381,  1.399,  1.411,  1.418,  1.423,  1.427,  1.429,
   1.431,  1.432,  1.433,  1.434,  1.434,  1.433,  1.433,  1.432,  1.431,
   1.429,  1.427,  1.425,  1.422,  1.419,  1.415,  1.411,  1.406,  1.401,
   1.395,  1.389,  1.382,  1.374,  1.366,  1.357,  1.347,  1.336,  1.325,
   1.313,  1.299,  1.285,  1.270,  1.253,  1.236,  1.218,  1.200,  1.180,
   1.160,  1.139,  1.117,  1.095,  1.072,  1.049,  1.026,  1.003,  0.979,
   0.955,  0.932,  0.908,  0.884,  0.860,  0.837,  0.813,  0.790,  0.767,
   0.744,  0.721,  0.699,  0.677,  0.655,  0.633,  0.612,  0.591,  0.571,
   0.551,  0.531,  0.511,  0.493,  0.474,  0.456,  0.438,  0.421,  0.404,
   0.387,  0.371,  0.356,  0.341,  0.326,  0.311,  0.298,  0.284,  0.271,
   0.258,  0.246,  0.234,  0.222,  0.211,  0.201,  0.190,  0.180,  0.170,
   0.161,  0.152,  0.144,  0.135,  0.127,  0.120,  0.112,  0.105,  0.098,
   0.092,  0.086,  0.080,  0.074,  0.069,  0.063,  0.058,  0.054,  0.049,
   0.045,  0.041,  0.037,  0.033,  0.030,  0.026,  0.023,  0.020,  0.018,
   0.015,  0.012,  0.010,  0.008,  0.006,  0.004,  0.002,  0.000,  -0.001,
   -0.003, -0.004, -0.005, -0.007, -0.008, -0.009, -0.010, -0.010, -0.011,
   -0.012, -0.013, -0.013, -0.014, -0.014, -0.015, -0.015, -0.015, -0.016,
   -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016,
   -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.015,
   -0.015, -0.015, -0.015, -0.015, -0.014, -0.014, -0.014, -0.014, -0.013,
   -0.013, -0.013},
  {// charge = 390 fC
   0.000,  -0.001, 0.012,  0.048,  0.103,  0.175,  0.259,  0.354,  0.454,
   0.557,  0.661,  0.764,  0.865,  0.962,  1.056,  1.145,  1.225,  1.290,
   1.339,  1.373,  1.395,  1.409,  1.418,  1.424,  1.427,  1.430,  1.432,
   1.434,  1.435,  1.436,  1.436,  1.436,  1.436,  1.436,  1.436,  1.435,
   1.434,  1.432,  1.430,  1.428,  1.426,  1.423,  1.420,  1.416,  1.412,
   1.407,  1.402,  1.396,  1.390,  1.383,  1.375,  1.367,  1.358,  1.348,
   1.338,  1.327,  1.314,  1.301,  1.287,  1.272,  1.256,  1.239,  1.221,
   1.202,  1.183,  1.163,  1.142,  1.120,  1.098,  1.075,  1.052,  1.029,
   1.006,  0.982,  0.958,  0.935,  0.911,  0.887,  0.863,  0.840,  0.816,
   0.793,  0.770,  0.747,  0.724,  0.702,  0.679,  0.658,  0.636,  0.615,
   0.594,  0.573,  0.553,  0.533,  0.514,  0.495,  0.476,  0.458,  0.440,
   0.423,  0.406,  0.389,  0.374,  0.357,  0.342,  0.327,  0.313,  0.299,
   0.285,  0.272,  0.260,  0.247,  0.235,  0.224,  0.212,  0.202,  0.191,
   0.181,  0.171,  0.162,  0.153,  0.144,  0.136,  0.128,  0.120,  0.113,
   0.106,  0.099,  0.092,  0.086,  0.080,  0.074,  0.069,  0.064,  0.059,
   0.054,  0.049,  0.045,  0.041,  0.037,  0.033,  0.030,  0.026,  0.023,
   0.020,  0.017,  0.015,  0.012,  0.010,  0.008,  0.006,  0.004,  0.002,
   0.000,  -0.001, -0.003, -0.004, -0.006, -0.007, -0.008, -0.009, -0.010,
   -0.011, -0.012, -0.012, -0.013, -0.013, -0.014, -0.015, -0.015, -0.015,
   -0.016, -0.016, -0.016, -0.016, -0.016, -0.017, -0.017, -0.017, -0.017,
   -0.017, -0.017, -0.017, -0.017, -0.016, -0.016, -0.016, -0.016, -0.016,
   -0.016, -0.016, -0.016, -0.015, -0.015, -0.015, -0.015, -0.015, -0.014,
   -0.014, -0.014},
  {// charge = 405 fC
   0.000,  -0.001, 0.013,  0.049,  0.104,  0.177,  0.262,  0.358,  0.460,
   0.566,  0.672,  0.778,  0.882,  0.983,  1.080,  1.172,  1.251,  1.313,
   1.358,  1.387,  1.405,  1.416,  1.422,  1.427,  1.430,  1.432,  1.434,
   1.435,  1.436,  1.437,  1.438,  1.438,  1.438,  1.438,  1.438,  1.438,
   1.437,  1.436,  1.435,  1.433,  1.431,  1.429,  1.427,  1.424,  1.421,
   1.417,  1.413,  1.408,  1.403,  1.397,  1.391,  1.384,  1.376,  1.368,
   1.359,  1.350,  1.339,  1.328,  1.316,  1.303,  1.289,  1.274,  1.258,
   1.241,  1.223,  1.205,  1.186,  1.165,  1.145,  1.123,  1.101,  1.078,
   1.055,  1.032,  1.009,  0.985,  0.962,  0.938,  0.914,  0.890,  0.867,
   0.843,  0.819,  0.796,  0.773,  0.750,  0.727,  0.704,  0.682,  0.660,
   0.639,  0.617,  0.596,  0.576,  0.555,  0.535,  0.516,  0.497,  0.478,
   0.460,  0.442,  0.425,  0.408,  0.391,  0.375,  0.359,  0.344,  0.329,
   0.314,  0.300,  0.287,  0.274,  0.261,  0.248,  0.236,  0.225,  0.213,
   0.202,  0.192,  0.182,  0.172,  0.163,  0.154,  0.145,  0.137,  0.128,
   0.121,  0.113,  0.106,  0.099,  0.093,  0.086,  0.080,  0.075,  0.069,
   0.064,  0.059,  0.054,  0.049,  0.045,  0.041,  0.037,  0.033,  0.030,
   0.026,  0.023,  0.020,  0.017,  0.015,  0.012,  0.010,  0.007,  0.005,
   0.003,  0.002,  -0.000, -0.002, -0.003, -0.005, -0.006, -0.007, -0.008,
   -0.009, -0.010, -0.011, -0.012, -0.013, -0.013, -0.014, -0.014, -0.015,
   -0.015, -0.016, -0.016, -0.016, -0.016, -0.017, -0.017, -0.017, -0.017,
   -0.017, -0.017, -0.017, -0.017, -0.017, -0.017, -0.017, -0.017, -0.017,
   -0.017, -0.016, -0.016, -0.016, -0.016, -0.016, -0.016, -0.015, -0.015,
   -0.015, -0.015},
  {// charge = 420 fC
   0.000,  -0.001, 0.013,  0.050,  0.105,  0.178,  0.265,  0.362,  0.466,
   0.573,  0.682,  0.790,  0.897,  1.000,  1.099,  1.193,  1.271,  1.330,
   1.370,  1.396,  1.410,  1.419,  1.424,  1.428,  1.431,  1.433,  1.435,
   1.436,  1.437,  1.438,  1.439,  1.439,  1.440,  1.440,  1.440,  1.440,
   1.439,  1.439,  1.438,  1.437,  1.436,  1.434,  1.432,  1.430,  1.428,
   1.425,  1.421,  1.418,  1.414,  1.409,  1.404,  1.398,  1.392,  1.385,
   1.378,  1.370,  1.361,  1.351,  1.341,  1.330,  1.318,  1.305,  1.291,
   1.276,  1.260,  1.244,  1.226,  1.208,  1.188,  1.168,  1.148,  1.126,
   1.104,  1.081,  1.059,  1.035,  1.012,  0.989,  0.965,  0.941,  0.917,
   0.893,  0.870,  0.846,  0.822,  0.799,  0.776,  0.753,  0.730,  0.707,
   0.685,  0.663,  0.641,  0.620,  0.599,  0.578,  0.558,  0.538,  0.518,
   0.499,  0.480,  0.462,  0.444,  0.426,  0.410,  0.392,  0.376,  0.361,
   0.345,  0.330,  0.316,  0.302,  0.288,  0.275,  0.262,  0.249,  0.237,
   0.225,  0.214,  0.203,  0.193,  0.183,  0.173,  0.163,  0.154,  0.145,
   0.137,  0.129,  0.121,  0.113,  0.106,  0.099,  0.093,  0.086,  0.080,
   0.075,  0.069,  0.064,  0.059,  0.054,  0.049,  0.045,  0.041,  0.037,
   0.033,  0.030,  0.026,  0.023,  0.020,  0.017,  0.014,  0.012,  0.009,
   0.007,  0.005,  0.003,  0.001,  -0.001, -0.002, -0.004, -0.005, -0.006,
   -0.008, -0.009, -0.010, -0.011, -0.012, -0.012, -0.013, -0.014, -0.014,
   -0.015, -0.015, -0.016, -0.016, -0.016, -0.017, -0.017, -0.017, -0.017,
   -0.017, -0.017, -0.018, -0.018, -0.018, -0.018, -0.017, -0.017, -0.017,
   -0.017, -0.017, -0.017, -0.017, -0.017, -0.016, -0.016, -0.016, -0.016,
   -0.016, -0.016},
  {// charge = 435 fC
   0.000,  -0.001, 0.013,  0.050,  0.107,  0.180,  0.267,  0.365,  0.470,
   0.579,  0.690,  0.800,  0.908,  1.013,  1.114,  1.208,  1.285,  1.341,
   1.378,  1.400,  1.413,  1.421,  1.425,  1.429,  1.431,  1.433,  1.435,
   1.436,  1.438,  1.439,  1.439,  1.440,  1.441,  1.441,  1.441,  1.441,
   1.441,  1.441,  1.441,  1.440,  1.439,  1.438,  1.437,  1.435,  1.433,
   1.431,  1.429,  1.426,  1.422,  1.419,  1.415,  1.410,  1.405,  1.399,
   1.393,  1.387,  1.379,  1.371,  1.363,  1.353,  1.343,  1.332,  1.320,
   1.308,  1.294,  1.279,  1.263,  1.247,  1.229,  1.211,  1.192,  1.172,
   1.151,  1.130,  1.108,  1.085,  1.062,  1.039,  1.016,  0.992,  0.968,
   0.945,  0.921,  0.897,  0.873,  0.849,  0.826,  0.802,  0.779,  0.756,
   0.733,  0.710,  0.688,  0.665,  0.644,  0.622,  0.601,  0.580,  0.560,
   0.540,  0.520,  0.501,  0.482,  0.464,  0.446,  0.428,  0.411,  0.394,
   0.378,  0.362,  0.346,  0.331,  0.317,  0.303,  0.289,  0.275,  0.262,
   0.250,  0.238,  0.226,  0.215,  0.204,  0.193,  0.183,  0.173,  0.163,
   0.154,  0.145,  0.137,  0.129,  0.121,  0.113,  0.106,  0.099,  0.092,
   0.086,  0.080,  0.074,  0.069,  0.063,  0.058,  0.053,  0.049,  0.044,
   0.040,  0.036,  0.033,  0.029,  0.026,  0.022,  0.019,  0.016,  0.014,
   0.011,  0.009,  0.007,  0.004,  0.002,  0.001,  -0.001, -0.003, -0.004,
   -0.006, -0.007, -0.008, -0.009, -0.010, -0.011, -0.012, -0.013, -0.014,
   -0.014, -0.015, -0.016, -0.016, -0.016, -0.017, -0.017, -0.017, -0.018,
   -0.018, -0.018, -0.018, -0.018, -0.018, -0.018, -0.018, -0.018, -0.018,
   -0.018, -0.018, -0.018, -0.018, -0.018, -0.017, -0.017, -0.017, -0.017,
   -0.017, -0.016},
  {// charge = 450 fC
   0.000,  -0.001, 0.014,  0.051,  0.108,  0.181,  0.269,  0.368,  0.474,
   0.584,  0.696,  0.807,  0.916,  1.022,  1.124,  1.217,  1.292,  1.347,
   1.382,  1.398,  1.410,  1.418,  1.424,  1.430,  1.436,  1.439,  1.441,
   1.442,  1.443,  1.442,  1.441,  1.439,  1.435,  1.431,  1.425,  1.419,
   1.412,  1.404,  1.395,  1.386,  1.376,  1.366,  1.355,  1.343,  1.330,
   1.316,  1.301,  1.285,  1.268,  1.250,  1.232,  1.212,  1.192,  1.171,
   1.149,  1.127,  1.104,  1.080,  1.056,  1.032,  1.008,  0.983,  0.959,
   0.934,  0.910,  0.885,  0.860,  0.836,  0.812,  0.788,  0.764,  0.740,
   0.717,  0.694,  0.671,  0.649,  0.627,  0.605,  0.583,  0.562,  0.542,
   0.522,  0.502,  0.483,  0.464,  0.445,  0.427,  0.410,  0.393,  0.376,
   0.360,  0.344,  0.329,  0.314,  0.299,  0.285,  0.271,  0.259,  0.245,
   0.233,  0.221,  0.210,  0.199,  0.188,  0.178,  0.168,  0.158,  0.149,
   0.140,  0.131,  0.123,  0.115,  0.108,  0.100,  0.093,  0.087,  0.081,
   0.074,  0.069,  0.063,  0.058,  0.053,  0.048,  0.044,  0.039,  0.035,
   0.031,  0.027,  0.024,  0.021,  0.018,  0.015,  0.012,  0.009,  0.007,
   0.004,  0.002,  0.000,  -0.002, -0.003, -0.005, -0.006, -0.008, -0.009,
   -0.010, -0.011, -0.012, -0.013, -0.014, -0.015, -0.016, -0.016, -0.017,
   -0.017, -0.018, -0.018, -0.018, -0.019, -0.019, -0.019, -0.019, -0.019,
   -0.019, -0.019, -0.019, -0.019, -0.019, -0.019, -0.019, -0.019, -0.019,
   -0.019, -0.018, -0.018, -0.018, -0.018, -0.017, -0.017, -0.017, -0.017,
   -0.016, -0.016, -0.016, -0.015, -0.015, -0.015, -0.015, -0.014, -0.014,
   -0.014, -0.013, -0.013, -0.013, -0.012, -0.012, -0.012, -0.012, -0.011,
   -0.011, -0.011},
  {// charge = 465 fC
   0.000,  -0.001, 0.014,  0.052,  0.109,  0.183,  0.271,  0.370,  0.477,
   0.588,  0.701,  0.813,  0.922,  1.028,  1.130,  1.223,  1.305,  1.361,
   1.393,  1.409,  1.421,  1.430,  1.435,  1.438,  1.439,  1.439,  1.437,
   1.433,  1.426,  1.417,  1.405,  1.392,  1.378,  1.362,  1.345,  1.327,
   1.308,  1.287,  1.266,  1.244,  1.220,  1.196,  1.172,  1.146,  1.120,
   1.094,  1.067,  1.040,  1.013,  0.986,  0.959,  0.932,  0.905,  0.878,
   0.851,  0.825,  0.799,  0.773,  0.747,  0.722,  0.697,  0.673,  0.649,
   0.625,  0.602,  0.579,  0.557,  0.535,  0.514,  0.493,  0.472,  0.453,
   0.433,  0.414,  0.396,  0.378,  0.361,  0.344,  0.328,  0.312,  0.297,
   0.282,  0.267,  0.253,  0.240,  0.227,  0.215,  0.203,  0.191,  0.180,
   0.169,  0.159,  0.149,  0.139,  0.130,  0.121,  0.113,  0.105,  0.097,
   0.090,  0.083,  0.076,  0.069,  0.063,  0.058,  0.052,  0.047,  0.042,
   0.037,  0.033,  0.028,  0.024,  0.021,  0.017,  0.014,  0.011,  0.007,
   0.005,  0.002,  -0.000, -0.003, -0.005, -0.007, -0.009, -0.010, -0.012,
   -0.013, -0.015, -0.016, -0.017, -0.018, -0.019, -0.020, -0.020, -0.021,
   -0.021, -0.022, -0.022, -0.023, -0.023, -0.023, -0.024, -0.024, -0.024,
   -0.024, -0.024, -0.024, -0.024, -0.024, -0.023, -0.023, -0.023, -0.023,
   -0.022, -0.022, -0.022, -0.022, -0.021, -0.021, -0.021, -0.020, -0.020,
   -0.020, -0.019, -0.019, -0.018, -0.018, -0.018, -0.017, -0.017, -0.016,
   -0.016, -0.016, -0.015, -0.015, -0.015, -0.014, -0.014, -0.013, -0.013,
   -0.013, -0.012, -0.012, -0.012, -0.011, -0.011, -0.011, -0.010, -0.010,
   -0.010, -0.010, -0.009, -0.009, -0.009, -0.008, -0.008, -0.008, -0.008,
   -0.007, -0.007},
  {// charge = 480 fC
   0.000,  -0.001, 0.014,  0.052,  0.109,  0.184,  0.272,  0.372,  0.480,
   0.591,  0.705,  0.817,  0.927,  1.032,  1.140,  1.246,  1.324,  1.373,
   1.399,  1.416,  1.426,  1.433,  1.436,  1.438,  1.437,  1.433,  1.426,
   1.415,  1.401,  1.385,  1.366,  1.346,  1.324,  1.300,  1.275,  1.249,
   1.222,  1.194,  1.166,  1.137,  1.107,  1.077,  1.047,  1.017,  0.987,
   0.957,  0.927,  0.897,  0.868,  0.839,  0.811,  0.782,  0.755,  0.727,
   0.700,  0.674,  0.648,  0.622,  0.598,  0.573,  0.549,  0.526,  0.503,
   0.481,  0.460,  0.439,  0.419,  0.399,  0.380,  0.361,  0.343,  0.325,
   0.308,  0.292,  0.276,  0.261,  0.246,  0.232,  0.218,  0.205,  0.193,
   0.180,  0.169,  0.157,  0.147,  0.136,  0.127,  0.117,  0.108,  0.100,
   0.091,  0.083,  0.076,  0.068,  0.062,  0.055,  0.049,  0.043,  0.038,
   0.033,  0.028,  0.023,  0.019,  0.015,  0.011,  0.007,  0.004,  0.001,
   -0.002, -0.005, -0.008, -0.010, -0.012, -0.015, -0.016, -0.018, -0.020,
   -0.021, -0.023, -0.024, -0.025, -0.026, -0.027, -0.028, -0.028, -0.029,
   -0.029, -0.030, -0.030, -0.030, -0.030, -0.030, -0.031, -0.031, -0.030,
   -0.030, -0.030, -0.030, -0.030, -0.030, -0.029, -0.029, -0.029, -0.028,
   -0.028, -0.028, -0.027, -0.027, -0.026, -0.026, -0.025, -0.025, -0.024,
   -0.024, -0.023, -0.023, -0.022, -0.022, -0.021, -0.021, -0.020, -0.020,
   -0.019, -0.019, -0.018, -0.018, -0.017, -0.017, -0.016, -0.016, -0.015,
   -0.015, -0.014, -0.014, -0.013, -0.013, -0.013, -0.012, -0.012, -0.012,
   -0.011, -0.011, -0.010, -0.010, -0.010, -0.009, -0.009, -0.009, -0.008,
   -0.008, -0.008, -0.008, -0.007, -0.007, -0.007, -0.007, -0.006, -0.006,
   -0.006, -0.006},
  {// charge = 495 fC
   0.000,  -0.001, 0.014,  0.053,  0.110,  0.185,  0.274,  0.374,  0.482,
   0.594,  0.708,  0.820,  0.929,  1.041,  1.155,  1.259,  1.333,  1.378,
   1.403,  1.419,  1.428,  1.434,  1.436,  1.436,  1.433,  1.426,  1.414,
   1.398,  1.379,  1.357,  1.333,  1.306,  1.278,  1.248,  1.218,  1.187,
   1.155,  1.122,  1.089,  1.056,  1.023,  0.990,  0.958,  0.925,  0.893,
   0.861,  0.830,  0.799,  0.769,  0.739,  0.710,  0.682,  0.653,  0.626,
   0.599,  0.573,  0.547,  0.522,  0.498,  0.475,  0.452,  0.429,  0.408,
   0.387,  0.367,  0.347,  0.328,  0.310,  0.292,  0.275,  0.259,  0.243,
   0.228,  0.213,  0.199,  0.185,  0.172,  0.160,  0.148,  0.137,  0.126,
   0.115,  0.105,  0.096,  0.087,  0.078,  0.070,  0.063,  0.054,  0.048,
   0.041,  0.035,  0.029,  0.024,  0.018,  0.013,  0.009,  0.004,  0.000,
   -0.003, -0.007, -0.010, -0.013, -0.016, -0.019, -0.021, -0.024, -0.026,
   -0.028, -0.029, -0.031, -0.032, -0.033, -0.035, -0.035, -0.036, -0.037,
   -0.038, -0.038, -0.039, -0.039, -0.039, -0.039, -0.039, -0.039, -0.039,
   -0.039, -0.039, -0.039, -0.039, -0.038, -0.038, -0.037, -0.037, -0.036,
   -0.036, -0.035, -0.035, -0.034, -0.034, -0.033, -0.033, -0.032, -0.031,
   -0.030, -0.030, -0.029, -0.029, -0.028, -0.027, -0.026, -0.026, -0.025,
   -0.025, -0.024, -0.023, -0.022, -0.022, -0.021, -0.021, -0.020, -0.019,
   -0.019, -0.018, -0.018, -0.017, -0.017, -0.016, -0.016, -0.015, -0.015,
   -0.014, -0.013, -0.013, -0.013, -0.012, -0.012, -0.011, -0.011, -0.011,
   -0.010, -0.010, -0.009, -0.009, -0.009, -0.008, -0.008, -0.008, -0.007,
   -0.007, -0.007, -0.007, -0.006, -0.006, -0.006, -0.006, -0.006, -0.005,
   -0.005, -0.005},
  {// charge = 510 fC
   0.000,  -0.001, 0.015,  0.053,  0.111,  0.186,  0.275,  0.376,  0.484,
   0.597,  0.710,  0.823,  0.934,  1.049,  1.165,  1.267,  1.338,  1.381,
   1.405,  1.420,  1.429,  1.434,  1.436,  1.435,  1.430,  1.420,  1.405,
   1.385,  1.362,  1.335,  1.307,  1.276,  1.243,  1.210,  1.176,  1.141,
   1.106,  1.070,  1.034,  0.999,  0.964,  0.929,  0.895,  0.861,  0.828,
   0.795,  0.763,  0.731,  0.700,  0.670,  0.640,  0.611,  0.583,  0.556,
   0.529,  0.503,  0.478,  0.453,  0.430,  0.407,  0.384,  0.363,  0.342,
   0.322,  0.303,  0.284,  0.266,  0.249,  0.232,  0.216,  0.201,  0.186,
   0.172,  0.159,  0.146,  0.133,  0.122,  0.110,  0.100,  0.089,  0.080,
   0.071,  0.062,  0.053,  0.046,  0.038,  0.031,  0.025,  0.018,  0.012,
   0.007,  0.002,  -0.003, -0.007, -0.012, -0.016, -0.019, -0.022, -0.025,
   -0.028, -0.031, -0.033, -0.036, -0.038, -0.039, -0.041, -0.042, -0.044,
   -0.045, -0.046, -0.047, -0.047, -0.048, -0.048, -0.049, -0.049, -0.049,
   -0.049, -0.049, -0.049, -0.049, -0.048, -0.048, -0.048, -0.047, -0.047,
   -0.046, -0.046, -0.045, -0.044, -0.044, -0.043, -0.042, -0.042, -0.041,
   -0.040, -0.039, -0.038, -0.038, -0.037, -0.036, -0.035, -0.034, -0.033,
   -0.032, -0.032, -0.031, -0.030, -0.029, -0.028, -0.028, -0.027, -0.026,
   -0.025, -0.024, -0.024, -0.023, -0.022, -0.021, -0.021, -0.020, -0.019,
   -0.019, -0.018, -0.017, -0.017, -0.016, -0.016, -0.015, -0.015, -0.014,
   -0.014, -0.013, -0.013, -0.012, -0.012, -0.011, -0.011, -0.010, -0.010,
   -0.010, -0.009, -0.009, -0.008, -0.008, -0.008, -0.007, -0.007, -0.007,
   -0.007, -0.006, -0.006, -0.006, -0.006, -0.005, -0.005, -0.005, -0.005,
   -0.005, -0.004},
  {// charge = 525 fC
   0.000,  -0.001, 0.015,  0.054,  0.112,  0.187,  0.277,  0.377,  0.486,
   0.599,  0.713,  0.825,  0.939,  1.056,  1.172,  1.272,  1.342,  1.383,
   1.406,  1.421,  1.430,  1.435,  1.436,  1.435,  1.428,  1.416,  1.398,
   1.376,  1.350,  1.321,  1.289,  1.255,  1.220,  1.184,  1.147,  1.110,
   1.072,  1.035,  0.997,  0.960,  0.924,  0.888,  0.852,  0.817,  0.783,
   0.749,  0.717,  0.684,  0.653,  0.622,  0.593,  0.564,  0.535,  0.508,
   0.481,  0.455,  0.430,  0.406,  0.383,  0.360,  0.338,  0.317,  0.297,
   0.277,  0.259,  0.241,  0.223,  0.207,  0.191,  0.175,  0.161,  0.147,
   0.134,  0.121,  0.109,  0.097,  0.086,  0.076,  0.066,  0.057,  0.048,
   0.039,  0.032,  0.024,  0.017,  0.011,  0.004,  -0.001, -0.007, -0.012,
   -0.017, -0.021, -0.025, -0.029, -0.032, -0.036, -0.039, -0.041, -0.044,
   -0.046, -0.048, -0.050, -0.051, -0.053, -0.054, -0.055, -0.056, -0.056,
   -0.057, -0.058, -0.058, -0.058, -0.058, -0.058, -0.058, -0.058, -0.058,
   -0.057, -0.057, -0.056, -0.056, -0.055, -0.054, -0.054, -0.053, -0.052,
   -0.051, -0.050, -0.050, -0.049, -0.048, -0.047, -0.046, -0.045, -0.044,
   -0.043, -0.042, -0.041, -0.040, -0.039, -0.038, -0.037, -0.036, -0.035,
   -0.034, -0.033, -0.032, -0.031, -0.030, -0.029, -0.028, -0.028, -0.027,
   -0.026, -0.025, -0.024, -0.023, -0.022, -0.022, -0.021, -0.020, -0.020,
   -0.019, -0.018, -0.017, -0.017, -0.016, -0.016, -0.015, -0.015, -0.014,
   -0.013, -0.013, -0.012, -0.012, -0.011, -0.011, -0.011, -0.010, -0.010,
   -0.009, -0.009, -0.008, -0.008, -0.008, -0.007, -0.007, -0.007, -0.007,
   -0.006, -0.006, -0.006, -0.006, -0.005, -0.005, -0.005, -0.005, -0.005,
   -0.004, -0.004},
  {// charge = 540 fC
   0.000,  -0.001, 0.015,  0.054,  0.113,  0.188,  0.278,  0.379,  0.487,
   0.601,  0.715,  0.827,  0.944,  1.061,  1.177,  1.276,  1.344,  1.384,
   1.407,  1.422,  1.430,  1.435,  1.437,  1.435,  1.428,  1.415,  1.395,
   1.371,  1.343,  1.312,  1.278,  1.242,  1.205,  1.167,  1.129,  1.090,
   1.051,  1.012,  0.974,  0.935,  0.898,  0.861,  0.825,  0.789,  0.754,
   0.720,  0.686,  0.654,  0.622,  0.591,  0.561,  0.532,  0.503,  0.476,
   0.449,  0.423,  0.399,  0.374,  0.351,  0.329,  0.307,  0.286,  0.266,
   0.247,  0.229,  0.211,  0.194,  0.178,  0.163,  0.148,  0.134,  0.120,
   0.108,  0.095,  0.084,  0.073,  0.062,  0.052,  0.043,  0.034,  0.026,
   0.018,  0.011,  0.004,  -0.003, -0.009, -0.014, -0.019, -0.025, -0.029,
   -0.033, -0.037, -0.041, -0.044, -0.047, -0.050, -0.052, -0.054, -0.056,
   -0.058, -0.060, -0.061, -0.062, -0.063, -0.064, -0.065, -0.065, -0.066,
   -0.066, -0.066, -0.066, -0.066, -0.066, -0.065, -0.065, -0.064, -0.064,
   -0.063, -0.062, -0.062, -0.061, -0.060, -0.059, -0.058, -0.057, -0.056,
   -0.055, -0.054, -0.053, -0.052, -0.051, -0.050, -0.049, -0.048, -0.046,
   -0.045, -0.044, -0.043, -0.042, -0.041, -0.040, -0.039, -0.037, -0.036,
   -0.035, -0.034, -0.033, -0.032, -0.031, -0.030, -0.029, -0.028, -0.027,
   -0.026, -0.025, -0.025, -0.024, -0.023, -0.022, -0.021, -0.021, -0.020,
   -0.019, -0.018, -0.018, -0.017, -0.016, -0.016, -0.015, -0.015, -0.014,
   -0.013, -0.013, -0.012, -0.012, -0.011, -0.011, -0.010, -0.010, -0.010,
   -0.009, -0.009, -0.008, -0.008, -0.008, -0.007, -0.007, -0.007, -0.007,
   -0.006, -0.006, -0.006, -0.005, -0.005, -0.005, -0.005, -0.005, -0.004,
   -0.004, -0.004}};
ClassImp(CbmTrdFASP)
