/** @file CbmTofSimLinkDef.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmTof + ;
#pragma link C++ class CbmTofMergeMcPoints + ;
#pragma link C++ class CbmDigitize < CbmTofDigi> + ;
#pragma link C++ class CbmTofDigitize + ;

#endif
