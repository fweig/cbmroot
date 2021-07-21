/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, Alexandru Bercuci */


#include "CbmTrdUnpackAlgoFasp2D.h"

#include "CbmTrdDigi.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <boost/format.hpp>

using namespace std;

CbmTrdUnpackAlgoFasp2D::CbmTrdUnpackAlgoFasp2D() : CbmRecoUnpackAlgo("CbmTrdUnpackAlgoFasp2D")
{
  memset(fTime, 0, NCRI * sizeof(ULong64_t));
}

CbmTrdUnpackAlgoFasp2D::~CbmTrdUnpackAlgoFasp2D() {}

//_________________________________________________________________________________
CbmTrdUnpackAlgoFasp2D::CbmTrdFaspMessageType CbmTrdUnpackAlgoFasp2D::mess_type(uint32_t wd)
{
  if ((wd >> kMessCh) & 0x1) return kData;
  return kEpoch;
}

//_________________________________________________________________________________
void CbmTrdUnpackAlgoFasp2D::mess_readDW(uint32_t w, CbmTrdFaspContent* mess)
{
  uint32_t wd(w), shift(0);
  mess->ch = wd & 0xf;
  shift += Int_t(kMessCh);
  mess->type = (wd >> shift) & 0x1;
  shift += Int_t(kMessType);
  mess->tlab = (wd >> shift) & 0x7f;
  shift += Int_t(kMessTlab);
  mess->data = (wd >> shift) & 0x3fff;
  shift += Int_t(kMessData);
  mess->fasp = (wd >> shift) & 0x3f;

  printf("DBG :: ");
  mess_prt(mess);
}

//_________________________________________________________________________________
void CbmTrdUnpackAlgoFasp2D::mess_readEW(uint32_t w, CbmTrdFaspContent* mess)
{
  uint32_t wd(w), shift(0);
  mess->ch = wd & 0xf;
  shift += Int_t(kMessCh);
  mess->type = (wd >> shift) & 0x1;
  shift += Int_t(kMessType);
  mess->epoch = (wd >> shift) & 0x1fffff;
  shift += Int_t(kMessEpoch);
  mess->cri = (wd >> shift) & 0x3f;
  printf("DBG :: ");
  mess_prt(mess);
}

//_________________________________________________________________________________
void CbmTrdUnpackAlgoFasp2D::mess_prt(CbmTrdFaspContent* mess)
{
  if (mess->type == kData)
    cout << boost::format("    DATA : fasp_id=%02d ch_id=%02d tclk=%03d data=%4d\n")
              % static_cast<unsigned int>(mess->fasp) % static_cast<unsigned int>(mess->ch)
              % static_cast<unsigned int>(mess->tlab) % static_cast<unsigned int>(mess->data);
  else
    cout << boost::format("    EPOCH: cri_id=%02d ch_id=%02d epoch=%05d\n") % static_cast<unsigned int>(mess->cri)
              % static_cast<unsigned int>(mess->ch) % static_cast<unsigned int>(mess->epoch);
}

#define VERBOSE 0
//_________________________________________________________________________________
bool CbmTrdUnpackAlgoFasp2D::pushDigis(
  std::map<UChar_t, std::vector<CbmTrdUnpackAlgoFasp2D::CbmTrdFaspContent*>> messes)
{
  bool use(false);
  UChar_t lFasp(0xff);
  UShort_t lchR, lchT;
  Double_t r, t;
  Int_t dt, dtime;
  vector<CbmTrdDigi*> digis;
  for (Int_t col(0); col < NCOLS; col++) {
    if (!messes[col].size()) continue;
    if (lFasp == 0xff) lFasp = messes[col][0]->fasp;
    //printf("col[%d]=%lu\n", col, messes[col].size());
    for (vector<CbmTrdFaspContent*>::iterator i = messes[col].begin(); i != messes[col].end(); i++) {
      if (VERBOSE) mess_prt((*i));

      lchR = 0;
      lchT = 0;
      use  = false;
      if ((*i)->ch % 2) lchR = (*i)->data;
      else
        lchT = (*i)->data;
      for (vector<CbmTrdDigi*>::iterator id = digis.begin(); id != digis.end(); id++) {
        dtime = (*id)->GetTimeDAQ() - (*i)->tlab;
        if (TMath::Abs(dtime) < 5) {
          r = (*id)->GetCharge(t, dt);
          if (lchR && !int(r)) {
            (*id)->SetCharge(t, lchR, -dtime);
            use = true;
            break;
          }
          else if (lchT && !int(t)) {
            (*id)->SetCharge(lchT, r, +dtime);
            use = true;
            break;
          }
        }
      }
      if (!use) digis.push_back(new CbmTrdDigi(lFasp * NCOLS + col, lchT, lchR, (*i)->tlab));
      delete (*i);
    }

    // push finalized digits to the next level
    for (vector<CbmTrdDigi*>::iterator id = digis.begin(); id != digis.end(); id++) {
      (*id)->SetTimeDAQ(fTime[0] + (*id)->GetTimeDAQ());
      fOutputVec.emplace_back(*std::move(*id));
      if (VERBOSE) cout << (*id)->ToString();
    }

    digis.clear();
    messes[col].clear();
  }


  return true;
}

// ---- unpack ----
bool CbmTrdUnpackAlgoFasp2D::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{
  if (VERBOSE) printf("CbmTrdUnpackAlgoFasp2D::unpack 0x%04x %d\n", icomp, imslice);
  //LOG(info) << "Component " << icomp << " connected to config CbmTrdUnpackConfig2D. Slice "<<imslice;

  bool unpackOk = true;
  //Double_t fdMsSizeInNs = 1.28e6;

  auto msdesc = ts->descriptor(icomp, imslice);
  if (VERBOSE) printf("time start %lu\n", msdesc.idx);
  // define time wrt start of time slice in TRD/FASP clks [80 MHz]
  fTime[0] = ULong64_t((msdesc.idx - fTsStartTime) / 12.5);

  // Get the µslice size in bytes to calculate the number of completed words
  auto mssize = msdesc.size;

  // Get the number of complete words in the input MS buffer.
  std::uint32_t nwords = mssize / 4;  //fBytesPerWord;

  const auto mspointer = ts->content(icomp, imslice);

  // We have 32 bit spadic frames in this readout version
  const auto mscontent = reinterpret_cast<const size_t*>(mspointer);

  const uint32_t* wd = reinterpret_cast<const uint32_t*>(mscontent);


  UChar_t lFaspOld(0xff), col;
  map<UChar_t, vector<CbmTrdFaspContent*>> vDigi;
  CbmTrdFaspContent* mess(nullptr);
  for (uint64_t j = 0; j < nwords; j++, wd++) {
    //     // Select the appropriate conversion type of the word according to the message type
    //     switch(mess_type(*wd)){
    //       case CbmTrdFaspMessageType::kData:
    //         mess_readDW(*wd, &mess);
    //         break;
    //       case CbmTrdFaspMessageType::kEpoch:
    //         mess_readEW(*wd, &mess);
    //         break;
    //     }
    //if(VERBOSE) mess_prt(&mess);
    uint32_t w      = *wd;
    uint8_t ch_id   = w & 0xf;
    uint8_t isaux   = (w >> 4) & 0x1;
    uint8_t slice   = (w >> 5) & 0x7f;
    uint16_t data   = (w >> 12) & 0x3fff;
    uint32_t epoch  = (w >> 5) & 0x1fffff;
    uint8_t fasp_id = (w >> 26) & 0x3f;
    //    out<<"fasp_id="<<static_cast<unsigned int>(fasp_id)<<" ch_id="<<static_cast<unsigned int>(ch_id)<<" isaux="<<static_cast<unsigned int>(isaux)<<std::endl;
    if (isaux) {
      if (!ch_id) {
        if (VERBOSE)
          cout << boost::format("    EE : cri_id=%02d ch_id=%02d epoch=%03d\n") % static_cast<unsigned int>(fasp_id)
                    % static_cast<unsigned int>(ch_id) % static_cast<unsigned int>(epoch);

        if (vDigi.size()) { pushDigis(vDigi); }
        vDigi.clear();
        lFaspOld = 0xff;

        fTime[fasp_id] += 128;
      }
      else if (ch_id == 1) {
        if (VERBOSE) cout << boost::format("    PAUSE: fasp_id=%02d\n") % static_cast<unsigned int>(fasp_id);
      }
    }
    else {
      if (lFaspOld != fasp_id) {
        // push
        if (vDigi.size()) { pushDigis(vDigi); }
        vDigi.clear();
        lFaspOld = fasp_id;
      }
      if (VERBOSE)
        cout << boost::format("    DD : fasp_id=%02d ch_id=%02d slice=%03d data=%4d\n")
                  % static_cast<unsigned int>(fasp_id) % static_cast<unsigned int>(ch_id)
                  % static_cast<unsigned int>(slice) % static_cast<unsigned int>(data >> 1);
      mess       = new CbmTrdFaspContent;
      mess->ch   = ch_id;
      mess->type = 1;
      mess->tlab = slice;
      mess->data = data >> 1;
      mess->fasp = lFaspOld;
      col        = ch_id >> 1;
      vDigi[col].push_back(mess);
    }
    //prt_wd(*wd);
  }
  return unpackOk;
}

//_____________________________________________________________
void CbmTrdUnpackAlgoFasp2D::prt_wd(uint32_t w)
{
  //    out<<w<<std::endl;
  uint8_t ch_id   = w & 0xf;
  uint8_t isaux   = (w >> 4) & 0x1;
  uint8_t slice   = (w >> 5) & 0x7f;
  uint16_t data   = (w >> 12) & 0x3fff;
  uint32_t epoch  = (w >> 5) & 0x1fffff;
  uint8_t fasp_id = (w >> 26) & 0x3f;
  //    out<<"fasp_id="<<static_cast<unsigned int>(fasp_id)<<" ch_id="<<static_cast<unsigned int>(ch_id)<<" isaux="<<static_cast<unsigned int>(isaux)<<std::endl;
  if (isaux) {
    if (!ch_id) { cout << boost::format("EE: %08d\n") % epoch; }
    else if (ch_id == 1) {
      cout << boost::format("    PAUSE: fasp_id=%02d\n") % static_cast<unsigned int>(fasp_id);
    }
  }
  else {
    cout << boost::format("    DATA: w=%08x fasp_id=%02d ch_id=%02d slice=%03d data=%04d\n") % w
              % static_cast<unsigned int>(fasp_id) % static_cast<unsigned int>(ch_id) % static_cast<unsigned int>(slice)
              % static_cast<unsigned int>(data >> 1);
  }
}

ClassImp(CbmTrdUnpackAlgoFasp2D)
