// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTofStarData2018                         -----
// -----               Created 13.01.2017 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTofStarData2018_H
#define CbmTofStarData2018_H

#include "rocMess_wGet4v2.h"
#include "CbmTofStarData.h"

#include "Rtypes.h"

#include <vector>


class CbmTofStarTrigger2018 : public CbmTofStarTrigger
{
   // Methods implementation can be found at the end of CbmTsMonitorTofStar.cxx!
   public:
      // Constructors
      CbmTofStarTrigger2018( ULong64_t ulGdpbTsFullIn, ULong64_t ulStarTsFullIn,
                             UInt_t    uStarTokenIn,   UInt_t    uStarDaqCmdIn,
                             UInt_t    uStarTrigCmdIn );

      // Destructor
      ~CbmTofStarTrigger2018() {};

      // Setters => Provided by CbmTofStarTrigger

      // Accessors => Mostly provided by CbmTofStarTrigger
      std::vector< gdpb::FullMessage > GetGdpbMessages( UShort_t usGdpbId ) const;

      // Operators
      bool operator<(const CbmTofStarTrigger2018& other) const;

   private:

//      CbmTofStarTrigger2018(const CbmTofStarTrigger2018&);
//      CbmTofStarTrigger2018 operator=(const CbmTofStarTrigger2018&);

//   ClassDef(CbmTofStarTrigger2018, 1)
};

class CbmTofStarSubevent2018
{
   // Methods implementation can be found at the end of CbmTsMonitorTofStar.cxx!
   public:
      // Constructors
      CbmTofStarSubevent2018();
      CbmTofStarSubevent2018( CbmTofStarTrigger triggerIn, UShort_t sourceIdIn );

      // Destructor
      ~CbmTofStarSubevent2018();

      // Setters
      inline void SetTrigger( CbmTofStarTrigger triggerIn ){ fTrigger = triggerIn; fbTriggerSet = kTRUE; }
      inline void SetSource( UShort_t sourceIdIn ){ fusSourceId = sourceIdIn;
		                                              fulEventStatusFlags =   (fulEventStatusFlags & !( kulSourceIdMask << kulSourceIdOffset ) )
		                                                                    | ( ( sourceIdIn & kulSourceIdMask ) << kulSourceIdOffset );
		                                            }
      inline void SetBadEventFlag(  Bool_t bFlagState = kTRUE ){
                        bFlagState ? (fulEventStatusFlags |= kulFlagBadEvt) :
                                     (fulEventStatusFlags &= ~(kulFlagBadEvt) ); }
      inline void SetOverlapEventFlag(  Bool_t bFlagState = kTRUE ){
                        bFlagState ? (fulEventStatusFlags |= kulFlagOverlapEvt) :
                                     (fulEventStatusFlags &= ~(kulFlagOverlapEvt) ); }
      inline void SetEmptyEventFlag(  Bool_t bFlagState = kTRUE ){
                        bFlagState ? (fulEventStatusFlags |= kulFlagEmptyEvt) :
                                     (fulEventStatusFlags &= ~(kulFlagEmptyEvt) ); }
      inline void SetStartBorderEventFlag(  Bool_t bFlagState = kTRUE ){
                        bFlagState ? (fulEventStatusFlags |= kulFlagStartBorderEvt) :
                                     (fulEventStatusFlags &= ~(kulFlagStartBorderEvt) ); }
      inline void SetEndBorderEventFlag(  Bool_t bFlagState = kTRUE ){
                        bFlagState ? (fulEventStatusFlags |= kulFlagEndBorderEvt) :
                                     (fulEventStatusFlags &= ~(kulFlagEndBorderEvt) ); }
      inline void AddMsg( gdpb::FullMessage & msgIn){ fvMsgBuffer.push_back( msgIn ); }

      // Accessors
      inline CbmTofStarTrigger GetTrigger() const { return fTrigger;}
      inline UShort_t          GetSource() const { return fusSourceId; }
      inline Bool_t            GetBadEventFlag()         const { return 0 < (fulEventStatusFlags & kulFlagBadEvt); }
      inline Bool_t            GetOverlapEventFlag()     const { return 0 < (fulEventStatusFlags & kulFlagOverlapEvt); }
      inline Bool_t            GetEmptyEventFlag()       const { return 0 < (fulEventStatusFlags & kulFlagEmptyEvt); }
      inline Bool_t            GetStartBorderEventFlag() const { return 0 < (fulEventStatusFlags & kulFlagStartBorderEvt); }
      inline Bool_t            GetEndBorderEventFlag()   const { return 0 < (fulEventStatusFlags & kulFlagEndBorderEvt); }
      inline Int_t             GetEventSize()            const { return fuEventSizeBytes; }
      inline gdpb::Message     GetMsg( UInt_t uMsgIdx ) const;
      inline UInt_t            GetMsgBuffSize() const { return fvMsgBuffer.size();}
      inline static uint32_t   GetMaxOutputSize() { return kuMaxOutputSize;}

      // Content clearing
      void   ClearSubEvent();

      // Sort content of output buffer
      void   SortMessages();

      // Sub-event output
      void * BuildOutput( Int_t & iOutputSizeBytes );

      // Sub-event input
      Bool_t LoadInput( void * pBuff, Int_t iInputSizeBytes );

      // Print the heade and the messages in the buffer
      void   PrintSubEvent();

   private:
      static const uint32_t         kuMaxOutputSize       =     131072; // 2^17
      static const uint32_t         kuMaxNbMsgs           =       8190; // 4 * 64b in header => floor( (2^17 / 8 ) - 4) / 2
      static const uint64_t         kulFlagBadEvt         =   0x1 << 0;
      static const uint64_t         kulFlagOverlapEvt     =   0x1 << 1;
      static const uint64_t         kulFlagEmptyEvt       =   0x1 << 2;
      static const uint64_t         kulFlagStartBorderEvt =   0x1 << 3;
      static const uint64_t         kulFlagEndBorderEvt   =   0x1 << 4;
      static const uint64_t         kulSourceIdOffset     =         16;
      static const uint64_t         kulSourceIdMask       =     0xFFFF;
      static const uint64_t         kulEventSizeOffset    =         32;
      static const uint64_t         kulEventSizeMask      = 0xFFFFFFFF;

      Bool_t                        fbTriggerSet;
      CbmTofStarTrigger             fTrigger;
      UShort_t                      fusSourceId;
      ULong64_t                     fulEventStatusFlags;
      UInt_t                        fuEventSizeBytes;
      std::vector< gdpb::FullMessage > fvMsgBuffer;
      ULong64_t                     fpulBuff[kuMaxOutputSize];


      CbmTofStarSubevent2018(const CbmTofStarSubevent2018&);
      CbmTofStarSubevent2018 operator=(const CbmTofStarSubevent2018&);

//   ClassDef(CbmTofStarSubevent2018, 1)
};

#endif
