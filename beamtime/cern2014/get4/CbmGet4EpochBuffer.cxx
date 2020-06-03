// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmGet4EpochBuffer                           -----
// -----                    Created 24.02.2015 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------


#include <iostream>
#include <iomanip>

#include "TString.h"

#include "FairLogger.h"

#include "CbmGet4EpochBuffer.h"

/*
 * TODO:
 * 1) Find proper default values for fuBufferMaxSize, fuBufferMaxEpochSpread
 *    and fuBufferHighWater
 */

// -----   Initialization of static variables   ------------------------------
CbmGet4EpochBuffer* CbmGet4EpochBuffer::fgInstance = NULL;
// ---------------------------------------------------------------------------

// -----   Constructor   -----------------------------------------------------
CbmGet4EpochBuffer::CbmGet4EpochBuffer() :
      fData(),
      fuBufferMaxSize(200),
      fuBufferMaxEpochSpread(100),
      fuBufferHighWater(160)
{
   LOG(info) << "CbmGet4EpochBuffer::CbmGet4EpochBuffer = Starting epoch buffer with: ";
   LOG(info) << fuBufferHighWater << " stored epochs for buffer high water ";
   LOG(info) << fuBufferMaxSize << " stored epochs for maximum buffer storage ";
   LOG(info) << fuBufferMaxEpochSpread << " for epoch spread rejection when maximum reached ";
   PrintStatus();
}
// ---------------------------------------------------------------------------

// -----   Destructor   ------------------------------------------------------
CbmGet4EpochBuffer::~CbmGet4EpochBuffer()
{
}
// ---------------------------------------------------------------------------

// -----   Access to next epoch   ---------------------------------------------
std::multiset< get4v1x::FullMessage >* CbmGet4EpochBuffer::GetNextEpoch()
{
  // --- Check for empty buffer
  if( !fData.size() )
     return NULL;

  // --- Get data from buffer
  std::multiset< get4v1x::FullMessage >* multiset = fData.front();
  fData.pop_front();
  return multiset;

}
// ---------------------------------------------------------------------------

// -----   Insert Epoch into buffer   -----------------------------------------
Int_t CbmGet4EpochBuffer::InsertEpoch( std::multiset< get4v1x::FullMessage >* epochCont )
{

  if( !epochCont )
     LOG(fatal)<<"CbmGet4EpochBuffer:InsertEpoch = Invalid epoch multiset pointer";
/*
  if( 100000 < epochCont->size() )
     LOG(info) << "CbmGet4EpochBuffer::InsertEpoch = Inserting epoch multiset, epoch idx "
                 << ( epochCont->begin() )->getExtendedEpoch()
                 << " New buffer size: " << fData.size()
                 << " First epoch: "<< GetEpochFirst()
                 << " Last epoch: "<< GetEpochLast();
*/
  if( 0 < epochCont->size() )
  {
     fData.push_back(epochCont);
     LOG(debug2) << "CbmGet4EpochBuffer::InsertEpoch = Inserting epoch multiset, epoch idx "
//     LOG(info) << "CbmGet4EpochBuffer::InsertEpoch = Inserting epoch multiset, epoch idx "
                 << ( epochCont->begin() )->getExtendedEpoch()
                 << " New buffer size: " << fData.size()
                 << " First epoch: "<< GetEpochFirst()
                 << " Last epoch: "<< GetEpochLast();

     if( fuBufferHighWater == fData.size() )
     {
        LOG(info) << "CbmGet4EpochBuffer::InsertEpoch = High water mark reached,  "
                    << fuBufferHighWater << "/" << fuBufferMaxSize
                    << " epoch stored in buffer.";
        LOG(info) << "                                  If MAX reached, epoch older than "
                  << fuBufferMaxEpochSpread << " epoch from the new one will be dropped!";
     } // if( fuBufferHighWater == fData.size() )
     if( fuBufferMaxSize <= fData.size() )
     {
        LOG(warning) << "CbmGet4EpochBuffer::InsertEpoch = Max buffer size reached. "
                     << "Now dropping all epochs more than " << fuBufferMaxEpochSpread
                     << " epochs older than the newly inserted one!!!";

        // now drop all epochs too old
        // WARNING: in case of epoch index cycle (should never happen but well...) the
        //          "older" epoch is also thrown out
        // Loop stop at least when reaching the newly inserted epoch
        while( ( ((fData.front())->begin() )->getExtendedEpoch() + fuBufferMaxEpochSpread
                    < ( epochCont->begin() )->getExtendedEpoch() ) ||
               ( ( ( epochCont->begin() )->getExtendedEpoch() )
                     < ((fData.front())->begin() )->getExtendedEpoch() )
              )
        {
           // Throw out the epoch: clear multiset, delete multiset, pop multiset pointer
           (fData.front())->clear();
           delete (fData.front());
           fData.pop_front();
        } // while first epoch has to be thrown out
     } // if( fuBufferMaxSize <= fData.size() )
  } // if( 0 < epochCont->size() )
     else
     {
        LOG(debug2) << "CbmGet4EpochBuffer: Trying to insert an empty epoch multiset => do nothing ";
//        LOG(info) << "CbmGet4EpochBuffer: Trying to insert an empty epoch multiset => do nothing ";
        epochCont->clear();
        delete epochCont;
     } // else of if( 0 < epochCont->size() )

   return fData.size();
}
// ---------------------------------------------------------------------------



// -----   Instance   --------------------------------------------------------
CbmGet4EpochBuffer* CbmGet4EpochBuffer::Instance() {
  if ( ! fgInstance ) fgInstance = new CbmGet4EpochBuffer();
  return fgInstance;
}
// ---------------------------------------------------------------------------



// -----   Print status   ----------------------------------------------------
void CbmGet4EpochBuffer::PrintStatus() const {
  Int_t size = GetSize();
  LOG(info) << "CbmGet4EpochBuffer: Status ";
  if( !size )
  {
    LOG(info) << "empty";
    return;
  } // if( !size )
  LOG(info) << GetSize() << " epoch multisets from epoch "
            << Form("%9llu to epoch ", GetEpochFirst() )
            << Form("%9llu ", GetEpochLast() );
}
// ---------------------------------------------------------------------------

