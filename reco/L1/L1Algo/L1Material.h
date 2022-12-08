/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov, Sergey Gorbunov [committer], Andrey Lebedev, Sergei Zharko */

#ifndef L1Material_h
#define L1Material_h

#include <boost/serialization/vector.hpp>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "L1Def.h"
#include "L1NaN.h"
#include "L1SimdSerializer.h"
#include "L1Undef.h"

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
  float GetRadThickBin(int iBinX, int iBinY) const { return fTable[iBinX + fNbins * iBinY]; }

  /// Gets material thickness in units of X0 in (x,y) point of the station
  /// \param x  X coordinate of the point [cm]
  /// \param y  Y coordinate of the point [cm]
  float GetRadThickScal(float x, float y) const;

  /// Gets material thickness in units of X0 in (x,y) point of the station
  /// fvec type can be float, that is why "Vec" and "Scal" specifications
  /// \param x  X coordinate of the point [cm] (SIMDized vector)
  /// \param y  Y coordinate of the point [cm] (SIMDized veotor)
  fvec GetRadThickVec(fvec x, fvec y) const;

  /// Checks, if the fields are NaN
  bool IsNaN() const { return L1NaN::IsNaN(fNbins) || L1NaN::IsNaN(fRmax) || L1NaN::IsNaN(fFactor); }

  /// Verifies class invariant consistency
  void CheckConsistency() const;

  /// Sets value of material thickness in units of X0 for a given cell of the material table
  /// WARNING: Indeces of rows and columns in the table runs from 0 to nBins-1 inclusively, where nBins is the number both of rows
  /// and columns. One should be carefull reading and storing the table from ROOT-file, because iBinX = 0 and iBinY = 0 in the
  /// SetBinContent method of a ROOT histogram usually defines underflow bin
  /// \param iBinX      Index of table column
  /// \param iBinY      Index of table row
  /// \param thickness  Thickness of the material in units of X0
  void SetRadThickBin(int iBinX, int iBinY, float thickness) { fTable[iBinX + fNbins * iBinY] = thickness; }

  /// Sets properties of the material table -- number of rows or columnts and the size of station in XY plane
  /// \param  nBins        Number of rows or columns
  /// \param  stationSize  Size of station in x and y dimensions [cm]
  void SetBins(int nBins, float stationSize);

  /// Swap method
  void Swap(L1Material& other) noexcept;

  /// repare the map - fill empty bins
  void Repare();

private:
  int fNbins    = undef::kI32;   ///< Number of rows (columns) in the material budget table
  float fRmax   = undef::kF;     ///< Size of the station in x and y dimensions [cm]
  float fFactor = undef::kF;     ///< Factor used in the recalculation of point coordinates to row/column id
  std::vector<float> fTable {};  ///< Material budget table

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& fNbins;
    ar& fRmax;
    ar& fFactor;
    ar& fTable;
  }
} _fvecalignment;

#endif
