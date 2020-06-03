
#include "TString.h"

#include "FileListDef.h"

Bool_t mergeTrees( )
{
   TChain chainTreeFiles( "CaenHV" );
   
   for( Int_t iFileIndex = 0; iFileIndex < kiNbFiles; iFileIndex++)
      chainTreeFiles.Add( Form("LogHv_%s_%s.root", 
                                    ksFilesDay[iFileIndex].Data(), 
                                    ksFilesHour[iFileIndex].Data()) );
cout<<"test 1"<<endl;
   // Prepare the output fille where the full Tree will be stored
   TString sOutputFilename = "LogHv_Full_CernFeb15.root";

   chainTreeFiles.Merge(sOutputFilename);

   return kTRUE;
}
