/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

#include <cstdint>
#include <string>
#include <vector>

class AccTimingEvent {
public:
  AccTimingEvent() = default;

  AccTimingEvent(uint64_t ulPlannedUTCIn, uint64_t ulPlannedTAIIn, uint64_t ulRawEventIn, uint64_t ulRawParamsIn,
                 uint32_t uRawTimingFlagsIn, uint64_t ulExecutedUTCIn, uint64_t ulExecutedTAIIn);

  AccTimingEvent(std::string sLine, bool bVerbose = false);

  void Print() const;

  uint32_t GetGroupId() const { return ((fulRawEvent >> kOffsetGroupId) & ((1ULL << kNbBitsGroupId) - 1)); }
  uint32_t GetEventId() const { return ((fulRawEvent >> kOffsetEventId) & ((1ULL << kNbBitsEventId) - 1)); }

  uint64_t GetTime() const { return fulPlannedUTC; }

  inline bool IsCycleStart() const { return (kEventIdStartCycle == GetEventId()); }
  inline bool IsCycleEnd() const { return (kEventIdEndCycle == GetEventId()); }
  inline bool IsExtractionStart() const { return (kEventIdExtrStartSlow == GetEventId()); }
  inline bool IsExtractionEnd() const { return (kEventIdExtrEnd == GetEventId()); }

  inline bool operator<(const AccTimingEvent& rhs) { return fulPlannedUTC < rhs.fulPlannedUTC; }
  inline bool operator<(const uint64_t& rhs) { return fulPlannedUTC < rhs; }

  /// Needed for finding time position within a vector of AccTimingEvent with lower_bound/upper_bound
  friend bool operator<(const uint64_t& lhs, const AccTimingEvent& rhs) { return lhs < rhs.fulPlannedUTC; }

private:
  /// Constants
  /// --> Bit fields of the Raw event descriptor
  /// ----> Field size
  static const uint32_t kNbBitsFormatId   = 4;  // Content of field should always be 1!!
  static const uint32_t kNbBitsGroupId    = 12;
  static const uint32_t kNbBitsEventId    = 12;
  static const uint32_t kNbBitsFlags      = 4;
  static const uint32_t kNbBitsSequenceId = 12;
  static const uint32_t kNbBitsBeamProcId = 14;
  static const uint32_t kNbBitsReserved   = 6;
  /// ----> Field offset
  static const uint32_t kOffsetReserved   = 0;
  static const uint32_t kOffsetBeamProcId = kOffsetReserved + kNbBitsReserved;
  static const uint32_t kOffsetSequenceId = kOffsetBeamProcId + kNbBitsBeamProcId;
  static const uint32_t kOffsetFlags      = kOffsetSequenceId + kNbBitsSequenceId;
  static const uint32_t kOffsetEventId    = kOffsetFlags + kNbBitsFlags;
  static const uint32_t kOffsetGroupId    = kOffsetEventId + kNbBitsEventId;
  static const uint32_t kOffsetFormatId   = kOffsetGroupId + kNbBitsGroupId;
  /// --> Event types
  ///     Dec  Hex  Name                  Meaning
  ///
  ///     Spill limits
  ///     46   2E   EVT_EXTR_START_SLOW   Start of extraction
  ///     51   33   EVT_EXTR_END          End of extraction
  ///     78   4E   EVT_EXTR_STOP_SLOW    End of slow extraction
  ///
  ///     Cycle limits
  ///     32   20   EVT_START_CYCLE       First Event in a cycle
  ///     55   37   EVT_END_CYCLE         End of a cycle
  static const uint32_t kEventIdStartCycle    = 32;
  static const uint32_t kEventIdExtrStartSlow = 46;
  static const uint32_t kEventIdExtrEnd       = 51;
  static const uint32_t kEventIdEndCycle      = 55;
  static const uint32_t kEventIdExtrStopSlow  = 78;

  /// Fields
  uint64_t fulPlannedUTC    = 0;
  uint64_t fulPlannedTAI    = 0;
  uint64_t fulRawEvent      = 0;
  uint64_t fulRawParams     = 0;
  uint32_t fuRawTimingFlags = 0;
  uint64_t fulExecutedUTC   = 0;
  uint64_t fulExecutedTAI   = 0;

  ClassDef(AccTimingEvent, 1);
};

//--------------------------------------------------------------------------------------------------------------------//

class AccStatusTs {
public:
  AccStatusTs() = default;

  AccStatusTs(uint32_t uSpillIdx, AccTimingEvent lastEvtBefTs)
    : fuSpillIndexAtStart(uSpillIdx)
    , fLastEvtBeforeTs(lastEvtBefTs)
  {
  }

  void SetLastEvtBefTs(AccTimingEvent lastEvtBefTs) { fLastEvtBeforeTs = lastEvtBefTs; }

  /// True when we start within a spill cycle (not in short interval between cycle end and cycle start))
  inline bool IsCycleOnAtStart() const { return !(fLastEvtBeforeTs.IsCycleEnd()); }
  /// True when we start in the middle of an extraction spill
  inline bool IsSpillOnAtStart() const { return fLastEvtBeforeTs.IsExtractionStart(); }

  bool IsSpillOnAtTime(uint64_t uTimeUtc);
  uint32_t GetSpillIdxAtTime(uint64_t uTimeUtc);

  /// Members
  uint32_t fuSpillIndexAtStart                 = 0;
  AccTimingEvent fLastEvtBeforeTs              = {};
  std::vector<AccTimingEvent> fvEventsDuringTS = {};

  ClassDef(AccStatusTs, 1);
};
//--------------------------------------------------------------------------------------------------------------------//
