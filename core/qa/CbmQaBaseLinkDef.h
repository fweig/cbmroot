/** @file CbmQaBaseLinkDef.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Sergey Gorbunov [orginator] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedef;

#pragma link C++ class CbmQaCanvas - ;
#pragma link C++ class CbmQaPieSlice + ;
#pragma link C++ class CbmQaPie - ;
#pragma link C++ class CbmQaH1F + ;
#pragma link C++ class CbmQaH1D + ;

#endif
