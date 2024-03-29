/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmCaInputQaTrd.h
/// \date   13.01.2023
/// \brief  QA-task for CA tracking input from TRD detector (header)
/// \author S.Zharko <s.zharko@gsi.de>


#ifndef CbmCaInputQaTrd_h
#define CbmCaInputQaTrd_h 1

#include "CbmCaInputQaBase.h"
#include "CbmMCDataManager.h"
#include "CbmQaTask.h"

#include "TMath.h"

#include <set>
#include <unordered_map>
#include <vector>

class CbmMCEventList;
class CbmMCDataArray;
class CbmMCDataManager;
class CbmTimeSlice;
class CbmMatch;
class CbmTrdHit;
class CbmTrdTrackingInterface;
class TClonesArray;
class TH1F;
class TH2F;
class CbmQaEff;

/// A QA-task class, which provides assurance of TRD hits and geometry
class CbmCaInputQaTrd : public CbmQaTask, public CbmCaInputQaBase {
public:
  /// Constructor from parameters
  /// \param  verbose   Verbose level
  /// \param  isMCUsed  Flag, whether MC information is available for this task
  CbmCaInputQaTrd(int verbose, bool isMCUsed);

  ClassDef(CbmCaInputQaTrd, 0);

protected:
  // ********************************************
  // ** Virtual method override from CbmQaTask **
  // ********************************************

  /// Checks results of the QA
  /// \return  Success flag
  bool Check();

  /// Initializes data branches
  InitStatus InitDataBranches();

  /// Initializes canvases
  InitStatus InitCanvases();

  /// Initializes histograms
  InitStatus InitHistograms();

  /// Fills histograms per event or time-slice
  void FillHistograms();

  /// De-initializes histograms
  void DeInit();

private:
  // *********************
  // ** Private methods **
  // *********************

  // *********************
  // ** Class variables **
  // *********************


  // ** Data branches **
  CbmTrdTrackingInterface* fpDetInterface = nullptr;  ///< Instance of detector interface

  CbmTimeSlice* fpTimeSlice = nullptr;  ///< Pointer to current time-slice

  TClonesArray* fpHits = nullptr;  ///< Array of hits

  CbmMCDataManager* fpMCDataManager = nullptr;  ///< MC data manager
  CbmMCEventList* fpMCEventList     = nullptr;  ///< MC event list
  CbmMCDataArray* fpMCTracks        = nullptr;  ///< Array of MC tracks
  CbmMCDataArray* fpMCPoints        = nullptr;  ///< Array of MC points

  TClonesArray* fpHitMatches = nullptr;  ///< Array of hit matches

  std::vector<char> fvTrdStationType;  ///< Station type: TRD-1D and TRD-2D

  // ** Histograms **
  static constexpr double kEffRangeMin = 35.;  ///< Lower limit of hit distance for efficiency integration [cm]
  static constexpr double kEffRangeMax = 70.;  ///< Upper limit of hit distance for efficiency integration [cm]

  static constexpr double kRHitX[2] = {-100., 100};  ///< Range for hit x coordinate [cm]
  static constexpr double kRHitY[2] = {-100., 100};  ///< Range for hit y coordinate [cm]
  static constexpr double kRHitZ[2] = {30., 300.};   ///< Range for hit z coordinate [cm]

  static constexpr int kNbins  = 100;  ///< General number of bins
  static constexpr int kNbinsZ = 800;  ///< Number of bins for z coordinate axis

  static constexpr double kRHitDx[2] = {-.05, .005};  ///< Range for hit x coordinate [cm]
  static constexpr double kRHitDy[2] = {-.05, .005};  ///< Range for hit y coordinate [cm]
  static constexpr double kRHitDt[2] = {-10., 10.};   ///< Range for hit time [ns]

  static constexpr double kRResX[2] = {-2., 2.};    ///< Range for residual of x coordinate [cm]
  static constexpr double kRResY[2] = {-2., 2.};    ///< Range for residual of y coordinate [cm]
  static constexpr double kRResT[2] = {-20., 20.};  ///< Range for residual of time [ns]

  static constexpr double kRPullX[2] = {-10., 10.};  ///< Range for pull of x coordinate
  static constexpr double kRPullY[2] = {-10., 10.};  ///< Range for pull of y coordinate
  static constexpr double kRPullT[2] = {-5., 5.};    ///< Range for pull of time

  // NOTE: the last three elements of each vector stands for integral distribution over TRD-1D and TRD-2D

  // hit occupancy
  std::vector<TH2F*> fvph_hit_ypos_vs_xpos;  ///< hit ypos vs xpos in different stations
  std::vector<TH2F*> fvph_hit_xpos_vs_zpos;  ///< hit xpos vs zpos in different stations
  std::vector<TH2F*> fvph_hit_ypos_vs_zpos;  ///< hit ypos vs zpos in different stations

  // difference between hit and station z positions
  std::vector<TH1F*> fvph_hit_station_delta_z;  ///< Difference between station and hit z positions [cm]

  // hit errors
  std::vector<TH1F*> fvph_hit_dx;
  std::vector<TH1F*> fvph_hit_dy;
  std::vector<TH1F*> fvph_hit_dt;

  // MC points occupancy
  std::vector<TH1F*> fvph_n_points_per_hit;  ///< number of points per one hit in different stations


  std::vector<TH2F*> fvph_point_ypos_vs_xpos;  ///< point ypos vs xpos in different stations
  std::vector<TH2F*> fvph_point_xpos_vs_zpos;  ///< point xpos vs zpos in different stations
  std::vector<TH2F*> fvph_point_ypos_vs_zpos;  ///< point ypos vs zpos in different stations

  std::vector<TH1F*> fvph_point_hit_delta_z;  ///< difference between z of hit and MC point in different stations

  // Residuals
  std::vector<TH1F*> fvph_res_x;  ///< residual for x coordinate in different stations
  std::vector<TH1F*> fvph_res_y;  ///< residual for y coordinate in different stations
  std::vector<TH1F*> fvph_res_t;  ///< residual for t coordinate in different stations

  std::vector<TH2F*> fvph_res_x_vs_x;  ///< residual for x coordinate in different stations
  std::vector<TH2F*> fvph_res_y_vs_y;  ///< residual for y coordinate in different stations
  std::vector<TH2F*> fvph_res_t_vs_t;  ///< residual for t coordinate in different stations

  // Pulls
  std::vector<TH1F*> fvph_pull_x;  ///< pull for x coordinate in different stations
  std::vector<TH1F*> fvph_pull_y;  ///< pull for y coordinate in different stations
  std::vector<TH1F*> fvph_pull_t;  ///< pull for t coordinate in different stations

  std::vector<TH2F*> fvph_pull_x_vs_x;  ///< pull for x coordinate in different stations
  std::vector<TH2F*> fvph_pull_y_vs_y;  ///< pull for y coordinate in different stations
  std::vector<TH2F*> fvph_pull_t_vs_t;  ///< pull for t coordinate in different stations

  // Hit efficiencies
  std::vector<CbmQaEff*> fvpe_reco_eff_vs_xy;  ///< Efficiency of hit reconstruction vs. x and y coordinates of MC
  std::vector<CbmQaEff*> fvpe_reco_eff_vs_r;   ///< Efficiency of hit reconstruction vs. distance from center
};

#endif  // CbmCaInputQaTrd_h
