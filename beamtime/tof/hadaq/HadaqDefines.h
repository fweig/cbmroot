// -----------------------------------------------------------------------------
// ----- HadaqDefines header file                                          -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-03-06                                 -----
// -----                                                                   -----
// ----- based on stream by S. Linev                                       -----
// ----- https://subversion.gsi.de/go4/app/stream/include/hadaq/defines.h  -----
// ----- revision 967, 2013-05-13                                          -----
// -----------------------------------------------------------------------------

#ifndef HADAQ_DEFINES_H
#define HADAQ_DEFINES_H

#include "Rtypes.h"

#pragma pack(push, 1)

namespace hadaq {

   enum {
      HADAQ_TIMEOFFSET       = 1200000000 /* needed to reconstruct time from runId */
   };

   enum EvtId {
      EvtId_data     = 0x00000001,
      EvtId_DABC     = 0x00002001,      // hades DAQVERSION=2 (same as evtbuild.c uses DAQVERSION=2)
      EvtId_runStart = 0x00010002,
      EvtId_runStop  = 0x00010003,
      EvtId_DABC_TrbNet_Type_0 =  0x00002000,
      EvtId_DABC_TrbNet_Type_1 =  0x00002001,
      EvtId_DABC_TrbNet_Type_2 =  0x00002002,
      EvtId_DABC_TrbNet_Type_3 =  0x00002003,
      EvtId_DABC_TrbNet_Type_4 =  0x00002004,
      EvtId_DABC_TrbNet_Type_5 =  0x00002005,
      EvtId_DABC_TrbNet_Type_6 =  0x00002006,
      EvtId_DABC_TrbNet_Type_7 =  0x00002007,
      EvtId_DABC_TrbNet_Type_8 =  0x00002008,
      EvtId_DABC_TrbNet_Type_9 =  0x00002009,
      EvtId_DABC_TrbNet_Type_A =  0x0000200A,
      EvtId_DABC_TrbNet_Type_B =  0x0000200B,
      EvtId_DABC_TrbNet_Type_C =  0x0000200C,
      EvtId_DABC_TrbNet_Type_D =  0x0000200D,
      EvtId_DABC_TrbNet_Type_E =  0x0000200E,
      EvtId_DABC_TrbNet_Type_F =  0x0000200F
   };

   enum EvtDecoding {
      EvtDecoding_default = 1,
      EvtDecoding_64bitAligned = (0x03 << 16) | 0x0001
   };

   enum {
      HADTU_PAYLOAD_MAX = 65535
   };

   /*
    * HADES transport unit header
    * used as base for event and subevent
    * also common envelope for trd network data packets
    */

   struct HadTu {
//      protected:
         UInt_t tuSize;
         UInt_t tuDecoding;

      public:

         HadTu() : 
            tuSize(0),
            tuDecoding(0)
            {}
            
         // Makes destructor virtual to get rid of warning when using as 
         // base struct
         ~HadTu() {}

         /** msb of decode word is always non zero...? */
         inline Bool_t IsSwapped() const  { return  tuDecoding > 0xffffff; }

         inline UInt_t Value(const UInt_t *member) const
         {
            return IsSwapped() ? ((((UChar_t *) member)[0] << 24) |
                                  (((UChar_t *) member)[1] << 16) |
                                  (((UChar_t *) member)[2] << 8) |
                                  (((UChar_t *) member)[3])) : *member;
         }

         /** swapsave method to set value stolen from hadtu.h */
         inline void SetValue(UInt_t *member, UInt_t val)
         {
            *member = IsSwapped() ?
                  ((((UChar_t *) &val)[0] << 24) |
                  (((UChar_t *) &val)[1] << 16) |
                  (((UChar_t *) &val)[2] << 8) |
                  (((UChar_t *) &val)[3])) : val;
         }


         UInt_t GetDecoding() const { return Value(&tuDecoding); }
         inline UInt_t GetSize() const { return Value(&tuSize); }

         inline UInt_t GetPaddedSize() const
         {
            UInt_t hedsize = GetSize();
            UInt_t rest = hedsize % 8;
            return (rest==0) ? hedsize : (hedsize + 8 - rest);
         }

         void SetSize(UInt_t bytes) { SetValue(&tuSize, bytes); }
   };

   // ======================================================================

   /*
    * Intermediate hierarchy class as common base for event and subevent
    */
   struct HadTuId : public HadTu {
//      protected:
         UInt_t tuId;

      public:

         HadTuId() : tuId(0) {}
         // Makes destructor virtual to get rid of warning when using as 
         // base struct
         ~HadTuId() {}

         inline UInt_t GetId() const { return Value(&tuId); }
         void SetId(UInt_t id) { SetValue(&tuId, id); }

         inline Bool_t GetDataError() const { return (GetId() & 0x80000000) != 0; }

         void SetDataError(Bool_t on)
         {
            if(on)
               SetId(GetId() | 0x80000000);
            else
               SetId(GetId() & ~0x80000000);
         }
   };

   // =================================================================================

   //Description of the Event Structure
   //
   //An event consists of an event header and of varying number of subevents, each with a subevent header. The size of the event header is fixed to 0x20 bytes
   //
   //Event header
   //evtHeader
   //evtSize   evtDecoding    evtId    evtSeqNr    evtDate  evtTime  runNr    expId
   //
   //    * evtSize - total size of the event including the event header, it is measured in bytes.
   //    * evtDecoding - event decoding type: Tells the analysis the binary format of the event data, so that it can be handed to a corresponding routine for unpacking into a software usable data structure. For easier decoding of this word, the meaning of some bits is already predefined:
   //      evtDecoding
   //      msB    ---   ---   lsB
   //      0   alignment   decoding type  nonzero
   //          o The first (most significant) byte is always zero.
   //          o The second byte contains the alignment of the subevents in the event. 0 = byte, 1 = 16 bit word, 2 = 32 bit word...
   //          o The remaining two bytes form the actual decoding type, e.g. fixed length, zero suppressed etc.
   //          o The last byte must not be zero, so the whole evtDecoding can be used to check for correct or swapped byte order.
   //
   //It is stated again, that the whole evtDecoding is one 32bit word. The above bit assignments are merely a rule how to select this 32bit numbers.
   //
   //    * evtId - event identifier: Tells the analysis the semantics of the event data, e.g. if this is a run start event, data event, simulated event, slow control data, end file event, etc..
   //      evtId
   //      31  30 - 16  15 - 12  11 - 8   5 - 7    4  3- 0
   //      error bit    reserved    version  reserved    MU decision    DS flag  ID
   //          o error bit - set if one of the subsystems has set the error bit
   //          o version - 0 meaning of event ID before SEP03; 1 event ID after SEP03
   //          o MU decision - 0 = negative LVL2 decision; >0 positive LVL2 decision
   //            MU trigger algo result
   //            1   negative decision
   //            2   positive decision
   //            3   positive decision due to too many leptons or dileptons
   //            4   reserved
   //            5   reserved
   //          o DS flag - LVL1 downscaling flag; 1 = this event is written to the tape independent on the LVL2 trigger decision
   //          o ID - defines the trigger code
   //            ID before SEP03    description
   //            0   simulation
   //            1   real
   //            2,3,4,5,6,7,8,9    calibration
   //            13  beginrun
   //            14  endrun
   //
   //            ID after SEP03  description
   //            0   simulation
   //            1,2,3,4,5    real
   //            7,9    calibration
   //            1   real1
   //            2   real2
   //            3   real3
   //            4   real4
   //            5   real5
   //            6   special1
   //            7   offspill
   //            8   special3
   //            9   MDCcalibration
   //            10  special5
   //            13  beginrun
   //            14  endrun
   //    * evtSeqNr - event number: This is the sequence number of the event in the file. The pair evtFileNr/evtSeqNr
   //
   //is unique throughout all events ever acquired by the system.
   //
   //    * evtDate - date of event assembly (filled by the event builder, rough precision):
   //      evtDate   ISO-C date format
   //      msB    ---   ---   lsB
   //      0   year  month    day
   //
   //   1. The first (most significant) byte is zero
   //   2. The second byte contains the years since 1900
   //   3. The third the months since January [0-11]
   //   4. The last the day of the month [1-31]
   //
   //    * evtTime - time of assembly (filled by the event builder, rough precision):
   //      evtTime   ISO-C time format
   //      msB    ---   ---   lsB
   //      0   hour  minute   second
   //
   //   1. The first (most significant) byte is zero
   //   2. The second byte contains the hours since midnight [0-23]
   //   3. The third the minutes after the hour [0-59]
   //   4. The last the seconds after the minute [0-60]
   //
   //    * runNr - file number: A unique number assigned to the file. The runNr is used as key for the RTDB.
   //    * evtPad - padding: Makes the event header a multiple of 64 bits long.


   struct RawEvent : public HadTuId  {

//      protected:
         UInt_t evtSeqNr;
         UInt_t evtDate;
         UInt_t evtTime;
         UInt_t evtRunNr;
         UInt_t evtPad;

         /** Method to set initial header value like decoding and date/time */
         void InitHeader(UInt_t evid);


      public:

         RawEvent() : 
            evtSeqNr(0),
            evtDate(0),
            evtTime(0),
            evtRunNr(0),
            evtPad(0)
            {}
         ~RawEvent() {}

         UInt_t GetSeqNr() const { return Value(&evtSeqNr); }
         void SetSeqNr(UInt_t n) { SetValue(&evtSeqNr, n); }

         Int_t GetRunNr() const { return Value(&evtRunNr); }
         void SetRunNr(UInt_t n) { SetValue(&evtRunNr, n); }

         Int_t GetDate() const { return Value(&evtDate); }
         void SetDate(UInt_t d) { SetValue(&evtDate, d); }

         Int_t GetTime() const { return Value(&evtTime); }
         void SetTime(UInt_t t) { SetValue(&evtTime, t); }

         UChar_t GetTrigType () const { return (GetId() & 0xf); }

         void Init(UInt_t evnt, UInt_t run=0, UInt_t id=EvtId_DABC)
         {
            InitHeader(id);
            SetSeqNr(evnt);
            SetRunNr(run);
            evtPad = 0;
         }

         void Dump();

         static UInt_t CreateRunId();
   };

/*
//Subevent
//
//Every event contains zero to unspecified many subevents. As an empty event is allowed, data outside of any subevent are not allowed. A subevents consists out of a fixed size subevent header and a varying number of data words.
//
//    * The subevent header
//      subEvtHeader
//      subEvtSize   subEvtDecoding    subEvtId    subEvtTrigNr
//          o subEvtSize - size of the subevent: This includes the the subevent header, it is measured in bytes.
//          o subEvtDecoding - subevent decoding type: Tells the analysis the binary format of the subevent data, so that it can be handed to a corresponding routine for unpacking into a software usable data structure. For easier decoding of this word, the meaning of some bits is already predefined:
//            subEvtDecoding
//            msB    ---   ---   lsB
//            0   data type   decoding type  nonzero
//                + The first (most significant) byte is always zero
//                + The second byte contains the word length of the subevent data. 0 = byte, 1 = 16 bit word, 2 = 32 bit word...
//                + The remaining two bytes form the actual decoding type, e.g. fixed length, zero suppressed etc.
//                + The last byte must not be zero, so the whole subEvtDecoding can be used to check for correct or swapped byte order. It is stated again, that the whole subEvtDecoding is one 32bit word. The above bit assignments are merely a rule how to select this 32bit numbers.
//          o subEvtId - subevent identifier: Tells the analysis the semantics of the subevent data, e.g. every subevent builder may get its own subEvtId. So the data structure can be analyzed by the corresponding routine after unpacking.
//            1-99   DAQ
//            100-199   RICH
//            200-299   MDC
//            300-399   SHOWER
//            400-499   TOF
//            500-599   TRIG
//            600-699   SLOW
//            700-799   TRB_RPC  common TRB, but contents is RPC
//            800-899   TRB_HOD  pion-hodoscope
//            900-999   TRB_FW   forward wall
//            1000-1099    TRB_START   start detector
//            1100-1199    TRB_TOF  TOF detector
//            1200-1299    TRB RICH    RICH detector
//
//Additionally, all subEvtIds may have the MSB set. This indicates a sub event of the corresponding id that contains broken data (e.g. parity check failed, sub event was too long etc.).
//
//    *
//          o subEvtTrigNr - subevent Trigger Number: This is the event tag that is produced by the trigger system and is used for checking the correct assembly of several sub entities. In general, this number is not counting sequentially. The lowest significant byte represents the trigger tag generated by the CTU and has to be same for all detector subsystems in one event. The rest is filled by the EB.
//    * The data words: The format of the data words (word length, compression algorithm, sub-sub-event format) is defined by the subEvtDecoding and apart from this it is completely free. The meaning of the data words (detector, geographical position, error information) is defined by the subEvtId and apart from this it is completely unknown to the data acquisition system.
*/

   struct RawSubevent : public HadTuId  {

//      protected:

         UInt_t subEvtTrigNr;

      public:

         RawSubevent() : subEvtTrigNr(0) {}
         ~RawSubevent() {}

         UInt_t Alignment() const { return 1 << ( GetDecoding() >> 16 & 0xff); }

         UInt_t GetTrigNr() const { return Value(&subEvtTrigNr); }
         void SetTrigNr(UInt_t trigger) { SetValue(&subEvtTrigNr, trigger); }

         UChar_t GetTrigType () const { return (GetDecoding() & 0xf0) >> 4; }

         void Init(UInt_t trigger = 0)
         {
            SetTrigNr(trigger);
         }


         /** Return pointer on data by index - user should care itself about swapping */
         UInt_t* GetDataPtr(UInt_t indx) const
         {
            return (UInt_t*) (this) + sizeof(RawSubevent)/sizeof(UInt_t) + indx;
         }


         /* returns number of payload data words, not maximum index!*/
         UInt_t GetNrOfDataWords() const
         {
            UInt_t datasize = GetSize() - sizeof(hadaq::RawSubevent);
            switch (Alignment()) {
               case 4:  return datasize / sizeof(UInt_t);
               case 2:  return datasize / sizeof(UShort_t);
               default: return datasize / sizeof(UChar_t);
            }
            return datasize;
         }

         /** swap-save access to any data. stolen from hadtu.h */
         UInt_t Data(UInt_t idx) const
         {
            const void* my = (char*) (this) + sizeof(RawSubevent);

            switch (Alignment()) {
               case 4:
                  return Value((UInt_t *) my + idx);

               case 2: {
                  UShort_t tmp = ((UShort_t *) my)[idx];

                  if (IsSwapped()) tmp = ((tmp >> 8) & 0xff) | ((tmp << 8) & 0xff00);

                  return tmp;
               }
            }

            return ((UChar_t*) my)[idx];
         }

         UInt_t GetErrBits()
         {
            return Data(GetNrOfDataWords()-1);
         }

         void CopyDataTo(void* buf, UInt_t indx, UInt_t datalen)
         {
            if (buf==0) return;
            while (datalen-- > 0) {
               *((UInt_t*) buf) = Data(indx++);
               buf = ((UInt_t*) buf) + 1;
            }
         }

         /** Return pointer where raw data should starts */
         void* RawData() const { return (char*) (this) + sizeof(hadaq::RawSubevent); }


         void Dump(Bool_t print_raw_data = false);
   };


}

#pragma pack(pop)

#endif
