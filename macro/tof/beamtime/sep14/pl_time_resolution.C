void pl_time_resolution(UInt_t uTdcNumber1, UInt_t uChNumber1, UInt_t uTdcNumber2, UInt_t uChNumber2)
{

  delete gROOT->FindObjectAny("tHistogram");

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
  TH1D* tHistogram = new TH1D("tHistogram","",5000,0,50000);
//  TH2D* tHistogram = new TH2D("tHistogram","",10000,-50000,50000,10000,-50000,50000);

  Long64_t lBranchEntries = tBranch->GetEntries();

  for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
  {
    tArray->Clear("C");

    tBranch->GetEntry(lBranchEntry);

    Int_t iDataIndex1 = -1;
    Int_t iDataIndex2 = -1;
//    Int_t iDataIndex3 = -1;
//    Int_t iDataIndex4 = -1;
    
    Int_t iArrayEntries = tArray->GetEntriesFast();

    for(Int_t iArrayEntry = 0; iArrayEntry < iArrayEntries; iArrayEntry++)
    {
      TTofCalibData* tCalibTdcData = (TTofCalibData*)tArray->At(iArrayEntry);

//      cout << "TDC event: " << lBranchEntry << ", TDC board: " << tCalibTdcData->GetBoard() << ", TDC channel: " << tCalibTdcData->GetChannel() << endl;

      if(tCalibTdcData->GetBoard() == uTdcNumber1 && tCalibTdcData->GetChannel() == uChNumber1)
      {
        iDataIndex1 = iArrayEntry;
      }
      else if(tCalibTdcData->GetBoard() == uTdcNumber2 && tCalibTdcData->GetChannel() == uChNumber2)
      {
        iDataIndex2 = iArrayEntry;
      }
/*
      else if(tCalibTdcData->GetBoard() == uTdcNumber1 && tCalibTdcData->GetChannel() == 0)
      {
        iDataIndex3 = iArrayEntry;
      }
      else if(tCalibTdcData->GetBoard() == uTdcNumber1 && tCalibTdcData->GetChannel() == 15)
      {
        iDataIndex4 = iArrayEntry;
      }
*/

    }

    if(iDataIndex1 != -1 && iDataIndex2 != -1 )//&& iDataIndex3 != -1 && iDataIndex4 != -1)
    {
      tHistogram->Fill(((TTofCalibData*)tArray->At(iDataIndex1))->GetTime()-((TTofCalibData*)tArray->At(iDataIndex2))->GetTime());//,
//                       ((TTofCalibData*)tArray->At(iDataIndex3))->GetTime()-((TTofCalibData*)tArray->At(iDataIndex4))->GetTime() );
    }

  }

  tBranch->SetAddress(vOldAddress);

  tArray->Clear("C");
  tArray = NULL;

  tHistogram->Draw();

  tOldDirectory->cd();
}
