#include <string.h>
#include <math.h>

#include <TROOT.h>
#include <iostream>
#include <fstream>
#include<TTree.h>
#include<TString.h>
#include<TMath.h>
#include<TObject.h>
#include<TFile.h>
#include<TH1F.h>
#include<TH2F.h>
#include<TCanvas.h>
#include<TBrowser.h>
#include<TObjArray.h>
#include<TGraph.h>
#include <TLatex.h>
#include <TChain.h>

#include <TGraphErrors.h>
#include <TF1.h>
#include <TStyle.h>

#include "event_data_struct.h"


using namespace std;

// root -l -b -q 'GBTtreeShowSimple_fin.cpp("/mnt/c/Users/dmitr/cernbox/SOURCE/CALO/INR_ADC/16_cosmic_TRG09_44V_LC/Readout.root", "/mnt/c/Users/dmitr/cernbox/SOURCE/CALO/INR_ADC/16_cosmic_TRG09_44V_LC/Readout_ss", 1000)'





  Double_t SpectraCenter(TH1* hist, float nrms = 6.){
    Double_t hist_start_Xmin = hist->GetXaxis()->GetXmin();
    Double_t hist_start_Xmax = hist->GetXaxis()->GetXmax();
    hist->SetAxisRange(hist_start_Xmin, hist_start_Xmax);
	  
    Double_t histMean = hist->GetMean();
    Double_t histRMS = hist->GetRMS();
	Double_t histBin = hist->GetBinWidth(1);
	if (histRMS < histBin) histRMS = histBin;
	
    hist->SetAxisRange(histMean-histRMS*nrms, histMean+histRMS*nrms);
	printf("%s m %f r %f\n", hist->GetName(), histMean, histRMS);
	return histMean;
  }






int compare_to_sim(TString file_name, TString result_file_name, TString calc_mode = "EdepFPGA", int events_to_read = -1, int nhits = 0, int cut_option = 0) {

	const int total_channels = 10;	
	std::vector<int> hist_pars = {200, 0, 100};
	result_file_name = result_file_name+Form("_by_%s", calc_mode.Data());
	switch(cut_option){
	case 0:
		result_file_name = result_file_name+"_noCut";
		break;
	case 1:
		result_file_name = result_file_name+"_tCut";
		break;
	case 2:
		result_file_name = result_file_name+"_tR2Cut";
		break;
	}

	TFile* file_sim = TFile::Open("/home/nikolay/git_projects/mPSD_test_bench/data/ONi2AGeV_sim/O2AGeV_Ni_Hists_10kk.root");
	gDirectory->cd("Rint:/");

	TObjArray *canvas_array = new TObjArray;
	canvas_array->SetName ("Canvas");

	TObjArray *result_array = new TObjArray;
	result_array->SetName("calo_results");
	result_array->SetOwner();

	TChain *data_tree = new TChain;
	data_tree->AddFile( (file_name + "/EventTree").Data() );
	if(events_to_read==-1) events_to_read = data_tree->GetEntries();

	event_data_struct *event_struct = new event_data_struct[total_channels];
	for(Int_t ch = 0; ch < total_channels; ch++)
	    (event_struct+ch)->SetBranch(data_tree, ch);

	TH1 *th1_hist_ptr = nullptr;
	TH2 *th2_hist_ptr = nullptr;

	th1_hist_ptr  = new TH1F( "EdepProfile", Form("EdepProfile"), total_channels,0,total_channels);
	th1_hist_ptr->SetTitle( Form("Energy profile; Chan; Energy [MeV]") );
	result_array->Add(th1_hist_ptr);

	TH1F* temp = new TH1F( "temp", Form("temp"), total_channels,0,total_channels);
	data_tree->Draw( Form("channel_%i.%s>>temp", 0, calc_mode.Data()), Form("channel_%i.%s > 1", 0, calc_mode.Data()), "", events_to_read );

	for(int ch_iter = 0; ch_iter < total_channels; ch_iter++)
	{
		th1_hist_ptr  = new TH1F( Form("Edep_ch%i", ch_iter), Form("Edep_ch%i", ch_iter), hist_pars[0],hist_pars[1],hist_pars[2] );
		th1_hist_ptr->SetTitle( Form("Charge channel %i; Edep [MeV]", ch_iter) );

		th2_hist_ptr  = new TH2F( Form("timeMax_vs_Edep_ch%i", ch_iter), Form("timeMax_vs_Edep_ch%i", ch_iter), 32,0,32, hist_pars[0],hist_pars[1],hist_pars[2] );
		th2_hist_ptr->SetTitle( Form("TimeMax vs Edep channel %i; TimeMax [12,5ns]; Charge [mV]", ch_iter) );

		th2_hist_ptr  = new TH2F( Form("FitR2_vs_Edep_ch%i", ch_iter), Form("FitR2_vs_Edep_ch%i", ch_iter), hist_pars[0],hist_pars[1],hist_pars[2], 200,0,2 );
		th2_hist_ptr->SetTitle( Form("FitR2 vs EdepFPGA channel %i; Charge [mV]; R2 []", ch_iter) );
	}

	TString common_selection =  Form("(Nhits > %i)", nhits);
	//draw selection
	for(int ch_iter = 0; ch_iter < total_channels; ch_iter++)
	{
			TString ich_selection = "";
			//ich_selection += Form("&&(channel_%i.Edep>1.5) ", ch_iter);//dont touch (not filled channels removal)
			switch(cut_option){
			case 0:
				break;
			case 1:
				ich_selection += Form("&&(channel_%i.TimeMax>=4 && channel_%i.TimeMax<=7)", ch_iter, ch_iter);
				break;
			case 2:
				ich_selection += Form("&&(channel_%i.TimeMax>=4 && channel_%i.TimeMax<=7)", ch_iter, ch_iter);
				ich_selection += Form("&&(channel_%i.FitR2<0.4)", ch_iter);
				break;
			}

			TString hist_name = Form("Edep_ch%i", ch_iter);
			data_tree->Draw( Form("channel_%i.%s>>%s", ch_iter, calc_mode.Data(), hist_name.Data()), (common_selection+ich_selection).Data(), "", events_to_read );

			hist_name = Form("timeMax_vs_Edep_ch%i", ch_iter);
			data_tree->Draw( Form("channel_%i.%s:channel_%i.TimeMax>>%s", ch_iter, calc_mode.Data(), ch_iter, hist_name.Data()), 
													(common_selection+ich_selection).Data(), "", events_to_read );

			hist_name = Form("FitR2_vs_Edep_ch%i", ch_iter);
			data_tree->Draw( Form("channel_%i.FitR2:channel_%i.EdepFPGA>>%s", ch_iter, ch_iter, /*calc_mode.Data(),*/ hist_name.Data()), 
													(common_selection+ich_selection).Data(), "", events_to_read );
			
			printf("selection for ch %i: [%s]\n", ch_iter, (common_selection+ich_selection).Data());
	}


	//drawing histogramms
	for(int ch_iter = 0; ch_iter < total_channels; ch_iter++)
	{
		double mip = 5.0; //MeV
		TH1* th1_ch_ptr = ((TH1*)(gDirectory->FindObjectAny( Form("Edep_ch%i", ch_iter) )));

		th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "EdepProfile" )));
		//th1_hist_ptr->SetBinContent(ch_iter+1, th1_ch_ptr->GetMean());//*th1_ch_ptr->GetEntries()/data_tree->GetEntries());
		th1_hist_ptr->SetBinContent(ch_iter+1, th1_ch_ptr->GetMean()*th1_ch_ptr->GetEntries()/temp->GetEntries());

	}
	


	TCanvas *canv_charge_Edep = new TCanvas("canv_charge_Edep", "canv_charge_Edep");
	canvas_array->Add(canv_charge_Edep);
	canv_charge_Edep->DivideSquare(total_channels);
	for (int ch_iter = 0; ch_iter < total_channels; ch_iter++){
		canv_charge_Edep->cd(ch_iter+1)->SetLogy(1);
		th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( Form("Edep_ch%i", ch_iter) )));
		th1_hist_ptr->Draw();
		result_array->Add(th1_hist_ptr);
		int start = 8;
		int stop = 20;
		Int_t binx_min = th1_hist_ptr->GetXaxis()->FindBin(start);
		Int_t binx_max = th1_hist_ptr->GetXaxis()->FindBin(stop);
		double data_integral = th1_hist_ptr->Integral(binx_min, binx_max);

		th1_hist_ptr = ((TH1F*)(file_sim->FindObjectAny( Form("hf4_%i", ch_iter+1) )));
		if(th1_hist_ptr == nullptr) {cout<<"no hist"<<endl; break;}
		th1_hist_ptr->SetLineColor(kMagenta);
		th1_hist_ptr->SetLineWidth(2);
		binx_min = th1_hist_ptr->GetXaxis()->FindBin(start);
		binx_max = th1_hist_ptr->GetXaxis()->FindBin(stop);
		th1_hist_ptr->Scale(data_integral/th1_hist_ptr->Integral(binx_min, binx_max));
		th1_hist_ptr->Draw("same hist");

	}
	

	TCanvas *canv_time_Edep = new TCanvas("canv_time_Edep", "canv_time_Edep");
	canvas_array->Add(canv_time_Edep);
	canv_time_Edep->DivideSquare(total_channels);
	for (int ch_iter = 0; ch_iter < total_channels; ch_iter++){
		canv_time_Edep->cd(ch_iter+1)->SetLogz(1);
		th2_hist_ptr = ((TH2*)(gDirectory->FindObjectAny( Form("timeMax_vs_Edep_ch%i", ch_iter) )));
		th2_hist_ptr->Draw("colz");
		result_array->Add(th2_hist_ptr);
	}

	TCanvas *canv_R2_Edep = new TCanvas("canv_R2_Edep", "canv_R2_Edep");
	canvas_array->Add(canv_R2_Edep);
	canv_R2_Edep->DivideSquare(total_channels);
	for (int ch_iter = 0; ch_iter < total_channels; ch_iter++){
		canv_R2_Edep->cd(ch_iter+1)->SetLogz(1);
		th2_hist_ptr = ((TH2*)(gDirectory->FindObjectAny( Form("FitR2_vs_Edep_ch%i", ch_iter) )));
		th2_hist_ptr->Draw("colz");
		result_array->Add(th2_hist_ptr);
	}

	TCanvas *canv_Profile = new TCanvas("canv_Profile", "canv_Profile");
	canvas_array->Add(canv_Profile);
	th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "EdepProfile" )));
	th1_hist_ptr->GetYaxis()->SetRangeUser(0, 1.1*th1_hist_ptr->GetBinContent(1));
	th1_hist_ptr->Draw("hist");

	TFile* file_simm = TFile::Open("/home/nikolay/git_projects/mPSD_test_bench/data/ONi2AGeV_sim/profile_4.00MeV_trshd_10kk.root");
	TCanvas* canv_sim = (TCanvas*) file_simm->Get("c1");
	th1_hist_ptr = (TH1F*) canv_sim->GetPrimitive("hEnergyProfile");
	th1_hist_ptr->SetLineColor(kMagenta);
	th1_hist_ptr->SetLineWidth(2);
	canv_Profile->cd();
	th1_hist_ptr->Draw("same hist");	

	// saving results #####################################################################

	for (Int_t i = 0; i < canvas_array->GetLast (); i++) 
	    ((TCanvas*) canvas_array->At(i))->SaveAs ((result_file_name + ".pdf(").Data ());
	((TCanvas*) canvas_array->At (canvas_array->GetLast ()))->SaveAs ((result_file_name + ".pdf)").Data ());
  	printf("file %s was written\n", (result_file_name + ".pdf").Data());


	result_array->Add(canvas_array);
	TFile *result_file = new TFile((result_file_name + ".root").Data(), "RECREATE");
	TString start_path = result_array->GetName();
	result_file->mkdir(start_path.Data());
	result_file->cd(start_path.Data());
	TIter nx_iter((TCollection*)(result_array));
	TObject* obj_ptr;
	while ( (obj_ptr=(TObjArray*)nx_iter()) )
	{
	    if(obj_ptr == nullptr) continue; 
	    printf("Writing %s object\n", obj_ptr->GetName());
	    obj_ptr->Write();
	}
  	printf("file %s was written\n", (result_file_name + ".root").Data());
	result_file->Close();
	delete result_file;


	return 0;

}

#ifdef __CINT__
 
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;
 
#pragma link C++ struct data_header_struct+;
#pragma link C++ struct digi_struct+;
#endif

