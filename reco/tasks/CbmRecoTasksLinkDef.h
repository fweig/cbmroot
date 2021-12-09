/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

// ===== LinkDef for reco/task =====

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// --- Classes
#pragma link C++ class CbmTaskBuildEvents + ;
#pragma link C++ class CbmTaskMakeRecoEvents + ;
#pragma link C++ class CbmTaskTriggerDigi + ;

#endif /* __CINT__ */
