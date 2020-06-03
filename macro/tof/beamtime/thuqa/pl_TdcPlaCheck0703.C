

void pl_TdcPlaCheck0703(const TString& sTreeFile = "unpack_CbmTofQa_02Jul1721_mrpc1_000_mrpc2_200_mrpc3_200_nb.out.root")
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
   TH1D* hTdc02HitDist  = new TH1D ("hTdc02HitDist ","Hit distribution of TDC 02",32,-0,32);
   TH1D* hTdc03HitDist  = new TH1D ("hTdc03HitDist ","Hit distribution of TDC 03",32,-0,32);
 	TH1D* hPlaTimeDiff = new TH1D ("hPlaTimeDiff","Pla-time difference PLA0-PLA1 (first hit in channel)",20000,-10**6,10**6);
	TH2D* hPla0YPosTot = new TH2D ("hPla0YPosTot","Pla0: Time_L-Time_R vs Tot_L+Tot_R",20000,-10**5,10**5,2000,0,5000000);
	TH2D* hPla1YPosTot = new TH2D("hPla1YPosTot","Pla1: Time_L-Time_R vs Tot_L+Tot_R",20000,-10**5,10**5,2000,0,5000000);


cout<<"Filling Histograms"<<endl;
fAnalysis->cd();
 

//event loop----------------------------------------------------------------------------------
for(Int_t jEvent = 0; jEvent < nentries; jEvent++)
{
   DigiBranch->GetEntry(jEvent);
	Int_t n=0;
         
	if(!jEvent%10000)
	cout<<jEvent<<" Events processed"<<endl;
			
   //Initializierung
	Int_t nentriesArray = DigiArray->GetEntriesFast();
	//cout<<nentriesArray<<"-Array-Entries in Event "<<jArray<<endl;

	UInt_t bPla0Left = 0;
	UInt_t bPla0Right = 0;
	UInt_t bPla1Left = 0;
	UInt_t bPla1Right = 0;
	Double_t dPla0TimeLeft = 0;
	Double_t dPla0TimeRight = 0;
	Double_t dPla1TimeLeft = 0;
	Double_t dPla1TimeRight = 0; 
	Double_t dPla0TotLeft = 0;
	Double_t dPla0TotRight = 0;
	Double_t dPla1TotLeft = 0;
	Double_t dPla1TotRight = 0; 

//Loop over array entries-----------------------------------------------------------------------------
   for (Int_t jArray = 0; jArray < nentriesArray; jArray++)
      {
		DigiAdr = (TTofCalibData*)DigiArray->At(jArray);

			if (DigiAdr->GetBoard()==2)
				hTdc02HitDist ->Fill(DigiAdr->GetChannel());
			if (DigiAdr->GetBoard()==3)
				hTdc03HitDist ->Fill(DigiAdr->GetChannel());

//Fill Plastic variables (only Padi_digital_out_1)
			if (DigiAdr->GetBoard()==3)
			{
				if (2 == DigiAdr->GetChannel()) //PMT1: ch 2 & ch 10
				{
					bPla0Left++;
					if(dPla0TimeLeft==0){dPla0TimeLeft=DigiAdr->GetTime();}
					if(dPla0TotLeft==0){dPla0TotLeft=DigiAdr->GetTot();}
				}

				if (3 == DigiAdr->GetChannel()) //PMT2: ch 3 & ch 11
				{
					bPla0Right++;
					if(dPla0TimeRight==0){dPla0TimeRight=DigiAdr->GetTime();}
					if(dPla0TotRight==0){dPla0TotRight=DigiAdr->GetTot();}
				}

				if (4 == DigiAdr->GetChannel()) //PMT3: ch 4 & ch 12
				{
					bPla1Left++;
					if(dPla1TimeLeft==0){dPla1TimeLeft=DigiAdr->GetTime();}
					if(dPla1TotLeft==0){dPla1TotLeft=DigiAdr->GetTot();}
				}

				if (5 == DigiAdr->GetChannel()) //PMT4: ch 5 & ch 13
				{
					bPla1Right++;
					if(dPla1TimeRight==0){dPla1TimeRight=DigiAdr->GetTime();}
					if(dPla1TotRight==0){dPla1TotRight=DigiAdr->GetTot();}
				}	
			}
      }

//------------------------------------------------------------------------
       if(bPla0Right && bPla0Left)
       {
			 hPla0YPosTot->Fill(((dPla0TimeLeft-dPla0TimeRight)) ,(dPla0TotLeft+dPla0TotRight));
		 }

       if(bPla1Right && bPla1Left)
       {
			 hPla1YPosTot->Fill(((dPla1TimeLeft-dPla1TimeRight)) ,(dPla1TotLeft+dPla1TotRight));
		 }

       if(bPla0Right && bPla0Left && bPla1Right && bPla1Left)
       {
			 hPlaTimeDiff->Fill( ((dPla0TimeLeft+dPla0TimeRight)/2) - ((dPla1TimeLeft+dPla1TimeRight)/2) );
		 }
}
//------------------------------------------------------------------------------------------------

 //Create Canvases
gROOT->cd();   

     TCanvas* c1 = new TCanvas( "Tdc hit distribution","Tdc hit distribution",1680.,1000);
      c1->SetFillColor(0);
		c1->Divide(1,2);
      c1->SetGridx(0);
      c1->SetGridy(0);
      c1->GetFrame()->SetFillColor(0);
      c1->GetFrame()->SetBorderSize(0);
 
      c1->cd(1);
      hTdc02HitDist ->GetXaxis()->SetTitle("Channel number on TDC 02");
      hTdc02HitDist ->GetYaxis()->SetTitle("Counts");
      gPad->SetLogz();
      hTdc02HitDist ->Draw("histtext");

      c1->cd(2);
      hTdc03HitDist ->GetXaxis()->SetTitle("Channel number on TDC 03");
      hTdc03HitDist ->GetYaxis()->SetTitle("Counts");
      gPad->SetLogz();
      hTdc03HitDist ->Draw("histtext");

      c1->cd(1);
      c1->Update();


 	  TCanvas* c4 = new TCanvas("Full pla Time resolution","Full pla Time resolution",1680.,1000);
      c4->SetFillColor(0);
		c4->Divide(1,3);
      c4->SetGridx(0);
      c4->SetGridy(0);
      c4->GetFrame()->SetFillColor(0);
      c4->GetFrame()->SetBorderSize(0);

      c4->cd(1);
      gPad->SetLogy();
    	//hPlaTimeDiff->GetXaxis()->SetRangeUser(-1**12,1**12);
      hPlaTimeDiff->GetXaxis()->SetTitle("t_pla0-t_Pla1");
      hPlaTimeDiff->GetYaxis()->SetTitle("Counts");
      hPlaTimeDiff->Draw();

      c4->cd(2);
      gPad->SetLogz();
      hPla0YPosTot->GetXaxis()->SetTitle("t_Pla0_l-t_Pla0_r");
      hPla0YPosTot->GetYaxis()->SetTitle("Total Tot");
      hPla0YPosTot->Draw("colz");

      c4->cd(3);
      gPad->SetLogz();
      hPla1YPosTot->GetXaxis()->SetTitle("t_Pla1_l-t_Pla1_r");
      hPla1YPosTot->GetYaxis()->SetTitle("Total Tot");
      hPla1YPosTot->Draw("colz");

}
