/** @file CbmMCEventInfo.h
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmMCEventInfo.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.06.2018
 **/

#ifndef CBMMCEVENTINFO_H
#define CBMMCEVENTINFO_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t

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
  CbmMCEventInfo(Int_t fileId = -1, Int_t eventId = -1, Double_t time = 0.);


  /** @brief Destructor **/
  virtual ~CbmMCEventInfo();


  /** @brief File index
     ** @value File index
     **/
  Int_t GetFileId() const { return fFileId; }


  /** @brief Event index
     ** @value Event index
     **/
  Int_t GetEventId() const { return fEventId; }


  /** @brief Event time
     ** @value Event time [ns]
     **/
  Double_t GetTime() const { return fTime; }


  /** Status to string **/
  std::string ToString() const;


  /** @brief Comparison operator **/
  bool operator<(const CbmMCEventInfo& other) const
  {
    if (fFileId == other.fFileId) return (fEventId < other.fEventId);
    return (fFileId < other.fFileId);
  }


private:
  Int_t fFileId;
  Int_t fEventId;
  Double_t fTime;

  ClassDef(CbmMCEventInfo, 1);
};


#endif /* CBMMCEVENTINFO_H_ */
