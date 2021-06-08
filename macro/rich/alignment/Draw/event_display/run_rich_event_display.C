/* Copyright (C) 2012-2019 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Jordan Bendarouach [committer] */

/** run_rich_event_display.C
 * @author Semen Lebedev <s.lebedev@gsi.de>
 * @since 2012
 * @version 1.0
 **/

void run_rich_event_display()
{
  //  TString outDir = "/data/Sim_Outputs/Event_Display/plusY/";
  TString outDir = "/data/Sim_Outputs/Position/plusY/";
  TString name   = "setup_v17a_1e_3mrad_plusY_";

  TString paramFile = outDir + name + "param.root";
  TString mcFile    = outDir + name + "mc.root";
  TString recoFile  = outDir + name + "reco.root";
  TString outFile   = outDir + "ed.root";

  FairRunAna* run = new FairRunAna();
  run->SetInputFile(mcFile);
  run->AddFriend(recoFile);
  run->SetOutputFile(outFile);

  CbmRichEventDisplay* ed = new CbmRichEventDisplay();
  ed->SetDrawRings(true);
  ed->SetDrawHits(true);
  ed->SetDrawPoints(false);  //
  ed->SetDrawProjections(true);
  string dirout = string(outDir.Data()) + "events/";
  ed->SetOutputDir(dirout);
  run->AddTask(ed);

  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(paramFile.Data());
  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();

  run->Init();
  cout << "Starting run" << endl;
  run->Run(40, 80);
}
