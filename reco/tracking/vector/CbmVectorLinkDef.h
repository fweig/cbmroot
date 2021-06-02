/** @file CbmVectorLinkDef.h
  * @copyright Copyright (C) 2018 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Alexander Zinchenko [committer] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmVectorFinder + ;
#pragma link C++ class CbmTrdFindVectors + ;
#pragma link C++ class CbmTrdToTofVector + ;
#pragma link C++ class CbmMuchFindVectors + ;
#pragma link C++ class CbmMuchFindVectorsGem + ;
//#pragma link C++ class  CbmMuchFindVectorsQA+;
#pragma link C++ class CbmMuchMergeVectors + ;
//#pragma link C++ class  CbmMuchMergeVectorsQA+;
#pragma link C++ class CbmMuchToTrdVectors + ;
#pragma link C++ class CbmMuchToTofVectors + ;

#endif
