// -------------------------------------------------------------------------
// -----                CbmTofStar2018Par source file                  -----
// -----                Created 18/01/17  by P.-A. Loizeau             -----
// -------------------------------------------------------------------------

#include "CbmTofStar2018Par.h"

#include "FairParamList.h"
#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairLogger.h"

#include "TString.h"

// -----   Standard constructor   ------------------------------------------
CbmTofStar2018Par::CbmTofStar2018Par(const char* name,
			             const char* title,
			             const char* context)
  : FairParGenericSet(name, title, context),
    fiNrOfGdpb(-1),
    fiGdpbIdArray(),
    fiNrOfFebsPerGdpb(-1),
    fiNrOfGet4PerFeb(-1),
    fiNrOfChannelsPerGet4(-1),
    fiNrOfChannels(-1),
    fiChannelToDetUIdMap(),
    fiNbMsTot(0),
    fiNbMsOverlap(0),
    fdSizeMsInNs(0.0),
    fdStarTriggerDeadtime(),
    fdStarTriggerDelay(),
    fdStarTriggerWinSize(),
    fdTsDeadtimePeriod(0.0)
{
   detName="Tof";
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
CbmTofStar2018Par::~CbmTofStar2018Par()
{
}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmTofStar2018Par::clear()
{
   status = kFALSE;
   resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmTofStar2018Par::putParams(FairParamList* l)
{
   if (!l) return;
   l->add("NrOfRocs",            fiNrOfGdpb);
   l->add("RocIdArray",          fiGdpbIdArray);
   l->add("NrOfFebsPerGdpb",     fiNrOfFebsPerGdpb);
   l->add("NrOfGet4PerFeb",      fiNrOfGet4PerFeb);
   l->add("NrOfChannelsPerGet4", fiNrOfChannelsPerGet4);
   l->add("NrOfChannels",        fiNrOfChannels);
   l->add("ChannelToDetUIdMap",  fiChannelToDetUIdMap);
   l->add("NbMsTot",             fiNbMsTot);
   l->add("NbMsOverlap",         fiNbMsOverlap);
   l->add("SizeMsInNs",          fdSizeMsInNs);
   l->add("StarTriggerDeadtime", fdStarTriggerDeadtime);
   l->add("StarTriggerDelay",    fdStarTriggerDelay);
   l->add("StarTriggerWinSize",  fdStarTriggerWinSize);
   l->add("TsDeadtimePeriod",    fdTsDeadtimePeriod);
}

//------------------------------------------------------

Bool_t CbmTofStar2018Par::getParams(FairParamList* l) {

   if (!l) return kFALSE;

   if ( ! l->fill("NrOfRocs", &fiNrOfGdpb) ) return kFALSE;

   fiGdpbIdArray.Set(fiNrOfGdpb);
   if ( ! l->fill("RocIdArray", &fiGdpbIdArray) ) return kFALSE;

   if ( ! l->fill("NrOfFebsPerGdpb", &fiNrOfFebsPerGdpb) ) return kFALSE;
   if ( ! l->fill("NrOfGet4PerFeb", &fiNrOfGet4PerFeb) ) return kFALSE;
   if ( ! l->fill("NrOfChannelsPerGet4", &fiNrOfChannelsPerGet4) ) return kFALSE;

   if ( ! l->fill("NrOfChannels", &fiNrOfChannels) ) return kFALSE;

   fiChannelToDetUIdMap.Set(fiNrOfChannels);

   if ( ! l->fill("ChannelToDetUIdMap", &fiChannelToDetUIdMap) ) return kFALSE;

   if ( ! l->fill("NbMsTot",     &fiNbMsTot) ) return kFALSE;
   if ( ! l->fill("NbMsOverlap", &fiNbMsOverlap) ) return kFALSE;
   if ( ! l->fill("SizeMsInNs",  &fdSizeMsInNs) ) return kFALSE;

   fdStarTriggerDeadtime.Set(fiNrOfGdpb);
   fdStarTriggerDelay.Set(fiNrOfGdpb);
   fdStarTriggerWinSize.Set(fiNrOfGdpb);
   if ( ! l->fill("StarTriggerDeadtime", &fdStarTriggerDeadtime) ) return kFALSE;
   if ( ! l->fill("StarTriggerDelay",    &fdStarTriggerDelay) ) return kFALSE;
   if ( ! l->fill("StarTriggerWinSize",  &fdStarTriggerWinSize) ) return kFALSE;
   if ( ! l->fill("TsDeadtimePeriod",  &fdTsDeadtimePeriod) ) return kFALSE;

   return kTRUE;
}


ClassImp(CbmTofStar2018Par)
