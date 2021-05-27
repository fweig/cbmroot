//forward declaration
void loop_over_nodes(TObjArray*, TString&, std::map<TString, TVector3>&);

void create_positionlist(TString inFileName = "")
{

  if (inFileName.Length() > 0) {
    cout << "Open file " << inFileName << endl;
    TFile* f = new TFile(inFileName);
    if (!f->IsOpen()) {
      std::cout << "create_tgeonode_list: geometry file " << inFileName << " is not accessible!" << std::endl;
      return;
    }
    gGeoManager = (TGeoManager*) f->Get("FAIRGeom");
  }

  std::map<TString, TVector3> matlist;

  TGeoNode* topNode = gGeoManager->GetTopNode();
  TString Path      = topNode->GetName();
  ;
  TObjArray* nodes = topNode->GetNodes();
  if (1 != nodes->GetEntriesFast()) {
    std::cerr << "There should be only one node in the top node which is the "
                 "detector keeping volume"
              << std::endl;
    return;
  }
  TGeoNode* node = static_cast<TGeoNode*>(nodes->At(0));
  TString TopNodeName {node->GetName()};
  Path = "/" + Path + "/" + TopNodeName + "/";

  std::cout << "{\"" << Path << "\", \"" << node->GetMedium()->GetName() << "\"}," << std::endl;

  gGeoManager->cd(Path);
  TGeoMatrix* globalmatrix = gGeoManager->GetCurrentMatrix();

  Double_t local[3] {0., 0., 0.};
  Double_t global[3] {0., 0., 0.};
  globalmatrix->LocalToMaster(local, global);

  matlist[Path] = TVector3(global[0], global[1], global[2]);

  TObjArray* detectornodes = node->GetNodes();
  loop_over_nodes(detectornodes, Path, matlist);

  ofstream myfile;
  myfile.open("example.txt");
  for (auto& info : matlist) {
    std::cout << info.first << ", " << info.second.X() << ", " << info.second.Y() << ", " << info.second.Z()
              << std::endl;
    myfile << info.first << ", " << info.second.X() << ", " << info.second.Y() << ", " << info.second.Z() << "\n";
  }
  myfile.close();

  RemoveGeoManager();
}

void loop_over_nodes(TObjArray* nodes, TString& path, std::map<TString, TVector3>& matlist)
{
  for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
    TGeoNode* node   = static_cast<TGeoNode*>(nodes->At(iNode));
    TString Fullpath = path + node->GetName() + "/";

    gGeoManager->cd(Fullpath);
    TGeoMatrix* globalmatrix = gGeoManager->GetCurrentMatrix();

    Double_t local[3] {0., 0., 0.};
    Double_t global[3] {0., 0., 0.};
    globalmatrix->LocalToMaster(local, global);

    matlist[Fullpath] = TVector3(global[0], global[1], global[2]);

    TObjArray* subnodes = node->GetNodes();
    if (nullptr != subnodes) { loop_over_nodes(subnodes, Fullpath, matlist); }
  }
}
