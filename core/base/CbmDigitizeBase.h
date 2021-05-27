/** @file CbmDigitizeBase.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 01.06.2018
 **/

#ifndef CBMDIGITIZEBASE_H
#define CBMDIGITIZEBASE_H 1

#include "CbmDefs.h"  // for ECbmModuleId

#include <FairTask.h>  // for FairTask

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Bool_t, Double_t, Int_t, kTRUE, ULong64_t

#include <string>  // for string
class CbmTimeSlice;

/** @class CbmDigitizeBase
 ** @brief Abstract base class for CBM digitisation tasks
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 01 June 2018
 **
 ** This abstract class defines the interfaces of the digitizers to the DAQ.
 **/
class CbmDigitizeBase : public FairTask {

public:
  /** @brief Constructor **/
  CbmDigitizeBase();


  /** @brief Constructor with name
     ** @param name Task name
     **/
  CbmDigitizeBase(const char* name);


  /** @brief Destructor **/
  virtual ~CbmDigitizeBase();


  /** @brief Check the output arrays
     ** @return kTRUE if output is OK
     **
     ** Will be called by the DAQ before filling the output tree.
     ** A typical check to be implemented is e.g. whether the output
     ** array is time-sorted.
     **/
  virtual Bool_t CheckOutput() = 0;


  /** @brief Clear the output arrays
     **
     ** Will be called by the DAQ at the beginning of a new time slice
     ** (after a tree fill).
     **/
  virtual void ClearOutput() = 0;


  /** @brief Fill custom data into time slice
     ** @param fillTime Time until data can be filled
     ** @param limit If kTRUE, only data up to fillTime will be treated; otherwise, all.
     **
     ** This method allows the digitizer to implement additional functionality
     ** than writing digis and match objects. It will be called from CbmDaq.
     **/
  virtual void FillCustomData(Double_t /*fillTime*/, Bool_t /*limit*/ = kTRUE) {}


  /** @brief Fill data into the current time slice
     ** @param timeSlice  Pointer to current time slice
     ** @return Number of data filled into the time slice
     **
     ** Data with time stamp in the interval defined by the current time slice
     ** are filled into the output array.
     **/
  virtual ULong64_t FillTimeSlice(CbmTimeSlice* timeSlice) = 0;


  /** @brief Fill data into the current time slice
     ** @param timeSlice  Pointer to current time slice
     ** @param tMax  Maximum time stamp of data to be filled
     ** @return Number of data filled into the time slice
     **
     ** Data with time stamp in the interval defined by the current time slice,
     ** but less than the time tMax are filled into the output arrays.
     **/
  virtual ULong64_t FillTimeSlice(CbmTimeSlice*, Double_t tMax) = 0;


  /** @brief Get event information
     **
     ** MC input number, entry number and event time are taken from
     ** FairEventHeader and stored in the private data members for
     ** convenience. Note that the MC entry number need not coincide with the
     ** event number, for instance if the run does not start with the first
     ** MC entry, or in the case of mixed MC inputs to digitization.
     **/
  void GetEventInfo();


  /** @brief Current event time
     ** @value Start time of current event [ns]
     **/
  Double_t GetEventTime() const { return fCurrentEventTime; }


  /** @brief Size of DAQ buffer
     ** @value Number of data in the DAQ buffer
     **/
  virtual ULong64_t GetDaqBufferSize() const = 0;


  /** @brief Status of DAQ buffer
     ** @value Status string of the DAQ buffer
     **/
  virtual std::string GetDaqBufferStatus() const = 0;


  /** @brief Time of first datum in DAQ buffer
     ** @value Time of first datum in DAQ buffer
     **/
  virtual Double_t GetDaqBufferTimeFirst() const = 0;


  /** @brief Detector system ID
     ** @return Detector system ID [ECbmModuleId]
     **/
  virtual ECbmModuleId GetSystemId() const = 0;


  /** @brief Time of last datum in DAQ buffer
     ** @value Time of last datum in DAQ buffer
     **/
  virtual Double_t GetDaqBufferTimeLast() const = 0;


  /** @brief Set creation of links to MC
     ** @param Choice If kTRUE, the match objects will be created
     **/
  void SetCreateMatches(Bool_t choice = kTRUE) { fCreateMatches = choice; }


  /** @brief Set event-by-event mode
     ** @param Choice If kTRUE, the digitizer will run in event-by-event mode
     **/
  void SetEventMode(Bool_t choice = kTRUE) { fEventMode = choice; }


  /** @brief Set production of inter-event noise
     ** @param Choice If kTRUE, the digitizer will produce noise
     **/
  void SetProduceNoise(Bool_t choice = kTRUE) { fProduceNoise = choice; }


protected:
  Bool_t fEventMode;           /// Flag for event-by-event mode
  Bool_t fProduceNoise;        /// Flag for production of inter-event noise
  Bool_t fCreateMatches;       /// Flag for creation of links to MC
  Int_t fCurrentInput;         /// Number of current input
  Int_t fCurrentEvent;         /// Number of current MC event
  Int_t fCurrentMCEntry;       /// Number of current MC entry
  Double_t fCurrentEventTime;  /// Time of current MC event [ns]


private:
  /** @brief Copy constructor forbidden **/
  CbmDigitizeBase(const CbmDigitizeBase&) = delete;


  /** @brief Assignment operator forbidden **/
  void operator=(const CbmDigitizeBase&) = delete;


  ClassDef(CbmDigitizeBase, 3);
};

#endif /* CBMDIGITIZEBASE_H */
