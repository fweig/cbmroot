/** @file CbmOffsetDigiTimeTester.cxx
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmOffsetDigiTimeTester                        -----
// -----               Created 18.02.2020 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmOffsetDigiTimeTester.h"

/// CbmRoot (+externals) headers

/// FairRoot headers

/// Fairsoft (Root, Boost, ...) headers

/// C/C++ headers

CbmOffsetDigiTimeTester::CbmOffsetDigiTimeTester()
{
  CbmOffsetDigiTimeSts stsDigiCal("CbmStsDigi");
  CbmOffsetDigiTimeTof tofDigiCal("CbmTofDigi");
  CbmOffsetDigiTimeMuch muchDigiCal("CbmMuchBeamTimeDigi");
}

CbmOffsetDigiTimeTester::~CbmOffsetDigiTimeTester() {}
