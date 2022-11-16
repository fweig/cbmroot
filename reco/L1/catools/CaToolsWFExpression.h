/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#ifndef CaToolsWFExpression_h
#define CaToolsWFExpression_h 1

#include "TCut.h"
#include "TString.h"

#include <cassert>
#include <tuple>
#include <vector>

class TTree;
class TH2F;
class TPad;

namespace ca
{
  namespace tools
  {
    /// A helper class for ca::tools::WindowFinder to handle a particular expression (dx vs. x0 etc.) and all related
    /// methods to work with it.
    /// \note DISTANCE (expression, axis) -- dx or dy, signed distance between the extrapolated point and real MC-point
    ///       PARAMETER (expression, axis) -- x0 or y0, a parameter, vs. which the distance is studied
    class WFExpression {
    public:
      static constexpr int kNpars = 1;  /// TMP: number of parameters

      /// Default constructor
      WFExpression() = delete;

      /// Constructor
      /// \param  pChain     A pointer to a tree with MC triplets
      /// \param  exprDist   Expression for distance
      /// \param  exprParam  Expression for parameter
      WFExpression(TTree* pTree, const char* exprDist, const char* exprParam);

      /// Copy constructor
      WFExpression(const WFExpression& other) = delete;

      /// Move constructor
      WFExpression(WFExpression&& other) = delete;

      /// Destructor
      ~WFExpression() = default;

      /// Copy assignment operator
      WFExpression& operator=(const WFExpression& other) = delete;

      /// Move assignment operator
      WFExpression& operator=(WFExpression&& other) = delete;

      /// Calculates parameters
      /// \param   pTree  Pointer to a tree with MC triplets
      /// \return  A tuple:
      ///  - vector of parameters for upper bound
      ///  - vector of parameters for lower bound
      std::tuple<std::vector<float>, std::vector<float>> CalculateParameters();

      /// Sets cut, including information on the station and track finder iteration
      void SetCut(const TCut& cut) { fCut = cut; }

      /// Sets fraction of the events, which can be left out of boundaries
      void SetEps(float eps);

      /// Sets number of slices
      void SetNslices(int nSlices);

      /// Sets number of bins
      /// \param  nBinsDist  Number of bins along the distance axis
      /// \param  nBinsParam Number of bins along the parameter axis
      void SetNbins(int nBinsDist, int nBinsParam);

      /// Sets base pad pointer
      void SetPadBase(TPad* pad);

      /// Sets slices pad pointer
      void SetPadSlices(TPad* pad);

      /// Sets title of the histograms
      void SetTitle(const char* title) { fsTitle = title; }

    private:
      // *****************************
      // ** Private class functions **
      // *****************************

      /// Process slice
      /// \param  iBinMax  Max bin of the parameter axis to start a projection (date from the bin are included)
      /// \return          Tuple: lower bound, upper bound, slice center
      std::tuple<float, float, float> ProcessSlice(int iBinMin, int iBinMax);

      /// Gets window parameterisations assuming there is no dependence from parameter
      void GetConstWindowParams();
      // TODO: use other functions for other window shapes: GetParabWindowParams, GetEllipticWindowParams etc.


      // *********************
      // ** Class variables **
      // *********************
      TTree* fpTree    = nullptr;  ///< Tree to be analyzed
      TH2F* fpHistBase = nullptr;  ///< Base histogram (distance vs. parameter (x0 or y0))

      TString fsExprDist  = "";  ///< Expression along the distance axis
      TString fsExprParam = "";  ///< Expression along the parameter axis

      TCut fCut    = "";      ///< Cut used to draw and expression
      int fNslices = 8;       ///< Number of slices along the parameter axis
      float fEps   = 0.0005;  ///< A fraction of triplets, which can be lost


      std::vector<float> fvUpSBoundaries = std::vector<float>(fNslices);  ///< Upper boundaries for diff. slices
      std::vector<float> fvLoSBoundaries = std::vector<float>(fNslices);  ///< Lower boundaries for diff. slices
      std::vector<float> fvSCenters      = std::vector<float>(fNslices);  ///< Slice centers
      std::vector<float> fvUpParams      = std::vector<float>(kNpars);    ///< Parameters for max
      std::vector<float> fvLoParams      = std::vector<float>(kNpars);    ///< Parameters for min

      // ----- Plotting options
      int fNbinsParam   = 400;      ///< Number of bins along the parameter axis
      int fNbinsDist    = 400;      ///< Number of bins along the distance axis
      TString fsTitle   = "";       ///< Title of expression
      TString fsName    = "";       ///< Name of the expression (expr + cut, must be unique!!, TODO: make a check)
      TPad* fpPadBase   = nullptr;  ///< Pointer to a pad for base histogram
      TPad* fpPadSlices = nullptr;  ///< Pointer to a pad for slices
    };
  }  // namespace tools
}  // namespace ca

#endif  // CaToolsWFExpression_h
