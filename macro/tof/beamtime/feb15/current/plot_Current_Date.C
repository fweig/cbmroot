
#include "StructDef.h"
#include "FileListDef.h"

// Overloaded functions used to get a user friendly date/time input as string
void plot_Current_Date(TString sInputName = "LogHv_Full_CernFeb15.root",
               TString sStartDate = "2015-02-24 09:56:00",
               TString sStopDate  = "2015-03-05 07:53:00",
               Int_t iNbSecPerBin = 1,
               Int_t iSpillDistSec= kiSpillDistSec,
               Int_t iFirstDet = 0, Int_t iSecondDet = 1,
               Int_t iRefDet = 30
               )
{
   // Use ROOT TDatime class for its nice string to date function
   TDatime dateStart(sStartDate);
   TDatime dateStop( sStopDate);
   cout<<dateStart.GetDate()<<" "<<dateStart.GetTime()<<endl;
   cout<<dateStop.GetDate() <<" "<<dateStop.GetTime() <<endl;

   // Convert the TDatime (rel. to 01/01/95) to UNIX-like TTimeStamp
   // TIP: need to explicitly add a 0 for the nanoseconds to be sure the right
   //      ctor is used!
   TTimeStamp timeStart( (time_t)( dateStart.Convert( kFALSE ) ), 0 );
   TTimeStamp timeStop(  (time_t)( dateStop.Convert(  kFALSE ) ), 0 );

   UInt_t uDay, uMonth, uYear;
   UInt_t uHour, uMin, uSec;
   timeStart.GetDate( kFALSE, 0, &uDay, &uMonth, &uYear);
   timeStart.GetTime( kFALSE, 0, &uHour, &uMin, &uSec);
   cout<<timeStart.GetSec()<<" "
       <<uDay<<"-"<<uMonth<<"-"<<uYear<<" "
       <<uHour<<":"<<uMin<<":"<<uSec<<endl;
   timeStop.GetDate( kFALSE, 0, &uDay, &uMonth, &uYear);
   timeStop.GetTime( kFALSE, 0, &uHour, &uMin, &uSec);
   cout<<timeStop.GetSec()<<" "
       <<uDay<<"-"<<uMonth<<"-"<<uYear<<" "
       <<uHour<<":"<<uMin<<":"<<uSec<<endl;

   // Set time offset for histograms to 0 => Start on 01/01/1970 as we use
   // are the UNIX time frame!
   gStyle->SetTimeOffset(0);

   // Do real call to function
   plot_Current_Date_B( sInputName, timeStart, timeStop, iNbSecPerBin, iSpillDistSec, iFirstDet, iSecondDet, iRefDet );
}

Bool_t plot_Current_Date_B( TString sInputName = "LogHv_Full_CernFeb15.root",
                           TTimeStamp tStartTime = 1424768160, // Start of rate data taking
                           TTimeStamp tStopTime = 1425538380, // End of rate data taking
                           Int_t iNbSecPerBin = 1,
                           Int_t iSpillDistSec= kiSpillDistSec,
                           Int_t iFirstDet = 0, Int_t iSecondDet = 2,
                           Int_t iRefDet = 30
                           )
{
   // For now this macro works only for "RPC"-like detectors
   if( ( !( 0 <= iFirstDet              && iFirstDet < kiNbRpc) &&
         !( 0 <= iFirstDet-kiPmtOffset  && iFirstDet-kiPmtOffset < kiNbPmt) &&
         !( 0 <= iFirstDet-kiTrdOffset  && iFirstDet-kiTrdOffset < kiNbTrd) ) ||
       ( !( 0 <= iSecondDet             && iSecondDet < kiNbRpc) &&
         !( 0 <= iSecondDet-kiPmtOffset && iSecondDet-kiPmtOffset < kiNbPmt) &&
         !( 0 <= iSecondDet-kiTrdOffset && iSecondDet-kiTrdOffset < kiNbTrd) )
      )
   {
      cout<<" One of the Detector indices is out of bound"<<endl;
      PrintHelp();
      return kFALSE;
   }
   // For now this macro works only for "RPC"-like detectors
   if( !( 0 <= iRefDet-kiPmtOffset && iRefDet-kiPmtOffset < kiNbPmt) )
   {
      cout<<" Reference Detector index is out of bound (should be less than number of PMT detectors = "
            <<kiNbPmt<<" )"
            <<endl;
      PrintHelp();
      return kFALSE;
   }
      else iRefDet -= kiPmtOffset;

   Bool_t bFirstIsRpc = kFALSE;
   Bool_t bFirstIsPmt = kFALSE;
   TString sNameFirstDet = "";
   if( 0 <= iFirstDet && iFirstDet < kiNbRpc )
   {
      sNameFirstDet = sChNameRpc[iFirstDet];
      bFirstIsRpc = kTRUE;
   } // First detector is an RPC
   else if( 0 <= iFirstDet-kiPmtOffset  && iFirstDet-kiPmtOffset < kiNbPmt)
   {
      iFirstDet -= kiPmtOffset;
      sNameFirstDet = sChNamePmt[iFirstDet];
      bFirstIsPmt = kTRUE;
   } // First detector is a PMT
   else if( 0 <= iFirstDet-kiTrdOffset  && iFirstDet-kiTrdOffset < kiNbTrd )
   {
      iFirstDet -= kiTrdOffset;
      sNameFirstDet = sChNameTrd[iFirstDet];
   } // First detector is a TRD

   Bool_t bSecondIsRpc = kFALSE;
   Bool_t bSecondIsPmt = kFALSE;
   TString sNameSecondDet = "";
   if( 0 <= iSecondDet && iSecondDet < kiNbRpc )
   {
      sNameSecondDet = sChNameRpc[iSecondDet];
      bSecondIsRpc = kTRUE;
   } // First detector is an RPC
   else if( 0 <= iSecondDet-kiPmtOffset  && iSecondDet-kiPmtOffset < kiNbPmt)
   {
      iSecondDet -= kiPmtOffset;
      sNameSecondDet = sChNamePmt[iSecondDet];
      bSecondIsPmt = kTRUE;
   } // First detector is a PMT
   else if( 0 <= iSecondDet-kiTrdOffset  && iSecondDet-kiTrdOffset < kiNbTrd )
   {
      iSecondDet -= kiTrdOffset;
      sNameSecondDet = sChNameTrd[iSecondDet];
   } // First detector is a TRD

   // HV slow control
   if( "" == sInputName )
   {
      cout<<"Empty input filename!!!"<<endl;
      return;
   }

   TFile * fInputFileHv = new TFile( sInputName, "READ");
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


   TBranch* tBranchEventRpc[kiNbRpc];
   Rpc_HV tRpcHvEvt[kiNbRpc];
   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
   {
      tBranchEventRpc[iRpcIndex] = NULL;
      tBranchEventRpc[iRpcIndex] = tInputTreeHv->GetBranch( sChNameRpc[iRpcIndex]  );
      if(!tBranchEventRpc[iRpcIndex])
      {
         cout<<"No branch "<<sChNameRpc[iRpcIndex]<<" in input tree."<<endl;
         return kFALSE;
      } // if(!tBranchEventRpc)
      tBranchEventRpc[iRpcIndex]->SetAddress(&(tRpcHvEvt[iRpcIndex].iTimeSec));
   } // for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)

   TBranch* tBranchEventPmt[kiNbPmt];
   Pmt_HV tPmtHvEvt[kiNbPmt];
   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
   {
      tBranchEventPmt[iPmtIndex] = NULL;
      tBranchEventPmt[iPmtIndex] = tInputTreeHv->GetBranch( sChNamePmt[iPmtIndex]  );
      if(!tBranchEventPmt[iPmtIndex])
      {
         cout<<"No branch "<<sChNamePmt[iPmtIndex]<<" in input tree."<<endl;
         return kFALSE;
      } // if(!tBranchEventPmt)
      tBranchEventPmt[iPmtIndex]->SetAddress(&(tPmtHvEvt[iPmtIndex].iTimeSec));
   } // for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)

   TBranch* tBranchEventTrd[kiNbTrd];
   Rpc_HV tTrdHvEvt[kiNbTrd];
   for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
   {
      tBranchEventTrd[iTrdIndex] = NULL;
      tBranchEventTrd[iTrdIndex] = tInputTreeHv->GetBranch( sChNameTrd[iTrdIndex]  );
      if(!tBranchEventTrd[iTrdIndex])
      {
         cout<<"No branch "<<sChNameTrd[iTrdIndex]<<" in input tree."<<endl;
         return kFALSE;
      } // if(!tBranchEventPmt)
      tBranchEventTrd[iTrdIndex]->SetAddress(&(tPmtHvEvt[iTrdIndex].iTimeSec));
   } // for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)

/*
   Rpc_HV tDetHvEvtA;
   Rpc_HV tDetHvEvtB;
   Pmt_HV tDetHvEvtDiam;

   TBranch* tBranchEventDetA = NULL;
   tBranchEventDetA = tInputTreeHv->GetBranch( sNameFirstDet  );
   if(!tBranchEventDetA)
   {
      cout<<"No branch "<<sNameFirstDet<<" in input tree."<<endl;
      return kFALSE;
   } // if(!tBranchEventDetA)
   tBranchEventDetA->SetAddress(&(tDetHvEvtA.iTimeSec));

   TBranch* tBranchEventDetB = NULL;
   tBranchEventDetB = tInputTreeHv->GetBranch( sNameSecondDet  );
   if(!tBranchEventDetB)
   {
      cout<<"No branch "<<sNameSecondDet<<" in input tree."<<endl;
      return kFALSE;
   } // if(!tBranchEventDetB)
   tBranchEventDetB->SetAddress(&(tDetHvEvtB.iTimeSec));

   TBranch* tBranchEventDetDiam = NULL;
//   tBranchEventDetDiam = tInputTreeHv->GetBranch( "calo"  );
//   tBranchEventDetDiam = tInputTreeHv->GetBranch( "BFL"  );
//   tBranchEventDetDiam = tInputTreeHv->GetBranch( "PMT5"  );
   tBranchEventDetDiam = tInputTreeHv->GetBranch( "Dia_PMT"  );
   if(!tBranchEventDetDiam)
   {
      cout<<"No branch Dia_PMT in input tree."<<endl;
      return kFALSE;
   } // if(!tBranchEventDetB)
   tBranchEventDetDiam->SetAddress(&(tDetHvEvtDiam.iTimeSec));
*/
   // Switch back current directory to gROOT so that new object are created in memory, not in file
   gROOT->cd();

//   TTofCalibScaler* tEventScal(0);

   cout<<uNTreeEntriesHv<<" entries in HV tree "<<endl;

   // Prepare histograms and variables
   Int_t iNbBins       = (Int_t)( tStopTime.GetSec() - tStartTime.GetSec() ) / iNbSecPerBin ;
   Double_t dStartTime = tStartTime.GetSec();
   Double_t dStopTime  = tStopTime.GetSec();
   cout<<"Nb Bins: "<<iNbBins<<", "<< dStartTime<<", "<<dStopTime<<endl;
   Int_t iTimeIntervalsec = dStopTime - dStartTime;
   cout<<"Time interval: "<<iTimeIntervalsec<<" s"<<endl;

   // Full time interval histograms
   TProfile * hCurrentEvoNegA = new TProfile( "hCurrentEvoNegA",
                              Form("Current evolution for the Negative HV of %s; Time [s]; Current [uA]",
                                    sNameFirstDet.Data()),
                                    iNbBins, dStartTime, dStopTime );
   hCurrentEvoNegA->SetLineColor(kBlue);
   hCurrentEvoNegA->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoNegA->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile * hCurrentEvoPosA = new TProfile( "hCurrentEvoPosA",
                              Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]",
                                    sNameFirstDet.Data()),
                                    iNbBins, dStartTime, dStopTime );
   hCurrentEvoPosA->SetLineColor(kRed);
   hCurrentEvoPosA->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoPosA->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");

   TProfile * hCurrentEvoNegB = new TProfile( "hCurrentEvoNegB",
                              Form("Current evolution for the Negative HV of %s; Time [s]; Current [uA]",
                                    sNameSecondDet.Data()),
                                    iNbBins, dStartTime, dStopTime );
   hCurrentEvoNegB->SetLineColor(kBlue);
   hCurrentEvoNegB->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoNegB->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile * hCurrentEvoPosB = new TProfile( "hCurrentEvoPosB",
                              Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]",
                                    sNameSecondDet.Data()),
                                    iNbBins, dStartTime, dStopTime );
   hCurrentEvoPosB->SetLineColor(kRed);
   hCurrentEvoPosB->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoPosB->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");

   // Full time interval histograms for diamonds
   TProfile * hCurrentEvoDiam = new TProfile( "hCurrentEvoDiam",
                              Form("Current evolution for the HV of %s; Time [s]; Current [uA]",
                                    sChNamePmt[iRefDet].Data()),
                                    iNbBins, dStartTime, dStopTime );
   hCurrentEvoDiam->SetLineColor(kBlue);
   hCurrentEvoDiam->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoDiam->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile * hVoltageEvoDiam = new TProfile( "hVoltageEvoDiam",
                              Form("Voltage evolution for the HV of %s; Time [s]; Voltage [V]",
                                    sChNamePmt[iRefDet].Data()),
                                    iNbBins, dStartTime, dStopTime );
   hVoltageEvoDiam->SetLineColor(kBlue);
   hVoltageEvoDiam->GetXaxis()->SetTimeDisplay(1);
   hVoltageEvoDiam->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");

   // In spill histograms
   Int_t iNbBinsSpill   = (Int_t)(tStopTime.GetSec() - tStartTime.GetSec()) /iSpillDistSec;
   TProfile * hCurrentEvoSpillNegA = new TProfile( "hCurrentEvoSpillNegA",
                              Form("Current evolution for the Negative HV of %s in spill; Time [s]; Current [uA]",
                                    sNameFirstDet.Data()),
                                    iNbBinsSpill, dStartTime, dStopTime );
   hCurrentEvoSpillNegA->SetLineColor(kBlue);
   hCurrentEvoSpillNegA->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoSpillNegA->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile * hCurrentEvoSpillPosA = new TProfile( "hCurrentEvoSpillPosA",
                              Form("Current evolution for the Positive HV of %s in spill; Time [s]; Current [uA]",
                                    sNameFirstDet.Data()),
                                    iNbBinsSpill, dStartTime, dStopTime );
   hCurrentEvoSpillPosA->SetLineColor(kRed);
   hCurrentEvoSpillPosA->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoSpillPosA->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");

   TProfile * hCurrentEvoSpillNegB = new TProfile( "hCurrentEvoSpillNegB",
                              Form("Current evolution for the Negative HV of %s in spill; Time [s]; Current [uA]",
                                    sNameSecondDet.Data()),
                                    iNbBinsSpill, dStartTime, dStopTime );
   hCurrentEvoSpillNegB->SetLineColor(kBlue);
   hCurrentEvoSpillNegB->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoSpillNegB->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile * hCurrentEvoSpillPosB = new TProfile( "hCurrentEvoSpillPosB",
                              Form("Current evolution for the Positive HV of %s in spill; Time [s]; Current [uA]",
                                    sNameSecondDet.Data()),
                                    iNbBinsSpill, dStartTime, dStopTime );
   hCurrentEvoSpillPosB->SetLineColor(kRed);
   hCurrentEvoSpillPosB->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoSpillPosB->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");

   // Out of spill histograms
   TProfile * hCurrentEvoNoSpillNegA = new TProfile( "hCurrentEvoNoSpillNegA",
                              Form("Current evolution for the Negative HV of %s off spill; Time [s]; Current [uA]",
                                    sNameFirstDet.Data()),
                                    iNbBinsSpill, dStartTime, dStopTime );
   hCurrentEvoNoSpillNegA->SetLineColor(kBlue);
   hCurrentEvoNoSpillNegA->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoNoSpillNegA->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile * hCurrentEvoNoSpillPosA = new TProfile( "hCurrentEvoNoSpillPosA",
                              Form("Current evolution for the Positive HV of %s off spill; Time [s]; Current [uA]",
                                    sNameFirstDet.Data()),
                                    iNbBinsSpill, dStartTime, dStopTime );
   hCurrentEvoNoSpillPosA->SetLineColor(kRed);
   hCurrentEvoNoSpillPosA->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoNoSpillPosA->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");

   TProfile * hCurrentEvoNoSpillNegB = new TProfile( "hCurrentEvoNoSpillNegB",
                              Form("Current evolution for the Negative HV of %s off spill; Time [s]; Current [uA]",
                                    sNameSecondDet.Data()),
                                    iNbBinsSpill, dStartTime, dStopTime );
   hCurrentEvoNoSpillNegB->SetLineColor(kBlue);
   hCurrentEvoNoSpillNegB->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoNoSpillNegB->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile * hCurrentEvoNoSpillPosB = new TProfile( "hCurrentEvoNoSpillPosB",
                              Form("Current evolution for the Positive HV of %s off spill; Time [s]; Current [uA]",
                                    sNameSecondDet.Data()),
                                    iNbBinsSpill, dStartTime, dStopTime );
   hCurrentEvoNoSpillPosB->SetLineColor(kRed);
   hCurrentEvoNoSpillPosB->GetXaxis()->SetTimeDisplay(1);
   hCurrentEvoNoSpillPosB->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");

   TProfile * hCurrentEvoNegRpc[kiNbRpc];
   TProfile * hCurrentEvoPosRpc[kiNbRpc];
   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
   {
      hCurrentEvoNegRpc[iRpcIndex] = new TProfile(
                  Form("hCurrentEvoNegRpc_%s", sChNameRpc[iRpcIndex].Data() ),
                  Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]",
                        sChNameRpc[iRpcIndex].Data()),
                  iNbBins, dStartTime, dStopTime );
      hCurrentEvoNegRpc[iRpcIndex]->SetLineColor(kBlue);
      hCurrentEvoNegRpc[iRpcIndex]->GetXaxis()->SetTimeDisplay(1);
      hCurrentEvoNegRpc[iRpcIndex]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
      hCurrentEvoPosRpc[iRpcIndex] = new TProfile(
                  Form("hCurrentEvoPosRpc_%s", sChNameRpc[iRpcIndex].Data() ),
                  Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]",
                        sChNameRpc[iRpcIndex].Data()),
                  iNbBins, dStartTime, dStopTime );
      hCurrentEvoNegRpc[iRpcIndex]->SetLineColor(kRed);
      hCurrentEvoNegRpc[iRpcIndex]->GetXaxis()->SetTimeDisplay(1);
      hCurrentEvoNegRpc[iRpcIndex]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   } // for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)

   TProfile * hCurrentEvoPmt[kiNbPmt];
   TProfile * hVoltageEvoPmt[kiNbPmt];
   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
   {
      hCurrentEvoPmt[iPmtIndex] = new TProfile(
                  Form("hCurrentEvoPmt_%s", sChNamePmt[iPmtIndex].Data() ),
                  Form("Current evolution for the HV of %s; Time [s]; Current [uA]",
                        sChNamePmt[iPmtIndex].Data()),
                  iNbBins, dStartTime, dStopTime );
      hCurrentEvoPmt[iPmtIndex]->SetLineColor(kBlue);
      hCurrentEvoPmt[iPmtIndex]->GetXaxis()->SetTimeDisplay(1);
      hCurrentEvoPmt[iPmtIndex]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
      hVoltageEvoPmt[iPmtIndex] = new TProfile(
                  Form("hVoltageEvoPmt_%s", sChNamePmt[iPmtIndex].Data() ),
                  Form("Voltage evolution for the HV of %s; Time [s]; Voltage [V]",
                        sChNamePmt[iPmtIndex].Data()),
                  iNbBins, dStartTime, dStopTime );
      hVoltageEvoPmt[iPmtIndex]->SetLineColor(kRed);
      hVoltageEvoPmt[iPmtIndex]->GetXaxis()->SetTimeDisplay(1);
      hVoltageEvoPmt[iPmtIndex]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   } // for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)

   TProfile * hCurrentEvoNegTrd[kiNbTrd];
   TProfile * hCurrentEvoPosTrd[kiNbTrd];
   for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
   {
      hCurrentEvoNegTrd[iTrdIndex] = new TProfile(
                  Form("hCurrentEvoNegTrd_%s", sChNameTrd[iTrdIndex].Data() ),
                  Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]",
                        sChNameTrd[iTrdIndex].Data()),
                  iNbBins, dStartTime, dStopTime );
      hCurrentEvoNegTrd[iTrdIndex]->SetLineColor(kBlue);
      hCurrentEvoNegTrd[iTrdIndex]->GetXaxis()->SetTimeDisplay(1);
      hCurrentEvoNegTrd[iTrdIndex]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
      hCurrentEvoPosTrd[iTrdIndex] = new TProfile(
                  Form("hCurrentEvoPosTrd_%s", sChNameTrd[iTrdIndex].Data() ),
                  Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]",
                        sChNameTrd[iTrdIndex].Data()),
                  iNbBins, dStartTime, dStopTime );
      hCurrentEvoNegTrd[iTrdIndex]->SetLineColor(kRed);
      hCurrentEvoNegTrd[iTrdIndex]->GetXaxis()->SetTimeDisplay(1);
      hCurrentEvoNegTrd[iTrdIndex]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   } // for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)

   Int_t iNbHvPoint = 0;
   for( UInt_t uHvPoint = 0; uHvPoint < uNTreeEntriesHv; uHvPoint ++)
   {
      tInputTreeHv->GetEntry(uHvPoint);

      // First get time using the first detector
      // TIP: need to explicitly add a 0 for the nanoseconds to be sure the right
      //      ctor is used!
      TTimeStamp tEventTime( (time_t)(tPmtHvEvt[iRefDet].iTimeSec), 0 );

      // Jump events before chosen start time
      if( tEventTime < tStartTime )
         continue;
      // Stop looping when chosen stop time is reached
      if( tStopTime < tEventTime )
         break;

      // Plot current vs time
      if( kTRUE == bFirstIsRpc ) // RPC
      {
         hCurrentEvoNegA->Fill( tEventTime.GetSec(), tRpcHvEvt[iFirstDet].dCurrentNeg );
         hCurrentEvoPosA->Fill( tEventTime.GetSec(), tRpcHvEvt[iFirstDet].dCurrentPos );
      } // if( kTRUE == bFirstIsRpc )
         else if( kFALSE == bFirstIsPmt ) // TRD
         {
            hCurrentEvoNegA->Fill( tEventTime.GetSec(), tTrdHvEvt[iFirstDet].dCurrentNeg );
            hCurrentEvoPosA->Fill( tEventTime.GetSec(), tTrdHvEvt[iFirstDet].dCurrentPos );
         } // else if( kFALSE == bFirstIsPmt )
         else // PMT
         {
            hCurrentEvoNegA->Fill( tEventTime.GetSec(), tPmtHvEvt[iFirstDet].dCurrent );
         }

      if( kTRUE == bSecondIsRpc ) // RPC
      {
         hCurrentEvoNegB->Fill( tEventTime.GetSec(), tRpcHvEvt[iSecondDet].dCurrentNeg );
         hCurrentEvoPosB->Fill( tEventTime.GetSec(), tRpcHvEvt[iSecondDet].dCurrentPos );
      } // if( kTRUE == bSecondIsRpc )
         else if( kFALSE == bSecondIsPmt ) // TRD
         {
            hCurrentEvoNegB->Fill( tEventTime.GetSec(), tTrdHvEvt[iSecondDet].dCurrentNeg );
            hCurrentEvoPosB->Fill( tEventTime.GetSec(), tTrdHvEvt[iSecondDet].dCurrentPos );
         } // else if( kFALSE == bSecondIsPmt )
         else // PMT
         {
            hCurrentEvoNegB->Fill( tEventTime.GetSec(), tPmtHvEvt[iSecondDet].dCurrent );
         }


      hCurrentEvoDiam->Fill( tEventTime.GetSec(), tPmtHvEvt[iRefDet].dCurrent );
      hVoltageEvoDiam->Fill( tEventTime.GetSec(), tPmtHvEvt[iRefDet].dVoltage );

      // Plot current vs time

      if( kdSpillDiamThr <= tPmtHvEvt[iRefDet].dCurrent )
      {
         if( kTRUE == bFirstIsRpc ) // RPC
         {
            hCurrentEvoSpillNegA->Fill( tEventTime.GetSec(), tRpcHvEvt[iFirstDet].dCurrentNeg );
            hCurrentEvoSpillPosA->Fill( tEventTime.GetSec(), tRpcHvEvt[iFirstDet].dCurrentPos );
         } // if( kTRUE == bFirstIsRpc )
            else if( kFALSE == bFirstIsPmt ) // TRD
            {
               hCurrentEvoSpillNegA->Fill( tEventTime.GetSec(), tTrdHvEvt[iFirstDet].dCurrentNeg );
               hCurrentEvoSpillPosA->Fill( tEventTime.GetSec(), tTrdHvEvt[iFirstDet].dCurrentPos );
            } // else if( kFALSE == bFirstIsPmt )
            else // PMT
            {
               hCurrentEvoSpillNegA->Fill( tEventTime.GetSec(), tPmtHvEvt[iFirstDet].dCurrent );
            }

         if( kTRUE == bSecondIsRpc ) // RPC
         {
            hCurrentEvoSpillNegB->Fill( tEventTime.GetSec(), tRpcHvEvt[iSecondDet].dCurrentNeg );
            hCurrentEvoSpillPosB->Fill( tEventTime.GetSec(), tRpcHvEvt[iSecondDet].dCurrentPos );
         } // if( kTRUE == bSecondIsRpc )
            else if( kFALSE == bSecondIsPmt ) // TRD
            {
               hCurrentEvoSpillNegB->Fill( tEventTime.GetSec(), tTrdHvEvt[iSecondDet].dCurrentNeg );
               hCurrentEvoSpillPosB->Fill( tEventTime.GetSec(), tTrdHvEvt[iSecondDet].dCurrentPos );
            } // else if( kFALSE == bSecondIsPmt )
            else // PMT
            {
               hCurrentEvoSpillNegB->Fill( tEventTime.GetSec(), tPmtHvEvt[iSecondDet].dCurrent );
            }
      } // if( kdSpillDiamThr <= dMeanFluxDiamA )
         else if( tPmtHvEvt[iRefDet].dCurrent < kdNoSpillDiamThr )
         {
            if( kTRUE == bFirstIsRpc ) // RPC
            {
               hCurrentEvoNoSpillNegA->Fill( tEventTime.GetSec(), tRpcHvEvt[iFirstDet].dCurrentNeg );
               hCurrentEvoNoSpillPosA->Fill( tEventTime.GetSec(), tRpcHvEvt[iFirstDet].dCurrentPos );
            } // if( kTRUE == bFirstIsRpc )
               else if( kFALSE == bFirstIsPmt ) // TRD
               {
                  hCurrentEvoNoSpillNegA->Fill( tEventTime.GetSec(), tTrdHvEvt[iFirstDet].dCurrentNeg );
                  hCurrentEvoNoSpillPosA->Fill( tEventTime.GetSec(), tTrdHvEvt[iFirstDet].dCurrentPos );
               } // else if( kFALSE == bFirstIsPmt )
               else // PMT
               {
                  hCurrentEvoNoSpillNegA->Fill( tEventTime.GetSec(), tPmtHvEvt[iFirstDet].dCurrent );
               }

            if( kTRUE == bSecondIsRpc ) // RPC
            {
               hCurrentEvoNoSpillNegB->Fill( tEventTime.GetSec(), tRpcHvEvt[iSecondDet].dCurrentNeg );
               hCurrentEvoNoSpillPosB->Fill( tEventTime.GetSec(), tRpcHvEvt[iSecondDet].dCurrentPos );
            } // if( kTRUE == bSecondIsRpc )
               else if( kFALSE == bSecondIsPmt ) // TRD
               {
                  hCurrentEvoNoSpillNegB->Fill( tEventTime.GetSec(), tTrdHvEvt[iSecondDet].dCurrentNeg );
                  hCurrentEvoNoSpillPosB->Fill( tEventTime.GetSec(), tTrdHvEvt[iSecondDet].dCurrentPos );
               } // else if( kFALSE == bSecondIsPmt )
               else // PMT
               {
                  hCurrentEvoNoSpillPosB->Fill( tEventTime.GetSec(), tPmtHvEvt[iSecondDet].dCurrent );
               }
         } // else if( dMeanFluxDiamA < kdNoSpillDiamThr )


      for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      {
         hCurrentEvoNegRpc[iRpcIndex]->Fill( tEventTime.GetSec(), tRpcHvEvt[iRpcIndex].dCurrentNeg );
         hCurrentEvoPosRpc[iRpcIndex]->Fill( tEventTime.GetSec(), tRpcHvEvt[iRpcIndex].dCurrentPos );
      } // for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      {
         hCurrentEvoPmt[iPmtIndex]->Fill( tEventTime.GetSec(), tPmtHvEvt[iPmtIndex].dCurrent );
         hVoltageEvoPmt[iPmtIndex]->Fill( tEventTime.GetSec(), tPmtHvEvt[iPmtIndex].dVoltage );
      } // for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
      {
         hCurrentEvoNegTrd[iTrdIndex]->Fill( tEventTime.GetSec(), tTrdHvEvt[iTrdIndex].dCurrentNeg );
         hCurrentEvoPosTrd[iTrdIndex]->Fill( tEventTime.GetSec(), tTrdHvEvt[iTrdIndex].dCurrentPos );
      } // for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
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

   /*
   // Plotting the last HV point is easy: known it match the last MBS event
   tInputTreeHv->GetEntry(uHvStopPoint);

   hCurrentEvoNegA->Fill(dLastMbsTime - dFirstMbsTime, tDetHvEvtA.dCurrentNeg );
   hCurrentEvoPosA->Fill(dLastMbsTime - dFirstMbsTime, tDetHvEvtA.dCurrentPos );
   hCurrentEvoNegB->Fill(dLastMbsTime - dFirstMbsTime, tDetHvEvtB.dCurrentNeg );
   hCurrentEvoPosB->Fill(dLastMbsTime - dFirstMbsTime, tDetHvEvtB.dCurrentPos );
*/

/*
   delete tInputTreeScal;
   delete tInputTreeHv;
*/
   // Check is histos are filled
   cout<<"First RPC histos:  "<<hCurrentEvoNegA->GetEntries()<<" "
       <<hCurrentEvoPosA->GetEntries()<<endl;
   cout<<"Second RPC histos: "<<hCurrentEvoNegB->GetEntries()<<" "
       <<hCurrentEvoPosB->GetEntries()<<endl;

   // Now need to be displayed
   TCanvas* tCanvasA = new TCanvas("tCanvasA","Currents evolution",0,0,1000, 500);
   tCanvasA->SetFillColor(0);
   tCanvasA->SetGridx(0);
   tCanvasA->SetGridy(0);
   tCanvasA->SetTopMargin(0);
   tCanvasA->SetRightMargin(0);
   tCanvasA->SetBottomMargin(0);
   tCanvasA->SetLeftMargin(0);
   tCanvasA->Divide(2);

   tCanvasA->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hCurrentEvoPosA->Draw("hSAME");
   hCurrentEvoNegA->Draw("hSAME");

   tCanvasA->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hCurrentEvoPosB->Draw("hSAME");
   hCurrentEvoNegB->Draw("hSAME");

   TCanvas* tCanvasDiam = new TCanvas("tCanvasDiam","Currents evolution Diamond",0,0,1000, 500);
   tCanvasDiam->SetFillColor(0);
   tCanvasDiam->SetGridx(0);
   tCanvasDiam->SetGridy(0);
   tCanvasDiam->SetTopMargin(0);
   tCanvasDiam->SetRightMargin(0);
   tCanvasDiam->SetBottomMargin(0);
   tCanvasDiam->SetLeftMargin(0);
   tCanvasDiam->Divide(2);

   tCanvasDiam->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hCurrentEvoDiam->Draw("hSAME");

   tCanvasDiam->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hVoltageEvoDiam->Draw("hSAME");


   TCanvas* tCanvasB = new TCanvas("tCanvasB","Currents evolution in spill",0,0,1000, 500);
   tCanvasB->SetFillColor(0);
   tCanvasB->SetGridx(0);
   tCanvasB->SetGridy(0);
   tCanvasB->SetTopMargin(0);
   tCanvasB->SetRightMargin(0);
   tCanvasB->SetBottomMargin(0);
   tCanvasB->SetLeftMargin(0);
   tCanvasB->Divide(2);

   tCanvasB->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hCurrentEvoSpillPosA->Draw("hSAME");
   hCurrentEvoSpillNegA->Draw("hSAME");

   tCanvasB->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hCurrentEvoSpillPosB->Draw("hSAME");
   hCurrentEvoSpillNegB->Draw("hSAME");


   TCanvas* tCanvasC = new TCanvas("tCanvasC","Currents evolution out of spill",0,0,1000, 500);
   tCanvasC->SetFillColor(0);
   tCanvasC->SetGridx(0);
   tCanvasC->SetGridy(0);
   tCanvasC->SetTopMargin(0);
   tCanvasC->SetRightMargin(0);
   tCanvasC->SetBottomMargin(0);
   tCanvasC->SetLeftMargin(0);
   tCanvasC->Divide(2);

   tCanvasC->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hCurrentEvoNoSpillPosA->Draw("hSAME");
   hCurrentEvoNoSpillNegA->Draw("hSAME");

   tCanvasC->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);
   hCurrentEvoNoSpillPosB->Draw("hSAME");
   hCurrentEvoNoSpillNegB->Draw("hSAME");

   TCanvas* tCanvasRpc = new TCanvas("tCanvasRpc","Currents evolution for all RPCs",0,0,1200, 900);
   tCanvasRpc->SetFillColor(0);
   tCanvasRpc->SetGridx(0);
   tCanvasRpc->SetGridy(0);
   tCanvasRpc->SetTopMargin(0);
   tCanvasRpc->SetRightMargin(0);
   tCanvasRpc->SetBottomMargin(0);
   tCanvasRpc->SetLeftMargin(0);
   tCanvasRpc->Divide(4, 3);

   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
   {
      tCanvasRpc->cd(1 + iRpcIndex);
      gPad->SetTopMargin(0.15);
      gPad->SetRightMargin(0.15);
      gPad->SetBottomMargin(0.15);
      gPad->SetLeftMargin(0.15);
      hCurrentEvoNegRpc[iRpcIndex]->Draw("hSAME");
      hCurrentEvoPosRpc[iRpcIndex]->Draw("hSAME");
   } // for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)

   TCanvas* tCanvasPmt = new TCanvas("tCanvasPmt","Currents evolution for all PMTs",0,0,1200, 900);
   tCanvasPmt->SetFillColor(0);
   tCanvasPmt->SetGridx(0);
   tCanvasPmt->SetGridy(0);
   tCanvasPmt->SetTopMargin(0);
   tCanvasPmt->SetRightMargin(0);
   tCanvasPmt->SetBottomMargin(0);
   tCanvasPmt->SetLeftMargin(0);
   tCanvasPmt->Divide(4, 3);

   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
   {
      tCanvasPmt->cd(1 + iPmtIndex);
      gPad->SetTopMargin(0.15);
      gPad->SetRightMargin(0.15);
      gPad->SetBottomMargin(0.15);
      gPad->SetLeftMargin(0.15);
      hVoltageEvoPmt[iPmtIndex]->Draw("hSAME");
      hCurrentEvoPmt[iPmtIndex]->Draw("hSAME");
   } // for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)

   TCanvas* tCanvasTrd = new TCanvas("tCanvasTrd","Currents evolution for all TRDs",0,0,1200, 900);
   tCanvasTrd->SetFillColor(0);
   tCanvasTrd->SetGridx(0);
   tCanvasTrd->SetGridy(0);
   tCanvasTrd->SetTopMargin(0);
   tCanvasTrd->SetRightMargin(0);
   tCanvasTrd->SetBottomMargin(0);
   tCanvasTrd->SetLeftMargin(0);
   tCanvasTrd->Divide(4, 3);

   for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
   {
      tCanvasTrd->cd(1 + iTrdIndex);
      gPad->SetTopMargin(0.15);
      gPad->SetRightMargin(0.15);
      gPad->SetBottomMargin(0.15);
      gPad->SetLeftMargin(0.15);
      hCurrentEvoNegTrd[iTrdIndex]->Draw("hSAME");
      hCurrentEvoPosTrd[iTrdIndex]->Draw("hSAME");
   } // for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)

   fInputFileHv->Close();

   return kTRUE;
}
