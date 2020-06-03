// --------------------------------------------------------------------------------
// ----- HadaqTdcIterator header file                                         -----
// -----                                                                      -----
// ----- created by C. Simon on 2014-04-02                                    -----
// -----                                                                      -----
// ----- based on stream by S. Linev                                          -----
// ----- https://subversion.gsi.de/go4/app/stream/include/hadaq/TdcIterator.h -----
// ----- revision 1111, 2014-01-15                                            -----
// --------------------------------------------------------------------------------

#ifndef HADAQ_TDCITERATOR_H
#define HADAQ_TDCITERATOR_H

#include "HadaqDefines.h"
#include "HadaqTdcMessage.h"
#include "HadaqTimeStamp.h"

namespace hadaq {

   class TdcIterator {
      protected:

         enum { DummyEpoch = 0xffffffff };

         UInt_t*  fBuf;        //! pointer on raw data
         UInt_t   fBuflen;     //! length of raw data
         Bool_t   fSwapped;    //! true if raw data are swapped

         hadaq::TdcMessage fMsg; //! current message
         UInt_t  fCurEpoch;    //! current epoch

         hadaq::LocalStampConverter  fConv;   //! use to covert time stamps in seconds

      public:

         TdcIterator() :
            fBuf(0),
            fBuflen(0),
            fSwapped(kFALSE),
            fMsg(),
            fCurEpoch(DummyEpoch),
            fConv()
         {
            // 1st argument: wrap bits = epoch counter size + coarse time counter size
            // 2nd argument: conversion factor from coarse time unit to seconds
            fConv.SetTimeSystem(trbtdc::size_EpochCounter + trbtdc::size_CoarseTimeCounter, hadaq::TdcMessage::CoarseUnit());
         }

         void assign(UInt_t* buf, UInt_t len, Bool_t swapped = kTRUE)
         {
            fBuf = buf;
            fBuflen = len;
            fSwapped = swapped;
            fMsg.assign(0);

            if (fBuflen == 0) fBuf = 0;
            fCurEpoch = DummyEpoch;
         }

         void assign(hadaq::RawSubevent* subev, UInt_t indx, UInt_t datalen)
         {
            if (subev!=0)
               assign(subev->GetDataPtr(indx), datalen, subev->IsSwapped());
         }

         /** One should call method to set current reference epoch */
         void setRefEpoch(UInt_t epoch)
         {
            fConv.MoveRef(((ULong64_t) epoch) << trbtdc::size_CoarseTimeCounter);
         }

         Bool_t next()
         {
            if (fBuf==0) return false;

            if (fSwapped)
               fMsg.assign((((UChar_t *) fBuf)[0] << 24) | (((UChar_t *) fBuf)[1] << 16) | (((UChar_t *) fBuf)[2] << 8) | (((UChar_t *) fBuf)[3]));
            else
               fMsg.assign(*fBuf);

            if (fMsg.isEpochMsg()) fCurEpoch = fMsg.getEpochValue();

            fBuf++;
            if (--fBuflen == 0) fBuf = 0;

            return true;
         }

         /** Returns message stamp which combines epoch and coarse time counter */
         ULong64_t getMsgStamp() const
         { return (isCurEpoch() ? ((ULong64_t) fCurEpoch) << trbtdc::size_CoarseTimeCounter : 0) | (fMsg.isTimeMsg() ? fMsg.getTimeTmCoarse() : 0); }

         /** Returns epoch + coarse time converted to seconds */
         inline Double_t getMsgTimeCoarse() const
         { return fConv.ToSeconds(getMsgStamp()); }

         /** Returns fine time converted to seconds */
         inline Double_t getMsgTimeFine() const
         { return fMsg.isTimeMsg() ? hadaq::TdcMessage::SimpleFineCalibr(fMsg.getTimeTmFine()) : 0.; }

         hadaq::TdcMessage& msg() { return fMsg; }

         /** Returns true, if current epoch was assigned */
         Bool_t isCurEpoch() const { return fCurEpoch != DummyEpoch; }

         /** Clear current epoch value */
         void clearCurEpoch() { fCurEpoch = DummyEpoch; }

         /** Set value of current epoch */
         void setCurEpoch(UInt_t val) { fCurEpoch = val; }

         /** Return value of current epoch */
         UInt_t getCurEpoch() const { return fCurEpoch; }

         void printmsg()
         {
            Double_t tm = -1.;
            if (msg().isTimeMsg() || msg().isEpochMsg())
               tm = getMsgTimeCoarse() - getMsgTimeFine();
            msg().print(tm);
         }
         
      private:
         // no default Copy constructor and = OP as class not meant to be copied
         TdcIterator(const TdcIterator&);
         TdcIterator& operator=(const TdcIterator&);
   };
}

#endif
