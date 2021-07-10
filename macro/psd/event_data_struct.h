#ifndef EVENT_DATA_STRUCT
#define EVENT_DATA_STRUCT

#include<TTree.h>
#include<TString.h>
#include<TMath.h>
#include<TObject.h>
#include<fstream>


//data processed from waveform
struct event_data_struct
{

    Float_t EdepFPGA;
    Float_t EdepWfm;
    Float_t Ampl;
    Float_t Minimum;
    Float_t ZL;
    Float_t Time;
    Int_t TimeMax;
    Float_t TimeHalf;
    Float_t TimeInEvent;

    Float_t FitEdep;
    Float_t FitAmpl;
    Float_t FitZL;
    Float_t FitR2;
    Float_t FitTimeMax;

    ULong64_t EventTime;


    void reset()
    {
	EdepFPGA = 0.;
	EdepWfm = 0.;
	Ampl = 0.;
	Minimum = 0.;
	ZL = 0.;
	Time = 0.;
	TimeMax = -1;
	TimeHalf = -1.;
	TimeInEvent = -1.;

	FitEdep = 0.;
	FitAmpl = 0.;
	FitZL = 0.;
	FitR2 = 999.;
	FitTimeMax = -1.;

	EventTime = 0;
    }

    void Print()
    {
	printf("[EdepFPGA %.0f; EdepWfm %.0f; Ampl %.0f; Minumum %.0f; ZL %.2f; Time %.2f; TimeMax %i; TimeHalf %.2f; TimeInEvent %.2f; FitEdep %.2f; FitAmpl %.2f; FitZL %.2f; FitR2 %.2f; FitTimeMax %.2f; EventTime %llu]",
	       EdepFPGA, EdepWfm, Ampl, Minimum, ZL, Time, TimeMax, TimeHalf, TimeInEvent, FitEdep, FitAmpl, FitZL, FitR2, FitTimeMax, EventTime);
    }

    static TString GetChName(Int_t channel_num)
    {
	return TString::Format("channel_%i", channel_num);
    }

    TBranch* CreateBranch(TTree *tree, Int_t channel_num)
    {
	return tree->Branch(GetChName(channel_num).Data(), this,
	    "EdepFPGA/F:EdepWfm/F:Ampl/F:Minimum/F:ZL/F:Time/F:TimeMax/I:TimeHalf/F:TimeInEvent/F:FitEdep/F:FitAmpl/F:FitZL/F:FitR2/F:FitTimeMax/F:EventTime/l");
    }


    Int_t SetBranch(TTree *tree, Int_t channel_num)
    {
	return tree->SetBranchAddress(GetChName(channel_num).Data(), this);
    }

};





#endif // EVENT_DATA_STRUCT

