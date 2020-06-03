void run_mirror_sim(Int_t nEvents = 5000, TString tile="", TString axis="", TString numb="")
// !!! DEFINE NEW ROTATION ANGLE IN THE GEO FILE AND RUN IMPORT_EXPORT_GEO MACRO BEFORE RUNNING RUN_SIM !!!
{
    TTree::SetMaxTreeSize(90000000000);
    Int_t iVerbose = 0;

    TString script = TString(gSystem->Getenv("SCRIPT"));
    TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters");

    //gRandom->SetSeed(10);

	TString urqmdFile = "/data/Cbm_Root/urqmd/auau/25gev/centr/urqmd.auau.25gev.centr.00001.root";

//	TString outDir = "/data/misalignment_correction/Sim_Outputs/Alignment_Correction/Test/";
//	TString outDir = "/data/misalignment_correction/Sim_Outputs/Mirror_Sorting/First/";
//	TString outDir = "/data/misalignment_correction/Sim_Outputs/Mirror_Sorting/Study_Track_Ring/Tile_" + tile + "/" + axis + "_" + numb + "mrad" + "/";
    TString outDir = "/home/aghoehne/Desktop/";
//	TString numb = "0.1mrad_";
//	TString axis = "Y_";
//	TString tile = "TILE";
//	TString tile = "1_4";
//	TString tile = "2_1";
//	numb = "test", axis = "", tile = "";
    TString parFile = outDir + "param." + numb + "_" + axis + "_" + tile + ".root";
    TString mcFile = outDir + "mc." + numb + "_" + axis + "_" + tile + ".root";
	TString geoFile = outDir + "geofilefull." + numb + "_" + axis + "_" + tile + ".root";
	TString outFile = outDir + "out." + numb + axis + tile + ".root";

/*	TString outDir = "/data/misalignment_correction/event_display/test/"; // For eventDisplay and run_rich_event_display macros
	TString numb = ".00001";
	TString parFile = outDir + "param" + numb + ".root";
	TString mcFile = outDir + "mc" + numb + ".root";
	TString geoFile = outDir + "geofilefull." + numb + ".root";
*/
	// Set geometries:
	TString caveGeom = "cave.geo";
	TString pipeGeom = "pipe/pipe_v14l.root";
	TString magnetGeom = "magnet/magnet_v15a.geo.root";
	TString fieldMap = "field_v12b";
	TString stsGeom = "sts/sts_v15c.geo.root";
//	TString richPath = "/data/misalignment_correction/Sim_Outputs/RICH_Geometries/Tile_" + tile + "/Rich_jan2016_misalign_" + numb + "mrad_" + axis + ".root";
//	TString richPath = "/data/cbm/cbmroot_new/geometry/rich/geoGen/GeoFiles1/Rich_jan2016_misalign_tile_" + tile + "_axis_" + axis  + "_mis_" + numb  + "mrad.root";
//	TString richGeom = richPath;
	TString richGeom = "rich/Rich_jan2016_misalign.root";
	TString trdGeom = ""; //"trd_v15a_1e.geo.root";
	TString tofGeom = ""; //"tof_v16a_1e.geo.root";
	TString mvdGeom = ""; //"mvd_v15a.geo.root";

    TString geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/rich/run/geosetup/geosetup_25gev.C";

    TString electrons = "yes"; // If "yes" then primary electrons will be generated
    Int_t NELECTRONS = 1; // number of e- to be generated
    Int_t NPOSITRONS = 1; // number of e+ to be generated
    TString urqmd = "no"; // If "yes" then UrQMD will be used as background
    TString pluto = "no"; // If "yes" PLUTO particles will be embedded
    TString plutoFile = "";
    TString plutoParticle = "";

    if (script == "yes") {
        urqmdFile = TString(gSystem->Getenv("IN_FILE"));
        mcFile = TString(gSystem->Getenv("MC_FILE"));
        parFile = TString(gSystem->Getenv("PAR_FILE"));

        geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/rich/run/geosetup/" + TString(gSystem->Getenv("GEO_SETUP_FILE"));

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
    cout << "geoSetupName:" << geoSetupFile << endl;
    gROOT->LoadMacro(geoSetupFile);
    init_geo_setup();

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
    cout <<"fieldSymType=" << fieldSymType << endl;
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

/*    Double_t Xi = -64.310094 + 20.9690586, Yi = 61.256446 - 0.1092312, Zi = 338.476947 + 4.3663151; // 5mrad Study.
    Double_t omega = TMath::ATan(-1*Xi/Yi);
    Double_t phi0 = 90 + omega*TMath::RadToDeg();
    Double_t phi1 = phi0 - 1.5;
    Double_t phi2 = phi0 + 1.5;
    Double_t oc = TMath::Sqrt(Xi*Xi + Yi*Yi);
    Double_t theta0 = TMath::ATan(oc/Zi)*TMath::RadToDeg();
    Double_t theta1 = theta0 - 0.5;
    Double_t theta2 = theta0 + 0.5;*/

//    phi0 = 145., theta0 = 28.5; // Tile 1_2 Study
//    phi0 = 109., theta0 = 10.1; // Tile 1_4 Study
//    phi1 = 136., theta1 = 13.8; // Tile 1_3 Study
//    phi2 = 38., theta2 = 24.; // Tile 2_8 Study
    phi0 = 10., theta0 = 21.; // Tile 0_8 Study

    //add electrons
    if (electrons == "yes"){
/*	// Box tile 1_3
	FairBoxGenerator* boxGen1 = new FairBoxGenerator(11, NELECTRONS);
    boxGen1->SetPtRange(8.,9.);
    boxGen1->SetPhiRange(phi1 - 3., phi1 + 3.);
    boxGen1->SetThetaRange(theta1 - 1., theta1 + 1.);
    boxGen1->SetCosTheta();
    boxGen1->Init();
    primGen->AddGenerator(boxGen1);
*/
/*	// Box tile 1_2 & 1_4
	FairBoxGenerator* boxGen4 = new FairBoxGenerator(11, NELECTRONS);
    boxGen4->SetPtRange(8.,9.);
    boxGen4->SetPhiRange(phi0 - 3., phi0 + 3.);
    boxGen4->SetThetaRange(theta0 - 1., theta0 + 1.);
    boxGen4->SetCosTheta();
    boxGen4->Init();
    primGen->AddGenerator(boxGen4);
*/
/*	// Box tile 2_8
    FairBoxGenerator* boxGen2 = new FairBoxGenerator(-11, NPOSITRONS);
    boxGen2->SetPtRange(1.,3.);
    boxGen2->SetPhiRange(phi2 - 5., phi2 + 5.);
    boxGen2->SetThetaRange(theta2, theta2 + 1.);
    boxGen2->SetCosTheta();
    boxGen2->Init();
    primGen->AddGenerator(boxGen2);
*/
    // Box tile 0_8
    FairBoxGenerator* boxGen3 = new FairBoxGenerator(-11, NPOSITRONS);
    boxGen3->SetPtRange(1.,3.);
    boxGen3->SetPhiRange(phi0 - 6., phi0 + 6.);
    boxGen3->SetThetaRange(theta0, theta0 + 1.);
    boxGen3->SetCosTheta();
    boxGen3->Init();
    primGen->AddGenerator(boxGen3);

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
    }

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
