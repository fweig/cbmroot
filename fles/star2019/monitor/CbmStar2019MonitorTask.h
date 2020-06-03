// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmStar2019MonitorTask                        -----
// -----               Created 12.10.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmStar2019MonitorTask_H
#define CbmStar2019MonitorTask_H

#include "Timeslice.hpp"

#include "CbmMcbmUnpack.h"

#include "TString.h"

class CbmStar2019MonitorAlgo;
class CbmTbDaqBuffer;
class TClonesArray;

class CbmStar2019MonitorTask : public CbmMcbmUnpack
{
   public:

      CbmStar2019MonitorTask();
      virtual ~CbmStar2019MonitorTask();

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
      inline void SetSectorIndex( Int_t iSector = -1 ) { fiSectorIndex = iSector; }

      Bool_t SaveLatencyHistograms( TString sFilename );

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
      Int_t  fiSectorIndex;

      /// Parameters management
      TList* fParCList;

      /// Statistics & first TS rejection
      uint64_t fulTsCounter;

      /// Processing algo
      CbmStar2019MonitorAlgo * fMonitorAlgo;

      CbmStar2019MonitorTask(const CbmStar2019MonitorTask&);
      CbmStar2019MonitorTask operator=(const CbmStar2019MonitorTask&);

      ClassDef(CbmStar2019MonitorTask, 1)
};

#endif // CbmStar2019MonitorTask_H
