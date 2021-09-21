/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmRawEvent.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 25.03.2021
 **/


#ifndef CBMRAWEVENT_H
#define CBMRAWEVENT_H 1

#include "CbmDefs.h"  // for ECbmDataType, ECbmModuleId::kStsTrack
//#include "CbmMatch.h"  // for CbmMatch
#include "CbmMuchDigi.h"
#include "CbmMvdDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"
//#include "CbmVertex.h"  // for CbmVertex, found in core/data/global

//#include <Rtypes.h>          // for THashConsistencyHolder, ClassDef
//#include <RtypesCore.h>      // for Double_t, UInt_t, Int_t
//#include <TMatrixFSymfwd.h>  // for TMatrixFSym
//#include <TObject.h>         // for TObject


//#include <map>  // for map, map<>::mapped_type
//#include <span>
//#include <string>  // for string
//#include <utility>
#include <vector>  // for vector


/** @class CbmRawEvent
 ** @brief Data class as collections of digis for each detector system
 ** @author V.Friese <v.friese@gsi.de>
 ** @version 1.0
 **
 **/
class CbmRawEvent : public TObject {

public:
  /** @brief Default constructor **/
  CbmRawEvent() {};


  /** @brief Constructor with event number and time
   ** @param number    Event number
   ** @param time      Event trigger time [ns]
   **/
  CbmRawEvent(uint64_t number, Double_t time = 0) : fNumber(number), fTime(time) {};


  /** @brief Copy constructor **/
  CbmRawEvent(const CbmRawEvent&) = default;


  /** @brief Move constructor **/
  CbmRawEvent(CbmRawEvent&&) = default;


  /** @brief Assignment operator **/
  CbmRawEvent& operator=(const CbmRawEvent&) = default;


  /** @brief Destructor **/
  virtual ~CbmRawEvent() {};


  /** @brief Add a digi to the event
   ** @param data Digi
   ** TODO: Somehow, this does not work, so dedicated methods were implemented.
   **/
  template<class Digi>
  void AddDigi(ECbmModuleId system, Digi digi)
  {
    switch (system) {
      case ECbmModuleId::kMvd: fDigisMvd.push_back(digi); break;
      case ECbmModuleId::kSts: fDigisSts.push_back(digi); break;
      case ECbmModuleId::kRich: fDigisRich.push_back(digi); break;
      case ECbmModuleId::kMuch: fDigisMuch.push_back(digi); break;
      case ECbmModuleId::kTrd: fDigisTrd.push_back(digi); break;
      case ECbmModuleId::kTof: fDigisTof.push_back(digi); break;
      case ECbmModuleId::kPsd: fDigisPsd.push_back(digi); break;
      default: break;
    }
  }


  void AddDigiMvd(CbmMvdDigi digi) { fDigisMvd.push_back(digi); }
  void AddDigiSts(CbmStsDigi digi) { fDigisSts.push_back(digi); }
  void AddDigiRich(CbmRichDigi digi) { fDigisRich.push_back(digi); }
  void AddDigiMuch(CbmMuchDigi digi) { fDigisMuch.push_back(digi); }
  void AddDigiTrd(CbmTrdDigi digi) { fDigisTrd.push_back(digi); }
  void AddDigiTof(CbmTofDigi digi) { fDigisTof.push_back(digi); }
  void AddDigiPsd(CbmPsdDigi digi) { fDigisPsd.push_back(digi); }


  /** @brief Read-only access to a single digi
   ** @param system  System identifier (ECbmModuleId)
   ** @param index Index of digi in its vector
   ** @return Reference to digi object
   **/
  template<class Digi>
  const Digi& GetDigi(ECbmModuleId system, size_t index) const
  {
    switch (system) {
      case ECbmModuleId::kMvd: return fDigisMvd.at(index); break;
      case ECbmModuleId::kSts: return fDigisSts.at(index); break;
      case ECbmModuleId::kRich: return fDigisRich.at(index); break;
      case ECbmModuleId::kMuch: return fDigisMuch.at(index); break;
      case ECbmModuleId::kTrd: return fDigisTrd.at(index); break;
      case ECbmModuleId::kTof: return fDigisTof.at(index); break;
      case ECbmModuleId::kPsd: return fDigisPsd.at(index); break;
      default: break;
    }
  }


  /** @brief Read-only access to digi vectors
   ** @param system  System identifier (ECbmModuleId)
   ** @return Reference to digi vector for the specified system
   **/
  template<class Digi>
  const std::vector<Digi>& GetDigis(ECbmModuleId system) const
  {
    switch (system) {
      case ECbmModuleId::kMvd: return (&fDigisMvd); break;
      case ECbmModuleId::kSts: return (&fDigisSts); break;
      case ECbmModuleId::kRich: return (&fDigisRich); break;
      case ECbmModuleId::kMuch: return (&fDigisMuch); break;
      case ECbmModuleId::kTrd: return (&fDigisTrd); break;
      case ECbmModuleId::kTof: return (&fDigisTof); break;
      case ECbmModuleId::kPsd: return (&fDigisPsd); break;
      default: break;
    }
  }


  /** @brief Number of digis for a given system
   ** @param systemId   System identifier (ECbmModuleId)
   ** @return Number of digis for the specified system
   **/
  size_t GetNofDigis(ECbmModuleId) const;


  /** @brief Total number of digis (all systems)
   ** @return Number of digis in event
   **/
  size_t GetNofDigis() const;


  /** @brief Event number
   ** @return Event number
	 **/
  uint64_t GetNumber() const { return fNumber; }


  /** @brief Event time
   ** @return Event trigger time [ns]
   **/
  Double_t GetTime() const { return fTime; }


  /** @brief Set event time
   ** @param time  Event trigger time [ns]
	 **/
  void SetTime(Double_t time) { fTime = time; }


  /** @brief String output **/
  std::string ToString() const;


private:
  /** Event meta data **/
  uint64_t fNumber;  ///< Event number
  double fTime;      ///< Trigger time for this event [ns]

  std::vector<CbmMvdDigi> fDigisMvd   = {};
  std::vector<CbmStsDigi> fDigisSts   = {};
  std::vector<CbmRichDigi> fDigisRich = {};
  std::vector<CbmMuchDigi> fDigisMuch = {};
  std::vector<CbmTrdDigi> fDigisTrd   = {};
  std::vector<CbmTofDigi> fDigisTof   = {};
  std::vector<CbmPsdDigi> fDigisPsd   = {};


  ClassDef(CbmRawEvent, 1);
};

#endif /* CBMRAWEVENT_H_ */
