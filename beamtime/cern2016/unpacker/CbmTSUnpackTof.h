// -----                                                                   -----
// -----                        CbmTSUnpackTof                             -----
// -----               Created 27.10.2016 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTSUnpackTof_H
#define CbmTSUnpackTof_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v1.h"

#include "CbmTSUnpack.h"
#include "CbmHistManager.h"
#include "CbmNxyterRawMessage.h"
#include "CbmTofDigi.h"
#include "CbmTbDaqBuffer.h"

#include "TClonesArray.h"

#include <vector>
#include <map>

class CbmTofUnpackPar;

class CbmTSUnpackTof : public CbmTSUnpack
{
public:
  
  CbmTSUnpackTof( UInt_t uNbGdpb = 1 );
  virtual ~CbmTSUnpackTof();
    
  virtual Bool_t Init();

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

  virtual void Reset();

  virtual void Finish();

  void SetParContainers();

  Bool_t InitContainers();

  Bool_t ReInitContainers();


  void FillOutput(boost::any);
 
  void SetEpochSuppressedMode( Bool_t bEnable = kTRUE ) { fbEpochSuppModeOn = bEnable; }
 
  void SetMsLimitLevel( size_t uAcceptBoundaryPct = 100 ) { fuMsAcceptsPercent = uAcceptBoundaryPct; }
  size_t GetMsLimitLevel( ) { return fuMsAcceptsPercent; }

  void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
  size_t GetMsOverlap()                      { return fuOverlapMsNb; }
    
  inline void SetTShiftRef(Double_t val) {fdTShiftRef = val;}

private: 

  size_t fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
  size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
  UInt_t fuMinNbGdpb;
  UInt_t fuCurrNbGdpb;

  std::vector<int> fMsgCounter;
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap;

  CbmHistManager* fHM;  ///< Histogram manager

  /** Current epoch marker for each ROC and GET4 (first epoch in the stream initialises the map item) **/
  std::map<Int_t, std::map<Int_t, UInt_t> > fCurrentEpoch; 
  Int_t fNofEpochs;              /** Current epoch marker for each ROC **/
  ULong_t fCurrentEpochTime;     /** Time stamp of current epoch **/

  Int_t fEquipmentId;
  Double_t fdTShiftRef;

//  TClonesArray* fFiberHodoRaw;
  std::vector<CbmTofDigi>* fTofDigi;
//  CbmNxyterRawMessage* fRawMessage;  
  CbmTofDigi* fDigi;

  CbmTbDaqBuffer* fBuffer;
  
  CbmTofUnpackPar* fUnpackPar;      //!

  void CreateHistograms();
    
  Bool_t fbEpochSuppModeOn;

  std::vector< std::vector< std::vector < ngdpb::Message > > > fvmEpSupprBuffer;

  void FillHitInfo(ngdpb::Message);
  void FillStarTrigInfo(ngdpb::Message);
  void FillEpochInfo(ngdpb::Message);
  void PrintSlcInfo(ngdpb::Message);
  void PrintSysInfo(ngdpb::Message);
  void PrintGenInfo(ngdpb::Message);

  CbmTSUnpackTof(const CbmTSUnpackTof&);
  CbmTSUnpackTof operator=(const CbmTSUnpackTof&);

  ClassDef(CbmTSUnpackTof, 1)
};

#endif
