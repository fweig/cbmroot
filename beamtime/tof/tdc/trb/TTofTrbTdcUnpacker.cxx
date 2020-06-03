// -----------------------------------------------------------------------------
// ----- TTofTrbTdcUnpacker source file                                    -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-04-03                                 -----
// -----                                                                   -----
// ----- based on TTofVftxUnpacker by P.-A. Loizeau                      -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/tdc/vftx/TTofVftxUnpacker.cxx                 -----
// ----- revision 20754, 2013-07-17                                        -----
// -----------------------------------------------------------------------------

#include "TTofTrbTdcUnpacker.h"

// Parameter header
#include "TMbsUnpackTofPar.h"
#include "TMbsCalibTofPar.h"

// Output data objects
#include "TTofTrbTdcBoard.h"

// Defines
#include "TofDef.h"
#include "TofTdcDef.h"

// TDC iterator
#include "HadaqTdcIterator.h"

// ROOT
#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TROOT.h"
#include "TTree.h"
#include "TBranch.h"

// FairRoot
#include "FairRootManager.h"
#include "FairLogger.h"
#include "FairRuntimeDb.h"
#include "FairRun.h"

ClassImp(TTofTrbTdcUnpacker)

std::map<Int_t,Bool_t> TTofTrbTdcUnpacker::fmRingBufferOverflow;
Int_t TTofTrbTdcUnpacker::fiAcceptedHits(0);
Int_t TTofTrbTdcUnpacker::fiAvailableHits(0);
Bool_t TTofTrbTdcUnpacker::fbInspection(kFALSE);

TTofTrbTdcUnpacker::TTofTrbTdcUnpacker( TMbsUnpackTofPar * parUnpackIn, TMbsCalibTofPar * parCalibIn ):
   fParUnpack( parUnpackIn ),
   fParCalib( parCalibIn ),
   fuNbActiveTrbTdc( parUnpackIn->GetNbActiveBoards( tofMbs::trbtdc ) ),
   fbCalibTrigger(kFALSE), 
   fbJointEdgesChannel( parUnpackIn->IsTrbTdcJointEdgesChannel() ),
   fTrbTdcBoardCollection(0),
   fTrbTdcRingBufferOverflow(NULL),
   fLeadingOnlyShares(),
   fTrailingOnlyShares(),
   fUnequalEdgesShares(),
   fiLeadingOnlyBuffers(),
   fiTrailingOnlyBuffers(),
   fiUnequalEdgesBuffers(),
   fiAllNonEmptyBuffers(),
   fTrbTdcBoardTot(),
   fTrbTdcLeadingPosition(),
   fTrbTdcTrailingPosition(),
   fTrbTdcRefChannelFineTime(NULL),
   fTrbTdcBoardFineTime(),
   fTrbTdcChannelOccupancy(),
//   fTrbTdcChannelFineTimeOvershoot(),
//   fTrbTdcChannelUnprocessedHits(),
   fTrbTdcChannelFineTime(),
//   fTrbTdcChannelCoarseTime(),
   fTrbTdcChannelToT(),
   fuEventIndex(0)
{
   LOG(info)<<"**** TTofTrbTdcUnpacker: Call TTofTrbTdcUnpacker()...";

   // Recover first the TRB-TDC board objects created in TTrbUnpackTof
   FairRootManager* rootMgr = FairRootManager::Instance();
   
   fTrbTdcBoardCollection = (TClonesArray*) rootMgr->GetObject("TofTrbTdc");
   if(0 == fTrbTdcBoardCollection) 
   {
      LOG(warn)<<"TTofTrbTdcUnpacker::TTofTrbTdcUnpacker : no TOF TRB-TDC array! ";
      fuNbActiveTrbTdc = 0;
   }

   if( fbInspection )
   {
     for(UInt_t uBoardIndex = 0; uBoardIndex < fuNbActiveTrbTdc; uBoardIndex++)
     {
       if( !fParCalib->CheckTotInvFlag( toftdc::trb, uBoardIndex ) )
       {
         LOG(fatal)<<Form("Check your parCalib* file! No ToT inversion flags provided for TDCs indexed %2u and above!!!",uBoardIndex);
       }
     }
   }

}

TTofTrbTdcUnpacker::~TTofTrbTdcUnpacker()
{
   LOG(info)<<"**** TTofTrbTdcUnpacker: Call ~TTofTrbTdcUnpacker()...";
  // Creation and clean-up of member histogram vectors done by TMbsUnpTofMonitor
}

void TTofTrbTdcUnpacker::Clear(Option_t */*option*/)
{
   LOG(info)<<"**** TTofTrbTdcUnpacker: Call Clear()...";

//   fParUnpack = 0;
//   fuNbActiveTrbTdc = 0;
//   fTrbTdcBoardCollection = 0;
}

Int_t TTofTrbTdcUnpacker::ProcessData( hadaq::RawSubevent* tSubevent, UInt_t uStartIndex )
{
/* unclear points:
   - EPOCH reset compensation in stream

   2016-04-07
   Unclear point understood and appropriately treated in the new calibration class
*/
  LOG(debug)<<"**** TTofTrbTdcUnpacker: Call ProcessData()...";

  UInt_t uTdcDataIndex = uStartIndex;

  UInt_t uTdcData = tSubevent->Data(uTdcDataIndex);
  UInt_t uTdcAddress = uTdcData & 0xffff;
  UInt_t uNbTdcDataWords = (uTdcData >> 16) & 0xffff;
  Int_t iTdcBoardIndex = fParUnpack->GetActiveTrbTdcIndex( uTdcAddress );
  Bool_t bUnpackData = fParUnpack->UnpackTrbTdcAddress(uTdcAddress);

  TTofTrbTdcBoard* tTrbTdcBoard = (TTofTrbTdcBoard*) fTrbTdcBoardCollection->ConstructedAt( iTdcBoardIndex );
  tTrbTdcBoard->SetInvalid();
  UInt_t uTdcWordCount = 0;

  // PAL 23/07/2015: Use direct constructor call instead of copy constructor
//  hadaq::TdcIterator tTrbTdcIterator = hadaq::TdcIterator();
  hadaq::TdcIterator tTrbTdcIterator;
  tTrbTdcIterator.assign(tSubevent, uTdcDataIndex+1, uNbTdcDataWords);

  hadaq::TdcMessage& tTrbTdcMessage = tTrbTdcIterator.msg();

  Bool_t bPreviousIsEpoch = kFALSE;
  UInt_t uPreviousChannel = 0;
  UInt_t uPreviousCoarseTime = 0;
  UInt_t uPreviousTimeWord = 0;

  Int_t iStatusMessage = trbtdc::process_Success;

  while ( tTrbTdcIterator.next() )
  {
    uTdcWordCount++;

    if( 1 == uTdcWordCount )
    {
      if ( tTrbTdcMessage.isHeaderMsg() )
      {
        if( tTrbTdcMessage.getHeaderErr() & 0x1 )
        {
          tTrbTdcBoard->SetChannelBufferIssue();
          fmRingBufferOverflow[iTdcBoardIndex] = kTRUE;

//          LOG(error)<<Form("TDC 0x%.4x: At least one channel ring buffer has been overwritten.",uTdcAddress);
//          LOG(error)<<Form("TDC 0x%.4x: At least one channel ring buffer has been overwritten. Skip this TDC subsubevent!",uTdcAddress);
//          return trbtdc::process_RingBufferOverwrite;
        }
      }
      else
      {
        LOG(error)<<Form("TDC 0x%.4x: First data word is not a TDC HEADER word.",uTdcAddress);
        return trbtdc::process_FirstNoHeader;
      }
      
      continue;
    }

    if( 2 == uTdcWordCount )
    {
      if ( tTrbTdcMessage.isEpochMsg() )
      {
        tTrbTdcIterator.setRefEpoch( tTrbTdcIterator.getCurEpoch() );
      }
      else
      {
        LOG(error)<<Form("TDC 0x%.4x: Second data word is not a TDC EPOCH word.",uTdcAddress);
        return trbtdc::process_SecondNoEpoch;
      }
      
      continue;
    }

    if( 3 == uTdcWordCount )
    {
      if ( tTrbTdcMessage.isTimeMsg() )
      {
        if( tTrbTdcMessage.getTimeChannel() == 0 )
        {
          if( trbtdc::kuHighestReasonableFineTimeBin >= tTrbTdcMessage.getTimeTmFine() )
          {
            ( tTrbTdcBoard->GetRefChannelData() ).SetData( 
              tTrbTdcMessage.getTimeChannel(),
              tTrbTdcMessage.getTimeTmFine(),
              ( UInt_t )( tTrbTdcIterator.getMsgStamp() & ( trbtdc::kiCoarseCounterSize-1 ) ),
              0,
              tTrbTdcMessage.getTimeEdge() );
            ( tTrbTdcBoard->GetRefChannelData() ).SetEpoch( tTrbTdcIterator.getCurEpoch() );
            ( tTrbTdcBoard->GetRefChannelData() ).SetFullCoarseTime( tTrbTdcIterator.getMsgStamp() );

            tTrbTdcBoard->SetTriggerTime( ( tTrbTdcBoard->GetRefChannelData() ).GetCoarseTime() );

          }
          else
          {
            LOG(info)<<Form("TDC 0x%.4x: Third data word is a TDC TIME word from channel 0 but with an unreasonable fine time of 0x%.3x.",
                            uTdcAddress,tTrbTdcMessage.getTimeTmFine());
            return trbtdc::process_ThirdBadRefTime;
          }
        }
        else
        {
          LOG(error)<<Form("TDC 0x%.4x: Third data word is a TDC TIME word but not from channel 0.",uTdcAddress);
          return trbtdc::process_ThirdNoRefTime;
        }
      }
      else
      {
        LOG(error)<<Form("TDC 0x%.4x: Third data word is not a TDC TIME word.",uTdcAddress);
        return trbtdc::process_ThirdNoTime;
      }
      
      continue;
    }

    if( 4 == uTdcWordCount )
    {
      if ( !tTrbTdcMessage.isEpochMsg() && !fbCalibTrigger )
      {
        LOG(error)<<Form("TDC 0x%.4x: Fourth data word is not a TDC EPOCH word.",uTdcAddress);
        return trbtdc::process_FourthNoEpoch;
      }

      bPreviousIsEpoch = kTRUE;
      continue;
    }

    if( kTRUE == bPreviousIsEpoch )
    {
      switch ( tTrbTdcMessage.getKind() ) 
      {
        case trbtdc::message_Time:
        case trbtdc::message_Time1:
        case trbtdc::message_Time2:
        case trbtdc::message_Time3:
        {
          bPreviousIsEpoch = kFALSE;

          UInt_t uChNumber         = tTrbTdcMessage.getTimeChannel();
          UInt_t uChFineTime       = tTrbTdcMessage.getTimeTmFine();
          UInt_t uChCoarseTime     = ( UInt_t )( tTrbTdcIterator.getMsgStamp() & ( trbtdc::kiCoarseCounterSize-1 ) );
          UInt_t uChEdge           = tTrbTdcMessage.getTimeEdge();
          UInt_t uChEpoch          = tTrbTdcIterator.getCurEpoch();
          ULong64_t uChFullCoarseTime = tTrbTdcIterator.getMsgStamp();

          uPreviousChannel = uChNumber;

          if ( ( 0 == uChNumber && !fbCalibTrigger ) || ( tTrbTdcBoard->GetChannelNb() < uChNumber ) )
          {
            LOG(error)<<Form("TDC 0x%.4x: Invalid channel number %u in TDC TIME word.",uTdcAddress,uChNumber);
            return trbtdc::process_BadChannelNumber;
          }

          uPreviousCoarseTime = tTrbTdcMessage.getTimeTmCoarse();
          uPreviousTimeWord = tSubevent->Data(uStartIndex+uTdcWordCount);

          fiAvailableHits++;

          if ( trbtdc::time_FineTimeBitMask == uChFineTime )
          {
            LOG(debug)<<Form("TDC 0x%.4x: Hit detected in channel %u but not properly processed. Skip it.", uTdcAddress, uChNumber);
            iStatusMessage = trbtdc::process_BadFineTime;
            continue;
          }

        	if ( 0 == uChNumber )
          {
            LOG(debug2)<<Form("TDC 0x%.4x: Additional calibration hit detected in channel %u. Ignore it.", uTdcAddress, uChNumber);
            continue;
        	}

          LOG(debug2)<<Form("TDC 0x%.4x: Hit in channel %u properly processed. Store it.", uTdcAddress, uChNumber);

          // Remove reference channel index (i.e. ch1->ch0, ch2->ch1 etc.)
          // default: positive LVDS input signals
          //          leading edge in TDC channel with even index
          //          (after compensating for the reference channel index 0)
          //          trailing edge in TDC channel with odd index
          //          (after compensating for the reference channel index 0)
          UInt_t uFinalChNumber = uChNumber - 1;

          if( fbJointEdgesChannel )
          {
            uFinalChNumber = 2*uFinalChNumber + ( trbtdc::edge_Rising == uChEdge ? 0 : 1 );
          }

          if(bUnpackData)
          {
            TTofTrbTdcData tValidHit( uFinalChNumber, uChFineTime, uChCoarseTime, 0, uChEdge, uChEpoch, uChFullCoarseTime );
            tTrbTdcBoard->AddData( tValidHit );
          }
          fiAcceptedHits++;
          continue;
       
          break;
        }
        case trbtdc::message_Epoch:
        {
          LOG(error)<<Form("TDC 0x%.4x: Two consecutive TDC EPOCH words.", uTdcAddress);
          return trbtdc::process_TwoConsecutiveEpochs;

          break;
        }
        default:
        {
          continue;

          break;
        }
      }

    }
    else
    {
      switch ( tTrbTdcMessage.getKind() ) 
      {
        case trbtdc::message_Time:
        case trbtdc::message_Time1:
        case trbtdc::message_Time2:
        case trbtdc::message_Time3:
        {
          UInt_t uChNumber         = tTrbTdcMessage.getTimeChannel();
          UInt_t uChFineTime       = tTrbTdcMessage.getTimeTmFine();
          UInt_t uChCoarseTime     = ( UInt_t )( tTrbTdcIterator.getMsgStamp() & ( trbtdc::kiCoarseCounterSize-1 ) );
          UInt_t uChEdge           = tTrbTdcMessage.getTimeEdge();
          UInt_t uChEpoch          = tTrbTdcIterator.getCurEpoch();
          ULong64_t uChFullCoarseTime = tTrbTdcIterator.getMsgStamp();

          if( ( 0 == uChNumber && !fbCalibTrigger ) || ( tTrbTdcBoard->GetChannelNb() < uChNumber ) )
          {
            LOG(error)<<Form("TDC 0x%.4x: Invalid channel number %u in TDC TIME word.", uTdcAddress, uChNumber);
            return trbtdc::process_BadChannelNumber;
          }

          if( uPreviousChannel != uChNumber )
          {
            LOG(error)<<Form("TDC 0x%.4x: Change in hit channel number from %u to %u without a TDC EPOCH word in between.", uTdcAddress, uPreviousChannel, uChNumber);
            return trbtdc::process_ChannelButNoEpochChange;
          }

          UInt_t uBackupCoarseTime = uPreviousCoarseTime;
          UInt_t uBackupTimeWord = uPreviousTimeWord;

          uPreviousCoarseTime = tTrbTdcMessage.getTimeTmCoarse();
          uPreviousTimeWord = tSubevent->Data(uStartIndex+uTdcWordCount);

          fiAvailableHits++;

          if( trbtdc::time_FineTimeBitMask == uChFineTime )
          {
            LOG(debug)<<Form("TDC 0x%.4x: Hit detected in channel %u but not properly processed. Skip it.", uTdcAddress, uChNumber);
            iStatusMessage = trbtdc::process_BadFineTime;
            continue;
          }

          if( 0 == uChNumber )
          {
            LOG(debug2)<<Form("TDC 0x%.4x: Additional calibration hit detected in channel %u. Ignore it.", uTdcAddress, uChNumber);
            continue;
          }
          
          if( uBackupCoarseTime > tTrbTdcMessage.getTimeTmCoarse() )
          {
            LOG(error)<<Form("TDC 0x%.4x: Coarse counter of channel %u overflowed but the TDC epoch counter was not incremented.",uTdcAddress,uChNumber);
//            continue;
            return trbtdc::process_NoEpochIncrement;
          }

          if( uBackupTimeWord == uPreviousTimeWord )
          {
            LOG(error)<<Form("TDC 0x%.4x: Two identical TIME words in a row in channel %u.",uTdcAddress,uChNumber);
            return trbtdc::process_TwoIdenticalWords;
          }

          LOG(debug2)<<Form("TDC 0x%.4x: Hit in channel %u properly processed. Store it.", uTdcAddress, uChNumber);

          // Remove reference channel index (i.e. ch1->ch0, ch2->ch1 etc.)
          // default: positive LVDS input signals
          //          leading edge in TDC channel with even index
          //          (after compensating for the reference channel index 0)
          //          trailing edge in TDC channel with odd index
          //          (after compensating for the reference channel index 0)
          UInt_t uFinalChNumber = uChNumber - 1;

          if( fbJointEdgesChannel )
          {
            uFinalChNumber = 2*uFinalChNumber + ( trbtdc::edge_Rising == uChEdge ? 0 : 1 );
          }

          if(bUnpackData)
          {
            TTofTrbTdcData tValidHit( uFinalChNumber, uChFineTime, uChCoarseTime, 0, uChEdge, uChEpoch, uChFullCoarseTime );
            tTrbTdcBoard->AddData( tValidHit );
          }
          fiAcceptedHits++;
          continue;

          break;
        }
        case trbtdc::message_Epoch:
        {
          bPreviousIsEpoch = kTRUE;
          continue;

          break;
        }
        default:
        {
          continue;

          break;
        }
      }

    }

  }

/*
// Example of a raw data dump

  gLogger->SetLogScreenLevel("ERROR");
  LOG(error)<<"board index: "<<iTdcBoardIndex;

  for(UInt_t uWord = uStartIndex; uWord <= uStartIndex+uNbTdcDataWords; uWord++)
  {
    if((tSubevent->Data(uWord) & 0xe0000000) != 0x80000000)
    {
      LOG(error)<< Form("0x%.8x",tSubevent->Data(uWord));
    }
    else
    {
      LOG(error)<< Form("0x%.8x, channel: %02u, coarse time: %4u, fine time: %4u, edge: %1u",
                        tSubevent->Data(uWord),tSubevent->Data(uWord)>>22 & 0x7f,
                        tSubevent->Data(uWord) & 0x7ff,
                        tSubevent->Data(uWord)>>12 & 0x3ff,
                        tSubevent->Data(uWord)>>11 & 0x1);
    }
  }

  LOG(error);
  gLogger->SetLogScreenLevel("FATAL");
*/


//  tTrbTdcBoard->SortData();

  tTrbTdcBoard->SetCalibData(fbCalibTrigger);
  tTrbTdcBoard->SetValid();

  if( 0 == iTdcBoardIndex )
     fuEventIndex++;

  LOG(debug)<<Form("TRB-TDC subsubevent from FPGA 0x%.4x fully unpacked. Success!",uTdcAddress);
  return iStatusMessage;

}

// Histogram management
// TODO: maybe add some options to control which histograms are 
// created and filed (memory management)
void TTofTrbTdcUnpacker::CreateHistos()
{
   LOG(info)<<"**** TTofTrbTdcUnpacker: Call CreateHistos()...";
  
   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !

   fTrbTdcChannelOccupancy.resize( fuNbActiveTrbTdc, NULL );
/*
   fTrbTdcChannelFineTimeOvershoot.resize( fuNbActiveTrbTdc, NULL );
   fTrbTdcChannelUnprocessedHits.resize( fuNbActiveTrbTdc, NULL );
*/

   fTrbTdcChannelFineTime.resize( fuNbActiveTrbTdc, std::vector<TH1I*>( trbtdc::kuNbChan+1, NULL ) );
//   fTrbTdcChannelCoarseTime.resize( fuNbActiveTrbTdc, std::vector<TH1I*>( trbtdc::kuNbChan+1, NULL ) );
   fTrbTdcChannelToT.resize( fuNbActiveTrbTdc, std::vector<TH1I*>( trbtdc::kuNbChan, NULL ) );

   if( fbInspection )
   {
     fiLeadingOnlyBuffers.resize( fuNbActiveTrbTdc, std::vector<Int_t>( trbtdc::kuNbChan/2, 0. ) );
     fiTrailingOnlyBuffers.resize( fuNbActiveTrbTdc, std::vector<Int_t>( trbtdc::kuNbChan/2, 0. ) );
     fiUnequalEdgesBuffers.resize( fuNbActiveTrbTdc, std::vector<Int_t>( trbtdc::kuNbChan/2, 0. ) );
     fiAllNonEmptyBuffers.resize( fuNbActiveTrbTdc, std::vector<Int_t>( trbtdc::kuNbChan/2, 0. ) );

     fLeadingOnlyShares.resize( fuNbActiveTrbTdc, NULL );
     fTrailingOnlyShares.resize( fuNbActiveTrbTdc, NULL );
     fUnequalEdgesShares.resize( fuNbActiveTrbTdc, NULL );

     fTrbTdcBoardTot.resize( fuNbActiveTrbTdc, NULL );
     fTrbTdcLeadingPosition.resize( fuNbActiveTrbTdc, NULL );
     fTrbTdcTrailingPosition.resize( fuNbActiveTrbTdc, NULL );

     fTrbTdcBoardFineTime.resize( fuNbActiveTrbTdc, NULL );
   }

   for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbActiveTrbTdc; uBoardIndex++)
   {
      UInt_t uTrbNetAddress = fParUnpack->GetActiveTrbTdcAddr(uBoardIndex);

      // Board specific histograms
      fTrbTdcChannelOccupancy.at(uBoardIndex) = new TH1I( Form("tof_%s_ch_occ_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                          Form("Channel occupancy in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                          trbtdc::kuNbChan+1, 0.0, trbtdc::kuNbChan+1 );
      fTrbTdcChannelOccupancy.at(uBoardIndex)->GetXaxis()->SetTitle("TDC channel []");
      
/*
      fTrbTdcChannelFineTimeOvershoot.at(uBoardIndex) = new TH1I( Form("tof_%s_ch_highft_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                                  Form("Fine time overshoot in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                                  trbtdc::kuNbChan+1, 0.0, trbtdc::kuNbChan+1 );

      fTrbTdcChannelUnprocessedHits.at(uBoardIndex) = new TH1I( Form("tof_%s_ch_unproc_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                                Form("Unprocessed hits in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                                trbtdc::kuNbChan+1, 0.0, trbtdc::kuNbChan+1 );
*/
      if( fbInspection )
      {
        fLeadingOnlyShares.at(uBoardIndex) = new TH1D( Form("tof_%s_ch_leadonly_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                       Form("Leading-edge only event shares in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                       trbtdc::kuNbChan/2, 0., trbtdc::kuNbChan/2. );
        fLeadingOnlyShares.at(uBoardIndex)->GetXaxis()->SetTitle("Readout channel []");
        fLeadingOnlyShares.at(uBoardIndex)->GetYaxis()->SetTitle("event share [%]");

        fTrailingOnlyShares.at(uBoardIndex) = new TH1D( Form("tof_%s_ch_trailonly_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                        Form("Trailing-edge only event shares in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                        trbtdc::kuNbChan/2, 0., trbtdc::kuNbChan/2. );
        fTrailingOnlyShares.at(uBoardIndex)->GetXaxis()->SetTitle("Readout channel []");
        fTrailingOnlyShares.at(uBoardIndex)->GetYaxis()->SetTitle("event share [%]");

        fUnequalEdgesShares.at(uBoardIndex) = new TH1D( Form("tof_%s_ch_unequal_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                        Form("Unequal edge count event shares in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                        trbtdc::kuNbChan/2, 0., trbtdc::kuNbChan/2. );
        fUnequalEdgesShares.at(uBoardIndex)->GetXaxis()->SetTitle("Readout channel []");
        fUnequalEdgesShares.at(uBoardIndex)->GetYaxis()->SetTitle("event share [%]");

        fTrbTdcBoardTot.at(uBoardIndex) = new TH2I( Form("tof_%s_board_tot_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                    Form("All channel ToT in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                    trbtdc::kuNbChan/2, 0., trbtdc::kuNbChan/2.,
                                                    2000, -100., 100.);
        fTrbTdcBoardTot.at(uBoardIndex)->GetXaxis()->SetTitle("Readout channel []");
        fTrbTdcBoardTot.at(uBoardIndex)->GetYaxis()->SetTitle("ToT [ns]");

        fTrbTdcLeadingPosition.at(uBoardIndex) = new TH2I( Form("tof_%s_board_lead_pos_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                           Form("Leading edge position in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                           trbtdc::kuNbChan/2, 0., trbtdc::kuNbChan/2.,
                                                           701,-600.5,100.5);
        fTrbTdcLeadingPosition.at(uBoardIndex)->GetXaxis()->SetTitle("Readout channel []");
        fTrbTdcLeadingPosition.at(uBoardIndex)->GetYaxis()->SetTitle("leading time [ns]");

        fTrbTdcTrailingPosition.at(uBoardIndex) = new TH2I( Form("tof_%s_board_trail_pos_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                           Form("Trailing edge position in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                           trbtdc::kuNbChan/2, 0., trbtdc::kuNbChan/2.,
                                                           701,-600.5,100.5);
        fTrbTdcTrailingPosition.at(uBoardIndex)->GetXaxis()->SetTitle("Readout channel []");
        fTrbTdcTrailingPosition.at(uBoardIndex)->GetYaxis()->SetTitle("trailing time [ns]");

        fTrbTdcBoardFineTime.at(uBoardIndex) = new TH2I( Form("tof_%s_board_ft_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                           Form("Fine time in TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                           trbtdc::kuNbChan, 0., trbtdc::kuNbChan,
                                                           trbtdc::kuHighestReasonableFineTimeBin, 0, trbtdc::kuHighestReasonableFineTimeBin);
        fTrbTdcBoardFineTime.at(uBoardIndex)->GetXaxis()->SetTitle("TDC channel []");
        fTrbTdcBoardFineTime.at(uBoardIndex)->GetYaxis()->SetTitle("fine time [bins]");
      }

      // Reference Channel specific histograms
      (fTrbTdcChannelFineTime.at(uBoardIndex)).at(0) = new TH1I( Form("tof_%s_ft_b%03u_ref", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                                  Form("Counts per fine-time bin for reference channel on TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                                  trbtdc::kiFineCounterSize, -0.5, trbtdc::kiFineCounterSize - 0.5 );
      (fTrbTdcChannelFineTime.at(uBoardIndex)).at(0)->GetXaxis()->SetTitle("fine time [bins]");
/*
      // TODO: remove hard coding
      (fTrbTdcChannelCoarseTime.at(uBoardIndex)).at(0) = new TH1I( Form("tof_%s_ct_b%03u_ref", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex),
                                                                   Form("Counts per coarse-time bin for reference channel on TRB-TDC #%03u (0x%04x)", uBoardIndex, uTrbNetAddress),
                                                                   trbtdc::kiCoarseCounterSize/1024/512, -0.5, trbtdc::kiCoarseCounterSize/1024/512 - 0.5 );
*/

      // Channel specific histograms
      for( UInt_t uChannelIndex = 0; uChannelIndex < trbtdc::kuNbChan; uChannelIndex++)
      {

         (fTrbTdcChannelFineTime.at(uBoardIndex)).at(uChannelIndex+1) = new TH1I( Form("tof_%s_ft_b%03u_ch%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex, uChannelIndex),
                                                                                  Form("Counts per fine-time bin for channel %3u on TRB-TDC #%03u (0x%04x)", uChannelIndex, uBoardIndex, uTrbNetAddress),
                                                                                  trbtdc::kiFineCounterSize, -0.5, trbtdc::kiFineCounterSize - 0.5 );
         (fTrbTdcChannelFineTime.at(uBoardIndex)).at(uChannelIndex+1)->GetXaxis()->SetTitle("fine time [bins]");
/*
         // TODO: remove hard coding
         (fTrbTdcChannelCoarseTime.at(uBoardIndex)).at(uChannelIndex+1) = new TH1I( Form("tof_%s_ct_b%03u_ch%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex, uChannelIndex),
                                                                                    Form("Counts per coarse-time bin for channel %3u on TRB-TDC #%03u (0x%04x)", uChannelIndex, uBoardIndex, uTrbNetAddress),
                                                                                    trbtdc::kiCoarseCounterSize/1024/512, -0.5, trbtdc::kiCoarseCounterSize/1024/512 - 0.5 );
*/
      }

      if( fbInspection )
      {
        for( UInt_t uChannelIndex = 0; uChannelIndex < trbtdc::kuNbChan/2; uChannelIndex++)
        {
          (fTrbTdcChannelToT.at(uBoardIndex)).at(uChannelIndex) = new TH1I( Form("tof_%s_tot_b%03u_ch%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex, uChannelIndex+1),
                                                                            Form("ToT for channel pair %3u on TRB-TDC 0x%04x", uChannelIndex, uTrbNetAddress),
                                                                            2000, -100, 100);
          (fTrbTdcChannelToT.at(uBoardIndex)).at(uChannelIndex)->GetXaxis()->SetTitle("ToT [ns]");
        }
      }
      
   }
   
   fTrbTdcRingBufferOverflow = new TH1I( Form("tof_%s_buffer_overflow", toftdc::ksTdcHistName[ toftdc::trb ].Data()),
		                                     "TRB-TDC ring buffer overflows per board",
		                                     fuNbActiveTrbTdc, 0, fuNbActiveTrbTdc);
   fTrbTdcRingBufferOverflow->GetXaxis()->SetTitle("TDC board []");

   if( fbInspection )
   {
     fTrbTdcRefChannelFineTime = new TH2I( Form("tof_%s_all_ref_ft", toftdc::ksTdcHistName[ toftdc::trb ].Data()),
                                           Form("All TRB-TDC reference fine times"),
                                           fuNbActiveTrbTdc, 0, fuNbActiveTrbTdc,
                                           trbtdc::kuHighestReasonableFineTimeBin, 0, trbtdc::kuHighestReasonableFineTimeBin);
     fTrbTdcRefChannelFineTime->GetXaxis()->SetTitle("TDC board []");
     fTrbTdcRefChannelFineTime->GetYaxis()->SetTitle("fine time [bins]");
   }

   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!
}

void TTofTrbTdcUnpacker::FillHistos()
{
   LOG(debug)<<"**** TTofTrbTdcUnpacker: Call FillHistos()...";

   // loop over TDC boards
   TTofTrbTdcBoard * tTrbTdcBoard;

   for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbActiveTrbTdc; uBoardIndex++)
   { 
      UInt_t uTrbNetAddress = fParUnpack->GetActiveTrbTdcAddr(uBoardIndex);

      tTrbTdcBoard = (TTofTrbTdcBoard*) fTrbTdcBoardCollection->ConstructedAt(uBoardIndex);

      std::vector< std::vector<const TTofTrbTdcData*> > tToTEvenEdgeBuffers( trbtdc::kuNbChan/2, std::vector<const TTofTrbTdcData*>() );
      std::vector< std::vector<const TTofTrbTdcData*> > tToTOddEdgeBuffers( trbtdc::kuNbChan/2, std::vector<const TTofTrbTdcData*>() );

      if( fbInspection )
      {
        for( UInt_t uChannelIndex = 0; uChannelIndex < trbtdc::kuNbChan/2; uChannelIndex++ )
        {
          (tToTEvenEdgeBuffers.at(uChannelIndex)).reserve(trbtdc::kuNbMulti);
          (tToTOddEdgeBuffers.at(uChannelIndex)).reserve(trbtdc::kuNbMulti);
        }
      }

      // Loop over regular channels
      for( UInt_t uDataIndex = 0; uDataIndex < tTrbTdcBoard->GetDataNb() ; uDataIndex++ )
      {
         TTofTrbTdcData data = tTrbTdcBoard->GetData( uDataIndex );

         if( fbInspection )
         {
           const TTofTrbTdcData* tTdcData = tTrbTdcBoard->GetDataPtr(uDataIndex);

           UInt_t uCurrentChannel(tTdcData->GetChannel());

           if( 0 == uCurrentChannel % 2)
           {
             (tToTEvenEdgeBuffers.at(uCurrentChannel/2)).push_back(tTdcData);
           }
           else
           {
             (tToTOddEdgeBuffers.at(uCurrentChannel/2)).push_back(tTdcData);
           }
         }

         TString sTemp = Form( " TTofTrbTdcUnpacker::FillHistos: Addr 0x%04x Board #%03u Data #%04u Chan %3d CT %7d FT %7d Edge %1u",
                  uTrbNetAddress, uBoardIndex, uDataIndex, data.GetChannel()+1, 
                  data.GetCoarseTime(), data.GetFineTime(), data.GetEdge() );
         LOG(debug2)<<sTemp;
         
         fTrbTdcChannelOccupancy[uBoardIndex]->Fill( data.GetChannel() + 1 );

/*
         if( trbtdc::time_FineTimeBitMask == data.GetFineTime() )
         {
           fTrbTdcChannelUnprocessedHits[uBoardIndex]->Fill( data.GetChannel() + 1 );
         }

         if( trbtdc::kuHighestReasonableFineTimeBin < data.GetFineTime() )
         {
           fTrbTdcChannelFineTimeOvershoot[uBoardIndex]->Fill( data.GetChannel() + 1 );           
         }
*/

         fTrbTdcChannelFineTime[uBoardIndex][ data.GetChannel() +1 ]->Fill( data.GetFineTime() );
         if( fbInspection )
         {
           fTrbTdcBoardFineTime.at(uBoardIndex)->Fill(data.GetChannel(),data.GetFineTime());
         }
//         fTrbTdcChannelCoarseTime[uBoardIndex][ data.GetChannel() +1 ]->Fill( data.GetCoarseTime()%2048 );
      }

      if( fbInspection )
      {
        const TTofTrbTdcData& tTdcRefTime = tTrbTdcBoard->GetRefChannelData();
        Long64_t liRefCoarseTime = static_cast<Long64_t>(tTdcRefTime.GetFullCoarseTime());

        Double_t dReferenceTimeStamp = static_cast<Double_t>(liRefCoarseTime) - hadaq::TdcMessage::SimpleFineCalibr(tTdcRefTime.GetFineTime());

        // ToT building function
        for( UInt_t uChannelIndex = 0; uChannelIndex < trbtdc::kuNbChan/2; uChannelIndex++ )
        {
          UInt_t uLeadingModulo;
          
          UInt_t uLeadingMultiplicity;
          UInt_t uTrailingMultiplicity;

          // positive LVDS input signal
          // leading edge in even channel
          // trailing edge in odd channel
          if( fParCalib->GetTotInvFlag( toftdc::trb, uBoardIndex, uChannelIndex ) )
          {
            uLeadingModulo = 0;

            uLeadingMultiplicity = (tToTEvenEdgeBuffers.at(uChannelIndex)).size();
            uTrailingMultiplicity = (tToTOddEdgeBuffers.at(uChannelIndex)).size();
          }
          // negative LVDS input signal
          // trailing edge in even channel
          // leading edge in odd channel
          else
          {
            uLeadingModulo = 1;

            uTrailingMultiplicity = (tToTEvenEdgeBuffers.at(uChannelIndex)).size();
            uLeadingMultiplicity = (tToTOddEdgeBuffers.at(uChannelIndex)).size();
          }

          if( uLeadingMultiplicity && uTrailingMultiplicity )
          {
            if( uLeadingMultiplicity == uTrailingMultiplicity )
            {
              for( UInt_t uEvenIndex = 0; uEvenIndex < (tToTEvenEdgeBuffers.at(uChannelIndex)).size(); uEvenIndex++ )
              {
                const TTofTrbTdcData* leadingData;
                const TTofTrbTdcData* trailingData;

                if( 0 == uLeadingModulo )
                {
                  leadingData  = (tToTEvenEdgeBuffers.at(uChannelIndex)).at(uEvenIndex);
                  trailingData = (tToTOddEdgeBuffers.at(uChannelIndex)).at(uEvenIndex);
                }
                else
                {
                  leadingData = (tToTOddEdgeBuffers.at(uChannelIndex)).at(uEvenIndex);
                  trailingData = (tToTEvenEdgeBuffers.at(uChannelIndex)).at(uEvenIndex);
                }

                Long64_t liLeadingCoarseTime = static_cast<Long64_t>(leadingData->GetFullCoarseTime());
                Long64_t liTrailingCoarseTime = static_cast<Long64_t>(trailingData->GetFullCoarseTime());

                // Correcting for TDC internal coarse counter overflows w.r.t.
                // the reference channel
                hadaq::TdcMessage::CoarseOverflow(liLeadingCoarseTime,liRefCoarseTime);
                hadaq::TdcMessage::CoarseOverflow(liTrailingCoarseTime,liRefCoarseTime);

                Double_t dLeadingTimeStamp = static_cast<Double_t>(liLeadingCoarseTime) - hadaq::TdcMessage::SimpleFineCalibr(leadingData->GetFineTime());
                Double_t dTrailingTimeStamp = static_cast<Double_t>(liTrailingCoarseTime) - hadaq::TdcMessage::SimpleFineCalibr(trailingData->GetFineTime());

                // ToT in ps
                Double_t dTimeOverThreshold = (dTrailingTimeStamp - dLeadingTimeStamp)*trbtdc::kdClockCycleSize;

                fTrbTdcChannelToT[uBoardIndex][uChannelIndex]->Fill( dTimeOverThreshold/1000. );

                fTrbTdcBoardTot.at(uBoardIndex)->Fill(uChannelIndex,dTimeOverThreshold/1000.);

                fTrbTdcLeadingPosition.at(uBoardIndex)->Fill(uChannelIndex,(dLeadingTimeStamp-dReferenceTimeStamp)*trbtdc::kdClockCycleSize/1000.);

                fTrbTdcTrailingPosition.at(uBoardIndex)->Fill(uChannelIndex,(dTrailingTimeStamp-dReferenceTimeStamp)*trbtdc::kdClockCycleSize/1000.);

              }
            }
            else
            {
              (fiUnequalEdgesBuffers.at(uBoardIndex)).at(uChannelIndex)++;
            }
          }
          else if( uLeadingMultiplicity )
          {
            (fiLeadingOnlyBuffers.at(uBoardIndex)).at(uChannelIndex)++;
            (fiUnequalEdgesBuffers.at(uBoardIndex)).at(uChannelIndex)++;
          }
          else if( uTrailingMultiplicity )
          {
            (fiTrailingOnlyBuffers.at(uBoardIndex)).at(uChannelIndex)++;
            (fiUnequalEdgesBuffers.at(uBoardIndex)).at(uChannelIndex)++;
          }

          if( uLeadingMultiplicity || uTrailingMultiplicity )
          {
            (fiAllNonEmptyBuffers.at(uBoardIndex)).at(uChannelIndex)++;
          }


          if( (fiAllNonEmptyBuffers.at(uBoardIndex)).at(uChannelIndex) )
          {
            (fLeadingOnlyShares.at(uBoardIndex))->SetBinContent(uChannelIndex+1,(100.*(fiLeadingOnlyBuffers.at(uBoardIndex)).at(uChannelIndex))/(fiAllNonEmptyBuffers.at(uBoardIndex)).at(uChannelIndex));
            (fTrailingOnlyShares.at(uBoardIndex))->SetBinContent(uChannelIndex+1,(100.*(fiTrailingOnlyBuffers.at(uBoardIndex)).at(uChannelIndex))/(fiAllNonEmptyBuffers.at(uBoardIndex)).at(uChannelIndex));
            (fUnequalEdgesShares.at(uBoardIndex))->SetBinContent(uChannelIndex+1,(100.*(fiUnequalEdgesBuffers.at(uBoardIndex)).at(uChannelIndex))/(fiAllNonEmptyBuffers.at(uBoardIndex)).at(uChannelIndex));
          }
        }
      }

      // Consider the respective reference channel
      fTrbTdcChannelOccupancy[uBoardIndex]->Fill(0);

/*
      if( trbtdc::time_FineTimeBitMask == ( tTrbTdcBoard->GetRefChannelData() ).GetFineTime() )
      {
        fTrbTdcChannelUnprocessedHits[uBoardIndex]->Fill(0);
      }

      if( trbtdc::kuHighestReasonableFineTimeBin < ( tTrbTdcBoard->GetRefChannelData() ).GetFineTime() )
      {
        fTrbTdcChannelFineTimeOvershoot[uBoardIndex]->Fill(0);           
      }
*/
      fTrbTdcChannelFineTime[uBoardIndex][0]->Fill( ( tTrbTdcBoard->GetRefChannelData() ).GetFineTime() );
      if( fbInspection )
      {
        fTrbTdcRefChannelFineTime->Fill(uBoardIndex, ( tTrbTdcBoard->GetRefChannelData() ).GetFineTime());
      }
//      fTrbTdcChannelCoarseTime[uBoardIndex][0]->Fill( ( tTrbTdcBoard->GetRefChannelData() ).GetCoarseTime()%2048 );

   }

   for ( std::map<Int_t,Bool_t>::iterator it = fmRingBufferOverflow.begin();
	     it != fmRingBufferOverflow.end(); ++it)
   {
  	 if( it->second )
  	 {
       fTrbTdcRingBufferOverflow->Fill( it->first );
	   }
   }

   fmRingBufferOverflow.clear();
}

void TTofTrbTdcUnpacker::WriteHistos( TDirectory* inDir)
{
   LOG(info)<<"**** TTofTrbTdcUnpacker: Call WriteHistos()...";

   TDirectory * oldDir = gDirectory;
   TDirectory * cdTrbTdcUnp[fuNbActiveTrbTdc];
   
   for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbActiveTrbTdc; uBoardIndex++)
   { 
      cdTrbTdcUnp[uBoardIndex] = inDir->mkdir( Form( "Unp_%s_%03u", toftdc::ksTdcHistName[ toftdc::trb ].Data(), uBoardIndex) );
      cdTrbTdcUnp[uBoardIndex]->cd();    // make the "Unp..." directory the current directory
      
      fTrbTdcChannelOccupancy[uBoardIndex]->Write();
/*
      fTrbTdcChannelFineTimeOvershoot[uBoardIndex]->Write();
      fTrbTdcChannelUnprocessedHits[uBoardIndex]->Write();
*/
      for( UInt_t uChannelIndex = 0; uChannelIndex <= trbtdc::kuNbChan; uChannelIndex++)
      {
         fTrbTdcChannelFineTime[uBoardIndex][uChannelIndex]->Write();
//         fTrbTdcChannelCoarseTime[uBoardIndex][uChannelIndex]->Write();
      }

      if( fbInspection )
      {
        fLeadingOnlyShares.at(uBoardIndex)->Write();
        fTrailingOnlyShares.at(uBoardIndex)->Write();
        fUnequalEdgesShares.at(uBoardIndex)->Write();
        fTrbTdcBoardFineTime.at(uBoardIndex)->Write();

        for( UInt_t uChannelIndex = 0; uChannelIndex < trbtdc::kuNbChan/2; uChannelIndex++)
        {
           fTrbTdcChannelToT[uBoardIndex][uChannelIndex]->Write();
        }
      }      
   }

   TDirectory * tAllHistoDir = inDir->mkdir( Form( "Unp_%s_all", toftdc::ksTdcHistName[ toftdc::trb ].Data()) );
   tAllHistoDir->cd();
   fTrbTdcRingBufferOverflow->Write();

   if( fbInspection )
   {
     fTrbTdcRefChannelFineTime->Write();
   }

   gDirectory->Cd(oldDir->GetPath());

   LOG(info) << "Statistics report from TRB-TDC raw data unpacker";
   LOG(info) << "Available TDC hits (time words): " << fiAvailableHits;
   LOG(info) << "Accepted  TDC hits (time words): " << fiAcceptedHits;
   LOG(info) << Form("percentage share: %4.1f", 100.*fiAcceptedHits/fiAvailableHits) << " %";

}

void TTofTrbTdcUnpacker::DeleteHistos()
{  // Method probably never called!!!
   LOG(info)<<"**** TTofTrbTdcUnpacker: Call DeleteHistos()...";

   for( UInt_t uBoardIndex = 0; uBoardIndex < fuNbActiveTrbTdc; uBoardIndex++)
   {
     delete fTrbTdcChannelOccupancy[uBoardIndex];
/*
     delete fTrbTdcChannelFineTimeOvershoot[uBoardIndex];
     delete fTrbTdcChannelUnprocessedHits[uBoardIndex];
*/
     for( UInt_t uChannelIndex = 0; uChannelIndex <= trbtdc::kuNbChan; uChannelIndex++)
     {
       delete fTrbTdcChannelFineTime[uBoardIndex][uChannelIndex];
//       delete fTrbTdcChannelCoarseTime[uBoardIndex][uChannelIndex];
     }

     if( fbInspection )
     {
       delete fLeadingOnlyShares.at(uBoardIndex);
       delete fTrailingOnlyShares.at(uBoardIndex);
       delete fUnequalEdgesShares.at(uBoardIndex);
       delete fTrbTdcBoardFineTime.at(uBoardIndex);

       for( UInt_t uChannelIndex = 0; uChannelIndex < trbtdc::kuNbChan/2; uChannelIndex++)
       {
         delete fTrbTdcChannelToT[uBoardIndex][uChannelIndex];
       }
     }      
   }

   delete fTrbTdcRingBufferOverflow;

   if( fbInspection )
   {
     delete fTrbTdcRefChannelFineTime;
   }
}
