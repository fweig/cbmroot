void pl_diam_tres(UInt_t uTdcNumber1 = 32, UInt_t uTdcNumber2 = 33)
{
   const Int_t kiNbCh = 16;

   // Cleanup
   for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
      for( Int_t iChB = 0; iChB < kiNbCh; iChB ++)
      {
         delete gROOT->FindObjectAny( Form("tHistogram_%02d%_%02d", iChA, iChB ) );
         delete gROOT->FindObjectAny( Form("tHistogramFit_%02d%_%02d", iChA, iChB ) );
      }

   delete gROOT->FindObjectAny( "tHistogram_Entries" );
   delete gROOT->FindObjectAny( "tHistogram_Mean" );
   delete gROOT->FindObjectAny( "tHistogram_Summary" );

   // Prepare Trre access
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

   // Prepare histos and variables
   TH2D* tHistogram_Entries = new TH2D("tHistogram_Entries","",kiNbCh,0,kiNbCh, kiNbCh, 0, kiNbCh);
   TH2D* tHistogram_Mean    = new TH2D("tHistogram_Mean","",   kiNbCh,0,kiNbCh, kiNbCh, 0, kiNbCh);
   TH2D* tHistogram_Summary = new TH2D("tHistogram_Summary","",kiNbCh,0,kiNbCh, kiNbCh, 0, kiNbCh);
   TH1D* tHistogram[kiNbCh][kiNbCh];
   TH1D* tHistogramFit[kiNbCh][kiNbCh];
   for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
      for( Int_t iChB = 0; iChB < kiNbCh; iChB ++)
         tHistogram[iChA][iChB] = new TH1D(
               Form("tHistogram_%02d%_%02d", iChA, iChB ), "", 10000,-1000000,1000000);

   Long64_t lBranchEntries = tBranch->GetEntries();
   Int_t iDataIndex1[kiNbCh];
   Int_t iDataIndex2[kiNbCh];
   Int_t iDataMulti1[kiNbCh];
   Int_t iDataMulti2[kiNbCh];

   // Loop a first time to find the right histos boundaries
   for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
   {
      if(0 == lBranchEntry%10000 && 0 < lBranchEntry)
         cout<<"Event "<<lBranchEntry<<" / "<< lBranchEntries <<" done!"<<endl;
      tArray->Clear("C");

      tBranch->GetEntry(lBranchEntry);

      for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
      {
         iDataIndex1[iChA] = -1;
         iDataIndex2[iChA] = -1;
         iDataMulti1[iChA] =  0;
         iDataMulti2[iChA] =  0;
      }

      Int_t iArrayEntries = tArray->GetEntriesFast();

      for(Int_t iArrayEntry = 0; iArrayEntry < iArrayEntries; iArrayEntry++)
      {
         TTofCalibData* tCalibTdcData = (TTofCalibData*)tArray->At(iArrayEntry);

         if(tCalibTdcData->GetBoard() == uTdcNumber1 )
         {
            iDataIndex1[tCalibTdcData->GetChannel()] = iArrayEntry;
            iDataMulti1[tCalibTdcData->GetChannel()] ++;
         }
         if(tCalibTdcData->GetBoard() == uTdcNumber2 )
         {
            iDataIndex2[tCalibTdcData->GetChannel()] = iArrayEntry;
            iDataMulti2[tCalibTdcData->GetChannel()] ++;
         }
      }

      for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
         for( Int_t iChB = 0; iChB < kiNbCh; iChB ++)
            if(iDataIndex1[iChA] != -1 && iDataIndex2[iChB] != -1 &&
                1 == iDataMulti1[iChA] &&  1 == iDataMulti2[iChB] )
            {
               tHistogram[iChA][iChB]->Fill(
                       ((TTofCalibData*)tArray->At(iDataIndex1[iChA]))->GetTime()
                     - ((TTofCalibData*)tArray->At(iDataIndex2[iChB]))->GetTime() );
            } // if(iDataIndex1[iChA] != -1 && iDataIndex2[iChB] != -1 )
   } // for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)

   delete gROOT->FindObjectAny( "tCanvasBED" );
   TCanvas* tCanvasBED = new TCanvas("tCanvasBED","singles",0,0,700,700);
   tCanvasBED->Divide(kiNbCh, kiNbCh);
   // Find histo boundaries
   for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
      for( Int_t iChB = 0; iChB < kiNbCh; iChB ++)
         if( 0 < tHistogram[iChA][iChB]->GetEntries() )
         {
            tCanvasBED->cd( iChA * kiNbCh + 1 + iChB);
            tHistogram[iChA][iChB]->Draw();
            if( 0 == tHistogram[iChA][iChB]->Integral() )
            {
               cout<<"Check overflow and underflow for ch "<<iChA<<" vs "<<iChB<<endl;
               tHistogramFit[iChA][iChB] = new TH1D(
                     Form("tHistogramFit_%02d%_%02d", iChA, iChB ), "", 10000,-1000000,1000000);
               continue;
            } // if( 0 == tHistogram[iChA][iChB]->Integral() )
            Double_t dLow    = tHistogram[iChA][iChB]->GetMean() - 3*tHistogram[iChA][iChB]->GetRMS();
            Double_t dHigh   = tHistogram[iChA][iChB]->GetMean() + 3*tHistogram[iChA][iChB]->GetRMS();
//            Double_t dLow    = tHistogram[iChA][iChB]->GetMean() - 50000;
//            Double_t dHigh   = tHistogram[iChA][iChB]->GetMean() + 50000;
            Int_t    iNbBins = (Int_t)( TMath::Abs( dHigh - dLow) );
            //cout<<iNbBins<<endl;
            if( 100000 < iNbBins)
               iNbBins = 100000;

            tHistogramFit[iChA][iChB] = new TH1D(
                  Form("tHistogramFit_%02d%_%02d", iChA, iChB ), "", iNbBins, dLow, dHigh);
         } // if( 0 < tHistogram[iChA][iChB]->GetEntries() )
         else tHistogramFit[iChA][iChB] = new TH1D(
               Form("tHistogramFit_%02d%_%02d", iChA, iChB ), "", 10000,-1000000,1000000);

   // Loop a second time to fill properly dimensioned histos
   for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
   {
      if(0 == lBranchEntry%10000 && 0 < lBranchEntry)
         cout<<"Event "<<lBranchEntry<<" / "<< lBranchEntries <<" done!"<<endl;
      tArray->Clear("C");

      tBranch->GetEntry(lBranchEntry);

      for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
      {
         iDataIndex1[iChA] = -1;
         iDataIndex2[iChA] = -1;
         iDataMulti1[iChA] =  0;
         iDataMulti2[iChA] =  0;
      }

      Int_t iArrayEntries = tArray->GetEntriesFast();

      for(Int_t iArrayEntry = 0; iArrayEntry < iArrayEntries; iArrayEntry++)
      {
         TTofCalibData* tCalibTdcData = (TTofCalibData*)tArray->At(iArrayEntry);

         if(tCalibTdcData->GetBoard() == uTdcNumber1 &&
               0 < tCalibTdcData->GetTime() )
         {
            iDataIndex1[tCalibTdcData->GetChannel()] = iArrayEntry;
            iDataMulti1[tCalibTdcData->GetChannel()] ++;
         }
         if(tCalibTdcData->GetBoard() == uTdcNumber2 &&
               0 < tCalibTdcData->GetTime() )
         {
            iDataIndex2[tCalibTdcData->GetChannel()] = iArrayEntry;
            iDataMulti2[tCalibTdcData->GetChannel()] ++;
         }
      }

      for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
         for( Int_t iChB = 0; iChB < kiNbCh; iChB ++)
            if(iDataIndex1[iChA] != -1 && iDataIndex2[iChB] != -1  &&
                  1 == iDataMulti1[iChA] &&  1 == iDataMulti2[iChB] )
            {
               tHistogramFit[iChA][iChB]->Fill(
                       ((TTofCalibData*)tArray->At(iDataIndex1[iChA]))->GetTime()
                     - ((TTofCalibData*)tArray->At(iDataIndex2[iChB]))->GetTime() );
            } // if(iDataIndex1[iChA] != -1 && iDataIndex2[iChB] != -1 )
   } // for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)

   delete gROOT->FindObjectAny( "tCanvasBAD" );
   TCanvas* tCanvasBAD = new TCanvas("tCanvasBAD","singles zoom",0,0,700,700);
   tCanvasBAD->Divide(kiNbCh, kiNbCh);
   // Fit histo for each pair
   TF1 *fitFunc[kiNbCh][kiNbCh];
   for( Int_t iChA = 0; iChA < kiNbCh; iChA ++)
      for( Int_t iChB = 0; iChB < kiNbCh; iChB ++)
         if( 0 < tHistogramFit[iChA][iChB]->GetEntries() )
         {
            tCanvasBAD->cd( iChA * kiNbCh + 1 + iChB);
            tHistogramFit[iChA][iChB]->Draw();

            tHistogram_Entries->Fill(iChA, iChB, tHistogram[iChA][iChB]->GetEntries() );
            if( 0 == tHistogramFit[iChA][iChB]->Integral() )
            {
               cout<<"Check overflow and underflow for ch "<<iChA<<" vs "<<iChB<<endl;
               continue;
            } // if( 0 == tHistogram[iChA][iChB]->Integral() )
            fitFunc[iChA][iChB] = new TF1(Form("f_%02d_%02d",iChA,iChB), "gaus",
                  tHistogramFit[iChA][iChB]->GetMean() - 1*tHistogramFit[iChA][iChB]->GetRMS() ,
                  tHistogramFit[iChA][iChB]->GetMean() + 1*tHistogramFit[iChA][iChB]->GetRMS());

            tHistogramFit[iChA][iChB]->Fit( Form("f_%02d_%02d",iChA,iChB), "QR");

            tHistogram_Mean->Fill(iChA, iChB, fitFunc[iChA][iChB]->GetParameter(1) );
            tHistogram_Summary->Fill(iChA, iChB, fitFunc[iChA][iChB]->GetParameter(2) );
         } // if( 0 < tHistogram[iChA][iChB]->GetEntries() )

   tBranch->SetAddress(vOldAddress);

   tArray->Clear("C");
   tArray = NULL;

   delete gROOT->FindObjectAny( "tCanvas1" );
   TCanvas* tCanvas1 = new TCanvas("tCanvas1","Entries",0,0,700,700);
   tHistogram_Entries->SetStats( kFALSE );
   tHistogram_Entries->Draw("colz");

   delete gROOT->FindObjectAny( "tCanvas2" );
   TCanvas* tCanvas2 = new TCanvas("tCanvas2","Time resolution",0,0,700,700);
   tHistogram_Summary->SetStats( kFALSE );
   tHistogram_Summary->Draw("colz");

   delete gROOT->FindObjectAny( "tCanvas3" );
   TCanvas* tCanvas3 = new TCanvas("tCanvas3","Mean",0,0,700,700);
   tHistogram_Mean->SetStats( kFALSE );
   tHistogram_Mean->Draw("colz");

   tOldDirectory->cd();
}
