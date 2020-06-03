// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                             CbmRocSource                          -----
// -----                    Created 01.11.2013 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmRocSource.h"
#include "CbmDaqMap.h"
#include "much/CbmMuchAddress.h"
#include "fhodo/CbmFiberHodoAddress.h"
#include "CbmDetectorList.h"
#include "CbmHistManager.h"

#include "FairLogger.h"
#include "FairRunOnline.h"

#include "TH1D.h"
#include "TCanvas.h"

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::map;

CbmRocSource::CbmRocSource()
  : FairSource(),
    fInputFileName(""),
    fNofMessages(0),
    fRocIter(NULL),
    fCurrentMessage(NULL),
    fHM(NULL)
{
}

CbmRocSource::CbmRocSource(const char * inFile)
  : FairSource(),
    fInputFileName(inFile),
    fNofMessages(0),
    fRocIter(NULL),
    fCurrentMessage(NULL),
    fHM(NULL)
{
}


CbmRocSource::CbmRocSource(const CbmRocSource& source)
  : FairSource(source)
{
}


CbmRocSource::~CbmRocSource()
{
  delete fDaqMap;
}


Bool_t CbmRocSource::Init()
{

  cout << endl << endl;
  LOG(info) << "===== " << GetName() << ": Intialising ...";

  fDaqMap = new CbmDaqMap();

	for (Int_t iRoc = 7; iRoc < 13; iRoc++)
		LOG(info) << "ROC " << iRoc << " Station " << fDaqMap->GetStsStation(iRoc);


  // Check and open input file
  LOG(info) << "===== Opening file " << fInputFileName;
  fRocIter = new roc::Iterator(fInputFileName.Data());

  if ( ! fRocIter->next() ) {
    LOG(error) << GetName() << "::Init: input file does not exist "
	 << "or is empty!";
    return kFALSE;
  }
  fCurrentMessage = &fRocIter->msg();
  fNofMessages++;

  // Initialise counters
  for (Int_t iType = 0; iType < 8; iType++) fNofMessType[iType] = 0;
  for (Int_t iRoc = 0; iRoc < 20; iRoc++)
  	for (Int_t iNx = 0; iNx < 5; iNx++ )
  		fNofMessRoc[iRoc][iNx] = 0;
  fTimeStart = 0;
  fTimeStop  = 0;

  // Create histogram
  fHM = new CbmHistManager();
  string xy[2] = {"X","Y"};
  string name = "Hodo"; 
  string name1, name2;
  for (Int_t i=0; i<2; ++i) {
    for (Int_t j=0; j<2; ++j) {
      name1 = name + boost::lexical_cast<std::string>(i) + xy[j];
      name2 = name1 + ";Fiber number; Counts";
      fHM->Add(name1, new TH1D(name1.c_str(), name2.c_str(), 64, 0., 64. ));
    }
  }       
  fHM->Add("HitTime", new TH1D("HitTime", "Hit time", 4000, 8., 12. ));

  LOG(info) << "===== Initialisation done. ";

  return kTRUE;
}




Int_t CbmRocSource::ReadEvent()
{
  
  LOG(info) << "Read event " << fNofMessages;


  // No action if no message available
  if ( ! fCurrentMessage ) {
    LOG(error) << "No more messages in input file.";
    return 1;
  }
    
  const char* names[8] = { "NOP", "HIT", "EPOCH", "SYNC", "AUX", "EPOCH2", "GET4", "SYS" };
  Int_t nMessages = 0;


  // --- Loop over messages until event end
  while (fCurrentMessage) {

  	// --- ROC and NXYTER Id and message type
  	uint8_t type = fCurrentMessage->getMessageType();
  	Int_t rocId  = fCurrentMessage->getRocNumber();
  	//LOG(info) << "Message " << nMessages << "  " << type
  	//		<< "  " << rocId << "  ";


  	// ---- Do some statistics
  	nMessages++;
  	if ( type >= 0 && type < 8 ) fNofMessType[type]++;

    // --- Treat epoch markers
    if ( fCurrentMessage->isEpochMsg() ) ProcessEpochMarker();


    // --- Hit messages
    if ( fCurrentMessage->isHitMsg() ) {

    	// --- Increase counters
    	Int_t nxId  = fCurrentMessage->getNxNumber();
    	fNofMessRoc[rocId][nxId]++;

    	// --- A hit message should be preceded by an epoch marker
    	if ( fCurrentEpoch.find(rocId) == fCurrentEpoch.end() ) {
    		LOG(fatal) << GetName()
    				       << ": Hit message without previous epoch marker for ROC "
    				       << rocId;
    	}

    	// --- Get system identifier
    	Int_t systemId = fDaqMap->GetSystemId(rocId);

    	// --- Treat MUCH messages
    	switch (systemId) {
    		case kSTS:  ProcessStsMessage(); break;
    		case kMUCH: ProcessMuchMessage(); break;
    		case kFHODO: ProcessHodoMessage(); break;
    		default: break;
    	}


    }   // ? Hit message


    // Next ROC message
  	if ( fRocIter->next() ) fCurrentMessage = &fRocIter->msg();
  	else fCurrentMessage = NULL;

  }  // Message loop

  LOG(info) << "Messages read: " << nMessages;
  for (Int_t i=0; i<8; i++)
  	LOG(info) << setw(6) << names[i] << "  "
  	          << setw(10) << right << fNofMessType[i];


  Double_t deltaT = (fTimeStop - fTimeStart) / 1.e9;
  LOG(info) << "Start time : " << fTimeStart * 1.e-9 << " s  ";
  LOG(info) << "Stop  time : " << fTimeStop  * 1.e-9 << " s  ";
  LOG(info) << "Duration   : " << deltaT  << " s";

  for (Int_t iRoc = 0; iRoc < 13; iRoc ++)
  	for (Int_t iNx = 0; iNx < 3; iNx +=2 ) {
  		LOG(info) << " ROC " << setw(2) << iRoc << "  NXYTER " << iNx << "  Messages "
  			      << setw(12) << right << fNofMessRoc[iRoc][iNx] << "  Rate "
  			      << setw(12) << setprecision(4) << fixed  << right
  			      << Double_t(fNofMessRoc[iRoc][iNx]) / deltaT
  			      << " /s ";
  }

  TCanvas* c1 = new TCanvas("c1", "First canvas", 800, 600); 
  c1->Divide(2,3);
  c1->cd(1);
  fHM->H1("HitTime")->Draw();
  c1->cd(3);
  fHM->H1("Hodo0X")->Draw();
  c1->cd(4);
  fHM->H1("Hodo0Y")->Draw();
  c1->cd(5);
  fHM->H1("Hodo1X")->Draw();
  c1->cd(6);
  fHM->H1("Hodo1Y")->Draw();
  c1->Update();
  return 1;
}


// -----   Process an epoch marker   ----------------------------------------
void CbmRocSource::ProcessEpochMarker() {

  Int_t rocId          = fCurrentMessage->getRocNumber();
  fCurrentEpoch[rocId] = fCurrentMessage->getEpochNumber();
  if ( rocId ) return;  // Further action only for ROC Id 0

  fCurrentEpochTime = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
  fNofEpochs++;
  LOG(debug) << GetName() << ": Epoch message "
  		       << fNofEpochs << ", epoch " << Int_t(fCurrentEpoch[rocId])
  		       << ", time " << std::setprecision(9) << std::fixed
  		       << fCurrentEpochTime << " s";

  // --- Start and stop time
  if ( fTimeStart == 0  || fTimeStart > fCurrentEpochTime )
  		fTimeStart = fCurrentEpochTime;
  if ( fTimeStop < fCurrentEpochTime ) fTimeStop = fCurrentEpochTime;

}
// --------------------------------------------------------------------------



// -----   Process a HODO hit message   -------------------------------------
void CbmRocSource::ProcessHodoMessage() 
{
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

  if (iStation == 1) {
    if (iPlane == 1) {
      fHM->H1("Hodo1X")->Fill(iFiber);
    } else {
      fHM->H1("Hodo1Y")->Fill(iFiber);
    }
  } else {
    if (iPlane == 1) {
      fHM->H1("Hodo0X")->Fill(iFiber);
    } else {
      fHM->H1("Hodo0Y")->Fill(iFiber);
    }
  }


  LOG(debug) << "Roc, Nxyter, Channel: " << rocId << ", " << nxyterId 
	    << ", " << nxChannel;
  LOG(debug) << "Station, Plane, Fiber: " << iStation << ", " << iPlane 
	    << ", " << iFiber;

}
// --------------------------------------------------------------------------



// -----   Process a MUCH hit message   -------------------------------------
void CbmRocSource::ProcessMuchMessage() {

	// --- Get absolute time, NXYTER and channel number
	Int_t rocId      = fCurrentMessage->getRocNumber();
	ULong_t hitTime  = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
	Int_t nxyterId   = fCurrentMessage->getNxNumber();
	Int_t nxChannel  = fCurrentMessage->getNxChNum();
	Int_t charge     = fCurrentMessage->getNxAdcValue();

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

	// Fill time histogram for ROC 9 NX 0
	if ( rocId == 3 && nxyterId == 0 )  {
		Double_t tRel = Double_t(hitTime-fTimeStart) *1.e-9;
		LOG(debug2) << hitTime << "   " << fTimeStart << "  " << tRel;
		fHM->H1("HitTime")->Fill(tRel);
	}


}
// --------------------------------------------------------------------------



// -----   Process a STS hit message   --------------------------------------
void CbmRocSource::ProcessStsMessage() {

	// --- Get absolute time, NXYTER and channel number
	Int_t rocId      = fCurrentMessage->getRocNumber();
	ULong_t hitTime  = fCurrentMessage->getMsgFullTime(fCurrentEpoch[rocId]);
	Int_t nxyterId   = fCurrentMessage->getNxNumber();
	Int_t nxChannel  = fCurrentMessage->getNxChNum();
	Int_t charge     = fCurrentMessage->getNxAdcValue();

	/*
	// Fill time histogram for ROC 9 NX 0
	if ( rocId == 9 && nxyterId == 0 )  {
		Double_t tRel = Double_t(hitTime-fTimeStart) *1.e-9;
		LOG(debug2) << hitTime << "   " << fTimeStart << "  " << tRel;
		fhHitTime->Fill(tRel);
	}
	*/


}
// --------------------------------------------------------------------------


void CbmRocSource::Close()
{
  LOG(info) << GetName() << ": Closing.";
  delete fRocIter;
  fHM->WriteToFile();
}

void CbmRocSource::Reset()
{
}

ClassImp(CbmRocSource)
