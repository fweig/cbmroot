/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsParSetModule.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 31.03.2020
 **/


#ifndef CBMSTSPARSETMODULE_H
#define CBMSTSPARSETMODULE_H 1

#include "CbmStsParModule.h"  // for CbmStsParModule

#include <FairParGenericSet.h>  // for FairParGenericSet

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for UInt_t, Bool_t, kFALSE, kTRUE

#include <map>     // for map
#include <string>  // for string

class FairParamList;

/** @class CbmStsParSetModule
 ** @brief Parameters container for CbmStsParModule
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 31.03.2020
 **
 ** A set of parameters for each STS module is administered through
 ** FairRuntimeDb.
 **/
class CbmStsParSetModule : public FairParGenericSet {

public:
  /** @brief Constructor
     ** @param name  Name of parameter set
     ** @param title Parameter container factory name
     ** @param context  No idea
     **/
  CbmStsParSetModule(const char* name = "CbmParSetModule", const char* title = "STS parameters",
                     const char* context = "Default");


  /** @brief Destructor **/
  virtual ~CbmStsParSetModule();


  /** @brief Reset all parameters **/
  virtual void clear();


  /** @brief Randomly deactivate a fraction of the channels
   ** @param fraction  Fraction of channels to deactivate
   ** @return Number of deactivated channels
   **/
  UInt_t DeactivateRandomChannels(Double_t fraction);


  /** @brief Reading parameters from ASCII. Abstract in base class.
     **
     ** An ASCII I/O is not implemented. The method throws an error.
     **/
  virtual Bool_t getParams(FairParamList* parList);


  /** @brief Get condition parameters of a sensor
     ** @param Module address
     ** @return Module parameter object
     **/
  const CbmStsParModule& GetParModule(UInt_t address);


  /** @brief Check whether parameter container is set
     ** @return True if container is set
     **
     ** The container is set if either global parameters are defined
     ** or the parameter map is filled.
     **/
  Bool_t IsSet() const { return (fUseGlobal || (!fParams.empty())); }


  virtual void putParams(FairParamList* parList);


  /** @brief Set global parameters (for all modules)
     ** @param conditions  Module parameter object
     **/
  void SetGlobalPar(const CbmStsParModule& params)
  {
    fGlobalParams = params;
    fUseGlobal    = kTRUE;
  }

  /** @brief Set the parameters for a module
     ** @param address  Module address
     ** @param par      Module parameter object
     **/
  void SetParModule(UInt_t address, const CbmStsParModule& par);


  /** @brief Info to string **/
  std::string ToString() const;


private:
  /** @brief Flag for using global parameters **/
  Bool_t fUseGlobal = kFALSE;

  /** @brief Global parameters, used for all modules **/
  CbmStsParModule fGlobalParams {};

  /** @brief Map of parameters. Key is module address. **/
  std::map<UInt_t, CbmStsParModule> fParams {};


  ClassDef(CbmStsParSetModule, 1);
};

#endif /* CBMSTSPARSETMODULE */
