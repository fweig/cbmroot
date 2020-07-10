
#include "CbmRichMCbmHitProducer.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmRichDetectorData.h"  // for CbmRichPmtData, CbmRichPixelData
#include "CbmRichDigi.h"
#include "CbmRichDigiMapManager.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "FairLogger.h"
#include "TClonesArray.h"


#include <iostream>

using namespace std;


CbmRichMCbmHitProducer::CbmRichMCbmHitProducer()
  : FairTask("CbmRichMCbmHitProducer")
  , fRichHits(NULL)
  , fEventNum(0)
  , fHitError(0.6 / sqrt(12.))
  ,
  //fMappingFile("mRICH_Mapping_vert_20190318.geo")
  fMappingFile("mRICH_Mapping_vert_20190318_elView.geo") {}

CbmRichMCbmHitProducer::~CbmRichMCbmHitProducer() {
  FairRootManager* manager = FairRootManager::Instance();
  manager->Write();
}

void CbmRichMCbmHitProducer::SetParContainers() {}

InitStatus CbmRichMCbmHitProducer::Init() {
  FairRootManager* manager = FairRootManager::Instance();

  fCbmEvents = dynamic_cast<TClonesArray*>(manager->GetObject("CbmEvent"));
  if (fCbmEvents == nullptr) {
    LOG(info) << ": CbmEvent NOT found \n \n \n";
  } else {
    LOG(info) << ": CbmEvent found \n \n \n";
  }

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kRich))
    Fatal("CbmRichMCbmHitProducer::Init", "No RichDigi array!");

  fRichHits = new TClonesArray("CbmRichHit");
  manager->Register(
    "RichHit", "RICH", fRichHits, IsOutputBranchPersistent("RichHit"));

  InitMapping();

  return kSUCCESS;
}

void CbmRichMCbmHitProducer::InitMapping() {

  string line;
  ifstream file(fMappingFile);
  if (!file.is_open()) {
    std::cout
      << "<CbmRichMCbmHitProducer::InitMapping>: Unable to open mapping file:"
      << fMappingFile.c_str() << std::endl;
  }

  fRichMapping.clear();

  while (getline(file, line)) {

    istringstream iss(line);
    vector<std::string> results(istream_iterator<string> {iss},
                                std::istream_iterator<string>());
    if (results.size() != 8) continue;

    CbmRichMCbmMappingData data;
    data.fTrbId   = stoi(results[0], nullptr, 16);
    data.fChannel = stoi(results[1]);
    data.fX       = stod(results[6]);
    data.fY       = stod(results[7]);
    data.fZ       = 348.;

    data.fX -= 6.3;  //Shift by 1Pmt + PmtGap + 1cm

    Int_t adr = ((data.fTrbId << 16) | (data.fChannel & 0x00FF));

    // cout <<  data.fTrbId << " " << data.fChannel << " " << data.fX << " " << data.fY << " " << adr << endl;

    fRichMapping[adr] = data;
  }
  file.close();

  //cout << "Mapping size:" << fRichMapping.size() <<endl;
}

void CbmRichMCbmHitProducer::Exec(Option_t* /*option*/) {
  fEventNum++;
  LOG(info) << "CbmRichMCbmHitProducer Event (or TimeSlice) " << fEventNum;

  fRichHits->Delete();

  // if CbmEvent does not exist then process standard event.
  // if CbmEvent exists then proceed all events in time slice.
  Int_t nUnits = (fCbmEvents != nullptr) ? fCbmEvents->GetEntriesFast() : 1;

  for (Int_t iUnit = 0; iUnit < nUnits; iUnit++) {
    CbmEvent* event = (fCbmEvents != nullptr)
                        ? static_cast<CbmEvent*>(fCbmEvents->At(iUnit))
                        : nullptr;
    ProcessData(event);
  }
}

void CbmRichMCbmHitProducer::ProcessData(CbmEvent* event) {
  if (event != NULL) {
    LOG(info) << "CbmRichMCbmHitProducer CbmEvent mode. CbmEvent # "
              << event->GetNumber();
    Int_t nofDigis = event->GetNofData(ECbmDataType::kRichDigi);
    //LOG(info) << "nofDigis: " << nofDigis;

    fNofHits = 0;
    for (Int_t iDigi = 0; iDigi < nofDigis; iDigi++) {
      Int_t digiIndex = event->GetIndex(ECbmDataType::kRichDigi, iDigi);
      ProcessDigi(event, digiIndex);
    }
    LOG(info) << "nofDigis: " << nofDigis << "\t\t "
              << "nofHits : " << fNofHits;
    fNofHits = 0;
  } else {
    for (Int_t iDigi = 0; iDigi < fDigiMan->GetNofDigis(ECbmModuleId::kRich);
         iDigi++) {
      ProcessDigi(event, iDigi);
    }
  }
}

void CbmRichMCbmHitProducer::ProcessDigi(CbmEvent* event, Int_t digiIndex) {
  const CbmRichDigi* digi = fDigiMan->Get<CbmRichDigi>(digiIndex);
  if (digi == nullptr) return;
  if (digi->GetAddress() < 0) return;
  if (isInToT(digi->GetToT())) {
    TVector3 posPoint;
    CbmRichPixelData* data =
      CbmRichDigiMapManager::GetInstance().GetPixelDataByAddress(
        digi->GetAddress());

    if (data != nullptr) {
      posPoint.SetXYZ(data->fX, data->fY, data->fZ);
    } else {
      LOG(info) << "CbmRichMCbmHitProducer: No Node for 0x" << std::hex
                << digi->GetAddress() << std::dec
                << " found. Using ASCII File! ";
      CbmRichMCbmMappingData dataAscii = fRichMapping[digi->GetAddress()];

      /* if (dataAscii == NULL) {
               LOG(error) << "CbmRichMCbmHitProducer: No Position for "<<digi->GetAddress()<<"found! ";
               return;
           }*/
      posPoint.SetXYZ(dataAscii.fX, dataAscii.fY, dataAscii.fZ);
    }

    //std::cout<<std::hex<<digi->GetAddress()<<std::dec<<"    "<<data->fX<<"   "<<data->fY<<"   "<<data->fZ<<std::endl;
    if (!RestrictToAcc(posPoint)) return;
    if (!RestrictToFullAcc(posPoint)) return;
    if (!RestrictToAerogelAccDec2019(posPoint)) return;
    AddHit(event, posPoint, digi, digiIndex, data->fPmtId);
  }
}


void CbmRichMCbmHitProducer::AddHit(CbmEvent* event,
                                    TVector3& posHit,
                                    const CbmRichDigi* digi,
                                    Int_t index,
                                    Int_t PmtId) {
  Int_t nofHits = fRichHits->GetEntriesFast();
  new ((*fRichHits)[nofHits]) CbmRichHit();
  CbmRichHit* hit = (CbmRichHit*) fRichHits->At(nofHits);
  hit->SetPosition(posHit);
  hit->SetDx(fHitError);
  hit->SetDy(fHitError);
  hit->SetRefId(index);
  hit->SetTime(digi->GetTime());
  hit->SetToT(digi->GetToT());
  hit->SetAddress(digi->GetAddress());
  hit->SetPmtId(PmtId);
  // Not Implemented in Digi: hit->SetPmtId(digi->GetPmtId());

  if (event != NULL) {
    event->AddData(ECbmDataType::kRichHit, nofHits);
    fNofHits++;
  }
}


void CbmRichMCbmHitProducer::Finish() { fRichHits->Clear(); }


bool CbmRichMCbmHitProducer::isInToT(const double ToT) {

  if (!fDoToT) return true;

  if ((ToT > fToTLimitLow) && (ToT < fToTLimitHigh)) {
    return true;
  } else {
    return false;
  }
}


bool CbmRichMCbmHitProducer::RestrictToAcc(TVector3& pos) {
  Double_t x = pos.X();
  Double_t y = pos.Y();

  return this->RestrictToAcc(x, y);
}

bool CbmRichMCbmHitProducer::RestrictToAcc(
  Double_t x,
  Double_t y) {  //check if Track is in mRICH acceptance
  if (fRestrictToAcc == false) return true;
  bool inside = false;
  if (x >= -6.25 && x <= -1.05) {
    // left part of mRICH
    if (y >= 8 && y <= 15.9) { inside = true; }
  } else if (x > -1.05 && x <= 4.25) {
    //right part
    if (y >= 8 && y <= 13.2) { inside = true; }
  }

  return inside;
}

bool CbmRichMCbmHitProducer::RestrictToFullAcc(TVector3& pos) {
  Double_t x = pos.X();
  Double_t y = pos.Y();

  return this->RestrictToFullAcc(x, y);
}

bool CbmRichMCbmHitProducer::RestrictToFullAcc(
  Double_t x,
  Double_t y) {  //check if Track is in mRICH acceptance
  if (fRestrictToFullAcc == false) return true;
  bool inside = false;
  if (x >= -16.85 && x <= 4.25) {  //TODO:check values
    // left part of mRICH
    if (y >= -23.8 && y <= 23.8) { inside = true; }
  }

  return inside;
}

bool CbmRichMCbmHitProducer::RestrictToAerogelAccDec2019(TVector3& pos) {
  Double_t x = pos.X();
  Double_t y = pos.Y();

  return this->RestrictToAerogelAccDec2019(x, y);
}

bool CbmRichMCbmHitProducer::RestrictToAerogelAccDec2019(
  Double_t x,
  Double_t y) {  //check if Track is in mRICH acceptance
  if (fRestrictToAerogelAccDec2019 == false) return true;
  //bool inside = false;

  if (y > 13.5) return false;
  if (y > 8.0 && x < 12.5) return false;

  return true;
}

ClassImp(CbmRichMCbmHitProducer)
