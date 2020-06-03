/**
 * \file CbmAnaConversionPhotons2.h
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2015
 **/
 
 
#ifndef CBM_ANA_CONVERSION_PHOTONS2
#define CBM_ANA_CONVERSION_PHOTONS2

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

#include <vector>

class CbmAnaConversionPhotons2
{

public:
	CbmAnaConversionPhotons2();
	virtual ~CbmAnaConversionPhotons2();

	void Init();
	void InitHistos();
	void Finish();
	void Exec();

	void AnalyseElectronsReco();
	void AnalyseElectronsRecoWithRICH();

	void EstimateFullRecoResults();


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

	std::vector<TH1*> fHistoList_photons;			// list of all histograms related to rich rings
	std::vector<TH1*> fHistoList_photons_withRICH;	// list of all histograms related to rich rings
	std::vector<TH1*> fHistoList_photons_withRICH_withChi;	// list of all histograms related to rich rings
	std::vector<TH1*> fHistoList_photons_withRICH_fromTarget;	// list of all histograms related to rich rings

	std::vector<Int_t>			fRecoTracklist_gtIndex;
	std::vector<Int_t>			fRecoTracklist_mcIndex;
	std::vector<TVector3>		fRecoTracklist_momentum;
	std::vector<Double_t>		fRecoTracklist_chi;
	std::vector<CbmMCTrack*>		fRecoTracklist_mctrack;

	std::vector<Int_t>			fRecoTracklist_withRICH_gtIndex;
	std::vector<Int_t>			fRecoTracklist_withRICH_mcIndex;
	std::vector<TVector3>		fRecoTracklist_withRICH_momentum;
	std::vector<Double_t>		fRecoTracklist_withRICH_chi;
	std::vector<CbmMCTrack*>		fRecoTracklist_withRICH_mctrack;




	// distribution of opening angles from reconstructed momenta
	TH1D * fh2Electrons_angle_all;
	TH1D * fh2Electrons_angle_combBack;
	TH1D * fh2Electrons_angle_allSameG;

	// distribution of opening angles from reconstructed momenta, with application of opening angle cuts
	TH1D * fh2Electrons_angle_all_cuts;
	TH1D * fh2Electrons_angle_combBack_cuts;
	TH1D * fh2Electrons_angle_allSameG_cuts;
	
	// histogram for comparison of different opening angle cuts and their influence on signal and background amounts
	TH1I * fh2Electrons_angle_CUTcomparison;
	TH1I * fh2Electrons_angle_CUTcomparison_chi;
	
	TH1I * fh2Electrons_angle_CUTcomparison_withRICH;
	TH1I * fh2Electrons_angle_CUTcomparison_withRICH_chi;
	TH1I * fh2Electrons_angle_CUTcomparison_withRICH_withChi;


	// distribution of invariant masses from reconstructed momenta
	TH1D * fh2Electrons_invmass_all;
	TH1D * fh2Electrons_invmass_combBack;
	TH1D * fh2Electrons_invmass_allSameG;

	// distribution of invariant masses from reconstructed momenta with cut on opening angle
	TH1D * fh2Electrons_invmass_all_cut;
	TH1D * fh2Electrons_invmass_combBack_cut;
	TH1D * fh2Electrons_invmass_allSameG_cut;

	// opening angle vs pt from reconstructed data
	TH2D * fh2Electrons_angleVSpt_all;
	TH2D * fh2Electrons_angleVSpt_combBack;
	TH2D * fh2Electrons_angleVSpt_allSameG;

	// invariant mass vs pt from reconstructed data
	TH2D * fh2Electrons_invmassVSpt_all;
	TH2D * fh2Electrons_invmassVSpt_combBack;
	TH2D * fh2Electrons_invmassVSpt_allSameG;



	// distribution of opening angles from reconstructed momenta, with RICH
	TH1D * fh2Electrons_angle_withRICH_all;
	TH1D * fh2Electrons_angle_withRICH_combBack;
	TH1D * fh2Electrons_angle_withRICH_allSameG;


	// opening angle vs pt from reconstructed data, with RICH
	TH2D * fh2Electrons_angleVSpt_withRICH_all;
	TH2D * fh2Electrons_angleVSpt_withRICH_combBack;
	TH2D * fh2Electrons_angleVSpt_withRICH_allSameG;


	// distribution of opening angles from reconstructed momenta, with RICH
	TH1D * fh2Electrons_invmass_withRICH_all;
	TH1D * fh2Electrons_invmass_withRICH_combBack;
	TH1D * fh2Electrons_invmass_withRICH_allSameG;


	// opening angle vs pt from reconstructed data, with RICH
	TH2D * fh2Electrons_invmassVSpt_withRICH_all;
	TH2D * fh2Electrons_invmassVSpt_withRICH_combBack;
	TH2D * fh2Electrons_invmassVSpt_withRICH_allSameG;


	// opening angle vs pt from reconstructed data, with RICH and with CHI-cut
	TH1D * fh2Electrons_angle_withRICH_withChi_all;
	TH1D * fh2Electrons_angle_withRICH_withChi_combBack;
	TH1D * fh2Electrons_angle_withRICH_withChi_allSameG;
	TH2D * fh2Electrons_angleVSpt_withRICH_withChi_all;
	TH2D * fh2Electrons_angleVSpt_withRICH_withChi_combBack;
	TH2D * fh2Electrons_angleVSpt_withRICH_withChi_allSameG;
	TH1D * fh2Electrons_invmass_withRICH_withChi_all;
	TH1D * fh2Electrons_invmass_withRICH_withChi_combBack;
	TH1D * fh2Electrons_invmass_withRICH_withChi_allSameG;
	TH2D * fh2Electrons_invmassVSpt_withRICH_withChi_all;
	TH2D * fh2Electrons_invmassVSpt_withRICH_withChi_combBack;
	TH2D * fh2Electrons_invmassVSpt_withRICH_withChi_allSameG;


	// opening angle vs pt from reconstructed data, with RICH, from TARGET (z>1cm)
	TH1D * fh2Electrons_angle_withRICH_fromTarget_all;
	TH1D * fh2Electrons_angle_withRICH_fromTarget_combBack;
	TH1D * fh2Electrons_angle_withRICH_fromTarget_allSameG;
	TH2D * fh2Electrons_angleVSpt_withRICH_fromTarget_all;
	TH2D * fh2Electrons_angleVSpt_withRICH_fromTarget_combBack;
	TH2D * fh2Electrons_angleVSpt_withRICH_fromTarget_allSameG;
	TH1D * fh2Electrons_invmass_withRICH_fromTarget_all;
	TH1D * fh2Electrons_invmass_withRICH_fromTarget_combBack;
	TH1D * fh2Electrons_invmass_withRICH_fromTarget_allSameG;
	TH2D * fh2Electrons_invmassVSpt_withRICH_fromTarget_all;
	TH2D * fh2Electrons_invmassVSpt_withRICH_fromTarget_combBack;
	TH2D * fh2Electrons_invmassVSpt_withRICH_fromTarget_allSameG;


	// crosscheck for full reco, or estimation of signal and background
	std::vector< std::vector<int> > fVector_combinations;
	TH1I * fh2Electrons_fullrecoCheck;

	// timer
	TStopwatch timer;
	Double_t fTime;

	CbmAnaConversionPhotons2(const CbmAnaConversionPhotons2&);
	CbmAnaConversionPhotons2 operator=(const CbmAnaConversionPhotons2&);

	ClassDef(CbmAnaConversionPhotons2,1)
};

#endif

