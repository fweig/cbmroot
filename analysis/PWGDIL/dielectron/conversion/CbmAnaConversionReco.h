/**
 * \file CbmAnaConversionReco.h
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2014
 **/
 
 
#ifndef CBM_ANA_CONVERSION_RECO
#define CBM_ANA_CONVERSION_RECO

// included from ROOT
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TClonesArray.h>
#include <TStopwatch.h>

// included from CbmRoot
#include "CbmMCTrack.h"
#include "CbmVertex.h"
#include "CbmLitGlobalElectronId.h"
#include "CbmLmvmKinematicParams.h"
#include "CbmRichElectronIdAnn.h"

#include <vector>

class CbmAnaConversionReco
{

public:
	CbmAnaConversionReco();
	virtual ~CbmAnaConversionReco();

	void Init();
	void InitHistos();
	void Finish();

	void SetTracklistMC(std::vector<CbmMCTrack*> MCTracklist);
	void SetTracklistReco(std::vector<CbmMCTrack*> MCTracklist, std::vector<TVector3> RecoTracklist1, std::vector<TVector3> RecoTracklist2, std::vector<int> ids, std::vector<Double_t> chi, std::vector<Int_t> GlobalTrackId);
	void InvariantMassMC_all();
	Double_t Invmass_4particles(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2, const CbmMCTrack* mctrack3, const CbmMCTrack* mctrack4);
	Double_t SmearValue(Double_t value);
	Double_t Invmass_4particlesRECO(const TVector3 part1, const TVector3 part2, const TVector3 part3, const TVector3 part4);
	void InvariantMassTest_4epem();
	void CutEfficiencyStudies(int e1, int e2, int e3, int e4, int motherE1, int motherE2, int motherE3, int motherE4, int IsEta = 0);
	Int_t NofDaughters(Int_t motherId);
	Double_t CalculateOpeningAngleReco(TVector3 electron1, TVector3 electron2);
	Double_t CalculateOpeningAngleMC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2);
	Double_t CalculateOpeningAngleBetweenGammasMC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2, CbmMCTrack* mctrack3, CbmMCTrack* mctrack4);
	Double_t CalculateOpeningAngleBetweenGammasReco(TVector3 electron1, TVector3 electron2, TVector3 electron3, TVector3 electron4);

	void CalculateInvMassWithFullRecoCuts();
	CbmLmvmKinematicParams CalculateKinematicParamsReco(const TVector3 electron1, const TVector3 electron2);
	CbmLmvmKinematicParams CalculateKinematicParams_4particles(const TVector3 part1, const TVector3 part2, const TVector3 part3, const TVector3 part4);

	Bool_t IsRichElectronANN(Int_t globalTrackIndex, Double_t momentum);
	Double_t ElectronANNvalue(Int_t globalTrackIndex, Double_t momentum);
	Bool_t IsRichElectronNormal(Int_t globalTrackIndex, Double_t momentum);






private:
	TClonesArray* fMcTracks;
	TClonesArray* fGlobalTracks;
	TClonesArray* fRichRings;
	TClonesArray* fRichHits;


	std::vector<CbmMCTrack*>	fMCTracklist_all;
	std::vector<CbmMCTrack*>	fRecoTracklistEPEM;
	std::vector<int>			fRecoTracklistEPEM_ids;
	std::vector<Double_t>	fRecoTracklistEPEM_chi;
	std::vector<Int_t>		fRecoTracklistEPEM_gtid;	// globalTrack Id
	std::vector<TVector3>	fRecoMomentum;
	std::vector<TVector3>	fRecoRefittedMomentum;
	

	std::vector<TH1*> fHistoList_MC;			// list of all histograms generated with MC data
	std::vector<TH1*> fHistoList_reco;		// list of all histograms generated with reconstructed data
	std::vector<TH1*> fHistoList_reco_mom;	// list of all histograms of reconstruction data (used momenta)
	std::vector<TH1*> fHistoList_gg;
	std::vector<TH1*> fHistoList_gee;
	std::vector<TH1*> fHistoList_eeee;
	std::vector<TH1*> fHistoList_all;
	std::vector<TH1*> fHistoList_eta;


	TH1D * fhInvariantMass_MC_all;
	TH1D * fhInvariantMass_MC_pi0;
	TH1D * fhInvariantMass_MC_pi0_epem;
	TH1D * fhInvariantMass_MC_pi0_gepem;
	TH1D * fhInvariantMass_MC_pi0_gg;
	TH1D * fhInvariantMass_MC_eta;
	TH1D * fhInvariantMass_MC_etaPrime;

	TH1D * fhMC_electrons_theta;
	TH1D * fhMC_electrons_p;
	TH2D * fhMC_electrons_theta_vs_p;

	TH1D * fhEta_openingAngleGG;
	
	TH1D * fhMC_grandmotherPDGs;
   
	TH1D * fhInvariantMassReco_pi0;

	TH1D * fhMCtest;


	// histograms for procedure "InvariantMassTest_4epem()"
	TH1D * fhEPEM_invmass_gg_mc;
	TH1D * fhEPEM_invmass_gg_refitted;
	TH1D * fhEPEM_invmass_gee_mc;
	TH1D * fhEPEM_invmass_gee_refitted;
	TH1D * fhEPEM_invmass_eeee_mc;
	TH1D * fhEPEM_invmass_eeee_refitted;
	TH1D * fhEPEM_invmass_all_mc;
	TH1D * fhEPEM_invmass_all_refitted;
	
	TH1D * fhEPEM_openingAngle_gg_mc;
	TH1D * fhEPEM_openingAngle_gg_refitted;
	TH1D * fhEPEM_openingAngle_gee_mc;
	TH1D * fhEPEM_openingAngle_gee_refitted;
	TH1D * fhEPEM_openingAngle_gee_mc_dalitz;
	TH1D * fhEPEM_openingAngle_gee_refitted_dalitz;
	
	TH2D * fhEPEM_openingAngle_vs_pt_gg_mc;
	TH2D * fhEPEM_openingAngle_vs_pt_gg_reco;
	
	TH1D * fhEPEM_openingAngle_betweenGammas_mc;
	TH1D * fhEPEM_openingAngle_betweenGammas_reco;

	TH2D * fhPi0_pt_vs_rap_gg;
	TH2D * fhPi0_pt_vs_rap_gee;
	TH2D * fhPi0_pt_vs_rap_all;

	TH1D * fhPi0_pt_gg;
	TH1D * fhPi0_pt_gee;
	TH1D * fhPi0_pt_all;

	TH1D * fhEPEM_efficiencyCuts;
	TH1D * fhEPEM_efficiencyCuts2;
	TH1D * fhEPEM_pi0_nofLeptons_ann;
	TH1D * fhEPEM_pi0_ANNvalues_noCuts;
	TH1D * fhEPEM_pi0_ANNvalues_angleCut;
	TH1D * fhEPEM_pi0_ANNefficiencies;

	TH2D * fhEPEM_rap_vs_chi;
	TH2D * fhEPEM_rap_vs_invmass;


	TH1D * fhInvMass_EPEM_mc;
	TH1D * fhInvMass_EPEM_stsMomVec;
	TH1D * fhInvMass_EPEM_refitted;
	TH1D * fhInvMass_EPEM_error_stsMomVec;
	TH1D * fhInvMass_EPEM_error_refitted;
	TH1D * fhInvMass_EPEM_openingAngleRef;	// refitted 
	TH1D * fhUsedMomenta_stsMomVec;
	TH1D * fhUsedMomenta_mc;
	TH1D * fhUsedMomenta_error_stsMomVec;
	TH1D * fhUsedMomenta_error_refitted;
	TH1D * fhUsedMomenta_errorX_stsMomVec;
	TH2D * fhUsedMomenta_vsX_stsMomVec;
	TH1D * fhUsedMomenta_errorY_stsMomVec;
	TH2D * fhUsedMomenta_vsY_stsMomVec;
	TH1D * fhUsedMomenta_errorZ_stsMomVec;
	TH2D * fhUsedMomenta_vsZ_stsMomVec;
	TH1D * fhUsedMomenta_errorX_refitted;
	TH2D * fhUsedMomenta_vsX_refitted;
	TH1D * fhUsedMomenta_errorY_refitted;
	TH2D * fhUsedMomenta_vsY_refitted;
	TH1D * fhUsedMomenta_errorZ_refitted;
	TH2D * fhUsedMomenta_vsZ_refitted;
   
	TH1D * fhInvariantMass_pi0epem;
	TH1D * fhPi0_startvertex;
	TH1D * fhPi0_startvertexElectrons_all;
	TH1D * fhPi0_startvertexElectrons_gg;
	TH1D * fhPi0_startvertexElectrons_gee;
	TH2D * fhPi0_startvertex_vs_chi;
	TH2D * fhPi0_startvertex_vs_momentum;

	TH1D * fhInvMassWithFullRecoCuts;

	TH1D * fhEPEM_InDetector_invmass_gg_mc;
	TH1D * fhEPEM_InDetector_invmass_gg_refitted;
	TH1D * fhEPEM_InDetector_invmass_gee_mc;
	TH1D * fhEPEM_InDetector_invmass_gee_refitted;
	TH1D * fhEPEM_InDetector_invmass_all_mc;
	TH1D * fhEPEM_InDetector_invmass_all_refitted;

	TH2D * fhEPEM_pt_vs_p_all_mc;
	TH2D * fhEPEM_pt_vs_p_all_refitted;
	
	
	// analysis of missing lepton (for the case of only 3 leptons with identified as electron)
	TH1D * fhEPEM_missingLepton_nofRingHits;
	TH2D * fhEPEM_missingLepton_ringMid;
	TH1D * fhEPEM_missingLepton_ringRadius;
	TH1D * fhEPEM_missingLepton_distance;
	TH1D * fhEPEM_missingLepton_selectionNN;
	TH2D * fhEPEM_missingLepton_rings;
	TH2D * fhEPEM_missingLepton_radius_vs_p;
	TH1D * fhEPEM_missingLepton_ANNvalue;
	// analysis of missing lepton (for the case of only 3 leptons with identified as electron) - HERE: same parameters for identified electrons
	TH1D * fhEPEM_identifiedLepton_nofRingHits;
	TH2D * fhEPEM_identifiedLepton_ringMid;
	TH1D * fhEPEM_identifiedLepton_ringRadius;
	TH1D * fhEPEM_identifiedLepton_distance;
	TH1D * fhEPEM_identifiedLepton_selectionNN;
	TH2D * fhEPEM_identifiedLepton_rings;
	TH2D * fhEPEM_identifiedLepton_radius_vs_p;
	TH1D * fhEPEM_identifiedLepton_ANNvalue;
	
	
	// histograms for eta analysis
	TH1D * fhEPEM_invmass_eta_mc;
	TH1D * fhEPEM_invmass_eta_refitted;
	TH1D * fhEPEM_efficiencyCuts_eta;

	// timer
	TStopwatch timer;
	Double_t fTime;

	CbmAnaConversionReco(const CbmAnaConversionReco&);
	CbmAnaConversionReco operator=(const CbmAnaConversionReco&);

	ClassDef(CbmAnaConversionReco,1)
};

#endif
