// -------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input with Pluto signal
//
// P.Sitzmann 08/06/2015
//
// --------------------------------------------------------------------------

TString caveGeom="";
TString pipeGeom="";
TString magnetGeom="";
TString mvdGeom="";
TString stsGeom="";
TString richGeom="";
TString muchGeom="";
TString shieldGeom="";
TString trdGeom="";
TString tofGeom="";
TString ecalGeom="";
TString platformGeom="";
TString psdGeom="";
Double_t psdZpos=0.;
Double_t psdXpos=0.;

TString mvdTag="";
TString stsTag="";
TString trdTag="";
TString tofTag="";  

TString stsDigi="";
TString trdDigi="";
TString tofDigi="";
TString muchDigi="";
TString mvdMatBudget="";
TString stsMatBudget="";

TString  fieldMap="";
Double_t fieldZ=0.;
Double_t fieldScale=0.;
Int_t    fieldSymType=0;

TString defaultInputFile="";


// Input Parameter
TString input="pau";
TString inputGEV="10gev";
TString system="centr";
TString signal="d0";
Int_t  iVerbose=0;
TString setup="sis100_electron";
bool littrack=false;
Bool_t useMC=kFALSE;

void opencharm_sim(Int_t nEvents = 10,Int_t ProcID=1, bool backgroundProduction = false)
{
  // ========================================================================
  //          Adjust this part according to your requirements
 TString inDir = gSystem->Getenv("VMCWORKDIR");


 // Input file
  
  TString inFile = Form("%s/input/urqmd.%s.%s.%s.root",inDir.Data(), input.Data(), inputGEV.Data(), system.Data(), input.Data(), inputGEV.Data(), system.Data());
  // ------------------------------------------------------------------------

  if(!backgroundProduction)
  {
  // Output file
  TString outFile = Form("data/opencharm.mc.urqmd.%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());
  // ------------------------------------------------------------------------
  }
  else
  {
    // Output file
  TString outFile = Form("data/opencharm.mc.urqmd.bg.%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());
  // ------------------------------------------------------------------------
  }

  // Parameter file name
  TString parFile = Form("data/paramsunigen.urqmd.%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());
  // ------------------------------------------------------------------------

  TString setupFile = inDir + "/geometry/setup/" + setup + "_setup.C";
  TString setupFunct = setup;
  setupFunct += "_setup()";
  
  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  gRandom->SetSeed(0);

  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

 
  // -----   Create simulation run   ----------------------------------------
  FairRunSim* fRun = new FairRunSim();
  fRun->SetName("TGeant3");              // Transport engine
  fRun->SetOutputFile(outFile);          // Output file
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  // ------------------------------------------------------------------------


  // -----   Create media   -------------------------------------------------
  fRun->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------


  // -----   Create geometry   ----------------------------------------------
  FairModule* cave= new CbmCave("CAVE");
  cave->SetGeometryFileName(caveGeom);
  fRun->AddModule(cave);

  FairModule* pipe= new CbmPipe("PIPE");
  pipe->SetGeometryFileName(pipeGeom);
  fRun->AddModule(pipe);
  
  FairModule* target= new CbmTarget(79, 0.25);
  fRun->AddModule(target);		

  FairModule* magnet = new CbmMagnet("MAGNET");
  magnet->SetGeometryFileName(magnetGeom);
  fRun->AddModule(magnet);
  
  FairDetector* mvd= new CbmMvd("MVD", kTRUE);
  mvd->SetGeometryFileName(mvdGeom); 
  mvd->SetMotherVolume("pipevac1");
  fRun->AddModule(mvd);

  FairDetector* sts = new CbmStsMC(kTRUE);
  sts->SetGeometryFileName(stsGeom);
  fRun->AddModule(sts);

  if ( richGeom != "" ) {
    FairDetector* rich = new CbmRich("RICH", kTRUE);
    rich->SetGeometryFileName(richGeom);
    fRun->AddModule(rich);
  }

  if ( trdGeom != "" ) {
    FairDetector* trd = new CbmTrd("TRD",kTRUE );
    trd->SetGeometryFileName(trdGeom);
    fRun->AddModule(trd);
  }

  if ( tofGeom != "" ) {
    FairDetector* tof = new CbmTof("TOF", kTRUE);
    tof->SetGeometryFileName(tofGeom);
    fRun->AddModule(tof);
  }
  // ------------------------------------------------------------------------



  // -----   Create magnetic field   ---------------------------------------
  CbmFieldMap* magField = NULL;
  if ( 2 == fieldSymType ) {
      magField = new CbmFieldMapSym2(fieldMap);
  }  else if ( 3 == fieldSymType ) {
   magField = new CbmFieldMapSym3(fieldMap);
  }
  else
    magField = new CbmFieldMapSym1(fieldMap);

  magField->SetPosition(0., 0., fieldZ);
  magField->SetScale(fieldScale);
  fRun->SetField(magField);
  cout << endl << "set mag field" << endl;
  // ------------------------------------------------------------------------



  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  CbmUnigenGenerator*  uniGen = new CbmUnigenGenerator(inFile);
  primGen->AddGenerator(uniGen);
  // ------------------------------------------------------------------------

  // ----   Create SignalGenerator     --------------------------------------
//*********** Example 3: Inline style Pluto input (fireball "J/Psi")
    //This example was provided by A. Kotynia **/
    Float_t Eb   = 30; 	    // to jest AGeV - ile GeV na nukleon
    Float_t T1    = 0.120;   // temperature in GeV // powinno byc 150
    Float_t T2    = 0.;      // temperature in GeV
    Float_t blast = 0.;      // radial expansion velocity
    
    //
    // Thermal source with temperature(s) T1 (T2), frac*f(T1) + (1-frac)*f(T2),
    // optional blast, optional polar anisotropies (A2,A4), 
    // optional flow (v1,v2).
    //


    //D+ Meson
    //0 Dm+       1.8693 6.2627e-13 1 0 -1 1 7 0 1 0 411
    //0 Dm+       1.8693 6.2627e-13 1 0 -1 1 1 0 1 0 411
    makeStaticData()->AddParticle(100, "Dm+", 1.8693);
    makeStaticData()->SetParticleTotalWidth("Dm+", 6.2627e-13);
    makeStaticData()->SetParticleMeson("Dm+");
    makeStaticData()->SetParticleParity("Dm+",-1);
    makeStaticData()->SetParticleSpin("Dm+",   1);
	
    // 0.8581   8008012     "Dm+ --> K- + pi+ + pi+"
    makeStaticData()->AddDecay(-1, "Dm+ -> K- + pi+ + pi+", "Dm+", "K-,pi+,pi+", 1);


    //D- Meson
    //0 Dm-       1.8693 6.2627e-13 -1 0 -1 1 7 0 1 0 -411
    makeStaticData()->AddParticle(101, "Dm-", 1.8693);
    makeStaticData()->SetParticleTotalWidth("Dm-", 6.2627e-13);
    makeStaticData()->SetParticleMeson("Dm-");
    makeStaticData()->SetParticleParity("Dm-",-1);
    makeStaticData()->SetParticleSpin("Dm-",   1);
	
    // 0.8581   9009011     "Dm- --> K+ + pi- + pi-"
    makeStaticData()->AddDecay(-1, "Dm- -> K+ + pi- + pi-", "Dm-", "K+,pi-,pi-", 1);


    //D0 Meson
    //Dm0       1.8645 1.5948e-12 0 0 -1 1 8 0 1 0 421
    makeStaticData()->AddParticle(102, "Dm0", 1.8645);
    makeStaticData()->SetParticleTotalWidth("Dm0", 1.5948e-12);
    makeStaticData()->SetParticleMeson("Dm0");
    makeStaticData()->SetParticleParity("Dm0",-1);
    makeStaticData()->SetParticleSpin("Dm0",   1);
	
    // two body  0.929    7008012     "Dm0 --> K- + pi+ + pi0"
    makeStaticData()->AddDecay(-1, "Dm0 -> K- + pi+", "Dm0", "K-,pi+", 1);
    //four body
//    makeStaticData()->AddDecay(-1, "Dm0 -> K- + pi+ + pi+ + pi-", "Dm0", "K-,pi+,pi+,pi-", 1);


if(signal == "dplus")
{// D+
    PFireball *source_Dmp = new PFireball("Dm+",Eb,T1,T2,1.,blast,0.,0.,0.,0.);
    source_Dmp->setTrueThermal(kTRUE);
    source_Dmp->Print();

    PParticle *Dmp = new PParticle("Dm+");
    PParticle *Dmpkminus = new PParticle("K-");
    PParticle *Dmppiplus1 = new PParticle("pi+");
    PParticle *Dmppiplus2 = new PParticle("pi+");

    PParticle *s_Dmp[]  = {source_Dmp,Dmp};
    PChannel  *c_sDmp   = new PChannel(s_Dmp,1,1);

   

    PParticle *s_Dmpkpp[]  ={Dmp,Dmpkminus,Dmppiplus1,Dmppiplus2};
    PChannel  *c_sDmpkpp    = new PChannel(s_Dmpkpp,3,1);

    PChannel  *cc_Dmpkpp[] = {c_sDmp,c_sDmpkpp};
    PReaction *reaction=new PReaction(cc_Dmpkpp,2);
}

else if(signal == "dminus")
{       // D-
    PFireball *source_Dmm = new PFireball("Dm-",Eb,T1,T2,1.,blast,0.,0.,0.,0.);
    source_Dmm->setTrueThermal(kTRUE);
    source_Dmm->Print();

    PParticle *Dmm = new PParticle("Dm-");
    PParticle *s_Dmm[]  = {source_Dmm,Dmm};
    PChannel  *c_sDmm   = new PChannel(s_Dmm,1,1);

    PParticle *Dmmkplus = new PParticle("K+");
    PParticle *Dmmpiminus1 = new PParticle("pi-");
    PParticle *Dmmpiminus2 = new PParticle("pi-");

    PParticle *s_Dmmkpp[]  ={Dmm,Dmmkplus,Dmmpiminus1,Dmmpiminus2};
    PChannel  *c_sDmmkpp    = new PChannel(s_Dmmkpp,3,1);

    PChannel  *cc_Dmmkpp[] = {c_sDmm,c_sDmmkpp};
    PReaction *reaction=new PReaction(cc_Dmmkpp,2);
}

else if(signal == "d0")
{  // D0 two body
    PFireball *source_Dm0 = new PFireball("Dm0",Eb,T1,T2,1.,blast,0.,0.,0.,0.);
    source_Dm0->setTrueThermal(kTRUE);
    source_Dm0->Print();

    PParticle *Dm0 = new PParticle("Dm0");
    PParticle *s_Dm0[]  = {source_Dm0,Dm0};
    PChannel  *c_sDm0   = new PChannel(s_Dm0,1,1);

    PParticle *Dm0kminus = new PParticle("K-");
    PParticle *Dm0piplus1 = new PParticle("pi+");

    PParticle *s_Dm0kp[]  ={Dm0,Dm0kminus,Dm0piplus1};
    PChannel  *c_sDm0kp    = new PChannel(s_Dm0kp,2,1);

    PChannel  *cc_Dm0kp[] = {c_sDm0,c_sDm0kp};
    PReaction *reaction=new PReaction(cc_Dm0kp,2);

}

else if(signal == "d0_4B")
{  // D0 four body
    PFireball *source_Dm0 = new PFireball("Dm0",Eb,T1,T2,1.,blast,0.,0.,0.,0.);
    source_Dm0->setTrueThermal(kTRUE);
    source_Dm0->Print();

    PParticle *Dm0 = new PParticle("Dm0");
    PParticle *s_Dm0[]  = {source_Dm0,Dm0};
    PChannel  *c_sDm0   = new PChannel(s_Dm0,1,1);

    PParticle *Dm0kminus = new PParticle("K-");
    PParticle *Dm0piplus1 = new PParticle("pi+");
    PParticle *Dm0piplus2 = new PParticle("pi+");
    PParticle *Dm0piminus = new PParticle("pi-");

    PParticle *s_Dm0kppp[]  ={Dm0,Dm0kminus,Dm0piplus1,Dm0piplus2,Dm0piminus};
    PChannel  *c_sDm0kppp    = new PChannel(s_Dm0kppp,4,1);

    PChannel  *cc_Dm0kppp[] = {c_sDm0,c_sDm0kppp};
    PReaction *reaction=new PReaction(cc_Dm0kppp,2);
}
else
  exit(-1);

if(!backgroundProduction) CbmPlutoReactionGenerator *plutoGen = new CbmPlutoReactionGenerator(reaction);
if(!backgroundProduction) primGen->AddGenerator(plutoGen);

  fRun->SetGenerator(primGen);   
  // -----   Run initialisation   -------------------------------------------
  fRun->Init();
  // ------------------------------------------------------------------------

  // -----   Runtime database   ---------------------------------------------
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(fRun->GetRunId(),1);
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------

 
  // -----   Start run   ----------------------------------------------------
  fRun->Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime 
       << "s" << endl << endl;
  // ------------------------------------------------------------------------

}

