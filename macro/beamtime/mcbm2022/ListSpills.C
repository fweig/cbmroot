/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 ** May 2022 Nickel runs (UTC times):
 ** 2382 "2022-05-25 21:34:15", "2022-05-25 21:36:45"
 ** 2383 "2022-05-25 21:39:52", "2022-05-25 21:42:04"
 ** 2387 "2022-05-25 23:09:03", "2022-05-25 23:15:04"
 ** 2388 "2022-05-25 23:18:14", "2022-05-25 23:23:48"
 ** 2389 "2022-05-25 23:29:22", "2022-05-26 00:28:31"
 ** 2390 "2022-05-26 00:37:16", "2022-05-26 01:10:28"
 ** 2391 "2022-05-26 01:18:54", "2022-05-26 03:15:50"
 ** 2393 "2022-05-26 03:27:15", "2022-05-26 03:28:57"
 ** 2394 "2022-05-26 03:33:51", "2022-05-26 04:04:41"
 ** 2395 "2022-05-26 04:10:08", "2022-05-26 06:05:58"
 **/

void ListSpills(std::string sDateTimeStart, std::string sDateTimStop,
                std::string sTimingFilesFolder = "/lustre/cbm/prod/beamtime/2022/mcbm_sis18_events",
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

  /// Typical format in eLOG:
  /// Common start at 2022-05-26 08:09:18
  /// Common stop at 2022-05-26 08:14:33
  std::tm datetimeStart = {};
  std::istringstream issStart(sDateTimeStart);
  issStart >> std::get_time(&datetimeStart, "%Y-%m-%d %H:%M:%S");
  std::chrono::time_point<std::chrono::system_clock> timeStart {std::chrono::seconds(std::mktime(&datetimeStart))};
  std::cout << "Start time: " << timeStart.time_since_epoch().count() << std::endl;

  if (timeStart.time_since_epoch().count() < vAllEvtsBuff[0].GetTime()) {
    std::cout << "Earlier than the first recorded accelerator Event: " << vAllEvtsBuff[0].GetTime() << std::endl;
    std::cout << "=> No accelerator data available for this range!" << std::endl;
    return;
  }

  std::tm datetimeStop = {};
  std::istringstream issStop(sDateTimStop);
  issStop >> std::get_time(&datetimeStop, "%Y-%m-%d %H:%M:%S");
  std::chrono::time_point<std::chrono::system_clock> timeStop {std::chrono::seconds(std::mktime(&datetimeStop))};
  std::cout << "Stop time:  " << timeStop.time_since_epoch().count() << std::endl;

  if (timeStop.time_since_epoch().count() > vAllEvtsBuff[vAllEvtsBuff.size() - 1].GetTime()) {
    std::cout << "Latter than the last recorded accelerator Event: " << vAllEvtsBuff[vAllEvtsBuff.size() - 1].GetTime()
              << std::endl;
    std::cout << "=> No accelerator data available for this range!" << std::endl;
    return;
  }

  std::cout << "Run length: " << ((timeStop.time_since_epoch().count() - timeStart.time_since_epoch().count()) * 1e-9)
            << std::endl;

  /// Create a list of spills within the range
  bool bExtractionOn = false;
  bool bCycleOn      = false;
  uint32_t uSpillIdx = 0;
  uint32_t uCycleIdx = 0;
  std::vector<uint64_t> vSpillsStartTime;

  std::vector<AccTimingEvent>::iterator itEvt =
    std::upper_bound(vAllEvtsBuff.begin(), vAllEvtsBuff.end(), timeStart.time_since_epoch().count());
  if (vAllEvtsBuff.end() == itEvt) {
    std::cout << "No Events after the start of the range => no accelerator data available for this run!" << std::endl;
    return;
  }

  if (itEvt->IsExtractionEnd()) {
    /// We start in the middle of an extraction spill
    bExtractionOn = true;
    if (itEvt != vAllEvtsBuff.begin()) {
      std::vector<AccTimingEvent>::iterator itPrevEvt = itEvt;
      --itPrevEvt;
      vSpillsStartTime.push_back(itPrevEvt->GetTime());
    }
    else {
      vSpillsStartTime.push_back(timeStart.time_since_epoch().count());
    }
    std::cout << "Spill ongoing when starting at " << timeStart.time_since_epoch().count() << " (" << uSpillIdx << ") "
              << std::endl;
  }
  if (!itEvt->IsCycleStart()) {
    /// We start within a spill cycle (not in short interval between cycle end and cycle start))
    bCycleOn = true;
  }
  while ((vAllEvtsBuff.end() != itEvt) && itEvt->GetTime() <= timeStop.time_since_epoch().count()) {
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
      vSpillsStartTime.push_back(itEvt->GetTime());
      std::cout << "Spill starting at              " << itEvt->GetTime() << " (" << uSpillIdx << ") " << std::endl;
    }
    else if (itEvt->IsExtractionEnd()) {
      //
      bExtractionOn = false;
      std::cout << "Spill end at                   " << itEvt->GetTime() << std::endl;
    }

    itEvt++;
  }
  std::cout << "Number of spills found for this range: " << (1 + uSpillIdx) << std::endl;
}
