/** @file call_Tsa_client.C
  * @copyright Copyright (C) 2014 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Cyrano Bergmann [committer] **/

void call_Tsa_client(Int_t nMin = 1)
{
  Int_t nEvents      = nMin * 60 * 10;  //minutes * seconds * 10Hz TimeSlices
  FairRunOnline* run = new FairRunOnline();
  gROOT->ProcessLine(".x readTsa_server.C");
  run->Run(nEvents);
  //Int_t c;
  //std::cin >> c >> endl;
  //printf("to read additional n envents call\n  run->Run(n);\n");
}
