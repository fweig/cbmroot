/** @file CbmDaqBuffer.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17 July 2012
 **/

#ifndef CBMTBDAQBUFFER_H
#define CBMTBDAQBUFFER_H 1

#include "CbmDefs.h"

#include <RtypesCore.h>  // for Double_t, Int_t

#include <boost/any.hpp>

#include <map>      // for multimap, __map_const_iterator, multimap<>::...
#include <utility>  // for pair

/** @class CbmTbDaqBuffer
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 13 December 2012
 ** @brief Singleton buffer class for CBM raw data
 **
 ** The CbmTbDaqBuffer stores and sorts (w.r.t. time) CBM raw data
 ** (currently: boost::any) transiently.
 ** Data can be send to the buffer by the method InsertData.
 ** They can be retrieved by GetNextData, which delivers a
 ** time-ordered sequence of raw data objects.
 **
 ** There is only one buffer stream, irrespective of the detector system.
 **
 ** The buffer handles objects only by pointer, i.e. the data have
 ** to be instantiated by the sending task (digitiser) and
 ** deleted by the receiving class (CbmDaq).
 **/
class CbmTbDaqBuffer {
public:
  typedef std::pair<boost::any, ECbmModuleId> Data;

  /**   Destructor  **/
  ~CbmTbDaqBuffer();

  /** Pointer to next raw data object
   ** up to given time
   ** @param time  maximal time [ns]
   ** @return pointer to raw data object
   **
   ** A NULL pointer will be returned if no further data can be released.
   ** This does not mean that the buffer is empty.
   **/
  //  boost::any GetNextData(Double_t time);
  Data GetNextData(Double_t time);


  /** Current buffer size
   ** @return number of objects in buffer
   */
  Int_t GetSize() const { return fData.size(); }


  /**  Get first digi time  **/
  Double_t GetTimeFirst() const;

  /**  Get last digi time  **/
  Double_t GetTimeLast() const;

  /**   Access to singleton instance
   ** @return pointer to instance
   **/
  static CbmTbDaqBuffer* Instance();

  /**   Print buffer status  **/
  void PrintStatus() const;

  /** Insert digi of any type into the buffer */
  template<class Digi>
  void InsertData(Digi* digi)
  {
    Double_t digi_time    = digi->GetTime();
    ECbmModuleId systemID = Digi::GetSystem();
    InsertData(digi, digi_time, systemID);
  }

private:
  /** Buffer management **/
  std::multimap<Double_t, Data> fData;


  /** Pointer to singleton instance **/
  static CbmTbDaqBuffer* fgInstance;


  /**  Default constructor
   **  Declared private to prevent instantiation.
   **/
  CbmTbDaqBuffer();


  /**  Copy constructor. Defined private to prevent usage. **/
  CbmTbDaqBuffer(const CbmTbDaqBuffer&);


  /**  Assignment operator. Defined private to prevent usage. **/
  CbmTbDaqBuffer& operator=(const CbmTbDaqBuffer&);

  /** Insert data into the buffer
   ** @param digi  pointer to data object to be inserted
   **/
  //  void InsertData(boost::any digi);
  void InsertData(boost::any digi, Double_t time, ECbmModuleId systemID)
  {
    fData.insert(std::make_pair(time, std::make_pair(std::move(digi), systemID)));
  }
};


#endif /* CBMTBDAQBUFFER_H */
