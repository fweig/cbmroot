/** @file NicaCbmFormatLinkDef.h
  * @copyright Copyright (C) 2018-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer], Daniel Wielanek [committer] **/

/*
 * NicaBmnFormatLinkDef.h
 *
 *  Created on: 19 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace NicaCbmFormats;

#pragma link C++ class CbmHbtFullEvent + ;
#pragma link C++ class NicaCbmFullEvent + ;
#pragma link C++ class CbmHbtEvent + ;
#pragma link C++ class CbmHbtTrack + ;

#pragma link C++ class NicaGeneratorReadUnigen + ;
#pragma link C++ class NicaGeneratorWriteUnigen + ;
#pragma link C++ class NicaUnigenEvent + ;
#pragma link C++ class NicaUnigenEventInterface + ;
#pragma link C++ class NicaUnigenSource + ;
#pragma link C++ class NicaUnigenTrack + ;
#pragma link C++ class NicaUnigenTrackInterface + ;

#pragma link C++ class NicaCbmMCEvent + ;
#pragma link C++ class NicaCbmMCEventInterface + ;
#pragma link C++ class NicaCbmMCTrack + ;
#pragma link C++ class NicaCbmMCTrackInterface + ;

#pragma link C++ class NicaCbmEvent + ;
#pragma link C++ class NicaCbmEventInterface + ;
#pragma link C++ class NicaCbmTrack + ;
#pragma link C++ class NicaCbmTrackInterface + ;

#pragma link C++ class CbmAnaTreeRecoSourceContainer + ;
#pragma link C++ class CbmAnaTreeMcSourceContainer + ;
#pragma link C++ class CbmAnaTreeSource + ;

