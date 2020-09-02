void eventDisplay(
	       TString cSys="lam", 
	       TString cEbeam="2.5gev",
	       TString cCentr="-",
	       Int_t   iRun=0,
	       const char* setup = "sis18_mcbm")
{

  TString dataDir   = "data/";

  TString  InputFile = dataDir + setup + "_" + cSys + "." + cEbeam + "." + cCentr + ".mc." + Form("%05d",iRun) + ".root";
  TString  RecoFile  = dataDir + setup + "_" + cSys + "." + cEbeam + "." + cCentr + ".eds." + Form("%05d",iRun) + ".root"; 
  TString  ParFile   = dataDir + setup + "_" + cSys + "." + cEbeam + "." + cCentr + ".params." + Form("%05d",iRun) + ".root"; 

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();

  fRun->SetInputFile(InputFile.Data());
  fRun->AddFriend(RecoFile.Data());

  fRun->SetOutputFile(dataDir + setup + "_test.root");

  //FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG1");
  FairLogger::GetLogger()->SetLogVerbosityLevel("MEDIUM");

  CbmHadronAnalysis *HadronAna = new CbmHadronAnalysis(); // interpret event
  HadronAna->SetRecSec(kTRUE);     // enable lambda reconstruction
  Int_t parSet=0;
  switch(parSet){
    case 0: // with background  
      HadronAna->SetDistPrimLim(1.2);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3); // Max Sts-Sts trans distance for primaries 
      HadronAna->SetDistSecLim2(0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries  
      HadronAna->SetD0ProtLim(0.5);    // Min impact parameter for secondary proton 
      HadronAna->SetOpAngMin(0.1);     // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);       // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);       // Min Lambda flight path length for accepting pair  
      HadronAna->SetVLenMax(25.);      // Max Lambda flight path length for accepting pair  
      HadronAna->SetDistTRD(10.);      // max accepted distance of Trd Hit from STS-TOF line 
      HadronAna->SetTRDHmulMin(0.);    // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with 
      break;
    case 1:  // signal with background Ni+Ni 
      HadronAna->SetDistPrimLim(1.);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3); // Max Sts-Sts trans distance for primaries 
      HadronAna->SetDistSecLim2(0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries  
      HadronAna->SetD0ProtLim(0.4);    // Min impact parameter for secondary proton 
      HadronAna->SetOpAngMin(0.1);     // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);       // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);       // Min Lambda flight path length for accepting pair  
      HadronAna->SetVLenMax(25.);      // Max Lambda flight path length for accepting pair  
      HadronAna->SetDistTRD(10.);      // max accepted distance of Trd Hit from STS-TOF line 
      HadronAna->SetTRDHmulMin(0.);    // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with 
      break;
    case 2:  // signal with background Au+Au 
      HadronAna->SetDistPrimLim(1.);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3); // Max Sts-Sts trans distance for primaries 
      HadronAna->SetDistSecLim2(0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries  
      HadronAna->SetD0ProtLim(0.4);    // Min impact parameter for secondary proton 
      HadronAna->SetOpAngMin(0.1);     // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);       // Max DCA for accepting pair
      HadronAna->SetVLenMin(8.);       // Min Lambda flight path length for accepting pair  
      HadronAna->SetVLenMax(25.);      // Max Lambda flight path length for accepting pair  
      HadronAna->SetDistTRD(10.);      // max accepted distance of Trd Hit from STS-TOF line 
      HadronAna->SetTRDHmulMin(0.);    // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with 
      break;
    case 10:                           // "0" with TRD Mul 1  
      HadronAna->SetDistPrimLim(1.2);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3); // Max Sts-Sts trans distance for primaries 
      HadronAna->SetDistSecLim2(0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries  
      HadronAna->SetD0ProtLim(0.5);    // Min impact parameter for secondary proton 
      HadronAna->SetOpAngMin(0.1);     // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);       // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);       // Min Lambda flight path length for accepting pair  
      HadronAna->SetVLenMax(25.);      // Max Lambda flight path length for accepting pair  
      HadronAna->SetDistTRD(10.);      // max accepted distance of Trd Hit from STS-TOF line 
      HadronAna->SetTRDHmulMin(1.);    // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with 
      break;
    case 20:                           // "0" with TRD Mul 2  
      HadronAna->SetDistPrimLim(1.2);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3); // Max Sts-Sts trans distance for primaries 
      HadronAna->SetDistSecLim2(0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries  
      HadronAna->SetD0ProtLim(0.5);    // Min impact parameter for secondary proton 
      HadronAna->SetOpAngMin(0.1);     // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);       // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);       // Min Lambda flight path length for accepting pair  
      HadronAna->SetVLenMax(25.);      // Max Lambda flight path length for accepting pair  
      HadronAna->SetDistTRD(10.);      // max accepted distance of Trd Hit from STS-TOF line 
      HadronAna->SetTRDHmulMin(2.);    // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with 
      break;
    default:
      cout << "Cut value set " <<parSet<<" not existing, stop macro "<< endl;
      return;
  }

  fRun->AddTask(HadronAna);

  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(ParFile.Data());
  rtdb->setFirstInput(parInput1);

  FairEventManager *fMan  = new FairEventManager();
  FairMCTracks     *Track = new FairMCTracks ("Monte-Carlo Tracks");

  FairMCPointDraw  *MvdPoint      = new FairMCPointDraw ("MvdPoint",      kBlack,  kFullSquare);
  FairMCPointDraw  *StsPoint      = new FairMCPointDraw ("StsPoint",      kGreen,  kFullSquare);
  FairMCPointDraw  *MuchPoint     = new FairMCPointDraw ("MuchPoint",     kOrange, kFullSquare);
  FairMCPointDraw  *RichPoint     = new FairMCPointDraw ("RichPoint",     kGreen,  kFullSquare);
  FairMCPointDraw  *TrdPoint      = new FairMCPointDraw ("TrdPoint",      kBlue,   kFullSquare);
  FairMCPointDraw  *TofPoint      = new FairMCPointDraw ("TofPoint",      kRed,    kFullSquare);
  FairMCPointDraw  *EcalPoint     = new FairMCPointDraw ("EcalPoint",     kYellow, kFullSquare);
  FairMCPointDraw  *RefPlanePoint = new FairMCPointDraw ("RefPlanePoint", kPink,   kFullSquare);
                                                               
  fMan->AddTask(Track);
  
  fMan->AddTask(MvdPoint);
  fMan->AddTask(StsPoint);
  fMan->AddTask(MuchPoint);
  fMan->AddTask(RichPoint);
  fMan->AddTask(TrdPoint);
  fMan->AddTask(TofPoint);   
  fMan->AddTask(EcalPoint);   
  fMan->AddTask(RefPlanePoint);

  CbmPixelHitSetDraw *StsHits = new CbmPixelHitSetDraw ("StsHit", kRed, kOpenCircle );// kFullSquare);
  fMan->AddTask(StsHits);  
  CbmPixelHitSetDraw *TrdHits = new CbmPixelHitSetDraw ("TrdHit", kRed, kOpenCircle );// kFullSquare);
  fMan->AddTask(TrdHits);    
  CbmPixelHitSetDraw *TofHits = new CbmPixelHitSetDraw ("TofHit", kRed, kOpenCircle );// kFullSquare);
  fMan->AddTask(TofHits);   
  CbmPixelHitSetDraw *TofUHits = new CbmPixelHitSetDraw ("TofUHit", kRed, kOpenCross );
  fMan->AddTask(TofUHits); 
  CbmEvDisTracks *Tracks =  new CbmEvDisTracks ("Tof Tracks",1);
  Tracks->SetVerbose(4);
  fMan->AddTask(Tracks);
    
  //  fMan->Init(1,4,10000);
  //  fMan->Init(1,5,10000);  // make STS visible by default
  //  fMan->Init(1,6,10000);  // make MVD visible by default
  fMan->Init(1,7,10000);  // make MVD visible by default

  cout << "gEve "<< gEve << endl;
  gEve->GetDefaultGLViewer()->SetClearColor(kYellow-10);
  {   // from readCurrentCamera(const char* fname)
  TGLCamera& c = gEve->GetDefaultGLViewer()->CurrentCamera();
  const char* fname="Cam.sav";
  TFile* f = TFile::Open(fname, "READ");
  if (!f) 
    return;
  if (f->GetKey(c.ClassName())) {
    f->GetKey(c.ClassName())->Read(&c);
    c.IncTimeStamp();
    gEve->GetDefaultGLViewer()->RequestDraw();
  }
  }
}
