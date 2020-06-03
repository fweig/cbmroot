
Bool_t ratePlotExample( Int_t iScalerIndex = 3, Double_t dScalerArea = -1 )
{
   if( -1 == dScalerArea && 3 == iScalerIndex)
      dScalerArea = 40; //cm^2, just a guess for HD PMT

   if( -1 == dScalerArea )
   {
      cout<<" Undefined scaler area for scaler "<<iScalerIndex<<endl;
      return kFALSE;
   }


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

   TProfile * hRateEvoScalMbs = new TProfile( "hRateEvoScalMbs", 
                              Form("Scaler rate evolution for the input scaler #%d in TRIGLOG; MBS Time [s]; Rate [kHz/cm2]", 
                                    iScalerIndex), 
                              10*iTimeIntervalsec, 0, iTimeIntervalsec );
   hRateEvoScalMbs->SetLineColor(kBlue);

   TProfile * hRateEvoScalClk = new TProfile( "hRateEvoScalClk", 
                              Form("Scaler rate evolution for the input scaler #%d in TRIGLOG; Clock Time [s]; Rate [kHz/cm2]", 
                                    iScalerIndex), 
                              10*iTimeIntervalSecClk, 0, iTimeIntervalSecClk );
   hRateEvoScalClk->SetLineColor(kRed);

   // To have a meaningfull plot using MBS time, we need the time in the first MBS event
   tInputTreeScal->GetEntry(0);
   Double_t dFirstMbsTime = (Double_t)(tTimeFirstMbsEvent.GetSec()) 
                          + (Double_t)(tTimeFirstMbsEvent.GetNanoSec())/1e9;


   // Loop over MBS events
   UInt_t uMbsEvtIndex = 0;
   Double_t dRateKhzCm = 0;
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
      dRateKhzCm    = fCalTrloBoard->GetScalerValue(iScalerIndex) / 1000.0 / dScalerArea;

      if( 0 < fCalTrloBoard->GetTimeToLast() )
      {
         // Scale rate with time since last event for averaging
         //hRateEvoScalMbs->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm, fCalTrloBoard->GetTimeToLast() );
         //hRateEvoScalClk->Fill(   fCalTrloBoard->GetTimeToFirst(), dRateKhzCm, fCalTrloBoard->GetTimeToLast() );

         hRateEvoScalMbs->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm);
         hRateEvoScalClk->Fill(   fCalTrloBoard->GetTimeToFirst(), dRateKhzCm);
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
   hRateEvoScalMbs->Draw("");

   tCanvasA->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalClk->Draw("");

   TCanvas* tCanvasB = new TCanvas("tCanvasB","Rate evolution: comparison",0,0,2000,1000);
   tCanvasB->SetFillColor(0);
   tCanvasB->SetGridx(0);
   tCanvasB->SetGridy(0);
   tCanvasB->SetTopMargin(0);
   tCanvasB->SetRightMargin(0);
   tCanvasB->SetBottomMargin(0);
   tCanvasB->SetLeftMargin(0);

   tCanvasB->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScalMbs->Draw("");
   hRateEvoScalClk->Draw("SAME");

   fInputFileScal->Close();

   return kTRUE;
}
