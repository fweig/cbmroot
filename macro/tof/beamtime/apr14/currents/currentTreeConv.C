
#include "TString.h"
#include "StructDef.h"


Bool_t currentTreeConv( Int_t iFileIndex, Int_t iSecOffset = 0, Int_t iMilliSecOffset = 0 )
{

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

   if( iFileIndex < 0 || kiNbFiles <= iFileIndex )
   {
      cout<<" Invalid File Index."<<endl;
      cout<<" It should be between 0 and "<<kiNbFiles<<", corresponding to following periods:"<<endl;
      for( Int_t iFile = 0; iFile < kiNbFiles; iFile++)
         cout<<iFile<<" "<<ksFilesDay[iFile]<<" "<<ksFilesHour[iFile]<<endl;
      cout<<" The command to use should be in the form currentTreeConv( Integer FileIndex, Integer Offset in seconds )"<<endl;
      cout<<" For example currentTreeConv( 10, -5 )"<<endl;
      return kFALSE;
   } // if( iFileIndex < 0 || kiNbFiles <= iFileIndex )


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

   // Produce a root file with a TTree from the chosen csv file
   TString sOutputFilename = Form("LogHv_%s_%s.root", 
                                    ksFilesDay[iFileIndex].Data(), 
                                    ksFilesHour[iFileIndex].Data());
   TFile * fOutputFile = new TFile(sOutputFilename, "RECREATE", "TTree file with HV slow control data for GSI April 2014", 9);
   sOutputFilename += ":/";

   if( fOutputFile->IsZombie() )
   {
      cout<<" Error opening file for TTree output!!!"<<endl;
      fOutputFile->Close();
      return kFALSE;
   } // if (fOutputFile->IsZombie())

   gDirectory->Cd(sOutputFilename);
   
   TTree * fOutputTree = new TTree("CaenHV", "GSI April 2014 HV slow control");
   cout<<" Output Tree created in "<<gDirectory->GetPath()<<endl;

   // Add RPC data as Tree branches
   TBranch *branchRpc[kiNbRpc];
   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      branchRpc[iRpcIndex] = fOutputTree->Branch( sChNameRpc[iRpcIndex], 
                                                  &(stRpcValArray[iRpcIndex]), 
"iTimeSec/I:iTimeMilliSec:dVoltageNeg/D:dCurrentNeg:iPowerNeg/I:iStatusNeg:dVoltagePos/D:dCurrentPos:iPowerPos/I:iStatusPos" );   

   // Add PMT data as Tree branches
   TBranch *branchPmt[kiNbPmt];
   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      branchPmt[iPmtIndex] = fOutputTree->Branch( sChNamePmt[iPmtIndex], 
                                                  &(stPmtValArray[iPmtIndex]), 
         "iTimeSec/I:iTimeMilliSec:dVoltage/D:dCurrent:iPower/I:iStatus" );   

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

   Int_t iNbLinesRead = 0;
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
                        sSec.Atoi() + iSecOffset,                    // Seconds
                        (sMilSec.Atoi() + iMilliSecOffset) * 10000000, // Nanoseconds
                        kFALSE,              // Not in UTC
                        0
                         );
         stRpcValArray[iRpcIndex].iTimeSec      = tTempTime.GetSec(); //Extract seconds
         stRpcValArray[iRpcIndex].iTimeMilliSec = tTempTime.GetNanoSec()/1000000; //Extract milliseconds

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
                        sSec.Atoi() + iSecOffset,                      // Seconds
                        (sMilSec.Atoi() + iMilliSecOffset) * 10000000, // Nanoseconds
                        kFALSE,              // Not in UTC
                        0
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

      // Save data in the output tree
      fOutputTree->Fill();

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

   // Print RPC data from last line
   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      stRpcValArray[iRpcIndex].Print();

   // Print PMT data from last line
   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      stPmtValArray[iPmtIndex].Print();

   // Close the fstream objects (this also delete them)
   for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
      fInputFile[iBoardIndex]->close();
   
   fOutputFile->Write("",TObject::kOverwrite);
   fOutputFile->Close();

   return kTRUE;
}
