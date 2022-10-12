/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CbmCaPerformance.h
/// \brief  CA Tracking performance interface for CBM (header)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CbmCaPerformance_h
#define CbmCaPerformance_h 1

#include "CaToolsMcDataManager.h"
#include "CaToolsPerformance.h"


class CbmEvent;
class CbmMCDataObject;
class CbmMCDataArray;
class CbmMCEventList;
enum class L1DetectorID;


/// Class CbmCaPerformcance is an interface to communicate between
class CbmCaPerformance {
public:
  // *****************************************
  // **     Constructors and destructor     **
  // *****************************************

  /// Default constructor
  CbmCaPerformance() = default;

  /// Destructor
  ~CbmCaPerformance() = default;

  /// Copy constructor
  CbmCaPerformance(const CbmCaPerformance&) = delete;

  /// Move constructor
  CbmCaPerformance(CbmCaPerformance&&) = delete;

  /// Copy assignment operator
  CbmCaPerformance& operator=(const CbmCaPerformance&) = delete;

  /// Move assignment operator
  CbmCaPerformance& operator=(CbmCaPerformance&&) = delete;


  // *****************************************
  // **     Action definition functions     **
  // *****************************************

  /// Defines performance action in the beginning of the run
  /// \return Success flag
  bool Init();

  /// Defines performance action in each event or timeslice
  /// \param  pEvent Pointer to a current CbmEvent
  void ProcessEvent(CbmEvent* pEvent);

  /// Defines performance action in the end of the run
  void Finish();


  // ***********************
  // **     Accessors     **
  // ***********************

  /// Registers pointer to the tracking parameters object
  void SetParameters(const L1Parameters* pParameters) { fpParameters = pParameters; }

  /// Sets used detector subsystems
  /// \param  detID  Id of detector
  /// \param  flag   Flag: true - detector is used
  void SetDetector(L1DetectorID detID, bool flag);


private:
  // *******************************
  // **     Utility functions     **
  // *******************************

  /// Checks class initialization. Throws std::logic_error, if initialization is incomplete at initialization call
  void CheckInit() const;


  // *******************************
  // **     Utility variables     **
  // *******************************

  ca::tools::Performance fPerformance     = {};       ///< Instance of the internal performance object
  ca::tools::McDataManager fMcDataManager = {};       ///< Instance of the MC data manager
  const L1Parameters* fpParameters        = nullptr;  ///< Pointer to tracking parameters object

  bool fbUseMvd  = false;
  bool fbUseSts  = false;
  bool fbUseMuch = false;
  bool fbUseTrd  = false;
  bool fbUseTof  = false;


  // *********************************
  // **     Input data branches     **
  // *********************************

  // Mc-event
  //  const CbmMCEventList* fpEventList      = nullptr;  ///< MC event list
  //  const CbmMCDataObject* fpMcEventHeader = nullptr;  ///< MC event header
  //  const CbmMCDataArray* fpMcTracks       = nullptr;  ///< MC tracks

  // Mc-points
  const CbmMCDataArray* fpMvdPoints  = nullptr;  ///< MVD MC-points container
  const CbmMCDataArray* fpStsPoints  = nullptr;  ///< STS MC-points container
  const CbmMCDataArray* fpMuchPoints = nullptr;  ///< MuCh MC-points container
  const CbmMCDataArray* fpTrdPoints  = nullptr;  ///< TRD MC-points container
  const CbmMCDataArray* fpTofPoints  = nullptr;  ///< TOF MC-points container


  // Matching information
};

#endif  // CbmCaPerformance_h
