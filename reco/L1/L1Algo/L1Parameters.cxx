/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// @file L1Parameters.cxx
/// @brief Parameter container for the L1Algo library
/// @since 10.02.2022
/// @author S.Zharko <s.zharko@gsi.de>

#include "L1Parameters.h"

#include <FairLogger.h>

#include <iomanip>

//----------------------------------------------------------------------------------------------------------------------
//
L1Parameters::L1Parameters()
{
  fActiveStationGlobalIDs.fill(-1);  // by default, all stations are inactive, thus all the IDs must be -1
}

//----------------------------------------------------------------------------------------------------------------------
//
L1Parameters::~L1Parameters() noexcept {}

//----------------------------------------------------------------------------------------------------------------------
//
L1Parameters::L1Parameters(const L1Parameters& other) noexcept
  : fMaxDoubletsPerSinglet(other.fMaxDoubletsPerSinglet)
  , fMaxTripletPerDoublets(other.fMaxTripletPerDoublets)
  , fCAIterations(other.fCAIterations)
  , fTargetPos(other.fTargetPos)
  , fVertexFieldValue(other.fVertexFieldValue)
  , fVertexFieldRegion(other.fVertexFieldRegion)
  , fStations(other.fStations)
  , fThickMap(other.fThickMap)
  , fNstationsGeometryTotal(other.fNstationsGeometryTotal)
  , fNstationsActiveTotal(other.fNstationsActiveTotal)
  , fNstationsGeometry(other.fNstationsGeometry)
  , fNstationsActive(other.fNstationsActive)
  , fActiveStationGlobalIDs(other.fActiveStationGlobalIDs)
  , fTrackingLevel(other.fTrackingLevel)
  , fGhostSuppression(other.fGhostSuppression)
  , fMomentumCutOff(other.fMomentumCutOff)
  , fDevIsIgnoreHitSearchAreas(other.fDevIsIgnoreHitSearchAreas)
  , fDevIsUseOfOriginalField(other.fDevIsUseOfOriginalField)
  , fDevIsMatchDoubletsViaMc(other.fDevIsMatchDoubletsViaMc)
  , fDevIsMatchTripletsViaMc(other.fDevIsMatchTripletsViaMc)
{
}

//----------------------------------------------------------------------------------------------------------------------
//
L1Parameters& L1Parameters::operator=(const L1Parameters& other) noexcept
{
  if (this != &other) { L1Parameters(other).Swap(*this); }
  return *this;
}

//----------------------------------------------------------------------------------------------------------------------
//
L1Parameters::L1Parameters(L1Parameters&& other) noexcept { this->Swap(other); }

//----------------------------------------------------------------------------------------------------------------------
//
L1Parameters& L1Parameters::operator=(L1Parameters&& other) noexcept
{
  if (this != &other) {
    L1Parameters tmp(std::move(other));
    this->Swap(tmp);
  }
  return *this;
}

//----------------------------------------------------------------------------------------------------------------------
//
void L1Parameters::Swap(L1Parameters& other) noexcept
{
  std::swap(fMaxDoubletsPerSinglet, other.fMaxDoubletsPerSinglet);
  std::swap(fMaxTripletPerDoublets, other.fMaxTripletPerDoublets);
  std::swap(fCAIterations, other.fCAIterations);
  std::swap(fTargetPos, other.fTargetPos);
  std::swap(fVertexFieldValue, other.fVertexFieldValue);
  std::swap(fVertexFieldRegion, other.fVertexFieldRegion);
  std::swap(fStations, other.fStations);
  std::swap(fThickMap, other.fThickMap);
  std::swap(fNstationsGeometryTotal, other.fNstationsGeometryTotal);
  std::swap(fNstationsActiveTotal, other.fNstationsActiveTotal);
  std::swap(fNstationsGeometry, other.fNstationsGeometry);
  std::swap(fNstationsActive, other.fNstationsActive);
  std::swap(fActiveStationGlobalIDs, other.fActiveStationGlobalIDs);
  std::swap(fTrackingLevel, other.fTrackingLevel);
  std::swap(fGhostSuppression, other.fGhostSuppression);
  std::swap(fMomentumCutOff, other.fMomentumCutOff);
  std::swap(fDevIsIgnoreHitSearchAreas, other.fDevIsIgnoreHitSearchAreas);
  std::swap(fDevIsUseOfOriginalField, other.fDevIsUseOfOriginalField);
  std::swap(fDevIsMatchDoubletsViaMc, other.fDevIsMatchDoubletsViaMc);
  std::swap(fDevIsMatchTripletsViaMc, other.fDevIsMatchTripletsViaMc);
}

//----------------------------------------------------------------------------------------------------------------------
//
void L1Parameters::CheckConsistency() const
{
  LOG(info) << "Consistency test for L1 parameters object... ";
  /*
   * Arrays containing numbers of stations
   *
   * In the fNstationsActive and fNstationsGeometry array first L1Constants::size::kMaxNdetectors elements are the numbers of stations
   * in particular detector subsystem. The last element in both arrays corresponds to the total number of stations (geometry or 
   * active). This fact applies restriction on the arrays: the last element must be equal to the sum of all previous elements.
   *
   */

  if (std::accumulate(fNstationsGeometry.cbegin(), fNstationsGeometry.cend(), 0) != fNstationsGeometryTotal) {
    throw std::logic_error("L1Parameters: invalid object condition: total number of stations provided by geometry "
                           "differs from the sum of the station numbers for individual detector subsystems");
  };

  if (std::accumulate(fNstationsActive.cbegin(), fNstationsActive.cend(), 0) != fNstationsActiveTotal) {
    throw std::logic_error("L1Parameters: invalid object condition: total number of stations active in tracking "
                           "differs from the sum of the station numbers for individual detector subsystems");
  };

  /*
   * Array of active station IDs
   *
   * In the array of active station IDs, 
   *   (i)   sum of elements, which are not equal -1, must be equal the number of stations,
   *   (ii)  subsequence of all the elements, which are not equal -1, must be a gapless subset of integer numbers starting with 0
   */

  auto filterInactiveStationIDs = [](int x) { return x != -1; };
  int nStationsCheck =
    std::count_if(fActiveStationGlobalIDs.cbegin(), fActiveStationGlobalIDs.cend(), filterInactiveStationIDs);
  if (nStationsCheck != fNstationsActiveTotal) {
    std::stringstream msg;
    msg << "L1Parameters: invalid object condition: array of active station IDs is not consistent "
        << "with the total number of stations (" << nStationsCheck << " vs. " << fNstationsActiveTotal << ' '
        << "expected)";
    throw std::logic_error(msg.str());
  }

  // Check, if the subsequence of all the elements, which are not equal -1, must be a gapless subset of integer numbers
  // starting from 0. If it is, the testValue in the end must be equal the number of non -1 elements

  std::vector<int> idsCheck(
    nStationsCheck);  // temporary vector containing a sequence of active station IDs without -1 elements
  std::copy_if(fActiveStationGlobalIDs.cbegin(), fActiveStationGlobalIDs.cend(), idsCheck.begin(),
               filterInactiveStationIDs);
  bool isStationIDsOk = true;
  for (int id = 0; id < nStationsCheck; ++id) {
    isStationIDsOk = isStationIDsOk && idsCheck[id] == id;
  }
  if (!isStationIDsOk) {
    std::stringstream msg;
    msg << "L1Parameters: invalid object condition: array of active station IDs is not a gapless subset "
        << "of integer numbers starting from 0:\n\t";
    for (auto id : fActiveStationGlobalIDs) {
      msg << std::setw(3) << std::setfill(' ') << id << ' ';
    }
    throw std::logic_error(msg.str());
  }

  /*
   * Check magnetic field flags of the stations
   *
   * In a current version of tracking there are three configurations possible to be proceeded:
   *  A. All the stations are inside magnetic field
   *  B. There is no magnetic field in a setup
   *  C. All the first stations are inside magnetic field, all the last stations are outside the field
   * In all the cases the fieldStatus flags should be sorted containing all non-zero elements in the beginning
   * (representing stations placed into magnetic field) and all zero elements in the end of z-axis.
   */
  bool ifFieldStatusFlagsOk = std::is_sorted(
    fStations.cbegin(), fStations.cbegin() + fNstationsActiveTotal,
    [&](const L1Station& lhs, const L1Station& rhs) { return bool(lhs.fieldStatus) > bool(rhs.fieldStatus); });

  if (!ifFieldStatusFlagsOk) {
    std::stringstream msg;
    msg << "L1Parameters: invalid object condition: L1 tracking is impossible for a given field configuration:\n";
    for (int iSt = 0; iSt < fNstationsActiveTotal; ++iSt) {
      msg << "- station ID:  " << iSt << ",  field status: " << fStations[iSt].fieldStatus << '\n';
    }
    throw std::logic_error(msg.str());
  }

  /*
   * Check target position SIMD vector
   */

  L1Utils::CheckSimdVectorEquality(fTargetPos[0], "L1Parameters: target position x");
  L1Utils::CheckSimdVectorEquality(fTargetPos[1], "L1Parameters: target position y");
  L1Utils::CheckSimdVectorEquality(fTargetPos[2], "L1Parameters: target position z");

  /*
   * Check vertex field region and value objects at primary vertex
   */

  fVertexFieldValue.CheckConsistency();
  fVertexFieldRegion.CheckConsistency();


  /*
   * Check if each station object is consistent itself, and if all of them are placed after the target
   * NOTE: If a station was not set up, it is accounted inconsistent (uninitialized). In the array of stations there are uninitialized
   *       stations possible (with id > NstationsActiveTotal), thus one should NOT run the loop above over all the stations in array
   *       but only until *(fNstationsActive.cend() - 1) (== NstationsActiveTotal).
   * TODO: Probably, we should introduce methods, which check the consistency of fully initialized objects such as L1Station,
   *       L1MaterialInfo, etc. (S.Zharko)
   */

  for (int iSt = 0; iSt < fNstationsActiveTotal; ++iSt) {
    fStations[iSt].CheckConsistency();
    if (fStations[iSt].z[0] < fTargetPos[2][0]) {
      std::stringstream msg;
      msg << "L1Parameters: station with global ID = " << iSt << " is placed before target "
          << "(z_st = " << fStations[iSt].z[0] << " [cm] < z_targ = " << fTargetPos[2][0] << " [cm])";
      throw std::logic_error(msg.str());
    }
  }

  /*
   * Check thickness maps
   * NOTE: If a L1Material map was not set up, it is accounted inconsistent (uninitialized). In the array of thickness maps for each 
   *       there are uninitialized elements possible (with id > NstationsActiveTotal), thus one should NOT run the loop above over 
   *       all the stations in array but only until *(fNstationsActive.cend() - 1) (== NstationsActiveTotal).
   */

  for (int iSt = 0; iSt < fNstationsActiveTotal; ++iSt) {
    fThickMap[iSt].CheckConsistency();
  }


  /*
   *  Check iterations sequence
   *  1. Number of iterations should be larger then zero
   *  2. Each iteration should contain values within predefined limits
   *  3. Number of iterations with TrackFromTriplets flag turned on no more then 1
   *  4. If the TrackFromTriplets iteration exists, it should be the last one in the sequence
   */
  {
    int nIterations = fCAIterations.size();
    if (!nIterations) {
      std::stringstream msg;
      msg << "L1Parameters: 0 track finder iterations were found. Please, define at least one iteration";
      throw std::logic_error(msg.str());
    }

    std::string names = "";
    for (const auto iter : fCAIterations) {
      if (!iter.Check()) { names += iter.GetName() + " "; }
    }
    if (names.size()) {
      std::stringstream msg;
      msg << "L1Parameters: some parameters are out of range for the following iterations: " << names;
      throw std::logic_error(msg.str());
    }

    nIterations = std::count_if(fCAIterations.begin(), fCAIterations.end(),
                                [=](const L1CAIteration& it) { return it.GetTrackFromTripletsFlag(); });
    if (nIterations > 1) {
      std::stringstream msg;
      msg << "L1Parameters: found " << nIterations << " iterations with GetTrackFromTripletsFlag() == true:\n";
      for (const auto& iter : fCAIterations) {
        if (iter.GetTrackFromTripletsFlag()) { msg << '\t' << "- " << iter.GetName() << '\n'; }
      }
      msg << "Only the one iteration can have GetTrackFromTripletsFlag() == true, and this iteration should be ";
      msg << "the last one";
      throw std::logic_error(msg.str());
    }

    if (nIterations == 1 && !(fCAIterations.end() - 1)->GetTrackFromTripletsFlag()) {
      std::stringstream msg;
      msg << "L1Parameters: iteration with GetTrackFromTripletsFlag() == true is not the last in a sequence. ";
      msg << "The GetTrackFromTripletsFlag() value in the iterations sequence: \n";
      for (const auto& iter : fCAIterations) {
        msg << '\t' << "- " << std::setw(15) << std::setfill(' ') << iter.GetName() << ' ';
        msg << std::setw(6) << std::setfill(' ') << iter.GetTrackFromTripletsFlag() << '\n';
      }
      throw std::logic_error(msg.str());
    }
  }

  LOG(info) << "Consistency test for L1 parameters object... \033[1;32mpassed\033[0m";
}

//----------------------------------------------------------------------------------------------------------------------
//
void L1Parameters::Print(int /*verbosityLevel*/) const
{
  LOG(info) << ToString();
}

//----------------------------------------------------------------------------------------------------------------------
//
std::string L1Parameters::ToString(int verbosity, int indentLevel) const
{
  // FIXME: SZh: Fill it with other parameters
  std::stringstream aStream {};
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << '\n';
  aStream << indent << "--------------------------------------------------------------------------------\n";
  aStream << indent << "                              L1 parameters list\n";
  aStream << indent << "--------------------------------------------------------------------------------\n";
  aStream << indent << "COMPILE TIME CONSTANTS:\n";
  aStream << indent << indentChar << "Bits to code one station:           " << L1Constants::size::kStationBits << '\n';
  aStream << indent << indentChar << "Bits to code one thread:            " << L1Constants::size::kThreadBits << '\n';
  aStream << indent << indentChar << "Bits to code one triplet:           " << L1Constants::size::kTripletBits << '\n';
  aStream << indent << indentChar << "Max number of stations:             " << L1Constants::size::kMaxNstations << '\n';
  aStream << indent << indentChar << "Max number of threads:              " << L1Constants::size::kMaxNthreads << '\n';
  aStream << indent << indentChar << "Max number of triplets:             " << L1Constants::size::kMaxNtriplets << '\n';
  aStream << indent << "RUNTIME CONSTANTS:\n";
  aStream << indent << indentChar << "Max number of doublets per singlet: " << fMaxDoubletsPerSinglet << '\n';
  aStream << indent << indentChar << "Max number of triplets per doublet: " << fMaxTripletPerDoublets << '\n';
  aStream << indent << "CA TRACK FINDER ITERATIONS:\n";
  for (int idx = 0; idx < static_cast<int>(fCAIterations.size()); ++idx) {
    aStream << idx << ") " << fCAIterations[idx].ToString(indentLevel + 1) << '\n';
  }
  aStream << indent << "GEOMETRY:\n";

  aStream << indent << indentChar << "TARGET:\n";
  aStream << indent << indentChar << indentChar << "Position:\n";
  for (int dim = 0; dim < 3 /*nDimensions*/; ++dim) {
    aStream << indent << indentChar << indentChar << indentChar << char(120 + dim) << " = " << fTargetPos[dim][0]
            << " cm\n";
  }
  aStream << indent << indentChar << "NUMBER OF STATIONS:\n";
  aStream << indent << indentChar << indentChar << "Number of stations (Geometry): ";
  for (int idx = 0; idx < int(fNstationsGeometry.size()); ++idx) {
    aStream << std::setw(2) << std::setfill(' ') << fNstationsGeometry[idx] << ' ';
  }
  aStream << " | total = " << std::setw(2) << std::setfill(' ') << fNstationsGeometryTotal;
  aStream << '\n';
  aStream << indent << indentChar << indentChar << "Number of stations (Active):   ";
  for (int idx = 0; idx < int(fNstationsActive.size()); ++idx) {
    aStream << std::setw(2) << std::setfill(' ') << fNstationsActive[idx] << ' ';
  }
  aStream << " | total = " << std::setw(2) << std::setfill(' ') << fNstationsActiveTotal;
  aStream << '\n';
  aStream << indent << indentChar << indentChar << "Active station indexes: ";
  for (int idx = 0; idx < *(fNstationsActive.end() - 1); ++idx) {
    aStream << std::setw(3) << std::setfill(' ') << fActiveStationGlobalIDs[idx] << ' ';
  }
  aStream << '\n';

  aStream << indent << indentChar << "STATIONS:\n ";
  for (int idx = 0; idx < *(fNstationsActive.end() - 1); ++idx) {
    aStream << indent << indentChar << indentChar << fStations[idx].ToString(verbosity) << '\n';
  }

  aStream << indent << "FLAGS:\n";
  aStream << indent << "--------------------------------------------------------------------------------\n";
  return aStream.str();
}
