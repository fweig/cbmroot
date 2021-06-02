/** @file CbmAnalysisTreeInterfaceLinkDef.h
  * @copyright Copyright (C) 2020-2021 Physikalisches Institut, Eberhard Karls Universitaet Tuebingen, Tuebingen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Viktor Klochkov [committer], Viktor Klochkov [committer] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmConverterManager + ;
#pragma link C++ class CbmConverterTask + ;
#pragma link C++ class CbmSimEventHeaderConverter + ;
#pragma link C++ class CbmRecEventHeaderConverter + ;
#pragma link C++ class CbmSimTracksConverter + ;
#pragma link C++ class CbmStsTracksConverter + ;
#pragma link C++ class CbmTofHitsConverter + ;
#pragma link C++ class CbmPsdModulesConverter + ;
#pragma link C++ class CbmTrdTracksConverter + ;
#pragma link C++ class CbmRichRingsConverter + ;

//#pragma link C++ class CbmMuchTracksConverter+;

#endif
