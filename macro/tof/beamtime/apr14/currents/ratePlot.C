
Bool_t ratePlot()
{

   // Open the unpacked data file
   //TString sInputFilenameScal = "unpack.out.root";
   TString sInputFilenameScal = "../unpack_MbsTrbMon0153.out.root";
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

   TProfile * hRateEvoScalMbs0 = new TProfile( "hRateEvoScalMbs0", 
                              "Triglog input scaler 1 (Front HD PMT); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs1 = new TProfile( "hRateEvoScalMbs1", 
                              "Triglog input scaler 2 (Back HD PMT); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs2 = new TProfile( "hRateEvoScalMbs2", 
                              "Triglog input scaler 3 (Front Buc. PMT); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs3 = new TProfile( "hRateEvoScalMbs3", 
                              "Triglog input scaler 4 (Back Buc. PMT); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs4 = new TProfile( "hRateEvoScalMbs4", 
                              "Triglog input scaler 5 (TRD Buc. PMT); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs5 = new TProfile( "hRateEvoScalMbs5", 
                              "Triglog input scaler 6 (Diamond); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs6 = new TProfile( "hRateEvoScalMbs6", 
                              "Triglog input scaler 7 (Buc. Ref. RPC); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs7 = new TProfile( "hRateEvoScalMbs7", 
                              "Triglog input scaler 8 (HD. Ref. RPC); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs8 = new TProfile( "hRateEvoScalMbs8", 
                              "Triglog input scaler 9 (Buc. RPC2012); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs9 = new TProfile( "hRateEvoScalMbs9", 
                              "Triglog input scaler 10 (Buc. RPC2013); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs10 = new TProfile( "hRateEvoScalMbs10", 
                              "Triglog input scaler 11 (HD Big RPC); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs11 = new TProfile( "hRateEvoScalMbs11", 
                              "Triglog input scaler 12 (PAD RPC); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs12 = new TProfile( "hRateEvoScalMbs12", 
                              "Triglog input scaler 13 (USTC + Ts. Strip RPC); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs13 = new TProfile( "hRateEvoScalMbs13", 
                              "Triglog input scaler 14 (empty); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs14 = new TProfile( "hRateEvoScalMbs14", 
                              "Triglog input scaler 15 (empty); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   TProfile * hRateEvoScalMbs15 = new TProfile( "hRateEvoScalMbs15", 
                              "Triglog input scaler 16 (scaler); MBS Time [s]; Rate [kHz/cm2]", 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
 
 
  
   // To have a meaningfull plot using MBS time, we need the time in the first MBS event
   tInputTreeScal->GetEntry(0);
   Double_t dFirstMbsTime = (Double_t)(tTimeFirstMbsEvent.GetSec()) 
                          + (Double_t)(tTimeFirstMbsEvent.GetNanoSec())/1e9;


   // Loop over MBS events
   UInt_t uMbsEvtIndex = 0;
   Double_t dRateKhzCm0 = 0;
   Double_t dRateKhzCm1 = 0;
   Double_t dRateKhzCm2 = 0;
   Double_t dRateKhzCm3 = 0;
   Double_t dRateKhzCm4 = 0;
   Double_t dRateKhzCm5 = 0;
   Double_t dRateKhzCm6 = 0;
   Double_t dRateKhzCm7 = 0;
   Double_t dRateKhzCm8 = 0;
   Double_t dRateKhzCm9 = 0;
   Double_t dRateKhzCm10 = 0;
   Double_t dRateKhzCm11 = 0;
   Double_t dRateKhzCm12 = 0;
   Double_t dRateKhzCm13 = 0;
   Double_t dRateKhzCm14 = 0;
   Double_t dRateKhzCm15 = 0;

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

            dRateKhzCm0   = fCalTrloBoard->GetScalerValue(0) / 1000.0 / 16; // front HD PMT (2cmx8cm)
            dRateKhzCm1   = fCalTrloBoard->GetScalerValue(1) / 1000.0 / 44; // back HD PMT (4cmx11cm)
            dRateKhzCm2   = fCalTrloBoard->GetScalerValue(2) / 1000.0 / 15; // front Buc PMT (1.5cmx10cm)
            dRateKhzCm3   = fCalTrloBoard->GetScalerValue(3) / 1000.0 / 15; // back Buc PMT (1.5cmx10cm)
            dRateKhzCm4   = fCalTrloBoard->GetScalerValue(4) / 1000.0 / 1; // TRD Buc PMT area unknown ??????
            dRateKhzCm5   = fCalTrloBoard->GetScalerValue(5) / 1000.0 / 1; // Diamant 
            dRateKhzCm6   = fCalTrloBoard->GetScalerValue(6) / 1000.0 / 731.5; // Ref Buc RPC (72*0.254cm x 4cm)
            dRateKhzCm7   = fCalTrloBoard->GetScalerValue(7) / 1000.0 / 59.5; // Ref HD RPC (16*0.93 x 4cm)
            dRateKhzCm8   = fCalTrloBoard->GetScalerValue(8) / 1000.0 / 1; //Buc RPC2012 (??????)
            dRateKhzCm9   = fCalTrloBoard->GetScalerValue(9) / 1000.0 / 1; //Buc RPC2013 (??????)
            dRateKhzCm10   = fCalTrloBoard->GetScalerValue(10) / 1000.0 / 2756; //Big HD (53cm x 52cm)
            dRateKhzCm11   = fCalTrloBoard->GetScalerValue(11) / 1000.0 / 1; // Pad RPC (??????)
            dRateKhzCm12   = fCalTrloBoard->GetScalerValue(12) / 1000.0 / 1; // Strip  (??????)
            dRateKhzCm13   = fCalTrloBoard->GetScalerValue(13) / 1000.0 / 1; // empty
            dRateKhzCm14   = fCalTrloBoard->GetScalerValue(14) / 1000.0 / 1; // empty
            dRateKhzCm15   = fCalTrloBoard->GetScalerValue(15) / 1000.0 / 1; // pulser

//      if( 0 < fCalTrloBoard->GetTimeToLast() )
      if( 1e-5 < fCalTrloBoard->GetTimeToLast() ) // avoid too low reference clock counts
//      if( 5e-5 < fCalTrloBoard->GetTimeToLast() ) // avoid too low reference clock counts
      {
         // Scale rate with time since last event for averaging
         //hRateEvoScalMbs0->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm1, fCalTrloBoard->GetTimeToLast() );

         hRateEvoScalMbs0->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm0,fCalTrloBoard->GetTimeToLast()) ;
         hRateEvoScalMbs1->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm1,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs2->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm2,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs3->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm3,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs4->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm4,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs5->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm5,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs6->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm6,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs7->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm7,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs8->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm8,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs9->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm9,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs10->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm10,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs11->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm11,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs12->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm12,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs13->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm13,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs14->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm14,fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbs15->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm15,fCalTrloBoard->GetTimeToLast());
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
   tCanvasA->Divide(4,4);

   tCanvasA->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs0->Draw("HIST");

   tCanvasA->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs1->Draw("HIST");

   tCanvasA->cd(3);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs2->Draw("HIST");

   tCanvasA->cd(4);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs3->Draw("HIST");

   tCanvasA->cd(5);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs4->Draw("HIST");

   tCanvasA->cd(6);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs5->Draw("HIST");

   tCanvasA->cd(7);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs6->Draw("HIST");

   tCanvasA->cd(8);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs7->Draw("HIST");

   tCanvasA->cd(9);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs8->Draw("HIST");

   tCanvasA->cd(10);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs9->Draw("HIST");

   tCanvasA->cd(11);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs10->Draw("HIST");

   tCanvasA->cd(12);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs11->Draw("HIST");

   tCanvasA->cd(13);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs12->Draw("HIST");

   tCanvasA->cd(14);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs13->Draw("HIST");

   tCanvasA->cd(15);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs14->Draw("HIST");

   tCanvasA->cd(16);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs15->Draw("HIST");


   fInputFileScal->Close();

   return kTRUE;
}
