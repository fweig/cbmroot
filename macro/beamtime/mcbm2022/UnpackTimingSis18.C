/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

void UnpackTimingSis18(std::string sDigiFileName,
                       std::string sTimingFilesFolder = "/lustre/cbm/users/ploizeau/mcbm2022", bool bVerbose = false)
{
  std::vector<std::string> vTimingFileNames = {"sis18_events_2022_03_24.txt", "sis18_events_2022_03_29.txt",
                                               "sis18_events_2022_03_30.txt", "sis18_events_2022_03_31.txt",
                                               "sis18_events_2022_04_01.txt", "sis18_events_2022_04_02.txt",
                                               "sis18_events_2022_04_03.txt", "sis18_events_2022_04_04.txt"};

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
  TFile* pFile = new TFile(sDigiFileName.data(), "UPDATE");
  if (kTRUE == pFile->IsZombie()) {
    std::cout << "Digi file " << sDigiFileName << " could not be open!" << std::endl;
    return;
  }

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
    std::cout << "=> No eccelerator data available for this digi file!" << std::endl;
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

  /// Create for each TS a list of events, including the last event before its start and all event during its duration
  AccStatusTs statusTs;
  TBranch* pBranch = pTree->Branch("AccStatusTs", &statusTs);

  std::vector<AccTimingEvent>::iterator itFirstEventPrevTs = vAllEvtsBuff.begin();
  std::vector<AccTimingEvent>::iterator itLastEventPrevTs  = vAllEvtsBuff.begin();
  uSpillIdx                                                = 0;
  for (uint32_t uTs = 0; uTs < uNbTs; ++uTs) {
    pTree->GetEntry(uTs);

    /// Find the first event after the start of this TS
    uint64_t ulTsStartUtc = pEventHeader->GetTsStartTime();
    std::vector<AccTimingEvent>::iterator itEvtPrev =
      std::upper_bound(itFirstEventPrevTs, vAllEvtsBuff.end(), ulTsStartUtc);

    /// Loop on events between last TS and the current one to count potentially not recorded spills
    for (std::vector<AccTimingEvent>::iterator it = itLastEventPrevTs; it != itEvtPrev; ++it) {
      if (itEvt->IsExtractionStart()) {
        //
        uSpillIdx++;
      }
    }

    if (vAllEvtsBuff.begin() == itEvtPrev) {
      std::cout << "No Events before the start of the first TS "
                << "=> not possible to do a full list of spill status for this digi file!" << std::endl;
      pFile->Close();
      return;
    }
    itEvtPrev--;

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
        std::cout << "Spill starting at              " << it->GetTime() << " (" << uSpillIdx << ") " << std::endl;
      }
    }
    pBranch->Fill();

    /// Save iterator to allow detection of unrecorded spills in case of missing TS
    itLastEventPrevTs = itEvtNext;
  }
  /// Save only the new version of the tree which includes the new branch
  pTree->Write("", TObject::kOverwrite);


  pFile->Close();
}
