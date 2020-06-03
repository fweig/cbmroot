void pl_tot_per_channel(UInt_t uTdcNumber)
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

  TCanvas* tCanvas = new TCanvas("tCanvas","Pulse widths",0,0,700,700);
  tCanvas->Divide(kTdcChNb/4,kTdcChNb/4);

  TH1D* tHistograms[kTdcChNb];

  for(Int_t iHistogram = 0; iHistogram < kTdcChNb; iHistogram++)
  {
    delete gROOT->FindObjectAny(Form("tHistogram%u",iHistogram));

    tHistograms[iHistogram] = new TH1D(Form("tHistogram%u",iHistogram),Form("TOT TDC %u ch %u [ps]",uTdcNumber,iHistogram),3000,0,30000);
  }

  Long64_t lBranchEntries = tBranch->GetEntries();

  for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
  {
    tArray->Clear("C");

    tBranch->GetEntry(lBranchEntry);

    Int_t iArrayEntries = tArray->GetEntriesFast();

    Int_t iDataIndexCh[kTdcChNb];

    for(Int_t iTdcCh=0; iTdcCh < kTdcChNb; iTdcCh++)
    {
      iDataIndexCh[iTdcCh] = -1;
    }

    for(Int_t iArrayEntry = 0; iArrayEntry < iArrayEntries; iArrayEntry++)
    {
      TTofCalibData* tCalibTdcData = (TTofCalibData*)tArray->At(iArrayEntry);

      if(tCalibTdcData->GetBoard() == uTdcNumber)
      {
        iDataIndexCh[tCalibTdcData->GetChannel()] = iArrayEntry;
      }
    }

    for(Int_t iTdcCh=0; iTdcCh < kTdcChNb; iTdcCh++)
    {
      if(iDataIndexCh[iTdcCh] != -1)
      {
        tHistograms[iTdcCh]->Fill(((TTofCalibData*)tArray->At(iDataIndexCh[iTdcCh]))->GetTot());
      }
    }

  }

  tBranch->SetAddress(vOldAddress);

  tArray->Clear("C");
  tArray = NULL;

  for(Int_t iHistogram = 0; iHistogram < kTdcChNb; iHistogram++)
  {
    tCanvas->cd(iHistogram+1);
    tHistograms[iHistogram]->Draw();
  }

  tOldDirectory->cd();
}
