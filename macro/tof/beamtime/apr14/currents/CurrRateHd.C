#include "TString.h"
#include "StructDef.h"

Bool_t CurrRateHd(Int_t iDet = 0, Int_t iDetectorIndex = 0, Double_t dTimeOffset = 0.0, Double_t dScaleFactor = 1.0)
{
   if(2 < iDet)
  {
      cout<<" not a possible detector number "<<endl;
      cout<<" PMT = 0, ref RPC = 1, big RPC = 2 "<<endl;  
      return kFALSE;
   }
   Int_t iDetId[3] = {1, 7, 10};
   TString iDetName[3] = {"PMT", "ref RPC", "big RPC"};
   Double_t dDetArea[3] = {44.0, 59.5, 2756.0};


const Int_t   kiNbFiles  = 5 + 12 + 8 + 14 + 15 + 1;
const TString ksFilesDay[kiNbFiles]  = { "200414", "200414", "200414", "200414", "200414",  
                                         "210414", "210414", "210414", "210414", "210414", "210414", 
                                         "210414", "210414", "210414", "210414", "210414", "210414", 
                                         "220414", "220414", "220414", "220414", "220414", "220414", "220414", "220414",
                                         "230414", "230414", "230414", "230414", "230414", "230414",
                                         "230414", "230414", "230414", "230414", "230414", "230414", "230414", "230414",
                                         "240414", "240414", "240414", "240414", "240414", "240414", "240414", "240414", 
                                         "240414", "240414", "240414", "240414", "240414", "240414", "240414", "250414" };
const TString ksFilesHour[kiNbFiles] = {   "1615",   "1759",   "1806",   "1842",   "2249",  
                                           "0153",   "0342",   "0627",   "0712",   "0900",   "1050", 
                                           "1103",   "1557",   "1801",   "1941",   "2211",   "2338", 
                                           "0024",   "0251",   "0445",   "0531",   "1425",   "2032",   "2200",   "2332",
                                           "0101",   "0155",   "0315",   "0442",   "0619",   "1010",
                                           "1450",   "2036",   "2037",   "2136",   "2204",   "2331",   "2335",   "2336",
                                           "0002",   "0018",   "0041",   "0146",   "0244",   "0334",   "0342",   "0356",   
                                           "0414",   "0435",   "0454",   "1331",   "1855",   "2212",   "2223",   "0053" };

   //Int_t iDate = 210414;
   //Int_t iTime = 153;
   //Int_t iTime = 342;
   //Int_t iTime = 627;
   //Int_t iTime = 712;

   Int_t iDate = 240414;
   //Int_t iTime = 18;
   Int_t iTime = 41;
   Int_t iSecOffset = 0;

   Int_t   iIndexVoltage = 0;
   Int_t   iIndexCurrent = 1;
   Int_t   iIndexPower   = 2;
   Int_t   iIndexStatus  = 3;
   Int_t   iNbFields     = 4;
   Int_t   iNbChannels   = 4;

   const Int_t   kiNbRpc         = 11;
   TString sChNameRpc[kiNbRpc]   = { "HD_Big", "HD_Sm", "HD_Ref", "USTC", "TS_PAD", "TS_Strip", "BUC_Ref", "BUC_2010", "BUC_2012", "BUC_2013", "TRD_BUC"}; 
   Int_t   iSlotRpcNeg[kiNbRpc]  = {       0 ,      0 ,       0 ,      0 ,      0 ,         0 ,        1 ,         1 ,         1 ,         1 ,        1 };
   Int_t   iBdIdxRpcNeg[kiNbRpc] = {       0 ,      0 ,       0 ,      0 ,      0 ,         0 ,        4 ,         4 ,         4 ,         4 ,        4 };
   Int_t   iChIdxRpcNeg[kiNbRpc] = {       0 ,      1 ,       2 ,      3 ,      4 ,         5 ,        0 ,         1 ,         2 ,         3 ,        4 };
   Int_t   iSlotRpcPos[kiNbRpc]  = {       2 ,      2 ,       2 ,      2 ,      2 ,         2 ,        3 ,         3 ,         3 ,         3 ,        3 };
   Int_t   iBdIdxRpcPos[kiNbRpc] = {       2 ,      2 ,       2 ,      2 ,      2 ,         2 ,        6 ,         6 ,         6 ,         6 ,        6 };
   Int_t   iChIdxRpcPos[kiNbRpc] = {       0 ,      1 ,       2 ,      3 ,      4 ,         5 ,        0 ,         1 ,         2 ,         3 ,        4 };

   const Int_t   kiNbPmt         = 10;
   TString sChNamePmt[kiNbPmt]   = { "BUC_PMT1", "BUC_PMT2", "BUC_PMT3", "BUC_PMT4", "PMT1", "PMT2", "PMT3", "PMT4", "BUC_PMT5", "BUC_PMT6"};
   Int_t   iSlotPmtNeg[kiNbPmt]  = {         4 ,         4 ,         4 ,         4 ,     5 ,     5 ,     5 ,     5 ,         5 ,         5 };
   Int_t   iBdIdxPmtNeg[kiNbPmt] = {        13 ,        13 ,        13 ,        13 ,    14 ,    14 ,    14 ,    14 ,        14 ,        14 };
   Int_t   iChIdxPmtNeg[kiNbPmt] = {         0 ,         1 ,         4 ,         5 ,     0 ,     1 ,     2 ,     3 ,         4 ,         5 };

   Rpc_HV  stRpcValArray[kiNbRpc];
   Pmt_HV  stPmtValArray[kiNbPmt];


   // Open the unpacked data file
   //TString sInputFilenameScal = "unpack.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbMon0153.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbMon0345.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbMon0558.out.root";
   //TString sInputFilenameScal = "../unpack_MbsTrbMon0710.out.root";
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


   TProfile * hRateEvoScalMbs = new TProfile( "hRateEvoScalMbs", 
                              "Triglog input scaler 2 (Back HD PMT); MBS Time [s]; Rate [kHz/cm2]", 
                              iBinSec*iTimeIntervalsec, 0, iTimeIntervalsec );
   TH1 * hCurrent = new TH1D( "hCurrent", 
                              Form("Current and rate of the %s RPC; MBS Time [s]; Flux [%.1f x kHz/cm^{2}], I [#muA]",
                              sChNameRpc[iDetectorIndex].Data(), dScaleFactor), 
                              iBinSec*iTimeIntervalsec, 0, iTimeIntervalsec );


   TProfile * hRateEvoScalMbsw = new TProfile( "hRateEvoScalMbsw", 
                              "Triglog input scaler 2 (Back HD PMT); MBS Time [s]; Rate [kHz/cm2]", 
                              iBinSec*iTimeIntervalsec, 0, iTimeIntervalsec );
  
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
            dRateKhzCm   = fCalTrloBoard->GetScalerValue(iDetId[iDet]) / 1000.0 / dDetArea[iDet]; // back HD PMT (4cmx11cm)


//      if( 0 < fCalTrloBoard->GetTimeToLast() )
      if( 1e-5 < fCalTrloBoard->GetTimeToLast() ) // avoid too low reference clock counts
//      if( 5e-5 < fCalTrloBoard->GetTimeToLast() ) // avoid too low reference clock counts
      {
         // Scale rate with time since last event for averaging
         //hRateEvoScalMbs0->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm1, fCalTrloBoard->GetTimeToLast() );

         hRateEvoScalMbs->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm, fCalTrloBoard->GetTimeToLast());
         hRateEvoScalMbsw->Fill(   dCurrMbsTime - dFirstMbsTime,    dRateKhzCm, fCalTrloBoard->GetTimeToLast());
       }
   } // for( uMbsEvtIndex = 0; uMbsEvtIndex < uNTreeEntriesScal; uMbsEvtIndex ++)



   // Looking for RPC currents
   const Int_t kiNbBoardsUsed = 6;
   Int_t kiSlotIndex[kiNbBoardsUsed] = { 0,  4, 2, 6, 13, 14};
   Int_t kiSlotSzOff[kiNbBoardsUsed] = { 0,  0, 0, 0,  2,  2};
   TString sFileName[kiNbBoardsUsed];
   fstream * fInputFile[kiNbBoardsUsed];
   std::streampos fSizeFile[kiNbBoardsUsed];

   if( iDetectorIndex < 0 || 0 == iDate || -1 == iTime )
   {
      cout<<"Please provide a detector index, a data and a time as input!!!!"<<endl;
      cout<<"Example:        root -l \"currentConv.C( 3, 220414, 445  )\" "<<endl;
      cout<<"or within root: .x currentConv.C( 1 ) "<<endl;
      cout<<"Possible detector values are: "<<endl;
      cout<<" 0 for HD RPC-P3 "<<endl;
      cout<<" 1 for HD RPC-P2 "<<endl;
      cout<<" 2 for HD RPC-P5 "<<endl;
      cout<<" 3 for USTC "     <<endl;
      cout<<" 4 for TSU PAD "  <<endl;
      cout<<" 5 for TSU Strip "<<endl;
      cout<<" 6 for BUC ref "  <<endl;
      cout<<" 7 for BUC 2010 " <<endl;
      cout<<" 8 for BUC 2012 " <<endl;
      cout<<" 9 for BUC 2013"  <<endl;
      cout<<"10 for TRD BUC "  <<endl;
      cout<<"-----------------"<<endl;
      cout<<"10 for BUC PMT1"  <<endl;
      cout<<"11 for BUC PMT2"  <<endl;
      cout<<"12 for BUC PMT3"  <<endl;
      cout<<"13 for BUC PMT4"  <<endl;
      cout<<"14 for HD  PMT1"  <<endl;
      cout<<"15 for HD  PMT2"  <<endl;
      cout<<"16 for HD  PMT3"  <<endl;
      cout<<"17 for HD  PMT4"  <<endl;
      cout<<"18 for BUC PMT5"  <<endl;
      cout<<"19 for BUC PMT6"  <<endl;
      cout<<"The date format is: YYYYMMDD"<<endl;
      cout<<"The time format is: HHMM"<<endl;
      return kFALSE;
   } // if( iDetectorIndex < 0 )

   Bool_t bDetType = kFALSE; // kFALSE for RPC, kTRUE for PMT
   if( 9 < iDetectorIndex )
   {
      bDetType = kTRUE;
      iDetectorIndex -= 10;
   } // if( 9 < iDetectorIndex )

   Int_t iFileIndex = 0;
   for( iFileIndex = 0; iFileIndex < kiNbFiles; iFileIndex++)
      if( ksFilesDay[iFileIndex].Atoi() == iDate &&
          ksFilesHour[iFileIndex].Atoi() == iTime )
         break;
   if( kiNbFiles == iFileIndex )
   {
      cout<<"Invalid date/time combination!!!!"<<endl;
      cout<<"Possible combinations are the following:"<<endl;
      for( iFileIndex = 0; iFileIndex < kiNbFiles; iFileIndex++)
         cout<<ksFilesDay[iFileIndex]<<" "<<ksFilesHour[iFileIndex]<<endl;
      cout<<"Other hint: leading 0 should be removed."<<endl;
      return kFALSE;
   } // if( kiNbFiles == iFileIndex )
   
   for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
   {
      sFileName[iBoardIndex] = Form("./Log_Slot%02d_%s_%s.csv", 
                                    kiSlotIndex[iBoardIndex], 
                                    ksFilesDay[iFileIndex].Data(), 
                                    ksFilesHour[iFileIndex].Data());
//            cout<<iBoardIndex<<" . "<<kiSlotIndex[iBoardIndex]<<" . "
 //               <<ksFilesDay[iFileIndex]<<" . "<<ksFilesHour[iFileIndex]<<" . "<<sFileName[iBoardIndex]<<endl;
      fInputFile[iBoardIndex] = new fstream(sFileName[iBoardIndex], ios::in);

      if( true == fInputFile[iBoardIndex]->fail() )
      {
         cout<<"************************************************************************"<<endl;
         cout<<" File with index "<<iBoardIndex<<" could not be opened, filename tried: "<<sFileName[iBoardIndex]<<endl;
         return kFALSE;
      } // if( true == fNegFile.fail() || true == fPosFile.fail() )

      fSizeFile[iBoardIndex] = 0;
      fInputFile[iBoardIndex]->seekg( 0, std::ios::end );
      fSizeFile[iBoardIndex] = fInputFile[iBoardIndex]->tellg();
      fInputFile[iBoardIndex]->seekg( 0, std::ios::beg );
   } // for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)

   Bool_t bAllSameSize = kTRUE;
   for( Int_t iBoardIndex = 1; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
      if( (fSizeFile[0] + kiSlotSzOff[iBoardIndex]) != fSizeFile[iBoardIndex] )
      {
         cout<<" Warning!!!! the file for board "<<iBoardIndex<<" does not have the same size as the first one for file "
             <<iFileIndex<<" : "<<endl;
         cout<<" Sizes are "<<fSizeFile[0]<<" VS "<<fSizeFile[iBoardIndex]<<endl;
         cout<<" For files "<<sFileName[0]<<" and "<<sFileName[iBoardIndex]<<endl;
         bAllSameSize = kFALSE;
      } // if( fSizeFile[0] != fSizeFile[iBoardIndex] )

   // If at least one of the file is missing some line, just jump to the next file series
   if( kFALSE == bAllSameSize )
      continue;

   TString sLine[kiNbBoardsUsed];
   TObjArray *pValuesArray[kiNbBoardsUsed];
   TString sDayString = ksFilesDay[iFileIndex];
   TString sYearString  = sDayString(4,2);
   TString sMonthString = sDayString(2,2);
   sDayString           = sDayString(0,2);
   TTimeStamp tTempTime;

   // first four lines are header information
   for( Int_t iHeaderLine = 0; iHeaderLine < 5; iHeaderLine++)
   {
      for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
         // Read a line of the HV file and set the iterator automatically to the next line
      //   sLine[iBoardIndex].ReadLine( &(fInputFile[iBoardIndex]) , kFALSE); 
         sLine[iBoardIndex].ReadLine( *(fInputFile[iBoardIndex]) , kFALSE); 
   } // for( Int_t iHeaderLine = 0; iHeaderLine < 5; iHeaderLine++)

   // Clear RPC objects
   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      stRpcValArray[iRpcIndex].Clear();

   // Clear PMT objects
   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      stPmtValArray[iPmtIndex].Clear();

   // Print RPC data before first line
   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      stRpcValArray[iRpcIndex].Print();

   // Print PMT data before first line
   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      stPmtValArray[iPmtIndex].Print();

   // Prepare histograms
   // TODO: User should create his own histograms
   // TODO: User can plot data for more than one detector at a time using the stRpcValArray and stPmtValArray arrays

   Int_t iNbLinesRead = 0;
   Double_t dFirstTime = 0.0;
   Double_t dCurTime = 0.0;
   Int_t iNewDay = 0;
   Int_t iFirstHour = -1; 
   // Goes through all lines
   while( !fInputFile[0]->eof() )
   //while( iNbLinesRead < 5)
   {
      Bool_t bEndReachedInOne = kFALSE;
      for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
      {
         // Read a line of the HV file and set the iterator automatically to the next line
         sLine[iBoardIndex].ReadLine( *(fInputFile[iBoardIndex]) , kFALSE);
         if( 0 == sLine[iBoardIndex].Length() )
         {
            bEndReachedInOne = kTRUE;
            break;
         } // if( 0 == sLine[iBoardIndex].Length() )

         // Extract the fields as sub-strings
         pValuesArray[iBoardIndex] = sLine[iBoardIndex].Tokenize( ";" );
      }

      // If at least one file is over, stop!
      if( kTRUE == bEndReachedInOne )
         break;

      // Extract RPC data as numbers
      for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      {
         TObjString* subString;

         // Take readout time from negative voltage board
         subString = (TObjString*)pValuesArray[ iSlotRpcNeg[iRpcIndex] ]->At(0);
         TString sHour   = subString->GetString();
         TString sMin    = sHour(3,2);
         TString sSec    = sHour(6,2);
         TString sMilSec = sHour(9,2);
         sHour           = sHour(0,2);
         if (-1 == iFirstHour)
            iFirstHour = sHour.Atoi();
         else if ( sHour.Atoi() < iFirstHour)
         {
            iNewDay ++;
            iFirstHour = sHour.Atoi();
         } // else if ( sHour.Atoi() < iFirstHour)

         tTempTime.Set( sYearString.Atoi(),  // Year
                        sMonthString.Atoi(), // Month
                        sDayString.Atoi() +iNewDay,   // Day
                        sHour.Atoi(),        // Hour
                        sMin.Atoi(),         // Minutes
                        sSec.Atoi(),         // Seconds
                        sMilSec.Atoi() * 10000000, // Nanoseconds
                        kFALSE,              // Not in UTC
                        iSecOffset
                         );
         stRpcValArray[iRpcIndex].iTimeSec      = tTempTime.GetSec(); //Extract seconds
         stRpcValArray[iRpcIndex].iTimeMilliSec = tTempTime.GetNanoSec()/1000000; //Extract milliseconds
	  //cout<<iNbLinesRead<<" tTempTime.GetSec() = "<<tTempTime.GetSec()<<"   tTempTime.GetNanoSec() = "<<tTempTime.GetNanoSec()/1000000<<endl;
	 if (0 == iNbLinesRead)
	   dFirstTime = (Double_t)(tTempTime.GetSec()) + (Double_t)(tTempTime.GetNanoSec())/1000000000.;

	   dCurTime = (Double_t)(tTempTime.GetSec()) + (Double_t)(tTempTime.GetNanoSec())/1000000000.;

      //cout<<iNbLinesRead<<" dFirstTime = "<<dFirstTime<<" dCurTime = "<<dCurTime<<endl;
      //cout<<iNbLinesRead<<" Time = "<<dCurTime - dFirstTime<<endl;
         // Go for negative HV values
         subString = (TObjString*)pValuesArray[ iSlotRpcNeg[iRpcIndex] ]->At( 
                                    1 + iNbFields*iChIdxRpcNeg[iRpcIndex] + iIndexVoltage);
         stRpcValArray[iRpcIndex].dVoltageNeg   = (subString->GetString()).Atof(); // HV
         subString = (TObjString*)pValuesArray[ iSlotRpcNeg[iRpcIndex] ]->At( 
                                    1 + iNbFields*iChIdxRpcNeg[iRpcIndex] + iIndexCurrent);
         stRpcValArray[iRpcIndex].dCurrentNeg   = (subString->GetString()).Atof(); // Current
         subString = (TObjString*)pValuesArray[ iSlotRpcNeg[iRpcIndex] ]->At( 
                                    1 + iNbFields*iChIdxRpcNeg[iRpcIndex] + iIndexPower);
         stRpcValArray[iRpcIndex].iPowerNeg     = (subString->GetString()).Atoi(); // Power status (ON/OFF)
         subString = (TObjString*)pValuesArray[ iSlotRpcNeg[iRpcIndex] ]->At( 
                                    1 + iNbFields*iChIdxRpcNeg[iRpcIndex] + iIndexStatus);
         stRpcValArray[iRpcIndex].iStatusNeg    = (subString->GetString()).Atoi(); // Channel status (OK, Tripped, Ramping, ...)

         // Go for Positive HV values
         subString = (TObjString*)pValuesArray[ iSlotRpcPos[iRpcIndex] ]->At( 
                                    1 + iNbFields*iChIdxRpcPos[iRpcIndex] + iIndexVoltage);
         stRpcValArray[iRpcIndex].dVoltagePos   = (subString->GetString()).Atof(); // HV
         subString = (TObjString*)pValuesArray[ iSlotRpcPos[iRpcIndex] ]->At( 
                                    1 + iNbFields*iChIdxRpcPos[iRpcIndex] + iIndexCurrent);
         stRpcValArray[iRpcIndex].dCurrentPos   = (subString->GetString()).Atof(); // Current
         subString = (TObjString*)pValuesArray[ iSlotRpcPos[iRpcIndex] ]->At( 
                                    1 + iNbFields*iChIdxRpcPos[iRpcIndex] + iIndexPower);
         stRpcValArray[iRpcIndex].iPowerPos     = (subString->GetString()).Atoi(); // Power status (ON/OFF)
         subString = (TObjString*)pValuesArray[ iSlotRpcPos[iRpcIndex] ]->At( 
                                    1 + iNbFields*iChIdxRpcPos[iRpcIndex] + iIndexStatus);
         stRpcValArray[iRpcIndex].iStatusPos    = (subString->GetString()).Atoi(); // Channel status (OK, Tripped, Ramping, ...)

      } // for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)

      // Extract PMT data as numbers

      // Fill the histograms
      // For info on the Rpc_HV and Pmt_HV structure contents, see the StructDef.h file
      // TODO: User should fill his own histograms
      // TODO: User can plot data for more than one detector at a time using the stRpcValArray and stPmtValArray arrays
      
      hCurrent->Fill( dCurTime - dFirstTime + dTimeOffset, (stRpcValArray[iDetectorIndex].dCurrentNeg 
                                            + stRpcValArray[iDetectorIndex].dCurrentPos )/2);
  
      iNbLinesRead ++;
      if( 0 == iNbLinesRead%1000 && 0 < iNbLinesRead )
         cout<<iNbLinesRead<<endl;

      bEndReachedInOne = kFALSE;

      for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
      {
         if( NULL != pValuesArray[iBoardIndex] )
            delete pValuesArray[iBoardIndex];
         if( true == fInputFile[iBoardIndex]->eof() )
         {
            bEndReachedInOne = kTRUE;
            break;
         } // if( true == fInputFile[iBoardIndex]->eof() )
      } // for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)

      // If at least one file is over, stop!
      if( kTRUE == bEndReachedInOne )
         break;
   } // while( !fInputFile[0]->eof() )

   // Print number of data lines readout
   cout<<"Nb Lines read out: "<<iNbLinesRead<<endl;

   // plot histos
   // TODO: User should plot his own histograms
   // TODO: User should create canvases


   // Print RPC data from last line
   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      stRpcValArray[iRpcIndex].Print();

   // Print PMT data from last line
   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      stPmtValArray[iPmtIndex].Print();

   // Close the fstream objects (this also delete them)
   for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
      fInputFile[iBoardIndex]->close();


   // Now need to be displayed
   TCanvas* tCanvasA = new TCanvas("tCanvasA","Rate evolution: two case",0,0,2000,1000);
   tCanvasA->SetFillColor(0);
   tCanvasA->SetGridx(0);
   tCanvasA->SetGridy(0);
   tCanvasA->SetTopMargin(0);
   tCanvasA->SetRightMargin(0);
   tCanvasA->SetBottomMargin(0);
   tCanvasA->SetLeftMargin(0);
   //tCanvasA->Divide(2,1);

   tCanvasA->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);

   hCurrent->SetLineColor(kRed);
   hCurrent->SetMarkerColor(kRed);
   hCurrent->SetMarkerSize(1.);
   hCurrent->SetMarkerStyle(20);
   hCurrent->Draw("P");
   hCurrent->GetYaxis()->SetTitleOffset(1.5);

   hRateEvoScalMbs->SetFillColor(kGreen);
   hRateEvoScalMbs->SetFillStyle(3003);
   hRateEvoScalMbs->SetLineColor(kBlack);
   hRateEvoScalMbs->Scale(dScaleFactor);
   hRateEvoScalMbs->Draw("HIST,SAME");

   leg = new TLegend(0.1,0.8,0.4,0.9);
   //leg->SetHeader("The Legend Title");
   leg->AddEntry(hRateEvoScalMbs, Form("Incident particle flux on the %s counter x %.1f", iDetName[iDet].Data(), dScaleFactor),"f");
   leg->AddEntry(hCurrent, Form("%s RPC current", sChNameRpc[iDetectorIndex].Data()));
   leg->Draw();


   //tCanvasA->cd(2);
   //gPad->SetTopMargin(0.15);
   //gPad->SetRightMargin(0.15);
   //gPad->SetBottomMargin(0.15);
   //gPad->SetLeftMargin(0.15);
   //hRateEvoScalMbsw->SetLineColor(kBlack);
   //hRateEvoScalMbsw->Draw("HIST");

   //hRateEvoScalMbsw->GetYaxis()->SetTitleOffset(1.5);


   fInputFileScal->Close();

   return kTRUE;
}
