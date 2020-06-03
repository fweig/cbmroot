void Import_GDML_Export_ROOT()
{
	TGeoManager *gdml = new TGeoManager("gdml","My 3D Project");

	TGDMLParse parser;
	// Define your input GDML file HERE
	TGeoVolume* gdmlTop = parser.GDMLReadFile ("cframe.gdml");

	TGeoVolume* rootTop = new TGeoVolumeAssembly("geometryFromGDML");
	gGeoManager->SetTopVolume(rootTop);

	// Define your position HERE
	TGeoRotation* rot = new TGeoRotation ("rot", 0., 0., 0.);
	//	TGeoCombiTrans* posrot = new TGeoCombiTrans (-60., 0., 45., rot);
	TGeoCombiTrans* posrot = new TGeoCombiTrans (-60., 0., 20., rot);

	rootTop->AddNode (gdmlTop, 1, posrot);

	gGeoManager->CloseGeometry();
	// Define you output ROOT file HERE
	TFile* outfile = new TFile("cframe.geo.root", "RECREATE");

	rootTop->Write();
	outfile->Close();
}
