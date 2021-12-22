/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

/**
 * @file CbmRichUnpackAlgo.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Baseclass for the TrdR unpacker algorithms
 * @version 0.1
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2021
 * 
 * This is the base class for the algorithmic part of the tsa data unpacking 
 * processes of the CbmTrd.
 * The actual translation from tsa to digi happens in the derived classes. 
 * 
 * 
*/

#ifndef CbmRichUnpackAlgo2022_H
#define CbmRichUnpackAlgo2022_H

#include "CbmMcbm2018RichPar.h"
#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmRichDigi.h"
#include "CbmRichUnpackAlgoBase.h"

#include "Timeslice.hpp"  // timeslice

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <memory>
#include <utility>

class CbmRichUnpackAlgo2022 : public CbmRichUnpackAlgoBase {
public:
  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmRichUnpackAlgo2022();

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmRichUnpackAlgo2022();

  /** @brief Copy constructor - not implemented **/
  CbmRichUnpackAlgo2022(const CbmRichUnpackAlgo2022&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmRichUnpackAlgo2022& operator=(const CbmRichUnpackAlgo2022&) = delete;


protected:
  void processCBMtime(CbmRichUnpackAlgoMicrosliceReader& reader);

  void processTrbPacket(CbmRichUnpackAlgoMicrosliceReader& reader);

  void processHubBlock(CbmRichUnpackAlgoMicrosliceReader& reader);

  void processCtsSubSubEvent(CbmRichUnpackAlgoMicrosliceReader& reader, uint32_t subSubEventSize,
                             uint32_t subSubEventId);

  void processSubSubEvent(CbmRichUnpackAlgoMicrosliceReader& reader, int nofTimeWords, uint32_t subSubEventId);

  void processTimeDataWord(CbmRichUnpackAlgoMicrosliceReader& reader, int iTdc, uint32_t epoch, uint32_t tdcWord,
                           uint32_t subSubEventId, std::vector<double>& raisingTime);

  /**
	 * Write a gidi object into the output collection
	 */
  void writeOutputDigi(Int_t fpgaID, Int_t channel, Double_t time, Double_t tot);


  /**
   * @brief Unpack a given microslice. To be implemented in the derived unpacker algos.
   * 
   * @param ts timeslice pointer
   * @param icomp index to the component to be unpacked
   * @param imslice index of the microslice to be unpacked
   * @return true 
   * @return false 
   * 
   * @remark The content of the µslice can only be accessed via the timeslice. Hence, we need to pass the pointer to the full timeslice
  */
  bool unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice);

  //bool checkMaskedDiRICH(Int_t subSubEventId);

  /** @brief Parameters for the unpacking */
  //CbmMcbm2018RichPar fUnpackPar;

  double fMbsPrevTimeCh0 = 0.;
  double fMbsPrevTimeCh1 = 0.;

  uint64_t fCbmTimeMS;
  uint64_t fCbmTimePacket;

  double fPrevLastCh0ReTime[13];  // 12 DiRICHes chnl0 + 1 CTS chnl0
  int fCurrentSubSubEvent = 0;

private:
  ClassDef(CbmRichUnpackAlgo2022, 2)
};

#endif  // CbmRichUnpackAlgo2022_H
