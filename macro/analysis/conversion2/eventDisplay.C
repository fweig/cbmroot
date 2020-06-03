///// macro for visualization of simulated events. One can use it directly after simulation stage (reonctruction and digi are nor needed)
///// here one can see Monte Carlo hits in each detector.
///// If the option "store all hits" (in /gconfig/g3Config.C -> st->SetMinPoints(0);) is activated during the simulation, one will see also tracks (including light in RICH) 
void eventDisplay(const char* setup = "test_mirror_long_363")
{

  TString dataDir   = "data/";
  TString InputFile = dataDir + setup + ".tra.00002.root";
  TString ParFile   = dataDir + setup + ".par.00002.root";

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
  // fMan->Init(1,5,10000);  // make STS visible by default
   fMan->Init(1,6,10000);  // make MVD visible by default
}
