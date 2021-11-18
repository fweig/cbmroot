/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

//forward declaration
void loop_over_nodes(TObjArray*, TString&, CbmMediaList&);

void create_medialist(TString inFileName = "", bool removegeomgr = true)
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

  CbmMediaList matlist;

  TGeoNode* topNode = gGeoManager->GetTopNode();
  TString Path {"/cave_1/"};
  TObjArray* nodes = topNode->GetNodes();
  if (1 != nodes->GetEntriesFast()) {
    std::cerr << "There should be only one node in the top node which is the "
                 "detector keeping volume"
              << std::endl;
    return;
  }
  TGeoNode* node = static_cast<TGeoNode*>(nodes->At(0));
  TString TopNodeName {node->GetName()};
  // Replace the trailing _1 by _0 which is the correct number in the full geometry
  TopNodeName.Replace(TopNodeName.Length() - 1, 1, "0");
  Path = Path + TopNodeName + "/";

  std::cout << "{\"" << Path << "\", \"" << node->GetMedium()->GetName() << "\"}," << std::endl;
  matlist.AddEntry(Path, node->GetMedium()->GetName());
  TObjArray* detectornodes = node->GetNodes();
  loop_over_nodes(detectornodes, Path, matlist);

  for (auto& info : matlist.GetVector()) {
    std::cout << info.first << ", " << info.second << std::endl;
  }

  TString filename {TopNodeName};
  filename.Remove(filename.Length() - 2, 2);
  filename += "_geometrycheck.root";

  std::cout << "Filename: " << filename << std::endl;
  TFile* outfile = new TFile(filename, "RECREATE");

  matlist.Write();
  outfile->Close();

  if (removegeomgr)
    RemoveGeoManager();
}

void loop_over_nodes(TObjArray* nodes, TString& path, CbmMediaList& matlist)
{
  for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
    TGeoNode* node   = static_cast<TGeoNode*>(nodes->At(iNode));
    TString Fullpath = path + node->GetName() + "/";
    matlist.AddEntry(Fullpath, node->GetMedium()->GetName());
    TObjArray* subnodes = node->GetNodes();
    if (nullptr != subnodes) { loop_over_nodes(subnodes, Fullpath, matlist); }
  }
}
