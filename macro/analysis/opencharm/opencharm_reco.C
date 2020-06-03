// --------------------------------------------------------------------------
//
// Macro for local MVD reconstruction from MC data
//
// Tasks:  CbmMvdDigitiser
//         CbmMvdClusterfinder
//         CbmMvdHitfinder
// 
// P. Sitzmann Juli 2014
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

void opencharm_reco(Int_t nEvents = 10, Int_t ProcID = 1, bool PileUp = false)
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // Input file (MC events)
  TString inFile = Form("data/opencharm.mc.urqmd.%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());
  TString deltaFile = Form("data/opencharm.mc.delta.%i.root", ProcID);
  TString bgFile = Form("data/opencharm.mc.urqmd.bg.%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());

  // Output file
  TString outSystem = Form("data/opencharm.reco.urqmd.%s.%s.%i.%i.%s.%s", input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());

if(!PileUp)
{
if(littrack)
  TString outFile = outSystem + ".littrack.root";
else 
  TString outFile = outSystem + ".l1.root";
}
else if(littrack)
  TString outFile = outSystem + ".PileUp.littrack.root";
else 
  TString outFile = outSystem + ".PileUp.l1.root";


  // Parameter file
TString parFile = Form("data/paramsunigen.urqmd.%s.%s.%i.%i.%s.%s.root",input.Data(), inputGEV.Data(), nEvents, ProcID, signal.Data(), setup.Data());

  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly 
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList *parFileList = new TList();

  TString inDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = inDir + "/parameters/";

  TString setupFile = inDir + "/geometry/setup/" + setup + "_setup.C";
  TString setupFunct = setup;
  setupFunct += "_setup()";

  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  TObjString trdDigiFile = paramDir + trdDigi;
  parFileList->Add(&trdDigiFile);
  cout << "macro/run/run_reco.C using: " << trdDigi << endl;

  TObjString tofDigiFile = paramDir + tofDigi;
  parFileList->Add(&tofDigiFile);
  cout << "macro/run/run_reco.C using: " << tofDigi << endl;
  
 
TString globalTrackingType = "nn";

  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *run= new FairRunAna();

  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  // ------------------------------------------------------------------------
 
  // ----- MC Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("Mc Manager" , 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------

  // -----   MVD Digitiser   ------------------------------------------------
  CbmMvdDigitizer* mvdDigitise = new CbmMvdDigitizer("MVD Digitiser", 0, iVerbose);

if(PileUp)
	{
 	 Int_t pileUpInMVD=3; 
  	mvdDigitise->SetBgFileName(bgFile);
  	mvdDigitise->SetBgBufferSize(200); 
  	mvdDigitise->SetPileUp(pileUpInMVD-1);
  	//--- Delta electrons -------
 	 mvdDigitise->SetDeltaName(deltaFile);
 	 mvdDigitise->SetDeltaBufferSize(pileUpInMVD*200); 
  	mvdDigitise->SetDeltaEvents(pileUpInMVD*100);
	}
  //mvdDigitise->ShowDebugHistograms();
  run->AddTask(mvdDigitise);
  // ----------------------------------------------------------------------

  // -----   MVD Clusterfinder   --------------------------------------------
  CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Clusterfinder", 0, iVerbose); 
  //mvdCluster->ShowDebugHistos();
  run->AddTask(mvdCluster);
  // ----------------------------------------------------------------------

  // -----   MVD Hit Finder   ---------------------------------------------
  CbmMvdHitfinder* mvdHitfinder = new CbmMvdHitfinder("MVD Hit Finder", 0, iVerbose);
  mvdHitfinder->UseClusterfinder(kTRUE);
  //mvdHitfinder->ShowDebugHistos();
  run->AddTask(mvdHitfinder);
  // ----------------------------------------------------------------------


  // -----   STS digitizer   -------------------------------------------------
  // -----   The parameters of the STS digitizer are set such as to match
  // -----   those in the old digitizer. Change them only if you know what you
  // -----   are doing.
  Double_t dynRange       =   40960.;  // Dynamic range [e]
  Double_t threshold      =    4000.;  // Digitisation threshold [e]
  Int_t nAdc              =    4096;   // Number of ADC channels (12 bit)
  Double_t timeResolution =       5.;  // time resolution [ns]
  Double_t deadTime       = 9999999.;  // infinite dead time (integrate entire event)
  Double_t noise          =       0.;  // ENC [e]
  Int_t digiModel         =       1;   // User sensor type DSSD
	
  // The following settings correspond to a validated implementation. 
  // Changing them is on your own risk.
  Int_t  eLossModel       = 1;         // Energy loss model: uniform 
  Bool_t useLorentzShift  = kFALSE;    // Deactivate Lorentz shift
  Bool_t useDiffusion     = kFALSE;    // Deactivate diffusion
  Bool_t useCrossTalk     = kFALSE;    // Deactivate cross talk

  CbmStsDigitize* stsDigi = new CbmStsDigitize(digiModel);
  stsDigi->SetProcesses(eLossModel, useLorentzShift, useDiffusion, useCrossTalk);
  stsDigi->SetParameters(dynRange, threshold, nAdc, timeResolution,
  		                 deadTime, noise);
  run->AddTask(stsDigi);
  // -------------------------------------------------------------------------


  // -----   STS Cluster Finder   --------------------------------------------
  FairTask* stsClusterFinder = new CbmStsFindClusters();
  run->AddTask(stsClusterFinder);
  // -------------------------------------------------------------------------


  // -----   STS hit finder   ------------------------------------------------
  FairTask* stsFindHits = new CbmStsFindHits();
  run->AddTask(stsFindHits);
  // -------------------------------------------------------------------------

  // -----   STS track finding   --------------------------------------------
  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);
  CbmL1* l1 = new CbmL1();
  TString mvdMatBudgetFileName = paramDir + mvdMatBudget;
  TString stsMatBudgetFileName = paramDir + stsMatBudget;
  l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  run->AddTask(l1);

  Bool_t useMvdInL1Tracking = !littrack;
  CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
  FairTask* stsFindTracks = new CbmStsFindTracks(iVerbose, stsTrackFinder, useMvdInL1Tracking);
  run->AddTask(stsFindTracks);
  // ------------------------------------------------------------------------
  
 if(littrack)
{
  CbmLitFindMvdTracks* mvdFinder = new CbmLitFindMvdTracks();
  run->AddTask(mvdFinder);
}

// =========================================================================
  // ===                     TRD local reconstruction                      ===
  // =========================================================================

  Bool_t  simpleTR  = kTRUE;  // use fast and simple version for TR production
  CbmTrdRadiator *radiator = new CbmTrdRadiator(simpleTR,"K++");
  //"K++" : micro structured POKALON
  //"H++" : PE foam foils
  //"G30" : ALICE fibers 30 layers

  Bool_t triangularPads = false;// Bucharest triangular pad-plane layout
  Double_t triggerThreshold = 0.5e-6;//SIS100
  //Double_t triggerThreshold = 1.0e-6;//SIS300
  Double_t trdNoiseSigma_keV = 0.1; //default best matching to test beam PRF

  CbmTrdDigitizerPRF* trdDigiPrf = new CbmTrdDigitizerPRF(radiator);
  trdDigiPrf->SetTriangularPads(triangularPads);
  trdDigiPrf->SetNoiseLevel(trdNoiseSigma_keV);
  run->AddTask(trdDigiPrf);

  CbmTrdClusterFinderFast* trdCluster = new CbmTrdClusterFinderFast();
  trdCluster->SetNeighbourTrigger(true);
  trdCluster->SetTriggerThreshold(triggerThreshold);
  trdCluster->SetNeighbourRowTrigger(false);
  trdCluster->SetPrimaryClusterRowMerger(true);
  trdCluster->SetTriangularPads(triangularPads);
  run->AddTask(trdCluster);

  CbmTrdHitProducerCluster* trdHit = new CbmTrdHitProducerCluster();
  trdHit->SetTriangularPads(triangularPads);
  run->AddTask(trdHit);

  // -------------------------------------------------------------------------
  // ===                 End of TRD local reconstruction                   ===
  // =========================================================================


  // =========================================================================
  // ===                     TOF local reconstruction                      ===
  // =========================================================================


  // ------   TOF hit producer   ---------------------------------------------
  CbmTofHitProducerNew* tofHitProd = new CbmTofHitProducerNew("TOF HitProducerNew",iVerbose); 
  tofHitProd->SetInitFromAscii(kFALSE);
  run->AddTask(tofHitProd);
  // -------------------------------------------------------------------------

  // ===                   End of TOF local reconstruction                 ===
  // =========================================================================




  // =========================================================================
  // ===                        Global tracking                            ===
  // =========================================================================

  CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
  // Tracking method to be used
  // "branch" - branching tracking
  // "nn" - nearest neighbor tracking
  // "weight" - weighting tracking
  finder->SetTrackingType("branch");

  // Hit-to-track merger method to be used
  // "nearest_hit" - assigns nearest hit to the track
  finder->SetMergerType("nearest_hit");

  run->AddTask(finder);

  // ===                      End of global tracking                       ===
  // =========================================================================
  

  //------   Match Monte Carlo Data to Reco Data    -------------------------
  CbmMatchRecoToMC* matcher = new CbmMatchRecoToMC();
  matcher->SetIncludeMvdHitsInStsTrack(kTRUE);
  run->AddTask(matcher);
  // ------------------------------------------------------------------------
/*
  // ----------- TRD track Pid Ann ----------------------
  CbmTrdSetTracksPidANN* trdSetTracksPidAnnTask = new CbmTrdSetTracksPidANN(
  		"Ann", "Ann");
  run->AddTask(trdSetTracksPidAnnTask);
  // ----------------------------------------------------

  // =========================================================================
  // ===                        RICH reconstruction                        ===
  // =========================================================================

  if (richGeom.Length() != 0)  // if RICH is defined
    {
  // ---------------------RICH Hit Producer ----------------------------------
  CbmRichHitProducer* richHitProd  = new CbmRichHitProducer();
  richHitProd->SetDetectorType(4);
  richHitProd->SetNofNoiseHits(220);
  richHitProd->SetCollectionEfficiency(1.0);
  richHitProd->SetSigmaMirror(0.06);
  run->AddTask(richHitProd);
  //--------------------------------------------------------------------------

  //--------------------- RICH Reconstruction ----------------------------------
  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  run->AddTask(richReco);

  // ------------------- RICH Ring matching  ---------------------------------
  CbmRichMatchRings* matchRings = new CbmRichMatchRings();
  run->AddTask(matchRings);
  // -------------------------------------------------------------------------
    }
  // ===                 End of RICH local reconstruction                  ===
  // =========================================================================
*/
  // -----   Primary vertex finding   --------------------------------------
  CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
  CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
  findVertex->SetName("FindPrimaryVertex");
  run->AddTask(findVertex);
  // -----------------------------------------------------------------------

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------

  // -----   Run initialisation   -------------------------------------------
  run->Init();
  // ------------------------------------------------------------------------
  cout << endl << "Starting Run" << endl;
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
     
  // -----   Start run   ----------------------------------------------------
  run->Run(0,nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------


}
