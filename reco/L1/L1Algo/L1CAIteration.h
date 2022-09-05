/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   L1CAIteration.h
 * @brief  Declaration of the L1CAIteration class 
 * @since  05.02.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef L1CAIteration_h
#define L1CAIteration_h 1

#include <boost/serialization/access.hpp>

#include <bitset>
#include <string>

// TODO: discuss the border conditions for the parameters. Implement them (S.Zharko)
//       Implementation: use tuples for boundary conditions of the parameters
/// Class L1CAIteration describes L1 Track finder iteration.
/// Each iteration utilizes special physics cuts and run condition to find tracks of a particular
/// class (e.g., fast primary tracks or secondary electron tracks). Hits associated with tracks
/// reconstructed during current iteration are removed from the further iterations.
///
class L1CAIteration {
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

  /// Copy assignment operator
  L1CAIteration& operator=(const L1CAIteration& other) noexcept;

  /// Move assignment operator
  L1CAIteration& operator=(L1CAIteration&& other) noexcept;

  /// Gets doublet chi2 upper cut
  float GetDoubletChi2Cut() const { return fDoubletChi2Cut; }

  /// Gets station index of the first station used in tracking
  int GetFirstStationIndex() const { return fFirstStationIndex; }

  /// Gets correction for accounting overlaping and iff z
  float GetMaxDZ() const { return fMaxDZ; }

  /// Gets max considered q/p for tracks
  float GetMaxInvMom() const { return fMaxInvMom; }

  /// Gets max slope (tx\ty) in 3D hit position of a triplet
  float GetMaxSlope() const { return fMaxSlope; }

  /// Gets max slope (tx\ty) in primary vertex
  float GetMaxSlopePV() const { return fMaxSlopePV; }

  /// Gets min level of the triplet start
  int GetMinLevelTripletStart() const { return fMinLevelTripletStart; }

  /// Gets the name of the iteration
  const std::string& GetName() const { return fName; }

  /// Gets size of region [TODO: units??] to attach new hits to the created track
  float GetPickGather() const { return fPickGather; }

  /// Gets min value of dp/dp_error, for which two tiplets are neighbours
  float GetPickNeighbour() const { return fPickNeighbour; }

  /// Gets sigma target position in X direction [cm]
  float GetTargetPosSigmaX() const { return fTargetPosSigmaX; }

  /// Gets sigma target position in Y direction [cm]
  float GetTargetPosSigmaY() const { return fTargetPosSigmaY; }

  /// Gets track chi2 upper cut
  float GetTrackChi2Cut() const { return fTrackChi2Cut; }

  /// Gets triplet chi2 upper cut
  float GetTripletChi2Cut() const { return fTripletChi2Cut; }

  /// flag check: electrons/positrons - true, heavy charged - false
  bool GetElectronFlag() const { return fIsElectron; }

  /// Checks flag: true - only primary tracks are searched, false - [all or only secondary?]
  bool GetPrimaryFlag() const { return fIsPrimary; }

  /// Prints iteration options
  void Print(int verbosityLevel = 0) const;

  /// Sets doublet chi2 upper cut
  void SetDoubletChi2Cut(float input) { fDoubletChi2Cut = input; }

  /// Sets flag: electron tracks - true, heavy ion tracks - false
  void SetElectronFlag(bool flag) { fIsElectron = flag; }

  /// Sets index of first station used in tracking
  void SetFirstStationIndex(int index) { fFirstStationIndex = index; }

  /// Sets correction for accounting overlaping and iff z
  void SetMaxDZ(float input) { fMaxDZ = input; }

  /// Sets max considered q/p for tracks
  /// TODO: Replace with minimum momentum setter (S.Zharko)
  void SetMaxInvMom(float input) { fMaxInvMom = input; }

  /// Sets max slope (tx\ty) in 3D hit position of a triplet
  void SetMaxSlope(float input) { fMaxSlope = input; }

  /// Sets max slope (tx\ty) in primary vertex
  void SetMaxSlopePV(float input) { fMaxSlopePV = input; }

  /// Sets min level of the triplet start
  void SetMinLevelTripletStart(int input) { fMinLevelTripletStart = input; }

  /// Sets name of the iteration
  void SetName(const std::string& name) { fName = name; }

  /// Sets size of region [TODO: units??] to attach new hits to the created track
  void SetPickGather(float input) { fPickGather = input; }

  /// Sets min value of dp/dp_error, for which two tiplets are neighbours
  void SetPickNeighbour(float input) { fPickNeighbour = input; }

  /// Sets flag: primary tracks - true, secondary tracks - false
  void SetPrimaryFlag(bool flag) { fIsPrimary = flag; }

  /// Sets sigma of target positions in XY plane
  /// \param  sigmaX  Sigma value in X direction [cm]
  /// \param  sigmaX  Sigma value in Y direction [cm]
  void SetTargetPosSigmaXY(float sigmaX, float sigmaY);

  /// Sets track chi2 upper cut
  void SetTrackChi2Cut(float input) { fTrackChi2Cut = input; }

  /// Sets triplet chi2 upper cut
  void SetTripletChi2Cut(float input) { fTripletChi2Cut = input; }

  /// Swap method
  void Swap(L1CAIteration& other) noexcept;

  /// String representation of the class contents
  /// \param indentLevel  Level of indentation for the text (in terms of \t symbols)
  std::string ToString(int indentLevel = 0) const;

private:
  /** Basic fields **/
  std::string fName {""};           ///< Iteration name

  /** Track finder dependent cuts **/
  // TODO: Iteratively change the literals to floats (S.Zharko)
  // NOTE: For each new cut one should not forget to create a setter and a getter, insert the value
  //       initialization in the copy constructor and the Swap operator as well as a string repre-
  //       sentation to the ToString method (S.Zharko)
  float fTrackChi2Cut       = 10.f;                 ///< Track chi2 upper cut
  float fTripletChi2Cut     = 21.1075f;             ///< Triplet chi2 upper cut
  float fDoubletChi2Cut     = 11.3449 * 2.f / 3.f;  ///< Doublet chi2 upper cut
  float fPickGather         = 3.0;                  ///< Size of region to attach new hits to the created track
  float fPickNeighbour      = 5.0;                  ///< Min value of dp/dp_error, for which two tiplets are neighbours
  float fMaxInvMom          = 1.0 / 0.5;            ///< Max considered q/p for tracks
  float fMaxSlopePV         = 1.1;                  ///< Max slope (tx\ty) in primary vertex
  float fMaxSlope           = 2.748;                ///< Max slope (tx\ty) in 3D hit position of a triplet
  float fMaxDZ              = 0.f;                  ///< Correction for accounting overlaping and iff z [cm]
  float fTargetPosSigmaX    = 0;                    ///< Constraint on target position in X direction [cm]
  float fTargetPosSigmaY    = 0;                    ///< Constraint on target position in Y direction [cm]
  int fMinLevelTripletStart = 0;                    ///< Min level for starting a triplet.
                                                    ///< Track length = fMinLevelTripletStart + 3
  int fFirstStationIndex = 0;                       ///< First station, used for tracking

  bool fIsPrimary  = false;  ///< Flag: true - only primary tracks are searched for
  bool fIsElectron = false;  ///< Flag: true - only electrons are searched for

  /// Serialization method, used to save L1Hit objects into binary or text file in a defined order
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fTrackChi2Cut;
    ar& fTripletChi2Cut;
    ar& fDoubletChi2Cut;
    ar& fPickGather;
    ar& fPickNeighbour;
    ar& fMaxInvMom;
    ar& fMaxSlopePV;
    ar& fMaxSlope;
    ar& fMaxDZ;
    ar& fTargetPosSigmaX;
    ar& fTargetPosSigmaY;
    ar& fMinLevelTripletStart;
    ar& fFirstStationIndex;
    ar& fIsPrimary;
    ar& fIsElectron;
  }


  // ^ TODO: invent more proper name
};

#endif  // L1CAIteration_h
