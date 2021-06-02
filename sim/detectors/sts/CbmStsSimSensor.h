/** @file CbmStsSimSensor.h
  * @copyright Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsSimSensor.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.03.2020
 **/

#ifndef CBMSTSSIMSENSOR_H
#define CBMSTSSIMSENSOR_H 1


#include <TObject.h>
#include <TString.h>

class CbmLink;
class CbmStsElement;
class CbmStsParSensorCond;
class CbmStsParSim;
class CbmStsPoint;
class CbmStsSensorPoint;
class CbmStsSimModule;


/** @class CbmStsSimSensor
 ** @brief Class for the simulation of a sensor in the CBM-STS.
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 16.03.2020
 **
 ** The interface to the simulation is the method ProcessPoint(), which
 ** performs the coordinate transformation from the global system to the
 ** sensor system, having the sensor midpoint as origin. The analog response
 ** has to be  implemented in the pure virtual method CalulateResponse().
 **/
class CbmStsSimSensor : public TObject {

public:
  /** @brief Standard constructor
     ** @param setupSensor Pointer to sensor element in CbmStsSetup
     **/
  CbmStsSimSensor(CbmStsElement* element = nullptr);


  /** @brief Copy constructor (disabled) **/
  CbmStsSimSensor(const CbmStsSimSensor&) = delete;


  /** @brief Assignment operator (disabled) **/
  CbmStsSimSensor& operator=(const CbmStsSimSensor&) = delete;


  /** @brief Destructor  **/
  virtual ~CbmStsSimSensor() {};


  /** @brief Get the address from the sensor name (static)
     ** @param name Name of sensor
     ** @value Unique element address
     **/
  static UInt_t GetAddressFromName(TString name);


  /** @brief Sensor conditions
     ** @return Sensor condition object
     **/
  const CbmStsParSensorCond* GetConditions() const { return fConditions; }


  /** @brief Current link object
     ** @return Current link object (to CbmStsPoint)
     **/
  CbmLink* GetCurrentLink() const { return fCurrentLink; }


  /** @brief Corresponding element in STS setup
     ** @return Sensor element
     **/
  CbmStsElement* GetElement() const { return fElement; }


  /** @brief Simulation module
     ** @return Corresponding simulation module
     **/
  CbmStsSimModule* GetModule() const { return fModule; }


  /** @brief Initialise the sensor, if needed
     ** @return kTRUE is successfully initialised
     **
     ** The implementation depends on the concrete sensor class.
     **/
  virtual Bool_t Init() { return kTRUE; }


  /** @brief Sensor ID
     ** @return Sensor number with module
    **/
  Int_t GetSensorId() const;


  /** @brief Process one MC Point
     ** @param point  Pointer to CbmStsPoint object
     ** @return  Status variable, depends on sensor type
     **
     ** The point coordinates are converted into the internal coordinate
     ** system. The appropriate analogue response is then calculated
     ** with the pure virtual method CalculateResponse.
     **/
  Int_t ProcessPoint(const CbmStsPoint* point, Double_t eventTime = 0., CbmLink* link = NULL);


  /** Set the sensor conditions
     ** @param conditions    Pointer to conditions parameters
     **/
  void SetConditions(const CbmStsParSensorCond* conditions) { fConditions = conditions; }


  /** @brief Set the corresponding STS setup element
     ** @param Pointer to setup element
     **/
  void SetElement(CbmStsElement* element) { fElement = element; }


  /** @brief Set the magnetic field in the sensor centre
     ** @param bX  x component of magnetic field [T]
     ** @param bY  y component of magnetic field [T]
     ** @param bZ  z component of magnetic field [T]
     **
     ** The magnetic field is neede for the calculation of the
     ** Lorentz shift of charge carries in the sensor. The field is
     ** approximated to be constant.
     **/
  void SetField(Double_t bX, Double_t bY, Double_t bZ)
  {
    fBx = bX;
    fBy = bY;
    fBz = bZ;
  }


  /** @brief Set the corresponding simulation module
     ** @param module Pointer to module
     **/
  void SetModule(CbmStsSimModule* module) { fModule = module; }


  /** @brief Set the simulation settings
     ** @param Simulation setting parameteres
     **/
  void SetSimSettings(const CbmStsParSim* settings) { fSettings = settings; }


  /** @brief Set the sensor parameters
     ** @param par  Sensor parameter object
     **/
  // void SetParameters(const CbmStsParSensor* par) {
  //   fParameters = par;
  // }


  /** @brief String output **/
  virtual std::string ToString() const = 0;


protected:
  CbmStsElement* fElement       = nullptr;  //! Setup element
  CbmStsSimModule* fModule      = nullptr;  //! Simulation module
  const CbmStsParSim* fSettings = nullptr;  //! Simulation settings
  //const CbmStsParSensor* fParameters = nullptr;     //! Sensor parameters
  const CbmStsParSensorCond* fConditions = nullptr;  //! Operating conditions
  Double_t fBx                           = 0.;       ///< x component of magnetic field in sensor centre
  Double_t fBy                           = 0.;       ///< y component of magnetic field in sensor centre
  Double_t fBz                           = 0.;       ///< z component of magnetic field in sensor centre
  CbmLink* fCurrentLink                  = nullptr;  //! Link to currently processed MCPoint


  /** Perform response simulation for one MC Point
     ** @param point   Pointer to CbmStsSensorPoint with relevant parameters
     ** @return  Status variable, depends on concrete type
     **
     ** Perform the appropriate action for a particle trajectory in the
     ** sensor characterised by the CbmStsSensorPoint object. This is specific
     ** to the sensor type and has to be implemented in the derived class.
     **/
  virtual Int_t CalculateResponse(CbmStsSensorPoint* point) = 0;


  ClassDef(CbmStsSimSensor, 1);
};

#endif
