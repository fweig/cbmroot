// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmUnpackTofStar2018                       -----
// -----               Created 08.12.2017 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmUnpackTofStar2018.h"
#include "CbmTofUnpackPar.h"
#include "CbmTofDigi.h"
//#include "CbmTbEvent.h"

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

static Int_t iMess=0;
const  Int_t DetMask = 0x0001FFFF;


CbmUnpackTofStar2018::CbmUnpackTofStar2018( UInt_t uNbGdpb )
  : CbmTSUnpack(),
    fuMsAcceptsPercent(100),
    fuOverlapMsNb(0),
    fuMinNbGdpb( uNbGdpb ),
    fuCurrNbGdpb( 0 ),
    fuNrOfGdpbs(0),
    fuNrOfFebsPerGdpb(0),
    fuNrOfGet4PerFeb(0),
    fuNrOfChannelsPerGet4(0),
    fuNrOfChannelsPerFeet(0),
    fuNrOfGet4(0),
    fuNrOfGet4PerGdpb(0),
    fuNrOfChannelsPerGdpb(0),
    fMsgCounter(11,0), // length of enum MessageTypes initialized with 0
    fGdpbIdIndexMap(),
    fuGdpbId(0),
    fuGdpbNr(0),
    fuGet4Id(0),
    fuGet4Nr(0),
    fHM(new CbmHistManager()),
    fvulCurrentEpoch(),
    fvbFirstEpochSeen(),
    fNofEpochs(0),
    fulCurrentEpochTime(0.),
    fEquipmentId(0),
    fdMsIndex(0.),
    fdTShiftRef(0.),
    fTofDigi(),
    fDigi(NULL),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fUnpackPar(NULL),
    fdRefTime(0.),
    fdLastDigiTime(0.),
    fdFirstDigiTimeDif(0.),
    fdEvTime0(0.),
    fhRawTDigEvT0( NULL ),
    fhRawTDigRef0( NULL ),
    fhRawTDigRef( NULL ),
    fhRawTRefDig0( NULL ),
    fhRawTRefDig1( NULL ),
    fhRawDigiLastDigi( NULL ),
    fhRawTotCh(),
    fhChCount(),
    fvbChanThere(),
    fhChanCoinc(),
    fhDetChanCoinc(nullptr),
    fvmEpSupprBuffer()
{
}

CbmUnpackTofStar2018::~CbmUnpackTofStar2018()
{
}

Bool_t CbmUnpackTofStar2018::Init()
{
   LOG(info) << "Initializing flib Get4 unpacker";

   FairRootManager* ioman = FairRootManager::Instance();
   if( NULL == ioman )
   {
      LOG(fatal) << "No FairRootManager instance";
   }

   fTofDigi= new TClonesArray("CbmTofDigi", 10);
   if( NULL == fTofDigi )
   {
      LOG(fatal) << "No Digi TClonesarray ";
   }
   ioman->Register("CbmTofDigi", "Tof raw Digi", fTofDigi, kTRUE);

/*
   CbmTbEvent * fEventHeader = (CbmTbEvent *)ioman->GetObject("EventHeader.");
   if( NULL == fEventHeade r)
   {
      LOG(fatal) << "No EventHeader TClonesarray ";
   }
*/

   fUnpackPar = (CbmTofUnpackPar*)(FairRun::Instance());

   return kTRUE;
}

void CbmUnpackTofStar2018::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar = (CbmTofUnpackPar*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmTofUnpackPar"));

}

Bool_t CbmUnpackTofStar2018::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();
   Bool_t initOK = ReInitContainers();

   CreateHistograms();

   fvulCurrentEpoch.resize( fuNrOfGdpbs * fuNrOfGet4PerGdpb );
   fvbFirstEpochSeen.resize( fuNrOfGdpbs * fuNrOfGet4PerGdpb );
   fvbChanThere.resize( fUnpackPar->GetNumberOfChannels(), kFALSE );
   for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
   {
      for( UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j )
      {
         fvulCurrentEpoch[GetArrayIndex(i, j)] = 0;
         fvbFirstEpochSeen[GetArrayIndex(i, j)] = kFALSE;
      } // for( UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j )
   } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

   return initOK;
}

Bool_t CbmUnpackTofStar2018::ReInitContainers()
{
   LOG(info) << "ReInit parameter containers for " << GetName();

   fuNrOfGdpbs = fUnpackPar->GetNrOfRocs();
   LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;
   fuMinNbGdpb = fuNrOfGdpbs;

   fuNrOfFebsPerGdpb = fUnpackPar->GetNrOfFebsPerGdpb();
   LOG(info) << "Nr. of FEBS per Tof GDPB: " << fuNrOfFebsPerGdpb;

   fuNrOfGet4PerFeb = fUnpackPar->GetNrOfGet4PerFeb();
   LOG(info) << "Nr. of GET4 per Tof FEB: " << fuNrOfGet4PerFeb;

   fuNrOfChannelsPerGet4 = fUnpackPar->GetNrOfChannelsPerGet4();
   LOG(info) << "Nr. of channels per GET4: " << fuNrOfChannelsPerGet4;

   fuNrOfChannelsPerFeet = fuNrOfGet4PerFeb * fuNrOfChannelsPerGet4;
   LOG(info) << "Nr. of channels per FEET: " << fuNrOfChannelsPerFeet;

   fuNrOfGet4 = fuNrOfGdpbs * fuNrOfFebsPerGdpb * fuNrOfGet4PerFeb;
   LOG(info) << "Nr. of GET4s: " << fuNrOfGet4;

   fuNrOfGet4PerGdpb = fuNrOfFebsPerGdpb * fuNrOfGet4PerFeb;
   LOG(info) << "Nr. of GET4s per GDPB: " << fuNrOfGet4PerGdpb;

   fuNrOfChannelsPerGdpb = fuNrOfGet4PerGdpb * fuNrOfChannelsPerGet4;
   LOG(info) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;

   fGdpbIdIndexMap.clear();
   for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
   {
      fGdpbIdIndexMap[fUnpackPar->GetRocId(i)] = i;
      LOG(info) << "GDPB Id of TOF  " << i << " : " << std::hex << fUnpackPar->GetRocId(i)
                 << std::dec;
   } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
   UInt_t uNrOfChannels = fUnpackPar->GetNumberOfChannels();
   LOG(info) << "Nr. of mapped Tof channels: " << uNrOfChannels;
   std::stringstream ss;
   for( UInt_t i = 0; i < uNrOfChannels; ++i)
   {
      if(i%8 == 0)
         ss << "\n";
      ss << Form(" 0x%08x",fUnpackPar->GetChannelToDetUIdMap(i));
   } // for( UInt_t i = 0; i < uNrOfChannels; ++i)
   LOG(info) << ss.str();

   LOG(info) << "Plot Channel Rate => "
             << (fUnpackPar->IsChannelRateEnabled() ? "ON" : "OFF");

   fvmEpSupprBuffer.resize( fuNrOfGet4 );

	return kTRUE;
}

void CbmUnpackTofStar2018::CreateHistograms()
{
   LOG(info) << "create Histos for " << fuNrOfGdpbs <<" gDPBs ";

   fhRawTDigEvT0 = new TH1F( Form("Raw_TDig-EvT0"),
                             Form("Raw digi time difference to 1st digi ; time [ns]; cts"),
                             100, 0, 50.);
   fHM->Add( Form("Raw_TDig-EvT0"), fhRawTDigEvT0);

   fhRawTDigRef0 = new TH1F( Form("Raw_TDig-Ref0"),
                             Form("Raw digi time difference to Ref ; time [ns]; cts"),
                             5000, 0, 500000);
   fHM->Add( Form("Raw_TDig-Ref0"), fhRawTDigRef0);

   fhRawTDigRef = new TH1F( Form("Raw_TDig-Ref"),
                            Form("Raw digi time difference to Ref ; time [ns]; cts"),
                            5000, 0, 50000);
   fHM->Add( Form("Raw_TDig-Ref"), fhRawTDigRef);

   fhRawTRefDig0 = new TH1F( Form("Raw_TRef-Dig0"),
                             Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
                             9999, -500000000, 500000000);
   fHM->Add( Form("Raw_TRef-Dig0"), fhRawTRefDig0);

   fhRawTRefDig1 = new TH1F( Form("Raw_TRef-Dig1"),
                             Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
                             9999, -5000000, 5000000);
   fHM->Add( Form("Raw_TRef-Dig1"), fhRawTRefDig1);

   fhRawDigiLastDigi = new TH1F( Form("Raw_Digi-LastDigi"),
                                 Form("Raw Digi time difference to last digi  ; time [ns]; cts"),
                                 9999, -5000000, 5000000);
   fHM->Add( Form("Raw_Digi-LastDigi"), fhRawDigiLastDigi);

   fhRawTotCh.resize( fuNrOfGdpbs );
   fhChCount.resize( fuNrOfGdpbs );
   fhChanCoinc.resize( fuNrOfGdpbs * fuNrOfFebsPerGdpb / 2 );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      fhRawTotCh[ uGdpb ] = new TH2F( Form("Raw_Tot_gDPB_%02u", uGdpb),
                                      Form("Raw TOT gDPB %02u; channel; TOT [bin]", uGdpb),
                                      fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb,
                                      256, 0., 256. );
      fHM->Add( Form("Raw_Tot_gDPB_%02u", uGdpb), fhRawTotCh[ uGdpb ]);

      fhChCount[ uGdpb ] = new TH1I( Form("ChCount_gDPB_%02u", uGdpb),
                                     Form("Channel counts gDPB %02u; channel; Hits", uGdpb),
                                     fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb );
      fHM->Add( Form("ChCount_gDPB_%02u", uGdpb), fhChCount[ uGdpb ]);
/*
      for( UInt_t uLeftFeb = uGdpb*fuNrOfFebsPerGdpb / 2;
           uLeftFeb < (uGdpb + 1 )*fuNrOfFebsPerGdpb / 2;
           ++uLeftFeb )
      {
         fhChanCoinc[ uLeftFeb ] = new TH2F( Form("fhChanCoinc_%02u", uLeftFeb),
                                      Form("Channels Coincidence %02; Left; Right", uLeftFeb),
                                      fuNrOfChannelsPerFeet, 0., fuNrOfChannelsPerFeet,
                                      fuNrOfChannelsPerFeet, 0., fuNrOfChannelsPerFeet );
      } // for( UInt_t uLeftFeb = 0; uLeftFeb < fuNrOfFebsPerGdpb / 2; uLeftFeb ++ )
*/
         fhChanCoinc[ uGdpb ] = new TH2F( Form("fhChanCoinc_%02u", uGdpb),
                                      Form("Channels Coincidence %02u; Left; Right", uGdpb),
                                      fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb,
                                      fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb );
   } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
   fhDetChanCoinc = new TH2F( "fhDetChanCoinc",
                              "Det Channels Coincidence; Left; Right",
                              32, 0., 32,
                              32, 0., 32 );
}

Bool_t CbmUnpackTofStar2018::DoUnpack(const fles::Timeslice& ts, size_t component)
{
   LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
              << " microslices of component " << component;

   // Loop over microslices
   Int_t iMessageType = -111;
   size_t numCompMsInTs = ts.num_microslices(component);
   for (size_t m = 0; m < numCompMsInTs; ++m)
   {
      // Jump some microslices if needed
//      if( fuMsAcceptsPercent < m)
//         continue;

      // Ignore overlap ms if number defined by user
      if( numCompMsInTs - fuOverlapMsNb <= m )
         continue;

      constexpr uint32_t kuBytesPerMessage = 8;

      auto msDescriptor = ts.descriptor(component, m);
      fEquipmentId = msDescriptor.eq_id;
      fdMsIndex = static_cast<double>(msDescriptor.idx);
      const uint8_t* msContent = reinterpret_cast< const uint8_t* >( ts.content(component, m) );

      uint32_t size = msDescriptor.size;
      if( 0 < size )
         LOG(debug1) << "Microslice "<< m <<": " << fdMsIndex
                     << " has size: " << size;

      // If not integer number of message in input buffer, print warning/error
      if( 0 != (size % kuBytesPerMessage) )
         LOG(error) << "The input microslice buffer does NOT "
                    << "contain only complete nDPB messages!";

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

         if( gLogger->IsLogNeeded( fair::Severity::debug2 ) )
         {
            mess.printDataCout();
         } // if( gLogger->IsLogNeeded( fair::Severity::debug2 ) )

         // Increment counter for different message types
         iMessageType = mess.getMessageType();
         fMsgCounter[ iMessageType ]++;

         fuGdpbId = mess.getRocNumber();
         fuGdpbNr = fGdpbIdIndexMap[ fuGdpbId ];

         fuGet4Id = mess.getGdpbGenChipId();
         fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;

         if( fuNrOfGet4PerGdpb <= fuGet4Id &&
             ngdpb::MSG_STAR_TRI != iMessageType &&
             get4v1x::kuChipIdMergedEpoch != fuGet4Id )
            LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id
                         << " VS " << fuNrOfGet4PerGdpb << " set in parameters.";

         switch( mess.getMessageType() )
         {
            case ngdpb::MSG_HIT:
            case ngdpb::MSG_EPOCH:
            case ngdpb::MSG_GET4:
            {
               //           FillEpochInfo(mess);
               LOG(error) << "Message type " << mess.getMessageType()
                          << " not included in unpacker.";
               break;
            } // case old non tof messages
            case ngdpb::MSG_EPOCH2:
            {
               if( get4v1x::kuChipIdMergedEpoch == fuGet4Id )
               {

                  for( UInt_t uChan = fuGdpbNr * fuNrOfChannelsPerGdpb;
                       uChan < (fuGdpbNr + 1 ) * fuNrOfChannelsPerGdpb;
                       ++uChan )
                     fvbChanThere[ uChan ] = kFALSE;

                  if( 0 == fuGdpbNr )
                     for( UInt_t uDetChan = 0; uDetChan < 64; uDetChan ++)
                        fbDetChanThere[uDetChan] = kFALSE;

                  for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index ++ )
                  {
                     ngdpb::Message tmpMess( mess );
                     tmpMess.setGdpbGenChipId( uGet4Index );
                     fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + uGet4Index;

//                     fHistGet4MessType->Fill(uGet4Index, ngdpb::GET4_32B_EPOCH);
                     FillEpochInfo( tmpMess );
                  } // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGetIndex ++ )

                 for( UInt_t uChanA = fuGdpbNr * fuNrOfChannelsPerGdpb;
                       uChanA < (fuGdpbNr + 1 ) * fuNrOfChannelsPerGdpb;
                       ++uChanA )
                 {
                    if( kTRUE == fvbChanThere[ uChanA ] )
                     {
                        for( UInt_t uChanB = fuGdpbNr * fuNrOfChannelsPerGdpb;
                             uChanB < (fuGdpbNr + 1 ) * fuNrOfChannelsPerGdpb;
                             ++uChanB )
                        {
                           if( kTRUE == fvbChanThere[ uChanB ] )
                           {
                              fhChanCoinc[ fuGdpbNr  ]->Fill( uChanA - fuGdpbNr * fuNrOfChannelsPerGdpb,
                                                              uChanB - fuGdpbNr * fuNrOfChannelsPerGdpb );
                           } // if uChanB
                        } // for uChanB
                     } // if uChanA
                  } // for uChanA

                  if( 0 == fuGdpbNr )
                  {
                     for( UInt_t uDetChanLeft = 0; uDetChanLeft < 32; uDetChanLeft ++)
                        if( kTRUE == fbDetChanThere[ uDetChanLeft ] )
                           for( UInt_t uDetChanRight = 0; uDetChanRight < 32; uDetChanRight ++)
                              if( kTRUE == fbDetChanThere[ 32 + uDetChanRight ] )
                                 fhDetChanCoinc->Fill( uDetChanLeft, uDetChanRight );
                  } // if( 0 == fuGdpbNr )
               } // if this epoch message is a merged one valiud for all chips
               else
               {
//                  fHistGet4MessType->Fill( fuGet4Nr, ngdpb::GET4_32B_EPOCH );
                  FillEpochInfo(mess);
               } // if single chip epoch message
               break;
            } // case ngdpb::MSG_EPOCH2:
            case ngdpb::MSG_GET4_32B:
            {
               fvmEpSupprBuffer[ fuGet4Nr ].push_back( mess );
               break;
            } // case ngdpb::MSG_GET4_32B:
            case ngdpb::MSG_GET4_SLC:
            {
               PrintSlcInfo(mess);
               break;
            } // case ngdpb::MSG_GET4_SLC:
            case ngdpb::MSG_GET4_SYS:
            {
               if(100 > iMess++)
                  PrintSysInfo(mess);
               break;
            } // case ngdpb::MSG_GET4_SYS:
            case ngdpb::MSG_STAR_TRI:
            {
               FillStarTrigInfo(mess);
               break;
            } // case ngdpb::MSG_STAR_TRI:
            default:
            {
               if(100 > iMess++)
                  LOG(error) << "Message ("<<iMess<<") type " << std::hex << std::setw(2)
                             << static_cast< uint16_t >( mess.getMessageType() )
                             << " not included in Get4 unpacker.";
               if(100 == iMess)
                  LOG(error) << "Stop reporting MSG errors... ";
            } // default:
         } // switch( mess.getMessageType() )

      } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

   } // for (size_t m = 0; m < numCompMsInTs; ++m)


  return kTRUE;
}

void CbmUnpackTofStar2018::FillHitInfo( ngdpb::Message mess )
{
   // --- Get absolute time, GET4 ID and channel number
   UInt_t uGet4Id     = mess.getGdpbGenChipId();
   UInt_t uChannel    = mess.getGdpbHitChanId();
   UInt_t uTot        = mess.getGdpbHit32Tot();
   // In 32b mode the coarse counter is already computed back to 112 FTS bins
   // => need to hide its contribution from the Finetime
   // => FTS = Fullt TS modulo 112
//   UInt_t uFts        = mess.getGdpbHitFullTs() % 112;

   ULong_t  ulCurEpochGdpbGet4 = fvulCurrentEpoch[ fuGet4Nr ];

   if( kTRUE == fvbFirstEpochSeen[ fuGet4Nr ] )
   {
      // In Ep. Suppr. Mode, receive following epoch instead of previous
      if( 0 < ulCurEpochGdpbGet4 )
         ulCurEpochGdpbGet4 --;
         else ulCurEpochGdpbGet4 = get4v1x::kuEpochCounterSz; // Catch epoch cycle!

//      ULong_t  ulhitTime = mess.getMsgG4v2FullTime(  ulCurEpochGdpbGet4 );
      Double_t dHitTime  = mess.getMsgG4v2FullTimeD( ulCurEpochGdpbGet4 );
      Double_t dHitTot   = uTot;     // in bins


//      UInt_t uFebIdx     = (uGet4Id / fuNrOfGet4PerFeb);
//      UInt_t uFullFebIdx = (fuGdpbNr * fuNrOfFebsPerGdpb) + uFebIdx;

      UInt_t uChanInGdpb = uGet4Id * fuNrOfChannelsPerGet4 + uChannel;
      UInt_t uChanInSyst = fuGdpbNr * fuNrOfChannelsPerGdpb + uChanInGdpb;
      if( fUnpackPar->GetNumberOfChannels() < 1 ||
          static_cast< UInt_t >( fUnpackPar->GetNumberOfChannels() ) < uChanInSyst )
      {
         LOG(error) << "Invalid mapping index " << uChanInSyst
                    << " VS " << fUnpackPar->GetNumberOfChannels()
                    <<", from " << fuGdpbNr
                    <<", " << uGet4Id
                    <<", " << uChannel;
         return;
      } // if( fUnpackPar->GetNumberOfChannels() < uChanUId )

      fvbChanThere[ uChanInSyst ] = kTRUE;

      UInt_t uChanUId = fUnpackPar->GetChannelToDetUIdMap( uChanInSyst );
      if( 0 == uChanUId )
         return;   // Hit not mapped to digi

      if( (uChanUId & DetMask) == 0x00001006 )
      {
         UInt_t uDetChan = (uChanUId & 0xFF000000) >> 24;
         if( (uChanUId & 0x00800000) == 0x00800000 )
            uDetChan += 32;

         fbDetChanThere[ uDetChan ] = kTRUE;
      } // if( (uChanUId & DetMask) == 0x00001006 )

      fhRawDigiLastDigi->Fill( dHitTime - fdLastDigiTime );
      fdLastDigiTime = dHitTime;

      if( (uChanUId & DetMask) == 0x00001006 )
         dHitTime += fdTShiftRef;

      LOG(debug) << Form("Insert 0x%08x digi with time ", uChanUId ) << dHitTime << Form(", Tot %4.0f",dHitTot)
                 << " into buffer with " << fBuffer->GetSize() << " data from "
                 << Form("%11.1f to %11.1f ", fBuffer->GetTimeFirst(), fBuffer->GetTimeLast())
                 << " at epoch " << ulCurEpochGdpbGet4;
      fDigi = new CbmTofDigi(uChanUId, dHitTime, dHitTot);

      fBuffer->InsertData<CbmTofDigi>(fDigi);

      // Histograms filling
      fhRawTotCh[ fuGdpbNr ]->Fill( uChanInGdpb, dHitTot);
      fhChCount[ fuGdpbNr ] ->Fill( uChanInGdpb );

   } // if( kTRUE == fvbFirstEpochSeen[ fuGet4Nr ] )
}

void CbmUnpackTofStar2018::FillEpochInfo( ngdpb::Message mess )
{
   ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();

   fvulCurrentEpoch[ fuGet4Nr ] = ulEpochNr;

   if( kFALSE == fvbFirstEpochSeen[ fuGet4Nr ] )
      fvbFirstEpochSeen[ fuGet4Nr ] = kTRUE;

   fulCurrentEpochTime = mess.getMsgFullTime(ulEpochNr);
   fNofEpochs++;

   /// In Ep. Suppr. Mode, receive following epoch instead of previous
   /// Re-align the epoch number of the message in case it will be used later:
   /// We received the epoch after the data instead of the one before!
   if( 0 < ulEpochNr )
      mess.setEpoch2Number( ulEpochNr - 1 );
   else mess.setEpoch2Number( get4v1x::kuEpochCounterSz );

   Int_t iBufferSize = fvmEpSupprBuffer[ fuGet4Nr ].size();
   if( 0 < iBufferSize )
   {
      LOG(debug) << "Now processing stored messages for for get4 " << fuGet4Nr << " with epoch number "
                 << (fvulCurrentEpoch[ fuGet4Nr ] - 1);

      for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
      {
         FillHitInfo( fvmEpSupprBuffer[ fuGet4Nr ][ iMsgIdx ] );
      } // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
/*
      for( UInt_t uLeftFeb = fuGdpbNr * fuNrOfFebsPerGdpb / 2;
           uLeftFeb < (fuGdpbNr + 1) * fuNrOfFebsPerGdpb / 2;
           ++uLeftFeb )
      {
         for( UInt_t uChanA = 2*uLeftFeb * fuNrOfChannelsPerFeet;
              uChanA < (2*uLeftFeb + 1) * fuNrOfChannelsPerFeet;
              ++uChanA )
         {
            if( kTRUE == fvbChanThere[ uChanA ] )
            {
               for( UInt_t uChanB = (2*uLeftFeb + 1) * fuNrOfChannelsPerFeet;
                    uChanB < (2*uLeftFeb + 2) * fuNrOfChannelsPerFeet;
                    ++uChanB )
               {
                  if( kTRUE == fvbChanThere[ uChanB ] )
                  {
                     fhChanCoinc[ uLeftFeb  ]->Fill( uChanA, uChanB );
                  }
               }
            }
         }
      } // for( UInt_t uLeftFeb = 0; uLeftFeb < fuNrOfFebsPerGdpb / 2; ++uLeftFeb )
*/
      fvmEpSupprBuffer[fuGet4Nr].clear();
   } // if( 0 < fvmEpSupprBuffer[fGet4Nr] )
}

void CbmUnpackTofStar2018::PrintSlcInfo(ngdpb::Message /*mess*/)
{
   /// Nothing to do, maybe later use it to trakc parameter changes like treshold?
/*
  if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
     LOG(info) << "GET4 Slow Control message, epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << rocId << std::dec
                << "\n"
                << " +++++++ > Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                << ", Chan = " << std::setw(1) << mess.getGdpbSlcChan()
                << ", Edge = " << std::setw(1) << mess.getGdpbSlcEdge()
                << ", Type = " << std::setw(1) << mess.getGdpbSlcType()
                << ", Data = " << std::hex << std::setw(6) << mess.getGdpbSlcData() << std::dec
                << ", Type = " << mess.getGdpbSlcCrc();
*/
}

void CbmUnpackTofStar2018::PrintGenInfo(ngdpb::Message mess)
{
   Int_t mType        = mess.getMessageType();
   Int_t rocId          = mess.getRocNumber();
   Int_t get4Id     = mess.getGdpbGenChipId();
   Int_t channel    = mess.getGdpbHitChanId();
   uint64_t            uData = mess.getData();
   if(100 > iMess++)
      LOG(info) << "Get4 MSG type "<<mType<<" from rocId "<<rocId<<", getId "<<get4Id
                << ", (hit channel) "<<channel
//                <<Form(" hex data %0llx ",uData);
                << " hex data " << FormatHexPrintout( uData, 0, '0' );
}

void CbmUnpackTofStar2018::PrintSysInfo(ngdpb::Message mess)
{
   LOG(info) << "GET4 System message,       epoch " << (fvulCurrentEpoch[ fuGet4Nr])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << fuGdpbId << std::dec;

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

void CbmUnpackTofStar2018::Reset()
{
  //  fFiberHodoRaw->Clear();
  fTofDigi->Clear();
}

void CbmUnpackTofStar2018::Finish()
{
   TString message_type;

   for( UInt_t uType = 0; uType < fMsgCounter.size(); ++uType)
   {
      switch(uType)
      {
         case 0:   message_type ="NOP"; break;
         case 1:   message_type ="HIT"; break;
         case 2:   message_type ="EPOCH"; break;
         case 3:   message_type ="SYNC"; break;
         case 4:   message_type ="AUX"; break;
         case 5:   message_type ="EPOCH2"; break;
         case 6:   message_type ="GET4"; break;
         case 7:   message_type ="SYS"; break;
         case 8:   message_type ="GET4_SLC"; break;
         case 9:   message_type ="GET4_32B"; break;
         case 10:  message_type ="GET4_SYS"; break;
         default:  message_type ="UNKNOWN"; break;
      } // switch(uType)
      LOG(info) << message_type << " messages: "
                << fMsgCounter[uType];
   } // for( UInt_t uType = 0; uType < fMsgCounter.size(); ++uType)

   LOG(info) << "-------------------------------------";
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
      for( UInt_t uGet4 = 0; uGet4 < fuNrOfGet4PerGdpb; ++uGet4 )
         LOG(info) << "Last epoch for gDPB: "<< std::setw(4) << uGdpb
                   << " , GET4  " << std::setw(4) << uGet4
                   << " => " << fvulCurrentEpoch[GetArrayIndex(uGdpb, uGet4)];
   LOG(info) << "-------------------------------------";


   gDirectory->mkdir("Tof_Raw_gDPB");
   gDirectory->cd("Tof_Raw_gDPB");
   fHM->H1( Form("Raw_TDig-Ref0") )->Write();
   fHM->H1( Form("Raw_TDig-Ref") )->Write();
   fHM->H1( Form("Raw_TRef-Dig0") )->Write();
   fHM->H1( Form("Raw_TRef-Dig1") )->Write();
   fHM->H1( Form("Raw_Digi-LastDigi") )->Write();
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      fHM->H2( Form("Raw_Tot_gDPB_%02u", uGdpb) )->Write();
      fHM->H1( Form("ChCount_gDPB_%02u", uGdpb) )->Write();
/*
      for( UInt_t uLeftFeb = uGdpb*fuNrOfFebsPerGdpb / 2;
           uLeftFeb < (uGdpb + 1 )*fuNrOfFebsPerGdpb / 2;
           ++uLeftFeb )
         fhChanCoinc[ uLeftFeb ]->Write();
*/
      fhChanCoinc[ uGdpb ]->Write();
   } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
   fhDetChanCoinc->Write();
   gDirectory->cd("..");

}


void CbmUnpackTofStar2018::FillOutput(CbmDigi* digi)
{
   if( 100 > iMess++ )
      LOG(debug) << "Fill digi TClonesarray with "
                 << Form("0x%08x", digi->GetAddress())
                 << " at " << static_cast<Int_t>( fTofDigi->GetEntriesFast() );


   new( (*fTofDigi)[ fTofDigi->GetEntriesFast() ] )
      CbmTofDigi( *( dynamic_cast<CbmTofDigi*>(digi) ) );
   //CbmTofDigi((CbmTofDigi *)digi);

   if( 0 == fTofDigi->GetEntriesFast())
      fdEvTime0=digi->GetTime();
      else fhRawTDigEvT0->Fill( digi->GetTime() - fdEvTime0 );

   if( (digi->GetAddress() & DetMask) != 0x00001006 )
   {
      fhRawTDigRef0->Fill( digi->GetTime() - fdRefTime);
      fhRawTDigRef->Fill( digi->GetTime() - fdRefTime);
   } // if( (digi->GetAddress() & DetMask) != 0x00001006 )
      else  fdRefTime = digi->GetTime();

   digi->Delete();
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

void CbmUnpackTofStar2018::FillStarTrigInfo(ngdpb::Message mess)
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
/*
         if( (uNewToken != fuStarTokenLast + 1) &&
             0 < fulGdpbTsFullLast && 0 < fulStarTsFullLast &&
             ( 4095 != fuStarTokenLast || 1 != uNewToken)  )
            LOG(warning) << "Possible error: STAR token did not increase by exactly 1! "
                         << Form("old = %5u vs new = %5u ", fuStarTokenLast,   uNewToken)
                         << Form("old = %12llu vs new = %12llu ", fulGdpbTsFullLast, ulNewGdpbTsFull)
                         << Form("old = %12llu vs new = %12llu ", fulStarTsFullLast, ulNewStarTsFull)
                         << Form("old = %2u vs new = %2u ", fuStarDaqCmdLast,  uNewDaqCmd)
                         << Form("old = %2u vs new = %2u ", fuStarTrigCmdLast, uNewTrigCmd);
*/
         fulGdpbTsFullLast = ulNewGdpbTsFull;
         fulStarTsFullLast = ulNewStarTsFull;
         fuStarTokenLast   = uNewToken;
         fuStarDaqCmdLast  = uNewDaqCmd;
         fuStarTrigCmdLast = uNewTrigCmd;

         Double_t dTot = 1.;
         Double_t dTime = fulGdpbTsFullLast * 6.25;
         if( 0. == fdFirstDigiTimeDif && 0. != fdLastDigiTime )
         {
            fdFirstDigiTimeDif = dTime - fdLastDigiTime;
            LOG(info) << "Default fake digi time shift initialized to " << fdFirstDigiTimeDif;
         } // if( 0. == fdFirstDigiTimeDif && 0. != fdLastDigiTime )

         dTime -= fdFirstDigiTimeDif;
         dTime += fdTShiftRef;

         LOG(debug) << "Insert fake digi with time " << dTime << ", Tot " << dTot;
         fhRawTRefDig0->Fill( dTime - fdLastDigiTime);
         fhRawTRefDig1->Fill( dTime - fdLastDigiTime);

         fDigi = new CbmTofDigi(0x00005006, dTime, dTot); // fake start counter signal
         fBuffer->InsertData<CbmTofDigi>(fDigi);
         break;
	  } // case 3
      default:
         LOG(fatal) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  } // switch( iMsgIndex )
}

ClassImp(CbmUnpackTofStar2018)
