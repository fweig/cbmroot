/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsWindowFinder.h
/// \brief  Framework for CA tracking hit-finder window estimation from MC (header)
/// \since  14.10.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsWindowFinder_h
#define CaToolsWindowFinder_h 1

#include "TCut.h"
#include "TObject.h"

#include <array>
#include <vector>

#include "L1CAIteration.h"

// TODO: Replace tmp asserts with exceptions

class TChain;
class L1SearchWindow;
class TPad;
class TCanvas;

namespace ca
{
  namespace tools
  {
    /// Enumeration to handle processed expressions
    enum EExpr
    {
      kDxVsX0,
      kDxVsY0,
      kDyVsX0,
      kDyVsY0,
      kEND
    };

    /// TODO: ... write an instruction ...
    class WindowFinder : public TObject {
    public:
      // TODO: TEMPORARY CONSTANT EXPRESSIONS (TO BE MOVED TO A SEPARATE HEADER)
      static constexpr const char* kTreeName = "t";  ///< Name of the input MC triplets tree

    public:
      /// Default constructor
      WindowFinder();

      /// Destructor
      virtual ~WindowFinder() = default;

      /// Copy and move are forbidden
      WindowFinder(const WindowFinder&) = delete;
      WindowFinder(WindowFinder&&)      = delete;
      WindowFinder& operator=(const WindowFinder&) = delete;
      WindowFinder& operator=(WindowFinder&&) = delete;

      /// Adds an input file with a tree object of MC triplets
      /// \note Multiple file input is possible
      void AddInputFile(const char* filename);

      /// Saves canvases to a set of canvases to pdf
      void DumpCanvasesToPdf(const char* filename = "WFLog") const;

      /// Process a tree (TEST)
      /// \param opt Define options to process:
      /// 'T' - triplets are used instead of doublets
      void Process(Option_t* opt = "");

      /// Reads the iterations from YAML config
      void ReadTrackingIterationsFromYAML(const char* filename);

      /// Sets binning of the dx (dy) distribution
      /// \param nBinsX  Number of bins for the x-axis
      /// \param nBinsY  Number of bins for the y-axis
      void SetBinning(int nBinsX, int nBinsY);

      /// Sets a fraction of triplets (doublets), which can be omitted by the window
      void SetEpsilon(float eps);

      /// Sets additional cut (can be useful to reduce distribution contamination by outliers)
      /// \param  cut  A cut object
      void SetExtraCut(const TCut& cut) { fExtraCut = cut; }

      /// Sets number of slices along the x-axiso
      /// If the number of slices larger then 1, the window bounds will be fitted with a function (...which TODO).
      /// Otherwise, the bounds will be represented with constants (independent from x0 or y0)
      void SetNslices(int nSlices);

      /// Sets name of output file with search windows array
      /// \note Format of output file:
      /// \note <number of parameters> <number of search windows stored> <array of serialized L1SearchWindow objects>
      /// \param  filename  Name of the file
      void SetOutputName(const char* filename) { fsOutputName = filename; }

      /// Define indexes of stations for which windows are needed
      /// \note: A stution
      /// TODO: Get number of stations from a tree ...
      void SetStationIndexes(const std::vector<int>& vStationIndexes);

      /// Sets components of the target center position
      /// \param  x  Position x-component [cm]
      /// \param  y  Position y-component [cm]
      /// \param  z  Position z-component [cm]
      void SetTarget(double x, double y, double z);


    private:
      /// Creates a search window for a selected station and iteration
      L1SearchWindow CreateSW(int iStation, const L1CAIteration& caIter);

      /// Returns expression for dx or dy to be drawn in a tree
      const char* GetDistExpr(EExpr expr) const;

      /// Gets a cut for doublets/triplets defined by station and a tracking iteration
      /// \param  iStation  Global index of an active station
      /// \param  caIter    CA track finder iteration object
      TCut GetTrackSelectionCut(int iStation, const L1CAIteration& caIter) const;

      /// Prints information on the dx and dy expression as well as used cuts on the pad
      /// \param  pPad      A pad to print the information
      /// \param  iStation  Global index of an active station
      /// \param  caIter    CA track finder iteration object
      void PrintCaseInformation(TPad* pPad, int iStation, const L1CAIteration& caIter) const;


      // *********************
      // ** Class variables **
      // *********************
      int fNparams = 1;  ///< number of parameters of the searching window

      std::string fsOutputName = "SearchWindows.dat";  ///< Name for output file with estimated search windows

      // ----- Input parameters (iterations and stations)
      std::vector<L1CAIteration> fvCaIters = {};   ///< Tracking iterations
      std::vector<int> fvStationIndexes    = {};   ///< Global indexes of active stations to find the windows
      std::array<double, 3> fTargetPos     = {0};  ///< Target position {x, y, z} [cm]

      TCut fExtraCut = TCut("");  ///< Optional cut on the triplets/doublets

      // Window extraction settings
      int fNbinsX  = 400;    ///< Number of bins for the X axis of the distribution
      int fNbinsY  = 400;    ///< Number of bins for the Y axis of the distribution
      int fNslices = 1;      ///< Number of slices along the X axis
      float fEps   = 0.001;  ///< Fraction of triplets (doublets), which can be omitted

      TChain* fpMcTripletsTree = nullptr;  ///< Chain of trees containing MC triplets, generated in CbmL1 Performance

      std::vector<TCanvas*> fvpCanvases;  // Vector of pointer to cavnases

      ClassDef(WindowFinder, 0);
    };
  }  // namespace tools
}  // namespace ca

#endif  // CaToolsWindowsFinder_h
