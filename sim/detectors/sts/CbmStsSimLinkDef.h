/** @file CbmStsSimLinkDef.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [orginator] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmDigitize < CbmStsDigi> + ;
#pragma link C++ class CbmStsDigitize + ;
#pragma link C++ class CbmStsDigitizeQa + ;
#pragma link C++ class CbmStsDigitizeQaReport + ;
#pragma link C++ class CbmStsMC + ;
#pragma link C++ class CbmStsSensorPoint + ;
#pragma link C++ class CbmStsSignal + ;
#pragma link C++ class CbmStsSimModule + ;
#pragma link C++ class CbmStsSimSensor + ;
#pragma link C++ class CbmStsSimSensorDssd + ;
#pragma link C++ class CbmStsSimSensorDssdStereo + ;
#pragma link C++ class CbmStsSimSensorDssdOrtho + ;
#pragma link C++ class CbmStsSimSensorFactory + ;

#endif /* __CINT__ */
