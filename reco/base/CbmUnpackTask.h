// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmUnpackTask                              -----
// -----               Created 11.02.2020 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMUNPACKTASK_H
#define CBMUNPACKTASK_H

/// CbmRoot (+externals) headers
#include "CbmUnpackTaskBase.hpp"
#include "Timeslice.hpp"

/// FairRoot headers

/// Fairsoft (Root, Boost, ...) headers
#include "TObject.h"

/// C/C++ headers
#include <vector>

//#include "CbmMcbmUnpack.h" <= Move relevant parts as template code here or in virtual base class!

template< class TDigi, class TAlgo, class TParam >
class CbmUnpackTask : public CbmUnpackTaskBase
{
   public:

      CbmUnpackTask( TString sDigiBranchName, TString sDigiBranchDescr = "" );

      CbmUnpackTask(const CbmUnpackTask&) = delete;
      CbmUnpackTask operator=(const CbmUnpackTask&) = delete;

      ~CbmUnpackTask();

      Bool_t Init();
      Bool_t DoUnpack( const fles::Timeslice& ts );
      void Reset();

      void Finish();

      void SetParContainers();
      Bool_t InitContainers();
      Bool_t ReInitContainers();

      /// Temp until we change from CbmMcbmUnpack to something else
      void AddMsComponentToList( size_t /*component*/, UShort_t /*usDetectorId*/ ){};
      void SetNbMsInTs( size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/){};

      inline void SetMonitorMode( Bool_t bFlagIn = kTRUE ) { fbMonitorMode = bFlagIn; }
      void SetIgnoreOverlapMs( Bool_t bFlagIn = kTRUE );

//      void SetTimeOffsetNs( Double_t dOffsetIn = 0.0 ); <= Specialization method?

      /// Task settings
      void SetWriteOutputFlag( Bool_t bFlagIn ) { fbWriteOutput = bFlagIn; }

   private:
      /// Control flags
      Bool_t fbMonitorMode      = kFALSE; //! Switch ON the filling of a minimal set of histograms in the algo
      Bool_t fbDebugMonitorMode = kFALSE; //! Switch ON the filling of a additional set of histograms in the algo
      Bool_t fbWriteOutput      = kTRUE;  //! If ON the output vector of digi is written to disk by FairRoot, otherwise just made available for higher stages

      /// Statistics & first TS rejection
      uint64_t fulTsCounter     = 0;      //! TS counter, not same as TS index!

      /// Input/Output vector
      std::vector< TDigi > * fvDigiIO = nullptr;   //! IO vector of Digis, passed to algo for filling and propagated to framework for output
      std::vector< CbmErrorMessage > * fvErrorIO = nullptr;   //! IO vector of Errors, passed to algo for filling and propagated to framework for output

      /// Histograms for vector monitoring?
//      TH1* fhArraySize     = nullptr;
//      TH1* fhArrayCapacity = nullptr;

      /// Processing algo
      TAlgo * fUnpackerAlgo          = nullptr; //! pointer to unpacking algo

      /// Names for framework objects search
      TString fsDigiBranchName  = "";
      TString fsDigiBranchDescr = "";
};

/// INclude implementation of templated methods
#include "CbmUnpackTask.tmpl"

#endif // CBMUNPACKTASK_H
