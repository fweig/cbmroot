/** CbmAnaDielectronTask.h
 * @author Elena Lebedeva <e.lebedeva@gsi.de>
 * @since 2010
 * @version 3.0
 **/

#ifndef CBM_ANA_DIELECTRON_TASK_H
#define CBM_ANA_DIELECTRON_TASK_H

#include "CbmGlobalTrack.h"
#include "CbmKFVertex.h"
#include "CbmLmvmCandidate.h"
#include "CbmLmvmCuts.h"
#include "CbmLmvmHist.h"
#include "CbmLmvmKinematicParams.h"
#include "CbmMCTrack.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmStsTrack.h"
#include "CbmTrdTrack.h"
#include "CbmVertex.h"
#include "FairBaseParSet.h"
#include "FairMCEventHeader.h"
#include "FairTask.h"
#include "cbm/qa/mc/CbmLitMCTrackCreator.h"

#include <fstream>
#include <map>
#include <string>
#include <vector>

class TClonesArray;
class TH2D;
class TH1D;
class TH2F;
class TRandom3;
class FairRootManager;


class CbmAnaDielectronTask : public FairTask {

public:
  /*
    * \brief Standard constructor.
    */
  CbmAnaDielectronTask();

  /*
     * \brief Standard destructor.
     */
  virtual ~CbmAnaDielectronTask();

  /*
     * \brief Inherited from FairTask.
     */
  virtual InitStatus Init();

  /*
     * \brief Inherited from FairTask.
     */
  virtual void Exec(Option_t* option);

  /*
     * \brief Creates 1D histograms for each analysis step.
     * \param[in,out] hist Vector if the histograms for each analysis step.
     * \param[in] name Base name of the histograms.
     * \param[in] axisX X axis title.
     * \param[in] axisY Y axis title.
     * \param[in] nBins Number of bins in the histogram.
     * \param[in] min Minimum value.
     * \param[in] max Maximum value.
     */
  void CreateAnalysisStepsH1(std::vector<TH1D*>& hist,
                             const std::string& name,
                             const std::string& axisX,
                             const std::string& axisY,
                             double nBins,
                             double min,
                             double max);


  /*
     * \brief Creates 2D histograms for each analysis step.
     * \param[in,out] hist Vector if the histograms for each analysis step.
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
  void CreateAnalysisStepsH2(std::vector<TH2D*>& hist,
                             const std::string& name,
                             const std::string& axisX,
                             const std::string& axisY,
                             const std::string& axisZ,
                             double nBinsX,
                             double minX,
                             double maxX,
                             double nBinsY,
                             double minY,
                             double maxY);

  /*
     * \brief Creates 1D histograms for different track source types.
     * \param[in,out] hist Vector if the histograms for each analysis step.
     * \param[in] name Base name of the histograms.
     * \param[in] axisX X axis title.
     * \param[in] axisY Y axis title.
     * \param[in] nBins Number of bins in the histogram.
     * \param[in] min Minimum value.
     * \param[in] max Maximum value.
     */
  void CreateSourceTypesH1(std::vector<TH1D*>& hist,
                           const std::string& name,
                           const std::string& axisX,
                           const std::string& axisY,
                           double nBins,
                           double min,
                           double max);


  /*
     * \brief Creates 2D histograms for different track source types.
     * \param[in,out] hist Vector if the histograms for each analysis step.
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
  void CreateSourceTypesH2(std::vector<TH2D*>& hist,
                           const std::string& name,
                           const std::string& axisX,
                           const std::string& axisY,
                           const std::string& axisZ,
                           double nBinsX,
                           double minX,
                           double maxX,
                           double nBinsY,
                           double minY,
                           double maxY);


  /*
     * \brief Fills histograms for pairs.
     * \param[in] candP Positive candidate.
     * \param[in] candM Negative candidate.
     * \param[in] step Enumeration AnalysisSteps, specify analysis step.
     * \param[in] parRec Kinematic parameters for reconstructed pair.
     */
  void PairSource(CbmLmvmCandidate* candP,
                  CbmLmvmCandidate* candM,
                  CbmLmvmAnalysisSteps step,
                  CbmLmvmKinematicParams* parRec);

  /*
     * \brief Fills minv, pty, mom histograms for specified analysis step.
     * \param[in] candP Positive candidate.
     * \param[in] candM Negative candidate.
     * \param[in] parMc MC kinematic parameters.
     * \param[in] parRec Reconstructed kinematic parameters.
     * \param[in] step Enumeration AnalysisSteps, specify analysis step.
     */
  void FillPairHists(CbmLmvmCandidate* candP,
                     CbmLmvmCandidate* candM,
                     CbmLmvmKinematicParams* parMc,
                     CbmLmvmKinematicParams* parRec,
                     CbmLmvmAnalysisSteps step);

  void
  TrackSource(CbmLmvmCandidate* cand, CbmLmvmAnalysisSteps step, Int_t pdg);

  void SingleParticleAcceptance();

  void FillRichRingNofHits();

  Bool_t IsMcTrackAccepted(Int_t mcTrackInd);

  void RichPmtXY();

  void MCPairs();

  void PairMcAndAcceptance();

  void FillTopologyCandidates();

  void FillCandidates();


  void AssignMcToCandidates();

  void
  AssignMcToTopologyCandidates(std::vector<CbmLmvmCandidate>& cutCandidates);

  void DifferenceSignalAndBg();

  /*
     * \brief Initialize all histograms.
     */
  void InitHists();

  void SignalAndBgReco();

  void CheckGammaConvAndPi0();

  void FillNofChargedParticlesPerEvent();

  /*
     * \brief
     * \param[in] mvdStationNum MVD station number.
     * \param[in, out] hist Vector of histograms for different source types.
     */
  void CheckClosestMvdHit(Int_t mvdStationNum,
                          std::vector<TH2D*>& hist,
                          std::vector<TH1D*>& histQa);

  /*
     * \brief Set cut values and fill histograms for topology cut
     * \param[in] cutName ST or TT
     */
  void CheckTopologyCut(const std::string& cutName,
                        const std::vector<CbmLmvmCandidate>& cutCandidates,
                        const std::vector<TH2D*>& hcut,
                        const std::vector<TH2D*>& hcutPion,
                        const std::vector<TH2D*>& hcutTruepair,
                        Double_t angleCut,
                        Double_t ppCut);

  void CalculateNofTopologyPairs(TH1D* h_nof_pairs, const std::string& source);

  void MvdCutMcDistance();

  // Likelihood vs Momentum
  void FillMomLikeHist();

  virtual void Finish();

  void FillElPiMomHist();


  ClassDef(CbmAnaDielectronTask, 1);

private:
  CbmAnaDielectronTask(const CbmAnaDielectronTask&);
  CbmAnaDielectronTask& operator=(const CbmAnaDielectronTask&);

  Bool_t IsMismatch(CbmLmvmCandidate* cand);

  Bool_t IsGhost(CbmLmvmCandidate* cand);

  void
  IsElectron(Int_t globalTrackIndex, Double_t momentum, CbmLmvmCandidate* cand);

  FairMCEventHeader* fMCEventHeader;
  TClonesArray* fMCTracks;
  TClonesArray* fRichRings;
  TClonesArray* fRichProj;
  TClonesArray* fRichPoints;
  TClonesArray* fRichRingMatches;
  TClonesArray* fRichHits;
  TClonesArray* fGlobalTracks;
  TClonesArray* fStsTracks;
  TClonesArray* fStsTrackMatches;
  TClonesArray* fStsHits;
  TClonesArray* fMvdHits;
  TClonesArray* fMvdPoints;
  TClonesArray* fMvdHitMatches;
  TClonesArray* fTrdTracks;
  TClonesArray* fTrdHits;
  TClonesArray* fTrdTrackMatches;
  TClonesArray* fTofHits;
  TClonesArray* fTofHitsMatches;
  TClonesArray* fTofPoints;
  CbmVertex* fPrimVertex;
  CbmKFVertex fKFVertex;
  CbmStsKFTrackFitter fKFFitter;

  //CbmLitMCTrackCreator* fMCTrackCreator; // MC track creator tool

  Bool_t fUseMvd;
  Bool_t fUseRich;
  Bool_t fUseTrd;
  Bool_t fUseTof;

  std::vector<CbmLmvmCandidate> fCandidates;
  std::vector<CbmLmvmCandidate>
    fSTCandidates;  // STCut Segmented tracks, reconstructed only in STS
  std::vector<CbmLmvmCandidate>
    fTTCandidates;  // TTCut Reconstructed tracks, reconstructed in all detectors but not identified as electrons
  std::vector<CbmLmvmCandidate>
    fRTCandidates;  // RTCut Reconstructed tracks, reconstructed in STS + at least in one of the  detectro (RICH, TRD, TOF)

  Double_t fWeight;  //Multiplicity*BR

  Double_t
    fPionMisidLevel;  // For the ideal particle identification cases, set to -1 for real PID
  TRandom3* fRandom3;

  //Bool_t fUseMcMomentum;

  CbmLmvmCuts fCuts;  // electorn identification and analisys cuts

  std::vector<TH1*> fHistoList;  //list of all histograms

  // Number of hits in the MC RICH ring
  std::map<Int_t, Int_t> fNofHitsInRingMap;

  TH2D* fh_mc_signal_mom_angle;  // angle vs. sqrt(mom1*mom2) with MCTracks

  TH1D* fh_nof_charged_particles;      //charged UrQMD particles
  TH1D* fh_nof_charged_particles_acc;  //accepted charged UrQMD particles

  TH1D* fh_mc_mother_pdg;   //mother pdg code for e-/e+
  TH1D* fh_acc_mother_pdg;  //mother pdg code for accepted e-/e+

  // X-Y distribution of MC pints on the RICH PMT plane
  TH2D* fh_signal_pmtXY;
  TH2D* fh_pi0_pmtXY;
  TH2D* fh_gamma_pmtXY;

  // Vertex of secondary electron from gamma conversion for different analysis step
  //Index is the analysis step: [0]-mc, [1]-acc, [2]-reco, [3]-chi2prim, [4]-elid,
  // [5]-gamma cut, [6]-mvd1cut, [7]-mvd2cut, [8]-stcut, [9]-ttcut, [10]-ptcut.
  std::vector<TH2D*> fh_vertex_el_gamma_xz;
  std::vector<TH2D*> fh_vertex_el_gamma_yz;
  std::vector<TH2D*> fh_vertex_el_gamma_xy;
  std::vector<TH2D*> fh_vertex_el_gamma_rz;  //r=sqrt(x^2+y^2)

  //Index is the analysis step: [0]-mc, [1]-acc, [2]-reco, [3]-chi2prim, [4]-elid,
  // [5]-gamma cut, [6]-mvd1cut, [7]-mvd2cut, [8]-stcut, [9]-ttcut, [10]-ptcut.
  //Use AnalysisSteps enumeration for access.
  //MC and ACC histograms are not filled sometimes.
  std::vector<TH1D*> fh_signal_minv;     // Invariant mass for Signal
  std::vector<TH1D*> fh_bg_minv;         // Invariant mass for BG
  std::vector<TH1D*> fh_pi0_minv;        // Invariant mass for Pi0
  std::vector<TH1D*> fh_eta_minv;        // Invariant mass for Eta
  std::vector<TH1D*> fh_gamma_minv;      // Invariant mass for Eta
  std::vector<TH1D*> fh_signal_mom;      // Signal momentum distribution
  std::vector<TH2D*> fh_signal_pty;      // Pt/y distribution for signal
  std::vector<TH2D*> fh_signal_minv_pt;  // Invariant mass vs. MC Pt
  std::vector<TH2D*> fh_eta_minv_pt;     // Invariant mass vs. MC Pt
  std::vector<TH2D*> fh_pi0_minv_pt;     // Invariant mass vs. MC Pt


  std::vector<TH1D*>
    fh_bg_truematch_minv;  // Invariant mass for truly matched tracks
  std::vector<TH1D*>
    fh_bg_truematch_el_minv;  // Invariant mass for truly matched electron tracks
  std::vector<TH1D*>
    fh_bg_truematch_notel_minv;  // Invariant mass for truly matched tracks, not 2 electrons
  std::vector<TH1D*>
    fh_bg_mismatch_minv;  // Invariant mass for mis matches tracks

  //G-Gamma, P-Pi0, O-other
  //e-e+
  //[0]=G-G, [1]=P-P, [2]=O-O, [3]=G-P, [4]=G-O, [5]=P-O
  std::vector<std::vector<TH1D*>>
    fh_source_bg_minv;  // Invariant mass for different source


  //Index is the source type: [0]-signal, [1]-bg, [2]-pi0, [3]-gamma
  //Use SourceTypes enumeration for access.
  std::vector<TH1D*> fh_pt;  // Transverse momentum of single track distribution
  std::vector<TH1D*> fh_mom;       //Momentum of the single track
  std::vector<TH1D*> fh_chi2sts;   // Chi2 of the STS tracks
  std::vector<TH1D*> fh_chi2prim;  // Chi2 of the primary vertex
  std::vector<TH2D*> fh_ttcut;     // TT cut
  std::vector<TH2D*> fh_stcut;     // ST cut
  std::vector<TH2D*> fh_rtcut;     // RT cut
  std::vector<TH2D*> fh_mvd1cut;   // MVD cut at the first station
  std::vector<TH2D*> fh_mvd2cut;   // MVD cut at the second station
  std::vector<TH1D*> fh_richann;   // RICH ANN
  std::vector<TH1D*> fh_trdann;    // TRD ANN
  std::vector<TH2D*> fh_tofm2;     // TOF m2
  std::vector<TH2D*> fh_ttcut_pion;
  std::vector<TH2D*> fh_ttcut_truepair;
  std::vector<TH2D*> fh_stcut_pion;
  std::vector<TH2D*> fh_stcut_truepair;
  std::vector<TH2D*> fh_rtcut_pion;
  std::vector<TH2D*> fh_rtcut_truepair;

  std::vector<TH1D*> fh_nofMvdHits;  // number of MVD hits
  std::vector<TH1D*> fh_nofStsHits;  // number of STS hits
  std::vector<TH2D*> fh_mvd1xy;  // hit distribution in the first MVD station
  std::vector<TH1D*> fh_mvd1r;   // r = x^2+y^2
  std::vector<TH2D*> fh_mvd2xy;  // hit distribution in the second MVD station
  std::vector<TH1D*> fh_mvd2r;   // r = x^2+y^2

  //Distant to MVD hit from the same  MotherId
  TH1D* fh_mvd1cut_mc_dist_gamma;
  TH1D* fh_mvd1cut_mc_dist_pi0;
  TH1D* fh_mvd2cut_mc_dist_gamma;
  TH1D* fh_mvd2cut_mc_dist_pi0;

  std::vector<TH1D*> fh_mvd1cut_qa;  // MVD 1 cut quality
  std::vector<TH1D*> fh_mvd2cut_qa;  // MVD 2 cut quality

  //source of BG pairs 2D.
  //second index is the analysis step: [0]-mc, [1]-acc, [2]-reco, [3]-chi2prim, [4]-elid,
  // [5]-gamma cut, [6]-mvd1cut, [7]-mvd2cut, [8]-stcut, [9]-ttcut, [10]-ptcut.
  //Use AnalysisSteps enumeration for access.
  std::vector<TH2D*> fh_source_pairs_epem;

  //X axis: analysis step
  //Y axis: [0]=G-G, [1]=P-P, [2]=O-O, [3]=G-P, [4]=G-O, [5]=P-O
  TH2D* fh_source_pairs;

  //store event number
  TH1D* fh_event_number;

  //nof signal and bg tracks after each cut
  TH1D* fh_nof_bg_tracks;
  TH1D* fh_nof_el_tracks;
  TH2D* fh_source_tracks;

  TH1D* fh_nof_topology_pairs_gamma;
  TH1D* fh_nof_topology_pairs_pi0;

  //nof gamma and pi0 pairs for different track categories : global, only STS or partially reconstructed
  TH1D* fh_nof_rec_pairs_gamma;
  TH1D* fh_nof_rec_pairs_pi0;

  //nof gamma and pi0 tracks for different track categories : global, only STS or partially reconstructed
  TH1D* fh_nof_rec_gamma;
  TH1D* fh_nof_rec_pi0;

  TH1D* fh_nof_mismatches;
  TH1D* fh_nof_mismatches_rich;
  TH1D* fh_nof_mismatches_trd;
  TH1D* fh_nof_mismatches_tof;
  TH1D* fh_nof_ghosts;

  //First index is the source type: [0]-signal, [1]-bg, [2]-pi0, [3]-gamma
  //Use SourceTypes enumeration for access.
  //second index is the analysis step: [0]-mc, [1]-acc, [2]-reco, [3]-chi2prim, [4]-elid,
  // [5]-gamma cut, [6]-mvd1cut, [7]-mvd2cut, [8]-stcut, [9]-ttcut, [10]-ptcut.
  //Use AnalysisSteps enumeration for access.
  //Track momentum distribution for different sources after each cut.
  std::vector<std::vector<TH1D*>> fh_source_mom;
  //Pt distribution for different sources after each cut.
  std::vector<std::vector<TH1D*>> fh_source_pt;
  //Opening angle distribution for different sources after each cut.
  std::vector<std::vector<TH1D*>> fh_opening_angle;

  //Pions vs momentum
  TH1D* fh_pi_mom_mc;
  TH1D* fh_pi_mom_acc;
  TH1D* fh_pi_mom_rec;
  TH1D* fh_pi_mom_rec_only_sts;
  TH1D* fh_pi_mom_rec_sts_rich_trd;
  TH1D* fh_pi_mom_rec_sts_rich_trd_tof;
  TH1D* fh_pi_rapidity_mc;

  //Pions vs momentum for primary pions v < 0.1 cm
  TH1D* fh_piprim_mom_mc;
  TH1D* fh_piprim_mom_acc;
  TH1D* fh_piprim_mom_rec;
  TH1D* fh_piprim_mom_rec_only_sts;
  TH1D* fh_piprim_mom_rec_sts_rich_trd;
  TH1D* fh_piprim_mom_rec_sts_rich_trd_tof;

  TH1D* fh_piprim_plus_rapidity_mc;
  TH1D* fh_piprim_minus_rapidity_mc;
  TH1D* fh_pi0prim_rapidity_mc;
  TH1D* fh_etaprim_rapidity_mc;

  // Likelihood vs Momentum
  TH2D* fh_mom_likelihood_El;
  TH2D* fh_mom_likelihood_Pi;

public:
  void SetUseMvd(Bool_t use) { fUseMvd = use; };
  void SetUseRich(Bool_t use) { fUseRich = use; };
  void SetUseTrd(Bool_t use) { fUseTrd = use; };
  void SetUseTof(Bool_t use) { fUseTof = use; };
  void SetWeight(Double_t weight) { fWeight = weight; };
  void SetEnergyAndPlutoParticle(const string& energy, const string& particle);

  void SetPionMisidLevel(Double_t level) { fPionMisidLevel = level; }
  // void SetMomentumCut(Double_t mom) {fMomentumCut = mom;}
};

#endif
