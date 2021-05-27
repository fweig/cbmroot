/** @file CbmStsParSetSensorCond.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 26.03.2020
 **/


#ifndef CBMSTSPARSETSENSORCOND_H
#define CBMSTSPARSETSENSORCOND_H 1

#include "CbmStsParSensorCond.h"  // for CbmStsParSensorCond

#include <FairParGenericSet.h>  // for FairParGenericSet

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, UInt_t, Bool_t, kFALSE

#include <map>     // for map
#include <string>  // for string

class FairParamList;

/** @class CbmStsParSetSensorCond
 ** @brief Parameters container for CbmStsParSensorCond
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 26.03.2020
 **
 ** A set of parameters for each STS sensor is administered through
 ** FairRuntimeDb.
 **/
class CbmStsParSetSensorCond : public FairParGenericSet {

public:
  /** @brief Constructor
     ** @param name  Name of parameter set
     ** @param title Parameter container factory name
     ** @param context  No idea
     **/
  CbmStsParSetSensorCond(const char* name = "CbmStsParSetSensorCond", const char* title = "STS parameters",
                         const char* context = "Default");


  /** @brief Copy constructor (disabled) **/
  CbmStsParSetSensorCond(const CbmStsParSetSensorCond&) = delete;


  /** @brief Move constructor (disabled) **/
  CbmStsParSetSensorCond(CbmStsParSetSensorCond&&) = delete;


  /** @brief Destructor **/
  virtual ~CbmStsParSetSensorCond();


  /** @brief Reset all parameters **/
  virtual void clear();


  /** @brief Reading parameters from ASCII. Abstract in base class.
     **
     ** An ASCII I/O is not implemented. The method throws an error.
     **/
  virtual Bool_t getParams(FairParamList* parList);


  /** @brief Get condition parameters of a sensor
     ** @param Sensor address
     ** @return Condition parameter object
     **/
  const CbmStsParSensorCond& GetParSensor(UInt_t address);


  /** @brief Check for parameter set being filled
     ** @return kTRUE if neither global parameters nor parameter map are set
     **/
  Bool_t IsSet() const { return (fGlobal || !fParams.empty()); }


  /** @brief Copy assignment operator (disabled) **/
  CbmStsParSetSensorCond& operator=(const CbmStsParSetSensorCond&) = delete;


  /** @brief Move assignment operator (disabled) **/
  CbmStsParSetSensorCond& operator=(CbmStsParSetSensorCond&&) = delete;


  /** @brief Writing parameters to ASCII. Abstract in base class.
     **
     ** An ASCII I/O is not implemented. The method throws an error.
     **/
  virtual void putParams(FairParamList* parList);


  /** @brief Read sensor conditions from file
     ** @param fileName  Name of file with sensor conditions
     ** @value Number of sensors the conditions are set for
     **
     ** The file with the conditions is read and the condition parameters
     ** for the sensor are set accordingly. Each sensor in the setup must
     ** show up in the file.
     **
     ** The format is a text file containing for each sensor a line with
     ** sensor_name vDep vBias temperature cCoupling cInterstrip
     ** separated by blanks. Empty lines or lines starting with '#'
     ** (comments) are ignored.
     **
     ** TODO: Is using the RuntimeDb ASCII interface an alternative?
     **/
  UInt_t ReadParams(const char* fileName);


  /** @brief Set global conditions (for all sensors)
     ** @param vDep        Full-depletion voltage [V]
     ** @param vBias       Bias voltage [V]
     ** @param temperature Temperature [K]
     ** @param cCoupling   Coupling capacitance [pF]
     ** @param cInterstrip Inter-strip capacitance [pF]
     **/
  void SetGlobalPar(Double_t vDep, Double_t vBias, Double_t temperature, Double_t cCoupling, Double_t cInterstrip);


  /** @brief Set global conditions (for all sensors)
     ** @param conditions  Sensor condition object
     **/
  void SetGlobalPar(const CbmStsParSensorCond& conditions);


  /** @brief Info to string **/
  std::string ToString();


private:
  /** @brief Initialise all condition parameters
     **
     ** When the parameter set is streamed, the derived parameters of
     ** the conditions are not set, since the default constructor is
     ** called. So, the Init method of all conditions has to be called.
     **/
  void Init();


private:
  /** @brief Global parameters, used for all sensors **/
  CbmStsParSensorCond fGlobalParams {};

  /** @brief Map of parameters. Key is sensor address. **/
  std::map<UInt_t, CbmStsParSensorCond> fParams {};

  Bool_t fGlobal = kFALSE;  ///< Use global parameters for all sensors
  Bool_t fIsInit = kFALSE;  //! Initialisation flag

  ClassDef(CbmStsParSetSensorCond, 2);
};

#endif /* CBMSTSPARSETSENSORCOND */
