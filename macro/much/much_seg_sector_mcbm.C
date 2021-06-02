/** @file much_seg_sector_mcbm.C
  * @copyright Copyright (C) 2020 Variable Energy Cyclotron Centre, Kolkata
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors M.Ryzhinskiy m.ryzhinskiy@gsi.de, Vikas Singhal [committer] **/

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
void much_seg_sector_mcbm(TString mcFile = "", TString inDigiFile = "", TString outDigiFile = "", Int_t flag = 1)
{
  // ========================================================================
  //          Adjust this part according to your requirements


  if (mcFile == "") {
    //mcFile = "data/mc.root";
    mcFile = "data/sis18_mcbm_25deg_long_test.tra.root";
  }
  //  if (inDigiFile == "") {
  if (inDigiFile == "") {
    TString srcDir = gSystem->Getenv("VMCWORKDIR");
    inDigiFile     = "data/much_digi_sector_mcbm.seg";
    //inDigiFile = srcDir + "/macro/much/data/much_digi_sector_4station.seg";
  }

  if (outDigiFile == "") {
    //outDigiFile = "data/much_digi_sector_mcbm.root";
    outDigiFile = "data/much_digi_sector.root";
  }

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  // Dummy ROOT file (needed as an output)
  TString outFile = "data/dummy.root";
  TString parFile = "data/sis18_mcbm_25deg_long_test.par.root";
  //  TString parFile = "data/params.root";

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
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
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
