/**
 * \file CbmAnaConversionKF.cxx
 *
 * This class only extract some results from the KFParticle package,
 * which are relevant for the conversion analysis.
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2014
 **/

#include "CbmAnaConversionKF.h"


// included from CbmRoot
#include "FairRootManager.h"
#include "CbmKFParticleFinder.h"
#include "CbmKFParticleFinderQA.h"
#include "KFParticleTopoReconstructor.h"
#include "KFTopoPerformance.h"
#include "CbmStsTrack.h"
#include "CbmMCTrack.h"
#include "CbmTrackMatchNew.h"
#include "TDatabasePDG.h"


#define M2E 2.6112004954086e-7

using namespace std;



CbmAnaConversionKF::CbmAnaConversionKF()
 : fKFMcParticles(NULL),
   fMcTracks(NULL),
   fStsTracks(NULL),
   fStsTrackMatches(NULL),
   fKFparticle(NULL),
   fKFparticleFinderQA(NULL),
   fKFtopo(NULL),
   fKFtopoPerf(0),
   trackindexarray(),
   particlecounter(0),
   particlecounter_2daughters(0),
   particlecounter_3daughters(0),
   particlecounter_4daughters(0),
   particlecounter_all(0),
   fhPi0_NDaughters(NULL),
   fNofGeneratedPi0_allEvents(0),
   fNofPi0_kfparticle_allEvents(0),
   fNofGeneratedPi0(0),
   fNofPi0_kfparticle(0),
   fhPi0Ratio(NULL),
   fhPi0_mass(NULL),
   fSignalIds(),
   fGhostIds(),
   fHistoList_kfparticle(),
   particlevector(),
   particleMatch(),
   electronIDs(),
   gammaIDs(),
   fhInvMassPi0WithFullReco(NULL),
   fhInvMass2Gammas(NULL),
   fhInvMass2Gammas_cut(NULL),
   fhKF_particlevector(NULL),
   fhKF_trackvector(NULL),
   fhKF_NofPi0(NULL),
   fhKF_NofPi0_signal(NULL),
   fhKF_NofPi0_trackvector(NULL),
   fKF_photon_pairs(),
   fhKF_invmass_fullReco(NULL),
   timer(),
   fTime(0.)
{
}

CbmAnaConversionKF::~CbmAnaConversionKF()
{
}


void CbmAnaConversionKF::Init()
{
	FairRootManager* ioman = FairRootManager::Instance();
	if (NULL == ioman) { Fatal("CbmAnaConversionKF::Init","RootManager not instantised!"); }

	fKFMcParticles = (TClonesArray*) ioman->GetObject("KFMCParticles");
	if ( NULL == fKFMcParticles) { Fatal("CbmAnaConversionKF::Init","No KFMCParticles array!"); }

	fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
	if ( NULL == fMcTracks) { Fatal("CbmAnaConversionKF::Init","No MCTrack array!"); }

	fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
	if ( NULL == fStsTracks) { Fatal("CbmAnaConversionKF::Init","No StsTrack array!"); }

	fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
	if (NULL == fStsTrackMatches) { Fatal("CbmAnaConversionKF::Init","No StsTrackMatch array!"); }
	

	fKFtopo = fKFparticle->GetTopoReconstructor();
	fKFtopoPerf = new KFTopoPerformance;
	fKFtopoPerf->SetTopoReconstructor(fKFtopo);

	InitHistos();
}


void CbmAnaConversionKF::InitHistos()
{
	fHistoList_kfparticle.clear();

	// #############################################
	// Histograms related to KFParticle results
	fhPi0_NDaughters	= new TH1D("fhPi0_NDaughters","fhPi0_NDaughters;number of daughers;#", 4, 0.5, 4.5);
	fhPi0Ratio			= new TH1D("fhPi0Ratio", "fhPi0Ratio; ratio;#", 1000, 0., 0.02);
	fhPi0_mass			= new TH1D("fhPi0_mass", "fhPi0_mass;mass;#", 500, 0., 0.5);
	fHistoList_kfparticle.push_back(fhPi0_NDaughters);
	fHistoList_kfparticle.push_back(fhPi0Ratio);
	fHistoList_kfparticle.push_back(fhPi0_mass);

	fhInvMassPi0WithFullReco = new TH1D("fhInvMassPi0WithFullReco", "fhInvMassPi0WithFullReco;invmass;#", 400, 0., 2.);
	fHistoList_kfparticle.push_back(fhInvMassPi0WithFullReco);
	
	fhInvMass2Gammas = new TH1D("fhInvMass2Gammas", "fhInvMass2Gammas;invmass;#", 400, 0., 2.);
	fHistoList_kfparticle.push_back(fhInvMass2Gammas);
	fhInvMass2Gammas_cut = new TH1D("fhInvMass2Gammas_cut", "fhInvMass2Gammas_cut;invmass;#", 400, 0., 2.);
	fHistoList_kfparticle.push_back(fhInvMass2Gammas_cut);
	
	fhKF_particlevector = new TH1D("fhKF_particlevector", "fhKF_particlevector;;#", 10, 0., 10.);
	fHistoList_kfparticle.push_back(fhKF_particlevector);
	fhKF_particlevector->GetXaxis()->SetBinLabel(1, "electrons");
	fhKF_particlevector->GetXaxis()->SetBinLabel(2, "gamma");
	fhKF_particlevector->GetXaxis()->SetBinLabel(3, "pi0");
	fhKF_trackvector = new TH1D("fhKF_trackvector", "fhKF_trackvector;;#", 10, 0., 10.);
	fHistoList_kfparticle.push_back(fhKF_trackvector);
	fhKF_trackvector->GetXaxis()->SetBinLabel(1, "electrons");
	fhKF_trackvector->GetXaxis()->SetBinLabel(2, "gamma");
	fhKF_trackvector->GetXaxis()->SetBinLabel(3, "pi0");
	
	fhKF_NofPi0 = new TH1D("fhKF_NofPi0", "fhKF_NofPi0;nof;#", 10, -0.5, 9.5);
	fHistoList_kfparticle.push_back(fhKF_NofPi0);
	fhKF_NofPi0_signal = new TH1D("fhKF_NofPi0_signal", "fhKF_NofPi0_signal;nof;#", 10, -0.5, 9.5);
	fHistoList_kfparticle.push_back(fhKF_NofPi0_signal);
	fhKF_NofPi0_trackvector = new TH1D("fhKF_NofPi0_trackvector", "fhKF_NofPi0_trackvector;nof;#", 10, -0.5, 9.5);
	fHistoList_kfparticle.push_back(fhKF_NofPi0_trackvector);


	fhKF_invmass_fullReco = new TH1D("fhKF_invmass_fullReco", "fhKF_invmass_fullReco;invmass;#", 400, 0., 2.);
	fHistoList_kfparticle.push_back(fhKF_invmass_fullReco);

}







void CbmAnaConversionKF::Finish()
{
	//gDirectory->cd("analysis-conversion");
	gDirectory->mkdir("KF");
	gDirectory->cd("KF");
	for (UInt_t i = 0; i < fHistoList_kfparticle.size(); i++){
		fHistoList_kfparticle[i]->Write();
	}
	gDirectory->cd("..");

	cout << "CbmAnaConversionKF: Realtime - " << fTime << endl;
}




void CbmAnaConversionKF::Exec()
{
	timer.Start();
	
	electronIDs.clear();
	gammaIDs.clear();
	fKF_photon_pairs.clear();

	//KFParticle_Analysis();
	test2();

	CombineElectrons();
	CombinePhotons();

	Reconstruct();
	ReconstructGammas();

	timer.Stop();
	fTime += timer.RealTime();
}



void CbmAnaConversionKF::SetKF(CbmKFParticleFinder* kfparticle, CbmKFParticleFinderQA* kfparticleQA)
{
	fKFparticle = kfparticle;
	fKFparticleFinderQA = kfparticleQA;
	if(fKFparticle) {
		cout << "CbmAnaConversionKF: kf works" << endl;
	}
	else {
		cout << "CbmAnaConversionKF: kf works not" << endl;
	}
}


void CbmAnaConversionKF::SetSignalIds(std::vector<int> *signalids) 
{  
	fSignalIds.clear();
	fSignalIds = *signalids;
}

void CbmAnaConversionKF::SetGhostIds(std::vector<int> *ghostids) 
{
	fGhostIds.clear();
	fGhostIds = *ghostids;
}



/*
void CbmAnaConversionKF::KFParticle_Analysis()
{
	timer.Start();

	int testkf = fKFtopo->NPrimaryVertices();
	cout << "KFParticle_Analysis - test kf NPrimaryVertices: " << testkf << endl;

	const KFPTrackVector* kftrackvector;
	kftrackvector = fKFtopo->GetTracks();
	cout << "KFParticle_Analysis - trackvector: " << kftrackvector->Size() << endl;
	KFParticle testparticle;
	KFPTrack testtrack;
	const int bla = 2;
	kftrackvector->GetTrack(testtrack, bla);
	cout << "KFParticle_Analysis - test p: " << testtrack.GetP() << endl;
	kfvector_int pdgvector;
	pdgvector = kftrackvector->PDG();
	//cout << "pdg tests: " << pdgvector[1] << "\t" << pdgvector[2] << "\t" << pdgvector[3] << "\t" << pdgvector[4] << endl;
	
	
	int nofpions = 0;
	//nofpions = kftrackvector->NPions();
	cout << "KFParticle_Analysis - number of pions: " << nofpions << endl;
	
	int testpi = 0;
	for(int i=0; i<kftrackvector->Size(); i++) {
		if(TMath::Abs(pdgvector[i]) == 211) testpi++;
	}
	cout << "KFParticle_Analysis - testpi: " << testpi << endl;
	
	//kftrackvector->PrintTracks();
	
	
	vector<KFParticle> particlevector;
	particlevector = fKFtopo->GetParticles();
	

	cout << "KFParticle_Analysis - particlevector size: " << particlevector.size() << "\t";
	for(int i=0; i<particlevector.size(); i++) {
		if(particlevector[i].NDaughters() > 3) {
			cout << particlevector[i].NDaughters() << "\t";
		}
		if(particlevector[i].GetPDG() == 111) {
			cout << "blubb" << particlevector[i].NDaughters() << "\t";
			if(particlevector[i].NDaughters() > 2) {
				particlecounter++;
			}
			
			std::vector<int> daughterid;
			daughterid = particlevector[i].DaughterIds();
			cout << "daughterids: ";
			for(int j=0; j<daughterid.size(); j++) {
				cout << daughterid[j] << "/";
				cout << "pdg-" << particlevector[daughterid[j]].GetPDG() << "-";
			}
			particlecounter_all++;
		}
	}
	cout << endl;
	
	
	
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	cout << "KFParticle_Analysis - SignalIds size: " << fSignalIds.size() << "\t";
	for(int i=0; i<fSignalIds.size(); i++) {
		Int_t pdg = (fKFtopo->GetParticles()[fSignalIds[i]]).GetPDG();
		Int_t daughters = (fKFtopo->GetParticles()[fSignalIds[i]]).NDaughters();
		vector<int> daughterIds = (fKFtopo->GetParticles()[fSignalIds[i]]).DaughterIds();
		float mass = 0;
		float mass_sigma = 0;
		(fKFtopo->GetParticles()[fSignalIds[i]]).GetMass(mass, mass_sigma);
		if(pdg == 111) {
			cout << "CbmAnaConversionKF: pi0 found, signal id: " << (fKFtopo->GetParticles()[fSignalIds[i]]).Id() << "\t daughters: " << daughters << "\t";
			cout << "daughter ids: ";
			Int_t electronids[4] = {0};
			for(int j=0; j<daughterIds.size(); j++) {
				vector<int> granddaughterIds = (fKFtopo->GetParticles()[daughterIds[j]]).DaughterIds();
				cout << daughterIds[j] << " (" << granddaughterIds[0] << ",pdg" << (fKFtopo->GetParticles()[granddaughterIds[0]]).GetPDG() << "/" << granddaughterIds[1] << ",pdg" << (fKFtopo->GetParticles()[granddaughterIds[1]]).GetPDG() << ")" << " / ";
				electronids[j*2] = granddaughterIds[0];
				electronids[j*2+1] = granddaughterIds[1];
			}
			cout << endl;

			cout << "the 4 electrons and their grandmotherids: ";
			CbmMCTrack * electrontracks[4];
			for(int k=0; k<4; k++) {
				electrontracks[k] = (CbmMCTrack*) fMcTracks->At(electronids[k]);
				CbmMCTrack* mother = (CbmMCTrack*) fMcTracks->At(electrontracks[k]->GetMotherId());
				if(mother == NULL) continue;
				CbmMCTrack* grandmother = (CbmMCTrack*) fMcTracks->At(mother->GetMotherId());
				if(grandmother == NULL) continue;
				cout << "gmid" << mother->GetMotherId() << "pdg" << grandmother->GetPdgCode() << " / ";
				
			}
			cout << endl;

			particlecounter++;
			if(daughters == 2) { particlecounter_2daughters++; }
			if(daughters == 3) { particlecounter_3daughters++; }
			if(daughters == 4) { particlecounter_4daughters++; }
			fhPi0_NDaughters->Fill(daughters);
			fhPi0_mass->Fill(mass);
			fNofPi0_kfparticle++;
		}
	}
	
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	cout << "KFParticle_Analysis - GhostIds size: " << fGhostIds.size() << "\t";
	for(int i=0; i<fGhostIds.size(); i++) {
		Int_t pdg = (fKFtopo->GetParticles()[fGhostIds[i]]).GetPDG();
		Int_t daughters = (fKFtopo->GetParticles()[fGhostIds[i]]).NDaughters();
		if(pdg == 111) {
			cout << "pi0 found!\t";
			particlecounter++;
			if(daughters == 2) { particlecounter_2daughters++; }
			if(daughters == 3) { particlecounter_3daughters++; }
			if(daughters == 4) { particlecounter_4daughters++; }
			//fhPi0_NDaughters->Fill(daughters);
		}
	}
	
	cout << endl;
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	
	
	

//	trackindexarray.clear();
//	trackindexarray = fKFtopo->GetPVTrackIndexArray();
//	cout << "trackindexarray: " << trackindexarray.size() << endl;
	
//	KFVertex vertex;
//	vertex = fKFtopo->GetPrimKFVertex();
//	int testnof = 0;
//	testnof = vertex.GetNContributors();
//	cout << "nof contributors: " << testnof << endl;
	
//	KFParticle particle;
//	particle = fKFtopo->GetPrimVertex();
	
	
//	for(int i=0; i < kftrackvector->Size(); i++) {
		//const kfvector_int& tempPDG;
		//tempPDG = kftrackvector->PDG;
		//fKFVertex->At(trackindexarray[i]);
	
//	}



	test();

	timer.Stop();
	fTime += timer.RealTime();
}
*/


void CbmAnaConversionKF::test() {
	int nofparticles = fKFMcParticles->GetEntriesFast();
	cout << "CbmAnaConversionKF: test nof " << nofparticles << endl;
	for(int i=0; i<nofparticles; i++) {
		CbmMCTrack* mcTrack1 = (CbmMCTrack*) fKFMcParticles->At(i);
		int pdg = TMath::Abs(mcTrack1->GetPdgCode());
		if(pdg == 111) {
			cout << "CbmAnaConversionKF: test successful!" << endl;
		}
	}


	vector< vector<int> > ids;
	const vector<KFParticle>& particles = fKFparticle->GetTopoReconstructor()->GetParticles();
	for(unsigned int iPart=0; iPart<fSignalIds.size(); iPart++)
	{
		if(particles[fSignalIds[iPart]].GetPDG() != 111) continue;
		//some cuts on pi0 if needed
  
		const KFParticle& pi0 = particles[fSignalIds[iPart]];
		vector<int> electrons;
		for(int iGamma=0; iGamma<pi0.NDaughters(); iGamma++) {
			const int GammaID = pi0.DaughterIds()[iGamma];
			const KFParticle& Gamma = particles[GammaID];
			for(int iElectron=0; iElectron<Gamma.NDaughters(); iElectron++) {
				int ElectronID = Gamma.DaughterIds()[iElectron];
				const KFParticle& Electron = particles[ElectronID];
				int STStrackID = Electron.DaughterIds()[0];
				electrons.push_back(STStrackID);
			}
		}
		ids.push_back(electrons);
	}

	if(ids.size() > 0) {
		cout << "NEW TEST: (sts ids) ";
		for(unsigned int i=0; i<ids.size(); i++) {
			for(int j=0; j<4; j++) {
				cout << " " << ids[i][j];
			}
			cout << " | ";
		}
		cout << endl;


	cout << "MC-pdgs: ";
	CbmMCTrack* mcTracks[4];
	for(unsigned int i=0; i<ids.size(); i++) {
		for(int j=0; j<4; j++) {
			CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(ids[i][j]);
			if (stsTrack == NULL) return;
			CbmTrackMatchNew* stsMatch  = (CbmTrackMatchNew*)fStsTrackMatches->At(ids[i][j]);
			if (stsMatch == NULL) return;
			int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
			if (stsMcTrackId < 0) return;
			mcTracks[j] = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
			if (mcTracks[j] == NULL) return;
			
			CbmMCTrack* mother = (CbmMCTrack*) fMcTracks->At(mcTracks[j]->GetMotherId());
			CbmMCTrack* grandmother = (CbmMCTrack*) fMcTracks->At(mother->GetMotherId());
			
	
			cout << " " << stsMcTrackId << "/" << mcTracks[j]->GetPdgCode() << "(motherid" << mcTracks[j]->GetMotherId() << ",motherpdg" << mother->GetPdgCode() << ",grandmotherpdg" << grandmother->GetPdgCode() << ",grandmotherid" << mother->GetMotherId() << ")";
		}
	}
	cout << endl;
	Double_t mass = Invmass_4particles(mcTracks[0], mcTracks[1], mcTracks[2], mcTracks[3]);
	cout << "mass: " << mass << endl;

	}

}


Double_t CbmAnaConversionKF::Invmass_4particles(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2, const CbmMCTrack* mctrack3, const CbmMCTrack* mctrack4)
// calculation of invariant mass from four electrons/positrons
{
    TLorentzVector lorVec1;
    mctrack1->Get4Momentum(lorVec1);
    
    TLorentzVector lorVec2;
    mctrack2->Get4Momentum(lorVec2);
    
    TLorentzVector lorVec3;
    mctrack3->Get4Momentum(lorVec3);
    
    TLorentzVector lorVec4;
    mctrack4->Get4Momentum(lorVec4);
    
    
    TLorentzVector sum;
    sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;
    cout << "mc: \t" << sum.Px() << " / " << sum.Py() << " / " << sum.Pz() << " / " << sum.E() << "\t => mag = " << sum.Mag() << endl;

	return sum.Mag();
}




void CbmAnaConversionKF::test2()
{
	Int_t pi0counter_trackvector = 0;
	const KFPTrackVector* kftrackvector;
	kftrackvector = fKFtopo->GetTracks();
	Int_t kftv_size = kftrackvector->Size();
	cout << "CbmAnaConversionKF: size of kftrackvector: " << kftv_size << endl;
	
	kfvector_int vector_pdgs = kftrackvector->PDG();
	
	cout << "CbmAnaConversionKF: pdgs in trackvector: ";
	for(int i=0; i<kftv_size; i++) {
		cout << vector_pdgs[i] << " / ";
		if(TMath::Abs(vector_pdgs[i]) == 111) {
			pi0counter_trackvector++;
			fhKF_trackvector->Fill(2);
		}
		if(TMath::Abs(vector_pdgs[i]) == 11) {
			fhKF_trackvector->Fill(0);
		}
		if(TMath::Abs(vector_pdgs[i]) == 22) {
			fhKF_trackvector->Fill(1);
		}
	}
	cout << endl;
	
	fhKF_NofPi0_trackvector->Fill(pi0counter_trackvector);
	
	
	
	
	
	// particlevector to get all pi0 detected by KFParticlePackage (including signal, ghost, background); trackvector DOES NOT WORK
	particlevector.clear();
	particleMatch.clear();
	//vector<KFParticle> particlevector;







	particlevector = fKFtopo->GetParticles();


	particleMatch = fKFtopoPerf->ParticlesMatch();
	Int_t pv_size = particlevector.size();
	cout << "CbmAnaConversionKF: size of particlevector: " << pv_size << endl;
	cout << "CbmAnaConversionKF: size of particleMatch: " << particleMatch.size() << endl;

	Int_t electroncounter = 0;
	Int_t pi0counter = 0;
	Int_t pi0counter_signal = 0;
	Int_t gammacounter = 0;
	for(int i=0; i<pv_size; i++) {
		if(TMath::Abs(particlevector[i].GetPDG()) == 11) {
			electroncounter++;
			electronIDs.push_back(i);
			fhKF_particlevector->Fill(0);
		}
		if(TMath::Abs(particlevector[i].GetPDG()) == 111) {
			pi0counter++;
			fhKF_particlevector->Fill(2);
			//if(particleMatch[i].IsMatchedWithPdg()) {
			//	pi0counter_signal++;
			//}
		}
		if(TMath::Abs(particlevector[i].GetPDG()) == 22) {
			gammacounter++;
			gammaIDs.push_back(i);
			fhKF_particlevector->Fill(1);
		}
		
	}
	
	
	fhKF_NofPi0->Fill(pi0counter);
	fhKF_NofPi0_signal->Fill(pi0counter_signal);
	
	cout << "CbmAnaConversionKF: nof electrons in particlevector: " << electroncounter << endl;
	cout << "CbmAnaConversionKF: nof pi0 in particlevector: " << pi0counter << endl;
	cout << "CbmAnaConversionKF: nof gamma in particlevector: " << gammacounter << endl;
}






void CbmAnaConversionKF::CombineElectrons()
{
	Int_t nof = electronIDs.size();
	if(nof >= 2) {
		for(int a=0; a<nof-1; a++) {
			for(int b=a+1; b<nof; b++) {
				KFParticle electron1 = particlevector[electronIDs[a]];
				KFParticle electron2 = particlevector[electronIDs[b]];
				Double_t openingAngle = electron1.GetAngle(electron2);
				
				TLorentzVector lorentzE1;
				lorentzE1.SetPxPyPzE(electron1.GetPx(), electron1.GetPy(), electron1.GetPz(), electron1.GetE());
				TLorentzVector lorentzE2;
				lorentzE2.SetPxPyPzE(electron2.GetPx(), electron2.GetPy(), electron2.GetPz(), electron2.GetE());
				TLorentzVector sum = lorentzE1 + lorentzE2;
				Double_t invmass = sum.Mag();
				
				if(openingAngle < 1 && invmass < 0.03) {
					vector<int> pair; // = {a, b};
					pair.push_back(a);
					pair.push_back(b);
					fKF_photon_pairs.push_back(pair);
				}
			}
		}
	}
}


void CbmAnaConversionKF::CombinePhotons()
{
	Int_t nof = fKF_photon_pairs.size();
	if(nof >= 2) {
		for(int a=0; a<nof-1; a++) {
			for(int b=a+1; b<nof; b++) {
				Int_t electron11 = fKF_photon_pairs[a][0];
				Int_t electron12 = fKF_photon_pairs[a][1];
				Int_t electron21 = fKF_photon_pairs[b][0];
				Int_t electron22 = fKF_photon_pairs[b][1];
				
				Double_t invmass = Invmass_4particlesRECO(particlevector[electron11], particlevector[electron12], particlevector[electron21], particlevector[electron22]);
				fhKF_invmass_fullReco->Fill(invmass);
			}
		}
	}

}





void CbmAnaConversionKF::Reconstruct()
{
	Int_t nof = electronIDs.size();
	if(nof >= 4) {
		for(int a=0; a<nof-3; a++) {
			for(int b=a+1; b<nof-2; b++) {
				for(int c=b+1; c<nof-1; c++) {
					for(int d=c+1; d<nof; d++) {
						Int_t check1 = (particlevector[electronIDs[a]].GetPDG() > 0);
						Int_t check2 = (particlevector[electronIDs[b]].GetPDG() > 0);
						Int_t check3 = (particlevector[electronIDs[c]].GetPDG() > 0);
						Int_t check4 = (particlevector[electronIDs[d]].GetPDG() > 0);
						Int_t test1234 = check1 + check2 + check3 + check4;
						if(test1234 != 2) continue;		// need two electrons and two positrons
						
						
						KFParticle particle1 = particlevector[electronIDs[a]];
						KFParticle particle2 = particlevector[electronIDs[b]];
						KFParticle particle3 = particlevector[electronIDs[c]];
						KFParticle particle4 = particlevector[electronIDs[d]];
						Double_t invmass = Invmass_4particlesRECO(particle1, particle2, particle3, particle4);
						
						
						Bool_t fill = false;
						Double_t invmass_cut = 0.02;
						
						if( particle1.GetZ() == particle2.GetZ() && particle3.GetZ() == particle4.GetZ() ) {
							Double_t invmass_12 = Invmass_2electrons(particle1, particle2);
							Double_t invmass_34 = Invmass_2electrons(particle3, particle4);
							if(invmass_12 < invmass_cut && invmass_34 < invmass_cut) {
								fill = true;
							}
						}
						if( particle1.GetZ() == particle3.GetZ() && particle2.GetZ() == particle4.GetZ() ) {
							Double_t invmass_13 = Invmass_2electrons(particle1, particle3);
							Double_t invmass_24 = Invmass_2electrons(particle2, particle4);
							if(invmass_13 < invmass_cut && invmass_24 < invmass_cut) {
								fill = true;
							}
						}
						if( particle1.GetZ() == particle4.GetZ() && particle2.GetZ() == particle3.GetZ() ) {
							Double_t invmass_14 = Invmass_2electrons(particle1, particle4);
							Double_t invmass_23 = Invmass_2electrons(particle2, particle3);
							if(invmass_14 < invmass_cut && invmass_23 < invmass_cut) {
								fill = true;
							}
						}
						
						
						if(fill) fhInvMassPi0WithFullReco->Fill(invmass);
						
						/*
						CbmLmvmKinematicParams params1 = CalculateKinematicParamsReco(fElectrons_momenta[a], fElectrons_momenta[b]);
						CbmLmvmKinematicParams params2 = CalculateKinematicParamsReco(fElectrons_momenta[a], fElectrons_momenta[c]);
						CbmLmvmKinematicParams params3 = CalculateKinematicParamsReco(fElectrons_momenta[a], fElectrons_momenta[d]);
						CbmLmvmKinematicParams params4 = CalculateKinematicParamsReco(fElectrons_momenta[b], fElectrons_momenta[c]);
						CbmLmvmKinematicParams params5 = CalculateKinematicParamsReco(fElectrons_momenta[b], fElectrons_momenta[d]);
						CbmLmvmKinematicParams params6 = CalculateKinematicParamsReco(fElectrons_momenta[c], fElectrons_momenta[d]);
						
						Double_t openingAngleCut = 1;
						Int_t IsPhoton_openingAngle1 = (params1.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle2 = (params2.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle3 = (params3.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle4 = (params4.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle5 = (params5.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle6 = (params6.fAngle < openingAngleCut);
						
						Double_t invMassCut = 0.03;
						Int_t IsPhoton_invMass1 = (params1.fMinv < invMassCut);
						Int_t IsPhoton_invMass2 = (params2.fMinv < invMassCut);
						Int_t IsPhoton_invMass3 = (params3.fMinv < invMassCut);
						Int_t IsPhoton_invMass4 = (params4.fMinv < invMassCut);
						Int_t IsPhoton_invMass5 = (params5.fMinv < invMassCut);
						Int_t IsPhoton_invMass6 = (params6.fMinv < invMassCut);
						
						if(IsPhoton_openingAngle1 && IsPhoton_openingAngle6 && IsPhoton_invMass1 && IsPhoton_invMass6 && (check1 + check2 == 1) && (check3 + check4 == 1)) {
							fhElectrons_invmass_cut->Fill(invmass);
						}
						if(IsPhoton_openingAngle2 && IsPhoton_openingAngle5 && IsPhoton_invMass2 && IsPhoton_invMass5 && (check1 + check3 == 1) && (check2 + check4 == 1)) {
							fhElectrons_invmass_cut->Fill(invmass);
						}
						if(IsPhoton_openingAngle3 && IsPhoton_openingAngle4 && IsPhoton_invMass3 && IsPhoton_invMass4 && (check1 + check4 == 1) && (check2 + check3 == 1)) {
							fhElectrons_invmass_cut->Fill(invmass);
						}
						*/
						
						
					}
				}
			}
		}
	}

}


void CbmAnaConversionKF::ReconstructGammas()
{
	Int_t nof = gammaIDs.size();
	if(nof >= 2) {
		for(int a=0; a<nof-1; a++) {
			for(int b=a+1; b<nof; b++) {
				KFParticle particle1 = particlevector[gammaIDs[a]];
				KFParticle particle2 = particlevector[gammaIDs[b]];
				Double_t invmass = Invmass_2gamma(particle1, particle2);
				Double_t openingAngle = OpeningAngleBetweenPhotons(particle1, particle2);
				
				fhInvMass2Gammas->Fill(invmass);
				
				if( (TMath::Abs(particle1.GetZ() - particle2.GetZ()) < 0.005 ) && (openingAngle < 5) ) {
					fhInvMass2Gammas_cut->Fill(invmass);
				}
			}
		}
	}
}


Double_t CbmAnaConversionKF::Invmass_4particlesRECO(KFParticle part1, KFParticle part2, KFParticle part3, KFParticle part4)
// calculation of invariant mass from four electrons/positrons
{
	TVector3 momentum1(part1.GetPx(), part1.GetPy(), part1.GetPz());
    Double_t energy1 = TMath::Sqrt(momentum1.Mag2() + M2E);
    TLorentzVector lorVec1(momentum1, energy1);
    
	TVector3 momentum2(part2.GetPx(), part2.GetPy(), part2.GetPz());
    Double_t energy2 = TMath::Sqrt(momentum2.Mag2() + M2E);
    TLorentzVector lorVec2(momentum2, energy2);
    
	TVector3 momentum3(part3.GetPx(), part3.GetPy(), part3.GetPz());
    Double_t energy3 = TMath::Sqrt(momentum3.Mag2() + M2E);
    TLorentzVector lorVec3(momentum3, energy3);
    
	TVector3 momentum4(part4.GetPx(), part4.GetPy(), part4.GetPz());
    Double_t energy4 = TMath::Sqrt(momentum4.Mag2() + M2E);
    TLorentzVector lorVec4(momentum4, energy4);

    
    TLorentzVector sum;
    sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;    

	return sum.Mag();
}



Double_t CbmAnaConversionKF::Invmass_2gamma(KFParticle part1, KFParticle part2)
{
	TVector3 momentum1(part1.GetPx(), part1.GetPy(), part1.GetPz());
    Double_t energy1 = TMath::Sqrt(momentum1.Mag2() );
    TLorentzVector lorVec1(momentum1, energy1);
    
	TVector3 momentum2(part2.GetPx(), part2.GetPy(), part2.GetPz());
    Double_t energy2 = TMath::Sqrt(momentum2.Mag2() );
    TLorentzVector lorVec2(momentum2, energy2);

    
    TLorentzVector sum;
    sum = lorVec1 + lorVec2;    

	return sum.Mag();
}



Double_t CbmAnaConversionKF::Invmass_2electrons(KFParticle part1, KFParticle part2)
{
	TVector3 momentum1(part1.GetPx(), part1.GetPy(), part1.GetPz());
    Double_t energy1 = TMath::Sqrt(momentum1.Mag2() + M2E);
    TLorentzVector lorVec1(momentum1, energy1);
    
	TVector3 momentum2(part2.GetPx(), part2.GetPy(), part2.GetPz());
    Double_t energy2 = TMath::Sqrt(momentum2.Mag2() + M2E);
    TLorentzVector lorVec2(momentum2, energy2);

    
    TLorentzVector sum;
    sum = lorVec1 + lorVec2;    

	return sum.Mag();
}



Double_t CbmAnaConversionKF::OpeningAngleBetweenPhotons(KFParticle part1, KFParticle part2)
{
	TVector3 momentum1(part1.GetPx(), part1.GetPy(), part1.GetPz());
    Double_t energy1 = TMath::Sqrt(momentum1.Mag2() );
    TLorentzVector lorVec1(momentum1, energy1);
    
	TVector3 momentum2(part2.GetPx(), part2.GetPy(), part2.GetPz());
    Double_t energy2 = TMath::Sqrt(momentum2.Mag2() );
    TLorentzVector lorVec2(momentum2, energy2);


	Double_t angleBetweenPhotons = lorVec1.Angle(lorVec2.Vect());
	Double_t theta = 180.*angleBetweenPhotons/TMath::Pi();

	return theta;
}
