/** @file GlobalLinkDef.h
  * @copyright Copyright (C) 2017 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [committer] **/

// $Id: LxLinkDef.h,v 1.10 2013/09/30 11:36:47 tablyaz Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmGlobalTrackingTask + ;
#pragma link C++ class CbmGlobalTrackingQA + ;

#endif
