/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */


/************************************************************************************************************
 * @file L1CAIteration.h
 * @brief Class for L1 CA Track Finder Iteration 
 * @since 01.15.2022
 ***********************************************************************************************************/

#ifndef L1CAIteration_h
#define L1CAIteration_h 1

#include <bitset>
#include <string>

/// Class L1CAIteration describes L1 Track finder iteration. Each iteration contains specific cuts and special
/// flags.
///
class L1CAIteration {
  enum class ControlFlag {
    kePrimary,  ///< true - track is primary, false - track is secondary (not primary)
    keEnd
  };
  using ControlFlags_t = std::bitset<static_cast<int>(ControlFlag::keEnd)>;

public:
  /// Default constructor
  L1CAIteration() noexcept;
  /// Copy constructor
  L1CAIteration(const L1CAIteration& other) noexcept;
  /// Move constructor
  L1CAIteration(L1CAIteration&& other) noexcept;
  /// Constructor from L1CAIteration type
  L1CAIteration(const std::string& name) noexcept;
  /// Destructor
  ~L1CAIteration() noexcept;
  /// Copy operator=
  L1CAIteration& operator=(const L1CAIteration& other) noexcept;
  /// Move operator=
  L1CAIteration& operator=(L1CAIteration&& other) noexcept;

  /// Gets doublet chi2 upper cut
  float GetDoubletChi2Cut() const { return fDoubletChi2Cut; }
  ///
  float GetMaxDZ() const { return fMaxDZ; }
  /// Gets max considered q/p for tracks
  float GetMaxInvMom() const { return fMaxInvMom; }
  /// Gets max slope (tx\ty) in 3d hit position of a triplet
  float GetMaxSlope() const { return fMaxSlope; }
  /// Gets max slope (tx\ty) in prim vertex
  float GetMaxSlopePV() const { return fMaxSlopePV; }
  /// Gets the name of the iteration
  const std::string& GetName() const { return fName; }
  ///
  float GetPickGather() const { return fPickGather; }
  ///
  float GetPickNeighbour() const { return fPickNeighbour; }
  /// Gets sigma target position in X direction [cm]
  float GetTargetPosSigmaX() const { return fTargetPosSigmaX; }
  /// Gets sigma target position in Y direction [cm]
  float GetTargetPosSigmaY() const { return fTargetPosSigmaY; }
  /// Gets track chi2 upper cut
  float GetTrackChi2Cut() const { return fTrackChi2Cut; }
  /// Gets triplet chi2 upper cut
  float GetTripletChi2Cut() const { return fTripletChi2Cut; }
  
  /// flag check: primary tracks - true, secondary tracks - false
  bool IsPrimary() const { return fControlFlags[static_cast<int>(ControlFlag::kePrimary)]; }

  /// Prints iteration options
  void Print(int verbosityLevel = 0) const;

  /// Sets doublet chi2 upper cut
  void SetDoubletChi2Cut(float input) { fDoubletChi2Cut = input; }
  ///
  void SetMaxDZ(float input) { fMaxDZ = input; }
  /// Sets max considered q/p for tracks
  void SetMaxInvMom(float input) { fMaxInvMom = input; }
  /// Sets max slope (tx\ty) in 3d hit position of a triplet
  void SetMaxSlope(float input) { fMaxSlope = input; }
  /// Sets max slope (tx\ty) in prim vertex
  void SetMaxSlopePV(float input) { fMaxSlopePV = input; }
  /// Sets name of the iteration
  void SetName(const std::string& name) { fName = name; }
  ///
  void SetPickGather(float input) { fPickGather = input; }
  ///
  void SetPickNeighbour(float input) { fPickNeighbour = input; }
  /// Sets flag: primary tracks - true, secondary tracks - false
  void SetPrimary(bool flag) { fControlFlags[static_cast<int>(ControlFlag::kePrimary)] = flag; }
  /// Sets sigma of target positions in XY plane (in cm)
  void SetTargetPosSigmaXY(float sigmaX, float sigmaY);
  /// Sets track chi2 upper cut
  void SetTrackChi2Cut(float input) { fTrackChi2Cut = input; }
  /// Sets triplet chi2 upper cut
  void SetTripletChi2Cut(float input) { fTripletChi2Cut = input; }

  /// Swap method
  void Swap(L1CAIteration& other) noexcept;
  /// String representation of the class contents
  std::string ToString(int indentLevel = 0) const;

private:
  //-------------------------------------------------------------------------------------------------------------------
  std::string fName {""};           ///< Iteration name
  ControlFlags_t fControlFlags {};  ///< bitset flags to control iteration behaviour

  //-------------------------------------------------------------------------------------------------------------------
  // Track finder dependent cuts
  //-------------------------------------------------------------------------------------------------------------------

  // TODO: Iteratively change the literals to floats (S.Zharko)
  float fTrackChi2Cut {10.f};                   ///> track chi2 upper cut
  float fTripletChi2Cut {21.1075f};             ///> triplet chi2 upper cut
  float fDoubletChi2Cut {11.3449 * 2.f / 3.f};  ///> doublet chi2 upper cut
  float fPickGather {3.0};                      ///> size of region to attach new hits to the created track
  float fPickNeighbour {5.0};                   ///> min value of dp/dp_error, for which to tiplets are neighbours
  float fMaxInvMom {1.0 / 0.5};                 ///> max considered q/p for tracks
  float fMaxSlopePV {1.1};                      ///> max slope (tx\ty) in prim vertex
  float fMaxSlope {2.748};                      ///> max slope (tx\ty) in 3d hit position of a triplet
  float fMaxDZ {0.f};                           ///> Correction for accounting overlaping and iff z
  float fTargetPosSigmaX {0};                   ///> Constraint on target position in X direction [cm]
  float fTargetPosSigmaY {0};                   ///> Constraint on target position in Y direction [cm]
 
};

#endif  // L1CAIteration_h
