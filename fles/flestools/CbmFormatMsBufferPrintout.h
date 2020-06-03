// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmFormatMsBufferPrintout                      -----
// -----               Created 06.03.2020 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmFormatMsBufferPrintout_H
#define CbmFormatMsBufferPrintout_H

#include "Timeslice.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

/**
 ** Derived/inspired by the Flesnet BufferDump class (Found in TimesliceDebugger.xpp)
 **/

std::string FormatMsBufferPrintout( const fles::Timeslice & ts, const size_t uMsCompIdx, const size_t uMsIdx,
                                    const uint32_t uBlocksPerLine = 4 );

std::string FormatMsBufferPrintout( const fles::MicrosliceDescriptor & msDescriptor, const uint8_t* msContent,
                                    const uint32_t uBlocksPerLine = 4 );

#endif // CbmFormatMsBufferPrintout_H
