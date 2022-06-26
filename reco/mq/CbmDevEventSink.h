/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer], Pierre-Alain Loizeau, Volker Friese */

#ifndef CBMDEVICEEVTSINK_H_
#define CBMDEVICEEVTSINK_H_

#include "CbmDigiEvent.h"

#include "FairMQDevice.h"

#include <vector>

class TimesliceMetaData;
class FairRunOnline;
class FairRootManager;


/** @class CbmDefEventSink
 ** @brief MQ device class to write CbmDigiEvents to a ROOT file
 ** @author Dominik Smith <d.smith@gsi.de>
 **
 ** Based on previous, similar implementations by P.-A. Loizeau
 **
 ** The event sink device receives data (vector of CbmDigiEvents for a given timeslice) in the
 ** respective input channel and fills a ROOT tree/file with these data.
 **/
class CbmDevEventSink : public FairMQDevice {
public:
  /** @brief Constructor **/
  CbmDevEventSink() {};

  /** @brief Destructor **/
  virtual ~CbmDevEventSink();


protected:
  /** @brief Action on command messages
   ** @param parts  Message
   ** @param flag Not used; ignored
   ** @return Success
   **/
  bool HandleCommand(FairMQMessagePtr&, int flag);

  /** @brief Action on data messages
   ** @param parts  Message
   ** @param flag Not used; ignored
   ** @return Success
   */
  bool HandleData(FairMQParts& parts, int flag);

  /** @brief Initialization **/
  virtual void InitTask();


private:  // methods
  /** @brief Finishing run **/
  void Finish();


private:  // members
  // --- Counters and status flags
  size_t fNumMessages                  = 0;        ///< Number of received data messages
  size_t fNumTs                        = 0;        ///< Number of processed timeslices
  uint64_t fPrevTsIndex                = 0;        ///< Index of last processed timeslice
  bool fFinishDone                     = false;    ///< Keep track of whether the Finish method was already called
  TimesliceMetaData* fTsMetaData       = nullptr;  ///< Data output: TS meta data
  std::vector<CbmDigiEvent>* fEventVec = nullptr;  ///< Data output: events
  FairRunOnline* fFairRun              = nullptr;  ///< FairRunOnline to instantiate FairRootManager
  FairRootManager* fFairRootMgr        = nullptr;  ///< FairRootManager used for ROOT file I/O
};

#endif /* CBMDEVICEEVTSINK_H_ */
