/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CaToolsMCPoint.h
/// \brief  Internal class describing a MC point for CA tracking QA and performance (header)
/// \date   17.11.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsMCPoint_h
#define CaToolsMCPoint_h 1

#include <string>

#include "L1Undef.h"
#include "L1Vector.h"

enum class L1DetectorID;

/// Class describes a Monte-Carlo point used in CA tracking QA analysis
namespace ca::tools
{
  class MCTrack;

  class MCPoint {
  public:
    /// Default constructor
    MCPoint() = default;

    /// Destructor
    ~MCPoint() = default;

    /// Copy constructor
    MCPoint(const MCPoint&) = default;

    /// Move constructor
    MCPoint(MCPoint&&) = default;

    /// Copy assignment operator
    MCPoint& operator=(const MCPoint&) = default;

    /// Move assignment operator
    MCPoint& operator=(MCPoint&&) = default;


    /// Adds hit ID
    /// \param  hitID  A hit index in the external hits array
    void AddHitID(int hitID) { fHitIDs.push_back_no_warning(hitID); }


    // *********************
    // **     Getters     **
    // *********************

    /// Gets charge of the particle [e]
    double GetCharge() const { return fCharge; }

    /// Gets detector ID
    L1DetectorID GetDetectorID() const { return fDetectorID; }

    /// Gets event ID
    int GetEventID() const { return fEventID; }

    /// Gets file ID
    int GetFileID() const { return fFileID; }

    /// Gets mass of the particle [GeV/c2]
    double GetMass() const { return fMass; }

    /// Gets mother ID of the track
    int GetMotherID() const { return fMotherID; }

    /// Gets track momentum absolute value at reference z of station [GeV/c]
    double GetP() const { return std::sqrt(fMom[0] * fMom[0] + fMom[1] * fMom[1] + fMom[2] * fMom[2]); }

    /// Gets PDG code of the particle
    int GetPdgCode() const { return fPdgCode; }

    /// Gets track momentum absolute value at entrance to station [GeV/c]
    double GetPIn() const { return std::sqrt(fMomIn[0] * fMomIn[0] + fMomIn[1] * fMomIn[1] + fMomIn[2] * fMomIn[2]); }

    /// Gets point ID
    int GetPointID() const { return fPointID; }

    /// Gets track momentum absolute value at exit of station [GeV/c]
    double GetPOut() const
    {
      return std::sqrt(fMomOut[0] * fMomOut[0] + fMomOut[1] * fMomOut[1] + fMomOut[2] * fMomOut[2]);
    }

    /// Gets track momentum x component at reference z of station [GeV/c]
    double GetPx() const { return fMom[0]; }

    /// Gets track momentum x component at entrance to station [GeV/c]
    double GetPxIn() const { return fMomIn[0]; }

    /// Gets track momentum x component at exit of station [GeV/c]
    double GetPxOut() const { return fMomOut[0]; }

    /// Gets track momentum y component at reference z of station [GeV/c]
    double GetPy() const { return fMom[1]; }

    /// Gets track momentum y component at entrance to station [GeV/c]
    double GetPyIn() const { return fMomIn[1]; }

    /// Gets track momentum y component at exit of station [GeV/c]
    double GetPyOut() const { return fMomOut[1]; }

    /// Gets track momentum z component at reference z of station [GeV/c]
    double GetPz() const { return fMom[2]; }

    /// Gets track momentum z component at entrance to station [GeV/c]
    double GetPzIn() const { return fMomIn[2]; }

    /// Gets track momentum z component at exit of station [GeV/c]
    double GetPzOut() const { return fMomOut[2]; }

    /// Gets global ID of the active tracking station
    int GetStationID() const { return fStationID; }

    /// Gets time [ns]
    double GetTime() const { return fTime; }

    /// Gets ID of track
    int GetTrackID() const { return fTrackID; }

    /// Gets x coordinate at reference z of station [cm]
    double GetX() const { return fPos[0]; }

    /// Gets x coordinate at entrance to station [cm]
    double GetXIn() const { return fPosIn[0]; }

    /// Gets x coordinate at exit of station [cm]
    double GetXOut() const { return fPosOut[0]; }

    /// Gets y coordinate at reference z of station [cm]
    double GetY() const { return fPos[1]; }

    /// Gets y coordinate at entrance to station [cm]
    double GetYIn() const { return fPosIn[1]; }

    /// Gets y coordinate at exit of station [cm]
    double GetYOut() const { return fPosOut[1]; }

    /// Gets z coordinate at reference z of station [cm]
    double GetZ() const { return fPos[2]; }

    /// Gets z coordinate at entrance to station [cm]
    double GetZIn() const { return fPosIn[2]; }

    /// Gets z coordinate at exit of station [cm]
    double GetZOut() const { return fPosOut[2]; }


    /// Gets Hit ID
    int MapHitID(int iHitID) { return fHitIDs[iHitID]; }


    // *********************
    // **     Setters     **
    // *********************

    /// Sets particle charge [e]
    void SetCharge(double charge) { fCharge = charge; }

    /// Sets detector ID
    void SetDetectorID(L1DetectorID detID) { fDetectorID = detID; }

    /// Sets particle mass [GeV/c2]
    void SetMass(double mass) { fMass = mass; }

    /// Sets mother ID
    void SetMotherID(int motherID) { fMotherID = motherID; }

    /// Sets link address
    /// \param pointID  Index of
    /// \param eventID
    /// \param fileID
    void SetLinkAddress(int pointID, int eventID, int fileID);

    /// Sets PDG code
    void SetPdgCode(int pdg) { fPdgCode = pdg; }

    /// Sets track momentum x component at reference z of station [GeV/c]
    void SetPx(double px) { fMom[0] = px; }

    /// Sets track momentum x component at entrance to station [GeV/c]
    void SetPxIn(double px) { fMomIn[0] = px; }

    /// Sets track momentum x component at exit of station [GeV/c]
    void SetPxOut(double px) { fMomOut[0] = px; }

    /// Sets track momentum y component at reference z of station [GeV/c]
    void SetPy(double py) { fMom[1] = py; }

    /// Sets track momentum y component at entrance to station [GeV/c]
    void SetPyIn(double py) { fMomIn[1] = py; }

    /// Sets track momentum y component at exit of station [GeV/c]
    void SetPyOut(double py) { fMomOut[1] = py; }

    /// Sets track momentum z component at reference z of station [GeV/c]
    void SetPz(double pz) { fMom[2] = pz; }

    /// Sets track momentum z component at entrance to station [GeV/c]
    void SetPzIn(double pz) { fMomIn[2] = pz; }

    /// Sets track momentum z component at exit of station [GeV/c]
    void SetPzOut(double pz) { fMomOut[2] = pz; }

    /// Sets global index of active station
    void SetStationID(int stationID) { fStationID = stationID; }

    /// Sets time [ns]
    void SetTime(double time) { fTime = time; }

    /// Sets track ID
    void SetTrackID(int trackID) { fTrackID = trackID; }

    /// Sets x coordinate at reference z of station [cm]
    void SetX(double x) { fPos[0] = x; }

    /// Sets x coordinate at entrance to station [cm]
    void SetXIn(double x) { fPosIn[0] = x; }

    /// Sets x coordinate at exit of station [cm]
    void SetXOut(double x) { fPosOut[0] = x; }

    /// Sets y coordinate at reference z of station [cm]
    void SetY(double y) { fPos[1] = y; }

    /// Sets y coordinate at entrance to station [cm]
    void SetYIn(double y) { fPosIn[1] = y; }

    /// Sets x coordinate at exit of station [cm]
    void SetYOut(double y) { fPosOut[1] = y; }

    /// Sets z coordinate at reference z of station [cm]
    void SetZ(double z) { fPos[2] = z; }

    /// Sets z coordinate at entrance to station [cm]
    void SetZIn(double z) { fPosIn[2] = z; }

    /// Sets z coordinate at exit of station [cm]
    void SetZOut(double z) { fPosOut[2] = z; }

    /// Prints content for a given verbosity level
    /// \param  verbose  Verbosity level:
    ///                  -#0: Prints nothing
    ///                  -#1: Prints track ID, station ID, time and position
    ///                  -#2: Also prints zIn, zOut, absolute momentum, as well as point, event and file IDs
    /// \param  printHeader  If true, parameter names will be printed instead of the parameters themselves
    std::string ToString(int verbose, bool printHeader = false) const;

    // ****************************
    // **     Data variables     **
    // ****************************

  private:
    std::array<double, 3> fPos    = {undef::kD, undef::kD, undef::kD};  ///< Position at reference z of station [cm]
    std::array<double, 3> fPosIn  = {undef::kD, undef::kD, undef::kD};  ///< Position at entrance to station [cm]
    std::array<double, 3> fPosOut = {undef::kD, undef::kD, undef::kD};  ///< Position at exit of station [cm]
    std::array<double, 3> fMom    = {undef::kD, undef::kD, undef::kD};  ///< Momentum at reference z of station [cm]
    std::array<double, 3> fMomIn  = {undef::kD, undef::kD, undef::kD};  ///< Momentum at entrance to station [cm]
    std::array<double, 3> fMomOut = {undef::kD, undef::kD, undef::kD};  ///< Momentum at exit of station [cm]

    int fPdgCode   = undef::kI32;  ///< Particle PDG code
    double fMass   = undef::kD;    ///< Particle mass [GeV/c2]
    double fCharge = undef::kD;    ///< Particle charge [e]
    double fTime   = undef::kD;    ///< Point time [ns]

    int fTrackID   = undef::kI32;  ///< Track ID
    int fMotherID  = undef::kI32;  ///< Mother particle ID (NOTE: probably is redundant)
    int fStationID = undef::kI32;  ///< Global index of active tracking station

    L1DetectorID fDetectorID;  ///< Detector ID of MC point

    int fPointID = undef::kI32;  ///< TODO
    int fEventID = undef::kI32;
    int fFileID  = undef::kI32;

    L1Vector<int> fHitIDs {"ca::tools::MCPoint::fHitIDs"};
  };
}  // namespace ca::tools


#endif  // CaToolsMCPoint_h
