
Bool_t ratePlotHd()
{

   // Open the unpacked data file
   //TString sInputFilenameScal = "unpack.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbMon0153.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbMon0345.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbMon0558.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbMon0710.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbFr0230.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbWed0042.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbWed2350a.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbWed2350b.out.root";
   TString sInputFilenameScal = "../unpack_MbsTrbThu0036.out.root";
   TFile * fInputFileScal = TFile::Open( sInputFilenameScal, "READ");
   if(!fInputFileScal || kFALSE == fInputFileScal->IsOpen() )
   {
      cout<< Form("Input file %s cannot be opened.", sInputFilenameScal.Data())<<endl;
      return kFALSE;
   }

   // Recover the unpacked data tree
   TTree* tInputTreeScal = (TTree*)fInputFileScal->Get("cbmsim");
   if(!tInputTreeScal)
   {
      cout<<"No CbmSim tree inside the input file for scalers."<<endl;
      return kFALSE;
   }

   // Get number of events in Tree
   UInt_t uNTreeEntriesScal = tInputTreeScal->GetEntries();

   // Obtain pointers on the branches where the TRIGLOG and scaler data are stored
   TClonesArray* tArrayTriglog(NULL);
   TClonesArray* tArrayScal(NULL);

      // Branch pointer for Triglog
   TBranch* tBranchEventTriglog = tInputTreeScal->GetBranch( "TofTriglog" );
   if(!tBranchEventTriglog)
   {
      cout<<"No branch TofTriglog in input tree."<<endl;
      return kFALSE;
   } // if(!tBranchEventTriglog)
   tBranchEventTriglog->SetAddress(&tArrayTriglog);

      // Branch pointer for Scalers
   TBranch* tBranchEventScal = tInputTreeScal->GetBranch( "TofCalibScaler" );
   if(!tBranchEventScal)
   {
      cout<<"No branch TofCalibScaler in input tree."<<endl;
      return kFALSE;
   } // if(!tBranchEventScal)
   tBranchEventScal->SetAddress(&tArrayScal);

   // Switch back current directory to gROOT so that new object are created in memory, not in file
   gROOT->cd();

//   TTofCalibScaler* tEventScal(0);

   cout<<uNTreeEntriesScal<<" entries in scalers tree"<<endl;

   // Get time for MBS starting point
   Int_t iFirstGoodMbsEvent = 0;
   TTofTriglogBoard * fTriglogBoard;
   TTofCalibScaler  * fCalTrloBoard;
   // Find first MBS event with TRIGLOG
   for( iFirstGoodMbsEvent = 0; iFirstGoodMbsEvent < uNTreeEntriesScal; iFirstGoodMbsEvent ++)
   {
      tInputTreeScal->GetEntry(iFirstGoodMbsEvent);
      fTriglogBoard = (TTofTriglogBoard*) tArrayTriglog->ConstructedAt(0); // Always only 1 TRIGLOG board!
      if( 0 < fTriglogBoard->GetMbsTimeSec() )
         break;
   } // for( iFirstGoodMbsEvent = 0; iFirstGoodMbsEvent < uNTreeEntriesScal; iFirstGoodMbsEvent ++)
   tInputTreeScal->GetEntry(iFirstGoodMbsEvent);
   fTriglogBoard = (TTofTriglogBoard*) tArrayTriglog->ConstructedAt(0); // Always only 1 TRIGLOG board!
   fCalTrloBoard = (TTofCalibScaler*)  tArrayScal->ConstructedAt(0);    // TRIGLOG board always first!
 
      // Use TTimeStamp as an example, this give access to date and time in nice format
      // Direct use of the time is also possible!
   TTimeStamp tTimeFirstMbsEvent;
   tTimeFirstMbsEvent.SetSec( fTriglogBoard->GetMbsTimeSec() );
   tTimeFirstMbsEvent.SetNanoSec( fTriglogBoard->GetMbsTimeMilliSec() * 1000000 );
   Double_t dFirstMbsTime = (Double_t)(tTimeFirstMbsEvent.GetSec()) 
                          + (Double_t)(tTimeFirstMbsEvent.GetNanoSec())/1e9;

   // Get time for MBS ending point
   tInputTreeScal->GetEntry(uNTreeEntriesScal-1);
   fTriglogBoard = (TTofTriglogBoard*) tArrayTriglog->ConstructedAt(0); // Always only 1 TRIGLOG board!
   fCalTrloBoard = (TTofCalibScaler*)  tArrayScal->ConstructedAt(0);    // TRIGLOG board always first!
 
   TTimeStamp tTimeLastMbsEvent;
   tTimeLastMbsEvent.SetSec( fTriglogBoard->GetMbsTimeSec() );
   tTimeLastMbsEvent.SetNanoSec( fTriglogBoard->GetMbsTimeMilliSec() * 1000000 );
   Int_t iTimeIntervalSecClk = (Int_t)(fCalTrloBoard->GetTimeToFirst()) + 1;
   
   cout<<"Time first MBS event: "<<tTimeFirstMbsEvent.AsString()<<endl;
   cout<<"Time last MBS event:  "<<tTimeLastMbsEvent.AsString()<<endl;

   // Prepare histograms
   Int_t iTimeIntervalsec = tTimeLastMbsEvent.GetSec() - tTimeFirstMbsEvent.GetSec() + 1;
   cout<<"Time interval with data: "<<iTimeIntervalsec<<" s (MBS)"<<endl;
   cout<<"Time interval with data: "<<iTimeIntervalSecClk<<" s (clock)"<<endl;

   Int_t iBinSec = 10;


   TProfile * hRateEvoScalMbs1 = new TProfile( "hRateEvoScalMbs1", 
                              "Triglog input scaler 2 (Back HD PMT); MBS Time [s]; Rate [kHz/cm2]", 
                              iBinSec*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs7 = new TProfile( "hRateEvoScalMbs7", 
                              "Triglog input scaler 8 (HD. Ref. RPC); MBS Time [s]; Rate [kHz/cm2]", 
                              iBinSec*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs10 = new TProfile( "hRateEvoScalMbs10", 
                              "Triglog input scaler 11 (HD Big RPC); MBS Time [s]; Rate [kHz/cm2]", 
                              iBinSec*iTimeIntervalsec, 0, iTimeIntervalsec );

   TProfile * hRateEvoScalMbs1w = new TProfile( "hRateEvoScalMbs1w", 
                              "Triglog input scaler 2 (Back HD PMT); MBS Time [s]; Rate [kHz/cm2]", 
                              iBinSec*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs7w = new TProfile( "hRateEvoScalMbs7w", 
                              "Triglog input scaler 8 (HD. Ref. RPC); MBS Time [s]; Rate [kHz/cm2]", 
                              iBinSec*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs10w = new TProfile( "hRateEvoScalMbs10w", 
                              "Triglog input scaler 11 (HD Big RPC); MBS Time [s]; Rate [kHz/cm2]", 
                              iBinSec*iTimeIntervalsec, 0, iTimeIntervalsec );
 
 
  
   // To have a meaningfull plot using MBS time, we need the time in the first MBS event
   tInputTreeScal->GetEntry(0);
   Double_t dFirstMbsTime = (Double_t)(tTimeFirstMbsEvent.GetSec()) 
                          + (Double_t)(tTimeFirstMbsEvent.GetNanoSec())/1e9;


   // Loop over MBS events
   UInt_t uMbsEvtIndex = 0;
   Double_t dRateKhzCm1 = 0;
   Double_t dRateKhzCm7 = 0;
   Double_t dRateKhzCm10 = 0;

   for( uMbsEvtIndex = iFirstGoodMbsEvent; uMbsEvtIndex < uNTreeEntriesScal; uMbsEvtIndex ++)
   {
      // Get the event data
      tInputTreeScal->GetEntry( uMbsEvtIndex );

      // Get the TRIGLOG and SCALER object we want from the arrays
      fTriglogBoard = (TTofTriglogBoard*) tArrayTriglog->ConstructedAt(0); // Always only 1 TRIGLOG board!
      fCalTrloBoard = (TTofCalibScaler*)  tArrayScal->ConstructedAt(0);    // TRIGLOG board always first!

      // Calculate current MBS time (use directly the time, knowing the date is not so important here)
      Double_t dCurrMbsTime = (Double_t)(fTriglogBoard->GetMbsTimeSec()) 
                            + (Double_t)(fTriglogBoard->GetMbsTimeMilliSec())/1e3;

      // Calculate rate per cm2

            dRateKhzCm1   = fCalTrloBoard->GetScalerValue(1) / 1000.0 / 44; // back HD PMT (4cmx11cm)
            dRateKhzCm7   = fCalTrloBoard->GetScalerValue(7) / 1000.0 / 59.5; // Ref HD RPC (16*0.93 x 4cm)
            dRateKhzCm10   = fCalTrloBoard->GetScalerValue(10) / 1000.0 / 2756; //Big HD (53cm x 52cm)

//      if( 0 < fCalTrloBoard->GetTimeToLast() )
      if( 1e-5 < fCalTrloBoard->GetTimeToLast() ) // avoid too low reference clock counts
//      if( 5e-5 < fCalTrloBoard->GetTimeToLast() ) // avoid too low reference clock counts
      {
         // Scale rate with time since last event for averaging
         //hRateEvoScalMbs0->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm1, fCalTrloBoard->GetTimeToLast() );

         hRateEvoScalMbs1->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm1);
         hRateEvoScalMbs7->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm7);
         hRateEvoScalMbs10->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm10);

         hRateEvoScalMbs1w->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm1,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs7w->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm7,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs10w->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm10,fCalTrloBoard->GetTimeToLast());
       }
   } // for( uMbsEvtIndex = 0; uMbsEvtIndex < uNTreeEntriesScal; uMbsEvtIndex ++)


   // Now need to be displayed
   TCanvas* tCanvasA = new TCanvas("tCanvasA","Rate evolution: two case",0,0,2000,1000);
   tCanvasA->SetFillColor(0);
   tCanvasA->SetGridx(0);
   tCanvasA->SetGridy(0);
   tCanvasA->SetTopMargin(0);
   tCanvasA->SetRightMargin(0);
   tCanvasA->SetBottomMargin(0);
   tCanvasA->SetLeftMargin(0);
   tCanvasA->Divide(2,1);

   tCanvasA->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs1->SetLineColor(kBlack);
   hRateEvoScalMbs1->Draw("HIST");
   hRateEvoScalMbs7->SetLineColor(kBlue);
   hRateEvoScalMbs7->Draw("HIST,SAME");
   hRateEvoScalMbs10->SetLineColor(kRed);
   hRateEvoScalMbs10->Draw("HIST,SAME");

   hRateEvoScalMbs1->GetYaxis()->SetTitleOffset(1.5);

   leg1 = new TLegend(0.1,0.7,0.48,0.9);
   leg1->AddEntry(hRateEvoScalMbs1w,"PMT rate","l");
   leg1->AddEntry(hRateEvoScalMbs7w,"ref. RPC rate","l");
   leg1->AddEntry(hRateEvoScalMbs10w,"big RPC rate","l");
   leg1->Draw();


   tCanvasA->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs1w->SetLineColor(kBlack);
   hRateEvoScalMbs1w->Draw("HIST");
   hRateEvoScalMbs7w->SetLineColor(kBlue);
   hRateEvoScalMbs7w->Draw("HIST,SAME");
   hRateEvoScalMbs10w->SetLineColor(kRed);
   hRateEvoScalMbs10w->Draw("HIST,SAME");

   hRateEvoScalMbs1w->GetYaxis()->SetTitleOffset(1.5);

   leg1->Draw();


   fInputFileScal->Close();

   return kTRUE;
}
