#include <iostream>
using namespace std; 

// Changelog
//
// 2017-11-17 - v18d - DE - add aerogel as radiator to the mRICH module
// 2017-11-12 - v18c - DE - push mRICH downstream to z=355 cm for long setup
// 2017-07-19 - v18b - DE - add one level to the geometry hierarchy
// 2017-07-15 - v18b - DE - arrange 4x mRICH detectors in the setup and tilt towards target


enum RichGeomType{kGlassLense, kQuarzPlate, kAerogel};

void create_rich_v18d_gp_mcbm()
{
	gSystem->Load("libGeom");
	//gGeoMan = gGeoManager;// (TGeoManager*)gROOT->FindObject("FAIRGeom");
	//new TGeoManager ("Testbox", "Testbox");

	RichGeomType richGeomType = kAerogel;

	TString geoFileName= "rich_v18d_mcbm.geo.root";

	FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
	FairGeoInterface* geoFace = geoLoad->getGeoInterface();
//	TString geoPath = gSystem->Getenv("VMCWORKDIR");
//	TString medFile = geoPath + "/geometry/media.geo";
	TString medFile = "/home/aghoehne/Documents/CbmRoot/trunkNew/geometry/media.geo";

	geoFace->setMediaFile(medFile);
	geoFace->readMedia();
	gGeoMan = gGeoManager;

	FairGeoMedia*   geoMedia = geoFace->getMedia();
	FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

	//Media
	FairGeoMedium* mCoating = geoMedia->getMedium("RICHglass");
	if (mCoating == NULL) Fatal("Main", "FairMedium RICHglass not found");
	geoBuild->createMedium(mCoating);
	TGeoMedium* medCoating = gGeoMan->GetMedium("RICHglass");
	if (medCoating == NULL) Fatal("Main", "Medium RICHglass not found");

	FairGeoMedium* mAluminium = geoMedia->getMedium("aluminium");
	if (mAluminium == NULL) Fatal("Main", "FairMedium aluminium not found");
	geoBuild->createMedium(mAluminium);
	TGeoMedium* medAl = gGeoMan->GetMedium("aluminium");
	if (medAl == NULL) Fatal("Main", "Medium vacuum not found");

	FairGeoMedium* mVacuum = geoMedia->getMedium("vacuum");
	if (mVacuum == NULL) Fatal("Main", "FairMedium vacuum not found");
	geoBuild->createMedium(mVacuum);
	TGeoMedium* medVacuum = gGeoMan->GetMedium("vacuum");
	if (medVacuum == NULL) Fatal("Main", "Medium vacuum not found");

	FairGeoMedium* mAir = geoMedia->getMedium("air");
	if (mAir == NULL) Fatal("Main", "FairMedium air not found");
	geoBuild->createMedium(mAir);
	TGeoMedium* medAir = gGeoMan->GetMedium("air");
	if (medAir == NULL) Fatal("Main", "Medium air not found");

	FairGeoMedium* mNitrogen = geoMedia->getMedium("RICHgas_N2_dis");
	if (mNitrogen == NULL) Fatal("Main", "FairMedium RICHgas_N2_dis not found");
	geoBuild->createMedium(mNitrogen);
	TGeoMedium* medNitrogen = gGeoMan->GetMedium("RICHgas_N2_dis");
	if (medNitrogen == NULL) Fatal("Main", "Medium RICHgas_N2_dis not found");

	FairGeoMedium* mPMT = geoMedia->getMedium("CsI");
	if (mPMT == NULL) Fatal("Main", "FairMedium CsI not found");
	geoBuild->createMedium(mPMT);
	TGeoMedium* medCsI = gGeoMan->GetMedium("CsI");
	if (medCsI == NULL) Fatal("Main", "Medium CsI not found");

	FairGeoMedium* mLenseGlass = geoMedia->getMedium("Rich_NBK7_glass");
	if (mLenseGlass == NULL) Fatal("Main", "FairMedium Rich_NBK7_glass not found");
	geoBuild->createMedium(mLenseGlass);
	TGeoMedium* medLenseGlass = gGeoMan->GetMedium("Rich_NBK7_glass");
	if (medLenseGlass == NULL) Fatal("Main", "Medium Rich_NBK7_glass  not found");

	//	FairGeoMedium* mQuarz = geoMedia->getMedium("Rich_Quarz");
/*	FairGeoMedium* mQuarz = geoMedia->getMedium("air");   // there is no Rich_Quarz in media.geo
	if (mQuarz == NULL) Fatal("Main", "FairMedium Rich_Quarz not found");
	geoBuild->createMedium(mQuarz);
	//	TGeoMedium* medQuarz = gGeoMan->GetMedium("Rich_Quarz");
	TGeoMedium* medQuarz = gGeoMan->GetMedium("air");   // there is no Rich_Quarz in media.geo
	if (medQuarz == NULL) Fatal("Main", "Medium Rich_Quarz  not found");
*/
	FairGeoMedium* mElectronic = geoMedia->getMedium("air");
	if (mElectronic == NULL) Fatal("Main", "FairMedium air not found");
	geoBuild->createMedium(mElectronic);
	TGeoMedium* medElectronic = gGeoMan->GetMedium("air");
	if (medElectronic == NULL) Fatal("Main", "Medium air not found");

  	FairGeoMedium* mAerogel = geoMedia->getMedium("aerogel");
  	if (mAerogel == NULL) Fatal("Main", "FairMedium aerogel not found");
  	geoBuild->createMedium(mAerogel);
  	TGeoMedium* medAerogel = gGeoMan->GetMedium("aerogel");
  	if (medAerogel == NULL) Fatal("Main", "Medium aerogel  not found");

	//Dimensions of the RICH prototype [cm]
	// Box
	const Double_t boxLength = 44;
	const Double_t boxWidth = 25;
	const Double_t boxHeight = 30;
	const Double_t wallWidth = 0.3;
	const Double_t boxThickness = 0.1;

	// PMT
	// PMT specs https://www.hamamatsu.com/resources/pdf/etd/H12700_TPMH1348E.pdf
	const Double_t pmtNofPixels = 8;
	const Double_t pmtSize = 5.2;
	const Double_t pmtSizeHalf = pmtSize / 2.;
	const Double_t pmtSensitiveSize = 4.85;
	const Double_t pmtPixelSize = pmtSensitiveSize / pmtNofPixels;
	const Double_t pmtPixelSizeHalf = pmtPixelSize / 2.;
	const Double_t pmtGap = 0.1;
	const Double_t pmtGapHalf = pmtGap / 2.;
	const Double_t pmtPadding = 0.175;
	const Double_t pmtMatrixGap = 0.3; //3.5;
	const Double_t pmtThickness = 0.1;

	// Electronics
	const Double_t elecLength = 10.;
	const Double_t elecWidth = 3. * pmtSize;
	const Double_t elecHeight = 2. * pmtSize;

	// Lense
	const Double_t lenseCThickness = 2.44;
	const Double_t lenseRadius = 15.51;
	const Double_t lensePmtDistance = 3.0;
	const Double_t lenseCoating = 0.1;

	// Quarz plate
	const Double_t QuarzThickness = 0.3;
	const Double_t QuarzWidth = 5.;
	const Double_t QuarzHeight = 5.;
	const Double_t QuarzPmtDistance = 2.9;

	// Absorber
	const Double_t absorberThickness = 0.1;
	const Double_t absorberRadius = 0.0;// 2.0;		

	// Aerogel Box
	const Double_t aerogelLength = 2.;
	const Double_t aerogelWidth = 20.;
	const Double_t aerogelHeight = 20.;
	const Double_t aerogelPmtDistance = 10.;

	// Imaginary sensitive plane
	Bool_t isIncludeSensPlane = false;
	const Double_t sensPlaneSize = 200.;
	const Double_t sensPlaneBoxDistance = 1.;

	TGeoRotation *rotBox= new TGeoRotation("rotBox", 0., 0., 0.);

  	Double_t xPos[4] = {-48, -18, -48, -18};   // {-20,  20, -20,  20};  
  	Double_t yPos[4] = {-24, -24,  24,  24};   // {-20, -20,  20,  20};
  	Double_t zPos[4] = {355, 355, 355, 355};   // { 50,  50,  50,  50};
        Double_t proto_angle[4];
      	TGeoRotation* proto_rotation[4];
        TGeoCombiTrans* trCave[4];
	//	TGeoTranslation* trCave[4];
	//	TGeoTranslation *trCave= new TGeoTranslation(-48., 0., 280.);   // miniCBM position

        for (i=0;i<4;i++)
	{
	  //          trCave[i]= new TGeoTranslation(xPos[i], yPos[i], zPos[i]);
          proto_rotation[i] = new TGeoRotation();

          // rotate around x axis
          proto_angle[i] = atan( yPos[i] / sqrt( xPos[i]*xPos[i] + zPos[i]*zPos[i] ) ) * 180 / acos(-1);
	  proto_rotation[i]->RotateX(-proto_angle[i]);
  
          cout << "angle x" << i << " " << proto_angle[i] << " deg" << endl;
          cout << "y " << yPos[i] << " t " << sqrt( xPos[i]*xPos[i] + zPos[i]*zPos[i] ) << " cm" << endl;

          // rotate around y axis
          proto_angle[i] = atan( xPos[i] / zPos[i] ) * 180 / acos(-1);
	  proto_rotation[i]->RotateY(proto_angle[i]);

          cout << "angle y" << i << " " << proto_angle[i] << " deg" << endl;
          cout << "x " << xPos[i] << " y " << yPos[i] << " cm" << endl;

          cout << endl;
          trCave[i] = new TGeoCombiTrans(xPos[i], yPos[i], zPos[i], proto_rotation[i]);
        }

	TGeoTranslation *trBox= new TGeoTranslation(0., 0., 0.); //Gasbox/Box Translation
	TGeoTranslation *trSensPlane= new TGeoTranslation(0., 0., boxLength / 2. + sensPlaneBoxDistance);

	Double_t pmtPlaneZ = 0.;
	if (richGeomType == kGlassLense) {
		pmtPlaneZ = -(lenseRadius - lenseCThickness - lensePmtDistance);
	} else if(richGeomType == kQuarzPlate){
		pmtPlaneZ = QuarzPmtDistance + QuarzThickness / 2. + pmtThickness / 2.;
	} else{
		pmtPlaneZ = aerogelPmtDistance + aerogelLength / 2. + pmtThickness / 2.;
	}


	TGeoTranslation *trAerogel = new TGeoTranslation(0., 0., 0.);

	Double_t pmtPlaneY = pmtSize + pmtMatrixGap/2. + pmtGap/2.;
	TGeoTranslation *trPmtPlaneUp = new TGeoTranslation(0., pmtPlaneY, pmtPlaneZ);
	TGeoTranslation *trPmtPlaneDown = new TGeoTranslation(0., -pmtPlaneY, pmtPlaneZ);

	TGeoTranslation *trPmt1 = new TGeoTranslation(-pmtSize - pmtGap,  pmtSizeHalf + pmtGapHalf, 0.);
	TGeoTranslation *trPmt2 = new TGeoTranslation(0., pmtSizeHalf + pmtGapHalf, 0.);
	TGeoTranslation *trPmt3 = new TGeoTranslation( pmtSize + pmtGap, pmtSizeHalf + pmtGapHalf, 0.);
	TGeoTranslation *trPmt4 = new TGeoTranslation(-pmtSize - pmtGap, -pmtSizeHalf - pmtGapHalf, 0.);
	TGeoTranslation *trPmt5 = new TGeoTranslation(0., -pmtSizeHalf - pmtGapHalf, 0.);
	TGeoTranslation *trPmt6 = new TGeoTranslation( pmtSize + pmtGap, -pmtSizeHalf - pmtGapHalf, 0.);

	TGeoTranslation trQuarzPlate(0., 0., 0. );
	TGeoRotation rotQuarzPlate;
	rotQuarzPlate.SetAngles(0., 0., 0.);
	TGeoCombiTrans* combiTrQuarzPlate = new TGeoCombiTrans(trQuarzPlate, rotQuarzPlate);

	TGeoTranslation *trAbsorber= new TGeoTranslation(0., 0., -(lenseRadius - lenseCThickness) + absorberThickness/2);

	Double_t rotAngleLense = 0.;
	TGeoTranslation trLense(0., - lenseRadius * TMath::ASin(TMath::DegToRad() * rotAngleLense), 0.);
	TGeoRotation rotLense;
	rotLense.SetAngles(0., rotAngleLense, 0.);
	TGeoCombiTrans* combiTrLense = new TGeoCombiTrans(trLense, rotLense);

	TGeoTranslation *trComp= new TGeoTranslation("trComp", 0., 0., -(lenseRadius - lenseCThickness) / 2 + 0.1);
	trComp->RegisterYourself();

	// Create the top volume
	TGeoBBox* topbox= new TGeoBBox("", 1000., 1000., 2000.);
	TGeoVolume* top = new TGeoVolume("top", topbox, medAir);
	gGeoMan->SetTopVolume(top);

	// Rich assembly
	TGeoVolume* rich = new TGeoVolumeAssembly("rich_v18d_mcbm");
	top->AddNode(rich, 1);

	//	TGeoVolume *caveVol = gGeoMan->MakeBox("rich_smallprototype_v17a", medAl, (boxWidth + boxThickness)/2. , (boxHeight + boxThickness)/2., (boxLength + boxThickness)/2.);
	TGeoVolume *caveVol = gGeoMan->MakeBox("rich_smallprototype", medAl, (boxWidth + boxThickness)/2. , (boxHeight + boxThickness)/2., (boxLength + boxThickness)/2.);
	TGeoVolume *boxVol = gGeoMan->MakeBox("Box", medNitrogen, boxWidth/2., boxHeight/2., boxLength/2.);
	TGeoVolume *gasVol = gGeoMan->MakeBox("Gas", medNitrogen , boxWidth/2 - wallWidth, boxHeight/2 - wallWidth, boxLength/2 - wallWidth);
	TGeoVolume *pmtContVol = gGeoMan->MakeBox("PmtContainer", medCsI , 3 * pmtSizeHalf + pmtGap, 2 * pmtSizeHalf + pmtGap / 2., pmtThickness / 2.);
	TGeoVolume *pmtVol = gGeoMan->MakeBox("Pmt", medCsI , pmtSizeHalf, pmtSizeHalf, pmtThickness / 2.);
	TGeoVolume *pmtPixelVol = gGeoMan->MakeBox("pmt_pixel", medCsI, pmtPixelSize/2., pmtPixelSize/2., pmtThickness / 2.);

	// Lense composite shape
	TGeoSphere *lenseCoatingVol = new TGeoSphere("LenseCoating", lenseRadius, lenseRadius + lenseCoating, 90., 180., 0., 360.);
	TGeoSphere *lenseVol = new TGeoSphere("Lense", 0., lenseRadius, 90., 180., 0., 360.);
	TGeoBBox *lenseCut = new TGeoBBox("LenseCutShape", lenseRadius + lenseCoating, lenseRadius + lenseCoating, (lenseRadius - lenseCThickness)/2);
	TGeoCompositeShape *lenseCompShape = new TGeoCompositeShape("LenseCompShape", "Lense-LenseCutShape:trComp");
	TGeoCompositeShape *lenseCoatingCompShape = new TGeoCompositeShape("LenseCoatingCompShape", "LenseCoating-LenseCutShape:trComp");
	TGeoVolume *lenseCompVol = new TGeoVolume("LenseComp", lenseCompShape, medLenseGlass);
	TGeoVolume *lenseCoatingCompVol = new TGeoVolume("LenseCoatingComp", lenseCoatingCompShape, medCoating);

	TGeoVolume *absorberVol = gGeoMan->MakeTube("Absorber", medAl, 0., absorberRadius, absorberThickness/2);
	TGeoVolume *sensPlaneVol = gGeoMan->MakeBox("SensPlane", medCsI, sensPlaneSize/2 , sensPlaneSize/2  , 0.1);

	// Quarz plate
//	TGeoVolume *QuarzVol = gGeoMan->MakeBox("QuarzPlate", medQuarz , QuarzWidth / 2., QuarzHeight / 2., QuarzThickness / 2.);

	//Aerogel box
        TGeoVolume *aerogelVol = gGeoMan->MakeBox("AerogelBox", medAerogel, aerogelWidth/2., aerogelHeight/2., aerogelLength/2.);

	//Positioning
	rich->AddNode(caveVol, 1, trCave[0]);
	rich->AddNode(caveVol, 2, trCave[1]);		
	rich->AddNode(caveVol, 3, trCave[2]);
	rich->AddNode(caveVol, 4, trCave[3]);

	caveVol->AddNode(boxVol, 1, trBox);
	if (isIncludeSensPlane) {
		caveVol->AddNode(sensPlaneVol, 1, trSensPlane);
	}

	boxVol->AddNode(gasVol, 1, rotBox);

	if (richGeomType == kGlassLense) {
		gasVol->AddNode(lenseCoatingCompVol, 1, combiTrLense);
		gasVol->AddNode(lenseCompVol, 1, combiTrLense);
	} else if (richGeomType == kQuarzPlate){
//		gasVol->AddNode(QuarzVol, 1, combiTrQuarzPlate);
	} else{
		gasVol->AddNode(aerogelVol, 1, trAerogel);
	}

	gasVol->AddNode(pmtContVol, 1, trPmtPlaneUp);
	gasVol->AddNode(pmtContVol, 2, trPmtPlaneDown);

	if (absorberRadius > 0.){
		lenseCompVol->AddNode(absorberVol, 1, trAbsorber);
	}

	pmtContVol->AddNode(pmtVol, 1, trPmt1);
	pmtContVol->AddNode(pmtVol, 2, trPmt2);
	pmtContVol->AddNode(pmtVol, 3, trPmt3);
	pmtContVol->AddNode(pmtVol, 4, trPmt4);
	pmtContVol->AddNode(pmtVol, 5, trPmt5);
	pmtContVol->AddNode(pmtVol, 6, trPmt6);

	Int_t halfPmtNofPixels = pmtNofPixels / 2;
	for(Int_t i = 0; i < pmtNofPixels; i++) {
		for(Int_t j = 0; j < pmtNofPixels; j++) {
			Double_t x = (i - (halfPmtNofPixels - 1)) * pmtPixelSize - pmtPixelSizeHalf;
			Double_t y = (j - (halfPmtNofPixels - 1)) * pmtPixelSize - pmtPixelSizeHalf;
			cout << "x:" << x << "  ";
			TGeoTranslation *trij = new TGeoTranslation(x, y, 0.);
			pmtVol->AddNode(pmtPixelVol, pmtNofPixels * i + j + 1, trij);
		}	
		cout << endl;
	}	

	gGeoMan->CheckOverlaps();
	gGeoMan->PrintOverlaps();

	//Draw
	lenseCoatingCompVol->SetLineColor(kCyan);
	lenseCompVol->SetLineColor(kGreen);
	boxVol->SetLineColor(kBlack);
	gasVol->SetLineColor(kBlue);
	pmtVol->SetLineColor(kOrange);
	pmtPixelVol->SetLineColor(kYellow+4);
	aerogelVol->SetLineColor(kCyan);
	aerogelVol->SetTransparency(70);
	//gGeoMan->SetTopVisible();
	//boxVol->SetVisibility(false);

	//	boxVol->Draw("ogl");
	top->Draw("ogl");
        gGeoMan->SetVisLevel(6);
	  
//	rich->Export(geoFileName);   // an alternative way of writing the trd volume
	TFile* geoFile = new TFile(geoFileName, "RECREATE");
 	top->Write();
	cout << endl << "Geometry written to " << geoFileName << endl;
	geoFile->Close();


}
