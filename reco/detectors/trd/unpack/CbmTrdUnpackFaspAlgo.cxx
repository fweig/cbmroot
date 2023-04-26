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
CbmTrdUnpackFaspAlgo::CbmTrdFaspMessage::CbmTrdFaspMessage(uint8_t c, uint8_t typ, uint8_t t, uint16_t d, uint8_t rob,
                                                           uint8_t asic)
  : ch(c)
  , type(typ)
  , tlab(t)
  , data(d)
  , crob(rob)
  , fasp(asic)
{
}

CbmTrdUnpackFaspAlgo::CbmTrdUnpackFaspAlgo() : CbmRecoUnpackAlgo("CbmTrdUnpackFaspAlgo") {}

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
        if (VERBOSE > 2) asic->Print();
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
      if (ifasp % 9 == 0) printf("\n");
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
void CbmTrdUnpackFaspAlgo::mess_readDW(uint32_t w, CbmTrdFaspMessage* mess)
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
void CbmTrdUnpackFaspAlgo::mess_readEW(uint32_t w, CbmTrdFaspMessage* mess)
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
void CbmTrdUnpackFaspAlgo::mess_prt(CbmTrdFaspMessage* mess)
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
bool CbmTrdUnpackFaspAlgo::pushDigis(std::vector<CbmTrdUnpackFaspAlgo::CbmTrdFaspMessage> messes)
{
  const UChar_t lFasp             = messes[0].fasp;
  const CbmTrdParFasp* faspPar    = (CbmTrdParFasp*) fAsicPar.GetAsicPar(fMod * 1000 + lFasp);
  const CbmTrdParModDigi* digiPar = (CbmTrdParModDigi*) fDigiSet->GetModulePar(fMod);

  // link data to the position on the padplane
  if (!faspPar) {
    LOG(error) << GetName() << "::pushDigis - Par for FASP " << (int) lFasp << " in module " << fMod
               << " missing. Skip.";
    return false;
  }
  if (!digiPar) {
    LOG(error) << GetName() << "::pushDigis - DIGI par for module " << fMod << " missing. Skip.";
    return false;
  }

  // TODO temporary add DAQ time calibration for FASPRO.
  // Should be absorbed in the ASIC parameter definition
  ULong64_t tdaqOffset(0);
  if (digiPar->GetPadRow(faspPar->GetPadAddress(messes[0].ch)) % 2 == 0) tdaqOffset = 3;

  if (VERBOSE) faspPar->Print();

  for (auto imess : messes) {
    const Int_t pad                     = faspPar->GetPadAddress(imess.ch);
    const CbmTrdParFaspChannel* chCalib = faspPar->GetChannel(imess.ch);
    const ULong64_t lTime               = fTime + tdaqOffset + imess.tlab;
    const UShort_t lchR                 = chCalib->HasPairingR() ? imess.data : 0;
    const UShort_t lchT                 = chCalib->HasPairingR() ? 0 : imess.data;

    if (VERBOSE) {
      const Int_t ch = 2 * pad + chCalib->HasPairingR();
      mess_prt(&imess);
      printf("fasp[%2d] ch[%4d / %2d] pad[%4d] row[%2d] col[%2d] %c[%4d]\n", lFasp, ch, imess.ch, pad,
             digiPar->GetPadRow(pad), digiPar->GetPadColumn(pad), (chCalib->HasPairingT() ? 'T' : 'R'),
             lchT > 0 ? lchT : lchR);
    }

    if (fDigiBuffer[fCrob][pad].size() == 0) {  // init pad position in map and build digi for message
      fDigiBuffer[fCrob][pad].emplace_back(pad, lchT, lchR, lTime);
      fDigiBuffer[fCrob][pad].back().SetAddressModule(fMod);
      continue;
    }

    // check if last digi has both R/T message components. Update if not and is within time window
    auto id = fDigiBuffer[fCrob][pad].rbegin();  // Should always be valid here.
                                                 // No need to extra check
    Double_t r, t;
    Int_t dt;
    const Int_t dtime = (*id).GetTimeDAQ() - lTime;
    bool use(false);

    if (TMath::Abs(dtime) < 5) {  // test message part of (last) digi
      r = (*id).GetCharge(t, dt);
      if (lchR && r < 0.1) {  // set R charge on an empty slot
        (*id).SetCharge(t, lchR, -dtime);
        use = true;
      }
      else if (lchT && t < 0.1) {  // set T charge on an empty slot
        (*id).SetCharge(lchT, r, +dtime);
        (*id).SetTimeDAQ(ULong64_t((*id).GetTimeDAQ() - dtime));
        use = true;
      }
    }

    // build digi for message when update failed
    if (!use) {
      fDigiBuffer[fCrob][pad].emplace_back(pad, lchT, lchR, lTime);
      fDigiBuffer[fCrob][pad].back().SetAddressModule(fMod);
      id = fDigiBuffer[fCrob][pad].rbegin();
    }

    // update charge for previously allocated digis to account for FASPRO ADC buffering and read-out feature
    for (++id; id != fDigiBuffer[fCrob][pad].rend(); ++id) {
      r = (*id).GetCharge(t, dt);
      if (lchR && int(r)) {  // update R charge and mark on digi
        (*id).SetCharge(t, lchR, dt);
        (*id).SetFlag(1);
        break;
      }
      else if (lchT && int(t)) {  // update T charge and mark on digi
        (*id).SetCharge(lchT, r, dt);
        (*id).SetFlag(0);
        break;
      }
    }
  }
  messes.clear();

  return true;
}

uint32_t CbmTrdUnpackFaspAlgo::ResetTimeslice()
{
  uint32_t uNbLostDigis = 0;
  /// PAL 03/08/2022: clear internal buffer at latest between two timeslices (TS are self contained!)

  for (auto crobBuffer : fDigiBuffer) {
    for (auto pad_id(0); pad_id < NFASPMOD * NFASPCH; pad_id++) {
      if (!crobBuffer.second[pad_id].size()) continue;

      LOG(warn) << fName << "::ResetTimeslice - buffered digi @ CROB=" << crobBuffer.first << " / pad=" << pad_id
                << " store " << crobBuffer.second[pad_id].size() << " unprocessed digi.";
      uNbLostDigis += crobBuffer.second[pad_id].size();

      crobBuffer.second[pad_id].clear();
    }
  }
  return uNbLostDigis;
}

void CbmTrdUnpackFaspAlgo::FinalizeComponent()
{
  Double_t r, t;
  Int_t dt;
  // push finalized digits to the next level
  for (uint16_t ipad(0); ipad < NFASPMOD * NFASPCH; ipad++) {
    if (!fDigiBuffer[fCrob][ipad].size()) continue;
    uint nIncomplete(0);
    for (auto id = fDigiBuffer[fCrob][ipad].begin(); id != fDigiBuffer[fCrob][ipad].end(); id++) {
      r = (*id).GetCharge(t, dt);
      // check if digi has all signals CORRECTED
      if (((t > 0) != (*id).IsFlagged(0)) || ((r > 0) != (*id).IsFlagged(1))) {
        nIncomplete++;
        continue;
      }
      if (fMonitor) fMonitor->FillHistos((&(*id)));
      // reset flags as they were used only to mark the correctly setting of the charge/digi
      (*id).SetFlag(0, false);
      (*id).SetFlag(1, false);
      fOutputVec.emplace_back(std::move((*id)));
    }
    // clear digi buffer wrt the digi which was forwarded to higher structures
    fDigiBuffer[fCrob][ipad].clear();
    if (nIncomplete > 2) {
      LOG(warn) << fName << "FinalizeComponent(" << fCrob << ") skip " << nIncomplete << " incomplete digi at pad "
                << ipad << ".\n";
    }
  }
  fCrob = 0xffff;  // reset current crob id
}

// ---- unpack ----
bool CbmTrdUnpackFaspAlgo::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{
  uint8_t crob_id = 0;
  uint16_t eq_id;
  bool unpackOk = true;

  auto msdesc = ts->descriptor(icomp, imslice);
  // Cast required to silence a warning on macos (there a uint64_t is a llu)
  if (VERBOSE) printf("time start %lu\n", static_cast<size_t>(msdesc.idx));
  // define time wrt start of time slice in TRD/FASP clks [80 MHz]
  fTime = ULong64_t((msdesc.idx - fTsStartTime - fSystemTimeOffset) / 12.5);

  // get MOD_id and CROB id from the equipment
  bool mapped = false;
  eq_id       = msdesc.eq_id;
  for (auto mod_id : fModuleId) {
    for (crob_id = 0; crob_id < NCROBMOD; crob_id++) {
      if (((*fCrobMap)[mod_id])[crob_id] == eq_id) break;
    }
    if (crob_id == NCROBMOD) continue;

    // found module-cri pair
    // buffer module configuration
    if (fMod == 0xffff || fMod != mod_id) {
      fMod = mod_id;
      if (!init()) { return false; }
    }
    mapped = true;
    break;
  }
  if (!mapped) { return false; }
  if (fCrob == 0xffff) fCrob = icomp;

  // Get the µslice size in bytes to calculate the number of completed words
  auto mssize = msdesc.size;

  // Get the number of complete words in the input MS buffer.
  std::uint32_t nwords = mssize / 4;  //fBytesPerWord;

  const auto mspointer = ts->content(icomp, imslice);

  // We have 32 bit spadic frames in this readout version
  const auto mscontent = reinterpret_cast<const size_t*>(mspointer);

  const uint32_t* wd = reinterpret_cast<const uint32_t*>(mscontent);


  UChar_t lFaspOld(0xff);
  vector<CbmTrdFaspMessage> vMess;
  for (uint64_t j = 0; j < nwords; j++, wd++) {
    uint32_t w      = *wd;
    uint8_t ch_id   = w & 0xf;
    uint8_t isaux   = (w >> 4) & 0x1;
    uint8_t slice   = (w >> 5) & 0x7f;
    uint16_t data   = (w >> 12) & 0x3fff;
    uint8_t fasp_id = ((w >> 26) & 0x3f) + crob_id * NFASPCROB;
    if (isaux) {
      if (!ch_id) {
        // clear buffer
        if (vMess.size()) { pushDigis(vMess); }
        vMess.clear();

        lFaspOld = 0xff;
        fTime += FASP_EPOCH_LENGTH;
      }
      else if (ch_id == 1) {
        if (VERBOSE) cout << boost::format("    PAUSE: fasp_id=%02d\n") % static_cast<unsigned int>(fasp_id);
      }
    }
    else {
      if (fFaspMap) fasp_id = ((*fFaspMap)[fMod])[fasp_id];

      if (lFaspOld != fasp_id) {
        // push
        if (vMess.size()) { pushDigis(vMess); }
        vMess.clear();
        lFaspOld = fasp_id;
      }
      if (data & 0x1) { continue; }
      if (data & 0x2000) { data &= 0x1fff; }
      vMess.emplace_back(ch_id, kData, slice, data >> 1, crob_id, lFaspOld);
    }
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
