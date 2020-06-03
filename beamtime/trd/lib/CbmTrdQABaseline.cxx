#include "CbmTrdQABaseline.h"
#include "CbmBeamDefaults.h"
#include "FairLogger.h"
#include "CbmHistManager.h"
//#include "TString.h"
//#include "TStyle.h"


#include <cmath>
#include <map>
#include <vector>

ClassImp(CbmTrdQABaseline);
//const  Int_t NrRobs=4;
//const  Int_t NrSpadics=3;

CbmTrdQABaseline::CbmTrdQABaseline(TString ClassName) :
  CbmTrdQABase("CbmTrdQABaseline")
{
}

void CbmTrdQABaseline::CreateHistograms(){
  for (Int_t layer=0;layer<1;layer++){
    TString HistName="Baselinemap_Layer_"+std::to_string(layer);
    fHm->Add(HistName.Data(), new TProfile2D(HistName.Data(),HistName.Data(),32,-0.5,31.5,1,-0.5,0.5));
    fHm->P2(HistName.Data())->GetXaxis()->SetTitle("Column");
    fHm->P2(HistName.Data())->GetYaxis()->SetTitle("Row");
  }
  fHm->Add("multihits", new TH1D("multihits","multihits",3,-0.5,2.5));
  fHm->Add("delta", new TH1D("delta","delta",201,-100.5,100.5));


}

void CbmTrdQABaseline::Exec(Option_t*)
{
  static Int_t NrTimeSlice=-1;
  LOG(info)<<this->GetName()<<": Loop for Baselinemaps starting";
  std::vector<Double_t> LastHit(NrOfActiveSyscores*NrOfActiveSpadics*32);
    LOG(info) << this->GetName() <<": Number of current TimeSlice:"
	       << ++NrTimeSlice;
  LOG(info) << this->GetName() <<": rawMessages in TS:          "
	       << fRaw->GetEntriesFast ();

  Int_t nSpadicMessages = fRaw->GetEntriesFast (); //SPADIC messages per TimeSlice
  std::vector<TProfile2D*> Hitmaps;
  for (Int_t layer=0;layer<1;layer++){
    TString HistName="Baselinemap_Layer_"+std::to_string(layer);
    Hitmaps.push_back(fHm->P2(HistName.Data()));
  }
  for (Int_t iSpadicMessage=0; iSpadicMessage < nSpadicMessages; ++iSpadicMessage){
    CbmSpadicRawMessage *raw= static_cast<CbmSpadicRawMessage*>(fRaw->At(iSpadicMessage));
    //LOG(info) << "Predecessor"  << raw->GetPredecessor() << "Follower"<< raw->GetFollower() << "delta"<< raw->GetDelta();


    if (!(raw->GetHit()))
    {
      continue;
    }

    //LOG(info) << raw->GetTriggerType();
/*

    if (raw->GetPredecessor()==1||raw->GetTriggerType()==1)// Triggertype =1
    {fHm->H1("multihits")->Fill(1.);
    //fHm->H1("delta")->Fill(raw->GetDelta());
    }
    else
    {
    	if (raw->GetFollower()==1||raw->GetTriggerType()==1)
    	{fHm->H1("multihits")->Fill(2.);
  */
    if (raw->GetDelta()>-99)
    	fHm->H1("delta")->Fill(raw->GetDelta());
   /* 	}
    	else
    		if(raw->GetTriggerType()==1)
    		{
    	{fHm->H1("multihits")->Fill(0.);}
    		}
    }

    Hitmaps.at(GetLayerID(raw))->Fill(GetColumnID(raw),GetRowID(raw),raw->GetSamples()[0]);
  */
  }
  LOG(info)<<this->GetName()<<": Loop for Baselinemaps done";

}

