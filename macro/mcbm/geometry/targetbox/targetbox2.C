// from the following website
// https://root.cern.ch/doc/master/classTGeoCtub.html

// dimensions from z = -38.0 cm to z = +59.8 cm
// dimensions of core [-29.9 cm .. +15.2 cm]
// dimensions of sections -38.0 cm | + 7.7 + 0.4 + 45.1 + 0.6 + 44.0 | +59.8 cm

void targetbox2()
{
  TCanvas* c = new TCanvas("c", "c", 0, 0, 600, 600);
  new TGeoManager("targetbox", "mCBM targetbox");

  TGeoMaterial* mat = new TGeoMaterial("Al", 26.98, 13, 2.7);
  TGeoMedium* med   = new TGeoMedium("MED", 1, mat);
  TGeoVolume* top   = gGeoManager->MakeBox("top", med, 100, 100, 100);
  gGeoManager->SetTopVolume(top);

  //   Double_t angle1 = 180;  // lower half
  //   Double_t angle2 =   0;  // lower half

  //   Double_t angle1 = 190;  // open cut @ -x
  //   Double_t angle2 = 170;  // open cut @ -x

  Double_t angle1 = 0;    // full object
  Double_t angle2 = 360;  // full object

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

  Double_t rmax20   = 35.56 / 2.;
  Double_t rmin20   = rmax20 - 0.3;
  Double_t length20 = 45.1;
  Double_t offset20 = 29.9;

  TGeoVolume* vpipe20 = gGeoManager->MakeCtub("pipe20", med, rmin20, rmax20, length20 / 2., angle1, angle2, nlow[0],
                                              nlow[1], nlow[2], nhi[0], nhi[1], nhi[2]);
  vpipe20->SetLineWidth(2);
  TGeoTranslation* tra20 = new TGeoTranslation("tra20", 0, 0, -offset20 + length20 / 2.);
  top->AddNode(vpipe20, 1, tra20);


  // upstream cover
  Double_t rmax21   = rmax20;
  Double_t rmin21   = 15.9 / 2.;
  Double_t length21 = 0.4;

  TGeoVolume* vwall21 =
    gGeoManager->MakeCtub("wall21", med, rmin21, rmax21, length21 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  vwall21->SetLineWidth(2);
  TGeoTranslation* tra21 = new TGeoTranslation("tra21", 0, 0, -offset20 - length21 / 2.);
  top->AddNode(vwall21, 1, tra21);

  // DE   // Hoehe                   35.56 cm
  // DE   // Breite   19.62 cm * 2 = 39.24 cm
  // DE   // Dicke                    0.60 mm
  // DE
  // DE
  // DE    // downstream cover 1
  // DE //   TGeoVolume *dwst = gGeoManager->MakeEltu("DWST", med, 39.24/2., 35.56/2., length2/2.);
  // DE //   dwst->SetLineWidth(2);
  // DE //   TGeoRotation* dwrot = new TGeoRotation();
  // DE //   dwrot->RotateY(-25.);
  // DE //   TGeoCombiTrans* tdwst = new TGeoCombiTrans("tdwst", 0, 0, -offset20 +length20 +length2/2., dwrot);
  // DE //   TGeoTranslation* tdwst = new TGeoTranslation("tdwst", 0, 0, -offset20 +length20 +length2/2.);
  // DE //   top->AddNode(dwst, 1, tdwst);
  // DE
  // DE

  // DE2   // downstream cover 2
  // DE2    TGeoVolume *cdown = gGeoManager->MakeCtub("CDOWN", med, rmin2, rmax20, length2/2., angle1, angle2,
  // DE2 					    -nhi[0],-nhi[1],-nhi[2], nhi[0], nhi[1], nhi[2]);
  // DE2    cdown->SetLineWidth(2);
  // DE2    TGeoTranslation* tdown = new TGeoTranslation("tdown", 0, 0, -offset20 +length20 +length2/2.);
  // DE2    top->AddNode(cdown, 1, tdown);


  //=======================================================================================

  // Hoehe                   35.56 cm
  // Breite   19.62 cm * 2 = 39.24 cm
  // Dicke                    0.60 mm

  //   Double_t rmin2   =  5.0 /2.;
  //   Double_t length2  =  0.6;

  //   // downstream cover 1
  //   TGeoVolume *dwst = gGeoManager->MakeEltu("DWST", med, 39.24/2., 35.56/2., length2/2.);
  //   dwst->SetLineWidth(2);
  //   //   TGeoRotation* dwrot = new TGeoRotation();
  //   //   dwrot->RotateY(-25.);
  //   //   TGeoCombiTrans* tdwst = new TGeoCombiTrans("tdwst", 0, 0, -offset20 +length20 +length2/2., dwrot);
  //   TGeoTranslation* tdwst = new TGeoTranslation("tdwst", 0, 0, -offset20 +length20 +length2/2.);
  //   top->AddNode(dwst, 1, tdwst);


  //   TGeoVolume *cir1 = gGeoManager->MakeCtub("CIR1", med, 0, 1, 1/2., 0, 360,
  //					    0, 0,-1, 0, 0, 1);
  //   cir1->SetLineWidth(2);
  //   TGeoTranslation* tcir1 = new TGeoTranslation("tcir1", -(11.5/2.-1.), +(10.0/2.-1.), 20.);
  //   TGeoTranslation* tcir2 = new TGeoTranslation("tcir2", -(11.5/2.-1.), -(10.0/2.-1.), 20.);
  //   TGeoTranslation* tcir3 = new TGeoTranslation("tcir3", +(11.5/2.-1.), +(10.0/2.-1.), 20.);
  //   TGeoTranslation* tcir4 = new TGeoTranslation("tcir4", +(11.5/2.-1.), -(10.0/2.-1.), 20.);
  //   top->AddNode(cir1, 1, tcir1);
  //   top->AddNode(cir1, 2, tcir2);
  //   top->AddNode(cir1, 3, tcir3);
  //   top->AddNode(cir1, 4, tcir4);
  //
  //   TGeoVolume *box1 = gGeoManager->MakeBox("BOX1", med, 11.5/2., 10.0/2.-1., 1/2.);
  //   box1->SetLineWidth(2);
  //   TGeoTranslation* tbox1 = new TGeoTranslation("tbox1", 0., 0., 20.);
  //   top->AddNode(box1, 1, tbox1);
  //
  //   TGeoVolume *box2 = gGeoManager->MakeBox("BOX2", med, 11.5/2.-1., 10.0/2., 1/2.);
  //   box2->SetLineWidth(2);
  //   TGeoTranslation* tbox2 = new TGeoTranslation("tbox2", 0., 0., 20.);
  //   top->AddNode(box2, 1, tbox2);

  //   TGeoTranslation* tb1 = new TGeoTranslation("tb1", 0., 0., 0.);
  //   tb1->RegisterYourself();
  //   TGeoTranslation* tb2 = new TGeoTranslation("tb2", 0., 0., 0.);
  //   tb2->RegisterYourself();

  //   TGeoEltu *dcov = new TGeoEltu(39.24/2., 35.56/2., length2/2.);
  //   dcov->SetName("O");                 // shapes need names too

  Double_t rmax22   = rmax20;
  Double_t rmin22   = 5.4 / 2.;
  Double_t length22 = 0.6 / cos(25. * acos(-1.) / 180.);

  TGeoCtub* cdown =
    new TGeoCtub(rmin22, rmax22, length22 / 2., angle1, angle2, -nhi[0], -nhi[1], -nhi[2], nhi[0], nhi[1], nhi[2]);
  cdown->SetName("O");  // shapes need names too

  // kapton foil frame dimensions - inner dimensions - frame width
  // x/y/z - 13.9/12.4/0.6 cm     - 11.5/10.0 cm     - 1.2 cm

  // corner of pipe at
  // x = -2.70
  // z = 14.54 = tan(25.*acos(-1.)/180.) * -5.4/2. + 15.8

  // corner of frame towards beampipe
  // x = -3.74
  // z = 14.06
  // distance = sqrt( 1.04 * 1.04 + 0.48 * 0.48 ) = 1.14 cm

  TGeoRotation* rot22 = new TGeoRotation();
  rot22->RotateY(-25.);

  TGeoBBox* box22 = new TGeoBBox(11.5 / 2., 10.0 / 2., 2.0 / 2.);
  box22->SetName("A");  // shapes need names too

  //   TGeoTranslation* tcut = new TGeoTranslation("tcut", -10.0, 0., 0.);
  //   TGeoCombiTrans*  tcut = new TGeoCombiTrans("tcut", -10.0, 0., 0., rot22);
  TGeoCombiTrans* tcut =
    new TGeoCombiTrans("tcut", cos(25. * acos(-1.) / 180.) * -10.0, 0., sin(25. * acos(-1.) / 180.) * -10.0, rot22);
  tcut->RegisterYourself();

  Double_t fthick22 = 0.6;
  TGeoBBox* frame22 = new TGeoBBox(13.9 / 2., 12.4 / 2., fthick22 / 2.);
  frame22->SetName("F");  // shapes need names too

  TGeoCombiTrans* tfra =
    new TGeoCombiTrans("tfra", cos(25. * acos(-1.) / 180.) * -10.0 - sin(25. * acos(-1.) / 180.) * fthick22, 0.,
                       sin(25. * acos(-1.) / 180.) * -10.0 + cos(25. * acos(-1.) / 180.) * fthick22
                         - 0.0001,  // 0.0001 avoids optical error
                       rot22);
  tfra->RegisterYourself();

  //   TGeoCompositeShape *compsha = new TGeoCompositeShape("compsha", "O - A:tcut");
  //   TGeoCompositeShape *compsha = new TGeoCompositeShape("compsha", "O + F:tfra");
  TGeoCompositeShape* compsha = new TGeoCompositeShape("compsha", "O + F:tfra - A:tcut");

  TGeoVolume* vpipe22 = new TGeoVolume("wall22", compsha, med);

  TGeoTranslation* tra22 = new TGeoTranslation("tra22", 0, 0, -offset20 + length20 + length22 / 2.);
  top->AddNode(vpipe22, 1, tra22);

  //   // test the thickness
  //   TGeoRotation* back22 = new TGeoRotation();
  //   back22->RotateY(+25.);
  //   TGeoCombiTrans* ztra22 = new TGeoCombiTrans("ztra22", 0, 0, +0.6/2., back22);
  //   top->AddNode(vpipe22, 1, ztra22);

  //=======================================================================================

  // vertical tubes
  Double_t height23   = 28.0 - 17.8;
  TGeoRotation* rot23 = new TGeoRotation();
  rot23->RotateX(90.);

  // target tube
  TGeoVolume* tube23 = gGeoManager->MakeTube("shaft23", med, 10.4 / 2., 10.8 / 2., height23 / 2.);
  tube23->SetLineWidth(2);
  TGeoCombiTrans* tra23 = new TGeoCombiTrans("tra23", 0, 28.0 - height23 / 2., 0, rot23);
  top->AddNode(tube23, 1, tra23);

  // diamond tube
  TGeoVolume* tube24 = gGeoManager->MakeTube("shaft24", med, 10.4 / 2., 10.8 / 2., height23 / 2.);
  tube24->SetLineWidth(2);
  TGeoCombiTrans* tra24 = new TGeoCombiTrans("tra24", 0, 28.0 - height23 / 2., -20.0, rot23);
  top->AddNode(tube24, 1, tra24);

  //=======================================================================================

  // upstream pipe
  Double_t rmax10   = 15.9 / 2.;
  Double_t rmin10   = rmax10 - 0.2;
  Double_t length10 = 7.7 + length21;

  TGeoVolume* vpipe10 =
    gGeoManager->MakeCtub("pipe10", med, rmin10, rmax10, length10 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  vpipe10->SetLineWidth(2);
  TGeoTranslation* tra10 = new TGeoTranslation("tra10", 0, 0, -offset20 - length10 / 2.);
  top->AddNode(vpipe10, 1, tra10);


  // upstream flange

  // size of flange
  // Diameter  19.9 cm
  // Thickness  1.8 cm

  Double_t rmax11   = 19.9 / 2.;
  Double_t rmin11   = rmax10;
  Double_t length11 = 1.8;

  TGeoVolume* vfla11 =
    gGeoManager->MakeCtub("flange11", med, rmin11, rmax11, length11 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  vfla11->SetLineWidth(2);
  TGeoTranslation* tra11 = new TGeoTranslation("tra11", 0, 0, -offset20 - length10 + length11 / 2.);
  top->AddNode(vfla11, 1, tra11);

  //=======================================================================================

  // tan(25 deg) * 5.4 cm = 2.518 cm
  // tan(25.*acos(-1.)/180.) * 5.4 = 2.5180614
  // 47.12 - tan(25.*acos(-1.)/180.)*5.4 - 0.6 = 44.0 cm
  // Laenge       44.0 cm
  // Durchmesser   5.4 cm
  // Wanddicke     0.2 cm

  // downstream pipe
  Double_t rmax30   = 5.4 / 2.;
  Double_t rmin30   = rmax30 - 0.2;
  Double_t length30 = 44.0 + 0.6;

  TGeoVolume* vpipe30 = gGeoManager->MakeCtub("pipe30", med, rmin30, rmax30, length30 / 2., angle1, angle2, -nhi[0],
                                              -nhi[1], -nhi[2], -nlow[0], -nlow[1], -nlow[2]);
  vpipe30->SetLineWidth(2);
  TGeoTranslation* tra30 = new TGeoTranslation("tra30", 0, 0, -offset20 + length20 + length30 / 2.);
  top->AddNode(vpipe30, 1, tra30);


  // size of flange
  // Thickness  1.8 cm
  // Diameter  11.4 cm

  Double_t rmax40   = 11.4 / 2.;
  Double_t rmin40   = rmax30;
  Double_t length40 = 1.8;

  // downstream flange
  TGeoVolume* vfla31 =
    gGeoManager->MakeCtub("flange31", med, rmin40, rmax40, length40 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  vfla31->SetLineWidth(2);
  TGeoTranslation* fla31 = new TGeoTranslation("fla31", 0, 0, -offset20 + length20 + length30 - length40 / 2.);
  top->AddNode(vfla31, 1, fla31);

  //=======================================================================================

  gGeoManager->CloseGeometry();
  gGeoManager->SetNsegments(80);
  gGeoManager->CheckOverlaps(0.001);

  top->Draw();

  //   TView *view = gPad->GetView();
  //   view->ShowAxis();
}
