// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmROCUnpackEpoch                       -----
// -----                    Created 13.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmROCUnpackEpoch.h"

#include <FairLogger.h>         // for Logger, LOG
#include <stddef.h>             // for NULL
#include <iomanip>              // for setprecision, __iom_t5
#include <ios>                  // for fixed
#include "CbmSourceLmdNew.h"    // for CbmSourceLmdNew
#include <FairRunOnline.h>      // for FairRunOnline
#include "roc/Message.h"        // for Message


CbmROCUnpackEpoch::CbmROCUnpackEpoch()
  : CbmROCUnpack(),
    fSource(NULL),
    fNofEpochs(0)
{
}
  
CbmROCUnpackEpoch::~CbmROCUnpackEpoch()
{
}


Bool_t CbmROCUnpackEpoch::Init()
{
  FairRunOnline* online = FairRunOnline::Instance();
  fSource = static_cast<CbmSourceLmdNew*>(online->GetSource());
  return kTRUE;
}

Bool_t CbmROCUnpackEpoch::DoUnpack(roc::Message* Message, ULong_t /*hitTime*/)
{

  Int_t rocId          = Message->getRocNumber();
  fSource->SetEpoch(rocId, Message->getEpochNumber());

  if ( rocId ) { return kTRUE; }  // Further action only for ROC Id 0

  ULong_t currentEpochTime = Message->getMsgFullTime(Message->getEpochNumber());
  fNofEpochs++;
  LOG(debug) << GetName() << ": Epoch message "
             << fNofEpochs << ", epoch " << Int_t(Message->getEpochNumber())
             << ", time " << std::setprecision(9) << std::fixed
             << Double_t(currentEpochTime) * 1.e-9 << " s";

  // --- Start and stop time
  if ( fSource->GetStartTime() == 0  || 
       fSource->GetStartTime() > currentEpochTime ) {
    fSource->SetStartTime(currentEpochTime);
  }
  if ( fSource->GetStopTime() < currentEpochTime ) { 
    fSource->SetStopTime(currentEpochTime); 
  }

  return kTRUE;
}

void CbmROCUnpackEpoch::Reset()
{
}

ClassImp(CbmROCUnpackEpoch)
