// ------------------------------------------------------------------
// -----                     TTofVftxUnpacker                   -----
// -----              Created 14/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#include "TTofVftxUnpacker.h"

// TOF headers
#include "TofDef.h"
#include "TMbsUnpackTofPar.h"
#include "TofTdcDef.h"
#include "TofVftxDef.h"
#include "TTofVftxData.h"
#include "TTofVftxBoard.h"

// FAIR headers
#include "FairRootManager.h"
#include "FairLogger.h"
#include "TString.h"

// ROOT headers
#include "TClonesArray.h"
#include "TH1.h"
#include "TROOT.h"
#include "TDirectory.h"

TTofVftxUnpacker::TTofVftxUnpacker():
   fParUnpack(0),
   fuNbTdc(0),
   fVftxBoardCollection(NULL),
   fiLastFpgaTdcCoarse(),
   fh1VftxRawChMap(),
   fh1VftxChFt()
{
}
TTofVftxUnpacker::TTofVftxUnpacker( TMbsUnpackTofPar * parIn ):
   fParUnpack( parIn ),
   fuNbTdc( parIn->GetNbActiveBoards( tofMbs::vftx ) ),
   fVftxBoardCollection(NULL),
   fiLastFpgaTdcCoarse(),
   fh1VftxRawChMap(),
   fh1VftxChFt()
{
   // Recover first the VFTX board objects created in general unpacker class
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   fVftxBoardCollection = (TClonesArray*) rootMgr->GetObject("TofVftxTdc");
   if(NULL == fVftxBoardCollection) 
   {
      LOG(warn)<<"TTofVftxUnpacker::TTofVftxUnpacker : no TOF VFTX TDC array! ";
      fuNbTdc = 0;
   }
    
   if( 0 < fuNbTdc )
      fiLastFpgaTdcCoarse.resize( fuNbTdc * vftxtdc::kuNbChan, 0);
}
TTofVftxUnpacker::~TTofVftxUnpacker()
{
   // not sure if it will not make problems for seeing them
//   DeleteHistos();
}

void TTofVftxUnpacker::Clear(Option_t */*option*/)
{
   fParUnpack = NULL;
   fuNbTdc    = 0;
   fVftxBoardCollection = NULL;
   fiLastFpgaTdcCoarse.clear();
}

void TTofVftxUnpacker::ProcessVFTX( Int_t iTdcIndex, UInt_t* pMbsData, UInt_t uLength )
{
//   if( (iTdcIndex<0) || (fuNbTdc <= iTdcIndex) || 
//       (fVftxBoardCollection->GetEntriesFast() <= iTdcIndex )) 
   if( (iTdcIndex<0) || (fuNbTdc <= static_cast<UInt_t>(iTdcIndex)) ) 
   {
      LOG(error)<<"Error VFTX number "<<iTdcIndex<<" out of bound (max "<<fuNbTdc<<") ";
      return;
   }
   
   TTofVftxBoard * fVftxBoard = (TTofVftxBoard*) fVftxBoardCollection->ConstructedAt(iTdcIndex);

   for(UInt_t ch=0; ch < vftxtdc::kuNbChan; ch++)
      fiLastFpgaTdcCoarse[iTdcIndex*vftxtdc::kuNbChan + ch] = -1;

   UInt_t l_dat = *pMbsData++; // module header
   if ( (l_dat & vftxtdc::kiHeaderKeyMask)>>vftxtdc::kiHeaderKeyShift != static_cast<UInt_t>(vftxtdc::kiHeaderKeyword) ) 
   {
      LOG(warn)<<"This is not a vftx #"<< fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx)
                  <<" header, jumping this sub-event...";
      return;
   } // if ( (l_dat & vftxtdc::kiHeaderKeyMask)>>vftxtdc::kiHeaderKeyShift != vftxtdc::kiHeaderKeyword ) 
   
   UInt_t uMbsNbData = (l_dat & vftxtdc::kiHeaderNbMask);
   if (uMbsNbData+2 != uLength && uMbsNbData+2 + 256 != uLength)
   {
      LOG(warn)<<"Wrong length in vftx #"<<fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx)
               <<" header, jumping this sub-event... "<<uMbsNbData+2<<" vs "<<uLength;
     // Return commented: MBS vftxlib.c for gsi aug 12 not compatible with too many data in event: 
     // only 8 bit where used to code event size => effective limitation to 256 hits then loop
      //return;
   } // if (uMbsNbData+2 != uLength && uMbsNbData+2 + 256 != uLength)
   
/*   UInt_t uModIndex  = ((l_dat & vftxtdc::kiHeaderModMask) >> vftxtdc::kiHeaderModShift); */
/*
   // Not true when first VFTX are disabled!!!
   if( uModIndex != fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx) )
      LOG(warn)<<"Wrong VFTX index in header of vftx #"<<fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx)
                  <<": "<<uModIndex;
*/
   
   l_dat = *pMbsData++;  // vulom tdc fifo header
   UInt_t uFifoNbData = 0;
   if( l_dat & vftxtdc::kuFifoMessageType ) 
   {
      // Found proper TDC FIFO header => unpack it
      fVftxBoard->SetTriggerType( (l_dat & vftxtdc::kiFifoHeadTrigType) >> vftxtdc::kiFifoHeadTrigTypeShift );
      fVftxBoard->SetTriggerTime( (l_dat & vftxtdc::kiFifoHeadTrigTime) >> vftxtdc::kiFifoHeadTrigTimeShift );
      uFifoNbData = (l_dat & vftxtdc::kiFifoHeadDataCnt)  >> vftxtdc::kiFifoHeadDataCntShift;
   } // if( l_dat & vftxtdc::kuFifoMessageType ) 
      else LOG(warn)<<"Vftx #"<<fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx)<<" fifo header missing... ";
   
   if (uMbsNbData != uFifoNbData)
   {
   // uMbsNbData is always 0xAA in current versions of VFTX!!!    
/*
      LOG(warn)<<"Wrong length in vftx #"<<fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx)
                  <<" FIFO header compared to its MBS header, jumping this sub-event... "
                  <<uFifoNbData<<" vs "<<uMbsNbData;
      return;
*/
   } // if (uMbsNbData-2 != uFifoNbData)
   
   while (uMbsNbData-- > 0) 
   {
      UInt_t l_da0 = *pMbsData++;

      TTofVftxData hit;
      if ((l_da0 & vftxtdc::kuFifoMessageType) == vftxtdc::kuFifoMessageType) 
      {
         TString sTemp = Form("Wrong data item in vftx #%d: type %d (message %08x, header %08x), jumping this sub-event...", 
                       fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx), 
                       (l_da0 & vftxtdc::kuFifoMessageType) >> vftxtdc::kiFifoMessageTypeShift, l_da0, l_dat);
         LOG(warn)<<sTemp;
         sTemp = Form("Message: tt %01u ct %5u nd %3u", 
                      (l_da0 & vftxtdc::kiFifoHeadTrigType) >> vftxtdc::kiFifoHeadTrigTypeShift,
                      (l_da0 & vftxtdc::kiFifoHeadTrigTime) >> vftxtdc::kiFifoHeadTrigTimeShift,
                      (l_da0 & vftxtdc::kiFifoHeadDataCnt)  >> vftxtdc::kiFifoHeadDataCntShift );
         LOG(warn)<<sTemp;
         sTemp = Form("Header : tt %01u ct %5u nd %3u", 
                      (l_dat & vftxtdc::kiFifoHeadTrigType) >> vftxtdc::kiFifoHeadTrigTypeShift,
                      (l_dat & vftxtdc::kiFifoHeadTrigTime) >> vftxtdc::kiFifoHeadTrigTimeShift,
                      (l_dat & vftxtdc::kiFifoHeadDataCnt)  >> vftxtdc::kiFifoHeadDataCntShift );
         LOG(warn)<<sTemp;
         return;
      } // if ((l_da0 & vftxtdc::kuFifoMessageType) == vftxtdc::kuFifoMessageType) 
      
      hit.SetChannel( (l_da0 & vftxtdc::kiChannel) >> vftxtdc::kiChannelShift ); //4-5 bit
      if ( hit.GetChannel() > vftxtdc::kuNbChan - 1)
         LOG(warn)<<"Channel number larger than "<<vftxtdc::kuNbChan -1
                     <<" found in vftx #"<<fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx);
         
      hit.SetFutureBit(  (l_da0 & vftxtdc::kiFifoFutureBit)  >> vftxtdc::kiFiFoFutBitShift ); //  1 bit
      hit.SetCoarseTime( (l_da0 & vftxtdc::kiFifoCoarseTime) >> vftxtdc::kiFifoCtShift );     // 13-15 bits
      hit.SetFineTime(   (l_da0 & vftxtdc::kiFifoFineTime)   >> vftxtdc::kiFifoFtShift );     // 10-11 bits 

      if( kTRUE == fParUnpack->IsDebug() )
      {
         if( static_cast<Int_t>( hit.GetCoarseTime() ) - fiLastFpgaTdcCoarse[iTdcIndex*vftxtdc::kuNbChan + hit.GetChannel()] < 6 &&
               -6 < static_cast<Int_t>( hit.GetCoarseTime() ) - fiLastFpgaTdcCoarse[iTdcIndex*vftxtdc::kuNbChan + hit.GetChannel()] &&
               -1 < fiLastFpgaTdcCoarse[iTdcIndex*vftxtdc::kuNbChan + hit.GetChannel()] )
         {
            TString sTemp = Form("Too close hits in vftx #%d channel %d: old coarse %d new coarse %d diff %d ft %3x",
                  fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx), hit.GetChannel(), 
                  fiLastFpgaTdcCoarse[iTdcIndex*vftxtdc::kuNbChan + hit.GetChannel()], hit.GetCoarseTime(),
                  (Int_t)hit.GetCoarseTime() - fiLastFpgaTdcCoarse[iTdcIndex*vftxtdc::kuNbChan + hit.GetChannel()],
                  hit.GetFineTime());
            LOG(warn)<<sTemp;
         } // if hits on same channel closer than channel dead time

         fiLastFpgaTdcCoarse[iTdcIndex*vftxtdc::kuNbChan + hit.GetChannel()] = hit.GetCoarseTime();
      } // if( kTRUE == fParUnpack->IsDebug() )

      fVftxBoard->AddData( hit );
      
   } // while (uMbsNbData-- > 0)

   // Attempt at sorting hits according to their times
   fVftxBoard->SortData();

   LOG(debug2)<<" VFTX TDC #"<<fParUnpack->GetActiveToAllTypeInd(iTdcIndex, tofMbs::vftx)<<" finished";
   return;
} 

// Histogram management
// TODO: maybe add some options to control which histograms are 
// created and filed (memory management)
void TTofVftxUnpacker::CreateHistos()
{
   LOG(debug)<<" TTofVftxUnpacker::CreateHistos Nb boards: "<<fuNbTdc<<" Directory ";
   
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !
   
   TH1* hTemp = 0;
   
   // 2D vector of histograms
   fh1VftxChFt.resize( fuNbTdc );
   for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbTdc; uBoardIndex++)
   {
      // Board specific histograms
      hTemp = new TH1I( Form("tof_%s_ch_%03u", toftdc::ksTdcHistName[ toftdc::vftx ].Data(), uBoardIndex),
                        Form("Counts per TDC channel for vftx #%03u", uBoardIndex),
                        vftxtdc::kuNbChan, 0.0, vftxtdc::kuNbChan );
      fh1VftxRawChMap.push_back( hTemp );
      
      for( UInt_t uChannelIndex = 0; uChannelIndex < vftxtdc::kuNbChan; uChannelIndex++)
      {
         // Channel specific histograms
         hTemp = new TH1I( Form("tof_%s_ft_b%03u_ch%03u", 
                                    toftdc::ksTdcHistName[ toftdc::vftx ].Data(), 
                                    uBoardIndex, uChannelIndex),
                           Form("Counts per Finetime bin for TDC channel %3u on vftx #%03u", 
                                 uChannelIndex, uBoardIndex),
                           vftxtdc::kiFifoFineTime + 1, -0.5, vftxtdc::kiFifoFineTime + 1 - 0.5 );
         (fh1VftxChFt[uBoardIndex]).push_back( hTemp );
         
         LOG(debug)<<" TTofVftxUnpacker::CreateHistos => Create FT histo for "
                                  <<toftdc::ksTdcHistName[ toftdc::vftx ]<<" #"
                                  <<uBoardIndex<<" ch "<<uChannelIndex<<" from the unpack step: 0x"
                                  <<fh1VftxChFt[uBoardIndex][uChannelIndex]<<" "
                                  <<fh1VftxChFt[uBoardIndex][uChannelIndex]->GetEntries();
      } // for( UInt_t uChannelIndex = 0; uChannelIndex < vftxtdc::kuNbChan; uChannelIndex++)
      
   } // for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbTdc; uBoardIndex++)
   
   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
}
void TTofVftxUnpacker::FillHistos()
{
   // loop over TDC boards
   TTofVftxBoard * fVftxBoard;
   for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbTdc; uBoardIndex++)
   { 
      fVftxBoard = (TTofVftxBoard*) fVftxBoardCollection->ConstructedAt(uBoardIndex);
      // Loop over hits data
      for( UInt_t uDataIndex = 0; uDataIndex < fVftxBoard->GetDataNb() ; uDataIndex++ )
      {
         TTofVftxData data = fVftxBoard->GetData( uDataIndex);
         TString sTemp = Form( " TTofVftxUnpacker::FillHistos: Board #%03u Data #%04u Chan %3d CT %7d FT %7d",
                  uBoardIndex, uDataIndex, data.GetChannel(), 
                  data.GetCoarseTime(), data.GetFineTime() );
         LOG(debug)<<sTemp;
         
         fh1VftxRawChMap[uBoardIndex]->Fill(  data.GetChannel() );
         fh1VftxChFt[uBoardIndex][ data.GetChannel() ]->Fill( data.GetFineTime() );
         
      } // for( UInt_t uDataIndex = 0; uDataIndex < fVftxBoard[uBoardIndex]->GetDataNb() ; uDataIndex++ )
   } // for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbTdc; uBoardIndex++)
}
void TTofVftxUnpacker::WriteHistos( TDirectory* inDir)
{
   TDirectory * oldir = gDirectory;
   TDirectory *cdVftxUnp[fuNbTdc];
   
   for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbTdc; uBoardIndex++)
   { 
      cdVftxUnp[uBoardIndex] = inDir->mkdir( Form( "Unp_%s_%03u", toftdc::ksTdcHistName[ toftdc::vftx ].Data(), uBoardIndex) );
      cdVftxUnp[uBoardIndex]->cd();    // make the "Unp_triglog" directory the current directory
      
      fh1VftxRawChMap[uBoardIndex]->Write();
      
      for( UInt_t uChannelIndex = 0; uChannelIndex < vftxtdc::kuNbChan; uChannelIndex++)
      {
         fh1VftxChFt[uBoardIndex][uChannelIndex]->Write();
      } // for( UInt_t uChannelIndex = 0; uChannelIndex < vftxtdc::kuNbChan; uChannelIndex++)
      
   } // for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbTdc; uBoardIndex++)
   
   oldir->cd();
}
void TTofVftxUnpacker::DeleteHistos()
{
   fh1VftxRawChMap.clear();
   for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbTdc; uBoardIndex++)
      (fh1VftxChFt[uBoardIndex]).clear();
   fh1VftxChFt.clear();
}
