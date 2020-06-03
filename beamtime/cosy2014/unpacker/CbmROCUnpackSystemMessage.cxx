// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                       CbmROCUnpackSystemMessage                   -----
// -----                    Created 07.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmROCUnpackSystemMessage.h"

#include <FairLogger.h>         // for Logger, LOG
#include <stddef.h>             // for NULL
#include <iomanip>              // for setprecision, __iom_t5
#include "CbmAuxDigi.h"         // for CbmAuxDigi
#include "CbmSourceLmdNew.h"    // for CbmSourceLmdNew
#include "CbmTbDaqBuffer.h"     // for CbmTbDaqBuffer
#include <FairRunOnline.h>      // for FairRunOnline
#include "roc/Message.h"        // for Message, SYSMSG_USER, SYSMSG_USER_CAL...

using std::setprecision;

CbmROCUnpackSystemMessage::CbmROCUnpackSystemMessage()
  : CbmROCUnpack(),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fSource(NULL)
{
}

CbmROCUnpackSystemMessage::~CbmROCUnpackSystemMessage()
{
}


Bool_t CbmROCUnpackSystemMessage::Init()
{
  FairRunOnline* online = FairRunOnline::Instance();
  fSource = static_cast<CbmSourceLmdNew*>(online->GetSource());
  return kTRUE;
}


Bool_t CbmROCUnpackSystemMessage::DoUnpack(roc::Message* message, ULong_t hitTime)
{

  if (message->getSysMesType() == roc::SYSMSG_USER) {
    // --- Get absolute time and Roc number
    CbmAuxDigi* digi;
    Int_t rocId = message->getRocNumber();

    switch(message->getSysMesData()) {
    case roc::SYSMSG_USER_CALIBR_ON:
      {
	if ( !fSource->IsBaselineFill() ) {
	  // --- Create AuxDigi and send it to the buffer
	  digi = new CbmAuxDigi(666, 666, hitTime);
	  fBuffer->InsertData<CbmAuxDigi>(digi);
	  LOG(info) << " Switching now to baseline mode at " << 
	    setprecision(9) << Double_t(hitTime) * 1.e-9 << " s";
	  fSource->AddBaselineRoc(rocId);
	  LOG(info) << "ROC " << rocId << " now in baseline mode";
	  fSource->SetBaselineFill(kTRUE);
	}
	fSource->AddBaselineRoc(rocId);
	LOG(info) << "ROC " << rocId << " now in baseline mode";
	break;
      }
    case roc::SYSMSG_USER_CALIBR_OFF:
      {
	if ( fSource->IsBaselineFill() ) {
	  Bool_t isEmpty=fSource->RemoveBaselineRoc(rocId);
	  LOG(info) << "ROC " << rocId << " now in normal mode";
	  if ( isEmpty ) {
	    digi = new CbmAuxDigi(999, 999, hitTime);
	    fBuffer->InsertData<CbmAuxDigi>(digi);
	    fSource->SetBaselineFill(kFALSE);
	    LOG(info) << "Switching back to normal mode at " << 
	      setprecision(9) << Double_t(hitTime) * 1.e-9 << " s";
	  }     
	} 
	break;
      }
    case roc::SYSMSG_USER_RECONFIGURE:
      {
	LOG(debug) << "Found USER_RECONFIGURE at ";
	break;
      }
    default:
      {
	LOG(error) << "Found unknown system message at ";
	break;
      }
    }
  }
  
  return kTRUE;
}


void CbmROCUnpackSystemMessage::Reset()
{
}


ClassImp(CbmROCUnpackSystemMessage)
