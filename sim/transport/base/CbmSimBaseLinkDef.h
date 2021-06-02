/** @file CbmSimBaseLinkDef.h
  * @copyright Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// --- transport/base
#pragma link C++ class CbmBeamProfile + ;
#pragma link C++ class CbmEventGenerator + ;
#pragma link C++ class CbmMCEventFilter + ;
#pragma link C++ class CbmStack + ;
#pragma link C++ class CbmStackFilter + ;
#pragma link C++ class CbmTarget + ;


#endif /* __CINT__ */
