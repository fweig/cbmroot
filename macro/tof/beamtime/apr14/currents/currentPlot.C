
#include "TString.h"
#include "StructDef.h"

//200414 200414 200414 200414 200414  
//  1615   1759   1806   1842   2249

//210414 210414 210414 210414 210414 210414 210414 210414 210414 210414 210414 210414
//  0153   0342   0627   0712   0900   1050   1103   1557   1801   1941   2211   2338

//220414 220414 220414 220414 220414 220414 220414 220414
//  0024   0251   0445   0531   1425   2032   2200   2332

//230414 230414 230414 230414 230414 230414 230414 230414 230414 230414 230414 230414 230414 230414
//  0101   0155   0315   0442   0619   1010   1450   2036   2037   2136   2204   2331   2335   2336

//240414 240414 240414 240414 240414 240414 240414 240414 240414 240414 240414 240414 240414 240414 240414
//  0002   0018   0041   0146   0244   0334   0342   0356   0414   0435   0454   1331   1855   2212   2223

// 250414
//   0053

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

Bool_t currentPlot( Int_t iDetectorIndex = -1, Int_t iDate = 0, Int_t iTime = -1, Int_t iSecOffset = 0 )
{

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
//                <<ksFilesDay[iFileIndex]<<" . "<<ksFilesHour[iFileIndex]<<" . "<<sFileName[iBoardIndex]<<endl;
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
   TH1 * hCurrentEvoNeg;
   TH1 * hCurrentEvoPos;
   TH1 * hCurrentEvoTot;
   TH2 * hCurrentComp;

   TH1 * hVoltageEvoNeg;
   TH1 * hVoltageEvoPos;

   TH1 * hTimeBinCor;

   if( kFALSE == bDetType )
   {
      // Looking for RPC currents
      hCurrentEvoNeg = new TH1D( "hCurrentEvoNeg", 
                                    Form("Current evolution for the Negative HV of %s; Time [s]; Current [uA]", 
                                          sChNameRpc[iDetectorIndex].Data()), 
                                    5*3600, 0, 5*3600);

      hCurrentEvoPos = new TH1D( "hCurrentEvoPos", 
                                    Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]", 
                                          sChNameRpc[iDetectorIndex].Data()), 
                                    5*3600, 0, 5*3600);

      hCurrentEvoTot = new TH1D( "hCurrentEvoTot", 
                                    Form("Total Current evolution of %s; Time [s]; Current [uA]", 
                                          sChNameRpc[iDetectorIndex].Data()), 
                                    5*3600, 0, 5*3600);

      hCurrentComp = new TH2D( "hCurrentComp", 
                                    Form("Currents comparison of %s; Current Neg [uA]; Current Pos [uA]", 
                                          sChNameRpc[iDetectorIndex].Data()), 
                                    500, 0, 50, 
                                    500, 0, 50 );

      hVoltageEvoNeg = new TH1D( "hVoltageEvoNeg", 
                                    Form("Voltage evolution for the Negative HV of %s; Time [s]; Voltage [uA]", 
                                          sChNameRpc[iDetectorIndex].Data()), 
                                    5*3600, 0, 5*3600);

      hVoltageEvoPos = new TH1D( "hVoltageEvoPos", 
                                    Form("Voltage evolution for the Positive HV of %s; Time [s]; Voltage [uA]", 
                                          sChNameRpc[iDetectorIndex].Data()), 
                                    5*3600, 0, 5*3600);

      hTimeBinCor = new TH1D( "hTimeBinCor", "Time bin correlation; Bin; Time [s]", 5*3600, 0, 5*3600);


   } // if( kFALSE == bDetType )
      else
      {
         // Looking for PMT currents
         hCurrentEvoNeg = new TH1D( "hCurrentEvoNeg", 
                                       Form("Current evolution for the Negative HV of %s; Time [s]; Current [uA]", 
                                             sChNamePmt[iDetectorIndex].Data()), 
                                       5*3600, 0, 5*3600);

         hCurrentEvoPos = new TH1D( "hCurrentEvoPos", 
                                       Form("Current evolution for the Positive HV of %s; Time [s]; Current [uA]", 
                                             sChNamePmt[iDetectorIndex].Data()), 
                                       5*3600, 0, 5*3600);

         hCurrentEvoTot = new TH1D( "hCurrentEvoTot", 
                                       Form("Total Current evolution of %s; Time [s]; Current [uA]", 
                                             sChNamePmt[iDetectorIndex].Data()), 
                                       5*3600, 0, 5*3600);

         hCurrentComp = new TH2D( "hCurrentComp", 
                                       Form("Currents comparison of %s; Current Neg [uA]; Current Pos [uA]", 
                                             sChNamePmt[iDetectorIndex].Data()), 
                                       500, 0, 50, 
                                       500, 0, 50 );
      } // else of if( kFALSE == bDetType )

   Int_t iNbLinesRead = 0;
   Int_t iFirstTime = 0;
   Int_t iNewDay = 0;
   Int_t iFirstHour = -1; 
   // Goes through all lines
   while( !fInputFile[0]->eof() )
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
	 // cout<<iNbLinesRead<<" tTempTime.GetSec() = "<<tTempTime.GetSec()<<"   tTempTime.GetNanoSec() = "<<tTempTime.GetNanoSec()/1000000<<endl;
	 if (0 == iNbLinesRead)
	   iFirstTime = tTempTime.GetSec();
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
      for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      {
         TObjString* subString;

         // Take readout time
         subString = (TObjString*)pValuesArray[ iSlotPmtNeg[iPmtIndex] ]->At(0);
         TString sHour   = subString->GetString();
         TString sMin    = sHour(3,2);
         TString sSec    = sHour(6,2);
         TString sMilSec = sHour(9,2);
         sHour           = sHour(0,2);
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
         stPmtValArray[iPmtIndex].iTimeSec      = tTempTime.GetSec(); //Extract seconds
         stPmtValArray[iPmtIndex].iTimeMilliSec = tTempTime.GetNanoSec()/1000000; //Extract milliseconds

         // Go for negative HV values
         subString = (TObjString*)pValuesArray[ iSlotPmtNeg[iPmtIndex] ]->At( 
                                    1 + iNbFields*iChIdxPmtNeg[iPmtIndex] + iIndexVoltage);
         stPmtValArray[iPmtIndex].dVoltage   = (subString->GetString()).Atof(); // HV
         subString = (TObjString*)pValuesArray[ iSlotPmtNeg[iPmtIndex] ]->At( 
                                    1 + iNbFields*iChIdxPmtNeg[iPmtIndex] + iIndexCurrent);
         stPmtValArray[iPmtIndex].dCurrent   = (subString->GetString()).Atof(); // Current
         subString = (TObjString*)pValuesArray[ iSlotPmtNeg[iPmtIndex] ]->At( 
                                    1 + iNbFields*iChIdxPmtNeg[iPmtIndex] + iIndexPower);
         stPmtValArray[iPmtIndex].iPower     = (subString->GetString()).Atoi(); // Power status (ON/OFF)
         subString = (TObjString*)pValuesArray[ iSlotPmtNeg[iPmtIndex] ]->At( 
                                    1 + iNbFields*iChIdxPmtNeg[iPmtIndex] + iIndexStatus);
         stPmtValArray[iPmtIndex].iStatus    = (subString->GetString()).Atoi(); // Channel status (OK, Tripped, Ramping, ...)

      } // for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)

      // Fill the histograms
      // For info on the Rpc_HV and Pmt_HV structure contents, see the StructDef.h file
      // TODO: User should fill his own histograms
      // TODO: User can plot data for more than one detector at a time using the stRpcValArray and stPmtValArray arrays
      if( kFALSE == bDetType )
      {
         // Looking for RPC currents
         hCurrentEvoNeg->Fill( iNbLinesRead, stRpcValArray[iDetectorIndex].dCurrentNeg );
         hCurrentEvoPos->Fill( iNbLinesRead, stRpcValArray[iDetectorIndex].dCurrentPos );
         hCurrentEvoTot->Fill( iNbLinesRead, ( stRpcValArray[iDetectorIndex].dCurrentNeg 
                                            + stRpcValArray[iDetectorIndex].dCurrentPos )/2);
         hCurrentComp->Fill( stRpcValArray[iDetectorIndex].dCurrentNeg, stRpcValArray[iDetectorIndex].dCurrentPos );

         // Looking for RPC voltages
         hVoltageEvoNeg->Fill( iNbLinesRead, stRpcValArray[iDetectorIndex].dVoltageNeg );
         hVoltageEvoPos->Fill( iNbLinesRead, stRpcValArray[iDetectorIndex].dVoltagePos );

         // Looking for time bin correlation

         hTimeBinCor-> Fill(iNbLinesRead, tTempTime.GetSec()-iFirstTime);
         //hTimeBinCor-> Fill(iNbLinesRead, tTempTime.GetNanoSec()/1000000000);
      } // if( kFALSE == bDetType )
         else
         {
            // Looking for PMT currents
            hCurrentEvoNeg->Fill( iNbLinesRead, stPmtValArray[iDetectorIndex].dCurrent );
            hCurrentComp->Fill( stPmtValArray[iDetectorIndex].dVoltage, stPmtValArray[iDetectorIndex].dCurrent );
         } // else of if( kFALSE == bDetType )
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


   TCanvas* tCanvas = new TCanvas("tCanvas","Currents and rate evolution",0,0,2000,1000);
   tCanvas->Divide(2,2,0,0);
   tCanvas->cd(1);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);

   hCurrentEvoTot->Draw();
   hCurrentEvoPos->SetLineColor(kGreen);
   hCurrentEvoPos->Draw("SAME");
   hCurrentEvoNeg->SetLineColor(kRed);
   hCurrentEvoNeg->Draw("SAME");

   tCanvas->cd(2);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);

   hCurrentComp->Draw("colz");

   tCanvas->cd(3);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);

   hVoltageEvoPos->SetLineColor(kRed);
   hVoltageEvoPos->Draw();
   hVoltageEvoNeg->SetLineColor(kBlack);
   hVoltageEvoNeg->Draw("SAME");

   tCanvas->cd(4);
   gPad->SetTopMargin(0.15);
   gPad->SetRightMargin(0.15);
   gPad->SetBottomMargin(0.15);
   gPad->SetLeftMargin(0.15);

   hTimeBinCor->Draw();

   // Print RPC data from last line
   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      stRpcValArray[iRpcIndex].Print();

   // Print PMT data from last line
   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      stPmtValArray[iPmtIndex].Print();

   // Close the fstream objects (this also delete them)
   for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
      fInputFile[iBoardIndex]->close();

   return kTRUE;
}
