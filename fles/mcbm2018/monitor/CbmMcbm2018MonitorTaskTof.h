// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018MonitorTaskTof                      -----
// -----               Created 27.11.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorTaskTof_H
#define CbmMcbm2018MonitorTaskTof_H

#include "Timeslice.hpp"

#include "CbmMcbmUnpack.h"

#include "TString.h"

class CbmMcbm2018MonitorAlgoTof;

class CbmMcbm2018MonitorTaskTof : public CbmMcbmUnpack
{
   public:

      CbmMcbm2018MonitorTaskTof();
      CbmMcbm2018MonitorTaskTof(const CbmMcbm2018MonitorTaskTof&) = delete;
      CbmMcbm2018MonitorTaskTof operator=(const CbmMcbm2018MonitorTaskTof&) = delete;
      virtual ~CbmMcbm2018MonitorTaskTof();

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

      /// Algo settings setters
      inline void SetDebugMonitorMode( Bool_t bFlagIn = kTRUE ) { fbDebugMonitorMode = bFlagIn; }
      inline void SetIgnoreCriticalErrors( Bool_t bFlagIn = kTRUE ) { fbIgnoreCriticalErrors = bFlagIn; }
      void SetIgnoreOverlapMs( Bool_t bFlagIn = kTRUE );
      inline void SetHistoryHistoSize( UInt_t inHistorySizeSec = 1800 ) { fuHistoryHistoSize = inHistorySizeSec; }
      inline void SetHistoFilename( TString sNameIn ) { fsHistoFileName = sNameIn; }
      inline void SetPulserTotLimits( UInt_t uMin, UInt_t uMax ) { fuMinTotPulser = uMin; fuMaxTotPulser = uMax; }
      inline void SetGdpbIndex( Int_t iGdpb = -1 ) { fiGdpbIndex = iGdpb; }

   private:

      Bool_t SaveHistograms();

      /// Control flags
      Bool_t fbDebugMonitorMode; //! Switch ON the filling of a additional set of histograms
      Bool_t fbIgnoreCriticalErrors; //! If ON not printout at all for critical errors

      /// User settings parameters
      UInt_t  fuHistoryHistoSize;
      TString fsHistoFileName;
      UInt_t fuMinTotPulser;
      UInt_t fuMaxTotPulser;
      Int_t  fiGdpbIndex;

      /// Statistics & first TS rejection
      uint64_t fulTsCounter;

      /// Processing algo
      CbmMcbm2018MonitorAlgoTof * fMonitorAlgo;

      ClassDef(CbmMcbm2018MonitorTaskTof, 1)
};

#endif // CbmMcbm2018MonitorTaskTof_H
