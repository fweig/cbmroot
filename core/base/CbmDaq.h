/** @file CbmDaq.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 20 July 2012
 **
 **/


#ifndef CBMDAQ_H
#define CBMDAQ_H 1

#include <Rtypes.h>          // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>      // for Int_t, Double_t, Bool_t, kFALSE, ULong64_t
#include <TStopwatch.h>      // for TStopwatch

#include <FairTask.h>        // for FairTask, InitStatus

#include <string>            // for string
#include <map>               // for map
#include <utility>           // for pair

#include "CbmMCEventList.h"  // for CbmMCEventList
#include "CbmDefs.h"         // for ECbmModuleId
class CbmDigi;
class CbmDigitizeBase;
class CbmTimeSlice;
class TClonesArray;


/** @class CbmDaq
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 20 July 2012
 ** @brief CBM task class for filling digis into time slices
 **
 ** The CbmDaq collects raw data (digis) from various input sources
 ** (detectors), sorts them w.r.t. time and fills time slices.
 ** The digis in one time slice are written to TCLonesArrays as branches of
 ** the output tree. One tree entry corresponds to one time slice (interval),
 ** the duration of which can be adjusted.
 **/
class CbmDaq : public FairTask
{

  public:

    /** @brief Constructor
     ** @param eventMode  If true, run in event-by-event mode
     **
     ** By default, the DAQ will run in time-based mode with flexible
     ** time slices (all data into one time slice). To choose the
     ** event-by-event mode, set the argument to true; then,
     ** data will be grouped into events, one event per time slice.
     ** To select the regular mode with time slices of fixed duration,
     ** use the constructor with Double_t argument.
     **/
    CbmDaq(Bool_t eventMode = kFALSE);


    /** @brief Constructor for regular DAQ mode
     ** @param tslength  Duration of time slices [ns]
     **/
    CbmDaq(Double_t tsLength);


    /** @brief Destructor   **/
    ~CbmDaq();


    /** @brief Task execution **/
    virtual void Exec(Option_t* opt);


   /** @brief Initialisation **/
    virtual InitStatus Init();


    /** @brief Set the DAQ buffer latency
     ** @param time  Buffer latency [ns]
     **
     ** To decide when a time-slice can be closed (no more data in the
     ** time-slice interval will come), the DAQ takes into account
     ** a certain time-disordering of digis. The latency is the
     ** maximal time difference between the current (event) time
     ** and the time-stamp of the digi. It should
     ** at least be the maximum dead time of all detectors plus
     ** some safety margin accounting for the time resolution of the
     ** detectors.
     ** The current default of 2,000 ns corresponds to the STS with
     ** dead time of 800 ns and time resolution of 5 ns.
     **/
    void SetLatency(Double_t time) { fLatency = time; }


    /** @brief Set the digitizer for a given system
     ** @param system  System Id (ECbmModuleId)
     ** @param digitizer  Pointer to digitizer instance
     **/
    void SetDigitizer(ECbmModuleId system, CbmDigitizeBase* digitizer);


    /** @brief Set the time-slice length
     ** @param length  Length of a time-slice [ns]
     **/
    void SetTimeSliceLength(Double_t length) {
      fTimeSliceLength = length;
    }


    /** @brief Store all time-slices
     ** @param choice If kTRUE; also empty slices will be stored.
     **
     ** By default, only time slices containing data are filled into the tree.
     **/
    void StoreAllTimeSlices(Bool_t choice = kTRUE) {
      fStoreEmptySlices = choice;
    }


  private:

    Bool_t   fIsEventByEvent;      ///< Flag for event-by-event mode
    Double_t fTimeSliceLength  ;   ///< Time-slice length [ns]
    Double_t fLatency;             ///< Maximal time disorder of input data [ns]
    Bool_t   fStoreEmptySlices;    ///< Flag to store also empty time slices
    Double_t fTimeEventPrevious;   ///< Time of previous event [ns]

    Int_t fNofEvents;             ///< Number of processed events
    Int_t fNofDigis;              ///< Total number of processed digis
    Int_t fNofDigisIgnored;       ///< Number of ignored digis
    Int_t fNofTimeSlices;         ///< Number of time slices
    Int_t fNofTimeSlicesEmpty;    ///< Number of empty time slices
    Double_t fTimeDigiFirst;      ///< Time of first digi
    Double_t fTimeDigiLast;       ///< Time of last digi
    Double_t fTimeSliceFirst;     ///< Start time of first time slice
    Double_t fTimeSliceLast;      ///< Stop time of last time slice

    TStopwatch fTimer;              //! Stop watch
    std::map<ECbmModuleId, TClonesArray*> fDigis;  //! Output arrays (digis)
    std::map<ECbmModuleId, CbmDigitizeBase*> fDigitizers; //!  Array of registered digitizers
    CbmTimeSlice* fTimeSlice;       //! Current time slice
     CbmMCEventList fEventList;     //!  MC event list (all)
    CbmMCEventList* fEventsCurrent; //! MC events for current time slice


    /** First and last event in current time slice for each input **/
    std::map<Int_t, std::pair<Int_t, Int_t>> fEventRange; //!


    /** @brief Check the output arrays for being time-sorted
     ** @return kTRUE if all outputs are time-sorted
     **/
    Bool_t CheckOutput() const;


    /** Close the current time slice
     ** The current slice is filled to the tree. It is then reset
     ** to the next time slice interval.
     */
    void CloseTimeSlice();


    /** Copy the MC events contributing to the current time slice
     ** to the output array.
     **
     ** @return  Number of MC events for this time slice
     **/
    Int_t CopyEventList();


    /** @brief Copy data (digi) from the DaqBuffer into the output array
     ** @param digi  Pointer to digi object
     **/
    void FillData(CbmDigi* digi);
    //TODO: Obsolete, remove


    /** Move data from the buffer into the current time slice
     ** @param checkTime If true, data will be moved up to fillTime.
     ** @param fillTime  Time up to which data are moved
     ** @return Number of digis filled into the time slice
     **/
    ULong64_t FillTimeSlice(Bool_t timeLimit, Double_t fillTime = -1.);


    /** @brief Size of DAQ buffers
     ** @value Sum of number of data in all DAQ buffers
     **/
    ULong64_t GetBufferSize() const;


    /** @brief Debug output of DAQ buffer status
     ** @value String with status of DAQ buffers
     **/
    std::string GetBufferStatus(Bool_t verbose = kFALSE) const;


    /** @brief Time of first datum in DAQ buffers
     ** @value Minimum time stamp in all DAQ buffers
     **/
    Double_t GetBufferTimeFirst() const;


    /** @brief Time of last datum in DAQ buffers
     ** @value Maximum time stamp in all DAQ buffers
     **/
    Double_t GetBufferTimeLast() const;


    /** @brief Check for empty DAQ buffers
     ** @brief value True if all DAQ buffers are empty
     **/
    Bool_t IsDaqBufferEmpty() const;


    /** Screen log of the range of MC events contributing to the
     ** current time slice
     **/
    void PrintCurrentEventRange() const;


    /** @brief Start a new time slice in the output tree **/
    void StartNextTimeSlice();


    /** At end of run: Process the remaining data in the CbmDaqBuffer  **/
    virtual void Finish();


    /** Copy constructor and assignment operator are not allowed. **/
    CbmDaq(const CbmDaq&) = delete;
    CbmDaq& operator=(const CbmDaq&) = delete;
    
    ClassDef(CbmDaq, 4);

};

#endif /* CBMDAQ_H */
