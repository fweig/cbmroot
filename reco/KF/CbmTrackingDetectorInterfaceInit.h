/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackingDetectorInterfaceInit.h
 * @brief  FairTask for the tracker detector interfaces initialization (declaration)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmTrackingDetectorInterfaceInit_h
#define CbmTrackingDetectorInterfaceInit_h 1

#include "FairTask.h"

class CbmMvdTrackingInterface;
class CbmStsTrackingInterface;
class CbmMuchTrackingInterface;
class CbmTrdTrackingInterface;

/// Class CbmTrackingDetectorInterfaceInit implements a task for the tracking detector interfaces initialization.
/// The tracking detector interfaces are added as subtasks. The CbmTrackingDetectorInterfaceInit class instance is to
/// be added as a task to the FairRunAna instance prior to the CbmL1 task.
///
class CbmTrackingDetectorInterfaceInit : public FairTask {
public:
  /// Default constructor (only sts and mvd will be initialized)
  CbmTrackingDetectorInterfaceInit();

  /// Destructor
  ~CbmTrackingDetectorInterfaceInit();

  /// Returns a pointer to the class instance
  static CbmTrackingDetectorInterfaceInit* Instance() { return fpInstance; }

  // Copy and move of the instance is prohibited
  CbmTrackingDetectorInterfaceInit(const CbmTrackingDetectorInterfaceInit&) = delete;
  CbmTrackingDetectorInterfaceInit(CbmTrackingDetectorInterfaceInit&&)      = delete;
  CbmTrackingDetectorInterfaceInit& operator=(const CbmTrackingDetectorInterfaceInit&) = delete;
  CbmTrackingDetectorInterfaceInit& operator=(CbmTrackingDetectorInterfaceInit&&) = delete;

  // Accessors for the tracker interfaces
  CbmMvdTrackingInterface* GetMvdTrackingInterface() { return fpMvdTrackingInterface; }
  CbmStsTrackingInterface* GetStsTrackingInterface() { return fpStsTrackingInterface; }
  CbmMuchTrackingInterface* GetMuchTrackingInterface() { return fpMuchTrackingInterface; }
  CbmTrdTrackingInterface* GetTrdTrackingInterface() { return fpTrdTrackingInterface; }

private:
  static CbmTrackingDetectorInterfaceInit* fpInstance;  ///< Instance of the class

  CbmMvdTrackingInterface* fpMvdTrackingInterface {nullptr};    ///< Instance of the MVD tracker interface
  CbmStsTrackingInterface* fpStsTrackingInterface {nullptr};    ///< Instance of the STS tracker interface
  CbmMuchTrackingInterface* fpMuchTrackingInterface {nullptr};  ///< Instance of the MuCh tracker interface
  CbmTrdTrackingInterface* fpTrdTrackingInterface {nullptr};    ///< Instance of the TRD tracker interface

  ClassDef(CbmTrackingDetectorInterfaceInit, 0);
};

#endif  // bmTrackerDetInitializer_h
