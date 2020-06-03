
void Import_GDML_Export_ROOT()
{
    // we need to use latest root version to work with gdml geometry
    system(string("source /usr/local/Cellar/root/6.16.00/bin/thisroot.sh").c_str());

    TString richGeoFilename = "rich_v19d_pipe_v4.gdml";

    TGeoManager *gdml = new TGeoManager("gdml", "FAIRGeom");

    cout << "Importing '" << richGeoFilename << "'." << endl;

    TGDMLParse parser;
    // Define your input GDML file HERE
    TGeoVolume* gdmlTop = parser.GDMLReadFile(richGeoFilename);
    TGeoVolume* rootTop = new TGeoVolumeAssembly("TOP");

    gGeoManager->SetTopVolume(rootTop);

    // Starting from the version v18a position is defined inside the GDML file
    // Define your position HERE
    // Z coordinate for v16a = 270, for v17a = 258.75, for v18a = 0.
    TGeoRotation* rot = new TGeoRotation ("rot", 0., 0., 0.);
    TGeoCombiTrans* posrot = new TGeoCombiTrans (0., 0., 0., rot);// v16a - 270, v17a - 258.75, v18a - 0

    rootTop->AddNode (gdmlTop, 1, posrot);

    gGeoManager->CloseGeometry();
    gGeoManager->CheckOverlaps();
    gGeoManager->PrintOverlaps();
    // Just print the name of the rich volume
    // One may compare it to the file name
    //TGeoNode* richNode = gGeoManager->GetTopVolume()->GetNodes()->At(0);
    //cout << richNode->GetVolume()->GetName() << endl;

    // Extract name to form output file name
    TString richGeoOutFilename;
    if (richGeoFilename.EndsWith(".gdml")) {
        richGeoOutFilename = richGeoFilename(0, richGeoFilename.Length()-5) + ".geo.root";
    } else {
        richGeoOutFilename = richGeoFilename + ".geo.root";
    }

    cout << "Exporting '" << richGeoOutFilename << "'." << endl;

    // Define you output ROOT file HERE
    TFile* outfile = new TFile(richGeoOutFilename, "RECREATE");
    rootTop->Write();
    outfile->Close();
}
