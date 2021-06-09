/* Copyright (C) 2015-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

/*
 * CbmReadoutBuffer.h
 *
 *  Created on: 26.05.2015
 *      Author: vfriese
 */

#ifndef CBMREADOUTBUFFER_H
#define CBMREADOUTBUFFER_H 1

#include <FairRootManager.h>
#include <FairWriteoutBuffer.h>
#include <Logger.h>

#include <TClonesArray.h>
#include <TObject.h>

#include <cassert>
#include <map>
#include <vector>

// The data class must implement the methods Double_t GetTimeStart(),
// Double_t GetTimeStop(), and void SetTimeStop(Double_t),
// as well as a copy constructor (default one is fine).
// Data must inherit from TObject.


template<class Data>
class CbmReadoutBuffer : public FairWriteoutBuffer {

public:
  // ---------------------------------------------------------------------
  /** Default constructor
		 **
		 ** No output TClonesArray will be present. The buffered data have to
		 ** be fetched through ReadOutData.
		 **/
  CbmReadoutBuffer() : FairWriteoutBuffer(), fBuffer(), fBufferIt(), fOldIt(), fArray(nullptr), fWriteToArray(kFALSE) {}
  // ---------------------------------------------------------------------

  // ---------------------------------------------------------------------
  /** Constructor with branch and folder name
		 ** @param branchName  Name of branch in output tree
		 ** @param folderName  No idea what this is good for
		 ** @param persistence  If kTRUE (default), the branch will appear in the output file.
		 **
		 ** If this constructor is used, a branch with a TClonesArray will be created. Data
		 ** from the buffer will be copied to this array. Note that in this case,
		 ** ReadOutData will deliver no data (for the same readout time), since the
		 ** buffered data are deleted after being copied to the array.
		 **/
  CbmReadoutBuffer(TString branchName, TString folderName, Bool_t persistence = kTRUE)
    : FairWriteoutBuffer()
    , fBuffer()
    , fBufferIt()
    , fOldIt()
    , fArray(nullptr)
    , fWriteToArray(kTRUE)
  {

    if (FairRootManager::Instance()) {
      Data* data            = new Data();
      const char* className = data->ClassName();
      delete data;
      LOG(info) << "Class name is " << className;
      fArray = FairRootManager::Instance()->Register(branchName, className, folderName, persistence);
    }
  }
  // ---------------------------------------------------------------------
  CbmReadoutBuffer(const CbmReadoutBuffer&) = delete;
  CbmReadoutBuffer& operator=(const CbmReadoutBuffer&) = delete;


  // ---------------------------------------------------------------------
  /** Destructor
		 **
		 ** Cleans up remaining data in the buffer, if present, which should not
		 ** be the case if used properly.
		 **/
  virtual ~CbmReadoutBuffer()
  {
    if (fBuffer.size()) LOG(warn) << "Destroying non-empty readout buffer! Number of data: " << fBuffer.size();
    for (fBufferIt = fBuffer.begin(); fBufferIt != fBuffer.end(); fBufferIt++)
      if (fBufferIt->second) delete fBufferIt->second;
  }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Adding data to the TClonesArray
		 ** Abstract method from FairWriteoutBuffer; not needed in this
		 ** template implementation.
		 **/
  virtual void AddNewDataToTClonesArray(FairTimeStamp* /*data*/) {}
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Check data for consistency
		 ** (stop time should be larger than start time)
		 ** @param data  Data object
		 ** @value kTRUE is stop time is later than start time.
		 **/
  Bool_t CheckData(Data* data) { return (data->GetTimeStop() >= data->GetTimeStart()); }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Check interference of two data objects
		 ** @param data1,data2   Data objects
		 ** @value  kTRUE if objects interfere
		 **
		 ** Interference is present if the temporal extension of the objects
		 ** overlap.
		 **/
  Bool_t CheckInterference(Data* data1, Data* data2)
  {
    if (data1->GetTimeStop() < data2->GetTimeStart()) return kFALSE;
    if (data2->GetTimeStop() < data1->GetTimeStart()) return kFALSE;
    return kTRUE;
  }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Clear the output TClonesArray
		 ** Called at the end of the event from FairRootManager
		 **/
  virtual void DeleteOldData()
  {
    if (fArray) fArray->Delete();
  }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Erase data from data map
		 ** Abstract method from FairWriteoutBuffer; not needed in this
		 ** template implementation.
		 **/
  virtual void EraseDataFromDataMap(FairTimeStamp*) {}
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Fill data into the buffer
		 ** @param address Unique address
		 ** @param data    Pointer to data object
		 **
		 ** If, for the same address, other data are present in the buffer,
		 ** the time intervals of old and new data will be compared.
		 ** If there is no time overlap with already present data, the new data will
		 ** be inserted into the buffer. Otherwise, the method Modify is called
		 ** and the resulting, modified data are inserted into the buffer.
		 **/
  void Fill(UInt_t address, Data* data)
  {

    LOG(debug4) << "RO: Filling data at t = " << data->GetTimeStart() << " in address " << address;

    // --- Check data for consistency (start/stop time)
    if (!CheckData(data)) {
      LOG(fatal) << GetName() << ": inconsistent data input to Fill(). "
                 << "Start time is " << data->GetTimeStart() << " stop time is " << data->GetTimeStop();
    }

    // --- Loop over all present data with same address
    // --- Pick the first to which the interference criterion applies.
    Bool_t dataFound = kFALSE;
    for (fBufferIt = fBuffer.lower_bound(address); fBufferIt != fBuffer.upper_bound(address); fBufferIt++) {

      // --- Check interference of buffer data with old data. If so, jump
      // --- out of loop
      if (CheckInterference(data, fBufferIt->second)) {
        dataFound = kTRUE;
        break;
      }  //? Interference

    }  //# Data at same address

    // --- Action of interfering data found in the buffer
    if (dataFound) {

      // --- Call Modify method
      std::vector<Data*> newDataList;
      Merge(fBufferIt->second, data, newDataList);

      // --- Check return data list for non-interference
      // --- Modify has to be implemented in such as way as to return a
      // --- list (vector) of non-interfering data objects. This is
      // --- checked here to prevent unwanted behaviour
      // --- (e.g., endless loops).
      Int_t nData = newDataList.size();
      for (Int_t iData1 = 0; iData1 < nData; iData1++) {
        for (Int_t iData2 = iData1 + 1; iData2 < nData; iData2++) {
          if (CheckInterference(newDataList[iData1], newDataList[iData2]))
            LOG(fatal) << GetName() << ": Interfering data in return from Modify! "
                       << "Data 1: t(start) = " << newDataList[iData1]->GetTimeStart()
                       << " ns, t(stop) = " << newDataList[iData1]->GetTimeStop()
                       << ", Data 2: t(start) = " << newDataList[iData2]->GetTimeStart()
                       << " ns, t(stop) = " << newDataList[iData2]->GetTimeStop();
        }  //# data in vector (second loop)
      }    //# data in vector (first loop)

      // --- Remove old data from buffer and delete added data object
      if (fBufferIt->second) delete fBufferIt->second;
      fBuffer.erase(fBufferIt);
      delete data;

      // --- Fill new data to buffer, still checking for existing ones
      for (UInt_t iData = 0; iData < newDataList.size(); iData++) {
        LOG(debug4) << "RO: Filling modified data at address " << address
                    << ", t = " << newDataList[iData]->GetTimeStart() << " to " << newDataList[iData]->GetTimeStop();
        Fill(address, newDataList[iData]);
      }  //# result data of Modify

    }  //? Interfering data

    // --- No interference; just insert data into buffer
    else {

      fBuffer.insert(std::pair<UInt_t, Data*>(address, data));
      LOG(debug4) << "RO: Insert data at address " << address << ", t = " << data->GetTimeStart() << " to "
                  << data->GetTimeStop();

    }  //? No interference
  }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Fill data map
		 ** Abstract method from FairWriteoutBuffer; not needed in this
		 ** template implementation.
		 **/
  virtual void FillDataMap(FairTimeStamp*, double /*activeTime*/) {}
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Active time for a given data object
		 ** Abstract method from FairWriteoutBuffer; not needed in this
		 ** template implementation.
		 **/
  virtual double FindTimeForData(FairTimeStamp*) { return -1.; }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Number of data in the buffer
		 ** @return Number of data objects in buffer
		 **/
  virtual Int_t GetNData() { return fBuffer.size(); }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  virtual Int_t Merge(Data* oldData1, Data* oldData2, std::vector<Data*>& newDataList)
  {

    // Default prescription: return earlier data, skip the later one,
    // but set the stop time of the result to the maximum of the
    // two stop times
    Data* firstData   = (oldData1->GetTimeStart() < oldData2->GetTimeStart() ? oldData1 : oldData2);
    Double_t stopTime = std::max(oldData1->GetTimeStop(), oldData2->GetTimeStop());

    // Create new data object
    Data* newData = new Data(*firstData);
    newData->SetTimeStop(stopTime);

    // Add new data object to the result list
    newDataList.push_back(newData);

    return 1;  // Number of return data objects
  }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Write out all data in the buffer to the TClonesArray
		 **
		 ** All buffer data, irrespective of time, will be copied
		 ** to the output array and deleted from the buffer.
		 ** Called from Framework (FairRootManager)
		 */
  virtual void WriteOutAllData() { WriteOutData(-1.); }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Read out data from the buffer
		 ** @param[in]  time     Readout time. If < 0, all data will be read out.
		 ** @param[out] dataList Vector with pointers to data objects
		 ** @value      Number of data objects read out
		 **
		 ** Data interface alternative to the TClonesArray. Data from the buffer
		 ** with stop time before the specified readout time will be delivered
		 ** and removed from the buffer.
		 ** The object ownership is passed to the consumer, who is responsible
		 ** for destroying the data objects in the data vector.
		 **/
  Int_t ReadOutData(Double_t time, std::vector<Data*>& dataList)
  {

    LOG(debug) << "RO Buffer: read out at t = " << time << ", buffer size " << fBuffer.size();
    if (!dataList.empty()) dataList.clear();

    Int_t nData = 0;
    fBufferIt   = fBuffer.begin();
    while (fBufferIt != fBuffer.end()) {

      UInt_t address = fBufferIt->first;
      Data* data     = fBufferIt->second;

      LOG(debug4) << "Buffer entry: " << address << " " << data->GetTimeStart();

      // --- Assert valid data object
      assert(data);

      // --- Skip if stop time after readout time
      if (time >= 0. && data->GetTimeStop() > time) {
        fBufferIt++;
        continue;
      }

      // --- Fill output vector with data pointers
      LOG(debug4) << "RO: read out data at t = " << data->GetTimeStart();
      dataList.push_back(data);

      // --- Remove element from buffer
      fOldIt = fBufferIt;
      fBufferIt++;
      fBuffer.erase(fOldIt);

      nData++;
    }  //# elements in buffer

    return nData;
  }
  // ---------------------------------------------------------------------


  // ---------------------------------------------------------------------
  /** Write out data to the TClonesArray
		 ** @param time  Readout time. If < 0, all data will be written out.
		 **
		 ** All data with stop time before the readout time will be copied
		 ** to the output array and deleted from the buffer.
		 ** Called from Framework (FairRootManager).
		 ** Note that this method has no effect if fWriteToArray is kFALSE,
		 ** which is the default.
		 **/
  virtual void WriteOutData(Double_t time)
  {

    if (!fWriteToArray) return;

    LOG(info) << "RO Buffer: write out at t = " << time << ", buffer size " << fBuffer.size() << ", array size "
              << fArray->GetEntriesFast();

    Int_t nDataWritten = 0;
    fBufferIt          = fBuffer.begin();
    while (fBufferIt != fBuffer.end()) {

      UInt_t address = fBufferIt->first;
      Data* data     = fBufferIt->second;

      LOG(debug4) << "Buffer entry: " << address << " " << data->GetTimeStart();

      // --- Assert valid data object
      assert(data);

      // --- Skip if stop time after readout time
      if (time >= 0. && data->GetTimeStop() > time) {
        fBufferIt++;
        continue;
      }

      // --- Copy data to TClonesArray
      if (fArray) {
        LOG(debug4) << "RO: Writing to array: t = " << data->GetTimeStart();
        new ((*fArray)[fArray->GetEntriesFast()]) Data(*data);
      }

      // --- Delete data and remove element from buffer
      delete data;
      fOldIt = fBufferIt;
      fBufferIt++;
      fBuffer.erase(fOldIt);

      nDataWritten++;

    }  //# buffer elements

    LOG(info) << "RO Buffer: wrote " << nDataWritten << " data, buffer size " << fBuffer.size() << ", array size "
              << fArray->GetEntriesFast();
  }
  // ---------------------------------------------------------------------


protected:
  std::multimap<UInt_t, Data*> fBuffer;                       //!
  typename std::multimap<UInt_t, Data*>::iterator fBufferIt;  //!
  typename std::multimap<UInt_t, Data*>::iterator fOldIt;     //!
  TClonesArray* fArray;                                       //!
  Bool_t fWriteToArray;
};

#endif /* CBMREADOUTBUFFER_H_ */
