{
   Int_t iNbGet4 = 48;
   TH2D* pGet4Histos[iNbGet4];
   TH1D* pHistoProj;

   for( Int_t iTdcIdx = 0; iTdcIdx < iNbGet4; iTdcIdx++)
      pGet4Histos[iTdcIdx] = NULL;

//   gROOT->cd();

   for( UInt_t uChipIndex = 0; uChipIndex < 24; uChipIndex++)
      pGet4Histos[uChipIndex] = (TH2D*)(gROOT->FindObject( Form("hGet4ChDataCntEvo_%03u", uChipIndex) ) );

   for( UInt_t uChipIndex = 64; uChipIndex < 88; uChipIndex++)
      pGet4Histos[uChipIndex - 40] = (TH2D*)(gROOT->FindObject( Form("hGet4ChDataCntEvo_%03u", uChipIndex) ) );

   Int_t iNbTdcFound = 0;
   for( Int_t iTdcIdx = 0; iTdcIdx < iNbGet4; iTdcIdx++)
      if( NULL != pGet4Histos[iTdcIdx] )
         iNbTdcFound++;
   cout<<"Nb TDC found: "<<iNbTdcFound<<endl;

   Int_t iNbBinsPlot =   4000;
   Double_t dStartTime =    0.0;
   Double_t dStopTime  =  400.0;
   Double_t dBinSize = (dStopTime-dStartTime)/(Double_t)(iNbBinsPlot);
   Double_t dMinY = 1;
   Double_t dMaxY = 1e7;

   TH1* hMinChannelRate  = new TH1D("hMinChannelRate", 
      "Minimal rate as function of time among all GET4 channels; Time [s]; Rate [1/s]",
      iNbBinsPlot, dStartTime, dStopTime);
   TH1* hMeanChannelRate = new TH1D("hMeanChannelRate", 
      "Mean rate as function of time among all GET4 channels; Time [s]; Rate [1/s]",
      iNbBinsPlot, dStartTime, dStopTime);
   TH1* hMaxChannelRate  = new TH1D("hMaxChannelRate", 
      "Maximum rate as function of time among all GET4 channels; Time [s]; Rate [1/s]",
      iNbBinsPlot, dStartTime, dStopTime);

   Double_t dMinRate           = 1e15; // crazy value to start
   Double_t dMeanRate          = 0.0;
   Double_t dNbNonZeroChannels = 0;
   Double_t dMaxRate           = 0.0;

   for( Int_t iBinIdx = 1; iBinIdx < iNbBinsPlot + 1; iBinIdx++)
   {
      dMinRate           = -1.0; // crazy value to start
      dMeanRate          =  0.0;
      dNbNonZeroChannels =  0;
      dMaxRate           = -1.0; // crazy value to start
      for( Int_t iTdcIdx = 0; iTdcIdx < iNbGet4; iTdcIdx++)
         if( NULL != pGet4Histos[iTdcIdx] )
         {
            pHistoProj = pGet4Histos[iTdcIdx]->ProjectionX( "_px", iBinIdx, iBinIdx);

            for( Int_t iCh = 0; iCh < 4; iCh++)
            {
               Double_t dRate = pHistoProj->GetBinContent( iCh +1);
               if( 0 < dRate )
               {
                  dMeanRate += dRate;
                  dNbNonZeroChannels++;
                  if( dRate < dMinRate || dMinRate < 0)
                     dMinRate = dRate;
                  if( dMaxRate < dRate || dMaxRate < 0 )
                     dMaxRate = dRate;
               } // if( 0 < dRate )
            } // for( Int_t iCh = 1; iCh <= 4; iCh++)
            delete pHistoProj;
         } // if( NULL != pGet4Histos[iTdcIdx] )

//cout<<dMinRate<<" "<<dMeanRate<<" "<<dNbNonZeroChannels<<" "<<dMaxRate<<endl;

      if( 0 < dMinRate )
         hMinChannelRate->Fill( dBinSize * (iBinIdx-1), dMinRate);
      if( 0 < dNbNonZeroChannels )
         hMeanChannelRate->Fill( dBinSize * (iBinIdx-1), dMeanRate/dNbNonZeroChannels);
      if( 0 < dMaxRate )
         hMaxChannelRate->Fill( dBinSize * (iBinIdx-1), dMaxRate);
   } // for( Int_t iBinIdx = 1; iBinIdx < iNbBinsPlot + 1; iBinIdx++)

   TCanvas *cChannelRateEvo = new TCanvas("cChannelRateEvo", "Channel Rate Evo");
   cChannelRateEvo->cd();

   TH1 *frameA = gPad->DrawFrame(dStartTime, dMinY, dStopTime, dMaxY);
   frameA->GetXaxis()->SetTitle("Time in run [s]");
   frameA->GetYaxis()->SetTitle("Rate [1/s]");
//   frameA->GetXaxis()->SetTimeDisplay(1);
   frameA->GetXaxis()->SetTitleSize( 0.05 );
   frameA->GetXaxis()->SetLabelSize( 0.05 );
   frameA->GetYaxis()->SetTitleSize( 0.05 );
   frameA->GetYaxis()->SetLabelSize( 0.06 );
   gPad->Update();
   gPad->SetLogy();
   gPad->SetGridx();
   gPad->SetGridy();

   hMinChannelRate->Scale(0.1,  "");
   hMeanChannelRate->Scale(0.1, "");
   hMaxChannelRate->Scale(0.1,  "");

   hMinChannelRate->SetLineColor(kBlue);
   hMinChannelRate->SetLineWidth( 2 );
   hMinChannelRate->Draw("same");
   hMeanChannelRate->SetLineColor(kGreen);
   hMeanChannelRate->SetLineWidth( 2 );
   hMeanChannelRate->Draw("same");
   hMaxChannelRate->SetLineColor(kRed);
   hMaxChannelRate->SetLineWidth( 2 );
   hMaxChannelRate->Draw("same");
}
