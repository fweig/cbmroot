void pl_all_time_resolution(UInt_t uTdcNumber1, UInt_t uTdcNumber2,const TString& sTreeFile = "unpack_CbmTofQa_05Jun1043_mrpc1_000_mrpc2_200_mrpc3_200_nb.out.root", UInt_t iZoomChannel=0, UInt_t DEBUG=0)
{
  Int_t const kTdcChNb = 16;
/*
  FairRootManager* tManager = FairRootManager::Instance();
  if(!tManager)
  {
    cout<<"FairRootManager could not be retrieved. Abort macro execution."<<endl;
    return;
  }
*/

   TFile* fAnalysis = new TFile( sTreeFile,"READ");


   // Recover a pointer on the TTree object and check how many events it contains

   TTree* tOutputTree = (TTree*)fAnalysis->Get("cbmsim");
  if(!tOutputTree)
  {
    cout<<"Output tree could not be retrieved from FairRootManager. Abort macro execution."<<endl;
    return;
  }

  TBranch* tBranch = tOutputTree->GetBranch("TofCalibData");
  if(!tBranch)
  {
    cout<<"Branch 'TofCalibData' not found in output tree. Abort macro execution."<<endl;
    return;
  }

  void* vOldAddress = tBranch->GetAddress();

  TDirectory* tOldDirectory = gDirectory;
  gROOT->cd();

  TClonesArray* tArray = new TClonesArray("TTofCalibData");
  tBranch->SetAddress(&tArray);

  TCanvas* tCanvas = new TCanvas("tCanvas","Time resolution",0,0,700,700);
  tCanvas->Divide(kTdcChNb/4,kTdcChNb/4);  

    TCanvas* tCanvas2 = new TCanvas("tCanvas2","Time resolution",0,0,700,700);
  tCanvas2->Divide(2,2);

    TCanvas* tCanvas4 = new TCanvas("tCanvas4","TotDistribution",0,0,700,700);
  	 tCanvas4->Divide(1,2);
if(DEBUG)
{
    TCanvas* tCanvas3 = new TCanvas("tCanvas3","Time resolution on same TDCs",0,0,700,700);
  tCanvas3->Divide(2,4);

    TCanvas* tCanvas5 = new TCanvas("tCanvas5","Correlations",0,0,700,700);
  	 tCanvas4->Divide(1,2);

 	TH2D* hTotVsTimeDistTdc1 = new TH2D ("hTotVsTimeDistTdc1",Form(" Tot-Distribution on TDC %u vs TimeDifference on Channel %u",uTdcNumber1,iZoomChannel),5000,-1000000,1000000,20000,0,2000000);
 	TH2D* hTotVsTimeDistTdc2 = new TH2D ("hTotVsTimeDistTdc2",Form(" Tot-Distribution on TDC %u vs TimeDifference on Channel %u",uTdcNumber2,iZoomChannel),5000,-1000000,1000000,20000,0,2000000);
}

 	TH2D* hTotDistLeft = new TH2D ("hTotDistLeft",Form("TDC %u: Tot-Distribution per channnel",uTdcNumber1 ),kTdcChNb,-0.5,(kTdcChNb-0.5),20000,0,2000000);
 	TH2D* hTotDistRight = new TH2D ("hTotDistRight",Form("TDC %u: Tot-Distribution per channnel",uTdcNumber2 ),kTdcChNb,-0.5,(kTdcChNb-0.5),20000,0,2000000);

  TH1D* tHistograms[kTdcChNb];
  TH1D* tTimeHistograms[2*kTdcChNb];

  for(Int_t iHistogram = 0; iHistogram < kTdcChNb; iHistogram++)
  {
    delete gROOT->FindObjectAny(Form("tHistogram%u",iHistogram));

    tHistograms[iHistogram] = new TH1D(Form("tHistogram%u",iHistogram),Form("TDCs %u-%u,chs %u-%u",uTdcNumber1,uTdcNumber2,iHistogram,iHistogram),8000000,-40000000,40000000);
  }

for(Int_t iHistogram = 0; iHistogram < kTdcChNb; iHistogram++)
  {
    delete gROOT->FindObjectAny(Form("tTimeHistogram%u",iHistogram));
    delete gROOT->FindObjectAny(Form("tTimeHistogram%u",iHistogram+kTdcChNb));

    tTimeHistograms[iHistogram] = new TH1D(Form("tTimeHistogramTDC1%u",iHistogram),Form("TDC %u,chs %u-%u",uTdcNumber1,iHistogram,((iHistogram+1)%4) ),800000,-400000,400000);

    tTimeHistograms[(iHistogram+kTdcChNb)] = new TH1D(Form("tTimeHistogramTDC2%u",iHistogram),Form("TDC %u,chs %u-%u",uTdcNumber2,iHistogram,((iHistogram+1)%4)),800000,-400000,400000);
  }

  Long64_t lBranchEntries = tBranch->GetEntries();

  for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
  {
    tArray->Clear("C");

    tBranch->GetEntry(lBranchEntry);

    Int_t iArrayEntries = tArray->GetEntriesFast();

    Int_t iDataIndexTdc1[kTdcChNb];
    Int_t iDataIndexTdc2[kTdcChNb];
    for(Int_t iTdcCh=0; iTdcCh < kTdcChNb; iTdcCh++)
    {
    	iTotIndexTdc1[kTdcChNb]= -1;
    	iTotIndexTdc2[kTdcChNb]= -1;
      iDataIndexTdc1[iTdcCh] = -1;
      iDataIndexTdc2[iTdcCh] = -1;
    }


    for(Int_t iArrayEntry = 0; iArrayEntry < iArrayEntries; iArrayEntry++)
    {
      TTofCalibData* tCalibTdcData = (TTofCalibData*)tArray->At(iArrayEntry);

      if(tCalibTdcData->GetBoard() == uTdcNumber1)
      {
        iDataIndexTdc1[tCalibTdcData->GetChannel()] = iArrayEntry;
      }
      if(tCalibTdcData->GetBoard() == uTdcNumber2)
      {
        iDataIndexTdc2[tCalibTdcData->GetChannel()] = iArrayEntry;
      }
    }

    for(Int_t iTdcCh=0; iTdcCh < kTdcChNb; iTdcCh++)
    {
      if(iDataIndexTdc1[iTdcCh] != -1 && iDataIndexTdc2[iTdcCh] != -1)
      {
/*
        if (lBranchEntry < 10)
        {
          cout<<((TTofCalibData*)tArray->At(iDataIndexTdc1[iTdcCh]))->GetTime()-((TTofCalibData*)tArray->At(iDataIndexTdc2[iTdcCh]))->GetTime()<<endl;
        }
*/

        tHistograms[iTdcCh]->Fill(((TTofCalibData*)tArray->At(iDataIndexTdc1[iTdcCh]))->GetTime()-((TTofCalibData*)tArray->At(iDataIndexTdc2[iTdcCh]))->GetTime());

		if(iTdcCh == iZoomChannel && DEBUG)
			{
        	hTotVsTimeDistTdc1->Fill( ((TTofCalibData*)tArray->At(iDataIndexTdc1[iTdcCh]))->GetTot() ,((TTofCalibData*)tArray->At(iDataIndexTdc1[iTdcCh]))->GetTime()-((TTofCalibData*)tArray->At(iDataIndexTdc2[iTdcCh]))->GetTime() );
        	hTotVsTimeDistTdc2->Fill( ((TTofCalibData*)tArray->At(iDataIndexTdc2[iTdcCh]))->GetTot() ,((TTofCalibData*)tArray->At(iDataIndexTdc1[iTdcCh]))->GetTime()-((TTofCalibData*)tArray->At(iDataIndexTdc2[iTdcCh]))->GetTime() );
			}

		hTotDistLeft->Fill(iTdcCh,((TTofCalibData*)tArray->At(iDataIndexTdc1[iTdcCh]))->GetTot() );
		hTotDistRight->Fill(iTdcCh,((TTofCalibData*)tArray->At(iDataIndexTdc2[iTdcCh]))->GetTot());
      }

      if(iDataIndexTdc1[iTdcCh] != -1 && iDataIndexTdc1[(iTdcCh+1)%4] != -1)
      {
        tTimeHistograms[iTdcCh]->Fill(((TTofCalibData*)tArray->At(iDataIndexTdc1[iTdcCh]))->GetTime()-((TTofCalibData*)tArray->At(iDataIndexTdc1[(iTdcCh+1)%4]))->GetTime());
		}
      if(iDataIndexTdc2[iTdcCh] != -1 && iDataIndexTdc2[(iTdcCh+1)%4] != -1)
      {
        tTimeHistograms[iTdcCh+kTdcChNb]->Fill(((TTofCalibData*)tArray->At(iDataIndexTdc2[iTdcCh]))->GetTime()-((TTofCalibData*)tArray->At(iDataIndexTdc2[(iTdcCh+1)%4]))->GetTime());
		}
    }

  } 
  
  tBranch->SetAddress(vOldAddress);

  tArray->Clear("C");
  tArray = NULL;
  
  for(Int_t iHistogram = 0; iHistogram < kTdcChNb; iHistogram++)
  {
    tCanvas->cd(iHistogram+1);
    gPad->SetLogy();
    tHistograms[iHistogram]->GetXaxis()->SetRangeUser(tHistograms[iHistogram]->GetMean(1)-300,tHistograms[iHistogram]->GetMean(1)+300);
    tHistograms[iHistogram]->DrawClone();
  }


	for (Int_t iHistogram = iZoomChannel; iHistogram < iZoomChannel+4; iHistogram++)
	{
    tCanvas2->cd((iHistogram%4)+1);
    gPad->SetLogy();
    tHistograms[iHistogram]->GetXaxis()->SetRangeUser(tHistograms[iHistogram]->GetMean(1)-10000000,tHistograms[iHistogram]->GetMean(1)+10000000);
    tHistograms[iHistogram]->Draw();
 	}  



	tCanvas4->cd(1);    
		gPad->SetLogz();
		hTotDistLeft->GetXaxis()->SetTitle("Channel Nb");
      hTotDistLeft->GetYaxis()->SetTitle("Tot");
      hTotDistLeft->Draw("colz");

	tCanvas4->cd(2);
		gPad->SetLogz();
		hTotDistRight->GetXaxis()->SetTitle("Channel Nb");
      hTotDistRight->GetYaxis()->SetTitle("Tot");
      hTotDistRight->Draw("colz");
if(DEBUG)
{

  for(Int_t iHistogram = 0; iHistogram < 4; iHistogram++)
  {
    tCanvas3->cd(iHistogram+1);
    gPad->SetLogy();
    tTimeHistograms[iHistogram]->GetXaxis()->SetRangeUser(tHistograms[iHistogram]->GetMean(1)-10000,tTimeHistograms[iHistogram]->GetMean(1)+10000);
    tTimeHistograms[iHistogram]->Draw();

    tCanvas3->cd(iHistogram+5);
    gPad->SetLogy();
    tTimeHistograms[iHistogram+kTdcChNb]->GetXaxis()->SetRangeUser(tTimeHistograms[iHistogram+kTdcChNb]->GetMean(1)-10000,tTimeHistograms[iHistogram+kTdcChNb]->GetMean(1)+10000);
    tTimeHistograms[iHistogram+kTdcChNb]->Draw();
  }


	tCanvas5->cd(1);
		gPad->SetLogz();
		hTotVsTimeDistTdc1->GetXaxis()->SetTitle(Form("Time difference Tdc %u - %u",uTdcNumber1,uTdcNumber2);
      hTotVsTimeDistTdc1->GetYaxis()->SetTitle(Form("Tot of TDC %u",uTdcNumber1));
      hTotVsTimeDistTdc1->Draw("colz");

	tCanvas5->cd(2);
		gPad->SetLogz();
		hTotVsTimeDistTdc2->GetXaxis()->SetTitle(Form("Time difference Tdc %u - %u",uTdcNumber1,uTdcNumber2);
      hTotVsTimeDistTdc2->GetYaxis()->SetTitle(Form("Tot of TDC %u",uTdcNumber2));
      hTotVsTimeDistTdc2->Draw("colz");
}	
  tOldDirectory->cd();
}
