/*
 * CbmTrdQACosmics.cxx
 *
 *  Created on: Apr 22, 2017
 *      Author: philipp
 */

#include <CbmTrdQACosmics.h>

ClassImp(CbmTrdQACosmics)

CbmTrdQACosmics::CbmTrdQACosmics ()
    : CbmTrdQABase ("CbmTrdQACosmics")
{
  // TODO Auto-generated constructor stub

}

CbmTrdQACosmics::~CbmTrdQACosmics ()
{
  // TODO Auto-generated destructor stub
}

void
CbmTrdQACosmics::CreateHistograms ()
{
  for (Int_t Layer = 0; Layer <fBT->GetNrLayers (); ++Layer)
    {
      for (Int_t Row = 0; Row < fBT->GetNrRows(Layer) ; ++Row)
        {
          for (Int_t Col = 0; Col < fBT->GetNrColumns(Layer); Col++)
            {
              TString HistName = "Signalshape_Layer" + std::to_string(Layer) + "Row"+std::to_string(Row)+"Column"
                  + std::to_string (Col);
              fHm->Create2<TH2I> (HistName.Data (), HistName.Data (), 32, -0.5,
                                  31.5, 512, -256.5, 255.5);
              fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle (
                  "ADC-Value");
              fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("Timebin");
            }
        }
    }
  for(Int_t syscore = 0; syscore < fBT->GetNrRobs(); ++syscore) {
    for(Int_t spadic = 0; spadic < fBT->GetNrSpadics(); ++spadic) {
      TString spadicName = GetSpadicName(syscore, spadic, "SysCore",true);
      TString HistName="Lost_Hitratio_vs_Time_"+spadicName;
      fHm->Add(HistName.Data(), new TGraph());
      fHm->G1(HistName.Data())->SetNameTitle(HistName.Data(),HistName.Data());
      fHm->G1(HistName.Data())->GetYaxis()->SetTitle("Lost_Hitratio");
      fHm->G1(HistName.Data())->GetXaxis()->SetTitle("Time/s");
    }
  }
  for(Int_t syscore = 0; syscore < fBT->GetNrRobs(); ++syscore) {
    for(Int_t spadic = 0; spadic < fBT->GetNrSpadics(); ++spadic) {
      TString spadicName = GetSpadicName(syscore, spadic, "SysCore",true);
      TString HistName="Avg_StopType_vs_Time_"+spadicName;
      fHm->Add(HistName.Data(), new TGraph());
      fHm->G1(HistName.Data())->SetNameTitle(HistName.Data(),HistName.Data());
      fHm->G1(HistName.Data())->GetYaxis()->SetTitle("Lost_Hitratio");
      fHm->G1(HistName.Data())->GetXaxis()->SetTitle("Time/s");
    }
  }
}

void
CbmTrdQACosmics::Exec (Option_t*)
{
  Int_t NrDigis = fRaw->GetEntries();
  LOG(info) << this->GetName () << ": Analyzing Signalshape of " << NrDigis
               << " Digis ";
  for (Int_t Index = 0; Index < NrDigis; Index++)
    {
      auto raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (Index));
      if (!(raw->GetHit()||raw->GetHitAborted()))
	continue;
      if (fBT->GetSpadicID(raw)<0)
	continue;
//      Int_t Address=fBT->GetAddress(raw);
      Int_t Layer=fBT->GetLayerID(raw);
      Int_t Row=fBT->GetRowID(raw);
      Int_t Col=fBT->GetColumnID(raw);
      TString HistName = "Signalshape_Layer" + std::to_string(Layer) + "Row"+std::to_string(Row)+"Column"
          + std::to_string (Col);
      TH2* Hist=fHm->H2(HistName.Data());
      if(raw->GetTriggerType()==3)
	continue;
      for(Int_t Sample=0;Sample<raw->GetNrSamples();Sample++)
        Hist->Fill(Sample,raw->GetSamples()[Sample]);
    }
  LOG(debug) << this->GetName () << ": Done Analyzing Signalshape of " << NrDigis
               << " Digis ";
  LOG(info) << this->GetName () << ": Analyzing Lost_Ratio ";
  std::vector<Int_t> LostMessagesTotal(fBT->GetNrRobs()*fBT->GetNrSpadics()*2);
  std::vector<Int_t> HitsTotal(fBT->GetNrRobs()*fBT->GetNrSpadics()*2);
  for (Int_t Index = 0; Index < NrDigis; Index++)
    {
      auto raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (Index));
      if((fBT->GetRobID(raw)<0)||fBT->GetSpadicID(raw)<0)
	continue;
      if ((raw->GetHit()||raw->GetHitAborted())){
	++HitsTotal.at(fBT->GetNrSpadics()*fBT->GetRobID(raw)*2+fBT->GetSpadicID(raw));	
      }
      if (raw->GetOverFlow()){
	LostMessagesTotal.at(fBT->GetNrSpadics()*fBT->GetRobID(raw)*2+fBT->GetSpadicID(raw))+=raw->GetBufferOverflowCount();
      }
    }
    for(Int_t syscore = 0; syscore < fBT->GetNrRobs(); ++syscore) {
      for(Int_t spadic = 0; spadic < fBT->GetNrSpadics(); ++spadic) {
	TString spadicName = GetSpadicName(syscore, spadic, "SysCore",true);
	TString HistName="Lost_Hitratio_vs_Time_"+spadicName;
	if(HitsTotal.at(fBT->GetNrSpadics()*syscore*2+spadic)<=0)
	  continue;
	float LostRatio=static_cast<float>(LostMessagesTotal.at(fBT->GetNrSpadics()*syscore*2+spadic))/HitsTotal.at(fBT->GetNrSpadics()*syscore*2+spadic);
	std::cout << fNrTimeslice << " " << HitsTotal.at(fBT->GetNrSpadics()*syscore*2+spadic) << " " << LostMessagesTotal.at(fBT->GetNrSpadics()*syscore*2+spadic) << " " << LostRatio <<std::endl;
	fHm->G1(HistName.Data())->SetPoint(fNrTimeslice,static_cast<Float_t>(fNrTimeslice)/100.0,LostRatio);
      }
    }
  LOG(info) << this->GetName () << ": Done: Analyzing Lost_Ratio ";
  fNrTimeslice++;
  

}

