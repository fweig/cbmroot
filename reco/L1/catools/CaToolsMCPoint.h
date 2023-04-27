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

#include "CaToolsLinkKey.h"
#include "L1Constants.h"
#include "L1Undef.h"
#include "L1Vector.h"

enum class L1DetectorID;

namespace ca::tools
{
  /// @brief Class describes a unified MC-point, used in CA tracking QA analysis
  ///
  class MCPoint {
  public:
    /// @brief Default constructor
    MCPoint() = default;

    /// @brief Destructor
    ~MCPoint() = default;

    /// @brief Copy constructor
    MCPoint(const MCPoint&) = default;

    /// @brief Move constructor
    MCPoint(MCPoint&&) = default;

    /// @brief Copy assignment operator
    MCPoint& operator=(const MCPoint&) = default;

    /// @brief Move assignment operator
    MCPoint& operator=(MCPoint&&) = default;


    /// @brief Adds index of hits from the container of hits of event/TS
    /// @param  iH  A hit index in the external hits container of event/TS
    void AddHitID(int iH) { fvHitIndexes.push_back_no_warning(iH); }


    // *********************
    // **     Getters     **
    // *********************

    /// @brief Gets charge of the particle [e]
    double GetCharge() const { return fCharge; }

    /// @brief Gets detector ID
    L1DetectorID GetDetectorId() const { return fDetectorId; }

    /// @brief Gets MC event ID
    int GetEventId() const { return fLinkKey.fEvent; }

    /// @brief Gets MC file ID
    int GetFileId() const { return fLinkKey.fFile; }

    /// @brief Gets index of this point in internal CA container
    int GetId() const { return fId; }

    /// @brief Gets inverse speed at reference z of station [ns/cm]
    int GetInvSpeed() const
    {
      return std::sqrt(1. + GetMass() * GetMass() / GetP() / GetP()) * L1Constants::phys::kSpeedOfLightInv;
    }

    /// @brief Gets inverse speed at entrance to station [ns/cm]
    int GetInvSpeedIn() const
    {
      return std::sqrt(1. + GetMass() * GetMass() / GetPIn() / GetPIn()) * L1Constants::phys::kSpeedOfLightInv;
    }

    /// @brief Gets inverse speed at exit of station [ns/cm]
    int GetInvSpeedOut() const
    {
      return std::sqrt(1. + GetMass() * GetMass() / GetPOut() / GetPOut()) * L1Constants::phys::kSpeedOfLightInv;
    }

    /// @brief Gets container of matched hit indexes
    const auto& GetHitIndexes() const { return fvHitIndexes; }

    /// @brief Gets link key
    LinkKey GetLinkKey() const { return fLinkKey; }

    /// @brief Gets mass of the particle [GeV/c2]
    double GetMass() const { return fMass; }

    /// @brief Gets mother ID of the track
    int GetMotherId() const { return fMotherId; }

    /// @brief Gets track momentum absolute value at reference z of station [GeV/c]
    double GetP() const { return std::sqrt(fMom[0] * fMom[0] + fMom[1] * fMom[1] + fMom[2] * fMom[2]); }

    /// @brief Gets PDG code of the particle
    int GetPdgCode() const { return fPdgCode; }

    /// @brief Gets track azimuthal angle at reference z of station [rad]
    double GetPhi() const { return std::atan2(-fMom[1], fMom[0]); }

    /// @brief Gets track azimuthal angle at entrance to station [rad]
    double GetPhiIn() const { return std::atan2(-fMomIn[1], fMomIn[0]); }

    /// @brief Gets track azimuthal angle at exit of station [rad]
    double GetPhiOut() const { return std::atan2(-fMomOut[1], fMomOut[0]); }

    /// @brief Gets track momentum absolute value at entrance to station [GeV/c]
    double GetPIn() const { return std::sqrt(fMomIn[0] * fMomIn[0] + fMomIn[1] * fMomIn[1] + fMomIn[2] * fMomIn[2]); }

    /// @brief Gets track momentum absolute value at exit of station [GeV/c]
    double GetPOut() const
    {
      return std::sqrt(fMomOut[0] * fMomOut[0] + fMomOut[1] * fMomOut[1] + fMomOut[2] * fMomOut[2]);
    }

    /// @brief Gets track momentum x component at reference z of station [GeV/c]
    double GetPx() const { return fMom[0]; }

    /// @brief Gets track momentum x component at entrance to station [GeV/c]
    double GetPxIn() const { return fMomIn[0]; }

    /// @brief Gets track momentum x component at exit of station [GeV/c]
    double GetPxOut() const { return fMomOut[0]; }

    /// @brief Gets track momentum y component at reference z of station [GeV/c]
    double GetPy() const { return fMom[1]; }

    /// @brief Gets track momentum y component at entrance to station [GeV/c]
    double GetPyIn() const { return fMomIn[1]; }

    /// @brief Gets track momentum y component at exit of station [GeV/c]
    double GetPyOut() const { return fMomOut[1]; }

    /// @brief Gets track momentum z component at reference z of station [GeV/c]
    double GetPz() const { return fMom[2]; }

    /// @brief Gets track momentum z component at entrance to station [GeV/c]
    double GetPzIn() const { return fMomIn[2]; }

    /// @brief Gets track momentum z component at exit of station [GeV/c]
    double GetPzOut() const { return fMomOut[2]; }

    /// @brief Gets track charge over momentum at reference z of station [ec/GeV]
    double GetQp() const { return fCharge / GetP(); }

    /// @brief Gets track momentum absolute value at entrance to station [ec/GeV]
    double GetQpIn() const { return fCharge / GetPIn(); }

    /// @brief Gets track momentum absolute value at exit of station [ec/GeV]
    double GetQpOut() const { return fCharge / GetPOut(); }

    /// @brief Gets global ID of the active tracking station
    int GetStationId() const { return fStationId; }

    /// @brief Gets polar angle at reference z of station [rad]
    double GetTheta() const { return std::acos(GetPz() / GetP()); }

    /// @brief Gets polar angle at entrance to station [rad]
    double GetThetaIn() const { return std::acos(GetPzIn() / GetPIn()); }

    /// @brief Gets polar angle at exit of station [rad]
    double GetThetaOut() const { return std::acos(GetPzOut() / GetPOut()); }

    /// @brief Gets time [ns]
    double GetTime() const { return fTime; }

    /// @brief Gets ID of track from the internal CA MC track container (within event/TS)
    int GetTrackId() const { return fTrackId; }

    /// @brief Gets slope along x-axis at reference z of station
    double GetTx() const { return fMom[0] / fMom[2]; }

    /// @brief Gets slope along x-axis at entrance to station
    double GetTxIn() const { return fMomIn[0] / fMomIn[2]; }

    /// @brief Gets slope along x-axis at exit of station
    double GetTxOut() const { return fMomOut[0] / fMomOut[2]; }

    /// @brief Gets slope along x-axis at reference z of station
    double GetTy() const { return fMom[1] / fMom[2]; }

    /// @brief Gets slope along x-axis at entrance to station
    double GetTyIn() const { return fMomIn[1] / fMomIn[2]; }

    /// @brief Gets slope along x-axis at exit of station
    double GetTyOut() const { return fMomOut[1] / fMomOut[2]; }

    /// @brief Gets x coordinate at reference z of station [cm]
    double GetX() const { return fPos[0]; }

    /// @brief Gets x coordinate at entrance to station [cm]
    double GetXIn() const { return fPosIn[0]; }

    /// @brief Gets x coordinate at exit of station [cm]
    double GetXOut() const { return fPosOut[0]; }

    /// @brief Gets y coordinate at reference z of station [cm]
    double GetY() const { return fPos[1]; }

    /// @brief Gets y coordinate at entrance to station [cm]
    double GetYIn() const { return fPosIn[1]; }

    /// @brief Gets y coordinate at exit of station [cm]
    double GetYOut() const { return fPosOut[1]; }

    /// @brief Gets z coordinate at reference z of station [cm]
    double GetZ() const { return fPos[2]; }

    /// @brief Gets z coordinate at entrance to station [cm]
    double GetZIn() const { return fPosIn[2]; }

    /// @brief Gets z coordinate at exit of station [cm]
    double GetZOut() const { return fPosOut[2]; }

    // *********************
    // **     Setters     **
    // *********************

    /// @brief Sets particle charge [e]
    void SetCharge(double charge) { fCharge = charge; }

    /// @brief Sets detector ID
    void SetDetectorId(L1DetectorID detId) { fDetectorId = detId; }

    /// @brief Sets index of MC event containing this point
    void SetEventId(int eventId) { fLinkKey.fEvent = eventId; }

    /// @brief Sets index of this point in external data structures
    void SetExternalId(int id) { fLinkKey.fIndex = id; }

    /// @brief Sets index of MC file containing this point
    void SetFileId(int fileId) { fLinkKey.fFile = fileId; }

    /// @brief Sets index of this point in the CA internal structure
    void SetId(int id) { fId = id; }

    /// @brief Sets particle mass [GeV/c2]
    void SetMass(double mass) { fMass = mass; }

    /// @brief Sets index of mother track in the internal CA data structures
    void SetMotherId(int motherId) { fMotherId = motherId; }

    /// @brief Sets PDG code
    void SetPdgCode(int pdg) { fPdgCode = pdg; }

    /// @brief Sets track momentum x component at reference z of station [GeV/c]
    void SetPx(double px) { fMom[0] = px; }

    /// @brief Sets track momentum x component at entrance to station [GeV/c]
    void SetPxIn(double px) { fMomIn[0] = px; }

    /// @brief Sets track momentum x component at exit of station [GeV/c]
    void SetPxOut(double px) { fMomOut[0] = px; }

    /// @brief Sets track momentum y component at reference z of station [GeV/c]
    void SetPy(double py) { fMom[1] = py; }

    /// @brief Sets track momentum y component at entrance to station [GeV/c]
    void SetPyIn(double py) { fMomIn[1] = py; }

    /// @brief Sets track momentum y component at exit of station [GeV/c]
    void SetPyOut(double py) { fMomOut[1] = py; }

    /// @brief Sets track momentum z component at reference z of station [GeV/c]
    void SetPz(double pz) { fMom[2] = pz; }

    /// @brief Sets track momentum z component at entrance to station [GeV/c]
    void SetPzIn(double pz) { fMomIn[2] = pz; }

    /// @brief Sets track momentum z component at exit of station [GeV/c]
    void SetPzOut(double pz) { fMomOut[2] = pz; }


    /// @brief Sets global index of active station
    void SetStationId(int stationId) { fStationId = stationId; }

    /// @brief Sets time [ns]
    void SetTime(double time) { fTime = time; }

    /// @brief Sets track ID in the CA internal track container (within event/TS)
    void SetTrackId(int trackId) { fTrackId = trackId; }

    /// @brief Sets x coordinate at reference z of station [cm]
    void SetX(double x) { fPos[0] = x; }

    /// @brief Sets x coordinate at entrance to station [cm]
    void SetXIn(double x) { fPosIn[0] = x; }

    /// @brief Sets x coordinate at exit of station [cm]
    void SetXOut(double x) { fPosOut[0] = x; }

    /// @brief Sets y coordinate at reference z of station [cm]
    void SetY(double y) { fPos[1] = y; }

    /// @brief Sets y coordinate at entrance to station [cm]
    void SetYIn(double y) { fPosIn[1] = y; }

    /// @brief Sets x coordinate at exit of station [cm]
    void SetYOut(double y) { fPosOut[1] = y; }

    /// @brief Sets z coordinate at reference z of station [cm]
    void SetZ(double z) { fPos[2] = z; }

    /// @brief Sets z coordinate at entrance to station [cm]
    void SetZIn(double z) { fPosIn[2] = z; }

    /// @brief Sets z coordinate at exit of station [cm]
    void SetZOut(double z) { fPosOut[2] = z; }

    /// @brief Prints content for a given verbosity level
    /// @param  verbose  Verbosity level:
    ///                  -#0: Prints nothing
    ///                  -#1: Prints track ID, station ID, time and position
    ///                  -#2: Also prints zIn, zOut, absolute momentum, as well as point, event and file IDs
    /// @param  printHeader  If true, parameter names will be printed instead of the parameters themselves
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

    double fMass   = undef::kD;  ///< Particle mass [GeV/c2]
    double fCharge = undef::kD;  ///< Particle charge [e]
    double fTime   = undef::kD;  ///< Point time [ns]

    LinkKey fLinkKey = {undef::kI32, undef::kI32, undef::kI32};  ///< Link key of point

    int fPdgCode   = undef::kI32;  ///< Particle PDG code
    int fId        = undef::kI32;  ///< Index of MC point in the external MC point container
    int fTrackId   = undef::kI32;  ///< Index of associated MC track in CA internal track container within TS/event
    int fMotherId  = undef::kI32;  ///< Index of mother track in CA internal data structures (within event/TS)
    int fStationId = undef::kI32;  ///< Global index of active tracking station

    L1DetectorID fDetectorId;  ///< Detector ID of MC point

    L1Vector<int> fvHitIndexes {"ca::tools::MCPoint::fvHitIndexes"};  ///< Indexes of hits, assigned to this point
  };
}  // namespace ca::tools


#endif  // CaToolsMCPoint_h
