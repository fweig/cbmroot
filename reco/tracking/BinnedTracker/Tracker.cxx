/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CbmBinnedTrackerTask.h"

CbmBinnedTracker* CbmBinnedTracker::Instance()
{
   static CbmBinnedTracker* theInstance = 0;
      
   if (0 == theInstance)
   {
      if (CbmBinnedTrackerTask::Instance())
         theInstance = new CbmBinnedTracker(CbmBinnedTrackerTask::Instance()->GetBeamDx(), CbmBinnedTrackerTask::Instance()->GetBeamDy());
      else
         theInstance = new CbmBinnedTracker(0.1, 0.1);
   }
        
   return theInstance;
}
