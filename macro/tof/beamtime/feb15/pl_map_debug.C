
void pl_map_debug(Int_t iFirstTdc = 30, Int_t iSecondTdc = 30)
{
   // constants
   const Int_t kiNbCh = 16;

   // Prepare access to TTree data
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

   Long64_t lBranchEntries = tBranch->GetEntries();

   // Prepare Histos and variables
   Bool_t fbDebTdcChEvtThere[2][kiNbCh];
   delete gROOT->FindObjectAny( Form("fhDebTdcChEvtCoinc_tdc%03d_tdc%03d", iFirstTdc, iSecondTdc ) );
   TH2 * fhDebTdcChEvtCoinc  =  new TH2I(
         Form("fhDebTdcChEvtCoinc_tdc%03d_tdc%03d", iFirstTdc, iSecondTdc ),
         Form("Number of events with hits for in both channels for TDC #%03d and %03d; Channel TDC #%03d []; Channel TDC #%03d []; Events with hits in both[]",
               iFirstTdc, iSecondTdc, iFirstTdc, iSecondTdc ),
         kiNbCh, 0, kiNbCh,
         kiNbCh, 0, kiNbCh );

   // Loop on Data and plotting
   for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
   {
      if(0 == lBranchEntry%10000 && 0 < lBranchEntry)
         cout<<"Event "<<lBranchEntry<<" / "<< lBranchEntries <<" done!"<<endl;
      tArray->Clear("C");

      tBranch->GetEntry(lBranchEntry);

      Int_t iArrayEntries = tArray->GetEntriesFast();

      for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
      {
         fbDebTdcChEvtThere[0][iChA] = kFALSE;
         fbDebTdcChEvtThere[1][iChA] = kFALSE;
      } // for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)

      for(Int_t iArrayEntry = 0; iArrayEntry < iArrayEntries; iArrayEntry++)
      {
         TTofCalibData* tCalibTdcData = (TTofCalibData*)tArray->At(iArrayEntry);

         if(tCalibTdcData->GetBoard() == iFirstTdc )
         {
            fbDebTdcChEvtThere[0][tCalibTdcData->GetChannel()] = kTRUE;
         } // if(tCalibTdcData->GetBoard() == iFirstTdc )
         if(tCalibTdcData->GetBoard() == iSecondTdc )
         {
            fbDebTdcChEvtThere[1][tCalibTdcData->GetChannel()] = kTRUE;
         } // if(tCalibTdcData->GetBoard() == iSecondTdc )
      } // for(Int_t iArrayEntry = 0; iArrayEntry < iArrayEntries; iArrayEntry++)

      for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
         for( Int_t iChB = 0; iChB < kiNbCh; iChB ++)
            if( kTRUE == fbDebTdcChEvtThere[0][iChA] &&
                  kTRUE == fbDebTdcChEvtThere[1][iChB] )
               fhDebTdcChEvtCoinc->Fill(iChA, iChB);

   } // for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)

   tBranch->SetAddress(vOldAddress);

   tArray->Clear("C");
   tArray = NULL;

   delete gROOT->FindObjectAny( "tCanvasA" );
   TCanvas* tCanvas1 = new TCanvas("tCanvasA","Coincidences map",0,0,700,700);
   fhDebTdcChEvtCoinc->SetStats( kFALSE );
   fhDebTdcChEvtCoinc->Draw("colz");

   tOldDirectory->cd();
}
