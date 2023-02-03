/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmTofInteraction.h
/// \date   02.02.2023
/// \brief  Representation of MC track interaction with a TOF module
/// \author P.-A. Loizeau
/// \author S. Zharko

#ifndef CbmTofInteraction_h
#define CbmTofInteraction_h 1

#include "CbmTofPoint.h"

#include <string>
#include <vector>
/// Class describes an interaction of a MC track with a TOF module as a solid structure
///
class CbmTofInteraction : public CbmTofPoint {
public:
  // ----- Constructors and destructor
  /// Default constructor
  CbmTofInteraction();

  /// Constructor with signature from CbmTofPoint
  /// \param args  List of parameters for any CbmTofPoint constructor
  template<typename... Args>
  CbmTofInteraction(Args... args) : CbmTofPoint(args...)
                                  , fNofPoints(0)
  {
  }

  /// Destructor
  ~CbmTofInteraction() = default;

  // ----- Modifiers
  /// \brief Adds a point to the interaction
  /// New point updates the following properties of the interaction: position, entrance momentum
  /// \param pPoint  Pointer to TOF MC-point
  void AddPoint(const CbmTofPoint* pPoint);

  /// \brief Clears the instance
  void Clear();

  /// \brief Sets parameters from a particular TOF MC-point
  /// \param pPoint Pointer to TOF point
  /// The purpose of the
  void SetFromPoint(const CbmTofPoint* pPoint);

  // ----- Getters
  /// \brief Gets number of stored points
  int GetNofPoints() const { return fNofPoints; }

  /// \brief Saves content of the class to string
  std::string ToString() const;

  /// \brief Gets pointers to points (TMP!!!!)
  const std::vector<const CbmTofPoint*>& GetPoints() const { return fvpPoints; }

private:
  // ----- Utility functions
  /// \brief Updates average of the property from original TOF MC-point
  /// \param update    Property of the added MC point
  /// \param property  Updated property of this interaction
  template<typename T>
  void UpdateAverage(const T& update, T& property)
  {
    property = (fNofPoints * property + update) / (fNofPoints + 1);
  }

  // ----- Properties
  int fNofPoints = 0;  ///< Number of CbmTofPoint objects, from which the interaction is constructed
  std::vector<const CbmTofPoint*> fvpPoints;  ///< Vector of point pointer (TMP!!!!)
};

#endif  // CbmTofInteraction_h
