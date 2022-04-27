/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer], Alexandru Bercuci */


#include "CbmTrdUnpackFaspAlgo.h"

#include "CbmTrdDigi.h"
#include "CbmTrdParFasp.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdParSetGain.h"
#include "CbmTrdParSetGas.h"
#include "CbmTrdParSpadic.h"

#include <FairParAsciiFileIo.h>
#include <FairParGenericSet.h>
#include <FairParamList.h>
#include <FairRuntimeDb.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <boost/format.hpp>

#define VERBOSE 0

using namespace std;

CbmTrdUnpackFaspAlgo::CbmTrdUnpackFaspAlgo() : CbmRecoUnpackAlgo("CbmTrdUnpackFaspAlgo"), fModuleId(), fAsicPar()
{
  memset(fTime, 0, NCRI * sizeof(ULong64_t));
}

//_________________________________________________________________________________
CbmTrdUnpackFaspAlgo::~CbmTrdUnpackFaspAlgo() {}

//_________________________________________________________________________________
Bool_t CbmTrdUnpackFaspAlgo::initParSet(FairParGenericSet* parset)
{
  FairParamList parList;
  Int_t nModules(0);
  if (strcmp(parset->ClassName(), "CbmTrdParSetAsic") == 0) {
    CbmTrdParSetAsic* setPar = static_cast<CbmTrdParSetAsic*>(parset);
    for (auto did : fModuleId) {
      const CbmTrdParSetAsic* setDet = static_cast<const CbmTrdParSetAsic*>(setPar->GetModuleSet(did));
      if (!setDet) continue;
      if (setDet->GetAsicType() != Int_t(CbmTrdDigi::eCbmTrdAsicType::kFASP)) continue;
      if (fMonitor) fMonitor->addParam(did, setDet);
      nModules++;
      std::vector<Int_t> a;
      setDet->GetAsicAddresses(&a);
      for (auto add : a) {
        CbmTrdParAsic* asic = (CbmTrdParAsic*) setDet->GetModulePar(add);
        if (asic->IsA() == CbmTrdParSpadic::Class()) continue;
        fAsicPar.addParam(asic);
        if (VERBOSE) asic->Print();
      }
    }
    //      setPar->printParams();
    LOG(info) << GetName() << "::initParSet - for container " << parset->ClassName() << " modules " << nModules
              << " asics " << fAsicPar.GetNrOfModules();
  }
  else if (strcmp(parset->ClassName(), "CbmTrdParSetDigi") == 0) {
    fDigiSet                          = static_cast<CbmTrdParSetDigi*>(parset);
    map<Int_t, CbmTrdParMod*> digiPar = fDigiSet->GetModuleMap();
    for (auto digi : digiPar) {
      fModuleId.emplace_back(digi.first);
      if (fMonitor) fMonitor->addParam(digi.first, (CbmTrdParModDigi*) digi.second);
    }
    // setPar->printParams();
    LOG(info) << GetName() << "::initParSet - for container " << parset->ClassName() << " modules " << fModuleId.size();
  }
  else if (strcmp(parset->ClassName(), "CbmTrdParSetGas") == 0) {
    CbmTrdParSetGas* setPar = static_cast<CbmTrdParSetGas*>(parset);
    setPar->printParams();
    nModules = setPar->GetNrOfModules();
  }
  else if (strcmp(parset->ClassName(), "CbmTrdParSetGain") == 0) {
    CbmTrdParSetGain* setPar = static_cast<CbmTrdParSetGain*>(parset);
    setPar->printParams();
    nModules = setPar->GetNrOfModules();
  }
  else {
    LOG(error) << "Parameter set " << parset->ClassName() << " not known. Skip.";
    return kFALSE;
  }
  return kTRUE;
}

//_________________________________________________________________________________
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
CbmTrdUnpackFaspAlgo::GetParContainerRequest(std::string geoTag, std::uint32_t runId)
{
  LOG(info) << GetName() << "::GetParContainerRequest - for container " << geoTag.data() << " run " << runId << "  "
            << fParFilesBasePath.data();

  // Basepath for default Trd parameter sets (those connected to a geoTag)
  std::string basepath = Form("%s/trd_%s", fParFilesBasePath.data(), geoTag.data());
  std::string temppath = "";

  // Digest the runId information in case of runId = 0 we use the default fall back
  std::string runpath = "";
  if (runId != 0) { runpath = ".run" + std::to_string(runId); }

  temppath = basepath + runpath + ".digi" + ".par";
  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmTrdParSetDigi>()));
  temppath = basepath + runpath + ".asic" + ".par";
  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmTrdParSetAsic>()));
  //   temppath = basepath + runpath + ".gas" + ".par";
  //   fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmTrdParSetGas>()));
  //   temppath = basepath + runpath + ".gain" + ".par";
  //   fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmTrdParSetGain>()));

  return &fParContVec;
}

//_________________________________________________________________________________
void CbmTrdUnpackFaspAlgo::SetAsicMapping(const std::map<uint32_t, uint8_t[NFASPMOD]>& asicMap)
{
  if (!fFaspMap) fFaspMap = new std::map<uint32_t, uint8_t[NFASPMOD]>(asicMap);
  else {
    delete fFaspMap;
    fFaspMap = new std::map<uint32_t, uint8_t[NFASPMOD]>(asicMap);
  }
}

//_________________________________________________________________________________
void CbmTrdUnpackFaspAlgo::PrintAsicMapping()
{
  if (!fFaspMap) {
    LOG(info) << GetName() << "No asic mapping loaded.";
    return;
  }
  LOG(info) << GetName() << "Fasp Asic mapping on modules:";
  for (auto imod : (*fFaspMap)) {
    printf("Mod [%6d] :", imod.first);
    for (int ifasp(0); ifasp < NFASPMOD; ifasp++) {
      if (ifasp % 30 == 0) printf("\n");
      int jfasp = imod.second[ifasp];
      printf("%3d ", (jfasp == 0xff ? -1 : jfasp));
    }
    printf("\n");
  }
}

//_________________________________________________________________________________
CbmTrdUnpackFaspAlgo::CbmTrdFaspMessageType CbmTrdUnpackFaspAlgo::mess_type(uint32_t wd)
{
  if ((wd >> kMessCh) & 0x1) return kData;
  return kEpoch;
}

//_________________________________________________________________________________
void CbmTrdUnpackFaspAlgo::mess_readDW(uint32_t w, CbmTrdFaspContent* mess)
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
void CbmTrdUnpackFaspAlgo::mess_readEW(uint32_t w, CbmTrdFaspContent* mess)
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
void CbmTrdUnpackFaspAlgo::mess_prt(CbmTrdFaspContent* mess)
{
  if (mess->type == kData)
    cout << boost::format("    DATA : fasp_id=%02d ch_id=%02d tclk=%03d data=%4d\n")
              % static_cast<unsigned int>(mess->fasp) % static_cast<unsigned int>(mess->ch)
              % static_cast<unsigned int>(mess->tlab) % static_cast<unsigned int>(mess->data);
  else
    cout << boost::format("    EPOCH: cri_id=%02d ch_id=%02d epoch=%05d\n") % static_cast<unsigned int>(mess->cri)
              % static_cast<unsigned int>(mess->ch) % static_cast<unsigned int>(mess->epoch);
}

//_________________________________________________________________________________
bool CbmTrdUnpackFaspAlgo::pushDigis(std::vector<CbmTrdUnpackFaspAlgo::CbmTrdFaspContent*> messes)
{
  UChar_t lFasp(0xff);
  UShort_t lchR, lchT;
  Double_t r, t;
  Int_t dt, dtime, ch, pad, row;
  ULong64_t tlab, tdaqOffset(0);
  CbmTrdParFasp* faspPar(nullptr);
  const CbmTrdParFaspChannel* chCalib(nullptr);
  CbmTrdParModDigi* digiPar(nullptr);
  vector<CbmTrdDigi*> digis;
  for (auto imess : messes) {
    if (lFasp == 0xff) {
      lFasp = messes[0]->fasp;
      // link data to the position on the padplane
      if (!(faspPar = (CbmTrdParFasp*) fAsicPar.GetAsicPar(imess->cri * 1000 + lFasp))) {
        LOG(error) << GetName() << "::pushDigis - Par for FASP " << (int) lFasp << " in module " << imess->cri
                   << " missing. Skip.";
        return false;
      }
      if (!(digiPar = (CbmTrdParModDigi*) fDigiSet->GetModulePar(imess->cri))) {
        LOG(error) << GetName() << "::pushDigis - DIGI par for module " << imess->cri << " missing. Skip.";
        return false;
      }
      if (VERBOSE) faspPar->Print();
      pad     = faspPar->GetChannelAddress(imess->ch);
      chCalib = faspPar->GetChannel(imess->ch);
      ch      = 2 * pad + chCalib->HasPairingR();
      row     = digiPar->GetPadRow(pad);
      if (row % 2) tdaqOffset = 3;
      if (VERBOSE)
        printf("fasp[%2d] ch[%4d / %2d] pad[%4d] row[%2d] col[%2d] tilt[%d]\n", lFasp, ch, imess->ch, pad, row,
               digiPar->GetPadColumn(pad), chCalib->HasPairingT());
    }

    if (VERBOSE) mess_prt(imess);

    lchR    = 0;
    lchT    = 0;
    chCalib = faspPar->GetChannel(imess->ch);
    if (chCalib->HasPairingR()) lchR = imess->data;
    else
      lchT = imess->data;
    pad = faspPar->GetChannelAddress(imess->ch);

    bool use(false);
    for (auto id : digis) {
      if (id->GetAddressChannel() != pad) continue;
      dtime = id->GetTimeDAQ() - imess->tlab;
      if (TMath::Abs(dtime) < 5) {
        r = id->GetCharge(t, dt);
        if (lchR && !int(r)) {
          id->SetCharge(t, lchR, -dtime);
          use = true;
          break;
        }
        else if (lchT && !int(t)) {
          tlab = id->GetTimeDAQ();
          id->SetCharge(lchT, r, +dtime);
          id->SetTimeDAQ(ULong64_t(tlab - dtime));
          use = true;
          break;
        }
      }
    }

    if (!use) {
      CbmTrdDigi* digi = new CbmTrdDigi(pad, lchT, lchR, imess->tlab);
      digi->SetAddressModule(imess->cri);
      digis.push_back(digi);
    }
    delete imess;
  }

  // push finalized digits to the next level
  for (vector<CbmTrdDigi*>::iterator id = digis.begin(); id != digis.end(); id++) {
    // TODO temporary add DAQ time calibration for FASPRO.
    // Should be absorbed in the ASIC parameter definition
    (*id)->SetTimeDAQ(fTime[0] + (*id)->GetTimeDAQ() + tdaqOffset);
    fOutputVec.emplace_back(*std::move(*id));
    if (fMonitor) fMonitor->FillHistos((*id));
    if (VERBOSE) cout << (*id)->ToString();
  }

  digis.clear();
  messes.clear();

  return true;
}

// ---- unpack ----
bool CbmTrdUnpackFaspAlgo::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{
  if (VERBOSE) printf("CbmTrdUnpackFaspAlgo::unpack 0x%04x %d\n", icomp, imslice);
  //LOG(info) << "Component " << icomp << " connected to config CbmTrdUnpackConfig2D. Slice "<<imslice;

  uint32_t mod_id = 5;
  bool unpackOk   = true;
  //Double_t fdMsSizeInNs = 1.28e6;

  auto msdesc = ts->descriptor(icomp, imslice);
  // Cast required to silence a warning on macos (there a uint64_t is a llu)
  if (VERBOSE) printf("time start %lu\n", static_cast<size_t>(msdesc.idx));
  // define time wrt start of time slice in TRD/FASP clks [80 MHz]
  fTime[0] = ULong64_t((msdesc.idx - fTsStartTime - fSystemTimeOffset) / 12.5);

  // Get the µslice size in bytes to calculate the number of completed words
  auto mssize = msdesc.size;

  // Get the number of complete words in the input MS buffer.
  std::uint32_t nwords = mssize / 4;  //fBytesPerWord;

  const auto mspointer = ts->content(icomp, imslice);

  // We have 32 bit spadic frames in this readout version
  const auto mscontent = reinterpret_cast<const size_t*>(mspointer);

  const uint32_t* wd = reinterpret_cast<const uint32_t*>(mscontent);


  UChar_t lFaspOld(0xff);
  vector<CbmTrdFaspContent*> vDigi;
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
    uint32_t w      = *wd;
    uint8_t ch_id   = w & 0xf;
    uint8_t isaux   = (w >> 4) & 0x1;
    uint8_t slice   = (w >> 5) & 0x7f;
    uint16_t data   = (w >> 12) & 0x3fff;
    uint32_t epoch  = (w >> 5) & 0x1fffff;
    uint8_t fasp_id = (w >> 26) & 0x3f;
    // std::cout<<"fasp_id="<<static_cast<unsigned int>(fasp_id)<<" ch_id="<<static_cast<unsigned int>(ch_id)<<" isaux="<<static_cast<unsigned int>(isaux)<<std::endl;
    if (isaux) {
      if (!ch_id) {
        if (VERBOSE)
          cout << boost::format("    EE : fasp_id=%02d ch_id=%02d epoch=%03d\n") % static_cast<unsigned int>(fasp_id)
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
      if (fFaspMap) fasp_id = ((*fFaspMap)[mod_id])[fasp_id];

      if (lFaspOld != fasp_id) {
        // push
        if (vDigi.size()) { pushDigis(vDigi); }
        vDigi.clear();
        lFaspOld = fasp_id;
      }
      if (data & 0x1) {
        LOG(warn) << GetName() << "::unpack - Data corrupted : detect end bit set.";
        continue;
      }
      if (VERBOSE)
        cout << boost::format("    DD : fasp_id=%02d ch_id=%02d slice=%03d data=%4d\n")
                  % static_cast<unsigned int>(fasp_id) % static_cast<unsigned int>(ch_id)
                  % static_cast<unsigned int>(slice) % static_cast<unsigned int>(data >> 1);
      if (data & 0x2000) {
        LOG(debug) << GetName() << "::unpack - Self-triggered data.";
        data &= 0x1fff;
      }
      mess       = new CbmTrdFaspContent;
      mess->ch   = ch_id;
      mess->type = 1;
      mess->tlab = slice;
      mess->data = data >> 1;
      mess->fasp = lFaspOld;
      mess->cri  = mod_id;
      vDigi.push_back(mess);
    }
    //prt_wd(*wd);
  }
  return unpackOk;
}

//_____________________________________________________________
void CbmTrdUnpackFaspAlgo::prt_wd(uint32_t w)
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

ClassImp(CbmTrdUnpackFaspAlgo)
