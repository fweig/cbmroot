/* Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmFormatMsHeaderPrintout                      -----
// -----               Created 11.09.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmFormatMsHeaderPrintout_H
#define CbmFormatMsHeaderPrintout_H

#include "Timeslice.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

/**
 ** Replaces the following block of code which generate warnings depending on the OS
 LOG(info) << "hi hv eqid flag si sv idx/start        crc      size     offset";
 LOG(info) << Form( "%02x %02x %04x %04x %02x %02x %016llx %08x %08x %016llx",
                   static_cast<unsigned int>(msDescriptor.hdr_id),
                   static_cast<unsigned int>(msDescriptor.hdr_ver), msDescriptor.eq_id, msDescriptor.flags,
                   static_cast<unsigned int>(msDescriptor.sys_id),
                   static_cast<unsigned int>(msDescriptor.sys_ver), msDescriptor.idx, msDescriptor.crc,
                   msDescriptor.size, msDescriptor.offset );
 ** Derived/inspired by the Flesnet MicrosliceDescriptorDump class (Found in TimesliceDebugger.xpp)
 **/

std::string FormatMsHeaderPrintout(const fles::MicrosliceDescriptor& msDescriptor);

std::ostream& operator<<(std::ostream& os, const fles::MicrosliceDescriptor& msDescriptor);

#endif  // CbmFormatMsHeaderPrintout_H
