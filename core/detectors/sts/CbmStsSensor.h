/** @file CbmStsSensor.h
  * @copyright Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsSensor.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 03.05.2013
 **
 ** This class is to replace the CbmStsSensor, at which point it will be
 ** renamed to spell correctly.
 **/

#ifndef CBMSTSSENSOR_H
#define CBMSTSSENSOR_H 1
#include "CbmStsAddress.h"        // for GetElementId, kStsSensor
#include "CbmStsElement.h"        // for CbmStsElement
#include "CbmStsParSensorCond.h"  // for CbmStsParSensorCond

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, UInt_t, Bool_t, kTRUE
#include <TString.h>     // for TString

#include <string>  // for string

class CbmEvent;
class CbmStsCluster;
class CbmStsModule;
class CbmStsParSensor;
class TClonesArray;
class TGeoPhysicalNode;

/** @class CbmStsSensor
 ** @brief Class representing an instance of a sensor in the CBM-STS.
 ** @author V.Friese <v.friese@gsi.de>
 ** @version 2.0
 **
 ** The sensor is the smallest geometric element in the STS setup.
 ** It is the daughter node of a module, which may contain one sensor
 ** or several daisy-chained ones. The sensor class represents
 ** the physical node through its member fNode,
 **/
class CbmStsSensor : public CbmStsElement {

public:
  /** Constructor
     ** @param address Unique element address
     ** @param node    Pointer to geometry node
     ** @param mother  Pointer to mother element (module)
     **/
  CbmStsSensor(UInt_t address = 0, TGeoPhysicalNode* node = nullptr, CbmStsElement* mother = nullptr);


  /** Destructor  **/
  virtual ~CbmStsSensor() {};


  /** @brief Get the address from the sensor name (static)
     ** @param name Name of sensor
     ** @value Unique element address
     **/
  static UInt_t GetAddressFromName(TString name);


  /** @brief Sensor conditions
     ** @return Sensor condition object
     **/
  const CbmStsParSensorCond* GetConditions() const { return fConditions; }


  /** @brief Get mother module
     ** @return  Module the sensor is connected to
     **/
  CbmStsModule* GetModule() const;


  /** Get physical node
  	 ** @return Pointer to TGeoPhysicalNode of sensor
     **/
  TGeoPhysicalNode* GetNode() const { return fNode; }


  /** @brief Sensor parameters
  	 ** @return Sensor parameters object
  	 **/
  const CbmStsParSensor* GetParams() const { return fParams; }


  /** @brief Sensor Id within the module
     ** @return Sensor ID
     **
     ** Several sensor can be connected to a module. This is the
     ** running number, starting wth 0. The sensor with ID 0
     ** is the topmost one.
     **/
  Int_t GetSensorId() const { return CbmStsAddress::GetElementId(fAddress, kStsSensor); }


  /** @brief Set sensor address
     ** @param address STS element address
     **/
  void SetAddress(Int_t address)
  {
    fAddress = address;
    fName    = CbmStsElement::ConstructName(address, kStsSensor);
  }


  /** @brief Set the sensor conditions
     ** @param conditions    Sensor condition object
     **
     ** The operating conditions are e.g. temperature, voltages
     ** and capacitances.
     **/
  void SetConditions(const CbmStsParSensorCond* conditions)
  {
    if (!fConditions) fConditions = new CbmStsParSensorCond(*conditions);
    else
      *fConditions = *conditions;
  }


  /** @brief Set the sensor parameters
     ** @param conditions    Sensor parameter object
     **
     ** The internal sensor parameters are e.g. strip pitch,
     ** stereo angle, and other constructional quantities, which
     ** are not described on the geometry level.
     **/
  void SetParameters(const CbmStsParSensor* par) { fParams = par; }


  /** @brief Set the physical node
     ** @param node  Pointer to associated TGeoPhysicalNode object
     **/
  void SetNode(TGeoPhysicalNode* node) { fNode = node; }


  /** @brief String output **/
  virtual std::string ToString() const;


protected:
  const CbmStsParSensor* fParams   = nullptr;  ///< Sensor parameters
  CbmStsParSensorCond* fConditions = nullptr;  ///< Operating conditions

  /** @brief Copy constructor (disabled) **/
  CbmStsSensor(const CbmStsSensor&) = delete;

  /** @brief Copy assignment constructor (disabled) **/
  CbmStsSensor& operator=(const CbmStsSensor&) = delete;


  ClassDef(CbmStsSensor, 2);
};


#endif
