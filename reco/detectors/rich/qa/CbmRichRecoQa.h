/* Copyright (C) 2016-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Semen Lebedev */

#ifndef CBM_RICH_RECO_QA
#define CBM_RICH_RECO_QA

#include "FairTask.h"
class TClonesArray;
class CbmMCDataArray;
class CbmRichRing;
class CbmHistManager;
class TH1D;
class TH2D;
class TH2;
class TH3;
class CbmMCTrack;
class CbmDigiManager;
class CbmMCEventList;

#include <map>
#include <vector>

class CbmRichRecoQa : public FairTask {

public:
  /**
     * \brief Standard constructor.
     */
  CbmRichRecoQa();

  /**
     * \brief Standard destructor.
     */
  virtual ~CbmRichRecoQa() {}

  /**
     * \brief Inherited from FairTask.
     */
  virtual InitStatus Init();

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Exec(Option_t* option);

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Finish();

  static bool IsMcPrimaryElectron(const CbmMCTrack* mctrack);

  static bool IsMcPion(const CbmMCTrack* mctrack);

  /**
     * \brief Set output directory where you want to write results (figures and json).
     * \param[in] dir Path to the output directory.
     */
  void SetOutputDir(const std::string& dir) { fOutputDir = dir; }

  /**
         * \brief Draw histogram from file
         */
  void DrawFromFile(const std::string& fileName, const std::string& outputDir);

private:
  /**
     * \brief Initialize histograms.
     */
  void InitHistograms();

  /**
     * \brief Fill map mcTrackId -> nof RICH hits
     */
  void FillRichRingNofHits();

  /**
     * \brief Fill histogramms related to ring track distance
     */
  void FillRingTrackDistance();

  /**
     * \brief Fill histograms related to study of the source of ring-track mismatch
     */
  void RingTrackMismatchSource();

  /**
     *  \brief Draw histograms.
     */
  void DrawHist();

  /**
     * \brief Return string with mean, RMS and overflow percent for input TH1.
     */
  std::string GetMeanRmsOverflowString(TH1* h, bool withOverflow = true);

  /**
     *  \brief Draw histograms related to ring-track distance for pions or electrons (+/-).
     */
  void DrawRingTrackDist(const std::string& opt);

  /*
     * \brief Check that the ring with an input MCTrackId was found
     */
  bool WasRingFound(int mcTrackId);

  /*
     * \brief Check that the ring was matched with some global track
     */
  bool WasRingMatched(int mcTrackId);

  /*
     * \brief Check that the Sts track projection was matched with RICH ring
     */
  bool WasRichProjectionMatched(int stsTrackId);

  /*
     * Check that STS track has projection in the RICH
     */
  bool HasRichProjection(int stsTrackId);


  /**
     * \brief Copy constructor.
     */
  CbmRichRecoQa(const CbmRichRecoQa&);

  /**
     * \brief Assignment operator.
     */
  CbmRichRecoQa& operator=(const CbmRichRecoQa&);


  CbmHistManager* fHM = nullptr;

  int fEventNum = 0;

  std::string fOutputDir = "";  // output dir for results

  CbmMCDataArray* fMcTracks      = nullptr;
  CbmMCDataArray* fRichPoints    = nullptr;
  TClonesArray* fRichHits        = nullptr;
  TClonesArray* fRichRings       = nullptr;
  TClonesArray* fRichRingMatches = nullptr;
  TClonesArray* fGlobalTracks    = nullptr;
  TClonesArray* fStsTracks       = nullptr;
  TClonesArray* fStsTrackMatches = nullptr;
  TClonesArray* fRichProjections = nullptr;
  CbmDigiManager* fDigiMan       = nullptr;
  CbmMCEventList* fEventList     = nullptr;

  // Number of hits in the MC RICH ring
  std::map<std::pair<int, int>, int> fNofHitsInRingMap;

  ClassDef(CbmRichRecoQa, 1)
};

#endif
