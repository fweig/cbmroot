/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmMvd + ;

#pragma link C++ class CbmDigitize < CbmMvdDigi> + ;
#pragma link C++ class CbmMvdDigitizer + ;
#pragma link C++ class CbmMvdPixelCharge + ;

#pragma link C++ class CbmMvdSensorDigitizerTask + ;

#endif
