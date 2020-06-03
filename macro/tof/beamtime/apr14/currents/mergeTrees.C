
#include "TString.h"

Bool_t mergeTrees( )
{

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

   TChain chainTreeFiles( "CaenHV" );
   
   for( Int_t iFileIndex = 0; iFileIndex < kiNbFiles; iFileIndex++)
      chainTreeFiles.Add( Form("LogHv_%s_%s.root", 
                                    ksFilesDay[iFileIndex].Data(), 
                                    ksFilesHour[iFileIndex].Data()) );
cout<<"test 1"<<endl;
   // Prepare the output fille where the full Tree will be stored
   TString sOutputFilename = "LogHv_Full_GsiApr14.root";
/*
   TFile * fOutputFile = new TFile(sOutputFilename, "RECREATE", "TTree file with HV slow control data for GSI April 2014", 9);
   sOutputFilename += ":/";

cout<<"test 2"<<endl;
   if( fOutputFile->IsZombie() )
   {
      cout<<" Error opening file for TTree output!!!"<<endl;
      fOutputFile->Close();
      return kFALSE;
   } // if (fOutputFile->IsZombie())

cout<<"test 3"<<endl;
   gDirectory->Cd(sOutputFilename);

   // If folders needed:
//   fOutputFile->mkdir("mydir")->cd();

cout<<"test 4"<<endl;
   chainTreeFiles.Merge(fOutputFile);

cout<<"test 5"<<endl;
   fOutputFile->Write("",TObject::kOverwrite);
   fOutputFile->Close();
*/

   chainTreeFiles.Merge(sOutputFilename);

   return kTRUE;
}
