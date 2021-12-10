/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmMCEventInfo.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.06.2018
 **/

#ifndef CBMMCEVENTINFO_H
#define CBMMCEVENTINFO_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef

#include <cstdint>
#include <string>  // for string

/** @class CbmMCEventInfo
 ** @brief Allows to access an MC event in the source file
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.06.2018
 **
 ** During digitization, an event time is generated for each input MC event
 ** obtained from transport simulations. This event time is stored in this
 ** class and saved to the output (raw) tree using the container CbmMCEventList.
 ** File number and event number allow to access the input MC event in its
 ** respective file, using CbmMCDataManager. The MC event time can be used
 ** for QA of reconstructed data, i.e., comparing the time of the
 ** reconstructed event with the MC truth time.
 **/
class CbmMCEventInfo {

public:
  /** @brief Constructor
     ** @param fileId   Input file index
     ** @param eventId  MC event index (entry in ROOT tree)
     ** @param time     Event start time [ns]
     **/
  CbmMCEventInfo(int32_t fileId = -1, int32_t eventId = -1, double time = 0.);


  /** @brief Destructor **/
  virtual ~CbmMCEventInfo();


  /** @brief File index
     ** @value File index
     **/
  int32_t GetFileId() const { return fFileId; }


  /** @brief Event index
     ** @value Event index
     **/
  int32_t GetEventId() const { return fEventId; }


  /** @brief Event time
     ** @value Event time [ns]
     **/
  double GetTime() const { return fTime; }


  /** Status to string **/
  std::string ToString() const;


  /** @brief Comparison operator **/
  bool operator<(const CbmMCEventInfo& other) const
  {
    if (fFileId == other.fFileId) return (fEventId < other.fEventId);
    return (fFileId < other.fFileId);
  }


private:
  int32_t fFileId;
  int32_t fEventId;
  double fTime;

  ClassDef(CbmMCEventInfo, 1);
};


#endif /* CBMMCEVENTINFO_H_ */
