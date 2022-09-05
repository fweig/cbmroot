/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Igor Kulakov, Sergei Zharko */

#ifndef L1Station_h
#define L1Station_h 1

#include <string>
#include <type_traits>

#include "L1Field.h"
#include "L1MaterialInfo.h"
#include "L1NaN.h"
#include "L1SimdSerializer.h"
#include "L1UMeasurementInfo.h"
#include "L1Utils.h"
#include "L1XYMeasurementInfo.h"

/// Structure L1Station
/// Contains a set of geometry parameters for a particular station
///
class L1Station {
public:
  int type {L1NaN::SetNaN<decltype(type)>()};
  int timeInfo {L1NaN::SetNaN<decltype(timeInfo)>()};  ///< flag: if time information can be used
  int fieldStatus {L1NaN::SetNaN<decltype(
    fieldStatus)>()};  ///< flag: 1 - station is INSIDE the field, 0 - station is OUTSIDE the field (replace with enum)
  fvec z {L1NaN::SetNaN<decltype(z)>()};        ///< z position of station     [cm]
  fvec Rmin {L1NaN::SetNaN<decltype(Rmin)>()};  ///< min radius of the station [cm]
  fvec Rmax {L1NaN::SetNaN<decltype(Rmax)>()};  ///< max radius of the station [cm]
  fvec dt {L1NaN::SetNaN<decltype(dt)>()};      ///< time resolution [ns]
  /// structure containing station thickness(X), rad. length (X0), X/X0 and log(X/X0) values
  L1MaterialInfo materialInfo {};
  L1FieldSlice fieldSlice {};
  L1UMeasurementInfo frontInfo {};
  L1UMeasurementInfo backInfo {};
  L1UMeasurementInfo xInfo {};  ///< x axis in front,back coordinates
  L1UMeasurementInfo yInfo {};  ///< y axis in front,back coordinates
  L1XYMeasurementInfo XYInfo {};

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& type;
    ar& timeInfo;
    ar& fieldStatus;

    ar& z;
    ar& Rmin;
    ar& Rmax;
    ar& dt;

    ar& materialInfo;
    ar& fieldSlice;
    ar& frontInfo;
    ar& backInfo;
    ar& xInfo;
    ar& yInfo;
    ar& XYInfo;
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
  std::tuple<T, T, T> FormXYCovarianceMatrix(T du, T dv) const;

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
std::tuple<T, T, T> L1Station::FormXYCovarianceMatrix(T du, T dv) const
{
  if constexpr (std::is_same<T, fvec>::value) {
    return std::make_tuple(
      (xInfo.cos_phi * du) * (xInfo.cos_phi * du) + (xInfo.sin_phi * dv) * (xInfo.sin_phi * dv),  // dx2
      (xInfo.cos_phi * du) * (yInfo.cos_phi * du) + (xInfo.sin_phi * dv) * (yInfo.sin_phi * dv),  // dxy
      (yInfo.cos_phi * du) * (yInfo.cos_phi * du) + (yInfo.sin_phi * dv) * (yInfo.sin_phi * dv)   // dy2
    );
  }
  else {
    return std::make_tuple(
      (xInfo.cos_phi[0] * du) * (xInfo.cos_phi[0] * du) + (xInfo.sin_phi[0] * dv) * (xInfo.sin_phi[0] * dv),  // dx2
      (xInfo.cos_phi[0] * du) * (yInfo.cos_phi[0] * du) + (xInfo.sin_phi[0] * dv) * (yInfo.sin_phi[0] * dv),  // dxy
      (yInfo.cos_phi[0] * du) * (yInfo.cos_phi[0] * du) + (yInfo.sin_phi[0] * dv) * (yInfo.sin_phi[0] * dv)   // dy2
    );
  }
}

#endif  // L1Station_h
