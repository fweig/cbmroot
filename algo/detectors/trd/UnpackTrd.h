/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, Dominik Smith [committer] */

#ifndef CBM_ALGO_UNPACKTRD_H
#define CBM_ALGO_UNPACKTRD_H 1


#include "CbmTrdDigi.h"
#include "CbmTrdRawMessageSpadic.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include <memory>

#include <cmath>

namespace cbm::algo
{
  /** @struct UnpackTrdElinkPar
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023
   ** @brief TRD Unpacking parameters for one eLink 
   **/
  struct UnpackTrdElinkPar {
    std::vector<uint32_t> fChanAddress;  ///< CbmTrdAddress for different channels
    uint32_t fAddress;                   ///< Asic address
    uint64_t fTimeOffset = 0.;           ///< Time calibration parameter
  };

  /** @struct UnpackTrdCrobPar
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023
   ** @brief TRD Unpacking parameters for one CROB
   **/
  struct UnpackTrdCrobPar {
    std::vector<UnpackTrdElinkPar> fElinkParams = {};  ///< Parameters for each eLink
  };

  /** @struct UnpackTrdPar
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023
   ** @brief Parameters required for the TRD unpacking (specific to one component)
   **/
  struct UnpackTrdPar {
    bool fUseBaselineAvg                      = true;  ///< Is baseline average function of Spadic activated
    float_t fMaxAdcToEnergyCal                = 1.0;   ///< max adc to energy in keV
    uint32_t fNumChansPerAsic                 = 0;     ///< Number of channels per ASIC
    uint32_t fNumAsicsPerModule               = 0;     ///< Number of ASICS per module
    std::vector<UnpackTrdCrobPar> fCrobParams = {};    ///< Parameters for each CROB
  };


  /** @struct UnpackTrdMoni
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023
   ** @brief Monitoring data for TRD unpacking
   **/
  struct UnpackTrdMonitorData {
    uint32_t fNumNonHitOrTsbMessage     = 0;
    uint32_t fNumErrElinkOutOfRange     = 0;  ///< Elink not contained in parameters
    uint32_t fNumErrInvalidFirstMessage = 0;  ///< First message is not TS_MSB or second is not EPOCH
    uint32_t fNumErrInvalidMsSize       = 0;  ///< Microslice size is not multiple of message size
    uint32_t fNumErrTimestampOverflow   = 0;  ///< Overflow in 64 bit time stampa
    size_t fNumNonMajorTsMsb            = 0;  ///< Counter for the ts_msb used to reconstruct the time
    size_t fNumElinkMis                 = 0;  ///< Number of SOM to RDA/EOM mismatches
    size_t fNumCorruptEom               = 0;  ///< Number of corrupted EOM frames
    size_t fNumWildRda                  = 0;  ///< Number of rda frames outside of a SOM frame range
    size_t fNumWildEom                  = 0;  ///< Number of eom frames outside of a SOM frame range
    size_t fNumUnknownWords             = 0;  ///< Number of unknown words
    size_t fNumMissingEom               = 0;  ///< Number of missing EOM frames to finish a SOM frame
    size_t fNumWildNul                  = 0;  ///< Number of wild null words, should only appear at the end of a µSlice
    size_t fNumCreatedRawMsgs           = 0;  ///< counter of created raw messages
    size_t fNumEpochMsgs                = 0;  ///< counter of created raw messages
    size_t fNumCrcValidFlags            = 0;  ///< counter for inf/error flags from the µSlices
    size_t fNumOverflowFlimFlags        = 0;  ///< counter for inf/error flags from the µSlices
    size_t fNumOverflowUserFlags        = 0;  ///< counter for inf/error flags from the µSlices
    size_t fNumDataErrorFlags           = 0;  ///< counter for inf/error flags from the µSlices
    size_t fNumCreatedInfoMsgs          = 0;  ///< counter of created info messages
    bool HasErrors()
    {
      uint32_t numErrors = fNumNonHitOrTsbMessage + fNumErrElinkOutOfRange + fNumErrInvalidFirstMessage
                           + fNumErrInvalidMsSize + fNumErrTimestampOverflow;
      return (numErrors > 0 ? true : false);
    }
  };


  /** @class UnpackTrd
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 31 January 2023 
   ** @brief Unpack algorithm for TRD
   **/
  class UnpackTrd {

  public:
    typedef std::pair<std::vector<CbmTrdDigi>, UnpackTrdMonitorData> resultType;


    /** @brief Default constructor **/
    UnpackTrd() {};


    /** @brief Destructor **/
    ~UnpackTrd() {};


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
    void SetParams(std::unique_ptr<UnpackTrdPar> params) { fParams = *(std::move(params)); }

  private:                      // members
    UnpackTrdPar fParams = {};  ///< Parameter container

    /**
     ** @brief Handle the output created by the explicit algorithms. E.g. write to output vectors.
     ** @param digi 
     ** @param raw 
     **/
    void digestOutput(std::unique_ptr<CbmTrdDigi> digi, CbmTrdRawMessageSpadic raw);

    /**
     ** @brief Digest the aditional flags stored in the 4 "cccc" bits of the EPO messages.
     ** @param frame 
     ** @return Spadic::MsInfoType
     **/
    Spadic::MsInfoType digestBufInfoFlags(const std::uint32_t frame);

    /**
     ** @brief Digest the flags of the currently unpacked µSlice.
     ** @param flags flags stored in the µSlice descriptor 
     ** @param storage of monitoring data
     **/
    void digestMsFlags(const std::uint16_t flags, UnpackTrdMonitorData& monitor);

    /**
     ** @brief Digest a info message run all default information forwarding from the msg.
     ** @param frame 
     **/
    void digestInfoMsg(const std::uint32_t frame);

    /**
     ** @brief Extract one adc sample from a given adcbuffer
     ** @param[in] adcbuffer 
     ** @param[in,out] nadcbits 
     ** @return std::int16_t 
     **/
    std::int16_t extractSample(size_t* adcbuffer, size_t* nadcbits);

    /**
     ** @brief Extract the baseline average sample from a given adcbuffer.
     ** Depending on the Spadic settings sample-0 is a plain sample or the averaged 
     ** baseline calculation. The latter is not a 9 bit signed integer, but a 9 bit 
     ** floating point number 7 digits before the point and 2 afterwards.
     ** @param[in] adcbuffer 
     ** @param[in,out] nadcbits 
     ** @return std::float_t
     **/
    std::float_t extractAvgSample(size_t* adcbuffer, size_t* nadcbits);

    /** @brief Identify the InfoType of a 64bit InfoMessage word inside a Microslice */
    Spadic::MsInfoType getInfoType(const std::uint32_t frame);

    /**
     ** @brief Get the ts_msb information from the TS_MSB(kEPO) frame. We take the first of the 3
     ** The 3 redundant TS_MSB sets are already compared at the FPGA level.
     ** @param frame
     ** @param storage of monitoring data
     ** @return ts_msb value
     **/
    std::uint8_t getTsMsb(const std::uint32_t frame, UnpackTrdMonitorData& monitor);

    /**
     ** @brief Create a CbmTrdRawMessageSpadic from the hit message input. 
     ** @param word 
     ** @param criId id of the cri that send the µSlice
     ** @param criobId id of the crob that send the µSlice (currently not used set to 0 062021 PR)
     ** @param istream
     ** @return CbmTrdRawMessageSpadic 
     ** @todo Check if we can get rid of the future obsolete microslice stuff.
     **/
    CbmTrdRawMessageSpadic makeRaw(const std::uint32_t frame, std::uint16_t criId, std::uint8_t crobId,
                                   std::uint16_t elinkId, std::uint8_t istream);

    /**
     ** @brief Get the Digi Trigger Type from the raw message triggertype
     ** @param tt 
     ** @return CbmTrdDigi::eTriggerType 
     **/
    static CbmTrdDigi::eTriggerType GetDigiTriggerType(Spadic::eTriggerType tt);

    /**
     ** @brief Create an actual digi from the raw message
     ** @param raw 
     **/
    CbmTrdDigi makeDigi(CbmTrdRawMessageSpadic raw);

    /**
     ** @brief Get the Bin Time Shift value 
     ** @param samples 
     ** @return ULong64_t 
     **/
    uint64_t GetBinTimeShift(const std::vector<std::int16_t>* samples) { return 0.; };

    /**
     ** @brief Get the MaxAdc value
     ** @param samples 
     ** @return Float_t 
     **/
    std::float_t GetMaxAdcValue(const std::vector<std::int16_t>* samples);

    /**
     ** @brief Get the Baseline value
     ** The digi charge is an unsigned. Hence, we need to get the baseline to 0
     ** @param samples 
     ** @return float_t 
     **/
    float_t GetBaseline(const std::vector<std::int16_t>* samples);

    /** @brief Identify the message type of a given 32bit frame inside a Microslice */
    Spadic::MsMessageType getMessageType(const std::uint32_t frame);

    /** @brief Counter for the ts_msb used to reconstruct the time */
    std::vector<std::uint8_t> fNrTsMsbVec = {};

    /** @brief Bytes per spadic frame stored in the microslices */
    static const std::uint8_t fBytesPerWord = 8;

    /** For the msg format used from 2021 ongoing we have 2 parallel streams per word. *
     ** All data from eLinks 0..20 go to one stream and 21..41 to the other            */
    /** @brief Number of streams per word **/
    static const std::uint8_t fStreamsPerWord = 2;

    /**
     ** @brief Time offset for the system
     ** @todo This should be module and channel dependent and included into the asic parameters
     **/
    std::int32_t fSystemTimeOffset = 0;

    /** @brief Start time of the current µSlice relative to the Timeslice start time in Spadic CC. */
    size_t fMsStartTimeRelCC = 0;

    /** @brief Time of the last succesful digest hit message */
    size_t fLastFulltime = 0;

    /** @brief Number of samples not considered for max adc */
    static constexpr size_t fNrOfPresamples = 1;

    /** @brief Clock length of Spadic in ns. */
    static constexpr float_t fAsicClockCycle = 62.5;

    /** @brief length of one ts_msb in [ns] */
    static constexpr std::uint16_t fTsMsbLength = 16000;

    /** @brief length of one ts_msb in [cc] */
    static constexpr size_t fTsMsbLengthCC = fTsMsbLength / fAsicClockCycle;

    /** @brief First sample to look for the max adc */
    static constexpr size_t fPeakingBinMin = fNrOfPresamples;

    /**
     ** @brief Last sample to look for the max adc
     ** Default value is set based on the Shaping time + 5 samples safety margin.
     ** @remark the peaking time strongly depends on the input signal. Effective range of 
     ** the shaping time is between 120 and 240 ns.
     **/
    static constexpr size_t fPeakingBinMax = static_cast<size_t>(120.0 / fAsicClockCycle + fNrOfPresamples + 5);
  };


} /* namespace cbm::algo */

#endif /* CBM_ALGO_UNPACKTRD_H */
