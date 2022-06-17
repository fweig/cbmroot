/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov, Sergey Gorbunov [committer], Andrey Lebedev, Sergei Zharko */

#ifndef L1MaterialInfo_h
#define L1MaterialInfo_h

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "L1Def.h"
#include "L1Utils.h"

/// Class L1MaterialInfo contains SIMDized vector fields of the
/// The fields of the structure should ONLY be initialized within L1BaseStationInfo::SetMaterial(double, double) method, when the
/// stations sequence is initialized
struct L1MaterialInfo {
  fvec thick {L1Utils::kNaN};  ///< Average thickness of the station in arbitary length units
  fvec RL {
    L1Utils::kNaN};  ///< Average radiation length (X0) of the station material in THE SAME UNITS as the thickness
  fvec RadThick {L1Utils::kNaN};  ///< Average thickness in units of radiation length (X/X0)
  fvec logRadThick {L1Utils::kNaN};

  /// Verifies class invariant consistency
  void CheckConsistency() const;

  /// String representation of class contents
  /// \param indentLevel    number of indent characters in the output
  std::string ToString(int indentLevel = 0) const;
} _fvecalignment;

/// Class L1Material describes a map of station thickness in units of radiation length (X0) to the specific point in XY plane
class L1Material {
public:
  /// Constructor
  /// \param  nBins        Number of rows or columns
  /// \param  stationSize  Size of station in x and y dimensions [cm]
  L1Material();

  /// Copy constructor
  L1Material(const L1Material& other);

  /// Copy assignment operator
  L1Material& operator=(const L1Material& other);

  /// Move constructor
  L1Material(L1Material&& other) noexcept;

  /// Move assignment operator
  L1Material& operator=(L1Material&& other) noexcept;

  /// Destructor
  ~L1Material() noexcept;

  /// Gets number of bins (rows or columns) of the material table
  int GetNbins() const { return fNbins; }

  /// Gets value of X/X0 in a given cell of the material table by the indeces of the row and column
  /// \param iBinX  Index of table column
  /// \param iBinY  Index of table row
  float GetRadThick(int iBinX, int iBinY) const { return fTable[iBinX + fNbins * iBinY]; }

  /// Gets material thickness in units of X0 in (x,y) point of the station
  /// \param x  X coordinate of the point [cm]
  /// \param y  Y coordinate of the point [cm]
  float GetRadThick(float x, float y) const;

  /// Gets material thickness in units of X0 in (x,y) point of the station
  /// \param x  X coordinate of the point [cm] (SIMDized vector)
  /// \param y  Y coordinate of the point [cm] (SIMDized veotor)
  fvec GetRadThick(fvec x, fvec y) const;

  /// Verifies class invariant consistency
  void CheckConsistency() const;

  /// Sets value of material thickness in units of X0 for a given cell of the material table
  /// WARNING: Indeces of rows and columns in the table runs from 0 to nBins-1 inclusively, where nBins is the number both of rows
  /// and columns. One should be carefull reading and storing the table from ROOT-file, because iBinX = 0 and iBinY = 0 in the
  /// SetBinContent method of a ROOT histogram usually defines underflow bin
  /// \param iBinX      Index of table column
  /// \param iBinY      Index of table row
  /// \param thickness  Thickness of the material in units of X0
  void SetRadThick(int iBinX, int iBinY, float thickness) { fTable[iBinX + fNbins * iBinY] = thickness; }

  /// Sets properties of the material table -- number of rows or columnts and the size of station in XY plane
  /// \param  nBins        Number of rows or columns
  /// \param  stationSize  Size of station in x and y dimensions [cm]
  void SetBins(int nBins, float stationSize);

  /// Swap method
  void Swap(L1Material& other) noexcept;

private:
  int fNbins {-1};                ///< Number of rows (columns) in the material budget table
  float fRmax {L1Utils::kNaN};    ///< Size of the station in x and y dimensions [cm]
  float fFactor {L1Utils::kNaN};  ///< Factor used in the recalculation of point coordinates to row/column id
  std::vector<float> fTable {};  ///< Material budget table
} _fvecalignment;

#endif
