/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmPsdMC;
#pragma link C++ class CbmDigitize < CbmPsdDigi> + ;
#pragma link C++ class CbmPsdIdealDigitizer + ;
#pragma link C++ class CbmPsdSimpleDigitizer + ;

#endif
