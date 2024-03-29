/* Copyright (C) 2018-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// $Id: TrdBaseLinkDef.h $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmTrdGas + ;
#pragma link C++ class CbmTrdContFact + ;
#pragma link C++ class CbmMcbm2020TrdTshiftPar + ;
#pragma link C++ class CbmTrdModuleAbstract + ;

#pragma link C++ class CbmTrdParManager + ;
#pragma link C++ class CbmTrdParSet + ;
#pragma link C++ class CbmTrdParSetAsic + ;
#pragma link C++ class CbmTrdParSetGas + ;
#pragma link C++ class CbmTrdParSetGain + ;
#pragma link C++ class CbmTrdParSetGeo + ;
#pragma link C++ class CbmTrdParSetDigi + ;
#pragma link C++ class CbmTrdParMod + ;
#pragma link C++ class CbmTrdParAsic + ;
#pragma link C++ class CbmTrdParSpadic + ;
#pragma link C++ class CbmTrdSpadic + ;
#pragma link C++ class CbmTrdFASP + ;
#pragma link C++ class std::map<int, std::array<int, 16>> + ;
#pragma link C++ class CbmTrdParFasp + ;
#pragma link C++ class CbmTrdParFaspChannel + ;
#pragma link C++ class CbmTrdParModGas + ;
#pragma link C++ class CbmTrdParModGain + ;
#pragma link C++ class CbmTrdParModGeo + ;
#pragma link C++ class CbmTrdParModDigi + ;
#pragma link C++ class CbmTrdTrackingInterface + ;
//tools
#pragma link C++ class CbmTrdGeoHandler + ;
#pragma link C++ class CbmTrddEdxUtils + ;
#pragma link C++ class CbmTrdUtils + ;
#pragma link C++ class CbmTrdHardwareSetupR + ;
#pragma link C++ class CbmTrdRadiator + ;
#endif
