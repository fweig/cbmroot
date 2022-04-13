/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "AccDataSis18.h"

#include <Logger.h>  // for LOG output

#include <iomanip>
#include <iostream>

AccTimingEvent::AccTimingEvent(uint64_t ulPlannedUTCIn, uint64_t ulPlannedTAIIn, uint64_t ulRawEventIn,
                               uint64_t ulRawParamsIn, uint32_t uRawTimingFlagsIn, uint64_t ulExecutedUTCIn,
                               uint64_t ulExecutedTAIIn)
  : fulPlannedUTC {ulPlannedUTCIn}
  , fulPlannedTAI {ulPlannedTAIIn}
  , fulRawEvent {ulRawEventIn}
  , fulRawParams {ulRawParamsIn}
  , fuRawTimingFlags {uRawTimingFlagsIn}
  , fulExecutedUTC {ulExecutedUTCIn}
  , fulExecutedTAI {ulExecutedTAIIn}
{
}

AccTimingEvent::AccTimingEvent(std::string sLine, bool bVerbose)
{
  /*
   * Format used to generate the timing files
   * | std::cout << "Planned UTC: " << setw(20) << deadline.getUTC();
   * | std::cout << " TAI: " << setw(20) <<deadline.getTAI();
   * | std::cout << " Raw:" << std::hex << std::setfill('0')
   * |           << " 0x" << std::setw(16) << id
   * |           << " 0x" << std::setw(16) << param
   * |           << " 0x" << std::setw(4) << flags
   * |           << std::dec << std::setfill(' ');
   * | std::cout << " exec UTC: " << setw(20) <<executed.getUTC();
   * | std::cout << " TAI: " << setw(20) <<executed.getTAI();
   * |
   * | /// Dec  Hex  Name                  Meaning
   * | ///
   * | /// Spill limits
   * | /// 46   2E   EVT_EXTR_START_SLOW   Start of extraction
   * | /// 51   33   EVT_EXTR_END          End of extraction
   * | /// 78   4E   EVT_EXTR_STOP_SLOW    End of slow extraction
   * | ///
   * | /// Cycle limits
   * | /// 32   20   EVT_START_CYCLE       First Event in a cycle
   * | /// 55   37   EVT_END_CYCLE         End of a cycle
   * | uint32_t uEventNb = ((id >> 36) & 0xfff);
   * | switch (uEventNb) {
   * |   case 32:
   * |     std::cout << " => EVT_START_CYCLE     ";
   * |     break;
   * |   case 55:
   * |     std::cout << " => EVT_END_CYCLE       ";
   * |     break;
   * |   case 46:
   * |     std::cout << " => EVT_EXTR_START_SLOW ";
   * |     break;
   * |   case 51:
   * |     std::cout << " => EVT_EXTR_END        ";
   * |     break;
   * |   case 78:
   * |     std::cout << " => EVT_EXTR_STOP_SLOW  ";
   * |     break;
   * | }
   * | std::cout << tr_formatDate(deadline, pmode);
   * | std::cout << tr_formatActionFlags(flags, executed - deadline, pmode);
   * | std::cout << std::endl;
   */

  std::string sToken = "Planned UTC: ";
  std::size_t posTok = sLine.find(sToken);
  sLine              = sLine.substr(posTok + sToken.size());

  sToken        = " TAI: ";
  posTok        = sLine.find(sToken);
  fulPlannedUTC = std::stoul(sLine.substr(0, posTok));
  sLine         = sLine.substr(posTok + sToken.size());

  sToken        = " Raw: 0x";
  posTok        = sLine.find(sToken);
  fulPlannedTAI = std::stoul(sLine.substr(0, posTok));
  sLine         = sLine.substr(posTok + sToken.size());

  sToken      = " 0x";
  posTok      = sLine.find(sToken);
  fulRawEvent = std::stoul(sLine.substr(0, posTok), 0, 16);
  sLine       = sLine.substr(posTok + sToken.size());

  sToken       = " 0x";
  posTok       = sLine.find(sToken);
  fulRawParams = std::stoul(sLine.substr(0, posTok), 0, 16);
  sLine        = sLine.substr(posTok + sToken.size());

  sToken           = " exec UTC: ";
  posTok           = sLine.find(sToken);
  fuRawTimingFlags = std::stoul(sLine.substr(0, posTok), 0, 16);
  sLine            = sLine.substr(posTok + sToken.size());

  sToken         = " TAI: ";
  posTok         = sLine.find(sToken);
  fulExecutedUTC = std::stoul(sLine.substr(0, posTok));
  sLine          = sLine.substr(posTok + sToken.size());

  sToken         = " => ";
  posTok         = sLine.find(sToken);
  fulExecutedTAI = std::stoul(sLine.substr(0, posTok));
  sLine          = sLine.substr(posTok + sToken.size());

  if (bVerbose) Print();
}

void AccTimingEvent::Print() const
{
  /* clang-format off */
  LOG(info) << "Planned UTC: " << std::setw(20) << fulPlannedUTC
            << " TAI: " << std::setw(20) << fulPlannedTAI
            << " Raw:" << std::hex << std::setfill('0')
            << " 0x" << std::setw(16) << fulRawEvent
            << " 0x" << std::setw(16) << fulRawParams
            << " 0x" << std::setw(4) << fuRawTimingFlags
            << std::dec << std::setfill(' ')
            << " exec UTC: " << std::setw(20) << fulExecutedUTC
            << " TAI: " << std::setw(20) << fulExecutedTAI;
  /* clang-format on */
}
//--------------------------------------------------------------------------------------------------------------------//

bool AccStatusTs::IsSpillOnAtTime(uint64_t uTimeUtc)
{
  bool bSpillOn = IsSpillOnAtStart();

  std::vector<AccTimingEvent>::iterator it = fvEventsDuringTS.begin();
  while (it != fvEventsDuringTS.end() && (*it < uTimeUtc)) {

    if (bSpillOn && it->IsExtractionEnd()) {
      /// We start in the middle of an extraction spill
      bSpillOn = false;
    }
    else if (!bSpillOn && it->IsExtractionStart()) {
      /// We start in the middle of an extraction spill
      bSpillOn = true;
    }
  }
  return bSpillOn;
}

uint32_t AccStatusTs::GetSpillIdxAtTime(uint64_t uTimeUtc)
{
  bool bSpillOn      = IsSpillOnAtStart();
  uint32_t uSpillIdx = fuSpillIndexAtStart;

  std::vector<AccTimingEvent>::iterator it = fvEventsDuringTS.begin();
  while (it != fvEventsDuringTS.end() && (*it < uTimeUtc)) {

    if (bSpillOn && it->IsExtractionEnd()) {
      /// We start in the middle of an extraction spill
      bSpillOn = false;
    }
    else if (!bSpillOn && it->IsExtractionStart()) {
      /// We start in the middle of an extraction spill
      bSpillOn = true;
      uSpillIdx++;
    }
  }
  return uSpillIdx;
}
//--------------------------------------------------------------------------------------------------------------------//
