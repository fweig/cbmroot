/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Eoin Clerkin [committer] */

#if !defined(__CLING__)
#include "CbmSetup"
#include "CbmTransport.h"

#include "FairSystemInfo.h"

#include "TCanvas.h"
#include "TStopwatch.h"
#endif

#include <cstdlib>
#include <fstream>
#include <iostream>

#define NODE_DEPTH 25

TGeoNode* node[NODE_DEPTH];
TGeoMedium* med;

TGeoManager* gman;
TGeoVolume* top;

//const char* seekstr1 = "cable";
const char* seekstr2 = "cable";

void scan_geometry(const char* fileName = "test.geo.root")
{

  TString myName = "scan_and_graph";

  TFile file(fileName, "READ");
  if (!file.IsOpen()) {
    std::cerr << "Was a root geo file supplied?" << endl;
    exit(1);
  };

  file.GetListOfKeys()->Print();
  file.GetSize();

  char geo_tag[100];
  strcpy(geo_tag, basename(fileName));
  char* ptr = strstr(geo_tag, ".geo.root");
  *(ptr)    = '\0';

  gman = (TGeoManager*) file.Get("FAIRGeom");
  if (gman != NULL) { top = gman->GetTopVolume(); };

  if (top == NULL) top = (TGeoVolume*) file.Get(geo_tag);
  if (top == NULL) top = (TGeoVolume*) file.Get("FAIRGeom");
  if (top == NULL) top = (TGeoVolume*) file.Get("top");
  if (top == NULL) top = (TGeoVolume*) file.Get("TOP");
  if (top == NULL) top = (TGeoVolume*) file.Get("Top");
  if (top == NULL) top = (TGeoVolume*) file.Get("geometryFromGDML");

  if (top == NULL) {
    std::cerr << "No Top Volume found. Is the TGeoManager or Top Volume unusally named" << endl;
    exit(1);
  }

  FILE* graph = fopen("geo.gv", "w+");
  fprintf(graph, "digraph D {\n");

  TObjArray* nodes = top->GetNodes();
  nodes->Print();

  int i_array[NODE_DEPTH], num_array[NODE_DEPTH], j;
  for (int i = 0; i < NODE_DEPTH; i++)
    i_array[i] = 0;
  for (int i = 0; i < NODE_DEPTH; i++)
    num_array[i] = 0;

  //TGeoNode* node[NODE_DEPTH];
  //TGeoMedium* med;

  j            = 0;
  i_array[0]   = 0;
  num_array[0] = nodes->GetEntries();

  std::cout << "\n" << endl;


  int k = 0;
  while (num_array[0] != 0) {

    k++;  // Only used for stop.
          //if(k==3)break;

    if (j == 0) { node[0] = static_cast<TGeoNode*>(nodes->At(i_array[0])); }
    else {
      node[j] = static_cast<TGeoNode*>(node[j - 1]->GetDaughter(i_array[j]));
    };

    i_array[j]++;  // Update number.
    std::cout << "Node: \t";
    for (int i = 0; i < j + 1; i++)
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

    if (num_array[j + 1] > 0) {
      j++;
      num_array[j + 2] = 0;
    };

    while (i_array[j] == num_array[j]) {
      num_array[j] = 0;
      i_array[j]   = 0;  // Reset the counter before falling back.
      j--;
    };

    if (j >= NODE_DEPTH) {
      std::cerr << "Maximum depth of " << NODE_DEPTH << " exceeded. Increase NODE_DEPTH in header of macro," << endl;
      exit(NODE_DEPTH);
    };
  };

  fprintf(graph, "}\n");
  fclose(graph);


  printf("Processed %d nodes.\n", k);
  std::cout << "Program Ends" << endl;
}  // End of macro
