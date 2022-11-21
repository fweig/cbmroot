/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Eoin Clerkin [committer] */

#if !defined(__CLING__)
#include "CbmSetup"
#include "CbmTransport.h"

#include "FairSystemInfo.h"

#include "TCanvas.h"
#include "TStopwatch.h"
#endif  // !defined(__CLING__)

#include <cstdlib>
#include <fstream>
#include <iostream>

void scan_geometry(std::string fileName = "test.geo.root", uint32_t uMaxNodeDepth = 25)
{
  TString myName = "scan_and_graph";

  std::vector<TGeoNode*> node(uMaxNodeDepth, nullptr);
  TGeoMedium* med = nullptr;
  TGeoVolume* top = nullptr;

  TFile file(fileName.c_str(), "READ");
  if (!file.IsOpen()) {
    std::cerr << "Was a root geo file supplied?" << std::endl;
    exit(1);
  };
  gROOT->cd();

  file.GetListOfKeys()->Print();
  file.GetSize();

  file.GetObject("FAIRGeom", gGeoManager);

  if (gGeoManager != nullptr) { top = gGeoManager->GetTopVolume(); };
  gROOT->cd();

  if (top == nullptr) {
    std::string geo_tag = fileName.substr(fileName.find_last_of('/') + 1);
    geo_tag             = geo_tag.substr(0, geo_tag.find(".geo.root"));
    std::cout << "geo tag extracted from file name is: " << geo_tag << std::endl;
    file.GetObject(geo_tag.data(), top);

    // If still not there, may be an mCBM geometry not following the filename = tag convention
    if (top == nullptr) {
      std::string geo_tag_mcbm = geo_tag.substr(0, geo_tag.find("_mcbm"));
      std::cout << "mcbm geo tag extracted from file name is: " << geo_tag_mcbm << std::endl;
      file.GetObject(geo_tag_mcbm.data(), top);
    }

    // If still not there, may be a SIS18 geometry not following the filename = tag convention
    if (top == nullptr) {
      std::string geo_tag_sis18 = geo_tag.substr(0, geo_tag.find("_sis18"));
      std::cout << "sis18 geo tag extracted from file name is: " << geo_tag_sis18 << std::endl;
      file.GetObject(geo_tag_sis18.data(), top);
    }
  }
  if (top == nullptr) file.GetObject("FAIRGeom", top);
  if (top == nullptr) file.GetObject("top", top);
  if (top == nullptr) file.GetObject("TOP", top);
  if (top == nullptr) file.GetObject("Top", top);
  if (top == nullptr) file.GetObject("geometryFromGDML", top);

  if (top == nullptr) top = dynamic_cast<TGeoVolume*>(file.Get(file.GetListOfKeys()->First()->GetName()));

  gROOT->cd();

  if (top == nullptr) {
    std::cerr << "No Top Volume found. Is the TGeoManager or Top Volume unusally named" << std::endl;
    exit(1);
  }

  TObjArray* nodes = top->GetNodes();
  if (nodes == nullptr) {
    std::cerr << "Could not get the Nodes." << std::endl;
    exit(1);
  }
  nodes->Print();
  gROOT->cd();

  std::vector<int32_t> i_array(uMaxNodeDepth, 0);
  std::vector<int32_t> num_array(uMaxNodeDepth + 2, 0);
  int32_t j    = 0;
  num_array[0] = nodes->GetEntries();

  std::cout << "\n" << endl;

  FILE* graph = fopen("geo.gv", "w+");
  fprintf(graph, "digraph D {\n");

  int k = 0;
  while (num_array[0] != 0 && 0 <= j) {

    k++;  // Only used for stop.
          //if(k==3)break;

    if (j == 0) { node[0] = static_cast<TGeoNode*>(nodes->At(i_array[0])); }
    else {
      node[j] = static_cast<TGeoNode*>(node[j - 1]->GetDaughter(i_array[j]));
    };

    i_array[j]++;  // Update number.
    std::cout << "Node: \t";
    for (int i = 0; i < j + 1 && i < uMaxNodeDepth; i++)
      printf("%d/%d \t", i_array[i], num_array[i]);
    printf("\n");

    fprintf(graph, "%s -> %s \n", node[j]->GetMotherVolume()->GetName(), node[j]->GetVolume()->GetName());

    std::cout << "Name: ";
    node[j]->Print();
    //std::cout << "Mother: ";
    //node[j]->GetMotherVolume()->Print();
    std::cout << "Shape: ";
    node[j]->GetVolume()->GetShape()->Print();
    std::cout << "Medium: ";
    med = node[j]->GetMedium();
    med->Print();
    med->GetMaterial()->Print();
    std::cout << "Position: ";
    node[j]->GetMatrix()->Print();
    num_array[j + 1] = node[j]->GetNdaughters();
    std::cout << "Number of daughters: " << num_array[j + 1];
    std::cout << endl << endl << endl;

    if (j + 1 < uMaxNodeDepth && num_array[j + 1] > 0) {
      j++;
      num_array[j + 2] = 0;
    };

    while (0 <= j && i_array[j] == num_array[j]) {
      num_array[j] = 0;
      i_array[j]   = 0;  // Reset the counter before falling back.
      j--;
    };

    if (0 <= j && j >= uMaxNodeDepth) {
      std::cerr << "Maximum depth of " << uMaxNodeDepth << " exceeded (" << j
                << "). Increase uMaxNodeDepth which is the second parameter of the macro" << std::endl;
      exit(uMaxNodeDepth);
    };
  };

  fprintf(graph, "}\n");
  fclose(graph);

  gROOT->cd();
  file.Close();

  RemoveGeoManager();

  printf("Processed %d nodes.\n", k);
  std::cout << "Program Ends" << std::endl;
}  // End of macro
