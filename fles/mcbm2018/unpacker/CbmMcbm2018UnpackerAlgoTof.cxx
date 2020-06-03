// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018UnpackerAlgoTof                       -----
// -----               Created 10.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018UnpackerAlgoTof.h"

#include "CbmFormatMsHeaderPrintout.h"
#include "CbmFormatDecHexPrintout.h"
#include "CbmMcbm2018TofPar.h"
#include "CbmTofAddress.h"
#include "CbmTofDetectorId_v14a.h" // in cbmdata/tof

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRunOnline.h"

#include "TROOT.h"
#include "TList.h"
#include "TString.h"
#include "TH2.h"
#include "TProfile.h"
#include "TH1.h"
#include "TCanvas.h"

#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <fstream>
#include <vector>

/*
static Int_t  NMappingWarnings=100;
static uint32_t pat_mess[8]={8*0};
std::vector< std::vector <uint32_t> > Pat_Request;
std::vector<Bool_t> bGdpbOK;
static Bool_t    bEnableOut=kFALSE;
static Int_t fiMsGood=0;
static Int_t fiMsBad=0;
*/
// -------------------------------------------------------------------------
CbmMcbm2018UnpackerAlgoTof::CbmMcbm2018UnpackerAlgoTof() :
   CbmStar2019Algo(),
   /// From the class itself
   fbMonitorMode( kFALSE ),
   fbDebugMonitorMode( kFALSE ),
   fvbMaskedComponents(),
   fUnpackPar( nullptr ),
   fuNrOfGdpbs( 0 ),
   fGdpbIdIndexMap(),
   fuNrOfFeePerGdpb( 0 ),
   fuNrOfGet4PerFee( 0 ),
   fuNrOfChannelsPerGet4( 0 ),
   fuNrOfChannelsPerFee( 0 ),
   fuNrOfGet4( 0 ),
   fuNrOfGet4PerGdpb( 0 ),
   fuNrOfChannelsPerGdpb( 0 ),
   fuNrOfGbtx( 0 ),
   fuNrOfModules( 0 ),
   fviNrOfRpc(),
   fviRpcType(),
   fviRpcSide(),
   fviModuleId(),
   fviRpcChUId(),
   fdTimeOffsetNs( 0.0 ),
   fuDiamondDpbIdx( 99 ),
   fulCurrentTsIdx( 0 ),
   fulCurrentMsIdx( 0 ),
   fuCurrentMsSysId( 0 ),
   fdTsStartTime( -1.0 ),
   fdTsStopTimeCore( -1.0 ),
   fdMsTime( -1.0 ),
   fuMsIndex( 0 ),
   fuCurrentEquipmentId( 0 ),
   fuCurrDpbId( 0 ),
   fuCurrDpbIdx( 0 ),
   fuGet4Id( 0 ),
   fuGet4Nr( 0 ),
   fvulCurrentEpoch(),
   fvulCurrentEpochCycle(),
   fvulCurrentEpochFull(),
   fdStartTime( 0.0 ),
   fdStartTimeMsSz( 0.0 ),
   ftStartTimeUnix( std::chrono::steady_clock::now() ),
   fvvmEpSupprBuffer(),
   fvulGdpbTsMsb(),
   fvulGdpbTsLsb(),
   fvulStarTsMsb(),
   fvulStarTsMid(),
   fvulGdpbTsFullLast(),
   fvulStarTsFullLast(),
   fvuStarTokenLast(),
   fvuStarDaqCmdLast(),
   fvuStarTrigCmdLast(),
   fdRefTime(0.),
   fdLastDigiTime(0.),
   fdFirstDigiTimeDif(0.),
   fdEvTime0(0.),
   fhRawTDigEvT0( nullptr ),
   fhRawTDigRef0( nullptr ),
   fhRawTDigRef( nullptr ),
   fhRawTRefDig0( nullptr ),
   fhRawTRefDig1( nullptr ),
   fhRawDigiLastDigi( nullptr ),
   fhRawTotCh(),
   fhChCount(),
   fhChCountRemap(),
   fvbChanThere(),
   fhChanCoinc(),
   fhDetChanCoinc( nullptr )
{
}
CbmMcbm2018UnpackerAlgoTof::~CbmMcbm2018UnpackerAlgoTof()
{
   /// Clear buffers
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      fvvmEpSupprBuffer[ uGdpb ].clear();
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoTof::Init()
{
   LOG(info) << "Initializing mCBM TOF 2019 unpacker algo";

   return kTRUE;
}
void CbmMcbm2018UnpackerAlgoTof::Reset()
{
}
void CbmMcbm2018UnpackerAlgoTof::Finish()
{
/*
   /// Printout Goodbye message and stats
  LOG(INFO)<<"<I> MS statistics - Good: " << fiMsGood <<", Bad: " << fiMsBad;
*/
   /// Write Output histos
}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoTof::InitContainers()
{
   LOG(info) << "Init parameter containers for CbmMcbm2018UnpackerAlgoTof";

   Bool_t initOK = ReInitContainers();

   return initOK;
}
Bool_t CbmMcbm2018UnpackerAlgoTof::ReInitContainers()
{
   LOG(info) << "**********************************************";
   LOG(info) << "ReInit parameter containers for CbmMcbm2018UnpackerAlgoTof";

   fUnpackPar = (CbmMcbm2018TofPar*)fParCList->FindObject("CbmMcbm2018TofPar");
   if( nullptr == fUnpackPar ) {
      LOG(ERROR) << " CbmMcbm2018TofPar not found ";
      return kFALSE;
   }
   Bool_t initOK = InitParameters();

   return initOK;
}
TList* CbmMcbm2018UnpackerAlgoTof::GetParList()
{
   if( nullptr == fParCList )
      fParCList = new TList();
   fUnpackPar = new CbmMcbm2018TofPar("CbmMcbm2018TofPar");
   fParCList->Add(fUnpackPar);

   return fParCList;
}
Bool_t CbmMcbm2018UnpackerAlgoTof::InitParameters()
{
    LOG(INFO) << "InitParameters from " << fUnpackPar;
   fdMsSizeInNs  = fUnpackPar->GetSizeMsInNs();

   fuNrOfGdpbs = fUnpackPar->GetNrOfGdpbs();
   LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;

   fuNrOfFeePerGdpb = fUnpackPar->GetNrOfFeesPerGdpb();
   LOG(info) << "Nr. of FEES per Tof GDPB: " << fuNrOfFeePerGdpb;

   fuNrOfGet4PerFee = fUnpackPar->GetNrOfGet4PerFee();
   LOG(info) << "Nr. of GET4 per Tof FEE: " << fuNrOfGet4PerFee;

   fuNrOfChannelsPerGet4 = fUnpackPar->GetNrOfChannelsPerGet4();
   LOG(info) << "Nr. of channels per GET4: " << fuNrOfChannelsPerGet4;

   fuNrOfChannelsPerFee = fuNrOfGet4PerFee * fuNrOfChannelsPerGet4;
   LOG(info) << "Nr. of channels per FEE: " << fuNrOfChannelsPerFee;

   fuNrOfGet4 = fuNrOfGdpbs * fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
   LOG(info) << "Nr. of GET4s: " << fuNrOfGet4;

   fuNrOfGet4PerGdpb = fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
   LOG(info) << "Nr. of GET4s per GDPB: " << fuNrOfGet4PerGdpb;

   fuNrOfChannelsPerGdpb = fuNrOfGet4PerGdpb * fuNrOfChannelsPerGet4;
   LOG(info) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;

   fGdpbIdIndexMap.clear();
   for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
   {
      fGdpbIdIndexMap[fUnpackPar->GetGdpbId(i)] = i;
      LOG(info) << "GDPB Id of TOF  " << i << " : " << std::hex << fUnpackPar->GetGdpbId(i)
                 << std::dec;
   } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

   fuNrOfGbtx  =  fUnpackPar->GetNrOfGbtx();
   LOG(info) << "Nr. of GBTx: " << fuNrOfGbtx;

   fviRpcType.resize(  fuNrOfGbtx );
   fviModuleId.resize( fuNrOfGbtx );
   fviNrOfRpc.resize(  fuNrOfGbtx );
   fviRpcSide.resize(  fuNrOfGbtx );
   for( UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)
   {
      fviNrOfRpc[ uGbtx ]  = fUnpackPar->GetNrOfRpc( uGbtx );
      fviRpcType[ uGbtx ]  = fUnpackPar->GetRpcType( uGbtx );
      fviRpcSide[ uGbtx ]  = fUnpackPar->GetRpcSide( uGbtx );
      fviModuleId[ uGbtx ] = fUnpackPar->GetModuleId( uGbtx );
   } // for( UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)

   UInt_t uNrOfChannels = fuNrOfGet4 * fuNrOfChannelsPerGet4;
   LOG(info) << "Nr. of possible Tof channels: " << uNrOfChannels;

//   CbmTofDetectorId* fTofId = new CbmTofDetectorId_v14a();
   fviRpcChUId.resize( uNrOfChannels );
   UInt_t iCh= 0;
   for(UInt_t iGbtx= 0; iGbtx < fuNrOfGbtx; ++iGbtx )
   {
     switch(fviRpcType[iGbtx]) {

     case 0:  // CBM modules
       if(fviRpcSide[iGbtx]<2){    // mTof modules
	 const Int_t RpcMap[5]={4,2,0,3,1};
	 for(Int_t iRpc= 0; iRpc < fviNrOfRpc[iGbtx]; iRpc++)  {
	   Int_t iStrMax=32;
	   Int_t iChNext=1;

	   for(Int_t iStr= 0; iStr < iStrMax; iStr++)  {
	     Int_t iStrMap = iStr;
	     Int_t iRpcMap = RpcMap[iRpc];

	     if( fviRpcSide[iGbtx] == 0) iStrMap=31-iStr;
	     if(fviModuleId[iGbtx] > -1)
	       fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
								iRpcMap,iStrMap,
								fviRpcSide[iGbtx],
								fviRpcType[iGbtx]);
	     else
	       fviRpcChUId[iCh]=0;
//	 LOG(debug)<<Form("Map Ch %d to Address 0x%08x",iCh,fviRpcChUId[iCh]);
	     iCh += iChNext;
	   }
	 }
       }
       break;

     case 1:  // STAR eTOF  modules
       if(fviRpcSide[iGbtx]<2){    // mTof modules
	 const Int_t RpcMap[3]={0,1,2};
	 for(Int_t iRpc= 0; iRpc < fviNrOfRpc[iGbtx]; iRpc++)  {
	   Int_t iStrMax=32;
	   Int_t iChNext=1;

	   for(Int_t iStr= 0; iStr < iStrMax; iStr++)  {
	     Int_t iStrMap = iStr;
	     Int_t iRpcMap = RpcMap[iRpc];

	     if( fviRpcSide[iGbtx] == 0) iStrMap=31-iStr;
	     if(fviModuleId[iGbtx] > -1)
	       fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
								iRpcMap,iStrMap,
								fviRpcSide[iGbtx],
								fviRpcType[iGbtx]);
	     else
	       fviRpcChUId[iCh]=0;
//	 LOG(DEBUG)<<Form("Map Ch %d to Address 0x%08x",iCh,fviRpcChUId[iCh]);
	     iCh += iChNext;
	   }
	 }
       }
       iCh += 64;
       break;

      /// Special Treatment for the T0 diamond
     case 5:
      {
	 LOG(info) << " Map diamond  at GBTX  -  iCh = " << iCh;
         for( UInt_t uFee = 0; uFee < fUnpackPar->GetNrOfFeePerGbtx(); ++uFee )
         {
            for( UInt_t uCh = 0; uCh < fUnpackPar->GetNrOfChannelsPerFee(); ++uCh )
            {
/*
	      if( uFee < 4 && 0 == uCh ) {
                  fviRpcChUId[ iCh ] = CbmTofAddress::GetUniqueAddress(
                                             fviModuleId[iGbtx],
                                             0, uFee + 4 * fviRpcSide[iGbtx],
                                             0, fviRpcType[iGbtx] );
		  LOG(info) << Form( "Map T0 Ch %d to Address 0x%08x", iCh, fviRpcChUId[iCh] );
	      }
	      else fviRpcChUId[ iCh ] = 0;
*/

               /// Mapping for the 2019 beamtime
               if( 0 == uFee && 1 == fviNrOfRpc[iGbtx] )
               {
                  switch( uCh % 8 )
                  {
                     case 0:
		     case 2:
                     case 4:
                     {
                        /// 2019 mapping with 320/640 Mb/s FW
                        /// => 4 GET4 per GBTx
                        /// => 1 T0 channel per GET4
                        /// => 1-2 eLinks per GET4 => GET4 ID = GET4 * 2 (+ 1)
                        UInt_t uChannelT0 = uCh / 8 + 4 * fviRpcSide[iGbtx];
                        fviRpcChUId[ iCh ] = CbmTofAddress::GetUniqueAddress(
                                                fviModuleId[iGbtx],
                                                0, uChannelT0,
                                                0, fviRpcType[iGbtx] );
                        LOG(info) << Form( "T0 channel: %u from GBTx %2u Fee %2u Channel %2u, indx %d address %08x",
					   uChannelT0, iGbtx, uFee, uCh, iCh, fviRpcChUId[ iCh ] );
                        break;
                     } // Valid T0 channel
                     default:
                     {
                        fviRpcChUId[ iCh ] = 0;
                     } // Invalid T0 channel
                  } // switch( uCh % 4 )
               } // if( 0 == uFee )

               iCh++;
            } // for( UInt_t uCh = 0; uCh < fUnpackPar->GetNrOfChannelsPerFee(); ++uCh )
         } // for( UInt_t uFee = 0; uFee < fUnpackPar->GetNrOfFeePerGbtx(); ++uFee )
      } // if( 5 == fviRpcType[iGbtx] )
      break;

     case 78: // cern-20-gap + ceramic module
	 {
	   LOG(info) << " Map CERN 20 gap  at GBTX  -  iCh = " << iCh;
	   const Int_t StrMap[32]={0,1,2,3,4,31,5,6,7,30,8,9,10,29,11,12,13,14,28,15,16,17,18,27,26,25,24,23,22,21,20,19};
	   Int_t iModuleId = 0;
	   Int_t iModuleType=7;
	   Int_t iRpcMap = 0;
	   for(Int_t iFeet= 0; iFeet <2; iFeet++){
	     for(Int_t iStr= 0; iStr < 32; iStr++)  {
	       Int_t iStrMap = 31-12-StrMap[iStr];
	       Int_t iSideMap = iFeet;
	       if (iStrMap < 20 )
		 fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(iModuleId,
								  iRpcMap,iStrMap,
								  iSideMap,
								  iModuleType);
	       else
		 fviRpcChUId[iCh]=0;
	       iCh++;
	     }
	   }

	 LOG(info) << " Map end CERN 20 gap  at GBTX  -  iCh = " << iCh;
	 }
         // fall through is intended
     case 8: // ceramics
	 {
	   Int_t iModuleId=0;
	   Int_t iModuleType=8;
	   for(Int_t iRpc= 0; iRpc < 8; iRpc++)  {
	     fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(iModuleId,
							      7-iRpc,0,0,
							      iModuleType);
	     iCh++;
	   }
	   iCh += (24 + 2*32);

	 }

	 LOG(info) << " Map end ceramics  box  at GBTX  -  iCh = " << iCh;
	 break;

     case 9: // Star2 boxes
	 {
	   LOG(info) << " Map Star2 box  at GBTX  -  iCh = " << iCh;
	   const Int_t iRpc[5] ={1,-1,1,0,0};
	   const Int_t iSide[5]={1,-1,0,1,0};
	   for(Int_t iFeet= 0; iFeet <5; iFeet++){
	     for(Int_t iStr= 0; iStr < 32; iStr++)  {
	       Int_t iStrMap = iStr;
	       Int_t iRpcMap = iRpc[iFeet];
	       Int_t iSideMap = iSide[iFeet];
	       if(iSideMap == 0) iStrMap=31-iStr;
	       if( iSideMap > -1)
		 fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
								  iRpcMap,iStrMap,
								  iSideMap,
								  fviRpcType[iGbtx]);
	       else
		 fviRpcChUId[iCh]=0;
	       iCh++;
	     }
	   }
	 }
	 break;

     case 6: // Buc box
	 {
	   LOG(info) << " Map Buc box  at GBTX  -  iCh = " << iCh;
	   const Int_t iRpc[5] ={0,-1,0,1,1};
	   const Int_t iSide[5]={1,-1,0,1,0};
	   for(Int_t iFeet= 0; iFeet <5; iFeet++){
	     for(Int_t iStr= 0; iStr < 32; iStr++)  {
	       Int_t iStrMap = iStr;
	       Int_t iRpcMap = iRpc[iFeet];
	       Int_t iSideMap = iSide[iFeet];
	       switch (fviRpcSide[iGbtx]) {
	       case 0:
		 ;
		 break;
	       case  1:   // HD cosmic 2019, Buc2018, v18n
		 iStrMap=31-iStr;
		 iRpcMap=1-iRpcMap;
		 break;
	       case 2:  // v18m_cosmicHD
		 //		 iStrMap=31-iStr;
		 iSideMap=1-iSideMap;
		 break;
	       case 3:
		 iStrMap=31-iStr;
		 iRpcMap=1-iRpcMap;
		 iSideMap=1-iSideMap;
		 break;
	       case  4:   // HD cosmic 2019, Buc2018, v18o
		 iRpcMap=1-iRpcMap;
		 break;
	       case  5:   // HD cosmic 2020, Buc2018, v20a
		 iStrMap=31-iStr;
		 break;
	       default:
		 ;
	       }
	       if( iSideMap > -1)
		 fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
								  iRpcMap,iStrMap,
								  iSideMap,
								  fviRpcType[iGbtx]);
	       else
		 fviRpcChUId[iCh]=0;

	       iCh++;
	     }
	   }
	 }
	 break;

     case -1:
       LOG(info) << " Found unused GBTX link at iCh = " << iCh;
	 iCh+=160;
         break;

     default:
       LOG(error) << "Invalid Tof Type  specifier ";

     }
   }
   TString sPrintout = "";
   for( UInt_t uCh = 0; uCh < uNrOfChannels; ++uCh )
   {
      if( 0 == uCh % 8 )
         sPrintout += "\n";
      sPrintout += Form(" 0x%08x", fviRpcChUId[ uCh ] );
   } // for( UInt_t i = 0; i < uNrOfChannels; ++i)
   LOG(info) << sPrintout;

   // Request masks
   /*
   LOG(INFO) << " Load " << fUnpackPar->GetNrReqPattern() << " GET4 Request masks for " << fuNrOfGdpbs << " Gdpbs ";
   if(fUnpackPar->GetNrReqPattern()>0){
     bGdpbOK.resize(fuNrOfGdpbs);
     Pat_Request.resize(fuNrOfGdpbs);
     Int_t iInd=0;
     for(Int_t iGdpb=0; iGdpb<fuNrOfGdpbs; iGdpb++) {
       bGdpbOK[iGdpb]=kTRUE;
       Pat_Request[iGdpb].resize(fUnpackPar->GetNrReqPattern());
       for (Int_t iPat=0; iPat<fUnpackPar->GetNrReqPattern(); iPat++) {
	 UInt_t PatGet4=fUnpackPar->GetReqPattern(iInd++);
         for( UInt_t uBit = 0; uBit < 32; ++uBit ) {
	   if( ( PatGet4 >> uBit ) & 0x1 ) {
	     UInt_t iGet4=iPat*32+uBit;
	     UInt_t uElink=fUnpackPar->Get4IdxToElinkIdx(iGet4);
	     UInt_t ubit=uElink%32;
	     UInt_t iEPat=(uElink-ubit)/32;
	     Pat_Request[iGdpb][iEPat] |= (0x1 << ubit);
	   }
	 }
       }
     }
   }
   */
   /// Internal status initialization
   fvulCurrentEpoch.resize( fuNrOfGdpbs, 0 );
   fvulCurrentEpochCycle.resize( fuNrOfGdpbs, 0 );
   fvulCurrentEpochFull.resize( fuNrOfGdpbs, 0 );

   /// STAR Trigger decoding and monitoring
   fvulGdpbTsMsb.resize(  fuNrOfGdpbs );
   fvulGdpbTsLsb.resize(  fuNrOfGdpbs );
   fvulStarTsMsb.resize(  fuNrOfGdpbs );
   fvulStarTsMid.resize(  fuNrOfGdpbs );
   fvulGdpbTsFullLast.resize(  fuNrOfGdpbs );
   fvulStarTsFullLast.resize(  fuNrOfGdpbs );
   fvuStarTokenLast.resize(  fuNrOfGdpbs );
   fvuStarDaqCmdLast.resize(  fuNrOfGdpbs );
   fvuStarTrigCmdLast.resize(  fuNrOfGdpbs );
   for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
   {
      fvulGdpbTsMsb[ uGdpb ] = 0;
      fvulGdpbTsLsb[ uGdpb ] = 0;
      fvulStarTsMsb[ uGdpb ] = 0;
      fvulStarTsMid[ uGdpb ] = 0;
      fvulGdpbTsFullLast[ uGdpb ] = 0;
      fvulStarTsFullLast[ uGdpb ] = 0;
      fvuStarTokenLast[ uGdpb ]   = 0;
      fvuStarDaqCmdLast[ uGdpb ]  = 0;
      fvuStarTrigCmdLast[ uGdpb ] = 0;
   } // for (Int_t iGdpb = 0; iGdpb < fuNrOfGdpbs; ++iGdpb)

   /// Buffer initialization
   fvvmEpSupprBuffer.resize( fuNrOfGdpbs );

	return kTRUE;
}
// -------------------------------------------------------------------------

void CbmMcbm2018UnpackerAlgoTof::AddMsComponentToList( size_t component, UShort_t usDetectorId )
{
   /// Check for duplicates and ignore if it is the case
   for( UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx )
      if( component == fvMsComponentsList[ uCompIdx ] )
         return;

   /// Add to list
   fvMsComponentsList.push_back( component );

   LOG(info) << "CbmMcbm2018UnpackerAlgoTof::AddMsComponentToList => Component "
             << component << " with detector ID 0x"
             << std::hex << usDetectorId << std::dec << " added to list";

}
// -------------------------------------------------------------------------

Bool_t CbmMcbm2018UnpackerAlgoTof::ProcessTs( const fles::Timeslice& ts )
{
   fulCurrentTsIdx = ts.index();
   fdTsStartTime = static_cast< Double_t >( ts.descriptor( 0, 0 ).idx );
   LOG(DEBUG) << "ProcessTs "<< fulCurrentTsIdx;

   /// Ignore First TS as first MS is typically corrupt
   if( 0 == fulCurrentTsIdx )
   {
      return kTRUE;
   } // if( 0 == fulCurrentTsIdx )

   /// On first TS, extract the TS parameters from header (by definition stable over time)
   if( -1.0 == fdTsCoreSizeInNs )
   {
      fuNbCoreMsPerTs = ts.num_core_microslices();
      fuNbOverMsPerTs = ts.num_microslices( 0 ) - ts.num_core_microslices();
      fdTsCoreSizeInNs = fdMsSizeInNs * ( fuNbCoreMsPerTs );
      fdTsFullSizeInNs = fdMsSizeInNs * ( fuNbCoreMsPerTs + fuNbOverMsPerTs );
      LOG(info) << "Timeslice parameters: each TS has "
                << fuNbCoreMsPerTs << " Core MS and "
                << fuNbOverMsPerTs << " Overlap MS, for a core duration of "
                << fdTsCoreSizeInNs << " ns and a full duration of "
                << fdTsFullSizeInNs << " ns";

      /// Ignore overlap ms if flag set by user
      fuNbMsLoop = fuNbCoreMsPerTs;
      if( kFALSE == fbIgnoreOverlapMs )
         fuNbMsLoop += fuNbOverMsPerTs;
      LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
   } // if( -1.0 == fdTsCoreSizeInNs )

   /// Compute time of TS core end
   fdTsStopTimeCore = fdTsStartTime + fdTsCoreSizeInNs;
//      LOG(info) << Form( "TS %5d Start %12f Stop %12f", fulCurrentTsIdx, fdTsStartTime, fdTsStopTimeCore );

   /// Loop over core microslices (and overlap ones if chosen)
   for( fuMsIndex = 0; fuMsIndex < fuNbMsLoop; fuMsIndex ++ )
   {
      /// Loop over registered components
      for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )
      {
         UInt_t uMsComp = fvMsComponentsList[ uMsCompIdx ];

         if( kFALSE == ProcessMs( ts, uMsComp, fuMsIndex ) )
         {
            LOG(error) << "Failed to process ts " << fulCurrentTsIdx << " MS " << fuMsIndex
                       << " for component " << uMsComp;
            return kFALSE;
         } // if( kFALSE == ProcessMs( ts, uMsCompIdx, fuMsIndex ) )
      } // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )
   } // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )

   /// Fill plots if in monitor mode
   if( fbMonitorMode )
   {
      if( kFALSE == FillHistograms() )
      {
         LOG(error) << "Failed to fill histos in ts " << fulCurrentTsIdx;
         return kFALSE;
      } // if( kFALSE == FillHistograms() )

      fhVectorSize->Fill( fulCurrentTsIdx, fDigiVect.size() );
      fhVectorCapacity->Fill( fulCurrentTsIdx, fDigiVect.capacity() );
   } // if( fbMonitorMode )

   if( fuTsMaxVectorSize < fDigiVect.size() )
   {
      fuTsMaxVectorSize = fDigiVect.size() * fdCapacityIncFactor;
      fDigiVect.shrink_to_fit();
      fDigiVect.reserve( fuTsMaxVectorSize );
   } // if( fuTsMaxVectorSize < fDigiVect.size() )
/*
   if(!bEnableOut) {
     LOG(DEBUG) << "Ts  "<<   fulCurrentTsIdx << " removed ";
     fiMsBad++;
     fDigiVect.clear();
   }else {
     LOG(DEBUG) << "Ts  "<<   fulCurrentTsIdx << " accepted ";
     fiMsGood++;
   }
*/
   /// Sort the buffers of hits due to the time offsets applied
   sort(fDigiVect.begin(), fDigiVect.end(),
        [](const CbmTofDigi & a, const CbmTofDigi & b) -> bool
        {
          return a.GetTime() < b.GetTime();
        });

   return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoTof::ProcessMs( const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx )
{
   auto msDescriptor = ts.descriptor( uMsCompIdx, uMsIdx );
   fuCurrentEquipmentId = msDescriptor.eq_id;
   const uint8_t* msContent = reinterpret_cast<const uint8_t*>( ts.content( uMsCompIdx, uMsIdx ) );

   uint32_t uSize  = msDescriptor.size;
   fulCurrentMsIdx = msDescriptor.idx;
//   Double_t dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
   LOG(debug) << "Microslice: " << fulCurrentMsIdx
              << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
              << " has size: " << uSize;

   if( 0 == fvbMaskedComponents.size() )
      fvbMaskedComponents.resize( ts.num_components(), kFALSE );

   fuCurrDpbId  = static_cast< uint32_t >( fuCurrentEquipmentId & 0xFFFF );
//   fuCurrDpbIdx = fDpbIdIndexMap[ fuCurrDpbId ];

   /// Check if this sDPB ID was declared in parameter file and stop there if not
   auto it = fGdpbIdIndexMap.find( fuCurrDpbId );
   if( it == fGdpbIdIndexMap.end() )
   {
      if( kFALSE == fvbMaskedComponents[ uMsCompIdx ] )
      {
         LOG(info) << "---------------------------------------------------------------";
         LOG(info) << FormatMsHeaderPrintout( msDescriptor );
         LOG(warning) << "Could not find the gDPB index for AFCK id 0x"
                      << std::hex << fuCurrDpbId << std::dec
                      << " in timeslice " << fulCurrentTsIdx
                      << " in microslice " << uMsIdx
                      << " component " << uMsCompIdx
                      << "\n"
                      << "If valid this index has to be added in the TOF parameter file in the DbpIdArray field";
         fvbMaskedComponents[ uMsCompIdx ] = kTRUE;
      } // if( kFALSE == fvbMaskedComponents[ uMsComp ] )
         else return kTRUE;

      /// Try to get it from the second message in buffer (first is epoch cycle without gDPB ID)
      /// TODO!!!!

      return kFALSE;
   } // if( it == fGdpbIdIndexMap.end() )
      else fuCurrDpbIdx = fGdpbIdIndexMap[ fuCurrDpbId ];

   fuCurrentMsSysId = static_cast<unsigned int>(msDescriptor.sys_id);

   // If not integer number of message in input buffer, print warning/error
   if( 0 != ( uSize % sizeof( gdpbv100::Message ) ) )
      LOG(error) << "The input microslice buffer does NOT "
                 << "contain only complete gDPB messages!";

   // Compute the number of complete messages in the input microslice buffer
   uint32_t uNbMessages = ( uSize - ( uSize % sizeof( gdpbv100::Message ) ) )
                          / sizeof( gdpbv100::Message );

   // Prepare variables for the loop on contents
   Int_t messageType = -111;
   const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( msContent ); // for epoch cycle
   const gdpbv100::Message * pMess = reinterpret_cast< const gdpbv100::Message * >( pInBuff );
   for( uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++ )
   {
      /// Catch the Epoch cycle block which is always the first 64b of the MS
      if( 0 == uIdx )
      {
         ProcessEpochCycle( pInBuff[uIdx] );
         continue;
      } // if( 0 == uIdx )

      /// Get message type
      messageType = pMess[ uIdx ].getMessageType();

      fuGet4Id = fUnpackPar->ElinkIdxToGet4Idx( pMess[ uIdx ].getGdpbGenChipId() );
      fuGet4Nr = (fuCurrDpbIdx * fuNrOfGet4PerGdpb) + fuGet4Id;

      if( fuNrOfGet4PerGdpb <= fuGet4Id &&
          !pMess[ uIdx ].isStarTrigger()  &&
          ( gdpbv100::kuChipIdMergedEpoch != fuGet4Id ) )
         LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id
                      << " VS " << fuNrOfGet4PerGdpb << " set in parameters.";

/*
         if( 1 == uIdx && gdpbv100::MSG_EPOCH != messageType )
            LOG(warning) << " in timeslice " << fulCurrentTsIdx
                         << " in microslice " << fuMsIndex
                         << " component " << uMsCompIdx
                         << " first message is not an epoch: type " << messageType;

         if( uNbMessages - 1 == uIdx && gdpbv100::MSG_EPOCH != messageType )
            LOG(warning) << " in timeslice " << fulCurrentTsIdx
                         << " in microslice " << fuMsIndex
                         << " component " << uMsCompIdx
                         << " last message is not an epoch: type " << messageType;
*/
      /// FIXME mCBM 2018-2019: the 2nd message in the microslice should ALWAYS be
      /// an EPOCH, but we observed a few cases were a HIT from the previous MS is
      /// inserted first
      /// ===> This HIT is in wrong TS and not properly time sorted if in first MS
      /// ===> Temporary solution: simply ignore these hits, may decrease efficiency
      /// ====> Not sufficient if more than one HIT is in wrong MS !!!
      if( 1 == uIdx && gdpbv100::MSG_EPOCH != messageType )
      {
         LOG(debug) << " CbmMcbm2018UnpackerAlgoTof ==> In timeslice " << fulCurrentTsIdx
                      << " in microslice " << fuMsIndex
                      << " component " << uMsCompIdx
                      << " first message is not an epoch: type " << messageType
                      << " -> It will be ignored! ";
         continue;
      } // if( 1 == uIdx && gdpbv100::MSG_EPOCH != messageType )

      switch( messageType )
      {
         case gdpbv100::MSG_HIT:
         {
            if( pMess[ uIdx ].getGdpbHitIs24b() )
            {
               LOG(error) << "This event builder does not support 24b hit message!!!"
                          << " Message " << uIdx << "/" << uNbMessages
                          << " 0x" << FormatHexPrintout( pMess[ uIdx ].getData(), '0', 16 );
               continue;
            } // if( getGdpbHitIs24b() )
               else
               {
                  fvvmEpSupprBuffer[fuCurrDpbIdx].push_back( pMess[ uIdx ] );
               } // else of if( getGdpbHitIs24b() )
            break;
         } // case gdpbv100::MSG_HIT:
         case gdpbv100::MSG_EPOCH:
         {
            if( gdpbv100::kuChipIdMergedEpoch == fuGet4Id )
            {
               ProcessEpoch( pMess[ uIdx ], uIdx);
            } // if this epoch message is a merged one valid for all chips
               else
               {
                  /// Should be checked in monitor task, here we just jump it
                  LOG(debug2) << "This event builder does not support unmerged epoch messages!!!.";
                  continue;
               } // if single chip epoch message
            break;
         } // case gdpbv100::MSG_EPOCH:
         case gdpbv100::MSG_SLOWC:
         {
            fvvmEpSupprBuffer[fuCurrDpbIdx].push_back( pMess[ uIdx ] );
            break;
         } // case gdpbv100::MSG_SLOWC:
         case gdpbv100::MSG_SYST:
         {
            fvvmEpSupprBuffer[fuCurrDpbIdx].push_back( pMess[ uIdx ] );
            break;
         } // case gdpbv100::MSG_SYST:
         case gdpbv100::MSG_STAR_TRI_A:
         case gdpbv100::MSG_STAR_TRI_B:
         case gdpbv100::MSG_STAR_TRI_C:
         case gdpbv100::MSG_STAR_TRI_D:
         {
            ProcessStarTrigger( pMess[ uIdx ] );

            /// If A message, check that the following ones are B, C, D
            /// ==> TBD only if necessary
/*
            if( gdpbv100::MSG_STAR_TRI_A == messageType )
            {
            } // if( gdpbv100::MSG_STAR_TRI_A == messageType )
*/
            break;
         } // case gdpbv100::MSG_STAR_TRI_A-D
         default:
            LOG(error) << "Message type " << std::hex
                       << std::setw(2) << static_cast<uint16_t>(messageType)
                       << " not included in Get4 unpacker.";
      } // switch( mess.getMessageType() )
   } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

   /// FIXME: in the 2018-2019 data, we had only the epoch message before each
   ///        block of data in the MS
   /// ===> Until it is decided either to get rid of the epoch suppr. buffer or
   ///      to implement the "closing epoch" in the FW, do the following:
   /// For now this method is used to fake a "closing" epoch at the end of the MS
   ProcessEndOfMsEpoch();

   return kTRUE;
}

// -------------------------------------------------------------------------
void CbmMcbm2018UnpackerAlgoTof::ProcessEpochCycle( const uint64_t & ulCycleData )
{
   ULong64_t ulEpochCycleVal = ulCycleData & gdpbv100::kulEpochCycleFieldSz;

   if( !( ulEpochCycleVal == fvulCurrentEpochCycle[fuCurrDpbIdx] ||
          ulEpochCycleVal == fvulCurrentEpochCycle[fuCurrDpbIdx] + 1 ) &&
       0 < fulCurrentMsIdx ) {
      LOG(warning) << "CbmMcbm2018UnpackerAlgoTof::ProcessEpochCycle => "
                 << " Missmatch in epoch cycles detected for Gdpb " << fuCurrDpbIdx <<", probably fake cycles due to epoch index corruption! "
                 << Form( " Current cycle 0x%09llX New cycle 0x%09llX", fvulCurrentEpochCycle[fuCurrDpbIdx], ulEpochCycleVal );
   } // if epoch cycle did not stay constant or increase by exactly 1, except if first MS of the TS
   if( ulEpochCycleVal != fvulCurrentEpochCycle[fuCurrDpbIdx] )
   {
      LOG(info) << "CbmMcbm2018UnpackerAlgoTof::ProcessEpochCycle   => "
                 << " New epoch cycle for Gdpb " << fuCurrDpbIdx
                 << Form( ": Current cycle 0x%09llX New cycle 0x%09llX", fvulCurrentEpochCycle[fuCurrDpbIdx], ulEpochCycleVal );
   } // if( ulEpochCycleVal != fvulCurrentEpochCycle[fuCurrDpbIdx] )
   fvulCurrentEpochCycle[fuCurrDpbIdx] = ulEpochCycleVal;

   return;
}
void CbmMcbm2018UnpackerAlgoTof::ProcessEpoch( const gdpbv100::Message & mess, uint32_t /*uMesgIdx*/ )
{
   ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();
/*
   if( (  6 == fulCurrentTsIdx && 2 == fuCurrDpbIdx ) ||
       ( 57 == fulCurrentTsIdx && 0 == fuCurrDpbIdx ) )
      LOG(info) << Form( "Gdpb %d TS %3llu MS %3u Msg %4u Ep %08llx",
                           fuCurrDpbIdx, fulCurrentTsIdx, fuMsIndex, uMesgIdx, ulEpochNr );

   if( !( ulEpochNr == fvulCurrentEpoch[ fuCurrDpbIdx ] + 1 ||
//          ulEpochNr == fvulCurrentEpoch[ fuCurrDpbIdx ] || // For the fake "closing epoch"
          ( 0 == ulEpochNr && gdpbv100::kuEpochCounterSz == fvulCurrentEpoch[ fuCurrDpbIdx ] )
        )
     )
   {
      Int_t iDiff = ulEpochNr;
      iDiff -= fvulCurrentEpoch[ fuCurrDpbIdx ];
      LOG(info) << "CbmMcbm2018UnpackerAlgoTof::ProcessEpoch        => "
                << " Non consecutive epochs for Gdpb " << fuCurrDpbIdx
                << " in TS " << fulCurrentTsIdx
                << " MS " << fuMsIndex
                << " Msg " << uMesgIdx
                << Form( " : old Ep %08llx new Ep %08llx Diff %d ", fvulCurrentEpoch[ fuCurrDpbIdx ], ulEpochNr, iDiff )
                << std::endl;
   } // if epoch neither +1 nor equal nor overflow
*/
/*
   /// Check for epoch cycle, except for the first epoch in MS buffer (2nd message) where this is taken care
   /// by the epoch cycle index (1st message)
   if( 0 < fvulCurrentEpoch[ fuCurrDpbIdx ] && ulEpochNr < fvulCurrentEpoch[ fuCurrDpbIdx ] &&
       1 < uMesgIdx )
   {
      LOG(info) << "CbmMcbm2018UnpackerAlgoTof::ProcessEpoch        => "
                << " New epoch cycle for Gdpb " << fuCurrDpbIdx
                << Form( ": Current cycle 0x%09llX New cycle 0x%09llX", fvulCurrentEpochCycle[fuCurrDpbIdx], fvulCurrentEpochCycle[fuCurrDpbIdx] + 1 )
                << Form( "(old Ep %08llx new Ep %08llx)", fvulCurrentEpoch[ fuCurrDpbIdx ], ulEpochNr )
                << std::endl;
      fvulCurrentEpochCycle[ fuCurrDpbIdx ]++;
   } // if( 0 < fvulCurrentEpoch[ fuCurrDpbIdx ] && ulEpochNr < fvulCurrentEpoch[ fuCurrDpbIdx ] && 1 < uMesgIdx)
*/
   fvulCurrentEpoch[ fuCurrDpbIdx ] = ulEpochNr;
   fvulCurrentEpochFull[ fuCurrDpbIdx ] = ulEpochNr + ( gdpbv100::kuEpochCounterSz + 1 ) * fvulCurrentEpochCycle[ fuCurrDpbIdx ];

/*
   /// Re-align the epoch number of the message in case it will be used later:
   /// We received the epoch after the data instead of the one before!
   if( 0 < ulEpochNr )
      mess.setGdpbEpEpochNb( ulEpochNr - 1 );
      else mess.setGdpbEpEpochNb( gdpbv100::kuEpochCounterSz );
*/
   /// Process the corresponding messages buffer for current gDPB
   ProcessEpSupprBuffer();
}
void CbmMcbm2018UnpackerAlgoTof::ProcessEndOfMsEpoch()
{
   /// Used to fake a "closing" epoch at the end of the MS
   ULong64_t ulEpochNr = (fvulCurrentEpoch[ fuCurrDpbIdx ] + 1) % gdpbv100::kuEpochCounterSz;

   if( 0 < fvulCurrentEpoch[ fuCurrDpbIdx ] && ulEpochNr < fvulCurrentEpoch[ fuCurrDpbIdx ] )
   {
      LOG(info) << "CbmMcbm2018UnpackerAlgoTof::ProcessEndOfMsEpoch => "
                << " New epoch cycle for Gdpb " << fuCurrDpbIdx
                << Form( ": Current cycle 0x%09llX New cycle 0x%09llX", fvulCurrentEpochCycle[fuCurrDpbIdx], fvulCurrentEpochCycle[fuCurrDpbIdx] + 1 )
                << Form( "(old Ep %08llx new Ep %08llx)", fvulCurrentEpoch[ fuCurrDpbIdx ], ulEpochNr )
                ;
      fvulCurrentEpochCycle[ fuCurrDpbIdx ]++;
   } // if( 0 < fvulCurrentEpoch[ fuCurrDpbIdx ] && ulEpochNr < fvulCurrentEpoch[ fuCurrDpbIdx ] )
/*
   fvulCurrentEpoch[ fuCurrDpbIdx ] = ulEpochNr;
   fvulCurrentEpochFull[ fuCurrDpbIdx ] = ulEpochNr + ( gdpbv100::kuEpochCounterSz + 1 ) * fvulCurrentEpochCycle[ fuCurrDpbIdx ];
*/
   fvulCurrentEpochFull[ fuCurrDpbIdx ] = ulEpochNr + ( gdpbv100::kuEpochCounterSz + 1 ) * fvulCurrentEpochCycle[ fuCurrDpbIdx ];

   /// Process the corresponding messages buffer for current gDPB
   ProcessEpSupprBuffer();
}
void CbmMcbm2018UnpackerAlgoTof::ProcessStarTrigger( const gdpbv100::Message & mess)
{
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

  switch( iMsgIndex )
  {
      case 0:
         fvulGdpbTsMsb[fuCurrDpbIdx] = mess.getGdpbTsMsbStarA();
         break;
      case 1:
         fvulGdpbTsLsb[fuCurrDpbIdx] = mess.getGdpbTsLsbStarB();
         fvulStarTsMsb[fuCurrDpbIdx] = mess.getStarTsMsbStarB();
         break;
      case 2:
         fvulStarTsMid[fuCurrDpbIdx] = mess.getStarTsMidStarC();
         break;
      case 3:
      {
         ULong64_t ulNewGdpbTsFull = ( fvulGdpbTsMsb[fuCurrDpbIdx] << 24 )
                                   + ( fvulGdpbTsLsb[fuCurrDpbIdx]       );
         ULong64_t ulNewStarTsFull = ( fvulStarTsMsb[fuCurrDpbIdx] << 48 )
                                   + ( fvulStarTsMid[fuCurrDpbIdx] <<  8 )
                                   + mess.getStarTsLsbStarD();
         UInt_t uNewToken  = mess.getStarTokenStarD();
         UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
         UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();

         if( ( uNewToken == fvuStarTokenLast[fuCurrDpbIdx] ) && ( ulNewGdpbTsFull == fvulGdpbTsFullLast[fuCurrDpbIdx] ) &&
             ( ulNewStarTsFull == fvulStarTsFullLast[fuCurrDpbIdx] ) && ( uNewDaqCmd == fvuStarDaqCmdLast[fuCurrDpbIdx] ) &&
             ( uNewTrigCmd == fvuStarTrigCmdLast[fuCurrDpbIdx] ) )
         {
            UInt_t uTrigWord =  ( (fvuStarTrigCmdLast[fuCurrDpbIdx] & 0x00F) << 16 )
                     + ( (fvuStarDaqCmdLast[fuCurrDpbIdx]   & 0x00F) << 12 )
                     + ( (fvuStarTokenLast[fuCurrDpbIdx]    & 0xFFF)       );
            LOG(warning) << "Possible error: identical STAR tokens found twice in a row => ignore 2nd! "
                         << " TS " << fulCurrentTsIdx
                         << " gDBB #" << fuCurrDpbIdx << " "
                         << Form("token = %5u ", fvuStarTokenLast[fuCurrDpbIdx] )
                         << Form("gDPB ts  = %12llu ", fvulGdpbTsFullLast[fuCurrDpbIdx] )
                         << Form("STAR ts = %12llu ", fvulStarTsFullLast[fuCurrDpbIdx] )
                         << Form("DAQ cmd = %2u ", fvuStarDaqCmdLast[fuCurrDpbIdx] )
                         << Form("TRG cmd = %2u ", fvuStarTrigCmdLast[fuCurrDpbIdx] )
                         << Form("TRG Wrd = %5x ", uTrigWord );
            return;
         } // if exactly same message repeated

         // GDPB TS counter reset detection
         if( ulNewGdpbTsFull < fvulGdpbTsFullLast[fuCurrDpbIdx] )
            LOG(debug) << "Probable reset of the GDPB TS: old = " << Form("%16llu", fvulGdpbTsFullLast[fuCurrDpbIdx])
                       << " new = " << Form("%16llu", ulNewGdpbTsFull)
                       << " Diff = -" << Form("%8llu", fvulGdpbTsFullLast[fuCurrDpbIdx] - ulNewGdpbTsFull)
                       << " GDPB #" << Form( "%2u", fuCurrDpbIdx);

         // STAR TS counter reset detection
         if( ulNewStarTsFull < fvulStarTsFullLast[fuCurrDpbIdx] )
            LOG(debug) << "Probable reset of the STAR TS: old = " << Form("%16llu", fvulStarTsFullLast[fuCurrDpbIdx])
                       << " new = " << Form("%16llu", ulNewStarTsFull)
                       << " Diff = -" << Form("%8llu", fvulStarTsFullLast[fuCurrDpbIdx] - ulNewStarTsFull)
                       << " GDPB #" << Form( "%2u", fuCurrDpbIdx);

         /// Check needed to avoid double counting
         if( fulCurrentMsIdx < fuNbCoreMsPerTs )
         {
            fvulGdpbTsFullLast[fuCurrDpbIdx] = ulNewGdpbTsFull;
            fvulStarTsFullLast[fuCurrDpbIdx] = ulNewStarTsFull;
            fvuStarTokenLast[fuCurrDpbIdx]   = uNewToken;
            fvuStarDaqCmdLast[fuCurrDpbIdx]  = uNewDaqCmd;
            fvuStarTrigCmdLast[fuCurrDpbIdx] = uNewTrigCmd;
         } // if( fuCurrentMs < fuNbCoreMsPerTs )

         /// FIXME: for now do nothing with it!
/*
         Double_t dTot = 1.;
         Double_t dTime = fulGdpbTsFullLast * 6.25;
         if( 0. == fdFirstDigiTimeDif && 0. != fdLastDigiTime )
         {
            fdFirstDigiTimeDif = dTime - fdLastDigiTime;
            LOG(info) << "Reference fake digi time shift initialized to " << fdFirstDigiTimeDif;
         } // if( 0. == fdFirstDigiTimeDif && 0. != fdLastDigiTime )

	 //         dTime -= fdFirstDigiTimeDif;

         LOG(debug) << "Insert fake digi with time " << dTime << ", Tot " << dTot;
         fhRawTRefDig0->Fill( dTime - fdLastDigiTime);
         fhRawTRefDig1->Fill( dTime - fdLastDigiTime);

         fDigi = new CbmTofDigiExp(0x00005006, dTime, dTot); // fake start counter signal
         fBuffer->InsertData<CbmTofDigi>(fDigi);
*/
         break;
	  } // case 3
      default:
         LOG(error) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  } // switch( iMsgIndex )
}
// -------------------------------------------------------------------------
void CbmMcbm2018UnpackerAlgoTof::ProcessEpSupprBuffer()
{
   Int_t iBufferSize = fvvmEpSupprBuffer[ fuCurrDpbIdx ].size();

   if( 0 == iBufferSize )
      return;

   LOG(debug) << "Now processing stored messages for for gDPB " << fuCurrDpbIdx << " with epoch number "
              << (fvulCurrentEpoch[fuCurrDpbIdx] - 1);

   /// Data are sorted between epochs, not inside => Epoch level ordering
   /// Sorting at lower bin precision level
//   std::stable_sort( fvvmEpSupprBuffer[ fuCurrDpbIdx ].begin(), fvvmEpSupprBuffer[ fuCurrDpbIdx ].end() );

   /// Compute original epoch index before epoch suppression
   ULong64_t ulCurEpochGdpbGet4 = fvulCurrentEpochFull[ fuCurrDpbIdx ];

   /// Ignore the first epoch as it should never appear (start delay!!)
   if( 0 == ulCurEpochGdpbGet4 )
      return;

   /// In Ep. Suppr. Mode, receive following epoch instead of previous
   ulCurEpochGdpbGet4 --;

   Int_t messageType = -111;
   for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
   {
      messageType = fvvmEpSupprBuffer[ fuCurrDpbIdx ][ iMsgIdx ].getMessageType();

      fuGet4Id = fUnpackPar->ElinkIdxToGet4Idx( fvvmEpSupprBuffer[ fuCurrDpbIdx ][ iMsgIdx ].getGdpbGenChipId() );
      if( fuDiamondDpbIdx == fuCurrDpbIdx || 0x90 == fuCurrentMsSysId )
         fuGet4Id = fvvmEpSupprBuffer[ fuCurrDpbIdx ][ iMsgIdx ].getGdpbGenChipId();
      fuGet4Nr = (fuCurrDpbIdx * fuNrOfGet4PerGdpb) + fuGet4Id;

      /// Store the full message in the proper buffer
      gdpbv100::FullMessage fullMess( fvvmEpSupprBuffer[ fuCurrDpbIdx ][ iMsgIdx ], ulCurEpochGdpbGet4 );

      /// Do other actions on it if needed
      switch( messageType )
      {
         case gdpbv100::MSG_HIT:
         {
            ProcessHit( fullMess );
            break;
         } // case gdpbv100::MSG_HIT:
         case gdpbv100::MSG_SLOWC:
         {
            ProcessSlCtrl( fullMess );
            break;
         } // case gdpbv100::MSG_SLOWC:
         case gdpbv100::MSG_SYST:
         {
            ProcessSysMess( fullMess );
            break;
         } // case gdpbv100::MSG_SYST:
         case gdpbv100::MSG_EPOCH:
         case gdpbv100::MSG_STAR_TRI_A:
         case gdpbv100::MSG_STAR_TRI_B:
         case gdpbv100::MSG_STAR_TRI_C:
         case gdpbv100::MSG_STAR_TRI_D:
            /// Should never appear there
            break;
         default:
            LOG(error) << "Message type " << std::hex
                       << std::setw(2) << static_cast<uint16_t>(messageType)
                       << " not included in Get4 unpacker.";
      } // switch( mess.getMessageType() )
   } // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

   fvvmEpSupprBuffer[ fuCurrDpbIdx ].clear();
}
// -------------------------------------------------------------------------
void CbmMcbm2018UnpackerAlgoTof::ProcessHit( const gdpbv100::FullMessage & mess )
{
   UInt_t uChannel = mess.getGdpbHitChanId();
   UInt_t uTot     = mess.getGdpbHit32Tot();

   // In 32b mode the coarse counter is already computed back to 112 FTS bins
   // => need to hide its contribution from the Finetime
   // => FTS = Fullt TS modulo 112
//   UInt_t uFts     = mess.getGdpbHitFullTs() % 112;

   UInt_t uChannelNr         = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
   UInt_t uChannelNrInFee    = (fuGet4Id % fuNrOfGet4PerFee) * fuNrOfChannelsPerGet4 + uChannel;
   UInt_t uFeeNr             = (fuGet4Id / fuNrOfGet4PerFee);
   UInt_t uFeeNrInSys        = fuCurrDpbIdx * fuNrOfFeePerGdpb + uFeeNr;
   UInt_t uRemappedChannelNr = uFeeNr * fuNrOfChannelsPerFee + fUnpackPar->Get4ChanToPadiChan( uChannelNrInFee );
//   UInt_t uGbtxNr            = (uFeeNr / fUnpackPar->GetNrOfFeePerGbtx());
//   UInt_t uFeeInGbtx         = (uFeeNr % fUnpackPar->GetNrOfFeePerGbtx());
//   UInt_t uGbtxNrInSys       = fuCurrDpbIdx * fUnpackPar->GetNrOfGbtxPerGdpb() + uGbtxNr;

//   UInt_t uChanInSyst = fuCurrDpbIdx * fuNrOfChannelsPerGdpb + uChannelNr;
   UInt_t uRemappedChannelNrInSys = fuCurrDpbIdx * fuNrOfChannelsPerGdpb
                                   + uFeeNr * fuNrOfChannelsPerFee
                                   + fUnpackPar->Get4ChanToPadiChan( uChannelNrInFee );
   /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
   if( fuDiamondDpbIdx == fuCurrDpbIdx || 0x90 == fuCurrentMsSysId )
   {
      uRemappedChannelNr      = uChannelNr;
      uRemappedChannelNrInSys = fuCurrDpbIdx * fUnpackPar->GetNrOfChannelsPerGdpb() + uChannelNr;
   } // if( fuDiamondDpbIdx == fuCurrDpbIdx || 0x90 == fuCurrentMsSysId )

//   ULong_t  ulHitTime = mess.getMsgFullTime(  mess.getExtendedEpoch() );
   Double_t dHitTime  = mess.GetFullTimeNs();
   Double_t dHitTot   = uTot;     // in bins

   // Histograms filling
   if( kTRUE == fbMonitorMode )
   {
      fhRawTotCh[ fuCurrDpbIdx ]->Fill( uRemappedChannelNr, dHitTot);
      fhChCount[ fuCurrDpbIdx ] ->Fill( uChannelNr );
      fhChCountRemap[ fuCurrDpbIdx ] ->Fill( uRemappedChannelNr );
   } // if( kTRUE == fbMonitorMode )

/*
   if( fUnpackPar->GetNumberOfChannels() < uChanInSyst )
   {
      LOG(fatal) << "Invalid mapping index " << uChanInSyst
                 << " VS " << fUnpackPar->GetNumberOfChannels()
                 <<", from " << fuCurrDpbIdx
                 <<", " << fuGet4Id
                 <<", " << uChannel;
      return;
   } // if( fUnpackPar->GetNumberOfChannels() < uChanUId )
*/
   if( fviRpcChUId.size() < uRemappedChannelNrInSys )
   {
      LOG(fatal) << "Invalid mapping index " << uRemappedChannelNrInSys
                 << " VS " << fviRpcChUId.size()
                 << ", from GdpbNr " << fuCurrDpbIdx
                 << ", Get4 " << fuGet4Id
                 << ", Ch " << uChannel
                 << ", ChNr " << uChannelNr
                 << ", ChNrIF " << uChannelNrInFee
                 << ", FiS " << uFeeNrInSys;
      return;
   } // if( fviRpcChUId.size() < uRemappedChannelNrInSys )

   UInt_t uChanUId =  fviRpcChUId[ uRemappedChannelNrInSys ];
/*
   if( 5 == fviRpcType[uGbtxNrInSys] )
      LOG(info) << "T0 mapping index " << uRemappedChannelNrInSys
                 << " UID " << std::hex << std::setw(8) << uChanUId << std::dec
                 << ", from GdpbNr " << fuCurrDpbIdx
                 << ", Get4 " << fuGet4Id
                 << ", Ch " << uChannel
                 << ", ChNr " << uChannelNr
                 << ", ChNrIF " << uChannelNrInFee
                 << ", FiS " << uFeeNrInSys
                 << ", GBTx " << uGbtxNrInSys;
*/
/*
   if( 0 == uChanUId ) {
     if( 0 < NMappingWarnings-- )
     LOG(warning) << "Unused data item at " << uRemappedChannelNrInSys
		  << ", from GdpbNr " << fuCurrDpbIdx
		  << ", Get4 " << fuGet4Id
		  << ", Ch " << uChannel
		  << ", ChNr " << uChannelNr
		  << ", ChNrIF " << uChannelNrInFee
		  << ", FiS " << uFeeNrInSys
		  ;
      return;   // Hit not mapped to digi
   }
*/
   /// Apply offset to T0 only to TOF digis
   if( 0x90 != fuCurrentMsSysId )
      dHitTime -= fdTimeOffsetNs;

   LOG(debug) << Form("Insert 0x%08x digi with time ", uChanUId ) << dHitTime << Form(", Tot %4.0f",dHitTot)
//              << " into buffer with " << fBuffer->GetSize() << " data from "
//              << Form("%11.1f to %11.1f ", fBuffer->GetTimeFirst(), fBuffer->GetTimeLast())

              << " at epoch " << mess.getExtendedEpoch();

//   CbmTofDigi digi( uChanUId, dHitTime, dHitTot );
//   fDigiVect.emplace_back( digi );
   fDigiVect.emplace_back( uChanUId, dHitTime, dHitTot );

}
// -------------------------------------------------------------------------
void CbmMcbm2018UnpackerAlgoTof::ProcessSlCtrl( const gdpbv100::FullMessage & /*mess*/ )
{
}
// -------------------------------------------------------------------------
void CbmMcbm2018UnpackerAlgoTof::ProcessSysMess( const gdpbv100::FullMessage & mess )
{
   switch( mess.getGdpbSysSubType() )
   {
      case gdpbv100::SYS_GET4_ERROR:
      {
         ProcessError( mess );
         break;
      } // case gdpbv100::SYSMSG_GET4_EVENT
      case gdpbv100::SYS_GDPB_UNKWN:
      {
         LOG(debug) << "Unknown GET4 message, data: " << std::hex << std::setw(8)
                    << mess.getGdpbSysUnkwData() << std::dec
                    <<" Full message: " << std::hex << std::setw(16)
                    << mess.getData() << std::dec;
         break;
      } // case gdpbv100::SYS_GDPB_UNKWN:
      case gdpbv100::SYS_GET4_SYNC_MISS:
      {
         if( mess.getGdpbSysFwErrResync() )
            LOG(info) << Form( "GET4 Resynchronization: Get4:0x%04x ", mess.getGdpbGenChipId() ) << fuCurrDpbIdx;
            else LOG(info) << "GET4 synchronization pulse missing in gDPB " << fuCurrDpbIdx;
         break;
      } // case gdpbv100::SYS_GET4_SYNC_MISS:
      case gdpbv100::SYS_PATTERN:
      {
         ProcessPattern( mess );
         break;
      } // case gdpbv100::SYS_PATTERN:
      default:
      {
         LOG(info) << "Crazy system message, subtype " << mess.getGdpbSysSubType();
         break;
      } // default
   } // switch( mess.getGdpbSysSubType() )
}
void CbmMcbm2018UnpackerAlgoTof::ProcessError( const gdpbv100::FullMessage & mess )
{
   uint32_t uErrorType = mess.getGdpbSysErrData();

   switch( uErrorType )
   {
      case gdpbv100::GET4_V2X_ERR_READ_INIT:
      case gdpbv100::GET4_V2X_ERR_SYNC:
      case gdpbv100::GET4_V2X_ERR_EP_CNT_SYNC:
      case gdpbv100::GET4_V2X_ERR_EP:
      case gdpbv100::GET4_V2X_ERR_FIFO_WRITE:
      case gdpbv100::GET4_V2X_ERR_CHAN_STATE:
      case gdpbv100::GET4_V2X_ERR_TOK_RING_ST:
      case gdpbv100::GET4_V2X_ERR_TOKEN:
      case gdpbv100::GET4_V2X_ERR_READOUT_ERR:
      case gdpbv100::GET4_V2X_ERR_DLL_LOCK:
      case gdpbv100::GET4_V2X_ERR_DLL_RESET:
         /// Critical errors
         break;
      case gdpbv100::GET4_V2X_ERR_SPI:
         /// Error during SPI communication with slave (e.g. PADI)
         break;
      case gdpbv100::GET4_V2X_ERR_LOST_EVT:
      case gdpbv100::GET4_V2X_ERR_TOT_OVERWRT:
      case gdpbv100::GET4_V2X_ERR_TOT_RANGE:
      case gdpbv100::GET4_V2X_ERR_EVT_DISCARD:
      case gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG:
      case gdpbv100::GET4_V2X_ERR_UNPAIR_FALL:
      case gdpbv100::GET4_V2X_ERR_SEQUENCE_ER:
         /// Input channel realted errors (TOT, shaky signals, etc...)
         break;
      case gdpbv100::GET4_V2X_ERR_EPOCH_OVERF:
         break;
      case gdpbv100::GET4_V2X_ERR_UNKNOWN:
         /// Unrecognised error code from GET4
         break;
      default:
         /// Corrupt error or not yet supported error
         break;
   } // switch( uErrorType )

   return;
}

void CbmMcbm2018UnpackerAlgoTof::ProcessPattern( const gdpbv100::FullMessage & mess )
{
   uint16_t usType   = mess.getGdpbSysPattType();
   uint16_t usIndex  = mess.getGdpbSysPattIndex();
   uint32_t uPattern = mess.getGdpbSysPattPattern();

   switch( usType )
   {
      case gdpbv100::PATT_MISSMATCH:
      {
         LOG(debug) << Form( "Missmatch pattern message => Type %u, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern );
/*
	if(usIndex==7) {
	  TString Tok;
	  if (bEnableOut) Tok="Ena";
	  else  Tok="Dis";
	  LOG(DEBUG) << Form( "Mismatch pat in TS %llu, MS %llu, Gdpb %u, T %u, Pattern 0x%08X %08X %08X %08X %08X %08X %08X %08X ",
			     fulCurrentTsIdx, fulCurrentMsIdx, fuCurrDpbIdx,  usType,
			     pat_mess[0],  pat_mess[1],  pat_mess[2],  pat_mess[3],  pat_mess[4],  pat_mess[5],  pat_mess[6],  pat_mess[7]  )
		    << Tok;
	}
	break;
*/
      } // case gdpbv100::PATT_MISSMATCH:

      case gdpbv100::PATT_ENABLE:
      {
         LOG(debug2) << Form( "Enable pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern );
/*
         for( UInt_t uBit = 0; uBit < 32; ++uBit )
            if( ( uPattern >> uBit ) & 0x1 )
            {
               fhPatternEnable->Fill( 32 * usIndex + uBit, fuCurrDpbIdx );
               fvhGdpbPatternEnableEvo[ fuCurrDpbIdx ]->Fill( fulCurrentTsIndex, 32 * usIndex + uBit );
            } // if( ( uPattern >> uBit ) & 0x1 )
*/
         break;
      } // case gdpbv100::PATT_ENABLE:

      case gdpbv100::PATT_RESYNC:
      {
         LOG(debug) << Form( "RESYNC pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern );

         break;
      } // case gdpbv100::PATT_RESYNC:
      default:
      {
         LOG(debug) << "Crazy pattern message, subtype " << usType;
         break;
      } // default
   } // switch( usType )
/*
   if(usIndex==7) {
     bEnableOut=kTRUE;
//     for(Int_t iGdpb=0; iGdpb<fUnpackPar->GetNrOfGdpbs(); iGdpb++) {
//       bEnableOut &= bGdpbOK[iGdpb];
//     }
   }
*/
   return;
}
// -------------------------------------------------------------------------

Bool_t CbmMcbm2018UnpackerAlgoTof::CreateHistograms()
{
   std::string sFolder = "Tof_Raw_gDPB";

   LOG(info) << "create Histos for " << fuNrOfGdpbs <<" gDPBs ";

   fhRawTDigEvT0 = new TH1F( Form("Raw_TDig-EvT0"),
                             Form("Raw digi time difference to 1st digi ; time [ns]; cts"),
                             500, 0, 100.);

   fhRawTDigRef0 = new TH1F( Form("Raw_TDig-Ref0"),
                             Form("Raw digi time difference to Ref ; time [ns]; cts"),
                             6000, -10000, 50000);

   fhRawTDigRef = new TH1F( Form("Raw_TDig-Ref"),
                            Form("Raw digi time difference to Ref ; time [ns]; cts"),
                            6000, -1000, 5000);

   fhRawTRefDig0 = new TH1F( Form("Raw_TRef-Dig0"),
                             Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
                             9999, -50000, 50000);

   fhRawTRefDig1 = new TH1F( Form("Raw_TRef-Dig1"),
                             Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
                             9999, -5000, 5000);

   fhRawDigiLastDigi = new TH1F( Form("Raw_Digi-LastDigi"),
                                 Form("Raw Digi time difference to last digi  ; time [ns]; cts"),
                                 9999, -5000, 5000);

   /// Add pointers to the vector with all histo for access by steering class
   AddHistoToVector( fhRawTDigEvT0, sFolder );
   AddHistoToVector( fhRawTDigRef0, sFolder );
   AddHistoToVector( fhRawTDigRef, sFolder );
   AddHistoToVector( fhRawTRefDig0, sFolder );
   AddHistoToVector( fhRawTRefDig1, sFolder );
   AddHistoToVector( fhRawDigiLastDigi, sFolder );

   fhRawTotCh.resize( fuNrOfGdpbs );
   fhChCount.resize(  fuNrOfGdpbs );
   fhChCountRemap.resize(  fuNrOfGdpbs );
   fhChanCoinc.resize( fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      fhRawTotCh[ uGdpb ] = new TH2F( Form("Raw_Tot_gDPB_%02u", uGdpb),
                                      Form("Raw TOT gDPB %02u; channel; TOT [bin]", uGdpb),
                                      fuNrOfGet4PerGdpb*4, 0., fuNrOfGet4PerGdpb*4,
                                      256, 0., 256. );

      fhChCount[ uGdpb ] = new TH1I( Form("ChCount_gDPB_%02u", uGdpb),
                                     Form("Channel counts gDPB %02u; channel; Hits", uGdpb),
                                     fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb );

      fhChCountRemap[ uGdpb ] = new TH1I( Form("ChCountRemap_gDPB_%02u", uGdpb),
                                     Form("Remapped channel counts gDPB %02u; Remapped channel; Hits", uGdpb),
                                     fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb );

      fhChanCoinc[ uGdpb ] = new TH2F( Form("fhChanCoinc_%02u", uGdpb),
                                      Form("Channels Coincidence %02u; Left; Right", uGdpb),
                                      fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb,
                                      fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb );

      /// Add pointers to the vector with all histo for access by steering class
      AddHistoToVector( fhRawTotCh[ uGdpb ], sFolder );
      AddHistoToVector( fhChCount[ uGdpb ], sFolder );
      AddHistoToVector( fhChCountRemap[ uGdpb ], sFolder );
      AddHistoToVector( fhChanCoinc[ uGdpb ], sFolder );
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)

   fhVectorSize = new TH1I( "fhVectorSize", "Size of the vector VS TS index; TS index; Size [bytes]",
                            10000, 0., 10000. );
   fhVectorCapacity = new TH1I( "fhVectorCapacity", "Size of the vector VS TS index; TS index; Size [bytes]",
                            10000, 0., 10000. );
   AddHistoToVector( fhVectorSize, sFolder );
   AddHistoToVector( fhVectorCapacity, sFolder );

/*
   /// Canvases
   Double_t w = 10;
   Double_t h = 10;

      /// Event building process summary and statistics
   fcEventBuildStats = new TCanvas( "cEvtBuildStats", "Event building statistics", w, h);
   if( kTRUE == fbDebugMonitorMode )
      fcEventBuildStats->Divide( 2, 3 );
      else fcEventBuildStats->Divide( 2, 2 );

   fcEventBuildStats->cd( 1 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   fhEventNbPerTs->Draw();

   fcEventBuildStats->cd( 2 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   fhEventSizeDistribution->Draw();

   fcEventBuildStats->cd( 3 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   fhEventSizeEvolution->Draw();

   fcEventBuildStats->cd( 4 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   fhEventNbEvolution->Draw();

   if( kTRUE == fbDebugMonitorMode )
   {
      fcEventBuildStats->cd( 5 );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fhEventNbDistributionInTs->Draw();

      fcEventBuildStats->cd( 6 );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fhEventSizeDistributionInTs->Draw();
   } // if( kTRUE == fbDebugMonitorMode )

   AddCanvasToVector( fcEventBuildStats, "canvases" );
*/
   return kTRUE;
}
Bool_t CbmMcbm2018UnpackerAlgoTof::FillHistograms()
{
/*
   UInt_t uNbEvents = fvEventsBuffer.size();
   fhEventNbPerTs->Fill( uNbEvents );

   for( UInt_t uEvent = 0; uEvent < uNbEvents; ++uEvent )
   {
      UInt_t uEventSize       = fvEventsBuffer[ uEvent ].GetEventSize();
      Double_t dEventTimeSec  = fvEventsBuffer[ uEvent ].GetEventTimeSec();
      Double_t dEventTimeMin  = dEventTimeSec / 60.0;

      fhEventSizeDistribution->Fill( uEventSize );
      fhEventSizeEvolution->Fill( dEventTimeMin, uEventSize );
      fhEventNbEvolution->Fill( dEventTimeMin );

      if( kTRUE == fbDebugMonitorMode )
      {
         Double_t dEventTimeInTs = ( fvEventsBuffer[ uEvent ].GetTrigger().GetFullGdpbTs() * gdpbv100::kdClockCycleSizeNs
                                    - fdTsStartTime ) / 1000.0;

         fhEventNbDistributionInTs->Fill( dEventTimeInTs  );
         fhEventSizeDistributionInTs->Fill( dEventTimeInTs, uEventSize );
      } // if( kTRUE == fbDebugMonitorMode )
   } // for( UInt_t uEvent = 0; uEvent < uNbEvents; ++uEvent )
*/
   return kTRUE;
}
Bool_t CbmMcbm2018UnpackerAlgoTof::ResetHistograms()
{
/*
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      fvhHitsTimeToTriggerRaw[ uGdpb ]->Reset();
      fvhHitsTimeToTriggerSel[ uGdpb ]->Reset();

      if( kTRUE == fbDebugMonitorMode )
      {
         fvhHitsTimeToTriggerSelVsDaq[ uGdpb ]->Reset();
         fvhHitsTimeToTriggerSelVsTrig[ uGdpb ]->Reset();
         fvhTriggerDt[ uGdpb ]->Reset();
         fvhTriggerDistributionInTs[ uGdpb ]->Reset();
         fvhTriggerDistributionInMs[ uGdpb ]->Reset();
         fvhMessDistributionInMs[ uGdpb ]->Reset();
      } // if( kTRUE == fbDebugMonitorMode )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

   /// Create event builder related histograms
   fhEventNbPerTs->Reset();
   fhEventSizeDistribution->Reset();
   fhEventSizeEvolution->Reset();
   fhEventNbEvolution->Reset();

   if( kTRUE == fbDebugMonitorMode )
   {
      fhEventNbDistributionInTs->Reset();
      fhEventSizeDistributionInTs->Reset();
      fhRawTriggersStats->Reset();
      fhMissingTriggersEvolution->Reset();
   } // if( kTRUE == fbDebugMonitorMode )
*/
   return kTRUE;
}
// -------------------------------------------------------------------------
