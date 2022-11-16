/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   L1SearchWindow.h
/// \brief  Provides parameterisation for hits searching window in the CA tracking (header)
/// \date   08.11.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef L1SearchWindow_h
#define L1SearchWindow_h 1

#include <boost/serialization/array.hpp>
#include <boost/serialization/string.hpp>

#include <array>
#include <string>

/// TODO: SZh 8.11.2022: add selection of parameterisation

/// \brief Class L1SearchWindow defines a parameterisation of hits search window for CA tracking algorithm
/// TODO: SZh 8.11.2022: add description
class L1SearchWindow {
public:
  /// Constructor
  /// \param stationID  Global index of active station
  /// \param trackGrID  Track group ID
  L1SearchWindow(int stationID, int trackGrID);

  /// Default constructor
  L1SearchWindow() = default;

  /// Destructor
  ~L1SearchWindow() = default;

  /// Copy constructor
  L1SearchWindow(const L1SearchWindow& other) = default;

  /// Copy assignment operator
  L1SearchWindow& operator=(const L1SearchWindow& other) = default;

  /// Move constructor
  L1SearchWindow(L1SearchWindow&& other) noexcept = default;

  /// Move assignment operator
  L1SearchWindow& operator=(L1SearchWindow&& other) = default;

  /// Parameterisation function for dx_max(x0)
  float DxMaxVsX0(float /*x*/) const { return fvParams[kDxMaxVsX0 /*+ 0*/]; }

  /// Parameterisation function for dx_min(x0)
  float DxMinVsX0(float /*x*/) const { return fvParams[kDxMinVsX0 /*+ 0*/]; }

  /// Parameterisation function for dx_max(y0)
  float DxMaxVsY0(float /*x*/) const { return fvParams[kDxMaxVsY0 /*+ 0*/]; }

  /// Parameterisation function for dx_min(y0)
  float DxMinVsY0(float /*x*/) const { return fvParams[kDxMinVsY0 /*+ 0*/]; }

  /// Parameterisation function for dy_max(x0)
  float DyMaxVsX0(float /*x*/) const { return fvParams[kDyMaxVsX0 /*+ 0*/]; }

  /// Parameterisation function for dy_min(x0)
  float DyMinVsX0(float /*x*/) const { return fvParams[kDyMinVsX0 /*+ 0*/]; }

  /// Parameterisation function for dy_max(y0)
  float DyMaxVsY0(float /*y*/) const { return fvParams[kDyMaxVsY0 /*+ 0*/]; }

  /// Parameterisation function for dy_min(y0)
  float DyMinVsY0(float /*y*/) const { return fvParams[kDyMinVsY0 /*+ 0*/]; }


  /// Gets station id
  int GetStationID() const { return fStationID; }

  /// Gets track group id
  int GetTrackGroupID() const { return fTrackGroupID; }

  /// Sets tag
  /// A tag can be used for insurance, if this search window corresponds to a desired track finder iteration
  void SetTag(const char* name) { fsTag = name; }

  /// TODO: SZh 08.11.2022: Implement variadic  template function
  /// TODO: SZh 08.11.2022: Try to reduce number of functions
  /// Sets parameters for dx_max(x0)
  /// \param id  Parameter index
  /// \param val Parameter value
  void SetParamDxMaxVsX0(int id, float val);

  /// Sets parameters for dx_min(x0)
  /// \param id  Parameter index
  /// \param val Parameter value
  void SetParamDxMinVsX0(int id, float val);

  /// Sets parameters for dx_max(y0)
  /// \param id  Parameter index
  /// \param val Parameter value
  void SetParamDxMaxVsY0(int id, float val);

  /// Sets parameters for dx_min(y0)
  /// \param id  Parameter index
  /// \param val Parameter value
  void SetParamDxMinVsY0(int id, float val);

  /// Sets parameters for dy_max(x0)
  /// \param id  Parameter index
  /// \param val Parameter value
  void SetParamDyMaxVsX0(int id, float val);

  /// Sets parameters for dy_min(x0)
  /// \param id  Parameter index
  /// \param val Parameter value
  void SetParamDyMinVsX0(int id, float val);

  /// Sets parameters for dy_max(y0)
  /// \param id  Parameter index
  /// \param val Parameter value
  void SetParamDyMaxVsY0(int id, float val);

  /// Sets parameters for dy_min(y0)
  /// \param id  Parameter index
  /// \param val Parameter value
  void SetParamDyMinVsY0(int id, float val);

  /// String representation of the contents
  std::string ToString() const;

private:
  static constexpr unsigned char kNpars = 1;  ///< Max number of parameters for one dependency
  static constexpr unsigned char kNdeps = 8;  ///< Number of the dependencies

  /// Enumeration for dependencies stored
  enum EDependency
  {
    kDxMaxVsX0,
    kDxMinVsX0,
    kDxMaxVsY0,
    kDxMinVsY0,
    kDyMaxVsX0,
    kDyMinVsX0,
    kDyMaxVsY0,
    kDyMinVsY0
  };

  /// Search window parameter array containing parameters from
  /// - dx_max vs x0 - indexes [0          .. kNpars - 1]
  /// - dx_min vs x0 - indexes [kNpars     .. (2 * kNpars - 1)]
  /// - dx_max vs y0 - indexes [2 * kNpars .. (3 * kNpars - 1)]
  /// - dx_min vs y0 - indexes [3 * kNpars .. (4 * kNpars - 1)]
  /// - dy_max vs y0 - indexes [4 * kNpars .. (5 * kNpars - 1)]
  /// - dy_min vs y0 - indexes [5 * kNpars .. (6 * kNpars - 1)]
  /// - dy_max vs y0 - indexes [6 * kNpars .. (7 * kNpars - 1)]
  /// - dy_min vs y0 - indexes [7 * kNpars .. (8 * kNpars - 1)]
  std::array<float, kNdeps* kNpars> fvParams = {0};

  int fStationID    = -1;  ///< Global index of active tracking station
  int fTrackGroupID = -1;  ///< Index of tracks group
  std::string fsTag = "";  ///< Tag, containing information on the tracks group (TODO: can be omitted?)

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& fvParams;
    ar& fStationID;
    ar& fTrackGroupID;
    ar& fsTag;
  }
};

#endif  // L1SearchWindow_h
