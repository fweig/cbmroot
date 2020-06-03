void pl_pla_res_vs_tot(UInt_t uTdcNumber1, UInt_t uChNumber1, UInt_t uTdcNumber2, UInt_t uChNumber2,
                        UInt_t uTdcNumber3, UInt_t uChNumber3, UInt_t uTdcNumber4, UInt_t uChNumber4)
{

  delete gROOT->FindObjectAny("tHistogram1");
  delete gROOT->FindObjectAny("tHistogram2");
  delete gROOT->FindObjectAny("tHistogram3");
  delete gROOT->FindObjectAny("tHistogram4");

  FairRootManager* tManager = FairRootManager::Instance();
  if(!tManager)
  {
    cout<<"FairRootManager could not be retrieved. Abort macro execution."<<endl;
    return;
  }

  TTree* tOutputTree = tManager->GetOutTree();
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

  TPaveStats* tStats = 0;
  TPaletteAxis* tPalette = 0;

  TCanvas* tCanvas1 = new TCanvas("tCanvas1","",0,0,500,500);
  tCanvas1->SetFillColor(0);
  tCanvas1->SetGridx(0);
  tCanvas1->SetGridy(0);
  tCanvas1->SetTopMargin(0);
  tCanvas1->SetRightMargin(0);
  tCanvas1->SetBottomMargin(0);
  tCanvas1->SetLeftMargin(0);

  tCanvas1->Divide(2,2);

  TH2D* tHistogram1 = new TH2D("tHistogram1","",300,20000,50000,2000,-10000,10000);
  TH2D* tHistogram2 = new TH2D("tHistogram2","",300,20000,50000,2000,-10000,10000);
  TH2D* tHistogram3 = new TH2D("tHistogram3","",300,20000,50000,2000,-10000,10000);
  TH2D* tHistogram4 = new TH2D("tHistogram4","",300,20000,50000,2000,-10000,10000);

  tHistogram1->GetXaxis()->SetTitle(Form("tot PMT (%u,%u)",uTdcNumber1,uChNumber1));
  tHistogram1->GetYaxis()->SetTitle(Form("dt PLA (%u,%u;%u,%u) - (%u,%u;%u,%u) [ps]",uTdcNumber1,uChNumber1,uTdcNumber2,uChNumber2,uTdcNumber3,uChNumber3,uTdcNumber4,uChNumber4));

  tHistogram2->GetXaxis()->SetTitle(Form("tot PMT (%u,%u)",uTdcNumber2,uChNumber2));
  tHistogram2->GetYaxis()->SetTitle(Form("dt PLA (%u,%u;%u,%u) - (%u,%u;%u,%u) [ps]",uTdcNumber1,uChNumber1,uTdcNumber2,uChNumber2,uTdcNumber3,uChNumber3,uTdcNumber4,uChNumber4));

  tHistogram3->GetXaxis()->SetTitle(Form("tot PMT (%u,%u)",uTdcNumber3,uChNumber3));
  tHistogram3->GetYaxis()->SetTitle(Form("dt PLA (%u,%u;%u,%u) - (%u,%u;%u,%u) [ps]",uTdcNumber1,uChNumber1,uTdcNumber2,uChNumber2,uTdcNumber3,uChNumber3,uTdcNumber4,uChNumber4));

  tHistogram4->GetXaxis()->SetTitle(Form("tot PMT (%u,%u)",uTdcNumber4,uChNumber4));
  tHistogram4->GetYaxis()->SetTitle(Form("dt PLA (%u,%u;%u,%u) - (%u,%u;%u,%u) [ps]",uTdcNumber1,uChNumber1,uTdcNumber2,uChNumber2,uTdcNumber3,uChNumber3,uTdcNumber4,uChNumber4));

  Long64_t lBranchEntries = tBranch->GetEntries();

  for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
  {
    tArray->Clear("C");

    tBranch->GetEntry(lBranchEntry);

    Int_t iDataIndex1 = -1;
    Int_t iDataIndex2 = -1;
    Int_t iDataIndex3 = -1;
    Int_t iDataIndex4 = -1;
    
    Int_t iArrayEntries = tArray->GetEntriesFast();

    for(Int_t iArrayEntry = 0; iArrayEntry < iArrayEntries; iArrayEntry++)
    {
      TTofCalibData* tCalibTdcData = (TTofCalibData*)tArray->At(iArrayEntry);

      if(tCalibTdcData->GetBoard() == uTdcNumber1 && tCalibTdcData->GetChannel() == uChNumber1)
      {
        iDataIndex1 = iArrayEntry;
      }
      else if(tCalibTdcData->GetBoard() == uTdcNumber2 && tCalibTdcData->GetChannel() == uChNumber2)
      {
        iDataIndex2 = iArrayEntry;
      }

      else if(tCalibTdcData->GetBoard() == uTdcNumber3 && tCalibTdcData->GetChannel() == uChNumber3)
      {
        iDataIndex3 = iArrayEntry;
      }

      else if(tCalibTdcData->GetBoard() == uTdcNumber4 && tCalibTdcData->GetChannel() == uChNumber4)
      {
        iDataIndex4 = iArrayEntry;
      }


    }

    if(iDataIndex1 != -1 && iDataIndex2 != -1 && iDataIndex3 != -1 && iDataIndex4 != -1)
    {
      tHistogram1->Fill(((TTofCalibData*)tArray->At(iDataIndex1))->GetTot(),
                         0.5*(((TTofCalibData*)tArray->At(iDataIndex1))->GetTime()+((TTofCalibData*)tArray->At(iDataIndex2))->GetTime())
                        -0.5*(((TTofCalibData*)tArray->At(iDataIndex3))->GetTime()+((TTofCalibData*)tArray->At(iDataIndex4))->GetTime()) );

      tHistogram2->Fill(((TTofCalibData*)tArray->At(iDataIndex2))->GetTot(),
                         0.5*(((TTofCalibData*)tArray->At(iDataIndex1))->GetTime()+((TTofCalibData*)tArray->At(iDataIndex2))->GetTime())
                        -0.5*(((TTofCalibData*)tArray->At(iDataIndex3))->GetTime()+((TTofCalibData*)tArray->At(iDataIndex4))->GetTime()) );

      tHistogram3->Fill(((TTofCalibData*)tArray->At(iDataIndex3))->GetTot(),
                         0.5*(((TTofCalibData*)tArray->At(iDataIndex1))->GetTime()+((TTofCalibData*)tArray->At(iDataIndex2))->GetTime())
                        -0.5*(((TTofCalibData*)tArray->At(iDataIndex3))->GetTime()+((TTofCalibData*)tArray->At(iDataIndex4))->GetTime()) );

      tHistogram4->Fill(((TTofCalibData*)tArray->At(iDataIndex4))->GetTot(),
                         0.5*(((TTofCalibData*)tArray->At(iDataIndex1))->GetTime()+((TTofCalibData*)tArray->At(iDataIndex2))->GetTime())
                        -0.5*(((TTofCalibData*)tArray->At(iDataIndex3))->GetTime()+((TTofCalibData*)tArray->At(iDataIndex4))->GetTime()) );

    }

  }

  tBranch->SetAddress(vOldAddress);

  tArray->Clear("C");
  tArray = NULL;

/*
  tHistogram->GetXaxis()->SetTitleSize(0.06);
  tHistogram->GetXaxis()->SetTitleOffset(1.20);
  tHistogram->GetXaxis()->SetLabelSize(0.055);
  tHistogram->GetXaxis()->SetLabelOffset(0.018);
  tHistogram->GetYaxis()->SetTitleSize(0.06);
  tHistogram->GetYaxis()->SetTitleOffset(1.27);
  tHistogram->GetYaxis()->SetLabelSize(0.055);
  tHistogram->GetYaxis()->SetLabelOffset(0.01);
  tHistogram->SetLineColor(kBlue);
  tHistogram->SetLineWidth(2);
*/

  tCanvas1->cd(1);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.15);
  tHistogram1->Draw("colz");

  tCanvas1->cd(2);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.15);
  tHistogram2->Draw("colz");

  tCanvas1->cd(3);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.15);
  tHistogram3->Draw("colz");

  tCanvas1->cd(4);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.15);
  tHistogram4->Draw("colz");

/*
  (tHistogram->GetYaxis())->SetRangeUser(0.2,tHistogram->GetMaximum()+1000.);
  tHistogram->Draw();
  gPad->Update();
  tHistogram->Fit("gaus","Q");
  (tHistogram->GetFunction("gaus"))->FixParameter(0,(tHistogram->GetFunction("gaus"))->GetParameter(0));
  (tHistogram->GetFunction("gaus"))->FixParameter(1,(tHistogram->GetFunction("gaus"))->GetParameter(1));
  (tHistogram->GetFunction("gaus"))->SetLineWidth(3);
  gPad->SetLogy();
  tStats = (TPaveStats*)tHistogram->FindObject("stats");
  tStats->SetOptStat(1010);
  tStats->SetOptFit(11);
  tStats->SetX1NDC(0.50);
  tStats->SetY1NDC(0.70);
  tStats->SetX2NDC(0.95);
  tStats->SetY2NDC(0.95);
  tStats->SetTextSizePixels(25);
  tStats = 0;
  tPalette = 0;
  gPad->Modified();
  gPad->Update();
*/

  tOldDirectory->cd();
}
