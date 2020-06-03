void pl_all_time_resolution(UInt_t uTdcNumber1, UInt_t uTdcNumber2)
{
  Int_t const kTdcChNb = 16;

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

  TCanvas* tCanvas = new TCanvas("tCanvas","Time resolution",0,0,700,700);
  tCanvas->Divide(kTdcChNb/4,kTdcChNb/4);

  TH1D* tHistograms[kTdcChNb];

  for(Int_t iHistogram = 0; iHistogram < kTdcChNb; iHistogram++)
  {
    delete gROOT->FindObjectAny(Form("tHistogram%u",iHistogram));

    tHistograms[iHistogram] = new TH1D(Form("tHistogram%u",iHistogram),Form("TDCs %u-%u,chs %u-%u",uTdcNumber1,uTdcNumber2,iHistogram,iHistogram),400000,0,400000);
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
      else if(tCalibTdcData->GetBoard() == uTdcNumber2)
      {
        iDataIndexTdc2[tCalibTdcData->GetChannel()] = iArrayEntry;
      }
    }

    for(Int_t iTdcCh=0; iTdcCh < kTdcChNb; iTdcCh++)
    {
      if(iDataIndexTdc1[iTdcCh] != -1 && iDataIndexTdc2[iTdcCh] != -1)
      {
        if (lBranchEntry < 10)
        {
          cout<<((TTofCalibData*)tArray->At(iDataIndexTdc1[iTdcCh]))->GetTime()-((TTofCalibData*)tArray->At(iDataIndexTdc2[iTdcCh]))->GetTime()<<endl;
        }
        tHistograms[iTdcCh]->Fill(((TTofCalibData*)tArray->At(iDataIndexTdc1[iTdcCh]))->GetTime()-((TTofCalibData*)tArray->At(iDataIndexTdc2[iTdcCh]))->GetTime());
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
    tHistograms[iHistogram]->Draw();
  }

  tOldDirectory->cd();
}
