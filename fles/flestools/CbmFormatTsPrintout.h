/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmFormatTsPrintout                          -----
// -----               Created 11.09.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmFormatTsPrintout_H
#define CbmFormatTsPrintout_H

#include "Timeslice.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

/**
 ** Derived/inspired by the Flesnet TimesliceDump class (Found in TimesliceDebugger.xpp)
 **/

std::string FormatTsHeaderPrintout(const fles::Timeslice& ts);

std::string FormatTsPrintout(const fles::Timeslice& ts);

std::ostream& operator<<(std::ostream& os, const fles::Timeslice& ts);

#endif  // CbmFormatTsPrintout_H
