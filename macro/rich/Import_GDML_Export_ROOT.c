
void addCopyNumbersToNodes(TGeoVolume* volume);


void Import_GDML_Export_ROOT()
{

  TString richGeoFilename = "rich_v21a.gdml";
  cout << "Importing '" << richGeoFilename << "'." << endl;

  // Load FairRunSim to ensure the correct unit system
  FairRunSim* sim = new FairRunSim();

  // Since the GDML file does not contain any information about the materials beside the
  // material name, the material have to be created prior to the import of the GDML
  // geometry from the media.geo file. This is done using the FairRoot tolls

  // -------   Load media from media file   -----------------------------------
  FairGeoLoader* geoLoad    = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath           = gSystem->Getenv("VMCWORKDIR");
  TString medFile           = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();

  // -----------------   Get and create the required media    -----------------
  FairGeoMedia* geoMedia   = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // ---> CsI
  FairGeoMedium* mCsI = geoMedia->getMedium("CsI");
  if (!mCsI) Fatal("Main", "FairMedium CsI not found");
  geoBuild->createMedium(mCsI);
  TGeoMedium* CsI = gGeoManager->GetMedium("CsI");
  if (!CsI) Fatal("Main", "Medium CsI not found");

  // ---> iron
  FairGeoMedium* miron = geoMedia->getMedium("iron");
  if (!miron) Fatal("Main", "FairMedium iron not found");
  geoBuild->createMedium(miron);
  TGeoMedium* iron = gGeoManager->GetMedium("iron");
  if (!iron) Fatal("Main", "Medium iron not found");

  // ---> vacuum
  FairGeoMedium* mvacuum = geoMedia->getMedium("vacuum");
  if (!mvacuum) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(mvacuum);
  TGeoMedium* vacuum = gGeoManager->GetMedium("vacuum");
  if (!vacuum) Fatal("Main", "Medium vacuum not found");

  // ---> kapton
  FairGeoMedium* mkapton = geoMedia->getMedium("kapton");
  if (!mkapton) Fatal("Main", "FairMedium kapton not found");
  geoBuild->createMedium(mkapton);
  TGeoMedium* kapton = gGeoManager->GetMedium("kapton");
  if (!kapton) Fatal("Main", "Medium kapton not found");

  // ---> aluminium
  FairGeoMedium* mAluminium = geoMedia->getMedium("aluminium");
  if (!mAluminium) Fatal("Main", "FairMedium aluminium not found");
  geoBuild->createMedium(mAluminium);
  TGeoMedium* aluminium = gGeoManager->GetMedium("aluminium");
  if (!aluminium) Fatal("Main", "Medium aluminium not found");

  // ---> PMTglass
  FairGeoMedium* mPMTglass = geoMedia->getMedium("PMTglass");
  if (!mPMTglass) Fatal("Main", "FairMedium PMTglass not found");
  geoBuild->createMedium(mPMTglass);
  TGeoMedium* PMTglass = gGeoManager->GetMedium("PMTglass");
  if (!PMTglass) Fatal("Main", "Medium PMTglass not found");

  // ---> RICHglass
  FairGeoMedium* mRICHglass = geoMedia->getMedium("RICHglass");
  if (!mRICHglass) Fatal("Main", "FairMedium RICHglass not found");
  geoBuild->createMedium(mRICHglass);
  TGeoMedium* RICHglass = gGeoManager->GetMedium("RICHglass");
  if (!RICHglass) Fatal("Main", "Medium RICHglass not found");

  // --->  RICHgas_CO2_dis
  FairGeoMedium* mRICHgas_CO2_dis = geoMedia->getMedium("RICHgas_CO2_dis");
  if (!mRICHgas_CO2_dis) Fatal("Main", "FairMedium RICHgas_CO2_dis not found");
  geoBuild->createMedium(mRICHgas_CO2_dis);
  TGeoMedium* RICHgas_CO2_dis = gGeoManager->GetMedium("RICHgas_CO2_dis");
  if (!RICHgas_CO2_dis) Fatal("Main", "Medium RICHgas_CO2_dis not found");

  // --->  RICHgas_CO2_dis+
  FairGeoMedium* mRICHgas_CO2_disp = geoMedia->getMedium("RICHgas_CO2_dis+");
  if (!mRICHgas_CO2_disp) Fatal("Main", "FairMedium RICHgas_CO2_dis+ not found");
  geoBuild->createMedium(mRICHgas_CO2_disp);
  TGeoMedium* RICHgas_CO2_disp = gGeoManager->GetMedium("RICHgas_CO2_dis+");
  if (!RICHgas_CO2_disp) Fatal("Main", "Medium RICHgas_CO2_dis+ not found");

  TGDMLParse parser;
  // Define your input GDML file HERE
  TGeoVolume* gdmlTop = parser.GDMLReadFile(richGeoFilename);
  TGeoVolume* rootTop = new TGeoVolumeAssembly("TOP");

  gdmlTop->Print();
  gGeoManager->SetTopVolume(rootTop);
  gGeoManager->SetAllIndex();

  // Starting from the version v18a position is defined inside the GDML file
  // Define your position HERE
  // Z coordinate for v16a = 270, for v17a = 258.75, for v18a = 0.
  TGeoRotation* rot      = new TGeoRotation("rot", 0., 0., 0.);
  TGeoCombiTrans* posrot = new TGeoCombiTrans(0., 0., 218.75, rot);  // v16a - 270, v17a - 258.75, v18a - 0

  rootTop->AddNode(gdmlTop, 1, posrot);

  addCopyNumbersToNodes(gGeoManager->GetTopNode()->GetVolume());

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
    richGeoOutFilename = richGeoFilename(0, richGeoFilename.Length() - 5) + ".geo.root";
  }
  else {
    richGeoOutFilename = richGeoFilename + ".geo.root";
  }

  cout << "Exporting '" << richGeoOutFilename << "'." << endl;

  // Export RICH geometry to the output ROOT file
  gdmlTop->Export(richGeoOutFilename);
  // Write the transformation matrix into the output file
  TFile* outfile = new TFile(richGeoOutFilename, "UPDATE");
  posrot->Write();
  outfile->Close();

  // create medialist for this geometry
  TString createmedialist = gSystem->Getenv("VMCWORKDIR");
  createmedialist += "/macro/geometry/create_medialist.C";
  std::cout << "Loading macro " << createmedialist << std::endl;
  gROOT->LoadMacro(createmedialist);
  gROOT->ProcessLine("create_medialist(\"\", false)");
}


void addCopyNumbersToNodes(TGeoVolume* volume)
{
  map<string, int> counterMap;
  TGeoIterator geoIterator(volume);
  geoIterator.SetType(1);
  geoIterator.Reset();
  TGeoNode* curNode;
  while ((curNode = geoIterator())) {
    string volumeName = string(curNode->GetVolume()->GetName());
    counterMap[volumeName]++;
    int curCounter = counterMap[volumeName];
    curNode->SetNumber(curCounter);
    curNode->SetName((volumeName + "_" + to_string(curCounter)).c_str());
    addCopyNumbersToNodes(curNode->GetVolume());
  }
}
