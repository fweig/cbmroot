#// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmROCUnpackSts                         -----
// -----                    Created 13.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmROCUnpackSts.h"

#include "CbmStsDigi.h"
#include "CbmTbDaqBuffer.h"
#include "CbmSourceLmdNew.h"
#include "CbmDaqMap.h"

#include "CbmStsAddress.h"

#include "FairRunOnline.h"
#include "FairLogger.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

CbmROCUnpackSts::CbmROCUnpackSts()
: CbmROCUnpack(),
  fBuffer(CbmTbDaqBuffer::Instance()),
  fDaqMap(NULL),
  fSource(NULL),
  fStsDigis(nullptr),
  fStsBaselineDigis(nullptr)
{
}
  
CbmROCUnpackSts::~CbmROCUnpackSts()
{
}
  
  
Bool_t CbmROCUnpackSts::Init()
{
  FairRunOnline* online = FairRunOnline::Instance();
  fSource = static_cast<CbmSourceLmdNew*>(online->GetSource());

  fDaqMap = fSource->GetDaqMap();

//  FairRootManager* ioman = FairRootManager::Instance();
  fStsDigis = RegisterOutput<CbmStsDigi>("StsDigi");
  fStsBaselineDigis = RegisterOutput<CbmStsDigi>("StsBaselineDigi");

/*
  ioman->Register("StsDigi", "STS raw data", fStsDigis, fPersistence);
  ioman->Register("StsBaselineDigi", "STS baseline data",
                  fStsBaselineDigis, fPersistence);
*/

  return kTRUE;
}
 
Bool_t CbmROCUnpackSts::DoUnpack(roc::Message* Message, ULong_t hitTime)
{
  // --- Get absolute time, NXYTER and channel number
  Int_t rocId        = Message->getRocNumber();
  //  ULong64_t hitTime  = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
  Int_t nxyterId     = Message->getNxNumber();
  Int_t nxChannel    = Message->getNxChNum();
  Int_t charge       = Message->getNxAdcValue();

  // --- Check for epoch marker for this ROC
  fSource->CheckCurrentEpoch(rocId);

  // --- Get detector element from DaqMap
  Int_t station = fDaqMap->GetStsStation(rocId);
  Int_t sector  = 0;
  Int_t side    = fDaqMap->GetStsSensorSide(rocId);
  Int_t channel = fDaqMap->GetStsChannel(rocId, nxyterId, nxChannel);


  Int_t iStation;
  Int_t iSector;
  Int_t iPlane;
  Int_t iFiber;

  fDaqMap->Map(rocId, nxyterId, nxChannel, iStation, iSector, iPlane, iFiber);

  if( station != iStation ||  sector != iSector ||
      side != iPlane || channel != iFiber ) {
    LOG(info) << "Roc, Nxyter: " << rocId << ", " << nxyterId;
    LOG(info) << "Station: " << station << ", " << iStation;
    LOG(info) << "Sector: " << sector << ", " << iSector;
    LOG(info) << "Side: " << side << ", " << iPlane;
    LOG(info) << "Channel: " << channel << ", " << iFiber;
  } 
  // --- Create a STS digi and send it to the buffer
  UInt_t address = CbmStsAddress::GetAddress(station,
                   0,   // ladder
                   0,   // halfladder
                   0,   // module
                   0,   // sensor
                   side);
  CbmStsDigi* digi = new CbmStsDigi(address, channel, hitTime, charge);


  // In case of normal data insert the digi into the buffer.
  // In case of baseline data insert the digi only if the roc 
  // is already in baseline mode.  
  if ( !fSource->IsBaselineFill() ) {
    fBuffer->InsertData<CbmStsDigi>(digi);
  } else {
    if ( fSource->IsBaselineFill(rocId) ) { 
      fBuffer->InsertData<CbmStsDigi>(digi); 
    } else {
      fSource->AddDiscardedDigi(ECbmModuleId::kSts);
    }
  }

  return kTRUE;
}

void CbmROCUnpackSts::FillOutput(boost::any _digi)
{
  CbmStsDigi* digi = boost::any_cast<CbmStsDigi*>(_digi);

  if ( !fSource->IsBaselineRetrieve() ) {
    fStsDigis->emplace_back(*digi);
/*
    new( (*fStsDigis)[fStsDigis->GetEntriesFast()])
      CbmStsDigi(*(boost::any_cast<CbmStsDigi*>(digi)));
*/
  } else {
    fStsBaselineDigis->emplace_back(*digi);
/*
      new( (*fStsBaselineDigis)[fStsBaselineDigis->GetEntriesFast()])
      CbmStsDigi(*(boost::any_cast<CbmStsDigi*>(digi)));
*/
  }
  delete digi;
}

void CbmROCUnpackSts::Reset()
{
  // The next block is needed do to the problem that the TClonesArray is
  // very large after usage (2.5 M entries). This somehow slows down
  // the Clear of the container even if no entries are inside by 2-3 orders
  // of magnitude which slows down the execution of the whole program by 
  // more then a factor of 10.
  // TODO:
  // It has to be checked if this is a bug in Root by producing a small
  // example program to demonstarete the issue
  if (fStsBaselineDigis->size() > 1000) {
    fStsBaselineDigis->resize(10);
/*
    fStsBaselineDigis->Delete();
    fStsBaselineDigis->Expand(10);
*/
  } 
/*
  fStsDigis->Clear();
  fStsBaselineDigis->Clear();
*/
  fStsDigis->clear();
  fStsBaselineDigis->clear();
}

ClassImp(CbmROCUnpackSts)
