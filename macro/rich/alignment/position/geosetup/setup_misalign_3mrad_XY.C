/** ROOT macro to define the CBM setup sis_100_electron
 **
 ** @author Volker Friese <v.friese@gsi.de>
 ** @author David Emschermann <d.emschermann@gsi.de>
 ** @date   5 February 2016
 **
 **/

// 2016-06-27 - DE - use STS v16x as new default, see issue #647
// 2016-05-19 - VF - Add platform module
// 2016-02-25 - DE - use STS v16c as new default
// 2016-02-05 - VF - Replaces former sis100_electron_setup.C,
//                   now using the CbmSetup class.
// 2016-01-29 - AL - Use RICH v16a_1e as new default
// 2015-12-11 - DE - use STS v15c as new default
// 2015-07-20 - FU - magnet v15a is v12b with correct keeping volume material
// 2015-07-20 - FU - add material budget files
// 2015-03-13 - DE - use STS v13y with correct ladder orientation as new default
// 2015-01-28 - DE - use STS v13x fitting with with PIPE v14l
// 2015-01-22 - DE - use MVD v14b with PIPE v14l as default
// 2015-01-19 - DE - use TRD v15a as new default
// 2014-07-05 - DE - use PIPE v14x as interim solution
// 2014-07-01 - DE - use PIPE v14f -> overlap with STS
// 2014-06-30 - DE - use PIPE v14d together with RICH v14a
// 2014-06-30 - DE - make RICH v14a the new default
// 2014-06-30 - DE - make TRD v14a the new default
// 2014-06-25 - DE - define digi files through tags for STS, TRD and TOF
// 2013-11-05 - DE - switch to trd_v13p geometry
// 2013-10-11 - DE - add empty string defining the platform
// 2013-10-07 - DE - pipe_v13d.geo fixes overlap with TOF
// 2013-10-07 - DE - rich_v13a is not at nominal position 1800mm, but at 1600mm
// 2013-11-04 - VF - make STS v15b the new default
//


void setup_misalign_3mrad_XY()
{

  // -----  Geometry Tags  --------------------------------------------------
  TString magnetGeoTag    = "v15a";
  TString pipeGeoTag      = "v16c_1e";
  TString mvdGeoTag       = "v15a";
  TString stsGeoTag       = "v16x";
//  TString richGeoTag      = "v16a_1e";		//"v16a_3e"
  TString richGeoTag	  = "v16a_position_3mrad_XY";
  TString trdGeoTag       = "v15a_3e";
  TString tofGeoTag       = "v16a_3e";
  TString psdGeoTag       = "psd_geo_xy.txt";
  TString platGeoTag      = "v13a";
  // ------------------------------------------------------------------------


  // -----  Magnetic field  -------------------------------------------------
  TString fieldTag      = "v12b";
  Double_t fieldZ       = 40.;            // field centre z position
  Double_t fieldScale   =  1.;            // field scaling factor
  // ------------------------------------------------------------------------


  // -----  PSD  ------------------------------------------------------------
  TString  psdGeoFile = "psd_geo_xy.txt";
  Double_t psdZpos = 800.;
  Double_t psdXpos = 11.;
  // ------------------------------------------------------------------------


  // -----  Create setup  ---------------------------------------------------
  CbmSetup* setup = CbmSetup::Instance();
  if ( ! setup->IsEmpty() ) {
  	std::cout << "-W- setup_align: overwriting existing setup"
  			      << setup->GetTitle() << std::endl;
  	setup->Clear();
  }
  setup->SetTitle("SIS100 - Electron Setup");
  setup->SetModule(kMagnet, magnetGeoTag);
  setup->SetModule(kPipe, pipeGeoTag);
//  setup->SetModule(kMvd, mvdGeoTag);
  setup->SetModule(kSts, stsGeoTag);
  setup->SetModule(kRich, richGeoTag);
//  setup->SetModule(kTrd, trdGeoTag);
//  setup->SetModule(kTof, tofGeoTag);
  setup->SetModule(kPlatform, platGeoTag);
//  setup->SetPsd(psdGeoFile, psdZpos, psdXpos);
  setup->SetField(fieldTag, fieldScale, 0., 0., fieldZ);
  // ------------------------------------------------------------------------


  // -----   Screen output   ------------------------------------------------
  setup->Print();
  // ------------------------------------------------------------------------

}
