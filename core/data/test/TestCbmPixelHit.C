/** @file TestCbmPixelHit.C
  * @copyright Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

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

enum ConstructorType
{
  defaultConstructor,
  standardConstructor,
  standardConstructorTVector,
  standardConstructorTime,
  standardConstructorTVectorTime
};

void CreateTestFile(TString filename, ConstructorType constructor)
{
  TTree* outTree    = new TTree("cbmsim", "/cbmout", 99);
  TClonesArray* hit = new TClonesArray("CbmPixelHit");

  outTree->Branch("CbmPixelHit", &hit, 500, 99);

  for (Int_t i = 0; i < 100; ++i) {
    switch (constructor) {
      case (defaultConstructor): new ((*hit)[i]) CbmPixelHit(); break;
      case (standardConstructor): new ((*hit)[i]) CbmPixelHit(-1, 0., 0., 0., 0., 0., 0., 0., -1); break;
      case (standardConstructorTVector): {
        TVector3 pos(0., 0., 0.);
        TVector3 err(0., 0., 0.);
        new ((*hit)[i]) CbmPixelHit(-1, pos, err, 0., -1);
        break;
      }
      case (standardConstructorTime): new ((*hit)[i]) CbmPixelHit(-1, 0., 0., 0., 0., 0., 0., 0., -1, -2., -2.); break;
      case (standardConstructorTVectorTime): {
        TVector3 pos(0., 0., 0.);
        TVector3 err(0., 0., 0.);
        new ((*hit)[i]) CbmPixelHit(-1, pos, err, 0., -1, -2., -2.);
        break;
      }
      default: new ((*hit)[i]) CbmPixelHit();
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

Bool_t CompareDouble(Double_t found, Double_t expected, TString text)
{
  if (TMath::Abs(found - expected) > 0.0001) {
    cout << text << found << ", " << expected << endl;
    return kFALSE;
  }
  return kTRUE;
}

Bool_t CompareInt(Int_t found, Int_t expected, TString text)
{
  if (found != expected) {
    cout << text << found << ", " << expected << endl;
    return kFALSE;
  }
  return kTRUE;
}


Bool_t TestTestFile(TString filename, Int_t* intValues, Double_t* doubleValues)
{
  // Open the file
  TFile* testFile = TFile::Open(filename, "update");

  // Get the TTree from file
  TTree* t2 = (TTree*) testFile->Get("cbmsim");

  // Connect a TClonesArray of class CbmHit with the tree
  TClonesArray* hits = new TClonesArray("CbmPixelHit");
  t2->SetBranchAddress("CbmPixelHit", &hits);

  CbmPixelHit* testHit = nullptr;

  Bool_t finalResult = kTRUE;
  // Loop over all entries in the TClonesArray
  Int_t nentries = t2->GetEntries();
  cout << "Number of events: " << nentries << endl;
  for (Int_t i = 0; i < nentries; i++) {
    t2->GetEntry(i);

    // Loop over all hits in one event
    Int_t nHits = hits->GetEntriesFast();
    cout << "Number of hits in event " << i << ": " << nHits << endl;
    for (Int_t iHits = 0; iHits < nHits; iHits++) {
      testHit = (CbmPixelHit*) hits->At(iHits);

      // members from base class
      finalResult =
        finalResult && CompareDouble(testHit->GetZ(), doubleValues[2], "z position different(found, expected): ");
      finalResult =
        finalResult
        && CompareDouble(testHit->GetDz(), doubleValues[5], "z position error different(found, expected): ");
      finalResult =
        finalResult && CompareDouble(testHit->GetTime(), doubleValues[7], "time different(found, expected): ");
      finalResult =
        finalResult
        && CompareDouble(testHit->GetTimeError(), doubleValues[8], "time error different(found, expected): ");
      finalResult =
        finalResult && CompareInt(testHit->GetType(), intValues[0], "hit type different(found, expected): ");
      finalResult = finalResult && CompareInt(testHit->GetRefId(), intValues[1], "refId different(found, expected): ");
      finalResult =
        finalResult && CompareInt(testHit->GetAddress(), intValues[2], "address different(found, expected): ");

      // members from derrived class
      finalResult =
        finalResult && CompareDouble(testHit->GetX(), doubleValues[0], "x position different(found, expected): ");
      finalResult =
        finalResult && CompareDouble(testHit->GetY(), doubleValues[1], "y position different(found, expected): ");
      finalResult =
        finalResult
        && CompareDouble(testHit->GetDx(), doubleValues[3], "x position error different(found, expected): ");
      finalResult =
        finalResult
        && CompareDouble(testHit->GetDy(), doubleValues[4], "y position error different(found, expected): ");
      finalResult =
        finalResult
        && CompareDouble(testHit->GetDxy(), doubleValues[6], "xy correclation error different(found, expected): ");
    }
  }

  // Close the file
  testFile->Close();

  delete testFile;
  delete hits;

  if (finalResult) {
    cout << "Test passed" << endl;
    return kTRUE;
  }
  else {
    cout << "Test failed" << endl;
    return kFALSE;
  }
}


int TestCbmPixelHit()
{
  // expected values for type, refId, address
  Int_t intValues[3] = {kPIXELHIT, -1, -1};

  // expected values for x, y, z, dx, dy, dz, dxy, time, time error
  //                          x   y   z   dx  dy  dz  dxy  t    dt
  Double_t doubleValues[9] = {0., 0., 0., 0., 0., 0., 0., -1., -1.};

  Bool_t result = kTRUE;

  TString filename = "test1.root";
  CreateTestFile(filename, defaultConstructor);
  result = result && TestTestFile(filename, intValues, doubleValues);

  filename = "test2.root";
  CreateTestFile(filename, standardConstructor);
  result = result && TestTestFile(filename, intValues, doubleValues);

  filename = "test3.root";
  CreateTestFile(filename, standardConstructorTVector);
  result = result && TestTestFile(filename, intValues, doubleValues);

  doubleValues[7] = -2.;
  doubleValues[8] = -2.;
  filename        = "test4.root";
  CreateTestFile(filename, standardConstructorTime);
  result = result && TestTestFile(filename, intValues, doubleValues);

  filename = "test5.root";
  CreateTestFile(filename, standardConstructorTVectorTime);
  result = result && TestTestFile(filename, intValues, doubleValues);

  if (result) return 0;

  return 1;
}
