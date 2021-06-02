/** @file CbmFormatMsBufferPrintout.h
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmFormatMsBufferPrintout                      -----
// -----               Created 06.03.2020 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmFormatMsBufferPrintout_H
#define CbmFormatMsBufferPrintout_H

#include "Timeslice.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

/**
 ** Derived/inspired by the Flesnet BufferDump class (Found in TimesliceDebugger.xpp)
 **/

std::string FormatMsBufferPrintout(const fles::Timeslice& ts, const size_t uMsCompIdx, const size_t uMsIdx,
                                   const uint32_t uBlocksPerLine = 4);

std::string FormatMsBufferPrintout(const fles::MicrosliceDescriptor& msDescriptor, const uint8_t* msContent,
                                   const uint32_t uBlocksPerLine = 4);

#endif  // CbmFormatMsBufferPrintout_H
