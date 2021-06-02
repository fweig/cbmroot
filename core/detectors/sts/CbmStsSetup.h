/** @file CbmStsSetup.h
  * @copyright Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsSetup.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 27.05.2013
 **/


#ifndef CBMSTSSETUP_H
#define CBMSTSSETUP_H 1

#include "CbmStsElement.h"  // for CbmStsElement
#include "CbmStsSensor.h"   // for CbmStsSensor

#include <Logger.h>  // for LOG

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Bool_t, UInt_t, UChar_t

#include <map>      // for map, __map_const_iterator, operator!=
#include <set>      // for set
#include <string>   // for string
#include <utility>  // for pair
#include <vector>   // for vector

class CbmStsModule;
class CbmStsParSetModule;
class CbmStsParSetSensor;
class CbmStsParSetSensorCond;
class CbmStsStation;
class TGeoManager;

/** @class CbmStsSetup
 ** @brief Class representing the top level of the STS setup
 ** @author V.Friese <v.friese@gsi.de>
 ** @version 1.0
 **
 ** The CbmStsSetup is a singleton class and represents the
 ** interface to the CBM geometry and its elements.
 **/
class CbmStsSetup : public CbmStsElement {

public:
  /** Destructor **/
  virtual ~CbmStsSetup() {};


  /** Get an STS element by address
     ** @param address  Unique element address (see CbmStsAddress)
     ** @param level    Element level (EStsElementLevel)
     ** @return Pointer to STS element
     **/
  CbmStsElement* GetElement(Int_t address, Int_t level);


  /** Get the name of an element level
     ** @param level    Element level (EStsElementLevel)
     ** @return  Name of element level (station, ladder, etc.)
     **/
  const char* GetLevelName(Int_t level) const;


  /** @brief Get a module from the module array.
     ** @param  index  Index of module in the vector
     ** @return  Pointer to module
     **
     ** For convenient loops over all modules.
     ** Note that the index of the module is meaningless.
     **/
  CbmStsModule* GetModule(Int_t index) const { return fModuleVector.at(index); }


  /** @brief Get a sensor from the sensor array.
     ** @param  index  Index of sensor in the vector
     ** @return  Pointer to sensor
     **
     ** For convenient loops over all sensors.
     ** Note that the index of the sensor is meaningless.
     **/
  CbmStsSensor* GetSensor(Int_t index) const { return fSensorVector.at(index); }


  /** Get number of modules in setup **/
  Int_t GetNofModules() const { return fModules.size(); }


  /** Get number of sensors in setup **/
  Int_t GetNofSensors() const { return fSensors.size(); }


  /** Get number of stations **/
  Int_t GetNofStations() const { return fStations.size(); }


  /** Get a station
     ** @param stationId  Station number
     ** @value Pointer to station object. nullptr if not present.
     **/
  CbmStsStation* GetStation(Int_t stationId) const
  {
    if (fStations.find(stationId) == fStations.end()) return nullptr;
    return fStations.at(stationId);
  }


  /** Get station number from address
     ** @param address  Unique detector address
     ** @value Station number
     **/
  Int_t GetStationNumber(Int_t address);


  /** @brief Initialise the setup
     ** @param geometryFile         Name of file with STS geometry
     ** @return  kTRUE if successfully initialised
     **
     ** The setup will be initialised from the STS geometry, either
     ** taken from the TGeoManager or, if specified, read from a
     ** geometry file.
     **/
  Bool_t Init(const char* geometryFile = nullptr);


  /** @brief Initialisation status for sensor parameters
     ** @return kTRUE if setup is initialised
     **/
  Bool_t IsInit() const { return fIsInitialised; }


  /** @brief Initialisation status for module parameters
     ** @return kTRUE if modules are initialised
     **/
  Bool_t IsModuleParsInit() const { return fIsModuleParsInit; }


  /** @brief Initialisation status for sensor conditions
     ** @return kTRUE if sensor conditions are initialised
     **/
  Bool_t IsSensorCondInit() const { return fIsSensorParsInit; }


  /** @brief Initialisation status for sensor parameters
     ** @return kTRUE if sensor parameters are initialised
     **/
  Bool_t IsSensorParsInit() const { return fIsSensorParsInit; }


  /** Static instance of CbmStsSetup **/
  static CbmStsSetup* Instance();


  /** Print list of modules with parameters **/
  void ListModules() const;


  /** Print list of sensors with parameters **/
  void ListSensors() const
  {
    for (auto it = fSensors.begin(); it != fSensors.end(); it++)
      LOG(info) << it->second->ToString();
  }


  /** @brief Set module parameters from parameter container
     ** @param Parameter container for modules
     ** @return Number of module the paraneters of which were set
     **/
  UInt_t SetModuleParameters(CbmStsParSetModule* modulePars);


  /** @brief Set sensor conditions from parameter container
     ** @param Parameter container for sensor conditions
     ** @return Number of sensors the conditions of which were set
     **/
  UInt_t SetSensorConditions(CbmStsParSetSensorCond* conds);


  /** @brief Set sensor parameters from parameter container
     ** @param Parameter container for sensor parameters
     ** @return Number of sensors the parameters of which were set
     **/
  UInt_t SetSensorParameters(CbmStsParSetSensor* parSet);


private:
  /** @brief Default constructor  **/
  CbmStsSetup();


  /** @brief Create station objects **/
  Int_t CreateStations();


  /** @brief Read the geometry from TGeoManager
     ** @param geoManager  Instance of TGeoManager
     ** @return kTRUE if successfully read; kFALSE else
     **
     ** The ROOT geometry is browsed for elements of the setup,
     ** which are then instantiated and connected to the respective
     ** physical node.
     **/
  Bool_t ReadGeometry(TGeoManager* geoManager);


  /** @brief Read the geometry from a ROOT geometry file
     ** @param fileName  Name of geometry file
     ** @return kTRUE if successfully read; kFALSE else
     **
     ** The ROOT geometry is browsed for elements of the setup,
     ** which are then instantiated and connected to the respective
     ** physical node.
     **/
  Bool_t ReadGeometry(const char* fileName);


  /** @brief Copy constructor (disabled) **/
  CbmStsSetup(const CbmStsSetup&) = delete;


  /** @brief Assignment operator (disabled) **/
  CbmStsSetup operator=(const CbmStsSetup&) = delete;


private:
  static CbmStsSetup* fgInstance;     ///< Static instance of this class
  Bool_t fIsInitialised    = kFALSE;  ///< All parameter containers set
  Bool_t fIsModuleParsInit = kFALSE;  ///< Module parameters set
  Bool_t fIsSensorParsInit = kFALSE;  ///< Sensor parameters set
  Bool_t fIsSensorCondInit = kFALSE;  ///< Sensor conditions set
  Bool_t fHasStations      = kFALSE;  ///< Legacy with stations instead of units

  // --- Map of sensors. Key is address.
  std::map<Int_t, CbmStsSensor*> fSensors;

  // --- Map of modules. Key is address.
  std::map<Int_t, CbmStsModule*> fModules;

  // --- Vector of modules. For convenient loops.
  std::vector<CbmStsModule*> fModuleVector;

  // --- Vector of sensors. For convenient loops.
  std::vector<CbmStsSensor*> fSensorVector;

  // --- Map of stations. Key is station number.
  // --- Stations are a special case needed for reconstruction;
  // --- they are not elements in the setup.
  std::map<Int_t, CbmStsStation*> fStations;  //!


  ClassDef(CbmStsSetup, 3);
};

#endif /* CBMSTSSETUP_H */
