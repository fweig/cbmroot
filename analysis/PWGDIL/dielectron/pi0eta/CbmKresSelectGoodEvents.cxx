/**
 *    file CbmKresSelectGoodEvents.cxx
 *
 *    author Ievgenii Kres
 *    date 27.03.2018 
 *    modified 30.01.2020
 *
 *    Class for selection of Geant events only when special conditions are fulfilled.
 *    It was uded to select events, where eta -> gamma + gamma -> e+ e- e+ e-    --> double conversion is happening.
 *    See thesis chapter 7.3
 *
 **/

#include "CbmKresSelectGoodEvents.h"

#include "CbmMCTrack.h"

#include "FairRootManager.h"
#include "FairRunSim.h"
#include <Logger.h>

#include <iostream>

using namespace std;

CbmKresSelectGoodEvents::CbmKresSelectGoodEvents()
  : FairTask(), fMcTracks(nullptr), fApp(nullptr) {}

CbmKresSelectGoodEvents::~CbmKresSelectGoodEvents() {}

InitStatus CbmKresSelectGoodEvents::Init() {

  FairRunSim* sim = FairRunSim::Instance();
  if (sim) { fApp = FairMCApplication::Instance(); }

  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) {
    Fatal("CbmKresEta::Init", "RootManager not instantised!");
  }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) {
    Fatal("CbmKresSelectGoodEvents::Init", "No MCTrack array!");
  }

  return kSUCCESS;
}


void CbmKresSelectGoodEvents::Exec(Option_t*) {
  Electrons.clear();
  Int_t nofMcTracks = fMcTracks->GetEntriesFast();
  for (int i = 0; i < nofMcTracks; i++) {
    CbmMCTrack* mctrack = (CbmMCTrack*) fMcTracks->At(i);
    if (mctrack == nullptr) continue;
    if (mctrack->GetMotherId() == -1) continue;
    CbmMCTrack* mcMotherTrack =
      (CbmMCTrack*) fMcTracks->At(mctrack->GetMotherId());
    if (mcMotherTrack == nullptr) continue;

    if (TMath::Abs(mctrack->GetPdgCode()) == 11
        && mcMotherTrack->GetPdgCode() == 22) {
      if (mcMotherTrack->GetMotherId() == -1) continue;
      CbmMCTrack* mcGrTrack =
        (CbmMCTrack*) fMcTracks->At(mcMotherTrack->GetMotherId());
      if (mcGrTrack == nullptr) continue;
      if (mcGrTrack->GetPdgCode() == 221) { Electrons.push_back(mctrack); }
    }
  }

  int EtaConversion = 0;
  if (Electrons.size() >= 4) {
    for (size_t i = 0; i < Electrons.size(); i++) {
      for (size_t j = i + 1; j < Electrons.size(); j++) {
        for (size_t k = j + 1; k < Electrons.size(); k++) {
          for (size_t l = k + 1; l < Electrons.size(); l++) {

            int pdg1 = Electrons.at(i)->GetPdgCode();
            int pdg2 = Electrons.at(j)->GetPdgCode();
            int pdg3 = Electrons.at(k)->GetPdgCode();
            int pdg4 = Electrons.at(l)->GetPdgCode();

            if (pdg1 + pdg2 + pdg3 + pdg4 != 0) continue;
            if (TMath::Abs(pdg1) != 11 || TMath::Abs(pdg2) != 11
                || TMath::Abs(pdg3) != 11 || TMath::Abs(pdg4) != 11)
              continue;

            int motherId1 = Electrons.at(i)->GetMotherId();
            int motherId2 = Electrons.at(j)->GetMotherId();
            int motherId3 = Electrons.at(k)->GetMotherId();
            int motherId4 = Electrons.at(l)->GetMotherId();

            if (motherId1 == -1 || motherId2 == -1 || motherId3 == -1
                || motherId4 == -1)
              continue;

            CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
            CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
            CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
            CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);

            int mcMotherPdg1 = mother1->GetPdgCode();
            int mcMotherPdg2 = mother2->GetPdgCode();
            int mcMotherPdg3 = mother3->GetPdgCode();
            int mcMotherPdg4 = mother4->GetPdgCode();

            if (mcMotherPdg1 != 22 || mcMotherPdg2 != 22 || mcMotherPdg3 != 22
                || mcMotherPdg4 != 22)
              continue;

            int grandmotherId1 = mother1->GetMotherId();
            int grandmotherId2 = mother2->GetMotherId();
            int grandmotherId3 = mother3->GetMotherId();
            int grandmotherId4 = mother4->GetMotherId();

            if (grandmotherId1 == -1) continue;
            CbmMCTrack* GrTrack = (CbmMCTrack*) fMcTracks->At(grandmotherId1);

            if (grandmotherId1 == grandmotherId2
                && grandmotherId1 == grandmotherId3
                && grandmotherId1 == grandmotherId4
                && GrTrack->GetPdgCode() == 221) {
              EtaConversion++;
              cout << "Decay eta -> gamma gamma -> e+e- e+e- detected!\t\t mc "
                      "mass: "
                   << GrTrack->GetMass() << endl;
              cout << "motherids: " << motherId1 << "/" << motherId2 << "/"
                   << motherId3 << "/" << motherId4 << endl;
              cout << "grandmotherid: " << grandmotherId1 << "/"
                   << grandmotherId2 << "/" << grandmotherId3 << "/"
                   << grandmotherId4 << endl;
            }
          }
        }
      }
    }
  }

  cout << "CbmKresSelectGoodEvents, EtaConversion = " << EtaConversion << endl;

  // if (fApp && EtaConversion == 0) {
  // 	LOG(WARNING) << "No double converted Eta";
  // 	fApp->SetSaveCurrentEvent(kFALSE);
  // }
}


void CbmKresSelectGoodEvents::Finish() {}
