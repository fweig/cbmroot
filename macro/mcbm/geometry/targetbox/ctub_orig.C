/** @file ctub_orig.C
  * @copyright Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors David Emschermann [orginator] **/

// from the following website
// https://root.cern.ch/doc/master/classTGeoCtub.html

void ctub_orig()
{
  TCanvas* c = new TCanvas("c", "c", 0, 0, 600, 600);
  new TGeoManager("ctub", "poza3");
  TGeoMaterial* mat = new TGeoMaterial("Al", 26.98, 13, 2.7);
  TGeoMedium* med   = new TGeoMedium("MED", 1, mat);
  TGeoVolume* top   = gGeoManager->MakeBox("TOP", med, 100, 100, 100);
  gGeoManager->SetTopVolume(top);
  Double_t theta = 160. * TMath::Pi() / 180.;
  Double_t phi   = 30. * TMath::Pi() / 180.;
  Double_t nlow[3];
  nlow[0] = TMath::Sin(theta) * TMath::Cos(phi);
  nlow[1] = TMath::Sin(theta) * TMath::Sin(phi);
  nlow[2] = TMath::Cos(theta);
  theta   = 20. * TMath::Pi() / 180.;
  phi     = 60. * TMath::Pi() / 180.;
  Double_t nhi[3];
  nhi[0] = TMath::Sin(theta) * TMath::Cos(phi);
  nhi[1] = TMath::Sin(theta) * TMath::Sin(phi);
  nhi[2] = TMath::Cos(theta);
  TGeoVolume* vol =
    gGeoManager->MakeCtub("CTUB", med, 20, 30, 40, -30, 250, nlow[0], nlow[1], nlow[2], nhi[0], nhi[1], nhi[2]);
  vol->SetLineWidth(2);
  top->AddNode(vol, 1);
  gGeoManager->CloseGeometry();
  gGeoManager->SetNsegments(80);
  top->Draw();
  TView* view = gPad->GetView();
  view->ShowAxis();
}
