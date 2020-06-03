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
   CbmOffsetDigiTimeSts  stsDigiCal(  "CbmStsDigi" );
   CbmOffsetDigiTimeTof  tofDigiCal(  "CbmTofDigi" );
   CbmOffsetDigiTimeMuch muchDigiCal( "CbmMuchBeamTimeDigi" );
}

CbmOffsetDigiTimeTester::~CbmOffsetDigiTimeTester()
{
}
