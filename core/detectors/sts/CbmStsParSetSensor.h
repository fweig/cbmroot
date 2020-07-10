/** @file CbmStsParSetSensor.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 06.04.2020
 **/


#ifndef CBMSTSPARSETSENSOR_H
#define CBMSTSPARSETSENSOR_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for UInt_t, Bool_t, kFALSE, kTRUE

#include <FairParGenericSet.h>  // for FairParGenericSet

#include <map>     // for map
#include <string>  // for string

#include "CbmStsParSensor.h"  // for CbmStsParSensor

class FairParamList;

/** @class CbmStsParSetSensor
 ** @brief Parameters container for CbmStsParSensor
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 06.04.2020
 **
 ** A set of parameters for each STS sensor is administered through
 ** FairRuntimeDb.
 **/
class CbmStsParSetSensor : public FairParGenericSet {

public:
  /** @brief Constructor
     ** @param name  Name of parameter set
     ** @param title Parameter container factory name
     ** @param context  No idea
     **/
  CbmStsParSetSensor(const char* name    = "CbmParSetSensor",
                     const char* title   = "STS parameters",
                     const char* context = "Default");


  /** @brief Copy constructor (disabled) **/
  CbmStsParSetSensor(const CbmStsParSetSensor&) = delete;


  /** @brief Move constructor (disabled) **/
  CbmStsParSetSensor(CbmStsParSetSensor&&) = delete;


  /** @brief Reset all parameters **/
  virtual void clear();


  /** @brief Destructor **/
  virtual ~CbmStsParSetSensor();


  /** @brief Reading parameters from ASCII. Abstract in base class.
     **
     ** An ASCII I/O is not implemented. The method throws an error.
     **/
  virtual Bool_t getParams(FairParamList* parList);


  /** @brief Get condition parameters of a sensor
     ** @param Module address
     ** @return Module parameter object
     **/
  const CbmStsParSensor& GetParSensor(UInt_t address);


  /** @brief Copy assignment operator (disabled) **/
  CbmStsParSetSensor& operator=(const CbmStsParSetSensor&) = delete;


  /** @brief Move assignment operator (disabled) **/
  CbmStsParSetSensor& operator=(CbmStsParSetSensor&&) = delete;


  /** @brief Writing parameters to ASCII. Abstract in base class.
     **
     ** An ASCII I/O is not implemented. The method throws an error.
     **/
  virtual void putParams(FairParamList* parList);


  /** @brief Set global parameters (for all modules)
     ** @param conditions  Module parameter object
     **/
  void SetGlobalPar(const CbmStsParSensor& params) {
    fGlobalParams = params;
    fUseGlobal    = kTRUE;
  }


  /** @brief Set the parameters for a sensor
     ** @param address  Sensor address
     ** @param parSensor parameter object
     **/
  void SetParSensor(UInt_t address, const CbmStsParSensor& par);


  /** @brief Info to string **/
  std::string ToString() const;


private:
  /** @brief Flag for using global parameters **/
  Bool_t fUseGlobal = kFALSE;

  /** @brief Global parameters, used for all modules **/
  CbmStsParSensor fGlobalParams {};

  /** @brief Map of parameters. Key is sensor address. **/
  std::map<UInt_t, CbmStsParSensor> fParams {};


  ClassDef(CbmStsParSetSensor, 1);
};

#endif /* CBMSTSPARSETSENSOR */
