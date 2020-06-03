// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmTbEventBuilderFixedTimeWindow                -----
// -----                    Created 14.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmTbEventBuilderFixedTimeWindow.h"

#include <FairEventHeader.h>    // for FairEventHeader
#include <TMathBase.h>          // for Abs
#include <FairLogger.h>         // for LOG, Logger
#include "CbmTbEvent.h"         // for CbmTbEvent
#include <FairRootManager.h>    // for FairRootManager
#include <FairRunOnline.h>      // for FairRunOnline

CbmTbEventBuilderFixedTimeWindow::CbmTbEventBuilderFixedTimeWindow()
  : CbmTbEventBuilder(),
    fEventTimeWindow(500.)
{
}

CbmTbEventBuilderFixedTimeWindow::~CbmTbEventBuilderFixedTimeWindow()
{
}

Bool_t CbmTbEventBuilderFixedTimeWindow::Init()
{
  // --- Get event header from Run
  fCurrentEvent = 
    dynamic_cast<CbmTbEvent*> (FairRunOnline::Instance()->GetEventHeader());
  if ( ! fCurrentEvent ) {
    LOG(fatal) << "No event header in run!";
  }

  LOG(info) << "Init : event header at " << fCurrentEvent;
  FairRootManager* ioman = FairRootManager::Instance();
  ioman->Register("EventHeader.", "Event", fCurrentEvent, kTRUE);

  return kTRUE;
}

Bool_t CbmTbEventBuilderFixedTimeWindow::IsInEvent(boost::any /*digi*/)
{
/*
  if ( ! fCurrentEvent ) {
    LOG(fatal) << "No pointer to event header! " << fCurrentEvent;
  }
  
  // If event is empty or time of digi matches to event, 
  // add digi to event.
  Double_t tDif = TMath::Abs(digi->GetTime() - fCurrentEvent->GetTime() );

  if ( fCurrentEvent->IsEmpty() || tDif < fEventTimeWindow ) {

    // --- Update event object
    fCurrentEvent->AddDigi(digi);
    return kTRUE;
  }

*/
  return kFALSE;
}

void CbmTbEventBuilderFixedTimeWindow::Reset()
{
  // --- Clear event header
  fCurrentEvent->Clear();
}
ClassImp(CbmTbEventBuilderFixedTimeWindow)
