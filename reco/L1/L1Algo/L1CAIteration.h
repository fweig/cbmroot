/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */


/************************************************************************************************************
 * @file L1CAIteration.h
 * @brief Class for L1 CA Track Finder Iteration 
 * @since 01.15.2022
 *
 ***********************************************************************************************************/

#ifndef L1CAIteration_h
#define L1CAIteration_h 1

#include <string>

/// All possible iteration types of Track Finder algorithm to select
///
//enum class L1CAIterationType;  // definition in CbmL1/BmnL1
//{
//  kFastPrim,      ///< primary fast tracks
//  kAllPrim,       ///< primary all tracks
//  kAllPrimJump,   ///< primary all tracks with jumped triplets
//  kAllSec,        ///< secondary all tracks
//  kAllPrimE,      ///< primary all electron tracks
//  kAllSecE,       ///< secondary all electron tracks
//  kFastPrimJump,  ///< primary fast tracks with jumped triplets
//  kAllSecJump,    ///< secondary all tracks with jumped triplets
//  kEND            ///< dummy item, represents number of iterations
//};

// TODO: insert names into separate structure

/// Class describes L1 Track finder iteration
///
/// The class contains different for
class L1CAIteration {
public:
  /// Default constructor
  L1CAIteration() noexcept;
  /// Destructor
  ~L1CAIteration() noexcept;
  /// Copy constructor
  L1CAIteration(const L1CAIteration& other) noexcept;
  /// Move constructor
  L1CAIteration(L1CAIteration&& other) noexcept;
  /// Copy operator=
  L1CAIteration& operator=(const L1CAIteration& other) noexcept;
  /// Move operator=
  L1CAIteration& operator=(L1CAIteration&& other) noexcept;
  /// Constructor from L1CAIteration type
  L1CAIteration(const std::string& name) noexcept;
  /// Swap method
  void Swap(L1CAIteration& other) noexcept;

  //
  // BASIC METHODS
  //

  /// Prints iteration options
  void Print(int verbosityLevel = 0) const;

  //
  // SETTERS
  //

  //-------------------------------------------------------------------------------------------------------------------
  // Basic fields setters
  //-------------------------------------------------------------------------------------------------------------------

  /// Sets type of the iteration
  //void SetType(L1CAIterationType type) { fType = type; }
  /// Sets name of the iteration
  void SetName(const std::string& name) { fName = name; }

  //-------------------------------------------------------------------------------------------------------------------
  // Cuts setters
  //-------------------------------------------------------------------------------------------------------------------

  /// Sets track chi2 upper cut
  void SetTrackChi2Cut(float input) { fTrackChi2Cut = input; }
  /// Sets triplet chi2 upper cut
  void SetTripletChi2Cut(float input) { fTripletChi2Cut = input; }
  /// Sets doublet chi2 upper cut
  void SetDoubletChi2Cut(float input) { fDoubletChi2Cut = input; }
  ///
  void SetPickGather(float input) { fPickGather = input; }
  ///
  void SetPickNeighbour(float input) { fPickNeighbour = input; }
  ///
  void SetMaxInvMom(float input) { fMaxInvMom = input; }
  ///
  void SetMaxSlopePV(float input) { fMaxSlopePV = input; }
  ///
  void SetMaxSlope(float input) { fMaxSlope = input; }

  //
  // GETTERS
  //

  //-------------------------------------------------------------------------------------------------------------------
  // Basic fields getters
  //-------------------------------------------------------------------------------------------------------------------

  /// Gets the type of the iteration
  //L1CAIterationType GetType() const { return fType; }
  /// Gets the name of the iteration
  const std::string& GetName() const { return fName; }

  //-------------------------------------------------------------------------------------------------------------------
  // Cuts getters
  //-------------------------------------------------------------------------------------------------------------------

  /// Gets track chi2 upper cut
  float GetTrackChi2Cut() const { return fTrackChi2Cut; }
  /// Gets triplet chi2 upper cut
  float GetTripletChi2Cut() const { return fTripletChi2Cut; }
  /// Gets doublet chi2 upper cut
  float GetDoubletChi2Cut() const { return fDoubletChi2Cut; }
  ///
  float GetPickGather() const { return fPickGather; }
  ///
  float GetPickNeighbour() const { return fPickNeighbour; }
  ///
  float GetMaxInvMom() const { return fMaxInvMom; }
  ///
  float GetMaxSlopePV() const { return fMaxSlopePV; }
  ///
  float GetMaxSlope() const { return fMaxSlope; }

private:
  //-------------------------------------------------------------------------------------------------------------------
  // Basic fields
  //-------------------------------------------------------------------------------------------------------------------

  //L1CAIterationType fType {static_cast<L1CAIterationType>(0)};  ///< Iteration type
  std::string fName {""};  ///< Iteration name

  //-------------------------------------------------------------------------------------------------------------------
  // Track finder dependent cuts
  //-------------------------------------------------------------------------------------------------------------------

  // TODO: Iteratively change the literals to floats
  // TODO: Change the names of variables
  float fTrackChi2Cut {10.f};                   ///> track chi2 upper cut
  float fTripletChi2Cut {21.1075f};             ///> triplet chi2 upper cut
  float fDoubletChi2Cut {11.3449 * 2.f / 3.f};  ///> doublet chi2 upper cut
  float fPickGather {3.0};                      ///> size of region to attach new hits to the created track
  float fPickNeighbour {5.0};
  float fMaxInvMom {1.0 / 0.5};
  float fMaxSlopePV {1.1};
  float fMaxSlope {2.748};
};

#endif  // L1CAIteration_h
