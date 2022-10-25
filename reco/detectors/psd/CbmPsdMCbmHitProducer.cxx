/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmPsdMCbmHitProducer.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmPsdDigi.h"
#include "CbmPsdMCbmHit.h"

#include <Logger.h>

#include "TClonesArray.h"

#include <iostream>

using namespace std;


CbmPsdMCbmHitProducer::CbmPsdMCbmHitProducer()
  : FairTask("CbmPsdMCbmHitProducer")
  , fPsdHits(NULL)
  , fEventNum(0)
  , fHitError(10) /*half a module in cm*/
{
}

CbmPsdMCbmHitProducer::~CbmPsdMCbmHitProducer()
{
  FairRootManager* manager = FairRootManager::Instance();
  manager->Write();
}

void CbmPsdMCbmHitProducer::SetParContainers() {}

InitStatus CbmPsdMCbmHitProducer::Init()
{
  FairRootManager* manager = FairRootManager::Instance();

  fCbmEvents = dynamic_cast<TClonesArray*>(manager->GetObject("CbmEvent"));
  if (fCbmEvents == nullptr) { LOG(info) << GetName() << "::Init() CbmEvent NOT found \n"; }
  else {
    LOG(info) << GetName() << "::Init() CbmEvent found";
  }

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kPsd)) Fatal("CbmPsdMCbmHitProducer::Init", "No PsdDigi array!");

  fPsdHits = new TClonesArray("CbmPsdMCbmHit");  //TODO
  manager->Register("PsdHit", "PSD", fPsdHits, IsOutputBranchPersistent("PsdHit"));

  //InitMapping();

  return kSUCCESS;
}

/*
void CbmPsdMCbmHitProducer::InitMapping() //TODO change for psd
{

    string line;
    ifstream file (fMappingFile);
    if (!file.is_open()){
        std::cout<<"<CbmPsdMCbmHitProducer::InitMapping>: Unable to open mapping file:" << fMappingFile.c_str()<<std::endl;
    }

    fPsdMapping.clear();

    while ( getline (file,line) ) {

        istringstream iss(line);
        vector<std::string> results(istream_iterator<string>{iss}, std::istream_iterator<string>());
        if (results.size() != 8) continue;

        CbmRichMCbmMappingData data;
        data.fTrbId = stoi(results[0], nullptr, 16);
        data.fChannel = stoi(results[1]);
        data.fX = stod(results[6]);
        data.fY = stod(results[7]);
        data.fZ = 348.;

        data.fX -= 6.3; //Shift by 1Pmt + PmtGap + 1cm

        Int_t adr = ((data.fTrbId << 16) | (data.fChannel & 0x00FF));

       // cout <<  data.fTrbId << " " << data.fChannel << " " << data.fX << " " << data.fY << " " << adr << endl;

        fPsdMapping[adr] = data;
    }
    file.close();

    //cout << "Mapping size:" << fPsdMapping.size() <<endl;

}
*/

void CbmPsdMCbmHitProducer::Exec(Option_t* /*option*/)
{
  fEventNum++;
  LOG(info) << "CbmPsdMCbmHitProducer Event " << fEventNum;

  fPsdHits->Delete();

  // if CbmEvent does not exist then process standard event.
  // if CbmEvent exists then proceed all events in time slice.
  Int_t nUnits = (fCbmEvents != nullptr) ? fCbmEvents->GetEntriesFast() : 1;

  for (Int_t iUnit = 0; iUnit < nUnits; iUnit++) {
    CbmEvent* event = (fCbmEvents != nullptr) ? static_cast<CbmEvent*>(fCbmEvents->At(iUnit)) : nullptr;
    ProcessData(event);
  }
}

void CbmPsdMCbmHitProducer::ProcessData(CbmEvent* event)
{
  if (event != NULL) {
    LOG(info) << "CbmPsdMCbmHitProducer CbmEvent mode. CbmEvent # " << event->GetNumber();
    Int_t nofDigis = event->GetNofData(ECbmDataType::kPsdDigi);
    LOG(info) << "nofDigis: " << nofDigis;

    for (Int_t iDigi = 0; iDigi < nofDigis; iDigi++) {
      Int_t digiIndex = event->GetIndex(ECbmDataType::kPsdDigi, iDigi);
      ProcessDigi(event, digiIndex);
    }
  }
  else {
    for (Int_t iDigi = 0; iDigi < fDigiMan->GetNofDigis(ECbmModuleId::kPsd); iDigi++) {
      ProcessDigi(event, iDigi);
    }
  }
}

void CbmPsdMCbmHitProducer::ProcessDigi(CbmEvent* event, Int_t digiIndex)
{
  const CbmPsdDigi* digi = fDigiMan->Get<CbmPsdDigi>(digiIndex);
  if (digi == nullptr) return;
  if (isInEnRange(digi->GetEdep())) {
    AddHit(event, digi->GetTime(), digi->GetEdep(), digi->GetModuleID(), digi->GetSectionID(), digiIndex);
  }
}

void CbmPsdMCbmHitProducer::AddHit(CbmEvent* event, Double_t time, Double_t energy, UInt_t moduleId, UInt_t sectionId,
                                   Int_t /*index*/)
{

  Int_t nofHits = fPsdHits->GetEntriesFast();
  new ((*fPsdHits)[nofHits]) CbmPsdMCbmHit();
  CbmPsdMCbmHit* hit = (CbmPsdMCbmHit*) fPsdHits->At(nofHits);
  hit->SetEdep(energy);
  hit->SetTime(time);
  hit->SetModuleID(moduleId);
  hit->SetSectionID(sectionId);

  if (event != NULL) { event->AddData(ECbmDataType::kPsdHit, nofHits); }
}


void CbmPsdMCbmHitProducer::Finish() { fPsdHits->Clear(); }


bool CbmPsdMCbmHitProducer::isInEnRange(const double energy)
{

  if (!fDoEnCut) return true;

  if ((energy > fEnLimitLow) && (energy < fEnLimitHigh)) { return true; }
  else {
    return false;
  }
}

ClassImp(CbmPsdMCbmHitProducer)
