

void merge_rate()
{
   TChain * inputChain = new TChain("cbmsim");
   inputChain->Add("out/unpack_rates_2015_02_27_2311.out.root");
   inputChain->Add("out/unpack_rates_2015_02_28_0928.out.root");
   inputChain->Add("out/unpack_2015_02_28_21_34.out.root");
   inputChain->Add("out/unpack_2015_03_01_09_47.out.root");
   inputChain->Add("out/unpack_2015_03_01_13_40.out.root");
   inputChain->Add("out/unpack_2015_03_01_04_09.out.root");
   inputChain->Add("out/unpack_2015_03_01_16_14.out.root");
   inputChain->Add("out/unpack_2015_03_01_19_06.out.root");
   inputChain->Add("out/unpack_2015_03_02_11_02.out.root");
   inputChain->Add("out/unpack_2015_03_02_14_17.out.root");
   inputChain->Add("out/unpack_2015_03_02_14_57.out.root");
   inputChain->Add("out/unpack_2015_03_02_17_37_freeStr.out.root");
   inputChain->Add("out/unpack_2015_03_02_23_16_freeStr_noiserun.out.root");
   inputChain->Add("out/unpack_2015_03_02_23_41_freeStr.out.root");
   inputChain->Add("out/unpack_2015_03_03_01_53.out.root");
   inputChain->Add("out/unpack_2015_03_03_14_23.out.root");
   inputChain->Add("out/unpack_2015_03_03_17_25.out.root");
   inputChain->Add("out/unpack_2015_03_03_22_15.out.root");
   inputChain->Add("out/unpack_2015_03_04_02_51.out.root");
   inputChain->Add("out/unpack_2015_03_04_17_17.out.root");
/*
   TBranch* tBranchTrlo = tTree->GetBranch("TofTriglog");
   if(!tBranchTrlo)
   {
    cout<<"Branch 'TofTriglog' not found in output tree. Abort macro execution."<<endl;
    return;
   } // if(!tBranch)
*/
   /*
   TBranch* tBranchScal = inputChain->GetBranch("TofCalibScaler");
   if(!tBranchScal)
   {
    cout<<"Branch 'TofCalibScaler' not found in output tree. Abort macro execution."<<endl;
    return;
   } // if(!tBranchScal)

   TClonesArray* tArray = new TClonesArray("TTofCalibScaler");
   tBranchScal->SetAddress(&tArray);
   TTofCalibScaler* tCalibTriglog;
*/
   TClonesArray* tArray = new TClonesArray("TTofCalibScaler");
   inputChain->SetBranchAddress("TofCalibScaler", &tArray);

   //Create a new file + a clone of old tree in new file
   TFile *outfile = new TFile("out/unpack_rate_all.out.root","recreate");
   TTree *newtree = inputChain->CloneTree(0);

   // Loop over measurements
   Long64_t lBranchEntries = inputChain->GetEntries();
   cout << lBranchEntries << " rate measurements in total"<<endl;
   for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
   {
      if(0 == lBranchEntry%10000 && 0 < lBranchEntry)
         cout<<"Event "<<lBranchEntry<<" / "<< lBranchEntries <<" done!"<<endl;
      tArray->Clear("C");

      inputChain->GetEntry(lBranchEntry);

      // Board 0 is TRIGLOG for trigger
      tCalibTriglog = (TTofCalibScaler*)tArray->At(0);

      if( NULL == tCalibTriglog)
      {
         cout<<" Missing Triglog calib scaler in entry "<< lBranchEntry << endl;
         continue;
      }

      if( 0.1 < tCalibTriglog->GetTimeToFirst() )
         newtree->Fill();
   } // for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)

   newtree->AutoSave();
   cout << newtree->GetEntries() << " rate measurements saved in total"<<endl;
   delete inputChain;
   delete outfile;
}
