/** @file CbmStsParModule.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 31.03.2020
 **/

#ifndef CBMSTSPARMODULE_H
#define CBMSTSPARMODULE_H 1

#include <Rtypes.h>         // for THashConsistencyHolder, ClassDefNV
#include <RtypesCore.h>     // for UInt_t

#include <string>           // for string
#include <vector>           // for vector

#include "CbmStsParAsic.h"  // for CbmStsParAsic

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
    CbmStsParModule() {
    };


    /** @brief Standard constructor
     ** @param nChannels     Number of readout channels
     ** @param nAsics        Number of readout channels per ASIC
     **/
    CbmStsParModule(UInt_t nChannels, UInt_t nAsicChannels);


    /** @brief Destructor **/
    ~CbmStsParModule() {
    };


    /** @brief ASIC parameters for a given channel
     ** @param channel  Channel number
     ** @return ASIC parameters
     **/
    const CbmStsParAsic& GetParAsic(UInt_t channel) const;


    /** @brief All ASIC parameters
     ** @return Vector of ASIC parameters
     **/
    const std::vector<CbmStsParAsic>& GetAsicParams() const {
      return fAsicPars;
    }


    /** @brief Number of channels per ASIC
     ** @return Number of channels per ASIC
     **/
    UInt_t GetNofAsicChannels() const { return fNofAsicChannels; }


    /** @brief Number of ASICs
     ** @return Number of ASICs
     **/
    UInt_t GetNofAsics() const { return fAsicPars.size(); }


    /** @brief Number of channels
     ** @return Number of channels
     **/
    UInt_t GetNofChannels() const { return fNofChannels; }


    /** @brief Check for a channel being active
     ** @param channel  Channel number
     ** @return True if the channel is active
     **/
    Bool_t IsChannelActive(UInt_t channel) const;


    /** @brief Set all ASICs with the same parameter set
     ** @param asicPar  Parameters for all ASICs
     **/
    void SetAllAsics(const CbmStsParAsic& asicPar);


    /** @brief String output **/
    std::string ToString() const;


  private:

    UInt_t fNofChannels = 0;         ///< Number of readout channels
    UInt_t fNofAsicChannels = 0.;    ///< Number of channels per ASIC
    std::vector<CbmStsParAsic> fAsicPars { }; ///< ASIC parameters


    ClassDefNV(CbmStsParModule, 1);
};

#endif /* CBMSTSPARMODULE_H */
