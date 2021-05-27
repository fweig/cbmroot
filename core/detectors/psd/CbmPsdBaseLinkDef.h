/** @file CbmPsdBaseLinkDef.h
  * @copyright Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [orginator], Florian Uhlig **/

// $Id: TofLinkDef.h,v 1.3 2006/03/07 11:51:55 friese Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class PsdSignalFitting::PronyFitter;
#pragma link C++ class CbmPsdContFact + ;
#pragma link C++ class CbmMcbm2018PsdPar + ;

#endif
