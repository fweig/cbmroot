/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

void CheckTimingSis18(std::string sDigiFileName, std::string sEventFileName = "",
                      std::string sTimingFilesFolder = "/lustre/cbm/prod/beamtime/2022/mcbm_sis18_events/",
                      bool bVerbose                  = false)
{
  std::vector<std::string> vTimingFileNames = {
    "sis18_events_2022_03_24.txt",    "sis18_events_2022_03_29.txt",   "sis18_events_2022_03_30.txt",
    "sis18_events_2022_03_31.txt",    "sis18_events_2022_04_01.txt",   "sis18_events_2022_04_02.txt",
    "sis18_events_2022_04_03.txt",    "sis18_events_2022_04_04.txt",   "sis18_events_2022_05_18.txt",
    "sis18_events_2022_05_23_24.txt", "sis18_events_2022_05_25_26.txt"};

  /// Test the decoder
  /*
  std::string testStr = "Planned UTC:  1648763972022760000 TAI:  1648764009022760000 Raw: 0x112c0338003005c0 "
                        "0x0000140000000000 0x0000 exec UTC:  1648763972022760000 TAI:  1648764009022760000 "
                        "=> EVT_EXTR_END        2022-03-31 22:00:09.022760000";
  std::cout << testStr << std::endl;
  AccTimingEvent tester(testStr);
  */

  std::vector<AccTimingEvent> vAllEvtsBuff = {};

  /// Loop on Timing files
  for (uint32_t uFile = 0; uFile < vTimingFileNames.size(); ++uFile) {
    /// Open File
    std::string sFullPath = sTimingFilesFolder + "/" + vTimingFileNames[uFile];
    std::ifstream ifsRaw(sFullPath);

    if (ifsRaw.is_open()) {
      /// Read it line by line
      std::string sLine;
      while (std::getline(ifsRaw, sLine)) {
        /// Convert each line to an event, if not empty
        if ("\n" != sLine) {
          if (bVerbose) std::cout << sLine << std::endl;
          AccTimingEvent newEvent(sLine);
          vAllEvtsBuff.push_back(newEvent);
        }
      }
      std::cout << "File " << sFullPath << " done" << std::endl;
    }
    else {
      std::cout << "File " << sFullPath << " could not be open!" << std::endl;
      return;
    }
  }
  std::cout << "Total events in all recorded files: " << vAllEvtsBuff.size() << std::endl;

  /// Open the digi file in update mode
  TFile* pFile = new TFile(sDigiFileName.data(), "READ");
  if (kTRUE == pFile->IsZombie()) {
    std::cout << "Digi file " << sDigiFileName << " could not be open!" << std::endl;
    return;
  }
  gROOT->cd();

  /// Access the tree in the file
  TTree* pTree = dynamic_cast<TTree*>(pFile->Get("cbmsim"));
  if (nullptr == pTree) {
    std::cout << "Could not find the cbm tree in the digi file" << std::endl;
    pFile->Close();
    return;
  }
  uint32_t uNbTs = pTree->GetEntries();
  if (0 == uNbTs) {
    std::cout << "No entries in the Tree found in the digi file!" << std::endl;
    pFile->Close();
    return;
  }

  /// Access the header of the last TS
  CbmTsEventHeader* pEventHeader = nullptr;
  pTree->SetBranchAddress("EventHeader.", &pEventHeader);
  pTree->GetEntry(uNbTs - 1);
  if (nullptr == pEventHeader) {
    std::cout << "Could not find the last TS Event header in the digi file" << std::endl;
    pFile->Close();
    return;
  }
  /// Get last start time and check that it is later than the first event in the buffer
  uint64_t ulLastTsStartUtc = pEventHeader->GetTsStartTime();
  if (ulLastTsStartUtc < vAllEvtsBuff[0].GetTime()) {
    std::cout << "Last TS start time in this file: " << ulLastTsStartUtc << std::endl;
    std::cout << "Earlier than the first recorded accelerator Event: " << vAllEvtsBuff[0].GetTime() << std::endl;
    std::cout << "=> No accelerator data available for this digi file!" << std::endl;
    return;
  }

  /// Access the header of the first TS
  pTree->GetEntry(0);
  if (nullptr == pEventHeader) {
    std::cout << "Could not find the first TS Event header in the digi file" << std::endl;
    pFile->Close();
    return;
  }

  /// Get first start time and search for the closest past event in the buffer
  uint64_t ulFirstTsStartUtc = pEventHeader->GetTsStartTime();
  std::cout << "First TS start time: " << ulFirstTsStartUtc << std::endl;

  std::vector<AccTimingEvent>::iterator itEvt =
    std::upper_bound(vAllEvtsBuff.begin(), vAllEvtsBuff.end(), ulFirstTsStartUtc);
  if (vAllEvtsBuff.end() == itEvt) {
    std::cout << "No Events after the start of the first TS => no accelerator data available for this run!"
              << std::endl;
    pFile->Close();
    return;
  }
  /*
  if (itEvt->IsCycleEnd()) {
    if (itEvt != vAllEvtsBuff.begin()) {
      itEvt--; // not at end of vector so rewind to previous item
    }
    else {
      std::cout << "No Events before the start of the first TS & next event is the end of a cycle "
                << "=> unknown state, not handled in current implementation!"
                << std::endl;
      pFile->Close();
      return;
    }
  }
  */
  std::cout << "Found an event before the first TS start time:" << std::endl;
  itEvt->Print();

  /// Create a list of spills within the digi file
  bool bExtractionOn = false;
  bool bCycleOn      = false;
  uint32_t uSpillIdx = 0;
  uint32_t uCycleIdx = 0;
  if (itEvt->IsExtractionEnd()) {
    /// We start in the middle of an extraction spill
    bExtractionOn = true;
    std::cout << "Spill ongoing when starting at " << ulFirstTsStartUtc << " (" << uSpillIdx << ") " << std::endl;
  }
  if (!itEvt->IsCycleStart()) {
    /// We start within a spill cycle (not in short interval between cycle end and cycle start))
    bCycleOn = true;
  }
  while ((vAllEvtsBuff.end() != itEvt) && itEvt->GetTime() <= ulLastTsStartUtc) {
    if (itEvt->IsCycleStart()) {
      //
      uCycleIdx++;
      bCycleOn = true;
    }
    else if (itEvt->IsCycleEnd()) {
      //
      bCycleOn = false;
    }
    else if (itEvt->IsExtractionStart()) {
      //
      uSpillIdx++;
      bExtractionOn = true;
      std::cout << "Spill starting at              " << itEvt->GetTime() << " (" << uSpillIdx << ") " << std::endl;
    }
    else if (itEvt->IsExtractionEnd()) {
      //
      bExtractionOn = false;
      std::cout << "Spill end at                   " << itEvt->GetTime() << std::endl;
    }

    itEvt++;
  }
  std::cout << "Number of spill found for this digi file: " << (1 + uSpillIdx) << std::endl;

  /*
   * TODO: decide what is saved in the digi file
   * => Should we try looking for the first spill start before the beginning of the first TS?
   * => Should we save only the start and stop of the spills? or also the index within the file?
   * => Should we save the spill times in UTC or in ref to first TS time?
   */
  /*
   * FIXME: not compatible with usage of timeslice overlap as relying on "end of TS = start of next TS"
   * => Need addin the limits of the timeslice core and timeslice overlap in the TsEventHeader class (from TsMetaData)
   * => Need to add to the RecoUnpack a determination of these parameters from the boundaries of the microslices and
   *    microslices numbers on component
   */
  if (uNbTs < 2) {
    std::cout << "Only 1 TS in the digi file, cannot determine the TS duration" << std::endl;
    pFile->Close();
    return;
  }
  pTree->GetEntry(1);
  uint64_t ulTsDuration = pEventHeader->GetTsStartTime() - ulFirstTsStartUtc;

  /// Prepare histograms
  // clang-format off
  gROOT->cd();
  uint64_t uNsToSec  = 1000000000;
  uint64_t uSecToBin = 1000000; // 0.001 ms bin
//  uint64_t uSecToBin = 50000; // 0.02 ms bin
//  uint64_t uSecToBin = 10000; // 0.1 ms bin
//  uint64_t uSecToBin = 1000; // 1 ms bin
  uint64_t uBegTime  = (ulFirstTsStartUtc / uNsToSec);
  uint64_t uEndTime  = ((ulLastTsStartUtc + ulTsDuration) / uNsToSec);
  uint64_t uNbBins   = uSecToBin * (uEndTime - uBegTime + 2);

  TH1* phSpillOnOff  = new TH1I("hSpillOnOff",  "Spill ON/OFF vs time in run; Time in Run [s]",
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phSpillIndex  = new TH1I("hSpillIndex",  "Spill Index vs time in run; Time in Run [s]",
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsBmon  = new TH1I("hCountsBmon",
                                Form("BMon digis per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsSts   = new TH1I("hCountsSts",
                                Form("STS digis per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsMuch  = new TH1I("hCountsMuch",
                                Form("MUCH digis per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsTrd1d = new TH1I("hCountsTrd1d",
                                Form("TRD 1D digis per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsTrd2d = new TH1I("hCountsTrd2d",
                                Form("TRD 1D digis per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsTof   = new TH1I("hCountsTof",
                                Form("TOF digis per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsRich  = new TH1I("hCountsRich",
                                Form("RICH digis per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsEvts  = new TH1I("hCountsEvts",
                                Form("Events per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);

  TH1* phCountsEvtBmon  = new TH1I("hCountsEvtBmon",
                                Form("BMon digis in evts per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsEvtSts   = new TH1I("hCountsEvtSts",
                                Form("STS digis in evts per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsEvtMuch  = new TH1I("hCountsEvtMuch",
                                Form("MUCH digis in evts per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsEvtTrd1d = new TH1I("hCountsEvtTrd1d",
                                Form("TRD 1D digis in evts per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsEvtTrd2d = new TH1I("hCountsEvtTrd2d",
                                Form("TRD 1D digis in evts per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsEvtTof   = new TH1I("hCountsEvtTof",
                                Form("TOF digis in evts per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phCountsEvtRich  = new TH1I("hCountsEvtRich",
                                Form("RICH digis in evts per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);

  TH1* phSelRatioBmon  = new TH1D("hSelRatioBmon",
                                Form("BMon ratio of digis sel in evt per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phSelRatioSts   = new TH1D("hSelRatioSts",
                                Form("STS ratio of digis sel in evt per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phSelRatioMuch  = new TH1D("hSelRatioMuch",
                                Form("MUCH ratio of digis sel in evt per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phSelRatioTrd1d = new TH1D("hSelRatioTrd1d",
                                Form("TRD 1D ratio of digis sel in evt per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phSelRatioTrd2d = new TH1D("hSelRatioTrd2d",
                                Form("TRD 1D ratio of digis sel in evt per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phSelRatioTof   = new TH1D("hSelRatioTof",
                                Form("TOF ratio of digis sel in evt per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  TH1* phSelRatioRich  = new TH1D("hSelRatioRich",
                                Form("RICH ratio of digis sel in evt per %5.3f ms vs time in run; Time in Run [s]", 1000./uSecToBin),
                                uNbBins, -1, (uEndTime - uBegTime) + 1);
  // clang-format on

  /// Prepare branches accessors
  std::vector<CbmTofDigi>* pVectBmon  = nullptr;
  std::vector<CbmStsDigi>* pVectSts   = nullptr;
  std::vector<CbmMuchDigi>* pVectMuch = nullptr;
  std::vector<CbmTrdDigi>* pVectTrd   = nullptr;
  std::vector<CbmTofDigi>* pVectTof   = nullptr;
  std::vector<CbmRichDigi>* pVectRich = nullptr;
  TClonesArray* pArrayEvents          = new TClonesArray("CbmEvent", 100);

  pTree->SetBranchAddress("T0Digi", &pVectBmon);
  pTree->SetBranchAddress("StsDigi", &pVectSts);
  pTree->SetBranchAddress("MuchDigi", &pVectMuch);
  pTree->SetBranchAddress("TrdDigi", &pVectTrd);
  pTree->SetBranchAddress("TofDigi", &pVectTof);
  pTree->SetBranchAddress("RichDigi", &pVectRich);
  pTree->SetBranchAddress("CbmEvent", &pArrayEvents);

  /// Fill for each TS data count histograms for each detector + Spill state + Spill Index + Events counts
  AccStatusTs statusTs;
  std::vector<AccTimingEvent>::iterator itFirstEventPrevTs = vAllEvtsBuff.begin();
  std::vector<AccTimingEvent>::iterator itLastEventPrevTs  = vAllEvtsBuff.begin();
  uSpillIdx                                                = 0;
  bExtractionOn                                            = false;
  uint64_t uTimeCurrSpillPnt                               = uBegTime * uNsToSec - uNsToSec;
  uint64_t ulTsTimeInRun                                   = 0;
  for (uint32_t uTs = 0; uTs < uNbTs; ++uTs) {
    std::cout << Form("TS %5u / %5u", uTs, uNbTs) << std::endl;

    pTree->GetEntry(uTs);

    /// Find the first event after the start of this TS
    uint64_t ulTsStartUtc = pEventHeader->GetTsStartTime();
    std::vector<AccTimingEvent>::iterator itEvtPrev =
      std::upper_bound(itFirstEventPrevTs, vAllEvtsBuff.end(), ulTsStartUtc);

    /// Loop on events between last TS and the current one to count potentially not recorded spills
    for (std::vector<AccTimingEvent>::iterator it = itLastEventPrevTs; it != itEvtPrev; ++it) {
      if (it->IsExtractionStart()) {
        //
        bExtractionOn = true;
        uSpillIdx++;
        while (uTimeCurrSpillPnt <= it->GetTime()) {
          double dBinTime = (uTimeCurrSpillPnt - ulFirstTsStartUtc) / double(uNsToSec);
          if (-1. <= dBinTime) {
            //
            phSpillOnOff->SetBinContent(phSpillOnOff->FindBin(dBinTime), 0);
          }
          uTimeCurrSpillPnt += uNsToSec / uSecToBin;
        }
      }
      else if (it->IsExtractionEnd()) {
        bExtractionOn = false;
        while (uTimeCurrSpillPnt <= it->GetTime()) {
          double dBinTime = (uTimeCurrSpillPnt - ulFirstTsStartUtc) / double(uNsToSec);
          if (-1. <= dBinTime) {
            //
            phSpillOnOff->SetBinContent(phSpillOnOff->FindBin(dBinTime), 10000);
          }
          uTimeCurrSpillPnt += uNsToSec / uSecToBin;
        }
      }
    }
    std::cout << " A " << std::flush;

    /// Check the last event before the TS
    if (vAllEvtsBuff.begin() == itEvtPrev) {
      std::cout << "No Events before the start of the first TS "
                << "=> not possible to do a full list of spill status for this digi file!" << std::endl;
      pFile->Close();
      return;
    }
    itEvtPrev--;
    std::cout << Form(" %lu %lu %lu ", uTimeCurrSpillPnt, itEvtPrev->GetTime(), ulFirstTsStartUtc) << std::flush;
    if (itEvtPrev->IsExtractionStart()) {
      //
      bExtractionOn = true;
      uSpillIdx++;
      while (uTimeCurrSpillPnt <= itEvtPrev->GetTime()) {
        double dBinTime = (uTimeCurrSpillPnt - ulFirstTsStartUtc) / double(uNsToSec);
        if (-1. <= dBinTime) {
          //
          phSpillOnOff->SetBinContent(phSpillOnOff->FindBin(dBinTime), 0);
        }
        uTimeCurrSpillPnt += uNsToSec / uSecToBin;
      }
    }
    else if (itEvt->IsExtractionEnd()) {
      bExtractionOn = false;
      while (uTimeCurrSpillPnt <= itEvtPrev->GetTime()) {
        double dBinTime = (uTimeCurrSpillPnt - ulFirstTsStartUtc) / double(uNsToSec);
        if (-1. <= dBinTime) {
          //
          phSpillOnOff->SetBinContent(phSpillOnOff->FindBin(dBinTime), 10000);
        }
        uTimeCurrSpillPnt += uNsToSec / uSecToBin;
      }
    }
    else {
      while (uTimeCurrSpillPnt <= itEvtPrev->GetTime()) {
        uTimeCurrSpillPnt += uNsToSec / uSecToBin;
      }
    }
    std::cout << " B " << std::flush;

    /// Save iterator to speed up the search for the next event
    itFirstEventPrevTs = itEvtPrev;

    /// Find the first event after the end of this TS
    std::vector<AccTimingEvent>::iterator itEvtNext =
      std::upper_bound(itFirstEventPrevTs, vAllEvtsBuff.end(), ulTsStartUtc + ulTsDuration);

    statusTs.SetLastEvtBefTs(*itEvtPrev);
    statusTs.fuSpillIndexAtStart             = uSpillIdx;
    std::vector<AccTimingEvent>::iterator it = itEvtPrev;
    it++;
    for (; it != itEvtNext; ++it) {
      statusTs.fvEventsDuringTS.push_back(*it);
      if (it->IsExtractionStart()) {
        //
        uSpillIdx++;
        bExtractionOn = true;
        std::cout << "Spill starting at              " << it->GetTime() << " (" << uSpillIdx << ","
                  << ((it->GetTime() - ulFirstTsStartUtc) / 1000000000) << ") " << std::endl;
        while (uTimeCurrSpillPnt <= it->GetTime()) {
          phSpillOnOff->SetBinContent(phSpillOnOff->FindBin((uTimeCurrSpillPnt - ulFirstTsStartUtc) / double(uNsToSec)),
                                      0);
          uTimeCurrSpillPnt += uNsToSec / uSecToBin / 2;
        }
      }
      else if (itEvt->IsExtractionEnd()) {
        bExtractionOn = false;
        while (uTimeCurrSpillPnt <= it->GetTime()) {
          phSpillOnOff->SetBinContent(phSpillOnOff->FindBin((uTimeCurrSpillPnt - ulFirstTsStartUtc) / double(uNsToSec)),
                                      100000);
          uTimeCurrSpillPnt += uNsToSec / uSecToBin / 2;
        }
      }
    }
    std::cout << " C " << std::flush;

    /// Save iterator to allow detection of unrecorded spills in case of missing TS
    itLastEventPrevTs = itEvtNext;

    ulTsTimeInRun = ulTsStartUtc - ulFirstTsStartUtc;
    for (auto itDigi = pVectBmon->begin(); itDigi < pVectBmon->end(); ++itDigi) {
      phCountsBmon->Fill((ulTsTimeInRun + itDigi->GetTime()) / uNsToSec);
    }
    for (auto itDigi = pVectSts->begin(); itDigi < pVectSts->end(); ++itDigi) {
      phCountsSts->Fill((ulTsTimeInRun + itDigi->GetTime()) / uNsToSec);
    }
    for (auto itDigi = pVectMuch->begin(); itDigi < pVectMuch->end(); ++itDigi) {
      phCountsMuch->Fill((ulTsTimeInRun + itDigi->GetTime()) / uNsToSec);
    }
    for (auto itDigi = pVectTrd->begin(); itDigi < pVectTrd->end(); ++itDigi) {
      if (itDigi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kSPADIC) {
        phCountsTrd1d->Fill((ulTsTimeInRun + itDigi->GetTime()) / uNsToSec);
      }
      else if (itDigi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP) {
        phCountsTrd2d->Fill((ulTsTimeInRun + itDigi->GetTime()) / uNsToSec);
      }
    }
    for (auto itDigi = pVectTof->begin(); itDigi < pVectTof->end(); ++itDigi) {
      phCountsTof->Fill((ulTsTimeInRun + itDigi->GetTime()) / uNsToSec);
    }
    for (auto itDigi = pVectRich->begin(); itDigi < pVectRich->end(); ++itDigi) {
      phCountsRich->Fill((ulTsTimeInRun + itDigi->GetTime()) / uNsToSec);
    }

    int32_t uNbEvts = pArrayEvents->GetEntriesFast();
    for (int32_t iEvt = 0; iEvt < uNbEvts; ++iEvt) {
      CbmEvent* pEvt = dynamic_cast<CbmEvent*>(pArrayEvents->At(iEvt));

      double dEvtTimeInRun = (ulTsTimeInRun + pEvt->GetStartTime()) / uNsToSec;
      phCountsEvts->Fill(dEvtTimeInRun);

      /// Fill counts of selected Digis in each detector
      if (-1 < pEvt->GetNofData(ECbmDataType::kT0Digi)) {
        phCountsEvtBmon->Fill(dEvtTimeInRun, pEvt->GetNofData(ECbmDataType::kT0Digi));
      }
      if (-1 < pEvt->GetNofData(ECbmDataType::kStsDigi)) {
        phCountsEvtSts->Fill(dEvtTimeInRun, pEvt->GetNofData(ECbmDataType::kStsDigi));
      }
      if (-1 < pEvt->GetNofData(ECbmDataType::kMuchDigi)) {
        phCountsEvtMuch->Fill(dEvtTimeInRun, pEvt->GetNofData(ECbmDataType::kMuchDigi));
      }
      if (-1 < pEvt->GetNofData(ECbmDataType::kTrdDigi)) {
        /// Check TRD 1D and TRD 2D
        for (int idigi = 0; idigi < pEvt->GetNofData(ECbmDataType::kTrdDigi); ++idigi) {
          uint idx = pEvt->GetIndex(ECbmDataType::kTrdDigi, idigi);
          if (pVectTrd->at(idx).GetType() == CbmTrdDigi::eCbmTrdAsicType::kSPADIC) {
            phCountsEvtTrd1d->Fill(dEvtTimeInRun);
          }
          else if (pVectTrd->at(idx).GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP) {
            phCountsEvtTrd2d->Fill(dEvtTimeInRun);
          }
        }
      }
      if (-1 < pEvt->GetNofData(ECbmDataType::kTofDigi)) {
        phCountsEvtTof->Fill(dEvtTimeInRun, pEvt->GetNofData(ECbmDataType::kTofDigi));
      }
      if (-1 < pEvt->GetNofData(ECbmDataType::kRichDigi)) {
        phCountsEvtRich->Fill(dEvtTimeInRun, pEvt->GetNofData(ECbmDataType::kRichDigi));
      }
    }
    std::cout << " => done" << std::endl;
  }
  if (bExtractionOn) {
    //
    while (uTimeCurrSpillPnt <= (uEndTime + 1) * uNsToSec) {
      phSpillOnOff->SetBinContent(phSpillOnOff->FindBin((uTimeCurrSpillPnt - ulFirstTsStartUtc) / double(uNsToSec)),
                                  100000);
      uTimeCurrSpillPnt += uNsToSec / uSecToBin / 2;
    }
  }


  for (uint32_t uBin = 1; uBin <= uNbBins; ++uBin) {
    if (phCountsBmon->GetBinContent(uBin)) {
      phSelRatioBmon->SetBinContent(uBin, phCountsEvtBmon->GetBinContent(uBin) / phCountsBmon->GetBinContent(uBin));
    }
    if (phCountsSts->GetBinContent(uBin)) {
      phSelRatioSts->SetBinContent(uBin, phCountsEvtSts->GetBinContent(uBin) / phCountsSts->GetBinContent(uBin));
    }
    if (phCountsMuch->GetBinContent(uBin)) {
      phSelRatioMuch->SetBinContent(uBin, phCountsEvtMuch->GetBinContent(uBin) / phCountsMuch->GetBinContent(uBin));
    }
    if (phCountsTrd1d->GetBinContent(uBin)) {
      phSelRatioTrd1d->SetBinContent(uBin, phCountsEvtTrd1d->GetBinContent(uBin) / phCountsTrd1d->GetBinContent(uBin));
    }
    if (phCountsTrd2d->GetBinContent(uBin)) {
      phSelRatioTrd2d->SetBinContent(uBin, phCountsEvtTrd2d->GetBinContent(uBin) / phCountsTrd2d->GetBinContent(uBin));
    }
    if (phCountsTof->GetBinContent(uBin)) {
      phSelRatioTof->SetBinContent(uBin, phCountsEvtTof->GetBinContent(uBin) / phCountsTof->GetBinContent(uBin));
    }
    if (phCountsRich->GetBinContent(uBin)) {
      phSelRatioRich->SetBinContent(uBin, phCountsEvtRich->GetBinContent(uBin) / phCountsRich->GetBinContent(uBin));
    }
  }

  /// Draw the histograms
  phSpillOnOff->SetLineColor(kRed);
  phCountsBmon->SetLineColor(kRed);
  phCountsSts->SetLineColor(kGreen + 1);
  phCountsMuch->SetLineColor(kBlue);
  phCountsTrd1d->SetLineColor(kCyan);
  phCountsTrd2d->SetLineColor(kViolet - 1);
  phCountsTof->SetLineColor(kBlack);
  phCountsRich->SetLineColor(kOrange + 1);
  phCountsEvts->SetLineColor(kYellow);

  phSpillOnOff->SetLineWidth(5);
  phCountsBmon->SetLineWidth(2);
  phCountsSts->SetLineWidth(2);
  phCountsMuch->SetLineWidth(2);
  phCountsTrd1d->SetLineWidth(2);
  phCountsTrd2d->SetLineWidth(2);
  phCountsTof->SetLineWidth(2);
  phCountsRich->SetLineWidth(2);
  phCountsEvts->SetLineWidth(2);

  THStack* pStackDet = new THStack("stackDet", Form("Detector counts per %5.3f ms", 1000. / uSecToBin));
  pStackDet->Add(phSpillOnOff);
  pStackDet->Add(phCountsBmon);
  pStackDet->Add(phCountsSts);
  pStackDet->Add(phCountsMuch);
  pStackDet->Add(phCountsTrd1d);
  pStackDet->Add(phCountsTrd2d);
  pStackDet->Add(phCountsTof);
  pStackDet->Add(phCountsRich);
  pStackDet->Add(phCountsEvts);

  THStack* pStackEvt = new THStack("stackEvt", Form("Bmon and TOF counts and events per %5.3f ms", 1000. / uSecToBin));
  pStackEvt->Add(phSpillOnOff);
  pStackEvt->Add(phCountsBmon);
  pStackDet->Add(phCountsSts);
  pStackEvt->Add(phCountsTof);
  pStackEvt->Add(phCountsEvts);

  TCanvas* pCanvasDigis = new TCanvas("CanvasDigis");
  pCanvasDigis->Divide(2);

  pCanvasDigis->cd(1);
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  pStackDet->Draw("nostack");
  gPad->BuildLegend(0.79, 0.84, 0.99, 0.99, "");

  pCanvasDigis->cd(2);
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  pStackEvt->Draw("nostack");
  gPad->BuildLegend(0.79, 0.84, 0.99, 0.99, "");

  /// Digis in events
  phCountsEvtBmon->SetLineColor(kRed);
  phCountsEvtSts->SetLineColor(kGreen + 1);
  phCountsEvtMuch->SetLineColor(kBlue);
  phCountsEvtTrd1d->SetLineColor(kCyan);
  phCountsEvtTrd2d->SetLineColor(kViolet - 1);
  phCountsEvtTof->SetLineColor(kBlack);
  phCountsEvtRich->SetLineColor(kOrange + 1);

  phCountsEvtBmon->SetLineWidth(2);
  phCountsEvtSts->SetLineWidth(2);
  phCountsEvtMuch->SetLineWidth(2);
  phCountsEvtTrd1d->SetLineWidth(2);
  phCountsEvtTrd2d->SetLineWidth(2);
  phCountsEvtTof->SetLineWidth(2);
  phCountsEvtRich->SetLineWidth(2);

  THStack* pStackDetInEvt = new THStack("stackDetInEvt", Form("Detector counts per %5.3f ms", 1000. / uSecToBin));
  pStackDetInEvt->Add(phSpillOnOff);
  pStackDetInEvt->Add(phCountsEvtBmon);
  pStackDetInEvt->Add(phCountsEvtSts);
  pStackDetInEvt->Add(phCountsEvtMuch);
  pStackDetInEvt->Add(phCountsEvtTrd1d);
  pStackDetInEvt->Add(phCountsEvtTrd2d);
  pStackDetInEvt->Add(phCountsEvtTof);
  pStackDetInEvt->Add(phCountsEvtRich);
  pStackDetInEvt->Add(phCountsEvts);

  /// Ratios Digis in events / All Digis
  phSelRatioBmon->SetLineColor(kRed);
  phSelRatioSts->SetLineColor(kGreen + 1);
  phSelRatioMuch->SetLineColor(kBlue);
  phSelRatioTrd1d->SetLineColor(kCyan);
  phSelRatioTrd2d->SetLineColor(kViolet - 1);
  phSelRatioTof->SetLineColor(kBlack);
  phSelRatioRich->SetLineColor(kOrange + 1);

  phSelRatioBmon->SetLineWidth(2);
  phSelRatioSts->SetLineWidth(2);
  phSelRatioMuch->SetLineWidth(2);
  phSelRatioTrd1d->SetLineWidth(2);
  phSelRatioTrd2d->SetLineWidth(2);
  phSelRatioTof->SetLineWidth(2);
  phSelRatioRich->SetLineWidth(2);

  THStack* pStackDetSelRatio = new THStack("stackDetSelRatio", Form("Detector counts per %5.3f ms", 1000. / uSecToBin));
  pStackDetSelRatio->Add(phSelRatioBmon);
  pStackDetSelRatio->Add(phSelRatioSts);
  pStackDetSelRatio->Add(phSelRatioMuch);
  pStackDetSelRatio->Add(phSelRatioTrd1d);
  pStackDetSelRatio->Add(phSelRatioTrd2d);
  pStackDetSelRatio->Add(phSelRatioTof);
  pStackDetSelRatio->Add(phSelRatioRich);


  TCanvas* pCanvasSel = new TCanvas("CanvasSel");
  pCanvasSel->Divide(2);

  pCanvasSel->cd(1);
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  pStackDetInEvt->Draw("nostack");
  gPad->BuildLegend(0.79, 0.84, 0.99, 0.99, "");

  pCanvasSel->cd(2);
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  pStackDetSelRatio->Draw("nostack");
  gPad->BuildLegend(0.79, 0.84, 0.99, 0.99, "");


  pFile->Close();
}
