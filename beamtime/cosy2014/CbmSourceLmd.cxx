// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                             CbmSourceLmd                          -----
// -----                    Created 13.12.2013 by V. Friese                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmSourceLmd.h"
#include "CbmAuxDigi.h"
#include "CbmStsDigi.h"
#include "CbmStsAddress.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmDaqMap.h"
#include "CbmTbDaqBuffer.h"
#include "CbmTbEvent.h"
#include "CbmModuleList.h"

#include "much/CbmMuchAddress.h"
#include "fhodo/CbmFiberHodoAddress.h"
#include "fhodo/CbmFiberHodoDigi.h"

#include "FairLogger.h"
#include "FairRunOnline.h"
#include "FairEventHeader.h"

#include "TClonesArray.h"

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::map;
using std::setprecision;
using std::setw;
using std::fixed;
using std::right;

CbmSourceLmd::CbmSourceLmd()
  : FairSource(),
    fInputFileName(""),
    fInputFileList(),
    fFileCounter(0),
    fReadInTimeStep(1e9),
    fPersistence(kTRUE),
    fEventTimeWindow(500.),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fDaqMap(NULL),
    fRocIter(NULL),
    fTimeStart(0),
    fTimeStop(0),
    fCurrentEpochTime(0),
    fStartEpoch(0),
    fCurrentEpoch(),
    fCurrentBufferTime(0),
    fCurrentMessage(NULL),
    fTimeBufferFill(0),
    fTimeBufferOut(0.),
    fCurrentEvent(NULL),
    fCurrentDigi(),
    fNofMessages(0),
    fNofEpochs(0),
    fNofEvents(0),
    fNofMessType(),
    fNofMessRoc(),
    fNofHitMsg(),
    fNofDigis(),
    fNofAux(0),
    fHodoDigis(nullptr),
    fHodoBaselineDigis(nullptr),
    fStsDigis(nullptr),
    fStsBaselineDigis(nullptr),
    fMuchDigis(nullptr),
    fMuchBaselineDigis(nullptr),
    fAuxDigis(nullptr),
    fBaselineData(kFALSE),
    fBaselineRoc(),
    fTriggeredMode(kFALSE),
    fRocNumber1(6),
    fRocNumber2(7)
{
  // --- Initialise counters
  for (Int_t iType = 0; iType < 8; iType++) { fNofMessType[iType] = 0; }
  for (Int_t iRoc = 0; iRoc < 20; iRoc++)
    for (Int_t iNx = 0; iNx < 5; iNx++ ) {
      fNofMessRoc[iRoc][iNx] = 0;
    }
}

CbmSourceLmd::CbmSourceLmd(const char* inFile)
  : FairSource(),
    fInputFileName(inFile),
    fInputFileList(new TObjString(inFile)),
    fFileCounter(0),
    fReadInTimeStep(1e9),
    fPersistence(kTRUE),
    fEventTimeWindow(500.),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fDaqMap(NULL),
    fRocIter(NULL),
    fTimeStart(0),
    fTimeStop(0),
    fCurrentEpochTime(0),
    fStartEpoch(0),
    fCurrentEpoch(),
    fCurrentBufferTime(0),
    fCurrentMessage(NULL),
    fTimeBufferFill(0),
    fTimeBufferOut(0.),
    fCurrentEvent(NULL),
    fCurrentDigi(),
    fNofMessages(0),
    fNofEpochs(0),
    fNofEvents(0),
    fNofMessType(),
    fNofMessRoc(),
    fNofHitMsg(),
    fNofDigis(),
    fNofAux(0),
    fHodoDigis(nullptr),
    fHodoBaselineDigis(nullptr),
    fStsDigis(nullptr),
    fStsBaselineDigis(nullptr),
    fMuchDigis(nullptr),
    fMuchBaselineDigis(nullptr),
    fAuxDigis(nullptr),
    fBaselineData(kFALSE),
    fBaselineRoc(),
    fTriggeredMode(kFALSE),
    fRocNumber1(6),
    fRocNumber2(7)
{
  // --- Initialise counters
  for (Int_t iType = 0; iType < 8; iType++) { fNofMessType[iType] = 0; }
  for (Int_t iRoc = 0; iRoc < 20; iRoc++)
    for (Int_t iNx = 0; iNx < 5; iNx++ ) {
      fNofMessRoc[iRoc][iNx] = 0;
    }
}

/*
 * PAL: Comment copy constructor as does not seem to be used in any macro
CbmSourceLmd::CbmSourceLmd(const CbmSourceLmd& source)
  : FairSource(source)
{
}
*/

CbmSourceLmd::~CbmSourceLmd()
{
  //  delete fDaqMap;
}


// -----   Fill the buffer up to a given time   -----------------------------
Bool_t CbmSourceLmd::FillBuffer(ULong_t time)
{

  // --- Signal end of file if there are no more messages
  if ( ! fCurrentMessage ) {
    LOG(info) << GetName() << ": End of input file reached.";
    return kFALSE;
  }

  Int_t nMessages = 0;
  while ( fCurrentMessage ) {
    nMessages++;

    // --- Message type counters
    Int_t msgType = fCurrentMessage->getMessageType();
    if ( msgType < 0 || msgType > 7 ) {
      LOG(error) << GetName() << ": Skipping mesage with unknwon type "
                 << msgType;
      continue;
    }
    fNofMessType[msgType]++;

    // --- Treat system messages
    if ( fCurrentMessage->isSysMsg() ) { ProcessSystemMessage(); }

    // --- Treat epoch markers
    if ( fCurrentMessage->isEpochMsg() ) { ProcessEpochMarker(); }

    // --- Treat AUX messages
    if ( fCurrentMessage->isAuxMsg() ) {

      if (!fBaselineData) {
        // --- ROC Id and channel number
        Int_t rocId = fCurrentMessage->getRocNumber();
        Int_t channel = fCurrentMessage->getAuxChNum();
        fNofAux++;

        // --- Check for epoch marker for this ROC
        if ( fCurrentEpoch.find(rocId) == fCurrentEpoch.end() )
          LOG(fatal) << GetName()
                     << ": Hit message without previous epoch marker for ROC "
                     << rocId;

        // --- Get absolute time
        ULong_t auxTime = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);

        // --- Jump out of loop of hit time is after time limit
        if (auxTime > time) { break; }

        // --- Create AuxDigi and send it to the buffer
        CbmAuxDigi* digi = new CbmAuxDigi(rocId, channel, auxTime);
        fBuffer->InsertData<CbmAuxDigi>(digi);
      }

    } //? AUX message

    // --- Treat hit messages
    if ( fCurrentMessage->isHitMsg() ) {

      // --- ROC and NXYTER number; increase counter
      Int_t rocId = fCurrentMessage->getRocNumber();
      Int_t nxyId = fCurrentMessage->getNxNumber();
      fNofMessRoc[rocId][nxyId]++;

      // --- Check for epoch marker for this ROC
      if ( fCurrentEpoch.find(rocId) == fCurrentEpoch.end() )
        LOG(fatal) << GetName()
                   << ": Hit message without previous epoch marker for ROC "
                   << rocId;

      // --- Get absolute time
      ULong_t hitTime = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);

      // --- Jump out of loop of hit time is after time limit
      if (hitTime > time) { break; }

      // --- Process hit message (detector dependent)
      ECbmModuleId systemId = fDaqMap->GetSystemId(rocId);
      switch (systemId) {
      case ECbmModuleId::kSts:
        ProcessStsMessage();
        break;
      case ECbmModuleId::kMuch:
        ProcessMuchMessage();
        break;
      case ECbmModuleId::kHodo:
        ProcessHodoMessage();
        break;
      default:
        break;
      }

    }  //? Hit message

    // --- Get next ROC message
    if ( fRocIter->next() ) {
      fCurrentMessage = &fRocIter->msg();
    } else {
      // Check if there is another file in the list
      fFileCounter += 1;
      if ( fInputFileList.GetSize() > fFileCounter ) {
        TObjString* tmp =
          dynamic_cast<TObjString*>(fInputFileList.At(fFileCounter));
        fInputFileName = tmp->GetString();
        LOG(info) << GetName() << ": Opening next file "
                  << fInputFileName;
        // Normally one should delete the old object before, but this
        // doesn't work and result in a crash when creating a new iterator
        fRocIter = new roc::Iterator(fInputFileName.Data());
        if ( ! fRocIter->next() ) {
          LOG(error) << GetName() << "::Init: input file does not exist "
                     << "or is empty!";
          return kFALSE;
        }
        fCurrentMessage = &fRocIter->msg();
      } else {
        fCurrentMessage = NULL;
      }
    }
  } //- message loop

  // --- Update buffer fill time
  fTimeBufferFill  = time;

  // --- Set buffer retrieval time to one time step before fill time.
  fTimeBufferOut  = Double_t(time - fReadInTimeStep);

  // --- End of input: retrieval time equals fill time
  if ( ! fCurrentMessage ) {
    LOG(info) << GetName() << ": End of input reached.";
    fTimeBufferOut = Double_t(time);
  }

  // --- Status info
  LOG(info) << GetName() << ": Buffer fill time " << fixed
            << setprecision(3) << Double_t(fTimeBufferFill) * 1.e-9
            << " s, retrieval up to " << fTimeBufferOut * 1.e-9
            << " s, " << nMessages << " messages processed";
  fBuffer->PrintStatus();
  fNofMessages += nMessages;

  return kTRUE;
}
// --------------------------------------------------------------------------



// -----   Get next data   --------------------------------------------------
CbmTbDaqBuffer::Data CbmSourceLmd::GetNextData()
{

  // --- Retrieve next data from the buffer.
  CbmTbDaqBuffer::Data digi = fBuffer->GetNextData(fTimeBufferOut);

  // --- If no data: fill the buffer with next time step
  // --- N.b.: fCurrentMessage = NULL means no more data in input and
  // ---       retrieval of buffer up to the fill time.
  while ( ( digi.first.empty() )  &&  fCurrentMessage ) {
    FillBuffer(fTimeBufferFill + fReadInTimeStep);
    digi = fBuffer->GetNextData(fTimeBufferOut);
  }

  // --- If the digi pointer is NULL; the input is exhausted and the buffer
  // --- is empty.
  return digi;
}
// --------------------------------------------------------------------------



// -----   Initialisation   -------------------------------------------------
Bool_t CbmSourceLmd::Init()
{

  cout << endl;
  LOG(info) << "=====================================================";
  LOG(info) << GetName() << ": Initialising ...";

  if ( NULL == fDaqMap ) {
    LOG(fatal) << "No DaqMap defined.";
  }

  // --- Register output branches
  FairRootManager* ioman = FairRootManager::Instance();
  assert ( ioman );

  // --- Branch for digis
  fHodoDigis = new std::vector<CbmFiberHodoDigi>;
  ioman->RegisterAny("HodoDigi", fHodoDigis, fPersistence);

  fHodoBaselineDigis = new std::vector<CbmFiberHodoDigi>;
  ioman->RegisterAny("HodoBaselineDigi", fHodoBaselineDigis, fPersistence);

  fStsDigis = new std::vector<CbmStsDigi>;
  ioman->RegisterAny("StsDigi", fStsDigis, fPersistence);

  fStsBaselineDigis = new std::vector<CbmStsDigi>;
  ioman->RegisterAny("StsBaselineDigi", fStsBaselineDigis, fPersistence);

  fMuchDigis = new std::vector<CbmMuchBeamTimeDigi>;
  ioman->RegisterAny("MuchDigi", fMuchDigis, fPersistence);

  fMuchBaselineDigis = new std::vector<CbmMuchBeamTimeDigi>;
  ioman->RegisterAny("MuchBaselineDigi", fMuchBaselineDigis, fPersistence);

  fAuxDigis = new std::vector<CbmAuxDigi>;
  ioman->RegisterAny("AuxDigi", fAuxDigis, fPersistence);

  // --- Get event header from Run
  fCurrentEvent = dynamic_cast<CbmTbEvent*> (FairRunOnline::Instance()->GetEventHeader());
  if ( ! fCurrentEvent ) {
    LOG(fatal) << "No event header in run!";
    return kFALSE;
  }
  LOG(info) << "Init : event header at " << fCurrentEvent;
  ioman->Register("EventHeader.", "Event", fCurrentEvent, kTRUE);

  // --- Open input file and get first message
  TObjString* tmp =
    dynamic_cast<TObjString*>(fInputFileList.At(fFileCounter));
  fInputFileName = tmp->GetString();
  LOG(info) << GetName() << ": Opening file " << fInputFileName;
  fRocIter = new roc::Iterator(fInputFileName.Data());
  if ( ! fRocIter->next() ) {
    LOG(error) << GetName() << "::Init: input file does not exist "
               << "or is empty!";
    return kFALSE;
  }
  fCurrentMessage = &fRocIter->msg();
  if ( fCurrentMessage->getMessageType() != roc::MSG_EPOCH ) {
    LOG(error) << GetName() << ": First message is not epoch marker!";
    return kFALSE;
  }

  // --- Get start time and set initial buffer fill time
  Int_t epoch  = fCurrentMessage->getEpochNumber();
  ULong_t time = fCurrentMessage->getMsgFullTime(epoch);
  LOG(info) << GetName() << ": First message: epoch " << epoch << "  time "
            << setprecision(9) << Double_t(time) * 1.e-9 << " s";

  // --- Set initial buffer fill time (rounded to next read-in time step)
  fTimeBufferFill = ULong_t( Double_t(time) / Double_t(fReadInTimeStep) )
                    * fReadInTimeStep;
  fTimeBufferOut  = fTimeBufferFill - fReadInTimeStep;

  // --- Read the first digi from the buffer
  fCurrentDigi = GetNextData();
  if ( fCurrentDigi.first.empty() ) {
    LOG(error) << GetName() << ": No hit data in input!";
    return kFALSE;
  }

  LOG(info) << GetName() << ": Initialisation done. ";
  LOG(info) << "=====================================================";
  cout << endl;

  return kTRUE;
}
// --------------------------------------------------------------------------



// -----   Event loop   -----------------------------------------------------
// Action: Digis are retrieved from the buffer one by one.
// If their time is within the association window, they are added to the
// event. If not, the event loop is stopped.
Int_t CbmSourceLmd::ReadEvent(UInt_t)
{

  if ( ! fCurrentEvent ) {
    LOG(fatal) << "No pointer to event header! " << fCurrentEvent;
  }

  // The next block is needed do to the problem that the TClonesArray is
  // very large after usage (2.5 M entries). This somehow slows down
  // the Clear of the container even if no entries are inside by 2-3 orders
  // of magnitude which slows down the execution of the whole program by 
  // more then a factor of 10.
  // TODO:
  // It has to be checked if this is a bug in Root by producing a small
  // example program to demonstarete the issue
  if (fStsBaselineDigis->size() > 1000) {
    fStsBaselineDigis->clear();
    fStsBaselineDigis->resize(10);
  } 
  if (fHodoBaselineDigis->size() > 1000) {
    fHodoBaselineDigis->clear();
    fHodoBaselineDigis->resize(10);
  } 
  if (fMuchBaselineDigis->size() > 1000) {
    fMuchBaselineDigis->clear();
    fMuchBaselineDigis->resize(10);
  } 

/*
  // --- Clear output arrays
  fHodoDigis->clear();
  fHodoBaselineDigis->clear();
  fStsDigis->clear();
  fStsBaselineDigis->clear();
  fMuchDigis->clear();
  fMuchBaselineDigis->clear();
  fAuxDigis->clear();
*/

/*
  // --- Clear output arrays
  fHodoDigis->Delete();
  fHodoBaselineDigis->Delete();
  fStsDigis->Delete();
  fStsBaselineDigis->Delete();
  fMuchDigis->Delete();
  fMuchBaselineDigis->Delete();
  fAuxDigis->Delete();
*/
  // --- Clrear event header
  fCurrentEvent->Clear();


  // Loop over digis
  while ( kTRUE ) {

    ECbmModuleId systemId = fCurrentDigi.second;
    Double_t _time{0.};
    if ( systemId == ECbmModuleId::kSts ) {
        _time = boost::any_cast<CbmStsDigi*>(fCurrentDigi.first)->GetTime();
    } else if ( systemId == ECbmModuleId::kMuch ) {
        _time = boost::any_cast<CbmMuchBeamTimeDigi*>(fCurrentDigi.first)->GetTime();
    } else if ( systemId == ECbmModuleId::kHodo ) {
        _time = boost::any_cast<CbmFiberHodoDigi*>(fCurrentDigi.first)->GetTime();
    } else if ( systemId == static_cast<ECbmModuleId>(999) ) {
        _time = boost::any_cast<CbmAuxDigi*>(fCurrentDigi.first)->GetTime();
    }  

    // --- If event is empty or time of digi matches to event, add digi to event.
    Double_t tDif = TMath::Abs(_time - fCurrentEvent->GetTime() );
    if ( fCurrentEvent->IsEmpty() || tDif < fEventTimeWindow ) {

      // --- Update event object
//      fCurrentEvent->AddDigi(fCurrentDigi);

      // --- Copy digi to output array
      if ( systemId == ECbmModuleId::kSts ) {
        CbmStsDigi*  _tmpdigi = boost::any_cast<CbmStsDigi*>(fCurrentDigi.first);
//        LOG(info) << _tmpdigi->ToString();
        fCurrentEvent->AddDigi<CbmStsDigi>(_tmpdigi);
        fStsDigis->emplace_back(*(_tmpdigi));
        delete _tmpdigi;
        fNofDigis[ToIntegralType(ECbmModuleId::kSts)]++;
        LOG(debug) << "STS digis " << fNofDigis[ToIntegralType(ECbmModuleId::kSts)];
      } //? STS digi
      else if ( systemId == ECbmModuleId::kMuch ) {
        CbmMuchBeamTimeDigi*  _tmpdigi = boost::any_cast<CbmMuchBeamTimeDigi*>(fCurrentDigi.first);
        fCurrentEvent->AddDigi<CbmMuchBeamTimeDigi>(_tmpdigi);
        fMuchDigis->emplace_back(*(_tmpdigi));
        fNofDigis[ToIntegralType(ECbmModuleId::kMuch)]++;
        LOG(debug) << "MUCH digis " << fNofDigis[ToIntegralType(ECbmModuleId::kMuch)];
      } //? MUCH digi
      else if ( systemId == ECbmModuleId::kHodo ) {
        CbmFiberHodoDigi*  _tmpdigi = boost::any_cast<CbmFiberHodoDigi*>(fCurrentDigi.first);
        fCurrentEvent->AddDigi<CbmFiberHodoDigi>(_tmpdigi);
        fHodoDigis->emplace_back(*(_tmpdigi));
        delete _tmpdigi;
        fNofDigis[ToIntegralType(ECbmModuleId::kHodo)]++;
        LOG(debug) << "HODO digis " << fNofDigis[ToIntegralType(ECbmModuleId::kHodo)];
      } //? HODO digi
      else if ( systemId == static_cast<ECbmModuleId>(999)) { // I know I should not hardcode numbers....
        CbmAuxDigi* _tmpdigi = boost::any_cast<CbmAuxDigi*>(fCurrentDigi.first);
	Int_t val = _tmpdigi->GetRocId();
	if ( val == 666 ) {
	  LOG(info) << "In Old Aux RocId: "<< val;
	  Int_t retval = FillBaselineDataContainer();
	  fNofEvents++;
	  return retval;
	} else {
          fCurrentEvent->AddDigi<CbmAuxDigi>(_tmpdigi);
          fAuxDigis->emplace_back(*(_tmpdigi));
          delete _tmpdigi;
	}
      }

      // --- Delete current digi from memory and get next one
      fCurrentDigi = GetNextData();
      if ( fCurrentDigi.first.empty() ) {
        LOG(info) << "No more input data. End the run.";
        return 1;  // no more data; trigger end of run
      }
    }   //? Digi belongs to event?

    // --- If digi does not belong to current event: stop event loop.
    else {
      if ( FairLogger::GetLogger()->IsLogNeeded(fair::Severity::debug1) ) fCurrentEvent->Print();
      fNofEvents++;
      return 0;
    }

  } //- event loop over digis

  LOG(info) << "===== Leaving Exec. ";

  return 0;
}

// -----   Process a system message   ----------------------------------------
void CbmSourceLmd::ProcessSystemMessage()
{
  if (fCurrentMessage->getSysMesType() == roc::SYSMSG_USER) {
    // --- Get absolute time and Roc number
    Int_t rocId = fCurrentMessage->getRocNumber();
    ULong_t hitTime = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
    CbmAuxDigi* digi;

    switch(fCurrentMessage->getSysMesData()) {
    case roc::SYSMSG_USER_CALIBR_ON:
      if ( !fBaselineData ) {
        fBaselineData = kTRUE;
        // --- Create AuxDigi and send it to the buffer
        digi = new CbmAuxDigi(666, 666, hitTime);
        fBuffer->InsertData<CbmAuxDigi>(digi);
	LOG(info) << " Switching now to baseline mode at " << 
	  setprecision(9) << Double_t(hitTime) * 1.e-9 << " s";
      }
      if (fTriggeredMode) {
        fBaselineRoc.insert(fRocNumber1);
        fBaselineRoc.insert(fRocNumber2);
        LOG(info) << "Triggered mode";
        LOG(info) << "ROC "  << fRocNumber1 << " now in baseline mode";
        LOG(info) << "ROC "  << fRocNumber2 << " now in baseline mode";
      }
      fBaselineRoc.insert(rocId);
      LOG(info) << "ROC " << rocId << " now in baseline mode";
      break;
    case roc::SYSMSG_USER_CALIBR_OFF:
      if (fTriggeredMode) {
        fBaselineRoc.erase(fRocNumber1);
        fBaselineRoc.erase(fRocNumber2);
        LOG(info) << "Triggered mode";
        LOG(info) << "ROC "  << fRocNumber1 << " now in normal mode";
        LOG(info) << "ROC "  << fRocNumber2 << " now in normal mode";
      }
      fBaselineRoc.erase(rocId);
      LOG(info) << "ROC " << rocId << " now in normal mode";
      if ( fBaselineRoc.empty() ) {
        digi = new CbmAuxDigi(999, 999, hitTime);
        fBuffer->InsertData<CbmAuxDigi>(digi);
        fBaselineData = kFALSE;
        LOG(info) << "Switching back to normal mode at " << 
	  setprecision(9) << Double_t(hitTime) * 1.e-9 << " s";
      }
      break;
    case roc::SYSMSG_USER_RECONFIGURE:
      LOG(debug) << "Found USER_RECONFIGURE at ";//;
      break;
    default:
      LOG(error) << "Found unknown system message at ";//;
      break;
    }
  }
}
// -----   Process an epoch marker   ----------------------------------------
void CbmSourceLmd::ProcessEpochMarker()
{

  Int_t rocId          = fCurrentMessage->getRocNumber();
  fCurrentEpoch[rocId] = fCurrentMessage->getEpochNumber();
  if ( rocId ) { return; }  // Further action only for ROC Id 0

  fCurrentEpochTime = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
  fNofEpochs++;
  LOG(debug) << GetName() << ": Epoch message "
             << fNofEpochs << ", epoch " << Int_t(fCurrentEpoch[rocId])
             << ", time " << std::setprecision(9) << std::fixed
             << Double_t(fCurrentEpochTime) * 1.e-9 << " s";

  // --- Start and stop time
  if ( fTimeStart == 0  || fTimeStart > fCurrentEpochTime ) {
    fTimeStart = fCurrentEpochTime;
  }
  if ( fTimeStop < fCurrentEpochTime ) { fTimeStop = fCurrentEpochTime; }

}
// --------------------------------------------------------------------------



// -----   Process a HODO hit message   -------------------------------------
void CbmSourceLmd::ProcessHodoMessage()
{

  // --- Increment message counter
  fNofHitMsg[ToIntegralType(ECbmModuleId::kHodo)]++;
  
  // --- Get absolute time, NXYTER and channel number
  Int_t rocId      = fCurrentMessage->getRocNumber();
  ULong_t hitTime  = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
  Int_t nxyterId   = fCurrentMessage->getNxNumber();
  Int_t nxChannel  = fCurrentMessage->getNxChNum();
  Int_t charge     = fCurrentMessage->getNxAdcValue();
  
  Int_t iStation;
  Int_t iSector;
  Int_t iPlane;
  Int_t iFiber;
  
  fDaqMap->Map(rocId, nxyterId, nxChannel, iStation, iSector, iPlane, iFiber);
  Int_t address = CbmFiberHodoAddress::GetAddress(iStation, iPlane, iFiber);
  
  // --- Create a HODO digi and send it to the buffer
  CbmFiberHodoDigi* digi = new CbmFiberHodoDigi(address, charge, hitTime);
  
  // In case of normal data insert the digi into the buffer.
  // In case of baseline data insert the digi only if the roc 
  // is already in baseline mode.  
  if (!fBaselineData) {
    fBuffer->InsertData<CbmFiberHodoDigi>(digi);
  } else {
    std::set<Int_t>::iterator it;
    it = fBaselineRoc.find(rocId);
    if (it != fBaselineRoc.end() ) { fBuffer->InsertData<CbmFiberHodoDigi>(digi); }
  }
  
}
// --------------------------------------------------------------------------



// -----   Process a MUCH hit message   -------------------------------------
void CbmSourceLmd::ProcessMuchMessage()
{

  // --- Increment message counter
  fNofHitMsg[ToIntegralType(ECbmModuleId::kMuch)]++;
  
  // --- Get absolute time, NXYTER and channel number
  Int_t rocId      = fCurrentMessage->getRocNumber();
  ULong_t hitTime  = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
  Int_t nxyterId   = fCurrentMessage->getNxNumber();
  Int_t nxChannel  = fCurrentMessage->getNxChNum();
  Int_t charge     = fCurrentMessage->getNxAdcValue();
  
  LOG(debug2) << "MUCH mssage at " << hitTime;
  
  // --- Get detector element from DaqMap
  Int_t station = fDaqMap->GetMuchStation(rocId);
  Int_t layer   = 0;
  Int_t side    = 0;
  Int_t module  = 0;
  Int_t sector  = nxyterId;
  Int_t channel = nxChannel;

  // --- Construct unique address
  UInt_t address = CbmMuchAddress::GetAddress(station, layer, side,
					      module, sector, channel);
  
  // --- Create digi
  CbmMuchBeamTimeDigi* digi = new CbmMuchBeamTimeDigi(address, charge, hitTime);
  LOG(debug2) << "MUCH digi at " << digi->GetTime();
  
  // In case of normal data insert the digi into the buffer.
  // In case of baseline data insert the digi only if the roc 
  // is already in baseline mode.  
  if (!fBaselineData) {
    fBuffer->InsertData<CbmMuchBeamTimeDigi>(digi);
  } else {
    std::set<Int_t>::iterator it;
    it = fBaselineRoc.find(rocId);
    if (it != fBaselineRoc.end() ) {fBuffer->InsertData<CbmMuchBeamTimeDigi>(digi); }
  }

}
// --------------------------------------------------------------------------



// -----   Process a STS hit message   --------------------------------------
void CbmSourceLmd::ProcessStsMessage()
{

  // --- Increment message counter
  fNofHitMsg[ToIntegralType(ECbmModuleId::kSts)]++;

  // --- Get absolute time, NXYTER and channel number
  Int_t rocId        = fCurrentMessage->getRocNumber();
  ULong64_t hitTime  = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
  Int_t nxyterId     = fCurrentMessage->getNxNumber();
  Int_t nxChannel    = fCurrentMessage->getNxChNum();
  Int_t charge       = fCurrentMessage->getNxAdcValue();

  // --- Get detector element from DaqMap
  Int_t station = fDaqMap->GetStsStation(rocId);
/*  Int_t sector  = 0;*/
  Int_t side    = fDaqMap->GetStsSensorSide(rocId);
  Int_t channel = fDaqMap->GetStsChannel(rocId, nxyterId, nxChannel);

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
  if (!fBaselineData) {
    fBuffer->InsertData<CbmStsDigi>(digi);
  } else {
    std::set<Int_t>::iterator it;
    it = fBaselineRoc.find(rocId);
    if (it != fBaselineRoc.end() ) {fBuffer->InsertData<CbmStsDigi>(digi); }
  }

}
// --------------------------------------------------------------------------


void CbmSourceLmd::Close()
{

  cout << endl;
  LOG(info) << "=====================================================";
  fBuffer->PrintStatus();
  LOG(info) << GetName() << ": Run Summary";

  cout << endl;
  LOG(info) << "Messages in input: " << fNofMessages;
  Double_t deltaT = (fTimeStop - fTimeStart) / 1.e9;
  LOG(info) << "Start time : " << fTimeStart * 1.e-9 << " s  ";
  LOG(info) << "Stop  time : " << fTimeStop  * 1.e-9 << " s  ";
  LOG(info) << "Duration   : " << deltaT  << " s";

  const char* names[8] = { "NOP", "HIT", "EPOCH", "SYNC", "AUX", "EPOCH2", "GET4", "SYS" };
  cout << endl;
  LOG(info) << "Messages per message type: ";
  for (Int_t i=0; i<8; i++)
    LOG(info) << setw(6) << names[i] << "  "
              << setw(10) << right << fNofMessType[i];

  cout << endl;
  LOG(info) << "Hit messages per ROC/NXYTER: ";
  for (Int_t iRoc = 0; iRoc < 13; iRoc ++)
    for (Int_t iNx = 0; iNx < 3; iNx +=2 ) {
      LOG(info) << " ROC " << setw(2) << iRoc << "  NXYTER " << iNx << "  Messages "
                << setw(12) << right << fNofMessRoc[iRoc][iNx] << "  Rate "
                << setw(12) << setprecision(4) << fixed  << right
                << Double_t(fNofMessRoc[iRoc][iNx]) / deltaT
                << " /s ";
    }

  cout << endl;
  LOG(info) << "Hit messages and digis per system: ";
  for (ECbmModuleId iSys = ECbmModuleId::kRef; iSys < ECbmModuleId::kNofSystems; ++iSys) {
    TString sysName = CbmModuleList::GetModuleNameCaps(iSys);
    LOG(info) << setw(5) << sysName << ": Messages " << fNofHitMsg[ToIntegralType(iSys)]
              << ", Digis " << fNofDigis[ToIntegralType(iSys)];
  }
  LOG(info) << "AUX  : Messages " << fNofMessType[roc::MSG_AUX] << ", Digis "
            << fNofAux;


  LOG(info) << "Total number of events: " << fNofEvents;

  delete fRocIter;
}

void CbmSourceLmd::Reset()
{
}

Int_t CbmSourceLmd::FillBaselineDataContainer()
{

  // --- Clear output arrays
  fHodoDigis->clear();
  fHodoBaselineDigis->clear();
  fStsDigis->clear();
  fStsBaselineDigis->clear();
  fMuchDigis->clear();
  fMuchBaselineDigis->clear();
  fAuxDigis->clear();
  
  // --- Clrear event header
  fCurrentEvent->Clear();
  
  LOG(info) << "Event type is now: " << fCurrentEvent->GetEventType();
  fCurrentEvent->SetEventType(1);
  LOG(info) << "Event type is now: " << fCurrentEvent->GetEventType();
  
  Int_t _nofevents = fNofDigis[ToIntegralType(ECbmModuleId::kDummyDet)];
  // Loop over digis
  while ( kTRUE ) {
    
    ECbmModuleId systemId = fCurrentDigi.second;
    
    // --- Copy digi to output array
    if ( systemId == ECbmModuleId::kSts ) {
      CbmStsDigi*  _tmpdigi = boost::any_cast<CbmStsDigi*>(fCurrentDigi.first);
      fStsBaselineDigis->emplace_back(*(_tmpdigi));
      delete _tmpdigi;
      fNofDigis[ToIntegralType(ECbmModuleId::kDummyDet)]++;
    } else if ( systemId == ECbmModuleId::kMuch ) {
      CbmMuchBeamTimeDigi*  _tmpdigi = boost::any_cast<CbmMuchBeamTimeDigi*>(fCurrentDigi.first);
      fMuchBaselineDigis->emplace_back(*(_tmpdigi));
      fNofDigis[ToIntegralType(ECbmModuleId::kDummyDet)]++;
    } else if ( systemId == ECbmModuleId::kHodo ) {
      CbmFiberHodoDigi*  _tmpdigi = boost::any_cast<CbmFiberHodoDigi*>(fCurrentDigi.first);
      fHodoBaselineDigis->emplace_back(*(_tmpdigi));
      delete _tmpdigi;
      fNofDigis[ToIntegralType(ECbmModuleId::kDummyDet)]++;
    } else if ( systemId == static_cast<ECbmModuleId>(999)) { // I know I should not hardcode numbers....
      // check if this is a misused auxmessage to set/unset basline
      // calibration
      CbmAuxDigi* _tmpdigi = boost::any_cast<CbmAuxDigi*>(fCurrentDigi.first);
      Int_t val = _tmpdigi->GetRocId();
      if ( val  == 999 ) {
        LOG(info) << "Aux RocId: "<< val;
        LOG(info) << "Leaving FillBaselineDataContainer after " 
                  << (fNofDigis[ToIntegralType(ECbmModuleId::kDummyDet)] - _nofevents) << " events";
	return 0;
      }
    } else {
          //  LOG(error) << "Between baseline start and end marker there should be only sts data";
    }
//    } else {
         //     LOG(error) << "Between baseline start and end marker there should be only sts data";

    fCurrentDigi = GetNextData();
    if (  fCurrentDigi.first.empty() ) {
      LOG(info) << "No more input data. End the run.";
      return 1;  // no more data; trigger end of run
    }
  }
}

ClassImp(CbmSourceLmd)
