/** @file CbmDaqBuffer.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17 July 2012
 **/

#ifndef CBMDAQBUFFER_H
#define CBMDAQBUFFER_H 1

#include <map>

#include "CbmSpadicRawMessage.h"


/** @Class CbmTrdDaqBuffer
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 13 December 2012
 ** @brief Singleton buffer class for CBM raw data
 **
 ** The CbmTrdDaqBuffer stores and sorts (w.r.t. time) CBM raw data
 ** (currently: CbmSpadicRawMessage) transiently.
 ** Data can be send to the buffer by the method InsertData.
 ** They can be retrieved by GetNextData, which delivers a
 ** time-ordered sequence of raw data objects.
 **
 ** There is only one buffer stream, irrespective of the detector system.
 **
 ** The buffer handles objects only by pointer, i.e. the data have
 ** to be instantiated by the sending task (rawMessagetiser) and
 ** deleted by the receiving class (CbmTrdDaq).
 **/
class CbmTrdDaqBuffer
{
  public:

    /**   Destructor  **/
     ~CbmTrdDaqBuffer();



    /** Pointer to next raw data object
     ** up to given time
     ** @param time  maximal time [ns]
     ** @return pointer to raw data object
     **
     ** A NULL pointer will be returned if no further data can be released.
     ** This does not mean that the buffer is empty.
     **/
    CbmSpadicRawMessage* GetNextData(Double_t time);


    /** Current buffer size
     ** @return number of objects in buffer
     */
    Int_t GetSize() const { return fData.size(); }


    /**  Get first rawMessage time  **/
    Double_t GetTimeFirst() const {
      if ( ! GetSize() ) return -1.;
      return (fData.begin())->second->GetFullTime();
     }


    /**  Get last rawMessage time  **/
    Double_t GetTimeLast() const {
      if ( ! GetSize() ) return -1.;
      return (--fData.end())->second->GetFullTime();
    }


    /** Insert data into the buffer
     ** @param rawMessage  pointer to data object to be inserted
     **/
    void InsertData(CbmSpadicRawMessage* rawMessage);


    /**   Access to singleton instance
     ** @return pointer to instance
     **/
    static CbmTrdDaqBuffer* Instance();


    /**   Print buffer status  **/
    void PrintStatus() const;


  private:


    /** Buffer management **/
    std::multimap<Double_t, CbmSpadicRawMessage*> fData;


    /** Pointer to singleton instance **/
    static CbmTrdDaqBuffer* fgInstance;


    /**  Default constructor
     **  Declared private to prevent instantiation.
     **/
    CbmTrdDaqBuffer();


    /**  Copy constructor. Defined private to prevent usage. **/
    CbmTrdDaqBuffer(const CbmTrdDaqBuffer&);


    /**  Assignment operator. Defined private to prevent usage. **/
    CbmTrdDaqBuffer& operator=(const CbmTrdDaqBuffer&);



};

#endif /* CBMDAQBUFFER_H */
