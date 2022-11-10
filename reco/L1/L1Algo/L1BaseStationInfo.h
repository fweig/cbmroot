/* Copyright (C) 2021-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1BaseStationInfo.h
 * @brief A base class for a L1 station interface
 * @since 18.12.2021
 *
 * The class is implemented to connect concrete experimental setup (CBM or BMN in CbmL1 
 * or BmnL1) with general L1Algo tracking.
 * 
 ***********************************************************************************************************/

#ifndef L1BaseStationInfo_h
#define L1BaseStationInfo_h 1

// L1 Core
#include "L1ObjectInitController.h"
#include "L1Station.h"
// C++ std
#include <bitset>
#include <functional>
#include <string>

enum class L1DetectorID;

/// A base class which provides interface to L1Algo station geometry
class L1BaseStationInfo {
  /// Enumeration for internal logic control
  enum class EManagementFlag
  {
    kThicknessMapMoved,  ///< if the thickness map was moved from the L1BaseStationInfo instance
    kEnd
  };
  using ManagementFlags_t = std::bitset<static_cast<int>(EManagementFlag::kEnd)>;

public:
  /// Enumeration of fields, which must be initialized so the object can pass the threshold
  enum class EInitKey
  {
    // Basic fields initialization
    kDetectorID,      ///< detector ID
    kStationID,       ///< station ID
    kTrackingStatus,  ///< flag, if station is used in tracking or not
    kXmax,            ///< max size in X direction
    kYmax,            ///< max size in Y direction
    // L1Station initialization
    kType,            ///< station type
    kTimeInfo,        ///< if time info is used (flag)
    kFieldStatus,     ///< if station is placed in field (flag)
    kZ,               ///< z coordinate of the station position
    kRmin,            ///< internal radius of station (gap size)
    kRmax,            ///< exteranl radius of station
    kZthickness,      ///< Z thickness of the station
    kThicknessMap,    ///< thickness map of the station (optional?)
    kFieldSlice,      ///< L1Station.L1FieldSlice object initialization
    kStripsFrontPhi,  ///< strips geometry initialization
    kStripsBackPhi,   ///<
    // The last item is equal to the number of bits in fInitFlags
    kEnd
  };

  using InitController_t = L1ObjectInitController<static_cast<int>(EInitKey::kEnd), EInitKey>;

  //
  // CONSTRUCTORS AND DESTRUCTORS
  //
  /// Default constructor
  L1BaseStationInfo() noexcept;

  /// Constructor from stationID and detectorID
  L1BaseStationInfo(L1DetectorID detetorID, int stationID) noexcept;

  /// Destructor
  ~L1BaseStationInfo() noexcept;

  /// Copy constructor
  L1BaseStationInfo(const L1BaseStationInfo& other) noexcept;

  /// Move constructor
  L1BaseStationInfo(L1BaseStationInfo&& other) noexcept;

  /// Copy assignment operator
  L1BaseStationInfo& operator=(const L1BaseStationInfo& other) noexcept;

  /// Move assignment operator
  L1BaseStationInfo& operator=(L1BaseStationInfo&& other) noexcept;

  /// Less operator for L1BaseStationInfo object to perform sorts. Sorting is carried out first by fDetectorID,
  /// and then by fStationID. The detectorID must be defined in L1DetectorID enum class
  bool operator<(const L1BaseStationInfo& right) const
  {
    return fDetectorID != right.fDetectorID ? fDetectorID < right.fDetectorID : fStationID < right.fStationID;
  }

  /// Gets detector ID
  L1DetectorID GetDetectorID() const { return fDetectorID; }

  /// Gets a coefficient with idx for the field x-component approximation
  fvec GetFieldSliceCx(int idx) const { return fL1Station.fieldSlice.cx[idx]; }

  /// Gets a coefficient with idx for the field y-component approximation
  fvec GetFieldSliceCy(int idx) const { return fL1Station.fieldSlice.cy[idx]; }

  /// Gets a coefficient with idx for the field z-component approximation
  fvec GetFieldSliceCz(int idx) const { return fL1Station.fieldSlice.cz[idx]; }

  /// Gets array of the coefficients for the field x-component approximation
  const fvec* GetFieldSliceCx() const { return fL1Station.fieldSlice.cx; }

  /// Gets array of the coefficients for the field y-component approximation
  const fvec* GetFieldSliceCy() const { return fL1Station.fieldSlice.cy; }

  /// Gets array of the coefficients for the field z-component approximation
  const fvec* GetFieldSliceCz() const { return fL1Station.fieldSlice.cz; }

  /// Gets field status: 0 - station is outside the field, 1 - station is inside the field
  int GetFieldStatus() const { return fL1Station.fieldStatus; }

  /// Gets a const reference to the L1ObjectInitController object
  const InitController_t& GetInitController() const { return fInitController; }

  /// Gets a reference to L1Station info field of the L1BaseStation info
  const L1Station& GetL1Station() const;

  /// Gets a reference to L1Material map
  const L1Material& GetMaterialMap() const;

  /// Gets station thickness
  fvec GetZthickness() const { return fL1Station.fZthick; }

  /// Gets min transverse size of the station [cm]
  fvec GetRmin() const { return fL1Station.Rmin; }

  /// Gets max transverse size of the station [cm]
  fvec GetRmax() const { return fL1Station.Rmax; }

  /// Gets station ID
  int GetStationID() const { return fStationID; }

  /// Gets station type
  int GetStationType() const { return fL1Station.type; }

  /// Gets tracking status: true - station is active for tracking, false - station exists, but not used in tracking
  bool GetTrackingStatus() const { return fTrackingStatus; }

  /// Gets maximum distance between station center and its edge in x direction
  double GetXmax() const { return fXmax; }

  /// Gets maximum distance between station center and its edge in y direction
  double GetYmax() const { return fYmax; }

  /// Gets double precised z position of the station [cm]
  double GetZdouble() const { return fZPos; }

  /// Gets SIMD vectorized z position of the station [cm]
  fvec GetZsimdVec() const { return fL1Station.fZ; }

  /// Prints registered fields
  /// verbosity = 0: print only station id, detector id and address in one line
  /// verbosity = 1: print basic L1Station fields
  /// verbosity = 2: print all L1Staion fields
  void Print(int verbosity = 0) const;

  /// Resets fields to the default values
  void Reset();

  /// Sets detector ID
  void SetDetectorID(L1DetectorID inID);

  /// Sets flag: true - station is placed in field, false - station is placed outside the field
  void SetFieldStatus(int fieldStatus);

  /// Sets arrays of the approcimation
  /// \param getField A user function, which gets a xyz array of position coordinates and fills B array
  ///                 of magnetic field components in position
  void SetFieldFunction(const std::function<void(const double (&xyz)[3], double (&B)[3])>& getFieldValue);

  /// Sets stereo angles for front and back strips
  /// \param f_phi   Stereoangle of front strips [rad]
  /// \param b_phi   Stereoangle of back strips  [rad]
  void SetFrontBackStripsGeometry(double fPhi, double bPhi);

  /// Sets station thickness and radiation length
  /// \param thickness       Thickness of station [arb. units]
  void SetZthickness(double thickness);

  /// Sets station thickness in units of radiation length mapped vs. position in XY plane (copy semantics)
  /// \param thicknessMap  Map of station thickness in units of radiation length
  void SetMaterialMap(const L1Material& thicknessMap);

  /// Sets station thickness in units of radiation length mapped vs. position in XY plane (move semantics)
  /// \param thicknessMap  Map of station thickness in units of radiation length
  void SetMaterialMap(L1Material&& thicknessMap) noexcept;

  /// Sets max transverse size of the station []
  void SetRmax(double inRmax);

  /// Sets min transverse size of the station []
  void SetRmin(double inRmin);

  /// Sets station ID
  void SetStationID(int inID);

  /// Sets type of station
  void SetStationType(int inType);  // TODO: this is a temporary solution (S.Zh.)

  /// Sets flag: 0 - time information is not provided by this detector type
  ///            1 - time information is provided by the detector and can be used in tracking
  void SetTimeInfo(int inTimeInfo);

  /// Sets tracking status: true - station is active for tracking, false - station exists, but not used in tracking
  void SetTrackingStatus(bool flag);

  /// Sets maximum distance between station center and its edge in x direction
  void SetXmax(double aSize);

  /// Sets maximum distance between station center and its edge in y direction
  void SetYmax(double aSize);

  /// Sets nominal z position of the station
  void SetZ(double inZ);

  /// Swap method for easy implementation of move constructor and copy and move assignment operator
  void Swap(L1BaseStationInfo& other) noexcept;

  /// Takes an instance of L1Material map
  /// This method can be called only once. It will catch the content of L1Material map (using move semantics), and after the warning will be generated
  /// both for GetMaterialMap and TakeMaterialMap
  L1Material&& TakeMaterialMap();

  /// String representation of class contents
  /// \param indentLevel  Number of indent characters in the output
  std::string ToString(int verbosityLevel = 0, int indentLevel = 0) const;

private:
  L1DetectorID fDetectorID {static_cast<L1DetectorID>(0)};  ///< Detector ID
  int fStationID {-1};                                      ///< Station ID
  bool fTrackingStatus {false};                             ///< Tracking status: true - station is used for tracking
  double fXmax {0};                     ///< Maximum distance between station center and its edge in x direction
  double fYmax {0};                     ///< Maximum distance between station center and its edge in y direction
  double fZPos {0};                     ///< z position of the station in double precision, used in field approximation
  L1Station fL1Station {};              ///< L1Station structure, describes a station in L1Algo
  L1Material fThicknessMap {};          ///< Map of station thickness in units of radiation length
  InitController_t fInitController {};  ///< Class fileds initialization flags
  ManagementFlags_t fManagementFlags {};  ///< bitset flags to manage internal behaviour of the class
};

/// swap function for two L1BaseStationInfo objects, expected to be used instead of std::swap
inline void swap(L1BaseStationInfo& a, L1BaseStationInfo& b) noexcept { a.Swap(b); }

#endif  // L1BaseStationInfo_h
