/** @file MCBM PSD Energy profile
 ** @author Nikolay Karpushkin <karpushkin@inr.ru>
 ** @date 15.12.2019
 ** ROOT macro to visualize psd energy in psd digi
 */


void standalone_event_build(TString inFileName) 
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

   TFile* inFile = TFile::Open(inFileName.Data(), "READONLY");
   TTree* inTree = (TTree*)inFile->Get("cbmsim");
   if(inTree == nullptr) { cout<<"tree not found"<<endl; return; }

   std::vector<CbmPsdDigi> *fPsdDigis = new std::vector<CbmPsdDigi>();
   inTree->SetBranchAddress("PsdDigi", &fPsdDigis);

   const Float_t event_time_gate = 100.; //in ns
   const Int_t total_channels = 16;
   const Int_t total_cells = 10;
   UInt_t ch_address[total_channels];
   for(UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
      ch_address[ch_iter] = 8 + ch_iter*1024;

   Double_t ch_energy[total_channels];
   for(UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
      ch_energy[ch_iter] = 0.;

   Double_t ch_energy_overEv[total_channels];
   for(UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
      ch_energy_overEv[ch_iter] = 0.;
   Int_t Ev_counter = 0;
   Int_t Fired_channels_in_ev = 0;

   TH1 *th1_hist_ptr = nullptr;
   TH2 *th2_hist_ptr = nullptr;
   th1_hist_ptr = new TH1F("Energy_profile", "PSD energy profile per event; cell # []; Energy [adc counts]", 16, 0, 16);
   th1_hist_ptr = new TH1F("Energy_distrib", "PSD energy distribution; Energy over event all channels [adc counts]; Counts []", 1000, 0, 100000);
   th1_hist_ptr = new TH1F("Spill_structure", "PSD time spill structure; Time [s]; Counts []", 5000, 0, 5000);
   th1_hist_ptr = new TH1F("Event_time_diff", "PSD event time difference; Time [ns]; Counts []", (Int_t)300, 0, 300);
   th1_hist_ptr = new TH1F("Fired_chs", "Fired channels in event; fired channels []; Counts []", 16, 0, 16);

   for(UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
      th1_hist_ptr = new TH1F( Form("Energy_distrib_ch%i", ch_iter), Form("Energy_distrib_ch%i; Energy [adc counts]; Counts []", ch_iter), 1000, 0, 10000);

   for(UInt_t ev = 0; ev < inTree->GetEntries(); ev++)
   //for(UInt_t ev = 0; ev < 901; ev++)
   {
      inTree->GetEntry(ev);

      Int_t nPsdDigis = fPsdDigis->size();
      if (ev % 1000 == 0 ) std::cout  << "Entry " << ev << " / " << inTree->GetEntries() << " Psd Digis: " << nPsdDigis  << std::endl;

      //if(nPsdDigis > 100) inTree->Show();

      CbmPsdDigi digi;
      Double_t event_time = 0.;
      Double_t prev_event_time = 0.;
      Bool_t IsNewEvent = false;
      for (Int_t iDigi = 0; iDigi < nPsdDigis; iDigi++)
      {
         digi = (*fPsdDigis)[iDigi];

         //if(! (digi.GetSectionID() == 0 && digi.GetTime()>4e12) ) continue;
         if(! ( digi.GetTime()>4e12) ) continue;

	 if( digi.GetTime()-event_time > event_time_gate )
	 {
	     prev_event_time = event_time;
	     event_time = digi.GetTime();
	     if( prev_event_time > 1. ) IsNewEvent = true;
	 }
	 else IsNewEvent = false;

	 if(IsNewEvent)
	 {
	    Bool_t GoodEvent = true;

	    if(GoodEvent)
	    {
	        th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Spill_structure" )));
	        th1_hist_ptr->Fill(event_time/1e9);

	        th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Event_time_diff" )));
	        th1_hist_ptr->Fill(event_time - prev_event_time);

	        Double_t total_energy = 0.;
		Fired_channels_in_ev = 0;

	        for(UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
	        {
                   ch_energy_overEv[ch_iter] += ch_energy[ch_iter];
	 	   total_energy += ch_energy[ch_iter];

		   if(ch_energy[ch_iter] > 1.) Fired_channels_in_ev++;
	           ch_energy[ch_iter] = 0.;
	        }

	        th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Energy_distrib" )));
	        th1_hist_ptr->Fill(total_energy);
	        th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Fired_chs" )));
	        th1_hist_ptr->Fill(Fired_channels_in_ev);

		//cout<<worked_cells_counter<<endl;

	        Ev_counter++;

            }//is good event
	    else
	    {
	       for(UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
	           ch_energy[ch_iter] = 0.;
	    }

	 }//is new event

         Double_t Edep = digi.GetEdep();
         UInt_t SectionID = digi.GetSectionID();

	 ch_energy[SectionID] = Edep;

	 th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( Form("Energy_distrib_ch%i", SectionID) )));
	 th1_hist_ptr->Fill(Edep);


      }//digi loop

   }//event loop

   th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Energy_profile" )));
   for(UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
      th1_hist_ptr->Fill(ch_iter, ch_energy_overEv[ch_iter]/Ev_counter);

   TCanvas *canvas_time_spill = new TCanvas("c_time_spill", "c_time_spill");
   th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Spill_structure" )));
   th1_hist_ptr->Draw();

   TCanvas *canvas_time_diff_ev = new TCanvas("c_time_diff_ev", "c_time_diff_ev");
   th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Event_time_diff" )));
   th1_hist_ptr->Draw();

   TCanvas *canvas_energy_pr = new TCanvas("c_energy_pr", "c_energy_pr");
   th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Energy_profile" )));
   th1_hist_ptr->Draw("hist");

   TCanvas *canvas_energy_dstr = new TCanvas("c_energy_dstr", "c_energy_dstr");
   th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Energy_distrib" )));
   th1_hist_ptr->Draw();

   TCanvas *canvas_fired_chs = new TCanvas("c_fired_ch", "c_fired_ch");
   th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Fired_chs" )));
   th1_hist_ptr->Draw();

   TCanvas *canvas_energy_dstr_ch = new TCanvas("c_energy_dstr_ch", "c_energy_dstr_ch");
   canvas_energy_dstr_ch->DivideSquare(total_channels);
   for(UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
   {
       canvas_energy_dstr_ch->cd(ch_iter+1);
       th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( Form("Energy_distrib_ch%i", ch_iter) )));
       th1_hist_ptr->Draw();
   }

}
