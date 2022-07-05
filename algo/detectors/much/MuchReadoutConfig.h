/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----            MuchReadoutConfig header file                       -----
// -----            Created 22/02/22  by P.-A. Loizeau                 -----
// -----            Modified 07/12/18  by A Kumar                      -----
// -------------------------------------------------------------------------

#ifndef ALGO_DETECTORS_MUCH_MUCHREADOUTCONFIG_H
#define ALGO_DETECTORS_MUCH_MUCHREADOUTCONFIG_H

//#include "FairParGenericSet.h"

#include <cstdint>
#include <vector>

//#include "TArrayD.h"
//#include "TArrayI.h"

//class FairParIo;
//class FairParamList;

namespace cbm::algo
{

  //class MuchReadoutConfig : public FairParGenericSet {
  class MuchReadoutConfig {

  public:
    /** Standard constructor **/
    MuchReadoutConfig() {};

    /** Destructor **/
    ~MuchReadoutConfig() {};

    /** Reset all parameters **/
    virtual void clear();

    /*
  void putParams(FairParamList*);
  bool getParams(FairParamList*);
*/
    static constexpr uint16_t GetNbCrobsPerDpb() { return kuNbCrobsPerDpb; }
    static constexpr uint16_t GetNbElinkPerCrob() { return kuNbElinksPerCrob; }
    static constexpr uint16_t GetNbElinkPerDpb() { return kuNbCrobsPerDpb * kuNbElinksPerCrob; }
    static constexpr uint16_t GetNbFebsPerCrob() { return kuNbFebsPerCrob; }
    static constexpr uint16_t GetNbFebsPerDpb() { return kuNbCrobsPerDpb * kuNbFebsPerCrob; }
    static constexpr uint16_t GetNbAsicsPerFeb() { return kuNbAsicsPerFeb; }
    static constexpr uint16_t GetNbAsicsPerCrob() { return kuNbFebsPerCrob * kuNbAsicsPerFeb; }
    static constexpr uint16_t GetNbAsicsPerDpb() { return kuNbCrobsPerDpb * GetNbAsicsPerCrob(); }
    static constexpr uint16_t GetNbChanPerAsic() { return kuNbChanPerAsic; }
    static constexpr uint16_t GetNbChanPerFeb() { return kuNbAsicsPerFeb * kuNbChanPerAsic; }

    //! Convert from eLink index to FEB Connection ( 0 to kuNbFebsPerCrob)
    int16_t ElinkIdxToFebIdx(uint16_t uElink);

    uint16_t GetNrOfDpbs() { return fuNrOfDpbs; }
    uint16_t GetDpbId(uint16_t uDpbIdx);
    uint16_t GetNrOfCrobs() { return fuNrOfDpbs * kuNbCrobsPerDpb; }
    uint16_t GetNrOfFebs() { return GetNrOfCrobs() * kuNbFebsPerCrob; }
    uint16_t GetNrOfAsics() { return GetNrOfFebs() * kuNbAsicsPerFeb; }
    uint16_t GetNrOfFebsInGemA() { return fuFebsInGemA; }
    uint16_t GetNrOfFebsInGemB() { return fuFebsInGemB; }
    int16_t GetNrOfChannels() { return kuNbChanPerAsic; }

    int32_t GetFebId(uint16_t);
    uint16_t GetModule(uint16_t);

    int8_t GetPadXA(uint8_t febid, uint8_t channelid);
    int8_t GetPadYA(uint8_t febid, uint8_t channelid);
    int8_t GetPadXB(uint8_t febid, uint8_t channelid);
    int8_t GetPadYB(uint8_t febid, uint8_t channelid);
    double GetRealX(int16_t);
    double GetRealPadSize(int16_t);

    double GetRealX(int16_t Channel, int16_t Sector);
    double GetRealPadSize(int16_t Channel, int16_t Sector);

    //RPC Module Related Functions
    uint16_t GetNrOfFebsInRpc() { return fuFebsInRpc; }
    int8_t GetPadXRpc(uint8_t febid, uint8_t channelid);
    int8_t GetPadYRpc(uint8_t febid, uint8_t channelid);

    bool IsCrobActive(uint16_t uDpbIdx, uint16_t uCrobIdx);
    bool IsFebActive(uint16_t uFebInSystIdx);
    bool IsFebActive(uint16_t uDpbIdx, uint16_t uCrobIdx, uint16_t uFebIdx);
    bool IsFebPulser(uint16_t uFebInSystIdx);
    bool IsFebPulser(uint16_t uDpbIdx, uint16_t uCrobIdx, uint16_t uFebIdx);
    double GetFebAdcGain(uint16_t uDpbIdx, uint16_t uCrobIdx, uint16_t uFebIdx);
    double GetFebAdcOffset(uint16_t uDpbIdx, uint16_t uCrobIdx, uint16_t uFebIdx);

  private:
    /// Constants
    static const uint16_t kuNbCrobsPerDpb   = 1;    // Number of CROBs possible per DPB
    static const uint16_t kuNbElinksPerCrob = 42;   // Number of elinks in each CROB ?
    static const uint16_t kuNbFebsPerCrob   = 9;    // Number of FEBs  connected to each CROB for mMuch 2019
    static const uint16_t kuNbAsicsPerFeb   = 1;    // Number of ASICs connected in each FEB for MUCH
    static const uint16_t kuNbChanPerAsic   = 128;  // Number of channels in each ASIC
    //   static constexpr uint16_t  kuCrobMapElinkFebIdx[ kuNbElinksPerCrob ] = {
    const int16_t kiCrobMapElinkFebIdx[kuNbElinksPerCrob] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2,
                                                             3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5,
                                                             6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8};
    //! Map from eLink index to ASIC index within CROB ( 0 to kuNbFebsPerCrob * kuNbAsicPerFeb )

    /// Variables
    uint16_t fuNrOfDpbs = 0;                  // Total number of MUCH DPBs in system
    std::vector<int16_t> fiDbpIdArray;        // Array to hold the unique IDs (equipment ID) for all MUCH DPBs
    std::vector<int16_t> fiCrobActiveFlag;    // Array to hold the active flag for all CROBs, [ NbDpb * kuNbCrobPerDpb ]
    uint16_t fuFebsInGemA = 0;                // Number of FEBs connected in GEM Module A
    uint16_t fuFebsInGemB = 0;                // Number of FEBs connected in GEM Module B
    uint16_t fuFebsInRpc  = 0;                // Number of FEBs connected in RPC Module
    std::vector<int16_t> fnFebsIdsArrayGemA;  // Array to hold FEB IDs connected to GEM Module A
    std::vector<int16_t> fnFebsIdsArrayGemB;  // Array to hold FEB IDs connected to GEM Module B
    std::vector<int16_t> fnFebsIdsArrayRpc;   // Array to hold FEB IDs connected to RPC Module
    std::vector<int16_t> fChannelsToPadXA;  // Array which stores the corresponding x position of PAD of entire module A
    std::vector<int16_t> fChannelsToPadYA;  // Array which stores the corresponding y position of PAD of entire module A
    std::vector<int16_t> fChannelsToPadXB;  // Array which stores the corresponding x position of PAD of entire module B
    std::vector<int16_t> fChannelsToPadYB;  // Array which stores the corresponding y position of PAD of entire module B
    std::vector<int16_t> fChannelsToPadXRpc;  // Array which stores the corresponding x position of PAD of RPC module
    std::vector<int16_t> fChannelsToPadYRpc;  // Array which stores the corresponding y position of PAD of RPC module
    std::vector<double> fRealX;               // Array which stores the Real X (starting 18.733 cm) position of PAD
    std::vector<double> fRealPadSize;  // Array which stores the Real Progressive size of each padX (starting .327 cm )
  };

} /* namespace cbm::algo */

#endif  // ALGO_DETECTORS_MUCH_MUCHREADOUTCONFIG_H
