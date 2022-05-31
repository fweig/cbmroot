/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackerDetInitializer.h
 * @brief  FairTask for the tracker detector interfaces initialization (declaration)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmTrackerDetInitializer_h
#define CbmTrackerDetInitializer_h 1

class FairTask;

/// Class CbmTrackerDetInitializer implements a task for the tracking detector interfaces initialization.
/// The tracking detector interfaces are added as subtasks. The CbmTrackerDetInitializer class instance is to
/// be added as a task to the FairRunAna instance prior to the CbmL1 task.
///
class CbmTrackerDetInitializer: public FairTask {
public:
  /// Default constructor: only STS is initialized (TODO: try delete)
  CbmTrackerDetInitializer();
  
  /// Destructor 
  ~CbmTrackerDetInitializer();
  
  /// Returns a pointer to the class instance
  static CbmTrackerDetInitializer* Instance() { return fpInstance; }

  // Copy and move of the instance is prohibited
  CbmTrackerDetInitializer(const CbmTrackerDetInitializer& ) = delete;
  CbmTrackerDetInitializer(CbmTrackerDetInitializer&&) = delete;
  CbmTrackerDetInitializer& operator=(const CbmTrackerDetInitializer&) = delete;
  CbmTrackerDetInitializer& operator=(CbmTrackerDetInitializer&&) = delete;

private:
  static CbmTrackerDetInitializer* fpInstance;  ///< Instance of the class

  ClassDef(CbmTrackerDetInitializer, 0);
};

#endif // bmTrackerDetInitializer_h
