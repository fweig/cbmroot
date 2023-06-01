/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, Alexandru Bercuci, Dominik Smith [committer] */

#ifndef CBM_ALGO_UNPACKTRD2D_H
#define CBM_ALGO_UNPACKTRD2D_H 1


#include "CbmTrdDigi.h"
#include "CbmTrdRawMessageSpadic.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include <array>
#include <memory>
#include <sstream>

#include <cmath>

#define NFASPMOD 180
#define NCROBMOD 5
#define NFASPCROB NFASPMOD / NCROBMOD
#define NFASPCH 16

#define FASP_EPOCH_LENGTH 128

namespace cbm::algo
{

  enum CbmTrdFaspMessageType
  {
    kEpoch = 0,
    kData
  };

  /** @brief Data structure for unpacking the FASP word */
  struct CbmTrdFaspMessage {
    CbmTrdFaspMessage(uint8_t c, uint8_t typ, uint8_t t, uint16_t d, uint8_t rob, uint8_t asic);
    uint8_t ch     = 0;  ///< ch id in the FASP
    uint8_t type   = 0;  ///< message type 0 = epoch, 1 = data (not used for the moment)
    uint8_t tlab   = 0;  ///< time of the digi inside the epoch
    uint16_t data  = 0;  ///< ADC value
    uint32_t epoch = 0;  ///< epoch id (not used for the moment)
    uint32_t mod   = 0;  ///< full module address according to CbmTrdAddress
    uint8_t crob   = 0;  ///< CROB id in the module
    uint8_t fasp   = 0;  ///< FASP id in the module
  };

  /** @struct UnpackTrd2dChannelPar
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023
   ** @brief TRD Unpacking parameters for one Asic channel
   **/
  struct UnpackTrd2dChannelPar {
    int32_t fPadAddress;      ///< Pad address for channel
    bool fHasPairingR;        ///< Flag for R or T compoment
    uint64_t fDaqOffset = 0;  ///< Time calibration parameter
  };

  /** @struct UnpackTrd2dAsicPar
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023
   ** @brief TRD Unpacking parameters for one Asic
   **/
  struct UnpackTrd2dAsicPar {
    std::vector<UnpackTrd2dChannelPar> fChanParams;  ///< Parameters for different channels
  };

  /** @struct UnpackTrd2dPar
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023
   ** @brief Parameters required for the TRD unpacking (specific to one component)
   **/
  struct UnpackTrd2dPar {
    uint16_t fModId                             = 0;   ///< Module ID of component
    uint8_t fCrobId                             = 0;   ///< CROB ID of component
    std::vector<UnpackTrd2dAsicPar> fAsicParams = {};  ///< Parameters for each ASIC
  };


  /** @struct UnpackTrd2dMoni
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023
   ** @brief Monitoring data for TRD unpacking
   **/
  struct UnpackTrd2dMonitorData {
    uint32_t fNumSelfTriggeredData = 0;  ///< word fulfills data & 0x2000
    uint32_t fNumIncompleteDigis   = 0;  ///< incomplete digis left in pads after finalization
    uint32_t fNumErrEndBitSet      = 0;  ///< Corrupted data with end bit set

    bool HasErrors()
    {
      uint32_t numErrors = fNumErrEndBitSet;
      return (numErrors > 0 ? true : false);
    }
    std::string print()
    {
      std::stringstream ss;
      ss << "stats " << fNumSelfTriggeredData << " | " << fNumIncompleteDigis << " | errors " << fNumErrEndBitSet
         << " | ";
      return ss.str();
    }
  };

  /** @class UnpackTrd2d
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023 
   ** @brief Unpack algorithm for TRD
   **/
  class UnpackTrd2d {

  public:
    typedef std::pair<std::vector<CbmTrdDigi>, UnpackTrd2dMonitorData> resultType;


    /** @brief Default constructor **/
    UnpackTrd2d() {};


    /** @brief Destructor **/
    ~UnpackTrd2d() {};


    /** @brief Algorithm execution
     ** @param  msContent  Microslice payload
     ** @param  msDescr    Microslice descriptor
     ** @param  tTimeslice Unix start time of timeslice [ns]
     ** @return TRD digi data
     **/
    resultType operator()(const uint8_t* msContent, const fles::MicrosliceDescriptor& msDescr,
                          const uint64_t tTimeslice);

    /** @brief Set the parameter container
     ** @param params Pointer to parameter container
     **/
    void SetParams(std::unique_ptr<UnpackTrd2dPar> params) { fParams = *(std::move(params)); }

  private:                        // members
    UnpackTrd2dPar fParams = {};  ///< Parameter container

    UnpackTrd2dMonitorData fMonitor;  ///< Container for monitoring data

    /**
     ** @brief Time offset for the system
     ** @todo This should be module and channel dependent and included into the asic parameters
     **/
    std::int32_t fSystemTimeOffset = 0;

    bool pushDigis(std::vector<CbmTrdFaspMessage> messages, const uint64_t time);

    /** @brief Finalize component (e.g. copy from temp buffers)  */
    std::vector<CbmTrdDigi> FinalizeComponent();

    // Constants
    /** @brief Bytes per FASP frame stored in the microslices (32 bits words)
   * - DATA WORD -
   * ffff.ffdd dddd.dddd dddd.tttt ttta.cccc
   * f - FASP id
   * d - ADC signal
   * t - time label inside epoch
   * a - word type (1)
   * c - channel id
   * - EPOCH WORD -
   * ffff.fftt tttt.tttt tttt.tttt ttta.cccc
   * f - FASP id
   * t - epoch index
   * a - word type (0)
   * c - channel id
   */
    static const std::uint8_t fBytesPerWord = 4;

    std::array<std::vector<CbmTrdDigi>, NFASPMOD* NFASPCH> fDigiBuffer = {
      {}};  ///> Buffered digi for each pad in CROB component
  };


} /* namespace cbm::algo */

#endif /* CBM_ALGO_UNPACKTRD2D_H */
