
#ifndef CBMJPSIANATASK
#define CBMJPSIANATASK

#include "FairTask.h"

#include "CbmHistManager.h"
#include "CbmKFVertex.h"
#include "CbmVertex.h"
#include "CbmAnaJpsiCandidate.h"
#include "CbmAnaJpsiCuts.h"
#include "CbmAnaJpsiHist.h"
#include "CbmAnaJpsiKinematicParams.h"
#include "CbmRichRing.h"
#include "CbmTrdTrack.h"
#include "CbmGlobalTrack.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TClonesArray.h"

#include <vector>
#include <string>

using namespace std;

class CbmAnaJpsiTask : public FairTask
{

public:
   /**
    * \brief Standard constructor.
    */
	CbmAnaJpsiTask();

   /**
    * \brief Standard destructor.
    */
   virtual ~CbmAnaJpsiTask();

   /**
    * \brief Inherited from FairTask.
    */
   virtual InitStatus Init();

   /**
    * \brief Inherited from FairTask.
    */
   virtual void Exec(
		   Option_t* option);

   /**
    * \brief Inherited from FairTask.
    */
   virtual void Finish();
   



private:
  Int_t fEventNum;
  
  TClonesArray* fMcTracks;
  TClonesArray* fStsPoints;
  TClonesArray* fStsHits;
  TClonesArray* fStsTracks;
  TClonesArray* fStsTrackMatches;
  TClonesArray* fRichPoints; 
  TClonesArray* fRichHits;
  TClonesArray* fRichRings;
  TClonesArray* fRichRingMatches;
  TClonesArray* fTrdPoints; 
  TClonesArray* fTrdHits;
  TClonesArray* fTrdTracks;
  TClonesArray* fTrdTrackMatches;
  TClonesArray* fTofPoints; 
  TClonesArray* fTofHits;
  TClonesArray* fTofHitsMatches;
  TClonesArray* fGlobalTracks;
  TClonesArray* fJpsiCandidates; //output array of Jpsi candidates


  // Number of hits in the MC RICH ring
  std::map<Int_t, Int_t> fNofHitsInRingMap;
  
  CbmVertex* fPrimVertex;
  CbmKFVertex fKFVertex;

  std::vector<CbmAnaJpsiCandidate> fCandidates; // array of JPsi candidates

  CbmAnaJpsiCuts fCuts; //electron identification and analysis cuts

  Double_t fWeight; // Multiplicity * Branching Ratio of J/Psi;

  CbmHistManager* fHM;

  Bool_t fUseTrd;
  Bool_t fUseTof;

  /*
       * \brief Creates 1D histograms for each analysis step.
       * \param[in] name Base name of the histograms.
       * \param[in] axisX X axis title.
       * \param[in] axisY Y axis title.
       * \param[in] nBins Number of bins for X axis in the histogram.
       * \param[in] min Minimum value for X axis.
       * \param[in] max Maximum value for X axis.
       */
  void CreateAnalysisStepsH1(
        const string& name,
        const string& axisX,
        const string& axisY,
        double nBins,
        double min,
        double max);

  /*
       * \brief Creates 2D histograms for each analysis step.
       * \param[in] name Base name of the histograms.
       * \param[in] axisX X axis title.
       * \param[in] axisY Y axis title.
       * \param[in] axisZ Z axis title.
       * \param[in] nBinsX Number of bins for X axis in the histogram.
       * \param[in] minX Minimum value for X axis.
       * \param[in] maxX Maximum value for X axis.
       * \param[in] nBinsY Number of bins for Y axis in the histogram.
       * \param[in] minY Minimum value for Y axis.
       * \param[in] maxY Maximum value for Y axis.
       */
  void CreateAnalysisStepsH2(
        const string& name,
        const string& axisX,
        const string& axisY,
        const string& axisZ,
        double nBinsX,
        double minX,
        double maxX,
        double nBinsY,
        double minY,
        double maxY);


  /*
   * \brief Creates 1D histograms for different track source types.
   * \param[in] name Base name of the histograms.
   * \param[in] axisX X axis title.
   * \param[in] axisY Y axis title.
   * \param[in] nBins Number of bins in the histogram.
   * \param[in] min Minimum value.
   * \param[in] max Maximum value.
   */
  void CreateSourceTypesH1(
        const string& name,
        const string& axisX,
        const string& axisY,
        double nBins,
        double min,
        double max);


  /*
   * \brief Creates 2D histograms for different track source types.
   * \param[in] name Base name of the histograms.
   * \param[in] axisX X axis title.
   * \param[in] axisY Y axis title.
   * \param[in] axisZ Z axis title.
   * \param[in] nBinsX Number of bins for X axis in the histogram.
   * \param[in] minX Minimum value for X axis.
   * \param[in] maxX Maximum value for X axis.
   * \param[in] nBinsY Number of bins for Y axis in the histogram.
   * \param[in] minY Minimum value for Y axis.
   * \param[in] maxY Maximum value for Y axis.
   */
  void CreateSourceTypesH2(
        const string& name,
        const string& axisX,
        const string& axisY,
        const string& axisZ,
        double nBinsX,
        double minX,
        double maxX,
        double nBinsY,
        double minY,
        double maxY);

  void CreateAnaStepsPairSourceH1(
        const string& name,
        const string& axisX,
        const string& axisY,
        double nBins,
        double min,
        double max);

    void InitHist();

    void FillRichRingNofHits();

    void MCPairs();

    void RichPmtXY();

    /**
     * \brief Fill fCandidates array with JPsiCandidates. Candidate should have STS, RICH, TRD, TOF local segments.
     *
     */
    void FillCandidates();

    /**
     * \brief Assign MC info to the candidates.
     */
    void AssignMcToCandidates();

    /**
     * \brief Return true if MC track is in detector acceptance
     */
    Bool_t IsMcTrackAccepted(
    		Int_t mcTrackInd);

    void IsRecoTrackAccepted(
    		CbmAnaJpsiCandidate* cand);

    void SingleParticleAcceptance();

    /**
     * \brief Fill histograms for MC and Acc pairs
     */
    void PairMcAndAcceptance();

    void PairSource(
    		  CbmAnaJpsiCandidate* candP,
    		  CbmAnaJpsiCandidate* candM,
    	      CbmAnaJpsiAnalysisSteps step,
    		  CbmAnaJpsiKinematicParams* parRec);

    void TrackSource(
    		CbmAnaJpsiCandidate* cand,
			CbmAnaJpsiAnalysisSteps step,
			Int_t pdg);

    void FillPairHists(
    	  CbmAnaJpsiCandidate* candP,
          CbmAnaJpsiCandidate* candM,
    	  CbmAnaJpsiKinematicParams* parMc,
    	  CbmAnaJpsiKinematicParams* parRec,
          CbmAnaJpsiAnalysisSteps step);

    void SignalAndBgReco();

    /**
     * \brief  Identifies particle as Electron (or not)
     */
    void IsElectron(
    		Int_t globalTrackIndex,
    		Double_t momentum,
    		CbmAnaJpsiCandidate* cand);


    void IsMismatch(
    		CbmAnaJpsiCandidate* cand);

    /**
     * \brief Fill histograms for signal and BG electrons
     */
    void DifferenceSignalAndBg();

    /*
     * \brief Copy candidates to the output array.
     */
    void CopyCandidatesToOutputArray();


   /**
    * \brief Copy constructor.
    */
   CbmAnaJpsiTask(const CbmAnaJpsiTask&);

   /**
    * \brief Assignment operator.
    */
   CbmAnaJpsiTask& operator=(const CbmAnaJpsiTask&);


	ClassDef(CbmAnaJpsiTask,1)
};

#endif

