// -------------------------------------------------------------------------
// -----                CbmFHodoUnpackPar source file                  -----
// -----                Created 02/11/16  by F. Uhlig                  -----
// -------------------------------------------------------------------------

#include "CbmFHodoUnpackPar.h"

#include "FairParamList.h"
#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairLogger.h"

#include "TString.h"

// -----   Standard constructor   ------------------------------------------
CbmFHodoUnpackPar::CbmFHodoUnpackPar(const char* name, 
			             const char* title,
			             const char* context)
  : FairParGenericSet(name, title, context), 
    fNrOfRocs(-1),
    fRocIdArray(),
    fNrOfChannels(-1),
	fChannelToFiberMap(),
	fChannelToPixelMap(),
	fChannelToPlaneMap()
{
  detName="FHodo";
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
CbmFHodoUnpackPar::~CbmFHodoUnpackPar() 
{
}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmFHodoUnpackPar::clear() 
{
  status = kFALSE;
  resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmFHodoUnpackPar::putParams(FairParamList* l) 
{
  if (!l) return;
  l->add("NrOfRocs",          fNrOfRocs);
  l->add("RocIdArray",        fRocIdArray);
  l->add("NrOfChannels",      fNrOfRocs);
  l->add("ChannelToFiberMap", fChannelToFiberMap);
  l->add("ChannelToPixelMap", fChannelToPixelMap);
  l->add("ChannelToPlaneMap", fChannelToPlaneMap);
}

//------------------------------------------------------

Bool_t CbmFHodoUnpackPar::getParams(FairParamList* l) {

  if (!l) return kFALSE;
  
  if ( ! l->fill("NrOfRocs", &fNrOfRocs) ) return kFALSE;
  
  fRocIdArray.Set(fNrOfRocs);
  if ( ! l->fill("RocIdArray", &fRocIdArray) ) return kFALSE;

  if ( ! l->fill("NrOfChannels", &fNrOfChannels) ) return kFALSE;

  fChannelToFiberMap.Set(fNrOfChannels);
  fChannelToPixelMap.Set(fNrOfChannels);
  fChannelToPlaneMap.Set(fNrOfChannels);
  if ( ! l->fill("ChannelToFiberMap", &fChannelToFiberMap) ) return kFALSE;
  if ( ! l->fill("ChannelToPixelMap", &fChannelToPixelMap) ) return kFALSE;
  if ( ! l->fill("ChannelToPlaneMap", &fChannelToPlaneMap) ) return kFALSE;

  return kTRUE;
}


ClassImp(CbmFHodoUnpackPar)
