/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: David Emschermann [committer] */

// from the following website
// https://root.cern.ch/doc/master/classTGeoCtub.html

void targetbox1()
{
  TCanvas* c = new TCanvas("c", "c", 0, 0, 600, 600);
  new TGeoManager("ctub", "poza3");
  TGeoMaterial* mat = new TGeoMaterial("Al", 26.98, 13, 2.7);
  TGeoMedium* med   = new TGeoMedium("MED", 1, mat);
  TGeoVolume* top   = gGeoManager->MakeBox("TOP", med, 100, 100, 100);
  gGeoManager->SetTopVolume(top);

  Double_t angle1 = 190;
  Double_t angle2 = 170;

  //   Double_t angle1 =   0;
  //   Double_t angle2 = 360;

  Double_t theta = 160. * TMath::Pi() / 180.;
  Double_t phi   = 30. * TMath::Pi() / 180.;

  Double_t nlow[3];
  nlow[0] = TMath::Sin(theta) * TMath::Cos(phi);
  nlow[1] = TMath::Sin(theta) * TMath::Sin(phi);
  nlow[2] = TMath::Cos(theta);

  cout << "1 - lx: " << nlow[0] << " ly: " << nlow[1] << " lz: " << nlow[2] << endl;

  nlow[0] = 0;
  nlow[1] = 0;
  nlow[2] = -1;

  cout << "2 - lx: " << nlow[0] << " ly: " << nlow[1] << " lz: " << nlow[2] << endl;


  //   theta = 20.*TMath::Pi()/180.;
  //   phi   = 60.*TMath::Pi()/180.;
  theta = 25. * TMath::Pi() / 180.;
  phi   = 180. * TMath::Pi() / 180.;

  Double_t nhi[3];
  nhi[0] = TMath::Sin(theta) * TMath::Cos(phi);
  nhi[1] = TMath::Sin(theta) * TMath::Sin(phi);
  nhi[2] = TMath::Cos(theta);

  cout << "3 - hx: " << nhi[0] << " hy: " << nhi[1] << " hz: " << nhi[2] << endl;

  // TGeoVolume *ctub = gGeoManager->MakeCtub("CTUB", med, 25, 27, 30, 200, 160, nlow[0], nlow[1], nlow[2], nhi[0], nhi[1], nhi[2]);


  //   // Durchmesser   35.56 cm
  //   // Wanddicke      0.3 cm
  //
  //   // 0.4 + 53.31 + cos(25.*acos(-1.)/180.) * 0.6 = 54.25 cm
  //
  //   53.31 cm - tan(25.*acos(-1.)/180.) * 17.78 cm = 45.019050 cm

  //   // check
  //   60.98 - 45.2 - cos(25.*acos(-1.)/180.) * 0.6 = 15.236215
  //   60.98 cm - 45.2 cm - cos(25.*acos(-1.)/180.) * 0.6 cm = 15.24 cm
  //
  //   15.24 + 38.0 - 7.7 - 0.4 = 45.14
  //   15.24 cm + 38.0 cm - 7.7 cm - 0.4 cm = 45.14 cm
  //
  //   // Laenge       45.1 cm
  //
  //   // offset to left side
  //   38.0 - 7.7 - 0.4 = 29.9
  //
  //   // Offset       29.9 cm

  Double_t rmax   = 35.56 / 2.;
  Double_t rmin   = rmax - 0.3;
  Double_t length = 45.1;
  Double_t offset = 29.9;

  TGeoVolume* ctub = gGeoManager->MakeCtub("CTUB", med, rmin, rmax, length / 2., angle1, angle2, nlow[0], nlow[1],
                                           nlow[2], nhi[0], nhi[1], nhi[2]);
  ctub->SetLineWidth(2);
  TGeoTranslation* tctub = new TGeoTranslation("tctub", 0, 0, -offset + length / 2.);
  top->AddNode(ctub, 1, tctub);


  //   from z= -29.9 to z= 15.2 cm

  Double_t rmin1  = 15.9 / 2.;
  Double_t thick1 = 0.4;

  // upstream cover
  TGeoVolume* cups = gGeoManager->MakeCtub("CUPS", med, rmin1, rmax, thick1 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  cups->SetLineWidth(2);
  TGeoTranslation* tups = new TGeoTranslation("tups", 0, 0, -offset - thick1 / 2.);
  top->AddNode(cups, 1, tups);


  // Hoehe                   35.56 cm
  // Breite   19.62 cm * 2 = 39.24 cm
  // Dicke                    0.60 mm

  Double_t rmin2  = 5.0 / 2.;
  Double_t thick2 = 0.6;

  // downstream cover 1
  //   TGeoVolume *dwst = gGeoManager->MakeEltu("DWST", med, 39.24/2., 35.56/2., thick2/2.);
  //   dwst->SetLineWidth(2);
  //   TGeoRotation* dwrot = new TGeoRotation();
  //   dwrot->RotateY(-25.);
  //   TGeoCombiTrans* tdwst = new TGeoCombiTrans("tdwst", 0, 0, -offset +length +thick2/2., dwrot);
  //   TGeoTranslation* tdwst = new TGeoTranslation("tdwst", 0, 0, -offset +length +thick2/2.);
  //   top->AddNode(dwst, 1, tdwst);


  // downstream cover 2
  TGeoVolume* cdown = gGeoManager->MakeCtub("CDOWN", med, rmin2, rmax, thick2 / 2., angle1, angle2, -nhi[0], -nhi[1],
                                            -nhi[2], nhi[0], nhi[1], nhi[2]);
  cdown->SetLineWidth(2);
  TGeoTranslation* tdown = new TGeoTranslation("tdown", 0, 0, -offset + length + thick2 / 2.);
  top->AddNode(cdown, 1, tdown);


  // tan(25 deg) * 5.4 cm = 2.518 cm
  // tan(25.*acos(-1.)/180.) * 5.4 = 2.5180614
  // 47.12 - tan(25.*acos(-1.)/180.)*5.4 - 0.6 = 44.0 cm
  // Laenge       44.0 cm
  // Durchmesser   5.4 cm
  // Wanddicke     0.2 cm

  Double_t rmax3   = 5.4 / 2.;
  Double_t rmin3   = rmax3 - 0.2;
  Double_t length3 = 44.0;


  // downstream pipe
  TGeoVolume* cpip = gGeoManager->MakeCtub("CPIP", med, rmin2, rmax3, length3 / 2., angle1, angle2, -nhi[0], -nhi[1],
                                           -nhi[2], -nlow[0], -nlow[1], -nlow[2]);
  cpip->SetLineWidth(2);
  TGeoTranslation* tpip = new TGeoTranslation("tpip", 0, 0, -offset + length + thick2 + length3 / 2.);
  top->AddNode(cpip, 1, tpip);


  gGeoManager->CloseGeometry();
  gGeoManager->SetNsegments(80);
  gGeoManager->CheckOverlaps(0.001);

  top->Draw();
  TView* view = gPad->GetView();
  view->ShowAxis();
}
