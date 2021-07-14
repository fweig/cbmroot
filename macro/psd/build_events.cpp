#include <TBrowser.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TObject.h>
#include <TString.h>
#include <TTree.h>

#include <fstream>
#include <iostream>

#include <math.h>
#include <string.h>

#include "event_data_struct.h"

using namespace std;

// root -l -b -q 'GBTreaderEventBuild.cpp("DspTree.root", "EventTree.root")'

int build_events(TString inFileName, TString outFileName, int dsps_to_read = -1)
{
  TString srcDir           = gSystem->Getenv("VMCWORKDIR");
  const int total_channels = 32;
  const int gate_length    = 60;  //in ns

  TFile* inFile = TFile::Open(inFileName.Data(), "READONLY");
  TTree* inTree = (TTree*) inFile->Get("cbmsim");
  if (inTree == nullptr) {
    cout << "tree not found" << endl;
    return -1;
  }

  std::vector<CbmPsdDsp>* fPsdDspVect = new std::vector<CbmPsdDsp>;
  inTree->SetBranchAddress("PsdDsp", &fPsdDspVect);
  if (fPsdDspVect == nullptr) {
    cout << "no psd dsps vector in tree" << endl;
    return -2;
  }
  Int_t Nhits         = 0;
  ULong64_t EventTime = 0;


  printf("\n\n====================================================\n");
  printf("================= TSA DATA READOUT =================\n");
  printf("====================================================\n\n\n");

  TFile* result_file              = new TFile(outFileName.Data(), "RECREATE");
  event_data_struct* event_struct = new event_data_struct[total_channels];
  TTree* event_tree               = new TTree("EventTree", "EventTree");
  event_tree->Branch("Nhits", &Nhits, "Nhits/I");
  event_tree->Branch("EventTime", &EventTime, "EventTime/l");
  for (Int_t ch = 0; ch < total_channels; ch++) {
    (event_struct + ch)->CreateBranch(event_tree, ch);
    (event_struct + ch)->reset();
  }

  bool first_all_sync   = false;
  double hit_time       = 0.;
  double ev_start_time  = 0.;
  double prev_hit_time  = 0.;
  double timestump      = 0.;
  double prev_timestump = 0.;
  std::vector<CbmPsdDsp> event_dsp_vect;

  /*
	{
		int dsps_for_time_btw = (int) 1e5; //1e5
		TH1F *h1_time_btw_hits = new TH1F("h1_time_btw", "h1_time_btw; time [ns]; Counts", 500, -50, 300);
		hit_time = 0.;

 		CbmPsdDsp dsp;
		for( int entr_iter = 0; entr_iter < dsps_for_time_btw; entr_iter++ )
		{
			inTree->GetEntry(entr_iter);
			Int_t nPsdDsps = fPsdDspVect->size();
			for (Int_t iDsp = 0; iDsp < nPsdDsps; iDsp++) {
				dsp = (*fPsdDspVect)[iDsp];

				hit_time = dsp.fdTime + dsp.fdTsTime;
				int hit_channel = dsp.GetSectionID();
				if(hit_channel>9) continue;
				h1_time_btw_hits->Fill(hit_time-prev_hit_time);
				prev_hit_time = hit_time;
			}

		}

		TCanvas *cc = new TCanvas();
		gPad->SetLogy();
		h1_time_btw_hits->Draw();

		hit_time = 0.;
		prev_hit_time = 0.;	
	}
*/

  time_t start_time = time(NULL);
  prev_hit_time     = 0.;
  CbmPsdDsp dsp;
  for (int entr_iter = 0; entr_iter < inTree->GetEntries(); entr_iter++) {
    inTree->GetEntry(entr_iter);
    Int_t nPsdDsps = fPsdDspVect->size();
    for (Int_t iDsp = 0; iDsp < nPsdDsps; iDsp++) {
      dsp = (*fPsdDspVect)[iDsp];
      if (dsp.GetSectionID() > 9) continue;
      hit_time = dsp.fdTime + dsp.fdTsTime;

      if (hit_time < ev_start_time) cout << "negative time " << hit_time << " " << ev_start_time << endl;
      if (hit_time < prev_hit_time) cout << "negative time" << endl;

      if ((hit_time - ev_start_time) < gate_length)
      //if( (hit_time-prev_hit_time) < gate_length )
      {
        event_dsp_vect.push_back(dsp);
        prev_hit_time = dsp.fdTime + dsp.fdTsTime;
      }
      else {
        Nhits     = event_dsp_vect.size();
        EventTime = (ULong64_t) ev_start_time;

        for (int ch_iter = 0; ch_iter < total_channels; ch_iter++)
          event_struct[ch_iter].reset();

        // Check channel duplicates in dsp vector
        std::map<uint32_t, int> countMap;
        for (auto& elem : event_dsp_vect) {
          auto result = countMap.insert(std::pair<uint32_t, int>(elem.GetSectionID(), 1));
          if (result.second == false) result.first->second++;
        }

        // Iterate over the map
        for (auto& elem : countMap) {
          // If frequency count is greater than 1 then its a duplicate element
          if (elem.second > 1) {
            std::cout << "Replacement in channel " << elem.first << " :: total times " << elem.second
                      << " :: Event vector size " << event_dsp_vect.size() << std::endl;

            int counter = 0;
            cout.precision(32);
            for (auto& PsdDsp : event_dsp_vect) {
              cout << "# " << counter << " dsp_time-event_begin_time " << PsdDsp.fdTime + dsp.fdTsTime - ev_start_time
                   << endl;
              cout << " prev_timestump " << prev_timestump << " timestamp " << timestump << endl;
              cout << PsdDsp.GetSectionID() << " " << PsdDsp.fdEdep << " " << PsdDsp.fdTime + dsp.fdTsTime << endl;
              counter++;
            }

            cout << "End of microslice" << endl;
          }
        }


        for (auto& PsdDsp : event_dsp_vect) {
          int hit_channel                 = PsdDsp.GetSectionID();
          event_data_struct& result_event = event_struct[hit_channel];

          result_event.EdepFPGA    = PsdDsp.fdEdep;
          result_event.EdepWfm     = PsdDsp.fdEdepWfm;
          result_event.Ampl        = PsdDsp.fdAmpl;
          result_event.Minimum     = PsdDsp.fuMinimum;
          result_event.ZL          = PsdDsp.fuZL;
          result_event.Time        = PsdDsp.fdTime + PsdDsp.fdTsTime;
          result_event.TimeMax     = PsdDsp.fuTimeMax;
          result_event.TimeInEvent = PsdDsp.fdTime - ev_start_time;

          result_event.FitEdep    = PsdDsp.fdFitEdep;
          result_event.FitAmpl    = PsdDsp.fdFitAmpl;
          result_event.FitZL      = PsdDsp.fdFitZL;
          result_event.FitR2      = PsdDsp.fdFitR2;
          result_event.FitTimeMax = PsdDsp.fdFitTimeMax;

          result_event.EventTime = (ULong64_t) ev_start_time;
        }

        if (Nhits > 0) event_tree->Fill();  //skipping first dsp
        event_dsp_vect.clear();
        //countMap.clear();

        event_dsp_vect.push_back(dsp);
        ev_start_time = hit_time;
        prev_hit_time = hit_time;
      }
    }

    if (dsps_to_read > 0 && event_tree->GetEntries() >= dsps_to_read) break;


    if ((entr_iter % 100) == 0) {
      time_t current_time = time(NULL);
      Int_t proc_sec      = difftime(current_time, start_time);
      Float_t percents    = (float) entr_iter / (inTree->GetEntries());
      Int_t time_est      = (percents == 0) ? 0 : proc_sec / percents * (1. - percents);
      Float_t proc_rate   = (float) proc_sec / entr_iter * 1000000. / 60.;

      printf("Processed entries: %i (%5.1f%%); [pas %3.0dm %2.0is] [est %3.0dm %2.0is] [%.1f min [%.1fh]/1M ev]\r",
             entr_iter, (percents * 100.), (proc_sec / 60), proc_sec % 60, (time_est / 60), time_est % 60, proc_rate,
             (proc_rate / 60.));
      cout << flush;
    }

    prev_timestump = timestump;
  }


  event_tree->Write();
  result_file->Close();

  return 0;
}
