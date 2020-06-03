#ifndef BASE_COMMONS_H
#define BASE_COMMONS_H

/** This is a place for common constants, types
  * One should try to keep file generic and small - not everything should go here,
  * while later it will be difficult to avoid them */

namespace roc3587 {
   //! value of iProcId if mbs subevent.
   /*!
    * Data, taken from ROC by DABC, packed into mbs events and stored in lmd file.
    * MBS event consists from one or several subevents.
    * Each subevent has three fields - iProcId (16bit), iSubcrate (8bit) and iControl (8bit)
    * iProcId used to code data origin - see comment for  /ref ERocMbsTypes
    * iSubcrate contains roc number (rocid)
    * iControl stores data format see /ref MessageFormat
    */
   enum ERocMbsTypes {
      proc_RocEvent     =  1,   //!< complete event from one roc board (iSubcrate = rocid)
      proc_ErrEvent     =  2,   //!< one or several events with corrupted data inside (iSubcrate = rocid)
      proc_MergedEvent  =  3,   //!< sorted and synchronized data from several rocs (iSubcrate = upper rocid bits)
      proc_RawData      =  4,   //!< unsorted uncorrelated data from one ROC, no SYNC markers required (mainly for FEET case)
      proc_Triglog      =  5,   //!< subevent produced by MBS directly with sync number and trigger module scalers
      proc_TRD_MADC     =  6,   //!< subevent produced by MBS directly with CERN-Nov10 data
      proc_TRD_Spadic   =  7,   //!< collection of data from susibo board (spadic 0.3)
      proc_CERN_Oct11   =  8,   //!< id for CERN beamtime in October 11
      proc_SlaveMbs     =  9,   //!< subevent produce by slave MBS system, which emulates number of triglog module
      proc_EPICS        = 10,   //!< subevent produced by dabc EPICS plugin (ezca)
      proc_COSY_Nov11   = 11,   //!< id for COSY beamtime in November 11
      proc_SpadicV10Raw = 12,   //!< raw data from single spadic V1.0 chip
      proc_SpadicV10Event = 13, //!< dabc-packed data for spadic V1.0 from SP605
      proc_CERN_Oct12   = 14,   //!< id for CERN beamtime in October 12
      proc_FASP         = 15,   //!< id for the FASP data
      proc_TRBEvent = 31     //!< container for TRB frontend data
   };
}

#endif
