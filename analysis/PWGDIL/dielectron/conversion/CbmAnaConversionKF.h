/**
 * \file CbmAnaConversionKF.h
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2014
 **/
 
 
#ifndef CBM_ANA_CONVERSION_KF
#define CBM_ANA_CONVERSION_KF

// included from ROOT
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TClonesArray.h>
#include <TStopwatch.h>

// included from CbmRoot
#include "CbmMCTrack.h"
#include "CbmVertex.h"
#include "CbmKFParticleFinder.h"
#include "CbmKFParticleFinderQA.h"
#include "CbmKFParticle.h"
#include "KFParticle.h"
#include "KFPartMatch.h"
#include "KFTopoPerformance.h"

#include <vector>

class CbmAnaConversionKF
{

public:
	CbmAnaConversionKF();
	virtual ~CbmAnaConversionKF();

	void Init();
	void InitHistos();
	void Finish();
	void Exec();

	//void KFParticle_Analysis();

	void SetKF(CbmKFParticleFinder* kfparticle, CbmKFParticleFinderQA* kfparticleQA);
	void SetSignalIds(std::vector<int> *signalids);
	void SetGhostIds(std::vector<int> *ghostids);


	void test();
	Double_t Invmass_4particles(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2, const CbmMCTrack* mctrack3, const CbmMCTrack* mctrack4);

	void test2();
	void CombineElectrons();
	void CombinePhotons();
	void Reconstruct();
	void ReconstructGammas();
	Double_t Invmass_4particlesRECO(KFParticle part1, KFParticle part2, KFParticle part3, KFParticle part4);
	Double_t Invmass_2gamma(KFParticle part1, KFParticle part2);
	Double_t Invmass_2electrons(KFParticle part1, KFParticle part2);

	Double_t OpeningAngleBetweenPhotons(KFParticle part1, KFParticle part2);




private:
	TClonesArray* fKFMcParticles;
	TClonesArray* fMcTracks;
	TClonesArray* fStsTracks;
	TClonesArray* fStsTrackMatches;

	CbmKFParticleFinder* fKFparticle;
	CbmKFParticleFinderQA* fKFparticleFinderQA;
	
	const KFParticleTopoReconstructor* fKFtopo;
	KFTopoPerformance* fKFtopoPerf;
	std::vector<int> trackindexarray;
	Int_t particlecounter;
	Int_t particlecounter_2daughters;
	Int_t particlecounter_3daughters;
	Int_t particlecounter_4daughters;
	Int_t particlecounter_all;
	TH1D * fhPi0_NDaughters;

	// for data gained from the KFParticle package
	Int_t fNofGeneratedPi0_allEvents;	// number of generated pi0 summed up over ALL EVENTS
	Int_t fNofPi0_kfparticle_allEvents;	// number of all reconstructed pi0 summed up over ALL EVENTS
	Int_t fNofGeneratedPi0;
	Int_t fNofPi0_kfparticle;
	TH1D * fhPi0Ratio;
	TH1D * fhPi0_mass;


	std::vector<int> fSignalIds;
	std::vector<int> fGhostIds;


	std::vector<TH1*> fHistoList_kfparticle;	// list of all histograms containing results from KFParticle package
	

	std::vector<KFParticle> particlevector;
	std::vector<KFPartMatch> particleMatch;
	std::vector<int> electronIDs;
	std::vector<int> gammaIDs;
	TH1D * fhInvMassPi0WithFullReco;

	TH1D * fhInvMass2Gammas;
	TH1D * fhInvMass2Gammas_cut;

	TH1D * fhKF_particlevector;
	TH1D * fhKF_trackvector;

	TH1D * fhKF_NofPi0;
	TH1D * fhKF_NofPi0_signal;
	TH1D * fhKF_NofPi0_trackvector;


	std::vector< std::vector<int> > fKF_photon_pairs;
	TH1D * fhKF_invmass_fullReco;


	// timer
	TStopwatch timer;
	Double_t fTime;

	CbmAnaConversionKF(const CbmAnaConversionKF&);
	CbmAnaConversionKF operator=(const CbmAnaConversionKF&);

	ClassDef(CbmAnaConversionKF,1)
};

#endif
