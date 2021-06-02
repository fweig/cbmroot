/** @file run_rich_event_display.C
  * @copyright Copyright (C) 2012-2018 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev, Semen Lebedev [committer] **/

/** run_rich_event_display.C
 * @author Semen Lebedev <s.lebedev@gsi.de>
 * @since 2012
 * @version 1.0
 **/

void run_rich_event_display()
{

  TString outDir   = "/Users/slebedev/Development/cbm/data/sim/rich/urqmdtest/";
  TString parFile  = outDir + "param.00000.root";
  TString mcFile   = outDir + "mc.00000.root";
  TString recoFile = outDir + "reco.00000.root";
  TString outFile  = outDir + "ed.00000.root";

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(mcFile);
  inputSource->AddFriend(recoFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);

  TList* parFileList = new TList();

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile);
  run->AddTask(mcManager);


  CbmRichEventDisplay* ed = new CbmRichEventDisplay();
  ed->SetDrawRings(true);
  ed->SetDrawHits(true);
  ed->SetDrawPoints(false);
  ed->SetDrawProjections(true);
  run->AddTask(ed);

  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }

  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, 20);
}
