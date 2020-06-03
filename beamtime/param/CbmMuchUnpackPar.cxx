// -------------------------------------------------------------------------
// -----                CbmMuchUnpackPar source file                  -----
// -----                Created 11/11/16  by V. Singhal and A. Kumar                  -----
// -------------------------------------------------------------------------

#include "CbmMuchUnpackPar.h"

#include "FairParamList.h"
#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairLogger.h"

#include "TString.h"

using namespace std;
// -----   Standard constructor   ------------------------------------------
CbmMuchUnpackPar::CbmMuchUnpackPar(const char* name,
				   const char* title,
				   const char* context)
  : FairParGenericSet(name, title, context),
    fNrOfnDpbsModA(-1),
    fnDbpsIdsArrayA(),
    fNrOfnDpbsModB(-1),
    fnDbpsIdsArrayB(),
    fuNbFebPerNdpb(-1),
    fNrOfFebs(-1),
    fnFebsIdsArrayA(),
    fnFebsIdsArrayB(),
    fNrOfChannels(-1),
    fChannelsToPadX(),
    fChannelsToPadY()
{
  detName="Much";
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
CbmMuchUnpackPar::~CbmMuchUnpackPar()
{
}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmMuchUnpackPar::clear()
{
  status = kFALSE;
  resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmMuchUnpackPar::putParams(FairParamList* l)
{
  if (!l) return;
  l->add("NrOfnDpbsModA",          fNrOfnDpbsModA);
  l->add("nDbpsIdsArrayA",         fnDbpsIdsArrayA);
  l->add("NrOfnDpbsModB",          fNrOfnDpbsModB);
  l->add("nDbpsIdsArrayB",         fnDbpsIdsArrayB);
  l->add("NbFebPerNdpb",           fuNbFebPerNdpb);
  l->add("NrOfFebs",               fNrOfFebs);
  l->add("nFebsIdsArrayA",         fnFebsIdsArrayA);
  l->add("nFebsIdsArrayB",         fnFebsIdsArrayB);
  l->add("NrOfChannels",           fNrOfChannels);
  l->add("ChannelsToPadX",         fChannelsToPadX);
  l->add("ChannelsToPadY",         fChannelsToPadY);
}

//------------------------------------------------------

Bool_t CbmMuchUnpackPar::getParams(FairParamList* l) {

  if (!l) return kFALSE;

  LOG(info)<< " Before CbmMuchUnpackPar GetParams at NrOfnDpbs ";
  if ( ! l->fill("NrOfnDpbsModA", &fNrOfnDpbsModA) ) return kFALSE;
  LOG(info)<< " After CbmMuchUnpackPar GetParams at NrOfnDpbs ";

  fnDbpsIdsArrayA.Set(fNrOfnDpbsModA);
  if ( ! l->fill("nDbpsIdsArrayA", &fnDbpsIdsArrayA) ) return kFALSE;



  LOG(info)<< " Before CbmMuchUnpackPar GetParams at NrOfnDpbs for GEM2 ";
  if ( ! l->fill("NrOfnDpbsModB", &fNrOfnDpbsModB) ) return kFALSE;
  LOG(info)<< " After CbmMuchUnpackPar GetParams at NrOfnDpbs ";

  fnDbpsIdsArrayB.Set(fNrOfnDpbsModB);
  if ( ! l->fill("nDbpsIdsArrayB", &fnDbpsIdsArrayB) ) return kFALSE;

  if ( ! l->fill("NbFebPerNdpb", &fuNbFebPerNdpb) ) return kFALSE;

  if ( ! l->fill("NrOfFebs", &fNrOfFebs) ) return kFALSE;

  fnFebsIdsArrayA.Set(fNrOfFebs);
  if ( ! l->fill("nFebsIdsArrayA", &fnFebsIdsArrayA) ) return kFALSE;

  fnFebsIdsArrayB.Set(fNrOfFebs);
  if ( ! l->fill("nFebsIdsArrayB", &fnFebsIdsArrayB) ) return kFALSE;

  if ( ! l->fill("NrOfChannels", &fNrOfChannels) ) return kFALSE;

  fChannelsToPadX.Set(fNrOfFebs*fNrOfChannels);
  if ( ! l->fill("ChannelsToPadX", &fChannelsToPadX) ) return kFALSE;

  fChannelsToPadY.Set(fNrOfFebs*fNrOfChannels);
  if ( ! l->fill("ChannelsToPadY", &fChannelsToPadY) ) return kFALSE;

  return kTRUE;
}


Int_t CbmMuchUnpackPar::GetPadX(Int_t febid, Int_t channelid)
{
  if( fChannelsToPadX.GetSize () <= (febid*fNrOfChannels)+channelid )
  {
    LOG(debug) << "CbmMuchUnpackPar::GetPadX => Index out of bounds: "
	       << ((febid*fNrOfChannels)+channelid) << " VS " << fChannelsToPadX.GetSize()
	       << " (" << febid << " and " << channelid << ")";
    return -2;
  } // if( fChannelsToPadX.GetSize () <= (febid*fNrOfChannels)+channelid )


  return fChannelsToPadX[(febid*fNrOfChannels)+channelid];
}
Int_t CbmMuchUnpackPar::GetPadY(Int_t febid, Int_t channelid)
{
  if( fChannelsToPadY.GetSize() <= (febid*fNrOfChannels)+channelid )
  {
      LOG(debug) << "CbmMuchUnpackPar::GetPadY => Index out of bounds: "
                 << ((febid*fNrOfChannels)+channelid) << " VS " << fChannelsToPadY.GetSize()
                 << " (" << febid << " and " << channelid << ")";
    return -2;
  } // if( fChannelsToPadY.GetSize () <= (febid*fNrOfChannels)+channelid )

   return fChannelsToPadY[(febid*fNrOfChannels)+channelid];
}

ClassImp(CbmMuchUnpackPar)
