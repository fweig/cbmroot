// Transform pair tdcId/ch into some unique ID [from 0 to 255]
// Size of the matrix is fixed to 16*16
UInt_t tdcAndChToID(UInt_t tdcId, UInt_t ch)
{
   UInt_t tdcN = 16;
   switch (tdcId) {
   case 10: tdcN=0; break;
   case 11: tdcN=1; break;
   case 12: tdcN=2; break;
   case 13: tdcN=3; break;
   case 20: tdcN=4; break;
   case 21: tdcN=5; break;
   case 22: tdcN=6; break;
   case 23: tdcN=7; break;
   case 50: tdcN=8; break;
   case 51: tdcN=9; break;
   case 52: tdcN=10; break;
   case 53: tdcN=11; break;
   case 60: tdcN=12; break;
   case 61: tdcN=13; break;
   case 62: tdcN=14; break;
   case 63: tdcN=15; break;
   }
   return tdcN*16+ch;
}

// 'b' is the reference channel
// 'a' is the measured channel
// a>b
// in the input data TDCp_chq_TDCr_chs
// p/q code 'b' and r/s code 'a', so first comes reference and then measured channels

void buildDeltaTable(TString filename="inputData.txt")
{
   // Open input file
   FILE* inFile = fopen(filename.Data(), "r");
   if (!inFile) {
      printf("Failed to open file %s. Abort.\n", filename.Data());
      return 1;
   }

   // Create new ROOT file to work inside it
   TFile* rootFile = new TFile("alphas.root", "RECREATE");

   // For each pair of channels (256*256) prepare histograms
   // Also create and zero 'filled' flags
   TH1F* hAlpha[256][256];
   Bool_t alphaOriginalFilled[256][256];
   TString histoName;
   for (UInt_t b=0; b<255; b++) {
      for (UInt_t a=b+1; a<256; a++) {
         histoName.Form("ID1_%d_ID2_%d", b, a);
         hAlpha[b][a] = new TH1F(histoName, histoName, 600, -30., 30.);
         alphaOriginalFilled[b][a] = kFALSE;
      }
   }

   // Read input table into the alphaOriginal 2D array
   UInt_t refId, mesId;
   UInt_t tdcId1, tdcId2, ch1, ch2;
   UInt_t maxBinNum;
   Float_t hmean, hmax, devia;
   Float_t alphaOriginal[256][256];
   int getLineRes;
   do {
      getLineRes = fscanf(inFile, "LeadingEdgeDiff_TDC%d_ch%d_TDC%d_ch%d\t%d\t%f\t%f\t%f\n", &tdcId1, &ch1, &tdcId2, &ch2, &maxBinNum, &hmean, &hmax, &devia);
      //printf ("TDC %d ch %d TDC %d ch %d : %d %f %f %f\n", tdcId1, ch1, tdcId2, ch2, maxBinNum, hmean, hmax, devia);
      refId = tdcAndChToID(tdcId1, ch1);
      mesId = tdcAndChToID(tdcId2, ch2);
      alphaOriginal[refId][mesId] = hmax;
      alphaOriginalFilled[refId][mesId] = kTRUE;
   }
   while (getLineRes == 8);

   // Fill the distributions
   for (UInt_t b=0; b<255; b++) {
      for (UInt_t a=b+1; a<256; a++) {

         // Loop over 'C' values inside a certain pair of channels
         for (UInt_t c=0; c<256; c++) {
            if (c==a || c==b) continue; // skip cause it has no sense

            if (c<a && c<b) { // case 1
               if (alphaOriginalFilled[c][a] && alphaOriginalFilled[c][b]) {
                  hAlpha[b][a]->Fill(alphaOriginal[c][a] - alphaOriginal[c][b]);
               }
            } else if (c<a && c>b) { // case 2
               if (alphaOriginalFilled[c][a] && alphaOriginalFilled[b][c]) {
                  hAlpha[b][a]->Fill(alphaOriginal[c][a] + alphaOriginal[b][c]);
               }
            } else if (c>a && c>b) { // case 3
               if (alphaOriginalFilled[a][c] && alphaOriginalFilled[b][c]) {
                  hAlpha[b][a]->Fill(-alphaOriginal[a][c] + alphaOriginal[b][c]);
               }
            } else {
            }
         }

      }
   }

   // Write generated histograms into the output ROOT file
   UInt_t b=0;
   //for (UInt_t b=0; b<255; b++)
   {
      for (UInt_t a=b+1; a<256; a++)
      {
         hAlpha[b][a]->Write();
      }
   }

   // Finalize
   fclose(inFile);
   rootFile->Close();
}
