int merge(char* input_list="list.txt", char *output_file="merged.root"){

  /// set global debug level
  // gErrorIgnoreLevel = kInfo, kWarning, kError, kFatal;
  gErrorIgnoreLevel = kError;

  /// load special libraries if needed
  // gSystem->Load("libWHATEVER.so");

  /// switch on if you want to merge files from grid
  //  TGrid::Connect("alien://",0,0,"t");

  char filename[200];
  ifstream infile(input_list);

  TFileMerger *FM = new TFileMerger();
  FM->OutputFile(output_file);

  /// add files to merger
  while ( infile >> filename ){
    FM->AddFile(filename);
    cout << "Adding " << filename << "..." << endl;
  }

  FM->SetFastMethod(kFALSE);
  cout << "Merging into " << output_file << "..." << endl;

  /// Partial merging
  /// NOTE: somehow partial merging throws a segmentation error after everything
  ///       was merge properly, you can savely ignore it (this results in job status FAILED)
  if(1) {
    /// selection/rejection of objects
    FM->AddObjectNames("cbmout BranchList FileHeader cbmsim TimeBasedBranchList");

    // Must add new merging flag on top of the the default ones
    Int_t mode  =
      TFileMerger::kAllIncremental | TFileMerger::kSkipListed;     /// skip ObjectNames
    //  TFileMerger::kAllIncremental | TFileMerger::kOnlyListed; /// merge only ObjectNames
    FM->PartialMerge(mode);
  }
  else {
    /// default merging
    FM->Merge();
  }

  cout << "Done!!" << endl;
}
