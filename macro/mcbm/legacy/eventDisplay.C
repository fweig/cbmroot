//void eventDisplay(const char* setup = "sis18_mcbm_25deg_long")
void eventDisplay(const char* setup = "mcbm_beam_2019_03")
// void eventDisplay(const char* setup = "mcbm_beam_2018_11")
// void eventDisplay(const char* setup = "sis18_mcbm_25deg_cosmic_2018")
{

  TString dataDir   = "data/";
  TString InputFile = dataDir + setup + "_test.tra.root";
  TString ParFile   = dataDir + setup + "_test.par.root";

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* fRun = new FairRunAna();

  fRun->SetInputFile(InputFile.Data());
  fRun->SetOutputFile(dataDir + setup + "_test.root");

  FairRuntimeDb* rtdb          = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(ParFile.Data());
  rtdb->setFirstInput(parInput1);

  FairEventManager* fMan = new FairEventManager();
  FairMCTracks* Track    = new FairMCTracks("Monte-Carlo Tracks");

  FairMCPointDraw* MvdPoint =
    new FairMCPointDraw("MvdPoint", kBlack, kFullSquare);
  FairMCPointDraw* StsPoint =
    new FairMCPointDraw("StsPoint", kGreen, kFullSquare);
  FairMCPointDraw* MuchPoint =
    new FairMCPointDraw("MuchPoint", kOrange, kFullSquare);
  FairMCPointDraw* RichPoint =
    new FairMCPointDraw("RichPoint", kGreen, kFullSquare);
  FairMCPointDraw* TrdPoint =
    new FairMCPointDraw("TrdPoint", kBlue, kFullSquare);
  FairMCPointDraw* TofPoint =
    new FairMCPointDraw("TofPoint", kRed, kFullSquare);
  FairMCPointDraw* EcalPoint =
    new FairMCPointDraw("EcalPoint", kYellow, kFullSquare);
  FairMCPointDraw* RefPlanePoint =
    new FairMCPointDraw("RefPlanePoint", kPink, kFullSquare);

  fMan->AddTask(Track);

  fMan->AddTask(MvdPoint);
  fMan->AddTask(StsPoint);
  fMan->AddTask(MuchPoint);
  fMan->AddTask(RichPoint);
  fMan->AddTask(TrdPoint);
  fMan->AddTask(TofPoint);
  fMan->AddTask(EcalPoint);
  fMan->AddTask(RefPlanePoint);


  //  fMan->Init(1,4,10000);
  //  fMan->Init(1,5,10000);  // make STS visible by default
  //  fMan->Init(1,6,10000);  // make MVD visible by default
  fMan->Init(1, 7, 10000);  // make MVD visible by default
}
