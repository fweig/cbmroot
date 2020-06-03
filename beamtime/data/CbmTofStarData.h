// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTofStarData                             -----
// -----               Created 13.01.2017 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTofStarData_H
#define CbmTofStarData_H

#include "rocMess_wGet4v1.h"

#include "Rtypes.h"

#include <vector>

class CbmTofStarTrigger
{
   // Methods implementation can be found at the end of CbmTsMonitorTofStar.cxx!
   public:
      // Constructors
      CbmTofStarTrigger( ULong64_t ulGdpbTsFullIn, ULong64_t ulStarTsFullIn,
                         UInt_t    uStarTokenIn,   UInt_t    uStarDaqCmdIn,
                         UInt_t    uStarTrigCmdIn );

      // Destructor
      ~CbmTofStarTrigger() {};

      // Setters
      inline void SetFullGdpbTs( ULong64_t ulGdpbTsFullIn ){ fulGdpbTsFull = ulGdpbTsFullIn; }
      inline void SetFullStarTs( ULong64_t ulStarTsFullIn ){ fulStarTsFull = ulStarTsFullIn; }
      inline void SetStarToken(  UInt_t    uStarTokenIn ){   fuStarToken   = uStarTokenIn; }
      inline void SetStarDaqCmd( UInt_t    uStarDaqCmdIn ){  fusStarDaqCmd  = uStarDaqCmdIn; }
      inline void SetStarTRigCmd(UInt_t    uStarTrigCmdIn ){ fusStarTrigCmd = uStarTrigCmdIn; }

      // Accessors
      inline ULong64_t GetFullGdpbTs()  const { return fulGdpbTsFull;}
      inline ULong64_t GetFullStarTs()  const { return fulStarTsFull;}
      inline UInt_t    GetStarToken()   const { return fuStarToken;}
      inline UShort_t  GetStarDaqCmd()  const { return fusStarDaqCmd;}
      inline UShort_t  GetStarTrigCmd() const { return fusStarTrigCmd;}
      UInt_t           GetStarTrigerWord() const;
      UInt_t           GetFullGdpbEpoch()  const;

      // Operators
      bool operator<(const CbmTofStarTrigger& other) const;

   private:
      ULong64_t fulGdpbTsFull;
      ULong64_t fulStarTsFull;
      UInt_t    fuStarToken;
      UShort_t  fusStarDaqCmd;
      UShort_t  fusStarTrigCmd;

//      CbmTofStarTrigger(const CbmTofStarTrigger&);
//      CbmTofStarTrigger operator=(const CbmTofStarTrigger&);

//   ClassDef(CbmTofStarTrigger, 1)
};

class CbmTofStarSubevent
{
   // Methods implementation can be found at the end of CbmTsMonitorTofStar.cxx!
   public:
      // Constructors
      CbmTofStarSubevent();
      CbmTofStarSubevent( CbmTofStarTrigger triggerIn );

      // Destructor
      ~CbmTofStarSubevent();

      // Setters
      inline void SetTrigger( CbmTofStarTrigger triggerIn ){ fTrigger = triggerIn; fbTriggerSet = kTRUE; }
      inline void SetBadEventFlag(  Bool_t bFlagState = kTRUE ){
                        bFlagState ? (fulEventStatusFlags |= kulFlagBadEvt) :
                                     (fulEventStatusFlags &= ~(kulFlagBadEvt) ); }
      inline void SetOverlapEventFlag(  Bool_t bFlagState = kTRUE ){
                        bFlagState ? (fulEventStatusFlags |= kulFlagOverlapEvt) :
                                     (fulEventStatusFlags &= ~(kulFlagOverlapEvt) ); }
      inline void SetEmptyEventFlag(  Bool_t bFlagState = kTRUE ){
                        bFlagState ? (fulEventStatusFlags |= kulFlagEmptyEvt) :
                                     (fulEventStatusFlags &= ~(kulFlagEmptyEvt) ); }
      inline void AddMsg( ngdpb::Message & msgIn){ fvMsgBuffer.push_back( msgIn ); }

      // Accessors
      inline CbmTofStarTrigger GetTrigger()  const { return fTrigger;}
      inline Bool_t            GetBadEventFlag() const { return (fulEventStatusFlags & kulFlagBadEvt); }
      inline Bool_t            GetOverlapEventFlag() const { return (fulEventStatusFlags & kulFlagOverlapEvt); }
      inline Bool_t            GetEmptyEventFlag() const { return (fulEventStatusFlags & kulFlagEmptyEvt); }
      inline ngdpb::Message    GetMsg( UInt_t uMsgIdx ) const;
      inline UInt_t            GetMsgBuffSize() const { return fvMsgBuffer.size();}
      inline static uint32_t   GetMaxOutputSize() { return kuMaxOutputSize;}

      // Content clearing
      void   ClearSubEvent();

      // Sub-event output
      void * BuildOutput( Int_t & iOutputSizeBytes );

      // Sub-event input
      Bool_t LoadInput( void * pBuff, Int_t iInputSizeBytes );

   private:
      static const uint32_t         kuMaxOutputSize   = 131072; // 2^17
      static const uint32_t         kuMaxNbMsgs       =  16380; // 4 * 64b in header => floor( (2^17 / 8 ) - 4)
      static const uint64_t         kulFlagBadEvt     = 0x1 << 0;
      static const uint64_t         kulFlagOverlapEvt = 0x1 << 1;
      static const uint64_t         kulFlagEmptyEvt   = 0x1 << 2;

      Bool_t                        fbTriggerSet;
      CbmTofStarTrigger             fTrigger;
      ULong64_t                     fulEventStatusFlags;
      std::vector< ngdpb::Message > fvMsgBuffer;
      ULong64_t                     fpulBuff[kuMaxOutputSize];


      CbmTofStarSubevent(const CbmTofStarSubevent&);
      CbmTofStarSubevent operator=(const CbmTofStarSubevent&);

//   ClassDef(CbmTofStarSubevent, 1)
};

#endif
