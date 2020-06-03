//Copy from CbmHit
// otherwise it doesn't work with ROOT5
/*
enum HitType {
        kHIT,
        kPIXELHIT,
        kSTRIPHIT,
        kSTSHIT,
        kMVDHIT,
        kRICHHIT,
        kMUCHPIXELHIT,
        kMUCHSTRAWHIT,
        kTRDHIT,
        kTOFHIT,
        kECALHIT
};
*/

enum ConstructorType {
 defaultConstructor,
 standardConstructorFull,
 standardConstructor
};
  
void CreateTestFile(TString filename, ConstructorType constructor)
{
  TTree* outTree =new TTree("cbmsim", "/cbmout", 99);
  TClonesArray* hit = new TClonesArray("CbmHit");
  
  outTree->Branch("CbmHit", &hit, 500, 99);
  
  for (Int_t i=0; i<100; ++i) {
    switch(constructor) {
    case(defaultConstructor): 
      new((*hit)[i]) CbmHit();
      break;
    case(standardConstructor): 
      new((*hit)[i]) CbmHit(kHIT, 0., 0., -1, -1);
      break;
    case(standardConstructorFull): 
      new((*hit)[i]) CbmHit(kHIT, 0., 0., -1, -1, -2., -2.);
      break;
    default:
      new((*hit)[i]) CbmHit();
    }
  }
  outTree->Fill();
  
  TFile* outFile = TFile::Open(filename, "recreate");
  outTree->Write();
  outFile->Close();
  
  delete outFile;
  delete hit;
  delete outTree;
}

Bool_t CompareDouble(Double_t found, Double_t expected,
		     TString text)
{  
  if (TMath::Abs(found - expected) > 0.0001) {
    cout << text << found << ", "
	 << expected <<endl;	
    return kFALSE;
  }
  return kTRUE;
}

Bool_t CompareInt(Int_t found, Int_t expected,
		     TString text)
{  
  if (found != expected) {
    cout << text << found << ", "
	 << expected <<endl;	
    return kFALSE;
  }
  return kTRUE;
}


Bool_t TestTestFile(TString filename, Int_t* intValues,
                  Double_t* doubleValues)
{  
  // Open the file 
  TFile* testFile = TFile::Open(filename, "update");
  
  // Get the TTree from file
  TTree *t2 = (TTree*)testFile->Get("cbmsim");
  
  // Connect a TClonesArray of class CbmHit with the tree    
  TClonesArray *hits = new TClonesArray("CbmHit");
  t2->SetBranchAddress("CbmHit",&hits);
  
  CbmHit *testHit = nullptr;

  Bool_t finalResult = kTRUE;
  // Loop over all entries in the TClonesArray
  Int_t nentries = t2->GetEntries();
  cout << "Number of events: " << nentries << endl;
  for (Int_t i=0;i<nentries;i++) {
    t2->GetEntry(i); 
    
    // Loop over all hits in one event
    Int_t nHits = hits->GetEntriesFast();
    cout << "Number of hits in event " << i << ": " << nHits << endl;
    for (Int_t iHits=0;iHits< nHits ; iHits++) {
      testHit = (CbmHit*) hits->At(iHits);

      finalResult = finalResult &&
	CompareDouble(testHit->GetZ(), doubleValues[0],
		      "z position different(found, expected): ");
      finalResult = finalResult &&
	CompareDouble(testHit->GetDz(), doubleValues[1],
		      "z position error different(found, expected): ");
      finalResult = finalResult &&
	CompareDouble(testHit->GetTime(), doubleValues[2],
		      "time different(found, expected): ");
      finalResult = finalResult &&
	CompareDouble(testHit->GetTimeError(), doubleValues[3],
		      "time error different(found, expected): ");
      finalResult = finalResult &&
	CompareInt(testHit->GetType(), intValues[0],
		   "hit type different(found, expected): ");
      finalResult = finalResult &&
	CompareInt(testHit->GetRefId(), intValues[1],
		   "refId different(found, expected): ");
      finalResult = finalResult &&
	CompareInt(testHit->GetAddress(), intValues[2],
		   "address different(found, expected): ");
    }
  }

  // Close the file 
  testFile->Close();

  delete testFile; 
  delete hits;

  if (finalResult) {
    cout << "Test passed" << endl;
    return kTRUE;
  } else {
    cout << "Test failed" << endl;
    return kFALSE;
  }
}

  
int TestCbmHit()
{
  // expected values for type, refId, address
  Int_t intValues[3] = {0, -1, -1};

  // expected values for z, dz, time, time error
  Double_t doubleValues[4] = {0., 0., -1., -1.};

  Bool_t result=kTRUE;
  
  TString filename="test1.root";
  CreateTestFile(filename, defaultConstructor);
  result = result && TestTestFile(filename, intValues, doubleValues);

  filename="test2.root";
  CreateTestFile(filename, standardConstructor);
  result = result && TestTestFile(filename, intValues, doubleValues);
  
  filename="test3.root";
  doubleValues[2]=-2.;
  doubleValues[3]=-2.;
  CreateTestFile(filename, standardConstructorFull);
  result = result && TestTestFile(filename, intValues, doubleValues);
  
  if (result) return 0;

  return 1;
}

  
