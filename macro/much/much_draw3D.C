/** @file much_draw3D.C
  * @copyright Copyright (C) 2020 Variable Energy Cyclotron Centre, Kolkata
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Vikas Singhal [orginator] **/

void much_draw3D(TString geofile = "geofile.root")
{


  TFile* f = new TFile(geofile);
  f->Get("FairBaseParSet");
  TGeoManager* gGeoManager = (TGeoManager*) f->Get("FAIRGeom");
  gGeoManager->SetVisLevel(0);

  // Check overlaps
  gGeoManager->CheckOverlaps(0.0000001);
  gGeoManager->PrintOverlaps();

  TGeoVolume* master = gGeoManager->GetMasterVolume();

  //Draw all
  master->Draw("ogl");

  // Draw much
  TGeoVolume* much = master->FindNode("much_v17b_sis100_1m_lmvm_hDcarbon_0")->GetVolume();
  //much->Draw("ogl");
  TGeoVolume* station = much->FindNode("station_1")->GetVolume();
  //station->Draw("oglsame");

  // Draw pipe
  TGeoVolume* pipe = master->FindNode("pipe_v18_v2.AuAu12AGeV_0")->GetVolume();
  //pipe->Draw("oglsame");

  f->Close();
}
