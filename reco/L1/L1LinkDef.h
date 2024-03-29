/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Denis Bertini [committer] */

// $Id: L1LinkDef.h,v 1.10 2006/05/18 11:36:47 friese Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmL1 + ;
#pragma link C++ class CbmL1StsTrackFinder + ;
//#pragma link C++ class  CbmL1TrdTrackFinderSts+;
#pragma link C++ class CbmL1TrackMerger + ;
#pragma link C++ class CbmL1TofMerger + ;
//#pragma link C++ class  CbmL1MuchFinder+;
//#pragma link C++ class  CbmL1MuchHit+;
//#pragma link C++ class  CbmL1MuchTrack+;
//#pragma link C++ class  CbmL1MuchFinderQa+;
#pragma link C++ class CbmL1TrdTracklet + ;
#pragma link C++ class CbmL1TrdTracklet4 + ;
#pragma link C++ class CbmL1GlobalTrackFinder + ;
#pragma link C++ class CbmL1GlobalFindTracksEvents + ;
//#pragma link C++ class  CbmL1CATrdTrackFinderSA+;
//#pragma link C++ class  CbmL1SttHit+;
//#pragma link C++ class  CbmL1SttTrackFinder+;
//#pragma link C++ class  CbmL1SttTrack+;
#pragma link C++ class CbmTrackingInputQaSts + ;
#pragma link C++ class CbmTrackerInputQaTrd + ;
#pragma link C++ class CbmTrackerInputQaTof + ;
#pragma link C++ class CbmCaInputQaMuch + ;
#pragma link C++ class CbmCaInputQaSts + ;
#pragma link C++ class CbmCaInputQaTrd + ;
#pragma link C++ class CbmCaInputQaTof + ;
#pragma link C++ enum cbm::ca::ETrackType;
#pragma link C++ enum class L1DetectorID;
#pragma link C++ class cbm::ca::OutputQa + ;
#pragma link C++ class ca::tools::WindowFinder + ;

#endif
