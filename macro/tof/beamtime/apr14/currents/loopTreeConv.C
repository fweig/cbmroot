

Bool_t loopTreeConv( Int_t iSecOffset = 0, Int_t iMilliSecOffset = 0 )
{
   const Int_t   kiNbFiles  = 5 + 12 + 8 + 14 + 15 + 1;

   for( Int_t iFileIndex = 0; iFileIndex < kiNbFiles; iFileIndex++)
      gROOT->ProcessLine( Form(".x currentTreeConv.C( %d, %d, %d)", iFileIndex, iSecOffset, iMilliSecOffset) );

   cout<<"Finished creating Root files, now merge them"<<endl;

   // For now offsets are not used, probably better to shift it to the previous loop
   gROOT->ProcessLine( ".x mergeTrees.C()" );

   return kTRUE;
}
