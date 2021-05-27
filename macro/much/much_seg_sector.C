/**
 * Performs segmentation of Much stations based on
 * user specified conditions.
 *
 * @author M.Ryzhinskiy m.ryzhinskiy@gsi.de
 * @param mcFile       Input transport file name
 * @param inDigiFile   Input file name containing initial segmentation parameters
 * @param outDigiFile  Output file name containing segmentation parameters

flag =0 for sis 100 geometry
flag = 1 for mini-cbm geometry
 */
void much_seg_sector(TString mcFile = "", TString inDigiFile = "", TString outDigiFile = "", Int_t flag = 0,
                     TString setupName = "much_test")

{
  // ========================================================================
  //          Adjust this part according to your requirements


  if (mcFile == "") { mcFile = "data/" + setupName + ".tra.root"; }
  //  if (inDigiFile == "") {
  if (inDigiFile == "") {
    TString srcDir = gSystem->Getenv("VMCWORKDIR");
    inDigiFile     = srcDir + "/macro/much/data/much_digi_sector_4station.seg";
  }

  if (outDigiFile == "") { outDigiFile = "data/much_digi_sector.root"; }

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  // Dummy ROOT file (needed as an output)
  TString outFile = "data/dummy.root";
  TString parFile = "data/" + setupName + ".par.root";

  // ------------------------------------------------------------------------

  // -----   Analysis run   -------------------------------------------------
  FairRunAna* fRun = new FairRunAna();
  fRun->SetInputFile(mcFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb       = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parFile);
  rtdb->setFirstInput(parIo1);
  // ------------------------------------------------------------------------

  // -----  Segmentation task  ----------------------------------------------
  CbmMuchSegmentSector* seg = new CbmMuchSegmentSector(inDigiFile, outDigiFile, flag);
  // seg->DebugSwitchOn();
  fRun->AddTask(seg);
  // ------------------------------------------------------------------------

  // Run segmentation
  fRun->Init();

  cout << " Test passed" << endl;
  cout << " All ok " << endl;

  RemoveGeoManager();
}
