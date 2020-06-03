/**
 *    file CbmKresConversionReconstruction.cxx
 *
 *    author Ievgenii Kres
 *    date 04.04.2017
 *    modified 30.01.2020
 *
 *    Class dedicated to estimate from simulation how many pi^0 can be reconstructed.
 *    Loop over all global tracks -> check with MC data if particles are leptons -> combine particles in 4 pairs. If 4 particles stem from one pi^0 or eta -> store in histogram
 *    Considered are channels: g+g, dalitz, doduble-dalitz.
 *    It is kind pf "perfect PID" + "perfect matching in mother/grandmother particles"
 *    One can also see here the difference for the final reconstruction if one would use MCtrue momenta or reconstructed momenta.
 *
 **/

#include "CbmKresConversionReconstruction.h"
#include "CbmKresFunctions.h"

#include "FairRootManager.h"
#include "CbmMCTrack.h"
#include "CbmGlobalTrack.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"
#include "CbmRichRing.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TProfile2D.h"

#include <iostream>


using namespace std;

CbmKresConversionReconstruction::CbmKresConversionReconstruction()
  : fMcTracks(nullptr),
	fGlobalTracks(nullptr),
	fStsTracks(nullptr),
	fStsTrackMatches(nullptr),
	fRichRings(nullptr),
	fRichRingMatches(nullptr),
	STS_refmomentum(),
	STS_MCtrack(),
	STS_Id(),
	STS_and_RICH_refmomentum(),
	STS_and_RICH_MCtrack(),
	STS_and_RICH_Id(),
	fHistoList_reco(),
	ConversionPoints2D(nullptr),
	ConversionPoints3D(nullptr),
	fHistoList_reco_STS_gg(),
	fHistoList_reco_STS_and_RICH_gg(),
	fHistoList_reco_STS_gee(),
	fHistoList_reco_STS_and_RICH_gee(),
	fHistoList_reco_STS_eeee(),
	fHistoList_reco_STS_and_RICH_eeee(),
	STS_InvMass_eeee_mc(nullptr),
	STS_InvMass_eeee_refitted(nullptr),
	STSRICH_InvMass_eeee_mc(nullptr),
	STSRICH_InvMass_eeee_refitted(nullptr),
	STS_InvMass_gee_mc(nullptr),
	STS_InvMass_gee_refitted(nullptr),
	STS_InvMass_realg_gee_reffited(nullptr),
	STS_InvMass_img_gee_refitted(nullptr),
	STS_OpeningAngle_realg_gee_mc(nullptr),
	STS_OpeningAngle_realg_gee_reffited(nullptr),
	STS_OpeningAngle_img_gee_mc(nullptr),
	STS_OpeningAngle_img_gee_reffited(nullptr),
	STSRICH_InvMass_gee_mc(nullptr),
	STSRICH_InvMass_gee_refitted(nullptr),
	STSRICH_InvMass_realg_gee_reffited(nullptr),
	STSRICH_InvMass_img_gee_refitted(nullptr),
	STSRICH_OpeningAngle_realg_gee_mc(nullptr),
	STSRICH_OpeningAngle_realg_gee_reffited(nullptr),
	STSRICH_OpeningAngle_img_gee_mc(nullptr),
	STSRICH_OpeningAngle_img_gee_reffited(nullptr),
	STS_InvMass_gg_mc(nullptr),
	STS_InvMass_gg_reffited(nullptr),
	STS_InvMass_realg_gg_mc(nullptr),
	STS_InvMass_realg_gg_reffited(nullptr),
	STS_OpeningAngle_realg_gg_mc(nullptr),
	STS_OpeningAngle_realg_gg_reffited(nullptr),
	STS_OpeningAngle_between_gg_mc(nullptr),
	STS_OpeningAngle_between_gg_reffited(nullptr),
	STSRICH_InvMass_gg_mc(nullptr),
	STSRICH_InvMass_gg_reffited(nullptr),
	STSRICH_InvMass_realg_gg_mc(nullptr),
	STSRICH_InvMass_realg_gg_reffited(nullptr),
	STSRICH_OpeningAngle_realg_gg_mc(nullptr),
	STSRICH_OpeningAngle_realg_gg_reffited(nullptr),
	STSRICH_OpeningAngle_between_gg_mc(nullptr),
	STSRICH_OpeningAngle_between_gg_reffited(nullptr)
{

}

CbmKresConversionReconstruction::~CbmKresConversionReconstruction()
{

}

void CbmKresConversionReconstruction::Init()
{
	InitHistograms();

	FairRootManager * ioman = FairRootManager::Instance();
	if (nullptr == ioman) { Fatal("CbmKresConversionReconstruction::Init", "RootManager not instantised!"); }

	fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
	if ( nullptr == fMcTracks) { Fatal("CbmKresConversionReconstruction::Init", "No MCTrack array!"); }

	fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
	if (nullptr == fGlobalTracks) { Fatal("CbmKresConversionReconstruction::Init", "No GlobalTrack array!"); }

	fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
	if ( nullptr == fStsTracks) { Fatal("CbmKresConversionReconstruction::Init", "No StsTrack array!"); }

	fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
	if (nullptr == fStsTrackMatches) { Fatal("CbmKresConversionReconstruction::Init", "No StsTrackMatch array!"); }

	fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
	if (nullptr == fRichRings) { Fatal("CbmKresConversionReconstruction::Init", "No RichRing array!"); }

	fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
	if (nullptr == fRichRingMatches) { Fatal("CbmKresConversionReconstruction::Init", "No RichRingMatch array!"); } 
}

void CbmKresConversionReconstruction::Exec(int /*fEventNumRec*/)
{
	// cout << "CbmKresConversionReconstruction, event No. " <<  fEventNumRec << endl;

	STS_refmomentum.clear();
	STS_MCtrack.clear();
	STS_Id.clear();
	STS_and_RICH_refmomentum.clear();
	STS_and_RICH_MCtrack.clear();
	STS_and_RICH_Id.clear();

	///////   Global tracks analysis
	// ========================================================================================
	Int_t ngTracks = fGlobalTracks->GetEntriesFast();
	for (Int_t iTr = 0; iTr < ngTracks; iTr++){
		CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iTr);
		if(nullptr == gTrack) continue;
		int stsInd = gTrack->GetStsTrackIndex();
		int richInd = gTrack->GetRichRingIndex();

		// ========================================================================================
		///////   STS
		if (stsInd < 0) continue;
		CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(stsInd);
		if (stsTrack == nullptr) continue;
		CbmTrackMatchNew* stsMatch  = (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
		if (stsMatch == nullptr) continue;
		if (stsMatch->GetNofLinks() <= 0) continue;
		int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
		if (stsMcTrackId < 0) continue;
		CbmMCTrack* mcTrackSTS = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
		if (mcTrackSTS == nullptr) continue;
		int pdgSTS = mcTrackSTS->GetPdgCode();
		int motherIdSTS = mcTrackSTS->GetMotherId();
		if(TMath::Abs(pdgSTS) != 11) continue;
		///////   STS (END)
		// ========================================================================================


		if (motherIdSTS == -1) continue;
		CbmMCTrack* mcTrackMotherSTS = (CbmMCTrack*) fMcTracks->At(motherIdSTS);
		if (mcTrackMotherSTS == nullptr) continue;

		if (mcTrackMotherSTS->GetPdgCode() == 111) {

		} else if (mcTrackMotherSTS->GetPdgCode() == 22) {
			if(mcTrackMotherSTS->GetMotherId() == -1) continue;
			CbmMCTrack* mcTrackMotherOfGammaSTS = (CbmMCTrack*) fMcTracks->At(mcTrackMotherSTS->GetMotherId());
			if (mcTrackMotherOfGammaSTS->GetPdgCode() != 111) continue;
			float xConv = mcTrackSTS->GetStartX();
			float yConv = mcTrackSTS->GetStartY();
			float zConv = mcTrackSTS->GetStartZ();
			ConversionPoints3D->Fill(zConv, xConv, yConv);
			ConversionPoints2D->Fill(zConv, yConv);
		}
		else {
			continue;
		}

		TVector3 refmomentum = CbmKresFunctions::FitToVertex(stsTrack, mcTrackSTS->GetStartX(), mcTrackSTS->GetStartY(), mcTrackSTS->GetStartZ());

		STS_refmomentum.push_back(refmomentum);
		STS_MCtrack.push_back(mcTrackSTS);
		STS_Id.push_back(stsMcTrackId);


		// ========================================================================================
		///////   RICH 
		if (richInd < 0) continue; 
		CbmRichRing* richRing = (CbmRichRing*) fRichRings->At(richInd);
		if (richRing == nullptr) continue;
		CbmTrackMatchNew* richMatch  = (CbmTrackMatchNew*)fRichRingMatches->At(richInd);
		if (richMatch == nullptr) continue;
		if (richMatch->GetNofLinks() <= 0) continue;
		int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
		if (richMcTrackId < 0) continue;
		CbmMCTrack* mcTrackRICH = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
		if (mcTrackRICH == nullptr) continue;
		int pdgRICH = mcTrackRICH->GetPdgCode();
		if(TMath::Abs(pdgRICH) != 11) continue;
		///////   RICH (END)
		// ========================================================================================

		if(stsMcTrackId != richMcTrackId) continue;

		STS_and_RICH_refmomentum.push_back(refmomentum);
		STS_and_RICH_MCtrack.push_back(mcTrackSTS);
		STS_and_RICH_Id.push_back(stsMcTrackId);

	}
	///////   Global tracks analysis (END)
	// ========================================================================================

	MakeRecoAnalysis(STS_refmomentum, STS_MCtrack, STS_Id, fHistoList_reco_STS_gg, fHistoList_reco_STS_gee, fHistoList_reco_STS_eeee);
	MakeRecoAnalysis(STS_and_RICH_refmomentum, STS_and_RICH_MCtrack, STS_and_RICH_Id, fHistoList_reco_STS_and_RICH_gg, fHistoList_reco_STS_and_RICH_gee, fHistoList_reco_STS_and_RICH_eeee);


}

void CbmKresConversionReconstruction::MakeRecoAnalysis(vector<TVector3> RefMom, vector<CbmMCTrack*> MC, vector<Int_t> Id, vector<TH1*> gg, vector<TH1*> gee, vector<TH1*> eeee)
{
	Int_t fDecayedParticlePdg = 111;

	if(MC.size() < 4) return;

	for(size_t i=0; i<MC.size(); i++) {
		for(size_t j=i+1; j<MC.size(); j++) {
			for(size_t k=j+1; k<MC.size(); k++) {
				for(size_t l=k+1; l<MC.size(); l++) {

					if(MC.at(i)->GetPdgCode() + MC.at(j)->GetPdgCode() + MC.at(k)->GetPdgCode() + MC.at(l)->GetPdgCode() != 0) continue;
					
					int motherId1 = MC.at(i)->GetMotherId();
					int motherId2 = MC.at(j)->GetMotherId();
					int motherId3 = MC.at(k)->GetMotherId();
					int motherId4 = MC.at(l)->GetMotherId();

					int STSmcId1 = Id.at(i);
					int STSmcId2 = Id.at(j);
					int STSmcId3 = Id.at(k);
					int STSmcId4 = Id.at(l);
					if(STSmcId1 == STSmcId2 || STSmcId1 == STSmcId3 || STSmcId1 == STSmcId4 || STSmcId2 == STSmcId3 || STSmcId2 == STSmcId4 || STSmcId3 == STSmcId4) continue; // particle is not used twice

					///////   decay pi0 -> e+ e- e+ e-
					if(motherId1 == motherId2 && motherId1 == motherId3 && motherId1 == motherId4) {
						if(motherId1 != -1) {
							CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
							if(nullptr == mother1) continue;
							int mcMotherPdg1 = mother1->GetPdgCode();
							if(mcMotherPdg1 == fDecayedParticlePdg) { 
								Double_t InvMass_true = CbmKresFunctions::Invmass_4particles_MC(MC.at(i), MC.at(j), MC.at(k), MC.at(l));
								Double_t InvMass_reco = CbmKresFunctions::Invmass_4particles_RECO(RefMom.at(i), RefMom.at(j), RefMom.at(k), RefMom.at(l));
								cout << "Decay pi0 -> e+e-e+e- detected!\t\t mc mass: " << InvMass_true << " \t reco mass: " << InvMass_reco << endl;
								cout << "motherIds: " << motherId1 << "/" << motherId2 << "/" << motherId3 << "/" << motherId4 << "\t motherpdg: " << mcMotherPdg1 << "\t mctrack mass: " << mother1->GetMass() << endl;
								cout << "pdgs: " << MC.at(i)->GetPdgCode() << "/" << MC.at(j)->GetPdgCode() << "/" << MC.at(k)->GetPdgCode() << "/" << MC.at(l)->GetPdgCode() << endl;
								eeee[0]->Fill(InvMass_true);
								eeee[1]->Fill(InvMass_reco);

							}
						}
					}

					
					//================================== decay pi0 -> gamma(e+e-) e+e-
					if( (motherId1 == motherId2 && motherId3 == motherId4) ||
						(motherId1 == motherId3 && motherId2 == motherId4) ||
						(motherId1 == motherId4 && motherId2 == motherId3) ) {

						int grandmotherId1 = -1;
						int grandmotherId2 = -1;
						int grandmotherId3 = -1;
						int grandmotherId4 = -1;

						int mcMotherPdg1  = -1;
						int mcMotherPdg2  = -1;
						int mcMotherPdg3  = -1;
//						int mcMotherPdg4  = -1;
						int mcGrandmotherPdg1  = -1;
//						int mcGrandmotherPdg2  = -1;
//						int mcGrandmotherPdg3  = -1;
//						int mcGrandmotherPdg4  = -1;

						CbmMCTrack* grandmother1 = nullptr;

						if (motherId1 == -1 || motherId2 == -1 || motherId3 == -1 || motherId4 == -1) continue;
						CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
						if (nullptr != mother1) mcMotherPdg1 = mother1->GetPdgCode();
						grandmotherId1 = mother1->GetMotherId();
						if(grandmotherId1 != -1) {
							grandmother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
							if (nullptr != grandmother1) mcGrandmotherPdg1 = grandmother1->GetPdgCode();
						}
						CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
						if (nullptr != mother2) mcMotherPdg2 = mother2->GetPdgCode();
						grandmotherId2 = mother2->GetMotherId();
						CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
						if (nullptr != mother3) mcMotherPdg3 = mother3->GetPdgCode();
						grandmotherId3 = mother3->GetMotherId();
						CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);
//						if (nullptr != mother4) mcMotherPdg4 = mother4->GetPdgCode();
						grandmotherId4 = mother4->GetMotherId();

						if(motherId1 == motherId2 && motherId3 == motherId4) {
							if(CbmKresFunctions::NofDaughters(motherId1, MC) != 2 || CbmKresFunctions::NofDaughters(motherId3, MC) != 2) continue;
							if( (grandmotherId1 == motherId3 && mcMotherPdg3 == fDecayedParticlePdg) || (motherId1 == grandmotherId3 && mcMotherPdg1 == fDecayedParticlePdg)) {
								Double_t InvMass_true = CbmKresFunctions::Invmass_4particles_MC(MC.at(i), MC.at(j), MC.at(k), MC.at(l));
								Double_t InvMass_reco = CbmKresFunctions::Invmass_4particles_RECO(RefMom.at(i), RefMom.at(j), RefMom.at(k), RefMom.at(l));
								cout << "Decay pi0 -> gamma(e+e-) e+e- detected!\t\t mc mass: " << InvMass_true << "\t, reco mass: " << InvMass_reco << endl;
								cout << "motherids: " << motherId1 << "/" << motherId2 << "/" << motherId3 << "/" << motherId4 << endl;
								cout << "pdgs " << MC.at(i)->GetPdgCode() << "/" << MC.at(j)->GetPdgCode() << "/" << MC.at(k)->GetPdgCode() << "/" << MC.at(l)->GetPdgCode() << endl;

								gee[0]->Fill(InvMass_true);
								gee[1]->Fill(InvMass_reco);

								TVector3 start1;
								MC.at(i)->GetStartVertex(start1);
								TVector3 start2;
								MC.at(j)->GetStartVertex(start2);
								TVector3 start3;
								MC.at(k)->GetStartVertex(start3);
								TVector3 start4;
								MC.at(l)->GetStartVertex(start4);
								cout << "start: " << start1.Z() << "/" << start2.Z() << "/" << start3.Z() << "/" << start4.Z() << endl;

								if(mcGrandmotherPdg1 == fDecayedParticlePdg) {	// case: i,j = electrons from gamma, k,l = electrons from pi0
									Double_t InvMass_realg = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(j));
									Double_t InvMass_img = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(k), RefMom.at(l));
									gee[2]->Fill(InvMass_realg);
									gee[3]->Fill(InvMass_img);
									
									Double_t OpeningAngle_realg_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(j));
									Double_t OpeningAngle_realg_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(j));
									gee[4]->Fill(OpeningAngle_realg_mc);
									gee[5]->Fill(OpeningAngle_realg_refitted);
									
									Double_t OpeningAngle_img_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(k), MC.at(l));
									Double_t OpeningAngle_img_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(k), RefMom.at(l));
									gee[6]->Fill(OpeningAngle_img_mc);
									gee[7]->Fill(OpeningAngle_img_refitted);
								}
								if(mcMotherPdg1 == fDecayedParticlePdg) {	// case: i,j = electrons from pi0, k,l = electrons from gamma
									Double_t InvMass_realg = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(k), RefMom.at(l));
									Double_t InvMass_img = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(j));
									gee[2]->Fill(InvMass_realg);
									gee[3]->Fill(InvMass_img);

									Double_t OpeningAngle_realg_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(k), MC.at(l));
									Double_t OpeningAngle_realg_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(k), RefMom.at(l));
									gee[4]->Fill(OpeningAngle_realg_mc);
									gee[5]->Fill(OpeningAngle_realg_refitted);
									
									Double_t OpeningAngle_img_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(j));
									Double_t OpeningAngle_img_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(j));
									gee[6]->Fill(OpeningAngle_img_mc);
									gee[7]->Fill(OpeningAngle_img_refitted);
								}
							}
						}

						if(motherId1 == motherId3 && motherId2 == motherId4) {
							if(CbmKresFunctions::NofDaughters(motherId1, MC) != 2 || CbmKresFunctions::NofDaughters(motherId2, MC) != 2) continue;
							if( (grandmotherId1 == motherId2 && mcMotherPdg2 == fDecayedParticlePdg) || (motherId1 == grandmotherId2 && mcMotherPdg1 == fDecayedParticlePdg)) {
								Double_t InvMass_true = CbmKresFunctions::Invmass_4particles_MC(MC.at(i), MC.at(j), MC.at(k), MC.at(l));
								Double_t InvMass_reco = CbmKresFunctions::Invmass_4particles_RECO(RefMom.at(i), RefMom.at(j), RefMom.at(k), RefMom.at(l));
								cout << "Decay pi0 -> gamma(e+e-) e+e- detected!\t\t mc mass: " << InvMass_true << "\t, reco mass: " << InvMass_reco << endl;
								cout << "motherids: " << motherId1 << "/" << motherId2 << "/" << motherId3 << "/" << motherId4 << endl;
								cout << "pdgs " << MC.at(i)->GetPdgCode() << "/" << MC.at(j)->GetPdgCode() << "/" << MC.at(k)->GetPdgCode() << "/" << MC.at(l)->GetPdgCode() << endl;
								
								gee[0]->Fill(InvMass_true);
								gee[1]->Fill(InvMass_reco);

								TVector3 start1;
								MC.at(i)->GetStartVertex(start1);
								TVector3 start2;
								MC.at(j)->GetStartVertex(start2);
								TVector3 start3;
								MC.at(k)->GetStartVertex(start3);
								TVector3 start4;
								MC.at(l)->GetStartVertex(start4);
								cout << "start: " << start1.Z() << "/" << start2.Z() << "/" << start3.Z() << "/" << start4.Z() << endl;

								if(mcGrandmotherPdg1 == fDecayedParticlePdg) {	// case: i,k = electrons from gamma, j,l = electrons from pi0
									Double_t InvMass_realg = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(k));
									Double_t InvMass_img = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(j), RefMom.at(l));
									gee[2]->Fill(InvMass_realg);
									gee[3]->Fill(InvMass_img);
									
									Double_t OpeningAngle_realg_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(k));
									Double_t OpeningAngle_realg_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(k));
									gee[4]->Fill(OpeningAngle_realg_mc);
									gee[5]->Fill(OpeningAngle_realg_refitted);
									
									Double_t OpeningAngle_img_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(j), MC.at(l));
									Double_t OpeningAngle_img_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(j), RefMom.at(l));
									gee[6]->Fill(OpeningAngle_img_mc);
									gee[7]->Fill(OpeningAngle_img_refitted);
								}
								if(mcMotherPdg1 == fDecayedParticlePdg) {	// case: i,k = electrons from pi0, j,l = electrons from gamma
									Double_t InvMass_realg = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(j), RefMom.at(l));
									Double_t InvMass_img = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(k));
									gee[2]->Fill(InvMass_realg);
									gee[3]->Fill(InvMass_img);

									Double_t OpeningAngle_realg_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(j), MC.at(l));
									Double_t OpeningAngle_realg_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(j), RefMom.at(l));
									gee[4]->Fill(OpeningAngle_realg_mc);
									gee[5]->Fill(OpeningAngle_realg_refitted);
									
									Double_t OpeningAngle_img_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(k));
									Double_t OpeningAngle_img_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(k));
									gee[6]->Fill(OpeningAngle_img_mc);
									gee[7]->Fill(OpeningAngle_img_refitted);
								}
							}
						}

						if(motherId1 == motherId4 && motherId2 == motherId3) {
							if(CbmKresFunctions::NofDaughters(motherId1, MC) != 2 || CbmKresFunctions::NofDaughters(motherId2, MC) != 2) continue;
							if( (grandmotherId1 == motherId2 && mcMotherPdg2 == fDecayedParticlePdg) || (motherId1 == grandmotherId2 && mcMotherPdg1 == fDecayedParticlePdg)) {
								Double_t InvMass_true = CbmKresFunctions::Invmass_4particles_MC(MC.at(i), MC.at(j), MC.at(k), MC.at(l));
								Double_t InvMass_reco = CbmKresFunctions::Invmass_4particles_RECO(RefMom.at(i), RefMom.at(j), RefMom.at(k), RefMom.at(l));
								cout << "Decay pi0 -> gamma(e+e-) e+e- detected!\t\t mc mass: " << InvMass_true << "\t, reco mass: " << InvMass_reco << endl;
								cout << "motherids: " << motherId1 << "/" << motherId2 << "/" << motherId3 << "/" << motherId4 << endl;
								cout << "pdgs " << MC.at(i)->GetPdgCode() << "/" << MC.at(j)->GetPdgCode() << "/" << MC.at(k)->GetPdgCode() << "/" << MC.at(l)->GetPdgCode() << endl;
								
								gee[0]->Fill(InvMass_true);
								gee[1]->Fill(InvMass_reco);

								TVector3 start1;
								MC.at(i)->GetStartVertex(start1);
								TVector3 start2;
								MC.at(j)->GetStartVertex(start2);
								TVector3 start3;
								MC.at(k)->GetStartVertex(start3);
								TVector3 start4;
								MC.at(l)->GetStartVertex(start4);
								cout << "start: " << start1.Z() << "/" << start2.Z() << "/" << start3.Z() << "/" << start4.Z() << endl;

								if(mcGrandmotherPdg1 == fDecayedParticlePdg) {	// case: i,l = electrons from gamma, k,j = electrons from pi0
									Double_t InvMass_realg = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(l));
									Double_t InvMass_img = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(k), RefMom.at(j));
									gee[2]->Fill(InvMass_realg);
									gee[3]->Fill(InvMass_img);
									
									Double_t OpeningAngle_realg_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(l));
									Double_t OpeningAngle_realg_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(l));
									gee[4]->Fill(OpeningAngle_realg_mc);
									gee[5]->Fill(OpeningAngle_realg_refitted);
									
									Double_t OpeningAngle_img_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(k), MC.at(j));
									Double_t OpeningAngle_img_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(k), RefMom.at(j));
									gee[6]->Fill(OpeningAngle_img_mc);
									gee[7]->Fill(OpeningAngle_img_refitted);
								}
								if(mcMotherPdg1 == fDecayedParticlePdg) {	// case: i,l = electrons from pi0, k,j = electrons from gamma
									Double_t InvMass_realg = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(k), RefMom.at(j));
									Double_t InvMass_img = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(l));
									gee[2]->Fill(InvMass_realg);
									gee[3]->Fill(InvMass_img);

									Double_t OpeningAngle_realg_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(k), MC.at(j));
									Double_t OpeningAngle_realg_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(k), RefMom.at(j));
									gee[4]->Fill(OpeningAngle_realg_mc);
									gee[5]->Fill(OpeningAngle_realg_refitted);
									
									Double_t OpeningAngle_img_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(l));
									Double_t OpeningAngle_img_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(l));
									gee[6]->Fill(OpeningAngle_img_mc);
									gee[7]->Fill(OpeningAngle_img_refitted);
								}
							}
						}
						

						//================================== decay pi0 -> gamma gamma -> e+e- e+e-
						if(grandmotherId1 == grandmotherId2 && grandmotherId1 == grandmotherId3 && grandmotherId1 == grandmotherId4) {
							if(mcGrandmotherPdg1 != fDecayedParticlePdg) continue; 
							Double_t InvMass_true = CbmKresFunctions::Invmass_4particles_MC(MC.at(i), MC.at(j), MC.at(k), MC.at(l));
							Double_t InvMass_reco = CbmKresFunctions::Invmass_4particles_RECO(RefMom.at(i), RefMom.at(j), RefMom.at(k), RefMom.at(l));
							cout << "Decay pi0 -> gamma gamma -> e+e- e+e- detected!\t\t mc mass: " << InvMass_true << "\t, reco mass: " << InvMass_reco << endl;
							cout << "motherids: " << motherId1 << "/" << motherId2 << "/" << motherId3 << "/" << motherId4 << endl;
							cout << "grandmotherid: " << grandmotherId1 << "/" << grandmotherId2 << "/" << grandmotherId3 << "/" << grandmotherId4 << endl;
							cout << "pdgs " << MC.at(i)->GetPdgCode() << "/" << MC.at(j)->GetPdgCode() << "/" << MC.at(k)->GetPdgCode() << "/" << MC.at(l)->GetPdgCode() << endl;

							gg[0]->Fill(InvMass_true);
							gg[1]->Fill(InvMass_reco);

							cout << "\t \t mc true info: "  << endl;
							cout << "particle 1: \t" << MC.at(i)->GetPdgCode() << ";\t pt = " << MC.at(i)->GetPt() << ";\t X = " << MC.at(i)->GetStartX() << ";\t Y = " << MC.at(i)->GetStartY() << ";\t Z = " << MC.at(i)->GetStartZ() << ";\t E = " << MC.at(i)->GetEnergy() << endl;
							cout << "particle 2: \t" << MC.at(j)->GetPdgCode() << ";\t pt = " << MC.at(j)->GetPt() << ";\t X = " << MC.at(j)->GetStartX() << ";\t Y = " << MC.at(j)->GetStartY() << ";\t Z = " << MC.at(j)->GetStartZ() << ";\t E = " << MC.at(j)->GetEnergy() << endl;
							cout << "particle 3: \t" << MC.at(k)->GetPdgCode() << ";\t pt = " << MC.at(k)->GetPt() << ";\t X = " << MC.at(k)->GetStartX() << ";\t Y = " << MC.at(k)->GetStartY() << ";\t Z = " << MC.at(k)->GetStartZ() << ";\t E = " << MC.at(k)->GetEnergy() << endl;
							cout << "particle 4: \t" << MC.at(l)->GetPdgCode() << ";\t pt = " << MC.at(l)->GetPt() << ";\t X = " << MC.at(l)->GetStartX() << ";\t Y = " << MC.at(l)->GetStartY() << ";\t Z = " << MC.at(l)->GetStartZ() << ";\t E = " << MC.at(l)->GetEnergy() << endl;

							Double_t OpeningAngle1_mc = 0;
							Double_t OpeningAngle2_mc = 0;
							Double_t OpeningAngle1_refitted = 0;
							Double_t OpeningAngle2_refitted = 0;
							Double_t InvMass_realg1_mc = 0;
							Double_t InvMass_realg2_mc = 0;
							Double_t InvMass_realg1_refitted = 0;
							Double_t InvMass_realg2_refitted = 0;
						
							if(motherId1 == motherId2) {
								OpeningAngle1_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(j));
								OpeningAngle2_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(k), MC.at(l));
								gg[4]->Fill(OpeningAngle1_mc);
								gg[4]->Fill(OpeningAngle2_mc);
								OpeningAngle1_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(j));
								OpeningAngle2_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(k), RefMom.at(l));
								gg[5]->Fill(OpeningAngle1_refitted);
								gg[5]->Fill(OpeningAngle2_refitted);

								InvMass_realg1_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(i), MC.at(j));
								InvMass_realg2_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(k), MC.at(l));
								gg[2]->Fill(InvMass_realg1_mc);
								gg[2]->Fill(InvMass_realg2_mc);
								InvMass_realg1_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(j));
								InvMass_realg2_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(k), RefMom.at(l));
								gg[3]->Fill(InvMass_realg1_refitted);
								gg[3]->Fill(InvMass_realg2_refitted);

							}
							if(motherId1 == motherId3) {
								OpeningAngle1_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(k));
								OpeningAngle2_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(j), MC.at(l));
								gg[4]->Fill(OpeningAngle1_mc);
								gg[4]->Fill(OpeningAngle2_mc);
								OpeningAngle1_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(k));
								OpeningAngle2_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(j), RefMom.at(l));
								gg[5]->Fill(OpeningAngle1_refitted);
								gg[5]->Fill(OpeningAngle2_refitted);

								InvMass_realg1_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(i), MC.at(k));
								InvMass_realg2_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(j), MC.at(l));
								gg[2]->Fill(InvMass_realg1_mc);
								gg[2]->Fill(InvMass_realg2_mc);
								InvMass_realg1_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(k));
								InvMass_realg2_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(j), RefMom.at(l));
								gg[3]->Fill(InvMass_realg1_refitted);
								gg[3]->Fill(InvMass_realg2_refitted);
							}
							if(motherId1 == motherId4) {
								OpeningAngle1_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(i), MC.at(l));
								OpeningAngle2_mc = CbmKresFunctions::CalculateOpeningAngle_MC(MC.at(j), MC.at(k));
								gg[4]->Fill(OpeningAngle1_mc);
								gg[4]->Fill(OpeningAngle2_mc);
								OpeningAngle1_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(i), RefMom.at(l));
								OpeningAngle2_refitted = CbmKresFunctions::CalculateOpeningAngle_Reco(RefMom.at(j), RefMom.at(k));
								gg[5]->Fill(OpeningAngle1_refitted);
								gg[5]->Fill(OpeningAngle2_refitted);

								InvMass_realg1_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(i), MC.at(l));
								InvMass_realg2_mc = CbmKresFunctions::Invmass_2particles_MC(MC.at(j), MC.at(k));
								gg[2]->Fill(InvMass_realg1_mc);
								gg[2]->Fill(InvMass_realg2_mc);
								InvMass_realg1_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(i), RefMom.at(l));
								InvMass_realg2_refitted = CbmKresFunctions::Invmass_2particles_RECO(RefMom.at(j), RefMom.at(k));
								gg[3]->Fill(InvMass_realg1_refitted);
								gg[3]->Fill(InvMass_realg2_refitted);
							}
							Double_t openingAngleBetweenGammas = CalculateOpeningAngleBetweenGammas_MC(MC.at(i), MC.at(j), MC.at(k), MC.at(l));
							gg[6]->Fill(openingAngleBetweenGammas);
							Double_t openingAngleBetweenGammasReco = CalculateOpeningAngleBetweenGammas_Reco(RefMom.at(i), RefMom.at(j), RefMom.at(k), RefMom.at(l));
							gg[7]->Fill(openingAngleBetweenGammasReco);
						}
					}
				}
			}
		}
	}
}


Double_t CbmKresConversionReconstruction::CalculateOpeningAngleBetweenGammas_MC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2, CbmMCTrack* mctrack3, CbmMCTrack* mctrack4)
{
	Double_t openingAngle;
	TLorentzVector gamma1;
	TLorentzVector gamma2;

	if(mctrack1->GetMotherId() == mctrack2->GetMotherId() && mctrack3->GetMotherId() == mctrack4->GetMotherId()) {
		CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(mctrack1->GetMotherId());
		mother1->Get4Momentum(gamma1);
		CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(mctrack3->GetMotherId());
		mother2->Get4Momentum(gamma2);
	}
	if(mctrack1->GetMotherId() == mctrack3->GetMotherId() && mctrack2->GetMotherId() == mctrack4->GetMotherId()) {
		CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(mctrack1->GetMotherId());
		mother1->Get4Momentum(gamma1);
		CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(mctrack2->GetMotherId());
		mother2->Get4Momentum(gamma2);
	}
	if(mctrack1->GetMotherId() == mctrack4->GetMotherId() && mctrack2->GetMotherId() == mctrack3->GetMotherId()) {
		CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(mctrack1->GetMotherId());
		mother1->Get4Momentum(gamma1);
		CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(mctrack2->GetMotherId());
		mother2->Get4Momentum(gamma2);
	}

	Double_t angle = gamma1.Angle(gamma2.Vect()); 
	openingAngle = 180.*angle/TMath::Pi();

	return openingAngle;
}


Double_t CbmKresConversionReconstruction::CalculateOpeningAngleBetweenGammas_Reco(TVector3 electron1, TVector3 electron2, TVector3 electron3, TVector3 electron4)
{
	Double_t energy1 = TMath::Sqrt(electron1.Mag2() + M2E);
	TLorentzVector lorVec1(electron1, energy1);
	
	Double_t energy2 = TMath::Sqrt(electron2.Mag2() + M2E);
	TLorentzVector lorVec2(electron2, energy2);
	
	Double_t energy3 = TMath::Sqrt(electron3.Mag2() + M2E);
	TLorentzVector lorVec3(electron3, energy3);
	
	Double_t energy4 = TMath::Sqrt(electron4.Mag2() + M2E);
	TLorentzVector lorVec4(electron4, energy4);

	TLorentzVector lorPhoton1 = lorVec1 + lorVec2;
	TLorentzVector lorPhoton2 = lorVec3 + lorVec4;

	Double_t angleBetweenPhotons = lorPhoton1.Angle(lorPhoton2.Vect());
	Double_t theta = 180.*angleBetweenPhotons/TMath::Pi();

	return theta;
}


void CbmKresConversionReconstruction::Finish()
{
	gDirectory->mkdir("Reconstruction");
	gDirectory->cd("Reconstruction");

	gDirectory->mkdir("STS");
	gDirectory->cd("STS");

	gDirectory->mkdir("STS_gg");
	gDirectory->cd("STS_gg");
	for (UInt_t i = 0; i < fHistoList_reco_STS_gg.size(); i++){
		fHistoList_reco_STS_gg[i]->Write();
	}
	gDirectory->cd("..");

	gDirectory->mkdir("STS_gee");
	gDirectory->cd("STS_gee");
	for (UInt_t i = 0; i < fHistoList_reco_STS_gee.size(); i++){
		fHistoList_reco_STS_gee[i]->Write();
	}
	gDirectory->cd("..");

	gDirectory->mkdir("STS_eeee");
	gDirectory->cd("STS_eeee");
	for (UInt_t i = 0; i < fHistoList_reco_STS_eeee.size(); i++){
		fHistoList_reco_STS_eeee[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->cd("..");

	gDirectory->mkdir("STS_and_RICH");
	gDirectory->cd("STS_and_RICH");

	gDirectory->mkdir("STS_and_RICH_gg");
	gDirectory->cd("STS_and_RICH_gg");
	for (UInt_t i = 0; i < fHistoList_reco_STS_and_RICH_gg.size(); i++){
		fHistoList_reco_STS_and_RICH_gg[i]->Write();
	}
	gDirectory->cd("..");

	gDirectory->mkdir("STS_and_RICH_gee");
	gDirectory->cd("STS_and_RICH_gee");
	for (UInt_t i = 0; i < fHistoList_reco_STS_and_RICH_gee.size(); i++){
		fHistoList_reco_STS_and_RICH_gee[i]->Write();
	}
	gDirectory->cd("..");

	gDirectory->mkdir("STS_and_RICH_eeee");
	gDirectory->cd("STS_and_RICH_eeee");
	for (UInt_t i = 0; i < fHistoList_reco_STS_and_RICH_eeee.size(); i++){
		fHistoList_reco_STS_and_RICH_eeee[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->cd("..");

	for (UInt_t i = 0; i < fHistoList_reco.size(); i++){
		fHistoList_reco[i]->Write();
	}
	gDirectory->cd("..");
}


void CbmKresConversionReconstruction::InitHistograms()
{
	ConversionPoints2D = new TH2D("ConversionPoints2D", "Conversion Points 2D ; z [cm]; y [cm]", 200, -1, 200,  300, -150, 150);
	fHistoList_reco.push_back(ConversionPoints2D);
	ConversionPoints3D = new TH3D("ConversionPoints3D", "Conversion Points 3D ; z [cm]; x [cm]; y [cm]", 200, -1, 200, 300, -150, 150, 300, -150, 150);
	fHistoList_reco.push_back(ConversionPoints3D);

	///////   STS == pi -> e+ e- e+ e-
	STS_InvMass_eeee_mc = new TH1D("STS_InvMass_eeee_mc","STS_InvMass_eeee_mc; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_eeee.push_back(STS_InvMass_eeee_mc);
	STS_InvMass_eeee_refitted = new TH1D("STS_InvMass_eeee_refitted","STS_InvMass_eeee_refitted; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_eeee.push_back(STS_InvMass_eeee_refitted);

	///////   STSRICH == pi -> e+ e- e+ e-
	STSRICH_InvMass_eeee_mc = new TH1D("STSRICH_InvMass_eeee_mc","STSRICH_InvMass_eeee_mc; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_and_RICH_eeee.push_back(STSRICH_InvMass_eeee_mc);
	STSRICH_InvMass_eeee_refitted = new TH1D("STSRICH_InvMass_eeee_refitted","STSRICH_InvMass_eeee_refitted; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_and_RICH_eeee.push_back(STSRICH_InvMass_eeee_refitted);

	///////   STS == pi -> gamma(e+ e-) e+ e-     ==> dalitz
	STS_InvMass_gee_mc = new TH1D("STS_InvMass_gee_mc","STS_InvMass_gee_mc; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_gee.push_back(STS_InvMass_gee_mc);
	STS_InvMass_gee_refitted = new TH1D("STS_InvMass_gee_refitted","STS_InvMass_gee_refitted; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_gee.push_back(STS_InvMass_gee_refitted);
	STS_InvMass_realg_gee_reffited = new TH1D("STS_InvMass_realg_gee_reffited","STS_InvMass_realg_gee_reffited; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_reco_STS_gee.push_back(STS_InvMass_realg_gee_reffited);
	STS_InvMass_img_gee_refitted = new TH1D("STS_InvMass_img_gee_refitted","STS_InvMass_img_gee_refitted; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_reco_STS_gee.push_back(STS_InvMass_img_gee_refitted);
	STS_OpeningAngle_realg_gee_mc = new TH1D("STS_OpeningAngle_realg_gee_mc","STS_OpeningAngle_realg_gee_mc (between e+e- from #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_gee.push_back(STS_OpeningAngle_realg_gee_mc);
	STS_OpeningAngle_realg_gee_reffited = new TH1D("STS_OpeningAngle_realg_gee_reffited","STS_OpeningAngle_realg_gee_reffited (between e+e- from #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_gee.push_back(STS_OpeningAngle_realg_gee_reffited);
	STS_OpeningAngle_img_gee_mc = new TH1D("STS_OpeningAngle_img_gee_mc","STS_OpeningAngle_img_gee_mc (between e+e- from imaginary #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_gee.push_back(STS_OpeningAngle_img_gee_mc);
	STS_OpeningAngle_img_gee_reffited = new TH1D("STS_OpeningAngle_img_gee_reffited","STS_OpeningAngle_img_gee_reffited (between e+e- from imaginary #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_gee.push_back(STS_OpeningAngle_img_gee_reffited);

	///////   STSRICH == pi -> gamma(e+ e-) e+ e-     ==> dalitz
	STSRICH_InvMass_gee_mc = new TH1D("STSRICH_InvMass_gee_mc","STSRICH_InvMass_gee_mc; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_and_RICH_gee.push_back(STSRICH_InvMass_gee_mc);
	STSRICH_InvMass_gee_refitted = new TH1D("STSRICH_InvMass_gee_refitted","STSRICH_InvMass_gee_refitted; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_and_RICH_gee.push_back(STSRICH_InvMass_gee_refitted);
	STSRICH_InvMass_realg_gee_reffited = new TH1D("STSRICH_InvMass_realg_gee_reffited","STSRICH_InvMass_realg_gee_reffited; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_reco_STS_and_RICH_gee.push_back(STSRICH_InvMass_realg_gee_reffited);
	STSRICH_InvMass_img_gee_refitted = new TH1D("STSRICH_InvMass_img_gee_refitted","STSRICH_InvMass_img_gee_refitted; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_reco_STS_and_RICH_gee.push_back(STSRICH_InvMass_img_gee_refitted);
	STSRICH_OpeningAngle_realg_gee_mc = new TH1D("STSRICH_OpeningAngle_realg_gee_mc","STSRICH_OpeningAngle_realg_gee_mc (between e+e- from #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_and_RICH_gee.push_back(STSRICH_OpeningAngle_realg_gee_mc);
	STSRICH_OpeningAngle_realg_gee_reffited = new TH1D("STSRICH_OpeningAngle_realg_gee_reffited","STSRICH_OpeningAngle_realg_gee_reffited (between e+e- from #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_and_RICH_gee.push_back(STSRICH_OpeningAngle_realg_gee_reffited);
	STSRICH_OpeningAngle_img_gee_mc = new TH1D("STSRICH_OpeningAngle_img_gee_mc","STSRICH_OpeningAngle_img_gee_mc (between e+e- from imaginary #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_and_RICH_gee.push_back(STSRICH_OpeningAngle_img_gee_mc);
	STSRICH_OpeningAngle_img_gee_reffited = new TH1D("STSRICH_OpeningAngle_img_gee_reffited","STSRICH_OpeningAngle_img_gee_reffited (between e+e- from imaginary #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_and_RICH_gee.push_back(STSRICH_OpeningAngle_img_gee_reffited);

	///////   STS == decay pi0 -> gamma gamma -> e+e- e+e-
	STS_InvMass_gg_mc = new TH1D("STS_InvMass_gg_mc","STS_InvMass_gg_mc; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_gg.push_back(STS_InvMass_gg_mc);
	STS_InvMass_gg_reffited = new TH1D("STS_InvMass_gg_reffited","STS_InvMass_gg_reffited; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_gg.push_back(STS_InvMass_gg_reffited);
	STS_InvMass_realg_gg_mc = new TH1D("STS_InvMass_realg_gg_mc","STS_InvMass_realg_gg_mc; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_reco_STS_gg.push_back(STS_InvMass_realg_gg_mc);
	STS_InvMass_realg_gg_reffited = new TH1D("STS_InvMass_realg_gg_reffited","STS_InvMass_realg_gg_reffited; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_reco_STS_gg.push_back(STS_InvMass_realg_gg_reffited);
	STS_OpeningAngle_realg_gg_mc = new TH1D("STS_OpeningAngle_realg_gg_mc","STS_OpeningAngle_realg_gg_mc (between e+e- from #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_gg.push_back(STS_OpeningAngle_realg_gg_mc);
	STS_OpeningAngle_realg_gg_reffited = new TH1D("STS_OpeningAngle_realg_gg_reffited","STS_OpeningAngle_realg_gg_reffited (between e+e- from #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_gg.push_back(STS_OpeningAngle_realg_gg_reffited);
	STS_OpeningAngle_between_gg_mc = new TH1D("STS_OpeningAngle_between_gg_mc","STS_OpeningAngle_between_gg_mc (between #gamma#gamma from #pi^{0}); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_gg.push_back(STS_OpeningAngle_between_gg_mc);
	STS_OpeningAngle_between_gg_reffited = new TH1D("STS_OpeningAngle_between_gg_reffited","STS_OpeningAngle_between_gg_reffited (between #gamma#gamma from #pi^{0}); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_gg.push_back(STS_OpeningAngle_between_gg_reffited);

	///////   STSRICH == decay pi0 -> gamma gamma -> e+e- e+e-
	STSRICH_InvMass_gg_mc = new TH1D("STSRICH_InvMass_gg_mc","STSRICH_InvMass_gg_mc; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_and_RICH_gg.push_back(STSRICH_InvMass_gg_mc);
	STSRICH_InvMass_gg_reffited = new TH1D("STSRICH_InvMass_gg_reffited","STSRICH_InvMass_gg_reffited; invariant mass in GeV/c^{2};#", 410, -0.01, 0.4);
	fHistoList_reco_STS_and_RICH_gg.push_back(STSRICH_InvMass_gg_reffited);
	STSRICH_InvMass_realg_gg_mc = new TH1D("STSRICH_InvMass_realg_gg_mc","STSRICH_InvMass_realg_gg_mc; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_reco_STS_and_RICH_gg.push_back(STSRICH_InvMass_realg_gg_mc);
	STSRICH_InvMass_realg_gg_reffited = new TH1D("STSRICH_InvMass_realg_gg_reffited","STSRICH_InvMass_realg_gg_reffited; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_reco_STS_and_RICH_gg.push_back(STSRICH_InvMass_realg_gg_reffited);
	STSRICH_OpeningAngle_realg_gg_mc = new TH1D("STSRICH_OpeningAngle_realg_gg_mc","STSRICH_OpeningAngle_realg_gg_mc (between e+e- from #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_and_RICH_gg.push_back(STSRICH_OpeningAngle_realg_gg_mc);
	STSRICH_OpeningAngle_realg_gg_reffited = new TH1D("STSRICH_OpeningAngle_realg_gg_reffited","STSRICH_OpeningAngle_realg_gg_reffited (between e+e- from #gamma); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_and_RICH_gg.push_back(STSRICH_OpeningAngle_realg_gg_reffited);
	STSRICH_OpeningAngle_between_gg_mc = new TH1D("STSRICH_OpeningAngle_between_gg_mc","STSRICH_OpeningAngle_between_gg_mc (between #gamma#gamma from #pi^{0}); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_and_RICH_gg.push_back(STSRICH_OpeningAngle_between_gg_mc);
	STSRICH_OpeningAngle_between_gg_reffited = new TH1D("STSRICH_OpeningAngle_between_gg_reffited","STSRICH_OpeningAngle_between_gg_reffited (between #gamma#gamma from #pi^{0}); angle [deg];#", 300, -0.1, 29.9);
	fHistoList_reco_STS_and_RICH_gg.push_back(STSRICH_OpeningAngle_between_gg_reffited);
	
}
