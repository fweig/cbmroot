void unpackCalib(char* cFileId="CbmTofSps_01Dec0206")
{
  FairRunOnline *run = new FairRunOnline ();
  gROOT->LoadMacro("setup_unpack.C");
  cout << "Process FileId  "<< cFileId <<endl;
  Char_t *cCom=Form("setup_unpack(0,\"%s\")",cFileId);
  cout << "Processline "<<cCom<<endl;
  gInterpreter->ProcessLine(cCom);
  run->Run(10000000);
  run->Finish();
}
