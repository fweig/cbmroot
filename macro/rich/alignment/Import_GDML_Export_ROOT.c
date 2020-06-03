void Import_GDML_Export_ROOT()
{
	TString vmcDir = TString(gSystem->Getenv("VMCWORKDIR"));
	TString vmcDir2 = vmcDir + "/geometry/rich/";

	TGeoManager *gdml = new TGeoManager("gdml", "FAIRGeom");

	TGDMLParse parser;
	// Define your input GDML file HERE
//	TGeoVolume* gdmlTop = parser.GDMLReadFile (vmcDir + "/geometry/rich/rich_v16a_1e.gdml");
//	TGeoVolume* gdmlTop = parser.GDMLReadFile (vmcDir + "/geometry/rich/Rich_jan2016_misalign.gdml");
//	TGeoVolume* gdmlTop = parser.GDMLReadFile (vmcDir + "/geometry/rich/Rich_jan2016_misalign_5mrad_study.gdml");
//	TGeoVolume* gdmlTop = parser.GDMLReadFile (vmcDir + "/geometry/rich/RICH_21Nov2016_simplified_test.gdml");

//	TGeoVolume* gdmlTop = parser.GDMLReadFile (vmcDir2 + "misalign_5mrad.gdml");
//	TGeoVolume* gdmlTop = parser.GDMLReadFile (vmcDir2 + "position_cyl/rich_v17a_1e_test.gdml");

//	TGeoVolume* gdmlTop = parser.GDMLReadFile ("/data/ROOT6/trunk/geometry/rich/gauss/gauss_distrib_sigma_5.gdml");
	TGeoVolume* gdmlTop = parser.GDMLReadFile ("/data/ROOT6/trunk/geometry/rich/gauss/3mrad_minusX.gdml");
//	TGeoVolume* gdmlTop = parser.GDMLReadFile ("/data/ROOT6/trunk/geometry/rich/gauss/100mrad.gdml");
	TGeoVolume* rootTop = new TGeoVolumeAssembly("TOP");

	gGeoManager->SetTopVolume(rootTop);

	// Define your position HERE
	TGeoRotation* rot = new TGeoRotation ("rot", 0., 0., 0.);
	TGeoCombiTrans* posrot = new TGeoCombiTrans (0., 0., 270., rot);

	rootTop->AddNode (gdmlTop, 1, posrot);

//	gGeoManager->CheckGeometryFull();
	gGeoManager->CloseGeometry();
	gGeoManager->CheckOverlaps();
	gGeoManager->PrintOverlaps();

	// Define you output ROOT file HERE
//	TFile* outfile = new TFile(vmcDir + "/geometry/rich/rich_v16a_1e.root", "RECREATE");
//	TFile* outfile = new TFile(vmcDir + "/geometry/rich/Rich_jan2016_misalign_1pt5.root", "RECREATE");
//	TFile* outfile = new TFile(vmcDir + "/geometry/rich/Rich_jan2016_misalign_5mrad_Tiles_1_5_0_1.root", "RECREATE");
//	TFile* outfile = new TFile(vmcDir + "/geometry/rich/Rich_jan2016_misalign_5mradXY_Tile1_3.root", "RECREATE");
//	TFile* outfile = new TFile(vmcDir + "/geometry/rich/RICH_21Nov2016_simplified_test.root", "RECREATE");
//	TFile* outfile = new TFile("/data/misalignment_correction/Sim_Outputs/RICH_Geometries/Tile_2_8/Rich_jan2016_misalign_" + nb + "mrad_" + axis + ".root", "RECREATE");
//	TFile* outfile = new TFile("/home/aghoehne/Desktop/Rich_jan2016_misalign_" + nb + "mrad_" + axis + ".root", "RECREATE");

//	TFile* outfile = new TFile(vmcDir2 + "rich_v16a_misalign_5mrad_01y_15x.geo.root", "RECREATE");
//	TFile* outfile = new TFile("/data/ROOT6/trunk/geometry/rich/rich_v17a_gauss_distrib_sigma_5.geo.root", "RECREATE");
	TFile* outfile = new TFile("/data/ROOT6/trunk/geometry/rich/rich_v17a_1e_3mrad_minusX.geo.root", "RECREATE");
//	TFile* outfile = new TFile("/data/ROOT6/trunk/geometry/rich/rich_v17a_1e_100mrad.geo.root", "RECREATE");
	rootTop->Write();
	outfile->Close();
}

