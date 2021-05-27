/** @file CbmSimSteerLinkDef.h
  * @copyright Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// --- transport/base
#pragma link C++ class CbmSetup + ;
#pragma link C++ class CbmTransport + ;
#pragma link C++ class CbmVMCSettings + ;
#pragma link C++ class CbmGeant3Settings + ;
#pragma link C++ class CbmGeant4Settings + ;
#pragma link C++ class CbmTransportConfig + ;
#pragma link C++ enum ECbmEngine;


#endif /* __CINT__ */
