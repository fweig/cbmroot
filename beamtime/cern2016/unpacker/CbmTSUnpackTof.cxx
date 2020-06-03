// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSUnpackTof                             -----
// -----               Created 27.10.2016 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTSUnpackTof.h"
#include "CbmTofUnpackPar.h"
#include "CbmTofDigi.h"
#include "CbmTbEvent.h"

#include "CbmFormatDecHexPrintout.h"

#include "CbmTbDaqBuffer.h"

//#include "CbmFiberHodoAddress.h"
#include "CbmHistManager.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"

#include "TClonesArray.h"
#include "TString.h"

#include <iostream>
#include <stdint.h>
#include <iomanip>

const  UInt_t kuNbChanGet4 =   4;
static UInt_t kuNbChanAfck = 128; //96;  // FIXME - should be read from parameter file
static Int_t iMess=0;
// const  Double_t FineTimeConvFactor=0.048828; (VF) unused
static Double_t RefTime=0.;
static Double_t LastDigiTime=0.;
static Double_t FirstDigiTimeDif=0.;
static Double_t EvTime0=0.;
const  Int_t DetMask = 0x0001FFFF;


CbmTSUnpackTof::CbmTSUnpackTof( UInt_t uNbGdpb )
  : CbmTSUnpack(),
    fuMsAcceptsPercent(100),
    fuOverlapMsNb(0),
    fuMinNbGdpb( uNbGdpb ),
    fuCurrNbGdpb( 0 ),
    fMsgCounter(11,0), // length of enum MessageTypes initialized with 0
    fGdpbIdIndexMap(),
    fHM(new CbmHistManager()),
    fCurrentEpoch(),
    fNofEpochs(0),
    fCurrentEpochTime(0.),
    fEquipmentId(0),
    fdTShiftRef(0.),
//    fFiberHodoRaw(new TClonesArray("CbmNxyterRawMessage", 10)),
    fTofDigi(nullptr),
//    fRawMessage(NULL),
    fDigi(NULL),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fUnpackPar(NULL),
    fbEpochSuppModeOn(kFALSE),
    fvmEpSupprBuffer()
{
}

CbmTSUnpackTof::~CbmTSUnpackTof()
{
}

Bool_t CbmTSUnpackTof::Init()
{
  LOG(info) << "Initializing flib Get4 unpacker";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  fTofDigi = RegisterOutput<CbmTofDigi>("CbmTofDigi");
//  fTofDigi= new TClonesArray("CbmTofDigi", 10);
  if (fTofDigi == NULL) {
    LOG(fatal) << "No Digi vector ";
  }
//  ioman->Register("CbmTofDigi", "Tof raw Digi", fTofDigi, kTRUE);

  /*
  CbmTbEvent * fEventHeader = (CbmTbEvent *)ioman->GetObject("EventHeader.");
  if (NULL == fEventHeader) {
    LOG(fatal) << "No EventHeader TClonesarray ";
  }
  */

  fUnpackPar = (CbmTofUnpackPar*)(FairRun::Instance());

  CreateHistograms();

  return kTRUE;
}

void CbmTSUnpackTof::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar = (CbmTofUnpackPar*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmTofUnpackPar"));

}

Bool_t CbmTSUnpackTof::InitContainers()
{
	LOG(info) << "Init parameter containers for " << GetName();
	return ReInitContainers();
}

Bool_t CbmTSUnpackTof::ReInitContainers()
{
	LOG(info) << "ReInit parameter containers for " << GetName();
	Int_t nrOfRocs = fUnpackPar->GetNrOfRocs();
        kuNbChanAfck = fUnpackPar->GetNrOfFebsPerGdpb()
	             * fUnpackPar->GetNrOfGet4PerFeb()
                     * fUnpackPar->GetNrOfChannelsPerGet4();
	LOG(info) << "Nr. of Tof Rocs: " << nrOfRocs
		  << ", Nr of Channel/Roc: "<< kuNbChanAfck;

	fGdpbIdIndexMap.clear();
	for (Int_t i = 0; i< nrOfRocs; ++i) {
	  fGdpbIdIndexMap[fUnpackPar->GetRocId(i)] = i;
	  LOG(info) << "Roc Id of TOF  " << i
			  << " : " << fUnpackPar->GetRocId(i);
	}
	Int_t NrOfChannels = fUnpackPar->GetNumberOfChannels();
	//Int_t fNrOfGet4    = NrOfChannels/4;
	LOG(info) << "Nr. of mapped Tof channels: " << NrOfChannels;
  std::stringstream ss;
	for (Int_t i = 0; i< NrOfChannels; ++i) {
	  if(i%8 == 0)
      ss << "\n";
	  ss << Form(" 0x%08x",fUnpackPar->GetChannelToDetUIdMap(i));
	}
	LOG(info) << ss.str();
	if( fbEpochSuppModeOn )
	{
	  fvmEpSupprBuffer.resize( nrOfRocs );
	  for (Int_t i = 0; i< nrOfRocs; ++i) {
	    Int_t nrOfGet4 = fUnpackPar->GetNrOfFebsPerGdpb()*fUnpackPar->GetNrOfGet4PerFeb();
	    fvmEpSupprBuffer[i].resize( nrOfGet4 );
	  }
	}
	return kTRUE;
}

void CbmTSUnpackTof::CreateHistograms()
{
   LOG(info) << "create Histos for " << fuMinNbGdpb <<" Rocs ";

     fHM->Add( Form("Raw_TDig-EvT0"),
           new TH1F( Form("Raw_TDig-EvT0"),
                     Form("Raw digi time difference to 1st digi ; time [ns]; cts"),
                     100, 0, 50.) );

     fHM->Add( Form("Raw_TDig-Ref0"),
           new TH1F( Form("Raw_TDig-Ref0"),
                     Form("Raw digi time difference to Ref ; time [ns]; cts"),
                     5000, 0, 500000) );

     fHM->Add( Form("Raw_TDig-Ref"),
           new TH1F( Form("Raw_TDig-Ref"),
                     Form("Raw digi time difference to Ref ; time [ns]; cts"),
                     5000, 0, 50000) );

     fHM->Add( Form("Raw_TRef-Dig0"),
           new TH1F( Form("Raw_TRef-Dig0"),
                     Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
                     9999, -500000000, 500000000) );
     fHM->Add( Form("Raw_TRef-Dig1"),
           new TH1F( Form("Raw_TRef-Dig1"),
                     Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
                     9999, -5000000, 5000000) );
     fHM->Add( Form("Raw_Digi-LastDigi"),
           new TH1F( Form("Raw_Digi-LastDigi"),
                     Form("Raw Digi time difference to last digi  ; time [ns]; cts"),
                     9999, -5000000, 5000000) );

   for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
   {
      fHM->Add( Form("Raw_Tot_gDPB_%02u", uGdpb),
           new TH2F( Form("Raw_Tot_gDPB_%02u", uGdpb),
                     Form("Raw TOT gDPB %02u; channel; TOT [bin]", uGdpb),
                     kuNbChanAfck, 0, kuNbChanAfck, 256, 0, 255) );
      fHM->Add( Form("ChCount_gDPB_%02u", uGdpb),
           new TH1I( Form("ChCount_gDPB_%02u", uGdpb),
                     Form("Channel counts gDPB %02u; channel; Hits", uGdpb),
                     kuNbChanAfck, 0, kuNbChanAfck ) );
   } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
}

Bool_t CbmTSUnpackTof::DoUnpack(const fles::Timeslice& ts, size_t component)
{

  LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
             << " microslices of component " << component;

  // Loop over microslices
  size_t numCompMsInTs = ts.num_microslices(component);
  for (size_t m = 0; m < numCompMsInTs; ++m)
    {
      // if( fuMsAcceptsPercent < m )         continue;

      // Ignore overlap ms if number defined by user
      if( numCompMsInTs - fuOverlapMsNb <= m )
        continue;

      constexpr uint32_t kuBytesPerMessage = 8;

      auto msDescriptor = ts.descriptor(component, m);
      fEquipmentId = msDescriptor.eq_id;
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(component, m));

      uint32_t size = msDescriptor.size;
      if(size>0)
	LOG(debug1) << "Microslice "<<m<<": " << msDescriptor.idx
                << " has size: " << size;

      // If not integer number of message in input buffer, print warning/error
      if( 0 != (size % kuBytesPerMessage) )
        LOG(error) << "The input microslice buffer does NOT "
                   << "contain only complete nDPB messages!" ;

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

          if(gLogger->IsLogNeeded( fair::Severity::debug1 )) {
            mess.printDataCout();
          }

          // Increment counter for different message types
          fMsgCounter[mess.getMessageType()]++;

          switch(mess.getMessageType()) {
          case ngdpb::MSG_HIT:
 //           FillHitInfo(mess);
            LOG(error) << "Message type " << mess.getMessageType()
                       << " not yet included in unpacker.";
            break;
          case ngdpb::MSG_EPOCH:
 //           FillEpochInfo(mess);
            LOG(error) << "Message type " << mess.getMessageType()
                       << " not yet included in unpacker.";
            break;
          case ngdpb::MSG_EPOCH2:
            FillEpochInfo(mess);
            break;
          case ngdpb::MSG_GET4:
            PrintGenInfo(mess);
            break;
	  case ngdpb::MSG_GET4_32B:
	    if( fbEpochSuppModeOn )
	      {
		Int_t rocId      = mess.getRocNumber();
		Int_t get4Id     = mess.getGdpbGenChipId();
		//		Int_t fGet4Nr    = fGdpbIdIndexMap[rocId]*kuNbChanAfck + get4Id*kuNbChanGet4;
		fvmEpSupprBuffer[fGdpbIdIndexMap[rocId]][get4Id].push_back( mess );
	      }
	    else   FillHitInfo(mess);
            break;
          case ngdpb::MSG_GET4_SLC:
            PrintSlcInfo(mess);
            break;
          case ngdpb::MSG_GET4_SYS:
	    if(100 > iMess++)
            PrintSysInfo(mess);
            break;

	  case ngdpb::MSG_STAR_TRI:
	    FillStarTrigInfo(mess);
	    break;

          default:
	    if(100 > iMess++)
	      LOG(error) << "Message ("<<iMess<<") type " << std::hex << std::setw(2)
                       << static_cast< uint16_t >( mess.getMessageType() )
                       << " not yet included in Get4 unpacker.";
	    if(100 == iMess)
	      LOG(error) << "Stop reporting MSG errors... ";
          }

        } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

    }


  return kTRUE;
}

void CbmTSUnpackTof::FillHitInfo(ngdpb::Message mess)
{
  // --- Get absolute time, NXYTER and channel number
  Int_t rocId      = mess.getRocNumber();
  Int_t get4Id     = mess.getGdpbGenChipId();
  Int_t channel    = mess.getGdpbHitChanId();
  Int_t tot        = mess.getGdpbHit32Tot();
  ULong_t hitTime  = mess.getMsgFullTime( 0 );

  if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
  {
    fHM->H2( Form("Raw_Tot_gDPB_%02u", fGdpbIdIndexMap[ rocId ]) )
       ->Fill( get4Id*kuNbChanGet4 + channel, tot);
    fHM->H1( Form("ChCount_gDPB_%02u", fGdpbIdIndexMap[ rocId ]) )
       ->Fill( get4Id*kuNbChanGet4 + channel );


    Int_t curEpochGdpbGet4 = fCurrentEpoch[rocId][get4Id];
    if( fbEpochSuppModeOn )
      curEpochGdpbGet4 --; // In Ep. Suppr. Mode, receive following epoch instead of previous
    hitTime  = mess.getMsgFullTime(curEpochGdpbGet4);

    Int_t Ft = mess.getGdpbHitFineTs();

    if(100 > iMess++)
    LOG(debug) << "Hit: " << Form("0x%08x ",rocId) << ", " << get4Id
            << ", " << channel << ", " << tot
	    << ", epoch " << fCurrentEpoch[rocId][get4Id]
	    << ", FullTime " << hitTime
	    << ", FineTime " << Ft;

    Int_t iChan = fGdpbIdIndexMap[rocId]*kuNbChanAfck + get4Id*kuNbChanGet4 + channel;
    if (iChan >  fUnpackPar->GetNumberOfChannels()){
      LOG(error) << "Invalid mapping index "<<iChan
		 <<", from " << fGdpbIdIndexMap[rocId]
		 <<", " << get4Id
		 <<", " << channel;
      return;
    }
    Int_t iChanUId = fUnpackPar->GetChannelToDetUIdMap( iChan );
    if(0==iChanUId) return;   // Hit not mapped to digi

    Double_t dTime = mess.getMsgFullTimeD( curEpochGdpbGet4 );
    Double_t dTot  = tot;     // in ps ?

    fHM->H1( Form("Raw_Digi-LastDigi") )
       ->Fill( dTime - LastDigiTime);

    LastDigiTime = dTime;

    if( (iChanUId & DetMask) == 0x00005006 ) dTime += fdTShiftRef;

    LOG(debug) << Form("Insert 0x%08x digi with time ",iChanUId)<< dTime<<Form(", Tot %4.0f",dTot)
	       << " into buffer with "<<fBuffer->GetSize()<<" data from "
	       <<Form("%11.1f to %11.1f ",fBuffer->GetTimeFirst(),fBuffer->GetTimeLast())
               << " at epoch " << fCurrentEpoch[rocId][get4Id];
    fDigi = new CbmTofDigi(iChanUId, dTime, dTot);

    fBuffer->InsertData<CbmTofDigi>(fDigi);

  }else
    LOG(warning) << "found rocId: " << Form("0x%08x ",rocId);

}

void CbmTSUnpackTof::FillEpochInfo(ngdpb::Message mess)
{
  Int_t rocId      = mess.getRocNumber();
  Int_t get4Id     = mess.getGdpbGenChipId();
  //  Int_t fGet4Nr    = fGdpbIdIndexMap[rocId]*kuNbChanAfck + get4Id*kuNbChanGet4;
  fCurrentEpoch[rocId][get4Id] = mess.getEpoch2Number();
  if( fbEpochSuppModeOn )
  {
    Int_t iBufferSize = fvmEpSupprBuffer[fGdpbIdIndexMap[rocId]][get4Id].size();
    if( 0 < iBufferSize )
    {
      LOG(debug) << "Now processing stored messages for roc " <<rocId<<", get4 "<<get4Id<< " with epoch number "
                 << (fCurrentEpoch[fGdpbIdIndexMap[rocId]][get4Id] - 1);
      for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
      {
        FillHitInfo( fvmEpSupprBuffer[fGdpbIdIndexMap[rocId]][get4Id][ iMsgIdx ] );
      } // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
      fvmEpSupprBuffer[fGdpbIdIndexMap[rocId]][get4Id].clear();
    } // if( 0 < fvmEpSupprBuffer[fGet4Nr] )
  } // if( fbEpochSuppModeOn )

  if( fGdpbIdIndexMap.end() == fGdpbIdIndexMap.find( rocId ) )
  {
     fGdpbIdIndexMap[ rocId ] = fuCurrNbGdpb;
     fuCurrNbGdpb ++;

     if( fuMinNbGdpb < fuCurrNbGdpb )
     {
        // Add new histo
        fHM->Add( Form("Raw_Tot_gDPB_%02u", fuMinNbGdpb),
           new TH2F( Form("Raw_Tot_gDPB_%02u", fuMinNbGdpb),
                     Form("Raw TOT gDPB %02u; channel; TOT [bin]", fuMinNbGdpb),
                     kuNbChanAfck, 0, kuNbChanAfck, 256, 0, 255) );

         fHM->Add( Form("ChCount_gDPB_%02u", fuMinNbGdpb),
              new TH1I( Form("ChCount_gDPB_%02u", fuMinNbGdpb),
                        Form("Channel counts gDPB %02u; channel; Hits", fuMinNbGdpb),
                        kuNbChanAfck, 0, kuNbChanAfck ) );
	 LOG(info)<<" Add histos for gDPB "<<fuMinNbGdpb<<", rocID "<<rocId;
        // increase fuMinNbGdpb
        fuMinNbGdpb++;
     } // if( fuMinNbGdpb < fuCurrNbGdpb )
  } // if( std::map::end == fGdpbIdIndexMap.find( rocId ) )

  //  LOG(info) << "Epoch message for ROC " << rocId << " with epoch number "
  //            << fCurrentEpoch[rocId];

  fCurrentEpochTime = mess.getMsgFullTime(fCurrentEpoch[rocId][get4Id]);
  fNofEpochs++;
  LOG(debug1) << "Epoch message "
              << fNofEpochs << ", epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
              << ", time " << std::setprecision(9) << std::fixed
              << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
              << " for board ID " << std::hex << std::setw(4) << rocId << std::dec
              << " and chip " << mess.getEpoch2ChipNumber();

}

void CbmTSUnpackTof::PrintSlcInfo(ngdpb::Message mess)
{
  Int_t rocId          = mess.getRocNumber();
  Int_t get4Id     = mess.getGdpbGenChipId();

  if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
     LOG(info) << "GET4 Slow Control message, epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << rocId << std::dec
                << "\n"
                << " +++++++ > Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                << ", Chan = " << std::setw(1) << mess.getGdpbSlcChan()
                << ", Edge = " << std::setw(1) << mess.getGdpbSlcEdge()
                << ", Type = " << std::setw(1) << mess.getGdpbSlcType()
                << ", Data = " << std::hex << std::setw(6) << mess.getGdpbSlcData() << std::dec
                << ", Type = " << mess.getGdpbSlcCrc();

}

void CbmTSUnpackTof::PrintGenInfo(ngdpb::Message mess)
{
  Int_t mType        = mess.getMessageType();
  Int_t rocId          = mess.getRocNumber();
  Int_t get4Id     = mess.getGdpbGenChipId();
  Int_t channel    = mess.getGdpbHitChanId();
  uint64_t            uData = mess.getData();
  if(100 > iMess++)
  LOG(info) << "Get4 MSG type "<<mType<<" from rocId "<<rocId<<", getId "<<get4Id
	    << ", (hit channel) "<<channel
//      <<Form(" hex data %0llx ",uData);
      << " hex data " << FormatHexPrintout( uData, 0, '0' );
}

void CbmTSUnpackTof::PrintSysInfo(ngdpb::Message mess)
{
  Int_t rocId          = mess.getRocNumber();
  Int_t get4Id     = mess.getGdpbGenChipId();

  if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
     LOG(info) << "GET4 System message,       epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << rocId << std::dec;

   switch( mess.getGdpbSysSubType() )
   {
      case ngdpb::SYSMSG_GET4_EVENT:
      {
         LOG(info) << " +++++++ > Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                   << ", Chan = " << std::setw(1) << mess.getGdpbSysErrChanId()
                   << ", Edge = " << std::setw(1) << mess.getGdpbSysErrEdge()
                   << ", Empt = " << std::setw(1) << mess.getGdpbSysErrUnused()
                   << ", Data = " << std::hex << std::setw(2) << mess.getGdpbSysErrData() << std::dec
                   << " -- GET4 V1 Error Event";
         break;
      } //
      case ngdpb::SYSMSG_CLOSYSYNC_ERROR:
         LOG(info) << "Closy synchronization error";
         break;
      case ngdpb::SYSMSG_TS156_SYNC:
         LOG(info) << "156.25MHz timestamp reset";
         break;
      case ngdpb::SYSMSG_GDPB_UNKWN:
         LOG(info) << "Unknown GET4 message, data: " << std::hex << std::setw(8)
                   << mess.getGdpbSysUnkwData() << std::dec;
         break;
   } // switch( getGdpbSysSubType() )
}

void CbmTSUnpackTof::Reset()
{
  //  fFiberHodoRaw->Clear();
//  fTofDigi->Clear();
  fTofDigi->clear();
}

void CbmTSUnpackTof::Finish()
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
    default:  message_type ="UNKNOWN"; break;
    }
    LOG(info) << message_type << " messages: "
              << fMsgCounter[i];
  }

   LOG(info) << "-------------------------------------";
   for( auto it = fCurrentEpoch.begin(); it != fCurrentEpoch.end(); ++it)
      for( auto itG = (it->second).begin(); itG != (it->second).end(); ++itG)
      LOG(info) << "Last epoch for gDPB: "
                << std::hex << std::setw(4) << it->first
                << " , GET4  " << std::setw(4) << itG->first
                << " => " << itG->second;
   LOG(info) << "-------------------------------------";


   gDirectory->mkdir("Tof_Raw_gDPB");
   gDirectory->cd("Tof_Raw_gDPB");
   fHM->H1( Form("Raw_TDig-Ref0") )->Write();
   fHM->H1( Form("Raw_TDig-Ref") )->Write();
   fHM->H1( Form("Raw_TRef-Dig0") )->Write();
   fHM->H1( Form("Raw_TRef-Dig1") )->Write();
   fHM->H1( Form("Raw_Digi-LastDigi") )->Write();
   for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
   {
      fHM->H2( Form("Raw_Tot_gDPB_%02u", uGdpb) )->Write();
      fHM->H1( Form("ChCount_gDPB_%02u", uGdpb) )->Write();
   } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
   gDirectory->cd("..");

}


void CbmTSUnpackTof::FillOutput(boost::any _digi)
{
 CbmTofDigi* digi = boost::any_cast<CbmTofDigi*>(_digi);
 if(100 > iMess++)
 LOG(debug) << "Fill digi vector with "
	    <<Form("0x%08x",digi->GetAddress())
	    <<" at " << (Int_t)fTofDigi->size();


// new( (*fTofDigi)[fTofDigi->GetEntriesFast()] )
//    CbmTofDigi(*(boost::any_cast<CbmTofDigi*>(digi)));
    fTofDigi->emplace_back(*(digi));
    //CbmTofDigi((CbmTofDigi *)digi);

 if(0==fTofDigi->size()) EvTime0=digi->GetTime();
 else
       fHM->H1( Form("Raw_TDig-EvT0") )
	   ->Fill( digi->GetTime() - EvTime0);

 if( (digi->GetAddress() & DetMask) != 0x00005006 ) {
       fHM->H1( Form("Raw_TDig-Ref0") )
	  ->Fill( digi->GetTime() - RefTime);
       fHM->H1( Form("Raw_TDig-Ref") )
	  ->Fill( digi->GetTime() - RefTime);
 }
 else  RefTime=digi->GetTime();

 delete digi;

}

static    ULong64_t fulGdpbTsMsb;
static    ULong64_t fulGdpbTsLsb;
static    ULong64_t fulStarTsMsb;
static    ULong64_t fulStarTsMid;
static    ULong64_t fulGdpbTsFullLast;
static    ULong64_t fulStarTsFullLast;
static    UInt_t    fuStarTokenLast;
static    UInt_t    fuStarDaqCmdLast;
static    UInt_t    fuStarTrigCmdLast;

void CbmTSUnpackTof::FillStarTrigInfo(ngdpb::Message mess)
{
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

  switch( iMsgIndex )
  {
      case 0:
         fulGdpbTsMsb = mess.getGdpbTsMsbStarA();
         break;
      case 1:
         fulGdpbTsLsb = mess.getGdpbTsLsbStarB();
         fulStarTsMsb = mess.getStarTsMsbStarB();
         break;
      case 2:
         fulStarTsMid = mess.getStarTsMidStarC();
         break;
      case 3:
      {
         ULong64_t ulNewGdpbTsFull = ( fulGdpbTsMsb << 24 )
                           + ( fulGdpbTsLsb       );
         ULong64_t ulNewStarTsFull = ( fulStarTsMsb << 48 )
                           + ( fulStarTsMid <<  8 )
                           + mess.getStarTsLsbStarD();
         UInt_t uNewToken  = mess.getStarTokenStarD();
         UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
         UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();
         if( ( uNewToken == fuStarTokenLast ) && ( ulNewGdpbTsFull == fulGdpbTsFullLast ) &&
             ( ulNewStarTsFull == fulStarTsFullLast ) && ( uNewDaqCmd == fuStarDaqCmdLast ) &&
             ( uNewTrigCmd == fuStarTrigCmdLast ) )
         {
            LOG(debug) << "Possible error: identical STAR tokens found twice in a row => ignore 2nd! "
                         << Form("token = %5u ", fuStarTokenLast )
                         << Form("gDPB ts  = %12llu ", fulGdpbTsFullLast )
                         << Form("STAR ts = %12llu ", fulStarTsFullLast )
                         << Form("DAQ cmd = %2u ", fuStarDaqCmdLast )
                         << Form("TRG cmd = %2u ", fuStarTrigCmdLast );
            return;
         } // if exactly same message repeated

         if( (uNewToken != fuStarTokenLast + 1) &&
             0 < fulGdpbTsFullLast && 0 < fulStarTsFullLast &&
             ( 4095 != fuStarTokenLast || 1 != uNewToken)  )
            LOG(warning) << "Possible error: STAR token did not increase by exactly 1! "
                         << Form("old = %5u vs new = %5u ", fuStarTokenLast,   uNewToken)
                         << Form("old = %12llu vs new = %12llu ", fulGdpbTsFullLast, ulNewGdpbTsFull)
                         << Form("old = %12llu vs new = %12llu ", fulStarTsFullLast, ulNewStarTsFull)
                         << Form("old = %2u vs new = %2u ", fuStarDaqCmdLast,  uNewDaqCmd)
                         << Form("old = %2u vs new = %2u ", fuStarTrigCmdLast, uNewTrigCmd);

         fulGdpbTsFullLast = ulNewGdpbTsFull;
         fulStarTsFullLast = ulNewStarTsFull;
         fuStarTokenLast   = uNewToken;
         fuStarDaqCmdLast  = uNewDaqCmd;
         fuStarTrigCmdLast = uNewTrigCmd;
	 Double_t dTot = 1.;
	 Double_t dTime = fulGdpbTsFullLast * 6.25;
	 if (FirstDigiTimeDif==0. && LastDigiTime!=0.) {
	   FirstDigiTimeDif=dTime-LastDigiTime;
	   LOG(info) << "Default fake digi time shift initialized to " << FirstDigiTimeDif;
	 }
	 dTime -= FirstDigiTimeDif;
	 dTime += fdTShiftRef;
	 LOG(debug) << "Insert fake digi with time " << dTime<<", Tot "<<dTot;
         fHM->H1( Form("Raw_TRef-Dig0") )
	    ->Fill( dTime - LastDigiTime);
         fHM->H1( Form("Raw_TRef-Dig1") )
	    ->Fill( dTime - LastDigiTime);

	 fDigi = new CbmTofDigi(0x00005006, dTime, dTot); // fake start counter signal
	 fBuffer->InsertData<CbmTofDigi>(fDigi);
         break;
	  } // case 3
      default:
         LOG(fatal) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  } // switch( iMsgIndex )
}

ClassImp(CbmTSUnpackTof)
