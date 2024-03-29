/* Copyright (C) 2017-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaUnigenSource.cxx
 *
 *  Created on: 27 mar 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaUnigenSource.h"

#include <FairRootManager.h>
#include <Logger.h>

#include <TBranch.h>

#include <fstream>
#include <iostream>

#include "UEvent.h"
#include "UParticle.h"


NicaUnigenSource::NicaUnigenSource() : fUnigenChain(NULL), fEvent(NULL) {}

NicaUnigenSource::NicaUnigenSource(TString inFile) : fUnigenChain(NULL), fEvent(NULL) { fFileName.push_back(inFile); }

NicaUnigenSource::~NicaUnigenSource()
{
  if (fUnigenChain) delete fUnigenChain;
}

Bool_t NicaUnigenSource::Init()
{
  FairRootManager* mngr = FairRootManager::Instance();
  fUnigenChain          = new TChain("events");
  if (fFileName[0].EndsWith(".root")) {
    for (auto i : fFileName) {
      LOG(debug) << "NicaUnigenSource: opening single file" << i;
      fUnigenChain->Add(i);
    }
  }
  else {  // this is long list
    std::ifstream list;
    list.open(fFileName[0]);
    do {
      TString temp;
      list >> temp;
      if (temp.Length() > 1) { fUnigenChain->Add(temp); }
      else {
        break;
      }
      LOG(debug) << "Adding file " << temp << " to chain";
    } while (!list.eof());
    list.close();
  }

  fEvent = new UEvent();
  if (fUnigenChain->GetBranch("event")) {
    fUnigenChain->Print();
    fUnigenChain->SetBranchStatus("event", 1);
    fUnigenChain->SetBranchAddress("event", &fEvent);
  }
  else {
    std::cout << "Event read II" << std::endl;
    fUnigenChain->SetBranchStatus("UEvent.", 1);
    fUnigenChain->SetBranchAddress("UEvent.", &fEvent);
  }
  mngr->SetInChain(fUnigenChain, -1);
  mngr->Register("UEvent.", "UEvent", (TNamed*) fEvent, kFALSE);
  return kTRUE;
}

Int_t NicaUnigenSource::ReadEvent(UInt_t unsignedInt)
{
  //std::cout<<"READING EVENT " <<unsignedInt<<std::endl;
  fUnigenChain->GetEntry(unsignedInt);
  // std::cout<<"xxx"<<std::endl;
  return 0;
}

void NicaUnigenSource::Close() {}

void NicaUnigenSource::Boost(Double_t vx, Double_t vy, Double_t vz)
{
  for (int i = 0; i < fEvent->GetNpa(); i++) {
    UParticle* p       = fEvent->GetParticle(i);
    TLorentzVector mom = p->GetMomentum();
    TLorentzVector pos = p->GetPosition();
    mom.Boost(vx, vy, vz);
    pos.Boost(vx, vy, vz);
    p->SetMomentum(mom);
    p->SetPosition(pos);
  }
}

Int_t NicaUnigenSource::CheckMaxEventNo(Int_t /*int1*/) { return fUnigenChain->GetEntries(); }
