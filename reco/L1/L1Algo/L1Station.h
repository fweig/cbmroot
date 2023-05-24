/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Igor Kulakov, Sergei Zharko */

#ifndef L1Station_h
#define L1Station_h 1

#include <string>
#include <type_traits>

#include "L1Field.h"
#include "L1Material.h"
#include "L1SimdSerializer.h"
#include "L1UMeasurementInfo.h"
#include "L1Undef.h"
#include "L1Utils.h"
#include "L1XYMeasurementInfo.h"

/// Structure L1Station
/// Contains a set of geometry parameters for a particular station
///
class L1Station {
public:
  // TODO: SZh 12.05.2022: Rewrite type into L1DetectorID, change detector indexing scheme
  // TODO: SZh 12.05.2022: Provide getters to stations

  int type        = undef::kI32;
  int timeInfo    = undef::kI32;  ///< flag: if time information can be used
  int fieldStatus = undef::kI32;  ///< flag: 1 - station is INSIDE the field, 0 - station is OUTSIDE the field
  fvec fZ         = undef::kFvc;  ///< z position of station     [cm]
  fvec fZthick    = undef::kFvc;  ///< z thickness of the station     [cm]
  fvec Rmin       = undef::kFvc;  ///< min radius of the station [cm]
  fvec Rmax       = undef::kFvc;  ///< max radius of the station [cm]

  L1FieldSlice fieldSlice {};
  L1UMeasurementInfo frontInfo {};
  L1UMeasurementInfo backInfo {};
  L1UMeasurementInfo xInfo {};  ///< x axis in front,back coordinates
  L1UMeasurementInfo yInfo {};  ///< y axis in front,back coordinates

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& type;
    ar& timeInfo;
    ar& fieldStatus;

    ar& fZ;
    ar& fZthick;
    ar& Rmin;
    ar& Rmax;

    ar& fieldSlice;
    ar& frontInfo;
    ar& backInfo;
    ar& xInfo;
    ar& yInfo;
  }


  /// Prints object fields
  /// \param verbosity  Verbosity level of the output
  void Print(int verbosity = 0) const;

  /// String representation of class contents
  /// \param verbosityLevel  Verbosity level of the output
  /// \param indentLevel     Number of indent characters in the output
  std::string ToString(int verbosityLevel = 0, int indentLevel = 0) const;

  /// Verifies class invariant consistency
  /// \note Object is considered undefined in the creation time, so this function should be called after the object
  /// initialization
  void CheckConsistency() const;

  /// Converts Cartesian coordinates X and Y into strip coordinates U and V in transverse plane
  /// \param  x  X coordinate [length unit]
  /// \param  y  Y coordinate [length unit]
  /// \return Pair of U and V strip coordinates
  template<typename T, std::enable_if_t<std::is_floating_point<T>::value || std::is_same<T, fvec>::value, bool> = true>
  std::pair<T, T> ConvXYtoUV(T x, T y) const;

  /// Converts strip coordinates U and V into Cartesian coordinates X and Y in transverse plane
  /// \param  x  X coordinate [length unit]
  /// \param  y  Y coordinate [length unit]
  /// \return Pair of Y and X Cartesian coordinates
  template<typename T, std::enable_if_t<std::is_floating_point<T>::value || std::is_same<T, fvec>::value, bool> = true>
  std::pair<T, T> ConvUVtoXY(T u, T v) const;

  /// Converts variances of strip coordinates U and V into covariance matrix of Cartesian coordinates X and Y
  /// \param du  U coordinate uncertainty [length unit]
  /// \param dv  V coordinate uncertainty [length unit]
  /// \return Covariance matrix of hit position in Cartesian coordinates: [dxx, dxy, dyy] [length unit squared]
  template<typename T, std::enable_if_t<std::is_floating_point<T>::value || std::is_same<T, fvec>::value, bool> = true>
  std::tuple<T, T, T> FormXYCovarianceMatrix(T du2, T dv2) const;

} _fvecalignment;

//
// Template member functions implementation
//

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, std::enable_if_t<std::is_floating_point<T>::value || std::is_same<T, fvec>::value, bool>>
std::pair<T, T> L1Station::ConvXYtoUV(T x, T y) const
{
  if constexpr (std::is_same<T, fvec>::value) {
    return std::make_pair(x * frontInfo.cos_phi + y * frontInfo.sin_phi, x * backInfo.cos_phi + y * backInfo.sin_phi);
  }
  else {
    return std::make_pair(x * frontInfo.cos_phi[0] + y * frontInfo.sin_phi[0],
                          x * backInfo.cos_phi[0] + y * backInfo.sin_phi[0]);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, std::enable_if_t<std::is_floating_point<T>::value || std::is_same<T, fvec>::value, bool>>
std::pair<T, T> L1Station::ConvUVtoXY(T u, T v) const
{
  if constexpr (std::is_same<T, fvec>::value) {
    return std::make_pair(u * xInfo.cos_phi + v * xInfo.sin_phi, u * yInfo.cos_phi + v * yInfo.sin_phi);
  }
  else {
    return std::make_pair(u * xInfo.cos_phi[0] + v * xInfo.sin_phi[0], u * yInfo.cos_phi[0] + v * yInfo.sin_phi[0]);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, std::enable_if_t<std::is_floating_point<T>::value || std::is_same<T, fvec>::value, bool>>
std::tuple<T, T, T> L1Station::FormXYCovarianceMatrix(T du2, T dv2) const
{
  if constexpr (std::is_same<T, fvec>::value) {
    return std::make_tuple(xInfo.cos_phi * xInfo.cos_phi * du2 + xInfo.sin_phi * xInfo.sin_phi * dv2,  // dx2
                           xInfo.cos_phi * yInfo.cos_phi * du2 + xInfo.sin_phi * yInfo.sin_phi * dv2,  // dxy
                           yInfo.cos_phi * yInfo.cos_phi * du2 + yInfo.sin_phi * yInfo.sin_phi * dv2   // dy2
    );
  }
  else {
    return std::make_tuple(
      xInfo.cos_phi[0] * xInfo.cos_phi[0] * du2 + xInfo.sin_phi[0] * xInfo.sin_phi[0] * dv2,  // dx2
      xInfo.cos_phi[0] * yInfo.cos_phi[0] * du2 + xInfo.sin_phi[0] * yInfo.sin_phi[0] * dv2,  // dxy
      yInfo.cos_phi[0] * yInfo.cos_phi[0] * du2 + yInfo.sin_phi[0] * yInfo.sin_phi[0] * dv2   // dy2
    );
  }
}

#endif  // L1Station_h
