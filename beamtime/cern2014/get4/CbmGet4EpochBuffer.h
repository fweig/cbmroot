// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmGet4EpochBuffer                           -----
// -----                    Created 24.02.2015 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMGET4EPOCHBUFFER_H
#define CBMGET4EPOCHBUFFER_H 1

#include <deque>
#include <set>

#include "CbmGet4v1xHackDef.h"

/** @class CbmGet4EpochBuffer
 ** @author P.-A. Loizeau
 ** @date 24 February 2015
 ** @brief Singleton buffer class for CBM TOF GET4 raw data organized in epochs
 **
 ** The CbmGet4EpochBuffer stores GET4 raw data already ordered
 ** in epochs (multisets) transiently.
 ** Full time sorted epoch multisets can be sent to the buffer by
 ** the method InsertEpoch.
 ** They can be retrieved by GetNextEpoch, which delivers a
 ** pointer to the multiset of time sorted data.
 ** The standard deque is used to approximate a FIFO behavior.
 **
 ** There is only one buffer stream.
 **
 ** The buffer handles objects only by pointer, i.e. the multisets have
 ** to be instantiated by the sending task (unpacker) and
 ** deleted by the receiving class (Event Builder).
 **/
class CbmGet4EpochBuffer
{
  public:

    /**   Destructor  **/
    ~CbmGet4EpochBuffer();

    /** Set the maximal number of epochs that the buffer is allowed to store
     ** @param uBuffMaxSz  buffer maximal size in epochs
     **/
    void SetBufferMaxSize( UInt_t uBuffMaxSz = 200 ) { fuBufferMaxSize = uBuffMaxSz;};

    /** Set the maximal spread in epoch number above which epochs are dropped in case
     ** the maximal number of stored epochs is reached
     ** @param ulBuffMaxEpSprd  epoch number spread limit for event dropping
     **/
    void SetBufferDropEpochSpread( ULong64_t ulBuffMaxEpSprd = 100 )
                                        { fuBufferMaxEpochSpread = ulBuffMaxEpSprd;};

    /** High water mark: when this number of stored epochs is reached at insertion,
     ** a message is printed to warn the user that the maximal limit of stored events
     ** is close.
     ** @param uBuffHW  buffer high water level in epochs
     **/
    void SetBufferHighWater( UInt_t uBuffHW = 180 ) { fuBufferHighWater = uBuffHW;};

    /** Pointer to next epoch container
     ** @return pointer to multiset containing time ordered data of this single epoch
     **
     ** A NULL pointer will be returned if the buffer is empty.
     **/
    std::multiset< get4v1x::FullMessage >* GetNextEpoch();


    /** Current buffer size
     ** @return number of objects in buffer
     **/
    Int_t GetSize() const { return fData.size(); }

    /**  Get first Epoch index  **/
    ULong64_t GetEpochFirst() const {
      if( !GetSize() ) return 0;
      return ( ( fData.front() )->begin() )->getExtendedEpoch();
     }


    /**  Get last Epoch index  **/
    ULong64_t GetEpochLast() const {
      if ( ! GetSize() ) return 0;
      return ( ( fData.back() )->begin() )->getExtendedEpoch();
    }

    /** Insert data into the buffer
     ** @param epochCont  pointer to multiset containing the epoch to be inserted
     ** @return number of objects in buffer
     **/
    Int_t InsertEpoch( std::multiset< get4v1x::FullMessage >* epochCont);


    /**   Access to singleton instance
     ** @return pointer to instance
     **/
    static CbmGet4EpochBuffer* Instance();


    /**   Print buffer status  **/
    void PrintStatus() const;


  private:

    /** Buffer management
     ** Store pointer of time ordered data multiset for get4 epoch.
     **/
    std::deque< std::multiset< get4v1x::FullMessage >* > fData;


    /** Pointer to singleton instance **/
    static CbmGet4EpochBuffer* fgInstance;

    /** Limit in number of stored epochs **/
    UInt_t fuBufferMaxSize;

    /** Border for throwing out older epochs (FIFO)
     ** If the buffer limit is reached, all epoch older
     ** than this limit relative to the new epoch to be
     ** inserted are thrown out.
     ** Warning are printed out!
     **/
    ULong64_t fuBufferMaxEpochSpread;

    /** High water mark for buffer full.
     ** Info message is printed out when reached.
     **/
    UInt_t fuBufferHighWater;

    /**  Default constructor
     **  Declared private to prevent instantiation.
     **/
    CbmGet4EpochBuffer();


    /**  Copy constructor. Defined private to prevent usage. **/
    CbmGet4EpochBuffer(const CbmGet4EpochBuffer&);


    /**  Assignment operator. Defined private to prevent usage. **/
    CbmGet4EpochBuffer& operator=(const CbmGet4EpochBuffer&);

};

#endif /* CBMGET4EPOCHBUFFER_H */
