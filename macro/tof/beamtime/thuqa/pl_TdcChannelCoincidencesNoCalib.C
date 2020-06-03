

void pl_TdcChannelCoincidencesNoCalib(const TString& sTreeFile = "unpack_CbmTofQa_28May1635_mrpc1_000_mrpc2_200_mrpc3_200_nb.out.root")
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
   TBranch* DigiBranch = RawTree->GetBranch("TofTrbTdc");
	TClonesArray* DigiArray = new TClonesArray("TTofTrbTdcBoard");
	DigiBranch->SetAddress(&DigiArray);
  	TTofTrbTdcBoard* DigiAdr = new TTofTrbTdcBoard(); 
  	TTofTrbTdcData* DigiData = new TTofTrbTdcData();
 //Create Histograms
gROOT->cd(); 
   TH1D* h1 = new TH1D ("h1","Hit distribution of TDC 00 (No Calib)",64,-0.5,63.5);
   TH1D* h2 = new TH1D ("h2","Hit distribution of TDC 02 (No Calib)",64,-0.5,63.5);
 	TH1D* h3 = new TH1D ("h3","Coincidence Pattern (No Calib)",7,-0.5,6.5);
 	TH1D* h4 = new TH1D ("h4","triggered RPC-Coincidences on strips (No Calib)",16,-0.5,15.5);
 	TH1D* h5 = new TH1D ("h5","RPC-Coincidences on strips (No Calib)",16,-0.5,15.5);
 	TH1D* h6 = new TH1D ("h6","RPC+2Pla-Coincidences on strips (No Calib)",16,-0.5,15.5);
 	TH1D* h7 = new TH1D ("h7","PMT-time difference PLA0 (No Calib)",2000,-100000,100000);
 	TH1D* h8 = new TH1D ("h8","PMT-time difference PLA1 (No Calib)",2000,-100000,100000);
 	TH1D* h9 = new TH1D ("h9","PMT-time difference PLA0-PLA1 (No Calib)",2000,-100000,100000);
 	TH2D* h10 = new TH2D ("h10","Leading edges vs falling edges by TDC0 channels",32,-0.5,31.5,3,-0.5,2.5);
 	TH2D* h11 = new TH2D ("h11","Leading edges vs falling edges by TDC2 channels",32,-0.5,31.5,3,-0.5,2.5);
cout<<"Filling Histograms"<<endl;
fAnalysis->cd();
 

//Eventschleife
for(Int_t jEvent = 0; jEvent < nentries; jEvent++)
{
   DigiBranch->GetEntry(jEvent);
	Int_t n=0;
         
	if(!jEvent%1000)
	cout<<jEvent<<" Events processed"<<endl;
			
   //Initializierung
	Int_t nentriesArray = DigiArray->GetEntriesFast();
//	cout<<nentriesArray<<"-Array-Entries in Event "<<jEvent<<endl;
	bool bRpcLeftRising[32] = false;
	bool bRpcLeftFalling[32] = false;
	bool bRpcRightRising[32] = false;
	bool bRpcRightFalling[32] = false;


	bool bTdcHitLeft[64] = false;
	bool bTdcHitRight[64] = false;



  for (Int_t i = 0; i < 32; i++)
	{
		bRpcRightRising[i] = false;
		bRpcLeftRising [i] = false;
		bRpcRightFalling[i] = false;
		bRpcLeftFalling [i] = false;
	}

  for (Int_t i = 0; i < 64; i++)
	{
		bTdcHitLeft[i] = false;
		bTdcHitRight[i] = false;
	}


	bool bRpcHit = false;
	bool bPla0LeftRising = false;
	bool bPla0RightRising = false;
	bool bPla1LeftRising = false;
	bool bPla1RightRising = false;
	bool bPlaTrigRising = false;
	bool bPla0LeftFalling = false;
	bool bPla0RightFalling = false;
	bool bPla1LeftFalling = false;
	bool bPla1RightFalling = false;
	bool bPlaTrigFalling = false;
//	Double_t dPla0TimeLeft = 0;
//	Double_t dPla0TimeRight = 0;
//	Double_t dPla1TimeLeft = 0;
//	Double_t dPla1TimeRight = 0; 




   for (Int_t jArray = 0; jArray < nentriesArray; jArray++)
      {
		DigiAdr = (TTofTrbTdcBoard*)DigiArray->At(jArray);

			int j=1;
			while (DigiAdr->GetDataPtr(j) != 0)
			{
		   DigiData = DigiAdr->GetDataPtr(j);

			if(DigiData!=0)
				{

				if (jArray == 0)
					h1->Fill(DigiData->GetChannel());
				if (jArray == 2)
					h2->Fill(DigiData->GetChannel());

				if (jArray == 0)
					 bTdcHitLeft[DigiData->GetChannel()] = true;
				if (jArray == 2)
					 bTdcHitRight[DigiData->GetChannel()] = true;


				if(DigiData->GetChannel()<32)
					{
					if (jArray == 0)
						if (DigiData->GetChannel()%2)
						bRpcLeftRising[DigiData->GetChannel()/2]= true;
						else
						bRpcLeftFalling[DigiData->GetChannel()/2]= true;
					if (jArray == 2)
						if (DigiData->GetChannel()%2)
						bRpcRightRising[DigiData->GetChannel()/2]= true;
						else
						bRpcRightFalling[DigiData->GetChannel()/2]= true;
					}

				if (34 == DigiData->GetChannel())
					{
				bPla0LeftRising = true;
					}
				if (35 == DigiData->GetChannel())
					{
				bPla0LeftFalling = true;
					}

				if (36 == DigiData->GetChannel())
					{
				bPla0RightRising = true;
					}
				if (37 == DigiData->GetChannel())
					{
				bPla0RightFalling = true;
					}


				if (38 == DigiData->GetChannel())
					{
				bPla1LeftRising = true;
					}
				if (39 == DigiData->GetChannel())
					{
				bPla1LeftFalling = true;
					}

				if (40 == DigiData->GetChannel())
					{
				bPla0RightRising = true;
					}
				if (41 == DigiData->GetChannel())
					{
				bPla0RightFalling = true;
					}


				if (42 == DigiData->GetChannel())
					{
				bPlaTrigRising = true;
					}
				if (43 == DigiData->GetChannel())
					{
				bPlaTrigFalling = true;
					}



//cout<<"blubb"<<endl;

				}
			j++;			
			}
      }


  for (Int_t i = 0; i < 16; i++)
	{
		if (bRpcLeftRising[i] && bRpcRightRising[i] &&
			 bRpcLeftFalling[i] && bRpcRightFalling[i])
		{
			bRpcHit= true;

			if(bPlaTrigRising && bPlaTrigFalling && bPla1RightRising && bPla1RightFalling)
				h4->Fill(i);

			if(bPla1RightRising && bPla1RightFalling && 
				bPla1LeftRising && bPla1LeftFalling && 
				bPla0RightRising && bPla0RightFalling && 
				bPla0LeftRising && bPla0LeftFalling)
				h6->Fill(i);

			h5->Fill(i);
		}
	}

  for (Int_t i = 0; i < 32; i++)
	{
		if (bTdcHitLeft[2*i] && bTdcHitLeft[2*i+1])
		{
			h10->Fill(i,2);
		}
		if (bTdcHitLeft[2*i] && !bTdcHitLeft[2*i+1])
		{
			h10->Fill(i,0);
		}
		if (!bTdcHitLeft[2*i] && bTdcHitLeft[2*i+1])
		{
			h10->Fill(i,1);
		}


		if (bTdcHitRight[2*i] && bTdcHitRight[2*i+1])
		{
			h11->Fill(i,2);
		}
		if (bTdcHitRight[2*i] && !bTdcHitRight[2*i+1])
		{
			h11->Fill(i,0);
		}
		if (!bTdcHitRight[2*i] && bTdcHitRight[2*i+1])
		{
			h11->Fill(i,1);
		}
	}

if( bRpcHit    &&
	 bPla0LeftRising && bPla0LeftFalling  &&
	 bPla0RightRising && bPla0RightFalling &&
	 bPla1LeftRising && bPla1LeftFalling  &&
	 bPla1RightRising && bPla1RightFalling  )
   h3->Fill(0);

if( bPla0LeftRising && bPla0LeftFalling  &&
	 bPla0RightRising && bPla0RightFalling &&
	 bPla1LeftRising && bPla1LeftFalling  &&
	 bPla1RightRising && bPla1RightFalling  )
	{
   h3->Fill(1);
	}
if( bRpcHit    &&
	 bPla0LeftRising && bPla0LeftFalling  &&
	 bPla0RightRising && bPla0RightFalling )

   h3->Fill(2);

if( bRpcHit    &&
	 bPla1LeftRising && bPla1LeftFalling  &&
	 bPla1RightRising && bPla1RightFalling  )
   h3->Fill(3);

if( bPlaTrigRising && bPlaTrigFalling  &&
	 bPla1RightRising && bPla1RightFalling )
   h3->Fill(4);

if( bPla0LeftRising && bPla0LeftFalling  &&
	 bPla0RightRising && bPla0RightFalling )
	{
   h3->Fill(5);
	}

if( bPla1LeftRising && bPla1LeftFalling  &&
	 bPla1RightRising && bPla1RightFalling )
	{
   h3->Fill(6);
	}
}



 //Create Canvases
gROOT->cd();   

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
      h1->Draw("colz,text");

      c1->cd(2);
      h2->GetXaxis()->SetTitle("Channel number on TDC 00");
      h2->GetYaxis()->SetTitle("Counts");
      gPad->SetLogz();
      h2->Draw("colz,text");

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
      h3->Draw("ehist");

      c2->cd(1);
      c2->Update();

/*

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
      h4->Draw("colz");

      c3->cd(2);
      h5->GetXaxis()->SetTitle("Hits on RPC-strips");
      h5->GetYaxis()->SetTitle("Counts");
      h5->Draw("colz");

      c3->cd(3);
      h6->GetXaxis()->SetTitle("Hits on RPC-strips");
      h6->GetYaxis()->SetTitle("Counts");
      h6->Draw("colz");

      c3->cd(1);
      c3->Update();
*/
/*
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
      h7->GetYaxis()->SetTitle("Counts");
      h7->Draw("colz");

      c4->cd(2);
      h8->GetXaxis()->SetTitle("t_left-t_right");
      h8->GetYaxis()->SetTitle("Counts");
      h8->Draw("colz");

      c4->cd(3);
      h9->GetXaxis()->SetTitle("t_pla0-t_pla1");
      h9->GetYaxis()->SetTitle("Counts");
		h9->Fit("gaus");
      h9->Draw("colz");
*/


	TCanvas* c5 = new TCanvas( 
         "c5",
         "c5",
         1680.,1000);
      c5->SetFillColor(0);
		
      c5->SetGridx(0);
      c5->SetGridy(0);
		c5->Divide(1,2);
      c5->GetFrame()->SetFillColor(0);
      c5->GetFrame()->SetBorderSize(0);

		c5->cd(1);
      h10->GetYaxis()->SetTitle("");
		h10->GetYaxis()->SetBinLabel(1,"Event with leading edge only");
		h10->GetYaxis()->SetBinLabel(2,"Event with trailing edge only");
		h10->GetYaxis()->SetBinLabel(3,"leading and falling edge");
      h10->GetXaxis()->SetTitle("Channels as used after calibration");
      h10->Draw("colz,text");

		c5->cd(2);
      h11->GetYaxis()->SetTitle("");
		h11->GetYaxis()->SetBinLabel(1,"Event with leading edge only");
		h11->GetYaxis()->SetBinLabel(2,"Event with trailing edge only");
		h11->GetYaxis()->SetBinLabel(3,"leading and falling edge");
      h11->GetXaxis()->SetTitle("Channels as used after calibration");
      h11->Draw("colz,text");


      c5->cd(1);
      c5->Update();



}
