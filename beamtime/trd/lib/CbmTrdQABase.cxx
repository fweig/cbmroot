/**
 *Minimal example of an CBM TRD QA analysis class.
 *Any daughter class only needs to implement the CreateHistograms() and the Exec() functions.
 *A future version will be parametriseable for different in-beam tests.
 */
#include "CbmTrdQABase.h"
#include "CbmSpadicRawMessage.h"
#include "CbmTrdTestBeamTools.h"
//#include "CbmBeamDefaults.h"

#include "FairLogger.h"

#include "CbmHistManager.h"

#include "TString.h"
#include "TStyle.h"
#include <TFile.h>

#include <cmath>
#include <map>
#include <vector>

ClassImp(CbmTrdQABase)

// ---- Default constructor -------------------------------------------
CbmTrdQABase::CbmTrdQABase (CbmTrdTestBeamTools*ptr, TString _ClassName) :
    FairTask (_ClassName.Data ()), fRaw (nullptr), fClusters (
	nullptr), fBT (CbmTrdTestBeamTools::Instance (ptr)), fHm (
	new CbmHistManager), fNrTimeslice (0)
{
  LOG(info) << TString ("Default Constructor of ") + TString (this->GetName ());
}

// ---- Destructor ----------------------------------------------------
CbmTrdQABase::~CbmTrdQABase ()
{
  LOG(debug) << TString ("Destructor of ") + TString (this->GetName ());
}

// ----  Initialisation  ----------------------------------------------
void
CbmTrdQABase::SetParContainers ()
{
  LOG(debug) << TString ("SetParContainers of ") + TString (this->GetName ());
  // Load all necessary parameter containers from the runtime data base
  /*
   FairRunAna* ana = FairRunAna::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   <CbmTrdQABaseDataMember> = (<ClassPointer>*)
   (rtdb->getContainer("<ContainerName>"));
   */
}

// ---- Init ----------------------------------------------------------
InitStatus
CbmTrdQABase::Init ()
{
  LOG(debug) << TString ("Initilization of ") + TString (this->GetName ());

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance ();

  // Get a pointer to the previous already existing data level
  fRaw = static_cast<TClonesArray*> (ioman->GetObject ("SpadicRawMessage"));
  if (!fRaw)
    {
      LOG(fatal) << "No InputDataLevelName array!\n " << this->GetName ()
		    << " will be inactive";
      return kERROR;
    }
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (fDigiMan->IsPresent(ECbmModuleId::kTrd))
    {
      LOG(info) << "Digi Input Available";
    }
  TClonesArray* tempArray = static_cast<TClonesArray*> (ioman->GetObject ("TrdCluster"));
  if (tempArray)
    {
      LOG(info) << "Cluster Input Available";
      fClusters = tempArray;
    }
  CreateHistograms ();
  return kSUCCESS;
}

// ---- ReInit  -------------------------------------------------------
InitStatus
CbmTrdQABase::ReInit ()
{
  LOG(debug) << "Initilization of " << this->GetName ();
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void
CbmTrdQABase::Exec (Option_t*)
{
  static Bool_t FirstTS = true;
  if (FirstTS)
    {
      fNrTimeslice = -1;
      FirstTS = false;
    }
  LOG(info) << this->GetName () << ": Number of current TimeSlice:"
	       << ++fNrTimeslice;
  LOG(info) << this->GetName () << ": rawMessages in TS:          "
	       << fRaw->GetEntriesFast ();
  LOG(debug) << this->GetName () << ": Begin sorting CbmSpadicMessage*";
  Int_t nSpadicMessages = fRaw->GetEntriesFast (); //SPADIC messages per TimeSlice
  TGraph* EinzelHistogram = fHm->G1 ("TSCounter");
  std::vector<TGraph*> HistogramArray;
  for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
    {
      for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics () * 2; SpadicID++)
	{
	  HistogramArray.push_back (
	      fHm->G1 (
		  TString (
		      "TSCounter"
			  + GetSpadicName (RobID, SpadicID, "Syscore", false)).Data ()));
	}
    }
  EinzelHistogram->SetPoint (fNrTimeslice, fNrTimeslice, nSpadicMessages);
  std::vector<Int_t> MessageCounters (
      fBT->GetNrRobs () * fBT->GetNrSpadics () * 2, 0);
  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (!raw)
	continue;
      if (static_cast<size_t>(GetRobID (raw) * fBT->GetNrSpadics () * 2 + GetSpadicID (raw))
	  > HistogramArray.size ()
	  || static_cast<size_t>(GetRobID (raw) * fBT->GetNrSpadics () * 2 + GetSpadicID (raw))
	      > MessageCounters.size ())
	continue;
      if (fBT->GetSpadicID (raw) > 0)
	MessageCounters.at (
	    GetRobID (raw) * fBT->GetNrSpadics () * 2 + GetSpadicID (raw))++;}
  for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
    {
      for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics () * 2; SpadicID++)
	{
	  HistogramArray.at (RobID * fBT->GetNrSpadics () * 2 + SpadicID)->SetPoint (
	      fNrTimeslice, fNrTimeslice,
	      MessageCounters.at (RobID * fBT->GetNrSpadics () * 2 + SpadicID));
	}
    }
  std::vector<Long64_t> MaxTime(2*fBT->GetNrSpadics()*fBT->GetNrRobs ());
  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (!raw)	continue;

      //LOG(info) << "Find Values out of Bounds Fulltime:"  << raw->GetFullTime() << "   GetRobID:"  << fBT->GetRobID (raw) << "   GetSpadicID:" << fBT->GetSpadicID(raw);

	if ((Long64_t(raw->GetFullTime())>MaxTime[2*fBT->GetNrSpadics()*fBT->GetRobID (raw)+fBT->GetSpadicID(raw)])&&(fBT->GetSpadicID(raw)>=0))

	if(((static_cast<size_t>(2*fBT->GetNrSpadics()*fBT->GetRobID (raw)+fBT->GetSpadicID(raw))))<MaxTime.size())
	{
			MaxTime.at(2*fBT->GetNrSpadics()*fBT->GetRobID (raw)+fBT->GetSpadicID(raw))=raw->GetFullTime();
    }

	}

  for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
    {
      for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics () * 2; SpadicID++)
	{
	  TString GraphName = "Time_vs_TS_"
	      + GetSpadicName (RobID, SpadicID, "Afck", false);
	  TGraph* currentGraph =fHm->G1(GraphName.Data());
	  currentGraph->SetPoint(currentGraph->GetN(), fNrTimeslice,static_cast<double>(MaxTime.at(2*fBT->GetNrSpadics()*RobID+SpadicID)));
	}
    }
}
// ---- Create Histograms----------------------------------------------
void
CbmTrdQABase::CreateHistograms ()
{
  //TODO: Implement with CbmTrdTestBeamTools
  fHm->Add ("TSCounter", new TGraph);
  fHm->G1 ("TSCounter")->SetNameTitle ("TSCounter", "TSCounter");
  fHm->G1 ("TSCounter")->GetXaxis ()->SetTitle ("TS Number");
  fHm->G1 ("TSCounter")->GetYaxis ()->SetTitle ("Message Counter");
  for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
    {
      for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics () * 2; SpadicID++)
	{
	  fHm->Add (
	      TString (
		  "TSCounter"
		      + GetSpadicName (RobID, SpadicID, "Syscore", false)).Data (),
	      new TGraph);
	  TString GraphName = "TSCounter"
	      + GetSpadicName (RobID, SpadicID, "Syscore", false);
	  fHm->G1 (GraphName.Data ())->SetNameTitle (GraphName.Data (),
						     GraphName.Data ());
	  fHm->G1 (GraphName.Data ())->GetXaxis ()->SetTitle ("TS Number");
	  fHm->G1 (GraphName.Data ())->GetYaxis ()->SetTitle (
	      "Message Counter");
	}
    }
  for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
    {
      for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics () * 2; SpadicID++)
	{
	  TString GraphName = "Time_vs_TS_"
	      + GetSpadicName (RobID, SpadicID, "Afck", false);
	  fHm->Add (GraphName.Data (), new TGraph);
	  fHm->G1 (GraphName.Data ())->SetNameTitle (GraphName.Data (),
						     GraphName.Data ());
	  fHm->G1 (GraphName.Data ())->GetXaxis ()->SetTitle ("TS Number");
	  fHm->G1 (GraphName.Data ())->GetYaxis ()->SetTitle ("FullTime");
	}
    }
}

// ---- FinishEvent----------------------------------------------------
/*void CbmTrdQABase::FinishEvent ()
 {
 LOG(debug) << "FinishEvent of CbmTrdQABase";
 fProcSpadic->Clear ();
 }*/

// ---- Finish --------------------------------------------------------
void
CbmTrdQABase::Finish ()
{
  LOG(debug) << "Finish of " << this->GetName ();
  FairRootManager::Instance ()->GetOutFile ()->cd ();
  //Save Histograms
  LOG(info) << "Saving Histograms of " << this->GetName ();
  gDirectory->mkdir (this->GetName ());
  gDirectory->Cd (this->GetName ());
  fHm->WriteToFile ();
  FairRootManager::Instance ()->GetOutFile ()->cd ();
  LOG(debug) << "Finish of " << this->GetName () << " done.";
}/*
 void CbmTrdQABase::FinishTask ()
 {
 LOG(debug) << "Finish of CbmTrdQABase";
 // Update Histos and Canvases
 }*/
