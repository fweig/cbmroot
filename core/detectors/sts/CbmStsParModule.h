/* Copyright (C) 2014-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMSTSPARMODULE_H
#define CBMSTSPARMODULE_H 1

#include "CbmStsParAsic.h"  // for CbmStsParAsic

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDefNV

#include <string>  // for string
#include <vector>  // for vector

/** @class CbmStsParModule
 ** @brief Parameters for one STS module
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 31.03.2020
 **
 ** The module is the basic building block of the STS. It consists
 ** of a sensor connected to a number of ASICs. The number of module
 ** channels must be adjusted to the number of strips of the connected
 ** sensor. The number of ASICS is given by the number of module channels
 ** divided by the number of channels per ASIC.
 **/
class CbmStsParModule {

public:
  /** @brief Default constructor **/
  CbmStsParModule() {};


  /** @brief Standard constructor
   ** @param nChannels     Number of readout channels
   ** @param nAsics        Number of readout channels per ASIC
   **/
  CbmStsParModule(uint32_t nChannels, uint32_t nAsicChannels);


  /** @brief Destructor **/
  ~CbmStsParModule() {};


  /** @brief Randomly deactivate a fraction of the channels
   ** @param fraction  Fraction of channels to deactivate
   ** @return Number of deactivated channels
   **/
  uint32_t DeactivateRandomChannels(double fraction);


  /** @brief ASIC parameters for a given channel
   ** @param channel  Channel number
   ** @return ASIC parameters
   **/
  const CbmStsParAsic& GetParAsic(uint32_t channel) const;


  /** @brief All ASIC parameters
   ** @return Vector of ASIC parameters
   **/
  const std::vector<CbmStsParAsic>& GetAsicParams() const { return fAsicPars; }


  /** @brief Number of channels per ASIC
   ** @return Number of channels per ASIC
   **/
  uint32_t GetNofAsicChannels() const { return fNofAsicChannels; }


  /** @brief Number of ASICs
   ** @return Number of ASICs
   **/
  uint32_t GetNofAsics() const { return fAsicPars.size(); }


  /** @brief Number of channels
   ** @return Number of channels
   **/
  uint32_t GetNofChannels() const { return fNofChannels; }


  /** @brief Check for a channel being active
   ** @param channel  Channel number
   ** @return True if the channel is active
   **/
  bool IsChannelActive(uint32_t channel) const;


  /** @brief Set all ASICs with the same parameter set
   ** @param asicPar  Parameters for all ASICs
   **/
  void SetAllAsics(const CbmStsParAsic& asicPar);


  /** @brief Set parameters for a single ASIC
   ** @param asicPar  ASIC parameter set
   **/
  void SetAsic(uint32_t asicNr, const CbmStsParAsic& asicPar);


  /** @brief String output **/
  std::string ToString() const;


private:
  uint32_t fNofChannels     = 0;            ///< Number of readout channels
  uint32_t fNofAsicChannels = 0.;           ///< Number of channels per ASIC
  std::vector<CbmStsParAsic> fAsicPars {};  ///< ASIC parameters


  ClassDefNV(CbmStsParModule, 2);
};

#endif /* CBMSTSPARMODULE_H */
