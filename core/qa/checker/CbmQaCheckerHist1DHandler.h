/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerHist1DHandler.h
/// @brief  Handler class for 1D-histograms (including TProfile objects) (declaration)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  09.02.2023

#ifndef CbmQaCheckerHist1DHandler_h
#define CbmQaCheckerHist1DHandler_h 1

#include "CbmQaCheckerObjectHandler.h"

#include <memory>
#include <string>

class TObject;
class TH1;

namespace cbm::qa::checker
{
  /// @brief Handler for 1D-histograms.
  ///
  /// The handler keeps one-dimensional histogram objects of the same quantity within different code versions
  /// and provides several comparison methods including point-by-point comparison, test of ratio deviation and
  /// statistical hypothesis tests (Chi2 test and Kolmogorov test).
  ///
  class Hist1DHandler : public ObjectHandler {
  public:
    /// Enumeration for bit-flags of different comparison methods
    enum EFlags
    {
      kPOINT,  ///< Point-by-point exact comparison
      kRATIO,  ///< Comparison within of ratio difference from unity
      kCHI2,   ///< Statistical hypothesis test using chi2 test
      kKOLM    ///< Statistical hypothesis test using Kolmogorov test
    };

    /// @brief Constructor
    /// @param iObject  Index of object
    /// @param iFile    Index of file
    /// @param iDataset Index of dataset
    Hist1DHandler(int iObject, int iFile, int iDataset);

    /// @brief Destructor
    ~Hist1DHandler() = default;

    /// @brief Creates object comparison canvas
    void CreateCanvases();

    /// @brief Compares objects to default
    /// @param pNum    Pointer to "numerator" object
    /// @param pDenom  Pointer to "denominator" object
    /// @return  Comparison result represented as a bitset in unsigned integer
    ///
    /// The function provides comparison of objects and stores the comparison result in the field of the ObjectDB
    /// instance for a given version, file, object and dataset ids. The comparison result is represented with the
    /// unsigned integer, each raised bit of which declares, that the objects are different within the corresponding
    /// comparison method.
    ///         Bits used:
    ///         0: Point to point comparison
    ///         1: Comparison based on the ratio deviation
    ///         2: Statistical test comparison with chi2-test
    ///         3: Statistical test comparison with Kolmogorov test
    CmpResult_t Compare(const TNamed* pNum, const TNamed* pDenom) const;

  private:
    /// @brief Compares two histograms point-to-point
    /// @param pNumerator    Pointer to the numerator object of comparison (usually new version)
    /// @param pDenominator  Pointer to the denominator object for comparison (usually default)
    /// @return true  Histograms are different
    /// @return false Histograms are equal
    static bool ComparePointToPoint(const TH1* pNumerator, const TH1* pDenominator);

    /// @brief Compares two histograms by maximum deviation of the ratio
    /// @param pNumerator    Pointer to the numerator object of comparison (usually new version)
    /// @param pDenominator  Pointer to the denominator object for comparison (usually default)
    /// @param allowedDev    Max allowed ratio deviation from unity
    /// @return true  Histograms are different
    /// @return false Histograms are equal
    static bool CompareRatioDeviation(const TH1* pNumerator, const TH1* pDenominator, double allowedDev = 10);

    /// @brief Compares two histograms with chi2-test
    /// @param pNumerator    Pointer to the numerator object of comparison (usually new version)
    /// @param pDenominator  Pointer to the denominator object for comparison (usually default)
    /// @return true  Histograms are different
    /// @return false Histograms are equal
    static bool CompareWithChi2(const TH1* pNumerator, const TH1* pDenominator);

    /// @brief Compares two histograms with Kolmogorov test
    /// @param pNumerator    Pointer to the numerator object of comparison (usually new version)
    /// @param pDenominator  Pointer to the denominator object for comparison (usually default)
    /// @return true  Histograms are different
    /// @return false Histograms are equal
    static bool CompareWithKolmogorov(const TH1* pNumerator, const TH1* pDenominator);
  };
}  // namespace cbm::qa::checker

#endif  // CbmQaCheckerHist1DHandler_h
