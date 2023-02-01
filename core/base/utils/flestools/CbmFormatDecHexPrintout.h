/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmFormatDecHexPrintout                           -----
// -----               Created 11.09.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmFormatDecHexPrintout_H
#define CbmFormatDecHexPrintout_H

#include <iomanip>
#include <iostream>
#include <sstream>

/**
 ** Replaces the following block of code which generate warnings depending on the OS
    Form( "%llu", static_cast<uint64_t>(val) );
    Form( "%lu", static_cast<uint64_t>(val) );
    Form( "%0llu", static_cast<uint64_t>(val) );
    Form( "%0lu", static_cast<uint64_t>(val) );
    Form( "%016llu", static_cast<uint64_t>(val) );
    Form( "%016lu", static_cast<uint64_t>(val) );
 **/

std::string FormatDecPrintout(uint64_t ulVal, char cFill = 0, uint uWidth = 0);

/**
 ** Replaces the following block of code which generate warnings depending on the OS
    Form( "%llx", static_cast<uint64_t>(val) );
    Form( "%lx", static_cast<uint64_t>(val) );
    Form( "%0llx", static_cast<uint64_t>(val) );
    Form( "%0lx", static_cast<uint64_t>(val) );
    Form( "%016llx", static_cast<uint64_t>(val) );
    Form( "%016lx", static_cast<uint64_t>(val) );
 **/

std::string FormatHexPrintout(uint64_t ulVal, char cFill = 0, uint uWidth = 0, bool bUppercase = false);

#endif
