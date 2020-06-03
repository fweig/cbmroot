

void merge_rate()
{
   TChain * inputChain = new TChain("cbmsim");
   
   inputChain->Add("out/unpack_rates_2015_11_26_18_21.out.root");
   inputChain->Add("out/unpack_rates_2015_11_26_22_22.out.root");
   inputChain->Add("out/unpack_rates_2015_11_27_11_42.out.root");
   inputChain->Add("out/unpack_rates_2015_11_27_16_45.out.root");
   inputChain->Add("out/unpack_rates_2015_11_27_16_47.out.root");
   inputChain->Add("out/unpack_rates_2015_11_28_08_07.out.root");
   inputChain->Add("out/unpack_rates_2015_11_28_09_01__0001.out.root");
   inputChain->Add("out/unpack_rates_2015_11_28_09_01__0002.out.root");
   inputChain->Add("out/unpack_rates_2015_11_28_09_01__0003.out.root");
   inputChain->Add("out/unpack_rates_2015_11_28_09_01__0004.out.root");
   inputChain->Add("out/unpack_rates_2015_11_28_09_01__0005.out.root");
   inputChain->Add("out/unpack_rates_2015_11_29_19_26__0006.out.root");
   inputChain->Add("out/unpack_rates_2015_11_29_19_26__0007.out.root");
   inputChain->Add("out/unpack_rates_2015_11_29_19_26__0008.out.root");
   inputChain->Add("out/unpack_rates_2015_11_29_19_26__0009.out.root");
   inputChain->Add("out/unpack_rates_2015_11_29_19_26__0010.out.root");
   
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
*/
   TClonesArray* tArray = new TClonesArray("TTofCalibScaler");
   inputChain->SetBranchAddress("TofCalibScaler", &tArray);
   TTofCalibScaler* tCalibTriglog;

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
