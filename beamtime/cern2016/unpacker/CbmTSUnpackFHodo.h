// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSUnpackTest                            -----
// -----                 Created 20.06.2016 by F. Uhlig                    -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTSUNPACKTEST_H
#define CBMTSUNPACKTEST_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v1.h"

#include "CbmTSUnpack.h"
#include "CbmHistManager.h"
#include "CbmNxyterRawMessage.h"
#include "CbmFiberHodoDigi.h"
#include "CbmTbDaqBuffer.h"

#include "TClonesArray.h"

#include <vector>
#include <map>

class CbmFHodoUnpackPar;

class CbmTSUnpackFHodo : public CbmTSUnpack
{
public:
  
  CbmTSUnpackFHodo();
  virtual ~CbmTSUnpackFHodo();
    
  virtual Bool_t Init();

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

  virtual void Reset();

  virtual void Finish();

  void SetParContainers();

  Bool_t InitContainers();

  Bool_t ReInitContainers();

  void FillOutput(boost::any);

  void CreateRawMessageOutput(Bool_t val) {fCreateRawMessage=val;}

private:

  std::vector<int> fMsgCounter;

  std::map<int,int> fHodoStationMap;
  
  Int_t fHodoFiber[128];  /** Mapping from fiber hodoscope feb channel to fiber number **/
  Int_t fHodoPlane[128];  /** Mapping from fiber hodoscope feb channel to plane number 1=X, 2=Y **/
  Int_t fHodoPixel[128];  /** Mapping from fiber hodoscope feb channel to pixel number **/

  CbmHistManager* fHM;  ///< Histogram manager

  std::map<Int_t, UInt_t> fCurrentEpoch; // Current epoch (first epoch in the stream initialises the map item)
  Int_t fNofEpochs; /** Current epoch marker for each ROC **/
  ULong_t fCurrentEpochTime;     /** Time stamp of current epoch **/

  Int_t fEquipmentId;

  TClonesArray* fFiberHodoRaw;
  TClonesArray* fFiberHodoDigi;
  CbmNxyterRawMessage* fRawMessage;  
  CbmFiberHodoDigi* fDigi;  

  CbmTbDaqBuffer* fBuffer;

  Bool_t fCreateRawMessage;

  CbmFHodoUnpackPar* fUnpackPar;    //!

  void CreateHistograms();

  void FillHitInfo(ngdpb::Message);
  void FillEpochInfo(ngdpb::Message);

  CbmTSUnpackFHodo(const CbmTSUnpackFHodo&);
  CbmTSUnpackFHodo operator=(const CbmTSUnpackFHodo&);

  ClassDef(CbmTSUnpackFHodo, 1)
};

#endif
