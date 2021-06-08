/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// --- calibration templates tester to get them included in the library
#pragma link C++ class CbmOffsetDigiTimeTester + ;

// --- calibration templates
#pragma link C++ class CbmOffsetDigiTime                                       \
  < CbmStsDigi> + ;  // <= Template specialization
#pragma link C++ class CbmOffsetDigiTime                                       \
  < CbmTofDigi> + ;  // <= Template specialization
#pragma link C++ class CbmOffsetDigiTime                                       \
  < CbmMuchBeamTimeDigi> + ;  // <= Template specialization

#pragma link C++ class CbmOffsetDigiTimeSts                                    \
  + ;  // <= Template specialization for STS
#pragma link C++ class CbmOffsetDigiTimeTof                                    \
  + ;  // <= Template specialization for TOF
#pragma link C++ class CbmOffsetDigiTimeMuch                                   \
  + ;  // <= Template specialization for MUCH

#endif /* __CINT__ */
