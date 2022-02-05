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

/// All possible iteration types of Track Finder algorithm to select  
///
enum class L1CAIterationType {
  kFastPrim,       ///< primary fast tracks
  kAllPrim,        ///< primary all tracks
  kAllPrimJump,    ///< primary all tracks with jumped triplets
  kAllSec,         ///< secondary all tracks
  kAllPrimE,       ///< primary all electron tracks
  kAllSecE,        ///< secondary all electron tracks 
  kFastPrimJump,   ///< primary fast tracks with jumped triplets
  kAllSecJump      ///< secondary all tracks with jumped triplets
};


/// Class describes L1 Track finder iteration
///
class L1CAIteration {
public:
  L1CAIteration() = default;
  ~L1CAIteration() = default;
  L1CAIteration(const L1CAIteration& /*other*/) = default;
  L1CAIteration(L1CAIteration&& /*other*/) = default;
  L1CAIteration& operator=(const L1CAIteration& /*other*/) = default;
  L1CAIteration& operator=(L1CAIteration&& /*other*/) = default;

  L1CAIteration(const std::string& /*name*/, L1CAIterationType /*type*/);

  //
  // BASIC METHODS
  //

  /// Prints iteration options
  void Print() const;

  //
  // SETTERS
  //

  //-------------------------------------------------------------------------------------------------------------------
  // Basic fields setters
  //-------------------------------------------------------------------------------------------------------------------

  /// Sets name for this iteration, the name should be unique
  // TODO: develope naming system and checkers
  void SetName(const std::string& name) { fName = name; }
  /// Sets type of the iteration
  void SetType(L1CAIterationType type) { fType = type; }

  //-------------------------------------------------------------------------------------------------------------------
  // Cuts setters
  //-------------------------------------------------------------------------------------------------------------------

  /// Sets track chi2 upper cut
  void SetTrackChi2Cut(float input) { fTrackChi2Cut = input; }
  /// Sets triplet chi2 upper cut
  void SetTripletChi2Cut(float input) { fTripletChi2Cut = input; }
  /// Sets doublet chi2 upper cut
  void SetDoubletChi2Cut(float input) { fDoubletChi2Cut = input; }


  //
  // GETTERS
  //

  //-------------------------------------------------------------------------------------------------------------------
  // Basic fields getters
  //-------------------------------------------------------------------------------------------------------------------

  /// Gets the name of the iteration
  std::string GetName() const { return fName; }
  /// Gets the type of the iteration
  L1CAIterationType GetType() { return fType; }

  //-------------------------------------------------------------------------------------------------------------------
  // Cuts getters
  //-------------------------------------------------------------------------------------------------------------------

  /// Gets track chi2 upper cut
  float GetTrackChi2Cut() const { return fTrackChi2Cut; }
  /// Gets triplet chi2 upper cut
  float GetTripletChi2Cut() const { return fTripletChi2Cut; }
  /// Gets doublet chi2 upper cut
  float GetDoubletChi2Cut() const { return fDoubletChi2Cut; }

private:
  
  //-------------------------------------------------------------------------------------------------------------------
  // Basic fields                                                   
  //-------------------------------------------------------------------------------------------------------------------

  std::string fName;  ///< Iteration name
  L1CAIterationType fType;  ///< Iteration type

  //-------------------------------------------------------------------------------------------------------------------
  // Track finder dependent cuts                                                   
  //-------------------------------------------------------------------------------------------------------------------
  
  float fTrackChi2Cut;  ///> track chi2 upper cut
  float fTripletChi2Cut;  ///> triplet chi2 upper cut
  float fDoubletChi2Cut;  ///> doublet chi2 upper cut

  

};

#endif // L1CAIteration_h
