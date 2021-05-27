/** @file CbmRichSimLinkDef.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmRich;

#pragma link C++ class CbmDigitize < CbmRichDigi> + ;
#pragma link C++ class CbmRichDigitizer + ;

#endif
