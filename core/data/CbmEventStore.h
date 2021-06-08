/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

/** @file CbmEventStore.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 11.03.2020
 **/


#ifndef CBMEVENTSTORE_H
#define CBMEVENTSTORE_H 1

#include "CbmDefs.h"           // for kNofSystems
#include "CbmDigiContainer.h"  // for CbmDigiContainer
#include "CbmDigiVector.h"     // for CbmDigiVector

#include <Logger.h>  // for LOG

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for UInt_t, Bool_t, kFALSE, kTRUE
#include <TObjArray.h>   // for TObjArray
#include <TObject.h>     // for TObject

#include <boost/any.hpp>  // for any_cast

#include <string>  // for string

#include <assert.h>  // for assert

class CbmMatch;

/** @class CbmEventStore
 ** @brief Storable event class for CBM
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 11.03.2020
 ** @version 1.0
 **
 ** The storable event contains digi objects associated to an event
 ** as direct data members. It is as such persistent.
 ** The internal representation of digis and their matches
 ** is through CbmDigiVector, using std::vector as underlying container.
 **/
class CbmEventStore : public TObject {

public:
  /** @brief Default constructor
		 ** @param eventId  Unique event identifier
		 ** @param has Matches  True if matches to MC are stored
		 **/
  CbmEventStore(UInt_t eventId = 0, Bool_t hasMatches = kFALSE);


  /** @brief Copy constructor **/
  CbmEventStore(const CbmEventStore&);


  /** @brief Move constructor **/
  CbmEventStore(CbmEventStore&&) = default;


  /** @brief Destructor **/
  virtual ~CbmEventStore();


  /** @brief Add a digi object to the event
		 ** @param digi Pointer to digi object
		 **
		 ** The data referenced by the argument will be copied into
		 ** the internal arrays.
		 **
		 ** This method cannot be used if the event is set to contain matches.
		 **/
  template<class Digi>
  void AddDigi(const Digi* digi)
  {
    if (fHasMatches) {
      LOG(fatal) << "CbmEventStore: Trying to add digi without match!";
      return;
    }
    assert(digi);
    ECbmModuleId system = Digi::GetSystem();
    assert(system < ECbmModuleId::kNofSystems);
    if (!fDigis[system]) fDigis[system] = new CbmDigiVector<Digi>(kFALSE);
    auto digis = static_cast<CbmDigiContainer*>(fDigis.at(system));
    assert(digis);
    digis->AddDigi(digi, nullptr);
  }


  /** @brief Add a digi and its match object to the event
     ** @param digi Pointer to digi object
     ** @param match Pointer to match object
     **
     ** The data referenced by the arguments will be copied into
     ** the internal arrays.
     **
     ** This method cannot be used if the event is not set to contain matches.
     **/
  template<class Digi>
  void AddDigi(const Digi* digi, const CbmMatch* match)
  {
    if (!fHasMatches) {
      LOG(fatal) << "CbmEventStore: Trying to add digi without match!";
      return;
    }
    assert(digi);
    assert(match);
    ECbmModuleId system = Digi::GetSystem();
    assert(system < ECbmModuleId::kNofSystems);
    if (!fDigis[system]) fDigis[system] = new CbmDigiVector<Digi>(kTRUE);
    auto digis = static_cast<CbmDigiContainer*>(fDigis.at(system));
    assert(digis);
    digis->AddDigi(digi, match);
  }


  /** @brief Get a digi object
		 ** @param Index  Index of digi object for the given system in the event
		 ** @return Pointer to digi object.
		 **
		 ** The method will return a null pointer if the detector system is not
		 ** present or the index is out of range.
		 **/
  template<class Digi>
  const Digi* GetDigi(UInt_t index) const
  {
    ECbmModuleId system = Digi::GetSystem();
    assert(system < ECbmModuleId::kNofSystems);
    auto digis = static_cast<CbmDigiContainer*>(fDigis.at(system));
    assert(digis);
    return boost::any_cast<const Digi*>(digis->GetDigi(index));
  }


  /** @brief Get event ID
     ** @return Event identifier
     **/
  UInt_t GetEventId() const { return fEventId; }


  /** @brief Number of digis for a given system
     ** @param system System identifier [ECbmModuleId]
     ** @return Number of digis for system in event
     **/
  UInt_t GetNofDigis(ECbmModuleId system) const;


  /** @brief Presence of match objects
		 ** @param If true, match objects are stored
		 **/
  Bool_t HasMatches() const { return fHasMatches; }


  /** @brief Indicate whether event contains no digis
		 ** @return True is event is empty
		 **/
  Bool_t IsEmpty() const;


  /** @brief Match to MC event
		 ** @param[out] Reference to event match object
		 **
		 ** The method evaluates all digi matches and combines them
		 ** into an event match object.
		 **/
  void MatchToMC(CbmMatch& result) const;


  /** @brief Assignment operator **/
  CbmEventStore& operator=(const CbmEventStore&) = delete;


  /** String output **/
  std::string ToString() const;


private:
  UInt_t fEventId    = -1;      ///< Event identifier
  Bool_t fHasMatches = kFALSE;  ///< Presence of matches to MC
  //		TObjArray* fDigis = nullptr;   ///< Array of CbmDigiVector
  std::map<ECbmModuleId, TObject*> fDigis;  ///< Map of CbmDigiVector

  ClassDef(CbmEventStore, 2);
};

#endif /* CBMEVENTSTORE_H */
