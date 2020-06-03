void eventDisplay(const char* setup = "sis100_electron")
{

  TString dataDir   = "data/";
  TString InputFile = dataDir + setup + "_test.mc.root";
  TString ParFile   = dataDir + setup + "_params.root";

  TString dataDir   = "/data/misalignment_correction/event_display/test/";
  TString numb = "00001";
  TString InputFile = dataDir + "mc." + numb + ".root";
  TString ParFile   = dataDir + "param." + numb + ".root";

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();

  fRun->SetInputFile(InputFile.Data());
  fRun->SetOutputFile(dataDir + setup + "_test.root");

  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(ParFile.Data());
  rtdb->setFirstInput(parInput1);

  FairEventManager *fMan  = new FairEventManager();
  FairMCTracks     *Track = new FairMCTracks ("Monte-Carlo Tracks");

  FairMCPointDraw  *MvdPoint      = new FairMCPointDraw ("MvdPoint",      kBlack,  kFullSquare);
  FairMCPointDraw  *StsPoint      = new FairMCPointDraw ("StsPoint",      kBlue,   kFullSquare);
  FairMCPointDraw  *RichPoint     = new FairMCPointDraw ("RichPoint",     kOrange, kFullSquare);
  FairMCPointDraw  *RefPlanePoint = new FairMCPointDraw ("RefPlanePoint", kPink,   kFullSquare);
  FairMCPointDraw  *TrdPoint      = new FairMCPointDraw ("TrdPoint",      kCyan,   kFullSquare);
  FairMCPointDraw  *TofPoint      = new FairMCPointDraw ("TofPoint",      kGreen,  kFullSquare);
  FairMCPointDraw  *EcalPoint     = new FairMCPointDraw ("EcalPoint",     kYellow, kFullSquare);

  fMan->AddTask(Track);

  fMan->AddTask(MvdPoint);
  fMan->AddTask(StsPoint);
  fMan->AddTask(RichPoint);
  fMan->AddTask(RefPlanePoint);
  fMan->AddTask(TrdPoint);
  fMan->AddTask(TofPoint);
  fMan->AddTask(EcalPoint);

  //  fMan->Init(1,4,10000);
  fMan->Init(1,7,10000);  // make STS visible by default
  //  fMan->Init(1,6,10000);  // make MVD visible by default
}
