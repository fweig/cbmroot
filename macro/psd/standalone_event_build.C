/** @file standalone_event_build.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Nikolay Karpushkin, Pierre-Alain Loizeau [committer] **/

/** @file MCBM PSD Energy profile
 ** @author Nikolay Karpushkin <karpushkin@inr.ru>
 ** @date 15.12.2019
 ** ROOT macro to visualize psd energy in psd digi
 */


void standalone_event_build(TString inFileName)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  TFile* inFile = TFile::Open(inFileName.Data(), "READONLY");
  TTree* inTree = (TTree*) inFile->Get("cbmsim");
  if (inTree == nullptr) {
    cout << "tree not found" << endl;
    return;
  }

  std::vector<CbmPsdDigi>* fPsdDigis = new std::vector<CbmPsdDigi>;
  inTree->SetBranchAddress("PsdDigi", &fPsdDigis);
  if (fPsdDigis == nullptr) {
    cout << "no psd digis vector in tree" << endl;
    return;
  }

  const Float_t event_time_gate = 200.;  //in ns
  const Int_t total_channels    = 9;
  const Int_t total_cells       = 9;

  Double_t ch_energy[total_channels];
  for (UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
    ch_energy[ch_iter] = 0.;

  Double_t ch_energy_overEv[total_channels];
  for (UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
    ch_energy_overEv[ch_iter] = 0.;
  Int_t Ev_counter               = 0;
  Int_t Fired_psd_channels_in_ev = 0;

  TH1* th1_hist_ptr = nullptr;
  TH2* th2_hist_ptr = nullptr;
  th1_hist_ptr      = new TH1F("Energy_profile", "PSD energy profile per event; cell # []; Energy [adc counts]",
                          total_channels, 0, total_channels);
  th1_hist_ptr      = new TH1F("Energy_distrib",
                          "PSD energy distribution; Energy over event all "
                          "channels [adc counts]; Counts []",
                          1000, 0, 10000);
  th1_hist_ptr      = new TH1F("Spill_structure", "PSD time spill structure; Time [s]; Counts []", 600, 20, 40);

  th1_hist_ptr = new TH1F("Digi_time_diff", "PSD digi time difference; Time [ns]; Counts []", (Int_t) 500, 0, 500);
  th1_hist_ptr =
    new TH1F("Fired_psd_chs", "Fired PSD channels in event; fired channels []; Counts []", total_cells, 0, total_cells);
  th1_hist_ptr = new TH1F("Cosmic_check", "Half a sum from two hodo channels; Energy []; Counts []", 100, 0, 100);

  th2_hist_ptr = new TH2F("PsdVsHodo",
                          "Energy in Psd vs energy in Hodo; Psd energy [adc "
                          "counts]; Hodo energy [adc counts]",
                          1000, 0, 2000, 140, 0, 70);

  for (UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
    th1_hist_ptr = new TH1F(Form("Energy_distrib_ch%i", ch_iter),
                            Form("Energy_distrib_ch%i; Energy [adc counts]; Counts []", ch_iter), 100, 0, 2000);

  for (UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
    th1_hist_ptr = new TH1F(Form("Energy_distrib_good_ev_ch%i", ch_iter),
                            Form("Energy_distrib_good_ev_ch%i; Energy [adc counts]; Counts []", ch_iter), 100, 0, 2000);

  Double_t event_time      = -1.;
  Double_t prev_event_time = -1.;
  Bool_t IsFirstEvent      = true;
  for (UInt_t ev = 0; ev < inTree->GetEntries(); ev++) {
    inTree->GetEntry(ev);

    Int_t nPsdDigis = fPsdDigis->size();
    if (ev % 10 == 0)
      std::cout << "Entry " << ev << " / " << inTree->GetEntries() << " Psd Digis: " << nPsdDigis << std::endl;
    if (nPsdDigis == 0) continue;

    CbmPsdDigi digi;
    Bool_t IsNewEvent = false;
    for (Int_t iDigi = 0; iDigi < nPsdDigis; iDigi++) {
      digi = (*fPsdDigis)[iDigi];
      //if(! (digi.GetSectionID() == 0 ) ) continue;
      //if(digi.GetSectionID() > 8) continue;

      if (digi.GetTime() - event_time > event_time_gate) {
        prev_event_time = event_time;
        event_time      = digi.GetTime();
        IsNewEvent      = true;
      }
      else
        IsNewEvent = false;

      if (IsNewEvent) {
        if (IsFirstEvent) {
          IsFirstEvent = false;
          continue;
        }  //skip info before first event
        Bool_t GoodEvent = true;

        if (GoodEvent) {
          th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny("Spill_structure")));
          th1_hist_ptr->Fill(event_time / 1e9);


          th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny("Digi_time_diff")));
          th1_hist_ptr->Fill(event_time - prev_event_time);

          Double_t psd_energy      = 0.;
          Fired_psd_channels_in_ev = 0;

          for (UInt_t ch_iter = 0; ch_iter < total_cells; ch_iter++) {
            ch_energy_overEv[ch_iter] += ch_energy[ch_iter];
            psd_energy += ch_energy[ch_iter];

            th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny(Form("Energy_distrib_good_ev_ch%i", ch_iter))));
            if (ch_energy[ch_iter] > 1.) th1_hist_ptr->Fill(ch_energy[ch_iter]);

            if (ch_energy[ch_iter] > 1.) Fired_psd_channels_in_ev++;
            ch_energy[ch_iter] = 0.;
          }

          //if(Fired_psd_channels_in_ev < 2) continue;

          th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny("Energy_distrib")));
          th1_hist_ptr->Fill(psd_energy);
          th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny("Fired_psd_chs")));
          th1_hist_ptr->Fill(Fired_psd_channels_in_ev);

          /*
		Double_t hodo_energy = 0.5*(ch_energy[17] + ch_energy[19]);
                ch_energy_overEv[17] += ch_energy[17];
                ch_energy_overEv[19] += ch_energy[19];
	        th1_hist_ptr = ((TH1*)(gDirectory->FindObjectAny( "Cosmic_check" )));
	        if(ch_energy[17] > 1. && ch_energy[19] > 1.) th1_hist_ptr->Fill(0.5*(ch_energy[17] + ch_energy[19]));

		ch_energy[17] = 0.;
		ch_energy[19] = 0.;

	        th2_hist_ptr = ((TH2*)(gDirectory->FindObjectAny( "PsdVsHodo" )));
	        th2_hist_ptr->Fill(psd_energy, hodo_energy);
*/
          Ev_counter++;

        }  //is good event
        else {
          for (UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
            ch_energy[ch_iter] = 0.;
        }

      }  //is new event

      Double_t Edep    = digi.GetEdep();
      UInt_t SectionID = digi.GetSectionID();

      //if(ch_energy[SectionID] > 1.) cout<<"PileUp candidate in channel "<<SectionID<<endl;
      ch_energy[SectionID] = Edep;

      th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny(Form("Energy_distrib_ch%i", SectionID))));
      th1_hist_ptr->Fill(Edep);

    }  //digi loop

  }  //event loop

  th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny("Energy_profile")));
  for (UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++)
    th1_hist_ptr->Fill(ch_iter, ch_energy_overEv[ch_iter] / Ev_counter);

  TCanvas* canvas_time_spill = new TCanvas("c_time_spill", "c_time_spill");
  th1_hist_ptr               = ((TH1*) (gDirectory->FindObjectAny("Spill_structure")));
  th1_hist_ptr->Draw();

  TCanvas* canvas_time_diff_ev = new TCanvas("c_time_diff_ev", "c_time_diff_ev");
  th1_hist_ptr                 = ((TH1*) (gDirectory->FindObjectAny("Digi_time_diff")));
  th1_hist_ptr->Draw();

  TCanvas* canvas_energy_pr = new TCanvas("c_energy_pr", "c_energy_pr");
  th1_hist_ptr              = ((TH1*) (gDirectory->FindObjectAny("Energy_profile")));
  th1_hist_ptr->Draw("hist");

  TCanvas* canvas_energy_dstr = new TCanvas("c_energy_dstr", "c_energy_dstr");
  th1_hist_ptr                = ((TH1*) (gDirectory->FindObjectAny("Energy_distrib")));
  th1_hist_ptr->Draw();

  TCanvas* canvas_fired_chs = new TCanvas("c_fired_ch", "c_fired_ch");
  th1_hist_ptr              = ((TH1*) (gDirectory->FindObjectAny("Fired_psd_chs")));
  th1_hist_ptr->Draw();

  TCanvas* canvas_psd_hodo = new TCanvas("c_psd_hodo", "c_psd_hodo");
  th2_hist_ptr             = ((TH2*) (gDirectory->FindObjectAny("PsdVsHodo")));
  th2_hist_ptr->Draw("colz");

  th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny("Cosmic_check")));
  th1_hist_ptr->Draw();

  TCanvas* canvas_energy_dstr_ch = new TCanvas("c_energy_dstr_ch", "c_energy_dstr_ch");
  canvas_energy_dstr_ch->DivideSquare(total_channels);
  for (UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++) {
    canvas_energy_dstr_ch->cd(ch_iter + 1);
    th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny(Form("Energy_distrib_ch%i", ch_iter))));
    th1_hist_ptr->Draw();
  }

  TCanvas* canvas_energy_dstr_gEv_ch = new TCanvas("c_energy_dstr_gEv_ch", "c_energy_dstr_gEv_ch");
  canvas_energy_dstr_gEv_ch->DivideSquare(total_channels);
  for (UInt_t ch_iter = 0; ch_iter < total_channels; ch_iter++) {
    canvas_energy_dstr_gEv_ch->cd(ch_iter + 1);
    th1_hist_ptr = ((TH1*) (gDirectory->FindObjectAny(Form("Energy_distrib_good_ev_ch%i", ch_iter))));
    th1_hist_ptr->Draw();
  }
}
