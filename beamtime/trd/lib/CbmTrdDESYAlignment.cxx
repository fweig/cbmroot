#include "CbmTrdDESYAlignment.h"
#include "FairLogger.h"
#include "CbmHistManager.h"
#include "TString.h"
#include "TStyle.h"

#include <cmath>
#include <map>
#include <vector>
#include <cmath>
#include <map>
#include <vector>

ClassImp(CbmTrdDESYAlignment)

CbmTrdDESYAlignment::CbmTrdDESYAlignment (TString /*_ClassName*/) :
  CbmTrdQABase ("CbmTrdDESYAlignment"), fTemporalAlign(NULL), fSpatialAlign(NULL)
{
}

void
CbmTrdDESYAlignment::CreateHistograms ()
{
  TString HistName = "Temporal_Alignment_Layers";
  fTemporalAlign = new TH2I (HistName.Data (), HistName.Data (), 8, -0.5, 7.5,
			     5001, -.5, 5000.5);
  fHm->Add (HistName.Data (), fTemporalAlign);
  fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("Layer");
  fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("Delta TimeStamp");

  HistName = "Spatial_Alignment_Layers";
  fSpatialAlign = new TH2I (HistName.Data (), HistName.Data (), 32*4, -0.5, 32*4-.5,
			     32*4, -0.5, 32*4-.5);
  fHm->Add (HistName.Data (), fSpatialAlign);
  fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("first Pad");
  fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("second Pad");


}

//----------------------------------------------------------------------------------------

std::vector<CbmSpadicRawMessage*>::iterator
CbmTrdDESYAlignment::FindNextHit (
    std::vector<CbmSpadicRawMessage*>::iterator refIt,
    std::vector<CbmSpadicRawMessage*>::iterator Current,
    std::vector<CbmSpadicRawMessage*> Messages, Long64_t TimeLimit)
{
  Long64_t RefTime = (*refIt)->GetFullTime ();
  CbmSpadicRawMessage*ref = *refIt;
  std::vector<CbmSpadicRawMessage*>::iterator end = Messages.end ();
  //std::vector<CbmSpadicRawMessage*>::iterator rend = Messages.rend ();
  for (auto it = Current; it != end; Current++)
    {
      if ((*it)->GetFullTime () > ULong_t(RefTime + TimeLimit))
	return refIt;
      if (fBT->GetColumnID (*it) == fBT->GetColumnID (ref))
	if (fBT->GetRowID (*it) == fBT->GetRowID (ref))
	  return Current;
    }
  return refIt;
}

void
CbmTrdDESYAlignment::Exec (Option_t*)
{
  static Int_t NrTimeSlice = -1;
  std::vector<std::vector<Long64_t> > LastTime (fBT->GetNrLayers ());
  LOG(info) << this->GetName () << ": Number of current TimeSlice:"
	       << ++NrTimeSlice;
  LOG(info) << this->GetName () << ": rawMessages in TS:          "
	       << fRaw->GetEntriesFast ();
  static int AlignCtr = 0;
  Int_t nSpadicMessages = fRaw->GetEntriesFast (); //SPADIC messages per TimeSlice
  std::vector<CbmSpadicRawMessage*> Messages;
  //Messages.reserve (nSpadicMessages);
  //Define Sorting algo, originally taken from CbmTrdTimeCorrel
/*
  auto CompareSpadicMessages =
      [&](CbmSpadicRawMessage* a,CbmSpadicRawMessage* b)
	{
	  Int_t SpadicAddressA = fBT->GetAddress(a);
	  Int_t SpadicAddressB = fBT->GetAddress(b);
	    {
	      if(CbmTrdAddress::GetLayerId(SpadicAddressA)<CbmTrdAddress::GetLayerId(SpadicAddressB))
		return true;
	      else if(CbmTrdAddress::GetLayerId(SpadicAddressA)>CbmTrdAddress::GetLayerId(SpadicAddressB))
		return false;
	      else
		{
		  if (CbmTrdAddress::GetRowId(SpadicAddressA)<CbmTrdAddress::GetRowId(SpadicAddressB))
		    return true;
		  else if (CbmTrdAddress::GetRowId(SpadicAddressA)>CbmTrdAddress::GetRowId(SpadicAddressB))
		    return false;
		  else
		    {
		      if (CbmTrdAddress::GetColumnId(SpadicAddressA)<CbmTrdAddress::GetColumnId(SpadicAddressB))
			return true;
		      else
			  if(a->GetFullTime() < b->GetFullTime())
			    return true;
			  else
			    return false;
		    }
		}
	    }
	};
  LOG(info) << this->GetName () << " : Sorting " << Messages.size() <<" Messages for Alignment"
	      ;
  std::sort (Messages.begin (), Messages.end(), CompareSpadicMessages);
  LOG(info) << this->GetName () << ": Sorting done";
*/
  /*if (AlignCtr > 100000)
    return;
*/
  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (raw)
	if (raw->GetHit () || raw->GetHitAborted ())
	  {
	    int next = iSpadicMessage;
	    Long64_t lastTime = raw->GetFullTime ();
	    Int_t LastPad = fBT->GetChannelOnPadPlane (raw)
		    + 32 * fBT->GetLayerID (raw);
	    do
	      {
		CbmSpadicRawMessage *rawnext =
		    static_cast<CbmSpadicRawMessage*> (fRaw->At (next));
		if (rawnext->GetHit () || rawnext->GetHitAborted ())
		  {
		    Long64_t nextTime = rawnext->GetFullTime ();
		    if (std::abs (nextTime - lastTime) < 10)
		      {
			Int_t NextPad = fBT->GetChannelOnPadPlane (rawnext)
				+ 32 * fBT->GetLayerID (rawnext);
			fSpatialAlign->Fill (LastPad, NextPad);
		      }
		  }
		next++;
	      }
	    while (next != nSpadicMessages);
	  }
      AlignCtr++;
    }

return;

  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if(raw)
	if(raw->GetHit()||raw->GetHitAborted())
	  if(fBT->GetLayerID(raw)==2)
	  {
	    Long64_t refTime=raw->GetFullTime();
	    int next=0;
	    do
		{
		  CbmSpadicRawMessage *rawnext =
		      static_cast<CbmSpadicRawMessage*> (fRaw->At (next));
		  if (rawnext->GetHit () || rawnext->GetHitAborted ())
		    {
		      if (fBT->GetColumnID (rawnext) == fBT->GetColumnID (raw))
			if (fBT->GetRowID (rawnext) == fBT->GetRowID (raw))
			  {
			    Long64_t nextTime = rawnext->GetFullTime ();
			    fTemporalAlign->Fill (fBT->GetLayerID (rawnext),
						  nextTime - refTime);
			  }
		    }
		  next++;
		}
      while (next != nSpadicMessages);
    }
    }
}

