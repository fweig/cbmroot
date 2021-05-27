/** @file CbmMuchSimLinkDef.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;


#pragma link C++ class CbmGeoMuch + ;
#pragma link C++ class CbmMuch + ;

#pragma link C++ class CbmDigitize < CbmMuchDigi> + ;
#pragma link C++ class CbmMuchDigitizeGem + ;
#pragma link C++ class CbmMuchReadoutBuffer + ;
#pragma link C++ class CbmMuchSignal + ;
#pragma link C++ class CbmMuchTransportQa + ;
#pragma link C++ class CbmMuchDigitizerQa + ;

#endif
