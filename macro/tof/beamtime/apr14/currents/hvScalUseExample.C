
#include "StructDef.h"

//#include "$SIMPATH/unpack/tof/calib/scaler/TTofCalibScaler.h"

Bool_t hvScalUseExample( Int_t iFirstRPC = 0, Int_t iSecondRpc = 2, Int_t iScalerIndex = 3, Double_t dScalerArea = -1 )
{
   const Int_t   kiNbRpc         = 11;
   TString sChNameRpc[kiNbRpc]   = { "HD_Big", "HD_Sm", "HD_Ref", "USTC", "TS_PAD", "TS_Strip", "BUC_Ref", "BUC_2010", "BUC_2012", "BUC_2013", "TRD_BUC"}; 

   if( kiNbRpc <= iFirstRPC || kiNbRpc <= iSecondRpc )
   {
      cout<<" One of the RPC indices is out of bound, values should be between 0 and "<<kiNbRpc -1<<endl;
      return kFALSE;
   }
   if( -1 == dScalerArea && 3 == iScalerIndex)
      dScalerArea = 40; //cm^2, just a guess for HD PMT

   if( -1 == dScalerArea )
   {
      cout<<" Undefined scaler area for scaler "<<iScalerIndex<<endl;
      return kFALSE;
   }

   // HV slow control
   TString sInputFilenameHv = "LogHv_Full_GsiApr14.root";
//   TString sInputFilenameHv = "LogHv_200414_2249.root";
//   TString sInputFilenameHv = "LogHv_210414_0153.root";

   TFile * fInputFileHv = new TFile( sInputFilenameHv, "READ");
   if(!fInputFileHv || kFALSE == fInputFileHv->IsOpen() )
   {
      cout<< Form("Input file %s cannot be opened.", sInputFilenameHv.Data())<<endl;
      return kFALSE;
   }

   TTree* tInputTreeHv = (TTree*)fInputFileHv->Get("CaenHV");
   if(!tInputTreeHv)
   {
      cout<<"No Hv tree inside the input file for CAEN HV."<<endl;
      return kFALSE;
   }

   UInt_t uNTreeEntriesHv = tInputTreeHv->GetEntries();

   Rpc_HV tRpcHvEvtA;
   Rpc_HV tRpcHvEvtB;

   TBranch* tBranchEventRpcA = tInputTreeHv->GetBranch( sChNameRpc[iFirstRPC]  );
   if(!tBranchEventRpcA)
   {
      cout<<"No branch "<<sChNameRpc[iFirstRPC]<<" in input tree."<<endl;
      return kFALSE;
   } // if(!tBranchEventRpcA)
   tBranchEventRpcA->SetAddress(&(tRpcHvEvtA.iTimeSec));

   TBranch* tBranchEventRpcB = tInputTreeHv->GetBranch( sChNameRpc[iSecondRpc] );
   if(!tBranchEventRpcB)
   {
      cout<<"No branch "<<sChNameRpc[iSecondRpc]<<" in input tree."<<endl;
      return kFALSE;
   } // if(!tBranchEventRpcB)
   tBranchEventRpcB->SetAddress(&(tRpcHvEvtB.iTimeSec));

   // Scalers
   TString sInputFilenameScal = "unpack.out.root";
//   TString sInputFilenameScal = "MbsTrbSun2325_unpack.out.root";
//   TString sInputFilenameScal = "../unpack_MbsTrbMon0153.out.root";

   TFile * fInputFileScal = TFile::Open( sInputFilenameScal, "READ");
   if(!fInputFileScal || kFALSE == fInputFileScal->IsOpen() )
   {
      cout<< Form("Input file %s cannot be opened.", sInputFilenameScal.Data())<<endl;
      return kFALSE;
   }

   TTree* tInputTreeScal = (TTree*)fInputFileScal->Get("cbmsim");
   if(!tInputTreeScal)
   {
      cout<<"No CbmSim tree inside the input file for scalers."<<endl;
      return kFALSE;
   }

   UInt_t uNTreeEntriesScal = tInputTreeScal->GetEntries();

   TClonesArray* tArrayTriglog(NULL);
   TClonesArray* tArrayScal(NULL);

   TBranch* tBranchEventTriglog = tInputTreeScal->GetBranch( "TofTriglog" );
   if(!tBranchEventTriglog)
   {
      cout<<"No branch TofTriglog in input tree."<<endl;
      return kFALSE;
   } // if(!tBranchEventTriglog)
   tBranchEventTriglog->SetAddress(&tArrayTriglog);

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

   cout<<uNTreeEntriesHv<<" entries in HV tree and "<<uNTreeEntriesScal<<" entries in scalers tree"<<endl;

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
 
   TTimeStamp tTimeFirstMbsEvent;
   tTimeFirstMbsEvent.SetSec( fTriglogBoard->GetMbsTimeSec() );
   tTimeFirstMbsEvent.SetNanoSec( fTriglogBoard->GetMbsTimeMilliSec() * 1000000 );

   // Get time for MBS ending point
   tInputTreeScal->GetEntry(uNTreeEntriesScal-1);
   fTriglogBoard = (TTofTriglogBoard*) tArrayTriglog->ConstructedAt(0); // Always only 1 TRIGLOG board!
   fCalTrloBoard = (TTofCalibScaler*)  tArrayScal->ConstructedAt(0);    // TRIGLOG board always first!
 
   TTimeStamp tTimeLastMbsEvent;
   tTimeLastMbsEvent.SetSec( fTriglogBoard->GetMbsTimeSec() );
   tTimeLastMbsEvent.SetNanoSec( fTriglogBoard->GetMbsTimeMilliSec() * 1000000 );
   
   cout<<"Time first MBS event: "<<tTimeFirstMbsEvent.AsString()<<endl;
   cout<<"Time last MBS event:  "<<tTimeLastMbsEvent.AsString()<<endl;

   // Prepare histograms
   Int_t iTimeIntervalsec = tTimeLastMbsEvent.GetSec() - tTimeFirstMbsEvent.GetSec() + 1;
   cout<<"Time interval with data: "<<iTimeIntervalsec<<" s"<<endl;
   TH1 * hCurrentEvoNegA = new TH1D( "hCurrentEvoNegA", 
                              Form("Current evolution for the Negative HV of %s; Time [s]; Current [uA]", 
                                    sChNameRpc[iFirstRPC].Data()), 
                              2*iTimeIntervalsec, 0, iTimeIntervalsec );
   hCurrentEvoNegA->SetLineColor(kBlue);
   TH1 * hCurrentEvoPosA = new TH1D( "hCurrentEvoPosA", 
                              Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]", 
                                    sChNameRpc[iFirstRPC].Data()), 
                              2*iTimeIntervalsec, 0, iTimeIntervalsec );
   hCurrentEvoPosA->SetLineColor(kRed);

   TH1 * hCurrentEvoNegB = new TH1D( "hCurrentEvoNegB", 
                              Form("Current evolution for the Negative HV of %s; Time [s]; Current [uA]", 
                                    sChNameRpc[iSecondRpc].Data()), 
                              2*iTimeIntervalsec, 0, iTimeIntervalsec );
   hCurrentEvoNegB->SetLineColor(kBlue);
   TH1 * hCurrentEvoPosB = new TH1D( "hCurrentEvoPosB", 
                              Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]", 
                                    sChNameRpc[iSecondRpc].Data()), 
                              2*iTimeIntervalsec, 0, iTimeIntervalsec );
   hCurrentEvoPosB->SetLineColor(kRed);

   TH1 * hRateEvoScal = new TH1D( "hRateEvoScal", 
                              Form("Scaler rate evolution for the input scaler #%d in TRIGLOG; Time [s]; Rate [kHz/cm2]", 
                                    iScalerIndex), 
                              2*iTimeIntervalsec, 0, iTimeIntervalsec );
   hRateEvoScal->SetLineColor(kGreen);

/*
   TProfile * hCurrentEvoNegB = new TProfile( "hCurrentEvoNegB", 
                              Form("Current evolution for the Negative HV of %s; Time [s]; Current [uA]", 
                                    sChNameRpc[iSecondRpc].Data()), 
                              iTimeIntervalsec, 0, iTimeIntervalsec );
   hCurrentEvoNegB->SetLineColor(kBlue);
   TProfile * hCurrentEvoPosB = new TProfile( "hCurrentEvoPosB", 
                              Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]", 
                                    sChNameRpc[iSecondRpc].Data()), 
                              iTimeIntervalsec, 0, iTimeIntervalsec );
   hCurrentEvoPosB->SetLineColor(kRed);

   TProfile * hRateEvoScal = new TProfile( "hRateEvoScal", 
                              Form("Scaler rate evolution for the input scaler #%d in TRIGLOG; Time [s]; Rate [kHz/cm2]", 
                                    iScalerIndex), 
                              iTimeIntervalsec, 0, iTimeIntervalsec );
   hRateEvoScal->SetLineColor(kGreen);
*/
   TH2 * hRateCurrCompA = new TH2D( "hRateCurrCompA", 
                              Form("Comparison of scaler #%d rate and RPC %s current; Rate [kHz/cm2]; Total current [uA]", 
                                    iScalerIndex, sChNameRpc[iFirstRPC].Data()), 
                              1000, 0, 5,
                               300, 0,  3 );
   TH2 * hRateCurrCompB = new TH2D( "hRateCurrCompB", 
                              Form("Comparison of scaler #%d rate and RPC %s current; Rate [kHz/cm2]; Total current [uA]", 
                                    iScalerIndex, sChNameRpc[iSecondRpc].Data()), 
                              1000, 0, 5,
                               300, 0,  3 );

   // Loop until the HV point matching best the first MBS event is found
   Double_t dTimeDistMbsPrevA = -1e10;
   Double_t dTimeDistMbsPrevB = -1e10;
   Double_t dTimeDistMbsLastA = -1e10;
   Double_t dTimeDistMbsLastB = -1e10;
   UInt_t uHvStartPoint = 0;
   for( uHvStartPoint = 0; uHvStartPoint < uNTreeEntriesHv; uHvStartPoint++)
   {
      tInputTreeHv->GetEntry(uHvStartPoint);

      dTimeDistMbsPrevA = dTimeDistMbsLastA;
      dTimeDistMbsPrevB = dTimeDistMbsLastB;

      dTimeDistMbsLastA = (Double_t)tRpcHvEvtA.iTimeSec + (Double_t)tRpcHvEvtA.iTimeMilliSec/1000.0 
                         - (Double_t)(tTimeFirstMbsEvent.GetSec()) - (Double_t)(tTimeFirstMbsEvent.GetNanoSec())/1e9;

      dTimeDistMbsLastB = (Double_t)tRpcHvEvtB.iTimeSec + (Double_t)tRpcHvEvtB.iTimeMilliSec/1000.0 
                         - (Double_t)(tTimeFirstMbsEvent.GetSec()) - (Double_t)(tTimeFirstMbsEvent.GetNanoSec())/1e9;

      if( 0 < dTimeDistMbsLastA && 0 < dTimeDistMbsLastB )
         break;
   } // for( UInt_t uHvPoint = 0; uHvPoint < uNTreeEntriesHv; uHvPoint++)
   if( TMath::Abs( dTimeDistMbsPrevA ) < TMath::Abs( dTimeDistMbsLastA ) && 
       TMath::Abs( dTimeDistMbsPrevB ) < TMath::Abs( dTimeDistMbsLastB ) )
      uHvStartPoint --;

   cout<<" Best Match in time with first MBS event found for HV point #"<<uHvStartPoint
       <<" : DiffT_A_p = "<< dTimeDistMbsPrevA
       <<"   DiffT_B_p = "<< dTimeDistMbsPrevB
       <<" - DiffT_A_l = "<< dTimeDistMbsLastA
       <<"   DiffT_B_l = "<< dTimeDistMbsLastB
       <<endl;

   // Loop until the HV point matching best the last MBS event is found
   Double_t dTimeDistLastMbsPrevA = -1e10;
   Double_t dTimeDistLastMbsPrevB = -1e10;
   Double_t dTimeDistLastMbsLastA = -1e10;
   Double_t dTimeDistLastMbsLastB = -1e10;
   UInt_t uHvStopPoint = 0;
   for( uHvStopPoint = 0; uHvStopPoint < uNTreeEntriesHv; uHvStopPoint++)
   {
      tInputTreeHv->GetEntry(uHvStopPoint);

      dTimeDistLastMbsPrevA = dTimeDistLastMbsLastA;
      dTimeDistLastMbsPrevB = dTimeDistLastMbsLastB;

      dTimeDistLastMbsLastA = (Double_t)tRpcHvEvtA.iTimeSec + (Double_t)tRpcHvEvtA.iTimeMilliSec/1000.0 
                         - (Double_t)(tTimeLastMbsEvent.GetSec()) - (Double_t)(tTimeLastMbsEvent.GetNanoSec())/1e9;

      dTimeDistLastMbsLastB = (Double_t)tRpcHvEvtB.iTimeSec + (Double_t)tRpcHvEvtB.iTimeMilliSec/1000.0 
                         - (Double_t)(tTimeLastMbsEvent.GetSec()) - (Double_t)(tTimeLastMbsEvent.GetNanoSec())/1e9;

      if( 0 < dTimeDistLastMbsLastA && 0 < dTimeDistLastMbsLastB )
         break;
   } // for( UInt_t uHvPoint = 0; uHvPoint < uNTreeEntriesHv; uHvPoint++)
   if( TMath::Abs( dTimeDistLastMbsPrevA ) < TMath::Abs( dTimeDistLastMbsLastA ) && 
       TMath::Abs( dTimeDistLastMbsPrevB ) < TMath::Abs( dTimeDistLastMbsLastB ) )
      uHvStopPoint --;

   cout<<" Best Match in time with last MBS event found for HV point  #"<<uHvStopPoint
       <<" : DiffT_A_p = "<< dTimeDistLastMbsPrevA
       <<"   DiffT_B_p = "<< dTimeDistLastMbsPrevB
       <<" - DiffT_A_l = "<< dTimeDistLastMbsLastA
       <<"   DiffT_B_l = "<< dTimeDistLastMbsLastB
       <<endl;

   // Plotting the first HV point is easy: known it match the first MBS event
   tInputTreeScal->GetEntry(0);
   tInputTreeHv->GetEntry(uHvStartPoint);
   Double_t dFirstMbsTime = (Double_t)(tTimeFirstMbsEvent.GetSec()) 
                          + (Double_t)(tTimeFirstMbsEvent.GetNanoSec())/1e9;
   Double_t dRateKhzCm    = fCalTrloBoard->GetScalerValue(iScalerIndex) / 1000.0 / dScalerArea;
   hCurrentEvoNegA->Fill(0.0, tRpcHvEvtA.dCurrentNeg );
   hCurrentEvoPosA->Fill(0.0, tRpcHvEvtA.dCurrentPos );
   hCurrentEvoNegB->Fill(0.0, tRpcHvEvtB.dCurrentNeg );
   hCurrentEvoPosB->Fill(0.0, tRpcHvEvtB.dCurrentPos );
   hRateEvoScal->Fill(   0.0, dRateKhzCm );
   hRateCurrCompA->Fill( dRateKhzCm,
                         tRpcHvEvtA.dCurrentNeg + tRpcHvEvtA.dCurrentPos );
   hRateCurrCompB->Fill( dRateKhzCm,
                         tRpcHvEvtB.dCurrentNeg + tRpcHvEvtB.dCurrentPos );

   // Loop over HV points
   UInt_t uLastBestMbsMatch = iFirstGoodMbsEvent;
   UInt_t uMbsEvtIndex = iFirstGoodMbsEvent;
   TTimeStamp tTimeCurrMbsEvent;
   Double_t dTimeDistCurrMbsPrevA = -1e10;
   Double_t dTimeDistCurrMbsPrevB = -1e10;
   Double_t dTimeDistCurrMbsLastA = -1e10;
   Double_t dTimeDistCurrMbsLastB = -1e10;
   Int_t iNbHvPoint = 0;
   for( UInt_t uHvPoint = uHvStartPoint + 1; uHvPoint < uHvStopPoint; uHvPoint ++)
   {
      tInputTreeHv->GetEntry(uHvPoint);
      // Find for each the closest MBS event
      dTimeDistCurrMbsPrevA = -1e10;
      dTimeDistCurrMbsPrevB = -1e10;
      dTimeDistCurrMbsLastA = -1e10;
      dTimeDistCurrMbsLastB = -1e10;
      for( uMbsEvtIndex = uLastBestMbsMatch; uMbsEvtIndex < uNTreeEntriesScal; uMbsEvtIndex ++)
      {
         tInputTreeScal->GetEntry(uMbsEvtIndex);
         tTimeCurrMbsEvent.SetSec( fTriglogBoard->GetMbsTimeSec() );
         tTimeCurrMbsEvent.SetNanoSec( fTriglogBoard->GetMbsTimeMilliSec() * 1000000 );

         dTimeDistCurrMbsPrevA = dTimeDistCurrMbsLastA;
         dTimeDistCurrMbsPrevB = dTimeDistCurrMbsLastB;

         dTimeDistCurrMbsLastA = (Double_t)(tTimeCurrMbsEvent.GetSec()) + (Double_t)(tTimeCurrMbsEvent.GetNanoSec())/1e9
                               - (Double_t)tRpcHvEvtA.iTimeSec - (Double_t)tRpcHvEvtA.iTimeMilliSec/1000.0;

         dTimeDistCurrMbsLastB = (Double_t)(tTimeCurrMbsEvent.GetSec()) + (Double_t)(tTimeCurrMbsEvent.GetNanoSec())/1e9
                               - (Double_t)tRpcHvEvtB.iTimeSec - (Double_t)tRpcHvEvtB.iTimeMilliSec/1000.0;
         if( 0 < dTimeDistCurrMbsLastA && 0 < dTimeDistCurrMbsLastB )
            break;
      } // for( uMbsEvtIndex = uLastBestMbsMatch; uMbsEvtIndex < uNTreeEntriesScal; uMbsEvtIndex ++)
      uLastBestMbsMatch = uMbsEvtIndex;
      if( TMath::Abs( dTimeDistCurrMbsPrevA ) < TMath::Abs( dTimeDistCurrMbsLastA ) && 
          TMath::Abs( dTimeDistCurrMbsPrevB ) < TMath::Abs( dTimeDistCurrMbsLastB ) )
         uLastBestMbsMatch --;

      // Plot current & rate vs time and current vs rate
      tInputTreeScal->GetEntry( uLastBestMbsMatch );
      tTimeCurrMbsEvent.SetSec( fTriglogBoard->GetMbsTimeSec() );
      tTimeCurrMbsEvent.SetNanoSec( fTriglogBoard->GetMbsTimeMilliSec() * 1000000 );
      Double_t dCurrMbsTime = (Double_t)(tTimeCurrMbsEvent.GetSec()) 
                            + (Double_t)(tTimeCurrMbsEvent.GetNanoSec())/1e9;
      dRateKhzCm    = fCalTrloBoard->GetScalerValue(iScalerIndex) / 1000.0 / dScalerArea;
      hCurrentEvoNegA->Fill(dCurrMbsTime - dFirstMbsTime, tRpcHvEvtA.dCurrentNeg );
      hCurrentEvoPosA->Fill(dCurrMbsTime - dFirstMbsTime, tRpcHvEvtA.dCurrentPos );
      hCurrentEvoNegB->Fill(dCurrMbsTime - dFirstMbsTime, tRpcHvEvtB.dCurrentNeg );
      hCurrentEvoPosB->Fill(dCurrMbsTime - dFirstMbsTime, tRpcHvEvtB.dCurrentPos );
      hRateEvoScal->Fill(   dCurrMbsTime - dFirstMbsTime, dRateKhzCm );
      hRateCurrCompA->Fill( dRateKhzCm,
                            tRpcHvEvtA.dCurrentNeg + tRpcHvEvtA.dCurrentPos );
      hRateCurrCompB->Fill( dRateKhzCm,
                            tRpcHvEvtB.dCurrentNeg + tRpcHvEvtB.dCurrentPos );

/*
      cout<<"MBS Time HV point #"<<iNbHvPoint<<":  "<<tTimeCurrMbsEvent.AsString()<<endl;
      cout<<" Best Match in time with HV point found for MBS event "<<uLastBestMbsMatch
          <<" : DiffT_A_p = "<< dTimeDistCurrMbsPrevA
          <<"   DiffT_B_p = "<< dTimeDistCurrMbsPrevB
          <<" - DiffT_A_l = "<< dTimeDistCurrMbsLastA
          <<"   DiffT_B_l = "<< dTimeDistCurrMbsLastB
          <<endl;
*/
      iNbHvPoint++;
   } // for( UInt_t uHvPoint = uHvStartPoint + 1; uHvPoint < uHvStopPoint; uHvPoint ++)

   // Plotting the last HV point is easy: known it match the last MBS event
   tInputTreeScal->GetEntry(uNTreeEntriesScal-1);
   tInputTreeHv->GetEntry(uHvStopPoint);
   Double_t dLastMbsTime = (Double_t)(tTimeLastMbsEvent.GetSec()) 
                         + (Double_t)(tTimeLastMbsEvent.GetNanoSec())/1e9;
   dRateKhzCm    = fCalTrloBoard->GetScalerValue(iScalerIndex) / 1000.0 / dScalerArea;
   hCurrentEvoNegA->Fill(dLastMbsTime - dFirstMbsTime, tRpcHvEvtA.dCurrentNeg );
   hCurrentEvoPosA->Fill(dLastMbsTime - dFirstMbsTime, tRpcHvEvtA.dCurrentPos );
   hCurrentEvoNegB->Fill(dLastMbsTime - dFirstMbsTime, tRpcHvEvtB.dCurrentNeg );
   hCurrentEvoPosB->Fill(dLastMbsTime - dFirstMbsTime, tRpcHvEvtB.dCurrentPos );
   hRateEvoScal->Fill(   dLastMbsTime - dFirstMbsTime, dRateKhzCm );
   hRateCurrCompA->Fill( dRateKhzCm,
                         tRpcHvEvtA.dCurrentNeg + tRpcHvEvtA.dCurrentPos );
   hRateCurrCompB->Fill( dRateKhzCm,
                         tRpcHvEvtB.dCurrentNeg + tRpcHvEvtB.dCurrentPos );

/*
   delete tInputTreeScal;
   delete tInputTreeHv;
*/
   // Check is histos are filled
   cout<<"First RPC histos:  "<<hCurrentEvoNegA->GetEntries()<<" "
       <<hCurrentEvoPosA->GetEntries()<<endl;
   cout<<"Second RPC histos: "<<hCurrentEvoNegB->GetEntries()<<" "
       <<hCurrentEvoPosB->GetEntries()<<endl;
   cout<<"Rate histos:       "<<hRateEvoScal->GetEntries()<<endl;
   cout<<"Comparison histos: "<<hRateCurrCompA->GetEntries()<<" "
       <<hRateCurrCompB->GetEntries()<<endl;

   // Now need to be displayed
   TCanvas* tCanvasA = new TCanvas("tCanvasA","Currents and rate evolution",0,0,2000,1000);
   tCanvasA->SetFillColor(0);
   tCanvasA->SetGridx(0);
   tCanvasA->SetGridy(0);
   tCanvasA->SetTopMargin(0);
   tCanvasA->SetRightMargin(0);
   tCanvasA->SetBottomMargin(0);
   tCanvasA->SetLeftMargin(0);
   tCanvasA->Divide(2,2,0,0);

   tCanvasA->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScal->Draw();
   hCurrentEvoPosA->Draw("SAME");
   hCurrentEvoNegA->Draw("SAME");

   tCanvasA->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScal->Draw();
   hCurrentEvoPosB->Draw("SAME");
   hCurrentEvoNegB->Draw("SAME");

   tCanvasA->cd(3);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScal->Draw();

   tCanvasA->cd(4);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateEvoScal->Draw();

   TCanvas* tCanvasB = new TCanvas("tCanvasB","Currents VS rate",0,0,2000,1000);
   tCanvasB->SetFillColor(0);
   tCanvasB->SetGridx(0);
   tCanvasB->SetGridy(0);
   tCanvasB->SetTopMargin(0);
   tCanvasB->SetRightMargin(0);
   tCanvasB->SetBottomMargin(0);
   tCanvasB->SetLeftMargin(0);
   tCanvasB->Divide(2,1,0,0);

   tCanvasB->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateCurrCompA->Draw("COLZ");

   tCanvasB->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hRateCurrCompB->Draw("COLZ");

   fInputFileHv->Close();
   fInputFileScal->Close();

   return kTRUE;
}
