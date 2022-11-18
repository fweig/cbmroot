/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmDigitize.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 31.01.2020
 **/

#ifndef CBMDIGITIZE_H
#define CBMDIGITIZE_H 1

#include "CbmDaq.h"
#include "CbmDigitizeBase.h"
#include "CbmMatch.h"
#include "CbmTimeSlice.h"

#include <FairRootManager.h>
#include <FairTask.h>
#include <Logger.h>  // for LOG

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Bool_t, ULong64_t, kFALSE, kTRUE

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>


/** @class CbmDigitize
 ** @brief Base class template for CBM digitisation tasks
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 31 January 2020
 **
 ** Concrete digitisers should concretise with their digi class as template
 ** parameters.
 **
 ** The requirement for the digi class is to implement Double_t GetTime().
 **/
template<class Digi>
class CbmDigitize : public CbmDigitizeBase {

public:
  /** @brief Short for data to be handled (pair of digi and match) **/
  typedef std::pair<std::unique_ptr<Digi>, std::unique_ptr<CbmMatch>> Data;


  /** @brief Default constructor **/
  CbmDigitize() {};


  /** @brief Constructor with name
     ** @param name Task name
     **/
  CbmDigitize(const char* name, const char* branchName = "") : CbmDigitizeBase(name), fBranchName(branchName) {};


  /** @brief Destructor **/
  virtual ~CbmDigitize() {};


  // --------------------------------------------------------------------------
  /** @brief Check the output for being time-sorted **/
  Bool_t CheckOutput()
  {
    assert(fDigis);
    if (fDigis->empty()) return kTRUE;
    Double_t prevTime = fDigis->begin()->GetTime();
    for (auto it = (fDigis->begin())++; it != fDigis->end(); it++) {
      if (it->GetTime() < prevTime) {
        LOG(error) << GetName() << ": CheckBuffer: Found digi at t = " << it->GetTime()
                   << " ns after digi at t = " << prevTime << " ns";
        return kFALSE;
        break;
      }
      prevTime = it->GetTime();
    }
    return kTRUE;
  }
  // --------------------------------------------------------------------------


  // --------------------------------------------------------------------------
  /** @brief Clear the output arrays **/
  void ClearOutput()
  {
    if (fDigis) fDigis->clear();
    if (fCreateMatches)
      if (fMatches != nullptr) fMatches->clear();
  }
  // --------------------------------------------------------------------------


  // --------------------------------------------------------------------------
  /** @brief Move data from the DaqBuffer into the current time slice.
     ** @param timeSlice  Pointer to current time slice object
     ** @value Number of digi objects filled into the time slice.
     **
     ** For regular time slices, all data with time stamp within the interval
     ** of the current time slice are moved from the buffer to the time slice.
     ** For time slices of type kFlexible or kEvent, all data will be moved.
     **/
  ULong64_t FillTimeSlice(CbmTimeSlice* timeSlice) { return FillTimeSlice(timeSlice, kFALSE, -1.); }
  // --------------------------------------------------------------------------


  // --------------------------------------------------------------------------
  /** @brief Move data from the DaqBuffer into the current time slice.
     ** @param timeSlice  Pointer to current time slice object
     ** @param fillTime Time up to which data will be moved [ns]
     ** @value Number of digi objects filled into the time slice.
     **
     ** Move data with time stamp up to fillTime from the buffer to the time
     ** slice. For regular time slices, only data with time stamp within
     ** the time slice interval will be moved. For time slices of type
     ** kFlexible or kEvent, all data up to fillTime will be moved.
     **/
  ULong64_t FillTimeSlice(CbmTimeSlice* timeSlice, Double_t fillTime)
  {
    return FillTimeSlice(timeSlice, kTRUE, fillTime);
  }
  // --------------------------------------------------------------------------


  // --------------------------------------------------------------------------
  /** @brief Size of DAQ buffer
     ** @value Number of data in the DAQ buffer
     **/
  ULong64_t GetDaqBufferSize() const { return fDaqBuffer.size(); }
  // --------------------------------------------------------------------------


  // --------------------------------------------------------------------------
  /** @brief Debug output of DAQ buffer status
     ** @value String with status of DAQ buffer
     **/
  std::string GetDaqBufferStatus() const
  {
    std::stringstream ss;
    ss << "Status DAQ buffer: " << GetDaqBufferSize() << " data from t = " << GetDaqBufferTimeFirst() << " to "
       << GetDaqBufferTimeLast() << " ns";
    return ss.str();
  }
  // --------------------------------------------------------------------------


  // --------------------------------------------------------------------------
  /** @brief Time stamp of first data in the DAQ buffer
     ** @value Time stamp of first data in the DAQ buffer
     **/
  Double_t GetDaqBufferTimeFirst() const
  {
    if (fDaqBuffer.empty()) return -1.;
    return fDaqBuffer.begin()->first;
  }
  // --------------------------------------------------------------------------


  // --------------------------------------------------------------------------
  /** @brief Time stamp of last data in the DAQ buffer
     ** @value Time stamp of last data in the DAQ buffer
     **/
  Double_t GetDaqBufferTimeLast() const
  {
    if (fDaqBuffer.empty()) return -1.;
    return (--fDaqBuffer.end())->first;
  }
  // --------------------------------------------------------------------------


  // --------------------------------------------------------------------------
  /** @brief Register the output arrays
     **
     ** Arrays for the digis and the match objects will be created and
     ** registered as output to the ROOT tree. The current implementation
     ** uses std::vector as container.
     **/
  void RegisterOutput()
  {

    // --- Get FairRootManager instance
    FairRootManager* ioman = FairRootManager::Instance();
    assert(ioman);

    // --- Digi branch name. If not set otherwise (through constructor), it
    // --- equals the digi class name minus the leading "Cbm".
    TString digiBranchName = fBranchName;
    if (digiBranchName.IsNull()) {
      TString digiClassName = Digi::GetClassName();
      if (digiClassName.BeginsWith("Cbm")) digiBranchName = digiClassName(3, digiClassName.Length());
    }  //? No branch name set via constructor


    // --- Branch for digis
    fDigis = new std::vector<Digi>();
    ioman->RegisterAny(digiBranchName.Data(), fDigis, IsOutputBranchPersistent(digiBranchName));
    LOG(info) << GetName() << ": Registered branch " << digiBranchName;

    // --- Branch for matches
    if (fCreateMatches) {
      TString matchBranchName = digiBranchName + "Match";
      fMatches                = new std::vector<CbmMatch>();
      ioman->RegisterAny(matchBranchName.Data(), fMatches, IsOutputBranchPersistent(matchBranchName));
      LOG(info) << GetName() << ": Registered branch " << matchBranchName;
    }
  }
  // --------------------------------------------------------------------------


  // --------------------------------------------------------------------------
  /** @brief Send a digi and the corresponding match object to the DAQ
     ** @param time  Global time of the digi
     ** @param digi  Pointer to digi object (template parameter)
     ** @param match Pointer to match object
     **
     ** Time is passed as a seperate parameter because the global time might
     ** be too large for some digi classes to store internally. So digis are not
     ** required to have a valid timestamp at this point. Later on when the
     ** time slices are known, the timestamp is overwritten with the relative time
     ** to the beginning of the time slice.
     **
     ** TODO: The interface should be unique pointers, meaning
     ** that the digitisers have to create objects by unique pointers
     ** from the start.
     **/
  void SendData(Double_t time, Digi* digi, CbmMatch* match = nullptr)
  {
    std::unique_ptr<Digi> tmpDigi(digi);
    std::unique_ptr<CbmMatch> tmpMatch(match);
    fDaqBuffer.insert(make_pair(time, std::make_pair(std::move(tmpDigi), std::move(tmpMatch))));
  }
  // --------------------------------------------------------------------------

private:
  TString fBranchName             = "";       ///< Output branch name
  std::vector<Digi>* fDigis       = nullptr;  //! Output array (Digi)
  std::vector<CbmMatch>* fMatches = nullptr;  //! Output array (CbmMatch)


private:
  /** DAQ buffer. Here, the digis and matches are buffered until they are
      ** filled into the time slice output (ROOT branch).
      ** The map key is the digi time. **/
  std::multimap<double, Data> fDaqBuffer;  //!


  // --------------------------------------------------------------------------
  /** @brief Move data from the DaqBuffer into the current time slice.
      ** @param timeSlice  Pointer to current time slice object
      ** @param fillTime Time up to which data will be moved [ns]
      ** @value Number of digi objects filled into the time slice.
      **
      ** For regular time slices, all data with time stamp within the interval
      ** of the current time slice are moved from the buffer to the time slice.
      ** For time slices of type kFlexible or kEvent, all data will be moved.
      **
      ** If checkLimit is selected, only data with time stamp less than fillTime
      ** are moved.
      **/
  ULong64_t FillTimeSlice(CbmTimeSlice* timeSlice, Bool_t checkLimit, Double_t fillTime)
  {

    assert(timeSlice);
    ULong64_t nData     = 0;
    Double_t tMin       = timeSlice->GetStartTime();
    Double_t tMax       = timeSlice->GetEndTime();
    Bool_t checkMinTime = kTRUE;
    Bool_t checkMaxTime = kTRUE;
    if (timeSlice->IsRegular()) {
      if (checkLimit && fillTime < tMax) tMax = fillTime;
    }
    else if (timeSlice->IsFlexible() || timeSlice->IsEvent()) {
      checkMinTime = kFALSE;
      checkMaxTime = checkLimit;
      tMax         = fillTime;
      tMin         = 0;  // Do not make digi time relative for these types of time slices.
    }
    else {
      LOG(fatal) << GetName() << ": Unknown time-slice type!";
    }

    // This implementation makes use of the fact that the data in the
    // DAQ buffer are time-sorted.
    auto it = fDaqBuffer.begin();
    while (it != fDaqBuffer.end() && ((!checkMaxTime) || it->first < tMax)) {

      // For regular time-slices, discard digis with negative times.
      // The first time slice starts at t = 0. All data before are just
      // not recorded.
      if (timeSlice->IsRegular() && it->first < 0.) {
        it++;
        continue;
      }

      // Digi times before the start of the current time slice
      // should not happen.
      assert((!checkMinTime) || it->first >= tMin);

      // TODO: This implementation uses the implicit copy constructor and
      // manual removal from the source vector. There might be a more
      // elegant way.
      assert(fDigis);
      Double_t globalTime      = it->first;
      std::unique_ptr<Digi>& d = it->second.first;
      assert(d);
      d->SetTime(globalTime - tMin);
      fDigis->push_back(*d);
      if (fCreateMatches) {
        assert(fMatches);
        assert(it->second.second);
        fMatches->push_back(*(it->second.second));
      }

      // Register datum to the time slice header
      if (fCreateMatches) timeSlice->RegisterData(GetSystemId(), it->first, fMatches->at(fMatches->size() - 1));
      else
        timeSlice->RegisterData(GetSystemId(), it->first);

      nData++;
      it++;
    }

    // Remove the corresponding elements from the buffer
    fDaqBuffer.erase(fDaqBuffer.begin(), it);

    return nData;
  }
  // --------------------------------------------------------------------------


  ClassDef(CbmDigitize, 1);
};

#endif /* CBMDIGITIZE_H */
