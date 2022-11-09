/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsWindowFinder.h
/// \brief  Framework for CA tracking hit-finder window estimation from MC (header)
/// \since  14.10.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsWindowFinder_h
#define CaToolsWindowFinder_h 1

#include "TObject.h"

// TODO: Replace tmp asserts with exceptions

class TChain;

namespace ca
{
  namespace tools
  {

    /// Enumeration of parameterisations used to describe a search window
    enum class EWindowForm
    {
      kConstant,
      kLinear,
      kParabolic,
      kElliptic
    };

    /// Enumeration
    enum class EBuildingMode
    {
      kDouplets,  ///< Windows are built from doublets and a target
      kTriplets   ///< Windows are built from triplets without using target
    };

    /// TODO: ... write an instruction ...
    class WindowFinder : public TObject {
    public:
      // TODO: TEMPORARY CONSTANT EXPRESSIONS (TO BE MOVED TO A SEPARATE HEADER)
      static constexpr const char* kTreeName = "t";  ///< Name of the input MC triplets tree

    public:
      /// Default constructor
      WindowFinder();

      virtual ~WindowFinder() = default;

      /// Copy and move are forbidden
      WindowFinder(const WindowFinder&) = delete;
      WindowFinder(WindowFinder&&)      = delete;
      WindowFinder& operator=(const WindowFinder&) = delete;
      WindowFinder& operator=(WindowFinder&&) = delete;

      /// Adds an input file with a tree object of MC triplets
      /// \note Multiple file input is possible
      void AddInputFile(const char* filename);

      /// Process a tree (TEST)
      void Process(EBuildingMode buildingMode);

      /// Sets binning of the dx (dy) distribution
      /// \param nBinsX  Number of bins for the x-axis
      /// \param nBinsY  Number of bins for the y-axis
      void SetBinning(int nBinsX, int nBinsY);

      /// Sets a fraction of triplets (doublets), which can be omitted by the window
      void SetEpsilon(float eps);

      /// Sets number of slices along the x-axis
      /// If the number of slices larger then 1, the window bounds will be fitted with a function (...which TODO).
      /// Otherwise, the bounds will be represented with constants (independent from x0 or y0)
      void SetNslices(int nSlices);

      /// Sets components of the target center position
      /// \param  x  Position x-component [cm]
      /// \param  y  Position y-component [cm]
      /// \param  z  Position z-component [cm]
      void SetTarget(double x, double y, double z);


      // ******************************
      // **     Member variables     **
      // ******************************

    private:
      std::array<double, 3> fTargetPos = {0};  ///< Target position {x, y, z} [cm]

      // Window extraction settings

      int fNbinsX  = 360;    ///< Number of bins for the X axis of the distribution
      int fNbinsY  = 800;    ///< Number of bins for the Y axis of the distribution
      int fNslices = 1;      ///< Number of slices along the X axis
      float fEps   = 0.001;  ///< Fraction of triplets (doublets), which can be omitted

      TChain* fpMcTripletsTree = nullptr;  ///< Chain of trees containing MC triplets, generated in CbmL1 Performance

      ClassDef(WindowFinder, 0);
    };
  }  // namespace tools
}  // namespace ca

#endif  // CaToolsWindowsFinder_h
