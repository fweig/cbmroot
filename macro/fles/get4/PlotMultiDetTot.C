{

   TCanvas *cHDP2 = new TCanvas("cHDP2", "HDP2");
   cHDP2->Divide(4, 4);

   cHDP2->cd( 1);
   hGet4MultipleHitsVsTot_000->Draw("colz");
   cHDP2->cd( 2);
   hGet4MultipleHitsVsTot_001->Draw("colz");
   cHDP2->cd( 3);
   hGet4MultipleHitsVsTot_002->Draw("colz");
   cHDP2->cd( 4);
   hGet4MultipleHitsVsTot_003->Draw("colz");
   cHDP2->cd( 5);
   hGet4MultipleHitsVsTot_004->Draw("colz");
   cHDP2->cd( 6);
   hGet4MultipleHitsVsTot_005->Draw("colz");
   cHDP2->cd( 7);
   hGet4MultipleHitsVsTot_006->Draw("colz");
   cHDP2->cd( 8);
   hGet4MultipleHitsVsTot_007->Draw("colz");
   cHDP2->cd( 9);
   hGet4MultipleHitsVsTot_008->Draw("colz");
   cHDP2->cd(10);
   hGet4MultipleHitsVsTot_009->Draw("colz");
   cHDP2->cd(11);
   hGet4MultipleHitsVsTot_010->Draw("colz");
   cHDP2->cd(12);
   hGet4MultipleHitsVsTot_011->Draw("colz");
   cHDP2->cd(13);
   hGet4MultipleHitsVsTot_012->Draw("colz");
   cHDP2->cd(14);
   hGet4MultipleHitsVsTot_013->Draw("colz");
   cHDP2->cd(15);
   hGet4MultipleHitsVsTot_014->Draw("colz");
   cHDP2->cd(16);
   hGet4MultipleHitsVsTot_015->Draw("colz");

   TCanvas *cTSUStrip = new TCanvas("cTSUStrip", "TSU Strip");
   cTSUStrip->Divide(4, 4);

   cTSUStrip->cd( 1);
   hGet4MultipleHitsVsTot_016->Draw("colz");
   cTSUStrip->cd( 2);
   hGet4MultipleHitsVsTot_017->Draw("colz");
   cTSUStrip->cd( 3);
   hGet4MultipleHitsVsTot_018->Draw("colz");
   cTSUStrip->cd( 4);
   hGet4MultipleHitsVsTot_019->Draw("colz");
   cTSUStrip->cd( 5);
   hGet4MultipleHitsVsTot_020->Draw("colz");
   cTSUStrip->cd( 6);
   hGet4MultipleHitsVsTot_021->Draw("colz");
   cTSUStrip->cd( 7);
   hGet4MultipleHitsVsTot_022->Draw("colz");
   cTSUStrip->cd( 8);
   hGet4MultipleHitsVsTot_023->Draw("colz");
   cTSUStrip->cd( 9);
   hGet4MultipleHitsVsTot_064->Draw("colz");
   cTSUStrip->cd(10);
   hGet4MultipleHitsVsTot_065->Draw("colz");
   cTSUStrip->cd(11);
   hGet4MultipleHitsVsTot_066->Draw("colz");
   cTSUStrip->cd(12);
   hGet4MultipleHitsVsTot_067->Draw("colz");
   cTSUStrip->cd(13);
   hGet4MultipleHitsVsTot_068->Draw("colz");
   cTSUStrip->cd(14);
   hGet4MultipleHitsVsTot_069->Draw("colz");
   cTSUStrip->cd(15);
   hGet4MultipleHitsVsTot_070->Draw("colz");
   cTSUStrip->cd(16);
   hGet4MultipleHitsVsTot_071->Draw("colz");

   TCanvas *cHDP5 = new TCanvas("cHDP5", "HDP5");
   cHDP5->Divide(4, 2);

   cHDP5->cd( 1);
   hGet4MultipleHitsVsTot_072->Draw("colz");
   cHDP5->cd( 2);
   hGet4MultipleHitsVsTot_073->Draw("colz");
   cHDP5->cd( 3);
   hGet4MultipleHitsVsTot_074->Draw("colz");
   cHDP5->cd( 4);
   hGet4MultipleHitsVsTot_075->Draw("colz");
   cHDP5->cd( 5);
   hGet4MultipleHitsVsTot_076->Draw("colz");
   cHDP5->cd( 6);
   hGet4MultipleHitsVsTot_077->Draw("colz");
   cHDP5->cd( 7);
   hGet4MultipleHitsVsTot_078->Draw("colz");
   cHDP5->cd( 8);
   hGet4MultipleHitsVsTot_079->Draw("colz");

   /*
   TCanvas *cUSTC = new TCanvas("cUSTC", "USTC");
   cUSTC->Divide(4, 3);

   cUSTC->cd( 1);
   hGet4MultipleHitsVsTot_080->Draw("colz");
   cUSTC->cd( 2);
   hGet4MultipleHitsVsTot_081->Draw("colz");
   cUSTC->cd( 3);
   hGet4MultipleHitsVsTot_082->Draw("colz");
   cUSTC->cd( 4);
   hGet4MultipleHitsVsTot_083->Draw("colz");
   cUSTC->cd( 5);
   hGet4MultipleHitsVsTot_084->Draw("colz");
   cUSTC->cd( 6);
   hGet4MultipleHitsVsTot_085->Draw("colz");
   cUSTC->cd( 7);
   hGet4MultipleHitsVsTot_086->Draw("colz");
   cUSTC->cd( 8);
   hGet4MultipleHitsVsTot_087->Draw("colz");
*/

   Int_t iNbChanChip = 10;

   TCanvas *cHDP2S = new TCanvas("cHDP2S", "HDP2 summary");
   cHDP2S->Divide(2);

   TH2* hDoubleHitsRatioHDP2 = new TH2D("hDoubleHitsRatioHDP2",
         "Ratio of double hits per channel for HDP2; chip; ToT [ns]; Ratio [\%]",
         16, -0.5, 15.5,
         10,  0.0, 10.0 );
   TH2* hTripleHitsRatioHDP2 = new TH2D("hTripleHitsRatioHDP2",
         "Ratio of triple hits per channel for HDP2;  chip; ToT [ns]; Ratio [\%]",
         16, -0.5, 15.5,
         10,  0.0, 10.0 );

   TH2* hDoubleHitsRatioTSUS = new TH2D("hDoubleHitsRatioTSUS",
         "Ratio of double hits per channel for TSUS;  chip; ToT [ns]; Ratio [\%]",
         16, -0.5, 15.5,
         10,  0.0, 10.0 );
   TH2* hTripleHitsRatioTSUS = new TH2D("hTripleHitsRatioTSUS",
         "Ratio of triple hits per channel for TSUS;  chip; ToT [ns]; Ratio [\%]",
         16, -0.5, 15.5,
         10,  0.0, 10.0 );

   TH2* hDoubleHitsRatioHDP5 = new TH2D("hDoubleHitsRatioHDP5",
         "Ratio of double hits per channel for HDP5;  chip; ToT [ns];; Ratio [\%]",
         8, -0.5,  7.5,
         10,  0.0, 10.0 );
   TH2* hTripleHitsRatioHDP5 = new TH2D("hTripleHitsRatioHDP5",
         "Ratio of triple hits per channel for HDP5;  chip; ToT [ns]; Ratio [\%]",
         8, -0.5,  7.5,
         10,  0.0, 10.0 );

   for( Int_t iChan = 0; iChan < iNbChanChip; iChan ++)
   {
      // Double hits
      if( 0 < hGet4MultipleHitsVsTot_000->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 0, 0.5 + iChan,
            100.0 *
             hGet4MultipleHitsVsTot_000->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_000->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_001->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill(  1, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_001->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_001->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_002->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 2, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_002->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_002->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_003->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 3, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_003->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_003->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_004->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 4, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_004->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_004->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_005->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 5, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_005->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_005->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_006->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 6, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_006->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_006->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_007->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 7, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_007->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_007->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_008->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 8, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_008->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_008->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_009->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 9, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_009->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_009->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_010->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 10, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_010->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_010->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_011->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 11, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_011->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_011->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_012->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 12, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_012->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_012->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_013->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 13, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_013->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_013->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_014->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 14, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_014->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_014->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_015->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP2->Fill( 15, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_015->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_015->GetBinContent( 1 + iChan, 1) );

      // Triple hits
      if( 0 < hGet4MultipleHitsVsTot_000->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill(  0, 0.5 + iChan,
            100.0 *
             hGet4MultipleHitsVsTot_000->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_000->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_001->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill(  1, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_001->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_001->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_002->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 2, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_002->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_002->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_003->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 3, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_003->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_003->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_004->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 4, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_004->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_004->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_005->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 5, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_005->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_005->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_006->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 6, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_006->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_006->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_007->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 7, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_007->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_007->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_008->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 8, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_008->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_008->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_009->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 9, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_009->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_009->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_010->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 10, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_010->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_010->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_011->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 11, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_011->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_011->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_012->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 12, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_012->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_012->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_013->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 13, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_013->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_013->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_014->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 14, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_014->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_014->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_015->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP2->Fill( 15, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_015->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_015->GetBinContent( 1 + iChan, 1) );
   } // for( Int_t iChan = 0; iChan < 4; iChan ++)

   cHDP2S->cd(1);
   hDoubleHitsRatioHDP2->SetStats(kFALSE);
   hDoubleHitsRatioHDP2->Draw("colz");
   cHDP2S->cd(2);
   hTripleHitsRatioHDP2->SetStats(kFALSE);
   hTripleHitsRatioHDP2->Draw("colz");

   TCanvas *cTSUStripS = new TCanvas("cTSUStripS", "TSU Strip summary");
   cTSUStripS->Divide(2);

   for( Int_t iChan = 0; iChan < iNbChanChip; iChan ++)
   {
      // Double hits
/*
      if( 0 < hGet4MultipleHitsVsTot_016->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 0, 0.5 + iChan,
            100.0 *
             hGet4MultipleHitsVsTot_016->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_016->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_017->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill(  1, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_017->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_017->GetBinContent( 1 + iChan, 1) );
*/
      if( 0 < hGet4MultipleHitsVsTot_018->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 2, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_018->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_018->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_019->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 3, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_019->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_019->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_020->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 4, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_020->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_020->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_021->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 5, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_021->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_021->GetBinContent( 1 + iChan, 1) );
/*
      if( 0 < hGet4MultipleHitsVsTot_022->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 6, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_022->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_022->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_023->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 7, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_023->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_023->GetBinContent( 1 + iChan, 1) );
*/

      if( 0 < hGet4MultipleHitsVsTot_064->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 8, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_064->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_064->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_065->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 9, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_065->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_065->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_066->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 10, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_066->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_066->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_067->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 11, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_067->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_067->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_068->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 12, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_068->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_068->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_069->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 13, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_069->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_069->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_070->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 14, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_070->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_070->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_071->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioTSUS->Fill( 15, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_071->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_071->GetBinContent( 1 + iChan, 1) );

      // Triple hits
/*
      if( 0 < hGet4MultipleHitsVsTot_016->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 0, 0.5 + iChan,
            100.0 *
             hGet4MultipleHitsVsTot_016->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_016->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_017->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill(  1, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_017->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_017->GetBinContent( 1 + iChan, 1) );
*/
      if( 0 < hGet4MultipleHitsVsTot_018->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 2, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_018->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_018->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_019->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 3, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_019->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_019->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_020->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 4, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_020->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_020->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_021->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 5, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_021->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_021->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_022->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 6, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_022->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_022->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_023->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 7, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_023->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_023->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_064->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 8, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_064->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_064->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_065->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 9, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_065->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_065->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_066->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 10, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_066->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_066->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_067->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 11, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_067->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_067->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_068->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 12, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_068->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_068->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_069->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 13, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_069->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_069->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_070->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 14, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_070->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_070->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_071->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioTSUS->Fill( 15, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_071->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_071->GetBinContent( 1 + iChan, 1) );
   } // for( Int_t iChan = 0; iChan < 4; iChan ++)

   cTSUStripS->cd(1);
   hDoubleHitsRatioTSUS->SetStats(kFALSE);
   hDoubleHitsRatioTSUS->Draw("colz");
   cTSUStripS->cd(2);
   hTripleHitsRatioTSUS->SetStats(kFALSE);
   hTripleHitsRatioTSUS->Draw("colz");

   TCanvas *cHDP5S = new TCanvas("cHDP5S", "HDP5 summary");
   cHDP5S->Divide(2);

   for( Int_t iChan = 0; iChan < iNbChanChip; iChan ++)
   {
      // Double hits
      if( 0 < hGet4MultipleHitsVsTot_072->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP5->Fill( 0, 0.5 + iChan,
            100.0 *
             hGet4MultipleHitsVsTot_072->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_072->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_073->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP5->Fill(  1, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_073->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_073->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_074->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP5->Fill(  2, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_074->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_074->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_075->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP5->Fill( 3, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_075->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_075->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_076->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP5->Fill( 4, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_076->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_076->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_077->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP5->Fill( 5, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_077->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_077->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_078->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP5->Fill( 6, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_078->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_078->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_079->GetBinContent( 1 + iChan, 1) )
         hDoubleHitsRatioHDP5->Fill( 7, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_079->GetBinContent( 1 + iChan, 2)
           / hGet4MultipleHitsVsTot_079->GetBinContent( 1 + iChan, 1) );

      // Triple hits
      if( 0 < hGet4MultipleHitsVsTot_072->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP5->Fill( 0, 0.5 + iChan,
            100.0 *
             hGet4MultipleHitsVsTot_072->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_072->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_073->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP5->Fill(  1, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_073->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_073->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_074->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP5->Fill(  2, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_074->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_074->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_075->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP5->Fill( 3, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_075->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_075->GetBinContent( 1 + iChan, 1) );

      if( 0 < hGet4MultipleHitsVsTot_076->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP5->Fill( 4, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_076->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_076->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_077->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP5->Fill( 5, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_077->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_077->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_078->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP5->Fill( 6, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_078->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_078->GetBinContent( 1 + iChan, 1) );
      if( 0 < hGet4MultipleHitsVsTot_079->GetBinContent( 1 + iChan, 1) )
         hTripleHitsRatioHDP5->Fill( 7, 0.5 + iChan,
            100.0 *
            hGet4MultipleHitsVsTot_079->GetBinContent( 1 + iChan, 3)
           / hGet4MultipleHitsVsTot_079->GetBinContent( 1 + iChan, 1) );
   } // for( Int_t iChan = 0; iChan < 4; iChan ++)

   cHDP5S->cd(1);
   hDoubleHitsRatioHDP5->SetStats(kFALSE);
   hDoubleHitsRatioHDP5->Draw("colz");
   cHDP5S->cd(2);
   hTripleHitsRatioHDP5->SetStats(kFALSE);
   hTripleHitsRatioHDP5->Draw("colz");
}
