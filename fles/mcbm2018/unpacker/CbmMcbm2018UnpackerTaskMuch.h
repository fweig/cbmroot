// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018UnpackerTaskMuch                   -----
// -----               Created 02.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018UnpackerTaskMuch_H
#define CbmMcbm2018UnpackerTaskMuch_H

#include "Timeslice.hpp"

#include "CbmMcbmUnpack.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmErrorMessage.h"

class CbmMcbm2018UnpackerAlgoMuch;
//class TClonesArray;

class CbmMcbm2018UnpackerTaskMuch : public CbmMcbmUnpack
{
   public:

      CbmMcbm2018UnpackerTaskMuch( UInt_t uNbSdpb = 1 );

      CbmMcbm2018UnpackerTaskMuch(const CbmMcbm2018UnpackerTaskMuch&) = delete;
      CbmMcbm2018UnpackerTaskMuch operator=(const CbmMcbm2018UnpackerTaskMuch&) = delete;

      virtual ~CbmMcbm2018UnpackerTaskMuch();

      virtual Bool_t Init();
      virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
      virtual void Reset();

      virtual void Finish();

      void SetParContainers();

      Bool_t InitContainers();

      Bool_t ReInitContainers();

      /// Temp until we change from CbmMcbmUnpack to something else
      void AddMsComponentToList( size_t component, UShort_t usDetectorId );
      void SetNbMsInTs( size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/){};

      inline void SetMonitorMode( Bool_t bFlagIn = kTRUE ) { fbMonitorMode = bFlagIn; }
      void SetIgnoreOverlapMs( Bool_t bFlagIn = kTRUE );

      void SetTimeOffsetNs( Double_t dOffsetIn = 0.0 );
      void SetTimeOffsetNsAsic( UInt_t uAsicIdx, Double_t dOffsetIn = 0.0 );

      void EnableAsicType( Int_t fiFlag = 0 );
      /// Task settings
      void SetWriteOutputFlag( Bool_t bFlagIn ) { fbWriteOutput = bFlagIn; }

   private:
      /// Control flags
      Bool_t fbMonitorMode;  //! Switch ON the filling of a minimal set of histograms
      Bool_t fbDebugMonitorMode; //! Switch ON the filling of a additional set of histograms
      Bool_t fbWriteOutput; //! If ON the output TClonesArray of digi is written to disk

      /// Statistics & first TS rejection
      uint64_t fulTsCounter;

      /// Output vectors
      std::vector< CbmMuchBeamTimeDigi > * fpvDigiMuch  = nullptr;
      std::vector< CbmErrorMessage >     * fpvErrorMuch = nullptr;

      /// Processing algo
      CbmMcbm2018UnpackerAlgoMuch * fUnpackerAlgo;

      ClassDef(CbmMcbm2018UnpackerTaskMuch, 3)
};

#endif
