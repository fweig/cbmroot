// @(#)root/eve:$Id$
// Author: Matevz Tadel

// Shows STS geometry.
// for deatils see
// https://root.cern.ch/download/doc/ROOTUsersGuideHTML/ch09s12.html

void geom_sts(const int statid = 1, const char* ststag = "v17a")
{
   TEveManager::Create();

   TFile::SetCacheFileDir(".");

   //   TString thests = "sts_" + ststag.Data() + "_1";
   TString thests = Form("sts_%s_1", ststag);
   std::cout << thests << std::endl;   

   TString thestat = Form("Station%02d_1", statid);
   TString pngstat = Form("sts_%s_station%02d.png", ststag, statid);
   std::cout << thestat << std::endl << std::endl;
 
   gGeoManager = gEve->GetGeometry("http://cbm.uni-muenster.de/sts/more/sts_v17a_geo.root");
//   gGeoManager->DefaultColors();
   
//   TGeoNode* node1 = gGeoManager->GetTopVolume()->FindNode("sts_v17a_1");
//   TEveGeoTopNode* mysts = new TEveGeoTopNode(gGeoManager, node1);
//   mysts->SetVisLevel(6);
//   gEve->AddGlobalElement(mysts);

//   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode("sts_v17a_1")->GetVolume();
//   TEveGeoTopNode* stat1 = new TEveGeoTopNode(gGeoManager, top->FindNode("Station01_1"));
//   stat1->SetVisLevel(6);
//   gEve->AddGlobalElement(stat1);
//   
//   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode("sts_v17a_1")->GetVolume();
//   TEveGeoTopNode* stat2 = new TEveGeoTopNode(gGeoManager, top->FindNode("Station02_1"));
//   stat2->SetVisLevel(6);
//   gEve->AddGlobalElement(stat2);
//   
//   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode("sts_v17a_1")->GetVolume();
//   TEveGeoTopNode* stat3 = new TEveGeoTopNode(gGeoManager, top->FindNode("Station03_1"));
//   stat3->SetVisLevel(6);
//   gEve->AddGlobalElement(stat3);
//   
//   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode("sts_v17a_1")->GetVolume();
//   TEveGeoTopNode* stat4 = new TEveGeoTopNode(gGeoManager, top->FindNode("Station04_1"));
//   stat4->SetVisLevel(6);
//   gEve->AddGlobalElement(stat4);
//      
//   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode("sts_v17a_1")->GetVolume();
//   TEveGeoTopNode* stat5 = new TEveGeoTopNode(gGeoManager, top->FindNode("Station05_1"));
//   stat5->SetVisLevel(6);
//   gEve->AddGlobalElement(stat5);
//   
//   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode("sts_v17a_1")->GetVolume();
//   TEveGeoTopNode* stat6 = new TEveGeoTopNode(gGeoManager, top->FindNode("Station06_1"));
//   stat6->SetVisLevel(6);
//   gEve->AddGlobalElement(stat6);
//   
//   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode("sts_v17a_1")->GetVolume();
//   TEveGeoTopNode* stat7 = new TEveGeoTopNode(gGeoManager, top->FindNode("Station07_1"));
//   stat7->SetVisLevel(6);
//   gEve->AddGlobalElement(stat7);
//   
//   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode("sts_v17a_1")->GetVolume();
//   TEveGeoTopNode* stat8 = new TEveGeoTopNode(gGeoManager, top->FindNode("Station08_1"));
//   stat8->SetVisLevel(6);
//   gEve->AddGlobalElement(stat8);
   
   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode(thests.Data())->GetVolume();
   TEveGeoTopNode* stat = new TEveGeoTopNode(gGeoManager, top->FindNode(thestat.Data()));
   stat->SetVisLevel(6);
   gEve->AddGlobalElement(stat);
   
   gEve->FullRedraw3D(kTRUE);

   TGLViewer *v = gEve->GetDefaultGLViewer();

   // EClipType not exported to CINT (see TGLUtil.h):
   // 0 - no clip, 1 - clip plane, 2 - clip box
   //   v->GetClipSet()->SetClipType(1);
   v->GetClipSet()->SetClipType(0);

/* backgrund color */
   v->SwitchColorSet();
   //  v->SetClearColor(kWhite);

/* axes */
   //   v->SetGuideState(TGLUtil::kAxesEdge, kTRUE, kFALSE, 0);  // Edge Axes
   v->SetGuideState(TGLUtil::kAxesOrigin, kFALSE, kFALSE, 0);  // Origin Axes

/* line style */
   //   v->SetStyle(TGLRnrCtx::kFill);
   v->SetStyle(TGLRnrCtx::kOutline);
   //   v->SetStyle(TGLRnrCtx::kWireFrame);

   v->RefreshPadEditor(v);

//   v->DoDraw();
//   v->SavePicture("sts_1.png");

   //   v->CurrentCamera().RotateRad(-.7, 0.5);
   v->CurrentCamera().RotateRad(0, acos(-1)/2.);  // check from frontside
   v->DoDraw();
   v->SavePicture(pngstat.Data());
   //   v->SavePicture("sts_2.png");

//   v->CurrentCamera().RotateRad(0, acos(-1));  // check from backside
//   v->DoDraw();
//   v->SavePicture("sts_3.png");

}
