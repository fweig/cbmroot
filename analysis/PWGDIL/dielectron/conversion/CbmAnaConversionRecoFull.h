/**
 * \file CbmAnaConversionRecoFull.h
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2015
 **/
 
 
#ifndef CBM_ANA_CONVERSION_RECOFULL
#define CBM_ANA_CONVERSION_RECOFULL


// included from ROOT
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TClonesArray.h>
#include <TStopwatch.h>

// included from CbmRoot
#include "CbmMCTrack.h"
#include "CbmVertex.h"
#include "CbmKFVertex.h"
#include "CbmLitGlobalElectronId.h"
#include "CbmLmvmKinematicParams.h"

#include <vector>

class CbmAnaConversionRecoFull
{

public:
	CbmAnaConversionRecoFull();
	virtual ~CbmAnaConversionRecoFull();

	void Init();
	void InitHistos();
	void Finish();
	void Exec();

	void CombineElectrons(std::vector<CbmGlobalTrack*> gtrack, std::vector<TVector3> momenta, std::vector<float> momentaChi, std::vector<int> mctrackID, std::vector< std::vector<int> > reconstructedPhotons, Int_t index);
	Double_t Invmass_4particlesRECO(const TVector3 part1, const TVector3 part2, const TVector3 part3, const TVector3 part4);
	Double_t Pt_4particlesRECO(const TVector3 part1, const TVector3 part2, const TVector3 part3, const TVector3 part4);
	Double_t Rap_4particlesRECO(const TVector3 part1, const TVector3 part2, const TVector3 part3, const TVector3 part4);

	CbmLmvmKinematicParams CalculateKinematicParamsReco(const TVector3 electron1, const TVector3 electron2);
	CbmLmvmKinematicParams CalculateKinematicParams_4particles(const TVector3 part1, const TVector3 part2, const TVector3 part3, const TVector3 part4);
	
	void CombinePhotons(std::vector<CbmGlobalTrack*> gtrack, std::vector<TVector3> momenta, std::vector<float> momentaChi, std::vector<int> mctrackID, std::vector< std::vector<int> > reconstructedPhotons, Int_t index);
	Double_t OpeningAngleBetweenPhotons2(std::vector<int> photon1, std::vector<int> photons2);
	Double_t OpeningAngleBetweenPhotons(std::vector<TVector3> momenta, std::vector<int> photon1, std::vector<int> photons2);

	//void CombineElectronsRefit();
	//void CombinePhotonsRefit();
	//Double_t OpeningAngleBetweenPhotonsRefit(std::vector<int> photon1, std::vector<int> photons2);

	void CombinePhotonsDirection();

	void CombineElectrons();
	void CombinePhotons();
	
	void MixedEventTest();
	void MixedEventTest2();
	void MixedEventTest3();
	void MixedEventTest4();

	Double_t ElectronANNvalue(Int_t globalTrackIndex, Double_t momentum);

private:
	TClonesArray* fRichPoints;
	TClonesArray* fRichRings;
	TClonesArray* fRichRingMatches;
	TClonesArray* fMcTracks;
	TClonesArray* fStsTracks;
	TClonesArray* fStsTrackMatches;
	TClonesArray* fGlobalTracks;
	CbmVertex *fPrimVertex;
	CbmKFVertex fKFVertex;

	std::vector<TH1*> fHistoList_recofull;
	std::vector<TH1*> fHistoList_recofull_1;
	std::vector<TH1*> fHistoList_recofull_2;
	std::vector<TH1*> fHistoList_recofull_3;
	std::vector<TH1*> fHistoList_recofull_4;
	std::vector<TH1*> fHistoList_recofull_new[5];

	TH1I * fhElectrons;
	
	TH1D * fhMomentumFits;
	TH1D * fhMomentumFits_electronRich;
	TH1D * fhMomentumFits_pi0reco;

	std::vector<CbmGlobalTrack*> fElectrons_track;
	std::vector<TVector3> fElectrons_momenta;
	std::vector<float> fElectrons_momentaChi;
	std::vector<int> fElectrons_mctrackID;
	
	
	
	std::vector<CbmGlobalTrack*>	fElectrons_track_1;
	std::vector<TVector3>		fElectrons_momenta_1;
	std::vector<float>			fElectrons_momentaChi_1;
	std::vector<int>				fElectrons_mctrackID_1;
	std::vector< std::vector<int> >	fVector_photons_pairs_1;
	
	std::vector<CbmGlobalTrack*>	fElectrons_track_2;
	std::vector<TVector3>		fElectrons_momenta_2;
	std::vector<float>			fElectrons_momentaChi_2;
	std::vector<int>				fElectrons_mctrackID_2;
	std::vector< std::vector<int> >	fVector_photons_pairs_2;
	
	std::vector<CbmGlobalTrack*>	fElectrons_track_3;
	std::vector<TVector3>		fElectrons_momenta_3;
	std::vector<float>			fElectrons_momentaChi_3;
	std::vector<int>				fElectrons_mctrackID_3;
	std::vector< std::vector<int> >	fVector_photons_pairs_3;
	
	std::vector<CbmGlobalTrack*>	fElectrons_track_4;
	std::vector<TVector3>		fElectrons_momenta_4;
	std::vector<float>			fElectrons_momentaChi_4;
	std::vector<int>				fElectrons_mctrackID_4;
	std::vector< std::vector<int> >	fVector_photons_pairs_4;
	
	std::vector<CbmGlobalTrack*>	fElectrons_track_new[5];
	std::vector<TVector3>		fElectrons_momenta_new[5];
	std::vector<float>			fElectrons_momentaChi_new[5];
	std::vector<int>				fElectrons_mctrackID_new[5];
	std::vector<int>				fElectrons_globaltrackID_new[5];
	std::vector< std::vector<int> >	fVector_photons_pairs_new[5];


	TH1D * fhElectrons_invmass;
	TH1D * fhElectrons_invmass_cut;
	TH1D * fhElectrons_nofPerEvent;
	TH1D * fhPhotons_nofPerEvent;
	
	
	std::vector< std::vector<int> > fVector_photons_pairs;
	std::vector<TVector3> fVector_photons_momenta;
	TH1D * fhPhotons_invmass;
	TH1D * fhPhotons_invmass_cut;
	TH1D * fhPhotons_invmass_cut_chi1;
	TH1D * fhPhotons_invmass_cut_chi3;
	TH1D * fhPhotons_invmass_cut_chi5;
	TH1D * fhPhotons_invmass_cut_chi10;
	TH1D * fhPhotons_invmass_cut_chi25;
	TH1D * fhPhotons_invmass_cut_chi40;
	TH1D * fhPhotons_invmass_cut_chi65;
	TH1D * fhPhotons_invmass_cut_chi80;
	
	TH1D * fhPhotons_invmass_cut_ptBin1;
	TH1D * fhPhotons_invmass_cut_ptBin2;
	TH1D * fhPhotons_invmass_cut_ptBin3;
	TH1D * fhPhotons_invmass_cut_ptBin4;
	
	TH2D * fhPhotons_invmass_vs_chi;
	TH2D * fhPhotons_startvertex_vs_chi;
	TH1D * fhPhotons_angleBetween;
	TH2D * fhPhotons_invmass_vs_pt;
	TH2D * fhPhotons_rapidity_vs_pt;
	TH2D * fhPhotons_invmass_vs_openingAngle;
	TH2D * fhPhotons_openingAngle_vs_momentum;
	
	TH1D * fhPhotons_MC_motherpdg;
	TH1D * fhPhotons_MC_invmass1;
	TH1D * fhPhotons_MC_invmass2;
	TH1D * fhPhotons_MC_invmass3;
	TH1D * fhPhotons_MC_invmass4;
	TH1D * fhPhotons_MC_startvertexZ;
	TH1D * fhPhotons_MC_motherIdCut;

	TH1D * fhPhotons_Refit_chiDiff;
	TH1D * fhPhotons_Refit_momentumDiff;
	TH1D * fhPhotons_Refit_chiDistribution;
	TH1D * fhPhotons_RefitPion_chiDistribution;



	std::vector<CbmGlobalTrack*> fElectrons_track_refit;
	std::vector<TVector3> fElectrons_momenta_refit;
	std::vector< std::vector<int> > fVector_photons_pairs_refit;
	TH1D * fhPhotons_invmass_refit;
	TH1D * fhPhotons_invmass_refit_cut;
	
	std::vector< std::vector<int> > fVector_photons_pairs_direction;
	TH1D * fhPhotons_invmass_direction;
	TH1D * fhPhotons_invmass_direction_cut;
	TH1D * fhPhotons_boostAngle;
	TH1D * fhPhotons_boostAngleMC;
	TH1D * fhPhotons_boostAngleTest;
	
	TH1D * fhPhotons_tX;
	TH1D * fhPhotons_tY;
	
	// histograms for index = 1
	TH1D * fhElectrons_nofPerEvent_1;
	TH1D * fhPhotons_nofPerEvent_1;
	TH1D * fhPhotons_invmass_1;
	TH1D * fhPhotons_invmass_ptBin1_1;
	TH1D * fhPhotons_invmass_ptBin2_1;
	TH1D * fhPhotons_invmass_ptBin3_1;
	TH1D * fhPhotons_invmass_ptBin4_1;
	TH1D * fhElectrons_invmass_1;
	TH2D * fhPhotons_invmass_vs_pt_1;
	
	// histograms for index = 2
	TH1D * fhElectrons_nofPerEvent_2;
	TH1D * fhPhotons_nofPerEvent_2;
	TH1D * fhPhotons_invmass_2;
	TH1D * fhPhotons_invmass_ptBin1_2;
	TH1D * fhPhotons_invmass_ptBin2_2;
	TH1D * fhPhotons_invmass_ptBin3_2;
	TH1D * fhPhotons_invmass_ptBin4_2;
	TH1D * fhElectrons_invmass_2;
	TH2D * fhPhotons_invmass_vs_pt_2;
	
	// histograms for index = 3
	TH1D * fhElectrons_nofPerEvent_3;
	TH1D * fhPhotons_nofPerEvent_3;
	TH1D * fhPhotons_invmass_3;
	TH1D * fhPhotons_invmass_ptBin1_3;
	TH1D * fhPhotons_invmass_ptBin2_3;
	TH1D * fhPhotons_invmass_ptBin3_3;
	TH1D * fhPhotons_invmass_ptBin4_3;
	TH1D * fhElectrons_invmass_3;
	TH2D * fhPhotons_invmass_vs_pt_3;
	
	// histograms for index = 4
	TH1D * fhElectrons_nofPerEvent_4;
	TH1D * fhPhotons_nofPerEvent_4;
	TH1D * fhPhotons_invmass_4;
	TH1D * fhPhotons_invmass_ptBin1_4;
	TH1D * fhPhotons_invmass_ptBin2_4;
	TH1D * fhPhotons_invmass_ptBin3_4;
	TH1D * fhPhotons_invmass_ptBin4_4;
	TH1D * fhElectrons_invmass_4;
	TH2D * fhPhotons_invmass_vs_pt_4;



	TH1D * fhElectrons_nofPerEvent_new[5];
	TH1D * fhPhotons_nofPerEvent_new[5];
	TH1D * fhPhotons_nofPerEventAfter_new[5];
	TH1D * fhPi0_nofPerEvent_new[5];
	TH1D * fhPhotons_invmass_new[5];
	TH1D * fhPhotons_invmass_ann0_new[5];
	TH1D * fhPhotons_invmass_ptBin1_new[5];
	TH1D * fhPhotons_invmass_ptBin2_new[5];
	TH1D * fhPhotons_invmass_ptBin3_new[5];
	TH1D * fhPhotons_invmass_ptBin4_new[5];
	TH1D * fhPhotons_invmass_ptBin5_new[5];
	TH1D * fhPhotons_invmass_ptBin6_new[5];
	TH1D * fhElectrons_invmass_new[5];
	TH2D * fhPhotons_invmass_vs_pt_new[5];
	TH1D * fhPhotons_invmass_MCcut1_new[5];
	TH1D * fhPhotons_invmass_MCcut2_new[5];
	TH1D * fhPhotons_invmass_MCcut3_new[5];
	TH1D * fhPhotons_invmass_MCcut4_new[5];
	TH1D * fhPhotons_invmass_MCcut5_new[5];
	TH1D * fhPhotons_invmass_MCcut6_new[5];
	TH1D * fhPhotons_invmass_MCcut7_new[5];
	TH1D * fhPhotons_invmass_MCcutTest_new[5];
	TH1D * fhPhotons_invmass_MCcutTest2_new[5];
	TH1D * fhPhotons_invmass_MCcutTest3_new[5];
	TH2D * fhPhotons_invmass_MCcutAll_new[5];
	TH2D * fhPhotons_pt_vs_rap_new[5];
	TH1D * fhElectrons_openingAngle_sameSign_new[5];
	TH1D * fhPhotons_stats[5];
	TH1D * fhPhotons_MCtrue_pdgCodes;
	TH1D * fhPhotons_peakCheck1[5];
	TH1D * fhPhotons_peakCheck2[5];
	TH2D * fhPhotons_invmass_ANNcuts_new[5];
	
	TH2D * fhPhotons_phaseSpace_pi0[5];
	TH2D * fhPhotons_phaseSpace_eta[5];



	// test with mixed event method for estimation of invariant mass background
		// test1
	std::vector<TVector3>		fMixedEventsElectrons[5];
	std::vector<CbmGlobalTrack*>	fMixedEventsElectrons_gtrack[5];
	TH1D *					fhMixedEventsTest_invmass[5];
	
		// test2
	std::vector<TVector3>		fMixedEventsElectrons_list1;
	std::vector<TVector3>		fMixedEventsElectrons_list2;
	std::vector<TVector3>		fMixedEventsElectrons_list3;
	std::vector<TVector3>		fMixedEventsElectrons_list4;
	std::vector<CbmGlobalTrack*>	fMixedEventsElectrons_list1_gtrack;
	std::vector<CbmGlobalTrack*>	fMixedEventsElectrons_list2_gtrack;
	std::vector<CbmGlobalTrack*>	fMixedEventsElectrons_list3_gtrack;
	std::vector<CbmGlobalTrack*>	fMixedEventsElectrons_list4_gtrack;
	TH1D *					fhMixedEventsTest2_invmass;
	
		// test3
	std::vector<TVector3>		fMixedTest3_momenta;
	std::vector<CbmGlobalTrack*>	fMixedTest3_gtrack;
	std::vector<int>				fMixedTest3_eventno;
	Int_t					globalEventNo;
	TH1D *					fhMixedEventsTest3_invmass;
	
		// test4
	std::vector< std::vector<TVector3> >	fMixedTest4_photons;
	std::vector< std::vector<CbmMCTrack*> >	fMixedTest4_mctracks;
	std::vector< std::vector<Bool_t> >	fMixedTest4_isRichElectronAnn0;
	std::vector< std::vector<Double_t> >	fMixedTest4_ElectronAnns;
	std::vector<int>					fMixedTest4_eventno;
	TH1D *						fhMixedEventsTest4_invmass;
	TH1D *						fhMixedEventsTest4_invmass_ann0;
	TH2D *						fhMixedEventsTest4_pt_vs_rap;
	TH1D *						fhMixedEventsTest4_invmass_ptBin1;
	TH1D *						fhMixedEventsTest4_invmass_ptBin2;
	TH1D *						fhMixedEventsTest4_invmass_ptBin3;
	TH1D *						fhMixedEventsTest4_invmass_ptBin4;
	TH2D *						fhMixedEventsTest4_invmass_ANNcuts;

	// timer
	TStopwatch timer;
	Double_t fTime;

	CbmAnaConversionRecoFull(const CbmAnaConversionRecoFull&);
	CbmAnaConversionRecoFull operator=(const CbmAnaConversionRecoFull&);

	ClassDef(CbmAnaConversionRecoFull,1)
};

#endif
