// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                             CbmROCUnpackAux                       -----
// -----                    Created 14.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmROCUnpackAux.h"

#include <FairRootManager.h>    // for FairRootManager
#include <stddef.h>             // for NULL
#include "CbmAuxDigi.h"         // for CbmAuxDigi
#include "CbmSourceLmdNew.h"    // for CbmSourceLmdNew
#include "CbmTbDaqBuffer.h"     // for CbmTbDaqBuffer
#include <FairRunOnline.h>      // for FairRunOnline
#include <TClonesArray.h>       // for TClonesArray
#include "roc/Message.h"        // for Message
#include <boost/any.hpp>

using boost::any_cast;

CbmROCUnpackAux::CbmROCUnpackAux()
  : CbmROCUnpack(),
    fSource(nullptr),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fAuxDigis(nullptr)
{
}
  
CbmROCUnpackAux::~CbmROCUnpackAux()
{
}


Bool_t CbmROCUnpackAux::Init()
{
  FairRunOnline* online = FairRunOnline::Instance();
  fSource = static_cast<CbmSourceLmdNew*>(online->GetSource());

//  FairRootManager* ioman = FairRootManager::Instance();

  fAuxDigis = RegisterOutput<CbmAuxDigi>("AuxDigi");
//  ioman->Register("AuxDigi", "AUX data", fAuxDigis, fPersistence);

  return kTRUE;

}

Bool_t CbmROCUnpackAux::DoUnpack(roc::Message* Message, ULong_t hitTime)
{
  if (!fSource->IsBaselineFill()) {
    // --- ROC Id and channel number
    Int_t rocId = Message->getRocNumber();
    Int_t channel = Message->getAuxChNum();

    fSource->CheckCurrentEpoch(rocId);
    ULong_t offset = fSource->GetAuxOffset();
    
    // --- Create AuxDigi and send it to the buffer
    CbmAuxDigi* digi = new CbmAuxDigi(rocId, channel, hitTime + offset);
    fBuffer->InsertData<CbmAuxDigi>(digi);
  } else {
    fSource->AddDiscardedAux();
  }

  return kTRUE;
}

void CbmROCUnpackAux::FillOutput(boost::any _digi)
{
  CbmAuxDigi* digi = boost::any_cast<CbmAuxDigi*>(_digi);
/*
  new( (*fAuxDigis)[fAuxDigis->GetEntriesFast()])
    CbmAuxDigi(*(boost::any_cast<CbmAuxDigi*>(digi)));
*/
  fAuxDigis->emplace_back(*(digi));
  
  delete digi;
}


void CbmROCUnpackAux::Reset()
{
//  fAuxDigis->Clear();
  fAuxDigis->clear();
}

ClassImp(CbmROCUnpackAux)
