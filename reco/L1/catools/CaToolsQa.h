/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsQa.h
/// \brief  Tracking performance class (header)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsQa_h
#define CaToolsQa_h 1

#include "L1Vector.h"

class L1Parameters;
class TDirectory;
class TH1;
class TH1F;
class TH2F;
class TProfile;

namespace ca::tools
{
  class MCData;

  /// Class ca::tools::Qa defines CA tracking internal performance measurement
  ///
  class Qa {
  public:
    // *****************************************
    // **     Constructors and destructor     **
    // *****************************************

    /// Default constructor
    Qa() = default;

    /// Destructor
    ~Qa() = default;

    /// Copy constructor
    Qa(const Qa& other) = delete;

    /// Move constructor
    Qa(Qa&& other) = delete;

    /// Copy assignment operator
    Qa& operator=(const Qa& other) = delete;

    /// Move assignment operator
    Qa& operator=(Qa&& other) = delete;

    /// Initializes histograms
    /// \note should be called in the beginning of the run
    void InitHistograms();

    /// Fills histograms
    /// \note should be called in the end of the run
    void FillHistograms();

    /// Saves histograms to the file
    /// \note Should be called in the end of the run
    void SaveHistograms();

    // ***********************
    // **     Accessors     **
    // ***********************

    /// Sets pointer to MC data object
    void SetMCData(const MCData* pMCData) { fpMCData = pMCData; }

    /// Sets pointer to tracking parameters object
    void SetParameters(const L1Parameters* pParameters) { fpParameters = pParameters; }


  private:
    const L1Parameters* fpParameters = nullptr;  ///< Instance of the tracking core class parameters
    const MCData* fpMCData           = nullptr;  ///< Container for MC information on the event

    // ***********************
    // **    Histograms     **
    // ***********************

    TDirectory* fpHistoDirectory = nullptr;  ///< directory object to store performance histograms

    TH1F* fphRegMcMom = nullptr;
    TH1F* fphAccMCMom = nullptr;

    L1Vector<TH1*> fvpHistoList = {"ca::tools::Qa::fvpHistoList"};  ///< List of histograms for iteration over them
  };
}  // namespace ca::tools

#endif  // CaToolsQa_h
