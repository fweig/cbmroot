/** @file check_media.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

// The macro compares the media for defined TGeoNodes in the mcbm setup with
// the media assigned to the nodes during creation of the feometries of the
// different detector systems.
// Due to the way we build up the complete CBM geometry from the
// several independent geometries of the detector systems it can happen that
// a node gets a wrong media information.

// Forward declaration
std::pair<int, int> loop_over_nodes(const std::vector<std::pair<TString, TString>>&, TString&, TString&);
std::pair<int, int> CheckGeometry(TString);

void check_media(const char* dataset = "test")
{
  TString geoFile = TString(dataset) + ".geo.root";
  TFile* f        = new TFile(geoFile);
  if (!f->IsOpen()) {
    std::cout << "check_media_1: geometry file " << geoFile << " with the ROOT TGeoManager is not accessible!"
              << std::endl;
    return;
  }

  gGeoManager = dynamic_cast<TGeoManager*>(f->Get("FAIRGeom"));
  assert(gGeoManager);


  int wrong_media {0};
  TGeoNode* topNode = gGeoManager->GetTopNode();
  TObjArray* nodes  = topNode->GetNodes();
  for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
    TGeoNode* node   = static_cast<TGeoNode*>(nodes->At(iNode));
    TString nodename = node->GetName();
    if (nodename.Contains("target")) continue;
    std::cout << "Checking node " << nodename << std::endl;
    std::pair<int, int> retval = CheckGeometry(nodename);
    if (-1 == retval.second) {
      RemoveGeoManager();
      return;
    }
    wrong_media += retval.second;
    std::cout << "Checked " << retval.first << " sub nodes from " << nodename << " and found " << retval.second
              << " with wrongly assigned media" << std::endl;
  }

  if (0 == wrong_media) {
    std::cout << std::endl;
    std::cout << "Test passed" << std::endl;
    std::cout << "All ok" << std::endl;
  }
  else {
    std::cout << std::endl;
    std::cout << "Test failed" << std::endl;
    std::cout << "Found in total " << wrong_media << " nodes with wrongly assigned media" << std::endl;
  }
  RemoveGeoManager();
}

std::pair<int, int> CheckGeometry(TString geoname)
{
  // All TOF geometries v16c have the equal internal structure,
  // only the position in the cave is different so we use the
  // same input file with the media at creation
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  TString filename {geoname};
  TString substitution {geoname};
  TString toReplace {};
  if (geoname.Contains("tof_v16c")) {
    filename.Remove(filename.Length() - 5, 5);
    substitution.Remove(substitution.Length() - 2, 2);
    filename  = srcDir + "/input/geometry_check/" + filename + "_1h_geometrycheck.root";
    toReplace = "tof_v16c_1h";
  }
  else if (geoname.Contains("trd_v17n")) {
    filename.Remove(filename.Length() - 5, 5);
    substitution.Remove(substitution.Length() - 2, 2);
    filename  = srcDir + "/input/geometry_check/" + filename + "_1e_geometrycheck.root";
    toReplace = "trd_v17n_1e";
  }
  else {
    filename.Remove(filename.Length() - 2, 2);
    filename     = srcDir + "/input/geometry_check/" + filename + "_geometrycheck.root";
    substitution = "";
    toReplace    = "";
  }

  TFile* infile = TFile::Open(filename);

  if (nullptr == infile) {
    std::cout << "Could not open input file " << filename << std::endl;
    return std::make_pair(-1, -1);
  }
  CbmMediaList* matlistPtr {nullptr};
  infile->GetObject("CbmMediaList", matlistPtr);
  const std::vector<std::pair<TString, TString>>& matlist = matlistPtr->GetVector();

  std::pair<int, int> retval = loop_over_nodes(matlist, substitution, toReplace);

  infile->Close();

  return retval;
}

std::pair<int, int> loop_over_nodes(const std::vector<std::pair<TString, TString>>& matlist, TString& substitution,
                                    TString& toReplace)
{
  int media_checked {0};
  int wrong_media {0};
  TGeoNode* node {nullptr};
  TString medName {""};
  TString nodename {""};
  std::cout << std::endl;
  for (auto material : matlist) {
    media_checked++;
    nodename = material.first;
    if (toReplace.Length() > 0) { nodename = material.first.ReplaceAll(toReplace, substitution); }
    if (gGeoManager->cd(nodename)) {
      node    = gGeoManager->GetCurrentNode();
      medName = node->GetMedium()->GetName();
      if (medName.CompareTo(material.second)) {
        wrong_media++;
        std::cout << "Medium for " << material.first << " is wrong." << std::endl;
        std::cout << "Expected: " << material.second << std::endl;
        std::cout << "Found   : " << medName << std::endl;
        std::cout << std::endl;
        //      } else {
        //       std::cout << "Medium for " << material.first << " is correct." << std::endl;
        //       std::cout << std::endl;
      }
    }
  }
  return std::make_pair(media_checked, wrong_media);
}
