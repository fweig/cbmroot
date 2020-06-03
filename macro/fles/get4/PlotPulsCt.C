{
   TCanvas *cCtChipsA = new TCanvas("cCtChipsA", "CT distributions for chips 0-3 in FEE A");
   cCtChipsA->Divide(2, 2);

   cCtChipsA->cd( 1);
   fhPulserFeeDistCT_chip000->Draw("colz");

   cCtChipsA->cd( 2);
   fhPulserFeeDistCT_chip001->Draw("colz");

   cCtChipsA->cd( 3);
   fhPulserFeeDistCT_chip002->Draw("colz");

   cCtChipsA->cd( 4);
   fhPulserFeeDistCT_chip003->Draw("colz");

   TCanvas *cCtChipsB = new TCanvas("cCtChipsB", "CT distributions for chips 4-7 in FEE A");
   cCtChipsB->Divide(2, 2);

   cCtChipsB->cd( 1);
   fhPulserFeeDistCT_chip004->Draw("colz");

   cCtChipsB->cd( 2);
   fhPulserFeeDistCT_chip005->Draw("colz");

   cCtChipsB->cd( 3);
   fhPulserFeeDistCT_chip006->Draw("colz");

   cCtChipsB->cd( 4);
   fhPulserFeeDistCT_chip007->Draw("colz");

   TCanvas *cCtTotChipsA = new TCanvas("cCtTotChipsA", "CT distributions for Tot for chips 0-3 in FEE A");
   cCtTotChipsA->Divide(2, 2);

   cCtTotChipsA->cd( 1);
   fhPulserFeeTotDistCT_chip000->Draw("colz");

   cCtTotChipsA->cd( 2);
   fhPulserFeeTotDistCT_chip001->Draw("colz");

   cCtTotChipsA->cd( 3);
   fhPulserFeeTotDistCT_chip002->Draw("colz");

   cCtTotChipsA->cd( 4);
   fhPulserFeeTotDistCT_chip003->Draw("colz");

   TCanvas *cCtTotChipsB = new TCanvas("cCtTotChipsB", "CT distributions for Tot for chips 4-7 in FEE A");
   cCtTotChipsB->Divide(2, 2);

   cCtTotChipsB->cd( 1);
   fhPulserFeeTotDistCT_chip004->Draw("colz");

   cCtTotChipsB->cd( 2);
   fhPulserFeeTotDistCT_chip005->Draw("colz");

   cCtTotChipsB->cd( 3);
   fhPulserFeeTotDistCT_chip006->Draw("colz");

   cCtTotChipsB->cd( 4);
   fhPulserFeeTotDistCT_chip007->Draw("colz");
}
