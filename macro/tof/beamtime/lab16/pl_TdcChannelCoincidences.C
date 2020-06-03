

void pl_TdcChannelCoincidences(const TString& sTreeFile = "unpack_CbmTofQa_28May1635_mrpc1_000_mrpc2_200_mrpc3_200_nb.out.root", bool usePlaMultEvents = 1)
{
   gStyle->SetOptStat("emruo");
   gStyle->SetOptFit(10001);

   TStopwatch timer;
   timer.Start();


// Load Tree and Branch



   TFile* fAnalysis = new TFile( sTreeFile,"READ");


   // Recover a pointer on the TTree object and check how many events it contains

   TTree* RawTree = (TTree*)fAnalysis->Get("cbmsim");
   Int_t nentries = RawTree->GetEntries();
   cout<<nentries<<" events in the Raw Tree"<<endl;

//Creating Outputfile
/*
   TString sOutput = (TString)sTreeFile(0, sTreeFile.	Length() -5 );
   sOutput+="_corr.root";
   TFile* fOutput   = new TFile( sOutput,"RECREATE");
   TTree* tTree = new TTree ();
   tTree->SetName("GsiAug12");
   gDirectory->pwd();
   tTree->SetDirectory(gDirectory);
*/
   TBranch* DigiBranch = RawTree->GetBranch("TofCalibData");
	TClonesArray* DigiArray = new TClonesArray("TTofCalibData");
	DigiBranch->SetAddress(&DigiArray);
  	TTofCalibData* DigiAdr = new TTofCalibData(); 

 //Create Histograms
gROOT->cd(); 
   TH1D* h1 = new TH1D ("h1","Hit distribution of TDC 02",32,-0.5,31.5);
   TH1D* h2 = new TH1D ("h2","Hit distribution of TDC 00",32,-0.5,31.5);
 	TH1D* h3 = new TH1D ("h3","Coincidence Pattern",7,-0.5,6.5);
 	TH1D* h4 = new TH1D ("h4","triggered RPC-Coincidences on strips",16,-0.5,15.5);
 	TH1D* h5 = new TH1D ("h5","RPC-Coincidences on strips",16,-0.5,15.5);
 	TH1D* h6 = new TH1D ("h6","RPC+2Pla-Coincidences on strips",16,-0.5,15.5);
 	TH2D* h7 = new TH2D ("h7","PMT-time difference PLA0",1000,-100000,100000,20000,0,20000000);
 	TH2D* h8 = new TH2D ("h8","PMT-time difference PLA1",1000,-100000,100000,20000,0,20000000);
 	TH2D* h9 = new TH2D ("h9","PMT-time difference PLA0-PLA1",1000,-100000,100000,2000,0,20000000);
 	TH1D* h10 = new TH1D ("h10","Full RPC-time difference RPC-PLA0 (first hit in channel)",20000,-1000000,1000000);
	TH1D* h11 = new TH1D ("h11","Full RPC-time difference RPC-PLA0 und PlaTrig (first hit in channel)",20000,-1000000,1000000);
	TH2D* h12 = new TH2D ("h9","Rpc-Y-Position vs Tot",1000,-100000,100000,2000,0,5000000);


  TH1D* tChannelResHistogram[16];
  for(Int_t iHistogram = 0; iHistogram < 16; iHistogram++)
  {
    delete gROOT->FindObjectAny(Form("tChannelResHistogram%u",iHistogram));

    tChannelResHistogram[iHistogram] = new TH1D(Form("tChannelResHistogram%u",iHistogram),Form("channel %u",iHistogram),4000,-20**12,20**12);
  }

  TH1D* tChannelResHistogramTrig[16];
  for(Int_t iHistogram = 0; iHistogram < 16; iHistogram++)
  {
    delete gROOT->FindObjectAny(Form("tChannelResHistogramTrig%u",iHistogram));

    tChannelResHistogramTrig[iHistogram] = new TH1D(Form("tChannelResHistogramTrig%u",iHistogram),Form("channel %u",iHistogram),4000,-20**12,20**12);
  }

  TH2D* tChannelYPosHistogram[16];
  for(Int_t iHistogram = 0; iHistogram < 16; iHistogram++)
  {
    delete gROOT->FindObjectAny(Form("tChannelYPosHistogram%u",iHistogram));

    tChannelYPosHistogram[iHistogram] = new TH2D(Form("tChannelYPosHistogram%u",iHistogram),Form("channel %u",iHistogram),1000,-100000,100000,2000,0,500000);
  }






cout<<"Filling Histograms"<<endl;
fAnalysis->cd();
 

//Eventschleife----------------------------------------------------------------------------------
for(Int_t jEvent = 0; jEvent < nentries; jEvent++)
{
   DigiBranch->GetEntry(jEvent);
	Int_t n=0;
         
	if(!jEvent%10000)
	cout<<jEvent<<" Events processed"<<endl;
			
   //Initializierung
	Int_t nentriesArray = DigiArray->GetEntriesFast();
	//cout<<nentriesArray<<"-Array-Entries in Event "<<jArray<<endl;
	UInt_t bRpcLeft[16] = false;
	UInt_t bRpcRight[16] = false;
	Double_t dRpcTimeLeft[16] = 0;
	Double_t dRpcTimeRight[16] = 0; 
	Double_t dRpcTotLeft[16] = 0;
	Double_t dRpcTotRight[16] = 0; 

  for (Int_t i = 0; i < 16; i++)
	{
		bRpcRight[i] = false;
		bRpcLeft [i] = false;
		dRpcTimeLeft[i] = 0;
		dRpcTimeRight[i] = 0;
		dRpcTotLeft[i] = 0;
		dRpcTotRight[i] = 0; 
	}


	UInt_t bRpcHit = 0;
	UInt_t bPla0Left = 0;
	UInt_t bPla0Right = 0;
	UInt_t bPla1Left = 0;
	UInt_t bPla1Right = 0;
	UInt_t bPlaTrig = 0;
	Double_t dPla0TimeLeft = 0;
	Double_t dPla0TimeRight = 0;
	Double_t dPla1TimeLeft = 0;
	Double_t dPla1TimeRight = 0; 
	Double_t dPla0TotLeft = 0;
	Double_t dPla0TotRight = 0;
	Double_t dPla1TotLeft = 0;
	Double_t dPla1TotRight = 0; 
 



//-----------------------------------------------------------------------------
   for (Int_t jArray = 0; jArray < nentriesArray; jArray++)
      {
		DigiAdr = (TTofCalibData*)DigiArray->At(jArray);


			if (DigiAdr->GetBoard())
				h1->Fill(DigiAdr->GetChannel());
			else
				h2->Fill(DigiAdr->GetChannel());


			if(DigiAdr->GetChannel()<16)
			{
			if (DigiAdr->GetBoard())
			{
				bRpcLeft[DigiAdr->GetChannel()]++;
				if(dRpcTimeLeft[DigiAdr->GetChannel()]==0 && dRpcTotLeft[DigiAdr->GetChannel()]==0)
				{
					dRpcTimeLeft[DigiAdr->GetChannel()] = DigiAdr->GetTime();
//cout<<"RpcLeft: "<< DigiAdr->GetTime()<<endl;
					dRpcTotLeft[DigiAdr->GetChannel()] = DigiAdr->GetTot();
				}
			}
			else
			{
				bRpcRight[DigiAdr->GetChannel()]++;
				if(dRpcTimeRight[DigiAdr->GetChannel()]==0 && dRpcTotRight[DigiAdr->GetChannel()]==0)
				{
					dRpcTimeRight[DigiAdr->GetChannel()] = DigiAdr->GetTime();
//cout<<"RpcRight: "<< DigiAdr->GetTime()<<endl;
					dRpcTotRight[DigiAdr->GetChannel()] = DigiAdr->GetTot();
				}
			}
			}

			else
			{
			if (17 == DigiAdr->GetChannel())
				{
				bPla0Left++;
				if(dPla0TimeLeft==0){dPla0TimeLeft=DigiAdr->GetTime();}
				if(dPla0TotLeft==0){dPla0TotLeft=DigiAdr->GetTot();}
				}
			if (18 == DigiAdr->GetChannel())
				{
				bPla0Right++;
				if(dPla0TimeRight==0){dPla0TimeRight=DigiAdr->GetTime();}
				if(dPla0TotRight==0){dPla0TotRight=DigiAdr->GetTot();}
				}
			if (19 == DigiAdr->GetChannel())
				{
				bPla1Left++;
				if(dPla1TimeLeft==0){dPla1TimeLeft=DigiAdr->GetTime();}
				if(dPla1TotLeft==0){dPla1TotLeft=DigiAdr->GetTot();}
				}
			if (20 == DigiAdr->GetChannel())
				{
				bPla1Right++;
				if(dPla1TimeRight==0){dPla1TimeRight=DigiAdr->GetTime();}
				if(dPla1TotRight==0){dPla1TotRight=DigiAdr->GetTot();}
				}
			if (21 == DigiAdr->GetChannel())
				bPlaTrig++;			
			}
      }
//------------------------------------------------------------------------

  for (Int_t i = 0; i < 16; i++)
	{
		if (bRpcLeft[i] && bRpcRight[i])
		{
			bRpcHit= true;
			if(bPlaTrig)
				h4->Fill(i);
	if(usePlaMultEvents==1)
	{
//			if(bPla1Right==1 && bPla1Left==1 && bPla0Right==1 && bPla0Left==1)
			if(bPla0Right && bPla0Left)//If PLA1 is not in use!
				{
				h6->Fill(i);
				h10->Fill( ((dRpcTimeLeft[i]+dRpcTimeRight[i])/2) - ((dPla0TimeLeft+dPla0TimeRight)/2) );
				tChannelResHistogram[i]->Fill( ((dRpcTimeLeft[i]+dRpcTimeRight[i])/2) - ((dPla0TimeLeft+dPla0TimeRight)/2) );
				}
			if(bPla1Right==1 && bPla1Left==1 && bPla0Right==1 && bPla0Left==1 && bPlaTrig==1)
				{
				h11->Fill( ((dRpcTimeLeft[i]+dRpcTimeRight[i])/2) - ((dPla0TimeLeft+dPla0TimeRight)/2) );
				tChannelResHistogramTrig[i]->Fill( ((dRpcTimeLeft[i]+dRpcTimeRight[i])/2) - ((dPla0TimeLeft+dPla0TimeRight)/2) );
				}
	}
	else
	{
//			if(bPla1Right && bPla1Left && bPla0Right && bPla0Left)
			if(bPla0Right && bPla0Left)//If PLA1 is not in use!
				{
				h6->Fill(i);
				h10->Fill( ((dRpcTimeLeft[i]+dRpcTimeRight[i])/2) - ((dPla0TimeLeft+dPla0TimeRight)/2) );
				tChannelResHistogram[i]->Fill( ((dRpcTimeLeft[i]+dRpcTimeRight[i])/2) - ((dPla0TimeLeft+dPla0TimeRight)/2) );
				}
			if(bPla1Right && bPla1Left && bPla0Right && bPla0Left && bPlaTrig)
				{
				h11->Fill( ((dRpcTimeLeft[i]+dRpcTimeRight[i])/2) - ((dPla0TimeLeft+dPla0TimeRight)/2) );
				tChannelResHistogramTrig[i]->Fill( ((dRpcTimeLeft[i]+dRpcTimeRight[i])/2) - ((dPla0TimeLeft+dPla0TimeRight)/2) );
				}

	}
			h5->Fill(i);
//			h12->Fill( (dRpcTimeLeft[i]-dRpcTimeRight[i]),dRpcTotLeft[i] );
//		tChannelYPosHistogram[i]->Fill( (dRpcTimeLeft[i]-dRpcTimeRight[i]),(dRpcTotLeft[i]) );
//			h12->Fill( (dRpcTimeLeft[i]-dRpcTimeRight[i]),dRpcTotLeft[i] );
//		tChannelYPosHistogram[i]->Fill( (dRpcTimeLeft[i]-dRpcTimeRight[i]),(dRpcTotRight[i]) );
			h12->Fill( (dRpcTimeLeft[i]-dRpcTimeRight[i]),(dRpcTotLeft[i]+dRpcTotRight[i]) );
		tChannelYPosHistogram[i]->Fill( (dRpcTimeLeft[i]-dRpcTimeRight[i]),(dRpcTotLeft[i]+dRpcTotRight[i]) );

		}
	}

	if(usePlaMultEvents==1)
	{
	if( bRpcHit    &&
		 bPla0Left  &&
		 bPla0Right &&
		 bPla1Left  &&
		 bPla1Right  )
	   h3->Fill(0);
	
	if(
		 bPla0Left  &&
		 bPla0Right &&
		 bPla1Left  &&
		 bPla1Right  )
		{
	   h3->Fill(1);
		h9->Fill( ( (dPla0TimeLeft+dPla0TimeRight) - (dPla1TimeLeft+dPla1TimeRight) )*0.5,(dPla1TotLeft+dPla1TotRight) ); 
		}
	if( bRpcHit    &&
		 bPla0Left  &&
		 bPla0Right )
	
	   h3->Fill(2);
	
	if( bRpcHit    &&
		 bPla1Left  &&
		 bPla1Right  )
	   h3->Fill(3);
	
	if( bPlaTrig)
	   h3->Fill(4);
	
	if( bPla0Left  &&
		 bPla0Right )
		{
	   h3->Fill(5);
		h7->Fill(dPla0TimeLeft-dPla0TimeRight,(dPla0TotLeft+dPla0TotRight));
		}
	
	if( bPla1Left  &&
		 bPla1Right )
		{
	   h3->Fill(6);
		h8->Fill(dPla1TimeLeft-dPla1TimeRight,(dPla1TotLeft+dPla1TotRight));
		}
	}
	else
	{
	if( bRpcHit    &&
		 bPla0Left==1  &&
		 bPla0Right==1 &&
		 bPla1Left==1  &&
		 bPla1Right==1  )
	   h3->Fill(0);
	
	if(
		 bPla0Left==1  &&
		 bPla0Right==1 &&
		 bPla1Left==1  &&
		 bPla1Right==1  )
		{
	   h3->Fill(1);
		h9->Fill( ( (dPla0TimeLeft+dPla0TimeRight) - (dPla1TimeLeft+dPla1TimeRight) )*0.5,(dPla1TotLeft+dPla1TotRight) ); 
		}
	if( bRpcHit    &&
		 bPla0Left==1  &&
		 bPla0Right==1 )
	
	   h3->Fill(2);
	
	if( bRpcHit    &&
		 bPla1Left==1  &&
		 bPla1Right==1  )
	   h3->Fill(3);
	
	if( bPlaTrig==1)
	   h3->Fill(4);
	
	if( bPla0Left==1  &&
		 bPla0Right==1 )
		{
	   h3->Fill(5);
		h7->Fill(dPla0TimeLeft-dPla0TimeRight,(dPla0TotLeft+dPla0TotRight));
		}

	if( bPla1Left==1  &&
		 bPla1Right==1 )
		{
	   h3->Fill(6);
		h8->Fill(dPla1TimeLeft-dPla1TimeRight,(dPla1TotLeft+dPla1TotRight));
		}
	}
}
//------------------------------------------------------------------------------------------------


 //Create Canvases
gROOT->cd();   
/*
     TCanvas* c1 = new TCanvas( 
         "c1",
         "c1",
         1680.,1000);
      c1->SetFillColor(0);
		c1->Divide(1,2);
      c1->SetGridx(0);
      c1->SetGridy(0);
      c1->GetFrame()->SetFillColor(0);
      c1->GetFrame()->SetBorderSize(0);

 
      c1->cd(1);
      h1->GetXaxis()->SetTitle("Channel number on TDC 02");
      h1->GetYaxis()->SetTitle("Counts");
      gPad->SetLogz();
      h1->Draw("histtext");

      c1->cd(2);
      h2->GetXaxis()->SetTitle("Channel number on TDC 00");
      h2->GetYaxis()->SetTitle("Counts");
      gPad->SetLogz();
      h2->Draw("histtext");

      c1->cd(1);
      c1->Update();



     TCanvas* c2 = new TCanvas( 
         "c2",
         "c2",
         1680.,1000);
      c2->SetFillColor(0);
		//c2->Divide(1,3);
      c2->SetGridx(0);
      c2->SetGridy(0);
      c2->GetFrame()->SetFillColor(0);
      c2->GetFrame()->SetBorderSize(0);

 
      c2->cd(1);
      h3->GetXaxis()->SetTitle("");
		h3->GetXaxis()->SetBinLabel(1,"Rpc+2 Pla");
		h3->GetXaxis()->SetBinLabel(2,"2 Pla");
		h3->GetXaxis()->SetBinLabel(3,"Rpc+Pla0");
		h3->GetXaxis()->SetBinLabel(4,"Rpc+Pla1");
		h3->GetXaxis()->SetBinLabel(5,"Plastic Trigger");
		h3->GetXaxis()->SetBinLabel(6,"Plastic 0");
		h3->GetXaxis()->SetBinLabel(7,"Plastic 1");
      h3->GetYaxis()->SetTitle("Counts");
      h3->Draw("histtext");

      c2->cd(1);
      c2->Update();



	TCanvas* c3 = new TCanvas( 
         "c3",
         "c3",
         1680.,1000);
      c3->SetFillColor(0);
		c3->Divide(1,3);
      c3->SetGridx(0);
      c3->SetGridy(0);
      c3->GetFrame()->SetFillColor(0);
      c3->GetFrame()->SetBorderSize(0);


      c3->cd(1);
      h4->GetXaxis()->SetTitle("Hits on RPC-strips under PMT 0+4 Trigger");
      h4->GetYaxis()->SetTitle("Counts");
      h4->Draw("histtext");

      c3->cd(2);
      h5->GetXaxis()->SetTitle("Hits on RPC-strips");
      h5->GetYaxis()->SetTitle("Counts");
      h5->Draw("histtext");

      c3->cd(3);
      h6->GetXaxis()->SetTitle("Hits on RPC-strips");
      h6->GetYaxis()->SetTitle("Counts");
      h6->Draw("histtext");

      c3->cd(1);
      c3->Update();
*/
	TCanvas* c4 = new TCanvas( 
         "c4",
         "c4",
         1680.,1000);
      c4->SetFillColor(0);
		c4->Divide(1,3);
      c4->SetGridx(0);
      c4->SetGridy(0);
      c4->GetFrame()->SetFillColor(0);
      c4->GetFrame()->SetBorderSize(0);

      c4->cd(1);
      h7->GetXaxis()->SetTitle("t_left-t_right");
      h7->GetYaxis()->SetTitle("total Tot (left+right)");
      h7->Draw("colz");

      c4->cd(2);
      h8->GetXaxis()->SetTitle("t_left-t_right");
      h8->GetYaxis()->SetTitle("total Tot (left+right)");
      h8->Draw("colz");

      c4->cd(3);
      h9->GetXaxis()->SetTitle("t_pla0-t_pla1");
      h9->GetYaxis()->SetTitle("Tot of Pmt 1(left+right)");
      h9->Draw("colz");

	TCanvas* c5 = new TCanvas( 
         "Full Rpc Time resolution",
         "Full Rpc Time resolution",
         1680.,1000);
      c5->SetFillColor(0);
		c5->Divide(1,3);
      c5->SetGridx(0);
      c5->SetGridy(0);
      c5->GetFrame()->SetFillColor(0);
      c5->GetFrame()->SetBorderSize(0);

      c5->cd(1);
      h10->GetXaxis()->SetTitle("t_Rpc-t_Pla0");
      h10->GetYaxis()->SetTitle("Counts");
      h10->Draw();

      c5->cd(2);
      h11->GetXaxis()->SetTitle("t_Rpc-t_Pla0");
      h11->GetYaxis()->SetTitle("Counts");
      h11->Draw();

      c5->cd(2);
      h12->GetXaxis()->SetTitle("t_Rpc_l-t_Rpc_r");
      h12->GetYaxis()->SetTitle("Total Tot");
      h12->Draw("colz");


	TCanvas* c6 = new TCanvas( 
         "Rpc Channel resolution",
         "Rpc Channel resolution",
         1680.,1000);
      c6->SetFillColor(0);
		c6->Divide(4,4);
      c6->SetGridx(0);
      c6->SetGridy(0);
      c6->GetFrame()->SetFillColor(0);
      c6->GetFrame()->SetBorderSize(0);

	TCanvas* c7 = new TCanvas( 
         "Rpc Channel resolution triggered",
         "Rpc Channel resolution triggered",
         1680.,1000);
      c7->SetFillColor(0);
		c7->Divide(4,4);
      c7->SetGridx(0);
      c7->SetGridy(0);
      c7->GetFrame()->SetFillColor(0);
      c7->GetFrame()->SetBorderSize(0);

	TCanvas* c8 = new TCanvas( 
         "Rpc Hit position vs Tot",
         "Rpc Hit position vs Tot",
         1680.,1000);
      c8->SetFillColor(0);
		c8->Divide(4,4);
      c8->SetGridx(0);
      c8->SetGridy(0);
      c8->GetFrame()->SetFillColor(0);
      c8->GetFrame()->SetBorderSize(0);

 for(Int_t iHistogram=0; iHistogram<16; iHistogram++)
	{
		c6->cd(iHistogram+1);
		tChannelResHistogram[iHistogram]->GetXaxis()->SetTitle("t_Rpc-t_Pla0");
		tChannelResHistogram[iHistogram]->GetYaxis()->SetTitle("counts");
//    	tChannelResHistogram[iHistogram]->GetXaxis()->SetRangeUser(tChannelResHistogram[iHistogram]->GetMean(1)-(2**12),tChannelResHistogram[iHistogram]->GetMean(1)+(2**12));
		tChannelResHistogram[iHistogram]->Draw();
		tChannelResHistogram[iHistogram]->Fit("gaus");
	}

 for(Int_t iHistogram=0; iHistogram<16; iHistogram++)
	{
		c7->cd(iHistogram+1);
		tChannelResHistogramTrig[iHistogram]->GetXaxis()->SetTitle("t_Rpc-t_Pla0");
		tChannelResHistogramTrig[iHistogram]->GetYaxis()->SetTitle("counts");
//    	tChannelResHistogramTrig[iHistogram]->GetXaxis()->SetRangeUser(tChannelResHistogramTrig[iHistogram]->GetMean(1)-(2**12),tChannelResHistogramTrig[iHistogram]->GetMean(1)+(2**12));
		tChannelResHistogramTrig[iHistogram]->Draw();
		tChannelResHistogramTrig[iHistogram]->Fit("gaus");
	}

 for(Int_t iHistogram=0; iHistogram<16; iHistogram++)
	{
		c8->cd(iHistogram+1);
		tChannelYPosHistogram[iHistogram]->GetXaxis()->SetTitle("t_Rpc_l-t_Rpc_r");
		tChannelYPosHistogram[iHistogram]->GetYaxis()->SetTitle("Tot");
//    	tChannelResHistogramTrig[iHistogram]->GetXaxis()->SetRangeUser(tChannelResHistogramTrig[iHistogram]->GetMean(1)-(2**12),tChannelResHistogramTrig[iHistogram]->GetMean(1)+(2**12));
		tChannelYPosHistogram[iHistogram]->Draw("colz");

	}

}
