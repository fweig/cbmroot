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

// select verbosity level
// 0 : none
// 1 : data unpacker
// 2 : digi packing
#define VERBOSE 0

using namespace std;

CbmTrdUnpackFaspAlgo::CbmTrdUnpackFaspAlgo()
  : CbmRecoUnpackAlgo("CbmTrdUnpackFaspAlgo")
  , fTime(0)
  , fModuleId()
  , fAsicPar()
{
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
void CbmTrdUnpackFaspAlgo::SetCrobMapping(const std::map<uint32_t, uint16_t[NCROBMOD]>& map)
{
  if (fCrobMap) delete fCrobMap;
  fCrobMap = new std::map<uint32_t, uint16_t[NCROBMOD]>(map);
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
  mess->crob = (wd >> shift) & 0x3f;
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
    cout << boost::format("    EPOCH: crob_id=%02d ch_id=%02d epoch=%05d\n") % static_cast<unsigned int>(mess->crob)
              % static_cast<unsigned int>(mess->ch) % static_cast<unsigned int>(mess->epoch);
}

//_________________________________________________________________________________
bool CbmTrdUnpackFaspAlgo::pushDigis(std::vector<CbmTrdUnpackFaspAlgo::CbmTrdFaspContent*> messes)
{
  UChar_t lFasp(0xff);
  UShort_t lchR, lchT;
  Double_t r, t;
  Int_t dt, dtime, ch, pad;
  ULong64_t lTime, tdaqOffset(0);
  CbmTrdParFasp* faspPar(nullptr);
  const CbmTrdParFaspChannel* chCalib(nullptr);
  CbmTrdParModDigi* digiPar(nullptr);
  for (auto imess : messes) {
    if (lFasp == 0xff) {
      lFasp = messes[0]->fasp;
      // link data to the position on the padplane
      if (!(faspPar = (CbmTrdParFasp*) fAsicPar.GetAsicPar(imess->mod * 1000 + lFasp))) {
        LOG(error) << GetName() << "::pushDigis - Par for FASP " << (int) lFasp << " in module " << imess->mod
                   << " missing. Skip.";
        return false;
      }
      if (!(digiPar = (CbmTrdParModDigi*) fDigiSet->GetModulePar(imess->mod))) {
        LOG(error) << GetName() << "::pushDigis - DIGI par for module " << imess->mod << " missing. Skip.";
        return false;
      }
      // TODO temporary add DAQ time calibration for FASPRO.
      // Should be absorbed in the ASIC parameter definition
      if (digiPar->GetPadRow(faspPar->GetChannelAddress(imess->ch)) % 2) tdaqOffset = 3;

      if (VERBOSE) faspPar->Print();
    }
    if (VERBOSE) mess_prt(imess);

    pad     = faspPar->GetChannelAddress(imess->ch);
    chCalib = faspPar->GetChannel(imess->ch);
    ch      = 2 * pad + chCalib->HasPairingR();
    lTime   = fTime + tdaqOffset + imess->tlab;
    lchR    = 0;
    lchT    = 0;
    if (chCalib->HasPairingR()) lchR = imess->data;
    else
      lchT = imess->data;
    if (VERBOSE)
      printf("fasp[%2d] ch[%4d / %2d] pad[%4d] row[%2d] col[%2d] %c[%4d]\n", lFasp, ch, imess->ch, pad,
             digiPar->GetPadRow(pad), digiPar->GetPadColumn(pad), (chCalib->HasPairingT() ? 'T' : 'R'),
             lchT > 0 ? lchT : lchR);

    if (fDigiBuffer.find(pad) != fDigiBuffer.end()) {
      // check if last digi has both R/T message components. Update if not and is within time window
      auto id = fDigiBuffer[pad].rbegin();
      dtime   = (*id)->GetTimeDAQ() - lTime;
      bool use(false);
      if (TMath::Abs(dtime) < 5) {  // test message part of (last) digi
        r = (*id)->GetCharge(t, dt);
        if (lchR && r < 0.1) {  // set R charge on an empty slot
          (*id)->SetCharge(t, lchR, -dtime);
          use = true;
        }
        else if (lchT && t < 0.1) {  // set T charge on an empty slot
          (*id)->SetCharge(lchT, r, +dtime);
          (*id)->SetTimeDAQ(ULong64_t((*id)->GetTimeDAQ() - dtime));
          use = true;
        }
      }

      // build digi for message when update failed
      if (!use) {
        CbmTrdDigi* digi = new CbmTrdDigi(pad, lchT, lchR, lTime);
        digi->SetAddressModule(imess->mod);
        fDigiBuffer[pad].push_back(digi);
        id = fDigiBuffer[pad].rbegin();
      }

      if (id != fDigiBuffer[pad].rend()) id++;

      // update charge for previously allocated digis to account for FASPRO ADC buffering and read-out problem
      use = false;
      for (; id != fDigiBuffer[pad].rend(); ++id) {
        r = (*id)->GetCharge(t, dt);
        if (lchR && int(r)) {  // update R charge and mark on digi
          (*id)->SetCharge(t, lchR, dt);
          (*id)->SetFlag(1);
          use = true;
        }
        else if (lchT && int(t)) {  // update T charge and mark on digi
          (*id)->SetCharge(lchT, r, dt);
          (*id)->SetFlag(0);
          use = true;
        }
        if (use) break;
      }
    }
    else {  // init pad position in map and build digi for message
      CbmTrdDigi* digi = new CbmTrdDigi(pad, lchT, lchR, lTime);
      digi->SetAddressModule(imess->mod);
      fDigiBuffer[pad].push_back(digi);
    }
    delete imess;
  }

  // push finalized digits to the next level
  for (int jd(0); jd < NFASPCH; jd += 2) {
    int ipad(faspPar->GetChannelAddress(jd));
    if (fDigiBuffer.find(ipad) == fDigiBuffer.end()) continue;

    for (auto id = fDigiBuffer[ipad].begin(); id != fDigiBuffer[ipad].end(); id++) {
      r = (*id)->GetCharge(t, dt);
      // check if digi has all signals CORRECTED
      if (((t > 0) != (*id)->IsFlagged(0)) || ((r > 0) != (*id)->IsFlagged(1))) continue;
      if (fMonitor) fMonitor->FillHistos(((*id)));
      // reset flags as they were used only to mark the correctly setting of the charge/digi
      (*id)->SetFlag(0, false);
      (*id)->SetFlag(1, false);
      fOutputVec.emplace_back(*std::move((*id)));
      // clear digi buffer wrt the digi which was forwarded to higher structures
      id = fDigiBuffer[ipad].erase(id);
    }
  }
  messes.clear();

  return true;
}

// ---- unpack ----
bool CbmTrdUnpackFaspAlgo::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{
  if (VERBOSE) printf("CbmTrdUnpackFaspAlgo::unpack 0x%04x %d\n", icomp, imslice);
  //LOG(info) << "Component " << icomp << " connected to config CbmTrdUnpackConfig2D. Slice "<<imslice;

  uint32_t mod_id = 5;
  uint8_t crob_id = 0;
  uint16_t eq_id;
  bool unpackOk   = true;
  //Double_t fdMsSizeInNs = 1.28e6;

  auto msdesc = ts->descriptor(icomp, imslice);
  // Cast required to silence a warning on macos (there a uint64_t is a llu)
  if (VERBOSE) printf("time start %lu\n", static_cast<size_t>(msdesc.idx));
  // define time wrt start of time slice in TRD/FASP clks [80 MHz]
  fTime = ULong64_t((msdesc.idx - fTsStartTime - fSystemTimeOffset) / 12.5);
  eq_id = msdesc.eq_id;  // read the CROB id
  for (; crob_id < NCROBMOD; crob_id++) {
    if (((*fCrobMap)[mod_id])[crob_id] == eq_id) break;
  }
  if (crob_id == NCROBMOD) {
    LOG(error) << GetName() << "::unpack - CROB eq_id=" << eq_id << " not registered in the unpacker.";
    return false;
  }

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
    uint8_t fasp_id = ((w >> 26) & 0x3f) + crob_id * NFASPCROB;
    // std::cout<<"fasp_id="<<static_cast<unsigned int>(fasp_id)<<" ch_id="<<static_cast<unsigned int>(ch_id)<<" isaux="<<static_cast<unsigned int>(isaux)<<std::endl;
    if (isaux) {
      if (!ch_id) {
        // clear buffer
        if (vDigi.size()) { pushDigis(vDigi); }
        vDigi.clear();

        if (VERBOSE)
          cout << boost::format("    EE : fasp_id=%02d ch_id=%02d epoch=%03d\n") % static_cast<unsigned int>(fasp_id)
                    % static_cast<unsigned int>(ch_id) % static_cast<unsigned int>(epoch);

        lFaspOld = 0xff;
        fTime += FASP_EPOCH_LENGTH;
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
      mess->type = kData;
      mess->tlab = slice;
      mess->data = data >> 1;
      mess->fasp = lFaspOld;
      mess->mod  = mod_id;
      mess->crob = crob_id;
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
