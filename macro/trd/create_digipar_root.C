// --------------------------------------------------------------------------
//
// Macro to create digi parameters for the TRD
// This macro needs a geometry file with the TRD geometry only.
//
// The way how the pad layout looks like has to be implemented in a task called
// in this macro. CbmTrdCreateDigiPar is the implementation used here.
//
// F.Uhlig 07.05.2010
//
// --------------------------------------------------------------------------

void create_digipar_root(TString geoName = "trd_v13q",
                         Bool_t asicFASP = kTRUE) {
  TString inFile   = "data/test.mc." + geoName + ".root";
  TString geoFile  = "geofile_" + geoName + ".root";
  TString outFile  = "data/test.esd." + geoName + ".root";
  TString digiFile = geoName + ".par";  // Digi Parameter Output File

  FairRunAna* run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  run->SetGeomFile(geoFile);

  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo2->open(digiFile, "out");
  rtdb->setOutput(parIo2);

  CbmTrdParManager* trdParManager = new CbmTrdParManager(asicFASP);
  run->AddTask(trdParManager);

  rtdb->saveOutput();
  run->Init();
  rtdb->print();
  CbmTrdParSetDigi* digiPar =
    (CbmTrdParSetDigi*) rtdb->getContainer("CbmTrdParSetDigi");
  digiPar->setChanged();
  digiPar->setInputVersion(run->GetRunId(), 1);
  CbmTrdParSetAsic* asicPar =
    (CbmTrdParSetAsic*) rtdb->getContainer("CbmTrdParSetAsic");
  asicPar->setChanged();
  asicPar->setInputVersion(run->GetRunId(), 1);
  CbmTrdParSetGas* gasPar =
    (CbmTrdParSetGas*) rtdb->getContainer("CbmTrdParSetGas");
  gasPar->setChanged();
  gasPar->setInputVersion(run->GetRunId(), 1);
  CbmTrdParSetGain* gainPar =
    (CbmTrdParSetGain*) rtdb->getContainer("CbmTrdParSetGain");
  gainPar->setChanged();
  gainPar->setInputVersion(run->GetRunId(), 1);
  rtdb->print();
  rtdb->saveOutput();
}
