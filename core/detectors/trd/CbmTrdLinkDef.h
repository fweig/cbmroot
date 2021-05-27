/** @file CbmTrdLinkDef.h
  * @copyright Copyright (C) 2006-2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Denis Bertini [orginator], Cyrano Bergmann **/

// $Id: TrdLinkDef.h,v 1.10 2006/06/20 09:39:59 kresan Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmGeoTrd + ;
#pragma link C++ class CbmGeoTrdPar;
#pragma link C++ class CbmTrdContFact;
#pragma link C++ class CbmTrdSPADIC;
#pragma link C++ class CbmTrdUtils + ;
#pragma link C++ class CbmTrdGeoHandler + ;
#pragma link C++ class CbmTrdGas + ;

#pragma link C++ class CbmTrd + ;
#pragma link C++ class CbmTrdDigitizer + ;
#pragma link C++ class CbmTrdRadiator;
#pragma link C++ class CbmTrdTrackFinderIdeal + ;
#pragma link C++ class CbmTrdTrackFitterIdeal + ;

#pragma link C++ class CbmTrdDigiPar + ;
#pragma link C++ class CbmTrdCreateDigiPar + ;
#pragma link C++ class CbmTrdDigitizerPRF;
#pragma link C++ class CbmTrdDigitizerPRF_TB;
#pragma link C++ class CbmTrdHitProducerIdeal + ;
#pragma link C++ class CbmTrdHitProducerSmearing + ;
#pragma link C++ class CbmTrdHitProducerDigi + ;
#pragma link C++ class CbmTrdClusterFinderFast + ;
#pragma link C++ class CbmTrdClusterFinderDPB + ;
#pragma link C++ class CbmTrdHitProducerCluster + ;
#pragma link C++ class CbmTrdTriangle + ;
#pragma link C++ class CbmTrdModule + ;
#pragma link C++ class CbmTrdAsic + ;
#pragma link C++ class CbmTrdFindTracks + ;
#pragma link C++ class CbmTrdFitTracks + ;

#endif
