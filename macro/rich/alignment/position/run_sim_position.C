void run_sim_position(Int_t nEvents = 100, Int_t Flag = 0)
{
	//TEST//
    TTree::SetMaxTreeSize(90000000000);
    Int_t iVerbose = 0;

    TString script = TString(gSystem->Getenv("SCRIPT"));
    TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters");

    //gRandom->SetSeed(10);

    TString urqmdFile = "/data/Cbm_Root/urqmd/auau/25gev/centr/urqmd.auau.25gev.centr.00001.root";

    if (Flag == 0) {TString outDir = "/u/jbendar/Sim_Outputs/Ring_Track_VS_Position/Misaligned/";}
    else if (Flag == 1) {TString outDir = "/u/jbendar/Sim_Outputs/Ring_Track_VS_Position/Aligned/";}
    TString mcFile = outDir + "mc.root";
    outDir = "/home/aghoehne/Desktop/";
    TString parFile = outDir + "param.root";
    TString geoFile = outDir + "geofilefull.root";
    TString outFile = outDir + "out.root";

/*	TString outDir = "/data/misalignment_correction/event_display/test/"; // For eventDisplay and run_rich_event_display macros
	TString parFile = outDir + "param.root";
	TString mcFile = outDir + "mc.root";
	TString geoFile = outDir + "geofilefull.root";
*/
	// Set geometries:
	TString caveGeom = "cave.geo";
	TString pipeGeom = "pipe/pipe_v14l.root";
	TString magnetGeom = "magnet/magnet_v15a.geo.root";
	TString fieldMap = "field_v12b";
	TString stsGeom = "sts/sts_v15c.geo.root";
	if (Flag == 0) {TString richGeom = "rich/Rich_jan2016_aligned.root";}
	else if (Flag == 1) {TString richGeom = "rich/Rich_jan2016_misalign_5mradXY_Tile1_3.root";}
	TString trdGeom = ""; //"trd_v15a_1e.geo.root";
	TString tofGeom = ""; //"tof_v16a_1e.geo.root";
	TString mvdGeom = ""; //"mvd_v15a.geo.root";
    Double_t fieldZ       = 40.;            // field centre z position
    Double_t fieldScale   =  1.;            // field scaling factor
    Double_t fieldSymType =  3;

    //TString geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/rich/run/geosetup/geosetup_25gev.C";

    TString electrons = "yes"; // If "yes" then primary electrons will be generated
    Int_t NELECTRONS = 1; // number of e- to be generated
    Int_t NPOSITRONS = 1; // number of e+ to be generated
    TString urqmd = "no"; // If "yes" then UrQMD will be used as background
    TString pluto = "no"; // If "yes" PLUTO particles will be embedded
    TString plutoFile = "";
    TString plutoParticle = "";

    if (script == "yes") {
        urqmdFile = TString(gSystem->Getenv("URQMD_FILE"));
        mcFile = TString(gSystem->Getenv("MC_FILE"));
        parFile = TString(gSystem->Getenv("PAR_FILE"));
	cout << "mcFile: " << TString(gSystem->Getenv("MC_FILE")) << endl << "parFile: " << TString(gSystem->Getenv("PAR_FILE")) << endl << "urqmdFile: " << TString(gSystem->Getenv("URQMD_FILE")) << endl;

        //geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/rich/run/geosetup/" + TString(gSystem->Getenv("GEO_SETUP_FILE"));

        NELECTRONS = TString(gSystem->Getenv("NELECTRONS")).Atoi();
        NPOSITRONS = TString(gSystem->Getenv("NPOSITRONS")).Atoi();
        electrons = TString(gSystem->Getenv("ELECTRONS"));
        urqmd = TString(gSystem->Getenv("URQMD"));
        pluto = TString(gSystem->Getenv("PLUTO"));
        plutoFile = TString(gSystem->Getenv("PLUTO_FILE"));
        plutoParticle = TString(gSystem->Getenv("PLUTO_PARTICLE"));
    }

    remove(parFile.Data());
    remove(mcFile.Data());

    gDebug = 0;
    TStopwatch timer;
    timer.Start();

    //setup all geometries from macro
    //cout << "geoSetupName:" << geoSetupFile << endl;
    //gROOT->LoadMacro(geoSetupFile);
    //init_geo_setup();

    gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
    loadlibs();

    //Logger settings
    TString logLevel = "INFO";   // "DEBUG";
    TString logVerbosity = "LOW";

    //Target geometry
    TString  targetElement   = "Gold";
    Double_t targetThickness = 0.025; // 250 mum, full thickness in cm
    Double_t targetDiameter  = 2.5;    // diameter in cm
    Double_t targetPosX      = 0.;     // target x position in global c.s. [cm]
    Double_t targetPosY      = 0.;     // target y position in global c.s. [cm]
    Double_t targetPosZ      = 0.;     // target z position in global c.s. [cm]
    Double_t targetRotY      = 0.;     // target rotation angle around the y axis [deg]

    // creation of the primary vertex
    Bool_t smearVertexXY = kTRUE;
    Bool_t smearVertexZ  = kTRUE;
    Double_t beamWidthX   = 1.;  // Gaussian sigma of the beam profile in x [cm]
    Double_t beamWidthY   = 1.;  // Gaussian sigma of the beam profile in y [cm]
    // ------------------------------------------------------------------------

    FairRunSim* fRun = new FairRunSim();
    fRun->SetName("TGeant3");
    fRun->SetOutputFile(mcFile);
    fRun->SetGenerateRunInfo(kTRUE);
    FairRuntimeDb* rtdb = fRun->GetRuntimeDb();

//    gLogger->SetLogScreenLevel(logLevel.Data());
//    gLogger->SetLogVerbosityLevel(logVerbosity.Data());

    fRun->SetMaterials("media.geo"); // Materials

    if ( caveGeom != "" ) {
        FairModule* cave = new CbmCave("CAVE");
        cave->SetGeometryFileName(caveGeom);
        fRun->AddModule(cave);
    }

    if ( pipeGeom != "" ) {
        FairModule* pipe = new CbmPipe("PIPE");
        pipe->SetGeometryFileName(pipeGeom);
        fRun->AddModule(pipe);
    }

    CbmTarget* target = new CbmTarget(targetElement.Data(), targetThickness, targetDiameter);
    target->SetPosition(targetPosX, targetPosY, targetPosZ);
    target->SetRotation(targetRotY);
    fRun->AddModule(target);

    if ( magnetGeom != "" ) {
        FairModule* magnet = new CbmMagnet("MAGNET");
        magnet->SetGeometryFileName(magnetGeom);
        fRun->AddModule(magnet);
    }

    if ( mvdGeom != "" ) {
        FairDetector* mvd = new CbmMvd("MVD", kTRUE);
        mvd->SetGeometryFileName(mvdGeom);
        mvd->SetMotherVolume("pipevac1");
        fRun->AddModule(mvd);
    }

    if ( stsGeom != "" ) {
        FairDetector* sts = new CbmStsMC(kTRUE);
        sts->SetGeometryFileName(stsGeom);
        fRun->AddModule(sts);
    }

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

    // Create magnetic field
    cout << "fieldSymType=" << fieldSymType << endl;
    CbmFieldMap* magField = NULL;
    if ( 2 == fieldSymType ) {
        CbmFieldMap* magField = new CbmFieldMapSym2(fieldMap);
    }  else if ( 3 == fieldSymType ) {
        CbmFieldMap* magField = new CbmFieldMapSym3(fieldMap);
    }
    magField->SetPosition(0., 0., fieldZ);
    magField->SetScale(fieldScale);
    fRun->SetField(magField);

    // -----   Create PrimaryGenerator   --------------------------------------
    FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
    Double_t tX = 0.;
    Double_t tY = 0.;
    Double_t tZ = 0.;
    Double_t tDz = 0.;
    if ( target ) {
        target->GetPosition(tX, tY, tZ);
        tDz = target->GetThickness();
    }
    primGen->SetTarget(tZ, tDz);
    primGen->SetBeam(0., 0., beamWidthX, beamWidthY);
    primGen->SmearGausVertexXY(smearVertexXY);
    primGen->SmearVertexZ(smearVertexZ);

    if (urqmd == "yes"){
        CbmUnigenGenerator*  uniGen = new CbmUnigenGenerator(urqmdFile);
        uniGen->SetEventPlane(0. , 360.);
        primGen->AddGenerator(uniGen);
    }

    if (electrons == "yes"){
        FairBoxGenerator* boxGen1 = new FairBoxGenerator(-11, NPOSITRONS);
        boxGen1->SetPRange(1., 9.);
        //boxGen1->SetPtRange(0., 3.);
        boxGen1->SetPhiRange(0.5, 179.5);
        boxGen1->SetThetaRange(2.5, 25);
        boxGen1->SetCosTheta();
        boxGen1->Init();
        primGen->AddGenerator(boxGen1);

        FairBoxGenerator* boxGen2 = new FairBoxGenerator(11, NELECTRONS);
        boxGen2->SetPRange(1., 9.);
        //boxGen1->SetPtRange(0., 3.);
        boxGen2->SetPhiRange(0.5, 179.5);
        boxGen2->SetThetaRange(2.5, 25);
        boxGen2->SetCosTheta();
        boxGen2->Init();
        primGen->AddGenerator(boxGen2);
    }

        //      CbmLitPolarizedGenerator *polGen;
        //      polGen = new CbmLitPolarizedGenerator(443, NELECTRONS);
        //      polGen->SetDistributionPt(0.176);        // 25 GeV
        //      polGen->SetDistributionY(1.9875,0.228);  // 25 GeV
        //      polGen->SetRangePt(0.,3.);
        //      polGen->SetRangeY(1.,3.);
        //      polGen->SetBox(0);
        //      polGen->SetRefFrame(CbmLitPolarizedGenerator::kHelicity);
        //      polGen->SetDecayMode(CbmLitPolarizedGenerator::kDiElectron);
        //      polGen->SetAlpha(0);
        //      polGen->Init();
        //      primGen->AddGenerator(polGen);

    if (pluto == "yes") {
        CbmPlutoGenerator *plutoGen= new CbmPlutoGenerator(plutoFile);
        primGen->AddGenerator(plutoGen);
    }

    fRun->SetGenerator(primGen);
//    fRun->SetStoreTraj(kTRUE);
    fRun->Init();

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

    fRun->Run(nEvents);
    fRun->CreateGeometryFile(geoFile);

    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is "    << mcFile << endl;
    cout << "Parameter file is " << parFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime
    << "s" << endl << endl;

    cout << " Test passed" << endl;
    cout << " All ok " << endl;
}
