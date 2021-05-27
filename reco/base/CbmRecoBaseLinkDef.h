/** @file CbmRecoBaseLinkDef.h
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Pierre-Alain Loizeau [orginator] **/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// --- base
//#pragma link C++ class CbmUnpack+; <= Template, not needed
#pragma link C++ class CbmUnpackTaskBase + ;
//#pragma link C++ class CbmUnpackTask+; <= Template, not needed

#pragma link C++ class CbmTrdTrackFitter + ;
#pragma link C++ class CbmTrdTrackFinder + ;

#pragma link C++ class CbmTofTrackFitter + ;
#pragma link C++ class CbmTofTrackFinder + ;

#pragma link C++ class CbmStsTrackFitter + ;
#pragma link C++ class CbmStsTrackFinder + ;

#pragma link C++ class CbmMuchTrackFinder + ;

#pragma link C++ class CbmRichRingFinder + ;

#pragma link C++ class CbmPrimaryVertexFinder + ;
#pragma link C++ class CbmGlobalTrackFitter + ;
#pragma link C++ class CbmTofMerger + ;
#pragma link C++ class CbmTrackMerger + ;
#pragma link C++ class CbmRichMerger + ;

#endif /* __CINT__ */
