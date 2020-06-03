#include "CbmTrdQAHit.h"
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

ClassImp(CbmTrdQAHit)

CbmTrdQAHit::CbmTrdQAHit (TString /*_ClassName*/) :
    CbmTrdQABase ("CbmTrdQAHit")
{
}

void
CbmTrdQAHit::CreateHistograms ()
{
  for (Int_t syscore = 0; syscore < fBT->GetNrRobs (); ++syscore)
    {
      for (Int_t spadic = 0; spadic < fBT->GetNrSpadics (); ++spadic)
	{
	  TString spadicName = GetSpadicName (syscore, spadic, "SysCore", true);
	  TString HistName = "TriggerType_vs_StopType_" + spadicName;
	  fHm->Add (
	      HistName.Data (),
	      new TH2I (HistName.Data (), HistName.Data (), 5, -1.5, 3.5, 7,
			-1.5, 5.5));
	  fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("StopType");
	  fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("TriggerType");
	  //Triggertype vs Nr of Samples:
	  HistName = "NrOfSamples_vs_TriggerType_" + spadicName;
	  fHm->Add (
	      HistName.Data (),
	      new TH2I (HistName.Data (), HistName.Data (), 33, -0.5, 32.5, 5,
			-1.5, 3.5));
	  fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("NrOfSamples");
	  fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("TriggerType");
	  HistName = "NrOfSamples_vs_StopType_" + spadicName;
	  fHm->Add (
	      HistName.Data (),
	      new TH2I (HistName.Data (), HistName.Data (), 33, -0.5, 32.5, 7,
			-1.5, 5.5));
	  fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("NrOfSamples");
	  fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("StopType");
	}
    }
  //Triggertype vs Channel:
  for (Int_t syscore = 0; syscore < fBT->GetNrRobs (); ++syscore)
    {
      for (Int_t spadic = 0; spadic < fBT->GetNrSpadics (); ++spadic)
	{
	  TString spadicName = GetSpadicName (syscore, spadic, "SysCore", true);
	  TString HistName = "TriggerType_vs_Channel_" + spadicName;
	  fHm->Add (
	      HistName.Data (),
	      new TH2I (HistName.Data (), HistName.Data (), 32, -0.5, 31.5, 4,
			-0.5, 3.5));
	  fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("Channel");
	  fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("TriggerType");
	}
    }

  for (Int_t syscore = 0; syscore < fBT->GetNrRobs (); ++syscore)
    {
      for (Int_t spadic = 0; spadic < fBT->GetNrSpadics (); ++spadic)
	{
	  TString spadicName = GetSpadicName (syscore, spadic, "SysCore", true);
	  TString HistName = "Hitfrequency_vs_Time_" + spadicName;
	  fHm->Add (HistName.Data (), new TGraph ());
	  fHm->G1 (HistName.Data ())->SetNameTitle (HistName.Data (),
						    HistName.Data ());
	  fHm->G1 (HistName.Data ())->GetYaxis ()->SetTitle ("HitFrequency");
	  fHm->G1 (HistName.Data ())->GetXaxis ()->SetTitle ("Fulltime/s");
	}
    }
  for (Int_t layer = 0; layer < fBT->GetNrLayers (); layer++)
    {
      TString HistName = "Hitmap_Layer_" + std::to_string (layer);
      fHm->Add (
	  HistName.Data (),
	  new TH2I (HistName.Data (), HistName.Data (), fBT->GetNrColumns(layer), -0.5, fBT->GetNrColumns(layer)-0.5, fBT->GetNrRows(layer),
		    -0.5, fBT->GetNrRows(layer)-0.5));
      fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("Column");
      fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("Row");
    }
  for (Int_t syscore = 0; syscore < fBT->GetNrRobs (); syscore++)
    {
      for (Int_t spadic = 0; spadic < fBT->GetNrSpadics () * 2; spadic++)
	{
	  TString spadicName = GetSpadicName (syscore, spadic, "SysCore", true);
	  TString HistName = "Hitwindow_" + spadicName;
	  fHm->Add (
	      HistName.Data (),
	      new TH1I (HistName.Data (), HistName.Data (), 4096, 0.0 - 0.05,
			4096.0 * 4096.0 / 8.0 - 0.05));
	  fHm->H1 (HistName.Data ())->GetXaxis ()->SetTitle ("HW Length");
	  fHm->H1 (HistName.Data ())->GetYaxis ()->SetTitle ("Counts");
	}
    }
  for (Int_t syscore = 0; syscore < fBT->GetNrRobs (); syscore++)
    {
      for (Int_t spadic = 0; spadic < fBT->GetNrSpadics () ;
	  spadic++)
	{
	  for (Int_t channel = 0; channel < 32; channel++)
	    {
	      for (Int_t trigger = 0; trigger < 4; trigger++)
		{
		  TString spadicName = GetSpadicName (syscore, spadic, "Afck",
						      true);
		  TString HistName = "SignalShape_Trigger_"
		      + std::to_string (trigger) + "_" + spadicName
		      + "_Channel_" + std::to_string (channel);
		  fHm->Add (
		      HistName.Data (),
		      new TH2I (HistName.Data (), HistName.Data (), 32, -0.5,
				31.5, 512, -256.5, 255.5));
		  fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle (
		      "Sample Nr");
		  fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle (
		      "ADC Value");
		}
	    }
	}
    }

}

//----------------------------------------------------------------------------------------

void
CbmTrdQAHit::Exec (Option_t*)
{
  static Int_t NrTimeSlice = -1;
  std::vector<Double_t> LastHit (fBT->GetNrRobs () * fBT->GetNrSpadics () * 32);
    LOG(info) << this->GetName() <<": Number of current TimeSlice:"
   << ++NrTimeSlice;
   LOG(info) << this->GetName() <<": rawMessages in TS:          "
   << fRaw->GetEntriesFast ();

  Int_t nSpadicMessages = fRaw->GetEntriesFast (); //SPADIC messages per TimeSlice
  std::vector<TH2*> HistogramArray;
  std::vector<TGraph*> HitfrequencyGraphs;
  for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
    {
      for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics (); SpadicID++)
	{
	  TString spadicName = GetSpadicName (RobID, SpadicID, "SysCore", true);
	  TString HistName = "TriggerType_vs_StopType_" + spadicName;
	  HistogramArray.push_back (fHm->H2 (HistName.Data ()));
	  HistName = "Hitfrequency_vs_Time_" + spadicName;
	  HitfrequencyGraphs.push_back (fHm->G1 (HistName.Data ()));
	}
    }
  //first Loop for TriggerType_vs_StopType:
  LOG(info) << this->GetName () << ": Loop for TriggerType_vs_StopType Start";
  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (static_cast<size_t>(GetRobID (raw) * fBT->GetNrSpadics () + GetSpadicID (raw) / 2)
	  > HistogramArray.size ())
	continue;
      HistogramArray.at (
	  fBT->GetRobID (raw) * fBT->GetNrSpadics () + GetSpadicID (raw) / 2)->Fill (
	  raw->GetTriggerType (), raw->GetStopType ());
    }
  LOG(info) << this->GetName () << ": Loop for TriggerType_vs_StopType done";
  //Loop for TriggerType_vs_Channel_:
  HistogramArray.clear ();
  for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
    {
      for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics (); SpadicID++)
	{
	  TString spadicName = GetSpadicName (RobID, SpadicID, "SysCore", true);
	  TString HistName = "TriggerType_vs_Channel_" + spadicName;
	  HistogramArray.push_back (fHm->H2 (HistName.Data ()));
	}
    }

  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (static_cast<size_t>(GetRobID (raw) * fBT->GetNrSpadics () + GetSpadicID (raw) / 2)>HistogramArray.size())
	std::cout << GetRobID (raw) << " "<< fBT->GetNrSpadics () << " "<< GetSpadicID (raw)<<std::endl;
      if (raw->GetHit () || raw->GetHitAborted ())
	HistogramArray.at (
	    GetRobID (raw) * fBT->GetNrSpadics () + GetSpadicID (raw) / 2)->Fill (
		(GetSpadicID (raw) % 2) * 16 + raw->GetChannelID (),
	    raw->GetTriggerType ());
    }
  LOG(info) << this->GetName () << ": Loop for TriggerType_vs_Channel done";

  //Loop for NrOfSamples_vs_TriggerType:
  HistogramArray.clear ();
  for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
    {
      for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics (); SpadicID++)
	{
	  TString spadicName = GetSpadicName (RobID, SpadicID, "SysCore", true);
	  TString HistName = "NrOfSamples_vs_TriggerType_" + spadicName;
	  HistogramArray.push_back (fHm->H2 (HistName.Data ()));
	}
    }
  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (raw->GetHit () || raw->GetHitAborted ())
	HistogramArray.at (
	    GetRobID (raw) * fBT->GetNrSpadics () + GetSpadicID (raw) / 2)->Fill (
	    raw->GetNrSamples (), raw->GetTriggerType ());
    }
  LOG(info) << this->GetName () << ": Loop for NrOfSamples_vs_TriggerType done";
  //Loop for NrOfSamples_vs_StopType:
  HistogramArray.clear ();
  for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
    {
      for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics (); SpadicID++)
	{
	  TString spadicName = GetSpadicName (RobID, SpadicID, "SysCore", true);
	  TString HistName = "NrOfSamples_vs_StopType_" + spadicName;
	  HistogramArray.push_back (fHm->H2 (HistName.Data ()));
	}
    }
  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (raw->GetHit () || raw->GetHitAborted ())
	HistogramArray.at (
	    GetRobID (raw) * fBT->GetNrSpadics () + GetSpadicID (raw) / 2)->Fill (
	    raw->GetNrSamples (), raw->GetStopType ());
    }
  LOG(info) << this->GetName () << ": Loop for NrOfSamples_vs_StopType done";

  std::vector<TH2*> Hitmaps;
  for (Int_t layer = 0; layer < fBT->GetNrLayers (); layer++)
    {
      TString HistName = "Hitmap_Layer_" + std::to_string (layer);
      Hitmaps.push_back (fHm->H2 (HistName.Data ()));
    }
  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (!(raw->GetHit () || raw->GetHitAborted ()))
	{
	  continue;
	}
      Hitmaps.at (GetLayerID (raw))->Fill (GetColumnID (raw), GetRowID (raw));
    }
  LOG(info) << this->GetName () << ": Loop for Hitmaps done";

    {
      std::vector<TH1*> Histogram1DArray;
      for (Int_t RobID = 0; RobID < fBT->GetNrRobs (); RobID++)
	{
	  for (Int_t SpadicID = 0; SpadicID < fBT->GetNrSpadics () * 2;
	      SpadicID++)
	    {
	      TString spadicName = GetSpadicName (RobID, SpadicID, "SysCore",
						  true);
	      TString HistName = "Hitwindow_" + spadicName;
	      Histogram1DArray.push_back (fHm->H1 (HistName.Data ()));
	    }
	}
      //Loop for Hitwindowbuffer:
	{
	  std::vector<Long64_t> FirstHitTime (
	      fBT->GetNrRobs () * fBT->GetNrSpadics () * 2), LastHitTime (
	      fBT->GetNrRobs () * fBT->GetNrSpadics () * 2);
	  static Int_t LastHalfSpadic = 0;
	  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
	      ++iSpadicMessage)
	    {
	      CbmSpadicRawMessage *raw =
		  static_cast<CbmSpadicRawMessage*> (fRaw->At (iSpadicMessage));
	      if (!(raw->GetHit () || raw->GetHitAborted ()))
		continue;
	      Int_t CurrentHalfSpadic = GetRobID (raw) * fBT->GetNrSpadics ()
		  * 2 + GetSpadicID (raw);
	      if (CurrentHalfSpadic != LastHalfSpadic)
		{
		  Histogram1DArray.at (LastHalfSpadic)->Fill (
		      LastHitTime.at (LastHalfSpadic)
			  - FirstHitTime.at (LastHalfSpadic));
		  LastHalfSpadic = CurrentHalfSpadic;
		  FirstHitTime.at (CurrentHalfSpadic) = raw->GetFullTime ();
		}
	      LastHitTime.at (CurrentHalfSpadic) = raw->GetFullTime ();
	    }
	}
      LOG(info) << this->GetName () << ": Loop for Hitwindowbuffer done";
      //Loop for NrOfSamples_vs_TriggerType:
      Histogram1DArray.clear ();
    }
  const Double_t SpadicFrequency = 1E9 / fBT->GetSamplingTime ();
  for (Int_t iSpadicMessage = 0; iSpadicMessage < 0;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (!(raw->GetHit () || raw->GetTriggerType () == 1))
	continue;
      TGraph* CurrentGraph = HitfrequencyGraphs.at (
	  GetRobID (raw) * fBT->GetNrSpadics () + GetSpadicID (raw) / 2);
      Long64_t CurrentTime = raw->GetFullTime ();
      UInt_t Index = GetRobID (raw) * fBT->GetNrSpadics () * 32
	  + GetSpadicID (raw) * 16 + raw->GetChannelID ();
      Double_t CurrentFrequency = SpadicFrequency
	  / static_cast<Double_t> (CurrentTime - LastHit.at (Index));
      LastHit.at (Index) = CurrentTime;
      if (std::isinf (CurrentFrequency) || std::isnan (CurrentFrequency)
	  || CurrentFrequency < 10.0)
	continue;
      CurrentGraph->SetPoint (CurrentGraph->GetN (),
			      CurrentTime / (SpadicFrequency),
			      CurrentFrequency);
    }
  LOG(info) << this->GetName () << ": Loop for Hitfrequency_vs_Time done";
  if (nSpadicMessages != 0)
    {
      std::vector<TH2*> SignalShapes (
	  static_cast<size_t> (fBT->GetNrRobs () * fBT->GetNrSpadics () * 32 * 5));
      for (Int_t afck = 0; afck < fBT->GetNrRobs (); afck++)
	{
	  for (Int_t spadic = 0; spadic < fBT->GetNrSpadics (); spadic++)
	    {
	      for (Int_t channel = 0; channel < 32; channel++)
		{
		  for (Int_t trigger = 0; trigger < 4; trigger++)
		    {
		      TString spadicName = GetSpadicName (afck, spadic, "Afck",
							  true);
		      TString HistName = "SignalShape_Trigger_"
			  + std::to_string (trigger) + "_" + spadicName
			  + "_Channel_" + std::to_string (channel);
		      SignalShapes.at (
			  afck * fBT->GetNrSpadics () * 32 * 4 + spadic * 32 * 4
			      + channel * 4 + trigger) = fHm->H2 (
			  HistName.Data ());
		    }
		}
	    }
	}
      LOG(info) << this->GetName () << ": Loop for Signalshapes start";
      for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
	  ++iSpadicMessage)
	{
	  CbmSpadicRawMessage *raw =
	      static_cast<CbmSpadicRawMessage*> (fRaw->At (iSpadicMessage));
	  if (!(raw->GetHit () || raw->GetHitAborted ()))
	    continue;
	  int afck = fBT->GetRobID (raw);
	  int spadic = fBT->GetSpadicID (raw) / 2;
	  int trigger = raw->GetTriggerType ();
	  int channel = raw->GetChannelID () + 16 * (fBT->GetSpadicID (raw) % 2);	  TH2* CurrentHisto = SignalShapes.at (
	      afck * fBT->GetNrSpadics () * 32 * 4 + spadic * 32 * 4
		  + channel * 4 + trigger);
	  Int_t* Samples = raw->GetSamples ();
	  for (int i = 0; i < raw->GetNrSamples (); i++)
	    {
	      CurrentHisto->Fill (i, Samples[i]);
	    }
	}

      LOG(info) << this->GetName () << ": Loop for Signalshapes done";
  }
}

