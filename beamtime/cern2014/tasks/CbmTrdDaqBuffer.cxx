/** @file CbmDaqBuffer.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 13 December 2013
 **/


#include <iostream>
#include <iomanip>

#include "TString.h"

#include "FairLogger.h"

#include "CbmTrdDaqBuffer.h"

using std::pair;
using std::multimap;
using std::fixed;
using std::setprecision;

// -----   Initialisation of static variables   ------------------------------
CbmTrdDaqBuffer* CbmTrdDaqBuffer::fgInstance = NULL;
// ---------------------------------------------------------------------------



// -----   Constructor   -----------------------------------------------------
CbmTrdDaqBuffer::CbmTrdDaqBuffer() : fData() {
}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTrdDaqBuffer::~CbmTrdDaqBuffer() {
}
// ---------------------------------------------------------------------------




// -----   Access to next data   ---------------------------------------------
CbmSpadicRawMessage* CbmTrdDaqBuffer::GetNextData(Double_t time) {


  // --- Check for empty buffer
  if ( ! fData.size() ) return NULL;

  // --- Get data from buffer
  CbmSpadicRawMessage* rawMessage = NULL;
  multimap<Double_t, CbmSpadicRawMessage*>::iterator it = fData.begin();
  CbmSpadicRawMessage* test = it->second;
  if ( test->GetTime() < time ) {
    rawMessage = test;
    fData.erase(it);
  }
  return rawMessage;

}
// ---------------------------------------------------------------------------




// -----   Insert data into buffer   -----------------------------------------
void CbmTrdDaqBuffer::InsertData(CbmSpadicRawMessage* rawMessage) {

  if ( ! rawMessage ) LOG(fatal) << "DaqBuffer: invalid rawMessage pointer";

  pair<Double_t, CbmSpadicRawMessage*> value (rawMessage->GetFullTime(), rawMessage);
  fData.insert(value);

  LOG(debug2) << "TrdDaqBuffer: Inserting rawMessage, EquipmentID "
	      << rawMessage->GetEquipmentID()
	      << ", SourceAddress " << rawMessage->GetSourceAddress()
	      << ", ChannelID " << rawMessage->GetChannelID()
	      << ", time " << rawMessage->GetFullTime();

}
// ---------------------------------------------------------------------------



// -----   Instance   --------------------------------------------------------
CbmTrdDaqBuffer* CbmTrdDaqBuffer::Instance() {
  if ( ! fgInstance ) fgInstance = new CbmTrdDaqBuffer();
  return fgInstance;
}
// ---------------------------------------------------------------------------



// -----   Print status   ----------------------------------------------------
void CbmTrdDaqBuffer::PrintStatus() const {
  TString sysName;
  Int_t size = GetSize();
  LOG(info) << "DaqBuffer: Status ";
  if ( ! size ) {
    LOG(info) << "empty";
    return;
  }
  LOG(info) << GetSize() << " rawMessages from "
            << fixed << setprecision(9) << GetTimeFirst() * 1.e-9 << " s to "
            << GetTimeLast() *1.e-9 << " s";
}
// ---------------------------------------------------------------------------

