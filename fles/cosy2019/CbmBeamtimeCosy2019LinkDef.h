/** @file CbmBeamtimeCosy2019LinkDef.h
  * @copyright Copyright (C) 2021 GSI, IKF-UFra
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmCosy2019HodoPar;
#pragma link C++ class CbmCosy2019ContFact;

#pragma link C++ class CbmCosy2019UnpackerAlgoHodo;
#pragma link C++ class CbmCosy2019UnpackerTaskHodo + ;
#pragma link C++ class CbmStsCoincHodo;

#pragma link C++ class CbmCosy2019MonitorAlgoHodo;
#pragma link C++ class CbmCosy2019MonitorTaskHodo;

#endif
