// -------------------------------------------------------------------------
// -----                 CbmMcbm2018PsdPar header file                 -----
// -----              Created 26.09.2019 by N.Karpushkin               -----
// -----         based on CbmMcbm2018TofPar by P.-A. Loizeau           -----
// -------------------------------------------------------------------------

#include "CbmMcbm2018PsdPar.h"

#include "FairParamList.h"
#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairLogger.h"

#include "TString.h"

// -----   Standard constructor   ------------------------------------------
CbmMcbm2018PsdPar::CbmMcbm2018PsdPar(const char* name,
			             const char* title,
			             const char* context)
  : FairParGenericSet(name, title, context),
    fiMonitorMode(-1),
    fiDebugMonitorMode(-1),
    fiNrOfGdpb(-1),
    fiGdpbIdArray(),
    fiNrOfFeesPerGdpb(-1),
    fiNrOfChannelsPerFee(-1),
    fiNrOfGbtx(-1),
    fiNrOfModules( -1 ),
    fiModuleId(),
    fiNbMsTot(0),
    fiNbMsOverlap(0),
    fdSizeMsInNs(0.0),
    fdStarTriggerDeadtime(),
    fdStarTriggerDelay(),
    fdStarTriggerWinSize(),
    fdTsDeadtimePeriod(0.0)
{
   detName="Psd";
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
CbmMcbm2018PsdPar::~CbmMcbm2018PsdPar()
{
}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmMcbm2018PsdPar::clear()
{
   status = kFALSE;
   resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmMcbm2018PsdPar::putParams(FairParamList* l)
{
   if (!l) return;
   l->add("NrOfGdpbs",           fiNrOfGdpb);
   l->add("GdpbIdArray",         fiGdpbIdArray);
   l->add("NrOfFeesPerGdpb",     fiNrOfFeesPerGdpb);
   l->add("NrOfChannelsPerFee",  fiNrOfChannelsPerFee);
   l->add("NrOfGbtx",            fiNrOfGbtx);
   l->add("NrOfModules",         fiNrOfModules);
   l->add("ModuleId",            fiModuleId);
   l->add("NbMsTot",             fiNbMsTot);
   l->add("NbMsOverlap",         fiNbMsOverlap);
   l->add("SizeMsInNs",          fdSizeMsInNs);
   l->add("StarTriggerDeadtime", fdStarTriggerDeadtime);
   l->add("StarTriggerDelay",    fdStarTriggerDelay);
   l->add("StarTriggerWinSize",  fdStarTriggerWinSize);
   l->add("TsDeadtimePeriod",    fdTsDeadtimePeriod);
}

//------------------------------------------------------

Bool_t CbmMcbm2018PsdPar::getParams(FairParamList* l) {

   if (!l) return kFALSE;

   if ( ! l->fill("NrOfGdpbs", &fiNrOfGdpb) ) return kFALSE;

   fiGdpbIdArray.Set(fiNrOfGdpb);
   if ( ! l->fill("GdpbIdArray", &fiGdpbIdArray) ) return kFALSE;

   if ( ! l->fill("NrOfFeesPerGdpb", &fiNrOfFeesPerGdpb) ) return kFALSE;

   if ( ! l->fill("NrOfChannelsPerFee", &fiNrOfChannelsPerFee) ) return kFALSE;

   if ( ! l->fill("NrOfGbtx", &fiNrOfGbtx) ) return kFALSE;

   if ( ! l->fill("NrOfModules", &fiNrOfModules) ) return kFALSE;

   fiModuleId.Set(fiNrOfGbtx);
   if ( ! l->fill("ModuleId", &fiModuleId) ) return kFALSE;

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
// -------------------------------------------------------------------------
Int_t CbmMcbm2018PsdPar::FeeChanToGbtChan( UInt_t uChannelInFee )
{
   if( uChannelInFee < kuNbChannelsPerFee )
      return kuFeeToGbt[ uChannelInFee ];
      else
      {
         LOG(fatal) << "CbmMcbm2018PsdPar::FeeChanToGbtChan => Index out of bound, "
                    << uChannelInFee << " vs " << static_cast< const uint32_t >( kuNbChannelsPerFee )
                    << ", returning crazy value!";
         return -1;
      } // else of if( uChannelInFee < kuNbChannelsPerFee )
}

// -------------------------------------------------------------------------


ClassImp(CbmMcbm2018PsdPar)
