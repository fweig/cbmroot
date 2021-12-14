/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

/** @file CbmStsDigitizePixel.h
 ** @author Sergey Gorbunov
 ** @date 09.12.2021
 **/

#ifndef CbmStsDigitizePixel_H
#define CbmStsDigitizePixel_H 1

#include "CbmDefs.h"
#include "CbmDigitize.h"
#include "CbmMatch.h"
#include "CbmStsDefs.h"
#include "CbmStsDigi.h"
#include "CbmStsPhysics.h"
#include "CbmStsSimModule.h"
#include "CbmStsSimSensor.h"

#include "TStopwatch.h"

#include <map>

class TClonesArray;
class CbmStsPoint;
class CbmStsParAsic;
class CbmStsParModule;
class CbmStsParSensor;
class CbmStsParSensorCond;
class CbmStsParSetModule;
class CbmStsParSetSensor;
class CbmStsParSetSensorCond;
class CbmStsParSim;
class CbmStsSetup;
class CbmStsSimSensorFactory;


/** @class CbmStsDigitizePixel
 **
 ** @brief Task class for simulating the detector response of the experimental STS Pixel setup
 ** @author Sergey Gorbunov
 ** @since 09.12.2021
 **
 ** A digitiser for the experimental STS Pixel detector
 **
 ** To run it, add the following line to run_digi.C macro:
 **
 ** run.SetDigitizer(ECbmModuleId::kSts, new CbmStsDigitizePixel);
 **
 **/
class CbmStsDigitizePixel : public CbmDigitize<CbmStsDigi> {

public:
  /** Constructor **/
  CbmStsDigitizePixel(Double_t pitchXcm = 0.0010, Double_t pitchYcm = 0.0010, Double_t resolutionTns = 5.);


  /** Destructor **/
  virtual ~CbmStsDigitizePixel();


  /** @brief Detector system ID
   ** @return kSts
   **/
  ECbmModuleId GetSystemId() const { return ECbmModuleId::kSts; }

  /**
    * \brief Inherited from FairTask.
    */
  virtual void SetParContainers();


  /** Execution **/
  virtual void Exec(Option_t* opt);


  /** Re-initialisation **/
  virtual InitStatus ReInit();

  /** End-of-run action **/
  virtual void Finish();


  /** Initialisation **/
  virtual InitStatus Init();

  /// set pitch X [cm]
  void SetPitchX(double pitchXcm) { fPitchXcm = pitchXcm; }

  /// set pitch Y [cm]
  void SetPitchY(double pitchYcm) { fPitchYcm = pitchYcm; }

  /// set resolution Time [ns]
  void SetResolutionTime(double resolutionTns) { fResolutionTns = resolutionTns; }

private:
  Bool_t fIsInitialised;  ///< kTRUE if Init() was called

  //std::map<Int_t, CbmStsDigitizeParameters*> fModuleParameterMap; ///< Individual module parameter map
  CbmStsSetup* fSetup;              //! STS setup interface
  TClonesArray* fPoints {nullptr};  ///< Input array of CbmStsPoint

  // --- Module and sensor parameters for runtime DB output
  CbmStsParSim* fParSim               = nullptr;  ///< Simulation settings
  CbmStsParSetModule* fParSetModule   = nullptr;  ///< Module parameter
  CbmStsParSetSensor* fParSetSensor   = nullptr;  ///< Sensor parameters
  CbmStsParSetSensorCond* fParSetCond = nullptr;  ///< Sensor conditions

  // data members

  Double_t fPitchXcm {0.0058};  // pitch in X [cm]
  Double_t fPitchYcm {0.0058};  // pitch in Y [cm]
  Double_t fResolutionTns {5.};  // resolution in time [ns]

  /** @brief Initialise the parameters **/
  void InitParams();

  /** Process StsPoints from MCEvent **/
  void ProcessMCEvent();

  /** Prevent usage of copy constructor and assignment operator **/
  CbmStsDigitizePixel(const CbmStsDigitizePixel&);
  CbmStsDigitizePixel operator=(const CbmStsDigitizePixel&);

  ClassDef(CbmStsDigitizePixel, 0);
};

#endif
