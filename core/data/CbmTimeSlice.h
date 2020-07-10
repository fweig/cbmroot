/** @file CbmTimeSlice.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17 July 2012
 **/

#ifndef CBMTIMESLICE_H
#define CBMTIMESLICE_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t, kFALSE
#include <TNamed.h>      // for TNamed

#include <map>     // for map
#include <string>  // for string

#include "CbmDefs.h"   // for ECbmModuleId
#include "CbmMatch.h"  // for CbmMatch

// boost
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>


/** @class CbmTimeSlice
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17 July 2012
 ** @brief Bookkeeping of time-slice content
 **/
class CbmTimeSlice : public TNamed {

public:
  /** @brief Time-slice type enumerator **/
  enum EType {
    kRegular  = 0,  /// Regular time slice with fixed-size time interval
    kFlexible = 1,  /// Flexible time slice; no fixed time limits
    kEvent    = 2   /// Time slice representing one event; no fixed time limits
  };


  /** @brief Constructor without time interval
     **
     ** Use this constructor for the modes kFlexible and kEvent.
     **/
  CbmTimeSlice(EType type = kFlexible);


  /** Standard constructor for a regular (fixed-length) time-slice
     ** @param start    Start time of time slice [ns]
     ** @param duration Duration of time slice [ns]
     */
  CbmTimeSlice(Double_t start, Double_t duration);


  /** Destructor **/
  ~CbmTimeSlice();


  /** @brief Add data to time-slice
     ** @param detector  System ID (ECbmModuleId)
     **
     ** The respective counter will be incremented.
     **/
  void AddData(ECbmModuleId detector) {
    fNofData[detector]++;
    fIsEmpty = kFALSE;
  }
  // TODO: Obsolete, remove

  /** @brief Add data with time to time-slice
     ** @param detector  System ID (ECbmModuleId)
     ** @param time      Data time [ns]
     ** @value kFALSE if time is out of time-slice bounds; else kTRUE
     **
     ** The respective counter will be incremented.
     ** Time of data is checked with time-slice bounds.
     ** Time of first and last data are updated.
     **/
  Bool_t AddData(ECbmModuleId detector, Double_t time);
  // TODO: Obsolete, remove


  /** @brief Get size of raw data container for given detector
     ** @param detector  system ID (ECbmModuleId)
     ** @value Size of raw data container (number of digis)
     **/
  Int_t GetNofData(ECbmModuleId detector) const;


  /** Duration of time slice
     **
     ** @return duration [ns]
     **/
  Double_t GetLength() const { return fLength; }


  /** Get match object
     ** @return Match object
     **/
  const CbmMatch& GetMatch() const { return fMatch; }


  /** Start time of time slice
     ** @return start time [ns]
     **
     ** If the type of the time slice is kEvent, the method returns the
     ** event time.
     **/
  Double_t GetStartTime() const { return fStartTime; }


  /** End time of time slice
     ** @return End time [ns]
     **
     ** If the type of the time slice is kEvent, the method returns the
     ** event time.
     **/
  Double_t GetEndTime() const;


  /** @brief Time stamp of first data
     ** @return Time stamp of first data [ns]
     **/
  Double_t GetTimeDataFirst() const { return fTimeDataFirst; }


  /** @brief Time stamp of last data
     ** @return Time stamp of last data [ns]
     **/
  Double_t GetTimeDataLast() const { return fTimeDataLast; }


  /** Check whether time slice contains data
     ** @return kTRUE if time slice contains data
     **/
  Bool_t IsEmpty() const { return fNofData.empty(); }


  /** Check for being of type regular
     ** @return kTRUE if type is regular
     **/
  Bool_t IsEvent() const { return fType == kEvent; }


  /** Check for being of type regular
     ** @return kTRUE if type is regular
     **/
  Bool_t IsFlexible() const { return fType == kFlexible; }


  /** Check for being of type regular
     ** @return kTRUE if type is regular
     **/
  Bool_t IsRegular() const { return fType == kRegular; }


  /** @brief Register data to time-slice header
     ** @param system    System ID (ECbmModuleId)
     ** @param time      Data time [ns]
     ** @value kFALSE if time is out of time-slice bounds; else kTRUE
     **
     ** The respective counter will be incremented.
     ** Time of data is checked with time-slice bounds.
     ** Time of first and last data are updated.
     **/
  Bool_t RegisterData(ECbmModuleId system, Double_t time);


  /** @brief Register data to time-slice header (with match object)
     ** @param system    System ID (ECbmModuleId)
     ** @param time      Data time [ns]
     ** @param match     Reference to digi match object
     ** @value kFALSE if time is out of time-slice bounds; else kTRUE
     **
     ** The respective counter will be incremented.
     ** Time of data is checked with time-slice bounds.
     ** Time of first and last data are updated.
     ** The match objects of the time slice is updated.
     **/
  Bool_t
  RegisterData(ECbmModuleId system, Double_t time, const CbmMatch& match);


  /** @brief Reset the time slice
     **
     ** Reset time-slice bookkeeping. Cannot be used in mode kRegular.
     **/
  void Reset();


  /** @brief Reset the time slice
     ** @param start    New start time [ns]
     ** @param length   New length [ns]
     **
     ** Reset start time, length and counters.
     ** If used in mode kFlexible or kEvent, the arguments are ignored.
     **/
  void Reset(Double_t start, Double_t length);


  /** @brief Set start time
     ** @param time Start time [ns]
     **/
  void SetStartTime(Double_t time) { fStartTime = time; }


  /** Status to string **/
  std::string ToString() const;


  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/) {
    ar& fLength;
    ar& fIsEmpty;
  }

private:
  EType fType;                             ///< Time-slice type
  Double_t fStartTime;                     ///< Start time [ns]
  Double_t fLength;                        ///< Length of time-slice [ns]
  Bool_t fIsEmpty;                         ///< Flag for containing no data
  std::map<ECbmModuleId, Int_t> fNofData;  ///< SystemId -> Number of digis
  Double_t fTimeDataFirst;                 ///< Time of first data object
  Double_t fTimeDataLast;                  ///< Time of last data object
  CbmMatch fMatch;                         ///< Link time slice to events


  /** @brief Reset the time slice bookkeeping **/
  void ResetCounters();


  friend class boost::serialization::access;

  ClassDef(CbmTimeSlice, 7)
};

#endif /* CBMTIMESLICE_H */
