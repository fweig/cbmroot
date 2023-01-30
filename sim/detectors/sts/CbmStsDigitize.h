/* Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsDigitize.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.05.2014
 **/

#ifndef CBMSTSDIGITIZE_H
#define CBMSTSDIGITIZE_H 1

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


/** @class CbmStsDigitize
 ** @brief Task class for simulating the detector response of the STS
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 23.05.2014
 ** @version 2.0
 **
 ** The STS digitiser task reads CbmStsPoint from the input and produces
 ** objects of type CbmStsDigi. The StsPoints are distributed to the
 ** respective sensors, where the analogue response is calculated. This
 ** is buffered and digitised by the connected module.
 ** The digitiser task triggers the readout of each module after the end
 ** of each call to Exec(), i.e. after processing one input MC event. All
 ** buffered data prior to the MC time of the current event are read out
 ** and stored in the output.
 **/
class CbmStsDigitize : public CbmDigitize<CbmStsDigi> {

public:
  /** Constructor **/
  CbmStsDigitize();


  /** Destructor **/
  virtual ~CbmStsDigitize();


  /** Create a digi and send it for further processing
   ** @param address   Unique channel address
   ** @param time      Absolute time [ns]
   ** @param adc       Digitised charge [ADC channels]
   ** @param match    MC Match object
   **/
  void CreateDigi(Int_t address, UShort_t channel, Long64_t time, UShort_t adc, const CbmMatch& match);


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


  /** Get number of signals front side **/
  Int_t GetNofSignalsF() const { return fNofSignalsF; }


  /** Get number of signals back side **/
  Int_t GetNofSignalsB() const { return fNofSignalsB; }


  /** Initialise the STS setup and the parameters **/
  void InitSetup();


  /** Re-initialisation **/
  virtual InitStatus ReInit();


  /** @brief Set individual module parameters
   ** @param parMap Map of module addresses and corresponding module parameters
   **
   ** These parameters will be applied to individual modules after the global defaults are set.
   ** TODO: Include this with the new parameter scheme
   **/
  /*
  void SetModuleParameterMap(std::map<Int_t, CbmStsDigitizeParameters*> parMap) {
    fModuleParameterMap = parMap;
  }
  */


  /** @brief Set the global ASIC parameters
   ** @param nChannels        Number of readout channels
   ** @param nAdc             Number of ADC channels
   ** @param dynRange         Dynamic range [e]
   ** @param threshold        Threshold [e]
   ** @param timeResolution   Time resolution [ns]
   ** @param deadTime         Channel dead time [ns]
   ** @param noise            Noise RMS [e]
   ** @param zeroNoiseRate    Zero-threshold noise rate [1/ns]
   **
   ** These parameters will be applied to all ASICS in all modules.
   **/
  void SetGlobalAsicParams(UShort_t nChannels, UShort_t nAdc, Double_t dynRange, Double_t threshold,
                           Double_t timeResolution, Double_t deadTime, Double_t noise, Double_t zeroNoiseRate);


  /** @brief Set global fraction of dead channels
   ** @param fraction Fraction of dead channels
   **
   ** If this number is different from zero, in each ASIC a number of
   ** channels corresponding to this fraction are deactivated.
   **/
  void SetGlobalFracDeadChannels(Double_t fraction) { fUserFracDeadChan = fraction; }


  /** @brief Set the global module parameters
   ** @param nChannels             Number of readout channels
   ** @param nAsicChannels         Number of readout channels per ASIC
   **
   ** These parameters will be applied to all modules.
   **/
  void SetGlobalModuleParams(UInt_t nChannels, UInt_t nAsicChannels);


  /** @brief Set the global sensor conditions
   ** @param vDep        Full-depletion voltage [V]
   ** @param vBias       Bias voltage [V]
   ** @param temperature Temperature [K]
   ** @param cCoupling   Coupling capacitance [pF]
   ** @param cInterstrip Inter-strip capacitance [pF]
   **
   ** These parameters will be applied to all sensors when no
   ** condition file is specified.
   **/
  void SetGlobalSensorConditions(Double_t vDep, Double_t vBias, Double_t temperature, Double_t cCoupling,
                                 Double_t cInterstrip);


  /** @brief Set the file name with module parameters
   ** @param fileName  File name with module parameters
   **
   ** The format of the file must comply with
   ** CbmStsSetup::ReadModuleParameters(const char*)
   **/
  void SetModuleParameterFile(const char* fileName);


  /** Set physics processes
   ** @param eLossModel       Energy loss model
   ** @param useLorentzShift  If kTRUE, activate Lorentz shift
   ** @param useDiffusion     If kTRUE, activate diffusion
   ** @param useCrossTalk     If kTRUE, activate cross talk
   **
   ** Changing the physics flags is only allowed before Init() is called.
   **/
  void SetProcesses(CbmStsELoss eLossModel, Bool_t useLorentzShift = kTRUE, Bool_t useDiffusion = kTRUE,
                    Bool_t useCrossTalk = kTRUE);


  /** @brief Set the file name with sensor conditions
   ** @param fileName  File name with sensor conditions
   **
   ** The format of the file must comply with
   ** CbmStsSetup::ReadSensorConditions(const char*)
   **/
  void SetSensorConditionFile(const char* fileName);


  /** @brief Set the file name with sensor parameters
   ** @param fileName  File name with sensor parameters
   **
   ** The format of the file must comply with
   ** CbmStsSetup::ReadSensorParameters(const char*)
   **/
  void SetSensorParameterFile(const char* fileName);


  /** Set the sensor strip pitch
   ** @param  pitch  Strip pitch [cm]
   **
   ** The internal sensor parameters like pitch, stereo angle etc. are normally taken
   ** from a sensor database. This method allows to override the value for the strip
   ** pitch defined there, in order to easily test different sensor layout options
   ** without defining new types in the database. It has effect only for strip sensor types.
   ** The specified strip pitch will be applied for all sensors in the setup.
   **
   ** TODO: Functionality still used? Should be included in new parameter scheme,
   ** then. Through CbmStsParSensor.
   **/
  /*
  void SetSensorStripPitch(Double_t pitch) {
    fDigiPar->SetStripPitch(pitch);
  }
  */

  /** @brief Discard processing of secondary tracks
   ** @param flag  kTRUE if secondaries shall be discarded
   **
   ** This flag enables the user to suppress the digitisation of
   ** StsPoints from secondary tracks for debug purposes. By default,
   ** points from all tracks are processed.
   **/
  void UseOnlyPrimaries(Bool_t flag = kTRUE);


private:
  Bool_t fIsInitialised;  ///< kTRUE if Init() was called

  //std::map<Int_t, CbmStsDigitizeParameters*> fModuleParameterMap; ///< Individual module parameter map
  CbmStsSetup* fSetup;                               //! STS setup interface
  CbmStsSimSensorFactory* fSensorFactory = nullptr;  //! Sensor factory
  TClonesArray* fPoints;                             ///< Input array of CbmStsPoint
  TClonesArray* fTracks;                             ///< Input array of CbmMCTrack
  TStopwatch fTimer;                                 ///< ROOT timer

  /** Map of modules. Key is the address. **/
  std::map<UInt_t, CbmStsSimModule*> fModules {};

  /** Map of sensors. Key is the address. **/
  std::map<UInt_t, std::unique_ptr<CbmStsSimSensor>> fSensors {};

  // --- Global user-defined parameter settings
  CbmStsParSim* fUserParSim         = nullptr;  ///< Settings for simulation
  CbmStsParAsic* fUserParAsic       = nullptr;  ///< User defined, global
  CbmStsParModule* fUserParModule   = nullptr;  ///< User defined, global
  CbmStsParSensor* fUserParSensor   = nullptr;  ///< User defined, global
  CbmStsParSensorCond* fUserParCond = nullptr;  ///< User defined, global
  Double_t fUserDinactive           = 0.;       ///< Size of inactive sensor border [cm]
  Double_t fUserFracDeadChan        = 0.;       ///< Fraction of inactive ASIC channels

  // --- Module and sensor parameters for runtime DB output
  CbmStsParSim* fParSim               = nullptr;  ///< Simulation settings
  CbmStsParSetModule* fParSetModule   = nullptr;  ///< Module parameter
  CbmStsParSetSensor* fParSetSensor   = nullptr;  ///< Sensor parameters
  CbmStsParSetSensorCond* fParSetCond = nullptr;  ///< Sensor conditions

  // --- Default sensor parameters (apply to SensorDssdStereo)
  Double_t fSensorDinact;   ///< Size of inactive border [cm]
  Double_t fSensorPitch;    ///< Strip pitch [cm]
  Double_t fSensorStereoF;  ///< Stereo angle front side [degrees]
  Double_t fSensorStereoB;  ///< Stereo angle back side [degrees]

  // --- Input parameter files
  TString fSensorParameterFile;  ///< File with sensor parameters
  TString fSensorConditionFile;  ///< File with sensor conditions
  TString fModuleParameterFile;  ///< File with module parameters

  // --- Time of last processed StsPoint (for stream mode)
  Double_t fTimePointLast;

  // --- Digi times (for stream mode, in each step)
  Double_t fTimeDigiFirst;  ///< Time of first digi sent to DAQ
  Double_t fTimeDigiLast;   ///< Time of last digi sent to DAQ

  // --- Event counters
  Int_t fNofPointsProc = 0;  ///< Number of processed points
  Int_t fNofPointsIgno = 0;  ///< Number of ignored points
  Int_t fNofSignalsF   = 0;  ///< Number of signals on front side
  Int_t fNofSignalsB   = 0;  ///< Number of signals on back side
  Int_t fNofDigis      = 0;  ///< Number of created digis in Exec

  // --- Run counters
  Int_t fNofEvents           = 0;   ///< Total number of processed events
  Double_t fNofPointsProcTot = 0;   ///< Total number of processed points
  Double_t fNofPointsIgnoTot = 0;   ///< Total number of ignored points
  Double_t fNofSignalsFTot   = 0;   ///< Number of signals on front side
  Double_t fNofSignalsBTot   = 0;   ///< Number of signals on back side
  Double_t fNofDigisTot      = 0;   ///< Total number of digis created
  Double_t fNofNoiseTot      = 0;   ///< Total number of noise digis
  Double_t fTimeTot          = 0.;  ///< Total execution time

  // --- List of inactive channels
  // --- We do not use the base class here since STS channels are identified not only by
  // --- CbmAddress like in all other detectors, but by address plus channel number.
  std::map<Int_t, std::set<UShort_t>> fInactiveChannelsSts = {};


  /** @brief Number of signals in the analogue buffers
   ** @value nSignals  Sum of number of signals in all modules
   **/
  Int_t BufferSize() const;


  /** @brief Status of the analogue buffers
   ** @param[out] nSignals  Sum of number of signals in alll modules
   ** @value String output
   **/
  std::string BufferStatus() const;


  /** End-of-run action **/
  virtual void Finish();


  /** Initialisation **/
  virtual InitStatus Init();


  /** @brief Instantiate modules
   ** @return Number of instantiated modules
   **/
  UInt_t InitModules();


  /** @brief Initialise the parameters **/
  void InitParams();


  /** @brief Instantiate sensors
   ** @return Number of instantiated sensors
   **/
  UInt_t InitSensors();


  /** @brief Test if the channel of a digi object is set active
   ** @param address CbmStdAddress of module
   ** @param channel Channel number in module
   ** @return .true. if the respective channel is active
   **
   ** We do not use the base class method IsChannelActive(), because unlike for the other detector systems,
   ** an STS channel is not identified by the address only, but by address plus channel number.
   **/
  bool IsChannelActiveSts(Int_t address, UShort_t channel);


  /** Process the analog buffers of all modules
   ** @param readoutTime  Time of readout [ns]
   **/
  void ProcessAnalogBuffers(Double_t readoutTime);


  /** Process StsPoints from MCEvent **/
  void ProcessMCEvent();


  /** Process one MCPoint
   ** @param point  Pointer to CbmStsPoint to be processed
   ** @param link   Link to MCPoint
   **/
  void ProcessPoint(const CbmStsPoint* point, Double_t eventTime, const CbmLink& link);


  /** @brief Read the list of inactive channels from file
   ** @param fileName   File name
   ** @return Number of channels read from file, success of file reading
   **
   ** This re-implements the respective method from the base class by reading not only the address,
   ** but also the channel number from file. The file must contain one line for each channel
   ** containing the module address and the channel number, separated by a blank. Comments can
   ** follow after the channel number, if separated by a blank.
   **
   ** Reading from the file will stop when a read error occurs. In that case, or when the file
   ** could not be opened at all, the success flag will be .false.
   **/
  std::pair<size_t, bool> ReadInactiveChannels();


  /** @brief Reset event counters **/
  void ResetCounters();


  /** @brief Set global default parameters
   **
   ** The global module and sensor parameters will be applied for all
   ** modules and sensors if no element-specific parameters are applied.
   ** Default values for the global parameters are hard-coded here.
   ** They can be changed from the macro level by the appropriate
   ** methods (e.g. SetGlobalSensorConditions).
   **/
  void SetGlobalDefaults();


  /** Prevent usage of copy constructor and assignment operator **/
  CbmStsDigitize(const CbmStsDigitize&);
  CbmStsDigitize operator=(const CbmStsDigitize&);


  ClassDef(CbmStsDigitize, 5);
};

#endif
