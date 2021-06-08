/* Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ievgenii Kres, Florian Uhlig [committer] */

/**
 *    file CbmKresConversionBG.cxx
 *
 *    author Ievgenii Kres
 *    date 04.04.2017
 *    modified 30.01.2020
 *
 *
 *    Class for identification different types of background in the final invariant mass spectrum of pi^0/eta candidates.
 *    With a help of MCtrue information one loops over all candidates and separate it into different cases. One particle candidate can belong to several background groups at the same time.
 *
 **/

#include "CbmKresConversionBG.h"

#include "FairRootManager.h"


using namespace std;

CbmKresConversionBG::CbmKresConversionBG() {}

CbmKresConversionBG::~CbmKresConversionBG() {}

void CbmKresConversionBG::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmKresConversionBG::Init", "RootManager not instantised!"); }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) { Fatal("CbmKresConversionBG::Init", "No MCTrack array!"); }
}

void CbmKresConversionBG::Exec(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2, CbmMCTrack* mctrack3, CbmMCTrack* mctrack4,
                               Double_t invmassRecoPi0, vector<TH1*> BGCases)
{
  /*
	BGCases[0] = Case1   --> correctly reconstructed signal
	BGCases[1] = Case2   --> gammas are wrongly combined, but 4 particles come from one pi0
	BGCases[2] = Case3   --> gammas combined correctly, but not from one pi0 (combinatorial BG == Event Mixing Technique)
	BGCases[3] = Case4   --> 3 lepptons from one pi0, fourth is from somewhere
	BGCases[4] = Case5   --> one gamma correctly combined, second is wrongly
	BGCases[5] = Case6   --> both gammas are wrongly combined
	BGCases[6] = Case7   --> 1 from 4 particles are not electrons
	BGCases[7] = Case8   --> 2 from 4 particles are not electrons
	BGCases[8] = Case9   --> 3 from 4 particles are not electrons
	BGCases[9] = Case10   --> 4 from 4 particles are not electrons

	BGCases[10] = PdgCase8 
	BGCases[11] = PdgCase8mothers
	BGCases[12] = sameMIDcase8 
	BGCases[13] = sameGRIDcase8 
	BGCases[14] = Case1ZYPos 
	BGCases[15] = sameMIDcase8_mothedPDG              // useless. Almost the same like BGCases[11]
	BGCases[16] = PdgCase8NonEComeFromTarget
	BGCases[17] = PdgCase8NonE_NOT_FromTarget
	BGCases[18] = PdgCase8motherNonE 
	BGCases[19] = Case8ElFromDalitz
	BGCases[20] = Case8NonElFrom_pn 
	BGCases[21] = Case8NonElFrom_eta 
	BGCases[22] = Case8NonElFrom_kaon 
	BGCases[23] = sameMIDcase8NonEPdg
	BGCases[24] = sameMIDcase8NonEMotherPdg
	BGCases[25] = sameMIDcase8NonEMotherIM
	BGCases[26] = sameMIDcase8NonEPdgFromTarget
	BGCases[27] = sameMIDcase8NonEComeFromTargetIM
	BGCases[28] = sameMIDcase8NonEComeFromTargetP
	BGCases[29] = sameMIDcase8NonEComeFromTargetPt
	*/

  int pdg1      = mctrack1->GetPdgCode();
  int pdg2      = mctrack2->GetPdgCode();
  int pdg3      = mctrack3->GetPdgCode();
  int pdg4      = mctrack4->GetPdgCode();
  int motherId1 = mctrack1->GetMotherId();
  int motherId2 = mctrack2->GetMotherId();
  int motherId3 = mctrack3->GetMotherId();
  int motherId4 = mctrack4->GetMotherId();

  int NumOfOthers = 0;
  if (TMath::Abs(pdg1) != 11) NumOfOthers++;
  if (TMath::Abs(pdg2) != 11) NumOfOthers++;
  if (TMath::Abs(pdg3) != 11) NumOfOthers++;
  if (TMath::Abs(pdg4) != 11) NumOfOthers++;

  // cases 7-10
  if (NumOfOthers == 4) BGCases[9]->Fill(invmassRecoPi0);
  if (NumOfOthers == 3) BGCases[8]->Fill(invmassRecoPi0);
  // case 8 !!!!
  if (NumOfOthers == 2) {
    BGCases[7]->Fill(invmassRecoPi0);
    BGCases[10]->Fill(pdg1);
    BGCases[10]->Fill(pdg2);
    BGCases[10]->Fill(pdg3);
    BGCases[10]->Fill(pdg4);

    CbmMCTrack* mother1 = nullptr;
    CbmMCTrack* mother2 = nullptr;
    CbmMCTrack* mother3 = nullptr;
    CbmMCTrack* mother4 = nullptr;
    if (TMath::Abs(pdg1) == 11) mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
    if (TMath::Abs(pdg2) == 11) mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
    if (TMath::Abs(pdg3) == 11) mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
    if (TMath::Abs(pdg4) == 11) mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);

    CbmMCTrack* mother1N = nullptr;
    CbmMCTrack* mother2N = nullptr;
    CbmMCTrack* mother3N = nullptr;
    CbmMCTrack* mother4N = nullptr;
    if (TMath::Abs(pdg1) != 11 && motherId1 != -1) mother1N = (CbmMCTrack*) fMcTracks->At(motherId1);
    if (TMath::Abs(pdg2) != 11 && motherId2 != -1) mother2N = (CbmMCTrack*) fMcTracks->At(motherId2);
    if (TMath::Abs(pdg3) != 11 && motherId3 != -1) mother3N = (CbmMCTrack*) fMcTracks->At(motherId3);
    if (TMath::Abs(pdg4) != 11 && motherId4 != -1) mother4N = (CbmMCTrack*) fMcTracks->At(motherId4);

    if (nullptr != mother1) BGCases[11]->Fill(mother1->GetPdgCode());
    if (nullptr != mother2) BGCases[11]->Fill(mother2->GetPdgCode());
    if (nullptr != mother3) BGCases[11]->Fill(mother3->GetPdgCode());
    if (nullptr != mother4) BGCases[11]->Fill(mother4->GetPdgCode());

    std::vector<int> testM;
    if (TMath::Abs(pdg1) == 11) testM.push_back(motherId1);
    if (TMath::Abs(pdg2) == 11) testM.push_back(motherId2);
    if (TMath::Abs(pdg3) == 11) testM.push_back(motherId3);
    if (TMath::Abs(pdg4) == 11) testM.push_back(motherId4);

    if (testM.size() == 2 && testM[0] == testM[1]) {
      BGCases[12]->Fill(invmassRecoPi0);
      if (testM[0] != -1) {
        CbmMCTrack* Mam = (CbmMCTrack*) fMcTracks->At(testM[0]);
        BGCases[15]->Fill(Mam->GetPdgCode());
        if (Mam->GetPdgCode() == 111) BGCases[19]->Fill(invmassRecoPi0);
      }

      if (TMath::Abs(pdg1) != 11 && motherId1 != -1) {
        BGCases[23]->Fill(pdg1);
        BGCases[24]->Fill(mother1N->GetPdgCode());
        BGCases[25]->Fill(invmassRecoPi0);
      }
      if (TMath::Abs(pdg2) != 11 && motherId2 != -1) {
        BGCases[23]->Fill(pdg2);
        BGCases[24]->Fill(mother2N->GetPdgCode());
        BGCases[25]->Fill(invmassRecoPi0);
      }
      if (TMath::Abs(pdg3) != 11 && motherId3 != -1) {
        BGCases[23]->Fill(pdg3);
        BGCases[24]->Fill(mother3N->GetPdgCode());
        BGCases[25]->Fill(invmassRecoPi0);
      }
      if (TMath::Abs(pdg4) != 11 && motherId4 != -1) {
        BGCases[23]->Fill(pdg4);
        BGCases[24]->Fill(mother4N->GetPdgCode());
        BGCases[25]->Fill(invmassRecoPi0);
      }

      if (TMath::Abs(pdg1) != 11 && motherId1 == -1) {
        BGCases[26]->Fill(pdg1);
        BGCases[27]->Fill(invmassRecoPi0, 0.5);
        BGCases[28]->Fill(mctrack1->GetP());
        BGCases[29]->Fill(mctrack1->GetPt());
      }
      if (TMath::Abs(pdg2) != 11 && motherId2 == -1) {
        BGCases[26]->Fill(pdg2);
        BGCases[27]->Fill(invmassRecoPi0, 0.5);
        BGCases[28]->Fill(mctrack2->GetP());
        BGCases[29]->Fill(mctrack2->GetPt());
      }
      if (TMath::Abs(pdg3) != 11 && motherId3 == -1) {
        BGCases[26]->Fill(pdg3);
        BGCases[27]->Fill(invmassRecoPi0, 0.5);
        BGCases[28]->Fill(mctrack3->GetP());
        BGCases[29]->Fill(mctrack3->GetPt());
      }
      if (TMath::Abs(pdg4) != 11 && motherId4 == -1) {
        BGCases[26]->Fill(pdg4);
        BGCases[27]->Fill(invmassRecoPi0, 0.5);
        BGCases[28]->Fill(mctrack4->GetP());
        BGCases[29]->Fill(mctrack4->GetPt());
      }
    }

    std::vector<int> testGR;
    if (TMath::Abs(pdg1) == 11 && nullptr != mother1) testGR.push_back(mother1->GetMotherId());
    if (TMath::Abs(pdg2) == 11 && nullptr != mother2) testGR.push_back(mother2->GetMotherId());
    if (TMath::Abs(pdg3) == 11 && nullptr != mother3) testGR.push_back(mother3->GetMotherId());
    if (TMath::Abs(pdg4) == 11 && nullptr != mother4) testGR.push_back(mother4->GetMotherId());

    if (testGR.size() == 2) {
      if (testGR[0] == testGR[1]) { BGCases[13]->Fill(invmassRecoPi0); }
    }

    testGR.clear();
    testM.clear();


    if (TMath::Abs(pdg1) != 11 && motherId1 == -1) BGCases[16]->Fill(mctrack1->GetPdgCode());
    if (TMath::Abs(pdg2) != 11 && motherId2 == -1) BGCases[16]->Fill(mctrack2->GetPdgCode());
    if (TMath::Abs(pdg3) != 11 && motherId3 == -1) BGCases[16]->Fill(mctrack3->GetPdgCode());
    if (TMath::Abs(pdg4) != 11 && motherId4 == -1) BGCases[16]->Fill(mctrack4->GetPdgCode());


    if (TMath::Abs(pdg1) != 11 && nullptr != mother1N) {
      BGCases[18]->Fill(mother1N->GetPdgCode());
      BGCases[17]->Fill(pdg1);
      if (mother1N->GetPdgCode() == 2112 || mother1N->GetPdgCode() == 2212) BGCases[20]->Fill(invmassRecoPi0);
      if (mother1N->GetPdgCode() == 221) BGCases[21]->Fill(invmassRecoPi0);
      if (mother1N->GetPdgCode() == 310) BGCases[22]->Fill(invmassRecoPi0);
    }
    if (TMath::Abs(pdg2) != 11 && nullptr != mother2N) {
      BGCases[18]->Fill(mother2N->GetPdgCode());
      BGCases[17]->Fill(pdg2);
      if (mother2N->GetPdgCode() == 2112 || mother2N->GetPdgCode() == 2212) BGCases[20]->Fill(invmassRecoPi0);
      if (mother2N->GetPdgCode() == 221) BGCases[21]->Fill(invmassRecoPi0);
      if (mother2N->GetPdgCode() == 310) BGCases[22]->Fill(invmassRecoPi0);
    }
    if (TMath::Abs(pdg3) != 11 && nullptr != mother3N) {
      BGCases[18]->Fill(mother3N->GetPdgCode());
      BGCases[17]->Fill(pdg3);
      if (mother3N->GetPdgCode() == 2112 || mother3N->GetPdgCode() == 2212) BGCases[20]->Fill(invmassRecoPi0);
      if (mother3N->GetPdgCode() == 221) BGCases[21]->Fill(invmassRecoPi0);
      if (mother3N->GetPdgCode() == 310) BGCases[22]->Fill(invmassRecoPi0);
    }
    if (TMath::Abs(pdg4) != 11 && nullptr != mother4N) {
      BGCases[18]->Fill(mother4N->GetPdgCode());
      BGCases[17]->Fill(pdg4);
      if (mother4N->GetPdgCode() == 2112 || mother4N->GetPdgCode() == 2212) BGCases[20]->Fill(invmassRecoPi0);
      if (mother4N->GetPdgCode() == 221) BGCases[21]->Fill(invmassRecoPi0);
      if (mother4N->GetPdgCode() == 310) BGCases[22]->Fill(invmassRecoPi0);
    }
  }

  if (NumOfOthers == 1) BGCases[6]->Fill(invmassRecoPi0);
  // cases 1-6
  if (NumOfOthers == 0) {
    if (motherId1 != -1 && motherId2 != -1 && motherId3 != -1 && motherId4 != -1) {
      CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
      CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
      CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
      CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);
      if (nullptr != mother1 && nullptr != mother2 && nullptr != mother3 && nullptr != mother4) {
        //				int mcMotherPdg1 = mother1->GetPdgCode();
        //				int mcMotherPdg2 = mother2->GetPdgCode();
        //				int mcMotherPdg3 = mother3->GetPdgCode();
        //				int mcMotherPdg4 = mother4->GetPdgCode();
        int grandmotherId1 = mother1->GetMotherId();
        int grandmotherId2 = mother2->GetMotherId();
        int grandmotherId3 = mother3->GetMotherId();
        int grandmotherId4 = mother4->GetMotherId();
        // case 1
        if (motherId1 == motherId2 && motherId3 == motherId4 && grandmotherId1 == grandmotherId3) {
          if (grandmotherId1 != -1) {
            CbmMCTrack* grm1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
            if (nullptr != grm1
                && grm1->GetPdgCode() == 111) {  // && mctrack1->GetStartZ() < 4.5 && mctrack3->GetStartZ() < 4.5){
              BGCases[0]->Fill(invmassRecoPi0);
              BGCases[14]->Fill(mctrack1->GetStartZ(), mctrack1->GetStartY());
              BGCases[14]->Fill(mctrack3->GetStartZ(), mctrack3->GetStartY());
            }
          }
        }
        if (motherId1 != motherId2 && motherId3 != motherId4) {
          if (grandmotherId1 == grandmotherId3 && grandmotherId1 == grandmotherId4
              && grandmotherId1 == grandmotherId2) {
            BGCases[1]->Fill(invmassRecoPi0);
          }
          else {
            BGCases[5]->Fill(invmassRecoPi0);
          }
        }

        if (motherId1 == motherId2 && motherId3 == motherId4 && grandmotherId1 != grandmotherId3)
          BGCases[2]->Fill(invmassRecoPi0);

        if ((motherId1 == motherId2 && motherId3 != motherId4) || (motherId1 != motherId2 && motherId3 == motherId4)) {
          if ((grandmotherId1 == grandmotherId3 && grandmotherId1 == grandmotherId4 && grandmotherId1 != grandmotherId2)
              || (grandmotherId1 == grandmotherId2 && grandmotherId1 == grandmotherId4
                  && grandmotherId1 != grandmotherId3)
              || (grandmotherId1 == grandmotherId2 && grandmotherId1 == grandmotherId3
                  && grandmotherId1 != grandmotherId4)
              || (grandmotherId2 == grandmotherId3 && grandmotherId2 == grandmotherId4
                  && grandmotherId2 != grandmotherId1)
              || (grandmotherId2 == grandmotherId1 && grandmotherId2 == grandmotherId4
                  && grandmotherId2 != grandmotherId3)
              || (grandmotherId2 == grandmotherId1 && grandmotherId2 == grandmotherId3
                  && grandmotherId2 != grandmotherId4)
              || (grandmotherId3 == grandmotherId1 && grandmotherId3 == grandmotherId2
                  && grandmotherId3 != grandmotherId4)
              || (grandmotherId3 == grandmotherId1 && grandmotherId3 == grandmotherId4
                  && grandmotherId3 != grandmotherId2)
              || (grandmotherId3 == grandmotherId2 && grandmotherId3 == grandmotherId4
                  && grandmotherId3 != grandmotherId1)
              || (grandmotherId4 == grandmotherId1 && grandmotherId4 == grandmotherId2
                  && grandmotherId4 != grandmotherId3)
              || (grandmotherId4 == grandmotherId1 && grandmotherId4 == grandmotherId3
                  && grandmotherId4 != grandmotherId2)
              || (grandmotherId4 == grandmotherId2 && grandmotherId4 == grandmotherId3
                  && grandmotherId4 != grandmotherId1)) {
            BGCases[3]->Fill(invmassRecoPi0);
          }
          else {
            BGCases[4]->Fill(invmassRecoPi0);
          }
        }
      }
    }
  }
}
