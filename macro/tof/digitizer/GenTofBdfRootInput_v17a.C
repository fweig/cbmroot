/*
 * GenTofBdfRootInput_v17a.C
 * p.-a Loizeau, GSI, 09/05/2017
 * This macro can be used to generated the root input file needed for
 * simulations with the CbmTofDigitizerBDF class.
 */

Bool_t GenTofBdfRootInput_v17a()
{
/*********************++++++ Configuration ++++++**********************/
   /// Tag of the output file, e.g. "v17a"
   /// => This will be used to the output file name, e.g. "bdf_input_v17a.root"
   /// => The full path to this file should be set in the "BeamInputFile"
   ///    field in the "XXXXX.digibdf.par" file
   TString      sTag        = "v17a";
   /// These types can be used for any simulation SM type
   /// e.g. Beamtime analysis SM type 0 can be used for simulation SM type 3
   /// => The matching should be set in the "SmTypeInpMapp" field in the
   ///    "XXXXX.digibdf.par" file
   const UInt_t kuNbSmType  = 6;
   Double_t     dEffPercent[kuNbSmType] = 
                              { 0.99999, 0.99999, 0.99999,
                                0.99999, 0.99999, 0.99999 };
   Double_t     dTimeResNs[kuNbSmType] = 
                              { 0.057, 0.057, 0.057,
                                0.057, 0.057, 0.057 };
   /// Path to the file holding the histograms from the beamtime analysis
   /// => 1 filename per type, same file can be used in each of them
   TString      sHistosSourceFile[kuNbSmType] =
                              { "../../../parameters/tof/test_bdf_input.root",
                                "../../../parameters/tof/test_bdf_input.root",
                                "../../../parameters/tof/test_bdf_input.root",
                                "../../../parameters/tof/test_bdf_input.root",
                                "../../../parameters/tof/test_bdf_input.root",
                                "../../../parameters/tof/test_bdf_input.root" };
   /// Name of the histogram with the cluster size distribution for each type
   /// => (sub)folder(s) not needed, dimensions should be: (size [strips], # clusters[])
   ///    The size of the X axis should not matter as it is just used to reconstruct a
   ///    probability map
   TString      sSizeHistSourceName[kuNbSmType] =
                              { "h1ClusterSizeType000",
                                "h1ClusterSizeType000",
                                "h1ClusterSizeType000",
                                "h1ClusterSizeType000",
                                "h1ClusterSizeType000",
                                "h1ClusterSizeType000" };
   /// Name of the histogram with the cluster TOT distribution for each type
   /// => (sub)folder(s) not needed, dimensions should be: (cluster TOT [ps], # clusters[])
   ///    The size of the X axis should not matter as it is just used to reconstruct a
   ///    probability map
   /// => If ns are used instead of ps, the CbmTofDigitizerBDF method called "LoadBeamtimeValues"
   ///    will need to be modified around l.661 ===> BACKWARD COMPATIBILITY BREAKING!
   TString      sTotHistSourceName[kuNbSmType] =
                              { "h1ClusterTot000",
                                "h1ClusterTot000",
                                "h1ClusterTot000",
                                "h1ClusterTot000",
                                "h1ClusterTot000",
                                "h1ClusterTot000" };
/*********************+++++++++++++++++++++++++++**********************/

   // Create and fill the two TArrayD
   TArrayD	adEfficiency( kuNbSmType );
   TArrayD	adTimeResol(  kuNbSmType );

   // Create the pointers arrays for the Histograms clones
   TH1D *   apClusterSize[kuNbSmType];
   TH1D *   apClusterTot[ kuNbSmType];

   // Loop on SM type, fill the 2 TArrayD and clone the 2 histos with
   // proper names
   TH1D  * pTemp;
   TFile * pSourceFile;
   for( UInt_t uSmType = 0; uSmType < kuNbSmType; uSmType++ )
   {
      // TArrayD
      adEfficiency[ uSmType ] = dEffPercent[ uSmType ];
   	adTimeResol[  uSmType ] = dTimeResNs[  uSmType ];

      // Histos
         // Open source file   
      pSourceFile = new TFile( sHistosSourceFile[uSmType], "READ" );
      if( NULL == pSourceFile )
      {
         std::cerr << "ERROR: Failed to open the histograms source file for type "
                   << uSmType << ", stopping there. filename = "
                   << sHistosSourceFile[uSmType]
                   << std::endl;
         return kFALSE;
      } // if( NULL == pSourceFile )
      gROOT->cd();
         // Recover ClusterSize histo and clone it
      pTemp = NULL;
      pTemp = (TH1D*) (pSourceFile->FindObjectAny( sSizeHistSourceName[uSmType] ) );
      if( NULL != pTemp )
      {
         apClusterSize[uSmType] = (TH1D*)(pTemp->Clone( Form( "h1ClusterSizeType%03u", uSmType ) ) );
         apClusterSize[uSmType]->SetTitle(
                     Form("Cluster Size distribution for RPC type %03u in Strips", uSmType) );
      } // if( NULL != pTemp )
         else
         {
            std::cerr << "ERROR: Failed to recover the cluster size histogram for type "
                      << uSmType << ", stopping there. histo name = "
                      << sSizeHistSourceName[uSmType]
                      << std::endl;
            pSourceFile->Close();
            return kFALSE;
         } // else of if( NULL != pTemp )
         // Recover ClusterTot histo and clone it
      pTemp = NULL;
      pTemp = (TH1D*) (pSourceFile->FindObjectAny( sTotHistSourceName[uSmType] ) );
      if( NULL != pTemp )
      {
         apClusterTot[uSmType] = (TH1D*)(pTemp->Clone( Form( "h1ClusterTot%03u", uSmType ) ) );
         apClusterTot[uSmType]->SetTitle(
                     Form("Cluster Total Charge distribution for RPC type %03u as Tot in ps", uSmType) );
      } // if( NULL != pTemp )
         else
         {
            std::cerr << "ERROR: Failed to recover the cluster tot histogram for type "
                      << uSmType << ", stopping there. histo name = "
                      << sTotHistSourceName[uSmType]
                      << std::endl;
            pSourceFile->Close();
            return kFALSE;
         } // else of if( NULL != pTemp )
      
      // Close Source file
      pSourceFile->Close();

      /// Change Title of histo to reflect type index
   } // for( UInt_t uSmType = 0; uSmType < kuNbSmType; uSmType++ )

   // Open the output file
   TFile * pOutFile = new TFile( "./bdf_input_" + sTag + ".root", "RECREATE");

   // Save the two ArrayD
   pOutFile->WriteObject(&adEfficiency, "Efficiency");
   pOutFile->WriteObject(&adTimeResol,  "TimeResol");

   // Save the histograms
   for( UInt_t uSmType = 0; uSmType < kuNbSmType; uSmType++ )
   {
      apClusterSize[uSmType]->Write();
      apClusterTot[ uSmType]->Write();
   } // for( UInt_t uSmType = 0; uSmType < kuNbSmType; uSmType++ )

   // Close output file
   pSourceFile->Close();
   
   return kTRUE;
}
