// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018MonitorAlgoPsd                        -----
// -----              Created 26.09.2019 by N.Karpushkin                   -----
// -----      based on CbmMcbm2018MonitorAlgoT0 by P.-A. Loizeau           -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorAlgoPsd.h"

#include "CbmFormatMsHeaderPrintout.h"
#include "CbmMcbm2018PsdPar.h"
#include "CbmFlesHistosTools.h"

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
#include "TGraph.h"
#include "TCanvas.h"
#include "TPaveStats.h"

#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <fstream>

// -------------------------------------------------------------------------
CbmMcbm2018MonitorAlgoPsd::CbmMcbm2018MonitorAlgoPsd() :
   CbmStar2019Algo(),
   /// From the class itself
   fbMonitorMode( kFALSE ),
   fbDebugMonitorMode( kTRUE ),
   fvbMaskedComponents(),
   fbFirstPackageError( kTRUE ),
   fUnpackPar( nullptr ),
   fuNrOfGdpbs( 0 ),
   fGdpbIdIndexMap(),
   fuNrOfFeePerGdpb( 0 ),
   fuNrOfChannelsPerFee( 0 ),
   fuNrOfChannelsPerGdpb( 0 ),
   fulCurrentTsIdx( 0 ),
   fulCurrentMsIdx( 0 ),
   fdTsStartTime( -1.0 ),
   fdTsStopTimeCore( -1.0 ),
   fdMsTime( -1.0 ),
   fuMsIndex( 0 ),
   fuCurrentEquipmentId( 0 ),
   fuCurrDpbId( 0 ),
   fuCurrDpbIdx( 0 ),
   fiRunStartDateTimeSec( -1 ),
   fiBinSizeDatePlots( -1 ),
   fvulCurrentEpoch(),
   fvulCurrentEpochCycle(),
   fvulCurrentEpochFull(),
   fdStartTime( - 1.0 ),
   fdStartTimeMsSz( 0.0 ),
   ftStartTimeUnix( std::chrono::steady_clock::now() ),
   fuHistoryHistoSize( 3600 ),
   fviHistoChargeArgs( 3, 0 ),
   fviHistoAmplArgs( 3, 0 ),
   fviHistoZLArgs( 3, 0 ),
   fuReadEvtCnt ( 0 ),
   fuMsgsCntInMs ( 0 ),
   fuReadMsgsCntInMs ( 0 ),
   fuLostMsgsCntInMs ( 0 ),
   fuReadEvtCntInMs ( 0 ),
   fvuHitCntChanMs( kuNbChanPsd, 0 ),
   fvuErrorCntChanMs( kuNbChanPsd, 0  ),
   fvuEvtLostCntChanMs( kuNbChanPsd, 0  ),
   fvhHitCntEvoChan( kuNbChanPsd, nullptr ),
   fvhHitCntPerMsEvoChan( kuNbChanPsd, nullptr ),
   fvhHitChargeChan( kuNbChanPsd, nullptr ),
   fvhHitZeroLevelChan( kuNbChanPsd, nullptr ),
   fvhHitAmplChan( kuNbChanPsd, nullptr ),
   fvhHitChargeByWfmChan( kuNbChanPsd, nullptr ),
   fvhHitChargeEvoChan( kuNbChanPsd, nullptr ),
   fvhHitWfmChan( kuNbChanPsd, nullptr ),
   kvuWfmRanges( kuNbWfmRanges, 0 ),
   kvuWfmInRangeToChangeChan( kuNbChanPsd*kuNbWfmRanges, 0 ),
   fv3hHitWfmFlattenedChan( kuNbChanPsd*kuNbWfmRanges*kuNbWfmExamples, nullptr ),
   fbSpillOn( kTRUE ),
   fuCurrentSpillIdx( 0 ),
   fuCurrentSpillPlot( 0 ),
   fdStartTimeSpill( -1.0 ),
   fdLastSecondTime( -1.0 ),
   fuCountsLastSecond( 0 ),
   fhChannelMap( nullptr ),
   fhHitChargeMap( nullptr ),
   fhHitMapEvo( nullptr ),
   fhChanHitMapEvo( nullptr ),
   fvhChannelMapSpill(),
   fhHitsPerSpill( nullptr ),
   fhMsgsCntEvo( nullptr ),
   fhReadMsgsCntEvo( nullptr ),
   fhLostMsgsCntEvo( nullptr ),
   fhReadEvtsCntEvo( nullptr ),
   fhAdcTimeEvo( nullptr ),
   fhMsgsCntPerMsEvo( nullptr ),
   fhReadMsgsCntPerMsEvo( nullptr ),
   fhLostMsgsCntPerMsEvo( nullptr ),
   fhReadEvtsCntPerMsEvo( nullptr ),
   fcSummary( nullptr ),
   fcHitMaps( nullptr ),
   fcChargesFPGA( nullptr ),
   fcChargesWfm( nullptr ),
   fcAmplitudes( nullptr ),
   fcGenCntsPerMs( nullptr ),
   fcSpillCounts( nullptr ),
   fcSpillCountsHori( nullptr ),
   fcWfmsAllChannels( nullptr ),
   fvcWfmsChan( kuNbChanPsd, nullptr )
{
}
CbmMcbm2018MonitorAlgoPsd::~CbmMcbm2018MonitorAlgoPsd()
{
   /// Clear buffers

}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018MonitorAlgoPsd::Init()
{
   LOG(info) << "Initializing mCBM Psd 2019 monitor algo";

   return kTRUE;
}
void CbmMcbm2018MonitorAlgoPsd::Reset()
{
}
void CbmMcbm2018MonitorAlgoPsd::Finish()
{
   /// Printout Goodbye message and stats

   /// Write Output histos

}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018MonitorAlgoPsd::InitContainers()
{
   LOG(info) << "Init parameter containers for CbmMcbm2018MonitorAlgoPsd";
   Bool_t initOK = ReInitContainers();

   return initOK;
}
Bool_t CbmMcbm2018MonitorAlgoPsd::ReInitContainers()
{
   LOG(info) << "**********************************************";
   LOG(info) << "ReInit parameter containers for CbmMcbm2018MonitorAlgoPsd";

   fUnpackPar = (CbmMcbm2018PsdPar*)fParCList->FindObject("CbmMcbm2018PsdPar");
   if( nullptr == fUnpackPar )
      return kFALSE;

   Bool_t initOK = InitParameters();

   return initOK;
}
TList* CbmMcbm2018MonitorAlgoPsd::GetParList()
{
   if( nullptr == fParCList )
      fParCList = new TList();
   fUnpackPar = new CbmMcbm2018PsdPar("CbmMcbm2018PsdPar");
   fParCList->Add(fUnpackPar);

   return fParCList;
}
Bool_t CbmMcbm2018MonitorAlgoPsd::InitParameters()
{
   fuNrOfGdpbs = fUnpackPar->GetNrOfGdpbs();
   LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;

   fuNrOfFeePerGdpb = fUnpackPar->GetNrOfFeesPerGdpb();
   LOG(info) << "Nr. of FEEs per Psd GDPB: " << fuNrOfFeePerGdpb;

   fuNrOfChannelsPerFee = fUnpackPar->GetNrOfChannelsPerFee();
   LOG(info) << "Nr. of channels per FEE: " << fuNrOfChannelsPerFee;

   fuNrOfChannelsPerGdpb = fuNrOfChannelsPerFee * fuNrOfFeePerGdpb;
   LOG(info) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;

   fGdpbIdIndexMap.clear();
   for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
   {
      fGdpbIdIndexMap[fUnpackPar->GetGdpbId(i)] = i;
      LOG(info) << "GDPB Id of PSD  " << i << " : " << std::hex << fUnpackPar->GetGdpbId(i)
                 << std::dec;
   } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

   /// Internal status initialization
   fvulCurrentEpoch.resize( fuNrOfGdpbs, 0 );
   fvulCurrentEpochCycle.resize( fuNrOfGdpbs, 0 );
   fvulCurrentEpochFull.resize( fuNrOfGdpbs, 0 );

   return kTRUE;
}
// -------------------------------------------------------------------------

void CbmMcbm2018MonitorAlgoPsd::AddMsComponentToList( size_t component, UShort_t usDetectorId )
{
   /// Check for duplicates and ignore if it is the case
   for( UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx )
      if( component == fvMsComponentsList[ uCompIdx ] )
         return;

   /// Add to list
   fvMsComponentsList.push_back( component );

   LOG(info) << "CbmMcbm2018MonitorAlgoPsd::AddMsComponentToList => Component "
             << component << " with detector ID 0x"
             << std::hex << usDetectorId << std::dec << " added to list";

}
// -------------------------------------------------------------------------

Bool_t CbmMcbm2018MonitorAlgoPsd::ProcessTs( const fles::Timeslice& ts )
{
   fulCurrentTsIdx = ts.index();
   fdTsStartTime = static_cast< Double_t >( ts.descriptor( 0, 0 ).idx );

   /// Ignore First TS as first MS is typically corrupt
   if( 0 == fulCurrentTsIdx )
   {
      LOG(info) << "Reseting Histos for a new run";
      ResetHistograms();
      return kTRUE;
   }

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

      /// Clear the buffer of hits

   } // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )

   /// Clear buffers to prepare for the next TS


   /// Fill plots if in monitor mode
   if( fbMonitorMode )
   {
      if( kFALSE == FillHistograms() )
      {
         LOG(error) << "Failed to fill histos in ts " << fulCurrentTsIdx;
         return kFALSE;
      } // if( kFALSE == FillHistograms() )
   } // if( fbMonitorMode )

   return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoPsd::ProcessMs( const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx )
{
   auto msDescriptor = ts.descriptor( uMsCompIdx, uMsIdx );
   fuCurrentEquipmentId = msDescriptor.eq_id;
   const uint8_t* msContent = reinterpret_cast<const uint8_t*>( ts.content( uMsCompIdx, uMsIdx ) );

   uint32_t uSize  = msDescriptor.size;
   fulCurrentMsIdx = msDescriptor.idx;

   fdMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);

   LOG(debug) << "Microslice: " << fulCurrentMsIdx
              << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
              << " has size: " << uSize;

   if( 0 == fvbMaskedComponents.size() )
      fvbMaskedComponents.resize( ts.num_components(), kFALSE );

   fuCurrDpbId  = static_cast< uint32_t >( fuCurrentEquipmentId & 0xFFFF );

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
                   << "If valid this index has to be added in the PSD parameter file in the DbpIdArray field";
         fvbMaskedComponents[ uMsCompIdx ] = kTRUE;
      } // if( kFALSE == fvbMaskedComponents[ uMsComp ] )
         else return kTRUE;

      /// Try to get it from the second message in buffer (first is epoch cycle without gDPB ID)
      /// TODO!!!!


      return kFALSE;

   } // if( it == fGdpbIdIndexMap.end() )
      else fuCurrDpbIdx = fGdpbIdIndexMap[ fuCurrDpbId ];

   /// Spill Detection
   if( 0 == fuCurrDpbIdx )
   {
      /// Check only every second
      if( 1.0 < fdMsTime - fdLastSecondTime )
      {
         /// Spill Off detection
         if( fbSpillOn && fuCountsLastSecond < kuOffSpillCountLimit )
         {
            fbSpillOn = kFALSE;
            fuCurrentSpillIdx++;
            fuCurrentSpillPlot = ( fuCurrentSpillPlot + 1 ) % kuNbSpillPlots;
            fdStartTimeSpill = fdMsTime;
            fvhChannelMapSpill[ fuCurrentSpillPlot ]->Reset();
            fhHitsPerSpill->SetBinContent(fuCurrentSpillPlot+1, 0);
         } // if( fbSpillOn && fuCountsLastSecond < kuOffSpillCountLimit )
         else if( kuOffSpillCountLimit < fuCountsLastSecond  )
            fbSpillOn = kTRUE;

         fuCountsLastSecond = 0;
         fdLastSecondTime = fdMsTime;
      } // if( 1 < fdMsTime - fdLastSecondTime )
   } // if( 0 == fuCurrDpbIdx )

   /// Save start time of first valid MS )
   if( fdStartTime < 0 )
      fdStartTime = fdMsTime;

   /// Reset the histograms if reached the end of the evolution histos range
   else if( fuHistoryHistoSize < fdMsTime - fdStartTime )
   {
      ResetHistograms();
      fdStartTime = fdMsTime;
   } // else if( fuHistoryHistoSize < fdMsTime - fdStartTime )

   // If MS time is less than start time print error and return
   if( fdMsTime - fdStartTime < 0 )
   {
      LOG(error) << "negative time! ";
      ResetHistograms();
      return kFALSE;
   }

   // If not integer number of message in input buffer, print warning/error
   if( 0 != ( uSize % kuBytesPerMessage ) )
      LOG(error) << "The input microslice buffer does NOT "
                 << "contain only complete nDPB messages!";

   // Compute the number of complete messages in the input microslice buffer
   uint32_t uNbMessages = ( uSize - ( uSize % kuBytesPerMessage ) )
                          / kuBytesPerMessage;

   // Prepare variables for the loop on contents
   const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( msContent );

   if(gLogger->IsLogNeeded( fair::Severity::debug ))
   {
      if(uNbMessages != 0) printf("\n\n");
      for( uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++ )
      {
         // Fill message
         uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);
         printf("%016llx\n", (long long int)ulData);
      }
   }

   kvuWfmRanges.clear();
   for (uint8_t i=0; i<=kuNbWfmRanges; ++i)
      kvuWfmRanges.push_back( fviHistoChargeArgs.at(1) + i*(fviHistoChargeArgs.at(2)-fviHistoChargeArgs.at(1))/kuNbWfmRanges );

   PsdData::PsdGbtReader PsdReader(pInBuff);
   if(gLogger->IsLogNeeded( fair::Severity::debug )) PsdReader.SetPrintOutMode(true);
   if(uSize > 0)
   {
      while(PsdReader.GetTotalGbtWordsRead()<uNbMessages)
      {
         int ReadResult = PsdReader.ReadEventFles();
         if(PsdReader.EvHdrAb.uHitsNumber > kuNbChanPsd)
         {
            LOG(error) << "too many triggered channels! In header: " << PsdReader.EvHdrAb.uHitsNumber << " in PSD: " << GetNbChanPsd();
            break;
         }

         if(ReadResult == 0)
         {
            fuCountsLastSecond++;
            fhAdcTimeEvo->Fill(fdMsTime - fdStartTime, PsdReader.EvHdrAc.uAdcTime);
            fuReadEvtCnt++;
            fuReadEvtCntInMs++;
            fhHitsPerSpill->AddBinContent( fuCurrentSpillPlot+1, PsdReader.EvHdrAb.uHitsNumber );
            //hit loop
            for(int hit_iter = 0; hit_iter < PsdReader.EvHdrAb.uHitsNumber; hit_iter++)
            {
               UInt_t uHitChannel = PsdReader.VectHitHdr.at(hit_iter).uHitChannel;
               UInt_t uSignalCharge = PsdReader.VectHitHdr.at(hit_iter).uSignalCharge;
               UInt_t uZeroLevel = PsdReader.VectHitHdr.at(hit_iter).uZeroLevel;
               if(uHitChannel >= kuNbChanPsd) //uHitChannel numerated from 0
               {
                  LOG(error) << "hit channel number out of range! channel index: " << uHitChannel << " max: " << GetNbChanPsd();
                  break;
               }
               //Hit header
               fhChannelMap->Fill( uHitChannel );
               fhHitChargeMap->Fill( uHitChannel, uSignalCharge );
               fvhChannelMapSpill[ fuCurrentSpillPlot ]->Fill(  uHitChannel  ); //should be placed map(channel)
               fhHitMapEvo->Fill( uHitChannel, fdMsTime - fdStartTime );
               fhChanHitMapEvo->Fill( uHitChannel, fdMsTime - fdStartTime ); //should be placed map(channel)

               fvhHitCntEvoChan[ uHitChannel ]->Fill( fdMsTime - fdStartTime );
               fvuHitCntChanMs[ uHitChannel ]++;

               fvhHitChargeChan[ uHitChannel ]->Fill( uSignalCharge );
               fvhHitZeroLevelChan[ uHitChannel ]->Fill( uZeroLevel );
               fvhHitChargeEvoChan[ uHitChannel ]->Fill( fdMsTime - fdStartTime, uSignalCharge );

               //Hit data
               uint16_t uHitAmlpitude = 0;
               uint16_t uHitChargeWfm = 0;
               fvhHitWfmChan[ uHitChannel ]->Reset();
               for(UInt_t wfm_iter = 0; wfm_iter < PsdReader.VectHitData.at(hit_iter).uWfm.size(); wfm_iter++)
               {
                  if(PsdReader.VectHitData.at(hit_iter).uWfm.at(wfm_iter) > uHitAmlpitude) uHitAmlpitude = PsdReader.VectHitData.at(hit_iter).uWfm.at(wfm_iter);
                  uHitChargeWfm += PsdReader.VectHitData.at(hit_iter).uWfm.at(wfm_iter) - uZeroLevel;
                  fvhHitWfmChan[ uHitChannel ]->Fill(wfm_iter, PsdReader.VectHitData.at(hit_iter).uWfm.at(wfm_iter));
               }
               fvhHitWfmChan[ uHitChannel ]->SetTitle(Form( "Waveform channel %03u charge %0u zero level %0u; Time [adc counts]; Amplitude [adc counts]", uHitChannel, uSignalCharge, uZeroLevel ));
               uHitAmlpitude -= uZeroLevel;
               fvhHitAmplChan[ uHitChannel ]->Fill( uHitAmlpitude );
               fvhHitChargeByWfmChan[ uHitChannel ]->Fill( uHitChargeWfm );

               for (uint8_t i=0; i<kuNbWfmRanges; ++i)
               {
                  if( uSignalCharge > kvuWfmRanges.at(i) && uSignalCharge < kvuWfmRanges.at(i+1) )
                  {
                     UInt_t uFlatIndexOfChange = i * kuNbChanPsd + uHitChannel;

		     UInt_t uWfmExampleIter = kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange);
		     UInt_t uFlatIndexHisto = uWfmExampleIter * kuNbWfmRanges * kuNbChanPsd + i * kuNbChanPsd + uHitChannel;
                     fv3hHitWfmFlattenedChan[ uFlatIndexHisto ]->Reset();

                     for(UInt_t wfm_iter = 0; wfm_iter < PsdReader.VectHitData.at(hit_iter).uWfm.size(); wfm_iter++)
                        fv3hHitWfmFlattenedChan[ uFlatIndexHisto ]->Fill(wfm_iter, PsdReader.VectHitData.at(hit_iter).uWfm.at(wfm_iter));
                     fv3hHitWfmFlattenedChan[ uFlatIndexHisto ]->SetTitle(Form( "Waveform channel %03u charge %0u zero level %0u; Time [adc counts]; Amplitude [adc counts]", uHitChannel, uSignalCharge, uZeroLevel ));

                     kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange) ++;
                     if(kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange) == kuNbWfmExamples) kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange) = 0;

		  }// if( uSignalCharge > kvuWfmRanges.at(i) && uSignalCharge < kvuWfmRanges.at(i+1) )
               }// for (uint8_t i=0; i<kuNbWfmRanges; ++i)

            } // for(int hit_iter = 0; hit_iter < PsdReader.EvHdrAb.uHitsNumber; hit_iter++)

         }
         else if(ReadResult == 1)
         {
            LOG(error) << "no event headers in message!";
            break;
         }
         else if(ReadResult == 2)
         {
            LOG(error) << "check number of waveform points! In header: " << PsdReader.HitHdr.uWfmPoints << " should be: " << 8;
            break;
         }
         else if(ReadResult == 3)
         {
            LOG(error) << "wrong amount of hits read! In header: " << PsdReader.EvHdrAb.uHitsNumber << " in hit vector: " << PsdReader.VectHitHdr.size();
            break;
         }
         else
         {
            LOG(error) << "PsdGbtReader.ReadEventFles() didn't return expected values";
            break;
         }

      }// while(PsdReader.GetTotalGbtWordsRead()<uNbMessages)

      if( uNbMessages != PsdReader.GetTotalGbtWordsRead() )
      {
         LOG(error) << "Wrong amount of messages read!"
                    << " in microslice " << uNbMessages
                    << " by PsdReader " << PsdReader.GetTotalGbtWordsRead();

         if(fbFirstPackageError)
         {
            std::ofstream error_log (Form("%llu_errorlog.txt", fulCurrentMsIdx), std::ofstream::out);
            for( uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++ )
            {
               uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);
               error_log << Form("%016llx\n", (long long int)ulData);
            }
            error_log.close();
            fbFirstPackageError = kFALSE;
            printf("Written file %llu_errorlog.txt\n", fulCurrentMsIdx);
         }
      }

      fhMsgsCntEvo->AddBinContent( fdMsTime - fdStartTime, uNbMessages );
      fhReadMsgsCntEvo->AddBinContent( fdMsTime - fdStartTime, PsdReader.GetTotalGbtWordsRead() );
      fhLostMsgsCntEvo->AddBinContent( fdMsTime - fdStartTime, uNbMessages - PsdReader.GetTotalGbtWordsRead() );
      fhReadEvtsCntEvo->AddBinContent( fdMsTime - fdStartTime, fuReadEvtCnt );

      fuMsgsCntInMs += uNbMessages;
      fuReadMsgsCntInMs += PsdReader.GetTotalGbtWordsRead();
      fuLostMsgsCntInMs += uNbMessages - PsdReader.GetTotalGbtWordsRead();

   }//if(uSize != 0)

   /// Fill histograms
   FillHistograms();

   return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoPsd::CreateHistograms()
{
   std::string sFolder = "MoniPsd";

   LOG(info) << "create Histos for PSD monitoring ";

   /// Logarithmic bining
   uint32_t iNbBinsLog = 0;
      /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
   std::vector<double> dBinsLogVector = GenerateLogBinArray( 4, 9, 1, iNbBinsLog );
   double* dBinsLog = dBinsLogVector.data();

   fhChannelMap     = new TH1I( "hChannelMap", "Map of hits in PSD detector; Chan; Hits Count []",
                           kuNbChanPsd, 0., kuNbChanPsd);
   fhHitChargeMap   = new TH2I( "hHitChargeMap", "Map of hits charges in PSD detector; Chan; Charge [adc counts]",
                           kuNbChanPsd, 0., kuNbChanPsd,
                           fviHistoChargeArgs.at(0), fviHistoChargeArgs.at(1), fviHistoChargeArgs.at(2) );
   fhHitMapEvo      = new TH2I( "hHitMapEvo", "Map of hits in PSD detector electronics vs time in run; Chan; Time in run [s]; Hits Count []",
                           kuNbChanPsd, 0., kuNbChanPsd,
                           fuHistoryHistoSize, 0, fuHistoryHistoSize );
   fhChanHitMapEvo  = new TH2I( "hChanHitMapEvo", "Map of hits in PSD detector vs time in run; Chan; Time in run [s]; Hits Count []",
                           kuNbChanPsd, 0., kuNbChanPsd,
                           fuHistoryHistoSize, 0, fuHistoryHistoSize );
   for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)
   {
      fvhChannelMapSpill.push_back( new TH1I( Form("hChannelMapSpill%02u", uSpill),
                                     Form("Map of hits in PSD detector in current spill %02u; Chan; Hits Count []", uSpill),
                              kuNbChanPsd, 0., kuNbChanPsd) );
   } // for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)
   fhHitsPerSpill   = new TH1I( "hHitsPerSpill", "Hit count per spill; Spill; Hits Count []",
                           kuNbSpillPlots, 0, kuNbSpillPlots);

   fhMsgsCntEvo     = new TH1I( "hMsgsCntEvo", "Evolution of TotalMsgs counts vs time in run; Time in run [s]; Msgs Count []",
                        fuHistoryHistoSize, 0, fuHistoryHistoSize );
   fhReadMsgsCntEvo = new TH1I( "hReadMsgsCntEvo", "Evolution of ReadMsgs counts vs time in run; Time in run [s]; ReadMsgs Count []",
                        fuHistoryHistoSize, 0, fuHistoryHistoSize );
   fhLostMsgsCntEvo = new TH1I( "hLostMsgsCntEvo", "Evolution of LostMsgs counts vs time in run; Time in run [s]; LostMsgs Count []",
                        fuHistoryHistoSize, 0, fuHistoryHistoSize );
   fhReadEvtsCntEvo = new TH1I( "hReadEvtsCntEvo", "Evolution of ReadEvents counts vs time in run; Time in run [s]; ReadEvents Count []",
                        fuHistoryHistoSize, 0, fuHistoryHistoSize );

   fhAdcTimeEvo     = new TH2I( "hAdcTimeEvo", "Evolution of ADC time vs time in run; Time in run [s]; Adc time *12.5[ns]",
                        fuHistoryHistoSize, 0, fuHistoryHistoSize, 500, 0, 9000 );

   fhMsgsCntPerMsEvo     = new TH2I( "hMsgsCntPerMsEvo",
         "Evolution of TotalMsgs counts, per MS vs time in run; Time in run [s]; TotalMsgs Count/MS []; MS",
         fuHistoryHistoSize, 0, fuHistoryHistoSize,
         iNbBinsLog, dBinsLog  );
   fhReadMsgsCntPerMsEvo = new TH2I( "ReadMsgsCntPerMsEvo",
         "Evolution of ReadMsgs counts, per MS vs time in run; Time in run [s]; ReadMsgs Count/MS []; MS",
         fuHistoryHistoSize, 0, fuHistoryHistoSize,
         iNbBinsLog, dBinsLog  );
   fhLostMsgsCntPerMsEvo = new TH2I( "hLostMsgsCntPerMsEvo",
         "Evolution of LostMsgs counts, per MS vs time in run; Time in run [s]; LostMsgs Count/MS []; MS",
         fuHistoryHistoSize, 0, fuHistoryHistoSize,
         iNbBinsLog, dBinsLog );
   fhReadEvtsCntPerMsEvo = new TH2I( "hReadEvtCntPerMsEvo",
         "Evolution of ReadEvents, per MS counts vs time in run; Time in run [s]; ReadEvents Count/MS []; MS",
         fuHistoryHistoSize, 0, fuHistoryHistoSize,
         iNbBinsLog, dBinsLog  );

   /// Add pointers to the vector with all histo for access by steering class
   AddHistoToVector( fhChannelMap, sFolder );
   AddHistoToVector( fhHitChargeMap, sFolder );
   AddHistoToVector( fhHitMapEvo, sFolder );
   AddHistoToVector( fhChanHitMapEvo, sFolder );
   for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)
      AddHistoToVector( fvhChannelMapSpill[ uSpill ], sFolder );
   AddHistoToVector( fhHitsPerSpill, sFolder );

   AddHistoToVector( fhMsgsCntEvo, sFolder );
   AddHistoToVector( fhReadMsgsCntEvo, sFolder );
   AddHistoToVector( fhLostMsgsCntEvo, sFolder );
   AddHistoToVector( fhReadEvtsCntEvo, sFolder );

   AddHistoToVector( fhAdcTimeEvo, sFolder );

   AddHistoToVector( fhMsgsCntPerMsEvo, sFolder );
   AddHistoToVector( fhReadMsgsCntPerMsEvo, sFolder );
   AddHistoToVector( fhLostMsgsCntPerMsEvo, sFolder );
   AddHistoToVector( fhReadEvtsCntPerMsEvo, sFolder );

   /*******************************************************************/
   for( UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan )
   {
      fvhHitCntEvoChan[ uChan ] = new TH1I(
            Form( "hHitCntEvoChan%03u", uChan ),
            Form( "Evolution of Hit counts vs time in run for channel %03u; Time in run [s]; Hits Count []", uChan ),
            fuHistoryHistoSize, 0, fuHistoryHistoSize );

      fvhHitCntPerMsEvoChan[ uChan ] = new TH2I(
            Form( "hHitCntPerMsEvoChan%03u", uChan ),
            Form( "Evolution of Hit counts per MS vs time in run for channel %03u; Time in run [s]; Hits Count/MS []; MS", uChan ),
            fuHistoryHistoSize, 0, fuHistoryHistoSize,
            iNbBinsLog, dBinsLog );

      fvhHitChargeChan[ uChan ] = new TH1I(
            Form( "hHitChargeChan%03u", uChan ),
            Form( "Hits charge distribution for channel %03u; Charge [adc counts]", uChan ),
            fviHistoChargeArgs.at(0), fviHistoChargeArgs.at(1), fviHistoChargeArgs.at(2));

      fvhHitZeroLevelChan[ uChan ] = new TH1I(
            Form( "hHitZeroLevelChan%03u", uChan ),
            Form( "Hits zero level distribution for channel %03u; ZeroLevel [adc counts]", uChan ),
            fviHistoZLArgs.at(0), fviHistoZLArgs.at(1), fviHistoZLArgs.at(2));

      fvhHitAmplChan[ uChan ] = new TH1I(
            Form( "hHitAmplChan%03u", uChan ),
            Form( "Hits amplitude distribution for channel %03u; Amplitude [adc counts]", uChan ),
            fviHistoAmplArgs.at(0), fviHistoAmplArgs.at(1), fviHistoAmplArgs.at(2));

      fvhHitChargeByWfmChan[ uChan ] = new TH1I(
            Form( "hHitChargeByWfmChan%03u", uChan ),
            Form( "Hits charge by waveform distribution for channel %03u; Charge [adc counts]", uChan ),
            fviHistoChargeArgs.at(0), fviHistoChargeArgs.at(1), fviHistoChargeArgs.at(2));

      fvhHitChargeEvoChan[ uChan ] = new TH2I(
            Form( "hHitChargeEvoChan%03u", uChan ),
            Form( "Evolution of Hit charge vs time in run for channel %03u; Time in run [s]; Charge [adc counts]", uChan ),
            fuHistoryHistoSize, 0, fuHistoryHistoSize, fviHistoChargeArgs.at(0), fviHistoChargeArgs.at(1), fviHistoChargeArgs.at(2) );

      fvhHitWfmChan[ uChan ] = new TH1I(
            Form( "hHitWfmChan%03u", uChan ),
            Form( "HitWfmChan%03u", uChan ),
            8, 0, 8);

      for( UInt_t uWfmRangeIter = 0; uWfmRangeIter < kuNbWfmRanges; uWfmRangeIter ++)
      {
         for( UInt_t uWfmExampleIter = 0; uWfmExampleIter < kuNbWfmExamples; uWfmExampleIter ++)
         {
            UInt_t uFlatIndex = uWfmExampleIter * kuNbWfmRanges * kuNbChanPsd + uWfmRangeIter * kuNbChanPsd + uChan;
            fv3hHitWfmFlattenedChan[ uFlatIndex ] = new TH1I(
                  Form( "hHitWfmChan%03uRange%02uExample%02u", uChan, uWfmRangeIter, uWfmExampleIter ),
                  Form( "HitWfmChan%03uRange%02uExample%02u", uChan, uWfmRangeIter, uWfmExampleIter ),
                  8, 0, 8);

         }// for( UInt_t uWfmRangeIter = 0; uWfmRangeIter < kuNbWfmRanges; uWfmRangeIter ++)
      } // for( UInt_t uWfmExampleIter = 0; uWfmExampleIter < kuNbWfmExamples; uWfmExampleIter ++)


      /// Add pointers to the vector with all histo for access by steering class
      AddHistoToVector( fvhHitCntEvoChan[ uChan ], sFolder );
      AddHistoToVector( fvhHitCntPerMsEvoChan[ uChan ], sFolder );
      AddHistoToVector( fvhHitChargeChan[ uChan ], sFolder );
      AddHistoToVector( fvhHitZeroLevelChan[ uChan ], sFolder );
      AddHistoToVector( fvhHitAmplChan[ uChan ], sFolder );
      AddHistoToVector( fvhHitChargeByWfmChan[ uChan ], sFolder );
      AddHistoToVector( fvhHitChargeEvoChan[ uChan ], sFolder );

   } // for( UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan )

   /// Cleanup array of log bins
//   delete dBinsLog;

   /*******************************************************************/

   /// Canvases
   Double_t w = 10;
   Double_t h = 10;

   /*******************************************************************/
   /// Map of hits over PSD detector and same vs time in run
   fcHitMaps = new TCanvas( "cHitMaps", "Hit maps", w, h);
   fcHitMaps->Divide( 2 );

   fcHitMaps->cd( 1 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   fhChannelMap->Draw();

   fcHitMaps->cd( 2 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhChanHitMapEvo->Draw( "colz" );

   fcHitMaps->cd( 3 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhHitChargeMap->Draw("colz");

   AddCanvasToVector( fcHitMaps, "canvases" );
   /*******************************************************************/

   /*******************************************************************/
   /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
   fcSummary = new TCanvas( "cSummary", "Hit maps, Hit rate, Error fraction", w, h);
   fcSummary->Divide( 2, 2 );

   fcSummary->cd( 1 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   fhChannelMap->Draw();

   fcSummary->cd( 2 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhChanHitMapEvo->Draw( "colz" );

   fcSummary->cd( 3 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhHitChargeMap->Draw("colz");

   AddCanvasToVector( fcSummary, "canvases" );
   /*******************************************************************/

   /*******************************************************************/
   /// Charge from FPGA all channels
   fcChargesFPGA = new TCanvas( "cChargesFPGA", "Charges spectra in all channels calculated by FPGA", w, h);
   fcChargesFPGA->DivideSquare( kuNbChanPsd );

   for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)
   {
      fcChargesFPGA->cd( 1 + uChan );
      fvhHitChargeChan[ uChan ]->Draw();
   } // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

   AddCanvasToVector( fcChargesFPGA, "canvases" );
   /*******************************************************************/

   /*******************************************************************/
   /// Charge from Waveform all channels
   fcChargesWfm = new TCanvas( "cChargesWfm", "Charges spectra in all channels calculated over waveform", w, h);
   fcChargesWfm->DivideSquare( kuNbChanPsd );

   for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)
   {
      fcChargesWfm->cd( 1 + uChan );
      fvhHitChargeByWfmChan[ uChan ]->Draw();
   } // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

   AddCanvasToVector( fcChargesWfm, "canvases" );
   /*******************************************************************/

   /*******************************************************************/
   /// Amplitudes all channels
   fcAmplitudes = new TCanvas( "cAmplitudes", "Amplitude spectra in all channels", w, h);
   fcAmplitudes->DivideSquare( kuNbChanPsd );

   for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)
   {
      fcAmplitudes->cd( 1 + uChan );
      fvhHitAmplChan[ uChan ]->Draw();
   } // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

   AddCanvasToVector( fcAmplitudes, "canvases" );
   /*******************************************************************/

   /*******************************************************************/
   /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
   fcGenCntsPerMs = new TCanvas( "cGenCntsPerMs", "Messages and hit cnt per MS, Error and Evt Loss Fract. per MS ", w, h);
   fcGenCntsPerMs->Divide( 2, 2 );

   fcGenCntsPerMs->cd( 1 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   gPad->SetLogz();
   fhMsgsCntPerMsEvo->Draw( "colz" );

   fcGenCntsPerMs->cd( 2 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   gPad->SetLogz();
   fhReadMsgsCntPerMsEvo->Draw( "colz" );

   fcGenCntsPerMs->cd( 3 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   gPad->SetLogz();
   fhReadEvtsCntPerMsEvo->Draw( "colz" );

   fcGenCntsPerMs->cd( 4 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   gPad->SetLogz();
   fhLostMsgsCntPerMsEvo->Draw( "colz" );

   AddCanvasToVector( fcGenCntsPerMs, "canvases" );
   /*******************************************************************/

   /*******************************************************************/
   /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
   fcSpillCounts = new TCanvas( "cSpillCounts", "Counts per spill, last 5 spills including current one", w, h);
   fcSpillCounts->Divide( 1, kuNbSpillPlots );

   for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)
   {
      fcSpillCounts->cd( 1 + uSpill );
      gPad->SetGridx();
      gPad->SetGridy();
      fvhChannelMapSpill[ uSpill ]->Draw();
   } // for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)

   AddCanvasToVector( fcSpillCounts, "canvases" );
   /*******************************************************************/

   /*******************************************************************/
   /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
   fcWfmsAllChannels = new TCanvas( "cWfmsAllChannels", "Last waveforms in PSD fired channels", w, h);
   fcWfmsAllChannels->DivideSquare( kuNbChanPsd );

   for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)
   {
      fcWfmsAllChannels->cd( 1 + uChan );
      fvhHitWfmChan[ uChan ]->Draw("HIST LP");
   } // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

   AddCanvasToVector( fcWfmsAllChannels, "waveforms" );
   /*******************************************************************/

   /*******************************************************************/
   /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
   for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)
   {
      fvcWfmsChan[ uChan ] = new TCanvas( Form("cWfmsChan%03u", uChan), Form("Canvas with last waveforms in channel %03u", uChan), w, h);
      fvcWfmsChan[ uChan ]->Divide( kuNbWfmExamples, kuNbWfmRanges );
      UInt_t uHisto = 0;

      for( UInt_t uWfmRangeIter = 0; uWfmRangeIter < kuNbWfmRanges; uWfmRangeIter ++)
      {
         for( UInt_t uWfmExampleIter = 0; uWfmExampleIter < kuNbWfmExamples; uWfmExampleIter ++)
         {
            fvcWfmsChan[ uChan ]->cd( 1 + uHisto );
            UInt_t uFlatIndex = uWfmExampleIter * kuNbWfmRanges * kuNbChanPsd + uWfmRangeIter * kuNbChanPsd + uChan;
            fv3hHitWfmFlattenedChan[ uFlatIndex ]->Draw("HIST LP");
            uHisto++;
         }// for( UInt_t uWfmRangeIter = 0; uWfmRangeIter < kuNbWfmRanges; uWfmRangeIter ++)
      } // for( UInt_t uWfmExampleIter = 0; uWfmExampleIter < kuNbWfmExamples; uWfmExampleIter ++)

      AddCanvasToVector( fvcWfmsChan[ uChan ], "waveforms" );
   }// for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

   /*******************************************************************/

   return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoPsd::FillHistograms()
{

   for( UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan )
   {
      fvhHitCntPerMsEvoChan[       uChan ]->Fill( fdMsTime - fdStartTime, fvuHitCntChanMs[ uChan ] );
      fvuHitCntChanMs[ uChan ] = 0;

   } // for( UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan )

   fhMsgsCntPerMsEvo->Fill(     fdMsTime - fdStartTime, fuMsgsCntInMs );
   fhReadMsgsCntPerMsEvo->Fill( fdMsTime - fdStartTime, fuReadMsgsCntInMs );
   fhLostMsgsCntPerMsEvo->Fill( fdMsTime - fdStartTime, fuLostMsgsCntInMs );
   fhReadEvtsCntPerMsEvo->Fill( fdMsTime - fdStartTime, fuReadEvtCntInMs );
   fuMsgsCntInMs = 0;
   fuReadMsgsCntInMs = 0;
   fuLostMsgsCntInMs = 0;
   fuReadEvtCntInMs = 0;

   return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoPsd::ResetHistograms()
{
   for( UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan )
   {
      fvhHitCntEvoChan[ uChan ]->Reset();
      fvhHitCntPerMsEvoChan[ uChan ]->Reset();
      fvhHitChargeChan[ uChan ]->Reset();
      fvhHitZeroLevelChan[ uChan ]->Reset();
      fvhHitAmplChan[ uChan ]->Reset();
      fvhHitChargeByWfmChan[ uChan ]->Reset();
      fvhHitChargeEvoChan[ uChan ]->Reset();
      fvhHitWfmChan[ uChan ]->Reset();
   } // for( UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan )

   for( UInt_t uFlatIndex = 0; uFlatIndex < kuNbChanPsd*kuNbWfmRanges*kuNbWfmExamples; ++uFlatIndex )
      fv3hHitWfmFlattenedChan[ uFlatIndex ]->Reset();
   for( UInt_t uWfmIndex = 0; uWfmIndex < kuNbChanPsd*kuNbWfmRanges; ++uWfmIndex )
      kvuWfmInRangeToChangeChan[ uWfmIndex ] = 0;

   fuCurrentSpillIdx = 0;
   fuCurrentSpillPlot = 0;
   fhChannelMap->Reset();
   fhHitChargeMap->Reset();
   fhHitMapEvo->Reset();
   fhChanHitMapEvo->Reset();
   for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)
      fvhChannelMapSpill[ uSpill ]->Reset();
   fhHitsPerSpill->Reset();

   fhMsgsCntEvo->Reset();
   fhReadMsgsCntEvo->Reset();
   fhLostMsgsCntEvo->Reset();
   fhReadEvtsCntEvo->Reset();

   fhAdcTimeEvo->Reset();

   fhMsgsCntPerMsEvo->Reset();
   fhReadMsgsCntPerMsEvo->Reset();
   fhLostMsgsCntPerMsEvo->Reset();
   fhReadEvtsCntPerMsEvo->Reset();

   /// Also reset the Start time for the evolution plots!
   fdStartTime = -1.0;


   return kTRUE;
}

// -------------------------------------------------------------------------
