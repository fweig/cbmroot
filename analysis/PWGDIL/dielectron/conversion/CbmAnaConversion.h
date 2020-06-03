/**
* \file CbmAnaConversion.h
*
* Base class for conversion analysis
*
* \author Sascha Reinecke <reinecke@uni-wuppertal.de>
* \date 2014
**/

#ifndef CBM_ANA_CONVERSION
#define CBM_ANA_CONVERSION

#include "FairTask.h"

#include "CbmMCTrack.h"
#include "CbmLmvmKinematicParams.h"
#include "CbmKFVertex.h"

//#include "KFParticleTopoReconstructor.h"
#include "CbmKFParticleFinder.h"
#include "CbmKFParticleFinderQA.h"

#include "CbmAnaConversionTomography.h"
#include "CbmAnaConversionRich.h"
#include "CbmAnaConversionKF.h"
#include "CbmAnaConversionReco.h"
#include "CbmAnaConversionPhotons.h"
#include "CbmAnaConversionPhotons2.h"
#include "CbmAnaConversionRecoFull.h"
#include "CbmAnaConversionTest.h"
#include "CbmAnaConversionTest2.h"

#include "TStopwatch.h"

#include <vector>

class TH1;
class TH2;
class TH3;
class TH1D;
class TH2D;
class TH2I;
class TH3D;
class TClonesArray;
class CbmRichRing;
class TCanvas;
class TRandom3;




/*class KinematicParams{
public:
   Double_t momentumMag; // Absolute value of momentum
   Double_t pt; // Transverse momentum
   Double_t rapidity; // Rapidity
   Double_t minv; // Invariant mass
   Double_t angle; // Opening angle
};*/

class CbmAnaConversion : public FairTask
{

public:
   /**
    * \brief Standard constructor.
    */
	CbmAnaConversion();

   /**
    * \brief Standard destructor.
    */
   virtual ~CbmAnaConversion();

   /**
    * \brief Inherited from FairTask.
    */
   virtual InitStatus Init();

   /**
    * \brief Inherited from FairTask.
    */
   virtual void Exec(
		   Option_t* option);
		   
   CbmLmvmKinematicParams CalculateKinematicParams(const CbmMCTrack* mctrackP, const CbmMCTrack* mctrackM);
        
	Double_t	Invmass_2gammas(const CbmMCTrack* gamma1, const CbmMCTrack* gamma2);
	Double_t	Invmass_2particles(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2);
	Double_t	Invmass_4particles(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2, const CbmMCTrack* mctrack3, const CbmMCTrack* mctrack4);
	Double_t	Invmass_4particlesRECO(const TVector3 part1, const TVector3 part2, const TVector3 part3, const TVector3 part4);


	void	CalculateInvMass_MC_2particles();

   /**
    * \brief Inherited from FairTask.
    */
	virtual void Finish();
   
	void	AnalyseElectrons(CbmMCTrack* mctrack);
	void	FillMCTracklists(CbmMCTrack* mctrack, int i);
	void	FillRecoTracklist(CbmMCTrack* mtrack);
	Bool_t	FillRecoTracklistEPEM(CbmMCTrack* mctrack, TVector3 stsMomentum, TVector3 refittedMom, int i, Double_t chi, Int_t GlobalTrackId);
	void	InvariantMassTest();
	void	InvariantMassTest_4epem();
	void	InvariantMassTestReco();
	int		GetTest();
	int		GetNofEvents();
	void	SetMode(int mode);
   
	void	ReconstructGamma();
   
   
	void	SetKF(CbmKFParticleFinder* kfparticle, CbmKFParticleFinderQA* kfparticleQA);

	Bool_t	AnalysePi0_MC(CbmMCTrack *mctrack, int i);
	void	AnalysePi0_Reco();
	Bool_t	AnalysePi0_Reco_calc(int e1, int e2, int e3, int e4);
	void	AnalysePi0_Reco_noRichInd();
	Bool_t	AnalysePi0_Reco_noRichInd_calc(int e1, int e2, int e3, int e4);


private:
	Int_t DoTomography;
	Int_t DoRichAnalysis;
	Int_t DoKFAnalysis;
	Int_t DoReconstruction;
	Int_t DoPhotons;
	Int_t DoPhotons2;
	Int_t DoRecoFull;
	Int_t DoTest;
   
   TH1D * fhPdgCodes;
   TH1D * fhNofElPrim;
   TH1D * fhNofElSec;
   TH1D * fhNofElAll;
   TH1D * fhElectronSources;
   TH1D * fhNofPi0_perEvent;		// number of pi0 per event
   TH1D * fhNofPi0_perEvent_cut;	// number of pi0 with cut on z-axis (z <= 4cm, i.e. generated before 4cm)
   TH1D * fhNofPi0_perEvent_cut2;	// number of pi0 with cut motherId = -1
   TH1D * fhNofPi0_perEvent_cut3;	// number of pi0 with conversion of gammas happening before RICH
   TH1D * fhNofEta_perEvent;		// number of pi0 per event
   TH1D * fhNofEta_perEvent_cut;	// number of pi0 with cut on z-axis (z <= 10cm, i.e. generated before 10cm)
   TH1D * fhNofEta_perEvent_cut2;	// number of pi0 with cut motherId = -1
   TH1D * fhPi0_z;					// number of pi0 per z-bin
   TH1D * fhPi0_z_cut;				// number of pi0 per z-bin with cut on acceptance (via x^2 + y^2 <= r^2 with r = z*tan)
   TH1D * fhPi0_pt;					// 
   TH2D * fhPi0_pt_vs_rap;			// 
   TH1D * fhPi0_theta;				// 
   TH2D * fhPi0_theta_vs_rap;		// 
   TH1D * fhEta_pt;					// 
   TH2D * fhEta_pt_vs_rap;			// 
   TH1D * fhEta_theta;				// 
   TH2D * fhEta_theta_vs_rap;		// 
   TH1D * fhRho_pt;					// 
   TH2D * fhRho_pt_vs_rap;			// 
   TH1D * fhRho_theta;				// 
   TH2D * fhRho_theta_vs_rap;		// 
   TH1D * fhOmega_pt;				// 
   TH2D * fhOmega_pt_vs_rap;		// 
   TH1D * fhOmega_theta;			// 
   TH2D * fhOmega_theta_vs_rap;		// 
   TH1D * fhElectronsFromPi0_z;		//
   
   TH1D * fhNofTracks_mctrack;
   TH1D * fhNofTracks_globaltrack;
   
   TH1D * fhInvariantMass_test;
   TH1D * fhInvariantMass_test2;
   TH1D * fhInvariantMass_test3;
   TH1D * fhInvariantMassReco_test;
   TH1D * fhInvariantMassReco_test2;
   TH1D * fhInvariantMassReco_test3;
      
   TH1D * fhInvariantMassReco_pi0;
   
   TH2D * fhMomentum_MCvsReco;
   TH1D * fhMomentum_MCvsReco_diff;
   

      
   TH1D * fhSearchGammas;

	CbmVertex *fPrimVertex;
	CbmKFVertex fKFVertex;
   TClonesArray* fRichPoints;
   TClonesArray* fRichRings;
   TClonesArray* fRichRingMatches;
   TClonesArray* fMcTracks;
   TClonesArray* fStsTracks;
   TClonesArray* fStsTrackMatches;
   TClonesArray* fGlobalTracks;
   
	TH1D * fhANN_output_electrons;
	TH1D * fhANN_output_electrons2;
	TH1D * fhANN_output_electrons_chiCut;
	TH2D * fhANN_output_electrons_vs_p;
	TH1D * fhANN_output_else;
	TH1D * fhANN_output_else2;
	TH1D * fhANN_output_else_chiCut;
	TH2D * fhANN_output_else_vs_p;

   Int_t fEventNum;
   
   Int_t test;
   
   int testint;
   
   Int_t fAnalyseMode;
   
   
   CbmKFParticleFinder* fKFparticle;
   CbmKFParticleFinderQA* fKFparticleFinderQA;
   const KFParticleTopoReconstructor* fKFtopo;
   std::vector<int> trackindexarray;
   Int_t particlecounter;
   Int_t particlecounter_2daughters;
   Int_t particlecounter_3daughters;
   Int_t particlecounter_4daughters;
   Int_t particlecounter_all;


	// for data gained from the KFParticle package
	Int_t fNofGeneratedPi0_allEvents;	// number of generated pi0 summed up over ALL EVENTS
	Int_t fNofPi0_kfparticle_allEvents;	// number of all reconstructed pi0 summed up over ALL EVENTS
	Int_t fNofGeneratedPi0;
	Int_t fNofPi0_kfparticle;
	TH1D * fhPi0Ratio;
	TH1D * fhPi0_mass;
	TH1D * fhPi0_NDaughters;








   /**
    * \brief Initialize histograms.
    */
   void InitHistograms();
   
   std::vector<TH1*> fHistoList;				// list of all histograms
   std::vector<TH1*> fHistoList_MC;			// list of all histograms generated with MC data
   std::vector<TH1*> fHistoList_tomography;	// list of all histograms of tomography data (photon conversion)
   std::vector<TH1*> fHistoList_reco;		// list of all histograms of reconstruction data
   std::vector<TH1*> fHistoList_reco_mom;	// list of all histograms of reconstruction data (used momenta)
   std::vector<TH1*> fHistoList_kfparticle;	// list of all histograms containing results from KFParticle package
   std::vector<TH1*> fHistoList_richrings;	// list of all histograms related to rich rings
   std::vector<TH1*> fHistoList_furtherAnalyses;	// list of all histograms from further analyses (occurence, etc.)
   
   std::vector<CbmMCTrack*>	fMCTracklist;
   std::vector<CbmMCTrack*>	fMCTracklist_all;
   std::vector<CbmMCTrack*>	fRecoTracklist;
   std::vector<CbmMCTrack*>	fRecoTracklistEPEM;
   std::vector<int>			fRecoTracklistEPEM_id; // ids of mctracks from fRecoTracklistEPEM
   std::vector<Double_t>		fRecoTracklistEPEM_chi; // chi of fitted momenta from fRecoTracklistEPEM
   std::vector<Int_t>		fRecoTracklistEPEM_gtid; // GlobalTrack ID from fRecoTracklistEPEM
   
   std::vector<CbmMCTrack*>	fTestTracklist;
   std::vector<TVector3>		fTestTracklist_momentum;
   std::vector<float>		fTestTracklist_chi;
   std::vector<int>			fTestTracklist_richInd;
   std::vector<int>			fTestTracklist_ndf;
   std::vector<int>			fTestTracklist_nofhits;
   
   std::vector<CbmMCTrack*>	fTestTracklist_noRichInd;
   std::vector<int>			fTestTracklist_noRichInd_MCindex;
   std::vector<TVector3>		fTestTracklist_noRichInd_momentum;
   std::vector<float>		fTestTracklist_noRichInd_chi;
   std::vector<int>			fTestTracklist_noRichInd_richInd;
   std::vector<int>			fTestTracklist_noRichInd_gTrackId;
   std::vector<int>			fTestTracklist_noRichInd_ndf;
   std::vector<int>			fTestTracklist_noRichInd_nofhits;
   
   std::vector<TVector3> fRecoMomentum;
   std::vector<TVector3> fRecoRefittedMomentum;


	TH1D * fhNofElectrons_4epem;
	
	TH1D * fhPi0_MC_occurence;
	TH1D * fhPi0_MC_occurence2;
	TH1D * fhPi0_Reco_occurence;
	TH1D * fhPi0_Reco_occurence2;
	TH1D * fhPi0_Reco_angles;
	TH1D * fhPi0_Reco_chi;
	TH1D * fhPi0_Reco_ndf;
	TH2D * fhPi0_Reco_ndf_vs_chi;
	TH2D * fhPi0_Reco_ndf_vs_startvertex;
	TH2D * fhPi0_Reco_startvertex_vs_chi;
	TH2D * fhPi0_Reco_startvertex_vs_nofhits;
	TH1D * fhPi0_noRichInd_diffPhi;
	TH1D * fhPi0_noRichInd_diffTheta;
	TH2D * fhPi0_Reco_invmass_cases;
	TH2D * fhPi0_Reco_noRichInd_invmass_cases;
	TH1D * fhPi0_Reco_invmass;
	TH1D * fhPi0_Reco_invmass_mc;
	TH1D * fhPi0_Reco_noRichInd_invmass;
	TH1D * fhPi0_Reco_noRichInd_invmass_mc;
	TH2D * fhPi0_Reco_noRichInd_ndf_vs_nofhits;
	TH2D * fhPi0_Reco_noRichInd_ndf_vs_nofhits_withChi;
	TH2D * fhPi0_Reco_ndf_vs_nofhits;
	TH2D * fhPi0_Reco_ndf_vs_nofhits_withChi;
	TH2D * fhPi0_Reco_noRichInd_chi_vs_momentum;
	TH2D * fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0;
	TH2D * fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0_Target;
	TH2D * fhPi0_Reco_noRichInd_chi_vs_momentum_eRest;
	TH2D * fhPi0_Reco_noRichInd_chi_vs_pt;
	TH2D * fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0;
	TH2D * fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0_Target;
	TH2D * fhPi0_Reco_noRichInd_chi_vs_pt_eRest;
	
	
	TH2D * fhPi0_Reco_chi_vs_momentum;
	TH2D * fhPi0_Reco_chi_vs_momentum_eFromPi0;
	TH2D * fhPi0_Reco_chi_vs_momentum_eFromPi0_Target;
	TH2D * fhPi0_Reco_chi_vs_pt;
	TH2D * fhPi0_Reco_chi_vs_pt_eFromPi0;
	TH2D * fhPi0_Reco_chi_vs_pt_eFromPi0_Target;
   
   
   // timer
   TStopwatch timer_all;
   Double_t fTime_all;
   TStopwatch timer_exec;
   Double_t fTime_exec;
   
   TStopwatch timer_mc;
   Double_t fTime_mc;
   TStopwatch timer_rec;
   Double_t fTime_rec;
   
   
   CbmAnaConversionTomography	*fAnaTomography;
   CbmAnaConversionRich			*fAnaRich;
   CbmAnaConversionKF			*fAnaKF;
   CbmAnaConversionReco			*fAnaReco;
   CbmAnaConversionPhotons		*fAnaPhotons;
   CbmAnaConversionPhotons2		*fAnaPhotons2;
   CbmAnaConversionRecoFull		*fAnaRecoFull;
   CbmAnaConversionTest			*fAnaTest;
   CbmAnaConversionTest2		*fAnaTest2;
   
   
   
   /**
    * \brief Copy constructor.
    */
   CbmAnaConversion(const CbmAnaConversion&);

   /**
    * \brief Assignment operator.
    */
   CbmAnaConversion& operator=(const CbmAnaConversion&);


	ClassDef(CbmAnaConversion,1)
};

#endif
