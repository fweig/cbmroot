void eventDisplay()
{
  TString dataDir   = "/home/jordan/Desktop/Thesis_Results/HERA-B_Ana/4mrad_X_2mrad_Y_5_3/New/";
  TString setupName = "setup_4mrad_X_2mrad_Y_5_3";
  TString InputFile = dataDir + setupName + "_mc.root";
  TString ParFile   = dataDir + setupName + "_param.root";

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* fRun = new FairRunAna();

  fRun->SetInputFile(InputFile.Data());
  fRun->SetOutputFile(dataDir + setupName + "_ed.root");

  FairRuntimeDb* rtdb          = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(ParFile.Data());
  rtdb->setFirstInput(parInput1);

  FairEventManager* fMan = new FairEventManager();
  FairMCTracks* Track    = new FairMCTracks("Monte-Carlo Tracks");

  //  FairMCPointDraw  *MvdPoint      = new FairMCPointDraw ("MvdPoint",      kBlack,  kFullSquare);
  FairMCPointDraw* StsPoint      = new FairMCPointDraw("StsPoint", kBlue, kFullSquare);
  FairMCPointDraw* RichPoint     = new FairMCPointDraw("RichPoint", kOrange, kFullSquare);
  FairMCPointDraw* RefPlanePoint = new FairMCPointDraw("RefPlanePoint", kPink, kFullSquare);
  //  FairMCPointDraw  *TrdPoint      = new FairMCPointDraw ("TrdPoint",      kCyan,   kFullSquare);
  //  FairMCPointDraw  *TofPoint      = new FairMCPointDraw ("TofPoint",      kGreen,  kFullSquare);
  //  FairMCPointDraw  *EcalPoint     = new FairMCPointDraw ("EcalPoint",     kYellow, kFullSquare);

  fMan->AddTask(Track);

  //  fMan->AddTask(MvdPoint);
  fMan->AddTask(StsPoint);
  fMan->AddTask(RichPoint);
  fMan->AddTask(RefPlanePoint);
  //  fMan->AddTask(TrdPoint);
  //  fMan->AddTask(TofPoint);
  //  fMan->AddTask(EcalPoint);

  //  fMan->Init(1,4,10000);
  fMan->Init(1, 3, 1000);  // make STS visible by default
  //  fMan->Init(1,6,10000);  // make MVD visible by default
}
