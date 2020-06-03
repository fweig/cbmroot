// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSUnpackMuch                            -----
// -----               Created 11.11.2016 by V. Singhal and A. Kumar                      -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmTSUnpackMuch.h"
#include "CbmMuchUnpackPar.h"
#include "CbmTbDaqBuffer.h"
#include "CbmMuchAddress.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"
#include "TString.h"

#include <iostream>
#include <stdint.h>
#include <iomanip>

class CbmMuchAddress;
using std::hex;
using std::dec;
using namespace std;

CbmTSUnpackMuch::CbmTSUnpackMuch()
  : CbmTSUnpack(),
    fuOverlapMsNb(0),
    fMsgCounter(11,0), // length of enum MessageTypes initialized with 0
    fNdpbIdIndexMapA(),
    fNdpbIdIndexMapB(),
    fMuchStationMapX(),
    fMuchStationMapY(),
    fCurrentEpoch(),
    fNofEpochs(0),
    fCurrentEpochTime(0.),
    fdStartTime( -1 ),
    fEquipmentId(0),
    fMuchRaw(new TClonesArray("CbmNxyterRawMessageForMUCH", 10)),
    fMuchDigi(nullptr),
    fRawMessage(NULL),
    fDigi(NULL),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fCreateRawMessage(kFALSE),
    fUnpackPar(NULL)
{
}

CbmTSUnpackMuch::~CbmTSUnpackMuch()
{
}

Bool_t CbmTSUnpackMuch::Init()
{
  LOG(info) << "Initializing flib nxyter unpacker for MUCH";
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }
  //Commented due to (Error in <TTree::Bronch>: TClonesArray with no class defined in branch: cbmout.MUCH raw data.MuchRawMessage)

  if (fCreateRawMessage) {
    ioman->Register("MuchRawMessage", "MUCH raw data", fMuchRaw, kTRUE);
  }
  fMuchDigi = RegisterOutput<CbmMuchBeamTimeDigi>("CbmMuchBeamTimeDigi");
//  ioman->Register("MuchDigi", "Sps Much digi", fMuchDigi, kTRUE);

  //Bellow line is for creating ASCII file
  //std::cout << "AFCK Nu" << "   " << "FEB Nu"
  //	    << "  " << "Channel"  << "   " << "ADC Value" <<"  " << "Absolute Time" << "  " << " Sector X " << "  Channel Y" << std::endl;

  //  CreateHistograms();
  return kTRUE;
}

void CbmTSUnpackMuch::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar = (CbmMuchUnpackPar*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMuchUnpackPar"));
}


Bool_t CbmTSUnpackMuch::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();
  return ReInitContainers();

}

Bool_t CbmTSUnpackMuch::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();

  Int_t NrOfnDpbsModA = fUnpackPar->GetNrOfnDpbsModA();

  LOG(info) << "Nr. of nDPBs Mod. A: " << NrOfnDpbsModA;

  Int_t NrOfnDpbsModB = fUnpackPar->GetNrOfnDpbsModB();

  LOG(info) << "Nr. of nDPBs Mod. B: " << NrOfnDpbsModB;

  fNdpbIdIndexMapA.clear();
  fNdpbIdIndexMapB.clear();

  for (Int_t i = 0; i< NrOfnDpbsModA; ++i)
    {
      fNdpbIdIndexMapA[fUnpackPar->GetNdpbIdA(i)] = i;
      LOG(info) << "nDPB Id of MUCH  " << i
		<< " : 0x" << std::hex << fUnpackPar->GetNdpbIdA(i)
		<< std::dec;
    } // for (Int_t i = 0; i< NrOfnDpbsModA; ++i)


  for (Int_t i = 0; i< NrOfnDpbsModB; ++i)
    {
      fNdpbIdIndexMapB[fUnpackPar->GetNdpbIdB(i)] = i;
      LOG(info) << "nDPB Id of MUCH  " << i
		<< " : 0x" << std::hex << fUnpackPar->GetNdpbIdB(i)
		<< std::dec;
    } // for (Int_t i = 0; i< NrOfnDpbsModA; ++i)
  Int_t NrOfFebs = fUnpackPar->GetNrOfFebs();

  LOG(info) << "Nr. of FEBs : " << NrOfFebs;

  Int_t NrOfChannels = fUnpackPar->GetNrOfChannels();
  LOG(info) << "Nr. of Channels : " << NrOfChannels;

  // Need to clear the 2 Arrays fMuchStationMapX and MapY
  // log all the DPBs number
  // Filling all the Mapping values in the 2 X and Y arrays
  //	Int_t nrOfChannels = fUnpackPar->GetNumberOfChannels();

  /*
    for (Int_t febId = 0 ; febId<NrOfFebs; febId++){// looping on all the FEB IDs
    Int_t febNrA = fUnpackPar->GetFebsIdsFromArrayA(febId);
    Int_t febNrB = fUnpackPar->GetFebsIdsFromArrayB(febId);
    for (Int_t channelId=0; channelId<NrOfChannels; channelId++){

    //Have to check GetPadX and GetPadY values.
    //			LOG(info) << "Value of GetPadX " << 	fUnpackPar->GetPadX(febId,channelId);
    fMuchStationMapXA[febId][channelId] = fUnpackPar->GetPadX(febNrA,channelId);
    //			LOG(info) << "Value of GetPadY " << 	fUnpackPar->GetPadY(febId,channelId);
    fMuchStationMapYA[febId][channelId] = fUnpackPar->GetPadY(febNrA,channelId);
    fMuchStationMapXB[febId][channelId] = fUnpackPar->GetPadX(febNrB,channelId);
    //			LOG(info) << "Value of GetPadY " << 	fUnpackPar->GetPadY(febId,channelId);
    fMuchStationMapYB[febId][channelId] = fUnpackPar->GetPadY(febNrB,channelId);
    }
    }*/ //To Do think if required then create MuchStationMap

  return kTRUE;

}
//----------------------------------------------------------------//
Bool_t CbmTSUnpackMuch::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
             << " microslices.";

  Int_t messageType = -111;
  // Loop over microslices
  size_t numCompMsInTs = ts.num_microslices(component);
  for (size_t m = 0; m < numCompMsInTs; ++m)
  {
      // Ignore overlap ms if number defined by user
      if( numCompMsInTs - fuOverlapMsNb <= m )
        continue;

      constexpr uint32_t kuBytesPerMessage = 8;
      auto msDescriptor = ts.descriptor(component, m);
      fEquipmentId = msDescriptor.eq_id;
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(component, m));
      uint32_t size = msDescriptor.size;
      LOG(debug) << "Microslice: " << msDescriptor.idx
		 << " has size: " << size;

      // If not integer number of message in input buffer, print warning/error
      if( 0 != (size % kuBytesPerMessage) )
        LOG(error) << "The input microslice buffer does NOT "
                   << "contain only complete nDPB messages!";

      // Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages = (size - (size % kuBytesPerMessage) )
	/ kuBytesPerMessage;

      // Prepare variables for the loop on contents
      const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( msContent );
      for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
        {
          // Fill message
          uint64_t ulData = static_cast<uint64_t>( pInBuff[uIdx] );
          ngdpb::Message mess( ulData );

          if(gLogger->IsLogNeeded( fair::Severity::debug )) {
            mess.printDataCout();
          }

          // Increment counter for different message types
          // and fill the corresponding histogram
          messageType = mess.getMessageType();
          fMsgCounter[messageType]++;

	  switch( messageType ) {
          case ngdpb::MSG_HIT:
            FillHitInfo(mess);
            break;
          case ngdpb::MSG_EPOCH:
            FillEpochInfo(mess);
            break;
          case ngdpb::MSG_SYNC:
            // Do nothing, this message is just there to make sure we get all Epochs
            break;
          case ngdpb::MSG_SYS:
            // Just keep track of which type of System message we receive
       	    // histSysMessType->Fill(mess.getSysMesType());
            break;
          default:
            LOG(error) << "Message type " << std::hex << std::setw(2)
                       << static_cast< uint16_t >( mess.getMessageType() )
                       << " not yet include in nXYTER unpacker.";
          }
        } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
    }
  return kTRUE;
}
//--------------------------------------------------------------------//

void CbmTSUnpackMuch::FillHitInfo(ngdpb::Message mess){
  // --- Get absolute time, NXYTER and channel number
  Int_t rocId      = mess.getRocNumber();
  Int_t nxyterId   = mess.getNxNumber();
  Int_t nxChannel  = mess.getNxChNum();
  Int_t charge     = mess.getNxAdcValue();

  // First check if nDPB is mapped
  //  if(!((fNdpbIdIndexMapA.end() == fNdpbIdIndexMapA.find( rocId ))||(fNdpbIdIndexMapB.end() == fNdpbIdIndexMapB.find( rocId ))))
  //LOG(fatal) << "Unmapped nDPB Id " << std::hex << rocId << std::dec;
  //Below creating Digi if mess is at GEM1 Roc
  if( !(fNdpbIdIndexMapA.end() == fNdpbIdIndexMapA.find( rocId )) ) {
    ULong_t hitTime  = mess.getMsgFullTime(fCurrentEpoch[rocId][nxyterId]);
    //Commented due to (Error in <TTree::Bronch>: TClonesArray with no class defined in branch: cbmout.MUCH raw data.MuchRawMessage)
    if (fCreateRawMessage) {
      new( (*fMuchRaw)[fMuchRaw->GetEntriesFast()] )
	CbmNxyterRawMessage(fEquipmentId,
			    rocId*4 + nxyterId,   //TODO check
			    nxChannel,
			    fCurrentEpoch[rocId][nxyterId], // note the trick
			    mess.getNxTs(),
			    charge,
			    mess.getNxLastEpoch(),
			    mess.getNxPileup(),
			    mess.getNxOverflow());

    }//if (fCreateRawMessage)

    LOG(debug) << "Hit: " << rocId << ", " << nxyterId
	       << ", " << nxChannel << ", " << charge;
    //Bellow line is for creating ASCII file

    // --- Compute detector element for GEM 1
    Int_t station = 0;
    Int_t layer   = 0;
    Int_t sector  = 0;
    Int_t channel = 0;

    // --- Construct unique address
    //UInt_t address = CreateAddress(rocId,nxyterId,0, 0, 0, 0, nxChannel);

    Int_t SysFebNum = fNdpbIdIndexMapA[rocId]*fUnpackPar->GetNrOfFebsPerNdpb() + nxyterId;
    //Below if condition is due to FEB connected in reverse direction due to FLEX cable.
    //This has to be implemented in the *.par file
    //if (SysFebNum==7||SysFebNum==8||SysFebNum==9||SysFebNum==10||SysFebNum==11){
    //  nxChannel=127-nxChannel;
    //}//if (SysFebNum ...

    //converted SysFebNum into the febNr as per the MuchUnpackPar.par file
    Int_t febNr = fUnpackPar->GetFebsIdsFromArrayA(SysFebNum);
    sector  = fUnpackPar->GetPadX(febNr, nxChannel);
    channel = fUnpackPar->GetPadY(febNr, nxChannel);
    //Bellow line is for creating ASCII file
    //cout << sector << "  " << channel << std::endl;
    UInt_t address = CbmMuchAddress::GetAddress(station, layer, 0, 0, sector, channel);

    // UInt_t address = CbmMuchAddress::GetAddress(station, layer, 0, 0, sector, channel);
    //cout<< "Address \t"<<address << " Charge\t "<< charge <<" "<<hitTime<< endl;

    // --- Create digi
    fDigi = new CbmMuchBeamTimeDigi(address, charge, hitTime);
    fDigi->SetPadX(sector);
    fDigi->SetPadY(channel);
    fDigi->SetRocId(rocId);
    fDigi->SetNxId(nxyterId);
    fDigi->SetNxCh(nxChannel);


    LOG(debug) << "MUCH message: rocId " << rocId << " NXYTER " << nxyterId
	       << " channel " << nxChannel << " charge " << charge << " time "
	     << hitTime;

    LOG(debug) << "Create digi with time " << hitTime
	       << " at epoch " << fCurrentEpoch[rocId][nxyterId];
    //Creating digi in CbmMuchBeamtimeDigi format
    //CbmMuchBeamTimeDigi::CbmMuchBeamTimeDigi()
    //			  : CbmDigi(), fData(0), fTime(0), fMatch(0), fPadX(-1), fPadY(-1), fRocId(-1), fNxId(-1), fNxCh(-1)

    //if(fDigi = new CbmMuchBeamTimeDigi(0,address, charge, hitTime);
    //Commented due to (Error in <TTree::Bronch>: TClonesArray with no class defined in branch: cbmout.MUCH raw data.MuchRawMessage)

    if(!fBuffer) cout << "------------- NULL pointer ------------- " << endl;
    if(!fBuffer) cout << "------------- NULL pointer ------------- " << endl;
    if(!fBuffer) {cout << "------------- NULL pointer ------------- " << endl;return;}

    fBuffer->InsertData<CbmMuchBeamTimeDigi>(fDigi);
  }//( !(fNdpbIdIndexMapA.end() == fNdpbIdIndexMapA.find( rocId ))
  else {
    if( !(fNdpbIdIndexMapB.end() == fNdpbIdIndexMapB.find( rocId)) ){

      ULong_t hitTime  = mess.getMsgFullTime(fCurrentEpoch[rocId][nxyterId]);
      //Commented due to (Error in <TTree::Bronch>: TClonesArray with no class defined in branch: cbmout.MUCH raw data.MuchRawMessage)
      if (fCreateRawMessage) {
	new( (*fMuchRaw)[fMuchRaw->GetEntriesFast()] )
	  CbmNxyterRawMessage(fEquipmentId,
			      rocId*4 + nxyterId,   //TODO check
			      nxChannel,
			      fCurrentEpoch[rocId][nxyterId], // note the trick
			      mess.getNxTs(),
			      charge,
			      mess.getNxLastEpoch(),
			      mess.getNxPileup(),
			      mess.getNxOverflow());

      }//if (fCreateRawMessage)

      LOG(debug) << "Hit: " << rocId << ", " << nxyterId
		 << ", " << nxChannel << ", " << charge;
      //Bellow line is for creating ASCII file

      // --- Compute detector element for GEM2
      Int_t station = 0;
      Int_t layer   = 1;
      Int_t sector  = 0;
      Int_t channel = 0;

      // --- Construct unique address
      //UInt_t address = CreateAddress(rocId,nxyterId,0, 0, 0, 0, nxChannel);

      Int_t SysFebNum = fNdpbIdIndexMapB[rocId]*fUnpackPar->GetNrOfFebsPerNdpb() + nxyterId;
      //Below conditions as per GEM 2 module connections
      //if (SysFebNum==7||SysFebNum==8||SysFebNum==9||SysFebNum==10||SysFebNum==11){
      //nxChannel=127-nxChannel;
      //}//if (SysFebNum ...

      //converted SysFebNum into the febNr as per the MuchUnpackPar.par file
      Int_t febNr = fUnpackPar->GetFebsIdsFromArrayB(SysFebNum);
      sector  = fUnpackPar->GetPadX(febNr, nxChannel);
      channel = fUnpackPar->GetPadY(febNr, nxChannel);
      //Bellow line is for creating ASCII file
      //cout << sector << "  " << channel << std::endl;
      UInt_t address = CbmMuchAddress::GetAddress(station, layer, 0, 0, sector, channel);

      // UInt_t address = CbmMuchAddress::GetAddress(station, layer, 0, 0, sector, channel);
      //cout<< "Address \t"<<address << " Charge\t "<< charge <<" "<<hitTime<< endl;

      // --- Create digi
      fDigi = new CbmMuchBeamTimeDigi(address, charge, hitTime);
      fDigi->SetPadX(sector);
      fDigi->SetPadY(channel);
      fDigi->SetRocId(rocId);
      fDigi->SetNxId(nxyterId);
      fDigi->SetNxCh(nxChannel);


      LOG(debug) << "MUCH message: rocId " << rocId << " NXYTER " << nxyterId
		 << " channel " << nxChannel << " charge " << charge << " time "
		 << hitTime;

      LOG(debug) << "Create digi with time " << hitTime
		 << " at epoch " << fCurrentEpoch[rocId][nxyterId];
      //Creating digi in CbmMuchBeamtimeDigi format
      //CbmMuchBeamTimeDigi::CbmMuchBeamTimeDigi()
      //			  : CbmDigi(), fData(0), fTime(0), fMatch(0), fPadX(-1), fPadY(-1), fRocId(-1), fNxId(-1), fNxCh(-1)

      //if(fDigi = new CbmMuchBeamTimeDigi(0,address, charge, hitTime);
      //Commented due to (Error in <TTree::Bronch>: TClonesArray with no class defined in branch: cbmout.MUCH raw data.MuchRawMessage)

      if(!fBuffer) cout << "------------- NULL pointer ------------- " << endl;
      if(!fBuffer) cout << "------------- NULL pointer ------------- " << endl;
      if(!fBuffer) {cout << "------------- NULL pointer ------------- " << endl;return;}

      fBuffer->InsertData<CbmMuchBeamTimeDigi>(fDigi);
    }//( !(fNdpbIdIndexMapB.end() == fNdpbIdIndexMapB.find( rocId))
    else {
      LOG(fatal) << "Unmapped nDPB Id " << std::hex << rocId << std::dec;
    } // if( fNdpbIdIndexMap.end() == fNdpbIdIndexMap.find( rocId ) )

  }


  /*new( (*fMuchDigi)[fMuchDigi->GetEntriesFast()] ) CbmSpsMuchDigi(*(dynamic_cast<CbmSpsMuchDigi*>(fDigi)));*/
  LOG(debug) << "Number of Digis created till  now " << fMuchDigi->size() << endl;
  //}//sid comment
  //else {
  //LOG(error) << "Unknown Roc Id " << rocId << " or nxyterId "<< nxyterId << " or channelId "<< nxChannel;
  //}
  //	fHM->H2("Raw_ADC_Much")->Fill(nxChannel,charge);
  //	fHM->H2("Pad_Distribution")->Fill(nxChannel,charge);

}//void CbmTSUnpackMuch::FillHitInfo(ngdpb::Message mess){
//-----------------------------------------------------------------------//


/*

  Int_t CbmTSUnpackMuch::CreateAddress(Int_t rocId, Int_t febId, Int_t stationId, Int_t layerId, Int_t sideId, Int_t moduleId, Int_t channelId)
  {

  Int_t SysFebNum = fNdpbIdIndexMap[rocId]*fUnpackPar->GetNrOfFebsPerNdpb() + febId;
  //Below if condition is due to FEB connected in reverse direction due to FLEX cable.
	if (SysFebNum==7||SysFebNum==8||SysFebNum==9||SysFebNum==10||SysFebNum==11){
		channelId=127-channelId;
	}

	//converted SysFebNum into the febNr as per the MuchUnpackPar.par file
	Int_t febNr = fUnpackPar->GetFebsIdsFromArray(SysFebNum);
	Int_t sector  = fUnpackPar->GetPadX(febNr, channelId);
	Int_t channel = fUnpackPar->GetPadY(febNr, channelId);
	//Bellow line is for creating ASCII file
	//cout << sector << "  " << channel << std::endl;
	Int_t address = CbmMuchAddress::GetAddress(stationId, layerId, sideId, moduleId, sector, channel);
	return address;
}
*/
void CbmTSUnpackMuch::FillEpochInfo(ngdpb::Message mess)
{
  Int_t rocId          = mess.getRocNumber();
  Int_t nxyterId       = mess.getEpochNxNum();

  // First check if nDPB is mapped
  if( fNdpbIdIndexMapA.end() == fNdpbIdIndexMapA.find( rocId ) || fNdpbIdIndexMapB.end() == fNdpbIdIndexMapB.find( rocId ))
    {
      //bellow should be FATAL error, converting it into INFO
      LOG(debug) << "Unmapped nDPB Id " << std::hex << rocId << std::dec;
    } // if( fNdpbIdIndexMap.end() == fNdpbIdIndexMap.find( rocId ) )

  fCurrentEpoch[rocId][nxyterId] = mess.getEpochNumber();

  //  LOG(info) << "Epoch message for ROC " << rocId << " with epoch number "
  //            << fCurrentEpoch[rocId];
  fCurrentEpochTime = mess.getMsgFullTime(fCurrentEpoch[rocId][nxyterId]);
  fNofEpochs++;
  LOG(debug) << "Epoch message "
             << fNofEpochs << ", epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][nxyterId])
             << ", time " << std::setprecision(9) << std::fixed
             << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
             << " for board ID " << std::hex << std::setw(4) << rocId << std::dec;

}

void CbmTSUnpackMuch::Reset()
{
  if (fCreateRawMessage) {
    fMuchRaw->Clear();
  }
//  fMuchDigi->Clear();
  fMuchDigi->clear();
}

void CbmTSUnpackMuch::Finish()
{
  TString message_type;

  for (unsigned int i=0; i< fMsgCounter.size(); ++i) {
    switch(i) {
    case 0: message_type ="NOP"; break;
    case 1: message_type ="HIT"; break;
    case 2: message_type ="EPOCH"; break;
    case 3: message_type ="SYNC"; break;
    case 4: message_type ="AUX"; break;
    case 5: message_type ="EPOCH2"; break;
    case 6: message_type ="GET4"; break;
    case 7: message_type ="SYS"; break;
    case 8: message_type ="GET4_SLC"; break;
    case 9: message_type ="GET4_32B"; break;
    case 10: message_type ="GET4_SYS"; break;
    }
    LOG(info) << message_type << " messages: "
              << fMsgCounter[i];
  }
   LOG(info) << "-------------------------------------";
   for( auto it = fCurrentEpoch.begin(); it != fCurrentEpoch.end(); ++it)
     for( auto itN = (it->second).begin(); itN != (it->second).end(); ++itN)
      LOG(info) << "Last epoch for nDPB: "
                << std::hex << std::setw(4) << it->first << std::dec
                << " , FEB  " << std::setw(4) << itN->first
                << " => " << itN->second;
   LOG(info) << "-------------------------------------";


/*  gDirectory->mkdir("Much_Raw");
  gDirectory->cd("Much_Raw");
  fHM->H2("Raw_ADC_Much")->Write();
  fHM->H2("Pad_Distribution")->Write();
  gDirectory->cd("..");
*/
}


void CbmTSUnpackMuch::FillOutput(boost::any _digi)
{

  CbmMuchBeamTimeDigi* digi = boost::any_cast<CbmMuchBeamTimeDigi*>(_digi);
  fMuchDigi->emplace_back(*(digi));
/*
  new( (*fMuchDigi)[fMuchDigi->GetEntriesFast()] )
CbmMuchBeamTimeDigi(*(boost::any_cast<CbmMuchBeamTimeDigi*>(digi)));
*/
  // Delete object pointed by digi after using it
  delete digi;
}

ClassImp(CbmTSUnpackMuch)
