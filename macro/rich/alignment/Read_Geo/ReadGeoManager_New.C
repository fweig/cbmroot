#include <unistd.h>

void ReadGeoManager_New()
{
// ---------------------------------------------------------------------------------------------------------------------

	TString vmcDir = TString(gSystem->Getenv("VMCWORKDIR"));

	TGeoManager *gdml = new TGeoManager("gdml", "FAIRGeom");

	TGDMLParse parser;
	// Define your input GDML file HERE
//	TGeoVolume* gdmlTop = parser.GDMLReadFile ("../../../../../geometry/rich/alignment/misalignment_correction/v15_rot/Rich_dec_2015_misalign.gdml");
	TGeoVolume* gdmlTop = parser.GDMLReadFile ("../../../../../geometry/rich/Rich_jan2016_misalign.gdml");
	TGeoVolume* rootTop = new TGeoVolumeAssembly("TOP");

	gGeoManager->SetTopVolume(rootTop);

	// Define your position HERE
	TGeoRotation* rot = new TGeoRotation ("rot", 0., 0., 0.);
	TGeoCombiTrans* posrot = new TGeoCombiTrans (0., 0., 270., rot);

	rootTop->AddNode (gdmlTop, 1, posrot);

	gGeoManager->CloseGeometry();

// ---------------------------------------------------------------------------------------------------------------------

	TGeoNavigator* navi = gGeoManager->GetCurrentNavigator();

	Double_t coords[3];
	coords[0] = -64.310094;
	coords[1] = 61.256446;
	coords[2] = 338.476947;

	TGeoNode* foundNode = gGeoManager->FindNode(coords[0], coords[1], coords[2]);

	if (foundNode) {
		std::cout << foundNode->GetName() << std::endl;
		std::cout << navi->GetPath() << std::endl;

		navi->GetCurrentMatrix()->Print();
		navi->GetMotherMatrix()->Print();

	} else {
		std::cout << "No node found." << std::endl;
	}
	//return;

// ---------------------------------------------------------------------------------------------------------------------

   // Just to use shorter names
   Double_t PI = TMath::Pi();
   Double_t RADTODEG = TMath::RadToDeg();
   Double_t DEGTORAD = TMath::DegToRad();

   TGeoIterator nextNode(rootTop);
   TGeoNode* curNode;
   TGeoMatrix* curMatrix;

   const Double_t* curNodeTranslation; // 3 components - pointers to some memory which is provided by ROOT
   const Double_t* curNodeRotationM; // 9 components - pointers to some memory which is provided by ROOT

   TString filterName0("mirror_tile_type0");
   TString filterName1("mirror_tile_type1");
   TString filterName2("mirror_tile_type2");
   TString filterName3("mirror_tile_type3");
   TString filterName4("mirror_tile_type4");
   TString filterName5("mirror_tile_type5");

   while ((curNode=nextNode())) {
      TString nodeName(curNode->GetName());
      TString nodePath;

      // Filter using volume name, not node name
      // But you can do 'if (nodeName.Contains("filter"))'
      if (curNode->GetVolume()->GetName() == filterName0 ||
          curNode->GetVolume()->GetName() == filterName1 ||
          curNode->GetVolume()->GetName() == filterName2 ||
          curNode->GetVolume()->GetName() == filterName3 ||
          curNode->GetVolume()->GetName() == filterName4 ||
          curNode->GetVolume()->GetName() == filterName5 ) {

         if (curNode->GetNdaughters() == 0) {

            // All deepest nodes of mirror tiles here (leaves)
            // Thus we get spherical surface centers

            nextNode.GetPath(nodePath);
            curMatrix = nextNode.GetCurrentMatrix();
            curNodeTranslation = curMatrix->GetTranslation();
            curNodeRotationM = curMatrix->GetRotationMatrix();

            //TODO Retrieve rotation angles

            printf ("%s tr:\t", nodePath.Data());
            printf ("%08f\t%08f\t%08f\t\n", curNodeTranslation[0], curNodeTranslation[1], curNodeTranslation[2]);
/*	    const Char_t *topNodePath = gGeoManager->GetTopNode()->GetName();
	    printf ("top node path: %s\n", topNodePath);
	    TGeoVolume *topVolume;
	    topVolume = gGeoManager->GetTopVolume();
	    topVolume->Print();
	    rootTop->Print();		//compare rootTop with topVolume
*/
         }
      }
   }

// ---------------------------------------------------------------------------------------------------------------------

   printf("\n");

   TString filterName_inter0("mirror_tile_type0_inter");
   TString filterName_inter1("mirror_tile_type1_inter");
   TString filterName_inter2("mirror_tile_type2_inter");
   TString filterName_inter3("mirror_tile_type3_inter");
   TString filterName_inter4("mirror_tile_type4_inter");
   TString filterName_inter5("mirror_tile_type5_inter");

   // Don't forget to reset the iterator before looping over the geometry each next time
   nextNode.Reset();

   while ((curNode=nextNode())) {
      TString nodeName(curNode->GetName());
      TString nodePath;

      // Filter using volume name, not node name
      // But you can do 'if (nodeName.Contains("filter"))'
      if (curNode->GetVolume()->GetName() == filterName_inter0 ||
          curNode->GetVolume()->GetName() == filterName_inter1 ||
          curNode->GetVolume()->GetName() == filterName_inter2 ||
          curNode->GetVolume()->GetName() == filterName_inter3 ||
          curNode->GetVolume()->GetName() == filterName_inter4 ||
          curNode->GetVolume()->GetName() == filterName_inter5 ) {

         if (curNode->GetNdaughters() > 0) {

            // Intermediate nodes here (with "inter" in the name)
            // Thus we get centers of the tiles

            nextNode.GetPath(nodePath);
            curMatrix = nextNode.GetCurrentMatrix();
            curNodeTranslation = curMatrix->GetTranslation();
            curNodeRotationM = curMatrix->GetRotationMatrix();

            //TODO Retrieve rotation angles

            printf ("%s tr:\t", nodePath.Data());
            printf ("%08f\t%08f\t%08f\t\n", curNodeTranslation[0], curNodeTranslation[1], curNodeTranslation[2]);

         }
      }
   }

// ---------------------------------------------------------------------------------------------------------------------
/*
   printf("\n");

   TH2S* histo = new TH2S("XYplane", "XYplane", 675, -270., 270., 675, -270., 270.);

   TString filterName_pixel("pmt_pixel");

   // Don't forget to reset the iterator before looping over the geometry each next time
   nextNode.Reset();

   while ((curNode=nextNode())) {
      TString nodeName(curNode->GetName());
      TString nodePath;

      // Filter using volume name, not node name
      // But you can do 'if (nodeName.Contains("filter"))'
      if (curNode->GetVolume()->GetName() == filterName_pixel) {

         nextNode.GetPath(nodePath);
         curMatrix = nextNode.GetCurrentMatrix();
         curNodeTranslation = curMatrix->GetTranslation();
         curNodeRotationM = curMatrix->GetRotationMatrix();

         //TODO Retrieve rotation angles

         //printf ("%s tr:\t", nodePath.Data());
         //printf ("%08f\t%08f\t%08f\t\n", curNodeTranslation[0], curNodeTranslation[1], curNodeTranslation[2]);

         histo->Fill(curNodeTranslation[0], curNodeTranslation[1]);

      }
   }

   histo->Draw();
*/
// ---------------------------------------------------------------------------------------------------------------------

}

/*
        Double_t beta = TMath::ASin(-curNodeRotationM[2])*RADTODEG;          // around Y
        Double_t alpha1 = TMath::ASin(curNodeRotationM[1]/TMath::Cos(TMath::ASin(-curNodeRotationM[2])))*RADTODEG; // around Z
        Double_t gamma1 = TMath::ASin(curNodeRotationM[5]/TMath::Cos(TMath::ASin(-curNodeRotationM[2])))*RADTODEG; // around X
        Double_t gamma2 = TMath::ACos(curNodeRotationM[8]/TMath::Cos(TMath::ASin(-curNodeRotationM[2])))*RADTODEG; // around X
        printf ("%08f\t\t%08f\t\t%08f\t%08f\n\n", alpha1, beta, gamma1, gamma2);
*/
