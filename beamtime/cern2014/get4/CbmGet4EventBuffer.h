// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmGet4EventBuffer                           -----
// -----                    Created 24.02.2015 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMGET4EVENTBUFFER_H
#define CBMGET4EVENTBUFFER_H 1

#include <deque>
#include <vector>
//#include <utility> // for std::pair, maybe not needed

#include "CbmGet4v1xHackDef.h"

class CbmGet4EventHeader
{
   public:
      ULong64_t fuEventIndex;
      Int_t     fiTriggerType;
      ULong64_t fuTriggerExtEp;
      UInt_t    fuTriggerTs;
      CbmGet4EventHeader(UInt_t uInitVal = 0):
         fuEventIndex(uInitVal),
         fiTriggerType(uInitVal),
         fuTriggerExtEp(uInitVal),
         fuTriggerTs(uInitVal)
         {};
      CbmGet4EventHeader( UInt_t uEvtIdxIn, Int_t iTrigTypIn,
                          UInt_t uTrigExtEpIn, UInt_t uTrigTsIn):
         fuEventIndex(   uEvtIdxIn),
         fiTriggerType(  iTrigTypIn),
         fuTriggerExtEp( uTrigExtEpIn),
         fuTriggerTs(    uTrigTsIn)
         {};
};
/** @class CbmGet4EventBuffer
 ** @author P.-A. Loizeau
 ** @date 24 February 2015
 ** @brief Singleton buffer class for CBM TOF GET4 raw data organized in events
 **
 ** The CbmGet4EventBuffer stores GET4 raw data already ordered
 ** in event (vectors) transiently.
 ** Full time sorted event vectors can be sent to the buffer by
 ** the method InsertEvent.
 ** They can be retrieved by GetNextEvent, which delivers a
 ** pointer to the vector of time sorted data.
 ** The standard deque is used to approximate a FIFO behavior.
 **
 ** There is only one buffer stream.
 **
 ** The buffer handles buffer objects only by pointer, i.e. the vectors have
 ** to be instantiated by the sending task (unpacker) and
 ** deleted by the receiving class (Event Builder).
 **/
class CbmGet4EventBuffer
{
  public:

    /**   Destructor  **/
    ~CbmGet4EventBuffer();

    /** Set the maximal number of events that the buffer is allowed to store
     ** @param uBuffMaxSz  buffer maximal size in events
     **/
    void SetBufferMaxSize( UInt_t uBuffMaxSz = 500 ) { fuBufferMaxSize = uBuffMaxSz;};

    /** Set the maximal spread in event number above which events are dropped in case
     ** the maximal number of stored events is reached
     ** @param ulBuffMaxEvtSprd  event number spread limit for event dropping
     **/
    void SetBufferDropEventSpread( ULong64_t ulBuffMaxEvtSprd = 100 )
                                        { fuBufferMaxEventSpread = ulBuffMaxEvtSprd;};

    /** High water mark: when this number of stored events is reached at insertion,
     ** a message is printed to warn the user that the maximal limit of stored events
     ** is close.
     ** @param uBuffHW  buffer high water level in events
     **/
    void SetBufferHighWater( UInt_t uBuffHW = 400 ) { fuBufferHighWater = uBuffHW;};

    /** Pointer to next epoch container
     ** @return a pair of event header and pointer to vector containing time
     **         ordered data of this event
     **
     ** A pair (0, NULL pointer) will be returned if the buffer is empty.
     **/
    std::pair< CbmGet4EventHeader, std::vector< get4v1x::FullMessage >* > GetNextEvent();


    /** Current buffer size
     ** @return number of objects in buffer
     **/
    UInt_t GetSize() const { return fData.size(); }

    /**  Get first Event index  **/
    ULong64_t GetEventFirst() const {
      if( !GetSize() ) return 0;
      return (( fData.front() ).first).fuEventIndex;
     }


    /**  Get last Event index  **/
    ULong64_t GetEventLast() const {
      if ( ! GetSize() ) return 0;
      return (( fData.back() ).first).fuEventIndex;
     }

    /** Insert data into the buffer
     ** @param uEventIdx  event index of the event to be inserted
     ** @param eventCont  pointer to vector containing the event to be inserted
     ** @return number of objects in buffer
     **/
    Int_t InsertEvent( ULong64_t uEventIdx, std::vector< get4v1x::FullMessage >* eventCont);

    /** Insert data into the buffer
     ** @param uEventIdx  event index of the event to be inserted
     ** @param iTrigType  Trigger type of the event to be inserted
     ** @param uTrigExtEp Trigger extended epoch of the event to be inserted
     ** @param uTrigTs    Trigger timestamp of the event to be inserted
     ** @param eventCont  pointer to vector containing the event to be inserted
     ** @return number of objects in buffer
     **/
    Int_t InsertEvent( ULong64_t uEventIdx, Int_t iTrigType, ULong64_t uTrigExtEp, UInt_t uTrigTs,
                       std::vector< get4v1x::FullMessage >* eventCont);

    /** Insert data into the buffer
     ** @param eventHdr   event header containing the index of the event and its trigger info
     ** @param eventCont  pointer to vector containing the event to be inserted
     ** @return number of objects in buffer
     **/
    Int_t InsertEvent( CbmGet4EventHeader eventHdr, std::vector< get4v1x::FullMessage >* eventCont);


    /**   Clear buffer and delete corresponding events
     **/
    void Clear();

    /**   Access to singleton instance
     ** @return pointer to instance
     **/
    static CbmGet4EventBuffer* Instance();


    /**   Print buffer status  **/
    void PrintStatus() const;


  private:

    /** Buffer management
     ** Store pairs of event header and pointer of time ordered data vector for get4 event.
     **/
    std::deque< std::pair< CbmGet4EventHeader, std::vector< get4v1x::FullMessage >* > > fData;


    /** Pointer to singleton instance **/
    static CbmGet4EventBuffer* fgInstance;

    /** Limit in number of stored events **/
    UInt_t fuBufferMaxSize;

    /** Border for throwing out older events (FIFO)
     ** If the buffer limit is reached, all events older
     ** than this limit relative to the new event to be
     ** inserted are thrown out.
     ** Warning message are be printed out!
     **/
    ULong64_t fuBufferMaxEventSpread;

    /** High water mark for buffer full.
     ** Info message is printed out when reached.
     **/
    UInt_t fuBufferHighWater;


    /**  Default constructor
     **  Declared private to prevent instantiation.
     **/
    CbmGet4EventBuffer();


    /**  Copy constructor. Defined private to prevent usage. **/
    CbmGet4EventBuffer(const CbmGet4EventBuffer&);


    /**  Assignment operator. Defined private to prevent usage. **/
    CbmGet4EventBuffer& operator=(const CbmGet4EventBuffer&);

};

#endif /* CBMGET4EVENTBUFFER_H */
