/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1BaseStationInfo.h
 * @bried A base class for a L1 station interface
 * @since 18.12.2021
 *
 * The class is implemented to connect concrete experimental setup (CBM or BMN in CbmL1 
 * or BmnL1) with general L1Algo tracking.
 * 
 ***********************************************************************************************************/

#ifndef L1BaseStationInfo_h
#define L1BaseStationInfo_h 1

// C++ std
#include <bitset>
#include <functional>

#include "L1Station.h"

enum class L1DetectorID;

/// A base class which provides interface to L1Algo station geometry
class L1BaseStationInfo {
private:
  enum
  {  // Here we list all the fields, which must be initialized by user
    // Basic fields initialization
    keDetectorID,
    keStationID,
    keXmax,
    keYmax,
    // L1Station initialization
    keType,
    keTimeInfo,
    keZ,
    keRmin,
    keRmax,
    keMaterialInfoThick,
    keMaterialInfoRL,
    keFieldSlice,
    keStripsFrontPhi,
    keStripsFrontSigma,
    keStripsBackPhi,
    keStripsBackSigma,
    // The last item is equal to the number of bits in fInitFlags
    keEnd
  };

public:
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
  /// Copy operator=
  L1BaseStationInfo& operator=(const L1BaseStationInfo& other) noexcept;
  /// Move operator=
  L1BaseStationInfo& operator=(L1BaseStationInfo&& other) noexcept;
  /// Swap method for easier implementation of above ones (NOTE: all the fields must be accounted carefully here)
  void Swap(L1BaseStationInfo& other) noexcept;

  //
  // BASIC METHODS
  //

  /// Checks if all the necessary fields are initialized by user
  bool IsInitialized() const { return fInitFlags.size() == fInitFlags.count(); }
  /// Prints registered fields
  /// verbosity = 0: print only station id, detector id and address in one line
  /// verbosity = 1: print basic L1Station fields
  /// verbosity = 2: print all L1Staion fields
  void Print(int verbosity = 0) const;
  /// Print initialization flags (debug function)
  void PrintInit() const;
  /// Resets fields to the default values
  void Reset();
  /// Less operator for L1BaseStationInfo object to perform their sorts. Sorting is carried out first by fDetectorID,
  /// and then by fStationID
  bool operator<(const L1BaseStationInfo& right) const
  {
    return fDetectorID != right.fDetectorID ? fDetectorID < right.fDetectorID : fStationID < right.fStationID;
  }


  //
  // GETTERS
  //

  /// Gets station ID
  int GetStationID() const { return fStationID; }
  /// Gets detector ID
  L1DetectorID GetDetectorID() const { return fDetectorID; }
  /// Gets station type
  int GetStationType() const { return fL1Station.type; }
  /// Gets SIMD vectorized z position of the station
  fvec GetZsimdVec() const { return fL1Station.z; }
  /// Gets double precised z position of the station
  double GetZdouble() const { return fZPos; }
  /// Gets min transverse size of the station
  fvec GetRmin() const { return fL1Station.Rmin; }
  /// Gets max transverse size of the station
  fvec GetRmax() const { return fL1Station.Rmax; }

  /// Gets material thickness
  fvec GetMaterialThickness() const { return fL1Station.materialInfo.thick; }
  /// Gets the radiation length of the station material
  fvec GetMaterialRadLength() const { return fL1Station.materialInfo.RL; }
  /// Gets the relative material thickness in units of the radiational length
  fvec GetMaterialRadThick() const { return fL1Station.materialInfo.RadThick; }
  /// Gets log of the relative material thickness in units of the radiational length
  fvec GetMaterialLogRadThick() const { return fL1Station.materialInfo.logRadThick; }

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

  /// Gets maximum distance between station center and its edge in x direction
  double GetXmax() const { return fXmax; }
  /// Gets maximum distance between station center and its edge in y direction
  double GetYmax() const { return fYmax; }

  /// Gets a reference to L1Station info field of the L1BaseStation info
  const L1Station& GetL1Station() const;
  /// Gets a reference to Bitset object of initialization bits
  const std::bitset<L1BaseStationInfo::keEnd>& GetInitFlags() const { return fInitFlags; }


  //
  //  SETTERS
  //

  /// Sets station ID
  void SetStationID(int inID);
  /// Sets detector ID
  void SetDetectorID(L1DetectorID inID);

  /// Sets type of station
  void SetStationType(int inType);  // TODO: this is a temporary solution (S.Zh.)
  /// Sets flag: 0 - time information is not provided by this detector type
  //             1 - time information is provided by the detector and can be used in tracking
  void SetTimeInfo(int inTimeInfo);
  /// Sets nominal z position of the station
  void SetZ(double inZ);
  /// Sets min transverse size of the station
  void SetRmin(double inRmin);
  /// Sets max transverse size of the station
  void SetRmax(double inRmax);
  /// Sets station thickness and radiational length
  /// \param thickness         thickness of station
  /// \param radiationalLength radiational length of station
  void SetMaterial(double thickness, double radiationalLength);
  /// Sets field approximation at the station plain
  /// \param Cx Array of approximation coefficients for x field component
  /// \param Cy Array of approximation coefficients for y field component
  /// \param Cz Array of approximation coefficients for z field component
  void SetFieldSlice(const double* Cx, const double* Cy, const double* Cz);
  /// Sets arrays of the approcimation
  /// \param getField A user function, which gets a xyz array of position coordinates and fills B array
  ///                 of magnetic field components in position
  void SetFieldSlice(const std::function<void(const double (&xyz)[3], double (&B)[3])>& getFieldValue);
  /// Sets stereo angles and sigmas for front and back strips
  /// \param f_phi   Stereoangle of front strips
  /// \param f_sigma Sigma of front strips
  /// \param b_phi   Stereoangle of back strips
  /// \param b_sigma Sigma of back strips
  void SetFrontBackStripsGeometry(double fPhi, double fSigma, double bPhi, double bSigma);
  /// Sets maximum distance between station center and its edge in x direction
  void SetXmax(double aSize);
  /// Sets maximum distance between station center and its edge in y direction
  void SetYmax(double aSize);

private:
  L1DetectorID fDetectorID {static_cast<L1DetectorID>(0)};  ///< Detector ID
  int fStationID {-1};                                      ///< Station ID
  double fXmax {0};         ///< Maximum distance between station center and its edge in x direction
  double fYmax {0};         ///< Maximum distance between station center and its edge in y direction
  double fZPos {0};         ///< z position of the station in double precision, used in field approximation
  L1Station fL1Station {};  ///< L1Station structure, describes a station in L1Algo
  std::bitset<L1BaseStationInfo::keEnd> fInitFlags;  ///< Class fileds initialization flags
};

/// swap function for two L1BaseStationInfo objects, expected to be used instead of std::swap
inline void swap(L1BaseStationInfo& a, L1BaseStationInfo& b) noexcept { a.Swap(b); }

#endif  // L1BaseStationInfo_h
