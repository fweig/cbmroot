/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmCriGet4RawPrint.h"

#include "CbmFormatMsHeaderPrintout.h"

#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include <iomanip>

CbmCriGet4RawPrint::CbmCriGet4RawPrint()
  : CbmMcbmUnpack()
  , fvMsComponentsList()
  , fuNbCoreMsPerTs(0)
  , fuNbOverMsPerTs(0)
  , fuNbMsLoop(0)
  , fbIgnoreOverlapMs(kFALSE)
  , fdMsSizeInNs(-1.0)
  , fdTsCoreSizeInNs(-1.0)
  , fulCurrentTsIdx(0)
  , fulCurrentMsIdx(0)
  , fdTsStartTime(0.0)
  , fdTsStopTimeCore(0.0)
  , fdMsTime(0.0)
  , fuMsIndex(0)
  , fuCurrentEquipmentId(0)
{
}

CbmCriGet4RawPrint::~CbmCriGet4RawPrint() {}

Bool_t CbmCriGet4RawPrint::Init()
{
  LOG(info) << "CbmCriGet4RawPrint::Init";
  LOG(info) << "Initializing mCBM gDPB 2018 Raw Messages Converter";

  return kTRUE;
}

void CbmCriGet4RawPrint::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();
}

Bool_t CbmCriGet4RawPrint::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();

  return kTRUE;
}

Bool_t CbmCriGet4RawPrint::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();

  return kTRUE;
}

void CbmCriGet4RawPrint::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  LOG(info) << "CbmCriGet4RawPrint::AddMsComponentToList => Component " << component << " with detector ID 0x"
            << std::hex << usDetectorId << std::dec << " added to list";
}

Bool_t CbmCriGet4RawPrint::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  
  static const uint8_t NGET4 = 80;
  static const uint8_t NERROR = 0x16;
  
  char buf[256];
  
  static uint32_t lastGlobalEpoch = 0; 
  uint32_t nGet4, epoch, msgType, errorCode; 
  static int32_t pEpochDiff[NGET4];
  int32_t epochDiff;
  static uint32_t pErrorCnt[NGET4]={ 0 };
  static uint32_t pHitsCnt[NGET4]={ 0 };
  static uint32_t pTotCnt[NGET4]={ 0 };
  
  static uint32_t pErrorCntMatrix[NGET4][NERROR];
  
  static uint32_t procEpochUntilError = 0;
   
  fulCurrentTsIdx = ts.index();
  fdTsStartTime   = static_cast<Double_t>(ts.descriptor(0, 0).idx);

  /// Ignore First TS as first MS is typically corrupt
  if (0 == fulCurrentTsIdx) { return kTRUE; }  // if( 0 == fulCurrentTsIdx )

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsFullSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs + fuNbOverMsPerTs);
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a core duration of " << fdTsCoreSizeInNs << " ns and a full duration of "
              << fdTsFullSizeInNs << " ns";

    /// Ignore overlap ms if flag set by user
    fuNbMsLoop = fuNbCoreMsPerTs;
    if (kFALSE == fbIgnoreOverlapMs) fuNbMsLoop += fuNbOverMsPerTs;
    LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
  }  // if( -1.0 == fdTsCoreSizeInNs )

  /// Compute time of TS core end
  fdTsStopTimeCore = fdTsStartTime + fdTsCoreSizeInNs;

  /// Loop over registered components
  for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx) {
    /// Loop over core microslices (and overlap ones if chosen)
    for (fuMsIndex = 0; fuMsIndex < fuNbMsLoop; fuMsIndex++) {
      UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];

      auto msDescriptor        = ts.descriptor(uMsComp, fuMsIndex);
      fuCurrentEquipmentId     = msDescriptor.eq_id;
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsComp, fuMsIndex));

      uint32_t uSize  = msDescriptor.size;
      fulCurrentMsIdx = msDescriptor.idx;
      //         Double_t dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
      LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
                 << " has size: " << uSize;

      /// Always print the MS header
      LOG(info) << "---------------------------------------------------------------";
      LOG(info) << FormatMsHeaderPrintout(msDescriptor);

      /// If not integer number of message in input buffer, print warning/error
      if (0 != (uSize % kuBytesPerMessage))
        LOG(error) << "The input microslice buffer does NOT "
                   << "contain only complete gDPB messages!";

      /// Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages = (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

      /// Prepare variables for the loop on contents
      const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);
      for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
        /// Fill message
        uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);

        critof001::Message mess(ulData);

        /// TODOD: pick the options!!!
        //mess.printDataCout( critof001::msg_print_Hex );
        //mess.printDataCout( critof001::msg_print_Hex | critof001::msg_print_Prefix | critof001::msg_print_Data );
        
        msgType = ulData & 0xF;
        nGet4 = (ulData>>40) & 0xFF;
        epoch = (ulData>>8) & 0xFFFFFF;
		epoch &= 0xFFFFFF;
		errorCode = (ulData>>4) & 0x7F;
		/*snprintf(buf, sizeof(buf),
                 "Data: 0x%016lx - %d - 0x06%X ",
                 ulData, nGet4, epoch);
                 
				std::cout << buf << std::endl;
		*/
		
		//if (fuCurrentEquipmentId == 0xabc0)
		{
			//------------------- TLAST ----------------------------//
			if ((ulData & 0xFFFFFFFFFFFF)==0xdeadbeeeeeef)		
			{
			}
			//------------------- EPOCH ----------------------------//
			else if (msgType == 0x01)
			{
				if (nGet4 == 0xFF) {
					
					procEpochUntilError++;
					if (lastGlobalEpoch!=0xFFFFFF){
						if ((lastGlobalEpoch + 1) != epoch){
							snprintf(buf, sizeof(buf),
							 "Error global epoch, last epoch, current epoch, diff  0x%06x 0x%06x %d 0x%016lx %d",
							 lastGlobalEpoch, epoch, lastGlobalEpoch - epoch, ulData, procEpochUntilError);
							 
							 std::cout << buf << std::endl;
							 procEpochUntilError=0;
						}
					}
					else{
						snprintf(buf, sizeof(buf),
							 "Global epoch overflow, last epoch, current epoch  0x%06x 0x%06x",
							 lastGlobalEpoch, epoch);
							 
							 std::cout << buf << std::endl;				
					}
					
					
					lastGlobalEpoch = epoch;	 
					snprintf(buf, sizeof(buf),"Global epoch %d",epoch);							 
					std::cout << buf << std::endl;
							 
				
				}
				else if (nGet4 <= 120){
					
					if (lastGlobalEpoch > epoch)
						epochDiff = lastGlobalEpoch - epoch;
					else
						epochDiff = 0xFFFFFF + lastGlobalEpoch - epoch;
					
					if (epochDiff != pEpochDiff[nGet4]){
						snprintf(buf, sizeof(buf),
						 "eTime %d - Error epoch drift Get4 %3d , last epoch diff, current epoch  diff  0x%06x 0x%06x %d",
						 lastGlobalEpoch, nGet4, pEpochDiff[nGet4], epochDiff, pEpochDiff[nGet4]-epochDiff);
						 std::cout << buf << std::endl;
						 mess.printDataCout( critof001::msg_print_Hex | critof001::msg_print_Prefix | critof001::msg_print_Data );
						 
					}
					pEpochDiff[nGet4] = epochDiff;
					
				}
			}
			/*
			//------------------- CTRL ----------------------------//
			else if (msgType == 0x02)
			{
				snprintf(buf, sizeof(buf),
				 "eTime %d - Ctrl Msg: 0x%016lx, Get4 %3d" ,
				 lastGlobalEpoch, ulData, nGet4);
				std::cout << buf << std::endl;
				mess.printDataCout( critof001::msg_print_Hex | critof001::msg_print_Prefix | critof001::msg_print_Data );
			}
			//------------------- ERROR ----------------------------//
			else if (msgType == 0x03)
			{
				snprintf(buf, sizeof(buf),
				 "eTime %d - Error Msg: 0x%016lx, Get4 %3d, Error code 0x%02x",
				 lastGlobalEpoch, ulData, nGet4, errorCode);
				std::cout << buf << std::endl;
				mess.printDataCout( critof001::msg_print_Hex | critof001::msg_print_Prefix | critof001::msg_print_Data );
				
				if (nGet4 < NGET4)
					pErrorCnt[nGet4] = pErrorCnt[nGet4] +1;
				if (errorCode ==  0x12)
					if (nGet4 < NGET4)
						pTotCnt[nGet4]=pTotCnt[nGet4]+1;
				if ( (nGet4 < NGET4) && (errorCode<NERROR) )
					pErrorCntMatrix[nGet4][errorCode]=pErrorCntMatrix[nGet4][errorCode]+1;
				
			}
			//------------------- HITS ----------------------------//
			else if (msgType == 0x0)
			{
				snprintf(buf, sizeof(buf),
				 "eTime %d - Hit Msg: 0x%016lx, Get4 %3d",
				 lastGlobalEpoch, ulData, nGet4);
				std::cout << buf << std::endl;
				mess.printDataCout( critof001::msg_print_Hex | critof001::msg_print_Prefix | critof001::msg_print_Data );
				if (nGet4 < NGET4)
					pHitsCnt[nGet4]=pHitsCnt[nGet4]+1;
			}
			 */
			/*snprintf(buf, sizeof(buf),
					 "Data: 0x%016lx",
					 ulData);
					 
			std::cout << buf << std::endl;*/
		}	
        
      }  // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
    }    // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )
  }      // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )

  if (0 == fulCurrentTsIdx % 10000) LOG(info) << "Processed TS " << fulCurrentTsIdx;

	/*
	uint32_t nPulses = 4*10000;
	float effi;
	for(uint32_t i =0; i < NGET4 ; i++)
	{	
		effi = float(pHitsCnt[i])/float(nPulses) * 100.0;
		snprintf(buf, sizeof(buf),
			 "Hit counter %d: Hits: %d Errors: %d ErrTot: %d Effi: %f",
			 i, pHitsCnt[i], pErrorCnt[i], pTotCnt[i], effi);
			std::cout << buf << std::endl;
		for(uint32_t j = 0; j<NERROR ; j++){
			if (pErrorCntMatrix[i][j] != 0){
				snprintf(buf, sizeof(buf),
					"Error counter %d: error code: 0x%02x, times: %d",
				 i, j, pErrorCntMatrix[i][j]);
				std::cout << buf << std::endl;
			}
		}
	 }
	 */ 
  

  return kTRUE;
}

void CbmCriGet4RawPrint::Reset()
{
}

void CbmCriGet4RawPrint::Finish() {}

ClassImp(CbmCriGet4RawPrint)
