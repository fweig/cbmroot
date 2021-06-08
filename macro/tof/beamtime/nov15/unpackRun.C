/* Copyright (C) 2016 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void unpackRun(char* cFileId = "CbmTofSps_01Dec0206")
{
  FairRunOnline* run = new FairRunOnline();
  gROOT->LoadMacro("setup_unpack.C");
  cout << "Process FileId  " << cFileId << endl;
  Char_t* cCom = Form("setup_unpack(1,\"%s\")", cFileId);
  cout << "Processline " << cCom << endl;
  gInterpreter->ProcessLine(cCom);
  run->Run(1000000000);
  run->Finish();
}
