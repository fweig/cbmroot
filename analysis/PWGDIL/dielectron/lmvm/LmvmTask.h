/* Copyright (C) 2010-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Elena Lebedeva, Florian Uhlig */

#ifndef LMVM_TASK_H
#define LMVM_TASK_H

#include "CbmKFVertex.h"
#include "CbmStsKFTrackFitter.h"

#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "FairTask.h"

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "LmvmCand.h"
#include "LmvmCuts.h"
#include "LmvmDef.h"
#include "LmvmHist.h"
#include "LmvmKinePar.h"

class TClonesArray;
class TH2D;
class TH1D;
class TH2F;
class TRandom3;


class LmvmTask : public FairTask {

public:
  /*
    * \brief Standard constructor.
    */
  LmvmTask();

  /*
     * \brief Standard destructor.
     */
  virtual ~LmvmTask();

  /*
     * \brief Inherited from FairTask.
     */
  virtual InitStatus Init();

  /*
     * \brief Inherited from FairTask.
     */
  virtual void Exec(Option_t* option);

  template<typename T>
  T* InitOrFatal(const std::string& name)
  {
    FairRootManager* ioman = FairRootManager::Instance();
    if (ioman == nullptr) { LOG(fatal) << "LmvmTask::Init No FairRootManager!"; }
    T* array = static_cast<T*>(ioman->GetObject(name.c_str()));
    if (array == nullptr) { LOG(fatal) << "LmvmTask::Init No " << name << " object!"; }
    return array;
  }

  /*
     * \brief Fills histograms for pairs.
     * \param[in] candP Positive candidate.
     * \param[in] candM Negative candidate.
     * \param[in] step Enumeration AnalysisSteps, specify analysis step.
     * \param[in] parRec Kinematic parameters for reconstructed pair.
     */
  void PairSource(const LmvmCand& candP, const LmvmCand& candM, ELmvmAnaStep step, const LmvmKinePar& parRec);

  /*
     *  \brief Fills minv, pty, mom histograms for specified analysis step.
     * \param[in] candP Positive candidate.
     * \param[in] candM Negative candidate.
     * \param[in] parMc MC kinematic parameters.
     * \param[in] parRec Reconstructed kinematic parameters.
     * \param[in] step Enumeration AnalysisSteps, specify analysis step.
     */
  void FillPairHists(const LmvmCand& candP, const LmvmCand& candM, const LmvmKinePar& parMc, const LmvmKinePar& parRec,
                     ELmvmAnaStep step);

  void FillMomHists(const CbmMCTrack* mct, const LmvmCand* cand, ELmvmSrc src, ELmvmAnaStep step);

  void TrackSource(const LmvmCand& cand, ELmvmAnaStep step, int pdg);

  void BgPairPdg(const LmvmCand& candP, const LmvmCand& candM, ELmvmAnaStep step);

  void DoMcTrack();

  void DoMcPair();

  void FillRichRingNofHits();

  bool IsMcTrackAccepted(int mcTrackInd);

  void RichPmtXY();

  void FillTopologyCands();

  void FillCands();

  void AssignMcToCands(std::vector<LmvmCand>& cands);

  void AssignMcToTopologyCands(std::vector<LmvmCand>& topoCands);

  void DifferenceSignalAndBg();

  /*
     * \brief Initialize all histograms.
     */
  void InitHists();

  void SignalAndBgReco();

  void CheckGammaConvAndPi0();

  /*
     * \brief
     * \param[in] mvdStationNum MVD station number.
     * \param[in, out] hist Vector of histograms for different source types.
     */
  void CheckClosestMvdHit(int mvdStationNum, const std::string& hist, const std::string& histQa);

  /*
     * \brief Set cut values and fill histograms for topology cut
     * \param[in] cutName ST or TT
     */
  void CheckTopologyCut(ELmvmTopologyCut cut, const std::string& name);

  void CalculateNofTopologyPairs(const std::string& name, ELmvmSrc src);

  void MvdCutMcDistance();

  void CombinatorialPairs();

  virtual void Finish();

  void FillAccRecVsMomHist();


  ClassDef(LmvmTask, 1);

private:
  LmvmTask(const LmvmTask&);
  LmvmTask& operator=(const LmvmTask&);

  FairMCEventHeader* fMCEventHeader = nullptr;
  TClonesArray* fMCTracks           = nullptr;
  TClonesArray* fRichRings          = nullptr;
  TClonesArray* fRichProj           = nullptr;
  TClonesArray* fRichPoints         = nullptr;
  TClonesArray* fRichRingMatches    = nullptr;
  TClonesArray* fRichHits           = nullptr;
  TClonesArray* fGlobalTracks       = nullptr;
  TClonesArray* fStsTracks          = nullptr;
  TClonesArray* fStsTrackMatches    = nullptr;
  TClonesArray* fStsHits            = nullptr;
  TClonesArray* fMvdHits            = nullptr;
  TClonesArray* fMvdPoints          = nullptr;
  TClonesArray* fMvdHitMatches      = nullptr;
  TClonesArray* fTrdTracks          = nullptr;
  TClonesArray* fTrdHits            = nullptr;
  TClonesArray* fTrdTrackMatches    = nullptr;
  TClonesArray* fTofHits            = nullptr;
  TClonesArray* fTofHitsMatches     = nullptr;
  TClonesArray* fTofPoints          = nullptr;
  CbmVertex* fPrimVertex            = nullptr;
  CbmKFVertex fKFVertex;
  CbmStsKFTrackFitter fKFFitter;

  bool fUseMvd = false;

  std::vector<LmvmCand> fCands;
  std::vector<LmvmCand> fCandsTotal;
  // STCut Segmented tracks, reconstructed only in STS
  std::vector<LmvmCand> fSTCands;
  // TTCut Reconstructed tracks, reconstructed in all detectors but not identified as electrons
  std::vector<LmvmCand> fTTCands;
  // RTCut Reconstructed tracks, reconstructed in STS + at least in one of the  detectro (RICH, TRD, TOF)
  std::vector<LmvmCand> fRTCands;

  double fW = 0.;  //Multiplicity*BR

  double fPionMisidLevel = -1.;  // For the ideal particle identification cases, set to -1 for real PID

  int fEventNumber = 0;  // number of current event
  LmvmCuts fCuts;        // electorn identification and analisys cuts

  LmvmHist fH;  // histogram manager

  std::map<int, int> fNofHitsInRingMap;  // Number of hits in the MC RICH ring

public:
  void SetUseMvd(bool use) { fUseMvd = use; }
  void SetWeight(double w) { fW = w; }
  void SetEnergyAndPlutoParticle(const std::string& energy, const std::string& particle);
  void SetPionMisidLevel(double level) { fPionMisidLevel = level; }
};

#endif
