/** @file CbmMCEventList.h
  * @copyright Copyright (C) 2015-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmMCEventList.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 24.11.2015
 **/

#ifndef CBMMCEVENTLIST_H
#define CBMMCEVENTLIST_H 1

#include "CbmMCEventInfo.h"  // for CbmMCEventInfo

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for UInt_t, Bool_t, Double_t, Int_t, Option_t
#include <TNamed.h>      // for TNamed

#include <string>  // for string
#include <vector>  // for vector, vector<>::iterator

#include <stddef.h>  // for size_t

/* Implementation note (VF/180618):
 * Both indexed access (for loops over all MC events in a time slice)
 * and random access (to get the event time for a given fileId and EventId)
 * are required. It was thus chosen to internally represent the event list
 * as a sortable vector of CbmMCEventInfo. I tried to use std::tuple instead
 * of CbmMCEventInfo, but ROOT seems not to be able to stream that.
 * Since it is assumed that the creation of the list is separate from the
 * access to the list, no sorting or checking for double occurrences of events
 * is done on insertion for performance reasons.
 * Sorting will be done on first access to the list. It includes checking of
 * double occurrences of (fileId, eventId).
 */

/** @class CbmMCEventList
 ** @brief Container class for MC events with number, file and start time
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 24.11.2015
 ** @version 18.06.2018
 **/
class CbmMCEventList : public TNamed {

public:
  /** @brief Standard constructor **/
  CbmMCEventList();


  /** @brief Destructor **/
  virtual ~CbmMCEventList();


  /** @brief Delete all event entries **/
  virtual void Clear(Option_t*) { fEvents.clear(); }


  /** @brief Event number by index
     ** @value Event number for event at given index in list
     **
     ** Returns -1 if the index is out of bounds.
     **/
  Int_t GetEventIdByIndex(UInt_t index);


  /** @brief Event start time
     ** @param event  MC event number
     ** @param file   MC input file number
     ** @value MC event start time [ns]
     **
     ** Returns -1. if the event is not present in the list.
     **/
  Double_t GetEventTime(UInt_t event, UInt_t file);


  /** @brief Event time by index
     ** @value Event time for event at given index in list
     **
     ** Returns -1. if the index is out of bounds.
     **/
  Double_t GetEventTimeByIndex(UInt_t index);


  /** @brief File number by index
     ** @value File number for event at given index in list
     **
     ** Returns -1 if the index is out of bounds.
     **/
  Int_t GetFileIdByIndex(UInt_t index);


  /** @brief Number of events in the list
     ** @value Number of events
     **/
  std::size_t GetNofEvents() const { return fEvents.size(); }


  /** Insert an event with its start time into the event list
     ** @param event  MC event number
     ** @param file   MC input file number
     ** @param time   MC event start time [ns]
     ** @value kFALSE if (file, event) is already in the list. Else kTRUE.
     **
     ** If the event from the given file is already in the list, the list
     ** will not be modified and kFALSE is returned.
     **
     ** Negative event times are not allowed. In that case, kFALSE is
     ** returned and the list will not be modified.
     */
  Bool_t Insert(UInt_t event, UInt_t file, Double_t time);


  /** Print to screen **/
  virtual void Print(Option_t* opt = "") const;


  /** @brief Sort the list
     **
     ** The sorting uses the comparison operator of std::tuple, i.e.
     ** the list is first sorted w.r.t. fileId, then w.r.t. the event Id.
     ** Double occurrences of a pair (fileId, eventId) will throw an
     ** exception.
     */
  void Sort();


  /** Status to string **/
  std::string ToString(const char* option = "") const;


private:
  /** Event container **/
  std::vector<CbmMCEventInfo> fEvents;

  /** Flag whether list has been sorted **/
  Bool_t fIsSorted;


  /** @brief Check for double occurrences of events in list
     ** @value kTRUE is no double occurrences, else kFALSE
     **/
  Bool_t Check();


  /** @brief Find an element in the list
     ** @param file  Input file ID
     ** @param event MC event number (index)
     **/
  std::vector<CbmMCEventInfo>::iterator Find(UInt_t file, UInt_t event);


  ClassDef(CbmMCEventList, 3);
};


#endif /* CBMMCEVENTLIST_H */
