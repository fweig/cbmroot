// ROOT headers
#include "Riostream.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TTimeStamp.h"

// C++ headers
//#include <iostream>
#include <fstream>
//#include <string>


// Custom headers
#include "StructDef.h"
#include "FileListDef.h"

Bool_t currentTreeConv( Int_t iFileIndex, Int_t iSecOffset = 0, Int_t iMilliSecOffset = 0 )
{
   if( iFileIndex < 0 || kiNbFiles <= iFileIndex )
   {
      cout<<" Invalid File Index."<<endl;
      cout<<" It should be between 0 and "<<kiNbFiles<<", corresponding to following periods:"<<endl;
      for( Int_t iFile = 0; iFile < kiNbFiles; iFile++)
         cout<<iFile<<" "<<ksFilesDay[iFile]<<" "<<ksFilesHour[iFile]<<endl;
      cout<<" The command to use should be in the form currentTreeConv"
           <<"( Integer FileIndex, Integer Offset in seconds, Integer Offset in Milliseconds )"<<endl;
      cout<<" For example currentTreeConv( 10, -5, 0 )"<<endl;
      return kFALSE;
   } // if( iFileIndex < 0 || kiNbFiles <= iFileIndex )


   Int_t   iIndexVoltage = 0;
   Int_t   iIndexCurrent = 1;
   Int_t   iIndexPower   = 2;
   Int_t   iIndexStatus  = 3;
   Int_t   iNbFields     = 4;

   Rpc_HV  stRpcValArray[kiNbRpc];
   Pmt_HV  stPmtValArray[kiNbPmt];
   /*
   Rpc_HV  stTrdValArray[kiNbTrd];
   * */

   // Looking for RPC currents
   TString sFileName[kiNbBoardsUsed];
   ifstream * fInputFile[kiNbBoardsUsed];
   std::streampos fSizeFile[kiNbBoardsUsed];
   
   for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
   {
      sFileName[iBoardIndex] = Form("./csv/Log_Slot%02d_%s_%s.csv",
                                    kiSlotIndex[iBoardIndex], 
                                    ksFilesDay[iFileIndex].Data(), 
                                    ksFilesHour[iFileIndex].Data());
//            cout<<iBoardIndex<<" . "<<kiSlotIndex[iBoardIndex]<<" . "
//                <<ksFilesDay[iFileIndex]<<" . "<<ksFilesHour[iFileIndex]<<" . "<<sFileName[iBoardIndex]<<endl;
      fInputFile[iBoardIndex] = new ifstream(sFileName[iBoardIndex], ios::in);

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
   TFile * fOutputFile = new TFile(sOutputFilename, "RECREATE",
               "TTree file with HV slow control data for CERN SPS February 2015", 9);
   sOutputFilename += ":/";

   if( fOutputFile->IsZombie() )
   {
      cout<<" Error opening file for TTree output!!!"<<endl;
      fOutputFile->Close();
      return kFALSE;
   } // if (fOutputFile->IsZombie())

   gDirectory->Cd(sOutputFilename);
   
   TTree * fOutputTree = new TTree("CaenHV", "CERN SPS February 2015 HV slow control");
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

   // Add TRD data as Tree branches
/*
   TBranch *branchTrd[kiNbTrd];
   for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
      branchTrd[iTrdIndex] = fOutputTree->Branch( sChNameTrd[iTrdIndex],
                                                  &(stTrdValArray[iTrdIndex]),
"iTimeSec/I:iTimeMilliSec:dVoltageNeg/D:dCurrentNeg:iPowerNeg/I:iStatusNeg:dVoltagePos/D:dCurrentPos:iPowerPos/I:iStatusPos" );
*/

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
      return kFALSE;

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

   // Clear TRD objects
/*
   for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
      stTrdValArray[iTrdIndex].Clear();
*/

   // Print RPC data before first line
   for( Int_t iRpcIndex = 0; iRpcIndex < kiNbRpc; iRpcIndex++)
      stRpcValArray[iRpcIndex].Print();

   // Print PMT data before first line
   for( Int_t iPmtIndex = 0; iPmtIndex < kiNbPmt; iPmtIndex++)
      stPmtValArray[iPmtIndex].Print();

   // Print TRD data before first line
/*
   for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
      stTrdValArray[iTrdIndex].Print();
*/

   // TTree configuration
   /*
   fOutputTree->SetAutoFlush(500);
//   fOutputTree->SetAutoFlush(-500000);
   Long64_t autoflush = fOutputTree->GetAutoFlush();
   Long64_t autosave  = fOutputTree->GetAutoSave();
   cout<<"Tree config autoflush: "<<autoflush<<endl;
   cout<<"Tree config autosave:  "<<autosave<<endl;
*/

   Int_t iNbLinesRead = 0;
   Int_t iNewDay = 0;
   Int_t iFirstHour = -1; 
   Int_t iLines =0;
   // Goes through all lines
//   while( !fInputFile[0]->eof() && iLines < 1000)
   while( !fInputFile[0]->eof() )
   {
      iLines++;
      Bool_t bEndReachedInOne = kFALSE;
      for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
      {
         // Read a line of the HV file and set the iterator automatically to the next line
         sLine[iBoardIndex].ReadLine( *(fInputFile[iBoardIndex]) , kFALSE);
         // ROOT function as slowing down after ~100 lines
         // Try C++ variant
/*
         std::string line;
         std::getline( *(fInputFile[iBoardIndex]), line);
         sLine[iBoardIndex] = line;
*/

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

      // Extract TRD data as numbers
/*
      for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
      {
         TObjString* subString;

         // Take readout time from negative voltage board
         subString = (TObjString*)pValuesArray[ iSlotTrdNeg[iTrdIndex] ]->At(0);
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
         stTrdValArray[iTrdIndex].iTimeSec      = tTempTime.GetSec(); //Extract seconds
         stTrdValArray[iTrdIndex].iTimeMilliSec = tTempTime.GetNanoSec()/1000000; //Extract milliseconds

         // Go for negative HV values
         subString = (TObjString*)pValuesArray[ iSlotTrdNeg[iTrdIndex] ]->At(
                                    1 + iNbFields*iChIdxTrdNeg[iTrdIndex] + iIndexVoltage);
         stTrdValArray[iTrdIndex].dVoltageNeg   = (subString->GetString()).Atof(); // HV
         subString = (TObjString*)pValuesArray[ iSlotTrdNeg[iTrdIndex] ]->At(
                                    1 + iNbFields*iChIdxTrdNeg[iTrdIndex] + iIndexCurrent);
         stTrdValArray[iTrdIndex].dCurrentNeg   = (subString->GetString()).Atof(); // Current
         subString = (TObjString*)pValuesArray[ iSlotTrdNeg[iTrdIndex] ]->At(
                                    1 + iNbFields*iChIdxTrdNeg[iTrdIndex] + iIndexPower);
         stTrdValArray[iTrdIndex].iPowerNeg     = (subString->GetString()).Atoi(); // Power status (ON/OFF)
         subString = (TObjString*)pValuesArray[ iSlotTrdNeg[iTrdIndex] ]->At(
                                    1 + iNbFields*iChIdxTrdNeg[iTrdIndex] + iIndexStatus);
         stTrdValArray[iTrdIndex].iStatusNeg    = (subString->GetString()).Atoi(); // Channel status (OK, Tripped, Ramping, ...)

         // Go for Positive HV values
         subString = (TObjString*)pValuesArray[ iSlotTrdPos[iTrdIndex] ]->At(
                                    1 + iNbFields*iChIdxTrdPos[iTrdIndex] + iIndexVoltage);
         stTrdValArray[iTrdIndex].dVoltagePos   = (subString->GetString()).Atof(); // HV
         subString = (TObjString*)pValuesArray[ iSlotTrdPos[iTrdIndex] ]->At(
                                    1 + iNbFields*iChIdxTrdPos[iTrdIndex] + iIndexCurrent);
         stTrdValArray[iTrdIndex].dCurrentPos   = (subString->GetString()).Atof(); // Current
         subString = (TObjString*)pValuesArray[ iSlotTrdPos[iTrdIndex] ]->At(
                                    1 + iNbFields*iChIdxTrdPos[iTrdIndex] + iIndexPower);
         stTrdValArray[iTrdIndex].iPowerPos     = (subString->GetString()).Atoi(); // Power status (ON/OFF)
         subString = (TObjString*)pValuesArray[ iSlotTrdPos[iTrdIndex] ]->At(
                                    1 + iNbFields*iChIdxTrdPos[iTrdIndex] + iIndexStatus);
         stTrdValArray[iTrdIndex].iStatusPos    = (subString->GetString()).Atoi(); // Channel status (OK, Tripped, Ramping, ...)

      } // for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
*/

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

   // Print TRD data from last line
/*
   for( Int_t iTrdIndex = 0; iTrdIndex < kiNbTrd; iTrdIndex++)
      stTrdValArray[iTrdIndex].Print();
*/

   // Close the fstream objects (this also delete them)
   for( Int_t iBoardIndex = 0; iBoardIndex < kiNbBoardsUsed; iBoardIndex++)
      fInputFile[iBoardIndex]->close();
   
   fOutputFile->Write("",TObject::kOverwrite);
   fOutputFile->Close();

   return kTRUE;
}
