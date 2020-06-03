#ifndef ROC_MESSAGE_H
#define ROC_MESSAGE_H

#ifdef HAVE_UINT8_T_HEADER_FILE
#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#else
#include <stdint.h>
#endif

#include <iostream>

namespace roc {


   enum MessageFormat {
      formatEth1   = 0,  // original message format with big-endian coding, 6-byte
      formatOptic1 = 1,  // original message format with big-endian coding, add 2 byte send/recv info, 8-byte
      formatEth2   = 2,  // new message format with little-endian coding, 6-byte
      formatOptic2 = 3,  // new message format with little-endian coding, add 2 byte send/recv info, 8-byte
      formatNormal = 4   // new message format with little-endian coding, 16-bit roc number (default)
   };

   enum MessageTypes {
      MSG_NOP    = 0,
      MSG_HIT    = 1,
      MSG_EPOCH  = 2,
      MSG_SYNC   = 3,
      MSG_AUX    = 4,
      MSG_EPOCH2 = 5,
      MSG_GET4   = 6,
      MSG_SYS    = 7
   };

   enum SysMessageTypes {
      SYSMSG_DAQ_START    = 1,      // indicates start daq in data stream
      SYSMSG_DAQ_FINISH   = 2,      // stop daq
      SYSMSG_NX_PARITY    = 3,      // nx_parity error
      SYSMSG_SYNC_PARITY  = 4,      // sync parity error
      SYSMSG_DAQ_RESUME   = 5,      // daq resume due to low/high water marker, only in udp case
      SYSMSG_FIFO_RESET   = 6,      // FPGA fifo reset
      SYSMSG_USER         = 7,      // user define message, generated by writing into ROC_ADDSYSMSG register
      SYSMSG_PCTIME       = 8,      // contains value of time() function, indicates when message was created on PC
      SYSMSG_ADC          = 9,      // contains feb1d (1 bit), channel id (7 bit) and adc value (24 bit), measured on PC
      SYSMSG_PACKETLOST   = 10,     // inserted by udp transport when packet was lost at this place
      SYSMSG_GET4_EVENT   = 11,     // GET4 event
      SYSMSG_CLOSYSYNC_ERROR = 12,  // added to data stream when the closy-sync-strobe does not match the rocs 156MHz timestamp counter
      SYSMSG_TS156_SYNC   = 13      // added when 156MHz timestamp counter is reset by a DLM

   };

   enum SysMessageUserTypes {
      SYSMSG_USER_CALIBR_ON    = 7,
      SYSMSG_USER_CALIBR_OFF   = 8,
      SYSMSG_USER_RECONFIGURE  = 9
   };

   enum MessagePrintMask {
      msg_print_Prefix = 1,
      msg_print_Data   = 2,
      msg_print_Hex    = 4,
      msg_print_Human  = 8
   };

   class Message {

      protected:
         uint64_t data;   // main and only storage field for the message

      public:
         Message() : data(0) {}

         Message(const Message& src) : data(src.data) {}

         void assign(const Message& src) { data = src.data; }

//         void operator=(const Message& src) { assign(src); }

         inline void reset() { data = 0; }

         inline uint32_t getField(uint32_t shift, uint32_t len) const
            { return (data >> shift) & ((((uint32_t)1) << len) - 1); }

         inline void setField(uint32_t shift, uint32_t len, uint32_t value)
            { data = (data & ~(((((uint64_t) 1) << len) - 1) << shift)) | (((uint64_t) value) << shift); }

         inline uint8_t getBit(uint32_t shift) const
            { return (data >> shift) & 1; }

         inline void setBit(uint32_t shift, uint8_t value)
            { data = value ? (data | (((uint64_t) 1) << shift)) : (data & ~(((uint64_t) 1) << shift)) ; }


         // --------------------------- common fields ---------------------------------

         //! Returns the message type. Valid for all message types. 4 bit
         inline uint8_t getMessageType() const { return getField(0, 4); }

         //! Returns the number of the sending ROC. Valid for all message types.
         /*!
          * The field has full 16 bits and allows to aggregate data of up to 64K ROC's
          * in one message stream.
          */
         inline uint16_t getRocNumber() const { return getField(48, 16); }

         //! Sets the message type field in the current message
         inline void setMessageType(uint8_t v) { setField(0, 4, v); }

         //! Sets the ROC number field in the current message
         inline void setRocNumber(uint16_t v) { setField(48, 16, v); }


         // ---------- nXYTER Hit data access methods ----------------

         //! For Hit data: Returns nXYTER number (2 bit field)
         /*!
          * A ROC can support up to 2 FEBs with a total of 4 nXYTER chips.
          * This field identifies FEB as well as chip. Each supported
          * configuration (either 2 x FEB1nx/FEB2nx or a single FEB4nx)
          * as a unique nXYTER chip numbering.
          */
         inline uint8_t getNxNumber() const { return getField(6, 2); }

         //! For Hit data: Returns 3 most significant bits of ROC LTS (3 bit field)
         /*!
          * The 3 MSBs of the ROC local time stamp counter at the time of data
          * capture are stored in this field. A comparison of this number,
          * which reflects the time of readout, and the 3 MSBs of the timestamp
          * (returned by getNxLtsMsb() const), which reflects the time of the
          * hit, allows to estimate the time the hit stayed in the nXYTER internal
          * FIFOs. This in turn gives an indication of the data load in the
          * nXYTER.
          */
         inline uint8_t getNxLtsMsb() const { return getField(8, 3); }

         //! For Hit data: Returns nXYTER time stamp (14 bit field)
         /*!
          * Raw timestamp as delivered by the nXYTER. The last-epoch flag
          * (returned by getNxLastEpoch() const) determines whether the
          * hit belongs to the current or the last epoch. Use the
          * getMsgFullTime(uint32_t) const method to determine the
          * expanded and adjusted timestamp.inline uint32_t getSysMesData() const { return getField(16, 32); }
          */
         inline uint16_t getNxTs() const { return getField(11, 14); }

         //! For Hit data: Returns nXYTER channel number (7 bit field)
         inline uint8_t getNxChNum() const { return getField(25, 7); }

         // 1 bit unused

         //! For Hit data: Returns ADC value (12 bit field)
         inline uint16_t getNxAdcValue() const { return getField(33, 12); }

         //! For Hit data: Returns nXYTER pileup flag (1 bit field)
         /*!
          * This flag is set when the nXYTER trigger circuit detected two hits
          * during the peak sense time interval of the slow amplitude channel.
          * This flag indicates that separation between two hits was smaller than
          * what can be handled by the amplitude channel. \sa getNxOverflow() const
          */
         inline uint8_t getNxPileup() const { return getBit(45); }

         //! For Hit data: Returns nXYTER overflow flag (1 bit field)
         /*!
          * This flag is set when a nXYTER channels detects a hit but this hit
          * can't be stored because the 4 stage channel FIFO is full. This flag
          * indicates that the hit data rate was higher than the chip can handle
          * over a longer time. \sa getNxPileup() const
          */
         inline uint8_t getNxOverflow() const { return getBit(46); }

         //! For Hit data: Returns nXYTER last-epoch flag (1 bit field)
         /*!
          * This flag is set when the hit actually belong to the previous epoch.
          * Use the getMsgFullTime(uint32_t) const method to determine the
          * expanded and adjusted timestamp.
          */
         inline uint8_t getNxLastEpoch() const { return getBit(47); }


         //! For Hit data: Sets nXYTER number (2 bit field)
         inline void setNxNumber(uint8_t v) { setField(6, 2, v); }

         //! For Hit data: Sets 3 most significant bits of ROC LTS (3 bit field)
         inline void setNxLtsMsb(uint8_t v) { setField(8, 3, v); }

         //! For Hit data: Sets nXYTER time stamp (14 bit field)
         inline void setNxTs(uint16_t v) { setField(11, 14, v); }

         //! For Hit data: Sets nXYTER channel number (7 bit field)
         inline void setNxChNum(uint8_t v) { setField(25, 7, v); }

         // 1 bit unused

         //! For Hit data: Sets ADC value (12 bit field)
         inline void setNxAdcValue(uint16_t v) { setField(33, 12, v); }

         //! For Hit data: Sets nXYTER pileup flag (1 bit field)
         inline void setNxPileup(uint8_t v) { setBit(45, v); }

         //! For Hit data: Sets nXYTER overflow flag (1 bit field)
         inline void setNxOverflow(uint8_t v) { setBit(46,v ); }

         //! For Hit data: Sets nXYTER last-epoch flag (1 bit field)
         inline void setNxLastEpoch(uint8_t v) { setBit(47, v); }

         // ---------- Epoch marker access methods ------------

         // 2 bit unused

         //! For Epoch data: Returns current epoch number (32 bit field)
         // inline uint32_t getEpochNumber() const { return getField(8, 32); }
         // on some machines 32-bit field is not working
         inline uint32_t getEpochNumber() const { return data >> 8; }


         //! For Epoch data: Returns number of missed hits (8 bit field)
         inline uint8_t getEpochMissed() const { return getField(40, 8); }

         //! For Epoch data: Sets epoch number (32 bit field)
         inline void setEpochNumber(uint32_t v) { setField(8, 32, v); }

         //! For Epoch data: Sets missed hits count (8 bit field)
         inline void setEpochMissed(uint8_t v) { setField(40, 8, v); }


         // ---------- Sync marker access methods -------------

         //! For Sync data: Returns sync channel number (2 bit field)
         inline uint8_t getSyncChNum() const { return getField(6, 2); }

         //! For Sync data: Returns sync time stamp (13 bit field, 2 ns bins)
         inline uint16_t getSyncTs() const { return getField(8, 13) << 1; }

         //! For Sync data: Returns LSB of true epoch (1 bit field)
         inline uint8_t getSyncEpochLSB() const { return getBit(21); }

         //! For Sync data: Returns sync message data (24 bit field)
         inline uint32_t getSyncData() const { return getField(22, 24); }

         //! For Sync data: Returns sync status flags (2 bit field)
         inline uint8_t getSyncStFlag() const { return getField(46, 2); }


         //! For Sync data: Set sync channel number (2 bit field)
         inline void setSyncChNum(uint8_t v) { setField(6, 2, v); }

         //! For Sync data: Set sync time stamp (13 bit field, 2 ns bins )
         inline void setSyncTs(uint16_t v) { setField(8, 13, v >> 1); }

         //! For Sync data: Set LSB of true epoch (1 bit field)
         inline void setSyncEpochLSB(uint8_t v) { setBit(21, v); }

         //! For Sync data: Set sync message data (24 bit field)
         inline void setSyncData(uint32_t v) { setField(22, 24, v); }

         //! For Sync data: Set sync status flags (2 bit field)
         inline void setSyncStFlag(uint8_t v)  { setField(46, 2, v); }


         // ---------- AUX marker access methods --------------

         //! For Aux data: Returns aux channel number (7 bit field)
         inline uint8_t getAuxChNum() const { return getField(6, 7); }

         //! For Aux data: Returns aux time stamp (13 bit field, 2 ns bins)
         inline uint16_t getAuxTs() const { return getField(13, 13) << 1; }

         //! For Aux data: Returns LSB of true epoch (1 bit field)
         inline uint8_t getAuxEpochLSB() const { return getBit(26); }

         //! For Aux data: Returns falling edge flag (1 bit flag)
         inline uint8_t getAuxFalling() const { return getBit(27); }

         //! For Aux data: Returns overflow flag (1 bit field)
         inline uint8_t getAuxOverflow() const { return getBit(28); }


         //! For Aux data: Set aux channel number (7 bit field)
         inline void setAuxChNum(uint8_t v) { setField(6, 7, v); }

         //! For Aux data: Set aux time stamp (13 bit field)
         inline void setAuxTs(uint16_t v) { setField(13, 13, v >> 1); }

         //! For Aux data: Set LSB of true epoch (1 bit field)
         inline void setAuxEpochLSB(uint8_t v) { setBit(26, v); }

         //! For Aux data: Set falling edge flag (1 bit flag)
         inline void setAuxFalling(uint8_t v) { setBit(27, v); }

         //! For Aux data: Set overflow flag (1 bit field)
         inline void setAuxOverflow(uint8_t v) { setBit(28, v); }

         // ---------- Epoch2 marker access methods ------------

         //! For Epoch2 data: Returns epoch-lost flag (1 bit field)
         inline uint32_t getEpoch2EpochLost() const { return getBit(5); }

         //! For Epoch2 data: Returns data-lost flag (1 bit field)
         inline uint32_t getEpoch2DataLost() const { return getBit(6); }

         //! For Epoch2 data: Returns sync flag (1 bit field)
         inline uint32_t getEpoch2Sync() const { return getBit(7); }

         //! For Epoch2 data: Returns the LTS156 bits 11 to 8. This
			//! gives information at what time in the Epoche the epoche number
			//! was set (4 bit field)
         inline uint32_t getEpoch2StampTime() const { return getField(8, 4); }

         //! For Epoch2 data: Returns the epoch number (28 bit field)
         // inline uint32_t getEpoch2Number() const { return getField(12, 20); }
         // on some machines 32-bit field is not working
         // inline uint32_t getEpoch2Number() const { return getField(12, 32); }
         inline uint32_t getEpoch2Number() const { return data >> 12; }

         //! For Epoch2 data: Returns the number of the GET4 chip that send
			//! the epoche message (8 bit field)
         inline uint32_t getEpoch2ChipNumber() const { return getField(44, 4); }

         //! For Epoch2 data: Returns the CRC-8 of the rest of the message 
			//! (not yet implemented in HW) (8 bit field)
         inline uint32_t getEpoch2CRC() const { return getField(40, 8); }

         //! For Epoch2 data: Set epoch-lost flag (1 bit field)
         inline void setEpoch2EpochLost(uint32_t v) { setBit(5, v); }

         //! For Epoch2 data: Set data-lost flag (1 bit field)
         inline void setEpoch2DataLost(uint32_t v) { setBit(6, v); }

         //! For Epoch2 data: Set sync flag (1 bit field)
         inline void setEpoch2Sync(uint32_t v) { setBit(7, v); }

         //! For Epoch2 data: Set the LTS156 bits 11 to 8. This
			//! gives information at what time in the Epoche the epoche number
			//! was set (4 bit field)
         inline void setEpoch2StampTime(uint32_t v) { setField(8, 4, v); }

         //! For Epoch2 data: Set the epoch number (20 bit field)
         inline void setEpoch2Number(uint32_t v) { setField(12, 32, v); }

         //! For Epoch2 data: Set the number of the GET4 chip that send
			//! the epoche message (8 bit field)
         inline void setEpoch2ChipNumber(uint32_t v) { setField(44, 4, v); }

         //! For Epoch2 data: Set the CRC-8 of the rest of the message 
			//! (not yet implemented in HW) (8 bit field)
         inline void setEpoch2CRC(uint32_t v) { setField(40, 8, v); }

         // ---------- Get4 Hit data access methods ----------------

         //! For Get4 data: Returns Get4 chip number (6 bit field)
         inline uint8_t getGet4Number() const { return getField(6, 6); }

         //! For Get4 data: Returns Get4 channel number (2 bit field)
         inline uint8_t getGet4ChNum() const { return getField(12, 2); }

         //! For Get4 data: Returns Get4 time stamp, 50 ps binning (19 bit field)
         inline uint32_t getGet4Ts() const { return getField(14, 19); }

         //! For Get4 data: Returns Get4 rising or falling edge (1 bit field)
         inline uint32_t getGet4Edge() const { return getBit(33); }

         //! For Get4 data: Returns the CRC-8 of the rest of the message.
			//! For details check the ROC documentation. (8 bit field)
         inline uint32_t getGet4CRC() const { return getField(40, 8); }


         //! For Get4 data: Sets Get4 chip number (6 bit field)
         inline void setGet4Number(uint8_t v) { setField(6, 6, v); }

         //! For Get4 data: Sets Get4 channel number (2 bit field)
         inline void setGet4ChNum(uint8_t v) { setField(12, 2, v); }

         //! For Get4 data: Sets Get4 time stamp, 50 ps binning (19 bit field)
         inline void setGet4Ts(uint32_t v) { setField(14, 19, v); }

         //! For Get4 data: Sets Get4 rising or falling edge (1 bit field)
         inline void setGet4Edge(uint32_t v) { setBit(33, v); }

         //! For Get4 data: Set the CRC-8 of the rest of the message 
			//! For details check the ROC documentation. (8 bit field)
         inline void setGet4CRC(uint32_t v) { setField(40, 8, v); }

         // ---------- System message access methods ----------

         // 2 bit unused

         //! For SysMes data: Returns system message subtype (8 bit field)
         inline uint8_t getSysMesType() const { return getField(8, 8); }

         //! For SysMes data: Returns system message data (32 bit field)
         // inline uint32_t getSysMesData() const { return getField(16, 32); }
         // on some machine 32-bit field not working
         inline uint32_t getSysMesData() const { return data >> 16; }

         //! For SysMes data: Set system message type (8 bit field)
         inline void setSysMesType(uint8_t v) { setField(8, 8, v); }

         //! For SysMes data: Set system message data (32 bit field)
         inline void setSysMesData(uint32_t v) { setField(16, 32, v); }


         // ---------- Common functions -----------------------

         //! Returns \a true is message type is #MSG_NOP (filler message)
         inline bool isNopMsg() const { return getMessageType() == MSG_NOP; }
         //! Returns \a true is message type is #MSG_HIT (nXYTER hit data)
         inline bool isHitMsg() const { return getMessageType() == MSG_HIT; }
         //! Returns \a true is message type is #MSG_EPOCH (epoch marker)
         inline bool isEpochMsg() const { return getMessageType() == MSG_EPOCH;}
         //! Returns \a true is message type is #MSG_SYNC
         inline bool isSyncMsg() const { return getMessageType() == MSG_SYNC; }
         //! Returns \a true is message type is #MSG_AUX
         inline bool isAuxMsg() const { return getMessageType() == MSG_AUX; }
         //! Returns \a true is message type is #MSG_EPOCH2 (epoch2 marker)
         inline bool isEpoch2Msg() const { return getMessageType() == MSG_EPOCH2;}
         //! Returns \a true is message type is #MSG_GET4 (Get4 hit data)
         inline bool isGet4Msg() const { return getMessageType() == MSG_GET4; }
         //! Returns \a true is message type is #MSG_SYS (system message)
         inline bool isSysMsg() const { return getMessageType() == MSG_SYS; }

         //! Returns \a true if system message and subtype #ROC_SYSMSG_DAQ_START
         inline bool isStartDaqMsg() const
           { return isSysMsg() && (getSysMesType() == SYSMSG_DAQ_START); }
         //! Returns \a true if system message and subtype #ROC_SYSMSG_DAQ_FINISH
         inline bool isStopDaqMsg() const
           { return isSysMsg() && (getSysMesType() == SYSMSG_DAQ_FINISH); }


         void printData(unsigned kind = msg_print_Prefix | msg_print_Data, uint32_t epoch = 0) const;

         void printData(std::ostream& os, unsigned kind = msg_print_Human, uint32_t epoch = 0) const;

         uint64_t getMsgFullTime(uint32_t epoch) const;

         double getMsgFullTimeD(uint32_t epoch) const;

         //! Expanded timestamp for 250 MHz * 14 bit epochs
         inline static uint64_t FullTimeStamp(uint32_t epoch, uint16_t stamp)
            { return ((uint64_t) epoch << 14) | (stamp & 0x3fff); }

         //! Expanded timestamp for 250/8*5 MHz * 19 bit epochs
         inline static uint64_t FullTimeStamp2(uint32_t epoch, uint32_t stamp)
            { return ((uint64_t) epoch << 19) | (stamp & 0x7ffff); }


         static uint64_t CalcDistance(uint64_t start, uint64_t stop);

         static double CalcDistanceD(double start, double stop);

         // -------------------- methods for working with different formats

         static uint32_t RawSize(MessageFormat fmt);

         bool convertFromOld(void* src);

         bool convertToOld(void* tgt);

         bool assign(void* src, MessageFormat fmt = formatNormal);

         bool copyto(void* tgt, MessageFormat fmt = formatNormal);
   };

}


#endif
